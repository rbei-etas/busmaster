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
 * \file      FrameProcessor_Common.h
 * \brief     Definition file for CFrameProcessor_Common class realising
 * \author    Ratnadip Choudhury
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Definition file for CFrameProcessor_Common class realising
 */

#pragma once

#include "Utility/Utility_Thread.h"

#include "CommonClass/RefTimeKeeper.h"
#include "IFrameProcessor_Common.h"
const USHORT FOR_ALL = (USHORT) -1;

typedef CArray<CBaseLogObject*, CBaseLogObject*&> CLogObjArray;

class CFrameProcessor_Common : virtual public IFrameProcessor_Common
{
private:

    bool    m_bFilterON;
    USHORT  m_ushLastBlkID;
    bool    m_bEditingON;
    BYTE    m_bLogFlagTmp;
    CString m_omStrVersion;

    void vCopyLogObjArray(CLogObjArray& omLogObjArrayTarget,
                          const CLogObjArray& omLogObjArraySrc);
    UINT unGetBufSize(void);
    void vUpdateLoggingFlag(void);
    USHORT GetUniqueID(void);
    CLogObjArray* GetActiveLogObjArray(void);
    void parseLogEnabledStatus(xmlDocPtr);
protected:
    CRefTimeKeeper m_ouRefTimer;
    CPARAM_THREADPROC   m_sDataCopyThread;
    bool                m_bLogEnabled;
    bool                m_bResetAbsTime;
    bool                m_bClientBufferON;
    CLogObjArray        m_omLogObjectArray;
    CLogObjArray        m_omLogListTmp;
    SYSTEMTIME          m_LogSysTime;
    bool                m_bIsDataLogged;
    bool                m_bEnableLogOnConnect;
    BYTE     m_bExprnFlag_Log;
    ETYPE_BUS m_eBusType;

    HRESULT DoInitialisation(void);
    virtual void vEmptyLogObjArray(CLogObjArray& omLogObjArray) = 0;
    CBaseLogObject* FindLoggingBlock(USHORT ushID);
    bool bIsEditingON(void);
    // To create a new logging object
    virtual CBaseLogObject* CreateNewLogObj(const CString& omStrVersion) = 0;
    // To delete a logging object
    virtual void DeleteLogObj(CBaseLogObject*& pouLogObj) = 0;
    virtual void CreateTimeModeMapping(SYSTEMTIME& CurrSysTime,
                                       UINT64& unAbsTime) = 0;

public:
    bool                m_bIsThreadBlocked;
    // Alias functions - start
    HRESULT EnableLoggingBlock(USHORT ushBlk, bool bEnable);
    HRESULT EnableLogging(bool bEnable);
    HRESULT EnableLogOnConnect(bool enable);
    HRESULT EnableFilter(USHORT ushBlk, bool bEnable);
    bool IsClientBufferON(void);
    bool IsLoggingON(void);
    bool IsLogOnConnectEnabled();

    bool IsDataLogged(void);

    void vCloseLogFile();
    bool IsThreadBlocked(void);
    void DisableDataLogFlag(void);
    bool IsFilterON(void);
    HRESULT LogString(CString& omStr);
    HRESULT AddLoggingBlock(const SLOGINFO& sLogObject);
    HRESULT RemoveLoggingBlock(USHORT ushBlk);
    USHORT GetLoggingBlockCount(void);
    HRESULT ClearLoggingBlockList(void);
    HRESULT GetLoggingBlock(USHORT ushBlk, SLOGINFO& sLogObject);
    HRESULT SetLoggingBlock(USHORT ushBlk, const SLOGINFO& sLogObject);
    HRESULT GetConfigData(BYTE** ppvConfigData, UINT& unLength);
    HRESULT SetConfigData(BYTE* pvDataStream, const CString& omStrVersion);
    HRESULT SetConfigData( xmlDocPtr pDoc);
    void vAddLogFile( CLogObjArray& omLogListTmp, CBaseLogObject*& pouBaseLogObj);

    HRESULT GetConfigData(xmlNodePtr pxmlNodePtr);
    HRESULT Reset(void);
    HRESULT Confirm(void);
    HRESULT StartEditingSession(void);
    HRESULT StopEditingSession(bool bConfirm);
    HRESULT SetDatabaseFiles(const CStringArray& omList);
    void GetDatabaseFiles(CStringArray& omList);
    void SetChannelBaudRateDetails(void* controllerDetails,
                                   int nNumChannels);

    virtual std::string getConfigPath();


    // Alias functions - end

    // Overrides
public:
    CFrameProcessor_Common();
    virtual ~CFrameProcessor_Common();

    // To be used by the read thread
    virtual void vRetrieveDataFromBuffer(void) = 0;
    void InitTimeParams(void);

    virtual BOOL InitInstance(void);
    virtual int ExitInstance(void);
};
