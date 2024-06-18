#include <stdio.h>
#include <stdlib.h>
#include "TM4C123GH6PM.h"
void portB_init(void);
void printer(int num);
extern void OutStr(char*);

/*Pulse_init.h file
Function for creating a pulse train using interrupts
Uses Channel 0, and a 1Mhz Timer clock (_TAPR = 15)
Uses Timer0A to create pulse train on PF2
*/

#include "TM4C123GH6PM.h"
void pulse_init(void);
void TIMER0A_Handler (void);

#define TIME 4096
static int counter = 0;

void pulse_init(void){
	volatile int *NVIC_EN0 = (volatile int*) 0xE000E100;
	volatile int *NVIC_PRI4 = (volatile int*) 0xE000E410;
	SYSCTL->RCGCGPIO |= 0x20; // turn on bus clock for GPIOF
	__ASM("NOP");
	__ASM("NOP");
	__ASM("NOP");
	
  GPIOF->DIR			|= 0x08; //set PF2 as output
  GPIOF->AFSEL		&= (0xFFFFFFFB);  // Regular port function
	GPIOF->PCTL			&= 0xFFFFF0FF;  // No alternate function
	GPIOF->AMSEL		=0; //Disable analog
	GPIOF->DEN			|=0x08; // Enable port digital
	GPIOF->DATA 		&= ~0x08;
	
	SYSCTL->RCGCTIMER	|=0x01; // Start timer0
	__ASM("NOP");
	__ASM("NOP");
	__ASM("NOP");
	TIMER0->CTL			&=0xFFFFFFFE; //Disable timer during setup
	TIMER0->CFG			=0x04;  //Set 16 bit mode
	TIMER0->TAMR		=0x02; // set to periodic, count down
	TIMER0->TAILR		=TIME; //Set interval load as LOW
	TIMER0->TAPR		=15; // Divide the clock by 16 to get 1us
	TIMER0->IMR			=0x01; //Enable timeout intrrupt	
	
	//Timer0A is interrupt 19
	//Interrupt 16-19 are handled by NVIC register PRI4
	//Interrupt 19 is controlled by bits 31:29 of PRI4
	*NVIC_PRI4 &=0x00FFFFFF; //Clear interrupt 19 priority
	*NVIC_PRI4 |=0x40000000; //Set interrupt 19 priority to 2
	
	//NVIC has to be neabled
	//Interrupts 0-31 are handled by NVIC register EN0
	//Interrupt 19 is controlled by bit 19
	*NVIC_EN0 |=0x00080000;
	
	//Enable timer
	TIMER0->CTL			 |=0x03; // bit0 to enable and bit 1 to stall on debug
	return;
}

void TIMER0A_Handler (void){
	TIMER0->TAILR = TIME;
	GPIOF->DATA  ^= 8;  //toggle PF3 pin
	if(counter == 0){
		TIMER0->TAILR =(int) (TIME*(0xFFF&ADC0->SSFIFO3)/4096);
		counter = (counter+1)%2;
	}
	else if (counter ==1){
		TIMER0->TAILR =(int) (TIME*(1-(0xFFF&ADC0->SSFIFO3)/4096));
		counter = (counter+1)%2;
	}
	TIMER0->ICR |=0x01; //Clear the interrupt
	return;
}

void portB_init(void){
		
	SYSCTL->RCGCGPIO |= (1<<1);
	
	__ASM("NOP");
	__ASM("NOP");
	__ASM("NOP");
	
	GPIOB->DIR &= ~(1<<4);
	GPIOB->AFSEL |= (1<<4);
	GPIOB->AMSEL |= (1<<4);
	GPIOB->DEN &=~(1<<4);
	
	SYSCTL->RCGCADC |= (1<<0);
	
	__ASM("NOP");
	__ASM("NOP");
	__ASM("NOP");
	__ASM("NOP");
	__ASM("NOP");
	__ASM("NOP");
	__ASM("NOP");
	__ASM("NOP");
	__ASM("NOP");
	
	ADC0->ACTSS &=~(1<<3);
	
	ADC0->EMUX &=~(15<<12);
	
	ADC0->SSMUX3 &=~(15<<0);
	ADC0->SSMUX3 |= 0xA;
	
	ADC0->SSCTL3 &=~(15<<0);
	ADC0->SSCTL3 |= (1<<1)|(1<<2);
	
	ADC0->PC &=~(15<<0);
	ADC0->PC |=(1<<0);
	
	ADC0->ACTSS |=(1<<3);
}
void printer(int num){
	char string[10];
	int n = 0;
	int holder = abs(num);
	int X = holder*3.3/4095;
	int Y = holder*33/4095-10*X;
	int Z = holder*330/4095-100*X-10*Y;
	if(num<0){
		string[n++] = '-';
	}
	string[n++] = X+48;
	string[n++] = '.';
	string[n++] = Y+48;
	string[n++] = Z+48;
	string[n++] = '\r';
	string[n++] = '\4';
	OutStr(string);
}