#include "TM4C123GH6PM.h"
#include <stdio.h>
#include <stdlib.h>

void initPortF(void);
void initADC(void);
void GPIOF_Handler(void);
void controlRGB(int luminosity);

static int x;
static int LOWTH = 15;
static int HIGHTH = 100;

void initPortF(void){

	SYSCTL->RCGCGPIO |= 0x20; // turn on bus clock for GPIOF
	__ASM("NOP");
	__ASM("NOP");
	__ASM("NOP");
	__ASM("NOP");
	__ASM("NOP");
	__ASM("NOP");
	
	GPIOF->LOCK			=0x4C4F434B;
	GPIOF->CR				=0x01;
	
	
  GPIOF->DIR			|= (0x08 | 0x04 | 0x02); //set PF1, PF2, PF3 as output
  GPIOF->AFSEL		&= (0xFFFFFFE0);  // Regular port function
	GPIOF->AMSEL		=0; //Disable analog function of the port
	GPIOF->DEN			|=0x1F; // Enable port digital for PF0-PF4
	GPIOF->PUR			|=((1<<4)|(1<<0)); //Pull Up resistors for switch 0 and 1
	
	GPIOF->IS				&= ~(1 << 4)|~(1<<0); //edge sensitive
	GPIOF->IBE			&= ~(1 << 4)|~(1<<0); //No both edge
	GPIOF->IEV			&= ~(1 << 4)|~(1<<0); //Falling edge
	GPIOF->ICR 			|= (1<<4)|(1<<0);			//Interrupt clear
	GPIOF->IM				|= (1 << 4)|(1<<0);		//Enable interrupt for PF0,PF4
	
	NVIC->IP[30]     = 3 << 5;	//Set priority as 6 for Port F interrupts
	NVIC->ISER[0]   |= (1<<30); //Enable PORTF Interrupt IRQ30 
	__enable_irq();

}

void initADC(void){ //To get analog input

	SYSCTL->RCGCGPIO |= (1<<1); //Enable clock for Port B
	
	__ASM("NOP");
	__ASM("NOP");
	__ASM("NOP");
	__ASM("NOP");
	__ASM("NOP");
	__ASM("NOP");
	
	GPIOB->DIR &= ~(1<<4);	//PB4 is input
	GPIOB->AFSEL |= (1<<4); //Alternative function select for PB4 AIN10
	GPIOB->AMSEL |= (1<<4);	//set as anlog pin
	GPIOB->DEN &=~(1<<4); // No digital pin
	
	SYSCTL->RCGCADC |= (1<<0);	//Enable clock for ADC
	
	__ASM("NOP"); //for stabilization of ADC0 module
	__ASM("NOP");
	__ASM("NOP");
	__ASM("NOP");
	__ASM("NOP");
	__ASM("NOP");
	__ASM("NOP");
	__ASM("NOP");
	__ASM("NOP");
	__ASM("NOP");
	__ASM("NOP");
	__ASM("NOP");
	__ASM("NOP");
	
	
	ADC0->ACTSS &=~(1<<3);	//SS3 is disabled
	
	ADC0->EMUX &=~(15<<12); 
	
	ADC0->SSMUX3 &=~(15<<0);//SSMUX3 is cleared
	ADC0->SSMUX3 |= 0xA;		//AIN10 is selected to be sampled as input
	
	ADC0->SSCTL3 &=~(15<<0); //SSCTL3 is cleared
	ADC0->SSCTL3 |= (1<<1)|(1<<2); //First sample is the last sample, after the first sample interrupt is enabled
	
	ADC0->PC &=~(15<<0);	//Peripheral config is clear to be set
	ADC0->PC |=(1<<0);		//125 kbps is selected
	
	ADC0->ACTSS |=(1<<3);	//SS3 is enabled
}

void GPIOF_Handler(void){
	int i;
	while(1){
		ADC0->PSSI |= (1<<3);
		__ASM("NOP");
		__ASM("NOP");
		x = ADC0->RIS;
		x &=0x8;
		
		__ASM("NOP");
		__ASM("NOP");
		__ASM("NOP");
		__ASM("NOP");
		
		if(x==8){
			if(GPIOF->RIS & 0x01){						// if the interrupt is sensed from switch2
					if((ADC0->SSFIFO3 & 0xFFF) > LOWTH){ 
							for(i=0; i<100; i++);
							HIGHTH = (ADC0->SSFIFO3 & 0xFFF);	// update HIGHTH
					}
					ADC0->ISC |= (1<<3);
					GPIOF->ICR |= (1<<0);
					break;
			}
			else if(GPIOF->RIS & 0x10){ 		// if the interrupt is sensed from switch1
					if((ADC0->SSFIFO3 & 0xFFF) < HIGHTH){
							for(i=0; i<100; i++);
							LOWTH = (ADC0->SSFIFO3 & 0xFFF);	//update LOWTH
					}
					ADC0->ISC |= (1<<3);
					GPIOF->ICR |= (1<<4);
					break;
			}
		}
	}
} 

void controlRGB(int luminosity){
	
	if(luminosity < LOWTH){			//If lux<LOWTH, Red LED is open 
		GPIOF->DATA &= ~(0xE);
		GPIOF->DATA |= 0x2;
	}
	else if(LOWTH<luminosity && luminosity<HIGHTH){ //If LOWTH< lux < HIGHTH , Green LED is open
		GPIOF->DATA &= ~(0xE);
		GPIOF->DATA |= 0x8;
	}
	else if(HIGHTH<luminosity){ //If lux>HIGHTH, Blue LED is open
		GPIOF->DATA &= ~(0xE);
		GPIOF->DATA |= 0x4;
	}
}