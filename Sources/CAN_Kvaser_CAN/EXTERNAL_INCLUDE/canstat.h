/**
 * \section LICENSE
 *                   Copyright 1995-2011 by KVASER AB, SWEDEN
 *
 *                        WWW: http://www.kvaser.com
 *
 * This software is furnished under a license and may be used and copied
 * only in accordance with the terms of such license.
 *
 * \section DESCRIPTION
 *
 *   This file defines status codes for canlib.
 *
 * \file canstat.h
 * \author Kvaser AB
 *
 */


#ifndef _CANSTAT_H_
#define _CANSTAT_H_

//
// Don't forget to update canGetErrorText in canlib.c if this is changed!
//

/**
 * \name canERR_xxx
 * \anchor canERR_xxx
 * 
 * Generally, a return code greater than or equal to zero means success. A
 * value less than zero means failure.
 * 
 * You should avoid testing for a specific error code unless it is explicitly
 * stated in the documentation for the API in question that this specific error
 * code is returned. Instead, check if the error code is less than zero. Use
 * canGetErrorText() to obtain an error message suitable for a message to the
 * user, or for a line in an error log file.
 */
typedef enum {
  /**
   * <b>Normal successful completion</b>; The driver is just fine, and really
   * believes it carried out your command to everyone's satisfaction.
   */
    canOK                  = 0,
  /**
   * <b>Error in one or more parameters</b>; a parameter \ref canERR_xxx
   * specified in the call was invalid, out of range, or so. This status code
   * will also be returned when the call is not implemented.
   */
    canERR_PARAM           = -1,
  /**
   * <b>There were no messages to read</b>; A function tried to read a message,
   * but there was no message to read.
   */
    canERR_NOMSG           = -2,
  /**
   * <b>Specified device or channel not found.</b> There is no hardware
   * available that matches the given search criteria. For example, you may
   * have specified ::canOPEN_REQUIRE_EXTENDED but there's no controller
   * capable of extended CAN. You may have specified a channel number that is
   * out of the range for the hardware in question. You may have requested
   * exclusive access to a channel, but the channel is already occupied.
   */
    canERR_NOTFOUND        = -3,
  /**
   * <b>Out of memory</b>; A memory allocation failed.
   */
    canERR_NOMEM           = -4,
  /**
   * <b>No channels available</b>; There is indeed hardware matching the
   * criteria you specified, but there are no channels available, or the
   * channel you specified is already occupied.
   */
    canERR_NOCHANNELS      = -5,
    canERR_RESERVED_3      = -6,    ///< Reserved
  /**
   * <b>Timeout occurred</b>; A function waited for something to happen (for
   * example, the arrival of a message), but that something didn't happen.
   */
    canERR_TIMEOUT         = -7,
  /**
   * <b>The library is not initialized</b>; The driver is not
   * initialized. canInitializeLibrary() was probably not called?
   */
    canERR_NOTINITIALIZED  = -8,
  /**
   * <b>Out of handles</b>; No handles are available inside canlib32. The
   * application has too many handles open (i.e. has called canOpenChannel() too
   * many times, or there's a memory leak somewhere.)
   *
   * \note We are not talking about Windows handles here, it's CANLIB's own
   * internal handles.
   */
    canERR_NOHANDLES       = -9,
  /**
   * <b>Handle is invalid</b>; The CANLIB handle you specified (if the API call
   * includes a handle) is not valid. Ensure you are passing the handle and
   * not, for example, a channel number.
   */
    canERR_INVHANDLE       = -10,
    canERR_INIFILE         = -11,    ///< Error in the ini-file (16-bit only)
  /**
   * <b>Driver type not supported</b>; CAN driver mode is not supported by the
   * present hardware.
   */
    canERR_DRIVER          = -12,
  /**
   * <b>Transmit buffer overflow</b>; The transmit queue was full, so the
   * message was dropped.
   */
    canERR_TXBUFOFL        = -13,
    canERR_RESERVED_1      = -14,    ///< Reserved
  /**
   * <b>A hardware error has occurred</b>; Something probably related to the
   * hardware happened. This could mean that the device does not respond (IRQ
   * or address conflict?), or that the response was invalid or unexpected
   * (faulty card?).
   */
    canERR_HARDWARE        = -15,
  /**
   * <b>A driver DLL can't be found or loaded</b>; (One of) the DLL(s)
   * specified in the registry failed to load. This could be a driver
   * installation problem.
   */
    canERR_DYNALOAD        = -16,
  /**
   * <b>A DLL seems to have wrong version</b>; DLL version mismatch. (One of)
   * the DLL(s) specified in the registry is - probably - too old, or - less
   * likely - too new.
   */
    canERR_DYNALIB         = -17,
  /**
   * <b>Error when initializing a DLL</b>; Something failed when a device
   * driver was being initialized. In other words, we can open the driver but
   * it makes a lot of fuss about something we don't understand.
   */
    canERR_DYNAINIT        = -18,
    canERR_NOT_SUPPORTED   = -19,    ///< Operation not supported by hardware or firmware
    canERR_RESERVED_5      = -20,    ///< Reserved
    canERR_RESERVED_6      = -21,    ///< Reserved
    canERR_RESERVED_2      = -22,    ///< Reserved
  /**
   * <b>Can't find or load kernel driver</b>; A device driver (kernel mode
   * driver for NT, VxD for W95/98) failed to load; or the DLL could not open
   * the device. Privileges? Driver file missing?
   */
    canERR_DRIVERLOAD      = -23,
  /**
   * <b>DeviceIOControl failed</b>; Use Win32 GetLastError() to learn what
   * really happened.
   */
    canERR_DRIVERFAILED    = -24,
    canERR_NOCONFIGMGR     = -25,    ///< Can't find req'd config s/w (e.g. CS/SS)
    canERR_NOCARD          = -26,    ///< The card was removed or not inserted
    canERR_RESERVED_7      = -27,    ///< Reserved
  /**
   * <b>Error (missing data) in the Registry</b>; A registry key is missing,
   * invalid, malformed, has gone for lunch or what not. can_verify.exe might
   * provide some insight.
   */
    canERR_REGISTRY        = -28,
    canERR_LICENSE         = -29,    ///< The license is not valid.
  /**
   * <b>Internal error in the driver</b>; Indicates an error condition in the
   * driver or DLL, which couldn't be properly handled. Please contact the
   * friendly support at support@kvaser.com.
   */
    canERR_INTERNAL        = -30,
  /**
   * <b>Access denied</b>; This means that you tried to set the bit rate on a
   * handle to which you haven't got init access or you tried to open a channel
   * that already is open with init access. See canOpenChannel() for more
   * information about init access.
   */
    canERR_NO_ACCESS       = -31,
  /**
   * <b>Not implemented</b>; The requested feature or function is not
   * implemented in the device you are trying to use it on.
   */
    canERR_NOT_IMPLEMENTED = -32,
  /**
   * <b>Device File error</b>; An error has occured when trying to access a
   * file on the device.
   */
    canERR_DEVICE_FILE     = -33,
  /**
   * <b>Host File error</b>; An error has occured when trying to access a file on the host.
   *
   */
    canERR_HOST_FILE       = -34,
  /**
   * <b>Disk error</b>; A disk error has occurred. Verify that the disk is
   * initialized.
   */
    canERR_DISK            = -35,
  /**
   * <b>CRC error</b>; The CRC calculation did not match the expected result.
   */
    canERR_CRC             = -36,
  /**
   * <b>Configuration Error</b>; The configuration is corrupt. 
   */
    canERR_CONFIG          = -37,
  /**
   * <b>Memo Error</b>; Other configuration error. 
   */
    canERR_MEMO_FAIL       = -38,
  /**
   * <b>Script Fail</b>; A script has failed.
   *
   * \note This code represents several different failures, for example:
   * - Trying to load a corrupt file or not a .txe file
   * - Trying to start a t script that has not been loaded
   * - Trying to load a t script compiled with the wrong version of the t compiler
   * - Trying to unload a t script that has not been stopped
   * - Trying to use an envvar that does not exist
   */
    canERR_SCRIPT_FAIL     = -39,

  // The last entry - a dummy so we know where NOT to place a comma.
    canERR__RESERVED       = -40    ///< Reserved
} canStatus;


/**
 * \name Convenience
 * @{
 */
#define CANSTATUS_SUCCESS(X) ((X) == canOK)
#define CANSTATUS_FAILURE(X) ((X) != canOK)
/** @} */

/**
 * \name canEVENT_xxx WM__CANLIB notification codes
 * \anchor canEVENT_xxx
 * Appears in the notification \c WM__CANLIB message.
 * @{
 */
#define canEVENT_RX             32000       ///< when the queue of received CAN messages goes from empty to non-empty
#define canEVENT_TX             32001       ///< when a CAN message has been transmitted
#define canEVENT_ERROR          32002       ///< when a CAN bus error is reported by the CAN controller
#define canEVENT_STATUS         32003       ///< when the CAN controller changes state
#define canEVENT_ENVVAR         32004       ///< An envvar changed
#define canEVENT_BUSONOFF       32005       ///< Bus on/off status changed
#define canEVENT_REMOVED        32006       ///< Device removed
/** @} */

/**
 * \name canNOTIFY_xxx
 * \anchor canNOTIFY_xxx
 *
 * These are notification codes used in calls to canSetNotify() and kvSetNotifyCallback().
 * 
 * This can be canNOTIFY_NONE (zero), meaning that no event notification is to
 * occur, or a combination of any of the following constants
 *
 * @{
 */
#define canNOTIFY_NONE          0           ///< Turn notifications off.
#define canNOTIFY_RX            0x0001      ///< CAN message reception notification
#define canNOTIFY_TX            0x0002      ///< CAN message transmission notification
#define canNOTIFY_ERROR         0x0004      ///< CAN bus error notification
#define canNOTIFY_STATUS        0x0008      ///< CAN chip status change
#define canNOTIFY_ENVVAR        0x0010      ///< An environment variable was changed by a script
#define canNOTIFY_BUSONOFF      0x0020      ///< Notify on bus on/off status changed
#define canNOTIFY_REMOVED       0x0040      ///< Notify on device removed
/** @} */


/**
 * \ingroup InformationServices
 * \name canSTAT_xxx
 * \anchor canSTAT_xxx
 *
 * The following circuit status flags are returned by canReadStatus(). Note
 * that more than one flag might be set at any one time.
 *
 * \note Usually both ::canSTAT_HW_OVERRUN and ::canSTAT_SW_OVERRUN are set
 * when overrun has occurred. This is because the kernel driver can't see the
 * difference between a software overrun and a hardware overrun. So the code
 * should always test for both types of overrun using the ::canSTAT_OVERRUN
 * "flag".
 *
 * @{
 */
#define canSTAT_ERROR_PASSIVE   0x00000001  ///< The circuit is error passive
#define canSTAT_BUS_OFF         0x00000002  ///< The circuit is Off Bus
#define canSTAT_ERROR_WARNING   0x00000004  ///< At least one error counter > 96
#define canSTAT_ERROR_ACTIVE    0x00000008  ///< The circuit is error active.
#define canSTAT_TX_PENDING      0x00000010  ///< There are messages pending transmission
#define canSTAT_RX_PENDING      0x00000020  ///< There are messages in the receive buffer
#define canSTAT_RESERVED_1      0x00000040
#define canSTAT_TXERR           0x00000080  ///< There has been at least one TX error
#define canSTAT_RXERR           0x00000100  ///< There has been at least one RX error of some sort
#define canSTAT_HW_OVERRUN      0x00000200  ///< The has been at least one HW buffer overflow
#define canSTAT_SW_OVERRUN      0x00000400  ///< The has been at least one SW buffer overflow
#define canSTAT_OVERRUN         (canSTAT_HW_OVERRUN | canSTAT_SW_OVERRUN) ///< For convenience.
/** @} */

/**
 * \name Message information flags, < 0x100
 * \anchor canMSG_xxx
 *
 * The following flags can be returned from canRead() et al, or passed to
 * canWrite().
 *
 * All flags and/or combinations of them are meaningful for received messages.
 *
 * ::canMSG_RTR, ::canMSG_STD, ::canMSG_EXT, ::canMSG_WAKEUP and
 * ::canMSG_ERROR_FRAME are meaningful also for transmitted messages.
 *
 * @{
 */
#define canMSG_MASK             0x00ff      ///< Used to mask the non-info bits
#define canMSG_RTR              0x0001      ///< Message is a remote request
#define canMSG_STD              0x0002      ///< Message has a standard ID
#define canMSG_EXT              0x0004      ///< Message has an extended ID
#define canMSG_WAKEUP           0x0008      ///< Message to be sent / was received in wakeup mode

/**
 * NERR was active during the message
 *
 * The NERR flag is set by the CAN transceiver when certain CAN bus wire faults
 * occur, namely:
 * \li CANH wire interrupted
 * \li CANL wire interrupted
 * \li CANH short-circuited to battery
 * \li CANH short-circuited to VCC
 * \li CANL short-circuited to ground
 * \li CANH short-circuited to ground
 * \li CANL short-circuited to battery
 * \li CANL short-circuited to VCC
 * \li CANL and CANH mutually short-circuited
 *
 * See the TJA1054 data sheet (available from Philips) for more detailed
 * information.
 */
#define canMSG_NERR             0x0010
#define canMSG_ERROR_FRAME      0x0020      ///< Message is an error frame
#define canMSG_TXACK            0x0040      ///< Message is a TX ACK (msg is really sent)
#define canMSG_TXRQ             0x0080      ///< Message is a TX REQUEST (msg is transfered to the chip)
/** @} */

/**
 * \name Message error flags, >= 0x0100
 * \anchor canMSGERR_xxx
 *
 * \note Not all hardware platforms can detect the difference between hardware
 * overruns and software overruns, so your application should test for both
 * conditions. You can use the symbol ::canMSGERR_OVERRUN for this purpose.
 *
 * @{
 */
#define canMSGERR_MASK          0xff00      ///< Used to mask the non-error bits
// 0x0100 reserved
#define canMSGERR_HW_OVERRUN    0x0200      ///< HW buffer overrun
#define canMSGERR_SW_OVERRUN    0x0400      ///< SW buffer overrun
#define canMSGERR_STUFF         0x0800      ///< Stuff error
#define canMSGERR_FORM          0x1000      ///< Form error
#define canMSGERR_CRC           0x2000      ///< CRC error
#define canMSGERR_BIT0          0x4000      ///< Sent dom, read rec
#define canMSGERR_BIT1          0x8000      ///< Sent rec, read dom

// Convenience values for the message error flags.
#define canMSGERR_OVERRUN       0x0600      ///< Any overrun condition.
#define canMSGERR_BIT           0xC000      ///< Any bit error.
#define canMSGERR_BUSERR        0xF800      ///< Any RX error
/** @} */


/**
 * \name canTRANSCEIVER_LINEMODE_xxx
 * \anchor canTRANSCEIVER_LINEMODE_xxx
 *
 * The following values can be used with canSetDriverMode() to set different 
 * driver (i.e. transceiver) modes. Use canGetDriverMode() to obtain the 
 * current driver mode for a particular CAN channel.
 * @{
 */
#define canTRANSCEIVER_LINEMODE_NA          0  ///< Not Affected/Not available.
#define canTRANSCEIVER_LINEMODE_SWC_SLEEP   4  ///< SWC Sleep Mode.
#define canTRANSCEIVER_LINEMODE_SWC_NORMAL  5  ///< SWC Normal Mode.
#define canTRANSCEIVER_LINEMODE_SWC_FAST    6  ///< SWC High-Speed Mode.
#define canTRANSCEIVER_LINEMODE_SWC_WAKEUP  7  ///< SWC Wakeup Mode.
#define canTRANSCEIVER_LINEMODE_SLEEP       8  ///< Sleep mode for those supporting it.
#define canTRANSCEIVER_LINEMODE_NORMAL      9  ///< Normal mode (the inverse of sleep mode) for those supporting it.
#define canTRANSCEIVER_LINEMODE_STDBY      10  ///< Standby for those who support it
#define canTRANSCEIVER_LINEMODE_TT_CAN_H   11  ///< Truck & Trailer: operating mode single wire using CAN high
#define canTRANSCEIVER_LINEMODE_TT_CAN_L   12  ///< Truck & Trailer: operating mode single wire using CAN low
#define canTRANSCEIVER_LINEMODE_OEM1       13  ///< Reserved for OEM apps
#define canTRANSCEIVER_LINEMODE_OEM2       14  ///< Reserved for OEM apps
#define canTRANSCEIVER_LINEMODE_OEM3       15  ///< Reserved for OEM apps
#define canTRANSCEIVER_LINEMODE_OEM4       16  ///< Reserved for OEM apps

/** @} */

/**
 * \anchor canTRANSCEIVER_RESNET_xxx
 *
 */
#define canTRANSCEIVER_RESNET_NA            0
#define canTRANSCEIVER_RESNET_MASTER        1
#define canTRANSCEIVER_RESNET_MASTER_STBY   2
#define canTRANSCEIVER_RESNET_SLAVE         3

/**
 * \name Transceiver (logical) types
 * \anchor canTRANSCEIVER_TYPE_xxx
 *
 * The following constants can be returned from canGetChannelData(), using the
 * ::canCHANNELDATA_TRANS_TYPE item code. They identify the bus transceiver
 * type for the channel specified in the call to canGetChannelData.
 *
 * \note They indicate a hardware type, but not necessarily a specific circuit
 * or product.
 *
 * @{
 */
// Also see src\include\hwnames.h and registered document 048.
#define canTRANSCEIVER_TYPE_UNKNOWN         0  ///< Unknown or undefined
#define canTRANSCEIVER_TYPE_251             1  ///< 82c251
#define canTRANSCEIVER_TYPE_252             2  ///< 82c252, TJA1053, TJA1054
#define canTRANSCEIVER_TYPE_DNOPTO          3  ///< Optoisolated 82C251
#define canTRANSCEIVER_TYPE_W210            4  ///< 
#define canTRANSCEIVER_TYPE_SWC_PROTO       5  ///< AU5790 prototype
#define canTRANSCEIVER_TYPE_SWC             6  ///< AU5790
#define canTRANSCEIVER_TYPE_EVA             7  ///< 
#define canTRANSCEIVER_TYPE_FIBER           8  ///< 82c251 with fibre extension
#define canTRANSCEIVER_TYPE_K251            9  ///< K-line + 82c251
#define canTRANSCEIVER_TYPE_K              10  ///< K-line, without CAN
#define canTRANSCEIVER_TYPE_1054_OPTO      11  ///< TJA1054 with optical isolation
#define canTRANSCEIVER_TYPE_SWC_OPTO       12  ///< AU5790 with optical isolation
#define canTRANSCEIVER_TYPE_TT             13  ///< B10011S Truck-And-Trailer
#define canTRANSCEIVER_TYPE_1050           14  ///< TJA1050
#define canTRANSCEIVER_TYPE_1050_OPTO      15  ///< TJA1050 with optical isolation
#define canTRANSCEIVER_TYPE_1041           16  ///< TJA1041
#define canTRANSCEIVER_TYPE_1041_OPTO      17  ///< TJA1041 with optical isolation
#define canTRANSCEIVER_TYPE_RS485          18  ///< RS485 (i.e. J1708)
#define canTRANSCEIVER_TYPE_LIN            19  ///< LIN
#define canTRANSCEIVER_TYPE_KONE           20  ///< KONE
#define canTRANSCEIVER_TYPE_LINX_LIN       64
#define canTRANSCEIVER_TYPE_LINX_J1708     66
#define canTRANSCEIVER_TYPE_LINX_K         68
#define canTRANSCEIVER_TYPE_LINX_SWC       70
#define canTRANSCEIVER_TYPE_LINX_LS        72
/** @} */

#endif
