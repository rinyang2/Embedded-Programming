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


/*****************************    Defines    *******************************/
#define USERTASK_STACK_SIZE configMINIMAL_STACK_SIZE
#define IDLE_PRIO 0
#define LOW_PRIO  1
#define MED_PRIO  2
#define HIGH_PRIO 3


/*****************************   Constants   *******************************/

/*****************************   Variables   *******************************/

/*****************************   Functions   *******************************/



static void setupHardware(void)
/*****************************************************************************
*   Input    :  -
*   Output   :  -
*   Function :
*****************************************************************************/
{
  // TODO: Put hardware configuration and initialisation in here

  // Warning: If you do not initialize the hardware clock, the timings will be inaccurate
    init_systick();
    init_gpio();
    init_adc();
    init_files();
}

int void case_int()
{
	INT8U key_input = ;//this is supposed to read the keypad input and return a number

	switch(key_input){
		case 1://1 is CARD
			CARD_PIN = ;//here should exists a string read from the keypad, which will be later used to verify the CAR NUMBER AND PIN; this number should be a 12-digit number, where the 8 and 12 digit is compared
			CARD_NUM = CARD_PIN >> 16;
			if (0x0001 & CARD_PIN == 0x0001 & CARD_NUM){
				//display the gas prices
				INT8U gas_choice = ;//read from keypad
				INT8U gas_price = 0;
				switch(gas_choice){
					case 1://#92
						gas_price = 8.49;
						break;
					case 2://#95
						gas_price = 8.79;
						break;
					case 3://DIESEL
						gas_price = 8.12;
						break;
				}
			}
			break;
		case 2:// 2 is CASH
			//read digiswitch
			//display the gas prices
				INT8U gas_choice = ;//read from keypad
				INT8U gas_price = 0;
				switch(gas_choice){
					case 1://#92
						gas_price = 8.49;
						break;
					case 2://#95
						gas_price = 8.79;
						break;
					case 3://DIESEL
						gas_price = 8.12;
						break;
				}
			}
			break;
	}
}

int main(void)
/*****************************************************************************
*   Input    :
*   Output   :
*   Function : The super loop.
******************************************************************************/
{
  setupHardware();

  xMutex = xSemaphoreCreateMutex();                                                     // create the mutex and the queues. make sure the handles are defined globally (in glob_def.h for example)

  Q_KEY = xQueueCreate( 128,  sizeof(INT8U) );
  Q_LCD = xQueueCreate( 128, sizeof(INT8U) );

  // Start the tasks.
  xTaskCreate( key_task,        "key",  USERTASK_STACK_SIZE, NULL, LOW_PRIO, NULL );    // the tasks are created. all priorities are the same or else only the higher priorities will run if ready
  xTaskCreate( lcd_task,        "lcd",  USERTASK_STACK_SIZE, NULL, LOW_PRIO, NULL );
  xTaskCreate( ai_task,         "ai",   USERTASK_STACK_SIZE, NULL, LOW_PRIO, NULL );
  xTaskCreate( scale_task,      "scale",USERTASK_STACK_SIZE, NULL, LOW_PRIO, NULL );
  xTaskCreate( ui_task,         "ui",   USERTASK_STACK_SIZE, NULL, LOW_PRIO, NULL );

  // ----------------

  // Start the scheduler.
  // --------------------
  vTaskStartScheduler();

  // Will only get here, if there was insufficient memory.
  // -----------------------------------------------------
  return 1;
}

/****************************** End Of Module *******************************/
