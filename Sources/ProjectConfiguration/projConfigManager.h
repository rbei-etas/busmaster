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
 * \file      projConfigManager.h
 * \brief     Contains project configuration manager class definition.
 * \authors   Anish Kumar, Ratnadip Choudhury
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Contains project configuration manager class definition.
 */

#pragma once

#include <list>
#include <map>
#include <string>

class CProjConfig;
class PROJECTDATA;
class SECTIONDATA;

typedef std::map<std::string, CProjConfig> PROJECTMAP;

class CProjConfigManager
{

private:

    PROJECTMAP m_MapOfProject;

    bool GetProjectConfig(std::string ProjectName, CProjConfig*& ProjConfig);

public:

    // Constructor and destructor
    CProjConfigManager();
    virtual ~CProjConfigManager();

    // Getters
    int GetProjectCount();
    int GetProjectList(std::list<std::string>& ProjectList);
    bool GetProjectData(std::string ProjectName, PROJECTDATA& ProjData);
    int GetSectionCount(std::string ProjectName);
    int GetSectionList(std::string ProjectName, std::list<std::string>& SectionList);
    bool GetSectionData(std::string ProjectName, std::string SectionName, SECTIONDATA& Sectiondata);

    // Setters
    void AddModifyProjDetail(const PROJECTDATA& ProjData);
    bool AddModifySection(std::string ProjectName, const SECTIONDATA& SectionData);
    void DeleteProjectTable(std::string ProjectName);
    void DeleteAllProjectTable(void);
};
