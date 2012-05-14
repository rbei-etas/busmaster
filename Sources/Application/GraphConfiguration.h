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
 * \file      GraphConfiguration.h
 * \brief     Interface file for CGraphConfiguration class
 * \author    Raja N
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Interface file for CGraphConfiguration class
 */

#pragma once

// For Graph Parameter Class definition
#include "GraphParameters.h"

class CGraphConfiguration : public CDialog
{
    // Construction
public:
    // Local instance of View Style parameter
    CGraphParameters m_omGraphParams;
    eTYPE_BUS m_eBusType;
    // standard constructor
    CGraphConfiguration(CWnd* pParent = NULL);

    // Dialog Data
    //{{AFX_DATA(CGraphConfiguration)
    enum { IDD = IDD_DLG_GRAPH_WINDOW_CONF };
    CColorSelector   m_omAxisColor;
    CColorSelector   m_omFrameColor;
    CColorSelector   m_omGridColor;
    CColorSelector   m_omPlotColor;
    int     m_nYGridCount;
    int     m_nXGridCount;
    int     m_nUpdateRate;
    int     m_nBufferSize;
    int     m_omFrameType;
    //}}AFX_DATA


    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CGraphConfiguration)
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

    // Implementation
protected:

    // Generated message map functions
    //{{AFX_MSG(CGraphConfiguration)
    afx_msg void OnSetDefault();
    virtual void OnOK();
    virtual BOOL OnInitDialog();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
private:
    // To set the values of the object in to UI control
    void vSetValues(CGraphParameters omParam);
public:
    int m_nLineDisplay;
};
