/*******************************************************************************
  Project       :  BUSMASTER
  FileName      :  Wrapper_LIN.cpp
  Description   :
  Author(s)     :
  Date Created  :  12/12/2013
*******************************************************************************/
#include <WINDOWS.h>
#include "BMCANDefines.h"

extern HMODULE g_hModuleHandle;
static APPFUNCPROC sg_SendMsgFunc = NULL;

unsigned long* longAt(STCAN_MSG* sMsg,int nByte)
{
    if(nByte<(sMsg->dlc/4))
    {
        return  ((unsigned long*)(&(sMsg->data[nByte*4])));
    }
    else
    {
        return ((unsigned long*)(&(sMsg->data[0])));
    }
    return NULL;
}
unsigned short* wordAt(STCAN_MSG* sMsg,int nByte)
{
    if(nByte<(sMsg->dlc/2))
    {
        return  ((unsigned short*)(&(sMsg->data[nByte*2])));
    }
    else
    {
        return NULL;
    }
}
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
    //STCAN_MSG msgTx;
    if (sg_SendMsgFunc != NULL)
    {
        //copyST_BM_MSG_CAN(&msgTx, &sTxMsg);
        nResult = (*sg_SendMsgFunc)(&sTxMsg,g_hModuleHandle);
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
static DLLMSGTXONOFF sg_OnOffTxMsg = NULL;

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
        bResult = (*sg_OnOffTxMsg)(Enable,g_hModuleHandle) ;
    }

    return bResult;
}
static DLLGETNODEHANDLER sg_GetNodeHandler = NULL;


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
    return 0;
}


/******************************************************************************
    Function Name    :  GetFirstCANdbName
    Input(s)         :  cBuffer to store the name
                        size of buffer
    Output           :  name of buffer
    Functionality    :  Gives the name of first CAN db associated
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
    return 0;
}


void copyBM_ST_MSG_CAN(DBCanMsg* pBmMsg, STCAN_MSG* pUserMsg)
{
    //DBCanMsg* <- STCAN_MSG
    //Called When BUSMASTER Handler is called

    if(NULL != pBmMsg && NULL != pUserMsg)
    {
        pBmMsg->id = pUserMsg->id;
        pBmMsg->isExtended = pUserMsg->isExtended;
        pBmMsg->isRtr = pUserMsg->isRtr;
        pBmMsg->timeStamp = pUserMsg->timeStamp;
        pBmMsg->cluster = pUserMsg->cluster;
        pBmMsg->isCanfd = pUserMsg->isCanfd;
        memcpy(pBmMsg->data, pUserMsg->data, sizeof(pBmMsg->data));
    }
}
void copyST_BM_MSG_CAN(STCAN_MSG* pUserMsg, DBCanMsg* pBmMsg)
{
    //Called when SendMsg is called
    pUserMsg->isExtended = pBmMsg->isExtended;
    pUserMsg->isRtr = pBmMsg->isRtr;
    pUserMsg->cluster = pBmMsg->cluster;
    pUserMsg->dlc = pBmMsg->dlc;
    pUserMsg->id = pBmMsg->id;
    pUserMsg->isCanfd = pBmMsg->isCanfd;
    memcpy(pUserMsg->data, pBmMsg->data, sizeof(pUserMsg->data));
}

