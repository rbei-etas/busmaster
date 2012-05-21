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
 * \file      MsgIDAttr.cpp
 * \brief     This file contain the implementation of CMsgIDAttr class
 * \author    Ratnadip Choudhury
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * This file contain the implementation of CMsgIDAttr class
 */

// For all standard header file include
#include "stdafx.h"
// Interface file for the application class
#include "BUSMASTER.h"
// Interface file for CRadixEdit class
#include "Utility/RadixEdit.h"
// Interface file for CMsgIDAttr class
#include "MsgIDAttr.h"
// Interface file for CMessageAttrib class
#include "MessageAttrib.h"

/////////////////////////////////////////////////////////////////////////////
// CMsgIDAttr dialog



/******************************************************************************
 Function Name  :   CMsgIDAttr

 Description    :   Default constructor of the dialog box
 Input(s)       :   pParent : Pointer to the parent CWnd object
 Output         :
 Functionality  :   Initialise data members
 Member of      :   CMsgIDAttr

 Author(s)      :   Ratnadip Choudhury
 Date Created   :   18-03-2002
******************************************************************************/
CMsgIDAttr::CMsgIDAttr(ETYPE_BUS eBusType, CWnd* pParent /*=NULL*/)
    : CDialog(CMsgIDAttr::IDD, pParent)
{
    //{{AFX_DATA_INIT(CMsgIDAttr)
    m_omStrMsg = "";
    m_sColour = DEFAULT_MSG_COLOUR;
    m_nID = 0x0;
    m_bForEdit = false;
    m_bDBMessage = FALSE;
    m_hClrBtn = NULL;
    //}}AFX_DATA_INIT
    m_eBusType = eBusType;
}

/******************************************************************************
 Function Name  :   DoDataExchange

 Description    :   Called by the framework to exchange & validate dialog data
 Input(s)       :   pDX : Pointer to a CDataExchange object
 Output         :   -
 Functionality  :
 Member of      :   CMsgIDAttr

 Author(s)      :   Ratnadip Choudhury
 Date Created   :   18-03-2002
******************************************************************************/
void CMsgIDAttr::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CMsgIDAttr)
    DDX_Control(pDX, IDC_PBTN_COLOUR, m_omColourBox);
    DDX_Control(pDX, IDC_EDIT_CANID_VAL, m_odCanIDVal);
    DDX_Text(pDX, IDC_EDIT_CANID_STRING, m_omStrMsg);
    //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMsgIDAttr, CDialog)
    //{{AFX_MSG_MAP(CMsgIDAttr)
    ON_BN_CLICKED(IDC_PBTN_COLOUR, OnPbtnColour)
    ON_WM_HELPINFO()
    ON_WM_CTLCOLOR()
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMsgIDAttr message handlers



/******************************************************************************
 Function Name  :   OnOK

 Description    :   Called by the framework when the user clicks the OK button
                    (the button with an ID of IDOK)
 Input(s)       :   -
 Output         :   -
 Functionality  :   Invokes confirmation for a new message entry or
                    modification of a existing one.
 Member of      :   CMsgIDAttr

 Author(s)      :   Ratnadip Choudhury
 Date Created   :   19-03-2002
******************************************************************************/
void CMsgIDAttr::OnOK()
{
    UpdateData(TRUE);
    m_nID = (UINT)m_odCanIDVal.lGetValue();
    m_omStrMsg.TrimLeft();
    m_omStrMsg.TrimRight();
    UpdateData(FALSE);
    int nResult = 0;

    if (m_odCanIDVal.LineLength() == 0)
    {
        AfxMessageBox(_T("Invalid Message ID"));
        GotoDlgCtrl(GetDlgItem(IDC_EDIT_CANID_VAL));
        nResult = -1;
    }

    if (m_bForEdit == false)
    {
        if (nResult == 0)
        {
            nResult = CMessageAttrib::ouGetHandle(m_eBusType).nValidateNewID(m_nID);

            if (nResult < 0)
            {
                AfxMessageBox(_T("Duplicate Message ID"));
                GotoDlgCtrl(GetDlgItem(IDC_EDIT_CANID_VAL));
            }
        }
    }

    if (nResult >= 0) // valid CanID
    {
        if (m_omStrMsg.IsEmpty()) //invalid string
        {
            AfxMessageBox(_T("Invalid message string"));
            GotoDlgCtrl(GetDlgItem(IDC_EDIT_CANID_STRING));
            nResult = -1;
        }
    }

    if (nResult >= 0) // everything valid
    {
        CDialog::OnOK();
    }
}


/******************************************************************************
 Function Name  :   OnInitDialog

 Description    :   This member function is called in response to the
                    WM_INITDIALOG message
 Input(s)       :   -
 Output         :   Specifies whether the application has set the input focus
                    to one of the controls in the dialog box. If OnInitDialog
                    returns nonzero, Windows sets the input focus to the first
                    control in the dialog box. The application can return 0
                    only if it has explicitly set the input focus to one of the
                    controls in the dialog box.
 Functionality  :   Performs the initial settings of the edit control which
                    accepts message ID depending on the mode of operation
 Member of      :   CMsgIDAttr

 Author(s)      :   Ratnadip Choudhury
 Date Created   :   08-03-2002
******************************************************************************/
BOOL CMsgIDAttr::OnInitDialog()
{
    CDialog::OnInitDialog();
    // TODO: Add extra initialization here
    // Limit the text field only to eight characters
    m_odCanIDVal.vSetSigned(false);
    m_odCanIDVal.LimitText(8);

    if (m_bDBMessage == TRUE)
    {
        CEdit* pomEdit = (CEdit*) GetDlgItem(IDC_EDIT_CANID_STRING);

        if (pomEdit != NULL)
        {
            pomEdit->SetReadOnly(TRUE);
        }

        m_bForEdit = true; // You can't add one if it is for DB message
    }

    if (m_bForEdit == true)
    {
        m_odCanIDVal.vSetValue(m_nID);
        m_odCanIDVal.SetReadOnly(TRUE);
    }

    if (m_bForEdit == false)
    {
        SetWindowText("Add Message Display Attributes");
    }

    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

/******************************************************************************
 Function Name  :   OnPbtnColour

 Input(s)       :   -
 Output         :   -
 Functionality  :   Invokes the system colour dialog box so that a suitable
                    colour for the message in question may be selected
 Member of      :   CMsgIDAttr

 Author(s)      :   Ratnadip Choudhury
 Date Created   :   18-03-2002
******************************************************************************/
void CMsgIDAttr::OnPbtnColour()
{
    CColorDialog omColourDlg(m_sColour);

    if (omColourDlg.DoModal() == IDOK)
    {
        m_sColour = omColourDlg.GetColor();
        m_omColourBox.InvalidateRect(NULL);
    }
}

/******************************************************************************
 Function Name  :   OnHelpInfo

 Description    :   If a menu is active when F1 is pressed, WM_HELP is sent to
                    the window associated with the menu; otherwise, WM_HELP is
                    sent to the window that has the keyboard focus. If no
                    window has the keyboard focus, WM_HELP is sent to the
                    currently active window
 Input(s)       :   pHelpInfo : Pointer to a HELPINFO structure that contains
                    information about the menu item, control, dialog box, or
                    window for which help is requested.
 Output         :
 Functionality  :   Called by the framework when the user hits the F1 key
 Member of      :   CMsgIDAttr

 Author(s)      :   Ratnadip Choudhury
 Date Created   :   18-03-2002
******************************************************************************/
BOOL CMsgIDAttr::OnHelpInfo(HELPINFO* pHelpInfo)
{
    // TODO: Add your message handler code here and/or call default
    return CDialog::OnHelpInfo(pHelpInfo);
}
/******************************************************************************
 Function Name  :   OnCtlColor

 Description    :
 Input(s)       :
 Output         :
 Functionality  :
 Member of      :   CMsgIDAttr

 Author(s)      :   Ratnadip Choudhury
 Date Created   :   20-08-2003
******************************************************************************/
HBRUSH CMsgIDAttr::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

    // TODO: Change any attributes of the DC here
    if (pWnd->GetDlgCtrlID() == IDC_PBTN_COLOUR)
    {
        if (m_hClrBtn != NULL)
        {
            ::DeleteObject((HBRUSH) m_hClrBtn);
        }

        if ((m_hClrBtn = ::CreateSolidBrush(m_sColour)) != NULL)
        {
            hbr = m_hClrBtn;
        }
    }

    // TODO: Return a different brush if the default is not desired
    return hbr;
}

BOOL CMsgIDAttr::PreTranslateMessage(MSG* pMsg)
{
    // Capture the space character and do not process the same
    BOOL bSkip = FALSE;

    if ( pMsg->message == WM_CHAR )
    {
        if ( pMsg->wParam == ' ')
        {
            bSkip = TRUE;
        }
    }

    if (bSkip == FALSE)
    {
        bSkip = CDialog::PreTranslateMessage(pMsg);
    }

    return bSkip;
}
