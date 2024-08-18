#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

#include "TM4C123GH6PM.h"
#include "MyDefines.h"
#include "ez123G.h"
char ReadKeypad(void);
void Setup_GPIO(void);

int main(void)
{
    PEZOBJ_LCD  lcd;
    char        str[100];
    uint16_t    i = 0;
    char        ch;

    Setup_GPIO();

    lcd = ezLCD_Create();
    ezLCD_Connect_DataPort(lcd, _PORTD,ON_PIN_3_0);
	
    ezLCD_Connect_ENPin(lcd, _PORTE, _PIN1);
    ezLCD_Connect_RSPin(lcd, _PORTE, _PIN2);
    ezLCD_Connect_RWPin(lcd, _PORTE, _PIN3);

    ezLCD_Start(lcd);
    ezLCD_ClearDisplay(lcd);
    ezLCD_Position(lcd, 1, 0);
    ezLCD_PrintString(lcd, "HELLO");

    while(1){
        ch = ReadKeypad();

        if (ch == '*') i *= 100;
        if (ch == '#') {
            i = 0;
            ezLCD_ClearDisplay(lcd);
        }
        if (ch >='0' && ch <= '9') i = i*10 + ch - '0';

        sprintf(str, "%d ", i);
        ezLCD_Position(lcd, 0, 0);
        ezLCD_PrintString(lcd, str);

        ezDelay(100);
    }
}
//--------------------------------------------------------------
char Keypad[4][4]={         // Lookup Table for Keypad
    {'1','2','3','A'},
    {'4','5','6','B'},
    {'7','8','9','C'},
    {'*','0','#','D'}};

volatile uint32_t *R1 = (uint32_t *)GPIOB + (_PIN0);
volatile uint32_t *R2 = (uint32_t *)GPIOB + (_PIN1);
volatile uint32_t *R3 = (uint32_t *)GPIOE + (_PIN4);
volatile uint32_t *R4 = (uint32_t *)GPIOE + (_PIN5);
volatile uint32_t *C1 = (uint32_t *)GPIOB + (_PIN4);
volatile uint32_t *C2 = (uint32_t *)GPIOA + (_PIN5);
volatile uint32_t *C3 = (uint32_t *)GPIOA + (_PIN6);
volatile uint32_t *C4 = (uint32_t *)GPIOA + (_PIN7);

char ReadKeypad()
{
    int     row = 0;
    char    key = 0x00;
		
    // Code for Scanning Keypad
		while(row < 4 && key == 0x00){
			*R1 = 0xFF;
			*R2 = 0xFF;
			*R3 = 0xFF;
			*R4 = 0xFF;
		
		if(row == 0){
			*R1 = 0;
		}
		else if(row ==1){
			*R2 =0;
		}
		else if(row ==2){
			*R3 = 0;
		}
		else if(row ==3){
			*R4 = 0;
		}
		ezDelayUs(10);
		
		if(*C1 == 0){
			key = Keypad[row][0];
		}
		else if(*C2 == 0){
			key = Keypad[row][1];
		}
		else if(*C3 == 0){
			key = Keypad[row][2];
		}
		else if(*C4 == 0){
			key = Keypad[row][3];
		}
		if(key != 0x00){
			break;
		}
		row ++;
	}
		
		static int preKey = 0;
		if(key != preKey){
			preKey = key;
		}
		else{
			key=0;
		}
		
    return key;
}

void Setup_GPIO()
{
    // GPIO Initialization and Configuration
    // 1. Enable Clock to the GPIO Modules (SYSCTL->RCGCGPIO |= (_PORTs);)
    SYSCTL->RCGCGPIO |= (_PORTE | _PORTD | _PORTB | _PORTA);
    // allow time for clock to stabilize (SYSCTL->PRGPIO & (_PORTs))
    while ((SYSCTL->PRGPIO & (_PORTE | _PORTD | _PORTB | _PORTA)) != (_PORTE | _PORTD | _PORTB | _PORTA)) {};
    // 2. Unlock GPIO only PD7, PF0 on TM4C123G; PD7, PE7 on TM4C1294 (GPIOx->LOCK = 0x4C4F434B; and GPIOx->CR = _PINs;)
    // 3. Set Analog Mode Select bits for each Port (GPIOx->AMSEL = _PINs;  0=digital, 1=analog)
    // 4. Set Port Control Register for each Port (GPIOx->PCTL = PMCn << _PTCL_PINn, check the PCTL table)
    // 5. Set Alternate Function Select bits for each Port (GPIOx->AFSEL = _PINs;  0=regular I/O, 1=PCTL peripheral)
    // 6. Set Output pins for each Port (Direction of the Pins: GPIOx->DIR = _PINs; 0=input, 1=output)

		GPIOE ->DIR = _PIN2 | _PIN3 |_PIN1 | _PIN4 | _PIN5;
		GPIOD ->DIR = _PIN0 | _PIN1 | _PIN2 | _PIN3;
		GPIOB ->DIR = _PIN0 | _PIN1;
    // 7. Set PUR bits (internal Pull-Up Resistor), PDR (Pull-Down Resistor), ODR (Open Drain) for each Port (0: disable, 1=enable)
		GPIOB ->ODR = _PIN0 | _PIN1;
		GPIOE ->ODR = _PIN4 | _PIN5;
		GPIOB ->PUR = _PIN4;
		GPIOA ->PUR = _PIN5 | _PIN6 |_PIN7;
    // 8. Set Digital ENable register on all port.pins (GPIOx->DEN = _PINs; 0=disable, 1=enable)
		GPIOE ->DEN = _PIN2 | _PIN3 |_PIN1 | _PIN4 | _PIN5;
		GPIOD ->DEN = _PIN0 | _PIN1 | _PIN2 | _PIN3;
		GPIOB ->DEN = _PIN0 | _PIN1 | _PIN4;
		GPIOA ->DEN = _PIN5 | _PIN6 |_PIN7;
			
			
}


