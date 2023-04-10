#ifndef APP_CFG_H
#define APP_CFG_H

/* task priority */ 
#define STARTUP_TASK_PRIO                          2u 
/* task stack size */ 
#define STARTUP_TASK_STK_SIZE                256u

/*
*********************************************************************************************************
*                                            TASK PRIORITIES
*********************************************************************************************************
*/

// #define  APP_CFG_TASK_START_PRIO                          2u

#define  TASK1_PRIO                            3u
#define  TASK2_PRIO                            4u
#define  TASK3_PRIO                            5u


/*
*********************************************************************************************************
*                                            TASK STACK SIZES
*********************************************************************************************************
*/

// #define  APP_CFG_TASK_START_STK_SIZE                    256u

#define  TASK1_STK_SIZE                    128u
#define  TASK2_STK_SIZE                    128u
#define  TASK3_STK_SIZE                    128u


#endif

