#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#include "TM4C123GH6PM.h"
#include "MyDefines.h"
#include "ez123g.h" // small board

void SegDisplay(uint8_t d, int digit);
void Setup_GPIO(void);
// You nave to finish the following array for Seven Segment Display
char SevenSeg[] = { 0x3f,   // 0
                    0x06,   // 1
                    0x5B,   // 2
										0x4F,   // 3
	                  0x66,   // 4
	                  0x6D,   // 5
	                  0x7D,   // 6
										0x07,   // 7
	                  0x7F,   // 8
	                  0x6F,   // 9
                  };
// Global Variables
uint8_t		MM,SS;		// MM: minute, SS: Second
bool			IsPause;
bool			Seg_Dp;
									
void		CheckSW();
void		Display7Seg();
void		ToggleDp();
									
int main()
{
		int	timer = 0;
		Setup_123G_80MHz(); //small board, it configures the CPU SPeed to 80 Mhz
    Setup_GPIO();
		MM = 0;
		SS = 0;
		IsPause = true;
		Seg_Dp = false;

    while (1) {
        // Place your application code here
				if((timer % 20) == 0){
					CheckSW();
				}
				
				//Display one digit of the seven-segment every 5 ms
				if((timer % 5) == 0 ){
					Display7Seg();
				}
				
				//Toggle the decimal point(Seg_DP) of the second digit (D2) every 500 ms
				if((timer % 500) == 0){
					ToggleDp();
				}
				
				//Increase the second digit by 1 every 1000 ms, and update the minute digit
				if((timer % 1000) == 0){
					if(IsPause != true){
					//increase SS by 1
					SS++;
					}
						
					//check if SS is greater than 59, set SS to 0 and increase MM by 1
					if(SS > 59){
						SS=0;
						MM++;
					}
						
					//check if 59:59, set IsPause to true
						if((SS == 59) && (MM==59)){
							IsPause = true;
							MM = 0;
							SS = 0;
						}
				}
			
			
				ezDelay(1);		// Delay for 1ms
				timer++;
    }
}
//------------------------------------------------------------------------------
void SegDisplay(uint8_t d, int digit)
{
     char    seg;
		seg = SevenSeg[d & 0xF];
		// check bit0 for Seg_A
		GPIOA ->DATA = ((seg & _BIT0) == 0) ? GPIOA-> DATA & (~_BIT2): GPIOA -> DATA | (_BIT2);
	
		// check bit1 for Seg_B
		GPIOA ->DATA = ((seg & _BIT1) == 0) ? GPIOA-> DATA & (~_BIT3): GPIOA -> DATA | (_BIT3);
	
		// check bit2 for Seg_C
	  GPIOA ->DATA = ((seg & _BIT2) == 0) ? GPIOA-> DATA & (~_BIT4): GPIOA -> DATA | (_BIT4);
	
		// check bit3 for Seg_D
		GPIOC ->DATA = ((seg & _BIT3) == 0) ? GPIOC-> DATA & (~_BIT4): GPIOC -> DATA | (_BIT4);
	
		// check bit4 for Seg_E
	  GPIOC ->DATA = ((seg & _BIT4) == 0) ? GPIOC-> DATA & (~_BIT5): GPIOC -> DATA | (_BIT5);
	
		// check bit5 for Seg_F
		GPIOC ->DATA = ((seg & _BIT5) == 0) ? GPIOC-> DATA & (~_BIT6): GPIOC -> DATA | (_BIT6);
	
		// check bit6 for Seg_G
		GPIOC ->DATA = ((seg & _BIT6) == 0) ? GPIOC-> DATA & (~_BIT7): GPIOC -> DATA | (_BIT7);
		
		//check bit7 for Seg_DP
		GPIOD ->DATA = ((digit == 1) && (Seg_Dp ==true)) ? GPIOD-> DATA | (_BIT7): GPIOD -> DATA &(~_BIT7);
		
	//GPIOx_AHB -> Data = ((digit ==1) && (Seg_Dp ==true)) ? GPIOx_AHB-> Data | _BITn : GPIOx_AHB -> Data & (~_BIT2);
}
//------------------------------------------------------------------------------
void Setup_GPIO(void)
{
    // Configure GPIOs
    // 1. Enable Clock to the GPIO Modules (SYSCTL->RCGCGPIO |= (_PORTs);)
    SYSCTL->RCGCGPIO |= (_PORTA | _PORTC | _PORTD |_PORTF);
    // allow time for clock to stabilize (SYSCTL->PRGPIO & (_PORTs))
    while((SYSCTL->PRGPIO & (_PORTA | _PORTC | _PORTD |_PORTF)) != (_PORTA | _PORTC | _PORTD |_PORTF)) {};
    // 2. Unlock GPIO only PD7, PF0 on TM4C123G; PD7, PE7 on TM4C1294 (GPIOx->LOCK = 0x4C4F434B; and GPIOx->CR = _PINs;)
    GPIOD->LOCK = 0x4C4F434B;
    GPIOD->CR |= _PIN7;
    GPIOF->LOCK = 0x4C4F434B;
    GPIOF->CR |= _PIN0;
    // 3. Set Analog Mode Select bits for each Port (GPIOx->AMSEL = _PINs; 0=digital, 1=analog)
    // 4. Set Port Control Register for each Port (GPIOx->PCTL = PMCn << _PTCL_PINn, check the PCTL table)
    // 5. Set Alternate Function Select bits for each Port (GPIOx->AFSEL = _PINs; 0=regular I/O, 1=PCTL peripheral)
    // 6. Set Output pins for each Port (Direction of the Pins: GPIOx->DIR = _PINs; 0=input, 1=output)
			GPIOA -> DIR = _PIN2 | _PIN3 | _PIN4;
			GPIOC -> DIR = _PIN4 | _PIN5 | _PIN6 |_PIN7;
			GPIOD -> DIR = _PIN0 | _PIN1 | _PIN2 |_PIN3 |_PIN7;
    // 7. Set PUR bits (internal Pull-Up Resistor), PDR (Pull-Down Resistor), ODR (Open Drain) for each Port (0: disable, 1=enable)
			GPIOF ->PUR = _PIN4 | _PIN0;
    // 8. Set Digital ENable register on all port.pins (GPIOx->DEN = _PINs; 0=disable, 1=enable)
			GPIOA -> DEN = _PIN2 | _PIN3 | _PIN4;
			GPIOC -> DEN = _PIN4 | _PIN5 | _PIN6 |_PIN7;
			GPIOD -> DEN = _PIN0 | _PIN1 | _PIN2 |_PIN3 |_PIN7;
			GPIOF ->DEN = _PIN4 |_PIN0;
}
//------------------------------------------------------------------------------

void		CheckSW()
{
	// 0 When Pressed, 1 When not pressed

		static bool		preSW1 = true;
		bool					curSw1;
		curSw1 = GPIOF ->DATA & _PIN4;
		if ((preSW1 == true) && (curSw1 ==false)){
			IsPause = !IsPause;
		}
		preSW1 = curSw1;
		
		
		static bool		preSW2 = true;
		bool					curSw2;
		curSw2 = GPIOF ->DATA & _PIN0;
		if ((preSW2 == true) && (curSw2 ==false)){
			MM = 0;
			SS = 0;
			IsPause = true;
		}
		preSW2 = curSw2;
}
//------------------------------------------------------------------------------
// this changes which display 
void		Display7Seg()
{
		static int digit = 0;
		uint8_t d;	// the value will be displayed on the 7-segement display
		switch(digit){
			case 0:
							//Set SEG_D1(PD0) to 1
							//Set SEG_D2,D3,D4 to 0	
							//Set d to MM 2nd digit
								GPIOD -> DATA |= (_PIN1 | _PIN2 |_PIN3);
								GPIOD ->DATA &= ~_PIN0;
			
								d = MM/10;
								break;
			case 1:
							//Set SEG_D2(PD1) to 1
							//Set SEG_D1,D3,D4 to 0
							//Set d to MM 1st digit
								GPIOD -> DATA |= (_PIN0 | _PIN2 |_PIN3);
								GPIOD ->DATA &= ~_PIN1;			
								d = MM %10;
								break;
			case 2:
							//Set SEG_D3(PD2) to 1
							//Set SEG_D2,D1,D4 to 0	
							// Set d to SS 2nd digit
								GPIOD -> DATA |= (_PIN1 | _PIN3 |_PIN0);
								GPIOD ->DATA &= ~_PIN2;			
			
								d = SS/10;
								break;
			default:
							//Set SEG_D4(PD3) to 1
							//Set SEG_D2,D3,D1 to 0	
							//Set d to SS 1st digit
								
								GPIOD -> DATA |= (_PIN1 | _PIN2 |_PIN0);
								GPIOD ->DATA &= ~_PIN3;
			
								d = SS%10;
								//break;
		}
		SegDisplay(d, digit);
		digit++;
		if(digit > 3){
			digit = 0;
		}
		
}
//------------------------------------------------------------------------------
void		ToggleDp()
{
			// Toggle (invert) Seg_Dp
		if (IsPause !=true){
			Seg_Dp = !Seg_Dp;
		}
}
//------------ ------------------------------------------------------------------

