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
 * \file      ProjConfig.h
 * \brief     This contains project configuration class definition.
 * \authors   Anish Kumar, Ratnadip Choudhury
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * This contains project configuration class definition.
 */

// ProjConfig.h: interface for the ProjConfig class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PROJCONFIG_H__B9B4B9B5_BA2F_47AC_98D9_8933DB6DFC55__INCLUDED_)
#define AFX_PROJCONFIG_H__B9B4B9B5_BA2F_47AC_98D9_8933DB6DFC55__INCLUDED_


#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


typedef std::map <string, SECTIONDATA*> SECTIONMAP;


class CProjConfig  
{
private:

    PROJECTDATA m_sProjectDetails;
	SECTIONMAP m_MapOfSection;

	void vClearMap(void);

public:

    // Constructor and destructor
	CProjConfig();
	virtual ~CProjConfig();

    // All the getters
	int GetSectionCount();
	void GetProjectDetail(PROJECTDATA &ProjData);
	int GetSectionList(list<string>& SectionList);
	bool GetSectionData(string SectionName, SECTIONDATA &SectionData);

    // All the setters
	bool AddModifySectionDetail(const SECTIONDATA& SectionData);
	void ModifyProjValues(const PROJECTDATA& ProjDATA);
};

#endif // !defined(AFX_PROJCONFIG_H__B9B4B9B5_BA2F_47AC_98D9_8933DB6DFC55__INCLUDED_)
