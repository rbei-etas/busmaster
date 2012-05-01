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
 * \file      Network_McNet.h
 * \brief     This contains definitions of some classes/ structures
 * \author    Ratnadip Choudhury
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * This contains definitions of some classes/ structures
 */

#pragma once

#include "AfxTempl.h"
#include "DataTypes/McNet_Datatypes.h"
#import <msxml6.dll>
using namespace MSXML2;

const DWORD LCN_INVALID_VALUE = (DWORD)-1;
const DWORD CANID_INVALID_VALUE = (DWORD)-1;
#define LCN_INVALID_NAME        _T("")


typedef enum eTYPE_MCNET_CONN
{
    CONNVAL_INVALID = 0,
    CONNVAL_MAIN,
    CONNVAL_SUB

} TYPE_MCNET_CONN;

typedef struct tagMCNET_LOGCOMP
{
    DWORD   m_dwLCN;
    CString m_omLCName;

    tagMCNET_LOGCOMP();
    ~tagMCNET_LOGCOMP();
    tagMCNET_LOGCOMP& operator=(const tagMCNET_LOGCOMP& RefObj);

    CString DEBUG_VALUES(void);

} S_MCNET_LOGCOMP;

typedef CArray<S_MCNET_LOGCOMP, S_MCNET_LOGCOMP&> CArrayLCN;

typedef struct tagMCNET_CONN
{
    TYPE_MCNET_CONN m_eType;
    DWORD           m_dwCANID;
    S_MCNET_LOGCOMP m_sLocalLC;
    S_MCNET_LOGCOMP m_sRemoteLC;
    CString         m_omSlave;

    tagMCNET_CONN();
    ~tagMCNET_CONN();
    tagMCNET_CONN& operator=(const tagMCNET_CONN& RefObj);
    DWORD dwGetSlaveLCN(void) const;
    void vSetConnValue(CString omLocal, DWORD dwID, CString omRemote,
                       BOOL bLocalMas, TYPE_MCNET_CONN eType, BOOL bAbsVal);

    CString DEBUG_VALUES(void);

} S_MCNET_CONNBASIC;

typedef CArray<S_MCNET_CONNBASIC, S_MCNET_CONNBASIC&> CBasicConnArray;

typedef struct tagMainConn
{
    CString         m_omMaster;
    BYTE            m_byMasterNibble;
    BYTE            m_bySlaveNibble;
    CStringArray    m_omSlaves;

    tagMainConn();
    ~tagMainConn();
    tagMainConn& operator=(const tagMainConn& RefObj);
    void vClear(void);

} S_MAIN_CONN_DETAILS;

typedef CArray<S_MAIN_CONN_DETAILS, S_MAIN_CONN_DETAILS&> CMainConnList;

typedef struct tagSubConnDetail
{
    CString m_omMaster;
    CString m_omSlave;
    DWORD   m_dwCANID_MS;
    DWORD   m_dwCANID_SM;
} S_DETAILS_SUBCONN;

class CNetwork_McNet
{
private:
    CBasicConnArray m_odBasicConnList;

    void vInitAll(void);
    void vClearAll(void);
    void ReadANode(BYTE bType, MSXML2::IXMLDOMNodePtr pomNode, void* pvBuf = NULL) const;
    bool bReadAttribute(MSXML2::IXMLDOMNode* Node, CString& Str, _bstr_t tmp) const;
    DWORD dwGetLCN(CMcNetMessageMap& omMsgMap, CString omNode) const;

public:
    CNetwork_McNet();              // Constructor
    ~CNetwork_McNet();             // Destructor

    // Call this function to load the assembly information file
    HRESULT hLoadAssemblyFile(CString omFilePath, CString& omErrStr);

    // Given the CAN id, this function returns the associated LCs.
    BOOL bGetLCNValues(DWORD dwCANID, DWORD& LocalLC, DWORD& RemoteLC) const;

    // Given the local & remote LCs, this function returns the CAN id.
    BOOL bGetCANIDfromLCs(DWORD LocalLC, DWORD RemoteLC, DWORD& dwCANID) const;

    // To get a map between the CAN id and string depicting the direction
    BOOL bFormulateRules(CMcNetMessageMap& odMapMcNet) const;

    // To get the basic connection list
    void vGetBasicConnList(CBasicConnArray& odConnListTgt) const;

    // To set the basic connection list
    void vSetBasicConnList(const CNetwork_McNet& ouNetwork);

    // Given a LC, this returns the associated connections where the current
    // LC is either local or remote
    BOOL bGetConnectionList(DWORD CurrLC, BOOL bIsLocal,
                            CBasicConnArray& odConnListTgt) const;

    // This will return array of the different LCs
    BOOL bGetArrayLC(CArrayLCN& odArrayLC) const;
    // Call this to check presence of the LCN
    BOOL bIsNodePresent(DWORD dwLCN) const;
};
