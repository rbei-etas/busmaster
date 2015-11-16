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
#include "ReplayManager.h"
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
    //To get a specific line for given line number from log file.
    CString omStrGetMsgFromLog(DWORD dwLineNo,STCANDATA& sCanMsg,bool& bSessionFlag, bool& bEOFflag, bool& bProtocolMismatch, bool& bInvalidMsg);
    DWORD dwGetNoOfMsgsInLog();
    //Close handles related to Memory Map for log file.
    void vCloseReplayFile();
    //To get the size of vecPeg vector.
    DWORD dwGetvecPegSize();
    bool bGetbIsProtocolMismatch();
    void bSetbIsProtocolMismatch(bool bIsProtocolMismatch);
    bool bGetbIsInvalidMsg();
    void bSetbIsInvalidMsg(bool bIsInvalidMessage);
    //To get the specific can message from the log file
    bool sGetCanMsg(CString omStrMessage,STCANDATA& sCanMsg);
    // To load repaly file
    BOOL bOpenReplayFile(BOOL bIsInteractive);
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
    //To store the location of a line(in bytes) in log file.
    std::vector<DWORD> vecPeg;
    //To track the sessions crossed by reading the log file.
    std::vector<bool> vecSessionFlag;
    std::ifstream omInReplayFile;
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
    //Check Interactive
    BOOL m_bIsInteractive;
    // For Error Strings
    CString m_omStrError;
    bool m_bIsProtocolMismatch;
    bool m_bIsInvalidMessage;
    bool m_bIsEmptySession;
private:
    void vFormatCANDataMsg(STCANDATA* pMsgCAN, tagSFRAMEINFO_BASIC_CAN* CurrDataCAN);
    BOOL bMessageTobeBlocked(SFRAMEINFO_BASIC_CAN& sBasicCanInfo);

};
