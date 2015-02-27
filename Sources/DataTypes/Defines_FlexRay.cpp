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
 * \file      FEALData.cpp
 * \brief     Implementation file for FIBEX data types.
 * \author    Ratnadip Choudhury
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Implementation file for FIBEX data types.
 */
#include "DataTypes_StdAfx.h"
#include "MsgSignal_Datatypes.h"
#include "FEALData.h"
#include "../Application/HashDefines.h"
#include "..\Utility\MsgInterpretation.h"
#include "..\Utility\\Utility.h"
#include "Cluster.h"
#include <stack>
#include <queue>

#define defMAX_BYTE 8
#define LP 10
#define RP 20
#define OPERATOR 30
#define OPERAND 40

const int SIZE_CHAR = sizeof(char);

CFrameMap g_ouFrameDataSet;

CCompuMethodEx& CCompuMethodEx::operator=(const CCompuMethodEx& RefObj)
{
    m_eCompuType = RefObj.m_eCompuType;
    m_uMethod = RefObj.m_uMethod;

    return *this;
}

// This function expects that the range check has already been done
bool CCompuMethodEx::omGetEnggValue(DWORD dwRawValue, CString& omEnggValue)
{
    bool bResult = false;

    // reset LowerLimit to ensure a defined value
    m_uMethod.m_LinearCode.m_sRange.m_dwLowerLimit = 0x0;

    if (m_eCompuType == LINEAR_ENUM)
    {
        if (m_uMethod.m_LinearCode.m_sRange.bIsWithinRange(dwRawValue))
        {
            double ApplValue = (m_uMethod.m_LinearCode.m_dN0
                                + m_uMethod.m_LinearCode.m_dN1 * dwRawValue)
                               / m_uMethod.m_LinearCode.m_dD0;


            omEnggValue.Format("%lf", ApplValue);
            bResult = true;
        }
        else
        {
            dwRawValue = m_uMethod.m_LinearCode.m_sRange.m_dwUpperLimit;
            double ApplValue = (m_uMethod.m_LinearCode.m_dN0
                                + m_uMethod.m_LinearCode.m_dN1 * dwRawValue)
                               / m_uMethod.m_LinearCode.m_dD0;


            omEnggValue.Format("%lf", ApplValue);
            bResult = true;
        }
    }
    else if (m_eCompuType == TEXTTABLE_ENUM)
    {
        OBJ_TEXTCODEVAR_EX::iterator itr = m_uMethod.m_objTextCode.begin();
        while (itr != m_uMethod.m_objTextCode.end())
        {
            if (itr->m_sRange.bIsWithinRange(dwRawValue))
            {
                omEnggValue.Format("%lf", (DOUBLE)dwRawValue);

                //omEnggValue = itr->m_aTextName;
                bResult = true;
            }
            itr++;
        }

        /*PTEXT_CODE_VAR_EX tempTextCode = m_uMethod.m_pTextCode;
        while (tempTextCode != nullptr)
        {
            if (tempTextCode->m_sRange.bIsWithinRange(dwRawValue))
            {
                omEnggValue = tempTextCode->m_aTextName;
                bResult = true;
                tempTextCode->m_pNextTextCodeType = nullptr;
            }
            tempTextCode = tempTextCode->m_pNextTextCodeType;
        }*/
    }
    else if (m_eCompuType == TAB_NOINTP_ENUM)
    {
        OBJ_TABCODVAR::iterator itr = m_uMethod.m_objTabCode.begin();
        while (itr != m_uMethod.m_objTabCode.end())
        {
            if (itr->m_sRange.bIsWithinRange(dwRawValue))
            {
                omEnggValue.Format("%lf", (DOUBLE)itr->m_dPhysVal);
                bResult = true;
            }
            itr++;
        }

        /*PTAB_CODE_VAR_EX tempTabCode = m_uMethod.m_pTabCode;
        while (tempTabCode != nullptr)
        {
            if (tempTabCode->m_sRange.bIsWithinRange(dwRawValue))
            {
                omEnggValue.Format("%lf", tempTabCode->m_dPhysVal);
                bResult = true;
                tempTabCode->m_pNextTabCodeType = nullptr;
            }
            tempTabCode = tempTabCode->m_pNextTabCodeType;
        }*/
    }
    else if (m_eCompuType == IDENTICAL_ENUM)
    {
        if (m_uMethod.m_IdenticalCode.m_sRange.bIsWithinRange(dwRawValue))
        {
            omEnggValue.Format("%lf", (DOUBLE)dwRawValue);
            bResult = true;
        }
    }
    else if (m_eCompuType == SCALE_LINEAR_ENUM)
    {
        OBJ_SCALELINEAR::iterator itr = m_uMethod.m_objScaleLinear.begin();
        while (itr != m_uMethod.m_objScaleLinear.end())
        {
            if ((*itr).m_sRange.bIsWithinRange(dwRawValue))
            {
                double ApplValue = ((*itr).m_dN0 + (*itr).m_dN1 * dwRawValue)
                                   / (*itr).m_dD0;
                omEnggValue.Format("%lf", ApplValue);
                bResult = true;
            }
            itr++;
        }
        /*PSCALE_LINEAR_CODE_VAR_EX tempScaleLinear = m_uMethod.m_pScaleLinear;
        while (tempScaleLinear != nullptr)
        {
            LINEAR_CODE_VAR_EX tempLinear = tempScaleLinear->m_sLinearType;
            if (tempLinear.m_sRange.bIsWithinRange(dwRawValue))
            {
                double ApplValue = (tempLinear.m_dN0 + tempLinear.m_dN1 * dwRawValue)
                                   / tempLinear.m_dD0;
                omEnggValue.Format("%lf", ApplValue);
                bResult = true;
                tempScaleLinear->m_pNextLinearType = nullptr;
            }
            tempScaleLinear = tempScaleLinear->m_pNextLinearType;
        }*/
    }
    else if (m_eCompuType == FORMULA_ENUM)// FORMULA
    {
        omEnggValue = m_uMethod.m_Formula.omGetEnggValue(dwRawValue);
    }

    return bResult;
}

// This function expects that the range check has already been done
bool CCompuMethodEx::omGetRawValue(CString& omRawValue, double dwEnggValue)
{
    bool bResult = false;

    if (m_eCompuType == LINEAR_ENUM)
    {
        //if (m_uMethod.m_LinearCode.m_sRange.bIsWithinRange(dwRawValue))
        {
            double dRawValue = ((dwEnggValue * m_uMethod.m_LinearCode.m_dD0) - m_uMethod.m_LinearCode.m_dN0) / m_uMethod.m_LinearCode.m_dN1;

            UINT dwRawValue = static_cast<UINT>(dRawValue);
            if (dRawValue - (UINT)(dRawValue) > 0.99990000)
            {
                dwRawValue = static_cast<UINT>(ceil(dRawValue));
            }
            else
            {
                dwRawValue = static_cast<UINT>(floor(dRawValue));
            }

            omRawValue.Format("%d", dwRawValue);
            bResult = true;
        }
    }
    else if (m_eCompuType == TEXTTABLE_ENUM)
    {
        OBJ_TEXTCODEVAR_EX::iterator itr = m_uMethod.m_objTextCode.begin();
        while (itr != m_uMethod.m_objTextCode.end())
        {
            UINT unRawVal = dwEnggValue;
            omRawValue.Format("%d", unRawVal);
            bResult = true;
            itr++;
        }

    }
    else if (m_eCompuType == TAB_NOINTP_ENUM)
    {
        //TODO
    }
    else if (m_eCompuType == IDENTICAL_ENUM)
    {
        UINT unRawVal = dwEnggValue;
        omRawValue.Format("%d", unRawVal);
        bResult = true;
    }
    else if (m_eCompuType == SCALE_LINEAR_ENUM)
    {
        // TODO
    }
    else if (m_eCompuType == FORMULA_ENUM)// FORMULA
    {
        // TODO
    }

    return bResult;
}


tagCompuMethodEx::~tagCompuMethodEx()
{
    /* if (m_pScaleLinear != nullptr)
     {
         delete m_pScaleLinear;
         m_pScaleLinear = nullptr;
     }*/
    /*if (m_pTextCode != nullptr)
    {
        delete m_pTextCode;
        m_pTextCode = nullptr;
    }*/
    /*if (m_pTabCode != nullptr)
    {
        delete m_pTabCode;
        m_pTabCode = nullptr;
    }*/
}

tagCompuMethodEx& tagCompuMethodEx::operator =(const tagCompuMethodEx& RefObj)
{
    m_LinearCode = RefObj.m_LinearCode;
    m_IdenticalCode = RefObj.m_IdenticalCode;
    m_Formula = RefObj.m_Formula;
    m_objTabCode = RefObj.m_objTabCode;
    m_objTextCode = RefObj.m_objTextCode;
    m_objScaleLinear = RefObj.m_objScaleLinear;

    return *this;
}

tagCompuMethodEx::tagCompuMethodEx(tagCompuMethodEx& RefObj)
{
    m_LinearCode = RefObj.m_LinearCode;
    m_IdenticalCode = RefObj.m_IdenticalCode;
    m_Formula = RefObj.m_Formula;
    m_objTabCode = RefObj.m_objTabCode;
    m_objTextCode = RefObj.m_objTextCode;
    m_objScaleLinear = RefObj.m_objScaleLinear;
}

int nPrecedence(char ch)
{
    int nResult = 0;
    switch (ch)
    {
        case '*':
        case '/':
            nResult = 2;
            break;
        case '+':
        case '-':
            nResult = 1;
            break;
    }
    return (nResult);
}

int nGetType(char ch)
{
    int nResult = 0;
    if (isdigit(ch))//if (isalpha(ch))
    {
        ch = 0;
    }
    switch (ch)
    {
        case '+':
        case '-':
        case '*':
        case '/':
        case '&':
        case '|':
        case '%':
        {
            nResult = OPERATOR;
            break;
        }
        case '(':
        {
            nResult = LP;
            break;
        }
        case ')':
        {
            nResult = RP;
            break;
        }
        case 0:
        {
            nResult = OPERAND;
            break;
        }
    }
    return nResult;
}
int nGetTypeOfSymbol(int index, CString omString, CString& NumValue )
{
    char Num = omString.GetAt(index);
    int nResult = Num - '0';
    char temp = omString.GetAt(++index);
    while (isdigit(temp))//if (isalpha(temp))
    {
        nResult = 10 * nResult + (int) temp - (int) '0';
        temp = omString.GetAt(++index);
    }
    NumValue.Format("%d",nResult);
    return index;
}

CString tagFormulaCodeVarEx::omGetEnggValue(DWORD /* dwRawValue */)
{
    CString omEnggValue = "";
    int nLenght = m_omFormula.GetLength();
    int i = 0;

    /*converting infix expr to postfix*/
    std::stack <char> PostfixStack;
    std::queue <CString> PostfixExpression;
    while (i < nLenght)
    {
        char temp = m_omFormula.GetAt(i);
        char next;
        int choice = nGetType(temp);
        switch (choice)
        {
            case LP:
            {
                PostfixStack.push(temp);
                break;
            }
            case RP:
            {
                while ((next = PostfixStack.top()) != '(')
                {
                    CString str;
                    str = next;
                    PostfixExpression.push(str);
                    PostfixStack.pop();
                }
                PostfixStack.pop();
                break;
            }
            case OPERAND:
            {
                CString number;
                int index = nGetTypeOfSymbol(i, m_omFormula, number);
                PostfixExpression.push(number);
                i = index - 1;
                break;
            }
            case OPERATOR:
            {
                while (!PostfixStack.empty() &&
                        (nPrecedence(temp) <= nPrecedence(PostfixStack.top())))
                {
                    char ch = PostfixStack.top();
                    CString str;
                    str = ch;
                    PostfixExpression.push(str);
                    PostfixStack.pop();
                }
                PostfixStack.push(temp);
                break;
            }
        }
        i++;
    }
    while (!PostfixStack.empty())
    {
        char ch = PostfixStack.top();
        CString str;
        str = ch;
        PostfixExpression.push(str);
        PostfixStack.pop();
    }
    /*converting infix expr to postfix - end*/

    /*Evaluation of postfix Expression*/
    std::stack<float> s;
    float value, value1, value2;
    while (!PostfixExpression.empty())
    {
        CString token = PostfixExpression.front();
        PostfixExpression.pop();
        char ch = token.GetAt(0);
        if(isdigit(ch))
        {
            char* stopstring;
            value = strtod(token.GetBuffer(1024), &stopstring);
            s.push(value);
        }
        else
        {
            value2 = s.top();
            s.pop();
            value1 = s.top();
            s.pop();
            switch(ch)
            {
                case '+':
                {
                    value = value1 + value2;
                    break;
                }
                case '-':
                {
                    value = value1 - value2;
                    break;
                }
                case '*':
                {
                    value = value1 * value2;
                    break;
                }
                case '/':
                {
                    value = value1 / value2;
                    break;
                }
                case '%':
                {
                    value = (int)value1 % (int)value2;
                    break;
                }
                case '|':
                {
                    value = (int)value1 | (int)value2;
                    break;
                }
                case '&':
                {
                    value = (int)value1 & (int)value2;
                    break;
                }
            }
            s.push(value);
        }
    }
    value = s.top();
    s.pop();
    omEnggValue.Format("%lf", value);

    return omEnggValue;
}

/* STARTS DEFINITION OF CSignalDef */
CSignalDef::CSignalDef()
{
    m_unStartbit = 0;
    m_unLength = 0;
    m_omNameSignal = "";
    m_bIsBigEndian = FALSE;
    m_omUnit = "";
    m_omPhylVal = "";
    m_ouSigConstrnt.RemoveAll();
    m_ouCompuMethods.RemoveAll();

}

CSignalDef::CSignalDef(CSignalDef& RefObj)
{
    m_unStartbit = RefObj.m_unStartbit;
    m_unLength = RefObj.m_unLength;
    m_omNameSignal = RefObj.m_omNameSignal;
    m_bIsBigEndian = RefObj.m_bIsBigEndian;
    m_omUnit = RefObj.m_omUnit;
    m_ouSigConstrnt.Copy(RefObj.m_ouSigConstrnt);
    m_ouCompuMethods.Copy(RefObj.m_ouCompuMethods);
}

CSignalDef::~CSignalDef()
{
    m_ouCompuMethods.RemoveAll();
    m_ouSigConstrnt.RemoveAll();
}

CSignalDef& CSignalDef::operator =(const CSignalDef& RefObj)
{
    m_unStartbit = RefObj.m_unStartbit;
    m_unLength = RefObj.m_unLength;
    m_omNameSignal = RefObj.m_omNameSignal;
    m_bIsBigEndian = RefObj.m_bIsBigEndian;
    m_omUnit = RefObj.m_omUnit;
    m_ouSigConstrnt.Copy(RefObj.m_ouSigConstrnt);
    m_ouCompuMethods.Copy(RefObj.m_ouCompuMethods);

    return *this;
}

CString CSignalDef::omSearchValueFromCompuBlks(DWORD dwRawValue)
{
    CString omResult = "";
    bool bContinue = false, bResult = false;
    int nCnt = (int) m_ouCompuMethods.GetSize();
    for (int j = 0; (j < nCnt) && (!bResult); j++)
    {
        CCompuMethodEx& ouTempCompuMtd = m_ouCompuMethods.GetAt(j);
        bResult = ouTempCompuMtd.omGetEnggValue(dwRawValue, omResult);
        bContinue = bResult;
    }
    if (!bContinue) // No associated block found till now
    {
        omResult.Format("%f", (DOUBLE)dwRawValue); // default is IDENTICAL
    }

    return omResult;
}

tagSIG_INT_CONSTRAINTS_EX::tagSIG_INT_CONSTRAINTS_EX()
{
}

tagSIG_INT_CONSTRAINTS_EX::~tagSIG_INT_CONSTRAINTS_EX()
{
}

tagSIG_INT_CONSTRAINTS_EX& tagSIG_INT_CONSTRAINTS_EX::operator=(const tagSIG_INT_CONSTRAINTS_EX& Refobj)
{
    m_eValid = Refobj.m_eValid;
    m_sRange = Refobj.m_sRange;

    return *this;
}

tagSIG_INT_CONSTRAINTS_EX::tagSIG_INT_CONSTRAINTS_EX(tagSIG_INT_CONSTRAINTS_EX& Refobj)
{
    m_eValid = Refobj.m_eValid;
    m_sRange = Refobj.m_sRange;
}

UINT64 CSignalDef::un64GetRawValue(WORD* pwData)
{
    // First convert pData into a byte array
    CByteArray omMsgByte;

    int nTotalCount = (m_unLength / 2) + (m_unLength % 2);

    for (register INT nCount = 0; nCount < nTotalCount; nCount++)
    {
        WORD wCurrDat = *(pwData + nCount);
        omMsgByte.Add(LOBYTE(wCurrDat));
        omMsgByte.Add(HIBYTE(wCurrDat));
    }

    UINT64 un64SigVal = 0;
    UINT byteNumber = m_unStartbit / defBITS_IN_BYTE;
    UINT unBitNumber = m_unStartbit % defBITS_IN_BYTE;
    if ((m_unLength != 0) && (byteNumber < (UINT) omMsgByte.GetSize() - 1))
    {
        UCHAR byMsgByteVal = 0;
        UINT64 un64Temp = 1;
        UINT unSigLen = m_unLength;

        byMsgByteVal = omMsgByte.GetAt(byteNumber);

        //let the signal value be the LSB, ignore rest
        if (unBitNumber != 0)
        {
            byMsgByteVal >>= (unBitNumber );
        }

        un64Temp = 0x01;
        UINT totalCount = unSigLen;

        // check each bit in Message byte value if high- set corresponding
        // signal bit high loop until signal bit length
        register int nloopvar = defBITS_IN_BYTE - unBitNumber;
        while (nloopvar-- && (un64Temp != MAX_VAL_OF_BYTE) && totalCount)
        {
            if ((byMsgByteVal & un64Temp) !=0)
            {
                un64SigVal |= un64Temp;
            }
            un64Temp <<= 1;
            totalCount--;
        }

        // Special case : if the signal starts in betn the byte and extends
        // to next byte
        UINT ntempByteNo = byteNumber + 1;

        if ((totalCount > 0) && (ntempByteNo <= (UINT) omMsgByte.GetSize()))
        {
            do // get next byte
            {
                byMsgByteVal = omMsgByte.GetAt(ntempByteNo);
                UCHAR ucByteTemp = 1;
                // check each bit in Message byte value if high- set
                // corresponding signal bit high and loop until
                // signal bit length
                for (register int i = 0; (i < defMAX_BYTE) && totalCount; i++)
                {
                    totalCount--;
                    if ((byMsgByteVal & ucByteTemp) != 0)
                    {
                        un64SigVal |= un64Temp;
                    }
                    un64Temp <<= 1;
                    ucByteTemp <<=1;
                }
                ntempByteNo++;
            }
            while (totalCount > 0);
        }
    }

    return un64SigVal;
}

CString CSignalDef::omGetEnggValue(DWORD dwRawValue)
{
    CString RetStr = "";
    int nCnt = (int) m_ouSigConstrnt.GetSize();
    SIG_INT_CONSTRAINT_EX ouTempSigCnst;
    bool bContinue = true;
    for (int j = 0; (j < nCnt) && bContinue; j++)
    {
        ouTempSigCnst = m_ouSigConstrnt.GetAt(j);
        // CHECKING FOR THE RANGE AND SIGNAL CONSTRAINT VALIDITY
        if (ouTempSigCnst.m_sRange.bIsWithinRange(dwRawValue))
        {
            if (ouTempSigCnst.m_eValid == INVALID)
            {
                RetStr = "INVALID";
                bContinue = false;
            }
            else if (ouTempSigCnst.m_eValid == NOT_AVAILABLE)
            {
                RetStr = "NOT_AVAILABLE";
                bContinue = false;
            }
        }
    }

    if (bContinue)
    {
        // Get the engineering value as the signal constrint is valid
        RetStr = omSearchValueFromCompuBlks(dwRawValue);
    }

    return RetStr;
}

CFrameDef::CFrameDef ()
{
}

CFrameDef::CFrameDef (CFrameDef& RefObj)
{
    *this = RefObj;
}

CFrameDef::~CFrameDef()
{
    m_ouSignalArray.RemoveAll();
}

CFrameDef& CFrameDef::operator =(const CFrameDef& RefObj)
{
    m_unFrameID = RefObj.m_unFrameID;
    m_omNameFrame = RefObj.m_omNameFrame;
    m_ouSignalArray.RemoveAll();
    m_ouSignalArray.Copy(RefObj.m_ouSignalArray);

    return *this;
}

BOOL CFrameDef::bGetSignalInfoSet(WORD* pwData, SSignalInfoArray& sSignalInfoSet)
{
    BOOL bReturn = FALSE;
    int nCnt = (int) m_ouSignalArray.GetSize();
    for (int i = 0; i < nCnt; i++)
    {
        static CSignalDef ouTempSigDef;
        static SSignalInfo ouSignalInfo;

        ouTempSigDef = m_ouSignalArray.GetAt(i);

        if ( !ouTempSigDef.m_omNameSignal.IsEmpty())
        {
            ouSignalInfo.m_omSigName = ouTempSigDef.m_omNameSignal;
            ouSignalInfo.m_omUnit = ouTempSigDef.m_omUnit;
            // Get the raw value now
            DWORD dwRawValue = (DWORD) (ouTempSigDef.un64GetRawValue(pwData));
            ouSignalInfo.m_omRawValue.Format("%ld", dwRawValue);
            // Engineering value
            ouSignalInfo.m_omEnggValue = ouTempSigDef.omGetEnggValue(dwRawValue);

            sSignalInfoSet.Add(ouSignalInfo);
            bReturn = true;
        }
    }

    return bReturn;
}

void vCopyFrameData(CFrameMap& ouFrameTarget, const CFrameMap& ouFrameSource)
{
    vCleanFrameDataSet(ouFrameTarget); // First clean the target
    POSITION pos = ouFrameSource.GetStartPosition();
    while (pos != nullptr)
    {
        UINT m_unKey = 0x0;
        static CFrameDef ouTempFrameDef;
        // Get each CFrameMap element into Temporary placeholder
        ouFrameSource.GetNextAssoc(pos, m_unKey, ouTempFrameDef);
        // Finally update the target object
        ouFrameTarget.SetAt(ouTempFrameDef.m_unFrameID, ouTempFrameDef);
    }
}

int nGetFrameDataSet(CFrameMap& ouDataSet)
{
    CFrameDef temp;
    vCopyFrameData(ouDataSet, g_ouFrameDataSet);

    return 0;
}

BOOL bGetSignalInfoSet(CFrameMap& ouDataSet, UINT unFrameID, WORD* pwData,
                       CString& omFrameName, SSignalInfoArray& sSignalInfoSet)
{
    BOOL bResult = FALSE;
    static CFrameDef ouFrameDef;

    BOOL bRet = ouDataSet.Lookup(unFrameID, ouFrameDef);
    if (bRet)
    {
        omFrameName = ouFrameDef.m_omNameFrame;
        bResult = ouFrameDef.bGetSignalInfoSet(pwData, sSignalInfoSet);
    }

    return bResult;
}

BOOL bGetFrameName(CFrameMap& ouDataSet, UINT unFrameID, CString& omFrameName)
{
    BOOL bResult = FALSE;
    CFrameDef ouFrameDef;

    if (ouDataSet.Lookup(unFrameID, ouFrameDef))
    {
        omFrameName = ouFrameDef.m_omNameFrame;
    }

    return bResult;
}

void vCleanFrameDataSet(CFrameMap& ouDataSet)
{
    ouDataSet.RemoveAll();
}

//// SRange Functions
BOOL tagSRange::bIsWithinRange(DWORD dwValue)
{
    BOOL bReturn = FALSE;
    if ((m_dwLowerLimit == 0) && (m_dwUpperLimit == 0))
    {
        bReturn = TRUE;
    }
    else
    {
        bReturn = (m_dwLowerLimit <= dwValue) && (dwValue <= m_dwUpperLimit);
    }
    return bReturn;
}

tagSRange::tagSRange()
{
    m_dwLowerLimit = 0;
    m_dwUpperLimit = 0;
}

// tagScaleLinearCodeVarEx scale linear
tagScaleLinearCodeVarEx::tagScaleLinearCodeVarEx()
{
}

tagScaleLinearCodeVarEx& tagScaleLinearCodeVarEx::operator=(const tagScaleLinearCodeVarEx& /* RefObj */)
{
    return *this;
}

tagScaleLinearCodeVarEx::~tagScaleLinearCodeVarEx()
{
}

// tagTextCodeVarEx functions
tagTextCodeVarEx::tagTextCodeVarEx()
{
    m_aTextName = "";
}

tagTextCodeVarEx& tagTextCodeVarEx::operator=(const tagTextCodeVarEx& RefObj)
{
    m_sRange = RefObj.m_sRange;
    m_aTextName = RefObj.m_aTextName;
    return *this;
}

tagTextCodeVarEx::~tagTextCodeVarEx()
{
}

//tagTabCodeVarEx functions
tagTabCodeVarEx::tagTabCodeVarEx()
{
}

tagTabCodeVarEx& tagTabCodeVarEx::operator=(const tagTabCodeVarEx& RefObj)
{
    m_sRange    = RefObj.m_sRange;
    m_dPhysVal  = RefObj.m_dPhysVal;

    /*if (nullptr != RefObj.m_pNextTabCodeType)
    {
        if (nullptr == m_pNextTabCodeType)
        {
            m_pNextTabCodeType = new tagTabCodeVarEx;
            ASSERT(nullptr != m_pNextTabCodeType);
        }
        *(m_pNextTabCodeType) = *(RefObj.m_pNextTabCodeType);
    }*/
    return *this;
}

tagTabCodeVarEx::~tagTabCodeVarEx()
{
    /* if (nullptr != m_pNextTabCodeType)
     {
         delete m_pNextTabCodeType;
         m_pNextTabCodeType = nullptr;
     }*/
}

tagOperands::tagOperands()
{
    m_pNextOpearnd = nullptr;
}

tagOperands& tagOperands::operator=(const tagOperands& RefObj)
{
    m_cOpredandName = RefObj.m_cOpredandName;
    m_fOperandValue = RefObj.m_fOperandValue;

    if (nullptr != RefObj.m_pNextOpearnd)
    {
        if (nullptr == m_pNextOpearnd)
        {
            m_pNextOpearnd = new tagOperands;
            ASSERT(nullptr != m_pNextOpearnd);
        }
        *m_pNextOpearnd = *(RefObj.m_pNextOpearnd);
    }
    return *this;
}

tagOperands::~tagOperands()
{
    if (nullptr != m_pNextOpearnd)
    {
        delete m_pNextOpearnd;
        m_pNextOpearnd = nullptr;
    }
}
//
tagFormulaCodeVarEx::tagFormulaCodeVarEx()
{
    m_pFirstOperand = nullptr;
}

tagFormulaCodeVarEx& tagFormulaCodeVarEx::operator=(const tagFormulaCodeVarEx& RefObj)
{
    m_sRange    = RefObj.m_sRange;
    m_omFormula = RefObj.m_omFormula;
    if (nullptr != RefObj.m_pFirstOperand)
    {
        if (nullptr == m_pFirstOperand)
        {
            m_pFirstOperand = new OPERANDS;
            ASSERT(nullptr != m_pFirstOperand);
        }
        *m_pFirstOperand = *(RefObj.m_pFirstOperand);
    }
    return *this;
}

tagFormulaCodeVarEx::~tagFormulaCodeVarEx()
{
    if (nullptr != m_pFirstOperand)
    {
        delete m_pFirstOperand;
        m_pFirstOperand = nullptr;
    }
}

CString omSearchValueFromCompuBlks(SIGNAL_STRUCT& ouSignal, DWORD dwRawValue)
{
    CString omResult = "";
    bool bContinue = false, bResult = false;


    CCompuMethodEx& ouTempCompuMtd = ouSignal.m_ouCompuMethod;
    bContinue = ouTempCompuMtd.omGetEnggValue(dwRawValue, omResult);

    if (!bContinue) // No associated block found till now
    {
        omResult.Format("%f", (DOUBLE)dwRawValue); // default is IDENTICAL
    }

    return omResult;
}

std::string omGetEnggValue(SIGNAL_STRUCT& ouSignal, DWORD dwRawValue)
{
    // Get the engineering value as the signal constrint is valid
    return (LPCTSTR)omSearchValueFromCompuBlks(ouSignal, dwRawValue);
}

UINT64 un64GetRawValue( SIGNAL_STRUCT& ouStruct, CByteArray& omMsgByte)
{
    // First convert pData into a byte array
    UINT byteNumber = ouStruct.m_unStartbit / defBITS_IN_BYTE +1;
    UINT unBitNumber = ouStruct.m_unStartbit % defBITS_IN_BYTE;
    CMsgInterpretation ouMsgInterpretation;

    //TODO::Change the format type to comman type
    EFORMAT_DATA oeFormat;
    if ( ouStruct.m_ouEndianness == INTEL )
    {
        oeFormat = DATA_FORMAT_INTEL;
    }
    else
    {
        oeFormat = DATA_FORMAT_MOTOROLA;
    }

    //DataType
    BYTE bDataType = ouStruct.m_bDataType;

    return ouMsgInterpretation.n64GetSignalValue(&omMsgByte, byteNumber, unBitNumber, ouStruct.m_nLength, bDataType, oeFormat);
}


bool bGetSignalInfo(FRAME_STRUCT& ouFrame, unsigned char uchBytes[], int nByteSize, std::list<Flexray_SSIGNALINFO>& ouSignalInfoList, BOOL bIsHex)
{
    BOOL bReturn = FALSE;
    std::list<SIGNAL_STRUCT> ouSignalList;
    ouFrame.GetSignalList(ouSignalList);

    int nCnt = ouSignalList.size();
    std::list<SIGNAL_STRUCT>::iterator itrSignalList = ouSignalList.begin();
    char uchData[254];
    Flexray_SSIGNALINFO ouSignalInfo;
    ouSignalInfoList.clear();

    CByteArray omByteArray;
    for ( int i = 0 ; i < nByteSize; i++ )
    {
        omByteArray.Add(uchBytes[i]);
    }


    while ( itrSignalList != ouSignalList.end() )
    {
        //Unit
        ouSignalInfo.m_omUnit = itrSignalList->m_omUnit;

        //Name
        ouSignalInfo.m_omSigName = itrSignalList->m_strSignalName;

        //Raw
        DWORD dwRawValue = (DWORD) un64GetRawValue(*itrSignalList, omByteArray);

        if(bIsHex == FALSE)
        {
            if(itrSignalList->m_bDataType == CHAR_INT)
            {
                sprintf_s(uchData, "%d", dwRawValue);
            }
            else
            {
                sprintf_s(uchData, "%u", dwRawValue);
            }
        }
        else
        {
            sprintf_s(uchData, "%X", dwRawValue);
        }

        ouSignalInfo.m_omRawValue = uchData;
        //Eng
        ouSignalInfo.m_omEnggValue = omGetEnggValue (*itrSignalList,dwRawValue);

        ouSignalInfoList.push_back(ouSignalInfo);

        itrSignalList++;
    }
    return true;
}

void GetSignalNames(std::list<Flexray_SSIGNALINFO> lstSignalInfo, CStringList& lstSignalNames)
{
    std::list<Flexray_SSIGNALINFO>::iterator itrSigInfo = lstSignalInfo.begin();

    while(itrSigInfo != lstSignalInfo.end())
    {
        lstSignalNames.AddTail(itrSigInfo->m_omSigName.c_str());
        itrSigInfo++;
    }
}