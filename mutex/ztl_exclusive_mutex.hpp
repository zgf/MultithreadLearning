#pragma once
#include "../forward.h"
namespace ztl
{
	namespace exclusive_mutex
	{
		/*
		提供独占式mutex
		*/
		class Mutex
		{
		public:
			Mutex() = default;
			~Mutex()
			{
				HANDLE expect = NULL;
				event.compare_exchange_strong(expect, NULL);
				CloseHandle(expect);
			}
			Mutex(Mutex&&) = default;
			Mutex(const Mutex&) = delete;
			Mutex& operator=(const Mutex&) = delete;
			Mutex& operator=(Mutex&&) = default;
			//成功锁定返回,失败挂起,count++
			void Lock()
			{
				while(lock.exchange(1))
				{
					count++;
					WaitForSingleObject(GetEvent(), INFINITE);
					count--;
				}
			}
			//唤醒一个等待的线程,lock重置为0.count--
			void Unlock()
			{
				assert(lock != 0);
				lock.store(0);
				SetEvent(GetEvent());
			}

			template <class Rep, class Period>
			bool TimedLockFor(const duration<Rep, Period>& rel_time)
			{
				return TimedLock(static_cast<DWORD>(rel_time.count()));
			}
			template <class Clock, class Duration>
			bool TimedLockUntil(const time_point<Clock, Duration>& abs_time)
			{
				return TimedLock(static_cast<DWORD>(abs_time.time_since_epoch().count()));
			}
			//成功锁定返回true,锁定失败返回false
			//这里和标准的独占式mutex实现不一致,标准要求对于trylock.同一线程重入就会死锁.
			//因为我的实现不包括线程的id,所以这个实现不了- -...
			//而且也没必要实现,trylock的作用应该是观察锁的状态并尝试获得锁.不会阻塞
			bool TryLock()
			{
				if(lock.exchange(1))
				{
					//锁定失败了
					return false;
				}
				return true;
			}
			bool IsLocked() const
			{
				return lock;
			}
		private:
			//尝试锁定,等待事件直到超时就返回false
			bool TimedLock(DWORD milliseconds)
			{
				while(lock.exchange(1))
				{
					//int i = -1;
					count++;
					auto result = WaitForSingleObject(GetEvent(), milliseconds);
					count--;
					switch(result)
					{
						case  WAIT_TIMEOUT:
							return false;
							//疏忽了,唤醒后lock=0;应该去尝试一下锁
						case WAIT_OBJECT_0:
							//i = lock;
							//
							//assert(lock.load() == 0);
							//问题出在如果线程1被唤醒,运行到76行后被上下文切换.另一个线程超时返回,return出去后重入了lock.然后因为超时的线程没有锁.而唤醒的线程还没获得锁,到76就被切换了
							//重入的时候锁处在未锁定状态.重入lock后锁定了锁.之后线程切换,76行的线程继续运行,到这里,lock很明显=1不等于0.所以assert失败
							//这是feature,not bug.是对程序流程了解不够清晰导致.唤醒的线程会再次在while测试锁,再决定等待还是返回.所以对外的行为不会出现问题.
							break;
						default:
							cout << GetLastError() << endl;
							abort();
							break;
					}
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
			atomic<int> count = 0;
			atomic<bool> lock = 0;
		};
	}
}