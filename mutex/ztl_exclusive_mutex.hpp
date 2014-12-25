#pragma once
#include "../forward.h"
namespace ztl
{
	namespace exclusive_mutex
	{
		/*
		�ṩ��ռʽmutex
		*/
		class Mutex
		{
		public:
			Mutex() = default;
			~Mutex()
			{
				HANDLE expect = NULL;
				event.compare_exchange_strong(expect, NULL);
				CloseHandle(expect);
			}
			Mutex(Mutex&&) = default;
			Mutex(const Mutex&) = delete;
			Mutex& operator=(const Mutex&) = delete;
			Mutex& operator=(Mutex&&) = default;
			//�ɹ���������,ʧ�ܹ���,count++
			void Lock()
			{
				while(lock.exchange(1))
				{
					count++;
					WaitForSingleObject(GetEvent(), INFINITE);
					count--;
				}
			}
			//����һ���ȴ����߳�,lock����Ϊ0.count--
			void Unlock()
			{
				assert(lock != 0);
				lock.store(0);
				SetEvent(GetEvent());
			}

			template <class Rep, class Period>
			bool TimedLockFor(const duration<Rep, Period>& rel_time)
			{
				return TimedLock(static_cast<DWORD>(rel_time.count()));
			}
			template <class Clock, class Duration>
			bool TimedLockUntil(const time_point<Clock, Duration>& abs_time)
			{
				return TimedLock(static_cast<DWORD>(abs_time.time_since_epoch().count()));
			}
			//�ɹ���������true,����ʧ�ܷ���false
			//����ͱ�׼�Ķ�ռʽmutexʵ�ֲ�һ��,��׼Ҫ�����trylock.ͬһ�߳�����ͻ�����.
			//��Ϊ�ҵ�ʵ�ֲ������̵߳�id,�������ʵ�ֲ���- -...
			//����Ҳû��Ҫʵ��,trylock������Ӧ���ǹ۲�����״̬�����Ի����.��������
			bool TryLock()
			{
				if(lock.exchange(1))
				{
					//����ʧ����
					return false;
				}
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
				while(lock.exchange(1))
				{
					//int i = -1;
					count++;
					auto result = WaitForSingleObject(GetEvent(), milliseconds);
					count--;
					switch(result)
					{
						case  WAIT_TIMEOUT:
							return false;
							//�����,���Ѻ�lock=0;Ӧ��ȥ����һ����
						case WAIT_OBJECT_0:
							//i = lock;
							//
							//assert(lock.load() == 0);
							//�����������߳�1������,���е�76�к��������л�.��һ���̳߳�ʱ����,return��ȥ��������lock.Ȼ����Ϊ��ʱ���߳�û����.�����ѵ��̻߳�û�����,��76�ͱ��л���
							//�����ʱ��������δ����״̬.����lock����������.֮���߳��л�,76�е��̼߳�������,������,lock������=1������0.����assertʧ��
							//����feature,not bug.�ǶԳ��������˽ⲻ����������.���ѵ��̻߳��ٴ���while������,�پ����ȴ����Ƿ���.���Զ������Ϊ�����������.
							break;
						default:
							cout << GetLastError() << endl;
							abort();
							break;
					}
				}
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
		};
	}
}