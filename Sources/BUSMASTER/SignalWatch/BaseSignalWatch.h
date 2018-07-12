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
 * \file      BaseSignalWatch.h
 * \author    Ratnadip Choudhury
 * \Modified  venkatanarayana Makam - Made CBaseSignalWatch_CAN as base class for every protocol
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 */

#pragma once
#include "IBMNetWorkGetService.h"
#include <libxml/parser.h>
class CBaseSignalWatch
{
public:
    virtual HRESULT SW_DoInitialization( IBMNetWorkGetService* info) = 0;
    virtual HRESULT SW_ShowAddDelSignalsDlg(CWnd* pParent, void* info) = 0;
    virtual HRESULT SW_ShowSigWatchWnd(CWnd* pParent, HWND hMainWnd, INT nCmd) = 0;
    virtual HRESULT SW_GetConfigData(xmlNodePtr pNodePtr) = 0;
    virtual HRESULT SW_SetConfigData(xmlNodePtr pNode) = 0;
    virtual HRESULT SW_ClearSigWatchWnd(void) = 0;
    virtual BOOL    SW_IsWindowVisible(void) = 0;
    virtual HRESULT SW_SetDisplayMode(BOOL bHex) = 0;
    virtual HRESULT SW_SetClusterInfo(void* ouCluster) = 0;
};
