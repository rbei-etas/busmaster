//////////////////////////////////////////////////////////////////////////
// IXXAT Automation GmbH
//////////////////////////////////////////////////////////////////////////
/**
  VCI error codes.

  @file "vcierr.h"
*/
//////////////////////////////////////////////////////////////////////////
// (C) 2002-2011 IXXAT Automation GmbH, all rights reserved
//////////////////////////////////////////////////////////////////////////

#ifndef VCIERR_H
#define VCIERR_H

//////////////////////////////////////////////////////////////////////////
//  include files
//////////////////////////////////////////////////////////////////////////
#include <windows.h>

//////////////////////////////////////////////////////////////////////////
//  constants and macros
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
// data types
//////////////////////////////////////////////////////////////////////////

/*****************************************************************************
 facility codes
*****************************************************************************/

#define SEV_INFO             0x40000000     // informational
#define SEV_WARN             0x80000000     // warnings
#define SEV_ERROR            0xC0000000     // errors
#define SEV_MASK             0xC0000000
#define SEV_SUCCESS          0x00000000

#define RESERVED_FLAG        0x10000000
#define CUSTOMER_FLAG        0x20000000

#define STATUS_MASK          0x0000FFFF
#define FACILITY_MASK        0x0FFF0000

#define SEV_STD_INFO         (SEV_INFO |CUSTOMER_FLAG|FACILITY_STD)
#define SEV_STD_WARN         (SEV_WARN |CUSTOMER_FLAG|FACILITY_STD)
#define SEV_STD_ERROR        (SEV_ERROR|CUSTOMER_FLAG|FACILITY_STD)

#define FACILITY_VCI         0x00010000
#define SEV_VCI_INFO         (SEV_INFO |CUSTOMER_FLAG|FACILITY_VCI)
#define SEV_VCI_WARN         (SEV_WARN |CUSTOMER_FLAG|FACILITY_VCI)
#define SEV_VCI_ERROR        (SEV_ERROR|CUSTOMER_FLAG|FACILITY_VCI)

#define FACILITY_DAL         0x00020000
#define SEV_DAL_INFO         (SEV_INFO |CUSTOMER_FLAG|FACILITY_DAL)
#define SEV_DAL_WARN         (SEV_WARN |CUSTOMER_FLAG|FACILITY_DAL)
#define SEV_DAL_ERROR        (SEV_ERROR|CUSTOMER_FLAG|FACILITY_DAL)

#define FACILITY_CCL         0x00030000
#define SEV_CCL_INFO         (SEV_INFO |CUSTOMER_FLAG|FACILITY_CCL)
#define SEV_CCL_WARN         (SEV_WARN |CUSTOMER_FLAG|FACILITY_CCL)
#define SEV_CCL_ERROR        (SEV_ERROR|CUSTOMER_FLAG|FACILITY_CCL)

#define FACILITY_BAL         0x00040000
#define SEV_BAL_INFO         (SEV_INFO |CUSTOMER_FLAG|FACILITY_BAL)
#define SEV_BAL_WARN         (SEV_WARN |CUSTOMER_FLAG|FACILITY_BAL)
#define SEV_BAL_ERROR        (SEV_ERROR|CUSTOMER_FLAG|FACILITY_BAL)


/*##########################################################################*/
/*##                                                                      ##*/
/*##     VCI error codes                                                  ##*/
/*##                                                                      ##*/
/*##########################################################################*/


//
// MessageId: VCI_SUCCESS
//
// MessageText:
//
//  The operation completed successfully.
//
#define VCI_SUCCESS                    0x00000000L
#define VCI_OK                         VCI_SUCCESS

//
// MessageId: VCI_E_UNEXPECTED
//
// MessageText:
//
//  Unexpected failure
//
#define VCI_E_UNEXPECTED               (SEV_VCI_ERROR | 0x0001)

//
// MessageId: VCI_E_NOT_IMPLEMENTED
//
// MessageText:
//
//  Not implemented
//
#define VCI_E_NOT_IMPLEMENTED          (SEV_VCI_ERROR | 0x0002)

//
// MessageId: VCI_E_OUTOFMEMORY
//
// MessageText:
//
//  Not enough storage is available to complete this operation.
//
#define VCI_E_OUTOFMEMORY              (SEV_VCI_ERROR | 0x0003)

//
// MessageId: VCI_E_INVALIDARG
//
// MessageText:
//
//  One or more parameters are invalid.
//
#define VCI_E_INVALIDARG               (SEV_VCI_ERROR | 0x0004)

//
// MessageId: VCI_E_NOINTERFACE
//
// MessageText:
//
//  The object does not support the requested interface
//
#define VCI_E_NOINTERFACE              (SEV_VCI_ERROR | 0x0005)

//
// MessageId: VCI_E_INVPOINTER
//
// MessageText:
//
//  Invalid pointer
//
#define VCI_E_INVPOINTER               (SEV_VCI_ERROR | 0x0006)

//
// MessageId: VCI_E_INVHANDLE
//
// MessageText:
//
//  Invalid handle
//
#define VCI_E_INVHANDLE                (SEV_VCI_ERROR | 0x0007)

//
// MessageId: VCI_E_ABORT
//
// MessageText:
//
//  Operation aborted
//
#define VCI_E_ABORT                    (SEV_VCI_ERROR | 0x0008)

//
// MessageId: VCI_E_FAIL
//
// MessageText:
//
//  Unspecified error
//
#define VCI_E_FAIL                     (SEV_VCI_ERROR | 0x0009)

//
// MessageId: VCI_E_ACCESSDENIED
//
// MessageText:
//
//  Access is denied.
//
#define VCI_E_ACCESSDENIED             (SEV_VCI_ERROR | 0x000A)

//
// MessageId: VCI_E_TIMEOUT
//
// MessageText:
//
//  This operation returned because the timeout period expired.
//
#define VCI_E_TIMEOUT                  (SEV_VCI_ERROR | 0x000B)

//
// MessageId: VCI_E_BUSY
//
// MessageText:
//
//  The requested resource is in use.
//
#define VCI_E_BUSY                     (SEV_VCI_ERROR | 0x000C)

//
// MessageId: VCI_E_PENDING
//
// MessageText:
//
//  The data necessary to complete this operation is not yet available.
//
#define VCI_E_PENDING                  (SEV_VCI_ERROR | 0x000D)

//
// MessageId: VCI_E_NO_DATA
//
// MessageText:
//
//  No more data available.
//
#define VCI_E_NO_DATA                  (SEV_VCI_ERROR | 0x000E)

//
// MessageId: VCI_E_NO_MORE_ITEMS
//
// MessageText:
//
//  No more entries are available from an enumeration operation.
//
#define VCI_E_NO_MORE_ITEMS            (SEV_VCI_ERROR | 0x000F)

//
// MessageId: VCI_E_NOTINITIALIZED
//
// MessageText:
//
//  The component is not initialized.
//
#define VCI_E_NOT_INITIALIZED          (SEV_VCI_ERROR | 0x0010)

//
// MessageId: VCI_E_ALREADY_INITIALIZED
//
// MessageText:
//
//  An attempt was made to reinitialize an already initialized component.
//
#define VCI_E_ALREADY_INITIALIZED      (SEV_VCI_ERROR | 0x00011)

//
// MessageId: VCI_E_RXQUEUE_EMPTY
//
// MessageText:
//
//  Receive queue empty.
//
#define VCI_E_RXQUEUE_EMPTY            (SEV_VCI_ERROR | 0x00012)

//
// MessageId: VCI_E_TXQUEUE_FULL
//
// MessageText:
//
//  Transmit queue full.
//
#define VCI_E_TXQUEUE_FULL             (SEV_VCI_ERROR | 0x0013)

//
// MessageId: VCI_E_BUFFER_OVERFLOW
//
// MessageText:
//
//  The data was too large to fit into the specified buffer.
//
#define VCI_E_BUFFER_OVERFLOW          (SEV_VCI_ERROR | 0x0014)

//
// MessageId: VCI_E_INVALID_STATE
//
// MessageText:
//
//  The component is not in a valid state to perform this request.
//
#define VCI_E_INVALID_STATE            (SEV_VCI_ERROR | 0x0015)

//
// MessageId: VCI_E_OBJECT_ALREADY_EXISTS
//
// MessageText:
//
//  The object already exists.
//
#define VCI_E_OBJECT_ALREADY_EXISTS    (SEV_VCI_ERROR | 0x0016)

//
// MessageId: VCI_E_INVALID_INDEX
//
// MessageText:
//
//  Invalid index.
//
#define VCI_E_INVALID_INDEX            (SEV_VCI_ERROR | 0x0017)

//
// MessageId: VCI_E_END_OF_FILE
//
// MessageText:
//
//  The end-of-file marker has been reached.
//  There is no valid data in the file beyond this marker.
//
#define VCI_E_END_OF_FILE              (SEV_VCI_ERROR | 0x0018)

//
// MessageId: VCI_E_DISCONNECTED
//
// MessageText:
//
// Attempt to send a message to a disconnected communication port.
//
#define VCI_E_DISCONNECTED             (SEV_VCI_ERROR | 0x0019)

//
// MessageId: VCI_E_WRONG_FLASHFWVERSION
//
// MessageText:
//
// Invalid flash firmware version or version not supported.
// Check driver version and/or update firmware.
//
#define VCI_E_WRONG_FLASHFWVERSION     (SEV_VCI_ERROR | 0x001A)


#endif //_VCIERR_H_