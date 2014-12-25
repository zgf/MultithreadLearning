#pragma once
#include "../../concurrency_contianer/ztl_concurrency_queue.hpp"
#include <string>
#include <vector>
#include <random>
#undef  min
#undef  max
namespace ztl
{
	namespace test_concurrency_queue
	{
		void Test_Concurrency()
		{
			
			
			ztl::concurrency_queue::ConcurrencyQueue<unsigned int> queue;
			auto product = [&queue]()
			{
				std::random_device rd;
				std::mt19937 mt(rd());
				int i = 0;
				while(++i!=1000000)
				{
					queue.Enqueue(mt());
					if (i %100000 == 0)
					{
						cout << i<<"time enqueue"<<endl;
					}
				}
				cout << "exit"<<endl;
				return;
			};
			auto consume = [&queue]()
			{
				int i = 1;
				while(i != 1000000)
				{
					unsigned int value;
					auto result = queue.Dequeue(value);
					if (result == true)
					{
						i++;
					}
					if( i % 100000 == 0)
					{
						cout << i << "time dequeue" << endl;
					}
				}
				cout << "exit" << endl;
				return;
			};
	
			std::thread _1(product);
			std::thread _2(product);
			std::thread _3(consume);
			std::thread _4(consume);
			_1.join();
			_2.join();
		
				_3.join();
				_4.join();
			cout << "exit main";
		}
		
		void Test_Concurrency1()
		{
			ztl::concurrency_queue::ConcurrencyQueue<unsigned int> queue;
			unsigned int value;
			queue.Enqueue(10);
			queue.Enqueue(20);
			queue.Enqueue(30);
			queue.Enqueue(10);
			queue.Enqueue(20);
			/*queue.Dequeue(value);
			queue.Dequeue(value);
			queue.Dequeue(value);
			queue.Dequeue(value);
			queue.Dequeue(value);*/
		}
		void Test_All()
		{
			//Test_Concurrency1();
			Test_Concurrency();
		}

	}
}