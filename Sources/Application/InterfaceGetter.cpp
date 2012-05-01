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
#include "DIL_Interface/DIL_Interface_extern.h"
// For CAN logger interface
#include "FrameProcessor/BaseFrameProcessor_CAN.h"
#include "FrameProcessor/BaseFrameProcessor_J1939.h"
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

extern CCANMonitorApp theApp;


CBaseFrameProcessor_CAN* GetICANLogger(void)
{
    CBaseFrameProcessor_CAN* Result = NULL;

    if (FP_GetInterface(FRAMEPROC_CAN, (void**) &Result) == S_OK)
    {
        // Nothing to do at this moment
    }

    ASSERT(NULL != Result);
    return Result;
}

CBaseFrameProcessor_J1939* GetIJ1939Logger(void)
{
    CBaseFrameProcessor_J1939* Result = NULL;

    if (FP_GetInterface(FRAMEPROC_J1939, (void**) &Result) == S_OK)
    {
        // Nothing to do at this moment
    }

    ASSERT(NULL != Result);
    return Result;
}

CMainFrame* GetIMainFrame(void)
{
    CMainFrame* Result = static_cast<CMainFrame*> (theApp.m_pMainWnd);
    ASSERT(NULL != Result);
    return Result;
}

CFlags* GetIFlags(void)
{
    CFlags* Result = (theApp.pouGetFlagsPtr());
    ASSERT(NULL != Result);
    return Result;
}

CBaseDIL_CAN* GetICANDIL(void)
{
    CBaseDIL_CAN* Result = NULL;

    if (DIL_GetInterface(CAN, (void**) &Result) == S_OK)
    {
        ASSERT(NULL != Result);
    }
    else
    {
        ASSERT(FALSE);
    }

    return Result;
}

CBaseDILI_J1939* GetIJ1939DIL(void)
{
    CBaseDILI_J1939* Result = NULL;

    if (DIL_GetInterface(J1939, (void**) &Result) == S_OK)
    {
        ASSERT(NULL != Result);
    }
    else
    {
        ASSERT(FALSE);
    }

    return Result;
}

CBaseBusStatisticCAN* GetICANBusStat(void)
{
    CBaseBusStatisticCAN* Result = NULL;

    if (BS_GetInterface(CAN, (void**) &Result) == S_OK)
    {
        ASSERT(NULL != Result);
    }
    else
    {
        ASSERT(FALSE);
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
    CBaseNodeSim* Result = NULL;

    if (NS_GetInterface(CAN, (void**) &Result) == S_OK)
    {
        // Nothing to do at this moment
    }

    ASSERT(NULL != Result);
    return Result;
}

CBaseNodeSim* GetIJ1939NodeSim(void)
{
    CBaseNodeSim* Result = NULL;

    if (NS_GetInterface(J1939, (void**) &Result) == S_OK)
    {
        // Nothing to do at this moment
    }

    ASSERT(NULL != Result);
    return Result;
}