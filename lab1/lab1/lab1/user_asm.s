
	IMPORT SVC_Handler_Main
	EXPORT SVC_Handler
	EXPORT ASM_Switch_To_Unprivileged	
	
	AREA |.text|, CODE, READONLY, ALIGN=2
    THUMB
	REQUIRE8
    PRESERVE8

; @brief  This function handles SVCall exception.
; @param  None
; @retval None
SVC_Handler
	B  SVC_Handler_Main	
	
ASM_Switch_To_Unprivileged	
	MRS     R0, control
	ORR     R0, #1 
	MSR     control, R0
	BX      LR



	END