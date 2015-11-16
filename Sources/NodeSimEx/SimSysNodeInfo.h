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
#include "Utility/DirectoryWatcher.h"
enum eNODE_FILE_TYPE
{
    NODE_FILE_DLL,
    NODE_FILE_C_CPP,
};
enum eNODE_STATE
{
    NODE_BUILD_SUCCESS,
    NODE_BUILD_FAIL,
    NODE_NOT_BUILT,
    NODE_REQ_CLEAN_BUILT,
};
struct sNODEINFO:private IDirectoryUpdate
{
    CCANBufFSE* m_ouCanBufFSE;
    CMsgBufVSE* m_ouMsgBufVSE;
    CLINBufFSE* m_ouLinBufSE;
    DWORD m_dwClientId;
    BYTE m_byPrefAddress;
    UINT64 m_unEcuName;
    CPARAM_THREADPROC m_ThreadProc;             // read thread for each node
    CString m_omStrNodeName;                    // Node Name
    CString m_omStrCFileName;                    // C File Name
    CString m_omStrDllName;                     // DLL Name
    HINSTANCE m_hModuleHandle;
    BOOL m_bIsNodeEnabled;                      //Check Node is Enabled = 1; Disabled = 0;
    eNODE_STATE m_eNodeState;
    eNODE_FILE_TYPE m_eNodeFileType;
    UINT m_unChannel;
    unsigned long long m_ouLastDBChecksum;
    CStringArray m_omStrArrayTimerHandlers;     // contains all the user defined
    CStringArray m_omStrArrayKeyHandlers;       // handler functions of the
    CStringArray m_omStrArrayMsgHandlers;       // respective handlers
    CStringArray m_omStrArrayErrorHandlers;
    CStringArray m_omStrArrayEventHandlers;
    CStringArray m_omStrArrayDLLHandlers;
    CStringArray m_omStrArrayMsgRange;
    CStringArray m_omStrArrayMsgIDandName;
    CDirectoryWatcher* m_pouDirectoryWatcher;
    ETYPE_BUS m_eBus;
    BOOL bStartThreadProc();
    BOOL bTerminateThreadProc();
    HRESULT RegisterFileToMonitor(std::string strFileName,bool bRegister);
    ~sNODEINFO();
    sNODEINFO(ETYPE_BUS eBus);
private:                                            //Following functions are not tested.Enable if needed.

    sNODEINFO& operator = ( const sNODEINFO& sobj); //Assingment operator
    sNODEINFO ( const sNODEINFO& sobj); // copy constructor
    void OnFileChanged(const std::string strFileName,DWORD dwAction);
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
        m_psNextNode = nullptr;
    }
    ~sNODELIST()
    {
    }
};
typedef sNODELIST SNODELIST;
typedef sNODELIST* PSNODELIST;


/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////
class CSimSysNodeInfo
{
public:
    CSimSysNodeInfo(ETYPE_BUS eBus);
    virtual ~CSimSysNodeInfo();

    // Checks if the dll name is a duplicate
    BOOL bIsDuplicateDllName( CString omStrDllName);
    // Checks if the file name is a duplicate
    BOOL bIsDuplicateCFile( CString omStrCfile);
    // Checks if the node name is a duplicate
    BOOL bIsDuplicateNode( CString omStrNodeName);
    // Checks if the node ECU NAME is a duplicate
    BOOL bIsDuplicateEcuName( UINT64 un64EcuName);
    // Checks if the node Address is a duplicate
    BOOL bIsDuplicateAddress(BYTE byAddress);
    // Get node ptr for the given simulated system name
    PSNODEINFO psGetSimSysNodePointer(CString omStrNodeName);
    // Deletes node
    BOOL bDeleteNodeFromSimSys(CString omStrNodeToBeDeleted);


    // Get all node names whose dlls are loaded
    // Validates if there is any C File associated with the sim sys to build them
    BOOL bIsBuildAllNodesValid();
    // Validates if there is any unloaded DLL associated with the sim sys to load them
    BOOL bIsEnableAllNodesValid();
    // Validates if there is any loaded DLL associated with the sim sys to load them
    BOOL bIsDisableAllNodesValid();
    // releases the multiple message structure pointer that is passed..
    void vReleaseNodeList();

    void vAddNodeToList(PSNODELIST pNode);
    PSNODELIST m_psNodesList;
    UINT m_unNumberOfNodesAdded;    // Number of Nodes added
private:
    //This data member will contain pointer to sSIMSYSINFO structure that holds active simulated system information.
    ETYPE_BUS m_eBus;
};
