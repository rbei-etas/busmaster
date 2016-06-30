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
 * \file      SignalDetailsDlg.cpp
 * \brief     This file contain definition of all function of
 * \author    Robin G.K.
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * This file contain definition of all function of
 */
#pragma once
#include "SignalDetailsDlg.h"

/*******************************************************************************
 Function Name    :  CSignalDetailsDlg
 Input(s)         :  eMode - Mode of the View
                     psSigInfo - Pointer to the Signal Information
                     pParent   - Pointer to the Parent Window
 Output           :   -
 Functionality    :  Constructor
 Member of        :  CSignalDetailsDlg
 Friend of        :      -
 Author(s)        :  Raja N
 Date Created     :  22.07.2004
 Modifications    :  Raja N on 31.07.2004, Modifications as per code review
                     comments and changed code with utility class function calls
/******************************************************************************/
CSignalDetailsDlg::CSignalDetailsDlg(SIGNAL_DETAILS ouSignalDetails,
                                     CWnd* pParent /*=nullptr*/)
    : CDialog(CSignalDetailsDlg::IDD, pParent) , m_ouSignalDetails(ouSignalDetails)
{

}

void CSignalDetailsDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_EDIT_SGNAME, m_ouStrSignalName);
    DDX_Text(pDX, IDC_EDIT_TYPE, m_ouStrType);
    DDX_Text(pDX, IDC_EDIT_BYINDEX, m_ouStrByteIndex);
    DDX_Text(pDX, IDC_EDIT_STBIT, m_ouStrStartBit);
    DDX_Text(pDX, IDC_EDIT_SGLEN, m_ouStrLength);
    DDX_Text(pDX, IDC_EDIT_MAX, m_ouStrMaxValue);
    DDX_Text(pDX, IDC_EDIT_MIN, m_ouStrMinValue);
    DDX_Text(pDX, IDC_EDIT_UNIT, m_ouStrUnit);
    DDX_Text(pDX, IDC_EDIT_BYTE_ORDER, m_ouStrByteOrder);
}

BOOL CSignalDetailsDlg::OnInitDialog()
{
    m_ouStrSignalName = m_ouSignalDetails.m_strSignalName.c_str();
    m_ouStrType = m_ouSignalDetails.m_strType.c_str();
    m_ouStrByteIndex = m_ouSignalDetails.m_strByteIndex.c_str();
    m_ouStrStartBit = m_ouSignalDetails.m_strStartBit.c_str();
    m_ouStrLength = m_ouSignalDetails.m_strLength.c_str();
    m_ouStrMaxValue = m_ouSignalDetails.m_strMaxValue.c_str();
    m_ouStrMinValue = m_ouSignalDetails.m_strMinValue.c_str();
    m_ouStrUnit = m_ouSignalDetails.m_strUnit.c_str();
    m_ouStrByteOrder = m_ouSignalDetails.m_strByteOrder.c_str();
    UpdateData(FALSE);
    return TRUE;
}



