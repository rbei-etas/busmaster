
/******************************************************************************
  Project       :  Auto-SAT_Tools
  FileName      :  Logger_CommonDataTypes.h
  Description   :  Definition of CParamLoggerBUS class.
  $Log:   X:/Archive/Sources/FrameProcessor/Logger_CommonDataTypes.h_v  $
 * 
 *    Rev 1.0   02 Dec 2011 20:20:56   rac2kor
 *  

  Author(s)     :  Ratnadip Choudhury
  Date Created  :  2.12.2011
  Modified By   :  
  Copyright (c) 2011, Robert Bosch Engineering and Business Solutions.  All rights reserved.
******************************************************************************/

#if !defined LOGGER_COMMONDATATYPES_H__INCLUDED_
#define LOGGER_COMMONDATATYPES_H__INCLUDED_


class CParamLoggerBUS
{
public:
    TCHAR m_acVersion[MAX_PATH];        // Version info of the application suite
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
        _tcscpy(m_acVersion, RefObj.m_acVersion);

        return *this;
    }
};

#endif // LOGGER_COMMONDATATYPES_H__INCLUDED_