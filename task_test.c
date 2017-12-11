//Header Files
#include <stdio.h>
#include <string.h>
#include "stm32f3xx_hal.h"
#include "task_executive.h"
#include "stm32f3_discovery.h"
#include "common.h"
int currentMillis = 0;
int previousMillis = 0;

void Task1(void *data)
{
    currentMillis = myTickCount;                     // store the value of myTickCount in CurrentMillis
    int interval = (currentMillis - previousMillis); // Find the current time interval
    if(interval >= 1000)                             // Here we set the limit of 1 sec
    {
      previousMillis = currentMillis;                // Reset the value of previousMillis So that LEDs Blink 
      for (int ledNo = 0; ledNo <=7; ledNo++)        // after every Sec
      {                                             
        BSP_LED_Toggle(ledNo);                       // Toggle the value
      }                       
    }


//printf("Hello from Task 1\n");
}

void Task2(void *data)
{
printf("Hello from Task 2\n");
}

void CmdTaskTest(int mode)
{
    if(mode != CMD_INTERACTIVE) {
    return;
  }
  printf("Intializing Task Executive\n");
  TaskInit();

  printf("Adding tasks\n");
  TaskAdd(Task1, NULL);
  TaskAdd(Task2, NULL);
  
  printf("Running Task Switcher\n");

  for(int i = 0; i<25; i++)
  {
   TaskSwitcher();
  }
}

ADD_CMD("task",CmdTaskTest,"                Enter the no of task");
