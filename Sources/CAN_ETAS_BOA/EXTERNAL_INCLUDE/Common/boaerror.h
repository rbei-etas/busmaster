#ifndef ETAS_BOAERROR_H
#define ETAS_BOAERROR_H

/**
* @file       boaerror.h
* @brief      Error codes and types of the Basic OpenAPI (BOA).
* @copyright  Copyright (c) 2007-2009 ETAS GmbH. All rights reserved.
* @todo       The dependencies must be changed. Intrinsically the special 
*             declaration "ocierror.h" depends on the common declaration 
*             "boaerror.h".
*
* $Revision: 4838 $
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

/** @copydoc  OCI_ERR_INVALID_PARAMETER */
#define BOA_ERR_INVALID_PARAMETER               (BOA_RESULT_FLAG_ERROR | BOA_RESULT_TYPE_PARAM | 0)

/** Some Parameter is inplausible. It uses a inplausible combination of settings. */
#define BOA_ERR_INCONSISTENT_PARAMETER_SET      (BOA_RESULT_FLAG_ERROR | BOA_RESULT_TYPE_PARAM | 1)

/** @copydoc  OCI_ERR_INVALID_HANDLE */
#define BOA_ERR_INVALID_HANDLE                  (BOA_RESULT_FLAG_ERROR | BOA_RESULT_TYPE_PARAM | 2)

/** @copydoc  OCI_ERR_BUFFER_OVERFLOW */
#define BOA_ERR_BUFFER_OVERFLOW                 (BOA_RESULT_FLAG_ERROR | BOA_RESULT_TYPE_PARAM | 3)

/** @copydoc  OCI_ERR_UNSUPPORTED_PARAMETER */
#define BOA_ERR_UNSUPPORTED_PARAMETER           (BOA_RESULT_FLAG_ERROR | BOA_RESULT_TYPE_PARAM | 4)

/** @} */


/** 
* @anchor ANCHOR_BOA_RESULT_TYPE_SEMANTIC
* @name   Member of the BOA Result Type @ref BOA_RESULT_TYPE_SEMANTIC. 
* These error codes indicate an error in some preconditions of the call.
* @{
*/

/** @copydoc  OCI_ERR_INVALID_STATE */
#define BOA_ERR_INVALID_STATE                   (BOA_RESULT_FLAG_ERROR | BOA_RESULT_TYPE_SEMANTIC | 0)

/** @copydoc  OCI_ERR_NO_CONFIG */
#define BOA_ERR_NO_CONFIG                       (BOA_RESULT_FLAG_ERROR | BOA_RESULT_TYPE_SEMANTIC | 1)

/** @copydoc  OCI_ERR_QUEUE_IS_FULL */
#define BOA_ERR_QUEUE_IS_FULL                   (BOA_RESULT_FLAG_ERROR | BOA_RESULT_TYPE_SEMANTIC | 2)

/** @} */


/**
* @anchor ANCHOR_BOA_RESULT_TYPE_RESOURCE
* @name   Member of the BOA Result Type @ref BOA_RESULT_TYPE_RESOURCE. 
* These error codes indicate a resource problem in the driver.
* @{
*/

/** @copydoc  OCI_ERR_INCOMPATIBLE_CONFIG */
#define BOA_ERR_INCOMPATIBLE_CONFIG             (BOA_RESULT_FLAG_ERROR | BOA_RESULT_TYPE_RESOURCE | 0)

/** @copydoc  OCI_ERR_OUT_OF_MEMORY */
#define BOA_ERR_OUT_OF_MEMORY                   (BOA_RESULT_FLAG_ERROR | BOA_RESULT_TYPE_RESOURCE | 1)

/** @copydoc  OCI_ERR_NO_RESOURCES */
#define BOA_ERR_NO_RESOURCES                    (BOA_RESULT_FLAG_ERROR | BOA_RESULT_TYPE_RESOURCE | 2)

/** @} */


/**
* @anchor ANCHOR_BOA_RESULT_TYPE_COMMUNICATION
* @name   Member of the BOA Result Type @ref BOA_RESULT_TYPE_COMMUNICATION. 
* These error codes indicate a problem in the communication to the hardware interface.
* @{
*/

/** @copydoc  OCI_ERR_DRIVER_NO_RESPONSE */
#define BOA_ERR_DRIVER_NO_RESPONSE              (BOA_RESULT_FLAG_ERROR | BOA_RESULT_TYPE_COMMUNICATION | 0)

/** @copydoc  OCI_ERR_DRIVER_DISCONNECTED */
#define BOA_ERR_DRIVER_DISCONNECTED             (BOA_RESULT_FLAG_ERROR | BOA_RESULT_TYPE_COMMUNICATION | 1)

/** @} */


/**
* @anchor ANCHOR_BOA_RESULT_TYPE_INTERNAL
* @name   Member of the BOA Result Type @ref BOA_RESULT_TYPE_INTERNAL. 
* These error codes indicate a driver internal problem. Some internal 
* plausibility checks detect an error although parameter of the call seem to 
* be correct.
* @{
*/

/** @copydoc  OCI_ERR_UNEXPECTED_NULL */
#define BOA_ERR_UNEXPECTED_NULL                 (BOA_RESULT_FLAG_ERROR | BOA_RESULT_TYPE_INTERNAL | 0)

/** @copydoc  OCI_ERR_HW_NOT_READY */
#define BOA_ERR_HW_NOT_READY                    (BOA_RESULT_FLAG_ERROR | BOA_RESULT_TYPE_INTERNAL | 1)

/** @copydoc  OCI_ERR_OUT_OF_RANGE */
#define BOA_ERR_OUT_OF_RANGE                    (BOA_RESULT_FLAG_ERROR | BOA_RESULT_TYPE_INTERNAL | 2)

/** @copydoc  OCI_ERR_INTERNAL_STATE */
#define BOA_ERR_INTERNAL_STATE                  (BOA_RESULT_FLAG_ERROR | BOA_RESULT_TYPE_INTERNAL | 3)

/** @copydoc  OCI_ERR_INTERNAL_HANDLE */
#define BOA_ERR_INTERNAL_HANDLE                 (BOA_RESULT_FLAG_ERROR | BOA_RESULT_TYPE_INTERNAL | 4)

/** @copydoc  OCI_ERR_INTERNAL_OVERFLOW */
#define BOA_ERR_INTERNAL_OVERFLOW               (BOA_RESULT_FLAG_ERROR | BOA_RESULT_TYPE_INTERNAL | 5)

/** @copydoc  OCI_ERR_NOT_IMPLEMENTED */
#define BOA_ERR_NOT_IMPLEMENTED                 (BOA_RESULT_FLAG_ERROR | BOA_RESULT_TYPE_INTERNAL | 6)

/** @copydoc  OCI_ERR_SYSTEM_ERROR */
#define BOA_ERR_SYSTEM_ERROR                    (BOA_RESULT_FLAG_ERROR | BOA_RESULT_TYPE_INTERNAL | 7)

/** @copydoc  OCI_ERR_NO_DATA */
#define BOA_ERR_NO_DATA                         (BOA_RESULT_FLAG_ERROR | BOA_RESULT_TYPE_INTERNAL | 8)

/** @copydoc  OCI_ERR_NOT_INITIALIZED */
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

/** @copydoc  OCI_ERR_PROTOCOL_VERSION_NOT_SUPPORTED */
#define BOA_ERR_PROTOCOL_VERSION_NOT_SUPPORTED  (BOA_RESULT_FLAG_ERROR | BOA_RESULT_TYPE_BIND | 0)

/** @copydoc  OCI_ERR_INVALID_ACCESS_SYNTAX */
#define BOA_ERR_INVALID_ACCESS_SYNTAX           (BOA_RESULT_FLAG_ERROR | BOA_RESULT_TYPE_BIND | 1)

/** @copydoc  OCI_ERR_INVALID_ACCESS_PARAM */
#define BOA_ERR_INVALID_ACCESS_PARAM            (BOA_RESULT_FLAG_ERROR | BOA_RESULT_TYPE_BIND | 2)

/** @copydoc  OCI_ERR_INVALID_TRANSFER_SYNTAX */
#define BOA_ERR_INVALID_TRANSFER_SYNTAX         (BOA_RESULT_FLAG_ERROR | BOA_RESULT_TYPE_BIND | 3)

/** @copydoc  OCI_ERR_NO_INTERFACE */
#define BOA_ERR_NO_INTERFACE                    (BOA_RESULT_FLAG_ERROR | BOA_RESULT_TYPE_BIND | 4)

/** @copydoc  OCI_ERR_HW_NOT_PRESENT */
#define BOA_ERR_HW_NOT_PRESENT                  (BOA_RESULT_FLAG_ERROR | BOA_RESULT_TYPE_BIND | 5)

/** @copydoc  OCI_ERR_CANNOT_OPEN_DRIVER */
#define BOA_ERR_CANNOT_OPEN_DRIVER              (BOA_RESULT_FLAG_ERROR | BOA_RESULT_TYPE_BIND | 6)

/** @copydoc  OCI_ERR_LICENSE_MISSING */
#define BOA_ERR_LICENSE_MISSING                 (BOA_RESULT_FLAG_ERROR | BOA_RESULT_TYPE_BIND | 7)

/** @copydoc  OCI_ERR_CANNOT_FIND_CHILDREN */
#define BOA_ERR_CANNOT_FIND_CHILDREN            (BOA_RESULT_FLAG_ERROR | BOA_RESULT_TYPE_BIND | 8)

/** @} */

/** 
* @} 
*/

#ifdef __cplusplus
}
#endif

#endif
