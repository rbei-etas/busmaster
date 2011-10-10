#ifndef ETAS_OCI_DEBUG_H
#define ETAS_OCI_DEBUG_H

/**
* @file       ocidebug.h
* @brief      Debugging support part of the Open Controller Interface (API).
* @copyright  Copyright (c) 2007-2008 ETAS GmbH. All rights reserved.
*
* $Revision: 4636 $
*/

#ifdef __cplusplus
extern "C" {
#endif

/**
* @addtogroup GROUP_OCI_COMMON OCI Common
* @{
*/

/**
* @addtogroup GROUP_OCI_DEBUGGING Debugging Support
* @{
*/

#if (OCI_VERSION > 100) || defined (OCI_INCLUDE_UNSUPPORTED_API)

/** 
* Get the data dictionary describing the controller's statistic
* structure delivered by OCI_GetControllerStatistics(). The entry 
* at index 0 will describe the root structure.
* @param[in]   controller
*                      handle of the controller instance to query the statistics
*                      info from. 
* @param[out]  dataDictionary
*                      pointer to the first of @a size elements in
*                      an array of @ref OCIG_DataDictionaryEntry structs.
*                      The data dictionary will be stored here.
*                      May be NULL.
* @param[in]   ddSize  size of the @a dataDictionary buffer in entries. 
*                      If @a ddCount > @a ddSize, no entries of the 
*                      @a dataDictionary will be filled. 
* @param[out]  ddCount Number of @ref OCIG_DataDictionaryEntry records
*                      required to hold the configuration structure
*                      description. If @a ddCount > @a ddSize, no entries of 
*                      the @a dataDictionary will be filled.
* @return     If the operation was successful, @ref OCI_SUCCESS 
*             will be returned. Otherwise details will be stored for 
*             the handle. If no valid handle exists, details will be 
*             stored for handle @ref OCI_NO_HANDLE.
* @remark     Any client may reset the statistics information at any time.
*             Since this feature is merely intended to support debugging
*             of the client application, no further operational guarantees
*             are given.
*/
#ifndef OCI_DYNAMICDLL
OCI_DECLSPEC OCI_ErrorCode
OCI_CALL OCI_GetControllerStatisticParameter(
    OCI_ControllerHandle controller, 
    OCIG_DataDictionaryEntry* dataDictionary,
    uint32 size,
    uint32* count);
#endif

typedef OCI_ErrorCode
(OCI_CALL *PF_OCI_GetControllerStatisticParameter)( 
    OCI_ControllerHandle controller, 
    OCIG_DataDictionaryEntry* dataDictionary,
    uint32 size,
    uint32* count);

/**
* Return the internal statistics information for a given controller instance.
* Such information is entirely implementation dependant and is intended
* for human interpretation only. Such data may comprise total frame
* counters, error counters etc.
* An OCI may choose to return empty port statistics.
* The interpretation of the data can be aided by a @a dataDictionary
* @param[in]   controller
*                      handle of the controller instance to query the statistics
*                      info from. 
* @param[in]   reset   if set, the internal statics information will be reset 
*                      after storing its current state in @a statistics.
* @param[out]  statistics
*                      Receives the current statistics in form of a struct.
*
* @param[in]   size    size of the @a statistics buffer in bytes. 
*                      If @a count > @a size, no data will be written to this 
*                      buffer. 
* @param[out]  count   Number of bytes required to hold the @a statistics 
*                      structure description. If @a count > @a size, no data
*                      will be written. Must not be NULL.
*                      
* @return     If the operation was successful, @ref OCI_SUCCESS 
*             will be returned. Otherwise details will be stored for 
*             the handle. If no valid handle exists, details will be 
*             stored for handle @ref OCI_NO_HANDLE.
* @remark     Any client may reset the statistics information at any time.
*             Since this feature is merely intended to support debugging
*             of the client application, no further operational guarantees
*             are given.
*/
#ifndef OCI_DYNAMICDLL
OCI_DECLSPEC OCI_ErrorCode
OCI_CALL OCI_GetControllerStatistic(
    OCI_ControllerHandle controller,
    OCI_bool reset,
    void* statistics,
    uint32 size,
    uint32* count);
#endif

typedef OCI_ErrorCode
(OCI_CALL *PF_OCI_GetControllerStatistic)(
    OCI_ControllerHandle controller,
    OCI_bool reset,
    void* statistics,
    uint32 size,
    uint32* count);

#endif

/** @} */

/** @} */


#ifdef __cplusplus
}
#endif

#endif
