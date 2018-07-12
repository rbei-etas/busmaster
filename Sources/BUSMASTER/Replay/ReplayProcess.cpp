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
//#include "DataTypes/MsgBufAll_Datatypes.h"
//#include "DataTypes/DIL_Datatypes.h"
#include "DataTypes/Filter_Datatypes.h"
#include "BaseDIL_CAN.h"
//#include "DIL_Interface_extern.h"
#include "Error.h"         // For Errors
#include "Utility_Replay.h"
#include "Utility\UtilFunctions.h"
#
#define PEG_STEP 1
#define BYTES_PER_LINE 20
#define MAX_FILE_SIZE_INTERACTIVE_REPLAY 52428800 //50MB
CBaseDIL_CAN* CReplayProcess::s_pouDIL_CAN_Interface = nullptr;
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
    m_pReplayWndPtr( nullptr ),
    m_omBreakPoints( defBREAK_POINT_MAP_SIZE ),
    m_hThread( nullptr ),
    m_bReplayHexON( TRUE ),
    m_wLogReplayTimeMode(0 ),
    m_nCurrentIndex( 0 ),
    m_nUserSelectionIndex( 0 ),
    m_nNoOfMessagesToPlay( 0 ),
    m_bStopReplayThread( TRUE )
{

    m_omSelectedIndex.RemoveAll();
    //DIL related code
    DIL_GetInterface( CAN, (void**)&CReplayProcess::s_pouDIL_CAN_Interface );
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
    bool bNxtSessionFlag = false;
    bool bNxtEOFflag = false;
    bool bCurSessionFlag = false;
    bool bCurEOFflag = false;
    bool bCurProtocolMismatch = false;
    bool bNxtProtocolMismatch = false;
    bool bNxtInvalidMsg = false;
    bool bCurInvalidMsg = false;
    STCANDATA sNxtCanMsg;
    STCANDATA sCurCanMsg;

    ZeroMemory(&sCurCanMsg, sizeof(STCANDATA));

    if( pReplayDetails != nullptr )
    {
        CMsgReplayWnd* pWnd = (CMsgReplayWnd*)pReplayDetails->m_pReplayWndPtr;
        // Reset the event
        pReplayDetails->m_omThreadEvent.ResetEvent();
        // Disable List control
        // To avoid flickering avoid disabling for a step action
        if( pWnd != nullptr && pReplayDetails->m_nNoOfMessagesToPlay > 1)
        {
            pWnd->m_omMessageList.EnableWindow( FALSE );
        }
        // Replay code here
        /// Get the number of messages to play
        int nCount = pReplayDetails->m_nNoOfMessagesToPlay;
        int nOffset = pReplayDetails->m_nUserSelectionIndex;

        // Time Calculation
        CArray<UINT,UINT> omTimeDelay;

        TIMECAPS time;
        MMRESULT mmResult = TIMERR_NOERROR;
        if (timeGetDevCaps(&time, sizeof(TIMECAPS)) == TIMERR_NOERROR)
        {
            mmResult = timeBeginPeriod(time.wPeriodMin);
        }

        // Create the event object to wait for
        HANDLE hEventReplayWait = CreateEvent(nullptr, FALSE, FALSE, nullptr);
        // Assign the message delay time

        UINT unDelay;
        UINT unMsgDelay = pReplayDetails->m_ouReplayFile.m_unMsgTimeDelay;
        if( unMsgDelay == 0 )
        {
            unMsgDelay = 1;
        }
        
        UINT unSessionDelay = pReplayDetails->m_ouReplayFile.m_unSessionDelay;
        if( unSessionDelay == 0 )
        {
            unSessionDelay = 1;
        }

        // main loop for message transmission.
        BOOL bBreakPointFlag = FALSE;
        if(pReplayDetails->m_omBreakPoints[nOffset ] == TRUE)
        {
            bBreakPointFlag = TRUE;
        }
        for( int nIndex = 0;
                pReplayDetails->m_bStopReplayThread == FALSE &&( (pReplayDetails->m_omBreakPoints[ nIndex + nOffset ] == FALSE)||(bBreakPointFlag == TRUE));
                nIndex++ )
        {
            bBreakPointFlag = FALSE;
            int nCurrentIndex = nIndex + nOffset;
            if( ( nIndex + 1 ) <= nCount )
            {
                CString omStrCurr;
                CString omStrNext;
                omStrNext = pReplayDetails->omStrGetMsgFromLog(nIndex + nOffset + 1,sNxtCanMsg,bNxtSessionFlag, bNxtEOFflag,bNxtProtocolMismatch,bNxtInvalidMsg);
                omStrCurr = pReplayDetails->omStrGetMsgFromLog(nIndex + nOffset,sCurCanMsg, bCurSessionFlag, bCurEOFflag,bCurProtocolMismatch,bCurInvalidMsg);

                unDelay = unMsgDelay;
                if( pReplayDetails->m_ouReplayFile.m_nTimeMode ==
                        defREPLAY_RETAIN_DELAY )
                {
                    if(bCurEOFflag)
                    {
                        pReplayDetails->m_bStopReplayThread = TRUE;
                    }

                    UINT unTime = unTimeDiffBetweenMsg( omStrNext, omStrCurr,
                                                        pReplayDetails->m_wLogReplayTimeMode );
                    if( unTime == 0 )
                    {
                        unTime = 1;
                    }

                    unDelay = unTime;
                }

                if(bNxtSessionFlag && (pReplayDetails->m_ouReplayFile.m_nSessionMode == defREPLAY_SPECIFIC_SESSION_DELAY))
                {
                    unDelay = unSessionDelay;
                }


                timeSetEvent( unDelay, time.wPeriodMin,
                              (LPTIMECALLBACK) hEventReplayWait, 0,
                              TIME_CALLBACK_EVENT_SET | TIME_ONESHOT);
            }
            // Send message in CAN bus if the message ID is valid

            if ( sCurCanMsg.m_uDataInfo.m_sCANMsg.m_unMsgID != -1 )
            {
                HRESULT hRet = 0;
                SFRAMEINFO_BASIC_CAN sBasicCanInfo;

                pReplayDetails->vFormatCANDataMsg(&sCurCanMsg, &sBasicCanInfo);
                BOOL bTobeBlocked = FALSE;
                if( pReplayDetails->m_ouReplayFile.m_sFilterApplied.m_bEnabled == TRUE)
                {
                    EnterCriticalSection(&pReplayDetails->m_omCritSecFilter);
                    bTobeBlocked = pReplayDetails->m_ouReplayFile.m_sFilterApplied.bToBeBlocked(sBasicCanInfo);
                    LeaveCriticalSection(&pReplayDetails->m_omCritSecFilter);
                }
                if(bTobeBlocked == FALSE)
                {
                    // Use HIL Function to send CAN message

                    HRESULT hRet =  s_pouDIL_CAN_Interface->DILC_SendMsg(s_dwClientID,
                                    sCurCanMsg.
                                    m_uDataInfo.m_sCANMsg );
                }

            }
            // Increment cur sel to set to next sel
            pReplayDetails->m_nCurrentIndex = nCurrentIndex;
            // last message is already sent

            if(bNxtEOFflag || nCount == 1)
            {
                pReplayDetails->m_bStopReplayThread = TRUE;
            }
            else
            {
                // Wait for the event
                WaitForSingleObject(hEventReplayWait, INFINITE);
            }

            if(bNxtProtocolMismatch)
            {
                pReplayDetails->m_bStopReplayThread = TRUE;
                if(!pReplayDetails->bGetbIsProtocolMismatch())
                {
                    CReplayManager::ouGetReplayManager().vSendToTrace(defSTR_LOG_PRTOCOL_MISMATCH);
                    pReplayDetails->bSetbIsProtocolMismatch(true);
                }
            }
            else if(bNxtInvalidMsg && !bNxtEOFflag)
            {
                pReplayDetails->m_bStopReplayThread = TRUE;
                if(!pReplayDetails->bGetbIsInvalidMsg())
                {
                    CReplayManager::ouGetReplayManager().vSendToTrace(defSTR_LOG_INVALID_MESSAGE);
                    pReplayDetails->bSetbIsProtocolMismatch(true);
                }
            }

        }
        if (mmResult == TIMERR_NOERROR)
        {
            timeEndPeriod(time.wPeriodMin);
        }
        CloseHandle(hEventReplayWait);

        if( pWnd != nullptr )
        {
            pWnd->m_omMessageList.EnableWindow( );
            pWnd->m_eReplayState = REPLAY_TO_START;
            // Set the selection
            pReplayDetails->m_nCurrentIndex++;
            pReplayDetails->m_nCurrentIndex %= pReplayDetails->dwGetvecPegSize();
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
    bool bNxtSessionFlag = false;
    bool bNxtEOFflag = false;
    bool bCurSessionFlag = false;
    bool bCurEOFflag = false;
    bool bCurProtocolMismatch = false;
    bool bNxtProtocolMismatch = false;
    bool bNxtInvalidMsg = false;
    bool bCurInvalidMsg = false;
    STCANDATA sNxtCanMsg;
    STCANDATA sCurCanMsg;

    if( pReplayDetails != nullptr )
    {
        CMsgReplayWnd* pWnd = (CMsgReplayWnd*)pReplayDetails->m_pReplayWndPtr;
        // Reset the event
        pReplayDetails->m_omThreadEvent.ResetEvent();
        // Disable List control
        if( pWnd != nullptr )
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
        UINT unDelay;
        UINT unMsgDelay = pReplayDetails->m_ouReplayFile.m_unMsgTimeDelay;
        if( unMsgDelay == 0 )
        {
            unMsgDelay = 1;
        }
        int nIndex; //nIndex declared outside

        UINT unSessionDelay = pReplayDetails->m_ouReplayFile.m_unSessionDelay;
        if( unSessionDelay == 0 )
        {
            unSessionDelay = 1;
        }

        // Add the cyclic delay at the end
        UINT unCycleDelay = pReplayDetails->m_ouReplayFile.m_unCycleTimeDelay;
        if( unCycleDelay == 0 )
        {
            unCycleDelay = 1;
        }

        TIMECAPS time;
        MMRESULT mmResult = TIMERR_NOERROR;
        if (timeGetDevCaps(&time, sizeof(TIMECAPS)) == TIMERR_NOERROR)
        {
            mmResult = timeBeginPeriod(time.wPeriodMin);
        }

        // Create the event object to wait for
        HANDLE hEventReplayWait = CreateEvent(nullptr, FALSE, FALSE, nullptr);
        // main loop for message transmission.
        nIndex = 0;
        while( pReplayDetails->m_bStopReplayThread == FALSE )
        {
            int nCurrentIndex = pReplayDetails->m_omSelectedIndex[ nIndex ];
            int nNextIndex =  pReplayDetails->m_omSelectedIndex[ (nIndex + 1)%nCount ];
            omStrNext = pReplayDetails->omStrGetMsgFromLog(nNextIndex,sNxtCanMsg,bNxtSessionFlag,bNxtEOFflag,bNxtProtocolMismatch,bNxtInvalidMsg);
            omStrCurr = pReplayDetails->omStrGetMsgFromLog(nCurrentIndex,sCurCanMsg,bCurSessionFlag,bCurEOFflag,bCurProtocolMismatch,bCurInvalidMsg);
            unDelay = unMsgDelay;
            if( pReplayDetails->m_ouReplayFile.m_nTimeMode ==
                    defREPLAY_RETAIN_DELAY && (nIndex < nCount-1) )
            {

                UINT unTime = unTimeDiffBetweenMsg( omStrNext, omStrCurr,
                                                    pReplayDetails->m_wLogReplayTimeMode );
                if( unTime == 0 )
                {
                    unTime = 1;
                }
                unMsgDelay = unTime ;
            }


            if(bNxtSessionFlag && (pReplayDetails->m_ouReplayFile.m_nSessionMode == defREPLAY_SPECIFIC_SESSION_DELAY))
            {
                unDelay = unSessionDelay;
            }
            if(nIndex == nCount - 1)
            {
                unDelay = unCycleDelay;
            }

            timeSetEvent( unDelay, time.wPeriodMin,
                          (LPTIMECALLBACK) hEventReplayWait, NULL,
                          TIME_CALLBACK_EVENT_SET | TIME_ONESHOT);
            // Send message in CAN bus if the message ID is valid

            if ( sCurCanMsg.m_uDataInfo.m_sCANMsg.m_unMsgID != -1 )
            {
                HRESULT hRet = 0;

                SFRAMEINFO_BASIC_CAN sBasicCanInfo;

                pReplayDetails->vFormatCANDataMsg(&sCurCanMsg, &sBasicCanInfo);
                BOOL bTobeBlocked = FALSE;

                EnterCriticalSection(&pReplayDetails->m_omCritSecFilter);
                if( pReplayDetails->m_ouReplayFile.m_sFilterApplied.m_bEnabled == TRUE)
                {
                    bTobeBlocked = pReplayDetails->m_ouReplayFile.m_sFilterApplied.bToBeBlocked(sBasicCanInfo);
                }
                LeaveCriticalSection(&pReplayDetails->m_omCritSecFilter);

                if(bTobeBlocked == FALSE)
                {
                    // Use HIL Function to send CAN message

                    HRESULT hRet = s_pouDIL_CAN_Interface->DILC_SendMsg(s_dwClientID,sCurCanMsg.m_uDataInfo.m_sCANMsg );
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

        if( pWnd != nullptr )
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

    if (pMsgCAN->m_uDataInfo.m_sCANMsg.m_ucRTR == 0)
    {
        CurrDataCAN->m_byMsgType =  TYPE_MSG_CAN_NON_RTR;
    }
    else
    {
        CurrDataCAN->m_byMsgType = TYPE_MSG_CAN_RTR;
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
BOOL CReplayProcess::bOpenReplayFile(BOOL bIsInteractive)
{
    BOOL bReturn = TRUE;
    BOOL bModeMismatch              = FALSE;
    CString     omStrLine           = "";
    CString     omStrTemp           = "";
    CHAR        Line[500]           = { 0 };
    CString     omStrMsgType        = " ";

    int nBlockCounter = 0;
    m_bIsInteractive = FALSE;
    // Clear string array
    m_omEntries.RemoveAll();
    m_omMsgList.RemoveAll();
    // Clear error message
    m_omStrError = "";
    m_bIsEmptySession = false;
    TRY
    {
        omInReplayFile.open( m_ouReplayFile.m_omStrFileName,
        std::ios::in  );
        if (!omInReplayFile.good())
        {
            // Info file open error notification
            m_omStrError  = defSTR_FILE_OPEN_ERROR;
            bReturn = FALSE ;
        }

        if((dwGetNoOfMsgsInLog()*BYTES_PER_LINE > MAX_FILE_SIZE_INTERACTIVE_REPLAY) && bIsInteractive && bReturn != FALSE)
        {
            m_omStrError = defSTR_REPLAY_FILE_SIZE_EXCEEDED;
            bReturn = FALSE ;
        }
        omInReplayFile.seekg(0, std::ios::beg);
        if(bReturn != FALSE)
        {
            // Read the file line by line.
            BOOL bModeFound = FALSE;
            BOOL bFileEndFlag = FALSE;
            BOOL bMsgModeFound = FALSE;
            BOOL bOldVersionFile = FALSE;
            BOOL bVersionFound = FALSE;
            BOOL bProtocolFound = FALSE;
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
                // Check if Protocol exists in the log file
                if( omStrLine.Find(defSTR_PROTOCOL_USED) != -1)
                {
                    // If can related log file
                    if( omStrLine.Find(defSTR_PROTOCOL_CAN) == -1)
                    {
                        m_omStrError = defSTR_LOG_PRTOCOL_MISMATCH;
                        bReturn = FALSE ;
                        bFileEndFlag = TRUE;
                    }
                }
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

                    bMsgModeFound = TRUE;

                }
                else if (omStrLine.Find(DEC_MODE) == 0)
                {

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



        }
    }
    CATCH_ALL(pomException)
    {
        if(pomException != nullptr )
        {
            CHAR scErrorMsg[255];
            // Get the exception error message
            pomException->GetErrorMessage(scErrorMsg,sizeof(scErrorMsg));
            m_omStrError = scErrorMsg;
            pomException->Delete();
        }
        bReturn = FALSE;

    }
    END_CATCH_ALL
    if(nBlockCounter >=1 )
    {
        m_omStrError.Format( defSTR_MIXED_MODE_WARNING, nBlockCounter );
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
        pReplayDetails->bSetbIsProtocolMismatch(false);
        pReplayDetails->bSetbIsInvalidMsg(false);
        // Reset the event
        pReplayDetails->m_omThreadEvent.ResetEvent();
        // Replay code here
        // Time Calculation
        CArray<UINT,UINT> omTimeDelay;
        // Get the item count
        bool bSessionFlag = false;
        bool bDetectSession = false;
        bool bDetectEOF = false;
        bool bEOFFlag = false;
        CString omStrCurr = "";
        CString omStrNext= "";
        // Init Message delay
        UINT unMsgDelay = pReplayDetails->m_ouReplayFile.m_unMsgTimeDelay;
        if( unMsgDelay == 0 )
        {
            unMsgDelay = 1;
        }
        //int nIndex; //nIndex declared outside


        // Add the cyclic delay at the end
        UINT unCycleDelay = pReplayDetails->m_ouReplayFile.m_unCycleTimeDelay;
        if( unCycleDelay == 0 )
        {
            unCycleDelay = 1;
        }

        UINT unSessionDelay=pReplayDetails->m_ouReplayFile.m_unSessionDelay;
        if( unSessionDelay == 0 )
        {
            unSessionDelay = 1;
        }
        TIMECAPS time;
        MMRESULT mmResult = TIMERR_NOERROR;
        if (timeGetDevCaps(&time, sizeof(TIMECAPS)) == TIMERR_NOERROR)
        {
            mmResult = timeBeginPeriod(time.wPeriodMin);
        }

        // Create the event object to wait for
        HANDLE hEventReplayWait = CreateEvent(NULL, FALSE, FALSE, NULL);
        // main loop for message transmission.
        UINT unTime;
        pReplayDetails->omInReplayFile.clear(); // clear eof and fail bits
        pReplayDetails->omInReplayFile.seekg(0, std::ios::beg);
        std::string strLine;

        while( pReplayDetails->m_bStopReplayThread == FALSE )
        {
            bool bIsProtocolMismatch = false;
            unMsgDelay = pReplayDetails->m_ouReplayFile.m_unMsgTimeDelay;
            if( unMsgDelay == 0 )
            {
                unMsgDelay = 1;
            }
            bSessionFlag = false;
            bEOFFlag = false;
            if(bDetectSession)
            {
                bSessionFlag = true;
                bDetectSession = false;
            }
            if(bDetectEOF)
            {
                bEOFFlag = true;
                bDetectEOF = false;
            }
            bool bIsComment = false;
            while(getline(pReplayDetails->omInReplayFile , strLine))
            {
                CUtilFunctions::Trim(strLine,' ');
                if(strLine.find(END_SESSION) != std::string::npos)
                {
                    bDetectSession = true ;
                }
                if(strLine.find(START_COMMENT) != std::string::npos)
                {
                    bIsComment = true;
                }
                if(strLine.find(END_COMMENT) != std::string::npos)
                {
                    bIsComment = false;
                }

                if(strLine.empty() || bIsComment)
                {
                    continue;
                }

                if(strLine.find("*") == std::string::npos)
                {
                    omStrCurr = omStrNext;
                    omStrNext = strLine.c_str();
                    if(omStrCurr.Compare("") == 0)
                    {
                        continue;
                    }
                    else
                    {
                        break;
                    }
                }
                if(strLine.find("*") != std::string::npos)
                {
                    if( strLine.find(defSTR_PROTOCOL_USED) != std::string::npos)
                    {
                        // If can related log file
                        if( strLine.find(defSTR_PROTOCOL_CAN) == std::string::npos)
                        {
                            pReplayDetails->m_omStrError = defSTR_LOG_PRTOCOL_MISMATCH;
                            bIsProtocolMismatch = true;
                            if(pReplayDetails->m_ouReplayFile.m_nReplayMode ==  defREPLAY_MODE_CYCLIC)
                            {
                                pReplayDetails->omInReplayFile.clear(); // clear eof and fail bits
                                pReplayDetails->omInReplayFile.seekg(0, std::ios::beg);
                                bDetectEOF = true;
                            }
                            else
                            {
                                pReplayDetails->m_bStopReplayThread = TRUE;
                            }
                            omStrCurr = omStrNext;
                            omStrNext = "";
                            break;
                        }
                    }


                }
            }
            if(omStrCurr.IsEmpty() && omStrNext.IsEmpty())
            {
                CReplayManager::ouGetReplayManager().vSendToTrace(defSTR_REPLAY_FILE_EMPTY);
                pReplayDetails->m_bStopReplayThread = TRUE;
                continue;
            }
            if(pReplayDetails->omInReplayFile.eof())
            {
                omStrCurr = omStrNext;
                omStrNext = "";
                bDetectEOF = true;
            }
            STCANDATA sCanMsg,sNxtCanMsg;
            bool bIsValidMessage = true;
            pReplayDetails->sGetCanMsg(omStrCurr,sCanMsg);
            if(omStrNext.IsEmpty() == FALSE)
            {
                bIsValidMessage = pReplayDetails->sGetCanMsg(omStrNext,sNxtCanMsg);
            }
            if(!bIsValidMessage)
            {
                pReplayDetails->m_omStrError = defSTR_LOG_INVALID_MESSAGE;
                if(!pReplayDetails->bGetbIsInvalidMsg())
                {
                    CReplayManager::ouGetReplayManager().vSendToTrace(defSTR_LOG_INVALID_MESSAGE);
                    pReplayDetails->bSetbIsInvalidMsg(true);
                }
                if(pReplayDetails->m_ouReplayFile.m_nReplayMode ==  defREPLAY_MODE_CYCLIC)
                {
                    pReplayDetails->omInReplayFile.clear(); // clear eof and fail bits
                    pReplayDetails->omInReplayFile.seekg(0, std::ios::beg);
                    bDetectEOF = true;
                    omStrNext = "";
                }
                else
                {
                    pReplayDetails->m_bStopReplayThread = TRUE;
                }


            }


            if( (pReplayDetails->m_ouReplayFile.m_nTimeMode ==
                    defREPLAY_RETAIN_DELAY) && bIsValidMessage && !bIsProtocolMismatch)
            {
                unTime = unTimeDiffBetweenMsg( omStrNext, omStrCurr,
                                               pReplayDetails->m_wLogReplayTimeMode );
                if( unTime == 0 )
                {
                    unTime = 1;
                }
                unMsgDelay = unTime;
            }

            if((pReplayDetails->m_ouReplayFile.m_nSessionMode == defREPLAY_SPECIFIC_SESSION_DELAY) && bDetectSession == true)
            {
                unMsgDelay = unSessionDelay;
            }

            if(bDetectEOF && pReplayDetails->m_ouReplayFile.m_nReplayMode ==    defREPLAY_MODE_CYCLIC)
            {
                unMsgDelay = unCycleDelay;
                pReplayDetails->omInReplayFile.clear(); // clear eof and fail bits
                pReplayDetails->omInReplayFile.seekg(0, std::ios::beg);
            }

            // Send message in CAN bus if the message ID is valid
            timeSetEvent( unMsgDelay, time.wPeriodMin,
                          (LPTIMECALLBACK) hEventReplayWait, NULL,
                          TIME_CALLBACK_EVENT_SET | TIME_ONESHOT);

            if (sCanMsg.m_uDataInfo.m_sCANMsg.m_unMsgID != -1 )
            {
                HRESULT hRet = 0;

                SFRAMEINFO_BASIC_CAN sBasicCanInfo;

                pReplayDetails->vFormatCANDataMsg(&sCanMsg, &sBasicCanInfo);
                BOOL bTobeBlocked = FALSE;
                if( pReplayDetails->m_ouReplayFile.m_sFilterApplied.m_bEnabled == TRUE)
                {
                    EnterCriticalSection(&pReplayDetails->m_omCritSecFilter);
                    bTobeBlocked = pReplayDetails->m_ouReplayFile.m_sFilterApplied.bToBeBlocked(sBasicCanInfo);
                    LeaveCriticalSection(&pReplayDetails->m_omCritSecFilter);
                }


				bTobeBlocked = bTobeBlocked | pReplayDetails->bMessageTobeBlocked(sBasicCanInfo);



                if(bTobeBlocked == FALSE )
                {
                    // Use HIL Function to send CAN message

                    hRet = s_pouDIL_CAN_Interface->DILC_SendMsg(s_dwClientID, sCanMsg.m_uDataInfo.m_sCANMsg);
                }

                if (hRet != 0)
                {
                    //::PostMessage(GUI_hDisplayWindow, WM_ERROR,
                    //            ERROR_DRIVER_API_FAIL, nZERO);
                }
            }
            // Update the index
            // if last message is already sent,
            // and it is a cyclic block then go to begining. Otherwise stop the
            // thread
            if( pReplayDetails->omInReplayFile.eof() )
            {
                if( pReplayDetails->m_ouReplayFile.m_nReplayMode ==
                        defREPLAY_MODE_MONOSHOT )
                {
                    pReplayDetails->m_bStopReplayThread = TRUE;
                }

            }
            // Wait for the event
            if( pReplayDetails->m_bStopReplayThread == FALSE)
            {
                WaitForSingleObject(hEventReplayWait, INFINITE);
            }

            if(bIsProtocolMismatch && bIsValidMessage)
            {
                if(!pReplayDetails->bGetbIsProtocolMismatch())
                {
                    CReplayManager::ouGetReplayManager().vSendToTrace(defSTR_LOG_PRTOCOL_MISMATCH);
                    pReplayDetails->bSetbIsProtocolMismatch(true);
                }

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

    // Create the thread
    CWinThread* pThread =
        AfxBeginThread( CReplayProcess::sunNIReplayThreadFunc,
                        this );
    if( pThread != nullptr )
    {
        m_hThread = pThread->m_hThread;
        m_bStopReplayThread = FALSE;
        bResult = TRUE;
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
    if( m_hThread != nullptr )
    {
        m_bStopReplayThread = TRUE;
        bIsValid = TRUE;
    }
    vCloseReplayFile();
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
        m_hThread = nullptr;
    }
    return bIsThreadKilled;
}

BOOL CReplayProcess::bMessageTobeBlocked( SFRAMEINFO_BASIC_CAN& sBasicCanInfo )
{
    BOOL bReturn = TRUE;
    if(m_ouReplayFile.m_ouReplayMsgType == DIR_ALL)
    {
        bReturn = FALSE;
    }
    else if(m_ouReplayFile.m_ouReplayMsgType == DIR_RX && sBasicCanInfo.m_eDrctn == DIR_RX)
    {
        bReturn = FALSE;
    }
    else if(m_ouReplayFile.m_ouReplayMsgType == DIR_TX && sBasicCanInfo.m_eDrctn == DIR_TX)
    {
        bReturn = FALSE;
    }
    return bReturn;
}
DWORD CReplayProcess::dwGetNoOfMsgsInLog()
{
    omInReplayFile.seekg(0, std::ios::end);
    DWORD dwEnd = omInReplayFile.tellg();
    omInReplayFile.clear();
    return dwEnd/BYTES_PER_LINE;
}
void CReplayProcess::vCloseReplayFile()
{
    omInReplayFile.clear();
    omInReplayFile.close();
}
/*******************************************************************************
  Function Name  : omStrGetMsgFromLog
  Input(s)       : -
  Output         : -
  Functionality  : To get a particular message from the log file.
  Member of      : CReplayProcess
  Author(s)      : Robin George Koshy
  Date Created   : 09.06.2014
  Modifications  :
*******************************************************************************/
CString CReplayProcess::omStrGetMsgFromLog(DWORD dwLineNo,STCANDATA& sCanMsg,bool& bSessionFlag, bool& bEOFflag, bool& bProtocolMismatch, bool& bInvalidMsg)
{
    EnterCriticalSection(&m_omCritSecFilter);
    bSessionFlag = false;
    bEOFflag = false;
    bProtocolMismatch = false;
    bInvalidMsg = false;
    DWORD dwPegOffset = dwLineNo%PEG_STEP;
    DWORD dwPegCount = dwLineNo/PEG_STEP;
    DWORD dwLinesNotPegged = dwLineNo - vecPeg.size()*PEG_STEP;
    DWORD dwDepth;
    std::string strLine = "";
    bool bIsComment = false;

    if((dwPegCount < vecPeg.size()) || dwLinesNotPegged < 0)
    {
        omInReplayFile.seekg(vecPeg[dwPegCount],std::ios::beg);
        bSessionFlag = vecSessionFlag[dwPegCount];
        for(int i=0; i<=dwPegOffset; i++)
        {
            do
            {
                getline(omInReplayFile,strLine);
                CUtilFunctions::Trim(strLine,' ');
                if(strLine.find(START_COMMENT) != std::string::npos)
                {
                    bIsComment = true;
                }
                if(strLine.find(END_COMMENT) != std::string::npos)
                {
                    bIsComment = false;
                }
                if(bIsComment)
                {
                    continue;
                }
                if(strLine.find("*")!=std::string::npos && dwLineNo!=0)
                {
                    if( strLine.find(defSTR_PROTOCOL_USED) != std::string::npos)
                    {
                        // If can related log file
                        if( strLine.find(defSTR_PROTOCOL_CAN) == std::string::npos)
                        {
                            bProtocolMismatch = true;
                            strLine = "";
                        }
                    }
                    bSessionFlag = true;
                }
                if(omInReplayFile.eof())
                {
                    bEOFflag = true;
                }
            }
            while(((strLine.find("*")!=std::string::npos || strLine.empty())&& !omInReplayFile.eof() && !bProtocolMismatch)||bIsComment);
            //Check valid Message
            if(strLine.find("0x")!=-1)
            {
                m_bReplayHexON = TRUE;
            }
            else
            {
                m_bReplayHexON = FALSE;
            }
            if(!bGetMsgInfoFromMsgStr( strLine.c_str(),
                                       &sCanMsg,
                                       m_bReplayHexON ))
            {
                bInvalidMsg = true;
            }
        }
    }
    else
    {
        if(dwLineNo==0)
        {
            omInReplayFile.seekg(0,std::ios::beg);

        }
        else
        {
            omInReplayFile.seekg(vecPeg.back(),std::ios::beg);

        }
        if(dwLinesNotPegged==0 && dwLineNo != 0)
        {
            dwLinesNotPegged = PEG_STEP;
        }
        for(int i=0; i<=dwLinesNotPegged; i++)
        {
            do
            {
                dwDepth = omInReplayFile.tellg();
                getline(omInReplayFile,strLine);
                CUtilFunctions::Trim(strLine,' ');
                if(strLine.find(START_COMMENT) != std::string::npos)
                {
                    bIsComment = true;
                }
                if(strLine.find(END_COMMENT) != std::string::npos)
                {
                    bIsComment = false;
                }
                if(bIsComment)
                {
                    continue;
                }
                if(strLine.find("*")!=std::string::npos && dwLineNo!=0)
                {
                    if( strLine.find(defSTR_PROTOCOL_USED) != std::string::npos)
                    {
                        // If can related log file
                        if( strLine.find(defSTR_PROTOCOL_CAN) == std::string::npos)
                        {
                            bProtocolMismatch = true;
                            strLine = "";
                        }
                    }
                    bSessionFlag = true;
                }
                if(omInReplayFile.eof())
                {
                    bEOFflag = true;
                }
            }
            while(((strLine.find("*")!=std::string::npos || strLine.empty())&& !omInReplayFile.eof() && !bProtocolMismatch)||bIsComment);
            //Check valid Message
            if(strLine.find("0x")!=-1)
            {
                m_bReplayHexON = TRUE;
            }
            else
            {
                m_bReplayHexON = FALSE;
            }
            if(!bGetMsgInfoFromMsgStr( strLine.c_str(),
                                       &sCanMsg,
                                       m_bReplayHexON ))
            {
                strLine = "";
                bInvalidMsg = true;
            }
            if(((i%PEG_STEP == 0 && i!=0) || dwLineNo == 0) && !strLine.empty())
            {
                vecPeg.push_back(dwDepth);
                vecSessionFlag.push_back(bSessionFlag);
            }

        }
    }

    if(omInReplayFile.eof())
    {
        omInReplayFile.clear();
    }

    LeaveCriticalSection(&m_omCritSecFilter);

    return strLine.c_str();


}
DWORD CReplayProcess::dwGetvecPegSize()
{
    return vecPeg.size();
}
bool CReplayProcess::bGetbIsProtocolMismatch()
{
    return m_bIsProtocolMismatch;
}
void CReplayProcess::bSetbIsProtocolMismatch(bool bIsProtocolMismatch)
{
    m_bIsProtocolMismatch = bIsProtocolMismatch;
}
bool CReplayProcess::bGetbIsInvalidMsg()
{
    return m_bIsInvalidMessage;
}
void CReplayProcess::bSetbIsInvalidMsg(bool bIsInvalidMessage)
{
    m_bIsInvalidMessage = bIsInvalidMessage;
}
/*******************************************************************************
  Function Name  : sGetCanMsg
  Input(s)       : -
  Output         : -
  Functionality  : To extract CAN data from a message
  Member of      : CReplayProcess
  Author(s)      : Robin George Koshy
  Date Created   : 09.06.2014
  Modifications  :
*******************************************************************************/
bool CReplayProcess::sGetCanMsg(CString omStrMessage,STCANDATA& sCanMsg)
{
    //Check whether hex or dec and set the flag appropriately.
    if(omStrMessage.Find("0x")!=-1)
    {
        m_bReplayHexON = TRUE;
    }
    else
    {
        m_bReplayHexON = FALSE;
    }
    BOOL bIsValidMessage = bGetMsgInfoFromMsgStr( omStrMessage,
                           &sCanMsg,
                           m_bReplayHexON );
    if(bIsValidMessage == TRUE)
    {
        return true;
    }
    else
    {
        return false;
    }
}
