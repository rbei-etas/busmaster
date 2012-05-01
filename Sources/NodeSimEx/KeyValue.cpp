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
 * \file      KeyValue.cpp
 * \brief     This file contain the definition all member function of
 * \author    Ratnadip Choudhury
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * This file contain the definition all member function of
 */
// For all standard header file include
#include "NodeSimEx_stdafx.h"
// Definition of App class
#include "FunctionEditorDoc.h"  // Get Document
// CKeyValue class definition
#include "FnsTreeView.h"
#include "GlobalObj.h"
#include "KeyValue.h"

/******************************************************************************/
/*  Function Name    :  CKeyValue                                             */
/*  Input(s)         :  Pointer to Parent Window and Pointer to Char          */
/*  Output           :                                                        */
/*  Functionality    :  This function is defualt constructor. Data member is  */
/*                      assigned the value passed as parameter                */
/*                                                                            */
/*  Member of        :  CKeyValue                                             */
/*  Friend of        :      -                                                 */
/*  Author(s)        :  Amitesh Bharti                                        */
/*  Date Created     :  08.03.2002                                            */
/*  Modification By  :  Amitesh Bharti                                        */
/*  Modification on  :  28.03.2002, Review commenrs                           */
/******************************************************************************/
CKeyValue::CKeyValue(CFnsTreeView* pFnsTreeView, CFunctionEditorDoc* pDoc,
                     CWnd* pParent/*=NULL*/,TCHAR* pcKeyVal )
    : CDialog(CKeyValue::IDD, pParent),m_pcKeyVal(pcKeyVal)
{
    //{{AFX_DATA_INIT(CKeyValue)
    // NOTE: the ClassWizard will add member initialization here
    //}}AFX_DATA_INIT
    m_pFnsTreeView  = pFnsTreeView;
    m_pDoc          = pDoc;
}
/******************************************************************************/
/*  Function Name    :  DoDataExchange                                        */
/*  Input(s)         :                                                        */
/*  Output           :                                                        */
/*  Functionality    :  Called by the framework to exchange and validate      */
/*                         dialog data                                        */
/*                                                                            */
/*  Member of        :  CKeyValue                                             */
/*  Friend of        :      -                                                 */
/*  Author(s)        :  Amitesh Bharti                                        */
/*  Date Created     :  08.03.2002                                            */
/*  Modification By  :                                                        */
/*  Modification on  :                                                        */
/******************************************************************************/
void CKeyValue::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CKeyValue)
    // NOTE: the ClassWizard will add DDX and DDV calls here
    //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CKeyValue, CDialog)
    //{{AFX_MSG_MAP(CKeyValue)
    ON_WM_HELPINFO()
    ON_BN_CLICKED(IDC_CBTN_KEY_APPLY, OnCbtnKeyApply)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/******************************************************************************/
/*  Function Name    :  OnCancel                                              */
/*  Input(s)         :                                                        */
/*  Output           :                                                        */
/*  Functionality    :  This function is called by frame work when user presse*/
/*                      CANCEL button. The value pointer by data member is    */
/*                      initialised to NULL                                   */
/*                                                                            */
/*  Member of        :  CKeyValue                                             */
/*  Friend of        :      -                                                 */
/*  Author(s)        :  Amitesh Bharti                                        */
/*  Date Created     :  08.03.2002                                            */
/*  Modification By  :                                                        */
/*  Modification on  :                                                        */
/******************************************************************************/
void CKeyValue::OnCancel()
{
    *m_pcKeyVal = NULL;
    CDialog::OnCancel();
}
/******************************************************************************/
/*  Function Name    :  PreTranslateMessage                                   */
/*  Input(s)         :  Pointer to MSG structure containing information of MSG*/
/*  Output           :                                                        */
/*  Functionality    :  This function is called by frame work to filter       */
/*                      messages before they are sent. WM_CHAR message is     */
/*                      checked for and key value is assign to data member.   */
/*                      Only alphanumeric key value is assigned allowed.      */
/*                                                                            */
/*  Member of        :  CKeyValue                                             */
/*  Friend of        :      -                                                 */
/*  Author(s)        :  Amitesh Bharti                                        */
/*  Date Created     :  08.03.2002                                            */
/*  Modification By  :  Amarnath Shastry                                      */
/*  Modification on  :  27.03.2002                                            */
/*  Modification By  :  Amitesh Bharti                                        */
/*  Modification on  :  28.03.2002, Review comments                           */
/*  Modification By  :  Amitesh Bharti                                        */
/*  Modification on  :  06.03.2003,"*"key is processed for generic key handler*/
/*  Modification By  :  Amitesh Bharti                                        */
/*  Modification on  :  07.04.2003,numeric key is also processed  key handler */
/******************************************************************************/
BOOL CKeyValue::PreTranslateMessage(MSG* pMsg)
{
    CString omStrKey = _T("");

    // check for WM_CHAR message and then check for key pressed.
    // Stored only alphabatic keys pressed and "*". display the key values
    if(pMsg->message == WM_CHAR)
    {
        omStrKey.Format(_T("Key Pressed : %c"),pMsg->wParam);

        if(   ( pMsg->wParam >= 'a' && pMsg->wParam<='z' )
                ||( pMsg->wParam >= 'A' && pMsg->wParam<='Z' )
                ||( pMsg->wParam >= '0' && pMsg->wParam<='9' )
                || pMsg->wParam == defGENERIC_KEY )
        {
            *m_pcKeyVal = static_cast <CHAR> (pMsg->wParam);
            SetDlgItemText(IDC_STAT_KEY, omStrKey);

            // Amarnath S , 27.03.2002
            // Enable the Ok button if disabled
            if (!(GetDlgItem( IDOK )->IsWindowEnabled()) &&
                    !(GetDlgItem( IDC_CBTN_KEY_APPLY )->IsWindowEnabled()))
            {
                GetDlgItem( IDOK )->EnableWindow(TRUE);
                GetDlgItem( IDC_CBTN_KEY_APPLY )->EnableWindow(TRUE);
            }
        }
    }

    return CDialog::PreTranslateMessage(pMsg);
}
/******************************************************************************/
/*  Function Name    :  OnHelpInfo                                            */
/*                                                                            */
/*  Input(s)         :                                                        */
/*  Output           :                                                        */
/*  Functionality    :  This function is called by the framework in response  */
/*                      to the WM_HELPINFO message. This message is sent to   */
/*                      the dialog box during help request from user.         */
/*  Member of        :  CKeyValue                                             */
/*  Friend of        :      -                                                 */
/*                                                                            */
/*  Author(s)        :  Amitesh Bharti                                        */
/*  Date Created     :  11.03.2002                                            */
/*  Modifications    :                                                        */
/*                                                                            */
/******************************************************************************/
BOOL CKeyValue::OnHelpInfo(HELPINFO* pHelpInfo)
{
    //theApp.vSetHelpID(pHelpInfo->dwContextId);
    return CDialog::OnHelpInfo(pHelpInfo);
}
/******************************************************************************/
/*  Function Name    :  OnOK                                                  */
/*                                                                            */
/*  Input(s)         :                                                        */
/*  Output           :                                                        */
/*  Functionality    :  Validates for duplicate key handler if any
/*  Member of        :  CKeyValue                                             */
/*  Friend of        :      -                                                 */
/*                                                                            */
/*  Author(s)        :  Amarnath Shastry                                      */
/*  Date Created     :  12.03.2002                                            */
/*  Modifications    :  Amitesh Bharti                                        */
/*                      07.03.2003, "*" key is process for generic key hanlder*/
/******************************************************************************/
void CKeyValue::OnOK()
{
    BOOL bRetVal = TRUE;

    // Check for duplicate selection
    // Get key handler array from the document
    // Check for valid pointer
    if (m_pDoc != NULL)
    {
        CStringArray* pKeyArray = m_pDoc->omStrGetKeyHandlerPrototypes();

        if (pKeyArray)
        {
            CString omStrMsgPro   = STR_EMPTY;
            UINT unKeyHandlerCount = 0;
            unKeyHandlerCount = (COMMANUINT)pKeyArray->GetSize();

            for (UINT nCount = 0; nCount < unKeyHandlerCount; nCount++)
            {
                omStrMsgPro = pKeyArray->GetAt( nCount );
                CString omStrFuncName   = defKEY_HANDLER;
                CString omStrKeyPressed = _T("");

                if((*m_pcKeyVal) == defGENERIC_KEY)
                {
                    omStrKeyPressed = defGENERIC_KEY_HANDLER_TEXT;
                }
                else
                {
                    omStrKeyPressed = m_pcKeyVal;
                }

                omStrFuncName += omStrKeyPressed;
                // Construct Function definiton
                CString omStrFuncPrototype = defDEFAULT_KEY_HANDLER_CODE;
                omStrFuncPrototype.Replace(_T("KEYNAME"), omStrKeyPressed );

                if ( omStrMsgPro == omStrFuncPrototype )
                {
                    omStrMsgPro = defMSG_DUPL_KEY_HANDLER;
                    omStrMsgPro.Replace( _T("KEYNAME"), m_pcKeyVal );
                    // Duplicate message hanlder, shout
                    AfxMessageBox( omStrMsgPro, MB_OK|MB_ICONINFORMATION );
                    nCount = (COMMANUINT)pKeyArray->GetSize();
                    bRetVal = FALSE;
                }
            }
        }
    }

    if ( bRetVal == TRUE)
    {
        CDialog::OnOK();
    }
}
/******************************************************************************/
/*  Function Name    :  OnInitDialog                                          */
/*                                                                            */
/*  Input(s)         :                                                        */
/*  Output           :  BOOL                                                  */
/*  Functionality    :  Disables the OK button
/*  Member of        :  CKeyValue                                             */
/*  Friend of        :      -                                                 */
/*                                                                            */
/*  Author(s)        :  Amarnath Shastry                                      */
/*  Date Created     :  27.03.2002                                            */
/*  Modifications    :                                                        */
/*                                                                            */
/******************************************************************************/
BOOL CKeyValue::OnInitDialog()
{
    CDialog::OnInitDialog();
    CWnd* pWnd = GetDlgItem(IDOK );

    if (pWnd != NULL)
    {
        pWnd->EnableWindow(FALSE);
    }

    pWnd = GetDlgItem(IDC_CBTN_KEY_APPLY );

    if (pWnd != NULL)
    {
        pWnd->EnableWindow(FALSE);
    }

    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}
/******************************************************************************/
/*  Function Name    :  OnCbtnKeyApply                                        */
/*                                                                            */
/*  Input(s)         :                                                        */
/*  Output           :                                                        */
/*  Functionality    :  Validates for duplicate key handler if any. This      */
/*                      is called by frame work when user selects "Apply"     */
/*                      button. The key handler is added from this function to*/
/*                      function editor.                                      */
/*  Member of        :  CKeyValue                                             */
/*  Friend of        :      -                                                 */
/*                                                                            */
/*  Author(s)        :  Amarnath Shastry                                      */
/*  Date Created     :  07.03.2003                                            */
/******************************************************************************/
void CKeyValue::OnCbtnKeyApply()
{
    BOOL bValidateSelection = TRUE;

    if(m_pDoc != NULL )
    {
        SBUS_SPECIFIC_INFO sBusSpecInfo;
        m_pDoc->bGetBusSpecificInfo(sBusSpecInfo);
        bValidateSelection = bValidateDuplicateKeyHandler(m_pDoc);

        if(bValidateSelection == TRUE)
        {
            CString omSelectedText = _T("");
            omSelectedText = BUS_FN_HDR;
            CString omFunc = defKEY_HANDLER;

            if(*m_pcKeyVal != defGENERIC_KEY)
            {
                omFunc += m_pcKeyVal;
            }
            else
            {
                omFunc += defGENERIC_KEY_HANDLER_TEXT;
            }

            omSelectedText.Replace(_T("PLACE_HODLER_FOR_BUSNAME"), sBusSpecInfo.m_omBusName);
            omSelectedText.Replace( _T("PLACE_HODLER_FOR_FUNCTIONNAME"),
                                    omFunc );
            m_pDoc->m_omSourceCodeTextList.AddTail( omSelectedText );
            // Form the function prototype
            omSelectedText = defDEFAULT_KEY_HANDLER_CODE;

            if(*m_pcKeyVal != defGENERIC_KEY)
            {
                omSelectedText.Replace( _T("KEYNAME"), m_pcKeyVal );
            }
            else
            {
                omSelectedText.Replace( _T("KEYNAME"), defGENERIC_KEY_HANDLER_TEXT );
            }

            m_pDoc->m_omSourceCodeTextList.AddTail( omSelectedText );
            CString omStrPrototype = omSelectedText;

            if(m_pFnsTreeView != NULL )
            {
                // Add the prototype to the tree view
                CTreeCtrl& omTree = m_pFnsTreeView->GetTreeCtrl();
                HTREEITEM hItem = omTree.GetSelectedItem();
                HTREEITEM hNew =
                    omTree.InsertItem( omSelectedText, hItem);
                omTree.SetItemImage( hNew, 5, 5 );
                omTree.SelectItem( hNew );
                // Form the body of the function
                omSelectedText = "{";
                m_pDoc->m_omSourceCodeTextList.AddTail( omSelectedText );
                omSelectedText = defTODO;
                m_pDoc->m_omSourceCodeTextList.AddTail( omSelectedText );
                // Form the function footer
                omSelectedText = BUS_FN_FOOTER;
                omSelectedText.Replace(_T("PLACE_HODLER_FOR_BUSNAME"), sBusSpecInfo.m_omBusName);
                omSelectedText.Replace( _T("PLACE_HODLER_FOR_FUNCTIONNAME"),
                                        omFunc );
                m_pDoc->m_omSourceCodeTextList.AddTail( omSelectedText );
                CStringArray* pKeyArray =
                    m_pDoc->omStrGetKeyHandlerPrototypes();

                if ( pKeyArray != NULL )
                {
                    pKeyArray->Add( omStrPrototype );
                }

                m_pDoc->UpdateAllViews( NULL );
                m_pDoc->SetModifiedFlag( TRUE );
            }
        }
    }
}
/******************************************************************************/
/*  Function Name    :  bValidateDuplicateKeyHandler                          */
/*                                                                            */
/*  Input(s)         :  pDoc : pointer to document                            */
/*  Output           :  TRUE or FALSE                                         */
/*  Functionality    :  Validates for duplicate key handler if any. This      */
/*                      is called to validate key handler if it is already add*/
/*                      button. The key handler is added from this function to*/
/*                      function editor.                                      */
/*  Member of        :  CKeyValue                                             */
/*  Friend of        :      -                                                 */
/*                                                                            */
/*  Author(s)        :  Amarnath Shastry                                      */
/*  Date Created     :  07.03.2003                                            */
/*  Modification By  :  Amitesh Bharti                                        */
/*  Modification on  :  07.04.2003,review comment incorporated                */
/*  Modification By  :  Amitesh Bharti                                        */
/*  Modification on  :  17.04.2003,validation for generic key handler already */
/*                                 added. Found during unit testing.          */
/******************************************************************************/
BOOL CKeyValue:: bValidateDuplicateKeyHandler(CFunctionEditorDoc* pDoc)
{
    BOOL bReturn = TRUE;
    CStringArray* pKeyArray;

    if(pDoc != NULL )
    {
        pKeyArray = pDoc->omStrGetKeyHandlerPrototypes();

        if(pKeyArray != NULL )
        {
            UINT unKeyCount = 0;
            unKeyCount = (COMMANUINT)pKeyArray->GetSize();
            CString omStrKeyHandlerSelected = defDEFAULT_KEY_HANDLER_CODE;
            CString omStrKeyhandler = _T("");

            if(*m_pcKeyVal != defGENERIC_KEY)
            {
                omStrKeyHandlerSelected.Replace(_T("KEYNAME"), m_pcKeyVal );
            }
            else
            {
                omStrKeyHandlerSelected.Replace(_T("KEYNAME"),
                                                defGENERIC_KEY_HANDLER_TEXT);
            }

            for(UINT i = 0; i<unKeyCount ; i++)
            {
                omStrKeyhandler = pKeyArray->GetAt(i);

                if(omStrKeyHandlerSelected.Compare(omStrKeyhandler) == 0)
                {
                    i = unKeyCount;
                    bReturn = FALSE;
                    omStrKeyhandler = defMSG_DUPL_KEY_HANDLER;
                    omStrKeyhandler.Replace( _T("KEYNAME"), m_pcKeyVal );
                    // Duplicate message hanlder, shout
                    AfxMessageBox( omStrKeyhandler, MB_OK|MB_ICONINFORMATION );
                }
            }
        }
        else
        {
            bReturn = FALSE;
        }
    }

    return bReturn;
}
