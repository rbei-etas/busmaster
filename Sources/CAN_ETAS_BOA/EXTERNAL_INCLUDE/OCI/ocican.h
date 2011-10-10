#ifndef ETAS_OCI_CAN_H
#define ETAS_OCI_CAN_H

/**
* @file       ocican.h
* @brief      CAN specific part of the Open Contoller Interface (OCI) API.
* @copyright  Copyright (c) 2007-2008 ETAS GmbH. All rights reserved.
*
* $Revision: 4636 $
*/


#include "ocicanio.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
* Internal identifier for the OCI CAN interface.
*/

#define UUID_OCICAN {0xA6,0xCF,0x48,0x01,0x13,0x8F,0x44,0x3d,0x9C,0x68,0xAD,0x5D,0xE3,0x3A,0xBD,0xF9}

/**
* @addtogroup GROUP_OCI_CAN_CONTROLLER OCI CAN
* @brief      CAN specific part of the Open Controller Interface (OCI).
* @details    The CAN controller interface instance supports a single
*             TxQueue and a single RxQueue. All CAN frames will be
*             queued and transmitted in the same order as they are
*             delivered via @ref OCI_WriteCANData() to the TxQueue.
* @{
*/

/**
* @addtogroup GROUP_OCI_CAN_INITIALIZATION Initialization
* @{
*/

/**
* @anchor  OCI_ANCHOR_CAN_SAMPLES_PER_BIT_VALUES
* @name    OCI CAN configuration samples per bit values.
* @brief   Values to specify the CAN controller configuration field samples per
*          bit. These are possible values of
*          @ref OCI_CANConfiguration.samplesPerBit.
* @{ */

/**
* Use one sample per bit
*/
#define     OCI_CAN_ONE_SAMPLE_PER_BIT          (1u)

/**
*  Use three samples per bit.
*  @note  Some CAN controllers do not support three samples per bit.
*         In this case the default value of one sample per bit is
*         used, even if the configuration is set to
*         @ref OCI_CAN_THREE_SAMPLES_PER_BIT.
*/
#define     OCI_CAN_THREE_SAMPLES_PER_BIT       (2u)

/** @} */


/**
* @anchor  OCI_ANCHOR_CAN_SYNC_EDGE_VALUES
* @name    OCI CAN configuration values for the synchronization edge.
* @brief   Values to specify the synchronization edge. These are
*          possible values of @ref OCI_CANConfiguration.syncEdge.
*
* @{ */

/**
 * Synchronization single edge
 *  ISO Can specification defines the use of a single
 *  edge synchronization. The synchronization should be
 *  done on recessive to dominant level change.
 */
#define     OCI_CAN_SINGLE_SYNC_EDGE            (1u)

/**
 * Synchronization dual edge
 * Additionally to the ISO CAN specification required
 * recessive to dominant level change the dominant to
 * recessive level change could be used.
 */
#define     OCI_CAN_DUAL_SYNC_EDGE           (2u)

/** @} */


/**
* @anchor  OCI_ANCHOR_CAN_MEDIA_TYPE
* @name    OCI CAN configuration values for the media type.
* @brief   Values to specify the CAN configuration field media
*          type. These are possible values of
*          @ref OCI_CANConfiguration.physicalMedia.
*
* @{ */

/** Physical media is high speed CAN */
#define     OCI_CAN_MEDIA_HIGH_SPEED            (1u)

/* Physical media is fault tolerant CAN */
#define     OCI_CAN_MEDIA_FAULT_TOLERANT        (2u)

/** @} */


/**
* Structure containing the configuration for generic CAN controllers.
*/

typedef struct OCI_CANConfiguration
{
    /** Baudrate in Hz. */
    uint32 baudrate;

    /** Sample point in percent (0 .. 100). */
    uint32 samplePoint;

    /**
    * Number of samples per bit (1 or 3).
    * @note        Some CAN controllers do not support three samples per bit.
    * @values      The following values are valid:
    * @n           OCI_CAN_ONE_SAMPLE_PER_BIT
    * @n           OCI_CAN_THREE_SAMPLES_PER_BIT
    * @sa          @ref OCI_ANCHOR_CAN_SAMPLES_PER_BIT_VALUES
    *              "OCI CAN SamplesPerBit values"
    */
    uint32 samplesPerBit;

    /** BTL cycles*/
    uint32 BTL_Cycles;

    /** Length of the sync segment.  */
    uint32 SJW;

    /**
    * Synchronisation edge.
    * @values      The following values are valid:
    * @n           OCI_CAN_SINGLE_SYNC_EDGE
    * @n           OCI_CAN_DUAL_SYNC_EDGE
    * @sa          @ref OCI_ANCHOR_CAN_SYNC_EDGE_VALUES
    *              "OCI CAN synchronisation edge values"
    */
    uint32 syncEdge;

    /**
    * Specify the CAN physical media to use.
    * @note        Some implementations support only one kind of media.
    *              Selecting a media which is not supported by the
    *              implementation results in a configuration error.
    * @values      The following values are valid:
    * @n           OCI_CAN_MEDIA_HIGH_SPEED
    * @n           OCI_CAN_MEDIA_FAULT_TOLERANT
    * @sa          @ref OCI_ANCHOR_CAN_MEDIA_TYPE
    *              "OCI CAN media type"
    */
    uint32 physicalMedia;

    /**
    * Specify whether the CAN controller / CAN controller firmware
    * shall receive data sent through the same port.
    * @note        This flag will be AND-gated with the respective
    *              Rx queue setting.
    * @values      The following values are valid:
    * @n           OCI_SELF_RECEPTION_OFF
    * @n           OCI_SELF_RECEPTION_ON
    * @sa          @ref OCI_SelfReceptionMode
    *              "OCI Self Reception Mode"
    */
    OCI_SelfReceptionMode selfReceptionMode;
} OCI_CANConfiguration;



/**
* Specific structure for CAN controller properties.
*/

typedef struct OCI_CANControllerProperties
{
    /**
    * Specifies if the controller interface instance is suspended or running.
    * @values      The following values are valid:
    * @n           @ref OCI_CONTROLLER_MODE_RUNNING
    * @n           @ref OCI_CONTROLLER_MODE_SUSPENDED
    * @sa          @ref OCI_ControllerPropertiesMode
    *              "OCI Controller property mode values"
    */
    OCI_ControllerPropertiesMode mode;

} OCI_CANControllerProperties;


/**
  Create a new CAN controller instance and bind it to the hardware identified by an URI location.

  This is the API used by the application to bind a specific controller. The application can use information
  delivered by the CSI component to search for accessible controller. The CSI component will transform the
  (user friendly) URI-Information from this call into a (system friendly) protocol stack. Any driver needs only
  to implement the CreateCANControllerBind()-API call.

  @controllerState This function creates a new controller instance and initializes it to the CREATED state.

  @param[in]  uriLocation  URI compatible name which identifies the controller.

  @param[out] controller   Handle to the created controller interface instance. This handle must be used in
                           any subsequent call to access the controller.

  @return
  <TABLE border="0" rules="rows" cellspacing="3" cellpadding="3">
   <tr> <td> @ref OCI_SUCCESS </td>
        <td> The operation was successful. The @a controller handle is valid and can be used in further calls
             to the API. </td>
   </tr>
   <tr> <td> @ref OCI_ERR_HW_NOT_PRESENT </td>
        <td> The operation failed because the controller could not be found. Probably the hardware is not
             connected. </td>
   </tr>
   <tr> <td> @ref OCI_ERR_INVALID_STATE </td>
        <td> The application has already bound the controller at this location. A second binding is not possible.
             Use the existing handle to access the controller. </td>
   </tr>
   <tr> <td> @ref OCI_ERR_NO_RESOURCES </td>
        <td> The driver or CSI component run into a resource problem and can not instantiate a new handle. Destroy
             some unneeded handles to release resources and try again. </td>
   </tr>
   <tr> <td> @ref OCI_ERR_DRIVER_NO_RESPONSE </td>
        <td> The driver for a removable hardware received no response to a request. The hardware may be removed.
             It is not known wether the request is lost and the command not exectuted or the response is lost and
             the command was executed. The controller enters the @a DISCONNECTED state. All subsequent calls will
             return @ref OCI_ERR_DRIVER_DISCONNECTED.
             Internal resources are locked until OCI_DestroyCANController() for the controller is called. </td>
   </tr>
   <tr> <td> @ref OCI_ERR_DRIVER_DISCONNECTED </td>
        <td> The driver for a removable hardware detected a communication problem (in asynchromous communication,
             by event signalling or during a previous call to the same hardware) and is not able to communicate to
             the hardware any more.
             The state of the controller is @a DISCONNECTED. The command is not executed. All subsequent calls to
             the same Controller will return @ref OCI_ERR_DRIVER_DISCONNECTED. Internal resources are locked until
             OCI_DestroyCANController() for the controller is called.  </td>
   </tr>
   <tr> <td> @ref OCI_ERR_OUT_OF_MEMORY </td>
        <td> The driver run out of memory. This error should only occur on heavy loaded systems with multiple
             clients and large send and receive queues. The error may indicate a resource leak in the driver
             implementation. When multiple application access the same resource (interface hardware or physical
             controller) the termination of a different application may return the resources required for this
             instance. </td>
   </tr>
  </TABLE>
  @sa         GROUP_OCI_CONTROLLER, @ref PAGE_BOA_ADDRESSINGSCHEME "Addressing scheme of the Basic OpenAPI"

  @todo       Document the syntax of the uriLocation parameter.

*/

#ifndef OCI_DYNAMICDLL
OCI_DECLSPEC OCI_ErrorCode
OCI_CALL OCI_CreateCANController(
    const char* uriLocation,
    OCI_ControllerHandle* controller);
#endif

typedef OCI_ErrorCode
(OCI_CALL *PF_OCI_CreateCANController)(
    const char* uriLocation,
    OCI_ControllerHandle* controller);


/**
  Create a new CAN controller instance and bind it to a hardware identified by a protocol stack.

  This is the API implemented by the driver to bind a specific controller. The application can use information
  delivered by the CSI component to search for accessible controller. The CSI component will transform the
  (user friendly) URI-Information from this call into a (system friendly) protocol stack. Any driver needs only
  to implement the OCI_CreateCANControllerBind()-API call.

  @controllerState This function creates a new controller instance and initializes it to the CREATED state.

  @param[in]  protocolStack  The access parameters required to bind to the appropriate controller instance.

  @param[out] controller     The handle to the created controller interface instance. This handle must be used in
                             any subsequent call to access the controller.

  @return
  <TABLE border="0" rules="rows" cellspacing="3" cellpadding="3">
   <tr> <td> @ref OCI_SUCCESS </td>
        <td> The operation was successful. The controller handle is valid and can be used in further calls
             to the API. </td>
   </tr>
   <tr> <td> @ref OCI_ERR_HW_NOT_PRESENT </td>
        <td> The operation failed because the controller could not be found. Probably the hardware is not
             connected. </td>
   </tr>
   <tr> <td> @ref OCI_ERR_INVALID_STATE </td>
        <td> The application has already bound the controller at this location. A second binding is not possible.
             Use the existing handle to access the controller. </td>
   </tr>
   <tr> <td> @ref OCI_ERR_NO_RESOURCES </td>
        <td> The driver or CSI component run into a resource problem and can not instantiate a new handle. Destroy
             some unneeded handles to release resources and try again. </td>
   </tr>
   <tr> <td> @ref OCI_ERR_DRIVER_NO_RESPONSE </td>
        <td> The driver for a removable hardware received no response to a request. The hardware may be removed.
             It is not known wether the request is lost and the command not exectuted or the response is lost and
             the command was executed. The controller enters the @a DISCONNECTED state. All subsequent calls will
             return @ref OCI_ERR_DRIVER_DISCONNECTED.
             Internal resources are locked until OCI_DestroyCANController() for the controller is called. </td>
   </tr>
   <tr> <td> @ref OCI_ERR_DRIVER_DISCONNECTED </td>
        <td> The driver for a removable hardware detected a communication problem (in asynchromous communication,
             by event signalling or during a previous call to the same hardware) and is not able to communicate to
             the hardware any more.
             The state of the controller is @a DISCONNECTED. The command is not executed. All subsequent calls to
             the same Controller will return @ref OCI_ERR_DRIVER_DISCONNECTED. Internal resources are locked until
             OCI_DestroyCANController() for the controller is called.  </td>
   </tr>
   <tr> <td> @ref OCI_ERR_OUT_OF_MEMORY </td>
        <td> The driver run out of memory. This error should only occur on heavy loaded systems with multiple
             clients and large send and receive queues. The error may indicate a resource leak in the driver
             implementation. When multiple application access the same resource (interface hardware or physical
             controller) the termination of a different application may return the resources required for this
             instance. </td>
   </tr>
   <tr> <td> @ref OCI_ERR_PROTOCOL_VERSION_NOT_SUPPORTED </td>
        <td> The binding of the protocol stack failed, because a version check of the requested API or some
             intermediate API in the protocol stack detected some incompatibilities. This is a problem of the
             installed software. A different driver version or a generic version adapter is required to access
             the controller. </td>
   </tr>
   <tr> <td> @ref OCI_ERR_INVALID_ACCESS_SYNTAX </td>
        <td> the parsing of the access parameter failed. The protocol driver or some intermediate driver can not
             interpret the parameter of the protcolStack. This error should not occur, when the protocol stack was
             build by the CSI component and the CSI-services in the protocol driver are implemented in a correct
             way. </td>
   </tr>
   <tr> <td> @ref OCI_ERR_INVALID_ACCESS_PARAM </td>
        <td> The driver or an intermediate layer could not be found. An element of the protocol stack can not be
             resolved. The connection can not be established. This error should not occur, when the protocol stack
             was build by the CSI component and the CSI-services in the protocol driver are implemented in a
             correct way. </td>
   </tr>
   <tr> <td> @ref OCI_ERR_INVALID_TRANSFER_SYNTAX </td>
        <td> The driver can not access the requested interface. There is a server offering the interface with the
             requested UUID, but the client and server offer only incompatible binding mechanism. This may be a
             software installation problem and some intermediate driver, that maps different binding mechanism may
             be missing.
             This error should not occur, when the protocol stack was build by the CSI component and the
             CSI-services in the protocol driver are implemented in a correct way. </td>
   </tr>
  </TABLE>

  @sa         @ref GROUP_OCI_CONTROLLER "Controller Handling"

*/

#ifndef OCI_DYNAMICDLL
OCI_DECLSPEC OCI_ErrorCode
OCI_CALL OCI_CreateCANControllerBind(
    const BOA_ProtocolStack* protocolStack,
    OCI_ControllerHandle* controller);
#endif

typedef OCI_ErrorCode
(OCI_CALL *PF_OCI_CreateCANControllerBind)(
    const BOA_ProtocolStack* protocolStack,
    OCI_ControllerHandle* controller);


/**
  Query the configuration of the CAN controller.

  This call can be executed in any state of the controller instance. Hardware that supports more than one
  logical controller instance on a single physical interface delivers the configuration of the physical
  controller. When the controller instance is in the @a RUNNING or @a SUSPENDED mode, the data will be
  compatible to the configuration set by OCI_OpenCANController(). When this instance is in the @a CREATED state
  and a different controller instance has already configured the hardware (via OCI_OpenCANController()) the
  configuration used by that instance is returned. When no logical instance accesses the physical controller
  the call fails and delivers @ref OCI_ERR_NO_CONFIG.

  This call can be used to attach a second application to a single physical controller.

  @controllerState CREATED, SUSPENDED, RUNNING.

  @param[in]   controller      Handle of the CAN controller instance that is queried.

  @param[out]  configuration   Pointer to the buffer that will receive the configuration structure.
                               Must not be @c NULL.

  @return
  <TABLE border="0" rules="rows" cellspacing="3" cellpadding="3">
   <tr> <td> @ref OCI_SUCCESS </td>
        <td> The operation was successful. The @a configuration is filled with valid information.</td>
   </tr>
   <tr> <td> @ref OCI_ERR_INVALID_HANDLE </td>
        <td> The @a controller handle is not valid. </td>
   </tr>
   <tr> <td> @ref OCI_ERR_INVALID_PARAMETER </td>
        <td> The @a configuration paramter is NULL or references an invalid address.</td>
   </tr>
   <tr> <td> @ref OCI_ERR_NO_CONFIG </td>
        <td> There is currently no instance accessing the CAN controller and the controller uses no vaild
             configuration. </td>
   </tr>
   <tr> <td> @ref OCI_ERR_DRIVER_NO_RESPONSE </td>
        <td> The driver for a removable hardware received no response to a request. The hardware may be removed.
             It is not known wether the request is lost and the command not exectuted or the response is lost and
             the command was executed. The controller enters the @a DISCONNECTED state. All subsequent calls will
             return @ref OCI_ERR_DRIVER_DISCONNECTED.
             Internal resources are locked until OCI_DestroyCANController() for the controller is called. </td>
   </tr>
   <tr> <td> @ref OCI_ERR_DRIVER_DISCONNECTED </td>
        <td> The driver for a removable hardware detected a communication problem (in asynchromous communication,
             by event signalling or during a previous call to the same hardware) and is not able to communicate to
             the hardware any more.
             The state of the controller is @a DISCONNECTED. The command is not executed. All subsequent calls to
             the same Controller will return @ref OCI_ERR_DRIVER_DISCONNECTED. Internal resources are locked until
             OCI_DestroyCANController() for the controller is called.  </td>
   </tr>
   <tr> <td> @ref OCI_ERR_OUT_OF_MEMORY </td>
        <td> The driver run out of memory. This error should only occur on heavy loaded systems with multiple
             clients and large send and receive queues. The error may indicate a resource leak in the driver
             implementation. When multiple application access the same resource (interface hardware or physical
             controller) the termination of a different application may return the resources required for this
             instance. </td>
   </tr>
   <tr> <td> @ref OCI_ERR_UNEXPECTED_NULL </td>
        <td> An unexpected implementation error occured. </td>
   </tr>
  </TABLE>
*/

#ifndef OCI_DYNAMICDLL
OCI_DECLSPEC OCI_ErrorCode
OCI_CALL OCI_GetCANConfiguration(
    OCI_ControllerHandle controller,
    OCI_CANConfiguration* configuration);
#endif

typedef OCI_ErrorCode
(OCI_CALL *PF_OCI_GetCANConfiguration)(
    OCI_ControllerHandle controller,
    OCI_CANConfiguration* configuration);


/**
  Open and configure (activate / begin use of) a CAN controller instance.

  The configuration provided by this call must either be compatible with the setup currently used by the
  physical controller or this instance must be the first logical controller instance that opens the physical
  controller.

  The configuration can only be set once with this call while the controller is in the @a CREATED state. The
  configuration is used and locked until OCI_CloseCANController() (or OCI_DestroyCANController()) for this
  controller instance is called.

  The controller may be opened in the @a SUSPENDED mode or @a RUNNING mode. In the @a SUSPENDED mode the
  controller will not take part in the bus communication. It is possible to configure filter and queues with
  multiple subsequent API calls. Switching the controller to the @a RUNNING mode by calling
  OCI_SetCANControllerProperties() will activate all settings at the same time when the controller is activated.
  In the @a RUNNING mode the controller will immideatly take part in the bus communication. Subsequent API calls
  will activate queues and filter step by step.

  @controllerState CREATED. \n
                   A transition to the RUNNING or SUSPENDED state will be executed
                   (depends on the parameter @a properties).

  @param[in]  controller     Handle of the CAN controller instance that will be opened.

  @param[in]  configuration  Pointer to the buffer that contains the configuration for the controller.

  @param[in]  properties     Pointer to a buffer that contains the properties for the controller.

  @return
  <TABLE border="0" rules="rows" cellspacing="3" cellpadding="3">
   <tr> <td> @ref OCI_SUCCESS </td>
        <td> The operation was successful. </td>
   </tr>
   <tr> <td> @ref OCI_WARN_PARAM_ADAPTED </td>
        <td> The driver is not able to use the exact settings of the parameter @a configuration. Because of some
             hardware limitations some values have been adapted. The driver expects a compatible behaviour as
             if the requested settings were used. The application can continue normal operation. The application
             may read the configuration by OCI_GetCANConfiguration() to identify the differences of the
             requested and used parameter. </td>
   </tr>
   <tr> <td> @ref OCI_ERR_INVALID_HANDLE </td>
        <td> The handle is not valid. </td>
   </tr>
   <tr> <td> @ref OCI_ERR_INVALID_STATE </td>
        <td> The controller was already opened. The controller is already in the @a RUNNING or @a SUSPENDED state and
             can not be opened. </td>
   </tr>
   <tr> <td> @ref OCI_ERR_DRIVER_NO_RESPONSE </td>
        <td> The driver for a removable hardware received no response to a request. The hardware may be removed.
             It is not known wether the request is lost and the command not exectuted or the response is lost and
             the command was executed. The controller enters the @a DISCONNECTED state. All subsequent calls will
             return @ref OCI_ERR_DRIVER_DISCONNECTED.
             Internal resources are locked until OCI_DestroyCANController() for the controller is called. </td>
   </tr>
   <tr> <td> @ref OCI_ERR_DRIVER_DISCONNECTED </td>
        <td> The driver for a removable hardware detected a communication problem (in asynchromous communication,
             by event signalling or during a previous call to the same hardware) and is not able to communicate to
             the hardware any more.
             The state of the controller is @a DISCONNECTED. The command is not executed. All subsequent calls to
             the same Controller will return @ref OCI_ERR_DRIVER_DISCONNECTED. Internal resources are locked until
             OCI_DestroyCANController() for the controller is called.  </td>
   </tr>
   <tr> <td> @ref OCI_ERR_OUT_OF_MEMORY </td>
        <td> The driver run out of memory. This error should only occur on heavy loaded systems with multiple
             clients and large send and receive queues. The error may indicate a resource leak in the driver
             implementation. When multiple application access the same resource (interface hardware or physical
             controller) the termination of a different application may return the resources required for this
             instance. </td>
   </tr>
   <tr> <td> @ref OCI_ERR_UNEXPECTED_NULL </td>
        <td> An unexpected implementation error occured. </td>
   </tr>
   <tr> <td> @ref OCI_ERR_HW_NOT_READY </td>
        <td> An internal hardware error occured (e.g. sub CPU hasn't booted, hardware registers or memory
             isn't accessible) </td>
   </tr>
  </TABLE>

  @sa         @ref GROUP_OCI_CONTROLLER "Controller Handling"
*/

#ifndef OCI_DYNAMICDLL
OCI_DECLSPEC OCI_ErrorCode
OCI_CALL OCI_OpenCANController(
    OCI_ControllerHandle controller,
    OCI_CANConfiguration* configuration,
    OCI_CANControllerProperties* properties);
#endif

typedef OCI_ErrorCode
(OCI_CALL *PF_OCI_OpenCANController)(
    OCI_ControllerHandle controller,
    OCI_CANConfiguration* configuration,
    OCI_CANControllerProperties* properties);


/**
  Query the properties of the CAN controller instance.

  Properties are read/writeable and can be changed using OCI_SetCANControllerProperties().

  These properties represent the locigal state of this controller instance. In the @a SUSPENDED state the
  controller instance will neither send any data on the interface nor receive any data. In the @a RUNNING
  state the controller instance will send data, when the transmit queue is not empty and receive any data
  that passes a configured filter condition.

  In a hardware that supports multiple instances on a single physical interface the state of the physical
  controller will be @a RUNNING until the last logical instance suspends the logical state.

  @controllerState CREATED, RUNNING, SUSPENDED.

  @param[in]  controller  Handle of the CAN controller instance that is queried.

  @param[out] properties  Pointer to a buffer that will receive the controller interface instance properties.
                          Must not be @c NULL.

  @return
  <TABLE border="0" rules="rows" cellspacing="3" cellpadding="3">
   <tr> <td> @ref OCI_SUCCESS </td>
        <td> The operation was successful. The @a properties are filled with valid information. </td>
   </tr>
   <tr> <td> @ref OCI_ERR_INVALID_HANDLE </td>
        <td> The @a controller handle is not valid. </td>
   </tr>
   <tr> <td> @ref OCI_ERR_INVALID_PARAMETER </td>
        <td> The properties paramter is NULL or references an invalid address.</td>
   </tr>
   <tr> <td> @ref OCI_ERR_DRIVER_NO_RESPONSE </td>
        <td> The driver for a removable hardware received no response to a request. The hardware may be removed.
             It is not known wether the request is lost and the command not exectuted or the response is lost and
             the command was executed. The controller enters the @a DISCONNECTED state. All subsequent calls will
             return @ref OCI_ERR_DRIVER_DISCONNECTED.
             Internal resources are locked until OCI_DestroyCANController() for the controller is called. </td>
   </tr>
   <tr> <td> @ref OCI_ERR_DRIVER_DISCONNECTED </td>
        <td> The driver for a removable hardware detected a communication problem (in asynchromous communication,
             by event signalling or during a previous call to the same hardware) and is not able to communicate to
             the hardware any more.
             The state of the controller is @a DISCONNECTED. The command is not executed. All subsequent calls to
             the same Controller will return @ref OCI_ERR_DRIVER_DISCONNECTED. Internal resources are locked until
             OCI_DestroyCANController() for the controller is called.  </td>
   </tr>
   <tr> <td> @ref OCI_ERR_OUT_OF_MEMORY </td>
        <td> The driver run out of memory. This error should only occur on heavy loaded systems with multiple
             clients and large send and receive queues. The error may indicate a resource leak in the driver
             implementation. When multiple application access the same resource (interface hardware or physical
             controller) the termination of a different application may return the resources required for this
             instance. </td>
   </tr>
  </TABLE>

  @sa         @ref GROUP_OCI_CONTROLLER "Controller Handling"
*/

#ifndef OCI_DYNAMICDLL
OCI_DECLSPEC OCI_ErrorCode
OCI_CALL OCI_GetCANControllerProperties(
    OCI_ControllerHandle controller,
    OCI_CANControllerProperties *properties);
#endif

typedef OCI_ErrorCode
(OCI_CALL *PF_OCI_GetCANControllerProperties)(
    OCI_ControllerHandle controller,
    OCI_CANControllerProperties *properties);


/**
  Set the CAN controller instance properties (suspended / running) of the CAN controller instance.

  Properties are read/writeable and can be queried OCI_GetCANControllerProperties().

  These properties are the locigal state of this controller instance.
  @li In the @a SUSPENDED state the controller instance will neither send any data on the interface nor receive
      any data.
  @li In the @a RUNNING state the controller instance will send data, when the transmit queue is not empty and
      receive any data that passes a configured filter condition.

  In a hardware that supports multiple instances on a single physical interface the state of the physical
  controller will be @a RUNNING until the last instance suspends the logical state.

  The suspended state is useful to configure a set of filter and activate the complete new set at the
  same time. When the filter are set by (especially a sequence of different) API-calls intermediate states
  exist where some filter already active and other not. This may be critical for specific applications.

  The @a properties parameter describes the controller state after successful execution of the command. When
  the controller is already in the desired state the driver will ignore the command and return @ref OCI_SUCCESS.

  @controllerState RUNNING, SUSPENDED. \n
                   A transition to the RUNNING or SUSPENDED state may be executed (depends on the parameter).

  @param[in]  controller  Handle of the CAN controller instance for which the properties are set.

  @param[in]  properties  CAN controller interface instance properties to set.
                          Must not be @c NULL.

  @return
  <TABLE border="0" rules="rows" cellspacing="3" cellpadding="3">
   <tr> <td> @ref OCI_SUCCESS </td>
        <td> The operation was successful. The properties are used by the controller instance. </td>
   </tr>
   <tr> <td> @ref OCI_ERR_INVALID_HANDLE </td>
        <td> The @a controller handle is not valid. </td>
   </tr>
   <tr> <td> @ref OCI_ERR_INVALID_PARAMETER </td>
        <td> The properties paramter is NULL or references an invalid address.</td>
   </tr>
   <tr> <td> @ref OCI_ERR_INVALID_STATE </td>
        <td> The controller is not in the @a RUNNING or @a SUSPENDED state. When the controller is in the
             @a CREATED state additional configuration information is required and OCI_OpenCANController() must
             be used. </td>
   </tr>
   <tr> <td> @ref OCI_ERR_DRIVER_NO_RESPONSE </td>
        <td> The driver for a removable hardware received no response to a request. The hardware may be removed.
             It is not known wether the request is lost and the command not exectuted or the response is lost and
             the command was executed. The controller enters the @a DISCONNECTED state. All subsequent calls will
             return @ref OCI_ERR_DRIVER_DISCONNECTED.
             Internal resources are locked until OCI_DestroyCANController() for the controller is called. </td>
   </tr>
   <tr> <td> @ref OCI_ERR_DRIVER_DISCONNECTED </td>
        <td> The driver for a removable hardware detected a communication problem (in asynchromous communication,
             by event signalling or during a previous call to the same hardware) and is not able to communicate to
             the hardware any more.
             The state of the controller is @a DISCONNECTED. The command is not executed. All subsequent calls to
             the same Controller will return @ref OCI_ERR_DRIVER_DISCONNECTED. Internal resources are locked until
             OCI_DestroyCANController() for the controller is called.  </td>
   </tr>
   <tr> <td> @ref OCI_ERR_OUT_OF_MEMORY </td>
        <td> The driver run out of memory. This error should only occur on heavy loaded systems with multiple
             clients and large send and receive queues. The error may indicate a resource leak in the driver
             implementation. When multiple application access the same resource (interface hardware or physical
             controller) the termination of a different application may return the resources required for this
             instance. </td>
   </tr>
  </TABLE>

  @sa         @ref GROUP_OCI_CONTROLLER "Controller Handling"
*/

#ifndef OCI_DYNAMICDLL
OCI_DECLSPEC OCI_ErrorCode
OCI_CALL OCI_SetCANControllerProperties(
    OCI_ControllerHandle controller,
    OCI_CANControllerProperties *properties);
#endif

typedef OCI_ErrorCode
(OCI_CALL *PF_OCI_SetCANControllerProperties)(
    OCI_ControllerHandle controller,
    OCI_CANControllerProperties *properties);

/**
  Close and unlock the configuration of a CAN controller instance.

  All queues and filters for this instance are implicitly destroyed with this call. It is undefined if data
  in transmit queues is flushed or discarded. When this call returns, all resources associated with this instance
  have been cleaned up and the instance stopped transmitting data on the bus.

  The state of the controller instance is moved to @a CREATED (see also @ref GROUP_OCI_CONTROLLER). In a hardware
  that supports multiple instances on a single physical interface the configuration may be changed by a different
  instance by calling OCI_OpenCANController() in tis state.

  @controllerState RUNNING, SUSPENDED. \n
                   A transition to the CREATED state is executed.  \n
                   When the controller is in the DISCONNECTED state the function may be called to do local clean
                   up. Only a limitied plausibility check may be done in this case.


  @param[in]  controller    Handle of the controller instance that will be closed.

  @return
  <TABLE border="0" rules="rows" cellspacing="3" cellpadding="3">
   <tr> <td> @ref OCI_SUCCESS </td>
        <td> The operation was successful. The controller is closed and the state moved to @a CREATED. </td>
   </tr>
   <tr> <td> @ref OCI_ERR_INVALID_HANDLE </td>
        <td> The @a controller handle is not valid. </td>
   </tr>
   <tr> <td> @ref OCI_ERR_INVALID_STATE </td>
        <td> The controller was not opened. The controller is not in the @a RUNNING or @a SUSPENDED state and
             can not be closed. </td>
   </tr>
   <tr> <td> @ref OCI_ERR_OUT_OF_MEMORY </td>
        <td> The driver run out of memory. This error should only occur on heavy loaded systems with multiple
             clients and large send and receive queues. The error may indicate a resource leak in the driver
             implementation. When multiple application access the same resource (interface hardware or physical
             controller) the termination of a different application may return the resources required for this
             instance. </td>
   </tr>
  </TABLE>

  @sa         @ref GROUP_OCI_CONTROLLER "Controller Handling"
*/

#ifndef OCI_DYNAMICDLL
OCI_DECLSPEC OCI_ErrorCode
OCI_CALL OCI_CloseCANController(
    OCI_ControllerHandle controller);
#endif

typedef OCI_ErrorCode
(OCI_CALL *PF_OCI_CloseCANController)(
    OCI_ControllerHandle controller);


/**
  Destroy an interface instance of a CAN controller.

  This function destroys the access to a CAN controller instance. It implicitly releases all resources
  (queues, filter). When the controller is in the @a RUNNING or @a SUSPENDED state the Controller configuration is
  unlocked (see also OCI_CloseCANController()).
  After this call the handle @a controller is invalid and must not be used any more.

  @controllerState CREATED, RUNNING, SUSPENDED, DISCONNECTED. \n
                   The controller instance is destroyed and not valid any longer.

  @param[in]  controller  Handle of the CAN controller instance that will be destroyed.

  @return
  <TABLE border="0" rules="rows" cellspacing="3" cellpadding="3">
   <tr> <td> @ref OCI_SUCCESS </td>
        <td> The operation was successful. The @a controller handle must not be used any more. </td>
   </tr>
   <tr> <td> @ref OCI_ERR_INVALID_HANDLE </td>
        <td> The @a controller handle is not valid. </td>
   </tr>
   <tr> <td> @ref OCI_ERR_OUT_OF_MEMORY </td>
        <td> The driver run out of memory. This error should only occur on heavy loaded systems with multiple
             clients and large send and receive queues. The error may indicate a resource leak in the driver
             implementation. When multiple application access the same resource (interface hardware or physical
             controller) the termination of a different application may return the resources required for this
             instance. </td>
   </tr>
  </TABLE>

  @sa         @ref GROUP_OCI_CONTROLLER "Controller Handling"
*/
#ifndef OCI_DYNAMICDLL
OCI_DECLSPEC OCI_ErrorCode
OCI_CALL OCI_DestroyCANController(
    OCI_ControllerHandle controller);
#endif

typedef OCI_ErrorCode
(OCI_CALL *PF_OCI_DestroyCANController)(
    OCI_ControllerHandle controller);

/** @} */


/**
* @addtogroup GROUP_OCI_CAN_INTERNAL Internal Structures
* @brief      Additional definitions relevant for OCI implementors.
* @{
*/

/**
* Internal v-table for the OCI CAN interface.
*/

typedef struct OCI_CAN_VTable
{
    /* ocitime.h */

    OCI_Time_VTable timeVTable;

    /* ocierror.h */

    OCI_Error_VTable errorVTable;

    /* ociqueue.h */

    OCI_Queue_VTable queueVTable;

    /* ocicanio.h */

    OCI_CANIO_VTable canioVTable;

    /* ocican.h */

    PF_OCI_CreateCANController createCANController;
    PF_OCI_CreateCANControllerBind createCANControllerBind;
    PF_OCI_DestroyCANController destroyCANController;

    PF_OCI_OpenCANController openCANController;
    PF_OCI_CloseCANController closeCANController;
    PF_OCI_GetCANConfiguration getCANConfiguration;

    PF_OCI_GetCANControllerProperties getCANControllerProperties;
    PF_OCI_SetCANControllerProperties setCANControllerProperties;

    PF_OCI_GetUnmappedHandle getUnmappedHandle;

} OCI_CAN_VTable;

/** @} */

/** @} */

#ifdef __cplusplus
}
#endif

#endif
