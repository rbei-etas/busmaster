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
 * \file      Flags.h
 * \brief     Contains CFlags class definition
 * \authors   Amaranath Shastry, Amitesh Bharti, Amarnath Shastry, Ratnadip Choudhury
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Contains CFlags class definition
 */
#if !defined (_FLAGS_H_INCLUDED_)
#define _FLAGS_H_INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// Definition of PSTOOLBARINFO structure is here
#include "struct.h"

// enumeration for all flags 
typedef enum eCANMONITORFLAG
{
    LOGTODISPLAY = 0, 
    LOGTOFILE, 
    REPLAYMODE,
    REPLAYFILESELECTED,
    LOGFILTER, 
    DISPLAYFILTERON, 
    TRANSLATIONMODE, 
    FILTERUNDEFINEDMSGS, 
    FILTERTXMSGS,
    RXMSG,
    SELECTDATABASEFILE, 
    DLLLOADED, 
    TIMERBUTTON, 
    MSGHANDLERBUTTON, 
    //SIGNALWATCH, 
    FUNCEDITOR, 
    DBOPEN, 
    HEX, 
    DISPLAYMSGWINDOW, 
    DISPLAYTIMEMODE, 
    SCROLLING,
    SENDMESG,
    OVERWRITE,
    TIME_DELAY_REPLAY_FILE,
    PASS_STOP_FILTER,
    CONNECTED,
    KEY_HANDLER_ON,
    ERROR_HANDLER,
    DLL_HANDLER,
    ALL_HANDLER,
    CONTROLLER_MODE,
    PARALLEL_PORT_EPP,
    LOGHEXON,
    LOGTIMEMODE,
    eLOGOVERWRITEMODEON,
    eLOGREPLAYMSGTYPE,
	SEND_SIGNAL_MSG,
    ACTIVATED_J1939,
};

class CFlags  
{
public:
    void vSetToolbarButtonStatus(  PSTOOLBARINFO psToolBarInfo );
    void vGetToolbarButtonStatus( PSTOOLBARINFO psToolBarInfo );
    int nGetFlagStatus(eCANMONITORFLAG WhichFlag);
    VOID vSetFlagStatus(eCANMONITORFLAG WhichFlag, int nValue);
	void vInitializeFlags();
    //CFlags(PSTOOLBARINFO psToolBarInfo);
    //CFlags();
    virtual ~CFlags();
    static CFlags& ouGetFlagObj(void);

private:
    CCriticalSection m_omCriticalSec;
    BOOL m_bLogToWindow;
    BOOL m_bLogEnable;
    BOOL m_bLogFilterOn;
    BOOL m_bDisplayFilterOn;
    BOOL m_bTranslationMode;
    BOOL m_bMsgFilterButton;
    BOOL m_bFilterUndefinedMsgs;
    BOOL m_bRxMsg;
    BOOL m_bSelectDatabaseFile;
    BOOL m_bDllLoaded;
    BOOL m_bTimerButtonOn;
    BOOL m_bFunctionEditorOn;
    BOOL m_bFilterTxMsg;
    BOOL m_bDbOpen;
    WORD m_wReplayMode;
    BOOL m_bMsgHandlerOn;
    WORD m_wDisplayTimeMode;
    BOOL m_bScrolling;
    BOOL m_bDisplayHexON;
    BOOL m_bMessageSelected;
    BOOL m_bOverwriteEnable;
    BOOL m_bReplayFileSelected;
    BOOL m_bTimeDelayReplayFile;
    BOOL m_bStopFilter;
    BOOL m_bConnected;
    BOOL m_bKeyHandlerOn;
    WORD m_wControllerMode;
    BOOL m_bErrorHandlerOn;
    BOOL m_bDLLHandlerOn;
    BOOL m_bALLHandler;
    BOOL m_bParallelPortEPP;
    BOOL m_bLogHexON; 
    WORD m_wLogTimeMode;
    int m_nReplayMsgType;
    BOOL m_bLogOverWriteON;
	BOOL m_bSendSignalMSg;
    BOOL m_bActivatedJ1939;     // Current active bus for configuration.

    // Private constructors to make singleton class
    CFlags();
    CFlags(PSTOOLBARINFO psToolBarInfo);
    static CFlags sm_ouSingletonObj;
};
#endif //#define _FLAGS_H_INCLUDED_
