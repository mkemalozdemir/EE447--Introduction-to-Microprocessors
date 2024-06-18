;LABEL      DIRECTIVE   VALUE       COMMENT
PB_INP 		EQU 		0x4000503C
PB_OUT 		EQU 		0x400053C0
;LABEL      DIRECTIVE   VALUE       COMMENT
            AREA        main,CODE,READONLY
            THUMB
            EXTERN      DELAY100
			EXTERN		PortB_Init
			EXTERN		DELAY4
				
            EXPORT      __main      ; Make available

__main		
			BL			PortB_Init
start		LDR			R1,=PB_OUT
			LDR			R0,[R1]
			ORR			R0,R0,#0xF0		;initialize the output pins as logic 1
			STR			R0,[R1]
			LDR			R1,=PB_INP
			
loop		LDR			R0,[R1]
			EOR			R2,R0,#0x0F		; take in put and toggle it to find if any button is pressed
			CMP			R2,#0
			BEQ			loop
			
			MOV			R3,#0x04
			MOV			R2,#0x01
shift		LSRS		R0,R0,#1		;iteratively shifting to find which button is pressed
			ADC			R2,R2,#0
			SUB			R3,#1
			BCS			shift
			
			CMP			R2,#1
			BEQ			display1
			CMP			R2,#2
			BEQ			display2		;according to the pressed button go to display process
			CMP			R2,#3
			BEQ			display3
			CMP			R2,#4
			BEQ			display4
			
display1	LDR			R1,=PB_OUT
			MOV			R3,#0xE0		;Arrange the proper led and add delay
			STR			R3,[R1]
			BL			DELAY4
			B			start

display2	LDR			R1,=PB_OUT
			MOV			R3,#0xD0			;Arrange the proper led and add delay
			STR			R3,[R1]
			BL			DELAY4
			B			start

display3	LDR			R1,=PB_OUT
			MOV			R3,#0xB0		;Arrange the proper led and add delay
			STR			R3,[R1]
			BL			DELAY4
			B			start

display4	LDR			R1,=PB_OUT
			MOV			R3,#0x70		;Arrange the proper led and add delay
			STR			R3,[R1]
			BL			DELAY4			
			B			start
			ALIGN
			END
			

			

