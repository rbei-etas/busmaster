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
 * \file      RadixEdit.cpp
 * \brief     This file contains the implementation of CRadixEdit class
 * \authors   Soj Thomas, Ratnadip Choudhury, Amarnath Shastry
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * This file contains the implementation of CRadixEdit class
 */


// For all standard header file include
#include "Utils_stdafx.h"
// Interface file for CRadixEdit class
#include "RadixEdit.h"
#include "Utility/MultiLanguageSupport.h"

CRadixEdit::CRadixEdit()
{
    m_nBase         = BASE_HEXADECIMAL;
    m_bSigned       = true;
    m_n64Value      = 0;
    m_fValue        = 0.0;
    m_bFloatAllowed = false;
    m_nCtrlID       = -1;
}

CRadixEdit::~CRadixEdit()
{
}


BEGIN_MESSAGE_MAP(CRadixEdit, CEdit)
    ON_WM_CHAR()
    ON_CONTROL_REFLECT(EN_CHANGE, OnChange)
END_MESSAGE_MAP()


/******************************************************************************
 Function Name  :   OnChar

 Input(s)       :   -
 Output         :   -
 Functionality  :   Called by the frame work on keying a character. This
                    function filters the display on the edit control
                    depending on the flags set

 Member of      :   CRadixEdit
 Friend of      :

 Author(s)      :   Soj Thomas
 Date Created   :   08-03-2002
 Modifications  :   Ratnadip Choudhury, Amarnath Shastry
******************************************************************************/
void CRadixEdit::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
    bool bSkip = true;

    UINT unCurChar = toupper(nChar);

    switch (m_nBase)
    {
        case BASE_ALPHANUMERIC:
            // Accept '0-9' or backspace and a-z, A- Z
            if (
                ((unCurChar >= '0') && (unCurChar <= '9'))
                || (unCurChar == BACK_SPACE)
                ||((unCurChar >= 'a') && (unCurChar <= 'z'))
                ||((unCurChar >= 'A') && (unCurChar <= 'Z'))

            )
            {
                bSkip = false;
            }
        case BASE_DECIMAL:
            // Accept '0-9' or backspace
            if (
                ((unCurChar >= '0') && (unCurChar <= '9'))
                || (unCurChar == BACK_SPACE)
            )
            {
                bSkip = false;
            }
            // Do not accept '-' if not signed. Obviously, it should occur only
            // as the first character
            else if ((unCurChar == MINUS_SIGN) && m_bSigned)
            {
                int nStart, nEnd;
                GetSel(nStart, nEnd);
                if (nStart == 0)
                {
                    bSkip = false;
                }
            }
            // Do not accept '.' char if floatig point is not allowed.
            // Obviously this should occur only once
            else if ((unCurChar == DECIMAL_POINT) && (m_bFloatAllowed == true))
            {
                CString omBuf;
                GetWindowText(omBuf);
                if (omBuf.Find('.') == -1)
                {
                    bSkip = false;
                }
            }
            break;

        case BASE_HEXADECIMAL:
            // Accept '0-9' or 'A-F' or backspace
            if (
                ((unCurChar >= '0') && (unCurChar <= '9'))
                || (unCurChar == BACK_SPACE)
                || ((unCurChar >= 'A') && (unCurChar <= 'F'))
            )
            {
                bSkip = false;
            }
            // Do not accept '-' if not signed. Obviously, it should occur only
            // as the first character
            else if ((unCurChar == MINUS_SIGN) && m_bSigned)
            {
                int nStart, nEnd;
                GetSel(nStart, nEnd);
                if (nStart == 0)
                {
                    bSkip = false;
                }
            }
            // Do not accept '.' char if floatig point is not allowed.
            // Obviously this should occur only once
            else if ((unCurChar == DECIMAL_POINT) && (m_bFloatAllowed == true))
            {
                CString omBuf;
                GetWindowText(omBuf);
                if (omBuf.Find('.') == -1)
                {
                    bSkip = false;
                }
            }
            break;

        case BASE_OCTAL:
            // Accept '0-7' or backspace
            if (
                ((unCurChar >= '0') && (unCurChar <= '7'))
                || (unCurChar == BACK_SPACE)
            )
            {
                bSkip = false;
            }
            break;

        case BASE_BINARY:
            // Accept '0-1' or backspace
            if (
                ((unCurChar >= '0') && (unCurChar <= '1'))
                || (unCurChar == BACK_SPACE)
            )
            {
                bSkip = false;
            }
            break;

        default:
            break;
    }

    if (!bSkip)
    {
        CEdit::OnChar(unCurChar, nRepCnt, nFlags);

        WPARAM wParam = MAKELONG(m_nCtrlID, EN_CHANGE);
        GetOwner()->PostMessage(WM_COMMAND, wParam, (LPARAM) m_hWnd);
    }
}

/******************************************************************************
 Function Name  :   vSetBase

 Input(s)       :   int (base to be set)
 Output         :   -
 Functionality  :   Sets base of numbers.
 Member of      :   CRadixEdit
 Friend of      :

 Author(s)      :   Soj Thomas
 Date Created   :   08-03-2002
 Modifications  :   Soj Thomas, Added check for m_hWnd
                    Ratnadip Choudhury, Amarnath Shastry
 Date Modified  :   22.03.2002
******************************************************************************/
void CRadixEdit::vSetBase(int nBase)
{
    switch (nBase)
    {
        case BASE_ALPHANUMERIC:
            m_nBase = nBase;
            break;
        case BASE_HEXADECIMAL:
            m_nBase = nBase;
            break;
        case BASE_OCTAL:
            m_nBase = nBase;
            break;
        case BASE_BINARY:
            m_nBase = nBase;
            break;
        case BASE_DECIMAL:
            m_nBase = nBase;
            break;
        default:
            m_nBase = BASE_DECIMAL;
    }
}

/******************************************************************************
 Function Name  :   nGetBase

 Input(s)       :   void
 Output         :   int (currently selected base)
 Functionality  :   Call the function to get the base currently selected
 Member of      :   CRadixEdit
 Friend of      :

 Author(s)      :   Soj Thomas
 Date Created   :   08-03-2002
 Modifications  :   Ratnadip Choudhury, Amarnath Shastry
******************************************************************************/
int CRadixEdit::nGetBase()
{
    return m_nBase;
}

/******************************************************************************
 Function Name  :   OnChange

 Input(s)       :   -
 Output         :   -
 Functionality  :   Called by the frame work on keying a character. If floating
                    point is not allowed, then charactres until "." are
                    considered as valid.
 Member of      :   CRadixEdit
 Friend of      :

 Author(s)      :   Soj Thomas
 Date Created   :   08-03-2002
 Modifications  :   Ratnadip Choudhury, Amarnath Shastry
                    Rajesh Kumar 05.03.2003 : call strtol/strtoul based on sign
 Modifications  :   Amitesh Bharti, 05.06.2003,replace datatype for supporting
                    64bits to __int64
******************************************************************************/
void CRadixEdit::OnChange()
{
    int nBufLength = LineLength() + 1;
    CString     strBuffer;
    //char* acBuffer= new char[1786];

    // if (strBuffer != "")
    {
        GetWindowText(strBuffer);
        if (m_bFloatAllowed)
        {
            m_fValue = (float) _tstof(strBuffer.GetBuffer(0));
        }
        else
        {
            if(m_nBase == BASE_DECIMAL )
            {
                if(bIsSigned())
                {
                    m_n64Value = _tstoi64(strBuffer.GetBuffer(0));
                    m_un64Value = m_n64Value;
                }
                else
                {
                    m_un64Value = _strtoui64(strBuffer.GetBuffer(0), nullptr, 10);
                    m_n64Value = m_un64Value;
                }
            }
            else
            {
                INT nCurrentPost = 0;
                __int64 n64Val = 0;
                INT nActualLength = nBufLength - 1;
                BOOL bNegativeNumber = FALSE;
                m_n64Value = 0;

                while(nActualLength > nCurrentPost)
                {
                    if(strBuffer.GetAt(nCurrentPost) >='0'
                            && strBuffer.GetAt(nCurrentPost) <='9')
                    {
                        char cBuff = strBuffer.GetAt(nCurrentPost);
                        cBuff -='0';
                        strBuffer.SetAt(nCurrentPost, cBuff) ;
                    }
                    else if(strBuffer.GetAt(nCurrentPost) != '-')
                    {
                        char cBuff = strBuffer.GetAt(nCurrentPost);
                        cBuff = (char)(tolower(cBuff));
                        cBuff -= 87;
                        strBuffer.SetAt(nCurrentPost, cBuff) ;
                    }
                    else
                    {
                        bNegativeNumber = TRUE;
                    }
                    if(strBuffer.GetAt(nCurrentPost) != '-')
                    {
                        n64Val = strBuffer.GetAt(nCurrentPost);
                        n64Val <<= ((nActualLength - nCurrentPost - 1 )* 4 );
                        m_n64Value |= n64Val;
                    }
                    nCurrentPost++;
                }
                if(bNegativeNumber == TRUE )
                {
                    m_n64Value = -m_n64Value;
                }
            }
        }
        //delete []acBuffer;
        //acBuffer = nullptr;
    }
}

/******************************************************************************
 Function Name  :   lGetValue

 Input(s)       :   -
 Output         :   long
 Functionality  :   call this function to get the value if non-floating number
 Member of      :   CRadixEdit
 Friend of      :

 Author(s)      :   Soj Thomas
 Date Created   :   08-03-2002
 Modifications  :   Amitesh Bharti, 05.06.2003,replace datatype for supporting
                    64bits to __int64
******************************************************************************/
__int64 CRadixEdit::lGetValue()
{
    OnChange();
    return m_n64Value;
}

unsigned __int64 CRadixEdit::lGetUnsignedValue()
{
    OnChange();
    return m_un64Value;
}

/******************************************************************************
 Function Name  :   vSetValue

 Input(s)       :   long (value to set for non-floating number)
 Output         :   -
 Functionality  :   Sets long value passed as parameter and displays on the
                    edit control. At present binary conversion is not taken
                    care of.
 Member of      :   CRadixEdit
 Friend of      :

 Author(s)      :   Soj Thomas
 Date Created   :   08-03-2002
 Modifications  :   Ratnadip Choudhury, Amarnath Shastry
                :   Rajesh Kumar : 05.03.2003: formating changed to accomodate
                    signed and unsigned to full 32 bits
                 :  Amitesh Bharti, 05.06.2003,replace datatype for supporting
                    64bits to __int64
******************************************************************************/
void CRadixEdit::vSetValue(__int64 n64NewVal)
{
    // rajesh: 05.03.2003: BEGIN: formating changed slightly to accomodate
    // full 32 bits for signed and unsigned numbers
    // if the number is having negative value
    char acFormat[8] = {0};

    if (m_nBase == BASE_DECIMAL)
    {
        if(m_bSigned == true)
        {
            strcpy_s(acFormat, 8,  "%I64d");
        }
        else
        {
            strcpy_s(acFormat, 8,  "%I64u");
        }
    }
    else
    {
        if (m_bSigned == true && n64NewVal < 0)
        {
            strcpy_s(acFormat, 8, "-");
            n64NewVal = -n64NewVal;
        }

        if (m_nBase == BASE_HEXADECIMAL)
        {
            strcat_s(acFormat, 8, "%I64X");
        }
        else if (m_nBase == BASE_OCTAL)
        {
            strcat_s(acFormat, 8, "%I64o");
        }
    }
    // rajesh: 05.03.2003: END:

    m_n64Value = n64NewVal;
    CString omStr;
    omStr.Format(acFormat, n64NewVal);
    SetWindowText(omStr);
}

/******************************************************************************
 Function Name  :   vSetSigned

 Input(s)       :   bool
 Output         :   -
 Functionality  :
 Member of      :   CRadixEdit
 Friend of      :

 Author(s)      :   Amarnath Shastry
 Date Created   :   08-03-2002
 Modifications  :
 Date Modified  :
******************************************************************************/
void CRadixEdit::vSetSigned(bool bSigned)
{
    m_bSigned = bSigned;
}

/******************************************************************************
 Function Name  :   bIsSigned

 Input(s)       :   -
 Output         :   bool (true if signed values are allowed, else false)
 Functionality  :
 Member of      :   CRadixEdit
 Friend of      :

 Author(s)      :   Amarnath Shastry
 Date Created   :   08-03-2002
 Modifications  :
 Date Modified  :
******************************************************************************/
bool CRadixEdit::bIsSigned()
{
    return m_bSigned;
}

/******************************************************************************
 Function Name  :   vAcceptFloatingNum

 Input(s)       :   -
 Output         :   -
 Functionality  :   Allows accepting floting point number
 Member of      :   CRadixEdit
 Friend of      :

 Author(s)      :   Amarnath Shastry
 Date Created   :   08-03-2002
 Modifications  :
 Date Modified  :
******************************************************************************/
void CRadixEdit::vAcceptFloatingNum(bool bFloatAllow)
{
    m_bFloatAllowed = bFloatAllow;
}

/******************************************************************************
 Function Name  :   fGetValue

 Input(s)       :   -
 Output         :   float
 Functionality  :   Returns floating point value
 Member of      :   CRadixEdit
 Friend of      :

 Author(s)      :   Amarnath Shastry
 Date Created   :   08-03-2002
 Modifications  :
 Date Modified  :
******************************************************************************/
float CRadixEdit::fGetValue()
{
    return m_fValue;
}

void CRadixEdit::vSetConfigData(int CtrlID)
{
    m_nCtrlID = CtrlID;
}
