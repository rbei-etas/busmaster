#ifndef ETAS_BOAERROR_H
#define ETAS_BOAERROR_H

/**
* @file       boaerror.h
* @brief      Error codes and types of the Basic OpenAPI (BOA).
* @copyright  Copyright (c) 2007-2009 ETAS GmbH. All rights reserved.
* @todo       The dependencies must be changed. Intrinsically the special 
*             declaration "ocierror.h" depends on the common declaration 
*             "boaerror.h".
*/

#include "stdtypes.h"
#include "boatypes.h"
#include "../OCI/ocierror.h"

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
* Return code used by BOA interfaces. It is zero upon success and nonzero 
* to represent an error code or status information. 
* @coding  Use negative values for error codes to support the usage of 
*          FAILED() and SUCCEEDED() macros under Windows. 
*          The practice of the flags @ref BOA_RESULT_FLAG_ERROR,
*          @ref BOA_RESULT_FLAG_WARNING enable this feature. 
* @n       Moreover there are BOA specific macros @ref BOA_FAILED() 
*          and @ref BOA_SUCCEEDED() with the same functionality such 
*          as FAILED() and SUCCEEDED(), which should be preferred. 
* @ingroup GROUP_BOA_ERROR_MANAGEMENT
*/

typedef uint32 BOA_ResultCode;


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
#define BOA_RESULT_FLAG_ERROR      OCI_ERR_FLAG_ERROR

/** 
* Flag in result code, which classifies a result as a warning. 
*/
#define BOA_RESULT_FLAG_WARNING    OCI_ERR_FLAG_WARNING

/**  
* Provides a generic test for failure on any result code. 
*/
#define BOA_FAILED(code)           OCI_FAILED(code)

/** 
* Provides a generic test for success on any result code.
*/
#define BOA_SUCCEEDED(code)        OCI_SUCCEEDED(code)

/**  
* Generic test for a returned warning from the execution of an OCI-call. 
*/
#define BOA_WARNED(code)           OCI_WARNED(code)

/** @} */


/**
* @anchor ANCHOR_BOA_RESULT_TYPES
* @name   BOA Result Type Flags and Masks
* @brief  Special flags and masks of @ref BOA_ResultCode to group an result 
*         into different types.
* @{
*/

/** 
* Currently used for error codes. 
*/
#define BOA_RESULT_TYPE_RESERVED        OCI_ERR_TYPE_RESERVED  
/** 
* Parameter validation fails. 
*/
#define BOA_RESULT_TYPE_PARAM           OCI_ERR_TYPE_PARAM
/** 
* Binding process of the API failed. When the CSI component is used for 
* searching and binding this can be seen as an internal error. 
*/
#define BOA_RESULT_TYPE_BIND            OCI_ERR_TYPE_BIND
/** 
* Semantic / Precondition validation fails.
*/
#define BOA_RESULT_TYPE_SEMANTIC        OCI_ERR_TYPE_SEMANTIC
/** 
* Driver runs out of resources.
*/
#define BOA_RESULT_TYPE_RESOURCE        OCI_ERR_TYPE_RESOURCE
/**
* Communication problems to the interface hardware.
*/
#define BOA_RESULT_TYPE_COMMUNICATION   OCI_ERR_TYPE_COMMUNICATION  
/** 
* Driver internal implementation error.
*/
#define BOA_RESULT_TYPE_INTERNAL        OCI_ERR_TYPE_INTERNAL 
/*
* Reserved for internal result codes.
*/
#define BOA_RESULT_TYPE_PRIVATE         OCI_ERR_TYPE_PRIVATE

/** 
* Mask to unmask the error type information from all other
* error information to allow a generic type specific reaction 
*/  
#define BOA_RESULT_TYPE_MASK            OCI_ERR_TYPE_MASK
/**
* Separate the error type information.
*/
#define BOA_RESULT_TYPE(code)           OCI_ERR_TYPE(code)

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
#define BOA_SUCCESS    OCI_SUCCESS

/** 
* Special value to generally indicate a failure of the respective function call.
* @coding  Deprecated code. Prefer returning specific result codes.
*/
#define BOA_FAILURE    OCI_FAILURE


/**
* @anchor ANCHOR_BOA_RESULT_TYPE_PARAM
* @name   Member of the BOA Result Type @ref BOA_RESULT_TYPE_PARAM. 
* These error codes indicate an error in some parameter of a call.
* @{
*/

/** @copydoc  OCI_ERR_INVALID_PARAMETER */
#define BOA_ERR_INVALID_PARAMETER               OCI_ERR_INVALID_PARAMETER

/** @copydoc  OCI_ERR_UNSUPPORTED_PARAMETER */
#define BOA_ERR_UNSUPPORTED_PARAMETER           OCI_ERR_UNSUPPORTED_PARAMETER

/** @copydoc  OCI_ERR_INCONSISTENT_PARAMETER_SET */
#define BOA_ERR_INCONSISTENT_PARAMETER_SET      OCI_ERR_INCONSISTENT_PARAMETER_SET

/** @copydoc  OCI_ERR_INVALID_HANDLE */
#define BOA_ERR_INVALID_HANDLE                  OCI_ERR_INVALID_HANDLE

/** @copydoc  OCI_ERR_BUFFER_OVERFLOW */
#define BOA_ERR_BUFFER_OVERFLOW                 OCI_ERR_BUFFER_OVERFLOW

/** @} */


/** 
* @anchor ANCHOR_BOA_RESULT_TYPE_SEMANTIC
* @name   Member of the BOA Result Type @ref BOA_RESULT_TYPE_SEMANTIC. 
* These error codes indicate an error in some preconditions of the call.
* @{
*/

/** @copydoc  OCI_ERR_INVALID_STATE */
#define BOA_ERR_INVALID_STATE                   OCI_ERR_INVALID_STATE

/** @copydoc  OCI_ERR_NO_CONFIG */
#define BOA_ERR_NO_CONFIG                       OCI_ERR_NO_CONFIG

/** @copydoc  OCI_ERR_QUEUE_IS_FULL */
#define BOA_ERR_QUEUE_IS_FULL                   OCI_ERR_QUEUE_IS_FULL

/** @} */


/**
* @anchor ANCHOR_BOA_RESULT_TYPE_RESOURCE
* @name   Member of the BOA Result Type @ref BOA_RESULT_TYPE_RESOURCE. 
* These error codes indicate a resource problem in the driver.
* @{
*/

/** @copydoc  OCI_ERR_INCOMPATIBLE_CONFIG */
#define BOA_ERR_INCOMPATIBLE_CONFIG             OCI_ERR_INCOMPATIBLE_CONFIG

/** @copydoc  OCI_ERR_OUT_OF_MEMORY */
#define BOA_ERR_OUT_OF_MEMORY                   OCI_ERR_OUT_OF_MEMORY

/** @copydoc  OCI_ERR_NO_RESOURCES */
#define BOA_ERR_NO_RESOURCES                    OCI_ERR_NO_RESOURCES

/** @} */


/**
* @anchor ANCHOR_BOA_RESULT_TYPE_COMMUNICATION
* @name   Member of the BOA Result Type @ref BOA_RESULT_TYPE_COMMUNICATION. 
* These error codes indicate a problem in the communication to the hardware interface.
* @{
*/

/** @copydoc  OCI_ERR_DRIVER_NO_RESPONSE */
#define BOA_ERR_DRIVER_NO_RESPONSE              OCI_ERR_DRIVER_NO_RESPONSE

/** @copydoc  OCI_ERR_DRIVER_DISCONNECTED */
#define BOA_ERR_DRIVER_DISCONNECTED             OCI_ERR_DRIVER_DISCONNECTED

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
#define BOA_ERR_UNEXPECTED_NULL                 OCI_ERR_UNEXPECTED_NULL

/** @copydoc  OCI_ERR_HW_NOT_READY */
#define BOA_ERR_HW_NOT_READY                    OCI_ERR_HW_NOT_READY

/** @copydoc  OCI_ERR_OUT_OF_RANGE */
#define BOA_ERR_OUT_OF_RANGE                    OCI_ERR_OUT_OF_RANGE

/** @copydoc  OCI_ERR_INTERNAL_STATE */
#define BOA_ERR_INTERNAL_STATE                  OCI_ERR_INTERNAL_STATE

/** @copydoc  OCI_ERR_INTERNAL_HANDLE */
#define BOA_ERR_INTERNAL_HANDLE                 OCI_ERR_INTERNAL_HANDLE

/** @copydoc  OCI_ERR_INTERNAL_OVERFLOW */
#define BOA_ERR_INTERNAL_OVERFLOW               OCI_ERR_INTERNAL_OVERFLOW

/** @copydoc  OCI_ERR_NOT_IMPLEMENTED */
#define BOA_ERR_NOT_IMPLEMENTED                 OCI_ERR_NOT_IMPLEMENTED

/** @copydoc  OCI_ERR_SYSTEM_ERROR */
#define BOA_ERR_SYSTEM_ERROR                    OCI_ERR_SYSTEM_ERROR

/** @copydoc  OCI_ERR_NO_DATA */
#define BOA_ERR_NO_DATA                         OCI_ERR_NO_DATA

/** @copydoc  OCI_ERR_NOT_INITIALIZED */
#define BOA_ERR_NOT_INITIALIZED                 OCI_ERR_NOT_INITIALIZED




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
#define BOA_ERR_PROTOCOL_VERSION_NOT_SUPPORTED  OCI_ERR_PROTOCOL_VERSION_NOT_SUPPORTED

/** @copydoc  OCI_ERR_INVALID_ACCESS_SYNTAX */
#define BOA_ERR_INVALID_ACCESS_SYNTAX           OCI_ERR_INVALID_ACCESS_SYNTAX

/** @copydoc  OCI_ERR_INVALID_ACCESS_PARAM */
#define BOA_ERR_INVALID_ACCESS_PARAM            OCI_ERR_INVALID_ACCESS_PARAM

/** @copydoc  OCI_ERR_INVALID_TRANSFER_SYNTAX */
#define BOA_ERR_INVALID_TRANSFER_SYNTAX         OCI_ERR_INVALID_TRANSFER_SYNTAX

/** @copydoc  OCI_ERR_NO_INTERFACE */
#define BOA_ERR_NO_INTERFACE                    OCI_ERR_NO_INTERFACE    

/** @copydoc  OCI_ERR_HW_NOT_PRESENT */
#define BOA_ERR_HW_NOT_PRESENT                  OCI_ERR_HW_NOT_PRESENT

/** @copydoc  OCI_ERR_CANNOT_OPEN_DRIVER */
#define BOA_ERR_CANNOT_OPEN_DRIVER              OCI_ERR_CANNOT_OPEN_DRIVER

/** @copydoc  OCI_ERR_LICENSE_MISSING */
#define BOA_ERR_LICENSE_MISSING                 OCI_ERR_LICENSE_MISSING

/** @} */

/** 
* @} 
*/

#ifdef __cplusplus
}
#endif

#endif