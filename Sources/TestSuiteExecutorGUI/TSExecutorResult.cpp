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
 * \file      TSExecutorResult.cpp
 * \author    Venkatanarayana makam
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 */
#include "TSExecutorGUI_stdafx.h"
#include "TSExecutorResult.h"
#include "TSExecutorGUI_resource.h"
#define defMAX_CHAR_PER_LINE        80

/******************************************************************************
Function Name  :  CResultTc
Input(s)       :
Output         :  -
Functionality  :
Member of      :  CResultTc
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  07/04/2011
Modifications  :
Code Tag       :
******************************************************************************/
CResultTc::CResultTc()
{
    m_omTestCase = "";
    m_omTestDescripter = "";
    GetSystemTime(&m_sStartTime);
    GetSystemTime(&m_sEndTime);
    m_eResult = ERRORS;
    m_ouVerifyList.RemoveAll();
};

/******************************************************************************
Function Name  :  operator=
Input(s)       :  CResultTc& RefObj
Output         :  CResultTc&
Functionality  :  = operator overloading
Member of      :  CResultTc
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  07/04/2011
Modifications  :
Code Tag       :
******************************************************************************/
CResultTc& CResultTc::operator=(CResultTc& RefObj)
{
    m_omTestCase = RefObj.m_omTestCase;
    m_omTestDescripter = RefObj.m_omTestDescripter;
    m_sStartTime = RefObj.m_sStartTime;
    m_sEndTime = RefObj.m_sEndTime;
    m_eResult = RefObj.m_eResult;
    m_ouVerifyList.RemoveAll();
    INT nCount = (INT)RefObj.m_ouVerifyList.GetCount();

    for(INT nIndex=0; nIndex<nCount; nIndex++)
    {
        POSITION pos = RefObj.m_ouVerifyList.FindIndex(nIndex);
        m_ouVerifyList.AddTail(RefObj.m_ouVerifyList.GetAt(pos));
    }

    return *this;
}
/******************************************************************************
Function Name  :  ~CResultTc
Input(s)       :  -
Output         :  -
Functionality  :  Destructor
Member of      :  CResultTc
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  07/04/2011
Modifications  :
Code Tag       :
******************************************************************************/
CResultTc::~CResultTc(void)
{
}

/******************************************************************************
Function Name  :  CSummary
Input(s)       :  -
Output         :  -
Functionality  :  Constructor
Member of      :  CSummary
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  07/04/2011
Modifications  :
Code Tag       :
******************************************************************************/
CSummary::CSummary(void)
{
    m_unTcExecuted = 0;
    m_unTcSucceded = 0;
    m_unTcFailed = 0;
    m_unTcLeft =0;
}

/******************************************************************************
Function Name  :  ~CSummary
Input(s)       :  -
Output         :  -
Functionality  :  Destructor
Member of      :  CSummary
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  07/04/2011
Modifications  :
Code Tag       :
******************************************************************************/
CSummary::~CSummary(void)
{
}

/******************************************************************************
Function Name  :  CResultGenerator
Input(s)       :  -
Output         :  -
Functionality  :  Constructor
Member of      :  CResultGenerator
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  07/04/2011
Modifications  :
Code Tag       :
******************************************************************************/
CResultGenerator::CResultGenerator(void)
{
}

/******************************************************************************
Function Name  :  ~CResultGenerator
Input(s)       :  -
Output         :  -
Functionality  :  Destructor
Member of      :  CResultGenerator
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  07/04/2011
Modifications  :
Code Tag       :
******************************************************************************/
CResultGenerator::~CResultGenerator(void)
{
}

/******************************************************************************
Function Name  :  GenerateReport
Input(s)       :  -
Output         :  HRESULT
Functionality  :  Genrates the report
Member of      :  CResultGenerator
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  07/04/2011
Modifications  :
Code Tag       :  CS042
******************************************************************************/
HRESULT CResultGenerator::GenerateReport(void)
{
    CStdioFile omReportFile;

    if(omReportFile.Open(m_ouHeaderInfo.m_sReportFile.m_omPath, CFile::modeCreate|CFile::modeReadWrite)!=NULL)
    {
        if(m_ouHeaderInfo.m_sReportFile.m_eType == TXT)
        {
            nGenerateTextReport(omReportFile);
        }
        else
        {
            nGenerateHtmlReport(omReportFile);
        }

        omReportFile.Close();
    }

    return S_OK;
}

/******************************************************************************
Function Name  :  nGenerateHtmlReport
Input(s)       :  CStdioFile& omReportFile
Output         :  INT
Functionality  :  Generates the HTML report
Member of      :  CResultGenerator
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  07/04/2011
Modifications  :
Code Tag       :
******************************************************************************/
INT CResultGenerator::nGenerateHtmlReport(CStdioFile& omReportFile)
{
    CString omStrHtml;
    omStrHtml.LoadString(IDS_HTMLSTART);
    omReportFile.WriteString(omStrHtml);
    nGenerateTextReport(omReportFile);
    omStrHtml.LoadString(IDS_HTMLEND);
    omReportFile.WriteString(omStrHtml);
    return 0;
}

/******************************************************************************
Function Name  :  nGenerateTextReport
Input(s)       :  CStdioFile& omReportFile
Output         :  INT
Functionality  :  Generates Text Report
Member of      :  CResultGenerator
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  07/04/2011
Modifications  :
Code Tag       :
******************************************************************************/
INT CResultGenerator::nGenerateTextReport(CStdioFile& omReportFile)
{
    //BUSMASTER Version
    omReportFile.WriteString("\n\t\t\t\tBUSMASTER Version:: BUSMASTER " + m_omStrVersion);
    omReportFile.WriteString("\n\t\t\t\tReport File For:: ");
    omReportFile.WriteString(m_omStrTestSetupFile);
    omReportFile.WriteString("\n\nTestSetup File Information:");
    omReportFile.WriteString("\n---------------------------\n\t");
    omReportFile.WriteString("\n\tTitle: "+ m_omStrTestSetupFile);
    omReportFile.WriteString("\n\tModule Name:"+m_ouHeaderInfo.m_sModuleName.m_omValue);
    omReportFile.WriteString("\n\tVersion: "+m_ouHeaderInfo.m_sVersion.m_omValue);
    omReportFile.WriteString("\n\tDatabase Information:"+m_ouHeaderInfo.m_omDatabasePath);
    omReportFile.WriteString("\n\n");
    omReportFile.WriteString("Engineer Info:");
    omReportFile.WriteString("\n--------------");
    omReportFile.WriteString("\n\tName: "+m_ouHeaderInfo.m_sEngineerInfo1.m_omValue);
    omReportFile.WriteString("\n\tDesignation: "+m_ouHeaderInfo.m_sEngineerInfo2.m_omValue);
    omReportFile.WriteString("\nResults:");
    omReportFile.WriteString("\n-------");
    omReportFile.WriteString("\n");
    INT nTcCount = (INT)m_odTcResultList.GetCount();

    for(INT nTcIndex = 0; nTcIndex< nTcCount; nTcIndex++)
    {
        POSITION pos = m_odTcResultList.FindIndex(nTcIndex);
        CResultTc& ouResultTc = m_odTcResultList.GetAt(pos);
        omReportFile.WriteString("\nTestCase:  "+ouResultTc.m_omTestCase);

        if(ouResultTc.m_eResult == ERRORS)
        {
            omReportFile.WriteString("\nResult:\tFAILED");
        }
        else
        {
            omReportFile.WriteString("\nResult:\tSUCCESS");
        }

        omReportFile.WriteString("\nStratTime:"+SysTimeToString(ouResultTc.m_sStartTime));
        omReportFile.WriteString("\nEndTime:"+SysTimeToString(ouResultTc.m_sEndTime));
        //omReportFile.WriteString("\n");
        INT nVerifyCount = (INT)ouResultTc.m_ouVerifyList.GetCount();

        for(INT nVerifyIndex = 0; nVerifyIndex <nVerifyCount; nVerifyIndex++)
        {
            CString omStrVerify;
            omStrVerify.Format("\n\tVerify - %d", nVerifyIndex+1);
            omReportFile.WriteString(omStrVerify);
            POSITION pos = ouResultTc.m_ouVerifyList.FindIndex(nVerifyIndex);
            CResultVerify& ouVerify = ouResultTc.m_ouVerifyList.GetAt(pos);
            INT nMsgCount = (INT)ouVerify.m_MessageResultList.GetCount();

            if(nMsgCount == 0)
            {
                CString omStrTemp("No Message is Verified");
                omReportFile.WriteString("\n\t\t" + omStrTemp);
            }
            else
            {
                for(INT nMsgIndex = 0; nMsgIndex<nMsgCount; nMsgIndex++)
                {
                    POSITION pos = ouVerify.m_MessageResultList.FindIndex(nMsgIndex);
                    CMessageResult& ouMsgResult = ouVerify.m_MessageResultList.GetAt(pos);
                    omReportFile.WriteString("\n\t\t" + ouMsgResult.m_omMessage + ":");
                    INT nSignalCount = (INT)ouMsgResult.m_SignalResultList.GetCount();

                    for(INT nSignalIndex=0; nSignalIndex<nSignalCount; nSignalIndex++)
                    {
                        CString omStrTemp;
                        POSITION posSignal = ouMsgResult.m_SignalResultList.FindIndex(nSignalIndex);
                        CSignalResult ouSignalResult = ouMsgResult.m_SignalResultList.GetAt(posSignal);
                        omStrTemp.Format("\n\t\t\t%-32s %16s (x = %s) %16s", ouSignalResult.m_omSignal, ouSignalResult.m_omSignalCondition,
                                         ouSignalResult.m_omSignalValue, ouSignalResult.m_omResult);
                        omReportFile.WriteString(omStrTemp);
                    }
                }
            }
        }

        omReportFile.WriteString("\n");
    }

    return 0;
}

/******************************************************************************
Function Name  :  SysTimeToString
Input(s)       :  SYSTEMTIME& systime
Output         :  CString
Functionality  :  Gets The time in String Format
Member of      :  CResultGenerator
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  07/04/2011
Modifications  :
Code Tag       :
******************************************************************************/
CString CResultGenerator::SysTimeToString(SYSTEMTIME& systime)
{
    //Taken From Msdn
    // Determine day of the week.
    CString day;

    switch (systime.wDayOfWeek)
    {
        case 0:
            day = "Sunday";
            break;

        case 1:
            day = "Monday";
            break;

        case 2:
            day = "Tuesday";
            break;

        case 3:
            day = "Wednesday";
            break;

        case 4:
            day = "Thursday";
            break;

        case 5:
            day = "Friday";
            break;

        case 6:
            day = "Saturday";
            break;
    }

    CString omStrTime;
    omStrTime.Format("%s %u/%u/%u  %u:%u:%u:%u",
                     day,
                     systime.wYear, systime.wMonth, systime.wDay,
                     systime.wHour, systime.wMinute, systime.wSecond,
                     systime.wMilliseconds);
    return omStrTime;
}