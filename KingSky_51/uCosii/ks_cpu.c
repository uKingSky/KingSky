/*
*********************************************************************************************************
*                                               KingSky
*                                          实时操作系统内核
*
*                                       (c) Copyright 2011, 庄桐泉
*                                          All Rights Reserved
*
* File         : ks_cpu.c
********************************************************************************************************* */
#include "KingSky.h"



 KS_STACK * ks_stack_init(THREAD_ADDR thread_addr,void *p_arg,KS_STACK *stk1) reentrant
{
    KS_STACK *stk;
    p_arg = p_arg; 
    stk    = stk1;                              //用户堆栈最低有效地址
    *stk++ = 15;                                //用户堆栈长度
    *stk++ = (uint16)thread_addr & 0xFF;               //任务地址低8位
    *stk++ = (uint16)thread_addr >> 8;                 //任务地址高8位    
    *stk++ = 0x0A;                              //ACC
    *stk++ = 0x0B;                              //B
    *stk++ = 0x00;                              //DPH
    *stk++ = 0x00;                              //DPL
    *stk++ = 0x00;                              //PSW
    *stk++ = 0x00;                              //R0
    
	//R3、R2、R1用于传递任务参数ppdata，其中R3代表存储器类型，R2为高字节偏移，R1为低字节位移。
	//通过分析KEIL汇编，了解到任务的void *ppdata参数恰好是用R3、R2、R1传递，不是通过虚拟堆栈。
    *stk++ = (uint16)p_arg & 0xFF;             //R1
    *stk++ = (uint16)p_arg >> 8;               //R2
    *stk++ = 0x01;                              //R3  因为我用的全是XDATA，所以存储器类型固定为1，见C51.PDF第178页说明。

    *stk++ = 0x04;                              //R4
    *stk++ = 0x05;                              //R5
    *stk++ = 0x06;                              //R6
    *stk++ = 0x07;                              //R7
                                                //不用保存SP，任务切换时根据用户堆栈长度计算得出。    
    *stk++ = (uint16) (stk1+MaxStkSize) >> 8;   //?C_XBP 仿真堆栈指针高8位
    *stk++ = (uint16) (stk1+MaxStkSize) & 0xFF; //?C_XBP 仿真堆栈指针低8位
        
    return ((void *)stk1);

}

void UserTickTimer(void) reentrant
{
/*	TH0=0x70;    		//普通51定时器方式1，必须在发生中断时，重新赋值并再次启动计时
    TL0=0;            	//Tick=50次/秒(即0.02秒/次)，晶振22.1184M	   */
	TH0=0xB8;    		//普通51定时器方式1，必须在发生中断时，重新赋值并再次启动计时
    TL0=0;            	//Tick=50次/秒(即0.02秒/次)，晶振11.0592M
//	ET0=1;              //允许T0中断
    TR0=1;
}

/* 
  ucOS-II系统时钟中断处理程序
*/
  sbit LED2 = P2^3;


 

void ks_tick_isr(void) interrupt 1
{
 	LED2 = ~LED2;
    ks_int_enter();					// Must be called first at every hardware interrupt entry point 
    UserTickTimer();				// User functions can be called here.
   	ks_time_tick();					// Must be called during tick isr
   	ks_int_exit();					// Must be called finally at every hardware interupt exit point 
}

/*
  设置硬件寄存器的初始值。
  初始化定时器0,作为ucOS-II的系统时钟。
  还有其他的与硬件相关的初始化也可以放在这里。
*/

void InitHardware(void) reentrant
{   
    TMOD = 0x01;   //定时器0：模式1(16位定时器)，仅受TR0控制；定时器1：波特率发生器
    TMOD=TMOD&0xF0;
    TMOD=TMOD|0x01;    //模式1(16位定时器)，仅受TR0控制
//	TMOD = 0x11;
   /* TH0  = 0x70;   //定义Tick=50次/秒(即0.02秒/次),TH,TL值与CPU的频率有关(22.1184M)
    TL0  = 0x00;   //OS_CPU_C.C中定时器中断响应也要设置，OS_CFG.H中OS_TICKS_PER_SEC也有关系		*/
    TH0=0xB8;    		//普通51定时器方式1，必须在发生中断时，重新赋值并再次启动计时
    TL0=0;            	//Tick=50次/秒(即0.02秒/次)，晶振11.0592M
    //ET0  = 1;    //允许T0中断(在第一个任务开始执行时才开时钟中断,否则万一中断系统进入不可知状态)
    TR0  = 1;
	ET0=1;  //开时钟节拍中断
//	EA = 1;
}