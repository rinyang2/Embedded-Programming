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
INT8U counter = 0;
INT8U anticounter =0;
INT8U total_price = 0;

INT8U btn_Pushed;//if button is pushed, digit is set to 1, otherwise it is set to 0


/*****************************   Functions   *******************************/

INT16U driver()
/*****************************************************************************
*   Input    :
*   Output   :
*   Function :
******************************************************************************/
{
   int temp=0;
    LAST_AB = (GPIO_PORTA_DATA_R & 0x60)/0x20;
    while (1) {

        A = (GPIO_PORTA_DATA_R & 0x20)/0x20;//read the current state of input A
        B = (GPIO_PORTA_DATA_R & 0x40)/0x40;//read the current state of input B
        AB = (GPIO_PORTA_DATA_R & 0x60)/0x20;

        if(AB!=LAST_AB){

            YY=AB^LAST_AB;

            if(A==B){
                if(YY==1){
                    anticounter++;
                }
                else if(YY==2){
                    counter++;
                }
            }

            else{
                if(YY==1){
                    counter++;
                 }
                else if(YY==2){
                    anticounter++;
                }
            }
            gfprintf(COM2, "%c%cCREDIT:%5uDKK", 0x1B, 0xA8,temp=(counter*50-anticounter*5));
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
        return temp/10;
        }



        vTaskDelay(5 / portTICK_RATE_MS);
    }
}


/*****************************************************************************

*****************************************************************************/

/****************************** End Of Module *******************************/












