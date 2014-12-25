#pragma once
#include "../../common/ztl_atomic_operator.hpp"
namespace ztl
{
	namespace test_atomic_operator
	{
		void Test_Atomic_Increment()
		{
			int a = 3;
			ztl::atomic_opertaor::AtomicIncrement(&a);
		}
		void Test_All()
		{
			Test_Atomic_Increment();
		}
	}
}