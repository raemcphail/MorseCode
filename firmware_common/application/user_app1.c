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

extern u32 G_u32AntApiCurrentMessageTimestamp;
extern AntApplicationMessageType G_eAntApiCurrentMessageClass;
extern u8 G_au8AntApiCurrentMessageBytes[ANT_APPLICATION_MESSAGE_BYTES];
extern AntExtendedDataType G_sAntApiCurrentMessageExtData;

AntAssignChannelInfoType        sChannelInfo;

/***********************************************************************************************************************
Global variable definitions with scope limited to this local application.
Variable names shall start with "UserApp1_" and be declared as static.
***********************************************************************************************************************/
static fnCode_type UserApp1_StateMachine;            /* The state machine function pointer */
static u32 UserApp1_u32Timeout;                      /* Timeout counter used across states */
static u8 au8TestMessage[] = {0, 0, 0, 0, 0, 0, 0};
AntAssignChannelInfoType UserApp1_sChannelInfo;

static u16 u16countTapTime;
static u16 u16countSpaceTime;
static u8 au8Taps[] = "";
//static u8 au8Message[] = "";
static u16 u16countTaps;
static u16 u16countLetter;
static u8  au8DataContent[] = {0, 0, 0, 0, 0, 0, 0, 0};
  
/**********************************************************************************************************************
Function Definitions
**********************************************************************************************************************/

/*--------------------------------------------------------------------------------------------------------------------*/
/* Public functions                                                                                                   */
/*--------------------------------------------------------------------------------------------------------------------*/
/*------------------------------------------------------------
Function: wasE
Description:
Determines if letter was E
*/
void wasE(void)
{
  if(au8Taps[0] == '.')
  {
    LCDMessage(LINE2_START_ADDR + u16countLetter, "E");
    au8TestMessage[u16countLetter] = 0x05;
    u16countLetter++;
  }
}
/* end of wasE*/

/*------------------------------------------------------------
Function: wasT
Description:
Determines if letter was T
*/
void wasT(void)
{
  if(au8Taps[0] == '-')
  {
    LCDMessage(LINE2_START_ADDR + u16countLetter, "T");
    au8TestMessage[u16countLetter] = 0x20;
    u16countLetter++;
  }
}
/* end of wasT*/

/*------------------------------------------------------------
Function: wasA
Description:
Determines if letter was A
*/
void wasA(void)
{
  if((au8Taps[0] == '.') && (au8Taps[1] == '-'))
  {
    LCDMessage(LINE2_START_ADDR + u16countLetter, "A");
    au8TestMessage[u16countLetter] = 0x01;
    u16countLetter++;
  }
}
/* end of wasA*/
/*------------------------------------------------------------
Function: wasI
Description:
Determines if letter was I
*/
void wasI(void)
{
  if((au8Taps[0] == '.') && (au8Taps[1] == '.'))
  {
    LCDMessage(LINE2_START_ADDR + u16countLetter, "I");
    au8TestMessage[u16countLetter] = 0x09;
    u16countLetter++;
  }
}
/* end of wasI*/

/*------------------------------------------------------------
Function: wasM
Description:
Determines if letter was M
*/
void wasM(void)
{
  if((au8Taps[0] == '-') && (au8Taps[1] == '-'))
  {
    LCDMessage(LINE2_START_ADDR + u16countLetter, "M");
    au8TestMessage[u16countLetter] = 0x13;
    u16countLetter++;
  }
}
/* end of wasm*/
/*------------------------------------------------------------
Function: wasN
Description:
Determines if letter was N
*/
void wasN(void)
{
  if((au8Taps[0] == '-') && (au8Taps[1] == '.'))
  {
    LCDMessage(LINE2_START_ADDR + u16countLetter, "N");
    au8TestMessage[u16countLetter] = 0x14;
    u16countLetter++;
  }
}
/* end of wasN*/
/*------------------------------------------------------------
Function: wasD
Description:
Determines if letter was D
*/
void wasD(void)
{
  if((au8Taps[0] == '-') && (au8Taps[1] == '.') && (au8Taps[2] == '.'))
  {
    LCDMessage(LINE2_START_ADDR + u16countLetter, "D");
    au8TestMessage[u16countLetter] = 0x04;
    u16countLetter++;
  }
}
/* end of wasD*/
/*------------------------------------------------------------
Function: wasG
Description:
Determines if letter was G
*/
void wasG(void)
{
  if((au8Taps[0] == '-') && (au8Taps[1] == '-') && (au8Taps[2] == '.'))
  {
    LCDMessage(LINE2_START_ADDR + u16countLetter, "G");
    au8TestMessage[u16countLetter] = 0x07;
    u16countLetter++;
  }
}
/* end of wasG*/
/*------------------------------------------------------------
Function: wasK
Description:
Determines if letter was K
*/
void wasK(void)
{
  if((au8Taps[0] == '-') && (au8Taps[1] == '.') && (au8Taps[2] == '-'))
  {
    LCDMessage(LINE2_START_ADDR + u16countLetter, "K");
    au8TestMessage[u16countLetter] = 0x11;
    u16countLetter++;
  }
}
/* end of wasK*/
/*------------------------------------------------------------
Function: wasO
Description:
Determines if letter was O
*/
void wasO(void)
{
  if((au8Taps[0] == '-') && (au8Taps[1] == '-') && (au8Taps[2] == '-'))
  {
    LCDMessage(LINE2_START_ADDR + u16countLetter, "O");
    au8TestMessage[u16countLetter] = 0x15;
    u16countLetter++;
  }
}
/* end of wasO*/
/*------------------------------------------------------------
Function: wasR
Description:
Determines if letter was R
*/
void wasR(void)
{
  if((au8Taps[0] == '.') && (au8Taps[1] == '-') && (au8Taps[2] == '.'))
  {
    LCDMessage(LINE2_START_ADDR + u16countLetter, "R");
    au8TestMessage[u16countLetter] = 0x18;
    u16countLetter++;
  }
}
/* end of wasR*/
/*------------------------------------------------------------
Function: wasS
Description:
Determines if letter was S
*/
void wasS(void)
{
  if((au8Taps[0] == '.') && (au8Taps[1] == '.') && (au8Taps[2] == '.'))
  {
    LCDMessage(LINE2_START_ADDR + u16countLetter, "S");
    au8TestMessage[u16countLetter] = 0x19;
    u16countLetter++;
  }
}
/* end of wasS*/
/*------------------------------------------------------------
Function: wasU
Description:
Determines if letter was U
*/
void wasU(void)
{
  if((au8Taps[0] == '.') && (au8Taps[1] == '.') && (au8Taps[2] == '-'))
  {
    LCDMessage(LINE2_START_ADDR + u16countLetter, "U");
    au8TestMessage[u16countLetter] = 0x21;
    u16countLetter++;
  }
}
/* end of wasU*/
/*------------------------------------------------------------
Function: wasW
Description:
Determines if letter was W
*/
void wasW(void)
{
  if((au8Taps[0] == '.') && (au8Taps[1] == '-') && (au8Taps[2] == '-'))
  {
    LCDMessage(LINE2_START_ADDR + u16countLetter, "W");
    au8TestMessage[u16countLetter] = 0x23;
    u16countLetter++;
  }
}
/* end of wasW*/
/*------------------------------------------------------------
Function: wasB
Description:
Determines if letter was B
*/
void wasB(void)
{
  if((au8Taps[0] == '-')&&(au8Taps[1] == '.')&&(au8Taps[2] == '.')&&(au8Taps[3] == '.'))
  {
    LCDMessage(LINE2_START_ADDR + u16countLetter, "B");
    au8TestMessage[u16countLetter] = 0x02;
    u16countLetter++;
  }
}
/* end of wasB*/
/*------------------------------------------------------------
Function: wasC
Description:
Determines if letter was C
*/
void wasC(void)
{
  if((au8Taps[0] == '-')&&(au8Taps[1] == '.')&&(au8Taps[2] == '-')&&(au8Taps[3] == '.'))
  {
    LCDMessage(LINE2_START_ADDR + u16countLetter, "C");
    au8TestMessage[u16countLetter] = 0x03;
    u16countLetter++;
  }
}
/* end of wasC*/
/*------------------------------------------------------------
Function: wasF
Description:
Determines if letter was F
*/
void wasF(void)
{
  if((au8Taps[0] == '.')&&(au8Taps[1] == '.')&&(au8Taps[2] == '-')&&(au8Taps[3] == '.'))
  {
    LCDMessage(LINE2_START_ADDR + u16countLetter, "F");
    au8TestMessage[u16countLetter] = 0x06;
    u16countLetter++;
  }
}
/* end of wasF*/
/*------------------------------------------------------------
Function: wasH
Description:
Determines if letter was H
*/
void wasH(void)
{
  if((au8Taps[0] == '.')&&(au8Taps[1] == '.')&&(au8Taps[2] == '.')&&(au8Taps[3] == '.'))
  {
    LCDMessage(LINE2_START_ADDR + u16countLetter, "H");
    au8TestMessage[u16countLetter] = 0x08;
    u16countLetter++;
  }
}
/* end of wasH*/
/*------------------------------------------------------------
Function: wasJ
Description:
Determines if letter was J
*/
void wasJ(void)
{
  if((au8Taps[0] == '.')&&(au8Taps[1] == '-')&&(au8Taps[2] == '-')&&(au8Taps[3] == '-'))
  {
    LCDMessage(LINE2_START_ADDR + u16countLetter, "J");
    au8TestMessage[u16countLetter] = 0x10;
    u16countLetter++;
  }
}
/* end of wasJ*/
/*------------------------------------------------------------
Function: wasL
Description:
Determines if letter was L
*/
void wasL(void)
{
  if((au8Taps[0] == '.')&&(au8Taps[1] == '-')&&(au8Taps[2] == '.')&&(au8Taps[3] == '.'))
  {
    LCDMessage(LINE2_START_ADDR + u16countLetter, "L");
    au8TestMessage[u16countLetter] = 0x12;
    u16countLetter++;
  }
}
/* end of wasL*/
/*------------------------------------------------------------
Function: wasP
Description:
Determines if letter was P
*/
void wasP(void)
{
  if((au8Taps[0] == '.')&&(au8Taps[1] == '-')&&(au8Taps[2] == '-')&&(au8Taps[3] == '.'))
  {
    LCDMessage(LINE2_START_ADDR + u16countLetter, "P");
    au8TestMessage[u16countLetter] = 0x16;
    u16countLetter++;
  }
}
/* end of wasP*/
/*------------------------------------------------------------
Function: wasQ
Description:
Determines if letter was Q
*/
void wasQ(void)
{
  if((au8Taps[0] == '-')&&(au8Taps[1] == '-')&&(au8Taps[2] == '.')&&(au8Taps[3] == '-'))
  {
    LCDMessage(LINE2_START_ADDR + u16countLetter, "Q");
    au8TestMessage[u16countLetter] = 0x17;
    u16countLetter++;
  }
}
/* end of wasQ*/
/*------------------------------------------------------------
Function: wasV
Description:
Determines if letter was V
*/
void wasV(void)
{
  if((au8Taps[0] == '.')&&(au8Taps[1] == '.')&&(au8Taps[2] == '.')&&(au8Taps[3] == '-'))
  {
    LCDMessage(LINE2_START_ADDR + u16countLetter, "V");
    au8TestMessage[u16countLetter] = 0x22;
    u16countLetter++;
  }
}
/* end of wasV*/
/*------------------------------------------------------------
Function: wasX
Description:
Determines if letter was X
*/
void wasX(void)
{
  if((au8Taps[0] == '-')&&(au8Taps[1] == '.')&&(au8Taps[2] == '.')&&(au8Taps[3] == '-'))
  {
    LCDMessage(LINE2_START_ADDR + u16countLetter, "X");
    au8TestMessage[u16countLetter] = 0x24;
    u16countLetter++;
  }
}
/* end of wasX*/
/*------------------------------------------------------------
Function: wasY
Description:
Determines if letter was Y
*/
void wasY(void)
{
  if((au8Taps[0] == '-')&&(au8Taps[1] == '.')&&(au8Taps[2] == '-')&&(au8Taps[3] == '-'))
  {
    LCDMessage(LINE2_START_ADDR + u16countLetter, "Y");
    au8TestMessage[u16countLetter] = 0x25;
    u16countLetter++;
  }
}
/* end of wasY*/
/*------------------------------------------------------------
Function: wasZ
Description:
Determines if letter was Z
*/
void wasZ(void)
{
  if((au8Taps[0] == '-')&&(au8Taps[1] == '-')&&(au8Taps[2] == '.')&&(au8Taps[3] == '.'))
  {
    LCDMessage(LINE2_START_ADDR + u16countLetter, "Z");
    au8TestMessage[u16countLetter] = 0x26;
    u16countLetter++;
  }
}
/* end of wasZ*/
/*------------------------------------------------------------
Function: wasShort
Description:
Determines if tap was short
*/
void wasShort(void)
{
  LedOn(GREEN);
  LCDMessage(LINE1_START_ADDR + u16countTaps, ".                   ");
  au8Taps[u16countTaps] = '.';
  u16countTaps++;
  if(u16countTaps >= 6)
  {
    u16countTaps = 0;
  }
}
/* end of wasShort*/

/*------------------------------------------------------------
Function: wasLong
Description:
Determines if tap was long
*/
void wasLong(void)
{
  LedOn(RED);
  LCDMessage(LINE1_START_ADDR + u16countTaps, "-                          ");
  au8Taps[u16countTaps] = '-';
  u16countTaps++;
  if(u16countTaps >=6)
  {
    u16countTaps = 0;
  }
}
/* end of wasLong*/

/*------------------------------------------------------------
Function: wasLetter
Description:
Determines if a letter was correctly entered
*/
void wasLetter(void)
{
  if(u16countTaps == 1)
  {
    wasT();
    wasE();
  }
  if(u16countTaps == 2)
  {
    wasA();
    wasI();
    wasM();
    wasN();
  }
  if(u16countTaps == 3)
  {
    wasD();
    wasG();
    wasK();
    wasO();
    wasR();
    wasS();
    wasU();
    wasW();
  }
   if(u16countTaps == 4)
  {
    wasB();
    wasC();
    wasF();
    wasH();
    wasJ();
    wasL();
    wasP();
    wasQ();
    wasV();
    wasX();
    wasY();
    wasZ();
  }
  
  if(u16countLetter >= 8)
  {
    u16countLetter = 0;
  }
  u16countTaps = 0;
  
}
/* end of wasLetter*/
/*------------------------------------------------------------
Function: deleteLetter
Description:deletes letter that was entered most recently

*/
void deleteLetter(void)
{
  if(u16countLetter == 0)
  {
    u16countLetter = 7;
  }
  else
  {
    u16countLetter--;
  }
  LCDMessage(LINE2_START_ADDR + u16countLetter, " ");
  au8TestMessage[u16countLetter] = 0x00;
  au8Taps[u16countTaps] = ' ';
}
/* end of deleteLetter*/

/*------------------------------------------------------------
Function: checkTime
Description:

*/
void checkTime(void)
{
   if(u16countSpaceTime>=1000)
  {
    LedOn(WHITE);
    wasLetter();
  }
  else
  {
    LedOff(WHITE);
  }
  
  if(u16countSpaceTime>=300)
  {
    LedOn(PURPLE);
    LedOff(GREEN);
    LedOff(RED);
  }
  else
  {
    LedOff(PURPLE);
  }
  
  if(u16countTapTime>=300)
  {
    LedOn(CYAN);
  }
  else
  {
    LedOff(CYAN);
  }
  
  if(u16countTapTime>=1)
  {
    LedOn(BLUE);
  }
  else
  {
    LedOff(BLUE);
  }
  
}
/* end of checkTime*/
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
    /* Set a message up on LCD. Delay is required to let the clear command send */
    LCDCommand(LCD_CLEAR_CMD);
    for(u32 i = 0; i < 10000; i++);
    LCDMessage(LINE1_START_ADDR, "Button 3 for Master");
    LCDMessage(LINE2_START_ADDR, "Button 2 for Slave");
    UserApp1_StateMachine = UserApp1SM_Master_or_Slave;
    
    /* Set counter to 0 to start. Counts number of longs or shorts*/
    static u16 u16countTaps;
   /* Set counter to 0 to start. Counts number of letters*/
    static u16 u16countLetter; 
   /* Set counter to 0 to start. Counts to 300ms*/
   u16countTapTime = 0;
   /* Set counter to 0 to start. Counts to 300ms*/
   u16countSpaceTime = 0;
  /* If good initialization, set state to Idle */
  if( 1 )
  {
  //  UserApp1_StateMachine = UserApp1SM_Idle;
  }
  else
  {
    /* The task isn't properly initialized, so shut it down and don't run */
    UserApp1_StateMachine = UserApp1SM_Error;
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

/*----------------------------------------------------------------------------------------------------------------------
Function AntMasterConfig()

Description:

Requires:


Promises:

*/
void AntMasterConfig(void)
{
  LCDCommand(LCD_CLEAR_CMD);
  
  /* Configure ANT for this application */
    UserApp1_sChannelInfo.AntChannel                  = ANT_CHANNEL_USERAPP;
    UserApp1_sChannelInfo.AntChannelType              = ANT_CHANNEL_TYPE_USERAPP;
    UserApp1_sChannelInfo.AntChannelPeriodLo          = ANT_CHANNEL_PERIOD_LO_USERAPP;
    UserApp1_sChannelInfo.AntChannelPeriodHi          = ANT_CHANNEL_PERIOD_HI_USERAPP;
    
    UserApp1_sChannelInfo.AntDeviceIdLo               = ANT_DEVICEID_LO_USERAPP;
    UserApp1_sChannelInfo.AntDeviceIdHi               = ANT_DEVICEID_HI_USERAPP;
    UserApp1_sChannelInfo.AntDeviceType               = ANT_DEVICE_TYPE_USERAPP;
    UserApp1_sChannelInfo.AntTransmissionType         = ANT_TRANSMISSION_TYPE_USERAPP;
    UserApp1_sChannelInfo.AntFrequency                = ANT_FREQUENCY_USERAPP;
    UserApp1_sChannelInfo.AntTxPower                  = ANT_TX_POWER_USERAPP;
    
    UserApp1_sChannelInfo.AntNetwork                  = ANT_NETWORK_DEFAULT;
    for(u8 i = 0; i < ANT_NETWORK_NUMBER_BYTES; i++)
    {
      UserApp1_sChannelInfo.AntNetworkKey[i]          = ANT_DEFAULT_NETWORK_KEY;
    }
    
    /* Attempt to queue the ant channel setup */
    if(AntAssignChannel(&UserApp1_sChannelInfo))
    {
      LedOn(PURPLE);
      UserApp1_u32Timeout = G_u32SystemTime1ms;
      UserApp1_StateMachine = UserApp1SM_AntChannelAssignMaster;
    }
    else
    {
      /* The task ins't properly initialized so shut down and don't run*/
      //DebugPrinf(UseraApp1_au8MessageFail)
      UserApp1_StateMachine = UserApp1SM_Error;
    }
} /* end AntMasterConfig */

/*----------------------------------------------------------------------------------------------------------------------
Function AntSlaveConfig()

Description:

Requires:


Promises:

*/
void AntSlaveConfig(void)
{
  LCDCommand(LCD_CLEAR_CMD);
  
  /* Configure ANT for this application */
    sChannelInfo.AntChannel                  = ANT_CHANNEL_USERAPP;
    sChannelInfo.AntChannelType              = CHANNEL_TYPE_SLAVE;
    sChannelInfo.AntChannelPeriodLo          = ANT_CHANNEL_PERIOD_LO_USERAPP;
    sChannelInfo.AntChannelPeriodHi          = ANT_CHANNEL_PERIOD_HI_USERAPP;
    
    sChannelInfo.AntDeviceIdLo               = ANT_DEVICEID_LO_USERAPP;
    sChannelInfo.AntDeviceIdHi               = ANT_DEVICEID_HI_USERAPP;
    sChannelInfo.AntDeviceType               = ANT_DEVICE_TYPE_USERAPP;
    sChannelInfo.AntTransmissionType         = ANT_TRANSMISSION_TYPE_USERAPP;
    sChannelInfo.AntFrequency                = ANT_FREQUENCY_USERAPP;
    sChannelInfo.AntTxPower                  = ANT_TX_POWER_USERAPP;
    
    sChannelInfo.AntFlags                    = 1;
    
    for(u8 i = 0; i < ANT_NETWORK_NUMBER_BYTES; i++)
    {
      sChannelInfo.AntNetworkKey[i]          = ANT_DEFAULT_NETWORK_KEY;
    }
    
    /* Attempt to queue the ant channel setup */
    if(AntAssignChannel(&sChannelInfo))
    {
      UserApp1_u32Timeout = G_u32SystemTime1ms;
      UserApp1_StateMachine = UserApp1SM_AntChannelAssignSlave;
      if(UserApp1_u32Timeout == 50000)
      {
        LedBlink(RED, LED_4HZ);
        UserApp1_StateMachine = UserApp1SM_Error;
      }
    }
    else
    {
      /* The task ins't properly initialized so shut down and don't run*/
      //DebugPrinf(UseraApp1_au8MessageFail)
      LedOn(RED);
      UserApp1_StateMachine = UserApp1SM_Error;
    }
} /* end AntMasterConfig */

/*--------------------------------------------------------------------------------------------------------------------*/
/* Private functions                                                                                                  */
/*--------------------------------------------------------------------------------------------------------------------*/


/**********************************************************************************************************************
State Machine Function Definitions
**********************************************************************************************************************/

/*-------------------------------------------------------------------------------------------------------------------*/
/* Wait for ??? */
static void UserApp1SM_MasterIdle(void)
{ 
  
  /* start AntReadAppMessageBuffer */
  if(AntReadAppMessageBuffer())
  {
    /* new message from ANT task: check what it is */
    if(G_eAntApiCurrentMessageClass == ANT_DATA)
    {
      
    }
    else if(G_eAntApiCurrentMessageClass == ANT_TICK)
    {
      
    }

    AntQueueBroadcastMessage(ANT_CHANNEL_USERAPP, au8TestMessage);
  }/* end AntReadAppMessageBuffer */
  
  
  checkTime();
  if(IsButtonPressed(BUTTON0))
  {
    u16countSpaceTime = 0;
    u16countTapTime++;
    LedOff(WHITE);
    LedOff(PURPLE);
  }
  else
  {
    u16countSpaceTime++;
    if(u16countTapTime>=300)
    {
      wasLong();
    }
    else if(u16countTapTime>=1)
    {
      wasShort();
    }
    u16countTapTime = 0;
  }
  
  if(WasButtonPressed(BUTTON1))
  {
    ButtonAcknowledge(BUTTON1);
    deleteLetter();
  }
  if(WasButtonPressed(BUTTON2))
  {
    ButtonAcknowledge(BUTTON2);
    LCDMessage(LINE2_START_ADDR + u16countLetter, " ");
    au8TestMessage[u16countLetter] = 0x00;
    u16countLetter++;
  }
  if(WasButtonPressed(BUTTON3))
  {
    LCDCommand(LCD_CLEAR_CMD);
    for(int i = 0; i < 8; i++)
    {
      au8TestMessage[i] = 0x00;
    }
    u16countTaps = 0;
    u16countLetter = 0; 
    ButtonAcknowledge(BUTTON3);
  }
   
} /* end UserApp1SM_Idle() */
    
/*-------------------------------------------------------------------------------------------------------------------*/
/* Wait for ??? */
static void UserApp1SM_Master_or_Slave()
{
  if(WasButtonPressed(BUTTON3))
  {
    ButtonAcknowledge(BUTTON3);
    AntMasterConfig();
  }
  if(WasButtonPressed(BUTTON2))
  {
    ButtonAcknowledge(BUTTON2);
    AntSlaveConfig();
  }
}
/*-------------------------------------------------------------------------------------------------------------------*/
/* Wait for ANT channel assignment */
static void UserApp1SM_AntChannelAssignMaster()
{
  if(AntRadioStatusChannel(ANT_CHANNEL_USERAPP) == ANT_CONFIGURED)
  {
    /* Channel Assignment is successful so open channel and procede to idle state */
    AntOpenChannelNumber (ANT_CHANNEL_USERAPP);
    UserApp1_StateMachine = UserApp1SM_MasterIdle;
  }
  
  /* Watch for time out */
  if(IsTimeUp(&UserApp1_u32Timeout, 3000))
  {
    //DebugPrintf(UserApp1_au8MessageFail)
    UserApp1_StateMachine = UserApp1SM_Error;
  }
}/* end UserApp1SM_AntChannelAssign() */

/*-------------------------------------------------------------------------------------------------------------------*/
/* Wait for ANT channel assignment */
static void UserApp1SM_AntChannelAssignSlave()
{
  if(AntRadioStatusChannel(ANT_CHANNEL_USERAPP)== ANT_CONFIGURED)
  {
    /* Channel assignment is sucessful, so open channel and procede*/
    AntOpenChannelNumber(ANT_CHANNEL_USERAPP);
    UserApp1_StateMachine = UserApp1SM_SlaveIdle;
  }
  
  /* Watch for time out */
  if(IsTimeUp(&UserApp1_u32Timeout, 50000))
  {
    UserApp1_StateMachine = UserApp1SM_Error;
  }
}/* end UserApp1SM_AntChannelAssignSlave() */

/*-------------------------------------------------------------------------------------------------------------------*/
/* Wait for ANT channel assignment */
static void UserApp1SM_SlaveIdle()
{
 // LCDMessage(LINE1_START_ADDR, "Pressed Button 0 to");
 // LCDMessage(LINE2_START_ADDR, "Connect Channel");
  /* Look at BUTTON0 to open channel */
  if(WasButtonPressed(BUTTON0))
  {
    /* Got the button so complete one-time actions before next state */
    ButtonAcknowledge(BUTTON0);
    
    /* Queue open channel and change LED0 from yellow to blinking green to indicate channel opening */
    AntOpenChannelNumber(ANT_CHANNEL_USERAPP);
    LedOff(YELLOW);
    LedBlink(GREEN, LED_2HZ);
    
    /* Set timer and advance states */
    UserApp1_u32Timeout = G_u32SystemTime1ms;
    UserApp1_StateMachine = UserApp1SM_WaitChannelOpen;
  }
}/* end UserApp1SM_SlaveIdle() */


/*-------------------------------------------------------------------------------------------------------------------*/
/* Wait for ANT channel assignment */
static void UserApp1SM_WaitChannelOpen()
{
  //LCDCommand(LCD_CLEAR_CMD);
  LCDMessage(LINE1_START_ADDR, "Waiting");
  
  
  /* Monitor the channel status to check if channel is opened */
  if(AntRadioStatusChannel(ANT_CHANNEL_USERAPP) == ANT_OPEN)
  {
    LedOn(GREEN);
    UserApp1_StateMachine = UserApp1SM_ChannelOpen;
  }
  
  /* Check for timeout */
  if(IsTimeUp(&UserApp1_u32Timeout, 500000))
  {
    AntCloseChannelNumber(ANT_CHANNEL_USERAPP);
    LedOff(GREEN);
    LedOn(YELLOW);
    UserApp1_StateMachine = UserApp1SM_SlaveIdle;
  }
}/* end UserApp1SM_WaitChannelOpen() */

/*-------------------------------------------------------------------------------------------------------------------*/
/* Wait for ANT channel assignment */
static void UserApp1SM_ChannelOpen()
{
  LedOn(PURPLE);
  /* Check for BUTTON0 to close chnnel */
  if(WasButtonPressed(BUTTON0))
  {
    /* Got the button, so complete one-time actions before next state */
    ButtonAcknowledge(BUTTON0);
    
    /* Queue close channel, change LED to blinking green to indicate channel closing */
    AntCloseChannelNumber(ANT_CHANNEL_USERAPP);
    //u8LastState = 0xff
    LedOff(YELLOW);
    LedOff(BLUE);
    LedBlink(GREEN, LED_2HZ);
    
    /* Set timer and advance states */
    UserApp1_u32Timeout = G_u32SystemTime1ms;
   // UserApp1_StateMachine = UserApp1SM_WaitChannelClose;
  } /* end if (WasButtonPressed(BUTTON0)) */
  
  /* Always check for ANT messages */
  if(AntReadAppMessageBuffer())
  {
    /* New data message: check what it is */
    if(G_eAntApiCurrentMessageClass == ANT_DATA)
    {
      //UserApp1_u32DataMsgCount++;
      
      /* We synced with a device, blue is solid */
      LedOff(GREEN);
      LedOn(BLUE);
      
      /* Check if the data is a letter */
      for(u8 i = 0; i < 8; i++)
      {
        if(G_au8AntApiCurrentMessageBytes[i] == 0x01)
        {
          au8DataContent[i] = 'A';
        }
       else if(G_au8AntApiCurrentMessageBytes[i] == 0x02)
        {
          au8DataContent[i] = 'B';
        }
        else if(G_au8AntApiCurrentMessageBytes[i] == 0x03)
        {
          au8DataContent[i] = 'C';
        }
         else if(G_au8AntApiCurrentMessageBytes[i] == 0x04)
        {
          au8DataContent[i] = 'D';
        }
        else if(G_au8AntApiCurrentMessageBytes[i] == 0x05)
        {
          au8DataContent[i] = 'E';
        }
        else if(G_au8AntApiCurrentMessageBytes[i] == 0x06)
        {
          au8DataContent[i] = 'F';
        }
        else if(G_au8AntApiCurrentMessageBytes[i] == 0x07)
        {
          au8DataContent[i] = 'G';
        }
        else if(G_au8AntApiCurrentMessageBytes[i] == 0x08)
        {
          au8DataContent[i] = 'H';
        }
        else if(G_au8AntApiCurrentMessageBytes[i] == 0x09)
        {
          au8DataContent[i] = 'I';
        }
        else if(G_au8AntApiCurrentMessageBytes[i] == 0x10)
        {
          au8DataContent[i] = 'J';
        }
        else if(G_au8AntApiCurrentMessageBytes[i] == 0x11)
        {
          au8DataContent[i] = 'K';
        }
        else if(G_au8AntApiCurrentMessageBytes[i] == 0x12)
        {
          au8DataContent[i] = 'L';
        }
        else if(G_au8AntApiCurrentMessageBytes[i] == 0x13)
        {
          au8DataContent[i] = 'M';
        }
        else if(G_au8AntApiCurrentMessageBytes[i] == 0x14)
        {
          au8DataContent[i] = 'N';
        }
        else if(G_au8AntApiCurrentMessageBytes[i] == 0x15)
        {
          au8DataContent[i] = 'O';
        }
        else if(G_au8AntApiCurrentMessageBytes[i] == 0x16)
        {
          au8DataContent[i] = 'P';
        }
        else if(G_au8AntApiCurrentMessageBytes[i] == 0x17)
        {
          au8DataContent[i] = 'Q';
        }
        else if(G_au8AntApiCurrentMessageBytes[i] == 0x18)
        {
          au8DataContent[i] = 'R';
        }
        else if(G_au8AntApiCurrentMessageBytes[i] == 0x19)
        {
          au8DataContent[i] = 'S';
        }  
        else if(G_au8AntApiCurrentMessageBytes[i] == 0x20)
        {
          au8DataContent[i] = 'T';
        }
        else if(G_au8AntApiCurrentMessageBytes[i] == 0x21)
        {
          au8DataContent[i] = 'U';
        }
        else if(G_au8AntApiCurrentMessageBytes[i] == 0x22)
        {
          au8DataContent[i] = 'V';
        }
        else if(G_au8AntApiCurrentMessageBytes[i] == 0x23)
        {
          au8DataContent[i] = 'W';
        }
        else if(G_au8AntApiCurrentMessageBytes[i] == 0x24)
        {
          au8DataContent[i] = 'X';
        }
        else if(G_au8AntApiCurrentMessageBytes[i] == 0x25)
        {
          au8DataContent[i] = 'Y';
        }
        else if(G_au8AntApiCurrentMessageBytes[i] == 0x26)
        {
          au8DataContent[i] = 'Z';
        }
        else
        {
          au8DataContent[i] = ' ';
        }
      }
      
      LCDCommand(LCD_CLEAR_CMD);
      LCDMessage(LINE2_START_ADDR, au8DataContent);
     }
  }
}/* end UserApp1SM_WaitChannelOpen() */

/*-------------------------------------------------------------------------------------------------------------------*/
/* Handle an error */
static void UserApp1SM_Error(void)          
{
  
} /* end UserApp1SM_Error() */



/*--------------------------------------------------------------------------------------------------------------------*/
/* End of File                                                                                                        */
/*--------------------------------------------------------------------------------------------------------------------*/
