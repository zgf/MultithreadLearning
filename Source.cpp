
#include "stdlib.h"
#include "crtdbg.h"
#include "forward.h"
#include "test/test_all_component.hpp"
int main()
{
	ztl::Test_All_Component();
	//std::is_trivially_copyable<ztl::concurrency_queue::ConcurrencyQueue<unsigned int>::ConcurrencyQueueImpl<unsigned int,true>::Reference>::value;
	_CrtDumpMemoryLeaks();
}
