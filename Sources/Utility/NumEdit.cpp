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
 * \file      NumEdit.cpp
 * \brief     Implementation file for CNumEdit
 * \author    Arunkumar K
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Implementation file for CNumEdit
 */

// For all standard header file include
#include "Utils_stdafx.h"
#include "Utility_Structs.h"
// For the RAdix Edit control definition
#include "radixedit.h"
// For the custom spin control
#include "numSpinCtrl.h"
// For CNumEdit Class definition
#include "NumEdit.h"

/////////////////////////////////////////////////////////////////////////////
// CEditItem
#define IDC_NUM_CONTROL         0x12345
/******************************************************************************
 Function Name  :   CNumEdit

 Description    :   Constructor
 Member of      :   CNumEdit
 Functionality  :   Initialises data members

 Author(s)      :   Raja N
 Date Created   :   22.07.2004
 Modifications  :   
******************************************************************************/
CNumEdit::CNumEdit( int nItem,
                    int nSubItem,
                    CString &sContent,
                    const SNUMERICINFO& sInfo)
{
    // Init Member variables
    m_nItem = nItem;
    m_nSubItem = nSubItem;
    m_sContent = sContent;
    m_bVK_ESCAPE = FALSE;
    m_sInfo = sInfo;
    m_pomSpin = NULL;
}

/******************************************************************************
 Function Name  :   ~CNumEdit

 Description    :   Standard destructor
 Member of      :   CNumEdit

 Author(s)      :   Raja N
 Date Created   :   22.07.2004
 Modifications  :
******************************************************************************/
CNumEdit::~CNumEdit()
{
}


BEGIN_MESSAGE_MAP(CNumEdit, CRadixEdit)
    //{{AFX_MSG_MAP(CNumEdit)
    ON_WM_CHAR()
    ON_WM_NCDESTROY()
    ON_WM_KILLFOCUS()
    ON_WM_CREATE()
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEditItem message handlers

/*******************************************************************************
 Function Name  : OnChar
 Input(s)       :
 Output         :  -
 Functionality  : Called by the frame work on keying a character. This function
                  hides the control on press of Enter or Escape.This will resize
                  the edit control if the text is hidden.
 Member of      : CNumEdit
 Friend of      :
 Author(s)      : Raja N
 Date Created   : 22.07.2004
 Modifications  :
*******************************************************************************/
void CNumEdit::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
    // On Press of Enter or Escape hide the control
    if( nChar == VK_ESCAPE ||
        nChar == VK_RETURN) 
    {       
        if( nChar == VK_ESCAPE)
        {
            // Set the Escape flag so that the initial text will be
            // restored
            m_bVK_ESCAPE = TRUE;
        }
        // Set the focus to the parent
        GetParent()->SetFocus();
        return;
    }
    // Call default function
    CRadixEdit::OnChar(nChar, nRepCnt, nFlags);
}

/*******************************************************************************
 Function Name  : OnNcDestroy
 Input(s)       :
 Output         :  -
 Functionality  : This will be called after destroying the window. This will
                  delete the memory used by the control.
 Member of      : CNumEdit
 Friend of      :
 Author(s)      : Raja N
 Date Created   : 22.07.2004
 Modifications  :
*******************************************************************************/
void CNumEdit::OnNcDestroy() 
{
    CRadixEdit::OnNcDestroy();
    // Delete Spin button Memory
    if( IS_BUDDY_ENABLED( m_sInfo.m_byFlag) && m_pomSpin != NULL )
    {
        delete m_pomSpin;
        m_pomSpin = NULL;
    }
    // Delete the control here
    delete this;
}

/*******************************************************************************
 Function Name    : OnCreate
 Input(s)         : LPCREATESTRUCT
 Output           : int
 Functionality    : Called by the framework while a window is being created.
                    This will set the text, font and the selection.
 Member of        : CNumEdit
 Friend of        : -
 Author(s)        : Raja N
 Date Created     : 22.07.2004
 Modifications    : Raja N on 01.08.2004, Modificatinos as per code review.
*******************************************************************************/
int CNumEdit::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
    int nRetVal = 0;
    // Create the control first
    nRetVal = CRadixEdit::OnCreate(lpCreateStruct);
    // OnCreate is not returning 0 to indicate success. It sometimes returns 1
    // or any positive non-zero values. So check for failure condition instead
    // of success condition
    if( nRetVal != defCREATE_FAILED )
    {
        // Set the base value
        vSetBase(m_sInfo.m_byBase);
        // Set All CAPs in case of Hex Number
        if( m_sInfo.m_byBase == defBASE_HEX )
        {
            // Add Upper case style to the control
            ModifyStyle( 0, ES_UPPERCASE );
        }
        // Set the number type - Signed or Unsigned
        vSetSigned( IS_SIGNED_NUMBER(m_sInfo.m_byFlag) &&
                    m_sInfo.m_byBase == defBASE_DEC );
        // Floating or Integer
        vAcceptFloatingNum(IS_FLOAT_ENABLED( m_sInfo.m_byFlag));
        // Text Limit
        if( m_sInfo.m_nTextLimit != 0 )
        {
            SetLimitText(m_sInfo.m_nTextLimit);
        }
        
        // Create Spin Control only if it is required
        if( IS_BUDDY_ENABLED( m_sInfo.m_byFlag))
        {
            CRect omRect(0,0,0,0);
            // Make the style word
            DWORD dwStyle = lpCreateStruct->style | UDS_ARROWKEYS |
                            UDS_ALIGNRIGHT | UDS_AUTOBUDDY;
            // Disable wrap property
            dwStyle ^= UDS_WRAP;
            // Create the spin Control
            m_pomSpin = new CNumSpinCtrl;
            if( m_pomSpin != NULL )
            {
                m_pomSpin->Create( dwStyle, omRect, GetParent(),
                                   IDC_NUM_CONTROL );
                // Init spin control
                // Set the base value
                m_pomSpin->SetBase(m_sInfo.m_byBase);
                // Set the Signal Length
                m_pomSpin->vSetDataSize( m_sInfo.m_nSigLength);
                // Set the data type
                m_pomSpin->vSetSigned( IS_SIGNED_NUMBER(m_sInfo.m_byFlag) );
                // Set the float number property
                m_pomSpin->vSetFloatNumber( IS_FLOAT_ENABLED(m_sInfo.m_byFlag));
                // Set the formatting string
                if( m_sInfo.m_byBase == defBASE_HEX)
                {
                    m_pomSpin->vSetFormatString( defFORMAT_INT64_HEX );
                }
                else
                {
                    // Show decimal pointes only if it is of Float type
                    if(IS_FLOAT_ENABLED( m_sInfo.m_byFlag))
                    {
                        m_pomSpin->vSetFormatString( defSTR_FORMAT_PHY_VALUE );
                    }
                    else
                    {
                        m_pomSpin->vSetFormatString( defFORMAT_INT64_DECIMAL );
                    }
                }
                // Set the Min, Max and Step Value
                // If floating point is enabled then use doublew member
                if( IS_FLOAT_ENABLED( m_sInfo.m_byFlag) )
                {
                    m_pomSpin->vSetRangeAndDelta( m_sInfo.m_uMinVal.m_dValue,
                        m_sInfo.m_uMaxVal.m_dValue, m_sInfo.m_uDelta.m_dValue);
                }
                // If floating point is not enabled then use __int64 member to
                // get 64 bit resolution
                else
                {
                    m_pomSpin->vSetRangeAndDelta( m_sInfo.m_uMinVal.m_n64Value,
                     m_sInfo.m_uMaxVal.m_n64Value, m_sInfo.m_uDelta.m_n64Value);
                }
            }
            //Spin Control Creaate failed. Avoid proceeding further
            else
            {
                nRetVal = defCREATE_FAILED;
            }
        }
    }
    // Set the basic properties of the control
    CWnd *pomWnd = NULL;
    // Get parent window. That is list control
    pomWnd = GetParent();
    if( pomWnd != NULL )
    {
        // Get parent's font
        CFont* pomFont = pomWnd->GetFont();
        // Set the same font
        SetFont(pomFont);
        // Set the initial Text
        SetWindowText(m_sContent);
        SetFocus();
        // Select the text completly
        SetSel(0, m_sContent.GetLength());
    }

    return nRetVal;
}

/*******************************************************************************
 Function Name  : OnKillFocus
 Input(s)       : pNewWnd - Pointer to the window that got focus
 Output         :  -
 Functionality  : This will be called if the edit control losses its focus. This
                  function wikk post a message to parent list control to inform
                  that list data has been editied. If the loss of focus is
                  because of Escape key this will restore the old value.
 Member of      : CNumEdit
 Friend of      :
 Author(s)      : Raja N
 Date Created   : 22.07.2004
 Modifications  : Raja N on 02.08.2004
                  Removed not required checks from the if condition
*******************************************************************************/
void CNumEdit::OnKillFocus(CWnd* pNewWnd) 
{
    CRadixEdit::OnKillFocus(pNewWnd);
	CWnd* m_pMainWnd = AfxGetMainWnd();
    if( pNewWnd != NULL                   // NULL condition - Minimize condition
        && pNewWnd != m_pMainWnd ) // For Dialog Close
    {
        CString omStr;
        // Get the textbox content
        GetWindowText(omStr);
        // Notify ListView ctrl's parent 
        LV_DISPINFO lvDisplayInfo;
		lvDisplayInfo.hdr.idFrom = GetDlgCtrlID(); 
        lvDisplayInfo.hdr.hwndFrom = GetParent()->m_hWnd;        
		lvDisplayInfo.item.mask = LVIF_TEXT;   
        lvDisplayInfo.hdr.code = LVN_ENDLABELEDIT;        
		lvDisplayInfo.item.iSubItem = m_nSubItem;
        lvDisplayInfo.item.iItem = m_nItem;                
        lvDisplayInfo.item.cchTextMax = omStr.GetLength();
		lvDisplayInfo.item.pszText = m_bVK_ESCAPE ? NULL : LPTSTR((LPCTSTR)omStr);
        GetParent()->GetParent()->SendMessage( WM_NOTIFY,
                                               GetParent()->GetDlgCtrlID(),
                                               (LPARAM)&lvDisplayInfo);
        // Destroy Spin Control
        if( IS_BUDDY_ENABLED( m_sInfo.m_byFlag) && m_pomSpin != NULL )
        {
            m_pomSpin->DestroyWindow();
        }
        
    }
    // Kill the control as its lifetime is over
    DestroyWindow();
}

/*******************************************************************************
 Function Name    : PreTranslateMessage
                                                                           
 Input(s)         : MSG* pMsg
 Output           : BOOL
 Functionality    : To avoid processing special keys
 Member of        : CNumEdit
 Friend of        :   -
                                                                           
 Author(s)        : Raja N
 Date Created     : 22.07.2004
 Modifications    : 
*******************************************************************************/
BOOL CNumEdit::PreTranslateMessage(MSG* pMsg) 
{
    // TODO: Add your specialized code here and/or call the base class
    if( pMsg->message == WM_KEYDOWN )   
    {       
        if( pMsg->wParam == VK_RETURN || 
            pMsg->wParam == VK_ESCAPE )
        {           
            ::TranslateMessage(pMsg);
            ::DispatchMessage(pMsg);
            return 1;
        }
        if(pMsg->wParam == VK_TAB)
            MessageBeep(0);
    }

    return CRadixEdit::PreTranslateMessage(pMsg);
}

