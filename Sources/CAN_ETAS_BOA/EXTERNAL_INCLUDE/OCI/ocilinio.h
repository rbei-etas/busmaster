#ifndef ETAS_OCI_LIN_IO_H
#define ETAS_OCI_LIN_IO_H

/**
* @file       ocilinio.h
* @brief      LIN specific part of the Open Controller Interface (OCI) API
*             excluding the strongly typed controller configuration.
* @copyright  Copyright (c) 2007-2008 ETAS GmbH. All rights reserved.
*
* $Revision: 4801 $
*/


#include "ocibase.h"

#ifdef __cplusplus
extern "C" {
#endif


/**
* @addtogroup GROUP_OCI_LIN_CONTROLLER OCI LIN
* @brief      LIN specific part of the Open Controller Interface (OCI). 
* @{
*/
/**
* @addtogroup GROUP_OCI_LIN_INITIALIZATION Initialization
* @{
*/


/**
* Definition for available LIN controller modes.
*/
typedef enum
{
    /**
    * This controller instance may act as a master on the LIN Bus.
    */
    OCI_LIN_MASTER_CAPABLE           = 1,

    /**
    * This controller instance may act as a slave on the LIN Bus.
    */
    OCI_LIN_SLAVE_CAPABLE            = 2,

    /**
    * This controller instance may act as a master or a slave on the LIN Bus.
    */
    OCI_LIN_MASTER_OR_SLAVE_CAPABLE  = 3
    
} OCI_LIN_CAPABILITY_CONTROLLER_MODE;


/**
* Definition for available LIN controller modes.
*/
typedef enum
{
    /**
    * LIN master resistor is not available.
    */
    OCI_LIN_MASTER_RESITOR_ABSENT    = 1,

    /**
    * LIN master resistor is present and may be turned off or on.
    */
    OCI_LIN_MASTER_RESITOR_PRESENT   = 2

} OCI_LIN_CAPABILITY_MASTER_RESISTOR;


/**
* Definition for supported LIN frame types.
*/
typedef enum
{
    /**
    * This controller supports all kinds of LIN frame types.
    * See @red OCI_LIN_FRAME_TYPE for list of frame types.
    */
    OCI_LIN_SUPPORT_ALL_FRAME_TYPES             = 1,

    /**
    * This controller supports only unconditional LIN frames.
    */
    OCI_LIN_SUPPORT_UNCONDITIONAL_FRAME_TYPE    = 2
    

} OCI_LIN_CAPABILITY_FRAME_TYPE;


/**
* Definition for available LIN frame transmission mode.
*/
typedef enum
{
    /**
    * LIN controller only able to receives frames.
    */
    OCI_LIN_RX_CAPABLE   = 1,

    /**
    * LIN controller only able to send frames.
    */
    OCI_LIN_TX_CAPABLE   = 2,

    /**
    * LIN controller able to send and receive frames.
    */
    OCI_LIN_TXRX_CAPABLE = 3

} OCI_LIN_CAPABILITY_TRANSMISSION_DIRECTION;


/**
* Definition for sleep mode.
*/
typedef enum
{
    /**
    * Controller instance is in sleep state.
    */
    OCI_LIN_SLEEP = 1,

    /**
    * Controller instance is in wake state.
    */
    OCI_LIN_WAKE  = 2

} OCI_LIN_SLEEP_MODE;


/**
* Definition for frame transmission direction.
*/
typedef enum
{
    /**
    * Frame is received from LIN bus.
    */
    OCI_LIN_FRAME_RX    = 1,

    /**
    * Frame is sent to LIN bus.
    */
    OCI_LIN_FRAME_TX    = 2,

    /**
    * Frame is sent and received to/from LIN bus, e.g.\ self-reception.
    */
    OCI_LIN_FRAME_TXRX  = 3,

    /**
    * Frame is not used.
    */
    OCI_LIN_FRAME_NOP   = 4

} OCI_LIN_TRANSMISSION_DIRECTION;

/**
* Definition for TX queue types.
*/
typedef enum
{
    /**
    * This queue type is able to send LIN frames.
    */
    OCI_LIN_FRAME_QUEUE     = 1,

    /**
    * This queue type is able to send all kinds of control data (e.g. LIN sleep mode control).
    */
    OCI_LIN_CONTROL_QUEUE   = 2,

    /**
    * This queue type is able to send LIN master requests.
    */                      
    OCI_LIN_MASTER_QUEUE    = 3

} OCI_LIN_TX_QUEUE_TYPE;

/**
* Definition for LIN frame transmission states.
*/
typedef enum
{
    /**
    * No LIN transmission has taken place.
    */
    OCI_LIN_TRANS_NONE            = 0,

    /**
    * LIN frame transmission successful.
    */
    OCI_LIN_TRANS_OK              = 1,

    /**
    * While LIN transmission bus level does not correspond to sent bit.
    */
    OCI_LIN_TRANS_ERR_BIT         = 2,
    
    /**
    * Checksum error in LIN frame.
    */
    OCI_LIN_TRANS_ERR_CRC         = 3,
   
    /**
    * Error in LIN frame header (LIN slave only).
    */
    OCI_LIN_TRANS_ERR_HEADER      = 4,
    
    /**
    * Frame transfer on LIN has not completed in time.
    * Typical reason for timeout is mismatch of actual frame length and
    * @ref OCI_LIN_PerIDConfiguration.dlc or no response at all in case of
    * @ref OCI_LIN_PerIDConfiguration.frameType equals @ref OCI_LIN_UNCONDITIONAL_FRAME.  
    */
    OCI_LIN_TRANS_ERR_TOUT        = 5,
    
    /**
    * Collision while transmission of an event triggered frame.
    */
    OCI_LIN_TRANS_COLLISION       = 6,
    
    /**
    * No response to LIN frame header (event triggered frame only).
    */
    OCI_LIN_TRANS_NO_RESPONSE     = 7,

    /**
    * LIN master scheduling does not pick up after wake up request (LIN slave only).
    */
    OCI_LIN_TRANS_NO_MSTR_SCHED   = 8,
    
    /**
    * No data available to be sent in LIN frame.
    */
    OCI_LIN_TRANS_NO_TX_DATA       = 9,

    /**
    * Nondescript error occurred while LIN frame transmission.
    */
    OCI_LIN_TRANS_ERR              = 10

} OCI_LIN_TRANSMISSION_STATE;

/**
* Definition for LIN frame transmission result.
*/
typedef struct OCI_LINTransmissionResult
{
    /** 
    * LIN frame ID of the last transmitted frame.
    * According to LIN specification, 
    * the LIN ID is used in the following way:
    * @li       0..59 - Transmittance of signals (messages).
    * @li       60 - LIN master diagnostic request.
    * @li       61 - LIN slave diagnostic response.
    * @li       62 - Reserved for future enhancement.
    * @li       63 - Reserved for future enhancement.
    * @li       @ref OCI_NO_VALUE - Unknown LIN frame ID.
    */
    uint32                frameID;

    /** 
    * Specifies the transmission direction of the last transmitted frame.
    * @values   @ref OCI_LIN_FRAME_RX
    * @n        @ref OCI_LIN_FRAME_TX
    * @n        @ref OCI_LIN_FRAME_TXRX
    * @n        @ref OCI_NO_VALUE, if the transmission direction is unknown.
    * @sa       @ref OCI_LIN_TRANSMISSION_DIRECTION
    *           "OCI LIN frame transmission directions"
    */
    uint32                direction;

    /** 
    * Specifies the length information (number of data bytes) of the last transmitted frame.
    * @li       15 - Number of bytes determined by LIN frame ID itself
    * @li       @ref OCI_NO_VALUE - Unknown length information.
    */
    uint32                dlc;

    /** 
    * Specifies the checksum calculation method of the last transmitted frame. 
    * @values   @ref OCI_LIN_CHECKSUM_CLASSIC
    * @n        @ref OCI_LIN_CHECKSUM_ENHANCED
    * @n        @ref OCI_NO_VALUE - Unknown checksum calculation method.
    * @sa       @ref OCI_LIN_CHECKSUM_TYPE
    *           "OCI LIN Checksum Calculation Methods"
    */
    uint32                crcType;

    /** 
    * Transmission result of the last transmitted frame.
    * @values   @ref OCI_LIN_TRANS_NONE
    * @n        @ref OCI_LIN_TRANS_OK
    * @n        @ref OCI_LIN_TRANS_ERR_BIT
    * @n        @ref OCI_LIN_TRANS_ERR_CRC
    * @n        @ref OCI_LIN_TRANS_ERR_HEADER
    * @n        @ref OCI_LIN_TRANS_ERR_TOUT     
    * @n        @ref OCI_LIN_TRANS_COLLISION    
    * @n        @ref OCI_LIN_TRANS_NO_RESPONSE  
    * @n        @ref OCI_LIN_TRANS_NO_MSTR_SCHED
    * @n        @ref OCI_LIN_TRANS_NO_TX_DATA   
    * @sa       @ref OCI_LIN_TRANSMISSION_STATE
    *           "OCI LIN transmission states"
    */
    uint32                state;

} OCI_LIN_TRANSMISSION_RESULT;


/**
* Specific structure for LIN controller capabilities.
* Static read-only information of a LIN controller.
* This information will never change.
*/
typedef struct OCI_LINControllerCapabilities
{
    /** 
    * Number of supported parallel virtual controller accesses, when all 
    * configurations use the same values. 
    */
    uint32 sharedAccess;

    /** 
    * Supported LIN controller mode @ref OCI_LIN_CAPABILITY_CONTROLLER_MODE
    * (LIN Master or Slave).
    */
    uint32 controllerMode;

    /** 
    * Supported generic LIN frame transmission direction
    * @ref OCI_LIN_CAPABILITY_TRANSMISSION_DIRECTION.
    */
    uint32 transmissionDirection;

    /** 
    * Availability of LIN master resistor
    * @ref OCI_LIN_CAPABILITY_MASTER_RESISTOR.
    */
    uint32 masterResistor;
    
    /** 
    * Supported LIN frame types
    * @ref OCI_LIN_CAPABILITY_FRAME_TYPE.
    */
    uint32 frameType;

} OCI_LINControllerCapabilities;


/**
* Specific structure for LIN controller properties.
*/
typedef struct OCI_LINControllerProperties
{
    /** 
    * Specifies if the controller interface instance is suspended or running
    * @ref OCI_ControllerPropertiesMode. 
    * @values      The following values are valid:
    * @n           @ref OCI_CONTROLLER_MODE_RUNNING
    * @n           @ref OCI_CONTROLLER_MODE_SUSPENDED 
    * @sa          @ref OCI_ControllerPropertiesMode
    *              "OCI Controller property mode values"
    */
    OCI_ControllerPropertiesMode mode;
     
} OCI_LINControllerProperties;


/**
 Query the capabilities of the LIN controller instance.

 The capabilities are static information, which will never change for a specific instance.

 @param[in]  controller   Handle of the LIN controller interface instance for which the capabilities are requested.
 @param[out] capabilities Pointer to a buffer receiving the requested capability information of the LIN 
                          controller.

 @controllerState CREATED, SUSPENDED, RUNNING.

 @return
   <TABLE border="0" rules="rows" cellspacing="3" cellpadding="3">  
   <tr> <td> @ref OCI_SUCCESS </td> 
        <td> The operation was successful. The @a capabilities are filled with valid information. </td> 
   </tr>
   <tr> <td> @ref OCI_ERR_INVALID_HANDLE </td>  
        <td> The @a controller handle is not valid. </td> 
   </tr>
   <tr> <td> @ref OCI_ERR_INVALID_PARAMETER </td>
        <td> The @a capabilities parameter is NULL or references an invalid address.</td> 
   </tr>
   <tr> <td> @ref OCI_ERR_DRIVER_NO_RESPONSE </td>
        <td> The driver for a removable hardware received no response to a request. The hardware may be removed. 
             It is not known wether the request is lost and the command not exectuted or the response is lost and
             the command was executed. The controller enters the @a DISCONNECTED state. All subsequent calls will 
             return @ref OCI_ERR_DRIVER_DISCONNECTED. 
             Internal resources are locked until OCI_DestroyLINController() for the controller is called. </td> 
   </tr>
   <tr> <td> @ref OCI_ERR_DRIVER_DISCONNECTED </td>
        <td> The driver for a removable hardware detected a communication problem (in asynchromous communication, 
             by event signalling or during a previous call to the same hardware) and is not able to communicate to 
             the hardware any more. 
             The state of the controller is @a DISCONNECTED. The command is not executed. All subsequent calls to 
             the same Controller will return @ref OCI_ERR_DRIVER_DISCONNECTED. Internal resources are locked until 
             OCI_DestroyLINController() for the controller is called.  </td> 
   </tr>
   <tr> <td> @ref OCI_ERR_OUT_OF_MEMORY </td>
        <td> The driver run out of memory. This error should only occur on heavy loaded systems with multiple 
             clients and large send and receive queues. The error may indicate a resource leak in the driver 
             implementation. When multiple application access the same resource (interface hardware or physical
             controller) the termination of a different application may return the resources required for this 
             instance. </td> 
   </tr>
  </TABLE> 
*/
#ifndef OCI_DYNAMICDLL
OCI_DECLSPEC OCI_ErrorCode 
OCI_CALL OCI_GetLINControllerCapabilities(
    OCI_ControllerHandle controller, 
    OCI_LINControllerCapabilities* capabilities);
#endif

typedef OCI_ErrorCode
(OCI_CALL *PF_OCI_GetLINControllerCapabilities)(
    OCI_ControllerHandle controller, 
    OCI_LINControllerCapabilities* capabilities);


/**
* @}
* @addtogroup GROUP_OCI_LIN_MESSAGES Messages
* @{
*/

/**
* Discriminator for the different LIN messages in the 
* @ref OCI_LINMessageData union 
*/
typedef enum OCI_LINMessageDataType
{
    /**
    * Message type is @ref OCI_LINRxMessage and the message was received.
    */
    OCI_LIN_RX_MESSAGE                      = 1,

    /**
    * Message type is @ref OCI_LINTxMessage and the message is to be sent.
    */
    OCI_LIN_TX_MESSAGE                      = 2,

    /**
    * Message type is @ref OCI_LINMasterRequestMessage 
    * This type is only defined for transmit messages, 
    * which can be written to message queue with @ref OCI_WriteLINMasterData().
    */
    OCI_LIN_REQUEST                         = 3,

    /**
    * Message type is @ref OCI_LINEventMessage
    */
    OCI_LIN_EVENT                           = 4,

    /**
    * Message type is @ref OCI_InternalErrorEventMessage
    */
    OCI_LIN_INTERNAL_ERROR_EVENT            = 5,

    /**
    * Message type is @ref OCI_TimerEventMessage
    */
    OCI_LIN_TIMER_EVENT                     = 6,

    /**
    * Message type is @ref OCI_QueueEventMessage
    */
    OCI_LIN_QUEUE_EVENT                     = 7,

    /**
    * Message type is @ref OCI_LINSleepModeControlMessage and the message is to be sent.
    */
    OCI_LIN_SLEEP_MODE_CONTROL              = 8


} OCI_LINMessageDataType;

/**
* @anchor  ANCHOR_OCI_LIN_MESSAGE_FLAGS
* @name    OCI LIN Message Flags
* @brief   Constants to specify the attributes of a LIN message. 
*          These constants are valid for @ref OCI_LINRxMessage.flags.
*/

/** @{ */
/** The LIN message was sent by the same HW port. */
#define OCI_LIN_MESSAGE_TX         0x01
/** @} */


/** 
* Specific structure for LIN frame messages, send direction.
*/
typedef struct OCI_LINTxMessage
{      
    /**
     *  reserved flags, must be 0.
     */
    uint32 flags;

    /** 
    * LIN frame payload.
    * Only the first @ref OCI_LIN_PerIDConfiguration.dlc elements
    * of the related LIN frame ID are valid.
    */
    uint8 data[8];
    
} OCI_LINTxMessage;

/** 
* Specific structure for a LIN master request, send direction.
*/
typedef struct OCI_LINMasterRequestMessage 
{      
    /**
    * LIN frame ID to be sent by LIN master.
    * According to LIN specification, 
    * the LIN ID is used in the following way:
    * @li       0..59 - Transmittance of signals (messages).
    * @li       60 - LIN master diagnostic request.
    * @li       61 - LIN slave diagnostic response.
    * @li       62 - Reserved for future enhancement.
    * @li       63 - Reserved for future enhancement.
    */
    uint32 frameID;
    
} OCI_LINMasterRequestMessage;


/**
* @anchor  ANCHOR_OCI_LIN_SLEEPMODE_FLAGS
* @name    OCI LIN Sleep Mode Flags
* @brief   The constants describe how the sleep mode of the LIN hardware 
*          is affected or changed.
*/

/** @{ */
/** The hardware is set into sleep state. */
#define OCI_LIN_SLEEPMODE_GOTOSLEEP  0x01
/** The hardware wakes up. Hardware enters wake state*/
#define OCI_LIN_SLEEPMODE_WAKEUP     0x02
/** The hardware is being kept alive. Hardware stays in wake state. */
#define OCI_LIN_SLEEPMODE_STAYALIVE  0x00
/** @} */


/** 
* Specific structure for LIN sleep mode control, send direction.
*/
typedef struct OCI_LINSleepModeControlMessage 
{      
    /** 
    * Specifies the sleep mode action.
    * @values   @ref OCI_LIN_SLEEPMODE_GOTOSLEEP
    * @n        @ref OCI_LIN_SLEEPMODE_WAKEUP
    * @n        @ref OCI_LIN_SLEEPMODE_STAYALIVE
    * @sa       @ref ANCHOR_OCI_LIN_SLEEPMODE_FLAGS
    *           "OCI LIN Sleep Mode Flags"
    */
    uint32 mode;
    
} OCI_LINSleepModeControlMessage;


/** 
* Specific structure for LIN frame messages, receive direction.
*/
typedef struct OCI_LINRxMessage
{      
    /** 
    * Global receive time stamp. Depending on the implementation, 
    * this may be based on a hardware-internal timer with no global 
    * synchronization. In particular, the epoch (i.e.\ value 0) may
    * be the point in time the hardware was switched on.
    */
    OCI_Time timeStamp;

    /**
    * LIN filter tag ID. This ID is set by the definition of the RxFilter.
    */
    uint32 tag;

    /** 
    * @values   @ref OCI_LIN_MESSAGE_TX
    * @sa       @ref ANCHOR_OCI_LIN_MESSAGE_FLAGS
    *           "OCI LIN Message Flags"
    */
    uint32 flags;
    
    /** 
    * LIN frame ID.
    * According to LIN specification, 
    * the LIN ID is used in the following way:
    * @li       0..59 - Transmittance of signals (messages).
    * @li       60 - LIN master diagnostic request.
    * @li       61 - LIN slave diagnostic response.
    * @li       62 - Reserved for future enhancement.
    * @li       63 - Reserved for future enhancement.
    */
    uint32 frameID;

    /** 
    * Specifies the checksum calculation method.
    * @values   @ref OCI_LIN_CHECKSUM_CLASSIC
    * @n        @ref OCI_LIN_CHECKSUM_ENHANCED
    * @sa       @ref OCI_LIN_CHECKSUM_TYPE
    *           "OCI LIN Checksum Calculation Methods"
    */
    uint32 crcType;
    
    /**
    * Specifies the length information (number of valid data bytes).
    * 15 - number of bytes determined by LIN frame ID itself
    */
    uint32 dlc;

    /**
    * Payload. Only the first @a dlc elements are valid.
    */
    uint8 data[8];
    
} OCI_LINRxMessage;


/** 
* @anchor  ANCHOR_OCI_LIN_WAKEUP_FLAGS
* @name    OCI LIN Wake-Up Flags
* @brief   The flags describes the properties of a 
*          LIN wake-up event.
*/

/** @{ */
/** The wake-up signal comes from the internal hardware. */
#define OCI_LIN_WAKEUP_INTERNAL   0x01
/** @} */


/** 
* Specific structure for a message, which is received,
* when a LIN node wakes up (comes out of the sleep state)
* or receives a wake up frame.
*/
typedef struct OCI_LINWakeUpEventMessage
{      
    /** 
    * Properties of the LIN wake-up event.
    * @flags    A combination of the following flags is possible:
    * @n        @ref OCI_LIN_WAKEUP_INTERNAL
    * @sa       @ref ANCHOR_OCI_LIN_WAKEUP_FLAGS
    *           "OCI LIN Wake-Up Flags"
    */
    uint32 flags;

} OCI_LINWakeUpEventMessage;


/** 
* Specific structure for a message, which is received,
* when the sleep mode state of a LIN node changes. 
* There can be different reasons to receive this message:
* @li After creating a controller instance, when a HW port is activated.
* @li After sending a wake-up signal.
* @li After receiving a LIN message the LIN master goes back into sleep state.
*/
typedef struct OCI_LINSleepModeEventMessage
{      
    /** 
    * Specifies the sleep mode event.
    * @values   @ref OCI_LIN_SLEEPMODE_GOTOSLEEP
    * @n        @ref OCI_LIN_SLEEPMODE_WAKEUP
    * @n        @ref OCI_LIN_SLEEPMODE_STAYALIVE
    * @sa       @ref ANCHOR_OCI_LIN_SLEEPMODE_FLAGS
    *           "OCI LIN Sleep Mode Flags"
    */
    uint32 mode;

} OCI_LINSleepModeEventMessage;

/** 
* Specific structure for a message concerning the transmission result 
* of the last transmitted LIN frame.
*/
typedef struct OCI_LINTransmissionStatusEventMessage
{      
    /** 
    * Contains the LIN frame ID of the transmitted frame. 
    * According to LIN specification, 
    * the LIN ID is used in the following way:
    * @li       0..59 - Transmittance of signals (messages).
    * @li       60 - LIN master diagnostic request.
    * @li       61 - LIN slave diagnostic response.
    * @li       62 - Reserved for future enhancement.
    * @li       63 - Reserved for future enhancement.
    * @li       @ref OCI_NO_VALUE - Unknown LIN frame ID.
    */
    uint32 frameID;

    /** 
    * Specifies the frame transmission direction of the transmitted frame.
    * @values   @ref OCI_LIN_FRAME_RX
    * @n        @ref OCI_LIN_FRAME_TX
    * @n        @ref OCI_LIN_FRAME_TXRX
    * @n        @ref OCI_NO_VALUE - Unknown transmission direction.
    * @sa       @ref OCI_LIN_TRANSMISSION_DIRECTION
    *           "OCI LIN frame transmission directions"
    */
    uint32 direction;
    
    /** 
    * Specifies the length information (number of data bytes) of the transmitted frame.
    * @li 15 - number of bytes determined by LIN frame ID itself. 
    * @li @ref OCI_NO_VALUE - Unknown length information.
    */
    uint32 dlc;

    /** 
    * Specifies the checksum calculation method of the transmitted frame.
    * @values   @ref OCI_LIN_CHECKSUM_CLASSIC
    * @n        @ref OCI_LIN_CHECKSUM_ENHANCED
    * @n        @ref OCI_NO_VALUE - Unknown checksum calculation method.
    * @sa       @ref OCI_LIN_CHECKSUM_TYPE
    *           "OCI LIN Checksum Calculation Methods"
    */
    uint32 crcType;

    /** 
    * Specifies the transmission result of the transmitted frame.
    * @values   @ref OCI_LIN_TRANS_NONE
    * @n        @ref OCI_LIN_TRANS_OK
    * @n        @ref OCI_LIN_TRANS_ERR_BIT
    * @n        @ref OCI_LIN_TRANS_ERR_CRC
    * @n        @ref OCI_LIN_TRANS_ERR_HEADER
    * @n        @ref OCI_LIN_TRANS_ERR_TOUT
    * @n        @ref OCI_LIN_TRANS_COLLISION    
    * @n        @ref OCI_LIN_TRANS_NO_RESPONSE  
    * @n        @ref OCI_LIN_TRANS_NO_MSTR_SCHED
    * @n        @ref OCI_LIN_TRANS_NO_TX_DATA   
    * @sa       @ref OCI_LIN_TRANSMISSION_STATE
    *           "OCI LIN transmission states"
    */
    uint32 state;

} OCI_LINTransmissionStatusEventMessage;

/** 
* Specific structure for a message concerning unsuccessful 
* synchronization to the LIN bus (e.g. wrong baud rate). 
*/
typedef struct OCI_LINErrSyncEventMessage
{      
    /** 
    * Contains the configured LIN baud rate in baud (1000 to 20000). 
    */
    uint32 baudrate;

} OCI_LINErrSyncEventMessage;

/** 
* Specific structure for a message concerning checksum
* calculation method of an unconfigured receive LIN frame.
* If an unconfigured frame is received, the hardware
* detects the according checksum calculation method by itself.
* The message occurs only one time for the according LIN ID.
*/
typedef struct OCI_LINCrcDetectEventMessage
{      
    /** 
    * Contains the LIN frame ID for the according checksum calculation method. 
    * According to LIN specification, 
    * the LIN ID is used in the following way:
    * @li       0..59 - Transmittance of signals (messages).
    * @li       60 - LIN master diagnostic request.
    * @li       61 - LIN slave diagnostic response.
    * @li       62 - Reserved for future enhancement.
    * @li       63 - Reserved for future enhancement.
    */
    uint32 frameID;

    /** 
    * Specifies the checksum calculation method.
    * @values   @ref OCI_LIN_CHECKSUM_CLASSIC
    * @n        @ref OCI_LIN_CHECKSUM_ENHANCED
    * @sa       @ref OCI_LIN_CHECKSUM_TYPE
    *           "OCI LIN Checksum Calculation Methods"
    */
    uint32 crcType;

} OCI_LINCrcDetectEventMessage;

/** 
* Specific structure for a message concerning data length
* of an unconfigured receive LIN frame.
* If an unconfigured frame is received, the hardware
* detects the according data length by itself.
* The message occurs only one time for the according LIN ID.
*/
typedef struct OCI_LINDlcDetectEventMessage
{      
    /** 
    * Contains the LIN frame ID for the according length information. 
    */
    uint32 frameID;

    /** 
    * Specifies the length information (number of data bytes).
    */
    uint32 dlc;

} OCI_LINDlcDetectEventMessage;

/** 
* Specific structure for a message signalling that data has been lost by the device. 
* Use this event only when the data was lost on the bus. If the data was lost due to
* queue overflow, use an instance of @ref OCI_QueueEventMessage instead.
*/
typedef struct OCI_LINErrDataLostEventMessage
{      
    /** 
    * Contains the number of the lost messages. 
    */
    uint32 lostMessageCount;

} OCI_LINErrDataLostEventMessage;

/**
* Event codes for LIN specific events.
*/
typedef enum OCI_LINEvent
{
    /**
    * Controller has woken up.
    */
    OCI_LIN_EVENT_WAKEUP                = 1,

    /**
    * Controller has changed its sleep mode state.
    */
    OCI_LIN_EVENT_SLEEP_MODE            = 2,

    /**
    * Frame transmission has terminated, transmission result available.
    */
    OCI_LIN_EVENT_TRANSMISSION_STATUS   = 3,

    /**
    * Baudrate/Sync error occurred.
    */
    OCI_LIN_ERR_SYNC                    = 4,

    /**
    * Checksum algorithm detected for unconfigured receive frame.
    */
    OCI_LIN_EVENT_CRC_DETECT            = 5,

    /**
    * Data length detected for unconfigured receive frame.
    */
    OCI_LIN_EVENT_DLC_DETECT            = 6,

    /**
    * Data lost error occurred. Use this event only when the data was lost on the bus.
    * If the data was lost due to queue overflow, use an instance of @ref OCI_QueueEventMessage
    * instead.
    */
    OCI_LIN_ERR_DATA_LOST               = 7

} OCI_LINEvent;

/**
* Union with eventCode specific data.
* Use eventCode for discrimination.
*/
typedef union OCI_LINEventUnion
{   
    /** 
    * Data of the message, which is received,
    * when a LIN node wakes up (comes out of sleep state)
    * or receives a wake up frame.
    * Use this part, if @ref OCI_LINEventMessage.eventCode is 
    * @ref OCI_LIN_EVENT_WAKEUP.
    */
    OCI_LINWakeUpEventMessage wakeupEvent;

    /** 
    * Data of the message, which is received,
    * when the sleep mode state of a LIN node changes.
    * Use this part, if @ref OCI_LINEventMessage.eventCode is 
    * @ref OCI_LIN_EVENT_SLEEP_MODE.
    */
    OCI_LINSleepModeEventMessage sleepModeEvent;

    /** 
    * Data of the message concerning the transmission status of a frame.
    * Use this part, if @ref OCI_LINEventMessage.eventCode is 
    * @ref OCI_LIN_EVENT_TRANSMISSION_STATUS.
    */
    OCI_LINTransmissionStatusEventMessage transmissionStatusEvent;

    /** 
    * Data of the message concerning a failed sync detection or baud rate 
    * mismatch.
    * Use this part, if @ref OCI_LINEventMessage.eventCode is 
    * @ref OCI_LIN_ERR_SYNC.
    */
    OCI_LINErrSyncEventMessage errSyncEvent;

    /** 
    * Data of the message concerning the checksum calculation method of an 
    * unconfigured receive frame. Use this part, if 
    * @ref OCI_LINEventMessage.eventCode is @ref OCI_LIN_EVENT_CRC_DETECT.
    */
    OCI_LINCrcDetectEventMessage crcDetectEvent;

    /** 
    * Data of the message concerning the data length of an unconfigured receive 
    * frame. Use this part, if @ref OCI_LINEventMessage.eventCode is 
    * @ref OCI_LIN_EVENT_DLC_DETECT.
    */
    OCI_LINDlcDetectEventMessage dlcDetectEvent;

    /** 
    * Data of the message concerning lost data. 
    * Use this part, if @ref OCI_LINEventMessage.eventCode is 
    * @ref OCI_LIN_ERR_DATA_LOST.
    */
    OCI_LINErrDataLostEventMessage errDataLostEvent;    

}OCI_LINEventUnion;

/**
* LIN-Bus specific events.
*
* @todo   The events for chip state, sync pulse, transceiver or timer 
*         are not taken into account when receiving a LIN message.
*         The structure @ref OCI_LINMessageData does not contain
*         suitable data fields.
*         
*/
typedef struct OCI_LINEventMessage
{
    /** 
    * Global receive time stamp. Depending on the implementation, 
    * this may be based on a hardware-internal timer with no global 
    * synchronization. In particular, the epoch (i.e.\ value 0) may
    * be the point in time the hardware was switched on.
    */
    OCI_Time timeStamp;

    /**
    * LIN filter tag ID. This ID is set by the definition of the RxFilter.
    */
    uint32 tag;

    /**
    * Code for the event @ref OCI_LINEvent that generated this message.
    */
    uint32 eventCode;

    /**
    * Destination of the event. The event message will use the combination 
    * of @ref OCI_EVENT_DESTINATION_INBAND and 
    * @ref OCI_EVENT_DESTINATION_CALLBACK that was 
    * registered for this event by calling OCI_AddLINEventFilter().
    */
    uint32 destination;

    /**
    * Union with eventCode specific data.
    * Use eventCode for discrimination.
    */
    OCI_LINEventUnion data;

} OCI_LINEventMessage;

/**
* Union of LIN messages.
*/
typedef union OCI_LINMessageData 
{
    /** 
    * Data of a LIN send message. 
    * Use this part, if @ref OCI_LINMessage.type is @ref OCI_LIN_TX_MESSAGE.
    */
    OCI_LINTxMessage txMessage;

    /** 
    * Data of a LIN master request. 
    * Use this part, if @ref OCI_LINMessage.type is @ref OCI_LIN_REQUEST.
    */
    OCI_LINMasterRequestMessage masterRequestMessage;

    /** 
    * Data of a LIN receive message. 
    * Use this part, if @ref OCI_LINMessage.type is @ref OCI_LIN_RX_MESSAGE.
    */
    OCI_LINRxMessage rxMessage;

    /** 
    * Data of a LIN event message. 
    * Use this part, if @ref OCI_LINMessage.type is @ref OCI_LIN_EVENT.
    */
    OCI_LINEventMessage linEventMessage;
    /** 
    * Data of a LIN error event message. 
    * Use this part, if @ref OCI_LINMessage.type is 
    * @ref OCI_LIN_INTERNAL_ERROR_EVENT.
    */
    OCI_InternalErrorEventMessage internalErrorEventMessage;

    /** 
    * Data of a LIN timer event message. 
    * Use this part, if @ref OCI_LINMessage.type is @ref OCI_LIN_TIMER_EVENT.
    */
    OCI_TimerEventMessage   timerEventMessage;

    /** 
    * Data of a LIN queue event message. 
    * Use this part, if @ref OCI_LINMessage.type is @ref OCI_LIN_QUEUE_EVENT.
    */
    OCI_QueueEventMessage   queueEventMessage;

    /** 
    * Data of a LIN sleep mode control message. 
    * Use this part, if @ref OCI_LINMessage.type is @ref OCI_LIN_SLEEP_MODE_CONTROL.
    */
    OCI_LINSleepModeControlMessage sleepModeControlMessage;

} OCI_LINMessageData;


/**
* Container for a discriminated union passed to the OCI_WriteLINData() 
* and OCI_ReadLINData() function.
*/
typedef struct OCI_LINMessage
{    
    /**
    * Discriminator @ref OCI_LINMessageDataType of the OCI_LIN_MessageData. 
    */
    uint32 type;

	/** reserved parameter necessary for 64 Bit alignment of OCI_LINMessageData */
	uint32 reserved;

    /**
    * Data of the message dependent on the @a type of the message. 
    */
    OCI_LINMessageData data;

} OCI_LINMessage;


/**
* @}
* @addtogroup GROUP_OCI_LIN_QUEUES Queues
* @{
*/


/** 
* Structure containing the configuration of a LIN transmit queue. This 
* structure must be filled in, when @ref OCI_CreateLINTxQueue is called. The
* settings of this configuration are valid for the life-time of the queue.
*/
typedef struct OCI_LINTxQueueConfiguration
{
    /**
    * Contains the type of the TX queue specified in @ref OCI_LIN_TX_QUEUE_TYPE.
    * It specifies the kind of messages the queue is able to send. 
    */
    uint32 type;

    /** 
    * Contains the LIN frame ID of the unconditional LIN frame to be sent via this queue.
    * Only relevant in case of @a type is @ref OCI_LIN_FRAME_QUEUE.
    */
    uint32 frameID;

    /** 
    * TX event triggered LIN frames which are assigned to
    * this TX unconditional LIN frame to be sent via this queue.
    * This unconditional LIN frame will also be sent if one of the assigned event
    * triggered LIN frames is scheduled.
    * Event triggered frames must only range from LIN frame ID 0 to 59.
    * A LIN mater node must not use event triggered frames in send direction.
    * Thus this parameter must be set to 0x00000000.
    * Parameter only relevant in case of @a type is @ref OCI_LIN_FRAME_QUEUE.
    * Each bit represents an event triggered LIN frame. LSB equals LIN frame ID 0,
    * MSB equals LIN frame ID 31. If a bit is set respective event triggered LIN frame
    * is assigned to this unconditional LIN frame.
    *
    * @li       0x00000001..0xFFFFFFFF - Assigned event triggered LIN frames with ID 0 to 31.
    * @li       0x00000000             - No assignment between event triggered and
    *                                    unconditional LIN frame.
    * @default  0x00000000
    */ 
    uint32 assignedEventFrameID0to31;

    /** 
    * TX event triggered LIN frames which are assigned to
    * this TX unconditional LIN frame to be sent via this queue.
    * This unconditional LIN frame will also be sent if one of the assigned event
    * triggered LIN frames is scheduled.
    * Event triggered frames must only range from LIN frame ID 0 to 59.
    * A LIN mater node must not use event triggered frames in send direction.
    * Thus this parameter must be set to 0x00000000.
    * Parameter only relevant in case of @a type is @ref OCI_LIN_FRAME_QUEUE.
    * Each bit represents an event triggered LIN frame. LSB equals LIN frame ID 32,
    * MSB equals LIN frame ID 64. If a bit is set respective event triggered LIN frame
    * is assigned to this unconditional LIN frame. Bits for LIN IDs 60 to 63 must be 0.
    *
    * @li       0x00000001..0x0FFFFFFF - Assigned event triggered LIN frames with ID 32 to 59.
    * @li       0x00000000             - No assignment between event triggered and
    *                                    unconditional LIN frame.
    * @default  0x00000000
    */ 
    uint32 assignedEventFrameID32to59;

} OCI_LINTxQueueConfiguration;

/** 
  Create a LIN transmit queue. 
  
  A transmit queue is necessary to transmit frames or commands for a specific LIN controller instance.
  The kind of data a transmit queue is able to transmit is specified by the queue type in
  @ref OCI_LINTxQueueConfiguration.
  LIN controllers require an individual transmit queue for each unconditional LIN frame.
  Check with @ref OCI_LINControllerCapabilities whether controller is able to send LIN frames.

  @controllerState SUSPENDED, RUNNING.

  @param[in]  controller     The handle of the controller instance. 
  @param[in]  configuration  The configuration of the transmit queue. 
  @param[out] queue          Receives the handle of the newly created transmit queue. 

  @return
   <TABLE border="0" rules="rows" cellspacing="3" cellpadding="3">  
   <tr> <td> @ref OCI_SUCCESS </td> 
        <td> The operation was successful. </td> 
   </tr>
   <tr> <td> @ref OCI_ERR_INVALID_HANDLE </td>  
        <td> The @a controller handle is not valid. </td> 
   </tr>
   <tr> <td> @ref OCI_ERR_INVALID_PARAMETER </td>
        <td> The @a queue parameter is NULL or references an invalid object.</td> 
   </tr>
   <tr> <td> @ref OCI_ERR_INVALID_STATE </td>
        <td> The controller was not opened. The controller is not in the @a RUNNING or @a SUSPENDED state and
             can not be closed. </td> 
   </tr>
   <tr> <td> @ref OCI_ERR_DRIVER_NO_RESPONSE </td>
        <td> The driver for a removable hardware received no response to a request. The hardware may be removed. 
             It is not known wether the request is lost and the command not exectuted or the response is lost and
             the command was executed. The controller enters the @a DISCONNECTED state. All subsequent calls will 
             return @ref OCI_ERR_DRIVER_DISCONNECTED. 
             Internal resources are locked until OCI_DestroyLINController() for the controller is called. </td> 
   </tr>
   <tr> <td> @ref OCI_ERR_DRIVER_DISCONNECTED </td>
        <td> The driver for a removable hardware detected a communication problem (in asynchromous communication, 
             by event signalling or during a previous call to the same hardware) and is not able to communicate to 
             the hardware any more. 
             The state of the controller is @a DISCONNECTED. The command is not executed. All subsequent calls to 
             the same Controller will return @ref OCI_ERR_DRIVER_DISCONNECTED. Internal resources are locked until 
             OCI_DestroyLINController() for the controller is called.  </td> 
   </tr>
   <tr> <td> @ref OCI_ERR_OUT_OF_MEMORY </td>
        <td> The driver run out of memory. This error should only occur on heavy loaded systems with multiple 
             clients and large send and receive queues. The error may indicate a resource leak in the driver 
             implementation. When multiple application access the same resource (interface hardware or physical
             controller) the termination of a different application may return the resources required for this 
             instance. </td> 
   </tr>
   <tr> <td> @ref OCI_ERR_UNEXPECTED_NULL </td>
        <td> An unexpected implementation error occured. </td> 
   </tr>
  </TABLE> 

   @sa         @ref GROUP_OCI_CONTROLLER "Controller Handling"

*/

#ifndef OCI_DYNAMICDLL
OCI_DECLSPEC OCI_ErrorCode
OCI_CALL OCI_CreateLINTxQueue(
    OCI_ControllerHandle controller,
    OCI_LINTxQueueConfiguration* configuration,
    OCI_QueueHandle* queue);
#endif

typedef OCI_ErrorCode
(OCI_CALL *PF_OCI_CreateLINTxQueue)(
    OCI_ControllerHandle controller,
    OCI_LINTxQueueConfiguration* configuration,
    OCI_QueueHandle* queue);


/** 
  Queue Data for transmission on a LIN Bus Interface. 

  The routine returns, when all frames can be queued. Since the transmit queue has a limited capacity, 
  this function may need to block until the last entry could be placed into the queue. A @ref timeout
  can be specified to limit the blocking.
  
  There is no information, when the frames are send on the LIN bus. There may be a delay between storing the 
  frames in the queue and sending them, because of the scheduling algorithm in the LIN master. Some 
  fatal error on the LIN-Bus may occur before the frames can be send. 

  @todo consistent behavior?

  If an error occurs before all frames could be placed into the controller transmit queue, this function will 
  return immediately with an error code. It will not try to push the remaining messages into the queues. Already 
  queued entries will not be discarded.

  @controllerState SUSPENDED, RUNNING. \n
                   When the controller is in the CREATED state no valid Queue can exist and the call will return
                   @ref OCI_ERR_INVALID_HANDLE.

  @param[in]  queue     The handle of the @a queue to write to. 
  @param[in]  timeout   Maximum time (delta, not global time stamp) to wait for messages to be queued.
                        The timeout uses the same resolution as the controller timestamp. The resolution may vary
                        between different driver for different hardware. The interval may be increased to the time
                        slice(s) used by the Operating System and jitter in this magnitude.
                        Use @ref OCI_NO_TIME to wait indefinitely if necessary. 
                        0 will queue as many messages as possible without waiting for them to be transmitted.
  @param[in]  messages  A pointer to an array of <@a size>
                        @ref OCI_LINMessage elements. May be @c NULL, if @a size is 0.
  @param[in]  size      The number of entries in the @a messages array. 0 is an allowed value here.
  @param[out] remaining Number of entries not processed. Will be smaller or equal to @a size. 
                        May be @c NULL.

  @return     
  <TABLE border="0" rules="rows" cellspacing="3" cellpadding="3">  
   <tr> <td> @ref OCI_SUCCESS </td> 
        <td> The operation was successful. </td> 
   </tr>
   <tr> <td> @ref OCI_ERR_INVALID_HANDLE </td>  
        <td> The @a queue handle is not valid. </td> 
   </tr>
   <tr> <td> @ref OCI_ERR_INCONSISTENT_PARAMETER_SET </td> 
        <td> At least one message has an invalid format. 
             (e.g.\ messages[i].OCI_LINMessageDataType != OCI_LIN_TX_MESSAGE). </td> 
   </tr>
   <tr> <td> @ref OCI_ERR_DRIVER_NO_RESPONSE </td>
        <td> The driver for a removable hardware received no response to a request. The hardware may be removed. 
             It is not known wether the request is lost and the command not exectuted or the response is lost and
             the command was executed. The controller enters the @a DISCONNECTED state. All subsequent calls will 
             return @ref OCI_ERR_DRIVER_DISCONNECTED. 
             Internal resources are locked until OCI_DestroyLINController() for the controller is called. </td> 
   </tr>
   <tr> <td> @ref OCI_ERR_DRIVER_DISCONNECTED </td>
        <td> The driver for a removable hardware detected a communication problem (in asynchromous communication, 
             by event signalling or during a previous call to the same hardware) and is not able to communicate to 
             the hardware any more. 
             The state of the controller is @a DISCONNECTED. The command is not executed. All subsequent calls to 
             the same Controller will return @ref OCI_ERR_DRIVER_DISCONNECTED. Internal resources are locked until 
             OCI_DestroyLINController() for the controller is called.  </td> 
   </tr>
   <tr> <td> @ref OCI_ERR_OUT_OF_MEMORY </td>
        <td> The driver run out of memory. This error should only occur on heavy loaded systems with multiple 
             clients and large send and receive queues. The error may indicate a resource leak in the driver 
             implementation. When multiple application access the same resource (interface hardware or physical
             controller) the termination of a different application may return the resources required for this 
             instance. </td> 
   </tr>
   <tr> <td> @ref OCI_ERR_INVALID_PARAMETER </td>
        <td> The count exceeds the maximum number of messages, that can be queued. This is a fatal driver internal 
             error, because the driver should fragment the call to small enough sequences and block, until all 
             fragmented messages are queued or send.</td> 
   </tr>
   <tr> <td> @ref OCI_ERR_QUEUE_IS_FULL </td>
        <td> The specified @a queue is full; at least one message could not be added to the queue. </td> 
   </tr>
   </TABLE> 

* @sa         @ref GROUP_OCI_CONTROLLER "Controller Handling" 
*/

#ifndef OCI_DYNAMICDLL
OCI_DECLSPEC OCI_ErrorCode
OCI_CALL OCI_WriteLINData(
    OCI_QueueHandle queue,
    OCI_Time timeout, 
    OCI_LINMessage messages[], 
    uint32 size,
    uint32 *remaining);
#endif

typedef OCI_ErrorCode
(OCI_CALL *PF_OCI_WriteLINData)(
    OCI_QueueHandle queue,
    OCI_Time timeout, 
    OCI_LINMessage messages[], 
    uint32 size,
    uint32 *remaining);


/**
 Destroy a LIN transmit queue.

 The transmission Queue of a LIN controller is destroyed and all resources used by the queue are released. If the
 queue is not empty and some frames are waiting for transmission the frames are silently discarded. If the 
 application wants to be sure, that all frames are send before the queue is destroyed it can use the 
 OCI_GetQueueStatus() command to wait for an empty queue before calling this command.
 Check with @ref OCI_LINControllerCapabilities whether controller is able to send LIN frames.
 
 @controllerState SUSPENDED, RUNNING. \n
                   When the controller is in the CREATED state no valid Queue can exist and the call will return
                   @ref OCI_ERR_INVALID_HANDLE.  \n
                   When the controller is in the DISCONNECTED state the function may be called to do local clean
                   up. Only a limited plausibility check may be done in this case.  


 @note Frames already stored in the send register of a LIN controller, but not requested from the master on the
       Bus may be sent, when the ID is requested, even when this call already returned and the send
       queue is invalid.

 @param[in] queue The handle of the queue to destroy. 
                  The handle becomes invalid after this call and must not be used any longer. 
    

 @return
   <TABLE border="0" rules="rows" cellspacing="3" cellpadding="3">  
   <tr> <td> @ref OCI_SUCCESS </td> 
        <td> The operation was successful. </td> 
   </tr>
   <tr> <td> @ref OCI_ERR_INVALID_HANDLE </td>  
        <td> The @a queue handle is not valid. </td> 
   </tr>
   <tr> <td> @ref OCI_ERR_INVALID_STATE </td>
        <td> The controller is in the suspended state. No Data can be written to the queue. Set the controller to 
             the running state by OCI_SetLINControllerProperties() before calling OCI_WriteLINData(). </td> 
   </tr>
   <tr> <td> @ref OCI_ERR_OUT_OF_MEMORY </td>
        <td> The driver run out of memory. This error should only occur on heavy loaded systems with multiple 
             clients and large send and receive queues. The error may indicate a resource leak in the driver 
             implementation. When multiple application access the same resource (interface hardware or physical
             controller) the termination of a different application may return the resources required for this 
             instance. </td> 
   </tr>
   <tr> <td> @ref OCI_ERR_UNEXPECTED_NULL </td>
        <td> An unexpected implementation error occurred. </td> 
   </tr>
  </TABLE>

  @sa         @ref GROUP_OCI_CONTROLLER "Controller Handling"  
*/
#ifndef OCI_DYNAMICDLL
OCI_DECLSPEC OCI_ErrorCode 
OCI_CALL OCI_DestroyLINTxQueue(
    OCI_QueueHandle queue);
#endif

typedef OCI_ErrorCode
(OCI_CALL *PF_OCI_DestroyLINTxQueue)(
    OCI_QueueHandle queue);


/**
 Signature of a callback function for LIN event and message handling. The callback function registered by the
 user must have this signature to receive LIN Frames and LIN errors.

 @param[in] userData Pointer to a user defined structure or handle for user supplied information. Any callback
                     of a single instance will use the same value, that is supplied during the callback registration
                     in the @ref OCI_LINRxCallback structure during execution of OCI_CreateLINRxQueue().
 @param[in] msg      Pointer to the received message or event. The object is owned by the caller of the API. The
                     reference must not used after completion of the callback. When the user requires access to the
                     data after completion of the callback the callback must create a copy of the data.
                     
                     @todo verify implementation of resource ownership and description!

 @todo 2009-01-28: define OCI_DOCU to expand OCI_CALLBACK to nothing --> correct signature
*/
typedef void (OCI_CALLBACK *OCI_LINRxCallbackFunction)(void *userData, OCI_LINMessage* msg);

/** 
* Specific structure to define a LIN RX Callback. 
*/
typedef struct OCI_LINRxCallback
{
    /**
    * Callback function pointer
    */
    OCI_LINRxCallbackFunction function;

    /**
    * Opaque context information that will be passed to the callback 
    * @a function. The pointer is supplied by the caller and not 
    * interpreted or modified by the OCD implementation. 
    * May be @c NULL.
    */
    void *userData;

} OCI_LINRxCallback;


/**
* Configuration message for a LIN receive queue. This structure
* must be filled in, when OCI_CreateLINRxQueue() is called. The
* settings of this configuration are valid for the lifetime of the
* queue.
*/
typedef struct OCI_LINRxQueueConfiguration {
    /** 
    * Callback function for the reception of a LIN frame.
    *
    * When a frame arrives which matches one of the frame filters defined for the queue instance,
    * there are two possibilities:
    *   - If @ref onFrame is not @c NULL, the frame is passed to @ref onFrame.
    *   - If @ref onFrame is @c NULL, the frame is added to the RX queue.
    *
    * Note that if @ref OCI_DestroyLINRxQueue() is used to destroy the queue while reception is in progress,
    * it is possible for @ref onFrame to be called a short time @em after OCI_DestroyLINRxQueue() has returned
    * successfully.
    *
    * @sa @ref PAGE_OCI_RECEIVE_MESSAGES "Receive Messages"
    */
    OCI_LINRxCallback onFrame;

    /** 
    * Callback function for the reception of an event.
    *
    * When an event is fired which matches one of the event filters defined for the queue instance,
    * there are several possibilities:
    *   - If @ref onEvent is not @c NULL, and if the @c destination member of the matched filter specifies
    *     @ref OCI_EVENT_DESTINATION_CALLBACK, then the event is passed to @ref onEvent.
    *   - If the @c destination member of the matched filter specifies @ref OCI_EVENT_DESTINATION_INBAND,
    *     then the event is delivered to the application in the same way as a data frame.
    *   - If @ref onEvent is @c NULL, then the event is always delivered to the application in the
    *     same way as a data frame.
    *
    * Note:
    *   - The @c destination member of a filter can specify both @ref OCI_EVENT_DESTINATION_CALLBACK and
    *     @ref OCI_EVENT_DESTINATION_INBAND. In other words, the first two possibilities above are not mutually
    *     exclusive.
    *   - See @ref onFrame for a description of how received data frames are delivered to the application.
    *   - If @ref OCI_DestroyLINRxQueue() is used to destroy the queue while reception is in progress, it is
    *     possible for @ref onEvent to be called a short time @em after OCI_DestroyLINRxQueue() has returned
    *     successfully.
    *   - Setting only @ref onEvent but not @ref onFrame allows a lazy reaction on frame reception and a
    *     quick reaction on every enabled event.
    *
    * @sa   @ref PAGE_OCI_RECEIVE_MESSAGES "Receive Messages"
    * @sa   OCI_QueueEventFilter
    */
    OCI_LINRxCallback onEvent;

    /**
    * Specify whether self-receipted frames shall be forwarded to this queue.
    * @note        This feature cannot be deactivated in this BOA version.
    * @values      The following values are valid:
    * @n           OCI_SELF_RECEPTION_ON
    * @sa          @ref OCI_SelfReceptionMode
    *              "OCI Self Reception Mode"
    */
    OCI_SelfReceptionMode selfReceptionMode;

} OCI_LINRxQueueConfiguration;



/**
  Create a a receive queue or register callback functions for frame reception for a specific LIN controller. 
  
  LIN controller allow a single receive queue per controller. The configuration parameter allows the
  registration of a callback function for frames and a callback function for events. When these callbacks are
  set to NULL the data will be queued and must be read by OCI_ReadLINData(). When the callbacks are set
  the data will not be stored in the queue, but a reference is passed to the callback function whenever a frame
  is received. 

  All Frames and Events must pass a white filter to be stored in the queue. The filter conditions must be set
  by OCI_AddLINFrameFilter()and OCI_AddLINEventFilter(). After creation of a receive 
  queue the queue will not receive any data, because no white filter is active and all data is blocked.

  @controllerState SUSPENDED, RUNNING. \n
                   When the controller is in the CREATED state no valid Queue can exist and the call will return
                   @ref OCI_ERR_INVALID_HANDLE.

  @param[in]  controller     Handle of the controller instance. 
  @param[in]  configuration  Pointer to the buffer that contains the configuration structure. 
  @param[out] queue          Handle of the newly created receive queue. 

   @return
   <TABLE border="0" rules="rows" cellspacing="3" cellpadding="3">  
   <tr> <td> @ref OCI_SUCCESS </td> 
        <td> The operation was successful. 
             The @a queue handle is valid and must be used in any subsequent call accessing the queue. </td> 
   </tr>
   <tr> <td> @ref OCI_ERR_INVALID_HANDLE </td>  
        <td> The @a controller handle is not valid. </td> 
   </tr>
   <tr> <td> @ref OCI_ERR_INVALID_PARAMETER </td>
        <td> The configuration parameter or queue parameter is NULL or references to an invalid address.</td> 
   </tr>
   <tr> <td> @ref OCI_ERR_DRIVER_NO_RESPONSE </td>
        <td> The driver for a removable hardware received no response to a request. The hardware may be removed. 
             It is not known wether the request is lost and the command not exectuted or the response is lost and
             the command was executed. The controller enters the @a DISCONNECTED state. All subsequent calls will 
             return @ref OCI_ERR_DRIVER_DISCONNECTED. 
             Internal resources are locked until OCI_DestroyLINController() for the controller is called. </td> 
   </tr>
   <tr> <td> @ref OCI_ERR_DRIVER_DISCONNECTED </td>
        <td> The driver for a removable hardware detected a communication problem (in asynchromous communication, 
             by event signalling or during a previous call to the same hardware) and is not able to communicate to 
             the hardware any more. 
             The state of the controller is @a DISCONNECTED. The command is not executed. All subsequent calls to 
             the same Controller will return @ref OCI_ERR_DRIVER_DISCONNECTED. Internal resources are locked until 
             OCI_DestroyLINController() for the controller is called.  </td> 
   </tr>
   <tr> <td> @ref OCI_ERR_OUT_OF_MEMORY </td>
        <td> The driver run out of memory. This error should only occur on heavy loaded systems with multiple 
             clients and large send and receive queues. The error may indicate a resource leak in the driver 
             implementation. When multiple application access the same resource (interface hardware or physical
             controller) the termination of a different application may return the resources required for this 
             instance. </td> 
   </tr>
   <tr> <td> @ref OCI_ERR_UNEXPECTED_NULL </td>
        <td> An unexpected implementation error occurred. </td> 
   </tr>
  </TABLE> 
 */

#ifndef OCI_DYNAMICDLL
OCI_DECLSPEC OCI_ErrorCode 
OCI_CALL OCI_CreateLINRxQueue(
    OCI_ControllerHandle controller, 
    OCI_LINRxQueueConfiguration * configuration,
    OCI_QueueHandle * queue); 
#endif

typedef OCI_ErrorCode
(OCI_CALL *PF_OCI_CreateLINRxQueue)(
    OCI_ControllerHandle controller, 
    OCI_LINRxQueueConfiguration * configuration,
    OCI_QueueHandle * queue); 


/** 
  Read entries from a receive queue or wait for a timeout when less than requested entries are available. 
  
  The returned number of entries may be less than the requested number. A non blocking call can be executed,
  when @a timeout is set to 0. Even when waiting a infinite time for the entries the function may return with
  less data. 

  This function should deliver as long queued data as possible. As long as the integrity of the returned data 
  can be guaranteed the function will not return any error, even when the controller or some part of the driver
  entered an error state. The function should not block, when the driver is in error state. In this case it
  delivers the remaining data without an error code or - when empty - the error code.

  @controllerState SUSPENDED, RUNNING. \n
                   When the controller is in the CREATED state no valid Queue can exist and the call will return
                   @ref OCI_ERR_INVALID_HANDLE.

  @param[in]  queue      Handle of the receive queue.
  @param[in]  timeout    Maximum time (delta, not global time stamp) to wait at the queue to receive 
                         @a size entries. 
                         Use @ref OCI_NO_TIME to wait indefinitely if necessary. 
                         0 will fetch the queue's current content.
  @param[out] messages   Pointer to an array of @a size @ref OCI_LINMessage elements to receive 
                         the entries from the receive queue. 
                         Must not be @c NULL.
  @param[in]  size       Number of entries allocated in the @a messages array. 
                         Must not be 0.
  @param[out] count      Number of entries actually read with 0 <= @a count <= @a size. 
                         Must not be @c NULL.
  @param[out] remaining  Number of entries still in the queue after moving @a count into @a messages. 
                         May be @c NULL.

  @return     
  <TABLE border="0" rules="rows" cellspacing="3" cellpadding="3">  
   <tr> <td> @ref OCI_SUCCESS </td> 
        <td> The operation was successful. </td> 
   </tr>
   <tr> <td> @ref OCI_ERR_INVALID_HANDLE </td>  
        <td> The @a queue handle is not valid. </td> 
   </tr>
   <tr> <td> @ref OCI_ERR_INVALID_PARAMETER </td>
        <td> The @a messages or count parameter is NULL or references to an invalid address.</td> 
   </tr>
   <tr> <td> @ref OCI_ERR_DRIVER_NO_RESPONSE </td>
        <td> The driver for a removable hardware received no response to a request. The hardware may be removed. 
             It is not known wether the request is lost and the command not exectuted or the response is lost and
             the command was executed. The controller enters the @a DISCONNECTED state. All subsequent calls will 
             return @ref OCI_ERR_DRIVER_DISCONNECTED. 
             Internal resources are locked until OCI_DestroyLINController() for the controller is called. </td> 
   </tr>
   <tr> <td> @ref OCI_ERR_DRIVER_DISCONNECTED </td>
        <td> The driver for a removable hardware detected a communication problem (in asynchromous communication, 
             by event signalling or during a previous call to the same hardware) and is not able to communicate to 
             the hardware any more. 
             The state of the controller is @a DISCONNECTED. The command is not executed. All subsequent calls to 
             the same Controller will return @ref OCI_ERR_DRIVER_DISCONNECTED. Internal resources are locked until 
             OCI_DestroyLINController() for the controller is called.  </td> 
   </tr>
   <tr> <td> @ref OCI_ERR_OUT_OF_MEMORY </td>
        <td> The driver run out of memory. This error should only occur on heavy loaded systems with multiple 
             clients and large send and receive queues. The error may indicate a resource leak in the driver 
             implementation. When multiple application access the same resource (interface hardware or physical
             controller) the termination of a different application may return the resources required for this 
             instance. </td> 
   </tr>

   </TABLE> 
*/

#ifndef OCI_DYNAMICDLL
OCI_DECLSPEC OCI_ErrorCode
OCI_CALL OCI_ReadLINData(
    OCI_QueueHandle queue, 
    OCI_Time timeout,
    OCI_LINMessage messages[], 
    uint32 size, 
    uint32 *count,
    uint32 *remaining);
#endif

typedef OCI_ErrorCode
(OCI_CALL *PF_OCI_ReadLINData)(
    OCI_QueueHandle queue, 
    OCI_Time timeout,
    OCI_LINMessage messages[], 
    uint32 size, 
    uint32 *count,
    uint32 *remaining);

/**
  Destroy a receive queue and unregister callback routines for data and event reception for a specific LIN controller.

  Any data in the queue, that is not read by OCI_ReadLINData() will be discarded. The registered callback routines
  for data reception and event reception (see @ref OCI_LINRxQueueConfiguration) will not be called after completion of
  this routine. All white filter added to this queue will be discarded.  
  
  The handle for the queue must not used any longer. 

  @todo This call needs a synchronization of the data transmission via callback mechanism 
  and the synchronous command processing through all transport layer!

  @controllerState SUSPENDED, RUNNING. \n
                   When the controller is in the CREATED state no valid Queue can exist and the call will return
                   @ref OCI_ERR_INVALID_HANDLE. \n
                   When the controller is in the DISCONNECTED state the function may be called to do local clean
                   up. Only a limited plausibility check may be done in this case.  

  @param[in]  queue  Handle of the queue.

  @return     
  <TABLE border="0" rules="rows" cellspacing="3" cellpadding="3">  
   <tr> <td> @ref OCI_SUCCESS </td> 
        <td> The operation was successful. The queue is destroyed and the handle must not be used any longer. </td> 
   </tr>
   <tr> <td> @ref OCI_ERR_INVALID_HANDLE </td>  
        <td> The @a queue handle is not valid. </td> 
   </tr>
   <tr> <td> @ref OCI_ERR_OUT_OF_MEMORY </td>
        <td> The driver run out of memory. This error should only occur on heavy loaded systems with multiple 
             clients and large send and receive queues. The error may indicate a resource leak in the driver 
             implementation. When multiple application access the same resource (interface hardware or physical
             controller) the termination of a different application may return the resources required for this 
             instance. </td> 
   </tr>

   </TABLE> 
* This function requires a specific state of the controller 
* @sa         @ref GROUP_OCI_CONTROLLER "Controller Handling" 
*/
 
#ifndef OCI_DYNAMICDLL
OCI_DECLSPEC OCI_ErrorCode 
OCI_CALL OCI_DestroyLINRxQueue(
    OCI_QueueHandle queue);
#endif

typedef OCI_ErrorCode
(OCI_CALL *PF_OCI_DestroyLINRxQueue)(
    OCI_QueueHandle queue);


/**
* @}
* @addtogroup GROUP_OCI_LIN_FILTERING Filtering
* @{
*/

/** 
* Structure that defines a single LIN frame filter.
*/
typedef struct OCI_LINRxFilter
{
    /**
    * Value that the LIN frame ID must match after AND-gating it with 
    * @a frameIDMask.
    */
    uint32 frameIDValue;
    
    /** 
    * Specifies the mask for the message. The mask can be used to
    * enable a monitoring of all frameIDs with a single filter condition:
    * frameIDValue = 0, frameIDMask = 0
    */
    uint32 frameIDMask;
    
    /**
    * Application defined filter tag. 
    * All events of this kind are tagged with this value.
    */
    uint32 tag;

} OCI_LINRxFilter;


/** 
* Structure that defines a single LIN event filter.
*/
typedef struct OCI_LINEventFilter
{
    /**
    * Specifies the event kind @ref OCI_LINEvent.
    */
    uint32 eventCode;
    
    /**
    * Application defined filter tag. 
    * All events of this kind are tagged with this value.
    */
    uint32 tag;

    /** 
    * Destination of the event. When @ref OCI_EVENT_DESTINATION_INBAND 
    * is set, the event will be delivered inband to the application. 
    * When the @ref OCI_EVENT_DESTINATION_CALLBACK is set the event 
    * will trigger the event handler of the receive queue for that 
    * event. When both flags are combined the event is duplicated and 
    * the callback is triggered and the event is stored inband in the 
    * queue.
    */
    uint32 destination;

} OCI_LINEventFilter;

/** 
  Add white filter condition(s) to enable the reception of LIN frames for a specific LIN receive queue.

  In the default configuration of a receive queue no data will be received because any data is blocked by
  some filter condition. The application must enable the reception of the data by setting white filter 
  (defining pass conditions). 

  This function allows the reception of specific LIN frames on the LIN bus. The frames may be queued and read
  by OCI_ReadLINData() or a callback may is triggered whenever a frame passes the filter. The signalization 
  path to the application is defined by the creation of the receive queue. (See also OCI_CreateLINRxQueue()).
  
  Filter conditions allow an effective reduction of the payload, that must be handled by the driver and
  application. The sequence of processing the filter array is undefined. When a frame matches to more than 
  one filter the frame will be passed to the application, but the used filtertag (identifying the matching
  filter) is implementation specific and may change in different versions and on different hardware.

  @controllerState SUSPENDED, RUNNING. \n
                   When the controller is in the CREATED state no valid Queue can exist and the call will return
                   @ref OCI_ERR_INVALID_HANDLE.

  @param[in]  queue    Handle of the LIN receive queue that will receive the messages. 
  @param[in]  filter   A pointer to an array of @a count entries of type @ref OCI_LINRxFilter. 
  @param[in]  count    Number of entries in the @a filter array. 

  @return     
  <TABLE border="0" rules="rows" cellspacing="3" cellpadding="3">  
   <tr> <td> @ref OCI_SUCCESS </td> 
        <td> The operation was successful. </td> 
   </tr>
   <tr> <td> @ref OCI_ERR_INVALID_HANDLE </td>  
        <td> The @a queue handle is not valid. </td> 
   </tr>
   <tr> <td> @ref OCI_ERR_INVALID_PARAMETER </td>
        <td> The @a filter parameter is NULL or references an invalid address.</td> 
   </tr>
   <tr> <td> @ref OCI_ERR_INCONSISTENT_PARAMETER_SET </td>
        <td> One or more filter objects are invalid or already exist.</td> 
   </tr>
   <tr> <td> @ref OCI_ERR_DRIVER_NO_RESPONSE </td>
        <td> The driver for a removable hardware received no response to a request. The hardware may be removed. 
             It is not known wether the request is lost and the command not exectuted or the response is lost and
             the command was executed. The controller enters the @a DISCONNECTED state. All subsequent calls will 
             return @ref OCI_ERR_DRIVER_DISCONNECTED. 
             Internal resources are locked until OCI_DestroyLINController() for the controller is called. </td> 
   </tr>
   <tr> <td> @ref OCI_ERR_DRIVER_DISCONNECTED </td>
        <td> The driver for a removable hardware detected a communication problem (in asynchromous communication, 
             by event signalling or during a previous call to the same hardware) and is not able to communicate to 
             the hardware any more. 
             The state of the controller is @a DISCONNECTED. The command is not executed. All subsequent calls to 
             the same Controller will return @ref OCI_ERR_DRIVER_DISCONNECTED. Internal resources are locked until 
             OCI_DestroyLINController() for the controller is called.  </td> 
   </tr>
   <tr> <td> @ref OCI_ERR_OUT_OF_MEMORY </td>
        <td> The driver run out of memory. This error should only occur on heavy loaded systems with multiple 
             clients and large send and receive queues. The error may indicate a resource leak in the driver 
             implementation. When multiple application access the same resource (interface hardware or physical
             controller) the termination of a different application may return the resources required for this 
             instance. </td> 
   </tr>

  </TABLE> 
 
  @sa PAGE_OCI_RECEIVE_MESSAGES, @ref GROUP_OCI_CONTROLLER "Controller Handling"
*/

#ifndef OCI_DYNAMICDLL
OCI_DECLSPEC OCI_ErrorCode 
OCI_CALL OCI_AddLINFrameFilter(
    OCI_QueueHandle queue, 
    OCI_LINRxFilter filter[], 
    uint32 count);
#endif    

typedef OCI_ErrorCode
(OCI_CALL *PF_OCI_AddLINFrameFilter)(
    OCI_QueueHandle queue, 
    OCI_LINRxFilter filter[], 
    uint32 count);

/**
  Remove white filter condition(s) to disable the reception of LIN frames for a specific LIN receive queue.

  In the default configuration of a receive queue no data will be received because any data is blocked by
  some filter condition. The application must enable the reception of the data by setting white filter 
  (defining pass conditions). 

  This function allows to withdraw one or more white filter conditions set by OCI_AddLINFrameFilter(). Each
  @ref OCI_LINRxFilter of the @a filter array must exact match to a previously added filter. It is
  not possible to restrict an existing white filter, without redrawing it and adding a new more restrictive
  one.
     
  @controllerState SUSPENDED, RUNNING. \n
                   When the controller is in the CREATED state no valid Queue can exist and the call will return
                   @ref OCI_ERR_INVALID_HANDLE.  \n
                   When the controller is in the DISCONNECTED state the function may be called to do local clean
                   up. Only a limited plausibility check may be done in this case.  


  @param[in]  queue    Handle of the LIN receive queue to modify. 
  @param[in]  filter   A pointer to an array of @a count entries of type @ref OCI_LINRxFilter. 
  @param[in]  count    Number of entries in the @a filter  array. 

  @return     
  <TABLE border="0" rules="rows" cellspacing="3" cellpadding="3">  
   <tr> <td> @ref OCI_SUCCESS </td> 
        <td> The operation was successful. </td> 
   </tr>
   <tr> <td> @ref OCI_ERR_INVALID_HANDLE </td>  
        <td> The @a queue handle is not valid. </td> 
   </tr>
   <tr> <td> @ref OCI_ERR_INVALID_PARAMETER </td>
        <td> The @a filter parameter is NULL or points to an invalid address.</td> 
   </tr>
   <tr> <td> @ref OCI_ERR_INCONSISTENT_PARAMETER_SET </td>
        <td> One or more @a filter objects are not set and can not be removed. The processing of the
             filter array is undefined. </td> 
   </tr>
   <tr> <td> @ref OCI_ERR_OUT_OF_MEMORY </td>
        <td> The driver run out of memory. This error should only occur on heavy loaded systems with multiple 
             clients and large send and receive queues. The error may indicate a resource leak in the driver 
             implementation. When multiple application access the same resource (interface hardware or physical
             controller) the termination of a different application may return the resources required for this 
             instance. </td> 
   </tr>
  </TABLE> 

  @sa         PAGE_OCI_RECEIVE_MESSAGES, @ref GROUP_OCI_CONTROLLER "Controller Handling"

*/


#ifndef OCI_DYNAMICDLL
OCI_DECLSPEC OCI_ErrorCode 
OCI_CALL OCI_RemoveLINFrameFilter(
    OCI_QueueHandle queue, 
    OCI_LINRxFilter filter[], 
    uint32 count);
#endif

typedef OCI_ErrorCode
(OCI_CALL *PF_OCI_RemoveLINFrameFilter)(
    OCI_QueueHandle queue, 
    OCI_LINRxFilter filter[], 
    uint32 count);

/** 
  Add white filter condition(s) to enable the reception of LIN bus events for a specific LIN receive queue.

  In the default configuration of a receive queue no data will be received because any data is blocked by
  some filter condition. The application must enable the reception of the data by setting white filter 
  (defining pass conditions). 

  This function allows the reception of specific events on the LIN bus. The events may be delivered in-band
  in the normal data stream or generate a (separate) callback. The signalization path to the application is
  defined by the creation of the receive queue. (See also OCI_CreateLINRxQueue()).

  @controllerState RUNNING, SUSPENDED. \n
                   When the controller is in the CREATED state no valid Queue can exist and the call will return
                   @ref OCI_ERR_INVALID_HANDLE.

  @param[in]  queue    Handle of the LIN receive queue that will receive the messages. 
  @param[in]  filter   A pointer to an array of @a count entries of type @ref OCI_LINEventFilter. 
  @param[in]  count    Number of entries in the @a filter array. 

  @return     
  <TABLE border="0" rules="rows" cellspacing="3" cellpadding="3">  
   <tr> <td> @ref OCI_SUCCESS </td> 
        <td> The operation was successful. </td> 
   </tr>
   <tr> <td> @ref OCI_ERR_INVALID_HANDLE </td>  
        <td> The handle is not valid. </td> 
   </tr>
   <tr> <td> @ref OCI_ERR_INVALID_PARAMETER </td>
        <td> The filters parameter is NULL or references an invalid address.</td> 
   </tr>
   <tr> <td> @ref OCI_ERR_INCONSISTENT_PARAMETER_SET </td>
        <td> One or more filter objects are invalid or already exist.</td> 
   </tr>
   <tr> <td> @ref OCI_ERR_DRIVER_NO_RESPONSE </td>
        <td> The driver for a removable hardware received no response to a request. The hardware may be removed. 
             It is not known wether the request is lost and the command not exectuted or the response is lost and
             the command was executed. The controller enters the @a DISCONNECTED state. All subsequent calls will 
             return @ref OCI_ERR_DRIVER_DISCONNECTED. 
             Internal resources are locked until OCI_DestroyLINController() for the controller is called. </td> 
   </tr>
   <tr> <td> @ref OCI_ERR_DRIVER_DISCONNECTED </td>
        <td> The driver for a removable hardware detected a communication problem (in asynchromous communication, 
             by event signalling or during a previous call to the same hardware) and is not able to communicate to 
             the hardware any more. 
             The state of the controller is @a DISCONNECTED. The command is not executed. All subsequent calls to 
             the same Controller will return @ref OCI_ERR_DRIVER_DISCONNECTED. Internal resources are locked until 
             OCI_DestroyLINController() for the controller is called.  </td> 
   </tr>
   <tr> <td> @ref OCI_ERR_OUT_OF_MEMORY </td>
        <td> The driver run out of memory. This error should only occur on heavy loaded systems with multiple 
             clients and large send and receive queues. The error may indicate a resource leak in the driver 
             implementation. When multiple application access the same resource (interface hardware or physical
             controller) the termination of a different application may return the resources required for this 
             instance. </td> 
   </tr>
  </TABLE> 

  @sa GROUP_OCI_EVENT_HANDLING, @ref PAGE_OCI_RECEIVE_MESSAGES "Receive Messages"
*/

#ifndef OCI_DYNAMICDLL
OCI_DECLSPEC OCI_ErrorCode 
OCI_CALL OCI_AddLINEventFilter(
    OCI_QueueHandle queue, 
    OCI_LINEventFilter filter[], 
    uint32 count);
#endif

typedef OCI_ErrorCode
(OCI_CALL *PF_OCI_AddLINEventFilter)(
    OCI_QueueHandle queue, 
    OCI_LINEventFilter filter[], 
    uint32 count);

/**
  Remove white filter condition(s) do disable reception of LIN events for a specific LIN receive queue. 

  In the default configuration of a receive queue no data will be received because any data is blocked by
  some filter condition. The application must enable the reception of the data by setting white filter 
  (defining pass conditions). 

  This function allows to withdraw one or more white filter conditions set by OCI_AddLINEventFilter(). Each
  @ref OCI_LINEventFilter of the @a filter array must exact match to a previously added filter. 

  @controllerState RUNNING, SUSPENDED. \n
                   When the controller is in the CREATED state no valid Queue can exist and the call will return
                   @ref OCI_ERR_INVALID_HANDLE.  \n
                   When the controller is in the DISCONNECTED state the function may be called to do local clean
                   up. Only a limited plausibility check may be done in this case.  
  
  @param[in]  queue    Handle of the LIN receive queue to modify. 
  @param[in]  filter   A pointer to an array of @a count entries of type @ref OCI_LINEventFilter. 
  @param[in]  count    Number of entries in the @a filters array. 

  @return     
  <TABLE border="0" rules="rows" cellspacing="3" cellpadding="3">  
   <tr> <td> @ref OCI_SUCCESS </td> 
        <td> The operation was successful. </td> 
   </tr>
   <tr> <td> @ref OCI_ERR_INVALID_HANDLE </td>  
        <td> The @a queue handle is not valid. </td> 
   </tr>
   <tr> <td> @ref OCI_ERR_INVALID_PARAMETER </td>
        <td> The @a filters parameter is NULL or references an invalid object.</td> 
   </tr>
   <tr> <td> @ref OCI_ERR_INCONSISTENT_PARAMETER_SET </td>
        <td> One or more @a filter objects are not set and can not be removed. The processing of the
             filter array is undefined. </td> 
   </tr>
   <tr> <td> @ref OCI_ERR_OUT_OF_MEMORY </td>
        <td> The driver run out of memory. This error should only occur on heavy loaded systems with multiple 
             clients and large send and receive queues. The error may indicate a resource leak in the driver 
             implementation. When multiple application access the same resource (interface hardware or physical
             controller) the termination of a different application may return the resources required for this 
             instance. </td> 
   </tr>
  </TABLE> 

  @sa         GROUP_OCI_EVENT_HANDLING, @ref PAGE_OCI_RECEIVE_MESSAGES "Receive Messages", 
              @ref GROUP_OCI_CONTROLLER "Controller Handling"
*/

#ifndef OCI_DYNAMICDLL
OCI_DECLSPEC OCI_ErrorCode 
OCI_CALL OCI_RemoveLINEventFilter(
    OCI_QueueHandle queue, 
    OCI_LINEventFilter filter[], 
    uint32 count);
#endif

typedef OCI_ErrorCode
(OCI_CALL *PF_OCI_RemoveLINEventFilter)(
    OCI_QueueHandle queue, 
    OCI_LINEventFilter filter[], 
    uint32 count);


/**
* @}
* @addtogroup GROUP_OCI_LIN_INITIALIZATION Initialization
* @{
*/

/**
* Non static read only information of a LIN controller (status). 
* This information may change during the run-time by different events. 
*/
typedef struct OCI_LINControllerStatus
{
    /** 
    * Global receive time stamp. Depending on the implementation, this may be 
    * based on a hardware-internal timer with no global synchronization. 
    * In particular, the epoch (i.e. value 0) may be the point in time the 
    * hardware was switched on. In general the timestamp results from a
    * received frame or event by which the @ref sleepMode or @ref transResult
    * has been changed.
    */
    OCI_Time timeStamp;

    /**
    * Result of the last executed LIN frame transmission by the controller.
    */
    OCI_LIN_TRANSMISSION_RESULT transResult; 

    /**
    * Sleep mode state @ref OCI_LIN_SLEEP_MODE of the controller.
    * @ref OCI_NO_VALUE, if the Sleep mode state is unknown.
    */
    uint32 sleepMode;

} OCI_LINControllerStatus;

/**
 Query the status of a specific LIN Controller instance. 
 
 The status information of a LIN controller may change by some events on the LIN bus.
 
 @param[in]  controller  handle of the LIN controller interface instance for which the status is requested.
 @param[out] status      Pointer to a buffer receiving the requested status information of the LIN controller.
 
 @controllerState CREATED, RUNNING, SUSPENDED.

 @return
   <TABLE border="0" rules="rows" cellspacing="3" cellpadding="3">  
   <tr> <td> @ref OCI_SUCCESS </td> 
        <td> The operation was successful. </td> 
   </tr>
   <tr> <td> @ref OCI_ERR_INVALID_HANDLE </td>  
        <td> The @a controller handle is not valid. </td> 
   </tr>
   <tr> <td> @ref OCI_ERR_INVALID_PARAMETER </td>
        <td> The @a status parameter is NULL or references an invalid address.</td> 
   </tr>
   <tr> <td> @ref OCI_ERR_DRIVER_NO_RESPONSE </td>
        <td> The driver for a removable hardware received no response to a request. The hardware may be removed. 
             It is not known wether the request is lost and the command not exectuted or the response is lost and
             the command was executed. The controller enters the @a DISCONNECTED state. All subsequent calls will 
             return @ref OCI_ERR_DRIVER_DISCONNECTED. 
             Internal resources are locked until OCI_DestroyLINController() for the controller is called. </td> 
   </tr>
   <tr> <td> @ref OCI_ERR_DRIVER_DISCONNECTED </td>
        <td> The driver for a removable hardware detected a communication problem (in asynchromous communication, 
             by event signalling or during a previous call to the same hardware) and is not able to communicate to 
             the hardware any more. 
             The state of the controller is @a DISCONNECTED. The command is not executed. All subsequent calls to 
             the same Controller will return @ref OCI_ERR_DRIVER_DISCONNECTED. Internal resources are locked until 
             OCI_DestroyLINController() for the controller is called.  </td> 
   </tr>
   <tr> <td> @ref OCI_ERR_OUT_OF_MEMORY </td>
        <td> The driver run out of memory. This error should only occur on heavy loaded systems with multiple 
             clients and large send and receive queues. The error may indicate a resource leak in the driver 
             implementation. When multiple application access the same resource (interface hardware or physical
             controller) the termination of a different application may return the resources required for this 
             instance. </td> 
   </tr>
  </TABLE> 
  @sa         @ref GROUP_OCI_CONTROLLER "Controller Handling" 
*/

#ifndef OCI_DYNAMICDLL
OCI_DECLSPEC OCI_ErrorCode 
OCI_CALL OCI_GetLINControllerStatus(
    OCI_ControllerHandle controller, 
    OCI_LINControllerStatus* status);
#endif

typedef OCI_ErrorCode
(OCI_CALL *PF_OCI_GetLINControllerStatus)(
    OCI_ControllerHandle controller, 
    OCI_LINControllerStatus* status);

typedef struct OCI_LINIO_VTable
{
    PF_OCI_GetLINControllerCapabilities getLINControllerCapabilities;
    PF_OCI_GetLINControllerStatus getLINControllerStatus;

    PF_OCI_CreateLINTxQueue createLINTxQueue;
    PF_OCI_WriteLINData writeLINData;
    PF_OCI_DestroyLINTxQueue destroyLINTxQueue;

    PF_OCI_CreateLINRxQueue createLINRxQueue;
    PF_OCI_ReadLINData readLINData;
    PF_OCI_DestroyLINRxQueue destroyLINRxQueue;

    PF_OCI_AddLINFrameFilter addLINFrameFilter;
    PF_OCI_RemoveLINFrameFilter removeLINFrameFilter;
    PF_OCI_AddLINEventFilter addLINEventFilter;
    PF_OCI_RemoveLINEventFilter removeLINEventFilter;
} OCI_LINIO_VTable;

/** @} */

/** @} */

#ifdef __cplusplus
}
#endif

#endif
