SYSCTL_RCGCGPIO		EQU			0x400FE608
SYSCTL_RCGCI2C		EQU			0x400FE620

GPIO_PORTB_DATA		EQU			0x400053FC
GPIO_PORTB_DIR		EQU			0x40005400
GPIO_PORTB_AFSEL	EQU			0x40005420
GPIO_PORTB_DEN		EQU			0x4000551C
GPIO_PORTB_AMSEL	EQU			0x40005528
GPIO_PORTB_PCTL		EQU			0x4000552C
GPIO_PORTB_ODR		EQU			0x4000550C

I2C0_MSA			EQU			0x40020000
I2C0_MCS			EQU			0x40020004
I2C0_MDR			EQU			0x40020008
I2C0_MTPR			EQU			0x4002000C
I2C0_MIMR			EQU			0x40020010
I2C0_MRIS			EQU			0x40020014
I2C0_MMIS			EQU			0x40020018
I2C0_MICR			EQU			0x4002001C	
I2C0_MCR			EQU			0x40020020
I2C0_MCLKOCNT		EQU			0x40020024
I2C0_MBMON			EQU			0x4002002C
I2C0_MCR2			EQU			0x40020038

ARRAY_ADDRESS      	EQU         0x20000400
SLAVE_ADDRESS		EQU			0x00000039
	


	
				AREA i2cinit, CODE, READONLY
				THUMB
				EXPORT	initI2C
				EXPORT	openSensor
				EXPORT	readBYTE
				EXPORT	isBusy
				EXPORT	putArray
				
initI2C			PROC												;I2C initialization
				
				LDR 	R1,=SYSCTL_RCGCGPIO 						;Clock register
				LDR 	R0, [R1]
				ORR 	R0, R0, #0x02 								;turn on clock for PORT B
				STR 	R0, [R1]
				NOP
				NOP
				NOP
				NOP
				NOP
				NOP
				
				LDR 	R1,=SYSCTL_RCGCI2C 							;Clock register
				LDR 	R0, [R1]
				ORR 	R0, R0, #0x01 								;turn on clock for I2C0
				STR 	R0, [R1]
				NOP
				NOP
				NOP
				NOP
				NOP
				NOP	

				LDR 	R1,=GPIO_PORTB_AFSEL
				LDR 	R0, [R1]
				BIC 	R0, #0xFF 									; logical AND NOT
				ORR 	R0, R0, #0x0C
				STR 	R0, [R1]
				
				LDR 	R1,=GPIO_PORTB_PCTL
				LDR 	R0, [R1]
				BIC 	R0, #0xFFFFFFFF 							; Determine the desired configuration per pin
				ORR 	R0, R0, #0x00003300
				STR 	R0, [R1]

				LDR 	R1,=GPIO_PORTB_DEN
				MOV 	R0, #0xFF 									; enable Port as digital port
				STR 	R0, [R1]
				LDR 	R1,=GPIO_PORTB_AMSEL
				MOV 	R0, #0 										; disable analog functionality
				STR 	R0, [R1]
				
				LDR 	R1,=GPIO_PORTB_ODR
				LDR 	R0, [R1]
				BIC 	R0, #0xFF 									; logical AND NOT
				ORR 	R0, R0, #0x08
				STR 	R0, [R1]
				
				LDR 	R1,=I2C0_MCR								;enable master
				MOV		R0, #0x10
				STR 	R0, [R1]
				
				LDR 	R1,=I2C0_MTPR								;enable 100 Kbps
				MOV		R0, #0x07
				STR 	R0, [R1]
				
				BX		LR
				ALIGN
				ENDP
					
openSensor		PROC					;SMBus WRITE BYTE PROTOCOL
				PUSH	{R0,R1,LR}

				LDR 	R1,=I2C0_MSA		
				MOV		R0, #0x39   								;SLAVE ADRESS 0x39 , FIRSTLY MASTER IS TRANSMITTER
				LSL		R0, #1
				STR 	R0, [R1]
				
				LDR 	R1,=I2C0_MDR
				MOV 	R0, #0x80   								;Control register address to open sensor
				STRB 	R0, [R1]
				
				LDR 	R1,=I2C0_MCS
				MOV 	R0, #0x3   									; FIRST BYTE -> START, SLAVE ADDRESS, R\S = 0
				STR 	R0, [R1]		;
				
				BL		isBusy
		
				LDR 	R1,=I2C0_MDR
				MOV 	R0, #0x03   								;control command to power up the sensor
				STR 	R0, [R1]
				
				LDR 	R1,=I2C0_MCS		
				MOV 	R0, #0x5   									; FIRST BYTE -> START, SLAVE ADDRESS, R\S = 0
				STR 	R0, [R1]
				
				BL		isBusy
				
				LDR 	R1,=I2C0_MSA		
				MOV		R0, #0x39   								;SLAVE ADRESS 0x39 , FIRSTLY MASTER IS TRANSMITTER
				LSL		R0, #1
				STR 	R0, [R1]
				
				LDR 	R1,=I2C0_MDR
				MOV 	R0, #0x81   								;Timing register to decrease integration time
				STRB 	R0, [R1]
				
				LDR 	R1,=I2C0_MCS
				MOV 	R0, #0x3   									; FIRST BYTE -> START, SLAVE ADDRESS, R\S = 0
				STR 	R0, [R1]		;
				
				BL		isBusy
		
				LDR 	R1,=I2C0_MDR
				MOV 	R0, #0x02   								;timing command to determine integration time
				STR 	R0, [R1]
				
				LDR 	R1,=I2C0_MCS		
				MOV 	R0, #0x5   									; FIRST BYTE -> START, SLAVE ADDRESS, R\S = 0
				STR 	R0, [R1]
				
				BL		isBusy
				
				POP		{R0,R1,LR}
				BX 		LR
				ALIGN
				ENDP



readBYTE		PROC								;SMBus READ BYTE PROTOCOL (int slaveAddress(R0), char slaveCommand (R1))
				PUSH	{R2-R3,LR}

				LDR 	R3, =I2C0_MSA
				LSL		R2, R0, #1   				;SLAVE ADRESS 0x39 , FIRSTLY MASTER IS TRANSMITTER
				STR 	R2, [R3]
				
				LDR 	R3,=I2C0_MDR
				STR 	R1, [R3] 		
				
				LDR 	R3,=I2C0_MCS		
				MOV 	R2, #0x3   					; FIRST BYTE -> START, SLAVE ADDRESS, R\S = 0
				STR 	R2, [R3]		;
				
				BL		isBusy						;start receive
				
				LDR 	R3, =I2C0_MSA
				LSL		R2, R0, #1   				;SLAVE ADRESS 0x39 , MASTER IS RECEIVER
				ADD		R2,R2,#0x1
				STR 	R2, [R3]
				
				LDR 	R3,=I2C0_MCS		
				MOV 	R2, #0x7					;receive first byte
				STR 	R2, [R3]		
				
				BL		isBusy
				
				AND		R0,R0,#0
				LDR 	R3,=I2C0_MDR				;RETURN CHAR AS R0
				LDRB 	R0, [R3] 
				
				POP		{R2-R3,LR}
				BX		LR
				ALIGN
				ENDP
				
					
isBusy			PROC
				PUSH 	{R0,R1}
check			LDR 	R1,=I2C0_MCS
				LDR		R0,[R1]
				AND		R0,#0x1
				CMP		R0,#0
				BNE		check
				POP		{R0,R1}
				BX		LR
				ENDP
					
putArray		PROC	;PUT THE SENSOR VALUES INTO AN ARRAY AND RETURN THE START OF ADDRESS OF THE ARRAY 
				PUSH	{R2-R3,LR}
				
				MOV		R3,#0
				LDR		R2,=ARRAY_ADDRESS
				
sample			BL		openSensor
				LDR		R0,=SLAVE_ADDRESS
				MOV		R1,#0x8C
				BL		readBYTE
				STRB	R0,[R2,R3]
				ADD		R3,R3,#1
				
				
				LDR		R0,=SLAVE_ADDRESS
				MOV		R1,#0x8D
				BL		readBYTE
				STRB	R0,[R2,R3]
				ADD		R3,R3,#1
				
				
				LDR		R0,=SLAVE_ADDRESS
				MOV		R1,#0x8E
				BL		readBYTE
				STRB	R0,[R2,R3]
				ADD		R3,R3,#1
				
				
				LDR		R0,=SLAVE_ADDRESS
				MOV		R1,#0x8F
				BL		readBYTE
				STRB	R0,[R2,R3]
				ADD		R3,R3,#1
				
				
				CMP		R3,#512
				BNE		sample

				
				LDR		R0,=ARRAY_ADDRESS
				POP		{R2-R3,LR}
				BX		LR
				ALIGN
				ENDP

				
				END
					