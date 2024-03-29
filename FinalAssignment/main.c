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

#include "button.h"
#include "uart.h"


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
    uart0_init( 9600, 8, 1, 'n' );

    init_files();

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
  Q_UART_TX = xQueueCreate( 128, sizeof(INT8U) );
  Q_UART_RX = xQueueCreate( 128, sizeof(INT8U) );
  // Start the tasks.
  xTaskCreate( key_task,        "key",  USERTASK_STACK_SIZE, NULL, LOW_PRIO, NULL );    // the tasks are created. all priorities are the same or else only the higher priorities will run if ready
  xTaskCreate( lcd_task,        "lcd",  USERTASK_STACK_SIZE, NULL, LOW_PRIO, NULL );
  xTaskCreate( ui_task,         "ui",   USERTASK_STACK_SIZE, NULL, LOW_PRIO, NULL );

 // xTaskCreate( uart_task,       "ut",   USERTASK_STACK_SIZE, NULL, LOW_PRIO, NULL );
  //xTaskCreate( uart_rx_task,    "ur",   USERTASK_STACK_SIZE, NULL, LOW_PRIO, NULL );
  // ----------------

  // Start the scheduler.
  // --------------------
  vTaskStartScheduler();

  // Will only get here, if there was insufficient memory.
  // -----------------------------------------------------
  return 1;
}

/****************************** End Of Module *******************************/
