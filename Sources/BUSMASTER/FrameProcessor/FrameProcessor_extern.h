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
 * \file      FrameProcessor_extern.h
 * \brief     Declaration file for the exported function used to get the
 * \author    Ratnadip Choudhury
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Declaration file for the exported function used to get the
 */

#pragma once

#include "../Include/ModuleID.h"

#if defined USAGEMODE
#undef USAGEMODE
#endif

#if defined USAGE_EXPORT
#define USAGEMODE   __declspec(dllexport)
#else
#define USAGEMODE   __declspec(dllimport)
#endif
#include "BaseFrameProcessor_CAN.h"
#include "BaseFrameProcessor_LIN.h"
#include "BaseFrameProcessor_J1939.h"

#ifdef __cplusplus
extern "C" {  // only need to export C interface if used by C++ source code
#endif

    /*  Exported function list */
    USAGEMODE HRESULT FP_GetInterface(eID_COMPONENT bInterfaceID, void** ppvInterface);
    USAGEMODE HRESULT FP_ReleaseInterface(eID_COMPONENT bInterfaceID);
    USAGEMODE HRESULT ConfigureCANLog(CBaseFrameProcessor_CAN* pouBaseLogger, BOOL& bLogOnConnect, CWnd* pParent = nullptr, const SFILTERAPPLIED_CAN* psFilter = nullptr);
    USAGEMODE HRESULT ConfigureLINLog(CBaseFrameProcessor_LIN* pouBaseLogger, BOOL& bLogOnConnect, CWnd* pParent = nullptr, const SFILTERAPPLIED_LIN* psFilter = nullptr);
    USAGEMODE HRESULT ConfigureJ1939Log(CBaseFrameProcessor_J1939* pouBaseLogger, BOOL& bLogOnConnect, CWnd* pParent = nullptr, const SFILTERAPPLIED_J1939* psFilter = nullptr);
#ifdef __cplusplus
}
#endif
