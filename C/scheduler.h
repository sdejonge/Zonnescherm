#ifndef _scheduler
#define _scheduler

// Scheduler data structure for storing task data
typedef struct
{
	// Pointer to task
	void (*task_pointer)(void);
	// Initial delay in ticks
	unsigned int delay;
	// Periodic interval in ticks
	unsigned int period;
	// Runme flag (indicating when the task is due to run)
	unsigned char runme;
} Task;

// Function prototypes
void scheduler_init_t1(void);
void scheduler_start(void);

// Core scheduler functions
void scheduler_dispatch_tasks(void);
unsigned char scheduler_add_task(void (*)(void), const unsigned int, const unsigned int);
unsigned char scheduler_delete_task(const unsigned char);

#define SCHEDULER_MAX_TASKS 5

#endif