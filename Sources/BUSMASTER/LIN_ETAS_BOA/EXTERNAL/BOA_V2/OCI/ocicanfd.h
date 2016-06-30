#ifndef ETAS_OCI_CANFD_H
#define ETAS_OCI_CANFD_H

/**
* @file       ocicanfd.h
* @brief      OCI-CAN extension for the support of the Flexible Datarate CAN (CAN-FD) with up to 64 Byte palyoad per frame.
* @copyright  Copyright (c) 2007-2013 ETAS GmbH. All rights reserved.
*
* $Revision: 6694 $
*/

#include "ocibase.h"

#ifdef __cplusplus
extern "C" {
#endif

    /**
        @addtogroup GROUP_OCI_CAN_CONTROLLER OCI CAN
        @{
            @addtogroup GROUP_OCI_CAN_FD CAN-FD support
            BOA support for CAN-FD.

            @section Overview

            BOA models CAN-FD as a separate mode of the existing OCI_CAN controller concept:
            - An OCI_CAN controller publishes capabilities which describe its support (or lack of support) for CAN-FD.
            - If CAN-FD is supported, an OCI_CAN client provides CAN-FD configuration when he opens the OCI_CAN controller.
            - The OCI_CAN client uses dedicated message types to transmit and receive CAN-FD messages via the OCI_CAN
              controller.

            So, to use CAN-FD, a client must take these steps:
            -# Call @ref OCI_GetCANControllerCapabilities() and verify that @ref OCI_CANControllerCapabilities::canFDSupport
               indicates that CAN-FD is supported by the controller.
            -# Call @ref OCI_OpenCANController(), and specify values for the fields @ref OCI_CANConfiguration::canFDEnabled
               and @ref OCI_CANConfiguration::canFDConfig.
            -# Create a TX queue as usual, via @ref OCI_CreateCANTxQueue(), and transmit CAN-FD messages by calling
               @ref OCI_WriteCANDataEx(), using the message type @ref OCI_CANFDTX_MESSAGE.
            -# Create an RX queue as usual, via @ref OCI_CreateCANRxQueue(), and apply some filters via @ref OCI_AddCANFrameFilter().
            -# Receive CAN-FD messages in the usual manner, either via a callback specified when the queue was created, or
               by polling @ref OCI_ReadCANDataEx().

            BOA's support for CAN-FD also contains features to ease the transition from CAN 2.0 to CAN-FD. A client can
            send and receive "short" CAN-FD messages (i.e. <= 8 bytes payload) using the same OCI_CAN APIs and structures
            which he already uses for CAN 2.0 messages:
            - The field @ref OCI_CANFDConfiguration::canFdTxConfig allows the client to specify that when he calls
              @ref OCI_WriteCANData(), the provided CAN messages should be transmitted at the CAN-FD data bit rate.
            - The field @ref OCI_CANFDConfiguration::canFdRxConfig allows the client to specify that when the controller
              receives a CAN-FD message with <= 8 payload bytes, the message should be delivered to the client in exactly
              the same manner as a CAN 2.0 message.

            In this way, just changing the configuration provided to @ref OCI_OpenCANController() allows a client to
            immediately benefit from the CAN-FD data bit rate, without making any other changes.

            The new API functions and structure fields required to support CAN-FD were introduced in v1.3 of the OCI_CAN API.
            As with all changes to the BOA APIs, at first no devices or clients will support the new API version. Then
            gradually support will be added as new versions of devices and clients are released. It is important to note that
            "support for OCI_CAN v1.3" is not the same as "support for CAN-FD". OCI_CAN v1.3 just allows a client to ask a
            device whether it supports CAN-FD. The device may still say "no, I don't support CAN-FD".

            Backwards compatibility between a new OCI_CAN client and old OCI_CAN devices (and vice versa) must be considered:
            - A client must be prepared to find that a connected device does not support OCI_CAN v1.3.
            - A device OCD must be prepared to find that a connected client knows nothing about OCI_CAN v1.3.

            This matter is discussed further on the page @ref BackwardsCompatibilityPage. In summary:
            - A client must not access structure fields or call API functions which were introduced in OCI_CAN 1.3 unless he
              explicitly requested OCI_CAN v1.3 when he called @ref OCI_CreateCANControllerVersion() or
              @ref OCI_CreateCANControllerNoSearch().
            - A device OCD must not access structure fields or call API callbacks which were introduced in OCI_CAN 1.3 unless
              v1.3 of OCI_CAN was requested when the current OCI_CAN session was created.

            @{
    */

    /**
    * @anchor  ANCHOR_OCI_CANFD_MESSAGE_FLAGS
    * @name    CAN-FD message flags
    * @brief   Flags to specify additional information of a CAN-FD message.
    *          These are possible values of @ref OCI_CANFDRxMessage::flags.
    * @{
    */

    /**
        The message's payload uses the data bit rate defined for CAN-FD.
        Only possible for CAN-FD frames and therefore @ref OCI_CAN_MSG_FLAG_FD_DATA must also be set.
    */
#define OCI_CAN_MSG_FLAG_FD_DATA_BIT_RATE (8u)

    /**
        The message may exceed the data length supported by the CAN-FD controller.
        In this case the driver (and CAN-Controller) truncates the data to the supported length and
        replaces the truncated data with by padding bytes.
    */
#define OCI_CAN_MSG_FLAG_FD_TRUNC_AND_PAD (16u)

    /**
        The received message contains an indication that the transmitter (our peer) is in the
        "error passive" state. The Error State Indicator in the received CAN-FD message has a
        recessive level.

        (Normal operation is represented by a reset flag, peer in error passive state is
        represented by a set flag.)
    */
#define OCI_CAN_MSG_FLAG_FD_ERROR_PASSIVE (32u)

    /**
        The message uses the CAN-FD frame format.
    */
#define OCI_CAN_MSG_FLAG_FD_DATA (64u)

    /** @} */

    /**
    * @anchor  ANCHOR_OCI_CANFD_CAPABILITY_FLAGS
    * @name    CAN-FD capability flags
    * @brief   Flags to specify the CAN-FD capabilities of a OCI_CAN controller.
    *          These are possible values of @ref OCI_CANControllerCapabilities::canFDSupport.
    * @{
    */

    /**
        The OCI-CAN controller supports CAN-FD.
    */
#define OCI_CANFDSUPPORT (1<<0)

    /**
        The OCI-CAN controller supports the configuration of a tranceiver delay compensation.
    */
#define OCI_CANFDTX_DELAY_COMPENSATION_SUPPORT (1<<1)

    /** @} */

    /**
        This enum lists the possible ways in which CAN-FD messages can be received by a CAN-FD controller.
    */
    typedef enum
    {
        /** The controller does not receive CAN-FD messages. */
        OCI_CANFDRXMODE_CANFD_FRAMES_IGNORED = 1,

        /** The controller receives CAN-FD messages which have up to 8 bytes of payload. The messages are delivered
            to the OCI_CAN client as instances of @ref OCI_CANRxMessage (@ref OCI_CANRxMessage::dlc encodings of
            9..15 are not supported).
            This allows a client which previously used CAN 2.0 to easily be adapted to receive "short" CAN-FD messages. */
        OCI_CANFDRXMODE_CANFD_FRAMES_USING_CAN_MESSAGE = 2,

        /** The controller receives CAN-FD messages. The messages are delivered to the OCI_CAN client as instances of
            @ref OCI_CANFDRxMessage. */
        OCI_CANFDRXMODE_CANFD_FRAMES_USING_CANFD_MESSAGE = 4,

        /** The controller receives CAN-FD messages. The messages are delivered to the OCI_CAN client as instances of
            @ref OCI_CANFDRxMessage.
            If one such message has a payload which is longer than @ref OCI_CANControllerCapabilities::canFDMaxDataSize,
            the physical CAN controller is permitted to replace the payload bytes which exceeded its capabilities with
            padding data. */
        OCI_CANFDRXMODE_CANFD_FRAMES_USING_CANFD_MESSAGE_PADDING = 8,

    }
    OCI_CANFDRxMode;

    /**
        This enum lists the possible ways in which CAN 2.0 messages can be received by a CAN-FD controller.
    */
    typedef enum
    {
        /** The controller does not receive CAN 2.0 messages. */
        OCI_CAN_RXMODE_CAN_FRAMES_IGNORED = 1,

        /** The controller receives CAN 2.0 messages. The messages are delivered to the OCI_CAN client as instances
            of @ref OCI_CANRxMessage. */
        OCI_CAN_RXMODE_CAN_FRAMES_USING_CAN_MESSAGE = 2,
    } OCI_CANRxMode;

    /**
        This struct specifies how a CAN-FD controller receives CAN-FD and CAN 2.0 messages.
    */
    typedef struct OCI_CANFDRxConfig
    {
        /** Defines how normal CAN 2.0 messages are received.
            Only values indicated by @ref OCI_CANControllerCapabilities::canFDRxConfig_CANMode can be used. */
        OCI_CANRxMode canRxMode;

        /** Defines how CAN-FD messages are received.
            Only values indicated by @ref OCI_CANControllerCapabilities::canFDRxConfig_CANFDMode can be used. */
        OCI_CANFDRxMode canFdRxMode;

    } OCI_CANFDRxConfig;

    /**
        This enum lists the possible ways in which an OCI_CAN client can transmit messages using a CAN-FD controller.
    */
    typedef enum
    {
        /** If a client calls @ref OCI_WriteCANData() or @ref OCI_WriteCANDataEx() and provides a @ref OCI_CANTxMessage,
            the message is transmitted as a CAN 2.0 message.

            If a client calls @ref OCI_WriteCANDataEx() and provides a @ref OCI_CANFDTxMessage, the error
            @ref OCI_ERR_INCONSISTENT_PARAMETER_SET is returned. */
        OCI_CANFDTX_USE_CAN_FRAMES_ONLY = 1,

        /** If a client calls @ref OCI_WriteCANData() or @ref OCI_WriteCANDataEx() and provides a @ref OCI_CANTxMessage,
            the message is transmitted as a CAN-FD message with a payload <= 8 bytes (@ref OCI_CANTxMessage::dlc
            encodings of 9..15 are not supported). This allows a client which previously used CAN 2.0 to easily be
            adapted to transmit "short" CAN-FD messages.

            If a client calls @ref OCI_WriteCANDataEx() and provides a @ref OCI_CANFDTxMessage, the message
            is transmitted as a CAN-FD message. */
        OCI_CANFDTX_USE_CANFD_FRAMES_ONLY = 2,

        /** If a client calls @ref OCI_WriteCANData() or @ref OCI_WriteCANDataEx() and provides a @ref OCI_CANTxMessage,
            the message is transmitted as a CAN 2.0 message.

            If a client calls @ref OCI_WriteCANDataEx() and provides a @ref OCI_CANFDTxMessage, the message
            is transmitted as a CAN-FD message. */
        OCI_CANFDTX_USE_CAN_AND_CANFD_FRAMES = 4

    } OCI_CANFDTxConfig;

    /**
        This enum indicates the possible states of the tranceiver delay compensation functionality.
    */
    typedef enum
    {
        /** The transceiver delay compensation is disabled. */
        OCI_CANFDTX_DELAY_COMPENSATION_OFF = 0,

        /** The transceiver delay compensation is enabled. */
        OCI_CANFDTX_DELAY_COMPENSATION_ON = 1

    } OCI_CANFDTxDelayCompensation;

    /**
        Extended configuration structure for a CAN-FD controller
    */
    typedef struct OCI_CANFDConfiguration
    {
        /** The bitrate used for the data bytes of the CAN message. Only values up to
            @ref OCI_CANControllerCapabilities::canFDMaxDataRate can be used. */
        uint32 dataBitRate;

        /** The sample point in percent (0..100) at the data bit rate. */
        uint32 dataSamplePoint;

        /** The BTL cycles at the data bit rate */
        uint32 dataBTL_Cycles;

        /** The length of the sync segment at data bit rate. */
        uint32 dataSJW;

        /** This indicates whether transceiver delay compensation is active. @ref OCI_CANFDTX_DELAY_COMPENSATION_ON
            can be used only if indicated by @ref OCI_CANControllerCapabilities::canFDSupport. */
        OCI_CANFDTxDelayCompensation txDelayCompensationControl;

        /** The offset used for transceiver delay compensation, measured as a percentage of the data phase bit-time.
            This is only valid when @ref txDelayCompensationControl is set to @ref OCI_CANFDTX_DELAY_COMPENSATION_ON. */
        uint32 txSecondarySamplePointOffset;

        /** This configures how the controller receives CAN 2.0 and CAN-FD messages. */
        OCI_CANFDRxConfig canFdRxConfig;

        /** This configures how the controller transmits CAN messages provided by the OCI_CAN client.
            Only values indicated by @ref OCI_CANControllerCapabilities::canFDTxConfig_Mode can be used. */
        OCI_CANFDTxConfig canFdTxConfig;

    } OCI_CANFDConfiguration;


    /**
        This struct is used for receiving CAN-FD messages.

        @note In some circumstances it is also possible to receive "short" CAN-FD messages via @ref OCI_CANRxMessage;
              see @ref GROUP_OCI_CAN_FD for more details.

        @note The structure elements and memory layout are guaranteed to be identical to @ref OCI_CANRxMessage, except
              for the extended size of the data[] array.
              The fields @ref OCI_CANFDRxMessage::size and @ref OCI_CANRxMessage::dlc have equivalent semantics,
              despite their different names.
    */
    typedef struct OCI_CANFDRxMessage
    {
        /**
        * Global receive time stamp. Depending on the implementation, this may be
        * based on a hardware-internal timer with no global synchronization.
        * In particular, the epoch (i.e.\ value 0) may be the point in time the
        * hardware was switched on.
        */
        OCI_Time timeStamp;

        /** Filter tag ID. This ID is set by the definition of the RxFilter. */
        uint32 tag;

        /** CAN frame ID. */
        uint32 frameID;

        /** Additional information and settings of the CAN data message.

          @flags    The following flags are valid:
          @n        @ref OCI_CAN_MSG_FLAG_EXTENDED
          @n        @ref OCI_CAN_MSG_FLAG_REMOTE_FRAME
          @n        @ref OCI_CAN_MSG_FLAG_SELFRECEPTION
          @n        @ref OCI_CAN_MSG_FLAG_FD_DATA
          @n        @ref OCI_CAN_MSG_FLAG_FD_DATA_BIT_RATE
          @n        @ref OCI_CAN_MSG_FLAG_FD_TRUNC_AND_PAD The payload of the received frame exceeded the data length
                    supported by the physical CAN controller (and indicated in @ref OCI_CANControllerCapabilities::canFDMaxDataSize).
                    The physical CAN controller replaced the data that exceeeds its capabilities by padding data.
          @n        @ref OCI_CAN_MSG_FLAG_FD_ERROR_PASSIVE
          @sa       @ref ANCHOR_OCI_CAN_MESSAGE_FLAGS   "OCI CAN Message Flags" and
                    @ref ANCHOR_OCI_CANFD_MESSAGE_FLAGS "OCI CAN-FD Message Flags"
        */
        uint16 flags;

        /** Reserved data field. */
        uint8 res;

        /** Number of valid bytes in the @ref data array. This is the decoded value of the DLC field which was present
            in the received bus frame. The valid range is [0..8, 12, 16, 20, 24, 32, 48, 64]. */
        uint8 size;

        /** keep 64 Bit alignment for first data byte */
        uint8 res1[4];

        /** The payload of the frame. If @ref size is < 64, only the first @ref size bytes are valid.
            The payload may include padding information that may be different to the data that was placed
            on the CAN-Bus if the payload exceeds the OCI_CANControllerCapabilities::canFDMaxDataSize
            and the controller is configured with @ref OCI_CANFDRXMODE_CANFD_FRAMES_USING_CANFD_MESSAGE_PADDING.
         */
        uint8 data[64];

    } OCI_CANFDRxMessage;



    /**
        This struct is used for transmitting CAN-FD messages.

        @note In some circumstances it is also possible to transmit "short" CAN-FD messages via @ref OCI_CANTxMessage;
              see @ref GROUP_OCI_CAN_FD for more details.

        @note The structure elements and memory layout are guaranteed to be identical to @ref OCI_CANTxMessage, except
              for the size of the data[] array.
    */
    typedef struct OCI_CANFDTxMessage
    {
        /** CAN frame ID. */
        uint32 frameID;

        /**
          Additional information and settings of the CAN data message.

          @flags    The following flags are valid:
          @n        @ref OCI_CAN_MSG_FLAG_EXTENDED
          @n        @ref OCI_CAN_MSG_FLAG_REMOTE_FRAME
          @n        @ref OCI_CAN_MSG_FLAG_SELFRECEPTION
          @n        @ref OCI_CAN_MSG_FLAG_FD_DATA
          @n        @ref OCI_CAN_MSG_FLAG_FD_DATA_BIT_RATE
          @n        @ref OCI_CAN_MSG_FLAG_FD_TRUNC_AND_PAD If this flag is set, the physical CAN controller is required
                    to accept this CAN-FD message even if the payload exceeds the data length supported by the controller
                    (i.e. @ref size is greater than OCI_CANControllerCapabilities::canFDMaxDataSize). In such a case, the
                    physical CAN controller is permitted to replace the data that exceeeds its capabilities with padding
                    data. If this flag is not set, and the payload exceeds the data length supported by the controller,
                    the controller will reject this message.
          @sa       @ref ANCHOR_OCI_CAN_MESSAGE_FLAGS   "OCI CAN Message Flags" and
                    @ref ANCHOR_OCI_CANFD_MESSAGE_FLAGS "OCI CAN-FD Message Flags"
        */
        uint16 flags;

        /** Reserved data field. */
        uint8 res;

        /** Length of the payload in bytes. CAN-FD supports only 16 different payload sizes:
            0..8, 12, 16, 20, 24, 32, 48 or 64. */
        uint8 size;

        /** Payload; only the first @ref size bytes are valid. */
        uint8 data[64];

    } OCI_CANFDTxMessage;

    /** @} */
    /** @} */

#ifdef __cplusplus
}
#endif

#endif //ETAS_OCI_CANFD_H
