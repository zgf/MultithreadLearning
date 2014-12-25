#pragma once
#include "test_mutex/test_exclusive_mutex.hpp"
#include "test_condition_variable.hpp"
#include "test_concurrency_container/test_concurrency_queue.hpp"
#include "test_common/test_atomic_operator.hpp"
//#include "test_concurrency_container/test_concurrency_stack.hpp"
namespace ztl
{
	void Test_All_Component()
	{
		//test_exclusive_mutex::Test_All();
		//test_condition_variable::Test_All();
		test_concurrency_queue::Test_All();
		//test_atomic_operator::Test_All();
		//test_concurrency_stack::Test_All();
	}
}