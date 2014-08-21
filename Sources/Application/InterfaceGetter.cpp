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
 * \file      InterfaceGetter.cpp
 * \brief     Source file for global getter functions for interfaces.
 * \author    Ratnadip Choudhury
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Source file for global getter functions for interfaces.
 */


#include "StdAfx.h"
// For some essential data types
#include "DataTypes/MsgBufAll_Datatypes.h"
// For CAN DIL interface
#include "DIL_Interface/BaseDIL_CAN.h"
#include "DIL_Interface/BaseDIL_J1939.h"
#include "DIL_Interface/BaseDIL_LIN.h"
#include "DIL_Interface/DIL_Interface_extern.h"
// For CAN logger interface
#include "FrameProcessor/BaseFrameProcessor_CAN.h"
#include "FrameProcessor/BaseFrameProcessor_J1939.h"
#include "FrameProcessor/BaseFrameProcessor_LIN.h"
#include "FrameProcessor/FrameProcessor_extern.h"
// For Bus statistic interface
#include "BusStatistics.h"

//For node simulation interface
#include "NodeSimEx/BaseNodeSim.h"
#include "NodeSimEx/NodeSimEx_Extern.h"
// Application class definition
#include "BUSMASTER.h"
// Mainframe class definition
#include "MainFrm.h"
#include "InterfaceGetter.h"
extern CCANMonitorApp theApp;


CBaseFrameProcessor_CAN* GetICANLogger(void)
{
    CBaseFrameProcessor_CAN* Result = nullptr;
    if (FP_GetInterface(FRAMEPROC_CAN, (void**) &Result) == S_OK)
    {
        // Nothing to do at this moment
    }
    ASSERT(nullptr != Result);
    return Result;
}


CBaseFrameProcessor_LIN* GetILINLogger(void)
{
    CBaseFrameProcessor_LIN* Result = nullptr;
    if (FP_GetInterface(FRAMEPROC_LIN, (void**) &Result) == S_OK)
    {
        // Nothing to do at this moment
    }
    ASSERT(nullptr != Result);
    return Result;
}


CBaseFrameProcessor_J1939* GetIJ1939Logger(void)
{
    CBaseFrameProcessor_J1939* Result = nullptr;
    if (FP_GetInterface(FRAMEPROC_J1939, (void**) &Result) == S_OK)
    {
        // Nothing to do at this moment
    }
    ASSERT(nullptr != Result);
    return Result;
}

CMainFrame* GetIMainFrame(void)
{
    CMainFrame* Result = static_cast<CMainFrame*> (theApp.m_pMainWnd);
    ASSERT(nullptr != Result);
    return Result;
}

CFlags* GetIFlags(void)
{
    CFlags* Result = (theApp.pouGetFlagsPtr());
    ASSERT(nullptr != Result);
    return Result;
}

CBaseDIL_LIN* GetILINDIL(void)
{
    CBaseDIL_LIN* Result = nullptr;
    if (DIL_GetInterface(LIN, (void**) &Result) == S_OK)
    {
        ASSERT(nullptr != Result);
    }
    else
    {
        ASSERT(false);
    }
    return Result;
}



CBaseDIL_CAN* GetICANDIL(void)
{
    CBaseDIL_CAN* Result = nullptr;
    if (DIL_GetInterface(CAN, (void**) &Result) == S_OK)
    {
        ASSERT(nullptr != Result);
    }
    else
    {
        ASSERT(false);
    }
    return Result;
}

CBaseDILI_J1939* GetIJ1939DIL(void)
{
    CBaseDILI_J1939* Result = nullptr;
    if (DIL_GetInterface(J1939, (void**) &Result) == S_OK)
    {
        ASSERT(nullptr != Result);
    }
    else
    {
        ASSERT(false);
    }
    return Result;
}

CBaseBusStatisticCAN* GetICANBusStat(void)
{
    CBaseBusStatisticCAN* Result = nullptr;
    if (BS_GetInterface(CAN, (void**) &Result) == S_OK)
    {
        ASSERT(nullptr != Result);
    }
    else
    {
        ASSERT(false);
    }
    return Result;
}

CBaseBusStatisticLIN* GetILINBusStat(void)
{
    CBaseBusStatisticLIN* Result = nullptr;
    if (BS_GetInterface(LIN, (void**) &Result) == S_OK)
    {
        ASSERT(nullptr != Result);
    }
    else
    {
        ASSERT(false);
    }
    return Result;
}

CBaseBusStatisticEthernet* GetIEthernetBusStat(void)
{
    CBaseBusStatisticEthernet* Result = nullptr;
	if (BS_GetInterface(ETHERNET, (void**) &Result) == S_OK)
    {
        ASSERT(nullptr != Result);
    }
    else
    {
        ASSERT(FALSE);
    }
    return Result;
}

CFlexRayNetworkStats* GetIFlexRayBusStat(void)
{
    CFlexRayNetworkStats* Result = nullptr;
    if (BS_GetInterface(FLEXRAY, (void**) &Result) == S_OK)
    {
        ASSERT(nullptr != Result);
    }
    else
    {
        ASSERT(false);
    }
    return Result;
}

CMsgSignal* GetIMsgDB(void)
{
    return theApp.m_pouMsgSignal;
}

extern DWORD g_dwClientID;

DWORD dwGetMonitorClientID(void)
{
    return g_dwClientID;
}

CBaseNodeSim* GetICANNodeSim(void)
{
    CBaseNodeSim* Result = nullptr;
    if (NS_GetInterface(CAN, (void**) &Result) == S_OK)
    {
        // Nothing to do at this moment
    }
    ASSERT(nullptr != Result);
    return Result;
}
CBaseNodeSim* GetILINNodeSim(void)
{
    CBaseNodeSim* Result = nullptr;
    if (NS_GetInterface(LIN, (void**) &Result) == S_OK)
    {
        // Nothing to do at this moment
    }
    ASSERT(nullptr != Result);
    return Result;
}

CBaseNodeSim* GetIJ1939NodeSim(void)
{
    CBaseNodeSim* Result = nullptr;
    if (NS_GetInterface(J1939, (void**) &Result) == S_OK)
    {
        // Nothing to do at this moment
    }
    ASSERT(nullptr != Result);
    return Result;
}