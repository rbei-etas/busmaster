/*
 * BLF Library
 * (c) 2014, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Author:      Pavel Boiko
 * Release:     1.0
 * Annotation:  Error Manager
 * Revision History :
 *
 *     Date             Author               Detail description
 * ------------- --------------------- ------------------------------
 * Aug. 25, 2010 Pavel Boiko           First experimental version
 * May  15, 2014 Pavel Boiko           Adapted for BLF Library
 * May  22, 2014 Andrey Oleynikov      Compatible with visual studio 2008 toolset.
 */

#include <stdio.h>
#include <stdarg.h>
#include <sstream>
#include <fstream>
#include <iostream>

#include "Out.h"
#include "ErrorManager.h"

int ErrorManager::m_ErrorsCount = 0;
int ErrorManager::m_WarningsCount = 0;
Out* logStream = NULL;

std::ostream& operator<<(std::ostream& o, Error& error)
{
    return o << error.getMessage() << std::endl;
}

std::string Error::getMessage() const
{
    std::string result;

    // Add producer location (file, line, function) for unexpected errors in debug mode
#   ifdef _DEBUG
    if (ekUnexpected == getKind())
    {
        result += fileName + "(line: " + lineNo + ", function: " + funcName + "): ";
    }
#   endif

    // Add message kind
    switch (getKind())
    {
        case ekUnexpected:
            result += "UNEXPECTED: ";
            break;
        case ekError:
            result += "Error: ";
            break;
        case ekWarning:
            result += "Warning: ";
            break;
        case ekInfo:
            result += "";
            break;
            // The following lines (with 'default' case processing) are commented specially
            // to get warning message if somebody adds the new item to ErrorKind enum,
            // but forget add corresponded lines to the current switch.
            //default:
            //    break;
    }

    // Add message body
    result += message;

    return result;
}

void ErrorManager::LogEvent(const Error& err)
{
    if (NULL != logStream)
    {
        // Set special flag, that next message must be marked as important message if need (if not "info" message)
        if (ekInfo != err.getKind())
        {
            *logStream << out::markedline;
        }
        // Print message
        *logStream << err.getMessage() << std::endl;
    }
}

void ErrorManager::IncLogDepth()
{
    if (logStream)
    {
        *logStream << out::levelup;
    }
}

void ErrorManager::DecLogDepth()
{
    if (logStream)
    {
        *logStream << out::leveldown;
    }
}

void ErrorManager::EnableLogging(const std::string& logFileName)
{
    if (logStream)
    {
        delete logStream;
        logStream = NULL;
    }
    logStream = new Out(logFileName);
    if (!logStream->good())
    {
        EM_ERROR("File can't be opened: " + logFileName);
    }
}

void ErrorManager::ProcessEvent(ErrorKind errKind, const std::string& message
#   ifdef _DEBUG
                                , const std::string& file, int line, const std::string& func
#   endif
                               )
{
    // Generate special error object to simplify processing of event
    Error error(errKind, message
#   ifdef _DEBUG
                , file, line, func
#   endif
               );

    // First of all - log any event
    LogEvent(error);

    // Then do some specific processing of event according to event kind
    switch(errKind)
    {
        case ekInfo:
            // Info: do nothing, because only logging is required for ekInfo
            break;
        case ekUnexpected:
            // Unexpected error: show message + throw exception
            m_ErrorsCount++;
            std::cerr << error;
            /*throw error;*/
            break;
        case ekError:
            // Error: show message
            m_ErrorsCount++;
            std::cerr << error;
            break;
        case ekWarning:
            // Warning: show message
            m_WarningsCount++;
            std::cerr << error;
            break;
            // The following lines (with 'default' case processing) are commented specially
            // to get warning message if somebody adds the new item to ErrorKind enum,
            // but forget add corresponded lines to the current switch.
            //default:
            //    break;
    }
}
