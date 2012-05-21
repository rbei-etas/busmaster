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
 * \file      BaseSignalWatch_J1939.h
 * \author    Ratnadip Choudhury
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 */

#pragma once

class CBaseSignalWatch_J1939
{
public:
    virtual HRESULT SW_DoInitialization(void) = 0;
    virtual HRESULT SW_ShowAddDelSignalsDlg(CWnd* pParent, CMainEntryList* podMainSubList) = 0;
    virtual HRESULT SW_ShowSigWatchWnd(CWnd* pParent, INT nCmd) = 0;
    virtual HRESULT SW_GetConfigSize(void) = 0;
    virtual HRESULT SW_GetConfigData(void* pbyConfigData) = 0;
    virtual HRESULT SW_SetConfigData(const void* pbyConfigData) = 0;
    virtual HRESULT SW_ClearSigWatchWnd(void) = 0;
    virtual HRESULT SW_UpdateMsgInterpretObj(void* RefObj) = 0;
    virtual BOOL    SW_IsWindowVisible(void) = 0;
    virtual HRESULT SW_SetDisplayMode(BOOL bHex) = 0;
};
