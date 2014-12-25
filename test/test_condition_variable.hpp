#pragma once
#include "../ztl_condition_variable.h"
namespace ztl
{
	namespace test_condition_variable
	{
		void Test_Condition_Variable_Wait()
		{
			int g_num = 0;  // protected by g_num_mutex
			ztl::exclusive_mutex::Mutex g_num_mutex;
			ztl::condition_variable::ConditionVariable conditionVar;

			auto slow_increment = [&g_num_mutex, &g_num, &conditionVar](int id)
			{
				for(int i = 0; i < 3; ++i)
				{
					g_num_mutex.Lock();

					conditionVar.Wait(g_num_mutex, [id, &g_num]()
					{
						return g_num % 2 == id;
					});
					std::cout << id << " => " << g_num << '\n';
					g_num++;
					conditionVar.NotifyOne();
					g_num_mutex.Unlock();

					//	std::this_thread::sleep_for(std::chrono::seconds(1));
				}
			};

			std::thread t1(slow_increment, 0);
			std::thread t2(slow_increment, 1);
			t1.join();
			t2.join();
		}
		void Test_Condition_Variable_TimedWaitFor()
		{

			ztl::condition_variable::ConditionVariable cv;
			ztl::exclusive_mutex::Mutex cv_m;
			std::atomic<int> i=0;

			auto waits=[&cv,&cv_m,&i](int idx)
			{
				cv_m.Lock();
				if(cv.WaitFor(cv_m, std::chrono::milliseconds(idx * 100), [&i]()
				{
					return i == 1;
				}))
				{
					std::cerr << "Thread " << idx << " finished waiting. i == " << i << '\n';

				}
				else
				{
					std::cerr << "Thread " << idx << " timed out. i == " << i << '\n';
				}
				cv_m.Unlock();
			};

			auto signals=[&cv,&i]()
			{

				std::this_thread::sleep_for(std::chrono::milliseconds(120));
				std::cerr << "Notifying...\n";
				cv.NotifyAll();
				std::this_thread::sleep_for(std::chrono::milliseconds(100));
				i = 1;
				std::cerr << "Notifying again...\n";
				cv.NotifyAll();

			};
			std::thread t1(waits, 1), t2(waits, 2), t3(waits, 3), t4(signals);
			t1.join(); t2.join(), t3.join(), t4.join();
		}
		void Test_All()
		{
			Test_Condition_Variable_Wait();
			Test_Condition_Variable_TimedWaitFor();
		}
	}
}