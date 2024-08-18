# Embedded Systems Project

## [Lab 1 - LED Light Bar Simulation](Lab_1)
This lab uses the TM4C123G microcontroller to control two sets of 3 LEDs where one set emits red while one emits blue. Three patterns were developed so that the LEDs would turn on and off based on the pattern. A structure will be used to store the patterns and the possibility of editing them in the future. A switch is also used to switch between different patterns or when looking at the code looping through the structure array.

[-> Source Code](Lab_1/main.c)

## [Lab 2 - Home control System](Lab_2)
This lab aims to implement a typical home control system using the TM4C123G microcontroller. For example, a switch connected to a buzzer will ring if the switch is pressed. And where an LED is controlled by another switch with which, if the button is pressed, the light will turn on. In the final case, two switches are connected to a light, and either switch can turn the light on or off.

[-> Source Code](Lab_2/main.c)

## [Lab 3 - 4 Digit Seven - Segment Display](Lab_3)
The purpose of the experiment was to be able to control a 4-digit segment display with the Tiva C series microcontroller to display a clock starting at 00:00 and ending at 59:59. The 4-digit segment display will be controlled with two switches where one will stop the clock and the other will reset it to 00:00. The program will check the states of the switches every 20 ms. Every 5 ms, one digit is displayed in the segment. The decimal point is toggled every 500 ms. Finally, every 1000 ms, the second digit is increased by 1.

[-> Source Code](Lab_3/main.c)

## [Lab 5 - Scan a Key from a Matrix Keypad](Lab_3)
Lab 5 used a microcontroller to read from a 4x4 matrix keypad and display the selection through LCD. For example, if the user presses the number “5” from the matrix, then the number 5 is displayed on the LCD.

[-> Source Code](Lab_5/main.c)