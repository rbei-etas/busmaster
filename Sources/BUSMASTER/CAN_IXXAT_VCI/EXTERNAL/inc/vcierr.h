/*****************************************************************************
 HMS Technology Center Ravensburg GmbH
******************************************************************************

 File    : VCIERR.H
 Summary : VCI error codes.

 Date    : 2003-11-05
 Author  : Hartmut Heim

 Compiler: MSVC

******************************************************************************
 all rights reserved
*****************************************************************************/

/*****************************************************************************
 Values are 32 bit values layed out as follows:

   3 3 2 2 2 2 2 2 2 2 2 2 1 1 1 1 1 1 1 1 1 1
   1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0
  +---+-+-+-----------------------+-------------------------------+
  |Sev|C|R|        Facility       |              Code             | (NTSTATUS)
  +---+-+-+-----------------------+-------------------------------+

  +-+-+-+-+-----------------------+-------------------------------+
  |S|s|C|R|        Facility       |              Code             | (HRESULT)
  +-+-+-+-+-----------------------+-------------------------------+

  where

    Sev - severity code (NTSTATUS)
          00 - Success
          01 - Informational
          10 - Warning
          11 - Error

    S - severity code (HRESULT)
        0 - Success
        1 - Fail (Error)

    s - reserved portion of the facility code, corresponds
        to NT's second severity bit (set to 0)

    C - is the Customer code flag

    R - reserved bit
        Used to indicate a NTSTATUS mapped to an HRESULT.

    Facility - is the facility code

    Code - is the facility's status code
*****************************************************************************/

#ifndef _VCIERR_H_
#define _VCIERR_H_

#include <stdtype.h>

/*****************************************************************************
 * severity & facility codes
 ****************************************************************************/

#ifndef STATUS_SEVERITY_SUCCESS
#define STATUS_SEVERITY_SUCCESS 0
#endif

#ifndef STATUS_SEVERITY_INFORMATIONAL
#define STATUS_SEVERITY_INFORMATIONAL 1
#endif

#ifndef STATUS_SEVERITY_WARNING
#define STATUS_SEVERITY_WARNING 2
#endif

#ifndef STATUS_SEVERITY_ERROR
#define STATUS_SEVERITY_ERROR 3
#endif

#define SEV_SUCCESS          STATUS_SEVERITY_SUCCESS
#define SEV_INFO             STATUS_SEVERITY_INFORMATIONAL
#define SEV_WARN             STATUS_SEVERITY_WARNING
#define SEV_ERROR            STATUS_SEVERITY_ERROR

#define SCODE_MASK           0x0000FFFF
#define FACILITY_MASK        0x0FFF0000
#define RESERVED_FLAG        0x10000000
#define SEVERITY_MASK        0xC0000000
#define CUSTOMER_FLAG        0x20000000

#define FACILITY_SHIFT       16
#define SEVERITY_SHIFT       30

#define MAKESCODE(c)         ((c) & SCODE_MASK)
#define MAKEFACILITY(f)      (((f) << FACILITY_SHIFT) & FACILITY_MASK)
#define MAKESEVERITY(s)      (((s) << SEVERITY_SHIFT) & SEVERITY_MASK)

#define GETSCODE(e)          ((e) & SCODE_MASK)
#define GETFACILITY(e)       (((e) & FACILITY_MASK) >> FACILITY_SHIFT)
#define GETSEVERITY(e)       (((e) & SEVERITY_MASK) >> SEVERITY_SHIFT)

#define FACILITY_VCI         1

#define SEV_VCI_INFO         (MAKESEVERITY(SEV_INFO) | \
                              CUSTOMER_FLAG | MAKEFACILITY(FACILITY_VCI))
#define SEV_VCI_WARN         (MAKESEVERITY(SEV_WARN) | \
                              CUSTOMER_FLAG | MAKEFACILITY(FACILITY_VCI))
#define SEV_VCI_ERROR        (MAKESEVERITY(SEV_ERROR) | \
                              CUSTOMER_FLAG | MAKEFACILITY(FACILITY_VCI))

#define _IS_VCI_SCODE(e)     (((e) & CUSTOMER_FLAG) && \
                              (GETFACILITY(e) == FACILITY_VCI))

#define _VCI_INFO(c)         (SEV_VCI_INFO  | MAKESCODE(c))
#define _VCI_WARN(c)         (SEV_VCI_WARN  | MAKESCODE(c))
#define _VCI_ERROR(c)        (SEV_VCI_ERROR | MAKESCODE(c))


/*****************************************************************************
 * error codes
 ****************************************************************************/

//
// MessageId: VCI_SUCCESS
//
// MessageText:
//
//  The operation completed successfully.
//
#define VCI_SUCCESS                    0
#define VCI_OK                         VCI_SUCCESS

//
// MessageId: VCI_E_UNEXPECTED
//
// MessageText:
//
//  Unexpected failure
//
#define VCI_E_UNEXPECTED               _VCI_ERROR(0x0001)

//
// MessageId: VCI_E_NOT_IMPLEMENTED
//
// MessageText:
//
//  Not implemented
//
#define VCI_E_NOT_IMPLEMENTED          _VCI_ERROR(0x0002)

//
// MessageId: VCI_E_OUTOFMEMORY
//
// MessageText:
//
//  Not enough storage is available to complete this operation.
//
#define VCI_E_OUTOFMEMORY              _VCI_ERROR(0x0003)

//
// MessageId: VCI_E_INVALIDARG
//
// MessageText:
//
//  One or more parameters are invalid.
//
#define VCI_E_INVALIDARG               _VCI_ERROR(0x0004)

//
// MessageId: VCI_E_NOINTERFACE
//
// MessageText:
//
//  The object does not support the requested interface
//
#define VCI_E_NOINTERFACE              _VCI_ERROR(0x0005)

//
// MessageId: VCI_E_INVPOINTER
//
// MessageText:
//
//  Invalid pointer
//
#define VCI_E_INVPOINTER               _VCI_ERROR(0x0006)

//
// MessageId: VCI_E_INVHANDLE
//
// MessageText:
//
//  Invalid handle
//
#define VCI_E_INVHANDLE                _VCI_ERROR(0x0007)

//
// MessageId: VCI_E_ABORT
//
// MessageText:
//
//  Operation aborted
//
#define VCI_E_ABORT                    _VCI_ERROR(0x0008)

//
// MessageId: VCI_E_FAIL
//
// MessageText:
//
//  Unspecified error
//
#define VCI_E_FAIL                     _VCI_ERROR(0x0009)

//
// MessageId: VCI_E_ACCESSDENIED
//
// MessageText:
//
//  Access is denied.
//
#define VCI_E_ACCESSDENIED             _VCI_ERROR(0x000A)

//
// MessageId: VCI_E_TIMEOUT
//
// MessageText:
//
//  This operation returned because the timeout period expired.
//
#define VCI_E_TIMEOUT                  _VCI_ERROR(0x000B)

//
// MessageId: VCI_E_BUSY
//
// MessageText:
//
//  The requested resource is currently busy.
//
#define VCI_E_BUSY                     _VCI_ERROR(0x000C)

//
// MessageId: VCI_E_PENDING
//
// MessageText:
//
//  The data necessary to complete this operation is not yet available.
//
#define VCI_E_PENDING                  _VCI_ERROR(0x000D)

//
// MessageId: VCI_E_NO_DATA
//
// MessageText:
//
//  No more data available.
//
#define VCI_E_NO_DATA                  _VCI_ERROR(0x000E)

//
// MessageId: VCI_E_NO_MORE_ITEMS
//
// MessageText:
//
//  No more entries are available from an enumeration operation.
//
#define VCI_E_NO_MORE_ITEMS            _VCI_ERROR(0x000F)

//
// MessageId: VCI_E_NOTINITIALIZED
//
// MessageText:
//
//  The component is not initialized.
//
#define VCI_E_NOT_INITIALIZED          _VCI_ERROR(0x0010)

//
// MessageId: VCI_E_ALREADY_INITIALIZED
//
// MessageText:
//
//  An attempt was made to reinitialize an already initialized component.
//
#define VCI_E_ALREADY_INITIALIZED      _VCI_ERROR(0x0011)

//
// MessageId: VCI_E_RXQUEUE_EMPTY
//
// MessageText:
//
//  Receive queue empty.
//
#define VCI_E_RXQUEUE_EMPTY            _VCI_ERROR(0x0012)

//
// MessageId: VCI_E_TXQUEUE_FULL
//
// MessageText:
//
//  Transmit queue full.
//
#define VCI_E_TXQUEUE_FULL             _VCI_ERROR(0x0013)

//
// MessageId: VCI_E_BUFFER_OVERFLOW
//
// MessageText:
//
//  The data was too large to fit into the specified buffer.
//
#define VCI_E_BUFFER_OVERFLOW          _VCI_ERROR(0x0014)

//
// MessageId: VCI_E_INVALID_STATE
//
// MessageText:
//
//  The component is not in a valid state to perform this request.
//
#define VCI_E_INVALID_STATE            _VCI_ERROR(0x0015)

//
// MessageId: VCI_E_OBJECT_ALREADY_EXISTS
//
// MessageText:
//
//  The object already exists.
//
#define VCI_E_OBJECT_ALREADY_EXISTS    _VCI_ERROR(0x0016)

//
// MessageId: VCI_E_INVALID_INDEX
//
// MessageText:
//
//  An attempt was made to access an array outside of its bounds.
//
#define VCI_E_INVALID_INDEX            _VCI_ERROR(0x0017)

//
// MessageId: VCI_E_END_OF_FILE
//
// MessageText:
//
//  The end-of-file marker has been reached.
//  There is no valid data in the file beyond this marker.
//
#define VCI_E_END_OF_FILE              _VCI_ERROR(0x0018)

//
// MessageId: VCI_E_DISCONNECTED
//
// MessageText:
//
// Attempt to send a message to a disconnected communication port.
//
#define VCI_E_DISCONNECTED             _VCI_ERROR(0x0019)

//
// MessageId: VCI_E_INVALID_FIRMWARE
//
// MessageText:
//
// Invalid fimware version or version not supported.
// Check driver version and/or update firmware.
//
#define VCI_E_INVALID_FIRMWARE         _VCI_ERROR(0x001A)
#define VCI_E_WRONG_FLASHFWVERSION     VCI_E_INVALID_FIRMWARE

//
// MessageId: VCI_E_INVALID_LICENSE
//
// MessageText:
//
// Invalid license.
//
#define VCI_E_INVALID_LICENSE          _VCI_ERROR(0x001B)

//
// MessageId: VCI_E_NO_SUCH_LICENSE
//
// MessageText:
//
// There is no license available.
//
#define VCI_E_NO_SUCH_LICENSE          _VCI_ERROR(0x001C)

//
// MessageId: VCI_E_LICENSE_EXPIRED
//
// MessageText:
//
// The time limited license has expired.
//
#define VCI_E_LICENSE_EXPIRED          _VCI_ERROR(0x001D)

//
// MessageId: VCI_E_LICENSE_QUOTA_EXCEEDED
//
// MessageText:
//
// The service request exceeds the license quota.
//
#define VCI_E_LICENSE_QUOTA_EXCEEDED   _VCI_ERROR(0x001E)

//
// MessageId: VCI_E_INVALID_TIMING
//
// MessageText:
//
// Invalid bit timing parameter.
//
#define VCI_E_INVALID_TIMING           _VCI_ERROR(0x001F)

//
// MessageId: VCI_E_IN_USE
//
// MessageText:
//
// The resource requested is already in use.
//
#define VCI_E_IN_USE                   _VCI_ERROR(0x0020)

//
// MessageId: VCI_E_NO_SUCH_DEVICE
//
// MessageText:
//
//  A device which does not exist was specified.
//
#define VCI_E_NO_SUCH_DEVICE           _VCI_ERROR(0x0021)

//
// MessageId: VCI_E_DEVICE_NOT_CONNECTED
//
// MessageText:
//
//  The device is not connected.
//
#define VCI_E_DEVICE_NOT_CONNECTED     _VCI_ERROR(0x0022)

//
// MessageId: VCI_E_DEVICE_NOT_READY
//
// MessageText:
//
// The device is not ready for use.
//
#define VCI_E_DEVICE_NOT_READY         _VCI_ERROR(0x0023)

//
// MessageId: VCI_E_TYPE_MISMATCH
//
// MessageText:
//
// Mismatch between the type of object required by the operation
// and the type of object specified in the request.
//
#define VCI_E_TYPE_MISMATCH            _VCI_ERROR(0x0024)

//
// MessageId: VCI_E_NOT_SUPPORTED
//
// MessageText:
//
// The request is not supported.
//
#define VCI_E_NOT_SUPPORTED            _VCI_ERROR(0x0025)

//
// MessageId: VCI_E_DUPLICATE_OBJECTID
//
// MessageText:
//
// The attempt to insert the object ID in the index failed
// because the object ID is already in the index.
//
#define VCI_E_DUPLICATE_OBJECTID       _VCI_ERROR(0x0026)

//
// MessageId: VCI_E_OBJECTID_NOT_FOUND
//
// MessageText:
//
// The specified object ID was not found.
//
#define VCI_E_OBJECTID_NOT_FOUND       _VCI_ERROR(0x0027)

//
// MessageId: VCI_E_WRONG_LEVEL
//
// MessageText:
//
// The requested operation was called from a wrong execution level.
//
#define VCI_E_WRONG_LEVEL              _VCI_ERROR(0x0028)

//
// MessageId: VCI_E_WRONG_DRV_VERSION
//
// MessageText:
//
// Incompatible version of the VCI device driver.
//
#define VCI_E_WRONG_DRV_VERSION        _VCI_ERROR(0x0029)

//
// MessageId: VCI_E_LUIDS_EXHAUSTED
//
// MessageText:
//
// Indicates there are no more LUIDs to allocate.
//
#define VCI_E_LUIDS_EXHAUSTED          _VCI_ERROR(0x002A)

#endif //_VCIERR_H_
