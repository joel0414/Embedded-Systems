#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#include "TM4C123GH6PM.h"
#include "MyDefines.h"

void Doorbell(void);
void RoomLightingControl(void);
void StairwallLightingControl(void);

void DelayMs(int ms);   // Software Delay Function
void Setup_GPIO(void);

int main()
{
    // Place your initialization/startup code here (e.g. Setup_GPIO() )
    Setup_GPIO();

    // Turn off LED2 here
		GPIOA ->DATA |= _BIT4;
 
    while (1) {
        Doorbell();
        RoomLightingControl();
        StairwallLightingControl();

        DelayMs(20);
    }
}
//------------------------------------------------------------------------------
void Doorbell()
{	
	bool sw1;
	// if pressed sw1 = 1 if not pressed than sw1 = 0
	sw1 = GPIOE->DATA & _PIN1;
	// if pressed 
	if(sw1 == 1){
		GPIOA ->DATA |= _BIT2;
	}else{
		GPIOA ->DATA &= ~_BIT2;
	}

}
//------------------------------------------------------------------------------
void RoomLightingControl()
{
	// needs static to be able to remember what its last value was
	static bool sw2PreState = 0;
  // variable for the status of the switch
	bool sw2;
	// if pressed sw1 = 1 if not pressed than sw1 = 0
	sw2 = GPIOE->DATA & _PIN2;
	
	if((sw2PreState == 0) && (sw2 == 1)){
		GPIOA ->DATA ^= _BIT3;
	}
	sw2PreState = sw2;
}
//------------------------------------------------------------------------------
void StairwallLightingControl()
{

		static bool sw3PreState = 1;
		static bool sw4PreState = 1;
    bool sw3;
		bool sw4;
		sw3 = GPIOB ->DATA & _PIN2;
		sw4 = GPIOB ->DATA & _PIN3;
		if((sw3PreState == 1) && (sw3 == 0)){
		GPIOA ->DATA ^= _BIT4;
		} 
		if((sw4PreState == 1) && (sw4 == 0)){
		GPIOA ->DATA ^= _BIT4;
		}
		sw3PreState = sw3;
		sw4PreState = sw4;

}

//------------------------------------------------------------------------------
void Setup_GPIO()
{
    // Configure GPIOs
    // 1. Enable Clock to the GPIO Modules (SYSCTL->RCGCGPIO |= (_PORTs);)
    SYSCTL->RCGCGPIO |= (_PORTA |_PORTE | _PORTB);
    // allow time for clock to stabilize (SYSCTL->PRGPIO & (_PORTs))
    while ((SYSCTL->PRGPIO & (_PORTA |_PORTE | _PORTB)) != (_PORTA |_PORTE |_PORTB)) {};
    // 2. Unlock GPIO only PD7, PF0 on TM4C123G; PD7, PE7 on TM4C1294 (GPIOx->LOCK = 0x4C4F434B; and GPIOx->CR = _PINs;)
    //GPIOF->LOCK = 0x4C4F434B;   // Unlock for GPIOF
    //GPIOF->CR |= _PIN0;         // Commit for PIN0
    // 3. Set Analog Mode Select bits for each Port (GPIOx->AMSEL = _PINs; 0=digital, 1=analog)
    // 4. Set Port Control Register for each Port (GPIOx->PCTL = PMCn << _PTCL_PINn, check the PCTL table)
    // 5. Set Alternate Function Select bits for each Port (GPIOx->AFSEL = _PINs; 0=regular I/O, 1=PCTL peripheral)
    // 6. Set Output pins for each Port (Direction of the Pins: GPIOx->DIR = _PINs; 0=input, 1=output)
		GPIOA ->DIR = _PIN2 | _PIN3 | _PIN4;
    // 7. Set PUR bits (internal Pull-Up Resistor), PDR (Pull-Down Resistor), ODR (Open Drain) for each Port (0: disable, 1=enable)
		GPIOE -> PDR = _PIN1 | _PIN2;
		GPIOB ->PUR = _PIN2 | _PIN3;
    // 8. Set Digital ENable register on all port.pins (GPIOx->DEN = _PINs; 0=disable, 1=enable)
		GPIOA ->DEN =_PIN2 | _PIN3 | _PIN4;
		GPIOE ->DEN =_PIN1 | _PIN2;
		GPIOB ->DEN = _PIN2 | _PIN3;

}
//------------------------------------------------------------------------------
// Delay ms milliseconds (4167:50MHz TM4C123G CPU, 1605:16MHz TM4C123G CPU Clock)
void DelayMs(int ms)
{
    volatile int i, j;
    for (i = 0; i < ms; i++)
        for (j = 0; j < 4167; j++) {} // Do nothing for 1ms
}					
					
					
					
					
					
					
					
					