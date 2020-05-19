/*****************************************************************************
* University of Southern Denmark
* Embedded C Programming (ECP)
*
* MODULENAME.: counter.c
*
* PROJECT....: EMP
*
* DESCRIPTION: See module specification file (.h-file).
*
* Change Log:
******************************************************************************
* Date    Id    Change
* YYMMDD
* --------------------
* 090215  MoH   Module created.
*
*****************************************************************************/

/***************************** Include files *******************************/
#include <stdint.h>
#include "tm4c123gh6pm.h"
#include "emp_type.h"
#include "events.h"
#include "swtimers.h"
#include "file.h"
/*****************************    Defines    *******************************/
// Counter States
// --------------
#define RED    1
#define YELLOW 2
#define GREEN  4

/*****************************   Constants   *******************************/

/*****************************   Variables   *******************************/

/*****************************   Functions   *******************************/


INT8U counter( INT8U payment,  INT8U price)
/*****************************************************************************
*   Input    :payment type
*   Output   :total value of gasoline
*   Function :calculate the amount of gas pumped
******************************************************************************/
{
  static INT8U  counter_value = 0;
  static INT16U counter_timer = 0;
  INT8U count;
  INT8U led_status;

  if( payment == 1)
  {
	    counter_value+=0.05;
	    counter_timer = TIM_1_SEC;
		led_status = YELLOW;
		counter_leds( led_status );
		gfprintf( COM2, "%c%cTOTAL:%03u", 0x1B, 0x84, (counter_value * price));
		counter_value+=0.05;
	    counter_timer = TIM_1_SEC;
		led_status = YELLOW;
		counter_leds( led_status );
		gfprintf( COM2, "%c%cTOTAL:%03u", 0x1B, 0x84, (counter_value * price));
		while (1){
			counter_value+=0.3;
	   		counter_timer = TIM_1_SEC;
			led_status = GREEN;
			counter_leds( led_status );
			gfprintf( COM2, "%c%cTOTAL:%03u", 0x1B, 0x84, (counter_value * price));
			if (lever() == 0){//lever released (idle state)
				for(count=0;count<5;count++){
				counter_value+=0.3;
	   			counter_timer = TIM_1_SEC;
				led_status = GREEN;
				counter_leds( led_status );
				gfprintf( COM2, "%c%cTOTAL:%03u", 0x1B, 0x84, (counter_value * price));
				}
				return 0;
			}
		}
    }else if ( payment == 2){
	    counter_value+=0.05;
	    counter_timer = TIM_1_SEC;
		led_status = YELLOW;
		counter_leds( led_status );
		gfprintf( COM2, "%c%cTOTAL:%03u", 0x1B, 0x84, (counter_value * price));
		counter_value+=0.05;
	    counter_timer = TIM_1_SEC;
		led_status = YELLOW;
		counter_leds( led_status );
		gfprintf( COM2, "%c%cTOTAL:%03u", 0x1B, 0x84, (counter_value * price));
		while (1){
			counter_value+=0.3;
	   		counter_timer = TIM_1_SEC;
			led_status = GREEN;
			counter_leds( led_status );
			gfprintf( COM2, "%c%cTOTAL:%03u", 0x1B, 0x84, (counter_value * price));
			if (lever() == 0){//lever released (idle state)
				return 0;
			}
			if (1){//condition for the second case
				counter_value+=0.05;
				led_status = YELLOW;
				counter_leds( led_status );
				gfprintf( COM2, "%c%cTOTAL:%03u", 0x1B, 0x84, (counter_value * price));
			}
	}
  }
  return( counter_value );
}


/****************************** End Of Module *******************************/












