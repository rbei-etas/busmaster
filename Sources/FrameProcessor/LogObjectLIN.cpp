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
* \file      LogObjectLIN.cpp
* \brief     Source file for CLogObjectLIN class.
* \author    Shashank Vernekar
* \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
*
* Source file for CLogObjectLIN class.
*/

#include "FrameProcessor_stdafx.h"
#include "include/Struct_LIN.h"
#include "LogObjectLIN.h"            // For CLogObjectCAN class declaration
#include <map>
#include <string>

#define LIN_VERSION           "***BUSMASTER Ver 2.0.0***"
#define LIN_LOG_HEADER        "***NOTE: PLEASE DO NOT EDIT THIS DOCUMENT***"
#define LIN_LOG_START         "***[START LOGGING SESSION]***"
#define LIN_LOG_STOP          "***[STOP LOGGING SESSION]***"
#define LIN_LOG_START_DATE_TIME \
    "***START DATE AND TIME %d:%d:%d %d:%d:%d:%d***"
#define LIN_LOG_END_DATE_TIME "***END DATE AND TIME %d:%d:%d %d:%d:%d:%d***"
#define LIN_LOG_HEXFORMAT     "***HEX***"
#define LIN_LOG_DECFORMAT     "***DEC***"
#define LIN_LOG_ABSMODE       "***ABSOLUTE MODE***"
#define LIN_LOG_RELMODE       "***RELATIVE MODE***"
#define LIN_LOG_SYSMODE       "***SYSTEM MODE***"
#define LIN_LOG_COLUMNS       "***<Time><Tx/Rx><Channel><ID><DLC><DataBytes><Checksum>***"



CLogObjectLIN::CLogObjectLIN(CString omVersion):CBaseLogObject(omVersion)
{
    // Initialise the filtering block
    m_sFilterApplied.vClear();
    m_pasControllerDetails = nullptr;
    m_nNumChannels = 0;
}

CLogObjectLIN::~CLogObjectLIN()
{
    if (nullptr != m_pasControllerDetails)
    {
        delete[] m_pasControllerDetails;
        m_pasControllerDetails = nullptr;
    }
}

void CLogObjectLIN::Der_CopySpecificData(const CBaseLogObject* pouLogObjRef)
{
    const CLogObjectLIN* pouLobObjLINRef = static_cast <const CLogObjectLIN*> (pouLogObjRef);
    pouLobObjLINRef->GetFilterInfo(m_sFilterApplied);
}

BOOL CLogObjectLIN::bLogData(const SFORMATTEDDATA_LIN& sDataLIN)
{
    // Multiple return statements are used to keep the code precise.

    SFRAMEINFO_BASIC_LIN LINInfo_Basic =
    {
        sDataLIN.m_dwMsgID, sDataLIN.m_eChannel, sDataLIN.m_eDirection,
        sDataLIN.m_byMsgType, sDataLIN.m_eEventType
    };

    // Assign appropriate values to FrameInfo_Basic

    if (bToBeLogged(LINInfo_Basic) == FALSE)
    {
        return FALSE;
    }

    CString omLogText = "";
    char* pTimeData = nullptr;
    char* pId = nullptr;
    char* pData = nullptr;
    char   acChecksum[LENGTH_STR_CHKSUM_LIN];
    char* pChkType=nullptr;
    switch (m_sLogInfo.m_eLogTimerMode) // Time Mode
    {
        case TIME_MODE_ABSOLUTE:
        {
            if(m_sLogInfo.m_bResetAbsTimeStamp)
            {
                pTimeData = (char*) (sDataLIN.m_acTimeAbsReset);
            }
            else
            {
                pTimeData = (char*) (sDataLIN.m_acTimeAbs);
            }
        }
        break;
        case TIME_MODE_RELATIVE:
        {
            pTimeData = (char*) (sDataLIN.m_acTimeRel);
        }
        break;
        case TIME_MODE_SYSTEM:
        {
            pTimeData = (char*) (sDataLIN.m_acTimeSys);
        }
        break;

        default:
            ASSERT(FALSE);
            break;
    }

    switch (m_sLogInfo.m_eNumFormat)
    {
        case HEXADECIMAL:
        {
            pId = (char*) (sDataLIN.m_acMsgIDHex);

            BYTE CurrDat = sDataLIN.m_byChecksum;

            _stprintf(acChecksum, FORMAT_STR_CHECKSUM_HEX, CurrDat);



            {
                pData = (char*) (sDataLIN.m_acDataHex);
            }
        }
        break;
        case DEC:
        {
            pId = (char*) (sDataLIN.m_acMsgIDDec);

            BYTE CurrDat = sDataLIN.m_byChecksum;
            _stprintf(acChecksum, FORMAT_STR_CHECKSUM_DEC, CurrDat);

            {
                pData = (char*) (sDataLIN.m_acDataDec);
            }
        }
        break;

        default:
            ASSERT(FALSE);
            break;
    }

    if(sDataLIN.m_byMsgType == LIN_MSG)
    {
        //pChecksum = (char*) (sDataLIN.m_byChecksum);
        if(sDataLIN.m_byChecksumType == 0)
        {
            pChkType="Classic";
        }
        else
        {
            pChkType="Enhanced";
        }
        // First put everything in a string to get the length
        omLogText.Format(  "%s %s %s %s %s %s %s(%s) \n",
                           pTimeData,
                           sDataLIN.m_acMsgDir,
                           sDataLIN.m_acChannel,
                           pId,
                           sDataLIN.m_acDataLen,
                           pData,acChecksum,pChkType);

        vWriteTextToFile(omLogText, LIN);
    }

    else if(sDataLIN.m_byMsgType == LIN_EVENT )
    {
        omLogText.Format("%s %s %s %s %s %s %s %s(%s) \n",
                         pTimeData,
                         sDataLIN.m_acType,sDataLIN.m_acMsgDir,
                         sDataLIN.m_acChannel,sDataLIN.m_acDataLen,
                         pId,pData,acChecksum,pChkType
                        );

        vWriteTextToFile(omLogText, LIN);


    }

    return TRUE;
}

// To format the header
void CLogObjectLIN::vFormatHeader(CString& omHeader, ETYPE_BUS /* eBus */)
{
    CBaseLogObject::vFormatHeader(omHeader, LIN);
    omHeader += LIN_LOG_COLUMNS;
    omHeader += L'\n';
}

// To format the footer
void CLogObjectLIN::vFormatFooter(CString& omFooter)
{
    CBaseLogObject::vFormatFooter(omFooter);
}

/*******************************************************************************
//  Function Name  : bLogData
//  Input(s)       : none
//  Output         : none
//  Description    : Logs data. This will open the file in
//                   appropriate mode and will insert header if it is new.
//  Member of      : CBaseLogObject
//  Friend of      : None
//  Author         : Shashank Vernekar
//  Creation Date  : 30.01.2014
*******************************************************************************/
//[RS_LIN_06_12-13]
BOOL CLogObjectLIN::bToBeLogged(SFRAMEINFO_BASIC_LIN& LINInfo_Basic)
{
    // Multiple return statements are used to keep the code precise.

    if (m_sLogInfo.m_bEnabled == FALSE)
    {
        return FALSE;
    }

    if (nullptr == m_pLogFile)
    {
        ASSERT(FALSE);
        return FALSE;
    }

    // Return if the curent frame occurs in different channel.
    if (LIN_CHANNEL_ALL != m_sLogInfo.m_ChannelSelected)
    {
        if (m_sLogInfo.m_ChannelSelected != LINInfo_Basic.m_eChannel)
        {
            return FALSE;
        }
    }

    if (m_sFilterApplied.m_bEnabled == TRUE)
    {
        if (m_sFilterApplied.bToBeBlocked(LINInfo_Basic) == TRUE)
        {
            return FALSE;
        }
    }

    // Check for the triggering conditions
    switch (m_CurrTriggerType)
    {
        case NONE:
            break;

        case STOPPED:
        {
            //If the log file is stopped then don't log
            return FALSE;
        }
        break;
        case START:
        {
            if ((m_sLogInfo.m_sLogTrigger.m_unStartID == LINInfo_Basic.m_dwFrameID)
                    && (LINInfo_Basic.m_eDrctn == DIR_RX))
            {
                m_CurrTriggerType = NONE;
            }
            else
            {
                return FALSE;
            }
        }
        break;
        case STOP:
        {
            if ((m_sLogInfo.m_sLogTrigger.m_unStopID == LINInfo_Basic.m_dwFrameID)
                    && (LINInfo_Basic.m_eDrctn  == DIR_RX))
            {
                m_CurrTriggerType = STOPPED;
            }
        }
        break;

        case BOTH:
        {
            if ((m_sLogInfo.m_sLogTrigger.m_unStartID == LINInfo_Basic.m_dwFrameID)
                    && (LINInfo_Basic.m_eDrctn == DIR_RX))
            {
                m_CurrTriggerType = STOP;
            }
            else
            {
                return FALSE;
            }
        }
        break;

        default:
            ASSERT(FALSE);
            break;
    }

    return TRUE;
}

BYTE* CLogObjectLIN::Der_SetConfigData(BYTE* pvDataStream)
{
    bool bResult = false;
    BYTE* pbSStream = pvDataStream;

    pbSStream = m_sFilterApplied.pbSetConfigData(pbSStream, bResult);

    return pbSStream;
}

int CLogObjectLIN::Der_SetConfigData(xmlNodePtr pNodePtr)
{
    int nResult = S_OK;
    SFILTERAPPLIED_LIN sFilterApplied;
    CStringArray omStrFilters;
    std::map<std::string, int> mapFilters;
    if (S_OK == sFilterApplied.nSetXMLConfigData(pNodePtr->doc, LIN))
    {
        while(pNodePtr != nullptr) //TODO:Move To Utils
        {
            if ( pNodePtr->xmlChildrenNode != nullptr )
            {
                if ((!xmlStrcmp(pNodePtr->name, (const xmlChar*)"Filter")))
                {
                    int nEnabled = 1;
                    xmlAttrPtr pAttr = pNodePtr->properties;
                    while (pAttr)
                    {
                        // walk through all the attributes and find the required one
                        if (pAttr->type == XML_ATTRIBUTE_NODE)
                        {
                            std::string strAttrName((char*)pAttr->name);
                            if ((strAttrName == "IsEnabled") )
                            {
                                nEnabled = atoi((char*)pAttr->children->content);
                                break; // found
                            }
                        }
                        pAttr = pAttr->next;
                    }

                    xmlChar* key = xmlNodeListGetString(pNodePtr->doc, pNodePtr->xmlChildrenNode, 1);
                    if(nullptr != key)
                    {
                        mapFilters[(char*)key] = nEnabled;
                        xmlFree(key);
                    }
                }
            }
            pNodePtr = pNodePtr->next;
        }
        //sFilterApplied.nGetFiltersFromName(m_sFilterApplied, omStrFilters);
        sFilterApplied.nGetFiltersFromName(m_sFilterApplied, mapFilters);
    }
    return nResult;
}
BYTE* CLogObjectLIN::Der_GetConfigData(BYTE* pvDataStream) const
{
    BYTE* pbTStream = pvDataStream;

    pbTStream = m_sFilterApplied.pbGetConfigData(pbTStream);

    return pbTStream;
}

void CLogObjectLIN::Der_GetConfigData(xmlNodePtr pNodePtr) const
{
    m_sFilterApplied.pbGetConfigFilterData(pNodePtr);
}
UINT CLogObjectLIN::Der_unGetBufSize(void) const
{
    return m_sFilterApplied.unGetSize();
}

void CLogObjectLIN::EnableFilter(BOOL bEnable)
{
    m_sFilterApplied.m_bEnabled = bEnable;
}

void CLogObjectLIN::GetFilterInfo(SFILTERAPPLIED_LIN& sFilterInfo) const
{
    sFilterInfo.bClone(m_sFilterApplied);
}

void CLogObjectLIN::SetFilterInfo(const SFILTERAPPLIED_LIN& sFilterInfo)
{
    m_sFilterApplied.bClone(sFilterInfo);
}

void CLogObjectLIN::Der_SetDatabaseFiles(const CStringArray& omList)
{
    // Clear before updating
    m_omListLDFiles.RemoveAll();

    for (int nIdx = 0; nIdx < omList.GetSize(); nIdx++)
    {
        m_omListLDFiles.Add(omList.GetAt(nIdx));
    }
}

// Get the list of LDF files associated
void CLogObjectLIN::Der_GetDatabaseFiles(CStringArray& omList)
{
    omList.Append(m_omListLDFiles);

}

//[RS_LIN_06_08]
void CLogObjectLIN::Der_SetChannelBaudRateDetails(void* controllerDetails, int nNumChannels)
{
    SCONTROLLER_DETAILS_LIN* pTempControllerDetails=(SCONTROLLER_DETAILS_LIN*)controllerDetails;

    if (nullptr != m_pasControllerDetails)
    {
        delete[] m_pasControllerDetails;
    }
    m_pasControllerDetails = nullptr;

    m_pasControllerDetails = new SCONTROLLER_DETAILS_LIN [nNumChannels];
    for (int nIdx = 0; nIdx < nNumChannels; nIdx++)
    {
        m_pasControllerDetails[nIdx] = pTempControllerDetails[nIdx];
        // memcpy(m_pasControllerDetails + nIdx, controllerDetails + nIdx, sizeof(SCONTROLLER_DETAILS));
    }
    m_nNumChannels = nNumChannels;
}

//To get the channel baud rate info for each channel
void CLogObjectLIN::Der_GetChannelBaudRateDetails
(void* controllerDetails, int& nNumChannels)
{
    SCONTROLLER_DETAILS_LIN* pTempControllerDetails=(SCONTROLLER_DETAILS_LIN*)controllerDetails;

    if (nullptr != m_pasControllerDetails && nullptr != pTempControllerDetails)
    {
        for (int nIdx = 0; nIdx < m_nNumChannels; nIdx++)
        {
            pTempControllerDetails[nIdx] = m_pasControllerDetails[nIdx];
            //  memcpy(controllerDetails + nIdx, m_pasControllerDetails + nIdx, sizeof(SCONTROLLER_DETAILS));
        }
        nNumChannels = m_nNumChannels;
    }
}

