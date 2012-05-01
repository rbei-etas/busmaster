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
 * \file      MessageInterpretation.cpp
 * \brief     Implementation file for CMessageInterpretation class
 * \authors   Ratnadip Choudhury, Amitesh Bharti
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Implementation file for CMessageInterpretation class
 */
// For all standard header file include
#include "stdafx.h"
// Interface file for the application class
#include "BUSMASTER.h"
// Application related information to be shared among windows
#include "common.h"
// Its own interface file
#include "Utility/fflistctrl.h"
#include "MessageInterpretation.h"
#include ".\messageinterpretation.h"
//#include "MsgInterpretation.h"

/////////////////////////////////////////////////////////////////////////////
// CMessageInterpretation dialog


extern CCANMonitorApp theApp;
/******************************************************************************
 Function Name  :   CMessageInterpretation

 Description    :
 Input(s)       :   CWnd* pParent =NULL, Pointer to parent window.
 Output         :   -
 Functionality  :   Default contructor
 Member of      :   CMessageInterpretation

 Author(s)      :   Ratnadip Choudhury
 Date Created   :   16.02.2004
 Modifications  :   Raja N
                    05/04/2004 Modified the color value to reduce the contrest
******************************************************************************/
CMessageInterpretation::CMessageInterpretation(CWnd* pParent /*=NULL*/)
    : CDialog(CMessageInterpretation::IDD, pParent)
    , m_strCaption(_T("  Message"))
{
    //{{AFX_DATA_INIT(CMessageInterpretation)
    m_omMsgName = STR_EMPTY;
    m_omMsgID = STR_EMPTY;
    //}}AFX_DATA_INIT
    m_hBrushStatic = CreateSolidBrush(RGB(30, 60, 150));
    m_hWndParent = NULL;
}

/******************************************************************************
 Function Name  :   DoDataExchange

 Description    :
 Input(s)       :   CDataExchange* pDX
 Output         :   -
 Functionality  :
 Member of      :   CMessageInterpretation

 Author(s)      :   Ratnadip Choudhury
 Date Created   :   16.02.2004
 Modifications  :
******************************************************************************/
void CMessageInterpretation::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CMessageInterpretation)
    DDX_Control(pDX, IDC_LIST_SIGNAL, m_ctrlSignal);
    DDX_Text(pDX, IDC_EDIT_MSG_NAME, m_omMsgName);
    DDX_Text(pDX, IDC_EDIT_MSG_ID, m_omMsgID);
    //}}AFX_DATA_MAP
    DDX_Text(pDX, IDC_CAPTION, m_strCaption);
}


BEGIN_MESSAGE_MAP(CMessageInterpretation, CDialog)
    //{{AFX_MSG_MAP(CMessageInterpretation)
    ON_WM_SIZE()
    ON_WM_CTLCOLOR()
    ON_WM_SHOWWINDOW()
    ON_WM_CLOSE()
    ON_WM_CREATE()
    //}}AFX_MSG_MAP
    ON_WM_MOVE()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMessageInterpretation message handlers
/******************************************************************************
 Function Name  :   OnSize

 Description    :
 Input(s)       :   UINT nType, int cx, int cy
 Output         :   -
 Functionality  :   The control is resized based on user resizing the dialog.
 Member of      :   CMessageInterpretation

 Author(s)      :   Ratnadip Choudhury
 Date Created   :   16.02.2004
 Modifications  :   Raja N on 13.04.2004
                    Added check for minimized condition
******************************************************************************/
void CMessageInterpretation::OnSize(UINT nType, int cx, int cy)
{
    CDialog::OnSize(nType, cx, cy);

    // TODO: Add your message handler code here
    // Resize controls only if the window is visible
    if( nType != SIZE_MINIMIZED )
    {
        vResizeControls();
    }

    ::PostMessage(m_hWndParent, WM_UPDATE_MSG_INTRP_WND_PLC , NULL, NULL);
}
/******************************************************************************
 Function Name  :   OnCtlColor

 Description    :   The framework calls this member function when a child
                    control is about to be drawn. Most controls send this
                    message to their parent (usually a dialog box) to prepare
                    the pDC for drawing the control using the correct colors.

 Input(s)       :   CDC* pDC       : Contains a pointer to the display context
                                     for the child window.
                    CWnd* pWnd     : Contains a pointer to the control asking
                                     for the color. May be temporary.
                    UINT nCtlColor : Contains one of the following values,
                                     specifying the type of control
 Output         :   HBRUSH:A handle to the brush that is to be used for painting
                    the control background.
 Functionality  :   The dialog is painted and the different control is draw with
                    a different colour.
 Member of      :   CMessageInterpretation

 Author(s)      :   Ratnadip Choudhury
 Date Created   :   16.02.2004
 Modifications  :
******************************************************************************/
HBRUSH CMessageInterpretation::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
    int nCurrentCtrlID = pWnd->GetDlgCtrlID();

    // TODO: Change any attributes of the DC here
    if ((nCurrentCtrlID == IDC_CAPTION) ||
            (nCurrentCtrlID == IDC_CAPTION2)
       )
    {
        // Set the text color to white
        pDC->SetTextColor(RGB(255, 255, 255));
        // Set the background mode for text to transparent
        // so background will show thru.
        pDC->SetBkMode(TRANSPARENT);
        // Return handle to our CBrush object
        hbr = m_hBrushStatic;
    }

    return hbr;
}
/******************************************************************************
 Function Name  :   vResizeControls

 Description    :
 Input(s)       :   -
 Output         :   -
 Functionality  :   Resize the control when dialog is resized.
 Member of      :   CMessageInterpretation

 Author(s)      :   Ratnadip Choudhury
 Date Created   :   16.02.2004
 Modifications  :   Raja N on 05/04/2004
                :   Modified width calculation as one more column is included
                    in the list.
******************************************************************************/
void CMessageInterpretation::vResizeControls(void)
{
    RECT WndClientRect, CtrlRect;
    CStatic* pomStatic = NULL;
    GetClientRect(&WndClientRect);

    /* Resize "Message" static control */
    if ((pomStatic = (CStatic*) GetDlgItem(IDC_CAPTION)) != NULL)
    {
        pomStatic->GetWindowRect(&CtrlRect);
        ScreenToClient(&CtrlRect);
        pomStatic->MoveWindow(CtrlRect.left, CtrlRect.top,
                              WndClientRect.right - WndClientRect.left,
                              CtrlRect.bottom - CtrlRect.top);
    }

    /* Resize "Signal" static control */
    if ((pomStatic = (CStatic*) GetDlgItem(IDC_CAPTION2)) != NULL)
    {
        pomStatic->GetWindowRect(&CtrlRect);
        ScreenToClient(&CtrlRect);
        pomStatic->MoveWindow(CtrlRect.left, CtrlRect.top,
                              WndClientRect.right - WndClientRect.left,
                              CtrlRect.bottom - CtrlRect.top);
    }

    /* Resize list control describing signal values */
    if (IsWindow(m_ctrlSignal.m_hWnd))
    {
        m_ctrlSignal.GetWindowRect(&CtrlRect);
        ScreenToClient(&CtrlRect);
        m_ctrlSignal.MoveWindow(5, CtrlRect.top,
                                WndClientRect.right - WndClientRect.left - 10,
                                WndClientRect.bottom - CtrlRect.top - 5);
        m_ctrlSignal.GetWindowRect(&CtrlRect);
        // Get the column width
        int nWidth = (CtrlRect.right - CtrlRect.left - 20) / 3;
        m_ctrlSignal.SetColumnWidth(0, nWidth);
        m_ctrlSignal.SetColumnWidth(1, nWidth);
        m_ctrlSignal.SetColumnWidth(2, nWidth);
    }
}
/******************************************************************************
 Function Name  :   OnShowWindow

 Description    :
 Input(s)       :   BOOL bShow, UINT nStatus
 Output         :   -
 Functionality  :   Notified the parent window about the display status of
                    dialog by posting a message.
 Member of      :   CMessageInterpretation

 Author(s)      :   Ratnadip Choudhury
 Date Created   :   16.02.2004
 Modifications  :   Raja N on 05/04/2004, Added code to save window position and
                    removed resize of controls
******************************************************************************/
void CMessageInterpretation::OnShowWindow(BOOL bShow, UINT nStatus)
{
    CDialog::OnShowWindow(bShow, nStatus);
    ::PostMessage(m_hWndParent, WM_NOTIFICATION_FROM_OTHER,
                  eWINID_MSG_INTERPRETATION, (LPARAM) bShow);

    if (bShow == TRUE)
    {
        // Set the window position
        SetWindowPlacement(&m_sWinCurrStatus);
        // Call NC paint to update title bar
        SendMessage(WM_NCPAINT, 1, 0);
        // Update Client Area
        Invalidate();
    }
    else
    {
        // Hiding the window. Save the status before close
        vUpdateWndCo_Ords();
    }
}
/******************************************************************************
 Function Name  :   CMessageInterpretation

 Description    :
 Input(s)       :   -
 Output         :   TRUE or FALSE
 Functionality  :   Called by framework to initialise the dialog controls.
 Member of      :   CMessageInterpretation

 Author(s)      :   Ratnadip Choudhury
 Date Created   :   16.02.2004
 Modifications  :   Raja N on 05/04/2004, Added one more column to display
                    physical value and modified the existing column name as
                    "Raw Value". Added full row select property.
******************************************************************************/
BOOL CMessageInterpretation::OnInitDialog()
{
    CDialog::OnInitDialog();
    // TODO: Add extra initialization here
    BOOL bRetVal = FALSE;

    if (m_ctrlSignal.InsertColumn(0, defSTR_SIGNAL_NAME, LVCFMT_LEFT, 110) != -1)
    {
        if (m_ctrlSignal.InsertColumn(1, defSTR_PHYSICAL_COLUMN, LVCFMT_LEFT, 130) != -1
                &&
                m_ctrlSignal.InsertColumn(2, defSTR_RAW_COLUMN, LVCFMT_LEFT,      120) != -1
                &&
                m_ctrlSignal.InsertColumn(3, defSTR_UNITS, LVCFMT_LEFT,           120) != -1)
        {
            bRetVal = TRUE;
            m_ctrlSignal.SetExtendedStyle(m_ctrlSignal.GetExtendedStyle()
                                          | LVS_EX_GRIDLINES
                                          | LVS_EX_FULLROWSELECT);
        }
    }

    return bRetVal; // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

/******************************************************************************
 Function Name  :   vCreateMsgIntrprtnDlg

 Description    :
 Input(s)       :   CWnd* pomParent: Pointer to parent window.
                    BOOL bShow : TRUE or FALSE
 Output         :   -
 Functionality  :   This function will be called from CMsgWnd to created the
                    dialog. If the parameter bShow is FALSE, the dialog will
                    be created but not shown.
 Member of      :   CMessageInterpretation

 Author(s)      :   Ratnadip Choudhury
 Date Created   :   16.02.2004
 Modifications  :   Raja N on 14.04.2004
                    Added vGetWinStatus function call to get the window position
                    from the configuration file.
 Modifications  : Raja N on 18.04.2005, Modified to refer new window placement
                  identity - CRH0030
******************************************************************************/
void CMessageInterpretation::vCreateMsgIntrprtnDlg(CWnd* pomParent, BOOL bShow)
{
    if (IsWindow(m_hWnd) == FALSE)
    {
        this->Create(IDD_DLG_INTERPRET_MSG, pomParent);
        m_hWndParent = pomParent->m_hWnd;
        vGetWinStatus();

        if (bShow)
        {
            this->ShowWindow(SW_SHOW);
        }
    }
}

void CMessageInterpretation::vSetCaption(CString strCaption)
{
    this->m_strCaption = _T("  ")+strCaption;
    this->UpdateData(FALSE);
}

/******************************************************************************
 Function Name  :   OnClose

 Description    :
 Input(s)       :   -
 Output         :   -
 Functionality  :   This function will be called by framework when dialog is
                    closed. The dialog is put in hidden state instead of
                    closing it.
 Member of      :   CMessageInterpretation

 Author(s)      :   Ratnadip Choudhury
 Date Created   :   16.02.2004
 Modifications  :
******************************************************************************/
void CMessageInterpretation::OnClose()
{
    // TODO: Add your message handler code here and/or call default
    this->ShowWindow(SW_HIDE);
    ::PostMessage(m_hWndParent, WM_UPDATE_MSG_INTRP_WND_PLC , NULL, NULL);
    //CDialog::OnClose();
}
/******************************************************************************
 Function Name  :   DestroyWindow

 Description    :
 Input(s)       :
 Output         :   BOOL : TRUE or FALSE
 Functionality  :   This function is called by framework to destroy the window.
                    The brush object created is deleted in this function.
 Member of      :   CMessageInterpretation

 Author(s)      :   Ratnadip Choudhury
 Date Created   :   16.02.2004
 Modifications  :
******************************************************************************/
BOOL CMessageInterpretation::DestroyWindow()
{
    if(m_hBrushStatic != NULL )
    {
        DeleteObject(m_hBrushStatic);
    }

    return CDialog::DestroyWindow();
}

/*******************************************************************************
 Function Name  :   vUpdateMessageData
 Input(s)       :   unMsgID - Message ID (i/p)
                    omStrMsgName - Message Name (o/p)
                    omSASignals - Signal Names (o/p)
                    omSARaw - Calculated Raw Value
                    omSAPhy - Calculated Physical Value
                    bHexON - Hex Flag (i/p)
 Output         :   -
 Functionality  :   This function is called to calculate Raw & Physical values
                    of the signals of a message id. The message ID will be sent
                    to this fubction and Message Name, Signals, Raw Values &
                    Physical Values will be returned.
 Member of      :   CMessageInterpretation
 Author(s)      :   Raja N
 Date Created   :   31.03.2004
 Modifications  :
*******************************************************************************/
void CMessageInterpretation::vUpdateMessageData(UINT unMsgID,
        const CString& omStrMsgName,
        const CStringArray& omSASignals,
        const CStringArray& omSARaw,
        const CStringArray& omSAPhy,
        const CStringArray& omSAUnits,
        BOOL bHexON)
{
    ASSERT(omSASignals.GetSize() == omSARaw.GetSize() ||
           omSARaw.GetSize() == omSAPhy.GetSize());
    BOOL bCLearFlag = FALSE;

    // Clear the list
    if( m_omMsgName != omStrMsgName)
    {
        bCLearFlag = TRUE;
        m_ctrlSignal.DeleteAllItems();
    }

    // Get the size to avoid function call in the loop
    UINT unSize = omSASignals.GetSize();

    // Add each items in to the list
    for( register UINT index = 0; index < unSize; index++)
    {
        // Signal Name
        if(bCLearFlag == TRUE)
        {
            m_ctrlSignal.InsertItem(index, omSASignals.GetAt(index));
        }

        // Physical Value in second col
        m_ctrlSignal.SetItemText(index, 1, omSAPhy.GetAt(index));
        //Raw Value in Third Col
        m_ctrlSignal.SetItemText(index, 2, omSARaw.GetAt(index));
        //Units in fourth column
        m_ctrlSignal.SetItemText(index, 3, omSAUnits.GetAt(index));
    }

    // Set Message ID
    m_omMsgID.Format(bHexON ? defFORMAT_MSGID_HEX_STR : defFORMAT_MSGID_DECIMAL,
                     unMsgID);
    // Set the Message Name
    m_omMsgName = omStrMsgName;
    // Update Values
    GetDlgItem(IDC_EDIT_MSG_NAME)->SetWindowText(m_omMsgName);
    GetDlgItem(IDC_EDIT_MSG_ID)->SetWindowText(m_omMsgID);
    // Show window
    ShowWindow(SW_SHOW);
}

/******************************************************************************
 Function Name    : OnCreate
 Input(s)         : LPCREATESTRUCT
 Output           : int
 Functionality    : Called by the framework while a window is being created.
                    This will upadte the window placement information from
                    config module
 Member of        : CMessageInterpretation
 Friend of        : -
 Author(s)        : Raja N
 Date Created     : 05.04.2004
 Modifications    : Raja N on 18.04.2005, Modified to refer new window placement
                    identity - CRH0030
*******************************************************************************/
int CMessageInterpretation::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    int nCreateFlag = -1;

    if (CDialog::OnCreate(lpCreateStruct) != -1)
    {
        nCreateFlag = 0;
        vGetWinStatus();
    }

    return nCreateFlag;
}

/******************************************************************************
 Function Name  : vUpdateWndCo_Ords
 Input(s)       :
 Output         :
 Functionality  : This function is called from OnDestroy function of this class.
                  Updates the window co-ords to the config class if modified.
 Member of      : CMessageInterpretation
 Friend of      :      -
 Author(s)      : Raja N
 Date Created   : 05.04.2004
 Modifications  : Raja N on 26.04.2005, Modified save functionality. Removed
                  parameter to vSaveWinStatus as it is already member of this
                  class
******************************************************************************/
void CMessageInterpretation::vUpdateWndCo_Ords()
{
    if( ::IsWindow(m_hWnd))
    {
        vSaveWinStatus();
    }
}

/******************************************************************************
 Function Name  : vSaveWinStatus

 Input(s)       : -
 Output         : -
 Functionality  : This function is called to save the current display status
                  of window to be used at the next time window is shown
 Member of      : CMessageInterpretation
 Friend of      :    -
 Author(s)      : Raja N
 Date Created   : 05.04.2004
 Modifications  : Raja N on 18.04.2005, Modified to refer new window placement
                  identity - CRH0030
******************************************************************************/
void CMessageInterpretation::vSaveWinStatus()
{
    // write into the configuration file..
    WINDOWPLACEMENT sCurrentPosition;
    // Get current placement
    GetWindowPlacement( &sCurrentPosition );

    // Compare the two WINDOWPLACEMENT type structures to find out if there
    // is any modification done in the window coordinates or state. A blind
    // bit pattern comparison between the two is justifiable because none
    // of the members of the structure is a pointer.
    if (memcmp( &sCurrentPosition,
                &m_sWinCurrStatus,
                sizeof(WINDOWPLACEMENT)) != 0)
    {
        // Save co-ord into the config class
        m_sWinCurrStatus = sCurrentPosition;
        // This flag is required to store minimised postion
        m_sWinCurrStatus.flags = WPF_SETMINPOSITION;
        // Update configuration module
    }
}

/******************************************************************************
 Function Name  : vGetWinStatus
 Input(s)       : sWinCurrStatus - A reference to (system defined)
                  WINDOWPLACEMENT structure to contain previous display
                  status of this window to be retrieved from the registry
 Output         : -
 Functionality  : This function is called to get the display status of window
                  stored
 Member of      : CMessageInterpretation
 Friend of      :    -
 Author(s)      : Raja N
 Date Created   : 05.04.2004
 Modifications  : Raja N on 18.04.2005, Modified to refer new window placement
                  identity - CRH0030
******************************************************************************/
void CMessageInterpretation::vGetWinStatus()
{
#if 0
    // Get window postion details from the configuration file
    WINDOWPLACEMENT* pDetails = &m_sWinCurrStatus;
    // Get data from the configuration module
    theApp.bGetData(MSGINTERP_WND_PLACEMENT, (void**)&pDetails);
    // Add this flag as this is required to restore minimised window postion
    // also. Refer WINDOWPLACEMENT Structure help from MSDN
    m_sWinCurrStatus.flags |= WPF_SETMINPOSITION;

    // Check for initial condition
    if( m_sWinCurrStatus.rcNormalPosition.bottom == -1 ||
            m_sWinCurrStatus.rcNormalPosition.top == -1 ||
            m_sWinCurrStatus.rcNormalPosition.left == -1 ||
            m_sWinCurrStatus.rcNormalPosition.right == -1 )
    {
        // Get Propotionate Initial value from Config Module
        theApp.bGetDefaultValue( MSGINTERP_WND_PLACEMENT, m_sWinCurrStatus );
        // Update Config Module to update Window position value
        theApp.bSetData( MSGINTERP_WND_PLACEMENT, (void*)&m_sWinCurrStatus);
    }

#endif
}

/******************************************************************************
 Function Name  : OnCancel

 Description    : The framework calls this member function prior to closing the
                  dialog box after selecing Cancel
 Input(s)       : -
 Output         : -
 Functionality  : This will be called when user hits the 'Escape' key. This
                  dialog should not be closed by Esc key. So ignore the
                  message by not calling the parent function which closes the
                  dialog
 Member of      : CMessageInterpretation
 Author(s)      : Raja N
 Date Created   : 05/04/2004
******************************************************************************/
void CMessageInterpretation::OnCancel()
{
    // Don't call parent's oncancel
}

/******************************************************************************
 Function Name  : OnOK

 Description    : The framework calls this member function prior to closing
                  the dialog box after seelcting OK
 Input(s)       :  -
 Output         :  -
 Functionality  : This will be called when user hits the 'Enter' key. This
                  dialog should not be closed by Enter key. So ignore the
                  message by not calling the parent function which closes the
                  dialog
 Member of      : CMessageInterpretation
 Author(s)      : Raja N
 Date Created   : 05/04/2004
******************************************************************************/
void CMessageInterpretation::OnOK()
{
    // Don't call parent's OnOk
}

/*******************************************************************************
 Function Name    : vUpdateWinStatus
 Input(s)         :
 Output           :
 Functionality    : This is a public member function defined . It is called
                    whenever a new configuration file is loaded or created.
 Member of        : CMessageInterpretation
 Friend of        :   -
 Author(s)        : Raja N
 Date Created     : 06.04.2004
 Modified by      :
 Modifications    : Raja N on 18.04.2005, Modified to refer new window placement
                  identity - CRH0030
*******************************************************************************/
void CMessageInterpretation::vUpdateWinStatus()
{
    if(::IsWindow(GetSafeHwnd()))
    {
        vGetWinStatus();
        SetWindowPlacement(&m_sWinCurrStatus);
    }
}

/*******************************************************************************
 Function Name    : vClearWindowContent
 Input(s)         :
 Output           :
 Functionality    : This is a public member function defined . It is called
                    whenever a new configuration file is loaded or created. This
                    will clear entries in the list control & message name and ID
 Member of        : CMessageInterpretation
 Friend of        :   -
 Author(s)        : Raja N
 Date Created     : 06.04.2004
 Modified by      :
 Modification     :
*******************************************************************************/
void CMessageInterpretation::vClearWindowContent()
{
    // Remove elements in the List
    m_ctrlSignal.DeleteAllItems();
    // Clear Message ID
    m_omMsgID = STR_EMPTY;
    // Clear Message Name
    m_omMsgName = STR_EMPTY;
    // Update Values
    UpdateData(FALSE);
}

/*******************************************************************************
 Function Name    : OnMove
 Input(s)         : int x, int y
 Output           :
 Functionality    : Updates the Parent window with window placement.
 Member of        : CMessageInterpretation
 Friend of        :   -
 Author(s)        : Raja N
 Date Created     : 06.04.2004
 Modified by      :
 Modification     :
*******************************************************************************/
void CMessageInterpretation::OnMove(int x, int y)
{
    CDialog::OnMove(x, y);
    ::PostMessage(m_hWndParent, WM_UPDATE_MSG_INTRP_WND_PLC , NULL, NULL);
}
