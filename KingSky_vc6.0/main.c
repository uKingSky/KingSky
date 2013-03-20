
#include "..\..\KingSky\includes\KingSky.h"

CONTEXT Context;
HANDLE mainhandle;
uint8 flag = 1;		//����һ��ȫ�ֱ�������Ϊ�Ƿ�ʱ�ӵ��ȵı�־

KS_STACK  main_thread_stack[KS_STACK_MAX_LEN];
KS_STACK  thread1_stack[KS_STACK_MAX_LEN];
KS_STACK  thread2_stack[KS_STACK_MAX_LEN];

ks_event *sem,*sem1;

static void ks_thread1(void *param);
static void ks_thread2(void *param);

KS_HANDLE h_thread1,h_thread2,h_thread3;

void timer_isr(void)
{
	printf("timer is reach\n");
}

void ks_vc_init(void)
{
	HANDLE cp,ct;
	Context.ContextFlags = CONTEXT_CONTROL;
	cp = GetCurrentProcess();	//�õ���ǰ���̾��
	ct = GetCurrentThread();	//�õ���ǰ�߳�α���
	DuplicateHandle(cp, ct, cp, &mainhandle, 0, TRUE, 2);	//α���ת��,�õ��߳�����
}

void ks_thread_main(void *param)
{

	param = param;
    timeSetEvent(1000/KS_TICKS_PER_SEC, 0, ks_tick_isr, 0, TIME_PERIODIC);	//����һ����ʱ���߳�,10 ticks/s
	h_thread1 = ks_thread_create(ks_thread1,"thread1",(void*)0,&thread1_stack[KS_STACK_MAX_LEN-1],1);
	h_thread2 = ks_thread_create(ks_thread2,"thread2",(void*)0,&thread2_stack[KS_STACK_MAX_LEN-1],2);

	for(;;)
	{
		printf("main\n");
		ks_thread_delay(10);
	}
}

static void ks_thread1(void *param)
{
	uint8 err;
	param = param;
	
//	ks_thread_suspend(ks_thread2);
	for(;;)
	{
		ks_sem_waitfor(sem,0,&err);
		printf("thread 1 is running\n");
		ks_sem_sendmsg(sem1);
		ks_thread_delay(10);
	}
}

static void ks_thread2(void *param)
{
	uint8 err;
	param = param;
	//ks_sem_sendmsg(sem);
	for(;;)
	{
		
		ks_sem_sendmsg(sem);
		printf("thread 2 is running\n");
		ks_sem_waitfor(sem1,0,&err);
	//	ks_thread_suspend(h_thread1);
		ks_thread_delay(10);
	}
}

int main()
{
	KS_HANDLE h_main;
	TIMER *timer;
	ks_vc_init();
	ks_system_initialize(); 
	h_main = ks_thread_create(ks_thread_main,"thread",(void*)0,&main_thread_stack[KS_STACK_MAX_LEN-1],0);
	sem = ks_sem_create(0);
	sem1 = ks_sem_create(0);
		timer = ks_timer_create(timer_isr);
	ks_timer_start(timer,10);
	ks_start();
	return 0;
}