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
 * \file      HandlerFunc.h
 * \author    Ratnadip Choudhury
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 */

#pragma once

#include "HashDefines.h"

UINT g_unReadDllMsg=0;
//message count
UINT g_unDllMsgCnt=0;
//flag for exiting from thread
BOOL g_bReadDllMsg;

SDLL_MSG g_asDllMessages[defMAX_HMODULE_MSG];
// protoype of the Key Handler thread function
UINT unKeyHandlerProc(LPVOID pParam);

UINT unDLLUnloadHandlerProc(LPVOID pParam);
UINT unDLLloadHandlerProc(LPVOID pParam);
	
// protoype of the Error Handler thread function
UINT unErrorHandlerProc(LPVOID pParam);
// protoype of the Event Handler thread function
UINT unEventHandlerProc(LPVOID pParam);
// prototype of fuontion having common processing of timer call back func.
// prototype for timer thread
UINT unTimerHandlerProc(LPVOID pParam);
// to read node buffer of message queue
UINT unReadNodeMsgHandlerBuffer(LPVOID pParam);
// to read ExecuteManager buffer of message queue
UINT unReadDllMsgBuffer(LPVOID pParam);
