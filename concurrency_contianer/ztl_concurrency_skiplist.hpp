#pragma once
#include "../forward.h"
#include "../common/ztl_atomic_shared_ptr.hpp"
namespace ztl
{
	namespace concurrency_skiplist
	{
		using atomic_shared_ptr::atomic_shared_pointer;
		template<typename ValueType,bool IsAtomicType,size_t MaxLevelNumber>
		class ConcurrencySkipListImpl
		{
			struct SkipNode
			{
				ValueType value;
				std::vector<atomic_shared_pointer<SkipNode>> nexts;
			public:
				SkipNode() = default;
				SkipNode(const ValueType& _value) :value(_value)
				{
					nexts.reserve(MaxLevelNumber);
				}
				SkipNode(SkipNode&&) = default;
				SkipNode(const SkipNode&) = default;
				~SkipNode() = default;
				SkipNode& operator=(SkipNode&&) = default;
				SkipNode& operator=(const SkipNode&) = default;
			};
		public:
			ConcurrencySkipListImpl()
			{
				head = make_shared<SkipNode>();
			}
			//为了避免一次性析构太多shared导致栈溢出.这里析构的时候自己拆分下
			~ConcurrencySkipListImpl()
			{
				

			}
			ConcurrencySkipListImpl(ConcurrencySkipListImpl&&) = default;
			ConcurrencySkipListImpl(const ConcurrencySkipListImpl&) = delete;
			ConcurrencySkipListImpl& operator=(const ConcurrencySkipListImpl&) = delete;
			ConcurrencySkipListImpl& operator=(ConcurrencySkipListImpl&&) = default;
		public:
			void Insert(const ValueType& value)
			{
				
			}
			bool Remove(ValueType& value)
			{

			}
		private:
			atomic_shared_pointer<SkipNode> head;
		};
		template<typename ValueType>
		class ConcurrencySkipList: public ConcurrencySkipListImpl<ValueType,(sizeof(ValueType)< 64)>
		{
			using ConcurrencySkipListImpl < ValueType, (sizeof(ValueType) < 64),5 > ::ConcurrencySkipListImpl;
		};
	}
}