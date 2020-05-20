/*****************************************************************************
* University of Southern Denmark
* Embedded Programming (EMP)
*
* MODULENAME.: rtcs.c
*
* PROJECT....: EMP
*
* DESCRIPTION: See module specification file (.h-file).
*
* Change Log:
*****************************************************************************
* Date    Id    Change
* YYMMDD
* --------------------
* 150303  MoH   Module created.
*
*****************************************************************************/

/***************************** Include files *******************************/
#include <stdint.h>
#include "tm4c123gh6pm.h"
#include "emp_type.h"
#include "glob_def.h"
#include "rtcs.h"
#include "systick.h"
#include "string.h"
#include "file.h"
#include "tmodel.h"

/*****************************    Defines    *******************************/

typedef struct
{
  INT8U  condition;
  INT8U  name;
  INT8U  state;
  INT8U  event;
  INT8U  sem;
  INT16U timer;
  void (*tf)(INT8U, INT8U, INT8U, INT8U);  // Pointer to task function
} tcb;

typedef struct
{
  INT8U  condition;
  INT8U  type;
  INT8U  count;
}scb;

typedef struct
{
	INT8U	head;
	INT8U	tail;
	SEM     q_not_full;
	SEM     q_not_empty;
	INT8U buf[QUEUE_SIZE];
}qcb;

/*****************************   Constants   *******************************/

/*****************************   Variables   *******************************/
extern volatile INT16S ticks;

HANDLE current_task;

tcb pot[MAX_TASKS];	            // Pool of tasks
scb pos[MAX_SEMAPHORES];        // Pool of semaphores
qcb poq[MAX_QUEUES];            // Pool of queues

/*****************************   Functions   *******************************/

HANDLE retrieve_id(void)
/*****************************************************************************
*   Input    :
*   Output   :
*   Function :
******************************************************************************/
{
  static HANDLE next_id = 0;

  return( next_id++ );
}

void i_am_alive( INT8U my_id, INT8U my_state, INT8U event, INT8U data )
/*****************************************************************************
*   Input    :
*   Output   :
*   Function :
******************************************************************************/
{
  if( my_state == 0 )
  {
    // Turn on the LED.
    GPIO_PORTD_DATA_R |= 0x40;
    set_state( 1 );
  }
  else
  {
    // Turn off the LED.
    GPIO_PORTD_DATA_R &= ~(0x40);
    set_state( 0 );
  }
  wait( 200 );
}

extern void set_state( INT8U new_state )
/*****************************************************************************
*   Function : See module specification (.h-file).
*****************************************************************************/
{
  pot[current_task].state = new_state;
}

extern void wait( INT16U timeout )
/*****************************************************************************
*   Function : See module specification (.h-file).
*****************************************************************************/
{
  pot[current_task].timer     = timeout;
  pot[current_task].condition = TASK_WAIT_FOR_TIMEOUT;
}

extern BOOLEAN wait_sem( INT8U sem, INT16U timeout )
/*****************************************************************************
*   Function : See module specification (.h-file).
*****************************************************************************/
{
  BOOLEAN result = TRUE;

  if( pos[sem].count )
  {
	pos[sem].count--;
    pot[current_task].condition = TASK_READY;
	result = TRUE;
  }
  else
  {
    pot[current_task].sem       = sem;
    pot[current_task].condition = TASK_WAIT_FOR_SEMAPHORE;
    if( timeout )
    {
	  pot[current_task].timer     = timeout;
	  pot[current_task].condition |= TASK_WAIT_FOR_TIMEOUT;
    }
    result = FALSE;
  }
  return( result );
}

extern void signal( INT8U sem )
/*****************************************************************************
*   Function : See module specification (.h-file).
*****************************************************************************/
{
  if( sem < MAX_SEMAPHORES )
	pos[sem].count++;
}

extern void preset_sem( INT8U sem, INT8U signals )
/*****************************************************************************
*   Function : See module specification (.h-file).
*****************************************************************************/
{
  if( sem < MAX_SEMAPHORES )
    pos[sem].count = signals;
}

INT8S open_queue( id )
INT8U id;
/*****************************************************************************
*   Function : See module specification (.h-file).
*****************************************************************************/
{
  INT8S result;

	if( id < MAX_QUEUES )
	{
	  poq[id].head        = 0;
      poq[id].tail        = 0;
      poq[id].q_not_full  = id;
      poq[id].q_not_empty = MAX_QUEUES + id;
      preset_sem( poq[id].q_not_full, QUEUE_SIZE );
      result = id;
	}
	else
	  result = -1;
	return( result );
}

BOOLEAN put_queue( id, ch, timeout )
INT8U  id;
INT8U  ch;
INT16U timeout;
/*****************************************************************************
*   Function : See module specification (.h-file).
*****************************************************************************/
{
  BOOLEAN result = FALSE;

  if( id < MAX_QUEUES )
  {
	if( wait_sem( poq[id].q_not_full, timeout ))
    {
      poq[id].buf[poq[id].head++] = ch;
      poq[id].head &= 0x7F;
      signal( poq[id].q_not_empty );
      result = TRUE;
    }
  }
  return( result );
}

BOOLEAN get_queue( id, pch, timeout )
INT8U  id;
INT8U *pch;
INT16U timeout;
/*****************************************************************************
*   Function : See module specification (.h-file).
*****************************************************************************/
{
  BOOLEAN result = FALSE;

  if( id < MAX_QUEUES )
  {
	if( wait_sem( poq[id].q_not_empty, timeout ))
    {
      *pch = poq[id].buf[poq[id].tail++];
      poq[id].tail &= 0x7F;
      signal( poq[id].q_not_full );
      result = TRUE;
    }
  }
  return( result );
}




extern HANDLE start_task( INT8U name, void (*tf)(INT8U, INT8U, INT8U, INT8U) )
/*****************************************************************************
*   Function : See module specification (.h-file).
*****************************************************************************/
{
  HANDLE this_id;

  this_id = retrieve_id();
  if( this_id != ERROR_TASK )
  {
	//pot[this_id].id    = this_id;
	pot[this_id].condition = TASK_READY;
	pot[this_id].name      = name;
    pot[this_id].state     = 0;
    pot[this_id].event     = EVENT_RESET;
	pot[this_id].timer     = 0;
	pot[this_id].tf        = tf;
  }
  return( 0 );
}

extern INT8U init_rtcs()
{
  INT8U i;

  init_systick();
  for( i = 0; i < MAX_SEMAPHORES; i++ )
    pos[i].count = 0;

  for( i = 0; i < MAX_TASKS; i++ )
  {
	  pot[i].condition = TASK_IDLE;
  }
  start_task( SYS_TASK+1, i_am_alive );
  return( 1 );
}

void schedule()
/*****************************************************************************
*   Function : See module specification (.h-file).
*****************************************************************************/
{

  while(1)
  {
    while( !ticks );
    ticks--;
    current_task = 0;
    do
    {
      if( pot[current_task].condition & TASK_WAIT_FOR_SEMAPHORE )
      {
    	  if( pos[pot[current_task].sem].count )
    	  {
    	    if( !( pot[current_task].sem < (2 * MAX_QUEUES )))
      	    pos[pot[current_task].sem].count--;
   	      pot[current_task].event     = EVENT_SIGNAL;
    	    pot[current_task].condition = TASK_READY;
    	  }
      }
      if( pot[current_task].condition & TASK_WAIT_FOR_TIMEOUT )
      {
    	  if( pot[current_task].timer )
    	  {
          pot[current_task].timer--;
  	      if( pot[current_task].timer == 0 )
  	      {
    	    pot[current_task].event     = EVENT_TIMEOUT;
    	    pot[current_task].condition = TASK_READY;
  	      }
    	  }
      }

      if( pot[current_task].condition == TASK_READY )
        pot[current_task].tf(current_task, pot[current_task].state, pot[current_task].event, 0);
   	  current_task++;
	} while ( pot[current_task].condition != TASK_IDLE );
  }
}

void debug_task(INT8U my_id, INT8U my_state, INT8U event, INT8U data)
{
    static tcb potcopy[MAX_TASKS];
    static INT8U i = 0;
    static char con[7][10] = {{"IDLE     "}, {"READY    "}, {"WAITING T"},  // Make a list of the strings used for the different task conditions
                            {"UNDEFINED"}, {"WAITING S"},
                            {"UNDEFINED"},{"UNDEFINED"}};

    switch (my_state)
    {
    case IDLE:
        if(wait_sem(SEM_DEBUG, WAIT_FOREVER))           // In this state we wait for the semaphore from the ui_task telling us to run the debugger
        {
            for(INT8U j = 0; j < MAX_TASKS; j++)        // Printing tasks takes more than one tick so we copy the pool of tasks so all the debug info is from the same timestamp
            {
                potcopy[j] = pot[j];
            }
            set_state(PRINT);                           // We go to the printing state
        }
        else wait(1);                                   // When calling if(wait_sem) we need this wait(1) or the code will not work
        break;
    case PRINT:
        if(potcopy[i].name != NULL)                     // Since the pool of tasks contains empty tasks we make sure to print only the ones with a name
        {
            gfprintf(COM1, "\nTask: %02u, Condition: %s, Sem: %03u, Tim: %04u, State: %02u",            // Print the task info to uart (COM1)
                     i, con[potcopy[i].condition], potcopy[i].sem, potcopy[i].timer, potcopy[i].state);
            wait(56);                                   // Since it takes several ticks for the uart queue to empty we have to wait between printing lines
        }
        i++;
        if(i > MAX_TASKS)                               // After going through all the tasks we go back to the IDLE state
        {
            gfprintf(COM1, "\n");
            i = 0;
            set_state(IDLE);
        }
        break;
    }
}

/****************************** End Of Module *******************************/





