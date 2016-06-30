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
 * \file      DatabaseDissociateDlg.cpp
 * \brief     Contains the description of the class CDatabaseDissociateDlg
 * \author    Ratnadip Choudhury
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Contains the description of the class CDatabaseDissociateDlg
 */
#include "stdafx.h"
#include "BUSMASTER.h"
#include "DatabaseDissociateDlg.h"
#include "MainFrm.h"

extern CCANMonitorApp theApp;

// CDatabaseDissociateDlg dialog

IMPLEMENT_DYNAMIC(CDatabaseDissociateDlg, CDialog)
CDatabaseDissociateDlg::CDatabaseDissociateDlg( std::list<std::string> dbPath, CWnd* pParent )
    : CDialog(CDatabaseDissociateDlg::IDD, pParent)
{
    mDbPaths = dbPath;
}

CDatabaseDissociateDlg::~CDatabaseDissociateDlg()
{
}

void CDatabaseDissociateDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_LSTB_DISSOCIATE_DBNAMES, m_omDissociateDbLst);
}


BEGIN_MESSAGE_MAP(CDatabaseDissociateDlg, CDialog)
    ON_BN_CLICKED(IDC_CBTN_DISSOCIATE, OnBnClickedCbtnDissociate)
    ON_LBN_SELCHANGE(IDC_LSTB_DISSOCIATE_DBNAMES, OnlbnSelChangeDbList)
    ON_WM_CLOSE()
END_MESSAGE_MAP()


/******************************************************************************
  Function Name    :  OnInitDialog

  Input(s)         :
  Output           :
  Functionality    :  Initialze the contents of List box
  Member of        :  CDatabaseDissociateDlg
  Friend of        :      -

  Author(s)        :  Anish Kumar
  Date Created     :  06.12.2006
  Modifications    :
******************************************************************************/
BOOL CDatabaseDissociateDlg::OnInitDialog()
{
    CDialog::OnInitDialog();
    //do initialization below
    m_omDissociateDbLst.ResetContent();
    mDbPathsDissociated.clear();
    CSize   sz;
    CDC*  pDC = m_omDissociateDbLst.GetDC();
    int nDx = 0;
for ( auto path : mDbPaths )
    {
        m_omDissociateDbLst.AddString( path.c_str() );
        sz = pDC->GetTextExtent( path.c_str() );

        if ( sz.cx > nDx )
        {
            nDx = sz.cx;
        }
    }


    m_omDissociateDbLst.ReleaseDC(pDC);
    // Set the horizontal extent so every character of all strings
    // can be scrolled to.
    m_omDissociateDbLst.SetHorizontalExtent(nDx);
    return TRUE;
}

/******************************************************************************
  Function Name    :  OnBnClickedCbtnDissociate

  Input(s)         :
  Output           :
  Functionality    :  Call the functions to remove the selected Databases
  Member of        :  CDatabaseDissociateDlg
  Friend of        :      -

  Author(s)        :  Anish Kumar
  Date Created     :  06.12.2006
  Modifications    :
******************************************************************************/
void CDatabaseDissociateDlg::OnBnClickedCbtnDissociate()
{
    int nCount = m_omDissociateDbLst.GetSelCount();
    if (nCount > 0)
    {
        if (IDOK == AfxMessageBox(_(defDISSOCIATE_WARNING), MB_OKCANCEL | MB_ICONWARNING))
        {
            // Array of selected item's position
            CArray<int, int> aomListBoxSel;
            aomListBoxSel.SetSize(nCount);
            //Pass the array pointer to get the selected item's positions
            m_omDissociateDbLst.GetSelItems(nCount, aomListBoxSel.GetData());
            int nSelectedPos = 0;
            for (int nTempCnt = 0; nTempCnt < nCount; nTempCnt++)
            {
                CString omstrDBPath;
                //Selected file's index
                nSelectedPos = aomListBoxSel.GetAt(nTempCnt);
                //Find the length of string to pass the buffer to have the selected File path
                int nBufferSize = m_omDissociateDbLst.GetTextLen(nSelectedPos);
                m_omDissociateDbLst.GetText(nSelectedPos, omstrDBPath.GetBuffer(nBufferSize));
                mDbPathsDissociated.push_back(omstrDBPath.GetBuffer(0));
            }
            for (int nTempCnt = 0; nTempCnt < nCount; nTempCnt++)
            {
                nSelectedPos = aomListBoxSel.GetAt(nTempCnt) - nTempCnt;
                m_omDissociateDbLst.DeleteString(nSelectedPos);
            }

            CButton* pButton = (CButton*)GetDlgItem(IDC_CBTN_DISSOCIATE);
            if (nullptr != pButton)
            {
                pButton->EnableWindow(FALSE);
            }
        }
    }


}
void CDatabaseDissociateDlg::OnOK()
{
    if (mDbPathsDissociated.size() > 0)
    {
        CDialog::OnOK();
    }
    else
    {
        CDialog::OnCancel();
    }
}
void CDatabaseDissociateDlg::OnClose()
{
    OnOK();
}
std::list<std::string> CDatabaseDissociateDlg::GetDissociatedFiles()
{
    return mDbPathsDissociated;
}
void CDatabaseDissociateDlg::OnlbnSelChangeDbList()
{
    CButton* pButton = (CButton*)GetDlgItem(IDC_CBTN_DISSOCIATE);
    if (nullptr != pButton)
    {
        if (m_omDissociateDbLst.GetSelCount() > 0)
        {
            pButton->EnableWindow(TRUE);
        }
        else
        {
            pButton->EnableWindow(FALSE);
        }
    }

}
