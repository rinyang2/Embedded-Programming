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
#include "button.h"
#include "devices.h"
#include "leds.h"

#include "FreeRTOS.h"
#include "Task.h"
#include "queue.h"
#include "semphr.h"
#include "uart.h"
/*****************************    Defines    *******************************/

/*****************************   Constants   *******************************/


/*****************************   Variables   *******************************/

INT8U btn_State;
INT8U btn_Pushed;//if button is pushed, digit is set to 1, otherwise it is set to 0


/*****************************   Functions   *******************************/

void gas_pump(INT16U price){
    INT32U temp = 0;
    INT16U left = 0;
    INT16U right = 0;
    INT16U liter = 0;
    INT8U clock = 0;
    INT8U timer = 0;
    INT8U indicator=0;
    while(1){
        if(lever1()==1||lever1()==3||timer==5){
                    gfprintf(COM2, "%c%cDELIVER FINISHED", 0x1B, 0x80);
                    set_leds( TURN_LED_ON, TURN_LED_OFF, TURN_LED_OFF ); //turn red led on
                    vTaskDelay(2000 / portTICK_RATE_MS);
                    gfprintf(COM2, "%c%c                ", 0x1B, 0xA8);
                    return 0;
        }
        left = temp/10000;
        right = temp%10000;

        gfprintf(COM2, "%c%c8.%2uDKK/L%3u.%2uL", 0x1B, 0x80,price%100,liter/100, liter%100);
        gfprintf(COM2, "%c%c    %4u.%4uDKK", 0x1B, 0xA8,left,right);
        btn_State = lever2();

        if (btn_State == 1 || btn_State == 3) {
            btn_Pushed = 1;
        }
        else {
            btn_Pushed = 0;
        }


        if(btn_Pushed){
            if(clock<2){
                liter +=5;
                temp += (price*5);
                set_leds( TURN_LED_OFF, TURN_LED_ON, TURN_LED_OFF ); //turn yellow led on
            }
            else if(clock >=2){
                liter += 30;
                temp += (price*30);
                set_leds( TURN_LED_OFF, TURN_LED_OFF, TURN_LED_ON ); //turn green led on
            }
            clock++;
            indicator=1;
            timer = 0;
        }
        else{
            if(indicator == 1){
                liter +=5;
                temp += (price*5);
                set_leds( TURN_LED_OFF, TURN_LED_ON, TURN_LED_OFF ); //turn yellow led on
            }
            indicator = 0;
            clock = 0;
            set_leds( TURN_LED_ON, TURN_LED_OFF, TURN_LED_OFF ); //turn red led on
        }
        timer++;
        vTaskDelay(1000 / portTICK_RATE_MS);
    }
}

INT8U cash_gas_pump(INT16U price, INT16U cash){
    INT32U temp = 0;

    INT16U left = 0;
    INT16U right = 0;
    INT16U liter = 0;
    INT8U clock = 0;
    INT8U indicator=0;
    while(1){
        left = temp/10000;
        right = temp%10000;
        if(cash<left){
            gfprintf(COM2, "%c%cDELIVER FINISHED", 0x1B, 0x80);
            set_leds( TURN_LED_ON, TURN_LED_OFF, TURN_LED_OFF ); //turn red led on
            vTaskDelay(1000 / portTICK_RATE_MS);
            gfprintf(COM2, "%c%c                ", 0x1B, 0xA8);
            return 0;
        }
        else if(lever1()==1||lever1()==3){
            gfprintf(COM2, "%c%cDELIVER FINISHED", 0x1B, 0x80);
            set_leds( TURN_LED_ON, TURN_LED_OFF, TURN_LED_OFF ); //turn red led on
            vTaskDelay(1000 / portTICK_RATE_MS);
            gfprintf(COM2, "%c%c                ", 0x1B, 0xA8);
            return 0;
        }
        gfprintf(COM2, "%c%c8.%2uDKK/L%3u.%2uL", 0x1B, 0x80,price%100,liter/100, liter%100);
        gfprintf(COM2, "%c%c    %4u.%4uDKK", 0x1B, 0xA8,left,right);


        btn_State = lever2();

        if (btn_State == 1 || btn_State == 3) {
            btn_Pushed = 1;
        }
        else {
            btn_Pushed = 0;
        }


        if(btn_Pushed){
                   if(clock<2){
                       liter +=5;
                       temp += (price*5);
                       set_leds( TURN_LED_OFF, TURN_LED_ON, TURN_LED_OFF ); //turn yellow led on
                   }
                   else if(clock >=2){
                       liter += 30;
                       temp += (price*30);
                       set_leds( TURN_LED_OFF, TURN_LED_OFF, TURN_LED_ON ); //turn green led on
                   }
                   clock++;
                   indicator=1;
               }
               else{
                   if(indicator == 1){
                       liter +=5;
                       temp += (price*5);
                       set_leds( TURN_LED_OFF, TURN_LED_ON, TURN_LED_OFF ); //turn yellow led on
                   }
                   indicator = 0;
                   clock = 0;
                   set_leds( TURN_LED_ON, TURN_LED_OFF, TURN_LED_OFF ); //turn red led on
               }


        vTaskDelay(500 / portTICK_RATE_MS);
    }
}
