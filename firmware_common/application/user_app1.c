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

AntAssignChannelInfoType    sChannelInfo;
/***********************************************************************************************************************
Global variable definitions with scope limited to this local application.
Variable names shall start with "UserApp1_" and be declared as static.
***********************************************************************************************************************/
static fnCode_type UserApp1_StateMachine;            /* The state machine function pointer */
static u32 UserApp1_u32Timeout;                      /* Timeout counter used across states */
AntAssignChannelInfoType UserApp1_sChannelInfo;

static u32 UserApp1_u32DataMsgCount = 0; /* ANT_DATA packets received */
static u32 UserApp1_u32TickMsgCount = 0; /* ANT_TICK packets received */

static u8 u8LastState = 0xff;
static u8 au8TickMessage[] = "EVENT x\n\r"; /* "x" at index [6] will be replaced by current code */
static u8 au8DataContent[] = "xxxxxxxxxxxxxxxx";
static u8 au8LastAntData [ANT_APPLICATION_MESSAGE_BYTES] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
static u8 au8TestMessage[] = {0, 0, 0, 0, 0xA5, 0, 0, 0};
bool bGotNewData;
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
   u8 au8WelcomeMessage[] = "ANT SLAVE DEMO";
   u8 au8Instructions[] = "B0 toggles radio";
   AntAssignChannelInfoType sAntSetupData;
   
   /* Set a message up on the LCD. Delay is required to let the clear commmand send*/
   LCDCommand(LCD_CLEAR_CMD);
   LCDMessage(LINE1_START_ADDR, au8WelcomeMessage);
   LCDMessage(LINE2_START_ADDR, au8Instructions);
   
   /* Start with LED0 in RED state = channel is not configured */
   LedOn(RED);
   UserApp1_StateMachine = UserApp1SM_Master_or_Slave;
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
   
   sAntSetupData.AntNetwork                          =  ANT_NETWORK_DEFAULT; 
   for(u8 i = 0; i < ANT_NETWORK_NUMBER_BYTES; i++)
   {
      sAntSetupData.AntNetworkKey[i] = ANT_DEFAULT_NETWORK_KEY;
   }
   
   /* Attempt to queue the ANT channel setup */
   if(AntAssignChannel(&sAntSetupData))
   {
      /* Channel is configured, so change Led to yellow*/
     LedOff(RED);
     LedOn(YELLOW);
     UserApp1_StateMachine = UserApp1SM_WaitChannelAssign;
   }
  // else
   //{
      /* The task isn't properly intialized, so shut down and don't run */
     //LedBlink(RED, LED_4HZ);
     //UserApp1_StateMachine = UserApp1SM_Error;
   //}

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


/*----------------------------------------------------------------------------------------------------------------------
Function AntInit()

Description:

Requires:

Promises:
*/
static void AntInit(void)
{
  sChannelInfo.AntChannel                   = ANT_CHANNEL_USERAPP;
  sChannelInfo.AntChannelPeriodLo           = ANT_CHANNEL_PERIOD_LO_USERAPP;
  sChannelInfo.AntChannelPeriodHi           = ANT_CHANNEL_PERIOD_HI_USERAPP;
  sChannelInfo.AntDeviceIdLo                = ANT_DEVICEID_LO_USERAPP;
  sChannelInfo.AntDeviceIdHi                = ANT_DEVICEID_HI_USERAPP;
  sChannelInfo.AntDeviceType                = ANT_DEVICE_TYPE_USERAPP;
  sChannelInfo.AntTransmissionType          = ANT_TRANSMISSION_TYPE_USERAPP;
  sChannelInfo.AntFrequency                 = ANT_FREQUENCY_USERAPP;
  sChannelInfo.AntTxPower                   = ANT_TX_POWER_USERAPP;
  sChannelInfo.AntNetwork                   = ANT_NETWORK_DEFAULT;
  
  sChannelInfo.AntFlags                     = 1;
  
  for(u8 i = 0; i < ANT_NETWORK_NUMBER_BYTES; i++)
  {
    sChannelInfo.AntNetworkKey[i] = ANT_DEFAULT_NETWORK_KEY;
  }
} /* end AntInit */

/*----------------------------------------------------------------------------------------------------------------------
Function AntSlaveConfig()

Description:

Requires:

Promises:
*/
static void AntSlaveConfig(void)
{
  //TODO
  AntInit();
  sChannelInfo.AntChannelType = CHANNEL_TYPE_SLAVE;
  if(AntAssignChannel(&sChannelInfo))
  {
    UserApp1_u32Timeout++;
    UserApp1_StateMachine = UserApp1SM_WaitChannelAssign;//nathan's said:   UserApp1_StateMachine = UserApp1SM_ANT_ChannelAssign;
  }
} /* end AntSlaveConfig */


/*--------------------------------------------------------------------------------------------------------------------*/
/* Private functions                                                                                                  */
/*--------------------------------------------------------------------------------------------------------------------*/


/**********************************************************************************************************************
State Machine Function Definitions
**********************************************************************************************************************/

/*-------------------------------------------------------------------------------------------------------------------*/
/* Wait for ANT channel assignment */
static void UserApp1SM_WaitChannelAssign()
{

   LedOn(PURPLE);
  if(AntRadioStatusChannel(ANT_CHANNEL_USERAPP)  == ANT_CONFIGURED)
  {
     /* Channel assignment is successful, so open channel and procede to idle state */
    AntOpenChannelNumber(ANT_CHANNEL_USERAPP);
    LedOff(GREEN);
    UserApp1_StateMachine = UserApp1SM_Idle;
  }
  
  /* Watch for time out */
  if(IsTimeUp(&UserApp1_u32Timeout, 5000))
  {
    //  DebugPrintf(UserApp1_au8MessageFail);
      UserApp1_StateMachine = UserApp1SM_Error;
  }
}/* end UserApp1SM_AntChannelAssign */

/*-------------------------------------------------------------------------------------------------------------------*/

/* Wait for ??? */

static void UserApp1SM_Master_or_Slave (void)
{
  if(WasButtonPressed(BUTTON0))
  {
    LedOn(GREEN);
    ButtonAcknowledge(BUTTON0);
    AntSlaveConfig();
  }
  if(WasButtonPressed(BUTTON1))
  {
    ButtonAcknowledge(BUTTON1);
    //AntMasterConfig();
  }
}

/*-------------------------------------------------------------------------------------------------------------------*/

/* Wait for ??? */
static void UserApp1SM_Idle(void)
{
  /* Look at BUTTON 0 to open channel */
  if(WasButtonPressed(BUTTON0))
  {
    /* Got the button so complete one-time actions before the next state */
    ButtonAcknowledge(BUTTON0);
    
    /* Queue open channel and change LED0 from yellow ot blinking green
        to indicate channel is opening*/
    AntOpenChannelNumber(ANT_CHANNEL_USERAPP);
    LedOff(YELLOW);
    LedBlink(GREEN, LED_2HZ);
    
    /* Set timer and advance states */
    UserApp1_u32Timeout = G_u32SystemTime1ms;
    UserApp1_StateMachine = UserApp1SM_WaitChannelOpen;
    
  }
} /* end UserApp1SM_Idle() */
    

/*-------------------------------------------------------------------------------------------------------------------*/

/* Wait for */
static void UserApp1SM_WaitChannelOpen(void)
{
  /* Monitor the channel status to check if channel is opened */
  if(AntRadioStatusChannel(ANT_CHANNEL_USERAPP) == ANT_OPEN)
  {
    LedOn(GREEN);
    UserApp1_StateMachine = UserApp1SM_ChannelOpen;
  }
  
  /* Check for timeout */
  if(IsTimeUp(&UserApp1_u32Timeout, TIMEOUT_VALUE))
  {
    AntCloseChannelNumber(ANT_CHANNEL_USERAPP);
    LedOff(GREEN);
    LedOn(YELLOW);
    UserApp1_StateMachine = UserApp1SM_Idle;
  }
} /* end UserAppSM_WaitChannelOpen() */

/*-------------------------------------------------------------------------------------------------------------------*/

/* Wait for */
static void UserApp1SM_ChannelOpen(void)
{
  /* Check for BUTTON0 to close channel */
  if(WasButtonPressed(BUTTON0))
  {
    /* Got the button, so complete one-time actions befor next state */
    ButtonAcknowledge(BUTTON0);
    
    /* Queue close channel, intilize the u8LastState variable 
    and change LED to blinking green to indicate channel is closing */
    AntCloseChannelNumber(ANT_CHANNEL_USERAPP);
    u8LastState = 0xff;
    LedOff(YELLOW);
    LedOff(BLUE);
    LedBlink(GREEN, LED_2HZ);
    
    /* Set timer and advance states */
    UserApp1_u32Timeout = G_u32SystemTime1ms;
    UserApp1_StateMachine = UserApp1SM_WaitChannelClose;
  }  /* end if (WasButtonPressed(BUTTON0)) */
  
  
  /* A slave channel can close on its own, so explicity check channle status */
  if(AntRadioStatusChannel(ANT_CHANNEL_USERAPP) != ANT_OPEN)
  {
    LedBlink(GREEN, LED_2HZ);
    LedOff(BLUE);
    u8LastState = 0xff;
    
    UserApp1_u32Timeout = G_u32SystemTime1ms;
    UserApp1_StateMachine = UserApp1SM_WaitChannelClose;
  } /* end if(AntRadioStatusChannel(ANT_CHANNEL_USERAPP) != ANT_OPEN) */
  
  
  /* Always check for ANT messages */
  if(AntReadAppMessageBuffer())
  {
    /* New data message: check what it is */
    if(G_eAntApiCurrentMessageClass == ANT_DATA)
    {
      UserApp1_u32DataMsgCount++;
      
      /* We are synced with a device, blue is solid*/
      LedOff(GREEN);
      LedOn(BLUE);
      
      /* Check if the new data is the same as the old data
         and update as we go*/
      bGotNewData = FALSE;
      for(u8 i = 0; i < ANT_APPLICATION_MESSAGE_BYTES; i++)
      {
        if(G_au8AntApiCurrentMessageBytes[i] != au8LastAntData[i])
        {
          bGotNewData = TRUE;
          au8LastAntData[i] = G_au8AntApiCurrentMessageBytes[i];
        }
      }
        if(bGotNewData)
        {
          /* We got new data: show on LCD */
          LCDClearChars(LINE2_START_ADDR, 20);
          LCDMessage(LINE2_START_ADDR, au8DataContent);
          
          /* Update our local message counter and send the message back */
          au8TestMessage[7]++;
          if(au8TestMessage[7] == 0)
          {
            au8TestMessage[6]++;
            if(au8TestMessage[6] == 0)
            {
              au8TestMessage[5]++;
            }
          }
          AntQueueBroadcastMessage(ANT_CHANNEL_USERAPP, au8TestMessage);
        }/* end if (bGotNewData)*/
    }/* end if(G_eAntApiCurrentMessageClass == ANT_DATA) */
    
    
    
    
    else if(G_eAntApiCurrentMessageClass == ANT_TICK)
    {
      UserApp1_u32TickMsgCount++;
      
      /* Look a the TICK contents to check event code and respond only 
         if it's different */
      if(u8LastState != G_au8AntApiCurrentMessageBytes [ANT_TICK_MSG_EVENT_CODE_INDEX])
      {
        /* The state changed so update u8LastState and queue a debg message */
        u8LastState = G_au8AntApiCurrentMessageBytes[ANT_TICK_MSG_EVENT_CODE_INDEX];
        au8TickMessage[6] = HexToASCIICharUpper(u8LastState);
        
        DebugPrintf(au8TickMessage);
        
        /* Parse u8LastState to update LED status*/
        switch(u8LastState)
        {
          
          /* If we are paired but missing messages, blue blinks */
          case EVENT_RX_FAIL:
          {
              LedOff(GREEN);
              LedBlink(BLUE, LED_2HZ);
              break;
          }
          
          /* If we drop to search, LED is green */
          case EVENT_RX_FAIL_GO_TO_SEARCH:
          {
              LedOff(BLUE);
              LedOn(GREEN);
              break;
          }
          
          /* If the search times out, the channel should automatically close */
          case EVENT_RX_SEARCH_TIMEOUT:
          {
            DebugPrintf("Search timeout\r\n");
            break;
          }
          
          default:
           {
              DebugPrintf("Search timeout\r\n");
              break;
           }  
        } /* end switch (G_au8AntApiCurrentMessageBytes)*/
      } /* end if (u8LastState ...) */
    } /* end else if(G_eAntApiCurrentMessageClass == ANT_TICK) */
  } /* end AntReadAppMessagesBuffer()*/

} /* end UserAppSM_ChannelOpen() */

/*-------------------------------------------------------------------------------------------------------------------*/

/* Wait for */
static void UserApp1SM_WaitChannelClose(void)
{
  /* Monitor the channel status to check if channel is closed */
  if(AntRadioStatusChannel(ANT_CHANNEL_USERAPP)  == ANT_CLOSED)
  {
    LedOff(GREEN);
    LedOn(YELLOW);
    UserApp1_StateMachine = UserApp1SM_Idle;
  }
  
  /* Check for timeout */
  if(IsTimeUp(&UserApp1_u32Timeout, TIMEOUT_VALUE))
  {
    LedOff(GREEN);
    LedOff(YELLOW);
    LedBlink(RED, LED_4HZ);
    
    UserApp1_StateMachine = UserApp1SM_Error;
  }
} /* end UserAppSM_WaitChannelClose*/

/*-------------------------------------------------------------------------------------------------------------------*/
/* Handle an error */
static void UserApp1SM_Error(void)          
{
  
} /* end UserApp1SM_Error() */



/*--------------------------------------------------------------------------------------------------------------------*/
/* End of File                                                                                                        */
/*--------------------------------------------------------------------------------------------------------------------*/
