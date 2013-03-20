#include "config.h"
/*****************************¡¾·äÃùÆ÷¡¿************************************/
KS_DEVICE_HANDLE beep_handle;
static file_operations beep_fs;

void Buzzer_Freq_Set( U32 freq ) //1000
{
	rGPBCON &= ~3;			//set GPB0 as tout0, pwm output
	rGPBCON |= 2;

	rTCFG0 &= ~0xff;
	rTCFG0 |= 15;			//prescaler = 15+1
	rTCFG1 &= ~0xf;
	rTCFG1 |= 2;			//mux = 1/8
	rTCNTB0 = (PCLK>>7)/freq;
	rTCMPB0 = rTCNTB0>>1;		// 50%
	rTCON &= ~0x1f;
	rTCON |= 0xb;			//disable deadzone, auto-reload, inv-off, update TCNTB0&TCMPB0, start timer 0
	rTCON &= ~2;			//clear manual update bit
}

static void beep_stop( void )
{
	rGPBCON &= ~3;			//set GPB0 as output
	rGPBCON |= 1;
	rGPBDAT &= ~1;
}

static void beep_start( void )
{
	rGPBCON &= ~3;			//set GPB0 as output
	rGPBCON |= 1;
	rGPBDAT &= ~1;
	rGPBDAT |= 1;
}

 void beep(U32 freq, U32 ms)
{
	Buzzer_Freq_Set( freq ) ;
	ks_thread_delay(ms);
	beep_stop() ;
}
 
#define BEEP_START    1
#define BEEP_STOP     2
#define BEEP_FREQ_SET 3
static void beep_ioctl(uint32 cmd,unsigned long arg)
{
switch (cmd)
{
case BEEP_START:
	beep_start();
	break;
case BEEP_STOP:
	beep_stop();
	break;
default:
	break;
}
}
static void beep_open(void)
{
	ks_log("beep open successful\n");
}
static void beep_close(void)
{
	ks_log("beep close successful\n");
}

void beep_device_init(void)
{
    beep_fs.device_open  = beep_open;
	beep_fs.device_ioctl = beep_ioctl;
	beep_fs.device_close = beep_close;
	beep_handle = ks_device_create("beep",CHAR_DEVICE,CAN_SHARED,beep_fs);
	if(beep_handle == NULL){
		ks_err("##beep device create err.\n");
	}
}

int  do_beep_ctl(struct cmd_tbl_s *cmdtp, int point_flag, int argc , char * const argv[])
{
    if(point_flag == 0)
    {
    ks_err("##error command %s,try 'help %s'\n",cmdtp->name,cmdtp->name);
    return -1;
    }
	if(strcmp(argv[1],"start") == 0){
		ks_device_ioctl(beep_handle,BEEP_START,0);
		return 0;
	}
	if(strcmp(argv[1],"stop") == 0){
		ks_device_ioctl(beep_handle,BEEP_STOP,0);
		return 0;
	}
	ks_err("##error use command %s,try 'help %s'\n",cmdtp->name,cmdtp->name);
	return 0;
}




