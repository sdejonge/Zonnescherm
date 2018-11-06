/*
 * GccApplication5.c
 *
 * Created: 5-11-2018 16:31:20
 *  Author: Maarten
 */ 


#include <avr/io.h>
#include <avr/interrupt.h>
#include "scheduler.h"

// The array of tasks
Task scheduler_tasks_g[SCHEDULER_MAX_TASKS];

/*------------------------------------------------------------------*-
  scheduler_dispatch_tasks()
  This is the 'dispatcher' function.  When a task (function)
  is due to run, scheduler_dispatch_tasks() will run it.
  This function must be called (repeatedly) from the main loop.
-*------------------------------------------------------------------*/

void scheduler_dispatch_tasks(void)
{
    // Dispatches (runs) the next task (if one is ready)
    for (unsigned char index = 0; index < SCHEDULER_MAX_TASKS; index++)
    {
        if ((scheduler_tasks_g[index].runme > 0) && (scheduler_tasks_g[index].task_pointer != 0))
        {
            (*scheduler_tasks_g[index].task_pointer)();  // Run the task
            scheduler_tasks_g[index].runme--;   // Reset / reduce runme flag

            // Periodic tasks will automatically run again
            // - if this is a 'one shot' task, remove it from the array
            if (scheduler_tasks_g[index].period == 0)
            {
                scheduler_delete_task(index);
            }
        }
    }
}

/*------------------------------------------------------------------*-
  scheduler_add_task()
  Causes a task (function) to be executed at regular intervals 
  or after a user-defined delay
  pFunction - The name of the function which is to be scheduled.
              NOTE: All scheduled functions must be 'void, void' -
              that is, they must take no parameters, and have 
              a void return type. 
                   
  DELAY     - The interval (TICKS) before the task is first executed
  PERIOD    - If 'PERIOD' is 0, the function is only called once,
              at the time determined by 'DELAY'.  If PERIOD is non-zero,
              then the function is called repeatedly at an interval
              determined by the value of PERIOD (see below for examples
              which should help clarify this).
  RETURN VALUE:  
  Returns the position in the task array at which the task has been 
  added.  If the return value is SCHEDULER_MAX_TASKS then the task could 
  not be added to the array (there was insufficient space).  If the
  return value is < SCHEDULER_MAX_TASKS, then the task was added 
  successfully.  
  Note: this return value may be required, if a task is
  to be subsequently deleted - see scheduler_delete_task().
  EXAMPLES:
  Task_ID = scheduler_add_task(Do_X,1000,0);
  Causes the function Do_X() to be executed once after 1000 sch ticks.            
  Task_ID = scheduler_add_task(Do_X,0,1000);
  Causes the function Do_X() to be executed regularly, every 1000 sch ticks.            
  Task_ID = scheduler_add_task(Do_X,300,1000);
  Causes the function Do_X() to be executed regularly, every 1000 ticks.
  Task will be first executed at T = 300 ticks, then 1300, 2300, etc.            
 
-*------------------------------------------------------------------*/

unsigned char scheduler_add_task(void (*pFunction)(), const unsigned int DELAY, const unsigned int PERIOD)
{
    unsigned char index = 0;

    // First find a gap in the array (if there is one)
    while ((scheduler_tasks_g[index].task_pointer != 0) && (index < SCHEDULER_MAX_TASKS))
    {
        index++;
    }

    // Have we reached the end of the list?
    if (index == SCHEDULER_MAX_TASKS)
    {
        // Task list is full, return an error code
        return SCHEDULER_MAX_TASKS;  
    }

    // If we're here, there is a space in the task array
    scheduler_tasks_g[index].task_pointer = pFunction;
    scheduler_tasks_g[index].delay = DELAY;
    scheduler_tasks_g[index].period = PERIOD;
    scheduler_tasks_g[index].runme = 0;

    // return position of task (to allow later deletion)
    return index;
}

/*------------------------------------------------------------------*-
  scheduler_delete_task()
  Removes a task from the scheduler.  Note that this does
  *not* delete the associated function from memory: 
  it simply means that it is no longer called by the scheduler. 
 
  TASK_INDEX - The task index.  Provided by scheduler_add_task(). 
  RETURN VALUE:  RETURN_ERROR or RETURN_NORMAL
-*------------------------------------------------------------------*/

unsigned char scheduler_delete_task(const unsigned char TASK_INDEX)
{
    scheduler_tasks_g[TASK_INDEX].task_pointer = 0;
    scheduler_tasks_g[TASK_INDEX].delay = 0;
    scheduler_tasks_g[TASK_INDEX].period = 0;
    scheduler_tasks_g[TASK_INDEX].runme = 0;

    return (unsigned char) 0;
}

/*------------------------------------------------------------------*-
  scheduler_init_t1()
  Scheduler initialisation function.  Prepares scheduler
  data structures and sets up timer interrupts at required rate.
  You must call this function before using the scheduler.  
-*------------------------------------------------------------------*/

void scheduler_init_t1(void)
{
    for (unsigned char i = 0; i < SCHEDULER_MAX_TASKS; i++)
    {
        scheduler_delete_task(i);
    }

    // Set up Timer 1
    // Values for 1ms and 10ms ticks are provided for various crystals

    OCR1A = (uint16_t) 625;                  // 10ms = (256/16.000.000) * 625
    TCCR1B = (1 << CS12) | (1 << WGM12);     // prescale op 64, top counter = value OCR1A (CTC mode)
    TIMSK1 = 1 << OCIE1A;                    // Timer 1 Output Compare A Match Interrupt Enable
}

/*------------------------------------------------------------------*-
  scheduler_start()
  Starts the scheduler, by enabling interrupts.
  NOTE: Usually called after all regular tasks are added,
  to keep the tasks synchronised.
  NOTE: ONLY THE SCHEDULER INTERRUPT SHOULD BE ENABLED!!! 
 
-*------------------------------------------------------------------*/

void scheduler_start(void)
{
    sei();
}

/*------------------------------------------------------------------*-
  SCHEDULER_Update
  This is the scheduler ISR.  It is called at a rate 
  determined by the timer settings in scheduler_init_t1().
-*------------------------------------------------------------------*/

ISR(TIMER1_COMPA_vect)
{
    for (unsigned char index = 0; index < SCHEDULER_MAX_TASKS; index++)
    {
        // Check if there is a task at this location
        if (scheduler_tasks_g[index].task_pointer)
        {
            if (scheduler_tasks_g[index].delay == 0)
            {
                // The task is due to run, Inc. the 'RunMe' flag
                scheduler_tasks_g[index].runme++;

                if (scheduler_tasks_g[index].period)
                {
                    // Schedule periodic tasks to run again
                    scheduler_tasks_g[index].delay = scheduler_tasks_g[index].period;
                    scheduler_tasks_g[index].delay--;
                }
            }
            else
            {
                // Not yet ready to run: just decrement the delay
                scheduler_tasks_g[index].delay--;
            }
        }
    }
}