#pragma once
namespace ztl
{
	namespace ztl_guard_locker
	{
		template<typename LockType>
		class GuardLocker
		{
		public:
			GuardLocker(LockType& _lock) :lock(_lock)
			{
				lock.Lock();
			}
			~GuardLocker()
			{
				lock.Unlock();
			}
			GuardLocker() = delete;
			GuardLocker(GuardLocker&&) = default;
			GuardLocker(const GuardLocker&) = delete;
			GuardLocker& operator=(GuardLocker&&) = default;
			GuardLocker& operator=(const GuardLocker&) = delete;
			
		private:
			LockType& lock;
		};
		template<typename LockFunctorType,typename UnLockFunctorType>
		class GuardLockerFunctor
		{
		public:
			GuardLockerFunctor(const LockFunctorType& lock,const UnLockFunctorType& _unlock) :unlock(_unlock)
			{
				lock();
			}
			~GuardLockerFunctor()
			{
				unlock();
			}
		
			GuardLockerFunctor() = delete;
			GuardLockerFunctor(GuardLockerFunctor&&) = default;
			GuardLockerFunctor(const GuardLockerFunctor&) = delete;
			GuardLockerFunctor& operator=(GuardLockerFunctor&&) = default;
			GuardLockerFunctor& operator=(const GuardLockerFunctor&) = delete;

		private:
			UnLockFunctorType unlock;
		};
		template<typename LockerFunctorType,typename UnLockFunctorType>
		decltype(auto) MakeGuardLocker(LockerFunctorType&& lock, UnLockFunctorType&& unlock)
		{
			return GuardLockerFunctor<std::remove_reference_t<LockerFunctorType>,
				std::remove_reference_t<UnLockFunctorType>>
				(std::forward<LockerFunctorType>(lock),
				std::forward<UnLockFunctorType>(unlock));
		}
		template<typename LockType>
		decltype(auto) MakeGuardLocker(LockType&& unlock )
		{
			return GuardLocker<std::remove_reference_t<LockType>>(std::forward<LockType>(unlock));
		}
	}
}