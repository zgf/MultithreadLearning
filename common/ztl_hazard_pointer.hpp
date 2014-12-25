#pragma once
#include "../forward.h"
#pragma warning(disable:4324) //提示64字节对齐需要填充数据而已
namespace ztl
{
	namespace hazard_pointer
	{
		

		template<typename PointerType,size_t SlotNumber=1>
		class HazardPointers
		{
		public:
			//和缓存线对齐
			__declspec(align(64)) struct HazardNode
			{
				std::array<atomic<PointerType>, SlotNumber> current;
			};
			using HazardNodePointer = HazardNode*;
			//////////////////////////////////////////////////////////////////////////
			//这个类是用作线程局部存储的,所以不必考虑线程安全,反正都是单线程
			//////////////////////////////////////////////////////////////////////////
			template<typename PointerType>
			class RetireListType
			{
			public:
				RetireListType() = default;
				RetireListType(HazardNodePointer _saveHead, HazardNodePointer _saveEnd)
					:saveHead(_saveHead),saveEnd(_saveEnd)
				{

				}
				RetireListType(RetireListType&&) = default;
				RetireListType(const RetireListType&) = default;
				RetireListType& operator=(const RetireListType&) = default;
				RetireListType& operator=(RetireListType&&) = default;
				~RetireListType()
				{
					Retire();
				}
				void Retire()
				{
					std::vector<PointerType> result;
					std::vector<PointerType> save;
					auto&& lookup = CollectPointer();
					std::sort(lookup.begin(), lookup.end());
					std::sort(retireList.begin(), retireList.end());
					for (auto&& iter:retireList)
					{
						if (find(lookup.begin(),lookup.end(),iter)!=lookup.end())
						{
							save.push_back(iter);
							std::swap(retireList.back(), iter);
							retireList.pop_back();
						}
					}
					//这里效率可优化.不过用库比较方便
					//std::set_intersection(retireList.begin(), retireList.end(), lookup.begin(), lookup.end(), std::inserter(save, save.begin()));
					//std::set_difference(retireList.begin(), retireList.end(), lookup.begin(), lookup.end(), std::inserter(result, result.begin()));
					if (!retireList.empty())
					{
						for(auto&& iter : retireList)
						{
							//delete iter;
						}
					}
					
					retireList.swap(save);
				}
			public:
				template<typename Type>
				void Regsiter(Type&& value)
				{
					retireList.emplace_back(std::forward<Type>(value));
					size++;
				}
			
				size_t Size() const 
				{
					return size;
				}
				//为了兼容范围for 所以小写
				decltype(auto) begin()
				{
					return retireList.begin();
				}
				decltype(auto)  end()
				{
					return retireList.end();
				}
				
			private:
				std::vector<PointerType> CollectPointer()
				{
					std::vector<PointerType> result;
					for(HazardNodePointer worker = saveHead; worker < saveEnd; worker++)
					{
						for(auto i = 0; i < SlotNumber;++i)
						{
							if(worker->current[i].load().current != nullptr)
							{
								result.push_back(worker->current[i].load());
							}
						}
					
					}
					return result;
				}
				std::vector<PointerType> retireList;
				atomic<size_t> size;
				HazardNodePointer saveHead;
				HazardNodePointer saveEnd;
				
			};

		public:
			HazardPointers() = default;
			HazardPointers(unsigned int _maxThreadNumber, unsigned int _gcNumber)
				:gcNumber(_gcNumber), maxThreadNumber(_maxThreadNumber), 
				head((HazardNodePointer)_aligned_malloc(sizeof(HazardNode)*_maxThreadNumber,64)), end(head + _maxThreadNumber)
			{
				memset(head, 0, _maxThreadNumber*sizeof(HazardNode));
			}
			HazardPointers(HazardPointers&&) = default;
			HazardPointers(const HazardPointers&) = default;
			HazardPointers& operator=(const HazardPointers&) = default;
			HazardPointers& operator=(HazardPointers&&) = default;
			~HazardPointers()
			{
				_aligned_free(head);
			}
			//从数组中找到一个空白槽位
			PointerType* Acquire()volatile
			{

				for(HazardNodePointer  worker = head.load(); worker != end.load(); worker++)
				{
					for(auto i = 0; i < SlotNumber; ++i)
					{
						if(worker->current[i].load() == nullptr)
						{
							return (PointerType*)(void*) & worker->current[i];
						}
					}
				}
				throw std::string("error!");
			}
			void Release(PointerType*  pointer)volatile
			{	
				*pointer = PointerType(nullptr);
			}
			void Retire(PointerType pointer )volatile
			{
				thread_local RetireListType<PointerType> retireList;
				retireList.Regsiter(pointer);
				if (retireList.Size() > gcNumber)
				{
					retireList.Retire();
				}
			}
		private:
			atomic<unsigned int> maxThreadNumber;
			atomic<HazardNodePointer>  head;
			atomic<HazardNodePointer>  end;
			atomic<unsigned int> gcNumber;
		};
	}
}