#include "TM4C123GH6PM.h"

void initLCD(void);
void command(int cmd);
void checkFIFO(void);
void data(int d);


void resetLCD(void);
void readValue(char* value, int num);

void digit0(void);
void digit1(void);
void digit2(void);
void digit3(void);
void digit4(void);
void digit5(void);
void digit6(void);
void digit7(void);
void digit8(void);
void digit9(void);
void point(void);

void staticScreen(void);
char* print(int n);



void initLCD(void){

		SYSCTL->RCGCGPIO	|= 0x1;			//ACtivate clocf for port A
		__ASM("NOP");
		__ASM("NOP");
		__ASM("NOP");
		__ASM("NOP");
	
		SYSCTL->RCGCSSI		|=0x1;			//Activate SSI0
		__ASM("NOP");
		__ASM("NOP");
		__ASM("NOP");
		__ASM("NOP");
	
		GPIOA->DIR 				|= 0xC0;	//PA6, PA7 are output pins
		GPIOA->AFSEL			|= 0x2C;	//Enable alternative function for PA2,PA3,PA5
		GPIOA->DEN				|= 0xEC;	//Arrange PA7,PA6,PA5,PA3,PA2 as digital pins
		
		SSI0->CR1					&= ~0x2; //close for configuration
		SSI0->CC					 = 0x5;		//chose clock as PIOSC
		SSI0->CR0					|= 0x1400;	// SSInClk = SysClk / (CPSDVSR * (1 + SCR)) , 16 /(2*(1+1)) = 4Mhz
		SSI0->CPSR				|= 2;			//SSInClk = SysClk / (CPSDVSR * (1 + SCR)) , 16 /(2*(1+1)) = 4Mhz
		SSI0->CR0					|= 0xC7; //8 bit data, freescale mode, phase 1, polaritiy 0
		SSI0->CR1					|= 0x12;	//open
		
		GPIOA->DATA				|= 0x80; //PortA Pin 7 reset
		GPIOA->DATA				&= ~0x80;
		
		int i =4000000;	//100ms delay to initialize LCD
		while(i>0){
			i--;
		}
		
		GPIOA->DATA				|= 0x80;
		
		command(0x21);	//Extended intruction, horizontal
		command(0xC0);
		command(0x4);
		command(0x13);
		command(0x20);
		command(0xC);
		
		command(0x40);
		command(0x80);
		
		resetLCD();	//resetLCD LCD
		staticScreen();	// call the static screen
}
void command(int cmd){
		
	GPIOA->DATA &= ~0x40;
	SSI0->DR = cmd;
	checkFIFO();
}
void checkFIFO(void){
	while((SSI0->SR & 0x10) != 0);
}

void data(int d){
	GPIOA->DATA |= 0x40;
	SSI0->DR = d;
	checkFIFO();
}
void resetLCD(void){
	int i;
	for(i=0; i<504; i++){
		data(0x00);
	}
}

void readValue(char* value, int num){
	char* holder = value;
	int i;
	switch(num){
		case 0x0:
			command(0x40);
			command(0xAF);
			break;
		
		case 0x1:
			command(0x42);
			command(0xAF);
			break;
		case 0x2:
			command(0x44);
			command(0xAF);
			break;
	}
	for(i=0; i<4; i++){
		if(*holder == 0x30){
			digit0();
		}
		else if(*holder == 0x31){
			digit1();
		}
		else if(*holder == 0x32){
			digit2();
		}
		else if(*holder == 0x33){
			digit3();
		}
		else if(*holder == 0x34){
			digit4();
		}
		else if(*holder == 0x35){
			digit5();
		}
		else if(*holder == 0x36){
			digit6();
		}
		else if(*holder == 0x37){
			digit7();
		}
		else if(*holder == 0x38){
			digit8();
		}
		else if(*holder == 0x39){
			digit9();
		}
		else if(*holder == 0x2E){
			point();
		}
		holder++;
	}
}

void staticScreen(void){

	command(0x80);	// Low
	command(0x40);
	
	data(0x7F);
	data(0x40);
	data(0x40);		//L
	data(0x40);
	data(0x40);
	
	data(0x38);
	data(0x44);
	data(0x44);		//o
	data(0x44);
	data(0x38);
	
	data(0x3C);
	data(0x40);
	data(0x30);		//w
	data(0x40);
	data(0x3C);
	
	data(0x00);
	data(0x36);
	data(0x36);		//:
	data(0x00);
	data(0x00);
	
	command(0x42);	//Current
	command(0x80);
	
	data(0x3E);
	data(0x41);
	data(0x41);		//C
	data(0x41);
	data(0x00);
	
	data(0x3C);
	data(0x40);
	data(0x40);		//u
	data(0x20);
	data(0x7C);
	
	data(0x7C);
	data(0x08);
	data(0x04);		//r
	data(0x04);
	data(0x08);
	
	data(0x7C);
	data(0x08);
	data(0x04);		//r
	data(0x04);
	data(0x08);
	
	data(0x38);
	data(0x54);
	data(0x54);		//e
	data(0x54);
	data(0x18);
	
	data(0x7C);
	data(0x08);
	data(0x04);		//n
	data(0x04);
	data(0x78);
	
	data(0x04);
	data(0x3F);
	data(0x44);		//t
	data(0x40);
	data(0x20);
	
	data(0x00);
	data(0x36);
	data(0x36);		//:
	data(0x00);
	data(0x00);
	
	command(0x44);	//High
	command(0x80);
	
	data(0x7F);
	data(0x08);
	data(0x08);		//H
	data(0x08);
	data(0x7F);
	
	data(0x00);
	data(0x44);
	data(0x7D);		//i
	data(0x40);
	data(0x00);
	
	data(0x0C);
	data(0x52);
	data(0x52);		//g
	data(0x52);
	data(0x3E);
	
	data(0x7F);
	data(0x08);
	data(0x04);		//h
	data(0x04);
	data(0x78);
	
	data(0x00);
	data(0x36);
	data(0x36);		//:
	data(0x00);
	data(0x00);
}

void digit0(void){
	data(0x3E);
	data(0x51);
	data(0x49);
	data(0x45);
	data(0x3E);
}
void digit1(void){
	data(0x00);
	data(0x42);
	data(0x7F);
	data(0x40);
	data(0x00);
}
void digit2(void){
	data(0x42);
	data(0x61);
	data(0x51);
	data(0x49);
	data(0x46);
}
void digit3(void){
	data(0x21);
	data(0x41);
	data(0x45);
	data(0x4B);
	data(0x31);
}
void digit4(void){
	data(0x18);
	data(0x14);
	data(0x12);
	data(0x7F);
	data(0x10);
}
void digit5(void){
	data(0x27);
	data(0x45);
	data(0x45);
	data(0x45);
	data(0x39);
}
void digit6(void){
	data(0x3C);
	data(0x4A);
	data(0x49);
	data(0x49);
	data(0x30);
}
void digit7(void){
	data(0x01);
	data(0x71);
	data(0x09);
	data(0x05);
	data(0x03);
}
void digit8(void){
	data(0x36);
	data(0x49);
	data(0x49);
	data(0x49);
	data(0x36);
}
void digit9(void){
	data(0x06);
	data(0x49);
	data(0x49);
	data(0x29);
	data(0x1E);
}
void point(void){
	data(0x00);
	data(0x60);
	data(0x60);
	data(0x00);
	data(0x00);
}
char* print(int n){					//convert the integer to char array
	static char convert[4];
	int temp0 = n/1000;
	int temp1 = (n-temp0*1000)/100;
	int temp2 = (n-temp0*1000-temp1*100)/10;
	int temp3 = (n-temp0*1000-temp1*100-temp2*10);
	
	convert[0] = (temp0+48);
	convert[1] = (temp1+48);
	convert[2] = (temp2+48);
	convert[3] = (temp3+48);
	
	return convert;
}