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
 * \file      SplFileDlg.cpp
 * \brief     This file contain the definition  of member function of
 * \author    Amitesh Bharti
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * This file contain the definition  of member function of
 */
#include "NodeSimEx_stdafx.h"
#include "SplFileDlg.h"

IMPLEMENT_DYNAMIC(CSplFileDlg, CFileDialog)

/******************************************************************************/
/*  Function Name    :  CSplFileDlg                                           */
/*  Input(s)         : parameter to be passed to CFileDialog constuctor and   */
/*                     omStrButtonCaption paramter for caption of buttin text */
/*  Output           :                                                        */
/*  Functionality    :  This is default constructor. This will initialise     */
/*                      parent class constructor with parameter pass          */
/*                                                                            */
/*  Member of        :  CSplFileDlg                                           */
/*  Friend of        :      -                                                 */
/*  Author(s)        :  Amitesh Bharti                                        */
/*  Date Created     :  22.03.2002                                            */
/*  Modification By  :                                                        */
/*  Modification on  :                                                        */
/******************************************************************************/
CSplFileDlg::CSplFileDlg(BOOL bOpenFileDialog,
                         LPCTSTR lpszDefExt,
                         LPCTSTR lpszFileName,
                         DWORD dwFlags,
                         LPCTSTR lpszFilter,
                         CWnd* pParentWnd,
                         CString omStrButtonCaption) :
    CFileDialog(bOpenFileDialog,lpszDefExt,lpszFileName,dwFlags,lpszFilter,
                pParentWnd),m_omStrButtonCaption(omStrButtonCaption)
{
}


BEGIN_MESSAGE_MAP(CSplFileDlg, CFileDialog)
    //{{AFX_MSG_MAP(CSplFileDlg)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/******************************************************************************/
/*  Function Name    :  OnInitDialog                                          */
/*  Input(s)         :  TRUE or FALSE                                         */
/*  Output           :                                                        */
/*  Functionality    :  This function is called by frame work before just     */
/*                      after  DoModal is called. This will set the text      */
/*                      caption of button to the text in m_omStrButtonCaption */
/*                                                                            */
/*  Member of        :  CSplFileDlg                                           */
/*  Friend of        :      -                                                 */
/*  Author(s)        :  Amitesh Bharti                                        */
/*  Date Created     :  22.03.2002                                            */
/*  Modification By  :  Amitesh Bharti                                        */
/*  Modification on  :  12.04.2002, review comment added                      */
/******************************************************************************/
BOOL CSplFileDlg::OnInitDialog()
{
    CFileDialog::OnInitDialog();
    GetParent()->GetDlgItem(IDOK)->SetWindowText(m_omStrButtonCaption);
    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}
