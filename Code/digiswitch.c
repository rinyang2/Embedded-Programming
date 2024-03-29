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
INT8U A;
INT8U LAST_A;
INT8U B;
INT8U LAST_B;
INT8U AB;
INT8U LAST_AB;
INT8U YY;
INT8U btn_State;
INT8U current_Dir=1;//CCW or CW
INT8U CCW = 0;//can be used later as an indicator for the driver
INT8U CW = 1;//can be used later as an indicator for the driver
INT8U counter = 120;

INT8U btn_Pushed;//if button is pushed, digit is set to 1, otherwise it is set to 0


/*****************************   Functions   *******************************/

void driver()
/*****************************************************************************
*   Input    :
*   Output   :
*   Function :
******************************************************************************/
{
    //LAST_A=(GPIO_PORTA_DATA_R & 0x20)/0x20;
    LAST_AB = (GPIO_PORTA_DATA_R & 0x60)/0x20;
    while (1) {

        A = (GPIO_PORTA_DATA_R & 0x20)/0x20;//read the current state of input A
        B = (GPIO_PORTA_DATA_R & 0x40)/0x40;//read the current state of input B
        AB = (GPIO_PORTA_DATA_R & 0x60)/0x20;

        if(AB!=LAST_AB){

            YY=AB^LAST_AB;

            if(A==B){
                if(YY==1){
                    counter--;
                    current_Dir=CCW;
                }
                else if(YY==2){
                    counter++;
                    current_Dir=CW;
                }
            }

            else{
                if(YY==1){
                    counter++;
                    current_Dir=CW;
                 }
                else if(YY==2){
                    counter--;
                    current_Dir=CCW;
                }
            }
            if(current_Dir){
                gfprintf( COM2, "%c%cANGLE:%03u CW", 0x1B, 0x84, (counter*12)%360);
            }
            else{
                gfprintf( COM2, "%c%cANGLE:%03uCCW", 0x1B, 0x84, (counter*12)%360);
            }
            LAST_AB=AB;
        }



        btn_State = GPIO_PORTA_DATA_R & 0x80;

        if (btn_State == 0) {
            btn_Pushed = 1;
        }
        else {
            btn_Pushed = 0;
        }


        if(btn_Pushed){
        gfprintf( COM2, "%c%cPRESSED", 0x1B, 0xC4, btn_Pushed );
        }
        else{
        gfprintf( COM2, "%c%cUNPRESS", 0x1B, 0xC4, btn_Pushed );
        }


        vTaskDelay(5 / portTICK_RATE_MS);
    }
}


/*****************************************************************************

*****************************************************************************/

/****************************** End Of Module *******************************/












