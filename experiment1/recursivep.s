;LABEL      DIRECTIVE   VALUE       COMMENT
FIRST		EQU			0X20000400              
			
			AREA        main, CODE, READONLY
            THUMB
			EXTERN		CONVRT
			EXTERN		InChar
			EXTERN		OutStr
				
			EXPORT		__main

__main		
			LDR			R4,=FIRST
			MOV			R1, #10			;R1 holds 10 for the second digit of monster health
			MOV			R2, #100		;R2 holds 100 for the third digit of monster health
take_third	BL			InChar			;Take third_input as third digit
			CMP			R0, #0x0A
			BEQ			take_third
			SUB			R0, #48
			MUL			R3, R0, R2		;input*100 
take_second	BL			InChar			;Take second_input as second digit
			CMP			R0, #0x0A
			BEQ			take_second
			SUB			R0, #48
			MLA			R3, R0, R1, R3	;R3 holds second_digit*10 + third_input*100
take_first	BL			InChar			;Take first_input as first digit
			CMP			R0, #0x0A
			BEQ			take_first
			SUB			R0, #48
			ADD			R3, R0, R3		;R3 holds R3 + first_input
			
take_firsts	BL			InChar			;Take seconds_input as second digit of the sword damage
			CMP			R0, #0x0A
			BEQ			take_firsts
			SUB			R0, #48
			MUL			R5, R0, R1		;seconds_input*10
take_scnds  BL			InChar			;Take firsts_input	as first digit of the sword damage
			CMP			R0, #0x0A
			BEQ			take_scnds
			SUB			R0, #48
			ADD			R5, R0, R5		;R5 holds the sword damage
			
			
			LSR			R1, R3, #1		;R1 holds half of the monster health
			LDR			R0, =0x20000500
			MOV			R6, #0x0420
			STR			R6, [R0]

			BL			rec
			
end_program	B			end_program		;infinite loop

rec			PUSH 		{R3,LR}
			
fight		SUBS		R3,R5
			BEQ			finish
			BLT			update
			B			fight
			

update		ADD			R3,R3,R1
			LSR			R5,#1
			BL			rec

finish		POP			{R3}			;R3 holds the last value in the stack
			STR			R3, [R4]		;Store the R3 in R4
			BL			CONVRT			;Print the healths
			PUSH		{R0}			
			BL			OutStr			;Print space
			POP			{R0}
			POP			{LR}
			MOV			PC,LR
				
			ALIGN
			END
				

			