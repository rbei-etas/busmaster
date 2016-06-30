
#ifndef ETAS_BOA_LOGGING_H
#define ETAS_BOA_LOGGING_H

/**
* @file       boalogging.h
* @brief      Definition of BOA logging functionality that dynamically load the correct logging dll for this BOA version
* @author     Paul Kenendy, ETAS Ltd., York
* @copyright  Copyright (c) 2013 ETAS GmbH. All rights reserved.
* @version    $id
* @history    2013-03-25 [Kennedy]: Draft
*/



#include "Common/boaloggingcontrol.h"


#ifdef __cplusplus
extern "C" {
#endif

    /**
    * @addtogroup GROUP_BOASL_LOGGING Logging Support
    * Conversion of strings.
    * @{
    */

    void BOA_LogWriteImpl(
        const char*  szFile,
        const char*  szFunc,
        uint32       iLine,
        Log_Category category,
        const char*  szMessage,
        ...
    );

    void BOA_LogWriteVImpl(
        const char*  szFile,
        const char*  szFunc,
        uint32       iLine,
        Log_Category category,
        const char*  szMessage,
        va_list      argPtr
    );

    uint32 BOA_GetLogFilter();


    /*-----------------------------------------------------------------------------
     * Helper macros
     *-----------------------------------------------------------------------------*/

    /* BOA_HERE is an OS-specific way to encapsulate the location of a line of code. */
#if defined (_MSC_VER)
#define BOA_HERE __FILE__, __FUNCTION__, __LINE__
#elif defined (__GNUC__)
#define BOA_HERE __FILE__, __func__,     __LINE__
#else
#  error unknown compiler/environment
#endif

    /* The following macros fulfil two functions:
     *  - They improve the performance of the logging API in cases when some log categories are disabled by performing
     *    category filtering as early as possible.
     *  - They allow all usages of the logging API to be disabled at build time by defining BOA_DISABLE_LOG.
     *  - They are implemented within lib-boa.lib
     */
#ifndef BOA_DISABLE_LOG

#define Log_Write( category, ... )                                  \
    {                                                               \
        if( BOA_GetLogFilter() & ( 1 << category ) )                \
        {                                                           \
            BOA_LogWriteImpl ( BOA_HERE, category, __VA_ARGS__ );   \
        }                                                           \
    }

#define Log_WriteV( category, szMessage, argPtr )                       \
    {                                                                   \
        if( BOA_GetLogFilter() & ( 1 << category ) )                    \
        {                                                               \
            BOA_LogWriteVImpl( BOA_HERE, category, szMessage, argPtr ); \
        }                                                               \
    }

#else

#define Log_Write( category, ... )                /* Do nothing */
#define Log_WriteV( category, szMessage, argPtr ) /* Do nothing */

#endif

    /*-----------------------------------------------------------------------------
     * Macro-based API: the preferred way to access the logging API.
     *-----------------------------------------------------------------------------*/

#define Log_Fatal( ... ) Log_Write( LOG_FATAL, __VA_ARGS__ )
#define Log_Error( ... ) Log_Write( LOG_ERROR, __VA_ARGS__ )
#define Log_Warn( ... )  Log_Write( LOG_WARN,  __VA_ARGS__ )
#define Log_Info( ... )  Log_Write( LOG_INFO,  __VA_ARGS__ )
#define Log_Trace()      Log_Write( LOG_TRACE, NULL )

#ifdef __cplusplus
}
#endif

#endif
