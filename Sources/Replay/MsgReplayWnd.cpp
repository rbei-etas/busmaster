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
 * \file      MsgReplayWnd.cpp
 * \brief     This file contain the definition all member function of
 * \authors   Amitesh Bharti, Krishnaswamy B.N
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * This file contain the definition all member function of
 */
// For all standard header file include
#include "Replay_stdafx.h"
#include "Replay_resource.h"
#include "MsgReplayWnd.h"
// Definition of CSplFileDlg class
//#include "Utility/SplFileDlg.h"
// For replay manager class declaration
#include "ReplayManager.h"

extern BOOL bGetMsgInfoFromMsgStr(CONST CString& omSendMsgLine,
                           PSTCANDATA psCANData,
                           BOOL bHexON);

/******************************************************************************/
/*  Function Name    :  CMsgReplayWnd                                         */
/*  Input(s)         :                                                        */
/*  Output           :                                                        */
/*  Functionality    :  This is default constructor                           */
/*                                                                            */
/*  Member of        :  CMsgReplayWnd                                         */
/*  Friend of        :      -                                                 */
/*  Author(s)        :  Amitesh Bharti                                        */
/*  Date Created     :  19.03.2002                                            */
/*  Modifications    :  Raja N on 26.04.2005                                  */
/*                      Added code to init window identity of base class      */
/*  Modifications    :  Raja N on 16.07.2005                                  */
/*                      Removed code for storing window position              */
/******************************************************************************/
CMsgReplayWnd::CMsgReplayWnd(CReplayFile ouReplayFile, WINDOWPLACEMENT& sWndPlacement) :
                m_ouReplayDetails( ouReplayFile ),
                m_eReplayState( REPLAY_INVALID )
{
    m_sWndPlacement = sWndPlacement;
}


/******************************************************************************/
/*  Function Name    :  ~CMsgReplayWnd                                        */
/*  Input(s)         :                                                        */
/*  Output           :                                                        */
/*  Functionality    :  This is default destructor                            */
/*                                                                            */
/*  Member of        :  CMsgReplayWnd                                         */
/*  Friend of        :      -                                                 */
/*  Author(s)        :  Amitesh Bharti                                        */
/*  Date Created     :  19.03.2002                                            */
/*  Modifications    :                                                        */
/*                                                                            */
/******************************************************************************/
CMsgReplayWnd::~CMsgReplayWnd()
{

}


BEGIN_MESSAGE_MAP(CMsgReplayWnd, CMDIChildWnd)
    //{{AFX_MSG_MAP(CMsgReplayWnd)
    ON_WM_HELPINFO()
    ON_WM_DESTROY()
    ON_WM_SIZE()
    ON_WM_CLOSE()
    ON_WM_MDIACTIVATE()
    ON_WM_ERASEBKGND()
    ON_MESSAGE( WM_OWNER_GET_DATA, vHandleListControlDataReq)
    ON_WM_SHOWWINDOW()
    ON_MESSAGE(WM_OW_LIST_DBLCLK, vListDoubleClick)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/******************************************************************************/
/*  Function Name    :  OnCreateClient                                        */
/*  Input(s)         :                                                        */
/*  Output           :                                                        */
/*  Functionality    :  This function is called to attach the list box with   */
/*                      Replay window dynamically                             */
/*                                                                            */
/*  Member of        :  COutWnd                                               */
/*  Friend of        :      -                                                 */
/*  Author(s)        :  Amitesh Bharti                                        */
/*  Date Created     :  19.03.2002                                            */
/*  Modification By  :  Amitesh Bharti                                        */
/*  Modifications on :  03.04.2002, create listbox with multiselection or     */
/*                              single selection depending on mode of replay. */
/*  Modifications    :  Raja N on 26.04.2005, Changed the base class to       */
/*                      CMDIChildBase                                         */
/*  Modifications    :  Raja N on 16.07.2005, Added code to init image list   */
/*                      and message list control                              */
/******************************************************************************/
BOOL CMsgReplayWnd::OnCreateClient(LPCREATESTRUCT lpcs,
                                   CCreateContext* pContext) 
{
    //Creation of the listbox window 
    CRect sRect(0,0,0,0);
    this->GetClientRect(sRect);
    int nReplayMode = m_ouReplayDetails.m_ouReplayFile.m_nReplayMode;
    DWORD wListStyle = WS_CHILD | WS_VISIBLE | WS_BORDER |
                      LVS_REPORT | LVS_NOCOLUMNHEADER | LVS_SHOWSELALWAYS |
                      LVS_OWNERDATA;    // Virtual List control
    
    if( nReplayMode == defREPLAY_MODE_MONOSHOT)
    {
        wListStyle |= LVS_SINGLESEL;
    }
    
    m_omMessageList.Create( wListStyle, sRect, this, IDC_LSTB_REPLAY );
    m_omMessageList.SetExtendedStyle( LVS_EX_FULLROWSELECT );
    // Create Image
    if( m_omImageList.Create( IDB_BMP_BREAKPOINT,
                              defICON_SIZE,
                              defICON_GROW,
                              WHITE_COLOR ) == TRUE )
    {
        m_omMessageList.SetImageList( &m_omImageList, LVSIL_SMALL );
    }
    // Insert invisible column
    m_omMessageList.InsertColumn( 0, STR_EMPTY );
    // Set the window icon
    SetIcon( AfxGetApp()->LoadIcon(IDI_ICO_REPLAY), TRUE);
 
   return CMDIChildWnd::OnCreateClient(lpcs, pContext);
}
/******************************************************************************/
/*  Function Name    :  OnHelpInfo                                            */
/*  Input(s)         :                                                        */
/*  Output           :                                                        */
/*  Functionality    :  This function is called when user request for context */
/*                      sensitive help. The help ID will be assign to public  */
/*                      data member of app class                              */
/*  Member of        :  CMsgReplayWnd                                         */
/*  Friend of        :      -                                                 */
/*  Author(s)        :  Amitesh Bharti                                        */
/*  Date Created     :  19.03.2002                                            */
/*  Modifications    :  Raja N on 26.04.2005, Changed the base class to       */
/*                      CMDIChildBase                                         */
/*  Modifications    :  Raja N on 20.07.2005, Changed the base class to       */
/*                      back to CMDIChildWnd to avoid window position save    */
/******************************************************************************/
BOOL CMsgReplayWnd::OnHelpInfo(HELPINFO* pHelpInfo) 
{
    //theApp.vSetHelpID(pHelpInfo->dwContextId);
    return CMDIChildWnd::OnHelpInfo(pHelpInfo);
}
/******************************************************************************/
/*  Function Name    :  OnDestroy                                             */
/*  Input(s)         :                                                        */
/*  Output           :                                                        */
/*  Functionality    :  This function is called to destroy the window. The    */
/*                      coordinate are stored in registry or ini file         */
/*                                                                            */
/*  Member of        :  CMsgReplayWnd                                         */  
/*  Friend of        :      -                                                 */
/*  Author(s)        :  Amitesh Bharti                                        */
/*  Date Created     :  19.03.2002                                            */
/*  Modifications By :  Amitesh Bhart                                         */
/*                      12.04.2002, Initialise the locat variable             */
/*  Modifications    :  Raja N on 26.04.2005, Changed the base class to       */
/*                      CMDIChildBase                                         */
/*  Modifications    :  Raja N on 16.07.2005, Added code to remove this window*/
/*                      pointer from the replay manager window pointers list  */
/******************************************************************************/
void CMsgReplayWnd::OnDestroy() 
{
    bStopReplayThread();
    CReplayManager::ouGetReplayManager().bRemovePointerFromList( this );
    CMDIChildWnd::OnDestroy();
}
/******************************************************************************/
/*  Function Name    :  OnSize                                                */
/*  Input(s)         :                                                        */
/*  Output           :                                                        */
/*  Functionality    :  This function is called when user resize the window   */
/*                                                                            */
/*  Member of        :  CMsgReplayWnd                                         */
/*  Friend of        :      -                                                 */
/*  Author(s)        :  Amitesh Bharti                                        */
/*  Date Created     :  19.03.2002                                            */
/*  Modifications By :  Amitesh Bhart                                         */
/*                      12.04.2002, Initialise the local variable             */
/*  Modifications    :  Raja N on 26.04.2005, Changed the base class to       */
/*                      CMDIChildBase                                         */
/*  Modifications    :  Raja N on 20.07.2005, Changed the base class to       */
/*                      back to CMDIChildWnd to avoid window position save    */
/*                      and to resize the list control col width              */
/******************************************************************************/
void CMsgReplayWnd::OnSize(UINT nType, int cx, int cy) 
{
    //Resizing the Listbox wnd when the output window 
    //is resized
    CRect omRect;
    CMDIChildWnd::OnSize(nType, cx, cy);
    this->GetClientRect(omRect);
    m_omMessageList.MoveWindow(omRect,TRUE);
    m_omMessageList.GetClientRect( omRect );
    m_omMessageList.SetColumnWidth(0, omRect.Width());
}
/******************************************************************************/
/*  Function Name    :  bOpenReplayFile                                       */
/*  Input(s)         :                                                        */
/*  Output           :   TRUE or FALSE                                        */
/*  Functionality    :  This function is called to open the log file and add  */
/*                      all items line by line in the list box                */
/*                                                                            */
/*  Member of        :  CMsgReplayWnd                                         */
/*  Friend of        :      -                                                 */
/*  Author(s)        :  Amitesh Bharti                                        */
/*  Date Created     :  19.03.2002                                            */
/*  Modifications By :  Amitesh Bhart                                         */
/*                      12.04.2002, Initialise the locat variable             */
/*                      09.10.2002, Exclude line with starting string hash    */
/*                                   defined as DefSPL_LINE                   */
/*  Modifications By :  Krishnaswamy B.N.                                     */
/*                      24.05.2003, Added code to read the mode of reply      */
/*  Modifications By :  Krishnaswamy B.N.                                     */
/*                      03.06.2003, Added code to read the time mode          */
/*                      06.06.2003, Removed hard code after code review       */
/*  Modifications By :  Krishnaswamy B.N.                                     */
/*                      19.08.2003, Added code for replaying in append  mode  */
/*  Modifications By :  Raja N on 16.07.2005                                  */
/*                      Modified code to refer Replay Process function for    */
/*                      parsing the file and use owner data list for message  */
/*                      entries                                               */
/******************************************************************************/
BOOL CMsgReplayWnd::bOpenReplayFile()
{
    BOOL bReturn = m_ouReplayDetails.bOpenReplayFile();

    // Update No. of Items
    if( bReturn == TRUE )
    {
        if( m_ouReplayDetails.m_omEntries.GetSize() > 0 )
        {
            m_omMessageList.SetItemCountEx(
                    (int)m_ouReplayDetails.m_omEntries.GetSize());
            m_omMessageList.SetItemState( 0, LVIS_SELECTED | LVIS_FOCUSED,
                                             LVIS_SELECTED | LVIS_FOCUSED );
            m_eReplayState = REPLAY_TO_START;
            m_ouReplayDetails.m_nUserSelectionIndex = 0;
            // Send message to resize list control
            SendMessage( WM_SIZE );
        }
        else
        {
            // Throw error message
            CString omStrErr;
            omStrErr.Format( defSTR_REPLAY_ERROR,
                m_ouReplayDetails.m_ouReplayFile.m_omStrFileName, // File
                defSTR_REPLAY_FILE_EMPTY );
            CReplayManager::ouGetReplayManager().vSendToTrace(omStrErr.GetBuffer(MAX_PATH));
            // Close the window
            DestroyWindow();
        }
    }
    else
    {
        // Show Error message
        CString omStrErr;
        omStrErr.Format( defSTR_REPLAY_ERROR,
            m_ouReplayDetails.m_ouReplayFile.m_omStrFileName, // File Name
            m_ouReplayDetails.m_omStrError );                 // Error Message
        CReplayManager::ouGetReplayManager().vSendToTrace(omStrErr.GetBuffer(MAX_PATH));
        //gbSendStrToTrace( (char *)omStrErr.operator LPCTSTR() );
        // Close the window
        DestroyWindow();
    }
    return bReturn;
}

/******************************************************************************/
/*  Function Name    :  vCmdGo                                                */
/*  Input(s)         :                                                        */
/*  Output           :                                                        */
/*  Functionality    :  This function is called when user select to go through*/
/*                      the replay of messages displayed on window from       */
/*                      selection till end in single or cyclic mode. Called by*/
/*                      OnMessageGo message mapped function                   */
/*  Member of        :  CMsgReplayWnd                                         */
/*  Friend of        :      -                                                 */
/*  Author(s)        :  Amitesh Bharti                                        */
/*  Date Created     :  19.03.2002                                            */
/*  Modifications By :  Amitesh Bhart                                         */
/*                      12.04.2002, Initialise the local variable, check the  */
/*                                  return values                             */
/*  Modifications By :  Amitesh Bhart                                         */
/*  Modification on  :  15.10.2002, New data member "m_bStepCmdReq" of        */ 
/*                                  structure psReplayData  is assigned the   */
/*                                  value FALSE to indicate a GO command req. */
/*  Modified On      :  28.08.2002, Changes improve accuracy of time delay    */
/*                                between two consecutive message transmission*/
/*                                Struncture name changed and new structure   */
/*  Modified On      :  27.12.2002, Thread handle is obtained and assigned to */
/*                      public data member of CCANMonitorApp class            */
/*                      pointer to static global g_ssReplayInfo is assigned to*/
/*                      psReplayInfo and passed to thread function.           */
/*  Modified On      :  17.03.2003, Before adding element remove content of   */
/*                      psReplayInfo->m_omReplayStrArray having address of    */
/*                      of a static variable.                                 */
/*  Modifications By :  Krishnswamy B.N.                                      */
/*                      24.05.2003, Mode of reply is added to psReplayInfo    */
/*                      03.06.2003, Time Mode is added to psReplayInfo        */
/*  Modifications By :  Raja N on 16.07.2005                                  */
/*                      Modifications due to interactive and noninteractive   */
/*                      replay                                                */
/******************************************************************************/
VOID CMsgReplayWnd::vCmdGo()
{
    if( m_eReplayState == REPLAY_TO_START )
    {
        // For monoshot block
        if( m_ouReplayDetails.m_ouReplayFile.m_nReplayMode ==
                    defREPLAY_MODE_MONOSHOT )
        {
            // Update Current Selection
            m_ouReplayDetails.m_nUserSelectionIndex =
                m_omMessageList.GetNextItem( -1, LVNI_SELECTED );
            if( m_ouReplayDetails.m_nUserSelectionIndex < 0 )
            {
                m_ouReplayDetails.m_nUserSelectionIndex = 0;
            }
            // Find the next index of break point
            int nCount = (int)m_ouReplayDetails.m_omEntries.GetSize();
            
            BOOL bBreakPointFound = FALSE;
            int nIndex; //index declared outside
            for( nIndex = m_ouReplayDetails.m_nUserSelectionIndex + 1;
                 nIndex < nCount&& bBreakPointFound == FALSE; nIndex++ )
            {
                if( m_ouReplayDetails.m_omBreakPoints[ nIndex ] == TRUE )
                {
                    bBreakPointFound = TRUE;
                }
            }
            
            // Get the count
            if( bBreakPointFound == TRUE )
            {
                m_ouReplayDetails.m_nNoOfMessagesToPlay = nIndex -
                                m_ouReplayDetails.m_nUserSelectionIndex - 1;
            }
            else // Till the end
            {
                m_ouReplayDetails.m_nNoOfMessagesToPlay = nCount -
                                m_ouReplayDetails.m_nUserSelectionIndex;
            }
            
            if( m_ouReplayDetails.m_nNoOfMessagesToPlay > 0 )
            {
                // Create the thread
                CWinThread * pThread =
                    AfxBeginThread( CReplayProcess::sunReplayMonoshotThreadFunc,
                                    &m_ouReplayDetails );
                if( pThread != NULL )
                {
                    m_ouReplayDetails.m_hThread = pThread->m_hThread;
                    m_eReplayState = REPLAY_RUNNING;
                    m_ouReplayDetails.m_bStopReplayThread = FALSE;
                }
            }
        }
        else // Cyclic block
        {
            vGetSelectedIndeces( m_ouReplayDetails.m_omSelectedIndex );
            m_ouReplayDetails.m_nNoOfMessagesToPlay  =
                (int)m_ouReplayDetails.m_omSelectedIndex.GetSize();
            if( m_ouReplayDetails.m_nNoOfMessagesToPlay > 0 )
            {
                // Create Replay Thread
                CWinThread * pThread =
                    AfxBeginThread( CReplayProcess::sunReplayCyclicThreadFunc,
                                    &m_ouReplayDetails );
                if( pThread != NULL )
                {
                    m_ouReplayDetails.m_hThread = pThread->m_hThread;
                    m_eReplayState = REPLAY_RUNNING;
                    m_ouReplayDetails.m_bStopReplayThread = FALSE;
                }
            }
        }
    }
}
/******************************************************************************/
/*  Function Name    :  vCmdStep                                              */
/*  Input(s)         :                                                        */
/*  Output           :                                                        */
/*  Functionality    :  This function is called  by OnMessageStep message     */
/*                      mapped function. This will send the current selected  */
/*                      on CAN bus and select the next message in message list*/
/*                      displayed.                                            */
/*  Member of        :  CMsgReplayWnd                                         */
/*  Friend of        :      -                                                 */
/*  Author(s)        :  Amitesh Bharti                                        */
/*  Date Created     :  07.03.2002                                            */
/*  Modification By  :  Amitesh Bharti                                        */
/*  Modified On      :  12.04.2002, used global function bGetMsgInfoFromMsgStr*/
/*  Modifications By :  Amitesh Bhart                                         */
/*  Modified On      :  15.10.2002, New thread is used to send message. Data  */ 
/*                                  members of structure psReplayData is      */
/*                                  assigned the values to be used in thread  */
/*                                  Removed bGetMsgInfoFromMsgStr(..)         */
/*  Modified On      :  28.08.2002, Changes improve accuracy of time delay    */
/*                                between two consecutive message transmission*/
/*                                Struncture name changed and new structure   */
/*  Modified On      :  27.12.2002, Thread handle is obtained and assigned to */
/*                      public data member of CCANMonitorApp class            */
/*                      pointer to static global g_ssReplayInfo is assigned to*/
/*                      psReplayInfo and passed to thread function.           */
/*  Modified On      :  17.03.2003, Before adding element remove content of   */
/*                      psReplayInfo->m_omReplayStrArray having address of    */
/*                      of a static variable.                                 */
/*  Modifications By :  Krishnswamy B.N.                                      */
/*                      24.05.2003, Mode of reply is added to psReplayInfo    */
/*                      03.06.2003, Time Mode is added to psReplayInfo        */
/*  Modifications    :  Raja N on 20.07.2005, Changes due to new replay       */
/*                      architecture                                          */
/*  Modifications    :  ArunKumar K on 06.05.2011,							  */ 
/*						introduced m_eReplayState variable status check       */
/*                      before setting it to REPLAY_RUNNING                   */
/******************************************************************************/
VOID CMsgReplayWnd::vCmdStep()
{
    if( m_eReplayState == REPLAY_TO_START )
    {
        // For monoshot block
        if( m_ouReplayDetails.m_ouReplayFile.m_nReplayMode ==
                    defREPLAY_MODE_MONOSHOT )
        {
            // Update Current Selection
            m_ouReplayDetails.m_nUserSelectionIndex =
                m_omMessageList.GetNextItem( -1, LVNI_SELECTED );
            if( m_ouReplayDetails.m_nUserSelectionIndex < 0 )
            {
                m_ouReplayDetails.m_nNoOfMessagesToPlay = 0;
            }
            else
            {
                m_ouReplayDetails.m_nNoOfMessagesToPlay = 1;
            }

            if( m_ouReplayDetails.m_nNoOfMessagesToPlay > 0 )
            {
                m_eReplayState = REPLAY_RUNNING;
                // Create the thread
                CWinThread * pThread =
                    AfxBeginThread( CReplayProcess::sunReplayMonoshotThreadFunc,
                                    &m_ouReplayDetails );				
                if( pThread != NULL )
                {
                    m_ouReplayDetails.m_hThread = pThread->m_hThread;
					if(m_eReplayState != REPLAY_TO_START)
						m_eReplayState = REPLAY_RUNNING;
                    m_ouReplayDetails.m_bStopReplayThread = FALSE;
                }
            }
        }
    }
}
/******************************************************************************/
/*  Function Name    :  vCmdStop                                              */
/*  Input(s)         :                                                        */
/*  Output           :                                                        */
/*  Functionality    :  This function is from OnMessageStop message map func. */
/*                      operation. It sets the member variable flag g_bStop   */
/*                                                                            */
/*  Member of        :  CMsgReplayWnd                                         */
/*  Friend of        :      -                                                 */
/*  Author(s)        :  Amitesh Bharti                                        */
/*  Date Created     :  19.03.2002                                            */
/*  Modifications    :  Raja N on 20.07.2005, Changes due to new replay       */
/*                      architecture                                          */
/******************************************************************************/
VOID CMsgReplayWnd::vCmdStop()
{
    m_ouReplayDetails.m_bStopReplayThread = TRUE;
}
/******************************************************************************/
/*  Function Name    :  vCmdSkip                                              */
/*  Input(s)         :                                                        */
/*  Output           :                                                        */
/*  Functionality    :  This function is called when user wants to skip the   */
/*                      message while in replay single lap run mode           */
/*                                                                            */
/*  Member of        :  CMsgReplayWnd                                         */
/*  Friend of        :      -                                                 */
/*  Author(s)        :  Amitesh Bharti                                        */
/*  Date Created     :  20.03.2002                                            */
/*  Modifications    :  Raja N on 20.07.2005, Changes due to new replay       */
/*                      architecture                                          */
/******************************************************************************/
VOID CMsgReplayWnd::vCmdSkip()
{
    // Get the current selection
    INT nItem = m_omMessageList.GetNextItem(-1, LVNI_SELECTED);
    if( nItem != -1 )
    {
        nItem++;
        nItem %= m_omMessageList.GetItemCount();
        m_omMessageList.SetItemState( nItem, LVIS_SELECTED | LVIS_FOCUSED,
                                      LVIS_SELECTED | LVIS_FOCUSED );
        m_ouReplayDetails.m_nUserSelectionIndex = nItem;
        m_omMessageList.EnsureVisible( nItem, FALSE );
    }
}
/******************************************************************************/
/*  Function Name    :  SetWindowFont                                         */
/*  Input(s)         :                                                        */
/*  Output           :                                                        */
/*  Functionality    :  Set the font for list box attached to this window     */
/*  Member of        :  CMsgReplayWnd                                         */
/*  Friend of        :      -                                                 */
/*  Author(s)        :  Amitesh Bharti                                        */
/*  Date Created     :  04.04.2002                                            */
/*  Modification By  :                                                        */
/*  Modified On      :                                                        */
/******************************************************************************/
void CMsgReplayWnd::SetWindowFont() 
{
   // Set the font of replay window list box
   LOGFONT LF;
   memset(&LF, 0, sizeof(LF));  //zero out structure

   LF.lfHeight = 10;
   LF.lfWidth  = 0;
   LF.lfItalic = FALSE;
   LF.lfUnderline = FALSE;
   LF.lfStrikeOut = FALSE;
   LF.lfOutPrecision = OUT_CHARACTER_PRECIS;
   LF.lfClipPrecision = CLIP_CHARACTER_PRECIS;
   LF.lfPitchAndFamily  = FIXED_PITCH | FF_SWISS;
   lstrcpy(LF.lfFaceName, "Courier");

   LF.lfWeight = FW_NORMAL; 
   if (m_omNewFont.CreateFontIndirect(&LF) == TRUE) 
   {
        // font setting.
        m_omMessageList.SetFont(&m_omNewFont,TRUE);
   }
   else 
   {
       AfxMessageBox("Font creation unsuccessful");
   }

}

/******************************************************************************/
/*  Function Name    :  OnClose                                               */
/*  Input(s)         :                                                        */
/*  Output           :                                                        */
/*  Functionality    :  Called by Framework to close the window. Set the flag */
/*                      for seletec file and g_bStop to FALSE.                */
/*  Member of        :  CMsgReplayWnd                                         */
/*  Friend of        :      -                                                 */
/*  Author(s)        :  Amitesh Bharti                                        */
/*  Date Created     :  05.04.2002                                            */
/*  Modification By  :                                                        */
/*  Modifications    :  Raja N on 26.04.2005, Changed the base class to       */
/*                      CMDIChildBase                                         */
/*  Modifications    :  Raja N on 20.07.2005, Changes due to new replay       */
/*                      architecture                                          */
/******************************************************************************/
void CMsgReplayWnd::OnClose() 
{
    bSetThreadStopSignal();
    CMDIChildWnd::OnClose();
}

/******************************************************************************/
/*  Function Name    :  bCreateReplayWindow                                   */
/*  Input(s)         :                                                        */
/*  Output           :  TRUE or FALSE                                         */
/*  Functionality    :  Create Replay window, if it is not created. It uses   */
/*                      default or last stored coordinates                    */
/*  Member of        :  CMsgReplayWnd                                         */
/*  Friend of        :      -                                                 */
/*  Author(s)        :  Amitesh Bharti                                        */
/*  Date Created     :  18.04.2003                                            */
/*  Modifications    :  Raja N on 20.07.2005, Changes due to new replay       */
/*                      architecture                                          */
/******************************************************************************/
BOOL CMsgReplayWnd::bCreateReplayWindow()
{
    INT nReturn = 0;
    // If the co-ordiantes are not correct, calculate the default value 
    CRect omRect;
    if (m_sWndPlacement.length == 0)
    {
        AfxGetApp()->GetMainWnd()->GetWindowPlacement(&m_sWndPlacement);
    }
    CString omStrTitle = defSTR_REPLAY_WINDOW_TITLE +
                    m_ouReplayDetails.m_ouReplayFile.m_omStrFileName;
    nReturn     = Create( NULL,
                          omStrTitle, 
                          WS_CHILD   |  
                          WS_VISIBLE | WS_OVERLAPPEDWINDOW, 
                          m_sWndPlacement.rcNormalPosition,
                          NULL, 
                          NULL );
    SetWindowFont();
    SendMessage(WM_NCPAINT, 1, 0);
    // Update Window Pointer in the Replay information object
    m_ouReplayDetails.m_pReplayWndPtr = this;
    // Update window with paint
    UpdateWindow();

    return nReturn;
}

/*******************************************************************************
  Function Name  : OnMDIActivate
  Input(s)       : bActivate - TRUE if activated, FALSE otherwise
                   pActivateWnd - New Active Window Pointer
                   pDeactivateWnd - Old Active window
  Output         : -
  Functionality  : This function will be called during focus change across MDI
                   child frames. This will update replay manager's active
                   window pointer.
  Member of      : CMsgReplayWnd
  Author(s)      : Raja N
  Date Created   : 16.7.2005
  Modifications  : 
*******************************************************************************/
void CMsgReplayWnd::OnMDIActivate( BOOL bActivate,
                                   CWnd* pActivateWnd,
                                   CWnd* pDeactivateWnd )
{
    CMDIChildWnd::OnMDIActivate(bActivate, pActivateWnd, pDeactivateWnd);
    // Set the pointer in the replay mamager class
    CReplayManager::ouGetReplayManager().vSetActiveReplayWindow( bActivate ? 
                                                                 this : NULL );
}

/*******************************************************************************
  Function Name  : OnEraseBkgnd
  Input(s)       : pDC - Device context to the Screen
  Output         : -
  Functionality  : This will return simply as the whole window area is occupied
                   by the list control. This will avoid flickering
  Member of      : CMsgReplayWnd
  Author(s)      : Raja N
  Date Created   : 16.07.2005
  Modifications  : 
*******************************************************************************/
BOOL CMsgReplayWnd::OnEraseBkgnd(CDC* pDC) 
{
    UNUSED_ALWAYS( pDC );
    return TRUE;
}

/*******************************************************************************
  Function Name  : vHandleListControlDataReq
  Input(s)       : -
  Output         : -
  Functionality  : This is the event handler for owner data. This will supply
                   the data requested by the list control.
  Member of      : CMsgReplayWnd
  Author(s)      : Raja N
  Date Created   : 16.7.2005
  Modifications  : 
*******************************************************************************/
LRESULT CMsgReplayWnd::vHandleListControlDataReq( WPARAM wParam, LPARAM /*lParam*/)
{
    LV_DISPINFO* pDispInfo = (LV_DISPINFO*)wParam;
    LV_ITEM* pItem= &(pDispInfo)->item;

    // Text Request
    if (pItem->mask & LVIF_TEXT) 
    {
        if( pItem->iSubItem == 0 &&
            pItem->iItem < m_ouReplayDetails.m_omEntries.GetSize() )
        {
            strcpy(pItem->pszText, m_ouReplayDetails.m_omEntries[ pItem->iItem]);   
        }
    }

    // Image Request
    if(pItem->mask & LVIF_IMAGE )
    {
        pItem->iImage = m_ouReplayDetails.m_omBreakPoints[ pItem->iItem];
    }
  return 0;
}

/*******************************************************************************
  Function Name  : vListDoubleClick
  Input(s)       : -
  Output         : -
  Functionality  : This function will be called during double click of list
                   control. This will toggle the break point of the selected
                   message
  Member of      : CMsgReplayWnd
  Author(s)      : Raja N
  Date Created   : 16.7.2005
  Modifications  : 
*******************************************************************************/
LRESULT CMsgReplayWnd::vListDoubleClick(WPARAM wParam, LPARAM /*lParam*/)
{
    if( m_ouReplayDetails.m_ouReplayFile.m_nReplayMode ==
                                        defREPLAY_MODE_MONOSHOT )
    {
        int nItem = (int)wParam;
        // Toggle the break point status
        m_ouReplayDetails.m_omBreakPoints[ nItem ] =
            !m_ouReplayDetails.m_omBreakPoints[ nItem ];
        // Update the list
        m_omMessageList.Invalidate();
    }
    return 0;
}

/*******************************************************************************
  Function Name  : bGetUIStateCmdStep
  Input(s)       : -
  Output         : BOOL - TRUE to enable and FALSE to disable
  Functionality  : To get the enable status of Step toolbar button
  Member of      : CMsgReplayWnd
  Author(s)      : Raja N
  Date Created   : 16.7.2005
  Modifications  : 
*******************************************************************************/
BOOL CMsgReplayWnd::bGetUIStateCmdStep()
{
    BOOL bEnable = FALSE;
    if( m_eReplayState == REPLAY_TO_START &&
        m_ouReplayDetails.m_ouReplayFile.m_nReplayMode == 
                                    defREPLAY_MODE_MONOSHOT )
    {
        bEnable = TRUE;
    }
    return bEnable;
}

/*******************************************************************************
  Function Name  : bGetUIStateCmdSkip
  Input(s)       : -
  Output         : BOOL - TRUE to enable and FALSE to disable
  Functionality  : To get the enable status of Skip toolbar button
  Member of      : CMsgReplayWnd
  Author(s)      : Raja N
  Date Created   : 16.7.2005
  Modifications  : 
*******************************************************************************/
BOOL CMsgReplayWnd::bGetUIStateCmdSkip()
{
    BOOL bEnable = FALSE;
    if( m_eReplayState == REPLAY_TO_START &&
        m_ouReplayDetails.m_ouReplayFile.m_nReplayMode == 
                                    defREPLAY_MODE_MONOSHOT )
    {
        bEnable = TRUE;
    }
    return bEnable;
}

/*******************************************************************************
  Function Name  : bGetUIStateCmdGo
  Input(s)       : -
  Output         : BOOL - TRUE to enable and FALSE to disable
  Functionality  : To get the enable status of Go toolbar button
  Member of      : CMsgReplayWnd
  Author(s)      : Raja N
  Date Created   : 16.7.2005
  Modifications  : 
*******************************************************************************/
BOOL CMsgReplayWnd::bGetUIStateCmdGo()
{
    BOOL bEnable = FALSE;
    if( m_eReplayState == REPLAY_TO_START )
    {
        bEnable = TRUE;
    }
    return bEnable;
}

/*******************************************************************************
  Function Name  : bGetUIStateCmdStop
  Input(s)       : -
  Output         : BOOL - TRUE to enable and FALSE to disable
  Functionality  : To get the enable status of Stop toolbar button
  Member of      : CMsgReplayWnd
  Author(s)      : Raja N
  Date Created   : 16.7.2005
  Modifications  : 
*******************************************************************************/
BOOL CMsgReplayWnd::bGetUIStateCmdStop()
{
    BOOL bEnable = FALSE;
    if( m_eReplayState == REPLAY_RUNNING )
    {
        bEnable = TRUE;
    }
    return bEnable;
}

/*******************************************************************************
  Function Name  : bSetThreadStopSignal
  Input(s)       : -
  Output         : BOOL - TRUE if the thread is running
  Functionality  : To set the thread stop signal to all replay process
  Member of      : CMsgReplayWnd
  Author(s)      : Raja N
  Date Created   : 16.7.2005
  Modifications  : 
*******************************************************************************/
BOOL CMsgReplayWnd::bSetThreadStopSignal()
{
    BOOL bIsValid = FALSE;
    if( m_eReplayState == REPLAY_RUNNING )
    {
        // Indicate to stop the thread
        m_ouReplayDetails.m_bStopReplayThread = TRUE;
        bIsValid = TRUE;
    }
    return bIsValid;
}


/*******************************************************************************
  Function Name  : bStopReplayThread
  Input(s)       : -
  Output         : BOOL - Is thread Killed status
  Functionality  : To kill all replay process and to clear data structures
  Member of      : CReplayManager
  Author(s)      : Raja N
  Date Created   : 16.7.2005
  Modifications  : 
*******************************************************************************/
BOOL CMsgReplayWnd::bStopReplayThread()
{
    BOOL bIsThreadKilled = FALSE;
    // Kill the replay thread if it is not responding
    if( m_eReplayState == REPLAY_RUNNING )
    {
        if( WaitForSingleObject( m_ouReplayDetails.m_omThreadEvent,
                defINTERACTIVE_THREAD_WAIT_TIME ) != WAIT_OBJECT_0 )
        {
            TerminateThread(m_ouReplayDetails.m_hThread, 0 );
            m_ouReplayDetails.m_hThread = NULL;
            bIsThreadKilled = TRUE;
            m_eReplayState = REPLAY_INVALID;
        }
    }
    return bIsThreadKilled;
}

/*******************************************************************************
  Function Name  : vGetSelectedIndeces
  Input(s)       : omIndexList - List of indeces
  Output         : -
  Functionality  : To get all selected indeces from the list control
  Member of      : CMsgReplayWnd
  Author(s)      : Raja N
  Date Created   : 16.7.2005
  Modifications  : 
*******************************************************************************/
VOID CMsgReplayWnd::vGetSelectedIndeces( CArray<int,int>& omIndexList )
{
    // Remove all elements from the array
    omIndexList.RemoveAll();
    // Get the position first
    POSITION pos = m_omMessageList.GetFirstSelectedItemPosition();
    // Get the list
    while( pos != NULL )
    {
        int nIndex = m_omMessageList.GetNextSelectedItem( pos );
        omIndexList.Add( nIndex );
    }
}