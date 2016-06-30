/*
 * BLF Library
 * (c) 2014, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Author:      Pavel Boiko
 * Release:     1.0
 * Annotation:  Error Manager interface
 * Revision History :
 *
 *     Date             Author               Detail description
 * ------------- --------------------- ------------------------------
 * Aug. 25, 2010 Pavel Boiko           First experimental version
 * May  15, 2014 Pavel Boiko           Adapted for BLF Library
 */

#ifndef ERRORMANAGER_H
#define ERRORMANAGER_H

#include <string>
#include "Strings.h"

//////////////////////////////////////////////////////////////////////////////////////////////////////////
// Warning: If you want to use this Error Manager - please use only macroses, and don't use it's internals.
//////////////////////////////////////////////////////////////////////////////////////////////////////////

// Count of errors that was processed by Error Manager
#define EM_ERRORS (ErrorManager::GetErrorsCount())
// Count of warnings that was processed by Error Manager
#define EM_WARNINGS (ErrorManager::GetWarningsCount())

// Enables all events logging into desired file.
#define EM_ENABLE_LOGGING(logFileName)\
    ErrorManager::EnableLogging(logFileName);

// CRITICAL_ERROR is a macro that should be used to produce "critical error" event.
// In this case CriticalError exception will be generated.
#ifdef _DEBUG
#   define EM_UNEXPECTED_ERROR(message)\
    ErrorManager::ProcessEvent(ekUnexpected, (std::string)message, __FILE__, __LINE__, __FUNCTION__)
#else
#   define EM_UNEXPECTED_ERROR(message)\
    ErrorManager::ProcessEvent(ekUnexpected, (std::string)message)
#endif

// ERROR is a macro that should be used to produce "error" event.
// It produces "error" message, but doesn't generate any exception.
#ifdef _DEBUG
#   define EM_ERROR(message)\
    ErrorManager::ProcessEvent(ekError, (std::string)message, __FILE__, __LINE__, __FUNCTION__)
#else
#   define EM_ERROR(message)\
    ErrorManager::ProcessEvent(ekError, (std::string)message)
#endif

// WARNING is a macro that should be used to produce "warning" event.
// It produces "warning" message, but doesn't generate any exception.
#ifdef _DEBUG
#   define EM_WARNING(message)\
    ErrorManager::ProcessEvent(ekWarning, (std::string)message, __FILE__, __LINE__, __FUNCTION__)
#else
#   define EM_WARNING(message)\
    ErrorManager::ProcessEvent(ekWarning, (std::string)message)
#endif

// INFO is a macro that should be used to produce "information" event.
// It can be used for some process logging for example.
// It produces "info" line to log file, but doesn't generate any message or exception.
#ifdef _DEBUG
#   define EM_INFO(message)\
    ErrorManager::ProcessEvent(ekInfo, (std::string)message, __FILE__, __LINE__, __FUNCTION__)
#else
#   define EM_INFO(message)\
    ErrorManager::ProcessEvent(ekInfo, (std::string)message)
#endif

// Increase tabulator length for future log messages.
#define EM_LOG_DEPTH_INC() ErrorManager::IncLogDepth()

// Decrease tabulator length for future log messages.
#define EM_LOG_DEPTH_DEC() ErrorManager::DecLogDepth()

////////////////////////////////////////////////////////////////////////////////////////////////////////
// Warning: Below is kernel of Error Manager - don't use these objects directly, use only macroses above.
////////////////////////////////////////////////////////////////////////////////////////////////////////

//! Possible kinds of errors.
enum ErrorKind
{
    ekUnexpected,
    ekError,
    ekWarning,
    ekInfo
};

//! Exception class. If some critical error occurs, this class will be returned
//! as 'throw' parameter and can be used to get details about the error.
class Error
{
public:
    //! Constructor for exception class.
    Error(ErrorKind errKind, const std::string& message
#   ifdef _DEBUG
          , const std::string& fileName
          , int lineNo
          , const std::string& funcName
#   endif
         ) : errKind(errKind), message(message)
#   ifdef _DEBUG
        , fileName(fileName), lineNo(lineNo), funcName(funcName)
#   endif
    {}
    //! Returns a kind of the occurred error.
    ErrorKind getKind() const
    {
        return errKind;
    }
    //! Returns message that describes details about the occurred error.
    //! \warning Some kinds of errors shouldn't be shown to end user and contain just debugging info.
    std::string getMessage() const;

private:
    //! Kind of the occurred error.
    ErrorKind errKind;
    //! Message that describes details about the occurred error.
    std::string message;
#   ifdef _DEBUG
    //! Name of source file that produces the error.
    std::string fileName;
    //! Line number in the file 'fileName' that produces the error.
    int lineNo;
    //! Name of function that produces the error.
    std::string funcName;
#   endif //#ifdef _DEBUG
};

//! Operator to output error into stream.
std::ostream& operator<<(std::ostream& o, Error& error);

//! Main class of Error Manager.
class ErrorManager
{
public:
    //! Enables all events logging into desired file.
    static void EnableLogging(const std::string& logFileName);
    //! Increases tabulator length for future log messages.
    static void IncLogDepth();
    //! Decreases tabulator length for future log messages.
    static void DecLogDepth();
    //! Process event (error, warning, etc.)
    static void ProcessEvent(ErrorKind errKind, const std::string& message
#   ifdef _DEBUG
                             , const std::string& file, int line, const std::string& func
#   endif
                            );
    //! Returns count of errors that was processed by Error Manager.
    static int GetErrorsCount()
    {
        return m_ErrorsCount;
    }
    //! Returns count of warnings that was processed by Error Manager.
    static int GetWarningsCount()
    {
        return m_WarningsCount;
    }
private:
    //! Save event information into file.
    static void LogEvent(const Error& error);
private:
    //! Count of errors that was processed by Error Manager.
    static int m_ErrorsCount;
    //! Count of warnings that was processed by Error Manager.
    static int m_WarningsCount;
};

#endif //#ifndef ERRORMANAGER_H
