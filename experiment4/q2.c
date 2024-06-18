#include "pulse_q2.h"

int main(void){
	int holder;
	int edge1;
	int edge2;
	int edge3;
	int width;
	int period;
	int duty_cycle;
	int counter = 0;
	
	portB_init();

	pulse_init();


	
	while(1){
		
		holder = TIMER1->RIS; // Assign RIS to holder to check interrupt
		holder &= 4;
		
		if(holder ==4 && counter ==0){
			edge1 = TIMER1->TAR/16;	//first edge detection
			counter++;
			TIMER1->ICR |=(1<<2);
		}
		else if(holder ==4 && counter ==1){
			edge2 = TIMER1->TAR/16;	//second edge detection
			counter++;
			TIMER1->ICR |=(1<<2);
		}
		else if(holder ==4 && counter ==2){
			edge3 = TIMER1->TAR/16; //third edge detection
			TIMER1->ICR |=(1<<2);
			break;
		}
	}
	width = edge1 - edge2;
	period = edge1 - edge3;
	duty_cycle = (100*width)/period;
	
	OutStr("Pulse Width: \r\4");
	printer(width);
	
	OutStr("Period: \r\4");
	printer(period);
	
	OutStr("Duty Cycle: \r\4");
	printer(duty_cycle);
	
	while(1){}
}