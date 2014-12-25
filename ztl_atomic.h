#pragma once
#include "forward.h"
namespace ztl
{
	/*
	等C++17 出了static if就可以编译过了- -...
	像VC那种实现写起来太痛苦了...
	*/
	using std::move;
	using std::is_integral;
	using std::is_pointer;
	using std::enable_if_t;
	namespace ztl_atomic
	{
		template<typename Type, size_t Byte>
		class AtomicImpl
		{
		};
		template<typename Type>
		class AtomicImpl<Type, 1U>
		{
		private:
			Type Load(volatile Type* src)noexcept
			{
				return _InterlockedExchange8((volatile char*) src, *src);
			}
		private:
			Type value;
		public:
			Type Load() const noexcept
			{
				return Load((volatile char*) &value);
			}
			Type Store(Type val)noexcept
			{
				return Exchange(val);
			}
			bool IsLockFree()const noexcept
			{
				return true;
			}
			Type Exchange(Type val)noexcept
			{
				return _InterlockedExchange8((volatile char*) &value, val);
			}
			operator Type() const noexcept
			{
				return Load();
			}
			bool CompareExchange(Type* exchange, Type comparand)noexcept
			{
				return _InterlockedCompareExchange8((volatile char*) &value, *exchange, comparand);
			}
			template<typename ExchangeType, size_t ExchangeByte>
			bool CompareExchange(AtomicImpl<ExchangeType, ExchangeByte>* exchange, Type comparand)noexcept
			{
				return _InterlockedCompareExchange8((volatile char*) &value, exchange->value, comparand);
			}
			//	if Type is integral or pointer
			enable_if_t<is_pointer<Type>::value || is_integral<Type>::value, Type> operator++()noexcept
			{
				return _InterlockedExchangeAdd8((volatile char)&value, 1);
			}
			enable_if_t<is_pointer<Type>::value || is_integral<Type>::value, Type> operator++(int)noexcept
			{
				return this->operator++();
			}
			enable_if_t<is_pointer<Type>::value || is_integral<Type>::value, Type> operator--()noexcept
			{
				return _InterlockedExchangeAdd8((volatile char) &value, -1);

			}
			enable_if_t<is_pointer<Type>::value || is_integral<Type>::value, Type> operator--(int)noexcept
			{
				return this->operator--();
			}

			//	if Type is integral

			enable_if_t<is_integral<Type>::value, Type> operator+= (Type val) noexcept
			{
				return FetchAdd(val);
			}
			enable_if_t<is_integral<Type>::value, Type> operator-= (Type val) noexcept
			{
				return FetchSub(val);
			}
			enable_if_t<is_integral<Type>::value, Type> operator&= (Type val) noexcept
			{
				return FetchAnd(val);
			}
			enable_if_t<is_integral<Type>::value, Type> operator|= (Type val) noexcept
			{
				return FetchOr(val);
			}
			enable_if_t<is_integral<Type>::value, Type> operator^= (Type val) noexcept
			{
				return FetchXor(val);
			}
			enable_if_t<is_integral<Type>::value, Type> FetchAdd(Type val)noexcept
			{
				return _InterlockedExchangeAdd8((volatile char*) &value, val);
			}
			enable_if_t<is_integral<Type>::value, Type> FetchSub(Type val)noexcept
			{
				return _InterlockedExchangeAdd8((volatile char*) &value, val);
			}
			enable_if_t<is_integral<Type>::value, Type> FetchOr(Type val)noexcept
			{
				return InterlockedOr8((volatile char*) &value, val);
			}
			enable_if_t<is_integral<Type>::value, Type> FetchXor(Type val)noexcept
			{
				return InterlockedXor8((volatile char*) &value, val);
			}
			enable_if_t<is_integral<Type>::value, Type> FetchAnd(Type val)noexcept
			{
				return InterlockedAnd8((volatile char*) &value, val);
			}
		public:
			/*
			构造析构拷贝赋值移动
			*/
			AtomicImpl(Type val) :value(val) noexcept
			{// x86硬件保证是原子性的
			}
			AtomicImpl() = default;
			~AtomicImpl() = default;
			AtomicImpl(const AtomicImpl&) = delete;
			AtomicImpl(AtomicImpl&&) = default;
			AtomicImpl& operator=(const AtomicImpl&) = delete;
			AtomicImpl& operator=(AtomicImpl&&) = default;
			AtomicImpl& operator=(Type val)
			{
				this->Store(val);
				return *this;
			}
		};

		template<typename Type>
		class AtomicImpl<Type, 2U>
		{
		private:
			Type Load(volatile Type* src)noexcept
			{
				return _InterlockedExchange16((volatile short*) src, *src);
			}
		private:
			Type value;
		public:
			Type Load() const noexcept
			{
				return Load((volatile short*) &value);
			}
			Type Store(Type val)noexcept
			{
				return Exchange(val);
			}
			bool IsLockFree()const noexcept
			{
				return true;
			}
			Type Exchange(Type val)noexcept
			{
				return _InterlockedExchange16((volatile short*) &value, val);
			}
			operator Type() const noexcept
			{
				return Load();
			}
			bool CompareExchange(Type* exchange, Type comparand)noexcept
			{
				return _InterlockedCompareExchange16((volatile short*) &value, *exchange, comparand);
			}
			template<typename ExchangeType, size_t ExchangeByte>
			bool CompareExchange(AtomicImpl<ExchangeType, ExchangeByte>* exchange, Type comparand)noexcept
			{
				return _InterlockedCompareExchange16((volatile short*) &value, exchange->value, comparand);
			}
			//	if Type is integral or pointer
			enable_if_t<is_pointer<Type>::value || is_integral<Type>::value, Type> operator++()noexcept
			{
				return _InterlockedIncrement16((volatile short*) &value);
			}
			enable_if_t<is_pointer<Type>::value || is_integral<Type>::value, Type> operator++(int)noexcept
			{
				return _InterlockedIncrement16((volatile short*) &value);
			}
			enable_if_t<is_pointer<Type>::value || is_integral<Type>::value, Type> operator--()noexcept
			{
				return _InterlockedDecrement16((volatile short*) &value);
			}
			enable_if_t<is_pointer<Type>::value || is_integral<Type>::value, Type> operator--(int)noexcept
			{
				return _InterlockedDecrement16((volatile short*) &value);
			}

			//	if Type is integral

			enable_if_t<is_integral<Type>::value, Type> operator+= (Type val) noexcept
			{
				return FetchAdd(val);
			}
			enable_if_t<is_integral<Type>::value, Type> operator-= (Type val) noexcept
			{
				return FetchSub(val);
			}
			enable_if_t<is_integral<Type>::value, Type> operator&= (Type val) noexcept
			{
				return FetchAnd(val);
			}
			enable_if_t<is_integral<Type>::value, Type> operator|= (Type val) noexcept
			{
				return FetchOr(val);
			}
			enable_if_t<is_integral<Type>::value, Type> operator^= (Type val) noexcept
			{
				return FetchXor(val);
			}
			enable_if_t<is_integral<Type>::value, Type> FetchAdd(Type val)noexcept
			{
				return _InterlockedExchangeAdd16((volatile short*) &value, val);
			}
			enable_if_t<is_integral<Type>::value, Type> FetchSub(Type val)noexcept
			{
				return _InterlockedExchangeAdd16((volatile short*) &value, -val);
			}
			enable_if_t<is_integral<Type>::value, Type> FetchOr(Type val)noexcept
			{
				return InterlockedOr16((volatile short*) &value, val);
			}
			enable_if_t<is_integral<Type>::value, Type> FetchXor(Type val)noexcept
			{
				return InterlockedXor16((volatile short*) &value, val);
			}
			enable_if_t<is_integral<Type>::value, Type> FetchAnd(Type val)noexcept
			{
				return InterlockedAnd16((volatile short*) &value, val);
			}
		public:
			/*
			构造析构拷贝赋值移动
			*/
			AtomicImpl(Type val) :value(val) noexcept
			{// x86硬件保证是原子性的
			}
			AtomicImpl() = default;
			~AtomicImpl() = default;
			AtomicImpl(const AtomicImpl&) = delete;
			AtomicImpl(AtomicImpl&&) = default;
			AtomicImpl& operator=(const AtomicImpl&) = delete;
			AtomicImpl& operator=(AtomicImpl&&) = default;
			AtomicImpl& operator=(Type val)
			{
				this->Store(val);
				return *this;
			}
		};

		template<typename Type>
		class AtomicImpl<Type, 4U>
		{
		private:
			Type Load(volatile Type* src)noexcept
			{
				return _InterlockedExchange((volatile long*) src, *src);
			}
		private:
			Type value;
		public:
			Type Load() const noexcept
			{
				return Load((volatile long*) &value);
			}
			Type Store(Type val)noexcept
			{
				return Exchange(val);
			}
			bool IsLockFree()const noexcept
			{
				return true;
			}
			Type Exchange(Type val)noexcept
			{
				return _InterlockedExchange((volatile long*) &value, val);
			}
			operator Type() const noexcept
			{
				return Load();
			}
			bool CompareExchange(Type* exchange, Type comparand)noexcept
			{
				return _InterlockedCompareExchange((volatile long*) &value, *exchange, comparand);
			}
			template<typename ExchangeType, size_t ExchangeByte>
			bool CompareExchange(AtomicImpl<ExchangeType, ExchangeByte>* exchange, Type comparand)noexcept
			{
				return _InterlockedCompareExchange((volatile long*) &value, exchange->value, comparand);
			}
			//	if Type is integral or pointer
			enable_if_t<is_pointer<Type>::value || is_integral<Type>::value, Type> operator++()noexcept
			{
				return _InterlockedIncrement((volatile long*) &value);
			}
			enable_if_t<is_pointer<Type>::value || is_integral<Type>::value, Type> operator++(int)noexcept
			{
				return _InterlockedIncrement((volatile long*) &value);
			}
			enable_if_t<is_pointer<Type>::value || is_integral<Type>::value, Type> operator--()noexcept
			{
				return _InterlockedDecrement((volatile long*) &value);
			}
			enable_if_t<is_pointer<Type>::value || is_integral<Type>::value, Type> operator--(int)noexcept
			{
				return _InterlockedDecrement((volatile long*) &value);
			}

			//	if Type is integral

			enable_if_t<is_integral<Type>::value, Type> operator+= (Type val) noexcept
			{
				return FetchAdd(val);
			}
			enable_if_t<is_integral<Type>::value, Type> operator-= (Type val) noexcept
			{
				return FetchSub(val);
			}
			enable_if_t<is_integral<Type>::value, Type> operator&= (Type val) noexcept
			{
				return FetchAnd(val);
			}
			enable_if_t<is_integral<Type>::value, Type> operator|= (Type val) noexcept
			{
				return FetchOr(val);
			}
			enable_if_t<is_integral<Type>::value, Type> operator^= (Type val) noexcept
			{
				return FetchXor(val);
			}
			enable_if_t<is_integral<Type>::value, Type> FetchAdd(Type val)noexcept
			{
				return InterlockedAdd((volatile long*) &value, val);
			}
			enable_if_t<is_integral<Type>::value, Type> FetchSub(Type val)noexcept
			{
				return InterlockedAdd((volatile long*) &value, -val);
			}
			enable_if_t<is_integral<Type>::value, Type> FetchOr(Type val)noexcept
			{
				return InterlockedOr((volatile long*) &value, val);
			}
			enable_if_t<is_integral<Type>::value, Type> FetchXor(Type val)noexcept
			{
				return InterlockedXor((volatile long*) &value, val);
			}
			enable_if_t<is_integral<Type>::value, Type> FetchAnd(Type val)noexcept
			{
				return InterlockedAnd((volatile long*) &value, val);
			}
			//if Type is pointer
			enable_if_t<is_pointer<Type>::value, Type> FetchAdd(ptrdiff_t val) noexcept
			{
				return InterlockedAdd((volatile long*) &value, val);
			}
			enable_if_t<is_pointer<Type>::value, Type> FetchSub(ptrdiff_t val) noexcept
			{
				return InterlockedAdd((volatile long*) &value, -val);
			}
			enable_if_t<is_pointer<Type>::value, Type> operator+= (ptrdiff_t val) noexcept
			{
				return FetchAdd(val);
			}
			enable_if_t<is_pointer<Type>::value, Type> operator-= (ptrdiff_t val) noexcept
			{
				return FetchSub(val);
			}
		public:
			/*
			构造析构拷贝赋值移动
			*/
			AtomicImpl(Type val) :value(val) noexcept
			{// x86硬件保证是原子性的
			}
			AtomicImpl() = default;
			~AtomicImpl() = default;
			AtomicImpl(const AtomicImpl&) = delete;
			AtomicImpl(AtomicImpl&&) = default;
			AtomicImpl& operator=(const AtomicImpl&) = delete;
			AtomicImpl& operator=(AtomicImpl&&) = default;
			AtomicImpl& operator=(Type val)
			{
				this->Store(val);
				return *this;
			}
		};

		template<typename Type>
		class AtomicImpl<Type, 8U>
		{
		private:
			Type Load(volatile Type* src)noexcept
			{
				return _InterlockedExchange64((volatile __int64 *) src, *src);
			}
		private:
			Type value;
		public:
			Type Load() const noexcept
			{
				return Load((volatile __int64*) &value);
			}
			Type Store(Type val)noexcept
			{
				return Exchange(val);
			}
			bool IsLockFree()const noexcept
			{
				return true;
			}
			Type Exchange(Type val)noexcept
			{
				return _InterlockedExchange64((volatile __int64*) &value, val);
			}
			operator Type() const noexcept
			{
				return Load();
			}
			bool CompareExchange(Type* exchange, Type comparand)noexcept
			{
				return _InterlockedCompareExchange64((volatile __int64*) &value, *exchange, comparand);
			}
			template<typename ExchangeType, size_t ExchangeByte>
			bool CompareExchange(AtomicImpl<ExchangeType, ExchangeByte>* exchange, Type comparand)noexcept
			{
				return _InterlockedCompareExchange64((volatile __int64*) &value, exchange->value, comparand);
			}
			//	if Type is integral or pointer
			enable_if_t<is_pointer<Type>::value || is_integral<Type>::value, Type> operator++()noexcept
			{
				return _InterlockedIncrement64((volatile __int64*) &value);
			}
			enable_if_t<is_pointer<Type>::value || is_integral<Type>::value, Type> operator++(int)noexcept
			{
				return _InterlockedIncrement64((volatile __int64*) &value);
			}
			enable_if_t<is_pointer<Type>::value || is_integral<Type>::value, Type> operator--()noexcept
			{
				return _InterlockedDecrement64((volatile __int64*) &value);
			}
			enable_if_t<is_pointer<Type>::value || is_integral<Type>::value, Type> operator--(int)noexcept
			{
				return _InterlockedDecrement64((volatile __int64*) &value);
			}

			//	if Type is integral

			enable_if_t<is_integral<Type>::value, Type> operator+= (Type val) noexcept
			{
				return FetchAdd(val);
			}
			enable_if_t<is_integral<Type>::value, Type> operator-= (Type val) noexcept
			{
				return FetchSub(val);
			}
			enable_if_t<is_integral<Type>::value, Type> operator&= (Type val) noexcept
			{
				return FetchAnd(val);
			}
			enable_if_t<is_integral<Type>::value, Type> operator|= (Type val) noexcept
			{
				return FetchOr(val);
			}
			enable_if_t<is_integral<Type>::value, Type> operator^= (Type val) noexcept
			{
				return FetchXor(val);
			}
			enable_if_t<is_integral<Type>::value, Type> FetchAdd(Type val)noexcept
			{
				return InterlockedAdd64((volatile __int64*) &value, val);
			}
			enable_if_t<is_integral<Type>::value, Type> FetchSub(Type val)noexcept
			{
				return InterlockedAdd64((volatile __int64*) &value, -val);
			}
			enable_if_t<is_integral<Type>::value, Type> FetchOr(Type val)noexcept
			{
				return InterlockedOr64((volatile __int64*) &value, val);
			}
			enable_if_t<is_integral<Type>::value, Type> FetchXor(Type val)noexcept
			{
				return InterlockedXor64((volatile __int64*) &value, val);
			}
			enable_if_t<is_integral<Type>::value, Type> FetchAnd(Type val)noexcept
			{
				return InterlockedAnd64((volatile __int64*) &value, val);
			}
			//if Type is pointer
			enable_if_t<is_pointer<Type>::value, Type> FetchAdd(ptrdiff_t val) noexcept
			{
				return InterlockedAdd64((volatile __int64*) &value, val);
			}
			enable_if_t<is_pointer<Type>::value, Type> FetchSub(ptrdiff_t val) noexcept
			{
				return InterlockedAdd64((volatile __int64*) &value, -val);
			}
			enable_if_t<is_pointer<Type>::value, Type> operator+= (ptrdiff_t val) noexcept
			{
				return FetchAdd(val);
			}
			enable_if_t<is_pointer<Type>::value, Type> operator-= (ptrdiff_t val) noexcept
			{
				return FetchSub(val);
			}
		public:
			/*
			构造析构拷贝赋值移动
			*/
			AtomicImpl(Type val) :value(val) noexcept
			{// x86硬件保证是原子性的
			}
			AtomicImpl() = default;
			~AtomicImpl() = default;
			AtomicImpl(const AtomicImpl&) = delete;
			AtomicImpl(AtomicImpl&&) = default;
			AtomicImpl& operator=(const AtomicImpl&) = delete;
			AtomicImpl& operator=(AtomicImpl&&) = default;
			AtomicImpl& operator=(Type val)
			{
				this->Store(val);
				return *this;
			}
		};
		template<typename Type>
		class Atomic :public AtomicImpl<Type,sizeof(Type)>
		{
			using AtomicImpl<Type,sizeof(Type)>::AtomicImpl;
		};
	}
}