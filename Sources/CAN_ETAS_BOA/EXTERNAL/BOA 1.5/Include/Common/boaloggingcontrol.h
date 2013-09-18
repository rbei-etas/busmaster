/**
 * @file
 * @brief      Declaration of the BOA logging-control API. This is a public BOA API.
 * @copyright  Copyright ETAS GmbH. All rights reserved.
 *
 * $Revision: 4775 $
 */

#ifndef _BOA_LOGGING_CONTROL_H
#define _BOA_LOGGING_CONTROL_H

#include "stdtypes.h"
#include <stdio.h>
#include <stdarg.h>

/**
 * \page PAGE_BOA_LOGGING BOA - Logging
 * 
 * BOA contains a logger component, which allows other BOA components to write log entries to a log destination. The log destination
 * can be supplied by a BOA client, allowing BOA log entries to be added to the client's log, or BOA can use its own default log
 * destination.
 * 
 * The default BOA log destination is enabled by creating the file "<BOA installation dir>\Log\log_config.txt". This file is expected
 * to have lines of the form "key = value". The following keys are supported:
 *  - @b logFileDir:        the path to the directory in which the default log destination will create the log file. The log file
 *                          itself will be named "boa_<process name>.log".
 *  - @b categoryFilter:    the value of the category filter used by the default log destination (see Log_SetCategoryFilter()).
 *  - @b maxLogFileSize:    the maximum permitted log file size. Once the log file reaches this size, it will be renamed to
 *                          "boa_<process name>.log.archived" (any previously archived file will be deleted) and a new log file will
 *                          be opened.
 *  - @b logFileFlushLevel: the stream to the log file will be flushed after every <logFileFlushLevel> entries are written.
 *                          If this is zero, the stream will never be explicitly flushed.
 * 
 * If a configuration file of this form can be read and parsed successfully, the default log destination will write log entries as
 * specified by the configuration file. If the configuration file is not present or cannot be parsed, the default log destination
 * will not operate.
 *
 * The log file written by the default log destination is XML, with each log entry taking this form:
 * @code
 *     <entry
 *          t="time (in nanoseconds since 1/1/1970)"
 *          pro="process name"
 *          mod="module name (unused)"
 *          stk="call stack ID (unused)"
 *          thd="thread ID"
 *          loc="filename, function name and line number"
 *          cat="category"><![CDATA[log message]]></entry>
 * @endcode
 * (line breaks inserted for readability)
 * 
 * The supported categories are described by the enum @ref Log_Category.
 * 
 * If a BOA client does not want to use the default log destination, the client can configure a custom log destination by supplying
 * a callback function pointer to either Log_SetDestinationEx() or Log_SetDestination(). Once this is done, the BOA logger will send
 * all future log entries to the client's callback, instead of to the default log destination. A BOA client can also use the function
 * Log_SetCategoryFilter() to control which categories of log entry are sent to the callback.
 */

/**
* @def BOALOGGING_API
* Specify the API dynamic-link library (DLL) with automatic import and export.
*
* This is the standard way of creating macros which make exporting from a DLL simpler. All files within dll-logging are compiled with
* the BOALOGGING_EXPORTS symbol defined on the command line. This symbol should not be defined on any project that uses this DLL.
* This way any other project whose source files include this file see BOALOGGING_API functions as being imported from a DLL,
* whereas dll-logging sees symbols defined with this macro as being exported.
*/
#if defined (_MSC_VER)
#ifdef BOALOGGING_EXPORTS
#define BOALOGGING_API __declspec(dllexport)
#else 
#define BOALOGGING_API __declspec(dllimport)
#endif
#elif defined(__GNUC__)
#define BOALOGGING_API __attribute__ ((visibility("default")))
#else
#error Unsupported platform
#endif


#ifdef __cplusplus
extern "C" {
#endif

/**
 * This enum lists the categories of log entry which are supported.
 */
typedef enum Log_Category
{
    LOG_FATAL = 0,  /**< A fatal error has occurred, i.e the error is so severe that the application must terminate. */
    LOG_ERROR = 1,  /**< An error has occurred, i.e the local code cannot continue its intended function. */
    LOG_WARN  = 2,  /**< A warning, i.e the local code has encountered an unusual situation, but can still complete its intended function. */
    LOG_INFO  = 3,  /**< Information helpful for debugging. */
    LOG_TRACE = 4   /**< Trace data, i.e this log entry announces that execution has reached a certain point in the code. */
} Log_Category;

/**
 * A function of this type can be used by the BOA logger to write an entry to a log destination. The BOA logger assumes that
 * such a function is thread-safe.
 *
 * @param[in] szFile        The file which created the log entry.
 * @param[in] szFunc        The function which created the log entry.
 * @param[in] iLine         The line which created the log entry.
 * @param[in] category      The category of the log entry.
 * @param[in] szMessage     Text describing the log entry; this may be NULL.
 * @param[in] argPtr        Arguments for expanding printf-style formatting in szMessage.
 */
typedef void (*PF_Log_WriteToDestination)(
    const char*  szFile,
    const char*  szFunc,
    uint32       iLine,
    Log_Category category,
    const char*  szMessage,
    va_list      argPtr
);

/**
 * A function of this type can be used by the BOA logger to write an entry to a log destination. The BOA logger assumes that
 * such a function is thread-safe.
 *
 * @param[in] szFile        The file which created the log entry.
 * @param[in] szFunc        The function which created the log entry.
 * @param[in] iLine         The line which created the log entry.
 * @param[in] iTimestamp    A timestamp for the log entry, measured in ns since midnight on 1/1/1970.
 * @param[in] iThread       A unique identifier for the thread which created the log entry.
 * @param[in] category      The category of the log entry.
 * @param[in] szMessage     Text describing the log entry; this may be NULL.
 * @param[in] argPtr        Arguments for expanding printf-style formatting in szMessage.
 */
typedef void (*PF_Log_WriteToDestinationEx)(
    const char*  szFile,
    const char*  szFunc,
    uint32       iLine,
    uint64       iTimestamp,
    uint32       iThread,
    Log_Category category,
    const char*  szMessage,
    va_list      argPtr
);

/**
 * This function sets the destination to be used by the BOA logger for future log entries. The BOA logger can only support a
 * single destination at a time, so setting a new destination implicitly removes the previous destination. Note that, due to
 * thread synchronisation, it is possible that the previous destination will continue to receive log entries for a short time
 * after this function is called.
 *
 * If the caller wishes the BOA logger to supply timestamp and thread information for each log entry which is sent to the log
 * destination, the caller should use Log_SetDestinationEx() instead of Log_SetDestination() to set the new log destination.
 *
 * @param[in] logDest   The new log destination. If this is NULL, the BOA internal log destination will be used for future log
 *                      entries.
 */
BOALOGGING_API void Log_SetDestination( PF_Log_WriteToDestination logDest );

/**
 * This function sets the destination to be used by the BOA logger for future log entries. The BOA logger can only support a
 * single destination at a time, so setting a new destination implicitly removes the previous destination. Note that, due to
 * thread synchronisation, it is possible that the previous destination will continue to receive log entries for a short time
 * after this function is called.
 *
 * If the caller does not wish the BOA logger to supply timestamp and thread information for each log entry which is sent to the
 * log destination, the caller should use Log_SetDestination() instead of Log_SetDestinationEx() to set the new log destination.
 *
 * @param[in] logDestEx   The new log destination. If this is NULL, the BOA internal log destination will be used for future log
 *                        entries.
 */
BOALOGGING_API void Log_SetDestinationEx( PF_Log_WriteToDestinationEx logDestEx );

/**
 * This function sets the current category filter for the BOA logger. If bit n of the category filter is set, a log entry with
 * category = n will pass the filter. Note that bit 0 is the least significant bit.
 *
 * Each log entry is sent to the current log destination only if it passes the current category filter. 
 *
 * @param[in] iFilter   The new category filter.
 */
BOALOGGING_API void Log_SetCategoryFilter( uint32 iFilter );

/**
 * This function causes the default log destination to flush any pending log entries to disk immediately. If the default log
 * destination is not in use, this function has no effect.
 *
 * This function is intended to be called in when the process is terminating in an uncontrolled manner.
 */
BOALOGGING_API void Log_Flush( void );

#ifdef __cplusplus
}
#endif

#endif /* _BOA_LOGGING_CONTROL_H */
