#ifndef ETAS_OCI_ERROR_H
#define ETAS_OCI_ERROR_H


/**
* @file       ocierror.h
* @brief      Error management functionality of the
*             Open Controller Interface (OCI) API.
* @copyright  Copyright (c) 2007-2008 ETAS GmbH. All rights reserved.
*
* $Revision: 6233 $
*/

#include "../common/boaerror.h"
#include "ocitypes.h"

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

    */

    /**
    * @anchor ANCHOR_OCI_ERROR_FLAGS
    * @name   OCI Error Code Flags and Masks
    * @brief  Special flags and masks of @ref OCI_ErrorCode to classify an error.
    *
    * @{
    */

    /** flag in error code, which classifies an OCI_ErrorCode as an error. */
#define OCI_ERR_FLAG_ERROR              BOA_RESULT_FLAG_ERROR

    /** flag in error code, which classifies an OCI_ErrorCode as a warning. */
#define OCI_ERR_FLAG_WARNING            BOA_RESULT_FLAG_WARNING

    /**  Generic test for failure of an OCI-call.
         When bit 31 is set an error occured during execution of the service. */
#define OCI_FAILED(code)       BOA_FAILED(code)

    /** Generic test for successful execution of an OCI-call. Any successful executed service will
        return with a non negative value. */
#define OCI_SUCCEEDED(code)    BOA_SUCCEEDED(code)

    /**  Generic test for a returned warning from the execution of an OCI-call. */
#define OCI_WARNED(code)       BOA_WARNED(code)

    /** @} */

    /**
    * @anchor ANCHOR_OCI_ERROR_TYPES
    * @name   OCI Error Type Flags and Masks
    * @brief  Special flags and masks of @ref OCI_ErrorCode to group an error into different types.
    *
    * @{
    */
#define OCI_ERR_TYPE_RESERVED           BOA_RESULT_TYPE_RESERVED  /**< used by old implementations. */
#define OCI_ERR_TYPE_PARAM              BOA_RESULT_TYPE_PARAM     /**< Parameter validation fails. */
#define OCI_ERR_TYPE_BIND               BOA_RESULT_TYPE_BIND      /**< Binding process of the API failed. When the CSI component
    is used for searching and binding this can be seen as an
    internal error. */
#define OCI_ERR_TYPE_SEMANTIC           BOA_RESULT_TYPE_SEMANTIC  /**< Semantic / Precondition validation fails. */
#define OCI_ERR_TYPE_RESOURCE           BOA_RESULT_TYPE_RESOURCE  /**< Driver runs out of resources. */
#define OCI_ERR_TYPE_COMMUNICATION      BOA_RESULT_TYPE_COMMUNICATION  /**< Communication problems to the interface hardware. */
#define OCI_ERR_TYPE_INTERNAL           BOA_RESULT_TYPE_INTERNAL  /**< Driver internal implementation error. */
#define OCI_ERR_TYPE_PRIVATE            BOA_RESULT_TYPE_PRIVATE  /**< Reserved for internal result codes. */

#define OCI_ERR_TYPE_MASK               BOA_RESULT_TYPE_MASK  /** < mask to unmask the error type information from all other
    error information to allow a generic type specific reaction */

#define OCI_ERR_TYPE(code)              BOA_RESULT_TYPE(code)  /**< separate the error type information */

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
#define OCI_SUCCESS                         BOA_SUCCESS

    /**
      Special @ref OCI_ErrorCode value to generally indicate a failure of the respective function call.
      @coding  Deprecated code. Prefer returning specific error codes.
    */
#define OCI_FAILURE                         BOA_FAILURE

    /*----------------------------------------------------------------------------------------------------------*/
    /**
      @anchor ANCHOR_OCI_ERROR_TYPE_PARAM
      @name   Member of the OCI Error Type @ref OCI_ERR_TYPE_PARAM.
      These error codes indicate an error in some parameter of a call.
      @{
    */

    /** @copydoc  BOA_ERR_INVALID_PARAMETER */
#define OCI_ERR_INVALID_PARAMETER           BOA_ERR_INVALID_PARAMETER

    /** @copydoc  BOA_ERR_INVALID_PARAMETER_SET */
#define OCI_ERR_INCONSISTENT_PARAMETER_SET  BOA_ERR_INCONSISTENT_PARAMETER_SET

    /** @copydoc  BOA_ERR_INVALID_HANDLE */
#define OCI_ERR_INVALID_HANDLE              BOA_ERR_INVALID_HANDLE

    /** @copydoc  BOA_ERR_BUFFER_OVERFLOW */
#define OCI_ERR_BUFFER_OVERFLOW             BOA_ERR_BUFFER_OVERFLOW

    /** @copydoc  BOA_ERR_UNSUPPORTED_PARAMETER */
#define OCI_ERR_UNSUPPORTED_PARAMETER       BOA_ERR_UNSUPPORTED_PARAMETER

    /** @copydoc  BOA_ERR_INVALID_FILTER */
#define OCI_ERR_INVALID_FILTER              BOA_ERR_INVALID_FILTER

    /** @copydoc  BOA_ERR_FILTER_UNKNOWN */
#define OCI_ERR_FILTER_UNKNOWN              BOA_ERR_FILTER_UNKNOWN

    /** @copydoc  BOA_WARN_INCONSISTENT_SELF_RECEPTION */
#define OCI_WARN_INCONSISTENT_SELF_RECEPTION BOA_WARN_INCONSISTENT_SELF_RECEPTION

    /** @} */

    /*----------------------------------------------------------------------------------------------------------*/
    /**
      @anchor ANCHOR_OCI_ERROR_TYPE_SEMANTIC
      @name   Member of the OCI Error Type @ref OCI_ERR_TYPE_SEMANTIC.
              These error codes indicate an error in some preconditions of the call.
      @{
     */

    /** @copydoc  BOA_ERR_INVALID_STATE */
#define OCI_ERR_INVALID_STATE               BOA_ERR_INVALID_STATE

    /** @copydoc  BOA_ERR_NO_CONFIG */
#define OCI_ERR_NO_CONFIG                   BOA_ERR_NO_CONFIG

#define OCI_ERR_CONTROLLER_IS_NOT_OPENED    OCI_ERR_NO_CONFIG

    /** @copydoc  BOA_ERR_QUEUE_IS_FULL */
#define OCI_ERR_QUEUE_IS_FULL               BOA_ERR_QUEUE_IS_FULL

    /** @} */

    /*----------------------------------------------------------------------------------------------------------*/
    /**
      @anchor ANCHOR_OCI_ERROR_TYPE_RESOURCE
      @name   Member of the OCI Error Type @ref OCI_ERR_TYPE_RESOURCE.
              These error codes indicate a resource problem in the driver.
      @{
    */

    /** @copydoc  BOA_ERR_INCOMPATIBLE_CONFIG */
#define OCI_ERR_INCOMPATIBLE_CONFIG         BOA_ERR_INCOMPATIBLE_CONFIG

    /** @copydoc  BOA_ERR_OUT_OF_MEMORY */
#define OCI_ERR_OUT_OF_MEMORY               BOA_ERR_OUT_OF_MEMORY

    /** @copydoc  BOA_ERR_NO_RESOURCES */
#define OCI_ERR_NO_RESOURCES                BOA_ERR_NO_RESOURCES

    /** @copydoc  BOA_WARN_PARAM_ADAPTED */
#define OCI_WARN_PARAM_ADAPTED              BOA_WARN_PARAM_ADAPTED

    /** @copydoc  BOA_WARN_NO_INORDER_TX */
#define OCI_WARN_NO_INORDER_TX              BOA_WARN_NO_INORDER_TX

    /** @} */

    /*----------------------------------------------------------------------------------------------------------*/
    /**
      @anchor ANCHOR_OCI_ERROR_TYPE_COMMUNICATION
      @name   Member of the OCI Error Type @ref OCI_ERR_TYPE_COMMUNICATION.
              These error codes indicate a problem in the communication to the hardware interface.
      @{
    */

    /** @copydoc  BOA_ERR_DRIVER_NO_RESPONSE */
#define OCI_ERR_DRIVER_NO_RESPONSE          BOA_ERR_DRIVER_NO_RESPONSE


    /** @copydoc  BOA_ERR_DRIVER_DISCONNECTED */
#define OCI_ERR_DRIVER_DISCONNECTED         BOA_ERR_DRIVER_DISCONNECTED

    /** @} */

    /*----------------------------------------------------------------------------------------------------------*/
    /**
      @anchor ANCHOR_OCI_ERROR_TYPE_INTERNAL
      @name   Member of the OCI Error Type @ref OCI_ERR_TYPE_INTERNAL.
              These error codes indicate a driver internal problem. Some internal plausibility checks detect an
              error although parameter of the call seem to be correct.
      @{
    */


    /** @copydoc  BOA_ERR_UNEXPECTED_NULL */
#define OCI_ERR_UNEXPECTED_NULL             BOA_ERR_UNEXPECTED_NULL

    /** @copydoc  BOA_ERR_HW_NOT_READY */
#define OCI_ERR_HW_NOT_READY                BOA_ERR_HW_NOT_READY

    /** @copydoc  BOA_ERR_OUT_OF_RANGE */
#define OCI_ERR_OUT_OF_RANGE                BOA_ERR_OUT_OF_RANGE

    /** @copydoc  BOA_ERR_INTERNAL_STATE */
#define OCI_ERR_INTERNAL_STATE              BOA_ERR_INTERNAL_STATE

    /** @copydoc  BOA_ERR_INTERNAL_HANDLE */
#define OCI_ERR_INTERNAL_HANDLE             BOA_ERR_INTERNAL_HANDLE

    /** @copydoc  BOA_ERR_INTERNAL_OVERFLOW */
#define OCI_ERR_INTERNAL_OVERFLOW           BOA_ERR_INTERNAL_OVERFLOW

    /** @copydoc  BOA_ERR_NOT_IMPLEMENTED */
#define OCI_ERR_NOT_IMPLEMENTED             BOA_ERR_NOT_IMPLEMENTED

    /** @copydoc  BOA_ERR_SYSTEM_ERROR */
#define OCI_ERR_SYSTEM_ERROR                BOA_ERR_SYSTEM_ERROR

    /** @copydoc  BOA_ERR_NO_DATA */
#define OCI_ERR_NO_DATA                     BOA_ERR_NO_DATA

    /** @copydoc  BOA_ERR_NOT_INITIALIZED */
#define OCI_ERR_NOT_INITIALIZED             BOA_ERR_NOT_INITIALIZED

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

    /** @copydoc  BOA_ERR_PROTOCOL_VERSION_NOT_SUPPORTED */
#define OCI_ERR_PROTOCOL_VERSION_NOT_SUPPORTED BOA_ERR_PROTOCOL_VERSION_NOT_SUPPORTED

    /** @copydoc  BOA_ERR_INVALID_ACCESS_SYNTAX */
#define OCI_ERR_INVALID_ACCESS_SYNTAX          BOA_ERR_INVALID_ACCESS_SYNTAX

    /** @copydoc  BOA_ERR_INVALID_ACCESS_PARAM */
#define OCI_ERR_INVALID_ACCESS_PARAM           BOA_ERR_INVALID_ACCESS_PARAM

    /** @copydoc  BOA_ERR_INVALID_TRANSFER_SYNTAX */
#define OCI_ERR_INVALID_TRANSFER_SYNTAX        BOA_ERR_INVALID_TRANSFER_SYNTAX

    /** @copydoc  BOA_ERR_NO_INTERFACE */
#define OCI_ERR_NO_INTERFACE                   BOA_ERR_NO_INTERFACE

    /** @copydoc  BOA_ERR_HW_NOT_PRESENT */
#define OCI_ERR_HW_NOT_PRESENT                 BOA_ERR_HW_NOT_PRESENT

    /** @copydoc  BOA_ERR_CANNOT_OPEN_DRIVER */
#define OCI_ERR_CANNOT_OPEN_DRIVER             BOA_ERR_CANNOT_OPEN_DRIVER

    /** @copydoc  BOA_ERR_LICENSE_MISSING */
#define OCI_ERR_LICENSE_MISSING                BOA_ERR_LICENSE_MISSING

    /** @copydoc  BOA_ERR_CANNOT_FIND_CHILDREN */
#define OCI_ERR_CANNOT_FIND_CHILDREN           BOA_ERR_CANNOT_FIND_CHILDREN

    /** @copydoc  BOA_ERR_NONUNIQUE_BIND_TARGET */
#define OCI_ERR_NONUNIQUE_BIND_TARGET          BOA_ERR_NONUNIQUE_BIND_TARGET

    /** @copydoc  BOA_WARN_OLDER_VERSION_IS_BETTER */
#define OCI_WARN_OLDER_VERSION_IS_BETTER       BOA_WARN_OLDER_VERSION_IS_BETTER

    /** @copydoc  BOA_WARN_API_V1_0_IS_UNAVAILABLE */
#define OCI_WARN_API_V1_0_IS_UNAVAILABLE       BOA_WARN_API_V1_0_IS_UNAVAILABLE

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
        char* utf8Text,
        uint32 size);
#endif

    typedef OCI_ErrorCode
    (OCI_CALL* PF_OCI_GetError)(
        OCI_Handle source,
        OCI_ErrorCode code,
        char* utf8Text,
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
        * Global time stamp when the event was triggered on the hardware. This may be OCI_NO_TIME if no timestamp was
        * available.
        */
        OCI_Time timeStamp;

        /**
        * Filter tag specified in @ref OCI_InternalErrorEventFilter.
        */
        uint32 tag;

        /** code for the event that generated this message */
        OCI_InternalErrorEvent eventCode;

        /**
        * The reason for the error. For example, @ref OCI_ERR_DRIVER_DISCONNECTED indicates that the connection to the
        * hardware device has been broken. @ref OCI_SUCCESS is used to indicate a recoverable error.
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
            <td> The @a filter parameter is NULL, references an invalid address or the filter already exists.</td>
       </tr>
       <tr> <td> @ref OCI_ERR_INCONSISTENT_PARAMETER_SET </td>
            <td> One or more filter objects are invalid or the same filter is defined more than once in the filter set.</td>
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
    (OCI_CALL* PF_OCI_AddInternalErrorEventFilter)(
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
       <tr> <td> @ref OCI_ERR_FILTER_UNKNOWN </td>
            <td> One or more @a filter objects are not set and can not be removed.</td>
       </tr>
       <tr> <td> @ref OCI_ERR_INCONSISTENT_PARAMETER_SET </td>
            <td> One or more @a filter objects are invalid or the same filter is defined more than once in the filter set.</td>
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
    */

#ifndef OCI_DYNAMICDLL
    OCI_DECLSPEC OCI_ErrorCode
    OCI_CALL OCI_RemoveInternalErrorEventFilter(
        OCI_QueueHandle queue,
        OCI_InternalErrorEventFilter filter[],
        uint32 count);
#endif

    typedef OCI_ErrorCode
    (OCI_CALL* PF_OCI_RemoveInternalErrorEventFilter)(
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

#endif
