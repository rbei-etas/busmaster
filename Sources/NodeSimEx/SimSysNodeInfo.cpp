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
#include "include/Error.h"
#include "Include/CAN_Error_Defs.h"
#include "HashDefines.h"
#include "SimSysNodeInfo.h"   // Class defintion file
#include "ExecuteManager.h"
#include "SimSysManager.h"
#include "Include/BaseDefs.h"
#include "GlobalObj.h"
#include "DIL_Interface/DIL_Interface_extern.h"
#include "DIL_Interface/BaseDIL_CAN.h"

#define DefDUPNODEINSYS    _T("The node in the simulated system you are trying to add is already present!!")
#define DefDUPCFILEINSYS   _T("The C file in the simulated system you are trying to add is already present!!")
#define DefDUPDLLINSYS     _T("The dll in the simulated system you are trying to add is already present!!")
const USHORT MCNET_DATA_MAX = 1000;

BOOL bIsTransitionInState( UINT unChannel, BYTE byRxError, BYTE byTxError, eERROR_STATE* eErrState)
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

void vProcessCurrErrorEntry(const SERROR_INFO& sErrInfo, DWORD dwClientId)
{
    // Get the Error code
    USHORT usErrorID;
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
            
    eERROR_STATE eErrorState = ERROR_ACTIVE;
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
        CExecuteManager::ouGetExecuteManager(CAN).vManageOnErrorHandlerCAN(eErrorState, sCanErr, dwClientId);
    }
    else
    {
        eERROR_STATE m_eCurrErrorState[ defNO_OF_CHANNELS ];
        // Handle interrupts. This is not error message and assign error code
        // to unknown
        // Interrupts are state transition indicators but not real error message
        if (sErrInfo.m_ucErrType == ERROR_BUS)
        {
            // Update Statistics information
            usErrorID = sErrInfo.m_ucReg_ErrCap & 0xE0;
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
            CExecuteManager::ouGetExecuteManager(CAN).vManageOnErrorHandlerCAN(eErrorState, sCanErr, dwClientId);
        }
    }
}

int ReadNodeDataBuffer(PSNODEINFO psNodeInfo)
{
    ASSERT(psNodeInfo != NULL);
    switch (psNodeInfo->m_eBus)
    {
        case CAN:
        {
            while (psNodeInfo->m_ouCanBufFSE.GetMsgCount() > 0)
            {
                static STCANDATA sCanData;
                INT Result = psNodeInfo->m_ouCanBufFSE.ReadFromBuffer(&sCanData);
                if (Result == ERR_READ_MEMORY_SHORT)
                {
                    TRACE(_T("ERR_READ_MEMORY_SHORT"));
                }
                if (Result == EMPTY_APP_BUFFER)
                {
                    TRACE(_T("EMPTY_APP_BUFFER"));
                }
                // Give the msg to NodeSimx for simulation
                if (sCanData.m_ucDataType == RX_FLAG)
                {
                    CExecuteManager::ouGetExecuteManager(psNodeInfo->m_eBus).vManageOnMessageHandlerCAN(sCanData.m_uDataInfo.m_sCANMsg, psNodeInfo->m_dwClientId);
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
        break;
        case J1939:
        {
            while (psNodeInfo->m_ouMsgBufVSE.GetMsgCount() > 0)
            {
                static STJ1939_MSG sJ1939Msg;
                static BYTE m_byTempData[MAX_MSG_LEN_J1939] = {0};
                static int nType = 0;
                int nSize = MAX_MSG_LEN_J1939;
                INT Result = psNodeInfo->m_ouMsgBufVSE.ReadFromBuffer(nType, m_byTempData, nSize);
                if (Result == ERR_READ_MEMORY_SHORT)
                {
                    TRACE(_T("ERR_READ_MEMORY_SHORT"));
                    break;
                }
                if (Result == EMPTY_APP_BUFFER)
                {
                    TRACE(_T("EMPTY_APP_BUFFER"));
                    break;
                }
                // Give the msg to NodeSimx for simulation
                if (Result == CALL_SUCCESS)
                {
                    sJ1939Msg.vSetDataStream(m_byTempData);
                    CExecuteFunc* pExecFunc = CExecuteManager::ouGetExecuteManager(psNodeInfo->m_eBus).
                                                pouGetExecuteFunc(psNodeInfo->m_dwClientId);
                    if ((pExecFunc != NULL) && (pExecFunc->bGetFlagStatus(EXMSG_HANDLER) == TRUE))
                    {
                        pExecFunc->vExecuteOnPGNHandler(&sJ1939Msg);
                    }

                }
            }
        }
        break;
    }
    return 0;
}

DWORD WINAPI NodeDataReadThreadProc(LPVOID pVoid)
{    
    CPARAM_THREADPROC* pThreadParam = (CPARAM_THREADPROC *) pVoid;
    if (pThreadParam == NULL)
    {
        return (DWORD)-1;
    }
    PSNODEINFO psNodeInfo = (PSNODEINFO)pThreadParam->m_pBuffer;
    if (psNodeInfo == NULL)
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
                ReadNodeDataBuffer(psNodeInfo); // Retrieve message from the driver
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

sNODEINFO::sNODEINFO(ETYPE_BUS eBus)
{
    m_eBus = eBus;
    m_byPrefAddress = 254;
    m_unEcuName     = 0x0;
    m_ouCanBufFSE.vClearMessageBuffer();
    m_dwClientId = 0;
    m_omStrNodeName     = "";
    m_omStrFileName     = "";
    m_omStrDllName      = "";

    m_bIsDllLoaded	    = FALSE;

    m_bIsAllHandlersEnabled = FALSE;
    m_bDllHandlersEnabled   = FALSE;
    m_bTimerHandlersEnabled = FALSE;
    m_bKeyHandlersEnabled   = FALSE;
    m_bErrorHandlersEnabled = FALSE;
    m_bMsgHandlersEnabled   = FALSE;

    m_omStrArrayTimerHandlers.RemoveAll();
    m_omStrArrayKeyHandlers.RemoveAll();
    m_omStrArrayErrorHandlers.RemoveAll();
    m_omStrArrayEventHandlers.RemoveAll();
    m_omStrArrayDLLHandlers.RemoveAll();
    m_omStrArrayMsgRange.RemoveAll();
    m_omStrArrayMsgIDandName.RemoveAll();
    m_omStrArrayMsgHandlers.RemoveAll();
}

sNODEINFO::~sNODEINFO()
{
    m_ThreadProc.bTerminateThread();
}
BOOL sNODEINFO::bStartThreadProc()
{
    if (m_eBus == CAN)
    {
        m_ThreadProc.m_hActionEvent = m_ouCanBufFSE.hGetNotifyingEvent();
    }
    else //For any other bus 
    {
        m_ThreadProc.m_hActionEvent = m_ouMsgBufVSE.hGetNotifyingEvent();
    }
    m_ThreadProc.m_unActionCode = INVOKE_FUNCTION;
    m_ThreadProc.m_pBuffer = this;
    return m_ThreadProc.bStartThread(NodeDataReadThreadProc);
}
BOOL sNODEINFO::bTerminateThreadProc()
{
    return m_ThreadProc.bTerminateThread();
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
CSimSysNodeInfo::CSimSysNodeInfo(ETYPE_BUS eBus)
{
    m_eBus          = eBus;
    m_psSimSysInfo  = NULL;
	
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
/******************************************************************************/
/*  Function Name    :  psReturnSimsysInfoListPtr                             
/*  Input(s)         :                                                        
/*  Output           :  PSSIMSYSINFO                                          
/*  Functionality    :  This method return the message block pointer          
/*  Member of        :  CSimSysConfigDetails                                  
/*  Friend of        :      -                                                 
/*                                                                            
/*  Author(s)        :  Anish                                            
/*  Date Created     :                                           
/*  Modifications on :                                                        
/******************************************************************************/
PSSIMSYSINFO CSimSysNodeInfo::psReturnSimsysInfoListPtr()
{
    return m_psSimSysInfo;
}


/******************************************************************************
Function Name    :  vGetActiveNodeNames
Input(s)         :  CString omStrSimSysName , CStringArray &omStrListSimsys
Output           :  CStringArray &omStrActiveNodeList
Functionality    :  Fills the list with all node names whose dlls are loaded
                    under a simulated system
Member of        :  CSimSysNodeInfo                                        
Friend of        :      -                                                 
Author(s)        :                                   
Date Created     :                                         
Modifications    :  
******************************************************************************/
void CSimSysNodeInfo::vGetActiveNodeNames( CString omStrSimSysName, 
										   CStringArray& omStrActiveNodeList)
{
    PSSIMSYSINFO pTempSimsys = psGetSimSysPointer(omStrSimSysName);
	if (pTempSimsys != NULL)
	{
		PSNODELIST pTempNode = pTempSimsys->m_psNodesList;
		while(pTempNode != NULL)
		{
			if(pTempNode->m_sNodeInfo.m_bIsDllLoaded)
			{
				omStrActiveNodeList.Add(pTempNode->m_sNodeInfo.m_omStrNodeName);
			}
			pTempNode = pTempNode->m_psNextNode;
		}	
	}
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
Modifications    :  
******************************************************************************/
BOOL CSimSysNodeInfo::bIsDuplicateDllName( CString omStrDllName)
{
	BOOL bDllPresent = FALSE;
    if (!omStrDllName.IsEmpty())
    {
        PSSIMSYSINFO pTempSimsys = m_psSimSysInfo;
        while((pTempSimsys != NULL)  && (!bDllPresent))
        {
	        PSNODELIST pTempNode = pTempSimsys->m_psNodesList;
	        while((pTempNode != NULL) && (!bDllPresent))
	        {
		        if( (pTempNode->m_sNodeInfo.m_omStrDllName.CompareNoCase(omStrDllName) == 0 ))
		        {
			        bDllPresent = TRUE;
		        }
		        pTempNode = pTempNode->m_psNextNode;
	        }
	        pTempSimsys = pTempSimsys->m_psSimsysNext;
        }
    }
	return bDllPresent;
}



/******************************************************************************
Function Name    :  bIsDuplicateCFile
Input(s)         :  CString omStrSimSysName ,CString omStrDllName
Output           :  BOOL
Functionality    :  Returns TRUE if the Dll is already associated with a node else FALSE
                    
Member of        :  CSimSysNodeInfo                                        
Friend of        :      -                                                 
Author(s)        :  Anish Kr                                     
Date Created     :                                           
Modifications    :  
******************************************************************************/
BOOL CSimSysNodeInfo::bIsDuplicateCFile(CString& omStrCfile)
{
	BOOL bCFilePresent = FALSE;
    if (!omStrCfile.IsEmpty())
    {
        PSSIMSYSINFO pTempSimsys = m_psSimSysInfo;
        while((pTempSimsys != NULL)  && (!bCFilePresent))
        {
	        PSNODELIST pTempNode = pTempSimsys->m_psNodesList;
	        while((pTempNode != NULL) && (!bCFilePresent))
	        {
                if( (pTempNode->m_sNodeInfo.m_omStrFileName.CompareNoCase(omStrCfile) == 0 ))
		        {
			        bCFilePresent = TRUE;
		        }
		        pTempNode = pTempNode->m_psNextNode;
	        }
	        pTempSimsys = pTempSimsys->m_psSimsysNext;
        }
    }
	return bCFilePresent;
}
/******************************************************************************
Function Name    :  bIsDuplicateNode
Input(s)         :  CString& omStrSimSysName ,CString omStrNodeName
Output           :  BOOL
Functionality    :  Returns TRUE if the node is already associated with a simulated
                    system else FALSE
Member of        :  CSimSysNodeInfo                                        
Friend of        :      -                                                 
Author(s)        :  Anish Kr                                    
Date Created     :                                          
Modifications    :  
******************************************************************************/
BOOL CSimSysNodeInfo::bIsDuplicateNode( CString omStrNodeName)
{
	BOOL bNodePresent = FALSE;
    if (!omStrNodeName.IsEmpty())
    {
        PSSIMSYSINFO pTempSimsys = m_psSimSysInfo;
        while((pTempSimsys != NULL)  && (!bNodePresent))
        {
	        PSNODELIST pTempNode = pTempSimsys->m_psNodesList;
	        while((pTempNode != NULL) && (!bNodePresent))
	        {
                if( (pTempNode->m_sNodeInfo.m_omStrNodeName.CompareNoCase(omStrNodeName) == 0 ))
		        {
			        bNodePresent = TRUE;
		        }
		        pTempNode = pTempNode->m_psNextNode;
	        }
	        pTempSimsys = pTempSimsys->m_psSimsysNext;
        }
    }
	return bNodePresent;
}

/******************************************************************************
Function Name    :  bIsDuplicateEcuName
Input(s)         :  UINT64 un64EcuName
Output           :  BOOL
Functionality    :  Returns TRUE if the node is already associated with a simulated
                    system else FALSE
Member of        :  CSimSysNodeInfo                                        
Friend of        :      -                                                 
Author(s)        :  Pradeep K
Date Created     :  18/03/2011                                        
Modifications    :  
******************************************************************************/
BOOL CSimSysNodeInfo::bIsDuplicateEcuName( UINT64 un64EcuName)
{
	BOOL bNodePresent = FALSE;
    PSSIMSYSINFO pTempSimsys = m_psSimSysInfo;
    while((pTempSimsys != NULL)  && (!bNodePresent))
    {
	    PSNODELIST pTempNode = pTempSimsys->m_psNodesList;
	    while((pTempNode != NULL) && (!bNodePresent))
	    {
            if(pTempNode->m_sNodeInfo.m_unEcuName == un64EcuName)
		    {
			    bNodePresent = TRUE;
		    }
		    pTempNode = pTempNode->m_psNextNode;
	    }
	    pTempSimsys = pTempSimsys->m_psSimsysNext;
    }
	return bNodePresent;
}
/******************************************************************************
Function Name    :  bIsDuplicateAddress
Input(s)         :  UINT64 un64EcuName
Output           :  BOOL
Functionality    :  
Member of        :  CSimSysNodeInfo                                        
Friend of        :      -                                                 
Author(s)        :  Pradeep K
Date Created     :  18/03/2011                                        
Modifications    :  
******************************************************************************/
BOOL CSimSysNodeInfo::bIsDuplicateAddress( BYTE byAddress)
{
    BOOL bNodePresent = FALSE;
    PSSIMSYSINFO pTempSimsys = m_psSimSysInfo;
    while((pTempSimsys != NULL)  && (!bNodePresent))
    {
	    PSNODELIST pTempNode = pTempSimsys->m_psNodesList;
	    while((pTempNode != NULL) && (!bNodePresent))
	    {
            if(pTempNode->m_sNodeInfo.m_byPrefAddress == byAddress)
		    {
			    bNodePresent = TRUE;
		    }
		    pTempNode = pTempNode->m_psNextNode;
	    }
	    pTempSimsys = pTempSimsys->m_psSimsysNext;
    }
	return bNodePresent;
}
/******************************************************************************
Function Name    :  bIsBuildAllValid
Input(s)         :  CString omStrSimSysName
Output           :  BOOL
Functionality    :  Returns TRUE if there is any C File associated 
                    with the sim sys to build them, else FALSE.
                    If the input is "" , returns TRUE if there is any C File 
                    associated with any sim sys to build them, else FALSE.
Member of        :  CSimSysNodeInfo                                        
Friend of        :      -                                                 
Author(s)        :  Harika M                                      
Date Created     :  17.1.2006                                            
Modifications    :  
******************************************************************************/
BOOL CSimSysNodeInfo::bIsBuildAllValid(CString omStrSimSysName)
{
    BOOL bIsBuildValid = FALSE;
    PSSIMSYSINFO pTempSimsys = m_psSimSysInfo;
    // when any sim sys name is specified.  or
    // when no sim sys name is specified , check for all the sim sys
    while(pTempSimsys != NULL)
    {
        if((pTempSimsys->m_omStrSimSysName == omStrSimSysName)
            || (omStrSimSysName == STR_EMPTY ))
        {
            PSNODELIST pTempNode = pTempSimsys->m_psNodesList;
            while(pTempNode != NULL)
            {
                if((pTempNode->m_sNodeInfo.m_omStrFileName != STR_EMPTY) &&
                    (!bIsBuildValid))
                {
                    bIsBuildValid = TRUE;
                    return bIsBuildValid;
                }
                pTempNode = pTempNode->m_psNextNode;
            } 
        }
        pTempSimsys = pTempSimsys->m_psSimsysNext;
    }
      
    return bIsBuildValid;	
}


/******************************************************************************
Function Name    :  bIsLoadAllValid
Input(s)         :  CString omStrSimSysName
Output           :  BOOL
Functionality    :  Returns TRUE if there is Dll associated 
                    with the sim sys to build them, else FALSE.
                    If the input is "" , returns TRUE if there is unloaded 
                    Dll associated with any sim sys to build them, else FALSE.
Member of        :  CSimSysNodeInfo                                        
Friend of        :      -                                                 
Author(s)        :  Harika M                                      
Date Created     :  17.1.2006                                            
Modifications    :  
******************************************************************************/
BOOL CSimSysNodeInfo::bIsLoadAllValid(CString omStrSimSysName)
{
    BOOL bIsLoadValid = FALSE;
    PSSIMSYSINFO pTempSimsys = m_psSimSysInfo;
    // when any sim sys name is specified.  or
    // when no sim sys name is specified , check for all the sim sys
    while(pTempSimsys != NULL)
    {
        if((pTempSimsys->m_omStrSimSysName == omStrSimSysName) ||
            (omStrSimSysName == STR_EMPTY ))
        {
            PSNODELIST pTempNode = pTempSimsys->m_psNodesList;
            while(pTempNode != NULL)
            {
                if((pTempNode->m_sNodeInfo.m_omStrDllName != STR_EMPTY) &&
                    (!bIsLoadValid))// if any dll is present
                {
                    bIsLoadValid = TRUE;
                    return bIsLoadValid;
                }
                pTempNode = pTempNode->m_psNextNode;
            }
            bIsLoadValid = FALSE; // no node present, so LoadAll invalid
           
        }
        pTempSimsys = pTempSimsys->m_psSimsysNext;
    }   
    
    return bIsLoadValid;	
}


/******************************************************************************
Function Name    :  bIsUnLoadAllValid
Input(s)         :  CString& omStrSimSysName
Output           :  BOOL
Functionality    :  Returns TRUE if there is unloaded Dll associated 
                    with the sim sys to build them, else FALSE.If the input is ""
                    check for all sim sys.
Member of        :  CSimSysNodeInfo                                      
Friend of        :      -                                                 
Author(s)        :  Harika M                                      
Date Created     :  17.1.2006                                            
Modifications    :  
******************************************************************************/
BOOL CSimSysNodeInfo::bIsUnLoadAllValid(CString omStrSimSysName)
{
	BOOL bIsUnLoadValid = FALSE;
	PSSIMSYSINFO pTempSimsys = m_psSimSysInfo;
	
	while(pTempSimsys != NULL)
	{
		if((pTempSimsys->m_omStrSimSysName == omStrSimSysName) 
            || (omStrSimSysName == STR_EMPTY ))
		{
			PSNODELIST pTempNode = pTempSimsys->m_psNodesList;
			while(pTempNode != NULL)
			{
				if((pTempNode->m_sNodeInfo.m_omStrDllName != STR_EMPTY) &&
                          (!bIsUnLoadValid)) // if any dll is present
				{
					if(pTempNode->m_sNodeInfo.m_bIsDllLoaded)   // if any dll is loaded
					{
						bIsUnLoadValid = TRUE;
						return bIsUnLoadValid;
					}
					
				}
				
				pTempNode = pTempNode->m_psNextNode;
			}
			bIsUnLoadValid = FALSE; // no node present, so UnLoadAll invalid
			
		}
		pTempSimsys = pTempSimsys->m_psSimsysNext;
	}
	return bIsUnLoadValid;	
}

/******************************************************************************
Function Name    :  psGetSimSysPointer
Input(s)         :  CString omStrSimSysName
Output           :  PSSIMSYSINFO
Functionality    :  Returns associted sim sys pointer if found, otherwise 
NULL.
Member of        :  CSimSysNodeInfo                                        
Friend of        :      -                                                 
Author(s)        :  Harika M                                      
Date Created     :  03.01.06                                            
Modifications    :  
******************************************************************************/

PSSIMSYSINFO CSimSysNodeInfo::psGetSimSysPointer( CString omStrSimSysName)
{
	PSSIMSYSINFO psSelectedSimSys = NULL;
	
	psSelectedSimSys = m_psSimSysInfo;
	
	while(psSelectedSimSys != NULL)
	{
		if(psSelectedSimSys->m_omStrSimSysName == omStrSimSysName)
		{
			return psSelectedSimSys;
		}
		psSelectedSimSys = psSelectedSimSys->m_psSimsysNext;
	}
	
	return NULL;   // if the simsys is not present.
}


/******************************************************************************
Function Name    :  psGetSimSysNodePointer
Input(s)         :  CString& omStrSimsysName, CString& omStrNodeName
Output           :  PSSIMSYSINFO
Functionality    :  Returns node ptr for the given simulated system name
if found, otherwise 
NULL.
Member of        :  CSimSysNodeInfo                                        
Friend of        :      -                                                 
Author(s)        :  Anish Kr                                  
Date Created     :                                          
Modifications    :  
******************************************************************************/
PSNODEINFO CSimSysNodeInfo::psGetSimSysNodePointer(CString omStrSimsysName, 
                                                   CString omStrNodeName)
{
    PSNODEINFO psNodeInfo = NULL;
    if ( !omStrNodeName.IsEmpty() )
    {
        //If simsys name is provided then searcch from through that 
        if ( !omStrSimsysName.IsEmpty())
        {
            PSSIMSYSINFO pTempSimsysInfo = psGetSimSysPointer(omStrSimsysName);
	        if(pTempSimsysInfo != NULL)
	        {
			    PSNODELIST pTempNode = pTempSimsysInfo->m_psNodesList;
			    while(pTempNode != NULL)
			    {
				    if(pTempNode->m_sNodeInfo.m_omStrNodeName == omStrNodeName)
				    {
					    psNodeInfo =  &(pTempNode->m_sNodeInfo);
				    }
				    pTempNode = pTempNode->m_psNextNode;
			    }
	        }
        }
        else
        {
            //Else search in all simulated systems
            //This is for execute func object calls where simsys name is not known
            BOOL bFound = FALSE;
            PSSIMSYSINFO pTempSimsysInfo = m_psSimSysInfo;
	        while((pTempSimsysInfo != NULL) && (!bFound))
	        {
			    PSNODELIST pTempNode = pTempSimsysInfo->m_psNodesList;
			    while((pTempNode != NULL) && (!bFound))
			    {
				    if(pTempNode->m_sNodeInfo.m_omStrNodeName == omStrNodeName)
				    {
					    psNodeInfo =  &(pTempNode->m_sNodeInfo);
				    }
				    pTempNode = pTempNode->m_psNextNode;
			    }
                pTempSimsysInfo = pTempSimsysInfo->m_psSimsysNext;
	        }
        }
    }
	return psNodeInfo;   // if the node is not present or the simsys is not present
}


/******************************************************************************
Function Name    :  bDeleteSimsysFromInfo
Input(s)         :  
Output           :  CString omStrSimSysname
Functionality    :  Deletes Simulated system from the datastructure if input is NULL 
                    then deletes all the simulated systems
Member of        :  CSimSysNodeInfo                                        
Friend of        :      -                                                 
Author(s)        :  Harika M                                      
Date Created     :  03.01.06                                            
Modifications    :  
******************************************************************************/
BOOL CSimSysNodeInfo::bDeleteSimsysFromInfo( CString omStrSimSysname )
{
    BOOL bReturnValue = TRUE;
	if (omStrSimSysname == STR_EMPTY)
	{ // to delete all the Simulated systems
		
        // Temporary list
        PSSIMSYSINFO pNext = NULL; // 
        PSSIMSYSINFO pPrev = NULL; // previous list
        pPrev = m_psSimSysInfo;
        while(pPrev != NULL)
        {
            // First delete all the nodes under each sim sys
            bDeleteNodeFromSimSys(pPrev->m_omStrSimSysName , STR_EMPTY );
            pPrev = pPrev->m_psSimsysNext;
        }
        //Now dalete all the simsysinfo pointers
        pPrev = m_psSimSysInfo;
        while(pPrev != NULL)
        {   
            pNext = pPrev->m_psSimsysNext;
            delete pPrev;
            pPrev = pNext;
        }
		//	return;
		m_psSimSysInfo = pPrev = NULL;		
	}
		
	else
	{
        bReturnValue = FALSE;
		// Temporary list
		PSSIMSYSINFO pCurr = NULL; // 
		PSSIMSYSINFO pPrev = NULL; // previous list
		pCurr = m_psSimSysInfo;
        //If first node
        if( pCurr->m_omStrSimSysName == omStrSimSysname)
        {
            bDeleteNodeFromSimSys(pCurr->m_omStrSimSysName , STR_EMPTY );
            m_psSimSysInfo = pCurr->m_psSimsysNext;
            delete pCurr;
            bReturnValue = TRUE;
        }
        else
        {
            //If other node go through the list
		    pPrev = pCurr;
		    while((pCurr != NULL) && (!bReturnValue))
		    {
			    if(pCurr->m_omStrSimSysName == omStrSimSysname)
			    {
				    pPrev->m_psSimsysNext = pCurr->m_psSimsysNext;
                    bDeleteNodeFromSimSys(pCurr->m_omStrSimSysName , STR_EMPTY );
				    delete pCurr;
                    bReturnValue = TRUE;
			    }
			    pPrev = pCurr;
			    pCurr = pCurr->m_psSimsysNext;
		    }
        }
	}	
    return bReturnValue;	
}

/******************************************************************************
Function Name    :  bDeleteNodeFromSimSys
Input(s)         :  CString& omStrSimsysName, CString omStrNodeToBeDeleted
Output           :  BOOL
Functionality    :  Deletes node from the sim sys datastructure, if input is STR_EMPTY 
                    then deletes all the nodes from the simulated systems
Member of        :  CSimSysNodeInfo                                        
Friend of        :      -                                                 
Author(s)        :  Anish Kr.                                   
Date Created     :                                          
Modifications    :  
******************************************************************************/
BOOL CSimSysNodeInfo::bDeleteNodeFromSimSys(CString& omStrSimsysName, 
											CString omStrNodeToBeDeleted)
{
    BOOL bReturnValue = FALSE;
    PSSIMSYSINFO pTempSimSys = psGetSimSysPointer(omStrSimsysName);
	//if simsys is present
	if(pTempSimSys != NULL)
	{
		//If the simsys is found get the nodelist pointer
		PSNODELIST pTempNodeList = pTempSimSys->m_psNodesList;
		//For delete all nodes 
		if (omStrNodeToBeDeleted== STR_EMPTY)
		{
			PSNODELIST pTemp;
			while(pTempNodeList != NULL )
			{
				pTemp = pTempNodeList;
				pTempNodeList = pTempNodeList->m_psNextNode;
                CGlobalObj::ouGetObj(m_eBus).RegisterNodeToDIL(FALSE, &(pTemp->m_sNodeInfo));
				delete pTemp;
			}
			bReturnValue = TRUE;
			//If all nodes are deleted initialize it
			pTempSimSys->m_psNodesList = NULL;
			pTempSimSys->m_unNumberOfNodesAdded = 0;
		}
		else
		{
			PSNODELIST pCurr = pTempNodeList; 
			PSNODELIST pPrev = pTempNodeList; 
			if(pTempNodeList->m_sNodeInfo.m_omStrNodeName == omStrNodeToBeDeleted)
			{
                //If first node to be deleted
				pTempNodeList = pTempNodeList->m_psNextNode;
                CGlobalObj::ouGetObj(m_eBus).RegisterNodeToDIL(FALSE, &(pCurr->m_sNodeInfo));
				delete pCurr;
				//one node is deleted
				pTempSimSys->m_psNodesList = pTempNodeList;
				pTempSimSys->m_unNumberOfNodesAdded -= 1;
				bReturnValue = TRUE;
			}
			else
			{
				BOOL bFound = FALSE;
				while( pCurr != NULL && bFound == FALSE)
				{
					if(pCurr->m_sNodeInfo.m_omStrNodeName == omStrNodeToBeDeleted)
					{
						pPrev->m_psNextNode = pCurr->m_psNextNode;
                        CGlobalObj::ouGetObj(m_eBus).RegisterNodeToDIL(FALSE, &(pCurr->m_sNodeInfo));
						delete pCurr;
						pTempSimSys->m_unNumberOfNodesAdded -= 1;
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
	}

	return bReturnValue;
	
}
/******************************************************************************/
/*  Function Name    :  bAddNewSimSysInfo                                       
/*                                                                            */
/*  Input(s)         :  CString omStrPathname                                 */
/*                                                                            */
/*  Output           :  TRUE : if the method succeeds                         */
/*                      FALSE : if memory allocation fails                    */
/*                                                                            */
/*  Functionality    :  This method sets the information from the input       */
/*                      structure into the data member. This updates the      */
/*                      information.                                          */
/*                                                                            */
/*  Member of        :  CSimSysNodeInfo                                       */
/*  Friend of        :      -                                                 */
/*                                                                            */
/*  Author(s)        :  Anish
/*  Date Created     :  
/*  Modifications By :  
/******************************************************************************/
BOOL CSimSysNodeInfo::bAddNewSimSysInfo(CString omStrPathname)
{
	BOOL bMemoryError = FALSE;		
    PSSIMSYSINFO psNewSimsys = new sSIMSYSINFO;
    if(psNewSimsys)
    {
        psNewSimsys->m_omStrSimSysName = omStrPathname;
        vAddSimSys(psNewSimsys);
        //Now save the file with new simsysinfo detail
        CSimSysConfigDetails *pSimsysConfig = 
            CSimSysManager::ouGetSimSysManager(m_eBus).pomGetSimSysConfig();
        if (pSimsysConfig != NULL)
        {
            pSimsysConfig->nSaveConfiguration(omStrPathname, psNewSimsys);
        }
    }
    else
	{
        bMemoryError = TRUE;
		AfxMessageBox( MSG_MEMORY_CONSTRAINT, MB_OK|MB_ICONINFORMATION);
	}	
	return bMemoryError;
}

/******************************************************************************
Function Name    :  unGetNumberOfSimSys
Input(s)         :  
Output           :  UINT 
Functionality    :  Returns number of sim sys in the structure.
Member of        :  CSimSysNodeInfo                                        
Friend of        :      -                                                 
Author(s)        :  Harika M                                      
Date Created     :  03.01.06                                            
Modifications    :  
******************************************************************************/
UINT CSimSysNodeInfo::unGetNumberOfSimSys()
{
    UINT unSimSysCount = 0;
    PSSIMSYSINFO pTempSimSys = m_psSimSysInfo;
    while (pTempSimSys != NULL)
    {
        unSimSysCount++;
        pTempSimSys = pTempSimSys->m_psSimsysNext;
    }
	return unSimSysCount;
}


/******************************************************************************/
/*  Function Name    :  vSetEnableAllSimSysHandlers                           */
/*                                                                            */
/*  Input(s)         :  CString &omStrSimSysName                              */
/*                        BOOL bIsEnabled                                     */
/*  Output           :                                                        */
/*  Functionality    :  This function will enable the m_bIsAllHandlersEnabled
/*						member and also enables all the handler members 
/*                      of all the nodes present under the sim sys 
/*  Member of        :  CSimSysNodeInfo                                       */
/*  Friend of        :      -                                                 */
/*                                                                            */
/*  Author(s)        :  Harika M		                                      */
/*  Date Created     :  19.01.2006                                            */
/*  Modification     :									                      */
/******************************************************************************/
void CSimSysNodeInfo::vSetEnableAllSimSysHandlers(CString omStrSimSysName, BOOL bIsEnabled )
{
    PSSIMSYSINFO pTempSimsys = psGetSimSysPointer(omStrSimSysName);
	if(pTempSimsys != NULL)
	{
		PSNODELIST pTempNode = pTempSimsys->m_psNodesList;
        while(pTempNode != NULL)  
        {
            if(pTempNode->m_sNodeInfo.m_bIsDllLoaded)
            {
                CExecuteManager::ouGetExecuteManager(m_eBus).vEnableDisableNodeHandlers(
                    &pTempNode->m_sNodeInfo , bIsEnabled);
                
                pTempNode->m_sNodeInfo.m_bIsAllHandlersEnabled = bIsEnabled; 
                
                // Set all handlers are enbled in all the nodes under the sim sys
                pTempNode->m_sNodeInfo.m_bDllHandlersEnabled = bIsEnabled; 
                pTempNode->m_sNodeInfo.m_bTimerHandlersEnabled = bIsEnabled; 
                pTempNode->m_sNodeInfo.m_bKeyHandlersEnabled = bIsEnabled; 
                pTempNode->m_sNodeInfo.m_bErrorHandlersEnabled = bIsEnabled; 
                pTempNode->m_sNodeInfo.m_bMsgHandlersEnabled = bIsEnabled; 
            }
            
            pTempNode = pTempNode->m_psNextNode;
        }
	}


}
/******************************************************************************
Function Name    :  bReturnAllHandlersStatus
Input(s)         :  CString &omStrSimSysName
Output           :  BOOL
Functionality    :  Returns TRUE if all the handlers are enabled for a 
                    given simulated system else FALSE
Member of        :  CSimSysNodeInfo                                        
Friend of        :      -                                                 
Author(s)        :  Harika M                                      
Date Created     :  19.01.2006                                            
Modifications    :  
******************************************************************************/
BOOL CSimSysNodeInfo::bReturnAllHandlersStatus(CString &omStrSimSysName)
{
    BOOL bIsEnabled = FALSE;
	PSSIMSYSINFO pTempSimsys = psGetSimSysPointer(omStrSimSysName);
	if(pTempSimsys != NULL )
	{
	    UINT nActiveNodeCount = 0;  
	    UINT nNoOfEnabledHan = 0;
		PSNODELIST pTempNode = pTempSimsys->m_psNodesList;
		while(pTempNode != NULL)
		{
			if(pTempNode->m_sNodeInfo.m_bIsAllHandlersEnabled == TRUE) 
            {  // if all the handlers are enabled of atleast 1 node 
                // bIsEnabled = TRUE;
				nNoOfEnabledHan++;
			}
            if(pTempNode->m_sNodeInfo.m_bIsDllLoaded)
            {
                nActiveNodeCount++;
            }
			pTempNode = pTempNode->m_psNextNode;
		}
        if(( nNoOfEnabledHan <= nActiveNodeCount) && (nNoOfEnabledHan != 0))
        {
            //If no. of node with enabled handler state is less than
            //no. of node having dll loaded as well as 
            bIsEnabled = TRUE;
        }
	}
	return bIsEnabled;
}
/******************************************************************************/
/*  Function Name    :  vSetEnableAllSimSysKeyHandlers                        */
/*                                                                            */
/*  Input(s)         :  CString &omStrSimSysName                              */
/*                        BOOL bIsEnabled                                     */
/*  Output           :                                                        */
/*  Functionality    :  This function will enable the m_bKeyHandlersEnabled
/*						member of all the nodes present under the sim sys. 
/*  Member of        :  CSimSysNodeInfo                                       */
/*  Friend of        :      -                                                 */
/*                                                                            */
/*  Author(s)        :  Harika M		                                      */
/*  Date Created     :  19.01.2006                                            */
/*  Modification     :									                      */
/******************************************************************************/
void CSimSysNodeInfo::vSetEnableAllSimSysKeyHandlers(CString &omStrSimSysName 
                                                        , BOOL bIsEnabled)
{
    PSSIMSYSINFO pTempSimsys = psGetSimSysPointer(omStrSimSysName);
	if(pTempSimsys != NULL)
	{
        PSNODELIST pTempNode = pTempSimsys->m_psNodesList;
        while(pTempNode != NULL)
        {
            if(pTempNode->m_sNodeInfo.m_bIsDllLoaded)
            {
                CExecuteManager::ouGetExecuteManager(m_eBus).vEnableNodeKeyHandler(             				
                    &pTempNode->m_sNodeInfo , bIsEnabled);             	          
                // Set all handlers are enbled in all the nodes under the sim sys
                
                pTempNode->m_sNodeInfo.m_bKeyHandlersEnabled = bIsEnabled; 
                
            }	
            pTempNode = pTempNode->m_psNextNode;
        }
	}
}
/******************************************************************************
Function Name    :  bReturnAllKeyHandlersStatus
Input(s)         :  CString &omStrSimSysName
Output           :  BOOL
Functionality    :  Returns TRUE if all the key handlers are enabled for a 
                    given simulated system else FALSE
Member of        :  CSimSysNodeInfo                                        
Friend of        :      -                                                 
Author(s)        :  Harika M                                      
Date Created     :  19.01.2006                                            
Modifications    :  
******************************************************************************/
BOOL CSimSysNodeInfo::bReturnAllKeyHandlersStatus(CString &omStrSimSysName)
{
    BOOL bIsEnabled = FALSE;
	PSSIMSYSINFO pTempSimsys = psGetSimSysPointer(omStrSimSysName);
	if(pTempSimsys != NULL )
	{
	    UINT nNoOfEnabledHan = 0;
	    UINT nActiveNodeCount = 0;
        PSNODELIST pTempNode = pTempSimsys->m_psNodesList;
        while(pTempNode != NULL)
        {
            if(pTempNode->m_sNodeInfo.m_bKeyHandlersEnabled == TRUE) 
            {  // if all the key handlers are enabled 
                nNoOfEnabledHan++;
            }
            if(pTempNode->m_sNodeInfo.m_bIsDllLoaded)
            {
                nActiveNodeCount++;
            }
            pTempNode = pTempNode->m_psNextNode;
        }
        if(( nNoOfEnabledHan <= nActiveNodeCount) && (nNoOfEnabledHan != 0))
        {
            bIsEnabled = TRUE;
            // return bIsEnabled;
        }
	}
	return bIsEnabled;
}

/******************************************************************************/
/*  Function Name    :  vSetEnableAllSimSysMsgHandlers                        */
/*                                                                            */
/*  Input(s)         :  CString &omStrSimSysName                              */
/*                        BOOL bIsEnabled                                     */
/*  Output           :                                                        */
/*  Functionality    :  This function will enable the m_bMsgHandlersEnabled
/*						member of all the nodes present under the sim sys. 
/*  Member of        :  CSimSysNodeInfo                                       */
/*  Friend of        :      -                                                 */
/*                                                                            */
/*  Author(s)        :  Harika M		                                      */
/*  Date Created     :  19.01.2006                                            */
/*  Modification     :									                      */
/******************************************************************************/
void CSimSysNodeInfo::vSetEnableAllSimSysMsgHandlers(CString &omStrSimSysName 
                                                        , BOOL bIsEnabled)
{
    PSSIMSYSINFO pTempSimsys = psGetSimSysPointer(omStrSimSysName);
	if (pTempSimsys != NULL)
	{
		PSNODELIST pTempNode = pTempSimsys->m_psNodesList;
        while(pTempNode != NULL)
        {
            
            // Set all handlers are enbled in all the nodes under the sim sys
            if(pTempNode->m_sNodeInfo.m_bIsDllLoaded)
            { 
                pTempNode->m_sNodeInfo.m_bMsgHandlersEnabled = bIsEnabled; 
                CExecuteManager::ouGetExecuteManager(m_eBus).vEnableNodeMessageHandler(             				
                    &pTempNode->m_sNodeInfo , bIsEnabled);
            }
            pTempNode = pTempNode->m_psNextNode;
        }
	}


}
/******************************************************************************
Function Name    :  bReturnAllMsgHandlersStatus
Input(s)         :  CString &omStrSimSysName
Output           :  BOOL
Functionality    :  Returns TRUE if all the Msg handlers are enabled for a 
                    given simulated system else FALSE
Member of        :  CSimSysNodeInfo                                        
Friend of        :      -                                                 
Author(s)        :  Harika M                                      
Date Created     :  19.01.2006                                            
Modifications    :  
******************************************************************************/
BOOL CSimSysNodeInfo::bReturnAllMsgHandlersStatus(CString &omStrSimSysName)
{
    BOOL bIsEnabled = FALSE;
	PSSIMSYSINFO pTempSimsys = psGetSimSysPointer(omStrSimSysName);
	if (pTempSimsys != NULL )
	{
	    UINT nNoOfEnabledHan = 0;
	    UINT nActiveNodeCount = 0;
        PSNODELIST pTempNode = pTempSimsys->m_psNodesList;
        while(pTempNode != NULL)
        {
            if(pTempNode->m_sNodeInfo.m_bMsgHandlersEnabled == TRUE) 
            {  // if all the Msg handlers are enabled 
                nNoOfEnabledHan++;
            }
            if(pTempNode->m_sNodeInfo.m_bIsDllLoaded)
            {
                nActiveNodeCount++;
            }
            pTempNode = pTempNode->m_psNextNode;
        }
        if(( nNoOfEnabledHan <= nActiveNodeCount) && (nNoOfEnabledHan != 0))
        {
            bIsEnabled = TRUE;
            // return bIsEnabled;
        }
	}
	return bIsEnabled;
}
/******************************************************************************/
/*  Function Name    :  vSetEnableAllSimSysErrorHandlers                      */
/*                                                                            */
/*  Input(s)         :  CString &omStrSimSysName                              */
/*                        BOOL bIsEnabled                                     */
/*  Output           :                                                        */
/*  Functionality    :  This function will enable the m_bErrorHandlersEnabled
/*						member of all the nodes present under the sim sys. 
/*  Member of        :  CSimSysNodeInfo                                       */
/*  Friend of        :      -                                                 */
/*                                                                            */
/*  Author(s)        :  Harika M		                                      */
/*  Date Created     :  19.01.2006                                            */
/*  Modification     :									                      */
/******************************************************************************/
void CSimSysNodeInfo::vSetEnableAllSimSysErrorHandlers(CString &omStrSimSysName 
                                                        , BOOL bIsEnabled)
{
    PSSIMSYSINFO pTempSimsys = psGetSimSysPointer(omStrSimSysName);
	if (pTempSimsys != NULL)
	{
        PSNODELIST pTempNode = pTempSimsys->m_psNodesList;
        while(pTempNode != NULL)
        {
            
            // Set all handlers are enbled in all the nodes under the sim sys
            if(pTempNode->m_sNodeInfo.m_bIsDllLoaded)
            {
                pTempNode->m_sNodeInfo.m_bErrorHandlersEnabled = bIsEnabled; 
                CExecuteManager::ouGetExecuteManager(m_eBus).vEnableNodeErrorHandler(             				
                    &pTempNode->m_sNodeInfo , bIsEnabled);             				
            }	
            pTempNode = pTempNode->m_psNextNode;
		}
    }

}
/******************************************************************************
Function Name    :  bReturnAllErrorHandlersStatus
Input(s)         :  CString &omStrSimSysName
Output           :  BOOL
Functionality    :  Returns TRUE if all the error handlers are enabled for a 
                    given simulated system else FALSE
Member of        :  CSimSysNodeInfo                                        
Friend of        :      -                                                 
Author(s)        :  Harika M                                      
Date Created     :  19.01.2006                                            
Modifications    :  
******************************************************************************/
BOOL CSimSysNodeInfo::bReturnAllErrorHandlersStatus(CString &omStrSimSysName)
{
    BOOL bIsEnabled = FALSE;
	PSSIMSYSINFO pTempSimsys = psGetSimSysPointer(omStrSimSysName);
    
	if (pTempSimsys != NULL )
	{
	    UINT nNoOfEnabledHan = 0;
	    UINT nActiveNodeCount = 0;
        PSNODELIST pTempNode = pTempSimsys->m_psNodesList;
        while(pTempNode != NULL)
        {
            if(pTempNode->m_sNodeInfo.m_bErrorHandlersEnabled == TRUE) 
            {  // if all the error handlers are enabled 
                nNoOfEnabledHan++;
            }
            if(pTempNode->m_sNodeInfo.m_bIsDllLoaded)
            {
                nActiveNodeCount++;
            }
            pTempNode = pTempNode->m_psNextNode;
        }
        if(( nNoOfEnabledHan <= nActiveNodeCount) && (nNoOfEnabledHan != 0))
        {
            bIsEnabled = TRUE;
        }
    }
	return bIsEnabled;
}
/******************************************************************************/
/*  Function Name    :  vSetEnableAllSimSysTimerHandlers                      */
/*                                                                            */
/*  Input(s)         :  CString &omStrSimSysName                              */
/*                        BOOL bIsEnabled                                     */
/*  Output           :                                                        */
/*  Functionality    :  This function will enable the m_bTimerHandlersEnabled
/*						member of all the nodes present under the sim sys. 
/*  Member of        :  CSimSysNodeInfo                                       */
/*  Friend of        :      -                                                 */
/*                                                                            */
/*  Author(s)        :  Harika M		                                      */
/*  Date Created     :  19.01.2006                                            */
/*  Modification     :									                      */
/******************************************************************************/
void CSimSysNodeInfo::vSetEnableAllSimSysTimerHandlers(CString omStrSimSysName)
{
    /*CStringArray omStrNodeList;
	omStrNodeList.RemoveAll();
    
	vGetActiveNodeNames( omStrSimSysName , omStrNodeList);         
    // Set all handlers are enabled in all the nodes under the sim sys         
	CExecuteManager::ouGetExecuteManager().vSetResetOnTimerHandler(             				
				                          omStrNodeList , TRUE);*/

    //Above code is commented because now the timer dialog box is not

    PSSIMSYSINFO pTempSimsys = psGetSimSysPointer(omStrSimSysName);
	if (pTempSimsys != NULL)
	{
		PSNODELIST pTempNode = pTempSimsys->m_psNodesList;
		while(pTempNode != NULL)
		{
			CExecuteManager::ouGetExecuteManager(m_eBus).vEnableNodeTimerHandler(             				
				&pTempNode->m_sNodeInfo , TRUE);
			// Set all timer handlers are disabled in the node
			pTempNode->m_sNodeInfo.m_bTimerHandlersEnabled = TRUE; 
			pTempNode = pTempNode->m_psNextNode;
		}
	}

}

/******************************************************************************/
/*  Function Name    :  vSetDisableAllSimSysTimerHandlers                     */
/*                                                                            */
/*  Input(s)         :  CString &omStrSimSysName                              */
/*                        BOOL bIsEnabled                                     */
/*  Output           :                                                        */
/*  Functionality    :  This function will disable the m_bTimerHandlersEnabled
/*						member of all the nodes present under the sim sys. 
/*  Member of        :  CSimSysNodeInfo                                       */
/*  Friend of        :      -                                                 */
/*                                                                            */
/*  Author(s)        :  Harika M		                                      */
/*  Date Created     :  19.01.2006                                            */
/*  Modification     :									                      */
/******************************************************************************/
void CSimSysNodeInfo::vSetDisableAllSimSysTimerHandlers(CString omStrSimSysName)
{
    PSSIMSYSINFO pTempSimsys = psGetSimSysPointer(omStrSimSysName);
	if (pTempSimsys != NULL)
	{
		PSNODELIST pTempNode = pTempSimsys->m_psNodesList;
		while(pTempNode != NULL)
		{
			CExecuteManager::ouGetExecuteManager(m_eBus).vEnableNodeTimerHandler(             				
				&pTempNode->m_sNodeInfo , FALSE);
			// Set all timer handlers are disabled in the node
			pTempNode->m_sNodeInfo.m_bTimerHandlersEnabled = FALSE; 
			pTempNode = pTempNode->m_psNextNode;
		}
	}
    
}
/******************************************************************************
Function Name    :  bReturnAllTimerHandlersStatus
Input(s)         :  CString &omStrSimSysName
Output           :  BOOL
Functionality    :  Returns TRUE if all the timer handlers are enabled for a 
                    given simulated system else FALSE
Member of        :  CSimSysNodeInfo                                        
Friend of        :      -                                                 
Author(s)        :  Harika M                                      
Date Created     :  19.01.2006                                            
Modifications    :  
******************************************************************************/
BOOL CSimSysNodeInfo::bReturnAllTimerHandlersStatus(CString &omStrSimSysName)
{
    BOOL bIsEnabled = FALSE;
	PSSIMSYSINFO pTempSimsys = psGetSimSysPointer(omStrSimSysName);
    
	if (pTempSimsys != NULL )
	{
	    UINT nNoOfEnabledHan = 0;
	    UINT nActiveNodeCount = 0;
		PSNODELIST pTempNode = pTempSimsys->m_psNodesList;
		while(pTempNode != NULL)
		{
			if(pTempNode->m_sNodeInfo.m_bTimerHandlersEnabled == TRUE) 
            {  // if all the timer handlers are enabled 
				nNoOfEnabledHan++;
			}
			if(pTempNode->m_sNodeInfo.m_bIsDllLoaded)
            {
                nActiveNodeCount++;
            }
            pTempNode = pTempNode->m_psNextNode;
        }
        if(( nNoOfEnabledHan <= nActiveNodeCount) && (nNoOfEnabledHan != 0))
        {
            bIsEnabled = TRUE;
            // return bIsEnabled;
        }
	}
	return bIsEnabled;
}

/********************************************************************************
 FOR NODE HANDLERS
/******************************************************************************/
/*  Function Name    :  vSetEnableNodeAllHandlers                             */
/*                                                                            */
/*  Input(s)         :  CString &omStrSimSysName , CString &omStrNodeName     */
/*                        BOOL bIsEnabled                                     */
/*  Output           :                                                        */
/*  Functionality    :  This function will enable the m_bIsAllHandlersEnabled
/*						member and also enables all the handler members 
/*                      ofthe node. 
/*  Member of        :  CSimSysNodeInfo                                       */
/*  Friend of        :      -                                                 */
/*                                                                            */
/*  Author(s)        :  Harika M		                                      */
/*  Date Created     :  19.01.2006                                            */
/*  Modification     :									                      */
/******************************************************************************/
void CSimSysNodeInfo::vSetEnableNodeAllHandlers(CString omStrSimSysName ,
                                                CString &omStrNodeName , 
                                                BOOL bIsEnabled)
{
    PSSIMSYSINFO pTempSimsys = m_psSimSysInfo;
	while(pTempSimsys != NULL)
	{
		if((pTempSimsys->m_omStrSimSysName == omStrSimSysName)||
           ( omStrSimSysName == STR_EMPTY))
		{
			PSNODELIST pTempNode = pTempSimsys->m_psNodesList;
			while(pTempNode != NULL)
			{
                if(pTempNode->m_sNodeInfo.m_omStrNodeName == omStrNodeName)
				{
                    CExecuteManager::ouGetExecuteManager(m_eBus).vEnableDisableNodeHandlers(
                                                  &pTempNode->m_sNodeInfo , bIsEnabled);

				    pTempNode->m_sNodeInfo.m_bIsAllHandlersEnabled = bIsEnabled; 
                
                    // Set all handlers are enbled in the node
                    pTempNode->m_sNodeInfo.m_bDllHandlersEnabled = bIsEnabled; 
                    pTempNode->m_sNodeInfo.m_bTimerHandlersEnabled = bIsEnabled; 
                    pTempNode->m_sNodeInfo.m_bKeyHandlersEnabled = bIsEnabled; 
                    pTempNode->m_sNodeInfo.m_bErrorHandlersEnabled = bIsEnabled; 
                    pTempNode->m_sNodeInfo.m_bMsgHandlersEnabled = bIsEnabled; 
                }
                				
					
				pTempNode = pTempNode->m_psNextNode;
			}
            
		}
	
		pTempSimsys = pTempSimsys->m_psSimsysNext;
	}


}
/******************************************************************************
Function Name    :  bReturnNodeAllHandlersStatus
Input(s)         :  CString &omStrSimSysName , CString &omStrNodeName
Output           :  BOOL
Functionality    :  Returns TRUE if all the handlers are enabled for a 
                    given node else FALSE
Member of        :  CSimSysNodeInfo                                        
Friend of        :      -                                                 
Author(s)        :  Harika M                                      
Date Created     :  19.01.2006                                            
Modifications    :  
******************************************************************************/
BOOL CSimSysNodeInfo::bReturnNodeAllHandlersStatus(CString &omStrSimSysName ,
                                                   CString &omStrNodeName)
{
    BOOL bIsEnabled = FALSE;
	PSSIMSYSINFO pTempSimsys = psGetSimSysPointer(omStrSimSysName);  
	if (pTempSimsys != NULL )
	{
		PSNODELIST pTempNode = pTempSimsys->m_psNodesList;
		while(pTempNode != NULL)  
		{
            if(pTempNode->m_sNodeInfo.m_omStrNodeName == omStrNodeName)
			{
				bIsEnabled = pTempNode->m_sNodeInfo.m_bIsAllHandlersEnabled;
            }
			pTempNode = pTempNode->m_psNextNode;
		}
	}
	return bIsEnabled;
}
/******************************************************************************/
/*  Function Name    :  vSetEnableNodeKeyHandlers                             */
/*                                                                            */
/*  Input(s)         :  CString &omStrSimSysName , CString &omStrNodeName     */
/*                        BOOL bIsEnabled                                     */
/*  Output           :                                                        */
/*  Functionality    :  This function will enable the m_bKeyHandlersEnabled
/*						member of the nodes. 
/*  Member of        :  CSimSysNodeInfo                                       */
/*  Friend of        :      -                                                 */
/*                                                                            */
/*  Author(s)        :  Harika M		                                      */
/*  Date Created     :  19.01.2006                                            */
/*  Modification     :									                      */
/******************************************************************************/
void CSimSysNodeInfo::vSetEnableNodeKeyHandlers(CString omStrSimSysName ,
                                      CString &omStrNodeName  , BOOL bIsEnabled)
{
    PSSIMSYSINFO pTempSimsys = m_psSimSysInfo;
	while(pTempSimsys != NULL)
	{
		if((pTempSimsys->m_omStrSimSysName == omStrSimSysName) ||
                                        ( omStrSimSysName == STR_EMPTY))
		{
			PSNODELIST pTempNode = pTempSimsys->m_psNodesList;
			while(pTempNode != NULL)
			{
				if(pTempNode->m_sNodeInfo.m_omStrNodeName == omStrNodeName)
				{  
                    CExecuteManager::ouGetExecuteManager(m_eBus).vEnableNodeKeyHandler(             				
				                    &pTempNode->m_sNodeInfo , bIsEnabled);             	
              // Set all key handlers are enabled in the node
                 
                    pTempNode->m_sNodeInfo.m_bKeyHandlersEnabled = bIsEnabled; 
                }
                             				
					
				pTempNode = pTempNode->m_psNextNode;
			}
            
		}
		
		pTempSimsys = pTempSimsys->m_psSimsysNext;
	}


}
/******************************************************************************
Function Name    :  bReturnNodeKeyHandlersStatus
Input(s)         :  CString &omStrSimSysName , CString &omStrNodeName
Output           :  BOOL
Functionality    :  Returns TRUE if all the key handlers are enabled for a 
                    given node else FALSE
Member of        :  CSimSysNodeInfo                                        
Friend of        :      -                                                 
Author(s)        :  Harika M                                      
Date Created     :  19.01.2006                                            
Modifications    :  
******************************************************************************/
BOOL CSimSysNodeInfo::bReturnNodeKeyHandlersStatus(CString &omStrSimSysName ,
                                                    CString &omStrNodeName )
{
    BOOL bIsEnabled = FALSE;
	PSSIMSYSINFO pTempSimsys = psGetSimSysPointer(omStrSimSysName);
	if (pTempSimsys != NULL )
	{
		PSNODELIST pTempNode = pTempSimsys->m_psNodesList;
		while(pTempNode != NULL)  
		{
            if(pTempNode->m_sNodeInfo.m_omStrNodeName == omStrNodeName)
			{
				bIsEnabled = pTempNode->m_sNodeInfo.m_bKeyHandlersEnabled;
                //  return bIsEnabled;
            }
			pTempNode = pTempNode->m_psNextNode;
		}
	}
	return bIsEnabled;
}

/******************************************************************************/
/*  Function Name    :  vSetEnableAllSimSysMsgHandlers                        */
/*                                                                            */
/*  Input(s)         :  CString &omStrSimSysName , CString &omStrNodeName     */
/*                        BOOL bIsEnabled                                     */
/*  Output           :                                                        */
/*  Functionality    :  This function will enable the m_bMsgHandlersEnabled
/*						member of the node. 
/*  Member of        :  CSimSysNodeInfo                                       */
/*  Friend of        :      -                                                 */
/*                                                                            */
/*  Author(s)        :  Harika M		                                      */
/*  Date Created     :  19.01.2006                                            */
/*  Modification     :									                      */
/******************************************************************************/
void CSimSysNodeInfo::vSetEnableNodeMsgHandlers(CString omStrSimSysName ,
                                        CString &omStrNodeName , BOOL bIsEnabled)
{
    PSSIMSYSINFO pTempSimsys = m_psSimSysInfo;
	while(pTempSimsys != NULL)
	{
		if((pTempSimsys->m_omStrSimSysName == omStrSimSysName) ||
                                        ( omStrSimSysName == STR_EMPTY))
		{
			PSNODELIST pTempNode = pTempSimsys->m_psNodesList;
			while(pTempNode != NULL)
			{
                if(pTempNode->m_sNodeInfo.m_omStrNodeName == omStrNodeName)
				{
                     CExecuteManager::ouGetExecuteManager(m_eBus).vEnableNodeMessageHandler(             				
				                    &pTempNode->m_sNodeInfo , bIsEnabled);
                    // Set all msg handlers are enabled in the node
                 
                    pTempNode->m_sNodeInfo.m_bMsgHandlersEnabled = bIsEnabled; 
                }
                              				
					
				pTempNode = pTempNode->m_psNextNode;
			}
            
		}
		
		pTempSimsys = pTempSimsys->m_psSimsysNext;
	}


}
/******************************************************************************
Function Name    :  bReturnNodeMsgHandlersStatus
Input(s)         :  CString &omStrSimSysName , CString &omStrNodeName
Output           :  BOOL
Functionality    :  Returns TRUE if all the Msg handlers are enabled for a 
                    given node else FALSE
Member of        :  CSimSysNodeInfo                                        
Friend of        :      -                                                 
Author(s)        :  Harika M                                      
Date Created     :  19.01.2006                                            
Modifications    :  
******************************************************************************/
BOOL CSimSysNodeInfo::bReturnNodeMsgHandlersStatus(CString &omStrSimSysName ,
                                                  CString &omStrNodeName )
{
    BOOL bIsEnabled = FALSE;
	PSSIMSYSINFO pTempSimsys = psGetSimSysPointer(omStrSimSysName);
    if (pTempSimsys != NULL  )
	{
		PSNODELIST pTempNode = pTempSimsys->m_psNodesList;
		while(pTempNode != NULL)
		{
            if(pTempNode->m_sNodeInfo.m_omStrNodeName == omStrNodeName)
			{
				bIsEnabled = pTempNode->m_sNodeInfo.m_bMsgHandlersEnabled;
                // return bIsEnabled;
            }
			pTempNode = pTempNode->m_psNextNode;
		}
	}
	return bIsEnabled;
}
/******************************************************************************/
/*  Function Name    :  vSetEnableNodeErrorHandlers                           */
/*                                                                            */
/*  Input(s)         :  CString &omStrSimSysName , CString &omStrNodeName     */
/*                        BOOL bIsEnabled                                     */
/*  Output           :                                                        */
/*  Functionality    :  This function will enable the m_bErrorHandlersEnabled
/*						member of the node. 
/*  Member of        :  CSimSysNodeInfo                                       */
/*  Friend of        :      -                                                 */
/*                                                                            */
/*  Author(s)        :  Harika M		                                      */
/*  Date Created     :  19.01.2006                                            */
/*  Modification     :									                      */
/******************************************************************************/
void CSimSysNodeInfo::vSetEnableNodeErrorHandlers(CString omStrSimSysName ,
                                        CString &omStrNodeName , BOOL bIsEnabled)
{
    PSSIMSYSINFO pTempSimsys = m_psSimSysInfo;
	while(pTempSimsys != NULL)
	{
		if((pTempSimsys->m_omStrSimSysName == omStrSimSysName) ||
                                        ( omStrSimSysName == STR_EMPTY))
		{
			PSNODELIST pTempNode = pTempSimsys->m_psNodesList;
			while(pTempNode != NULL)
			{
				if(pTempNode->m_sNodeInfo.m_omStrNodeName == omStrNodeName)
				{ 
                   CExecuteManager::ouGetExecuteManager(m_eBus).vEnableNodeErrorHandler(             				
				                    &pTempNode->m_sNodeInfo , bIsEnabled);
          // Set all error handlers are enabled in the node
                 
                    pTempNode->m_sNodeInfo.m_bErrorHandlersEnabled = bIsEnabled; 
                }
                              				
					
				pTempNode = pTempNode->m_psNextNode;
			}
            
		}
		
		pTempSimsys = pTempSimsys->m_psSimsysNext;
	}


}
/******************************************************************************
Function Name    :  bReturnNodeErrorHandlersStatus
Input(s)         :  CString &omStrSimSysName , CString &omStrNodeName
Output           :  BOOL
Functionality    :  Returns TRUE if all the error handlers are enabled for a 
                    given node else FALSE
Member of        :  CSimSysNodeInfo                                        
Friend of        :      -                                                 
Author(s)        :  Harika M                                      
Date Created     :  19.01.2006                                            
Modifications    :  
******************************************************************************/
BOOL CSimSysNodeInfo::bReturnNodeErrorHandlersStatus(CString &omStrSimSysName , 
                                                    CString &omStrNodeName)
{
    BOOL bIsEnabled = FALSE;
	PSSIMSYSINFO pTempSimsys = psGetSimSysPointer(omStrSimSysName);
	if (pTempSimsys != NULL  )
	{
		PSNODELIST pTempNode = pTempSimsys->m_psNodesList;
		while(pTempNode != NULL)  
		{
			if(pTempNode->m_sNodeInfo.m_omStrNodeName == omStrNodeName)
			{
				bIsEnabled = pTempNode->m_sNodeInfo.m_bErrorHandlersEnabled;
                //  return bIsEnabled;
            }
			pTempNode = pTempNode->m_psNextNode;
		}
	}
	return bIsEnabled;
}
/******************************************************************************/
/*  Function Name    :  vSetEnableNodeTimerHandlers                           */
/*                                                                            */
/*  Input(s)         :  CString &omStrSimSysName , CString &omStrNodeName     */
/*                        BOOL bIsEnabled                                     */
/*  Output           :                                                        */
/*  Functionality    :  This function will enable the m_bTimerHandlersEnabled
/*						member of the dll present under the node. 
/*  Member of        :  CSimSysNodeInfo                                       */
/*  Friend of        :      -                                                 */
/*                                                                            */
/*  Author(s)        :  Harika M		                                      */
/*  Date Created     :  19.01.2006                                            */
/*  Modification     :									                      */
/******************************************************************************/
void CSimSysNodeInfo::vSetEnableNodeTimerHandlers(CString omStrSimSysName ,
                                                  CString &omStrNodeName , 
                                                  BOOL bIsEnabled)
{
    CStringArray omStrNodeList;
    PSSIMSYSINFO pTempSimsys = m_psSimSysInfo;
    BOOL bFound = FALSE;
	while((pTempSimsys != NULL) && (TRUE != bFound))
	{
		if((pTempSimsys->m_omStrSimSysName == omStrSimSysName) ||
           ( omStrSimSysName == STR_EMPTY))
		{
			PSNODELIST pTempNode = pTempSimsys->m_psNodesList;
			while((pTempNode != NULL) && (TRUE != bFound))
			{
				if(pTempNode->m_sNodeInfo.m_omStrNodeName == omStrNodeName)
				{ 
                    omStrNodeList.Add( omStrNodeName );
                    CExecuteManager::ouGetExecuteManager(m_eBus).
                        vEnableNodeTimerHandler(&pTempNode->m_sNodeInfo, bIsEnabled); 
                    /*CExecuteManager::ouGetExecuteManager().vSetResetOnTimerHandler(             				
				                                           omStrNodeList , TRUE );*/
                    pTempNode->m_sNodeInfo.m_bTimerHandlersEnabled = bIsEnabled; 
                    bFound = TRUE;
                }
                              				
					
				pTempNode = pTempNode->m_psNextNode;
			}
            
		}
		
		pTempSimsys = pTempSimsys->m_psSimsysNext;
	}


}
/******************************************************************************
Function Name    :  bReturnNodeTimerHandlersStatus
Input(s)         :  CString &omStrSimSysName , CString &omStrNodeName
Output           :  BOOL
Functionality    :  Returns TRUE if all the timer handlers are enabled for a 
                    given node else FALSE
Member of        :  CSimSysNodeInfo                                        
Friend of        :      -                                                 
Author(s)        :  Harika M                                      
Date Created     :  19.01.2006                                            
Modifications    :  
******************************************************************************/
BOOL CSimSysNodeInfo::bReturnNodeTimerHandlersStatus(CString &omStrSimSysName , 
                                                    CString &omStrNodeName )
{
    BOOL bIsEnabled = FALSE;
	PSSIMSYSINFO pTempSimsys = psGetSimSysPointer(omStrSimSysName);
	if (pTempSimsys != NULL  )
	{
		PSNODELIST pTempNode = pTempSimsys->m_psNodesList;
		while(pTempNode != NULL)  
		{
			if(pTempNode->m_sNodeInfo.m_omStrNodeName == omStrNodeName)
			{
				bIsEnabled = pTempNode->m_sNodeInfo.m_bTimerHandlersEnabled;
                //  return bIsEnabled;
            }
            pTempNode = pTempNode->m_psNextNode;
		}
	}
	return bIsEnabled;
}

/******************************************************************************/
/*  Function Name    :  vReleaseSimSysInfo                                    */
/*                                                                            */
/*  Input(s)         :                                                        */
/*                                                                            */
/*  Output           :                                                        */
/*                                                                            */
/*  Functionality    :  This method releases all the memory allocated for the */
/*                      sim sys info structure.		   			              */
/*                                                                            */
/*  Member of        :  CSimSysNodeInfo                                       */
/*  Friend of        :      -                                                 */
/*                                                                            */
/*  Author(s)        :  Harika M                                              */
/*  Date Created     :  22.12.2005                                            */
/*  Modifications By :  
/******************************************************************************/
void CSimSysNodeInfo::vReleaseSimSysInfo()
{
    PSSIMSYSINFO psNextSimSysInfo    = NULL;
    PSNODELIST psNextNodeList    = NULL;
    PSSIMSYSINFO psCurrentSimSysInfo = NULL;
    PSNODELIST psCurrentNodeList = NULL;
	PSSIMSYSINFO psTempSimSysInfo = m_psSimSysInfo;
    if(psTempSimSysInfo != NULL )
    {
        psCurrentSimSysInfo = psTempSimSysInfo;
        do
        {
            if(psCurrentSimSysInfo->m_unNumberOfNodesAdded > 0 )
            {
                psCurrentNodeList = 
					psCurrentSimSysInfo->m_psNodesList;
                do
                {
                    psNextNodeList = psCurrentNodeList->m_psNextNode;
                    delete psCurrentNodeList;
                    psCurrentNodeList = psNextNodeList;
                }while(psNextNodeList != NULL );
                
            }
            psNextSimSysInfo = psCurrentSimSysInfo->m_psSimsysNext;
            delete psCurrentSimSysInfo;
            psCurrentSimSysInfo = psNextSimSysInfo;
        }while(psNextSimSysInfo != NULL );
    }
    psTempSimSysInfo = NULL;
    m_psSimSysInfo = NULL;
    
}  

BOOL CSimSysNodeInfo::bIsSimSysPresent(CString omStrSimsysPath)
{
    BOOL bReturn = FALSE;
	PSSIMSYSINFO psSimSys = m_psSimSysInfo;
	while(psSimSys != NULL)
	{
		if(psSimSys->m_omStrSimSysName == omStrSimsysPath)
		{
			bReturn = TRUE;
		}
		psSimSys = psSimSys->m_psSimsysNext;
	}
	
	return bReturn;  
}

void CSimSysNodeInfo::vAddSimSys(PSSIMSYSINFO psSimSys)
{
    if (psSimSys != NULL)
    {
        if (m_psSimSysInfo == NULL)
        {
            //If the list is empty add to the start
            m_psSimSysInfo = psSimSys;
            m_psSimSysInfo->m_psSimsysNext = NULL;
        }
        else
        {
            //Else add to the end of the list
            PSSIMSYSINFO psTempSimSys = m_psSimSysInfo;
            while (psTempSimSys->m_psSimsysNext != NULL)
            {
                psTempSimSys = psTempSimSys->m_psSimsysNext;
            }
            psTempSimSys->m_psSimsysNext = psSimSys;
        }
    }
}

/******************************************************************************
  Function Name    :  bIsAnyInfoInSimsysPreExist                           
                                                                           
  Input(s)         :                                                       
  Output           :                                                       
  Functionality    : checks if any info in the given simsys is already present
                     in the configuration
  Member of        :                                      
  Friend of        :      -                                                
                                                                           
  Author(s)        :  Anish Kr.                                            
  Date Created     :  01.03.09                                      
  Modification     :  
/******************************************************************************/
BOOL CSimSysNodeInfo::bIsAnyInfoInSimsysPreExist(sSIMSYSINFO *pSimsys) 
{
    BOOL bDuplicate = FALSE;
    if (pSimsys != NULL)
    {
        CString omError = _T("");
        PSNODELIST psTempNode =  pSimsys->m_psNodesList;
        while ((psTempNode != NULL) && (!bDuplicate) )
        {
            // Checks if the node name is a duplicate
            bDuplicate = bIsDuplicateNode( psTempNode->m_sNodeInfo.m_omStrNodeName);
            if (!bDuplicate)
            {
                // Checks if the file name is a duplicate
                bDuplicate =  bIsDuplicateCFile( psTempNode->m_sNodeInfo.m_omStrFileName);
                if (!bDuplicate)
                {
	                // Checks if the dll name is a duplicate
                    bDuplicate =  bIsDuplicateDllName( psTempNode->m_sNodeInfo.m_omStrDllName);
                    if (bDuplicate)
                    {
                        omError = DefDUPDLLINSYS;
                    }
                }
                else
                {
                    omError = DefDUPDLLINSYS;
                }
            }
            else
            {
                omError = DefDUPNODEINSYS;
            }
            psTempNode = psTempNode->m_psNextNode;
        }
        if (bDuplicate)
        {
            AfxMessageBox(omError);
        }
    }
    return bDuplicate;
}

/******************************************************************************
  Function Name    :  bIsAnySimSysModified                           
                                                                           
  Input(s)         :                                                       
  Output           :                                                       
  Functionality    : checks if any simsys info is modified
  Member of        :                                      
  Friend of        :      -                                                
                                                                           
  Author(s)        :  Anish Kr.                                            
  Date Created     :  01.03.09                                      
  Modification     :  
/******************************************************************************/
BOOL CSimSysNodeInfo::bIsAnySimSysModified()
{
    BOOL bModified = FALSE;
    PSSIMSYSINFO psTempSimSys = m_psSimSysInfo;
    while ((psTempSimSys != NULL) && (!bModified))
    {
        if (psTempSimSys->m_bIsSimSysModified)
        {
            bModified = TRUE;
        }
        psTempSimSys = psTempSimSys->m_psSimsysNext;
    }
    return bModified;
}