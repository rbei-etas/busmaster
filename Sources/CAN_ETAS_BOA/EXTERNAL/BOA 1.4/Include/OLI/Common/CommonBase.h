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
* @brief  Non-bus specific global constants
* @remark The header structure of the OLI may change
*         in future releases. Don't include this
*         header directly. Use @ref OLI.h instead.
*/

#if !defined(__OLI_COMMONBASE_INCLUDED__)
#define __OLI_COMMONBASE_INCLUDED__

#include "..\..\OCI\ocierror.h"
#include "..\..\OCI\ocitime.h"
#include "..\..\OCI\ociqueue.h"

// open ETAS::OLI namespace

#include "BeginNamespace.h"

#ifdef _DOXYGEN
namespace ETAS {namespace OLI {
#endif

/** 
* @ingroup GROUP_OLI_COMMON_BASE
* @brief  Common type range constants and masks.
*
* Despite the generic design of the interface classes within the OLI,
* applications need to identify the actual data. To that end, all
* typed derived from @ref IError and all instantiable interfaces derived
* from @IMessage feature a public @c TYPE member.
*
* All @c TYPE values are distinct and follow a common scheme:
*
* TYPE = 0x00PPCCTT
* @li 00 - reserved. Must be 0.
* @li PP - part of the API that the interface belongs to. It can be 
*          the "common" or one of the vehicle-bus-specific parts.
* @li CC - classification. Can be "event", "frame" etc.      
* @li TT - specific type
*
* @since  BOA 1.3
* @see    IMessage::GetType, IError::GetType
*/

enum CommonConstants
{
    /** Mask the first-level @c TYPE classification; 
        the masked value can be @ref COMMON_TYPE_BASE, @ref CAN_TYPE_BASE, 
        @ref LIN_TYPE_BASE or @ref FLEXRAY_TYPE_BASE */
    COMMON_BASE_MASK = 0xff0000,

    /** Base value for all non-bus-specific @c TYPEs */
    COMMON_TYPE_BASE = 0x010000,


    /** Mask for the second level @c TYPE classification */
    COMMON_TYPE_MASK  = 0xff00,

    /** Offset of event @c TYPEs relative to the respective
        @ref COMMON_BASE_MASK "BASE" value */
    COMMON_TYPE_EVENT = 0x0100,

    /** Offset of frame @c TYPEs relative to the respective
        @ref COMMON_BASE_MASK "BASE" value */
    COMMON_TYPE_FRAME = 0x0200,

    /** Offset of error @c TYPEs relative to the respective
        @ref COMMON_BASE_MASK "BASE" value */
    COMMON_TYPE_ERROR = 0x0300,


    /** Base value for all non-bus-specific event @c TYPEs */
    COMMON_TYPE_EVENT_BASE = COMMON_TYPE_BASE + COMMON_TYPE_EVENT,

    /** Base value for all non-bus-specific frame @c TYPEs; 
        currently not in use. */
    COMMON_TYPE_FRAME_BASE = COMMON_TYPE_BASE + COMMON_TYPE_FRAME,

    /** Base value for all @ref IError-derived @c TYPEs */
    COMMON_TYPE_ERROR_BASE = COMMON_TYPE_BASE + COMMON_TYPE_ERROR
};

/** 
* @ingroup GROUP_OLI_COMMON_BASE
* @brief  Event codes for internal errors.
*
* @remark For every event code in OCI_InternalErrorEvent, there
*         is a corresponding value in this enumeration, optimized
*         for efficient filtering:
*         @code
*         OLI event code = 1 << OCI event code
*         @endcode
*
* @since  BOA 1.3
* @see    IInternalErrorEvent, IInternalErrorEventFilter
*/

enum InternalErrorEventCode
{
    /** General internal error. */
    INTERNAL_GENERAL_ERROR = (1 << OCI_INTERNAL_GENERAL_ERROR)
};

/** 
* @ingroup GROUP_OLI_COMMON_BASE
* @brief  Event codes for timer events.
*
* @remark For every event code in OCI_TimerEvent, there
*         is a corresponding value in this enumeration, optimized
*         for efficient filtering:
*         @code
*         OLI event code = 1 << OCI event code
*         @endcode
*
* @since  BOA 1.3
* @see    ITimerEvent, ITimerEventFilter
*/

enum TimerEventCode
{
    /**
    * Timer event - the timer has locked to an external reference. There
    * may a non monotony in the timestamps.
    */
    TIMER_EVENT_SYNC_LOCK = (1 << OCI_TIMER_EVENT_SYNC_LOCK),

    /**
    * Timer event - the timer has lost the contact to an external reference.
    */
    TIMER_EVENT_SYNC_LOSS = (1 << OCI_TIMER_EVENT_SYNC_LOSS),

    /**
    * Timer event - A leap second occurs. The offset between TAI and UTC
    * has changed by one second. The change may be positive or negative. 
    */
    TIMER_EVENT_LEAP_SECOND = (1 << OCI_TIMER_EVENT_LEAP_SECOND)
};

/** 
* @ingroup GROUP_OLI_COMMON_BASE
* @brief  Event codes for queue events.
*
* @remark Only the overflow event in OCI_QueueEvent has a
*         corresponding value in this enumeration, optimized
*         for efficient filtering:
*         @code
*         OLI event code = 1 << OCI event code
*         @endcode
*
* @since  BOA 1.3
* @see    IQueueEvent, IQueueEventFilter
*/

enum QueueEventCode
{
    /**
    * Queue event - the receive queue has overflown and at least one 
    * message has been lost.
    */
    QUEUE_OVERFLOW = (1 << OCI_QUEUE_OVERFLOW)
};

// close ETAS::OLI namespace

#ifdef _DOXYGEN
}}
#endif

#include "EndNamespace.h"

#endif // !defined(__OLI_COMMONBASE_INCLUDED__)
