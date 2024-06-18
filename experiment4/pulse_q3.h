/*Pulse_init.h file
Function for creating a pulse train using interrupts
Uses Channel 0, and a 1Mhz Timer clock (_TAPR = 15)
Uses Timer0A to create pulse train on PF2
*/
#include <stdio.h>
#include "TM4C123GH6PM.h"

#define LOW 	30
#define HIGH 	20

void pulse_init(void);
void TIMER0A_Handler (void);
extern void OutStr(char*);
void portB_init(void);
void printer(int number);

void portB_init(void){
	SYSCTL->RCGCGPIO |= (1<<1);
	__ASM("NOP");
	__ASM("NOP");
	__ASM("NOP");
	
	GPIOB->DIR &= ~(1<<4);
	GPIOB->AFSEL |= (1<<4);
	GPIOB->PCTL &=~(15<<16);
	GPIOB->PCTL |=(7<<16);
	GPIOB->AMSEL = 0;
	GPIOB->DEN |=(1<<4);
	
	SYSCTL->RCGCTIMER |=(1<<1);
	__ASM("NOP");
	__ASM("NOP");
	__ASM("NOP");
	
	TIMER1->CTL	&=~(1<<0); //Disable timer during setup
	TIMER1->CFG =0x04;  //Set 16 bit mode
	TIMER1->TAMR =0x07; // set to capture mode,edge time, count down
	TIMER1->TAILR =60000; //
	TIMER1->CTL |= (1<<3)|(1<<2);
	
	TIMER1->CTL |=(1<<0);
}

void pulse_init(void){
	volatile int *NVIC_EN0 = (volatile int*) 0xE000E100;
	volatile int *NVIC_PRI4 = (volatile int*) 0xE000E410;
	SYSCTL->RCGCGPIO |= 0x20; // turn on bus clock for GPIOF
	__ASM("NOP");
	__ASM("NOP");
	__ASM("NOP");
	
  GPIOF->DIR			|= 0x04; //set PF2 as output
  GPIOF->AFSEL		&= (0xFFFFFFFB);  // Regular port function
	GPIOF->PCTL			&= 0xFFFFF0FF;  // No alternate function
	GPIOF->AMSEL		=0; //Disable analog
	GPIOF->DEN			|=0x04; // Enable port digital
	
	//GPIOF->DIR       |= 0x08; //set GREEN pin as a digital output pin
  //GPIOF->DEN       |= 0x08;  // Enable PF3 pin as a digital pin
	int k;
	for(k=0;k<4000;k++){}
	SYSCTL->RCGCTIMER	|=0x01; // Start timer0
	__ASM("NOP");
	__ASM("NOP");
	__ASM("NOP");
	TIMER0->CTL			&=0xFFFFFFFE; //Disable timer during setup
	TIMER0->CFG			|=(1<<2);  //Set 16 bit mode
	TIMER0->TAMR		=0x02; // set to periodic, count down
	TIMER0->TAILR		=LOW; //Set interval load as LOW
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
	GPIOF->DATA  ^= 4;  //toggle PF3 pin
	if(TIMER0->TAILR == LOW){
		TIMER0->CTL			&=0xFFFFFFFE;
		TIMER0->TAILR = HIGH;
		TIMER0->CTL			 |=0x03;
	}
	else if(TIMER0->TAILR == HIGH){
		TIMER0->CTL			&=0xFFFFFFFE;
		TIMER0->TAILR = LOW;
		TIMER0->CTL			 |=0x03;
	}
	else{}
	TIMER0->ICR |=0x01; //Clear the interrupt
	return;
}

void printer(int number){

	int i=0;
	char holder[10];
	char string[10];
	
	for(i; number>0;i++){
		holder[i] = number%10 + 48;
		number = number/10;
	}
	int j=0;
	for (i=i-1;i>=0;i--){
		string[j] = holder[i];
		j++;
	}
	string[j] ='\r';
	string[j+1] ='\4';
	OutStr(string);
}