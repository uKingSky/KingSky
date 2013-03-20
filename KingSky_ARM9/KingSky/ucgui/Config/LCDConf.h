/*
*********************************************************************************************************
*                                                uC/GUI
*                        Universal graphic software for embedded applications
*
*                       (c) Copyright 2002, Micrium Inc., Weston, FL
*                       (c) Copyright 2002, SEGGER Microcontroller Systeme GmbH
*
*              礐/GUI is protected by international copyright laws. Knowledge of the
*              source code may not be used to write a similar product. This file may
*              only be used in accordance with a license and should not be redistributed
*              in any way. We appreciate your understanding and fairness.
*
----------------------------------------------------------------------
File        : LCDConf_1375_C8_C320x240.h
Purpose     : Sample configuration file
----------------------------------------------------------------------
*/
#ifndef __LCDCONF_H
#define __LCDCONF_H

/*********************************************************************
*
*                   General configuration of LCD
*
**********************************************************************
*/

#define LCD_XSIZE        (320)          /* X-resolution of LCD, Logical coor. */
#define LCD_YSIZE        (240)          /* Y-resolution of LCD, Logical coor. */

#define LCD_BITSPERPIXEL  (16)          // Bpp.

#define LCD_BUSWIDTH      (8)

#define LCD_CONTROLLER    1375         	// 控制器型号.

#define LCD_FIXEDPALETTE  (565)

#define LCD_SWAP_RB       1             // 红绿位置交换.
//#define BUTTON_SUPPORT_STREAMED_BITMAP 1
//显示一个指定按钮对象时设置使用的位图数据流

/*********************************************************************
*                   Full bus configuration
**********************************************************************
*/

/*
#define LCD_READ_MEM(Off)            *((U16*)         (0xc00000+(((U32)(Off))<<1)))
#define LCD_WRITE_MEM(Off,data)      *((U16*)         (0xc00000+(((U32)(Off))<<1)))=data
#define LCD_READ_REG(Off)            *((volatile U16*)(0xc1ffe0+(((U16)(Off))<<1)))
#define LCD_WRITE_REG(Off,data)      *((volatile U16*)(0xc1ffe0+(((U16)(Off))<<1)))=data
// 将所有对 Video RAM 的操作放在 LCD13XX.C 文件中, 避免编译问题.
*/

/*********************************************************************
*                   Init sequence for 8 bit access
**********************************************************************
*/
extern int  LCD_L0_Init(void);    // 初始化 LCD 控制器.

#define LCD_INIT_CONTROLLER()  LCD_L0_Init ()

#endif /* LCDCONF_H */
