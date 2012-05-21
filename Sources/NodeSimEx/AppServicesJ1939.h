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
 * \file      AppServicesJ1939.h
 * \author    Ratnadip Choudhury
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 */

#pragma once

#include "AppServices.h"

UINT g_unGoOnline_J1939(BOOL bEnable, HMODULE hModule);

UINT g_unSendMsg_J1939(STJ1939_MSG* psTxMsg, HMODULE hModule);

UINT g_unRequestPGN_J1939(UINT PGN, BYTE DestAdres, UINT Channel, HMODULE hModule);

UINT g_unSendAckMsg_J1939(UINT PGN, BYTE AckType, BYTE DestAdres, UINT Channel,
                          HMODULE hModule);

UINT g_unClaimAddress_J1939(BYTE Address, UINT Channel, HMODULE hModule);

UINT g_unRequestAddress_J1939(UINT Channel, HMODULE hModule);

UINT g_unCommandAddress_J1939(UINT64 EcuName, BYTE NewAddress, UINT Channel,
                              HMODULE hModule);

UINT g_unSetTimeout_J1939(BYTE TimeoutType, UINT TimeoutValue, HMODULE hModule);
