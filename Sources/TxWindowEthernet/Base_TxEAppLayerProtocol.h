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
 * \file      Base_EthernetAppLayerProtocol.h
 * \brief     Exports API functions for adding Application Layer Protocol
 * \author    Ashwin R Uchil
 * \copyright Copyright (c) 2014, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Exports API functions for adding Application Layer Protocol
 */

#pragma once
#include <sstream>
#include <string.h>
#include <Windows.h>
#define  MAX_DATALENGTH	1500

using namespace std;

typedef struct sData
{
	unsigned int	unDataLength;
	u_char*			chData;   
}STDATA;

class CBase_TxEAppLayerProtocol
{
public:
	//Adds headers of respective protocols which will be sent to ethernet bus.
	virtual HRESULT AddHeaderToData(STDATA& sData) = 0;
};