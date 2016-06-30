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
 * \file      operations_file.h
 * \brief     This file contains the codes when configuration procedure
 * \authors   Anish Kumar, Ratnadip Choudhury
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * This file contains the codes when configuration procedure
 */

#pragma once

#include <string>

// Project getters
int FileGetProjectCount(void);
int FileGetProjectList(LISTSTR& ProjectList);
bool FileGetProjectData(std::string ProjectName, PROJECTDATA& ProjData);

// Section getters
int FileGetSectionCount(std::string ProjectName);
int FileGetSectionList(std::string ProjectName, LISTSTR& SectionList);
bool FileGetSectionData(std::string ProjectName, std::string SectionName, SECTIONDATA& SectionData);

// Project setters
void FileAddModifyProjectTable(std::string ProjectName, PROJECTDATA& ProjData);
bool FileDeleteProjectTable(std::string ProjectName);

// Section setter
bool FileAddModifySectionData(std::string ProjectName, std::string SectionName,
                              SECTIONDATA& SectionData);

// File read
int LoadDataFile(char FileName[]);

void CloseDataFile();
// File write
int SaveDataFile(char FileName[]);

int GetTheErrorType(FILE* dbFile);
