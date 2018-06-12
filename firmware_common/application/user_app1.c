/**********************************************************************************************************************
File: user_app1.c                                                                

----------------------------------------------------------------------------------------------------------------------
To start a new task using this user_app1 as a template:
 1. Copy both user_app1.c and user_app1.h to the Application directory
 2. Rename the files yournewtaskname.c and yournewtaskname.h
 3. Add yournewtaskname.c and yournewtaskname.h to the Application Include and Source groups in the IAR project
 4. Use ctrl-h (make sure "Match Case" is checked) to find and replace all instances of "user_app1" with "yournewtaskname"
 5. Use ctrl-h to find and replace all instances of "UserApp1" with "YourNewTaskName"
 6. Use ctrl-h to find and replace all instances of "USER_APP1" with "YOUR_NEW_TASK_NAME"
 7. Add a call to YourNewTaskNameInitialize() in the init section of main
 8. Add a call to YourNewTaskNameRunActiveState() in the Super Loop section of main
 9. Update yournewtaskname.h per the instructions at the top of yournewtaskname.h
10. Delete this text (between the dashed lines) and update the Description below to describe your task
----------------------------------------------------------------------------------------------------------------------

Description:
This is a user_app1.c file template 

------------------------------------------------------------------------------------------------------------------------
API:

Public functions:


Protected System functions:
void UserApp1Initialize(void)
Runs required initialzation for the task.  Should only be called once in main init section.

void UserApp1RunActiveState(void)
Runs current task state.  Should only be called once in main loop.


**********************************************************************************************************************/

#include "configuration.h"

/***********************************************************************************************************************
Global variable definitions with scope across entire project.
All Global variable names shall start with "G_UserApp1"
***********************************************************************************************************************/
/* New variables */
volatile u32 G_u32UserApp1Flags;                       /* Global state flags */


/*--------------------------------------------------------------------------------------------------------------------*/
/* Existing variables (defined in other files -- should all contain the "extern" keyword) */
extern volatile u32 G_u32SystemFlags;                  /* From main.c */
extern volatile u32 G_u32ApplicationFlags;             /* From main.c */

extern volatile u32 G_u32SystemTime1ms;                /* From board-specific source file */
extern volatile u32 G_u32SystemTime1s;                 /* From board-specific source file */

extern u32 G_u32AntApiCurrentMessageTimeStamp;
extern AntApplicationMessageType G_eAntApiCurrentMessageClass;
extern u8 G_au8AntApiCurrentMessageBytes[ANT_APPLICATION_MESSAGE_BYTES];
extern AntExtendedDataType G_sAntApiCurrentMessageExtData;


/***********************************************************************************************************************
Global variable definitions with scope limited to this local application.
Variable names shall start with "UserApp1_" and be declared as static.
***********************************************************************************************************************/
static fnCode_type UserApp1_StateMachine;            /* The state machine function pointer */
static u32 UserApp1_u32Timeout;                      /* Timeout counter used across states */
static u8 au8TestMessage[] = {0, 0, 0, 0, 0, 0, 0, 0};
AntAssignChannelInfoType UserApp1_sChannelInfo;
/**********************************************************************************************************************
Function Definitions
**********************************************************************************************************************/

/*--------------------------------------------------------------------------------------------------------------------*/
/* Public functions                                                                                                   */
/*--------------------------------------------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------------------------------------------*/
/* Protected functions                                                                                                */
/*--------------------------------------------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------------------------------------------
Function: UserApp1Initialize

Description:
Initializes the State Machine and its variables.

Requires:
  -

Promises:
  - 
*/
void UserApp1Initialize(void)
{
   u8 au8WelcomeMessage[] = "ANTMaster";
   /* Set a message up on the LCD. Delay is required to let the clear commmand send*/
   LCDCommand(LCD_CLEAR_CMD);
   for(u32 i = 0; i < 10000; i++);
   LCDMessage(LINE1_START_ADDR, au8WelcomeMessage);
   
   /* Configure ANT for this application*/
   UserApp1_sChannelInfo.AntChannel                  =   ANT_CHANNEL_USERAPP;
   UserApp1_sChannelInfo.AntChannelType              =   ANT_CHANNEL_TYPE_USERAPP;
   UserApp1_sChannelInfo.AntChannelPeriodLo          =   ANT_CHANNEL_PERIOD_LO_USERAPP;
   UserApp1_sChannelInfo.AntChannelPeriodHi          =   ANT_CHANNEL_PERIOD_HI_USERAPP;
   
   UserApp1_sChannelInfo.AntDeviceIdLo               =  ANT_DEVICEID_LO_USERAPP;
   UserApp1_sChannelInfo.AntDeviceIdHi               =  ANT_DEVICEID_HI_USERAPP;
   UserApp1_sChannelInfo.AntDeviceType               =  ANT_DEVICE_TYPE_USERAPP;
   UserApp1_sChannelInfo.AntTransmissionType         =  ANT_TRANSMISSION_TYPE_USERAPP;
   UserApp1_sChannelInfo.AntFrequency                =  ANT_FREQUENCY_USERAPP;
   UserApp1_sChannelInfo.AntTxPower                  =  ANT_TX_POWER_USERAPP;
   
   UserApp1_sChannelInfo.AntNetwork = ANT_NETWORK_DEFAULT;
   for(u8 i = 0; i < ANT_NETWORK_NUMBER_BYTES; i++)
   {
      UserApp1_sChannelInfo.AntNetworkKey[i] = ANT_DEFAULT_NETWORK_KEY;
   }
   
   /* Attempt to queue the ANT channel setup */
   if(AntAssignChannel(&UserApp1_sChannelInfo))
   {
     LedOn(ORANGE);
      UserApp1_u32Timeout = G_u32SystemTime1ms;
      UserApp1_StateMachine = UserApp1SM_AntChannelAssign;
   }
   else
   {
      /* The task isn't properly intialized, so shut down and don't run */
     //DebugPrintf(UserApp1_au8MessageFail);
     UserApp1_StateMachine = UserApp1SM_Error;
     LedOn(RED);
   }

} /* end UserApp1Initialize() */

  
/*----------------------------------------------------------------------------------------------------------------------
Function UserApp1RunActiveState()

Description:
Selects and runs one iteration of the current state in the state machine.
All state machines have a TOTAL of 1ms to execute, so on average n state machines
may take 1ms / n to execute.

Requires:
  - State machine function pointer points at current state

Promises:
  - Calls the function to pointed by the state machine function pointer
*/
void UserApp1RunActiveState(void)
{
  UserApp1_StateMachine();

} /* end UserApp1RunActiveState */


/*--------------------------------------------------------------------------------------------------------------------*/
/* Private functions                                                                                                  */
/*--------------------------------------------------------------------------------------------------------------------*/


/**********************************************************************************************************************
State Machine Function Definitions
**********************************************************************************************************************/

/*-------------------------------------------------------------------------------------------------------------------*/
/* Wait for ANT channel assignment */
static void UserApp1SM_AntChannelAssign()
{
  if(AntRadioStatusChannel(ANT_CHANNEL_USERAPP)  == ANT_CONFIGURED)
  {
    LedOn(YELLOW);
     /* Channel assignment is successful, so open channel and procede to idle state */
    AntOpenChannelNumber(ANT_CHANNEL_USERAPP);
    UserApp1_StateMachine = UserApp1SM_Idle;
  }
  
  /* Watch for time out */
  if(IsTimeUp(&UserApp1_u32Timeout, 3000))
  {
    LedOn(RED);
    //  DebugPrintf(UserApp1_au8MessageFail);
      UserApp1_StateMachine = UserApp1SM_Error;
  }
}/* end UserApp1SM_AntChannelAssign */


/* Wait for ??? */
static void UserApp1SM_Idle(void)
{
  
  if(IsButtonPressed(BUTTON0))
  {
    for(int i = 0; i < 8; i++)
    {
      au8TestMessage[i] = 0xff;
    }
  }
  else if(IsButtonPressed(BUTTON1))
  {
    au8TestMessage[0] = 0x00;
    au8TestMessage[1] = 0x01;
    au8TestMessage[2] = 0x02;
    au8TestMessage[3] = 0x03;
    au8TestMessage[4] = 0x04;
    au8TestMessage[5] = 0x05;
    au8TestMessage[6] = 0x06;
    au8TestMessage[7] = 0x07;
  }
    else if(IsButtonPressed(BUTTON2))
  {
    au8TestMessage[0] = 0x00;
    au8TestMessage[1] = 0x11;
    au8TestMessage[2] = 0x22;
    au8TestMessage[3] = 0x33;
    au8TestMessage[4] = 0x44;
    au8TestMessage[5] = 0x55;
    au8TestMessage[6] = 0x66;
    au8TestMessage[7] = 0x77;
  }
  else
  {
    for(int i = 0; i < 8; i++)
    {
      au8TestMessage[i] = 0x00;
    }
  }
   if(AntReadAppMessageBuffer())
   {
     /* New message from ANT task: check what it is */
      if(G_eAntApiCurrentMessageClass == ANT_DATA)
      {
        LedOn(GREEN);
          /* we got some data */
      }
      else if(G_eAntApiCurrentMessageClass == ANT_TICK)
      {
        LedOn(BLUE);
        
        /* A channel period has gone by: typically this is when new data should
           be queued to be sent*/
        /* Update and queue the new message data 
        au8TestMessage[7]++;
        if(au8TestMessage[7] == 0)
        {
          au8TestMessage[6]++;
          if(au8TestMessage[6] == 0)
          {
            au8TestMessage[5]++;
          }
        }
        
        au8TestMessage[0] = 0x00;
        if(IsButtonPressed(BUTTON0))
        {
          au8TestMessage[0] = 0xff;
        }
        */
        
      }
      LedOff(GREEN);
      AntQueueBroadcastMessage(ANT_CHANNEL_USERAPP, au8TestMessage);
      LedOff(BLUE);
   } /* end AntReadAppMessageBuffer() */ 
} /* end UserApp1SM_Idle() */
    

/*-------------------------------------------------------------------------------------------------------------------*/
/* Handle an error */
static void UserApp1SM_Error(void)          
{
  
} /* end UserApp1SM_Error() */



/*--------------------------------------------------------------------------------------------------------------------*/
/* End of File                                                                                                        */
/*--------------------------------------------------------------------------------------------------------------------*/
