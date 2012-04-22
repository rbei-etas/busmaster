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
 * \file      SimSysConfigDetails.h
 * \brief     This header file contains the definition of the class
 * \author    Ratnadip Choudhury
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * This header file contains the definition of the class
 */

#pragma once

#include "Utility/ComputeCheckSum.h"
#include "SimSysNodeInfo.h"

class CSimSysConfigDetails : public CComputeCheckSum  
{
public:
	CSimSysConfigDetails(ETYPE_BUS eBus);
	virtual ~CSimSysConfigDetails();
    // loads a configuration info from a file that is already present..
    INT     nLoadConfiguration(CString& omStrFilename,PSSIMSYSINFO &psSimSys);
    // writes the configuration info into the file omStrCfgFilename
    INT     nSaveConfiguration (CString omStrCfgFilename, PSSIMSYSINFO &psSimSys);
		
	
private:
    ETYPE_BUS m_eBus;
	// version information
    float m_fSimSysVersion;		
	// To Initialise simulated system details after configuration load/create
	// method to search and open/create the configuration file.
    INT nIsCfgFileFound (CString omStrFilename);
			
	// method to desrialize/serialize data into the file/data members
    int nLoadStoreData(UINT unArchiveMode , CString omStrSimSysPath,
                       PSSIMSYSINFO &psCurrSimSys);
	
	// called from bLoadStoreData(..) for simulated system info 
    BOOL bLoadStoreSimSysInfo(CArchive& roCfgArchive, PSSIMSYSINFO &psSimsysInfo);

		
};
