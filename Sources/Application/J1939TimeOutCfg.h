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
 * \file      J1939TimeOutCfg.h
 * \brief     Declaration file for CJ1939TimeOutCfg dialog
 * \author    Pradeep Kadoor
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Declaration file for CJ1939TimeOutCfg dialog
 */

#pragma once

#include "Utility/RadixEdit.h"

class CJ1939TimeOutCfg : public CDialog
{
    DECLARE_DYNAMIC(CJ1939TimeOutCfg)

public:
    CJ1939TimeOutCfg(CWnd* pParent = NULL);   // standard constructor
    virtual BOOL OnInitDialog();
    virtual ~CJ1939TimeOutCfg();

    // Dialog Data
    enum { IDD = IDD_TIMEOUT };

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

    DECLARE_MESSAGE_MAP()
public:
    CRadixEdit m_omBroadcast;
    CRadixEdit m_omResponse;
    CRadixEdit m_omHolding;
    CRadixEdit m_omT1;
    CRadixEdit m_omT2;
    CRadixEdit m_omT3;
    CRadixEdit m_omT4;
    afx_msg void OnBnClickedCheckBroadcast();
    afx_msg void OnBnClickedCheckResponse();
    afx_msg void OnBnClickedCheckHolding();
    afx_msg void OnBnClickedCheckT1();
    afx_msg void OnBnClickedCheckT2();
    afx_msg void OnBnClickedCheckT3();
    afx_msg void OnBnClickedCheckT4();
    afx_msg void OnEnChangeEditBroadcast();
    afx_msg void OnEnChangeEditResponse();
    afx_msg void OnEnChangeEditHolding();
    afx_msg void OnEnChangeEditT1();
    afx_msg void OnEnChangeEditT2();
    afx_msg void OnEnChangeEditT3();
    afx_msg void OnEnChangeEditT4();
    afx_msg void OnBnClickedOk();
};
