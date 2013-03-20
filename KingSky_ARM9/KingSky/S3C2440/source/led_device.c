#include "config.h"

KS_DEVICE_HANDLE led_handle;
static file_operations led_fs;

static void led_open(void)
{
	ks_log("led open successful\n");
}
static void led_close(void)
{
	ks_log("led close successful\n");
}
#define LED_OFF   0
#define LED_ON    1
static void led_ioctl(uint32 cmd,unsigned long arg)
{
switch (cmd)
{
 case LED_ON:
 	 rGPBDAT &= ~(1<<(arg+4));
	 rGPBDAT |= (0<<(arg+4)); //led on
 	break;
 case LED_OFF:
 	rGPBDAT &= ~(1<<(arg+4));
	rGPBDAT |= (1<<(arg+4)); //led off
 	break;
 default:
 	break;
}
}

void led_device_init(void)
{
     rGPBCON &= ~(3<<10);			//set GPB5 (led1)as  output
	 rGPBCON |= 1<<10;

	 rGPBCON &= ~(3<<12);			//set GPB6(led2) as  output
	 rGPBCON |= 1<<12;

	 rGPBCON &= ~(3<<14);			//set GPB7(led3) as  output
	 rGPBCON |= 1<<14;

	 rGPBCON &= ~(3<<16);			//set GPB8(led4) as  output
	 rGPBCON |= 1<<16;

	 rGPBDAT &= ~((1<<5)|(1<<6)|(1<<7)|(1<<8));
	 rGPBDAT |= ((1<<5)|(1<<6)|(1<<7)|(1<<8)); //all led off 
    
    led_fs.device_open  = led_open;
	led_fs.device_ioctl = led_ioctl;
	led_fs.device_close = led_close;
	led_handle = ks_device_create("led",CHAR_DEVICE,CAN_SHARED,led_fs);
	if(led_handle == NULL){
		ks_err("##led device create err.\n");
	}
}


int  do_led_ctl(struct cmd_tbl_s *cmdtp, int point_flag, int argc , char * const argv[])
{
    int i = 2;
    if(point_flag == 0)
    {
    ks_err("##error command %s,try 'help %s'\n",cmdtp->name,cmdtp->name);
    return -1;
    }
	if(strcmp(argv[1],"on") == 0){
		while(argv[i] != NULL){
			if((*argv[i] - '0')> 4){
				ks_log("## argv err \n");
			}
		ks_device_ioctl(led_handle,LED_ON,(*argv[i] - '0'));
		i++;
		}
		return 0;
	}
	if(strcmp(argv[1],"off") == 0){
		while(argv[i] != NULL){
			if((*argv[i] - '0')> 4){
				ks_log("## argv err \n");
			}
		ks_device_ioctl(led_handle,LED_OFF,(*argv[i] - '0'));
		i++;
		}
		return 0;
	}
	ks_err("##error use command %s,try 'help %s'\n",cmdtp->name,cmdtp->name);
	return 0;
}




