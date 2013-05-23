//////////////////////////////////////////////////////////////////////////
// IXXAT Automation GmbH
//////////////////////////////////////////////////////////////////////////
/**
  Native VCI Programming Library.

  @file "vcinpl.h"
*/

//
//////////////////////////////////////////////////////////////////////////
// event specific functions
//////////////////////////////////////////////////////////////////////////
// vciEventCreate
// vciEventDelete
// vciEventSignal
// vciEventReset
// vciEventWaitFor
//
//////////////////////////////////////////////////////////////////////////
// device manager specific functions
//////////////////////////////////////////////////////////////////////////
// vciEnumDeviceOpen
// vciEnumDeviceClose
// vciEnumDeviceNext
// vciEnumDeviceReset
// vciEnumDeviceWaitEvent
// vciFindDeviceByHwid
// vciFindDeviceByClass
// vciSelectDeviceDlg
//
//////////////////////////////////////////////////////////////////////////
// device specific functions
//////////////////////////////////////////////////////////////////////////
// vciDeviceOpen
// vciDeviceOpenDlg
// vciDeviceClose
// vciDeviceGetInfo
// vciDeviceGetCaps
//
//////////////////////////////////////////////////////////////////////////
// CAN controller specific functions
//////////////////////////////////////////////////////////////////////////
// canControlOpen
// canControlClose
// canControlGetCaps
// canControlGetStatus
// canControlDetectBitrate
// canControlInitialize
// canControlReset
// canControlStart
// canControlSetAccFilter
// canControlAddFilterIds
// canControlRemFilterIds
//
//////////////////////////////////////////////////////////////////////////
// CAN message channel specific functions
//////////////////////////////////////////////////////////////////////////
// canChannelOpen
// canChannelClose
// canChannelGetCaps
// canChannelGetStatus
// canChannelInitialize
// canChannelActivate
// canChannelPeekMessage
// canChannelPostMessage
// canChannelWaitRxEvent
// canChannelWaitTxEvent
// canChannelReadMessage
// canChannelSendMessage
// canChannelPeekMultipleMessages
// canChannelPostMultipleMessages
// canChannelReadMultipleMessages
// canChannelSendMultipleMessages
//
//////////////////////////////////////////////////////////////////////////
// cyclic CAN message scheduler specific functions
//////////////////////////////////////////////////////////////////////////
// canSchedulerOpen
// canSchedulerClose
// canSchedulerGetCaps
// canSchedulerGetStatus
// canSchedulerActivate
// canSchedulerReset
// canSchedulerAddMessage
// canSchedulerRemMessage
// canSchedulerStartMessage
// canSchedulerStopMessage
//
//////////////////////////////////////////////////////////////////////////
// LIN controller specific functions
//////////////////////////////////////////////////////////////////////////
// linControlOpen
// linControlClose
// linControlGetCaps
// linControlGetStatus
// linControlInitialize
// linControlReset
// linControlStart
// linControlWriteMessage
//
//////////////////////////////////////////////////////////////////////////
// LIN message monitor specific functions
//////////////////////////////////////////////////////////////////////////
// linMonitorOpen
// linMonitorClose
// linMonitorGetCaps
// linMonitorGetStatus
// linMonitorInitialize
// linMonitorActivate
// linMonitorPeekMessage
// linMonitorWaitRxEvent
// linMonitorReadMessage
//
//////////////////////////////////////////////////////////////////////////
// (C) 2002-2011 IXXAT Automation GmbH, all rights reserved
//////////////////////////////////////////////////////////////////////////

#ifndef VCINPL_H
#define VCINPL_H

//////////////////////////////////////////////////////////////////////////
//  include files
//////////////////////////////////////////////////////////////////////////
#include <windows.h> 

/*****************************************************************************
 * definition of the vci id
 ****************************************************************************/
#include <vciIdType.h>

/*****************************************************************************
 * vci types
 ****************************************************************************/
#include <vciType.h>

/*****************************************************************************
 * vci controller types
 ****************************************************************************/
#include <vciCtrlType.h>

/*****************************************************************************
 * definition of common declarations
 ****************************************************************************/
#include <vci3.h> 

//////////////////////////////////////////////////////////////////////////
//  constants and macros
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
// data types
//////////////////////////////////////////////////////////////////////////

/*##########################################################################*/
/*##                                                                      ##*/
/*##   exported API functions                                             ##*/
/*##                                                                      ##*/
/*##########################################################################*/

//////////////////////////////////////////////////////////////////////////
// event specific functions
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
/**
  This function creates a event object.

  @param fManReset
    If this parameter is TRUE, the function creates a manual-reset event object 
    which requires use of the vciResetEvent function set the state to non 
    signaled. If this parameter is FALSE, the function creates an auto-reset 
    event object, and the system automatically resets the state to non signaled 
    after a single waiting thread has been released.
  @param fInitState
    If this parameter is TRUE, the initial state of the event object is signaled; 
    otherwise, it is non signaled.
  @param phEvent
    Pointer to a variable where the function stores the handle of the newly 
    created event object. If the function fails the variable is set to NULL.

  @return
    VCI_OK on success, otherwise an Error code.
*/
//////////////////////////////////////////////////////////////////////////
EXTERN_C HRESULT VCIAPI vciEventCreate( IN  BOOL    fManReset,
                                        IN  BOOL    fInitState,
                                        OUT PHANDLE phEvent );

//////////////////////////////////////////////////////////////////////////
/**
  This function deletes a event object.

  @param hEvent
    Handle to the event object to delete.

  @return
    VCI_OK on success, otherwise an Error code.
*/
//////////////////////////////////////////////////////////////////////////
EXTERN_C HRESULT VCIAPI vciEventDelete( IN HANDLE hEvent );

//////////////////////////////////////////////////////////////////////////
/**
  This function sets the specified event object to the signaled state.

  @param hEvent
    Handle to the event object. 

  @return
    VCI_OK on success, otherwise an Error code.
*/
//////////////////////////////////////////////////////////////////////////
EXTERN_C HRESULT VCIAPI vciEventSignal( IN HANDLE hEvent );

//////////////////////////////////////////////////////////////////////////
/**
  This function sets the specified event object to the nonsignaled state.

  @param hEvent
    Handle to the event object. 

  @return
    VCI_OK on success, otherwise an Error code.
*/
//////////////////////////////////////////////////////////////////////////
EXTERN_C HRESULT VCIAPI vciEventReset( HANDLE hEvent );

//////////////////////////////////////////////////////////////////////////
/**
  This function waits until the specified event object is in the signaled
  state or the time-out interval elapses.

  @param hEvent
    Handle to the event object. If this handle is destroyed while the wait is 
    still pending, the function's behavior is undefined.
  @param dwMsTimeout
    Time-out interval, in milliseconds. The function returns if the interval 
    elapses, even if the object's state is non signaled. If this parameter is 
    zero, the function tests the object's state and returns immediately. If
    this parameter is INFINITE (0xFFFFFFFF), the function's time-out interval 
    never elapses.

  @return
    VCI_OK on success, otherwise an Error code.
*/
//////////////////////////////////////////////////////////////////////////
EXTERN_C HRESULT VCIAPI vciEventWaitFor( IN HANDLE hEvent, 
                                         IN UINT32 dwMsTimeout );


//////////////////////////////////////////////////////////////////////////
// device manager specific functions
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
/**
  This function opens a VCI device enumerator.

  @param hEnum
    Points to a variable where the function stores a handle to the opened device 
    enumerator. If the function fails this variable is set to NULL.

  @return
    VCI_OK on success, otherwise an Error code.
*/
//////////////////////////////////////////////////////////////////////////
EXTERN_C HRESULT VCIAPI vciEnumDeviceOpen( OUT PHANDLE hEnum );

//////////////////////////////////////////////////////////////////////////
/**
  This function close the specified VCI device enumerator.

  @param hEnum
    Handle to the VCI device enumerator to close.

  @return
    VCI_OK on success, otherwise an Error code.

  @note
    The handle specified by the parameter <hEnum> is not longer valid
    after the function returns and must not be used any longer.
*/
//////////////////////////////////////////////////////////////////////////
EXTERN_C HRESULT VCIAPI vciEnumDeviceClose ( IN HANDLE hEnum );

//////////////////////////////////////////////////////////////////////////
/**
  This function retrieves information about the next available VCI device.

  @param hEnum
    Handle to the VCI device enumerator.
  @param pInfo
    Points to a VCIDEVICEINFO structure where the function stores information 
    about the next VCI device.

  @return
    VCI_OK 
      on success
    VCI_E_INVHANDLE  
      Invalid handle.
    VCI_E_INVALIDARG 
      One or more arguments are invalid.
    VCI_E_NO_MORE_ITEMS 
    No more entries are available from an enumeration operation.
*/
//////////////////////////////////////////////////////////////////////////
EXTERN_C HRESULT VCIAPI vciEnumDeviceNext( IN  HANDLE         hEnum,
                                           OUT PVCIDEVICEINFO pInfo );

//////////////////////////////////////////////////////////////////////////
/**
  This function resets the specified VCI device enumerator.

  @param hEnum
    Handle to the VCI device enumerator.

  @return
    VCI_OK on success, otherwise an Error code.
*/
//////////////////////////////////////////////////////////////////////////
EXTERN_C HRESULT VCIAPI vciEnumDeviceReset( IN HANDLE hEnum );

//////////////////////////////////////////////////////////////////////////
/**
  This function waits until the device list has changed or the specified
  time-out interval elapses.

  @param hEnum
    Handle to the VCI device enumerator.
  @param dwMsTimeout
    Time-out interval, in milliseconds. The function returns if the interval 
    elapses, even if the device list is not changed. If this parameter is 
    zero, the function tests if the device list is changed and returns 
    immediately. If this parameter is INFINITE (0xFFFFFFFF), the function's
    time-out interval never elapses.

  @return
    VCI_OK on success, VCI_E_TIMEOUT if the time-out interval elapses, 
    otherwise an other Error code.
*/
//////////////////////////////////////////////////////////////////////////
EXTERN_C HRESULT VCIAPI vciEnumDeviceWaitEvent( IN HANDLE hEnum,
                                                IN UINT32 dwMsTimeout );

//////////////////////////////////////////////////////////////////////////
/**
  This function search for a VCI device with the specified hardware ID.

  @param rHwid
    Reference to the unique hardware ID of the device.
  @param pVciid
    Points to a variable where the function stores the unique object ID 
    of the device.

  @return
    VCI_OK on success, otherwise an Error code.
*/
//////////////////////////////////////////////////////////////////////////
EXTERN_C HRESULT VCIAPI vciFindDeviceByHwid( IN  REFGUID rHwid,
                                             OUT PVCIID  pVciid );

//////////////////////////////////////////////////////////////////////////
/**
  This function search for a VCI device with the specified device class
  and instance number.

  @param rClass
    Reference to the unique device class ID
  @param dwInst
    Instance number of the device (0 for 1st instance).
  @param pVciid
    Points to a variable where the function stores the unique object ID of the 
    device.

  @return
    VCI_OK on success, otherwise an Error code.
*/
//////////////////////////////////////////////////////////////////////////
EXTERN_C HRESULT VCIAPI vciFindDeviceByClass( IN  REFGUID rClass,
                                              IN  UINT32  dwInst,
                                              OUT PVCIID  pVciid );

//////////////////////////////////////////////////////////////////////////
/**
  This function shows a dialog box which allows the user to select a
  device from the list of available devices.

  @param hwndParent
    Handle to the window that owns the dialog box.
  @param pVciid
    Points to a variable where the function stores the VCI object ID of the 
    selected device. If the function fails, this variable is set to 0.

  @return
    VCI_OK on success, otherwise an Error code.
    The function returns VCI_E_ABORT if the user cancels the dialog box.
*/
//////////////////////////////////////////////////////////////////////////
EXTERN_C HRESULT VCIAPI vciSelectDeviceDlg( IN  HWND   hwndParent,
                                            OUT PVCIID pVciid );


//////////////////////////////////////////////////////////////////////////
// device specific functions
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
/**
  This function opens a device specified by the unique VCI object ID.

  @param rVciid
    Reference to the unique VCI object identifier of the device to open.
  @param phDevice
    Points to a variable where the function stores the handle to the opened 
    device object. If the function fails, this variable is set to NULL.

  @return
    VCI_OK on success, otherwise an Error code.
*/
//////////////////////////////////////////////////////////////////////////
EXTERN_C HRESULT VCIAPI vciDeviceOpen( IN  REFVCIID rVciid,
                                       OUT PHANDLE  phDevice );

//////////////////////////////////////////////////////////////////////////
/**
  This function shows a dialog box which allows the user to select a VCI device 
  and opens the selected device

  @param hwndParent
    Handle to the window that owns the dialog box.
  @param phDevice
    Points to a variable where the function stores the handle to the opened 
    device object. If the function fails, this variable is set to NULL.

  @return
    VCI_OK on success, otherwise an Error code.
    The function returns VCI_E_ABORT if the user cancels the dialog box.
*/
//////////////////////////////////////////////////////////////////////////
EXTERN_C HRESULT VCIAPI vciDeviceOpenDlg( IN  HWND    hwndParent,
                                          OUT PHANDLE phDevice );

//////////////////////////////////////////////////////////////////////////
/**
  This function close the specified VCI device.

  @param hDevice
    Handle to the VCI device to close.

  @return
    VCI_OK on success, otherwise an Error code.

  @note
    The handle specified by the parameter <hDevice> is not longer valid after 
    the function returns and must not be used any longer.
*/
//////////////////////////////////////////////////////////////////////////
EXTERN_C HRESULT VCIAPI vciDeviceClose( IN HANDLE hDevice );

//////////////////////////////////////////////////////////////////////////
/**
  This function retrieve information about the specified VCI device.

  @param hDevice
    Handle to the VCI device.
  @param pInfo
    Points to a VCIDEVICEINFO structure where the function stores information 
    about the device.

  @return
    VCI_OK on success, otherwise an Error code.

*/
//////////////////////////////////////////////////////////////////////////
EXTERN_C HRESULT VCIAPI vciDeviceGetInfo( IN  HANDLE         hDevice,
                                          OUT PVCIDEVICEINFO pInfo );

//////////////////////////////////////////////////////////////////////////
/**
  This function retrieve the capabilities of the specified VCI device.

  @param hDevice
    Handle to the VCI device.
  @param pCaps
    Points to a PVCIDEVICECAPS structure where the function stores the 
    capabilities of the device.

  @return
    VCI_OK on success, otherwise an Error code.
*/
//////////////////////////////////////////////////////////////////////////
EXTERN_C HRESULT VCIAPI vciDeviceGetCaps( IN  HANDLE         hDevice,
                                          OUT PVCIDEVICECAPS pCaps );


//////////////////////////////////////////////////////////////////////////
// CAN controller specific functions
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
/**
  This function opens the specified CAN controller.

  @param hDevice
    Handle of the device where the CAN controller is located.
  @param dwCanNo
    Number of the CAN controller to open (see also Remarks).
  @param phCanCtl
    Points to a variable where the function stores the handle to the opened 
    CAN controller. If the function fails, this variable is set to NULL.

  @return
    VCI_OK on success, otherwise an Error code.

  @note
    If <dwCanNo> is set to 0xFFFFFFFF, the function shows a dialog box which
    allows the user to select the VCI device and CAN controller. In this case
    <hDevice> must contain the handle to the window that owns the dialog box.
*/
//////////////////////////////////////////////////////////////////////////
EXTERN_C HRESULT VCIAPI canControlOpen( IN  HANDLE  hDevice,
                                        IN  UINT32  dwCanNo,
                                        OUT PHANDLE phCanCtl );

//////////////////////////////////////////////////////////////////////////
/**
  This function close the specified CAN controller.

  @param hCanCtl
    Handle to the CAN controller to close.

  @return
    VCI_OK on success, otherwise an Error code.

  @note
    The handle specified by the parameter <hCanCtl> is not longer valid
    after the function returns and must not be used any longer.
*/
//////////////////////////////////////////////////////////////////////////
EXTERN_C HRESULT VCIAPI canControlClose( IN HANDLE hCanCtl );

//////////////////////////////////////////////////////////////////////////
/**
  This function retrieve the capabilities of the specified CAN controller.

  @param hCanCtl
    Handle to the CAN controller.
  @param pCanCaps
    Points to a CANCAPABILITIES structure where the function stores the 
    capabilities of the CAN controller.

  @return
    VCI_OK on success, otherwise an Error code.
*/
//////////////////////////////////////////////////////////////////////////
EXTERN_C HRESULT VCIAPI canControlGetCaps( IN  HANDLE           hCanCtl,
                                           OUT PCANCAPABILITIES pCanCaps );

//////////////////////////////////////////////////////////////////////////
/**
  This function retrieve the current status of the specified CAN controller.

  @param hCanCtl
    Handle to the CAN controller.
  @param pStatus
    Points to a CANLINESTATUS structure where the function  stores the current 
    line status of the CAN controller.

  @return
    VCI_OK on success, otherwise an Error code.

  @note
    The <bOpMode>, <bBtReg0> and <bBtReg1> fields of the CANLINESTATUS 
    structure contains the same values as defined in the corresponding fields 
    of the CANINITLINE structure when the CAN controller is initialized.
    The <bBusLoad> field of the CANLINESTATUS structure contains the current 
    bus load in percent (if this feature is supported).
    The <wStatus> field of the CANLINESTATUS structure contains the current 
    status of the CAN line. The following bits are defined:
      CAN_STATUS_TXPEND - transmission pending
      CAN_STATUS_OVRRUN - data overrun occurred
      CAN_STATUS_ERRLIM - error warning limit exceeded
      CAN_STATUS_BUSOFF - bus off status
      CAN_STATUS_ININIT - init mode active
*/
//////////////////////////////////////////////////////////////////////////
EXTERN_C HRESULT VCIAPI canControlGetStatus( IN  HANDLE         hCanCtl,
                                             OUT PCANLINESTATUS pStatus );

//////////////////////////////////////////////////////////////////////////
/**
  This function detects the actual bit rate of the CAN bus to which the
  specified controller is connected.

  @param hCanCtl
    Handle to the CAN controller.
  @param wMsTimeout
    Timeout in milliseconds to wait between two successive receive messages.
  @param dwCount
    Number of elements within the BTR tables. This parameter must be less 
    than 64.
  @param pabBtr0
    Points to an array of values for bus timing register (BTR) 0
  @param pabBtr1
    Points to an array of values for bus timing register (BTR) 1
  @param plIndex
    Pointer to a variable where the function stores the index of the detected 
    BTR entry within the specified BTR tables. If no BTR value is found for the 
    current bit rate on the bus, the function set this variable to -1.

  @return
    VCI_OK on success, otherwise an Error code.

  @note
    The function detect the actual baud rate beginning at the first entry 
    within the specified BTR table and switch to the next entry until the
    correct baud rate is detected or the table limit, specified by <dwSize>
    is reached. If the time between two successive receive messages exceed
    the value specified by the <wTimeoutMs> parameter, the function returns
    with VCI_E_TIMEOUT. The total execution time of the function can be
    determined by the following formula:
    TotalExecutionTime [ms] = wTimeoutMs * wSize
*/
//////////////////////////////////////////////////////////////////////////
EXTERN_C HRESULT VCIAPI canControlDetectBitrate( IN  HANDLE  hCanCtl,
                                                 IN  UINT16  wMsTimeout,
                                                 IN  UINT32  dwCount,
                                                 IN  PUINT8  pabBtr0,
                                                 IN  PUINT8  pabBtr1,
                                                 OUT PINT32  plIndex );

//////////////////////////////////////////////////////////////////////////
/**
  This function initialize the specified CAN controller in the specified
  operating mode and bit transfer rate. The function also performs a reset
  of the CAN controller hardware and disables the reception of CAN messages.

  @param hCanCtl
    Handle to the CAN controller.
  @param bMode
    CAN operating mode (see Remarks)
  @param bBtr0
    Value for BUS timing register 0 (see Remarks)
  @param bBtr1
    Value for BUS timing register 1 (see Remarks)

  @return
    VCI_OK on success, otherwise an Error code.

  @note
    The <bMode> parameter defines the operating mode of the CAN controller.
    The operating mode can be a combination of the following primary
    operating mode flags:

    CAN_OPMODE_STANDARD - standard frame format (11 bit identifier)
    CAN_OPMODE_EXTENDED - extended frame format (29 bit identifier)

    Optionally, the following flags can be combined with the primary operating
    mode flags:

    CAN_OPMODE_LISTONLY - listen only mode
    CAN_OPMODE_ERRFRAME - accept error frames
    CAN_OPMODE_LOWSPEED - use low speed bus interface

    The bit transfer rate of the CAN controller is defined by the <bBtr0>
    and <bBtr1> parameters. The values for <bBtr0> and <bBtr1> corresponds
    to the values for the bus timing register 0 and bus timing register 1
    of the SJA 1000 CAN controller at an chip frequency of 16 MHz. See the
    SJA 1000 data sheet for more information of how to compute the timing
    values for a given bit rate.
*/
//////////////////////////////////////////////////////////////////////////
EXTERN_C HRESULT VCIAPI canControlInitialize( IN HANDLE hCanCtl,
                                              IN UINT8  bMode,
                                              IN UINT8  bBtr0,
                                              IN UINT8  bBtr1 );

//////////////////////////////////////////////////////////////////////////
/**
  This function reset the specified CAN controller to it's initial state.
  The function aborts a currently busy transmit message and switch the
  CAN controller into init mode. The function additionally clears the
  standard and extended mode ID filter.

  @param hCanCtl
    Handle to the CAN controller.

  @return
    VCI_OK on success, otherwise an Error code.
*/
//////////////////////////////////////////////////////////////////////////
EXTERN_C HRESULT VCIAPI canControlReset( IN HANDLE hCanCtl );

//////////////////////////////////////////////////////////////////////////
/**
  This function starts or stops the specified CAN controller and switch it
  into either running or init mode. After starting the CAN controller, CAN
  messages can be transmitted over CAN message channels. After stopping the
  CAN controller no further CAN messages are transmitted over the CAN message
  channels.

  @param hCanCtl
    Handle to the CAN controller.
  @param fStart
    TRUE starts the CAN controller, FALSE stops the CAN controller.

  @return
    VCI_OK on success, otherwise an Error code.

  @note
    Stopping the CAN controller does not abort a currently busy transmit
    message and also does not clear the standard and extended ID filter.
*/
//////////////////////////////////////////////////////////////////////////
EXTERN_C HRESULT VCIAPI canControlStart( IN HANDLE hCanCtl,
                                          IN BOOL   fStart );

//////////////////////////////////////////////////////////////////////////
/**
  This function set the global acceptance filter of the specified CAN 
  controller. The global acceptance filter enables the reception of CAN message 
  identifiers specified by the bit patterns passed in <dwCode> and <dwMask>. 
  The message IDs enabled by this function are always accepted, even if the 
  specified IDs are not registered within the filter list (see also 
  canControlAddFilterIds). The function can be called only if the CAN 
  controller is in 'init' mode.

  @param hCanCtl
    Handle to the CAN controller.
  @param fExtend
    Filter selection. If this parameter is set to TRUE, the function sets the 
    29-bit acceptance filter. If this parameter is set to FALSE, the function 
    sets the 11-bit acceptance filter.
  @param dwCode
    Acceptance code inclusive RTR bit.
  @param dwMask
    Acceptance mask that specifies the relevant bits within <dwCode>. Relevant 
    bits are specified by a 1 in the corresponding bit position, non relevant 
    bits are 0.

  @return
    VCI_OK on success, otherwise an Error code.

  @note
    The acceptance filter is defined by the acceptance code and acceptance mask.
    The bit pattern of CANIDs to be received are defined by the acceptance code.
    The corresponding acceptance mask allow to define certain bit positions to
    be don't care (bit x = 0). The values in <dwCode> and <dwMask> have the
    following format:

    <fExtend> = FALSE

    +----+----+----+----+ ~ +----+----+ ~ +---+---+---+---+
    bit | 31 | 30 | 29 | 28 |   | 13 | 12 |   | 3 | 2 | 1 | 0 |
    +----+----+----+----+ ~ +----+----+ ~ +---+---+---+---+
    |  0 |  0 |  0 |  0 |   |  0 |ID11|   |ID2|ID1|ID0|RTR|
    +----+----+----+----+ ~ +----+----+ ~ +---+---+---+---+

    <fExtend> = TRUE

    +----+----+----+----+ ~ +----+----+ ~ +---+---+---+---+
    bit | 31 | 30 | 29 | 28 |   | 13 | 12 |   | 3 | 2 | 1 | 0 |
    +----+----+----+----+ ~ +----+----+ ~ +---+---+---+---+
    |  0 |  0 |ID28|ID27|   |ID12|ID11|   |ID2|ID1|ID0|RTR|
    +----+----+----+----+ ~ +----+----+ ~ +---+---+---+---+

    Example:
    The following example demonstrates how to compute the <dwCode> and <dwMask>
    values to enable the standard IDs in the range from 0x100 to 0x103 whereas
    RTR is 0.

    <dwCode> = 001 0001 1000 0
    <dwMask> = 111 1111 1100 1
    result   = 001 0001 10xx 0

    enabled IDs:
    001 0000 0000 0 (0x100, RTR = 0)
    001 0000 0001 0 (0x101, RTR = 0)
    001 0000 0010 0 (0x102, RTR = 0)
    001 0000 0011 0 (0x103, RTR = 0)
*/
//////////////////////////////////////////////////////////////////////////
EXTERN_C HRESULT VCIAPI canControlSetAccFilter( IN HANDLE hCanCtl,
                                                IN BOOL   fExtend,
                                                IN UINT32 dwCode,
                                                IN UINT32 dwMask );

//////////////////////////////////////////////////////////////////////////
/**
  This function registers the specified CAN message identifier or group
  of identifiers at a filter list of the specified CAN controller. IDs
  registered within the filter list are accepted for reception. The
  function can be called only if the CAN controller is in 'init' mode.

  @param hCanCtl
    Handle to the CAN controller.
  @param fExtend
    Filter selection. If this parameter is set to TRUE, the function adds the 
    ID to the 29-bit filter list. If this parameter is set to FALSE, the 
    function adds the ID to the 11-bit filter list.
  @param dwCode
    Message identifier (inclusive RTR) to add to the filter list.
  @param dwMask
    Mask that specifies the relevant bits within <dwCode>. Relevant bits are 
    specified by a 1 in the corresponding bit position, non relevant bits are 0.

  @return
    VCI_OK on success, otherwise an Error code.

  @note
    The following example demonstrates how to compute the <dwCode> and <dwMask>
    values to register the standard IDs in the range from 0x518 to 0x51B whereas
    RTR is 1.

    <dwCode> = 0101 0001 1000 1
    <dwMask> = 0111 1111 1100 1
    result   = 0101 0001 10xx 1

    IDs registered by this function:
    0101 0001 1000 1 (0x518, RTR = 1)
    0101 0001 1001 1 (0x519, RTR = 1)
    0101 0001 1010 1 (0x51A, RTR = 1)
    0101 0001 1011 1 (0x51B, RTR = 1)
*/
//////////////////////////////////////////////////////////////////////////
EXTERN_C HRESULT VCIAPI canControlAddFilterIds( IN HANDLE hCanCtl,
                                                IN BOOL   fExtend,
                                                IN UINT32 dwCode,
                                                IN UINT32 dwMask );

//////////////////////////////////////////////////////////////////////////
/**
  This function removes the specified CAN message identifier or group of 
  identifiers from a filter list of the specified CAN controller. The function 
  can be called only if the CAN controller is in 'init' mode.

  @param hCanCtl
    Handle to the CAN controller.
  @param fExtend
    Filter selection. This parameter can be either CAN_FILTER_STD to select the 
    11-bit filter list, or CAN_FILTER_EXT to select the 29-bit filter list.
  @param dwCode
    Message identifier (inclusive RTR) to remove from the filter list.
  @param dwMask
    Mask that specifies the relevant bits within <dwCode>. Relevant bits are 
    specified by a 1 in the corresponding bit position, non relevant bits are 0.

  @return
    VCI_OK on success, otherwise an Error code.

  @note
    The following example demonstrates how to compute the <dwCode> and <dwMask>
    values to remove the standard IDs in the range from 0x518 to 0x51B whereas
    RTR is 1.

    <dwCode> = 0101 0001 1000 1
    <dwMask> = 0111 1111 1100 1
    result   = 0101 0001 10xx 1

    IDs removed by this function:
    0101 0001 1000 1 (0x518, RTR = 1)
    0101 0001 1001 1 (0x519, RTR = 1)
    0101 0001 1010 1 (0x51A, RTR = 1)
    0101 0001 1011 1 (0x51B, RTR = 1)
*/
//////////////////////////////////////////////////////////////////////////
EXTERN_C HRESULT VCIAPI canControlRemFilterIds( IN HANDLE hCanCtl,
                                                IN BOOL   fExtend,
                                                IN UINT32 dwCode,
                                                IN UINT32 dwMask );


//////////////////////////////////////////////////////////////////////////
// CAN message channel specific functions
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
/**
  This function opens a CAN message channel on the the specified CAN
  controller.

  @param hDevice
    Handle of the device where the CAN controller is located.
  @param dwCanNo
    Number of the CAN controller to open (see also Remarks).
  @param fExclusive
    If this parameter is set to TRUE the function tries to acquire exclusive 
    access to the CAN message channel, otherwise the function opens the channel 
    in shared mode.
  @param phCanChn
    Points to a variable where the function stores the handle to the opened CAN 
    channel. If the function fails, this variable is set to NULL.

  @return
    VCI_OK on success, otherwise an Error code.

  @note
    If <dwCanNo> is set to 0xFFFFFFFF, the function shows a dialog box which
    allows the user to select the VCI device and CAN controller. In this case
    <hDevice> must contain the handle to the window that owns the dialog box.
*/
//////////////////////////////////////////////////////////////////////////
EXTERN_C HRESULT VCIAPI canChannelOpen( IN  HANDLE  hDevice,
                                        IN  UINT32  dwCanNo,
                                        IN  BOOL    fExclusive,
                                        OUT PHANDLE phCanChn );

//////////////////////////////////////////////////////////////////////////
/**
  This function close the specified CAN message channel.

  @param hCanChn
    Handle to the CAN channel to close.

  @return
    VCI_OK on success, otherwise an Error code.

  @note
    The handle specified by the parameter <hCanChn> is not longer valid
    after the function returns and must not be used any longer.
*/
//////////////////////////////////////////////////////////////////////////
EXTERN_C HRESULT VCIAPI canChannelClose( IN HANDLE hCanChn );

//////////////////////////////////////////////////////////////////////////
/**
  This function retrieve the capabilities of the CAN controller to which
  the specified message channel is connected.

  @param hCanChn
    Handle to the CAN channel
  @param pCanCaps
    Points to a CANCAPABILITIES structure where the function stores the 
    capabilities of the CAN controller.

  @return
    VCI_OK on success, otherwise an Error code.
*/
//////////////////////////////////////////////////////////////////////////
EXTERN_C HRESULT VCIAPI canChannelGetCaps( IN  HANDLE           hCanChn,
                                           OUT PCANCAPABILITIES pCanCaps );

//////////////////////////////////////////////////////////////////////////
/**
  This function retrieve the current status of the specified CAN channel.

  @param hCanChn
    Handle to the CAN channel
  @param pStatus
    Points to a CANCHANSTATUS structure where the function stores the current 
    status of the CAN channel.

  @return
    VCI_OK on success, otherwise an Error code.
*/
//////////////////////////////////////////////////////////////////////////
EXTERN_C HRESULT VCIAPI canChannelGetStatus( IN  HANDLE         hCanChn,
                                             OUT PCANCHANSTATUS pStatus );

//////////////////////////////////////////////////////////////////////////
/**
  This function initializes the specified CAN channel. The function must be 
  called prior to any other function of channel.

  @param hCanChn
    Handle to the CAN channel
  @param wRxFifoSize
    Size of the receive FIFO (number of CAN messages)
  @param wRxThreshold
    Threshold for the receive event to be triggered. The event is triggered if 
    the receive FIFO contains at least the specified number of CAN messages.
  @param wTxFifoSize
    Size of the transmit FIFO (number  of CAN messages)       
  @param wTxThreshold
    Threshold for the transmit event to be triggered. The event is triggered 
    if the transmit FIFO contains at least the specified number of free message 
    entries.

  @return
    VCI_OK on success, otherwise an Error code.

  @note
    - The channel is deactivated after this function returns an must be
      activated by an canChannelActivate() function call.
    - The function can be called more than once to reconfigure the size
      of the receive or transmit FIFOs.
*/
//////////////////////////////////////////////////////////////////////////
EXTERN_C HRESULT VCIAPI canChannelInitialize( IN HANDLE hCanChn,
                                              IN UINT16 wRxFifoSize,
                                              IN UINT16 wRxThreshold,
                                              IN UINT16 wTxFifoSize,
                                              IN UINT16 wTxThreshold );

//////////////////////////////////////////////////////////////////////////
/**
  This function activates or deactivates the CAN channel. After activating
  the channel, CAN messages can be transmitted over the CAN bus by calling
  the transmit and receive functions. After deactivating the channel, no
  further CAN messages are transmitted over the CAN bus.

  @param hCanChn
    Handle to the CAN channel
  @param fEnable
    If this parameter is set to TRUE, the channel is activated.
    If this parameter is set to FALSE, the channel is deactivated.

  @return
    VCI_OK on success, otherwise an Error code.

  @note
    The CAN controller must be started, otherwise no messages are received or
    transmitted from/to the CAN bus (see also canControlStart).
*/
//////////////////////////////////////////////////////////////////////////
EXTERN_C HRESULT VCIAPI canChannelActivate( IN HANDLE hCanChn,
                                            IN BOOL   fEnable );

//////////////////////////////////////////////////////////////////////////
/**
  This function retrieves the next CAN message from the receive FIFO of
  the specified CAN channel. The function does not wait for a message to
  be received from the CAN bus.

  @param hCanChn
    Handle to the CAN channel
  @param pCanMsg
    Pointer to a CANMSG structure where the function stores the retrieved CAN 
    message. If this parameter is set to NULL, the function simply removes the 
    next CAN message from the receive FIFO. 

  @return
  VCI_OK on success, VCI_E_RXQUEUE_EMPTY If there is currently no CAN message 
  available, otherwise an other Error code.
*/
//////////////////////////////////////////////////////////////////////////
EXTERN_C HRESULT VCIAPI canChannelPeekMessage( IN  HANDLE  hCanChn,
                                               OUT PCANMSG pCanMsg );

//////////////////////////////////////////////////////////////////////////
/**
  This function places the specified CAN message in the transmit FIFO of the 
  CAN channel without waiting for the message to be transmitted over the CAN bus

  @param hCanChn
    Handle to the CAN channel
  @param pCanMsg
    Pointer to a initialized CANMSG structure which specifies the message to 
    send.

  @return
    VCI_OK on success, VCI_E_TXQUEUE_FULL if there is not enough free space 
    available within the transmit FIFO, otherwise an other Error code. 
*/
//////////////////////////////////////////////////////////////////////////
EXTERN_C HRESULT VCIAPI canChannelPostMessage( IN HANDLE  hCanChn,
                                               IN PCANMSG pCanMsg );

//////////////////////////////////////////////////////////////////////////
/**
  This function waits until a CAN message is received from the CAN bus
  or the time-out interval elapses.

  @param hCanChn
    Handle to the CAN channel
  @param dwMsTimeout
    Time-out interval, in milliseconds. The function returns if the interval 
    elapses, even if no message is received from the CAN bus. If this parameter 
    is zero, the function tests if a message is available and returns 
    immediately. If this parameter is INFINITE (0xFFFFFFFF), the function's
    time-out interval never elapses.

  @return
    VCI_OK on success, VCI_E_TIMEOUT if the time-out interval elapses, otherwise 
    VCI_E_FAIL.
*/
//////////////////////////////////////////////////////////////////////////
EXTERN_C HRESULT VCIAPI canChannelWaitRxEvent( IN HANDLE hCanChn,
                                               IN UINT32 dwMsTimeout );

//////////////////////////////////////////////////////////////////////////
/**
  This function waits until a CAN message can be written to the transmit
  FIFO or the time-out interval elapses.

  @param hCanChn
    Handle to the CAN channel
  @param dwMsTimeout
    Time-out interval, in milliseconds. The function returns if the interval 
    elapses, even if no message can be written to the transmit FIFO. If this 
    parameter is zero, the function tests if a message can be written and 
    returns immediately. If this parameter is INFINITE (0xFFFFFFFF), the 
    function's time-out interval never elapses.

  @return
    VCI_OK on success, VCI_E_TIMEOUT if the time-out interval elapses, otherwise 
    VCI_E_FAIL.
*/
//////////////////////////////////////////////////////////////////////////
EXTERN_C HRESULT VCIAPI canChannelWaitTxEvent( IN HANDLE hCanChn,
                                               IN UINT32 dwMsTimeout );

//////////////////////////////////////////////////////////////////////////
/**
  This function retrieves the next CAN message from the receive FIFO of
  the specified CAN channel. The function waits for a message to be
  received from the CAN bus.

  @param hCanChn
    Handle to the CAN channel
  @param dwMsTimeout
    Time-out interval, in milliseconds. The function returns if the interval 
    elapses, even if no message is received from the CAN bus. If this parameter 
    is zero, the function tests if a message is available and returns 
    immediately. If this parameter is INFINITE (0xFFFFFFFF), the function's
    time-out interval never elapses.
  @param pCanMsg
    Pointer to a CANMSG structure where the function stores the retrieved CAN 
    message. If this parameter is set to NULL, the function simply removes the 
    next CAN message from the FIFO.

  @return
    VCI_OK 
      on success
    VCI_E_TIMEOUT 
      if the time-out interval elapses
    VCI_E_RXQUEUE_EMPTY
      If there is currently no CAN message available, 
    otherwise an other Error code
*/
//////////////////////////////////////////////////////////////////////////
EXTERN_C HRESULT VCIAPI canChannelReadMessage( IN  HANDLE  hCanChn,
                                               IN  UINT32  dwMsTimeout,
                                               OUT PCANMSG pCanMsg );

//////////////////////////////////////////////////////////////////////////
/**
  This function places the specified CAN message in the transmit FIFO.
  The function waits until the message is placed in the transmit FIFO,
  but does not wait for the message to be transmitted over the CAN bus.

  @param hCanChn
    Handle to the CAN channel
  @param dwMsTimeout
    Time-out interval, in milliseconds. The function returns if the interval 
    elapses, even if no message can be written to the transmit FIFO. If this 
    parameter is zero, the function tests if a message can be written and 
    returns immediately. If this parameter is INFINITE (0xFFFFFFFF), the 
    function's time-out interval never elapses.
  @param pCanMsg
    Pointer to a initialized CANMSG structure which specifies the CAN message 
    to send.

  @return
    VCI_OK 
      on success
    VCI_E_TIMEOUT
      if the time-out interval elapses
    VCI_E_TXQUEUE_FULL
      If there is not enough free space available within the transmit FIFO, 
      the function returns
    otherwise an other Error code.
*/
//////////////////////////////////////////////////////////////////////////
EXTERN_C HRESULT VCIAPI canChannelSendMessage( IN HANDLE  hCanChn,
                                               IN UINT32  dwMsTimeout,
                                               IN PCANMSG pCanMsg );

//////////////////////////////////////////////////////////////////////////
/**
  This function retrieves the next CAN messages from the receive FIFO of
  the specified CAN channel. The function does not wait for a message to
  be received from the CAN bus.

  @param hCanChn
    Handle to the CAN channel
  @param pdwNum
    (IN) number of messages who can be received (OUT) messages received
  @param aCanMsg[]
    Pointer to a CANMSG structure where the function stores the retrieved CAN 
    message. If this parameter is set to NULL, the function simply removes the 
    next CAN message from the receive FIFO.

  @return
    VCI_OK 
      on success
    VCI_E_RXQUEUE_EMPTY
      if there is currently no CAN message available
     otherwise an Error code.
*/
//////////////////////////////////////////////////////////////////////////
EXTERN_C HRESULT VCIAPI 
  canChannelPeekMultipleMessages ( IN     HANDLE  hCanChn,
                                   IN OUT UINT32 * pdwNum, 
                                   OUT    CANMSG aCanMsg[] );

//////////////////////////////////////////////////////////////////////////
/**
  This function places the specified multiple CAN messages in the transmit FIFO
  of the CAN channel without waiting for the message to be transmitted over 
  the CAN bus.

  @param hCanChn
    Handle to the CAN channel
  @param pdwNum
    (IN) number of messages to send (OUT) messages sent
  @param aCanMsg[]
    Pointer to a initialized CANMSG structure which specifies the message to 
    send. 
  @return
    VCI_OK 
      on success
    VCI_E_TXQUEUE_FULL
      if there is not enough free space available within the transmit FIFO
    otherwise an other Error code.
*/
//////////////////////////////////////////////////////////////////////////
EXTERN_C HRESULT VCIAPI 
  canChannelPostMultipleMessages ( IN     HANDLE  hCanChn,
                                   IN OUT UINT32 * pdwNum, 
                                   IN     CANMSG aCanMsg[] );

//////////////////////////////////////////////////////////////////////////
/**
  This function retrieves the next CAN messages from the receive FIFO of
  the specified CAN channel. The function waits for a message to be
  received from the CAN bus.

  @param hCanChn
    Handle to the CAN channel
  @param dwMsTimeout
    Time-out interval, in milliseconds. The function returns if the interval 
    elapses, even if no message is received from the CAN bus. If this parameter 
    is zero, the function tests if a message is available and returns 
    immediately. If this parameter is INFINITE (0xFFFFFFFF), the function's
    time-out interval never elapses.
  @param pdwNum
    (IN) number of messages who can be received (OUT) messages received
  @param aCanMsg[]
    Pointer to a CANMSG structure where the function stores the retrieved CAN 
    message. If this parameter is set to NULL, the function simply removes the 
    next CAN message from the FIFO.

  @return
    VCI_OK 
      on success
    VCI_E_TIMEOUT
      if the time-out interval elapses, the function returns . 
    VCI_E_RXQUEUE_EMPTY
      if there is currently no CAN message available
    otherwise an other Error code.
*/
//////////////////////////////////////////////////////////////////////////
EXTERN_C HRESULT VCIAPI 
  canChannelReadMultipleMessages ( IN     HANDLE  hCanChn,
                                   IN     UINT32  dwMsTimeout,
                                   IN OUT UINT32 * pdwNum, 
                                   OUT    CANMSG aCanMsg[] );

//////////////////////////////////////////////////////////////////////////
/**
  This function places the specified CAN message in the transmit FIFO.
  The function waits until the message is placed in the transmit FIFO,
  but does not wait for the message to be transmitted over the CAN bus.

  @param hCanChn
    Handle to the CAN channel
  @param dwMsTimeout
  Time-out interval, in milliseconds. The function
  returns if the interval elapses, even if no message
  can be written to the transmit FIFO. If this parameter
  is zero, the function tests if a message can be written
  and returns immediately. If this parameter is INFINITE
  (0xFFFFFFFF), the function's time-out interval never
  elapses.
  @param pdwNum
    (IN) number of messages to send (OUT) messages sent
  @param aCanMsg[]
  Pointer to a initialized CANMSG structure which specifies
  the CAN message to send.

  @return
    VCI_OK 
      on success
    VCI_E_TIMEOUT
      if the time-out interval elapses
    VCI_E_TXQUEUE_FULL
      if there is not enough free space available within the transmit FIFO
    otherwise an other Error code.
*/
//////////////////////////////////////////////////////////////////////////
EXTERN_C HRESULT VCIAPI 
  canChannelSendMultipleMessages ( IN     HANDLE  hCanChn,
                                   IN     UINT32  dwMsTimeout,
                                   IN OUT UINT32 * pdwNum,
                                   IN     CANMSG aCanMsg[] );

//////////////////////////////////////////////////////////////////////////
// cyclic CAN message scheduler specific functions
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
/**
  This function opens a CAN message scheduler on the the specified CAN
  controller.

  @param hDevice
    Handle of the device where the CAN controller is located.
  @param dwCanNo
    Number of the CAN controller to open (see also Remarks).
  @param phCanShd
    Points to a variable where the function stores the handle to the opened CAN 
    message scheduler. If the function fails, this variable is set to NULL.

  @return
    VCI_OK on success, otherwise an Error code

  @note
    If <dwCanNo> is set to 0xFFFFFFFF, the function shows a dialog box which
    allows the user to select the VCI device and CAN controller. In this case
    <hDevice> must contain the handle to the window that owns the dialog box.
*/
//////////////////////////////////////////////////////////////////////////
EXTERN_C HRESULT VCIAPI canSchedulerOpen( IN  HANDLE  hDevice,
                                          IN  UINT32  dwCanNo,
                                          OUT PHANDLE phCanShd );

//////////////////////////////////////////////////////////////////////////
/**
  This function close the specified CAN message scheduler.

  @param hCanShd
    Handle to the CAN message scheduler to close.

  @return
    VCI_OK on success, otherwise an Error code

  @note
    The handle specified by the parameter <hCanShd> is not longer valid
    after the function returns and must not be used any longer.
*/
//////////////////////////////////////////////////////////////////////////
EXTERN_C HRESULT VCIAPI canSchedulerClose( IN HANDLE hCanShd );

//////////////////////////////////////////////////////////////////////////
/**
  This function retrieve the capabilities of the specified CAN controller.

  @param hCanShd
    Handle to the CAN message
  @param pCanCaps
    Points to a CANCAPABILITIES structure where the function stores the 
    capabilities of the CAN controller.

  @return
    VCI_OK on success, otherwise an Error code
*/
//////////////////////////////////////////////////////////////////////////
EXTERN_C HRESULT VCIAPI canSchedulerGetCaps( IN  HANDLE           hCanShd,
                                             OUT PCANCAPABILITIES pCanCaps );

//////////////////////////////////////////////////////////////////////////
/**
  This function retrieve the status of the cyclic message scheduler and
  all messages currently registered at the cyclic message scheduler.

  @param hCanShd
    Handle to the CAN message
  @param pStatus
    Points to a CANSCHEDULERSTATUS structure where the function stores the 
    current status of the cyclic message scheduler.

  @return
    VCI_OK on success, otherwise an Error code
*/
//////////////////////////////////////////////////////////////////////////
EXTERN_C HRESULT VCIAPI canSchedulerGetStatus( IN  HANDLE              hCanShd,
                                               OUT PCANSCHEDULERSTATUS pStatus );

//////////////////////////////////////////////////////////////////////////
/**
  This function starts or stops the scheduler and the processing of all 
  currently registered messages.

  @param hCanShd
    Handle to the CAN message
  @param fEnable
    If this parameter is set to TRUE, the scheduler is activated.
    If this parameter is set to FALSE, the scheduler is deactivated.

  @return
    VCI_OK on success, otherwise an Error code
*/
//////////////////////////////////////////////////////////////////////////
EXTERN_C HRESULT VCIAPI canSchedulerActivate( IN HANDLE hCanShd,
                                              IN BOOL   fEnable );

//////////////////////////////////////////////////////////////////////////
/**
  This function suspends execution of the scheduler and removes all
  currently registered messages.

  @param hCanShd
    Handle to the CAN message

  @return
    VCI_OK on success, otherwise an Error code
*/
//////////////////////////////////////////////////////////////////////////
EXTERN_C HRESULT VCIAPI canSchedulerReset( IN HANDLE hCanShd );

//////////////////////////////////////////////////////////////////////////
/**
  This function adds a new message to the cyclic message scheduler.
 
  @param hCanShd
    Handle to the CAN message
  @param pMessage
    Points to the cyclic transmit message to add
  @param pdwIndex
    Points to a variable where the function stores the index of the newly added 
    message within the transmit list of the cyclic message scheduler. The index
    returned in this variable must be passed to all other functions that access 
    the added message.

  @return
    VCI_OK on success, otherwise an Error code
*/
//////////////////////////////////////////////////////////////////////////
EXTERN_C HRESULT VCIAPI canSchedulerAddMessage( IN  HANDLE          hCanShd,
                                                IN  PCANCYCLICTXMSG pMessage,
                                                OUT PUINT32         pdwIndex );
//////////////////////////////////////////////////////////////////////////
/**
  This function removes the specified message from the cyclic message
  scheduler.

  @param hCanShd
    Handle to the CAN message
  @param dwIndex
    Index of the cyclic transmit message to remove.

  @return
    VCI_OK on success, otherwise an Error code
*/
//////////////////////////////////////////////////////////////////////////

EXTERN_C HRESULT VCIAPI canSchedulerRemMessage( IN HANDLE hCanShd,
                                                IN UINT32 dwIndex );

//////////////////////////////////////////////////////////////////////////
/**
  This function starts processing of the specified cyclic transmit message.

  @param hCanShd
    Handle to the CAN message
  @param dwIndex
    Index of the cyclic transmit message to start.
  @param wRepeat
    Number of repetitions the message should be sent. If this parameter is set 
    to 0, the message is sent endlessly.

  @return
    VCI_OK on success, otherwise an Error code
*/
//////////////////////////////////////////////////////////////////////////
EXTERN_C HRESULT VCIAPI canSchedulerStartMessage( IN HANDLE hCanShd,
                                                  IN UINT32 dwIndex,
                                                  IN UINT16 wRepeat );

//////////////////////////////////////////////////////////////////////////
/**
  This function stops processing of the specified cyclic transmit message.

  @param hCanShd
    Handle to the CAN message
  @param dwIndex
    Index of the cyclic transmit message to stop.

  @return
    VCI_OK on success, otherwise an Error code
*/
//////////////////////////////////////////////////////////////////////////
EXTERN_C HRESULT VCIAPI canSchedulerStopMessage( IN HANDLE hCanShd,
                                                 IN UINT32 dwIndex );


//////////////////////////////////////////////////////////////////////////
// LIN controller specific functions
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
/**
  This function opens the specified LIN controller.

  @param hDevice
    Handle of the device where the CAN controller is located.
  @param dwLinNo
    Number of the LIN controller to open (see also Remarks).
  @param phLinCtl
    Points to a variable where the function stores the handle to the opened LIN 
    controller. If the function fails, this variable is set to NULL.

  @return
    VCI_OK on success, otherwise an Error code

  @note
    If <dwLinNo> is set to 0xFFFFFFFF, the function shows a dialog box which
    allows the user to select the VCI device and LIN controller. In this case
    <hDevice> must contain the handle to the window that owns the dialog box.
*/
//////////////////////////////////////////////////////////////////////////
EXTERN_C HRESULT VCIAPI linControlOpen( IN  HANDLE  hDevice,
                                        IN  UINT32  dwLinNo,
                                        OUT PHANDLE phLinCtl );

//////////////////////////////////////////////////////////////////////////
/**
  This function close the specified LIN controller.

  @param hLinCtl
    Handle to the LIN controller to close.

  @return
    VCI_OK on success, otherwise an Error code

  @note
    The handle specified by the parameter <hLinCtl> is not longer valid
    after the function returns and must not be used any longer.
*/
//////////////////////////////////////////////////////////////////////////
EXTERN_C HRESULT VCIAPI linControlClose( IN HANDLE hLinCtl );

//////////////////////////////////////////////////////////////////////////
/**
  This function retrieve the capabilities of the specified LIN controller.

  @param hLinCtl
    Handle to the LIN controller.
  @param pLinCaps
    Points to a LINCAPABILITIES structure where the function stores the 
    capabilities of the LIN controller.

  @return
    VCI_OK on success, otherwise an Error code
*/
//////////////////////////////////////////////////////////////////////////
EXTERN_C HRESULT VCIAPI linControlGetCaps( IN  HANDLE           hLinCtl,
                                           OUT PLINCAPABILITIES pLinCaps );

//////////////////////////////////////////////////////////////////////////
/**
  This function retrieve the current status of the specified LIN controller.

  @param hLinCtl
    Handle to the LIN controller.
  @param pStatus
    Points to a LINLINESTATUS structure where the function stores the current 
    line status of the LIN controller.

  @return
    VCI_OK on success, otherwise an Error code
*/
//////////////////////////////////////////////////////////////////////////
EXTERN_C HRESULT VCIAPI linControlGetStatus( IN  HANDLE         hLinCtl,
                                             OUT PLINLINESTATUS pStatus );

//////////////////////////////////////////////////////////////////////////
/**
    This function initialize the specified LIN controller in the specified
  operating mode. The function also performs a reset of the LIN controller
  hardware and disables the transmission and reception of LIN messages.

  @param hLinCtl
    Handle to the LIN controller.
  @param bMode
    LIN operating mode (see Remarks)
  @param wBitrate
    bit rate

  @return
    VCI_OK on success, otherwise an Error code

  @note
    The <bMode> parameter defines the operating mode of the LIN controller.
    The operating mode can be a combination of the following operating mode
    flags:
    LIN_OPMODE_SLAVE  slave
    LIN_OPMODE_MASTER master 
    LIN_OPMODE_ERRORS Errors
*/
//////////////////////////////////////////////////////////////////////////
EXTERN_C HRESULT VCIAPI linControlInitialize( IN HANDLE hLinCtl,
                                              IN UINT8  bMode,
                                              IN UINT16 wBitrate );

//////////////////////////////////////////////////////////////////////////
/**
  This function reset the specified LIN controller to it's initial state.
  The function aborts the current message transmission and switch the LIN
  controller into init mode.

  @param hLinCtl
    Handle to the CAN controller.

  @return
    VCI_OK on success, otherwise an Error code
*/
//////////////////////////////////////////////////////////////////////////
EXTERN_C HRESULT VCIAPI linControlReset( IN HANDLE hLinCtl );

//////////////////////////////////////////////////////////////////////////
/**
  This function starts or stops the specified LIN controller and switch it
  into either running or init mode.

  @param hLinCtl
    Handle to the LIN controller.
  @param fStart
    TRUE starts the LIN controller, FALSE stops the LIN controller.

  @return
    VCI_OK on success, otherwise an Error code

  @note
    Stopping the LIN controller does not abort a current message transmission.
*/
//////////////////////////////////////////////////////////////////////////
EXTERN_C HRESULT VCIAPI linControlStart( IN HANDLE hLinCtl,
                                         IN BOOL   fStart );

//////////////////////////////////////////////////////////////////////////
/**
  This function either sends the specified message to the LIN bus or
  writes it to the appropriate response buffer.

  @param hLinCtl
    Handle to the LIN controller.
  @param fSend
    If this parameter is set to TRUE, the message is send to the LIN bus. 
    If this parameter is set to FALSE the message is written to the response 
    buffer.
  @param pLinMsg
    Points to a initialize LINMSG structure with the message.

  @return
    VCI_OK on success, otherwise an Error code
*/
//////////////////////////////////////////////////////////////////////////
EXTERN_C HRESULT VCIAPI linControlWriteMessage( IN HANDLE  hLinCtl,
                                                IN BOOL    fSend,
                                                IN PLINMSG pLinMsg );


//////////////////////////////////////////////////////////////////////////
// LIN message monitor specific functions
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
/**
  This function opens a LIN message monitor on the the specified LIN
  controller.

  @param hDevice
    Handle of the device where the CAN controller is located.
  @param dwLinNo
    Number of the LIN controller to open (see also Remarks).
  @param fExclusive
   If this parameter is set to TRUE the function tries to acquire exclusive 
   access to the LIN message monitor, otherwise the function opens the monitor 
   in shared mode.
  @param phLinMon
    Points to a variable where the function stores the handle to the opened LIN 
    monitor. If the function fails, this variable is set to NULL.

  @return
    VCI_OK on success, otherwise an Error code

  @note
    If <dwLinNo> is set to 0xFFFFFFFF, the function shows a dialog box which
    allows the user to select the VCI device and LIN controller. In this case
    <hDevice> must contain the handle to the window that owns the dialog box.
*/
//////////////////////////////////////////////////////////////////////////
EXTERN_C HRESULT VCIAPI linMonitorOpen( IN  HANDLE  hDevice,
                                        IN  UINT32  dwLinNo,
                                        IN  BOOL    fExclusive,
                                        OUT PHANDLE phLinMon );

//////////////////////////////////////////////////////////////////////////
/**
  This function close the specified LIN message monitor.

  @param hLinMon
    Handle to the LIN monitor to close.

  @return
    VCI_OK on success, otherwise an Error code

  @note
    The handle specified by the parameter <hLinMon> is not longer valid
    after the function returns and must not be used any longer.
*/
//////////////////////////////////////////////////////////////////////////
EXTERN_C HRESULT VCIAPI linMonitorClose( IN HANDLE hLinMon );

//////////////////////////////////////////////////////////////////////////
/**
  This function retrieve the capabilities of the LIN controller associated
  with the specified message monitor.

  @param hLinMon
    Handle to the LIN monitor to close.
  @param pLinCaps
    Points to a LINCAPABILITIES structure where the function stores the 
    capabilities of the LIN controller.

  @return
    VCI_OK on success, otherwise an Error code
*/
//////////////////////////////////////////////////////////////////////////
EXTERN_C HRESULT VCIAPI linMonitorGetCaps( IN  HANDLE           hLinMon,
                                           OUT PLINCAPABILITIES pLinCaps );

//////////////////////////////////////////////////////////////////////////
/**
  This function retrieves the current status of the specified LIN monitor.

  @param hLinMon
    Handle to the LIN monitor .
  @param pStatus
    Points to a LINMONITORSTATUS structure where the function stores the 
    current status of the LIN monitor.

  @return
    VCI_OK on success, otherwise an Error code

  @note
    [small(explanation)]
*/
//////////////////////////////////////////////////////////////////////////
EXTERN_C HRESULT VCIAPI linMonitorGetStatus( IN  HANDLE            hLinMon,
                                             OUT PLINMONITORSTATUS pStatus );

//////////////////////////////////////////////////////////////////////////
/**
  This function initializes the specified LIN monitor. The function
  must be called prior to any other function of monitor.

  @param hLinMon
    Handle to the LIN monitor.
  @param wFifoSize
    Size of the receive FIFO (number of LIN messages)
  @param wThreshold
    Threshold for the receive event to be triggered. The event is triggered if 
    the receive FIFO contains at least the specified number of LIN messages.

  @return
    VCI_OK on success, otherwise an Error code

  @note
  - The monitor is deactivated after this function returns and must be
    activated by an linMonitorActivate() function call.
  - The function can be called more than once to reconfigure the size
    of the receive or transmit FIFOs.
*/
//////////////////////////////////////////////////////////////////////////
EXTERN_C HRESULT VCIAPI linMonitorInitialize( IN HANDLE hLinMon,
                                              IN UINT16 wFifoSize,
                                              IN UINT16 wThreshold );

//////////////////////////////////////////////////////////////////////////
/**
  This function activates or deactivates the LIN monitor. After activating
  the monitor, LIN messages are received from the LIN bus by calling the
  receive functions. After deactivating the monitor, no further messages
  are received from the LIN bus.

  @param hLinMon
    Handle to the LIN monitor.
  @param fEnable
    If this parameter is set to TRUE, the channel is activated.
             If this parameter is set to FALSE, the channel is deactivated.

  @return
    VCI_OK on success, otherwise an Error code

  @note
    The LIN controller must be started, otherwise no messages are received
    from the LIN bus (see also linControlStart).
*/
//////////////////////////////////////////////////////////////////////////
EXTERN_C HRESULT VCIAPI linMonitorActivate( IN HANDLE hLinMon,
                                            IN BOOL   fEnable );

//////////////////////////////////////////////////////////////////////////
/**
  This function retrieves the next LIN message from the receive FIFO of
  the specified monitor. The function does not wait for a message to be
  received from the LIN bus.

  @param hLinMon
    Handle to the LIN monitor.
  @param pLinMsg
    Pointer to a LINMSG structure where the function stores the retrieved LIN 
    message. If this parameter is set to NULL, the function simply removes the 
    next LIN message from the receive FIFO.

  @return
    VCI_OK 
      on success
    VCI_E_RXQUEUE_EMPTY
      if there is currently no LIN message available, 
    otherwise an other Error code
*/
//////////////////////////////////////////////////////////////////////////
EXTERN_C HRESULT VCIAPI linMonitorPeekMessage( IN  HANDLE  hLinMon,
                                               OUT PLINMSG pLinMsg );

//////////////////////////////////////////////////////////////////////////
/**
  This function waits until a LIN message is received from the LIN bus or the 
  time-out interval elapses.

  @param hLinMon
    Handle to the LIN monitor.
  @param dwMsTimeout
    Time-out interval, in milliseconds. The function returns if the interval 
    elapses, even if no message is received from the LIN bus. If this parameter 
    is zero, the function tests if a message is available and returns 
    immediately. If this parameter is INFINITE (0xFFFFFFFF), the function's
    time-out interval never elapses.

  @return
    VCI_OK on success, otherwise an Error code
*/
//////////////////////////////////////////////////////////////////////////
EXTERN_C HRESULT VCIAPI linMonitorWaitRxEvent( IN HANDLE hLinMon,
                                               IN UINT32 dwMsTimeout );

//////////////////////////////////////////////////////////////////////////
/**
  This function retrieves the next LIN message from the receive FIFO of
  the specified LIN monitor. The function waits for a message to be
  received from the LIN bus.

  @param hLinMon
    Handle to the LIN monitor.
  @param dwMsTimeout
    Time-out interval, in milliseconds. The function returns if the interval 
    elapses, even if no message is received from the LIN bus. If this parameter 
    is zero, the function tests if a message is available and returns 
    immediately. If this parameter is INFINITE (0xFFFFFFFF), the function's
    time-out interval never elapses.
  @param pLinMsg
    Pointer to a LINMSG structure where the function stores the retrieved LIN 
    message. If this parameter is set to NULL, the function simply removes the 
    next LIN message from the FIFO.

  @return
    VCI_OK 
      on success, 
    VCI_E_TIMEOUT 
      if the time-out interval elapses, 
    VCI_E_RXQUEUE_EMPTY
      If there is currently LIN message available
    otherwise an other Error code
*/
//////////////////////////////////////////////////////////////////////////
EXTERN_C HRESULT VCIAPI linMonitorReadMessage( IN  HANDLE  hLinMon,
                                               IN  UINT32  dwMsTimeout,
                                               OUT PLINMSG pLinMsg );


#endif //_VCINPL_H_
