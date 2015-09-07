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
 * \file      CAN_VSCOM.h
 * \brief     Exports API functions for MHS-Elektronik Tiny-CAN Hardware interface
 * \author
 * \copyright Copyright (c) 2012, ETAS GmbH. All rights reserved.
 *
 */
#pragma once

#ifndef __AFXWIN_H__
#error include 'stdafx.h' before including this file for PCH
#endif

#include "EXTERNAL/vs_can_api.h"


struct _VSCanCfg
{
    VSCAN_HANDLE hCan;
    char szLocation[MAX_PATH + 1];
    DWORD dwMode;
    void* vSpeed;
    VSCAN_BTR btr;
    VSCAN_CODE_MASK codeMask;
    BOOL bDualFilter;
    BOOL bTimestamps;
    BOOL bDebug;
};

int WINAPI ShowCanVsComSetup(HWND hwnd, struct _VSCanCfg* cfg);

/**
 * See CAN_VSCOM.cpp for the implementation of this class
 */
class CCAN_VSCOM : public CWinApp
{
public:
    CCAN_VSCOM();

    // Overrides
public:
    virtual BOOL InitInstance();

    DECLARE_MESSAGE_MAP()
};
