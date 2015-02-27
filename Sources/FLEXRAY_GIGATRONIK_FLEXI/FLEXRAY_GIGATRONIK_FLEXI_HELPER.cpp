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

/**********************************************************************************************************************
 *
 *  FILENAME:           FLEXRAY_GIGATRONIK_FLEXI_HELPER.cpp
 *
 *  COPYRIGHT:          Gigatronik Ingolstadt GmbH
 *  AUTHOR, DPT:        M. Morgott, KE
 *
 *  DATE OF CREATION:   28.11.2014
 *
 *  DESCRIPTION:        Implements methods of helper class to serve DIL-class.
 *
 *********************************************************************************************************************/

/** --- CHANGE LOG ---
 *  V1.0.0.0        First reviewed version
 *
 */

/* Project includes */
#include "FLEXRAY_GIGATRONIK_FLEXI_HELPER.h"
#include "FLEXRAY_GIGATRONIK_FLEXI.h"
#include "FLEXRAY_GIGATRONIK_FLEXI_Log.h"


/**
 * Gets the flex-i API function pointer from the CLIWrapper.dll
 */
HRESULT Flexi_Helper::Get_FLEXI_API_Pointers(HMODULE& LibFLEXI, S_FLEXI_POINTER_TABLE& flexi_API)
{
    HRESULT hResult = S_OK;

    if (LibFLEXI != nullptr)
    {
        /* get functionpointer of the API-functions and store it in data structure flexi_API
         * (LibFLEXI is the handle of the 'CLIWrapper.dll')
         */
        if ((flexi_API.GtFr_EnableAsmResolver = (PF_FLEXI_GtFr_EnableAsmResolver)
                                                GetProcAddress(LibFLEXI, "GtFr_EnableAsmResolver")) == nullptr)
        {
            hResult = S_FALSE;
        }
        else if ((flexi_API.GtFr_Init = (PF_FLEXI_GtFr_Init)
                                        GetProcAddress(LibFLEXI, "GtFr_Init")) == nullptr)
        {
            hResult = S_FALSE;
        }
        else if ((flexi_API.GtFr_GetCtrlState = (PF_FLEXI_GtFr_GetCtrlState)
                                                GetProcAddress(LibFLEXI, "GtFr_GetCtrlState")) == nullptr)
        {
            hResult = S_FALSE;
        }
        else if ((flexi_API.GtFr_ReceiveEvent = (PF_FLEXI_GtFr_ReceiveEvent)
                                                GetProcAddress(LibFLEXI, "GtFr_ReceiveEvent")) == nullptr)
        {
            hResult = S_FALSE;
        }
        else if ((flexi_API.GtFr_TransmitTxLPdu = (PF_FLEXI_GtFr_TransmitTxLPdu)
                  GetProcAddress(LibFLEXI, "GtFr_TransmitTxLPdu")) == nullptr)
        {
            hResult = S_FALSE;
        }
        else if ((flexi_API.GtFr_TransmitTxLPduBlock = (PF_FLEXI_GtFr_TransmitTxLPduBlock)
                  GetProcAddress(LibFLEXI, "GtFr_TransmitTxLPduBlock")) == nullptr)
        {
            hResult = S_FALSE;
        }
        else if ((flexi_API.GtFr_GetGlobalTime = (PF_FLEXI_GtFr_GetGlobalTime)
                  GetProcAddress(LibFLEXI, "GtFr_GetGlobalTime")) == nullptr)
        {
            hResult = S_FALSE;
        }
        else if ((flexi_API.GtFr_GetCtrlRegister = (PF_FLEXI_GtFr_GetCtrlRegister)
                  GetProcAddress(LibFLEXI, "GtFr_GetCtrlRegister")) == nullptr)
        {
            hResult = S_FALSE;
        }
        else if ((flexi_API.GtFr_InitComCtrl = (PF_FLEXI_GtFr_InitComCtrl)
                                               GetProcAddress(LibFLEXI, "GtFr_InitComCtrl")) == nullptr)
        {
            hResult = S_FALSE;
        }
        else if ((flexi_API.GtFr_InitTimer = (PF_FLEXI_GtFr_InitTimer)
                                             GetProcAddress(LibFLEXI, "GtFr_InitTimer")) == nullptr)
        {
            hResult = S_FALSE;
        }
        else if ((flexi_API.GtFr_StartTimer = (PF_FLEXI_GtFr_StartTimer)
                                              GetProcAddress(LibFLEXI, "GtFr_StartTimer")) == nullptr)
        {
            hResult = S_FALSE;
        }
        else if ((flexi_API.GtFr_ExecuteCommand = (PF_FLEXI_GtFr_ExecuteCommand)
                  GetProcAddress(LibFLEXI, "GtFr_ExecuteCommand")) == nullptr)
        {
            hResult = S_FALSE;
        }
        else if ((flexi_API.GtFr_GetInterfaceCount = (PF_FLEXI_GtFr_GetInterfaceCount)
                  GetProcAddress(LibFLEXI, "GtFr_GetInterfaceCount")) == nullptr)
        {
            hResult = S_FALSE;
        }
        else if ((flexi_API.GtFr_GetInterfaceInfo = (PF_FLEXI_GtFr_GetInterfaceInfo)
                  GetProcAddress(LibFLEXI, "GtFr_GetInterfaceInfo")) == nullptr)
        {
            hResult = S_FALSE;
        }
        else if ((flexi_API.GtFr_GetInterfaceExtInfo = (PF_FLEXI_GtFr_GetInterfaceExtInfo)
                  GetProcAddress(LibFLEXI, "GtFr_GetInterfaceExtInfo")) == nullptr)
        {
            hResult = S_FALSE;
        }
        else if ((flexi_API.GtFr_ClockCommand = (PF_FLEXI_GtFr_ClockCommand)
                                                GetProcAddress(LibFLEXI, "GtFr_ClockCommand")) == nullptr)
        {
            hResult = S_FALSE;
        }
        else if ((flexi_API.GtFr_GetClockTime = (PF_FLEXI_GtFr_GetClockTime)
                                                GetProcAddress(LibFLEXI, "GtFr_GetClockTime")) == nullptr)
        {
            hResult = S_FALSE;
        }
        else if ((flexi_API.GtFr_DeInit = (PF_FLEXI_GtFr_DeInit)
                                          GetProcAddress(LibFLEXI, "GtFr_DeInit")) == nullptr)
        {
            hResult = S_FALSE;
        }
        else if ((flexi_API.GtFr_GetCtrlConfig = (PF_FLEXI_GtFr_GetCtrlConfig)
                  GetProcAddress(LibFLEXI, "GtFr_GetCtrlConfig")) == nullptr)
        {
            hResult = S_FALSE;
        }
        else if ((flexi_API.GtFr_GetProtConfig = (PF_FLEXI_GtFr_GetProtConfig)
                  GetProcAddress(LibFLEXI, "GtFr_GetProtConfig")) == nullptr)
        {
            hResult = S_FALSE;
        }
        else if ((flexi_API.GtFr_GetMsgBufConfig = (PF_FLEXI_GtFr_GetMsgBufConfig)
                  GetProcAddress(LibFLEXI, "GtFr_GetMsgBufConfig")) == nullptr)
        {
            hResult = S_FALSE;
        }
        else if ((flexi_API.GtFr_GetErayRegisterMap = (PF_FLEXI_GtFr_GetErayRegisterMap)
                  GetProcAddress(LibFLEXI, "GtFr_GetErayRegisterMap")) == nullptr)
        {
            hResult = S_FALSE;
        }
        else if ((flexi_API.GtFr_GetEventQueueInfo = (PF_FLEXI_GtFr_GetEventQueueInfo)
                  GetProcAddress(LibFLEXI, "GtFr_GetEventQueueInfo")) == nullptr)
        {
            hResult = S_FALSE;
        }
        else if ((flexi_API.GtFr_ClearEventQueue = (PF_FLEXI_GtFr_ClearEventQueue)
                  GetProcAddress(LibFLEXI, "GtFr_ClearEventQueue")) == nullptr)
        {
            hResult = S_FALSE;
        }
        else if ((flexi_API.GtFr_EnableTransceiver = (PF_FLEXI_GtFr_EnableTransceiver)
                  GetProcAddress(LibFLEXI, "GtFr_EnableTransceiver")) == nullptr)
        {
            hResult = S_FALSE;
        }
        else if ((flexi_API.GtFr_DisableTransceiver = (PF_FLEXI_GtFr_DisableTransceiver)
                  GetProcAddress(LibFLEXI, "GtFr_DisableTransceiver")) == nullptr)
        {
            hResult = S_FALSE;
        }
        else if ((flexi_API.GtFr_FlashFirmwareMode = (PF_FLEXI_GtFr_FlashFirmwareMode)
                  GetProcAddress(LibFLEXI, "GtFr_FlashFirmwareMode")) == nullptr)
        {
            hResult = S_FALSE;
        }
        else if ((flexi_API.GtFr_SetEventQueueNotification = (PF_FLEXI_GtFr_SetEventQueueNotification)
                  GetProcAddress(LibFLEXI, "GtFr_SetEventQueueNotification")) == nullptr)
        {
            hResult = S_FALSE;
        }
        else if ((flexi_API.GtFr_SetEventQueueOverflow = (PF_FLEXI_GtFr_SetEventQueueOverflow)
                  GetProcAddress(LibFLEXI, "GtFr_SetEventQueueOverflow")) == nullptr)
        {
            hResult = S_FALSE;
        }
        else if ((flexi_API.GtFr_ReconfigureMsgBuf = (PF_FLEXI_GtFr_ReconfigureMsgBuf)
                  GetProcAddress(LibFLEXI, "GtFr_ReconfigureMsgBuf")) == nullptr)
        {
            hResult = S_FALSE;
        }
        else if ((flexi_API.GtFr_GetMaxExtRxFifoFilterCount = (PF_FLEXI_GtFr_GetMaxExtRxFifoFilterCount)
                  GetProcAddress(LibFLEXI, "GtFr_GetMaxExtRxFifoFilterCount")) == nullptr)
        {
            hResult = S_FALSE;
        }
        else if ((flexi_API.GtFr_DisableExtRxFifoFilter = (PF_FLEXI_GtFr_DisableExtRxFifoFilter)
                  GetProcAddress(LibFLEXI, "GtFr_DisableExtRxFifoFilter")) == nullptr)
        {
            hResult = S_FALSE;
        }
        else if ((flexi_API.GtFr_EnableExtRxFifoFilter = (PF_FLEXI_GtFr_EnableExtRxFifoFilter)
                  GetProcAddress(LibFLEXI, "GtFr_EnableExtRxFifoFilter")) == nullptr)
        {
            hResult = S_FALSE;
        }
        else if ((flexi_API.GtFr_SetExtRxFifoFilter = (PF_FLEXI_GtFr_SetExtRxFifoFilter)
                  GetProcAddress(LibFLEXI, "GtFr_SetExtRxFifoFilter")) == nullptr)
        {
            hResult = S_FALSE;
        }

    }

    return hResult;
}


/**
 * Returns ProtocolCfg generated from the selected fibex file
 */
HRESULT Flexi_Helper::Get_ProtCfg(GtFr_ProtocolCfgType& ProtCfg, CHANNEL_CONFIG& FibexContainer)
{
    /* define variables to access fibex container information */
    HRESULT hResult = S_OK;
    ABS_FLEXRAY_CLUSTER Cluster;
    std::list<ECU_Struct> EcuList;
    ABS_FLEXRAY_SPEC_CNTLR ControllerData;
    std::list<ECU_Struct>::iterator itrEcu;

    /* get necessary cluster information from fibex container */
    FibexContainer.m_ouClusterInfo.GetClusterInfo(Cluster);

    /* copy relevant parameters in corresponding flex-i data structure */
    ProtCfg.GColdstartAttempts                  = (uint32)Cluster.m_shCOLD_START_ATTEMPTS;
    ProtCfg.GListenNoise                        = (uint32)Cluster.m_shLISTEN_NOISE;
    ProtCfg.GMacroPerCycle                      = (uint32)Cluster.m_shMACRO_PER_CYCLE;
    ProtCfg.GMaxWithoutClkCorrFatal             = (uint32)Cluster.m_shMAX_WITHOUT_CLOCK_CORRECTION_FATAL;
    ProtCfg.GMaxWithoutClkCorrPassive           = (uint32)Cluster.m_shMAX_WITHOUT_CLOCK_CORRECTION_PASSIVE;
    ProtCfg.GNetworkManagementVectorLength      = (uint32)Cluster.m_shNETWORK_MANAGEMENT_VECTOR_LENGTH;
    ProtCfg.GNumberOfMiniSlots                  = (uint32)Cluster.m_shNUMBER_OF_MINISLOTS;
    ProtCfg.GNumberOfStaticSlots                = (uint32)Cluster.m_shNUMBER_OF_STATIC_SLOTS;
    ProtCfg.GOffsetCorrectionStart              = (uint32)Cluster.m_shOFFSET_CORRECTION_START;
    ProtCfg.GPayloadLengthStatic                = (uint32)Cluster.m_shPAYLOAD_LENGTH_STATIC;
    ProtCfg.GSyncNodeMax                        = (uint32)Cluster.m_shSYNC_NODE_MAX;

    ProtCfg.GdActionPointOffset                 = (uint32)Cluster.m_shACTION_POINT_OFFSET;
    ProtCfg.GdDynamicSlotIdlePhase              = (uint32)Cluster.m_shDYNAMIC_SLOT_IDLE_PHASE;
    ProtCfg.GdMiniSlot                          = (uint32)Cluster.m_shMINISLOT;
    ProtCfg.GdMiniSlotActionPointOffset         = (uint32)Cluster.m_shMINISLOT_ACTION_POINT_OFFSET;
    ProtCfg.GdStaticSlot                        = (uint32)Cluster.m_shSTATIC_SLOT;
    ProtCfg.GdWakeupSymbolRxIdle                = (uint32)Cluster.m_ouWAKEUP.m_shWAKE_UP_SYMBOL_RX_IDLE;
    ProtCfg.GdWakeupSymbolRxLow                 = (uint32)Cluster.m_ouWAKEUP.m_shWAKE_UP_SYMBOL_RX_LOW;
    ProtCfg.GdWakeupSymbolRxWindow              = (uint32)Cluster.m_ouWAKEUP.m_shWAKE_UP_SYMBOL_RX_WINDOW;
    ProtCfg.GdWakeupSymbolTxIdle                = (uint32)Cluster.m_ouWAKEUP.m_shWAKE_UP_SYMBOL_TX_IDLE;
    ProtCfg.GdWakeupSymbolTxLow                 = (uint32)Cluster.m_ouWAKEUP.m_shWAKE_UP_SYMBOL_TX_LOW;
    ProtCfg.GdCasRxLowMax                       = (uint32)Cluster.m_shCAS_RX_LOW_MAX;
    ProtCfg.GdTssTransmitter                    = (uint32)Cluster.m_shTSS_TRANSMITTER;
    ProtCfg.GdNetworkIdleTime                   = 32u;                                                                  /* value from CLI example */

    FibexContainer.m_ouClusterInfo.GetECUList(EcuList);
    /* Take first ECU controller parameters for controller settings */
    itrEcu = EcuList.begin();
    /* check if there is at least one ECU */
    if ( itrEcu!= EcuList.end() )
    {
        /* finally get relevant data structure for protocol configuration */
        itrEcu->GetControllerParams(ControllerData);

        ProtCfg.PAllowHaltDueToClock        = (uint32)ControllerData.m_bAllowHaltDewToClock;
        ProtCfg.PAllowPassiveToActive       = (uint32)ControllerData.m_shAllowPassiveToActive;
        ProtCfg.PClusterDriftDamping        = (uint32)ControllerData.m_shClusterDriftDamping;
        ProtCfg.PDecodingCorrection         = (uint32)ControllerData.m_nDecodingCorr;
        ProtCfg.PDelayCompensationA         = (uint32)ControllerData.m_shDelayCompensationA;
        ProtCfg.PDelayCompensationB         = (uint32)ControllerData.m_shDelayCompensationB;
        ProtCfg.PExternOffsetCorrection     = (uint32)ControllerData.m_shExternOffsetCorr;
        ProtCfg.PExternRateCorrection       = (uint32)ControllerData.m_shExternRateCorr;
        ProtCfg.PKeySlotUsedForStartup      = (uint32)ControllerData.m_sKeySlotUsage.m_nStartUpSync;
        ProtCfg.PKeySlotUsedForSync         = (uint32)ControllerData.m_sKeySlotUsage.m_nSync;
        ProtCfg.PLatestTx                   = (uint32)ControllerData.m_shLatestTx;
        ProtCfg.PMicroInitialOffsetA        = (uint32)ControllerData.m_shMicroInitialOffsetA;
        ProtCfg.PMicroInitialOffsetB        = (uint32)ControllerData.m_shMicroInitialOffsetB;
        ProtCfg.PMacroInitialOffsetA        = (uint32)ControllerData.m_shMacroInitialOffsetA;
        ProtCfg.PMacroInitialOffsetB        = (uint32)ControllerData.m_shMacroInitialOffsetB;
        ProtCfg.PMicroPerCycle              = (uint32)ControllerData.m_nMicroPreCycle;
        ProtCfg.POffsetCorrectionOut        = (uint32)ControllerData.m_shOffsetCorrOut;
        ProtCfg.PRateCorrectionOut          = (uint32)ControllerData.m_shRateCorrOut;
        ProtCfg.PSingleSlotEnabled          = (uint32)ControllerData.m_bSingleSlotEnable;
        ProtCfg.PWakeupChannel              = (uint32)ControllerData.m_nWakeUpChannel;
        ProtCfg.PWakeupPattern              = (uint32)ControllerData.m_shWakeUpPattern;
        ProtCfg.PdAcceptedStartupRange      = (uint32)ControllerData.m_shAcceptedStartUpRange;
        ProtCfg.PdListenTimeout             = (uint32)ControllerData.m_nListenTimeOut;
        ProtCfg.PdMaxDrift                  = (uint32)ControllerData.m_shMaxDrift;

    }
    else
    {
        hResult = S_FALSE;
    }


    return hResult;
}



/**
 * Returns CtrlCfg generated from prepared values
 */
HRESULT Flexi_Helper::Get_CtrlCfg(GtFr_CtrlCfgType CtrlCfg[])
{
    /* define variables */
    HRESULT hResult = S_OK;

    /* set relevant parameters in corresponding flex-i data structure */
    for (uint16 controller_index = GTFR_COMMUNICATION_CONTROLLER_0; controller_index <= GTFR_COMMUNICATION_CONTROLLER_1; controller_index++)
    {
        CtrlCfg[controller_index].ChannelEnable =                GTFR_CHANNELS_BOTH;
        CtrlCfg[controller_index].BaudRate =                    GTFR_BAUDRATE_10M;
        CtrlCfg[controller_index].ColdstartNode =                GTFR_DISABLED;  /* coldstart configuration takes place at CDIL_FLEXRAY_GIGATRONIK_FLEXI::FLEXRAY_SetConfigData */
        CtrlCfg[controller_index].AutoRestart =                    GTFR_DISABLED;
        CtrlCfg[controller_index].MediaAccessTestA =            GTFR_DISABLED;
        CtrlCfg[controller_index].MediaAccessTestB =            GTFR_DISABLED;

        CtrlCfg[controller_index].InterruptConfig.errorEventEnable.protocolErrorMode =        GTFR_ENABLED;
        CtrlCfg[controller_index].InterruptConfig.errorEventEnable.syncBelowMinimum =        GTFR_ENABLED;
        CtrlCfg[controller_index].InterruptConfig.errorEventEnable.syncOverFlow =            GTFR_ENABLED;
        CtrlCfg[controller_index].InterruptConfig.errorEventEnable.clockCorrectionFail =    GTFR_ENABLED;
        CtrlCfg[controller_index].InterruptConfig.errorEventEnable.parityError =            GTFR_ENABLED;
        CtrlCfg[controller_index].InterruptConfig.errorEventEnable.fifoOverrun =            GTFR_ENABLED;
        CtrlCfg[controller_index].InterruptConfig.errorEventEnable.errorDetectA =            GTFR_ENABLED;
        CtrlCfg[controller_index].InterruptConfig.errorEventEnable.transmitViolationA =        GTFR_ENABLED;
        CtrlCfg[controller_index].InterruptConfig.errorEventEnable.boundaryA =                GTFR_ENABLED;
        CtrlCfg[controller_index].InterruptConfig.errorEventEnable.errorDetectB =            GTFR_ENABLED;
        CtrlCfg[controller_index].InterruptConfig.errorEventEnable.transmitViolationB =        GTFR_ENABLED;
        CtrlCfg[controller_index].InterruptConfig.errorEventEnable.boundaryB =                GTFR_ENABLED;

        CtrlCfg[controller_index].InterruptConfig.statusEventEnable.wakeupStatus =            GTFR_ENABLED;
        CtrlCfg[controller_index].InterruptConfig.statusEventEnable.collisionAvoidance =    GTFR_ENABLED;
        CtrlCfg[controller_index].InterruptConfig.statusEventEnable.cycleStart =            GTFR_ENABLED;
        CtrlCfg[controller_index].InterruptConfig.statusEventEnable.transmitInterrupt =        GTFR_ENABLED;
        CtrlCfg[controller_index].InterruptConfig.statusEventEnable.receiveInterrupt =        GTFR_ENABLED;
        CtrlCfg[controller_index].InterruptConfig.statusEventEnable.fifo =                    GTFR_ENABLED;
        CtrlCfg[controller_index].InterruptConfig.statusEventEnable.nmvChanged =            GTFR_ENABLED;
        CtrlCfg[controller_index].InterruptConfig.statusEventEnable.stopWatch =                GTFR_ENABLED;
        CtrlCfg[controller_index].InterruptConfig.statusEventEnable.startupCompleted =        GTFR_ENABLED;
        CtrlCfg[controller_index].InterruptConfig.statusEventEnable.dynamicSegmentStart =    GTFR_ENABLED;
        CtrlCfg[controller_index].InterruptConfig.statusEventEnable.wakeupPatternA =        GTFR_ENABLED;
        CtrlCfg[controller_index].InterruptConfig.statusEventEnable.mtsReceivedA =            GTFR_ENABLED;
        CtrlCfg[controller_index].InterruptConfig.statusEventEnable.wakeupPatternB =        GTFR_ENABLED;
        CtrlCfg[controller_index].InterruptConfig.statusEventEnable.mtsReceivedB =            GTFR_ENABLED;

    }

    return hResult;
}


/**
 * Returns MsgBufCfg generated from the selected fibex file
 */
HRESULT Flexi_Helper::Get_MsgBufCfg(GtFr_MsgBufCfgType MsgBufCfg[],
                                    const GtFr_ProtocolCfgType& ProtCfg,
                                    CHANNEL_CONFIG FibexContainer,
                                    std::map<SLOT_BASECYCLE, MSG_BUF_IDX_TYPE>& map_MsgBufIdx,
                                    std::map<SLOT_BASECYCLE, MSG_REPETITION_TYPE>& map_MsgRep,
                                    const GtFr_MsgBufTxModeType& transmission_mode_static,
                                    const GtFr_MsgBufTxModeType& transmission_mode_dynamic,
                                    const flexi_Keyslot_Settings& Keyslot_Settings,
                                    const std::string& strLogFile)
{
    /* defines variables */
    HRESULT hResult = S_OK;

    /* data structures to access fibex container information */
    SLOT_BASECYCLE nSlotBase;
    std::list<FRAME_STRUCT> ouFrameList;
    FibexContainer.GetSelectedECUTxFrames(ouFrameList);
    std::list<FRAME_STRUCT>::iterator itrTemp;

    char m_ouChannel;
    /* temporary message buffer array to store all message buffer before store them in the
     * corresponding MsgBufCfg in the right order.
     */
    GtFr_TxMsgBufCfgType txMsgBufArray[GTFR_MAX_ALLOWED_FRAME_ID][GTFR_NUMBER_SLOTS];
    /* frameIDCounter store the number of messages for corresponding frame ID */
    uint16 frameIDCounter[GTFR_MAX_ALLOWED_FRAME_ID];
    /* save actual frame ID */
    uint16 actualID;

    /* save highest payload length to set FIFO size with corresponding value */
    uint16 highest_payload_length;

    /* freeFrameID stores the frame ID for the CC1 for keyslot purpose */
    uint16 freeFrameID;
    /* freeFrameID_not_found indicates if a free frame ID get found and end the while-loop */
    bool freeFrameID_not_found;
    /* freeFrameID_not_found indicates if the current examined frame id is used by the CC0 */
    bool freeFrameID_occupied;
    /* keyslot_frame_found indicates if a free frame ID get found and end the for-loop */
    bool keyslot_frame_found;

    /* reset maps */
    map_MsgBufIdx.clear();
    map_MsgRep.clear();

    /* save highest payload length to set FIFO size with corresponding value */
    highest_payload_length = (uint16) ProtCfg.GPayloadLengthStatic;

    /* 1. - CC0, static MsgBuf */
    /* init Array, which is responsible to store number of messages for corresponding frame ID */
    for(uint16 i = 0; i < GTFR_MAX_ALLOWED_FRAME_ID; i++)
    {
        /* init array with 0 messages for every frame ID */
        frameIDCounter[i] = 0;
    }

    /* 1.1 - At first save all static message buffer in an array unsorted */
    for (itrTemp = ouFrameList.begin(); itrTemp != ouFrameList.end(); ++itrTemp)
    {
        /* check if the slot ID is valid */
        if (itrTemp->m_nSlotId >= GTFR_MAX_ALLOWED_FRAME_ID)
        {
            flexi_Log_String(strLogFile, "WARNING: Certain slot ID of the fibex container is not in the provided range (slot ID >= 1024).");
        }
        /* check if the basecyle - repetition relation is valid */
        else if (itrTemp->m_nBaseCycle >= itrTemp->m_nReptition)
        {
            flexi_Log_String(strLogFile, "WARNING: Incorrect basecyle - repetition relation got from the fibex container.");
        }
        /* check if the message belongs to the static segment */
        else if(itrTemp->m_eSlotType == STATIC)
        {
            /* save actual frame ID */
            actualID = itrTemp->m_nSlotId;

            /* copy relevant data to the temporary message buffer array */
            if(itrTemp->m_ouChannel == 'A')
            {
                txMsgBufArray[actualID][frameIDCounter[actualID]].ChannelAEnable          = GTFR_ENABLED;
                txMsgBufArray[actualID][frameIDCounter[actualID]].ChannelBEnable          = GTFR_DISABLED;
            }
            else if(itrTemp->m_ouChannel == 'B')
            {
                txMsgBufArray[actualID][frameIDCounter[actualID]].ChannelAEnable          = GTFR_DISABLED;
                txMsgBufArray[actualID][frameIDCounter[actualID]].ChannelBEnable          = GTFR_ENABLED;
            }
            else if (itrTemp->m_ouChannel == 'X')
            {
                txMsgBufArray[actualID][frameIDCounter[actualID]].ChannelAEnable          = GTFR_ENABLED;
                txMsgBufArray[actualID][frameIDCounter[actualID]].ChannelBEnable          = GTFR_ENABLED;
            }
            else
            {
                flexi_Log_String(strLogFile, "WARNING: Incorrect channel selection for CC0 from the fibex container.");
            }


            txMsgBufArray[actualID][frameIDCounter[actualID]].BaseCycle               = itrTemp->m_nBaseCycle;
            txMsgBufArray[actualID][frameIDCounter[actualID]].FrameId                 = itrTemp->m_nSlotId;
            txMsgBufArray[actualID][frameIDCounter[actualID]].HeaderCrc               = 0u;
            txMsgBufArray[actualID][frameIDCounter[actualID]].MsgBufInterruptEnable   = GTFR_ENABLED;
            txMsgBufArray[actualID][frameIDCounter[actualID]].PayloadLength           = (uint16) ProtCfg.GPayloadLengthStatic;       //itrTemp->m_nLength (m_nLength indicates the length in bytes, ProtCfg.GPayloadLengthStatic indicates the length in words) !!
            txMsgBufArray[actualID][frameIDCounter[actualID]].PayloadPreamble         = GTFR_DISABLED;
            txMsgBufArray[actualID][frameIDCounter[actualID]].Repetition              = itrTemp->m_nReptition;
            txMsgBufArray[actualID][frameIDCounter[actualID]].TransmissionMode        = transmission_mode_static;

            /* increment counter of corresponding frame ID */
            frameIDCounter[actualID]++;
        }
    }

    /* 1.2 - If there should be an manual keyslot for CC0, create an additional message buffer */
    /* check if the keyslot id is valid */
    if (Keyslot_Settings.CC0_Keyslot >= GTFR_MAX_ALLOWED_FRAME_ID)
    {
        flexi_Log_String(strLogFile, "WARNING: Manually selected keyslot of communication controller 0 is not in the provided range.");
    }
    else if(Keyslot_Settings.CC0_Keyslot_manual == true)
    {
        /* check if Frame ID is not already used by the CC0 */
        if(frameIDCounter[Keyslot_Settings.CC0_Keyslot] == 0)
        {
            GtFr_TxMsgBufCfgType tempMsgBuf =
                /* idx, slot id,                       pll,                                    ppi,  hcrc, ien, chA, chB, base, rep.,  transmission mode, */
            {   0u, Keyslot_Settings.CC0_Keyslot,  (uint16) ProtCfg.GPayloadLengthStatic,  0u, 0x081u,  0u, 1u, 1u, 0x00u, 0x01u, transmission_mode_static };
            /* - idx(MsgBufNr) is not relevant in this case, because it will be overwritten afterwards
             * - the keyslot frame have to be basecycle 0 and repetition 1
             * - default setting is that the keyslot is associated with both channels
             * - transmission mode is used by the custom defined settings
             */

            /* copy message buffer to the temporary message buffer array */
            txMsgBufArray[Keyslot_Settings.CC0_Keyslot][0] = tempMsgBuf;
            /* increment counter of corresponding frame ID */
            frameIDCounter[Keyslot_Settings.CC0_Keyslot]++;
        }
        else
        {
            /* log warning, if the chosen frame id is already used */
            flexi_Log_String(strLogFile, "WARNING: Manually selected keyslot could not be used for communication controller 0.");
        }
    }

    /**
     * 1.3
     * If the CC0 should represent a coldstart node, the first MsgBuf with its corresponding
     * Frame ID will be used as startup/sync frame.
     * The flexi allow only one MsgBuf with the startup/sync frame. Therefore other possible
     * MsgBuf with the same frame ID get erased.
     */
    if(Keyslot_Settings.CC0_Keyslot_automatic == true)
    {
        keyslot_frame_found = false;

        /* run through Frame IDs till free keyslot will be found */
        for(uint16 i = 0; i < GTFR_MAX_ALLOWED_FRAME_ID && keyslot_frame_found == false; i++)
        {
            /* check if frame id is already used */
            if(frameIDCounter[i] != 0)
            {
                /* set counter and set boolean keyslot_frame_found to quit the for-loop */
                frameIDCounter[i] = 1;
                keyslot_frame_found = true;

                /* log info */
                std::stringstream ss;
                ss << "INFO: Use keyslot frame ID " << i << " for communication controller 0.";
                flexi_Log_String(strLogFile, ss.str().c_str());
            }
        }
        if(keyslot_frame_found == false)
        {
            /* log warning, if there is no free keyslot */
            flexi_Log_String(strLogFile, "WARNING: No free keyslot found for communication controller 0.");
        }
    }

    /* 1.4 - copy all txMsgBuf in the right order in the MsgBufCfg */
    MsgBufCfg[0].sMsgBuf.sMsgBufCount = 0;
    /* run through Frame IDs */
    for(uint16 i = 0; i < GTFR_MAX_ALLOWED_FRAME_ID; i++)
    {
        /* run through corresponding Frame ID counter, and check if the number of static message buffer exceeds the acceptable number */
        for(uint16 j = 0; j < frameIDCounter[i] && MsgBufCfg[0].sMsgBuf.sMsgBufCount < GTFR_ERAY_MAX_MSGBUF_COUNT; j++)
        {
            /* copy relevant data to the Message Buffer Configuration */
            MsgBufCfg[GTFR_COMMUNICATION_CONTROLLER_0].sMsgBuf.sMsgBufDat[MsgBufCfg[GTFR_COMMUNICATION_CONTROLLER_0].sMsgBuf.sMsgBufCount].tag                  = GTFR_MSGBUF_TX;
            MsgBufCfg[GTFR_COMMUNICATION_CONTROLLER_0].sMsgBuf.sMsgBufDat[MsgBufCfg[GTFR_COMMUNICATION_CONTROLLER_0].sMsgBuf.sMsgBufCount].txMsgBuf             = txMsgBufArray[i][j];
            MsgBufCfg[GTFR_COMMUNICATION_CONTROLLER_0].sMsgBuf.sMsgBufDat[MsgBufCfg[GTFR_COMMUNICATION_CONTROLLER_0].sMsgBuf.sMsgBufCount].txMsgBuf.MsgBufNr    = (uint16) MsgBufCfg[GTFR_COMMUNICATION_CONTROLLER_0].sMsgBuf.sMsgBufCount;

            /* check the right channel configuration */
            if(MsgBufCfg[GTFR_COMMUNICATION_CONTROLLER_0].sMsgBuf.sMsgBufDat[MsgBufCfg[GTFR_COMMUNICATION_CONTROLLER_0].sMsgBuf.sMsgBufCount].txMsgBuf.ChannelAEnable == GTFR_ENABLED
                    && MsgBufCfg[GTFR_COMMUNICATION_CONTROLLER_0].sMsgBuf.sMsgBufDat[MsgBufCfg[GTFR_COMMUNICATION_CONTROLLER_0].sMsgBuf.sMsgBufCount].txMsgBuf.ChannelBEnable == GTFR_DISABLED)
            {
                m_ouChannel = 'A';
            }
            else if(MsgBufCfg[GTFR_COMMUNICATION_CONTROLLER_0].sMsgBuf.sMsgBufDat[MsgBufCfg[GTFR_COMMUNICATION_CONTROLLER_0].sMsgBuf.sMsgBufCount].txMsgBuf.ChannelAEnable == GTFR_DISABLED
                    && MsgBufCfg[GTFR_COMMUNICATION_CONTROLLER_0].sMsgBuf.sMsgBufDat[MsgBufCfg[GTFR_COMMUNICATION_CONTROLLER_0].sMsgBuf.sMsgBufCount].txMsgBuf.ChannelBEnable == GTFR_ENABLED)
            {
                m_ouChannel = 'B';
            }
            else if (MsgBufCfg[GTFR_COMMUNICATION_CONTROLLER_0].sMsgBuf.sMsgBufDat[MsgBufCfg[GTFR_COMMUNICATION_CONTROLLER_0].sMsgBuf.sMsgBufCount].txMsgBuf.ChannelAEnable == GTFR_ENABLED
                     && MsgBufCfg[GTFR_COMMUNICATION_CONTROLLER_0].sMsgBuf.sMsgBufDat[MsgBufCfg[GTFR_COMMUNICATION_CONTROLLER_0].sMsgBuf.sMsgBufCount].txMsgBuf.ChannelBEnable == GTFR_ENABLED)
            {
                m_ouChannel = 'X';
            }
            else
            {
                flexi_Log_String(strLogFile, "WARNING: Incorrect channel selection of a static message of CC0.");
            }

            /* save relation slot/base : MsgBufIdx and slot/bas : repetition for later sending purpose */
            nSlotBase = MAKELONG( MsgBufCfg[GTFR_COMMUNICATION_CONTROLLER_0].sMsgBuf.sMsgBufDat[MsgBufCfg[GTFR_COMMUNICATION_CONTROLLER_0].sMsgBuf.sMsgBufCount].txMsgBuf.FrameId, MAKEWORD(MsgBufCfg[GTFR_COMMUNICATION_CONTROLLER_0].sMsgBuf.sMsgBufDat[MsgBufCfg[GTFR_COMMUNICATION_CONTROLLER_0].sMsgBuf.sMsgBufCount].txMsgBuf.BaseCycle, (char) m_ouChannel) );
            map_MsgBufIdx.insert(std::pair<SLOT_BASECYCLE, MSG_BUF_IDX_TYPE>(nSlotBase, MsgBufCfg[GTFR_COMMUNICATION_CONTROLLER_0].sMsgBuf.sMsgBufCount));
            map_MsgRep.insert(std::pair<SLOT_BASECYCLE, MSG_REPETITION_TYPE>(nSlotBase, MsgBufCfg[GTFR_COMMUNICATION_CONTROLLER_0].sMsgBuf.sMsgBufDat[MsgBufCfg[GTFR_COMMUNICATION_CONTROLLER_0].sMsgBuf.sMsgBufCount].txMsgBuf.Repetition));

            /* increment counter */
            MsgBufCfg[GTFR_COMMUNICATION_CONTROLLER_0].sMsgBuf.sMsgBufCount++;
        }
    }

    /* 2. - CC1, static MsgBuf */
    /*
     * CC1, static MsgBuf currently just used as silent coldstarter: 1 static MsgBuf for coldstart, if modified as coldstart:
     * depending on the keyslot setting, create automatic coldstart frame, coldstart frame with manual frame id or no coldstart frame at all
     */
    MsgBufCfg[GTFR_COMMUNICATION_CONTROLLER_1].sMsgBuf.sMsgBufCount     = 0;
    if(Keyslot_Settings.CC1_Keyslot_automatic == true)
    {
        /* init frame id with 1 to start searching with the first frame id and look for a free keyslot id */
        freeFrameID = 1;
        freeFrameID_not_found = true;

        /* look for unused frame ID */
        while(freeFrameID_not_found)
        {
            freeFrameID_occupied = false;


            /* run through all message buffer of the CC0 to find a free keyslot, quit the for-loop if the currently freeFrameID is already used */
            for(uint16 i = 0 ; i < MsgBufCfg[GTFR_COMMUNICATION_CONTROLLER_0].sMsgBuf.sMsgBufCount && !freeFrameID_occupied; i++)
            {
                if(freeFrameID == MsgBufCfg[GTFR_COMMUNICATION_CONTROLLER_0].sMsgBuf.sMsgBufDat[i].txMsgBuf.FrameId)
                {
                    freeFrameID++;
                    freeFrameID_occupied = true;
                }
            }
            /* if the currently freeFrameID is not already used, set the boolean freeFrameID_not_found to false to quit the while-loop */
            if(freeFrameID_occupied == false)
            {
                /* there is a free Frame ID! */
                freeFrameID_not_found = false;
            }
        }

        /* check if there is no free Slot for coldstart */
        if(freeFrameID > ProtCfg.GNumberOfStaticSlots)
        {
            flexi_Log_String(strLogFile, "WARNING: No free keyslot found for communication controller 1.");
        }
        else
        {
            /* Log information */
            std::stringstream ss;
            ss << "INFO: Use keyslot frame ID " << freeFrameID << " for communication controller 1.";
            flexi_Log_String(strLogFile, ss.str().c_str());

            /* create message buffer with unused frame ID */
            MsgBufCfg[GTFR_COMMUNICATION_CONTROLLER_1].sMsgBuf.sMsgBufCount     = 1;

            GtFr_MsgBufType tempMsgBuf =
            {
                /*    tag */
                GTFR_MSGBUF_TX,
                /* idx, slot id,      pll,                                  ppi,  hcrc, ien, chA, chB, base, rep.,  transmission mode, */
                { 0u, freeFrameID, (uint16)ProtCfg.GPayloadLengthStatic,    0u, 0x081u, 0u, 1u, 1u, 0x00u, 0x01u, transmission_mode_static }
                /* - idx(MsgBufNr) is 0, because CC1 has only one MsgBuf
                * - the keyslot frame have to be basecycle 0 and repetition 1
                * - default setting is that the keyslot is associated with both channels
                * - transmission mode is used by the custom defined settings
                */
            };

            /* save keyslot message buffer in the message buffer configuration */
            MsgBufCfg[GTFR_COMMUNICATION_CONTROLLER_1].sMsgBuf.sMsgBufDat[0] = tempMsgBuf;
        }
    }
    /* create message buffer with manual frame ID if the frame ID is not already used */
    else if(Keyslot_Settings.CC1_Keyslot_manual == true && Keyslot_Settings.CC1_Keyslot < GTFR_MAX_ALLOWED_FRAME_ID)
    {
        /* check if selected frame ID is already used by the CC0 */
        if(frameIDCounter[Keyslot_Settings.CC1_Keyslot] == 0)
        {
            /* create coldstart message buffer and store it in the MsgBufConfig */
            GtFr_MsgBufType tempMsgBuf =
            {
                /*    tag */
                GTFR_MSGBUF_TX,
                /* idx, slot id,                       pll,                                 ppi,  hcrc, ien, chA, chB, base, rep.,  transmission mode, */
                { 0u, Keyslot_Settings.CC1_Keyslot, (uint16)ProtCfg.GPayloadLengthStatic, 0u, 0x081u, 0u, 1u, 1u, 0x00u, 0x01u, transmission_mode_static }
                /* - idx(MsgBufNr) is 0, because CC1 has only one MsgBuf
                * - the keyslot frame have to be basecycle 0 and repetition 1
                * - default setting is that the keyslot is associated with both channels
                * - transmission mode is used by the custom defined settings
                */
            };
            MsgBufCfg[GTFR_COMMUNICATION_CONTROLLER_1].sMsgBuf.sMsgBufDat[0] = tempMsgBuf;
            MsgBufCfg[GTFR_COMMUNICATION_CONTROLLER_1].sMsgBuf.sMsgBufCount     = 1;
        }
        else
        {
            /* log warning */
            flexi_Log_String(strLogFile, "WARNING: Manually selected keyslot could not be used for communication controller 1.");
        }
    }


    /* 3. - CC0, dynamic MsgBuf   */
    /* init Array, which is responsible to store number of messages for corresponding frame ID */
    for(uint16 i = 0; i < GTFR_MAX_ALLOWED_FRAME_ID; i++)
    {
        /* init array with 0 messages for every frame ID */
        frameIDCounter[i] = 0;
    }

    /* 3.1 - At first save all dynamic message buffer in an array unsorted */
    for (itrTemp = ouFrameList.begin(); itrTemp != ouFrameList.end(); ++itrTemp)
    {

        /* check if the slot ID is valid */
        if (itrTemp->m_nSlotId >= GTFR_MAX_ALLOWED_FRAME_ID)
        {
            flexi_Log_String(strLogFile, "WARNING: Certain slot ID of the fibex container is not in the provided range (slot ID >= 1024).");
        }
        /* check if the basecyle - repetition relation is valid */
        else if (itrTemp->m_nBaseCycle >= itrTemp->m_nReptition)
        {
            flexi_Log_String(strLogFile, "WARNING: Incorrect basecyle - repetition relation got from the fibex container.");
        }
        /* check if the message belongs to the dynamic segment */
        else if (itrTemp->m_eSlotType == DYNAMIC)
        {
            /* save actual frame ID */
            actualID = itrTemp->m_nSlotId;

            /* copy relevant data to the temporary message buffer array */
            if(itrTemp->m_ouChannel == 'A')
            {
                txMsgBufArray[actualID][frameIDCounter[actualID]].ChannelAEnable          = GTFR_ENABLED;
                txMsgBufArray[actualID][frameIDCounter[actualID]].ChannelBEnable          = GTFR_DISABLED;
            }
            else if(itrTemp->m_ouChannel == 'B')
            {
                txMsgBufArray[actualID][frameIDCounter[actualID]].ChannelAEnable          = GTFR_DISABLED;
                txMsgBufArray[actualID][frameIDCounter[actualID]].ChannelBEnable          = GTFR_ENABLED;
            }
            else
            {
                txMsgBufArray[actualID][frameIDCounter[actualID]].ChannelAEnable          = GTFR_ENABLED;
                txMsgBufArray[actualID][frameIDCounter[actualID]].ChannelBEnable          = GTFR_ENABLED;
            }

            txMsgBufArray[actualID][frameIDCounter[actualID]].BaseCycle               = itrTemp->m_nBaseCycle;
            txMsgBufArray[actualID][frameIDCounter[actualID]].FrameId                 = itrTemp->m_nSlotId;
            txMsgBufArray[actualID][frameIDCounter[actualID]].HeaderCrc               = 0u;
            txMsgBufArray[actualID][frameIDCounter[actualID]].MsgBufInterruptEnable   = GTFR_ENABLED;
            txMsgBufArray[actualID][frameIDCounter[actualID]].PayloadLength           = (itrTemp->m_nLength)/2;       //itrTemp->m_nLength (m_nLength indicates the length in bytes, txMsgBufArray[..].PayloadLength needs the length in words) !!
            txMsgBufArray[actualID][frameIDCounter[actualID]].PayloadPreamble         = GTFR_DISABLED;
            txMsgBufArray[actualID][frameIDCounter[actualID]].Repetition              = itrTemp->m_nReptition;
            txMsgBufArray[actualID][frameIDCounter[actualID]].TransmissionMode        = transmission_mode_dynamic;                            // keine unterscheidung zwischen continous und single?

            /* increment counter of corresponding frame ID */
            frameIDCounter[actualID]++;

        }
    }

    /* 3.2 - copy all txMsgBuf in the right order in the MsgBufCfg */
    MsgBufCfg[0].dMsgBuf.dMsgBufCount = 0;
    for(uint16 i = 0; i < GTFR_MAX_ALLOWED_FRAME_ID; i++)
    {
        for(uint16 j = 0; j < frameIDCounter[i] && MsgBufCfg[GTFR_COMMUNICATION_CONTROLLER_0].dMsgBuf.dMsgBufCount < GTFR_ERAY_MAX_MSGBUF_COUNT; j++)
        {
            /* copy relevant data */
            MsgBufCfg[GTFR_COMMUNICATION_CONTROLLER_0].dMsgBuf.dMsgBufDat[MsgBufCfg[GTFR_COMMUNICATION_CONTROLLER_0].dMsgBuf.dMsgBufCount].tag                  = GTFR_MSGBUF_TX;
            MsgBufCfg[GTFR_COMMUNICATION_CONTROLLER_0].dMsgBuf.dMsgBufDat[MsgBufCfg[GTFR_COMMUNICATION_CONTROLLER_0].dMsgBuf.dMsgBufCount].txMsgBuf             = txMsgBufArray[i][j];
            MsgBufCfg[GTFR_COMMUNICATION_CONTROLLER_0].dMsgBuf.dMsgBufDat[MsgBufCfg[GTFR_COMMUNICATION_CONTROLLER_0].dMsgBuf.dMsgBufCount].txMsgBuf.MsgBufNr    = (uint16) (MsgBufCfg[GTFR_COMMUNICATION_CONTROLLER_0].dMsgBuf.dMsgBufCount + MsgBufCfg[GTFR_COMMUNICATION_CONTROLLER_0].sMsgBuf.sMsgBufCount);

            if(MsgBufCfg[GTFR_COMMUNICATION_CONTROLLER_0].dMsgBuf.dMsgBufDat[MsgBufCfg[GTFR_COMMUNICATION_CONTROLLER_0].dMsgBuf.dMsgBufCount].txMsgBuf.ChannelAEnable == GTFR_ENABLED
                    && MsgBufCfg[GTFR_COMMUNICATION_CONTROLLER_0].dMsgBuf.dMsgBufDat[MsgBufCfg[GTFR_COMMUNICATION_CONTROLLER_0].dMsgBuf.dMsgBufCount].txMsgBuf.ChannelBEnable == GTFR_DISABLED)
            {
                m_ouChannel = 'A';
            }
            else if(MsgBufCfg[GTFR_COMMUNICATION_CONTROLLER_0].dMsgBuf.dMsgBufDat[MsgBufCfg[GTFR_COMMUNICATION_CONTROLLER_0].dMsgBuf.dMsgBufCount].txMsgBuf.ChannelAEnable == GTFR_DISABLED
                    && MsgBufCfg[GTFR_COMMUNICATION_CONTROLLER_0].dMsgBuf.dMsgBufDat[MsgBufCfg[GTFR_COMMUNICATION_CONTROLLER_0].dMsgBuf.dMsgBufCount].txMsgBuf.ChannelBEnable == GTFR_ENABLED)
            {
                m_ouChannel = 'B';
            }
            else if (MsgBufCfg[GTFR_COMMUNICATION_CONTROLLER_0].dMsgBuf.dMsgBufDat[MsgBufCfg[GTFR_COMMUNICATION_CONTROLLER_0].dMsgBuf.dMsgBufCount].txMsgBuf.ChannelAEnable == GTFR_ENABLED
                     && MsgBufCfg[GTFR_COMMUNICATION_CONTROLLER_0].dMsgBuf.dMsgBufDat[MsgBufCfg[GTFR_COMMUNICATION_CONTROLLER_0].dMsgBuf.dMsgBufCount].txMsgBuf.ChannelBEnable == GTFR_ENABLED)
            {
                m_ouChannel = 'X';
            }
            else
            {
                flexi_Log_String(strLogFile, "WARNING: Incorrect channel selection of a dynamic message of CC0.");
            }

            /* save relation slot/base : MsgBufIdx */
            nSlotBase = MAKELONG( MsgBufCfg[GTFR_COMMUNICATION_CONTROLLER_0].dMsgBuf.dMsgBufDat[MsgBufCfg[GTFR_COMMUNICATION_CONTROLLER_0].dMsgBuf.dMsgBufCount].txMsgBuf.FrameId, MAKEWORD(MsgBufCfg[GTFR_COMMUNICATION_CONTROLLER_0].dMsgBuf.dMsgBufDat[MsgBufCfg[GTFR_COMMUNICATION_CONTROLLER_0].dMsgBuf.dMsgBufCount].txMsgBuf.BaseCycle, (char) m_ouChannel) );
            map_MsgBufIdx.insert(std::pair<SLOT_BASECYCLE, MSG_BUF_IDX_TYPE>(nSlotBase, MsgBufCfg[GTFR_COMMUNICATION_CONTROLLER_0].dMsgBuf.dMsgBufCount + MsgBufCfg[GTFR_COMMUNICATION_CONTROLLER_0].sMsgBuf.sMsgBufCount));
            map_MsgRep.insert(std::pair<SLOT_BASECYCLE, MSG_REPETITION_TYPE>(nSlotBase, MsgBufCfg[GTFR_COMMUNICATION_CONTROLLER_0].dMsgBuf.dMsgBufDat[MsgBufCfg[GTFR_COMMUNICATION_CONTROLLER_0].dMsgBuf.dMsgBufCount].txMsgBuf.Repetition));

            /* check if there is a greater payload length */
            if(highest_payload_length < MsgBufCfg[GTFR_COMMUNICATION_CONTROLLER_0].dMsgBuf.dMsgBufDat[MsgBufCfg[GTFR_COMMUNICATION_CONTROLLER_0].dMsgBuf.dMsgBufCount].txMsgBuf.PayloadLength)
            {
                highest_payload_length = MsgBufCfg[GTFR_COMMUNICATION_CONTROLLER_0].dMsgBuf.dMsgBufDat[MsgBufCfg[GTFR_COMMUNICATION_CONTROLLER_0].dMsgBuf.dMsgBufCount].txMsgBuf.PayloadLength;
            }

            /* increment message buffer counter of the dynamic segment */
            MsgBufCfg[GTFR_COMMUNICATION_CONTROLLER_0].dMsgBuf.dMsgBufCount++;
        }
    }

    /* 4. - CC1, dynamic MsgBuf; currently just used as silent coldstarter (if modified as coldstart) */
    MsgBufCfg[1].dMsgBuf.dMsgBufCount     = 0;


    /* 5. - FIFO (+ sec + splm) settings, CC0 */
    MsgBufCfg[GTFR_COMMUNICATION_CONTROLLER_0].sec =    GTFR_MSGBUF_SEC_UNLOCK_ALL;
    MsgBufCfg[GTFR_COMMUNICATION_CONTROLLER_0].splm =    GTFR_DISABLED;
    /* following options are taken from the CLI demo */
    MsgBufCfg[GTFR_COMMUNICATION_CONTROLLER_0].fifoMsgBuf.criticalLevel =                  GTFR_FIFO_CC0_CRITICAL_LEVEL;
    MsgBufCfg[GTFR_COMMUNICATION_CONTROLLER_0].fifoMsgBuf.depth =                          GTFR_FIFO_CC0_DEPTH;
    MsgBufCfg[GTFR_COMMUNICATION_CONTROLLER_0].fifoMsgBuf.entrySize =                     highest_payload_length;    /* entrySize in words */
    /* currently the rejection filter are not used*/
    MsgBufCfg[GTFR_COMMUNICATION_CONTROLLER_0].fifoMsgBuf.rejectionFilterChannel =        GTFR_CHANNELS_NONE;
    MsgBufCfg[GTFR_COMMUNICATION_CONTROLLER_0].fifoMsgBuf.rejectionFilterMask =           0u;
    MsgBufCfg[GTFR_COMMUNICATION_CONTROLLER_0].fifoMsgBuf.rejectionFilterBaseCycle =      0u;
    MsgBufCfg[GTFR_COMMUNICATION_CONTROLLER_0].fifoMsgBuf.rejectionFilterRepetition =      0u;
    MsgBufCfg[GTFR_COMMUNICATION_CONTROLLER_0].fifoMsgBuf.rejectionFilterValue =          0u;
    /* allow receiving null frames */
    MsgBufCfg[GTFR_COMMUNICATION_CONTROLLER_0].fifoMsgBuf.rejectNullFrames =              GTFR_DISABLED;
    /* allow receiving static frames */
    MsgBufCfg[GTFR_COMMUNICATION_CONTROLLER_0].fifoMsgBuf.rejectStaticSegment =           GTFR_DISABLED;


    /* 6. - FIFO (+ sec + splm) settings, CC1 */
    MsgBufCfg[GTFR_COMMUNICATION_CONTROLLER_1].sec =    GTFR_MSGBUF_SEC_UNLOCK_ALL;
    MsgBufCfg[GTFR_COMMUNICATION_CONTROLLER_1].splm =    GTFR_DISABLED;
    /* CC1 do not use FIFO, therefore it is only important to set the depth to 0 to deactivate fifo */
    MsgBufCfg[GTFR_COMMUNICATION_CONTROLLER_1].fifoMsgBuf.criticalLevel =                  GTFR_FIFO_CC1_CRITICAL_LEVEL;
    MsgBufCfg[GTFR_COMMUNICATION_CONTROLLER_1].fifoMsgBuf.depth =                          GTFR_FIFO_CC1_DEPTH;                        /* depth with 0 deactivates fifo */
    MsgBufCfg[GTFR_COMMUNICATION_CONTROLLER_1].fifoMsgBuf.entrySize =                     highest_payload_length;    /* entrySize in words */
    MsgBufCfg[GTFR_COMMUNICATION_CONTROLLER_1].fifoMsgBuf.rejectionFilterChannel =        GTFR_CHANNELS_NONE;
    MsgBufCfg[GTFR_COMMUNICATION_CONTROLLER_1].fifoMsgBuf.rejectionFilterMask =           0u;
    MsgBufCfg[GTFR_COMMUNICATION_CONTROLLER_1].fifoMsgBuf.rejectionFilterBaseCycle =      0u;
    MsgBufCfg[GTFR_COMMUNICATION_CONTROLLER_1].fifoMsgBuf.rejectionFilterRepetition =      0u;
    MsgBufCfg[GTFR_COMMUNICATION_CONTROLLER_1].fifoMsgBuf.rejectionFilterValue =          0u;
    MsgBufCfg[GTFR_COMMUNICATION_CONTROLLER_1].fifoMsgBuf.rejectNullFrames =              GTFR_DISABLED;
    MsgBufCfg[GTFR_COMMUNICATION_CONTROLLER_1].fifoMsgBuf.rejectStaticSegment =           GTFR_DISABLED;


    return hResult;
}






/**
 * This function will be raised if an queue overflow occurs.
 * Overflow source may be hardware (firmware) or API.
 */
void Flexi_Helper::callback_EventQueueOverflow(GtFr_QueueOverflowArgsType overFlowArgs)
{
    /* Log Message */
    std::stringstream ss;
    if(overFlowArgs.ofSrc == GTFR_QUEUE_OVERFLOW_SOURCE_HARDWARE)
    {
        ss << "WARNING: Queue overflow occured; source: hardware";
    }
    else
    {
        ss << "WARNING: Queue overflow occured; source: api";
    }
    flexi_Log_String(g_pouDIL_FLEXRAY_GIGATRONIK_FLEXI->strLogFile, ss.str().c_str());

}


/**
 *  - callback function to process the events from the flex-i.
 *  - The function will be called if the the number of events reached the
 *      defined EVENTQUEUE_THRESHOLD.
 *  - The callback function will be set by the function GtFr_SetEventQueueNotification().
 */
void Flexi_Helper::callback_EventQueue(GtFr_EventQueueStatusType EventQueueStatus, GtFr_EventType* events)
{
    /* define data struct to store the relevant event data */
    s_FLXMSG FlexData;

    /* check all events of the queue */
    for(uint16 i = 0; i < EventQueueStatus.elementCount; i++)
    {
        /* check if the current event is a received frame */
        if(events[i].eventTag == GTFR_RX_FRAME
                &&
                (
                    (   /* check if event represents a null frame and check if null frames should be shown */
                        (!((events[i].eventData.rxFrame.headerFlags & GTFR_HEADERFLAG_NULL) > 0) && (g_pouDIL_FLEXRAY_GIGATRONIK_FLEXI->m_flexi_Settings.showNullFrames == true))
                    )
                    ||
                    (   /* check if event represents a sync or startup frame and check if sync/startup frames should be shown */
                        ((((events[i].eventData.rxFrame.headerFlags & GTFR_HEADERFLAG_STARTUP) > 0) ||
                          ((events[i].eventData.rxFrame.headerFlags & GTFR_HEADERFLAG_SYNC) > 0)) &&
                         (g_pouDIL_FLEXRAY_GIGATRONIK_FLEXI->m_flexi_Settings.showSyncStartupFrames == true))
                    )
                    ||
                    (   /* check if event represents a data frame */
                        ((events[i].eventData.rxFrame.headerFlags & GTFR_HEADERFLAG_NULL) > 0)
                    )
                )
          )
        {
            /* store the relevant event data */
            FlexData.m_eMessageType = FLXMSGTYPE_DATA;
            /* divide timestamp by 100 to get right time dimension */
            FlexData.m_unTimestamp = (LONGLONG)events[i].timeStamp / 100;
            FlexData.stcDataMsg.m_bIsRxMsg = true;
            FlexData.stcDataMsg.m_nSlotID = events[i].eventData.rxFrame.slotId;
            FlexData.stcDataMsg.m_ucCycleNumber = events[i].eventData.rxFrame.cycleCount;
            FlexData.stcDataMsg.m_nDLC = events[i].eventData.rxFrame.payloadLength;         // payloadLength in bytes
            memcpy(FlexData.stcDataMsg.m_ucData, events[i].eventData.rxFrame.payload, sizeof(events[i].eventData.rxFrame.payload));

            /* reset header info flags*/
            FlexData.stcDataMsg.m_lHeaderInfoFlags = 0;
            /* check the header bits */
            /* check if the startup frame indicator is set */
            if ((events[i].eventData.rxFrame.headerFlags & GTFR_HEADERFLAG_STARTUP) > 0)
            {
                FlexData.stcDataMsg.m_lHeaderInfoFlags |= RBIN_FLXHDRINFO_STARTUPFRAME;
            }
            /* check if the synch frame indicator is set */
            if ((events[i].eventData.rxFrame.headerFlags & GTFR_HEADERFLAG_SYNC) > 0)
            {
                FlexData.stcDataMsg.m_lHeaderInfoFlags |= RBIN_FLXHDRINFO_SYNCFRAME;
            }
            /* check if the null frame indicator is set */
            if (!((events[i].eventData.rxFrame.headerFlags & GTFR_HEADERFLAG_NULL) > 0))
            {
                FlexData.stcDataMsg.m_lHeaderInfoFlags |= RBIN_FLXHDRINFO_NULLFRAME;
            }
            /* check if the Payload Preamble Indicator is set */
            if ((events[i].eventData.rxFrame.headerFlags & GTFR_HEADERFLAG_PAYLOAD_PREAMBLE) > 0)
            {
                FlexData.stcDataMsg.m_lHeaderInfoFlags |= RBIN_FLXHDRINFO_PAYLOADPREAMBLE;
            }

            /* check used channel */
            if(events[i].channel == GTFR_CHANNELS_A)
            {
                FlexData.stcDataMsg.m_eChannel = CHANNEL_A;
            }
            else if(events[i].channel == GTFR_CHANNELS_B)
            {
                FlexData.stcDataMsg.m_eChannel = CHANNEL_B;
            }
            else if(events[i].channel == GTFR_CHANNELS_BOTH)
            {
                FlexData.stcDataMsg.m_eChannel = CHANNEL_AB;
            }
            else if(events[i].channel == GTFR_CHANNELS_NONE)
            {
                FlexData.stcDataMsg.m_eChannel = UNSPECIFIED;
            }


            /* print frame on the Message Window */
            if ( nullptr != g_pouDIL_FLEXRAY_GIGATRONIK_FLEXI )
            {
                g_pouDIL_FLEXRAY_GIGATRONIK_FLEXI->vWriteIntoClientsBuffer(FlexData);
            }

        }
        else if(events[i].eventTag == GTFR_TX_ACKNOWLEDGE
                &&
                (
                    (   /* check if event is a null frame and check if null frames should be shown */
                        (!((events[i].eventData.txAckFrame.headerFlags & GTFR_HEADERFLAG_NULL) > 0) && (g_pouDIL_FLEXRAY_GIGATRONIK_FLEXI->m_flexi_Settings.showNullFrames == true))
                    )
                    ||
                    (   /* check if event represents a sync or startup frame and check if sync/startup frames should be shown */
                        ((((events[i].eventData.txAckFrame.headerFlags & GTFR_HEADERFLAG_STARTUP) > 0) ||
                          ((events[i].eventData.txAckFrame.headerFlags & GTFR_HEADERFLAG_SYNC) > 0)) &&
                         (g_pouDIL_FLEXRAY_GIGATRONIK_FLEXI->m_flexi_Settings.showSyncStartupFrames == true))
                    )
                    ||
                    (   /* check if event represents a data frame */
                        ((events[i].eventData.txAckFrame.headerFlags & GTFR_HEADERFLAG_NULL) > 0)
                    )
                )

               )
        {

            /* store the relevant event data */
            FlexData.m_eMessageType = FLXMSGTYPE_DATA;
            /* divide timestamp by 100 to get right time dimension */
            FlexData.m_unTimestamp = (LONGLONG)events[i].timeStamp / 100;
            FlexData.stcDataMsg.m_bIsRxMsg = false;
            FlexData.stcDataMsg.m_nSlotID = events[i].eventData.txAckFrame.slotId;
            FlexData.stcDataMsg.m_ucCycleNumber = events[i].eventData.txAckFrame.cycleCount;
            FlexData.stcDataMsg.m_nDLC = events[i].eventData.txAckFrame.payloadLength;      // payloadLength in bytes
            memcpy(FlexData.stcDataMsg.m_ucData, events[i].eventData.txAckFrame.payload, sizeof(events[i].eventData.txAckFrame.payload));


            /* reset header info flags*/
            FlexData.stcDataMsg.m_lHeaderInfoFlags = 0;
            /* check the header bits */
            /* check if the startup frame indicator is set */
            if ((events[i].eventData.txAckFrame.headerFlags & GTFR_HEADERFLAG_STARTUP) > 0)
            {
                FlexData.stcDataMsg.m_lHeaderInfoFlags |= RBIN_FLXHDRINFO_STARTUPFRAME;
            }
            /* check if the synch frame indicator is set */
            if ((events[i].eventData.txAckFrame.headerFlags & GTFR_HEADERFLAG_SYNC) > 0)
            {
                FlexData.stcDataMsg.m_lHeaderInfoFlags |= RBIN_FLXHDRINFO_SYNCFRAME;
            }
            /* check if the null frame indicator is set */
            if (!((events[i].eventData.txAckFrame.headerFlags & GTFR_HEADERFLAG_NULL) > 0))
            {
                FlexData.stcDataMsg.m_lHeaderInfoFlags |= RBIN_FLXHDRINFO_NULLFRAME;
            }
            /* check if the Payload Preamble Indicator is set */
            if ((events[i].eventData.txAckFrame.headerFlags & GTFR_HEADERFLAG_PAYLOAD_PREAMBLE) > 0)
            {
                FlexData.stcDataMsg.m_lHeaderInfoFlags |= RBIN_FLXHDRINFO_PAYLOADPREAMBLE;
            }

            /* check used channel */
            if(events[i].channel == GTFR_CHANNELS_A)
            {
                FlexData.stcDataMsg.m_eChannel = CHANNEL_A;
            }
            else if(events[i].channel == GTFR_CHANNELS_B)
            {
                FlexData.stcDataMsg.m_eChannel = CHANNEL_B;
            }
            else if(events[i].channel == GTFR_CHANNELS_BOTH)
            {
                FlexData.stcDataMsg.m_eChannel = CHANNEL_AB;
            }
            else if(events[i].channel == GTFR_CHANNELS_NONE)
            {
                FlexData.stcDataMsg.m_eChannel = UNSPECIFIED;
            }

            /* print frame on the Message Window */
            if ( nullptr != g_pouDIL_FLEXRAY_GIGATRONIK_FLEXI )
            {
                g_pouDIL_FLEXRAY_GIGATRONIK_FLEXI->vWriteIntoClientsBuffer(FlexData);
            }
        }
        else if(events[i].eventTag == GTFR_ERROR)
        {
            /* there is currently no way to print error message on the Message Window */
        }
        else if(events[i].eventTag == GTFR_STATE_TRANSITION)         // State message currently not supported; see CMsgContainerFlexRay::vProcessNewData at the file MsgContainer_FlexRay.cpp
        {
            /* currently not supported */
        }
        else if(events[i].eventTag == GTFR_CYCLE_START)
        {
            /* currently there is no option to print cycle start */
        }
        else if(events[i].eventTag == GTFR_TIMER_EXPIRED)
        {
        }
    }
}
