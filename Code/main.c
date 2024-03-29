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
    lcd_init_queue();

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

	xTaskCreate( lcd_task, "LCD", USERTASK_STACK_SIZE, NULL, LOW_PRIO, NULL ); //(void *) xMutex_Scale_LCD
    xTaskCreate( driver, "Digiswitch", USERTASK_STACK_SIZE, NULL, LOW_PRIO, NULL ); //(void *) xMutex_Scale_LCD
	// Start the scheduler.
	vTaskStartScheduler();

	// Will only get here, if there was insufficient memory.
    return 1;
}


/****************************** End Of Module *******************************/
