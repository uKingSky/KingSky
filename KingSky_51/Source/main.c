#include "includes.h"

sbit LED=P2^4;
sbit LED1=P2^5;

unsigned char  code Duan[]={0x3F, 0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F,0x38};
//����������ܣ�0-9�����

  unsigned char  Data_Buffer[4]={0,0,0,0};
//�ĸ��������ʾ��ֵ�������������

 unsigned char Min=0,Sec=0;//ʵʱʱ��
sbit P10=P1^0;		//�ĸ�����ܵ�λ��ڶ���
sbit P11=P1^1;
sbit P12=P1^2;
sbit P13=P1^3;

 xdata KS_STACK TaskStartStk[MaxStkSize+1];
 xdata KS_STACK TaskStartStk1[MaxStkSize+1];
extern void InitHardware() reentrant;

 unsigned char t = 0;
void clock1(void) reentrant
{
static unsigned char Bit=0;	//��̬�������˳������ֵ����
static unsigned char count=0;

EA =0;
 count++;
 if(count>=200)	//���ʱ����ʱ����ʱ5ms,��200��Ϊһ��
 {
 LED1=~LED1;
   count=0;
   Sec++;
   if(Sec>=60)
   {
	     Sec=0;     
		 Min++;	 		//�����������ʾ
		 if(Min>=60)	//ʱ���ʱ
		{
		 	Min=0; 	  
	 	}
   }
 }
  		 
 Bit++;	
 if(Bit>=4)Bit=0;
 P1|=0x0f;					//�ȹ�λ��
 P0=Duan[Data_Buffer[Bit]];	//������
 if(count<100&&Bit==1)P0|=0x80;//0.5S�м�С��������֮���𣬲���ѭ��
 switch(Bit)				//��λ��
 {
  case 0: P10=0;break;
  case 1: P11=0;break;
  case 2: P12=0;break;
  case 3: P13=0;break;
 }
 EA = 1;
}

   sbit LED3 = P2^6;
void Task(void *ppdata)	 reentrant
{
	ppdata=ppdata;
	for(;;)
    	{
		  KS_ENTER_CRITICAL();		
	 	  Data_Buffer[0]=Min/10;
		  Data_Buffer[1]=Min%10;		
		  Data_Buffer[2]=Sec/10;		
		  Data_Buffer[3]=Sec%10;
		  if(Data_Buffer[1] == 0) LED3 = ~LED3;
		  KS_EXIT_CRITICAL();
        ks_thread_delay(40);
    	}    
}
sbit KEY_UP = P2^1;
sbit KEY_DOWN = P2^2;


void Task1(void *ppdata) reentrant
{
	ppdata=ppdata;
	for(;;)
    	{
		LED=~LED;
		if(KEY_UP == 0){ 
		 Sec ++;
		 if(Sec >= 60){
		 Sec = 0;
		 Min++; 
		 }
		 if(Min>=60)	//ʱ���ʱ
		{
		 	Min=0; 	  
	 	}
		  Data_Buffer[0]=Min/10;
		  Data_Buffer[1]=Min%10;		
		  Data_Buffer[2]=Sec/10;		
		  Data_Buffer[3]=Sec%10;
		 }
		if(KEY_DOWN == 0){
		 if(Sec != 0){
			Sec--;
		  }
		  else if(Sec == 0)
		  {
		  if(Min != 0){
		   Min--;
		   Sec = 59;
		   }
		  }
		  Data_Buffer[0]=Min/10;
		  Data_Buffer[1]=Min%10;		
		  Data_Buffer[2]=Sec/10;		
		  Data_Buffer[3]=Sec%10;
		 }
	
        ks_thread_delay(10);
    	}    
}

 void timer1_init(void) reentrant
{ 
	TMOD = 0x11;
    TH1 = 0x0EC;
    TL1 = 0x78;
    ET1 = 1;
    TR1 = 1;
}
 void timer1() interrupt 3 //��ʱ��1�ж���������2��ʱ�䶨ʱ
{
    ks_int_enter();		              
	TH1 = 0x0EC;
    TL1 = 0x78;
    clock1();
   	ks_int_exit();	
}

void main(void)
{	
	ks_system_initialize();
	InitHardware();	
	timer1_init();	
	ks_thread_create(Task,NULL,(void*)0,&TaskStartStk[0],0);
	ks_thread_create(Task1,NULL,(void*)0,&TaskStartStk1[0],1);
	ks_start();
}
