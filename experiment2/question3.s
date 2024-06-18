;LABEL      DIRECTIVE   VALUE       COMMENT
PB_INP 		EQU 		0x4000503C
PB_OUT 		EQU 		0x400053C0
;LABEL      DIRECTIVE   VALUE       COMMENT
            AREA        main,CODE,READONLY
            THUMB
            EXTERN      DELAY100
			EXTERN      DELAY20
			EXTERN		PortB_Init
			EXTERN 		CHECK1
			EXTERN		CHECK2
			EXTERN		CHECK3
			EXTERN		CHECK4
				
            EXPORT      __main      ; Make available

__main		
			BL			PortB_Init
start		LDR			R1,=PB_OUT
			LDR			R0,[R1]
			AND			R0,R0,#0x00
			STR			R0,[R1]
			LDR			R1,=PB_INP
			
loop		LDR			R0,[R1]
			CMP			R0,#0x0F		;take input and arrange debouncing
			BEQ			loop
			BL			DELAY20
			LDR			R3,[R1]
			CMP			R3,R0
			BNE			loop
			EOR			R2,R0,#0x0F		;toggle the input pins to find if any press process is done
			CMP			R2,#0
			BEQ			loop
			
			CMP			R3,#0x0E
			BEQ			check1
			CMP			R3,#0x0D		;According to the press, call proper subroutine
			BEQ			check2
			CMP			R3,#0x0B
			BEQ			check3
			CMP			R3,#0x07
			BEQ			check4	

check1		BL			CHECK1
			BL			DELAY100
			B			start
check2		BL			CHECK2
			BL			DELAY100
			B			start
check3		BL			CHECK3
			BL			DELAY100
			B			start
check4		BL			CHECK4
			BL			DELAY100
			B			start
			ALIGN
			END
			
			
			
			