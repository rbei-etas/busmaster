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
#include "Utility\MultiLanguageSupport.h"
//#include "../Application/GettextBusmaster.h"

CTestSetupHeader& CTestSetupHeader::operator= (const CTestSetupHeader& src)
{
    //Bus Type
    this->m_eBus = src.m_eBus;
    //Name of the engineer
    this->m_sEngineerInfo1 = src.m_sEngineerInfo1;
    //Engineer's role / designation
    this->m_sEngineerInfo2 = src.m_sEngineerInfo2;
    //Name of the test setup
    this->m_sModuleName = src.m_sModuleName;
    //Report File Path
    this->m_sReportFile = src.m_sReportFile;
    //Version information
    this->m_sVersion = src.m_sVersion;

    //Clean up databasefile paths
    this->m_omDatabasePaths.RemoveAll();
 
    //Store all new paths
    for (int i = 0; i < src.m_omDatabasePaths.GetSize(); i++)
    {
        this->m_omDatabasePaths.Add(src.m_omDatabasePaths.GetAt(i));
    }
    return *this;
}

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
    m_omDatabasePaths.RemoveAll();
    m_sEngineerInfo1.vInitialise(_("Engineer Name"));
    m_sEngineerInfo2.vInitialise(_("Engineer Role"));
    m_sModuleName.vInitialise(_("Module Name"), "");
    m_sReportFile.vInitialise();
    m_sVersion.vInitialise(_("version"), "1.0");
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

    if(m_omDatabasePaths.GetSize() < 1)
    {
        hResult = ERR_VALID_ERROR;
        omStrResult = _("No Database File is Included");
        omStrResult += "\r\n";
    }
    else
    {
        for(int i = 0; i < m_omDatabasePaths.GetSize(); i++)
        {
            if(PathFileExists(m_omDatabasePaths[i]) == FALSE)
            {
                omStrResult = _("Incorrect Database File path");
                omStrResult += "\r\n";
                hResult = ERR_VALID_ERROR;
            }
        }
    }
    return hResult;
}
