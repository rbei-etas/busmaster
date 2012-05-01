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
 * \file      SignalDefinerDlg.h
 * \brief     Main header file for the SignalDefinerDialog
 * \author    Arunkumar Karri
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Main header file for the SignalDefinerDialog
 */

#pragma once

#include "SignalDefiner_Resource.h"
#include "afxwin.h"
// CSignalDefinerDlg dialog

class CSignalDefinerDlg : public CDialog
{
    DECLARE_DYNAMIC(CSignalDefinerDlg)

public:
    CSignalDefinerDlg(CWnd* pParent = NULL);   // standard constructor
    virtual ~CSignalDefinerDlg();

    // Dialog Data
    enum { IDD = IDD_SIGNAL_DEFINER_DIALOG };

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

    DECLARE_MESSAGE_MAP()
public:
    float m_fAmplitude;
    float m_fFrequency;
    int m_nSelCycle;
    double m_dblSamplingTimePeriod;
    BSTR m_strSignalName;
    CComPtr<IDMGraphCollection> spElements;

    afx_msg void OnCbnSelchangeComboSignalType();
    afx_msg void OnCbnSelchangeComboCycles();
    afx_msg void OnEnChangeEditSignalAmplitude();
    afx_msg void OnEnChangeEditSignalFrequency();
    afx_msg void OnEnChangeEditSignalSamplingTime();
    afx_msg void OnBnClickedOk();

    virtual BOOL OnInitDialog();

    CComboBox m_ctrSignalType;
    CComboBox m_ctrNoOfCycles;

    CWnd* m_poDMGraphCtrl;
    int m_nSignalType;

private:

    void vGenerateWave();
    void SetGraphData(VARIANT* pvarrX, VARIANT* pvarrY);
};
