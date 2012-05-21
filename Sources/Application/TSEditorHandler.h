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
 * \file      TSEditorHandler.h
 * \author    Pradeep Kadoor
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 */

#pragma once

#define def_STR_TESTSETUPEDITORDLL  _T("TestSetupEditorGUI.dll")
#include "TestSetupEditorGUI/TSEditorGUI_Extern.h"
class TSEditorHandler
{
private:
    HMODULE m_hTSEditorHandle;
    void vInitializeFuncPtrs();
    void vloadFuncPtrAddress();
public:
    TSEditorHandler(void);
    virtual ~TSEditorHandler(void);
    void vLoadTSEditor_DLL();
    void vShowTSEditorWindow(VOID* pParentWnd);
    void vLoadTestSetupFile(CString omFilePath);
    void vGetConfigurationData(BYTE*& pDesBuffer, UINT& unBuffSize);
    void vSetConfigurationData(BYTE*& pDesBuffer, UINT& unBuffSize);
    HWND vGetSafeHWnd();
};
