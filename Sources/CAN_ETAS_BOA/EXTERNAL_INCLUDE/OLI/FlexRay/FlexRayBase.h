///////////////////////////////////////////////////////////
//  FlexRayBase.h
//  Constants for the FlexRay bus specific objects
///////////////////////////////////////////////////////////

/// @todo update doxygen comments

#if !defined(__OLI_FLEXRAYBASE_INCLUDED__)
#define __OLI_FLEXRAYBASE_INCLUDED__

#include "../Common/CommonBase.h"

#include "../Common/BeginNamespace.h"

/// \ingroup GROUP_OLI_FLEXRAY_MESSAGES

/// FlexRay constants

enum FlexRayConstants
{
    FLEXRAY_TYPE_BASE = 0x30000,
    FLEXRAY_TYPE_EVENT_BASE = FLEXRAY_TYPE_BASE + COMMON_TYPE_EVENT,
    FLEXRAY_TYPE_FRAME_BASE = FLEXRAY_TYPE_BASE + COMMON_TYPE_FRAME,

    FLEXRAY_TYPE_FRAME                   = FLEXRAY_TYPE_FRAME_BASE + 1,
    FLEXRAY_TYPE_SWITCH_MESSAGE          = FLEXRAY_TYPE_FRAME_BASE + 2,
    FLEXRAY_TYPE_RECONFIGURATION_MESSAGE = FLEXRAY_TYPE_FRAME_BASE + 3,
};

/// \ingroup GROUP_OLI_FLEXRAY_MESSAGES

/// FlexRay channel indicator type

enum FlexRayChannel
{
    FLEXRAY_CHANNEL_A = 1,
    FLEXRAY_CHANNEL_B = 2,
    FLEXRAY_CHANNEL_BOTH = FLEXRAY_CHANNEL_A | FLEXRAY_CHANNEL_B
};

/// \ingroup GROUP_OLI_FLEXRAY_MESSAGES

/// FlexRay frame indicator type. See OCI for details.

enum FlexRayFrameType
{
    FLEXRAY_PAYLOAD_PREAMBLE = 2,
    FLEXRAY_NO_NULL_FRAME    = 4,
    FLEXRAY_SYNC_FRAME       = 8,
    FLEXRAY_STARTUP_FRAME    = 16
};

/**
    Flags that are signalled by the @ref IFlexRayEvent in case of a FlexRayEventCode::FLEXRAY_EVENT_POCS_CHANGE. 
    The application can enable the events by defining white filter conditions using @ref IFlexRayEventFilter.
*/
enum FlexRayControllerProtocolOperationControlStatus
{
   FLEXRAY_POCS_ALL_PASS                       = 0,      ///< Filtercondition: (x & POCS_ALL_PASS) == POCS_ALL_PASS is always true
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
   FLEXRAY_POCS_INVALID_STATE                  = 1<<31   ///< invalid state information was received from the OCD
};

/**
    Flags that are signalled by the @ref IFlexRayEvent. The application can
    enable these events by defining white filter conditions using @ref IFlexRayEventFilter.
*/
enum FlexRayEventCode
{
    /** The static slot portion of a new cycle has started. */
    FLEXRAY_START_OF_STATIC_SEGMENT = 1<<1,
    
    /** The dynamic slot portion of a new cycle has started */
    FLEXRAY_START_OF_DYNAMIC_SEGMENT = 1<<2,
    
    /** The status of the communication controller has changed */
    FLEXRAY_EVENT_POCS_CHANGE = 1<<3,
    
    /** One or more frames got lost */
    FLEXRAY_DATA_LOST = 1<<4
    
};

#include "../Common/EndNamespace.h"

#endif // !defined(__OLI_FLEXRAYBASE_INCLUDED__)
