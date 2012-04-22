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
 * \file      TestSetupHeader.h
 * \author    Venkatanarayana makam
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 */

#pragma once

#include "TSDefinitions.h"
#include "Include/BaseDefs.h"
class CTestSetupHeader
{
public:
    //Bus Type    
    eTYPE_BUS m_eBus;
	//Database file path
	CString m_omDatabasePath;
	//Name of the engineer
	SInfo m_sEngineerInfo1;
	//Engineer's role / designation
	SInfo m_sEngineerInfo2;
	//Name of the test setup
	SInfo m_sModuleName;
    //Report File Path
	SReportFile m_sReportFile;
	//Version information
	SInfo m_sVersion;

//Member Functions
public:
	CTestSetupHeader(void);
	HRESULT InitHeaderToDefault(void);
    HRESULT ValidateEntity(CString& omStrResult);
    virtual ~CTestSetupHeader(void);
};
