/*****************************************************************************
* University of Southern Denmark
* Embedded C Programming (ECP)
*
* MODULENAME.: key.c
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
#include "tmodel.h"
#include "key.h"

#include "FreeRTOS.h"
#include "Task.h"
#include "queue.h"
#include "semphr.h"

#include "adc.h"
#include "file.h"
#include "string.h"

/*****************************    Defines    *******************************/
#define QUEUE_LEN   128
#define SIZE_UINT8  sizeof(unsigned char)

/*****************************   Constants   *******************************/

/*****************************   Variables   *******************************/
static INT8U my_state = 0;
static INT8U my_state_keyboard = 0, my_state_ui = 0;
static QueueHandle_t xQueueKey;
static SemaphoreHandle_t xMutex_Key_UI = NULL;

/*****************************   Functions   *******************************/

void key_init_queue()
{
    xQueueKey = xQueueCreate(QUEUE_LEN, sizeof(INT8U));
    xMutex_Key_UI = xSemaphoreCreateMutex();
    xSemaphoreGive(xMutex_Key_UI);
}


INT8U row( INT8U y )
{
  INT8U result = 0;

  switch( y )
  {
    case 0x01: result = 1; break;
    case 0x02: result = 2; break;
    case 0x04: result = 3; break;
    case 0x08: result = 4; break;
  }
  return( result );
}

INT8U key_catch( x, y )
INT8U x, y;
{
  const INT8U matrix[3][4] = {{'*','7','4','1'},
                              {'0','8','5','2'},
                              {'#','9','6','3'}};

  return( matrix[x-1][y-1] );
}

BOOLEAN get_keyboard( INT8U *pch )
{
    xQueueReceive(xQueueKey, &pch, 0U);
    return( 1 );
}

BOOLEAN check_column(INT8U x)
{
    INT8U y = GPIO_PORTE_DATA_R & 0x0F;             // Save the values of the 4 bits for the rows
    if( y )                                         // If one of them are set...
    {                                               // ...we first find the row number with the function row()
        INT8U ch = key_catch( x, row(y) );          // Now that we have the row and column we look up the corresponding character using the function key_catch
        xQueueSend(xQueueKey, &ch, 0U);             // Put the character in a queue so it can be used by another task
        //put_queue( Q_KEY, ch, 1 );                // Put the character in a queue so it can be used by another task
        return 1;
    }
    return 0;
}


void set_state(INT8U new_state)
{
    my_state = new_state;
}


void keyboard_task(void *pvParameters)
/*****************************************************************************
*   Input    :
*   Output   :
*   Function :
******************************************************************************/
{
  while(1)
  {
      //if (xSemaphoreTake(xMutex_Key_UI, portMAX_DELAY))
      //{
          switch(my_state_keyboard)
          {
          case 0:
            GPIO_PORTA_DATA_R &= 0xE3;          // Clear the 3 bits for the columns
            GPIO_PORTA_DATA_R |= 0x10;          // Set the bit for column 1
            if (check_column(1))                // Check all the rows for column 1, using the function check_column
            {                                   // If a button press is registered we go to next state so the press is only registered once
                my_state_keyboard = 1;
                //set_state(1);
                //xSemaphoreGive(xMutex_Key_UI);
                break;
            }
            GPIO_PORTA_DATA_R &= 0xE3;          // Repeat the above for the two other columns
            GPIO_PORTA_DATA_R |= 0x08;
            if (check_column(2))
            {
                my_state_keyboard = 1;
                //set_state(1);
                //xSemaphoreGive(xMutex_Key_UI);
                break;
            }
            GPIO_PORTA_DATA_R &= 0xE3;
            GPIO_PORTA_DATA_R |= 0x04;
            if (check_column(3))
            {
                my_state_keyboard = 1;
                //set_state(1);
                //xSemaphoreGive(xMutex_Key_UI);
                break;
            }
            break;
          case 1:
            if( !(GPIO_PORTE_DATA_R & 0x0F) )   // We stay here until the button is released so a button press is not counted more than once
            {
                my_state_keyboard = 0;
                //set_state( 0 );
            }
            break;
          }
          //xSemaphoreGive(xMutex_Key_UI);
          //vTaskDelay(250 / portTICK_RATE_MS); // wait 50 milliseconds
      //}
  }
}

void ui_task(void *pvParameters)
/*****************************************************************************
*   Input    :
*   Output   :
*   Function :
******************************************************************************/
{
  INT8U ch;
  static INT8U  Buf[4];

  while(1)
  {
      //if (xSemaphoreTake(xMutex_Key_UI, portMAX_DELAY))
      //{
          if( get_file( COM3, &ch ))
          {
              xQueueReceive(xQueueKey, &ch, portMAX_DELAY);
              switch( my_state_ui )
              {
              case 0:
                  Buf[0] = ch;
                  gfprintf( COM2, "%c%cS:%c", 0x1B, 0xC4, ch );
                  my_state_ui = 1;
                  //set_state( 1 );
                  break;
              case 1:
                  Buf[1] = ch;
                  gfprintf( COM2, "%c%c%c O:", 0x1B, 0xC7, ch );
                  my_state_ui = 2;
                  //set_state( 2 );
                  break;
              case 2:
                  Buf[2] = ch;
                  gfprintf( COM2, "%c%c%c", 0x1B, 0xCB, ch );
                  my_state_ui = 3;
                  //set_state( 3 );
                  break;
              case 3:
                  Buf[3] = ch;
                  INT16U scalefactor_new;
                  INT16U offset_new;
                  scalefactor_new = (Buf[0]-'0')*10+Buf[1]-'0';
                  offset_new = (Buf[2]-'0')*10+Buf[3]-'0';
                  set_scalefactor(scalefactor_new);
                  set_offset(offset_new);
                  gfprintf( COM2, "%c%c         ", 0x1B, 0xC3 );
                  my_state_ui = 0;
                  //set_state( 0 );
                  break;
              }
          }
          //xSemaphoreGive(xMutex_Key_UI);
          //vTaskDelay(250 / portTICK_RATE_MS); // wait 50 milliseconds
      //}
  }
}

