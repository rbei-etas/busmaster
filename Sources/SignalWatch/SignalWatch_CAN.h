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
 * \file      SignalWatch_CAN.h
 * \author    Ratnadip Choudhury
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 */

#pragma once

#include "SignalWatch_Resource.h"
#include "Datatypes/MsgBufAll_Datatypes.h"
#include "Datatypes/SigWatch_Datatypes.h"
#include "SignalWatchDefs.h"
#include "DataTypes/MainSubEntry.h"
#include "SigWatchAddDelDlg.h"
#include "SigWatchDlg.h"
/* MSG INTERPRETATION */
#include "Utility/MsgInterpretation.h"
#include "Utility/Utility_Thread.h"
/* DIL CAN INTERFACE */
#include "DIL_Interface/DIL_Interface_extern.h"
#include "DIL_Interface/BaseDIL_CAN.h"

#include "BaseSignalWatch_CAN.h"

class CSignalWatch_CAN : CBaseSignalWatch_CAN
{
public:
    CCANBufFSE m_ouCanBufFSE;

private:
    BOOL m_bHex;
    CSigWatchDlg* m_pouSigWnd;
    CMsgInterpretation* m_pMsgInterPretObj;
    CPARAM_THREADPROC m_ouReadThread;
    CRITICAL_SECTION m_omCritSecSW;

public:
    BOOL InitInstance(void);
    int ExitInstance(void);

    HRESULT SW_DoInitialization(void);
    HRESULT SW_ShowAddDelSignalsDlg(CWnd* pParent, CMainEntryList* podMainSubList);
    HRESULT SW_ShowSigWatchWnd(CWnd* pParent, INT nCmd);
    HRESULT SW_SetDisplayMode(BOOL bHex);
    HRESULT SW_GetConfigSize(void);
    HRESULT SW_GetConfigData(void* pbyConfigData);
    HRESULT SW_SetConfigData(const void* pbyConfigData);
    HRESULT SW_ClearSigWatchWnd(void);
    HRESULT SW_UpdateMsgInterpretObj(void* RefObj);
    BOOL    SW_IsWindowVisible(void);
    void vDisplayInSigWatchWnd(STCANDATA& sCanData);

private:
    BOOL bStartSigWatchReadThread(void);

};
