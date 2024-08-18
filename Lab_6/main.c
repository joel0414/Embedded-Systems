#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#include "TM4C123GH6PM.h"
#include "MyDefines.h"

void DelayMs(int ms);   // Software Delay Function

void Setup_GPIO(void);

// Full Step : 1-Phase at a Time
//uint8_t Steps[] = {0x01,0x02,0x04,0x08}; //Full Step: 1-phase at a time
uint8_t Steps[] = {_PIN4, _PIN4 | _PIN5, _PIN5,_PIN5 |_PIN6,_PIN6,_PIN6 | _PIN7,_PIN7,_PIN7 |_PIN4};
#define CW    0
#define CCW   1
void StepperMotor(int s, bool direction);
volatile uint32_t *Stepper = (uint32_t *)GPIOC + (_PIN7 | _PIN6 | _PIN5 | _PIN4);



//------------------------------------------------------------------------------
int main()
{
    int i = 0;
		bool sw1;
		bool sw2;
    Setup_GPIO();
    *Stepper = 0x01; // Step 0 value

    while(1){
        // If users press SW1, rotate CW for 5 steps
		sw1 = GPIOF ->DATA & _PIN0;
		sw2 = GPIOF ->DATA & _PIN4;
		// If users press SW2, rotate CCW for 5 steps
		if((sw1 == 0) && (sw2 ==0)){
			StepperMotor(1024,CW);
		}
		if((sw1 == 0) && (sw2 == 1)){
			StepperMotor(5,CW);
		}
		if((sw2 == 0) && (sw1 ==1)){
			StepperMotor(5,CCW);
		}
        DelayMs(20);
    }
}
//------------------------------------------------------------------------------
void Setup_GPIO(void)
{
    // GPIO Initialization and Configuration
    // 1. Enable Clock to the GPIO Modules (SYSCTL->RCGCGPIO |= (_PORTs);)
    SYSCTL->RCGCGPIO |= (_PORTC | _PORTF);
    // allow time for clock to stabilize (SYSCTL->PRGPIO)
    while ((SYSCTL->PRGPIO & (_PORTC | _PORTF) ) != (_PORTC | _PORTF) ){};
    // 2. Unlock GPIO only PD7, PF0 on TM4C123G; PD7, PE7 on TM4C1294 (GPIOx->LOCK = 0x4C4F434B; and GPIOx->CR = _PINs;)
    GPIOF->LOCK = 0x4C4F434B;
		GPIOF->CR |= _PIN0;
			// 3. Set Analog Mode Select bits for each Port (GPIOx->AMSEL = _PINs; 0=digital, 1=analog)
    // 4. Set Port Control Register for each Port (GPIOx->PCTL = PMCn << _PTCL_PINn, check the PCTL table)
    // 5. Set Alternate Function Select bits for each Port (GPIOx->AFSEL = _PINs; 0=regular I/O, 1=PCTL peripheral)
    // 6. Set Output pins for each Port (Direction of the Pins: GPIOx->DIR = _PINs; 0=input, 1=output)
		GPIOC ->DIR = _PIN4 |_PIN5 |_PIN6|_PIN7;
    // 7. Set PUR bits for internal pull-up, PDR for pull-down reg, ODR for open drain (0: disable, 1=enable)
		GPIOF ->PUR = _PIN4 | _PIN0;
    // 8. Set Digital ENable register on all port.pins (GPIOx->DEN = _PINs; 0=disable, 1=enable)
		GPIOC ->DEN = _PIN4 |_PIN5 |_PIN6|_PIN7;
		GPIOF->DEN = _PIN0 |_PIN4;
}
//------------------------------------------------------------------------------
void StepperMotor(int s, bool direction)
{
    int i = 0;
		//uint8_t step;
		static int idx = 0;
	
		while(i < s){
			
			if(direction == CW){
				idx++;
			}
			else{
				idx--;
			}
			if(idx < 0 ){
				idx = (sizeof(Steps)/sizeof(Steps[0])) - 1;
			}
			else if(idx >= (sizeof(Steps)/sizeof(Steps[0]))){
				idx = 0;
			}
			*Stepper = Steps[idx];
			i++;
			DelayMs(10);
		}
	
	
	
	
	
	
	
/*		// set *Stepper to Steps1[idx] value
		step = Steps[idx];
		if ((step & _BIT0) !=0) {
			// set PF4 TO 1
			*PF4 = _BIT4; // *PF4 = 0xFF
			
		}else {
			//set PF4 TO 0
			*PF4 = 0;
		}
	
*/	
	
}
//------------------------------------------------------------------------------
// Delay ms milliseconds (4167:50MHz TM4C123G CPU, 1605:16MHz TM4C1294 CPU Clock)
void DelayMs(int ms)
{
    volatile int i, j;
    for (i = 0; i < ms; i++)
        for (j = 0; j < 4167; j++) {} // Do nothing for 1 ms
}
//------------------------------------------------------------------------------