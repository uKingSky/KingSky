#include "KingSky.h"

#if KS_DEVICE_EN >0

KS_DEVICE_HANDLE ks_device_create(uint8 *device_name,
		                                uint8  device_type,
		                                uint8  can_shared,
		                                file_operations deivce_op)
{
 	device_node *device_ptr;
 	device_ptr = (device_node *)ks_malloc(sizeof(device_node));
 	device_ptr->device_name = device_name;
 	device_ptr->device_type = device_type;
 	device_ptr->can_shared  = can_shared;
 	device_ptr->deivce_op   = deivce_op;
	device_ptr->open_count  = 0;
    if(device_ptr->can_shared == NOT_CAN_SHARED){
	   device_ptr->device_sem = ks_sem_create(1);
	}
	device_ptr->state = "create";
 	return device_ptr;
}
 uint8 ks_device_open(KS_DEVICE_HANDLE device_handle)
 {

  if(device_handle->can_shared == NOT_CAN_SHARED){
  	if(device_handle->open_count != 0)
		return KS_ERR_DEVICE_NOT_SHARED;
  	}	
 	 device_handle->open_count++;
	 device_handle->state = "open";
	 device_handle->deivce_op.device_open();
	 return KS_DEVICE_OPEN_NO_ERR;
 }
 uint8 ks_device_write(KS_DEVICE_HANDLE device_handle, uint8 *buffer)
 {
   uint8 err;
     if(device_handle == (KS_DEVICE_HANDLE)NULL){
		return KS_DEVICE_WRITE_ERR;
    }
    if(device_handle->can_shared == NOT_CAN_SHARED){
		ks_sem_waitfor(device_handle->device_sem,0,&err);
 	}
 	device_handle->deivce_op.device_write(buffer);
	if(device_handle->can_shared == NOT_CAN_SHARED){
		ks_sem_sendmsg(device_handle->device_sem);
 	}
	return KS_DEVICE_WRITE_NO_ERR;
 }
 uint8 ks_device_read(KS_DEVICE_HANDLE device_handle,uint8 *buffer,uint32 count)
 {
 uint8 err;
 	if(device_handle == (KS_DEVICE_HANDLE)NULL){
		return KS_DEVICE_READ_ERR;
    }
 	if(device_handle->can_shared == NOT_CAN_SHARED){
		ks_sem_waitfor(device_handle->device_sem,0,&err);
 	}
   device_handle->deivce_op.device_read(buffer,count);
   if(device_handle->can_shared == NOT_CAN_SHARED){
		ks_sem_sendmsg(device_handle->device_sem);
 	}
 return KS_DEVICE_READ_NO_ERR;
 }
 uint8 ks_device_ioctl(KS_DEVICE_HANDLE device_handle,uint32 cmd,unsigned long arg)
{
  uint8 err;
 	if(device_handle == (KS_DEVICE_HANDLE)NULL){
		return KS_DEVICE_IOCTL_ERR;
    }
	if(device_handle->can_shared == NOT_CAN_SHARED){
		ks_sem_waitfor(device_handle->device_sem,0,&err);
 	}
   device_handle->deivce_op.device_ioctl(cmd,arg);
   if(device_handle->can_shared == NOT_CAN_SHARED){
		ks_sem_sendmsg(device_handle->device_sem);
 	}
 return KS_DEVICE_IOCTL_NO_ERR;
}
  uint8 ks_device_close(KS_DEVICE_HANDLE device_handle)
{
   uint8 err;
 	 if(device_handle == (KS_DEVICE_HANDLE)NULL){
		return KS_DEVICE_CLOSE_ERR;
    }
	 if(device_handle->can_shared == NOT_CAN_SHARED){
		ks_sem_waitfor(device_handle->device_sem,0,&err);
 	}
	 if(device_handle->open_count>0){
	 	device_handle->open_count --;
	 }	
	 device_handle->state = "close";
   device_handle->deivce_op.device_close();
   ks_free(device_handle);
  if(device_handle->can_shared == NOT_CAN_SHARED){
		ks_sem_sendmsg(device_handle->device_sem);
		ks_sem_delete(device_handle->device_sem,KS_DEL_ALWAYS,&err);
 	}
 return KS_DEVICE_CLOSE_NO_ERR;
}

 uint8 * ks_device_state(KS_DEVICE_HANDLE device_handle)
 {
    if(device_handle == (KS_DEVICE_HANDLE)NULL){
		return NULL;
    }
 	return device_handle->state;
 }

extern  void beep_device_init(void);
extern void  led_device_init(void);
 void ks_all_device_init(void)
 {
    beep_device_init();
	led_device_init();
 }
 
#endif
		                          
		                              
		                              



