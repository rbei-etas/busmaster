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
 *  FILENAME:           FLEXRAY_GIGATRONIK_FLEXI_GtFr_NativeInterface.h
 *
 *  COPYRIGHT:          Gigatronik Ingolstadt GmbH
 *  AUTHOR, DPT:        D. Riedl, KE
 *
 *  DATE OF CREATION:   16.07.2013
 *
 *  DESCRIPTION:        Contains types and function prototypes for the GIGABOX API.
 *
 *********************************************************************************************************************/

/** --- CHANGE LOG ---
 *
 * 1.0.1.0
 * - changed PacketInterpreter to handle multiple events in one USB Package
 *      affected: PacketInterpreter.cs : InterpretePackage()
 * - added SerialNumber to struct GtFr_HardwareInfoType
 *      affected: GtFr_InterfaceInfoTypes.cd : struct GtFr_HardwareInfoType
 * - updated wrapper to show Serial Number
 * - added function: GtFr_SetSerial() to GtFr_Api.cs
 *
 * 1.0.0.3 - 2013-09-04
 * - fixed bug: NullReference exception on some machines when DeInit and Init again
 *
 * 1.1.0.0 - 2013-10-15
 * - Multiple events in one usb package
 * - fixed bug: Every endpoint writer transmits data on both interfaces
 * - fixed bug: Serial number the same for all attached interfaces
 * - introduced some error counters
 * - introduced event callback
 * - performance of event queue increased (Concurrent queue)
 *
 * 1.1.0.1 - 2013-10-22
 * - fixed bug: Thread-safety issue with txmsglookup fixed
 * - introduced statustype and status class
 * - replaced hardware version form number in serial string
 * - Devicemanagement with all its classes to prepare common GIGABOX API :-) and for easy handle of hot plug events.
 * - performance increases
 * - reduced usb event size to 512
 * - increads usb read thread priority
 * - error tracer added
 * - fixed bug with transmit block of txpdus
 * - prepared high payload scenario with respect to overall performance
 * - replaced some static classes with instances for each device
 * - fixed reply manager
 * - fixed return value of receiveevent
 * - introduced callback interface
 * - fixed thread safety issue with DeviceManager
 *
 * 1.1.2.0
 * - changes in comments and security issues in unsafe code segments.
 *
 * 1.1.2.1
 * - DeInit now also works if not initialized
 * - CLI Wrapper has the capability of loading GtFrNET.dll
 * - Removed LibUsbDotNET resolver in GtFrNET.dll
 * - Added assembly resolver in CLIWrapper
 * - Added function GtFr_EnableAsmResolver
 * - Limit for marshalling flexpayload from yte stream to corresponding object in unsafe code.
 *
 * 1.1.2.2
 * - Added type GtFr_OverflowArg and function GtFr_SetOverflowNotifier(...).
 * - Added signature for callback in GtFr_SetoverflowNotifier(...)
 *
 */

#pragma once


#ifdef __cplusplus
extern "C"
{
#endif


    /**
    * This constant defines the maximum payload length for some FlexRay events in words.
    */
#define GTFR_MAX_PAYLOAD_LENGTH  254u

    /**
     * Data types
     */
    typedef unsigned char uint8;
    typedef unsigned short uint16;
    typedef unsigned int uint32;
    typedef unsigned long long uint64;

    /* ===================================== Enumerations ================================== */

    /**
     * This Enumeration defines the return value for the Gigabox flex-i interface api-functions.
     */
    typedef enum
    {
        /* Invalid register address */
        GTFR_INV_REG_ADDR = 0,
        /* No Error occured */
        GTFR_SUCCESS,
        /* Timeout Error */
        GTFR_TIMEOUT,
        /* Interface not connected */
        GTFR_INTERFACE_NOT_CONNECTED,
        /* Invalid interface index */
        GTFR_INV_INTERFACE_IDX,
        /* Invalid interface index */
        GTFR_INV_CTRL_IDX,
        /* This is returned in case of the GtFr_Init has not been called */
        GTFR_NOT_INITIALIZED,
        /* Invalid timer index */
        GTFR_INV_TIMER_IDX,
        /* Invalid controller state for this function */
        GTFR_INV_CTRL_STATE,
        /* Invalid configuration tag */
        GTFR_INV_CFG_TAG,
        /* Error in usb-transmission */
        GTFR_ERROR_USB,
        /* Message Buffer not configured for transmission. */
        GTFR_INV_MSG_BUF_IDX
    } GtFr_ReturnType;

    /**
     * This enumeration defines the transmission mode of a transmit message buffer.
     */
    typedef enum
    {
        /* Transmitted each time when frame id and cycle filter are matching */
        GTFR_TX_MSGBUF_CONTINOUS = 0,
        /* Content written in a transmit message buffer is transmitted only once */
        GTFR_TX_MSGBUF_SINGLE_SHOT,
    } GtFr_MsgBufTxModeType;

    /**
     * This enumeration defines the baud rate for a FlexRay communication controller.
     */
    typedef enum
    {
        /* Baudrate: 10 Mbit/sec. */
        GTFR_BAUDRATE_10M,
        /* Baudrate: 5 Mbit/sec. */
        GTFR_BAUDRATE_5M,
        /* Baudrate: 2.5 Mbit/sec. */
        GTFR_BAUDRATE_2M5,
    } GtFr_BaudRateType;

    /***
    * This enumeration defines all message buffer security modes for a FlexRay Communication
    * Controller. Currently not used by the FlexRay driver.
    **/
    typedef enum
    {
        /* Unlock all message buffers */
        GTFR_MSGBUF_SEC_UNLOCK_ALL = 0,
        /* Lock static transmission */
        GTFR_MSGBUF_SEC_LOCK_STATIC_DONT_TX_STATIC = 1,
        /* Lock all message buffers */
        GTFR_MSGBUF_SEC_LOCK_ALL = 2,
        /* Lock all receive static */
        GTFR_MSGBUF_SEC_LOCK_ALL_DONT_TX_STATIC = 3,
    } GtFr_MsgBufSecurityModeType;


    /**
    * This enumeration defines all channels if any channel is allowed.
    */
    typedef enum
    {
        /* No channel */
        GTFR_CHANNELS_NONE = 0u,
        /* Channel A */
        GTFR_CHANNELS_A = 1u,
        /* Channel B */
        GTFR_CHANNELS_B = 2u,
        /* Both Channels */
        GTFR_CHANNELS_BOTH = 3u,
    } GtFr_ChannelsType;

    /**
    * This enumeration defines all timer modes for a FlexRay communication controller.
    */
    typedef enum
    {
        /* Timer in single shot mode */
        GTFR_TIMER_SINGLE_SHOT_MODE = 0,
        /* Timer in continuous mode */
        GTFR_TIMER_CONTINUOUS_MODE = 1
    } GtFr_TimerModeType;

    /**
     * This enumeration defines the event queue state type.
     */
    typedef enum
    {
        /* no events in queue */
        GTFR_QUEUESTATE_EMPTY = 0,
        /* event successfully retreived */
        GTFR_QUEUESTATE_SUCCESS,
        /* event successfully retrieved, more events in queue */
        GTFR_QUEUESTATE_MORE_AVAILABLE
    }
    GtFr_EventQueueStateType;

    /* =============================== Structs ==============================================*/

    /**
     * This structure defines the status interrupt configuration.
     * Must be used to generate status events.
     */
    typedef struct
    {
        uint32 wakeupStatus;
        uint32 collisionAvoidance;
        uint32 cycleStart;
        uint32 transmitInterrupt;
        uint32 receiveInterrupt;
        uint32 fifo;
        uint32 nmvChanged;
        uint32 stopWatch;
        uint32 startupCompleted;
        uint32 dynamicSegmentStart;
        uint32 wakeupPatternA;
        uint32 mtsReceivedA;
        uint32 wakeupPatternB;
        uint32 mtsReceivedB;
    } GtFr_StatusEnableType;

    /**
     * This structure defines the error interrupt configuration.
     * Must be used to generate error events.
     */
    typedef struct
    {
        uint32 protocolErrorMode;
        uint32 syncBelowMinimum;
        uint32 syncOverFlow;
        uint32 clockCorrectionFail;
        uint32 parityError;
        uint32 fifoOverrun;
        uint32 errorDetectA;
        uint32 transmitViolationA;
        uint32 boundaryA;
        uint32 errorDetectB;
        uint32 transmitViolationB;
        uint32 boundaryB;
    } GtFr_ErrorEnableType;

    /**
     * This struct defines the interrupt configuration of a FlexRay communication controller.
     */
    typedef struct
    {
        /* Member to enable error events */
        GtFr_ErrorEnableType errorEventEnable;
        /* Member to enable status events */
        GtFr_StatusEnableType statusEventEnable;
    } GtFr_InterruptCfgType;

    /**
     * This structure defines the time configuration for a timer of a FlexRay
     * communication controller.
     */
    typedef struct
    {
        /* Value of the base cycle. */
        uint32 BaseCycle;
        /* Value for the repetition. */
        uint32 Repetition;
        /* Offset in macrotick. */
        uint32 MacrotickOffset;
    } GtFr_TimerTimeCfgType;

    /**
     * This class defines the overall configuration of a timer for a FlexRay communication controller.
     */
    typedef struct
    {
        /* This value specifies if the timer starts automatically or not. */
        uint32 AutoStart;
        /* This value sets the operation mode for this timer. */
        GtFr_TimerModeType TimerMode;
        /* This value stores the time configuration for a timer. */
        GtFr_TimerTimeCfgType TimerTimeConfig;

    } GtFr_TimerCfgType;

    /**
    * This structure defines the controller specific configuration of a FlexRay
    * communciation controller.
    */
    typedef struct
    {
        /* This value is used to enable channel A, channel B or both channels. */
        GtFr_ChannelsType ChannelEnable;
        /* This value defines the baudrate. */
        GtFr_BaudRateType BaudRate;
        /* This value specifies if a FlexRay Communicatin Controller is a coldstarter or not. */
        uint32 ColdstartNode;
        /* This value specifies if the driver automatically tries to restart communciation. */
        uint32 AutoRestart;
        /* This value defines the media access test symbol for channel A. */
        uint32 MediaAccessTestA;
        /* This value defines the media access test symbol for channel B. */
        uint32 MediaAccessTestB;
        /* Please refer to the description of GtFr_InterruptCfgType. */
        GtFr_InterruptCfgType InterruptConfig;
    } GtFr_CtrlCfgType;

    /**
    * This structure defines the protocol configuration of a FlexRay communication controller.
    * NOTE: For a detailed description of all fields in this structure please refer to
    * the general FlexRay description.
    */
    typedef struct
    {
        uint32 GColdstartAttempts;
        uint32 GListenNoise;
        uint32 PMacroInitialOffsetA;
        uint32 PMacroInitialOffsetB;
        uint32 GMacroPerCycle;
        uint32 GMaxWithoutClkCorrFatal;
        uint32 GMaxWithoutClkCorrPassive;
        uint32 GNetworkManagementVectorLength;
        uint32 GNumberOfMiniSlots;
        uint32 GNumberOfStaticSlots;
        uint32 GOffsetCorrectionStart;
        uint32 GPayloadLengthStatic;
        uint32 GSyncNodeMax;

        uint32 GdActionPointOffset;
        uint32 GdCasRxLowMax;
        uint32 GdDynamicSlotIdlePhase;
        uint32 GdMiniSlot;
        uint32 GdMiniSlotActionPointOffset;
        uint32 GdNetworkIdleTime;
        uint32 GdStaticSlot;
        uint32 GdTssTransmitter;
        uint32 GdWakeupSymbolRxIdle;
        uint32 GdWakeupSymbolRxLow;
        uint32 GdWakeupSymbolRxWindow;
        uint32 GdWakeupSymbolTxIdle;
        uint32 GdWakeupSymbolTxLow;

        uint32 PAllowHaltDueToClock;
        uint32 PAllowPassiveToActive;
        uint32 PClusterDriftDamping;
        uint32 PDecodingCorrection;
        uint32 PDelayCompensationA;
        uint32 PDelayCompensationB;
        uint32 PExternOffsetCorrection;
        uint32 PExternRateCorrection;
        uint32 PKeySlotUsedForStartup;
        uint32 PKeySlotUsedForSync;
        uint32 PLatestTx;
        uint32 PMicroInitialOffsetA;
        uint32 PMicroInitialOffsetB;
        uint32 PMicroPerCycle;
        uint32 POffsetCorrectionOut;
        uint32 PRateCorrectionOut;
        uint32 PSingleSlotEnabled;
        uint32 PWakeupChannel;
        uint32 PWakeupPattern;

        uint32 PdAcceptedStartupRange;
        uint32 PdListenTimeout;
        uint32 PdMaxDrift;
    } GtFr_ProtocolCfgType;

    /**
     * This structure defines the configuration of a receive message buffer for a
     * FlexRay communication controller.
     */
    typedef struct
    {
        /***
         * This index is used the reference a message buffer
         * The function GtFr_ReturnType GtFr_TransmitTxLPdu(...) is using this value.
         ***/
        uint16 MsgBufNr;
        /* This value defines in which slot a rx message buffer receives data. */
        uint16 FrameId;
        /* This value defines the payload length of a received frame. */
        uint16 PayloadLength;
        /***
         * This values defines whether a rx message buffer generates an interrupt, in
         * case of frame reception.
         ***/
        uint16 MsgBufInterruptEnable;
        /* This value defines if a rx message buffer is enabled for channel A. */
        uint16 ChannelAEnable;
        /* This value defines if a rx message buffer is enabled for channel B. */
        uint16 ChannelBEnable;
        /* This value defines the base cycle for a rx message buffer. */
        uint16 BaseCycle;
        /* This value defines the repetition for a rx message buffer. */
        uint16 Repetition;

    } GtFr_RxMsgBufCfgType;

    /**
     * This class defines the configuration of a transmit message buffer for a
     * FlexRay communication controller.
     */
    typedef struct
    {
        /***
        * This index is used to reference a message buffer.
        * The function GtFr_ReturnType GtFr_TransmitTxLPdu(...) is using this value.
        ***/
        uint16 MsgBufNr;
        /* This value defines in which slot a tx message buffer sends data. */
        uint16 FrameId;
        /* This value defines the payload length of a transmitted frame. */
        uint16 PayloadLength;
        /* Payload preamble indicator for a tx message buffer. */
        uint16 PayloadPreamble;
        /* The Header Crc is computed by the driver. This value has not to be considered */
        uint16 HeaderCrc;
        /****
         * This value defines whether a FlexRay Communciation Controller generates an interrupt in
         * case of frame transmission of this tx message buffer.
         * NOTE: This value has to be true in order to receive Tx Acknowledgements.
         ****/
        uint16 MsgBufInterruptEnable;
        /* This value defines if a tx message buffer is enabled for channel A. */
        uint16 ChannelAEnable;
        /* This value defines if a tx message buffer is enabled for channel B. */
        uint16 ChannelBEnable;
        /* This value defines the base cycle for a tx message buffer. */
        uint16 BaseCycle;
        /* This value defines the repetition for a tx message buffer. */
        uint16 Repetition;
        /* For a description of this value please refer to GtFr_TransModeType */
        GtFr_MsgBufTxModeType TransmissionMode;

    } GtFr_TxMsgBufCfgType;


    /* This enumeration defines the type of a FlexRay buffer */
    typedef enum
    {
        /*Transmission buffer */
        GTFR_MSGBUF_TX = 0,
        /* Receive buffer */
        GTFR_MSGBUF_RX
    } GtFr_MsgBufTagType;

    /* Structure to store a FlexRay buffer configuration */
    typedef struct
    {
        /* Type of buffer */
        GtFr_MsgBufTagType tag;
        /* buffer configuration */
        union
        {
            /* Transmission buffer configuration */
            GtFr_TxMsgBufCfgType txMsgBuf;
            /* Receive buffer configuration */
            GtFr_RxMsgBufCfgType rxMsgBuf;
        };
    } GtFr_MsgBufType;

    /**
    * This structure defines the configuration of message buffers
    * belonging to the static segment.
    */
    typedef struct
    {
        /* Number of buffers */
        uint32 sMsgBufCount;
        /* Pointer to data */
        //GtFr_MsgBufType *sMsgBufDat;
        GtFr_MsgBufType* sMsgBufDat;
    } GtFr_StaticMsgBufType;

    /**
    * This structure defines the configuration of message buffers
    * belonging to the dynamic segment.
    */
    typedef struct
    {
        /* Number of buffer */
        uint32 dMsgBufCount;
        /* Pointer to data */
        GtFr_MsgBufType* dMsgBufDat;
    } GtFr_DynamicMsgBufType;

    /**
     * This structure defines the configuration of the receive FIFO for a FlexRay
     * Communication Controller.
     */
    typedef struct
    {
        /* This values defines the overall size of a FIFO. */
        uint32 depth;
        /***
            * This value defines the entry size of a FIFO [words].
            * NOTE: In order to receive all frames completely, this value
            *       must be the payload length of the largest frame in a cluster.
            ***/
        uint32 entrySize;
        /* This value defines the fill level for the generation of a critical level interrupt. */
        uint32 criticalLevel;
        /* Please refer to the description of GtFr_ChannelType. */
        GtFr_ChannelsType rejectionFilterChannel;
        /* This value is currently not used. */
        uint32 rejectionFilterValue;
        /***
            * This value defines the FIFO Rejection Filter Mask.
            * NOTE: For a detailed description please refer to 4.7.3 FIFO Rejection Filter Mask
            *       in E-Ray user manual.
            ***/
        uint32 rejectionFilterMask;
        /***
            * For a detailed description please refer to 4.7.3 FIFO Rejection Filter Mask
            * in E-Ray user manual.
            ***/
        uint32 rejectionFilterBaseCycle;
        /***
        * For a detailed description please refer to 4.7.3 FIFO Rejection Filter Mask
        * in E-Ray user manual.
        ***/
        uint32 rejectionFilterRepetition;
        /* This value defines whether frames in the static segment are stored in the FIFO or not. */
        uint32 rejectStaticSegment;
        /* This value defines whether null frames are stored in the FIFO or not. */
        uint32 rejectNullFrames;
    } GtFr_FifoCfgType;

    /**
    * This structure defines the configuration of all message buffers of a FlexRay
    * controller.
    */
    typedef struct
    {
        /* sync frame payload multiplex */
        uint32 splm;
        /* message buffer security */
        GtFr_MsgBufSecurityModeType sec;
        /* static message buffers */
        GtFr_StaticMsgBufType sMsgBuf;
        /* dynamic message buffers */
        GtFr_DynamicMsgBufType dMsgBuf;
        /* fifo buffers */
        GtFr_FifoCfgType fifoMsgBuf;
    } GtFr_MsgBufCfgType;

    /* Union to store a configuration.
     * See the corresponding configuration structures for details
     */
    typedef union
    {
        /* Message buffer configuration */
        GtFr_MsgBufCfgType msgBufCfg;
        /* Controller configuration */
        GtFr_CtrlCfgType ctrlCfg;
        /* Protocol configuration */
        GtFr_ProtocolCfgType protCfg;
    } GtFr_CfgDataType;

    /* Type of configuration */
    typedef enum
    {
        /* Controller configuration */
        GTFR_CTRL_CFG = 0u,
        /* Protocol configuration */
        GTFR_PROT_CFG,
        /* Message buffer configuration */
        GTFR_MSGBUF_CFG
    } GtFr_CfgTagType;

    /**
     * This structure defines the configuration of a FlexRay communication controller.
     */
    typedef struct
    {
        /* Type of configuration */
        GtFr_CfgTagType cfgTag;
        /* Configuration data */
        GtFr_CfgDataType cfgData;
    } GtFr_CfgType;


    /**
     * This enumeration defines the protocol operation controller state (POC-State).
     * NOTE: For a detailed description of POC-States please refer to E-Ray user manual.
     */
    typedef enum
    {
        GTFR_PROTSTATE_DEFAULT_CONFIG = 0,
        GTFR_PROTSTATE_READY,
        GTFR_PROTSTATE_NORMAL_ACTIVE,
        GTFR_PROTSTATE_NORMAL_PASSIVE,
        GTFR_PROTSTATE_HALT,
        GTFR_PROTSTATE_MONITOR_MODE,
        GTFR_PROTSTATE_AUTORESTART,
        GTFR_PROTSTATE_RESERVED1,
        GTFR_PROTSTATE_RESERVED2,
        GTFR_PROTSTATE_RESERVED3,
        GTFR_PROTSTATE_RESERVED4,
        GTFR_PROTSTATE_RESERVED5,
        GTFR_PROTSTATE_RESERVED6,
        GTFR_PROTSTATE_RESERVED7,
        GTFR_PROTSTATE_RESERVED8,
        GTFR_PROTSTATE_CONFIG,

        GTFR_PROTSTATE_WAKEUP_STANDBY,
        GTFR_PROTSTATE_WAKEUP_LISTEN,
        GTFR_PROTSTATE_WAKEUP_SEND,
        GTFR_PROTSTATE_WAKEUP_DETECT,
        GTFR_PROTSTATE_WAKEUP_RESERVED0,
        GTFR_PROTSTATE_WAKEUP_RESERVED1,
        GTFR_PROTSTATE_WAKEUP_RESERVED2,
        GTFR_PROTSTATE_WAKEUP_RESERVED3,
        GTFR_PROTSTATE_WAKEUP_RESERVED4,
        GTFR_PROTSTATE_WAKEUP_RESERVED5,
        GTFR_PROTSTATE_WAKEUP_RESERVED6,
        GTFR_PROTSTATE_WAKEUP_RESERVED7,
        GTFR_PROTSTATE_WAKEUP_RESERVED8,
        GTFR_PROTSTATE_WAKEUP_RESERVED9,
        GTFR_PROTSTATE_WAKEUP_RESERVED10,
        GTFR_PROTSTATE_WAKEUP_RESERVED11,

        GTFR_PROTSTATE_STARTUP_PREPARE,
        GTFR_PROTSTATE_COLDSTART_LISTEN,
        GTFR_PROTSTATE_COLDSTART_COLLISION_RESOLUTION,
        GTFR_PROTSTATE_COLDSTART_CONSISTENCY_CHECK,
        GTFR_PROTSTATE_COLDSTART_GAP,
        GTFR_PROTSTATE_COLDSTART_JOIN,
        GTFR_PROTSTATE_INTEGRATION_COLDSTART_CHECK,
        GTFR_PROTSTATE_INTEGRATION_LISTEN,
        GTFR_PROTSTATE_INTEGRATION_CONSISTENCY_CHECK,
        GTFR_PROTSTATE_INITIALIZE_SCHEDULE,
        GTFR_PROTSTATE_ABORT_STARTUP,
        GTFR_PROTSTATE_STARTUP_SUCCESS,
        GTFR_PROTSTATE_STARTUP_RESERVED0,
        GTFR_PROTSTATE_STARTUP_RESERVED1,
        GTFR_PROTSTATE_STARTUP_RESERVED2,
        GTFR_PROTSTATE_STARTUP_RESERVED3
    } GtFr_PocStateType;


    /**
     * This enumeration defines an FrEvIf_EventTagType.
     */
    typedef enum
    {
        /* This value indicates a FlexRay cycle start event. */
        GTFR_CYCLE_START = 0,
        /* This value indicates a FlexRay received frame event. */
        GTFR_RX_FRAME,
        /* This value indicates a FlexRay transmit acknowledge event. */
        GTFR_TX_ACKNOWLEDGE,
        /* This value indicates a FlexRay error event. */
        GTFR_ERROR,
        /* This value indicates a FlexRay Timer event. */
        GTFR_TIMER_EXPIRED,
        /* This value indicates a FlexRay state transition event. */
        GTFR_STATE_TRANSITION
    } GtFr_EventTagType;

    /**
     * This struct defines a Transmit Acknowledge Frame.
     */
    typedef struct
    {
        /* This variable indicates the message buffer index */
        uint16 msgBufIdx;
        /* This value stores the flags of a frame header. */
        uint16 headerFlags;
        /* This value stores the stores the header crc. */
        uint16 headerCrc;
        /* This value stores the slot id of the frame. */
        uint16 slotId;
        /* This value stores the cycle the frame has been sent. */
        uint8 cycleCount;
        /* This value stores the payload length of the frame. */
        uint8 payloadLength;
        /* This struct stores the payload. */
        uint8 payload[GTFR_MAX_PAYLOAD_LENGTH];
    } GtFr_TxAckFrameType;

    /**
     * This struct defines a Receive Frame.
     */
    typedef struct
    {
        /* This parameter indicates the source of a rx frame */
        bool isFifo;
        /* This variable indicates the message buffer index */
        uint16 msgBufIdx;
        /* This value stores the flags of a frame header. */
        uint16 headerFlags;
        /* This value stores the stores the header crc. */
        uint16 headerCrc;
        /* This value stores the slot id of the frame. */
        uint16 slotId;
        /* This value stores the cycle the frame has been sent. */
        uint8 cycleCount;
        /* This value stores the payload length of the frame. */
        uint8 payloadLength;
        /* This struct stores the payload. */
        uint8 payload[GTFR_MAX_PAYLOAD_LENGTH];
    } GtFr_RxFrameType;

    /**
     * This struct defines a the data structure for a cycle event tag.
     */
    typedef struct
    {
        uint32 vRateCorrection;
        uint32 vOffsetCorrection;
        uint8 cycleCount;
        uint8 vAllowPassiveToActive;
        uint8 vClockCorrectionFailed;
    } GtFr_CycleStartType;

    /**
     * This struct defines a the data structure for a timer event tag.
     */
    typedef struct
    {
        /* Timer ID */
        uint8 timerIdx;
        /* Number of FlexRay cycles */
        uint8 cycleCount;
        /* Macrotick offset in a cycle */
        uint32 macrotickOffset;
    } GtFr_TimerExpiredTyped;

    /**
     * This enumeration defines a FrIf_ErrorTagType.
     * NOTE: Refer to E-Ray User manual chapter 4.4.1 for further information.
     */
    typedef enum
    {
        /* This tag indicates that POC Error Mode has changed. */
        GTFR_ERROR_FR_PEMC = 0u,
        /* This tag indicates that a CHI Command was not successful. */
        GTFR_ERROR_FR_CNA = 1u,
        /**
        * This tag signals that the number of sync frames received during
        * the last communication cycle was below the limit required by the FlexRay protocol.
        */
        GTFR_ERROR_FR_SFBM = 2u,
        /* This tag indicates that a sync frame overflow has occurred. */
        GTFR_ERROR_FR_SFO = 3u,
        /* This tag signals a clock correction failure. May happen during startup. */
        GTFR_ERROR_FR_CCF = 4u,
        /* This tag signals that a write access to the chi command vector was not successful */
        GTFR_ERROR_FR_CCL = 5u,
        /* This tag singals a fifo overrun. */
        GTFR_ERROR_FR_RFO = 6u,
        /* This tag signals a empty fifo access. */
        GTFR_ERROR_FR_EFA = 7u,
        /* This tag signals an illegal access to the input buffer. */
        GTFR_ERROR_FR_IIBA = 8u,
        /* This tag signals an illegal access to the output buffer. */
        GTFR_ERROR_FR_IOBA = 9u,
        /* This tag signals a message handler constraints violation condition. */
        GTFR_ERROR_FR_MHF = 10u,
        /* This tag signals an error on channel A. */
        GTFR_ERROR_FR_EDA = 11u,
        /* This tag signals a transmit violation on channel A. */
        GTFR_ERROR_FR_LTVA = 12u,
        /* This tag signals a boundary violation on channel A. */
        GTFR_ERROR_FR_TABA = 13u,
        /* This tag signals an error on channel B. */
        GTFR_ERROR_FR_EDB = 14u,
        /* This tag signals a transmit violation on channel B. */
        GTFR_ERROR_FR_LTVB = 15u,
        /* This tag signals a boundary violation on channel B. */
        GTFR_ERROR_FR_TABB = 16u,
    } GtFr_ErrorTagType;

    /**
     * This type defines the pemc error.
     */
    typedef enum
    {
        GTFR_FR_PEMC_ACTIVE = 0u,
        GTFR_FR_PEMC_PASSIVE = 1u,
        GTFR_FR_PEMC_COMM_HALT = 2u,
        GTFR_FR_PEMC_RESERVED = 3u,
    } GtFr_ErrorPemcType;

    /**
     * This type defines a sync frame SFBM error.
     */
    typedef struct
    {
        uint8 evenSyncFramesA;
        uint8 oddSyncFramesA;
        uint8 evenSyncFramesB;
        uint8 oddSyncFramesB;
    } GtFr_ErrorSfbmType;

    /**
     * This type defines a sync frame SFO error.
     */
    typedef struct
    {
        uint8 evenSyncFramesA;
        uint8 oddSyncFramesA;
        uint8 evenSyncFramesB;
        uint8 oddSyncFramesB;
        uint8 gSyncNodeMax;
    } GtFr_ErrorSfoType;

    /**
     * This type defines a CCF error.
     */
    typedef struct
    {
        uint8 evenSyncFramesA;
        uint8 oddSyncFramesA;
        uint8 evenSyncFramesB;
        uint8 oddSyncFramesB;
        uint8 mocs;
        uint8 oclr;
        uint8 mrcs;
        uint8 rclr;
        uint8 clockCorrFailedCounter;
    } GtFr_ErrorCcfType;

    /**
     * This type defines a EDA/B error.
     */
    typedef struct
    {
        /* Channel A = 0, Channel B = 1 */
        uint8 channel;
        uint8 validFrame;
        uint8 syntaxError;
        uint8 contentError;
        uint8 sbViolation;
    } GtFr_ErrorEdChType;

    /**
     * This type defines FlexRay-Errors.
     */
    typedef struct
    {
        /* Cycle the error occurred. */
        uint8 cycle;
        GtFr_ErrorTagType errorTag;
        /* Not all tags have further information! */
        union
        {
            /* POC Error mode changed. */
            GtFr_ErrorPemcType pemcError;
            /* SFBM error */
            GtFr_ErrorSfbmType sfbmError;
            /* SFO error */
            GtFr_ErrorSfoType sfoError;
            /* Clock correction failure (CCF). */
            GtFr_ErrorCcfType ccfError;
            /* Error detected on a channel. */
            GtFr_ErrorEdChType edError;
        };

    } GtFr_ErrorType;

    /**
     *This union stores all FlexRay events.
     */
    typedef union
    {
        /* This variable stores a GtFr_RxFrameType */
        GtFr_RxFrameType rxFrame;
        /* This variable stores a GtFr_TxAckFrameType */
        GtFr_TxAckFrameType txAckFrame;
        /* This variable stores a GtFr_CycleStartType */
        GtFr_CycleStartType cycleStart;
        /* This variable stores a GtFr_TimerExpiredType */
        GtFr_TimerExpiredTyped timer;
        /* This variable stores a GtFr_StateType */
        GtFr_PocStateType state;
        /* This variable stores a GtFr_ErrorType */
        GtFr_ErrorType errorInfo;
    } GtFr_EventDataType;

    /**
     * This structure defines a FlexRay event. Events are stored in external SDRAM and transmitted to the windows API.
     */
    typedef struct
    {
        /* This variable defines the overall size of the event. */
        uint32 eventSize;
        /* This variable defines the event type. */
        GtFr_EventTagType eventTag;
        /* This value indicates the channel for the event. */
        GtFr_ChannelsType channel;
        /* reserved. */
        uint32 reserved0;
        /* This array indicates the time the event has occurred, computed by API */
        uint64 timeStamp;
        /* This variable defines the controller index */
        uint8 ctrlIdx;
        /* reserved. */
        uint8 reserved1[3];
        /* This variable stores the event data. */
        GtFr_EventDataType eventData;
    } GtFr_EventType;


    /**
     * This type defines all commands used in function GtFr_ExecuteCommand(...).
     */
    typedef enum
    {
        /* This command transfers the FlexRay-Configuration to the Communication Controller,
        * and leaves the controller in state "READY" */
        GTFR_CMD_INIT = 0u,
        /* This command starts the cluster communication. */
        GTFR_CMD_START = 1u,
        /* This command sets a controller to state "WAKEUP". */
        GTFR_CMD_WAKEUP = 2u,
        /* This command halts the communication */
        GTFR_CMD_HALT = 3u,
        /* This command freezes the communication */
        GTFR_CMD_FREEZE = 4u,
        /* This command sends a single MTS symbol during the next following symbol window
        * on the channel configured by MTSA, MTSB, when called in POC state NORMAL_ACTIVE.*/
        GTFR_CMD_SEND_MTS = 5u,
        /* This command executes a hardware-reset for a FlexRay-Controller. */
        GTFR_CMD_HARD_RESET = 6u,
    } GtFr_CommandTagType;

    /**
     * Interface information types.
     */
    /**
     * This type defines Hardware-Interface informations.
     */
    typedef struct
    {
        /* Firmware of the Gigabox flex-i */
        char firmwareVersion[8];
        /* Hardware version of the Gigabox flex-i */
        char hardwareVersion[8];
    }  GtFr_HardwareInfoType;

    /**
     * This type defines the Usb-Driver information.
     */
    typedef enum
    {
        //UNKNOWN = 0u,           // already defined in Base_FlexRay_Buffer.h, value is also 0.
        LIBUSB = 1u,
        WINUSB = 2u,
        MONOLIBUSB = 3u,
        LIBUSBWINBACK = 4u,
    } GtFr_UsbDriverType;

    /**
     * This type defines Sofware informations.
     */
    typedef struct
    {
        char assemblyVersion[8];
        GtFr_UsbDriverType usbDriver;
    }  GtFr_SoftwareInfoType;

    /**
     * This type defines all interface informations.
     */
    typedef struct
    {
        /* Information about the attached hardware interface. */
        GtFr_HardwareInfoType hardwareInfo;
        /* Information about the Library. */
        GtFr_SoftwareInfoType softwareInfo;
        /* Unique interface index for each attached FlexRay Interface. */
        uint8 iIdx;
        /* reserved */
        uint8 reserved[3];
    } GtFr_InterfaceInfoType;

    /**
     * This type defines extended interface informations.
     */
    typedef struct
    {
        /* flex-i Serial */
        uint64 serial;
        /* License Number */
        uint8 license[32];
        /* reserved for future use */
        uint8 reserved[16];
    } GtFr_InterfaceExtInfoType;

    // end interface information types.

    /**
     * Defines all possible tags for the function GtFr_ClockCommand(...).
     * This clock is used for the event-timestamps.
     */
    typedef enum
    {
        /* Stops the clock. */
        GTFR_CLOCK_STOP = 0,
        /* Resets the clock value to 0 and stops counting. */
        GTFR_CLOCK_RESET,
        /* Restarts the clock counting beginning from 0. */
        GTFR_CLOCK_RESTART,
        /* Starts clock counting. */
        GTFR_CLOCK_START
    }
    GtFr_ClkCommandTagType;

    /**
     * This type can be used to transmit a block of several FlexRay frames.
     * NOTE: Used in function GtFr_TransmitTxLpuBlock(...)
     */
    typedef struct
    {
        /* Index of the message buffer */
        uint8 msgBufIdx;
        /* Length of the frame */
        uint8 lSduLength;
        /* Pointer to the payload to be transmitted. */
        uint8* lSdu;
    }
    GtFr_TxLPduType;

    /**
     * This type can be used to transmit a block of several FlexRay frames
     * NOTE: Used in function GtFr_TransmitTxLpuBlock(...)
     */
    typedef struct
    {
        /* Count of Frames to be transmitted. */
        uint8 txLPduCount;
        /* Pointer to the TxLpdu-struct holding the data. */
        GtFr_TxLPduType* txLPduDat;
    }
    GtFr_TxLPduBlockType;

    /**
     * Status type for the the reception of events.
     */
    typedef struct
    {
        /* Number of elements */
        uint32 elementCount;
        /* Status of the event queue */
        GtFr_EventQueueStateType state;
        /* Number of elements in the firmware buffer. */
        uint16 elementCountFirmware;
        uint8 maxElementCountFw;
        /* reserved */
        uint8 reserved;
    }
    GtFr_EventQueueStatusType;

    /**
     * This type defines the register map of the FlexRay communication controller
     * MB88121C from Bosch with E-Ray Core.
     */
    typedef struct
    {
        /* Mb88121 Customer registers */
        uint32 frver;
        uint32 frccnt;
        uint32 frcus2;
        uint32 boschInt;
        /* special registers */
        uint32 res1;
        uint32 res2;
        uint32 res3;
        uint32 frlck;
        /* interrupt registers */
        uint32 freir;
        uint32 frsir;
        uint32 freils;
        uint32 frsils;
        uint32 freies;
        uint32 freier;
        uint32 frsies;
        uint32 frsier;
        uint32 frile;
        uint32 frt0c;
        uint32 frt1c;
        uint32 frstpw[2];
        uint32 res4[11];
        /* CC control registers */
        uint32 frsucc[3];
        uint32 frnemc;
        uint32 frprtc[2];
        uint32 frmhdc;
        uint32 res5;
        uint32 frgtuc[11];
        uint32 res6[13];
        /* CC status register */
        uint32 frccsv;
        uint32 frccev;
        uint32 res7[2];
        uint32 frscv;
        uint32 frmtccv;
        uint32 frrcv;
        uint32 frocv;
        uint32 frsfs;
        uint32 frswnit;
        uint32 fracs;
        uint32 res8;
        uint32 fresid[15];
        uint32 res9;
        uint32 frosid[15];
        uint32 res10;
        uint32 frnmv[3];
        uint32 res11[81];
        /* message buffer control register */
        uint32 frmrc;
        uint32 frfrf;
        uint32 frfrfm;
        uint32 frfcl;
        /* message buffer status register */
        uint32 frmhds;
        uint32 frldts;
        uint32 frfsr;
        uint32 frmhdf;
        uint32 frtxrq[4];
        uint32 frndat[4];
        uint32 frmbsc[4];
        uint32 res12[40];
        /* identification register */
        uint32 frcrel;
        uint32 frendn;
        uint32 res13[2];
        /* input buffer */
        uint32 frwrds[64];
        uint32 frwrhs[3];
        uint32 res14;
        uint32 fribcm;
        uint32 fribcr;
        uint32 res15[58];
        /* output buffer */
        uint32 frrdds[64];
        uint32 frrdhs[3];
        uint32 frmbs;
        uint32 frobcm;
        uint32 frobcr;
        uint32 res16[58];
    }
    GtFr_ErayRegisterMapType;

    /**
     * This type defines possible sources for an queue overflow.
     */
    typedef enum
    {
        GTFR_QUEUE_OVERFLOW_SOURCE_HARDWARE = 0u,
        GTFR_QUEUE_OVERFLOW_SOURCE_API
    }
    GtFr_QueueOverflowSourceType;

    /**
     * This type defines the arguments for an queue overflow callback function.
     */
    typedef struct
    {
        /* Overflow source. */
        GtFr_QueueOverflowSourceType ofSrc;
        /* Reerved for future use. */
        uint32 reserved0;
        /* Serial of the device raising the overflow event. */
        uint64 serial;
        /* Interface index of the device raising the overflow event. */
        uint8 iIdx;
        /* Reerved for future use. */
        uint8 reserved1[3];
    }
    GtFr_QueueOverflowArgsType;


    /**
    * This enumaration defines the type of a software based extended FIFO Frame ID filter
    */
    typedef enum
    {
        FIFO_FID_ACCEPTANCE_FILTER = 0u,
        FIFO_FID_REJECTION_FILTER,
    }
    GtFr_FifoFIdFilterTagType;


    /**
     * This class defines the configuration of a software based receive fifo filter,
     * which can be used in addition to the Hardware Flx Controlelr Frame ID filter.
     * These filters can be set or changed during runtime.
     */
    typedef struct
    {
        /* Selection of Pass/Rejection filter. Invalid if general filter is selected. */
        GtFr_FifoFIdFilterTagType filterTag;
        /* At this Frame ID, filtering will start. Invalid if general filter is selected. */
        uint16 lowerFrameId;
        /* At this FrameId, filtering will end. Invalid if general filter is selected. */
        uint16 upperFrameId;
        /* Selection of the controller. */
        uint8 ctrlIdx;
        /* Selection of the filter. */
        uint8 filterId;
        /* Filter enablement. */
        bool isActive;
        /* reserved */
        uint8 reserved;
    }
    GtFr_ExtRxFifoFilterCfgType;




    /**************************************** functionpointer for flex-i API ****************************************/

    /**
     * This function enables the assembly resolver.
     * Call this function if you want to load CLIWrapper.dll from any folder other the application directory.
     * NOTE: Call this before any other API function.
     */
    typedef void (* PF_FLEXI_GtFr_EnableAsmResolver)(void);

    /**
     * This function initializes the API
     */
    typedef GtFr_ReturnType (* PF_FLEXI_GtFr_Init)(void);

    /**
    * This function returns the state of the indexed FlexRay communciation controller.
    */
    typedef GtFr_ReturnType (* PF_FLEXI_GtFr_GetCtrlState)(
        /* Interface index */
        const uint8 iIdx,
        /* Index of the FlexRay controller */
        const uint8 CtrlIdx,
        /* Protocol operation controller state */
        GtFr_PocStateType* CtrlState);

    /**
     * This function gets an event from the receive queue
     * and outputs some status information concerning the event queue.
     */
    typedef GtFr_ReturnType (* PF_FLEXI_GtFr_ReceiveEvent)(
        /* Interface index */
        const uint8 iIdx,
        /* Pointer to a event data type */
        GtFr_EventType* e,
        /* Pointer to a event queue status type. */
        GtFr_EventQueueStatusType* eQueueStatus);

    /**
     * This function forces the indexed FlexRay communication controller to transmit the message
     * specified by the function arguments.
     */
    typedef GtFr_ReturnType (* PF_FLEXI_GtFr_TransmitTxLPdu)(
        /* Interface index */
        uint8 iIdx,
        /* Index of the FlexRay controller */
        uint8 ctrlIdx,
        /* Index of the message buffer */
        uint16 msgBufIdx,
        /* Length of the frame */
        uint8 lSduLength,
        /* Payload to be transmitted */
        uint8* lSdu);
    // todo: const pointer!

    /**
    * This function forces the indexed FlexRay communication controller to transmit a block
    * of several frames specified by the function arguments.
    */
    typedef GtFr_ReturnType (* PF_FLEXI_GtFr_TransmitTxLPduBlock)(
        /* Interface Index */
        const uint8 iIdx,
        /* Index of the FlexRay controller */
        const uint8 ctrlIdx,
        /* Contains message buffer data. */
        const GtFr_TxLPduBlockType* txDat);
    // todo: const pointer!

    /**
    * This function returns the global network time of the specified
    * FlexRay communication controller,
    * if the specified controller is in state normal_active
    */
    typedef GtFr_ReturnType (* PF_FLEXI_GtFr_GetGlobalTime)(
        /* Interface index */
        const uint8 iIdx,
        /* Index of the FlexRay controller */
        const uint8 ctrlIdx,
        /* Cycle */
        uint8* cycle,
        /* Macrotick */
        uint16* macroTick);

    /**
     * This function can be used to read the value of the register indexed by CtrlIdx and RegAddr.
     */
    typedef GtFr_ReturnType (* PF_FLEXI_GtFr_GetCtrlRegister)(
        /* Interface index */
        const uint8 iIdx,
        /* Index of the FlexRay controller */
        const uint8 ctrlIdx,
        /* Register address (Range: 2Kbyte, Values: 32 Bit boundary) */
        const uint32 regAddr,
        /* Value of the indexed register */
        uint32* value);

    /**
     * This function initializes a specific FlexRay communication controller.
     * Supported Tags are:
     *      GTFR_CTRL_CFG = 0u,
     *      GTFR_PROT_CFG,
     *      GTFR_MSGBUF_CFG,
     *      GTFR_FIFO_CFG
     */
    typedef GtFr_ReturnType (* PF_FLEXI_GtFr_InitComCtrl)(
        /* Interface index */
        const uint8 iIdx,
        /* Index of the FlexRay controller */
        const uint8 ctrlIdx,
        /* Configuration */
        const GtFr_CfgType cfg);

    /**
     * This function initializes either the absolute timer or the relative timer.
     */
    typedef GtFr_ReturnType (* PF_FLEXI_GtFr_InitTimer)(
        /* Interface index */
        const uint8 iIdx,
        /* Index of the FlexRay controller */
        const uint8 ctrlIdx,
        /* Timer index, 0 = absolute timer, 1 = relative timer*/
        const uint8 timerIdx,
        /* Configuration of the specified timer */
        const GtFr_TimerCfgType timerConfig);

    /**
     * This function starts either the absolute timer or the relative timer.
     */
    typedef GtFr_ReturnType (* PF_FLEXI_GtFr_StartTimer)(
        /* Interface index */
        const uint8 iIdx,
        /* Index of the FlexRay controller */
        const uint8 ctrlIdx,
        /* Timer index, 0 = absolute timer, 1 = relative timer*/
        const uint8 timerIdx);

    /**
     * This function executes a command on a certain FlexRay controller.
     * Tags are decribed at GtFr_CommandTagType.
     */
    typedef GtFr_ReturnType (* PF_FLEXI_GtFr_ExecuteCommand)(
        /* Interface index */
        const uint8 iIdx,
        /* Index of the FlexRay controller */
        const uint8 ctrlIdx,
        /* Command type. */
        const GtFr_CommandTagType command);

    /**
     * This function can be used to get the number of attached
     * FlexRay interfaces.
     */
    typedef GtFr_ReturnType (* PF_FLEXI_GtFr_GetInterfaceCount)(
        /* the number of attached FlexRay interfaces */
        uint8* iCount);

    /**
     * This function can be used to get informations about all attached
     * FlexRay interfaces.
     */
    typedef GtFr_ReturnType (* PF_FLEXI_GtFr_GetInterfaceInfo)(
        /* Interface index */
        const uint8 iIdx,
        /* Interface information for all attached interfaces. */
        GtFr_InterfaceInfoType* iInfo);

    /**
     * This function can be used to get extended informations about all attached
     * FlexRay interfaces.
     */
    typedef GtFr_ReturnType (* PF_FLEXI_GtFr_GetInterfaceExtInfo)(
        /* Interface index */
        const uint8 iIdx,
        /* Interface information for all attached interfaces. */
        GtFr_InterfaceExtInfoType* iExtInfo);

    /**
     * This function can be used to reset, restart, stop and start the clock
     * used for the event-timestamps (API clock).
     */
    typedef GtFr_ReturnType (* PF_FLEXI_GtFr_ClockCommand)(
        /* The tag for this function. */
        const GtFr_ClkCommandTagType clkTag);


    /**
     * This function returns the time of the API clock
     * in µs.
     */
    typedef GtFr_ReturnType (* PF_FLEXI_GtFr_GetClockTime)(
        /* The current time in µs. */
        uint64* time);

    /**
     * This functions is closing the usb-driver (closes endpoints and releases all open interfaces).
     * It also deinitializes the API and resets the hardware-interfaces to the initial state.
     */
    typedef GtFr_ReturnType (* PF_FLEXI_GtFr_DeInit)(void);

    /**
     * This function can be used to get the Controller Configuration
     * of a FlexRay communication controller specified by the parameters
     * ctrlIdx and iIdx.
     */
    typedef GtFr_ReturnType (* PF_FLEXI_GtFr_GetCtrlConfig)(
        /* Index of the Interface. */
        const uint8 iIdx,
        /* Index of the FlexRay communication controller. */
        const uint8 ctrlIdx,
        /* FlexRay Controller Configuration output. */
        GtFr_CtrlCfgType** ctrlCfgPtrAddr);

    /**
     * This function can be used to get the Protocol Configuration
     * of a FlexRay communication controller specified by the parameters
     * ctrlIdx and iIdx.
     */
    typedef GtFr_ReturnType (* PF_FLEXI_GtFr_GetProtConfig)(
        /* Index of the Interface. */
        const uint8 iIdx,
        /* Index of the FlexRay communication controller. */
        const uint8 ctrlIdx,
        /* FlexRay Controller Configuration output. */
        GtFr_ProtocolCfgType** protCfgPtrAddr);

    /**
     * This function can be used to get the protocol configuration
     * of a FlexRay communication controller specified by the parameters
     * ctrlIdx and iIdx.
     */
    typedef GtFr_ReturnType (* PF_FLEXI_GtFr_GetMsgBufConfig)(
        /* Index of the Interface. */
        const uint8 iIdx,
        /* Index of the FlexRay communication controller. */
        const uint8 ctrlIdx,
        /* FlexRay Controller Configuration output. */
        GtFr_MsgBufCfgType** msgBufCfgPtrAddr);

    /**
     * This function reads the register map of the FlexRay Communciation Controller
     * MB88121 from Fujitsu with E-Ray Core.
     */
    typedef GtFr_ReturnType (* PF_FLEXI_GtFr_GetErayRegisterMap)(
        /* Interface Index */
        const uint8 iIdx,
        /* Index of the FlexRay communication controller */
        const uint8 ctrlIdx,
        /* Register Map output. */
        GtFr_ErayRegisterMapType* regMap);

    /**
     * This function returns the event queue count and capacity
     * of the indexed interface.
     */
    typedef GtFr_ReturnType (* PF_FLEXI_GtFr_GetEventQueueInfo)(
        /* index of the interface. */
        const uint8 iIdx,
        /* Current element count in the queue. */
        uint32* eQueueLevel,
        /* Capacity of the event queue. */
        uint32* eQueueCap);

    /**
     * This function resets the event queue to zero elements.
     */
    typedef GtFr_ReturnType (* PF_FLEXI_GtFr_ClearEventQueue)(
        /* Index of the interface. */
        const uint8 iIdx);

    /**
     * This function enables the transceivers specified by the parameter channels
     * for the indexed interface.
     */
    typedef GtFr_ReturnType (* PF_FLEXI_GtFr_EnableTransceiver)(
        /* Index of the interface. */
        const uint8 iIdx,
        /* Channel or channels to enable or disable. */
        const GtFr_ChannelsType channels);

    /**
     * This function enables the transceivers specified by the parameter channels
     * for the indexed interface.
     */
    typedef GtFr_ReturnType (* PF_FLEXI_GtFr_DisableTransceiver)(
        /* Index of the interface. */
        const uint8 iIdx,
        /* Channel or channels to enable or disable. */
        const GtFr_ChannelsType channels);

    /**
     * This function restarts the Hardware-Interface in the Firmware
     * Upgrade mode.
     */
    typedef GtFr_ReturnType (* PF_FLEXI_GtFr_FlashFirmwareMode)(
        const uint8 iIdx);

    /**
     * This function sets a user defined function as event queue notification function.
     * The funtion will be raised if the defined threshold (number of elements in the event queue) is reached.
     */
    typedef GtFr_ReturnType (* PF_FLEXI_GtFr_SetEventQueueNotification)(
        /* Index of the interface. */
        const uint8 iIdx,
        /* Threshold in the event queue. */
        const uint16 threshold,
        /* Callback function. */
        void (*cbkFct)(GtFr_EventQueueStatusType, GtFr_EventType*));

    /**
     * This function sets a user defined function that will be raised if an queue overflow occurs.
     * Overflow source may be hardware (firmware) or API.
     */
    typedef GtFr_ReturnType (* PF_FLEXI_GtFr_SetEventQueueOverflow)(
        /* Index of the interface. */
        const uint8 iIdx,
        /* Callback function. */
        void (*cbkFct)(GtFr_QueueOverflowArgsType));



    /**
     * This function reconfigures the Message Buffer specified through
     * Interface index and Index of the FlexRay communication controller.
     * Exceptions:
     * - MsgBuf is assigned to the FIFO buffer
     * - The PayloadLength (PL) doesn't match the previous configuration
     * - The MsgBuf config is invalid (Refer to InitMsgBuf)
     * Additional constraints if the controller is not in DEFAULT_CONFIG or
     * in CONFIG state: Message buffers
     * - locked by the security mode (sec)
     * - used for startup or sync
     * can not be reconfigured.
     */
    typedef GtFr_ReturnType (* PF_FLEXI_GtFr_ReconfigureMsgBuf)(
        /* Inteface Index */
        uint8 iIdx,
        /* Index of the FlexRay Communication Controller */
        uint8 ctrlIdx,
        /* Message buffer configuration to be applied */
        GtFr_MsgBufType msgBufCfg);



    /**
     * This function reads the maximum number of supported extended rx fifo
     * filter. In this scope extended means software based frame Id filter
     * applied to all rx fifo frames. Supported modes are reject and accept.
     * If the function is not supported by fw version return value will be
     * GtFr_ReturnType.GTFR_NOT_SUPPORTED.
     */
    typedef GtFr_ReturnType (* PF_FLEXI_GtFr_GetMaxExtRxFifoFilterCount)(
        /* Select the interface. */
        uint8 iIdx,
        /* Into this variable the count will be stored */
        uint8* count);

    /**
     * This function disables the software based FIFO Frame ID filter
     * for the selected interface and controller.
     */
    typedef GtFr_ReturnType (* PF_FLEXI_GtFr_DisableExtRxFifoFilter)(
        /* Select the interface. */
        uint8 iIdx,
        /* Select the controller. */
        uint8 ctrlIdx);

    /**
     * This function enables the software based FIFO FrameID filter
     * for the selected interface and controller.
     */
    typedef GtFr_ReturnType (* PF_FLEXI_GtFr_EnableExtRxFifoFilter)(
        /* Select the interface. */
        uint8 iIdx,
        /* Select the controller. */
        uint8 ctrlIdx);

    /**
     * This function adds a software based FIFO Frame ID Filter to the selected interface.
     */
    typedef  GtFr_ReturnType (* PF_FLEXI_GtFr_SetExtRxFifoFilter)(
        /* select the inteface */
        uint8 iIdx,
        /* Configuration of the fifo FramId filter */
        GtFr_ExtRxFifoFilterCfgType filterCfg);


    /**
     * Complete pointers of flex-i API
     * collected in this struct
     */
    typedef struct tagFLEXI_POINTER_TABLE
    {
        PF_FLEXI_GtFr_EnableAsmResolver             GtFr_EnableAsmResolver;
        PF_FLEXI_GtFr_Init                          GtFr_Init;
        PF_FLEXI_GtFr_GetCtrlState                  GtFr_GetCtrlState;
        PF_FLEXI_GtFr_ReceiveEvent                  GtFr_ReceiveEvent;
        PF_FLEXI_GtFr_TransmitTxLPdu                GtFr_TransmitTxLPdu;
        PF_FLEXI_GtFr_TransmitTxLPduBlock           GtFr_TransmitTxLPduBlock;
        PF_FLEXI_GtFr_GetGlobalTime                 GtFr_GetGlobalTime;
        PF_FLEXI_GtFr_GetCtrlRegister               GtFr_GetCtrlRegister;
        PF_FLEXI_GtFr_InitComCtrl                   GtFr_InitComCtrl;
        PF_FLEXI_GtFr_InitTimer                     GtFr_InitTimer;
        PF_FLEXI_GtFr_StartTimer                    GtFr_StartTimer;
        PF_FLEXI_GtFr_ExecuteCommand                GtFr_ExecuteCommand;
        PF_FLEXI_GtFr_GetInterfaceCount             GtFr_GetInterfaceCount;
        PF_FLEXI_GtFr_GetInterfaceInfo              GtFr_GetInterfaceInfo;
        PF_FLEXI_GtFr_GetInterfaceExtInfo           GtFr_GetInterfaceExtInfo;
        PF_FLEXI_GtFr_ClockCommand                  GtFr_ClockCommand;
        PF_FLEXI_GtFr_GetClockTime                  GtFr_GetClockTime;
        PF_FLEXI_GtFr_DeInit                        GtFr_DeInit;
        PF_FLEXI_GtFr_GetCtrlConfig                 GtFr_GetCtrlConfig;
        PF_FLEXI_GtFr_GetProtConfig                 GtFr_GetProtConfig;
        PF_FLEXI_GtFr_GetMsgBufConfig               GtFr_GetMsgBufConfig;
        PF_FLEXI_GtFr_GetErayRegisterMap            GtFr_GetErayRegisterMap;
        PF_FLEXI_GtFr_GetEventQueueInfo             GtFr_GetEventQueueInfo;
        PF_FLEXI_GtFr_ClearEventQueue               GtFr_ClearEventQueue;
        PF_FLEXI_GtFr_EnableTransceiver             GtFr_EnableTransceiver;
        PF_FLEXI_GtFr_DisableTransceiver            GtFr_DisableTransceiver;
        PF_FLEXI_GtFr_FlashFirmwareMode             GtFr_FlashFirmwareMode;
        PF_FLEXI_GtFr_SetEventQueueNotification     GtFr_SetEventQueueNotification;
        PF_FLEXI_GtFr_SetEventQueueOverflow         GtFr_SetEventQueueOverflow;
        PF_FLEXI_GtFr_ReconfigureMsgBuf             GtFr_ReconfigureMsgBuf;
        PF_FLEXI_GtFr_GetMaxExtRxFifoFilterCount    GtFr_GetMaxExtRxFifoFilterCount;
        PF_FLEXI_GtFr_DisableExtRxFifoFilter        GtFr_DisableExtRxFifoFilter;
        PF_FLEXI_GtFr_EnableExtRxFifoFilter         GtFr_EnableExtRxFifoFilter;
        PF_FLEXI_GtFr_SetExtRxFifoFilter            GtFr_SetExtRxFifoFilter;
    } S_FLEXI_POINTER_TABLE;




#ifdef __cplusplus
}
#endif
