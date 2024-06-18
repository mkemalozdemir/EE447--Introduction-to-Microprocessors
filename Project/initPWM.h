#include "TM4C123GH6PM.h"
#include <stdio.h>
#include <stdlib.h>
#include "initRGB.h"

void initPWM(void);
void updatePWM(int luminosity);


void initPWM(void){
	SYSCTL->RCGCPWM |= 1; 			//enable clock source to PWM0 module
	SYSCTL->RCGCGPIO  |= 0x02	; // Enable the clock for port B

	SYSCTL->RCC |= (1<<20);    // Enable System Clock Divisor function  
	SYSCTL->RCC |= 0x000E0000; // Use pre-divider valur of 64 and after that feed clock to PWM1 module*/
		
		
	GPIOB->AFSEL |= (1<<6);  // PB6 sets a alternate function 
	GPIOB->PCTL &= ~0x0F000000; 
	GPIOB->PCTL |= 	0x04000000; // make PB6 PWM output pin 
	GPIOB->DEN |= (1<<6);      // set PB6 as a digital pin 
		
		
	PWM0->_0_CTL &= ~(1<<0);   // Disable Generator 0 counter 
	PWM0->_0_CTL &= ~(1<<1);   // select down count mode of counter 3
	PWM0->_0_GENA = 0x0000008C;  // Set PWM output when counter reloaded and clear when matches PWMCMPA 
	PWM0->_0_LOAD = 5000;     // set load value for 50Hz 16MHz/64 = 250KHz and (250KHz/5000) 
	PWM0->_0_CMPA = 4999;     // set duty cyle to to minumum value
	PWM0->_0_CTL = 1;           // Enable Generator 0 counter 
	PWM0->ENABLE = 0x01;      // Enable PWM0 channel 0 output 		
}

void updatePWM(int luminosity){
	if(luminosity>HIGHTH){ //For a value greater than the high threshold, led lights with full brightness
	PWM0->_0_CMPA = 1;
	}
	else if (luminosity<LOWTH){	//For a value smaller than the low threshold, led is closed
	PWM0->_0_CMPA = 4999;
	}
	else{	//For a value between the thresholds, brighness is scaled 
	PWM0->_0_CMPA = 5000*(HIGHTH-luminosity)/(HIGHTH-LOWTH);
	}
}
