#pragma once
#include "../forward.h"
#include "../common/ztl_atomic_operator.hpp"
#include "../common/ztl_stamp_pair.hpp"
#include "../common/ztl_hazard_pointer.hpp"
#include "../common/ztl_atomic_shared_ptr.hpp"
#ifndef CHACHE_SIZE
#define CHACHE_SIZE 64
#endif // CHACHE_SIZE

namespace ztl
{
	namespace concurrency_queue
	{
		using atomic_opertaor::AtomicIncrement;
		using atomic_opertaor::AtomicCompareExchangePointer;
		using atomic_opertaor::AtomicCompareExchange;
		using atomic_opertaor::AtomicDecrement;
		using atomic_opertaor::AtomicExchange;
		using atomic_opertaor::CASCompareExchange;
		using atomic_shared_ptr::atomic_shared_pointer;
	
		template<typename ValueType,bool IsAtomicType>
		class ConcurrencyQueueImpl;
		template<typename ValueType>
		class ConcurrencyQueueImpl<ValueType, true>
		{
		public:

			alignas(CHACHE_SIZE) struct Node
			{
				ValueType value = ValueType();
				atomic_shared_pointer<Node> next;
			public:
				Node() = default;
				Node(const ValueType& _value)
					:value(_value)
				{
				}
				~Node()
				{

				}
				Node(Node&&) = default;
				Node(const Node&) = default;
				Node& operator=(Node&&) = default;
				Node& operator=(const Node&) = default;
			};

		public:
			//ConcurrencyQueueImpl() = delete;
			ConcurrencyQueueImpl()
			{
				head = make_shared<Node>();
				tail = head.load();
			}
			//为了避免一次性析构太多shared导致栈溢出.这里析构的时候自己拆分下
			~ConcurrencyQueueImpl()
			{
				auto current = head.load();
				while(head.load() != nullptr)
				{
					head = head.load()->next.load();
					current.reset();
				}
				
			}
			ConcurrencyQueueImpl(ConcurrencyQueueImpl&&) = default;
			ConcurrencyQueueImpl(const ConcurrencyQueueImpl&) = delete;
			ConcurrencyQueueImpl& operator=(const ConcurrencyQueueImpl&) = delete;
			ConcurrencyQueueImpl& operator=(ConcurrencyQueueImpl&&) = default;
			
		public:
			void Enqueue(const ValueType& value)
			{
				auto new_node = make_shared<Node>(value);
				auto lookup = tail.load();
				shared_ptr<Node> null;
				while(!lookup->next.compare_exchange_weak(null,new_node))
				{
					lookup = tail.load();
					null = nullptr;
				}
				tail.compare_exchange_strong(lookup, new_node);
				size++;
			}

			bool Dequeue(ValueType& value)
			{
				shared_ptr<Node> lookup;
				do
				{
					lookup = head.load();
					if (lookup->next.load() == nullptr)
					{
						return false;
					}
				}
				while(!head.compare_exchange_weak(lookup, lookup->next.load()));
				value = lookup->value;
				size--;
				return true;
			}

			bool Contains(const ValueType& value)
			{
				//节点不会被删除- -...直到链表析构掉.妹的,这种我早实现了,而且跑的比sharedptr快多了
				auto work = head.load();
				work = work->next.load();
				while(work!=nullptr)
				{
					if (work->value == value)
					{
						return true;
					}
					work = work->next.load();
				}
				return false;
			}

			int Size()const
			{
				return size;
			}
			bool IsLockFree()const
			{
				return true;
			}
		private:
			alignas(CHACHE_SIZE)  atomic_shared_pointer<Node> head;
			alignas(CHACHE_SIZE)  atomic_shared_pointer<Node> tail;
			alignas(CHACHE_SIZE)  atomic<int> size = 0;
		};



		//template<typename ValueType>
		//class ConcurrencyQueueImpl<ValueType,true>
		//{
		//	struct Node;
		//	using NodePointer = stamp_pair::StampPair<Node*, int>;
		//	using HazardPointersType = hazard_pointer::HazardPointers<NodePointer,2>;
		//	using HazardNodePointer = typename HazardPointersType::HazardNode*;
		//	struct Node
		//	{
		//		atomic<ValueType> value;
		//		atomic<NodePointer> next;
		//	public:
		//		Node() = default;
		//		Node(const ValueType& _value)
		//			:value(_value)
		//		{

		//		}
		//		~Node() = default;
		//		Node(Node&&) = default;
		//		Node(const Node&) = default;
		//		Node& operator=(Node&&) = default;
		//		Node& operator=(const Node&) = default;
		//	};
		//	
		//public:
		//	//初始化不保证线程安全性
		//	ConcurrencyQueueImpl() = delete;
		//	ConcurrencyQueueImpl(unsigned int maxThreadNmber, unsigned int gcNumber) :pointers(maxThreadNmber, gcNumber)
		//	{
		//		tail = new Node;
		//		head = tail.load();
		//	}
		//	//析构不保证线程安全性
		//	~ConcurrencyQueueImpl()
		//	{
		//		auto lookup = head.load();
		//		while(lookup.current!=nullptr)
		//		{
		//			auto old_ptr = lookup.current;
		//			lookup.current = lookup.current->next.load().current;
		//			delete old_ptr;
		//		}
		//	}
		//	ConcurrencyQueueImpl(ConcurrencyQueueImpl&&) = default;
		//	ConcurrencyQueueImpl(const ConcurrencyQueueImpl&) = delete;
		//	ConcurrencyQueueImpl& operator=(const ConcurrencyQueueImpl&) = delete;
		//	ConcurrencyQueueImpl& operator=(ConcurrencyQueueImpl&&) = default;
		//public:
		//	NodePointer* Acquire(NodePointer target)
		//	{
		//		NodePointer*  slot;
		//		do
		//		{
		//			slot = pointers.Acquire();
		//		}
		//		while(!CASCompareExchange(slot, target, NodePointer(nullptr)));
		//		return slot;
		//	}
		//public: 
		//	void Enqueue(const ValueType& value)
		//	{
		//		NodePointer new_node(new Node(value));
		//		NodePointer* slot;
		//		//NodePointer* slot_next ;
		//		NodePointer lookup;
		//		NodePointer* lookup_next;
		//		while(1)
		//		{
		//			lookup = tail;
		//			//lookup_next = (NodePointer*) &lookup.current->next;
		//			slot = Acquire(lookup);
		//			//slot_next = Acquire(*lookup_next);
		//			//if(*lookup_next != nullptr)
		//			//{
		//			//	if(CASCompareExchange(&tail, *lookup_next, lookup))
		//			//	{
		//			//		pointers.Release(slot);
		//			////		pointers.Release(slot_next);
		//			//		slot = Acquire(lookup);
		//			//	//	slot_next = Acquire(*lookup_next);
		//			//	}
		//			//}

		//			if (CASCompareExchange((NodePointer*) &lookup.current->next, new_node, NodePointer(nullptr)))
		//			{
		//				break;
		//			}
		//			else
		//			{
		//				pointers.Release(slot);
		//			//	pointers.Release(slot_next);
		//			}
		//		}
		//		//tail.compare_exchange_weak(lookup, new_node);
		//		CASCompareExchange(&tail, new_node, lookup);
		//		pointers.Release(slot);
		////		pointers.Release(slot_next);
		//		++size;
		//	}

		//	bool Dequeue(ValueType& value)
		//	{
		//		NodePointer lookup;
		//		NodePointer* lookup_next;
		//		NodePointer* slot;
		//		while(1)
		//		{
		//			lookup = head;
		//			lookup_next = (NodePointer*) &lookup.current->next;
		//			
		//			slot = Acquire(*lookup_next);
		//			if(lookup_next->current == nullptr)
		//			{
		//				pointers.Release(slot);
		//				return false;
		//			}

		//			if (CASCompareExchange((NodePointer*) &head, *lookup_next, lookup))
		//			{
		//				break;
		//			}
		//			else
		//			{
		//				pointers.Release(slot);

		//			}
		//		}

		//		value = lookup.current->value;
		//		pointers.Release(slot);
		//		pointers.Retire(lookup);
		//		--size;
		//		return true;
		//	}
		//	int Size()const
		//	{
		//		return size;
		//	}
		//	bool IsLockFree()const
		//	{
		//		return true;
		//	}
		//private:
		//	atomic<NodePointer> head;
		//	atomic<NodePointer> tail;
		//	atomic<int> size = 0;
		//	HazardPointersType pointers;

		//};
		//template<typename ValueType>
		//class ConcurrencyQueueImpl<ValueType, false>
		//{
		//	struct Node;
		//	using NodePointer = stamp_pair::StampPair<Node*, int>;
		//	using LookUpNodePointer = stamp_pair::LookUpStampPair<Node*, int>;
		//	struct Node
		//	{
		//		ValueType value;
		//		NodePointer next;
		//	};

		//public:
		//	//初始化不保证线程安全性
		//	ConcurrencyQueueImpl()
		//	{
		//		head = tail = new Node;
		//	}
		//	//析构不保证线程安全性
		//	~ConcurrencyQueueImpl()
		//	{
		//		while(head.current != nullptr)
		//		{
		//			auto old_ptr = head;
		//			head = head.current->next;
		//			delete old_ptr.current;
		//		}
		//	}
		//	ConcurrencyQueueImpl(ConcurrencyQueueImpl&&) = default;
		//	ConcurrencyQueueImpl(const ConcurrencyQueueImpl&) = delete;
		//	ConcurrencyQueueImpl& operator=(const ConcurrencyQueueImpl&) = delete;
		//	ConcurrencyQueueImpl& operator=(ConcurrencyQueueImpl&&) = default;
		//public:
		//	void Enqueue(const ValueType& value)
		//	{
		//		NodePointer new_node = new Node;
		//		lock.Lock();
		//		new_node.current->value = value;
		//		lock.Unlock();
		//		LookUpNodePointer lookup;
		//		do
		//		{
		//			lookup = tail.current->next.GetLookUp();
		//		}
		//		while(!CASCompareExchange(&tail.current->next,new_node, lookup));
		//		//到这里后面是enqueue的同步区了,因为tailnext不是null了,别的线程会一直卡在循环里面
		//		AtomicExchange(&tail.current, new_node);
		//		AtomicIncrement(&size);
		//	}

		//	ValueType Dequeue()
		//	{
		//		NodePointer lookup;
		//		do
		//		{
		//			lookup = head.current->next.GetLookUp();
		//			
		//		}
		//		while(!CASCompareExchange(&head,lookup.current->next, lookup));
		//		lock.Lock();
		//		auto value = lookup.current->value;
		//		lock.Unlock();
		//		delete lookup.current;
		//		AtomicDecrement(&size);
		//		return value;
		//	}
		//	int Size()const
		//	{
		//		return size;
		//	}
		//	bool IsLockFree()const
		//	{
		//		return false;
		//	}
		//private:
		//	NodePointer head;
		//	NodePointer tail;
		//	int size = 0;
		//	ztl::ztl_spin_lock::SpinLock lock;
		//};
		template<typename ValueType>
		class ConcurrencyQueue : public ConcurrencyQueueImpl<ValueType, sizeof(ValueType) <= 64>
		{
		public:
			using  ConcurrencyQueueImpl<ValueType, sizeof(ValueType) <= 64>::ConcurrencyQueueImpl;
		};

	}
}