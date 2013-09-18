/*
* Basic Open API - Open Link Interface
* Version 1.3
*
* Copyright (c) ETAS GmbH. All rights reserved.
*
* $Revision: 4509 $
*/

/** 
* @file
* @brief  LIN-specific global constants
* @remark The header structure of the OLI may change
*         in future releases. Don't include this
*         header directly. Use @ref OLI.h instead.
*/

#if !defined(__OLI_LINBASE_INCLUDED__)
#define __OLI_LINBASE_INCLUDED__

// include base definitions

#include "../Common/CommonBase.h"

// open ETAS::OLI namespace

#include "../Common/BeginNamespace.h"

#ifdef _DOXYGEN
namespace ETAS {namespace OLI {
#endif

/** 
* @ingroup GROUP_OLI_LIN_MESSAGES
* @brief  LIN type range constants.
*
* Despite the generic design of the interface classes within the OLI,
* applications need to identify the actual data. To that end, all
* instantiable interfaces derived from @IMessage feature a public 
* @c TYPE member.
*
* @since  BOA 1.3
* @see    CommonConstants, IMessage::GetType, IError::GetType
*/

enum LINConstants
{
    /** Base value for all LIN-specific @c TYPEs */
    LIN_TYPE_BASE = 0x40000,

    /** Base value for all LIN-specific event @c TYPEs */
    LIN_TYPE_EVENT_BASE = LIN_TYPE_BASE + COMMON_TYPE_EVENT,

    /** Base value for all LIN-specific frame @c TYPEs */
    LIN_TYPE_FRAME_BASE = LIN_TYPE_BASE + COMMON_TYPE_FRAME
};

/** 
* @ingroup GROUP_OLI_LIN_MESSAGES
* @brief  Checksum caclculation methods. 
*
* @remark These definitions are identical to those used in the OCI.
*
* @since  BOA 1.3
*/

enum LINCRCType
{
    /** Classic checksum calculation using only the data bytes. */
    LIN_CHECKSUM_CLASSIC    = 1,
    
    /** Enhanced checksum calculation using the data bytes 
        plus the id field.
     */
    LIN_CHECKSUM_ENHANCED   = 2,
    
    /** Undefined checksum calculation. */
    LIN_CHECKSUM_UNDEFINED  = 128
};

/** 
* @ingroup GROUP_OLI_LIN_MESSAGES
* @brief  Frame direction indicators. 
*
* @remark These definitions are identical to those used in the OCI.
*
* @since  BOA 1.3
*/

enum LINFrameDirection
{
    /** Frame is received from LIN bus. */
    LIN_FRAME_RX    = 1,

    /** Frame is sent to LIN bus. */
    LIN_FRAME_TX    = 2,

    /** Frame is sent and received to/from LIN bus, e.g. self-reception. */
    LIN_FRAME_TXRX  = LIN_FRAME_TX|LIN_FRAME_RX,

    /** Frame is not used. */
    LIN_FRAME_NOP   = 4
};

/** 
* @ingroup GROUP_OLI_LIN_MESSAGES
* @brief  Flags that are signalled by the @ref ILINSleepEvent event. 
*
*         The application can enable these events by defining white filter 
*         conditions using @ref ILINSleepEventFilter. 
*
* @remark For every @ref ANCHOR_OCI_LIN_SLEEPMODE_FLAGS 
*         "OCI LIN Sleep Mode Flag", there is a corresponding 
*         value in this enumeration, optimized for efficient filtering:
*         @code
*         OLI flag value = 1 << OCI flag value
*         @endcode
*
* @since  BOA 1.3
* @see    ILINSleepEvent, ILINSleepEventFilter
*/

enum LINSleepMode
 {
     /** The hardware is being kept alive. Hardware stays in wake state. */
     LIN_SLEEPMODE_STAYALIVE  = 1,

     /** The hardware is set into sleep state. */
     LIN_SLEEPMODE_GOTOSLEEP  = 2,

     /** The hardware wakes up. Hardware enters wake state. */
     LIN_SLEEPMODE_WAKEUP     = 4
 };

/** 
* @ingroup GROUP_OLI_LIN_MESSAGES
* @brief  Flags that are signalled by the @ref ILINTransmissionStatusEvent 
*         event. 
*
*         The application can enable these events by defining white filter 
*         conditions using @ref ILINTransmissionStatusEventFilter. 
*
* @remark For every @ref OCI_LIN_TRANSMISSION_STATE, there is a corresponding 
*         value in this enumeration, optimized for efficient filtering:
*         @code
*         OLI flag value = 1 << OCI flag value
*         @endcode
*
* @since  BOA 1.3
* @see    ILINTransmissionStatusEvent, ILINTransmissionStatusEventFilter
*/

 enum LINTransmissionState
 {
     /** No LIN transmission has taken place. */
     LIN_TRANS_NONE          = 1 << 0,

     /** LIN frame transmission successful. */
     LIN_TRANS_OK            = 1 << 1,

     /** While LIN transmission bus level does not correspond to sent bit. */
     LIN_TRANS_ERR_BIT       = 1 << 2,
     
     /** Checksum error in LIN frame. */
     LIN_TRANS_ERR_CRC       = 1 << 3,
    
     /** Error in LIN frame header (LIN slave only). */
     LIN_TRANS_ERR_HEADER    = 1 << 4,
     
     /** Frame transfer on LIN has not completed in time.
         Typical reason for timeout is mismatch of actual and configured 
         frame length or no response at all in case of unconditional frames.
      */
     LIN_TRANS_ERR_TOUT      = 1 << 5,
     
     /** Collision while transmission of an event triggered frame. */
     LIN_TRANS_COLLISION     = 1 << 6,
     
     /** No response to LIN frame header (event triggered frame only). */
     LIN_TRANS_NO_RESPONSE   = 1 << 7,

     /** LIN master scheduling does not pick up after wake up request 
         (LIN slave only). 
      */
     LIN_TRANS_NO_MSTR_SCHED = 1 << 8,
     
     /** No data available to be sent in LIN frame. */
     LIN_TRANS_NO_TX_DATA    = 1 << 9,

     /** Nondescript error occurred while LIN frame transmission. */
     LIN_TRANS_ERR_UNKNOWN   = 1 << 10
 };

// close ETAS::OLI namespace

#ifdef _DOXYGEN
}}
#endif

#include "../Common/EndNamespace.h"

#endif // !defined(__OLI_LINBASE_INCLUDED__)
