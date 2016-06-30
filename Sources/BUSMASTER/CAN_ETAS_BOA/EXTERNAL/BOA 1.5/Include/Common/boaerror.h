#ifndef ETAS_BOAERROR_H
#define ETAS_BOAERROR_H

/**
* @file       boaerror.h
* @brief      Error codes and types of the Basic OpenAPI (BOA).
* @copyright  Copyright (c) 2007-2009 ETAS GmbH. All rights reserved.
*
* $Revision: 6144 $
*/

#include "boatypes.h"

#ifdef __cplusplus
extern "C" {
#endif

    /**
    * @addtogroup GROUP_BOA_ERROR_MANAGEMENT Error Management
    *
    * @{
    *
    */

    /**
    * @anchor ANCHOR_BOA_RESULT_FLAGS
    * @name   BOA Result Code Flags and Masks
    * @brief  Special flags and masks of @ref BOA_ResultCode to classify a
    *         result value.
    * @{
    */

    /**
    * Flag in result code, which classifies a result as an error.
    */
#define BOA_RESULT_FLAG_ERROR      0x80000000

    /**
    * Flag in result code, which classifies a result as a warning.
    */
#define BOA_RESULT_FLAG_WARNING    0x40000000

    /**
    * Provides a generic test for failure on any result code.
    */
#define BOA_FAILED(code)           ((int32)(code) < 0)

    /**
    * Provides a generic test for success on any result code.
    */
#define BOA_SUCCEEDED(code)        ((int32)(code) >= 0)

    /**
    * Generic test for a returned warning from the execution of an BOA-call.
    */
#define BOA_WARNED(code)           ((code) & BOA_RESULT_FLAG_WARNING)

    /** @} */


    /**
    * @anchor ANCHOR_BOA_RESULT_TYPES
    * @name   BOA Result Type Flags and Masks
    * @brief  Special flags and masks of @ref BOA_ResultCode to group an result
    *         into different types.
    * @{
    */

    /**
    * used by old boa implementations (01/2009).
    */
#define BOA_RESULT_TYPE_RESERVED        0x00000000
    /**
    * Parameter validation fails.
    */
#define BOA_RESULT_TYPE_PARAM           0x00001000
    /**
    * Binding process of the API failed. When the CSI component is used for
    * searching and binding this can be seen as an internal error.
    */
#define BOA_RESULT_TYPE_BIND            0x00002000
    /**
    * Semantic / Precondition validation fails.
    */
#define BOA_RESULT_TYPE_SEMANTIC        0x00003000
    /**
    * Driver runs out of resources.
    */
#define BOA_RESULT_TYPE_RESOURCE        0x00004000
    /**
    * Communication problems to the interface hardware.
    */
#define BOA_RESULT_TYPE_COMMUNICATION   0x00005000
    /**
    * Driver internal implementation error.
    */
#define BOA_RESULT_TYPE_INTERNAL        0x00006000
    /*
    * Reserved for internal result codes.
    */
#define BOA_RESULT_TYPE_PRIVATE         0x0000F000

    /**
    * Mask to unmask the error type information from all other
    * error information to allow a generic type specific reaction
    */
#define BOA_RESULT_TYPE_MASK            0x0000F000
    /**
    * Separate the error type information.
    */
#define BOA_RESULT_TYPE(code)           ((code) & BOA_RESULT_TYPE_MASK)

    /** @} */


    /**
    * @anchor ANCHOR_BOA_RESULT_CODES
    * @name   BOA Result Codes
    * @brief  Predefined values of @ref BOA_ResultCode to specify a result value.
    * @{
    */

    /**
    * Special value to indicate that the respective function executed without error.
    */
#define BOA_SUCCESS    (0)

    /**
    * Special value to generally indicate a failure of the respective function call.
    * @coding  Deprecated code. Prefer returning specific result codes.
    */
#define BOA_FAILURE    (BOA_RESULT_FLAG_ERROR)


    /**
    * @anchor ANCHOR_BOA_RESULT_TYPE_PARAM
    * @name   Member of the BOA Result Type @ref BOA_RESULT_TYPE_PARAM.
    * These error codes indicate an error in some parameter of a call.
    * @{
    */


    /** At least one parameter is wrong or invalid. Typically this error occurs, if a parameter of a function of
        the OCI API is @c NULL, not initialized or unexpected. */
#define BOA_ERR_INVALID_PARAMETER               (BOA_RESULT_FLAG_ERROR | BOA_RESULT_TYPE_PARAM | 0)

    /** Some Parameter is inplausible. It uses a inplausible combination of settings. */
#define BOA_ERR_INCONSISTENT_PARAMETER_SET      (BOA_RESULT_FLAG_ERROR | BOA_RESULT_TYPE_PARAM | 1)

    /** The parameter is no valid handle or the used handle has the wrong type.
        (e.g.\ A OCI_ControllerHandle is used in a function call that expects a OCI_QueueHandle) */
#define BOA_ERR_INVALID_HANDLE                  (BOA_RESULT_FLAG_ERROR | BOA_RESULT_TYPE_PARAM | 2)

    /** The data was too large to fit into the specified buffer. */
#define BOA_ERR_BUFFER_OVERFLOW                 (BOA_RESULT_FLAG_ERROR | BOA_RESULT_TYPE_PARAM | 3)

    /** At least one parameter is not supported. */
#define BOA_ERR_UNSUPPORTED_PARAMETER           (BOA_RESULT_FLAG_ERROR | BOA_RESULT_TYPE_PARAM | 4)

    /** A filter destination definition is incorrect. */
#define BOA_ERR_INVALID_FILTER                  (BOA_RESULT_FLAG_ERROR | BOA_RESULT_TYPE_PARAM | 5)

    /** One or more filter objects are not set and can not be removed. The processing of the filter array is undefined. */
#define BOA_ERR_FILTER_UNKNOWN                  (BOA_RESULT_FLAG_ERROR | BOA_RESULT_TYPE_PARAM | 6)

    /** Self-reception is configured differently for a controller and an RX queue. One enables self-reception
     * while the other disables it. */
#define BOA_WARN_INCONSISTENT_SELF_RECEPTION    (BOA_RESULT_FLAG_WARNING | BOA_RESULT_TYPE_PARAM | 0)

    /** @} */


    /**
    * @anchor ANCHOR_BOA_RESULT_TYPE_SEMANTIC
    * @name   Member of the BOA Result Type @ref BOA_RESULT_TYPE_SEMANTIC.
    * These error codes indicate an error in some preconditions of the call.
    * @{
    */

    /** The driver is not in a valid state to perform this request. */
#define BOA_ERR_INVALID_STATE                   (BOA_RESULT_FLAG_ERROR | BOA_RESULT_TYPE_SEMANTIC | 0)

    /**  The controller interface is not opened, no valid configuration is available. */
#define BOA_ERR_NO_CONFIG                       (BOA_RESULT_FLAG_ERROR | BOA_RESULT_TYPE_SEMANTIC | 1)

    /**  The queue is full. */
#define BOA_ERR_QUEUE_IS_FULL                   (BOA_RESULT_FLAG_ERROR | BOA_RESULT_TYPE_SEMANTIC | 2)

    /** @} */


    /**
    * @anchor ANCHOR_BOA_RESULT_TYPE_RESOURCE
    * @name   Member of the BOA Result Type @ref BOA_RESULT_TYPE_RESOURCE.
    * These error codes indicate a resource problem in the driver.
    * @{
    */

    /** The call tries to use incompatible settings. A second OCI-Instance or a different client has locked the
        configuration. */
#define BOA_ERR_INCOMPATIBLE_CONFIG             (BOA_RESULT_FLAG_ERROR | BOA_RESULT_TYPE_RESOURCE | 0)

    /** No memory resources available. */
#define BOA_ERR_OUT_OF_MEMORY                   (BOA_RESULT_FLAG_ERROR | BOA_RESULT_TYPE_RESOURCE | 1)

    /** No resources available. */
#define BOA_ERR_NO_RESOURCES                    (BOA_RESULT_FLAG_ERROR | BOA_RESULT_TYPE_RESOURCE | 2)

    /** The caller tries to create an object (file, licence, etc.) that already exists. */
#define BOA_ERR_OBJECT_EXISTS                   (BOA_RESULT_FLAG_ERROR | BOA_RESULT_TYPE_RESOURCE | 3)

    /** The function could not use one of the supplied arguments, but was able to proceed by adapting the argument
        in a compatible manner. */
#define BOA_WARN_PARAM_ADAPTED                  (BOA_RESULT_FLAG_WARNING | BOA_RESULT_TYPE_RESOURCE | 0)

    /** The function will use the configuration which was supplied in its arguments, but cannot guarantee in-order
        sending behavior on the affected queue. */
#define BOA_WARN_NO_INORDER_TX                  (BOA_RESULT_FLAG_WARNING | BOA_RESULT_TYPE_RESOURCE | 1)

    /** @} */


    /**
    * @anchor ANCHOR_BOA_RESULT_TYPE_COMMUNICATION
    * @name   Member of the BOA Result Type @ref BOA_RESULT_TYPE_COMMUNICATION.
    * These error codes indicate a problem in the communication to the hardware interface.
    * @{
    */

    /** The driver for a removable hardware received no response to a request. The hardware may be removed. There
        is no information whether the request is executed or not. The request may be lost or the response may be lost.
        All subsequent calls to the same controller will deliver @ref BOA_ERR_DRIVER_DISCONNECTED. The state of the
        controller is set to @a DISCONNECTED. */
#define BOA_ERR_DRIVER_NO_RESPONSE              (BOA_RESULT_FLAG_ERROR | BOA_RESULT_TYPE_COMMUNICATION | 0)

    /** The driver for a removable hardware detected a communication problem (in asynchromous communication,
        by event signaling or during a previous call to the same hardware) and is not able to communicate to
        the hardware any more. The state of the session is undefined. All subsequent calls to the same
        Controller will return @ref BOA_ERR_DRIVER_DISCONNECTED. Internal resources are locked until
        OCI_DestroyXXXController() for the controller handle is called. */
#define BOA_ERR_DRIVER_DISCONNECTED             (BOA_RESULT_FLAG_ERROR | BOA_RESULT_TYPE_COMMUNICATION | 1)


    /** The called function is temporarily unable to complete the request; please try again later*/
#define BOA_ERR_BUSY             (BOA_RESULT_FLAG_ERROR | BOA_RESULT_TYPE_COMMUNICATION | 2)


    /** @} */


    /**
    * @anchor ANCHOR_BOA_RESULT_TYPE_INTERNAL
    * @name   Member of the BOA Result Type @ref BOA_RESULT_TYPE_INTERNAL.
    * These error codes indicate a driver internal problem. Some internal
    * plausibility checks detect an error although parameter of the call seem to
    * be correct.
    * @{
    */

    /** Some driver internal software related error occurred. Internal plausibility checks failed. The driver will
        not work correct. This is a fatal error condition. */
#define BOA_ERR_UNEXPECTED_NULL                 (BOA_RESULT_FLAG_ERROR | BOA_RESULT_TYPE_INTERNAL | 0)

    /** Some driver internal hardware related error occurred. Internal checks, communication between sub-systems,
    access to hardware resources failed or timed out. */
#define BOA_ERR_HW_NOT_READY                    (BOA_RESULT_FLAG_ERROR | BOA_RESULT_TYPE_INTERNAL | 1)

    /** An index or value is out of legal range. */
#define BOA_ERR_OUT_OF_RANGE                    (BOA_RESULT_FLAG_ERROR | BOA_RESULT_TYPE_INTERNAL | 2)

    /** The driver is not in a valid state to perform this request. */
#define BOA_ERR_INTERNAL_STATE                  (BOA_RESULT_FLAG_ERROR | BOA_RESULT_TYPE_INTERNAL | 3)

    /** A used internal handle is not valid or has the wrong type. */
#define BOA_ERR_INTERNAL_HANDLE                 (BOA_RESULT_FLAG_ERROR | BOA_RESULT_TYPE_INTERNAL | 4)

    /** Overflow of an internal buffer. */
#define BOA_ERR_INTERNAL_OVERFLOW               (BOA_RESULT_FLAG_ERROR | BOA_RESULT_TYPE_INTERNAL | 5)

    /** The requested function is not implemented. This is a deprecated error code, that must not occur in a
        BOA compatible driver. The BOA defines no optional API functions. The code may be used during driver
        development. */
#define BOA_ERR_NOT_IMPLEMENTED                 (BOA_RESULT_FLAG_ERROR | BOA_RESULT_TYPE_INTERNAL | 6)

    /** Error from an underlying driver or operating system. */
#define BOA_ERR_SYSTEM_ERROR                    (BOA_RESULT_FLAG_ERROR | BOA_RESULT_TYPE_INTERNAL | 7)

    /** No data available. */
#define BOA_ERR_NO_DATA                         (BOA_RESULT_FLAG_ERROR | BOA_RESULT_TYPE_INTERNAL | 8)

    /** Data is not initialized. */
#define BOA_ERR_NOT_INITIALIZED                 (BOA_RESULT_FLAG_ERROR | BOA_RESULT_TYPE_INTERNAL | 9)




    /** @} */


    /**
    * @anchor ANCHOR_BOA_RESULT_TYPE_BIND
    * @name   Member of the BOA Result Type @ref BOA_RESULT_TYPE_BIND.
    * These error codes indicate a problem during binding process. When the CSI
    * components are used for detecting the components and the result of this
    * search is used for binding, the error type indicates an internal error.
    * When the binding is not based on the search result the error type gives
    * detailed information for the binding failure.
    * @{
    */

    /** The server offers the interface with the requested UUID, but the can not fullfill the required version. */
#define BOA_ERR_PROTOCOL_VERSION_NOT_SUPPORTED  (BOA_RESULT_FLAG_ERROR | BOA_RESULT_TYPE_BIND | 0)

    /** The parsing of the access parameter failed. The driver can not interpret the parameter*/
#define BOA_ERR_INVALID_ACCESS_SYNTAX           (BOA_RESULT_FLAG_ERROR | BOA_RESULT_TYPE_BIND | 1)

    /** An element of the access parameter can not be found/resolved in the protocol stack. The connection can
        not be established */
#define BOA_ERR_INVALID_ACCESS_PARAM            (BOA_RESULT_FLAG_ERROR | BOA_RESULT_TYPE_BIND | 2)

    /** The server offers the interface with the requested UUID, but does not support the required binding mechanism
        for this interface. The interface can not be used. */
#define BOA_ERR_INVALID_TRANSFER_SYNTAX         (BOA_RESULT_FLAG_ERROR | BOA_RESULT_TYPE_BIND | 3)

    /** The interface with the requested UUID is not supported by the addressed node. */
#define BOA_ERR_NO_INTERFACE                    (BOA_RESULT_FLAG_ERROR | BOA_RESULT_TYPE_BIND | 4)

    /** The requested hardware is not present. */
#define BOA_ERR_HW_NOT_PRESENT                  (BOA_RESULT_FLAG_ERROR | BOA_RESULT_TYPE_BIND | 5)

    /** The attempt to load or open the driver failed. */
#define BOA_ERR_CANNOT_OPEN_DRIVER              (BOA_RESULT_FLAG_ERROR | BOA_RESULT_TYPE_BIND | 6)

    /** The validation of a license failed. */
#define BOA_ERR_LICENSE_MISSING                 (BOA_RESULT_FLAG_ERROR | BOA_RESULT_TYPE_BIND | 7)

    /** An error occurred when attempting to enumerate the children of a node. */
#define BOA_ERR_CANNOT_FIND_CHILDREN            (BOA_RESULT_FLAG_ERROR | BOA_RESULT_TYPE_BIND | 8)

    /** The target of a bind operation could not be uniquely identified. For example, the URI used to identify the
     * bind target may not identify a unique location in the tree. */
#define BOA_ERR_NONUNIQUE_BIND_TARGET           (BOA_RESULT_FLAG_ERROR | BOA_RESULT_TYPE_BIND | 9)

    /** The requested API version was bound successfully, but a "better" (in some case-specific sense) implementation
     * of the API could be obtained if the caller bound an earlier version of the API instead of the requested
     * version. */
#define BOA_WARN_OLDER_VERSION_IS_BETTER        (BOA_RESULT_FLAG_WARNING | BOA_RESULT_TYPE_BIND | 0)

    /** The requested API version was bound successfully, but it was noted that v1.0.0.0 of the same API
     * is not available at all. While this causes no immediate problems, it may cause errors at other
     * points in the future. */
#define BOA_WARN_API_V1_0_IS_UNAVAILABLE        (BOA_RESULT_FLAG_WARNING | BOA_RESULT_TYPE_BIND | 1)

    /** @} */

    /**
    * @}
    */

#ifdef __cplusplus
}
#endif

#endif
