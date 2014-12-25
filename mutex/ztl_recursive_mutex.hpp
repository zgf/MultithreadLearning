#pragma once
#include "../forward.h"
namespace ztl
{
	namespace recursive_mutex
	{
		/*
		嗯, id相同的时候lock++; 不同时候睡上去.unlock的时候lock--;
		lock = 0的时候唤醒等待线程, 只有一个线程去lock.然后重置id.*/

		class RecursiveMutex
		{
		public:
			RecursiveMutex() = default;
			~RecursiveMutex()
			{
				HANDLE expect = NULL;
				event.compare_exchange_strong(expect, NULL);
				CloseHandle(expect);
			}
			RecursiveMutex(RecursiveMutex&&) = default;
			RecursiveMutex(const RecursiveMutex&) = delete;
			RecursiveMutex& operator=(const RecursiveMutex&) = delete;
			RecursiveMutex& operator=(RecursiveMutex&&) = default;
			//成功锁定返回,失败挂起,count++
			void Lock()
			{
				auto current_id = GetCurrentThreadId();
				while(lock.exchange(1))
				{
					//锁不为0
					//检测是否是同id
					if(current_id == ownerId)
					{
						//锁被当前线程独占在
						break;
					}
					count++;
					WaitForSingleObject(GetEvent(), INFINITE);
					count--;
				}
				//这里开始是锁的内部
				lockCount++;
				ownerId = current_id;
			}
			//唤醒一个等待的线程,lock重置为0.count--
			void Unlock()
			{
				assert(lock != 0);
				assert(lockCount != 0);
				assert(GetCurrentThreadId() == ownerId);
				lockCount--;
				if(lockCount == 0)
				{
					lock = 0;
					ownerId = 0;
					SetEvent(GetEvent());
				}
			}

			template <class Rep, class Period>
			bool TimedLockFor(const duration<Rep, Period>& rel_time)
			{
				return TimedLock(rel_time.count());
			}
			template <class Clock, class Duration>
			bool TimedLockUntil(const time_point<Clock, Duration>& abs_time)
			{
				return TimedLock(abs_time.time_since_epoch().count());
			}
			//成功锁定返回true,锁定失败返回false
			//这里和标准的独占式mutex实现不一致,标准要求对于trylock.同一线程重入就会死锁.
			//因为我的实现不包括线程的id,所以这个实现不了- -...
			//而且也没必要实现,trylock的作用应该是观察锁的状态并尝试获得锁.不会阻塞
			bool TryLock()
			{
				auto current_id = GetCurrentThreadId();
				if(lock.exchange(1))
				{
					if(current_id != ownerId)
					{
						//锁定失败了
						return false;
					}
				}
				//已锁定
				lockCount++;
				ownerId = current_id;
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
				auto current_id = GetCurrentThreadId();
				while(lock.exchange(1))
				{
					if(current_id == ownerId)
					{
						//锁被当前线程独占在
						break;
					}
					count++;
					auto result = WaitForSingleObject(GetEvent(), milliseconds);
					count--;
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
				lockCount++;
				ownerId = current_id;
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
			atomic<int>  lockCount = 0;
			atomic<DWORD> ownerId = 0;
		};
	}
}