/**********************************************************************************************************************
File: user_app1.h                                                                

----------------------------------------------------------------------------------------------------------------------
To start a new task using this user_app1 as a template:
1. Follow the instructions at the top of user_app1.c
2. Use ctrl-h to find and replace all instances of "user_app1" with "yournewtaskname"
3. Use ctrl-h to find and replace all instances of "UserApp1" with "YourNewTaskName"
4. Use ctrl-h to find and replace all instances of "USER_APP1" with "YOUR_NEW_TASK_NAME"
5. Add #include yournewtaskname.h" to configuration.h
6. Add/update any special configurations required in configuration.h (e.g. peripheral assignment and setup values)
7. Delete this text (between the dashed lines)
----------------------------------------------------------------------------------------------------------------------

Description:
Header file for user_app1.c

**********************************************************************************************************************/

#ifndef __USER_APP1_H
#define __USER_APP1_H

/**********************************************************************************************************************
Type Definitions
**********************************************************************************************************************/


/**********************************************************************************************************************
Constants / Definitions
**********************************************************************************************************************/
/* Required constants for ANT channel configuration */
#define ANT_CHANNEL_TYPE_USERAPP          CHANNEL_TYPE_MASTER       //Device is a Master
#define ANT_CHANNEL_USERAPP               ANT_CHANNEL_0             //Channel 0-7
#define ANT_CHANNEL_PERIOD_LO_USERAPP     (u8)0x00                  //Lo; 0x0001 - 0x7fff
#define ANT_CHANNEL_PERIOD_HI_USERAPP     (u8)0x20                  //HI; 0x0001 - 0x7fff
#define ANT_DEVICEID_LO_USERAPP           (u8)0x34                  // Device # Low byte
#define ANT_DEVICEID_HI_USERAPP           (u8)0x12                  // Device # High byte
#define ANT_DEVICE_TYPE_USERAPP           (u8)1                     // 1-255
#define ANT_TRANSMISSION_TYPE_USERAPP     (u8)1                     // 1-127; MSB is pairng
#define ANT_FREQUENCY_USERAPP             (u8)50                    // 2400MHz + 0 - 99 MHz
#define ANT_TX_POWER_USERAPP              RADIO_TX_POWER_4DBM       // Max tx Power

#define TIMEOUT_VALUE                     (u8) 300000000 
/**********************************************************************************************************************
Function Declarations
**********************************************************************************************************************/

/*--------------------------------------------------------------------------------------------------------------------*/
/* Public functions                                                                                                   */
/*--------------------------------------------------------------------------------------------------------------------*/
  void AntMasterConfig(void);
  void AntSlaveConfig(void);

  void wasShort(void);
  void wasLong(void);
  void checkTime(void);
  void wasLetter(void);
  void deleteLetter(void);

  void wasA(void);
  void wasB(void);
  void wasC(void);
  void wasD(void);
  void wasE(void);
  void wasF(void);
  void wasG(void);
  void wasH(void);
  void wasI(void);
  void wasJ(void);
  void wasK(void);
  void wasL(void);
  void wasM(void);
 
  void wasN(void);
  void wasO(void);
  void wasP(void);
  void wasQ(void);
  void wasR(void);
  void wasS(void);
  void wasT(void);
  void wasU(void);
  void wasV(void);
  void wasW(void);
  void wasX(void);
  void wasY(void);
  void wasZ(void);
  
/*--------------------------------------------------------------------------------------------------------------------*/
/* Protected functions                                                                                                */
/*--------------------------------------------------------------------------------------------------------------------*/
void UserApp1Initialize(void);
void UserApp1RunActiveState(void);


/*--------------------------------------------------------------------------------------------------------------------*/
/* Private functions                                                                                                  */
/*--------------------------------------------------------------------------------------------------------------------*/


/***********************************************************************************************************************
State Machine Declarations
***********************************************************************************************************************/
static void UserApp1SM_MasterIdle(void);    
static void UserApp1SM_Error(void);   
static void UserApp1SM_AntChannelAssignMaster(void);
static void UserApp1SM_SendAntMessage(void);

static void UserApp1SM_Master_or_Slave(void);

static void UserApp1SM_SlaveIdle(void);
static void UserApp1SM_AntChannelAssignSlave(void);
static void UserApp1SM_WaitChannelOpen(void);
static void UserApp1SM_ChannelOpen(void);
static void UserApp1SM_WaitChannelClose(void);

#endif /* __USER_APP1_H */


/*--------------------------------------------------------------------------------------------------------------------*/
/* End of File                                                                                                        */
/*--------------------------------------------------------------------------------------------------------------------*/
