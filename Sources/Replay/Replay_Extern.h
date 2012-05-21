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
 * \file      Replay_Extern.h
 * \author    Ratnadip Choudhury
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 */

#pragma once

#include "Replay_Struct.h"

#if defined USAGEMODE
#undef USAGEMODE
#endif

#if defined USAGE_EXPORT
#define USAGEMODE   __declspec(dllexport)
#else
#define USAGEMODE   __declspec(dllimport)
#endif

#ifdef __cplusplus
extern "C" {  // only need to export C interface if used by C++ source code
#endif

    USAGEMODE void vREP_DisplayReplayConfigDlg(ETYPE_BUS eType, const void* pvFilterConfigured);
    USAGEMODE void vREP_CmdGo();
    USAGEMODE void vREP_CmdStop();
    USAGEMODE void vREP_CmdStep();
    USAGEMODE void vREP_CmdSkip();
    //Load and save the Replay config details in terms of bytes
    USAGEMODE void vREP_GetReplayConfigData(BYTE*& pDesBuffer, int& nBuffSize);
    USAGEMODE void vREP_SetReplayConfigData(BYTE* pSrcBuffer, int nBuffSize);
    //Set value for different objects in the module
    USAGEMODE void vREP_InitReplay(PVOID pvObjects, eREPLAY_INIT eUpdateType);

    USAGEMODE BOOL bREP_GetUIStateCmdGo();
    USAGEMODE BOOL bREP_GetUIStateCmdSkip();
    USAGEMODE BOOL bREP_GetUIStateCmdStep();
    USAGEMODE BOOL bREP_GetUIStateCmdStop();
    USAGEMODE void vREP_GetReplayFileNameList(CStringArray& omRepalyFiles);
    USAGEMODE void vREP_HandleConnectionStatusChange(BOOL bStatus);
    USAGEMODE void vREP_SetThreadStopSignal();
    USAGEMODE void vREP_StopReplayThread();
    USAGEMODE void vREP_SetConfiguredFilter(const void* pvFilterConfigured);

#ifdef __cplusplus
}
#endif
