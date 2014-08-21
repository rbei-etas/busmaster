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

//Structure that is to be formed by the protocol from the raw data 
//received from the device
typedef struct sEMsgDetails
{
	long					lMsgID;
	int						nDatalength;
	unsigned char			ucData[1500];
	unsigned char			ucProtocolName[20];
	unsigned char			ucMsgName[20];
	CTime					oTimeStamp;
	sEMsgDetails()
	{
		lMsgID			= -1;
		nDatalength		=  0;
		strcpy((char*)ucProtocolName ,"");
		strcpy((char*)ucMsgName ,"");
		strcpy((char*)ucData ,"");
	}
}EMSGDETAILS;

typedef struct sERawMsg
{
	u_char	chData[1500];
	char	chSrcMAC[12];
	char	chDestMAC[12];
}ERAWMSG;

//Structure that is to be formed from EMSGDETAILS to send to ethernet device
typedef struct sEDataToSend
{
	int						nLength;
	BYTE					byData[MAX_DATALENGTH];

	sEDataToSend()
	{
		nLength		= 0;
	}
}EDATATOSEND;

//Base class for implentation of Application layer protocol
class Base_EAppLayerProtocol
{
public:
	//This function should return the name of the Potocol that is to displayed.
	virtual string GetProtocolName() = 0;

	//Parse the data recived from the device in the function below and form the return structure
	virtual EMSGDETAILS ParseEthernetData(ERAWMSG& sRawMsg) = 0;

	//Create the byte array from the EMSGDETAILS structure and form the retuns structure
	virtual EDATATOSEND FormEDataPacket(EMSGDETAILS sMsg) = 0;
};