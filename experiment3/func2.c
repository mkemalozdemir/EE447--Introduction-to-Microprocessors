#include "head3.h"

void initPortB(void){	//port B initialization
	
	SYSCTL->RCGCGPIO |= 0x02;
	GPIOB->DIR |= 0x0F;
	GPIOB->AFSEL &= ~(0xFF);
	GPIOB->AMSEL &= ~(0xFF);
	GPIOB->DEN	|= 0x0F;
	GPIOB->DATA	|= 0x01;
}
void initPortC(void){//port C initialization
	
	SYSCTL->RCGCGPIO |= 0x04;
	GPIOC->DIR &= ~0xF0;
	GPIOC->AFSEL &= ~(0xF0);
	GPIOC->AMSEL &= ~(0xF0);
	GPIOC->DEN	|= 0xF0;
	GPIOC->PUR |= 0xF0;
	GPIOC->DATA =0x01;
}
void initSysTick(void){	//SysTick initialization
	SysTick->CTRL &= ~(0x07);
	SysTick->LOAD = 4000000-1;
	SysTick->VAL = 4000000-1;
	SysTick->CTRL |= 0x03;
}
static int controller;
void SysTick_Handler(void){	//Interrup Handler
		if (controller == 0x0E){
			cw();
		}
		else if(controller == 0x0D){
			ccw();
		}

}

void cw(void){						//Rotate cw
	if (GPIOB->DATA == 0x01){
		GPIOB->DATA = 0x08;
	}
	else{
		GPIOB->DATA = GPIOB->DATA>>1;
	}
}

void ccw(void){					//Rotate ccw
	if (GPIOB->DATA == 0x08){
		GPIOB->DATA = 0x01;
	}
	else{
		GPIOB->DATA = GPIOB->DATA<<1;
	}
}
static int pbutton_p;
static int pbutton_pdeb;
static int pbutton_r;
static int pbutton_rdeb;
static int i;

void debouncePress(void){				//Debouncing the press
	
	while(1){
		pbutton_p = GPIOC->DATA>>4;
		pbutton_p &=0x0F;
		for(i=0; i<80000; i++){}
		pbutton_pdeb = GPIOC->DATA>>4;
		pbutton_pdeb &= 0x0F;
		if(pbutton_p == pbutton_pdeb && pbutton_p !=0x0F){
			break;
		}
	}
}

void debounceRelease(void){			//Debouncing the release
	while(1){
		pbutton_r = GPIOC->DATA>>4;
		pbutton_r &=0x0F;
		for(i=0; i<80000; i++){}
		pbutton_rdeb = GPIOC->DATA>>4;
		pbutton_rdeb &= 0x0F;
		if(pbutton_r == pbutton_rdeb && pbutton_r == 0x0F){
			controller = pbutton_p;
			break;
		}	
	}
}

