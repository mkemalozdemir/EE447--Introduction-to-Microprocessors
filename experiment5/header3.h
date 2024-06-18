#include <stdio.h>
#include <stdlib.h>
#include "TM4C123GH6PM.h"
void portB_init(void);
void printer(int num);
extern void OutStr(char*);

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