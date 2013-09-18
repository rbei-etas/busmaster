/*
* Basic Open API - Open Link Interface
* Version 1.3
*
* Copyright (c) ETAS GmbH. All rights reserved.
*
* $Revision: 4806 $
*/

/** 
* @file
* @brief  FlexRay-specific global constants
* @remark The header structure of the OLI may change
*         in future releases. Don't include this
*         header directly. Use @ref OLI.h instead.
*/

#if !defined(__OLI_FLEXRAYBASE_INCLUDED__)
#define __OLI_FLEXRAYBASE_INCLUDED__

#include "..\..\OCI\ociflxio.h"

// include base definitions

#include "../Common/CommonBase.h"

// open ETAS::OLI namespace

#include "../Common/BeginNamespace.h"

#ifdef _DOXYGEN
namespace ETAS {namespace OLI {
#endif

/** 
* @ingroup GROUP_OLI_FLEXRAY_MESSAGES
* @brief  FlexRay type range constants.
*
* Despite the generic design of the interface classes within the OLI,
* applications need to identify the actual data. To that end, all
* instantiable interfaces derived from @IMessage feature a public 
* @c TYPE member.
*
* @since  BOA 1.3
* @see    CommonConstants, IMessage::GetType, IError::GetType
*/

enum FlexRayConstants
{
    /** Base value for all FlexRay-specific @c TYPEs */
    FLEXRAY_TYPE_BASE = 0x30000,

    /** Base value for all FlexRay-specific event @c TYPEs */
    FLEXRAY_TYPE_EVENT_BASE = FLEXRAY_TYPE_BASE + COMMON_TYPE_EVENT,

    /** Base value for all FlexRay-specific frame @c TYPEs */
    FLEXRAY_TYPE_FRAME_BASE = FLEXRAY_TYPE_BASE + COMMON_TYPE_FRAME
};

/** 
* @ingroup GROUP_OLI_CAN_MESSAGES
* @brief  FlexRay channel indicator type. 
*
* @remark These definitions are identical to those use in the OCI.
*
* @since  BOA 1.3
* @see    ICANErrorFrame, ICANErrorFrameFilter
*/

enum FlexRayChannel
{
    /** Applies to FlexRay channel A (only) */
    FLEXRAY_CHANNEL_A = 1,

    /** Applies to FlexRay channel B (only) */
    FLEXRAY_CHANNEL_B = 2,

    /** Applies to both FlexRay channels */
    FLEXRAY_CHANNEL_BOTH = FLEXRAY_CHANNEL_A | FLEXRAY_CHANNEL_B
};

/** 
* @ingroup GROUP_OLI_FLEXRAY_MESSAGES
* @brief  FlexRay frame indicator types. 
*
*         These flags specify the location of the respective frame indicators.
*
* @remark These definitions are identical to thosed used in the OCI.
*
* @since  BOA 1.3
* @see    ICANErrorFrame, ICANErrorFrameFilter
*/

enum FlexRayFrameType
{
    /** Startup frame indicator. If the startup frame indicator is set 
        to zero then the frame is not a startup frame. If the startup 
        frame indicator is set to one then the frame is a startup frame.
    */
    FLEXRAY_STARTUP_FRAME_INDICATOR = 1,

    /** Sync frame indicator. If the sync frame indicator is set to zero 
        then no receiving node considers the frame for synchronization. 
        If the sync frame indicator is set to one then all receiving nodes 
        utilize the frame for synchronization.
     */
    FLEXRAY_PAYLOAD_PREAMBLE        = 2,

    /** Null frame indicator. If the null frame indicator is set to zero 
        then the payload segment contains no valid data. All bytes in the 
        payload section are set to zero. If the null frame indicator is set 
        to one then the payload segment contains data. 
     */
    FLEXRAY_NO_NULL_FRAME           = 4,

    /** Payload preamble indicator. If the payload preamble indicator is 
        set to one then the payload segment of the frame contains a network 
        management vector if it is transmitted in the static segment or a 
        message ID if it is transmitted in the dynamic segment.
     */
    FLEXRAY_SYNC_FRAME              = 8,

    /** Reserved indicator. Currently this is a reserved feature: 
        A transmitting node shall set the reserved bit to logical '0'. 
        A receiving node shall ignore the reserved bit. 
     */
    FLEXRAY_RESERVED_INDICATOR      = 16
};

/** 
* @ingroup GROUP_OLI_CAN_MESSAGES
* @brief  Flags that are signalled by @ref IFlexRayEvent in case of a 
*         @ref FLEXRAY_EVENT_POCS_CHANGE event.
*
*         The application can enable these events by defining white filter 
*         conditions using @ref IFlexRayEventFilter. 
*
* @remark For every @a OCI_FlexRayControllerProtocolOperationControlStatus
*         event code, there is a corresponding value in this enumeration, 
*         optimized for efficient filtering:
*         @code
*         OLI event code = 1 << OCI event code
*         @endcode
*
* @since  BOA 1.3
* @see    FlexRayEventCode, IFlexRayEvent, IFlexRayEventFilter
*
* @todo   Comment control status values
*/

enum FlexRayControllerProtocolOperationControlStatus
{
    ///< Filtercondition: (x & POCS_ALL_PASS) == POCS_ALL_PASS is always true
   FLEXRAY_POCS_ALL_PASS                       = 0,      
   FLEXRAY_POCS_DEFAULT_CONFIG                 = 1<< 0,
   FLEXRAY_POCS_READY                          = 1<< 1,
   FLEXRAY_POCS_NORMAL_ACTIVE                  = 1<< 2,
   FLEXRAY_POCS_NORMAL_PASSIVE                 = 1<< 3,
   FLEXRAY_POCS_HALT                           = 1<< 4,
   FLEXRAY_POCS_MONITOR_MODE                   = 1<< 5,
   FLEXRAY_POCS_CONFIG                         = 1<< 6,

   FLEXRAY_POCS_WAKEUP_STANDBY                 = 1<< 7,
   FLEXRAY_POCS_WAKEUP_LISTEN                  = 1<< 8,
   FLEXRAY_POCS_WAKEUP_SEND                    = 1<< 9,
   FLEXRAY_POCS_WAKEUP_DETECT                  = 1<<10,

   FLEXRAY_POCS_STARTUP_PREPARE                = 1<<11,

   FLEXRAY_POCS_COLDSTART_LISTEN               = 1<<12,
   FLEXRAY_POCS_COLDSTART_COLLISION_RESOLUTION = 1<<13,
   FLEXRAY_POCS_COLDSTART_CONSISTENCY_CHECK    = 1<<14,
   FLEXRAY_POCS_COLDSTART_GAP                  = 1<<15,
   FLEXRAY_POCS_COLDSTART_JOIN                 = 1<<16,

   FLEXRAY_POCS_INTEGRATION_COLDSTART_CHECK    = 1<<17,
   FLEXRAY_POCS_INTEGRATION_LISTEN             = 1<<18,
   FLEXRAY_POCS_INTEGRATION_CONSISTENCY_CHECK  = 1<<19,

   FLEXRAY_POCS_INITIALIZE_SCHEDULE            = 1<<20,

   FLEXRAY_POCS_ABORT_STARTUP                  = 1<<21,
   ///< invalid state information was received from the OCD
   FLEXRAY_POCS_INVALID_STATE                  = 1<<31   
};

/** 
* @ingroup GROUP_OLI_CAN_MESSAGES
* @brief  Flags that are signalled by @ref IFlexRayEvent.
*
*         The application can enable these events by defining white filter 
*         conditions using @ref IFlexRayEventFilter. 
*
* @remark For every @a OCI_FlexRayEvent event code, there is a corresponding 
*         value in this enumeration, optimized for efficient filtering:
*         @code
*         OLI event code = 1 << OCI event code
*         @endcode
*
* @since  BOA 1.3
* @see    IFlexRayEvent, IFlexRayEventFilter
*/

enum FlexRayEventCode
{
    /** The static slot portion of a new cycle has started. */
    FLEXRAY_START_OF_STATIC_SEGMENT = (1 << OCI_FLEXRAY_EVENT_START_OF_STATIC_SEGMENT),
    
    /** The dynamic slot portion of a new cycle has started */
    FLEXRAY_START_OF_DYNAMIC_SEGMENT = (1 << OCI_FLEXRAY_EVENT_START_OF_DYNAMIC_SEGMENT),
    
    /** The status of the communication controller has changed */
    FLEXRAY_EVENT_POCS_CHANGE = (1 << OCI_FLEXRAY_EVENT_POCS_CHANGE),
    
    /** One or more frames got lost */
    FLEXRAY_DATA_LOST = (1 << OCI_FLEXRAY_EVENT_DATA_LOST)
    
};

// close ETAS::OLI namespace

#ifdef _DOXYGEN
}}
#endif

#include "../Common/EndNamespace.h"

#endif // !defined(__OLI_FLEXRAYBASE_INCLUDED__)
