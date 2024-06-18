#include <stdio.h>
#include "TM4C123GH6PM.h"

void initPortB(void);
void initPortC(void);
void initSysTick(void);
void SysTick_Handler(void);
void cw(void);
void ccw(void);
void debouncePress(void);
void debounceRelease(void);
void speedcontrol(void);