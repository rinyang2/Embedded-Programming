/*****************************************************************************
* University of Southern Denmark
* Embedded Programming (EMP)
*
* MODULENAME.: digiswitch.c
*
* PROJECT....: EMP
*
* DESCRIPTION: See module specification file (.h-file).
*
* Change Log:
*****************************************************************************
* Date    Id    Change
* YYMMDD
* --------------------
* 150322  MoH   Module created.
*
*****************************************************************************/

/***************************** Include files *******************************/
#include <stdint.h>
#include "tm4c123gh6pm.h"
#include "emp_type.h"
#include "lcd.h"
#include "glob_def.h"
#include "tmodel.h"
//#include "queue.h"
//#include "sem.h"

#include "adc.h"
#include "file.h"
#include "string.h"

#include "FreeRTOS.h"
#include "Task.h"
#include "queue.h"
#include "semphr.h"

/*****************************    Defines    *******************************/
#define LOW 0
#define HIGH 1
/*****************************   Constants   *******************************/

/*****************************   Variables   *******************************/


/*****************************   Functions   *******************************/

INT8U read_card()
/*****************************************************************************
*   Input    :
*   Output   :
*   Function :
******************************************************************************/
{
    INT8U ui_state = 0;
    INT8U valid=0;

    while(1)
    {
        INT8U key = 0;
        gfprintf(COM2, "%c%cCARD     PIN ", 0x1B, 0x80);


        switch(ui_state)
        {
        case 0:
            key = get_keyboard();
            if( key >= '0' && key <= '9')
            {
                gfprintf(COM2, "%c%c%c", 0x1B, 0xC0+ui_state, key);
                ui_state = 1;
                uart0_putc( key );
            }
            break;
        case 1:
            key = get_keyboard();
            if( key >= '0' && key <= '9')
            {
                gfprintf(COM2, "%c%c%c", 0x1B, 0xC0+ui_state, key);
                ui_state = 2;
                uart0_putc( key );
            }
            break;
        case 2:
            key = get_keyboard();
            if( key >= '0' && key <= '9')
            {
                gfprintf(COM2, "%c%c%c", 0x1B, 0xC0+ui_state, key);
                ui_state = 3;
                uart0_putc( key );
            }
            break;
        case 3:
            key = get_keyboard();
            if( key >= '0' && key <= '9')
            {
                gfprintf(COM2, "%c%c%c", 0x1B, 0xC0+ui_state, key);
                ui_state = 4;
                uart0_putc( key );
            }
            break;
        case 4:
            key = get_keyboard();
            if( key >= '0' && key <= '9')
            {
                gfprintf(COM2, "%c%c%c", 0x1B, 0xC0+ui_state, key);
                ui_state = 5;
                uart0_putc( key );
            }
            break;
        case 5:
            key = get_keyboard();
            if( key >= '0' && key <= '9')
            {
                gfprintf(COM2, "%c%c%c", 0x1B, 0xC0+ui_state, key);
                ui_state = 6;
                uart0_putc( key );
            }
            break;
        case 6:
            key = get_keyboard();
            if( key >= '0' && key <= '9')
            {
                gfprintf(COM2, "%c%c%c", 0x1B, 0xC0+ui_state, key);
                ui_state = 7;
                uart0_putc( key );
            }
            break;
        case 7:
            key = get_keyboard();
            if( key >= '0' && key <= '9')
            {
                gfprintf(COM2, "%c%c%c", 0x1B, 0xC0+ui_state, key);
                ui_state = 8;
                valid = key-'0'; //last digit of card number
                uart0_putc( key );
            }
            break;


        case 8:
            key = get_keyboard();
            if( key >= '0' && key <= '9')
            {
                gfprintf(COM2, "%c%c%c", 0x1B, 0xC1+ui_state, key);
                ui_state = 9;
                uart0_putc( '*' );
            }
            break;
        case 9:
            key = get_keyboard();
            if( key >= '0' && key <= '9')
            {
                gfprintf(COM2, "%c%c%c", 0x1B, 0xC1+ui_state, key);
                ui_state = 10;
                uart0_putc( '*' );
            }
            break;
        case 10:
            key = get_keyboard();
            if( key >= '0' && key <= '9')
            {
                gfprintf(COM2, "%c%c%c", 0x1B, 0xC1+ui_state, key);
                ui_state = 11;
                uart0_putc( '*' );
            }
            break;
        case 11:
            key = get_keyboard();
            if( key >= '0' && key <= '9')
            {
                gfprintf(COM2, "%c%c%c", 0x1B, 0xC1+ui_state, key);
                ui_state = 12;
                valid += key-'0'; //last digit of pin number
                uart0_putc( '*' );
            }
            break;
        }

        if(ui_state==12){
            return valid % 2;
        }
    }
};


/*****************************************************************************

*****************************************************************************/

/****************************** End Of Module *******************************/









