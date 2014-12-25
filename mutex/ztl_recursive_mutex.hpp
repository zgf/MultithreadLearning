#pragma once
#include "../forward.h"
namespace ztl
{
	namespace recursive_mutex
	{
		/*
		��, id��ͬ��ʱ��lock++; ��ͬʱ��˯��ȥ.unlock��ʱ��lock--;
		lock = 0��ʱ���ѵȴ��߳�, ֻ��һ���߳�ȥlock.Ȼ������id.*/

		class RecursiveMutex
		{
		public:
			RecursiveMutex() = default;
			~RecursiveMutex()
			{
				HANDLE expect = NULL;
				event.compare_exchange_strong(expect, NULL);
				CloseHandle(expect);
			}
			RecursiveMutex(RecursiveMutex&&) = default;
			RecursiveMutex(const RecursiveMutex&) = delete;
			RecursiveMutex& operator=(const RecursiveMutex&) = delete;
			RecursiveMutex& operator=(RecursiveMutex&&) = default;
			//�ɹ���������,ʧ�ܹ���,count++
			void Lock()
			{
				auto current_id = GetCurrentThreadId();
				while(lock.exchange(1))
				{
					//����Ϊ0
					//����Ƿ���ͬid
					if(current_id == ownerId)
					{
						//������ǰ�̶߳�ռ��
						break;
					}
					count++;
					WaitForSingleObject(GetEvent(), INFINITE);
					count--;
				}
				//���￪ʼ�������ڲ�
				lockCount++;
				ownerId = current_id;
			}
			//����һ���ȴ����߳�,lock����Ϊ0.count--
			void Unlock()
			{
				assert(lock != 0);
				assert(lockCount != 0);
				assert(GetCurrentThreadId() == ownerId);
				lockCount--;
				if(lockCount == 0)
				{
					lock = 0;
					ownerId = 0;
					SetEvent(GetEvent());
				}
			}

			template <class Rep, class Period>
			bool TimedLockFor(const duration<Rep, Period>& rel_time)
			{
				return TimedLock(rel_time.count());
			}
			template <class Clock, class Duration>
			bool TimedLockUntil(const time_point<Clock, Duration>& abs_time)
			{
				return TimedLock(abs_time.time_since_epoch().count());
			}
			//�ɹ���������true,����ʧ�ܷ���false
			//����ͱ�׼�Ķ�ռʽmutexʵ�ֲ�һ��,��׼Ҫ�����trylock.ͬһ�߳�����ͻ�����.
			//��Ϊ�ҵ�ʵ�ֲ������̵߳�id,�������ʵ�ֲ���- -...
			//����Ҳû��Ҫʵ��,trylock������Ӧ���ǹ۲�����״̬�����Ի����.��������
			bool TryLock()
			{
				auto current_id = GetCurrentThreadId();
				if(lock.exchange(1))
				{
					if(current_id != ownerId)
					{
						//����ʧ����
						return false;
					}
				}
				//������
				lockCount++;
				ownerId = current_id;
				return true;
			}
			bool IsLocked() const
			{
				return lock;
			}
		private:
			//��������,�ȴ��¼�ֱ����ʱ�ͷ���false
			bool TimedLock(DWORD milliseconds)
			{
				auto current_id = GetCurrentThreadId();
				while(lock.exchange(1))
				{
					if(current_id == ownerId)
					{
						//������ǰ�̶߳�ռ��
						break;
					}
					count++;
					auto result = WaitForSingleObject(GetEvent(), milliseconds);
					count--;
					switch(result)
					{
						case  WAIT_TIMEOUT:
							return false;
						case WAIT_OBJECT_0:
							break;
						default:
							cout << GetLastError() << endl;
							abort();
							break;
					}
				}
				lockCount++;
				ownerId = current_id;
				return true;
			}
			HANDLE GetEvent()
			{
				HANDLE expect = NULL;
				HANDLE new_event = CreateEvent(NULL, FALSE, FALSE, NULL);
				event.compare_exchange_strong(expect, new_event);
				return event;
			}

		private:
			atomic<HANDLE> event = 0;
			atomic<int> count = 0;
			atomic<bool> lock = 0;
			atomic<int>  lockCount = 0;
			atomic<DWORD> ownerId = 0;
		};
	}
}