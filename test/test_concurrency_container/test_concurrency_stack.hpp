#pragma once
#include "../../forward.h"
#include "../../concurrency_contianer/ztl_concurrency_stack.hpp"
#include <random>
namespace ztl
{
	namespace test_concurrency_stack
	{
		void Test_Stack()
		{
			ztl::concurrency_stack::ConcurrencyStack<unsigned int> queue(4,0);
			
			auto product = [&queue]()
			{
				std::random_device rd;
				std::mt19937 mt(rd());
				int i = 0;
				while(++i != 10000)
				{
					queue.Push(mt());
					if(i % 1000== 0)
					{
						cout << i << "time enqueue" << endl;
					}
				}
				cout << "exit" << endl;
				return;
			};
			auto consume = [&queue]()
			{
				int i = 1;
				unsigned int value;
				while(i != 10000)
				{
					
					auto result = queue.Pop(value);
					if(result == true)
					{
						i++;
						if(i % 1000 == 0)
						{
							cout << i << "time dequeue" << endl;
						}
					}
					
				}
				cout << "exit" << endl;
				return;
			};
			std::thread _3(product);

			std::thread _4(product);
			std::thread _1(product);

			std::thread _2(product);
		

			_1.join();
			_2.join();
			_3.join();
			_4.join();
			cout << "exit main";

		}
		void Test_All()
		{
			Test_Stack();
		}
	}
}
