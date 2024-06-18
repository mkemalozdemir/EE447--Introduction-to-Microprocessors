;LABEL      DIRECTIVE   VALUE       COMMENT
            AREA        routines, CODE, READONLY
            THUMB
			EXTERN		OutStr
			EXPORT		CONVRT
			
CONVRT		PROC						;start of the subroutine
			PUSH		{R0-R6,LR}
			LDR			R5,= 0x20000680	;Initialize R5
			LDR			R0,[R4]			;R0 holds the hexadecimal representation of the number
			MOV			R1, #10			;Set R1 as divider
			MOV 		R2, #0			;Set R2 as digit counter
			MOV			R3, R0			;Transfer R0 to R3
			MOV			R6, #0x04		;R6 holds string terminator
			
			CMP			R1, R0			;Compare divider and the number
			BGT			string			;If the number is one digit, go to string

counter		UDIV		R3, R3, R1		;Number is divided into 10
			ADD			R2, #1			;Increment in the digit counter			
			CMP			R1, R3		
			BGT			string			;After determine counter go to string
			B			counter			;Unconditional branch to the loop
					
string		ADD			R2, #1			;Increment in the digit counter
			STRB		R6, [R5,R2]		;Store string terminator

			
			
final		SUBS		R2, #1			;Decrement counter
			UDIV		R3, R0, R1		;Divide R0 into 10
			MUL			R3, R3, R1		;
			SUB			R3, R0, R3		;Hold remainder in R3
			ADD			R3, #48			;Add 48 to remainder to obtain decimal value according to ASCII table
			STRB		R3, [R5, R2]	;Store remainder in proper place
			UDIV		R0, R0, R1		;Update the number
			BEQ			end_prog
			B			final

end_prog	MOV			R0, R5 			;Address of the string assigned to the R0
			BL			OutStr
			POP			{R0-R6,LR}
			BX			LR
			ALIGN
			ENDP
			
			
			