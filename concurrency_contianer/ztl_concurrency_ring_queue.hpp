#pragma once
#include "../forward.h"
#include "../common/ztl_atomic_operator.hpp"
namespace ztl
{
	namespace concurrency_ring_queue
	{
		//地址1是头部,2是尾部
		//using ztl::atomic_opertaor::AtomicExchange;
		//using ztl::atomic_opertaor::AtomicCompareExchange;
		//class ConcurrencyRingQueue
		//{
		//	typedef unsigned int ValueType;
		//	typedef unsigned long long DoubleValueType;
		//	//typedef DoubleValueType* DoubleCASpointer;
		//	//using PointerType = ValueType*;
		//	//typedef std::pair<int, int> CASPointer;
		//	struct DoubleAddress
		//	{
		//		unsigned int first;
		//		unsigned int second;
		//	};
		//	union Chunk
		//	{
		//		DoubleAddress pointer;
		//		DoubleValueType chunk;
		//	};
		//public:
		//	ConcurrencyRingQueue(const unsigned int _maxSize):maxSize(_maxSize + 2),arrayRoot(new ValueType[maxSize]),headCount(0), tailCount(1)
		//	{
		//		memset(arrayRoot, 0, sizeof(ValueType)*maxSize);
		//		arrayRoot[0] = 1;
		//		arrayRoot[1] = 2;
		//	}
		//	//析构不保证线程安全性
		//	~ConcurrencyRingQueue()
		//	{
		//		delete[] arrayRoot;
		//	}
		//	ConcurrencyRingQueue(ConcurrencyRingQueue&&) = default;
		//	ConcurrencyRingQueue(const ConcurrencyRingQueue&) = delete;
		//	ConcurrencyRingQueue& operator=(const ConcurrencyRingQueue&) = delete;
		//	ConcurrencyRingQueue& operator=(ConcurrencyRingQueue&&) = default;
		//public:
		//	bool Enqueue(ValueType address)
		//	{
		//		
		//	}
		//	bool Dequeue(ValueType& address)
		//	{

		//	}
		//private:
		//	const unsigned int maxSize;
		//	unsigned int headCount;
		//	unsigned int tailCount;
		//	ValueType* arrayRoot;
		//};
	}
}