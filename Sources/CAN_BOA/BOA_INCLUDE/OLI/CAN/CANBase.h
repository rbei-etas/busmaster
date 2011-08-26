///////////////////////////////////////////////////////////
//  CANBase.h
//  Constants for the CAN bus specific objects
///////////////////////////////////////////////////////////

/// @todo update doxygen comments

#if !defined(__OLI_CANBASE_INCLUDED__)
#define __OLI_CANBASE_INCLUDED__

#include "../Common/CommonBase.h"

#include "../Common/BeginNamespace.h"

/// \addtogroup GROUP_OLI_CAN_MESSAGES
/// @{

/// CAN constants

enum CANConstants
{
    CAN_TYPE_BASE = 0x20000,
    CAN_TYPE_EVENT_BASE = CAN_TYPE_BASE + COMMON_TYPE_EVENT,
    CAN_TYPE_FRAME_BASE = CAN_TYPE_BASE + COMMON_TYPE_FRAME
};

/** Flags that are signalled by the @ref ICANErrorFrame event. The application can
    enable these events by defining white filter conditions using @ref ICANErrorFrameFilter. */
enum CANErrorFrame
{
   /** Bit stuff error */
   CAN_ERR_BITSTUFF = 1,
   /** Format error */
   CAN_ERR_FORMAT = 2,
   /** Acknowledge error */
   CAN_ERR_ACK = 4,
   /** Bit error */
   CAN_ERR_BIT = 8,
   /** CRC error */
   CAN_ERR_CRC = 16,
   /** 
     * Bit error desired recessive but monitored dominant  
     * Could happen if e.g. another node or the physical 
     * layer driver is defect and produces a permenant dominant
     * level.
     */
   CAN_ERR_BIT_RECSV_BUT_DOMINANT = 32,
   /**
     * Bit error desired dominant but monitored recessive
     * Could happen if e.g. the bus topilogy wasn't correctly
     * designed 
     */
   CAN_ERR_BIT_DOMINANT_BUT_RECSV = 64,
   /** Overload error */
   CAN_ERR_OVERLOAD = 128,
   /** Other unspecified error */
   CAN_ERR_UNKNOWN = 0x80000000
};

 /** Flags that are signalled by the @ref ICANEvent. The application can
    enable these events by defining white filter conditions using @ref ICANEventFilter. */
enum CANBusState
 {
     /// The CAN controller has switched into the active state. 
     CAN_BUS_ACTIVE   = 1,
 
     /// The CAN controller has switched into the passive state. 
     CAN_BUS_PASSIVE  = 2,
     
     /// The warning limit of the error counter has exceeded.
     CAN_BUS_ERRLIMIT = 4,
     
     /// The controller has switched into the bus off state.
     CAN_BUS_BUSOFF   = 8,

     /// The controller has switched to single wire mode. 
     /// (fault tolerant physical medium only) 
     CAN_BUS_FAULT_TOLERANT_SINGLE_WIRE = 16
 };

/// @}

#include "../Common/EndNamespace.h"

#endif // !defined(__OLI_CANBASE_INCLUDED__)
