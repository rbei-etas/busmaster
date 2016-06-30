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
 * \file      SimSysNodeInfo.cpp
 * \brief     This file contain definition of all function of
 * \author    Ratnadip Choudhury
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * This file contain definition of all function of
 */

#include "NodeSimEx_stdafx.h" // Standard header include file
#include "Error.h"
//#include "Include/CAN_Error_Defs.h"
#include "HashDefines.h"
#include "SimSysNodeInfo.h"   // Class defintion file
#include "ExecuteManager.h"
#include "SimSysManager.h"
#include "Include/BaseDefs.h"
//#include "GlobalObj.h"
//#include "DIL_Interface_extern.h"
#include "BaseDIL_CAN.h"
#include "ConvertCpp.h"
//#include "DataTypes/BaseAppServices.h"
#include "AppServices.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define DefDUPNODEINSYS    "The node in the simulated system you are trying to add is already present!!"
#define DefDUPCFILEINSYS   "The C file in the simulated system you are trying to add is already present!!"
#define DefDUPDLLINSYS     "The dll in the simulated system you are trying to add is already present!!"
const USHORT MCNET_DATA_MAX = 1000;

BOOL bIsTransitionInState( UINT unChannel, BYTE byRxError, BYTE byTxError, ERROR_STATE* eErrState)
{
    BOOL bIsTransition = FALSE;
    // Based on the value of transmit and receive error counter decide
    // the current error state

    if ((byTxError <= 127) && (byRxError <= 127))
    {
        // Error Active Mode
        if (eErrState[ unChannel ] != ERROR_ACTIVE)
        {
            bIsTransition = TRUE;
            eErrState[ unChannel ] = ERROR_ACTIVE;
        }
    }
    else if (byTxError == 255)
        // The sudden shift to the third state is to avoid
        // "else if ((byTxError > 127) || (byRxError > 127))"
    {
        // Bus off
        if (eErrState[ unChannel ] != ERROR_BUS_OFF)
        {
            bIsTransition = TRUE;
            eErrState[ unChannel ] = ERROR_BUS_OFF;
        }
    }
    else
    {
        // Error passive
        if (eErrState[ unChannel ] != ERROR_PASSIVE)
        {
            bIsTransition = TRUE;
            eErrState[ unChannel ] = ERROR_PASSIVE;
        }
    }

    return bIsTransition;
}

void vProcessCurrErrorEntryLin(  const SERROR_INFO_LIN& EventInfo, DWORD dwClientId )
{
    // Get the channel number
    CHAR nChannel = EventInfo.m_ucChannel - 1;
    if( nChannel < 0 || nChannel >= defNO_OF_CHANNELS )
    {
        ASSERT( FALSE );
        // Take prevension
        nChannel = 0;
    }

    CExecuteManager::ouGetExecuteManager(LIN, &CGlobalObj::ouGetObj(LIN)).vManageOnErrorHandlerLIN(EventInfo, dwClientId);
}

void vProcessCurrErrorEntry(const SERROR_INFO& sErrInfo, DWORD dwClientId)
{
    // Get the Error code
    USHORT usErrorID = ERROR_UNKNOWN;
    // Get the channel number
    CHAR nChannel = sErrInfo.m_ucChannel - 1;
    if( nChannel < 0 || nChannel >= defNO_OF_CHANNELS )
    {
        ASSERT( FALSE );
        // Take prevension
        nChannel = 0;
    }
    SCAN_ERR sCanErr;
    sCanErr.m_ucRxError = sErrInfo.m_ucRxErrCount;
    sCanErr.m_ucTxError = sErrInfo.m_ucTxErrCount;
    sCanErr.m_ucChannel = nChannel + 1; //sCanErr.m_ucChannel is 1 based channel number

    ERROR_STATE eErrorState = ERROR_ACTIVE;
    // Decide which module(s) to notify by analysing the error code
    // Accordingly notify the modules by sending/posting message
    if (sErrInfo.m_ucErrType == ERROR_WARNING_LIMIT_REACHED)
    {
        // Reaching warning limit isn't considered as an error.
        // In case of this interrupt there is no need for display to
        // be updated.
        // Use Channel ID as High Byte of WPARAM
        usErrorID = sErrInfo.m_ucErrType;
        eErrorState = ERROR_WARNING_LIMIT;
        CExecuteManager::ouGetExecuteManager(CAN, &CGlobalObj::ouGetObj(CAN)).vManageOnErrorHandlerCAN(eErrorState, sCanErr, dwClientId);
    }
    else
    {
        ERROR_STATE m_eCurrErrorState[ defNO_OF_CHANNELS ];
        // Handle interrupts. This is not error message and assign error code
        // to unknown
        // Interrupts are state transition indicators but not real error message
        if (sErrInfo.m_ucErrType == ERROR_BUS)
        {
            // Update Statistics information
            usErrorID = sErrInfo.m_ucReg_ErrCap /*& 0xE0*/;
            // Create Channel ID & Error code word
        }
        else if (sErrInfo.m_ucErrType == ERROR_INTERRUPT)
        {
            usErrorID = sErrInfo.m_ucErrType;
        }
        if (bIsTransitionInState(nChannel, sErrInfo.m_ucRxErrCount, sErrInfo.m_ucTxErrCount, &(m_eCurrErrorState[nChannel])))
        {
            if (usErrorID == STUFF_ERROR_RX)
            {
                eErrorState = ERROR_FRAME;
            }
            else
            {
                eErrorState = m_eCurrErrorState[nChannel];
            }
            CExecuteManager::ouGetExecuteManager(CAN, &CGlobalObj::ouGetObj(CAN)).vManageOnErrorHandlerCAN(eErrorState, sCanErr, dwClientId);
        }
    }
}
PSTCAN_TIME_MSG sCanTimeData=nullptr;
STCANDATA sCanData;//=nullptr;

PSTLIN_TIME_MSG sLinTimeData=nullptr;
STLINDATA sLinData;

int ReadNodeDataBuffer(PSNODEINFO psNodeInfo)
{
    ASSERT(psNodeInfo != nullptr);

    if(psNodeInfo != nullptr)
    {
        switch (psNodeInfo->m_eBus)
        {
            case CAN:
            {
                while (psNodeInfo->m_ouCanBufFSE->GetMsgCount() > 0)
                {
                    sCanData.m_ucDataType = RX_FLAG;

                    if(psNodeInfo != nullptr)
                    {
                        INT Result = psNodeInfo->m_ouCanBufFSE->ReadFromBuffer(&sCanData);
                        if (Result == ERR_READ_MEMORY_SHORT)
                        {
                            TRACE("ERR_READ_MEMORY_SHORT");
                        }
                        if (Result == EMPTY_APP_BUFFER)
                        {
                            TRACE("EMPTY_APP_BUFFER");
                        }
                        // Give the msg to NodeSimx for simulation
                        if (sCanData.m_ucDataType == RX_FLAG)
                        {
                            if ( sCanTimeData == nullptr )
                            {
                                sCanTimeData = new STCAN_TIME_MSG;
                            }
                            sCanTimeData->m_ucChannel    = sCanData.m_uDataInfo.m_sCANMsg.m_ucChannel;
                            memcpy(sCanTimeData->m_ucData, sCanData.m_uDataInfo.m_sCANMsg.m_ucData, sizeof(sCanTimeData->m_ucData));
                            sCanTimeData->m_bCANFD =  sCanData.m_uDataInfo.m_sCANMsg.m_bCANFD ;
                            //if (sCanData.m_bCANFDMsg)
                            //{
                            //    memcpy(sCanTimeData->m_aucCANFDData, sCanData.m_uDataInfo.m_sCANMsg.m_aucCANFDData,
                            //           /*sizeof(sCanTimeData->m_ucCANFDData)*/sCanData.m_uDataInfo.m_sCANMsg.m_ucDataLen);
                            //}
                            sCanTimeData->m_ucDataLen    = sCanData.m_uDataInfo.m_sCANMsg.m_ucDataLen;
                            sCanTimeData->m_ucEXTENDED   = sCanData.m_uDataInfo.m_sCANMsg.m_ucEXTENDED;
                            sCanTimeData->m_ucRTR        = sCanData.m_uDataInfo.m_sCANMsg.m_ucRTR;
                            sCanTimeData->m_ulTimeStamp  = (ULONG)sCanData.m_lTickCount.QuadPart;
                            sCanTimeData->m_unMsgID      = sCanData.m_uDataInfo.m_sCANMsg.m_unMsgID;
                            CExecuteManager::ouGetExecuteManager(psNodeInfo->m_eBus, &CGlobalObj::ouGetObj(CAN)).vManageOnMessageHandlerCAN_(sCanTimeData, psNodeInfo->m_dwClientId);
                        }
                        else if (sCanData.m_ucDataType == ERR_FLAG)
                        {
                            if (psNodeInfo->m_eBus == CAN)
                            {
                                vProcessCurrErrorEntry(sCanData.m_uDataInfo.m_sErrInfo, psNodeInfo->m_dwClientId);
                            }
                        }
                    }
                }
            }
            break;
            case J1939:
            {
                while (psNodeInfo->m_ouMsgBufVSE->GetMsgCount() > 0)
                {
                    static STJ1939_MSG sJ1939Msg;
                    static BYTE m_byTempData[MAX_MSG_LEN_J1939] = {0};
                    static int nType = 0;
                    int nSize = MAX_MSG_LEN_J1939;
                    INT Result = psNodeInfo->m_ouMsgBufVSE->ReadFromBuffer(nType, m_byTempData, nSize);
                    if (Result == ERR_READ_MEMORY_SHORT)
                    {
                        TRACE("ERR_READ_MEMORY_SHORT");
                        break;
                    }
                    if (Result == EMPTY_APP_BUFFER)
                    {
                        TRACE("EMPTY_APP_BUFFER");
                        break;
                    }
                    // Give the msg to NodeSimx for simulation
                    if (Result == CALL_SUCCESS)
                    {
                        sJ1939Msg.vSetDataStream(m_byTempData);
                        CExecuteFunc* pExecFunc = CExecuteManager::ouGetExecuteManager(psNodeInfo->m_eBus, &CGlobalObj::ouGetObj(J1939)).
                                                  pouGetExecuteFunc(psNodeInfo->m_dwClientId);
                        pExecFunc->vExecuteOnPGNHandler(&sJ1939Msg);

                    }
                }
            }
            break;
            case LIN:
            {
                while (psNodeInfo->m_ouLinBufSE->GetMsgCount() > 0)
                {
                    sLinData.m_ucDataType = RX_FLAG;

                    if(psNodeInfo != nullptr)
                    {
                        INT Result = psNodeInfo->m_ouLinBufSE->ReadFromBuffer(&sLinData);
                        if (Result == ERR_READ_MEMORY_SHORT)
                        {
                            TRACE("ERR_READ_MEMORY_SHORT");
                        }
                        if (Result == EMPTY_APP_BUFFER)
                        {
                            TRACE("EMPTY_APP_BUFFER");
                        }
                        if ( sLinData.m_eLinMsgType == LIN_MSG && sLinData.m_ucDataType == RX_FLAG )
                        {
                            // Give the msg to NodeSimx for simulation
                            if ( sLinTimeData == nullptr )
                            {
                                sLinTimeData = new STLIN_TIME_MSG;
                            }
                            sLinTimeData->m_ucChannel    = sLinData.m_uDataInfo.m_sLINMsg.m_ucChannel;
                            memcpy(sLinTimeData->m_ucData, sLinData.m_uDataInfo.m_sLINMsg.m_ucData, sizeof(sLinTimeData->m_ucData));

                            sLinTimeData->m_ucDataLen    = sLinData.m_uDataInfo.m_sLINMsg.m_ucDataLen;
                            sLinTimeData->m_ucChksum = sLinData.m_uDataInfo.m_sLINMsg.m_ucChksum;
                            sLinTimeData->m_ucChksumTyp = sLinData.m_uDataInfo.m_sLINMsg.m_ucChksumTyp;

                            if (LIN_SLAVE_RESPONSE == sLinData.m_uDataInfo.m_sLINMsg.m_ucMsgTyp)
                            {
                                sLinTimeData->m_ucMsgTyp = 1; // Response
                            }
                            else if (LIN_MASTER_RESPONSE == sLinData.m_uDataInfo.m_sLINMsg.m_ucMsgTyp)
                            {
                                sLinTimeData->m_ucMsgTyp = 0; // Header
                            }
                            sLinTimeData->m_ucMsgTyp = sLinData.m_uDataInfo.m_sLINMsg.m_ucMsgTyp;
                            sLinTimeData->m_ulTimeStamp  = (ULONG)sLinData.m_lTickCount.QuadPart;
                            sLinTimeData->m_ucMsgID      = sLinData.m_uDataInfo.m_sLINMsg.m_ucMsgID;
                            CExecuteManager::ouGetExecuteManager(psNodeInfo->m_eBus, &CGlobalObj::ouGetObj(LIN)).vManageOnMessageHandlerLIN(sLinTimeData, psNodeInfo->m_dwClientId);
                        }
                        else if (sLinData.m_eLinMsgType == LIN_EVENT)
                        {
                            if (psNodeInfo->m_eBus == LIN)
                            {
                                vProcessCurrErrorEntryLin( sLinData.m_uDataInfo.m_sErrInfo, psNodeInfo->m_dwClientId);
                            }
                        }
                    }
                }
            }
            break;
        }
    }
    if (nullptr != sCanTimeData)
    {
        //delete sCanTimeData;
        //sCanTimeData = nullptr;
    }
    return 0;
}

DWORD WINAPI NodeDataReadThreadProc(LPVOID pVoid)
{
    CPARAM_THREADPROC* pThreadParam = (CPARAM_THREADPROC*) pVoid;
    if (pThreadParam == nullptr)
    {
        return (DWORD)-1;
    }
    PSNODEINFO psNodeInfo = (PSNODEINFO)pThreadParam->m_pBuffer;
    if (psNodeInfo == nullptr)
    {
        return (DWORD)-1;
    }
    bool bLoopON = true;

    while (bLoopON)
    {
        WaitForSingleObject(pThreadParam->m_hActionEvent, INFINITE);
        switch (pThreadParam->m_unActionCode)
        {
            case INVOKE_FUNCTION:
            {
                __try
                {
                    ReadNodeDataBuffer(psNodeInfo); // Retrieve message from the driver
                }
                __except(EXCEPTION_EXECUTE_HANDLER)
                {
                    LPBYTE lpPage;
                    static SYSTEM_INFO si;
                    static MEMORY_BASIC_INFORMATION mi;
                    static DWORD dwOldProtect;

                    // Get page size of system
                    GetSystemInfo(&si);

                    // Find SP address
                    _asm mov lpPage, esp;

                    // Get allocation base of stack
                    VirtualQuery(lpPage, &mi, sizeof(mi));

                    // Go to page beyond current page
                    lpPage = (LPBYTE)(mi.BaseAddress)-si.dwPageSize;

                    // Free portion of stack just abandoned
                    if (!VirtualFree(mi.AllocationBase,
                                     (LPBYTE)lpPage - (LPBYTE)mi.AllocationBase,
                                     MEM_DECOMMIT))
                    {
                        // If we get here, exit
                        exit(1);
                    }

                    // Reintroduce the guard page
                    if (!VirtualProtect(lpPage, si.dwPageSize,
                                        PAGE_GUARD | PAGE_READWRITE,
                                        &dwOldProtect))
                    {
                        exit(1);
                    }
                    printf("Exception handler %lX\n", _exception_code());
                    //Sleep(2000);
                }
            }
            break;
            case EXIT_THREAD:
            {
                bLoopON = false;
            }
            break;
            case CREATE_TIME_MAP:
            {
                //Nothing at this moment
            }
            break;
            default:
            case INACTION:
            {
                // nothing right at this moment
            }
            break;
        }
    }
    SetEvent(pThreadParam->hGetExitNotifyEvent());
    return 0;
}
//SNODEINFO STARTS

sNODEINFO::sNODEINFO(ETYPE_BUS eBus, CGlobalObj* pGlobalObj)
{
    m_eBus = eBus;
    m_byPrefAddress = ADDRESS_NULL;
    m_unEcuName     = 0x0;
    m_dwClientId = 0;
    m_omStrNodeName     = "";
    m_omStrCFileName     = "";
    m_omStrDllName      = "";

    m_bIsNodeEnabled      = TRUE;

    m_ouCanBufFSE = nullptr;
    m_ouLinBufSE = nullptr;
    m_ouMsgBufVSE = nullptr;
    switch (m_eBus)
    {
        case CAN:
            m_ouCanBufFSE = new CMsgBufFSE<STCANDATA>();
            break;
        case LIN:
            m_ouLinBufSE = new CMsgBufFSE<STLINDATA>();
            break;
        case J1939:
            m_ouMsgBufVSE = new CMsgBufVSE();
            break;
        default:
            break;
    }

    m_omStrArrayTimerHandlers.RemoveAll();
    m_omStrArrayKeyHandlers.RemoveAll();
    m_omStrArrayErrorHandlers.RemoveAll();
    m_omStrArrayEventHandlers.RemoveAll();
    m_omStrArrayDLLHandlers.RemoveAll();
    m_omStrArrayMsgRange.RemoveAll();
    m_omStrArrayMsgIDandName.RemoveAll();
    m_omStrArrayMsgHandlers.RemoveAll();
    m_unChannel = 0;
    m_ouLastDBChecksum = 0;
    //Get Instance of class to monitor change in .cpp file.
    m_pouDirectoryWatcher = CDirectoryWatcher::GetInstance();

    m_pGlobalObj = pGlobalObj;
}

sNODEINFO::~sNODEINFO()
{
    if ( nullptr != m_ouLinBufSE )
    {
        delete m_ouLinBufSE;
        m_ouLinBufSE = nullptr;
    }
    if ( nullptr != m_ouMsgBufVSE )
    {
        delete m_ouMsgBufVSE;
        m_ouMsgBufVSE = nullptr;
    }
    if ( nullptr != m_ouCanBufFSE )
    {
        delete m_ouCanBufFSE;
        m_ouCanBufFSE = nullptr;
    }
    m_ThreadProc.bTerminateThread();
    RegisterFileToMonitor(std::string(m_omStrCFileName),false);
    CDirectoryWatcher::ReleaseInstance();
}

sNODEINFO& sNODEINFO::operator=(const sNODEINFO& sobj)
{
    if (this == &sobj )
    {
        return *this;
    }
    this->m_ouCanBufFSE = sobj.m_ouCanBufFSE;
    this->m_ouMsgBufVSE = sobj.m_ouMsgBufVSE;
    this->m_ouLinBufSE = sobj.m_ouLinBufSE;
    this->m_dwClientId = sobj.m_dwClientId;
    this->m_byPrefAddress = sobj.m_byPrefAddress;
    this->m_unEcuName = sobj.m_unEcuName;
    this->m_ThreadProc = sobj.m_ThreadProc;
    this->m_omStrNodeName = sobj.m_omStrNodeName;
    this->m_omStrCFileName = sobj.m_omStrCFileName;
    this->m_omStrDllName = sobj.m_omStrDllName;
    this->m_hModuleHandle = sobj.m_hModuleHandle;
    this->m_bIsNodeEnabled = sobj.m_bIsNodeEnabled;
    this->m_eNodeState = sobj.m_eNodeState;
    this->m_eNodeFileType = sobj.m_eNodeFileType;
    this->m_unChannel = sobj.m_unChannel;
    this->m_ouLastDBChecksum = sobj.m_ouLastDBChecksum;
    this->m_omStrArrayTimerHandlers.RemoveAll();
    this->m_omStrArrayTimerHandlers.Append( sobj.m_omStrArrayTimerHandlers);
    this->m_omStrArrayKeyHandlers.RemoveAll();
    this->m_omStrArrayKeyHandlers.Append(sobj.m_omStrArrayKeyHandlers);
    this->m_omStrArrayMsgHandlers.RemoveAll();
    this->m_omStrArrayMsgHandlers.Append(sobj.m_omStrArrayMsgHandlers);
    this->m_omStrArrayErrorHandlers.RemoveAll();
    this->m_omStrArrayErrorHandlers.Append(sobj.m_omStrArrayErrorHandlers);
    this->m_omStrArrayEventHandlers.RemoveAll();
    this->m_omStrArrayEventHandlers.Append(sobj.m_omStrArrayEventHandlers);
    this->m_omStrArrayDLLHandlers.RemoveAll();
    this->m_omStrArrayDLLHandlers.Append(sobj.m_omStrArrayDLLHandlers);
    this->m_omStrArrayMsgRange.RemoveAll();
    this->m_omStrArrayMsgRange.Append(sobj.m_omStrArrayMsgRange);
    this->m_omStrArrayMsgIDandName.RemoveAll();
    this->m_omStrArrayMsgIDandName.Append(sobj.m_omStrArrayMsgIDandName);
    this->m_eBus = sobj.m_eBus;
    return *this;
}
sNODEINFO::sNODEINFO(const sNODEINFO& sobj)
{
    *this = sobj;
}

BOOL sNODEINFO::bStartThreadProc()
{
    if (m_eBus == CAN)
    {
        m_ThreadProc.m_hActionEvent = m_ouCanBufFSE->hGetNotifyingEvent();
    }
    else if ( m_eBus == LIN )
    {
        m_ThreadProc.m_hActionEvent = m_ouLinBufSE->hGetNotifyingEvent();
    }
    else //For any other bus
    {
        m_ThreadProc.m_hActionEvent = m_ouMsgBufVSE->hGetNotifyingEvent();
    }
    m_ThreadProc.m_unActionCode = INVOKE_FUNCTION;
    m_ThreadProc.m_pBuffer = this;
    return m_ThreadProc.bStartThread(NodeDataReadThreadProc);
}
BOOL sNODEINFO::bTerminateThreadProc()
{
    return m_ThreadProc.bTerminateThread();
}

HRESULT sNODEINFO::RegisterFileToMonitor(std::string strFileName,bool bRegister)
{
    //Extract Directory
    HRESULT hResult = PathFileExists(strFileName.c_str());
    if(TRUE == hResult)
    {
        char acDirPath[MAX_PATH] = "";
        strcpy(acDirPath, strFileName.c_str());
        PathRemoveFileSpec(acDirPath);
        CString omstrDirPath = acDirPath;

        hResult == PathIsDirectory(omstrDirPath);
        if(TRUE == hResult)
        {
            if(true == bRegister)
            {
                hResult = m_pouDirectoryWatcher->AddDirectoryWatch(std::string(omstrDirPath),this);
            }
            else
            {
                hResult = m_pouDirectoryWatcher->RemoveDirectoryWatch(std::string(omstrDirPath),this);
            }
        }
    }
    return hResult;
}
void sNODEINFO::OnFileChanged(const std::string strFileName,DWORD dwAction)
{
    if(0 == strFileName.compare(m_omStrCFileName))
    {
        if(FILE_ACTION_MODIFIED == dwAction)
        {
            m_eNodeState = NODE_NOT_BUILT;
            CFunctionEditorDoc* pDoc = m_pGlobalObj->pGetDocPtrOfFile(strFileName.c_str());
            if(pDoc!=nullptr)
            {
                pDoc->SetNotifyReload(true);
            }
            CExecuteManager::ouGetExecuteManager(m_eBus, m_pGlobalObj).bSetNodeState(this, NODE_NOT_BUILT);
        }
    }
}

//SNODEINFO ENDS
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
/******************************************************************************
Function Name    :  CSimSysNodeInfo

  Input(s)         :
  Output           :
  Functionality    :  Constructor
  Member of        :  CSimSysNodeInfo
  Friend of        :      -

    Author(s)        :  Harika M
    Date Created     :  03.01.06
    Modifications    :
******************************************************************************/
CSimSysNodeInfo::CSimSysNodeInfo(ETYPE_BUS eBus, CGlobalObj* pGlobalObj)
{
    m_eBus          = eBus;
    m_psNodesList = nullptr;
    m_unNumberOfNodesAdded = 0;
    m_pGlobalObj = pGlobalObj;
}
/******************************************************************************
Function Name    :  ~CSimSysNodeInfo

  Input(s)         :
  Output           :
  Functionality    :  Destructor
  Member of        :  CSimSysNodeInfo
  Friend of        :      -

    Author(s)        :  Harika M
    Date Created     :  03.01.06
    Modifications    :
******************************************************************************/
CSimSysNodeInfo::~CSimSysNodeInfo()
{

}



/******************************************************************************
Function Name    :  bIsDuplicateDllName
Input(s)         :  CString omStrDllName
Output           :  BOOL
Functionality    :  Returns TRUE if the Dll is already associated with a node else FALSE
Member of        :  CSimSysNodeInfo
Friend of        :      -
Author(s)        :  Anish kr
Date Created     :
Modification     :  Robin G.K.
                    23.10.14, Removed unused sSIMSYSINFO structure.
******************************************************************************/
BOOL CSimSysNodeInfo::bIsDuplicateDllName( CString omStrDllName)
{
    BOOL bDllPresent = FALSE;
    if (!omStrDllName.IsEmpty())
    {
        PSNODELIST pTempNode = m_psNodesList;
        while((pTempNode != nullptr) && (!bDllPresent))
        {
            if( (pTempNode->m_sNodeInfo->m_omStrDllName.CompareNoCase(omStrDllName) == 0 ))
            {
                bDllPresent = TRUE;
            }
            pTempNode = pTempNode->m_psNextNode;
        }
    }
    return bDllPresent;
}



/******************************************************************************
Function Name    :  bIsDuplicateCFile
Input(s)         :  CString omStrCfile
Output           :  BOOL
Functionality    :  Returns TRUE if the .c or .cpp is already associated with
                    a node else FALSE
Member of        :  CSimSysNodeInfo
Friend of        :      -
Author(s)        :  Anish Kr
Date Created     :
Modification     :  Robin G.K.
                    23.10.14, Removed unused sSIMSYSINFO structure.
******************************************************************************/
BOOL CSimSysNodeInfo::bIsDuplicateCFile(CString omStrCfile)
{
    BOOL bCFilePresent = FALSE;
    if (!omStrCfile.IsEmpty())
    {
        PSNODELIST pTempNode = m_psNodesList;
        while((pTempNode != nullptr) && (!bCFilePresent))
        {
            if( (pTempNode->m_sNodeInfo->m_omStrCFileName.CompareNoCase(omStrCfile) == 0 ))
            {
                bCFilePresent = TRUE;
            }
            pTempNode = pTempNode->m_psNextNode;
        }
    }
    return bCFilePresent;
}
/******************************************************************************
Function Name    :  bIsDuplicateNode
Input(s)         :  CString omStrNodeName
Output           :  BOOL
Functionality    :  Returns TRUE if the node is already present in NodeList.
Member of        :  CSimSysNodeInfo
Friend of        :      -
Author(s)        :  Anish Kr
Date Created     :
Modification     :  Robin G.K.
                    23.10.14, Removed unused sSIMSYSINFO structure.
******************************************************************************/
BOOL CSimSysNodeInfo::bIsDuplicateNode( CString omStrNodeName)
{
    BOOL bNodePresent = FALSE;
    if (!omStrNodeName.IsEmpty())
    {
        PSNODELIST pTempNode = m_psNodesList;
        while((pTempNode != nullptr) && (!bNodePresent))
        {
            if( (pTempNode->m_sNodeInfo->m_omStrNodeName.CompareNoCase(omStrNodeName) == 0 ))
            {
                bNodePresent = TRUE;
            }
            pTempNode = pTempNode->m_psNextNode;
        }
    }
    return bNodePresent;
}

/******************************************************************************
Function Name    :  bIsDuplicateEcuName
Input(s)         :  UINT64 un64EcuName
Output           :  BOOL
Functionality    :  Returns TRUE if the ECU Name is already associated with a
                    node.
Member of        :  CSimSysNodeInfo
Friend of        :      -
Author(s)        :  Pradeep K
Date Created     :  18/03/2011
Modification     :  Robin G.K.
                    23.10.14, Removed unused sSIMSYSINFO structure.
******************************************************************************/
BOOL CSimSysNodeInfo::bIsDuplicateEcuName( UINT64 un64EcuName)
{
    BOOL bIsDuplicateECUName = FALSE;
    PSNODELIST pTempNode = m_psNodesList;
    while(pTempNode != nullptr)
    {
        if(pTempNode->m_sNodeInfo->m_unEcuName == un64EcuName &&
                !bIsDuplicateECUName)
        {
            bIsDuplicateECUName = TRUE;
            break;
        }
        pTempNode = pTempNode->m_psNextNode;
    }

    return bIsDuplicateECUName;
}
/******************************************************************************
Function Name    :  bIsDuplicateAddress
Input(s)         :  UINT64 un64EcuName
Output           :  BOOL
Functionality    :  Returns TRUE if Preferred Address is already associated with
                    a node.
Member of        :  CSimSysNodeInfo
Friend of        :      -
Author(s)        :  Pradeep K
Date Created     :  18/03/2011
Modification     :  Robin G.K.
                    23.10.14, Removed unused sSIMSYSINFO structure.
******************************************************************************/
BOOL CSimSysNodeInfo::bIsDuplicateAddress( BYTE byAddress)
{
    BOOL bIsDuplicateAddress = FALSE;
    PSNODELIST pTempNode = m_psNodesList;
    while(pTempNode != nullptr)
    {
        if(pTempNode->m_sNodeInfo->m_byPrefAddress == byAddress &&
                !bIsDuplicateAddress)
        {
            bIsDuplicateAddress = TRUE;
            break;
        }
        pTempNode = pTempNode->m_psNextNode;
    }

    return bIsDuplicateAddress;
}
/******************************************************************************
Function Name    :  bIsBuildAllNodesValid
Input(s)         :  -
Output           :  BOOL
Functionality    :  Returns TRUE if there is any .c or .cpp File associated
                    with a node to build them, else FALSE.

Member of        :  CSimSysNodeInfo
Friend of        :      -
Author(s)        :  Harika M
Date Created     :  17.1.2006
Modification     :  Robin G.K.
                    23.10.14, Removed unused sSIMSYSINFO structure.
******************************************************************************/
BOOL CSimSysNodeInfo::bIsBuildAllNodesValid()
{
    BOOL bIsBuildAllValid = FALSE;
    PSNODELIST pTempNode = m_psNodesList;
    while(pTempNode != nullptr)
    {
        if(!pTempNode->m_sNodeInfo->m_omStrCFileName.IsEmpty() &&
                pTempNode->m_sNodeInfo->m_bIsNodeEnabled == true &&
                !bIsBuildAllValid)
        {
            bIsBuildAllValid = TRUE;
            break;
        }
        pTempNode = pTempNode->m_psNextNode;
    }

    return bIsBuildAllValid;
}


/******************************************************************************
Function Name    :  bIsEnableAllNodesValid
Input(s)         :  -
Output           :  BOOL
Functionality    :  Returns TRUE is any node is Disabled.
Member of        :  CSimSysNodeInfo
Friend of        :      -
Author(s)        :  Robin G.K.
Date Created     :  23.10.14
Modifications    :
******************************************************************************/
BOOL CSimSysNodeInfo::bIsEnableAllNodesValid()
{
    BOOL bIsEnableAllValid = FALSE;
    PSNODELIST pTempNode = m_psNodesList;

    while(pTempNode != nullptr)
    {
        if(pTempNode->m_sNodeInfo->m_bIsNodeEnabled == FALSE &&
                !bIsEnableAllValid)// if any node is disabled.
        {
            bIsEnableAllValid = TRUE;
            break;
        }
        pTempNode = pTempNode->m_psNextNode;
    }

    return bIsEnableAllValid;
}


/******************************************************************************
Function Name    :  bIsDisableAllNodesValid
Input(s)         :  -
Output           :  BOOL
Functionality    :  Returns TRUE if any of the nodes is enabled.
Member of        :  CSimSysNodeInfo
Friend of        :      -
Author(s)        :  Robin G.K.
Date Created     :  23.10.14
Modifications    :
******************************************************************************/
BOOL CSimSysNodeInfo::bIsDisableAllNodesValid()
{
    BOOL bIsDisableAllValid = FALSE;
    PSNODELIST pTempNode = m_psNodesList;

    while(pTempNode != nullptr)
    {
        if(pTempNode->m_sNodeInfo->m_bIsNodeEnabled == TRUE &&
                !bIsDisableAllValid)// if any node is enabled.
        {
            bIsDisableAllValid = TRUE;
            break;
        }
        pTempNode = pTempNode->m_psNextNode;
    }

    return bIsDisableAllValid;
}



/******************************************************************************
Function Name    :  psGetSimSysNodePointer
Input(s)         :  CString omStrNodeName
Output           :  PSNODEINFO
Functionality    :  Returns node ptr for the given Node name.
if found, otherwise
nullptr.
Member of        :  CSimSysNodeInfo
Friend of        :      -
Author(s)        :  Robin G.K.
Date Created     :  23.10.14
Modifications    :
******************************************************************************/
PSNODEINFO CSimSysNodeInfo::psGetSimSysNodePointer(CString omStrNodeName)
{
    PSNODEINFO psNodeInfo = nullptr;
    if ( !omStrNodeName.IsEmpty() )
    {

        PSNODELIST pTempNode = m_psNodesList;
        while(pTempNode != nullptr)
        {
            if(pTempNode->m_sNodeInfo->m_omStrNodeName == omStrNodeName)
            {
                psNodeInfo =  (pTempNode->m_sNodeInfo);
            }
            pTempNode = pTempNode->m_psNextNode;
        }

    }
    return psNodeInfo;
}



/******************************************************************************
Function Name    :  bDeleteNodeFromSimSys
Input(s)         :  CString omStrNodeToBeDeleted
Output           :  BOOL
Functionality    :  Deletes node from the Node List, if input is ""
                    then deletes all the nodes from Node List.
Member of        :  CSimSysNodeInfo
Friend of        :      -
Author(s)        :  Anish Kr.
Date Created     :
Modification     :  Robin G.K.
                    23.10.14, Removed unused sSIMSYSINFO structure.
******************************************************************************/
BOOL CSimSysNodeInfo::bDeleteNodeFromSimSys(CString omStrNodeToBeDeleted)
{
    BOOL bReturnValue = FALSE;


    //If the simsys is found get the nodelist pointer
    PSNODELIST pTempNodeList = m_psNodesList;
    //For delete all nodes
    if (omStrNodeToBeDeleted== "")
    {
        PSNODELIST pTemp;
        while(pTempNodeList != nullptr )
        {
            pTemp = pTempNodeList;
            pTempNodeList = pTempNodeList->m_psNextNode;
            m_pGlobalObj->RegisterNodeToDIL(FALSE, (pTemp->m_sNodeInfo));
            delete pTemp;
        }
        bReturnValue = TRUE;

        //If all nodes are deleted initialize it
        m_psNodesList = nullptr;
        m_unNumberOfNodesAdded = 0;
    }
    else
    {
        PSNODELIST pCurr = pTempNodeList;
        PSNODELIST pPrev = pTempNodeList;
        if(pTempNodeList->m_sNodeInfo->m_omStrNodeName == omStrNodeToBeDeleted)
        {
            //If first node to be deleted
            pTempNodeList = pTempNodeList->m_psNextNode;
            m_pGlobalObj->RegisterNodeToDIL(FALSE, (pCurr->m_sNodeInfo));
            delete pCurr;
            //one node is deleted
            m_psNodesList = pTempNodeList;
            m_unNumberOfNodesAdded -= 1;
            bReturnValue = TRUE;
        }
        else
        {
            BOOL bFound = FALSE;
            while( pCurr != nullptr && bFound == FALSE)
            {
                if(pCurr->m_sNodeInfo->m_omStrNodeName == omStrNodeToBeDeleted)
                {
                    pPrev->m_psNextNode = pCurr->m_psNextNode;
                    m_pGlobalObj->RegisterNodeToDIL(FALSE, (pCurr->m_sNodeInfo));
                    delete pCurr;
                    m_unNumberOfNodesAdded -= 1;
                    bFound = TRUE;
                    bReturnValue = TRUE;
                }
                else
                {
                    pPrev = pCurr;
                    pCurr = pCurr->m_psNextNode;
                }
            }
        }
    }


    return bReturnValue;

}

/******************************************************************************/
/*  Function Name    :  vReleaseSimSysInfo                                    */
/*                                                                            */
/*  Input(s)         :                                                        */
/*                                                                            */
/*  Output           :                                                        */
/*                                                                            */
/*  Functionality    :  This method releases all the memory allocated for the */
/*                      Node List .                                           */
/*                                                                            */
/*  Member of        :  CSimSysNodeInfo                                       */
/*  Friend of        :      -                                                 */
/*                                                                            */
/*  Author(s)        :  Harika M                                              */
/*  Date Created     :  22.12.2005                                            */
/*  Modification     :  Robin G.K.
                        23.10.14, Removed unused sSIMSYSINFO structure.       */
/******************************************************************************/
void CSimSysNodeInfo::vReleaseNodeList()
{
    PSNODELIST psNextNodeList    = nullptr;

    PSNODELIST psCurrentNodeList = nullptr;

    if(m_unNumberOfNodesAdded > 0 )
    {
        psCurrentNodeList =  m_psNodesList;
        do
        {
            psNextNodeList = psCurrentNodeList->m_psNextNode;
            delete psCurrentNodeList;
            psCurrentNodeList = psNextNodeList;
            m_unNumberOfNodesAdded--;
        }
        while(psNextNodeList != nullptr );
        m_psNodesList = nullptr;
    }
}
/******************************************************************************
Function Name    :  vAddNodeToList
Input(s)         :  PSNODELIST pNode
Output           :  -
Functionality    :  Adds a Node to Node List.
Member of        :  CSimSysNodeInfo
Friend of        :      -
Author(s)        :  Robin G.K.
Date Created     :  23.10.14
Modification     :
******************************************************************************/
void CSimSysNodeInfo::vAddNodeToList(PSNODELIST pNode)
{
    if (pNode != nullptr)
    {
        pNode->m_psNextNode = nullptr;
        if(m_psNodesList == nullptr)
        {
            //If list is empty add to the first node
            m_psNodesList = pNode;
        }
        else
        {
            //Else add to the last
            PSNODELIST pTempNode = m_psNodesList;
            while (pTempNode->m_psNextNode != nullptr)
            {
                pTempNode = pTempNode->m_psNextNode;
            }
            pTempNode->m_psNextNode = pNode;
        }


        {
            if (pNode->m_sNodeInfo->m_omStrCFileName.IsEmpty() == false)
            {
                CConvertCpp objConvert;
                bool bConverted = objConvert.convertCppFile(std::string(pNode->m_sNodeInfo->m_omStrCFileName), "BUSMASTER", pNode->m_eBus);

                if (true == bConverted)
                {
                    char ch[500];
                    if (CAN == pNode->m_eBus)
                    {
                        sprintf(ch, "Cpp file %s loaded is updated as per new structures.\n Backup file is saved to %s.bak", pNode->m_sNodeInfo->m_omStrCFileName, pNode->m_sNodeInfo->m_omStrCFileName);
                    }
                    else if (J1939 == pNode->m_eBus)
                    {
                        sprintf(ch, "Cpp file %s loaded is created using old Node Simulation version. \nPlease refer help section \"CAN->J1939 API Reference\" to update as per new structures.",
                                pNode->m_sNodeInfo->m_omStrCFileName);
                    }
                    else if (LIN == pNode->m_eBus)
                    {
                        sprintf(ch, "Cpp file %s loaded is created using old Node Simulation version. \nPlease refer help section \"LIN->LIN Node Simulation->LIN API Reference\" to update as per new structures.",
                                pNode->m_sNodeInfo->m_omStrCFileName);
                    }

                    gbSendStrToTrace(ch);
                }
            }
            else if (pNode->m_sNodeInfo->m_omStrDllName.IsEmpty() == false)
            {
                CConvertCpp objConvert;
                if (false == objConvert.bCheckDllVersion(std::string(pNode->m_sNodeInfo->m_omStrDllName)))
                {
                    char ch[500];
                    if (CAN == pNode->m_eBus || LIN == pNode->m_eBus || J1939 == pNode->m_eBus)
                    {
                        sprintf(ch, "dll file %s loaded is created using old Node Simulation version. please recompile .cpp file.", pNode->m_sNodeInfo->m_omStrDllName);
                    }
                    gbSendStrToTrace(ch);
                }
            }
        }

        m_unNumberOfNodesAdded++;
    }
}