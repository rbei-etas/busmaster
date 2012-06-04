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
 * \file      TSExecutorResult.h
 * \author    Venkatanarayana makam
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 */

#pragma once

#include "TestSetupEditorLib/TestSetupHeader.h"

class CSignalResult
{
public:
    //CString m_omMessage;
	CString m_omSignal;
	CString m_omSignalCondition;
	CString m_omSignalValue;
	CString m_omResult;
    /*CSignalResult& operator=(const CSignalResult& RefObj)
    {
        m_omSignal = RefObj.m_omSignal;
	    m_omSignalCondition = RefObj.m_omSignalCondition;
	    m_omSignalValue = RefObj.m_omSignalValue;
	    m_omResult = RefObj.m_omResult;
        return *this;
    }*/
};
typedef CList<CSignalResult, CSignalResult&> CSignalResultList;
class CMessageResult
{
public:
    CString m_omMessage;
    CSignalResultList m_SignalResultList;
    CMessageResult& operator=(const CMessageResult& RefObj)
    {
        m_omMessage = RefObj.m_omMessage;
        m_SignalResultList.RemoveAll();
        INT nCount = (INT)RefObj.m_SignalResultList.GetCount();
        for(INT i = 0; i < nCount; i++)
        {
            POSITION pos = RefObj.m_SignalResultList.FindIndex(i);
            //m_SignalResultList.AddTail(RefObj.m_SignalResultList.GetAt(pos));
			CSignalResult msg = RefObj.m_SignalResultList.GetAt(pos);
			m_SignalResultList.AddTail(msg);
        }
        return *this;
    }
};
typedef CList<CMessageResult, CMessageResult&> CMessageResultList;
class CResultVerify
{
public:
	CString m_omStrMsg;
	CMessageResultList m_MessageResultList;
    eERROR_ATTRIBUTE m_eResult;
    CResultVerify& operator=(CResultVerify& RefObj)
    {
        m_omStrMsg = RefObj.m_omStrMsg;
        m_eResult = RefObj.m_eResult;
        m_MessageResultList.RemoveAll();
        INT nCount = (INT)RefObj.m_MessageResultList.GetCount();
        for(INT i = 0; i < nCount; i++)
        {
            POSITION pos = RefObj.m_MessageResultList.FindIndex(i);
            m_MessageResultList.AddTail(RefObj.m_MessageResultList.GetAt(pos));
        }

        return *this;
    }
};

typedef CList<CResultVerify, CResultVerify&> CResultVerifyList;
class CResultTc
{
public:
    CString m_omTestCase;
	CString m_omTestDescripter;
	SYSTEMTIME m_sStartTime;
	SYSTEMTIME m_sEndTime;
	eERROR_ATTRIBUTE m_eResult;
	CResultVerifyList m_ouVerifyList;

    CResultTc();
    ~CResultTc();
    CResultTc& operator=(CResultTc& RefObj);
};

typedef CList<CResultTc, CResultTc&> CTCResultTcList;

class CSummary
{
	UINT m_unTcExecuted;
	UINT m_unTcSucceded;
	UINT m_unTcFailed;
	UINT m_unTcLeft;
public:
    CSummary();
    ~CSummary();
};



class CResultGenerator
{
public:
    CResultGenerator(void);
    ~CResultGenerator(void);
    HRESULT GenerateReport();
    SYSTEMTIME m_sSystemTime;
    CTestSetupHeader m_ouHeaderInfo;
    CString m_omStrTestSetupFile;
	CTCResultTcList m_odTcResultList;
	CSummary m_ouSummary;
	SYSTEMTIME m_SendTime;
	CString m_omStrVersion;
private:
    CString SysTimeToString(SYSTEMTIME& systime);
    INT nGenerateTextReport(CStdioFile& omReportFile);
    INT nGenerateHtmlReport(CStdioFile& omReportFile);
};
