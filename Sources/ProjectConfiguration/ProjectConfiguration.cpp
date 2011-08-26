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
 * \file      ProjectConfiguration.cpp
 * \brief     This contains the dll entry point function and 
 * \authors   Anish Kumar, Ratnadip Choudhury
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * This contains the dll entry point function and 
 */

// ProjectConfiguration.cpp : Defines the entry point for the DLL application.
//

#include "StdAfx_ProjectConfiguration.h"
#include "ProjectConfiguration.h"
#include "include/utils_macro.h"

#define USAGE_EXPORT
#include "ProjectConfiguration_extern.h"

#include "operations_file.h"


// {410BD867-1AD9-4492-9DA4-8175CCB7147B}
static const GUID guidProjectConfig = 
{ 
    0x410bd867, 0x1ad9, 0x4492, { 0x9d, 0xa4, 0x81, 0x75, 0xcc, 0xb7, 0x14, 0x7b } 
};


static DATASTORAGEINFO sdataStorageInfo;


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//
//	Note!
//
//		If this DLL is dynamically linked against the MFC
//		DLLs, any functions exported from this DLL which
//		call into MFC must have the AFX_MANAGE_STATE macro
//		added at the very beginning of the function.
//
//		For example:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// normal function body here
//		}
//
//		It is very important that this macro appear in each
//		function, prior to any calls into MFC.  This means that
//		it must appear as the first statement within the 
//		function, even before any object variable declarations
//		as their constructors may generate calls into the MFC
//		DLL.
//
//		Please see MFC Technical Notes 33 and 58 for additional
//		details.
//

// CProjectConfigurationApp

BEGIN_MESSAGE_MAP(CProjectConfigurationApp, CWinApp)
END_MESSAGE_MAP()


// CProjectConfigurationApp construction

CProjectConfigurationApp::CProjectConfigurationApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}


// The one and only CProjectConfigurationApp object

CProjectConfigurationApp theApp;


// CProjectConfigurationApp initialization

BOOL CProjectConfigurationApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}

/*  PROJECT TABLE RELATED FUNCTIONS */


USAGEMODE int AddModifyProjectTable(string& ProjectName, PROJECTDATA* ProjData)
{
	int nReturn = 0;
	if(sdataStorageInfo.m_Datastore == FILEMODE)
	{
		FileAddModifyProjectTable(ProjectName, *ProjData);
	}
	else
	{
	}
	return nReturn;
}

USAGEMODE int DeleteProjectTable(string& ProjectName)
{
	int nReturn = 0;
	if(sdataStorageInfo.m_Datastore == FILEMODE)
	{
		nReturn = FileDeleteProjectTable( ProjectName);
	}
	else
	{
	}
	return nReturn;
}

// Getters
USAGEMODE int GetProjectCount(void)
{
	int nReturn = 0;
	if(sdataStorageInfo.m_Datastore == FILEMODE)
	{
		nReturn = FileGetProjectCount();
	}
	else
	{
	}
	return nReturn;
}

USAGEMODE int GetProjectList(LISTSTR* ProjList)
{
	int nReturn = 0;
	if(sdataStorageInfo.m_Datastore == FILEMODE)
	{
		nReturn = FileGetProjectList(*ProjList);
	}
	else
	{
	}
	return nReturn;
}

USAGEMODE int GetProjectData(string& ProjName, PROJECTDATA& ProjData)
{
	int nReturn = 0;
	if(sdataStorageInfo.m_Datastore == FILEMODE)
	{
		nReturn = FileGetProjectData(ProjName, ProjData);
	}
	else
	{
	}
	return nReturn;
}

/*  PROJECT TABLE RELATED FUNCTIONS */

USAGEMODE int AddModifySectionData(string& ProjectName, string& SectionName, SECTIONDATA* SectionData)
{
	int nReturn = 0;
	if(sdataStorageInfo.m_Datastore == FILEMODE)
	{
		if (SectionData != NULL)
		{
			nReturn = FileAddModifySectionData(ProjectName,SectionName, *SectionData);
		}
	}
	else
	{
	}
	return nReturn;
}

USAGEMODE BOOL GetSectionData(string& ProjectName, string& SectionName, SECTIONDATA &SectionData)
{
	BOOL bReturn = 0;
	if(sdataStorageInfo.m_Datastore == FILEMODE)
	{
		bReturn = FileGetSectionData(ProjectName, SectionName, SectionData);
	}
	else
	{
	}
	return bReturn;
}

USAGEMODE int GetSectionCount(string& ProjectName)
{
	int nReturn = 0;
	if(sdataStorageInfo.m_Datastore == FILEMODE)
	{
		nReturn = FileGetSectionCount(ProjectName);
	}
	else
	{
	}
	return nReturn;
}

USAGEMODE int GetSectionList(string& ProjectName, LISTSTR* SectionList)
{
	int nReturn = 0;
	if(sdataStorageInfo.m_Datastore == FILEMODE)
	{
		nReturn = FileGetSectionList(ProjectName, *SectionList);
	}
	else
	{
	}
	return nReturn;
}

// Basic operations

USAGEMODE int SetDatastorageConfig(DATASTORAGEINFO* StorageInfo)
{
	sdataStorageInfo.m_Datastore = StorageInfo->m_Datastore ;
	if (sdataStorageInfo.m_Datastore == FILEMODE)
	{
		if (sdataStorageInfo.FSInfo == NULL)
		{
			sdataStorageInfo.FSInfo = new FILESTORAGEINFO; //Change
		}
		strcpy_s(sdataStorageInfo.FSInfo->m_FilePath,StorageInfo->FSInfo->m_FilePath);
	}
	else
	{
	
	}
	return 0;
}

USAGEMODE int GetDatastorageConfig(DATASTORAGEINFO* StorageInfo)
{
	StorageInfo->m_Datastore = sdataStorageInfo.m_Datastore;
	if (sdataStorageInfo.m_Datastore == FILEMODE)
	{
		if (sdataStorageInfo.FSInfo != NULL)
		{
			StorageInfo->FSInfo = new FILESTORAGEINFO;
			strcpy_s(StorageInfo->FSInfo->m_FilePath,sdataStorageInfo.FSInfo->m_FilePath);
		}
		else
		{
			StorageInfo->FSInfo = NULL;
		}
	}
	else
	{

	}
	return 0;
}

USAGEMODE int DoDatastorageOperation(short operation)
{
	int nReturn = 0;
    if (sdataStorageInfo.m_Datastore == FILEMODE)
    {
	    if (operation & SAVE)
	    {
		    nReturn = SaveDataFile(sdataStorageInfo.FSInfo->m_FilePath);
	    }
	    else if (operation & OPEN)
	    {
		    nReturn = LoadDataFile(sdataStorageInfo.FSInfo->m_FilePath);
	    }
		else if (operation & CLOSE)
		{
			CloseDataFile();
			if (sdataStorageInfo.FSInfo != NULL)
			{
				delete sdataStorageInfo.FSInfo;
				sdataStorageInfo.FSInfo = NULL;
			}
		}
    }
	return nReturn;
}
