#pragma once
#include "forward.h"
#include "mutex/ztl_exclusive_mutex.hpp"
namespace ztl
{
	namespace condition_variable
	{
		class ConditionVariable
		{
		public:
			ConditionVariable() = default;
			~ConditionVariable()
			{
				HANDLE expect = NULL;
				event.compare_exchange_strong(expect, NULL);
				CloseHandle(expect);
			}
			ConditionVariable(ConditionVariable&&) = default;
			ConditionVariable(const ConditionVariable&) = delete;
			ConditionVariable& operator=(const ConditionVariable&) = delete;
			ConditionVariable& operator=(ConditionVariable&&) = default;
		public:
			template<class Predicate>
			void Wait(ztl::exclusive_mutex::Mutex& lock, const Predicate& pred)
			{
				while(!pred())
				{
					lock.Unlock();
					WaitForSingleObject(GetEvent(), INFINITE);
					lock.Lock();
				}
			}
			void Wait(ztl::exclusive_mutex::Mutex& lock)
			{
				lock.Unlock();
				WaitForSingleObject(GetEvent(), INFINITE);
				assert(!lock.IsLocked());
				lock.Lock();
			}
			void NotifyOne()
			{
				SetEvent(GetEvent());
			}
			void NotifyAll()
			{
				//自动重置模式下PulseEvent和SetEvent的作用没有什么区别- -
				PulseEvent(GetEvent());
			}
			template <class Rep, class Period, class Predicate>
			bool WaitFor(ztl::exclusive_mutex::Mutex& lock, const duration<Rep, Period>& rel_time, const Predicate& pred)
			{
				return TimedWait(lock, static_cast<DWORD>(rel_time.count()), pred);
			}
			template <class Rep, class Period>
			bool WaitFor(ztl::exclusive_mutex::Mutex& lock, const duration<Rep, Period>& rel_time)
			{
				return TimedWait(lock, static_cast<DWORD>(rel_time.count()));
			}
			template <class Clock, class Duration, class Predicate>
			bool WaitUntil(ztl::exclusive_mutex::Mutex& lock, const time_point<Clock, Duration>& abs_time, const Predicate& pred)
			{
				return TimedWait(lock, static_cast<DWORD>(abs_time.time_since_epoch().count()), pred);
			}
			template <class Clock, class Duration>
			bool WaitUntil(ztl::exclusive_mutex::Mutex& lock, const time_point<Clock, Duration>& abs_time)
			{
				return TimedWait(lock, static_cast<DWORD>(abs_time.time_since_epoch().count()));
			}

		private:
			template<class Predicate>
			bool TimedWait(ztl::exclusive_mutex::Mutex& lock, DWORD millisecond, const Predicate& pred)
			{
				while(!pred())
				{
					lock.Unlock();
					auto result = WaitForSingleObject(GetEvent(), millisecond);
					assert(!lock.IsLocked());
					lock.Lock();
					switch(result)
					{
						case  WAIT_TIMEOUT:
							return false;

						case WAIT_OBJECT_0:
							break;
						default:
							cout << GetLastError() << endl;
							abort();
							break;
					}
				}
				return true;
			}
			bool TimedWait(ztl::exclusive_mutex::Mutex& lock, DWORD millisecond)
			{
				lock.Unlock();
				auto result = WaitForSingleObject(GetEvent(), millisecond);
				assert(!lock.IsLocked());
				lock.Lock();
				switch(result)
				{
					case  WAIT_TIMEOUT:
						//这里没重新加锁,挂挂挂,条件变量要求等待的时候先解锁,测试条件,返回时重新加锁.
						return false;
					case WAIT_OBJECT_0:
						break;
					default:
						cout << GetLastError() << endl;
						abort();
						break;
				}
				return true;
			}
			HANDLE GetEvent()
			{
				HANDLE expect = NULL;
				HANDLE new_event = CreateEvent(NULL, FALSE, FALSE, NULL);
				event.compare_exchange_strong(expect, new_event);
				return event;
			}
		private:
			atomic<HANDLE> event = 0;
		};
	}
}