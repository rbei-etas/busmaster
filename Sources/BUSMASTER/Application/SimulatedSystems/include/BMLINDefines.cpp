/*******************************************************************************
  Project       :  BUSMASTER
  FileName      :  BMLINDefines.cpp
  Description   :
  Author(s)     :  Prathiba Bharathi
  Date Created  :  12/12/2013
*******************************************************************************/

#include "BMLINDefines.h"

extern HMODULE g_hModuleHandle;
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
UINT SendMsg(STLIN_MSG sTxMsg)
{
    UINT nResult = ERR_INVALID_ADDRESS;
    //STLIN_MSG msgTx;
    if (sg_SendMsgFunc != NULL)
    {
        //copyST_BM_MSG_LIN(&msgTx, &sTxMsg);
        nResult = (*sg_SendMsgFunc)(&sTxMsg,g_hModuleHandle);
    }

    return nResult;
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


void copyBM_ST_MSG_LIN(DBLinMsg* pBmMsg, STLIN_MSG* pUserMsg)
{
    //DBLinMsg* <- STLIN_MSG
    //Called When BUSMASTER Handler is called

    if(NULL != pBmMsg && NULL != pUserMsg)
    {
        pBmMsg->messagetype = pUserMsg->messagetype;
        pBmMsg->timeStamp = pUserMsg->timeStamp;
        pBmMsg->cluster = pUserMsg->cluster;
        pBmMsg->crc = pUserMsg->crc;
        memcpy(pBmMsg->data, pUserMsg->data, sizeof(pBmMsg->data));
    }
}
void copyST_BM_MSG_LIN(STLIN_MSG* pUserMsg, DBLinMsg* pBmMsg)
{
    //Called when SendMsg is called
    if(NULL != pBmMsg && NULL != pUserMsg)
    {
        pUserMsg->messagetype = pBmMsg->messagetype;
        pUserMsg->cluster = pBmMsg->cluster;
        pUserMsg->checksumtype = pBmMsg->checksumtype;
        pUserMsg->dlc = pBmMsg->dlc;
        pUserMsg->id = pBmMsg->id;
        pUserMsg->cluster = pBmMsg->cluster;
        pUserMsg->crc = pBmMsg->crc;
        memcpy(pUserMsg->data, pUserMsg->data, sizeof(pUserMsg->data));
    }
}