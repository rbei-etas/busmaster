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
 * \brief     This contains the dll entry point function and defines the entry point for the DLL application.
 * \authors   Anish Kumar, Ratnadip Choudhury
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * This contains the dll entry point function and defines the entry point for the DLL application.
 */

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

//
//  Note!
//
//      If this DLL is dynamically linked against the MFC
//      DLLs, any functions exported from this DLL which
//      call into MFC must have the AFX_MANAGE_STATE macro
//      added at the very beginning of the function.
//
//      For example:
//
//      extern "C" BOOL PASCAL EXPORT ExportedFunction()
//      {
//          AFX_MANAGE_STATE(AfxGetStaticModuleState());
//          // normal function body here
//      }
//
//      It is very important that this macro appear in each
//      function, prior to any calls into MFC.  This means that
//      it must appear as the first statement within the
//      function, even before any object variable declarations
//      as their constructors may generate calls into the MFC
//      DLL.
//
//      Please see MFC Technical Notes 33 and 58 for additional
//      details.
//

// CProjectConfigurationApp

BEGIN_MESSAGE_MAP(CProjectConfigurationApp, CWinApp)
END_MESSAGE_MAP()

/**
 * CProjectConfigurationApp construction
 */
CProjectConfigurationApp::CProjectConfigurationApp()
{
    // TODO: add construction code here,
    // Place all significant initialization in InitInstance
}

/**
 * The one and only CProjectConfigurationApp object
 */
CProjectConfigurationApp theApp;

/**
 * CProjectConfigurationApp initialization
 */
BOOL CProjectConfigurationApp::InitInstance()
{
    CWinApp::InitInstance();
    return TRUE;
}

/**
 * \brief Adds a project entry in the project table or modifies an existing one
 * \req RS_06_06 - Adds a project entry in the project table or modifies an existing one
 *
 * Adds a project entry in the project table or modifies an existing one.
 * This is a project table related function.
 */
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

/**
 * \brief Deletes the project entry from the project table
 * \req RS_06_07 - Deletes the project entry from the project table
 *
 * Deletes the project entry from the project table.
 * This is a project table related function.
 */
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

/**
 * \brief Get total number of projects in the project table
 * \req RS_06_04 - Get total number of projects in the project table
 *
 * Get total number of projects in the project table.
 * This is a project table related function.
 */
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

/**
 * \brief Get project name list from the project table
 * \req RS_06_05 - Get project name list from the project table
 *
 * Get project name list from the project table.
 * This is a project table related function.
 */
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

/**
 * \brief Retrieves project data from the project table
 * \req RS_06_08 - Retrieves project data from the project table
 *
 * Retrieves project data from the project table.
 * This is a project table related function.
 */
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

/**
 * \brief Add a section or modify an existing one in the section table of the project
 * \req RS_06_09 - Add a section or modify an existing one in the section table of the project
 *
 * Add a section or modify an existing one in the section table of the project.
 * This is a section data related function.
 */
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

/**
 * \brief Gets information of that particular section from the section table of the project
 * \req RS_06_11 - Gets information of that particular section from the section table of the project
 *
 * Gets information of that particular section from the section table of the project.
 * This is a section data related function.
 */
USAGEMODE BOOL GetSectionData(string& ProjectName, string& SectionName, SECTIONDATA& SectionData)
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

/**
 * \brief Receives total number of sections from the section table of the project
 * \req RS_06_12 - Receives total number of sections from the section table of the project
 *
 * Receives total number of sections from the section table of the project.
 * This is a section data related function.
 */
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

/**
 * \brief Retrieves list of all the section names from the section table of the project
 * \req RS_06_13 - Retrieves list of all the section names from the section table of the project
 *
 * Retrieves list of all the section names from the section table of the project.
 * This is a section data related function.
 */
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

/**
 * \brief Set the present data storage configuration
 * \req RS_06_01 - Set the present data storage configuration
 *
 * Set the present data storage configuration.
 * This is a data storage related function.
 */
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

/**
 * \brief Get the presently selected data storage configuration
 * \req RS_06_02 - Get the presently selected data storage configuration
 *
 * Get the presently selected data storage configuration.
 * This is a data storage related function.
 */
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

/**
 * \brief Perform data storage operation after selection
 * \param[in] operation is OPEN, SAVE or CLOSE
 * \req RS_06_03 - Perform data storage operation after selection
 *
 * Perform data storage operation after selection.
 * This is a data storage related function.
 */
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
