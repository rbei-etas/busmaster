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
 * \file      ProjConfig_DataTypes.h
 * \brief     Definition file for project configuration data types.
 * \author    Ratnadip Choudhury
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Definition file for project configuration data types.
 */

#pragma once

using namespace std;

enum {OPEN = 0x1, SAVE = 0x2, CLOSE = 0x4};

enum {FILEMODE = 0, DATABASEMODE};

typedef list<string> LISTSTR;;

class FILESTORAGEINFO
{
public:
    char m_FilePath[_MAX_PATH];
    FILESTORAGEINFO();
};

class DBSTORAGEINFO
{
};

typedef struct tagDATASTORAGEINFO
{
    short m_Datastore; // Value should be one between FILEMODE or DATABASEMODE
    union
    {
        FILESTORAGEINFO* FSInfo;
        DBSTORAGEINFO* DBSInfo;
    };

} DATASTORAGEINFO;

/* The following class is defined to represent a project entry completely 
without the data (or section). This is used as a structure and hence all the 
members are declared public. */
class PROJECTDATA
{
public:
    // DATA MEMBERS
    string m_ProjectName;   // The necessary and sufficient information for a 
    string m_Language;      // project entry is name (signature and hence it 
	SYSTEMTIME m_ProjSysTime;// has to be unique), the language (expressed in
	float m_fAppVersion;    // ISO 639-3 code), time and date of last saving,
	DWORD m_dwAppUniqueId;  // application version and unique identifier.

    // MEMBER FUNCTIONS
    PROJECTDATA();          // Standard constructor
	~PROJECTDATA();         // Destructor

    // This clears the project entry wiping out all the data.
    void Initialise(void);

    // Equal operator overloaded
    PROJECTDATA& operator=(const PROJECTDATA& RefObj);

    // To write project entry into a file. Advances the file pointer afterwards.
    bool Write(FILE* pFile);

    // To read project entry from a file. This assumes the file pointer is cur-
    // rently pointing to a project entry. Advances the file pointer afterwards.
    bool Read(FILE* pFile);
};

// This class is defined to represent a section completely.
class SECTIONDATA
{
public:
    string m_omSectionName; // The necessary and sufficient information for a
    int m_nBLOBLen;         // section are the section name and the data in 
    BYTE* m_bBLOB;          // byte stream that it should contain.

    SECTIONDATA();          // Standard constructor
    ~SECTIONDATA();         // Destructor

    // This clears the section wiping out all the data.
    void Initialise(void);

    // Equal operator overloaded
    SECTIONDATA& operator=(const SECTIONDATA& RefObj);

    // To write section data into a file. Advances the file pointer afterwards.
    bool Write(FILE* pFile);

    // To read section data from a file. This assumes the file pointer is 
    // currently pointing to a section. Advances the file pointer afterwards.
    bool Read(FILE* pFile);
};
