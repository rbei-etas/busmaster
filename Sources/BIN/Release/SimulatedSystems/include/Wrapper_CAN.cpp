/*******************************************************************************
  Project       :  CAN-Monitor                                      
  FileName      :  wrapper.c                                          
  Description   :  
                   
  $Log:   Y:/EMC2/Projects/CAN_Monitor_Maintenance/Archives/Source/Misc/GCC/Wrapper.c_v  $
 * 
 *    Rev 1.9   15 Sep 2010 17:26:42   rac2kor
 *  
 * 
 *    Rev 1.8   27 Dec 2006 16:12:14   amb2kor
 * Changes for Global variable support: Multiple database support.
 * 
 *    Rev 1.7   28 Feb 2006 16:09:28   amb2kor
 * Changes due to ST: CRH0031.
 * Performance issue is resolved.
 * 
 *    Rev 1.6   02 Feb 2006 19:55:38   amb2kor
 * Changes due to CRH0031 ( Multinode Support )
 * 
 *    Rev 1.5   21 Jul 2005 19:05:22   amb2kor
 * changes due to Multi Channel concept. Trace is modified and
 * New API to get version information is added.
 * 
 *    Rev 1.4   Aug 07 2004 07:48:08   amb2kor
 * Changes due to CRH0026: After UT.
 * 
 *    Rev 1.3   03 Nov 2003 18:22:58   amb2kor
 * review comment incorporated.
 * 
 *    Rev 1.2   30 Oct 2003 11:01:30   amb2kor
 * Changes due to CRH0011:before code review.
 * 
 *    Rev 1.1   25 Apr 2003 08:51:42   amb2kor
 * Taken after branch
 * 
 *    Rev 1.0.1.2   16 Apr 2003 15:58:24   amb2kor
 * syntext error in StartTimer Function corrected
 * 
 *    Rev 1.0.1.1   03 Apr 2003 12:20:48   amb2kor
 * prototype of SendMsg function is changed to accept single structure.
 * 
 *    Rev 1.0.1.0   17 Mar 2003 17:46:52   amb2kor
 * New interface function added as per requirment 
 * of CRH0002
 * 
 *    Rev 1.0   14 Feb 2003 14:34:36   amb2kor
 * Base Version : Latest version From CAN-Monitor2.x project
 * Version Label in CAN-Monitor2.x : CAN-Monitor2.0 EVAL/CMRL
 * 
 * 

  Author(s)     :  Amarnath Shastry
  Date Created  :  20/03/2002                                             
  Modified By   :  Ravikumar Patil  on 04.03.03
*******************************************************************************/
#include <WINDOWS.h>
#include "Wrapper_CAN.h"

HMODULE h_ModuleHandle;
static APPFUNCPROC sg_SendMsgFunc = NULL;

/******************************************************************************
    Function Name    :  vSetSendMsgProcAddress
    Input(s)         :  APPFUNCPROC which is a pointer to a global function
    Output           :  None
    Functionality    :  This function is used to pass a pointer to a global 
                        function which is called by the wrapper function.
    Member of        :  None (Global function)
    Friend of        :  None
    Author(s)        :  Amarnath Shastry
    Date Created     :  20-Feb-2002
    Modifications    :  
******************************************************************************/
void vSetSendMsgProcAddress(APPFUNCPROC pAppFunc)
{
    sg_SendMsgFunc = pAppFunc;
}

/******************************************************************************
    Function Name    :  SendMsg
    Input(s)         :  message ID, data length, Data
    Output           :  Returns 1->success, 0->Failure
    Functionality    :  This is a interface\wrapper function and user can 
                        call this function to send a message to CAN bus
    Member of        :  None (Global function)
    Friend of        :  None
    Author(s)        :  Ravikumar Patil
    Date Created     :  20-Feb-2003
    Modifications    :  Amitesh Bharti, 27.03.2003, function parameter is changed
******************************************************************************/
UINT SendMsg(STCAN_MSG sTxMsg)
{
    UINT nResult = ERR_INVALID_ADDRESS;

    if (sg_SendMsgFunc != NULL)
    {
        nResult = (*sg_SendMsgFunc)(&sTxMsg,h_ModuleHandle);
    }

    return nResult;
}

static LOGENABLE sg_EnableLoggingFunc = NULL;

/******************************************************************************
    Function Name    :  vSetEnableLoggingProcAddress
    Input(s)         :  LOGENABLE which is a pointer to a global function
    Output           :  None
    Functionality    :  This function is used to pass a pointer to a global 
                        function which is called by the wrapper function.
    Member of        :  None (Global function)
    Friend of        :  None
    Author(s)        :  Ravikumar Patil
    Date Created     :  20-Feb-2003
    Modifications    :  
******************************************************************************/
void vSetEnableLoggingProcAddress(LOGENABLE pEnableLoggingFunc)
{
    sg_EnableLoggingFunc = pEnableLoggingFunc ;
}

static LOGENABLE sg_DisableLoggingFunc = NULL;
/******************************************************************************
    Function Name    :  vSetDisableLoggingProcAddress
    Input(s)         :  LOGENABLE which is a pointer to a global function
    Output           :  None
    Functionality    :  This function is used to pass a pointer to a global 
                        function which is called by the wrapper function.
    Member of        :  None (Global function)
    Friend of        :  None
    Author(s)        :  Ravikumar Patil
    Date Created     :  20-Feb-2003
    Modifications    :  
******************************************************************************/
void vSetDisableLoggingProcAddress(LOGENABLE pDisableLoggingFunc)
{
    sg_DisableLoggingFunc = pDisableLoggingFunc ;
}

/******************************************************************************
    Function Name    :  EnableLogging
    Input(s)         :  None
    Output           :  Returns 1->success, 0->Failure
    Functionality    :  This is a interface\wrapper function and user can 
                        call this function to Start\Enable logging
    Member of        :  None (Global function)
    Friend of        :  None
    Author(s)        :  Ravikumar Patil
    Date Created     :  20-Feb-2003
    Modifications    :  
******************************************************************************/
UINT EnableLogging()
{
    UINT nResult = ERR_INVALID_ADDRESS;

    if (sg_EnableLoggingFunc != NULL)
    {
        nResult = (*sg_EnableLoggingFunc)(1) ;
    }

    return nResult;
}

/******************************************************************************
    Function Name    :  DisableLogging
    Input(s)         :  None
    Output           :  Returns 1->success, 0->Failure
    Functionality    :  This is a interface\wrapper function and user can 
                        call this function to Stop\Disable logging
    Member of        :  None (Global function)
    Friend of        :  None
    Author(s)        :  Ravikumar Patil
    Date Created     :  20-Feb-2003
    Modifications    :  
******************************************************************************/
UINT DisableLogging()
{
    UINT nResult = ERR_INVALID_ADDRESS;

    if (sg_DisableLoggingFunc != NULL)
    {
        nResult = (*sg_DisableLoggingFunc)(0);
    }
    return nResult;
}



static WRITETOLOGFILE sg_WriteToLogFile = NULL;

/******************************************************************************
    Function Name    :  vSetWriteToLogFileProcAddress
    Input(s)         :  WRITETOLOGFILE which is a pointer to a global function
    Output           :  None
    Functionality    :  This function is used to pass a pointer to a global 
                        function which is called by the wrapper function.
    Member of        :  None (Global function)
    Friend of        :  None
    Author(s)        :  Ravikumar Patil
    Date Created     :  21-Feb-2003
    Modifications    :  
******************************************************************************/
void vSetWriteToLogFileProcAddress(WRITETOLOGFILE pLogFileFunc)
{
    sg_WriteToLogFile = pLogFileFunc ;
}

/******************************************************************************
    Function Name    :  WriteToLogFile
    Input(s)         :  Formatted string to be Logged to file
    Output           :  Returns 1->success, 0->Failure
    Functionality    :  This is a interface\wrapper function and user can 
                        call this function to log a string to file
    Member of        :  None (Global function)
    Friend of        :  None
    Author(s)        :  Ravikumar Patil
    Date Created     :  21-Feb-2003
    Modifications    :  
******************************************************************************/
UINT WriteToLogFile(char* OutStr)
{
    UINT nResult = ERR_INVALID_ADDRESS;

    if (sg_WriteToLogFile != NULL && OutStr != NULL)
    {
        nResult = (*sg_WriteToLogFile)(OutStr);
    }

    return nResult;
}



static WRITETOTRACE sg_WriteToTrace = NULL;

/******************************************************************************
    Function Name    :  vSetTraceProcAddress
    Input(s)         :  WRITETOTRACE which is a pointer to a global function
    Output           :  None
    Functionality    :  This function is used to pass a pointer to a global 
                        function which is called by the wrapper function.
    Member of        :  None (Global function)
    Friend of        :  None
    Author(s)        :  Ravikumar Patil
    Date Created     :  24-Feb-2003
    Modifications    :  
******************************************************************************/
void vSetTraceProcAddress(WRITETOTRACE pTraceFunc)
{
    sg_WriteToTrace = pTraceFunc ;
}


/******************************************************************************
    Function Name    :  Trace
    Input(s)         :  Formatted string to be Logged to file
    Output           :  Returns 1->success, 0->Failure
    Functionality    :  This is a interface\wrapper function and user can 
                        call this function to write a formatted string to Trace window
    Member of        :  None (Global function)
    Friend of        :  None
    Author(s)        :  Ravikumar Patil
    Date Created     :  24-Feb-2003
    Modifications    :  Amitesh Bharti,
                        31.10.2003, Review comment incorporated.
						Arun Bellatti
						02-06-2005,Take variable arguments
******************************************************************************/
UINT Trace(char* OutStr,...)
{
	int ibufflength ;
	UINT nResult ;
	char ucbuffer[1024] ;
	va_list arglist ;
	
	va_start(arglist,OutStr);
	vsprintf(ucbuffer, OutStr,arglist);

	nResult = ERR_INVALID_ADDRESS;

    if (sg_WriteToTrace != NULL && OutStr != NULL)
    {
        nResult = (*sg_WriteToTrace)(ucbuffer);
    }
    return nResult;
}


static RESETCONTROLLER sg_ResetController = NULL;

/******************************************************************************
    Function Name    :  vSetResetControllerProcAddress
    Input(s)         :  RESETCONTROLLER which is a pointer to a global function
    Output           :  None
    Functionality    :  This function is used to pass a pointer to a global 
                        function which is called by the wrapper function.
    Member of        :  None (Global function)
    Friend of        :  None
    Author(s)        :  Ravikumar Patil
    Date Created     :  03-Mar-2003
    Modifications    :  
******************************************************************************/
void vSetResetControllerProcAddress(RESETCONTROLLER pResetFunc)
{
    sg_ResetController = pResetFunc ;
}

/******************************************************************************
    Function Name    :  ResetController
    Input(s)         :  bHardwareReset : TRUE ( Hardware Reset ) or
                                         FALSE( Software Reset )
    Output           :  None
    Functionality    :  This is a interface\wrapper function and user can 
                        call this function to ResetController
    Member of        :  None (Global function)
    Friend of        :  None
    Author(s)        :  Ravikumar Patil
    Date Created     :  04-Mar-2003
    Modifications    :  Amitesh Bharti, 02.08.2004, added a parameter to 
                        indicate if hardware reset to be given or software
                        reset.
******************************************************************************/
void ResetController(BOOL bHardwareReset)
{
    if (sg_ResetController != NULL)
    {
        (*sg_ResetController)(bHardwareReset) ;
    }
}


static CONTROLLERMODE sg_ActiveMode = NULL;


/******************************************************************************
    Function Name    :  vSetSetActiveModeProcAddress
    Input(s)         :  CONTROLLERMODE which is a pointer to a global function
    Output           :  None
    Functionality    :  This function is used to pass a pointer to a global 
                        function which is called by the wrapper function.
    Member of        :  None (Global function)
    Friend of        :  None
    Author(s)        :  Ravikumar Patil
    Date Created     :  04-Mar-2003
    Modifications    :  
******************************************************************************/
void vSetSetActiveModeProcAddress(CONTROLLERMODE pActiveModeFunc)
{
    sg_ActiveMode = pActiveModeFunc ;
}

static CONTROLLERMODE sg_PassiveMode = NULL;
/******************************************************************************
    Function Name    :  vSetSetPassiveModeProcAddress
    Input(s)         :  CONTROLLERMODE which is a pointer to a global function
    Output           :  None
    Functionality    :  This function is used to pass a pointer to a global 
                        function which is called by the wrapper function.
    Member of        :  None (Global function)
    Friend of        :  None
    Author(s)        :  Ravikumar Patil
    Date Created     :  04-Mar-2003
    Modifications    :  
******************************************************************************/
void vSetSetPassiveModeProcAddress(CONTROLLERMODE pPassiveModeFunc)
{
    sg_PassiveMode = pPassiveModeFunc ;
}

/******************************************************************************
    Function Name    :  SetPassiveMode
    Input(s)         :  None
    Output           :  returns 1->Success, 0->Failure
    Functionality    :  This is a interface\wrapper function and user can 
                        call this function to change the controller mode
    Member of        :  None (Global function)
    Friend of        :  None
    Author(s)        :  Ravikumar Patil
    Date Created     :  04-Mar-2003
    Modifications    :  
******************************************************************************/
UINT SetPassiveMode()
{
    UINT nResult = ERR_INVALID_ADDRESS;

    if (sg_PassiveMode != NULL)
    {
        nResult = (*sg_PassiveMode)(FALSE);
    }
    return nResult;
}


/******************************************************************************
    Function Name    :  SetActiveMode
    Input(s)         :  None
    Output           :  returns 1->Success, 0->Failure
    Functionality    :  This is a interface\wrapper function and user can 
                        call this function to change the controller mode
    Member of        :  None (Global function)
    Friend of        :  None
    Author(s)        :  Ravikumar Patil
    Date Created     :  05-Mar-2003
    Modifications    :  
******************************************************************************/
UINT SetActiveMode()
{
    UINT nResult = ERR_INVALID_ADDRESS;

    if (sg_ActiveMode != NULL)
    {
        nResult = (*sg_ActiveMode)(TRUE);
    }
    return nResult;
}


static GOONLINEOFFLINE sg_GoOffline = NULL;

/******************************************************************************
    Function Name    :  vSetGoOfflineProcAddress
    Input(s)         :  GOONLINEOFFLINE which is a pointer to a global function
    Output           :  None
    Functionality    :  This function is used to pass a pointer to a global 
                        function which is called by the wrapper function.
    Member of        :  None (Global function)
    Friend of        :  None
    Author(s)        :  Ravikumar Patil
    Date Created     :  05-Mar-2003
    Modifications    :  
******************************************************************************/
void vSetGoOfflineProcAddress(GOONLINEOFFLINE pGoOffline)
{
    sg_GoOffline = pGoOffline ;
}

static GOONLINEOFFLINE sg_GoOnline = NULL;
/******************************************************************************
    Function Name    :  vSetGoOnlineProcAddress
    Input(s)         :  GOONLINEOFFLINE which is a pointer to a global function
    Output           :  None
    Functionality    :  This function is used to pass a pointer to a global 
                        function which is called by the wrapper function.
    Member of        :  None (Global function)
    Friend of        :  None
    Author(s)        :  Ravikumar Patil
    Date Created     :  05-Mar-2003
    Modifications    :  
******************************************************************************/
void vSetGoOnlineProcAddress(GOONLINEOFFLINE pGoOnline)
{
    sg_GoOnline = pGoOnline;
}

/******************************************************************************
    Function Name    :  GoOnline
    Input(s)         :  None
    Output           :  returns 1->Success, 0->Failure
    Functionality    :  This is a interface\wrapper function and user can 
                        call this function to Activate Handlers
    Member of        :  None (Global function)
    Friend of        :  None
    Author(s)        :  Ravikumar Patil
    Date Created     :  05-Mar-2003
    Modifications    :  
******************************************************************************/
UINT GoOnline()
{
    UINT nResult = ERR_INVALID_ADDRESS;

    if (sg_GoOnline != NULL)
    {
        nResult = (*sg_GoOnline)(TRUE,h_ModuleHandle);
    }
    return nResult;
}


/******************************************************************************
    Function Name    :  GoOffline
    Input(s)         :  None
    Output           :  returns 1->Success, 0->Failure
    Functionality    :  This is a interface\wrapper function and user can 
                        call this function to Deactivate Handlers
    Member of        :  None (Global function)
    Friend of        :  None
    Author(s)        :  Ravikumar Patil
    Date Created     :  05-Mar-2003
    Modifications    :  
******************************************************************************/
UINT GoOffline()
{
    UINT nResult = ERR_INVALID_ADDRESS;

    if (sg_GoOffline != NULL)
    {
        nResult = (*sg_GoOffline)(FALSE,h_ModuleHandle);
    }
    return nResult;
}


static DISCONNECT sg_Disconnect = NULL;
static DISCONNECT sg_Connect = NULL;

/******************************************************************************
    Function Name    :  vSetDisconnectProcAddress
    Input(s)         :  DISCONNECT which is a pointer to a global function
    Output           :  None
    Functionality    :  This function is used to pass a pointer to a global 
                        function which is called by the wrapper function.
    Member of        :  None (Global function)
    Friend of        :  None
    Author(s)        :  Ravikumar Patil
    Date Created     :  06-Mar-2003
    Modifications    :  
******************************************************************************/
void vSetDisconnectProcAddress(DISCONNECT pDisconnect)
{
    sg_Disconnect = pDisconnect ;
}
/******************************************************************************
    Function Name    :  vSetConnectProcAddress
    Input(s)         :  DISCONNECT which is a pointer to a global function
    Output           :  None
    Functionality    :  This function is used to pass a pointer to a global 
                        function which is called by the wrapper function.
    Member of        :  None (Global function)
    Friend of        :  None
    Author(s)        :  Ravikumar Patil
    Date Created     :  06-Mar-2003
    Modifications    :  
******************************************************************************/
void vSetConnectProcAddress(DISCONNECT pConnect)
{
    sg_Connect = pConnect;
}

/******************************************************************************
    Function Name    :  Disconnect
    Input(s)         :  None
    Output           :  returns 1->Success, 0->Failure
    Functionality    :  This is a interface\wrapper function and user can 
                        call this function to Disconnect
    Member of        :  None (Global function)
    Friend of        :  None
    Author(s)        :  Ravikumar Patil
    Date Created     :  06-Mar-2003
    Modifications    :  
******************************************************************************/
UINT Disconnect()
{
    UINT nResult = ERR_INVALID_ADDRESS;

    if (sg_Disconnect != NULL)
    {
        nResult = (*sg_Disconnect)(FALSE);
    }
    return nResult;
}
/******************************************************************************
    Function Name    :  Disconnect
    Input(s)         :  None
    Output           :  returns 1->Success, 0->Failure
    Functionality    :  This is a interface\wrapper function and user can 
                        call this function to Disconnect
    Member of        :  None (Global function)
    Friend of        :  None
    Author(s)        :  Ravikumar Patil
    Date Created     :  06-Mar-2003
    Modifications    :  
******************************************************************************/
UINT Connect()
{
    UINT nResult = ERR_INVALID_ADDRESS;

    if (sg_Connect != NULL)
    {
        nResult = (*sg_Connect)(TRUE);
    }
    return nResult;
}



static STARTSTOPTIMER sg_StopTimer = NULL;

/******************************************************************************
    Function Name    :  vSetStopTimerProcAddress
    Input(s)         :  STARTSTOPTIMER which is a pointer to a global function
    Output           :  None
    Functionality    :  This function is used to pass a pointer to a global 
                        function which is called by the wrapper function.
    Member of        :  None (Global function)
    Friend of        :  None
    Author(s)        :  Ravikumar Patil
    Date Created     :  10-Mar-2003
    Modifications    :  
******************************************************************************/
void vSetStopTimerProcAddress(STARTSTOPTIMER pStopTimer)
{
    sg_StopTimer = pStopTimer ;
}

static STARTSTOPTIMER sg_StartTimer = NULL;
/******************************************************************************
    Function Name    :  vSetStartTimerProcAddress
    Input(s)         :  STARTSTOPTIMER which is a pointer to a global function
    Output           :  None
    Functionality    :  This function is used to pass a pointer to a global 
                        function which is called by the wrapper function.
    Member of        :  None (Global function)
    Friend of        :  None
    Author(s)        :  Ravikumar Patil
    Date Created     :  10-Mar-2003
    Modifications    :  
******************************************************************************/
void vSetStartTimerProcAddress(STARTSTOPTIMER pStartTimer)
{
    sg_StartTimer = pStartTimer ;
}
/******************************************************************************
    Function Name    :  StartTimer
    Input(s)         :  Timer name and type
    Output           :  returns 1->Success, 0->Failure
    Functionality    :  This is a interface\wrapper function and user can 
                        call this function to Start a timer
    Member of        :  None (Global function)
    Friend of        :  None
    Author(s)        :  Ravikumar Patil
    Date Created     :  10-Mar-2003
    Modifications    :  Amitesh Bharti,
                        31.10.2003, Review comment incorporated.
******************************************************************************/
UINT StartTimer(char* TimerName, UINT type)
{
    UINT nResult = ERR_INVALID_ADDRESS;

    if ( (type == 0) || (type == 1) )
    {
        if (sg_StartTimer != NULL && TimerName != NULL)
        {
            nResult = (*sg_StartTimer)(TimerName,type,TRUE,h_ModuleHandle);
        }
    }

    return nResult;
}


/******************************************************************************
    Function Name    :  StopTimer
    Input(s)         :  Timer name
    Output           :  returns 1->Success, 0->Failure
    Functionality    :  This is a interface\wrapper function and user can 
                        call this function to Start a timer
    Member of        :  None (Global function)
    Friend of        :  None
    Author(s)        :  Ravikumar Patil
    Date Created     :  12-Mar-2003
    Modifications    :  Amitesh Bharti,
                        31.10.2003, Review comment incorporated.
******************************************************************************/
UINT StopTimer(char* TimerName)
{
    UINT nResult = ERR_INVALID_ADDRESS;

    if (sg_StopTimer != NULL && TimerName != NULL)
    {
        nResult = (*sg_StopTimer)(TimerName,0,FALSE,h_ModuleHandle);
    }
    return nResult;
}

static SETTIMERVAL sg_SetTimerVal = NULL;

/******************************************************************************
    Function Name    :  vSetTimerValProcAddress
    Input(s)         :  SETTIMERVAL which is a pointer to a global function
    Output           :  None
    Functionality    :  This function is used to pass a pointer to a global 
                        function which is called by the wrapper function.
    Member of        :  None (Global function)
    Friend of        :  None
    Author(s)        :  Amitesh Bharti
    Date Created     :  28-10-2003
    Modifications    :  
******************************************************************************/
void vSetSetTimerValProcAddress(SETTIMERVAL pSetTimerVal)
{
     sg_SetTimerVal = pSetTimerVal ;
}


/******************************************************************************
    Function Name    :  SetTimerVal
    Input(s)         :  Timer name, Timer Value
    Output           :  returns 1->Success, 0->Failure
    Functionality    :  This is a interface\wrapper function and user can 
                        call this function to Change the timer value and Start 
                        it in monoshot mode.
    Member of        :  None (Global function)
    Friend of        :  None
    Author(s)        :  Amitesh Bharti
    Date Created     :  29-10-2003
    Modifications    :  Amitesh Bharti,
                        31.10.2003, Review comment incorporated.
******************************************************************************/
BOOL SetTimerVal(char* TimerName, UINT TimerVal)
{
    BOOL bResult = FALSE;
    if ( TimerVal >0)
    {
        if (sg_SetTimerVal != NULL && TimerName !=NULL )
        {
            bResult = (*sg_SetTimerVal)(TimerName, TimerVal,h_ModuleHandle);
        }
    }
    return bResult;
}

static ENABLEDISABLEMSGHANDLERS sg_MsgHandler = NULL;

/******************************************************************************
    Function Name    :  vSetEnableMsgHandlersProcAddress
    Input(s)         :  ENABLEDISABLEMSGHANDLER which is a pointer to a global 
                        function
    Output           :  None
    Functionality    :  This function is used to pass a pointer to a global 
                        function which is called by the wrapper function.
    Member of        :  None (Global function)
    Friend of        :  None
    Author(s)        :  Amitesh Bharti
    Date Created     :  14-07-2004
    Modifications    :  
******************************************************************************/
void vSetEnableMsgHandlersProcAddress(ENABLEDISABLEMSGHANDLERS pMsgHandlers)
{
     sg_MsgHandler = pMsgHandlers ;
}


/******************************************************************************
    Function Name    :  EnableMsgHandlers
    Input(s)         :  BOOL : TRUE or FALSE
    Output           :  returns 1->Success, 0->Failure
    Functionality    :  This is a interface\wrapper function. User can 
                        call this function to enable or disable message handlers 
                        execution.
    Member of        :  None (Global function)
    Friend of        :  None
    Author(s)        :  Amitesh Bharti
    Date Created     :  14-07-2004
    Modifications    :  
******************************************************************************/
BOOL EnableMsgHandlers(BOOL Enable)
{
    BOOL bResult = FALSE;
    if (sg_MsgHandler != NULL )
    {
        bResult = (*sg_MsgHandler)(Enable,h_ModuleHandle);
    }
    return bResult;
}
static ENABLEDISABLEKEYHANDLERS sg_KeyHandler = NULL;

/******************************************************************************
    Function Name    :  vSetEnableKeyHandlersProcAddress
    Input(s)         :  ENABLEDISABLEKEYHANDLER which is a pointer to a global 
                        function
    Output           :  None
    Functionality    :  This function is used to pass a pointer to a global 
                        function which is called by the wrapper function.
    Member of        :  None (Global function)
    Friend of        :  None
    Author(s)        :  Amitesh Bharti
    Date Created     :  14-07-2004
    Modifications    :  
******************************************************************************/
void vSetEnableKeyHandlersProcAddress(ENABLEDISABLEKEYHANDLERS pKeyHandlers)
{
     sg_KeyHandler = pKeyHandlers ;
}


/******************************************************************************
    Function Name    :  EnableKeyHandlers
    Input(s)         :  BOOL : TRUE or FALSE
    Output           :  returns 1->Success, 0->Failure
    Functionality    :  This is a interface\wrapper function. User can 
                        call this function to enable or disable key handlers 
                        execution.
    Member of        :  None (Global function)
    Friend of        :  None
    Author(s)        :  Amitesh Bharti
    Date Created     :  14-07-2004
    Modifications    :  
******************************************************************************/
BOOL EnableKeyHandlers(BOOL Enable)
{
    BOOL bResult = FALSE;
    if (sg_KeyHandler != NULL )
    {
        bResult = (*sg_KeyHandler)(Enable,h_ModuleHandle);
    }
    return bResult;
}

static ENABLEDISABLEERRORHANDLERS sg_ErrorHandler = NULL;

/******************************************************************************
    Function Name    :  vSetEnableErrorHandlersProcAddress
    Input(s)         :  ENABLEDISABLEERRORHANDLER which is a pointer to a global 
                        function
    Output           :  None
    Functionality    :  This function is used to pass a pointer to a global 
                        function which is called by the wrapper function.
    Member of        :  None (Global function)
    Friend of        :  None
    Author(s)        :  Amitesh Bharti
    Date Created     :  14-07-2004
    Modifications    :  
******************************************************************************/
void vSetEnableErrorHandlersProcAddress(ENABLEDISABLEERRORHANDLERS pErrorHandlers)
{
     sg_ErrorHandler = pErrorHandlers ;
}


/******************************************************************************
    Function Name    :  EnableErrorHandlers
    Input(s)         :  BOOL : TRUE or FALSE
    Output           :  returns 1->Success, 0->Failure
    Functionality    :  This is a interface\wrapper function. User can 
                        call this function to enable or disable error handlers 
                        execution.
    Member of        :  None (Global function)
    Friend of        :  None
    Author(s)        :  Amitesh Bharti
    Date Created     :  14-07-2004
    Modifications    :  
******************************************************************************/
BOOL EnableErrorHandlers(BOOL Enable)
{
    BOOL bResult = FALSE;
    if (sg_ErrorHandler != NULL )
    {
        bResult = (*sg_ErrorHandler)(Enable,h_ModuleHandle);
    }
    return bResult;
}


/******************************************************************************
    Function Name    :  GetProgramVersion
    Input(s)         :  BOOL : TRUE or FALSE
    Output           :  returns 1->Success, 0->Failure
    Functionality    :  This function is used to get the version number. 
						Both Major and Minor version Numbers.
    Member of        :  None (Global function)
    Friend of        :  None
    Author(s)        :  Arun Bellatti
    Date Created     :  01-06-2005
    Modifications    :  
******************************************************************************/
BOOL bGetProgramVersion(int* pnMajorVersion, int* pnMinorVersion,
						HMODULE hModuleHandle )
{
	h_ModuleHandle=hModuleHandle;
	if(pnMajorVersion == NULL && pnMinorVersion == NULL )
		return FALSE ;
	else
	{
		*pnMajorVersion = 1 ;
		*pnMinorVersion = 0 ;
		return TRUE ;
	}

}


static DLLMSGTXONOFF sg_OnOffTxMsg = NULL;

/******************************************************************************
    Function Name    :  vSetEnableDisableMsgTxProcAddress
    Input(s)         : 
    Output           :  
    Functionality    :  
    Member of        :  
    Friend of        : 
    Author(s)        :  Anish
    Date Created     :  05/01/06
    Modifications    :  
******************************************************************************/
void vSetEnableDisableMsgTxProcAddress(DLLMSGTXONOFF pMsgTxFunc)
{
    sg_OnOffTxMsg = pMsgTxFunc ;
}


/******************************************************************************
    Function Name    :  EnableDisableMsgTx
    Input(s)         : 
    Output           :  
    Functionality    :  
    Member of        :  
    Friend of        : 
    Author(s)        :  Anish
    Date Created     :  05/01/06
    Modifications    :    
******************************************************************************/
BOOL EnableDisableMsgTx(BOOL Enable)
{
    BOOL bResult = FALSE;

    if (sg_OnOffTxMsg != NULL)
    {
        bResult = (*sg_OnOffTxMsg)(Enable,h_ModuleHandle) ;
    }

    return bResult;
}
static DLLGETNODEHANDLER sg_GetNodeHandler = NULL;
/******************************************************************************
    Function Name    :  vSetGetDllHandleProcAddress
    Input(s)         : 
    Output           :  
    Functionality    : This function is used to pass a pointer to a global 
                       function which is called by the wrapper function. 
    Member of        :  
    Friend of        : 
    Author(s)        :  Anish
    Date Created     :  20/12/06
    Modifications    :  
******************************************************************************/
void vSetGetDllHandleProcAddress(DLLGETNODEHANDLER pGetNodeHandler)
{
    sg_GetNodeHandler = pGetNodeHandler ;
}

/******************************************************************************
    Function Name    :  GetDllHandle
    Input(s)         : 
    Output           :  
    Functionality    : Provide handle of the node's Dll  
    Member of        :  
    Friend of        : 
    Author(s)        :  Anish
    Date Created     :  20/12/06
    Modifications    :    
******************************************************************************/
HMODULE GetDllHandle(char* Node)
{
    HMODULE h_NodeModule = NULL;

    if (sg_GetNodeHandler != NULL && Node != NULL)
    {
         h_NodeModule = (*sg_GetNodeHandler)(Node) ;
    }

    return h_NodeModule;
}

/******************************************************************************
    Function Name    :  vSetKeyPressed
    Input(s)         : 
    Output           :  
    Functionality    : Returns the ASCII value of Key pressed 
    Member of        :  
    Friend of        : 
    Author(s)        :  Ashwin
    Date Created     :  
    Modifications    :    
******************************************************************************/
static KEYPRESSED sg_KeyPressed = NULL;
void vSetKeyPressed(KEYPRESSED pKeyPressed)
{
	sg_KeyPressed = pKeyPressed;
}
int keypressed_CAPL()
{
	if( NULL != sg_KeyPressed )
	{
		sg_KeyPressed();
	}
}

/******************************************************************************
    Function Name    :  GetMessageName
    Input(s)         : 
    Output           :  
    Functionality    : Gives the message name 
    Member of        :  
    Friend of        : 
    Author(s)        :  Ashwin
    Date Created     :  3-5-2012
    Modifications    :    
******************************************************************************/
static GETMESSAGENAME sg_GetMessageName = NULL;
void vSetGetMessageName(GETMESSAGENAME pGetMessageName)
{
	sg_GetMessageName = pGetMessageName;
}
int getmessagename_CAPL(DWORD dID, DWORD dContext, char* pBuffer,DWORD dSize)
{
	if( NULL != sg_GetMessageName )
	{
		sg_GetMessageName(dID, dContext, pBuffer,dSize);
	}
}

/******************************************************************************
    Function Name    :  TimeNow
    Input(s)         : 
    Output           :  
    Functionality    : Gives the absolute time
    Member of        :  
    Friend of        : 
    Author(s)        :  Ashwin
    Date Created     :  3-5-2012
    Modifications    :    
******************************************************************************/
static TIMENOW sg_TimeNow = NULL;
void vSetTimeNow(TIMENOW pTimeNow)
{
	sg_TimeNow = pTimeNow;
}
int timenow_CAPL()
{
	if( NULL != sg_TimeNow )
	{
		sg_TimeNow();
	}
}

/******************************************************************************
    Function Name    :  GetFirstCANdbName
    Input(s)         :	cBuffer to store the name
						size of buffer
    Output           :  name of buffer
    Functionality    :	Gives the name of first CAN db associated
    Member of        :  
    Friend of        : 
    Author(s)        :  Ashwin. R. Uchil
    Date Created     :  9-5-2012
    Modifications    :    
******************************************************************************/
static GETFIRSTCANDBNAME gdGetFirstCANdbName = NULL;
void vSetGetFirstCANdbName(GETFIRSTCANDBNAME pGetFirstCANdbName)
{
	gdGetFirstCANdbName = pGetFirstCANdbName;
}
int getfirstcandbname_CAPL(char* cBuffer, DWORD size)
{
	if( NULL != gdGetFirstCANdbName )
	{
		gdGetFirstCANdbName(cBuffer, size);
	}
}