#ifndef ETAS_OCI_ERROR_H
#define ETAS_OCI_ERROR_H


/**
* @file       ocierror.h
* @brief      Error management functionality of the 
*             Open Controller Interface (OCI) API.
* @copyright  Copyright (c) 2007-2008 ETAS GmbH. All rights reserved.
*/

#include "ocitypes.h"

#include "..\Common\pshpack1.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 @defgroup GROUP_OCI_ERROR_MANAGEMENT OCI Error Management
*/

#if !defined (OCI_DOCU)
/**
* @addtogroup GROUP_OCI_COMMON OCI Common
* @{
*/
#endif

/**
* @addtogroup GROUP_OCI_ERROR_MANAGEMENT 
* @{

  @section ERROR_CLASS Different Classes of Errors.

  Any Function call of of the OCI returns an @ref OCI_ErrorCode. There are three different classes of error codes:

   - SUCCESS: The normal return code of a function call should be @ref OCI_SUCCESS. The requested service is 
              executed without an error and the application can continue operation. 
              Note: Many services use a queued characteristic and a successful return of the call does not 
              guarantee a successful completion of the action. e.g. A CAN Frame can successfully be queued for 
              sending, but may never be send, when frames from other bus-nodes with higher priority saturate the 
              bus.
   - WARNING: In very rare cases a warning may occur. The driver was not able to execute the request exact in
              the requested way, but has adapted some setting. The adaptation is the best estimation for a 
              @em compatible setting. The application may react on a warning, but in most cases it can proceeed 
              in the normal operational way. 
              An example for a warning is the adaptation of the sample point in a CAN-Bus configuration. The 
              hardware may be limited to set the sample point to an accuracy of 5%, but was requested to set it
              to 87%. In this case the driver will use 85% and return with a warning. The application may read
              the used parameter (by OCI_GetCANConfiguration()) to identify the adaptation.
   - ERROR:   The service was not executed successfully. The application has to react on the error.
              Note: The driver is not able to detect the severity of the error. The error indicates, that the 
              driver did not execute the requested service for some reason.

  The different error classes are indicated by the upper two bits in the error code. 
  
  @section ERROR_TYPE The error type.

  There are several different groups of error sources. In many cases it will be possible to react on all errors
  of the same type in the same way. 

  - Parameter vaildation of a call fails.
    The driver validates the parameter supplied by the application and detects a plausibilty problem. In normal
    cases this type of error is caused by an implementation problem in the application.

  - Semantical inconsistences.
    The validation of some pre-conditions, that are required for a specific API call failed. The driver can not
    execute the requested action. 

  - (Hardware specific-) resource problems.
    The hardware run out of resources. The reason for these kind of errors may be different. If a hardware supports
    multiple clients and implements multiple BUS-interfaces it may share some resources for all application accessing
    any interface of the system. In this case the termination of a different application may return the required
    resources. Other reasons for this error may be a resource leak in the driver or application.

  - Communication problems to the interface hardware.
    Especially external hardware (connected via Ethernet, USB, CardBus, ExpressCard, ...) may be removed while it
    is accessed by the driver. The reliability of some interface and protocol in a rough environment may be limited.
    In both cases the communication to the interface hardware may be disturbed or even disabled. 
    When the hardware is accessible again the state of the interface hardware may be undefined. In most cases a
    complete re-initialization is required.

  - Implementation (internal) errors in the driver
    After passing the paramter and semantic validation a driver will verify the consistence of internal data. This
    verification may fail and an error can be delivered to the application. These kind of errors should never occur.
    The vendor of the driver has to analyze and fix the problem.

*/

/**
* @anchor ANCHOR_OCI_ERROR_FLAGS
* @name   OCI Error Code Flags and Masks
* @brief  Special flags and masks of @ref OCI_ErrorCode to classify an error.
*
* @{
*/

/** flag in error code, which classifies an OCI_ErrorCode as an error. */
#define OCI_ERR_FLAG_ERROR              0x80000000

/** flag in error code, which classifies an OCI_ErrorCode as a warning. */
#define OCI_ERR_FLAG_WARNING            0x40000000

/**  Generic test for failure of an OCI-call. 
     When bit 31 is set an error occured during execution of the service. */
#define OCI_FAILED(code)       ((int32)(code) < 0)

/** Generic test for successful execution of an OCI-call. Any successful executed service will 
    return with a non negative value. */
#define OCI_SUCCEEDED(code)    ((int32)(code) >= 0)

/**  Generic test for a returned warning from the execution of an OCI-call. */
#define OCI_WARNED(code)       ((code) & OCI_ERR_FLAG_WARNING)

/** @} */

/**
* @anchor ANCHOR_OCI_ERROR_TYPES
* @name   OCI Error Type Flags and Masks
* @brief  Special flags and masks of @ref OCI_ErrorCode to group an error into different types.
*
* @{
*/
#define OCI_ERR_TYPE_RESERVED           0x00000000  /**< currently (01/2009) used for error codes. */
#define OCI_ERR_TYPE_PARAM              0x00001000  /**< Parameter validation fails. */
#define OCI_ERR_TYPE_BIND               0x00002000  /**< Binding process of the API failed. When the CSI component
                                                         is used for searching and binding this can be seen as an
                                                         internal error. */
#define OCI_ERR_TYPE_SEMANTIC           0x00003000  /**< Semantic / Precondition validation fails. */
#define OCI_ERR_TYPE_RESOURCE           0x00004000  /**< Driver runs out of resources. */
#define OCI_ERR_TYPE_COMMUNICATION      0x00005000  /**< Communication problems to the interface hardware. */
#define OCI_ERR_TYPE_INTERNAL           0x00006000  /**< Driver internal implementation error. */
#define OCI_ERR_TYPE_PRIVATE            0x0000F000  /**< Reserved for internal result codes. */

#define OCI_ERR_TYPE_MASK               0x0000F000  /** < mask to unmask the error type information from all other
                                                          error information to allow a generic type specific reaction */  

#define OCI_ERR_TYPE(code) ((code) & OCI_ERR_TYPE_MASK) /**< separate the error type information */

/** @} */


/**
* @anchor ANCHOR_OCI_ERROR_CODES
* @name   OCI Error Codes
* @brief  Predefined values of @ref OCI_ErrorCode to specify an error.
* @{
*/

/** 
 Special @ref OCI_ErrorCode value to indicate that the respective function executed without error.
*/
#define OCI_SUCCESS                         0

/** 
  Special @ref OCI_ErrorCode value to generally indicate a failure of the respective function call.
  @coding  Deprecated code. Prefer returning specific error codes.
*/
#define OCI_FAILURE                         (OCI_ERR_FLAG_ERROR)

/*----------------------------------------------------------------------------------------------------------*/
/**
  @anchor ANCHOR_OCI_ERROR_TYPE_PARAM
  @name   Member of the OCI Error Type @ref OCI_ERR_TYPE_PARAM. 
  These error codes indicate an error in some parameter of a call.
  @{
*/

/** At least one parameter is wrong or invalid. Typically this error occurs, if a parameter of a function of 
    the OCI API is @c NULL, not initialized or unexpected. */
#define OCI_ERR_INVALID_PARAMETER           (OCI_ERR_FLAG_ERROR | OCI_ERR_TYPE_PARAM | 0)

/** Some Parameter is inplausible. It uses a inplausible combination of settings. */
#define OCI_ERR_INCONSISTENT_PARAMETER_SET  (OCI_ERR_FLAG_ERROR | OCI_ERR_TYPE_PARAM | 1)

/** The parameter is no valid handle or the used handle has the wrong type. 
    (e.g.\ A OCI_ControllerHandle is used in a function call that expects a OCI_QueueHandle) */
#define OCI_ERR_INVALID_HANDLE              (OCI_ERR_FLAG_ERROR | OCI_ERR_TYPE_PARAM | 2)

/** The data was too large to fit into the specified buffer. */
#define OCI_ERR_BUFFER_OVERFLOW             (OCI_ERR_FLAG_ERROR | OCI_ERR_TYPE_PARAM | 3)

/* The message type or message format of the parameter is not supported. */
/*#define OCI_ERR_UNSUPPORTED_MESSAGE         (OCI_ERR_FLAG_ERROR | OCI_ERR_TYPE_PARAM | 4) --> inconsistent parameter set
*/

/* One or more filter definitions have invalid values and can not be set. */
/*#define OCI_ERR_INVALID_FILTER				  (OCI_ERR_FLAG_ERROR | OCI_ERR_TYPE_PARAM | 5) --> inconsistent parameter set
*/
/*  A filter can not be removed from a queue because the filter is not known by the driver.
     The filter was never set or is already removed. */
/*#define OCI_ERR_FILTER_UNKNOWN				  (OCI_ERR_FLAG_ERROR | OCI_ERR_TYPE_PARAM | 6) --> inconsistent parameter set
*/

/** At least one parameter is not supported. */
#define OCI_ERR_UNSUPPORTED_PARAMETER       (OCI_ERR_FLAG_ERROR | OCI_ERR_TYPE_PARAM | 4)

/** @} */

/*----------------------------------------------------------------------------------------------------------*/
/** 
  @anchor ANCHOR_OCI_ERROR_TYPE_SEMANTIC
  @name   Member of the OCI Error Type @ref OCI_ERR_TYPE_SEMANTIC. 
          These error codes indicate an error in some preconditions of the call.
  @{
 */

/** The driver is not in a valid state to perform this request. */
#define OCI_ERR_INVALID_STATE               (OCI_ERR_FLAG_ERROR | OCI_ERR_TYPE_SEMANTIC | 0)

/**  The controller interface is not opened, no valid configuration is available. */
#define OCI_ERR_NO_CONFIG                   (OCI_ERR_FLAG_ERROR | OCI_ERR_TYPE_SEMANTIC | 1)

/** @todo Replaced by @ref OCI_ERR_NO_CONFIG */
#define OCI_ERR_CONTROLLER_IS_NOT_OPENED    OCI_ERR_NO_CONFIG

/**  The queue is full. */
#define OCI_ERR_QUEUE_IS_FULL               (OCI_ERR_FLAG_ERROR | OCI_ERR_TYPE_SEMANTIC | 2)

/** @} */

/*----------------------------------------------------------------------------------------------------------*/
/**
  @anchor ANCHOR_OCI_ERROR_TYPE_RESOURCE
  @name   Member of the OCI Error Type @ref OCI_ERR_TYPE_RESOURCE. 
          These error codes indicate a resource problem in the driver.
  @{
*/

/** The call tries to use incompatible settings. A second OCI-Instance or a different client has locked the
    configuration. */
#define OCI_ERR_INCOMPATIBLE_CONFIG         (OCI_ERR_FLAG_ERROR | OCI_ERR_TYPE_RESOURCE | 0)

/** No memory resources available. */
#define OCI_ERR_OUT_OF_MEMORY               (OCI_ERR_FLAG_ERROR | OCI_ERR_TYPE_RESOURCE | 1)

/** No resources available. */
#define OCI_ERR_NO_RESOURCES                (OCI_ERR_FLAG_ERROR | OCI_ERR_TYPE_RESOURCE | 2)

/** @} */

/*----------------------------------------------------------------------------------------------------------*/
/**
  @anchor ANCHOR_OCI_ERROR_TYPE_COMMUNICATION
  @name   Member of the OCI Error Type @ref OCI_ERR_TYPE_COMMUNICATION. 
          These error codes indicate a problem in the communication to the hardware interface.
  @{
*/

/** The driver for a removable hardware received no response to a request. The hardware may be removed. There
    is no information whether the request is executed or not. The request may be lost or the response may be lost.
    All subsequent calls to the same controller will deliver @ref OCI_ERR_DRIVER_DISCONNECTED. The state of the
    controller is set to @a DISCONNECTED. */
#define OCI_ERR_DRIVER_NO_RESPONSE          (OCI_ERR_FLAG_ERROR | OCI_ERR_TYPE_COMMUNICATION | 0)

/** The driver for a removable hardware detected a communication problem (in asynchromous communication, 
    by event signaling or during a previous call to the same hardware) and is not able to communicate to 
    the hardware any more. The state of the session is undefined. All subsequent calls to the same 
    Controller will return @ref OCI_ERR_DRIVER_DISCONNECTED. Internal resources are locked until 
    OCI_DestroyXXXController() for the controller handle is called. */
#define OCI_ERR_DRIVER_DISCONNECTED         (OCI_ERR_FLAG_ERROR | OCI_ERR_TYPE_COMMUNICATION | 1)

/** @} */

/*----------------------------------------------------------------------------------------------------------*/
/**
  @anchor ANCHOR_OCI_ERROR_TYPE_INTERNAL
  @name   Member of the OCI Error Type @ref OCI_ERR_TYPE_INTERNAL. 
          These error codes indicate a driver internal problem. Some internal plausibility checks detect an 
          error although parameter of the call seem to be correct.
  @{
*/

/** Some driver internal software related error occurred. Internal plausibility checks failed. The driver will 
    not work correct. This is a fatal error condition. */
#define OCI_ERR_UNEXPECTED_NULL             (OCI_ERR_FLAG_ERROR | OCI_ERR_TYPE_INTERNAL | 0)

/** Some driver internal hardware related error occurred. Internal checks, communication between sub-systems,
access to hardware resources failed or timed out. */
#define OCI_ERR_HW_NOT_READY                (OCI_ERR_FLAG_ERROR | OCI_ERR_TYPE_INTERNAL | 1)

/** An index or value is out of legal range. */
#define OCI_ERR_OUT_OF_RANGE                (OCI_ERR_FLAG_ERROR | OCI_ERR_TYPE_INTERNAL | 2)

/** The driver is not in a valid state to perform this request. */
#define OCI_ERR_INTERNAL_STATE              (OCI_ERR_FLAG_ERROR | OCI_ERR_TYPE_INTERNAL | 3)

/** A used internal handle is not valid or has the wrong type. */
#define OCI_ERR_INTERNAL_HANDLE             (OCI_ERR_FLAG_ERROR | OCI_ERR_TYPE_INTERNAL | 4)

/** Overflow of an internal buffer. */
#define OCI_ERR_INTERNAL_OVERFLOW           (OCI_ERR_FLAG_ERROR | OCI_ERR_TYPE_INTERNAL | 5)

/** The requested function is not implemented. This is a deprecated error code, that must not occur in a
    BOA compatible driver. The BOA defines no optional API functions. The code may be used during driver
    development. */
#define OCI_ERR_NOT_IMPLEMENTED             (OCI_ERR_FLAG_ERROR | OCI_ERR_TYPE_INTERNAL | 6)

/** Error from an underlying driver or operating system. */
#define OCI_ERR_SYSTEM_ERROR                (OCI_ERR_FLAG_ERROR | OCI_ERR_TYPE_INTERNAL | 7)

/** No data available. */
#define OCI_ERR_NO_DATA                     (OCI_ERR_FLAG_ERROR | OCI_ERR_TYPE_INTERNAL | 8)

/** Data is not initialized. */
#define OCI_ERR_NOT_INITIALIZED             (OCI_ERR_FLAG_ERROR | OCI_ERR_TYPE_INTERNAL | 9)

/* The user called a function with the wrong bus type. This is an internal error because the binding 
    process must not bind APIs to the wrong bus type. */
/*#define OCI_ERR_WRONG_BUS_TYPE                 (OCI_ERR_FLAG_ERROR | OCI_ERR_TYPE_INTERNAL | 2)
*/
/** @} */

/*----------------------------------------------------------------------------------------------------------*/
/**
  @anchor ANCHOR_OCI_ERROR_TYPE_BIND
  @name   Member of the OCI Error Type @ref OCI_ERR_TYPE_BIND. 
          These error codes indicate a problem during binding process. When the CSI components are used for
          detecting the components and the result of this search is used for binding, the error type indicates
          an internal error. When the binding is not based on the search result the error type gives detailed
          information for the binding failure.
  @{
*/

/** The server offers the interface with the requested UUID, but the can not fullfill the required version. */
#define OCI_ERR_PROTOCOL_VERSION_NOT_SUPPORTED (OCI_ERR_FLAG_ERROR | OCI_ERR_TYPE_BIND | 0)

/** The parsing of the access parameter failed. The driver can not interpret the parameter*/
#define OCI_ERR_INVALID_ACCESS_SYNTAX          (OCI_ERR_FLAG_ERROR | OCI_ERR_TYPE_BIND | 1)

/** An element of the access parameter can not be found/resolved in the protocol stack. The connection can 
    not be established */
#define OCI_ERR_INVALID_ACCESS_PARAM           (OCI_ERR_FLAG_ERROR | OCI_ERR_TYPE_BIND | 2)

/** The server offers the interface with the requested UUID, but does not support the required binding mechanism 
    for this interface. The interface can not be used. */
#define OCI_ERR_INVALID_TRANSFER_SYNTAX        (OCI_ERR_FLAG_ERROR | OCI_ERR_TYPE_BIND | 3)

/** The interface with the requested UUID is not supported by the addressed node. */
#define OCI_ERR_NO_INTERFACE                   (OCI_ERR_FLAG_ERROR | OCI_ERR_TYPE_BIND | 4)          

/** The requested hardware is not present. */
#define OCI_ERR_HW_NOT_PRESENT                 (OCI_ERR_FLAG_ERROR | OCI_ERR_TYPE_BIND | 5)  

/** The attempt to load or open the driver failed. */
#define OCI_ERR_CANNOT_OPEN_DRIVER             (OCI_ERR_FLAG_ERROR | OCI_ERR_TYPE_BIND | 6)  

/** The validation of a license failed. */
#define OCI_ERR_LICENSE_MISSING                (OCI_ERR_FLAG_ERROR | OCI_ERR_TYPE_BIND | 7)

/** @} */


#define OCI_ERROR_STRING_LENGTH  256


/**
  Returns a textual description of the last error associated with the @a source handle. 
  The text returned may be constant, i.e.\ carry no additional information but is just a 
  description of the respective error code. For complex calls the textual description may
  give detailed information about the parameter (or even parameter element), that caused
  the error.

  @param[in]  source   Handle used by the call, that caused the error. If this call required no handle
                       use OCI_NO_HANDLE.
  @param[in]  code     The error code which should be decoded.
  @param[in]  utf8Text A pointer to a buffer to receive the error description of the latest errors.
  @param[in]  size     The size of the buffer in bytes, including the terminating 0.

  @return
  <TABLE border="0" rules="rows" cellspacing="3" cellpadding="3">  
   <tr> <td> @ref OCI_SUCCESS </td> 
        <td> The operation was successful. The utf8Text is filled with the error description. </td> 
   </tr>
   <tr> <td> @ref OCI_ERR_INVALID_HANDLE </td>  
        <td> The @a source handle is not valid. </td> 
   </tr>
   <tr> <td> @ref OCI_ERR_INVALID_PARAMETER </td>
        <td> The @a utf8Text paramter is NULL or references an invalid address.</td> 
   </tr>
   <tr> <td> @ref OCI_ERR_BUFFER_OVERFLOW </td>
        <td> The size of the utf8Text-Buffer is too small to receive the complete error description. </td> 
   </tr>
   <tr> <td> @ref OCI_ERR_DRIVER_NO_RESPONSE </td>
        <td> The driver for a removable hardware received no response to the request. The hardware may be removed. 
             The state of the session is undefined. All subsequent calls will return @ref OCI_ERR_DRIVER_DISCONNECTED. 
             Internal resources are locked until OCI_DestroyCANController() for the controller is called. </td> 
   </tr>
   <tr> <td> @ref OCI_ERR_DRIVER_DISCONNECTED </td>
        <td> The driver for a removable hardware detected a communication problem (in asynchromous communication, 
             by event signalling or during a previous call to the same hardware) and is not able to communicate to 
             the hardware any more. The state of the session is undefined. All subsequent calls to the same 
             Controller will return @ref OCI_ERR_DRIVER_DISCONNECTED. Internal resources are locked until 
             OCI_DestroyCANController(), OCI_DestroyLINController() or OCI_DestroyFlexRayController() for the controller 
             handle is called.  </td> 
   </tr>
   <tr> <td> @ref OCI_ERR_OUT_OF_MEMORY </td>
        <td> The driver run out of memory. This error should only occur on heavy loaded systems with multiple 
             clients and large send and receive queues. The error may indicate a resource leak in the driver 
             implementation. When multiple application access the same resource (interface hardware or physical
             controller) the termination of a different application may return the resources required for this 
             instance. </td> 
   </tr>
  </TABLE>
*/

#ifndef OCI_DYNAMICDLL
OCI_DECLSPEC OCI_ErrorCode
OCI_CALL OCI_GetError(
    OCI_Handle source, 
    OCI_ErrorCode code,
    char *utf8Text,
    uint32 size); 
#endif

typedef OCI_ErrorCode
(OCI_CALL *PF_OCI_GetError)(
    OCI_Handle source, 
    OCI_ErrorCode code,
    char *utf8Text,
    uint32 size);


/**
* Codes for event generation. 
*/
typedef enum OCI_InternalErrorEvent
{
   OCI_INTERNAL_GENERAL_ERROR = 0

} OCI_InternalErrorEvent;


/**
* Definition of an event filter for internal error events.
*/
typedef struct OCI_InternalErrorEventFilter
{
    /**
    * code for the event, that shall generate a event message.
    */
    OCI_InternalErrorEvent eventCode;

    /**
    * tag that is used for an in-band message when the event fired.
    */
    uint32 tag;

} OCI_InternalErrorEventFilter;


/** 
* Structure that is passed as a callback parameter for internal
* failures like connection breakdown etc.
*/
typedef struct OCI_InternalErrorEventMessage
{
    /** 
    * Global time stamp when the event was triggered on the hardware. 
    */
    OCI_Time timeStamp;

    /**
    * Filter tag specified in @ref OCI_InternalErrorEventFilter.
    */
    uint32 tag;

    /** code for the event that generated this message */
    OCI_InternalErrorEvent eventCode;

    /** 
    * Reason for link failure or, in most re-connect scenarios, 
    * @ref OCI_SUCCESS. 
    */
    OCI_ErrorCode errorCode;
} OCI_InternalErrorEventMessage;


/**
  Add white filter condition(s) to enable the reception of internal error events. 

  In the default configuration of a receive queue no data will be received because any data is blocked by some
  filter condition. The application must enable the reception of the data by setting white filter (defining pass
  conditions).

  This function allows the reception of fatal error events of a driver implementation, that occur asynchronous to
  any API call. The event is coded as @ref OCI_InternalErrorEventMessage and may be delivered in-band in the normal 
  data stream or generate a (separate) callback. The signalization path to the application is defined by the 
  creation of the receive queue. 
  (See also OCI_CreateCANRxQueue(), OCI_CreateLINRxQueue() or OCI_CreateFlexRayRxQueue().)
  
  @param[in]  queue    Handle of the receive queue to modify. 
  @param[in]  filter   A pointer to an array of @a count entries of type @ref OCI_InternalErrorEventFilter. 
  @param[in]  count    Number of entries in the @a filter array. 

  @return     
  <TABLE border="0" rules="rows" cellspacing="3" cellpadding="3">  
   <tr> <td> @ref OCI_SUCCESS </td> 
        <td> The operation was successful. </td> 
   </tr>
   <tr> <td> @ref OCI_ERR_INVALID_HANDLE </td>  
        <td> The @a queue handle is not valid. </td> 
   </tr>
   <tr> <td> @ref OCI_ERR_INVALID_PARAMETER </td>
        <td> The @a filter parameter is NULL or references an invalid address.</td> 
   </tr>
   <tr> <td> @ref OCI_ERR_INCONSISTENT_PARAMETER_SET </td>
        <td> One or more filter objects are invalid or already exist.</td> 
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

  @ingroup GROUP_OCI_EVENT_HANDLING
  @sa GROUP_OCI_EVENT_HANDLING   
 
*/

#ifndef OCI_DYNAMICDLL
OCI_DECLSPEC OCI_ErrorCode 
OCI_CALL OCI_AddInternalErrorEventFilter(
    OCI_QueueHandle queue, 
    OCI_InternalErrorEventFilter filter[], 
    uint32 count);
#endif

typedef OCI_ErrorCode
(OCI_CALL *PF_OCI_AddInternalErrorEventFilter)(
    OCI_QueueHandle queue, 
    OCI_InternalErrorEventFilter filter[], 
    uint32 count);


/**
  Remove white filter condition(s) to disable the reception of driver internal error events for a specific receive 
  queue. 
  
  In the default configuration of a receive queue no data will be received because any data is blocked by some
  filter condition. The application must enable the reception of the data by setting white filter (defining pass
  conditions).

  This function allows to withdraw one or more white filter conditions set by OCI_AddInternalErrorEventFilter().
  Each @ref OCI_InternalErrorEventFilter of the filter array must exact match to a previously added filter. The 
  withdrawn filter will not pass the event(s) any longer and silently discard the events.
 
  @param[in]  queue    Handle of the receive queue to modify. 
  @param[in]  filter   A pointer to an array of @a count entries of type @ref OCI_InternalErrorEventFilter. 
  @param[in]  count    Number of entries in the @a filter array. 

  @return     
  <TABLE border="0" rules="rows" cellspacing="3" cellpadding="3">  
   <tr> <td> @ref OCI_SUCCESS </td> 
        <td> The operation was successful. </td> 
   </tr>
   <tr> <td> @ref OCI_ERR_INVALID_HANDLE </td>  
        <td> The @a queue handle is not valid. </td> 
   </tr>
   <tr> <td> @ref OCI_ERR_INVALID_PARAMETER </td>
        <td> The @a filter parameter is NULL or points to an invalid address.</td> 
   </tr>
   <tr> <td> @ref OCI_ERR_INCONSISTENT_PARAMETER_SET </td>
        <td> One or more @a filter objects are not set and can not be removed. The processing of the
             filter array is undefined. </td> 
   </tr>
   <tr> <td> @ref OCI_ERR_OUT_OF_MEMORY </td>
        <td> The driver run out of memory. This error should only occur on heavy loaded systems with multiple 
             clients and large send and receive queues. The error may indicate a resource leak in the driver 
             implementation. When multiple application access the same resource (interface hardware or physical
             controller) the termination of a different application may return the resources required for this 
             instance. </td> 
   </tr>
  </TABLE> 
* @ingroup GROUP_OCI_EVENT_HANDLING
* @sa GROUP_OCI_EVENT_HANDLING   
*
* @todo Is this a public function?
*/

#ifndef OCI_DYNAMICDLL
OCI_DECLSPEC OCI_ErrorCode 
OCI_CALL OCI_RemoveInternalErrorEventFilter(
    OCI_QueueHandle queue, 
    OCI_InternalErrorEventFilter filter[], 
    uint32 count);
#endif

typedef OCI_ErrorCode
(OCI_CALL *PF_OCI_RemoveInternalErrorEventFilter)(
    OCI_QueueHandle queue, 
    OCI_InternalErrorEventFilter filter[], 
    uint32 count);

typedef struct OCI_Error_VTable
{
    PF_OCI_GetError getError;
    PF_OCI_AddInternalErrorEventFilter addInternalErrorEventFilter;
    PF_OCI_RemoveInternalErrorEventFilter removeInternalErrorEventFilter;
} OCI_Error_VTable;

/** @} */

/** @} */


#ifdef __cplusplus
}
#endif

#include "..\Common\poppack.h"

#endif
