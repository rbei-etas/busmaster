/**
 * \section LICENSE
 *                   Copyright 2003-2011 by KVASER AB, SWEDEN
 *
 *                        WWW: http://www.kvaser.com
 *
 * This software is furnished under a license and may be used and copied
 * only in accordance with the terms of such license.
 *
 * \section DESCRIPTION
 *
 *   Definitions for the LINlib API.
 *
 * \file linlib.h
 * \author Kvaser AB
 */
#ifndef __LINLIB_H
#define __LINLIB_H

/**
 * \defgroup LIN LIN
 * @{
 */

 /**
  * LIN handle
  */
typedef int LinHandle;

/**
 * Invalid LIN handle
 */
#define linINVALID_HANDLE      ((LinHandle)(-1))

#if !WIN32
#  define CompilerAssert(exp) extern char _CompilerAssert[(exp) ? 1 : -1]
#endif


/**
 * \name Return codes from the LINlib functions
 * \anchor linERR_xxx
 * @{
 */
typedef enum {
  linOK                  = 0,      ///< OK - no error
  linERR_NOMSG           = -1,     ///< No messages available
  linERR_NOTRUNNING      = -3,
  linERR_RUNNING         = -4,
  linERR_MASTERONLY      = -5,
  linERR_SLAVEONLY       = -6,
  linERR_PARAM           = -7,     ///< Error in parameter 
  linERR_NOTFOUND        = -8,     ///< Specified hardware not found. This error is reported when the LIN transceiver isn't powered up
  linERR_NOMEM           = -9,     ///< Out of memory
  linERR_NOCHANNELS      = -10,    ///< No channels avaliable
  linERR_TIMEOUT         = -11,    ///< Timeout occurred
  linERR_NOTINITIALIZED  = -12,    ///< Library not initialized
  linERR_NOHANDLES       = -13,    ///< Can't get handle
  linERR_INVHANDLE       = -14,    ///< Handle is invalid
  linERR_CANERROR        = -15,
  linERR_ERRRESP         = -16,    ///< There was an error response from the LIN interface
  linERR_WRONGRESP       = -17,    ///< The LIN interface response wasn't the expected one
  linERR_DRIVER          = -18,    ///< CAN driver type not supported
  linERR_DRIVERFAILED    = -19,    ///< DeviceIOControl failed; use the Win32 GetLastError API to get the real (WIn32) error code.
  linERR_NOCARD          = -20,    ///< The card was removed or not inserted
  linERR_LICENSE         = -21,    ///< The license is not valid
  linERR_INTERNAL        = -22,    ///< Internal error in the driver
  linERR_NO_ACCESS       = -23,    ///< Access denied
  linERR_VERSION         = -24     ///< Function not supported in this version
} LinStatus;
/** @} */


// sizeof(LinMessageInfo) should be 68 for correct alignment
#if WIN32
#include <pshpack1.h>
#endif

/**
 * In certain LIN bus API calls, the following structure is used to provide more 
 * information about the LIN messages.
 *
 * \note The precision of the timing data given in us (microseconds) can be less 
 * than one microsecond; for low bitrates the lowest bits might always be zero.
 * 
 * \note The min and max values listed inside [] of the message timing values 
 * can be calculated from the LIN specification by using the shortest (0 bytes) 
 * or longest (8 bytes) messages at the lowest or highest allowed bitrate.
 *
 * \note The LIN interface will accept messages that are a bit out-of-bounds as 
 * well.
 */
typedef struct {
  /**
   * Kvaser DRV Lin timestamp - Timestamp in milliseconds of the falling edge of
   * the synch break of the message. Uses the canlib CAN timer.
   *
   * Kvaser LIN Leaf timestamp - Timestamp in milliseconds of the falling edge 
   * of the synch break of the message. Uses the canlib CAN timer. 
   *
   * \note All Kvaser Leaf with Kvaser MagiSync&tm; are synchronized 
   * (also with CAN channels).
   */
  unsigned long timestamp;

  /** 
   * Length of the synch break in microseconds.
   * [650 .. 13000],  [400 .. 8000] for a wakeup signal.
   */
  unsigned long synchBreakLength;

  /**
   * The total frame length in microseconds; from the synch break to the
   * end of the crc. [2200 .. 173600]
   */
  unsigned long frameLength;

  /**
   * The bitrate of the message in bits per seconds.
   * Range [1000 .. 20000] (plus some margin)
   */
  unsigned long bitrate;

  /**
   * The checksum as read from the LIN bus. Might not
   * match the data in case of ::LIN_CSUM_ERROR.
   */
  unsigned char checkSum;

  /**
   * The id with parity of the message as read from the
   * LIN bus. Might be invalid in case of ::LIN_PARITY_ERROR.
   */
  unsigned char idPar;

  unsigned short z; ///< Dummy for alignment

  /**
   * Time in microseconds of the falling edges in the
   * synch byte relative the falling edge of the start bit.
   */
  unsigned long synchEdgeTime[4];

  /**
   * Start time in microseconds of each data byte. In case of 8-byte messages, 
   * the crc time isn't included (but can be deduced from frameLength).
   */
   unsigned long byteTime[8];
} LinMessageInfo;

#if WIN32
#include <poppack.h>
#endif


/**
 * \name LIN message flags
 * \anchor LIN_xxx
 * The following flags can be returned from linReadMessage() and 
 * linReadMessageWait().
 *
 * @{
 */
#define LIN_TX              1   ///< The message was something we transmitted on the bus
#define LIN_RX              2   ///< The message was something we received from the bus
#define LIN_WAKEUP_FRAME    4   ///< A wake up frame was received. Id/msg/dlc are undefined
#define LIN_NODATA          8   ///< No data, only a header
#define LIN_CSUM_ERROR      16  ///< Checksum error
#define LIN_PARITY_ERROR    32  ///< ID parity error
#define LIN_SYNCH_ERROR     64  ///< A synch error
#define LIN_BIT_ERROR       128 ///< Bit error when transmitting
/** @} */

/**
 * Macro for determining if a flag field contains a LIN error flag bit.
 */
#define LINERROR(f) ((f) & (LIN_NODATA | LIN_CSUM_ERROR | LIN_SYNCH_ERROR | LIN_BIT_ERROR))


// Define LINLIBAPI unless it's done already.
// (linlib.c provides its own definition of LINLIBAPI before including this file.)
//
#if WIN32
#ifndef LINLIBAPI
#   if defined(__BORLANDC__)
#      define LINLIBAPI __stdcall
#   elif defined(_MSC_VER) || defined(__MWERKS__) || defined(__GNUC__)
#      define LINLIBAPI __stdcall
#   endif
#endif
#else
#define LINLIBAPI
#endif

#ifdef __cplusplus
extern "C" {
#endif

/** 
 * This function initializes the LIN library and must be called before any other 
 * LIN function is called. If this function isn't called, subsequent calls to 
 * the other LIN functions will return ::linERR_NOTINITIALIZED.
 *
 * \sa linOpenChannel()
 */
void LINLIBAPI linInitializeLibrary(void);


/** 
 * Retrieves the transceiver information for a CAN channel. The application 
 * typically uses this call to find out whether a particular CAN channel has a 
 * LIN interface connected to it.
 * For a Kvaser LIN Leaf it retrieves the transceiver type and device information.
 *
 * This function call will open the CAN channel, but no CAN messages are 
 * transmitted on it. In other words, it's risk-free to use even if no LIN 
 * interface is connected, or if the channel is connected to a CAN system. 
 *
 * \note Attempts to use the channel for LIN communication will be meaningful 
 * only if linGetTransceiverData() stores ::canTRANSCEIVER_TYPE_LIN in \a ttype. 
 *
 * \note A LIN interface need not be powered for this call to succeed. 
 *
 * \note The information may not always be accurate. Especially after changing 
 * transceiver on a running LAPcan card, you should go on bus and off bus again 
 * to be sure the transceiver information is updated. 
 *
 * \param[in]  channel The number of a CAN channel for which the transceiver 
 *                     data will be retrieved. 
 * \param[out] eanNo   A pointer to an array of 8 bytes where the EAN number of 
 *                     the LIN interface will be stored. 
 * \param[out] serNo   A pointer to an array of 8 bytes where the serial number 
 *                     of the LIN interface will be stored. 
 * \param[out] ttype   A pointer to an integer where the transceiver type will 
 *                     be stored.
 *
 * \return ::linOK (zero) if success
 * \return \ref linERR_xxx (negative) if failure
 *
 * \sa linGetFirmwareVersion()
 */ 
LinStatus LINLIBAPI linGetTransceiverData(int channel, unsigned char eanNo[8],
                                          unsigned char serNo[8], int *ttype);

/** 
 * Open a channel to a LIN interface.
 *
 * \note For DRV Lin: The cable must be powered and connected to a LAPcan channel.
 *
 * \note For Kvaser LIN Leaf: The Leaf must be powered from the LIN side.
 *
 * \param[in] channel  The number of the channel. Channel numbering is hardware
 *                     dependent. 
 *                     This is the same channel number as used by canOpenChannel().
 * \param[in] flags    Either one of the following values: ::LIN_MASTER or 
 *                     ::LIN_SLAVE. 
 *
 * \return If the call succeeds, a handle to the opened channel is returned. 
 * The handle is an integer greater than or equal to zero.
 *
 * \return If the call fails, the return value is a negative integer indicating 
 * an error code. See \ref linERR_xxx for a list of possible error codes.
 *
 * \sa linClose()
 */
LinHandle LINLIBAPI linOpenChannel(int channel, int flags);


/**
 * \name Flags for linOpenChannel()
 * @{
 */
#define LIN_MASTER 1 ///< The LIN interface will be a LIN master
#define LIN_SLAVE 2  ///< The LIN interface will be a LIN slave
/** @} */


/** 
 * Closes an open handle to a LIN channel. The handle becomes invalid and can 
 * not be used in subsequent calls to the LIN functions.
 *
 * \param[in] h        A handle to an open LIN channel.
 *
 * \return ::linOK (zero) if success
 * \return \ref linERR_xxx (negative) if failure
 *
 * \sa linOpenChannel()
 */
LinStatus LINLIBAPI linClose(LinHandle h);

/**
 * This function retrieves the firmware version from the LIN interface.
 *
 * 
 * \note The version numbers aren't valid until linBusOn() has been called.
 *
 * \note The firmware in the LIN interface is divided into two parts, the boot 
 * code and the application. The boot code is used only when reprogramming 
 * (reflashing) the LIN interface. The application handles all LIN communication.
 *
 * \note Version numbers are, since the precambric era, divided into a major 
 * version number, a minor version number and a build number. These are usually 
 * written like, for example, 3.2.12. Here the major number is 3, the minor 
 * number 2 and the build number 12.
 *
 * \param[in]  h             A handle to an open LIN channel.
 * \param[out] bootVerMajor  A pointer to a byte where the major version number 
 *                           of the boot code is stored. 
 * \param[out] bootVerMinor  A pointer to a byte where the minor version number 
 *                           of the boot code is stored. 
 * \param[out] bootVerBuild  A pointer to a byte where the build number of the 
 *                           boot code is stored. 
 * \param[out] appVerMajor   A pointer to a byte where the major version number 
 *                           of the application code is stored.
 * \param[out] appVerMinor   A pointer to a byte where the minor version number 
 *                           of the application code is stored. 
 * \param[out] appVerBuild   A pointer to a byte where the build number of the application is stored. 
 *
 * \return ::linOK (zero) if success
 * \return \ref linERR_xxx (negative) if failure
 */
LinStatus LINLIBAPI linGetFirmwareVersion(LinHandle h,
                                          unsigned char *bootVerMajor,
                                          unsigned char *bootVerMinor,
                                          unsigned char *bootVerBuild,
                                          unsigned char *appVerMajor,
                                          unsigned char *appVerMinor,
                                          unsigned char *appVerBuild);

/** 
 * This function sets the bit rate for a master, or the initial bit rate for a 
 * slave. The LIN interface should not be on-bus when this function is called.
 *
 * \note The LIN Interface should not be on bus.
 *
 * \note Supported bit rates are 1000 - 20000 bits per second.
 *
 * \param[in] h   A handle to an open LIN channel.
 * \param[in] bps Bit rate in bits per second.
 *
 * \return ::linOK (zero) if success
 * \return \ref linERR_xxx (negative) if failure
 */
LinStatus LINLIBAPI linSetBitrate(LinHandle h, unsigned int bps);

/** 
 * This function activates the LIN interface.
 *
 * \note It will be reset, so any setups done earlier will be lost.
 *
 * \param[in] h A handle to an open LIN channel.
 *
 * \return ::linOK (zero) if success
 * \return \ref linERR_xxx (negative) if failure
 *
 * \sa linBusOff()
 */
LinStatus LINLIBAPI linBusOn(LinHandle h);

/** 
 * This function deactivates the LIN interface. It will not participate further 
 * in the LIN bus traffic.
 *
 * \param[in] h A handle to an open LIN channel.
 *
 * \return ::linOK (zero) if success
 * \return \ref linERR_xxx (negative) if failure
 *
 * \sa linBusOn()
 */
LinStatus LINLIBAPI linBusOff(LinHandle h);

/** 
 * Return the current timer value (used for timestamps)
 *
 * \note For convenience, this call returns the timer value instead of passing 
 * it in a parameter. This means that if the call fails, it will still return 
 * a value (which then is the error code, type casted to a long unsigned 
 * integer - e.g. 0xFFFFFFF2 for ::linERR_INVHANDLE. Watch out. 
 *
 * \param[in] h        A handle to an open LIN channel.
 * \return If the call succeeds, the present timer value is returned.
 */
unsigned long LINLIBAPI linReadTimer(LinHandle h);

/** 
 * Write a LIN message.
 * It is advisable to wait until the message is echoed by linReadMessage() 
 * before transmitting a new message, or in case of a schedule table being used, 
 * transmit the next message when the previous one is known to be complete.
 * 
 * \note Only available in master mode.
 *
 * \param[in] h        A handle to an open LIN channel.
 * \param[in] id       The identifier of the LIN message.
 * \param[in] msg      A pointer to a buffer containing the data of the LIN 
 *                     message.
 * \param[in] dlc      The length of the LIN message. 
 *
 * \return ::linOK (zero) if success
 * \return \ref linERR_xxx (negative) if failure
 *
 * \sa inWriteSync()
 */
LinStatus LINLIBAPI linWriteMessage(LinHandle h, unsigned int id, const void *msg,
                                    unsigned int dlc);

/** 
 * This function writes a LIN message header to the LIN bus. A slave in the 
 * system is then expected to fill in the header with data. 
 *
 * \note This call is only available in master mode. 
 *
 * \param[in] h        A handle to an open LIN channel.
 * \param[in] id       The identifier of the LIN message. 
 *
 * \return ::linOK (zero) if success
 * \return \ref linERR_xxx (negative) if failure
 */
LinStatus LINLIBAPI linRequestMessage(LinHandle h, unsigned int id);

/** 
 * Read a message from the LIN interface. If a message is available for 
 * reception, ::linOK is returned. This is a non-blocking call. If no message 
 * is available in the LIN interface, an error code is returned.
 *
 * \note This call will also return echoes of what the LIN interface is 
 * transmitting with linWriteMessage. In other words, the LIN interface can hear 
 * itself. 
 *
 * \param[in] h        A handle to an open LIN channel.
 * \param[out] id      A pointer to an integer where the identifier of the 
 *                     received LIN message will be stored. 
 * \param[out] msg     A pointer to a buffer where the data of the LIN message 
 *                     will be stored. 
 * \param[out] dlc     A pointer to an integer where the length of the received 
 *                     LIN message will be stored. 
 * \param[out] flags   A combination of zero or more of the \ref LIN_xxx flags. 
 * \param[out] msgInfo A pointer to a ::LinMessageInfo struct where data about the 
 *                     received LIN message will be stored. 
 *
 * \return ::linOK (zero) if success
 * \return \ref linERR_xxx (negative) if failure
 *
 * \sa linReadMessageWait()
 */
LinStatus LINLIBAPI linReadMessage(LinHandle h, unsigned int *id, void *msg,
                                   unsigned int *dlc, unsigned int *flags,
                                   LinMessageInfo *msgInfo);

/** 
 * Read a message from the LIN interface. If a message is available for 
 * reception, ::linOK is returned. This is a blocking call. It waits until a 
 * message is received in the LIN interface, or the specified timeout period 
 * elapses.
 *
 * \note This call will also return echoes of what the LIN interface is 
 * transmitting with linWriteMessage(). In other words, the LIN interface can
 * hear itself. 
 *
 * \param[in] h        A handle to an open LIN channel.
 * \param[in] timeout  The maximum number of milliseconds to wait for a message 
 *                     to be received by the LIN interface. 
 * \param[out] id      A pointer to an integer where the identifier of the 
 *                     received LIN message will be stored. 
 * \param[out] msg     A pointer to a buffer where the data of the LIN message 
 *                     will be stored. 
 * \param[out] dlc     A pointer to an integer where the length of the received 
 *                     LIN message will be stored. 
 * \param[out] flags   A combination of zero or more of the \ref LIN_xxx flags. 
 * \param[out] msgInfo A pointer to a ::LinMessageInfo struct where data about the 
 *                     received LIN message will be stored. 
 *
 * \return ::linOK (zero) if success
 * \return \ref linERR_xxx (negative) if failure
 *
 * \sa linReadMessage()
 */
LinStatus LINLIBAPI linReadMessageWait(LinHandle h, unsigned int *id, void *msg,
                                       unsigned int *dlc, unsigned int *flags,
                                       LinMessageInfo *msgInfo, unsigned long timeout);

/**
 * This function updates a message buffer in a slave. The contents of the 
 * message buffer will be used the next time the slave is polled for the 
 * specified LIN message id.
 *
 * \note The LIN Interface must be on bus.
 *
 * \param[in] h        A handle to an open LIN channel.
 * \param[in] id       The identifier of the LIN message. 
 * \param[in] msg      A pointer to a buffer containing the data of the LIN message.
 * \param[in] dlc      The length of the LIN message. 
 *
 * \return ::linOK (zero) if success
 * \return \ref linERR_xxx (negative) if failure
 *
 * \sa linClearMessage()
 */
LinStatus LINLIBAPI linUpdateMessage(LinHandle h, unsigned int id, const void *msg,
                                     unsigned int dlc);


/**
 * Using this function, it is possible to use the LIN interface to create 
 * corrupted LIN messages. You call the function once for each LIN identifier 
 * that should be affected. 
 *
 * To return to normal mode, either restart the LIN interface (by going off bus 
 * and on the bus again) or call the function with delay and \a cFlags set to zero.
 *
 * \note  The LIN Interface must be on bus for this command to work.
 * \note It is supported in firmware version 2.4.1 and later.
 *
 * \param[in] h        A handle to an open LIN channel.
 * \param[in] id       The identifier of the LIN message. 
 * \param[in] cFlags   One or more of the \ref LIN_MSG_DISTURB_xxx flags.
 * \param[in] delay    The delay parameter will result in a delay of this many 
 *                     bittimes after the header and before the first data byte. 
 *
 * \return ::linOK (zero) if success
 * \return \ref linERR_xxx (negative) if failure
 *
 * \sa inSetupLIN()
 */
LinStatus LINLIBAPI linSetupIllegalMessage(LinHandle h, unsigned int id,
                                           unsigned int cFlags, unsigned int delay);
                                           
/**
 * \name LIN illegal message flags
 * \anchor LIN_MSG_DISTURB_xxx
 * @{
 */
/**
 * The checksum of transmitted messages will be inverted (and consequently illegal.)
 */
#define LIN_MSG_DISTURB_CSUM   1
/**
 * The two parity bits will be inverted (and consequently illegal.)
 * Used only in master mode.
 */
#define LIN_MSG_DISTURB_PARITY 2
/** @} */


/**
 * \name LIN message parity
 * \anchor LIN_MSG_USE_xxx
 * @{
 */
/**
 * Use enhanced (2.x) parity for the specified msg
 */
#define LIN_MSG_USE_STANDARD_PARITY 0x04
/**
 * Use standard (1.x) parity for the specified msg
 */ 
#define LIN_MSG_USE_ENHANCED_PARITY 0x08 
/** @} */

/** 
 * This function changes various settings on a LIN Interface that is on bus.
 * When going on bus, the bit rate and the flag values listed below are set to 
 * the default value (either as hard-coded in the firmware, or as stored in the 
 * non-volatile memory of the LIN Interface). 
 *
 * With this function, you can do one or more of the following things:
 * - Select checksum according to LIN 2.0 
 * - Turn variable message length off. The message length then will depend on 
 *   the message ID. 
 *
 * In master mode it is also possible to change the bit rate without going off 
 * bus first.
 *
 * \note The LIN Interface must be on bus for this command to work.
 * \note It is supported in firmware version 2.5.1 and later.
 * \note For LIN 2.0 compliance, you must specify both ::LIN_ENHANCED_CHECKSUM 
 * and ::LIN_VARIABLE_DLC. 
 *
 * \param[in] h        A handle to an open LIN channel.
 * \param[in] lFlags   One or more of the following flags: 
 *                     ::LIN_ENHANCED_CHECKSUM, ::LIN_VARIABLE_DLC 
 * \param bps          Specifies the bit rate in bits per second. This parameter 
 *                     can be used only in master mode. The bit rate is set 
 *                     without going off bus. 
 *
 * \return ::linOK (zero) if success
 * \return \ref linERR_xxx (negative) if failure
 *
 * \sa linSetupIllegalMessage()
 */
LinStatus LINLIBAPI linSetupLIN(LinHandle h, unsigned int lFlags, unsigned int bps);


/**
 * \name LIN setup
 * @{
 */
/**
 * When specified, the LIN interface will use the "enhanced" checksum according 
 * to LIN 2.0. Note that (as per the LIN 2.0 spec) the enhanced checksum is not 
 * used on the diagnostic frames even if the ::LIN_ENHANCED_CHECKSUM setting 
 * is in effect. 
 *
 * The default value is OFF.
 */
#define LIN_ENHANCED_CHECKSUM  1
/**
 * When specified, turns variable message length on, so the the message length 
 * will depend on the message ID. 
 *
 * The default value is ON.
 */
#define LIN_VARIABLE_DLC       2
/** @} */

/** 
 * Write a wakeup frame.
 *
 * If count is zero, one single wakeup frame is transmitted. If count > 1, 
 * several wakeup frames are transmitted spaced with 'interval' bittimes. 
 * The LIN interface will interrupt the sequence when a LIN message or another 
 * command is received. The stream of wakeups will be recived as incoming 
 * messages with the LIN_RX flag bit set.
 *
 * \param[in] h        A handle to an open LIN channel.
 * \param[in] count    The number of wakeup frames to send. 
 * \param[in] interval The time, in bit times, between the wakeup frames.
 *
 * \return ::linOK (zero) if success
 * \return \ref linERR_xxx (negative) if failure
 */
LinStatus LINLIBAPI linWriteWakeup(LinHandle h, unsigned int count,
                                   unsigned int interval);


/**
 * Clear a message buffer for a LIN slave. The message buffer will not answer 
 * next time it is polled.
 *
 * \param[in] h        A handle to an open LIN channel.
 * \param[in] id       The LIN message id for which the corresponding buffer 
 *                     will be cleared. 
 *
 * \return ::linOK (zero) if success
 * \return \ref linERR_xxx (negative) if failure
 */
LinStatus LINLIBAPI linClearMessage(LinHandle h, unsigned int id);

/** 
 * Call this function to make sure all messages transmitted to the
 * LIN Interface has been received by it.
 *
 * When messages are transmitted to the LIN Interface, they are queued by
 * Windows before appearing on the CAN bus.
 *
 * The function returns ::linOK if all writes are done, ::linERR_TIMEOUT in
 * case of timeout or possibly some other error code.
 *
 * If the LIN Interface is in master mode and a LIN message has been 
 * transmitted with linWriteMessage(), this function will return when
 * the LIN Interface has received the message. If another LIN messa ge is being
 * received or transmitted, the message will not be transmitted on the
 * LIN bus at once. And even if the LIN Interface is idle, the header of the new
 * message will just have been started when linWriteSync() retur ns.
 * 
 * After calling linUpdateMessage() and linClearMessage() for a slave,
 * this function is enough to know that the LIN Interface is updated. 
 * 
 * After linWriteMessage(), it is advisable to wait until the message is
 * echoed by linReadMessage() before transmitting a new message, or in
 * case of a schedule table being used, transmit the next message when
 * the previous one is known to be complete.
 *
 * When, in master mode, a message should be transmitted after a poll
 * (reception) is done, it might be necessary to call linWriteMessage()
 * before the result is received via linReadMessage() as the LIN Interface waits
 * up to the maximum frame length before knowing a received message is
 * complete. A new message to transmit will force completion if the
 * currently received one.
 *
 * \param[in]  h       A handle to an open LIN channel.
 * \param[in] timeout  The maximum number of milliseconds to wait for the queued 
 *                     messages to be transmitted by the LIN interface. 
 *
 * \return ::linOK (zero) if success
 * \return \ref linERR_xxx (negative) if failure
 *
 * \sa linWriteMessage()
 */
LinStatus LINLIBAPI linWriteSync(LinHandle h, unsigned long timeout);

/**
 * Return the CAN handle given an open LIN handle
 *
 * \param[in]  h          A handle to an open LIN channel.
 * \param[out] canHandle  A pointer to an integer where the CAN handle will be 
 *                        stored. 
 *
 * \return ::linOK (zero) if success
 * \return \ref linERR_xxx (negative) if failure
 */
LinStatus LINLIBAPI linGetCanHandle(LinHandle h, unsigned int *canHandle);

#ifdef __cplusplus
}
#endif

/** @} */

#endif // __LINLIB_H
