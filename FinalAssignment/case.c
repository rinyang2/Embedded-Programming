/*****************************************************************************
* University of Southern Denmark
* Embedded Programming (EMP)
*
* MODULENAME.: main.c
*
* PROJECT....: EMP
*
* DESCRIPTION: Assignment 2, main module. No main.h file.
*
* Change Log:
*****************************************************************************
* Date    Id    Change
* YYMMDD
* --------------------
* 150215  MoH   Module created.
*
*****************************************************************************/

/***************************** Include files *******************************/
#include <stdint.h>
#include "tm4c123gh6pm.h"
#include "emp_type.h"
#include "systick_frt.h"
#include "FreeRTOS.h"
#include "task.h"
#include "key.h"
#include "gpio.h"
#include "ui.h"
#include "lcd.h"
#include "queue.h"
#include "glob_def.h"
#include "semphr.h"
#include "scale.h"
#include "analog.h"
#include "button.h"
#include "file.h"
/*****************************    Defines    *******************************/
#define CARD 1
#define CASH 2
#define LEADFREE92  1
#define LEADFREE95  2
#define DIESEL 3

INT8U key_input;
INT8U gas_choice;
INT8U gas_price;

void case_int()
{
	key_input = get_keyboad();//this is supposed to read the keypad input and return a number

	switch(key_input){
		case CARD://1 is CARD
			CARD_PIN = ;//here should exists a string read from the keypad, which will be later used to verify the CAR NUMBER AND PIN; this number should be a 12-digit number, where the 8 and 12 digit is compared
			CARD_NUM = CARD_PIN >> 16;
			if (0x0001 & CARD_PIN != 0x0001 & CARD_NUM){
				gfprintf(COM2, "%c%cGAS TYPE:",0x1B, 0x80); //display the gas prices
				gfprintf(COM2, "%c%c1: #92 = 8.49; 2: #95 = 8.79; 3: DIESEL = 8.12",0x1B, 0x80);
				gas_choice = get_keyboard();//read from keypad
				gas_price = 0;
				switch(gas_choice){
					case LEADFREE92://#92
						gas_price = 8.49;
						break;
					case LEADFREE95://#95
						gas_price = 8.79;
						break;
					case DIESEL://DIESEL
						gas_price = 8.12;
						break;
				}
			}
			break;
		case CASH:// 2 is CASH
			digi_price();//read digiswitch
            gfprintf(COM2, "%c%cGAS TYPE:",0x1B, 0x80); //display the gas prices
            gfprintf(COM2, "%c%c1: #92 = 8.49; 2: #95 = 8.79; 3: DIESEL = 8.12",0x1B, 0x80);
			gas_choice = get_keyboard();//read from keypad
			gas_price = 0;
			switch(gas_choice){
				case LEADFREE92://#92
					gas_price = 8.49;
					break;
				case LEADFREE95://#95
					gas_price = 8.79;
					break;
				case DIESEL://DIESEL
					gas_price = 8.12;
					break;
			}
		break;
	}
}

void gas_pump()
{
	case_int();
    if(key_input == CARD){
        if( button_pushed()){
            //print out information
			counter_leds( 1 );//RED LIGHT
            if (lever() == 4){
                counter(key_input, gas_price);
            }
        }
    }else if(key_input == CASH){
        if( button_pushed()){
            //print out information
			counter_leds( 1 );//RED LIGHT
            if (lever() == 4){
                counter(key_input, gas_price);
            }
        }
    }
	
}
