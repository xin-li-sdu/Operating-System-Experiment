#include "ucos_ii.h" 
#include "stm32f10x.h" 
#include <stdio.h>
#include <stdlib.h>


#define ITM_PORT         (*(volatile unsigned char *)(0xe0000000))
#define DEMCR_               (*(volatile unsigned long *)(0xE000EDFC))
#define TRCENA_              0X01000000

#define BufferLen 100

uint8_t fputcp(char ch)
{
	if(DEMCR_ & TRCENA_)
	{
		while(ITM_PORT == 0);  
		ITM_PORT = ch;
	}
	else
	{
		return 0;
	}
  return 1;
}


uint8_t print_str(char *str)
{
	uint32_t i;
	i = 0;
	while(1)
	{
		if (*(str+i) == '\0')
		{
			return 1;
		}
		if(fputcp(*(str+i))==0)
		{
			return 0;
		}
		i++;
	}
}


static void systick_init(void) 
{ 
	RCC_ClocksTypeDef rcc_clocks; 
    RCC_GetClocksFreq(&rcc_clocks); 
    SysTick_Config(rcc_clocks.HCLK_Frequency / OS_TICKS_PER_SEC); 
} 

void ASM_Switch_To_Unprivileged(void);

#define FIRST_TASK 1
//#define SECOND_TASK 1

#ifdef FIRST_TASK
int main(void) 
{
	print_str("Hello world!\n");
	//ASM_Switch_To_Unprivileged();
	systick_init(); 
	print_str("Hello world!\n");
	while(1){}
} 
#endif

#ifdef SECOND_TASK
static OS_STK task1_stk[TASK1_STK_SIZE]; 
static OS_STK task2_stk[TASK1_STK_SIZE]; 
void task1(void *p_arg);
void task2(void *p_arg);
int main(void) 
{ 
	//ASM_Switch_To_Unprivileged();
	print_str("Hello world!\n");
	OSInit(); 
	systick_init(); 
	OSTaskCreate(task1, (void *)0, 
                   &task1_stk[TASK1_STK_SIZE-1], TASK1_PRIO);        
				OSTaskCreate(task2, (void *)0, 
                   &task2_stk[TASK2_STK_SIZE-1], TASK2_PRIO); 
	OSStart(); 
	return 0; 
} 
static void task1(void *p_arg) 
{ 
        for (;;) 
        { 
                OSTimeDly(100); 
                print_str("Hello from Task 1!\n");
                OSTimeDly(100);   
        } 
} 

static void task2(void *p_arg) 
{ 
        for (;;) 
        { 
                OSTimeDly(100); 
                print_str("Hello from Task 2!\n");
                OSTimeDly(100); 
        } 
} 
#endif 


/*END OF THE FILE*/
