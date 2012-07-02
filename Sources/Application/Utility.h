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
 * \file      Utility.h
 * \brief     This file contain the definition of all global utility function
 * \author    Amitesh Bharti
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * This file contain the definition of all global utility function
 */

#pragma once

#include "HashDefines.h"
// Global flag user for terminating the thread and disabling GUI.

// used for sending multiple messages.
BOOL g_bStopSendMultMsg     = TRUE; // Stopped
// used for Timer Handlers
BOOL g_bStopTimerHandlers   = TRUE; // Stopped
// used for Key Handlers
BOOL g_bStopKeyHandlers     = TRUE; // Stopped
// used for Error Handlers
BOOL g_bStopErrorHandlers   = TRUE; // Stopped

// used for Error Handlers
BOOL g_bStopDLLHandlers   = TRUE; // Stopped


//read index
UINT g_unReadDllMsg=0;
//write index
UINT g_unWriteDllMsg=0;
//message count
UINT g_unDllMsgCnt=0;
//event for terminating read dll msg thread
extern CEvent g_momDllMsgEvent;
//flag for exiting from thread
BOOL g_bReadDllMsg;
//read dll msg thread pointer
CWinThread* g_pomReadDllThrd;
BOOL g_bMsgHandlerON;
// Critical Section for Signal Watch List
//CRITICAL_SECTION g_CritSectionForSW;
CRITICAL_SECTION g_CritSectionForTimer;
CRITICAL_SECTION g_CritSectNodeBufferRead;

SBUSSTATISTICS g_sBusStatistics[ defNO_OF_CHANNELS ];


// Takes send message structure and message data array,and index to get the msg data
// functionality
BOOL bGetMsgData(UINT unMsgIndex,
                 UCHAR aunData[],
                 PSENDMULTIMSGINFO psSendMultipleMsgInfo);


VOID vProcessTimerCallBack(UINT unIndex, APPTIMERPOINTER pFTimerCallback);
// protoype of the Key Handler thread function
UINT unKeyHandlerProc(LPVOID pParam);

UINT unDLLUnloadHandlerProc(LPVOID pParam);
UINT unDLLloadHandlerProc(LPVOID pParam);

// protoype of the Error Handler thread function
UINT unErrorHandlerProc(LPVOID pParam);
// prototype of fuontion having common processing of timer call back func.
// prototype for timer thread
UINT unTimerHandlerProc(LPVOID pParam);
// to read node buffer of message queue
UINT unReadNodeMsgHandlerBuffer(LPVOID pParam);
// to read ExecuteManager buffer of message queue
UINT unReadDllMsgBuffer(LPVOID pParam);
