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

/***********************************************************************************************************************
Global variable definitions with scope limited to this local application.
Variable names shall start with "UserApp1_" and be declared as static.
***********************************************************************************************************************/
static fnCode_type UserApp1_StateMachine;            /* The state machine function pointer */
static u32 UserApp1_u32Timeout;                      /* Timeout counter used across states */
static u8 au8TestMessage[] = {0, 0, 0, 0, 0, 0, 0, 0};
AntAssignChannelInfoType UserApp1_sChannelInfo;

static u16 u16countTapTime;
static u16 u16countSpaceTime;
static u8 au8Taps[] = "";
static u8 au8Message[] = "";
static u16 u16countTaps;
static u16 u16countLetter; 
  
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
    au8Message[u16countLetter] = 'T';
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
    au8Message[u16countLetter] = 'A';
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
    au8Message[u16countLetter] = 'I';
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
    au8Message[u16countLetter] = 'M';
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
    au8Message[u16countLetter] = 'N';
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
    au8Message[u16countLetter] = 'D';
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
    au8Message[u16countLetter] = 'G';
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
    au8Message[u16countLetter] = 'K';
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
    au8Message[u16countLetter] = 'O';
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
    au8Message[u16countLetter] = 'R';
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
    au8Message[u16countLetter] = 'S';
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
    au8Message[u16countLetter] = 'U';
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
    au8Message[u16countLetter] = 'W';
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
    au8Message[u16countLetter] = 'B';
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
    au8Message[u16countLetter] = 'C';
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
    au8Message[u16countLetter] = 'F';
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
    au8Message[u16countLetter] = 'H';
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
    au8Message[u16countLetter] = 'J';
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
    au8Message[u16countLetter] = 'L';
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
    au8Message[u16countLetter] = 'P';
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
    au8Message[u16countLetter] = 'Q';
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
    au8Message[u16countLetter] = 'V';
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
    au8Message[u16countLetter] = 'X';
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
    au8Message[u16countLetter] = 'Y';
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
    au8Message[u16countLetter] = 'Z';
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
  if(u16countTaps >= 20)
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
  if(u16countTaps >= 20)
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
  
  if(u16countLetter >= 20)
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
  u16countLetter--;
  LCDMessage(LINE2_START_ADDR + u16countLetter, " ");
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
    u8 au8WelcomeMessage[] = "AntMaster";
    /* Set a message up on LCD. Delay is required to let the clear command send */
    LCDCommand(LCD_CLEAR_CMD);
    for(u32 i = 0; i < 10000; i++);
    LCDMessage(LINE1_START_ADDR, au8WelcomeMessage);
    
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
      UserApp1_u32Timeout = G_u32SystemTime1ms;
      UserApp1_StateMachine = UserApp1SM_AntChannelAssign;
    }
    else
    {
      /* The task ins't properly initialized so shut down and don't run*/
      //DebugPrinf(UseraApp1_au8MessageFail)
      UserApp1_StateMachine = UserApp1SM_Error;
    }
    /* Set counter to 0 to start. Counts number of longs or shorts*/
    static u16 u16countTaps;
   /* Set counter to 0 to start. Counts number of letters*/
    static u16 u16countLetter; 
   /* Intially clears the LCD*/
    LCDCommand(LCD_CLEAR_CMD);
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


/*--------------------------------------------------------------------------------------------------------------------*/
/* Private functions                                                                                                  */
/*--------------------------------------------------------------------------------------------------------------------*/


/**********************************************************************************************************************
State Machine Function Definitions
**********************************************************************************************************************/

/*-------------------------------------------------------------------------------------------------------------------*/
/* Wait for ??? */
static void UserApp1SM_Idle(void)
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
    au8Message[u16countLetter] = ' ';
    u16countLetter++;
  }
  if(WasButtonPressed(BUTTON3))
  {
    LCDCommand(LCD_CLEAR_CMD);
    u16countTaps = 0;
    u16countLetter = 0; 
    ButtonAcknowledge(BUTTON3);
  }
   
} /* end UserApp1SM_Idle() */
    

/*-------------------------------------------------------------------------------------------------------------------*/
/* Wait for ANT channel assignment */
static void UserApp1SM_AntChannelAssign()
{
  if(AntRadioStatusChannel(ANT_CHANNEL_USERAPP) == ANT_CONFIGURED)
  {
    /* Channel Assignment is successful so open channel and procede to idle state */
    AntOpenChannelNumber (ANT_CHANNEL_USERAPP);
    UserApp1_StateMachine = UserApp1SM_Idle;
  }
  
  /* Watch for time out */
  if(IsTimeUp(&UserApp1_u32Timeout, 3000))
  {
    //DebugPrintf(UserApp1_au8MessageFail)
    UserApp1_StateMachine = UserApp1SM_Error;
  }
}/* end UserApp1SM_AntChannelAssign() */

/*-------------------------------------------------------------------------------------------------------------------*/
/* Wait for ANT channel to be assigned */
static void UserApp1SM_SendAntMessage()
{
  
 if(IsButtonPressed(BUTTON0))
  {
   for(int i = 0; i < 8; i++)
    {
      au8TestMessage[i] = 0x77;
    }
  }
 else if(IsButtonPressed(BUTTON1))
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
  else if(IsButtonPressed(BUTTON2))
 {
     au8TestMessage[0] = 0x00;
     au8TestMessage[1] = 0x10;
     au8TestMessage[2] = 0x20;
     au8TestMessage[3] = 0x30;
     au8TestMessage[4] = 0x40;
     au8TestMessage[5] = 0x50;
     au8TestMessage[6] = 0x60;
     au8TestMessage[7] = 0x70;
     
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
    /* new message from ANT task: check what it is */
    if(G_eAntApiCurrentMessageClass == ANT_DATA)
    {
      
    }
    else if(G_eAntApiCurrentMessageClass == ANT_TICK)
    {
      
    }

    AntQueueBroadcastMessage(ANT_CHANNEL_USERAPP, au8TestMessage);
  }/* end AntReadAppMessageBuffer */
    
}/* end UserApp1SM_SendAntMessage() */
/*-------------------------------------------------------------------------------------------------------------------*/
/* Handle an error */
static void UserApp1SM_Error(void)          
{
  
} /* end UserApp1SM_Error() */



/*--------------------------------------------------------------------------------------------------------------------*/
/* End of File                                                                                                        */
/*--------------------------------------------------------------------------------------------------------------------*/
