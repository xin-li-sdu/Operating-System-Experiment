#include <stdlib.h>
#include <stdio.h>
#include "stm32f10x.h" 
#include "ucos_ii.h" 
#define ITM_PORT         (*(volatile unsigned char *)(0xe0000000))
#define DEMCR_               (*(volatile unsigned long *)(0xE000EDFC))
#define TRCENA_              0X01000000
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

void systick_init(void) 
{ 
	RCC_ClocksTypeDef rcc_clocks; 
	RCC_GetClocksFreq(&rcc_clocks); 
	SysTick_Config(rcc_clocks.HCLK_Frequency / OS_TICKS_PER_SEC); 
} 
