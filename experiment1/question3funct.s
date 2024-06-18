;LABEL      DIRECTIVE   VALUE       COMMENT
            AREA        routines2, CODE, READONLY
            THUMB
			EXTERN		InChar
			EXPORT		UPBND
			
UPBND		PROC					;start of the subroutine
			PUSH		{LR}
get_char	BL			InChar		;Take input for updating
			CMP			R0, #0x0A
			BEQ			get_char
			CMP			R0, #0x44	;Compare with D
			BEQ			down		;Go to Down operation
			CMP			R0, #0x55	;Compare with U
			BEQ			up			;Go to Up operation
			CMP			R0, #0x43	;Check correctness with C
			BEQ			correct		;Go to correct operation
			
down		LDR			R5, [R4]	;Set the guess as R5 which is upper limit
			SUB			R5, #1		;Decrease the upper limit
			B			end_f		;Branch to end_f

up			LDR			R3, [R4]	;Set the guess as R3 which is lower limit
			ADD			R3, #1		;Increase lower limit
			B			end_f		;Branch to end_f

correct		B			end_f

end_f		ADD			R2, R5, R3	;lower+upper
			LSR			R2, #1		;(lower+upper)/2
			STR			R2, [R4]	;Set new guess
			POP			{LR}
			BX			LR			;Exit function
			ENDP