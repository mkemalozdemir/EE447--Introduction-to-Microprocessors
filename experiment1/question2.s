;LABEL      DIRECTIVE   VALUE       COMMENT
NUM			EQU			0x20000680
FIRST		EQU			0X20000400
;***************************************************************


;LABEL      DIRECTIVE   VALUE       COMMENT
            AREA        main, CODE, READONLY
            THUMB
			EXTERN		CONVRT
			EXTERN		InChar
				
			EXPORT		__main

__main		
			LDR			R5	,=NUM
			LDR			R4	,=FIRST
get_char	BL			InChar		;Take input
			CMP			R0, #0x0A	
			BEQ			get_char
			STR			R0, [R4]	;Put the hexadecimal representation of the char in the address held by R4
			BL			CONVRT		;Call subroutine to print decimal representation of the char
			B			get_char	;Branch to the first step
			ALIGN
			END
			