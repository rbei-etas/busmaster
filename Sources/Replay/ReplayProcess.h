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
 * \file      ReplayProcess.h
 * \brief     Interface file for CReplayProcess class
 * \author    Raja N
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Interface file for CReplayProcess class
 */

#pragma once

class CBaseDIL_CAN;
class CReplayProcess
{
public:
    // Standard constructor and destructor
    CReplayProcess(const CReplayFile& ouReplayFile);
    ~CReplayProcess();
    // Thread for monoshot replay file tx
    static UINT sunReplayMonoshotThreadFunc( LPVOID pParam );
    // Thread for cyclic replay file tx
    static UINT sunReplayCyclicThreadFunc( LPVOID pParam );
    // Thread for non-interactive replay process
    static UINT sunNIReplayThreadFunc( LPVOID pParam );
    // DIL CAN interface
    static CBaseDIL_CAN* s_pouDIL_CAN_Interface;
    static DWORD s_dwClientID;
    // To load repaly file
    BOOL bOpenReplayFile();
    // To start NI replay threads
    BOOL bStartNIReplay();
    // To give stop signal to running thread
    BOOL bSetThreadStopSignal();
    // To kill running replay thread
    BOOL bStopReplayThread();
    // Friends of Replay Process Class
    friend class CReplayManager;
    friend class CMsgReplayWnd;
private:
    // Functions
    //Critical section for filtering
    CRITICAL_SECTION m_omCritSecFilter;
    // Reference to CReplayFile
    CReplayFile m_ouReplayFile;
    // Window Pointer
    CWnd* m_pReplayWndPtr;
    // String Array for replay file entries
    CStringArray m_omEntries;
    // Parsed details
    CArray<STCANDATA,STCANDATA> m_omMsgList;
    // Indeces of Break points
    // Hashing based on INdex and value is Break point enabled or not
    CMap<int,int, BOOL,BOOL> m_omBreakPoints;
    // Thread Handle
    HANDLE m_hThread;
    // Event for Thread Indication
    CEvent m_omThreadEvent;

    // Local Variables
    BOOL m_bReplayHexON;
    //Replay flag mode
    WORD m_wLogReplayTimeMode;
    // Index
    int m_nCurrentIndex;
    // Index Cur Sel
    int m_nUserSelectionIndex;
    // Array of indeceas for cyclic message transmission
    CArray<int,int> m_omSelectedIndex;
    // Count of messages for replay
    int m_nNoOfMessagesToPlay;
    // Stop Flag
    BOOL m_bStopReplayThread;
    // For Error Strings
    CString m_omStrError;
private:
    void vFormatCANDataMsg(STCANDATA* pMsgCAN, tagSFRAMEINFO_BASIC_CAN* CurrDataCAN);

};
