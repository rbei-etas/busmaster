/*******************************************************************************
  Project       :  BUSMASTER
  FileName      :  BMJ1939Defines.cpp
  Description   :
  Author(s)     :
  Date Created  :  12/12/2013
  Date Modified :  17/02/2016
*******************************************************************************/
#include <WINDOWS.h>
#include <stdio.h>
#include "BMJ1939Defines.h"


extern HMODULE g_hModuleHandle;
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
UINT SendMsg(J1939_MSG& sTxMsg)
{
    UINT nResult = ERR_INVALID_ADDRESS;
    //J1939_MSG msgTx;
    if (sg_SendMsgFunc != NULL)
    {
        //copyST_BM_MSG_J1939(&msgTx, &sTxMsg);
        nResult = (*sg_SendMsgFunc)(&sTxMsg,g_hModuleHandle);
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
static MSGTXONOFF sg_OnOffTxMsg = NULL;
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

void copyBM_ST_MSG_J1939(BMJ1939Msg* pBmMsg, J1939_MSG* pUserMsg)
{
    //BMCanMsg* <- STCAN_MSG
    //Called When BUSMASTER Handler is called

    if(NULL != pUserMsg && NULL != pBmMsg )
    {
        pBmMsg->cluster = pUserMsg->cluster;
        pBmMsg->msgType = pUserMsg->msgType;
        pBmMsg->direction =  pUserMsg->direction;
        pBmMsg->id = pUserMsg->id;
        pBmMsg->dlc = pUserMsg->dlc;
        memcpy(pBmMsg->data, pUserMsg->data, sizeof(pBmMsg->data));
    }
}
void copyST_BM_MSG_J1939(J1939_MSG* pUserMsg, BMJ1939Msg* pBmMsg)
{

    //Called when SendMsg is called
    pUserMsg->cluster = pBmMsg->cluster;
    pUserMsg->msgType = pBmMsg->msgType;
    pUserMsg->direction = pBmMsg->direction;
    pUserMsg->id = pBmMsg->id;
    pUserMsg->dlc = pBmMsg->dlc;
    memcpy(pUserMsg->data, pBmMsg->data, sizeof(pUserMsg->data));
}
