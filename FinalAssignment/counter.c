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
/*****************************    Defines    *******************************/
// Counter States
// --------------

/*****************************   Constants   *******************************/

/*****************************   Variables   *******************************/

/*****************************   Functions   *******************************/


INT8U counter( INT8U payment )
/*****************************************************************************
*   Input    :payment type
*   Output   :total value of gasoline
*   Function :calculate the amount of gas pumped
******************************************************************************/
{
  static INT8U  counter_value = 0;
  static INT16U counter_timer = 0;
  INT8U count;

  if( payment == 1)
  {
	    counter_value+=0.05;
	    counter_timer = TIM_1_SEC;
		counter_value+=0.05;
	    counter_timer = TIM_1_SEC;
		while (1){
			counter_value+=0.3;
	   		counter_timer = TIM_1_SEC;
			if (button_pushed()){//not button_pushed but lever released
				for(count=0;count<5;count++){
				counter_value+=0.3;
	   			counter_timer = TIM_1_SEC;
				}
				return 0;
			}
		}
    }else if ( payment == 2){
	    counter_value+=0.05;
	    counter_timer = TIM_1_SEC;
		counter_value+=0.05;
	    counter_timer = TIM_1_SEC;
		while (1){
			counter_value+=0.3;
	   		counter_timer = TIM_1_SEC;
			if (button_pushed()){//not button_pushed but lever released
				for(count=0;count<5;count++){
				counter_value+=0.3;
	   			counter_timer = TIM_1_SEC;
				}
				return 0;
			}
	}
  }
  return( counter_value );
}


/****************************** End Of Module *******************************/












