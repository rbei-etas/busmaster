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

// FrameProcessor.h : main header file for the FrameProcessor DLL
//

#if !defined FRAMEPROCESSOR_COMMON_H__INCLUDED
#define FRAMEPROCESSOR_COMMON_H__INCLUDED


//#include "FrameProcessor_resource.h"		// main symbols
#include "Utility/Utility_Thread.h"
#include "include/BaseDefs.h"
#include "BaseLogObject.h"

const USHORT FOR_ALL = (USHORT) -1;

typedef CArray<CBaseLogObject*, CBaseLogObject*&> CLogObjArray;

class CFrameProcessor_Common
{
private:

    BOOL    m_bFilterON;
    USHORT  m_ushLastBlkID;
    BOOL    m_bEditingON;
    BYTE    m_bLogFlagTmp;

    void vCopyLogObjArray(CLogObjArray& omLogObjArrayTarget, 
                          const CLogObjArray& omLogObjArraySrc);
    UINT unGetBufSize(void);
    void vUpdateLoggingFlag(void);
    USHORT GetUniqueID(void);
    CLogObjArray* GetActiveLogObjArray(void);

protected:
    CPARAM_THREADPROC   m_sDataCopyThread;
    BOOL                m_bLogEnabled;
    BOOL				m_bResetAbsTime;
    BOOL                m_bClientBufferON;
    CLogObjArray        m_omLogObjectArray;
    CLogObjArray        m_omLogListTmp;

    BYTE     m_bExprnFlag_Log;

    HRESULT DoInitialisation(void);
    virtual void vEmptyLogObjArray(CLogObjArray& omLogObjArray) = 0;
    CBaseLogObject* FindLoggingBlock(USHORT ushID);
    BOOL bIsEditingON(void);
	// To create a new logging object
	virtual CBaseLogObject* CreateNewLogObj(void) = 0;
    // To delete a logging object
    virtual void DeleteLogObj(CBaseLogObject*& pouLogObj) = 0;
	virtual void CreateTimeModeMapping(SYSTEMTIME& CurrSysTime, 
									   UINT64& unAbsTime) = 0;

public:
    // Alias functions - start
    HRESULT EnableLoggingBlock(USHORT ushBlk, BOOL bEnable);
    HRESULT EnableLogging(BOOL bEnable);
    HRESULT EnableFilter(USHORT ushBlk, BOOL bEnable);
    BOOL IsClientBufferON(void);
    BOOL IsLoggingON(void);
    BOOL IsFilterON(void);
    HRESULT LogString(CString& omStr);
    HRESULT AddLoggingBlock(const SLOGINFO& sLogObject);
    HRESULT RemoveLoggingBlock(USHORT ushBlk);
    USHORT GetLoggingBlockCount(void);
    HRESULT ClearLoggingBlockList(void);
    HRESULT GetLoggingBlock(USHORT ushBlk, SLOGINFO& sLogObject);
    HRESULT SetLoggingBlock(USHORT ushBlk, const SLOGINFO& sLogObject);
    HRESULT GetConfigData(BYTE** ppvConfigData, UINT& unLength);
    HRESULT SetConfigData(BYTE* pvDataStream);
    HRESULT Reset(void);
    HRESULT Confirm(void);
    HRESULT StartEditingSession(void);
    HRESULT StopEditingSession(BOOL bConfirm);
	HRESULT SetDatabaseFiles(const CStringArray& omList);
	void GetDatabaseFiles(CStringArray& omList);
	void SetChannelBaudRateDetails(SCONTROLER_DETAILS* controllerDetails, 
									int nNumChannels);
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

#endif // FRAMEPROCESSOR_COMMON_H__INCLUDED
