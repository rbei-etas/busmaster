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

void CUtilFunctions::s_vRemoveUnwantedBits(__int64& n64rData, int nLength)
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

void CUtilFunctions::s_vExtendSignBit( __int64& n64Val, int nSize)
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

INT CUtilFunctions::nGetBaseFolder(const char* omConfigFileName, std::string& omStrConfigFolder)
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

void CUtilFunctions::MakeRelativePath(const char* pchCurrentDir, char* pchAbsFileName, std::string& omStrRelativeParh)
{
    int nCurrentDirectoryLength = strlen(pchCurrentDir);
    int nAbsoluteFileLen = strlen(pchAbsFileName);

    if( nCurrentDirectoryLength > MAX_LEN || nCurrentDirectoryLength < ABS_START+1 ||
            nAbsoluteFileLen > MAX_LEN || nAbsoluteFileLen < ABS_START+1)
    {

        omStrRelativeParh = "";
        return ;
    }

    CString strDirectory ((char)pchCurrentDir[0]);
    CString strDirectory2 ((char)pchAbsFileName[0]);
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
    if (nCount < nAbsoluteFileLen)
    {
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
    while(nForword > 0 && nForword < nAbsoluteFileLen && pchAbsFileName[nForword-1] != '\\')
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
    if (nForword < nAbsoluteFileLen)
    {
        strcpy(&chRelativeFile[nReverse], &pchAbsFileName[nForword]);
    }
    omStrRelativeParh = chRelativeFile;
}
bool CUtilFunctions::bFindLastSuffix(std::string str, std::string subStr, int& npos)
{
    int nTemp1=0;
    int nTemp2=0;
    if(str.find(subStr) == std::string::npos)
    {
        return false;
    }
    else
    {
        do
        {
            nTemp1=nTemp2;
            nTemp2 = str.find(subStr,nTemp1+1);
        }
        while(nTemp2!=std::string::npos);
        npos = nTemp1;
        return true;
    }
}
void CUtilFunctions::Trim(std::string& str , char chChar)
{
    std::string::size_type pos = str.find_last_not_of(chChar);
    if(pos != std::string::npos)
    {
        str.erase(pos + 1);
        pos = str.find_first_not_of(chChar);
        if(pos != std::string::npos)
        {
            str.erase(0, pos);
        }
    }
    else
    {
        str.erase(str.begin(), str.end());
    }
}
/**************************************************************************************
    Function Name    :  IsNumber
    Input(s)         :  const std::string& strNumber
    Output           :  bool
    Functionality    :  Check string is a integer or not.
    Member of        :  CUtilFunctions
    Author(s)        :  Robin G.K.
    Date Created     :  16.10.2015
    Requirement ID   :
***************************************************************************************/
bool CUtilFunctions::IsNumber(const std::string& strNumber)
{
    bool bReturn = false;
    std::string::const_iterator it = strNumber.begin();
    while (it != strNumber.end() && std::isdigit(*it))
    {
        ++it;
    }
    bReturn = !strNumber.empty() && it == strNumber.end();
    return bReturn;
}
BOOL CUtilFunctions::nCalculateCANRegValues(struct sBTRTemp* psColListCtrl,
	WORD wNbt, WORD wBrp, UINT* puwIndex,
	INT nSample)
{
	WORD  wSJW = 0;
	WORD  wTSEG1 = 0;
	WORD  wTSEG2 = 0;
	WORD  wTSEG2min = 0;
	WORD  wTSEG2max = 0;
	WORD  wSampling = 0;

	INT k = 0;
	for (INT i = 0; i<defMAX_SJW; i++)
	{
		wSJW = (BYTE)(i + 1);
		k = 0;
		// Calculate Maximum and Minimum value of TSEG2.
		wTSEG2max = (BYTE)(defmcMIN2(defMAX_TSEG2, wNbt - defMIN_TSEG1 - 1));
		wTSEG2min = (BYTE)(defmcMAX3(defMIN_TSEG2, wSJW, wNbt - defMAX_TSEG1 - 1));
		for (INT j = wTSEG2min; j<wTSEG2max + 1; j++)
		{

			wTSEG2 = (BYTE)(j);
			k++;
			//Calculate Sampling = ((NBT-TSEG2)/NBT) *100
			wSampling = (BYTE)(((FLOAT)(wNbt - wTSEG2) / wNbt) * 100);
			// Only if sampling is greater then 50%, the value is stored in list.
			if (wSampling >= defMIN_SAMPLING)
			{
				// Calculate TSEG1 = NBT-TSEG2-1
				wTSEG1 = (BYTE)(wNbt - wTSEG2 - 1);
				*puwIndex += 1;
				//Fill the structure with BRP, Sampling, NBT,SJW values for display
				psColListCtrl[*puwIndex - 1].sBRPNBTSampNSJW.usBRP = wBrp;
				psColListCtrl[*puwIndex - 1].sBRPNBTSampNSJW.usSampling = wSampling;
				psColListCtrl[*puwIndex - 1].sBRPNBTSampNSJW.usNBT = wNbt;
				psColListCtrl[*puwIndex - 1].sBRPNBTSampNSJW.usSJW = wSJW;
				//Pack the register BTR0 its bit BRPbit = BRP -1; SJWbit = SJW -1;
				psColListCtrl[*puwIndex - 1].uBTRReg0.sBTR0Bit.ucBRPbit = wBrp - 1;
				psColListCtrl[*puwIndex - 1].uBTRReg0.sBTR0Bit.ucSJWbit = wSJW - 1;
				//Pack the register BTR1 its bit TESG1bit=TSEG1-1;TSEG2bit=TSEG2 -1;
				psColListCtrl[*puwIndex - 1].uBTRReg1.sBTR1Bit.ucTSEG1bit = wTSEG1 - 1;
				psColListCtrl[*puwIndex - 1].uBTRReg1.sBTR1Bit.ucTSEG2bit = wTSEG2 - 1;
				//Pack the register BTR1 its bit SAMbit=1 or 0
				if (nSample == 3)
				{
					psColListCtrl[*puwIndex - 1].uBTRReg1.sBTR1Bit.ucSAMbit = 1;
				}
				else
				{
					psColListCtrl[*puwIndex - 1].uBTRReg1.sBTR1Bit.ucSAMbit = 0;
				}
			}
		}
	}

	return 1;
}

BOOL CUtilFunctions::nCalculateCANChannelParameters(struct sBTRTemp* psColListCtrl,
	DOUBLE dBuadRate, WORD wClockFreq, INT nSample)
{
	UINT  unProductNbtNBrp = 0;
	DOUBLE dProductNbtNBrp = 0;
	INT   nReturn = -1;

	UINT puwIndex = 0;
	// Calcualte the product NBT * BRP = clock/(2.0 * baudrate ). This product
	// should be an integer multiple.
	dProductNbtNBrp = (wClockFreq / (dBuadRate / 1000)) / 2.0 *
		(defFACT_FREQUENCY / defFACT_BAUD_RATE);
	unProductNbtNBrp = (UINT)(dProductNbtNBrp + 0.5);
	//Check if product is integer multiple. Ignore diffrence <= 0.004
	if (fabs((dProductNbtNBrp - unProductNbtNBrp)) <= defVALID_DECIMAL_VALUE)
	{
		INT   i = 1;
		WORD wNBT = (WORD)(unProductNbtNBrp / i);
		FLOAT fNBT = (FLOAT)unProductNbtNBrp / i;

		//Calculate all set of NBT and BRP value for a given product of NBT and BRP.
		while (wNBT >= 1 && i <= defMAX_BRP)
		{
			if ((wNBT == fNBT) && (wNBT >= defMIN_NBT) && (wNBT <= defMAX_NBT))
			{
				WORD wBRP = (WORD)(unProductNbtNBrp / wNBT);
				//Call this function to calculate BTR0, BTR1 regsiter value for one set
				// of NBT and BRP.
				nReturn = nCalculateCANRegValues(psColListCtrl, wNBT, wBRP, &puwIndex, nSample);
			}

			i++;
			wNBT = (WORD)(unProductNbtNBrp / i);
			fNBT = (FLOAT)unProductNbtNBrp / i;
		}

	}
	else
	{
		nReturn = -1;
	}
	return nReturn;
}

DOUBLE CUtilFunctions::dCalculateBaudRateFromBTRs(CString omStrBTR0,
	CString omStrBTR1)
{
	uBTR1 uBTR1val;
	uBTR0 uBTR0val;
	DOUBLE dBaudRate = 0;
	BYTE   byBRP = 0;
	BYTE   byNBT = 0;
	BYTE   byTSEG1 = 0;
	BYTE   byTSEG2 = 0;
	char* pcStopStr = nullptr;

	uBTR0val.ucBTR0 = _tcstol(omStrBTR0, &pcStopStr, defHEXADECIMAL);
	uBTR1val.ucBTR1 = _tcstol(omStrBTR1, &pcStopStr, defHEXADECIMAL);

	// BRP = BRPbit+1
	byBRP = static_cast <BYTE> (uBTR0val.sBTR0Bit.ucBRPbit + 1);

	//  TSEG1 = TSEG1bit +1
	byTSEG1 = static_cast <BYTE> (uBTR1val.sBTR1Bit.ucTSEG1bit + 1);

	//TSEG2 = TSEG2bit+1;
	byTSEG2 = static_cast <BYTE> (uBTR1val.sBTR1Bit.ucTSEG2bit + 1);

	//NBT = TESG1 + TSEG2 +1
	byNBT = static_cast <BYTE> (byTSEG1 + byTSEG2 + 1);

	dBaudRate = (DOUBLE)(_tstoi(defCLOCK) / (2.0 * byBRP * byNBT));
	dBaudRate = dBaudRate * (defFACT_FREQUENCY / defFACT_BAUD_RATE);

	/* covert to bps */
	dBaudRate *= 1000;

	return dBaudRate;
}