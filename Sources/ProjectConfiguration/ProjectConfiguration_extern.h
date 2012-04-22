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
 * \file      ProjectConfiguration_extern.h
 * \brief     This contains exported function declarations, that can be
 * \authors   Anish Kumar, Ratnadip Choudhury
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * This contains exported function declarations, that can be
 */

#pragma once

#if defined USAGEMODE
#undef USAGEMODE
#endif

#if defined USAGE_EXPORT
#define USAGEMODE   __declspec(dllexport)
#else
#define USAGEMODE   __declspec(dllimport)
#endif

#include "DataTypes/ProjConfig_DataTypes.h"

#ifdef __cplusplus
extern "C" {  // only need to export C interface if used by C++ source code
#endif


USAGEMODE int SetDatastorageConfig(DATASTORAGEINFO* StorageInfo);
USAGEMODE int GetDatastorageConfig(DATASTORAGEINFO* StorageInfo);
//return type's definition for DoDatastorageOperation
/*
    0             NoError
	1             problem in opening file
   -1             file not present
    2             problem in writing/reading file
	3             Invalid file
	4             Unknown error
*/  
USAGEMODE int DoDatastorageOperation(short);/*OPEN = 0x1,SAVE = 0x2,CLOSE = 0x4*/


USAGEMODE int AddModifyProjectTable(string& ProjectName, PROJECTDATA* ProjData);
USAGEMODE int DeleteProjectTable(string& ProjectName);
USAGEMODE int GetProjectCount(void);
USAGEMODE int GetProjectList(LISTSTR* ProjList);
USAGEMODE int GetProjectData(string& ProjName, PROJECTDATA& ProjData);

USAGEMODE int AddModifySectionData(string& ProjectName, string& SectionName, SECTIONDATA* SectionData);
USAGEMODE int DeleteSectionData(string& ProjectName, SECTIONDATA* SectionData);
USAGEMODE BOOL GetSectionData(string& ProjectName, string& SectionName, SECTIONDATA& SectionData);
USAGEMODE int GetSectionCount(string& ProjectName);
USAGEMODE int GetSectionList(string& ProjectName, LISTSTR* SectionList);


#ifdef __cplusplus
}
#endif
