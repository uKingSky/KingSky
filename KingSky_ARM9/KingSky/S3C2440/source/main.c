
#define	GLOBAL_CLK		1
#include "config.h"
#include "fs_api.h"


KS_STACK  main_thread_stack[KS_STACK_MAX_LEN];
KS_STACK  thread1_stack[KS_STACK_MAX_LEN];
KS_STACK  thread2_stack[KS_STACK_MAX_LEN];
KS_STACK  thread3_stack[KS_STACK_MAX_LEN];

static void ks_thread1(void *param);
static void ks_thread2(void *param);
static void ks_thread3(void *param);

KS_HANDLE h_thread1,h_thread2,h_thread3;
extern ks_event* TouchMbox;
extern ks_event *uart_queue;
extern void add_all_cmd(void);
extern void touch_exec(void);

void ks_print_info(void)
{
ks_log("\t********************************************\n");
ks_log("\t********                             *******\n");
ks_log("\t********  welcome to KingSky system  *******\n");
ks_log("\t********        KingSky 0.2          *******\n");
ks_log("\t********                             *******\n");
ks_log("\t********************************************\n");
}

void ks_thread_main(void *param)
{
	
#if KS_CRITICAL_METHOD == 3		/* Allocate storage for CPU status register */
		KS_CPU_SR  cpu_sr;
#endif
	param = param;
	KS_ENTER_CRITICAL();
	Timer0Init();				//initial timer0 for ucos time tick
	ISRInit();				    //initial interrupt prio or enable or disable
	KS_EXIT_CRITICAL();
#if KS_DEVICE_EN > 0
	ks_all_device_init();
#endif
    ks_print_info();
	h_thread1 = ks_thread_create(ks_thread1,"touch",(void*)0,&thread1_stack[KS_STACK_MAX_LEN-1],1);
	h_thread2 = ks_thread_create(ks_thread2,"cmd",(void*)0,&thread2_stack[KS_STACK_MAX_LEN-1],2);
	h_thread3 = ks_thread_create(ks_thread3,"cmd",(void*)0,&thread3_stack[KS_STACK_MAX_LEN-1],3);
	 GUI_Init();
	for(;;)
	{
	    GUIDEMO_main();
	}
}

static void ks_thread1(void *param)
{
	uint8 err;
	param = param;
	for(;;)
	{
		ks_sem_waitfor(TouchMbox,0,&err);
		touch_exec();
	}
}

static void ks_thread2(void *param)
{
	uint8 err;
	void *msg;
	void *queue_buf[1];
	param = param;
	uart_queue = ks_queue_create(&queue_buf[0],1);
#if KS_CMD_EN > 0
	 init_cmd();
	 add_all_cmd();
#endif
	for(;;)
	{
#if KS_CMD_EN > 0
	    Uart_Printf("%s# ",CMD_NAME);
        msg = ks_queue_waitfor(uart_queue,0,&err);
		run_command(msg);
		Uart_Printf("\n");
#endif
	}
}

static void ks_thread3(void *param)
{
	param = param;
	for(;;)
	{
		ks_thread_delay(100);
	}
}

void Main(void)
{
	 KS_HANDLE h_main;
	 uint8 err;
  	 TargetInit();                              /*ARM板子初始化*/
	 ks_system_initialize();                    /*系统初始化*/
	 h_main = ks_thread_create(ks_thread_main,NULL,(void*)0,&main_thread_stack[KS_STACK_MAX_LEN-1],0);
	 FS_Init();
	 init_ramdisk();
	 if(h_main == (KS_HANDLE)KS_PRIO_ERROR)
	 {	
	    ks_err("##create main thread err\n");
	 	return;//失败处理
	 }
	 ks_start();	 
}


