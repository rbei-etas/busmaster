/*
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/**
 * \file      ExecuteFunc.cpp
 * \brief     This file contain definition of all function of
 * \author    Ratnadip Choudhury
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * This file contain definition of all function of
 */
// Include all standard header files
// Definition of CExecuteFunc class
#include "NodeSimEx_StdAfx.h"
#include "Include/Basedefs.h"
#include "Include/Dil_CommonDefs.h"
#include "Datatypes/J1939_Datatypes.h"
#include "ExecuteFunc.h"
#include "AppServicesCAN.h"
#include "AppServicesJ1939.h"
#include "GlobalObj.h"
//defination of exported class to DLL
#include "Export_UserDllCAN.h"
#include "Export_UserDllJ1939.h"
//accessin manager class object
#include "SimSysManager.h"


using namespace std;

extern UINT unKeyHandlerProc(LPVOID);
extern UINT unErrorHandlerProc(LPVOID);
extern UINT unEventHandlerProc(LPVOID);
// DLL Unload Handler
extern UINT unDLLUnloadHandlerProc(LPVOID);
// DLL Load Handler
extern UINT unDLLloadHandlerProc(LPVOID);
//begin execution of timer thread
extern UINT unTimerHandlerProc(LPVOID pParam);

// Trace window API
extern UINT gunWriteToTrace(CHAR* pcOutStrTrace);
//thread function from utility file
UINT unReadNodeMsgHandlerBuffer(LPVOID pParam);
//send msg API
//extern int gnSendFlexMsg(sTCANDATA sTxMsg,HMODULE hModule);
extern BOOL gbMsgTransmissionOnOff(BOOL bOnOff,HMODULE hModule);
extern HMODULE ghGetNodeDllHandler(char* strNodeName);
extern HWND g_hMainGUI;
const int CANMSG_STRUCT_SIZE    = sizeof(sTCANDATA);


/******************************************************************************/
/*  Function Name    :  CExecuteFunc                                          */
/*  Input(s)         :                                                        */
/*  Output           :                                                        */
/*  Functionality    :  Constructor is called when user creates an object of  */
/*                      this class. Initialisation of some data members       */
/*  Member of        :  CExecuteFunc                                          */
/*  Friend of        :      -                                                 */
/*  Author(s)        :  Amitesh Bharti                                        */
/*  Date Created     :  28.02.2002                                            */
/*  Modifications    :  Amitesh Bharti                                        */
/*                      12.03.2003, added initialisation for all new struct.  */
/*  Modifications    :  Raja N                                                */
/*                      10.05.2004, added initialisation of message handler   */
/*                      map and critical section                              */
/*  Modifications    :  Anish kumar                                           */
/*                      10.01.2006, added initialisation of new data members  */
/*                                                                            */
/******************************************************************************/
CExecuteFunc::CExecuteFunc(ETYPE_BUS eBus, CONST CString& omStrDllFileName) :
    m_psMsgHandlersInfo(NULL),
    m_psOnMsgIDRangeHandlersCAN(NULL),
    m_psOnMsgIDRangeHandlers(NULL),
    m_pFGenericMsgHandlerCAN(NULL),
    m_pFGenericMsgHandler(NULL),
    m_psOnKeyHandlers(NULL),
    m_psOnErrorHandlers(NULL),
    m_psOnEventHandlers(NULL),
    m_hDllModule(NULL),
    m_omStrDllFileName(omStrDllFileName),
    m_omStrGenericHandlerName(_T("")),
    m_pFGenericKeyHandler(NULL),
    m_psOnDLLHandlers(NULL),
    m_bDllLoaded(TRUE),
    m_bMsgTxOnFlag(TRUE),
    m_unQMsgCount(0),
    m_unReadQMsgIndex(0),
    m_unWriteQMsgIndex(0),
    m_bStopKeyHandlers(TRUE),
    m_bStopErrorHandlers(TRUE),
    m_bStopDLLHandlers(TRUE),
    m_pomMsgHandlerThrd(NULL),
    m_psFirstTimerStrList(NULL),
    m_psLastTimerStrList(NULL),
    m_bTimerThreadStarted(FALSE),
    m_sNodeInfo(eBus)


{
    m_eBus = eBus;
    m_omStrArrayTimerHandlers.RemoveAll();
    m_omStrArrayKeyHandlers.RemoveAll();
    m_omStrArrayErrorHandlers.RemoveAll();
    m_omStrArrayEventHandlers.RemoveAll();
    m_omStrArrayDLLHandlers.RemoveAll();
    m_omStrArrayMsgRange.RemoveAll();
    m_omStrArrayMsgIDandName.RemoveAll();
    m_omStrArrayMsgHandlers.RemoveAll();
    m_bIsStatWndCreated = FALSE;

    //initialization of buffer
    for(int nBufferCnt=0; nBufferCnt<defMAX_FUNC_MSG; nBufferCnt++)
    {
        memset(&m_asQMsg[m_unWriteQMsgIndex], 0, CANMSG_STRUCT_SIZE);
    }

    // All void pointer for memory allocated inside is initialised to NULL
    for(UINT i=0; i<defEVENT_EXFUNC_TOTAL; i++)
    {
        m_asUtilThread[i].m_hThread  = NULL;
        m_asUtilThread[i].m_pvThread = NULL;
    }

    m_aomState[defKEY_HANDLER_THREAD ].SetEvent();
    m_aomState[defDLL_LOAD_HANDLER_THREAD ].SetEvent();
    m_aomState[defDLL_UNLOAD_HANDLER_THREAD ].SetEvent();
    m_aomState[defERROR_HANDLER_THREAD].SetEvent();
    m_aomState[defMSG_HANDLER_THREAD ].SetEvent();
    // Init CMap with the Hash table size.(any prime number)
    m_omMsgHandlerMapCAN.InitHashTable(def_MSG_MAP_HASH_SIZE);
    m_omMsgHandlerMap.InitHashTable(def_MSG_MAP_HASH_SIZE);
    // Initialise critical section used in buffer access
    InitializeCriticalSection(&m_CritSectForFuncBuf);
    //creating read and execute message handler thread
    m_pomMsgHandlerThrd=AfxBeginThread(unReadNodeMsgHandlerBuffer,this);
}
/******************************************************************************/
/*  Function Name    :  ~CExecuteFunc                                         */
/*  Input(s)         :                                                        */
/*  Output           :                                                        */
/*  Functionality    :  This is default destractor.                           */
/*  Member of        :  CExecuteFunc                                          */
/*  Friend of        :      -                                                 */
/*  Author(s)        :  Amitesh Bharti                                        */
/*  Date Created     :  28.02.2002                                            */
/*  Modification By  :  Amitesh Bharti                                        */
/*  Modification on  :  07.03.2002                                            */
/*  Modification By  :  Amitesh Bharti                                        */
/*  Modification on  :  30.12.2002, remove killing of timer                   */
/*  Modification By  :  Amitesh Bharti                                        */
/*  Modification on  :  11.03.2003, added clean up code for new structure.    */
/*  Modification By  :  Amitesh Bharti                                        */
/*  Modification on  :  07.04.2003, review comment added                      */
/*  Modification By  :  Raja N                                                */
/*  Modification on  :  29.04.2004, Added function call vClearMessageStructure*/
/*                      to clear data allocated for global message structure  */
/*  Modifications    :  Raja N                                                */
/*                      10.05.2004, added deletion of message handler critical*/
/*                      section and removed the function call                 */
/*                      vClearMessageStructure as it is not required now      */
/*                      and removed deletion of message handler variable as it*/
/*                      is replaced with CMap                                 */
/*  Modification By  :  Anish kumar                                           */
/*  Modifications    :  Termination of message handler thread                 */
/******************************************************************************/
CExecuteFunc::~CExecuteFunc()
{
    // Delete key handlers array of structure.
    if(m_psOnKeyHandlers!=NULL)
    {
        delete []m_psOnKeyHandlers;
        m_psOnKeyHandlers = NULL;
    }

    // Delete message handler info array of structure.
    if(m_psMsgHandlersInfo!=NULL)
    {
        if(m_psMsgHandlersInfo->m_psRange != NULL)
        {
            delete []m_psMsgHandlersInfo->m_psRange;
            m_psMsgHandlersInfo->m_psRange = NULL;
        }

        delete m_psMsgHandlersInfo;
        m_psMsgHandlersInfo = NULL;
    }

    // Delete array of structure of message Id range handlers
    if(m_psOnMsgIDRangeHandlersCAN != NULL )
    {
        delete []m_psOnMsgIDRangeHandlersCAN;
        m_psOnMsgIDRangeHandlersCAN= NULL ;
    }

    if(m_psOnMsgIDRangeHandlers != NULL )
    {
        delete []m_psOnMsgIDRangeHandlers;
        m_psOnMsgIDRangeHandlers= NULL ;
    }

    // Delete Error handlers array of structure.
    if(m_psOnErrorHandlers!=NULL)
    {
        delete []m_psOnErrorHandlers;
        m_psOnErrorHandlers = NULL;
    }

    // Delete Event handlers array of structure.
    if(m_psOnEventHandlers!=NULL)
    {
        delete []m_psOnEventHandlers;
        m_psOnEventHandlers = NULL;
    }

    // Delete DLL Handlers array of structure.
    if(m_psOnDLLHandlers != NULL )
    {
        delete m_psOnDLLHandlers;
        m_psOnDLLHandlers = NULL;
    }

    // Free Cirical Section Resource
    DeleteCriticalSection(&m_CritSectForFuncBuf);
}
/******************************************************************************/
/*  Function Name    :  bInitStruct                                           */
/*  Input(s)         :                                                        */
/*  Output           :  Success(TRUE) or Failure(FALSE)                       */
/*  Functionality    :  Initialised all structure for message, key and timer  */
/*                      handlers.                                             */
/*  Member of        :  CExecuteFunc                                          */
/*  Friend of        :      -                                                 */
/*  Author(s)        :  Amitesh Bharti                                        */
/*  Date Created     :  28.02.2002                                            */
/*  Modification By  :  Amitesh Bharti                                        */
/*  Modification on  :  07.03.2002                                            */
/*  Modification By  :  Amitesh Bharti                                        */
/*  Modification on  :  01.01.2003, Function return type for CMainFrame is    */
/*                      validated before using it.                            */
/*  Modification By  :  Amitesh Bharti                                        */
/*  Modification on  :  01.03.2003,new function is called for DLL and error   */
/*                      handlers                                              */
/*  Modification By  :  Raja N                                                */
/*  Modification on  :  29.04.2004,Replaced PostMessage in to function call   */
/*                      vInitMsgHandlerStruct to init global message structure*/
/*  Modification By  :  Raja N                                                */
/*  Modification on  :  10.05.2004, Removed posting of message to init as the */
/*                      global variable is removed in the new design          */
/******************************************************************************/
BOOL CExecuteFunc::bInitStruct(CStringArray& omErrorArray)
{
    BOOL bReturn     = FALSE;
    bReturn = bReadDefFile(omErrorArray);

    if(bReturn==TRUE)
    {
        bReturn = bInitMSGStruct(omErrorArray);

        if(bReturn == TRUE)
        {
            bReturn = bInitOnKeyStruct(omErrorArray);
        }

        if(bReturn == TRUE)
        {
            bReturn = bInitTimerStruct(omErrorArray);
        }

        if(bReturn == TRUE)
        {
            if (m_eBus == CAN)
            {
                bReturn = bInitErrorStruct(omErrorArray);
            }
        }

        if (bReturn == TRUE)
        {
            if (m_eBus == J1939)
            {
                bReturn = bInitEventStructJ1939(omErrorArray);
            }
        }

        if(bReturn == TRUE)
        {
            bReturn = bInitDLLStruct(omErrorArray);
        }
    }

    return bReturn;
}

/******************************************************************************/
/*  Function Name    :  vExecuteOnPGNHandler                              */
/*  Input(s)         :  J1939 msg pointer                                            */
/*  Output           :                                                        */
/*  Functionality    :  Excutes message handler corresponding to PGN ID   */
/*                      passed as parameter                                   */
/*  Member of        :  CExecuteFunc                                          */
/*  Friend of        :      -                                                 */
/*  Author(s)        :  Pradeep Kadoor                                        */
/*  Date Created     :  28.02.2002                                            */
/******************************************************************************/

VOID CExecuteFunc::vExecuteOnPGNHandler(void* pRxMsg)
{
    VALIDATE_POINTER_RETURN_VOID(pRxMsg);
    STJ1939_MSG* psJ1939Msg = (STJ1939_MSG*)pRxMsg;
    // Search for msg name and msg ID handler matching the message ID.
    UINT unPGN = psJ1939Msg->m_sMsgProperties.m_uExtendedID.m_s29BitId.unGetPGN();
    // Check for Id in Name/Id specific Handler
    SMSGHANDLERDATA sMsgData = m_omMsgHandlerMap[unPGN];

    // If not found
    if ( sMsgData.m_pFMsgHandler == NULL )
    {
        // Find the Id in the Range Handlers
        sMsgData.m_pFMsgHandler = pFSearchMsgIdRangeHandler(unPGN);

        // If not found
        if(sMsgData.m_pFMsgHandler == NULL)
        {
            // Check for generic Handler
            if(m_pFGenericMsgHandler != NULL )
            {
                sMsgData.m_pFMsgHandler = m_pFGenericMsgHandler;
                // Add Generic Message Handler Proc Address in to the CMap
                // This will avoid search for this ID next time
                m_omMsgHandlerMap[unPGN] = sMsgData;
            }
        }
        // The ID comes under a range. Store the Range Handler in the CMap
        else
        {
            // This will avoide search for the same message ID next time
            m_omMsgHandlerMap[unPGN] = sMsgData;
        }
    }

    // If the handler found then proceed further
    if(sMsgData.m_pFMsgHandler != NULL)
    {
        m_bStopMsgHandlers = FALSE;

        // Check for Message Type Big or Little Endian
        // and reverse data bytes if it is not in intel format
        if( sMsgData.nMsgFormat != -1 && sMsgData.nMsgFormat != DATA_FORMAT_INTEL)
        {
            register BYTE byTmp;
            register BYTE* pbyMsgData = psJ1939Msg->m_pbyData;
            register UINT unLimit = sMsgData.unDLC / 2;

            for ( register UINT nIndex = 0; nIndex < unLimit; nIndex++)
            {
                register UINT unTempIndex = sMsgData.unDLC - 1 - nIndex;
                byTmp = pbyMsgData[nIndex];
                pbyMsgData[nIndex] = pbyMsgData[unTempIndex];
                pbyMsgData[unTempIndex] = byTmp;
            }
        }

        // Catch user program errors here
        try
        {
            sMsgData.m_pFMsgHandler(psJ1939Msg);
        }
        catch(...)
        {
            char acError[256];
            sprintf_s( acError,
                       defSTR_ERROR_IN_PGN_PROG,
                       psJ1939Msg->m_sMsgProperties.m_uExtendedID.m_s29BitId.unGetPGN() );
            // Display the error information in the Trace window
            gbSendStrToTrace(acError);
        }
    }
}

/******************************************************************************/
/*  Function Name    :  vExecuteOnMessageHandler                              */
/*  Input(s)         :  Message ID                                            */
/*  Output           :                                                        */
/*  Functionality    :  Excutes message handler corresponding to message ID   */
/*                      passed as parameter                                   */
/*  Member of        :  CExecuteFunc                                          */
/*  Friend of        :      -                                                 */
/*  Author(s)        :  Amitesh Bharti                                        */
/*  Date Created     :  28.02.2002                                            */
/******************************************************************************/

VOID CExecuteFunc::vExecuteOnMessageHandlerCAN(STCAN_MSG sRxMsgInfo)
{
    SEXECUTE_MSG_HANDLER_CAN sExecuteMsgHandler;
    memcpy(&sExecuteMsgHandler.m_sRxMsg, &sRxMsgInfo, sizeof (sRxMsgInfo));
    //vCopy_s_FLXMSG_to_sTCANDATA( sExecuteMsgHandler.m_sRxMsg, sRxMsgInfo);
    // Search for msg name and msg ID handler matching the message ID.
    UINT unMsgID = sExecuteMsgHandler.m_sRxMsg.m_unMsgID;
    // Check for Id in Name/Id specific Handler
    SMSGHANDLERDATA_CAN sMsgData = m_omMsgHandlerMapCAN[unMsgID];

    // If not found
    if ( sMsgData.m_pFMsgHandler == NULL )
    {
        // Find the Id in the Range Handlers
        sMsgData.m_pFMsgHandler = pFSearchMsgIdRangeHandlerCAN(unMsgID);

        // If not found
        if(sMsgData.m_pFMsgHandler == NULL)
        {
            // Check for generic Handler
            if(m_pFGenericMsgHandlerCAN != NULL )
            {
                sMsgData.m_pFMsgHandler = m_pFGenericMsgHandlerCAN;
                // Add Generic Message Handler Proc Address in to the CMap
                // This will avoid search for this ID next time
                m_omMsgHandlerMapCAN[unMsgID] = sMsgData;
            }
        }
        // The ID comes under a range. Store the Range Handler in the CMap
        else
        {
            // This will avoide search for the same message ID next time
            m_omMsgHandlerMapCAN[unMsgID] = sMsgData;
        }
    }

    // If the handler found then proceed further
    if(sMsgData.m_pFMsgHandler != NULL)
    {
        m_bStopMsgHandlers = FALSE;

        // Check for Message Type Big or Little Endian
        // and reverse data bytes if it is not in intel format
        if( sMsgData.nMsgFormat != -1 && sMsgData.nMsgFormat != DATA_FORMAT_INTEL)
        {
            register BYTE byTmp;
            register BYTE* pbyMsgData = sExecuteMsgHandler.m_sRxMsg.m_ucData;
            register UINT unLimit = sMsgData.unDLC / 2;

            for ( register UINT nIndex = 0; nIndex < unLimit; nIndex++)
            {
                register UINT unTempIndex = sMsgData.unDLC - 1 - nIndex;
                byTmp = pbyMsgData[nIndex];
                pbyMsgData[nIndex] = pbyMsgData[unTempIndex];
                pbyMsgData[unTempIndex] = byTmp;
            }
        }

        sExecuteMsgHandler.m_pFMsgHandler = sMsgData.m_pFMsgHandler;

        // Catch user program errors here
        try
        {
            sExecuteMsgHandler.m_pFMsgHandler(sExecuteMsgHandler.m_sRxMsg);
        }
        catch(...)
        {
            char acError[256];
            sprintf_s( acError,
                       defSTR_ERROR_IN_MSG_PROG,
                       sExecuteMsgHandler.m_sRxMsg.m_unMsgID );
            // Display the error information in the Trace window
            gbSendStrToTrace(acError);
        }
    }
}
/******************************************************************************/
/*  Function Name    :  vExecuteOnKeyHandler                                  */
/*  Input(s)         :  Key value                                             */
/*  Output           :                                                        */
/*  Functionality    :  Executes corresponding key handler for which the key  */
/*                      value is passed as parameter.                         */
/*  Member of        :  CExecuteFunc                                          */
/*  Friend of        :      -                                                 */
/*  Author(s)        :  Amitesh Bharti                                        */
/*  Date Created     :  28.02.2002                                            */
/*  Modification By  :  Amitesh Bharti                                        */
/*  Modification on  :  07.03.2002                                            */
/*  Modified by      :  Ratnadip Choudhury (06.11.2002)                       */
/*  Modifications    :  Create a worker thread to call the key handler        */
/*  Modification By  :  Amitesh Bharti                                        */
/*  Modification on  :  27.12.2002, Thread handle is obtained and assigned to */
/*                      public data member of CCANMonitorApp class            */
/*  Modification By  :  Amitesh Bharti                                        */
/*  Modification on  :  03.01.2003,Key hadler count unKeyHandlerCount was not */
/*                      initialised to number of key handlers. Corrected.     */
/******************************************************************************/
VOID CExecuteFunc::vExecuteOnKeyHandler(UCHAR ucKey)
{
    UINT unKeyHandlerCount   = 0;
    BOOL bKeyHandlerExecuted = FALSE;
    PSEXECUTE_KEY_HANDLER psExecuteKeyHandler = NULL;
    unKeyHandlerCount        = (COMMANUINT)m_omStrArrayKeyHandlers.GetSize();
    psExecuteKeyHandler = new SEXECUTE_KEY_HANDLER;

    if(psExecuteKeyHandler != NULL)
    {
        UINT unKeyCount          = 0;
        psExecuteKeyHandler->m_ucKeyValue = ucKey;
        psExecuteKeyHandler->m_pFKeyHandler = NULL;
        psExecuteKeyHandler->m_pCExecuteFunc=this;

        while(unKeyCount<unKeyHandlerCount && bKeyHandlerExecuted != TRUE)
        {
            if(ucKey == m_psOnKeyHandlers[unKeyCount].ucKey)
            {
                if(m_psOnKeyHandlers[unKeyCount].pFKeyHandlers!=NULL)
                {
                    psExecuteKeyHandler->m_pFKeyHandler =
                        m_psOnKeyHandlers[unKeyCount].pFKeyHandlers;
                }

                bKeyHandlerExecuted = TRUE;
            }

            unKeyCount++;
        }// while

        // could not found a match for the key pressed. Search for generic
        // handler.
        if( bKeyHandlerExecuted == FALSE && m_pFGenericKeyHandler != NULL)
        {
            psExecuteKeyHandler->m_pFKeyHandler = m_pFGenericKeyHandler ;
        }

        if(m_asUtilThread[defKEY_HANDLER_THREAD].m_hThread == NULL )
        {
            if(psExecuteKeyHandler->m_pFKeyHandler != NULL )
            {
                m_asUtilThread[defKEY_HANDLER_THREAD].m_pvThread =
                    psExecuteKeyHandler;
                // Get handle of thread and assign it to pulic data member
                // in app class. This will be used to terminate the thread.
                CWinThread* pomThread = NULL ;
                //pomThread = AfxBeginThread(unKeyHandlerProc,
                //                psExecuteKeyHandler);
                pomThread = AfxBeginThread(unKeyHandlerProc,
                                           psExecuteKeyHandler);

                if(pomThread != NULL )
                {
                    m_asUtilThread[defKEY_HANDLER_THREAD].m_hThread
                        = pomThread;
                }
            }
            else
            {
                delete psExecuteKeyHandler;
                psExecuteKeyHandler = NULL;
            }
        }
        else
        {
            delete psExecuteKeyHandler;
            psExecuteKeyHandler = NULL;
        }
    }
}
/******************************************************************************/
/*  Function Name    :  vExecuteOnErrorHandler                                */
/*  Input(s)         :  Error Code value                                      */
/*  Output           :                                                        */
/*  Functionality    :  Executes corresponding error handler for which the    */
/*                      eErrorCode value is passed as parameter.              */
/*  Member of        :  CExecuteFunc                                          */
/*  Friend of        :      -                                                 */
/*  Author(s)        :  Amitesh Bharti                                        */
/*  Date Created     :  24.02.2003                                            */
/*  Modification By  :                                                        */
/*  Modification on  :                                                        */
/******************************************************************************/
VOID CExecuteFunc::vExecuteOnErrorHandler(eERROR_STATE eErrorCode,SCAN_ERR sErrorVal)
{
    UINT unErrorHandlerCount   = 0;
    BOOL bErrorHandlerExecuted = FALSE;
    unErrorHandlerCount        = (COMMANUINT)m_omStrArrayErrorHandlers.GetSize();

    if(m_asUtilThread[defERROR_HANDLER_THREAD].m_hThread == NULL )
    {
        UINT unErrorCount = 0;

        while(unErrorCount<unErrorHandlerCount && bErrorHandlerExecuted != TRUE)
        {
            if(eErrorCode == m_psOnErrorHandlers[unErrorCount].m_eErrorCode)
            {
                if(m_psOnErrorHandlers[unErrorCount].m_pFErrorHandlers!=NULL)
                {
                    //Updating the global structure variable of SCAN_ERR
                    m_psOnErrorHandlers[unErrorCount].sErrorVal = sErrorVal ;
                    //pass the pointer to this object to access thread
                    m_psOnErrorHandlers[unErrorCount].m_pCExecuteFunc=this;
                    // Get handle of thread and assign it to pulic data member
                    // in app class. This will be used to terminate the thread.
                    CWinThread* pomThread = NULL ;
                    pomThread = AfxBeginThread(unErrorHandlerProc,
                                               &m_psOnErrorHandlers[unErrorCount] );

                    if(pomThread != NULL )
                    {
                        m_asUtilThread[defERROR_HANDLER_THREAD].m_hThread
                            = pomThread->m_hThread;
                    }
                }

                bErrorHandlerExecuted = TRUE;
            }

            unErrorCount++;
        }
    }
}
void CExecuteFunc::vExecuteOnEventHandlerJ1939(UINT32 unPGN, BYTE bySrc, BYTE byDest, BOOL bSuccess, BYTE byType)
{
    UINT unEventHandlerCount   = 0;
    BOOL bEventHandlerExecuted = FALSE;
    UINT unEventCount          = 0;
    unEventHandlerCount        = (COMMANUINT)m_omStrArrayEventHandlers.GetSize();

    if(m_asUtilThread[defEVENT_HANDLER_THREAD].m_hThread == NULL )
    {
        while((unEventCount < unEventHandlerCount) && (bEventHandlerExecuted != TRUE))
        {
            if(m_psOnEventHandlers[unEventCount].m_byEventType == byType)
            {
                if(m_psOnEventHandlers[unEventCount].m_pFEventHandlers!=NULL)
                {
                    m_psOnEventHandlers[unEventCount].m_unPGN       = unPGN;
                    m_psOnEventHandlers[unEventCount].m_bySrc       = bySrc;
                    m_psOnEventHandlers[unEventCount].m_byDest      = byDest;
                    m_psOnEventHandlers[unEventCount].m_bSuccess    = bSuccess;
                    //pass the pointer to this object to access thread
                    m_psOnEventHandlers[unEventCount].m_pCExecuteFunc=this;
                    // Get handle of thread and assign it to pulic data member
                    // in app class. This will be used to terminate the thread.
                    CWinThread* pomThread = NULL ;
                    pomThread = AfxBeginThread(unEventHandlerProc,
                                               &m_psOnEventHandlers[unEventCount] );

                    if(pomThread != NULL )
                    {
                        m_asUtilThread[defEVENT_HANDLER_THREAD].m_hThread
                            = pomThread->m_hThread;
                    }
                }

                bEventHandlerExecuted = TRUE;
            }

            unEventCount++;
        }
    }
}
/* Executes data confirmation event */
VOID CExecuteFunc::vExecuteOnDataConfHandlerJ1939(UINT32 unPGN, BYTE bySrc, BYTE byDest, BOOL bSuccess)
{
    vExecuteOnEventHandlerJ1939(unPGN, bySrc, byDest, bSuccess, 0x0);
}

/* Executes address claim event */
VOID CExecuteFunc::vExecuteOnAddressClaimHandlerJ1939(BYTE byAddress)
{
    vExecuteOnEventHandlerJ1939(0, byAddress, 0, 0, 0x1);
}
/******************************************************************************/
/*  Function Name    :  vExecuteOnDLLHandler                                  */
/*  Input(s)         :  DLL Handler                                           */
/*  Output           :  Excecutes the DLL handler function                    */
/*  Functionality    :                                                        */
/*                                                                            */
/*  Member of        :  CExecuteFunc                                          */
/*  Friend of        :      -                                                 */
/*  Author(s)        :  Ravikumar Patil                                       */
/*  Date Created     :  10.03.2003                                            */
/*  Modification By  :  Amitesh Bharti                                        */
/*  Modification on  :  22.07.2004, Unload handler moved in to a seprate thrd */
/*  Modification By  :  Anish kumar                                           */
/*  Modification on  :  21.11.06,Reset the event for Dll Unload befor starting*/
/*                     the thread for executing unload handler so that  it    */
/*                     will be executed at the time of closing the application*/
/******************************************************************************/
VOID CExecuteFunc::vExecuteOnDLLHandler(eDLLHANDLER eDLLHandler)
{
    UINT unDLLHandlerCount     = 0;
    BOOL bDLLHandlerExecuted   = FALSE;
    UINT unDLLCount            = 0;
    unDLLHandlerCount        = (COMMANUINT)m_omStrArrayDLLHandlers.GetSize();

    while(unDLLCount<unDLLHandlerCount && bDLLHandlerExecuted != TRUE)
    {
        if(eDLLHandler == m_psOnDLLHandlers[unDLLCount].m_eDLLHandler)
        {
            if(m_psOnDLLHandlers[unDLLCount].m_pFDLLHandlers!=NULL)
            {
                CWinThread* pomThread = NULL ;

                if(eDLLHandler == DLL_LOAD &&
                        m_asUtilThread[defDLL_LOAD_HANDLER_THREAD].m_hThread == NULL )
                {
                    //to pass this object and pointer to dll handler
                    PSEXECUTE_LOAD_HANDLER psExecuteLoadHandler=
                        new SEXECUTE_LOAD_HANDLER;

                    if(psExecuteLoadHandler!=NULL)
                    {
                        psExecuteLoadHandler->pFDllHandler=
                            m_psOnDLLHandlers[unDLLCount].m_pFDLLHandlers;
                        psExecuteLoadHandler->m_pCExecuteFunc=this;
                        // Get handle of thread and assign it to pulic data
                        // member in app class. This will be used to terminate
                        // the thread.
                        pomThread = AfxBeginThread(unDLLloadHandlerProc,
                                                   psExecuteLoadHandler);
                        Sleep(0);

                        if(pomThread != NULL )
                        {
                            m_asUtilThread
                            [defDLL_LOAD_HANDLER_THREAD].m_hThread =
                                pomThread->m_hThread;
                            m_asUtilThread
                            [defDLL_LOAD_HANDLER_THREAD].m_pvThread = NULL;
                        }
                    }
                    else
                    {
                        delete psExecuteLoadHandler;
                        psExecuteLoadHandler=NULL;
                    }
                }
                else if(eDLLHandler == DLL_UNLOAD &&
                        m_asUtilThread[defDLL_UNLOAD_HANDLER_THREAD].m_hThread == NULL )
                {
                    PSEXECUTE_LOAD_HANDLER psExecuteLoadHandler=
                        new SEXECUTE_LOAD_HANDLER;

                    if(psExecuteLoadHandler!=NULL)
                    {
                        psExecuteLoadHandler->pFDllHandler=
                            m_psOnDLLHandlers[unDLLCount].m_pFDLLHandlers;
                        psExecuteLoadHandler->m_pCExecuteFunc=this;
                        // Get handle of thread and assign it to pulic data
                        // member in app class. This will be used to terminate
                        // the thread.
                        m_aomState[defDLL_UNLOAD_HANDLER_THREAD].ResetEvent();
                        pomThread = AfxBeginThread(unDLLUnloadHandlerProc,
                                                   psExecuteLoadHandler);
                        Sleep(0);

                        if(pomThread != NULL )
                        {
                            m_asUtilThread
                            [defDLL_UNLOAD_HANDLER_THREAD].m_hThread =
                                pomThread->m_hThread;
                            m_asUtilThread
                            [defDLL_UNLOAD_HANDLER_THREAD].m_pvThread = NULL;
                        }
                    }
                }
            }

            bDLLHandlerExecuted = TRUE;
        }

        unDLLCount++;
    }
}


/******************************************************************************/
/*  Function Name    :  bReadDefFile                                          */
/*  Input(s)         :                                                        */
/*  Output           :                                                        */
/*  Functionality    :  This function will read the name of message handler   */
/*                      function and key handler function and store it in the */
/*                      respective data members                               */
/*                                                                            */
/*  Member of        :  CExecuteFunc                                          */
/*  Friend of        :      -                                                 */
/*  Author(s)        :  Amitesh Bharti                                        */
/*  Date Created     :  28.02.2002                                            */
/*  Modification By  :  Amitesh Bharti                                        */
/*  Modification on  :  08.03.2002                                            */
/*  Modification By  :  Amitesh Bharti                                        */
/*  Modification on  :  05.03.2003, Added initialisation for error and dll    */
/*                      handlers.Modified init for message and key handlers   */
/******************************************************************************/
BOOL CExecuteFunc::bReadDefFile(CStringArray& omErrorArray)
{
    BOOL bReturn = FALSE;
    ifstream    omInTextFile;
    CString     omStrErrorMessage;
    CString     omStrLine;
    CString     omStrTemp;
    CHAR        acLine[1024];
    CString omStrDllFileName = m_omStrDllFileName;
    CString omStrDefFileName = m_omStrDllFileName;
    // Get the .def file name with path from .dll file name and path
    omStrDefFileName.Replace(defDOT_DLL,defDOT_DEF);

    // If the .def file name is valid, set the flag
    if ( omStrDefFileName.IsEmpty()== 0 )
    {
        bReturn = TRUE;
    }

    if(bReturn == TRUE)
    {
        TRY
        {
            omInTextFile.open(/*T2A*/(omStrDefFileName.GetBuffer(MAX_PATH)), ios::in  );

            if (!omInTextFile.good())
            {
                // Info file open error notification
                omStrErrorMessage  =_T("Input file open error : ");
                omStrErrorMessage +=omStrDefFileName;
                /*AfxMessageBox(omStrErrorMessage ,
                                  MB_ICONERROR| MB_SYSTEMMODAL|MB_OK,nZERO);*/
                omErrorArray.Add(omStrErrorMessage);
                bReturn = FALSE ;
            }

            if(bReturn != FALSE)
            {
                while (!omInTextFile.eof())
                {
                    omInTextFile.getline( acLine, sizeof(acLine));
                    omStrLine = acLine;
                    int nIndex = 0;
                    omStrLine.TrimLeft();
                    omStrLine.TrimRight();
                    // Search for message handler function and add it to
                    // corresponding CStringArray data member.
                    nIndex = omStrLine.Find( CGlobalObj::omGetBusSpecMsgHndlrName(m_eBus));

                    if ( nIndex != -1 )
                    {
                        m_omStrArrayMsgHandlers.Add(omStrLine);
                        vCatagoriseMsgHandlers(omStrLine);
                    }
                    else
                    {
                        // Search for key handler function and add it to
                        // corresponding CStringArray data member.
                        nIndex = omStrLine.Find(defKEY_HANDLER);

                        if ( nIndex != -1 )
                        {
                            m_omStrArrayKeyHandlers.Add(omStrLine);
                        }
                        else
                        {
                            // Search for timer handler function and add it to
                            // corresponding CStringArray data member.
                            nIndex = omStrLine.Find(defTIMER_HANDLER);

                            if ( nIndex != -1 )
                            {
                                m_omStrArrayTimerHandlers.Add(omStrLine);
                            }
                            else
                            {
                                // Search for Error handler function and add it
                                // to corresponding CStringArray data member.
                                nIndex = omStrLine.Find(defERROR_HANDLER_FN);

                                if ( nIndex != -1 )
                                {
                                    m_omStrArrayErrorHandlers.Add(omStrLine);
                                }
                                else
                                {
                                    // Search for Error handler function and add
                                    // it to  corresponding CStringArray data
                                    // member.
                                    nIndex = omStrLine.Find(defDLL_HANDLER_FN);

                                    if ( nIndex != -1 )
                                    {
                                        m_omStrArrayDLLHandlers.Add(omStrLine);
                                    }
                                    else
                                    {
                                        nIndex = omStrLine.Find(defEVENT_IND_HANDLER);

                                        if ( nIndex != -1 )
                                        {
                                            m_omStrArrayEventHandlers.Add(omStrLine);
                                        }
                                    }
                                }
                            }
                        }
                    }
                }// while
            }
        }
        CATCH_ALL(pomException)
        {
            if(pomException != NULL )
            {
                TCHAR acErrorMsg[defSIZE_OF_ERROR_BUFFER];
                // Get the exception error message
                pomException->GetErrorMessage(acErrorMsg,sizeof(acErrorMsg));
                omStrErrorMessage = acErrorMsg;
                //Display the error
                /*AfxMessageBox(omStrErrorMessage ,
                    MB_ICONERROR| MB_SYSTEMMODAL|MB_OK,nZERO);*/
                omErrorArray.Add(omStrErrorMessage);
                pomException->Delete();
            }

            bReturn = FALSE;
        }
        END_CATCH_ALL
    }

    return bReturn;
}
/******************************************************************************/
/*  Function Name    :  vInitMSGStruct                                        */
/*  Input(s)         :                                                        */
/*  Output           :                                                        */
/*  Functionality    :  Initialise the array of Msg structure by dynamically  */
/*                      creating it and filling all available information     */
/*  Member of        :  CExecuteFunc                                          */
/*  Friend of        :      -                                                 */
/*  Author(s)        :  Amitesh Bharti                                        */
/*  Date Created     :  28.02.2002                                            */
/*  Modification By  :  Amitesh Bharti                                        */
/*  Modification on  :  08.03.2002,                                           */
/*  Modification By  :  Amitesh Bharti                                        */
/*  Modification on  :  05.03.2003, Initialisation of four type of msg handler*/
/*  Modification By  :  Raja N                                                */
/*  Modification on  :  29.04.2004, Initialisation of m_bGenericHandlerON to  */
/*                      FALSE is required and added CMap init code to clear   */
/*                      existing entries in the CMap                          */
/******************************************************************************/
BOOL CExecuteFunc::bInitMSGStruct(CStringArray& omErrorArray)
{
    UINT unMsgIDandNameCount    = 0;
    UINT unMsgIDRangeCount      = 0;
    UINT unTotalMsgHandler      = 0;
    BOOL bReturn                = TRUE;
    UINT unTemp                 = 0;
    unMsgIDandNameCount = (COMMANUINT)m_omStrArrayMsgIDandName.GetSize();
    unMsgIDRangeCount   = (COMMANUINT)m_omStrArrayMsgRange.GetSize();
    unTotalMsgHandler   =(COMMANUINT)m_omStrArrayMsgHandlers.GetSize();
    unTemp = unMsgIDandNameCount + unMsgIDRangeCount;

    if(m_omStrGenericHandlerName.IsEmpty() != TRUE)
    {
        unTemp      += 1 ;// one Generic message handler
    }

    if( unTemp != unTotalMsgHandler)
    {
        bReturn     = FALSE;
    }

    if(bReturn != FALSE && unTotalMsgHandler > 0)
    {
        bReturn = bAllocateMemory(unMsgIDRangeCount);

        if(bReturn != FALSE)
        {
            // If generic message handler exists.
            if(m_omStrGenericHandlerName.IsEmpty() != TRUE)
            {
                TRY
                {
                    if (m_eBus == CAN) //If Can use a separate variable itself 'm_pFGenericMsgHandlerCAN'
                    {
                        m_pFGenericMsgHandlerCAN =
                        (PFMSG_HANDLER_CAN)GetProcAddress(m_hDllModule,
                        LPCSTR(m_omStrGenericHandlerName.GetBuffer(MAX_PATH)));

                        if(m_pFGenericMsgHandlerCAN != NULL )
                        {
                            m_psMsgHandlersInfo->m_bGenericHandlerON = TRUE;
                        }
                        else
                        {
                            bReturn = FALSE ;
                        }
                    }
                    else //any other bus use the generic variable m_pFGenericMsgHandler
                    {
                        m_pFGenericMsgHandler =
                        (PFMSG_HANDLER)GetProcAddress(m_hDllModule,
                        LPCSTR(m_omStrGenericHandlerName.GetBuffer(MAX_PATH)));

                        if(m_pFGenericMsgHandler != NULL )
                        {
                            m_psMsgHandlersInfo->m_bGenericHandlerON = TRUE;
                        }
                        else
                        {
                            bReturn = FALSE ;
                        }
                    }
                }
                CATCH_ALL(pomException)
                {
                    if(pomException != NULL )
                    {
                        TCHAR acErrorMsg[defSIZE_OF_ERROR_BUFFER];
                        CString omStrErrorMessage =_T("");
                        // Get the exception error message
                        pomException->GetErrorMessage(acErrorMsg,
                                                      sizeof(acErrorMsg));
                        omStrErrorMessage.Format( defSTR_ERROR_LOADING_HANDLER,
                                                  defSTR_MESSAGE_HANDLER,
                                                  acErrorMsg);
                        //Display the error
                        /*AfxMessageBox( omStrErrorMessage ,
                                       MB_ICONERROR| MB_SYSTEMMODAL|MB_OK,0);*/
                        omErrorArray.Add(omStrErrorMessage);
                        pomException->Delete();
                    }

                    bReturn = FALSE;
                }
                END_CATCH_ALL
            }
            else
            {
                m_psMsgHandlersInfo->m_bGenericHandlerON = FALSE;
            }

            if(bReturn != FALSE )
            {
                m_omMsgHandlerMapCAN.RemoveAll();
                m_omMsgHandlerMap.RemoveAll();
                bReturn = bInitMsgIDandNameHandlStruct(unMsgIDandNameCount, omErrorArray);

                if(bReturn != FALSE )
                {
                    bReturn = bInitMsgIDRangeHandlStruct(unMsgIDRangeCount, omErrorArray);
                }
            }
        }
    }

    return bReturn;
}
/******************************************************************************/
/*  Function Name    :  vInitOnKeyStruct                                      */
/*  Input(s)         :                                                        */
/*  Output           :                                                        */
/*  Functionality    :  Initialise the array of key structure by dynamically  */
/*                      creating it and filling all available information     */
/*  Member of        :  CExecuteFunc                                          */
/*  Friend of        :      -                                                 */
/*  Author(s)        :  Amitesh Bharti                                        */
/*  Date Created     :  28.02.2002                                            */
/*  Modification By  :  Amitesh Bharti                                        */
/*  Modification on  :  08.03.2002                                            */
/*  Modification By  :  Amitesh Bharti                                        */
/*  Modification on  :  08.03.2003, added init. for generic key handler       */
/*  Modification By  :  Raja N                                                */
/*  Modification on  :  28.05.2004, Removed hardcode string and replaced with */
/*                      #defined error string                                 */
/******************************************************************************/
BOOL CExecuteFunc::bInitOnKeyStruct(CStringArray& omErrorArray)
{
    UINT unKeyHandlerCount = 0;
    BOOL bReturn           = TRUE;
    unKeyHandlerCount      = (COMMANUINT)m_omStrArrayKeyHandlers.GetSize();

    if(unKeyHandlerCount>0)
    {
        if(m_psOnKeyHandlers != NULL )
        {
            delete []m_psOnKeyHandlers ;
            m_psOnKeyHandlers = NULL;
        }
        else
        {
            m_psOnKeyHandlers = new SKEYHANDLER[unKeyHandlerCount];
        }

        if(m_psOnKeyHandlers !=NULL)
        {
            UINT unGenric          = 0;

            for(    UINT unKeyCount = 0;
                    unKeyCount < unKeyHandlerCount;
                    unKeyCount++    )
            {
                UCHAR ucKeyVal = 0;
                CString omStrKeyHanlderName = _T("");
                INT nIndex      = 0;
                omStrKeyHanlderName = m_omStrArrayKeyHandlers.GetAt(unKeyCount);
                TRY
                {
                    nIndex = omStrKeyHanlderName.GetLength();

                    if(nIndex > 0 )
                    {
                        if(omStrKeyHanlderName.Find(_T("All")) == -1)
                        {
                            // Index will be less then the unKeyCount if genric
                            // handler is initialised.
                            m_psOnKeyHandlers
                            [unKeyCount - unGenric].pFKeyHandlers =
                            (PFKEY_HANDLER)GetProcAddress(m_hDllModule,
                            /*T2A*/(omStrKeyHanlderName.GetBuffer(MAX_PATH)));
                            // Get the last charactor from the key hanlder name
                            ucKeyVal= omStrKeyHanlderName.GetAt(nIndex-1);
                            m_psOnKeyHandlers[unKeyCount - unGenric].ucKey =
                            ucKeyVal;

                            if( m_psOnKeyHandlers
                            [unKeyCount - unGenric].pFKeyHandlers == NULL)
                            {
                                bReturn = FALSE;
                            }
                        }
                        else
                        {
                            m_pFGenericKeyHandler =
                                (PFKEY_HANDLER)GetProcAddress(m_hDllModule,
                                                              /*T2A*/(omStrKeyHanlderName.GetBuffer(MAX_PATH)));
                            // increment and use it for index of other
                            // key handlers.
                            unGenric++;

                            if( m_pFGenericKeyHandler == NULL)
                            {
                                bReturn = FALSE;
                            }
                        }
                    }
                }
                CATCH_ALL(pomException)
                {
                    if(pomException != NULL )
                    {
                        TCHAR acErrorMsg[defSIZE_OF_ERROR_BUFFER];
                        CString omStrErrorMessage =_T("");
                        // Get the exception error message
                        pomException->GetErrorMessage(acErrorMsg,
                                                      sizeof(acErrorMsg));
                        omStrErrorMessage.Format( defSTR_ERROR_LOADING_HANDLER,
                                                  defSTR_KEY_HANDLER,
                                                  acErrorMsg );
                        //Display the error
                        /*AfxMessageBox( omStrErrorMessage ,
                                       MB_ICONERROR| MB_SYSTEMMODAL|MB_OK,0);*/
                        omErrorArray.Add(omStrErrorMessage);
                        pomException->Delete();
                    }

                    bReturn = FALSE;
                }
                END_CATCH_ALL
            }
        }
        else
        {
            bReturn = FALSE;
        }
    }

    return bReturn;
}
/******************************************************************************/
/*  Function Name    :  vInitTimerStruct                                      */
/*  Input(s)         :                                                        */
/*  Output           :                                                        */
/*  Functionality    :  Initialise "pFTimerHandler" element of the array of   */
/*                      timer structure by getting the timer handler pointers */
/*  Member of        :  CExecuteFunc                                          */
/*  Friend of        :      -                                                 */
/*  Author(s)        :  Amitesh Bharti                                        */
/*  Date Created     :  28.02.2002                                            */
/*  Modification By  :  Amitesh Bharti                                        */
/*  Modification on  :  08.03.2002                                            */
/*  Modification By  :  Raja N                                                */
/*  Modification on  :  28.05.2004, Removed hardcode string and replaced with */
/*                      #defined error string                                 */
/*  Modification By  :  Anish                                                 */
/*  Modification on  :  14.01.2006, For timer structure  replaced array by    */
/*                      linked list                                           */
/******************************************************************************/
BOOL CExecuteFunc::bInitTimerStruct(CStringArray& omErrorArray)
{
    BOOL bReturn = TRUE;
    CString omStrTimerHandlerName = _T("");
    INT nTotalNumberOfTimer = (COMMANINT)m_omStrArrayTimerHandlers.GetSize();
    CString omStrErrorMessage =_T("");

    //PSTIMERSTRUCTURE psTimerStruct;
    for(INT nTimerCount = 0;
            nTimerCount<nTotalNumberOfTimer;
            nTimerCount++)
    {
        PSTIMERHANDLERLIST psTimerHandlerList=new STIMERHANDLERLIST ;

        if(psTimerHandlerList!=NULL)
        {
            psTimerHandlerList->sTimerHandler.pFTimerHandler=NULL;
            omStrTimerHandlerName = m_omStrArrayTimerHandlers.GetAt(nTimerCount);

            if(!omStrTimerHandlerName.IsEmpty())
            {
                TRY
                {
                    psTimerHandlerList->sTimerHandler.pFTimerHandler
                    =(PFTIMER_HANDLER)GetProcAddress(m_hDllModule,
                    /*T2A*/(omStrTimerHandlerName.GetBuffer(MAX_PATH)));

                    if(psTimerHandlerList->sTimerHandler.pFTimerHandler == NULL)
                    {
                        bReturn = FALSE;
                    }
                    else
                    {
                        INT nIndex = 0;
                        CString omStrTimerValue = _T("");
                        UINT unTimerValue = 0;

                        nIndex = omStrTimerHandlerName.ReverseFind('_');
                        omStrTimerValue = omStrTimerHandlerName.Right(
                            omStrTimerHandlerName.GetLength() - nIndex - 1);
                        unTimerValue = _tstoi(LPCTSTR(omStrTimerValue));

                        if(unTimerValue != 0 )
                        {
                            psTimerHandlerList->sTimerHandler.
                            omStrTimerHandlerName=omStrTimerHandlerName;
                            psTimerHandlerList->sTimerHandler.unTimerVal=unTimerValue;
                            psTimerHandlerList->sTimerHandler.hDllHandle=m_hDllModule;
                            psTimerHandlerList->sTimerHandler.bTimerSelected=FALSE;
                            psTimerHandlerList->sTimerHandler.bTimerType=FALSE;
                            psTimerHandlerList->sTimerHandler.unTimerID=0;
                            psTimerHandlerList->sTimerHandler.pomThreadHandle=NULL;
                            psTimerHandlerList->sTimerHandler.unCurrTime=0;
                            psTimerHandlerList->psNextTimer=NULL;

                            if(m_psFirstTimerStrList!=NULL)
                            {
                                m_psLastTimerStrList->psNextTimer=psTimerHandlerList;
                                m_psLastTimerStrList=psTimerHandlerList;
                            }
                            else
                            {
                                m_psFirstTimerStrList=psTimerHandlerList;
                                m_psLastTimerStrList=psTimerHandlerList;
                            }
                        }
                        else
                        {
                            bReturn = FALSE;
                        }

                    }
                }
                CATCH_ALL(pomException)
                {
                    if(pomException != NULL)
                    {
                        TCHAR acErrorMsg[defSIZE_OF_ERROR_BUFFER];
                        // Get the exception error message
                        pomException->GetErrorMessage(
                            acErrorMsg,sizeof(acErrorMsg));
                        omStrErrorMessage.Format( defSTR_ERROR_LOADING_HANDLER,
                                                  defSTR_TIMER_HANDLER,
                                                  acErrorMsg );
                        //Display the error
                        /*AfxMessageBox(omStrErrorMessage ,
                            MB_ICONERROR| MB_SYSTEMMODAL|MB_OK,nZERO);*/
                        omErrorArray.Add(omStrErrorMessage);
                        pomException->Delete();
                    }

                    bReturn = FALSE;
                }
                END_CATCH_ALL
            }
        }
    }

    return bReturn;
}

/******************************************************************************/
/*  Function Name    :  bInitErrorStruct                                      */
/*  Input(s)         :                                                        */
/*  Output           :                                                        */
/*  Functionality    :                                                        */

/*  Member of        :  CExecuteFunc                                          */
/*  Friend of        :      -                                                 */
/*  Author(s)        :  Amitesh Bharti                                        */
/*  Date Created     :  01.03.2003s                                           */
/*  Modification By  :  Raja N                                                */
/*  Modification on  :  28.05.2004, Removed hardcode string and replaced with */
/*                      #defined error string                                 */
/******************************************************************************/
BOOL CExecuteFunc::bInitErrorStruct(CStringArray& omErrorArray)
{
    UINT unErrorHandlerCount = 0;
    BOOL bReturn     = TRUE;
    unErrorHandlerCount = (COMMANUINT)m_omStrArrayErrorHandlers.GetSize();

    if(unErrorHandlerCount>0)
    {
        if(m_psOnErrorHandlers != NULL )
        {
            delete []m_psOnErrorHandlers ;
            m_psOnErrorHandlers = NULL;
        }
        else
        {
            m_psOnErrorHandlers = new SERRORHANDLER[unErrorHandlerCount];
        }

        if(m_psOnErrorHandlers !=NULL)
        {
            for(    UINT unErrorCount = 0;
                    unErrorCount < unErrorHandlerCount;
                    unErrorCount++    )
            {
                eERROR_STATE eErrorCode  = ERROR_FRAME;
                CString omStrErrorHandlerName = _T("");
                omStrErrorHandlerName =
                    m_omStrArrayErrorHandlers.GetAt(unErrorCount);
                TRY
                {
                    m_psOnErrorHandlers[unErrorCount].m_pFErrorHandlers
                    = (PFERROR_HANDLER)GetProcAddress(m_hDllModule,
                    /*T2A*/(omStrErrorHandlerName.GetBuffer(MAX_PATH)));

                    if(m_psOnErrorHandlers[unErrorCount].m_pFErrorHandlers ==
                    NULL)
                    {
                        bReturn = FALSE;
                    }

                    if(omStrErrorHandlerName.Find(_T("Error_Frame")) != -1)
                    {
                        eErrorCode = ERROR_FRAME;
                    }
                    else if(omStrErrorHandlerName.Find(_T("Error_Active")) != -1)
                    {
                        eErrorCode = ERROR_ACTIVE;
                    }
                    else if(omStrErrorHandlerName.Find(_T("Error_Passive")) != -1)
                    {
                        eErrorCode = ERROR_PASSIVE ;
                    }
                    else if(omStrErrorHandlerName.Find(_T("Bus_Off")) != -1)
                    {
                        eErrorCode = ERROR_BUS_OFF;
                    }
                    else if(omStrErrorHandlerName.Find(_T("Warning_Limit")) != -1)
                    {
                        eErrorCode = ERROR_WARNING_LIMIT;
                    }

                    m_psOnErrorHandlers[unErrorCount].m_eErrorCode = eErrorCode;
                }
                CATCH_ALL(pomException)
                {
                    if(pomException != NULL )
                    {
                        TCHAR acErrorMsg[defSIZE_OF_ERROR_BUFFER];
                        CString omStrErrorMessage =_T("");
                        // Get the exception error message
                        pomException->GetErrorMessage(acErrorMsg,
                                                      sizeof(acErrorMsg));
                        omStrErrorMessage.Format( defSTR_ERROR_LOADING_HANDLER,
                                                  defSTR_ERROR_HANDLER,
                                                  acErrorMsg );
                        //Display the error
                        /*AfxMessageBox( omStrErrorMessage ,
                                       MB_ICONERROR| MB_SYSTEMMODAL|MB_OK,0);*/
                        omErrorArray.Add(omStrErrorMessage);
                        pomException->Delete();
                    }

                    bReturn = FALSE;
                }
                END_CATCH_ALL
            }
        }
        else
        {
            bReturn = FALSE;
        }
    }

    return bReturn;
}

/******************************************************************************/
/*  Function Name    :  bInitEventStruct                                      */
/*  Input(s)         :                                                        */
/*  Output           :                                                        */
/*  Functionality    :                                                        */

/*  Member of        :  CExecuteFunc                                          */
/*  Friend of        :      -                                                 */
/*  Author(s)        :  Pradeep Kadoor                                        */
/*  Date Created     :  01.03.2003s                                           */
/******************************************************************************/
BOOL CExecuteFunc::bInitEventStructJ1939(CStringArray& omErrorArray)
{
    UINT unEventHandlerCount = 0;
    BOOL bReturn     = TRUE;
    unEventHandlerCount = (COMMANUINT)m_omStrArrayEventHandlers.GetSize();

    if(unEventHandlerCount > 0)
    {
        if(m_psOnEventHandlers != NULL )
        {
            delete []m_psOnEventHandlers ;
            m_psOnEventHandlers = NULL;
        }
        else
        {
            m_psOnEventHandlers = new SEVENTHANDLER[unEventHandlerCount];
        }

        if(m_psOnEventHandlers !=NULL)
        {
            for(UINT unCount = 0; unCount < unEventHandlerCount; unCount++)
            {
                CString omStrEventHandlerName = _T("");
                omStrEventHandlerName =
                    m_omStrArrayEventHandlers.GetAt(unCount);
                TRY
                {
                    /*USES_CONVERSION;*/
                    m_psOnEventHandlers[unCount].m_pFEventHandlers = (PFEVENT_HANDLER)GetProcAddress(m_hDllModule,
                    omStrEventHandlerName.GetBuffer(MAX_CHAR));

                    if(m_psOnEventHandlers[unCount].m_pFEventHandlers == NULL)
                    {
                        bReturn = FALSE;
                    }
                    else
                    {
                        if(omStrEventHandlerName.Find(_T("DataConf")) != -1)
                        {
                            m_psOnEventHandlers[unCount].m_byEventType = 0x0;
                        }
                        else if(omStrEventHandlerName.Find(_T("AddressClaim")) != -1)
                        {
                            m_psOnEventHandlers[unCount].m_byEventType = 0x1;
                        }
                    }

                }
                CATCH_ALL(pomException)
                {
                    if(pomException != NULL )
                    {
                        TCHAR acErrorMsg[defSIZE_OF_ERROR_BUFFER];
                        CString omStrErrorMessage =_T("");
                        // Get the exception error message
                        pomException->GetErrorMessage(acErrorMsg,
                                                      sizeof(acErrorMsg));
                        omStrErrorMessage.Format( defSTR_ERROR_LOADING_HANDLER,
                                                  defSTR_EVENT_HANDLER,
                                                  acErrorMsg );
                        //Display the error
                        /*AfxMessageBox( omStrErrorMessage ,
                                       MB_ICONERROR| MB_SYSTEMMODAL|MB_OK,0);*/
                        omErrorArray.Add(omStrErrorMessage);
                        pomException->Delete();
                    }

                    bReturn = FALSE;
                }
                END_CATCH_ALL
            }
        }
        else
        {
            bReturn = FALSE;
        }
    }

    return bReturn;
}

/******************************************************************************/
/*  Function Name    :  bInitDLLStruct                                        */
/*  Input(s)         :  0                                                  */
/*  Output           :  BOOL                                                  */
/*  Functionality    :  Initialize the structure for DLL handler with the     */
/*                      function to be excecuted                              */
/*                                                                            */
/*  Member of        :  CExecuteFunc                                          */
/*  Friend of        :      -                                                 */
/*  Author(s)        :  Ravikumar Patil                                       */
/*  Date Created     :  10.03.2003                                            */
/*  Modification By  :  Raja N                                                */
/*  Modification on  :  28.05.2004, Removed hardcode string and replaced with */
/*                      #defined error string                                 */
/******************************************************************************/
BOOL CExecuteFunc::bInitDLLStruct(CStringArray& omErrorArray)
{
    UINT unDLLHandlerCount = 0;
    BOOL bReturn     = TRUE;
    unDLLHandlerCount = (COMMANUINT)m_omStrArrayDLLHandlers.GetSize();

    if(unDLLHandlerCount>0)
    {
        if(m_psOnDLLHandlers != NULL )
        {
            delete []m_psOnDLLHandlers ;
            m_psOnDLLHandlers = NULL;
        }
        else
        {
            m_psOnDLLHandlers = new SDLLHANDLER[unDLLHandlerCount];
        }

        if(m_psOnDLLHandlers !=NULL)
        {
            for(    UINT unDLLCount = 0;
                    unDLLCount < unDLLHandlerCount;
                    unDLLCount++    )
            {
                eDLLHANDLER eDLLCode  = DLL_UNLOAD ;
                CString omStrDLLHandlerName = _T("");
                omStrDLLHandlerName = m_omStrArrayDLLHandlers.GetAt(unDLLCount);
                TRY
                {
                    m_psOnDLLHandlers[unDLLCount].m_pFDLLHandlers
                    = (PFDLL_HANDLER)GetProcAddress(m_hDllModule,
                    /*T2A*/(omStrDLLHandlerName.GetBuffer(MAX_PATH)));

                    if(m_psOnDLLHandlers[unDLLCount].m_pFDLLHandlers == NULL)
                    {
                        bReturn = FALSE;
                    }

                    if(omStrDLLHandlerName.Find(_T("Load")) != -1)
                    {
                        eDLLCode = DLL_LOAD ;
                    }
                    else if(omStrDLLHandlerName.Find(_T("Unload")) != -1)
                    {
                        eDLLCode = DLL_UNLOAD ;
                    }

                    m_psOnDLLHandlers[unDLLCount].m_eDLLHandler = eDLLCode;
                }
                CATCH_ALL(pomException)
                {
                    if(pomException != NULL )
                    {
                        TCHAR acErrorMsg[defSIZE_OF_ERROR_BUFFER];
                        CString omStrErrorMessage =_T("");
                        // Get the exception error message
                        pomException->GetErrorMessage(acErrorMsg,
                                                      sizeof(acErrorMsg));
                        omStrErrorMessage.Format( defSTR_ERROR_LOADING_HANDLER,
                                                  defSTR_DLL_HANDLER,
                                                  acErrorMsg );
                        //Display the error
                        /*AfxMessageBox( omStrErrorMessage ,
                                       MB_ICONERROR| MB_SYSTEMMODAL|MB_OK,0);*/
                        omErrorArray.Add(omStrErrorMessage);
                        pomException->Delete();
                    }

                    bReturn = FALSE;
                }
                END_CATCH_ALL
            }
        }
        else
        {
            bReturn = FALSE;
        }
    }

    return bReturn;
}
/******************************************************************************/
/*  Function Name    :  vCatagoriseMsgHandlers                                */
/*  Input(s)         :  omStrFuncName :Message Handler Name                   */
/*  Output           :                                                        */
/*  Functionality    :  This function is called to catogorise the message     */
/*                      structure to stop execution of timer. This function   */
/*                      will catogorise the type of message hanlders and add  */
/*                      the corresponding name in the correspndingCStringArray*/
/*                      data member.                                          */
/*  Member of        :  CExecuteFunc                                          */
/*  Friend of        :      -                                                 */
/*  Author(s)        :  Amitesh Bharti                                        */
/*  Date Created     :  05.03.2003                                            */
/*  Modification By  :                                                        */
/*  Modification on  :                                                        */
/******************************************************************************/
void CExecuteFunc::vCatagoriseMsgHandlers(const CString& omStrFuncName)
{
    CString omMsgHandlerName     = CGlobalObj::omGetBusSpecMsgHndlrName(m_eBus);
    CString omStrGenericHandler  = omMsgHandlerName;
    CString omStrMsgIDHandler    = omMsgHandlerName;
    CString omStrMsgNameHandler  = omMsgHandlerName;
    CString omStrMsgRangeHandler = omMsgHandlerName;
    omStrGenericHandler  += defMSG_ALL;
    omStrMsgIDHandler    += defMSG_ID_HANDLER;
    omStrMsgNameHandler  += defMSG_NAME_HANDLER;
    omStrMsgRangeHandler += defMSG_IDRANGE_HANDLER;
    CString omStrFuncType = omStrFuncName;
    omStrFuncType = omStrFuncName.Left(omStrFuncName.Find('_')+1);

    // Check the type and add the function name in the corresponding data
    // member
    if(omStrFuncName.Find(omStrGenericHandler) != -1 )// Generic Msg Handler
    {
        m_omStrGenericHandlerName = omStrFuncName;
    }
    else if (omStrFuncType.Find(omStrMsgIDHandler) != -1 )// Msg ID Handler
    {
        m_omStrArrayMsgIDandName.Add(omStrFuncName);
    }
    else if (omStrFuncType.Find(omStrMsgNameHandler) != -1 )//Msg Name Handler
    {
        m_omStrArrayMsgIDandName.Add(omStrFuncName);
    }
    else if (omStrFuncType.Find(omStrMsgRangeHandler) != -1 )//Msg IDRange Handl
    {
        m_omStrArrayMsgRange.Add(omStrFuncName);
    }
}
/******************************************************************************/
/*  Function Name    :  bAllocateMemory                                       */
/*  Input(s)         :  unMsgIDCount: Message name and Id handler count       */
/*                      unMsgRangeCount:Message name and Id handler count     */
/*  Output           :  TRUE or FALSE                                         */
/*  Functionality    :  This function is called to allocate memory for        */
/*                      message strucutre depending upon the number of        */
/*                      passed as paramemter                                  */
/*  Member of        :  CExecuteFunc                                          */
/*  Friend of        :      -                                                 */
/*  Author(s)        :  Amitesh Bharti                                        */
/*  Date Created     :  05.03.2003                                            */
/*  Modification By  :  Raja N                                                */
/*  Modification on  :  Removed allocation of memory for the message handler  */
/*                      ID array m_punMsgID as it has been replaced with CMap */
/*  Modification By  :  Raja N                                                */
/*  Modification on  :  Removed the first parameter unMsgIDCount as this is   */
/*                      not required. This was used to allocate memory for    */
/*                      message ID/Name specific handlers                     */
/******************************************************************************/
BOOL CExecuteFunc::bAllocateMemory(UINT unMsgRangeCount)
{
    BOOL bReturn = TRUE;

    // Check if the memory is not deleted. If so delete it.
    if(m_psMsgHandlersInfo != NULL )
    {
        if(m_psMsgHandlersInfo->m_psRange != NULL )
        {
            delete [] m_psMsgHandlersInfo->m_psRange;
            m_psMsgHandlersInfo->m_psRange = NULL;
        }

        delete m_psMsgHandlersInfo;
        m_psMsgHandlersInfo = NULL;
    }

    // Allocated memory for message handler information structure to be posted
    // to the message window module.
    if(m_psMsgHandlersInfo == NULL)
    {
        m_psMsgHandlersInfo = new SMSGINFO_FOR_HANDLER;

        if(m_psMsgHandlersInfo != NULL )
        {
            if(unMsgRangeCount > 0 )
            {
                m_psMsgHandlersInfo->m_psRange =
                    new SMSG_RANGE[unMsgRangeCount];

                if(m_psMsgHandlersInfo->m_psRange == NULL)
                {
                    bReturn = FALSE;
                }
            }
            else
            {
                m_psMsgHandlersInfo->m_psRange = NULL;
            }
        }
        else
        {
            bReturn = FALSE;
        }
    }
    else
    {
        bReturn = FALSE;
    }

    if(unMsgRangeCount > 0)
    {
        // Check if the memory is not deleted. If so delete it.
        if (m_eBus == CAN) // If can then use a different structure itself
        {
            if(m_psOnMsgIDRangeHandlersCAN != NULL )
            {
                delete []m_psOnMsgIDRangeHandlersCAN;
                m_psOnMsgIDRangeHandlersCAN = NULL;
            }

            // Allocate memory for message Id range handlers
            if(m_psOnMsgIDRangeHandlersCAN == NULL)
            {
                m_psOnMsgIDRangeHandlersCAN =
                    new SMSGID_RANGE_HANDLER_CAN[unMsgRangeCount];

                if(m_psOnMsgIDRangeHandlersCAN == NULL )
                {
                    bReturn = FALSE;
                }
            }
            else
            {
                bReturn = FALSE;
            }
        }
        else //If its any other bus use generics structure
        {
            if(m_psOnMsgIDRangeHandlers != NULL )
            {
                delete []m_psOnMsgIDRangeHandlers;
                m_psOnMsgIDRangeHandlers = NULL;
            }

            // Allocate memory for message Id range handlers
            if(m_psOnMsgIDRangeHandlers == NULL)
            {
                m_psOnMsgIDRangeHandlers =
                    new SMSGID_RANGE_HANDLER[unMsgRangeCount];

                if(m_psOnMsgIDRangeHandlers == NULL )
                {
                    bReturn = FALSE;
                }
            }
            else
            {
                bReturn = FALSE;
            }
        }
    }

    return bReturn;
}
/******************************************************************************/
/*  Function Name    :  bInitMsgIDandNameHandlStruct                          */
/*  Input(s)         :  unMsgIDandNameCount:Message name and Id handler count */
/*  Output           :  TRUE or FALSE                                         */
/*  Functionality    :  This function is called to initialise the structurefor*/
/*                      message name and Id handlers                          */
/*  Member of        :  CExecuteFunc                                          */
/*  Friend of        :      -                                                 */
/*  Author(s)        :  Amitesh Bharti                                        */
/*  Date Created     :  05.03.2003                                            */
/*  Modification By  :  Amitesh Bharti                                        */
/*  Modification on  :  18.04.2003, if message Id is less then 0 return  FALSE*/
/*  Modification By  :  Raja N                                                */
/*  Modification on  :  10.05.2004, Changed the msg handler storage to CMap to*/
/*                      reduce the search time and remoced hardcoded strings  */
/*  Modification By  :  Anish                                                 */
/*  Modification on  :  10.04.09, FRAME modification
/******************************************************************************/
BOOL CExecuteFunc::bInitMsgIDandNameHandlStruct(UINT unMsgIDandNameCount,
        CStringArray& omErrorArray)
{
    CString omStrFuncName     = _T("");
    CString omStrMsgIDType    = _T("");
    CString omStrMsgNameType  = _T("");
    CString omStrTemp         = _T("");
    CString omStrMsgNameOrID  = _T("");
    BOOL    bReturn           = TRUE;
    omStrMsgIDType    = CGlobalObj::omGetBusSpecMsgHndlrName(m_eBus);
    omStrMsgIDType   += defMSG_ID_HANDLER;
    omStrMsgNameType  = CGlobalObj::omGetBusSpecMsgHndlrName(m_eBus);
    omStrMsgNameType += defMSG_NAME_HANDLER;

    if(m_psMsgHandlersInfo != NULL )
    {
        //CMsgSignal * pouDatabase = theApp.m_pouMsgSignal;
        for(UINT i=0; i<unMsgIDandNameCount; i++ )
        {
            INT nMsgID = -1;
            omStrFuncName    = m_omStrArrayMsgIDandName.GetAt(i);
            // Get the message name or Id from function prototype.
            omStrMsgNameOrID = omStrFuncName.Right(omStrFuncName.GetLength() -
                                                   omStrFuncName.Find(_T("_")) - 1 );
            // Get the handler type
            omStrTemp        = omStrFuncName.Left(omStrFuncName.Find(_T("_")) + 1);
            omStrTemp.TrimLeft();
            omStrTemp.TrimRight();

            if(omStrTemp.Compare(omStrMsgNameType) == 0)
            {
                SMSG_NAME_CODE sMsgCode;
                sMsgCode.m_omMsgName = omStrMsgNameOrID;
                POSITION pos = CGlobalObj::ouGetObj(m_eBus).m_odMsgNameMsgCodeList.Find(sMsgCode);

                if (pos != NULL)
                {
                    sMsgCode = CGlobalObj::ouGetObj(m_eBus).m_odMsgNameMsgCodeList.GetAt(pos);
                    nMsgID = sMsgCode.m_dwMsgCode;
                }
            }
            else if(omStrTemp.Compare(omStrMsgIDType) == 0 )
            {
                TCHAR* pcStopStr = NULL;
                nMsgID = static_cast <INT>
                         (_tcstol(omStrMsgNameOrID,&pcStopStr,16) );
            }

            TRY
            {
                // Get the proc Address
                if (m_eBus == CAN) //If CAN use separate structure
                {
                    PFMSG_HANDLER_CAN pfTemp = ( PFMSG_HANDLER_CAN)
                    GetProcAddress(m_hDllModule,omStrFuncName.GetBuffer(MAX_PATH));

                    if( pfTemp != NULL)
                    {
                        SMSGHANDLERDATA_CAN sMsgHandler;
                        // Assign Proc Address
                        sMsgHandler.m_pFMsgHandler = pfTemp;
                        m_omMsgHandlerMapCAN[nMsgID] = sMsgHandler;
                    }
                    else
                    {
                        bReturn = FALSE;
                    }
                }
                else// if any other bus use generic structure
                {
                    PFMSG_HANDLER pfTemp = ( PFMSG_HANDLER)
                    GetProcAddress(m_hDllModule,LPTSTR(omStrFuncName.GetBuffer(MAX_PATH)));

                    if( pfTemp != NULL)
                    {
                        SMSGHANDLERDATA sMsgHandler;
                        // Assign Proc Address
                        sMsgHandler.m_pFMsgHandler = pfTemp;
                        m_omMsgHandlerMap[nMsgID] = sMsgHandler;
                    }
                    else
                    {
                        bReturn = FALSE;
                    }
                }
            }
            CATCH_ALL(pomException)
            {
                if(pomException != NULL )
                {
                    TCHAR acErrorMsg[defSIZE_OF_ERROR_BUFFER];
                    CString omStrErrorMessage =_T("");
                    // Get the exception error message
                    pomException->GetErrorMessage(acErrorMsg,
                                                  sizeof(acErrorMsg));
                    omStrErrorMessage.Format( defSTR_ERROR_LOADING_HANDLER,
                                              omStrFuncName,
                                              acErrorMsg );
                    omErrorArray.Add(omStrErrorMessage);
                    pomException->Delete();
                }

                bReturn = FALSE;
            }
            END_CATCH_ALL
        }
    }

    return bReturn;
}
/******************************************************************************/
/*  Function Name    :  bInitMsgIDRangeHandlStruct                            */
/*  Input(s)         :  unMsgIDRangeCount:Message name and Id handler count   */
/*  Output           :  TRUE or FALSE                                         */
/*  Functionality    :  This function is called to initialise the structurefor*/
/*                      message Id range handlers.                            */
/*  Member of        :  CExecuteFunc                                          */
/*  Friend of        :      -                                                 */
/*  Author(s)        :  Amitesh Bharti                                        */
/*  Date Created     :  05.03.2003                                            */
/*  Modification By  :  Raja N                                                */
/*  Modification on  :  29.04.2004, Modified m_pFMsgHandler assignment        */
/*                      statement to refer correct index - Random Testing     */
/*  Modification By  :  Raja N                                                */
/*  Modification on  :  10.05.2004, Added initialisation of CMap with the msg */
/*                      data from the database to avoid iteration every time  */
/*                      to find DLC and Frame Type                            */
/******************************************************************************/
BOOL CExecuteFunc::bInitMsgIDRangeHandlStruct(UINT unMsgIDRangeCount,
        CStringArray& omErrorArray)
{
    CString omStrFuncName     = _T("");
    CString omStrMsgIDFrom    = _T("");
    CString omStrMsgIDTo      = _T("");
    CString omStrTemp         = _T("");
    BOOL    bReturn           = TRUE;

    if(m_psMsgHandlersInfo != NULL )
    {
        m_psMsgHandlersInfo->m_unRangeCount = unMsgIDRangeCount;

        for(UINT i=0; i<unMsgIDRangeCount; i++ )
        {
            omStrFuncName    = m_omStrArrayMsgRange.GetAt(i);
            // Get the message ID from the string
            omStrTemp        = omStrFuncName.Right(omStrFuncName.GetLength() -
                                                   omStrFuncName.Find(_T("_")) - 1 );
            omStrMsgIDFrom = omStrTemp.Left(omStrTemp.Find(_T("_")));
            omStrMsgIDTo   = omStrTemp;
            omStrMsgIDTo.Replace(omStrMsgIDFrom,_T(""));
            omStrMsgIDTo.Replace(_T("_"),_T(""));
            TCHAR* pcStopStr = NULL;
            UINT unMsgIDFrom  = _tcstol((LPCTSTR )omStrMsgIDFrom,&pcStopStr,16);
            UINT unMsgIDTo = _tcstol((LPCTSTR )omStrMsgIDTo,&pcStopStr,16);
            TRY
            {
                if (m_eBus == CAN) // Use a separate structure 'm_psOnMsgIDRangeHandlersCAN'
                {
                    // Assign proc address to correct index
                    m_psOnMsgIDRangeHandlersCAN[i].m_pFMsgHandler =
                    (PFMSG_HANDLER_CAN)GetProcAddress(m_hDllModule,
                    LPTSTR(omStrFuncName.GetBuffer(MAX_PATH)));
                    m_psOnMsgIDRangeHandlersCAN[i].m_sMsgIDRange.m_unFrom =
                    unMsgIDFrom;
                    m_psOnMsgIDRangeHandlersCAN[i].m_sMsgIDRange.m_unTo = unMsgIDTo;
                    m_psMsgHandlersInfo->m_psRange[i].m_unFrom = unMsgIDFrom;
                    m_psMsgHandlersInfo->m_psRange[i].m_unTo   = unMsgIDTo;

                    if(   m_psOnMsgIDRangeHandlersCAN[i].m_pFMsgHandler == NULL
                    || unMsgIDFrom < 0
                    || unMsgIDTo   < 0  )
                    {
                        bReturn = FALSE;
                    }
                }
                else //For any other bus use generic structure 'm_psOnMsgIDRangeHandlers'
                {
                    // Assign proc address to correct index
                    m_psOnMsgIDRangeHandlers[i].m_pFMsgHandler =
                    (PFMSG_HANDLER)GetProcAddress(m_hDllModule,
                    LPTSTR(omStrFuncName.GetBuffer(MAX_PATH)));
                    m_psOnMsgIDRangeHandlers[i].m_sMsgIDRange.m_unFrom =
                    unMsgIDFrom;
                    m_psOnMsgIDRangeHandlers[i].m_sMsgIDRange.m_unTo = unMsgIDTo;
                    m_psMsgHandlersInfo->m_psRange[i].m_unFrom = unMsgIDFrom;
                    m_psMsgHandlersInfo->m_psRange[i].m_unTo   = unMsgIDTo;

                    if(   m_psOnMsgIDRangeHandlers[i].m_pFMsgHandler == NULL
                    || unMsgIDFrom < 0
                    || unMsgIDTo   < 0  )
                    {
                        bReturn = FALSE;
                    }
                }
            }
            CATCH_ALL(pomException)
            {
                if(pomException != NULL )
                {
                    TCHAR acErrorMsg[defSIZE_OF_ERROR_BUFFER];
                    CString omStrErrorMessage =_T("");
                    // Get the exception error message
                    pomException->GetErrorMessage(acErrorMsg,
                                                  sizeof(acErrorMsg));
                    omStrErrorMessage.Format( defSTR_ERROR_LOADING_HANDLER,
                                              omStrFuncName,
                                              acErrorMsg );
                    omErrorArray.Add(omStrErrorMessage);
                    pomException->Delete();
                }

                bReturn = FALSE;
            }
            END_CATCH_ALL
        }
    }

    return bReturn;
}

/******************************************************************************/
/*  Function Name    :  pFSearchMsgIdRangeHandler                             */
/*  Input(s)         :  unMsgID:Message Id                                    */
/*  Output           :  PFMSG_HANDLER: Pointer to message handler function    */
/*  Functionality    :  This function is called to initialise get the matching*/
/*                      message id range handlers corresponding to message ID */
/*                      passed as parameter.                                  */
/*  Member of        :  CExecuteFunc                                          */
/*  Friend of        :      -                                                 */
/*  Author(s)        :  Amitesh Bharti                                        */
/*  Date Created     :  05.03.2003                                            */
/*  Modification By  :                                                        */
/*  Modification on  :                                                        */
/******************************************************************************/
PFMSG_HANDLER CExecuteFunc::pFSearchMsgIdRangeHandler(UINT unMsgID)
{
    UINT unMsgHandlerCount   = 0;
    BOOL bMsgHandlerExecuted = FALSE;
    UINT unMsgIDFrom         = 0;
    UINT unMsgIDTo           = 0;
    PFMSG_HANDLER pFMsgHandler = NULL;
    unMsgHandlerCount = (COMMANUINT)m_omStrArrayMsgRange.GetSize();
    UINT unMsgCount = 0;

    while (unMsgCount < unMsgHandlerCount && bMsgHandlerExecuted != TRUE)
    {
        unMsgIDTo   =
            m_psOnMsgIDRangeHandlers[unMsgCount].m_sMsgIDRange.m_unTo;
        unMsgIDFrom =
            m_psOnMsgIDRangeHandlers[unMsgCount].m_sMsgIDRange.m_unFrom;

        if (unMsgID >=unMsgIDFrom  && unMsgID <= unMsgIDTo)
        {
            if (m_psOnMsgIDRangeHandlers[unMsgCount].m_pFMsgHandler != NULL)
            {
                pFMsgHandler =
                    m_psOnMsgIDRangeHandlers[unMsgCount].m_pFMsgHandler;
            }

            bMsgHandlerExecuted = TRUE;
        }

        unMsgCount++;
    }

    return pFMsgHandler;
}

/******************************************************************************/
/*  Function Name    :  pFSearchMsgIdRangeHandlerCAN                             */
/*  Input(s)         :  unMsgID:Message Id                                    */
/*  Output           :  PFMSG_HANDLER: Pointer to message handler function    */
/*  Functionality    :  This function is called to initialise get the matching*/
/*                      message id range handlers corresponding to message ID */
/*                      passed as parameter. Only CAN                                  */
/*  Member of        :  CExecuteFunc                                          */
/*  Friend of        :      -                                                 */
/*  Author(s)        :  Amitesh Bharti                                        */
/*  Date Created     :  05.03.2003                                            */
/*  Modification By  :                                                        */
/*  Modification on  :                                                        */
/******************************************************************************/
PFMSG_HANDLER_CAN CExecuteFunc::pFSearchMsgIdRangeHandlerCAN(UINT unMsgID)
{
    UINT unMsgHandlerCount   = 0;
    BOOL bMsgHandlerExecuted = FALSE;
    UINT unMsgIDFrom         = 0;
    UINT unMsgIDTo           = 0;
    PFMSG_HANDLER_CAN pFMsgHandler = NULL;
    unMsgHandlerCount = (COMMANUINT)m_omStrArrayMsgRange.GetSize();
    UINT unMsgCount = 0;

    while (unMsgCount < unMsgHandlerCount && bMsgHandlerExecuted != TRUE)
    {
        unMsgIDTo   =
            m_psOnMsgIDRangeHandlersCAN[unMsgCount].m_sMsgIDRange.m_unTo;
        unMsgIDFrom =
            m_psOnMsgIDRangeHandlersCAN[unMsgCount].m_sMsgIDRange.m_unFrom;

        if (unMsgID >=unMsgIDFrom  && unMsgID <= unMsgIDTo)
        {
            if (m_psOnMsgIDRangeHandlersCAN[unMsgCount].m_pFMsgHandler != NULL)
            {
                pFMsgHandler =
                    m_psOnMsgIDRangeHandlersCAN[unMsgCount].m_pFMsgHandler;
            }

            bMsgHandlerExecuted = TRUE;
        }

        unMsgCount++;
    }

    return pFMsgHandler;
}


/******************************************************************************/
/*  Function Name    :  vEnableDisableAllTimers                               */
/*  Input(s)         :  BOOL                                                  */
/*  Output           :  void                                                  */
/*  Functionality    :  This function is will called Set/Reset all timer      */
/*                      handlers. Member function of CSetResetTimer class     */
/*                      to change the state if Enable/Disable All the Timer   */
/*                      Handlers. This is a wrapper function to               */
/*                      vSetResetAllTimers of the class CSetResetTimer        */
/*  Member of        :  CExecuteFunc                                          */
/*  Friend of        :      -                                                 */
/*  Author(s)        :  Raja N                                                */
/*  Date Created     :  04.02.2004                                            */
/*  Modification By  :  Anish kumar                                           */
/*  Modification on  :  14.01.06 ,Changed for Linked list                    */
/******************************************************************************/
void CExecuteFunc::vEnableDisableAllTimers(BOOL bEnable)
{
    PSTIMERHANDLERLIST psTimerListPtr= m_psFirstTimerStrList;

    while(psTimerListPtr!=NULL)
    {
        psTimerListPtr->sTimerHandler.bTimerSelected=bEnable;
        psTimerListPtr->sTimerHandler.unCurrTime=0;
        psTimerListPtr=psTimerListPtr->psNextTimer;
    }

    CExecuteManager::ouGetExecuteManager(m_eBus).
    vSetResetNodeTimers(this,bEnable);
    //Update timer handler
    m_sNodeInfo.m_bTimerHandlersEnabled = bEnable;
    CSimSysNodeInfo* pNodeInfo =
        CSimSysManager::ouGetSimSysManager(m_eBus).pomGetSimSysNodeInfo();
    PSNODEINFO psNodeInfo = NULL;

    if( pNodeInfo != NULL )
    {
        psNodeInfo = pNodeInfo->psGetSimSysNodePointer(STR_EMPTY ,
                     m_sNodeInfo.m_omStrNodeName);

        if(psNodeInfo != NULL)
        {
            psNodeInfo->m_bTimerHandlersEnabled = bEnable;
        }
    }
}



/*******************************************************************************
/*  Function Name    :  hGetDllHandle                                         */
/*  Input(s)         :  void                                                  */
/*  Output           :  HMODULE                                               */
/*  Functionality    :  This function is will called to get handle of DLL     */
/*                      associated to this object                             */
/*  Member of        :  CExecuteFunc                                          */
/*  Friend of        :      -                                                 */
/*  Author(s)        :  Anish Kumar                                           */
/*  Date Created     :  19.12.2005                                           */
/*  Modification By  :                                                        */
/*  Modification on  :                                                        */
/******************************************************************************/

HMODULE CExecuteFunc::hGetDllHandle()
{
    return m_hDllModule;
}
/*******************************************************************************
    Function Name    :  bActivateDeactivateHandlers
    Input(s)         :  BOOL
    Output           :  BOOL
    Functionality    :  This function enable/disable all the handlers according
                        to the input.While disabling handler it will stop the
                        threads executing them.
    Member of        :  CExecuteFunc
    Author(s)        :  Anish kumar
    Date Created     :  19.12.05
********************************************************************************/
BOOL CExecuteFunc::bActivateDeactivateHandlers(BOOL bCurrStatus)
{
    //*********** check Start/Stop Timers
    vEnableDisableAllTimers(bCurrStatus);
    BYTE byAction=0;

    if( bCurrStatus == FALSE)
    {
        /// Stop all executing handlers
        if( m_sNodeInfo.m_bMsgHandlersEnabled== TRUE)
        {
            m_bStopMsgHandlers = TRUE;
            // Stop only running threads
            // Don't call destroy threads
            byAction |= BIT_MSG_HANDLER_THREAD;
        }

        if( m_sNodeInfo.m_bKeyHandlersEnabled== TRUE)
        {
            byAction |= BIT_KEY_HANDLER_THREAD;
            m_bStopKeyHandlers = TRUE;
        }

        if( m_sNodeInfo.m_bErrorHandlersEnabled == TRUE)
        {
            byAction |= BIT_ERROR_HANDLER_THREAD;
            m_bStopErrorHandlers = TRUE;
        }
    }
    else
    {
        m_bStopMsgHandlers=FALSE;
        m_bStopKeyHandlers=FALSE;
        m_bStopErrorHandlers=FALSE;
    }

    CSimSysNodeInfo* pNodeInfo = CSimSysManager::ouGetSimSysManager(m_eBus).pomGetSimSysNodeInfo();
    PSNODEINFO psNodeInfo = NULL;

    if( pNodeInfo != NULL )
    {
        psNodeInfo = pNodeInfo->psGetSimSysNodePointer(STR_EMPTY
                     , m_sNodeInfo.m_omStrNodeName);

        if(psNodeInfo != NULL)
        {
            psNodeInfo->m_bIsAllHandlersEnabled= bCurrStatus;
            psNodeInfo->m_bMsgHandlersEnabled= bCurrStatus;
            psNodeInfo->m_bKeyHandlersEnabled= bCurrStatus;
            psNodeInfo->m_bErrorHandlersEnabled= bCurrStatus;
            psNodeInfo->m_bTimerHandlersEnabled= bCurrStatus;
            psNodeInfo->m_bDllHandlersEnabled=bCurrStatus;
        }
    }

    m_sNodeInfo.m_bIsAllHandlersEnabled= bCurrStatus;
    m_sNodeInfo.m_bMsgHandlersEnabled= bCurrStatus;
    m_sNodeInfo.m_bKeyHandlersEnabled= bCurrStatus;
    m_sNodeInfo.m_bErrorHandlersEnabled= bCurrStatus;
    m_sNodeInfo.m_bTimerHandlersEnabled= bCurrStatus;
    m_sNodeInfo.m_bDllHandlersEnabled=bCurrStatus;

    //*********** check Start/Stop Timers

    // Kill all threads now
    if( bCurrStatus == FALSE)
    {
        vDestroyUtilityThreads( 500, byAction);
    }
    else if(m_pomMsgHandlerThrd == NULL)
    {
        m_pomMsgHandlerThrd=AfxBeginThread(unReadNodeMsgHandlerBuffer,this);
    }

    return true;
}

/**********************************************************************************
    Function Name    :  bEnableDisableKeyHandlers
    Input(s)         :  BOOL
    Output           :  BOOL
    Functionality    :  This function enable/disable all key handlers according
                        to the input.While disabling handler it will stop the
                        threads executing them.
    Member of        :  CExecuteFunc
    Author(s)        :  Anish kumar
    Date Created     :  19.12.05
***********************************************************************************/
BOOL CExecuteFunc::bEnableDisableKeyHandlers(BOOL bEnable)
{
    if( bEnable == FALSE)
    {
        CWaitCursor omWait;
        vDestroyUtilityThreads(500, BIT_KEY_HANDLER_THREAD);
    }

    m_sNodeInfo.m_bKeyHandlersEnabled=bEnable;
    CSimSysNodeInfo* pNodeInfo = CSimSysManager::ouGetSimSysManager(m_eBus).pomGetSimSysNodeInfo();
    PSNODEINFO psNodeInfo = NULL;

    if( pNodeInfo != NULL )
    {
        psNodeInfo = pNodeInfo->psGetSimSysNodePointer(STR_EMPTY
                     , m_sNodeInfo.m_omStrNodeName);

        if(psNodeInfo != NULL)
        {
            psNodeInfo->m_bKeyHandlersEnabled= bEnable;
        }
    }

    return true;
}

/**************************************************************************************
    Function Name    :  bEnableDisableErrorHandlers
    Input(s)         :  BOOL
    Output           :  BOOL
    Functionality    :  This function enable/disable all error handlers according
                        to the input.While disabling handler it will stop the
                        threads executing them.
    Member of        :  CExecuteFunc
    Author(s)        :  Anish kumar
    Date Created     :  19.12.05
**************************************************************************************/
BOOL CExecuteFunc::bEnableDisableErrorHandlers(BOOL bEnable)
{
    if( bEnable == FALSE)
    {
        CWaitCursor omWait;
        // Destroy any handler if is running
        m_bStopErrorHandlers = TRUE;
        vDestroyUtilityThreads(500,BIT_ERROR_HANDLER_THREAD);
    }

    m_bStopErrorHandlers=FALSE;
    m_sNodeInfo.m_bErrorHandlersEnabled=bEnable;
    //for UI part
    CSimSysNodeInfo* pNodeInfo = CSimSysManager::ouGetSimSysManager(m_eBus).pomGetSimSysNodeInfo();
    PSNODEINFO psNodeInfo = NULL;

    if( pNodeInfo != NULL )
    {
        psNodeInfo = pNodeInfo->psGetSimSysNodePointer(STR_EMPTY
                     , m_sNodeInfo.m_omStrNodeName);

        if(psNodeInfo != NULL)
        {
            psNodeInfo->m_bErrorHandlersEnabled= bEnable;
        }
    }

    return true;
}

/***************************************************************************************
    Function Name    :  bEnableDisableMsgHandlers
    Input(s)         :  BOOL
    Output           :  BOOL
    Functionality    :  This function enable/disable all message handlers according
                        to the input.While disabling handler it will stop the
                        threads executing them.
    Member of        :  CExecuteFunc
    Author(s)        :  Anish kumar
    Date Created     :  19.12.05
****************************************************************************************/
BOOL CExecuteFunc::bEnableDisableMsgHandlers(BOOL bEnable)
{
    m_bStopMsgHandlers = !bEnable;
    m_sNodeInfo.m_bMsgHandlersEnabled=bEnable;
    CSimSysNodeInfo* pNodeInfo =
        CSimSysManager::ouGetSimSysManager(m_eBus).pomGetSimSysNodeInfo();
    PSNODEINFO psNodeInfo = NULL;

    if( pNodeInfo != NULL )
    {
        psNodeInfo = pNodeInfo->psGetSimSysNodePointer(STR_EMPTY
                     , m_sNodeInfo.m_omStrNodeName);

        if(psNodeInfo != NULL)
        {
            psNodeInfo->m_bMsgHandlersEnabled= bEnable;
        }
    }

    if((TRUE == bEnable) && (m_pomMsgHandlerThrd == NULL))
    {
        m_pomMsgHandlerThrd = AfxBeginThread(unReadNodeMsgHandlerBuffer,this);
    }

    return true;
}



/****************************************************************************************
    Function Name    :  vSetNodeInfo
    Input(s)         :  pointer to SNODEINFO structure
    Output           :  void
    Functionality    :  It takes the SNODEINFO from UI part and copy it
                        in local structure
    Member of        :  CExecuteFunc
    Author(s)        :  Anish kumar
    Date Created     :  16.12.05
****************************************************************************************/

void CExecuteFunc::vSetNodeInfo(PSNODEINFO psNodeInfo)
{
    m_sNodeInfo.m_omStrNodeName         = psNodeInfo->m_omStrNodeName;
    m_sNodeInfo.m_omStrFileName         = psNodeInfo->m_omStrFileName;
    m_sNodeInfo.m_omStrDllName          = psNodeInfo->m_omStrDllName;
    m_sNodeInfo.m_bDllHandlersEnabled   = psNodeInfo->m_bDllHandlersEnabled;
    m_sNodeInfo.m_bTimerHandlersEnabled = psNodeInfo->m_bTimerHandlersEnabled;
    m_sNodeInfo.m_bKeyHandlersEnabled   = psNodeInfo->m_bKeyHandlersEnabled;
    m_sNodeInfo.m_bErrorHandlersEnabled = psNodeInfo->m_bErrorHandlersEnabled;
    m_sNodeInfo.m_bMsgHandlersEnabled   = psNodeInfo->m_bMsgHandlersEnabled;
    m_sNodeInfo.m_dwClientId            = psNodeInfo->m_dwClientId;
    m_sNodeInfo.m_byPrefAddress         = psNodeInfo->m_byPrefAddress;
    m_sNodeInfo.m_eBus                  = psNodeInfo->m_eBus;
    m_sNodeInfo.m_unEcuName             = psNodeInfo->m_unEcuName;
}
void CExecuteFunc::vGetNodeInfo(sNODEINFO& sNodeInfo) const
{
    sNodeInfo.m_omStrNodeName         = m_sNodeInfo.m_omStrNodeName;
    sNodeInfo.m_omStrFileName         = m_sNodeInfo.m_omStrFileName;
    sNodeInfo.m_omStrDllName          = m_sNodeInfo.m_omStrDllName;
    sNodeInfo.m_bDllHandlersEnabled   = m_sNodeInfo.m_bDllHandlersEnabled;
    sNodeInfo.m_bTimerHandlersEnabled = m_sNodeInfo.m_bTimerHandlersEnabled;
    sNodeInfo.m_bKeyHandlersEnabled   = m_sNodeInfo.m_bKeyHandlersEnabled;
    sNodeInfo.m_bErrorHandlersEnabled = m_sNodeInfo.m_bErrorHandlersEnabled;
    sNodeInfo.m_bMsgHandlersEnabled   = m_sNodeInfo.m_bMsgHandlersEnabled;
    sNodeInfo.m_dwClientId            = m_sNodeInfo.m_dwClientId;
    sNodeInfo.m_byPrefAddress         = m_sNodeInfo.m_byPrefAddress;
    sNodeInfo.m_eBus                  = m_sNodeInfo.m_eBus;
    sNodeInfo.m_unEcuName             = m_sNodeInfo.m_unEcuName;
}

/****************************************************************************************
    Function Name    :  nGetFlagStatus
    Input(s)         :  enum/ which flag's status
    Output           :  int /flaf's status
    Functionality    :  Returns the flag's status
    Member of        :  CExecuteFunc
    Author(s)        :  Anish kumar
    Date Created     :  16.12.05
****************************************************************************************/

BOOL CExecuteFunc::bGetFlagStatus(eNODEFLAG eWhichFlag)
{
    BOOL bRetValue = FALSE ;

    switch( eWhichFlag )
    {
        case EXKEY_HANDLER:
            bRetValue = m_sNodeInfo.m_bKeyHandlersEnabled;
            break;

        case EXERROR_HANDLER :
            bRetValue = m_sNodeInfo.m_bErrorHandlersEnabled;
            break;

        case EXMSG_HANDLER :
            bRetValue = m_sNodeInfo.m_bMsgHandlersEnabled;
            break;

        case EXDLL_HANDLER :
            bRetValue = m_sNodeInfo.m_bDllHandlersEnabled;
            break;

        case ACTIVENODE :
            bRetValue = m_bMsgTxOnFlag;
            break;

        default:
            bRetValue = FALSE;
    }

    return bRetValue;
}



/****************************************************************************************
    Function Name    :  vDestroyUtilityThreads
    Input(s)         :  maximum time to wait for the threads to be destroyed
                        BYTE to denote the  required thread
    Output           :  void
    Functionality    :  This function wait for the thread to exit else terminate it
    Member of        :  CExecuteFunc
    Author(s)        :  Anish kumar
    Date Created     :  16.12.05
****************************************************************************************/

VOID CExecuteFunc::vDestroyUtilityThreads(UINT unMaxWaitTime, BYTE byThreadCode)
{
    DWORD dwResult = WAIT_ABANDONED;
    BYTE bySelectThread = 0;
    bySelectThread  =
        static_cast<UCHAR>( byThreadCode & BIT_MSG_HANDLER_THREAD );

    // if key handler thread exists
    if ( bySelectThread != 0 && m_pomMsgHandlerThrd != NULL &&
            (m_pomMsgHandlerThrd->m_hThread != NULL ))
    {
        // Set the flag to exit from thread.
        m_bStopMsgHandlers = TRUE;
        // Wait for thread to exit.
        dwResult = WaitForSingleObject(m_aomState[defMSG_HANDLER_THREAD],
                                       unMaxWaitTime);

        // If time out, come to begining of buffer there
        // will not be read or write operation,thread will be in
        //waiting state
        if( dwResult == WAIT_TIMEOUT )
        {
            m_unReadQMsgIndex=0;
            m_unWriteQMsgIndex=0;
            TerminateThread(m_pomMsgHandlerThrd->m_hThread,0);
            //delete m_pomMsgHandlerThrd ;
            m_pomMsgHandlerThrd = NULL;
        }
    }

    bySelectThread  =
        static_cast<UCHAR>( byThreadCode & BIT_KEY_HANDLER_THREAD );

    // if key handler thread exists
    if ( bySelectThread != 0 &&
            m_asUtilThread[defKEY_HANDLER_THREAD].m_hThread != NULL )
    {
        // Set the flag to exit from thread.
        m_bStopKeyHandlers = TRUE;
        // Wait for thread to exit.
        dwResult = WaitForSingleObject(m_aomState[defKEY_HANDLER_THREAD],
                                       unMaxWaitTime);

        // If time out, terminate the thread and delete the memory
        // if it is allocated inside the thread function and not
        // deleted
        if( dwResult == WAIT_TIMEOUT )
        {
            TerminateThread(m_asUtilThread[defKEY_HANDLER_THREAD].m_hThread,0);
            // Set the thread handle to NULL
            m_asUtilThread[defKEY_HANDLER_THREAD].m_hThread = NULL;

            if( m_asUtilThread[defKEY_HANDLER_THREAD].m_pvThread !=NULL )
            {
                delete []m_asUtilThread[defKEY_HANDLER_THREAD].m_pvThread;
                m_asUtilThread[defKEY_HANDLER_THREAD].m_pvThread = NULL;
            }
        }
    }

    bySelectThread  =
        static_cast<UCHAR>( byThreadCode & BIT_DLL_UNLOAD_HANDLER_THREAD );

    // if DLL handler thread exists
    if ( bySelectThread != 0 &&
            m_asUtilThread[defDLL_UNLOAD_HANDLER_THREAD].m_hThread != NULL )
    {
        // Wait for thread to exit.
        dwResult = WaitForSingleObject(m_aomState[defDLL_UNLOAD_HANDLER_THREAD],
                                       unMaxWaitTime);

        // If time out, terminate the thread and delete the memory
        // if it is allocated inside the thread function and not
        // deleted
        if( dwResult == WAIT_TIMEOUT )
        {
            TerminateThread(
                m_asUtilThread[defDLL_UNLOAD_HANDLER_THREAD].m_hThread, 0 );
            // Set the thread handle to NULL
            m_asUtilThread[defDLL_UNLOAD_HANDLER_THREAD].m_hThread = NULL;

            if( m_asUtilThread[defDLL_UNLOAD_HANDLER_THREAD].m_pvThread !=NULL )
            {
                delete []
                m_asUtilThread[defDLL_UNLOAD_HANDLER_THREAD].m_pvThread;
                m_asUtilThread[defDLL_UNLOAD_HANDLER_THREAD].m_pvThread = NULL;
            }
        }
    }

    bySelectThread  =
        static_cast<UCHAR>( byThreadCode & BIT_DLL_LOAD_HANDLER_THREAD );

    // if DLL handler thread exists
    if ( bySelectThread != 0 &&
            m_asUtilThread[defDLL_LOAD_HANDLER_THREAD].m_hThread != NULL )
    {
        // Wait for thread to exit.
        dwResult = WaitForSingleObject(m_aomState[defDLL_LOAD_HANDLER_THREAD],
                                       unMaxWaitTime);

        // If time out, terminate the thread and delete the memory
        // if it is allocated inside the thread function and not
        // deleted
        if( dwResult == WAIT_TIMEOUT )
        {
            TerminateThread(
                m_asUtilThread[defDLL_LOAD_HANDLER_THREAD].m_hThread,0 );
            // Set the thread handle to NULL
            m_asUtilThread[defDLL_LOAD_HANDLER_THREAD].m_hThread = NULL;

            if( m_asUtilThread[defDLL_LOAD_HANDLER_THREAD].m_pvThread !=NULL )
            {
                delete []m_asUtilThread[defDLL_LOAD_HANDLER_THREAD].m_pvThread;
                m_asUtilThread[defDLL_LOAD_HANDLER_THREAD].m_pvThread = NULL;
            }
        }
    }

    bySelectThread  =
        static_cast<UCHAR>( byThreadCode & BIT_ERROR_HANDLER_THREAD );

    // if Error handler thread exists
    if ( bySelectThread != 0 &&
            m_asUtilThread[defERROR_HANDLER_THREAD].m_hThread != NULL)
    {
        // Set the flag to exit from thread.
        m_bStopErrorHandlers = TRUE;
        // Wait for thread to exit.
        dwResult = WaitForSingleObject(m_aomState[defERROR_HANDLER_THREAD],
                                       unMaxWaitTime);

        // If time out, terminate the thread and delete the memory
        // if it is allocated inside the thread function and not
        // deleted
        if( dwResult == WAIT_TIMEOUT )
        {
            TerminateThread(
                m_asUtilThread[defERROR_HANDLER_THREAD].m_hThread, 0);
            // Set the thread handle to NULL
            m_asUtilThread[defERROR_HANDLER_THREAD].m_hThread = NULL;

            if( m_asUtilThread[defERROR_HANDLER_THREAD].m_pvThread !=NULL )
            {
                delete []m_asUtilThread[defERROR_HANDLER_THREAD].m_pvThread;
                m_asUtilThread[defERROR_HANDLER_THREAD].m_pvThread = NULL;
            }
        }

        m_aomState[defERROR_HANDLER_THREAD].SetEvent();
    }
}

/****************************************************************************************
    Function Name    :  bUnloadDll
    Input(s)         :  void
    Output           :  BOOL
    Functionality    :  It unload the loaded DLL of the object
    Member of        :  CExecuteFunc
    Author(s)        :  Anish kumar
    Date Created     :  16.12.05
    Modification By  :  Anish Kumar
    Modification on  :  29.01.07, Removed memory leak due to msg handler thread
                        termination
****************************************************************************************/
BOOL CExecuteFunc::bUnloadDll()
{
    BOOL bFreeLibrary        = TRUE;
    vExecuteOnDLLHandler(DLL_UNLOAD);
    vDestroyTimerThreads();
    // Set the flag to exit from thread.
    m_bDllLoaded = FALSE;
    m_omReadFromQEvent.SetEvent();
    vDestroyUtilityThreads(
        defWAIT_DELAY_FOR_DLL_UNLOAD,
        BIT_MSG_HANDLER_THREAD );
    m_sNodeInfo.m_bMsgHandlersEnabled=FALSE;

    if(   m_sNodeInfo.m_bKeyHandlersEnabled== TRUE
            || m_sNodeInfo.m_bErrorHandlersEnabled== TRUE )
    {
        BYTE byThread = 0x0;

        if (m_sNodeInfo.m_bKeyHandlersEnabled== TRUE )
        {
            byThread |= BIT_KEY_HANDLER_THREAD;
            m_bStopKeyHandlers   = TRUE;
        }

        if (m_sNodeInfo.m_bErrorHandlersEnabled==TRUE )
        {
            byThread |= BIT_ERROR_HANDLER_THREAD;
            m_bStopErrorHandlers = TRUE;
        }

        byThread |= BIT_DLL_LOAD_HANDLER_THREAD;
        vDestroyUtilityThreads(500,byThread);
        m_sNodeInfo.m_bKeyHandlersEnabled=FALSE;
        m_sNodeInfo.m_bErrorHandlersEnabled=FALSE;
    }

    //vExecuteOnDLLHandler(DLL_UNLOAD);
    //Sleep(0);
    vDestroyUtilityThreads(
        defWAIT_DELAY_FOR_DLL_UNLOAD,
        BIT_DLL_UNLOAD_HANDLER_THREAD );

    if(bFreeLibrary == TRUE)
    {
        bFreeLibrary =  FreeLibrary(m_hDllModule);
    }

    if(bFreeLibrary == TRUE)
    {
        m_hDllModule = NULL;
        // Reset DLLLoaded flag
        m_sNodeInfo.m_bDllHandlersEnabled=FALSE;
    }

    return bFreeLibrary;
}


/****************************************************************************************
    Function Name    :  omGetPrevFileName
    Input(s)         :  void
    Output           :  The previously loaded file name
    Functionality    :  It returns the previously loaded file name for unloading.
    Member of        :  CExecuteFunc
    Author(s)        :  Anish kumar
    Date Created     :  16.12.05
****************************************************************************************/

CString CExecuteFunc::omGetPrevFileName()
{
    return m_sNodeInfo.m_omStrFileName;
}


/****************************************************************************************
    Function Name    :  vSetDllHandle
    Input(s)         :  handle of loaded DLL
    Output           :
    Functionality    :  This function saves the DLL handle in member variable
    Member of        :  CExecuteFunc
    Author(s)        :  Anish kumar
    Date Created     :  16.12.05
****************************************************************************************/

void CExecuteFunc::vSetDllHandle(HMODULE hModuleHandle)
{
    m_hDllModule=hModuleHandle;
}


/****************************************************************************************
    Function Name    :  vWriteInQMsg
    Input(s)         :  sTCANDATA / message structure
    Output           :
    Functionality    :  This function write into message handler buffer associated to
                        each object.
    Member of        :  CExecuteFunc
    Author(s)        :  Anish kumar
    Date Created     :  16.12.05
****************************************************************************************/
void CExecuteFunc::vWriteInQMsg(STCAN_MSG sRxMsgInfo)
{
    if(m_bStopMsgHandlers==FALSE)
    {
        if(m_unQMsgCount<1000)
        {
            EnterCriticalSection(&m_CritSectForFuncBuf);
            m_asQMsg[m_unWriteQMsgIndex] = sRxMsgInfo;
            LeaveCriticalSection(&m_CritSectForFuncBuf);
            m_unQMsgCount++;
            //for circular buffer
            m_unWriteQMsgIndex=(m_unWriteQMsgIndex+1)%defMAX_FUNC_MSG;
            //set event for read thread
            m_omReadFromQEvent.SetEvent();
        }
        else
        {
            //COMMENTED BY AK********
            //::PostMessage(g_hMainGUI,WM_ERROR,ERROR_DLLMSG_BUFF_OVERFLOW,0);
        }
    }
}


/****************************************************************************************
    Function Name    :  sReadFromQMsg
    Input(s)         :
    Output           :  message structure
    Functionality    :  This function read fron message queue
    Member of        :  CExecuteFunc
    Author(s)        :  Anish kumar
    Date Created     :  16.12.05
****************************************************************************************/
STCAN_MSG CExecuteFunc::sReadFromQMsg()
{
    STCAN_MSG sRxMsgInfo;
    EnterCriticalSection(&m_CritSectForFuncBuf);
    sRxMsgInfo = m_asQMsg[m_unReadQMsgIndex];
    LeaveCriticalSection(&m_CritSectForFuncBuf);
    m_unQMsgCount--;
    m_unReadQMsgIndex=(m_unReadQMsgIndex+1)%defMAX_FUNC_MSG;
    return sRxMsgInfo;
}

/****************************************************************************************
    Function Name    :  unGetBufferMsgCnt
    Input(s)         :
    Output           :  no. of messages in message queue/buffer
    Functionality    :  Returns no. of messages in message queue/buffer
    Member of        :  CExecuteFunc
    Author(s)        :  Anish kumar
    Date Created     :  16.12.05
****************************************************************************************/
UINT CExecuteFunc::unGetBufferMsgCnt()
{
    return m_unQMsgCount;
}

void CExecuteFunc::vInitialiseInterfaceFnPtrs(HMODULE hLib)
{
    switch (m_eBus)
    {
        case CAN:
        {
            vInitialiseInterfaceFnPtrsCAN(hLib);
        }
        break;

        case J1939:
        {
            vInitialiseInterfaceFnPtrsJ1939(hLib);
        }
        break;
    }
}

/******************************************************************************/
/*  Function Name    :  vInitialiseInterfaceFnPtrsJ1939                       */
/*  Input(s)         :                                                        */
/*  Output           :                                                        */
/*  Functionality    :  This function will be called during dll load to init  */
/*                      Interface functions                                   */
/*                                                                            */
/*  Member of        :  CExecuteManager                                       */
/*  Friend of        :      -                                                 */
/*  Author(s)        :  Amitesh Bhart                                         */
/*  Date Created     :  22.07.2004                                            */
/*  Modification by  :  Anish kumar
    Modification on  :  19.12.05,Message enable/disable API initialization
                        is added(DllMsgOnOff),moved from CmainFrame,
/******************************************************************************/
void CExecuteFunc::vInitialiseInterfaceFnPtrsJ1939(HMODULE hModuleHandle)
{
    // For sending message, the DLL should call nWriteToCAN
    // function defined in the application. This is possible
    // only when the DLL has address of nWriteToCAN. The DLL
    // exports a function which the application must call to
    // pass address of nWriteToCAN. This same concept is used
    // all interface functions.
    DLLSENDPROC_J DllSendProc = (DLLSENDPROC_J) GetProcAddress(
                                    hModuleHandle, (char*) NAME_FUNC_SENGMSG_J);

    if (DllSendProc != NULL)
    {
        (*DllSendProc)((APPSENDPROC_J)g_unSendMsg_J1939);
    }

    DLLONLINEOFFLINE_J DllOnlineProc = (DLLONLINEOFFLINE_J)(GetProcAddress(
                                           hModuleHandle, (char*) NAME_FUNC_ONLINE_J));

    if (DllOnlineProc != NULL)
    {
        (*DllOnlineProc)((APPONLINEOFFLINE_J)g_unGoOnline_J1939);
    }

    DLLONLINEOFFLINE_J DllOfflineProc = (DLLONLINEOFFLINE_J)(GetProcAddress(
                                            hModuleHandle, (char*) NAME_FUNC_OFFLINE_J));

    if (DllOfflineProc != NULL)
    {
        (*DllOfflineProc)((APPONLINEOFFLINE_J)g_unGoOnline_J1939);
    }

    DLLREQUESTPGN_J DllRequestPGNProc = (DLLREQUESTPGN_J)(GetProcAddress(
                                            hModuleHandle, (char*) NAME_FUNC_REQUESTPGN_J));

    if (DllRequestPGNProc != NULL)
    {
        (*DllRequestPGNProc)((APPREQUESTPGN_J)g_unRequestPGN_J1939);
    }

    DLLACKMSG_J DllAckMsgProc = (DLLACKMSG_J)(GetProcAddress(
                                    hModuleHandle, (char*) NAME_FUNC_ACKMSG_J));

    if (DllAckMsgProc != NULL)
    {
        (*DllAckMsgProc)((APPACKMSG_J)g_unSendAckMsg_J1939);
    }

    DLLCLAIMADDRESS_J DllClaimAdresProc = (DLLCLAIMADDRESS_J)(GetProcAddress(
            hModuleHandle, (char*) NAME_FUNC_CLAIMADDRESS_J));

    if (DllClaimAdresProc != NULL)
    {
        (*DllClaimAdresProc)((APPCLAIMADDRESS_J)g_unClaimAddress_J1939);
    }

    DLLRQSTADDRESS_J DllRqstAdresProc = (DLLRQSTADDRESS_J)(GetProcAddress(
                                            hModuleHandle, (char*) NAME_FUNC_RQSTADDRESS_J));

    if (DllRqstAdresProc != NULL)
    {
        (*DllRqstAdresProc)((APPRQSTADDRESS_J)g_unRequestAddress_J1939);
    }

    DLLCMDADDRESS_J DllCmdAdresProc = (DLLCMDADDRESS_J)(GetProcAddress(
                                          hModuleHandle, (char*) NAME_FUNC_CMDADDRESS_J));

    if (DllCmdAdresProc != NULL)
    {
        (*DllCmdAdresProc)((APPCMDADDRESS_J)g_unCommandAddress_J1939);
    }

    DLLSETTIMEOUT_J DllSetTimeoutProc = (DLLSETTIMEOUT_J)(GetProcAddress(
                                            hModuleHandle, (char*) NAME_FUNC_TIMEOUT_J));

    if (DllSetTimeoutProc != NULL)
    {
        (*DllSetTimeoutProc)((APPSETTIMEOUT_J)g_unSetTimeout_J1939);
    }

    DLLLOGPROC DllLogEnableProc = (DLLLOGPROC)
                                  GetProcAddress(hModuleHandle,
                                          (char*) NAME_FUNC_LOG_ENABLE);

    if (DllLogEnableProc != NULL)
    {
        (*DllLogEnableProc)((LOGENABLE)gbEnableDisableLog);
    }

    DLLLOGPROC DllLogDisableProc = (DLLLOGPROC)
                                   GetProcAddress(hModuleHandle,
                                           (char*) NAME_FUNC_LOG_DISABLE);

    if (DllLogDisableProc != NULL)
    {
        (*DllLogDisableProc)((LOGENABLE)gbEnableDisableLog);
    }

    DLLLOGFILEPROC DllLogFileProc = (DLLLOGFILEPROC)
                                    GetProcAddress(hModuleHandle,
                                            (char*) NAME_FUNC_LOG_FILE);

    if (DllLogFileProc != NULL)
    {
        (*DllLogFileProc)((WRITETOLOGFILE)gbWriteToLog);
    }

    DLLTRACEPROC DllTraceFileProc = (DLLTRACEPROC)
                                    GetProcAddress(hModuleHandle,
                                            (char*) NAME_FUNC_TRACE);

    if (DllTraceFileProc != NULL)
    {
        (*DllTraceFileProc)((WRITETOTRACE)gbSendStrToTrace);
    }

    DLLSTARTSTOPTIMERPROC DllStartProc =
        (DLLSTARTSTOPTIMERPROC) GetProcAddress(hModuleHandle,
                (char*) NAME_FUNC_START_TIMER);

    if (DllStartProc != NULL)
    {
        (*DllStartProc)(gbSetResetTimer) ;
    }

    DLLSTARTSTOPTIMERPROC DllStopProc =
        (DLLSTARTSTOPTIMERPROC) GetProcAddress(hModuleHandle,
                (char*) NAME_FUNC_STOP_TIMER);

    if (DllStopProc != NULL)
    {
        (*DllStopProc)(gbSetResetTimer) ;
    }

    DLLSETTIMERVAL pFDllSetTimerValProc =
        (DLLSETTIMERVAL) GetProcAddress(hModuleHandle,
                                        (char*) defNAME_FUNC_SET_TIMER_VAL);

    if (pFDllSetTimerValProc != NULL)
    {
        (*pFDllSetTimerValProc)(gbSetTimerVal) ;
    }

    DLLENABLEDISABLEALLHANDLERS pFDllSetAllHandlersProc =
        (DLLENABLEDISABLEALLHANDLERS) GetProcAddress(hModuleHandle,
                (char*) defNAME_FUNC_ALL_HANDLERS);

    if (pFDllSetAllHandlersProc != NULL)
    {
        (*pFDllSetAllHandlersProc)(gbActivateDeactivateHandlers) ;
    }

    DLLENABLEDISABLEMSGHANDLERS pFDllSetMsgHandlersProc =
        (DLLENABLEDISABLEMSGHANDLERS) GetProcAddress(hModuleHandle,
                (char*) defNAME_FUNC_MSG_HANDLERS);

    if (pFDllSetMsgHandlersProc != NULL)
    {
        (*pFDllSetMsgHandlersProc)(gbEnableDisableMsgHandlers) ;
    }

    DLLENABLEDISABLEKEYHANDLERS pFDllSetKeyHandlersProc =
        (DLLENABLEDISABLEKEYHANDLERS) GetProcAddress(hModuleHandle,
                (char*) defNAME_FUNC_KEY_HANDLERS);

    if (pFDllSetKeyHandlersProc != NULL)
    {
        (*pFDllSetKeyHandlersProc)(gbEnableDisableKeyHandlers) ;
    }

    DLLENABLEDISABLEERRORHANDLERS pFDllSetErrorHandlersProc =
        (DLLENABLEDISABLEERRORHANDLERS) GetProcAddress(hModuleHandle,
                (char*) defNAME_FUNC_ERROR_HANDLERS);

    if (pFDllSetErrorHandlersProc != NULL)
    {
        (*pFDllSetErrorHandlersProc)(gbEnableDisableErrorHandlers) ;
    }

    DLLMSGTXONOFF DllMsgOnOff = (DLLMSGTXONOFF) GetProcAddress(
                                    hModuleHandle, (char*) NAME_FUNC_MSGTX_ON_OFF);

    if (DllMsgOnOff != NULL)
    {
        (*DllMsgOnOff)(gbMsgTransmissionOnOff);
    }

    DLLGETNODEHANDLER DllGetNodeHandler = (DLLGETNODEHANDLER)GetProcAddress(
            hModuleHandle,(char*) NAME_FUNC_SETDLLHANDLE);

    if(DllGetNodeHandler != NULL)
    {
        (*DllGetNodeHandler)(ghGetNodeDllHandler);
    }
}

/******************************************************************************/
/*  Function Name    :  vInitialiseInterfaceFnPtrs                            */
/*  Input(s)         :                                                        */
/*  Output           :                                                        */
/*  Functionality    :  This function will be called during dll load to init  */
/*                      Interface functions                                   */
/*                                                                            */
/*  Member of        :  CExecuteManager                                       */
/*  Friend of        :      -                                                 */
/*  Author(s)        :  Amitesh Bharti                                        */
/*  Date Created     :  22.07.2004                                            */
/*  Modification by  :  Anish kumar
    Modification on  :  19.12.05,Message enable/disable API initialization
                        is added(DllMsgOnOff),moved from CmainFrame,
/******************************************************************************/
void CExecuteFunc::vInitialiseInterfaceFnPtrsCAN(HMODULE hModuleHandle)
{
    // For sending message, the DLL should call nWriteToCAN
    // function defined in the application. This is possible
    // only when the DLL has address of nWriteToCAN. The DLL
    // exports a function which the application must call to
    // pass address of nWriteToCAN. This same concept is used
    // all interface functions.
    DLLFUNCPROC DllFuncProc = (DLLFUNCPROC) GetProcAddress(
                                  hModuleHandle, (char*) NAME_FUNC_USERDLL);

    if (DllFuncProc != NULL)
    {
        (*DllFuncProc)((APPFUNCPROC)gunSendMsg_CAN);
    }

    DLLLOGPROC DllLogEnableProc = (DLLLOGPROC)
                                  GetProcAddress(hModuleHandle,
                                          (char*) NAME_FUNC_LOG_ENABLE);

    if (DllLogEnableProc != NULL)
    {
        (*DllLogEnableProc)((LOGENABLE)gbEnableDisableLog);
    }

    DLLLOGPROC DllLogDisableProc = (DLLLOGPROC)
                                   GetProcAddress(hModuleHandle,
                                           (char*) NAME_FUNC_LOG_DISABLE);

    if (DllLogDisableProc != NULL)
    {
        (*DllLogDisableProc)((LOGENABLE)gbEnableDisableLog);
    }

    DLLLOGFILEPROC DllLogFileProc = (DLLLOGFILEPROC)
                                    GetProcAddress(hModuleHandle,
                                            (char*) NAME_FUNC_LOG_FILE);

    if (DllLogFileProc != NULL)
    {
        (*DllLogFileProc)((WRITETOLOGFILE)gbWriteToLog);
    }

    DLLTRACEPROC DllTraceFileProc = (DLLTRACEPROC)
                                    GetProcAddress(hModuleHandle,
                                            (char*) NAME_FUNC_TRACE);

    if (DllTraceFileProc != NULL)
    {
        (*DllTraceFileProc)((WRITETOTRACE)gbSendStrToTrace);
    }

    DLLRESETCONTROLLERPROC DllResetControllerProc =
        (DLLRESETCONTROLLERPROC) GetProcAddress(hModuleHandle,
                (char*) NAME_FUNC_RESET_CONTROLLER);

    if (DllResetControllerProc != NULL)
    {
        (*DllResetControllerProc)((RESETCONTROLLER)gvResetController_CAN);
    }

    DLLCONTROLLERMODEPROC DllControllerModeProc =
        (DLLCONTROLLERMODEPROC) GetProcAddress(hModuleHandle,
                (char*) NAME_FUNC_CONTROLLER_MODE);

    if (DllControllerModeProc != NULL)
    {
        (*DllControllerModeProc)(gvSetControllerMode_CAN);
    }

    DLLONLINEOFFLINEPROC DllOnlineProc =
        (DLLONLINEOFFLINEPROC) GetProcAddress(hModuleHandle,
                (char*) NAME_FUNC_ONLINE);

    if (DllOnlineProc != NULL)
    {
        (*DllOnlineProc)(gbActivateDeactivateHandlers);
    }

    DLLONLINEOFFLINEPROC DllOfflineProc =
        (DLLONLINEOFFLINEPROC) GetProcAddress(hModuleHandle,
                (char*) NAME_FUNC_OFFLINE);

    if (DllOfflineProc != NULL)
    {
        (*DllOfflineProc)(gbActivateDeactivateHandlers);
    }

    DLLDISCONNECTPROC DllDisconnectProc = (DLLDISCONNECTPROC)
                                          GetProcAddress(hModuleHandle,
                                                  (char*) NAME_FUNC_DISCONNECT);

    if (DllDisconnectProc != NULL)
    {
        (*DllDisconnectProc)((DISCONNECT)gbStartStopHardware_CAN) ;
    }

    DLLDISCONNECTPROC DllConnectProc = (DLLDISCONNECTPROC)
                                       GetProcAddress(hModuleHandle,
                                               (char*) NAME_FUNC_CONNECT);

    if (DllConnectProc != NULL)
    {
        (*DllConnectProc)((DISCONNECT)gbStartStopHardware_CAN) ;
    }

    DLLSTARTSTOPTIMERPROC DllStartProc =
        (DLLSTARTSTOPTIMERPROC) GetProcAddress(hModuleHandle,
                (char*) NAME_FUNC_START_TIMER);

    if (DllStartProc != NULL)
    {
        (*DllStartProc)(gbSetResetTimer) ;
    }

    DLLSTARTSTOPTIMERPROC DllStopProc =
        (DLLSTARTSTOPTIMERPROC) GetProcAddress(hModuleHandle,
                (char*) NAME_FUNC_STOP_TIMER);

    if (DllStopProc != NULL)
    {
        (*DllStopProc)(gbSetResetTimer) ;
    }

    DLLSETTIMERVAL pFDllSetTimerValProc =
        (DLLSETTIMERVAL) GetProcAddress(hModuleHandle,
                                        (char*) defNAME_FUNC_SET_TIMER_VAL);

    if (pFDllSetTimerValProc != NULL)
    {
        (*pFDllSetTimerValProc)(gbSetTimerVal) ;
    }

    DLLENABLEDISABLEMSGHANDLERS pFDllSetMsgHandlersProc =
        (DLLENABLEDISABLEMSGHANDLERS) GetProcAddress(hModuleHandle,
                (char*) defNAME_FUNC_MSG_HANDLERS);

    if (pFDllSetMsgHandlersProc != NULL)
    {
        (*pFDllSetMsgHandlersProc)(gbEnableDisableMsgHandlers) ;
    }

    DLLENABLEDISABLEKEYHANDLERS pFDllSetKeyHandlersProc =
        (DLLENABLEDISABLEKEYHANDLERS) GetProcAddress(hModuleHandle,
                (char*) defNAME_FUNC_KEY_HANDLERS);

    if (pFDllSetKeyHandlersProc != NULL)
    {
        (*pFDllSetKeyHandlersProc)(gbEnableDisableKeyHandlers) ;
    }

    DLLENABLEDISABLEERRORHANDLERS pFDllSetErrorHandlersProc =
        (DLLENABLEDISABLEERRORHANDLERS) GetProcAddress(hModuleHandle,
                (char*) defNAME_FUNC_ERROR_HANDLERS);

    if (pFDllSetErrorHandlersProc != NULL)
    {
        (*pFDllSetErrorHandlersProc)(gbEnableDisableErrorHandlers) ;
    }

    DLLMSGTXONOFF DllMsgOnOff = (DLLMSGTXONOFF) GetProcAddress(
                                    hModuleHandle, (char*) NAME_FUNC_MSGTX_ON_OFF);

    if (DllMsgOnOff != NULL)
    {
        (*DllMsgOnOff)(gbMsgTransmissionOnOff);
    }

    DLLGETNODEHANDLER DllGetNodeHandler = (DLLGETNODEHANDLER)GetProcAddress(
            hModuleHandle,(char*) NAME_FUNC_SETDLLHANDLE);

    if(DllGetNodeHandler != NULL)
    {
        (*DllGetNodeHandler)(ghGetNodeDllHandler);
    }
}


/******************************************************************************
    Function Name    :  bIsDllLoaded
    Input(s)         :
    Output           :  m_bDllLoaded
    Functionality    :  used by read thread to know status of Dll
                        associated to the CExecuteFunc object
    Member of        :  CExecuteFunc
    Author(s)        :  Anish kumar
    Date Created     :  03.01.06

/*****************************************************************************/
BOOL CExecuteFunc::bIsDllLoaded()
{
    return m_bDllLoaded;
}

/******************************************************************************
    Function Name    :  bIsTimerThreadStarted
    Input(s)         :
    Output           :  m_bTimerThreadStarted
    Functionality    :  Returns the status of timer thread
    Member of        :  CExecuteFunc
    Author(s)        :  Anish kumar
    Date Created     :  03.01.06

/*****************************************************************************/
BOOL CExecuteFunc::bIsTimerThreadStarted()
{
    return m_bTimerThreadStarted;
}


/******************************************************************************
    Function Name    :  vStartTimerThreads
    Input(s)         :
    Output           :
    Functionality    :  It starts the timer threads of the node if timers are
                        not started
    Member of        :  CExecuteFunc
    Author(s)        :  Anish kumar
    Date Created     :  03.01.06

/*****************************************************************************/
void CExecuteFunc::vStartTimerThreads()
{
    PSTIMERHANDLERLIST psCurrTimer=m_psFirstTimerStrList;

    while(psCurrTimer!=NULL)
    {
        //If timer is not started then start it
        if(psCurrTimer->sTimerHandler.pomThreadHandle==NULL)
        {
            psCurrTimer->sTimerHandler.pomThreadHandle=
                AfxBeginThread(unTimerHandlerProc,&psCurrTimer->sTimerHandler );
        }

        psCurrTimer=psCurrTimer->psNextTimer;
    }

    m_bTimerThreadStarted=TRUE;
}


/******************************************************************************
    Function Name    :  vResetTimerStructure
    Input(s)         :
    Output           :
    Functionality    :  If there is changed in timer by user it copy that in
                        local timer structure
    Member of        :  CExecuteFunc
    Author(s)        :  Anish kumar
    Date Created     :  03.01.06

/*****************************************************************************/
const int SIZE_TIMER_STRUCT = sizeof(STIMERHANDLER);
void CExecuteFunc::vResetTimerStructure(PSTIMERHANDLERLIST psFirstTimerPointer)
{
    if(psFirstTimerPointer!=NULL)
    {
        PSTIMERHANDLERLIST psLocalPtrToUITimerList=psFirstTimerPointer;
        PSTIMERHANDLERLIST psCurrTimer=m_psFirstTimerStrList;

        while(psCurrTimer!=NULL)
        {
            //memcpy(&psCurrTimer->sTimerHandler,
            //  &psLocalPtrToUITimerList->sTimerHandler,SIZE_TIMER_STRUCT);
            psCurrTimer->sTimerHandler.bTimerType =
                psLocalPtrToUITimerList->sTimerHandler.bTimerType;
            psCurrTimer->sTimerHandler.bTimerSelected =
                psLocalPtrToUITimerList->sTimerHandler.bTimerSelected;
            psCurrTimer->sTimerHandler.unTimerVal =
                psLocalPtrToUITimerList->sTimerHandler.unTimerVal;
            psCurrTimer->sTimerHandler.unTimerID =
                psLocalPtrToUITimerList->sTimerHandler.unTimerID;
            psCurrTimer->sTimerHandler.unCurrTime =
                psLocalPtrToUITimerList->sTimerHandler.unCurrTime;
            psLocalPtrToUITimerList=psLocalPtrToUITimerList->psNextTimer;
            psCurrTimer=psCurrTimer->psNextTimer;
        }
    }
}


/******************************************************************************
    Function Name    :  vDestroyTimerThreads
    Input(s)         :
    Output           :
    Functionality    :  It wait for 500ms for all the timer threads to stop
                        else it terminate them
    Member of        :  CExecuteFunc
    Author(s)        :  Anish kumar
    Date Created     :  03.01.06
    Modification by  :  Anish kumar
    Date             :  17.11.06,Added code to set the event for each timer before
                        terminating it and added Sleep there.
    Modification By  :  Anish Kumar
    Modification on  :  29.01.07, Removed memory leak due to timer thread
                        termination
/*****************************************************************************/
void CExecuteFunc::vDestroyTimerThreads()
{
    if(m_psFirstTimerStrList!=NULL)
    {
        PSTIMERHANDLERLIST psCurrTimer,psTempTimer;
        psCurrTimer=m_psFirstTimerStrList;

        while(psCurrTimer!=NULL)
        {
            //make hDllHandle null so that timer thread can come out of main loop
            psCurrTimer->sTimerHandler.hDllHandle=NULL;
            psCurrTimer->sTimerHandler.pFTimerHandler=NULL;
            //For proper thread exit while unloading
            psCurrTimer->sTimerHandler.omWaitEvent.SetEvent();
            psCurrTimer=psCurrTimer->psNextTimer;
        }

        Sleep(200);
        psCurrTimer=m_psFirstTimerStrList;

        while(psCurrTimer!=NULL)
        {
            if(psCurrTimer->sTimerHandler.pomThreadHandle!=NULL)
            {
                TerminateThread(psCurrTimer->
                                sTimerHandler.pomThreadHandle->m_hThread,0);
                delete psCurrTimer->sTimerHandler.pomThreadHandle;
            }

            psTempTimer=psCurrTimer;
            psCurrTimer=psCurrTimer->psNextTimer;
            m_psFirstTimerStrList=psCurrTimer;
            delete psTempTimer;
            psTempTimer=NULL;
        }

        m_psFirstTimerStrList=NULL;
        m_psLastTimerStrList=NULL;
    }
}


/******************************************************************************
    Function Name    :  bResetTimerStructure
    Input(s)         :  Timer name,timer type,BOOL start/stop
    Output           :  BOOL
    Functionality    :  It modify the timer structure as given by user from DLL
                        through DLL API call.
    Member of        :  CExecuteFunc
    Author(s)        :  Anish kumar
    Date Created     :  03.01.06

/*****************************************************************************/
BOOL CExecuteFunc::bResetTimer(char* pcTimerFunctionName, int type, BOOL bStart)
{
    BOOL bReturn=FALSE;
    CString omTimerName(pcTimerFunctionName) ;
    PSTIMERHANDLERLIST psCurrTimer=m_psFirstTimerStrList;

    while(psCurrTimer!=NULL&&!bReturn)
    {
        if(psCurrTimer->sTimerHandler.omStrTimerHandlerName==omTimerName)
        {
            psCurrTimer->sTimerHandler.bTimerType=type;
            psCurrTimer->sTimerHandler.bTimerSelected=bStart;
            //if timer type changed start from zero
            psCurrTimer->sTimerHandler.unCurrTime=0;
            bReturn= TRUE;
        }

        psCurrTimer=psCurrTimer->psNextTimer;
    }

    return bReturn;
}


/******************************************************************************
    Function Name    :  bResetTimerVal
    Input(s)         :  Timer name,Timer value
    Output           :
    Functionality    :  It change the timer value as given by user from DLL API
    Member of        :  CExecuteFunc
    Author(s)        :  Anish kumar
    Date Created     :  03.01.06

/*****************************************************************************/
BOOL CExecuteFunc::bResetTimerVal(char* pcTimerFunctionName,UINT unTimeVal)
{
    BOOL bReturn=FALSE;
    CString omTimerName(pcTimerFunctionName) ;
    PSTIMERHANDLERLIST psCurrTimer=m_psFirstTimerStrList;

    while(psCurrTimer!=NULL&&!bReturn)
    {
        if(psCurrTimer->sTimerHandler.omStrTimerHandlerName==omTimerName)
        {
            psCurrTimer->sTimerHandler.unTimerVal=unTimeVal;

            //if timer is not running start it
            if(psCurrTimer->sTimerHandler.bTimerSelected!=TRUE)
            {
                psCurrTimer->sTimerHandler.bTimerSelected=TRUE;
            }

            //if timer val changed start from zero
            psCurrTimer->sTimerHandler.unCurrTime=0;
            bReturn=TRUE;
        }

        psCurrTimer=psCurrTimer->psNextTimer;
    }

    return bReturn;
}

/******************************************************************************
    Function Name    :  bGetMsgTxFlag
    Input(s)         :
    Output           :  BOOL ,m_bMsgTxOnFlag
    Functionality    :  Give the m_bMsgTxOnFlag status
    Member of        :  CExecuteFunc
    Author(s)        :  Anish kumar
    Date Created     :  03.01.06

/*****************************************************************************/
BOOL CExecuteFunc::bGetMsgTxFlag()
{
    return m_bMsgTxOnFlag;
}

/******************************************************************************
    Function Name    :  vSetMsgTxFlag
    Input(s)         :  BOOL
    Output           :
    Functionality    :  Set the value of m_bMsgTxOnFlag
    Member of        :  CExecuteFunc
    Author(s)        :  Anish kumar
    Date Created     :  03.01.06

/*****************************************************************************/
void CExecuteFunc::vSetMsgTxFlag(BOOL bFlag)
{
    m_bMsgTxOnFlag=bFlag;
}

/******************************************************************************
    Function Name    :  psGetTimerListPtr
    Input(s)         :  BOOL
    Output           :  PSTIMERHANDLERLIST
    Functionality    :  Get the pointer to timer list
    Member of        :  CExecuteFunc
    Author(s)        :  Anish kumar
    Date Created     :  13.01.06

/*****************************************************************************/

const PSTIMERHANDLERLIST CExecuteFunc::psGetTimerListPtr()
{
    return m_psFirstTimerStrList;
}

/******************************************************************************
    Function Name    :  vCopyHandlersArrayToUI
    Input(s)         :  psNodeInfo
    Output           :  psNodeInfo
    Functionality    :  Get the handlers info in UI nodeinfo structure
    Member of        :  CExecuteFunc
    Author(s)        :  Anish kumar
    Date Created     :  20.01.06

/*****************************************************************************/
void CExecuteFunc::vCopyHandlersArrayToUI(sNODEINFO* psNodeInfo)
{
    if(psNodeInfo!=NULL)
    {
        psNodeInfo->m_omStrArrayDLLHandlers.Copy(m_omStrArrayDLLHandlers);
        psNodeInfo->m_omStrArrayErrorHandlers.Copy(m_omStrArrayErrorHandlers);
        psNodeInfo->m_omStrArrayKeyHandlers.Copy(m_omStrArrayKeyHandlers);
        psNodeInfo->m_omStrArrayEventHandlers.Copy(m_omStrArrayEventHandlers);
        psNodeInfo->m_omStrArrayMsgHandlers.Copy(m_omStrArrayMsgHandlers);
        psNodeInfo->m_omStrArrayMsgIDandName.Copy(m_omStrArrayMsgIDandName);
        psNodeInfo->m_omStrArrayMsgRange.Copy(m_omStrArrayMsgRange);
        psNodeInfo->m_omStrArrayTimerHandlers.Copy(m_omStrArrayTimerHandlers);
    }
}