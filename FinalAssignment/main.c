/*****************************************************************************
* University of Southern Denmark
* Embedded Programming (EMP)
*
* MODULENAME.: main.c
*
* PROJECT....: Assignment7_FreeRTOS
*
* DESCRIPTION: Assignment 7, main module. No main.h file.
*
* Change Log:
*****************************************************************************
* Date    Id    Change
* DDMMYY
* --------------------
* 060420  AMS   Module created.
*
*****************************************************************************/

/***************************** Include files *******************************/
#include <stdint.h>
#include "tm4c123gh6pm.h"
#include "emp_type.h"

#include "digiswitch.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
//#include "systick_frt.h"
//#include "semtest.h"

#include "status_led.h"
#include "adc.h"
#include "gpio.h"
#include "lcd.h"
#include "key.h"

#include "file.h"

/*****************************    Defines    *******************************/
#define USERTASK_STACK_SIZE configMINIMAL_STACK_SIZE
#define IDLE_PRIO 0
#define LOW_PRIO  1
#define MED_PRIO  2
#define HIGH_PRIO 3

/*****************************   Constants   *******************************/

/*****************************   Variables   *******************************/
//static SemaphoreHandle_t xMutex_Scale_LCD = NULL;

/*****************************   Functions   *******************************/


static void setupHardware(void)
/*****************************************************************************
*   Input    :  -
*   Output   :  -
*   Function :  Hardware configuration and initialization
*****************************************************************************/
{
    status_led_init();
    init_adc();
    init_gpio();

    set_scalefactor(1);
    set_offset(10);
}

static void initializeSemaphores(void)
/*****************************************************************************
*   Input    :  -
*   Output   :  -
*   Function :  Initialize the Semaphore handlers.
*****************************************************************************/
{
    init_adc_semaphores();
    //xMutex_Scale_LCD = xSemaphoreCreateMutex();
    //xSemaphoreGive(xMutex_Scale_LCD);
    lcd_init_queue();
    key_init_queue();
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
	initializeSemaphores();
	init_files();

	// Create the tasks.
	xTaskCreate( status_led_task, "Status_led", USERTASK_STACK_SIZE, NULL, LOW_PRIO, NULL );
	//xTaskCreate( ai_task, "Analog_input", USERTASK_STACK_SIZE, NULL, LOW_PRIO, NULL );
	xTaskCreate( lcd_task, "LCD", USERTASK_STACK_SIZE, NULL, LOW_PRIO, NULL ); //(void *) xMutex_Scale_LCD
	xTaskCreate( scale_task, "Scale", USERTASK_STACK_SIZE, NULL, LOW_PRIO, NULL ); //(void *) xMutex_Scale_LCD
	xTaskCreate( keyboard_task, "Keyboard", USERTASK_STACK_SIZE, NULL, LOW_PRIO, NULL );
	xTaskCreate( ui_task, "UI", USERTASK_STACK_SIZE, NULL, LOW_PRIO, NULL );
	xTaskCreate( driver, "Digiswitch", USERTASK_STACK_SIZE, NULL, LOW_PRIO, NULL );

	// Start the scheduler.
	vTaskStartScheduler();

	// Will only get here, if there was insufficient memory.
    return 1;
}


/****************************** End Of Module *******************************/
