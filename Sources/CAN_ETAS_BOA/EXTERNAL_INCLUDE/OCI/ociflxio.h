#ifndef ETAS_OCI_FLEXRAY_IO_H
#define ETAS_OCI_FLEXRAY_IO_H

/**
* @file       ociflxio.h
* @brief      FlexRay specific part of the Open Controller Interface (OCI) API
*             excluding the stronly typed controller configuration.
* @copyright  Copyright (c) 2007-2008 ETAS GmbH. All rights reserved.
*
* $Revision: 4801 $
*/


#include "ocibase.h"

#ifdef __cplusplus
extern "C" {
#endif


/**
* @addtogroup GROUP_OCI_FLEXRAY_CONTROLLER OCI FlexRay
* @brief      FlexRay specific part of the Open Controller Interface (OCI). 
* @{
*/

/**
* @addtogroup GROUP_OCI_FLEXRAY_INITIALIZATION Initialization
* @{
*/

/**
* @anchor  ANCHOR_OCI_FLEXRAY_MAX_RANGES
* @name    Range definitions given by ERay implementation.
*/

/** @{ */

/** Number of supported buffers. */
#define OCI_FLEXRAY_MAX_BUFFERCOUNT  (128u)

/** 
* Total amount of memory (in 32 bit words), useable for the supported 
* buffers 
*/
#define OCI_FLEXRAY_MAX_32BITMEMSIZE (2048u)

/** @} */

/**
* @anchor  ANCHOR_OCI_FLEXRAY_CC_INDICATORS
* @name    FlexRay communication controller indicator flags.
* @brief   Flags to specify additional information for the communication 
*          controller.
*/

/** @{ */

/** Bit indicator for FlexRay channel A (bit 0) */
#define OCI_FLEXRAY_CHANNEL_A   (1 << 0)

/** Bit indicator for FlexRay channel B (bit 1) */ 
#define OCI_FLEXRAY_CHANNEL_B   (1 << 1)

/** @} */


/** 
* Structure containing the core configuration parameters for a FlexRay transmit
* buffer. 
*/

typedef struct OCI_FlexRayTxBufferCoreConfiguration 
{
    /** 
    * Transmission slot. 
    * @values  Valid values are 1..2047. 
    */
    uint32 slot;
    
    /** 
    * Transmission channel (A, B or both).
    * @values  Valid values are OCI_FLEXRAY_CHANNEL_A and OCI_FLEXRAY_CHANNEL_B.
    * @remark  Both values can be bitwise OR-connected to enable transmission on both channels.
    */
    uint32 channel;
    
    /** 
    * Transmission periodicity. 
    * @values  Valid values are 1, 2, 4, 8, 16, 32, 64.
    * @remark  Set the periodicity (@ref cyclePeriod) to 1 and the 
    *          offset (@ref cycleOffset) to 0 to transmit messages in 
    *          every cycle.
    */
    uint32 cyclePeriod;
    
    /** 
    * Starting offset for periodicity. 
    * @values  Valid values are 0..CyclePeriod-1. 
    */
    uint32 cycleOffset;      
   
} OCI_FlexRayTxBufferCoreConfiguration;




/**
* @}
* @addtogroup GROUP_OCI_FLEXRAY_INITIALIZATION Initialization
* @{
*/

/** 
* Valid protocol operation control states. 
* @todo: Check and complete comments.
*/

typedef enum OCI_FlexRayControllerProtocolOperationControlStatus
{
    /** @todo  Comment is missing! */
    OCI_FLEXRAY_POCS_DEFAULT_CONFIG                 = 0x00,
    
    /** @todo  Comment is missing! */
    OCI_FLEXRAY_POCS_READY                          = 0x01,
    
    /** @todo  Comment is missing! */
    OCI_FLEXRAY_POCS_NORMAL_ACTIVE                  = 0x02,
    
    /** @todo  Comment is missing! */
    OCI_FLEXRAY_POCS_NORMAL_PASSIVE                 = 0x03,
    
    /** @todo  Comment is missing! */
    OCI_FLEXRAY_POCS_HALT                           = 0x04,
    
    /** @todo  Comment is missing! */
    OCI_FLEXRAY_POCS_MONITOR_MODE                   = 0x05,
    
    /** @todo  Comment is missing! */
    OCI_FLEXRAY_POCS_CONFIG                         = 0x0F,
    
    /** @todo  Comment is missing! */
    OCI_FLEXRAY_POCS_WAKEUP_STANDBY                 = 0x10,
    
    /** @todo  Comment is missing! */
    OCI_FLEXRAY_POCS_WAKEUP_LISTEN                  = 0x11,
    
    /** @todo  Comment is missing! */
    OCI_FLEXRAY_POCS_WAKEUP_SEND                    = 0x12,
    
    /** @todo  Comment is missing! */
    OCI_FLEXRAY_POCS_WAKEUP_DETECT                  = 0x13,
    
    /** @todo  Comment is missing! */
    OCI_FLEXRAY_POCS_STARTUP_PREPARE                = 0x20,
    
    /** @todo  Comment is missing! */
    OCI_FLEXRAY_POCS_COLDSTART_LISTEN               = 0x21,
    
    /** @todo  Comment is missing! */
    OCI_FLEXRAY_POCS_COLDSTART_COLLISION_RESOLUTION = 0x22,
    
    /** @todo  Comment is missing! */
    OCI_FLEXRAY_POCS_COLDSTART_CONSISTENCY_CHECK    = 0x23,
    
    /** @todo  Comment is missing! */
    OCI_FLEXRAY_POCS_COLDSTART_GAP                  = 0x24,
    
    /** @todo  Comment is missing! */
    OCI_FLEXRAY_POCS_COLDSTART_JOIN                 = 0x25,
    
    /** @todo  Comment is missing! */
    OCI_FLEXRAY_POCS_INTEGRATION_COLDSTART_CHECK    = 0x26,
    
    /** @todo  Comment is missing! */
    OCI_FLEXRAY_POCS_INTEGRATION_LISTEN             = 0x27,
    
    /** @todo  Comment is missing! */
    OCI_FLEXRAY_POCS_INTEGRATION_CONSISTENCY_CHECK  = 0x28,
    
    /** @todo  Comment is missing! */
    OCI_FLEXRAY_POCS_INITIALIZE_SCHEDULE            = 0x29,
    
    /** @todo  Comment is missing! */
    OCI_FLEXRAY_POCS_ABORT_STARTUP                  = 0x2A
    
} OCI_FlexRayControllerProtocolOperationControlStatus;


/** Common part of a FlexRay controller's status information. */
typedef struct OCI_FlexRayControllerStatusCommon
{
    /** 
    * Global receive time stamp. Depending on the implementation, this may be 
    * based on a hardware-internal timer with no global synchronization. 
    * In particular, the epoch (i.e. value 0) may be the point in time the 
    * hardware was switched on.
    */
    OCI_Time timeStamp;

    /** 
    * Current protocol operation control state of the communication controller.
    */
    OCI_FlexRayControllerProtocolOperationControlStatus \
        protocolOperationControlStatus;

    /** 
    * Number of cold start attempts the communication controller will still 
    * perform. 
    */
    uint32 remainingColdStartAttempts;
    
    /** 
    * Number of missing clock correction terms (counted in even/odd cycle 
    * pairs).
    */
    uint32 clockCorrectionFailedCounter;

    /** 
    * Number of valid consecutive clock correction terms, while waiting to 
    * transit from NORMAL_PASSIVE to NORMAL_ACTIVE state.
    */
    uint32 passiveToActiveCount;        
    
    /** Current value of the communication cycle counter. */
    uint32 cycleCounterValue;
    
    /** Latest applied rate correction value. The unit is µT. */
    sint32 rateCorrectionValue;
    
    /** Latest applied offset  correction value. The unit is µT. */
    sint32 offsetCorrectionValue;  
    
} OCI_FlexRayControllerStatusCommon;


/** Channel specific part of a FlexRay controller's status information. */
typedef struct OCI_FlexRayControllerStatusChannel
{
    /** 
    * Global receive time stamp. Depending on the implementation, this may be 
    * based on a hardware-internal timer with no global synchronization. 
    * In particular, the epoch (i.e. value 0) may be the point in time the 
    * hardware was switched on.
    */
    OCI_Time timeStamp;

    /**
    * This counter is incremented for every observation period (16ms) during 
    * which no valid frames were received on channel A. This may be due to a 
    * cause such as a missing or faulty ECU on the bus which should send this 
    * frame. The counter overflows at 4294967295.
    */
    uint32 missingValidFrameA;
    
    /**
    * This counter is incremented for every observation period (16ms) during 
    * which no valid frames were received on channel B. This may be due to a 
    * cause such as a missing or faulty ECU on the bus which should send this 
    * frame. The counter overflows at 4294967295.
    */
    uint32 missingValidFrameB;
    
    /** 
    * This counter is incremented whenever one or more syntax errors in static 
    * or dynamic slots, symbol window, or NIT were observed on channel A. The 
    * counter overflows at 4294967295.
    */
    uint32 syntaxErrorDetectedA;
    
    /** 
    * This counter is incremented whenever one or more syntax errors in static 
    * or dynamic slots, symbol window, or NIT were observed on channel B. The 
    * counter overflows at 4294967295.
    */
    uint32 syntaxErrorDetectedB;
    
    /**
    * This counter is incremented whenever one or more frames with a content 
    * error were received in any static or dynamic slot on channel A. The 
    * counter overflows at 4294967295.
    */
    uint32 contentErrorDetectedA;
    
    /**
    * This counter is incremented whenever one or more frames with a content 
    * error were received in any static or dynamic slot on channel B. The 
    * counter overflows at 4294967295.
    */
    uint32 contentErrorDetectedB;
    
    /**
    * This counter is incremented whenever one or more valid frames were 
    * received in slots on channel A that also contained additional 
    * communication, i.e. one or more slots received a valid frame AND had any 
    * combination of either syntax error OR content error OR slot boundary 
    * violation. The counter overflows at 4294967295.
    */
    uint32 communicationIndicatorA;
    
    /**
    * This counter is incremented whenever one or more valid frames were 
    * received in slots on channel B that also contained additional 
    * communication, i.e. one or more slots received a valid frame AND had any 
    * combination of either syntax error OR content error OR slot boundary 
    * violation. The counter overflows at 4294967295.
    */    
    uint32 communicationIndicatorB;
    
    /**
    * This counter provides the number of detected slot boundary violations in 
    * static and dynamic slots, symbol window, and NIT on channel A. The 
    * counter overflows at 4294967295.
    */
    uint32 slotBoundaryViolationA;
    
    /**
    * This counter provides the number of detected slot boundary violations in 
    * static and dynamic slots, symbol window, and NIT on channel B. The 
    * counter overflows at 4294967295.
    */
    uint32 slotBoundaryViolationB;
    
    /** 
    * This counter provides the number of wakeup patterns received on channel A.
    * The counter overflows at 4294967295.
    */
    uint32 wakeupPatternA;
    
    /** 
    * This counter provides the number of wakeup patterns received on channel B.
    * The counter overflows at 4294967295.
    */
    uint32 wakeupPatternB;
    
    /**
    * This counter provides the number of media test symbols received on channel
    * A. The counter overflows at 4294967295.
    */
    uint32 mediaTestSymbolA;
    
    /**
    * This counter provides the number of media test symbols received on channel
    * B. The counter overflows at 4294967295.
    */
    uint32 mediaTestSymbolB;
    
    /**
    * This counter provides the number of send actions detected after the latest
    * admissible mini slot in the dynamic segment (see also parameter pLatestTx)
    * on channel A. The counter overflows at 4294967295.
    */
    uint32 latestTransmitViolationA;
    
    /**
    * This counter provides the number of send actions detected after the latest
    * admissible mini slot in the dynamic segment (see also parameter pLatestTx)
    * on channel B. The counter overflows at 4294967295.
    */
    uint32 latestTransmitViolationB;
    
    /** @todo: Drop? */
    uint32 transmissionAcrossBoundaryA;
    
    /** @todo: Drop? */
    uint32 transmissionAcrossBoundaryB;
    
} OCI_FlexRayControllerStatusChannel;


/** Network specific part of a FlexRay controller's status information. */
typedef struct OCI_FlexRayControllerStatusNetwork
{
    /** 
    * Global receive time stamp. Depending on the implementation, this may be 
    * based on a hardware-internal timer with no global synchronization. 
    * In particular, the epoch (i.e. value 0) may be the point in time the 
    * hardware was switched on.
    */
    OCI_Time timeStamp;

    /**
    * Current content of the network management vector maintained by the FlexRay
    * controller. The valid length is determined by the cluster wide parameter 
    * gNetworkManagementVectorLength.
    */
    uint32 networkManagementVector[3];  
    
} OCI_FlexRayControllerStatusNetwork;


/** Types of FlexRay controller status information */
typedef enum OCI_FlexRayControllerStatusType
{
    /** FlexRay controller status information concerning common parts */
    OCI_FLEXRAY_CONTROLLER_STATUS_COMMON = 1,
    
    /** FlexRay controller status information concerning channel(s) */ 
    OCI_FLEXRAY_CONTROLLER_STATUS_CHANNEL = 2,
    
    /** FlexRay controller status information concerning network */
    OCI_FLEXRAY_CONTROLLER_STATUS_NETWORK = 3
      
} OCI_FlexRayControllerStatusType;


/** Union of FlexRay controller status information */
typedef union OCI_FlexRayControllerStatusData
{
    /** 
    * Use this part when @ref OCI_FlexRayControllerStatus.type is 
    * @ref OCI_FLEXRAY_CONTROLLER_STATUS_COMMON 
    */
    OCI_FlexRayControllerStatusCommon common;
        
    /** 
    * Use this part when @ref OCI_FlexRayControllerStatus.type is 
    * @ref OCI_FLEXRAY_CONTROLLER_STATUS_CHANNEL 
    */
    OCI_FlexRayControllerStatusChannel channel;
    
    /** 
    * Use this part when @ref OCI_FlexRayControllerStatus.type is 
    * @ref OCI_FLEXRAY_CONTROLLER_STATUS_NETWORK 
    */
    OCI_FlexRayControllerStatusNetwork network;

} OCI_FlexRayControllerStatusData;


/**
* Non-static read-only information of a FlexRay controller (status). This 
* information may change during the run-time by different events. 
*/
typedef struct OCI_FlexRayControllerStatus 
{
    /** 
    * Discriminator for the @a data union member 
    * @values   The following values are valid:
    * @n        @ref OCI_FLEXRAY_CONTROLLER_STATUS_COMMON    
    * @n        @ref OCI_FLEXRAY_CONTROLLER_STATUS_CHANNEL
    * @n        @ref OCI_FLEXRAY_CONTROLLER_STATUS_NETWORK
    * @sa       @ref OCI_FlexRayControllerStatusType
    *           "OCI FlexRay Status message types"
    */     
    OCI_FlexRayControllerStatusType type;   

    /** FlexRay controller status information data union */
    OCI_FlexRayControllerStatusData data;
    
} OCI_FlexRayControllerStatus;


/** 
 Query the status of a specific FlexRay controller. 

 The status information for the FlexRay Bus is devided into three different parts. All parts can be requested
 separatly by specifying a different @a type in the request. It is possible to request the status of the FlexRay
 network, the status of the local node represented by this FlexRay controller instance and channel specific 
 information of the network controller.
 
 @param[in]  controller  Handle of the FlexRay controller interface instance for which the status is requested.
 @param[in]  type        Type of the requested FlexRay controller status information. The FlexRay Controller
                         offers separate status information for the network, node and channel. 
 @param[out] status      Pointer to a structure filled with the requested status information of the FlexRay 
                         controller.
 
 @todo naming common == node ?

 @controllerState CREATED, SUSPENDED, RUNNING. 
 
 @return
   <TABLE border="0" rules="rows" cellspacing="3" cellpadding="3">  
   <tr> <td> @ref OCI_SUCCESS </td> 
        <td> The operation was successful. The information returned in @a status is valid. </td> 
   </tr>
   <tr> <td> @ref OCI_ERR_INVALID_HANDLE </td>  
        <td> The @a controller handle is not valid. </td> 
   </tr>
   <tr> <td> @ref OCI_ERR_INVALID_PARAMETER </td>
        <td> The @a status paramter is NULL or references an invalid address.</td> 
   </tr>
   <tr> <td> @ref OCI_ERR_DRIVER_TIMEOUT </td>
        <td> The driver for a removable hardware received no response to a request. The hardware may be removed. 
             The state of the session is undefined. All subsequent calls will return @ref OCI_ERR_DRIVER_DISCONNECTED. 
             Internal resources are locked until OCI_DestroyFlexRayController() for this handle called. </td> 
   </tr>
   <tr> <td> @ref OCI_ERR_DRIVER_DISCONNECTED </td>
        <td> The driver for a removable hardware detected a communication problem (in asynchromous communication, 
             by event signalling or during a previous call to the same hardware) and is not able to communicate to 
             the hardware any more. The state of the session is undefined. All subsequent calls to the same 
             controller will return @ref OCI_ERR_DRIVER_DISCONNECTED. Internal resources are locked until 
             OCI_DestroyFlexRayController() for this handle is called.  </td> 
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

*/

#ifndef OCI_DYNAMICDLL
OCI_DECLSPEC OCI_ErrorCode
OCI_CALL OCI_GetFlexRayControllerStatus(
    OCI_ControllerHandle controller, 
    OCI_FlexRayControllerStatusType type, 
    OCI_FlexRayControllerStatus* status);
#endif

typedef OCI_ErrorCode
(OCI_CALL *PF_OCI_GetFlexRayControllerStatus)(
    OCI_ControllerHandle controller, 
    OCI_FlexRayControllerStatusType type, 
    OCI_FlexRayControllerStatus* status);


/**
* @}
* @addtogroup GROUP_OCI_FLEXRAY_INITIALIZATION Initialization
* @{
*/

/** 
* Static read-only information of a FlexRay controller. This information will 
* never change.
*/
typedef struct OCI_FlexRayControllerCapabilities
{
   /** 
   * Number of supported parallel virtual controller accesses, when all 
   * configurations use the same values. 
   */
   uint32 sharedAccess;
   
   /**
   * Flags for all bus modes supported by the controller. Any combination of 
   * @ref OCI_FlexRayControllerBusMode is allowed.
   */
   uint32 supportedBusMode;
   
   /**
   * Flag indicating whether the controller supports WUP generation and waiting 
   * for WUPs on node start. 
   * If this feature is supported by the controller the wakeupEnabled field of 
   * the @ref OCI_FlexRayFlowConfiguration must be provided with valid values. 
   * If it is not supported the field is ignored. 
   */
   uint32 wakeupEnabledSupport;
   
   /**
   * Flag indicating whether the controller supports waiting for WUPs on channel A 
   * after performing a wakeup. 
   * If this feature is supported by the controller the waitForWUPA field of the 
   * @ref OCI_FlexRayFlowConfiguration must be provided with valid values. If it 
   * is not supported the field is ignored. 
   */
   uint32 waitForWUPASupport;
   
   /**
   * Flag indicating whether the controller supports waiting for WUPs on channel B 
   * after performing a wakeup. 
   * If this feature is supported by the controller the waitForWUPB field of the 
   * @ref OCI_FlexRayFlowConfiguration must be provided with valid values. If it 
   * is not supported the field is ignored. 
   */
   uint32 waitForWUPBSupport;
  
   /**
   * Flag indicating whether the controller supports the prohibition of entering 
   * the leading coldstart path. This feature allows to dictate a coldsart node
   * to only enter the following coldstart path or to also be able to initiate 
   * cluster communication.
   * If this feature is supported by the controller, the coldstartInhibit field 
   * of the @ref OCI_FlexRayFlowConfiguration must be provided with valid values. 
   * If it is not supported the field is ignored. 
   */
   uint32 coldstartInhibitSupport;
	
   /**
   * Flag indicating whether the controller supports infinite coldstart attempts 
   * or not. If so the value of gColdStartAttempts is annullated.
   * If this feature is supported by the controller, the repeatColdstart field 
   * of the @ref OCI_FlexRayFlowConfiguration must be provided with valid values. 
   * If it is not supported the field is ignored. 
   */
   uint32 repeatColdstartSupport;
   
   /**
   * Flag indicating whether the controller supports leaving the dead-end state 
   * "Halt" or not. 
   * If this feature is supported by the controller, the resumeAfterHalt field 
   * of the @ref OCI_FlexRayFlowConfiguration must be provided with valid values. 
   * If it is not supported the field is ignored. 
   */
   uint32 resumeAfterHaltSupport;
      

} OCI_FlexRayControllerCapabilities;


/**
 Query the capabilities of the FlexRay controller instance. 
 
 The capabilities are static information, which will never change for a specific controller instance.

 @param[in]  controller   Handle of the FlexRay controller that is queried.
 @param[out] capabilities Pointer to a buffer receiving the requested capability information of the FlexRay 
                          controller.
 
 @controllerState CREATED, SUSPENDED, RUNNING.

 @return
   <TABLE border="0" rules="rows" cellspacing="3" cellpadding="3">  
   <tr> <td> @ref OCI_SUCCESS </td> 
        <td> The operation was successful. The information in @a capabilities is valid. </td> 
   </tr>
   <tr> <td> @ref OCI_ERR_INVALID_HANDLE </td>  
        <td> The @a controller handle is not valid. </td> 
   </tr>
   <tr> <td> @ref OCI_ERR_INVALID_PARAMETER </td>
        <td> The @a capabilities paramter is NULL or references an invalid address.</td> 
   </tr>
   <tr> <td> @ref OCI_ERR_DRIVER_NO_RESPONSE </td>
        <td> The driver for a removable hardware received no response to a request. The hardware may be removed. 
             It is not known wether the request is lost and the command not exectuted or the response is lost and
             the command was executed. The controller enters the @a DISCONNECTED state. All subsequent calls will 
             return @ref OCI_ERR_DRIVER_DISCONNECTED. 
             Internal resources are locked until OCI_DestroyCANController() for the controller is called. </td> 
   </tr>
   <tr> <td> @ref OCI_ERR_DRIVER_DISCONNECTED </td>
        <td> The driver for a removable hardware detected a communication problem (in asynchromous communication, 
             by event signalling or during a previous call to the same hardware) and is not able to communicate to 
             the hardware any more. 
             The state of the controller is @a DISCONNECTED. The command is not executed. All subsequent calls to 
             the same Controller will return @ref OCI_ERR_DRIVER_DISCONNECTED. Internal resources are locked until 
             OCI_DestroyCANController() for the controller is called.  </td> 
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
OCI_CALL OCI_GetFlexRayControllerCapabilities(
    OCI_ControllerHandle controller, 
    OCI_FlexRayControllerCapabilities* capabilities);
#endif

typedef OCI_ErrorCode
(OCI_CALL *PF_OCI_GetFlexRayControllerCapabilities)(
    OCI_ControllerHandle controller, 
    OCI_FlexRayControllerCapabilities* capabilities);


/** 
* Structure containing all configuration parameters for a FlexRay transmit
* buffer. 
*/

typedef struct OCI_FlexRayTxBufferConfiguration 
{
    /** Core configuration. */
    OCI_FlexRayTxBufferCoreConfiguration core;
   
    /**
    * Number of 2-byte words reserved for payload - should match 
    * gPayloadLengthStatic in static segment.
    */
    uint32 payloadLength;
    
    /** Determines polarity of payload preamble indicator bit. */
    uint32 payloadPreambleIndicator;
    
} OCI_FlexRayTxBufferConfiguration;


/**
 * This structure describes a transmit buffer configuration.
 * It specifies the LPDU-ID in @ref core, the buffer length
 * and the owner.
 *
 * If the buffer is currently unused, @ref owner is @ref OCI_NO_HANDLE. 
 * In that case, the respective buffer index can be passed to
 * @ref OCI_CreateFlexRayTxQueue. 
 * 
 * Otherwise, the handle will only be reliable when it 
 * belongs to the client calling @ref OCI_GetFlexRayTxBufferList.
 * Handles from other clients will be reported as pseudo-handles:
 * They are only guaranteed to be invalid in this client's context
 * and to be different from @ref OCI_NO_HANDLE.
 * Depending on implementation details, they may all have the 
 * same value but there is no guarantee about that.
 */

typedef struct OCI_FlexRayTxBufferInfo
{
    /** Buffer configuration. */
    OCI_FlexRayTxBufferConfiguration txBuffer;

    /**
    * Handle of the queue that owns the buffer.
    * @ref OCI_NO_HANDLE if the buffer is currently not assigned. 
    */
    OCI_QueueHandle owner;

} OCI_FlexRayTxBufferInfo;


/**
  Reads the list of transmit buffers and their current usage.
  
  The number of buffers is fixed and will be defined when configuring the controller.
  To use one or more of these buffers, you need to create a tx queue and assign the
  buffers to it (see @ref OCI_CreateFlexRayTxQueue).
  This function can be used to find suitable buffers. The corresponding index within 
  the @ref buffers list must be passed in the @ref OCI_FlexRayTxQueueConfiguration
  structure, in reconfiguration messages and in switch messages. The absolute order of 
  the entries in @ref buffers is therefore consistent as long as the controller is not 
  reconfigured. Also, the order of the entries is identical to the order of tx buffers
  in the controller configuration structure.

  @param[in]  controller Handle of the controller.
                         The controller instance must be in the @a RUNNING or @a SUSPENDED state.
                         OCI_OpenFlexRayController() must be called prior to this function.
  @param[in]  original   If not 0, the buffers will receive the original buffer configuration.
                         Otherwise, the current controller configuration is returned. Disabled
                         buffers (see @ref OCI_FlexRaySwitchMessage) may report arbitrary 
                         LPDU-IDs in that case.
                         As soon as a buffer is no longer assigned to a queue, it will 
                         automatically be reverted to the original configuaration.
  @param[out] buffers    Pointer to an array of @a size @ref OCI_FlexRayTxBufferInfo elements 
                         to receive the buffer configurations.
                         Must not be @c NULL.
  @param[in]  size       Number of entries allocated in the @a buffers array. 
                         Must not be 0.
  @param[out] count      Number of entries actually read with 0 <= @a count <= size. 
                         If @a count == @a size, the @a buffers may have been too small to
                         receive all info. In that case, trailing entries have been omitted.
                         Must not be @c NULL.

  @return
  <TABLE border="0" rules="rows" cellspacing="3" cellpadding="3">  
   <tr> <td> @ref OCI_SUCCESS </td> 
        <td> The operation was successful. </td> 
   </tr>
   <tr> <td> @ref OCI_ERR_INVALID_HANDLE </td>  
        <td> The controller handle is not valid. </td> 
   </tr>
   <tr> <td> @ref OCI_ERR_INVALID_PARAMETER </td>
        <td> The buffers or count parameter is NULL or references to an invalid address.</td> 
   </tr>
   <tr> <td> @ref OCI_ERR_DRIVER_NO_RESPONSE </td>
        <td> The driver for a removable hardware received no response to a request. The hardware may be removed. 
             It is not known wether the request is lost and the command not exectuted or the response is lost and
             the command was executed. The controller enters the @a DISCONNECTED state. All subsequent calls will 
             return @ref OCI_ERR_DRIVER_DISCONNECTED. 
             Internal resources are locked until OCI_DestroyCANController() for the controller is called. </td> 
   </tr>
   <tr> <td> @ref OCI_ERR_DRIVER_DISCONNECTED </td>
        <td> The driver for a removable hardware detected a communication problem (in asynchromous communication, 
             by event signalling or during a previous call to the same hardware) and is not able to communicate to 
             the hardware any more. 
             The state of the controller is @a DISCONNECTED. The command is not executed. All subsequent calls to 
             the same Controller will return @ref OCI_ERR_DRIVER_DISCONNECTED. Internal resources are locked until 
             OCI_DestroyCANController() for the controller is called.  </td> 
   </tr>
   <tr> <td> @ref OCI_ERR_OUT_OF_MEMORY </td>
        <td> The driver ran out of memory. This error should only occur on heavy loaded 
             systems with multiple clients and large send and receive queues. The error may indicate a resource 
             leak in the driver implementation.
             Dependent on the Localization of the Error Code the required actions to resolve the error condition
             may be different. See also @ref ERROR_LOCALIZATION</td> 
   </tr>

   </TABLE> 
*/

#ifndef OCI_DYNAMICDLL
OCI_DECLSPEC OCI_ErrorCode
OCI_CALL OCI_GetFlexRayTxBufferList(
    OCI_ControllerHandle controller, 
    OCI_bool original,
    OCI_FlexRayTxBufferInfo buffers[],
    uint32 size,
    uint32* count);
#endif

typedef OCI_ErrorCode
(OCI_CALL *FP_OCI_GetFlexRayTxBufferList)(
    OCI_ControllerHandle controller, 
    OCI_bool original,
    OCI_FlexRayTxBufferInfo buffers[],
    uint32 size,
    uint32* count);

/**
* @}
* @addtogroup GROUP_OCI_FLEXRAY_MESSAGES Messages
* @{
*/


/**
* @anchor  ANCHOR_OCI_FLEXRAY_MESSAGE_FLAGS
* @name    OCI FlexRay Message Flags.
* @brief   Flags to specify additional information of a FlexRay message.
*          These are possible values of @ref OCI_FlexRayRxMessage.flags.
*/

/** @{ */
/**
* The message is a self reception frame.
*/
#define OCI_FLEXRAY_MSG_FLAG_SELFRECEPTION      (1 << 0)

/** @} */


/**
* @anchor  ANCHOR_OCI_FLEXRAY_FRAMEINDICATORS_FLAGS
* @name    FlexRay frame indicator flags
* @brief   These flags specify the location of the respective frame indicators.
*/

/** @{ */

/**
* Startup frame indicator. If the startup frame indicator is set to zero then 
* the frame is not a startup frame. If the startup frame indicator is set to one
* then the frame is a startup frame.
*/
#define OCI_FLEXRAY_STARTUP_FRAME_INDICATOR     (1 << 0)

/**
* Sync frame indicator. If the sync frame indicator is set to zero then no 
* receiving node considers the frame for synchronization. If the sync frame 
* indicator is set to one then all receiving nodes utilize the frame for 
* synchronization.
*/
#define OCI_FLEXRAY_SYNC_FRAME_INDICATOR        (1 << 1)

/**
* Null frame indicator. If the null frame indicator is set to zero then the 
* payload segment contains no valid data. All bytes in the payload section are 
* set to zero. If the null frame indicator is set to one then the payload 
* segment contains data. 
*/
#define OCI_FLEXRAY_NULL_FRAME_INDICATOR        (1 << 2)

/**
* Payload preamble indicator. If the payload preamble indicator is set to one 
* then the payload segment of the frame contains a network management vector if 
* it is transmitted in the static segment or a message ID if it is transmitted 
* in the dynamic segment.
*/
#define OCI_FLEXRAY_PAYLOAD_PREAMBLE_INDICATOR  (1 << 3)

/** 
* Reserved indicator. Currently this is a reserved feature: A transmitting node 
* shall set the reserved bit to logical '0'. A receiving node shall ignore the 
* reserved bit. 
*/
#define OCI_FLEXRAY_RESERVED_INDICATOR          (1 << 4)

/** @} */


/** Specific structure for FlexRay bus data frames. */
typedef struct OCI_FlexRayRxMessage
{
    /** 
    * Global receive time stamp. Depending on the implementation, this may be 
    * based on a hardware-internal timer with no global synchronization. 
    * In particular, the epoch (i.e. value 0) may be the point in time the 
    * hardware was switched on.
    */
    OCI_Time timeStamp;

    /** Filter tag ID. This ID is set by the definition of the RxFilter. */
    uint32 tag;

    /** Reserved flags. */
    uint16 flags;
    
    /**
    * Communication controller indicators.
    * @flags    The following flags are valid:
    * @n        @ref OCI_FLEXRAY_CHANNEL_A
    * @n        @ref OCI_FLEXRAY_CHANNEL_B
    * @sa       @ref ANCHOR_OCI_FLEXRAY_CC_INDICATORS 
    *           "FlexRay communication controller indicator flags"
    */
    uint16 ccIndicators; 

    /** 
    * Frame indicators, as received in the frame-header. 
    * @flags    The following flags are valid:
    * @n        @ref OCI_FLEXRAY_RESERVED_INDICATOR
    * @n        @ref OCI_FLEXRAY_PAYLOAD_PREAMBLE_INDICATOR
    * @n        @ref OCI_FLEXRAY_NULL_FRAME_INDICATOR
    * @n        @ref OCI_FLEXRAY_SYNC_FRAME_INDICATOR
    * @n        @ref OCI_FLEXRAY_STARTUP_FRAME_INDICATOR
    * @sa       @ref ANCHOR_OCI_FLEXRAY_FRAMEINDICATORS_FLAGS
    *           "FlexRay frame indicator flags" 
    */
    uint16 frameIndicators;         
    
    /** 
    * The frame ID defines the slot in which the frame was received.
    * @values   Valid values are 0x0001 .. 0x07ff.
    */
    uint16 frameID;
        
    /** 
    * The payload length in double-bytes, as received in the frame-header. 
    * @values   Valid values are 0 .. 127.
    */
    uint16 payloadLength;   

    /** 
    * CRC of the received frame-header
    * @values   Valid values are 0x0000 .. 0x07ff.
    */
    uint16 headerCrc;       

    /** 
    * FlexRay cycle in which this frame was received.
    * @values   Valid values are 0 .. 63.
    */
    uint16 cycleCount;
    
    /** 
    * Number of valid data bytes.
    * @values   Valid values are 0 .. 254.
    */
    uint16 size;  

    /** Payload. Only the first @ref size elements are valid. */
    uint8 data[254];             

} OCI_FlexRayRxMessage;


/** Events that may be fired by the FlexRay controller. */
typedef enum OCI_FlexRayEvent
{
    /** The static slot portion of a new cycle has started. */
    OCI_FLEXRAY_EVENT_START_OF_STATIC_SEGMENT = 1,
    
    /** The dynamic slot portion of a new cycle has started */
    OCI_FLEXRAY_EVENT_START_OF_DYNAMIC_SEGMENT = 2,
    
    /** The status of the communication controller has changed */
    OCI_FLEXRAY_EVENT_POCS_CHANGE = 3,
    
    /** One or more frames got lost.
    * Use this event only when the data was lost on the bus. If the data was lost due to
    * queue overflow, use an instance of @ref OCI_QueueEventMessage instead.
    */
    OCI_FLEXRAY_EVENT_DATA_LOST = 4
    
} OCI_FlexRayEvent;


/**
* Message that is generated when a FlexRay event fires and a filter for that 
* event was set.
*/
typedef struct OCI_FlexRayEventMessage
{
    /** 
    * Global receive time stamp. Depending on the implementation, this may be 
    * based on a hardware-internal timer with no global synchronization. In 
    * particular, the epoch (i.e. value 0) may be the point in time the hardware
    * was switched on.
    */
    OCI_Time timeStamp;

    /** Filter tag ID. This ID is set by the definition of the RxFilter. */
    uint32 tag;

    /** Code for the event that generated this message. */
    OCI_FlexRayEvent eventCode;

    /** 
    * Destination of the event. The event message will use the combination of 
    * @ref OCI_EVENT_DESTINATION_INBAND and @ref OCI_EVENT_DESTINATION_CALLBACK 
    * that was registered for this event by calling 
    * @ref OCI_AddFlexRayEventFilter. 
    */
    uint32 destination;

    /** Protocol operation control status, when the event fired. */
    OCI_FlexRayControllerProtocolOperationControlStatus \
        protocolOperationControlStatus;    

} OCI_FlexRayEventMessage;


/** Specific structure for FlexRay bus data frames, transmit direction. */
typedef struct OCI_FlexRayTxMessage
{
    /** Reserved fields adapting to layout of OCI_FlexRayRxMessage. */
    uint32 reserved[6];

    /** Additional information and settings of the FlexRay data message. */    
    uint16 flags;
            
    /** Number of valid data bytes. */
    uint16 size;             
    
    /** Payload. Only the first @a size elements are valid. */
    uint8 data[254];             
   
} OCI_FlexRayTxMessage;


/** 
 * Specific structure for reconfiguration messages, which allow partial 
* reconfiguration of the communication controller's transmit buffers.
 * To allow proper synchronization with the transmit queue contents,
 * buffer reconfiguration will be triggered by this in-band tx message.
 *
 * The buffer must have been assigned to the respective tx queue, 
 * see @ref OCI_CreateFlexRayTxQueue. Also, the buffer must currently be 
 * active, see @ref OCI_FlexRaySwitchMessage.
 *
 * A buffer reconfiguration is always temporary and will be lost as
 * soon as either the buffer is disabled or the respective queue is 
 * destroyed by @ref OCI_DestroyFlexRayTxQueue.
 *
 * @ref OCI_WriteFlexRayData will return @ref OCI_ERR_INCOMPATIBLE_CONFIG
 * if the controller does not support the requested new buffer configuration.
 * @ref OCI_ERR_INCONSISTENT_PARAMETER_SET will be returned for invalid
 * values in @ref bufferIndex.
*/

typedef struct OCI_FlexRayReconfigurationMessage
{
    /** Index of the buffer to reconfigure. */
    uint32 bufferIndex;

    /** 
     * New frame buffer configuration, i.e. its position
     * within the transmit controller's schedule (channel, 
     * slot, cycle period and cycle offset), its length and
     * its new payload preamble indicator bit.
     * In certain cases a different payload length may not 
     * be supported by the implementation.
     */
    OCI_FlexRayTxBufferConfiguration newConf;

} OCI_FlexRayReconfigurationMessage;


/**
 * Specific structure for in-band messages, which allow enabling and
 * disabling individual transmit buffers. Since most controllers will
 * only be able to @em receive data when they are not transmitting,
 * this is the only way to switch between send and receive mode for
 * a given (channel, slot, period, offset) tuple.
 * 
 * To allow proper synchronization with the transmit queue contents,
 * buffer reconfiguration will be triggered by this in-band tx message.
 * Multiple attempts to disable/ enable an already disabled/ enabled 
 * buffer will have no effect.
 *
 * Internally, the driver implementation of @ref OCI_WriteFlexRayData 
 * may use a @ref OCI_FlexRayReconfigurationMessage to assign the
 * buffer to some non-occuring slot to mimic the "disable buffer"
 * behavior. Upon re-enabling it, however, it must revert to the previous
 * state.
 *
 * The @ref bufferIndex must be assigned to the respective controller
 * instance. @ref OCI_WriteFlexRayData will return 
 * @ref OCI_ERR_INCONSISTENT_PARAMETER_SET for invalid values in @ref bufferIndex.
*/

typedef struct OCI_FlexRaySwitchMessage
{
    /** Index of the buffer to reconfigure. */
    uint32 bufferIndex;

    /** 0 to disable the transmission buffer, 1 to enable it. */
    uint32 switchOn;
} OCI_FlexRaySwitchMessage;


/** Union for all different types of FlexRay messages. */
typedef union OCI_FlexRayMessageData 
{   
    /** 
    * Data of a FlexRay message. Use this part, if 
    * @ref OCI_FlexRayMessageDataType is @ref OCI_FLEXRAY_RX_MESSAGE.
    */
    OCI_FlexRayRxMessage rxMessage;

    /** 
    * Data of a FlexRay message. Use this part, if 
    * @ref OCI_FlexRayMessageDataType is @ref OCI_FLEXRAY_TX_MESSAGE.
    */
    OCI_FlexRayTxMessage txMessage;

    /** 
    * Data of a FlexRay buffer reconfiguration message. Use this part, 
    * if  @ref OCI_FlexRayMessageDataType is 
    * @ref OCI_FLEXRAY_RECONFIGURATION_MESSAGE.
    */
    OCI_FlexRayReconfigurationMessage reconfigurationMessage;
    
    /** 
    * Data of a FlexRay buffer switch message. Use this part, if 
    * @ref OCI_FlexRayMessageDataType is 
    * @ref OCI_FLEXRAY_SWITCH_MESSAGE.
    */
    OCI_FlexRaySwitchMessage switchMessage;
    
    /** 
    * Data of a FlexRay message. Use this part, if 
    * @ref OCI_FlexRayMessageDataType is @ref OCI_FLEXRAY_EVENT.
    */
    OCI_FlexRayEventMessage flexRayEventMessage;

    /** 
    * Data of a FlexRay message. Use this part, if 
    * @ref OCI_FlexRayMessageDataType is @ref OCI_FLEXRAY_INTERNAL_ERROR_EVENT.
    */
    OCI_InternalErrorEventMessage internalErrorEventMessage;
    
    /** 
    * Data of a FlexRay message. Use this part, if 
    * @ref OCI_FlexRayMessageDataType is @ref OCI_FLEXRAY_TIMER_EVENT.
    */
    OCI_TimerEventMessage timerEventMessage;

    /** 
    * Data of a FlexRay message. Use this part, if 
    * @ref OCI_FlexRayMessageDataType is @ref OCI_FLEXRAY_QUEUE_EVENT.
    */
    OCI_QueueEventMessage queueEventMessage;

} OCI_FlexRayMessageData;


/** 
* Discriminator for the different FlexRay messages in the 
* @ref OCI_FlexRayMessageData union 
*/
typedef enum OCI_FlexRayMessageDataType
{
    /** Message type is @ref OCI_FlexRayRxMessage. */
    OCI_FLEXRAY_RX_MESSAGE = 1,

    /** Message type is @ref OCI_FlexRayTxMessage. */
    OCI_FLEXRAY_TX_MESSAGE = 2,

    /** Message type is @ref OCI_FlexRayReconfigurationMessage. */
    OCI_FLEXRAY_RECONFIGURATION_MESSAGE = 3,

    /** Message type is @ref OCI_FlexRaySwitchMessage. */
    OCI_FLEXRAY_SWITCH_MESSAGE = 4,

    /** Message type is @ref OCI_FlexRayEventMessage. */
    OCI_FLEXRAY_EVENT = 5,

    /** Message type is @ref OCI_InternalErrorEventMessage. */
    OCI_FLEXRAY_INTERNAL_ERROR_EVENT = 6,

    /** Message type is @ref OCI_TimerEventMessage. */
    OCI_FLEXRAY_TIMER_EVENT = 7,
    
    /** Message type is @ref OCI_QueueEventMessage. */
    OCI_FLEXRAY_QUEUE_EVENT = 8

} OCI_FlexRayMessageDataType;


/**
* Container for a discriminated union passed to the @ref OCI_WriteFlexRayData and 
* @ref OCI_ReadFlexRayData function.
*/
typedef struct OCI_FlexRayMessage
{    
    /**
    * Discriminator of the @ref OCI_FlexRayMessageData. 
    * @values   The following values are valid:
    * @n        @ref OCI_FLEXRAY_RX_MESSAGE for OCI_FlexRayRxMessage
    * @n        @ref OCI_FLEXRAY_TX_MESSAGE for OCI_FlexRayTxMessage
    * @n        @ref OCI_FLEXRAY_RECONFIGURATION_MESSAGE for 
    *           OCI_FlexRayReconfigurationMessage
    * @n        @ref OCI_FLEXRAY_EVENT for OCI_FlexRayEventMessage
    * @n        @ref OCI_FLEXRAY_INTERNAL_ERROR_EVENT for 
    *           OCI_InternalErrorEventMessage
    * @n        @ref OCI_FLEXRAY_TIMER_EVENT for OCI_TimerEventMessage
    * @n        @ref OCI_FLEXRAY_QUEUE_EVENT for OCI_QueueEventMessage
    * @sa       @ref OCI_FlexRayMessageDataType
    */
    OCI_FlexRayMessageDataType type;

	/** reserved parameter necessary for 64 Bit alignment of OCI_FlexRayMessageData */
	uint32 reserved;

    /** Data of the message dependent on the @a type of the message. */
    OCI_FlexRayMessageData data;

} OCI_FlexRayMessage;


/**
* @}
* @addtogroup GROUP_OCI_FLEXRAY_QUEUES Queues
* @{
*/

/**
 Signature of a callback function for FlexRay event and message handling. The callback function registered by the
 user must have this signature to receive FlexRay Frames and FlexRay Events.

 @param[in] userData Pointer to a user defined structure or handle for user supplied information. Any callback
                     of a single instance will use the same value, that is supplied during the callback registration
                     in the @ref OCI_CANRxCallback structure during execution of OCI_CreateCANRxQueue().
 @param[in] msg      Pointer to the received message or event. The object is owned by the caller of the API. The
                     reference must not used after completion of the callback. When the user requires access to the
                     data after completion of the callback the callback must create a copy of the data.
                     
                     @todo verify implementation of resource ownership and description!

 @todo 2009-01-28: define OCI_DOCU to expand OCI_CALLBACK to nothing --> correct signature
*/
typedef void (OCI_CALLBACK *OCI_FlexRayRxCallbackFunction)(void *userData, OCI_FlexRayMessage* msg);

/** 
* Specific structure to define a callback of a FlexRay receive queue 
* to handle events and frames.
*/
typedef struct OCI_FlexRayRxCallback
{
    /** Callback function pointer. */
    OCI_FlexRayRxCallbackFunction function;

    /**
    * Opaque context information that will be passed to the callback 
    * @a function. The pointer is supplied by the caller and not 
    * interpreted or modified by the OCD implementation. 
    * May be @c NULL.
    */
    void *userData;

} OCI_FlexRayRxCallback;


/** 
* Structure containing the configuration of a FlexRay receive queue. This 
* structure must be filled in, when @ref OCI_CreateFlexRayRxQueue is called. The
* settings of this configuration are valid for the life-time of the queue.
*/
typedef struct OCI_FlexRayRxQueueConfiguration 
{
    /** 
    * Callback function for the reception of a FlexRay frame.
    *
    * When a frame arrives which matches one of the frame filters defined for the queue instance,
    * there are two possibilities:
    *   - If @ref onFrame is not @c NULL, the frame is passed to @ref onFrame.
    *   - If @ref onFrame is @c NULL, the frame is added to the RX queue.
    *
    * Note that if @ref OCI_DestroyFlexRayRxQueue() is used to destroy the queue while reception is in progress,
    * it is possible for @ref onFrame to be called a short time @em after OCI_DestroyFlexRayRxQueue() has returned
    * successfully.
    *
    * @sa @ref PAGE_OCI_RECEIVE_MESSAGES "Receive Messages"
    */
    OCI_FlexRayRxCallback onFrame;

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
    *   - If @ref OCI_DestroyFlexRayRxQueue() is used to destroy the queue while reception is in progress, it is
    *     possible for @ref onEvent to be called a short time @em after OCI_DestroyFlexRayRxQueue() has returned
    *     successfully.
    *   - Setting only @ref onEvent but not @ref onFrame allows a lazy reaction on frame reception and a
    *     quick reaction on every enabled event.
    *
    * @sa   @ref PAGE_OCI_RECEIVE_MESSAGES "Receive Messages"
    * @sa   OCI_QueueEventFilter
    */
    OCI_FlexRayRxCallback onEvent;

    /**
    * Specify whether self-recepted frames shall be forwarded to this queue.
    * @note        This flag will be combined with the respective
    *              @ref OCI_FlexRayControllerBusMode setting.
    * @values      The following values are valid:
    * @n           OCI_SELF_RECEPTION_OFF
    * @n           OCI_SELF_RECEPTION_ON
    * @sa          @ref OCI_SelfReceptionMode
    *              "OCI Self Reception Mode"
    */
    OCI_SelfReceptionMode selfReceptionMode;

} OCI_FlexRayRxQueueConfiguration;

/** 
* Structure containing the configuration of a FlexRay transmit queue. This 
* structure must be filled in, when @ref OCI_CreateFlexRayTxQueue is called. The
* settings of this configuration are valid for the life-time of the queue.
*/

typedef struct OCI_FlexRayTxQueueConfiguration 
{
    /** Number of entries in @a buffers. Must not be 0. */
    uint32 count;
    
    /**
    * Array with transmit buffer indexes. Those must be a valid index in the
    * buffer list returned by @ref OCI_GetFlexRayTxBufferList, must be unique 
    * and must not overlap with buffers used by other queues on the same 
    * controller (not just this controller instance). 
    */
    uint32 bufferIndexes[OCI_FLEXRAY_MAX_BUFFERCOUNT];
    
} OCI_FlexRayTxQueueConfiguration;


/** 
  Create a FlexRay transmit queue. 
  
  A transmit queue is requierd to send FlexRay frames.
  FlexRay controllers support multiple transmit queues. Any queue locks exclusive access to a single or multiple 
  send buffer. When a queue accesses multiple send buffer the queued data may be multiplexed to all allocated 
  buffer to increase the usable bandwidth. The driver may not implement this optimization and use only a single 
  buffer for transmission.

  Different queues are not synchronized by the API. Any data written to a single queue will occur in order
  on the bus interface, but data written to different queues may occur in a different order on the interface.
  The order depends on the filling level of the different queues and scheduling algorithm of the FlexRay Bus
  configuration.

  @controllerState SUSPENDED, RUNNING.

  @param[in]  controller     Handle of the controller instance. 
  @param[in]  configuration  The configuration of the transmit queue. 
                             Currently reserved, must be set to @c NULL.
  @param[out] queue          Receives the handle of the newly created transmit queue. 

  @return
   <TABLE border="0" rules="rows" cellspacing="3" cellpadding="3">  
   <tr> <td> @ref OCI_SUCCESS </td> 
        <td> The operation was successful. The @a queue is created and can be used for data transmission. </td> 
   </tr>
   <tr> <td> @ref OCI_ERR_INVALID_HANDLE </td>  
        <td> The @a controller handle is not valid. </td> 
   </tr>
   <tr> <td> @ref OCI_ERR_INVALID_PARAMETER </td>
        <td> The @a configuration paramter or queue parameter is NULL or references an invalid address.</td> 
   </tr>
   <tr> <td> @ref OCI_ERR_INVALID_STATE </td>
        <td> The controller is not in the @a SUSPENDED or @a RUNNING state. Probably a call of 
             OCI_OpenFlexRayController() is missed.  </td> 
   </tr>
   <tr> <td> @ref OCI_ERR_DRIVER_NO_RESPONSE </td>
        <td> The driver for a removable hardware received no response to a request. The hardware may be removed. 
             It is not known wether the request is lost and the command not exectuted or the response is lost and
             the command was executed. The controller enters the @a DISCONNECTED state. All subsequent calls will 
             return @ref OCI_ERR_DRIVER_DISCONNECTED. 
             Internal resources are locked until OCI_DestroyCANController() for the controller is called. </td> 
   </tr>
   <tr> <td> @ref OCI_ERR_DRIVER_DISCONNECTED </td>
        <td> The driver for a removable hardware detected a communication problem (in asynchromous communication, 
             by event signalling or during a previous call to the same hardware) and is not able to communicate to 
             the hardware any more. 
             The state of the controller is @a DISCONNECTED. The command is not executed. All subsequent calls to 
             the same Controller will return @ref OCI_ERR_DRIVER_DISCONNECTED. Internal resources are locked until 
             OCI_DestroyCANController() for the controller is called.  </td> 
   </tr>
   <tr> <td> @ref OCI_ERR_OUT_OF_MEMORY </td>
        <td> The driver run out of memory. This error should only occur on heavy loaded systems with multiple 
             clients and large send and receive queues. The error may indicate a resource leak in the driver 
             implementation. When multiple application access the same resource (interface hardware or physical
             controller) the termination of a different application may return the resources required for this 
             instance. </td> 
   </tr>
   <tr> <td> @ref OCI_ERR_UNEXPECTED_NULL </td>
        <td> An unexpected implementation error in the driver occured. </td> 
   </tr>
  </TABLE> 
*/
#ifndef OCI_DYNAMICDLL
OCI_DECLSPEC OCI_ErrorCode
OCI_CALL OCI_CreateFlexRayTxQueue(
    OCI_ControllerHandle controller,
    OCI_FlexRayTxQueueConfiguration* configuration,
    OCI_QueueHandle* queue);
#endif

typedef OCI_ErrorCode
(OCI_CALL *PF_OCI_CreateFlexRayTxQueue)(
    OCI_ControllerHandle controller,
    OCI_FlexRayTxQueueConfiguration* configuration,
    OCI_QueueHandle* queue);


/** 
  Queue FlexRay frames for transmission on a FlexRay-Bus. 
  
  The function returns, when all frames can be placed in the transmit queue. Since the transmit queue has a limited 
  capacity, this function may need to block until the last entry could be placed into the queue. A @ref timeout
  can be specified to limit the blocking.
  
  There is no information, when the frames are send on the FlexRay bus. There may be a delay between storing the 
  frames in the queue and sending them, because of the scheduling of the FlexRay-Bus. Especially frames in the 
  dynamic segment may be stalled until the slot is scheduled. A fatal error on the FlexRay-Bus may occur
  before the controller was able to send the data.

  If an error occurs before all frames could be placed into the controller transmit queue, this function will 
  return immediately with an error code. It will not try to push the remaining messages into the queue. Frames
  that are already queued, will not be removed.

  The frames of a single queue will occur in order on the FlexRay-Bus. Transmission of frames from other queues 
  are not synchronized to the queue. 

  For the same reason, transmit buffer reconfiguration must use in-band messages making \ref OCI_WriteFlexRayData
  the only way to modify the otherwise static controller setup.

  
  @controllerState SUSPENDED, RUNNING. \n
                   When the controller is in the CREATED state no valid Queue can exist and the call will return
                   @ref OCI_ERR_INVALID_HANDLE.

  @param[in]  queue     Handle of the @a queue that will transmit the frames. 
  @param[in]  timeout   Maximum time (delta, not global time stamp) to wait for messages to be queued.
                        Reconfiguration messages must be processed within that time slot as they 
                        require queues to be emptied and cannot be queued themselves.
                        The timeout uses the same resolution as the controller timestamp. The resolution may vary
                        between different driver for different hardware. The interval may be increased to the time
                        slice(s) used by the Operating System and jitter in this magnitude.
                        Use @ref OCI_NO_TIME to wait indefinitely if necessary. 
                        0 will queue as many messages as possible without waiting for them to be transmitted.
  @param[in]  messages  A pointer to an array of @a size @ref OCI_FlexRayMessage elements. 
  @param[in]  size      The number of entries in the @a messages array. 
  @param[out] remaining Number of entries not processed. Will be smaller or equal to @a size. 
                        May be @c NULL.
  @return     
  <TABLE border="0" rules="rows" cellspacing="3" cellpadding="3">  
   <tr> <td> @ref OCI_SUCCESS </td> 
        <td> The operation was successful. All @a messages are queued and will be send, when they are scheduled
             by the FlexRay-Bus. </td> 
   </tr>
   <tr> <td> @ref OCI_ERR_INVALID_HANDLE </td>  
        <td> The @a queue handle is not valid. </td> 
   </tr>
   <tr> <td> @ref OCI_ERR_INVALID_PARAMETER </td>
        <td> The @a messages paramter is NULL or references an invalid address.</td> 
   </tr>
   <tr> <td> @ref OCI_ERR_INCONSISTENT_PARAMETER_SET </td> 
        <td> At least one message has an invalid format. 
             (e.g. messages[i].OCI_FlexRayMessageDataType not in 
             {OCI_FLEXRAY_TX_MESSAGE, OCI_FLEXRAY_RECONFIGURATION_MESSAGE, 
             OCI_FLEXRAY_TX_SWITCH_MESSAGE}). </td> 
   </tr>
   <tr> <td> @ref OCI_ERR_INCOMPATIBLE_CONFIG </td> 
        <td> An in-band transmit buffer reconfiguration failed because the same LPDU-ID has already
             been assigned to a different buffer and the HW controller does not support it. Most controllers 
             allow for at most one transmit buffer per (channel, slot, cycle, offset) tuple. Further
             controller restrictions may apply. All previous messages for the respective buffer have been 
             queued and will be sent. If there have been other reconfiguration messages, those have been
             processed successfully. All entries in \ref messages following the offending one have been ignored.
             The failed buffer retains its previous configuration. </td> 
   </tr>
   <tr> <td> @ref OCI_ERR_INVALID_CONFIGURATION </td>
        <td> At least one in-band transmit buffer reconfiguration or (de-)activation message has an invalid
             buffer index.</td> 
   </tr>
   <tr> <td> @ref OCI_ERR_INVALID_STATE </td>
        <td> The controller is not in the @a SUSPENDED or @a RUNNING state. Probably a call of 
             OCI_OpenFlexRayController() is missed.  </td> 
   </tr>
   <tr> <td> @ref OCI_ERR_DRIVER_NO_RESPONSE </td>
        <td> The driver for a removable hardware received no response to a request. The hardware may be removed. 
             It is not known wether the request is lost and the command not exectuted or the response is lost and
             the command was executed. The controller enters the @a DISCONNECTED state. All subsequent calls will 
             return @ref OCI_ERR_DRIVER_DISCONNECTED. 
             Internal resources are locked until OCI_DestroyCANController() for the controller is called. </td> 
   </tr>
   <tr> <td> @ref OCI_ERR_DRIVER_DISCONNECTED </td>
        <td> The driver for a removable hardware detected a communication problem (in asynchromous communication, 
             by event signalling or during a previous call to the same hardware) and is not able to communicate to 
             the hardware any more. 
             The state of the controller is @a DISCONNECTED. The command is not executed. All subsequent calls to 
             the same Controller will return @ref OCI_ERR_DRIVER_DISCONNECTED. Internal resources are locked until 
             OCI_DestroyCANController() for the controller is called.  </td> 
   </tr>
   <tr> <td> @ref OCI_ERR_OUT_OF_MEMORY </td>
        <td> The driver run out of memory. This error should only occur on heavy loaded systems with multiple 
             clients and large send and receive queues. The error may indicate a resource leak in the driver 
             implementation. When multiple application access the same resource (interface hardware or physical
             controller) the termination of a different application may return the resources required for this 
             instance. </td> 
   </tr>
   <tr> <td> @ref OCI_ERR_QUEUE_IS_FULL </td>
        <td> The specified @a queue is full; at least one message could not be added to the queue. </td> 
   </tr>
   </TABLE> 

*/

#ifndef OCI_DYNAMICDLL
OCI_DECLSPEC OCI_ErrorCode
OCI_CALL OCI_WriteFlexRayData(
    OCI_QueueHandle queue,
    OCI_Time timeout, 
    OCI_FlexRayMessage messages[], 
    uint32 size,
    uint32 *remaining);
#endif

typedef OCI_ErrorCode
(OCI_CALL *PF_OCI_WriteFlexRayData)(
    OCI_QueueHandle queue,
    OCI_Time timeout, 
    OCI_FlexRayMessage messages[], 
    uint32 size,
    uint32 *remaining);

/**
 Destroy a FlexRay transmit queue.

 The transmission Queue of a FlexRay controller is destroyed and all resources used by the queue are released. If the
 queue is not empty and some frames are waiting for transmission the frames are silently discarded. 
 
 If the application wants to be sure, that all frames are send before the queue is destroyed it can use the 
 OCI_GetQueueStatus() command to wait for an empty queue before calling this command. 
 
 @note Frames already stored in the send register of a FlexRay controller, but stalled by the scheduling mechanism
       (especially in the dynamic segment) may be sent, even when this call already returned and the send
       queue is invalid.
 
 @controllerState SUSPENDED, RUNNING. \n
                  When the controller is in the CREATED state no valid Queue can exist and the call will return
                  @ref OCI_ERR_INVALID_HANDLE.  \n
                  When the controller is in the DISCONNECTED state the function may be called to do local clean
                  up. Only a limitied plausibility check may be done in this case.  

 @param[in] queue The handle of the queue to destroy. 
                  The handle is invalid after this call and must not be used any longer. 

 @return
   <TABLE border="0" rules="rows" cellspacing="3" cellpadding="3">  
   <tr> <td> @ref OCI_SUCCESS </td> 
        <td> The operation was successful. The @a queue handle is invalid after this call and must not be used 
             any longer. </td> 
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
   <tr> <td> @ref OCI_ERR_UNEXPECTED_NULL </td>
        <td> An unexpected implementation error occured. </td> 
   </tr>
  </TABLE> 
*/

#ifndef OCI_DYNAMICDLL
OCI_DECLSPEC OCI_ErrorCode
OCI_CALL OCI_DestroyFlexRayTxQueue(
    OCI_QueueHandle queue);
#endif

typedef OCI_ErrorCode
(OCI_CALL *PF_OCI_DestroyFlexRayTxQueue)(
    OCI_QueueHandle queue);


/** 
  Create a receive queue or register callback functions for frame reception for a specific FlexRay controller. 
  
  FlexRay controllers allow a single receive queue per controller. The configuration parameter allows the
  registration of a callback function for frames and a callback function for events. When these callbacks are
  set to NULL the data will be queued and must be read by OCI_ReadFlexRayData(). When the callbacks are set
  the data will not be stored in the queue, but a reference is passed to the callback function whenever a frame
  or event is received. 

  All Frames and Events must pass a white filter to be stored in the queue. After creation of a receive queue the 
  queue will not receive any data, because no white filter is active and all data is blocked. The filter conditions 
  must be set by OCI_AddFlexRayFrameFilter() and OCI_AddFlexRayEventFilter(). 

  @controllerState SUSPENDED, RUNNING.

  @param[in]  controller     Handle of the controller instance. 
  @param[in]  configuration  Pointer to the buffer that contains the configuration structure. 
  @param[out] queue          Handle of the newly created receive queue. 

  @return
   <TABLE border="0" rules="rows" cellspacing="3" cellpadding="3">  
   <tr> <td> @ref OCI_SUCCESS </td> 
        <td> The operation was successful. 
             The @a queue handle is vaild and must be used in any subsequent call accessing the queue. </td> 
   </tr>
   <tr> <td> @ref OCI_ERR_INVALID_HANDLE </td>  
        <td> The @a controller handle is not valid. </td> 
   </tr>
   <tr> <td> @ref OCI_ERR_INVALID_PARAMETER </td>
        <td> The @a configuration paramter or @a queue parameter is NULL or references an invalid address.</td> 
   </tr>
   <tr> <td> @ref OCI_ERR_INVALID_STATE </td>
        <td> The controller is not in the @a SUSPENDED or @a RUNNING state. Probably a call of 
             OCI_OpenFlexRayController() is missed.  </td> 
   </tr>
   <tr> <td> @ref OCI_ERR_DRIVER_NO_RESPONSE </td>
        <td> The driver for a removable hardware received no response to a request. The hardware may be removed. 
             It is not known wether the request is lost and the command not exectuted or the response is lost and
             the command was executed. The controller enters the @a DISCONNECTED state. All subsequent calls will 
             return @ref OCI_ERR_DRIVER_DISCONNECTED. 
             Internal resources are locked until OCI_DestroyCANController() for the controller is called. </td> 
   </tr>
   <tr> <td> @ref OCI_ERR_DRIVER_DISCONNECTED </td>
        <td> The driver for a removable hardware detected a communication problem (in asynchromous communication, 
             by event signalling or during a previous call to the same hardware) and is not able to communicate to 
             the hardware any more. 
             The state of the controller is @a DISCONNECTED. The command is not executed. All subsequent calls to 
             the same Controller will return @ref OCI_ERR_DRIVER_DISCONNECTED. Internal resources are locked until 
             OCI_DestroyCANController() for the controller is called.  </td> 
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
*/

#ifndef OCI_DYNAMICDLL
OCI_DECLSPEC OCI_ErrorCode
OCI_CALL OCI_CreateFlexRayRxQueue(
    OCI_ControllerHandle controller,
    OCI_FlexRayRxQueueConfiguration* configuration,
    OCI_QueueHandle* queue);
#endif

typedef OCI_ErrorCode
(OCI_CALL *PF_OCI_CreateFlexRayRxQueue)(
    OCI_ControllerHandle controller,
    OCI_FlexRayRxQueueConfiguration* configuration,
    OCI_QueueHandle* queue);


/** 
  Read entries from a receive queue or wait for a timeout when less than requested entries are available. 
  
  The returned number of entries may be less than the requested number. A non blocking call can be executed,
  when timeout is set to 0. Even when waiting a infinite time for the entries the function may return with
  less data. 

  This function delivers as long queued data as possible. When the integrity of the returned data 
  can be guaranteed the function will not return any error, even when the controller or some part of the driver
  entered an error state. The function will not block, when the driver is in an error state. In this case it
  delivers the remaining data (may be less than requested) without an error code or - when empty - the error code.

  @controllerState SUSPENDED, RUNNING. \n
                   When the controller is in the CREATED state no valid Queue can exist and the call will return
                   @ref OCI_ERR_INVALID_HANDLE.


  @param[in]  queue      Handle of the receive queue.
  @param[in]  timeout    Maximum time (delta, not global time stamp) to wait at the queue to receive @a size entries.
                         The timeout uses the same resolution as the controller timestamp. The resolution may vary
                         between different driver for different hardware. The interval may be increased to the time
                         slice(s) used by the Operating System and jitter in this magnitude.
                         Use @ref OCI_NO_TIME to wait indefinitely if necessary. 
                         0 will fetch the queue's current content.
  @param[out] messages   Pointer to an array of @a size @ref OCI_FlexRayMessage elements to receive the entries from 
                         the receive queue. 
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
        <td> The operation was successful. @a count entries of the @a messages array are filled with vaild data.</td> 
   </tr>
   <tr> <td> @ref OCI_ERR_INVALID_HANDLE </td>  
        <td> The queue handle is not valid. </td> 
   </tr>
   <tr> <td> @ref OCI_ERR_INVALID_PARAMETER </td>
        <td> The messages or count parameter is NULL or references an invalid address.</td> 
   </tr>
   <tr> <td> @ref OCI_ERR_DRIVER_NO_RESPONSE </td>
        <td> The driver for a removable hardware received no response to a request. The hardware may be removed. 
             It is not known wether the request is lost and the command not exectuted or the response is lost and
             the command was executed. The controller enters the @a DISCONNECTED state. All subsequent calls will 
             return @ref OCI_ERR_DRIVER_DISCONNECTED. 
             Internal resources are locked until OCI_DestroyCANController() for the controller is called. </td> 
   </tr>
   <tr> <td> @ref OCI_ERR_DRIVER_DISCONNECTED </td>
        <td> The driver for a removable hardware detected a communication problem (in asynchromous communication, 
             by event signalling or during a previous call to the same hardware) and is not able to communicate to 
             the hardware any more. 
             The state of the controller is @a DISCONNECTED. The command is not executed. All subsequent calls to 
             the same Controller will return @ref OCI_ERR_DRIVER_DISCONNECTED. Internal resources are locked until 
             OCI_DestroyCANController() for the controller is called.  </td> 
   </tr>
   <tr> <td> @ref OCI_ERR_OUT_OF_MEMORY </td>
        <td> The driver run out of memory. This error should only occur on heavy loaded systems with multiple 
             clients and large send and receive queues. The error may indicate a resource leak in the driver 
             implementation. When multiple application access the same resource (interface hardware or physical
             controller) the termination of a different application may return the resources required for this 
             instance. </td> 
   </tr>

   </TABLE> 

   @sa @ref PAGE_OCI_RECEIVE_MESSAGES "Receive Messages", OCI_GetQueueStatus()

*/

#ifndef OCI_DYNAMICDLL
OCI_DECLSPEC OCI_ErrorCode
OCI_CALL OCI_ReadFlexRayData(
    OCI_QueueHandle queue, 
    OCI_Time timeout, 
    OCI_FlexRayMessage messages[], 
    uint32 size, 
    uint32 *count,
    uint32 *remaining);
#endif

typedef OCI_ErrorCode
(OCI_CALL *PF_OCI_ReadFlexRayData)(
    OCI_QueueHandle queue, 
    OCI_Time timeout,
    OCI_FlexRayMessage messages[], 
    uint32 size, 
    uint32 *count,
    uint32 *remaining);


/**
  Destroy a receive queue for a FlexRay controller instance.

  Any data in the queue, that is not read by OCI_ReadFlexRayData() will be discarded. The registered callback 
  routines for data recpetion and event reception (see @ref OCI_FlexRayRxQueueConfiguration) will not be called 
  after completion of this routine. All white filter added to this queue will be discarded.  
 
  The handle is invalid after this call and must not be used any longer. 

  @controllerState SUSPENDED, RUNNING. \n
                   When the controller is in the CREATED state no valid Queue can exist and the call will return
                   @ref OCI_ERR_INVALID_HANDLE.  \n
                   When the controller is in the DISCONNECTED state the function may be called to do local clean
                   up. Only a limitied plausibility check may be done in this case.  

  @todo This call needs a synchronization of the data transmission via callback mechanism 
  and the synchronous command processing through all transport layer!

  @param[in]  queue  Handle of the queue.

  @return     
  <TABLE border="0" rules="rows" cellspacing="3" cellpadding="3">  
   <tr> <td> @ref OCI_SUCCESS </td> 
        <td> The operation was successful. The @a queue is destroyed and the handle must not be used any longer. </td> 
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
*/
#ifndef OCI_DYNAMICDLL
OCI_DECLSPEC OCI_ErrorCode
OCI_CALL OCI_DestroyFlexRayRxQueue(
    OCI_QueueHandle queue);
#endif

typedef OCI_ErrorCode
(OCI_CALL *PF_OCI_DestroyFlexRayRxQueue)(
    OCI_QueueHandle queue);


/**
* @}
* @addtogroup GROUP_OCI_FLEXRAY_FILTERING Filter
* @{
*/


/** 
* Structure that defines a single FlexRay (read) frame filter. For a 
* given  controller instance, there may be a number of frame filters. 
* A frame will pass them and be added to the receive queue, if it matches 
* the condition of at least one of them. 
* A FlexRay frame filter describes the following condition:
* pass =     ((frameID      & frameIDMask)       == frameIDValue)
*         && ((ccIndicators & ccIndicatorsValue) != 0)
*         && ((cycle        % cyclePeriod)       == cycleOffset)
*/

typedef struct OCI_FlexRayRxFilter
{
    /** 
    * 11-bit value to be compared against the actual frame ID of the received 
    * frame. 
    */
    uint16 frameIDValue;
    
    /** 11-bit mask to be applied to the received frame ID. */
    uint16 frameIDMask;

    /**
    * Communication Controller indicators. 
    * @flags    The following flags are valid:
    * @n        @ref OCI_FLEXRAY_CHANNEL_A (bit 0) -  
    *           Frames received on channel A are accepted.
    * @n        @ref OCI_FLEXRAY_CHANNEL_B (bit 1) - 
    *           Frames received on channel B are accepted.
    * @remark   Both values can be bitwise OR-connected to receive on both channels.
    * 			Bits 2-15 are reserved and must be set to zero.
    * @sa       @ref ANCHOR_OCI_FLEXRAY_CC_INDICATORS
    *           "FlexRay communication controller indicator flags"
    */
    uint16 ccIndicators; 

    /** 
    * Defines in which cycle periods this filter is applied. Has to be a power 
    * of two. 
    * @values   The following values are valid:
    * @n        1
    * @n        2
    * @n        4
    * @n        8
    * @n        16
    * @n        32
    * @n        64
    */
    uint8 cyclePeriod;
    
    /**
    * Defines the cycle offset to the cycle period in which this filter is 
    * applied. Valid values are within the following range:
    * @n        0 <= @a cycleOffset <= @a cyclePeriod - 1.
    */
    uint8 cycleOffset;
    
    /**
    * Application defined filter tag. All received frames which match this 
    * filter are tagged with this value.
    */
    uint32 tag;

} OCI_FlexRayRxFilter;


/** Structure that defines a single FlexRay event filter. */
typedef struct OCI_FlexRayEventFilter
{
    /** Specifies the event kind. */
    OCI_FlexRayEvent eventCode;
    
    /**
    * Application defined filter tag. All events which match this filter are 
    * tagged with this value.
    */
    uint32 tag;
    
    /** 
    * Destination of the event. 
    * When @ref OCI_EVENT_DESTINATION_INBAND is set, the event will be delivered 
    * inband to the application. This means that the event will be queued or the 
    * callback function for the reception of a FlexRay frame will be executed, 
    * which depends on the configuration of the FlexRay receive queue 
    * (see @ref OCI_FlexRayRxQueueConfiguration).
    * @n 
    * When the  @ref OCI_EVENT_DESTINATION_CALLBACK is set, the event will 
    * trigger the event handler of the receive queue for that event
    * (see @ref OCI_FlexRayRxQueueConfiguration). Nothing happens, when the 
    * callback is not set.
    * @n
    * When both flags are combined, the event is duplicated and the callback is 
    * triggered and the event is stored inband in the queue. 
    *
    * @sa @ref PAGE_OCI_RECEIVE_MESSAGES "Receive Messages"
    */
    uint32 destination;

} OCI_FlexRayEventFilter;


/** 
  Add white filter condition(s) to enable the reception of FlexRay frames for a specific FlexRay receive queue.

  In the default configuration of a receive queue no data will be received because any data is blocked by
  some filter condition. The application must enable the recpetion of the data by setting white filter 
  (defining pass conditions). 

  This function allows the reception of specific FlexRay frames identified by (masked) Frame ID, channel and
  cycle. The frames may be queued and read by OCI_ReadFlexRayData() or a callback is triggered whenever a 
  frame passes the filter. The signalization path to the application is defined by the creation of the 
  receive queue. If OCI_CANRxQueueConfiguration::onFrame (see OCI_CreateFlexRayRxQueue) is not NULL the registered
  callback is executed. If If OCI_CANRxQueueConfiguration::onFrame is NULL the frame is queued and can be read
  by PCI_ReadFlexRayData().
  
  Filter conditions allow an effective reduction of the payload, that must be handled by the driver and
  application. The sequence of processing the filter array is undefined. When a frame matches to more than 
  one filter the frame will be passed to the application, but the used filter tag (identifying the matching
  filter) is implementation specific and may change in different versions and on different hardware. 

  @controllerState SUSPENDED, RUNNING. \n
                   When the controller is in the CREATED state no valid @a rxQueue exists and the call will return
                   @ref OCI_ERR_INVALID_HANDLE.

  @param[in]  rxQueue  Handle of the FlexRay receive queue that will receive the messages. 
  @param[in]  filter   A pointer to an array of @a count entries of type @ref OCI_FlexRayRxFilter. 
  @param[in]  count    Number of entries in the @a filter array. 

  @return     
  <TABLE border="0" rules="rows" cellspacing="3" cellpadding="3">  
   <tr> <td> @ref OCI_SUCCESS </td> 
        <td> The operation was successful. All @a filter are added. The FlexRay Frames will be received. </td> 
   </tr>
   <tr> <td> @ref OCI_ERR_INVALID_HANDLE </td>  
        <td> The @a rxQueue handle is not valid. </td> 
   </tr>
   <tr> <td> @ref OCI_ERR_INVALID_PARAMETER </td>
        <td> The @a count parameter is 0 or the @a filter paramter is NULL or references an invalid address.</td> 
   </tr>
   <tr> <td> @ref OCI_ERR_INCONSISTENT_PARAMETER_SET </td>
        <td> One or more @a filter objects are invalid or alredy exist.</td> 
   </tr>
   <tr> <td> @ref OCI_ERR_DRIVER_NO_RESPONSE </td>
        <td> The driver for a removable hardware received no response to a request. The hardware may be removed. 
             It is not known wether the request is lost and the command not exectuted or the response is lost and
             the command was executed. The controller enters the @a DISCONNECTED state. All subsequent calls will 
             return @ref OCI_ERR_DRIVER_DISCONNECTED. 
             Internal resources are locked until OCI_DestroyCANController() for the controller is called. </td> 
   </tr>
   <tr> <td> @ref OCI_ERR_DRIVER_DISCONNECTED </td>
        <td> The driver for a removable hardware detected a communication problem (in asynchromous communication, 
             by event signalling or during a previous call to the same hardware) and is not able to communicate to 
             the hardware any more. 
             The state of the controller is @a DISCONNECTED. The command is not executed. All subsequent calls to 
             the same Controller will return @ref OCI_ERR_DRIVER_DISCONNECTED. Internal resources are locked until 
             OCI_DestroyCANController() for the controller is called.  </td> 
   </tr>
   <tr> <td> @ref OCI_ERR_OUT_OF_MEMORY </td>
        <td> The driver run out of memory. This error should only occur on heavy loaded systems with multiple 
             clients and large send and receive queues. The error may indicate a resource leak in the driver 
             implementation. When multiple application access the same resource (interface hardware or physical
             controller) the termination of a different application may return the resources required for this 
             instance. </td> 
   </tr>
   <tr> <td> @ref OCI_ERR_UNEXPECTED_NULL </td>
        <td> An unexpected implementation error in the driver occured. Please contact the vendor of the driver. </td> 
   </tr>

  </TABLE> 

  @sa        @ref PAGE_OCI_RECEIVE_MESSAGES "Receive Messages"
*/

#ifndef OCI_DYNAMICDLL
OCI_DECLSPEC OCI_ErrorCode 
OCI_CALL OCI_AddFlexRayFrameFilter(
    OCI_QueueHandle rxQueue, 
    OCI_FlexRayRxFilter filter[], 
    uint32 count);
#endif

typedef OCI_ErrorCode
(OCI_CALL *PF_OCI_AddFlexRayFrameFilter)(
    OCI_QueueHandle queue, 
    OCI_FlexRayRxFilter filter[], 
    uint32 count);


/**
  Remove white filter condition(s) to disable reception of specific FlexRay frames for a specific
  FlexRay receive queue.

  In the default configuration of a receive queue no data and no event will be received because any data 
  is blocked by some filter condition. The application must enable the recpetion of the data or event by 
  setting white filter (defining pass conditions). 

  This function allows to withdraw one or more white filter conditions set by OCI_AddFlexRayFrameFilter(). Each
  @ref OCI_FlexRayRxFilter of the @a filter array must exact match to a previously added filter. It is
  not possible to restrict an existing white filter, without redrawing it and adding a new more restrictive
  one.
  
  @controllerState SUSPENDED, RUNNING. \n
                   When the controller is in the CREATED state no valid Queue can exist and the call will return
                   @ref OCI_ERR_INVALID_HANDLE.  \n
                   When the controller is in the DISCONNECTED state the function may be called to do local clean
                   up. Only a limitied plausibility check may be done in this case.  

  @param[in]  queue    Handle of the FlexRay receive queue to modify. 
  @param[in]  filter   A pointer to an array of @a count entries of type @ref OCI_FlexRayRxFilter. 
  @param[in]  count    specifies the number of entries in the @a filters array. 

  @return     
  <TABLE border="0" rules="rows" cellspacing="3" cellpadding="3">  
   <tr> <td> @ref OCI_SUCCESS </td> 
        <td> The operation was successful. All @a filter are removed.</td> 
   </tr>
   <tr> <td> @ref OCI_ERR_INVALID_HANDLE </td>  
        <td> The @a handle is not valid. </td> 
   </tr>
   <tr> <td> @ref OCI_ERR_INVALID_PARAMETER </td>
        <td> The @a filter paramter is NULL or references an invalid address.</td> 
   </tr>
   <tr> <td> @ref OCI_ERR_INCONSISTENT_PARAMETER_SET </td>
        <td> One or more filter objects are not set and can not be removed.  </td> 
   </tr>
   <tr> <td> @ref OCI_ERR_OUT_OF_MEMORY </td>
        <td> The driver run out of memory. This error should only occur on heavy loaded systems with multiple 
             clients and large send and receive queues. The error may indicate a resource leak in the driver 
             implementation. When multiple application access the same resource (interface hardware or physical
             controller) the termination of a different application may return the resources required for this 
             instance. </td> 
   </tr>
    <tr> <td> @ref OCI_ERR_UNEXPECTED_NULL </td>
        <td> An unexpected implementation error in the driver occured. Please contact the vendor of the driver. </td> 
   </tr>
  </TABLE> 

  @sa         PAGE_OCI_RECEIVE_MESSAGES 
  @todo       Define handling for overlapping filter.
*/

#ifndef OCI_DYNAMICDLL
OCI_DECLSPEC OCI_ErrorCode 
OCI_CALL OCI_RemoveFlexRayFrameFilter(
    OCI_QueueHandle queue, 
    OCI_FlexRayRxFilter filter[], 
    uint32 count);
#endif

typedef OCI_ErrorCode
(OCI_CALL *PF_OCI_RemoveFlexRayFrameFilter)(
    OCI_QueueHandle queue, 
    OCI_FlexRayRxFilter filter[], 
    uint32 count);


/** 
  Add white filter condition(s) to enable the reception of FlexRay events for a specific FlexRay receive queue.

  In the default configuration of a receive queue no data will be received because any data is blocked by
  some filter condition. The application must enable the recpetion of the data by setting white filter 
  (defining pass conditions). 

  This function allows the reception of specific events on the FlexRay bus. The events may be delivered in-order
  in the normal data stream or generate a callback. The signalization path to the application is defined by 
  the creation of the receive queue and by the definition of the filter condition in this call. 
  For any filter in the array it is possible to set OCI_FlexRayEventFilter::destination to a combination of
  @li  @ref OCI_EVENT_DESTINATION_CALLBACK 
       The event will generate the OCI_FlexRayRxQueueConfiguration::onEvent callback.
  @li  @ref OCI_EVENT_DESTINATION_INBAND 
       The event will use the same path as any received frame. Dependent on the configuration of the Queue the 
       event will generate the OCI_FlexRayRxQueueConfiguration::onFrame callback or will be queued. Queued events 
       and frames can be read by OCI_ReadFlexRayData().  

  When both Flags are set the event will be duplicated and use both signalization paths. 
  When no Flag is set @ref OCI_ERR_INVALID_FILTER is returned.

  @controllerState SUSPENDED, RUNNING. \n
                   When the controller is in the CREATED state no valid Queue can exist and the call will return
                   @ref OCI_ERR_INVALID_HANDLE.

  @param[in]  rxQueue    Handle of the CAN receive queue that will receive the event(s). 
  @param[in]  filter   A pointer to an array of @a count entries of type @ref OCI_FlexRayEventFilter. 
  @param[in]  count    Number of entries in the @a filter array. 

  @return     
  <TABLE border="0" rules="rows" cellspacing="3" cellpadding="3">  
   <tr> <td> @ref OCI_SUCCESS </td> 
        <td> The operation was successful. All @a filter are added. The events will be received. </td> 
   </tr>
   <tr> <td> @ref OCI_ERR_INVALID_HANDLE </td>  
        <td> The @a queue handle is not valid. </td> 
   </tr>
   <tr> <td> @ref OCI_ERR_INVALID_PARAMETER </td>
        <td> The @a filter paramter is NULL or references an invalid address.</td> 
   </tr>
   <tr> <td> @ref OCI_ERR_INCONSISTENT_PARAMETER_SET </td>
        <td> One or more filter objects are invalid or alredy exist.</td> 
   </tr>
   <tr> <td> @ref OCI_ERR_DRIVER_NO_RESPONSE </td>
        <td> The driver for a removable hardware received no response to a request. The hardware may be removed. 
             It is not known wether the request is lost and the command not exectuted or the response is lost and
             the command was executed. The controller enters the @a DISCONNECTED state. All subsequent calls will 
             return @ref OCI_ERR_DRIVER_DISCONNECTED. 
             Internal resources are locked until OCI_DestroyCANController() for the controller is called. </td> 
   </tr>
   <tr> <td> @ref OCI_ERR_DRIVER_DISCONNECTED </td>
        <td> The driver for a removable hardware detected a communication problem (in asynchromous communication, 
             by event signalling or during a previous call to the same hardware) and is not able to communicate to 
             the hardware any more. 
             The state of the controller is @a DISCONNECTED. The command is not executed. All subsequent calls to 
             the same Controller will return @ref OCI_ERR_DRIVER_DISCONNECTED. Internal resources are locked until 
             OCI_DestroyCANController() for the controller is called.  </td> 
   </tr>
   <tr> <td> @ref OCI_ERR_OUT_OF_MEMORY </td>
        <td> The driver run out of memory. This error should only occur on heavy loaded systems with multiple 
             clients and large send and receive queues. The error may indicate a resource leak in the driver 
             implementation. When multiple application access the same resource (interface hardware or physical
             controller) the termination of a different application may return the resources required for this 
             instance. </td> 
   </tr>
    <tr> <td> @ref OCI_ERR_UNEXPECTED_NULL </td>
        <td> An unexpected implementation error in the driver occured. Please contact the vendor of the driver. </td> 
   </tr>
  </TABLE> 

  @sa         GROUP_OCI_EVENT_HANDLING, @ref PAGE_OCI_RECEIVE_MESSAGES "Receive Messages"
*/

#ifndef OCI_DYNAMICDLL
OCI_DECLSPEC OCI_ErrorCode 
OCI_CALL OCI_AddFlexRayEventFilter(
    OCI_QueueHandle rxQueue, 
    OCI_FlexRayEventFilter filter[], 
    uint32 count);
#endif

typedef OCI_ErrorCode
(OCI_CALL *PF_OCI_AddFlexRayEventFilter)(
    OCI_QueueHandle rxQueue, 
    OCI_FlexRayEventFilter filter[], 
    uint32 count);


/**
  Remove white filter condition(s) to disable reception of FlexRay events for a specific FlexRay receive queue. 

  In the default configuration of a receive queue no data will be received because any data is blocked by
  some filter condition. The application must enable the recpetion of the data by setting white filter 
  (defining pass conditions). 

  This function allows to withdraw one or more white filter conditions set by OCI_AddFlexRayEventFilter(). Each
  @ref OCI_FlexRayEventFilter of the @a filter array must exact match to a previously added filter. 

  @controllerState SUSPENDED, RUNNING. \n
                   When the controller is in the CREATED state no valid Queue can exist and the call will return
                   @ref OCI_ERR_INVALID_HANDLE.  \n
                   When the controller is in the DISCONNECTED state the function may be called to do local clean
                   up. Only a limitied plausibility check may be done in this case.  

  @param[in]  queue    Handle of the FlexRay receive queue to modify. 
  @param[in]  filter   A pointer to an array of @a count entries of type @ref OCI_FlexRayEventFilter. 
  @param[in]  count    specifies the number of entries in the @a filter array. 

  @return     
  <TABLE border="0" rules="rows" cellspacing="3" cellpadding="3">  
   <tr> <td> @ref OCI_SUCCESS </td> 
        <td> The operation was successful. </td> 
   </tr>
   <tr> <td> @ref OCI_ERR_INVALID_HANDLE </td>  
        <td> The @a queue handle is not valid. </td> 
   </tr>
   <tr> <td> @ref OCI_ERR_INVALID_PARAMETER </td>
        <td> The @a filter paramter is NULL or references an invalid object.</td> 
   </tr>
   <tr> <td> @ref OCI_ERR_INCONSISTENT_PARAMETER_SET </td>
        <td> One or more filter objects are not set and can not be removed. The processing of the
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

  @sa         GROUP_OCI_EVENT_HANDLING, @ref PAGE_OCI_RECEIVE_MESSAGES "Receive Messages"

*/

#ifndef OCI_DYNAMICDLL
OCI_DECLSPEC OCI_ErrorCode 
OCI_CALL OCI_RemoveFlexRayEventFilter(
    OCI_QueueHandle queue, 
    OCI_FlexRayEventFilter filter[], 
    uint32 count);
#endif

typedef OCI_ErrorCode
(OCI_CALL *PF_OCI_RemoveFlexRayEventFilter)(
    OCI_QueueHandle queue, 
    OCI_FlexRayEventFilter filter[], 
    uint32 count);

/**
 V-Table for simplified dynamic binding of a driver instance.

 The V-Table allows an easy binding of the complete set of functions by exporting this structure.
*/
typedef struct OCI_FLEXRAYIO_VTable
{
    PF_OCI_GetFlexRayControllerStatus getFlexRayControllerStatus;
    PF_OCI_GetFlexRayControllerCapabilities getFlexRayControllerCapabilities;

    PF_OCI_CreateFlexRayTxQueue createFlexRayTxQueue;
    PF_OCI_WriteFlexRayData writeFlexRayData;
    PF_OCI_DestroyFlexRayTxQueue destroyFlexRayTxQueue;

    PF_OCI_CreateFlexRayRxQueue createFlexRayRxQueue;
    PF_OCI_ReadFlexRayData readFlexRayData;
    PF_OCI_DestroyFlexRayRxQueue destroyFlexRayRxQueue;

    PF_OCI_AddFlexRayFrameFilter addFlexRayFrameFilter;
    PF_OCI_RemoveFlexRayFrameFilter removeFlexRayFrameFilter;
    PF_OCI_AddFlexRayEventFilter addFlexRayEventFilter;
    PF_OCI_RemoveFlexRayEventFilter removeFlexRayEventFilter;
} OCI_FLEXRAYIO_VTable;

/** @} */

/** @} */

#ifdef __cplusplus
}
#endif

#endif
