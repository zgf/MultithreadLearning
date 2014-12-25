#pragma once
#include "ztl_condition_variable.h"
namespace ztl
{
	namespace count_down_latch
	{
		class CountDownLatch
		{
		public:
			CountDownLatch(int maxSlot):slot(maxSlot)
			{

			}
			~CountDownLatch() = default;
			CountDownLatch(CountDownLatch&&) = default;
			CountDownLatch(const CountDownLatch&) = delete;
			CountDownLatch& operator=(const CountDownLatch&) = delete;
			CountDownLatch& operator=(CountDownLatch&&) = default;
		public:
			void CountDown()
			{
				mutex.Lock();
				while(slot > 0)
				{
					--slot;
					conditionVariable.Wait(mutex);
				}
				conditionVariable.NotifyAll();
				mutex.Unlock();
			}
			
		private:
			int slot;
			ztl::condition_variable::ConditionVariable conditionVariable;
			ztl::exclusive_mutex::Mutex mutex;
		};

	}
}