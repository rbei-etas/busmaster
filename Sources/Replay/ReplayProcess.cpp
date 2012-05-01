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
 * \file      ReplayProcess.cpp
 * \brief     Implementation file for CReplayProcess class
 * \author    Raja N
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Implementation file for CReplayProcess class
 */


#include "Replay_stdafx.h"                 // For stand includes
#include "ReplayFile.h"             // For CReplayFile class declaration
#include "ReplayProcess.h"          // For Replay Process class declaration
#include "MsgReplayWnd.h"           // For Replay Window class declaration
// For DIL Interface Class
#include "Include/BaseDefs.h"
#include "DataTypes/Base_WrapperErrorLogger.h"
#include "DataTypes/MsgBufAll_Datatypes.h"
#include "DataTypes/DIL_Datatypes.h"
#include "DataTypes/Filter_Datatypes.h"
#include "DIL_Interface/BaseDIL_CAN.h"
#include "DIL_Interface/DIL_Interface_extern.h"
#include "include/CAN_Error_Defs.h"         // For Errors
#include "Utility_Replay.h"

using namespace std;




CBaseDIL_CAN* CReplayProcess::s_pouDIL_CAN_Interface = NULL;
DWORD CReplayProcess::s_dwClientID = 0;

/*******************************************************************************
  Function Name  : CReplayProcess
  Description    : Standard default constructor
  Member of      : CReplayProcess
  Functionality  : This will initialise local variables
  Author(s)      : Raja N
  Date Created   : 16.7.2005
  Modifications  :
*******************************************************************************/
CReplayProcess::CReplayProcess(const CReplayFile& ouReplayFile) :
    m_ouReplayFile( ouReplayFile ),
    m_pReplayWndPtr( NULL ),
    m_omBreakPoints( defBREAK_POINT_MAP_SIZE ),
    m_hThread( NULL ),
    m_bReplayHexON( TRUE ),
    m_wLogReplayTimeMode(0 ),
    m_nCurrentIndex( 0 ),
    m_nUserSelectionIndex( 0 ),
    m_nNoOfMessagesToPlay( 0 ),
    m_bStopReplayThread( TRUE )
{
    m_omEntries.RemoveAll();
    m_omMsgList.RemoveAll();
    m_omSelectedIndex.RemoveAll();
    //DIL related code
    DIL_GetInterface(CAN, (void**)&CReplayProcess::s_pouDIL_CAN_Interface);
    CReplayProcess::s_pouDIL_CAN_Interface->DILC_RegisterClient( TRUE,
            CReplayProcess::s_dwClientID,
            CAN_MONITOR_NODE);
    InitializeCriticalSection(&m_omCritSecFilter);
}

/*******************************************************************************
  Function Name  : ~CReplayProcess
  Description    : Standard Destructor
  Member of      : CReplayProcess
  Functionality  : -
  Author(s)      : Raja N
  Date Created   : 16.7.2005
  Modifications  :
*******************************************************************************/
CReplayProcess::~CReplayProcess()
{
    DeleteCriticalSection(&m_omCritSecFilter);
}

/*******************************************************************************
  Function Name  : sunReplayMonoshotThreadFunc
  Input(s)       : pParam - Parameter to the thread
  Output         : -
  Functionality  : This is the thread function for monoshot replay blocks
  Member of      : CReplayProcess
  Author(s)      : Raja N
  Date Created   : 16.7.2005
  Modifications  : Raja N on 26.07.2005, Implemented code review points
*******************************************************************************/
const int SIZE_STCAN_MSG = sizeof(STCAN_MSG);
UINT CReplayProcess::sunReplayMonoshotThreadFunc( LPVOID pParam )
{
    CReplayProcess* pReplayDetails = (CReplayProcess*)pParam;

    if( pReplayDetails != NULL )
    {
        CMsgReplayWnd* pWnd = (CMsgReplayWnd*)pReplayDetails->m_pReplayWndPtr;
        // Reset the event
        pReplayDetails->m_omThreadEvent.ResetEvent();

        // Disable List control
        // To avoid flickering avoid disabling for a step action
        if( pWnd != NULL && pReplayDetails->m_nNoOfMessagesToPlay > 1)
        {
            pWnd->m_omMessageList.EnableWindow( FALSE );
        }

        // Replay code here
        /// Get the number of messages to play
        int nCount = pReplayDetails->m_nNoOfMessagesToPlay;
        int nOffset = pReplayDetails->m_nUserSelectionIndex;
        // Time Calculation
        CArray<UINT,UINT> omTimeDelay;

        if( pReplayDetails->m_ouReplayFile.m_nTimeMode == defREPLAY_RETAIN_DELAY
                && nCount > 1 )
        {
            CString omStrCurr;
            CString omStrNext;

            for( int nIndex = 0; nIndex < nCount - 1; nIndex++ )
            {
                // Get the current entry
                omStrNext = pReplayDetails->m_omEntries[ nIndex + nOffset + 1];
                omStrCurr = pReplayDetails->m_omEntries[ nIndex + nOffset ];
                UINT unTime = unTimeDiffBetweenMsg( omStrNext, omStrCurr,
                                                    pReplayDetails->m_wLogReplayTimeMode );

                if( unTime == 0 )
                {
                    unTime = 1;
                }

                omTimeDelay.Add( unTime );
            }
        }

        TIMECAPS time;
        MMRESULT mmResult = TIMERR_NOERROR;

        if (timeGetDevCaps(&time, sizeof(TIMECAPS)) == TIMERR_NOERROR)
        {
            mmResult = timeBeginPeriod(time.wPeriodMin);
        }

        // Create the event object to wait for
        HANDLE hEventReplayWait = CreateEvent(NULL, FALSE, FALSE, NULL);
        // Assign the message delay time
        int nDelay = pReplayDetails->m_ouReplayFile.m_unMsgTimeDelay;

        // main loop for message transmission.
        for( int nIndex = 0;
                pReplayDetails->m_bStopReplayThread == FALSE && nIndex < nCount;
                nIndex++ )
        {
            int nCurrentIndex = nIndex + nOffset;

            if( ( nIndex + 1 ) < nCount )
            {
                if( pReplayDetails->m_ouReplayFile.m_nTimeMode ==
                        defREPLAY_RETAIN_DELAY )
                {
                    nDelay = omTimeDelay[ nIndex ];
                }

                timeSetEvent( nDelay, time.wPeriodMin,
                              (LPTIMECALLBACK) hEventReplayWait, NULL,
                              TIME_CALLBACK_EVENT_SET | TIME_ONESHOT);
            }

            // Send message in CAN bus if the message ID is valid
            if ( pReplayDetails->m_omMsgList[ nCurrentIndex].
                    m_uDataInfo.m_sCANMsg.m_unMsgID != -1 )
            {
                // Use HIL Function to send CAN message
                HRESULT hRet =  s_pouDIL_CAN_Interface->DILC_SendMsg(s_dwClientID,
                                pReplayDetails->m_omMsgList[ nCurrentIndex ].
                                m_uDataInfo.m_sCANMsg );

                if (hRet != defERR_OK)
                {
                    //::PostMessage(GUI_hDisplayWindow, WM_ERROR,
                    //            ERROR_DRIVER_API_FAIL, nZERO);
                }
            }

            // Increment cur sel to set to next sel
            pReplayDetails->m_nCurrentIndex = nCurrentIndex;

            // last message is already sent
            if( nIndex + 1 == nCount )
            {
                pReplayDetails->m_bStopReplayThread = TRUE;
            }
            else
            {
                // Wait for the event
                WaitForSingleObject(hEventReplayWait, INFINITE);
            }
        }

        if (mmResult == TIMERR_NOERROR)
        {
            timeEndPeriod(time.wPeriodMin);
        }

        CloseHandle(hEventReplayWait);

        if( pWnd != NULL )
        {
            pWnd->m_omMessageList.EnableWindow( );
            pWnd->m_eReplayState = REPLAY_TO_START;
            // Set the selection
            pReplayDetails->m_nCurrentIndex++;
            pReplayDetails->m_nCurrentIndex %=
                pReplayDetails->m_omEntries.GetSize();
            pWnd->m_omMessageList.SetItemState( pReplayDetails->m_nCurrentIndex,
                                                LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED );
            pWnd->m_omMessageList.EnsureVisible(
                pReplayDetails->m_nCurrentIndex, FALSE );
        }

        pReplayDetails->m_omThreadEvent.SetEvent();
    }

    return 0;
}

/*******************************************************************************
  Function Name  : sunReplayCyclicThreadFunc
  Input(s)       : pParam - Parameter to the thread
  Output         : -
  Functionality  : This is the thread function for cyclic replay blocks
  Member of      : CReplayProcess
  Author(s)      : Raja N
  Date Created   : 16.7.2005
  Modifications  :
*******************************************************************************/
UINT CReplayProcess::sunReplayCyclicThreadFunc( LPVOID pParam )
{
    CReplayProcess* pReplayDetails = (CReplayProcess*)pParam;

    if( pReplayDetails != NULL )
    {
        CMsgReplayWnd* pWnd = (CMsgReplayWnd*)pReplayDetails->m_pReplayWndPtr;
        // Reset the event
        pReplayDetails->m_omThreadEvent.ResetEvent();

        // Disable List control
        if( pWnd != NULL )
        {
            pWnd->m_omMessageList.EnableWindow( FALSE );
        }

        // Replay code here
        /// Get the number of messages to play
        int nCount = pReplayDetails->m_nNoOfMessagesToPlay;
        // Time Calculation
        CArray<UINT,UINT> omTimeDelay;
        CString omStrCurr;
        CString omStrNext;
        UINT unMsgDelay = pReplayDetails->m_ouReplayFile.m_unMsgTimeDelay;

        if( unMsgDelay == 0 )
        {
            unMsgDelay = 1;
        }

        int nIndex; //nIndex declared outside

        for( nIndex = 0; nIndex < nCount - 1; nIndex++ )
        {
            if( pReplayDetails->m_ouReplayFile.m_nTimeMode ==
                    defREPLAY_RETAIN_DELAY )
            {
                int nCurrentIndex = pReplayDetails->m_omSelectedIndex[ nIndex ];
                int nNextIndex =
                    pReplayDetails->m_omSelectedIndex[ nIndex + 1 ];
                // Get the current entry
                omStrNext = pReplayDetails->m_omEntries[ nNextIndex ];
                omStrCurr = pReplayDetails->m_omEntries[ nCurrentIndex ];
                UINT unTime = unTimeDiffBetweenMsg( omStrNext, omStrCurr,
                                                    pReplayDetails->m_wLogReplayTimeMode );

                if( unTime == 0 )
                {
                    unTime = 1;
                }

                omTimeDelay.Add( unTime );
            }
            else
            {
                omTimeDelay.Add( unMsgDelay );
            }
        }

        // Add the cyclic delay at the end
        unMsgDelay = pReplayDetails->m_ouReplayFile.m_unCycleTimeDelay;

        if( unMsgDelay == 0 )
        {
            unMsgDelay = 1;
        }

        omTimeDelay.Add( unMsgDelay );
        TIMECAPS time;
        MMRESULT mmResult = TIMERR_NOERROR;

        if (timeGetDevCaps(&time, sizeof(TIMECAPS)) == TIMERR_NOERROR)
        {
            mmResult = timeBeginPeriod(time.wPeriodMin);
        }

        // Create the event object to wait for
        HANDLE hEventReplayWait = CreateEvent(NULL, FALSE, FALSE, NULL);
        // main loop for message transmission.
        nIndex = 0;

        while( pReplayDetails->m_bStopReplayThread == FALSE )
        {
            int nCurrentIndex = pReplayDetails->m_omSelectedIndex[ nIndex ];
            // Set the event to wait
            timeSetEvent( omTimeDelay[ nIndex ], time.wPeriodMin,
                          (LPTIMECALLBACK) hEventReplayWait, NULL,
                          TIME_CALLBACK_EVENT_SET | TIME_ONESHOT);

            // Send message in CAN bus if the message ID is valid
            if ( pReplayDetails->m_omMsgList[ nCurrentIndex ].
                    m_uDataInfo.m_sCANMsg.m_unMsgID != -1 )
            {
                // Use HIL Function to send CAN message
                HRESULT hRet = s_pouDIL_CAN_Interface->DILC_SendMsg(s_dwClientID,
                               pReplayDetails->m_omMsgList[ nCurrentIndex ].m_uDataInfo.m_sCANMsg );

                if (hRet != defERR_OK)
                {
                    //::PostMessage(GUI_hDisplayWindow, WM_ERROR,
                    //            ERROR_DRIVER_API_FAIL, nZERO);
                }
            }

            // Update index and wrap up to modula of message list size
            nIndex++;
            nIndex %= nCount;
            // Wait for the event
            WaitForSingleObject(hEventReplayWait, INFINITE);
        }

        if (mmResult == TIMERR_NOERROR)
        {
            timeEndPeriod(time.wPeriodMin);
        }

        CloseHandle(hEventReplayWait);

        if( pWnd != NULL )
        {
            // Update the window
            pWnd->m_omMessageList.EnableWindow( );
            pWnd->m_eReplayState = REPLAY_TO_START;
        }

        pReplayDetails->m_omThreadEvent.SetEvent();
    }

    return 0;
}

void CReplayProcess::vFormatCANDataMsg(STCANDATA* pMsgCAN,
                                       tagSFRAMEINFO_BASIC_CAN* CurrDataCAN)

{
    if (RX_FLAG == pMsgCAN->m_ucDataType)
    {
        CurrDataCAN->m_eDrctn = DIR_RX;
    }
    else
    {
        CurrDataCAN->m_eDrctn = DIR_TX;
    }

    CurrDataCAN->m_eChannel = pMsgCAN->m_uDataInfo.m_sCANMsg.m_ucChannel;

    if (pMsgCAN->m_uDataInfo.m_sCANMsg.m_ucEXTENDED != 0)
    {
        CurrDataCAN->m_byIDType = TYPE_ID_CAN_EXTENDED;
    }
    else
    {
        CurrDataCAN->m_byIDType = TYPE_ID_CAN_STANDARD;
    }

    if (pMsgCAN->m_uDataInfo.m_sCANMsg.m_ucRTR != 0)
    {
        CurrDataCAN->m_byMsgType = TYPE_MSG_CAN_RTR;
    }
    else
    {
        CurrDataCAN->m_byMsgType = TYPE_MSG_CAN_NON_RTR;
    }

    CurrDataCAN->m_dwFrameID = pMsgCAN->m_uDataInfo.m_sCANMsg.m_unMsgID;
}
/*******************************************************************************
  Function Name  : bOpenReplayFile
  Input(s)       : BOOL - TRUE if parsing is successful, FALSE otherwise
  Output         : -
  Functionality  : Load the replay file. Updates entries and CAN message list
                   with the parsed content
  Member of      : CReplayProcess
  Author(s)      : Raja N
  Date Created   : 16.7.2005
  Modifications  : Raja N on 26.07.2005, Implementwed code review comments
*******************************************************************************/
BOOL CReplayProcess::bOpenReplayFile()
{
    BOOL bReturn = TRUE;
    BOOL bModeMismatch              = FALSE;
    CString     omStrLine           = STR_EMPTY;
    CString     omStrTemp           = STR_EMPTY;
    CHAR        Line[500]           = { NULL };
    CString     omStrMsgType        =_T(" ");
    ifstream    omInReplayFile;
    int nBlockCounter = 0;
    // Clear string array
    m_omEntries.RemoveAll();
    m_omMsgList.RemoveAll();
    // Clear error message
    m_omStrError = STR_EMPTY;
    TRY
    {
        omInReplayFile.open( m_ouReplayFile.m_omStrFileName,
        ios::in  );

        if (!omInReplayFile.good())
        {
            // Info file open error notification
            m_omStrError  = defSTR_FILE_OPEN_ERROR;
            bReturn = FALSE ;
        }

        if(bReturn != FALSE)
        {
            // Read the file line by line.
            BOOL bModeFound = FALSE;
            BOOL bFileEndFlag = FALSE;
            BOOL bMsgModeFound = FALSE;
            BOOL bOldVersionFile = FALSE;
            BOOL bVersionFound = FALSE;

            while ( bOldVersionFile == FALSE &&
            (!omInReplayFile.eof()) &&
            ( bMsgModeFound == FALSE  ||
            bModeFound == FALSE ||
            bVersionFound == FALSE ))
            {
                omInReplayFile.getline( Line, sizeof(Line));
                omStrLine = Line;
                omStrLine.TrimLeft();
                omStrLine.TrimRight();

                // Version check
                if( omStrLine.Find(defSTR_BUSMASTER_VERSION_STRING) != -1 )
                {
                    bVersionFound = TRUE;
                    int nPos = omStrLine.Find( defSTR_BUSMASTER_VERSION_STRING );
                    nPos += (int)strlen( defSTR_BUSMASTER_VERSION_STRING );
                    int nMajorVer = omStrLine[ nPos ] - '0';

                    if( nMajorVer < defLOG_FILE_MAJOR_VERSION_SUPPORTED )
                    {
                        bOldVersionFile = TRUE;
                    }
                }

                // set the mode of reply
                if( omStrLine.Find(HEX_MODE) == 0)
                {
                    m_bReplayHexON = TRUE;
                    bMsgModeFound = TRUE;
                }
                else if (omStrLine.Find(DEC_MODE) == 0)
                {
                    m_bReplayHexON = FALSE;
                    bMsgModeFound = TRUE;
                }

                if( omStrLine.Find(SYSTEM_MODE) == 0)
                {
                    m_wLogReplayTimeMode= eSYSTEM_MODE;
                    bModeFound = TRUE;
                }
                else if( omStrLine.Find(ABSOLUTE_MODE) == 0)
                {
                    m_wLogReplayTimeMode= eABSOLUTE_MODE;
                    bModeFound = TRUE;
                }
                else if( omStrLine.Find(RELATIVE_MODE) == 0)
                {
                    m_wLogReplayTimeMode= eRELATIVE_MODE;
                    bModeFound = TRUE;
                }
            }

            if( bOldVersionFile == TRUE )
            {
                m_omStrError = defSTR_LOG_FILE_UNSUPPORTED;
                bReturn = FALSE ;
                bFileEndFlag = TRUE;
            }

            if( bReturn == TRUE &&
                    ( bModeFound == FALSE || bMsgModeFound == FALSE ||
                      bVersionFound == FALSE ) )
            {
                m_omStrError = defINVALID_HEADER;
                bReturn = FALSE ;
                bFileEndFlag = TRUE;
            }

            while (! omInReplayFile.eof() && bFileEndFlag == FALSE )
            {
                omInReplayFile.getline( Line, sizeof(Line));
                omStrLine = Line;
                omStrLine.TrimLeft();
                omStrLine.TrimRight();

                // Exclude empty line, line with starting string
                // hash defined as DefSPL_LINE
                if( omStrLine.IsEmpty()==0 &&
                        omStrLine.Find(DefSPL_LINE) == -1 &&
                        omStrLine.Find(omStrMsgType) != -1)
                {
                    // Apply Filtering
                    STCANDATA sCanMsg;

                    if( bGetMsgInfoFromMsgStr( omStrLine,
                                               &sCanMsg,
                                               m_bReplayHexON ) == TRUE )
                    {
                        SFRAMEINFO_BASIC_CAN sBasicCanInfo;
                        vFormatCANDataMsg(&sCanMsg, &sBasicCanInfo);
                        EnterCriticalSection(&m_omCritSecFilter);
                        BOOL bTobeBlocked = m_ouReplayFile.m_sFilterApplied.bToBeBlocked(sBasicCanInfo);
                        LeaveCriticalSection(&m_omCritSecFilter);

                        // Add it to the list based on filtering result
                        if( bTobeBlocked == FALSE )
                        {
                            m_omEntries.Add( omStrLine );
                            m_omMsgList.Add(sCanMsg );
                        }
                    }
                }
                else if(! omStrLine.Compare(START_SESSION))
                {
                    bModeMismatch = bIsModeMismatch( omInReplayFile,
                                                     m_bReplayHexON, m_wLogReplayTimeMode );

                    if(bModeMismatch == TRUE)
                    {
                        nBlockCounter++;
                        BOOL bEndBlock = FALSE;
                        CString omStrEndBlock = END_SESSION;

                        while (! omInReplayFile.eof() && bEndBlock == FALSE)
                        {
                            omInReplayFile.getline( Line, sizeof(Line));

                            if( omStrEndBlock.Compare(Line) == 0)
                            {
                                bEndBlock = TRUE;
                            }
                        }
                    }
                }
            }// while
        }
    }
    CATCH_ALL(pomException)
    {
        if(pomException != NULL )
        {
            CHAR scErrorMsg[255];
            // Get the exception error message
            pomException->GetErrorMessage(scErrorMsg,sizeof(scErrorMsg));
            m_omStrError = scErrorMsg;
            pomException->Delete();
        }

        bReturn = FALSE;

        if( omInReplayFile.is_open() !=0)
        {
            omInReplayFile.close();
        }
    }
    END_CATCH_ALL

    if(nBlockCounter >=1 )
    {
        m_omStrError.Format( defSTR_MIXED_MODE_WARNING, nBlockCounter );
    }

    // close the file if it open
    if( omInReplayFile.is_open() !=0 )
    {
        omInReplayFile.close();
    }

    return bReturn;
}

/*******************************************************************************
  Function Name  : sunNIReplayThreadFunc
  Input(s)       : pParam - Parameter to the thread
  Output         : -
  Functionality  : This is the Non Interactive thread function
  Member of      : CReplayProcess
  Author(s)      : Raja N
  Date Created   : 16.7.2005
  Modifications  :
*******************************************************************************/
UINT CReplayProcess::sunNIReplayThreadFunc( LPVOID pParam )
{
    CReplayProcess* pReplayDetails = (CReplayProcess*)pParam;

    if( pReplayDetails != NULL )
    {
        // Reset the event
        pReplayDetails->m_omThreadEvent.ResetEvent();
        // Replay code here
        // Time Calculation
        CArray<UINT,UINT> omTimeDelay;
        // Get the item count
        int nCount = (int)pReplayDetails->m_omEntries.GetSize();
        CString omStrCurr;
        CString omStrNext;
        // Init Message delay
        UINT unMsgDelay = pReplayDetails->m_ouReplayFile.m_unMsgTimeDelay;

        if( unMsgDelay == 0 )
        {
            unMsgDelay = 1;
        }

        int nIndex; //nIndex declared outside

        for( nIndex = 0; nIndex < nCount - 1; nIndex++ )
        {
            if( pReplayDetails->m_ouReplayFile.m_nTimeMode ==
                    defREPLAY_RETAIN_DELAY )
            {
                // Get Next Entry
                omStrNext = pReplayDetails->m_omEntries[ nIndex + 1 ];
                // Get the current entry
                omStrCurr = pReplayDetails->m_omEntries[ nIndex ];
                UINT unTime = unTimeDiffBetweenMsg( omStrNext, omStrCurr,
                                                    pReplayDetails->m_wLogReplayTimeMode );

                if( unTime == 0 )
                {
                    unTime = 1;
                }

                omTimeDelay.Add( unTime );
            }
            else
            {
                omTimeDelay.Add( unMsgDelay );
            }
        }

        // Add the cyclic delay at the end
        unMsgDelay = pReplayDetails->m_ouReplayFile.m_unCycleTimeDelay;

        if( unMsgDelay == 0 )
        {
            unMsgDelay = 1;
        }

        omTimeDelay.Add( unMsgDelay );
        TIMECAPS time;
        MMRESULT mmResult = TIMERR_NOERROR;

        if (timeGetDevCaps(&time, sizeof(TIMECAPS)) == TIMERR_NOERROR)
        {
            mmResult = timeBeginPeriod(time.wPeriodMin);
        }

        // Create the event object to wait for
        HANDLE hEventReplayWait = CreateEvent(NULL, FALSE, FALSE, NULL);
        // main loop for message transmission.
        nIndex = 0;

        while( pReplayDetails->m_bStopReplayThread == FALSE )
        {
            // Set the event to wait
            timeSetEvent( omTimeDelay[ nIndex ], time.wPeriodMin,
                          (LPTIMECALLBACK) hEventReplayWait, NULL,
                          TIME_CALLBACK_EVENT_SET | TIME_ONESHOT);

            // Send message in CAN bus if the message ID is valid
            if ( pReplayDetails->m_omMsgList[ nIndex ].
                    m_uDataInfo.m_sCANMsg.m_unMsgID != -1 )
            {
                // Use HIL Function to send CAN message
                HRESULT hRet = s_pouDIL_CAN_Interface->DILC_SendMsg(s_dwClientID, pReplayDetails->m_omMsgList[ nIndex ].m_uDataInfo.m_sCANMsg);

                if (hRet != defERR_OK)
                {
                    //::PostMessage(GUI_hDisplayWindow, WM_ERROR,
                    //            ERROR_DRIVER_API_FAIL, nZERO);
                }
            }

            // Update the index
            nIndex++;

            // if last message is already sent,
            // and it is a cyclic block then go to begining. Otherwise stop the
            // thread
            if( nIndex >= nCount )
            {
                if( pReplayDetails->m_ouReplayFile.m_nReplayMode ==
                        defREPLAY_MODE_CYCLIC )
                {
                    nIndex %= nCount;
                }
                else
                {
                    pReplayDetails->m_bStopReplayThread = TRUE;
                }
            }

            // Wait for the event
            if( pReplayDetails->m_bStopReplayThread == FALSE )
            {
                WaitForSingleObject(hEventReplayWait, INFINITE);
            }
        }

        if (mmResult == TIMERR_NOERROR)
        {
            timeEndPeriod(time.wPeriodMin);
        }

        CloseHandle(hEventReplayWait);
        pReplayDetails->m_omThreadEvent.SetEvent();
    }

    return 0;
}

/*******************************************************************************
  Function Name  : bStartNIReplay
  Input(s)       : -
  Output         : BOOL - TRUE if fuccessful, FALSE otherwise
  Functionality  : To start non interactive threads
  Member of      : CReplayProcess
  Author(s)      : Raja N
  Date Created   : 16.7.2005
  Modifications  :
*******************************************************************************/
BOOL CReplayProcess::bStartNIReplay()
{
    BOOL bResult = FALSE;

    // Start Replay Thread
    if( m_omEntries.GetSize() > 0 )
    {
        // Create the thread
        CWinThread* pThread =
            AfxBeginThread( CReplayProcess::sunNIReplayThreadFunc,
                            this );

        if( pThread != NULL )
        {
            m_hThread = pThread->m_hThread;
            m_bStopReplayThread = FALSE;
            bResult = TRUE;
        }
    }

    return bResult;
}
/*******************************************************************************
  Function Name  : bSetThreadStopSignal
  Input(s)       : -
  Output         : BOOL - TRUE if the thread is already running, FALSE otherwise
  Functionality  : To set the stop signal of the thread
  Member of      : CReplayProcess
  Author(s)      : Raja N
  Date Created   : 16.7.2005
  Modifications  :
*******************************************************************************/
BOOL CReplayProcess::bSetThreadStopSignal()
{
    BOOL bIsValid = FALSE;

    if( m_hThread != NULL )
    {
        m_bStopReplayThread = TRUE;
        bIsValid = TRUE;
    }

    return bIsValid;
}

/*******************************************************************************
  Function Name  : bStopReplayThread
  Input(s)       : BOOL - TRUE if the thread is killed, FALSE for normal thread
                   termination
  Output         : -
  Functionality  : To stop the running replay thread
  Member of      : CReplayProcess
  Author(s)      : Raja N
  Date Created   : 16.7.2005
  Modifications  :
*******************************************************************************/
BOOL CReplayProcess::bStopReplayThread()
{
    BOOL bIsThreadKilled = FALSE;

    // Kill the replay thread if it is not responding
    if( WaitForSingleObject( m_omThreadEvent,
                             defNON_INTERACTIVE_THREAD_WAIT_TIME ) != WAIT_OBJECT_0 )
    {
        TerminateThread( m_hThread, 0 );
        bIsThreadKilled = TRUE;
        m_hThread = NULL;
    }

    return bIsThreadKilled;
}

