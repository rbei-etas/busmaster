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
 * \file      MsgBufferConfigPage.h
 * \brief     This file contain the definition of CMsgBufferConfigPage
 * \author    Raja N
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * This file contain the definition of CMsgBufferConfigPage
 */

#if !defined(AFX_MSGBUFFERCONFIGPAGE_H__1359F520_F69A_4723_A3D4_FDC3C886E5D4__INCLUDED_)
#define AFX_MSGBUFFERCONFIGPAGE_H__1359F520_F69A_4723_A3D4_FDC3C886E5D4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MsgBufferConfigPage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMsgBufferConfigPage dialog

class CMsgBufferConfigPage : public CPropertyPage
{
    DECLARE_DYNCREATE(CMsgBufferConfigPage)

// Construction
public:
    CMsgBufferConfigPage();
    void vSetBufferSize(INT* pnBufferSize);
    virtual ~CMsgBufferConfigPage();

// Dialog Data
    //{{AFX_DATA(CMsgBufferConfigPage)
    enum { IDD = IDD_DLG_MSG_BUFFER_CONGIF };
private:
    UINT    m_unAppendSize;
    UINT    m_unOverWriteSize;
    UINT    m_unDisplayUpdateRate;
    INT*    m_pnBufferSize;
    //}}AFX_DATA


// Overrides
    // ClassWizard generate virtual function overrides
    //{{AFX_VIRTUAL(CMsgBufferConfigPage)
    public:
    virtual void OnOK();
    protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

// Implementation
protected:
    // Generated message map functions
    //{{AFX_MSG(CMsgBufferConfigPage)
    virtual BOOL OnInitDialog();
    afx_msg void OnCbtnSetDefault();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MSGBUFFERCONFIGPAGE_H__1359F520_F69A_4723_A3D4_FDC3C886E5D4__INCLUDED_)
