
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
	TST   LR, #4
	MRSEQ R1, MSP
	MRSNE R1, PSP
	;r1 <- sp
	LDR   R0, [R1,#24]
	;r0 <- pc
	SUB   R0, 2
	;r0 <- instruction pointer
	LDR   R1, [R0]
	; r1 <- instruction
	AND   R0, R1, 0xFF
	SUB   R0, 1
	ADD   R0, R5
	B     SVC_Handler_Main	
	
ASM_Switch_To_Unprivileged	
	MRS     R0, control
	ORR     R0, #1 
	MSR     control, R0
	BX      LR



	END