/*****************************************************************************
* University of Southern Denmark
* Embedded Programming (EMP)
*
* MODULENAME.: gas_select.c
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

/*****************************   Constants   *******************************/

/*****************************   Variables   *******************************/


/*****************************   Functions   *******************************/

INT16U gas_select()
/*****************************************************************************
*   Input    :
*   Output   :
*   Function :
******************************************************************************/
{
    gfprintf(COM2, "%c%cSELECT GAS TYPE", 0x1B, 0x80);
    gfprintf(COM2, "%c%c                ", 0x1B, 0xA8);
    vTaskDelay(2000 / portTICK_RATE_MS);
    while(1)
    {
        INT8U key = 0;
        INT8U temp = 0;
        key = get_keyboard();
        gfprintf(COM2, "%c%cLF92: 1  LF95: 2", 0x1B, 0x80);
        gfprintf(COM2, "%c%cDIESEL: 3    ", 0x1B, 0xA8);

        temp = key - '0';

        if(temp==1){
            gfprintf(COM2, "%c%cLeadfree 92     ", 0x1B, 0x80);
                gfprintf(COM2, "%c%c                ", 0x1B, 0xA8);
                vTaskDelay(2000 / portTICK_RATE_MS);
            return 849;
        }
        else if(temp==2){
            gfprintf(COM2, "%c%cLeadfree 95     ", 0x1B, 0x80);
                gfprintf(COM2, "%c%c                ", 0x1B, 0xA8);
                vTaskDelay(2000 / portTICK_RATE_MS);
            return 879;
        }
        else if(temp==3){
            gfprintf(COM2, "%c%cDiesel          ", 0x1B, 0x80);
                gfprintf(COM2, "%c%c                ", 0x1B, 0xA8);
                vTaskDelay(2000 / portTICK_RATE_MS);
            return 812;
        }
    }
};


/*****************************************************************************

*****************************************************************************/

/****************************** End Of Module *******************************/
