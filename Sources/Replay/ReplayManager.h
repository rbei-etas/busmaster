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
 * \file      ReplayManager.h
 * \brief     Interface file for CReplayManager class
 * \author    Raja N
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Interface file for CReplayManager class
 */

#pragma once

#include "ReplayFile.h"

class CMsgReplayWnd;
class CReplayProcess;

class CReplayManager
{
public:
    // Avoid Virtual to optimize performance
    ~CReplayManager();
    // To get the singleton instance of Replay Manager
    static CReplayManager& ouGetReplayManager();
    // To show Replay configuration dialog
    DWORD dShowReplayConfigurationDlg(const SFILTERAPPLIED_CAN* psFilterConfigured);
    // To load/store details in to config file
    BOOL Serialize(CArchive& omArchive);
    // Initialize to default
    VOID vInitReplayManager();
    // To show all interactive replay windows
    VOID vShowInteractiveReplayWindows();
    // To start hidden replay threads
    VOID vStartNonInteractiveReplays();
    // To set the active replay window
    VOID vSetActiveReplayWindow(CMsgReplayWnd* pomWindow);
    // To delete window pointer from the list
    BOOL bRemovePointerFromList(CWnd* pWnd);
    // To indicate stop signal to running threads
    void vSetThreadStopSignal();
    // To terminate the thread
    void vStopReplayThread();
    void vSetConfiguredFilter(const void* pvFilterConfigured);
    // To handle connection status change
    void vHandleConnectionStatusChange(BOOL bConnect );
    // To start/stop replay sessions
    void vStartStopReplay( BOOL bStart );
    // To get list of replay file names
    void vGetReplayFileNameList(CStringArray& omList);
    // To get information from old version cfg file
    void vAddOlderVersionReplayInformation();
    // Declare Dialog class as Friend
    friend class CReplayFileConfigDlg;

    // Replay Operations
    VOID vCmdStep();
    VOID vCmdSkip();
    VOID vCmdStop();
    VOID vCmdGo();
    // UI Update Functions
    BOOL bGetUIStateCmdStep();
    BOOL bGetUIStateCmdSkip();
    BOOL bGetUIStateCmdGo();
    BOOL bGetUIStateCmdStop();

public:
    void vGetReplayConfigData(BYTE*& pDesBuffer, int& nBuffSize);
    void vSetReplayConfigData(BYTE* pSrcBuffer, int nBuffSize);
    BOOL bIsReplayConfigChanged();
    void vSetTraceObjPtr( PVOID pvObj);
    void vSendToTrace(char* pcString);
private:
    // Keep the constructor as private to avoid multiple instances
    CReplayManager();
    // To copy Replay manager
    void vCopyReplayManager( CReplayManager& ouDest,
                             const CReplayManager& ouSrc ) const;
private:
    //windowplacement from config
    WINDOWPLACEMENT m_sWndPlacement;
    // Replay File List
    CArray< CReplayFile, CReplayFile > m_omReplayFiles;
    // To keep one static instance
    static CReplayManager m_ouReplayManager;
    // To indicate object creation
    static BOOL m_bCreated;
    // To keep track of active replay window pointer
    CMsgReplayWnd* m_pomActiveWindow;
    // To keep all instances
    CArray<CMsgReplayWnd*,CMsgReplayWnd*> m_omReplayWindowArray;
    // To keep all background tasks
    CArray<CReplayProcess*, CReplayProcess*> m_omReplayProcess;
    // Connection Status
    BOOL m_bConnection;
};
