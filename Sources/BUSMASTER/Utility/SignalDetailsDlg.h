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
 * \file      SignalDetailsDlg.h
 * \brief     This header file contains the defintion of class
 * \author    Robin G.K.
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * This header file contains the defintion of class
 */

#pragma once
#include "Utils_stdafx.h"
#include <string>
#include "SignalDetails_resource.h"

struct SIGNAL_DETAILS
{
    std::string m_strSignalName="";
    std::string m_strType="";
    std::string m_strByteIndex="";
    std::string m_strStartBit="";
    std::string m_strLength="";
    std::string m_strMaxValue="";
    std::string m_strMinValue="";
    std::string m_strUnit = "";
    std::string m_strByteOrder="";
};
class CSignalDetailsDlg : public CDialog
{
    // Construction
public:
    CSignalDetailsDlg(SIGNAL_DETAILS SignalDetails,
                      CWnd* pParent /*=nullptr*/);
    enum { IDD = IDD_DLG_SIGNAL_DETAILS };


protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    virtual BOOL OnInitDialog();

private:
    SIGNAL_DETAILS m_ouSignalDetails;

    CString m_ouStrSignalName;
    CString m_ouStrType;
    CString m_ouStrByteIndex;
    CString m_ouStrStartBit;
    CString m_ouStrLength;
    CString m_ouStrMaxValue;
    CString m_ouStrMinValue;
    CString m_ouStrUnit;
    CString m_ouStrByteOrder;
};
