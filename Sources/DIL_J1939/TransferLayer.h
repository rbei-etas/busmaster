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
 * \file      ConnectionDet.h
 * \brief     Definition file for Transfer Layer
 * \author    Pradeep Kadoor
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Definition file for Transfer Layer
 */

#pragma once

#include "DIL_Interface/BaseDIL_CAN.h"

class CTransferLayer
{
private:
    CTransferLayer(void);
    CBaseDIL_CAN* m_pIDIL_CAN;
public:
    ~CTransferLayer(void);

    // CONNECTION SET-UP SERVICE -- START
    // Request for connection establishment
    LONG lTConnectReq(short sConNumber, char bBlockSize, eCON_MODE eSMmode);
    // Confirmation of connection establishment
    void vTConnectCon(short sConNumber, eCON_STATUS ConStatus,
                      char bBlockSize, eCON_MODE bServiceMode);
    // Indication of connection establishment
    void vTConnectInd(short sConNumber, char bBlockSize, BOOL bIsSMEnhance);
    // Indication of connection termination
    void vTDisconnectInd(short sConNumber, eREASON eReason);
    // CONNECTION SET-UP SERVICE -- END

    // CONNECTION TEST SERVICE -- START
    // Request for connection test
    LONG lTConTestReq(short sConNumber);
    // Confirmation of connection test
    void vTConTestCon(short sConNumber, char cConnectionStatus, char cBlockSize, char cServiceMode);
    void vTLongDataCon(short sConNumber, char cTransferResult);
    void vTBroadDataInd(short sBroadcastChannel,short sDataLength, BYTE* pbData);

    //Singleton class
    static CTransferLayer& ouGetTransLayerObj();
    void vTransmitCANMsg(DWORD dwClientID, UINT unID,
                         UCHAR ucDataLen, BYTE* pData, UINT unChannel);
    void vSetIDIL_CAN(CBaseDIL_CAN* pIDIL_CAN);

};
