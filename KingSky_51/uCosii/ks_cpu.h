/*
*********************************************************************************************************
*                                               KingSky
*                                          ʵʱ����ϵͳ�ں�
*
*                                       (c) Copyright 2011, ׯͩȪ
*                                          All Rights Reserved
*
* File         : ks_cpu.h
********************************************************************************************************* */
#ifndef _KS_CPU_H
#define _KS_CPU_H
#include "STC89C51RC_RD_PLUS.h"

/*
*********************************************************************************************************
*                                            CPU ��������
									����ͬCPUӦ�ö��岻ͬ��������)
*                                         
**********************************************************************************************************/

typedef unsigned char   uint8;                  /* �޷���8λ���ͱ���                        */
typedef signed   char   int8;                   /* �з���8λ���ͱ���                        */
typedef unsigned int  uint16;                 /* �޷���16λ���ͱ���                       */
typedef signed   int  int16;                  /* �з���16λ���ͱ���                       */
typedef unsigned long    uint32;                 /* �޷���32λ���ͱ���                       */
typedef signed   long    int32;                  /* �з���32λ���ͱ���                       */
typedef float           fp32;                   /* �����ȸ�������32λ���ȣ�                 */
typedef double          fp64;                   /* ˫���ȸ�������64λ���ȣ�                 */
typedef unsigned char          KS_STACK;               /*�����ջ32λ��                            */

typedef uint8          KS_CPU_SR;               /*����CPU״̬�ֵĿ��Ϊ32λ                */


#define  KS_CRITICAL_METHOD    (1)

#if      KS_CRITICAL_METHOD == 1
#define  KS_ENTER_CRITICAL()  (EA = 0)   /* �жϽ�ֹ                        */
#define  KS_EXIT_CRITICAL()   (EA = 1)   /* �жϴ�                        */
#endif

#if      KS_CRITICAL_METHOD == 3
#define  KS_ENTER_CRITICAL()  (EA = 0)   /* �жϽ�ֹ                        */
#define  KS_EXIT_CRITICAL()   (EA = 1)   /* �жϴ�                        */
#endif

#define  KS_STK_GROWTH        (0)              /*��ջ��������   */

//extern void ks_thread_sw(void);

#define  KS_THREAD_SW()        ks_thread_sw()                          


#if KS_CRITICAL_METHOD == 3                          
KS_CPU_SR  ks_cpu_save(void);
void       ks_cpu_restore(KS_CPU_SR cpu_sr);
#endif





#endif