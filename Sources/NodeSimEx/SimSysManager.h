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
#include "SimSysDetView.h"
#include "SimSysConfigDetails.h"
#include "SimSysTreeView.h"
#include "HashDefines.h"
#include "Flags.h"


class CSimSysManager
{
private:
    ETYPE_BUS m_eBus;
    CSimSysManager(ETYPE_BUS eBus);
    static CSimSysManager* sm_pouSimSysManager[BUS_TOTAL];
    //This is only CSimSysNodeInfo object created to keep track of
    //Current selected node's properties
    CSimSysNodeInfo m_ouSimSysNodeInfo;
    // Pointer to Simulated system List data structure
    PSSIMSYSARRAY m_psSimSysArray;
    //Keeps the only detView object ptr
    CSimSysDetView* m_pomSimSysDetView;
    // Simulated system config objectpointer
    CSimSysConfigDetails m_omSimSysConfig;
    // Pointer to CSimSysTreeView object
    CSimSysTreeView* m_pomSimSysTreeView;
    //POinter to Flags object
    CFlags m_ouFlags;
    //Add the file details while loading new configuration
    void vLoadSimInfoFromConfiguration(CString omFileName);
public:
    ~CSimSysManager(void);
    static CSimSysManager& ouGetSimSysManager(ETYPE_BUS eBus);
    static void vClearObj(ETYPE_BUS eBus);
    //This function is returning pointer to the only created CSimSysNodeInfo
    //object which is private variable so it can be changed later
    CSimSysNodeInfo* pomGetSimSysNodeInfo();
    //Return
    PSSIMSYSINFO psReturnSimsysInfoPtr();
    //Set the det view object
    void podSetSimSysDetView(CSimSysDetView* pDetView);
    // Get pointer to CSimSysDetView class
    CSimSysDetView* podGetSimSysDetView();

    // Get pointer to CSimSysConfigDetails class
    CSimSysConfigDetails* pomGetSimSysConfig();

    //Set the SimSysTreeView object
    void podSetSimSysTreeView(CSimSysTreeView* );
    // Get pointer to CSimSysTreeView class
    CSimSysTreeView* podGetSimSysTreeView();
    BOOL m_bFromAutomation;

    //Get the flags object pointer
    CFlags& ouGetFlags();
    //Load and store related functions
    UINT unGetStoreSIMFBufferSize();
    void SaveSIMDataIntoBuffer(BYTE* DesBuffer);
    void vSaveSimSysWndConfig();
    void vLoadSimSysWndConfig();
    void CopySIMDataFromBuffer(BYTE* SrcBuffer);
    BOOL bIsConfigChanged();

    void vApplicationClosing();
    void vSaveAllSimSys();
    void vInitailizeSimSysInfo();

    //Patch up for loading the simsys configuration
    CStringArray m_omSimsysInConfigFile;
};
