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
 * \file      ProjConfig_DataTypes.cpp
 * \brief     Source file for project configuration data types.
 * \author    Ratnadip Choudhury
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Source file for project configuration data types.
 */


// Insert your headers here
#include "DataTypes_StdAfx.h"

#include "ProjConfig_DataTypes.h"

/* STARTS IMPLEMENTATION OF FILESTORAGEINFO */

FILESTORAGEINFO::FILESTORAGEINFO()
{
    memset(m_FilePath, '\0', _MAX_PATH);
}
/* ENDS IMPLEMENTATION OF FILESTORAGEINFO */


/* STARTS IMPLEMENTATION OF PROJECTDATA */

/******************************************************************************
  Function Name    :  Initialise
  Input(s)         :  void
  Output           :  void
  Functionality    :  Does initialisation.
  Member of        :  PROJECTDATA
  Friend of        :  -
  Author(s)        :  Ratnadip Choudhury
  Date Created     :  1.12.2009
  Modification date:
  Modification By  :
******************************************************************************/
void PROJECTDATA::Initialise(void)
{
    SYSTEMTIME CurrSysTime;
    GetSystemTime(&CurrSysTime);
    m_ProjectName   = "";
    m_Language      = "eng";
    memcpy(&m_ProjSysTime, &CurrSysTime, sizeof (SYSTEMTIME));
    m_fAppVersion = 0;
    m_dwAppUniqueId = 0;
};

/******************************************************************************
  Function Name    :  PROJECTDATA
  Input(s)         :  -
  Output           :  -
  Functionality    :  Standard constructor
  Member of        :  PROJECTDATA
  Friend of        :  -
  Author(s)        :  Ratnadip Choudhury
  Date Created     :  1.12.2009
  Modification date:
  Modification By  :
******************************************************************************/
PROJECTDATA::PROJECTDATA()
{
    Initialise();
}

/******************************************************************************
  Function Name    :  ~PROJECTDATA
  Input(s)         :  -
  Output           :  -
  Functionality    :  Destructor
  Member of        :  PROJECTDATA
  Friend of        :  -
  Author(s)        :  Ratnadip Choudhury
  Date Created     :  1.12.2009
  Modification date:
  Modification By  :
******************************************************************************/
PROJECTDATA::~PROJECTDATA()
{
}

/******************************************************************************
  Function Name    :  operator=
  Input(s)         :  PROJECTDATA - The soure object
  Output           :  PROJECTDATA& - The current object, the modified one.
  Functionality    :  Equal operator overloaded.
  Member of        :  PROJECTDATA
  Friend of        :  -
  Author(s)        :  Ratnadip Choudhury
  Date Created     :  1.12.2009
  Modification date:
  Modification By  :
******************************************************************************/
PROJECTDATA& PROJECTDATA::operator=(const PROJECTDATA& RefObj)
{
    m_ProjectName = RefObj.m_ProjectName;
    m_Language    = RefObj.m_Language;
    m_fAppVersion = RefObj.m_fAppVersion;
    m_dwAppUniqueId = RefObj.m_dwAppUniqueId;
    memcpy(&m_ProjSysTime, &RefObj.m_ProjSysTime, sizeof (SYSTEMTIME));
    return *this;
}

/******************************************************************************
  Function Name    :  Write
  Input(s)         :  pFile - The target file pointer.
  Output           :  true if all the operations are successful, else false.
  Functionality    :  Writes the current object data into the target file.
  Member of        :  PROJECTDATA
  Friend of        :  -
  Author(s)        :  Ratnadip Choudhury
  Date Created     :  1.12.2009
  Modification date:
  Modification By  :
******************************************************************************/
bool PROJECTDATA::Write(FILE* pFile)
{
    bool bResult = false;
    /* Below is shown the order in which project data are saved:
    1. Length of the project name [int]
    2. Project name [character stream]
    3. Length of language name [int]
    4. Language name [character stream]
    5. System time and date [SYSTEMTIME, byte stream]
    6. Application version number [float]
    7. Application identifier [DWORD] */
    const char* Buffer = m_ProjectName.c_str();
    int n =  (int) strlen(Buffer);

    if (fwrite(&n, sizeof(int), 1, pFile) == 1)
    {
        if ((int) fwrite(Buffer, sizeof(char), n, pFile) == n)
        {
            Buffer = m_Language.c_str();
            n =  (int) strlen(Buffer);

            if (fwrite(&n, sizeof(int), 1, pFile) == 1)
            {
                if ((int) fwrite(Buffer, sizeof(char), n, pFile) == n)
                {
                    if (fwrite(&m_ProjSysTime, sizeof(SYSTEMTIME), 1, pFile) == 1)
                    {
                        if (fwrite(&m_fAppVersion, sizeof(float), 1, pFile) == 1)
                        {
                            if (fwrite(&m_dwAppUniqueId, sizeof(DWORD), 1, pFile) == 1)
                            {
                                bResult = true;
                            }
                        }
                    }
                }
            }
        }
    }

    return bResult;
}

/******************************************************************************
  Function Name    :  Read
  Input(s)         :  pFile - The source file pointer.
  Output           :  true if all the operations are successful, else false.
  Functionality    :  Assuming the current file pointer points to a project
                      data entry, this function retrieves project data entry
                      and initialises the current object with the information.
  Member of        :  PROJECTDATA
  Friend of        :  -
  Author(s)        :  Ratnadip Choudhury
  Date Created     :  1.12.2009
  Modification date:
  Modification By  :
******************************************************************************/
bool PROJECTDATA::Read(FILE* pFile)
{
    Initialise();
    bool bResult = false;
    char Buffer[_MAX_PATH] = {'\0'};
    int n;

    // For obvious reason reading takes place in the same order as writing.
    if (fread(&n, sizeof(int), 1, pFile) == 1)
    {
        if (fread(Buffer, sizeof(char) * n, 1, pFile) != NULL)
        {
            m_ProjectName = Buffer;

            if (fread(&n, sizeof(int), 1, pFile) == 1)
            {
                if (fread(Buffer, sizeof(char) * n, 1, pFile) != NULL)
                {
                    m_Language = Buffer;

                    if (fread(&m_ProjSysTime, sizeof(SYSTEMTIME), 1, pFile) == 1)
                    {
                        if (fread(&m_fAppVersion, sizeof(float), 1, pFile) == 1)
                        {
                            if (fread(&m_dwAppUniqueId, sizeof(DWORD), 1, pFile) == 1)
                            {
                                bResult = true;
                            }
                        }
                    }
                }
            }
        }
    }

    return bResult;
}
/* ENDS IMPLEMENTATION OF PROJECTDATA */


/* STARTS IMPLEMENTATION OF SECTIONDATA */

/******************************************************************************
  Function Name    :  SECTIONDATA
  Input(s)         :  -
  Output           :  -
  Functionality    :  Standard constructor
  Member of        :  SECTIONDATA
  Friend of        :  -
  Author(s)        :  Ratnadip Choudhury
  Date Created     :  1.12.2009
  Modification date:
  Modification By  :
******************************************************************************/
SECTIONDATA::SECTIONDATA()
{
    m_bBLOB = NULL;
    m_nBLOBLen = 0;
    m_omSectionName = "";
}

/******************************************************************************
  Function Name    :  ~SECTIONDATA
  Input(s)         :  -
  Output           :  -
  Functionality    :  Destructor
  Member of        :  SECTIONDATA
  Friend of        :  -
  Author(s)        :  Ratnadip Choudhury
  Date Created     :  1.12.2009
  Modification date:
  Modification By  :
******************************************************************************/
SECTIONDATA::~SECTIONDATA()
{
    Initialise();
}

/******************************************************************************
  Function Name    :  Initialise
  Input(s)         :  void
  Output           :  void
  Functionality    :  Does initialisation
  Member of        :  SECTIONDATA
  Friend of        :  -
  Author(s)        :  Ratnadip Choudhury
  Date Created     :  1.12.2009
  Modification date:
  Modification By  :
******************************************************************************/
void SECTIONDATA::Initialise(void)
{
    if (NULL != m_bBLOB)
    {
        delete[] m_bBLOB;
        m_bBLOB = NULL;
    }

    m_nBLOBLen = 0;
    m_omSectionName = "";
}

/******************************************************************************
  Function Name    :  operator=
  Input(s)         :  SECTIONDATA - The soure object
  Output           :  SECTIONDATA& - The current object, the modified one.
  Functionality    :  Equal operator overloaded.
  Member of        :  SECTIONDATA
  Friend of        :  -
  Author(s)        :  Ratnadip Choudhury
  Date Created     :  1.12.2009
  Modification date:
  Modification By  :
******************************************************************************/
SECTIONDATA& SECTIONDATA::operator=(const SECTIONDATA& RefObj)
{
    Initialise();
    m_bBLOB = new BYTE[RefObj.m_nBLOBLen];

    if (m_bBLOB != NULL)
    {
        m_nBLOBLen = RefObj.m_nBLOBLen;
        memcpy(m_bBLOB, RefObj.m_bBLOB, RefObj.m_nBLOBLen);
        m_omSectionName = RefObj.m_omSectionName;
    }

    return *this;
}

/******************************************************************************
  Function Name    :  Write
  Input(s)         :  pFile - The target file pointer.
  Output           :  true if all the operations are successful, else false.
  Functionality    :  Writes the current object data into the target file.
  Member of        :  SECTIONDATA
  Friend of        :  -
  Author(s)        :  Ratnadip Choudhury
  Date Created     :  1.12.2009
  Modification date:
  Modification By  :
******************************************************************************/
bool SECTIONDATA::Write(FILE* pFile)
{
    bool bResult = false;
    /* Below is shown the order in which section data are saved:
    1. Length of the section name [int]
    2. Section name [character stream]
    3. Length of BLOB [int]
    4. BLOB (section data) [byte stream] */
    const char* Buffer = m_omSectionName.c_str();
    int n = (int) strlen(Buffer);

    if (fwrite(&n, sizeof(int), 1, pFile) == 1)
    {
        if ((int) fwrite(Buffer, sizeof(char), n, pFile) == n)
        {
            if (fwrite(&m_nBLOBLen, sizeof(m_nBLOBLen), 1, pFile) == 1)
            {
                if (m_bBLOB != NULL)
                {
                    if ((int) fwrite(m_bBLOB, 1, m_nBLOBLen, pFile) == m_nBLOBLen)
                    {
                        bResult = true;
                    }
                }
            }
        }
    }

    return bResult;
}

/******************************************************************************
  Function Name    :  Read
  Input(s)         :  pFile - The source file pointer.
  Output           :  true if all the operations are successful, else false.
  Functionality    :  Assuming the current file pointer points to a section
                      entry, this function retrieves section entry and
                      initialises the current object with the information.
  Member of        :  SECTIONDATA
  Friend of        :  -
  Author(s)        :  Ratnadip Choudhury
  Date Created     :  1.12.2009
  Modification date:
  Modification By  :
******************************************************************************/
bool SECTIONDATA::Read(FILE* pFile)
{
    Initialise();
    bool bResult = false;
    char Buffer[_MAX_PATH] = {'\0'};
    int n;

    if (fread(&n, sizeof(int), 1, pFile) == 1)
    {
        if (fread(Buffer, sizeof(char) * n, 1,pFile) != NULL)
        {
            m_omSectionName = Buffer;

            if (fread(&m_nBLOBLen, sizeof(m_nBLOBLen), 1, pFile) == 1)
            {
                m_bBLOB = new BYTE[m_nBLOBLen];

                if (m_bBLOB != NULL)
                {
                    if ((int) fread(m_bBLOB, 1, m_nBLOBLen, pFile) == m_nBLOBLen)
                    {
                        bResult = true;
                    }
                }
            }
        }
    }

    return bResult;
}
/* ENDS IMPLEMENTATION OF SECTIONDATA */
