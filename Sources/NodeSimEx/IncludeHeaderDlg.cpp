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
 * \file      IncludeHeaderDlg.cpp
 * \brief     This file contain definition of all function of
 * \author    Ratnadip Choudhury
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * This file contain definition of all function of
 */

#include "NodeSimEx_stdafx.h"   // Standard header file
#include "HashDefines.h"        // All #defines are here
// MDI mainframe definition file
#include "FunctionEditorDoc.h"  // Document class definition file
#include "GlobalObj.h"
#include "IncludeHeaderDlg.h"   // Class definition file

/******************************************************************************/
/*  Function Name    :  CIncludeHeaderDlg                                     */
/*                                                                            */
/*  Input(s)         :                                                        */
/*  Output           :                                                        */
/*  Functionality    :  Constructor
/*  Member of        :  CIncludeHeaderDlg                                      */
/*  Friend of        :      -                                                 */
/*                                                                            */
/*  Author(s)        :  Amarnath Shastry                                      */
/*  Date Created     :  13.03.2002                                            */
/*  Modifications    :
/******************************************************************************/
CIncludeHeaderDlg::CIncludeHeaderDlg(CFunctionEditorDoc* pDoc, CWnd* pParent /*=NULL*/)
    : CDialog(CIncludeHeaderDlg::IDD, pParent)
{
    //{{AFX_DATA_INIT(CIncludeHeaderDlg)
    m_omStrIncludeFilename = "";
    //}}AFX_DATA_INIT
    m_pDoc = pDoc;
}
void CIncludeHeaderDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CIncludeHeaderDlg)
    DDX_Text(pDX, IDC_EDIT_FILENAME, m_omStrIncludeFilename);
    //}}AFX_DATA_MAP
}
BEGIN_MESSAGE_MAP(CIncludeHeaderDlg, CDialog)
    //{{AFX_MSG_MAP(CIncludeHeaderDlg)
    ON_BN_CLICKED(IDC_CBTN_OK, OnOk)
    ON_BN_CLICKED(IDC_CBTN_BROWSE, OnBrowse)
    ON_BN_CLICKED(IDC_CBTN_CANCEL, OnCancel)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CIncludeHeaderDlg message handlers
/******************************************************************************/
/*  Function Name    :  OnOk                                                  */
/*                                                                            */
/*  Input(s)         :                                                        */
/*  Output           :                                                        */
/*  Functionality    :  validates the input string and calls the
                        base class function
/*  Member of        :  CIncludeHeaderDlg                                      */
/*  Friend of        :      -                                                 */
/*                                                                            */
/*  Author(s)        :  Amarnath Shastry                                      */
/*  Date Created     :  13.03.2002                                            */
/*  Modifications    :  Anish kumar
                        20.06.06.get the present Doc view to include the header
                        file.
/******************************************************************************/
void CIncludeHeaderDlg::OnOk()
{
    BOOL bRetVal = TRUE;
    UpdateData(TRUE);

    if (!m_omStrIncludeFilename.IsEmpty())
    {
        // check if first char starts with '<' or '"'
        char cFirstChar = m_omStrIncludeFilename.GetAt( 0 );
        // check if last char '>' or '"'
        char cLastChar =
            m_omStrIncludeFilename.GetAt( m_omStrIncludeFilename.GetLength() - 1 );

        if ( cFirstChar != '<')
        {
            if ( cFirstChar != '"')
            {
                bRetVal = FALSE;
            }
            else if ( cLastChar != '"')
            {
                bRetVal = FALSE;
            }
        }
        else if ( cLastChar != '>')
        {
            bRetVal = FALSE;
        }

        if (bRetVal == FALSE)
        {
            AfxMessageBox(_T("Please make sure to include the filename in between\n\
 double quotes or in angle brackets appropriately.."),
                          MB_OK|MB_ICONINFORMATION);
        }
        else
        {
            // Get present view

            //POSITION pos = CGlobalObj::ouGetObj(m_eBus).m_pEditorDocTemplate->GetFirstDocPosition();
            //CFunctionEditorDoc* pDoc = (CFunctionEditorDoc*)CGlobalObj::ouGetObj(m_eBus).m_pEditorDocTemplate->GetNextDoc(pos);
            if ( m_pDoc != NULL )
            {
                // Get include header array
                CStringArray* pArray =
                    m_pDoc->omStrGetIncudeHeaderFiles();

                if (pArray != NULL)
                {
                    CString omStrTemp = STR_EMPTY;

                    for (int nCount = 0;
                            nCount < pArray->GetSize();
                            nCount++)
                    {
                        omStrTemp = pArray->GetAt(nCount);

                        if ( omStrTemp.Find(m_omStrIncludeFilename) !=
                                -1 )
                        {
                            AfxMessageBox( _T("File already included!"),
                                           MB_OK|MB_ICONINFORMATION);
                            bRetVal = FALSE;
                            nCount = (COMMANINT)pArray->GetSize();// exit loop
                        }
                    }
                }
            }
        }
    }
    else
    {
        AfxMessageBox(_T("Invalid Include file!"), MB_OK|MB_ICONINFORMATION);
        bRetVal = FALSE;
    }

    if (bRetVal == TRUE)
    {
        CDialog::OnOK();
    }
    else
    {
        GetDlgItem(IDC_EDIT_FILENAME)->SetFocus();
    }
}
/******************************************************************************/
/*  Function Name    :  OnBrowse                                              */
/*                                                                            */
/*  Input(s)         :                                                        */
/*  Output           :                                                        */
/*  Functionality    :  Displays open file dialog and fills the edit
                        with selected header file
/*  Member of        :  CIncludeHeaderDlg                                      */
/*  Friend of        :      -                                                 */
/*                                                                            */
/*  Author(s)        :  Amarnath Shastry                                      */
/*  Date Created     :  13.03.2002                                            */
/*  Modifications    :  22.03.2002,Amarnath Shastry
                        Changed the title of the open dialog box
/******************************************************************************/

void CIncludeHeaderDlg::OnBrowse()
{
    //AFX_MANAGE_STATE(AfxGetStaticModuleState());
    // Display open file dialog
    // which will allow the user to select
    // only .h files
    CFileDialog om_Dlg
    (
        TRUE,  //open an existing file
        NULL,  //extension to file
        STR_EMPTY,  //initial file name
        OFN_FILEMUSTEXIST| OFN_HIDEREADONLY|
        OFN_PATHMUSTEXIST,
        _T("C Header Files(*.h)|*.h||"),
        NULL
    );
    // Set caption text
    om_Dlg.m_ofn.lpstrTitle = _T("Select Header File");

    if ( om_Dlg.DoModal() == IDOK )
    {
        // Display selected file name
        m_omStrIncludeFilename = om_Dlg.GetPathName();
        m_omStrIncludeFilename.Insert( 0, '"' );
        m_omStrIncludeFilename += "\"";
        UpdateData(FALSE);
    }
}
/******************************************************************************/
/*  Function Name    :  OnCancel                                              */
/*                                                                            */
/*  Input(s)         :                                                        */
/*  Output           :                                                        */
/*  Functionality    :  Calls base class function
/*  Member of        :  CIncludeHeaderDlg                                      */
/*  Friend of        :      -                                                 */
/*                                                                            */
/*  Author(s)        :  Amarnath Shastry                                      */
/*  Date Created     :  13.03.2002                                            */
/*  Modifications    :
/******************************************************************************/
void CIncludeHeaderDlg::OnCancel()
{
    CDialog::OnCancel();
}
