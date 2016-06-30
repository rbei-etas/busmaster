#include <WINDOWS.h>
#include "Wrapper_Common.h"

HMODULE g_hModuleHandle;

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
    {
        return FALSE ;
    }
    else
    {
        *pnMajorVersion = 2 ;
        *pnMinorVersion = 0 ;
        return TRUE ;
    }

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
        return sg_KeyPressed();
    }
    else
    {
        return 0;
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
unsigned int timenow_CAPL()
{
    if( NULL != sg_TimeNow )
    {
        return sg_TimeNow();
    }
    else
    {
        return 0;
    }
}