/*********************************************************************
*                SEGGER Microcontroller GmbH & Co. KG                *
*                        The Embedded Experts                        *
**********************************************************************
*                                                                    *
*       (c) 1995 - 2017 SEGGER Microcontroller GmbH & Co. KG         *
*                                                                    *
*       Internet: segger.com  Support: support_embos@segger.com      *
*                                                                    *
**********************************************************************
*                                                                    *
*       embOS * Real time operating system for microcontrollers      *
*                                                                    *
*       Please note:                                                 *
*                                                                    *
*       Knowledge of this file may under no circumstances            *
*       be used to write a similar product or a real-time            *
*       operating system for in-house use.                           *
*                                                                    *
*       Thank you for your fairness !                                *
*                                                                    *
**********************************************************************
*                                                                    *
*       OS version: 4.34.1                                           *
*                                                                    *
**********************************************************************

----------------------------------------------------------------------
File    : OS_Mailbox.c
Purpose : embOS sample program demonstrating the usage of mailboxes.
--------  END-OF-HEADER  ---------------------------------------------
*/

#include "RTOS.h"

/*********************************************************************
*
*       Defines, configurable
*
**********************************************************************
*/

#define MAX_MSG_SIZE  (8)  // Max. number of bytes per message
#define MAX_MSG_NUM   (2)  // Max. number of messages per Mailbox

/*********************************************************************
*
*       Static data
*
**********************************************************************
*/

static OS_STACKPTR int StackHP[128], StackLP[128];   /* Task stacks */
static OS_TASK         TCBHP, TCBLP;         /* Task-control-blocks */
static OS_MAILBOX      MyMailbox;
static char            MyMailboxBuffer[MAX_MSG_SIZE * MAX_MSG_NUM];

/*********************************************************************
*
*       Local functions
*
**********************************************************************
*/

/*********************************************************************
*
*       HPTask()
*/
static void HPTask(void) {
  char aData[MAX_MSG_SIZE];

  while (1) {
    OS_GetMail(&MyMailbox, (void *)aData);
    OS_SendString(aData);
  }
}

/*********************************************************************
*
*       LPTask()
*/
static void LPTask(void) {
  while (1) {
    OS_PutMail(&MyMailbox, "\nHello");
    OS_PutMail(&MyMailbox, "\nWorld !");
  }
}

/*********************************************************************
*
*       Global functions
*
**********************************************************************
*/

/*********************************************************************
*
*       main()
*/
int main(void) {
  OS_InitKern();                /* Initialize OS                    */
  OS_InitHW();                  /* Initialize Hardware for OS       */
  OS_CreateMB(&MyMailbox, MAX_MSG_SIZE, MAX_MSG_NUM, &MyMailboxBuffer);
  /* You need to create at least one task before calling OS_Start() */
  OS_CREATETASK(&TCBHP, "HP Task", HPTask, 100, StackHP);
  OS_CREATETASK(&TCBLP, "LP Task", LPTask,  50, StackLP);
  OS_SendString("embOS OS_Mailbox example");
  OS_SendString("\n\nDemonstrating message passing\n");
  OS_Start();                   /* Start multitasking               */
  return 0;
}

/****** End Of File *************************************************/
