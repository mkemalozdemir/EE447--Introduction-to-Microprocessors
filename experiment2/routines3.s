;LABEL      			DIRECTIVE   VALUE       COMMENT
FIRST					EQU			0x20000468
COUNTER100     			EQU 		0x61A80
COUNTER20    			EQU 		0x13880
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
			EXTERN		OutChar
			
			EXPORT		DELAY100
			EXPORT		DELAY20
			EXPORT		PortB_Init
			EXPORT		DELAY4
			EXPORT		CHECK1
			EXPORT		CHECK2
			EXPORT		CHECK3
			EXPORT		CHECK4
			
DELAY100	PROC
			
			PUSH		{R1}
			LDR			R1,=COUNTER100
			
loop		SUBS		R1,#1			;100ms delay function
			BEQ			end_p
			B			loop

end_p		POP			{R1}
			BX			LR
			ALIGN
			ENDP

DELAY20		PROC
			
			PUSH		{R1}
			LDR			R1,=COUNTER20	;20ms delay function
			
loop20		SUBS		R1,#1
			BEQ			end_p20
			B			loop20

end_p20		POP			{R1}
			BX			LR
			ALIGN
			ENDP
				
PortB_Init	PROC
			LDR			R1,=SYSCTL_RCGCGPIO_R
			LDR			R0,[R1]					;clock initializer
			ORR			R0,R0,#0x2F
			STR			R0,[R1]
			NOP
			NOP
			
			LDR			R1,=GPIO_PORTB_DIR_R
			LDR			R0,[R1]
			ORR			R0,R0,#0xF0				;input output pin initializer
			STR			R0,[R1]
			
			LDR			R1,=GPIO_PORTB_AFSEL_R
			LDR			R0,[R1]					;alternative function initializer
			BIC			R0,R0,#0xFF
			STR			R0,[R1]
			
			LDR			R1,=GPIO_PORTB_PUR
			LDR			R0,[R1]					;pull up resistor initializer
			ORR			R0,R0,#0x0F				;Input pins are logic 1 if the buttons are not pressed
			STR			R0,[R1]
			
			LDR			R1,=GPIO_PORTB_PDR
			LDR			R0,[R1]					;pull down resistor initializer
			BIC			R0,R0,#0xFF
			STR			R0,[R1]
			
			LDR			R1,=GPIO_PORTB_DEN_R
			LDR			R0,[R1]					;digital data accessibility 
			ORR			R0,R0,#0xFF
			STR			R0,[R1]
			BX			LR
			ALIGN
			ENDP
				
DELAY4		PROC
			
			PUSH		{R0,LR}
			MOV			R0,#40
			
loop2		BL			DELAY100			;4 seconds delay
			SUBS		R0,#1
			BEQ			end_d
			B			loop2
			
end_d		POP			{R0,LR}
			BX			LR
			ALIGN
			ENDP

CHECK1		PROC						;Checks rows by iteration when input is 1110
			PUSH		{LR}
			LDR			R2,=PB_INP
			LDR			R1,=PB_OUT		
			
			LDR			R0,[R1]
			ORR			R0,R0,#0xE0		;For rows 1110
			STR			R0,[R1]
			BL			DELAY20
			LDR			R3,[R2]
			CMP			R3,#0x0E
			BEQ			nC		
			BIC			R0,#0xFF	
			STR			R0,[R1]
			BL			DELAY20
			LDR			R0,[R1]
			ORR			R0,R0,#0xD0		;For rows 1101
			STR			R0,[R1]
			BL			DELAY20
			LDR			R3,[R2]
			CMP			R3,#0x0E
			BEQ			n8
			BIC			R0,#0xFF
			STR			R0,[R1]
			BL			DELAY20
			LDR			R0,[R1]
			ORR			R0,R0,#0xB0		;For rows 1011
			STR			R0,[R1]
			BL			DELAY20
			LDR			R3,[R2]
			CMP			R3,#0x0E
			BEQ			n4
			BIC			R0,#0xFF
			STR			R0,[R1]			
			BL			DELAY20
			LDR			R0,[R1]
			ORR			R0,R0,#0x70		;For rows 0111
			STR			R0,[R1]
			BL			DELAY20
			LDR			R3,[R2]
			CMP			R3,#0x0E
			BEQ			n0
			BIC			R0,#0xFF
			STR			R0,[R1]
			BL			DELAY20
nC			LDR			R3,[R2]
			CMP			R3,#0x0F		;detect release
			BNE			nC
			MOV			R0,#0x43
			BL			OutChar
			B			end_p1
n8			LDR			R3,[R2]
			CMP			R3,#0x0F		;detect release
			BNE			n8
			MOV			R0,#0x38
			BL			OutChar
			B			end_p1
n4			LDR			R3,[R2]
			CMP			R3,#0x0F		;detect release
			BNE			n4
			MOV			R0,#0x34
			BL			OutChar
			B			end_p1
n0			LDR			R3,[R2]
			CMP			R3,#0x0F		;detect release
			BNE			n0
			MOV			R0,#0x30
			BL			OutChar
			B			end_p1
end_p1		POP			{LR}
			BX			LR
			ALIGN
			ENDP

CHECK2		PROC						;same logic with CHECK1 but, input is 1101
			PUSH		{LR}
			LDR			R2,=PB_INP
			LDR			R1,=PB_OUT		
			
			LDR			R0,[R1]
			ORR			R0,R0,#0xE0
			STR			R0,[R1]
			BL			DELAY20
			LDR			R3,[R2]
			CMP			R3,#0x0D
			BEQ			nD
			BIC			R0,#0xFF	
			STR			R0,[R1]
			BL			DELAY20
			LDR			R0,[R1]
			ORR			R0,R0,#0xD0
			STR			R0,[R1]
			BL			DELAY20
			LDR			R3,[R2]
			CMP			R3,#0x0D
			BEQ			n9
			BIC			R0,#0xFF
			STR			R0,[R1]
			BL			DELAY20
			LDR			R0,[R1]
			ORR			R0,R0,#0xB0
			STR			R0,[R1]
			BL			DELAY20
			LDR			R3,[R2]
			CMP			R3,#0x0D
			BEQ			n5
			BIC			R0,#0xFF
			STR			R0,[R1]			
			BL			DELAY20
			LDR			R0,[R1]
			ORR			R0,R0,#0x70
			STR			R0,[R1]
			BL			DELAY20
			LDR			R3,[R2]
			CMP			R3,#0x0D
			BEQ			n1
			BIC			R0,#0xFF
			STR			R0,[R1]
			BL			DELAY20
nD			LDR			R3,[R2]
			CMP			R3,#0x0F
			BNE			nD
			MOV			R0,#0x44
			BL			OutChar
			B			end_p2
n9			LDR			R3,[R2]
			CMP			R3,#0x0F
			BNE			n9
			MOV			R0,#0x39
			BL			OutChar
			B			end_p2
n5			LDR			R3,[R2]
			CMP			R3,#0x0F
			BNE			n5
			MOV			R0,#0x35
			BL			OutChar
			B			end_p2
n1			LDR			R3,[R2]
			CMP			R3,#0x0F
			BNE			n1
			MOV			R0,#0x31
			BL			OutChar
			B			end_p2
end_p2		POP			{LR}
			BX			LR
			ALIGN
			ENDP

CHECK3		PROC						;same logic with CHECK1 but, input is 1011
			PUSH		{LR}
			LDR			R2,=PB_INP
			LDR			R1,=PB_OUT		
			
			LDR			R0,[R1]
			ORR			R0,R0,#0xE0
			STR			R0,[R1]
			BL			DELAY20
			LDR			R3,[R2]
			CMP			R3,#0x0B
			BEQ			nE
			BIC			R0,#0xFF	
			STR			R0,[R1]
			BL			DELAY20
			LDR			R0,[R1]
			ORR			R0,R0,#0xD0
			STR			R0,[R1]
			BL			DELAY20
			LDR			R3,[R2]
			CMP			R3,#0x0B
			BEQ			nA
			BIC			R0,#0xFF
			STR			R0,[R1]
			BL			DELAY20
			LDR			R0,[R1]
			ORR			R0,R0,#0xB0
			STR			R0,[R1]
			BL			DELAY20
			LDR			R3,[R2]
			CMP			R3,#0x0B
			BEQ			n6
			BIC			R0,#0xFF
			STR			R0,[R1]			
			BL			DELAY20
			LDR			R0,[R1]
			ORR			R0,R0,#0x70
			STR			R0,[R1]
			BL			DELAY20
			LDR			R3,[R2]
			CMP			R3,#0x0B
			BEQ			n2
			BIC			R0,#0xFF
			STR			R0,[R1]
			BL			DELAY20
nE			LDR			R3,[R2]
			CMP			R3,#0x0F
			BNE			nE
			MOV			R0,#0x45
			BL			OutChar
			B			end_p3	
nA			LDR			R3,[R2]
			CMP			R3,#0x0F
			BNE			nA
			MOV			R0,#0x41
			BL			OutChar
			B			end_p3	
n6			LDR			R3,[R2]
			CMP			R3,#0x0F
			BNE			n6
			MOV			R0,#0x36
			BL			OutChar
			B			end_p3	
n2			LDR			R3,[R2]
			CMP			R3,#0x0F
			BNE			n2
			MOV			R0,#0x32
			BL			OutChar
			B			end_p3
end_p3		POP			{LR}
			BX			LR
			ALIGN
			ENDP

CHECK4		PROC						;same logic with CHECK1 but, input is 0111
			PUSH		{LR}
			LDR			R2,=PB_INP
			LDR			R1,=PB_OUT		
			
			LDR			R0,[R1]
			ORR			R0,R0,#0xE0
			STR			R0,[R1]
			BL			DELAY20
			LDR			R3,[R2]
			CMP			R3,#0x07
			BEQ			nF
			BIC			R0,#0xFF	
			STR			R0,[R1]
			BL			DELAY20
			LDR			R0,[R1]
			ORR			R0,R0,#0xD0
			STR			R0,[R1]
			BL			DELAY20
			LDR			R3,[R2]
			CMP			R3,#0x07
			BEQ			nB
			BIC			R0,#0xFF
			STR			R0,[R1]
			BL			DELAY20
			LDR			R0,[R1]
			ORR			R0,R0,#0xB0
			STR			R0,[R1]
			BL			DELAY20
			LDR			R3,[R2]
			CMP			R3,#0x07
			BEQ			n7
			BIC			R0,#0xFF
			STR			R0,[R1]			
			BL			DELAY20
			LDR			R0,[R1]
			ORR			R0,R0,#0x70
			STR			R0,[R1]
			BL			DELAY20
			LDR			R3,[R2]
			CMP			R3,#0x07
			BEQ			n3
			BIC			R0,#0xFF
			STR			R0,[R1]	
			BL			DELAY20
nF			LDR			R3,[R2]
			CMP			R3,#0x0F
			BNE			nF
			MOV			R0,#0x46
			BL			OutChar
			B			end_p4
nB			LDR			R3,[R2]
			CMP			R3,#0x0F
			BNE			nB
			MOV			R0,#0x42
			BL			OutChar
			B			end_p4
n7			LDR			R3,[R2]
			CMP			R3,#0x0F
			BNE			n7
			MOV			R0,#0x37
			BL			OutChar
			B			end_p4
n3			LDR			R3,[R2]
			CMP			R3,#0x0F
			BNE			n3
			MOV			R0,#0x33
			BL			OutChar	
			B			end_p4
end_p4		POP			{LR}
			BX			LR
			ALIGN
			ENDP
			

			