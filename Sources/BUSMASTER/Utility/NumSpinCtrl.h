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
 * @brief This file contain the definition of CNumSpinCtrl class
 * @author Ravi D.
 * @copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * This file contain the definition of CNumSpinCtrl class
 */

#pragma once

#include "afxcmn.h"

class CNumSpinCtrl : public CSpinButtonCtrl
{
public:
    CNumSpinCtrl();
    virtual ~CNumSpinCtrl();

    /**
     * To set the size of the data in terms of bits.
     * This is required to get 2s complement value
     */
    void vSetDataSize(short int nSize);

    /** To get the Min, Max and Step values */
    void vGetRangeAndDelta(double& dLower, double& dUpper, double& dDelta);

    /** To set the Min, Max and Step values */
    void vSetRangeAndDelta(double dLower, double dUpper, double dDelta);

    /** To get the int 64 Min, Max and Step values */
    void vGetRangeAndDelta( __int64& n64Lower, __int64& n64Upper,
                            __int64& n64delta);

    /** To set the int 64 Min, Max and Step values */
    void vSetRangeAndDelta( __int64 n64Lower, __int64 n64Upper,
                            __int64 n64Delta);
    void vSetRangeAndDelta( UINT64 n64Lower, UINT64 n64Upper,
                            __int64 n64Delta);

    /* Overridden function */
    int GetPos();

    /** To get double current position of the spin control */
    double dGetPos();

    /** To get int 64 current position of the spin control */
    __int64 n64GetPos();

    /** To set the new position */
    void SetPos(double val);

    /** To set int 64 value */
    void SetPos(__int64 val);

    /** To set the formatting string depending on the mode and type */
    void vSetFormatString (LPCTSTR lpszFormatString = nullptr);

    /** To set the type Signed or Not */
    void vSetSigned(BOOL bIsSigned);

    /** To set the type Int or Float */
    void vSetFloatNumber(BOOL bIsFloat);

    /** To conver string to __int64. The string value could be in Hex or dec */
    BOOL bConvertStringToInt64( CString omStrHexNo,
                                __int64& n64Value,
                                int nBase);

    void vSetValueForBuddy (UINT64 n64Val);

protected:
    virtual void PreSubclassWindow();

    /** To initialise default values */
    void vInitSpinCtrl();

    /** To set the position of the scroll bar. Scrollbar will take only int value */
    void vSetIntPos (double pos);

    /** Int 64 version */
    void vSetIntPos (__int64 n64Pos);

    /** To set the Int/ Float value to the Text control */
    void vSetValueForBuddy (double val);

    /** Int 64 version */
    void vSetValueForBuddy (__int64 val);

    afx_msg BOOL OnDeltaPos(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);

    DECLARE_MESSAGE_MAP()

protected:
    /** Minimum Value */
    double m_dMinVal;

    /** Maximun Value */
    double m_dMaxVal;

    /** Step value */
    double m_dDelta;

    /** Minimum Value */
    __int64 m_n64MinVal;

    /** Maximun Value */
    __int64 m_n64MaxVal;

    /** Step value */
    __int64 m_n64Delta;

    /** Minimum Value */
    UINT64 m_un64MinVal;

    /** Maximun Value */
    UINT64 m_un64MaxVal;

    /** Range in Int */
    UINT m_unIntRange;

    /** Format String */
    CString m_omStrFormat;

    /** Data Length in Bits */
    short int m_nDataLength;

    /** Signed or Unsigned */
    BOOL   m_bSigned;

    /** Is floating number or int 64 number */
    BOOL   m_bFloatingNumber;
};
