
/******************************************************************************
  Project       :  Auto-SAT_Tools
  FileName      :  Logger_CommonDataTypes.h
  Description   :  Definition of CParamLoggerBUS class.
  $Log:   X:/Archive/Sources/FrameProcessor/Logger_CommonDataTypes.h_v  $
 *
 *    Rev 1.0   02 Dec 2011 20:20:56   rac2kor
 *

/**
 * \file      Logger_CommonDataTypes.h
 * \brief     Definition of CParamLoggerBUS class.
 * \author    Ratnadip Choudhury
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Definition of CParamLoggerBUS class.
 */

#pragma once

class CParamLoggerBUS
{
public:
    char m_acVersion[MAX_PATH];        // Version info of the application suite
    Base_WrapperErrorLogger* m_pILog;   // Error logger module
    DWORD dwClientID;                   // Client identifier

    CParamLoggerBUS()                   // Default constructor
    {
        m_pILog = NULL;
        dwClientID = 0;
        for (int i = 0; i < MAX_PATH; i++)
        {
            m_acVersion[i] = L'\0';
        }
    };

    ~CParamLoggerBUS()                  // Destructor
    {
    };

    CParamLoggerBUS& operator=(const CParamLoggerBUS& RefObj)
    {
        m_pILog     = RefObj.m_pILog;
        dwClientID  = RefObj.dwClientID;
        strcpy_s(m_acVersion, MAX_PATH, RefObj.m_acVersion);
        return *this;
    }
};
