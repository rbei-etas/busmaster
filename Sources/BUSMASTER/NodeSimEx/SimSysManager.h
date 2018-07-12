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
 * \file      SimSysManager.h
 * \author    Ratnadip Choudhury
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 */
#pragma once

#include "SimSysNodeInfo.h"
#include "SimSysTreeView.h"
#include "HashDefines.h"
#include "Utility/XMLUtils.h"
#include "IBMNetWorkGetService.h"

class CSimSysManager
{

protected:
    CSimSysManager(ETYPE_BUS eBus, CGlobalObj* pGlobalObj, CSimSysNodeInfo* pSimSysNodeInfo);
    CSimSysTreeView* m_pomSimSysTreeView;

private:
    ETYPE_BUS m_eBus;

    static CSimSysManager* sm_pouSimSysManager[BUS_TOTAL];

    // Pointer to Simulated system List data structure
    PSSIMSYSARRAY m_psSimSysArray;
    // Pointer to CSimSysTreeView object

    void vLoadNodeInfoFromConfiguration(xmlNodePtr pNode);
    virtual sNODEINFO* vCreateBusSpecificNodeInfoPtr();
    virtual void vAddNodeToBusSpecificNodeList(PSNODELIST psNodeList);
protected:
    //This is only CSimSysNodeInfo object created to keep track of
    //Current selected node's properties
    CSimSysNodeInfo m_ouSimSysNodeInfo;

public:
    ~CSimSysManager(void);
    static CSimSysManager& ouGetSimSysManager(ETYPE_BUS eBus, CGlobalObj* pGlobalObj);
    static void vClearObj(ETYPE_BUS eBus);
    //This function is returning pointer to the only created CSimSysNodeInfo
    //object which is private variable so it can be changed later
    virtual CSimSysNodeInfo* pomGetSimSysNodeInfo();
    void pouSetSimSysNodeInfo(CSimSysNodeInfo*);
    //Set the SimSysTreeView object
    void podSetSimSysTreeView(CSimSysTreeView* );
    // Get pointer to CSimSysTreeView class
    CSimSysTreeView* podGetSimSysTreeView();
    BOOL m_bFromAutomation;
    //Load and store related functions
    UINT unGetStoreSIMFBufferSize();
    void SaveSIMDataIntoBuffer(BYTE* DesBuffer);
    void vSaveSimSysWndConfig();
    virtual bool bGetConfigData(xmlNodePtr pNodePtr);
    void vLoadSimSysWndConfig();
    void CopySIMDataFromBuffer(BYTE* SrcBuffer);
    void vLoadSimSysWndConfig(xmlDocPtr, ETYPE_BUS eBus);
    virtual void CopySIMDataFromBuffer(xmlDocPtr, ETYPE_BUS eBus);

    HRESULT GenerateMakeFile(std::string strCppFile);
    HRESULT GenerateMakeFile();

    virtual void vSetConfigData(xmlNodePtr pNode);
    xmlNodePtr m_CopyJ1939SimNode;
    BOOL bIsConfigChanged();
    virtual CSimSysNodeInfo vGetBusSpecificSimSysNodeInfo();


    void vSetDatabaseConfiguration(IBMNetWorkGetService* ouLINConfig, bool bModified = false);
    void bUpdateNodeInfoFile(PSNODELIST psNodeList);
    virtual void vInitailizeSimSysInfo();
    //Patch up for loading the simsys configuration
    CStringArray m_omSimsysInConfigFile;

    CGlobalObj* m_pGlobalObj;
};
