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
 * @file
 *  IxxatCanChannel.h
 *
 * @brief
 *  This class represent one IXXAT CAN channel. It allows
 *  to open, configure, receive and transmit CAN messages.
 *
 */

#pragma once


#include "AllIncludes.h"
#include "vcinpl.h"

/**
 * @class CIxxatCanChannel
 *
 * @brief
 *  Class which represent one IXXAT CAN channel.
 *
 */
class CIxxatCanChannel
{
public:
    CIxxatCanChannel(void);
    virtual ~CIxxatCanChannel(void);

    void    SetHardwareParams(INT64 qiVCIDeviceID, int iCANControllerNumber, CClientList* pClientList);
    void    Selected(BOOL bIsSelected);
    BOOL    IsSelected();
    DWORD   GetRxErrorFrameCounter();
    DWORD   GetTxErrorFrameCounter();

    HRESULT SetControllerParameter(SCONTROLLER_DETAILS* pCtrlDetails);
    HRESULT AccessController(BYTE byChannelNumber);
    HRESULT ReleaseController();
    HRESULT InitController();
    HRESULT ResetController();
    HRESULT StartController();
    HRESULT StopController();
    HRESULT GetStatus(s_STATUSMSG* pStatusData);
    HRESULT SendMessage(DWORD dwClientID, const STCAN_MSG& sCanTxMsg);

    void    InnerRxThreadFunc();


protected:
    BOOL m_bIsSelected; ///< true if this object is selected
    BYTE  m_byChannelNumber;  ///< The channel number
    INT64 m_qiVCIDeviceID;    ///< Identifier for the vci device to use
    CClientList* m_pClientList; ///< Pointer to a list of clients
    int   m_iCANControllerNumber; ///< The internal CAN controller number starts with 0 on every device
    BYTE  m_bBtr0;    ///< The Bittiming register 0
    BYTE  m_bBtr1;  ///< The Bittiming register 1
    BOOL  m_bControllerPassiveMode; ///< true controller should be in passive mode, else active mode
    BOOL  m_byCtrlInitialized;    ///< flag if the controller is initialized
    DWORD m_dwRxErrorFrameCounter;    ///< The receive error frame counter

    HANDLE m_hVciDevice;  ///< Handle of the vci device
    HANDLE m_hCANControl; ///< Handle of the can control
    HANDLE m_hCANChannel; ///< Handle of the can channel
    HANDLE m_hThreadHasEnded; ///<  Handle to set as event when the thread has ended

    BOOL   m_bRxThreadRunning;    ///< true means that the thread running
    long   m_lMustQuit; ///< Flag to inicate if the thread must quit

    DWORD  m_dwTimerTickResolutionNSec;   ///< The timer tick in nanoseconds
    UINT64 m_qwTimerOverruns; ///< The number of timer overruns since start



    void DestroyCANController();
    HRESULT CreateCANChannel();
    void DestroyCANChannel();

    void HandleReceivedMessage(CANMSG* pCanMsg);
    void DispatchFrameToClients(STCANDATA* pStCanData);
    void ConvertIxxatCanToBusmasterMsg(CANMSG* pCanMsg, STCANDATA* pStCanData);
    void ConvertIxxatErrorToBusmasterMsg(CANMSG* pCanMsg, STCANDATA* pStCanData);
    void ConvertBusmasterToIxxatMsg(CANMSG* pCanMsg, const STCAN_MSG& pStCanData);
    void ConvertIxxatStatusToBusmasterStatus(CANCHANSTATUS* pIxxatChannelStatus, s_STATUSMSG* pStatusData);
};
