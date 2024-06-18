;LABEL      DIRECTIVE   VALUE       COMMENT
NUM			EQU			0x20000680
FIRST		EQU			0X20000400           
			
			
			AREA        main, CODE, READONLY
            THUMB
			EXTERN		CONVRT
			EXTERN		UPBND
			EXTERN		InChar
				
			EXPORT		__main

__main		
			MOV			R2, #1		;minumum number held by R2
			MOV			R3, #10		;multiplier for arrangeing the second digit of n
			LDR			R1, =NUM
			LDR			R4, =FIRST
			BL			InChar		;take input for the second digit
			SUB			R0, #48
			STR			R0, [R1]	;store the second digit in R1
			LDR			R1,	[R1]	;R1 holds the second digit
			MUL			R1, R1, R3	;10*(second digit)
			
take_again	BL			InChar		;take input for the first digit
			CMP			R0, #0x0A
			BEQ			take_again
			SUB			R0, #48
			ADD			R1, R0		;first digit + 10*(second digit)
			LSL			R5, R2, R1	;Set the first upper limit as R5
			MOV			R3, #0		;Set the first lower limit as R3
			ADD			R1, R5, R3	;(lower+upper)
			LSR			R1, #1		;(lower+upper)/2
			STR			R1, [R4]	;store the guess in R4
			
cycle		BL			CONVRT		;Print the guess in decimal form
			BL			UPBND		;Take input for updating and update the guess and limits
			CMP			R0, #0x43	;Check correctness
			BEQ			end_guesser	;If input is C go to end_guesser
			B			cycle		;If not go to cycle

end_guesser	B			end_guesser	;infinite loop
			ALIGN
			END