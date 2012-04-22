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
 * \file      SigWatchDlg.cpp
 * \brief     Implementation file for CSigWatchDlg class
 * \authors   Amarnath Shastri, Ratnadip Choudhury
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Implementation file for CSigWatchDlg class
 */

// For all standard header file include
#include "SignalWatch_stdafx.h"
// Interface file for CSigWatchDlg class
#include "SignalWatch_Resource.h"
#include "DataTypes/SigWatch_DataTypes.h"
#include "SignalWatchDefs.h"
#include "SigWatchDlg.h"
// Interface file for CMsgInterpretation class

static const int LSB_MOTOROLA = 0x7; // 7th bit is the LSB for motorola

extern HWND GUI_hDisplayWindow;
extern VOID vConvStrtoByteArray(
               CByteArray* pomByteArrayBufTx,
               CHAR* pctempBuf,BOOL bHexON);
/////////////////////////////////////////////////////////////////////////////
// CSigWatchDlg dialog


/******************************************************************************
 Function Name  :   CSigWatchDlg

 Description    :   Standard default constructor for dialog box
 Member of      :   CSigWatchDlg
 Functionality  :   Initialises data members

 Author(s)      :   Ratnadip Choudhury
 Date Created   :   03-04-2002
******************************************************************************/
CSigWatchDlg::CSigWatchDlg(CWnd* pParent /*=NULL*/)
    : CDialog(CSigWatchDlg::IDD, pParent)
{
    //{{AFX_DATA_INIT(CSigWatchDlg)
        // NOTE: the ClassWizard will add member initialization here
    //}}AFX_DATA_INIT

    m_bEscape = false;
}

/******************************************************************************
 Function Name  :   ~CSigWatchDlg

 Description    :   Standard destructor
 Member of      :   CSigWatchDlg
 Functionality  :   Releases all the memory allocated

 Author(s)      :   Ratnadip Choudhury
 Date Created   :   03-04-2002
******************************************************************************/
void CSigWatchDlg::DoDataExchange(CDataExchange* pDX)
{
    m_omSignalList.vUpdateParentWnd(GetSafeHwnd());
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CSigWatchDlg)
    DDX_Control(pDX, IDC_LIST_SIGNAL_WATCH, m_omSignalList);
    //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSigWatchDlg, CDialog)
    //{{AFX_MSG_MAP(CSigWatchDlg)
    ON_WM_SIZE()
    ON_WM_CLOSE()
    ON_WM_SHOWWINDOW()
    ON_WM_CREATE()
    ON_WM_ERASEBKGND()
    ON_MESSAGE(WM_REMOVE_SIGNAL,vRemoveSignalFromMap)
    //}}AFX_MSG_MAP
    ON_WM_TIMER()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSigWatchDlg message handlers

/******************************************************************************
 Function Name  :   OnSize

 Description    :   The framework calls this member function after the window’s
                    size has changed.
 Input(s)       :   nType - Specifies the type of resizing requested
                    ncx - Specifies the new width of the client area
                    ncy - Specifies the new height of the client area
 Output         :   -
 Functionality  :   Adjusts dimensions of the list box so that it keeps
                    occupying the entire client area of the dialog box
 Member of      :   CSigWatchDlg

 Author(s)      :   Ratnadip Choudhury
 Date Created   :   03-04-2002
******************************************************************************/
void CSigWatchDlg::OnSize(UINT nType, int ncx, int ncy) 
{
    CDialog::OnSize(nType, ncx, ncy);
    
    // TODO: Add your message handler code here
    if (IsWindowVisible()) 
    {
        RECT sClientRect;
        GetClientRect(&sClientRect);
        int ClientWidth = abs(sClientRect.left - sClientRect.right);
        m_omSignalList.SetColumnWidth(0, (int)(0.2 * ClientWidth));
        m_omSignalList.SetColumnWidth(1, (int)(0.2 * ClientWidth));
        m_omSignalList.SetColumnWidth(2, (int)(0.4 * ClientWidth));
        m_omSignalList.SetColumnWidth(3, (int)(0.2 * ClientWidth));
        m_omSignalList.MoveWindow(&sClientRect);
    }
}

/******************************************************************************
 Function Name  :   OnCancel

 Description    :   The framework calls this member function prior to closing
                    the dialog box
 Input(s)       :   -
 Output         :   -
 Functionality  :   To close the dialog box only when the closure process is
                    not initiated by hitting the 'Escape' key. 
 Member of      :   CSigWatchDlg

 Author(s)      :   Ratnadip Choudhury
 Date Created   :   03-04-2002
 Modifications  :   Raja N on 02.05.2005, Removed cancel check and removed
                    calling base class Cancel function which will close the
                    window.
******************************************************************************/
void CSigWatchDlg::OnCancel()
{

}

/******************************************************************************
 Function Name  :   OnOK

 Description    :   The framework calls this member function prior to closing
                    the dialog box on press of Enter key. This should be
                    prevented by ignoring the call
 Input(s)       :   -
 Output         :   -
 Functionality  :   To close the dialog box only when the closure process is
                    not initiated by hitting the 'Escape' key. 
 Member of      :   CSigWatchDlg

 Author(s)      :   Raja N
 Date Created   :   13.04.2004
******************************************************************************/
void CSigWatchDlg::OnOK()
{
    // Don't call parents OnOK function
}

void CSigWatchDlg::vAddMsgSigIntoList(   const CString& omStrMsgName,
                                         const CStringArray& omSASignals,
                                         const CStringArray& omSARaw,
                                         const CStringArray& omSAPhysical,
                                         BOOL bIntptrDone)
{
    m_omCSDispEntry.Lock();

    ASSERT( omSASignals.GetSize() == omSARaw.GetSize() &&
        omSARaw.GetSize() == omSAPhysical.GetSize() );

    if( bIntptrDone == FALSE)
    {
        int nSize = (int)omSASignals.GetSize();
        for( register int index = 0; index < nSize; index++)
        {
            CString omStrSignalInfo;
            CString omListStr = omSASignals.GetAt(index);
            POSITION pos = NULL;
            sSIGENTRY sEntry;
            sEntry.m_omMsgName = omStrMsgName;
            sEntry.m_omSigName = omListStr;
            sEntry.m_omPhyValue = omSAPhysical.GetAt(index);
            sEntry.m_omRawValue = omSARaw.GetAt(index);
            if (NULL != (pos = m_odSigEntryList.Find(sEntry)))
            {
                sSIGENTRY& sTemp = m_odSigEntryList.GetAt(pos);
                sTemp.m_omPhyValue = sEntry.m_omPhyValue;
                sTemp.m_omRawValue = sEntry.m_omRawValue;
            }
            else
            {
                m_odSigEntryList.AddTail(sEntry);
            }
        }
    }
    m_omCSDispEntry.Unlock();
}
void CSigWatchDlg::vDisplayMsgSigList(void)
{
    m_omCSDispEntry.Lock();

    POSITION pos = m_odSigEntryList.GetHeadPosition();
    //UINT unCount = 0;
    while (pos)
    {
        sSIGENTRY& sEntry = m_odSigEntryList.GetNext(pos);
        // Insert Message Name
        if (sEntry.m_nEntryIndex != -1)
        {
            m_omSignalList.SetItemText( sEntry.m_nEntryIndex,
                                        defSTR_SW_MSG_COL,
                                        sEntry.m_omMsgName);
            // Insert Signal Name
            m_omSignalList.SetItemText( sEntry.m_nEntryIndex,
                                        defSTR_SW_SIG_COL,
                                        sEntry.m_omSigName);
            // Insert Raw Value
            m_omSignalList.SetItemText( sEntry.m_nEntryIndex,
                                        defSTR_SW_RAW_VAL_COL,
                                        sEntry.m_omRawValue);
            // Insert Physical Value
            m_omSignalList.SetItemText( sEntry.m_nEntryIndex,
                                        defSTR_SW_PHY_VAL_COL,
                                        sEntry.m_omPhyValue);
        }
        else
        {
            sEntry.m_nEntryIndex = m_omSignalList.InsertItem(
                                    m_omSignalList.GetItemCount(),
                                    sEntry.m_omMsgName);
            m_omSignalList.SetItemText( sEntry.m_nEntryIndex,
                                        defSTR_SW_SIG_COL,
                                        sEntry.m_omSigName);
            m_omSignalList.SetItemText( sEntry.m_nEntryIndex,
                                        defSTR_SW_RAW_VAL_COL,
                                        sEntry.m_omRawValue);
            m_omSignalList.SetItemText( sEntry.m_nEntryIndex,
                                            defSTR_SW_PHY_VAL_COL,
                                            sEntry.m_omPhyValue);

        }

    }
    m_omCSDispEntry.Unlock();
}

/******************************************************************************
 Function Name  :   OnClose
 Input(s)       :   
 Output         :   -
 Functionality  :   Called when the user clicks on Close[X] button
                    Sends a message to the message window to close
                    the watch window.
 Member of      :   CSigWatchDlg
 Author(s)      :   Amarnath Shastry
 Date Created   :   15.05.2002
******************************************************************************/
void CSigWatchDlg::OnClose() 
{
    // Call Show Window to save co ordinates
    ShowWindow(SW_HIDE);
}

/*******************************************************************************
 Function Name  :   vRemoveSignalFromMap
 Input(s)       :   
 Output         :   -
 Functionality  :   Called when the signal is deleted from the watch window. 
                    This function will remove the entry in m_omSignalMap data
                    member and read the content of list box. After reading the 
                    content it will intialise the m_omSignalMap with new index.
                    The map should have same memory address as key to ensure
                    that it insert the signal value at correct place. So the
                    signal name is searched through the list and signal name
                    pointer is copied.
 Member of      :   CSigWatchDlg
 Author(s)      :   Amitesh Bharti
 Date Created   :   16.05.2003
 Modifications  :   Raja N on 05/04/2004, Clear will empty the map. It will not
                    manipulate the data structure
*******************************************************************************/
LRESULT CSigWatchDlg::vRemoveSignalFromMap(WPARAM /*wParam*/, LPARAM /*lParam*/)
{
    // remove all entry from the map
    m_omCSDispEntry.Lock();
    m_omSignalList.DeleteAllItems();
    m_odSigEntryList.RemoveAll();
    m_omCSDispEntry.Unlock();
    return 0;
}

/******************************************************************************
 Function Name  :   OnShowWindow

 Description    :   Indicates that a window is about to be shown or hidden. The
                    framework calls this member function when the CWnd object 
                    is about to be hidden or shown
 Input(s)       :   bShow - TRUE if the window is being shown; it is FALSE if 
                    the window is being hidden
                    nStatus - Specifies the status of the window being shown
                    It is 0 if the message is sent because of a ShowWindow 
                    member function call; otherwise nStatus is one of the 
                    following: 
                        SW_PARENTCLOSING - Parent window is closing (being made
                                     iconic) or a pop-up window is being hidden
                        SW_PARENTOPENING - Parent window is opening (being 
                                   displayed) or a pop-up window is being shown
 Output         :   -
 Functionality  :   Prior to the window being shown, set the show state and 
                    different positions of the window exactly as the same when
                    previous instance of it was destroyed
 Member of      :   CSigWatchDlg

 Author(s)      :   Raja N
 Date Created   :   05.04.2004
******************************************************************************/
void CSigWatchDlg::OnShowWindow(BOOL bShow, UINT nStatus) 
{
    CDialog::OnShowWindow(bShow, nStatus);
    
    if (bShow == TRUE )
    {
        SendMessage(WM_NCPAINT, 1, 0);
    }
}

/******************************************************************************
 Function Name  : vSaveWinStatus

 Input(s)       : sWinCurrStatus - This contains information about the 
                  placement of a window on the screen
 Output         : -
 Functionality  : This function is called to save the current display status
                  of window to be used at the next time window is shown
 Member of      : CSigWatchDlg
 Friend of      :    -
 Author(s)      : Raja N
 Date Created   : 05.04.2004
 Modifications  : Raja N on 26.04.2005, Modified the prototype of this function
                  to avoid passing member variable as parameter
******************************************************************************/
void CSigWatchDlg::vSaveDefaultWinStatus( )
{
    // Get current window postion
    GetWindowPlacement(&m_sWinCurrStatus);
    m_sWinCurrStatus.flags |= WPF_SETMINPOSITION;
    for (UINT i = 0; i < defSW_LIST_COLUMN_COUNT; i++)
    {
       m_anColWidth[i] =  m_omSignalList.GetColumnWidth(i);
    }
}

/******************************************************************************
 Function Name  : vGetWinStatus

 Input(s)       : sWinCurrStatus - A reference to (system defined) 
                  WINDOWPLACEMENT structure to contain previous display 
                  status of this window to be retrieved from the registry
 Output         :  -
 Functionality  : This function is called to get the display status of window
                  stored
 Member of      : CSigWatchDlg
 Friend of      :  -
 Author(s)      : Raja N
 Date Created   : 05.04.2004
 Modifications  : Raja N on 26.04.2005, Changed code to get window position from
                  configuration file using window identity.
******************************************************************************/
void CSigWatchDlg::vSetDefaultWinStatus()
{
    SetWindowPlacement(&m_sWinCurrStatus);
    for (UINT i = 0; i < defSW_LIST_COLUMN_COUNT; i++)
    {
        m_omSignalList.SetColumnWidth(i, m_anColWidth[i]);
    }
}

/******************************************************************************
 Function Name    : OnCreate
 Input(s)         : LPCREATESTRUCT
 Output           : int
 Functionality    : Called by the framework while a window is being created.
                    This will upadte the window placement information from
                    config module
 Member of        : CSigWatchDlg
 Friend of        : -
 Author(s)        : Raja N
 Date Created     : 05.04.2004
 Modification     : Raja N on 26.04.2005, Modified the prototype of function
                    vGetWinStatus.
*******************************************************************************/
int CSigWatchDlg::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
    int nCreateFlag = -1;
    if (CDialog::OnCreate(lpCreateStruct) != -1)
    {
        nCreateFlag = 0;
    }
     return nCreateFlag;
}



/*******************************************************************************
 Function Name    : OnInitDialog
 Input(s)         :
 Output           :
 Functionality    : This is function will be called after creating the dialog.
                    This will init dialog controls
 Member of        : CSigWatchDlg
 Friend of        :   -
 Author(s)        : Raja N
 Date Created     : 22.07.2004
 Modified by      :
 Modification     :
*******************************************************************************/
BOOL CSigWatchDlg::OnInitDialog() 
{
    CDialog::OnInitDialog();
    vInitSignalWatchList();
    m_unTimerId = (UINT)SetTimer(0x123, 50, NULL);
    return TRUE;  // return TRUE unless you set the focus to a control
                  // EXCEPTION: OCX Property Pages should return FALSE
}

/*******************************************************************************
 Function Name    : vInitSignalWatchList
 Input(s)         :
 Output           :
 Functionality    : This is initialise the signal watch list with column header
                    and will insert necessery columns
 Member of        : CSigWatchDlg
 Friend of        :   -
 Author(s)        : Raja N
 Date Created     : 22.07.2004
 Modified by      :
 Modification     :
*******************************************************************************/
void CSigWatchDlg::vInitSignalWatchList()
{
    TCHAR caColumnName[][50] = { defSTR_SW_MSG_NAME,
                                defSTR_SW_SIG_NAME,
                                defSTR_SW_PHY_VALUE,
                                defSTR_SW_RAW_VALUE };
    INT nColumnFormat[]     = { LVCFMT_LEFT,
                                LVCFMT_LEFT,
                                LVCFMT_LEFT,
                                LVCFMT_CENTER };


    RECT rListCtrlRect;
    INT nTotalColunmSize = 0;
    INT nTotalStrLengthPixel = 0;
    //INT nColumnSize = 0;
    
    // Create Image List First time
    // Only One time creation
    if( m_omSigImageList.m_hImageList == NULL )
    {
        m_omSigImageList.Create(IDR_BMP_MSG_SIG_WATCH,
                                 16,
                                 1,
                                 defCOLOR_WHITE);
    }
    m_omSignalList.SetImageList(&m_omSigImageList, LVSIL_SMALL);
    
    //Calculate the total size of all column header   
    m_omSignalList.GetWindowRect( &rListCtrlRect);
    nTotalColunmSize     = rListCtrlRect.right - rListCtrlRect.left;
    nTotalStrLengthPixel = 0;

    for(UINT i = 0; i<defSW_LIST_COLUMN_COUNT;i++)
    {
         nTotalStrLengthPixel += 
             m_omSignalList.GetStringWidth(caColumnName[i]) ;
    }
    
    m_omSignalList.InsertColumn(0, caColumnName[0],nColumnFormat[0], (int)(0.2 * nTotalColunmSize));
    m_omSignalList.InsertColumn(1, caColumnName[1],nColumnFormat[1], (int)(0.2 * nTotalColunmSize));
    m_omSignalList.InsertColumn(2, caColumnName[2],nColumnFormat[2], (int)(0.4 * nTotalColunmSize));
    m_omSignalList.InsertColumn(3, caColumnName[3],nColumnFormat[3], (int)(0.2 * nTotalColunmSize));
    // Set the extended style
    m_omSignalList.SetExtendedStyle( /*GetExStyle() |*/
                                      LVS_EX_GRIDLINES |
                                      LVS_EX_FULLROWSELECT);

}

/*******************************************************************************
 Function Name    : OnEraseBkgnd
 Input(s)         :
 Output           :
 Functionality    : This function will be called by the framework to erase the
                    background. Return TRUE to show that background has been
                    erased
 Member of        : CSigWatchDlg
 Friend of        :   -
 Author(s)        : Raja N
 Date Created     : 22.07.2004
 Modified by      :
 Modification     :
*******************************************************************************/
BOOL CSigWatchDlg::OnEraseBkgnd(CDC* /*pDC*/) 
{
    return TRUE;
}
/*bGetDefaultValue
case SIGWATCH_WND_PLACEMENT:   // Non - MDI Child
{
    CRect omRect = omMainFrameSize;
    // Make the height 35% pf main frame height
    omRect.bottom = static_cast<LONG> ( omRect.top +
		omRect.Height() * defSIGNAL_WATCH_WND_HEIGHT_RATIO );
    // Make the width 65% of main frame
    omRect.left += (LONG)( omRect.Width() *
		defSIGNAL_WATCH_WND_WIDTH_RATIO );
    // Shift the window down by tool bar size
    omRect.top += omToolBarRect.Height();
    omRect.bottom += omToolBarRect.Height();
    // Reduce the border size
    omRect.right -= nBorderWidth;
    // Convert into screen
    pMainFrame->ClientToScreen(&omRect);
    // Assign the rect
    sPosition.rcNormalPosition = omRect;
}
break;*/
void CSigWatchDlg::OnTimer(UINT nIDEvent)
{
    // TODO: Add your message handler code here and/or call default
    CDialog::OnTimer(nIDEvent);
    if (m_unTimerId == nIDEvent)
    {
        vDisplayMsgSigList();
    }
    
}
