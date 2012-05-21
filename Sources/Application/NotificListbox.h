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
 * \file      Application/NotificListbox.h
 * \brief     Interface file for CNotificListbox class
 * \author    Ravikumar Patil
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Interface file for CNotificListbox class
 */


#if !defined(AFX_NOTIFICLISTBOX_H__44A75A71_6012_11D7_953E_000000000000__INCLUDED_)
#define AFX_NOTIFICLISTBOX_H__44A75A71_6012_11D7_953E_000000000000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// NotificListbox.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CNotificListbox window

class CNotificListbox : public CListBox
{
// Construction
public:
	CNotificListbox();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNotificListbox)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CNotificListbox();

	// Generated message map functions
protected:
	//{{AFX_MSG(CNotificListbox)
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnTraceWndClear();
	afx_msg void OnTraceWndDelete();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnNotificwndSelectall();
    afx_msg void OnNotificwndCopytoclipboard();
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NOTIFICLISTBOX_H__44A75A71_6012_11D7_953E_000000000000__INCLUDED_)
