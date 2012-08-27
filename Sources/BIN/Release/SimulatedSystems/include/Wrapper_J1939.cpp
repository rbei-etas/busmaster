/*******************************************************************************
  Project       :  CAN-Monitor                                      
  FileName      :  Wrapper_J1939.c                                         
  Description   :  
                   
  $Log:   Y:/EMC2/Projects/CAN_Monitor_Maintenance/Archives/Source/Misc/GCC/Wrapper_J1939.c_v  $
 * 
 *    Rev 1.0   02 Mar 2011 17:37:12   rac2kor
 *  

  Author(s)     :  Amarnath Shastry
  Date Created  :  20/03/2002                                             
  Modified By   :  Ravikumar Patil  on 04.03.03
*******************************************************************************/
#include <WINDOWS.h>
#include <stdio.h>
#include "Wrapper_J1939.h"

HMODULE g_hModuleHandle;
static APPSENDPROC_J sg_SendMsgFunc = NULL;

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
void vSetSendMsgProcAddress(APPSENDPROC_J pAppFunc)
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
UINT SendMsg(STJ1939_MSG* psTxMsg)
{
    UINT nResult = ERR_INVALID_ADDRESS;

    if (sg_SendMsgFunc != NULL)
    {
        nResult = (*sg_SendMsgFunc)(psTxMsg, g_hModuleHandle);
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


static APPONLINEOFFLINE_J sg_GoOffline = NULL;

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
void vSetGoOfflineProcAddress(APPONLINEOFFLINE_J pGoOffline)
{
    sg_GoOffline = pGoOffline ;
}

static APPONLINEOFFLINE_J sg_GoOnline = NULL;
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
void vSetGoOnlineProcAddress(APPONLINEOFFLINE_J pGoOnline)
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
        nResult = (*sg_GoOnline)(TRUE, g_hModuleHandle);
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
        nResult = (*sg_GoOffline)(FALSE, g_hModuleHandle);
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
            nResult = (*sg_StartTimer)(TimerName,type,TRUE,g_hModuleHandle);
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
        nResult = (*sg_StopTimer)(TimerName,0,FALSE,g_hModuleHandle);
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
            bResult = (*sg_SetTimerVal)(TimerName, TimerVal,g_hModuleHandle);
        }
    }
    return bResult;
}
static ENABLEDISABLEALLHANDLERS sg_AllHandler = NULL;

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
void vSetEnableAllHandlersProcAddress(ENABLEDISABLEALLHANDLERS pAllHandlers)
{
     sg_AllHandler = pAllHandlers ;
}


/******************************************************************************
    Function Name    :  EnableAllHandlers
    Input(s)         :  BOOL : TRUE or FALSE
    Output           :  returns 1->Success, 0->Failure
    Functionality    :  This is a interface\wrapper function. User can 
                        call this function to enable or disable All handlers 
                        execution.
    Member of        :  None (Global function)
    Friend of        :  None
    Author(s)        :  
    Date Created     :  
    Modifications    :  
******************************************************************************/
BOOL EnableAllHandlers(BOOL Enable)
{
    BOOL bResult = FALSE;
    if (sg_AllHandler != NULL )
    {
        bResult = (*sg_AllHandler)(Enable, g_hModuleHandle);
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
        bResult = (*sg_MsgHandler)(Enable,g_hModuleHandle);
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
        bResult = (*sg_KeyHandler)(Enable,g_hModuleHandle);
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
        bResult = (*sg_ErrorHandler)(Enable,g_hModuleHandle);
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
	g_hModuleHandle=hModuleHandle;
	if(pnMajorVersion == NULL && pnMinorVersion == NULL )
		return FALSE ;
	else
	{
		*pnMajorVersion = 1 ;
		*pnMinorVersion = 0 ;
		return TRUE ;
	}

}


static MSGTXONOFF sg_OnOffTxMsg = NULL;

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
void vSetEnableDisableMsgTxProcAddress(MSGTXONOFF pMsgTxFunc)
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
        bResult = (*sg_OnOffTxMsg)(Enable,g_hModuleHandle) ;
    }

    return bResult;
}
static GETNODEHANDLER sg_GetNodeHandler = NULL;
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
void vSetGetDllHandleProcAddress(GETNODEHANDLER pGetNodeHandler)
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

// RequestPGN
APPREQUESTPGN_J sg_RequestPGN = NULL;
void vSetRequestPGNProcAddress(APPREQUESTPGN_J pRequestPGN)
{
    sg_RequestPGN = pRequestPGN;
}
/******************************************************************************
    Function Name    :  RequestPGN
    Input(s)         : 
    Output           :  
    Functionality    : 
    Member of        :  
    Friend of        : 
    Author(s)        :  Pradeep Kadoor
    Date Created     :  24/01/2011
    Modifications    :    
******************************************************************************/
UINT RequestPGN(UINT PGN, BYTE DestAdres, UINT Channel)
{
    UINT nResult = ERR_INVALID_ADDRESS;

    if (sg_RequestPGN != NULL)
    {
        nResult = (*sg_RequestPGN)(PGN, DestAdres, Channel, g_hModuleHandle);
    }
    return nResult;
}


// SendAckMsg
APPACKMSG_J sg_SendAckMsg = NULL;
void vSetSendAckMsgProcAddress(APPACKMSG_J pSendAckMsg)
{
    sg_SendAckMsg = pSendAckMsg;
}
/******************************************************************************
    Function Name    :  SendAckMsg
    Input(s)         : 
    Output           :  
    Functionality    : 
    Member of        :  
    Friend of        : 
    Author(s)        :  Pradeep Kadoor
    Date Created     :  24/01/2011
    Modifications    :    
******************************************************************************/

UINT SendAckMsg(UINT PGN, BYTE AckType, BYTE DestAdres, UINT Channel)
{
    UINT nResult = ERR_INVALID_ADDRESS;

    if (sg_SendAckMsg != NULL)
    {
        nResult = (*sg_SendAckMsg)(PGN, AckType, DestAdres, Channel, g_hModuleHandle);
    }
    return nResult;
}
// ClaimAddress
APPCLAIMADDRESS_J sg_ClaimAddress = NULL;
void vSetClaimAddressProcAddress(APPCLAIMADDRESS_J pClaimAddress)
{
    sg_ClaimAddress = pClaimAddress;
}
/******************************************************************************
    Function Name    :  ClaimAddress
    Input(s)         : 
    Output           :  
    Functionality    : 
    Member of        :  
    Friend of        : 
    Author(s)        :  Pradeep Kadoor
    Date Created     :  24/01/2011
    Modifications    :    
******************************************************************************/

UINT ClaimAddress(BYTE Address, UINT Channel)
{
    UINT nResult = ERR_INVALID_ADDRESS;

    if (sg_ClaimAddress != NULL)
    {
        nResult = (*sg_ClaimAddress)(Address, Channel, g_hModuleHandle);
    }
    return nResult;
}
// RequestAddress
APPRQSTADDRESS_J sg_RequestAddress = NULL;
void vSetRequestAddressProcAddress(APPRQSTADDRESS_J pRequestAddress)
{
    sg_RequestAddress = pRequestAddress;
}
/******************************************************************************
    Function Name    :  RequestAddress
    Input(s)         : 
    Output           :  
    Functionality    : 
    Member of        :  
    Friend of        : 
    Author(s)        :  Pradeep Kadoor
    Date Created     :  24/01/2011
    Modifications    :    
******************************************************************************/

UINT RequestAddress(UINT Channel)
{
    UINT nResult = ERR_INVALID_ADDRESS;

    if (sg_RequestAddress != NULL)
    {
        nResult = (*sg_RequestAddress)(Channel, g_hModuleHandle);
    }
    return nResult;
}
// SetTimeout
APPCMDADDRESS_J sg_CommandAddress = NULL;
void vSetCommandAddressProcAddress(APPCMDADDRESS_J pCommandAddress)
{
    sg_CommandAddress = pCommandAddress;
}
/******************************************************************************
    Function Name    :  CommandAddress
    Input(s)         : 
    Output           :  
    Functionality    : 
    Member of        :  
    Friend of        : 
    Author(s)        :  Pradeep Kadoor
    Date Created     :  24/01/2011
    Modifications    :    
******************************************************************************/

UINT CommandAddress(UINT64 EcuName, BYTE NewAdres, UINT Channel)
{
    UINT nResult = ERR_INVALID_ADDRESS;

    if (sg_CommandAddress != NULL)
    {
        nResult = (*sg_CommandAddress)(EcuName, NewAdres, Channel, g_hModuleHandle);
    }
    return nResult;
}
// SetTimeout
APPSETTIMEOUT_J sg_SetTimeout = NULL;
void vSetSetTimeoutProcAddress(APPSETTIMEOUT_J pSetTimeout)
{
    sg_SetTimeout = pSetTimeout;
}
/******************************************************************************
    Function Name    :  SetTimeout
    Input(s)         : 
    Output           :  
    Functionality    : 
    Member of        :  
    Friend of        : 
    Author(s)        :  Pradeep Kadoor
    Date Created     :  24/01/2011
    Modifications    :    
******************************************************************************/

UINT SetTimeout(BYTE TimeoutType, UINT TimeoutValue)
{
    UINT nResult = ERR_INVALID_ADDRESS;

    if (sg_SetTimeout != NULL)
    {
        nResult = (*sg_SetTimeout)(TimeoutType, TimeoutValue, g_hModuleHandle);
    }
    return nResult;
}