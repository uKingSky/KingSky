/*****************************************
��������������
 *****************************************/
#include "config.h"

#define REQCNT 30
#define ADCPRS 9
#define LOOP 1

void  AdcTsAuto(void);
void AdcTsIntHandle(void);

 int xdata, ydata;
//extern OS_EVENT *TouchMbox;
//�����ж����������ж�  

#define	EnableIrq(bit)		rINTMSK &= ~(bit)

// ADCCON�Ĵ���
#define PRESCALE_DIS        (0 << 14)
#define PRESCALE_EN         (1 << 14)
#define PRSCVL(x)           ((x) << 6)
#define ADC_INPUT(x)        ((x) << 3)
#define ADC_START           (1 << 0)
#define ADC_ENDCVT          (1 << 15)

// ADCTSC�Ĵ���
#define UD_SEN          (1 << 8)
#define DOWN_INT        (UD_SEN*0)
#define UP_INT          (UD_SEN*1)
#define YM_SEN          (1 << 7)
#define YM_HIZ          (YM_SEN*0)
#define YM_GND          (YM_SEN*1)
#define YP_SEN          (1 << 6)
#define YP_EXTVLT       (YP_SEN*0)
#define YP_AIN          (YP_SEN*1)
#define XM_SEN          (1 << 5)
#define XM_HIZ          (XM_SEN*0)
#define XM_GND          (XM_SEN*1)
#define XP_SEN          (1 << 4)
#define XP_EXTVLT       (XP_SEN*0)
#define XP_AIN          (XP_SEN*1)
#define XP_PULL_UP      (1 << 3)
#define XP_PULL_UP_EN   (XP_PULL_UP*0)
#define XP_PULL_UP_DIS  (XP_PULL_UP*1)
#define AUTO_PST        (1 << 2)
#define CONVERT_MAN     (AUTO_PST*0)
#define CONVERT_AUTO    (AUTO_PST*1)
#define XP_PST(x)       (x << 0)

#define NOP_MODE        0
#define X_AXIS_MODE     1
#define Y_AXIS_MODE     2
#define WAIT_INT_MODE   3


/* ���ý���ȴ��ж�ģʽ��XP_PU,XP_Dis,XM_Dis,YP_Dis,YM_En
 * (1)����S3C2410��λ[8]ֻ��Ϊ0������ֻ��ʹ�������wait_down_int��
 *    ���ȵȴ�Pen Down�жϣ�Ҳ�ȴ�Pen Up�ж�
 * (2)����S3C2440��λ[8]Ϊ0��1ʱ�ֱ��ʾ�ȴ�Pen Down�жϻ�Pen Up�ж�
 */
/* ����"�ȴ��ж�ģʽ"���ȴ������������� */
#define wait_down_int() { rADCTSC = DOWN_INT | XP_PULL_UP_EN | \
                          XP_AIN | XM_HIZ | YP_AIN | YM_GND | \
                          XP_PST(WAIT_INT_MODE); }
/* ����"�ȴ��ж�ģʽ"���ȴ����������ɿ� */
#define wait_up_int()   { rADCTSC = UP_INT | XP_PULL_UP_EN | XP_AIN | XM_HIZ | \
                          YP_AIN | YM_GND | XP_PST(WAIT_INT_MODE); }

/* �����Զ�(����) X/Y������ת��ģʽ */
#define mode_auto_xy()  { rADCTSC = CONVERT_AUTO | XP_PULL_UP_DIS | XP_PST(NOP_MODE); }


/* 
 * INT_TC���жϷ������
 * ��������������ʱ�������Զ�(����) X/Y������ת��ģʽ��
 * �����������ɿ�ʱ������ȴ��ж�ģʽ���ٴεȴ�INT_TC�ж�
 */    
 extern ks_event* TouchMbox;
static void Isr_Tc(void)
{
    if (rADCDAT0 & 0x8000)
    {
       // Uart_Printf("Stylus Up!!\n");
        wait_down_int();    /* ����"�ȴ��ж�ģʽ"���ȴ������������� */
		ks_sem_sendmsg(TouchMbox);
    }
    else 
    {
        //Uart_Printf("Stylus Down: ");
        
        mode_auto_xy();     /* �����Զ�(����) X/Y������ת��ģʽ */
    
        /* ����λ[0]Ϊ1������A/Dת��
         * ע�⣺ADCDLYΪ50000��PCLK = 50MHz��
         *       Ҫ����(1/50MHz)*50000=1ms֮��ſ�ʼת��X����
         *       �پ���1ms֮��ſ�ʼת��Y����
         */
        rADCCON |= ADC_START;
    }
    
    // ��INT_TC�ж�
    rSUBSRCPND |= BIT_SUB_TC;
    rSRCPND    |= BIT_ADC;
    rINTPND    |= BIT_ADC;
}


/* 
 * INT_ADC���жϷ������
 * A/Dת������ʱ�������ж�
 * �ȶ�ȡX��Y����ֵ���ٽ���ȴ��ж�ģʽ
 */       
static void Isr_Adc(void)
{
    // ��ӡX��Y����ֵ
    xdata = (int)(rADCDAT0 & 0x3ff);
	ydata = (int)(rADCDAT1 & 0x3ff);
  //  Uart_Printf("xdata = %4d, ydata = %4d\r\n", xdata, ydata);

    /* �ж���S3C2410����S3C2440 */
    if ((rGSTATUS1 == 0x32410000) || (rGSTATUS1 == 0x32410002))
    {   // S3C2410
        wait_down_int();    /* ����"�ȴ��ж�ģʽ"���ȴ����������ɿ� */
    }
    else
    {   // S3C2440
        wait_up_int();      /* ����"�ȴ��ж�ģʽ"���ȴ����������ɿ� */
    }

    // ��INT_ADC�ж�
    rSUBSRCPND |= BIT_SUB_ADC;
    rSRCPND    |= BIT_ADC;
    rINTPND    |= BIT_ADC;
}

/* 
 * ADC�����������жϷ������
 * ����INT_TC��INT_ADC�жϣ��ֱ�������ǵĴ������
 */  

void Test_Ts(void)
{
    pISR_ADC = (int) AdcTsIntHandle;    // ����ADC�жϷ������
    
    rINTMSK &= ~BIT_ADC;          // ����ADC���ж�
    rINTSUBMSK &= ~(BIT_SUB_TC);  // ����INT_TC�жϣ��������������»��ɿ�ʱ�����ж�
    rINTSUBMSK &= ~(BIT_SUB_ADC); // ����INT_ADC�жϣ���A/Dת������ʱ�����ж�
    
    // ʹ��Ԥ��Ƶ���ܣ�����A/Dת������ʱ�� = PCLK/(49+1)
    rADCCON = PRESCALE_EN | PRSCVL(49);

    /* ������ʱʱ�� = (1/3.6864M)*50000 = 13.56ms
     * �����´��������ٹ�13.56ms�Ų���
     */
    rADCDLY = 50000;
  
    wait_down_int();    /* ����"�ȴ��ж�ģʽ"���ȴ������������� */
   
}


void AdcTsIntHandle(void)
{
	#if KS_CRITICAL_METHOD == 3                                // Allocate storage for CPU status register  
	KS_CPU_SR cpu_sr;
	#endif
	KS_ENTER_CRITICAL();
	ks_int_enter();
	KS_EXIT_CRITICAL();
    if (rSUBSRCPND & BIT_SUB_TC)
        Isr_Tc();

    if (rSUBSRCPND & BIT_SUB_ADC)
        Isr_Adc();
	ks_int_exit(); 
}
extern void beep(U32 freq, U32 ms);

void touch_exec(void)
{
if(xdata >637 & xdata <766 & ydata >160 & ydata < 211){ // stop
	GUI_StoreKey('s');
	beep(1000,10);
	return ;
}
if(xdata >780 & xdata <913 & ydata >170 & ydata <211){ //next
   GUI_StoreKey('n');
   beep(1000,10);
	return;
}
if(xdata >394 &xdata < 605 &ydata >659 & ydata <833){ // led1
     GUI_StoreKey(1);
     beep(1000,10);
	return;
}
if(xdata >412 &xdata <596 &ydata >600 &ydata <686){ //led2
	GUI_StoreKey(2);
     beep(1000,10);
	return;
}
if(xdata >411 &xdata <583 &ydata >442 &ydata <527){ //led3
	GUI_StoreKey(3);
     beep(1000,10);
	return;
}
if(xdata >412 &xdata <608 &ydata >267 &ydata <365){ //led4
	GUI_StoreKey(4);
     beep(1000,10);
	return;
}


}


