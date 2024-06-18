#include "pulse_q3.h"


/* EGER KOD CALISMAZSA "pulse_q2.h" YI KULLAN
	 DELAYI ARTIR
	 UZAK MESAFE ICIN PRESCALE KULLAN
	 PERIODU AYARLAMAK ICIN TIMER0 HANDLERI AZALT
	 */
int main(){
	int holder;
	int edge1;
	int edge2;
	int width;
	int distance;
	int counter = 0;
	
	portB_init();
	pulse_init();
	
	while(1){
		
		holder = TIMER1->RIS;
		holder &= 0x04;
		
		if(holder !=0 && counter ==0){
			edge1 = TIMER1->TAR/16;
			counter++;
			TIMER1->ICR |=(1<<2);
		}
		else if(holder !=0 && counter ==1){
			edge2 = TIMER1->TAR/16;
			counter = 0;
			TIMER1->ICR |=(1<<2);
			break;
		}
	}
	width = edge1 - edge2;
	distance = width*(0.17);
	
	OutStr("Pulse Width: \r\4");
	printer(width);
	
	OutStr("Distance(mm): \r\4");
	printer(distance);
	

	while(1){}
}

