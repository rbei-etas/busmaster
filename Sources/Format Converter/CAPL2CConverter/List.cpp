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
 * \file      List.cpp
 * \brief     Implementation file for CList class
 * \author    Amit Ranjan
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Implementation file for CList class
 */

/* Project includes */
#include "List.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// List dialog


List::List(CWnd* pParent /*=NULL*/)
    : CDialog(List::IDD, pParent)
{
    //{{AFX_DATA_INIT(List)
    // NOTE: the ClassWizard will add member initialization here
    //}}AFX_DATA_INIT
//	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}


void List::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(List)
    DDX_Control(pDX, IDC_LSTB_UNSK, m_UnSupKeys);
    DDX_Control(pDX, IDC_LSTB_AVLK, m_AltKeys);
    DDX_Control(pDX, IDC_LSTB_ALTK, m_EquiKeys);
    //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(List, CDialog)
    //{{AFX_MSG_MAP(List)
    ON_BN_CLICKED(IDC_CBTN_ADD, OnAdd)
    ON_BN_CLICKED(IDC_CBTN_OK, OnTerminate)
    ON_BN_CLICKED(IDC_CBTN_RMV, OnRemove)
    ON_WM_CLOSE()
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// List message handlers







