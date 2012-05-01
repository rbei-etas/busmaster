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
 * \file      TestSetupHeader.cpp
 * \author    Venkatanarayana makam
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 */
#include "TestSetupEditorLib_stdafx.h"
#include "TestSetupHeader.h"

/******************************************************************************
Function Name  :  CTestSetupHeader
Input(s)       :
Output         :  -
Functionality  :
Member of      :  CTestSetupHeader
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  06/04/2011
Modifications  :
******************************************************************************/
CTestSetupHeader::CTestSetupHeader(void)
{
    InitHeaderToDefault();
}

/******************************************************************************
Function Name  :  ~CTestSetupHeader
Input(s)       :  -
Output         :  -
Functionality  :  Destructor
Member of      :  CTestSetupHeader
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  06/04/2011
Modifications  :
******************************************************************************/
CTestSetupHeader::~CTestSetupHeader(void)
{
}
/******************************************************************************
Function Name  :  InitHeaderToDefault
Input(s)       :  -
Output         :  HRESULT
Functionality  :  Constructor
Member of      :  CTestSetupHeader
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  06/04/2011
Modifications  :  CS024
******************************************************************************/
HRESULT CTestSetupHeader::InitHeaderToDefault(void)
{
    m_eBus = CAN;
    m_omDatabasePath = "";
    m_sEngineerInfo1.vInitialise(_T("Engineer Name"));
    m_sEngineerInfo2.vInitialise(_T("Engineer Role"));
    m_sModuleName.vInitialise(_T("Module Name"), _T(""));
    m_sReportFile.vInitialise();
    m_sVersion.vInitialise(_T("version"), _T("1.0"));
    return S_OK;
}

/******************************************************************************
Function Name  :  ValidateEntity
Input(s)       :
Output         :  HRESULT
Functionality  :
Member of      :  CTestSetupHeader
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  14/04/2011
Modifications  :
Code Tag       :  CS023
******************************************************************************/
HRESULT CTestSetupHeader::ValidateEntity(CString& omStrResult)
{
    HRESULT hResult = ERR_VALID_SUCCESS;

    if(m_omDatabasePath == _T(""))
    {
        hResult = ERR_VALID_ERROR;
        omStrResult.Format(_T("No Database File is Included\r\n"));
    }
    else
    {
        if(PathFileExists(m_omDatabasePath) == FALSE)
        {
            omStrResult.Format(_T("Incorrect Database File path\r\n"));
            hResult = ERR_VALID_ERROR;
        }
    }

    return hResult;
}
