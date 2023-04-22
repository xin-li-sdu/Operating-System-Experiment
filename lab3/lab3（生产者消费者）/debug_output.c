#include <stdio.h>

#define ITM_PORT8(n)         (*(volatile unsigned char *)(0xe0000000 + 4*(n)))
#define ITM_PORT16(n)        (*(volatile unsigned short *)(0xe0000000 + 4*(n)))
#define ITM_PORT32(n)        (*(volatile unsigned long *)(0xe0000000 + 4*(n)))
#define DEMCR                (*(volatile unsigned long *)(0xE000EDFC))
#define TRCENA               0X01000000

int fputc(int ch, FILE *f)
{
    if(DEMCR & TRCENA)
    {
        while(ITM_PORT32(0) == 0);                                                                                                                                                                                                                                                                                      
        ITM_PORT8(0) = ch;
    }
    return ch;
}
