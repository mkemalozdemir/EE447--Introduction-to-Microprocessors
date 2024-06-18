;LABEL      			DIRECTIVE   VALUE       COMMENT
COUNTER100     			EQU 		0x61A80
PB_INP 					EQU 		0x4000503C
PB_OUT 					EQU 		0x400053C0
GPIO_PORTB_DIR_R 		EQU 		0x40005400
GPIO_PORTB_AFSEL_R 		EQU 		0x40005420
GPIO_PORTB_DEN_R 		EQU 		0x4000551C
GPIO_PORTB_PUR 			EQU 		0x40005510
GPIO_PORTB_PDR 			EQU 		0x40005514
SYSCTL_RCGCGPIO_R 		EQU 		0x400FE608

;LABEL      DIRECTIVE   VALUE       COMMENT
            AREA        routines, CODE, READONLY
            THUMB
			EXPORT		DELAY100
			EXPORT		PortB_Init
			EXPORT		DELAY4
			
DELAY100	PROC
			
			PUSH		{R1}
			LDR			R1,=COUNTER100	;set counter
			
loop		SUBS		R1,#1			;decrement until it becomes 0
			BEQ			end_p
			B			loop

end_p		POP			{R1}
			BX			LR
			ALIGN
			ENDP
				
PortB_Init	PROC
			LDR			R1,=SYSCTL_RCGCGPIO_R
			LDR			R0,[R1]					;initialize clock
			ORR			R0,R0,#0x2F
			STR			R0,[R1]
			NOP
			NOP
			
			LDR			R1,=GPIO_PORTB_DIR_R
			LDR			R0,[R1]					;initialize the pins as input output
			ORR			R0,R0,#0xF0
			STR			R0,[R1]
			
			LDR			R1,=GPIO_PORTB_AFSEL_R
			LDR			R0,[R1]
			BIC			R0,R0,#0xFF				;initialize the alternative functions
			STR			R0,[R1]
			
			LDR			R1,=GPIO_PORTB_PUR
			LDR			R0,[R1]					;initialize the pull up resistors
			BIC			R0,R0,#0xFF
			STR			R0,[R1]
			
			LDR			R1,=GPIO_PORTB_PDR
			LDR			R0,[R1]					;initialize the pull down resistors
			BIC			R0,R0,#0xFF
			STR			R0,[R1]
			
			LDR			R1,=GPIO_PORTB_DEN_R
			LDR			R0,[R1]					;initialize the digital data accessibility
			ORR			R0,R0,#0xFF
			STR			R0,[R1]
			BX			LR
			ALIGN
			ENDP
				
DELAY4		PROC
			
			PUSH		{R0,LR}
			MOV			R0,#40
			
loop2		BL			DELAY100				;delay for 4 seconds
			SUBS		R0,#1
			BEQ			end_d
			B			loop2
			
end_d		POP			{R0,LR}
			BX			LR
			ALIGN
			ENDP

			