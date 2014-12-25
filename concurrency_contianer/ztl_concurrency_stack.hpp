#pragma once
#include "../forward.h"
#include "../common/ztl_stamp_pair.hpp"
#include "../common/ztl_atomic_operator.hpp"
#include "../common/ztl_hazard_pointer.hpp"
namespace ztl
{
	namespace concurrency_stack
	{
		using ztl::atomic_opertaor::CASCompareExchange;
		using ztl::atomic_opertaor::AtomicExchange;
		template<typename ValueType>
		class ConcurrencyStack
		{

			struct Node;
			typedef ztl::stamp_pair::StampPair<Node*, int> NodePointer;
			typedef hazard_pointer::HazardPointers<NodePointer> HazardPointersType;
			typedef typename hazard_pointer::HazardPointers<NodePointer>::HazardNodePointer HazardNodePointer;
			struct Node
			{
				atomic<ValueType>    value= ValueType();
				atomic<NodePointer>  next = NodePointer();
			public:
				Node() = default;
				Node(const ValueType& _value)
					:value(_value)
				{

				}
				~Node() = default;
				Node(Node&&) = default;
				Node(const Node&) = default;
				Node& operator=(Node&&) = default;
				Node& operator=(const Node&) = default;
			};
		public:
			ConcurrencyStack(unsigned int maxThreadNmber, unsigned int gcNumber):pointers(maxThreadNmber, gcNumber)
			{
					
			}
			~ConcurrencyStack()
			{
				auto worker = top.load();
				while(worker.current!=nullptr)
				{
					auto old_ptr = worker.current;
					worker = worker.current->next.load();
					delete old_ptr;
				}
			}
		public:
			void Push(const ValueType& value)
			{
				NodePointer new_node(new Node(value));
				NodePointer* lookup;
				do
				{
					lookup = pointers.Acquire();
				}
				while (!CASCompareExchange((NodePointer*)lookup,top, NodePointer(nullptr)));
				do
				{
					new_node.current->next = top.load();
				}
				while(!CASCompareExchange(&top, new_node, new_node.current->next.load()));
				pointers.Release(lookup);
				++size;
			}
			bool Pop(ValueType& value)
			{
				NodePointer lookup;
				do 
				{
					lookup = top.load();
					if (lookup.current == nullptr)
					{
						return false;
					}
				}
				while(!CASCompareExchange(&top, lookup.current->next.load(), lookup));
				value = lookup.current->value;
				pointers.Retire(lookup);
				--size;
				return true;
			}
			int Size()const
			{
				return size;
			}
		private:
			atomic<int> size = 0;
			atomic<NodePointer> top;
			HazardPointersType pointers;
		};
	}
}