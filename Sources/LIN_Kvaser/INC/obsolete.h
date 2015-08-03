/**
 * \section LICENSE
 *                   Copyright 1998-2011 by KVASER AB, SWEDEN
 *
 *                        WWW: http://www.kvaser.com
 *
 * This software is furnished under a license and may be used and copied
 * only in accordance with the terms of such license.
 *
 * \section DESCRIPTION
 *
 *   This file contains definitions which are retained for compatibility.
 *
 * \file obsolete.h
 * \author Kvaser AB
 */

#ifndef _OBSOLETE_H_
#define _OBSOLETE_H_

/**
 * \defgroup Obsolete Obsolete API Reference
 * @{
 */

#define canCIRCUIT_ANY          -1   ///< Any circuit will do \warning Obsolete! 
#define canCARD_ANY             -1   ///< Any card will do    \warning Obsolete! 
#define canCHANNEL_ANY          -1   ///< Any channel will do \warning Obsolete! 

#define CANID_METAMSG  (-1L)        ///< Like msgs containing bus status changes. \warning Obsolete! 
#define CANID_WILDCARD (-2L)        ///< We don't care or don't know. \warning Obsolete! 

/**
 * Retained for compatibility.
 */
 
/**
 * \warning Obsolete!
 */
typedef void* (*canMemoryAllocator)  (size_t size);

/**
 * \warning Obsolete!
 */
typedef void  (*canMemoryDeallocator)(void* p);

/**
 * \warning Obsolete!
 */
typedef struct tagCanHWDescr {
    int                    circuitType;
    int                    cardType;
    int                    channel;
} canHWDescr;

/**
 * \warning Obsolete!
 */
typedef struct tagCanSWDescr {
    unsigned int           rxBufSize;
    unsigned int           txBufSize;
    canMemoryAllocator     alloc;
    canMemoryDeallocator   deAlloc;
} canSWDescr;

#ifdef __WIN32__
/**
 * \warning Obsolete!
 */
typedef struct {
    unsigned int size;
    unsigned int rxBufSize;
    unsigned int txBufSize;
} canSWDescriptorEx;
#endif

/**
 * \warning Obsolete!
 */
canStatus CANLIBAPI canLocateHardware(void);

/**
 * \warning Obsolete!
 */
int CANLIBAPI canOpen(const canHWDescr * const hwdescr,
                      const canSWDescr * const swdescr,
                      const unsigned int flags);

/**
 * \warning Obsolete!
 */
canStatus CANLIBAPI canGetCircuits(int * context,
                                   char * name,
                                   char * vendor,
                                   char * version,
                                   unsigned int * cardtype,
                                   unsigned int * circtype,
                                   unsigned int * channel);

/**
 * \name Some obsolete codes; they are not used nowadays.
 * \warning Obsolete!
 * @{
 */
#define canERR_BUFOFL           canERR_RESERVED_3   ///< "Buffer overflow"                 \warning Obsolete!
#define canERR_REINIT           canERR_RESERVED_4   ///< "Reinitialization failed"         \warning Obsolete!
#define canERR_MSGLOST          canERR_RESERVED_5   ///< "Message lost by CAN-chip"        \warning Obsolete!
#define canERR_OVERRUN          canERR_RESERVED_6   ///< "Receive buffer overrun"          \warning Obsolete!
#define canERR_CARDCOMMFAILED   canERR_RESERVED_7   ///< "Can't communicate with the card" \warning Obsolete!
/** @} */

/**
 * \name Circuit status flags, retained for compatibility
 * \warning Obsolete!
 * @{
 */
#define canCIRCSTAT_ERROR_PASSIVE 0x0001    ///< Error passive \warning Obsolete!
#define canCIRCSTAT_BUS_OFF       0x0002    ///< Bus off \warning Obsolete!
#define canCIRCSTAT_ERROR_WARNING 0x0004    ///< Error counter > 96 \warning Obsolete!
/** @} */

/**
 * \name Sticky status flags
 * \warning Obsolete!
 * @{
 */
#define canSTICKY_ACK_ERROR       0x80000801 ///< \warning Obsolete!
#define canSTICKY_FORM_ERROR      0x80000002 ///< \warning Obsolete!
#define canSTICKY_STUFF_ERROR     0x80000004 ///< \warning Obsolete!
#define canSTICKY_CRC_ERROR       0x80000008 ///< \warning Obsolete!
#define canSTICKY_BIT0_ERROR      0x80000010 ///< \warning Obsolete!
#define canSTICKY_BIT1_ERROR      0x80000020 ///< \warning Obsolete!
#define canSTICKY_HW_OVERRUN      0x80000040 ///< \warning Obsolete!
#define canSTICKY_SW_OVERRUN      0x80000080 ///< \warning Obsolete!
/** @} */

#define canSTAT_INACTIVE          canSTAT_RESERVED_1    ///< The circuit is not active (removed from the sw bus) \warning Obsolete!


/*
#define canCIRCSTAT_TXERR       0x0001      // TX error has occurred
#define canCIRCSTAT_RXERR       0x0002      // Receive error has occurred
#define canCIRCSTAT_OVERRUN     0x0004      // Receiver overrun
#define canCIRCSTAT_WAKEUP      0x0008      // Wakeup has occurred
#define canCIRCSTAT_INT         0x0010      // ??
#define canCIRCSTAT_ACK         0x0020      // ACK error
#define canCIRCSTAT_BIT         0x0040      // Bit error
*/
/*
More apocryphs.
#define canCIRCSTAT_OVERRUN     0x0004      // Receiver overrun
#define canCIRCSTAT_WAKEUP      0x0008      // Wakeup has occurred
#define canCIRCSTAT_ACK         0x0010      // ACK error
#define canCIRCSTAT_BIT0        0x0020      // Bit error
#define canCIRCSTAT_BIT1        0x0040      // Bit error
#define canCIRCSTAT_BOFF        0x0080      // Chip went Bus Off
*/
/*
Even more.
#define canCIRCSTAT_BOFF_HW     0x10000L      // Chip is Bus Off
#define canCIRCSTAT_BOFF_SW     0x20000L      // We are Bus Off
*/


/**
 * \name Flags for canOpen
 * \warning Obsolete! Use \ref canOPEN_xxx instead.
 * @{
 */
#define canWANT_ACTIONS         0x01  ///< \warning Obsolete!
#define canWANT_OWN_BUFFERS     0x02  ///< \warning Obsolete!
#define canWANT_ERROR_COUNTERS  0x04  ///< \warning Obsolete!
#define canWANT_EXCLUSIVE       0x08  ///< \warning Obsolete! Use \ref canOPEN_xxx instead.
#define canWANT_EXTENDED        0x10  ///< \warning Obsolete! Use \ref canOPEN_xxx instead.
#if defined(CANLIB_DECLARE_ALL)
# define canWANT_VIRTUAL        0x20  ///< \warning Obsolete! Use \ref canOPEN_xxx instead.
#endif

/** @} */

/**
 * \name Circuit types.
 * \warning Obsolete!
 * @{
 */
// No circuit                   0
#define PCCAN_PHILIPS           1     ///< 82C200 on PCCAN 1.0  \warning Obsolete!
#define PCCAN_INTEL526          2     ///< Not supported.       \warning Obsolete!
#define PCCAN_INTEL527          3     ///< 82527 on PCCAN 1.0   \warning Obsolete!
#define CANCARD_NEC72005        4     ///< NEC72005 on CANCard  \warning Obsolete!
#define CIRC_VIRTUAL            5     ///<                      \warning Obsolete!
#define CIRC_SJA1000            6     ///<                      \warning Obsolete!
/** @} */

/**
 * \name Card types.
 * \warning Obsolete!
 * @{
 */
#define canCARD_PCCAN           1     ///< PCCAN ver 1.x (KVASER)       \warning Obsolete!
#define canCARD_CANCARD         2     ///< CANCard (Softing)            \warning Obsolete!
#define canCARD_AC2             3     ///< CAN-AC2 (Softing)            \warning Obsolete!
#define canCARD_LAPCAN          4     ///<                              \warning Obsolete!
#define canCARD_ISACAN          5     ///<                              \warning Obsolete!
#define canCARD_PCCAN_OEM       6     ///< Special for OEM PCcan cards. \warning Obsolete!
/** @} */

/**
 * \name The canFlgXXX are left for compatibility.
 * \warning Obsolete!
 * @{
 */
#define canFlgACCEPT            1     ///< \warning Obsolete!
#define canFlgREJECT            2     ///< \warning Obsolete!
#define canFlgCODE              3     ///< \warning Obsolete!
#define canFlgMASK              4     ///< \warning Obsolete!
/** @} */

/**
 * \name Flags for action routines
 * \warning Obsolete!
 * @{
 */
#define canDISCARD_MESSAGE      3     ///< \warning Obsolete!
#define canRETAIN_MESSAGE       4     ///< \warning Obsolete!
/** @} */

/**
 * \name For busParams - sync on rising edge only or both rising and falling edge
 * (Not supported)
 * \warning Obsolete!
 * @{
 */
#define canSLOW_MODE            0x01  ///< Sync on rising and falling edge \warning Obsolete!
/** @} */

/**
 * \name CAN driver types; these constants are retained for compatibility.
 * (Not supported)
 * \warning Obsolete!
 * @{
 */
#define canOFF                  0     ///<                            \warning Obsolete!
#define canTRISTATE             1     ///<                            \warning Obsolete!
#define canPULLUP               2     ///<                            \warning Obsolete!
#define canPULLDOWN             3     ///<                            \warning Obsolete!
#define canPUSHPULL             4     ///< This is the usual setting. \warning Obsolete!
#define canINVPULLUP            5     ///<                            \warning Obsolete!
#define canINVPULLDOWN          6     ///<                            \warning Obsolete!
#define canINVPUSHPULL          7     ///<                            \warning Obsolete!
/** @} */

/**
 * \name These were never implemented.
 * (Not supported)
 * \warning Obsolete!
 * @{
 */
#define canIOCTL_LOCAL_ECHO_ON  3     ///< \warning Obsolete!
#define canIOCTL_LOCAL_ECHO_OFF 4     ///< \warning Obsolete!

#define canMSG_STATUS 0x0008
/** @} */

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \warning Obsolete!
 */
canStatus CANLIBAPI canInstallAction(int handle, long id, int (*fn)());

/**
 * \warning Obsolete!
 */
canStatus CANLIBAPI canUninstallAction(int handle, long id);

/**
 * \warning Obsolete!
 */
canStatus CANLIBAPI canInstallOwnBuffer(int handle, long id, unsigned int len,
                                        void * buf);

/**
 * \warning Obsolete!
 */
canStatus CANLIBAPI canUninstallOwnBuffer(int handle, long id);
#ifdef __cplusplus
}
#endif

/** @} */

#endif
