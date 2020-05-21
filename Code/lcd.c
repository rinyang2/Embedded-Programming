/*****************************************************************************
* University of Southern Denmark
* Embedded C Programming (ECP)
*
* MODULENAME.: leds.c
*
* PROJECT....: ECP
*
* DESCRIPTION: See module specification file (.h-file).
*
* Change Log:
******************************************************************************
* Date    Id    Change
* YYMMDD
* --------------------
* 050128  KA    Module created.
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

enum LCD_states
{
  LCD_POWER_UP,
  LCD_INIT,
  LCD_READY,
  LCD_ESC_RECEIVED,
};

#define QUEUE_LEN   128
#define SIZE_UINT8  sizeof(unsigned char)

/*****************************   Constants   *******************************/
const INT8U LCD_init_sequense[]= 
{
  0x30,		// Reset
  0x30,		// Reset
  0x30,		// Reset
  0x20,		// Set 4bit interface
  0x28,		// 2 lines Display
  0x0C,		// Display ON, Cursor OFF, Blink OFF
  0x06,		// Cursor Increment
  0x01,		// Clear Display
  0x02,     	// Home
  0xFF		// stop
}; 

/*****************************   Variables   *******************************/
//INT8U LCD_buf[QUEUE_LEN];
//INT8U LCD_buf_head = 0;
//INT8U LCD_buf_tail = 0;
//INT8U LCD_buf_len  = 0;

static enum LCD_states LCD_state = LCD_POWER_UP;
INT8U LCD_init;

static QueueHandle_t xQueueOutput;

/*****************************   Functions   *******************************/
INT8U wr_ch_LCD( INT8U Ch )
/*****************************************************************************
*   OBSERVE  : LCD_PROC NEEDS 20 mS TO PRINT OUT ONE CHARACTER 
*   Function : See module specification (.h-file).
*****************************************************************************/
{
  return( xQueueSend(xQueueOutput, &Ch, portMAX_DELAY) );
}

void wr_str_LCD( INT8U *pStr )
/*****************************************************************************
*   Function : See module specification (.h-file).
*****************************************************************************/
{
  while( *pStr )
  {
    wr_ch_LCD( *pStr );
	pStr++;
  }
}

void move_LCD( INT8U x, INT8U y )
/*****************************************************************************
*   Function : See module specification (.h-file).
*****************************************************************************/
{
  INT8U Pos;

  Pos = y*0x40 + x;
  Pos |= 0x80;
  wr_ch_LCD( ESC );
  wr_ch_LCD( Pos );
}
//----------------------------

void wr_ctrl_LCD_low( INT8U Ch )
/*****************************************************************************
*   Input    : -
*   Output   : -
*   Function : Write low part of control data to LCD.
******************************************************************************/
{
  INT8U temp;
  volatile int i;
  
  temp = GPIO_PORTC_DATA_R & 0x0F;
  temp  = temp | ((Ch & 0x0F) << 4);
  GPIO_PORTC_DATA_R  = temp;
  for( i=0; i<1000; i )
	  i++;
  GPIO_PORTD_DATA_R &= 0xFB;        // Select Control mode, write
  for( i=0; i<1000; i )
	  i++;
  GPIO_PORTD_DATA_R |= 0x08;		// Set E High

  for( i=0; i<1000; i )
	  i++;

  GPIO_PORTD_DATA_R &= 0xF7;		// Set E Low

  for( i=0; i<1000; i )
	  i++;
}

void wr_ctrl_LCD_high( INT8U Ch )
/*****************************************************************************
*   Input    : -
*   Output   : -
*   Function : Write high part of control data to LCD.
******************************************************************************/
{
  wr_ctrl_LCD_low(( Ch & 0xF0 ) >> 4 );
}

void out_LCD_low( INT8U Ch )
/*****************************************************************************
*   Input    : Mask
*   Output   : -
*   Function : Send low part of character to LCD. 
*              This function works only in 4 bit data mode.
******************************************************************************/
{
  INT8U temp;
	  
  temp = GPIO_PORTC_DATA_R & 0x0F;
  GPIO_PORTC_DATA_R  = temp | ((Ch & 0x0F) << 4);
  //GPIO_PORTD_DATA_R &= 0x7F;        // Select write
  GPIO_PORTD_DATA_R |= 0x04;        // Select data mode
  GPIO_PORTD_DATA_R |= 0x08;		// Set E High
  GPIO_PORTD_DATA_R &= 0xF7;		// Set E Low
}

void out_LCD_high( INT8U Ch )
/*****************************************************************************
*   Input    : Mask
*   Output   : -
*   Function : Send high part of character to LCD. 
*              This function works only in 4 bit data mode.
******************************************************************************/
{
  out_LCD_low((Ch & 0xF0) >> 4);
}

void wr_ctrl_LCD( INT8U Ch )
/*****************************************************************************
*   Input    : -
*   Output   : -
*   Function : Write control data to LCD.
******************************************************************************/
{
  static INT8U Mode4bit = FALSE;
  INT16U i;

  wr_ctrl_LCD_high( Ch );
  if( Mode4bit )
  {
	for(i=0; i<1000; i++);
	wr_ctrl_LCD_low( Ch );
  }
  else
  {
	if( (Ch & 0x30) == 0x20 )
	  Mode4bit = TRUE;
  }
}

void clr_LCD()
/*****************************************************************************
*   Input    : -
*   Output   : -
*   Function : Clear LCD.
******************************************************************************/
{
  wr_ctrl_LCD( 0x01 );
}


void home_LCD()
/*****************************************************************************
*   Input    : -
*   Output   : -
*   Function : Return cursor to the home position.
******************************************************************************/
{
  wr_ctrl_LCD( 0x02 );
}

void Set_cursor( INT8U Ch )
/*****************************************************************************
*   Input    : New Cursor position
*   Output   : -
*   Function : Place cursor at given position.
******************************************************************************/
{
  wr_ctrl_LCD( Ch );
}


void out_LCD( INT8U Ch )
/*****************************************************************************
*   Input    : -
*   Output   : -
*   Function : Write control data to LCD.
******************************************************************************/
{
  INT16U i;

  out_LCD_high( Ch );
  for(i=0; i<1000; i++);
  out_LCD_low( Ch );
}


void lcd_init_queue()
{
    xQueueOutput = xQueueCreate(QUEUE_LEN, sizeof(INT8U));

    //move_LCD( 4, 0 );
    //wr_ch_LCD( '4' );
    //move_LCD( 6, 1 );
    //wr_ch_LCD( '2' );
}


void lcd_task(void *pvParameters)
//INT8U task_no;
/*****************************************************************************
*   Input    :
*   Output   :
*   Function :
******************************************************************************/
{
  INT8U ch;
  SemaphoreHandle_t* mutex_scale_lcd = (SemaphoreHandle_t*) pvParameters;

  while (1)
  {
      //if (xSemaphoreTake(mutex_scale_lcd, portMAX_DELAY))
      //{
          switch( LCD_state )
          {
            case LCD_POWER_UP:
              LCD_init = 0;
              LCD_state = LCD_INIT;
              //vTaskDelay(500 / portTICK_RATE_MS); // wait 100 milliseconds
              vTaskDelay(  50 / portTICK_PERIOD_MS);
              break;

            case LCD_INIT:
              if( LCD_init_sequense[LCD_init] != 0xFF )
                wr_ctrl_LCD( LCD_init_sequense[LCD_init++] );
              else
              {
                LCD_state = LCD_READY;
              }
              vTaskDelay(  50 / portTICK_PERIOD_MS);
              //vTaskDelay(500 / portTICK_RATE_MS); // wait 100 milliseconds
              break;

            case LCD_READY:
              xQueueReceive(xQueueOutput, &ch, portMAX_DELAY);
              if( ch )
              {
                switch( ch )
                {
                  case 0xFF:
                    clr_LCD();
                    break;
                  case ESC:
                  LCD_state = LCD_ESC_RECEIVED;
                    break;
                  default:
                    out_LCD( ch );
                    vTaskDelay(  5 / portTICK_PERIOD_MS);
                }
              }
              break;

            case LCD_ESC_RECEIVED:
                xQueueReceive(xQueueOutput, &ch, portMAX_DELAY);
                if( ch )
                {
                    if( ch & 0x80 )
                    {
                        Set_cursor( ch );
                    }
                    else
                    {
                        switch( ch )
                        {
                            case '@':
                              home_LCD();
                              break;
                        }
                    }
                    LCD_state = LCD_READY;
                    vTaskDelay(  5 / portTICK_PERIOD_MS);
                    //vTaskDelay(50 / portTICK_RATE_MS); // wait 10 milliseconds
                }
              break;
          }
          //xSemaphoreGive(mutex_scale_lcd);
          //vTaskDelay(50 / portTICK_RATE_MS); // wait 10 milliseconds
      //}
  }
}


/****************************** End Of Module *******************************/




