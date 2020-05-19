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


    while(1)
    {
        INT8U key = 0;
        gfprintf(COM2, "%c%cENTER CARD NUM", 0x1B, 0x80);
        switch(ui_state)
        {
        case 0:
            key = get_keyboard();
            if( key >= '0' && key <= '9')
            {
                gfprintf(COM2, "%c%c%c", 0x1B, 0xC0+ui_state, key);
                ui_state = 1;
            }
            break;
        case 1:
            key = get_keyboard();
            if( key >= '0' && key <= '9')
            {
                gfprintf(COM2, "%c%c%c", 0x1B, 0xC0+ui_state, key);
                ui_state = 2;
            }
            break;
        case 2:
            key = get_keyboard();
            if( key >= '0' && key <= '9')
            {
                gfprintf(COM2, "%c%c%c", 0x1B, 0xC0+ui_state, key);
                ui_state = 3;
            }
            break;
        case 3:
            key = get_keyboard();
            if( key >= '0' && key <= '9')
            {
                gfprintf(COM2, "%c%c%c", 0x1B, 0xC0+ui_state, key);
                ui_state = 4;
            }
            break;
        case 4:
            key = get_keyboard();
            if( key >= '0' && key <= '9')
            {
                gfprintf(COM2, "%c%c%c", 0x1B, 0xC0+ui_state, key);
                ui_state = 5;
            }
            break;
        case 5:
            key = get_keyboard();
            if( key >= '0' && key <= '9')
            {
                gfprintf(COM2, "%c%c%c", 0x1B, 0xC0+ui_state, key);
                ui_state = 6;
            }
            break;
        case 6:
            key = get_keyboard();
            if( key >= '0' && key <= '9')
            {
                gfprintf(COM2, "%c%c%c", 0x1B, 0xC0+ui_state, key);
                ui_state = 7;
            }
            break;
        case 7:
            key = get_keyboard();
            if( key >= '0' && key <= '9')
            {
                gfprintf(COM2, "%c%c%c", 0x1B, 0xC0+ui_state, key);
                ui_state = 8;
            }
            return key-'0';
        }
    }
};

INT16U read_pin()
{
    INT8U ui_state = 0;


    while(1)
    {
        INT8U key = 0;
        gfprintf(COM2, "%c%cENTER CARD NUM", 0x1B, 0x80);
        switch(ui_state)
        {
        case 0:
            key = get_keyboard();
            if( key >= '0' && key <= '9')
            {
                gfprintf(COM2, "%c%c%c", 0x1B, 0xC0+ui_state, key);
                ui_state = 1;
            }
            break;
        case 1:
            key = get_keyboard();
            if( key >= '0' && key <= '9')
            {
                gfprintf(COM2, "%c%c%c", 0x1B, 0xC0+ui_state, key);
                ui_state = 2;
            }
            break;
        case 2:
            key = get_keyboard();
            if( key >= '0' && key <= '9')
            {
                gfprintf(COM2, "%c%c%c", 0x1B, 0xC0+ui_state, key);
                ui_state = 3;
            }
            break;
        case 3:
            key = get_keyboard();
            if( key >= '0' && key <= '9')
            {
                gfprintf(COM2, "%c%c%c", 0x1B, 0xC0+ui_state, key);
            }
            return key-'0';
        }
    }
};


/*****************************************************************************

*****************************************************************************/

/****************************** End Of Module *******************************/








