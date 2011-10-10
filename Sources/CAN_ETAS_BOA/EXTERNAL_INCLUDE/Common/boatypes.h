#ifndef ETAS_BOATYPES_H
#define ETAS_BOATYPES_H

#include "stdtypes.h"

/**
* @file       boatypes.h
* @brief      Header file for declaration of standard data types.
* @copyright  Copyright (c) 2007-2008 ETAS GmbH. All rights reserved.
*
* $Revision: 4509 $
*/

/**
* @addtogroup GROUP_BOA_GLOBAL_TYPES Global Types
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
* Standard type for all handles used by the BOA.
* Different types of handles use different value ranges.
* I.e.\ all handles a guaranteed to be unique.
* However, the knowledge about the actual ranges may only 
* be used internally by the OCI implementer.
*/
typedef int32 BOA_Handle;

/** 
* Time stamp or difference. All BOA time specs are in driver
* specific clock ticks. Accuracy and resolution depends on the 
* software / hardware implementation at hand. 
* Use OCI_GetTimerCapabilities() and OCI_GetTimerStatus()
* to query clock ID, tick resolution and accuracy.
* @ingroup GROUP_OCI_TIMER_HANDLING
*/
typedef int64 BOA_Time;

/** 
* Standard definition to indicate missing or undefined values.
*/
#define BOA_NO_VALUE (-1)

/** 
* @} 
*/

#endif
