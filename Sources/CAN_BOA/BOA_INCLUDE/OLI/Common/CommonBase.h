///////////////////////////////////////////////////////////
//  CommonBase.h
//  Constants for the bus-independent objects
///////////////////////////////////////////////////////////

/// @todo update doxygen comments

#if !defined(__OLI_COMMONBASE_INCLUDED__)
#define __OLI_COMMONBASE_INCLUDED__

#include "BeginNamespace.h"

/// \addtogroup GROUP_OLI_COMMON_MESSAGES
/// @{

/// Common constants

enum CommonConstants
{
    COMMON_BASE_MASK = 0xff0000,
    COMMON_TYPE_BASE = 0x10000,

    COMMON_TYPE_MASK = 0xff00,
    COMMON_TYPE_EVENT = 0x100,
    COMMON_TYPE_FRAME = 0x200,
    COMMON_TYPE_ERROR = 0x300,

    COMMON_TYPE_EVENT_BASE = COMMON_TYPE_BASE + COMMON_TYPE_EVENT,
    COMMON_TYPE_FRAME_BASE = COMMON_TYPE_BASE + COMMON_TYPE_FRAME,
    COMMON_TYPE_ERROR_BASE = COMMON_TYPE_BASE + COMMON_TYPE_ERROR
};

enum InternalErrorEventCode
{
   INTERNAL_GENERAL_ERROR = 1
};

enum TimerEventCode
{
   /**
   * Timer event - the timer has locked to an external reference. There
   * may a non monotony in the timestamps.
   */
   TIMER_EVENT_SYNC_LOCK = 1,

   /**
   * Timer event - the timer has lost the contact to an external reference.
   */
   TIMER_EVENT_SYNC_LOSS = 2,

   /**
   * Timer event - A leap second occurs. The offset between TAI and UTC
   * has changed by one second. The change may be positive or negative. 
   */
   TIMER_EVENT_LEAP_SECOND = 4
};

enum QueueEventCode
{
   /**
   * Queue event - the receive queue has overflown and at least one 
   * message has been lost.
   */
   QUEUE_OVERFLOW = 1 << 6
};

/// @}

#include "EndNamespace.h"

#endif // !defined(__OLI_COMMONBASE_INCLUDED__)
