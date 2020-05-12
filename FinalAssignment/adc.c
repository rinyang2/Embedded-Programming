/*****************************************************************************
* University of Southern Denmark
* Embedded C Programming (ECP)
*
* MODULENAME.: adc.c
*
* PROJECT....: Assignment7_FreeRTOS
*
* DESCRIPTION: See module specification file (.h-file).
*
* Change Log:
******************************************************************************
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

#include "FreeRTOS.h"
#include "Task.h"
#include "semphr.h"

#include "file.h"
#include "string.h"


/*****************************    Defines    *******************************/

/*****************************   Constants   *******************************/

/*****************************   Variables   *******************************/
INT16U adc_value;
static SemaphoreHandle_t xMutex_Scale_AI = NULL;

static INT16U scalefactor, offset;
static INT16U adc_adjusted_value;

/*****************************   Functions   *******************************/


INT16U get_adc()
/*****************************************************************************
*   Input    :  -
*   Output   :  -
*   Function :  Get the potentiometer's value.
*****************************************************************************/
{
  return( ADC0_SSFIFO3_R );
}


void set_scalefactor(INT16U sf)
/*****************************************************************************
*   Input    : -
*   Output   : -
*   Function : Set the scale factor's value.
******************************************************************************/
{
    scalefactor = sf;
}


INT16U get_scalefactor()
/*****************************************************************************
*   Input    : -
*   Output   : -
*   Function : Obtain the scale factor's value.
******************************************************************************/
{
    return scalefactor;
}


void set_offset(INT16U os)
/*****************************************************************************
*   Input    : -
*   Output   : -
*   Function : Set the offset's value.
******************************************************************************/
{
    offset = os;
}


INT16U get_offset()
/*****************************************************************************
*   Input    : -
*   Output   : -
*   Function : Obtain the offset's value.
******************************************************************************/
{
    return offset;
}


INT16U get_adjusted_adc()
/*****************************************************************************
*   Input    : -
*   Output   : -
*   Function : Obtain the analog input adjusted value.
******************************************************************************/
{
    return adc_adjusted_value;
}


void init_adc()
/*****************************************************************************
*   Input    :  -
*   Output   :  -
*   Function :  Initialize the potentiometer.
*****************************************************************************/
{
  SYSCTL_RCGC0_R |= SYSCTL_RCGC0_ADC0;
  SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOB;

  // Set ADC0 Sequencer priorities.
  // SS3(bit12-13): Priority = 0 ; Highest
  // SS2(bit8-9):   Priority = 1
  // SS1(bit4-5):   Priority = 2
  // SS0(bit0-1):   Priority = 3 ; Lowest
  ADC0_SSPRI_R = 0x00000123;

  //Disable all sequencers
  ADC0_ACTSS_R = 0;

  // Trigger for Sequencer 3 (bit 12-15) = 0xF = Always.
  ADC0_EMUX_R = 0x0000F000;

  //sample multiplexer input, sequencer 3 select, ADC 11 (0x0B) enable.
  ADC0_SSMUX3_R = 0x0B;

  //ADC sample sequence control 3 = END0
  ADC0_SSCTL3_R =  0x00000002;

  //enable sequencer 3
  ADC0_ACTSS_R = 0x00000008;

  // Start conversion at sequencer 3
  ADC0_PSSI_R = 0x08;
}

void init_adc_semaphores()
/*****************************************************************************
*   Input    : -
*   Output   : -
*   Function : Initialize the semaphores used in the file.
******************************************************************************/
{
    xMutex_Scale_AI = xSemaphoreCreateMutex();
    xSemaphoreGive(xMutex_Scale_AI);
}


void ai_task(void *pvParameters)
/*****************************************************************************
*   Input    : -
*   Output   : -
*   Function : Task for the analog input.
******************************************************************************/
{
    while (1)
    {
        if( xSemaphoreTake( xMutex_Scale_AI, portMAX_DELAY ))
        {
            adc_value = get_adc();
            xSemaphoreGive(xMutex_Scale_AI);
        }

    }
}


void scale_task(void *pvParameters)
/*****************************************************************************
*   Input    : -
*   Output   : -
*   Function : Task for scaling the value of the analog input.
******************************************************************************/
{

    while (1)
    {
        if( xSemaphoreTake( xMutex_Scale_AI, portMAX_DELAY ))
        {
            adc_adjusted_value = ( adc_value * scalefactor ) + offset;
            xSemaphoreGive(xMutex_Scale_AI);
        }
        gfprintf( COM2, "%c%cAI: %06u", 0x1B, 0x84, adc_adjusted_value);
        vTaskDelay(  500 / portTICK_PERIOD_MS);
    }
}


/****************************** End Of Module *******************************/
