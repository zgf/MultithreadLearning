#pragma once
#include "../../forward.h"
#include "../../mutex/ztl_exclusive_mutex.hpp"
#include "../../ztl_mutex.hpp"
#include <ctime>          // std::time_t, std::tm, std::localtime, std::mktime
namespace ztl
{
	namespace test_exclusive_mutex
	{

		void Test_Mutex_TryLock()
		{

			ztl::exclusive_mutex::Mutex mutex;
			std::chrono::milliseconds interval(100);

			int job_shared = 0; // both threads can modify 'job_shared',
								// mutex will protect this variable

			int job_exclusive = 0; // only one thread can modify 'job_exclusive'
								   // no protection needed

								   // this thread can modify both 'job_shared' and 'job_exclusive'
			auto job_1 = [&mutex, &interval, &job_exclusive, &job_shared]()
			{
				std::this_thread::sleep_for(interval); // let 'job_2' take a lock

				while(true)
				{
					// try to lock mutex to modify 'job_shared'
					if(mutex.TryLock())
					{
						std::cout << "job shared (" << job_shared << ")\n";
						mutex.Unlock();
						return;
					}
					else
					{
						// can't get lock to modify 'job_shared'
						// but there is some other work to do
						++job_exclusive;
						std::cout << "job exclusive (" << job_exclusive << ")\n";
						std::this_thread::sleep_for(interval);
					}
				}
			};

			// this thread can modify only 'job_shared'
			auto job_2 = [&mutex, &interval, &job_exclusive, &job_shared]()
			{
				mutex.Lock();
				std::this_thread::sleep_for(5 * interval);
				++job_shared;
				mutex.Unlock();
			};
			std::thread thread_1(job_1);
			std::thread thread_2(job_2);

			thread_1.join();
			thread_2.join();
		}

		void Test_Mutex_Lock()
		{
			int g_num = 0;  // protected by g_num_mutex
			ztl::exclusive_mutex::Mutex g_num_mutex;


			auto slow_increment = [&g_num_mutex, &g_num](int id)
			{
				for(int i = 0; i < 3; ++i)
				{
					{
						auto locker = ztl_guard_locker::MakeGuardLocker(g_num_mutex);
						//g_num_mutex.Lock();
						++g_num;
						std::cout << id << " => " << g_num << '\n';
						//g_num_mutex.Unlock();
					}
					std::this_thread::sleep_for(std::chrono::seconds(1));
				}
			};

			std::thread t1(slow_increment, 0);
			std::thread t2(slow_increment, 1);
			t1.join();
			t2.join();
		}
		void Test_Mutex_TryLockUntil()
		{
			ztl::exclusive_mutex::Mutex cinderella;

			// gets time_point for next midnight:
			auto midnight = []()->std::chrono::time_point<std::chrono::system_clock>
			{
				using std::chrono::system_clock;
				std::time_t tt = system_clock::to_time_t(system_clock::now());
				struct std::tm * ptm = std::localtime(&tt);
				++ptm->tm_mday; ptm->tm_hour = 0; ptm->tm_min = 0; ptm->tm_sec = 0;
				return system_clock::from_time_t(mktime(ptm));
			};

			auto carriage = [&cinderella, &midnight]()
			{
				if(cinderella.TimedLockUntil(midnight()))
				{
					std::cout << "ride back home on carriage\n";
					cinderella.Unlock();
				}
				else
					std::cout << "carriage reverts to pumpkin\n";
			};

			auto ball = [&cinderella]()
			{
				cinderella.Lock();
				std::cout << "at the ball...\n";
				cinderella.Unlock();
			};

			std::thread th1(ball);
			std::thread th2(carriage);

			th1.join();
			th2.join();
		}
		void Test_Mutex_TryLockFor()
		{

			ztl::exclusive_mutex::Mutex mtx;


			auto fireworks = [&mtx]()
			{
				// waiting to get a lock: each thread prints "-" every 200ms:
				while(!mtx.TimedLockFor(std::chrono::milliseconds(200)))
				{
					std::cout << "-";
				}
				// got a lock! - wait for 1s, then this thread prints "*"
				std::this_thread::sleep_for(std::chrono::milliseconds(1000));
				std::cout << "*\n";
				mtx.Unlock();
			};


			std::thread threads[10];
			// spawn 10 threads:
			for(int i = 0; i < 10; ++i)
			{
				threads[i] = std::thread(fireworks);
			}
			for(auto& th : threads)
			{
				th.join();
			}
		}
		void Test_All()
		{
			Test_Mutex_Lock();
			Test_Mutex_TryLock();
			Test_Mutex_TryLockFor();
			Test_Mutex_TryLockUntil();
		}
	}
}