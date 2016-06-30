/*
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/**
 * \file      BaseLogObject.cpp
 * \brief     Source file for CBaseLogObject class
 * \author    Ratnadip Choudhury
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Source file for CBaseLogObject class
 */


#include "FrameProcessor_stdafx.h"
#include "BaseLogObject.h"            // For CBaseLogObject class declaration
#include "Utility\UtilFunctions.h"
#include "Utility\Utility.h"



const int SIZE_CHAR = sizeof(char);
#define ENOENT          2




#define  DEFAULT_FILE_SIZE_IN_BYTES  4294967296 //4Gb

#define MAX_LOG_FILE_IN_GRP              65535
#define FILE_COUNT_STR                    "_%d"


#define BUS_LOG_HEADER          "***NOTE: PLEASE DO NOT EDIT THIS DOCUMENT***"
#define BUS_LOG_START           "***[START LOGGING SESSION]***"
#define BUS_LOG_STOP            "***[STOP LOGGING SESSION]***"
#define BUS_LOG_START_DATE_TIME \
    "***START DATE AND TIME %d:%d:%d %d:%d:%d:%d***"
#define BUS_LOG_END_DATE_TIME   "***END DATE AND TIME %d:%d:%d %d:%d:%d:%d***"
#define BUS_LOG_HEXFORMAT       "***HEX***"
#define BUS_LOG_DECFORMAT       "***DEC***"
#define BUS_LOG_ABSMODE         "***ABSOLUTE MODE***"
#define BUS_LOG_RELMODE         "***RELATIVE MODE***"
#define BUS_LOG_SYSMODE         "***SYSTEM MODE***"
#define BUS_LOG_BAUDRATE        "***CHANNEL %d BAUD RATE %d kbps***"
#define BUS_LOG_DATABASE_START  "***START DATABASE FILES***"
#define BUS_LOG_DATABASE_END    "***END DATABASE FILES***"
#define BUS_LOG_BAUDRATE_START  "***START CHANNEL BAUD RATE***"
#define BUS_LOG_BAUDRATR_END    "***END CHANNEL BAUD RATE***"
#define BUS_LOG_CHANNEL         "***CHANNEL %d - %s - %s bps***"
#define BUS_LOG_CHANNEL_LIN         "***CHANNEL %d - Protocol Version %s - Hardware %s - BaudRate %d bps***"
#define BUS_LOG_COMMENT_START   "***START COMMENT***"   //arun
#define BUS_LOG_COMMENT_END     "***END COMMENT***"

#define PROTOCOL "***PROTOCOL";
#define BUS_LOG_PADDING "***"



CBaseLogObject::CBaseLogObject(CString omVersion):m_omVersion(omVersion)
{
    vResetValues();
}

/**
 * Resets values of certain data members
 */
void CBaseLogObject::vResetValues(void)
{
    m_sLogInfo.vClear();    // Initialise the logging block
    m_pLogFile = nullptr;
    m_CurrTriggerType = NONE;
    m_nCurrFileCnt = 0;
    m_dTotalBytes = 0;
    m_bNewSession = TRUE;
    m_nMeasurement=0;
    m_nSize=0;
    m_nTime=0;
    InitializeCriticalSection(&m_CritSection);
}

/**
 * Destructor
 */
CBaseLogObject::~CBaseLogObject()
{
    DeleteCriticalSection(&m_CritSection);
}

/**
 * Equal operator
 */
CBaseLogObject& CBaseLogObject::operator=(const CBaseLogObject& RefObj)
{
    RefObj.GetLogInfo(m_sLogInfo);
    m_nCurrFileCnt = RefObj.m_nCurrFileCnt;
    m_dTotalBytes = RefObj.m_dTotalBytes;
    Der_CopySpecificData(&RefObj);

    return *this;
}

/**
 * Enable / disable logging
 */
void CBaseLogObject::EnableLogging(bool bEnable)
{
    m_sLogInfo.m_bEnabled = bEnable;


}

/**
 * Query - if logging is enable
 */
bool CBaseLogObject::IsLoggingEnable(void)
{
    return m_sLogInfo.m_bEnabled;
}

/**
 * Get the log info structure
 */
void CBaseLogObject::GetLogInfo(SLOGINFO& sLoginfo) const
{
    sLoginfo = m_sLogInfo;

}

/**
 * Set Log info structure
 */
void CBaseLogObject::SetLogInfo(const SLOGINFO& sLoginfo)
{
    m_sLogInfo = sLoginfo;

    //Now if the name of the file is changed then only search for other file
    //in this file cycle else curr file name and size will be same

}

/**
 * Set configuration data
 */
BYTE* CBaseLogObject::SetConfigData(BYTE* pvDataStream, BYTE bytLogVersion)
{
    BYTE* pbSStream = pvDataStream;
    //pbSStream = m_sLogInfo.pbSetConfigData(pbSStream, bytLogVersion);
    //pbSStream = Der_SetConfigData(pbSStream);

    // The default value of current log file should be the log file name


    return pbSStream;
}

INT CBaseLogObject::nSetConfigData(xmlNodePtr pNode)
{
    INT nRetValue = m_sLogInfo.nSetConfigData(pNode->children);
    if(nRetValue == S_OK)
    {
        nRetValue = Der_SetConfigData(pNode->children);

        // The default value of current log file should be the log file name
    }
    return nRetValue;
}
/**
 * Get configuration data
 */
BYTE* CBaseLogObject::GetConfigData(BYTE* pvDataStream) const
{
    BYTE* pbTStream = pvDataStream;
    //pbTStream = m_sLogInfo.pbGetConfigData(pbTStream);
    //pbTStream = Der_GetConfigData(pbTStream);
    return pbTStream;
}

bool CBaseLogObject::GetConfigData(xmlNodePtr pxmlNodePtr) const
{
    m_sLogInfo.pbGetConfigData(pxmlNodePtr);
    Der_GetConfigData(pxmlNodePtr);

    return TRUE;
}

/**
 * To get the total buffer size
 */
UINT CBaseLogObject::unGetBufSize(void) const
{
    return 0;
}

/**
 * To get the ID
 */
UINT CBaseLogObject::GetID(void)
{
    return m_sLogInfo.m_ushID;
}

/**
 * To log a string
 */
bool CBaseLogObject::bLogString(CString& omStr)
{
    if (!m_sLogInfo.m_bEnabled)
    {
        return false;
    }

    if (nullptr == m_pLogFile)
    {
        ASSERT(false);
        return false;
    }

    EnterCriticalSection(&m_CritSection);
    _ftprintf(m_pLogFile, omStr.GetBuffer(MAX_PATH));
    LeaveCriticalSection(&m_CritSection);

    return true;
}

void CBaseLogObject::vWriteTextToFile(CString& om_LogText, ETYPE_BUS eBus)
{
    DWORD dwBytes2Write = om_LogText.GetLength()* SIZE_CHAR; //no of bytes


    // If trigger Size is specified
    if(m_sLogInfo.m_sLogAdvStngs.m_bIsLogOnSize == TRUE)
    {
        ULONG64 unSpecifiedSize = atol(m_sLogInfo.m_sLogAdvStngs.m_omSizeInMB);

        unSpecifiedSize = unSpecifiedSize * 1024 *1024;
        if((m_dTotalBytes + dwBytes2Write) >= unSpecifiedSize)
        {
            //triggering type is saved to be used for next file
            ELOGTRIGGERSTATE LastTriggerType = m_CurrTriggerType;
            bStopLogging();

            //Set the next file name of the series
            vSetNextFileName(SUFFIX_SIZE);

            //If file is append mode then change it to overwrite mode bfore startlogging
            //So that old data will be deleted
            eMode eFileMode = m_sLogInfo.m_eFileMode;
            //The file mode is changed to overwrite so that when the same file
            //is opened in cycle by bStartLogging(), it should overwrite
            if (m_sLogInfo.m_eFileMode == APPEND_MODE)
            {
                m_sLogInfo.m_eFileMode = OVERWRITE_MODE;
            }
            bStartLogging(eBus);
            //Save the triggering type
            m_CurrTriggerType = LastTriggerType;
            //Revert back to the original mode
            m_sLogInfo.m_eFileMode = eFileMode;
            //theApp.GetDefaultLogFile();
            m_dTotalBytes = 0;
        }
    }
    // If trigger time is specified
    if(m_sLogInfo.m_sLogAdvStngs.m_bIsLogOnTime == TRUE)
    {
        LARGE_INTEGER stLogTime, sFrequency;
        QueryPerformanceFrequency(&sFrequency);
        QueryPerformanceCounter(&stLogTime);

        double lfCurrentTime;

        lfCurrentTime = (double)(stLogTime.QuadPart)/(double)(sFrequency.QuadPart);

        INT lfDiffTime = lfCurrentTime - m_sLogInfo.m_sLogAdvStngs.m_qwLogSysTime;


        INT nHrs = atoi(m_sLogInfo.m_sLogAdvStngs.m_omLogTimeHrs);
        INT nMins = atoi(m_sLogInfo.m_sLogAdvStngs.m_omLogTimeMins);

        // Converting in to seconds
        nHrs = ((nHrs * 60) + nMins) * 60;

        if(lfDiffTime >= nHrs)
        {
            //triggering type is saved to be used for next file
            ELOGTRIGGERSTATE LastTriggerType = m_CurrTriggerType;
            bStopLogging();

            //Set the next file name of the series
            vSetNextFileName(SUFFIX_TIME);


            //If file is append mode then change it to overwrite mode bfore startlogging
            //So that old data will be deleted
            eMode eFileMode = m_sLogInfo.m_eFileMode;
            //The file mode is changed to overwrite so that when the same file
            //is opened in cycle by bStartLogging(), it should overwrite
            if (m_sLogInfo.m_eFileMode == APPEND_MODE)
            {
                m_sLogInfo.m_eFileMode = OVERWRITE_MODE;
            }
            bStartLogging(eBus);
            //Save the triggering type
            m_CurrTriggerType = LastTriggerType;
            //Revert back to the original mode
            m_sLogInfo.m_eFileMode = eFileMode;
            //theApp.GetDefaultLogFile();
            m_dTotalBytes = 0;
        }



    }
    if(m_sLogInfo.m_sLogAdvStngs.m_bIsLogOnMesurement == FALSE && m_sLogInfo.m_sLogAdvStngs.m_bIsLogOnTime == FALSE && m_sLogInfo.m_sLogAdvStngs.m_bIsLogOnSize == FALSE)
    {
        if ((m_dTotalBytes + dwBytes2Write) >= DEFAULT_FILE_SIZE_IN_BYTES)
        {
            //triggering type is saved to be used for next file
            ELOGTRIGGERSTATE LastTriggerType = m_CurrTriggerType;
            bStopLogging();
            //Set the next file name of the series
            vSetNextFileName(SUFFIX_DEFAULT);

            //If file is append mode then change it to overwrite mode bfore startlogging
            //So that old data will be deleted
            eMode eFileMode = m_sLogInfo.m_eFileMode;
            //The file mode is changed to overwrite so that when the same file
            //is opened in cycle by bStartLogging(), it should overwrite
            if (m_sLogInfo.m_eFileMode == APPEND_MODE)
            {
                m_sLogInfo.m_eFileMode = OVERWRITE_MODE;
            }
            bStartLogging(eBus);
            //Save the triggering type
            m_CurrTriggerType = LastTriggerType;
            //Revert back to the original mode
            m_sLogInfo.m_eFileMode = eFileMode;
            //theApp.GetDefaultLogFile();
            m_dTotalBytes = 0;
        }
    }
    EnterCriticalSection(&m_CritSection);
    if ( m_pLogFile )
    {
        _ftprintf(m_pLogFile, om_LogText.GetBuffer(MAX_PATH));
    }
    LeaveCriticalSection(&m_CritSection);

    //Get the file size
    m_dTotalBytes += dwBytes2Write;
}
void CBaseLogObject::vSetMeasurementFileName()
{
    if(m_sLogInfo.m_sLogAdvStngs.m_bIsLogOnMesurement == TRUE)
    {
        //triggering type is saved to be used for next file
        ELOGTRIGGERSTATE LastTriggerType = m_CurrTriggerType;

        //Set the next file name of the series if
        vSetNextFileName(SUFFIX_MEASUREMENT);

        if (m_sLogInfo.m_eFileMode == APPEND_MODE)
        {
            m_sLogInfo.m_eFileMode = OVERWRITE_MODE;
        }

        //Save the triggering type
        m_CurrTriggerType = LastTriggerType;
        //Revert back to the original mode

    }
}
/*******************************************************************************
  Function Name  : vSetNextFileName
  Description    : Adding suffix to advanced log file
  Member of      : CBaseLogObject
  Functionality  : -
  Author(s)      : Robin George Koshy
  Date Created   : 03.09.2014
  Modifications  :
*******************************************************************************/
void CBaseLogObject::vSetNextFileName(eFILENAMESUFFIX eFileNameSuffix)
{

    std::string strLogFile = std::string(m_sLogInfo.m_sLogFileName);
    std::string strSuffixSubString = "";
    int n_pos;

    switch(eFileNameSuffix)
    {
        case SUFFIX_MEASUREMENT:
        {

            if(!CUtilFunctions::bFindLastSuffix(strLogFile,S_MEASUREMENT,n_pos))
            {
                CUtilFunctions::bFindLastSuffix(strLogFile,".log",n_pos);
                strSuffixSubString = S_MEASUREMENT+std::to_string(0)+".log";
            }

            else
            {
                strSuffixSubString = strLogFile.substr(n_pos, sizeof(strLogFile));
                sscanf_s(strSuffixSubString.c_str(),M_SUFFIX_FORMAT,&m_nMeasurement);
                std::string strCurr_M = S_MEASUREMENT+std::to_string(m_nMeasurement);
                std::string strNext_M = "";
                if (m_sLogInfo.m_sLogAdvStngs.m_nConnectionCount != 0)
                {
                    int nTemp = (m_nMeasurement + 1) % atoi(m_sLogInfo.m_sLogAdvStngs.m_omMaxNoOfLogFiles);
                    strNext_M = S_MEASUREMENT + std::to_string(nTemp);
                    if (nTemp == 0)
                    {
                        m_sLogInfo.m_sLogAdvStngs.m_nConnectionCount = 1;
                    }
                }
                else
                {
                    strNext_M = strCurr_M;
                }

                strSuffixSubString.replace(strSuffixSubString.find(strCurr_M),strlen(strCurr_M.c_str()),strNext_M);
            }
        }
        break;
        case SUFFIX_SIZE:
        {

            if(!CUtilFunctions::bFindLastSuffix(strLogFile,S_SIZE,n_pos))
            {
                CUtilFunctions::bFindLastSuffix(strLogFile,".log",n_pos);
                strSuffixSubString = S_SIZE+std::to_string(0)+".log";

            }
            else
            {
                strSuffixSubString = strLogFile.substr(n_pos,sizeof(strLogFile));
                sscanf_s(strSuffixSubString.c_str(),S_SUFFIX_FORMAT,&m_nSize);
                std::string strCurr_S = S_SIZE+std::to_string(m_nSize);
                std::string strNext_S = S_SIZE+std::to_string(m_nSize+1);
                if(m_nSize >= (atoi(m_sLogInfo.m_sLogAdvStngs.m_omMaxNoOfLogFiles)-1))
                {
                    strNext_S = S_SIZE+std::to_string(0);
                }
                strSuffixSubString.replace(strSuffixSubString.find(strCurr_S),strlen(strCurr_S.c_str()),strNext_S);
            }
        }
        break;
        case SUFFIX_TIME:
        {

            if(!CUtilFunctions::bFindLastSuffix(strLogFile,S_TIME,n_pos))
            {
                CUtilFunctions::bFindLastSuffix(strLogFile,".log",n_pos);
                strSuffixSubString = S_TIME+std::to_string(0)+".log";

            }

            else
            {
                strSuffixSubString = strLogFile.substr(n_pos,sizeof(strLogFile));
                sscanf_s(strSuffixSubString.c_str(),T_SUFFIX_FORMAT,&m_nTime);
                std::string strCurr_T = S_TIME+std::to_string(m_nTime);
                std::string strNext_T = S_TIME+std::to_string(m_nTime+1);
                if(m_nTime >= (atoi(m_sLogInfo.m_sLogAdvStngs.m_omMaxNoOfLogFiles)-1))
                {
                    strNext_T = S_TIME+std::to_string(0);
                }
                strSuffixSubString.replace(strSuffixSubString.find(strCurr_T),strlen(strCurr_T.c_str()),strNext_T);
            }
        }
        break;
        case SUFFIX_DEFAULT:
        {

            if(!CUtilFunctions::bFindLastSuffix(strLogFile,S_DEFAULT,n_pos))
            {
                CUtilFunctions::bFindLastSuffix(strLogFile,".log",n_pos);
                strSuffixSubString = S_DEFAULT+std::to_string(1)+".log";

            }
            else
            {
                strSuffixSubString = strLogFile.substr(n_pos,sizeof(strLogFile));
                if(sscanf_s(strSuffixSubString.c_str(),"_%d.log",&m_nDefault) == 0)
                {
                    CUtilFunctions::bFindLastSuffix(strLogFile,".log",n_pos);
                    strSuffixSubString = S_DEFAULT+std::to_string(1)+".log";
                }
                else
                {
                    std::string strCurr_D = S_DEFAULT+std::to_string(m_nDefault);
                    std::string strNext_D = S_DEFAULT+std::to_string(m_nDefault+1);
                    if(m_nDefault >= MAX_LOG_FILE_IN_GRP-1)
                    {
                        strNext_D = "";
                    }
                    strSuffixSubString.replace(strSuffixSubString.find(strCurr_D),strlen(strCurr_D.c_str()),strNext_D);
                }
            }

        }
        break;

    }
    std::string strBaseSubString = strLogFile.substr(0,n_pos);
    strBaseSubString.append(strSuffixSubString);
    strcpy(m_sLogInfo.m_sLogFileName,strBaseSubString.c_str());

}



void CBaseLogObject::vCloseLogFile()
{
    if(m_pLogFile != nullptr)
    {
        fclose(m_pLogFile);
    }
    m_pLogFile = nullptr;
}
/**
 * \brief Start logging
 * \req RS_12_23 Start logging
 *
 * To do actions before logging starts.
 */
BOOL CBaseLogObject::bStartLogging(ETYPE_BUS eBus)
{
    BOOL bResult = FALSE;

    if ((m_pLogFile == nullptr) && (m_sLogInfo.m_bEnabled))
    {
        UINT64 qwRefSysTime, qwAbsBaseTime;
        //m_ouRefTimeKeeper.vGetTimeParams(qwRefSysTime, qwAbsBaseTime);

        LARGE_INTEGER stLogTime, sFrequency;
        QueryPerformanceFrequency(&sFrequency);
        QueryPerformanceCounter(&stLogTime);

        m_sLogInfo.m_sLogAdvStngs.m_qwLogSysTime = ((double)(stLogTime.QuadPart))/((double)(sFrequency.QuadPart));
        // This function should be called every time logging is started
        m_CurrTriggerType = m_sLogInfo.m_sLogTrigger.m_unTriggerType;
        char Mode[2] =  " ";
        Mode[0] = (m_sLogInfo.m_eFileMode == APPEND_MODE) ? L'a' : L'w';
        EnterCriticalSection(&m_CritSection);
        //In case user has deleted the content of the file

        m_dTotalBytes = dwGetFileSize(m_sLogInfo.m_sLogFileName);

        //If it is new session always overwrite the file
        if (m_dTotalBytes >= DEFAULT_FILE_SIZE_IN_BYTES && m_bNewSession)
        {
            Mode[0] = L'w';
            m_dTotalBytes = 0;
        }

        if (m_sLogInfo.m_eFileMode == OVERWRITE_MODE)
        {
            m_dTotalBytes = 0;
        }

        fopen_s(&m_pLogFile, m_sLogInfo.m_sLogFileName, Mode);

        if (m_pLogFile != nullptr)
        {
            CString omHeader = "";
            vFormatHeader(omHeader, eBus);
            _ftprintf(m_pLogFile,  "%s", omHeader.GetBuffer(MAX_PATH));
            bResult = TRUE;
        }
        LeaveCriticalSection(&m_CritSection);
    }
    return bResult;
}

/**
 * \brief Stop logging
 * \req RS_12_24 Stop logging
 *
 * To do actions before logging stop.
 */
bool CBaseLogObject::bStopLogging()
{
    bool bResult = false;

    if ((m_pLogFile != nullptr) && (m_sLogInfo.m_bEnabled))
    {
        m_CurrTriggerType = NONE;
        CString omFooter = "";
        vFormatFooter(omFooter);
        _ftprintf(m_pLogFile,  "%s\n", omFooter.GetBuffer(MAX_PATH));
        fclose(m_pLogFile);
        m_pLogFile = nullptr;
        bResult = true;
        m_bNewSession = false;  // Old session closed
    }

    return bResult;
}

bool CBaseLogObject::bStopOnlyLogging()
{
    bool bResult = false;

    if ((m_pLogFile != nullptr) && (m_sLogInfo.m_bEnabled))
    {
        //m_CurrTriggerType = NONE;
        CString omFooter = "";
        vFormatFooter(omFooter);
        _ftprintf(m_pLogFile,  "%s\n", omFooter.GetBuffer(MAX_PATH));
        fclose(m_pLogFile);
        m_pLogFile = nullptr;
        //bResult = TRUE;
        //m_bNewSession = FALSE;  // Old session closed
    }

    return bResult;
}

#define MIN_NAME_LENGTH    5

/**
 * Get size of the file
 */
DWORD CBaseLogObject::dwGetFileSize(CString omFileName)
{
    DWORD dwFileSize = 0;
    CFile omfile;
    CFileException omFileException;
    if (omfile.Open(omFileName, CFile::modeRead | CFile::shareExclusive, &omFileException))
    {
        dwFileSize = (DWORD) (omfile.GetLength());
        omfile.Close();
    }
    return dwFileSize;
}

void CBaseLogObject::vFormatHeader(CString& omHeader, ETYPE_BUS eBus)
{
    omHeader = "***BUSMASTER ";

    omHeader += m_omVersion;
    omHeader += BUS_LOG_PADDING;
    omHeader += L'\n';
    omHeader += PROTOCOL;
    omHeader += CString(" ") + getBusInString(eBus).c_str() + BUS_LOG_PADDING;
    omHeader += L'\n';


    omHeader += BUS_LOG_HEADER;
    omHeader += L'\n';
    omHeader += BUS_LOG_START;
    omHeader += L'\n';


    if(!m_sLogInfo.m_sLogAdvStngs.m_omLogComment.IsEmpty())
    {
        omHeader += BUS_LOG_COMMENT_START;
        omHeader += L'\n';
        omHeader += m_sLogInfo.m_sLogAdvStngs.m_omLogComment;
        omHeader += L'\n';
        omHeader += BUS_LOG_COMMENT_END;
        omHeader += L'\n';
    }

    // Log current date and time as the start date and time of logging process
    SYSTEMTIME CurrSysTime;
    GetLocalTime(&CurrSysTime);
    CString omBuf;
    omBuf.Format(BUS_LOG_START_DATE_TIME, CurrSysTime.wDay, CurrSysTime.wMonth,
                 CurrSysTime.wYear, CurrSysTime.wHour, CurrSysTime.wMinute,
                 CurrSysTime.wSecond, CurrSysTime.wMilliseconds);
    omHeader += omBuf;
    omHeader += L'\n';

    omHeader += (m_sLogInfo.m_eNumFormat == HEXADECIMAL) ? BUS_LOG_HEXFORMAT : BUS_LOG_DECFORMAT;
    omHeader += L'\n';
    switch (m_sLogInfo.m_eLogTimerMode) // Time Mode
    {
        case TIME_MODE_ABSOLUTE:
            omHeader += BUS_LOG_ABSMODE;
            break;

        case TIME_MODE_RELATIVE:
            omHeader += BUS_LOG_RELMODE;
            break;

        case TIME_MODE_SYSTEM:
            omHeader += BUS_LOG_SYSMODE;
            break;

        default:
            ASSERT(false);
            break;
    }
    omHeader += L'\n';
    omHeader += BUS_LOG_BAUDRATE_START;
    omHeader += L'\n';

    omHeader += getNetworkFormatData(eBus);

    omHeader += BUS_LOG_BAUDRATR_END;
    omHeader += L'\n';

    //Update Baud Rate and Associated DBC and DBF files
    CStringArray aomList;
    GetDatabaseFiles(aomList);

    omHeader += BUS_LOG_DATABASE_START;
    omHeader += L'\n';
    for (int nIdx = 0; nIdx < aomList.GetSize(); nIdx++)
    {
        omHeader += "***" + aomList.GetAt(nIdx) + "***";
        omHeader += L'\n';

    }
    omHeader += BUS_LOG_DATABASE_END;
    omHeader += L'\n';
}

CString CBaseLogObject::getNetworkFormatData(ETYPE_BUS eBus)
{
    CString omHeader;
    CString strChannelNum = "";
    if (eBus == CAN || eBus == J1939)
    {
        // Update the channel and its baudrate information
        SCONTROLLER_DETAILS controllerDetails[defNO_OF_CHANNELS];
        int nNumChannels = 0;
        GetChannelBaudRateDetails(controllerDetails, nNumChannels);
        CString strChannelNum = "";
        for (int nChannelNum = 1; nChannelNum <= nNumChannels; nChannelNum++)
        {
            /* if baud rate is in kbps, convert to bps */
            if (controllerDetails[nChannelNum - 1].m_omStrBaudrate.size() < 4)
            {
                controllerDetails[nChannelNum - 1].m_omStrBaudrate += "000";
            }

            strChannelNum.Format(BUS_LOG_CHANNEL, nChannelNum,
                                 controllerDetails[nChannelNum - 1].m_omHardwareDesc.c_str(),
                                 controllerDetails[nChannelNum - 1].m_omStrBaudrate.c_str());
            omHeader += strChannelNum;
            omHeader += L'\n';
        }

    }
    else if (eBus == LIN)
    {

        SCONTROLLER_DETAILS_LIN controllerDetails[defNO_OF_LIN_CHANNELS];
        int nNumChannels = 0;
        GetChannelBaudRateDetails(controllerDetails, nNumChannels);
        CString strChannelNum = "";

        for (int nChannelNum = 1; nChannelNum <= nNumChannels; nChannelNum++)
        {
            // if baud rate is in kbps, convert to bps

            /*controllerDetails[nChannelNum].m_strProtocolVersion,
            controllerDetails[nChannelNum].m_strHwUri,controllerDetails[nChannelNum].m_nBaudRate*/
            strChannelNum.Format(BUS_LOG_CHANNEL_LIN, nChannelNum, controllerDetails[nChannelNum - 1].m_strProtocolVersion.c_str()
                                 , controllerDetails[nChannelNum - 1].m_strHwUri.c_str(), controllerDetails[nChannelNum - 1].m_BaudRate);
            omHeader += strChannelNum;
            omHeader += L'\n';
        }

    }
    return omHeader;
}

void CBaseLogObject::vFormatFooter(CString& omFooter)
{
    // Log current date and time as the stop date and time of logging process
    SYSTEMTIME CurrSysTime;
    GetLocalTime(&CurrSysTime);
    CString omBuf;
    omBuf.Format(BUS_LOG_END_DATE_TIME, CurrSysTime.wDay, CurrSysTime.wMonth,
                 CurrSysTime.wYear, CurrSysTime.wHour, CurrSysTime.wMinute,
                 CurrSysTime.wSecond, CurrSysTime.wMilliseconds);
    omFooter += omBuf;
    omFooter += L'\n';

    omFooter += BUS_LOG_STOP;
    omFooter += L'\n';
}


void CBaseLogObject::GetDatabaseFiles(CStringArray& omList)
{
    Der_GetDatabaseFiles(omList);
}

void CBaseLogObject::SetDatabaseFiles(const CStringArray& omList)
{
    Der_SetDatabaseFiles(omList);
}




// Set the baud rate details for each channel
void CBaseLogObject::Der_SetChannelBaudRateDetails
(void* controllerDetails,
 int nNumChannels)
{
    Der_SetChannelBaudRateDetails(controllerDetails, nNumChannels);
}

// To get the channel baud rate
void CBaseLogObject::GetChannelBaudRateDetails(void* controllerDetails, int& nNumChannels)
{
    Der_GetChannelBaudRateDetails(controllerDetails, nNumChannels);
}


