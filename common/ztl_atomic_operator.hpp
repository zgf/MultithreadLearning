#pragma once
#include <type_traits>
#include <iterator>
#include <Windows.h>
namespace ztl
{
	namespace atomic_opertaor
	{
		using std::forward;
		namespace detail
		{
			
			template<size_t Byte>
			struct SizeToVolatileType
			{

			};
			template<>
			struct SizeToVolatileType<1>
			{
				using type = volatile unsigned char;
			};
			template<>
			struct SizeToVolatileType<2>
			{
				using type = volatile unsigned short;
			};
			template<>
			struct SizeToVolatileType<4>
			{
				using type = volatile unsigned long;
			};
			template<>
			struct SizeToVolatileType<8>
			{
				using type = volatile LONG64;
			};
			//64°´8×ª»»
			template<>
			struct SizeToVolatileType<64>
			{
				using type = volatile LONG64;
			};
			decltype(auto) AtomicIncrementImpl(short volatile *Addend, std::integral_constant<int, 2>)
			{
				return InterlockedIncrement16(Addend);
			}
			
			decltype(auto) AtomicIncrementImpl(volatile long*Addend, std::integral_constant<int, 4>)
			{
				return InterlockedIncrement(Addend);
			}

			decltype(auto) AtomicIncrementImpl(volatile int*Addend, std::integral_constant<int, 4>)
			{
				static_assert(sizeof(int) == sizeof(long),"error size int != size long");
				return InterlockedIncrement((volatile long*)Addend);
			}
			decltype(auto) AtomicIncrementImpl(volatile unsigned int*Addend, std::integral_constant<int, 4>)
			{
				return InterlockedIncrement(Addend);
			}
			decltype(auto) AtomicIncrementImpl(volatile unsigned long*Addend, std::integral_constant<int, 4>)
			{
				return InterlockedIncrement(Addend);
			}
			decltype(auto) AtomicIncrementImpl(volatile unsigned long long *Addend, std::integral_constant<int, 4>)
			{
				return InterlockedIncrement(Addend);
			}
			decltype(auto) AtomicIncrementImpl(volatile LONG64 *Addend, std::integral_constant<int, 8>)
			{
				return InterlockedIncrement64(Addend);
			}



			decltype(auto) AtomicDecrementImpl(volatile short  *Addend, std::integral_constant<int, 2>)
			{
				return InterlockedDecrement16(Addend);
			}
			decltype(auto) AtomicDecrementImpl(volatile long*Addend, std::integral_constant<int, 4>)
			{
				return InterlockedDecrement(Addend);
			}
			decltype(auto) AtomicDecrementImpl(volatile int*Addend, std::integral_constant<int, 4>)
			{
				static_assert(sizeof(int) == sizeof(long), "error size int != size long");
				return InterlockedDecrement((volatile long*)Addend);
			}
			decltype(auto) AtomicDecrementImpl(volatile unsigned int*Addend, std::integral_constant<int, 4>)
			{
				return InterlockedDecrement(Addend);
			}
			decltype(auto) AtomicDecrementImpl(volatile unsigned long*Addend, std::integral_constant<int, 4>)
			{
				return InterlockedDecrement(Addend);
			}
			decltype(auto) AtomicDecrementImpl(volatile unsigned long long *Addend, std::integral_constant<int, 4>)
			{
				return InterlockedDecrement(Addend);
			}
			decltype(auto) AtomicDecrementImpl(volatile LONG64 *Addend, std::integral_constant<int, 8>)
			{
				return InterlockedDecrement64(Addend);
			}


			decltype(auto) AtomicCompareExchangeImpl(volatile char  *Destination, char ExChange, char Comperand, std::integral_constant<int, 1>)
			{
				return _InterlockedCompareExchange8(Destination, ExChange, Comperand);
			}
			decltype(auto) AtomicCompareExchangeImpl(volatile short  *Destination, short ExChange, short Comperand, std::integral_constant<int, 2>)
			{
				return InterlockedCompareExchange16(Destination, ExChange, Comperand);
			}
			decltype(auto) AtomicCompareExchangeImpl(volatile long*Destination, long ExChange, long Comperand, std::integral_constant<int, 4>)
			{
				return InterlockedCompareExchange(Destination, ExChange, Comperand);
			}
			decltype(auto) AtomicCompareExchangeImpl(volatile int*Destination, int ExChange, int Comperand, std::integral_constant<int, 4>)
			{
				static_assert(sizeof(int) == sizeof(long), "error size int != size long");

				return InterlockedCompareExchange((volatile long*)Destination, (long)ExChange, (long)Comperand);
			}
			decltype(auto) AtomicCompareExchangeImpl(volatile unsigned int*Destination, unsigned int ExChange, unsigned int Comperand, std::integral_constant<int, 4>)
			{
				return InterlockedCompareExchange(Destination, ExChange, Comperand);
			}
			decltype(auto) AtomicCompareExchangeImpl(volatile unsigned long*Destination, unsigned int ExChange, unsigned int Comperand, std::integral_constant<int, 4>)
			{
				return InterlockedCompareExchange(Destination, ExChange, Comperand);
			}
			decltype(auto) AtomicCompareExchangeImpl(volatile unsigned long long *Destination, unsigned int ExChange, unsigned int Comperand, std::integral_constant<int, 4>)
			{
				return InterlockedCompareExchange(Destination, ExChange, Comperand);
			}
			
			decltype(auto) AtomicCompareExchangeImpl(volatile LONG64*Destination, LONG64 ExChange, LONG64 Comperand, std::integral_constant<int, 8>)
			{
				return InterlockedCompareExchange64(Destination, ExChange, Comperand);
			}

			decltype(auto) AtomicExchangeImpl(volatile char  *Destination, char ExChange, std::integral_constant<int, 1>)
			{
				return InterlockedExchange8(Destination, ExChange);
			}
			decltype(auto) AtomicExchangeImpl(volatile short  *Destination, short ExChange, std::integral_constant<int, 2>)
			{
				return InterlockedExchange16(Destination, ExChange);
			}
			decltype(auto) AtomicExchangeImpl(volatile long*Destination, long ExChange, std::integral_constant<int, 4>)
			{
				return InterlockedExchange(Destination, ExChange);
			}
			decltype(auto) AtomicExchangeImpl(volatile int*Destination, int ExChange, std::integral_constant<int, 4>)
			{
				static_assert(sizeof(int) == sizeof(long), "error size int != size long");

				return InterlockedExchange((volatile long*)Destination, (long)ExChange);
			}
			decltype(auto) AtomicExchangeImpl(volatile unsigned int*Destination, unsigned int ExChange, std::integral_constant<int, 4>)
			{
				return InterlockedExchange(Destination, ExChange);
			}
			decltype(auto) AtomicExchangeImpl(volatile unsigned long*Destination, unsigned int ExChange, std::integral_constant<int, 4>)
			{
				return InterlockedExchange(Destination, ExChange);
			}
			decltype(auto) AtomicExchangeImpl(volatile unsigned long long *Destination, unsigned int ExChange, std::integral_constant<int, 4>)
			{
				return InterlockedExchange(Destination, ExChange);
			}
			decltype(auto) AtomicExchangeImpl(volatile LONG64*Destination, LONG64 ExChange, std::integral_constant<int, 8>)
			{
				return InterlockedExchange64(Destination, ExChange);
			}
			template<typename Type>
			decltype(auto) AtomicIncrementTypeDispath(Type&& Addend,std::true_type)
			{
				using ValueType = typename std::iterator_traits< std::remove_reference_t<Type>>::value_type;
				constexpr const int Byte = sizeof(ValueType);
				using VolatileValueType = std::add_volatile_t<ValueType>;
				using PointerType =typename std::add_pointer<VolatileValueType>::type;
				return detail::AtomicIncrementImpl(static_cast<PointerType>(Addend), std::integral_constant<int, Byte>());
			}
			template<typename Type>
			decltype(auto) AtomicIncrementTypeDispath(Type&& Addend, std::false_type)
			{
				using ValueType = typename std::iterator_traits<std::remove_reference_t<Type>>::value_type;
				constexpr const int Byte = sizeof(ValueType);
				using ConvertType =typename  SizeToVolatileType<Byte>::type*;
				return detail::AtomicIncrementImpl(reinterpret_cast<ConvertType>(Addend), std::integral_constant<int, Byte>());
			}
			template<typename Type>
			decltype(auto) AtomicDecrementTypeDispath(Type&& Addend, std::true_type)
			{
				using NonRefType = std::remove_reference_t<Type>;
				using ValueType = typename std::iterator_traits<NonRefType>::value_type;
				constexpr const int Byte = sizeof(ValueType);
				using VolatileValueType = std::add_volatile_t<ValueType>;
				using PointerType = typename std::add_pointer<VolatileValueType>::type;
				return detail::AtomicDecrementImpl(static_cast<PointerType>(Addend), std::integral_constant<int, Byte>());
			}
			template<typename Type>
			decltype(auto) AtomicDecrementTypeDispath(Type&& Addend, std::false_type)
			{
				using ValueType = typename std::iterator_traits<std::remove_reference_t<Type>>::value_type;
				constexpr const int Byte = sizeof(ValueType);
				using ConvertType = typename  SizeToVolatileType<Byte>::type*;
				return detail::AtomicDecrementImpl(reinterpret_cast<ConvertType>(Addend), std::integral_constant<int, Byte>());
			}
			
			template<typename AddressType, typename CompareTypeLeft, typename CompareTypeRight >
			decltype(auto) AtomicCompareExchangeTypeDispath(AddressType&& Destination, CompareTypeLeft&&ExChange, CompareTypeRight&&Comperand, std::false_type)
			{
				using ValueType = std::remove_volatile_t<typename std::iterator_traits<std::remove_reference_t<AddressType>>::value_type>;
				using PointerType = ValueType*;
				constexpr const int Byte = sizeof(ValueType);


				using ConvertValueType = typename  SizeToVolatileType<Byte>::type;
				using CompareValueType = std::remove_volatile_t<ConvertValueType>;
				using ConvertPointerType = ConvertValueType*;
				return detail::AtomicCompareExchangeImpl(reinterpret_cast<ConvertPointerType>((void*) Destination), *reinterpret_cast<CompareValueType*>((void*)&ExChange), *reinterpret_cast<CompareValueType*>((void*)&Comperand), typename std::integral_constant<int, Byte>());
			}
			template<typename AddressType, typename CompareTypeLeft, typename CompareTypeRight >
			decltype(auto) AtomicCompareExchangeTypeDispath(AddressType&& Destination, CompareTypeLeft&&ExChange, CompareTypeRight&&Comperand,std::true_type)
			{
				using NonRefType = std::remove_reference_t<AddressType>;
				using ValueType = std::remove_volatile_t<typename std::iterator_traits<NonRefType>::value_type>;
				constexpr const int Byte = sizeof(ValueType);
				using VolatileValueType = std::add_volatile_t<ValueType>;
				using ConvertType = typename std::add_pointer<VolatileValueType>::type;
				using PointerType = ValueType*;
				return detail::AtomicCompareExchangeImpl(reinterpret_cast<ConvertType>(Destination), *reinterpret_cast<PointerType>((void*) &ExChange), *reinterpret_cast<PointerType>((void*) &Comperand), typename std::integral_constant<int, Byte>());
			}
			template<typename AddressType, typename CompareTypeLeft>
			decltype(auto) AtomicExchangeTypeDispath(AddressType&& Destination, CompareTypeLeft&&ExChange, std::true_type)
			{
				using NonRefType = std::remove_reference_t<AddressType>;
				using ValueType = std::remove_volatile_t<typename std::iterator_traits<NonRefType>::value_type>;
				constexpr const int Byte = sizeof(ValueType);
				using VolatileValueType = std::add_volatile_t<ValueType>;
				using ConvertType = typename std::add_pointer<VolatileValueType>::type;
				using PointerType = ValueType*;
				return detail::AtomicExchangeImpl(reinterpret_cast<ConvertType>(Destination), *reinterpret_cast<PointerType>((void*) &ExChange), typename std::integral_constant<int, Byte>());
			}
			template<typename AddressType, typename CompareTypeLeft>
			decltype(auto) AtomicExchangeTypeDispath(AddressType&& Destination, CompareTypeLeft&&ExChange, std::false_type)
			{
				using ValueType = std::remove_volatile_t<typename std::iterator_traits<std::remove_reference_t<AddressType>>::value_type>;
				using PointerType = ValueType*;
				constexpr const int Byte = sizeof(ValueType);


				using ConvertValueType = typename  SizeToVolatileType<Byte>::type;
				using CompareValueType = std::remove_volatile_t<ConvertValueType>;
				using ConvertPointerType = ConvertValueType*;
				return detail::AtomicExchangeImpl(reinterpret_cast<ConvertPointerType>((void*) Destination), *reinterpret_cast<CompareValueType*>((void*) &ExChange), typename std::integral_constant<int, Byte>());
			}
		}
		
		template<typename Type>
		decltype(auto) AtomicIncrement(Type&& Addend)
		{
			using NonRefType = std::remove_reference_t<Type>;
			using ValueType = typename std::iterator_traits<NonRefType>::value_type;
			static_assert(std::is_pointer<NonRefType>::value, "error!Type isn't pointer");
			static_assert(sizeof(Type) < 64, "error! atomic Increment need Type size < 64");
			static_assert(sizeof(Type) != 1, "error! InterlockedIncrement not support 1 byte");
			return detail::AtomicIncrementTypeDispath(std::forward<Type>(Addend),typename std::is_integral<ValueType>::type());
		}

		
		template<typename Type>
		decltype(auto) AtomicDecrement(Type&& Addend)
		{
			using NonRefType = std::remove_reference_t<Type>;
			using ValueType = typename std::iterator_traits<NonRefType>::value_type;
			static_assert(std::is_pointer<NonRefType>::value, "error!Type isn't pointer");
			static_assert(sizeof(Type) < 64, "error! atomic Decrement need Type size < 64");
			static_assert(sizeof(Type) != 1, "error! InterlockedDecrement not support 1 byte");
			return detail::AtomicDecrementTypeDispath(std::forward<Type>(Addend), typename std::is_integral<ValueType>::type());
		}
		
		template<typename AddressType,typename CompareTypeLeft,typename CompareTypeRight >
		decltype(auto) AtomicCompareExchange(AddressType&& Destination, CompareTypeLeft&&ExChange, CompareTypeRight&&Comperand)
		{
			using NonRefType = std::remove_reference_t<AddressType>;
			using ValueType = typename std::iterator_traits<NonRefType>::value_type;
			static_assert(std::is_pointer<NonRefType>::value, "error!Type isn't pointer");
			static_assert(sizeof(AddressType) < 64, "error! atomic Decrement need Type size < 64");
			static_assert(sizeof(AddressType) != 1, "error! InterlockedDecrement not support 1 byte");
			return detail::AtomicCompareExchangeTypeDispath(std::forward<AddressType>(Destination), std::forward<CompareTypeLeft>(ExChange), std::forward<CompareTypeRight>(Comperand), typename std::is_integral<ValueType>::type());
		}
		template<typename AddressType, typename CompareTypeLeft, typename CompareTypeRight >
		decltype(auto) AtomicCompareExchangePointer(AddressType&& Destination, CompareTypeLeft&&ExChange, CompareTypeRight&&Comperand)
		{
			using NonRefAddressType = std::remove_reference_t<AddressType>;
			using NonRefCompareType = std::remove_reference_t<CompareTypeLeft>;
			static_assert(std::is_same<NonRefAddressType,void* volatile *>::value, "AddressType != void* volatile *");
			static_assert(std::is_same<NonRefCompareType, void*>::value, "CompareTypeLeft != void*");
			return InterlockedCompareExchangePointer(std::forward<AddressType>(Destination), std::forward<CompareTypeLeft>(ExChange), std::forward<CompareTypeRight>(Comperand));
		}

		template<typename AddressType, typename CompareTypeLeft>
		decltype(auto) AtomicExchange(AddressType&& Destination, CompareTypeLeft&&ExChange)
		{
			using NonRefType = std::remove_reference_t<AddressType>;
			using ValueType = typename std::iterator_traits<NonRefType>::value_type;
			static_assert(std::is_pointer<NonRefType>::value, "error!Type isn't pointer");
			static_assert(sizeof(AddressType) < 64, "error! atomic Decrement need Type size < 64");
			static_assert(sizeof(AddressType) != 1, "error! InterlockedDecrement not support 1 byte");
			return detail::AtomicExchangeTypeDispath(std::forward<AddressType>(Destination), std::forward<CompareTypeLeft>(ExChange), typename std::is_integral<ValueType>::type());
		}
		template<typename AddressType, typename CompareTypeLeft,typename CompareTypeRight>
		bool CASCompareExchange(AddressType&& Destination, CompareTypeLeft&&ExChange, CompareTypeRight&&Comperand)
		{
			using ResultType = decltype(AtomicCompareExchange(forward<AddressType>(Destination), forward<CompareTypeLeft>(ExChange), forward<CompareTypeRight>(Comperand)));
			return AtomicCompareExchange(forward<AddressType>(Destination), forward<CompareTypeLeft>(ExChange), forward<CompareTypeRight>(Comperand)) == *reinterpret_cast<ResultType*>((void*)&Comperand);
		}
		
	}
}