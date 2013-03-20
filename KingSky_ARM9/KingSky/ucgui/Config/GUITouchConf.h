/*
*********************************************************************************************************
*                                                �C/GUI
*                        Universal graphic software for embedded applications
*
*                       (c) Copyright 2002, Micrium Inc., Weston, FL
*                       (c) Copyright 2000, SEGGER Microcontroller Systeme GmbH          
*
*              �C/GUI is protected by international copyright laws. Knowledge of the
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

//��ע���Ǵ������꣬����lcd����
#define GUI_TOUCH_AD_LEFT  	85    //��A/D ת�������ص���Сֵ
#define GUI_TOUCH_AD_RIGHT  889    //��A/D ת�������ص����ֵ
#define GUI_TOUCH_SWAP_XY  	1       //��Ϊ1 ��X ���Y ���໥����
#define GUI_TOUCH_MIRROR_X 	1       //X �᾵��
#define GUI_TOUCH_MIRROR_Y 	1        //Y �᾵��
#define GUI_TOUCH_AD_TOP 	94      //��A/D ת�������ص���Сֵ
#define GUI_TOUCH_AD_BOTTOM 923      //��A/D ת�������ص����ֵ
#define GUI_TOUCH_XSIZE  LCD_XSIZE   //�����������ǵ�ˮƽ���� (����Ĭ��ֵ)
#define GUI_TOUCH_YSIZE LCD_YSIZE    //�����������ǵĴ�ֱ����
#endif /* GUITOUCH_CONF_H */
