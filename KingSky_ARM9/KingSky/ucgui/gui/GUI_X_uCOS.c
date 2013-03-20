/*
*********************************************************************************************************
*                                                uC/GUI
*                        Universal graphic software for embedded applications
*
*                       (c) Copyright 2002, Micrium Inc., Weston, FL
*                       (c) Copyright 2002, SEGGER Microcontroller Systeme GmbH
*
*              u/GUI is protected by international copyright laws. Knowledge of the
*              source code may not be used to write a similar product. This file may
*              only be used in accordance with a license and should not be redistributed
*              in any way. We appreciate your understanding and fairness.
*
----------------------------------------------------------------------
File        : GUI_X.C
Purpose     : COnfig / System dependent externals for GUI
---------------------------END-OF-HEADER------------------------------
*/
#include <stdio.h>
#include "option.H"
#include "gui_x.h"
static ks_event *EventMbox;
static ks_event * DispSem;
ks_event* TouchMbox;

extern int xdata,ydata;


/*********************************************************************
*
*      Timing:
*                 GUI_X_GetTime
*                 GUI_X_Delay

  Some timing dependent routines of emWin require a GetTime
  and delay funtion. Default time unit (tick), normally is
  1 ms.
*/

int GUI_X_GetTime(void) {
  return (int)ks_time_get();
}

void GUI_X_Delay(int Period) {
    uint32 ticks;
    ticks = (Period * 1000) / OS_TICKS_PER_SEC;
    ks_thread_delay((uint16)ticks);
                                  // GUI 程序对延时时间要求并不高,所以直接使用参数.
}

/*********************************************************************
*
*       GUI_X_ExecIdle()
*/
void GUI_X_ExecIdle(void) {
  ks_thread_delay(1);
}

/*********************************************************************
*
*      Multitasking:
*
*                 GUI_X_InitOS()
*                 GUI_X_GetTaskId()
*                 GUI_X_Lock()
*                 GUI_X_Unlock()

Note:
  The following routines are required only if emWin is used in a
  true multi task environment, which means you have more than one
  thread using the emWin API.
  In this case the
                      #define GUI_OS 1
  needs to be in GUIConf.h
*/



U32  GUI_X_GetTaskId (void)
{
    return ((uint32)(current_thread->priority));
}

void GUI_X_InitOS (void)
{
    DispSem = ks_sem_create(1);
    EventMbox = ks_sem_create(1);
}

void GUI_X_Unlock (void)
{
    ks_sem_sendmsg(DispSem);
}

void GUI_X_Lock (void) {
 uint8 err;

    ks_sem_waitfor(DispSem, 0, &err);
}

void GUI_X_WaitEvent (void) 
{
    uint8 err;
    (void)ks_sem_waitfor(EventMbox, 0, &err);
}
void GUI_X_SignalEvent (void) 
{
    (void)ks_sem_sendmsg(EventMbox);
}



/*********************************************************************
*
*       GUI_X_Init()
*
* Note:
*     GUI_X_Init() is called from GUI_Init is a possibility to init
*     some hardware which needs to be up and running before the GUI.
*     If not required, leave this routine blank.
*/
	extern void Test_Ts(void);


void GUI_X_Init(void)
{
   GUI_Key_Init();                 // 初始化一个按键信号量.
   TouchMbox = ks_sem_create(1);
   Test_Ts();     //此处接入触摸屏的中断设置
   GUI_TOUCH_SetDefaultCalibration();//用默认值校准触摸屏
}



extern void Uart_Printf (const char *format, ...);

void GUI_X_Log(const char *s)
{
#if 0
 static INT32U i;

    Uart_Printf (s);
	Uart_Printf ("\r");
    Uart_Printf ("------------------------ %i .\n\r",i++);
#endif
}

void GUI_X_Warn(const char *s)
{
}

void GUI_X_ErrorOut(const char *s)
{
}

int  GUI_TOUCH_X_MeasureX(void) { 
	
		   return xdata;
	 
	}  
	int  GUI_TOUCH_X_MeasureY(void){
	     return ydata;
	
	}
	void GUI_TOUCH_X_ActivateX(void) {}  
	void GUI_TOUCH_X_ActivateY(void) {}


/*
********************************************************************************************************
* End.
********************************************************************************************************
*/
