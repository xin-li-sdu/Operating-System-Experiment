#include "ucos_ii.h" 
#include "stm32f10x.h" 
#include <stdio.h>
#include <stdlib.h>
#include <os_cpu.h>
#include <time.h>


static OS_STK reader_stk[2][TASK_STK_SIZE]; 
static OS_STK writer_stk[TASK_STK_SIZE]; 
void reader(void *p_arg);
void writer(void *p_arg);
void ASM_Switch_To_Unprivileged(void);
void syscall_print_str(char *);
void syscall_systick_init(void);
void syscall_OSTimeDly(uint32_t);
INT8U syscall_OSTaskCreate(void (*task)(void *p_arg), void *p_arg, OS_STK *ptos, INT8U prio);
void systick_init(void);
void print_str(char *);

#define N 2
//#define N 8
OS_EVENT *mutex;
OS_EVENT *db;
INT32U RC = 0;

int main(void) 
{ 
	int a = rand();
	int b = rand();
	int c = rand();
	int d = rand();
	OSInit();   
	systick_init(); 

	OSTaskCreate(reader, (void *)1, 
                   &reader_stk[0][TASK_STK_SIZE-1], TASK1_PRIO);    
	OSTaskCreate(writer, (void *)0, 
                   &writer_stk[TASK_STK_SIZE-1], TASK2_PRIO); 
	
	OSTaskCreate(reader, (void *)1, 
                   &reader_stk[1][TASK_STK_SIZE-1], TASK3_PRIO); 
	//ASM_Switch_To_Unprivileged();
	
	
	db=OSSemCreate(1);
	mutex=OSSemCreate(1);
	OSStart(); 
	return 0; 
} 
static void reader(void *p_arg) 
{
	int i;
	INT8U err;
	int x_temp = (int) p_arg; 
	for (;;)
	{
		OSSemPend(mutex, 0, 0);
		RC++;
		if(RC==1)
		{
			OSSemPend(db,0,0);
		}
		OSSemPost(mutex);
		char txt[20]="X is READer!!\n";
		txt[0]=(char)(RC+48);
		print_str(txt);
		OSTimeDly(10);
		OSSemPend(mutex,0,0);
		RC--;
		if(RC==0)
		{
			OSSemPost(db);
		}
		OSSemPost(mutex);
		OSTimeDly (100);
		for(i=0;i<=10000;i++);
	} 
} 

static void writer(void *p_arg) 
{
	int i;
	for (;;)
	{
		OSSemPend(db, 0, 0);
		print_str("WRITER is WORKing!!\n");
		OSSemPost(db);
	  OSTimeDly (100);
		for(i=0;i<=10000;i++);
	} 
} 



/*END OF THE FILE*/
