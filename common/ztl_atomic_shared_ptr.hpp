#pragma once
#include "../forward.h"
namespace ztl
{
	namespace atomic_shared_ptr
	{
		template<typename value_type>
		class atomic_shared_pointer
		{
			shared_ptr<value_type> pointer=nullptr;
		public:
			atomic_shared_pointer() = default;
			atomic_shared_pointer(atomic_shared_pointer&&) = default;
			atomic_shared_pointer(const atomic_shared_pointer&) = delete;
			~atomic_shared_pointer()
			{
			}
			atomic_shared_pointer& operator=(atomic_shared_pointer&&) = default;
			atomic_shared_pointer& operator=(const atomic_shared_pointer&) = delete;
			atomic_shared_pointer(const shared_ptr<value_type>& value)
			{
				std::atomic_store(&pointer, value);
			}
			void operator=(const shared_ptr<value_type>& value)
			{
				std::atomic_store(&pointer, value);
			}
		public:
			shared_ptr<value_type> load()
			{
				return std::atomic_load(&pointer);
			}
			decltype(auto) store(const shared_ptr<value_type>& value)
			{
				return std::atomic_store(&pointer, value);
			}
			decltype(auto) compare_exchange_strong(shared_ptr<value_type>& expact,const shared_ptr<value_type>& value)
			{
				return std::atomic_compare_exchange_strong(&pointer, &expact, value);
			}
			decltype(auto) compare_exchange_weak(shared_ptr<value_type>& expact,const shared_ptr<value_type>& value)
			{
				return std::atomic_compare_exchange_weak(&pointer, &expact, value);
			}
			decltype(auto) exchange(const shared_ptr<value_type>& value)
			{
				return std::atomic_exchange(&pointer, value);
			}
			decltype(auto) is_lock_free()const
			{
				return std::atomic_is_lock_free(&pointer);
			}
			friend bool operator==(const atomic_shared_pointer<value_type>& left, nullptr_t)const 
			{
				return left.load() == nullptr;
			}
			friend bool operator!=(const atomic_shared_pointer<value_type>& left, nullptr_t)const
			{
				return !(left == nullptr);
			}
			friend bool operator==(const atomic_shared_pointer<value_type>& left, const atomic_shared_pointer<value_type>& right)const
			{
				return left.load() == right.load();
			}
			friend bool operator!=(const atomic_shared_pointer<value_type>& left, const atomic_shared_pointer<value_type>& right)const
			{
				return !(left == right);
			}
		};
	}
}