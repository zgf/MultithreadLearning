#pragma once
#include "forward.h"
#include "mutex/ztl_exclusive_mutex.hpp"
#include "ztl_condition_variable.h"
namespace ztl
{
	namespace ztl_semaphore
	{
		class Semaphore
		{
		public:
			Semaphore() = delete;
			Semaphore(int number):slot(number)
			{

			}
			~Semaphore() = default;
			Semaphore(Semaphore&&) = default;
			Semaphore(const Semaphore&) = delete;
			Semaphore& operator=(const Semaphore&) = delete;
			Semaphore& operator=(Semaphore&&) = default;
		public:
			void Wait()
			{
				locker.Lock();
				while (slot == 0)
				{
					conditionVariable.Wait(locker);
				}
				--slot;
				locker.Unlock();
			}
			template <class Rep, class Period, class Predicate>
			bool WaitFor(const duration<Rep, Period>& rel_time)
			{
				locker.Lock();
				while(slot == 0)
				{
					if(!conditionVariable.WaitFor(locker, rel_time))
					{
						locker.Unlock();
						return false;
					}
				}
				--slot;
				locker.Unlock();
				return true;
			}
			template <class Clock, class Duration, class Predicate>
			bool WaitUntil( const time_point<Clock, Duration>& abs_time)
			{
				locker.Lock();
				while(slot == 0)
				{
					if(!conditionVariable.WaitUntil(locker, abs_time))
					{
						locker.Unlock();
						return false;
					}
				}
				--slot;
				locker.Unlock();
				return true;
			}
			void Signal()
			{
				locker.Lock();
				++slot;
				conditionVariable.NotifyOne();
				locker.Unlock();
			}

			
		private:
			int slot;
			ztl::exclusive_mutex::Mutex locker;
			ztl::condition_variable::ConditionVariable conditionVariable;
		};
	}
}