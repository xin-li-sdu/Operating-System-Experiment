#include "ucos_ii.h" 
#include "stm32f10x.h" 
#include <stdio.h>
#include <stdlib.h>
#include <os_cpu.h>
#include <time.h>


static OS_STK producer_stk[TASK_STK_SIZE]; 
static OS_STK consumer_stk[TASK_STK_SIZE]; 
void producer(void *p_arg);
void consumer(void *p_arg);
void ASM_Switch_To_Unprivileged(void);
void syscall_print_str(char *);
void syscall_systick_init(void);
void syscall_OSTimeDly(uint32_t);
INT8U syscall_OSTaskCreate(void (*task)(void *p_arg), void *p_arg, OS_STK *ptos, INT8U prio);
void systick_init(void);
void print_str(char *);

#define N 2
OS_EVENT *full;
OS_EVENT *empty;
OS_EVENT *mutex; 
static INT32U food = 0;

int main(void) 
{ 
	int a = rand();
	int b = rand();
	int c = rand();
	int d = rand();
	OSInit();   
	systick_init(); 

	OSTaskCreate(producer, (void *)1, 
                   &producer_stk[TASK_STK_SIZE-1], TASK1_PRIO);    
	OSTaskCreate(consumer, (void *)0, 
                   &consumer_stk[TASK_STK_SIZE-1], TASK2_PRIO); 
	
	
	//ASM_Switch_To_Unprivileged();
	
	
	full = OSSemCreate (0);
	empty = OSSemCreate (N);
	mutex = OSSemCreate (1); 
	OSStart(); 
	return 0; 
} 
static void producer(void *p_arg) 
{
	int i;
	INT8U err;
	int x_temp = (int) p_arg; 
	for (;;)
	{
		print_str("Producer!\n");
		// your code
		OSSemPend(empty, 0, &err);
		OSSemPend(mutex, 0, &err);
		food++;
		//printf("生产者: 食物数量[%03d]\n", food);
		print_str("food++,生产者生成\n");
		OSSemPost(mutex);
		OSSemPost(full);
		OSTimeDly (100);
		for(i=0;i<=10000;i++);
	} 
} 

static void consumer(void *p_arg) 
{
	int i;
	for (;;)
	{
		print_str("Consumer!\n");
		// your code
		INT8U err;
		OSSemPend(full, 0, &err);
		OSSemPend(mutex, 0, &err);
		//********
		food--;
    
		print_str("food--，消费者消费\n");
		//***********
		OSSemPost(mutex);
		OSSemPost(empty);

	  OSTimeDly (100);
		for(i=0;i<=10000;i++);
	} 
} 



/*END OF THE FILE*/
