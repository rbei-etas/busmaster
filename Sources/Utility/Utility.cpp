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
 * \file      Utility.cpp
 * \brief     This file contains utility function definitions
 * \authors   Ratnadip Choudhury, Anish Kumar, Pradeep Kadoor
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * This file contains utility function definitions
 */

/// All the utility function defined here

#include "Utils_stdafx.h"
#include <math.h>
#include <shlwapi.h>
#include <cstringt.h>

#define defBASE_DEC                 10
#define defBASE_HEX                 16
#define defMAX_BITS                 64
#define defSIGN_MASK                0x8000000000000000

bool BSTR_2_PCHAR(BSTR bstrSrcStr, char acTargetStr[], int nTargetStrLen)
{
    bool bResult = false;

    int Length = SysStringLen(bstrSrcStr);
    if ((Length + 1) <= nTargetStrLen)
    {
        bResult = true;
        for (int i = 0; i < Length; i++)
        {
            acTargetStr[i] = (CHAR) bstrSrcStr[i];
        }
        acTargetStr[Length] = '\0';
    }

    return bResult;
}

BOOL WORD_2_CHAR(char acCharDat[], int nCharDatLen, 
                 WORD awWordDat[], int nWordDatLen)
{
    BOOL bResult = FALSE;
    if (nCharDatLen >= 2*nWordDatLen)
	{
		WORD dTempWord = 0;
		for (int i = 0; i < nWordDatLen; i++)
		{
			dTempWord = awWordDat[i];
			acCharDat[2*i] = LOBYTE(dTempWord);
			acCharDat[2*i+1] =  HIBYTE(dTempWord);
		}
		acCharDat[2*nWordDatLen] = '\0';
		bResult = TRUE;
	}	 
    return bResult;
}

const int MAX_CHAR = 1024;

/* This function 'distills' the source string of any unpermitted character. The
current implementation is inefficient in terms of memory usage; this makes use 
of a temporary memory. An ideal solution would be to use the same memory even 
as workspace. This should be done at the earliest. */

void FilterOut_NonHexChar(char acSource[])
{
    int Length = (int) strlen(acSource);
    ASSERT(Length < MAX_CHAR);

    char acTmp[MAX_CHAR] = {'\0'};
    char CurrChar = '\0';

    // First copy only the permitted characters into the temporary string
    for (int i = 0, j = 0; i < Length; i++)
    {
        CurrChar = (char)(toupper(acSource[i]));
        if ( ((CurrChar >= '0') && (CurrChar <= '9')) || 
             ((CurrChar >= 'A') && (CurrChar <= 'F')) )
        {
            acTmp[j++] = CurrChar;
        }
    }

    memset(acSource, '\0', Length); // Clear the source string
    strcpy(acSource, acTmp);        // Copy back the filtered string
}

/* Given the string representation of a hexadecimal number, this function 
returns the numerical equivalent */
BYTE HexStr_2_BYTE(char acHexStr[])
{
    BYTE bResult = 0;

    char CurrChar = (char)(toupper(acHexStr[1]));
    if ((CurrChar >= '0') && (CurrChar <= '9'))
    {
        bResult = CurrChar - '0';
    }
    else if ((CurrChar >= 'A') && (CurrChar <= 'F'))
    {
        bResult = CurrChar - 'A' + 10;
    }

    CurrChar = (char)(toupper(acHexStr[0]));
    if ((CurrChar >= '0') && (CurrChar <= '9'))
    {
        bResult += (CurrChar - '0') * 16;
    }
    else if ((CurrChar >= 'A') && (CurrChar <= 'F'))
    {
        bResult += (CurrChar - 'A' + 10) * 16;
    }

    return bResult;
}


/* This function expect that there is no non numeric character in 
   the passed array of char*/
BOOL CHAR_2_WORD(WORD awWordDat[], int nWordDatLen, 
                 char acCharDat[], int nCharDatLen)
{
    UNREFERENCED_PARAMETER(nWordDatLen);            //For release mode
    /* Four (= sizeof(WORD) * 2) characters represent a WORD value. Therefore,
    the granularity of characters for conversion into WORD should be 4 */
    int Gran = sizeof(WORD) * 2;

    // We assume that the input stream is ideally convertible to WORD stream.
    ASSERT((nCharDatLen % Gran) == 0);
    // The WORD array must be able to accommodate the result
    ASSERT((nWordDatLen * Gran) >= nCharDatLen);

    for (int i = 0, j = 0; i < nCharDatLen; i += Gran)
    {
        WORD CurrWVal = HexStr_2_BYTE(&acCharDat[i]);
        CurrWVal <<= 8;
        CurrWVal = CurrWVal + (WORD)(HexStr_2_BYTE(&acCharDat[i + 2]));

        awWordDat[j++] = CurrWVal;
    }

    return TRUE;
}

/* Calculation of HCF starts here */

int GCD(int a, int b) 
{ 
	if ( a < 0 || b < 0)
	{
		return -1;
	}
	//This is Euclid's method of finding GCD
    return ( b == 0 ? a : GCD(b, a % b) ); 
}

int CalculateGCF(int anNumbers[], int Length)
{
	int nGCF = -1;
	if (Length > 0)
	{
		if (Length > 1)
		{
			//Find GCD of first 2 nos. then GCD of the obtained GCD and next no. 
			nGCF = GCD(anNumbers[0], anNumbers[1]);
			for (int i = 2; i < Length; i++)
			{
				nGCF = GCD(nGCF, anNumbers[i]);
			}
		}
		else
		{
			nGCF = anNumbers[0];
		}
	}
	return nGCF;
}
/* Calculation of HCF ends here */

/*******************************************************************************

*******************************************************************************/
void vRemoveUnwantedBits(__int64& n64rData, int nLength)
{
    // So -1 is FFFFFFFFFFFFFFFF (as it is __int64 type)
    // if the data length is 8 bits take only FF. *th bit is sign
    // bit and that is set to denote negative number
    // Set all bits to 1. That is FFFFFFFF FFFFFFFF (-1)
    unsigned __int64 un64Mask = static_cast<unsigned __int64>(-1);
    // Create the mask
    un64Mask = un64Mask >> (defMAX_BITS - nLength);
    // Mask unwanted portion of signal details
    n64rData = n64rData & un64Mask;
}

void vExtendSignBit( __int64& n64Val, int nSize)
{
    // Avoid processing 64 bit signals
    if( nSize < defMAX_BITS )
    {
        __int64 n64Mask = 1;
        BOOL bSignBit;
        // Shift the mask by Length  - 1 times to get sign bit value
        n64Mask <<= nSize - 1;
        // Get the sign bit value
        bSignBit = n64Val & n64Mask ? TRUE : FALSE;
        // Set the value only for negative numbers
        if( bSignBit )
        {
            // Set the Sign bit to 1
            __int64 nVal = defSIGN_MASK;
            // Shift the value to extend the value
            nVal >>= ( defMAX_BITS - nSize - 1);
            // Apply the mask
            n64Val |= nVal;
        }
    }
}

#if 0
bool bGetSystemErrorString(HRESULT hResult, CHAR acErrStr[256])
{
    bool bResult = true;

    memset(acErrStr, 0, sizeof(acErrStr));
    LPVOID lpMsgBuf;
    DWORD dwResult = 0;

    dwResult = FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER |
                    FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                    NULL, hResult,
                    MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),// Default language
                    (LPTSTR) &lpMsgBuf, 0, NULL );
    if (dwResult <= 0)
    {
        strcpy(acErrStr, "system error message retrieval operation failed");
        bResult = false;
    }
    else
    {
        LPSTR pBuf = T2A((LPTSTR) lpMsgBuf);
        _tcscpy(acErrStr, pBuf);
        LocalFree(lpMsgBuf); // Free the buffer.
    }

    return bResult;
}
#endif

bool bGetSystemErrorString(HRESULT hResult, TCHAR acErrStr[256])
{
    bool bResult = true;

    memset(acErrStr, _T('\0'), _tcslen(acErrStr));
    LPVOID lpMsgBuf;
    DWORD dwResult = 0;

    dwResult = FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER |
                    FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                    NULL, hResult,
                    MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),// Default language
                    (LPTSTR) &lpMsgBuf, 0, NULL );
    if (dwResult <= 0)
    {
        _tcscpy(acErrStr, _T("system error message retrieval operation failed"));
        bResult = false;
    }
    else
    {
        _tcscpy(acErrStr, (LPTSTR) lpMsgBuf);
        LocalFree(lpMsgBuf); // Free the buffer.
    }

    return bResult;
}

#if 0
bool bGetSystemErrorString(char acErrStr[256])
{
    return bGetSystemErrorString((HRESULT) GetLastError(), acErrStr);
}
#endif

// To copy the text into the clipboard
BOOL CopyTextToClipboard(LPSTR lpstrText, HWND hWnd = NULL)
{
	HGLOBAL hGlobal;    // Global memory handle
	//TCHAR* lpszData;    // Pointer to clipboard data
    LPSTR lpszData;
	SIZE_T nSize;       // Size of clipboard data

	OpenClipboard(hWnd);// Open clipboard
	EmptyClipboard();   // Delete Other clipboard values

	nSize = strlen(lpstrText);
	// Allocate Global Memory which is required to store the text
	hGlobal = GlobalAlloc(GMEM_ZEROINIT, (nSize + 1));

	if (hGlobal == NULL) return FALSE;
	// Lock the memory and store the text into it
	lpszData = (LPSTR) GlobalLock(hGlobal);
    strcpy(lpszData, lpstrText);
    //_tcsncpy(lpszData, lpctstrText, nSize + 1);

	GlobalUnlock(hGlobal);  // Unlock the memory
	/* Copy the contents from the globally allocated memory to Clipboard
	GlobalFree function will be called by SetClipBoard Function */
    SetClipboardData(CF_TEXT, hGlobal);
	/* Close the clipboard so that other application can access it */
	CloseClipboard();

	return TRUE;
}

#if 0
BOOL CopyTextToClipboard(LPCTSTR lpctstrText, HWND hWnd = NULL)
{
	HGLOBAL hGlobal;		// Global memory handle
	LPSTR lpszData;			// Pointer to clipboard data
	unsigned long nSize;	// Size of clipboard data
	// Open clipboard
	OpenClipboard(hWnd);
	// Delete Other clipboard values
	EmptyClipboard();

	nSize = lstrlen(lpctstrText);
	// Allocate Global Memory which is required to store the text
	hGlobal = GlobalAlloc(GMEM_ZEROINIT, nSize+1);

	if (hGlobal == NULL) return FALSE;
	// Lock the memory and store the text into it
	lpszData = (LPSTR)GlobalLock(hGlobal);
	for (UINT i = 0; i < nSize + 1; ++i)
	{
        *(lpszData + i) = *(lpctstrText + i);
	}
	// Unlock the memory
	GlobalUnlock(hGlobal);
	/* Copy the contents from the globally allocated memory to Clipboard
	GlobalFree function will be called by SetClipBoard Function */
	SetClipboardData(CF_TEXT, hGlobal);
	/* Close the clipboard so that other application can access it */
	CloseClipboard();

	return TRUE;
}
#endif
/******************************************************************************
FUNCTION:       nConvertStringToInt
PARAMETERS:     CString omStrHexNo -- Hex number
RETURN VALUE:   Equivalent decimal number
AUTHOR:         RBIN/EMC2 - Amarnath Shastry
HISTORY:        
20-02-2002      Initial Version     Amarnath Shastry    
02.06.2003      Changed the int to __int64 Amitesh Bharti
12.06.2003      review comment for CRH0005 incorporated. Amitesh Bharti
*******************************************************************************/
__int64 nConvertStringToInt(LPCTSTR omStrHexNo)
{
    __int64 n64RetVal = 0;

    BOOL b_IsNegative = FALSE;
    int nLength = (int) _tcslen(omStrHexNo);
    for (int nCount = 0; nCount < nLength; nCount++)
    {
        TCHAR cChar = omStrHexNo[nCount];

        if ( cChar == '0' )
        {
            n64RetVal = n64RetVal * 16 + 0;
        }
        else if ( cChar == '1' )
        {
            n64RetVal = n64RetVal * 16 + 1;
        }
        else if ( cChar == '2' )
        {
            n64RetVal = n64RetVal * 16 + 2;
        }
        else if ( cChar == '3' )
        {
            n64RetVal = n64RetVal * 16 + 3;
        }
        else if ( cChar == '4' )
        {
            n64RetVal = n64RetVal * 16 + 4;
        }
        else if ( cChar == '5' )
        {
            n64RetVal = n64RetVal * 16 + 5;
        }
        else if ( cChar == '6' )
        {
            n64RetVal = n64RetVal * 16 + 6;
        }
        else if ( cChar == '7' )
        {
            n64RetVal = n64RetVal * 16 + 7;
        }
        else if ( cChar == '8' )
        {
            n64RetVal = n64RetVal * 16 + 8;
        }
        else if ( cChar == '9' )
        {
            n64RetVal = n64RetVal * 16 + 9;
        }
        else if ( cChar == 'A' || cChar == 'a')
        {
            n64RetVal = n64RetVal * 16 + 10;
        }
        else if ( cChar == 'B' || cChar == 'b')
        {
            n64RetVal = n64RetVal * 16 + 11;
        }
        else if ( cChar == 'C' || cChar == 'c')
        {
            n64RetVal = n64RetVal * 16 + 12;
        }
        else if ( cChar == 'D' || cChar == 'd')
        {
            n64RetVal = n64RetVal * 16 + 13;
        }
        else if ( cChar == 'E' || cChar == 'e')
        {
            n64RetVal = n64RetVal * 16 + 14;
        }
        else if ( cChar == 'F' || cChar == 'f')
        {
            n64RetVal = n64RetVal * 16 + 15;
        }
        else if ( cChar == '-' )
        {
            b_IsNegative = TRUE;
        }
    }
    if ( b_IsNegative )
    {
        n64RetVal = -n64RetVal;
    }
    return n64RetVal;
}

/*******************************************************************************
 Function Name   : s_vExtendSignBit
 Input(s)        : n64Val    - Int 64 bit data
                   nSize     - Valid Data length
 Output          :  -
 Functionality   : This function extends the sign bit of a data to get actual
                   value of a signed number that is stored in 2s complement form
 Member of       : Static member of CUtilFunctions
 Friend of       :  -
 Author(s)       : Raja N
 Date Created    : 31.07.2004
 Modification By :
 Modification on :
*******************************************************************************/
void s_vExtendSignBit( __int64& n64Val, int nSize)
{
    // Avoid processing 64 bit signals
    if( nSize < defMAX_BITS )
    {
        __int64 n64Mask = 1;
        BOOL bSignBit;
        // Shift the mask by Length  - 1 times to get sign bit value
        n64Mask <<= nSize - 1;
        // Get the sign bit value
        bSignBit = n64Val & n64Mask ? TRUE : FALSE;
        // Set the value only for negative numbers
        if( bSignBit )
        {
            // Set the Sign bit to 1
            __int64 nVal = defSIGN_MASK;
            // Shift the value to extend the value
            nVal >>= ( defMAX_BITS - nSize - 1);
            // Apply the mask
            n64Val |= nVal;
        }
    }
}
/*******************************************************************************
 Function Name    : s_vRemoveUnwantedBits
 Input(s)         : n64rData    - Int 64 bit data
                    nLength     - Valid Data length
 Output           :  -
 Functionality    : This function will set the unsued portion to zero. This is
                    required to get the 2's value of a signal lessage then 64
                    bit length
 Member of        : Static member of CUtilFunctions
 Friend of        :  -
 Author(s)        : Raja N
 Date Created     : 30.07.2004
 Modification By  :
 Modification on  :
*******************************************************************************/
void s_vRemoveUnwantedBits(__int64& n64rData, int nLength)
{
    // So -1 is FFFFFFFFFFFFFFFF (as it is __int64 type)
    // if the data length is 8 bits take only FF. *th bit is sign
    // bit and that is set to denote negative number
    // Set all bits to 1. That is FFFFFFFF FFFFFFFF (-1)
    unsigned __int64 un64Mask = static_cast<unsigned __int64>(-1);
    // Create the mask
    un64Mask = un64Mask >> (defMAX_BITS - nLength);
    // Mask unwanted portion of signal details
    n64rData = n64rData & un64Mask;
}

/*******************************************************************************
 Function Name    : bConvertStringToInt64
 Input(s)         : omStrHexNo - String Value
                    n64RetVal  - Numeric Value of the string
                    nBase - Base Value
 Output           : BOOL - Success or Failure
 Functionality    : This function will convert the string in to the 64 bit
                    integer.
 Member of        : Static member of CUtilFunctions
 Friend of        :  -
 Author(s)        : Raja N
 Date Created     : 30.07.2004
 Modification By  :
 Modification on  :
*******************************************************************************/
BOOL bConvertStringToInt64( CString omStrHexNo,
                              __int64& n64RetVal,
                              int nBase )
{
    // Initialise local variables
    // Success Flag
    BOOL bSuccess = TRUE;
    // Length of the string
    int nStrLength = 0;
    // Present digit value
    int nCharVal = 0;
    // Negative flag
    BOOL b_IsNegative = FALSE;
    // Initialise the return value
    n64RetVal = 0;
    // Convert in to upper case
    omStrHexNo.MakeUpper();
    // Removes spaces at the begining and at the end
    omStrHexNo.TrimLeft();
    omStrHexNo.TrimRight();
    // Get the String length now after removing spaces.
    nStrLength = omStrHexNo.GetLength();
    // Iterate through elements to calculate values
    for (int nCount = 0; nCount < nStrLength; nCount++)
    {
        // Get the charector
        TCHAR cChar = omStrHexNo.GetAt( nCount);
        
        // Check for 0 - 9 range
        if( cChar >= '0' && cChar <= '9')
        {
            // Subtract char '0' to get the int value
            // say char '5' - char '0' - int 5
            nCharVal = cChar - '0';
            // If the value is greater then base then
            // the string is invalid. say with base 8, 9 is invalid
            if( nCharVal - nBase >= 0 )
            {
                // Show Error
                ASSERT (FALSE );
                // Stop the loop by setting the max value
                nCount = nStrLength;
                // Indicate Failure
                bSuccess = FALSE;
            }
            else
            {
                // Add current digit with the value
                n64RetVal = n64RetVal * nBase + nCharVal;
            }
        }
        // If the value is Hex
        else if( cChar >= 'A' && cChar <= 'F' )
        {
            // If the base is not hex then show error
            // and stop processing
            if( nBase != defBASE_HEX )
            {
                // Show Error
                ASSERT( FALSE );
                // Stop the loop by setting the max value
                nCount = nStrLength;
                // Indicate Failure
                bSuccess = FALSE;

            }
            // else Valid value
            else
            {
                // Get the int value and add 10 with that
                // Say 'A' - 'A' + 10 = 10
                //     'C' - 'A' + 10 = 12
                nCharVal = cChar - 'A' + defBASE_DEC;
                // Add current digit with the value
                n64RetVal = n64RetVal * nBase + nCharVal;
            }
        }
        else if( cChar == '-' )
        {
            // In Hex mode negative values are not correct
            if ( nBase != defBASE_DEC || nCount != 0 )
            {
                ASSERT( FALSE );
                // Stop the loop by setting the max value
                nCount = nStrLength;
                // Indicate Failure
                bSuccess = FALSE;

            }
            else
            {
                b_IsNegative = TRUE;
            }
        }
        else
        {
            // Invalid input char
            ASSERT( FALSE );
            // Stop the loop by setting the max value
            nCount = nStrLength;
            // Indicate Failure
            bSuccess = FALSE;

        }
    }
    // If negative flag is set then take the negative value
    if ( b_IsNegative == TRUE)
    {
        n64RetVal = -n64RetVal;
    }
    return bSuccess;
}
