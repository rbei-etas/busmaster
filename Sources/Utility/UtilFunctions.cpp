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
 * @brief Implementation file for CUtilFunctions class
 * @author Raja N, Tobias Lorenz
 * @copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Implementation file for CUtilFunctions class
 */

/* Standard include header */
#include "Utils_stdafx.h"

/* For CUtilFunctions class definition */
#include "Utility_Structs.h"
#include "UtilFunctions.h"

#define MAX_LEN 512
#define ABS_START 3
#define SLASH '\\'

CUtilFunctions::CUtilFunctions()
{
}

CUtilFunctions::~CUtilFunctions()
{
}

void CUtilFunctions::s_vRemoveUnwantedBits(__int64 & n64rData, int nLength)
{
    /*
     * So -1 is FFFFFFFFFFFFFFFF (as it is __int64 type)
     * if the data length is 8 bits take only FF. *th bit is sign
     * bit and that is set to denote negative number
     * Set all bits to 1. That is FFFFFFFF FFFFFFFF (-1)
     */
    unsigned __int64 un64Mask = static_cast<unsigned __int64>(-1);

    /* Create the mask */
    un64Mask = un64Mask >> (defMAX_BITS - nLength);

    /* Mask unwanted portion of signal details */
    n64rData = n64rData & un64Mask;
}

void CUtilFunctions::s_vExtendSignBit( __int64 & n64Val, int nSize)
{
    /* Avoid processing 64 bit signals */
    if( nSize < defMAX_BITS )
    {
        __int64 n64Mask = 1;
        BOOL bSignBit;

        /*  Shift the mask by Length  - 1 times to get sign bit value */
        n64Mask <<= nSize - 1;

        /* Get the sign bit value */
        bSignBit = n64Val & n64Mask ? TRUE : FALSE;

        /* Set the value only for negative numbers */
        if( bSignBit )
        {
            /* Set the Sign bit to 1 */
            __int64 nVal = defSIGN_MASK;

            /* Shift the value to extend the value */
            nVal >>= ( defMAX_BITS - nSize - 1);

            /* Apply the mask */
            n64Val |= nVal;
        }
    }
}

INT CUtilFunctions::nGetBaseFolder(const char * omConfigFileName, std::string & omStrConfigFolder)
{
    char pchFilePath[MAX_PATH];
    char* pchTemp = pchFilePath;
    strcpy(pchFilePath, omConfigFileName);

    if ( PathRemoveFileSpec(pchTemp) == TRUE )
    {
        omStrConfigFolder = pchTemp;
        omStrConfigFolder += "\\";
    }
    else
    {
        int nChars = GetCurrentDirectory(0, nullptr);
        char* pchFilePath = new char [nChars];
        GetCurrentDirectory(nChars, pchFilePath);
        omStrConfigFolder = pchFilePath;
        delete []pchFilePath;
    }


    return S_OK;
}

void CUtilFunctions::MakeRelativePath(const char * pchCurrentDir, char * pchAbsFileName, std::string & omStrRelativeParh)
{
    int nCurrentDirectoryLength = strlen(pchCurrentDir);
    int nAbsoluteFileLen = strlen(pchAbsFileName);

    if( nCurrentDirectoryLength > MAX_LEN || nCurrentDirectoryLength < ABS_START+1 ||
            nAbsoluteFileLen > MAX_LEN || nAbsoluteFileLen < ABS_START+1)
    {

        omStrRelativeParh = "";
        return ;
    }

    CString strDirectory = pchCurrentDir[0];
    CString strDirectory2 = pchAbsFileName[0];
    char chRelativeFile[MAX_LEN+1];
    if(strDirectory.CompareNoCase(strDirectory2) != 0)
    {
        strcpy_s(chRelativeFile, MAX_LEN+1, pchAbsFileName );
        omStrRelativeParh = chRelativeFile;
        return;
    }
    int nCount = ABS_START;
    while(nCount < nAbsoluteFileLen && nCount < nCurrentDirectoryLength && pchCurrentDir[nCount] == pchAbsFileName[nCount] )
    {
        nCount++;
    }
    if(nCount == nCurrentDirectoryLength && (pchAbsFileName[nCount] == '\\' || pchAbsFileName[nCount-1] == '\\'))
    {
        if(pchAbsFileName[nCount] == '\\')
        {
            nCount++;
        }
        strcpy(chRelativeFile, &pchAbsFileName[nCount]);
        omStrRelativeParh = chRelativeFile;
        return;
    }
    int nForword = nCount;
    int nStages = 1;
    while(nCount < nCurrentDirectoryLength)
    {
        nCount++;
        if(pchCurrentDir[nCount] == '\\')
        {
            nCount++;
            if(pchCurrentDir[nCount] != '\0')
            {
                nStages++;
            }
        }
    }
    while(nForword > 0 && pchAbsFileName[nForword-1] != '\\')
    {
        nForword--;
    }
    if(nStages * 3 + nAbsoluteFileLen - nForword > MAX_LEN)
    {
        omStrRelativeParh = "";
        return ;
    }

    int nReverse = 0;
    for(nCount = 0; nCount < nStages; nCount++)
    {
        chRelativeFile[nReverse] = '.';
        nReverse++;
        chRelativeFile[nReverse] = '.';
        nReverse++;
        chRelativeFile[nReverse] = '\\';
        nReverse++;
    }
    strcpy(&chRelativeFile[nReverse], &pchAbsFileName[nForword]);
    omStrRelativeParh = chRelativeFile;
    return;
}
