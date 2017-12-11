//Header Files
#include <stdio.h>
#include <string.h>
#include "stm32f3xx_hal.h"
#include "stm32f3_discovery.h"
#include "task_executive.h"
#include "common.h"

# define MAX_TASKS 20

typedef struct Task_s 
{
 void (*f)(void *data); /* Task function */
 void *data;           /* Private data pointer for this task */
} Task_t;

//Now we need an array(task) to hold the list of currently active tasks.
//We also need an index(MAX_TASKS) in the array to keep track of the current task.

/* Process list */
Task_t tasks[MAX_TASKS];
int32_t currentTask;

void TaskInit(void)
{
 for(int i = 0; i<MAX_TASKS; i++)
 {
  tasks[i].f = NULL;
  tasks[i].data = NULL;
 }
}

//Cycle through the list, and find an empty slot, e.g. one with a function pointer equal to NULL.
//Fill the function pointer with the pointer passed in, and return the index(i).
//Otherwise, no new tasks can be created, and return -1.

int32_t TaskAdd(void (*f)(void *data), void *data)
{
  /* Try to find an empty slot */
  for(int i=0; i<MAX_TASKS; i++)
  {
    if(tasks[i].f == NULL)
     {
      tasks[i].f = f;
      tasks[i].data = data;
      return i;
     }
  }
 /* No slots available, return -1 */
 return -1;
}


//Set the function pointer in the array entry at the passed id equal to NULL.
//This has the effect of marking the task as inactive.

int32_t Taskkill(int32_t id)
{
  tasks[id].f    = NULL;
  tasks[id].data = NULL;
  return 0;
}


/* Find the next task to run */ 
static int32_t TaskNext(void) 
{  
  int32_t i;  
  uint32_t count=0;    
  i = currentTask;  
  do 
  {    
   i = (i + 1) % MAX_TASKS;   
   count++; 
  } 
  while((tasks[i].f == NULL) && (count <= MAX_TASKS));
  return (count <= MAX_TASKS) ? i : -1; 
}


int32_t TaskCurrent(void)
{
 currentTask = TaskNext();
 return currentTask;
}


int32_t TaskSwitcher(void)
{
  TaskCurrent(); 
  if(currentTask < 0) 
  {    
   return -1;  
  } 
  tasks[currentTask].f(tasks[currentTask].data);
  return 1;
}
