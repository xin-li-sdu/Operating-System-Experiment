#include "ucos_ii.h" 
#include "stm32f10x.h" 
#include <stdio.h>
#include <stdlib.h>
#include <os_cpu.h>
#include <time.h>


/*static OS_STK producer_stk[TASK_STK_SIZE]; 
static OS_STK consumer_stk[TASK_STK_SIZE]; 
void producer(void *p_arg);
void consumer(void *p_arg);*/
void ASM_Switch_To_Unprivileged(void);
void syscall_print_str(char *);
void syscall_systick_init(void);
void syscall_OSTimeDly(uint32_t);
INT8U syscall_OSTaskCreate(void (*task)(void *p_arg), void *p_arg, OS_STK *ptos, INT8U prio);
void systick_init(void);
void print_str(char *);

/*#define N 2
OS_EVENT *full;
OS_EVENT *empty;
OS_EVENT *mutex; 
static INT32U food = 0;*/
#define PHI_REPAST_STK_SIZE  64
#define START_STK_SIZE 64
#define PHI_REPAST_TASK_Prio  3
#define START_TASK_Prio 3
#define     N_TASKS          5
OS_STK   TASK_PHI_REPAST[N_TASKS][PHI_REPAST_STK_SIZE];
OS_STK   TASK_START_STK[N_TASKS][PHI_REPAST_STK_SIZE];
void TaskPhiRepast( void* pdata);
OS_EVENT*    ChopStick[N_TASKS];
INT8U        TaskData[N_TASKS];

void TaskPhiRepast( void* pdata)
{
    INT8U err;
    INT8U i,j;
     /* 第i个哲学家需要第i和j把筷子 */
    i = *( int*)pdata;
    j = (i+ 1) % N_TASKS;
    
     while( 1)
    {
        printf( "Philosopher %d is hungry... \r\n",i+ 1);
			  print_str("what");
        OSTimeDly( 500);
        
        OSSemPend(ChopStick[i], 0,&err);
        OSSemPend(ChopStick[j], 0,&err);
        
        printf( "Philosopher %d is eating... \r\n",i+ 1);
        OSTimeDly( 500);
        
        printf( "Philosopher %d is thinking... \r\n",i+ 1);
        OSSemPost(ChopStick[i]);
        OSSemPost(ChopStick[j]);
    }
}

static void TaskStart(void *p_arg) 
{
	
	/* 实验三  信号量：哲学家就餐问题的实现 */
	int i;
	INT8U err;
  for(i =  0; i < N_TASKS; i++)
  {
     err = OSTaskCreate(TaskPhiRepast, ( void *)&TaskData[i], (OS_STK *)&TASK_PHI_REPAST[i][PHI_REPAST_STK_SIZE -  1], PHI_REPAST_TASK_Prio + i);
     if(err == OS_NO_ERR)
    {
        printf( "Philosophor %d is created no error \r\n", i);
    }
}
}


int main( void)
 {
    INT8U i;     
    OSInit();
	 	systick_init(); 
     for(i =  0;i < N_TASKS;i++)
    {
        TaskData[i] = i;
        ChopStick[i] = OSSemCreate( 1);
         if(ChopStick[i] != (OS_EVENT*) 0)
            printf( "ChopStick %d is created no error \r\n",i);
    }
    OSTaskCreate( TaskStart,
                    ( void *) 0,   //parameter
                    (OS_STK *)&TASK_START_STK[START_STK_SIZE- 1],
                    START_TASK_Prio );
    OSStart();     
     return  0;    
 }
/*int main(void) 
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
		********
		food--;
    
		print_str("food--，消费者消费\n");
		***********
		OSSemPost(mutex);
		OSSemPost(empty);

	  OSTimeDly (100);
		for(i=0;i<=10000;i++);
	} 
} 

*/

/*END OF THE FILE*/
