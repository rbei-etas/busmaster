#ifndef ETAS_OCI_TYPES_H
#define ETAS_OCI_TYPES_H

/**
* @file       ocitypes.h
* @brief      Global types and constants of the Open Controller Interface (API).
* @copyright  Copyright (c) 2007-2008 ETAS GmbH. All rights reserved.
*
* $Revision: 4509 $
*/

#include "..\Common\boatypes.h"
#include "ocidefs.h"

#ifdef __cplusplus
extern "C" {
#endif


/**
* @addtogroup GROUP_OCI_COMMON OCI Common
* @{
*/

/**
* @addtogroup GROUP_OCI_GLOBAL_TYPES Global Types
* @{
*/

/** 
* Standard definition to indicate missing or undefined values.
*/
#define OCI_NO_VALUE BOA_NO_VALUE

/** 
* Special @ref OCI_Handle value to indicate a missing or
* invalid handle.
*/
#define OCI_NO_HANDLE (OCI_Handle)OCI_NO_VALUE

/** 
* Special @ref OCI_Time value used to indicate an invalid global time 
* stamp. However, -1 is valid for differences between time stamps.
* @ingroup GROUP_OCI_TIMER_HANDLING
*/
#define OCI_NO_TIME (OCI_Time)(OCI_NO_VALUE)

/** 
* Standard type for all handles used by the OCI.
* Different types of handles use different value ranges.
* I.e.\ all handles a guaranteed to be unique.
* However, the knowledge about the actual ranges may only 
* be used internally by the OCI implementer.
*/
typedef BOA_Handle OCI_Handle;

/** 
* All functions return an @ref OCI_ErrorCode value to indicate either 
* success @ref OCI_SUCCESS or failure of a function call. Use 
* @ref OCI_GetError() to get further information in case of an error.
* @coding  Use negative values for error codes to support the usage of 
*          FAILED() and SUCCEEDED() macros under Windows. 
*          The practice of the flags @ref OCI_ERR_FLAG_ERROR,
*          and @ref OCI_ERR_FLAG_WARNING will enable this feature. 
* @n       Moreover there are OCI specific macros @ref OCI_FAILED() 
*          and @ref OCI_SUCCEEDED() with the same functionality such 
*          as FAILED() and SUCCEEDED(), which should be preferred.
* @ingroup GROUP_OCI_ERROR_MANAGEMENT
*/
typedef BOA_ResultCode OCI_ErrorCode;

/** 
* A one-character boolean type. Actual type (char, bool etc.) 
* depends on target. "0" is @c false, "1" is @c true. Do not use 
* other values (like e.g.\ "-1").
*/
typedef char OCI_bool;

/** 
* Time stamp or difference. All OCI time specs are in driver
* specific clock ticks. Accuracy and resolution depends on the 
* software / hardware implementation at hand. 
* Use OCI_GetTimerCapabilities() and OCI_GetTimerStatus()
* to query clock ID, tick resolution and accuracy.
* @ingroup GROUP_OCI_TIMER_HANDLING
*/
typedef BOA_Time OCI_Time;

/** 
* Handle for a single hardware port.
* @ingroup GROUP_OCI_CONTROLLER
*/
typedef OCI_Handle OCI_ControllerHandle;

/** 
* Handle to some (Tx or Rx) queue.
* @ingroup GROUP_OCI_QUEUE
*/
typedef OCI_Handle OCI_QueueHandle;


/** @} */

/** @} */

#ifdef __cplusplus
}
#endif

#endif
