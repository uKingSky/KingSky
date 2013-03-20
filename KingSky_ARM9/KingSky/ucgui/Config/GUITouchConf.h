/*
*********************************************************************************************************
*                                                礐/GUI
*                        Universal graphic software for embedded applications
*
*                       (c) Copyright 2002, Micrium Inc., Weston, FL
*                       (c) Copyright 2000, SEGGER Microcontroller Systeme GmbH          
*
*              礐/GUI is protected by international copyright laws. Knowledge of the
*              source code may not be used to write a similar product. This file may
*              only be used in accordance with a license and should not be redistributed 
*              in any way. We appreciate your understanding and fairness.
*
* File        : GUITouch.Conf.h
* Purpose     : Configures emWin GSC touch screen module
*********************************************************************************************************
*/


#ifndef GUITOUCH_CONF_H
#define GUITOUCH_CONF_H

//、注意是触摸坐标，不是lcd坐标
#define GUI_TOUCH_AD_LEFT  	85    //由A/D 转换器返回的最小值
#define GUI_TOUCH_AD_RIGHT  889    //由A/D 转换器返回的最大值
#define GUI_TOUCH_SWAP_XY  	1       //设为1 则X 轴和Y 轴相互交换
#define GUI_TOUCH_MIRROR_X 	1       //X 轴镜像
#define GUI_TOUCH_MIRROR_Y 	1        //Y 轴镜像
#define GUI_TOUCH_AD_TOP 	94      //由A/D 转换器返回的最小值
#define GUI_TOUCH_AD_BOTTOM 923      //由A/D 转换器返回的最大值
#define GUI_TOUCH_XSIZE  LCD_XSIZE   //被触摸屏覆盖的水平区域 (都是默认值)
#define GUI_TOUCH_YSIZE LCD_YSIZE    //被触摸屏覆盖的垂直区域
#endif /* GUITOUCH_CONF_H */
