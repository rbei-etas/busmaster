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
* @brief  CAN-specific global constants
* @remark The header structure of the OLI may change
*         in future releases. Don't include this
*         header directly. Use @ref OLI.h instead.
*/

#if !defined(__OLI_CANBASE_INCLUDED__)
#define __OLI_CANBASE_INCLUDED__

#include "..\..\OCI\ocicanio.h"

// include base definitions

#include "../Common/CommonBase.h"

// open ETAS::OLI namespace

#include "../Common/BeginNamespace.h"

#ifdef _DOXYGEN
namespace ETAS {namespace OLI {
#endif

/** 
* @ingroup GROUP_OLI_CAN_MESSAGES
* @brief  CAN type range constants.
*
* Despite the generic design of the interface classes within the OLI,
* applications need to identify the actual data. To that end, all
* instantiable interfaces derived from @IMessage feature a public 
* @c TYPE member.
*
* @since  BOA 1.3
* @see    CommonConstants, IMessage::GetType, IError::GetType
*/

enum CANConstants
{
    /** Base value for all CAN-specific @c TYPEs */
    CAN_TYPE_BASE = 0x20000,

    /** Base value for all CAN-specific event @c TYPEs */
    CAN_TYPE_EVENT_BASE = CAN_TYPE_BASE + COMMON_TYPE_EVENT,

    /** Base value for all CAN-specific frame @c TYPEs */
    CAN_TYPE_FRAME_BASE = CAN_TYPE_BASE + COMMON_TYPE_FRAME
};

/** 
* @ingroup GROUP_OLI_CAN_MESSAGES
* @brief  Flags that are signalled by the @ref ICANErrorFrame event. 
*
*         The application can enable these events by defining white filter 
*         conditions using @ref ICANErrorFrameFilter. 
*
* @remark For every @ref OCI_ANCHOR_CAN_ERROR_FRAME_TYPES "OCI CAN error frame type", there is a corresponding 
*         value in this enumeration. Usually, an OLI enumeration such as @ref CANErrorFrame exists in order to map an OCI
*         enumeration, which uses consecutive values, onto a set of values in which one bit is used for each value, thus
*         permitting the construction of bitwise filters. However, in this case the @ref OCI_ANCHOR_CAN_ERROR_FRAME_TYPES
*         "OCI CAN error frame types" already use one bit per value, so the values of @ref CANErrorFrame can be identical
*         to their OCI counterparts.
*
* @since  BOA 1.3
* @see    ICANErrorFrame, ICANErrorFrameFilter
*/

enum CANErrorFrame
{
   /** Bit stuff error */
   CAN_ERR_BITSTUFF = OCI_CAN_ERR_TYPE_BITSTUFF,

   /** Format error */
   CAN_ERR_FORMAT = OCI_CAN_ERR_TYPE_FORMAT,

   /** Acknowledge error */
   CAN_ERR_ACK = OCI_CAN_ERR_TYPE_ACK,

   /** Bit error */
   CAN_ERR_BIT = OCI_CAN_ERR_TYPE_BIT,

   /** CRC error */
   CAN_ERR_CRC = OCI_CAN_ERR_TYPE_CRC,

   /** 
     * Bit error desired recessive but monitored dominant  
     * Could happen if e.g. another node or the physical 
     * layer driver is defect and produces a permenant dominant
     * level.
     */
   CAN_ERR_BIT_RECSV_BUT_DOMINANT = OCI_CAN_ERR_TYPE_BIT_RECSV_BUT_DOMINANT,

   /**
     * Bit error desired dominant but monitored recessive
     * Could happen if e.g. the bus topilogy wasn't correctly
     * designed 
     */
   CAN_ERR_BIT_DOMINANT_BUT_RECSV = OCI_CAN_ERR_TYPE_BIT_DOMINANT_BUT_RECSV,

   /** Overload error */
   CAN_ERR_OVERLOAD = OCI_CAN_ERR_TYPE_OVERLOAD,

   /** Other unspecified error */
   CAN_ERR_UNKNOWN = OCI_CAN_ERR_TYPE_UNKNOWN
};

/** 
* @ingroup GROUP_OLI_CAN_MESSAGES
* @brief  Flags that are signalled by the @ref ICANEvent event. 
*
*         The application can enable these events by defining white filter 
*         conditions using @ref ICANEventFilter. 
*
* @remark For every @ref OCI_ANCHOR_CAN_BUS_EVENTS 
*         "OCI CAN Bus Event" code, there is a corresponding 
*         value in this enumeration, optimized for efficient filtering:
*         @code
*         OLI event code = 1 << OCI event code
*         @endcode
*
* @remark For every @ref OCI_ANCHOR_CAN_BUS_EVENTS "OCI CAN bus event code", there is a corresponding 
*         value in this enumeration. Usually, an OLI enumeration such as @ref CANBusState exists in order to map an OCI
*         enumeration, which uses consecutive values, onto a set of values in which one bit is used for each value, thus
*         permitting the construction of bitwise filters. However, in this case the @ref OCI_ANCHOR_CAN_BUS_EVENTS
*         "OCI CAN bus event codes" already use one bit per value, so the values of @ref CANBusState can be identical
*         to their OCI counterparts.
*
* @since  BOA 1.3
* @see    ICANEvent, ICANEventFilter
*/
enum CANBusState
{
    /** The CAN controller has switched into the active state. */
    CAN_BUS_ACTIVE   = OCI_CAN_BUS_EVENT_STATE_ACTIVE,

    /** The CAN controller has switched into the passive state. */ 
    CAN_BUS_PASSIVE  = OCI_CAN_BUS_EVENT_STATE_PASSIVE,

    /** The warning limit of the error counter has exceeded. */
    CAN_BUS_ERRLIMIT = OCI_CAN_BUS_EVENT_STATE_ERRLIMIT,

    /** The controller has switched into the bus off state. */
    CAN_BUS_BUSOFF   = OCI_CAN_BUS_EVENT_STATE_BUSOFF,

    /** The controller has switched to single wire mode. 
        (fault tolerant physical medium only) 
     */
    CAN_BUS_FAULT_TOLERANT_SINGLE_WIRE = OCI_CAN_BUS_EVENT_FAULT_TOLERANT_SINGLE_WIRE
};

// close ETAS::OLI namespace

#ifdef _DOXYGEN
}}
#endif

#include "../Common/EndNamespace.h"

#endif // !defined(__OLI_CANBASE_INCLUDED__)
