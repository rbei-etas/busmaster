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

//#include "FrameProcessor_resource.h"      // main symbols
#include "Utility/Utility_Thread.h"
#include "include/BaseDefs.h"
#include "BaseLogObject.h"
#include "CommonClass/RefTimeKeeper.h"
const USHORT FOR_ALL = (USHORT) -1;

typedef CArray<CBaseLogObject*, CBaseLogObject*&> CLogObjArray;

class CFrameProcessor_Common
{
private:

    BOOL    m_bFilterON;
    USHORT  m_ushLastBlkID;
    BOOL    m_bEditingON;
    BYTE    m_bLogFlagTmp;
    CString m_omStrVersion;

    void vCopyLogObjArray(CLogObjArray& omLogObjArrayTarget,
                          const CLogObjArray& omLogObjArraySrc);
    UINT unGetBufSize(void);
    void vUpdateLoggingFlag(void);
    USHORT GetUniqueID(void);
    CLogObjArray* GetActiveLogObjArray(void);

protected:
    CRefTimeKeeper m_ouRefTimer;
    CPARAM_THREADPROC   m_sDataCopyThread;
    BOOL                m_bLogEnabled;
    BOOL                m_bResetAbsTime;
    BOOL                m_bClientBufferON;
    CLogObjArray        m_omLogObjectArray;
    CLogObjArray        m_omLogListTmp;
    SYSTEMTIME          m_LogSysTime;
    // PTV [1.6.4]
    BOOL                m_bIsDataLogged;
    BOOL                m_bIsJ1939DataLogged;
    BOOL                m_bIsLINDataLogged;
    BYTE     m_bExprnFlag_Log;

    HRESULT DoInitialisation(void);
    virtual void vEmptyLogObjArray(CLogObjArray& omLogObjArray) = 0;
    CBaseLogObject* FindLoggingBlock(USHORT ushID);
    BOOL bIsEditingON(void);
    // To create a new logging object
    virtual CBaseLogObject* CreateNewLogObj(const CString& omStrVersion) = 0;
    // To delete a logging object
    virtual void DeleteLogObj(CBaseLogObject*& pouLogObj) = 0;
    virtual void CreateTimeModeMapping(SYSTEMTIME& CurrSysTime,
                                       UINT64& unAbsTime) = 0;

public:
    BOOL                m_bIsThreadBlocked;
    // Alias functions - start
    HRESULT EnableLoggingBlock(USHORT ushBlk, BOOL bEnable);
    HRESULT EnableLogging(BOOL bEnable, ETYPE_BUS);
    HRESULT EnableFilter(USHORT ushBlk, BOOL bEnable);
    BOOL IsClientBufferON(void);
    BOOL IsLoggingON(void);
    // PTV [1.6.4]
    BOOL IsDataLogged(void);

    void vCloseLogFile();
    BOOL IsJ1939DataLogged(void);
    BOOL IsLINDataLogged(void);
    BOOL IsThreadBlocked(void);
    void DisableDataLogFlag(void);
    void DisableJ1939DataLogFlag(void);
    void DisableLINDataLogFlag(void);
    // PTV [1.6.4]
    BOOL IsFilterON(void);
    HRESULT LogString(CString& omStr);
    HRESULT AddLoggingBlock(const SLOGINFO& sLogObject);
    HRESULT RemoveLoggingBlock(USHORT ushBlk);
    USHORT GetLoggingBlockCount(void);
    HRESULT ClearLoggingBlockList(void);
    HRESULT GetLoggingBlock(USHORT ushBlk, SLOGINFO& sLogObject);
    HRESULT SetLoggingBlock(USHORT ushBlk, const SLOGINFO& sLogObject);
    HRESULT GetConfigData(BYTE** ppvConfigData, UINT& unLength);
    HRESULT SetConfigData(BYTE* pvDataStream, const CString& omStrVersion);
    //MVN
    HRESULT SetConfigData( xmlDocPtr pDoc, ETYPE_BUS eBus);
    void vAddLogFile( CLogObjArray& omLogListTmp, CBaseLogObject*& pouBaseLogObj);
    //~MVN

    // PTV XML
    HRESULT GetConfigData(xmlNodePtr pxmlNodePtr);
    // PTV XML
    HRESULT Reset(void);
    HRESULT Confirm(void);
    HRESULT StartEditingSession(void);
    HRESULT StopEditingSession(BOOL bConfirm);
    HRESULT SetDatabaseFiles(const CStringArray& omList);
    void GetDatabaseFiles(CStringArray& omList);
    void SetChannelBaudRateDetails(void* controllerDetails,
                                   int nNumChannels,ETYPE_BUS eBus);




    // Alias functions - end

    // Overrides
public:
    CFrameProcessor_Common();
    ~CFrameProcessor_Common();

    // To be used by the read thread
    virtual void vRetrieveDataFromBuffer(void) = 0;
    void InitTimeParams(void);

    virtual BOOL InitInstance(void);
    virtual int ExitInstance(void);
};
