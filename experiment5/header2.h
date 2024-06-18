#include <stdio.h>
#include "TM4C123GH6PM.h"
void portB_init(void);


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
