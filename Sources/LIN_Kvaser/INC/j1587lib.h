/**
 * \section LICENSE
 *                   Copyright 2007-2011 by KVASER AB, SWEDEN
 *
 *                        WWW: http://www.kvaser.com
 *
 * This software is furnished under a license and may be used and copied
 * only in accordance with the terms of such license.
 *
 * \section DESCRIPTION
 *
 *   Definitions for the J1587lib API.
 *
 * \file j1587lib.h
 * \author Kvaser AB
 */
 
 
#ifndef __J1587LIB_H
#define __J1587LIB_H

/**
 * \defgroup J1587 J1587
 * @{
 */

#include <windows.h>

/**
 * J1587 handle
 */
typedef int J1587Handle;

/**
 * Invalid J1587 handle
 */
#define j1587INVALID_HANDLE      ((J1587Handle)(-1))

/**
 * \name Linx tranceiver types
 * \anchor canTRANSCEIVER_TYPE_LINX_xxx
 * @{
 */
#define canTRANSCEIVER_TYPE_LINX_LIN       64
#define canTRANSCEIVER_TYPE_LINX_J1708     66
#define canTRANSCEIVER_TYPE_LINX_K         68
#define canTRANSCEIVER_TYPE_LINX_SWC       70
#define canTRANSCEIVER_TYPE_LINX_LS        72
/** @} */

/**
 * \name Operation modes
 * \anchor EEPROM_OP_MODE_xxx
 * @{
 */
#define EEPROM_OP_MODE_NONE             0x00
#define EEPROM_OP_MODE_J1587_NORMAL     0x01
#define EEPROM_OP_MODE_J1587_NODE       0x02
/** @} */

/**
 * \name Flags for J1587MessageInfo
 * \anchor j1587FLAG_xxx
 * @{
 */
#define j1587FLAG_CHECKSUM              0x80
#define j1587FLAG_FRAMEDELAY            0x40
#define j1587FLAG_BYTEDELAY             0x20
#define j1587FLAG_STOPBIT               0x10
#define j1587FLAG_OVERRUN               0x08
/** @} */

/**
 * \name Return codes from the J1587lib functions
 * \anchor j1587ERR_xxx
 * @{
 */
typedef enum {
  j1587OK                  = 0,      ///< OK - no error
  j1587ERR_NOMSG           = -1,     ///< No messages available
  j1587ERR_NOTRUNNING      = -3,
  j1587ERR_RUNNING         = -4,
  j1587ERR_NORMALONLY      = -5,
  j1587ERR_NODEONLY        = -6,
  j1587ERR_PARAM           = -7,     ///< Error in parameter
  j1587ERR_NOTFOUND        = -8,     ///< Specified hw not found.
  j1587ERR_NOMEM           = -9,     ///< Out of memory
  j1587ERR_NOCHANNELS      = -10,    ///< No channels avaliable
  j1587ERR_TIMEOUT         = -11,    ///< Timeout occurred
  j1587ERR_NOTINITIALIZED  = -12,    ///< Library not initialized
  j1587ERR_NOHANDLES       = -13,    ///< Can't get handle
  j1587ERR_INVHANDLE       = -14,    ///< Handle is invalid
  j1587ERR_CANERROR        = -15,
  j1587ERR_ERRRESP         = -16,    ///< There was an error response from the J1587 interface
  j1587ERR_WRONGRESP       = -17,    ///< The J1587 interface response wasn't the expected one
  j1587ERR_DRIVER          = -18,    ///< CAN driver type not supported
  j1587ERR_DRIVERFAILED    = -19,    ///< DeviceIOControl failed; use Win32 GetLastError()
  j1587ERR_NOCARD          = -20,    ///< The card was removed or not inserted
  j1587ERR_LICENSE         = -21,    ///< The license is not valid.
  j1587ERR_INTERNAL        = -22,    ///< Internal error in the driver.
  j1587ERR_NO_ACCESS       = -23,    ///< Access denied
  j1587ERR_VERSION         = -24     ///< Function not supported in this version.
} J1587Status;
/** @} */

/**
 * J1587 library version definition
 */
#define J1587LIB_VERSION  1

// sizeof(J1587MessageInfo) should be 16
#include <pshpack1.h>



/**
 * \name J1587MessageInfo
 * \anchor J1587MessageInfo
 *
 * In certain J1587 bus API calls, the following structure is used to provide 
 * more information about the J1587 messages.
 */
typedef struct {
  /**
   * To enable future extensions of this structure (see J1587LIB_VERSION).
   */
  unsigned char version;

  /**
   * Miscellaneous flags
   */
  unsigned char flags;

  /**
   * Frame delay in bit times (up to 255).
   * - Read:  Length of inter-frame delay.
   * - Write: Delay before message transmission could begin.
   */
  unsigned char frameDelay;

  union {
    /**
     * Checksum as read from the J1587 bus. Might not
     * match the data in case of J1587_CSUM_ERROR.
     */
    unsigned char checkSum;
    /**
     * Number of retries before message could be transmitted.
     */
    unsigned char retries;
  };

  /**
   * Timestamp in ms of the reception/transmission completion
   * of the checksum. Uses the canlib CAN timer.
   */
  unsigned long timestamp;

  /**
   * Total frame length in microseconds;
   * from the first start bit to the end of the checksum.
   */
  unsigned long frameLength;

  /**
   * Bitrate of the message in bits per second.
   * Range [1000 .. 60000] (plus some margin)
   */
  unsigned short bitrate;

  /**
   * Reserved
   */
  unsigned short reserved;
} J1587MessageInfo;
#include <poppack.h>


// Define J1587LIBAPI unless it's done already.
// (j1587lib.c provides its own definition of J1587LIBAPI before including this file.)
//
#ifndef J1587LIBAPI
#   if defined(__BORLANDC__)
#      define J1587LIBAPI __stdcall
#   elif defined(_MSC_VER) || defined(__MWERKS__) || defined(__GNUC__)
#      define J1587LIBAPI __stdcall
#   endif
#endif

#ifdef __cplusplus
extern "C" {
#endif

/** 
 * To be called before the library is used.
 */
void J1587LIBAPI j1587InitializeLibrary(void);


/** 
 * Open a channel to a J1587 interface.
 * 
 * \note The J1587 cable must be powered.
 * 
 * The channel number is the same that
 * is used by canOpenChannel() to access the LAPcan channel.
 *
 * \param[in] channel  The number of the channel. This is the same as used 
 *                     by canOpenChannel() to access the LAPcan channel. 
 * \param[in] flags    Flags of type \ref J1587_xxx
 *
 * \return If the call succeeds, a handle to the opened channel is returned. 
 * The handle is an integer greater than or equal to zero.
 *
 * \return If the call fails, the return value is a negative integer indicating 
 * an error code. See \ref j1587ERR_xxx for a list of possible error codes.
 *
 * \sa j1587Close()
 */
J1587Handle J1587LIBAPI j1587OpenChannel(int channel, int flags);

/**
 * \name Flags for j1587OpenChannel()
 * \anchor J1587_xxx
 * @{
 */
#define J1587_NORMAL 1
#define J1587_NODE   2
#define J1587_READ   4
#define J1587_WRITE  8
/** @} */

/** 
 * Closes an open handle to a J1587 channel. The handle becomes invalid and can 
 * not be used in subsequent calls to the J1587 functions.
 *
 * \param[in] h        A handle to an open J1587 channel.
 *
 * \return ::j1587OK (zero) if success
 * \return \ref j1587ERR_xxx (negative) if failure
 *
 * \sa j1587OpenChannel()
 */
J1587Status J1587LIBAPI j1587Close(J1587Handle h);

/**
 * This function retrieves the firmware version from the J1587 interface.
 *
 * 
 * \note The version numbers aren't valid until j1587BusOn() has been called.
 *
 * \note The firmware in the J1587 interface is divided into two parts, the boot 
 * code and the application. The boot code is used only when reprogramming 
 * (reflashing) the J1597 interface. The application handles all J1587 communication.
 *
 * \note Version numbers are, since the precambric era, divided into a major 
 * version number, a minor version number and a build number. These are usually 
 * written like, for example, 3.2.12. Here the major number is 3, the minor 
 * number 2 and the build number 12.
 *
 * \param[in]  h             A handle to an open J1587 channel.
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
 * \return ::j1587OK (zero) if success
 * \return \ref j1587ERR_xxx (negative) if failure
 */
J1587Status J1587LIBAPI j1587GetFirmwareVersion(J1587Handle h,
                                          unsigned char *bootVerMajor,
                                          unsigned char *bootVerMinor,
                                          unsigned char *bootVerBuild,
                                          unsigned char *appVerMajor,
                                          unsigned char *appVerMinor,
                                          unsigned char *appVerBuild);

/** 
 * This function sets the bit rate 
 *
 * \note The J1587 Interface should not be on bus.
 *
 * \param[in] h   A handle to an open J1587 channel.
 * \param[in] bps Bit rate in bits per second.
 *
 * \return ::j1587OK (zero) if success
 * \return \ref j1587ERR_xxx (negative) if failure
 */
J1587Status J1587LIBAPI j1587SetBitrate(J1587Handle h, unsigned int bps);

/** 
 * This function activates the J1587 interface.
 * \note It will be reset, so any setups done earlier will be lost.
 *
 * \param[in] h   A handle to an open J1587 channel.
 *
 * \return ::j1587OK (zero) if success
 * \return \ref j1587ERR_xxx (negative) if failure
 */
J1587Status J1587LIBAPI j1587BusOn(J1587Handle h);

/** 
 * This function deactivates the J1587 interface. It will be reset clearing any 
 * settings such as J1587 data buffers active.
 *
 * \param[in] h   A handle to an open J1587 channel.
 *
 * \return ::j1587OK (zero) if success
 * \return \ref j1587ERR_xxx (negative) if failure
 */
J1587Status J1587LIBAPI j1587BusOff(J1587Handle h);

/** 
 * Return the current timer value (used for timestamps)
 *
 * \note For convenience, this call returns the timer value instead of passing 
 * it in a parameter. This means that if the call fails, it will still return 
 * a value (which then is the error code, type casted to a long unsigned 
 * integer - e.g. 0xFFFFFFF2 for ::j1587ERR_INVHANDLE. Watch out. 
 *
 * \param[in] h        A handle to an open J1587 channel.
 * \return If the call succeeds, the present timer value is returned.
 */
unsigned long J1587LIBAPI j1587ReadTimer(J1587Handle h);

/**
 * Write a J1587 message. Times out after timeout ms.
 *
 * \param[in] h        A handle to an open J1587 channel.
 * \param[in] msg      A pointer to a buffer containing the J1587 message.
 * \param[in] length   Length of message, max 0xFFF
 * \param[in] priority Priority of message, max 0xFF
 * \param[in] timeout  Times out after \a timeout milliseconds
 *
 * \return ::j1587OK (zero) if success
 * \return \ref j1587ERR_xxx (negative) if failure
 */
J1587Status J1587LIBAPI j1587WriteMessageWait(J1587Handle h, const void *msg,
                                              unsigned int length,
                                              unsigned int priority,
                                              DWORD timeout);

/** 
 * Read a J1587 message.
 *
 * \param[in] h           A handle to an open J1587 channel.
 * \param[out] msg        A pointer to a buffer where the J1587 message will be 
 *                        stored. 
 * \param[out] msgLength  A pointer to an integer where the length of the  
 *                        received J1587 message will be stored. 
 * \param[out] msgInfo    A pointer a \ref J1587MessageInfo structure where the 
 *                        message information will be stored. 
 * \param[in] timeout     Times out after \a timeout milliseconds
 * 
 * \return ::j1587OK (zero) if success
 * \return ::j1587ERR_NOMSG if nothing is received
 * \return \ref j1587ERR_xxx (negative) if failure
 */
J1587Status J1587LIBAPI j1587ReadMessageWait(J1587Handle h, void *msg,
                                             unsigned int *msgLength,
                                             J1587MessageInfo *msgInfo,
                                             DWORD timeout);

/** 
 * Call this function to make sure all messages transmitted to the
 * J1587 Interface have been received by it.
 *
 * When messages are transmitted to the J1587 Interface, they are queued by
 * Windows before appearing on the CAN bus.
 *
 * \param[in] h           A handle to an open J1587 channel.
 * \param[out] msgInfo    A pointer a \ref J1587MessageInfo structure where the 
 *                        message information will be stored. 
 * \param[in] timeout     Times out after \a timeout milliseconds
 *
 * \return ::j1587OK (zero) if all writes are done
 * \return \ref j1587ERR_xxx (negative) if failure
 */
J1587Status J1587LIBAPI j1587WriteSync (J1587Handle h,
                                        J1587MessageInfo *msgInfo,
                                        DWORD timeout);

/**
 * With this function, it is possible to change settings on a J1587 Interface
 * that is on-bus.
 * When going on-bus, the bitrate and the flag values \ref J1587_REPORT_xxx are set 
 * to the default value (either as hard-coded in the firmware, or as stored in the
 * non-volatile memory of the J1587 Interface).
 *
 * \note The J1587 Interface must be on bus for this command to work.
 *
 * \param[in] h           A handle to an open J1587 channel.
 * \param[in] flags       Flags, see \ref J1587_REPORT_xxx
 *
 * \todo Describe how to use J1587_INTER_CHAR_DELAY_MASK
 *
 * \return ::j1587OK (zero) if all writes are done
 * \return \ref j1587ERR_xxx (negative) if failure
 */
J1587Status J1587LIBAPI j1587Configure(J1587Handle h, unsigned int flags);



/**
 * \name Flags for j1587Configure()
 * \anchor J1587_REPORT_xxx
 * @{
 */
#define J1587_INTER_CHAR_DELAY_MASK 0x0f ///< Inter character delay mask
#define J1587_REPORT_BAD_CHECKSUM   0x10 ///< Default off
#define J1587_REPORT_FRAME_DELAY    0x20 ///< Default off
#define J1587_REPORT_CHAR_DELAY     0x40 ///< Default off
/** @} */

/**
 * Return the CAN handle given an open J1587 handle
 *
 * \param[in]  h          A handle to an open J1587 channel.
 * \param[out] canHandle  A pointer to an integer where the CAN handle will be 
 *                        stored. 
 *
 * \return ::j1587OK (zero) if success
 * \return \ref j1587ERR_xxx (negative) if failure
 */
J1587Status J1587LIBAPI j1587GetCanHandle(J1587Handle h, unsigned int *canHandle);

#ifdef __cplusplus
}
#endif

/** @} */

#endif // __J1587LIB_H
