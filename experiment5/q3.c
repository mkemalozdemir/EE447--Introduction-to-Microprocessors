#include "header3.h"

int main(void){

portB_init();
int x;
int value;
int resultValue;
int delay;	

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
		value = ADC0->SSFIFO3;
		value &= 0xFFF;
		resultValue =value - 2048; //2062.5 or 2032.5
		
		__ASM("NOP");
		__ASM("NOP");
		__ASM("NOP");
		
		OutStr("Output Voltage: \r\4");
		
		__ASM("NOP");
		__ASM("NOP");
		__ASM("NOP");
		
		printer(resultValue);
		
		__ASM("NOP");
		__ASM("NOP");
		__ASM("NOP");
		
		ADC0->ISC |= (1<<0);
		
		__ASM("NOP");
		__ASM("NOP");
		__ASM("NOP");
		
		for(delay = 0; delay <1000000; delay++){}
	}
}
}