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
 * \file      AppServicesCAN.h
 * \author    Ratnadip Choudhury
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 */
#ifndef APPSERVICESlLIN_H__INCLUDED_
#define APPSERVICESLIN_H__INCLUDED_

#include "AppServices.h"

//Send can message
UINT gunSendMsg_LIN(STLIN_MSG* psTxMsg, HMODULE hModule);
// To set the controller mode Passive/Active
BOOL gvSetControllerMode_LIN(BOOL);
// To connect/diaconnect with the Hardware
BOOL gbStartStopHardware_LIN(BOOL) ;
//to get first connected database name
DWORD gdGetFirstLINdbName(char* , DWORD );

#endif // APPSERVICESCAN_H__INCLUDED_
