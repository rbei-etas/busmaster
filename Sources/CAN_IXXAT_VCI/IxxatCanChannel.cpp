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
 *  IxxatCanChannel.cpp
 *
 * @brief
 *  This class represent one IXXAT CAN channel. It allows
 *  to open, configure, receive and transmit CAN messages.
 *
 * @author  Peter Wucherer
 * @date  27.04.2012
 */

#include "CAN_IXXAT_VCI_stdafx.h"
#include "ClientList.h"
#include "IxxatCanChannel.h"
#include "vcinpl.h"
#include <process.h> // _beginthread
#include "vcinpldynl.h"

#include "CAN_Error_Defs.h"


// forward declaration for the C-Function
/**
 * @brief
 *  Forward declaration of the "C" Receive thread function.
 *
 * @param [in,out]  pParam
 *  Must be a pointer to the calling CIxxatCanChannel
 *  class object.
 */
void ReceiveThread( void* pParam);

/**
 * @brief
 *  Default constructor.
 */
CIxxatCanChannel::CIxxatCanChannel(void)
{
    m_byChannelNumber = 0;
    m_qiVCIDeviceID = 0;
    m_pClientList = NULL;

    m_iCANControllerNumber = 0;
    m_bIsSelected = FALSE;
    m_byCtrlInitialized = FALSE;

    m_hVciDevice = NULL;
    m_hCANControl = NULL;
    m_hCANChannel = NULL;

    m_bRxThreadRunning = FALSE;
    m_lMustQuit = 0;

    m_dwTimerTickResolutionNSec = 0;
    m_qwTimerOverruns = 0;

    m_dwRxErrorFrameCounter = 0;

    m_bBtr0 = 0;
    m_bBtr1 = 0;

}

/**
 * @brief
 *  Destructor. Clean up the class internal objects.
 */
CIxxatCanChannel::~CIxxatCanChannel(void)
{
}

/**
 * @brief
 *  Sets a hardware parameter to use for CAN controller
 *  initialization and the client list to use.
 *
 * @param qiVCIDeviceID
 *  The IXXAT device object identifier. Every IXAT interface
 *  has an own unique number which must be used to open it.
 *  Interface means a PCI card or a USB based interface with
 *  can have more CAN controllers.
 * @param iCANControllerNumber
 *  Zero-based index of the can controller number to use in
 *  this class object.
 * @param [in,out]  pClientList
 *  Object pointer with is used to handle the clients and
 *  message buffers to dispatch CAN messages.
 */
void CIxxatCanChannel::SetHardwareParams(INT64 qiVCIDeviceID, int iCANControllerNumber, CClientList* pClientList)
{
    m_qiVCIDeviceID = qiVCIDeviceID;
    m_iCANControllerNumber = iCANControllerNumber;
    m_pClientList = pClientList;
}

/**
 * @brief
 *  Gets hardware parameter to use for CAN controller
 *  initialization and the client list to use.
 *
 * @param qiVCIDeviceID
 *  The IXXAT device object identifier. Every IXAT interface
 *  has an own unique number which must be used to open it.
 *  Interface means a PCI card or a USB based interface with
 *  can have more CAN controllers.
 * @param iCANControllerNumber
 *  Zero-based index of the can controller number to use in
 *  this class object.
 * @param [out]  ppvClientList
 *  void double pointer returned which is used to handle the clients and
 *  message buffers to dispatch CAN messages.
 */
void CIxxatCanChannel::GetHardwareParams(INT64* pqiVCIDeviceID, int* piCANControllerNumber, void** ppvClientList)
{
    *pqiVCIDeviceID = m_qiVCIDeviceID;
    *piCANControllerNumber = m_iCANControllerNumber;
    *ppvClientList = (void*)m_pClientList;
}


/**
 * @brief
 *  Set the flag if this channel object is selected or not.
 *
 * @param bIsSelected
 *  True if this object is selected.
 *
 */
void CIxxatCanChannel::Selected(BOOL bIsSelected)
{
    m_bIsSelected = bIsSelected;
}

/**
 * @brief
 *  Query if this object is selected.
 *
 * @return
 *  true if selected, false if not.
 *
*/
BOOL CIxxatCanChannel::IsSelected()
{
    return m_bIsSelected;
}

/**
 * @brief
 *  Gets the number of received error frames
 *  since the last call to reset.
 *
 * @return
 *  The number of received error frames.
 *
 */
DWORD CIxxatCanChannel::GetRxErrorFrameCounter()
{
    return m_dwRxErrorFrameCounter;
}

/**
 * @brief
 *  Sets the parameter needed for CAN controller initialization.
 *
 * @todo
 *   when config changed while started or initialized then restart the controller,
 *   and restore the state
 *
 * @param [in] pCtrlDetails
 *  A structure with the details. Here only the bit timing register
 *  values and the tx-passive mode flag will be used.
 *
 * @return
 *  Always S_OK. No check for valid values.
 */
HRESULT CIxxatCanChannel::SetControllerParameter(SCONTROLLER_DETAILS* pCtrlDetails)
{
    // see struct_can.h:
    // UCHAR m_ucControllerMode;
    // // Controller mode : 1 : Active, 2: Passive

    m_bControllerPassiveMode = FALSE;
    if (pCtrlDetails->m_ucControllerMode == 2)
    {
        m_bControllerPassiveMode = TRUE;
    }

    m_bBtr0 = ((pCtrlDetails->m_nBTR0BTR1 & 0xFF00) >> 8);
    m_bBtr1 = (pCtrlDetails->m_nBTR0BTR1 & 0x00FF);


    return S_OK;
}

/**
 * @brief
 *  Prepare this class object to use the CAN controller.
 *  Try to open the access to the IXXAT interface card.
 *
 * @param byChannelNumber [in]
 *  The channel number to set into the dispatched messages that
 *  the program knows which CAN channel has received the message.
 *  Start with 1.
 *
 * @return
 *   HW_INTERFACE_ALREADY_SELECTED - the interface is already open.
 *   ERR_LOAD_HW_INTERFACE - Error opening interface or CAN controller.
 *   S_OK (or VCI_OK = 0) - Success.
 *
  */
HRESULT CIxxatCanChannel::AccessController(BYTE byChannelNumber)
{
    HRESULT hResult = HW_INTERFACE_ALREADY_SELECTED;

    m_byChannelNumber = byChannelNumber;

    if (!m_hVciDevice)
    {
        VCIID vciId;
        vciId.AsInt64 = m_qiVCIDeviceID;
        hResult = DYNCALL(vciDeviceOpen)(vciId, &m_hVciDevice);

        if (VCI_OK != hResult)
        {
            hResult = ERR_LOAD_HW_INTERFACE;
        }
        else
        {
            hResult = DYNCALL(canControlOpen)(m_hVciDevice, (DWORD) m_iCANControllerNumber, &m_hCANControl);
            if (VCI_OK != hResult)
            {
                hResult = ERR_LOAD_HW_INTERFACE;
            }
        }

    }
    return hResult;
}


/**
 * @brief
 *  Releases the controller.
 *
 * @return
 *  HW_INTERFACE_ALREADY_CLOSED - No access opened.
 *  ERR_CLOSE_HW_INTERFACE - VCI function returns an error code.
 *  CALL_SUCCESS - Success.
 *
 */
HRESULT CIxxatCanChannel::ReleaseController()
{
    HRESULT hResult = HW_INTERFACE_ALREADY_CLOSED;

    DestroyCANController();
    DestroyCANChannel();

    if (m_hVciDevice)
    {
        hResult = DYNCALL(vciDeviceClose)(m_hVciDevice);
        m_hVciDevice = NULL;
        if (VCI_OK == hResult)
        {
            hResult = CALL_SUCCESS;
        }
        else
        {
            hResult = ERR_CLOSE_HW_INTERFACE;
        }
    }
    return hResult;
}

/**
 * @brief
 *  Initializes the CAN controller.
 *
 * @return
 *  HW_INTERFACE_NO_SEL - No CAN controller with AccessController() opened.
 *  INFO_INITDAT_CONFIRM_CONFIG - Successful initialized.
 *  ERR_INITDAT_CONFIRM_CONFIG - Error from VCI function.
 *
 */
HRESULT CIxxatCanChannel::InitController()
{
    HRESULT hResult = HW_INTERFACE_NO_SEL;
    if (m_hCANControl)
    {
        m_byCtrlInitialized = FALSE;

        BYTE byMode = CAN_OPMODE_STANDARD | CAN_OPMODE_EXTENDED | CAN_OPMODE_ERRFRAME;
        if (m_bControllerPassiveMode)
        {
            byMode &= CAN_OPMODE_LISTONLY;
        }

        hResult = DYNCALL(canControlInitialize)(m_hCANControl, byMode, m_bBtr0, m_bBtr1);

        // The access denied error can occur when another application has
        // this CAN controller already opened and has the control about it.
        // In this case we can try to open a channel to listen parallel to
        // this other application. But we have no possibility to set baud rates,
        // filters etc.
        if (VCI_E_ACCESSDENIED == hResult)
        {
            hResult = S_OK;
        }


        if (VCI_OK == hResult)
        {
            hResult = CreateCANChannel();
        }
        if (VCI_OK == hResult)
        {
            hResult = INFO_INITDAT_CONFIRM_CONFIG;
            m_byCtrlInitialized = TRUE;
        }
        else
        {
            hResult = ERR_INITDAT_CONFIRM_CONFIG;
        }

    }
    return hResult;
}

/**
 * @brief
 *  Stop and resets the controller.
 *
 * @return
 *  HW_INTERFACE_NO_SEL - No controller selected.
 *  ERR_INITDAT_CONFIRM_CONFIG - Error while calling driver function.
 *  S_OK - Success.
 *
 */
HRESULT CIxxatCanChannel::ResetController()
{
    HRESULT hResult = HW_INTERFACE_NO_SEL;
    if (m_hCANControl)
    {
        StopController();
        hResult = DYNCALL(canControlReset)( m_hCANControl );

        if (VCI_OK != hResult)
        {
            hResult = ERR_INITDAT_CONFIRM_CONFIG;
        }
        m_byCtrlInitialized = FALSE;
        m_dwRxErrorFrameCounter = 0;
    }
    return hResult;
}


/**
 * @brief
 *  Starts bus access with controller.
 *  The controller must accessed and initialized with
 *  AccessController() and InitController().
 *
 * @return
 *  HW_INTERFACE_NO_SEL - No controller open.
 *  ERROR_OTHER - Error starting the controller.
 *  S_OK - success.
 *
 */
HRESULT CIxxatCanChannel::StartController()
{
    HRESULT hResult = HW_INTERFACE_NO_SEL;
    if (m_hCANControl)
    {
        hResult = DYNCALL(canControlStart)(m_hCANControl, TRUE);
        if (VCI_OK != hResult)
        {
            hResult = ERROR_OTHER;
        }
    }
    return hResult;
}

/**
 * @brief
 *  Stops the CAN controller.
 *
 * @return
 *  HW_INTERFACE_NO_SEL - No controller selected.
 *  ERROR_OTHER - DLL function has returned an error.
 *  S_OK - Success.
 *
 */
HRESULT CIxxatCanChannel::StopController()
{
    HRESULT hResult = HW_INTERFACE_NO_SEL;
    if (m_hCANControl)
    {
        hResult = DYNCALL(canControlStart)(m_hCANControl, FALSE);
        if (VCI_OK != hResult)
        {
            hResult = ERROR_OTHER;
        }
    }
    return hResult;

}

/**
 * @brief
 *  Gets the current status of the CAN controller.
 *
 * @param [out]  pStatusData
 *  If non-null, struct to store the status.
 *
 * @return
 *  HW_INTERFACE_NO_SEL - No controller selected.
 *  An VCI error result.
 *  S_OK - Successful.
 *
 */
HRESULT CIxxatCanChannel::GetStatus(s_STATUSMSG* pStatusData)
{
    HRESULT hResult = HW_INTERFACE_NO_SEL;
    if (m_hCANChannel)
    {
        CANCHANSTATUS sChannelStatus;
        hResult = DYNCALL(canChannelGetStatus)( m_hCANChannel, &sChannelStatus);
        if (VCI_OK == hResult)
        {
            ConvertIxxatStatusToBusmasterStatus(&sChannelStatus, pStatusData);
        }
    }
    return hResult;
}


/**
 * @brief
 *  Sends a CAN message on the CAN bus.
 *  TODO: what's with the client ID?
 *
 * @param dwClientID
 *  Identifier for the client.
 * @param sCanTxMsg
 *  The CAN message to transmit.
 *
 * @return
 *  HW_INTERFACE_NO_SEL - No controller selected.
 *  ERR_NO_CLIENT_EXIST - A client with this ID did not exists.
 *  An VCI error result.
 *  S_OK - Successful.
 *
 */
HRESULT CIxxatCanChannel::SendMessage(DWORD dwClientID, const STCAN_MSG& sCanTxMsg)
{
    HRESULT hResult = HW_INTERFACE_NO_SEL;
    if (m_hCANChannel)
    {
        CClientBuffer* pClient = m_pClientList->GetClientByID( (int) dwClientID);
        if (pClient)
        {
            CANMSG sIxxatCanMsg;
            ZeroMemory(&sIxxatCanMsg, sizeof(CANMSG));
            ConvertBusmasterToIxxatMsg(&sIxxatCanMsg, sCanTxMsg);

            // function returns: VCI_OK , VCI_E_TIMEOUT, VCI_E_TXQUEUE_FULL
            hResult = DYNCALL(canChannelSendMessage)(m_hCANChannel, 100, &sIxxatCanMsg);
        }
        else
        {
            hResult = ERR_NO_CLIENT_EXIST;
        }
    }
    return hResult;
}


/**
 * @brief
 *  Stop and release the CAN controller.
 *
 */
void CIxxatCanChannel::DestroyCANController()
{
    if (m_hCANControl)
    {
        StopController();
        DYNCALL(canControlClose)( m_hCANControl );
        m_hCANControl = NULL;
    }
}

/**
 * @brief
 *  Create a new CAN channel.
 *
 * @return
 *  ERR_LOAD_HW_INTERFACE - No access opened with AccessController()
 *  HW_INTERFACE_ALREADY_SELECTED - Controller already opened.
 *  An VCI error result.
 *  S_OK - Successful.
 *
 */
HRESULT CIxxatCanChannel::CreateCANChannel()
{
    HRESULT hResult = ERR_LOAD_HW_INTERFACE;

    if (m_hVciDevice)
    {
        // no channel existing, but device accessed
        if (!m_hCANChannel)
        {
            //
            // create and initialize a message channel
            //
            hResult = DYNCALL(canChannelOpen)(m_hVciDevice, (DWORD) m_iCANControllerNumber, FALSE, &m_hCANChannel);

            //
            // initialize the message channel
            //
            if (hResult == VCI_OK)
            {
                UINT16 wRxFifoSize  = 1024;
                UINT16 wRxThreshold = 1;
                UINT16 wTxFifoSize  = 128;
                UINT16 wTxThreshold = 1;

                hResult = DYNCALL(canChannelInitialize)( m_hCANChannel, wRxFifoSize, wRxThreshold, wTxFifoSize, wTxThreshold);
            }

            CANCAPABILITIES sCanCapabilities;
            ZeroMemory(&sCanCapabilities, sizeof(CANCAPABILITIES));
            // initialize the time calculation values
            if (hResult == VCI_OK)
            {
                hResult = DYNCALL(canChannelGetCaps)( m_hCANChannel, &sCanCapabilities);
            }

            // reset the timer overrun variable after open or reopen a CAN channel
            m_qwTimerOverruns = 0;
            if (hResult == VCI_OK)
            {
                if (  (sCanCapabilities.dwTscDivisor != 0)
                        && (sCanCapabilities.dwClockFreq > 0) )
                {
                    // use a UINT64 to be sure to have enough place to
                    // store the values
                    UINT64 qwTimerRes   =  1000000000 * (UINT64) sCanCapabilities.dwTscDivisor;

                    // calculate now the timer resolution in 1 nSecond ticks
                    m_dwTimerTickResolutionNSec = DWORD(qwTimerRes / sCanCapabilities.dwClockFreq);
                }
                else
                {
                    //TODO: what to do in release mode
#ifdef _DEBUG
                    ASSERT(FALSE);
#endif
                    m_dwTimerTickResolutionNSec = sCanCapabilities.dwClockFreq;
                }
            }
            //
            // activate the CAN channel
            //
            if (hResult == VCI_OK)
            {
                hResult = DYNCALL(canChannelActivate)(m_hCANChannel, TRUE);
            }

            if (hResult == VCI_OK)
            {
                m_bRxThreadRunning = TRUE;
                InterlockedExchange(&m_lMustQuit, 0);
                m_hThreadHasEnded =  CreateEvent(NULL, false, false, NULL);
                _beginthread( ReceiveThread, 0, this);
            }
        }
        else
        {
            hResult = HW_INTERFACE_ALREADY_SELECTED;
        }
    }  // if (m_hVciDevice)

    return hResult;
}

/**
 * @brief
 *  Release the CAN channel.
 *
 */
void CIxxatCanChannel::DestroyCANChannel()
{
    if (m_hCANChannel)
    {
        if (m_bRxThreadRunning)
        {
            InterlockedExchange(&m_lMustQuit, 1);
            WaitForSingleObject(m_hThreadHasEnded, INFINITE);
            m_bRxThreadRunning = NULL;
        }

        DYNCALL(canChannelActivate)(m_hCANChannel, FALSE);
        DYNCALL(canChannelClose)(m_hCANChannel);
        m_hCANChannel = NULL;
    }
}

/**
 * @brief
 *  The receive thread function. Called from the external
 *  "C" thread function. Look for received CAN messages
 *  and dispatch it to the clients.
 *  No matter if a big CAN bus load is there, after 100 messages handled
 *  the loop sees that it must quit with the flag m_lMustQuit
 *  and ends then the thread.
 *
 */
void CIxxatCanChannel::InnerRxThreadFunc()
{
    CANMSG arrCanMsg[100];
    UINT32 dwNumOfMsg;
    HRESULT hResult = S_OK;

    // run this loop inside this thread till
    // the m_lMustQuit is set to != 0
    while (0 == m_lMustQuit)
    {
        dwNumOfMsg = 100;

        // wait till messages was received, else wait 100 milliseconds
        hResult = DYNCALL(canChannelReadMultipleMessages)(m_hCANChannel, 100, &dwNumOfMsg, arrCanMsg);
        if (VCI_OK == hResult)
        {

            for (int i=0; i < (int)dwNumOfMsg; i++)
            {
                HandleReceivedMessage(&arrCanMsg[i]);
            }

        }
    }

    // inform the destroy function that it can finish it's work
    SetEvent(m_hThreadHasEnded);
}

/**
 * @brief
 *  Handles the received CAN message.
 *  It will be converted to busmaster CAN structure
 *  and dispatched to all clients.
 *
 * @param [in,out]  pCanMsg
 *  If non-null, IXXAT CAN message object.
 *
 */
void CIxxatCanChannel::HandleReceivedMessage(CANMSG* pCanMsg)
{
    STCANDATA sStCanData;
    switch (pCanMsg->uMsgInfo.Bits.type)
    {
        case CAN_MSGTYPE_DATA:    // data frame
            ConvertIxxatCanToBusmasterMsg(pCanMsg, &sStCanData);
            DispatchFrameToClients(&sStCanData);
            break;
        case CAN_MSGTYPE_INFO:    // info frame
            break;
        case CAN_MSGTYPE_ERROR:   // error frame
            ConvertIxxatErrorToBusmasterMsg(pCanMsg, &sStCanData);
            m_dwRxErrorFrameCounter++;
            DispatchFrameToClients(&sStCanData);
            break;
        case CAN_MSGTYPE_STATUS:  // status frame
            break;
        case CAN_MSGTYPE_WAKEUP:  // wakeup frame
            // will never occur (26-04-2012)
            break;
        case CAN_MSGTYPE_TIMEOVR: // timer overrun
            // add the overrun count which occurred since the last timer overrun message
            // it is the value in the identifier field.
            m_qwTimerOverruns += ( (UINT64) pCanMsg->dwMsgId) << 32;
            break;
        case CAN_MSGTYPE_TIMERST: // timer reset
            // will never occur (26-04-2012)
            break;
    }
}


/**
 * @brief
 *  Dispatch CAN messages to all clients.
 *
 * @param [in,out]  pStCanData
 *  If non-null, information describing a busmaster can structure .
 *
 */
void CIxxatCanChannel::DispatchFrameToClients(STCANDATA* pStCanData)
{
    for (int iClientIndex=0; iClientIndex < m_pClientList->Size(); iClientIndex++)
    {
        CClientBuffer* pClient = m_pClientList->GetClientByIndex(iClientIndex);
        if (pClient)
        {
            for (int iBufIndex = 0; iBufIndex < pClient->NumOfSEBuffers(); iBufIndex++)
            {
                CBaseCANBufFSE* pCANBufSE = pClient->GetSEBufferByIndex(iBufIndex);
                if (pCANBufSE)
                {
                    pCANBufSE->WriteIntoBuffer(pStCanData);
                }
            }
        }
    }
}


/**
 * @brief
 *  Convert a IXXAT CAN data message to a busmaster CAN message.
 *
 * @param [in]  pCanMsg
 *  IXXAT CAN message.
 * @param [out]  pStCanData
 *  Structure to copy the converted data into.
 *
 */
void CIxxatCanChannel::ConvertIxxatCanToBusmasterMsg(CANMSG* pCanMsg, STCANDATA* pStCanData)
{
    ZeroMemory(pStCanData, sizeof(STCANDATA));

    if (pCanMsg->uMsgInfo.Bits.srr)
    {
        pStCanData->m_ucDataType = TX_FLAG;
    }
    else
    {
        pStCanData->m_ucDataType = RX_FLAG;
    }

    /*Set CAN FD to false*/
    pStCanData->m_bCANFDMsg = false;
    pStCanData->m_uDataInfo.m_sCANMsg.m_ucChannel = m_byChannelNumber;

    pStCanData->m_lTickCount.QuadPart = pCanMsg->dwTime + m_qwTimerOverruns;
    pStCanData->m_lTickCount.QuadPart *= m_dwTimerTickResolutionNSec;

    // change it to a tick with 0.1 milliseconds
    pStCanData->m_lTickCount.QuadPart /= 100000;

    pStCanData->m_uDataInfo.m_sCANMsg.m_unMsgID = pCanMsg->dwMsgId;
    pStCanData->m_uDataInfo.m_sCANMsg.m_ucDataLen = pCanMsg->uMsgInfo.Bits.dlc;
    pStCanData->m_uDataInfo.m_sCANMsg.m_ucEXTENDED = pCanMsg->uMsgInfo.Bits.ext;
    pStCanData->m_uDataInfo.m_sCANMsg.m_ucRTR = pCanMsg->uMsgInfo.Bits.rtr;

    // CAN specification allows a DLC up to 15,
    // the values from 9 to 15 can be used for additional information,
    // but as I know nobody do that
    if (pCanMsg->uMsgInfo.Bits.dlc > 8)
    {
        memcpy(&pStCanData->m_uDataInfo.m_sCANMsg.m_ucData, pCanMsg->abData, 8);
    }
    else
    {
        memcpy(&pStCanData->m_uDataInfo.m_sCANMsg.m_ucData, pCanMsg->abData, pCanMsg->uMsgInfo.Bits.dlc);
    }
}

/**
/**
 * @brief
 *  Convert a IXXAT CAN error message to a busmaster CAN message.
 *
 * @param [in]  pCanMsg
 *  IXXAT CAN message.
 * @param [out]  pStCanData
 *  Structure to copy the converted data into.
 *
 */
void CIxxatCanChannel::ConvertIxxatErrorToBusmasterMsg(CANMSG* pCanMsg, STCANDATA* pStCanData)
{
    ZeroMemory(pStCanData, sizeof(STCANDATA));
    pStCanData->m_ucDataType = ERR_FLAG;

    pStCanData->m_uDataInfo.m_sErrInfo.m_ucChannel = m_byChannelNumber;

    pStCanData->m_lTickCount.QuadPart = pCanMsg->dwTime + m_qwTimerOverruns;
    pStCanData->m_lTickCount.QuadPart *= m_dwTimerTickResolutionNSec;

    // change it to a tick with 0.1 milliseconds
    pStCanData->m_lTickCount.QuadPart /= 100000;

    pStCanData->m_uDataInfo.m_sErrInfo.m_ucErrType = ERROR_BUS;

    // convert the IXXAT error types to Busmaster error types
    switch (pCanMsg->abData[0])
    {
        case CAN_ERROR_STUFF:  // stuff error
            if (pCanMsg->uMsgInfo.Bits.srr)
            {
                pStCanData->m_uDataInfo.m_sErrInfo.m_nSubError = STUFF_ERROR_TX;
            }
            else
            {
                pStCanData->m_uDataInfo.m_sErrInfo.m_nSubError = STUFF_ERROR_RX;
            }
            break;
        case CAN_ERROR_FORM:   // form error
            if (pCanMsg->uMsgInfo.Bits.srr)
            {
                pStCanData->m_uDataInfo.m_sErrInfo.m_nSubError = FORM_ERROR_TX;
            }
            else
            {
                pStCanData->m_uDataInfo.m_sErrInfo.m_nSubError = FORM_ERROR_RX;
            }
            break;
        case CAN_ERROR_BIT:    // bit error
            if (pCanMsg->uMsgInfo.Bits.srr)
            {
                pStCanData->m_uDataInfo.m_sErrInfo.m_nSubError = BIT_ERROR_TX;
            }
            else
            {
                pStCanData->m_uDataInfo.m_sErrInfo.m_nSubError = BIT_ERROR_RX;
            }
            break;
        case CAN_ERROR_CRC:    // CRC error
        {
            if (pCanMsg->uMsgInfo.Bits.srr)
            {
                pStCanData->m_uDataInfo.m_sErrInfo.m_nSubError = BIT_ERROR_TX;
            }
            else
            {
                pStCanData->m_uDataInfo.m_sErrInfo.m_nSubError = BIT_ERROR_RX;
            }
            break;
        }
        break;
        case CAN_ERROR_ACK:    // acknowledgment error
        case CAN_ERROR_OTHER:  // other (unspecified) error
            if (pCanMsg->uMsgInfo.Bits.srr)
            {
                pStCanData->m_uDataInfo.m_sErrInfo.m_nSubError = OTHER_ERROR_TX;
            }
            else
            {
                pStCanData->m_uDataInfo.m_sErrInfo.m_nSubError = OTHER_ERROR_RX;
            }
            break;
    }
}


/**
 * @brief
 *  Convert a Busmaster CAN message to a IXXAT CAN data message.
 *  Needed for send messages.
 *
 * @param [out]  pCanMsg
 *  Structure to copy the converted data into.
 * @param [in]  pStCanData
 *  Busmaster CAN message to get the data.
 *
 */
void CIxxatCanChannel::ConvertBusmasterToIxxatMsg(CANMSG* pCanMsg, const STCAN_MSG& pStCanData)
{
    pCanMsg->dwMsgId = pStCanData.m_unMsgID;
    pCanMsg->uMsgInfo.Bits.dlc = pStCanData.m_ucDataLen;
    if (pStCanData.m_ucDataLen > 8)
    {
        memcpy(&pCanMsg->abData, &pStCanData.m_ucData, pStCanData.m_ucDataLen);
    }
    else
    {
        memcpy(&pCanMsg->abData, &pStCanData.m_ucData, 8);
    }

    if (pStCanData.m_ucEXTENDED)
    {
        pCanMsg->uMsgInfo.Bits.ext = 1;
    }

    if (pStCanData.m_ucRTR)
    {
        pCanMsg->uMsgInfo.Bits.rtr = 1;
    }

    // send always with self reception flag
    pCanMsg->uMsgInfo.Bits.srr = 1;
}


/**
 * @brief
 *  Convert IXXAT CAN status to busmaster status structure.
 *
 * @param [in]  pIxxatChannelStatus
 *  The IXXAT CAN channel status.
 * @param [out]  pStatusData
 *  The structure to write the converted data into.
 *
 */
void CIxxatCanChannel::ConvertIxxatStatusToBusmasterStatus(CANCHANSTATUS* pIxxatChannelStatus, s_STATUSMSG* pStatusData)
{
    /*
    typedef struct struct_STATUSMSG
    {
      WORD  wControllerStatus;                  // Current controller state
      // 0 := reset
      // 1 := stopped / initialized
      // 2 := started / waiting for startup completion
      // 3 := started / normal active (running)
      // 4 := started / normal passiv
      // 5 := started / halt mode
      DWORD dwStatusInfoFlags;                  // Flagfield of status information (UCI_FLXSTSINFO_???)
    } s_STATUSMSG, *ps_STATUSMSG;
    */

    ZeroMemory(pStatusData, sizeof(s_STATUSMSG));

    if ( (pIxxatChannelStatus->sLineStatus.dwStatus & CAN_STATUS_ININIT) == CAN_STATUS_ININIT)
    {
        pStatusData->wControllerStatus = 1;
    }
    else if ( (pIxxatChannelStatus->sLineStatus.bOpMode & CAN_OPMODE_LISTONLY) == CAN_STATUS_ININIT)
    {
        pStatusData->wControllerStatus = 4;
    }
    else
    {
        pStatusData->wControllerStatus = 3;
    }
}


/**
 * @brief
 *   A C function which is called to start the thread. This function
 *   casts then the parameter to the calling class object and call
 *   then the class internal structure for the thread.
 *   This is necessary to give the Windows API function _beginthread
 *   a C-function.
 *
 * @param [in,out]  pParam
 *  Must be the this pointer of the calling class. No check for
 *  valid pointer will be made!
 */
void ReceiveThread( void* pParam)
{
    CIxxatCanChannel* pCallBackClass = (CIxxatCanChannel*) pParam;
    pCallBackClass->InnerRxThreadFunc();
}
