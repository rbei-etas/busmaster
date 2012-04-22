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
 * \file      operations_file.cpp
 * \brief     This file contains the codes when configuration procedure 
 * \authors   Anish Kumar, Ratnadip Choudhury
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * This file contains the codes when configuration procedure 
 */


#include "StdAfx_ProjectConfiguration.h"
#include "ProjectConfiguration_extern.h"
#include "ProjConfig.h"
#include "projConfigManager.h"

#define DATABASE_SIGNATURE    "/************* FrameFileDB **************/"
#define VERSIONLINE           "/************* Version 1.0 **************/"
#define PROJECT_TABLE_SIG     "  PROJECT_TABLE :: PROJECT_TABLE "
#define DB_END_SIGNATURE      "/*********** FrameFileDB_END ************/"
#define LEN_SIG_FILE_START                      sizeof(DATABASE_SIGNATURE)
#define LEN_SIG_FILE_END                        sizeof(DB_END_SIGNATURE)
#define LEN_SIG_PROJ_TABLE                      sizeof(PROJECT_TABLE_SIG)
#define LEN_VERSION                             sizeof(VERSIONLINE)
#define OPENING_FILE_ERROR                      1
#define FILE_NOT_PRESENT                       -1
#define PROBLEM_IN_WRITING_READING              2
#define INVALID_FORMAT_FILE                     3
#define UNKNOWN_ERROR                           4
#define FILE_INVALID_SECTIONID                  5

enum 
{
    SECTION_SIGNATURE                   = 0x0,
    SECTION_VERSION,
    SECTION_PROJECT_TABLE_SIGNATURE,
    SECTION_PROJECT_TABLE_COUNT,
    SECTION_PROJECT_TABLE_ENTRY,
    SECTION_SECTION_TABLE_ENTRY,
    SECTION_SIGNATURE_FILE_END,
	SECTION_FILE_INVALID
};

static bool ReadAString(FILE* pFile, string& ResultStr)
{
    bool bResult = false;
	int nLength;
	//fseek(pFile, sizeof("\n")-1, SEEK_CUR);
	if (fread(&nLength, sizeof(int), 1, pFile) == 1)
	{
		CHAR* Buffer = NULL;
		try
		{
			Buffer = new CHAR[nLength];
			if (Buffer != NULL)
			{
				if (fread(Buffer, sizeof(char) * nLength, 1, pFile) != NULL)
				{
					ResultStr = Buffer;
					bResult = true;
				}
				delete[] Buffer;
                Buffer = NULL;
			}
		}
		catch (...)
		{
			//throw 0;
			//MessageBox(str, MB_ICONERROR| MB_SYSTEMMODAL|MB_OK, 0 );
		}
	}
	return bResult;
}


int GetTheErrorType(FILE *pFile)
{
    int nReturn = UNKNOWN_ERROR;
    if (feof(pFile))
    {
        nReturn = INVALID_FORMAT_FILE;
    }
    else if (ferror(pFile))
    {
        nReturn = PROBLEM_IN_WRITING_READING;
    }

    return nReturn;
}

static int ReadAndValidateString(FILE* pFile, char Buffer[], char* ValidationStr)
{
    int nResult = 0;
	int nLength;
	//fseek(pFile, sizeof("\n")-1, SEEK_CUR);
	if (fread(&nLength, sizeof(int), 1, pFile) == 1)
	{
		memset(Buffer, '\0', nLength);
		if (fread(Buffer, sizeof(char) * nLength, 1, pFile) == NULL)
		{
			nResult = GetTheErrorType(pFile);
		}
	}

    if (strcmp(Buffer, ValidationStr) != 0)
    {
        nResult = INVALID_FORMAT_FILE;
    }

    return nResult;
}

static int WriteAString(FILE* pFile, char SrcString[])
{
    int nResult = PROBLEM_IN_WRITING_READING;

    size_t Length = strlen(SrcString);
	//fputs("\n", pFile);
	if (fwrite(&Length, sizeof(int), 1, pFile) == 1)
	{
		if (fwrite(SrcString, sizeof(char), Length, pFile) == Length)
		{
			nResult = 0;
		}
	}

    return nResult;
}

//Project manager object
static CProjConfigManager g_ProjCfgManager;


// Project getters: start
int FileGetProjectCount(void)
{
    return g_ProjCfgManager.GetProjectCount();
}

int FileGetProjectList(list<string>& ProjectList)
{
    return g_ProjCfgManager.GetProjectList(ProjectList);
}

bool FileGetProjectData(string ProjectName, PROJECTDATA& ProjData)
{
    return g_ProjCfgManager.GetProjectData(ProjectName, ProjData);
}
// Project getters: end


// Section getters: start
int FileGetSectionCount(string ProjectName)
{
    return g_ProjCfgManager.GetSectionCount(ProjectName);
}

int FileGetSectionList(string ProjectName, list<string>& SectionList)
{
    return g_ProjCfgManager.GetSectionList(ProjectName, SectionList);
}

bool FileGetSectionData(string ProjectName, string SectionName, SECTIONDATA& SectionData)
{
    return g_ProjCfgManager.GetSectionData(ProjectName, SectionName, SectionData);
}
// Section getters: end


// Project setters: start
void FileAddModifyProjectTable(string ProjectName, PROJECTDATA& ProjData)
{
    g_ProjCfgManager.AddModifyProjDetail(ProjData);
}

bool FileDeleteProjectTable(string ProjectName)
{
    g_ProjCfgManager.DeleteProjectTable(ProjectName);
    return true;
}
// Project setters: end

// Section setter: start
bool FileAddModifySectionData(string ProjectName, string SectionName, 
                              SECTIONDATA& SectionData)
{
    return g_ProjCfgManager.AddModifySection(ProjectName, SectionData);
}
// Section setter: end


static int ReadWriteASection(bool bToRead, short SectionID, 
                             FILE* pFile, void* pData = NULL)
{
    int nResult = 0;

    switch (SectionID)
    {
        case SECTION_SIGNATURE:
        {
            if (bToRead)
            {
                char acBuffer[LEN_SIG_FILE_START] = {'\0'};
				nResult = ReadAndValidateString(pFile, acBuffer, DATABASE_SIGNATURE);
				if (nResult != 0)
				{
					ReadWriteASection(bToRead, SECTION_FILE_INVALID, NULL);
				}
			}
            else
            {
                nResult = WriteAString(pFile, DATABASE_SIGNATURE);
            }
            if (nResult == 0)
            {
                nResult = ReadWriteASection(bToRead, SECTION_VERSION, pFile);
            }
        }
        break;
        case SECTION_VERSION:
        {
            if (bToRead)
            {
                char acBuffer[LEN_VERSION] = {'\0'};
                nResult = ReadAndValidateString(pFile, acBuffer, VERSIONLINE);
				if (nResult != 0)
				{
					ReadWriteASection(bToRead, SECTION_FILE_INVALID, NULL);
				}
            }
            else
            {
                nResult = WriteAString(pFile, VERSIONLINE);
            }
            if (nResult == 0)
            {
                nResult = ReadWriteASection(bToRead, SECTION_PROJECT_TABLE_SIGNATURE, pFile);
            }
        }
        break;
        case SECTION_PROJECT_TABLE_SIGNATURE:
        {
            if (bToRead)
            {
                char acBuffer[LEN_SIG_PROJ_TABLE] = {'\0'};
                nResult = ReadAndValidateString(pFile, acBuffer, PROJECT_TABLE_SIG);
				if (nResult != 0)
				{
					ReadWriteASection(bToRead, SECTION_FILE_INVALID, NULL);
				}
            }
            else
            {
                nResult = WriteAString(pFile, PROJECT_TABLE_SIG);
            }
            if (nResult == 0)
            {
                nResult = ReadWriteASection(bToRead, SECTION_PROJECT_TABLE_COUNT, pFile);
            }
        }
        break;
        case SECTION_PROJECT_TABLE_COUNT:
        {
            UCHAR Entries = 0;
            if (bToRead)
            {
				//fseek(pFile, sizeof("\n")-1, SEEK_CUR);
                if (fread(&Entries, sizeof(Entries), 1, pFile) == 1)
                {
                    nResult = ReadWriteASection(bToRead, SECTION_PROJECT_TABLE_ENTRY,
                                                pFile, &Entries);
                }
                else
                {
                    nResult = GetTheErrorType(pFile);
                }
            }
            else
            {
                Entries = (UCHAR) g_ProjCfgManager.GetProjectCount();
				//fputs("\n", pFile);
                if (fwrite(&Entries, sizeof(UCHAR), 1, pFile) == 1)
                {
                    nResult = ReadWriteASection(bToRead, SECTION_PROJECT_TABLE_ENTRY,
                                                pFile, &Entries);
                }
                else
                {
                    nResult = PROBLEM_IN_WRITING_READING;
                }
            }
        }
        break;
        case SECTION_PROJECT_TABLE_ENTRY:
        {
            UCHAR Entries = *((UCHAR*) pData);
            if (bToRead)
            {
                for (UCHAR i = 0; (i < Entries) && (nResult == 0); i++)
                {
                    PROJECTDATA sProjectDataTmp;
                    if (sProjectDataTmp.Read(pFile))
                    {
                        g_ProjCfgManager.AddModifyProjDetail(sProjectDataTmp);
                    }
                    else
                    {
                        nResult = GetTheErrorType(pFile);
                    }
                }
            }
            else
            {
                bool bAllWell = true;
                LISTSTR ProjectList;
                g_ProjCfgManager.GetProjectList(ProjectList);
                
                for (LISTSTR::iterator i = ProjectList.begin();
                     (i != ProjectList.end()) && bAllWell; i++)
                {
                    PROJECTDATA ProjData;
                    if (bAllWell = g_ProjCfgManager.GetProjectData(*i, ProjData))
                    {
                        bAllWell = ProjData.Write(pFile);
                    }
                }
                if (!bAllWell)
                {
                    nResult = PROBLEM_IN_WRITING_READING;
                }
            }
            if (nResult == 0)
            {
                nResult = ReadWriteASection(bToRead, SECTION_SECTION_TABLE_ENTRY,
                                            pFile, &Entries);
            }
        }
        break;
        case SECTION_SECTION_TABLE_ENTRY:
        {
            bool bAllWell = true;
            if (bToRead)
            {
                UCHAR ProjectEntries = *((UCHAR *) pData);
                for (UCHAR i = 0; (i < ProjectEntries) && bAllWell; i++)
                {
                    string ProjectName;
                    bAllWell = ReadAString(pFile, ProjectName);
					//ProjectName[ProjectName.length()-1] = '\0';
                    UCHAR SectionEntries = 0;
                    if (bAllWell)
                    {
						//fseek(pFile, sizeof("\n")-1, SEEK_CUR);
                        bAllWell = (fread(&SectionEntries, sizeof(SectionEntries),
                                          1, pFile) == 1);
                    }
					if (bAllWell)
					{
						for (UCHAR j = 0; (j < SectionEntries) && bAllWell; j++)
						{
							SECTIONDATA CurrSection;
							if (bAllWell = CurrSection.Read(pFile))
							{
								g_ProjCfgManager.AddModifySection(
									ProjectName, CurrSection);
							}
							else
							{
								nResult = GetTheErrorType(pFile);
							}
						}
					}
                }
				 if (!bAllWell)
                {
                    nResult = PROBLEM_IN_WRITING_READING;
                }
            }
            else
            {
                LISTSTR ProjectList;
                g_ProjCfgManager.GetProjectList(ProjectList);
                for (LISTSTR::iterator i = ProjectList.begin();
                     (i != ProjectList.end()) && bAllWell; i++)
                {
                    const char* str = i->c_str();
                    if ((nResult = WriteAString(pFile, (char *) str)) == 0)
                    {
                        LISTSTR SectionList;
                        UCHAR Sections = (UCHAR) g_ProjCfgManager.GetSectionList(*i, SectionList);
						//fputs("\n", pFile);
                        if (fwrite(&Sections, sizeof(UCHAR), 1, pFile) != 1)
                        {
                            bAllWell = false;
                        }
                        for (LISTSTR::iterator j = SectionList.begin();
                             (j != SectionList.end()) && bAllWell; j++)
                        {
                            SECTIONDATA CurrSecData;
                            bAllWell = g_ProjCfgManager.GetSectionData(*i, *j, CurrSecData);
                            if (bAllWell)
                            {
                                bAllWell = CurrSecData.Write(pFile);
                            }
                        }
                    }
                }
                if (!bAllWell)
                {
                    nResult = PROBLEM_IN_WRITING_READING;
                }
            }
            if (bAllWell)
            {
                nResult = ReadWriteASection(bToRead, SECTION_SIGNATURE_FILE_END, pFile);
            }
        }
        break;
        case SECTION_SIGNATURE_FILE_END:
        {
            if (bToRead)
            {
                char acBuffer[LEN_SIG_FILE_END] = {'\0'};
                nResult = ReadAndValidateString(pFile, acBuffer, DB_END_SIGNATURE);
				if (nResult != 0)
				{
					ReadWriteASection(bToRead, SECTION_FILE_INVALID, NULL);
				}
            }
            else
            {
                nResult = WriteAString(pFile, DB_END_SIGNATURE);
            }
        }
        break;


        default:
        {
            nResult = FILE_INVALID_SECTIONID;
        }
        break;
    }

    return nResult;
}

void CloseDataFile()
{
	g_ProjCfgManager.DeleteAllProjectTable();
}

int LoadDataFile(char FileName[])
{
    g_ProjCfgManager.DeleteAllProjectTable();

    int nResult = 0;
    FILE *pFile = NULL;	

    //Check if file exists
    if (fopen_s(&pFile, FileName, "rb") == 0)
    {
        nResult = ReadWriteASection(true, SECTION_SIGNATURE, pFile);
        fclose(pFile);
    }
    else
    {
        nResult = OPENING_FILE_ERROR;
    }

    return nResult;
}


int SaveDataFile(char FileName[])
{
    int nResult = 0;
    FILE *pFile = NULL;

    //open again with write permission
    if (fopen_s(&pFile, FileName, "wb") == 0)
    {
        nResult = ReadWriteASection(false, SECTION_SIGNATURE, pFile);
        fclose(pFile);
    }
    else
    {
        nResult = OPENING_FILE_ERROR;
    }

    return nResult;
}

