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
 * \file      RadixEdit.h
 * \brief     This file contains the definition of CRadixEdit class     
 * \authors   RBIN/EMC2 - Soj Thomas, Ratnadip Choudhury, Amarnath Shastry
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * This file contains the definition of CRadixEdit class     
 */

#pragma once

static const int BASE_HEXADECIMAL  = 0x10;
static const int BASE_DECIMAL      = 0x0A;
static const int BASE_OCTAL        = 0x08;
static const int BASE_BINARY       = 0x02;
static const int BASE_ALPHANUMERIC = 0xFF;

static const int MINUS_SIGN        = 0x2D;
static const int BACK_SPACE        = 0x08;
static const int DECIMAL_POINT     = 0x2E;


/////////////////////////////////////////////////////////////////////////////
// CRadixEdit window

class CRadixEdit : public CEdit
{
// Construction
public:
    CRadixEdit();

// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CRadixEdit)
    //}}AFX_VIRTUAL

    void vSetConfigData(int CtrlID);
    // Returns floating point value
    float fGetValue();
    // Allow flaoting point number
    void vAcceptFloatingNum(bool);
    // Set long value
    void vSetValue(__int64 n64NewVal);
    // Get long value
    __int64 lGetValue();
    // Set number base 
    void vSetBase(int nBase);
    // Get base
    int  nGetBase();
    // Set signed / unsigned
    void vSetSigned(bool bSigned = true);
    // Check for signed integer
    bool bIsSigned();

    virtual ~CRadixEdit();

    // Generated message map functions
protected:
    //{{AFX_MSG(CRadixEdit)
    afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
    afx_msg void OnChange();
    //}}AFX_MSG

    DECLARE_MESSAGE_MAP()

private:
    float m_fValue;
    __int64 m_n64Value;
    int  m_nBase;
    bool m_bSigned;
    bool m_bFloatAllowed;
    int m_nCtrlID;
};
