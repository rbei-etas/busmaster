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
 */

// projConfigManager.cpp: implementation of the CProjConfigManager class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx_ProjectConfiguration.h"
#include "ProjectConfiguration_extern.h"
#include "ProjConfig.h"
#include "projConfigManager.h"
#include "Application/StdAfx.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CProjConfigManager::CProjConfigManager()
{
    m_MapOfProject.clear();
}

CProjConfigManager::~CProjConfigManager()
{
    // ENSURE DESTRUCTOR OF CProjConfig IS CALLED
    m_MapOfProject.clear();
}


bool CProjConfigManager::GetProjectConfig(std::string ProjectName, CProjConfig*& ProjConfig)
{
    bool bResult = false;
    PROJECTMAP::iterator i = m_MapOfProject.begin();
    if (i != m_MapOfProject.end())
    {
        bResult = true;
        ProjConfig = &(i->second);
    }
    return bResult;
}

// Getters

int CProjConfigManager::GetProjectCount()
{
    return m_MapOfProject.size();
}

int CProjConfigManager::GetProjectList(std::list<std::string>& ProjectList)
{
    ProjectList.clear();

    for (PROJECTMAP::iterator i = m_MapOfProject.begin(); i != m_MapOfProject.end(); ++i)
    {
        ProjectList.push_front(i->first);
    }

    return GetProjectCount();
}

bool CProjConfigManager::GetProjectData(std::string ProjName, PROJECTDATA& ProjData)
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

int CProjConfigManager::GetSectionCount(std::string ProjectName)
{
    int nResult = 0;

    CProjConfig* ProjConfig = nullptr;
    if (GetProjectConfig(ProjectName, ProjConfig))
    {
        nResult = ProjConfig->GetSectionCount();
    }

    return nResult;
}

int CProjConfigManager::GetSectionList(std::string ProjectName, std::list<std::string>& SectionList)
{
    int nResult = 0;

    CProjConfig* ProjConfig = nullptr;
    if (GetProjectConfig(ProjectName, ProjConfig))
    {
        nResult = ProjConfig->GetSectionList(SectionList);
    }

    return nResult;
}

bool CProjConfigManager::GetSectionData(std::string ProjectName, std::string SectionName,
                                        SECTIONDATA& Sectiondata)
{
    bool bResult = false;

    CProjConfig* ProjConfig = nullptr;
    if (GetProjectConfig(ProjectName, ProjConfig))
    {
        bResult = ProjConfig->GetSectionData(SectionName, Sectiondata);
    }
    return bResult;
}


// Setters

void CProjConfigManager::AddModifyProjDetail(const PROJECTDATA& NewProjData)
{
    CProjConfig* pProjConfig = nullptr;
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

bool CProjConfigManager::AddModifySection(std::string ProjectName,
        const SECTIONDATA& SectionData)
{
    bool bResult = false;

    CProjConfig* ProjConfig = nullptr;
    if (GetProjectConfig(ProjectName, ProjConfig))
    {
        bResult = ProjConfig->AddModifySectionDetail(SectionData);
    }

    return bResult;
}

void CProjConfigManager::DeleteProjectTable(std::string ProjectName)
{
    m_MapOfProject.erase(ProjectName);
}

void CProjConfigManager::DeleteAllProjectTable(void)
{
    m_MapOfProject.clear();
}
