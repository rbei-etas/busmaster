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
 * \file      ConfigData.h
 * \author    Ratnadip Choudhury
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 */

#pragma once

#pragma pack(8)
//#pragma pack(show)

#include "Utility/ComputeCheckSum.h"
#include "DataTypes/ProjConfig_DataTypes.h"
//#include "ProjectConfiguration\ProjectConfiguration_extern.h"


class CConfigData : public CComputeCheckSum
{
private:
	CString m_omStrCurrFileName;
	string m_strCurrProjName;
	static CConfigData m_ouConfigDetails;
	CConfigData(void);
public:
	static CConfigData& ouGetConfigDetailsObject();
	~CConfigData(void);
	
	int SetConfigDatastorage(DATASTORAGEINFO* StorageInfo);
	int GetConfigDatastorage(DATASTORAGEINFO* StorageInfo);

	void vReadConfigFile();
	void vSaveConfigFile();
	void vCloseConfigFile();

	void vSetCurrProjName(string strCurrProjName);
	string GetCurrProjName();
	BOOL bSetCurrProjInfo(PROJECTDATA* ProjData);
	BOOL bGetCurrProjInfo(PROJECTDATA* ProjData);

	BOOL bSetData(LPVOID lpVoid, int nStreamLength, string strSectionName);
	BOOL bGetData(void *& lpData,int &nStreamLength, string strSectionName);
};
