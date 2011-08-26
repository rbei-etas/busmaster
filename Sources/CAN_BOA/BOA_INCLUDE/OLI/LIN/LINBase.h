///////////////////////////////////////////////////////////
//  LINBase.h
//  Constants for the LIN bus specific objects
///////////////////////////////////////////////////////////

/// @todo update doxygen comments

#if !defined(__OLI_LINBASE_INCLUDED__)
#define __OLI_LINBASE_INCLUDED__

#include "../Common/CommonBase.h"

#include "../Common/BeginNamespace.h"

/// \addtogroup GROUP_OLI_LIN_MESSAGES

/// @{

/// LIN constants

enum LINConstants
{
    LIN_TYPE_BASE = 0x40000,
    LIN_TYPE_EVENT_BASE = LIN_TYPE_BASE + COMMON_TYPE_EVENT,
    LIN_TYPE_FRAME_BASE = LIN_TYPE_BASE + COMMON_TYPE_FRAME
};

/**
* Definition for used checksum calculation.
*/
enum LINCRCType
{
    /// Classic checksum calculation (use only the data bytes).
    LIN_CHECKSUM_CLASSIC    = 1,
    
    /// Enhanced checksum calculation (use the data bytes including the id field).
    LIN_CHECKSUM_ENHANCED   = 2,
    
    /// Undefined checksum calculation.
    LIN_CHECKSUM_UNDEFINED  = 128
};

/**
    Flags that indicate the direction of a LIN Frame.
*/
enum LINFrameDirection
{
    /// Frame is received from LIN bus.
    LIN_FRAME_RX    = 1,

    /// Frame is sent to LIN bus.
    LIN_FRAME_TX    = 2,

    /// Frame is sent and received to/from LIN bus, e.g.\ self-reception.
    LIN_FRAME_TXRX  = LIN_FRAME_TX|LIN_FRAME_RX,

    /// Frame is not used.
    LIN_FRAME_NOP   = 4

};

 /**
    Flags that are signalled by the @ref ILINSleepEvent. The application can
    enable these events by defining white filter conditions using @ref ILINSleepEventFilter.
*/
enum LINSleepMode
 {
     /// The hardware is being kept alive. Hardware stays in wake state.
     LIN_SLEEPMODE_STAYALIVE  = 1,

     /// The hardware is set into sleep state. 
     LIN_SLEEPMODE_GOTOSLEEP  = 2,

     /// The hardware wakes up. Hardware enters wake state
     LIN_SLEEPMODE_WAKEUP     = 4
 };

/**
    Flags that are signalled by the @ref ILINTransmissionStatusEvent. The application can
    enable these events by defining white filter conditions using @ref ILINTransmissionStatusEventFilter.
*/
 enum LINTransmissionState
 {
     /// No LIN transmission has taken place.
     LIN_TRANS_NONE          = 1 << 0,

     /// LIN frame transmission successful.
     LIN_TRANS_OK            = 1 << 1,

     /// While LIN transmission bus level does not correspond to sent bit.
     LIN_TRANS_ERR_BIT       = 1 << 2,
     
     /// Checksum error in LIN frame.
     LIN_TRANS_ERR_CRC       = 1 << 3,
    
     /// Error in LIN frame header (LIN slave only).
     LIN_TRANS_ERR_HEADER    = 1 << 4,
     
     /// Frame transfer on LIN has not completed in time.
     /// Typical reason for timeout is mismatch of actual and configured 
     /// frame length or no response at all in case of unconditional frames.
     LIN_TRANS_ERR_TOUT      = 1 << 5,
     
     /// Collision while transmission of an event triggered frame.
     LIN_TRANS_COLLISION     = 1 << 6,
     
     /// No response to LIN frame header (event triggered frame only).
     LIN_TRANS_NO_RESPONSE   = 1 << 7,

     /// LIN master scheduling does not pick up after wake up request (LIN slave only).
     LIN_TRANS_NO_MSTR_SCHED = 1 << 8,
     
     /// No data available to be sent in LIN frame.
     LIN_TRANS_NO_TX_DATA    = 1 << 9,

     /// Nondescript error occurred while LIN frame transmission.
     LIN_TRANS_ERR_UNKNOWN   = 1 << 10
 };

/// @}

#include "../Common/EndNamespace.h"

#endif // !defined(__OLI_LINBASE_INCLUDED__)
