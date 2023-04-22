
	IMPORT print_str
	IMPORT systick_init
	IMPORT OSStartHighRdy_super
	IMPORT OSTimeDly
	IMPORT OSTaskCreate
		
	EXPORT SVC_Handler
	EXPORT syscall_print_str
	EXPORT syscall_systick_init
	EXPORT syscall_OSTimeDly
	EXPORT syscall_OSTaskCreate
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
	MRSEQ R2, MSP
	MRSNE R2, PSP
	; r1 <- sp
	LDR   R0, [R2,#24]
	;r0 <- instruction pointer
	LDR   R1, [R0,#-2]
	; r1 <- instruction
	AND   R0, R1, #0xFF
	; r0 <- index
	CMP   R0, #0x01
	BEQ   print_str_call
	CMP   R0, #0x02
	BEQ   systick_init_call
	CMP   R0, #0x03
	BEQ   OSStartHighRdy_super_call
	CMP   R0, #0x04
	BEQ   OSTimeDly_call
	CMP   R0, #0x05
	BEQ   OSTaskCreate_call
	BX    LR
print_str_call	
	LDR   R0, [R2]
	PUSH  {LR}
	BL    print_str
	POP   {LR}
	BX    LR
systick_init_call	
	PUSH  {LR}
	BL    systick_init
	BX    LR
OSStartHighRdy_super_call	
	PUSH  {LR}
	BL    OSStartHighRdy_super
	POP   {LR}
	BX    LR
OSTimeDly_call	
	LDR   R0, [R2]
	MOV   R1, #0xFFFF
	AND   R0, R1
	PUSH  {LR}
	BL    OSTimeDly
	POP   {LR}
	BX    LR
OSTaskCreate_call
	MOV   R5, R2
	LDR   R0, [R5]
	LDR   R1, [R5,#4]
	LDR   R2, [R5,#8]
	LDR   R3, [R5,#12]
	PUSH  {LR}
	BL    OSTaskCreate
	POP   {LR}
	BX    LR
	
	
ASM_Switch_To_Unprivileged	
	MRS     R0, control
	ORR     R0, #1 
	MSR     control, R0
	BX      LR

syscall_print_str
	SWI 0x01
	BX      LR

syscall_systick_init
	SWI 0x02
	BX      LR

syscall_OSTimeDly
	SWI 0x04
	BX  LR
	
syscall_OSTaskCreate
	SWI 0x05
	BX  LR
	
	END