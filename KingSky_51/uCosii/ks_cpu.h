/*
*********************************************************************************************************
*                                               KingSky
*                                          实时操作系统内核
*
*                                       (c) Copyright 2011, 庄桐泉
*                                          All Rights Reserved
*
* File         : ks_cpu.h
********************************************************************************************************* */
#ifndef _KS_CPU_H
#define _KS_CPU_H
#include "STC89C51RC_RD_PLUS.h"

/*
*********************************************************************************************************
*                                            CPU 数据类型
									（不同CPU应该定义不同数据类型)
*                                         
**********************************************************************************************************/

typedef unsigned char   uint8;                  /* 无符号8位整型变量                        */
typedef signed   char   int8;                   /* 有符号8位整型变量                        */
typedef unsigned int  uint16;                 /* 无符号16位整型变量                       */
typedef signed   int  int16;                  /* 有符号16位整型变量                       */
typedef unsigned long    uint32;                 /* 无符号32位整型变量                       */
typedef signed   long    int32;                  /* 有符号32位整型变量                       */
typedef float           fp32;                   /* 单精度浮点数（32位长度）                 */
typedef double          fp64;                   /* 双精度浮点数（64位长度）                 */
typedef unsigned char          KS_STACK;               /*定义堆栈32位宽                            */

typedef uint8          KS_CPU_SR;               /*定义CPU状态字的宽度为32位                */


#define  KS_CRITICAL_METHOD    (1)

#if      KS_CRITICAL_METHOD == 1
#define  KS_ENTER_CRITICAL()  (EA = 0)   /* 中断禁止                        */
#define  KS_EXIT_CRITICAL()   (EA = 1)   /* 中断打开                        */
#endif

#if      KS_CRITICAL_METHOD == 3
#define  KS_ENTER_CRITICAL()  (EA = 0)   /* 中断禁止                        */
#define  KS_EXIT_CRITICAL()   (EA = 1)   /* 中断打开                        */
#endif

#define  KS_STK_GROWTH        (0)              /*堆栈增长方向   */

//extern void ks_thread_sw(void);

#define  KS_THREAD_SW()        ks_thread_sw()                          


#if KS_CRITICAL_METHOD == 3                          
KS_CPU_SR  ks_cpu_save(void);
void       ks_cpu_restore(KS_CPU_SR cpu_sr);
#endif





#endif