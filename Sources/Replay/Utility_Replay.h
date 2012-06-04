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
 * \file      Utility_Replay.h
 * \author    Ratnadip Choudhury
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 */

#pragma once

#include "include/struct_can.h"

// Takes message string and get msg id, dlc and data
BOOL bGetMsgInfoFromMsgStr(CONST CString& omSendMsgLine,
                            PSTCANDATA  psReplayData,
                            BOOL bHexON);
// convert a string into bytearray data after replacing spaces between the bytes
VOID vConvStrtoByteArray(CByteArray* pomByteArrayBufTx, 
                            CHAR* pctempBuf,
                            BOOL bHexON);

// To get the time differenct between two message entries
UINT unTimeDiffBetweenMsg( CString& omStrNextMsg,
                            CString &omStrCurMsg,
                            WORD wLogReplyTimeMode);
BOOL bIsModeMismatch( ifstream& omInReplayFile,
                      BOOL bReplayHexON,
                      WORD wLogReplayTimeMode);
