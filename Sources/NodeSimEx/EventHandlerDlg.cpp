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
 * \file      EventHandlerDlg.cpp
 * \author    Pradeep Kadoor
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 */
// EventHandlerDlg.cpp : implementation file
//

#include "NodeSimEx_stdafx.h"
#include "NodeSimEx_Resource.h"
#include "Include/DIL_CommonDefs.h"
#include "Hashdefines.h"
#include "FunctionEditorDoc.h"
#include "EventHandlerDlg.h"


// CEventHandlerDlg dialog

IMPLEMENT_DYNAMIC(CEventHandlerDlg, CDialog)
CEventHandlerDlg::CEventHandlerDlg(CTreeCtrl& omTreeCtrl, CFunctionEditorDoc* pDoc, CWnd* pParent /*=NULL*/)
    : CDialog(CEventHandlerDlg::IDD, pParent),
      m_omTreeCtrl(omTreeCtrl)
{
    m_pDoc = pDoc;
}

CEventHandlerDlg::~CEventHandlerDlg()
{
}

void CEventHandlerDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CEventHandlerDlg, CDialog)
    ON_BN_CLICKED(IDOK, OnBnClickedOk)
END_MESSAGE_MAP()


// CEventHandlerDlg message handlers
BOOL CEventHandlerDlg::OnInitDialog()
{
    BOOL bAvailable = TRUE;
    /* Data conf event*/
    CButton* pButton = (CButton*)GetDlgItem(IDC_CHECK_DATA_CONF);
    pButton->SetCheck(BST_CHECKED);
    pButton->EnableWindow(FALSE);
    /* Address claim */
    pButton = (CButton*)GetDlgItem(IDC_CHECK_ADDRESS_CLAIM);
    pButton->SetCheck(BST_CHECKED);
    pButton->EnableWindow(FALSE);
    pButton = (CButton*)GetDlgItem(IDOK);
    pButton->EnableWindow(FALSE);

    if (!bIsEventHandlerExist(_T("DataConf")))
    {
        CButton* pButton = (CButton*)GetDlgItem(IDC_CHECK_DATA_CONF);
        pButton->SetCheck(BST_UNCHECKED);
        pButton->EnableWindow(TRUE);
        //Enable Ok button
        pButton = (CButton*)GetDlgItem(IDOK);
        pButton->EnableWindow(TRUE);
    }

    if (!bIsEventHandlerExist(_T("AddressClaim")))
    {
        CButton* pButton = (CButton*)GetDlgItem(IDC_CHECK_ADDRESS_CLAIM);
        pButton->SetCheck(BST_UNCHECKED);
        pButton->EnableWindow(TRUE);
        //Enable Ok button
        pButton = (CButton*)GetDlgItem(IDOK);
        pButton->EnableWindow(TRUE);
    }

    return TRUE;
}
void CEventHandlerDlg::vAddEventHandlerFunction(const CString& omStrFuncName, const CString& omStrFuncPrototype)
{
    VALIDATE_POINTER_RETURN_VOID(m_pDoc);
    SBUS_SPECIFIC_INFO sBusSpecInfo;
    m_pDoc->bGetBusSpecificInfo(sBusSpecInfo);
    // Form the function header
    CString omStrHdr = BUS_FN_HDR;
    omStrHdr.Replace(_T("PLACE_HODLER_FOR_BUSNAME"), sBusSpecInfo.m_omBusName);
    omStrHdr.Replace(_T("PLACE_HODLER_FOR_FUNCTIONNAME"), omStrFuncName);
    // Form the function footer
    CString omStrFooter = BUS_FN_FOOTER;
    omStrFooter.Replace(_T("PLACE_HODLER_FOR_BUSNAME"), sBusSpecInfo.m_omBusName);
    omStrFooter.Replace(_T("PLACE_HODLER_FOR_FUNCTIONNAME"), omStrFuncName);
    m_pDoc->m_omSourceCodeTextList.AddTail( omStrHdr ); //Function header
    m_pDoc->m_omSourceCodeTextList.AddTail(omStrFuncPrototype );// Function definition
    // Function body
    m_pDoc->m_omSourceCodeTextList.AddTail( "{" );
    m_pDoc->m_omSourceCodeTextList.AddTail( defTODO );
    m_pDoc->m_omSourceCodeTextList.AddTail( omStrFooter ); // Function footer
}

BOOL CEventHandlerDlg::bIsEventHandlerExist(const CString& omStrFuncType)
{
    VALIDATE_POINTER_RETURN_VAL(m_pDoc, FALSE);
    BOOL bExist = FALSE;
    CString omFuncToCheck;
    omFuncToCheck = defEVENT_IND_HANDLER + omStrFuncType;
    CStringArray* pEventHandlerArray = m_pDoc->omStrGetEventIndPrototypes();
    UINT unCount = (COMMANUINT)pEventHandlerArray->GetSize();

    for (UINT i = 0; i < unCount; i++)
    {
        CString omFns = pEventHandlerArray->GetAt(i);

        if (omFns.Find(omFuncToCheck, 0) > -1)
        {
            bExist = TRUE;
            i = unCount;
        }
    }

    return bExist;
}

void CEventHandlerDlg::vAddDataConfHandler(HTREEITEM hParent)
{
    if (bIsEventHandlerExist(_T("DataConf")) == FALSE)
    {
        CString omStrFuncName = defEVENT_IND_HANDLER;
        omStrFuncName += _T("DataConf");
        CString omStrFuncPrototype =  _T("void ")+ omStrFuncName + ("(UINT32 unPGN, BYTE bySrc, BYTE byDest, BOOL bSuccess)");
        //Add the function to current document
        vAddEventHandlerFunction(omStrFuncName, omStrFuncPrototype);
        //Add the function name to tree
        HTREEITEM hNew = m_omTreeCtrl.InsertItem( omStrFuncPrototype, 5, 5, hParent);
        m_omTreeCtrl.SelectItem( hNew );
        //Add the function name to array
        CStringArray* pEventHandlerArray = m_pDoc->omStrGetEventIndPrototypes();
        pEventHandlerArray->Add(omStrFuncPrototype);
    }
}

void CEventHandlerDlg::vAddAdresClaimHandler(HTREEITEM hParent)
{
    if (bIsEventHandlerExist(_T("AddressClaim")) == FALSE)
    {
        CString omStrFuncName = defEVENT_IND_HANDLER;
        omStrFuncName  += _T("AddressClaim");
        CString omStrFuncPrototype;
        omStrFuncPrototype = _T("void ") + omStrFuncName + ("(BYTE byAddress)");
        //Add the function to current document
        vAddEventHandlerFunction(omStrFuncName, omStrFuncPrototype);
        //Add the function name to tree
        HTREEITEM hNew = m_omTreeCtrl.InsertItem( omStrFuncPrototype, 5, 5, hParent);
        //m_omTreeCtrl.SelectItem( hNew );
        //Add the function name to array
        CStringArray* pEventHandlerArray = m_pDoc->omStrGetEventIndPrototypes();
        pEventHandlerArray->Add(omStrFuncPrototype);
    }
}
void CEventHandlerDlg::OnBnClickedOk()
{
    CButton* pButton = (CButton*)GetDlgItem(IDC_CHECK_DATA_CONF);
    HTREEITEM hParent = m_omTreeCtrl.GetSelectedItem();

    if (pButton->GetCheck() == BST_CHECKED)
    {
        vAddDataConfHandler(hParent);
    }

    pButton = (CButton*)GetDlgItem(IDC_CHECK_ADDRESS_CLAIM);

    if (pButton->GetCheck() == BST_CHECKED)
    {
        vAddAdresClaimHandler(hParent);
    }

    if (m_pDoc != NULL)
    {
        m_pDoc->UpdateAllViews( NULL );
        m_pDoc->SetModifiedFlag( TRUE );
    }

    OnOK();
}
