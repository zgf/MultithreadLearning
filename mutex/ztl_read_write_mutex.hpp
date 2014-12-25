#pragma once
#include "../forward.h"
#include "../ztl_condition_variable.h"
namespace ztl
{
	namespace read_write_mutex
	{
		class ReadWriteLock
		{
		public:
			ReadWriteLock() = default;
			~ReadWriteLock() = default;
			ReadWriteLock(ReadWriteLock&&) = default;
			ReadWriteLock(const ReadWriteLock&) = delete;
			ReadWriteLock& operator=(const ReadWriteLock&) = delete;
			ReadWriteLock& operator=(ReadWriteLock&&) = default;
		public:

			void ReadLock()
			{
				locker.Lock();
				while(state < 0)
				{
					conditionVariable.Wait(locker);
				}
				state++;
				locker.Unlock();
			}

			void ReadUnlock()
			{
				locker.Lock();
				--state;
				if (state == 0)
				{
					conditionVariable.NotifyOne();
				}
				locker.Unlock();
			}

			void WriteLock()
			{
				locker.Lock();
				while(state !=0)
				{
					conditionVariable.Wait(locker);
				}
				state = -1;
				locker.Unlock();
			}

			void WriteUnlock()
			{
				locker.Lock();
				state = 0;
				conditionVariable.NotifyAll();
				locker.Unlock();
			}
		
			bool ReadLockConvertToWriteLock()
			{
				locker.Lock();
				bool result;
				if (state == 1)
				{
					state = -1;
					result = true;
				}
				result = false;
				locker.Unlock();
				return result;
			}
			bool WriteLockConvertToReadLock()
			{
				locker.Lock();
				bool result;
				if(state == -1)
				{
					state = 1;
					result = true;
				}
				result = false;
				locker.Unlock();
				return result;
			}
			bool IsLocked()const
			{
				return state == 0;
			}
			bool IsReadLocked()const
 			{
				return state > 0;
			}
			bool IsWriteLocked()const
			{
				return state == -1;
			}
		private:
			ztl::exclusive_mutex::Mutex locker;
			ztl::condition_variable::ConditionVariable conditionVariable;
			int state;//state=-1是写锁 0无锁 >0读者计数
		};
	}
}