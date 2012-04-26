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
 * \file      ProjConfig.cpp
 * \brief     This contains project configuration class implementation.
 * \authors   Anish Kumar, Ratnadip Choudhury
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * This contains project configuration class implementation.
 * Implementation of the ProjConfig class.
 */

#include "StdAfx_ProjectConfiguration.h"
#include "ProjectConfiguration_extern.h"
#include "ProjConfig.h"

/**
 * \brief Construction
 */
CProjConfig::CProjConfig()
{
    vClearMap();
}

/**
 * \brief Destruction
 */
CProjConfig::~CProjConfig()
{
	vClearMap();
}

void CProjConfig::vClearMap(void)
{
	for (SECTIONMAP::iterator i = m_MapOfSection.begin(); i != m_MapOfSection.end(); ++i)
	{
		delete i->second;
		i->second = NULL;
	}
    m_MapOfSection.clear();
}

// Getters

int CProjConfig::GetSectionCount()
{
    return m_MapOfSection.size();
}

void CProjConfig::GetProjectDetail(PROJECTDATA &ProjDATA)
{
    ProjDATA = m_sProjectDetails;
}

int CProjConfig::GetSectionList(list<string>& SectionList)
{
    SectionList.empty();
	for (SECTIONMAP::iterator i = m_MapOfSection.begin(); i != m_MapOfSection.end(); ++i)
	{
        SectionList.push_front(i->first);
	}

    return GetSectionCount();
}

bool CProjConfig::GetSectionData(string SectionName, SECTIONDATA &SectionData)
{
    bool bResult = false;

    SECTIONMAP::const_iterator i;
	i = m_MapOfSection.find(SectionName);
    if (i != m_MapOfSection.end())
    {
        SectionData = *(i->second);
        bResult = true;
    }
	else
	{
		//MessageBox(NULL, "not found", "", MB_OK);
	}
    return bResult;
}


// Setters

bool CProjConfig::AddModifySectionDetail(const SECTIONDATA& SectionData)
{
    bool bAdded = false;

	SECTIONMAP::iterator i;
	i = m_MapOfSection.find(SectionData.m_omSectionName);
    if (i != m_MapOfSection.end())
    {
        *(i->second) = SectionData;
    }
    else
    {
		SECTIONDATA* pNewSection = new SECTIONDATA;
		*pNewSection = SectionData;
        m_MapOfSection.insert(SECTIONMAP::value_type(SectionData.m_omSectionName, 
                                                     pNewSection));
        bAdded = true;
    }

    return bAdded;
}

void CProjConfig::ModifyProjValues(const PROJECTDATA& ProjDATA)
{
    m_sProjectDetails = ProjDATA;
}
