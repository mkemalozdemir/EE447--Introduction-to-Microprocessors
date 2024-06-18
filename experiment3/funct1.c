#include "head1.h"

void initPortB(void){	//port B initialization
	
	SYSCTL->RCGCGPIO |= 0x02;
	GPIOB->DIR |= 0x0F;
	GPIOB->AFSEL &= ~(0xFF);
	GPIOB->AMSEL &= ~(0xFF);
	GPIOB->DEN	|= 0xFF;
	GPIOB->DATA	|= 0x01;
}
void initSysTick(void){	// SysTick initialization
	SysTick->CTRL &= ~(0x01);
	SysTick->LOAD = 4000000-1;
	SysTick->VAL = 4000000-1;
	SysTick->CTRL |= 0x03;
}

static int change=0;
void SysTick_Handler(void){	//Interrupt Handler
		change++;
		if ((change/12)%2 == 0){
			cw();
		}
		else{
			ccw();
		}

}

void cw(void){						//rotate clockwise
	if (GPIOB->DATA == 0x01){
		GPIOB->DATA = 0x08;
	}
	else{
		GPIOB->DATA = GPIOB->DATA>>1;
	}
}

void ccw(void){				//rotate counter clockwise
	if (GPIOB->DATA == 0x08){
		GPIOB->DATA = 0x01;
	}
	else{
		GPIOB->DATA = GPIOB->DATA<<1;
	}
}

