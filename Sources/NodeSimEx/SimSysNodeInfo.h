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
 * \file      SimSysNodeInfo.h
 * \brief     This header file contains the defintion of class
 * \author    Ratnadip Choudhury
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * This header file contains the defintion of class
 */

#pragma once

#include "Include/Basedefs.h"
#include "Datatypes/MsgBufVse.h"
#include "DataTypes/MsgBufAll_DataTypes.h"
#include "Utility/Utility_Thread.h"

struct sNODEINFO
{
    CCANBufFSE m_ouCanBufFSE;
    CMsgBufVSE m_ouMsgBufVSE;
    DWORD m_dwClientId;
    BYTE m_byPrefAddress;
    UINT64 m_unEcuName;
    CPARAM_THREADPROC m_ThreadProc;             // read thread for each node  
    CString m_omStrNodeName;					// Node Name
	CString m_omStrFileName;					// C File Name
	CString m_omStrDllName;						// DLL Name
	HINSTANCE m_hModuleHandle;
	BOOL m_bIsDllLoaded;						// Dll Loaded = 1, Dll Unloaded = 0.

    BOOL m_bIsAllHandlersEnabled;               // All Handlers Enabled = 1; Disabled = 0.
	BOOL m_bDllHandlersEnabled;					// Dll Handlers Enabled = 1, Disabled = 0.
	BOOL m_bTimerHandlersEnabled;				// Timer Handlers Enabled = 1, Disabled = 0.
	BOOL m_bKeyHandlersEnabled;					// Key Handlers Enabled = 1,Disabled = 0.
	BOOL m_bErrorHandlersEnabled;				// Error Handlers Enabled = 1,Disabled = 0.			
	BOOL m_bMsgHandlersEnabled;					// Message Handlers Enabled = 1,Disabled = 0.
	
	
	CStringArray m_omStrArrayTimerHandlers;     // contains all the user defined
    CStringArray m_omStrArrayKeyHandlers;       // handler functions of the 
    CStringArray m_omStrArrayMsgHandlers;       // respective handlers
    CStringArray m_omStrArrayErrorHandlers;
    CStringArray m_omStrArrayEventHandlers;
    CStringArray m_omStrArrayDLLHandlers;
    CStringArray m_omStrArrayMsgRange;
    CStringArray m_omStrArrayMsgIDandName;
    ETYPE_BUS m_eBus;
    sNODEINFO(ETYPE_BUS eBus);
    ~sNODEINFO();
    BOOL bStartThreadProc();
    BOOL bTerminateThreadProc();
};
typedef sNODEINFO* PSNODEINFO;

struct sNODELIST
{
	sNODEINFO m_sNodeInfo;
    ETYPE_BUS m_eBus;
	sNODELIST* m_psNextNode;
    sNODELIST(ETYPE_BUS eBus) : m_sNodeInfo(eBus)
    {
        m_eBus = eBus;
        m_sNodeInfo.m_eBus = m_eBus;
        m_psNextNode = NULL;
    }
    ~sNODELIST()
    {
    }
};
typedef sNODELIST SNODELIST;
typedef sNODELIST* PSNODELIST;

struct sSIMSYSINFO
{
	CString m_omStrSimSysName;		// Simulated System Name			
	UINT m_unNumberOfNodesAdded;	// Number of Nodes added
	PSNODELIST m_psNodesList;		// Nodes associated with the Simulated System.
    BOOL m_bIsSimSysModified;
	sSIMSYSINFO* m_psSimsysNext;

	sSIMSYSINFO()
	{
        m_omStrSimSysName = "";
		m_unNumberOfNodesAdded = 0; 
		m_psNodesList = NULL;
        m_bIsSimSysModified = FALSE;
		m_psSimsysNext = NULL;
	}

    void vAddNodeToList(PSNODELIST pNode)
    {
        if (pNode != NULL)
        {
            pNode->m_psNextNode = NULL;
            if(m_psNodesList == NULL)
            {
                //If list is empty add to the first node
                m_psNodesList = pNode;
            }
            else
            {
                //Else add to the last
                PSNODELIST pTempNode = m_psNodesList;
                while (pTempNode->m_psNextNode != NULL)
                {
                    pTempNode = pTempNode->m_psNextNode;
                }
                pTempNode->m_psNextNode = pNode;
            }
        }
    }
};
typedef sSIMSYSINFO* PSSIMSYSINFO;

/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////
class CSimSysNodeInfo  
{
public:
	CSimSysNodeInfo(ETYPE_BUS eBus);
	virtual ~CSimSysNodeInfo();
	
	// Delete simulated system
	BOOL bDeleteSimsysFromInfo(CString omStrSimSysname);
	
	BOOL bAddNewSimSysInfo(CString omStrPathname);

    void vAddSimSys(PSSIMSYSINFO psSimSys);
	
	//Return the SimSysInfo list first pointer
	PSSIMSYSINFO psReturnSimsysInfoListPtr();
	// Get simulated system pointer for given simulated system
	PSSIMSYSINFO psGetSimSysPointer( CString omStrSimSysName);
	// Checks if the dll name is a duplicate
	BOOL bIsDuplicateDllName( CString omStrDllName);
    // Checks if the file name is a duplicate
    BOOL bIsDuplicateCFile( CString& omStrCfile);
    // Checks if the node name is a duplicate
    BOOL bIsDuplicateNode( CString omStrNodeName);
    // Checks if the node ECU NAME is a duplicate
    BOOL bIsDuplicateEcuName( UINT64 un64EcuName);
    // Checks if the node Address is a duplicate
    BOOL bIsDuplicateAddress(BYTE byAddress);
	// Get node ptr for the given simulated system name
	PSNODEINFO psGetSimSysNodePointer( CString omStrSimsysName,
									   CString omStrNodeName);	
	// Deletes node
	BOOL bDeleteNodeFromSimSys(CString& omStrSimsysName,
                               CString omStrNodeToBeDeleted);

	// Return no of simulated systems
	UINT unGetNumberOfSimSys();
						
	// Get all node names whose dlls are loaded
	void vGetActiveNodeNames( CString omStrSimSysName, CStringArray& omStrActiveNodeList);
	// Validates if there is any C File associated with the sim sys to build them
	BOOL bIsBuildAllValid(CString omStrSimSysName);
	// Validates if there is any unloaded DLL associated with the sim sys to load them
	BOOL bIsLoadAllValid(CString omStrSimSysName);
	// Validates if there is any loaded DLL associated with the sim sys to load them
	BOOL bIsUnLoadAllValid(CString omStrSimSysName);
	

    void vSetEnableAllSimSysHandlers(CString omStrSimSysName , BOOL bIsEnabled);
    BOOL bReturnAllHandlersStatus(CString &omStrSimSysName);

    void vSetEnableAllSimSysKeyHandlers(CString &omStrSimSysName , BOOL bIsEnabled);
    BOOL bReturnAllKeyHandlersStatus(CString &omStrSimSysName);

    void vSetEnableAllSimSysMsgHandlers(CString &omStrSimSysName , BOOL bIsEnabled);
    BOOL bReturnAllMsgHandlersStatus(CString &omStrSimSysName);

    void vSetEnableAllSimSysErrorHandlers(CString &omStrSimSysName , BOOL bIsEnabled);
    BOOL bReturnAllErrorHandlersStatus(CString &omStrSimSysName);

    void vSetEnableAllSimSysTimerHandlers(CString omStrSimSysName );
    BOOL bReturnAllTimerHandlersStatus(CString &omStrSimSysName);
    void vSetDisableAllSimSysTimerHandlers(CString omStrSimSysName);

    // For node


    void vSetEnableNodeAllHandlers(CString omStrSimSysName ,
                                   CString &omStrNodeName , 
                                   BOOL bIsEnabled);
    BOOL bReturnNodeAllHandlersStatus(CString &omStrSimSysName ,
                                      CString &omStrNodeName);

    void vSetEnableNodeKeyHandlers(CString omStrSimSysName ,
                                    CString &omStrNodeName  , BOOL bIsEnabled);
    BOOL bReturnNodeKeyHandlersStatus(CString &omStrSimSysName ,
                                                CString &omStrNodeName );

    void vSetEnableNodeMsgHandlers(CString omStrSimSysName ,
                                    CString &omStrNodeName , BOOL bIsEnabled);
    BOOL bReturnNodeMsgHandlersStatus(CString &omStrSimSysName ,
                                                CString &omStrNodeName );

    void vSetEnableNodeErrorHandlers(CString omStrSimSysName ,
                                    CString &omStrNodeName , BOOL bIsEnabled);
    BOOL bReturnNodeErrorHandlersStatus(CString &omStrSimSysName , 
                                                CString &omStrNodeName);

    void vSetEnableNodeTimerHandlers(CString omStrSimSysName ,
                                        CString &omStrNodeName , BOOL bIsEnabled);
    BOOL bReturnNodeTimerHandlersStatus(CString &omStrSimSysName , 
                                                CString &omStrNodeName );

    // releases the multiple message structure pointer that is passed..
    void vReleaseSimSysInfo();
    BOOL bIsSimSysPresent(CString omStrSimsysPath);
    BOOL bIsAnyInfoInSimsysPreExist(sSIMSYSINFO *pSimsys);
    //Check if any modification is done in SimSysConfiguration 
    BOOL bIsAnySimSysModified();
        
private:
	//This data member will contain pointer to sSIMSYSINFO structure that holds active simulated system information.
    PSSIMSYSINFO m_psSimSysInfo;
    ETYPE_BUS m_eBus;
};
