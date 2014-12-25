#pragma once
#include "forward.h"
namespace ztl
{
	namespace ztl_spin_lock
	{
		class SpinLock
		{
		public:
			SpinLock() = default;
			SpinLock(const SpinLock&) = delete;
			SpinLock(SpinLock&&) = default;
			~SpinLock() = default;
			SpinLock& operator=(const SpinLock&) = delete;
			SpinLock& operator=(SpinLock&&) = default;
		public:
			void Lock() noexcept
			{
				while(lock.exchange(1));
			}
			void Unlock() noexcept
			{
				assert(lock != 0);
				lock.store(0);
			}
			bool IsLocked() const noexcept
			{
				return lock.load();
			}
		private:
			//0表示未锁
			//1表示锁定
			atomic<bool> lock = 0;
		};
	}
}