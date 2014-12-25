#pragma once
#include "ztl_atomic_operator.hpp"
namespace ztl
{
	namespace stamp_pair
	{
		using atomic_opertaor::AtomicIncrement;
		using atomic_opertaor::AtomicCompareExchange;
		using atomic_opertaor::AtomicExchange;
		using atomic_opertaor::CASCompareExchange;
		/*template<typename ElementType, typename StampType>
		struct LookUpStampPair
		{
			ElementType current= ElementType();
			StampType   id= StampType();
		};*/
		//如果一个CASPAIR还没有版本号,那么对他的第一次赋值或者构造,就会复制同一个版本号,或者分配新的版本号
		//如果一个CASPAIR已经有了版本号,那么对他的赋值会生成新版本号
		//新的做法,所有的赋值和构造都会生成新版本号,除了lookup获取快照的操作
		template<typename ElementType,typename StampType>
		struct StampPair
		{
			static_assert(std::is_integral<StampType>::value, "id Must be IntegralType");
			static_assert(sizeof(ElementType)+sizeof(StampType)<64, "StampPair size Must <64");
			ElementType current = ElementType();
			StampType   id = StampType();
			StampPair() = default;
			StampPair(const ElementType& target) :current(target), id(AtomicIncrement(&version))
			{

			}
			StampPair(std::nullptr_t) :current(ElementType()), id(StampType())
			{

			}
			~StampPair() = default;
			StampPair(StampPair&& target) = default;

			StampPair(const StampPair& target) = default;

			StampPair& operator=(const StampPair& target) = default;

			StampPair& operator=(StampPair&& target) = default;

			StampPair& operator=(const ElementType& target)
			{
				current = target;
				id = AtomicIncrement(&version);
				return *this;
			}
			
			void operator=(std::nullptr_t)
			{
				current = 0;
				id = 0;
			}
			operator void*()const
			{
				return current;
			}
			bool operator==(std::nullptr_t)const
			{
				return id == 0 && current == 0;
			}
			bool operator!=(std::nullptr_t)const
			{
				return !this->operator==(nullptr);
			}
			bool operator==(const StampPair&right)const
			{
				return current == right.current&& id == right.id;
			}
			bool operator!=(const StampPair& right)const
			{
				return !(*this == right);
			}
		private:
			static int version;
		};
		template<typename ElementType, typename StampType>
		int StampPair<ElementType, StampType>::version = 0;
	}
}