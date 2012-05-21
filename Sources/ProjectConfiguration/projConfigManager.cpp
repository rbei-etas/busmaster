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
 * \file      projConfigManager.cpp
 * \brief     Contains project configuration manager class implementation.
 * \authors   Anish Kumar, Ratnadip Choudhury
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Contains project configuration manager class implementation.
 * Implementation of the CProjConfigManager class.
 */

#include "StdAfx_ProjectConfiguration.h"
#include "ProjectConfiguration_extern.h"
#include "ProjConfig.h"
#include "projConfigManager.h"

/**
 * \brief Constructor
 *
 * Construction
 */
CProjConfigManager::CProjConfigManager()
{
    m_MapOfProject.clear();
}

/**
 * \brief Destructor
 *
 * Destruction
 */
CProjConfigManager::~CProjConfigManager()
{
    // ENSURE DESTRUCTOR OF CProjConfig IS CALLED
    m_MapOfProject.clear();
}

/**
 * \brief Get Project Configuration
 *
 * Gets the project configuration.
 */
bool CProjConfigManager::GetProjectConfig(string ProjectName, CProjConfig *& ProjConfig)
{
    bool bResult = false;
    PROJECTMAP::iterator i = m_MapOfProject.begin();

    if (i != m_MapOfProject.end())
    {
        bResult = true;
        ProjConfig = &(i->second);
    }

    //else
    //{
    //  if (m_MapOfProject.size() > 0)
    //  {
    //      PROJECTDATA xyz;
    //      PROJECTMAP::iterator i1 = m_MapOfProject.begin();
    //      CProjConfig *ProjConfig = &(i1->second);
    //      ProjConfig->GetProjectDetail(xyz);
    //      string ab;//= xyz.m_ProjectName;
    //      ab = i1->first;
    //  }
    //}
    return bResult;
}

/**
 * \brief Get Project Count
 *
 * Gets the project count.
 */
int CProjConfigManager::GetProjectCount()
{
    return m_MapOfProject.size();
}

/**
 * \brief Get Project List
 *
 * Gets the project list.
 */
int CProjConfigManager::GetProjectList(list<string>& ProjectList)
{
    ProjectList.clear();

    for (PROJECTMAP::iterator i = m_MapOfProject.begin(); i != m_MapOfProject.end(); ++i)
    {
        ProjectList.push_front(i->first);
    }

    return GetProjectCount();
}

/**
 * \brief Get Project Data
 *
 * Gets the project data.
 */
bool CProjConfigManager::GetProjectData(string ProjName, PROJECTDATA& ProjData)
{
    bool bResult = false;
    PROJECTMAP::iterator i = m_MapOfProject.find(ProjName);
    bResult = (i != m_MapOfProject.end());

    if (bResult)
    {
        i->second.GetProjectDetail(ProjData);
    }

    return bResult;
}

/**
 * \brief Get Section Count
 *
 * Gets the section count.
 */
int CProjConfigManager::GetSectionCount(string ProjectName)
{
    int nResult = 0;
    CProjConfig* ProjConfig = NULL;

    if (GetProjectConfig(ProjectName, ProjConfig))
    {
        nResult = ProjConfig->GetSectionCount();
    }

    return nResult;
}

/**
 * \brief Get Section List
 *
 * Gets the section list.
 */
int CProjConfigManager::GetSectionList(string ProjectName, list<string>& SectionList)
{
    int nResult = 0;
    CProjConfig* ProjConfig = NULL;

    if (GetProjectConfig(ProjectName, ProjConfig))
    {
        nResult = ProjConfig->GetSectionList(SectionList);
    }

    return nResult;
}

/**
 * \brief Get Section Data
 *
 * Gets the section data.
 */
bool CProjConfigManager::GetSectionData(string ProjectName, string SectionName,
                                        SECTIONDATA& Sectiondata)
{
    bool bResult = false;
    CProjConfig* ProjConfig = NULL;

    if (GetProjectConfig(ProjectName, ProjConfig))
    {
        bResult = ProjConfig->GetSectionData(SectionName, Sectiondata);
    }

    return bResult;
}

/**
 * \brief Add/Modify Project Data
 *
 * Adds or modifies the project data.
 */
void CProjConfigManager::AddModifyProjDetail(const PROJECTDATA& NewProjData)
{
    CProjConfig* pProjConfig = NULL;

    if (GetProjectConfig(NewProjData.m_ProjectName, pProjConfig))
    {
        pProjConfig->ModifyProjValues(NewProjData);
    }
    else
    {
        CProjConfig NewProjConfig;
        NewProjConfig.ModifyProjValues(NewProjData);
        m_MapOfProject.insert(PROJECTMAP::value_type(NewProjData.m_ProjectName,
                              NewProjConfig));
    }
}

/**
 * \brief Add/Modify Section
 *
 * Adds or modifies the section.
 */
bool CProjConfigManager::AddModifySection(string ProjectName,
        const SECTIONDATA& SectionData)
{
    bool bResult = false;
    CProjConfig* ProjConfig = NULL;

    if (GetProjectConfig(ProjectName, ProjConfig))
    {
        bResult = ProjConfig->AddModifySectionDetail(SectionData);
    }

    return bResult;
}

/**
 * \brief Delete Project Table
 *
 * Deletes the project table.
 */
void CProjConfigManager::DeleteProjectTable(string ProjectName)
{
    m_MapOfProject.erase(ProjectName);
}

/**
 * \brief Delete All Project Table
 *
 * Deletes all project tables.
 */
void CProjConfigManager::DeleteAllProjectTable(void)
{
    m_MapOfProject.clear();
}
