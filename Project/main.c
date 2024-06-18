#include "TM4C123GH6PM.h"
#include <stdio.h>
#include <stdlib.h>
#include "initPWM.h"
#include "Luminosity.h"
#include "initLCD.h"


static char* low;
static char* high;
static char* current;

int main(void){

	initI2C();
	initLCD();
	initPWM();
	initPortF();
	initADC();
	int LUX = 0;
	while(1){
		
		
		LUX = luminosity();		//determine the lux value
		updatePWM(LUX);				//updatePWM PWM value
		controlRGB(LUX);			//determine the color of LED
		low = print(LOWTH);		
		readValue(low,0);			//print the LOWTH in LCD
		high = print(HIGHTH);
		readValue(high,2);		//print the HIGHTH in LCD
		current = print(LUX);
		readValue(current,1);	//print the current LUX in LCD
	}
}
