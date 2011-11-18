/**
 * \section LICENSE
 *                   Copyright 1994-2011 by KVASER AB, SWEDEN
 *
 *                        WWW: http://www.kvaser.com
 *
 * This software is furnished under a license and may be used and copied
 * only in accordance with the terms of such license.
 *
 * \section DESCRIPTION
 *
 *   Definitions for the CANLIB API.
 *
 * \file canlib.h
 * \author Kvaser AB
 *
 * \note MATLAB users on Windows: if you define WIN32_LEAN_AND_MEAN before 
 * including this file, you will see a lot less warnings.
 *
 * \defgroup General                 General
 * \defgroup CAN                     CAN
 * \defgroup ObjectBuffers           Object buffers
 * \defgroup TimeDomainHandling      Time Domain Handling
 * \defgroup NamedParameterSettings  Named Parameter Settings
 * \defgroup tScript                 t-script
 */

#ifndef _CANLIB_H_
#define _CANLIB_H_

#include <stdlib.h>

#if defined(_WIN32_WCE)
# include <windows.h>
# if defined(DEBUG) && DEBUG
#   define CANLIB_DECLARE_ALL
# endif
#elif defined(_WIN32)
# include <windows.h>
# include "predef.h"
# include "canevt.h"
# define CANLIB_DECLARE_ALL
#else
# if defined(DEBUG) && DEBUG
#   define CANLIB_DECLARE_ALL
# endif
# define BYTE unsigned char
# define DWORD unsigned int
#endif
#include "canstat.h"

typedef int canHandle;
#if defined(_WIN32) && !defined(_WIN32_WCE)
# define CanHandle int
# define canINVALID_HANDLE      (-1)
#else
   typedef canHandle CanHandle;

/**
 * \ingroup CAN
 */
typedef struct canNotifyData {
  void *tag;
  int eventType;
  union {
    struct {
      unsigned long time;
    } busErr;
    struct {
      long id;
      unsigned long time;
    } rx;
    struct {
      long id;
      unsigned long time;
    } tx;
    struct {
      unsigned char busStatus;
      unsigned char txErrorCounter;
      unsigned char rxErrorCounter;
      unsigned long time;
    } status;
  } info;
} canNotifyData;

#endif

/* Notify message sent to the application window */
#if defined(_WIN32) && !defined(_WIN32_WCE)
# define WM__CANLIB              (WM_USER+16354)
#else
# define WM__CANLIB 648
#endif

/**
 * \anchor canOPEN_xxx
 *
 * Don't allow sharing of this circuit between applications.
 *
 * This define is used in canOpenChannel()
 */
#define canOPEN_EXCLUSIVE               0x0008
/**
 * This flag causes two things to happen:
 *
 * \li The call will fail if the specified circuit doesn't allow extended CAN
 *     (CAN 2.0B)
 *
 * \li if no frame-type flag is specified in a call to canWrite, it is assumed
 *     that extended CAN should be used.
 *
 * This define is used in canOpenChannel()
 */
#define canOPEN_REQUIRE_EXTENDED        0x0010
#if defined(CANLIB_DECLARE_ALL)

/**
 * Allow opening of virtual channels as well as physical channels.
 *
 * This define is used in canOpenChannel()
 *
 * \sa \ref page_user_guide_virtual_info
 */
# define canOPEN_ACCEPT_VIRTUAL         0x0020

/**
 * Open the channel even if it is opened for exclusive
 * access already. 
 *
 * \warning Use this flag with caution.
 *
 * This define is used in canOpenChannel()
 */
# define canOPEN_OVERRIDE_EXCLUSIVE     0x0040

/**
 * Fail the call if the channel cannot be opened with init access.
 *
 * Init access means that the thread that owns the handle can set bit rate and
 * CAN driver mode. Init access is the default. At most one thread can have
 * init access to any given channel. If you try to set the bit rate or CAN
 * driver mode for a handle to which you don't have init access, the call will
 * silently fail (i.e. canOK is returned although the call had no effect),
 * unless you enable "access error reporting" by calling canIoCtl(). Access
 * error reporting is by default off.
 *
 * This define is used in canOpenChannel()
 *
 * \sa ::canOPEN_NO_INIT_ACCESS
 */
# define canOPEN_REQUIRE_INIT_ACCESS    0x0080

/**
 * Don't open the handle with init access.
 *
 * This define is used in canOpenChannel()
 *
 * \sa ::canOPEN_REQUIRE_INIT_ACCESS
 */
# define canOPEN_NO_INIT_ACCESS         0x0100

/**
 * The channel will accept messages with DLC (Data Length Code) greater than
 * 8. If this flag is not used, a message with DLC > 8 will always be
 * reported or transmitted as a message with DLC = 8. If the
 * ::canOPEN_ACCEPT_LARGE_DLC flag is used, the message will be sent and/or
 * received with the true DLC, which can be at most 15. 
 *
 * \note The length of the message is always at most 8.
 *
 * This define is used in canOpenChannel()
 */
# define canOPEN_ACCEPT_LARGE_DLC       0x0200  // DLC can be greater than 8
#endif

/** 
 * \ingroup CAN
 * \name canFILTER
 *
 * Flags for canAccept()
 *
 * @{
 */
#define canFILTER_ACCEPT        1
#define canFILTER_REJECT        2
/** Sets the code for standard (11-bit) identifiers. */
#define canFILTER_SET_CODE_STD  3
/** Sets the mask for standard (11-bit) identifiers. */
#define canFILTER_SET_MASK_STD  4
/** Sets the code for extended (29-bit) identifiers. */
#define canFILTER_SET_CODE_EXT  5
/** Sets the mask for extended (29-bit) identifiers. */
#define canFILTER_SET_MASK_EXT  6

#define canFILTER_NULL_MASK     0L
/** @} */


/** 
 * \ingroup CAN
 * \name canDRIVER_xxx
 * \anchor canDRIVER_xxx
 *
 * CAN driver types - not all are supported on all cards.
 *
 * @{
 */

/**
 * The "normal" driver type (push-pull). This is the default.
 */
#define canDRIVER_NORMAL          4
/**
 * Sets the CAN controller in Silent Mode; that is, it doesn't send anything,
 * not even ACK bits, on the bus.  Reception works as usual.
 *
 * \note The values 2,3,5,6,7 are reserved values for compatibility reasons.
 */
#define canDRIVER_SILENT           1 
#define canDRIVER_SELFRECEPTION    8
#define canDRIVER_OFF              0
// 2,3,5,6,7 are reserved values for compatibility reasons.

/** @} */


/** 
 * \ingroup CAN
 * \anchor BAUD_xxx 
 * \anchor canBITRATE_xxx 
 * \name canBITRATE_xxx
 *
 * Common bus speeds. Used in canSetBusParams() and canSetBusParamsC200().
 * The values are translated in canlib, canTranslateBaud().
 *
 * \note The BAUD_xxx names are only retained for compability.
 *
 * \sa \ref page_user_guide_misc_bitrate
 *
 * \todo Change all BAUD_xxx references to CANBITRATE_xxx ?
 *
 * @{
 */

#define canBITRATE_1M        (-1)
#define canBITRATE_500K      (-2)
#define canBITRATE_250K      (-3)
#define canBITRATE_125K      (-4)
#define canBITRATE_100K      (-5)
#define canBITRATE_62K       (-6)
#define canBITRATE_50K       (-7)
#define canBITRATE_83K       (-8)
#define canBITRATE_10K       (-9)
// The BAUD_xxx names are retained for compability.
#define BAUD_1M              (-1)
#define BAUD_500K            (-2)
#define BAUD_250K            (-3)
#define BAUD_125K            (-4)
#define BAUD_100K            (-5)
#define BAUD_62K             (-6)
#define BAUD_50K             (-7)
#define BAUD_83K             (-8)
/* @} */



/*
** IOCTL types
*/
#define canIOCTL_PREFER_EXT             1
#define canIOCTL_PREFER_STD             2
// 3,4 reserved.
#define canIOCTL_CLEAR_ERROR_COUNTERS   5
#define canIOCTL_SET_TIMER_SCALE        6
#define canIOCTL_SET_TXACK              7


#define CANID_METAMSG  (-1L)        // Like msgs containing bus status changes
#define CANID_WILDCARD (-2L)        // We don't care or don't know

//
// Define CANLIBAPI unless it's done already.
// (canlib.c provides its own definitions of CANLIBAPI, DLLIMPORT
// and DLLEXPORT before including this file.)
//
#ifndef CANLIBAPI
# ifdef _WIN32
#   define CANLIBAPI __stdcall
#   define DLLIMPORT __declspec(dllimport)
#   define DLLEXPORT __declspec(dllexport)
# else
#   define CANLIBAPI 
#   define __stdcall
# endif
#endif


#ifdef __cplusplus
extern "C" {
#endif

/**
 * \ingroup General
 *
 * This function must be called before any other functions is used.  It will
 * initialize the driver.
 *
 * You may call canInitializeLibrary() more than once. The actual
 * initialization will take place only once.
 *
 * Any errors encountered during library initialization will be "silent" and an
 * appropriate \ref canERR_xxx error code will be returned later on when
 * canOpenChannel() (or any other API call that requires initialization) is
 * called.
 *
 * \note This call replaces the canLocateHardware() API call and serves the
 *  same purpose.
 *
 * \sa \ref page_code_snippets_examples
 *
 */ 
void CANLIBAPI canInitializeLibrary (void);

/**
 * \ingroup CAN
 * 
 * Closes the channel associated with the handle. If no other threads
 * are using the CAN circuit, it is taken off bus. The handle can not be
 * used for further references to the channel, so any variable containing
 * it should be zeroed.
 * 
 * canClose() will almost always return ::canOK; the specified handle is closed
 * on an best-effort basis.
 *
 * \param[in]  hnd  An open handle to a CAN channel.
 * 
 * \return ::canOK (zero) if success
 * \return \ref canERR_xxx (negative) if failure
 *
 * \sa \ref page_code_snippets_examples
 * \sa canOpenChannel(), canBusOn(), canBusOff()
 */
canStatus CANLIBAPI canClose (const CanHandle hnd);

/**
 * \ingroup CAN
 * 
 * Takes the specified channel on-bus.
 * 
 * If you are using multiple handles to the same physical channel, for example
 * if you are writing a threaded application, you must call canBusOn() once for
 * each handle. The same applies to canBusOff() - the physical channel will not
 * go off bus until the last handle to the channel goes off bus.
 *
 * \param[in]  hnd  An open handle to a CAN channel.
 * 
 * \return ::canOK (zero) if success
 * \return \ref canERR_xxx (negative) if failure
 *
 * \sa \ref page_code_snippets_examples
 * \sa canBusOff(), canResetBus()
 *
 */
canStatus CANLIBAPI canBusOn (const CanHandle hnd);

/**
 * \ingroup CAN
 * 
 * Takes the specified channel off-bus.
 * 
 * \param[in]  hnd  An open handle to a CAN channel.
 * 
 * \return ::canOK (zero) if success
 * \return \ref canERR_xxx (negative) if failure
 *
 * \sa \ref page_code_snippets_examples
 * \sa canBusOn(), canResetBus()
 *
 */
canStatus CANLIBAPI canBusOff (const CanHandle hnd);

/**
 * \ingroup CAN
 * 
 * This function sets the bus timing parameters for the specified CAN controller.
 *
 * The library provides default values for \a tseg1, \a tseg2, \a sjw and \a
 * noSamp when \a freq is specified to one of the
 * pre-defined constants, \ref BAUD_xxx.
 *
 * If \a freq is any other value, no default values are supplied by the
 * library.
 *
 * If you are using multiple handles to the same physical channel, for example
 * if you are writing a threaded application, you must call canBusOff() once
 * for each handle. The same applies to canBusOn() - the physical channel will
 * not go off bus until the last handle to the channel goes off bus.
 *
 * \note Use canSetBusParamsC200() to set the bus timing parameters in the 
 *  ubiquitous 82c200 bit-timing register format.
 *
 * \param[in]  hnd       An open handle to a CAN controller.
 * \param[in]  freq      Bit rate (measured in bits per second); or one of the
 *                       predefined constants \ref BAUD_xxx, which are described below.
 * \param[in]  tseg1     Time segment 1, that is, the number of quanta from (but not
 *                       including) the Sync Segment to the sampling point.
 * \param[in]  tseg2     Time segment 2, that is, the number of quanta from the sampling
 *                       point to the end of the bit.  
 * \param[in]  sjw       The Synchronization Jump Width; can be 1,2,3, or 4.
 * \param[in]  noSamp    The number of sampling points; can be 1 or 3.
 * \param[in]  syncmode  Unsupported and ignored.
 * 
 * \return ::canOK (zero) if success
 * \return \ref canERR_xxx (negative) if failure
 * 
 * \sa \ref page_code_snippets_bit_rate, \ref page_user_guide_misc_bitrate,
 * \ref page_user_guide_init_bit_rate, \ref page_code_snippets_examples
 * \sa canSetBusParamsC200(), canGetBusParams()
 *
 */
canStatus CANLIBAPI canSetBusParams (const CanHandle hnd,
                                     long freq,
                                     unsigned int tseg1,
                                     unsigned int tseg2,
                                     unsigned int sjw,
                                     unsigned int noSamp,
                                     unsigned int syncmode);

/**
 * \ingroup CAN
 * 
 * This function retrieves the current bus parameters for the specified
 * channel.
 *
 * The anatomy of a CAN bit is discussed in detail at Kvaser's
 * web site at <a href="http://www.kvaser.com">www.kvaser.com</a>.
 * 
 * \param[in]  hnd       An open handle to a CAN controller.
 * \param[out] freq      Bit rate (bits per second).
 * \param[out] tseg1     Time segment 1, that is, the number of quanta from (but not
 *                       including) the Sync Segment to the sampling point.
 * \param[out] tseg2     Time segment 2, that is, the number of quanta from the sampling
 *                       point to the end of the bit.
 * \param[out] sjw       The Synchronization Jump Width; can be 1,2,3, or 4.
 * \param[out] noSamp    The number of sampling points; can be 1 or 3.
 * \param[out] syncmode  Unsupported, always read as zero.
 * 
 * \return ::canOK (zero) if success
 * \return \ref canERR_xxx (negative) if failure
 * 
 * \sa \ref page_code_snippets_bit_rate, \ref page_user_guide_init_bit_rate
 * \sa canSetBusParams(), canSetBusParamsC200()
 *
 */
canStatus CANLIBAPI canGetBusParams (const CanHandle hnd,
                                     long  *freq,
                                     unsigned int *tseg1,
                                     unsigned int *tseg2,
                                     unsigned int *sjw,
                                     unsigned int *noSamp,
                                     unsigned int *syncmode);

/**
 * \ingroup CAN
 * 
 * This function sets the driver type for a CAN controller. This corresponds
 * loosely to the bus output control register in the CAN controller, hence the
 * name of this function. CANLIB does not allow for direct manipulation of the
 * bus output control register; instead, symbolic constants are used to select
 * the desired driver type.
 *
 * \note Not all CAN driver types are supported on all cards.
 * 
 * \param[in]  hnd         A handle to an open circuit.
 * \param[out] drivertype  Can driver type, \ref canDRIVER_xxx)
 * 
 * \return ::canOK (zero) if success
 * \return \ref canERR_xxx (negative) if failure
 * 
 * \sa canGetBusOutputControl()
 */
canStatus CANLIBAPI canSetBusOutputControl (const CanHandle hnd,
                                            const unsigned int drivertype);

/**
 * \ingroup CAN
 * 
 * This function retrieves the current CAN controller driver type.
 * This corresponds loosely to the bus output control register in the
 * CAN controller, hence the name of this function. CANLIB does not
 * allow for direct manipulation of the bus output control register;
 * instead, symbolic constants are used to select the desired driver
 * type.
 * 
 * \note Don't confuse the CAN controller driver type with the bus driver
 *       type. The CAN controller is not connected directly to the CAN bus;
 *       instead, it is connected to a bus transceiver circuit which interfaces
 *       directly to the bus. The "CAN controller driver type" we are talking
 *       about here refers to the mode which the CAN controller uses to drive
 *       the bus transceiver circuit.
 *
 * \note Silent Mode is not supported by all CAN controllers.
 * 
 * \param[in] hnd         An open handle to a CAN circuit.
 * \param[in] drivertype  A pointer to an unsigned int which receives the
 *                        current driver type. The driver type can be either
 *                        ::canDRIVER_NORMAL or ::canDRIVER_SILENT.
 * 
 * \return ::canOK (zero) if success
 * \return \ref canERR_xxx (negative) if failure
 *
 * \sa canSetBusOutputControl()
 */
canStatus CANLIBAPI canGetBusOutputControl (const CanHandle hnd,
                                            unsigned int *drivertype);

/**
 * \ingroup CAN
 * 
 * This routine sets the message acceptance filters on a CAN channel.
 * 
 * On some boards the acceptance filtering is done by the CAN hardware; on
 * other boards (typically those with an embedded CPU,) the acceptance
 * filtering is done by software. canAccept() behaves in the same way for all
 * boards, however.
 *
 * canSetAcceptanceFilter() and canAccept() both serve the same purpose but the
 * former can set the code and mask in just one call.
 *
 * If you want to remove a filter, call canAccept() with the mask set to 0.
 *
 * \note You can set the extended code and mask only on CAN boards that support
 *       extended identifiers.
 *
 * \note Not all CAN boards support different masks for standard and extended
 *       CAN identifiers.
 *
 * \param[in]  hnd       An open handle to a CAN circuit.
 * \param[in]  envelope  The mask or code to set.
 * \param[in]  flag      Any of ::canFILTER_SET_CODE_STD,
 *                       ::canFILTER_SET_MASK_STD,
 *                       ::canFILTER_SET_CODE_EXT or
 *                       ::canFILTER_SET_MASK_EXT
 * 
 * \return ::canOK (zero) if success
 * \return \ref canERR_xxx (negative) if failure
 * 
 * \sa \ref page_user_guide_misc_code_and_mask, 
 *     \ref page_user_guide_send_recv_filters, 
 *     \ref page_code_snippets_examples
 */
canStatus CANLIBAPI canAccept (const CanHandle hnd,
                               const long envelope,
                               const unsigned int flag);

/**
 * \ingroup CAN
 * 
 * Returns the status for the specified circuit. flags points to a longword
 * which receives a combination of the \ref canSTAT_xxx flags.
 * 
 * \note canReadStatus() returns the latest known status of the specified
 *       circuit. If a status change happens precisely when canReadStatus() is
 *       called, it may not be reflected in the returned result.
 *
 * \param[in]  hnd    A handle to an open circuit.
 * \param[out] flags  Pointer to a \c DWORD which receives the status flags;
 *                    this is a combination of any of the \ref canSTAT_xxx.
 *
 * \return ::canOK (zero) if success
 * \return \ref canERR_xxx (negative) if failure
 * 
 */
canStatus CANLIBAPI canReadStatus (const CanHandle hnd,
                                   unsigned long *const flags);

/**
 * \ingroup CAN
 * 
 * Reads the error counters of the CAN controller.
 *
 * canReadErrorCounters() returns the latest known values of the error counters
 * in the specified circuit. If the error counters change values precisely when
 * canReadErrorCounters() is called, it may not be reflected in the returned
 * result.
 *
 * It is allowed to pass \c NULL as the value of the \a txErr, \a rxErr, and \a
 * ovErr parameters.
 *
 * Use canIoCtl() to clear the counters.
 *
 * \note Not all CAN controllers provide access to the error counters;
 *       in this case, an educated guess is returned.
 *
 * \param[in]  hnd    A handle to an open circuit.
 * \param[out] txErr  A pointer to a \c DWORD which receives the transmit error
 *                    counter.
 * \param[out] rxErr  A pointer to a \c DWORD which receives the receive error
 *                    counter.
 * \param[out] ovErr  A pointer to a \c DWORD which receives the number of
 *                    overrun errors.
 * 
 * \return ::canOK (zero) if success
 * \return \ref canERR_xxx (negative) if failure
 * 
 * \sa canIoCtl()
 */
canStatus CANLIBAPI canReadErrorCounters (const CanHandle hnd,
                                          unsigned int *txErr,
                                          unsigned int *rxErr,
                                          unsigned int *ovErr);

/**
 * \ingroup CAN
 * 
 * This function sends a CAN message. The call returns immediately after queuing 
 * the message to the driver.
 *
 * \note The message has been queued for transmission when this calls return. 
 *       It has not necessarily been sent.
 *
 * \param[in]  hnd       A handle to an open CAN circuit.
 * \param[in]  id        The identifier of the CAN message to send. 
 * \param[in]  msg       A pointer to the message data, or \c NULL. 
 * \param[in]  dlc       The length of the message. Can be at most 8. 
 * \param[in]  flag      A combination of message flags, \ref canMSG_xxx. 
 *                       Use this parameter to send extended (29-bit) frames 
 *                       and/or remote frames. Use ::canMSG_EXT and/or 
 *                       ::canMSG_RTR for this purpose. 
 *
 * \return ::canOK (zero) if success
 * \return \ref canERR_xxx (negative) if failure
 * 
 * \sa \ref page_user_guide_send_recv_sending, \ref page_code_snippets_examples
 * \sa canWriteSync(), canWriteWait()
 *
 */
canStatus CANLIBAPI canWrite (const CanHandle hnd, 
                              long id, 
                              void *msg,
                              unsigned int dlc, 
                              unsigned int flag);

/**
 * \ingroup CAN
 * 
 * Waits until all CAN messages for the specified handle are sent, or the
 * timeout period expires.
 *
 * \param[in]  hnd       A handle to an open CAN circuit.
 * \param[in]  timeout   The timeout in milliseconds.
 * 
 * \return ::canOK (zero) if the queue emptied before the timeout period came to 
 *         its end. 
 * \return ::canERR_TIMEOUT (negative) not all messages were transmitted when 
 *         the timeout occurred. 
 * \return ::canERR_PARAM (negative) This could be caused by an erroneous 
 *         parameter, or if you have turned TXACKs off (by using canIoCtl()) 
 *         because if you do you can't use this call. The driver simply doesn't 
 *         know when all the messages are sent! 
 * \return \ref canERR_xxx (negative) if failure
 *
 * \sa canWrite(), canWriteWait()
 */
canStatus CANLIBAPI canWriteSync (const CanHandle hnd, unsigned long timeout);

/**
 * \ingroup CAN
 * 
 * Reads a message from the receive buffer. If no message is available, the 
 * function returns immediately with return code ::canERR_NOMSG.
 *
 * It is allowed to pass \c NULL as the value of \a id, \a msg, \a dlc, \a
 * flag, and \a time.
 *
 * \param[in]  hnd   A handle to an open circuit.
 * \param[out] id    Pointer to a buffer which receives the CAN identifier. 
 *                   This buffer will only get the identifier. To determine 
 *                   whether this identifier was standard (11-bit) or extended 
 *                   (29-bit), and/or whether it was remote or not, or if it 
 *                   was an error frame, examine the contents of the flag 
 *                   argument. 
 * \param[out] msg   Pointer to the buffer which receives the message data. 
 *                   This buffer must be large enough (i.e. 8 bytes.) Only the
 *                   message data is copied; the rest of the buffer is left 
 *                   as-is.  
 * \param[out] dlc   Pointer to a buffer which receives the message length.  
 * \param[out] flag  Pointer to a buffer which receives the message flags,
 *                   which is a combination of the \ref canMSG_xxx and 
 *                   \ref canMSGERR_xxx values. 
 * \param[out] time  Pointer to a buffer which receives the message time stamp. 
 *
 * \return ::canOK (zero) if a message was read.
 * \return ::canERR_NOMSG (negative) if there was no message available.
 * \return \ref canERR_xxx (negative) if failure
 * 
 * \sa \ref page_user_guide_send_recv_reading, \ref
 * page_user_guide_send_recv_mailboxes, \ref page_code_snippets_examples,
 * \ref page_user_guide_time_accuracy_and_resolution
 * \sa canReadSpecific(), canReadSpecificSkip(), canReadSync(),
 * canReadSyncSpecific(), canReadWait()
 *
 */
canStatus CANLIBAPI canRead (const CanHandle hnd,
                             long *id,
                             void *msg,
                             unsigned int *dlc,
                             unsigned int *flag,
                             unsigned long *time);

/**
 * \ingroup CAN
 * 
 * Reads a message from the receive buffer. If no message is available, the 
 * function waits until a message arrives or a timeout occurs.
 *
 * It is allowed to pass \c NULL as the value of \a id, \a msg, \a dlc, \a
 * flag, and \a time.
 *
 * \param[in]   hnd    A handle to an open circuit.
 * \param[out]  id     Pointer to a buffer which receives the CAN identifier. 
 *                     This buffer will only get the identifier. To determine 
 *                     whether this identifier was standard (11-bit) or extended 
 *                     (29-bit), and/or whether it was remote or not, or if it 
 *                     was an error frame, examine the contents of the flag 
 *                     argument. 
 * \param[out]  msg    Pointer to the buffer which receives the message data. 
 *                     This buffer must be large enough (i.e. 8 bytes.).
 * \param[out]  dlc    Pointer to a buffer which receives the message length.  
 * \param[out]  flag   Pointer to a buffer which receives the message flags,
 *                     which is a combination of the \ref canMSG_xxx and 
 *                     \ref canMSGERR_xxx values. 
 * \param[out] time    Pointer to a buffer which receives the message time stamp. 
 * \param[in]  timeout If no message is immediately available, this parameter 
 *                     gives the number of milliseconds to wait for a message 
 *                     before returning. 
 *
 * \return ::canOK (zero) if a message was read.
 * \return ::canERR_NOMSG (negative) if there was no message available.
 * \return \ref canERR_xxx (negative) if failure
 *
 * \sa canRead(), canReadSpecific(), canReadSpecificSkip(), canReadSync(),
 * canReadSyncSpecific()
 * \sa \ref page_user_guide_time_accuracy_and_resolution
 */
canStatus CANLIBAPI canReadWait (const CanHandle hnd,
                                 long *id,
                                 void *msg,
                                 unsigned int  *dlc,
                                 unsigned int  *flag,
                                 unsigned long *time,
                                 unsigned long timeout);

#if defined(CANLIB_DECLARE_ALL)

/**
 * \ingroup CAN
 * 
 * Reads a message with a specified identifier from the receive buffer. Any 
 * preceding message not matching the specified identifier will be kept 
 * in the receive buffer. If no message with the specified identifier is 
 * available, the function returns immediately with an error code.
 *
 * It is allowed to pass \c NULL as the value of \a id, \a msg, \a dlc, \a
 * flag, and \a time.
 *
 * \note Use canReadSpecific only if you for some reason must keep the unread 
 *       messages in the queue. If this is not the case, consider using 
 *       canReadSpecificSkip() or canRead() for better performance. 
 *
 * \param[in]   hnd    A handle to an open circuit.
 * \param[out]  id     The desired CAN identifier.  
 * \param[out]  msg    Pointer to the buffer which receives the message data. 
 *                     This buffer must be large enough (i.e. 8 bytes.).
 * \param[out]  dlc    Pointer to a buffer which receives the message length.  
 * \param[out]  flag   Pointer to a buffer which receives the message flags,
 *                     which is a combination of the \ref canMSG_xxx and 
 *                     \ref canMSGERR_xxx values. 
 * \param[out] time    Pointer to a buffer which receives the message time stamp. 
 *
 * \return ::canOK (zero) if a message was read.
 * \return ::canERR_NOMSG (negative) if a matching message was not found. 
 *         There might be other messages in the queue, though.
 * \return \ref canERR_xxx (negative) if failure
 * 
 * \sa \ref page_user_guide_send_recv_mailboxes \ref
 * page_user_guide_send_recv_reading \ref
 * page_user_guide_time_accuracy_and_resolution
 * \sa canRead(), canReadSpecificSkip(), canReadSync(), canReadSyncSpecific(),
 * canReadWait()
 *
 */
canStatus CANLIBAPI canReadSpecific (const CanHandle hnd, long id, void * msg,
                                     unsigned int * dlc, unsigned int * flag,
                                     unsigned long * time);

/**
 * \ingroup CAN
 * 
 * Waits until the receive buffer contains at least one message or a timeout 
 * occurs.
 *
 * \param[in]  hnd      A handle to an open circuit.
 * \param[in]  timeout  The timeout in milliseconds. 
 *
 * \return ::canOK (zero) if the queue contains the desired message.
 * \return ::canERR_TIMEOUT (negative) if a timeout occurs before a message
 *         arrived.
 * \return \ref canERR_xxx (negative) if the call fails.
 * 
 * \sa canRead(), canReadSpecific(), canReadSpecificSkip(),
 * canReadSyncSpecific(), canReadWait()
 */
canStatus CANLIBAPI canReadSync (const CanHandle hnd, unsigned long timeout);

/**
 * \ingroup CAN
 * 
 * Waits until the receive queue contains a message with the specified id, or a 
 * timeout occurs..
 *
 * \param[in]  hnd      A handle to an open circuit.
 * \param[in]  id       The desired message identifier. 
 * \param[in]  timeout  The timeout in milliseconds. 
 *
 * \return ::canOK (zero) if the queue contains the desired message.
 * \return ::canERR_TIMEOUT (negative) if a timeout occurs before the specified 
 *         message arrived.
 * \return \ref canERR_xxx (negative) if the call fails.
 * 
 * \sa canRead(), canReadSpecific(), canReadSpecificSkip(), canReadSync(),
 * canReadWait()
 */
canStatus CANLIBAPI canReadSyncSpecific (const CanHandle hnd, 
                                         long id, 
                                         unsigned long timeout);

/**
 * \ingroup CAN
 * 
 * Reads a message with a specified identifier from the receive buffer. Any 
 * preceding message not matching the specified identifier will be removed
 * in the receive buffer. If no message with the specified identifier is 
 * available, the function returns immediately with an error code.
 *
 * It is allowed to pass \c NULL as the value of \a id, \a msg, \a dlc, \a
 * flag, and \a time.
 *
 * \param[in]   hnd    A handle to an open circuit.
 * \param[out]  id     The desired CAN identifier.  
 * \param[out]  msg    Pointer to the buffer which receives the message data. 
 *                     This buffer must be large enough (i.e. 8 bytes.).
 * \param[out]  dlc    Pointer to a buffer which receives the message length.  
 * \param[out]  flag   Pointer to a buffer which receives the message flags,
 *                     which is a combination of the \ref canMSG_xxx and 
 *                     \ref canMSGERR_xxx values. 
 * \param[out] time    Pointer to a buffer which receives the message time stamp. 
 *
 * \return ::canOK (zero) if a matching message was found.
 * \return ::canERR_NOMSG if there was no matching message available. All other 
           messages (if any!) were discarded.
 * \return \ref canERR_xxx (negative) if failure
 * 
 * \sa \ref page_user_guide_send_recv_reading, \ref
 * page_user_guide_time_accuracy_and_resolution
 * \sa canRead(), canReadSpecific(), canReadSync(), canReadSyncSpecific(),
 * canReadWait()
 */
canStatus CANLIBAPI canReadSpecificSkip (const CanHandle hnd,
                                         long id,
                                         void * msg,
                                         unsigned int * dlc,
                                         unsigned int * flag,
                                         unsigned long * time);
#endif

#if defined(_WIN32)

/**
 * \ingroup CAN
 *
 * This function associates a window handle with the CAN circuit. A
 * \c WM__CANLIB message is sent to that window when certain events
 * (specified by the \ref canNOTIFY_xxx flags) occur. 
 *
 * When an event take place, a \c WM__CANLIB message will be sent to the window
 * whose handle is aHWnd. This \c WM__CANLIB message will have:
 *
 * \li \c WPARAM handle to the circuit where the event occurred
 * \li \c HIWORD(LPARAM) 0
 * \li \c LOWORD(LPARAM) \ref canEVENT_xxx
 *
 * In the routine that handles \c WM__CANLIB, you can call the CANLIB API
 * functions (for example, canRead()) using the handle found in \c wParam.
 *
 * \param[in]  hnd          A handle to an open CAN circuit.
 * \param[in] aHWnd         Handle of the window which will receive the 
 *                          notification messages.
 * \param[in] aNotifyFlags  The events specified with \ref canNOTIFY_xxx, for
 *                          which callback should be called.
 * 
 * \return ::canOK (zero) if success
 * \return \ref canERR_xxx (negative) if failure
 * 
 * \n In the routine that handles \c WM__CANLIB, you must call canRead() repeatedly
 *    until it returns ::canERR_NOMSG, regardless of the \c LPARAM value. This will
 *    flush the driver's internal event queues. If you fail to do this, no more
 *    events will be reported.
 *
 * \sa \ref page_code_snippets_examples
 */
canStatus CANLIBAPI canSetNotify (const CanHandle hnd, 
                                  HWND aHWnd,
                                  unsigned int aNotifyFlags);
#else
/**
 * \ingroup CAN
 *
 * \todo Rewrite, this is just guessing from windows version
 *
 * This function associates a callback function with the CAN circuit.
 *
 * \param[in] hnd          A handle to an open CAN circuit.
 * \param[in] callback     Handle to callback routine.
 * \param[in] notifyFlags  The events specified with \ref canNOTIFY_xxx, for
 *                         which callback should be called.
 * 
 * \return ::canOK (zero) if success
 * \return \ref canERR_xxx (negative) if failure
 * 
 */
canStatus CANLIBAPI canSetNotify (const CanHandle hnd,
                                  void (*callback)(canNotifyData *),
                                  unsigned int notifyFlags, 
                                  void *tag);
#endif

#if defined(_WIN32_WCE) || !defined(_WIN32)
/**
 * \ingroup Unknown
 * 
 * \todo Document this function
 * 
 * 
 * \param[in]  hnd   CanHandle
 * \param[out] pvFd  Pointer to raw can data.
 * 
 * \return ::canOK (zero) if success
 * \return \ref canERR_xxx (negative) if failure
 * 
 */
canStatus CANLIBAPI canGetRawHandle (const CanHandle hnd, void *pvFd);
#endif

/**
 * \ingroup CAN
 * 
 * This function translates the \ref BAUD_xxx constants to their corresponding
 * bus parameter values. At return, this \a freq contains the actual bit rate
 * (in bits per second). \a TSeg1 is the number of quanta (less one) in a bit
 * before the sampling point. \a TSeg2 is the number of quanta after the
 * sampling point.
 * 
 * \param[in]  freq      A pointer to a \c DWORD which contains the \ref BAUD_xxx 
 *                       constant to translate
 * \param[in]  tseg1     A pointer to a buffer which receives the Time segment 1, 
 *                       that is, the number of quanta from (but not including) 
 *                       the Sync Segment to the sampling point.
 * \param[in]  tseg2     A pointer to a buffer which receives the Time segment 2, 
 *                       that is, the number of quanta from the sampling point to 
 *                       the end of the bit.
 * \param[in]  sjw       A pointer to a buffer which receives the Synchronization 
 *                       Jump Width; can be 1,2,3, or 4.
 * \param[in]  nosamp    A pointer to a buffer which receives the number of 
 *                       sampling points; can be 1 or 3.
 * \param[in]  syncMode  Unsupported, always read as zero.
 * 
 * \return ::canOK (zero) if success
 * \return \ref canERR_xxx (negative) if failure
 * 
 * \sa canSetBusParams()
 */
canStatus CANLIBAPI canTranslateBaud (long *const freq,
                                      unsigned int *const tseg1,
                                      unsigned int *const tseg2,
                                      unsigned int *const sjw,
                                      unsigned int *const nosamp,
                                      unsigned int *const syncMode);

/**
 * \ingroup General
 * 
 * This function translates an error code (\ref canERR_xxx)
 * to a human-readable, English text.
 * 
 * \param[in]     err     The error code.
 * \param[in,out] buf     The buffer which is to receive the text, which is a
 *                        zero-terminated string (provided the buffer is large enough.)
 * \param[in]     bufsiz  The length of the input buffer.
 * 
 * \return ::canOK (zero) if success
 * \return \ref canERR_xxx (negative) if failure
 * 
 * \sa \ref page_code_snippets_examples
 *
 */
canStatus CANLIBAPI canGetErrorText (canStatus err, char *buf, unsigned int bufsiz);

/**
 * \ingroup  General
 * 
 * This API call returns the version of the CANLIB API DLL (canlib32.dll).  The
 * most significant byte is the major version number and the least significant
 * byte is the minor version number.
 *
 * The actual version of the different driver files can be obtained by studying
 * the version resources in each of the files.
 *
 * \note The version number of the canlib32.dll file is not related to the
 *       product version of CANLIB you are using. CANLIB consists of several
 *       driver and DLL files. To obtain the product version, use
 *       canGetVersionEx().
 * 
 * \return version number of canlib32.dll
 *
 * \sa canGetVersionEx(), canProbeVersion()
 */
unsigned short CANLIBAPI canGetVersion (void);

/**
 * \ingroup General
 * 
 * This API call performs several different functions; these are described
 * below. The functions are handle-specific unless otherwise noted; this means
 * that they affect only the handle you pass to canIoCtl(), whereas other open
 * handles will remain unaffected.  The contents of \a buf after the call is
 * dependent on the function code you specified.
 * 
 * \param[in]     hnd     A handle to an open circuit.
 * \param[in]     func    A \ref canIOCTL_xxx function code
 * \param[in,out] buf     Pointer to a buffer containing function-dependent data;
                          or a \c NULL pointer for certain function codes. The
                          buffer can be used for both input and output
                          depending on the function code. See \ref canIOCTL_xxx.
 * \param[in]     buflen  The length of the buffer.
 * 
 * \return ::canOK (zero) if success
 * \return \ref canERR_xxx (negative) if failure
 * 
 */
canStatus CANLIBAPI canIoCtl (const CanHandle hnd, 
                              unsigned int func,
                              void *buf, 
                              unsigned int buflen);

/* Note the difference from the windows version */
#if defined(_WIN32) && !defined(_WIN32_WCE)

/**
 * \ingroup CAN
 * 
 * Reads the current time from the clock used to timestamp the
 * messages for the indicated circuit.
 *
 * This API may return ::canERR_INVHANDLE and/or ::canERR_NOTINITIALIZED!
 * This happens if \a hnd is invalid, or if the library was not initialized.
 *
 * \note The clock used to timestamp the messages may not be available for
 * direct reading on all platforms. In such cases, the PC's clock is used
 * to return an approximation of the current time. Note that clock drift might
 * occur in this case.
 *
 * \param[in]  hnd  A handle to an open circuit.
 * 
 * \return The current time, with the prevailing time resolution (milliseconds
 *         by default).
 * 
 * \sa \ref page_user_guide_time_accuracy_and_resolution
 * \sa kvReadTimer()
 */
unsigned long CANLIBAPI canReadTimer (const CanHandle hnd);
#else

/**
 * \ingroup CAN
 * 
 * Reads the current time from the clock used to timestamp the
 * messages for the indicated circuit.
 * 
 * This API may return ::canERR_INVHANDLE and/or ::canERR_NOTINITIALIZED! 
 * This happens if \a hnd is invalid, or if the library was not initialized.
 *
 * \note The clock used to timestamp the messages may not be available for
 * direct reading on all platforms. In such cases, the PC's clock is used
 * to return an approximation of the current time. Note that clock drift might
 * occur in this case.
 * 
 * \param[in]  hnd   A handle to an open circuit.
 * \param[out] time  The current time, with the prevailing time resolution.
 * 
 * \return ::canOK (zero) if success
 * \return \ref canERR_xxx (negative) if failure
 * 
 * \sa \ref page_user_guide_time_accuracy_and_resolution
 * \sa kvReadTimer()
 *
 * \todo kvReadTimer() claims that canReadTimer should not be used since it
 * does not return any error
 */
canStatus CANLIBAPI canReadTimer (const CanHandle hnd, unsigned long *time);
#endif

/**
 * \ingroup CAN
 * 
 * Opens a CAN channel (circuit) and returns a handle which is used
 * in subsequent calls to CANLIB.
 * 
 *
 * Channel numbering is dependent on the installed hardware. The first channel
 * always has number 0.
 *
 * For example,
 *
 * \li If you have a single LAPcan, the channels are numbered 0 and 1.
 *
 * \li If you have a USBcan Professional, the channels are numbered 0-1
 *     according to the labels on the cables.
 *
 * \li The virtual channels comes after all physical channels.
 *
 * If you are using multiple threads, note that the returned handle is usable
 * only in the context of the thread that created it. That is, you must call
 * canOpenChannel() in each of the threads in your application that uses the
 * CAN bus. You can open the same channel from multiple threads, but you must
 * call canOpenChannel() once per thread.
 *
 * If you are using the same channel from multiple threads, note that the
 * different threads will "hear" each other just as if each handle referred to
 * a channel of its own. If you open, say, channel 0 from thread A and thread B
 * and then send a message from thread A, it will be "received" by thread B.
 *
 * \note The handle returned may be zero which is perfectly valid.
 *
 * \note This call replaces the canOpen() API call and serves the same purpose.
 *
 * \param[in]  channel  The number of the channel. Channel numbering is hardware
 *                      dependent.
 * \param[in]  flags    A combination of \ref canOPEN_xxx flags
 * 
 * \return Returns a handle to the opened circuit, or \ref canERR_xxx
 *         (negative) if the call failed.
 * 
 * \sa \ref page_code_snippets_examples,  \ref page_user_guide_virtual_info
 * \sa canGetNumberOfChannels(), canGetChannelData(), canIoCtl()
 *
 */
CanHandle CANLIBAPI canOpenChannel (int channel, int flags);

/**
 * \ingroup General
 * 
 * This function returns the number of available CAN channels in the
 * computer. The virtual channels are included in this number.
 *
 * \param[out] channelCount  A pointer to a \c DWORD which will receive the current
 *                           number of channels.
 * 
 * \return ::canOK (zero) if success
 * \return \ref canERR_xxx (negative) if failure
 * 
 * \sa \ref page_code_snippets_examples, \ref page_user_guide_virtual_info
 * \sa canGetChannelData()
 */
canStatus CANLIBAPI canGetNumberOfChannels (int *channelCount);

/**
 * \ingroup General
 * 
 * This function can be used to retrieve certain pieces of information about a channel.
 *
 * \note You must pass a channel number and not a channel handle.
 * 
 * \param[in]  channel  The number of the channel you are interested in. Channel
 *                        numbers are integers in the interval beginning at 0
 *                        (zero) and ending at the value returned by
 *                        canGetNumberOfChannels() minus 1.
 * \param[in]  item  This parameter specifies what data to obtain for the
 *                        specified channel. The value is one of the constants
 *                        \ref canCHANNELDATA_xxx.
 * \param[in,out] buffer  The address of a buffer which is to receive the data.
 * \param[in]  bufsize    The size of the buffer to which the buffer parameter
 *                        points.
 * 
 * \return ::canOK (zero) if success
 * \return \ref canERR_xxx (negative) if failure
 * 
 * \sa \ref page_code_snippets_examples
 * \sa canGetNumberOfChannels()
 */
canStatus CANLIBAPI canGetChannelData (int channel, 
                                       int item, 
                                       void *buffer, 
                                       size_t bufsize);

/**
 * \ingroup General
 * \anchor canCHANNELDATA_xxx
 * \name canCHANNELDATA_xxx
 *
 * These defines are used in canGetChannelData()
 * 
 *  @{
 */

  /**
   * This define is used in canGetChannelData(), \a buffer and \a bufsize
   * refers to this functions arguments.
   *
   * \a buffer points to a 32-bit unsigned integer that receives the
   * capabilities of the CAN controller; this is a combination of the \ref
   * canCHANNEL_CAP_xxx flags.
   */
#define canCHANNELDATA_CHANNEL_CAP                1

  /**
   * This define is used in canGetChannelData(), \a buffer and \a bufsize
   * refers to this functions arguments.
   *
   * \a buffer points to a 32-bit unsigned integer that receives the
   * capabilities of the CAN transceiver; this is a combination of the
   * \ref canDRIVER_CAP_xxx flags.
   */
#define canCHANNELDATA_TRANS_CAP                  2

  /**
   * This define is used in canGetChannelData(), \a buffer and \a bufsize
   * refers to this functions arguments.
   *
   * \note Currently not implemented.
   */
#define canCHANNELDATA_CHANNEL_FLAGS              3   // available, etc

  /**
   * This define is used in canGetChannelData(), \a buffer and \a bufsize
   * refers to this functions arguments.
   *
   * \a buffer points to a 32-bit unsigned integer that receives the hardware
   * type of the card. This value is any one of the \ref canHWTYPE_xxx
   * constants.
   */
#define canCHANNELDATA_CARD_TYPE                  4

  /**
   * This define is used in canGetChannelData(), \a buffer and \a bufsize
   * refers to this functions arguments.
   *
   * \a buffer points to a 32-bit unsigned integer that receives the card's
   * number in the computer. Each card type is numbered separately. For
   * example, the first LAPcan card in a machine will have number 0, the second
   * LAPcan number 1, etc.
   */
#define canCHANNELDATA_CARD_NUMBER                5

  /**
   * This define is used in canGetChannelData(), \a buffer and \a bufsize
   * refers to this functions arguments.
   *
   * \a buffer points to a 32-bit unsigned integer which receives the channel
   * number on the card.
   */
#define canCHANNELDATA_CHAN_NO_ON_CARD            6

  /**
   * This define is used in canGetChannelData(), \a buffer and \a bufsize
   * refers to this functions arguments.
   *
   * \a buffer points to a 64-bit (8 bytes) area which receives the serial
   * number of the card. If the card doesn't have a serial number, 0 is
   * returned. The serial number is an 8-byte unsigned integer. Currently, no
   * products are using all 8 bytes; at most 4 bytes are used.
   */
#define canCHANNELDATA_CARD_SERIAL_NO             7

  /**
   * This define is used in canGetChannelData(), \a buffer and \a bufsize
   * refers to this functions arguments.
   * 
   * \a buffer points to a 64-bit (8 bytes) area which receives the serial
   * number of the transceiver. The serial number is an 8-byte unsigned
   * integer. If the transceiver doesn't have a serial number, 0 is returned.
   */
#define canCHANNELDATA_TRANS_SERIAL_NO            8

  /**
   * This define is used in canGetChannelData(), \a buffer and \a bufsize
   * refers to this functions arguments.
   *
   * \a buffer points to a 64-bit (8 bytes) area which receives the firmware
   * revision number on the card. This number consists of four 16-bit words:
   * the major revision, the minor revision, the release number and the build
   * number, listed in order from the most significant to the least
   * significant.
   */
#define canCHANNELDATA_CARD_FIRMWARE_REV          9

  /**
   * This define is used in canGetChannelData(), \a buffer and \a bufsize
   * refers to this functions arguments.
   *
   * \a buffer points to a 64-bit (8 bytes) area which receives the hardware
   * revision number on the card. This number consists of four 16-bit words;
   * the two most significant are always 0, and the two least significant are
   * the major revision and the minor revision, listed in order from the most
   * significant to the least significant.
   */
#define canCHANNELDATA_CARD_HARDWARE_REV          10

  /**
   * This define is used in canGetChannelData(), \a buffer and \a bufsize
   * refers to this functions arguments.
   *
   * \a buffer points to a 8-byte area which receives the UPC (EAN) number for
   * the card. If there is no UPC number, the buffer is filled with zeros. The
   * UPC (EAN) number is coded as a BCD string with the LSB first, so
   * e.g. 733-0130-00122-0 is coded as 0x30001220 0x00073301.
   */
#define canCHANNELDATA_CARD_UPC_NO                11

  /**
   * This define is used in canGetChannelData(), \a buffer and \a bufsize
   * refers to this functions arguments.
   *
   * \a buffer points to a 8-byte area which receives the UPC (EAN) number for
   * the transceiver. If there is no UPC number, the buffer is filled with
   * zeros. The UPC (EAN) number is coded as a BCD string with the LSB first,
   * so e.g. 733-0130-00122-0 is coded as 0x30001220 0x00073301.
   */
#define canCHANNELDATA_TRANS_UPC_NO               12

  /**
   * This define is used in canGetChannelData(), \a buffer and \a bufsize
   * refers to this functions arguments.
   *
   * \a buffer points to an area which receives a zero-terminated string with a
   * clear-text name of the channel.
   *
   * \note Use of this item code is no longer recommended. The returned
   * channel name doesn't contain the exact hardware type (it just contains
   * the device family) and uses zero-based channel numbering, which is not
   * user friendly.  Instead, use e.g. ::canCHANNELDATA_DEVDESCR_ASCII and
   * ::canCHANNELDATA_CHAN_NO_ON_CARD to build your own channel name.
   *
   * \sa ::canCHANNELDATA_DEVNAME_ASCII
   */
#define canCHANNELDATA_CHANNEL_NAME               13
#if defined(CANLIB_DECLARE_ALL)

  /**
   * This define is used in canGetChannelData(), \a buffer and \a bufsize
   * refers to this functions arguments.
   *
   * \a buffer points to an array of 4 16-bit unsigned integers which receives
   * the file version number of the second-level DLL driver file, i.e. the DLL
   * that interfaces between CANLIB32.DLL and the driver proper.
   *
   * Contents depening on index:
   *
   * \li 0: 0
   * \li 1: The build number
   * \li 2: The minor revision number
   * \li 3: The major revision number 
   */
# define canCHANNELDATA_DLL_FILE_VERSION          14

  /**
   * This define is used in canGetChannelData(), \a buffer and \a bufsize
   * refers to this functions arguments.
   *
   * \a buffer points to an array of 4 16-bit unsigned integers which receives
   * the product version number of the second-level DLL driver file, i.e. the
   * DLL that interfaces between CANLIB32.DLL and the driver proper.
   *
   * Contents depening on index:
   *
   * \li 0: 0
   * \li 1: 1
   * \li 2: The minor revision number
   * \li 3: The major revision number 
   */
# define canCHANNELDATA_DLL_PRODUCT_VERSION       15

  /**
   * This define is used in canGetChannelData(), \a buffer and \a bufsize
   * refers to this functions arguments.
   *
   * \a buffer points to a 32-bit unsigned integer which receives a number that
   * identifies the second-level DLL driver file, i.e. the DLL that interfaces
   * between CANLIB32.DLL and the driver proper.
   *
   * Values:
   *
   * \li 1: kvalapw.dll - used with CANLIB up to 2.29 together with the Windows
   *        95/98/ME VxD and the NT 4.0 VCAN driver.
   *
   * \li 2: kvalapw2.dll - used with CANLIB from 3.0 and on
   *        together with the WDM and Windows 2000/XP drivers.
   *
   * \todo Should this still be here? Should we report 64bit somehow?
   */
# define canCHANNELDATA_DLL_FILETYPE              16

  /**
   * This define is used in canGetChannelData(), \a buffer and \a bufsize
   * refers to this functions arguments.
   *
   * \a buffer points to a 32-bit unsigned integer which receives the CAN
   * transceiver type of the specified channel.  This value is one of the
   * \ref canTRANSCEIVER_TYPE_xxx
   */
# define canCHANNELDATA_TRANS_TYPE                17

  /**
   * This define is used in canGetChannelData(), \a buffer and \a bufsize
   * refers to this functions arguments.
   *
   * \a buffer points to a 32-bit unsigned integer which receives an address
   * indicating where the device is located on its underlying bus. The
   * interpretation of this number is bus-specific. If the address is unknown
   * or the bus driver does not support an address, the bus driver leaves this
   * member at its default value of 0xFFFFFFFF.
   *
   * The following list describes the information certain bus drivers store in
   * the Address field for their child devices:
   *
   * \li ISA: Does not supply an address. Defaults to 0xFFFFFFFF.
   *
   * \li PC Card (PCMCIA): The socket number (typically 0x00 or 0x40)
   *
   * \li PCI: The device number in the high word and the function number in the
   *          low word.
   *
   * \li USB: The port number.
   *
   * \note Windows 2000, XP and later only
   */
# define canCHANNELDATA_DEVICE_PHYSICAL_POSITION  18

  /**
   * This define is used in canGetChannelData(), \a buffer and \a bufsize
   * refers to this functions arguments.
   *
   * \a buffer points to a 32-bit unsigned integer which receives a number
   * associated with the device that can be displayed in the user
   * interface. This number is typically a user-perceived slot number, such as
   * a number printed next to the slot on the board, or some other number that
   * makes locating the physical device easier for the user. For buses with no
   * such convention, or when the UI number is unknown, 0xFFFFFFFF is returned.
   *
   * \note Windows 2000, XP and later only
   */
# define canCHANNELDATA_UI_NUMBER                 19

  /**
   * This define is used in canGetChannelData(), \a buffer and \a bufsize
   * refers to this functions arguments.
   *
   * \a buffer points to a 32-bit unsigned integer which is set to 0, if the
   * legacy time synchronization is not currently enabled for the specified
   * channel, and 1, if the legacy time synchronization is currently enabled
   * for the specified channel.
   *
   * Legacy time synchronization is a mechanism that will keep the PC and CAN
   * channel clocks in sync. The synchronization is done in the driver, which
   * periodically calculates the difference between the PC clock and the CAN
   * device clock and compensates for the clock drift by recalculating the CAN
   * message time stamps. You need to enable clock synchronization in the
   * Control Panel using the Kvaser Hardware applet.
   *
   * \note Legacy time synchronization is implemented only on LAPcan and LAPcan
   * II. It is not related to Kvaser MagiSync&tm; which is implemented in the
   * high-end members of the Kvaser Leaf family. Kvaser MagiSync&tm; is always
   * enabled and allows for much more accurate time synchronization.
   *
   * \note Not supported on Windows 95
   */
# define canCHANNELDATA_TIMESYNC_ENABLED          20

  /**
   * This define is used in canGetChannelData(), \a buffer and \a bufsize
   * refers to this functions arguments.
   *
   * \a buffer points to an array of four 16-bit unsigned integers which
   * receives the file version number of the kernel-mode driver.
   *
   * Contents depening on index:
   *
   * \li 0: The build number
   * \li 1: 0
   * \li 2: The minor revision number
   * \li 3: The major revision number
   */
# define canCHANNELDATA_DRIVER_FILE_VERSION       21

  /**
   * This define is used in canGetChannelData(), \a buffer and \a bufsize
   * refers to this functions arguments.
   *
   *  \a buffer points to an array of four 16-bit unsigned integers which
   *  receives the product version number of the kernel-mode driver.
   *
   * Contents depening on index:
   *
   * \li 0: 0
   * \li 1: 0
   * \li 2: The minor revision number
   * \li 3: The major revision number
   */
# define canCHANNELDATA_DRIVER_PRODUCT_VERSION    22

  /**
   * This define is used in canGetChannelData(), \a buffer and \a bufsize
   * refers to this functions arguments.
   *
   * \a buffer points to a buffer which receives the device manufacturer's name
   * as a zero-terminated Unicode string.
   */
# define canCHANNELDATA_MFGNAME_UNICODE           23

  /**
   * This define is used in canGetChannelData(), \a buffer and \a bufsize
   * refers to this functions arguments.
   *
   * \a buffer points to a buffer which receives the device manufacturer's name
   * as a zero-terminated ASCII string.
   */
# define canCHANNELDATA_MFGNAME_ASCII             24

  /**
   * This define is used in canGetChannelData(), \a buffer and \a bufsize
   * refers to this functions arguments.
   *
   * \a buffer points to a buffer which receives the product name of the device
   * as a zero-terminated Unicode string.
   */
# define canCHANNELDATA_DEVDESCR_UNICODE          25

  /**
   * This define is used in canGetChannelData(), \a buffer and \a bufsize
   * refers to this functions arguments.
   *
   * \a buffer points to a buffer which receives the product name of the device
   * as a zero-terminated ASCII string.
   */
# define canCHANNELDATA_DEVDESCR_ASCII            26
# define canCHANNELDATA_DRIVER_NAME               27

  /**
   * This define is used in canGetChannelData(), \a buffer and \a bufsize
   * refers to this functions arguments.
   *
   * \a buffer points to a 32-bit unsigned integer that receives the quality of
   * the channel, where the quality is measured in percent of optimal quality.
   * 
   * For WLAN, -90 dBm and -35 dBm are considered 0% and 100%, respectively.
   *
   * The quality is 100% for any directly connected channel (USB, PCI etc.).
   */
# define canCHANNELDATA_CHANNEL_QUALITY           28

  /**
   * This define is used in canGetChannelData(), \a buffer and \a bufsize
   * refers to this functions arguments.
   *
   * \a buffer point to a 32-bit unsigned integer that receives the roundtrip
   * time which is measured in milliseconds.
   */
# define canCHANNELDATA_ROUNDTRIP_TIME            29

  /**
   * This define is used in canGetChannelData(), \a buffer and \a bufsize
   * refers to this functions arguments.
   *
   * \a buffer points to a 32-bit unsigned integer that receives the 
   * \ref kvBUSTYPE_GROUP_xxx bus type.
   */
# define canCHANNELDATA_BUS_TYPE                  30

  /**
   * This define is used in canGetChannelData(), \a buffer and \a bufsize
   * refers to this functions arguments.
   *
   * \a buffer points to a CHAR array of at least 32 characters which receives
   * the current device name as a \c NULL terminated ASCII string.
   *
   * If device name is not set or the device does not support this
   * functionality, an error will be returned.
   */
# define canCHANNELDATA_DEVNAME_ASCII             31

  /**
   * This define is used in canGetChannelData(), \a buffer and \a bufsize
   * refers to this functions arguments.
   *
   * \a buffer points to a 32-bit unsigned integer that contains the time in
   * milliseconds since the last communication occured.
   *
   * For WLAN devices, this is the time since the last keep-alive message.
   */
# define canCHANNELDATA_TIME_SINCE_LAST_SEEN      32
#endif

 /** @} */


// channelFlags in canChannelData
#define canCHANNEL_IS_EXCLUSIVE         0x0001
#define canCHANNEL_IS_OPEN              0x0002


/**
 * \name canHWTYPE_xxx
 * \anchor canHWTYPE_xxx
 *
 * The following constants can be returned from canGetChannelData(), using the
 * ::canCHANNELDATA_CARD_TYPE item code. They identify the hardware type for
 * the channel specified in the call to canGetChannelData().
 *
 * \note They indicate a hardware type, but not necessarily a specific
 * product. For example, ::canHWTYPE_LAPCAN is returned both for LAPcan and
 * LAPcan II. (You can use canGetChannelData() to obtain the UPC/EAN code for
 * the device. This number uniquely identifies the product.)
 *
 *  @{
 */
#define canHWTYPE_NONE                0  ///< Unknown or undefined
#define canHWTYPE_VIRTUAL             1  ///< The virtual CAN bus
#define canHWTYPE_LAPCAN              2  ///< LAPcan Family
#define canHWTYPE_CANPARI             3  ///< CANpari (obsolete)
#define canHWTYPE_PCCAN               8  ///< PCcan Family
#define canHWTYPE_PCICAN              9  ///< PCIcan Family
#define canHWTYPE_USBCAN             11  ///< USBcan (obsolete)
#define canHWTYPE_PCICAN_II          40  ///< PCIcan II family
#define canHWTYPE_USBCAN_II          42  ///< USBcan II, USBcan Rugged, Kvaser Memorator
#define canHWTYPE_SIMULATED          44  ///< Simulated CAN bus for Kvaser Creator. (obsolete)
#define canHWTYPE_ACQUISITOR         46  ///< Kvaser Acquisitor (obsolete)
#define canHWTYPE_LEAF               48  ///< Kvaser Leaf Family
#define canHWTYPE_PC104_PLUS         50  ///< Kvaser PC104+
#define canHWTYPE_PCICANX_II         52  ///< Kvaser PCIcanx II
#define canHWTYPE_MEMORATOR_II       54  ///< Kvaser Memorator Professional family
#define canHWTYPE_MEMORATOR_PRO      54  ///< Kvaser Memorator Professional family
#define canHWTYPE_USBCAN_PRO         56  ///< Kvaser USBcan Professional
#define canHWTYPE_IRIS               58  ///< Iris
#define canHWTYPE_BLACKBIRD          58  ///< Kvaser BlackBird
#define canHWTYPE_MEMORATOR_LIGHT    60  ///< Kvaser Memorator Light
#define canHWTYPE_MINIHYDRA          62  ///< Eagle n�e Minihydra
#define canHWTYPE_EAGLE              62  ///< Kvaser Eagle family
 /** @} */
                                 
/**
 * \anchor canCHANNEL_CAP_xxx
 * Channel capabilities.         
 *
 * \todo Fill out these
 */
#define canCHANNEL_CAP_EXTENDED_CAN         0x00000001L
#define canCHANNEL_CAP_BUS_STATISTICS       0x00000002L
#define canCHANNEL_CAP_ERROR_COUNTERS       0x00000004L
#define canCHANNEL_CAP_CAN_DIAGNOSTICS      0x00000008L
#define canCHANNEL_CAP_GENERATE_ERROR       0x00000010L
#define canCHANNEL_CAP_GENERATE_OVERLOAD    0x00000020L
#define canCHANNEL_CAP_TXREQUEST            0x00000040L
#define canCHANNEL_CAP_TXACKNOWLEDGE        0x00000080L
#define canCHANNEL_CAP_VIRTUAL              0x00010000L
#define canCHANNEL_CAP_SIMULATED            0x00020000L
#define canCHANNEL_CAP_REMOTE               0x00040000L // Remote device, like BlackBird

  /**
   *  \anchor canDRIVER_CAP_xxx
   *  Driver (transceiver) capabilities
   */
#define canDRIVER_CAP_HIGHSPEED             0x00000001L

/**
 * \ingroup General
 * \name canIOCTL_xxx
 * \anchor canIOCTL_xxx
 *
 * These defines are used in canIoCtl(), \a buf and \a buflen refers to this
 * functions arguments.
 *
 * @{
 */
  /**
   * This define is used in canIoCtl(), \a buf and \a buflen refers to this
   * functions arguments.
   *
   * \a buf points at a \c DWORD which receives the current RX queue level. The
   * returned value is approximative (this is because not all hardware supports
   * retrieving the queue levels. In that case a best-effort guess is
   * returned. Also note that a device with embedded CPU will report its queue
   * levels to the host computer after a short delay that depends on the bus
   * traffic intensity, and consequently the value returned by the call to
   * canIoCtl() might be a few milliseconds old.)
   */
#define canIOCTL_GET_RX_BUFFER_LEVEL              8

  /**
   * This define is used in canIoCtl(), \a buf and \a buflen refers to this
   * functions arguments.
   *
   * \a buf points at a \c DWORD which receives the current TX queue level. The
   * returned value is approximative (this is because not all hardware supports
   * retrieving the queue levels. In that case a best-effort guess is
   * returned. Also note that a device with embedded CPU will report its queue
   * levels to the host computer after a short delay that depends on the bus
   * traffic intensity, and consequently the value returned by the call to
   * canIoCtl() might be a few milliseconds old.)
   */
#define canIOCTL_GET_TX_BUFFER_LEVEL              9

  /**
   * This define is used in canIoCtl(), \a buf and \a buflen refers to this
   * functions arguments.
   *
   * Discard the current contents of the RX queue. The values of \a buf and \a
   * buflen are ignored. 
   *
   * \note This is the same thing as calling canFlushReceiveQueue()
   */
#define canIOCTL_FLUSH_RX_BUFFER                  10

  /**
   * This define is used in canIoCtl(), \a buf and \a buflen refers to this
   * functions arguments.
   *
   * Discard the current contents of the TX queue. The values of \a buf and \a
   * buflen are ignored.
   *
   * \note This is the same thing as calling canFlushTransmitQueue().
   */
#define canIOCTL_FLUSH_TX_BUFFER                  11

  /**
   * This define is used in canIoCtl(), \a buf and \a buflen refers to this
   * functions arguments.
   *
   * \a buf points to a \c DWORD which contains the desired time-stamp clock
   * resolution in microseconds. Note that the accuracy of the clock isn't
   * affected. The default value is 1000 microseconds, i.e. one millisecond.
   */
#define canIOCTL_GET_TIMER_SCALE                  12

  /**
   * This define is used in canIoCtl(), \a buf and \a buflen refers to this
   * functions arguments.
   *
   * \a buf points to a \c DWORD which contains
   *
   * \li \c 0 to turn Transmit Requests off.
   * \li \c 1 to turn Transmit Requests on.
   *
   * Default value is \c 0, Transmit Requests off.
   */
#define canIOCTL_SET_TXRQ                         13

  /**
   * This define is used in canIoCtl(), \a buf and \a buflen refers to this
   * functions arguments.
   *
   * \a buf points at a \c DWORD which receives a Windows Event handle which can
   * be passed to the Win32 API \c WaitForSingleObject. The event is signaled
   * when "something" (typically that a CAN message has been received or
   * transmitted) happens in the driver. 
   *
   * \note There is no more information available as to what happened when this 
   * call returns. The call may return on an "internal" event in CANLIB and your 
   * application must be prepared to handle this (i.e. go to sleep again.)
   * \note If canWaitForEvent() returns with success status (::canOK), you must call
   * canRead() repeatedly until it returns ::canERR_NOMSG, before calling 
   * canWaitForEvent() again. This will flush the driver's internal event queues. 
   * Failure to call canRead() can cause canWaitForEvent() to get stuck in a state 
   * where it always sleeps for the specified timeout and then returns with 
   * ::canERR_TIMEOUT.
   *
   * \sa canWaitForEvent()
   *
   * \note You must not set, reset, nor close this handle.  Waiting on it is
   *       the only supported operation.
   */
#define canIOCTL_GET_EVENTHANDLE                  14

  /**
   * This define is used in canIoCtl(), \a buf and \a buflen refers to this
   * functions arguments.
   *
   * \note Not yet implemented.
   */
#define canIOCTL_SET_BYPASS_MODE                  15

#define canIOCTL_SET_WAKEUP                       16

#if defined(CANLIB_DECLARE_ALL)
# define canIOCTL_GET_DRIVERHANDLE                17
# define canIOCTL_MAP_RXQUEUE                     18
# define canIOCTL_GET_WAKEUP                      19

  /**
   * This define is used in canIoCtl(), \a buf and \a buflen refers to this
   * functions arguments.
   * 
   * \a buf points to a BYTE which contains
   *
   * \li \c 0 to turn access error reporting off, and
   * \li \c 1 to turn access error reporting on.
   *
   * Default value is \c 0, access error reporting off.
   *
   */
# define canIOCTL_SET_REPORT_ACCESS_ERRORS        20

  /**
   * This define is used in canIoCtl(), \a buf and \a buflen refers to this
   * functions arguments.
   *
   * \a buf points to a BYTE which receives the current setting of the access
   * error reporting (0 or 1.)
   */
# define canIOCTL_GET_REPORT_ACCESS_ERRORS        21

# define canIOCTL_CONNECT_TO_VIRTUAL_BUS          22
# define canIOCTL_DISCONNECT_FROM_VIRTUAL_BUS     23

  /**
   * This define is used in canIoCtl(), \a buf and \a buflen refers to this
   * functions arguments.
   *
   * \a buf points to a ::canUserIoPortData struct that contains a port number
   * and a port value to set. This is used by special hardware only.
   */
# define canIOCTL_SET_USER_IOPORT                 24

  /**
   * This define is used in canIoCtl(), \a buf and \a buflen refers to this
   * functions arguments.
   *
   * \a buf points to a ::canUserIoPortData struct that contains a port
   *  number. After the call, the struct will contain the current value of the
   *  I/O port. This is used by special hardware only.
   */
# define canIOCTL_GET_USER_IOPORT                 25

# define canIOCTL_SET_BUFFER_WRAPAROUND_MODE      26

  /**
   * This define is used in canIoCtl(), \a buf and \a buflen refers to this
   * functions arguments.
   *
   * Use this function code to set the size of the receive buffer for a
   * specific handle. \a buf points to an unsigned integer which contains the
   * new size (number of messages) of the receive buffer.
   *
   * \note The receive buffer consumes system nonpaged pool memory, which is a
   *       limited resource. Do not increase the receive buffer size unless you
   *       have good reasons to do so.
   *
   * \note You can't use this function code when the channel is on bus.
   */
# define canIOCTL_SET_RX_QUEUE_SIZE               27

# define canIOCTL_SET_USB_THROTTLE                28
# define canIOCTL_GET_USB_THROTTLE                29
# define canIOCTL_SET_BUSON_TIME_AUTO_RESET       30
# define canIOCTL_GET_TXACK                       31

  /**
   * This define is used in canIoCtl(), \a buf and \a buflen refers to this
   * functions arguments.
   *
   * \a buf points to an unsigned byte that contains 0, if the local transmit
   * echo is to be turned off for the handle, and a value other than 0 if the
   * local transmit echo is to be turned on.
   * 
   * Local transmit echo is turned on by default on all handles.  This means
   * that if two handles are open on the same channel, and a message is
   * transmitted on the first handle, it will be received as a normal message
   * on the second channel. Use the canIOCTL_SET_LOCAL_TXECHO function code to
   * turn this function off, if it is not desired on a certain handle.
   *
   */
# define canIOCTL_SET_LOCAL_TXECHO                32

# define canIOCTL_SET_ERROR_FRAMES_REPORTING      33

  /**
   * This define is used in canIoCtl(), \a buf and \a buflen refers to this
   * functions arguments.
   *
   * \a buf points to a 32-bit unsigned integer that receives the quality of
   * the channel, where the quality is measured in percent of optimal quality.
   *
   * For a WLAN, -90 dBm and -35 dBm are considered 0% and 100%, respectively.
   *
   * The quality is 100% for any directly connected channel (USB, PCI etc.).
   */
# define canIOCTL_GET_CHANNEL_QUALITY             34 ///< In % of optimal quality

  /**
   * This define is used in canIoCtl(), \a buf and \a buflen refers to this
   * functions arguments.
   * 
   * \a buf points to a \c DWORD that contains the roundtrip time measured in 
   * milliseconds.
   */
# define canIOCTL_GET_ROUNDTRIP_TIME              35

  /**
   * This define is used in canIoCtl(), \a buf and \a buflen refers to this
   * functions arguments.
   *
   * \a buf points to a \c DWORD that contains the \ref kvBUSTYPE_GROUP_xxx bus type.
   */
# define canIOCTL_GET_BUS_TYPE                    36

  /**
   * This define is used in canIoCtl(), \a buf and \a buflen refers to this
   * functions arguments.
   *
   * \a buf points to a CHAR array of at least 32 characters which receives the
   * current device name as a \c NULL terminated ASCII string.
   *
   * If device name is not set or the device does not support this
   * functionality, an error will be returned.
   */
# define canIOCTL_GET_DEVNAME_ASCII               37

  /**
   * This define is used in canIoCtl(), \a buf and \a buflen refers to this
   * functions arguments.
   *
   * \a buf points to a \c DWORD that contains the time in milliseconds since the last
   * communication occured.
   *
   * For WLAN devices, this is the time since the last keep-alive message.
   */
# define canIOCTL_GET_TIME_SINCE_LAST_SEEN        38 ///< Time since last communication. 
#endif
 /** @} */   

#if defined(CANLIB_DECLARE_ALL)
// For canIOCTL_xxx_USER_IOPORT
typedef struct {
  unsigned int portNo;
  unsigned int portValue;
} canUserIoPortData;

#endif

#if defined(CANLIB_DECLARE_ALL)
/**
 * \ingroup Notification and Waiting
 * 
 * Waits for an event (of any kind) to happen at the specified CAN circuit, or
 * a timeout to occur. An event in this context means for example the arrival
 * of a CAN message or a CAN bus status change, but it can also be an event
 * internal to the driver.
 * 
 * \param[in] hnd      A handle to an open CAN circuit.
 * \param[in] timeout  The number of milliseconds to wait before the call
 *                     returns, if no event occurs.
 * 
 * \return ::canOK (zero) if an event happened during the specified time
 *         period.
 * \return ::canERR_TIMEOUT (negative) if nothing happened during the specified
 *         time period.
 * \return \ref canERR_xxx</a> (negative) if failure.
 *
 * \sa \ref page_code_snippets_examples,
 * \ref page_user_guide_send_recv_asynch_not
 * \sa canRead()
 */
canStatus CANLIBAPI canWaitForEvent (const CanHandle hnd, DWORD timeout);
#endif

/**
 * \ingroup CAN
 * 
 * This function sets the bus timing parameters using the same
 * convention as the 82c200 CAN controller (which is the same as many
 * other CAN controllers, for example, the 82527.)
 * 
 * To calculate the bit timing parameters, you can use the bit timing
 * calculator that is included with CANLIB SDK. Look in the BIN directory.
 *
 * 82c200 Bit Timing
 *
 * \li \a btr0 [b7..b6]: SJW - 1
 * \li \a btr0 [b5..b0]: Prescaler -1 
 * \li \a btr1 [b7]: \c 1: 3 samples, \c 0: 1 samples
 * \li \a btr1 [b6..b4]: tseg2 - 1
 * \li \a btr1 [b3..b0]: tseg1 - 2
 *
 * \note CANLIB will assume that the clock frequency is 16 MHz.
 *
 * \todo The 16 MHZ note should be elaborated.
 *
 * \param[in] hnd   A handle to an open CAN circuit.
 * \param[in] btr0  The desired bit timing, formatted as the contents of the
 *                  BTR0 register in the 82c200.
 * \param[in] btr1  The desired bit timing, formatted as the contents of the
 *                  BTR1 register in the 82c200.
 * 
 * \return ::canOK (zero) if success
 * \return \ref canERR_xxx (negative) if failure
 * 
 *
 * \sa \ref page_code_snippets_bit_rate, \ref page_user_guide_misc_bitrate
 * \sa canSetBusParams()
 *
 * \todo Move the  "82c200 Bit Timing" above to a html-page?
 */
canStatus CANLIBAPI canSetBusParamsC200 (const CanHandle hnd, BYTE btr0, BYTE btr1);

#if defined(CANLIB_DECLARE_ALL)

/**
 * \ingroup CAN
 * 
 * This function sets the current CAN bus driver mode. This is
 * typically a mode like sleep, wakeup, standby, fast mode, etc. The
 * different modes are almost always hardware dependent and requires
 * special DRVcan cables. As an example, the DRVcan S implements J2411
 * compliant single-wire CAN and supports four line modes, namely
 * Normal, Sleep, Fast and Wakeup.
 * 
 * Standard ISO 11898 CAN do not support any of these bus driver modes.
 * 
 * \note The bus driver mode is typically used to control things like one- or
 * two-wire mode, sleep mode, and so on. It requires special support in the CAN
 * driver circuit.
 *
 * \param[in] hnd       An open handle to a CAN circuit.
 * \param[in] lineMode  An int which defines the line mode, 
 *                      \ref canTRANSCEIVER_LINEMODE_xxx.
 * \param[in] resNet    An int which defines the resnet mode. Set this parameter to
 *                      ::canTRANSCEIVER_RESNET_NA unless you have good reasons to set it
 *                      to something else.
 *
 * \return ::canOK (zero) if success
 * \return \ref canERR_xxx (negative) if failure
 * 
 * \sa canGetDriverMode()
 */
canStatus CANLIBAPI canSetDriverMode (const CanHandle hnd, int lineMode, int resNet);

/**
 * \ingroup CAN
 * 
 * This function retrieves the current CAN bus driver mode. This is typically a
 * mode like sleep, wakeup, standby, fast mode, etc. The different modes are
 * almost always hardware dependent and requires special DRVcan cables. As an
 * example, the DRVcan S implements J2411 compliant single-wire CAN and
 * supports four line modes, namely Normal, Sleep, Fast and Wakeup.
 *
 * Standard ISO 11898 CAN do not support any of these bus driver modes.
 *
 * \note The bus driver mode is typically used to control things like one- or
 * two-wire mode, sleep mode, and so on. It requires special support in the CAN
 * driver circuit.
 *
 * \param[in]  hnd       An open handle to a CAN circuit.
 * \param[out] lineMode  A pointer to an int which receives the current line
 *                       mode (\ref canTRANSCEIVER_LINEMODE_xxx). 
 * \param[out] resNet    A pointer to an int which receives the current resnet
 *                       mode.  This value is usually
 *                       ::canTRANSCEIVER_RESNET_NA except for special DRVcan
 *                       cables.
 * 
 * \return ::canOK (zero) if success
 * \return \ref canERR_xxx (negative) if failure
 * 
 * \sa canSetDriverMode()
 */
canStatus CANLIBAPI canGetDriverMode (const CanHandle hnd, int *lineMode, int *resNet);
#endif

/** 
 * \anchor canVERSION_CANLIB32_xxx
 * @{
 * Item codes for canGetVersionEx()
 */

  /**
   * This define is used in canGetVersionEx() and controls which version number
   * that returned.
   * 
   * Version number of the canlib32.dll file coded as an unsigned 16-bit word
   * with the major version number in the upper byte and the minor version
   * number in the lower byte. This version number is not related to the
   * product version number of the whole CANLIB.  For example, canlib32.dll
   * belonging to CANLIB 2.27 would return 0x305.
   */
#define canVERSION_CANLIB32_VERSION     0

  /** 
   * This define is used in canGetVersionEx() and controls what type of version
   * number that is returned.
   * 
   * Product version number of canlib32.dll coded as an unsigned 16-bit word
   * with the major version number in the upper byte and the minor version
   * number in the lower byte. The product version number corresponds to the
   * version number of the whole CANLIB. For example, canlib32.dll belonging to
   * CANLIB 2.27 would return 0x21B.
   */
#define canVERSION_CANLIB32_PRODVER     1

  /** 
   * This define is used in canGetVersionEx() and controls what type of version
   * number that is returned.
   * 
   * Product version number of canlib32.dll coded as an unsigned 32-bit word
   * where the bytes contain (in order from the most significant to the least
   * significant byte) 0, major version number, minor version number, and the
   * minor version letter. (The minor version letter is the ASCII code for the
   * letter, or 0 (zero) if no letter). For example, CANLIB 3.8 would return
   * 0x00030800 and CANLIB 3.8a would return 0x00030861.
   */
#define canVERSION_CANLIB32_PRODVER32   2

  /**
   * This define is used in canGetVersionEx() and controls what type of version
   * number that is returned.
   * 
   * Returns 1 if the present version is a beta (preview) release, or 0 if it
   * is an official release.
   */
#define canVERSION_CANLIB32_BETA        3
/** @} */

#if defined(CANLIB_DECLARE_ALL)

/**
 * \ingroup Information Services
 * 
 * This function returns various version numbers from the driver routines.
 * 
 * \param[in] itemCode  Specifies which version number to retrieve. See 
 *                      \ref canVERSION_CANLIB32_xxx
 * 
 * \return The return value is desired version number.
 *
 */
unsigned int CANLIBAPI canGetVersionEx (unsigned int itemCode);

/**
 * \ingroup NamedParameterSettings
 * 
 * This function returns the number of entries in the table of named
 * channels.
 * 
 * \return The number of channels (zero or positive)
 * \return \ref canERR_xxx (negative) if failure
 * 
 * \sa \ref page_user_guide_misc_named_parameters
 *
 */
canStatus CANLIBAPI canParamGetCount (void);

/**
 * \ingroup NamedParameterSettings
 * 
 * This function writes the current set of named parameters to the
 * Registry. Previous entries are erased.
 * 
 * The named parameters are stored in the
 * \c HKEY_LOCAL_MACHINE\\SOFTWARE\\KVASER \c AB\\CANLIB32\\PredefinedBitrates
 * key in the Registry.
 *
 * \note You must have Administrator's rights to write to the Registry.
 *
 * \return ::canOK (zero) if success
 * \return \ref canERR_xxx (negative) if failure
 * 
 * \sa \ref page_user_guide_misc_named_parameters
 *
 */
canStatus CANLIBAPI canParamCommitChanges (void);

/**
 * \ingroup NamedParameterSettings
 * 
 * This function deletes the entry in the table of named parameter settings
 * with the given index. The entries below (i.e. with higher indices) the
 * deleted entry are moved up one step in the table.
 *
 * The named parameters are stored in the
 * \c HKEY_LOCAL_MACHINE\\SOFTWARE\\KVASER \c AB\\CANLIB32\\PredefinedBitrates
 * key in the Registry.
 * 
 * \param[in]  index  The index of the entry to delete.
 * 
 * \return ::canOK (zero) if success
 * \return \ref canERR_xxx (negative) if failure
 * 
 * \sa \ref page_user_guide_misc_named_parameters
 * \sa canParamCreateNewEntry(), canParamCommitChanges()
 */
canStatus CANLIBAPI canParamDeleteEntry (int index);

/**
 * \ingroup NamedParameterSettings
 * 
 * This function creates a new entry in the table of named parameter
 * settings.
 * 
 * The named parameters are stored in the
 * \c HKEY_LOCAL_MACHINE\\SOFTWARE\\KVASER \c AB\\CANLIB32\\PredefinedBitrates
 * key in the Registry.
 *
 * \return The index of the created entry (zero or positive) if success
 * \return \ref canERR_xxx (negative) if failure
 * 
 * \sa \ref page_user_guide_misc_named_parameters
 * \sa canParamCommitChanges(), canParamDeleteEntry()
 *
 */
canStatus CANLIBAPI canParamCreateNewEntry (void);

/**
 * \ingroup NamedParameterSettings
 * 
 * This function swaps two entries in the list of named
 * parameters.
 *
 * The named parameters are stored in the
 * \c HKEY_LOCAL_MACHINE\\SOFTWARE\\KVASER \c AB\\CANLIB32\\PredefinedBitrates
 * key in the Registry.
 * 
 * \param[in]  index1  The first of the two entries that are to be swapped in the
 *                     named parameters list.
 * \param[in]  index2  The second of the two entries that are to be swapped in the
 *                     named parameters list.
 * 
 * \return ::canOK (zero) if success
 * \return \ref canERR_xxx (negative) if failure
 * 
 * \sa \ref page_user_guide_misc_named_parameters
 * \sa canParamCommitChanges()
 *
 */
canStatus CANLIBAPI canParamSwapEntries (int index1, int index2);

/**
 * \ingroup NamedParameterSettings
 * 
 * This function returns the name of a given entry in the list of
 * named parameters.
 *
 * The named parameters are stored in the
 * \c HKEY_LOCAL_MACHINE\\SOFTWARE\\KVASER \c AB\\CANLIB32\\PredefinedBitrates
 * key in the Registry.
 * 
 * \param[in]  index   The index of the entry in the named parameters list, whose
 *                     name is to be returned.
 * \param[out] buffer  A pointer to a buffer that is to receive a
 *                     \c NULL terminated string which contains the name. The
 *                     buffer is allocated and deallocated by the user.
 * \param[in]  maxlen  The length of the buffer.
 * 
 * \return ::canOK (zero) if success
 * \return \ref canERR_xxx (negative) if failure
 * 
 * \sa \ref page_user_guide_misc_named_parameters
 * \sa canParamSetName(), canParamCommitChanges()
 */
canStatus CANLIBAPI canParamGetName (int index, char *buffer, int maxlen);

/**
 * \ingroup NamedParameterSettings
 * 
 * This function returns the channel number of the entry with the
 * given index in the table of named parameter settings.
 * 
 * \param[in] index The index of the entry in the table of named parameter
 *                  settings.
 * 
 * \return The channel number of the entry in question (zero or positive)
 * \return \ref canERR_xxx (negative) if failure
 * 
 * \sa \ref page_user_guide_misc_named_parameters
 * \sa canParamCommitChanges(), canParamGetChannelNumber()
 */
canStatus CANLIBAPI canParamGetChannelNumber (int index);

/**
 * \ingroup NamedParameterSettings
 * 
 * This function retrieves the bus parameters associated with the
 * entry with the given index in the table of named parameter
 *   settings.
 * 
 * \param[in] index     The index of the entry in the table of named parameter
 *                      settings.
 * \param[out] bitrate  Bit rate (bits per second).
 * \param[out] tseg1    Time segment 1, that is, the number of quanta from (but
 *                      not including) the Sync Segment to the sampling point.
 * \param[out] tseg2    Time segment 2, that is, the number of quanta from the
 *                      sampling point to the end of the bit.
 * \param[out] sjw      The Synchronization Jump Width; can be 1,2,3, or 4.
 * \param[out] noSamp   The number of sampling points; can be 1 or 3.
 * 
 * \return ::canOK (zero) if success
 * \return \ref canERR_xxx (negative) if failure
 * 
 * \sa \ref page_user_guide_misc_named_parameters
 * \sa canParamCommitChanges(), canParamSetBusParams()
 */
canStatus CANLIBAPI canParamGetBusParams (int index,
                                          long* bitrate,
                                          unsigned int *tseg1,
                                          unsigned int *tseg2,
                                          unsigned int *sjw,
                                          unsigned int *noSamp);

/**
 * \ingroup NamedParameterSettings
 * 
 * This function sets or changes the name of a named parameter.
 * 
 * \param[in]  index  The index of the named parameter whose name is to be 
 *                    changed or set.
 * \param[out] buffer A pointer to a \c NULL terminated string that contains the
 *                    new name. If the string is longer than the maximum
 *                    allowed name length, it is truncated.
 * 
 * \return ::canOK (zero) if success
 * \return \ref canERR_xxx (negative) if failure
 * 
 * \sa \ref page_user_guide_misc_named_parameters
 * \sa canParamCommitChanges(), canParamGetName()
 */
canStatus CANLIBAPI canParamSetName (int index, const char *buffer);

/**
 * \ingroup NamedParameterSettings
 * 
 * This function sets the channel number for a specified entry in the list of
 * named parameters. Channels are numbered from 0 and up.
 * 
 * \param[in] index    The index of the entry in the named parameter list whose
 *                     channel number is to be set.
 * \param[in] channel  The channel number.
 * 
 * \return ::canOK (zero) if success
 * \return \ref canERR_xxx (negative) if failure
 * 
 * \sa \ref page_user_guide_misc_named_parameters
 * \sa canParamCommitChanges(), canParamGetChannelNumber()
 */
canStatus CANLIBAPI canParamSetChannelNumber (int index, int channel);

/**
 * \ingroup NamedParameterSettings
 * 
 * This function sets or changes the bus parameters for a given entry in the
 * list of named parameters.
 * 
 * \note The bus parameters are not checked for validity.
 *
 * \param[in]  index   The index of the entry in the named parameter list whose
 *                     parameters are to be set or changed.
 * \param[in] bitrate  Bit rate (measured in bits per second); or one of the
 *                     predefined constants \ref BAUD_xxx.

 * \param[in] tseg1    Time segment 1, that is, the number of quanta from (but not
 *                     including) the Sync Segment to the sampling point.
 * \param[in] tseg2    Time segment 2, that is, the number of quanta from the
 *                     sampling point to the end of the bit.

 * \param[in]  sjw     The Synchronization Jump Width; can be 1,2,3, or 4.
 * \param[in]  noSamp  The number of sampling points; can be 1 or 3.
 * 
 * \return ::canOK (zero) if success
 * \return \ref canERR_xxx (negative) if failure
 * 
 * \sa \ref page_user_guide_misc_named_parameters
 * \sa canParamCommitChanges(), canParamGetBusParams()
 */
canStatus CANLIBAPI canParamSetBusParams (int index,
                                          long bitrate,
                                          unsigned int tseg1,
                                          unsigned int tseg2,
                                          unsigned int sjw,
                                          unsigned int noSamp);

/**
 * \ingroup NamedParameterSettings
 * 
 * This function returns the index of the parameter setting with the
 * given name.
 * 
 * \param[in] name A pointer to a string containing the name of the setting.
 * 
 * \return The index of the setting (zero or positive) if success.
 * \return \ref canERR_xxx (negative) if failure
 * 
 * \sa \ref page_user_guide_misc_named_parameters
 * \sa canParamCommitChanges()
 *
 * \todo This function is NOT returning canStatus, change to int?
 */
canStatus CANLIBAPI canParamFindByName (const char *name);

/**
 * \ingroup ObjectBuffers
 * 
 * Deallocates all object buffers on the specified handle. The
 * buffers cannot be referenced after this operation.
 * 
 * \param[in]  hnd  An open handle to a CAN circuit.
 * 
 * \return ::canOK (zero) if success
 * \return \ref canERR_xxx (negative) if failure
 * 
 * \sa \ref page_user_guide_send_recv_obj_buf
 * \sa canObjBufFree(), canObjBufAllocate()
 */
canStatus CANLIBAPI canObjBufFreeAll (const CanHandle hnd);

/**
 * \ingroup ObjectBuffers
 * 
 * Allocates an object buffer associated with a handle to a CAN
 * circuit.
 * 
 * \param[in] hnd   An open handle to a CAN circuit.
 * \param[in] type  The type of the buffer. Must be one of \ref canOBJBUF_TYPE_xxx
 * 
 * \return A buffer index (zero or positive) if success.
 * \return \ref canERR_xxx (negative) if failure
 * 
 * \sa \ref page_user_guide_send_recv_obj_buf
 * \sa canObjBufFree(), canObjBufFreeAll()
 */
canStatus CANLIBAPI canObjBufAllocate (const CanHandle hnd, int type);

 /**
  * \name canOBJBUF_TYPE_xxx
  * \anchor canOBJBUF_TYPE_xxx
  *
  * Used in canObjBufAllocate()
  * 
  * @{
  */
#define canOBJBUF_TYPE_AUTO_RESPONSE            0x01 ///< The buffer is an auto-response buffer.
#define canOBJBUF_TYPE_PERIODIC_TX              0x02 ///< The buffer is an auto-transmit buffer.
 /** @} */

/**
 * \ingroup ObjectBuffers
 * 
 * Deallocates the object buffer with the specified index. The buffer
 * can not be referenced after this operation.
 * 
 * \param[in] hnd  An open handle to a CAN circuit.
 * \param[in] idx  The object buffer to deallocate.
 * 
 * \return ::canOK (zero) if success
 * \return \ref canERR_xxx (negative) if failure
 * 
 * \sa \ref page_user_guide_send_recv_obj_buf
 * \sa canObjBufFreeAll(), canObjBufAllocate(),
 */
canStatus CANLIBAPI canObjBufFree (const CanHandle hnd, int idx);

// Writes CAN data to the object buffer with the specified index.

/**
 * \ingroup ObjectBuffers
 * 
 * Defines the contents of a specific object buffer.
 * 
 * \param[in] hnd   An open handle to a CAN circuit.
 * \param[in] idx   The index of the object buffer whose contents is to be
 *                  defined.
 * \param[in] id    The CAN identifier of the message.
 * \param[in] msg   Points to the contents of the message.
 * \param[in] dlc   The length of the message. Must be at least 0 and at most 8
 *                  bytes.
 * \param[in] flags Message flags; a combination of the \ref canMSG_xxx flags.
 * 
 * \return ::canOK (zero) if success
 * \return \ref canERR_xxx (negative) if failure
 *
 * \sa \ref page_user_guide_send_recv_obj_buf 
 */
canStatus CANLIBAPI canObjBufWrite (const CanHandle hnd, 
                                    int idx, 
                                    int id, 
                                    void* msg,
                                    unsigned int dlc, 
                                    unsigned int flags);

/**
 * \ingroup ObjectBuffers
 * 
 * Defines a message reception filter on the specified object buffer.
 * Messages not matching the filter are discarded.
 *
 * \note For an auto response buffer, set the code and mask that together define
 * the identifier(s) that trigger(s) the automatic response.
 *
 * \param[in] hnd   An open handle to a CAN circuit.
 * \param[in] idx   The index of the object buffer on which the filter is to be
 *                  set.
 * \param[in] code  The acceptance code in the filter.
 * \param[in] mask  The acceptance mask in the filter.
 * 
 * \return ::canOK (zero) if success
 * \return \ref canERR_xxx (negative) if failure
 * 
 * \sa \ref page_user_guide_misc_code_and_mask,
 *     \ref page_user_guide_send_recv_obj_buf
 */
canStatus CANLIBAPI canObjBufSetFilter (const CanHandle hnd, 
                                        int idx,
                                        unsigned int code, 
                                        unsigned int mask);

/**
 * \ingroup ObjectBuffers
 * 
 * Sets object buffer flags on a specified object buffer.
 * 
 * \param[in] hnd    An open handle to a CAN circuit.
 * \param[in] idx    The buffer on which the flags are to be set.
 * \param[in] flags  Specifies a combination of zero or more of the
 *                   \ref canOBJBUF_AUTO_RESPONSE_xxx flag values
 * 
 * \return ::canOK (zero) if success
 * \return \ref canERR_xxx (negative) if failure
 * 
 * \sa \ref page_user_guide_send_recv_obj_buf
 */
canStatus CANLIBAPI canObjBufSetFlags (const CanHandle hnd, 
                                       int idx, 
                                       unsigned int flags);

/**
 * \name canOBJBUF_AUTO_RESPONSE_xxx
 * \anchor canOBJBUF_AUTO_RESPONSE_xxx
 *
 * These defines are used in canObjBufSetFlags()
 *
 * @{
 */
 /**
  * This define is used in canObjBufSetFlags()
  *
  * For auto-response buffers only. When this flag is in effect, the buffer
  * will auto-respond to remote requests only.  If this flag is not in effect,
  * the buffer will auto-respond to both remote requests and ordinary data
  * frames.
  *
  */
#define canOBJBUF_AUTO_RESPONSE_RTR_ONLY        0x01
 /** @} */   

/**
 * \ingroup ObjectBuffers
 * 
 * The canObjBufSetPeriod function sets the transmission period for an auto
 * transmission object buffer.
 * 
 * \param[in] hnd     An open handle to a CAN channel.
 * \param[in] idx     The index of a CAN object buffer.
 * \param[in] period  The transmission interval, in microseconds.
 * 
 * \return ::canOK (zero) if success
 * \return \ref canERR_xxx (negative) if failure
 * 
 * \sa \ref page_user_guide_send_recv_obj_buf
 */
canStatus CANLIBAPI canObjBufSetPeriod (const CanHandle hnd, 
                                        int idx, 
                                        unsigned int period);

/**
 * \ingroup ObjectBuffers
 * 
 * The canObjBufSetMsgCount function sets the message count for an auto
 * transmit object buffer.
 * 
 * \param[in] hnd    An open handle to a CAN channel.
 * \param[in] idx    The index of a CAN object buffer.
 * \param[in] count  The message count.
 * 
 * \return ::canOK (zero) if success
 * \return \ref canERR_xxx (negative) if failure
 * 
 * \sa \ref page_user_guide_send_recv_obj_buf
 */
canStatus CANLIBAPI canObjBufSetMsgCount (const CanHandle hnd, 
                                          int idx, 
                                          unsigned int count);

/**
 * \ingroup ObjectBuffers
 * 
 * Enables the object buffer with the specified index.
 * 
 * \param[in] hnd  An open handle to a CAN circuit.
 * \param[in] idx  The index of the object buffer to enable.
 * 
 * \return ::canOK (zero) if success
 * \return \ref canERR_xxx (negative) if failure
 * 
 * \sa \ref page_user_guide_send_recv_obj_buf
 * \sa canObjBufDisable()
 */
canStatus CANLIBAPI canObjBufEnable (const CanHandle hnd, int idx);

/**
 * \ingroup ObjectBuffers
 * 
 * Disables the object buffer with the specified index.
 * 
 * \param[in] hnd  An open handle to a CAN circuit. 
 * \param[in] idx  The index of the buffer. 
 * 
 * \return ::canOK (zero) if success
 * \return \ref canERR_xxx (negative) if failure
 * 
 * \sa \ref page_user_guide_send_recv_obj_buf
 * \sa canObjBufEnable()
 */
canStatus CANLIBAPI canObjBufDisable (const CanHandle hnd, int idx);

/**
 * \ingroup ObjectBuffers
 * 
 * The canObjBufSendBurst function sends a burst of CAN messages. You have to
 * set up an object buffer first with the message to send. The messages will be
 * sent as fast as possible from the hardware. 
 *
 * This function is inteneded for certain diagnostic applications.
 * 
 * \param[in] hnd       An open handle to a CAN channel.
 * \param[in] idx       The index of a CAN object buffer.
 * \param[in] burstlen  The number of messages to send.
 * 
 * \return ::canOK (zero) if success
 * \return \ref canERR_xxx (negative) if failure
 * 
 * \sa \ref page_user_guide_send_recv_obj_buf
 */
canStatus CANLIBAPI canObjBufSendBurst (const CanHandle hnd, 
                                        int idx, 
                                        unsigned int burstlen);

/**
 * \name canVERSION_xxx
 * \anchor canVERSION_xxx
 *
 * These defines are used in canProbeVersion().
 *
 * @{
 */
  /**
   * This define is used in canProbeVersion(), \a major and \a minor refer to
   * this functions arguments.
   *
   * Require that exactly the version specified by \a major and \a minor be
   * present. Earlier and later versions are not accepted. This flag does not
   * affect the acceptance of beta versions.
   */
#define canVERSION_DONT_ACCEPT_LATER      0x01
  /**
   * This define is used in canProbeVersion(), \a major and \a minor refer to
   * this functions arguments.
   *
   * Return FALSE if a beta version (preview version) of CANLIB is installed,
   * regardless of its version number.
   */
#define canVERSION_DONT_ACCEPT_BETAS      0x02
 /** @} */   

/**
 * \ingroup General
 * 
 * This function checks whether a specific version of CANLIB is installed on
 * the system.
 *
 * CANLIB versions are denoted by two numbers, the "major version" and
 * the "minor version". Usually the version is presented as these two numbers
 * separated by a period. For example, CANLIB 3.6 has major version 3 and minor
 * version 6.
 *
 * \todo move the above paragraph about canlib versions to html and link (see
 * also) from here and all other version handling functions
 *
 * The default behaviour of canProbeVersion is to accept
 *
 * \li the version specified by \a major and \a minor, and
 * \li any later version, and
 * \li all beta versions.
 *
 * You get the default behaviour by setting \a flags to 0. Use any
 * combination of the \ref canVERSION_xxx flags to modify the behaviour.
 * 
 * \note Different handles might have different driver versions installed. This
 * should not normally be the case but it might happen anyway. You should check
 * the version for each handle you open, e.g. directly after calling
 * canOpenChannel().
 *
 * \param[in] hnd     A handle to an open circuit.
 * \param[in] major   The major version number of the version to test for.
 * \param[in] minor   The minor version number of the version to test for.
 * \param[in] oem_id  Reserved, must be zero.
 * \param[in] flags   Any combination of the \ref canVERSION_xxx flags, or 0.
 * 
 * \return TRUE if the specified version of CANLIB is installed on the system.
 *
 * \sa \ref page_code_snippets_examples
 * \sa canGetVersion(), canGetVersionEx(), canGetChannelData()
 */
BOOL CANLIBAPI canProbeVersion (const CanHandle hnd, 
                                int major, 
                                int minor, 
                                int oem_id, 
                                unsigned int flags);

#endif

/**
 * \ingroup CAN
 * 
 * This function tries to reset a CAN bus controller by taking the channel off
 * bus and then on bus again (if it was on bus before the call to canResetBus().)
 * 
 * This function will affect the hardware (and cause a real reset of the CAN
 * chip) only if \a hnd is the only handle open on the channel. If there
 * are other open handles, this operation will not affect the hardware.
 *
 * \param[in] hnd  A handle to an open circuit.
 * 
 * \return ::canOK (zero) if success
 * \return \ref canERR_xxx (negative) if failure
 * 
 * \sa canBusOn(), canBusOff()
 */
canStatus CANLIBAPI canResetBus (const CanHandle hnd);

/**
 * \ingroup CAN
 * 
 * This function sends a CAN message. It returns when the message is sent, or 
 * the timeout expires.
 *
 * This is a convenience function that combines canWrite() and canWriteSync().
 *
 * \param[in]  hnd       A handle to an open CAN circuit.
 * \param[in]  id        The identifier of the CAN message to send. 
 * \param[in]  msg       A pointer to the message data, or \c NULL. 
 * \param[in]  dlc       The length of the message. Can be at most 8. 
 * \param[in]  flag      A combination of message flags, \ref canMSG_xxx. 
 *                       Use this parameter to send extended (29-bit) frames 
 *                       and/or remote frames. Use ::canMSG_EXT and/or 
 *                       ::canMSG_RTR for this purpose. 
 * \param[in] timeout    The timeout, in milliseconds.
 *
 * \return ::canOK (zero) if success
 * \return \ref canERR_xxx (negative) if failure
 */
canStatus CANLIBAPI canWriteWait (const CanHandle hnd, 
                                  long id, 
                                  void *msg,
                                  unsigned int dlc, 
                                  unsigned int flag,
                                  unsigned long timeout);


#if defined(CANLIB_DECLARE_ALL)

/**
 * \ingroup General
 * 
 * Use this function if you are loading CANLIB32.DLL dynamically (that is,
 * using the Win32 API \c LoadLibrary) and need to unload it using the Win32
 * API \c FreeLibrary. canUnloadLibrary() will free allocated memory, unload
 * the DLLs canlib32.dll has loaded and de-initialize data structures. You must
 * call canInitializeLibrary() again to use the API functions in canlib32.dll.
 * 
 * \return ::canOK (zero) if success
 * \return \ref canERR_xxx (negative) if failure
 * 
 * \sa canInitializeLibrary()
 */
canStatus CANLIBAPI canUnloadLibrary (void);

/**
 * \ingroup CAN
 * 
 * This routine sets the message acceptance filters on a CAN channel.
 * 
 * Format of \a code and \a mask:
 *
 * \li A binary 1 in a mask means "the corresponding bit in the code is
 *     relevant"
 * \li A binary 0 in a mask means "the corresponding bit in the code is not
 *     relevant"
 * \li A relevant binary 1 in a code means "the corresponding bit in the
 *     identifier must be 1"
 * \li A relevant binary 1 in a code means "the corresponding bit in the
 *     identifier must be 1"
 *
 * In other words, the message is accepted if ((code XOR id) AND mask) == 0.
 *
 * \a extended should be set to:
 *
 * \li \c 0 (FALSE): if the code and mask shall apply to 11-bit CAN identifiers.
 * \li \c 1 (TRUE): if the code and mask shall apply to 29-bit CAN identifiers.
 *
 * If you want to remove a filter, call canSetAcceptanceFilter() with the mask
 * set to \c 0.
 *
 * On some boards the acceptance filtering is done by the CAN hardware; on
 * other boards (typically those with an embedded CPU,) the acceptance
 * filtering is done by software.  canSetAcceptanceFilter() behaves in the same
 * way for all boards, however.
 *
 * canSetAcceptanceFilter() and canAccept() both serve the same purpose but the
 * former can set the code and mask in just one call.
 *
 * \note You can set the extended code and mask only on CAN boards that support
 *       extended identifiers.
 *
 * \note Not all CAN boards support different masks for standard and
 *       extended CAN identifiers.
 *
 * \param[in] hnd          An open handle to a CAN circuit.
 * \param[in] code         The acceptance code to set.
 * \param[in] mask         The acceptance mask to set.
 * \param[in] is_extended  Select 29-bit CAN identifiers.
 * 
 * \return ::canOK (zero) if success
 * \return \ref canERR_xxx (negative) if failure
 * 
 * \sa \ref page_code_snippets_examples,
 *     \ref page_user_guide_misc_code_and_mask
 * \sa  canAccept()
 */
canStatus CANLIBAPI canSetAcceptanceFilter (const CanHandle hnd, 
                                            unsigned int code,
                                            unsigned int mask, 
                                            int is_extended);

/**
 * \ingroup CAN
 * 
 * This function removes all received messages from the handle's receive queue.
 * Other handles open to the same channel are not affcted by this
 * operation. That is, only the messages belonging to the handle you are
 * passing to ::canFlushReceiveQueue are discarded.
 *
 * \note This call has the same effect as calling canIoCtl() with a function
 * code of canIOCTL_FLUSH_RX_BUFFER.
 *
 * \param[in] hnd  A handle to an open circuit.
 * 
 * \return ::canOK (zero) if success
 * \return \ref canERR_xxx (negative) if failure
 * 
 * \sa canFlushTransmitQueue()
 */
canStatus CANLIBAPI canFlushReceiveQueue (const CanHandle hnd);

/**
 * \ingroup CAN
 * 
 * This function removes all messages pending transmission from the
 * transmit queue of the circuit.
 *
 * \note If there are other handles open to the same circuit, they are also
 * flushed.
 *
 * \note This call has the same effect as calling canIoCtl() with a function
 * code of ::canIOCTL_FLUSH_TX_BUFFER.
 *
 * \param[in] hnd  A handle to an open circuit.
 * 
 * \return ::canOK (zero) if success
 * \return \ref canERR_xxx (negative) if failure
 * 
 * \sa canFlushReceiveQueue()
 */
canStatus CANLIBAPI canFlushTransmitQueue (const CanHandle hnd);

/**
 * \ingroup General
 * 
 * \note The kvGetApplicationMapping function is presently not implemented.
 * 
 * \param busType
 * \param appName
 * \param appChannel
 * \param resultingChannel
 * 
 * \return ::canOK (zero) if success
 * \return \ref canERR_xxx (negative) if failure
 * 
 */
canStatus CANLIBAPI kvGetApplicationMapping (int busType,
                                             char *appName,
                                             int appChannel,
                                             int *resultingChannel);

/**
 * \ingroup General
 * 
 * The kvBeep function emits a sound of a specific frequency and duration from
 * the loudspeaker on the device.
 * 
 * \note This function requires that a loudspeaker be present on the hardware.
 *
 * \param[in] hnd       An open handle to a CAN channel.
 * \param[in] freq      The frequency (in Hertz) of the sound.
 * \param[in] duration  The duration of the sound, in milliseconds.
 * 
 * \return ::canOK (zero) if success
 * \return \ref canERR_xxx (negative) if failure
 * 
 */
canStatus CANLIBAPI kvBeep (const CanHandle hnd, 
                            int freq, 
                            unsigned int duration);

/**
 * \ingroup General
 * 
 * The kvSelfTest function runs a built-in self test in the device. Note that
 * not all devices supports built-in self tests.
 * 
 * \param[in]  hnd       An open hnd to a CAN channel.
 * \param[out] presults  A pointer to a 32-bit unsigned integer where the 
 *                       resuls of the self test will be placed.
 * 
 * \return ::canOK (zero) if success
 * \return \ref canERR_xxx (negative) if failure
 * 
 */
canStatus CANLIBAPI kvSelfTest (const CanHandle hnd, unsigned long *presults);

  /**
   * \anchor kvLED_ACTION_xxx
   * \name kvLED_ACTION_xxx
   *
   * The following constants can be used together with the kvFlashLeds() function.
   *
   * @{
   */
#define kvLED_ACTION_ALL_LEDS_ON    0 ///< Turn all LEDs on.
#define kvLED_ACTION_ALL_LEDS_OFF   1 ///< Turn all LEDs off.
#define kvLED_ACTION_LED_0_ON       2 ///< Turn LED 0 on.
#define kvLED_ACTION_LED_0_OFF      3 ///< Turn LED 0 off.
#define kvLED_ACTION_LED_1_ON       4 ///< Turn LED 1 on.
#define kvLED_ACTION_LED_1_OFF      5 ///< Turn LED 1 off.
#define kvLED_ACTION_LED_2_ON       6 ///< Turn LED 2 on.
#define kvLED_ACTION_LED_2_OFF      7 ///< Turn LED 2 off.
#define kvLED_ACTION_LED_3_ON       8 ///< Turn LED 3 on.
#define kvLED_ACTION_LED_3_OFF      9 ///< Turn LED 3 off.
  /** @} */

/**
 * \ingroup General
 * 
 * The kvFlashLeds function will turn the LEDs on the device on or off.
 * 
 * \param[in] hnd
 * \param[in] action   One of the \ref kvLED_ACTION_xxx constants, defining 
 *                     which LED to turn on or off.
 * \param[in] timeout  Specifies the time, in milliseconds, during which the
 *                     action is to be carried out. When the timeout expires,
 *                     the LED(s) will return to its ordinary function.
 * 
 * \return ::canOK (zero) if success
 * \return \ref canERR_xxx (negative) if failure
 * 
 */
canStatus CANLIBAPI kvFlashLeds (const CanHandle hnd, int action, int timeout);

/**
 * \ingroup CAN
 * 
 * The canRequestChipStatus function requests that the hardware report the chip
 * status (bus on/error passive status etc.) to the driver. The chip status can
 * later be retrieved using the canReadStatus() function.
 * 
 * \note The canRequestChipStatus() function is asynchronous, that is, it
 * completes before the answer is returned from the hardware.  The time between
 * a call to canRequestChipStatus() and the point in time where the chip status
 * is actually available via a call to canReadStatus() is not
 * defined. canReadStatus() always returns the latest data reported by the
 * hardware.
 *
 * \param[in] hnd An open handle to a CAN channel.
 * 
 * \return ::canOK (zero) if success
 * \return \ref canERR_xxx (negative) if failure
 * 
 * \sa canReadStatus()
 */
canStatus CANLIBAPI canRequestChipStatus (const CanHandle hnd);

/**
 * \ingroup CAN
 * 
 * The canRequestBusStatistics function requests bus statistics from the
 * hardware. The bus statistics figures can be retrieved later by a call to the
 * canGetBusStatistics function.
 * 
 * \note The time between a call to canRequestBusStatistics() and the point in
 * time where the bus statistics is actually available via a call to
 * canGetBusStatistics() is not defined. Typically, you would call
 * canRequestBusStatistics() from your application periodically (for example,
 * once per second) to request the data from the driver and then call
 * canGetBusStatistics() with the same rate to obtain the latest reported data.
 *
 * \param[in] hnd   An open handle to a CAN channel.
 * 
 * \return ::canOK (zero) if success
 * \return \ref canERR_xxx (negative) if failure
 * 
 * \sa canGetBusStatistics()
 */
canStatus CANLIBAPI canRequestBusStatistics (const CanHandle hnd);

  /**
   * \ingroup CAN
   * This struct is returned by canGetBusStatistics()
   *
   * The values are cleared when the corresponding channel goes on bus.
   */
typedef struct canBusStatistics_s {
  unsigned long  stdData;   ///< Number of received standard (11-bit identifiers) data frames.
  unsigned long  stdRemote; ///< Number of received standard (11-bit identifiers) remote frames.
  unsigned long  extData;   ///< Number of received extended (29-bit identifiers) data frames.
  unsigned long  extRemote; ///< Number of received extended (29-bit identifiers) remote frames.
  unsigned long  errFrame;  ///< Number of error frames

  /**
   * The bus load, expressed as an integer in the interval 0 - 10000
   * representing 0.00% - 100.00% bus load.
   */
  unsigned long  busLoad;
  unsigned long  overruns;  ///< The number of overruns detected by the hardware, firmware or driver.
} canBusStatistics;

/**
 * \ingroup CAN
 * 
 * The canGetBusStatistics() function retrieves the latest bus statistics
 * figures reported by the driver.  You request the bus statistics from the
 * driver by calling the canRequestBusStatistics() function.
 * 
 * \param[in]  hnd     An open handle to a CAN channel.
 * \param[out] stat    A pointer to a ::canBusStatistics struct that will receive
 *                     the bus statistics figures.
 * \param[in]  bufsiz  The size, in bytes, of the stat buffer.
 * 
 * \return ::canOK (zero) if success
 * \return \ref canERR_xxx (negative) if failure
 * 
 * \sa canRequestBusStatistics()
 */
canStatus CANLIBAPI canGetBusStatistics (const CanHandle hnd, 
                                         canBusStatistics *stat, 
                                         size_t bufsiz);

/**
 * \ingroup CAN
 * 
 * The canSetBitrate() function sets the bit rate of the specified CAN
 * channel. The SJW is left unchanged.  The sampling point is recalculated and
 * kept as close as possible to the value before the call.
 * 
 * \param[in] hnd      An open handle to a CAN channel.
 * \param[in] bitrate  The new bit rate, in bits/second. 
 * 
 * \return ::canOK (zero) if success
 * \return \ref canERR_xxx (negative) if failure
 * 
 * \sa canSetBusParamsC200(), canSetBusParams(), canGetBusParams()
 */
canStatus CANLIBAPI canSetBitrate (const CanHandle hnd, int bitrate);

/**
 * \ingroup General
 * 
 * The kvAnnounceIdentity function is used by certain OEM applications.
 * 
 * \param[in]  hnd     An open handle to a CAN channel.
 * \param[out] buf     A pointer to the data to be sent to the driver.
 * \param[in]  bufsiz  The size, in bytes, of the buffer that buf points to.
 * 
 * \return ::canOK (zero) if success
 * \return \ref canERR_xxx (negative) if failure
 */
canStatus CANLIBAPI kvAnnounceIdentity (const CanHandle hnd, 
                                        void *buf, 
                                        size_t bufsiz);
/**
 * \ingroup General
 * 
 * The kvAnnounceIdentityEx function is used by certain OEM applications.
 * 
 * \param[in]  hnd     An open handle to a CAN channel.
 * \param[in]  type    Type of announcement.
 * \param[out] buf     A pointer to the data to be sent to the driver.
 * \param[in]  bufsiz  The size, in bytes, of the buffer that buf points to.
 * 
 * \return ::canOK (zero) if success
 * \return \ref canERR_xxx (negative) if failure
 */
canStatus CANLIBAPI kvAnnounceIdentityEx (const CanHandle hnd,
                                          int type,
                                          void *buf, 
                                          size_t bufsiz);

/**
 * \ingroup CAN
 * 
 * \note This function is not implemented.
 * 
 * \param[in]  hnd      An open handle to a CAN channel.
 * \param[in]  item
 * \param[out] buffer
 * \param[in]  bufsize
 * 
 * \return ::canOK (zero) if success
 * \return \ref canERR_xxx (negative) if failure
 * 
 * \sa canGetChannelData()
 */
canStatus CANLIBAPI canGetHandleData (const CanHandle hnd, 
                                      int item, 
                                      void *buffer, 
                                      size_t bufsize);


typedef void *kvTimeDomain;
typedef canStatus kvStatus;

/**
 * \ingroup TimeDomainHandling
 * 
 */
typedef struct kvTimeDomainData_s {
  int nMagiSyncGroups;        ///< number of MagiSync&tm; groups
  int nMagiSyncedMembers;     ///< number of MagiSync&tm; members
  int nNonMagiSyncCards;      ///< number of non MagiSync&tm; interfaces
  int nNonMagiSyncedMembers;  ///< number of non MagiSync&tm; members
} kvTimeDomainData;

/**
 * \ingroup TimeDomainHandling
 * 
 * This routine creates an empty time domain.
 * 
 * The variable is set by this function and then used in later calls to
 * other functions using a kvTimeDomain.
 *
 * Time domains created by kvTimeDomainCreate() can be destroyed with a
 * call to kvTimeDomainDelete().
 *
 * \note A time domain is a set of channels with a common time base.
 *
 * \param[in] domain  A pointer to a caller allocated, opaque variable of type
 *                    ::kvTimeDomain that holds data to identify a particlar
 *                    time domain.
 *
 * \return ::canOK (zero) if success
 * \return \ref canERR_xxx (negative) if failure
 * 
 * \sa \ref page_code_snippets_examples
 * \sa kvTimeDomainDelete()
 */
kvStatus CANLIBAPI kvTimeDomainCreate (kvTimeDomain *domain);

/**
 * \ingroup TimeDomainHandling
 * 
 * This is a cleanup routine that deletes all members of a domain and then
 * deletes the domain itself.
 * 
 * \note A time domain is a set of channels with a common time base.
 *
 * \param[in] domain  An opaque variable set by kvTimeDomainCreate() that
 *                    identifies the domain to be deleted.
 * 
 * \return ::canOK (zero) if success
 * \return \ref canERR_xxx (negative) if failure
 * 
 * \sa \ref page_code_snippets_examples
 * \sa kvTimeDomainCreate()
 */
kvStatus CANLIBAPI kvTimeDomainDelete (kvTimeDomain domain);

/**
 * \ingroup TimeDomainHandling
 * 
 * This routine resets the time on all members of a time domain.
 * 
 * After a call to this routine timestamps from all channels with MagiSync&tm; 
 * running have no offset at all any longer. The same applies for channels that
 * reside on the same physical interface.
 *
 * \note A time domain is a set of channels with a common time base.
 *
 * \param[in] domain  An opaque variable set by kvTimeDomainCreate() that
 *                    identifies the domain to reset the time on.
 * 
 * \return ::canOK (zero) if success
 * \return \ref canERR_xxx (negative) if failure
 * 
 * \sa \ref page_code_snippets_examples
 * \sa kvTimeDomainCreate()
 */
kvStatus CANLIBAPI kvTimeDomainResetTime (kvTimeDomain domain);

/**
 * \ingroup TimeDomainHandling
 * 
 * This routine collects some data on a time domain.
 * 
 * \note A time domain is a set of channels with a common time base.
 *
 * \param[in]  domain  An opaque variable set by kvTimeDomainCreate() that
 *                     identifies the domain to add a handle to.
 * \param[out] data    A pointer to a ::kvTimeDomainData that is to be filled by
 *                     the function.
 * \param[in]  bufsiz  The size in bytes of the ::kvTimeDomainData struct.
 * 
 * \return ::canOK (zero) if success
 * \return \ref canERR_xxx (negative) if failure
 * 
 * \sa \ref page_code_snippets_examples
 * \sa kvTimeDomainCreate()
 */
kvStatus CANLIBAPI kvTimeDomainGetData (kvTimeDomain domain, 
                                        kvTimeDomainData *data, 
                                        size_t bufsiz);

/**
 * \ingroup TimeDomainHandling
 * 
 * This routine adds an open channel handle to a domain.
 * 
 * \note A time domain is a set of channels with a common time base.
 * 
 * \param[in] domain  An opaque variable set by kvTimeDomainCreate()
 *                    that identifies the domain to add a handle to. 
 * \param[in] hnd     A handle to an open channel.
 * 
 * \return ::canOK (zero) if success
 * \return \ref canERR_xxx (negative) if failure
 *
 * \sa \ref page_code_snippets_examples
 * \sa kvTimeDomainCreate(), kvTimeDomainRemoveHandle()
 */
kvStatus CANLIBAPI kvTimeDomainAddHandle(kvTimeDomain domain, 
                                         const CanHandle hnd);

/**
 * \ingroup TimeDomainHandling
 * 
 * This routine removes an open channel handle from a domain.
 * 
 * \note A time domain is a set of channels with a common time base.
 *
 * \param[in] domain  An opaque variable set by kvTimeDomainCreate()
 *                    that identifies the domain to remove a handle from.
 * \param[in] hnd     A handle to an open channel.
 * 
 * \return ::canOK (zero) if success
 * \return \ref canERR_xxx (negative) if failure
 * 
 * \sa kvTimeDomainCreate(), kvTimeDomainAddHandle()
 */
kvStatus CANLIBAPI kvTimeDomainRemoveHandle (kvTimeDomain domain, 
                                             const CanHandle hnd);

/**
 * \name kvCallback_t
 * \anchor kvCallback_t
 * kvCallback_t is used by the function kvSetNotifyCallback()
 *
 * The callback function is called with the following arguments:
 * \li hnd - the handle of the CAN channel where the event happened.
 * \li context - the context pointer you passed to kvSetNotifyCallback().
 * \li notifyEvent - one of the \ref canNOTIFY_xxx notification codes. 
 *
 * \note It is really the \ref canNOTIFY_xxx codes, and not the \ref
 *  canEVENT_xxx codes that the canSetNotify() API is using.)
 *
 * \param[in] hnd          An open handle to a CAN channel.
 * \param[in] context      Arbitrary user-defined context data which
 *                         is passed to the callback function.
 * \param[in] notifyEvent  One or more of the \ref canEVENT_xxx flags.
 * 
 */
typedef void (CANLIBAPI *kvCallback_t) (CanHandle hnd, void* context, unsigned int notifyEvent);

/**
 * \ingroup General
 * 
 * The kvSetNotifyCallback() function registers a callback function which is
 * called when certain events occur.
 *
 * You can register at most one callback function per handle at any time.
 *
 * To remove the callback, call kvSetNotifyCallback() with a \c NULL pointer in
 * the callback argument.
 *
 * \note The callback function is called in the context of a high-priority
 * thread created by CANLIB. You should take precaution not to do any time
 * consuming tasks in the callback.  You must also arrange the synchronization
 * between the callback and your other threads yourself.
 *
 * \param[in] hnd          An open handle to a CAN channel.
 * \param[in] callback     A pointer to a callback function of type 
 *                         \ref kvCallback_t
 * \param[in] context      A pointer to arbitrary user-defined context data which
 *                         is passed to the callback function.
 * \param[in] notifyFlags  One or more of the \ref canNOTIFY_xxx flags.
 * 
 * \return ::canOK (zero) if success
 * \return \ref canERR_xxx (negative) if failure
 * 
 * \sa canSetNotify()
 */
kvStatus CANLIBAPI kvSetNotifyCallback (const CanHandle hnd, 
                                        kvCallback_t callback, 
                                        void* context, 
                                        unsigned int notifyFlags);

/**
 * \name kvBUSTYPE_xxx
 * \anchor kvBUSTYPE_xxx
 *
 *  Bus types
 * @{
 */
#define kvBUSTYPE_NONE          0
#define kvBUSTYPE_PCI           1
#define kvBUSTYPE_PCMCIA        2
#define kvBUSTYPE_USB           3
#define kvBUSTYPE_WLAN          4
#define kvBUSTYPE_PCI_EXPRESS   5
#define kvBUSTYPE_ISA           6
#define kvBUSTYPE_VIRTUAL       7
#define kvBUSTYPE_PC104_PLUS    8
/** @} */

/**
 * \name Bus type groups
 * \anchor kvBUSTYPE_GROUP_xxx
 * @{
 */
#define kvBUSTYPE_GROUP_VIRTUAL  1 ///< ::kvBUSTYPE_VIRTUAL 
#define kvBUSTYPE_GROUP_LOCAL    2 ///< ::kvBUSTYPE_USB 
#define kvBUSTYPE_GROUP_REMOTE   3 ///< ::kvBUSTYPE_WLAN 
#define kvBUSTYPE_GROUP_INTERNAL 4 ///< ::kvBUSTYPE_PCI, ::kvBUSTYPE_PCMCIA, ... 
/** @} */

/**
 * \ingroup General
 * 
 * The kvGetSupportedInterfaceInfo function returns information about the
 * different supported hardware types in the installed version of CANLIB.
 * 
 * This function is used to enumerate all the supported hardware types in the
 * installed version of CANLIB. It does not return a complete list of all
 * supported devices, not does it return a list of the presently installed
 * hardware. The returned data indicates which device families are supported.
 *
 * For example, a returned set of data might be:
 * \li \a hwType = ::canHWTYPE_MEMORATOR_PRO
 * \li \a hwBusType = ::kvBUSTYPE_USB
 * \li \a hwName = \c "Kvaser Memorator Professional"
 *
 * This means that 
 * \li the presently installed version of CANLIB supports members in the Kvaser
 *     Memorator Pro family (e.g. the HS/HS and the HS/LS),
 * \li the members of the Kvaser Memorator Pro family are USB devices,
 * \li the members of the Kvaser Memorator Pro family use the
 *     ::canHWTYPE_MEMORATOR_PRO hardware type.
 *
 * The kvGetSupportedInterfaceInfo() function is intended to help
 * application designers build a bus-oriented display of the different
 * installed and/or supported Kvaser devices in the computer.
 *
 * \param[in] index       Use this parameter to enumerate the different supported
 *                        hardware types. Start with index = 0, and then call
 *                        kvGetSupportedInterfaceInfo again() with index =
 *                        1,2,3,... until the function returns an error code.
 * \param[out] hwName     A pointer to a buffer that will receive the name of the
 *                        hardware family, as a zero-terminated ASCII string.
 * \param[in]  nameLen    The length of the hwName buffer.
 * \param[out] hwType     Pointer to a 32-bit integer that will receive the
 *                        hardware type (one of the \ref canHWTYPE_xxx
 *                        constants.)
 * \param[out] hwBusType  Pointer to a 32-bit integer that will receive the bus
 *                        type (one of the \ref kvBUSTYPE_xxx constants.)
 * 
 * \return ::canOK (zero) if success
 * \return \ref canERR_xxx (negative) if failure
 * 
 * \sa canBusOn(), canResetBus()
 */
kvStatus CANLIBAPI kvGetSupportedInterfaceInfo (int index, 
                                                char *hwName, 
                                                size_t nameLen, 
                                                int *hwType, 
                                                int *hwBusType);

/**
 * \ingroup General
 * 
 * The kvReadTimer reads the hardware clock on the specified device and returns
 * the value.
 *
 * When the call to kvReadTimer() returns, the time value is already
 * obsolete. The time required for the device firmware, any intermediary buses
 * (like USB,) and the operating system to return the time value is not
 * defined.
 *
 * This call should be used instead of canReadTimer() because it can return an
 * error code if it fails.
 * 
 * \param[in]  hnd   An open handle to a CAN channel.
 * \param[out] time  A pointer to a 32-bit unsigned integer that will receive
 *                   the time value.
 * 
 * \return ::canOK (zero) if success
 * \return \ref canERR_xxx (negative) if failure
 * 
 * \sa canReadTimer(), kvReadTimer64()
 */
kvStatus CANLIBAPI kvReadTimer (const CanHandle hnd, unsigned int *time);

#if defined(KVINT64)
/**
 * \ingroup General
 * 
 * The kvReadTimer64 reads the hardware clock on the specified device and
 * returns the value.
 *
 * When the call to kvReadTimer64() returns, the time value is already
 * obsolete. The time required for the device firmware, any intermediary buses
 * (like USB,) and the operating system to return the time value is not
 * defined.
 *
 * This call should be used instead of canReadTimer() because it can return an
 * error code if it fails.
 * 
 * \param[in]  hnd   An open handle to a CAN channel.
 * \param[out] time  A pointer to a 64-bit signed integer that will receive the
 *                   time value.
 * 
 * \return ::canOK (zero) if success
 * \return \ref canERR_xxx (negative) if failure
 * 
 * \sa kvReadTimer(), canReadTimer()
 */
kvStatus CANLIBAPI kvReadTimer64 (const CanHandle hnd, KVINT64 *time);
#endif

/**
 * \ingroup General
 * 
 * Undocumented
 * 
 * \param[in]  hnd
 * \param[in]  userNumber
 * \param[in]  itemNumber
 * \param[out] data
 * \param[in]  bufsiz
 * 
 * \return ::canOK (zero) if success
 * \return \ref canERR_xxx (negative) if failure
 * 
 * \todo Document kvReadDeviceCustomerData()
 */
kvStatus CANLIBAPI kvReadDeviceCustomerData (const CanHandle hnd, 
                                             int userNumber, 
                                             int itemNumber, 
                                             void *data, 
                                             size_t bufsiz);

//�����������������������������������������������������������������������������
//
// APIs for t-script
// 

/**
 * \ingroup tScript
 * \name kvENVVAR_TYPE_xxx
 * \anchor kvENVVAR_TYPE_xxx
 *
 * These defines are used in kvScriptEnvvarOpen()
 *  @{
 */
  /**
   * This define is used in  kvScriptEnvvarOpen()
   *
   * It defines the type of the envvar as \c int
   */  
#define kvENVVAR_TYPE_INT       1

  /**
   * This define is used in  kvScriptEnvvarOpen()
   *
   * It defines the type of the envvar as \c float
   */  
#define kvENVVAR_TYPE_FLOAT     2

  /**
   * This define is used in  kvScriptEnvvarOpen()
   *
   * It defines the type of the envvar as \c string
   */  
#define kvENVVAR_TYPE_STRING    3
 /** @} */

/**
 * \ingroup tScript
 * \name kvEVENT_xxx
 * \anchor kvEVENT_xxx
 *
 * These defines are used in kvScriptSendEvent()
 *  @{
 */

  /**
   * This define is used in  kvScriptSendEvent()
   *
   * It defines an event of type "key pressed"
   */  
#define kvEVENT_TYPE_KEY        1
 /** @} */

  /**
   * \ingroup tScript
   *
   * A handle to a t-script envvar.
   * Returned by the function kvScriptEnvvarOpen().
   */
typedef __int64 kvEnvHandle;

/**
 * \ingroup tScript
 * 
 * The kvScriptStart() function starts a loaded script.
 * 
 * \param[in] hnd     An open handle to a CAN channel. 
 * \param[in] slotNo  The slot with the loaded script we want to start. 
 * 
 * \return ::canOK (zero) if success
 * \return \ref canERR_xxx (negative) if failure
 * 
 * \sa \ref page_code_snippets_examples
 * \sa kvScriptLoadFile(), kvScriptStop()
 */
kvStatus CANLIBAPI kvScriptStart (const CanHandle hnd, int slotNo); 

/**
 * Script stop modes. Used by kvScriptStop().
 * \anchor kvSCRIPT_STOP_xxx
 * \name kvSCRIPT_STOP_xxx
 * @{
 */ 
#define kvSCRIPT_STOP_NORMAL         0 /**< Stop a running script */
#define kvSCRIPT_STOP_FORCED        -9 /**< Request termination of a hanged script */
/** @} */


/**
 * \ingroup tSCript
 * 
 *  The kvScriptStop() function stops a started script.
 * 
 * \param[in] hnd     An open handle to a CAN channel.
 * \param[in] slotNo  The slot with the loaded and running script we want to stop.
 * \param[in] mode    Stop mode of type \ref kvSCRIPT_STOP_xxx
 *
 * \return ::canOK (zero) if success
 * \return \ref canERR_xxx (negative) if failure
 * 
 * \sa \ref page_code_snippets_examples
 * \sa kvScriptLoadFile(), kvScriptStart()
 */
kvStatus CANLIBAPI kvScriptStop (const CanHandle hnd, int slotNo, int mode); 

/**
 * \ingroup tSCript
 * 
 *  The kvScriptUnload() function unloads a stopped script.
 * 
 * \param[in] hnd     An open handle to a CAN channel.
 * \param[in] slotNo  The slot with the loaded and stopped script we want to unload.
 *
 * \return ::canOK (zero) if success
 * \return \ref canERR_xxx (negative) if failure
 * 
 * \sa \ref page_code_snippets_examples
 * \sa kvScriptLoadFile(), kvScriptStop()
 */
kvStatus CANLIBAPI kvScriptUnload (const CanHandle hnd, int slotNo); 

/**
 * \ingroup tScript
 * 
 * The kvScriptSendEvent() function sends an event of a type, and an event
 * number and associated data to a script running in a specific slot.
 * 
 * \param[in] hnd        An open handle to a CAN channel.
 * \param[in] slotNo     The slot where the script was loaded and is running.
 * \param[in] eventType  The event to send, of type \ref kvEVENT_xxx
 * \param[in] eventNo    The event's number.
 * \param[in] data       The event's data.
 * 
 * \return ::canOK (zero) if success
 * \return \ref canERR_xxx (negative) if failure
 * 
 * \sa \ref page_code_snippets_examples
 */
kvStatus CANLIBAPI kvScriptSendEvent (const CanHandle hnd, 
                                      int slotNo, 
                                      int eventType, 
                                      int eventNo, 
                                      unsigned int data);

/**
 * \ingroup tScript
 * 
 * The kvScriptEnvvarOpen() opens an existing envvar and returns a handle to it.
 * 
 * \param[in]  hnd         An open handle to a CAN channel.
 * \param[in]  envvarName  The envvar's name; a pointer to a \c NULL terminated
 *                         array of chars.
 * \param[out] envvarType  A pointer to a 32-bit integer that will receive the
 *                         \ref kvENVVAR_TYPE_xxx type.
 * \param[out] envvarSize  A pointer to a 32-bit integer that will receive the
 *                         size of the envvar in bytes.
 * 
 * \return A ::kvEnvHandle handle (positive) to an envvar if success
 * \return \ref canERR_xxx (negative) if failure
 * 
 * \sa \ref page_code_snippets_examples
 * \sa kvScriptEnvvarClose()
 */
kvEnvHandle CANLIBAPI kvScriptEnvvarOpen (const CanHandle hnd, 
                                          char* envvarName, 
                                          int *envvarType, 
                                          int *envvarSize); // returns scriptHandle

/**
 * \ingroup tScript
 * 
 * The kvScriptEnvvarClose() function closes an open envvar.
 * 
 * \param[in] eHnd  An open handle to an envvar.
 * 
 * \return ::canOK (zero) if success
 * \return \ref canERR_xxx (negative) if failure
 * 
 * \sa kvScriptEnvvarOpen() 
 */
kvStatus CANLIBAPI kvScriptEnvvarClose (kvEnvHandle eHnd);

/**
 * \ingroup tScript
 * 
 * The kvScriptEnvvarSetInt() sets the value of an \c int envvar.
 * 
 * \param[in] eHnd  An open handle to an envvar.
 * \param[in] val   The new value.
 * 
 * \return ::canOK (zero) if success
 * \return \ref canERR_xxx (negative) if failure
 * 
 * \sa \ref page_code_snippets_examples
 * \sa kvScriptEnvvarOpen(), kvScriptEnvvarGetInt(), kvScriptEnvvarSetFloat(),
 * kvScriptEnvvarSetData()
 */
kvStatus CANLIBAPI kvScriptEnvvarSetInt (kvEnvHandle eHnd, int val);

/**
 * \ingroup tScript
 * 
 * The kvScriptEnvvarGetInt() function retrieves the value of an \c int envvar.
 * 
 * \param[in]  eHnd An open handle to an envvar.
 * \param[out] val  The current value.
 * 
 * \return ::canOK (zero) if success
 * \return \ref canERR_xxx (negative) if failure
 * 
 * \sa \ref page_code_snippets_examples
 * \sa kvScriptEnvvarOpen(), kvScriptEnvvarSetInt(), kvScriptEnvvarGetFloat(),
 * kvScriptEnvvarGetData()
 *
 */
kvStatus CANLIBAPI kvScriptEnvvarGetInt (kvEnvHandle eHnd, int *val);

/**
 * \ingroup tScript
 * 
 * The kvScriptEnvvarSetFloat() sets the value of a \c float envvar.
 * 
 * \param[in] eHnd  An open handle to an envvar.
 * \param[in] val   The new value.
 * 
 * \return ::canOK (zero) if success
 * \return \ref canERR_xxx (negative) if failure
 * 
 * \sa \ref page_code_snippets_examples
 * \sa kvScriptEnvvarOpen(), kvScriptEnvvarGetFloat(), kvScriptEnvvarSetInt(),
 * kvScriptEnvvarSetData()
 */
kvStatus CANLIBAPI kvScriptEnvvarSetFloat (kvEnvHandle eHnd, float val);

/**
 * \ingroup tScript
 * 
 * The kvScriptEnvvarGetFloat() function retrieves the value of a \c float envvar.
 * 
 * \param[in]  eHnd  An open handle to an envvar.
 * \param[out] val   A pointer to a \c float where the retrieved result should be
 *                   stored.
 * 
 * \return ::canOK (zero) if success
 * \return \ref canERR_xxx (negative) if failure
 * 
 * \sa \ref page_code_snippets_examples
 * \sa kvScriptEnvvarOpen(), kvScriptEnvvarSetFloat(), kvScriptEnvvarGetInt(),
 * kvScriptEnvvarGetData()
 */
kvStatus CANLIBAPI kvScriptEnvvarGetFloat (kvEnvHandle eHnd, float *val);

/**
 * \ingroup tScript
 * 
 * The kvScriptEnvvarSetData() function sets a range of data bytes in an envvar.
 * 
 * \param[in] eHnd         An open handle to an envvar.
 * \param[in] buf          A pointer to a data area with the new values.
 * \param[in] start_index  The start index of the envvar's data range that we
 *                         want to update.
 * \param[in] data_len     The length in bytes of the envvar's data range that 
 *                         we want to update.
 * 
 * \return ::canOK (zero) if success
 * \return \ref canERR_xxx (negative) if failure
 * 
 * \sa \ref page_code_snippets_examples
 * \sa kvScriptEnvvarOpen(), kvScriptEnvvarGetData(), kvScriptEnvvarSetInt(),
 * kvScriptEnvvarSetFloat()
 */
kvStatus CANLIBAPI kvScriptEnvvarSetData (kvEnvHandle eHnd, 
                                          void *buf, 
                                          int start_index, 
                                          int data_len);

/**
 * \ingroup tScript
 * 
 * The kvScriptEnvvarGetData() function retrieves a range of data bytes from an envvar.
 * 
 * \param[in]  eHnd         An open handle to an envvar.
 * \param[out] buf          A pointer to a data area where the retrieved data 
 *                          range should be stored.
 * \param[in]  start_index  The start index of the data range.
 * \param[in]  data_len     The length in bytes of the data range.
 * 
 * \return ::canOK (zero) if success
 * \return \ref canERR_xxx (negative) if failure
 * 
 * \sa \ref page_code_snippets_examples
 * \sa kvScriptEnvvarOpen(), kvScriptEnvvarSetData(), kvScriptEnvvarGetInt(),
 * kvScriptEnvvarGetFloat()
 */
kvStatus CANLIBAPI kvScriptEnvvarGetData (kvEnvHandle eHnd, 
                                          void *buf, 
                                          int start_index, 
                                          int data_len);

/**
 * \ingroup tScript
 * 
 * The kvScriptLoadFileOnDevice() function loads a compiled script file (.txe)
 * stored on the device (SD card) into a script slot on the device.
 * 
 * \param[in] hnd        An open handle to a CAN channel.
 * \param[in] slotNo     The slot where to load the script.
 * \param[in] localFile  The script file name; a pointer to a \c NULL terminated
 *                       array of chars.
 * 
 * \return ::canOK (zero) if success
 * \return \ref canERR_xxx (negative) if failure
 * 
 * \sa \ref page_code_snippets_examples
 * \sa kvScriptLoadFile(), kvFileCopyToDevice(), kvScriptStart(),
 * kvScriptStop()
 */
kvStatus CANLIBAPI kvScriptLoadFileOnDevice (const CanHandle hnd, 
                                             int slotNo, 
                                             char *localFile);

/**
 * \ingroup tScript
 * 
 * The kvScriptLoadFile() function loads a compiled script file (.txe) stored
 * on the host (PC) into a script slot on the device.
 * 
 * \param[in] hnd           An open handle to a CAN channel.
 * \param[in] slotNo        The slot where to load the script.
 * \param[in] filePathOnPC  The script file name; a pointer to a \c NULL
 *                          terminated array of chars.
 * 
 * \return ::canOK (zero) if success
 * \return \ref canERR_xxx (negative) if failure
 * 
 * \sa \ref page_code_snippets_examples
 * \sa kvScriptLoadFileOnDevice(), kvFileCopyToDevice(), kvScriptStart(),
 * kvScriptStop()
 */
kvStatus CANLIBAPI kvScriptLoadFile (const CanHandle hnd, 
                                     int slotNo, 
                                     char *filePathOnPC);


   
 
/**
 * \ingroup General
 * \name kvSCRIPT_REQUEST_TEXT_xxx
 * \anchor kvSCRIPT_REQUEST_TEXT_xxx
 *
 * These defines are used in kvScriptRequestText() for printf message subscribe/unsubscribe. 
 *
 * @{
 */
 
/**  
 *  Cancel subscription of printf messages from script slots. 
 */ 
#define kvSCRIPT_REQUEST_TEXT_UNSUBSCRIBE  1

/**
 *  Subscribe to printf messages from script slots. 
 */ 
#define kvSCRIPT_REQUEST_TEXT_SUBSCRIBE    2            
  
/**
 *  Select all script slots. 
 */   
#define kvSCRIPT_REQUEST_TEXT_ALL_SLOTS    255    
 
/** @} */     
 
  
/**
 * \ingroup tScript
 * 
 * The kvScriptRequestText() Sets up a printf subscription to a 
 * selected script slot.
 * Read the printf messages with kvScriptGetText().
 * 
 * \param[in] hnd          An open handle to a CAN channel.
 * \param[in] slot         The slot to subscribe to.
 * \param[in] request      Subscription request i.e. \ref kvSCRIPT_REQUEST_TEXT_xxx.
 * 
 * \return ::canOK (zero) if success
 * \return \ref canERR_xxx (negative) if failure
 *
 */                                  
kvStatus CANLIBAPI kvScriptRequestText(const CanHandle hnd, 
                                       unsigned int slot, 
                                       unsigned int request);
       


/**
 * \ingroup tScript
 * 
 * The kvScriptGetText() Reads a printf from a subscribed script slot. 
 * Set up a subscription with kvScriptRequestText(). 
 * 
 * \param[in] hnd           An open handle to a CAN channel.
 * \param[out] slot         The slot where the printf originated.
 * \param[out] time         The printf timestamp.
 * \param[out] flags        Printf flags. A combination of \ref canSTAT_xxx flags. 
 * \param[out] buf          Buffer to hold the printf string. 
 * \param[in] bufsize       Size of the buffer.
 * 
 * \return ::canOK (zero) if success
 * \return \ref canERR_xxx (negative) if failure
 * 
 */
kvStatus CANLIBAPI kvScriptGetText(const CanHandle hnd,
                                        int  *slot, 
                                        unsigned long *time,
                                        unsigned int  *flags,
                                        char *buf, 
                                        size_t bufsize
                                        );                                   

                                      
                                      
                                      
/**
 * \ingroup tScript
 * 
 * The kvFileCopyToDevice() function copies an arbitrary file from the host to
 * the device.
 * 
 * \param[in] hnd             An open handle to a CAN channel.
 * \param[in] hostFileName    The host file name; a pointer to a \c NULL terminated 
 *                            array of chars.
 * \param[in] deviceFileName  The target device file name; a pointer to a \c NULL
 *                            terminated array of chars.
 * 
 * \return ::canOK (zero) if success
 * \return \ref canERR_xxx (negative) if failure
 * 
 * \sa kvFileCopyFromDevice(), kvFileDelete()
 */
kvStatus CANLIBAPI kvFileCopyToDevice (const CanHandle hnd, 
                                       char *hostFileName, 
                                       char *deviceFileName);

/**
 * \ingroup tScript
 * 
 * The kvFileCopyFromDevice() function copies an arbitrary file from the device
 * to the host.
 * 
 * \param[in] hnd             An open handle to a CAN channel.
 * \param[in] deviceFileName  The device file name; a pointer to a \c NULL
 *                            terminated array of chars.
 * \param[in] hostFileName    The target host file name; a pointer to a \c NULL terminated 
 *                            array of chars.
 * 
 * \return ::canOK (zero) if success
 * \return \ref canERR_xxx (negative) if failure
 * 
 * \sa kvFileCopyToDevice()
 */
kvStatus CANLIBAPI kvFileCopyFromDevice (const CanHandle hnd, 
                                         char *deviceFileName, 
                                         char *hostFileName);

/**
 * \ingroup tScript
 * 
 * The kvFileDelete() function deletes a file on the device.
 * 
 * \note Deleting system files is not recommended.
 *
 * \param[in] hnd             An open handle to a CAN channel.
 * \param[in] deviceFileName  The file on the device to delete; a pointer 
 *                            to a \c NULL terminated array of chars.
 * 
 * \return ::canOK (zero) if success
 * \return \ref canERR_xxx (negative) if failure
 * 
 * \sa kvFileCopyToDevice()
 */
kvStatus CANLIBAPI kvFileDelete (const CanHandle hnd, char *deviceFileName);

/**
 * \ingroup tScript
 * 
 * The kvFileGetName() function returns the name of the file with
 *   number \a fileNo.
 * 
 * \param[in]  hnd      An open handle to a CAN channel.
 * \param[in]  fileNo   The number of the file.
 * \param[out] name     A buffer that will contain the name of the 
 *                      file. The name is a zero-terminated ASCII string.
 * \param[in]  namelen  The length, in bytes, of the \a name buffer.
 * 
 * \return ::canOK (zero) if success
 * \return \ref canERR_xxx (negative) if failure
 * 
 * \sa kvFileGetCount(), kvFileGetSystemData()
 */
kvStatus CANLIBAPI kvFileGetName (const CanHandle hnd, 
                                  int fileNo, 
                                  char *name, 
                                  int namelen);

/**
 * \ingroup tScript
 * 
 * The kvFileGetCount() function returns the number of files.
 * 
 * \param[in]  hnd    An open handle to a CAN channel.
 * \param[out] count  A pointer to a 32-bit integer that will receive 
 *                    the file count.
 * 
 * \return ::canOK (zero) if success
 * \return \ref canERR_xxx (negative) if failure
 * 
 * \sa kvFileGetName(), kvFileGetSystemData()
 */
kvStatus CANLIBAPI kvFileGetCount (const CanHandle hnd, int *count);

/**
 * \ingroup tScript
 * 
 * The kvFileGetSystemData() function is used for reading disk parameters,
 * e.g. size, max number of (user) files, etc.
 * 
 * \note Not yet implemented
 *
 * \param[in]  hnd       An open handle to a CAN channel.
 * \param[in]  itemCode  The item we want information on.
 * \param[out] result    A pointer to a 32-bit integer that will 
 *                       receive the result.
 * 
 * \return ::canOK (zero) if success
 * \return \ref canERR_xxx (negative) if failure
 * 
 */
kvStatus CANLIBAPI kvFileGetSystemData (const CanHandle hnd, 
                                        int itemCode, 
                                        int *result);

/**
 * \ingroup General
 * \anchor kvDEVICE_MODE_xxx
 * \name kvDEVICE_MODE_xxx
 *
 * These defines are used in kvDeviceSetMode() and kvDeviceGetMode()
 *
 * \note The mode is device specific, which means that not all modes are
 * implemented in all products.
 *
 *  @{
 */
   /**
   * This define is used in kvDeviceSetMode() and kvDeviceGetMode()
   *
   * Device is running or should be running in interface mode.
   */
#define kvDEVICE_MODE_INTERFACE      0x00
  /**
   * This define is used in kvDeviceSetMode() and kvDeviceGetMode()
   *
   * Device is running or should be running in logger mode.
   */
#define kvDEVICE_MODE_LOGGER      0x01
 /** @} */


/**
 * \ingroup General
 * 
 * The kvDeviceGetMode() sets the mode.
 * 
 * \note The mode is device specific, which means that not all modes are
 * implemented in all products.
 *
 * \param[in] hnd   An open handle to a CAN channel.
 * \param[in] mode  One of the \ref kvDEVICE_MODE_xxx constants, 
 *                  defining which mode to use.
 * 
 * \return ::canOK (zero) if success
 * \return \ref canERR_xxx (negative) if failure
 * 
 * \sa kvDeviceGetMode()
 */
kvStatus CANLIBAPI kvDeviceSetMode (const CanHandle hnd, int mode);

/**
 * \ingroup General
 * 
 * The kvDeviceGetMode reads the current device's specific mode.
 * 
 * \note The mode is device specific, which means that not all modes are
 * implemented in all products.
 *
 * \param[in]  hnd     An open handle to a CAN channel.
 * \param[out] result  A pointer to a 32-bit integer that will receive the 
 *                     \ref kvDEVICE_MODE_xxx value.
 * 
 * \return ::canOK (zero) if success
 * \return \ref canERR_xxx (negative) if failure
 * 
 * \sa kvDeviceSetMode() 
 */
kvStatus CANLIBAPI kvDeviceGetMode (const CanHandle hnd, int *result);


/**
 * \ingroup General
 * 
 * This function sends an active ping to a device. The ping time can later be
 * retrieved using kvPingGetLatest().
 * 
 * \param[in]  hnd          A handle to an open circuit.
 * \param[out] requestTime  Time of request in microseconds. Used for matching answer to request.
 * 
 * \return ::canOK (zero) if success
 * \return \ref canERR_xxx (negative) if failure
 */
kvStatus CANLIBAPI kvPingRequest (const CanHandle hnd, 
                                  unsigned int *requestTime);

/**
 * \ingroup General
 * 
 * This retrieves the latest ping time issued by an earlier call to kvPingRequest()
 * 
 * \param[in]  hnd         A handle to an open circuit.
 * \param[out] requestTime Time of request in microseconds. Used for matching answer to request.
 * \param[out] pingTime    Latest value of ping time in milliseconds.
 * 
 * \return ::canOK (zero) if success
 * \return \ref canERR_xxx (negative) if failure
 */
 kvStatus CANLIBAPI kvPingGetLatest (const CanHandle hnd, 
                                     unsigned int *requestTime, 
                                     unsigned int *pingTime);

//�����������������������������������������������������������������������������


//
// The following functions are not yet implemented. Do not use them.
//
#if defined(_CANEVT_H_)
canStatus CANLIBAPI canReadEvent (const CanHandle hnd, CanEvent *event);
#endif
void CANLIBAPI canSetDebug(int d);

canStatus CANLIBAPI canSetNotifyEx (const CanHandle hnd, 
                                    HANDLE event, 
                                    unsigned int flags);

canStatus CANLIBAPI canSetTimer (const CanHandle hnd, 
                                 DWORD interval, 
                                 DWORD flags);
#define canTIMER_CYCLIC             0x01
#define canTIMER_EXPENSIVE          0x02
int CANLIBAPI canSplitHandle (CanHandle hnd, int channel);
int CANLIBAPI canOpenMultiple (DWORD bitmask, int flags);
#endif

#ifdef __cplusplus
}
#endif

#endif
