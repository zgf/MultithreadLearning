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
		//���һ��CASPAIR��û�а汾��,��ô�����ĵ�һ�θ�ֵ���߹���,�ͻḴ��ͬһ���汾��,���߷����µİ汾��
		//���һ��CASPAIR�Ѿ����˰汾��,��ô�����ĸ�ֵ�������°汾��
		//�µ�����,���еĸ�ֵ�͹��춼�������°汾��,����lookup��ȡ���յĲ���
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