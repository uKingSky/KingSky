/*
*********************************************************************************************************
*                                               KingSky
*                                          ʵʱ����ϵͳ�ں�
*
*                                       (c) Copyright 2011, ׯͩȪ
*                                          All Rights Reserved
*
* File         : ks_cpu.c
********************************************************************************************************* */
#include "KingSky.h"



 KS_STACK * ks_stack_init(THREAD_ADDR thread_addr,void *p_arg,KS_STACK *stk1) reentrant
{
    KS_STACK *stk;
    p_arg = p_arg; 
    stk    = stk1;                              //�û���ջ�����Ч��ַ
    *stk++ = 15;                                //�û���ջ����
    *stk++ = (uint16)thread_addr & 0xFF;               //�����ַ��8λ
    *stk++ = (uint16)thread_addr >> 8;                 //�����ַ��8λ    
    *stk++ = 0x0A;                              //ACC
    *stk++ = 0x0B;                              //B
    *stk++ = 0x00;                              //DPH
    *stk++ = 0x00;                              //DPL
    *stk++ = 0x00;                              //PSW
    *stk++ = 0x00;                              //R0
    
	//R3��R2��R1���ڴ����������ppdata������R3����洢�����ͣ�R2Ϊ���ֽ�ƫ�ƣ�R1Ϊ���ֽ�λ�ơ�
	//ͨ������KEIL��࣬�˽⵽�����void *ppdata����ǡ������R3��R2��R1���ݣ�����ͨ�������ջ��
    *stk++ = (uint16)p_arg & 0xFF;             //R1
    *stk++ = (uint16)p_arg >> 8;               //R2
    *stk++ = 0x01;                              //R3  ��Ϊ���õ�ȫ��XDATA�����Դ洢�����͹̶�Ϊ1����C51.PDF��178ҳ˵����

    *stk++ = 0x04;                              //R4
    *stk++ = 0x05;                              //R5
    *stk++ = 0x06;                              //R6
    *stk++ = 0x07;                              //R7
                                                //���ñ���SP�������л�ʱ�����û���ջ���ȼ���ó���    
    *stk++ = (uint16) (stk1+MaxStkSize) >> 8;   //?C_XBP �����ջָ���8λ
    *stk++ = (uint16) (stk1+MaxStkSize) & 0xFF; //?C_XBP �����ջָ���8λ
        
    return ((void *)stk1);

}

void UserTickTimer(void) reentrant
{
/*	TH0=0x70;    		//��ͨ51��ʱ����ʽ1�������ڷ����ж�ʱ�����¸�ֵ���ٴ�������ʱ
    TL0=0;            	//Tick=50��/��(��0.02��/��)������22.1184M	   */
	TH0=0xB8;    		//��ͨ51��ʱ����ʽ1�������ڷ����ж�ʱ�����¸�ֵ���ٴ�������ʱ
    TL0=0;            	//Tick=50��/��(��0.02��/��)������11.0592M
//	ET0=1;              //����T0�ж�
    TR0=1;
}

/* 
  ucOS-IIϵͳʱ���жϴ������
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
  ����Ӳ���Ĵ����ĳ�ʼֵ��
  ��ʼ����ʱ��0,��ΪucOS-II��ϵͳʱ�ӡ�
  ������������Ӳ����صĳ�ʼ��Ҳ���Է������
*/

void InitHardware(void) reentrant
{   
    TMOD = 0x01;   //��ʱ��0��ģʽ1(16λ��ʱ��)������TR0���ƣ���ʱ��1�������ʷ�����
    TMOD=TMOD&0xF0;
    TMOD=TMOD|0x01;    //ģʽ1(16λ��ʱ��)������TR0����
//	TMOD = 0x11;
   /* TH0  = 0x70;   //����Tick=50��/��(��0.02��/��),TH,TLֵ��CPU��Ƶ���й�(22.1184M)
    TL0  = 0x00;   //OS_CPU_C.C�ж�ʱ���ж���ӦҲҪ���ã�OS_CFG.H��OS_TICKS_PER_SECҲ�й�ϵ		*/
    TH0=0xB8;    		//��ͨ51��ʱ����ʽ1�������ڷ����ж�ʱ�����¸�ֵ���ٴ�������ʱ
    TL0=0;            	//Tick=50��/��(��0.02��/��)������11.0592M
    //ET0  = 1;    //����T0�ж�(�ڵ�һ������ʼִ��ʱ�ſ�ʱ���ж�,������һ�ж�ϵͳ���벻��֪״̬)
    TR0  = 1;
	ET0=1;  //��ʱ�ӽ����ж�
//	EA = 1;
}