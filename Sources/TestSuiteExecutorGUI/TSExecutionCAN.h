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
 * \file      TSExecutionCAN.h
 * \author    Venkatanarayana makam
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 */

#pragma once

#include "Utility/Utility_Thread.h"
#include "Utility/MsgInterpretation.h"
#include "Include/struct_can.h"
#include "Datatypes/MsgBufAll_Datatypes.h"
#include "Datatypes/MsgBufCANVFSE.h"
#include "Include/Basedefs.h"
#include "DIL_Interface/BaseDIL_CAN.h"
#include "TSExecutorBase.h"

typedef CMsgBufCANVFSE<STCANDATA> CCANBufferVFSE;
typedef CList<UINT, UINT&> MsgVerifiedList;
class CTSExecutionCAN: public CTSExecutorBase
{
    //Attributes
private:

    //Bus Statictics Structure
    SBUSSTATISTICS m_sBusStatistics[ defNO_OF_CHANNELS ];
    //Temporary Referance of STCAN_MSG
    STCAN_MSG m_sCurrEntry;
    //Client ID
    DWORD m_dwClientId;
    //CAN DIL pointer
    CBaseDIL_CAN* m_pouDIL_CAN;
public:
    //used to Creating threads
    CPARAM_THREADPROC m_ouReadThread;
    //Made Public because of the Thread
    //CAN buffer
    CCANBufFSE m_ouCanBufFSE;
    //Message storing Buffer
    CCANBufferVFSE m_ouCanBufVFSE;
    //Message InterpreterS
    CMsgInterpretation m_ouMsgInterpret;
    //Move to structure - Global Structure shared by thread
    CVerifyResponse* m_pCurrentVerify;
    UINT m_nVerifyCount;
    CEvent m_ouVerifyEvent;
    CResultVerify* m_ouVerifyResult;
    MsgVerifiedList m_MsgVerifiedList;
    CRITICAL_SECTION m_omCritSecTS;
    BOOL m_bTimeOver;
    long long int m_QueryFrequency;
    UINT64 m_LastCanMsg;

    //Member Functions
public:
    //Constructor
    CTSExecutionCAN(void);
    // This function should be called for the very first time.
    HRESULT TSX_DoInitialization(void);
    // This function resets the Message statistics
    HRESULT TSX_Reset(void);
    // Starts Reading Message Thread
    HRESULT TSX_bStartStopReadThread(BOOL bStart);
    // Sends A Message
    HRESULT TSX_SendMessage(CBaseEntityTA* pEntity);
    //Verifies The Message
    HRESULT TSX_VerifyMessage(CBaseEntityTA* pEntity, CResultVerify& ouVerifyResult);
    //VerifyResponse
    HRESULT TSX_VerifyResponse(CBaseEntityTA* pEntity, CResultVerify& ouVerifyResult);
    //Descrutor
    // HRESULT VerifyCurrentMessage(STCANDATA& sCanData);
    //Verifies the can Message
    BOOL bVerifyCanMessage(CVerify_MessageData& ouVerifyData, CSignalInfoArray& ouSignalInfo, CMessageResult& ouMsgResult);
    virtual ~CTSExecutionCAN(void);

private:
    //Initialise or resets the bus statistics.
    void vInitialiseTSXData(void);
    //Starts the BS Read Thread
    BOOL bStartBSReadThread(void);
    //Constructs the Can Message From the CSend_MessageData
    BOOL bMakeCanMessage(sMESSAGE*& pMsg, CSend_MessageData& ouSendData, UCHAR aucData[8], STCAN_MSG& stCanMsg);

};
