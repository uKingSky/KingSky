/*
*********************************************************************************************************
*                                                uC/GUI
*                        Universal graphic software for embedded applications
*
*                       (c) Copyright 2002, Micrium Inc., Weston, FL
*                       (c) Copyright 2002, SEGGER Microcontroller Systeme GmbH
*
*              µC/GUI is protected by international copyright laws. Knowledge of the
*              source code may not be used to write a similar product. This file may
*              only be used in accordance with a license and should not be redistributed
*              in any way. We appreciate your understanding and fairness.
*
----------------------------------------------------------------------
File        : GUIDEMO_Touch
Purpose     : Touch demo
---------------------------END-OF-HEADER------------------------------
*/
#include "config.h"
#include "GUI.h"
//#include "LCD_ConfDefaults.h"      /* valid LCD configuration */
#include "GUIDEMO.h"
#include "Button.h"
#include "Edit.h"

#if (GUI_WINSUPPORT && GUI_SUPPORT_TOUCH)

#define countof(Obj) (sizeof(Obj)/sizeof(Obj[0]))

/*********************************************************************
*
*       Static functions
*
**********************************************************************
*/

static int _Log2Phys(int l, I32 l0, I32 l1, I32 p0, I32 p1) {
  return p0 + ((p1 - p0) * (l - l0)) / (l1 - l0);
}

static void _Calibrate(int Coord, int Log0, int Log1, int Phys0, int Phys1, int *p0, int *p1) {
  int l0 = 0;
  int l1 = (Coord == GUI_COORD_X) ? LCD_XSIZE - 1 : LCD_YSIZE - 1;
  *p0 = _Log2Phys(l0, Log0, Log1, Phys0, Phys1);
  *p1 = _Log2Phys(l1, Log0, Log1, Phys0, Phys1);
}

/*********************************************************************
*
*       _ExecCalibration
*
**********************************************************************
*/

void _ExecCalibration(void) {
  int ax_Phys[2],ay_Phys[2];
/* calculate log. Positions */
  int ax[2] = { 15, LCD_XSIZE -1-15};
//  const int ay[2] = { 15, LCD_YSIZE-1-15};
  int ay[2] = { LCD_YSIZE-1-15, 15};
  GUI_TOUCH_SetDefaultCalibration();
/* _Calibrate upper left */
  GUI_SetBkColor(GUI_RED);  
  GUI_Clear();
  GUI_SetColor(GUI_WHITE);  GUI_FillCircle(ax[0], ay[0], 10);
  GUI_SetColor(GUI_RED);    GUI_FillCircle(ax[0], ay[0], 5);
  GUI_SetColor(GUI_WHITE);
  GUI_DispStringAt("Press here", ax[0]+20, ay[0]);
  do {
    GUI_TOUCH_tState State;
    GUI_TOUCH_GetState(&State);
    if (State.Pressed) {
      ax_Phys[0] = State.x;//GUI_TOUCH_GetxPhys();
      ay_Phys[0] = State.y;//GUI_TOUCH_GetyPhys();
      break;
    }
    GUI_Delay (100);
  } while (1);
/* Tell user to release */
  GUI_Clear();
  GUI_DispStringAt("OK", ax[0]+20, ay[0]);
  do {
    GUI_TOUCH_tState State;
    GUI_TOUCH_GetState(&State);
    if (State.Pressed == 0) {
      break;
    }
    GUI_Delay (100);
  } while (1);
/* _Calibrate lower right */
  GUI_SetBkColor(GUI_RED);  
  GUI_Clear();
  GUI_SetColor(GUI_WHITE);  GUI_FillCircle(ax[1], ay[1], 10);
  GUI_SetColor(GUI_RED);    GUI_FillCircle(ax[1], ay[1], 5);
  GUI_SetColor(GUI_WHITE);
  GUI_SetTextAlign(GUI_TA_RIGHT);
  GUI_DispStringAt("Press here", ax[1]-20, ay[1]);
  do {
    GUI_TOUCH_tState State;
    GUI_TOUCH_GetState(&State);
    if (State.Pressed) {
      ax_Phys[1] = GUI_TOUCH_GetxPhys();//State.x;//
      ay_Phys[1] = GUI_TOUCH_GetyPhys();//State.y;//
      break;
    }
    GUI_Delay (100);
  } while (1);
  GUI_TOUCH_Calibrate(GUI_COORD_X, ax[0], ax[1], ax_Phys[0], ax_Phys[1]);
  GUI_TOUCH_Calibrate(GUI_COORD_Y, ay[0], ay[1], ay_Phys[0], ay_Phys[1]);
  { /* calculate and display values for configuration file */
    int x0, x1;
    int y0, y1;
    GUI_Clear();
    _Calibrate(GUI_COORD_X, ax[0], ax[1], ax_Phys[0], ax_Phys[1], &x0, &x1);
    _Calibrate(GUI_COORD_Y, ay[0], ay[1], ay_Phys[0], ay_Phys[1], &y0, &y1);
    GUI_DispStringAt("x0: ", 0, 0); GUI_DispDec(x0, 4); GUI_DispNextLine();
    GUI_DispString  ("x1: ");       GUI_DispDec(x1, 4); GUI_DispNextLine();
    GUI_DispString  ("y0: ");       GUI_DispDec(y0, 4); GUI_DispNextLine();
    GUI_DispString  ("y1: ");       GUI_DispDec(y1, 4); GUI_DispNextLine();
    GUI_DispString  ("Please touch display to continue...");
    GUI_Delay(1000);
    do {
      GUI_TOUCH_tState State;
      GUI_TOUCH_GetState(&State);
      if (State.Pressed)
        break;
      GUI_Delay (10);
    } while (1);
  }
}

/*********************************************************************
*
*       _TestCalibration
*
**********************************************************************
*/
#define LED_OFF   0
#define LED_ON    1
extern KS_DEVICE_HANDLE led_handle;

void _TestCalibration(void) {
    BUTTON_Handle hButton1,hButton2,hButton3,hButton4;
	unsigned char state = 0x00;
	char c;
	int count = 0;
	GUI_SetBkColor(GUI_LIGHTGREEN); 
	GUI_SetColor(GUI_WHITE); 
	GUI_Clear();
	hButton1 =  BUTTON_Create( 120, 15, 80,  40, 1, BUTTON_CF_SHOW );
	hButton2 =  BUTTON_Create( 120, 60, 80,  40, 2, BUTTON_CF_SHOW );
	hButton3 =  BUTTON_Create( 120, 105, 80, 40, 3, BUTTON_CF_SHOW );
	hButton4 =  BUTTON_Create( 120, 150, 80, 40, 4, BUTTON_CF_SHOW );
	GUI_SetFont(&GUI_Font16B_1);
	GUI_SetColor(GUI_RED);
	GUI_DispStringHCenterAt("LED1",60,30);
	GUI_DispStringHCenterAt("LED2",60,75);
	GUI_DispStringHCenterAt("LED3",60,120);
	GUI_DispStringHCenterAt("LED4",60,165);

	if((rGPBDAT & (1<<5)) == (1<<5)){ //led1 off state
	    state &= ~(1<<0);
	    BUTTON_SetText (hButton1, "open");
		BUTTON_SetFont (hButton1, &GUI_FontComic18B_ASCII);
		GUI_SetColor(GUI_BLACK);
		GUI_FillCircle(100, 35, 10);
	}
	else{                        //led1 on state
		 state |= (1<<0);
		BUTTON_SetText (hButton1, "close");
		BUTTON_SetFont (hButton1, &GUI_FontComic18B_ASCII);
		GUI_SetColor(GUI_RED);
		GUI_FillCircle(100, 35, 10);
	}
	if((rGPBDAT & (1<<6)) == (1<<6)){ //led2 off state
		state &= ~(1<<1);
	    BUTTON_SetText (hButton2, "open");
	    BUTTON_SetFont (hButton2, &GUI_FontComic18B_ASCII);
		GUI_SetColor(GUI_BLACK);
		GUI_FillCircle(100, 80, 10);
	}
	else{//led2 on state
		state |= (1<<1);
		BUTTON_SetText (hButton2, "close");
	    BUTTON_SetFont (hButton2, &GUI_FontComic18B_ASCII);
		GUI_SetColor(GUI_RED);
		GUI_FillCircle(100, 80, 10);
	}
	if((rGPBDAT & (1<<7)) == (1<<7)){ //led3 off state
		state &= ~(1<<2);
	    BUTTON_SetText (hButton3, "open");
	    BUTTON_SetFont (hButton3, &GUI_FontComic18B_ASCII);
		GUI_SetColor(GUI_BLACK);
		GUI_FillCircle(100, 125, 10);
	}
	else{//led3 on state
		state |= (1<<2);
		BUTTON_SetText (hButton3, "close");
	    BUTTON_SetFont (hButton3, &GUI_FontComic18B_ASCII);
		GUI_SetColor(GUI_RED);
		GUI_FillCircle(100, 125, 10);
	}
	if((rGPBDAT & (1<<8)) == (1<<8)){ //led4 off state
	    state &= ~(1<<3);
		BUTTON_SetText (hButton4, "open");
		BUTTON_SetFont (hButton4, &GUI_FontComic18B_ASCII);
		GUI_SetColor(GUI_BLACK);
		GUI_FillCircle(100, 170, 10);
	}
	else{//led4 on state
	    state |= (1<<3);
		BUTTON_SetText (hButton4, "close");
		BUTTON_SetFont (hButton4, &GUI_FontComic18B_ASCII);
		GUI_SetColor(GUI_RED);
		GUI_FillCircle(100, 170, 10);
	}
	while (1) {
		if(++count == 500) break;
		c = GUI_GetKey();
		if (c=='n') break;
		switch (c)
		{
		case 1:
			if((state & (1<<0)) == 1){//led1 on
				ks_device_ioctl(led_handle,LED_OFF,c);
				BUTTON_SetText (hButton1, "open");
				BUTTON_SetFont (hButton1, &GUI_FontComic18B_ASCII);
				state &= ~(1<<0);
				GUI_SetColor(GUI_BLACK);
				GUI_FillCircle(100, 35, 10);
			}
			else if((state & (1<<0)) == 0){
				ks_device_ioctl(led_handle,LED_ON,c);
				BUTTON_SetText (hButton1, "close");
				BUTTON_SetFont (hButton1, &GUI_FontComic18B_ASCII);
				GUI_SetColor(GUI_RED);
				GUI_FillCircle(100, 35, 10);
				state |= (1<<0);
			}
			count = 0;
			break;
		case 2:
			if((state & (1<<1)) == (1<<1)){
				ks_device_ioctl(led_handle,LED_OFF,c);
				BUTTON_SetText (hButton2, "open");
				BUTTON_SetFont (hButton2, &GUI_FontComic18B_ASCII);
				state &= ~(1<<1);
				GUI_SetColor(GUI_BLACK);
				GUI_FillCircle(100, 80, 10);
			}
			else if((state & (1<<1)) == 0){
				ks_device_ioctl(led_handle,LED_ON,c);
				BUTTON_SetText (hButton2, "close");
				BUTTON_SetFont (hButton2, &GUI_FontComic18B_ASCII);
				state |= (1<<1);
				GUI_SetColor(GUI_RED);
				GUI_FillCircle(100, 80, 10);
			}
			count = 0;
			break;
		case 3:
			if((state & (1<<2)) == (1<<2)){
				ks_device_ioctl(led_handle,LED_OFF,c);
				BUTTON_SetText (hButton3, "open");
				BUTTON_SetFont (hButton3, &GUI_FontComic18B_ASCII);
				state &= ~(1<<2);
				GUI_SetColor(GUI_BLACK);
				GUI_FillCircle(100, 125, 10);
			}
			else if((state & (1<<2)) == 0){
				ks_device_ioctl(led_handle,LED_ON,c);
				BUTTON_SetText (hButton3, "close");
				BUTTON_SetFont (hButton3, &GUI_FontComic18B_ASCII);
				state |= (1<<2);
				GUI_SetColor(GUI_RED);
				GUI_FillCircle(100, 125, 10);
			}
			count = 0;
			break;
		case 4:
			if((state & (1<<3)) == (1<<3)){
				ks_device_ioctl(led_handle,LED_OFF,c);
				BUTTON_SetText (hButton4, "open");
				BUTTON_SetFont (hButton4, &GUI_FontComic18B_ASCII);
				state &= ~(1<<3);
				GUI_SetColor(GUI_BLACK);
				GUI_FillCircle(100, 170, 10);
			}
			else if((state & (1<<3)) == 0){
				ks_device_ioctl(led_handle,LED_ON,c);
				BUTTON_SetText (hButton4, "close");
				BUTTON_SetFont (hButton4, &GUI_FontComic18B_ASCII);
				state |= (1<<3);
				GUI_SetColor(GUI_RED);
				GUI_FillCircle(100, 170, 10);
			}
			count = 0;
			break;
		default:
			break;
		}
		
		GUI_Delay (10);
	}
	EDIT_Delete(hButton1);
	EDIT_Delete(hButton2);
	EDIT_Delete(hButton3);
	EDIT_Delete(hButton4);
}

/*********************************************************************
*
*       _ExecKeyboard
*
**********************************************************************

This creates a sample keyboard.
The routine returns after ENTER or ESC has been pressed.
*/

#if (LCD_XSIZE == 320) && (LCD_YSIZE == 240)
static char _acText[] = { '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '='
                        ,0, 'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P'
                        ,0, 'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', 'Q'
                        ,0, 'Z', 'X', 'C', 'V', 'B', 'N', 'M', 'Ö', 'Ä', 'Ü'
                        ,' ', ' ', ' ', ' ', ' ', 0, 0
};

int _ExecKeyboard(void) {
  int i;
  int Key;
  BUTTON_Handle ahButton[52];
  BUTTON_Handle hButtonESC;
  EDIT_Handle   hEdit;
  GUI_RECT rText = {000,0, LCD_XSIZE, 20};
  GUI_SetBkColor(GUI_RED);  
  GUI_Clear();
  GUI_DrawBitmap(&bmMicriumLogo, 5, 5); 
  GUI_SetFont(&GUI_Font16B_1);
  GUI_SetColor(GUI_WHITE);
  GUI_DispStringInRect("µC/GUI", &rText, GUI_TA_RIGHT | GUI_TA_VCENTER);
  rText.y0 +=20;
  rText.y1 +=20;
  GUI_DispStringInRect("Touch screen demo", &rText, GUI_TA_RIGHT | GUI_TA_VCENTER);
  /* Create Keyboard Buttons */
  for (i=0; i< 51; i++) {
    int Pos = (i < 47) ? i : i+4;
    int x0 = 5  + 28*(Pos%11);
    int y0 = 100 + 28*(Pos/11);
    char c = _acText[i];
    int Id = c ? c : 1;
    char ac[2] = {0};
    char *s= ac;
    ac[0] = c;
    ahButton[i] = BUTTON_Create( x0, y0, 25, 25, Id,BUTTON_CF_SHOW );
    BUTTON_SetText   (ahButton[i], s);
    #if GUI_SUPPORT_MEMDEV
      BUTTON_EnableMemdev(ahButton[i]);
    #endif
  }
  ahButton[i] = BUTTON_Create( 89, 212, 109, 25, ' ',BUTTON_CF_SHOW );
  hButtonESC = BUTTON_Create( 230, 40, 80, 25, GUI_ID_CANCEL,BUTTON_CF_SHOW );
  BUTTON_SetText   (hButtonESC, "ESC");
  hEdit = EDIT_Create( 5, 70, 310, 25, ' ', 80, 0 );
  EDIT_SetFont(hEdit, &GUI_Font8x16);
  BUTTON_SetBkColor(ahButton[49], 0, GUI_RED);
  BUTTON_SetBkColor(ahButton[50], 0, GUI_BLUE);
  /* Handle Keyboard until ESC or ENTER is pressed */
  do {
    Key = GUIDEMO_WaitKey();
    switch (Key) {
    case 0:
    case GUI_ID_CANCEL:
     break;
    default:
      EDIT_AddKey(hEdit, Key);
    }
  } while ((Key != 'N') && (Key!=GUI_ID_CANCEL) && (Key!=0));
  /* Cleanup */
  for (i=0; i< countof(ahButton); i++) {
    BUTTON_Delete(ahButton[i]);
  }
  BUTTON_Delete(hButtonESC);
  EDIT_Delete(hEdit);
  return Key;
}

#else

static char _acText[] = {
  '7','8','9','/',
  '4','5','6','*',
  '1','2','3','-',
  '0',',','+','=',0
};

int _ExecKeyboard(void) {
  int y0 = 75;
  int XSize = LCD_GetXSize();
  int YSize = LCD_GetYSize();
  int XStep = XSize / 4;
  int YStep = (YSize - y0) / 4;
  int i;
  int Key;
  BUTTON_Handle ahButton[16];
  BUTTON_Handle hButtonESC;
  EDIT_Handle   hEdit;
  GUI_RECT rText = {0};
  rText.x1 = LCD_GetXSize() - 3;
  rText.y1 = 15;
  GUI_SetBkColor(GUI_RED);  
  GUI_Clear();
  GUI_SetFont(&GUI_Font16B_1);
  GUI_SetColor(GUI_WHITE);
  GUI_DispStringInRect("µC/GUI", &rText, GUI_TA_RIGHT | GUI_TA_VCENTER);
  rText.y0 +=20;
  rText.y1 +=20;
  GUI_DispStringInRect("Touch screen demo", &rText, GUI_TA_RIGHT | GUI_TA_VCENTER);
  /* Create Keyboard Buttons */
  for (i=0; _acText[i]; i++) {
    int XPos = (i%4) * XStep + 3;
    int YPos = (i/4) * YStep + 3 + y0;
    char c = _acText[i];
    int Id = c ? c : 1;
    char ac[2] = {0};
    char *s= ac;
    ac[0] = c;
    ahButton[i] = BUTTON_Create( XPos, YPos, XStep - 5, YStep - 5, Id, BUTTON_CF_SHOW );
    BUTTON_SetText(ahButton[i], s);
    BUTTON_EnableMemdev(ahButton[i]);
  }
  hButtonESC = BUTTON_Create( 3, 3, 80, 25, GUI_ID_CANCEL, BUTTON_CF_SHOW );
  BUTTON_SetText(hButtonESC, "ESC");
  hEdit = EDIT_Create( 5, y0 - 30, XSize - 10, 25, ' ', 80, 0 );
  EDIT_SetFont(hEdit, &GUI_Font8x16);
  /* Handle Keyboard until ESC or ENTER is pressed */
  do {
    Key = GUIDEMO_WaitKey();
    switch (Key) {
    case 0:
    case GUI_ID_CANCEL:
     break;
    default:
      EDIT_AddKey(hEdit, Key);
    }
  } while ((Key != 'N') && (Key!=GUI_ID_CANCEL) && (Key!=0));
  /* Cleanup */
  for (i=0; i< countof(ahButton); i++) {
    BUTTON_Delete(ahButton[i]);
  }
  BUTTON_Delete(hButtonESC);
  EDIT_Delete(hEdit);
  return Key;
}

#endif

/*
**********************************************************************
*
*              USER_Task
*
**********************************************************************
*/

#if GUIDEMO_LARGE

void GUIDEMO_Touch(void) {
  #define ID_KEYBOARD  1
  #define ID_TESTCAL   2
  #define ID_CALIBRATE 3
  int i, r;
  int XSize = LCD_GetXSize();
  int YSize = LCD_GetYSize();
  int XMid = XSize / 2;
  int YMid = YSize / 2;
  GUIDEMO_NotifyStartNext();
  GUIDEMO_HideInfoWin();
  do {
    GUI_RECT rText;/*= {0, 80, XSize, 120};*/
    BUTTON_Handle ahButton[3];
    rText.x0=0;
    rText.y0=60;
    rText.x1=XSize;
    rText.y1=100;
    GUI_SetBkColor(GUI_BLUE);
    GUI_Clear();
    GUI_DrawBitmap(&bmMicriumLogo, (XSize - 1 - bmMicriumLogo.XSize) / 2, 15);
    GUI_SetFont(&GUI_Font24B_1);
    GUI_DispStringInRect("µC/GUI Touch screen demo", &rText, GUI_TA_HCENTER | GUI_TA_VCENTER);
	GUI_SetFont(&GUI_Font16B_1);
	ahButton[0] =  BUTTON_Create( XMid - 50, YMid - 20, 100, 50, ID_CALIBRATE,BUTTON_CF_SHOW );
    ahButton[1] =  BUTTON_Create( XMid - 90, YMid + 40, 80, 30, ID_KEYBOARD, BUTTON_CF_SHOW );
    ahButton[2] =  BUTTON_Create( XMid + 10, YMid + 40, 80, 30, ID_TESTCAL,BUTTON_CF_SHOW );
    BUTTON_SetText (ahButton[0], "Calibrate");
    BUTTON_SetBkColor(ahButton[0], 0, GUI_RED);
    BUTTON_SetText (ahButton[1], "Keyboard");
    BUTTON_SetText (ahButton[2], "Test calibration");
    BUTTON_SetFont(ahButton[0], &GUI_FontComic18B_ASCII);
    r = GUIDEMO_WaitKey();
    //r = ID_CALIBRATE;	// modefied by scw
    if (r==0) {
      r = ID_KEYBOARD;
      BUTTON_SetState(ahButton[1],BUTTON_STATE_PRESSED);
      GUIDEMO_Delay(500);
    }
    for (i=0; i< countof(ahButton); i++) {
      BUTTON_Delete(ahButton[i]);
    }
    switch (r) {
    case ID_KEYBOARD:  r= _ExecKeyboard(); break;
    case ID_CALIBRATE: _ExecCalibration(); break;
    case ID_TESTCAL:   _TestCalibration(); break;
    }
  } while (r && (r!='n') && (r!='N'));
}

#else

void GUIDEMO_Touch(void) {}

#endif

#else

void GUIDEMO_Touch(void) {} /* avoid empty object files */

#endif /* #if GUI_WINSUPPORT */
