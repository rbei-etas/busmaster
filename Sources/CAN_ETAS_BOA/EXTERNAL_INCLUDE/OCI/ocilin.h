#ifndef ETAS_OCI_LIN_H
#define ETAS_OCI_LIN_H

/**
* @file       ocilin.h
* @brief      LIN specific part of the Open Contoller Interface (OCI) API.
* @copyright  Copyright (c) 2007-2008 ETAS GmbH. All rights reserved.
*
* $Revision: 4636 $
*/


#include "ocilinio.h"

#ifdef __cplusplus
extern "C" {
#endif


/**
* @addtogroup GROUP_OCI_LIN_CONTROLLER OCI LIN
* @brief      LIN specific part of the Open Controller Interface (OCI). 
* @{
*/

/**
* @addtogroup GROUP_OCI_LIN_INITIALIZATION Initialization
* @{
*/

/**
* Definition for validity regarding properties of a single LIN frame.
*/
typedef enum
{
    /**
    * Properties of this LIN frame are defined.
    */
    OCI_LIN_FRAME_DEFINED    = 1,

    /**
    * Properties of this LIN frame are undefined.
    */
    OCI_LIN_FRAME_UNDEFINED  = 2

} OCI_LIN_FRAME_CONFIGURATION_MODE;

/**
* Definition for constants for acting as LIN-Master of LIN-Slave.
*/
typedef enum
{
    /**
    * This controller instance acts as a master on the LIN Bus.
    */
    OCI_LIN_MASTER = 1,

    /**
    * This controller instance acts as a slave on the LIN Bus.
    */
    OCI_LIN_SLAVE  = 2

} OCI_LIN_CONTROLLER_MODE;


/**
* Definition for LIN Protocol Versions.
*/
typedef enum
{
    /**
    * LIN specification 1_3.
    */
    OCI_LIN_VERSION_1_3 = 13,

    /**
    * LIN specification 2_0.
    */
    OCI_LIN_VERSION_2_0 = 20,

    /**
    * LIN specification 2_1.
    */
    OCI_LIN_VERSION_2_1 = 21

} OCI_LIN_PROTOCOL_VERSION;

/**
* Definition for LIN Master pull-up resistor activation.
*/
typedef enum
{
    /**
    * LIN master resistor is not connected to LIN bus.
    */
    OCI_LIN_MASTER_RESISTOR_OFF = 1,

    /**
    * LIN master resistor is connected to LIN bus.
    */
    OCI_LIN_MASTER_RESISTOR_ON  = 2

} OCI_LIN_MASTER_RESISTOR;

/**
* Definition for LIN bus Ubat supply.
*/
typedef enum
{
    /**
    * LIN port supplied inernally.
    */
    OCI_LIN_UBAT_SUPPLY_INTERNAL = 1,

    /**
    * LIN port supplied externally.
    */
    OCI_LIN_UBAT_SUPPLY_EXTERNAL = 2

} OCI_LIN_UBAT_SUPPLY;

/**
* Definition for used checksum calculation.
*/
typedef enum
{
    /**
    * Classic checksum calculation (use only the data bytes).
    */
    OCI_LIN_CHECKSUM_CLASSIC    = 0,
    
    /**
    * Enhanced checksum calculation (use the data bytes including the id field).
    */
    OCI_LIN_CHECKSUM_ENHANCED   = 128,
    
    /**
    * Undefined checksum calculation.
    */
    OCI_LIN_CHECKSUM_UNDEFINED  = 255

} OCI_LIN_CHECKSUM_TYPE;

/**
* Definition for type of LIN frame.
*/
typedef enum
{
    /**
    * Frame is an unconditional LIN frame.
    */
    OCI_LIN_UNCONDITIONAL_FRAME  = 1,

    /**
    * Frame is an event triggered LIN frame.
    */
    OCI_LIN_EVENT_FRAME          = 2

} OCI_LIN_FRAME_TYPE;

/**
* LIN bus relevant properties of a LIN frame. 
*/
typedef struct OCI_LIN_PerIDConfiguration
{
    /** 
    * Configuration mode @ref OCI_LIN_FRAME_CONFIGURATION_MODE of this LIN frame.
    * Specifies the validity regarding properties of this LIN frame .   
    * @default  @ref OCI_LIN_FRAME_DEFINED
    */
    uint32 mode;

    /** 
    * Defines the type @ref OCI_LIN_FRAME_TYPE of the LIN frame.
    * A LIN Master must not use event triggered frames in send direction.
    * Check with @ref OCI_LINControllerCapabilities which values may be configured.
    * @default  @ref OCI_LIN_UNCONDITIONAL_FRAME
    */
    uint32 frameType;

    /** 
    * Checksum calculation type @ref OCI_LIN_CHECKSUM_TYPE for the LIN message. 
    * This property is only for a LIN 2.0 node. 
    * Here the checksum calculation can be changed from 
    * the classic to enhanced model for the LIN IDs 0..59. The LIN ID 
    * 60..63 range is fixed to the classic model and can not be changed. 
    * Per default always the classic model is set for all IDs. There are 
    * no changes when it is called for a LIN 1.3 node.
    * @default  @ref OCI_LIN_CHECKSUM_CLASSIC
    */
    uint32 crcType;

    /** 
    * Defines the DLC in number of bytes for the requested messages.
    * @li       1..8 - number of bytes in LIN message
    * @li       15   - number of bytes determined by LIN frame ID
    * @default  8
    */
    uint32 dlc;

} OCI_LIN_PerIDConfiguration;

/** 
* LIN bus relevant properties of a controller instance. 
*/
typedef struct OCI_LINConfiguration
{
    /** 
    * Baud rate of the HW port associated with the controller instance.    
    * @values   Valid value are 1000 to 20000. Typical values e.g.\ 9600, 19200, ...
    * @default  9600
    */
    uint32 baudrate; 

    /** 
    * LIN bus client mode @ref OCI_LIN_CONTROLLER_MODE (MASTER, SLAVE) of 
    * the HW port associated with the controller instance.   
    * Check with @ref OCI_LINControllerCapabilities which values may be configured.
    * @default  @ref OCI_LIN_SLAVE
    */
    uint32 mode;
    
    /** 
    * LIN protocol version @ref OCI_LIN_PROTOCOL_VERSION of the
    * HW port associated with the controller instance.    
    * @default  @ref OCI_LIN_VERSION_2_1
    */
    uint32 version;
    
    /** 
    * LIN master pull-up resistor activation @ref OCI_LIN_MASTER_RESISTOR.    
    * Check with @ref OCI_LINControllerCapabilities which values may be configured.
    * @default  @ref OCI_LIN_MASTER_RESISTOR_OFF
    */
    uint32 masterResistor;

    /** 
    * LIN bus Ubat supply @ref OCI_LIN_UBAT_SUPPLY.    
    * @default  @ref OCI_LIN_UBAT_SUPPLY_INTERNAL
    */
    uint32 linSupply;

    /** 
    * Configurations for all LIN messages.
    */
    OCI_LIN_PerIDConfiguration messages[64];

    /**
    * Specify whether the LIN controller shall receive data sent through 
    * the same port. Currently, this feature cannot be deactivated.
    * @values      The following values are valid:
    * @n           OCI_SELF_RECEPTION_ON
    * @sa          @ref OCI_SelfReceptionMode
    *              "OCI Self Reception Mode"
    */
    OCI_SelfReceptionMode selfReceptionMode;
} OCI_LINConfiguration;


/**
  Create a new LIN controller instance and bind it to the hardware identified by an URI location. 

  This is the API used by the application to bind a specific controller. The application can use information 
  delivered by the CSI component to search for accessible controller. The CSI component will transform the 
  (user friendly) URI-Information from this call into a (system friendly) protocol stack. Any driver needs only 
  to implement the OCI_CreateLINControllerBind()-API call. 

  @controllerState This function creates a new controller instance and initializes it to the CREATED state.

  @param[in]  uriLocation  The URI compatible name which identifies the controller. 
  
  @param[out] controller   The handle to the created controller interface instance. This handle must be used in
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
             Internal resources are locked until OCI_DestroyLINController() for the controller is called. </td> 
   </tr>
   <tr> <td> @ref OCI_ERR_DRIVER_DISCONNECTED </td>
        <td> The driver for a removable hardware detected a communication problem (in asynchromous communication, 
             by event signalling or during a previous call to the same hardware) and is not able to communicate to 
             the hardware any more. 
             The state of the controller is @a DISCONNECTED. The command is not executed. All subsequent calls to 
             the same Controller will return @ref OCI_ERR_DRIVER_DISCONNECTED. Internal resources are locked until 
             OCI_DestroyLINController() for the controller is called.  </td> 
   </tr>
   <tr> <td> @ref OCI_ERR_OUT_OF_MEMORY </td>
        <td> The driver run out of memory. This error should only occur on heavy loaded systems with multiple 
             clients and large send and receive queues. The error may indicate a resource leak in the driver 
             implementation. When multiple application access the same resource (interface hardware or physical
             controller) the termination of a different application may return the resources required for this 
             instance. </td> 
   </tr>
  </TABLE>

  @sa         @ref GROUP_OCI_CONTROLLER "Controller Handling", @ref PAGE_BOA_ADDRESSINGSCHEME "Addressing scheme of the Basic OpenAPI"

  @todo      Document the syntax of the uriLocation parameter.
*/

#ifndef OCI_DYNAMICDLL
OCI_DECLSPEC OCI_ErrorCode 
OCI_CALL OCI_CreateLINController(
    const char* uriLocation, 
    OCI_ControllerHandle* controller);
#endif

typedef OCI_ErrorCode
(OCI_CALL *PF_OCI_CreateLINController)(
    const char* uriLocation, 
    OCI_ControllerHandle* controller);


/**
  Create a new LIN controller instance. Binding parameters are specified in the form of a protocol stack.

  This is the API implemented by the driver to bind a specific controller. The CSI component will transform 
  the (user friendly) URI-Information from the CreateLINController() call into this (system friendly) 
  protocol stack. 

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
             Internal resources are locked until OCI_DestroyLINController() for the controller is called. </td> 
   </tr>
   <tr> <td> @ref OCI_ERR_DRIVER_DISCONNECTED </td>
        <td> The driver for a removable hardware detected a communication problem (in asynchromous communication, 
             by event signalling or during a previous call to the same hardware) and is not able to communicate to 
             the hardware any more. 
             The state of the controller is @a DISCONNECTED. The command is not executed. All subsequent calls to 
             the same Controller will return @ref OCI_ERR_DRIVER_DISCONNECTED. Internal resources are locked until 
             OCI_DestroyLINController() for the controller is called.  </td> 
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
OCI_CALL OCI_CreateLINControllerBind(
    const BOA_ProtocolStack* protocolStack,
    OCI_ControllerHandle* controller);
#endif    

typedef OCI_ErrorCode
(OCI_CALL *PF_OCI_CreateLINControllerBind)(
    const BOA_ProtocolStack* protocolStack,
    OCI_ControllerHandle* controller);


/**
  Destroy an interface instance of a LIN controller. 
  
  This function implicitly releases all resources (queues, filter) and unlocks the controller configuration
  (see also OCI_CloseLINController()) if the controller is not already in the the closed state. After 
  this call the handle @a controller is invalid and must not be used any more.

  @controllerState CREATED, RUNNING, SUSPENDED, DISCONNECTED. \n
                   The controller instance is destroyed and has no state after successful execution of this call.

  @param[in]  controller LIN controller interface instance to destroy.
 
  @return
  <TABLE border="0" rules="rows" cellspacing="3" cellpadding="3">  
   <tr> <td> @ref OCI_SUCCESS </td> 
        <td> The operation was successful. </td> 
   </tr>
   <tr> <td> @ref OCI_ERR_INVALID_HANDLE </td>  
        <td> The handle is not valid. </td> 
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
OCI_CALL OCI_DestroyLINController(
    OCI_ControllerHandle controller);
#endif

typedef OCI_ErrorCode
(OCI_CALL *PF_OCI_DestroyLINController)(
    OCI_ControllerHandle controller);


/** 
  Open and configure (activate / begin use of) a LIN controller interface instance. 
  
  The configuration provided by this call must either be compatible with the setup currently used by the
  physical controller or this instance must be the first logical controller instance that opens the physical
  controller.
  
  The configuration can only be set once with this call while the controller is in the @a CREATED state. The 
  configuration is used and locked until OCI_CloseLINController() (or OCI_DestroyLINController()) for this 
  controller instance is called.

  The controller may be opened in the @a SUSPENDED mode or @a RUNNING mode. In the @a SUSPENDED mode the 
  controller will not take part in the bus communication. It is possible to configure filter and queues with 
  multiple subsequent API calls. Switching the controller to the @a RUNNING mode by calling 
  OCI_SetLINControllerProperties() will activate all settings at the same time when the controller is activated. 
  In the @a RUNNING mode the controller will immideatly take part in the bus communication. Subsequent API calls 
  will activate queues and filter step by step.
  
  @controllerState CREATED. \n
                   A transition to the RUNNING or SUSPENDED state will be executed 
                   (depends on the parameter @a properties).

  @param[in]  controller     Handle of the interface instance of the controller to open. 
  @param[in]  configuration  Pointer to a buffer that contains the configuration for the controller.  
  @param[in]  properties     Pointer to a buffer that contains the properties for the controller.  
  
  @return
  <TABLE border="0" rules="rows" cellspacing="3" cellpadding="3">  
   <tr> <td> @ref OCI_SUCCESS </td> 
        <td> The operation was successful. </td> 
   </tr>
   <tr> <td> @ref OCI_ERR_INVALID_HANDLE </td>  
        <td> The @a controller handle is not valid. </td> 
   </tr>
   <tr> <td> @ref OCI_ERR_DRIVER_NO_RESPONSE </td>
        <td> The driver for a removable hardware received no response to a request. The hardware may be removed. 
             It is not known wether the request is lost and the command not exectuted or the response is lost and
             the command was executed. The controller enters the @a DISCONNECTED state. All subsequent calls will 
             return @ref OCI_ERR_DRIVER_DISCONNECTED. 
             Internal resources are locked until OCI_DestroyLINController() for the controller is called. </td> 
   </tr>
   <tr> <td> @ref OCI_ERR_DRIVER_DISCONNECTED </td>
        <td> The driver for a removable hardware detected a communication problem (in asynchromous communication, 
             by event signalling or during a previous call to the same hardware) and is not able to communicate to 
             the hardware any more. 
             The state of the controller is @a DISCONNECTED. The command is not executed. All subsequent calls to 
             the same Controller will return @ref OCI_ERR_DRIVER_DISCONNECTED. Internal resources are locked until 
             OCI_DestroyLINController() for the controller is called.  </td> 
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

  @sa         @ref GROUP_OCI_CONTROLLER "Controller Handling"
*/

#ifndef OCI_DYNAMICDLL
OCI_DECLSPEC OCI_ErrorCode
OCI_CALL OCI_OpenLINController(
    OCI_ControllerHandle controller, 
    OCI_LINConfiguration* configuration,
    OCI_LINControllerProperties* properties);
#endif    

typedef OCI_ErrorCode
(OCI_CALL *PF_OCI_OpenLINController)(
    OCI_ControllerHandle controller, 
    OCI_LINConfiguration* configuration,
    OCI_LINControllerProperties* properties);


/**
  Query the configuration of the LIN controller.
  
  This call can be executed in any state of the controller instance. Hardware that supports more than one
  logical controller instance on a single physical interface delivers the configuration of the physical
  controller. When the controller instance is in the @a RUNNING or @a SUSPENDED mode, the data will be 
  compatible to the confguration set by OCI_OpenLINController(). When this instance is in the @a CREATED state
  and a different controller instance has already configured the hardware (via OCI_OpenLINController()) the
  configuration used by that instance is returned. When no logical instance accesses the physical controller 
  the call fails and delivers @ref OCI_ERR_NO_CONFIG.

  This call can be used to attach a second application to a single physical controller. 

  @controllerState CREATED, SUSPENDED, RUNNING.
 
  @param[in]   controller      Handle of the LIN controller instance that is queried. 
  
  @param[out]  configuration   Pointer to the buffer that will receive the configuration structure.
                               Must not be @c NULL.

  @return
  <TABLE border="0" rules="rows" cellspacing="3" cellpadding="3">  
   <tr> <td> @ref OCI_SUCCESS </td> 
        <td> The operation was successful. </td> 
   </tr>
   <tr> <td> @ref OCI_ERR_INVALID_HANDLE </td>  
        <td> The handle is not valid. </td> 
   </tr>
   <tr> <td> @ref OCI_ERR_INVALID_PARAMETER </td>
        <td> The configuration paramter is NULL or references an invalid address.</td> 
   </tr>
   <tr> <td> @ref OCI_ERR_NO_CONFIG </td>  
        <td> There is currently no instance accessing the LIN Controller and the controller uses no vaild
             configuration. This error can only occur when the call was made, when the controller instance
             is in the state @em created (see also @ref GROUP_OCI_CONTROLLER)
             </td> 
   </tr>
   <tr> <td> @ref OCI_ERR_DRIVER_NO_RESPONSE </td>
        <td> The driver for a removable hardware received no response to a request. The hardware may be removed. 
             It is not known wether the request is lost and the command not exectuted or the response is lost and
             the command was executed. The controller enters the @a DISCONNECTED state. All subsequent calls will 
             return @ref OCI_ERR_DRIVER_DISCONNECTED. 
             Internal resources are locked until OCI_DestroyLINController() for the controller is called. </td> 
   </tr>
   <tr> <td> @ref OCI_ERR_DRIVER_DISCONNECTED </td>
        <td> The driver for a removable hardware detected a communication problem (in asynchromous communication, 
             by event signalling or during a previous call to the same hardware) and is not able to communicate to 
             the hardware any more. 
             The state of the controller is @a DISCONNECTED. The command is not executed. All subsequent calls to 
             the same Controller will return @ref OCI_ERR_DRIVER_DISCONNECTED. Internal resources are locked until 
             OCI_DestroyLINController() for the controller is called.  </td> 
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
OCI_CALL OCI_GetLINConfiguration(
    OCI_ControllerHandle controller, 
    OCI_LINConfiguration* configuration);
#endif

typedef OCI_ErrorCode
(OCI_CALL *PF_OCI_GetLINConfiguration)(
    OCI_ControllerHandle controller, 
    OCI_LINConfiguration* configuration);


/**
  Close and unlock the configuration of a LIN controller instance. 
  
  All queues and filters for this instance are implicitly destroyed with this call. It is undefined if data 
  in transmit queues is flushed or discarded. When this call returns, all resources associated with this instance 
  have been cleaned up and the instance stopped transmitting data on the bus. 

  The state of the controller instance is moved to @a CREATED (see also @ref GROUP_OCI_CONTROLLER). In a hardware 
  that supports multiple instances on a single physical interface the configuration may be changed by a different 
  instance by calling OCI_OpenLINController() in tis state.

  @controllerState RUNNING, SUSPENDED. \n
                   A transition to the CREATED state is executed.  \n
                   When the controller is in the DISCONNECTED state the function may be called to do local clean
                   up. Only a limitied plausibility check may be done in this case.  

  @param[in]  controller Handle of the Controller instance that will be closed.
  
  @return
  <TABLE border="0" rules="rows" cellspacing="3" cellpadding="3">  
   <tr> <td> @ref OCI_SUCCESS </td> 
        <td> The operation was successful. 
             The @a controller is unlocked and may be re-configured by a different instance. </td> 
   </tr>
   <tr> <td> @ref OCI_ERR_INVALID_HANDLE </td>  
        <td> The @a controller handle is not valid. </td> 
   </tr>
   <tr> <td> @ref OCI_ERR_INVALID_STATE </td>
        <td> The @a controller was not opened.</td> 
   </tr>
   <tr> <td> @ref OCI_ERR_OUT_OF_MEMORY </td>
        <td> The driver run out of memory. This error should only occur on heavy loaded systems with multiple 
             clients and large send and receive queues. The error may indicate a resource leak in the driver 
             implementation. When multiple application access the same resource (interface hardware or physical
             controller) the termination of a different application may return the resources required for this 
             instance. </td> 
   </tr>
  </TABLE> 

* @sa         @ref GROUP_OCI_CONTROLLER "Controller Handling"  
*/

#ifndef OCI_DYNAMICDLL
OCI_DECLSPEC OCI_ErrorCode 
OCI_CALL OCI_CloseLINController(
    OCI_ControllerHandle controller);
#endif

typedef OCI_ErrorCode
(OCI_CALL *PF_OCI_CloseLINController)(
    OCI_ControllerHandle controller);


/**
* @}
* @addtogroup GROUP_OCI_LIN_MESSAGES Messages
* @{
*/
/**
* @}
* @addtogroup GROUP_OCI_LIN_INITIALIZATION Initialization
* @{
*/


/**
  Query the properties of the LIN controller instance. 

  Properties are read/writeable and can be changed using OCI_SetLINControllerProperties(). 

  These properties are the locigal state of this controller instance. In a hardware that 
  supports multiple instances on a single physical interface the state of the physical controller will be 
  active until the last instance suspends the logical state. 

  @controllerState CREATED, RUNNING, SUSPENDED.

  @param[in]  controller  Handle of the LIN controller interface instance that is queried.

  @param[out] properties  Pointer to a buffer that will receive the controller interface instance properties. 
                          Must not be @c NULL.
  
  @return
  <TABLE border="0" rules="rows" cellspacing="3" cellpadding="3">  
   <tr> <td> @ref OCI_SUCCESS </td> 
        <td> The operation was successful. </td> 
   </tr>
   <tr> <td> @ref OCI_ERR_INVALID_HANDLE </td>  
        <td> The handle is not valid. </td> 
   </tr>
   <tr> <td> @ref OCI_ERR_INVALID_PARAMETER </td>
        <td> The properties paramter is NULL or points to an invalid object.</td> 
   </tr>
   <tr> <td> @ref OCI_ERR_DRIVER_NO_RESPONSE </td>
        <td> The driver for a removable hardware received no response to a request. The hardware may be removed. 
             It is not known wether the request is lost and the command not exectuted or the response is lost and
             the command was executed. The controller enters the @a DISCONNECTED state. All subsequent calls will 
             return @ref OCI_ERR_DRIVER_DISCONNECTED. 
             Internal resources are locked until OCI_DestroyLINController() for the controller is called. </td> 
   </tr>
   <tr> <td> @ref OCI_ERR_DRIVER_DISCONNECTED </td>
        <td> The driver for a removable hardware detected a communication problem (in asynchromous communication, 
             by event signalling or during a previous call to the same hardware) and is not able to communicate to 
             the hardware any more. 
             The state of the controller is @a DISCONNECTED. The command is not executed. All subsequent calls to 
             the same Controller will return @ref OCI_ERR_DRIVER_DISCONNECTED. Internal resources are locked until 
             OCI_DestroyLINController() for the controller is called.  </td> 
   </tr>
   <tr> <td> @ref OCI_ERR_OUT_OF_MEMORY </td>
        <td> The driver run out of memory. This error should only occur on heavy loaded systems with multiple 
             clients and large send and receive queues. The error may indicate a resource leak in the driver 
             implementation. When multiple application access the same resource (interface hardware or physical
             controller) the termination of a different application may return the resources required for this 
             instance. </td> 
   </tr>
  </TABLE> 
* @sa         @ref GROUP_OCI_CONTROLLER "Controller Handling" 
*/

#ifndef OCI_DYNAMICDLL
OCI_DECLSPEC OCI_ErrorCode 
OCI_CALL OCI_GetLINControllerProperties(
    OCI_ControllerHandle controller, 
    OCI_LINControllerProperties * properties);
#endif

typedef OCI_ErrorCode
(OCI_CALL *PF_OCI_GetLINControllerProperties)(
    OCI_ControllerHandle controller, 
    OCI_LINControllerProperties * properties);

/**
  Set the LIN controller properties (suspended / running) of the LIN controller instance. 
  
  Properties are read/writeable and can be queried OCI_GetLINControllerProperties().

  These properties are the locigal state of this controller instance. 
  @li In the @a SUSPENDED state the controller instance will neither send any data on the interface nor receive 
      any data. 
  @li In the @a RUNNING state the controller instance will send data, when the transmit queue is not empty and 
      receive any data that passes a configured filter condition.

  In a hardware that supports multiple instances on a single physical interface the state of the physical controller 
  will be @a RUNNING until the last instance suspends the logical state. 

  The @a SUSPENDED state is useful to configure a set of filter and activate the complete new set with at the
  same time. When the filter are set by (especially a sequence of different) API-calls intermediate states
  exist where some filter already active and other not. This may be critical for specific applications.

  The @a properties parameter describes the controller state after successful execution of the command. When 
  the controller is already in the desired state the driver will ignore the command and return @ref OCI_SUCCESS.

  @controllerState RUNNING, SUSPENDED. \n
                   A transition to the RUNNING or SUSPENDED state may be executed 
                   (depends on the parameter @a properties).

  @param[in]  controller  Handle of the LIN controller interface instance for which the properties are set.
  
  @param[in]  properties  Pointer to the LIN controller properties that are set for the controller instance.
                          Must not be @c NULL.

  @return
  <TABLE border="0" rules="rows" cellspacing="3" cellpadding="3">  
   <tr> <td> @ref OCI_SUCCESS </td> 
        <td> The operation was successful. </td> 
   </tr>
   <tr> <td> @ref OCI_ERR_INVALID_HANDLE </td>  
        <td> The @a controller handle is not valid. </td> 
   </tr>
   <tr> <td> @ref OCI_ERR_INVALID_PARAMETER </td>
        <td> The @a properties paramter is NULL or references an invalid address.</td> 
   </tr>
   <tr> <td> @ref OCI_ERR_INVALID_STATE </td>
        <td> The controller is not in the @a RUNNING or @a SUSPENDED state. When the controller is in the
             @a CREATED state additional configuration information is required and OCI_OpenLINController() must
             be used. </td> 
   </tr>
   <tr> <td> @ref OCI_ERR_DRIVER_NO_RESPONSE </td>
        <td> The driver for a removable hardware received no response to a request. The hardware may be removed. 
             It is not known wether the request is lost and the command not exectuted or the response is lost and
             the command was executed. The controller enters the @a DISCONNECTED state. All subsequent calls will 
             return @ref OCI_ERR_DRIVER_DISCONNECTED. 
             Internal resources are locked until OCI_DestroyLINController() for the controller is called. </td> 
   </tr>
   <tr> <td> @ref OCI_ERR_DRIVER_DISCONNECTED </td>
        <td> The driver for a removable hardware detected a communication problem (in asynchromous communication, 
             by event signalling or during a previous call to the same hardware) and is not able to communicate to 
             the hardware any more. 
             The state of the controller is @a DISCONNECTED. The command is not executed. All subsequent calls to 
             the same Controller will return @ref OCI_ERR_DRIVER_DISCONNECTED. Internal resources are locked until 
             OCI_DestroyLINController() for the controller is called.  </td> 
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
OCI_CALL OCI_SetLINControllerProperties(
    OCI_ControllerHandle controller, 
    OCI_LINControllerProperties * properties);
#endif

typedef OCI_ErrorCode
(OCI_CALL *PF_OCI_SetLINControllerProperties)(
    OCI_ControllerHandle controller, 
    OCI_LINControllerProperties * properties);

/** @} */

/**
* @addtogroup GROUP_OCI_LIN_INTERNAL Internal Structures
* @brief      Additional definitions relevant for OCI implementors. 
* @{
*/

/**
* Internal identifier for the OCI LIN interface.
*/

#define UUID_OCILIN {0x00,0xAF,0x19,0x3F,0x16,0x7C,0x48,0x72,0x99,0x5D,0x81,0x63,0x38,0x12,0x5F,0xD0}

/**
* Internal v-table for the OCI LIN interface.
*/

typedef struct OCI_LIN_VTable
{
    /* ocitime.h */

    OCI_Time_VTable timeVTable;

    /* ocierror.h */

    OCI_Error_VTable errorVTable;

    /* ociqueue.h */

    OCI_Queue_VTable queueVTable;

    /* ocilinio.h */

    OCI_LINIO_VTable linioVTable;

    /* ocilin.h */

    PF_OCI_CreateLINController createLINController;
    PF_OCI_CreateLINControllerBind createLINControllerBind;
    PF_OCI_DestroyLINController destroyLINController;
    PF_OCI_OpenLINController openLINController;
    PF_OCI_GetLINConfiguration getLINConfiguration;
    PF_OCI_CloseLINController closeLINController;

    PF_OCI_GetLINControllerProperties getLINControllerProperties;
    PF_OCI_SetLINControllerProperties setLINControllerProperties;

} OCI_LIN_VTable;

/** @} */

/** @} */

#ifdef __cplusplus
}
#endif

#endif
