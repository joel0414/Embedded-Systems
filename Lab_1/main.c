#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#include "TM4C123GH6PM.h"
#include "MyDefines.h"

void DelayMs(int ms);   // Software Delay Function
void Setup_GPIO(void);
#define _LEDB1	0X01
#define _LEDB2	0X02	
#define _LEDB3	0X04
#define _LEDR1	0X08
#define _LEDR2	0X10
#define _LEDR3	0X20

typedef struct LEDS{
	uint8_t		led;
	int			duration;
}LEDS;
LEDS	myLED1[] = {
	{_LEDB1 | _LEDR3,200},
	{0,40},
	{_LEDB2 |_LEDR2 ,200},
	{0,40},
	{_LEDB3 | _LEDR1,200},
	{0,40}
};

LEDS	myLED2[] = {
	{_LEDB1 | _LEDB2| _LEDB3,200},
	{0,40},
	{_LEDR3 |_LEDR2 |_LEDR1 ,200},
	{0,40},
	{_LEDB1 | _LEDB2| _LEDB3,200},
	{0,40},
	{_LEDR3 |_LEDR2 |_LEDR1 ,200},
	{0,40}
};
LEDS	myLED3[] = {
	{_LEDR1,200},
	{_LEDR2,200},
	{_LEDR3,200},
	{_LEDR3|_LEDR2|_LEDR1,200},
	{0,400},
	{_LEDB3,200},
	{_LEDB2,200},
	{_LEDB1,200},
	{_LEDB3 | _LEDB2 | _LEDB1,200},
	{0,400}
};

int main(void)
{
	unsigned int state;
	int arraySize;
	int pattern = 0;
	int 			duration;
	uint8_t		led;
	LEDS *leds;
	int i;
// Place your initialization/startup code here (e.g. Setup_GPIO() )
	Setup_GPIO();
	
	while (1) {
//The current value of _PORTF is read and is bitwise AND with the required pin needed to be 0 and assigned to state
		state = GPIOF ->DATA & _PIN4;
		
// if the result of that is zero aka the pin4 value was originally 0( sw 1 is pressed)
		if (state == 0){
			pattern++;
			if(pattern > 2){
			pattern = 0;}
		}
		if (pattern ==0){
			leds = myLED1;
			arraySize = sizeof(myLED1) / sizeof(myLED1[0]);
		}
		else if(pattern ==1){
			leds = myLED2;
			arraySize = sizeof(myLED2) / sizeof(myLED1[0]);
		}
		else{
			leds = myLED3;
			arraySize = sizeof(myLED3) / sizeof(myLED3[0]);
		}
		
			for(i = 0; i < arraySize; i++){
				led = leds[i].led;
				duration = leds[i].duration;
				
				if(led &_LEDB1){
					GPIOD -> DATA |= _PIN1;
				}else{
					GPIOD -> DATA &= ~_PIN1;
				}
				
				if(led &_LEDB2){
					GPIOD -> DATA |= _PIN2;
				}else{
					GPIOD -> DATA &= ~_PIN2;
				}
				
				if(led & _LEDB3) {
					GPIOD -> DATA |= _PIN3;
				}else{
					GPIOD -> DATA &= ~_PIN3;
				}
				
				if(led & _LEDR1){
					GPIOA -> DATA |= _PIN4;
				}else{
					GPIOA -> DATA &= ~_PIN4;
				}
				
				if(led & _LEDR2){
					GPIOA -> DATA |= _PIN3;
				}else{
					GPIOA -> DATA &= ~_PIN3;
				}
				if(led & _LEDR3){
					GPIOA -> DATA |= _PIN2;
				}else{
					GPIOA -> DATA &= ~_PIN2;
				}
				DelayMs(duration);
			}

		}
	}
//------------------------------------------------------------------------------
void Setup_GPIO(void)
{
// Configure GPIOs
// 1. Enable Clock to the GPIO Modules (SYSCTL->RCGCGPIO |= (_PORTs);)
	SYSCTL->RCGCGPIO |= (_PORTF | _PORTA | _PORTD);
	
// allow time for clock to stabilize (SYSCTL->PRGPIO & (_PORTs))
	while ((SYSCTL->PRGPIO & (_PORTF |_PORTA | _PORTD)) != (_PORTF |_PORTA | _PORTD)) {};
		
// 2. Unlock GPIO only PD7, PF0 on TM4C123G; PD7, PE7 on TM4C1294 (GPIOx->LOCK = 0x4C4F434B; and GPIOx->CR = _PINs;)
//GPIOF->LOCK = 0x4C4F434B;   // Unlock for GPIOF
//GPIOF->CR |= _PIN0;         // Commit for PIN0
// 3. Set Analog Mode Select bits for each Port (GPIOx->AMSEL = _PINs; 0=digital, 1=analog)
// 4. Set Port Control Register for each Port (GPIOx->PCTL = PMCn << _PTCL_PINn, check the PCTL table)
// 5. Set Alternate Function Select bits for each Port (GPIOx->AFSEL = _PINs; 0=regular I/O, 1=PCTL peripheral)
		
// 6. Set Output pins for each Port (Direction of the Pins: GPIOx->DIR = _PINs; 0=input, 1=output)
	GPIOF -> DIR = ~_PIN4;
	GPIOA -> DIR = _PIN2 |_PIN3 | _PIN4;
	GPIOD -> DIR = _PIN1 | _PIN2 | _PIN3;
		
// 7. Set PUR bits (internal Pull-Up Resistor), PDR (Pull-Down Resistor), ODR (Open Drain) for each Port (0: disable, 1=enable)
 GPIOF ->PUR = _PIN4;
// 8. Set Digital ENable register on all port.pins (GPIOx->DEN = _PINs; 0=disable, 1=enable)
	GPIOF -> DEN = _PIN4;
	GPIOA -> DEN= _PIN2 |_PIN3 | _PIN4;
	GPIOD -> DEN = _PIN1 | _PIN2 | _PIN3;	
}
//------------------------------------------------------------------------------
// Delay ms milliseconds (4167:50MHz TM4C123G CPU, 1605:16MHz TM4C123G CPU Clock)
void DelayMs(int ms)
{
    volatile int i, j;
    for (i = 0; i < ms; i++)
        for (j = 0; j < 4167; j++) {} // Do nothing for 1ms
}








