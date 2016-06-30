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
 * @author Ratnadip Choudhury
 * @copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 */

#pragma once

BOOL gbGetProgramVersion(int* pnMajorVersion, int* pnMinorVersion);

HMODULE ghGetNodeDllHandler(char* pcNodeName);

UINT gunMsgTransmissionOnOff(BOOL bOnOff, HMODULE hModule);



// Enable/Disable logging
BOOL gbEnableDisableLog(BOOL) ;
// To write in to the log file
BOOL gbWriteToLog(char*) ;
// To write in to the trace window
BOOL gbSendStrToTrace(char*) ;
// Start/Stop timers
BOOL gbSetResetTimer(char*,int,BOOL,HMODULE) ;
// To set timer value
BOOL gbSetTimerVal(CHAR* , UINT,HMODULE );


//To Get Signal Physical value
double gdGetSignalEngValue(int nBus, unsigned long long nRawValue, int length, int type, unsigned long long unClusterId, const char* pchCodingName);

//To get Signal Raw Value
unsigned long long gdGetSignalRawValue( int bus, double unEngValue, unsigned long long unClusterId, const char* pchCodingName );


ICoding* GetCoding(int bus, unsigned long long unClusterId, const char* pchCodingName);