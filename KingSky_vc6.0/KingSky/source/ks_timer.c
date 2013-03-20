#include "..\..\KingSky\includes\KingSky.h"

TIMER *Freelist;
TIMER *Userlist;

void ks_timer_init(void)
{
	uint8 i;
	Userlist       = NULL;
	Freelist       = NULL;
	for(i=0; i<MAX_TIMER; i++)
	{
		//Timer_Tcb[i].id       = 0;
		Timer_Tcb[i].on_off   = OFF;
		Timer_Tcb[i].ldtime   = 0;
		Timer_Tcb[i].lefttime = 0;
		Timer_Tcb[i].proc     = NULL;
		Timer_Tcb[i].next     = Freelist;
		Freelist              = &Timer_Tcb[i];
	}
}

TIMER * ks_timer_create(void (*p)())
{
#if KS_CRITICAL_METHOD == 3                      /* Allocate storage for CPU status register           */
    KS_CPU_SR  cpu_sr = 0;
#endif	  
	TIMER *ptr;
	
	if(Freelist==NULL) return NULL;
	KS_ENTER_CRITICAL();
	ptr = Freelist; 
	Freelist = Freelist->next;
	
	//ptr->id       = 0;
	ptr->on_off   = OFF;
	ptr->proc     = p;
	ptr->ldtime   = 0;
	ptr->lefttime = 0;   
	ptr->next     = Userlist;
	Userlist      = ptr;
	KS_EXIT_CRITICAL();
	return Userlist;
}

void ks_timer_remove(TIMER *ptr)
{
#if KS_CRITICAL_METHOD == 3                      /* Allocate storage for CPU status register           */
    KS_CPU_SR  cpu_sr = 0;
#endif
	TIMER *tptr;
	if(ptr==NULL)return;
	KS_ENTER_CRITICAL();	
	//ptr->id       = 0;
	ptr->on_off   = OFF;
	ptr->proc     = NULL;
	ptr->ldtime   = 0;
	ptr->lefttime = 0; 
	
	tptr = Userlist;
	if(ptr==tptr)
	{
		Userlist   = Userlist->next;
		tptr->next = Freelist;
		Freelist   = tptr;
		return; 
	}
	else
	{
		while((tptr->next)!=ptr&&(tptr->next)!=NULL)
		{
			tptr = tptr->next;
		}
		if(tptr->next==NULL) return;
		tptr->next = ptr->next;
		ptr->next  = Freelist;
		Freelist   = ptr;
	}
	KS_EXIT_CRITICAL();
}

void ks_timer_start(TIMER *ptr,uint32 time)
{
	ptr->lefttime = (uint32)time;
	ptr->ldtime   = (uint32)time;
	ptr->on_off   = ON;
}

void ks_timer_stop(TIMER *ptr)
{
	ptr->on_off  = OFF;
}

uint32 ks_timer_lef_get(TIMER *ptr)
{
	return ptr->lefttime;
}

void ks_timer_restart(TIMER *ptr,uint32 time)
{
	uint32 temp;
	uint32 temp1;
	
	temp  = ks_timer_lef_get(ptr);
	temp1 = (uint32)(((uint32)time));
	if(temp>=temp1)
	{
		ptr->lefttime = (uint32)(((uint32)time));
		ptr->ldtime   = (uint32)(((uint32)time));
	}
}

void ks_timer_tick(void)
{
#if KS_CRITICAL_METHOD == 3                      /* Allocate storage for CPU status register           */
    KS_CPU_SR  cpu_sr = 0;
#endif
	TIMER *tptr;
	if(Userlist==NULL)return;
	KS_ENTER_CRITICAL();
	tptr = Userlist; 
	while(tptr!=NULL)
	{
		if(tptr->on_off==ON)
		{
			if(tptr->lefttime!=0)tptr->lefttime--;
			if(tptr->lefttime==0){
			tptr->on_off = OFF;
			if(tptr->proc!=NULL)
			{
				tptr->proc();
			}
			}
		}
		tptr = tptr->next;
	}
	KS_EXIT_CRITICAL();
}
