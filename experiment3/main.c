#include "head5.h"

int main(void){

initPortB();
initPortC();
initSysTick();
	
while(1){
	
	debouncePress();
	debounceRelease();
}
}