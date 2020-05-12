/*****************************************************************************
* University of Southern Denmark
* Embedded Programming (EMP)
*
* MODULENAME.: key.h
*
* PROJECT....: EMP
*
* DESCRIPTION: Test.
*
* Change Log:
******************************************************************************
* Date    Id    Change
* YYMMDD
* --------------------
* 150321  MoH   Module created.
*
*****************************************************************************/

#ifndef _KEY_H
  #define _KEY_H

void key_init_queue();
BOOLEAN get_keyboard( INT8U* );
void keyboard_task(void *pvParameters);
void set_state(INT8U);
void ui_task(void *pvParameters);

#endif
