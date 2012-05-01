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
 * \file      Replay.cpp
 * \author    Ratnadip Choudhury
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 */
// Replay.cpp : Defines the initialization routines for the DLL.
//

#include "Replay_stdafx.h"
#include <afxdllx.h>
#include "Include/Basedefs.h"

#define USAGE_EXPORT
#include "Replay_Extern.h"

#include "ReplayManager.h"

static AFX_EXTENSION_MODULE ReplayDLL = { NULL, NULL };

extern "C" int APIENTRY
DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
    // Remove this if you use lpReserved
    UNREFERENCED_PARAMETER(lpReserved);

    if (dwReason == DLL_PROCESS_ATTACH)
    {
        TRACE0("Replay.DLL Initializing!\n");

        // Extension DLL one-time initialization
        if (!AfxInitExtensionModule(ReplayDLL, hInstance))
        {
            return 0;
        }

        // Insert this DLL into the resource chain
        // NOTE: If this Extension DLL is being implicitly linked to by
        //  an MFC Regular DLL (such as an ActiveX Control)
        //  instead of an MFC application, then you will want to
        //  remove this line from DllMain and put it in a separate
        //  function exported from this Extension DLL.  The Regular DLL
        //  that uses this Extension DLL should then explicitly call that
        //  function to initialize this Extension DLL.  Otherwise,
        //  the CDynLinkLibrary object will not be attached to the
        //  Regular DLL's resource chain, and serious problems will
        //  result.
        new CDynLinkLibrary(ReplayDLL);
    }
    else if (dwReason == DLL_PROCESS_DETACH)
    {
        TRACE0("Replay.DLL Terminating!\n");
        // Terminate the library before destructors are called
        AfxTermExtensionModule(ReplayDLL);
    }

    return 1;   // ok
}

USAGEMODE void vREP_DisplayReplayConfigDlg(ETYPE_BUS eType, const void* pvFilterConfigured)
{
    //Place this code at the beginning of the export function.
    //Save previous resource handle and switch to current one.
    HINSTANCE hInst = AfxGetResourceHandle();
    AfxSetResourceHandle(ReplayDLL.hResource);

    switch(eType)
    {
        case CAN:
        {
            CReplayManager::ouGetReplayManager().
            dShowReplayConfigurationDlg((SFILTERAPPLIED_CAN*)pvFilterConfigured);
        }
        break;

        default:
        {
            ASSERT(FALSE);
        }
        break;
    }

    //Place this at the end of the export function.
    //switch back to previous resource handle.
    AfxSetResourceHandle(hInst);
}

USAGEMODE void vREP_CmdGo()
{
    CReplayManager::ouGetReplayManager().vCmdGo();
}
USAGEMODE void vREP_CmdStop()
{
    CReplayManager::ouGetReplayManager().vCmdStop();
}

USAGEMODE void vREP_CmdStep()
{
    CReplayManager::ouGetReplayManager().vCmdStep();
}

USAGEMODE void vREP_CmdSkip()
{
    CReplayManager::ouGetReplayManager().vCmdSkip();
}

//Load and save the Replay config details in terms of bytes
USAGEMODE void vREP_GetReplayConfigData(BYTE*& pDesBuffer, int& nBuffSize)
{
    CReplayManager::ouGetReplayManager().vGetReplayConfigData(pDesBuffer, nBuffSize);
}

USAGEMODE void vREP_SetReplayConfigData(BYTE* pSrcBuffer, int nBuffSize)
{
    CReplayManager::ouGetReplayManager().vSetReplayConfigData(pSrcBuffer, nBuffSize);
}

USAGEMODE void vREP_InitReplay(PVOID pvObjects, eREPLAY_INIT eUpdateType)
{
    if (pvObjects != NULL)
    {
        switch (eUpdateType)
        {
            case eINIT_TRACE:
            {
                CReplayManager::ouGetReplayManager().vSetTraceObjPtr(pvObjects);
            }
            break;

            case eINIT_ALL:
            {
            }
            break;
        }
    }
}

USAGEMODE BOOL bREP_GetUIStateCmdGo()
{
    return CReplayManager::ouGetReplayManager().bGetUIStateCmdGo();
}

USAGEMODE BOOL bREP_GetUIStateCmdSkip()
{
    return CReplayManager::ouGetReplayManager().bGetUIStateCmdSkip();
}

USAGEMODE BOOL bREP_GetUIStateCmdStep()
{
    return CReplayManager::ouGetReplayManager().bGetUIStateCmdStep();
}

USAGEMODE BOOL bREP_GetUIStateCmdStop()
{
    return CReplayManager::ouGetReplayManager().bGetUIStateCmdStop();
}
USAGEMODE void vREP_GetReplayFileNameList(CStringArray& omRepalyFiles)
{
    CReplayManager::ouGetReplayManager().vGetReplayFileNameList(omRepalyFiles);
}

USAGEMODE void vREP_HandleConnectionStatusChange(BOOL bStatus)
{
    CReplayManager::ouGetReplayManager().vHandleConnectionStatusChange(bStatus);
}

USAGEMODE void vREP_SetThreadStopSignal()
{
    CReplayManager::ouGetReplayManager().vSetThreadStopSignal();
}

USAGEMODE void vREP_StopReplayThread()
{
    CReplayManager::ouGetReplayManager().vStopReplayThread();
}
USAGEMODE void vREP_SetConfiguredFilter(const void* pvFilterConfigured)
{
    CReplayManager::ouGetReplayManager().vSetConfiguredFilter(pvFilterConfigured);
}

