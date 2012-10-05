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
 * \file      Log_Datatypes.cpp
 * \brief     Source file for log data types.
 * \author    Ratnadip Choudhury
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Source file for log data types.
 */


#include "DataTypes_StdAfx.h"
#include "include/Utils_macro.h"
#include "Log_Datatypes.h"
#include "include/XMLDefines.h"
#include <Shlwapi.h>
#include "Utility\UtilFunctions.h"

#define MSG_GET_CONFIGPATH  10000

/* tagLogInfo --- STARTS */

/******************************************************************************
  Function Name    :  tagLogInfo
  Input(s)         :  -
  Output           :  -
  Functionality    :  Standard constructor
  Member of        :  tagLogInfo
  Friend of        :  -
  Author(s)        :  Ratnadip Choudhury
  Date Created     :  1.12.2009
  Modification date:
  Modification By  :
******************************************************************************/
tagLogInfo::tagLogInfo()
{
    vClear();
}

/******************************************************************************
  Function Name    :  vClear
  Input(s)         :  void
  Output           :  void
  Functionality    :  Clears information inthe current logging block
  Member of        :  tagLogInfo
  Friend of        :  -
  Author(s)        :  Ratnadip Choudhury
  Date Created     :  1.12.2009
  Modification date:
  Modification By  :
******************************************************************************/
void tagLogInfo::vClear(void)
{
    m_ushID              = 0x0;
    m_bIsUpdated         = TRUE;
    m_bEnabled           = TRUE;
    m_eLogTimerMode      = TIME_MODE_SYSTEM;
    m_eNumFormat         = HEXADECIMAL;
    m_eFileMode          = APPEND_MODE;
    m_bResetAbsTimeStamp = FALSE;
    m_ChannelSelected    = CHANNEL_All_UNSPECIFIED;
    strcpy_s(m_sLogFileName, _MAX_PATH, "");

    // Init Trigger Condition
    m_sLogTrigger.m_unTriggerType = NONE;   // No trigger
    m_sLogTrigger.m_unStartID = 0;          // No Start-ID
    m_sLogTrigger.m_unStopID = 0;           // No Stop-ID
}

/******************************************************************************
  Function Name    :  unGetSize
  Input(s)         :  void
  Output           :  UINT - size of the current logging block
  Functionality    :  Returns in bytes size of the current logging block.
  Member of        :  tagLogInfo
  Friend of        :  -
  Author(s)        :  Ratnadip Choudhury
  Date Created     :  1.12.2009
  Modification date:
  Modification By  :
******************************************************************************/
UINT tagLogInfo::unGetSize(void) const
{
    UINT Result =
        sizeof(m_bIsUpdated) + sizeof(m_bEnabled) + sizeof(m_eLogTimerMode) +
        sizeof(m_eNumFormat) + sizeof(m_eFileMode) + sizeof(m_bResetAbsTimeStamp) + sizeof(m_ChannelSelected)
        + sizeof(m_sLogFileName) + sizeof(m_sLogTrigger);

    return Result;
}

/******************************************************************************
  Function Name    :  pbGetConfigData
  Input(s)         :  pbTarget - The target buffer to save logging data.
                      It assumes that pbTarget is currently pointing to a loca-
                      tion having sufficient space.
  Output           :  BYTE * - Location of the next available buffer.
  Functionality    :  Saves logging block information of the current object into
                      the target buffer. Advances the writing pointer to the
                      next byte occurring after the written block.
  Member of        :  tagLogInfo
  Friend of        :  -
  Author(s)        :  Ratnadip Choudhury
  Date Created     :  1.12.2009
  Modification date:
  Modification By  :
******************************************************************************/
BYTE* tagLogInfo::pbGetConfigData(BYTE* pbTarget) const
{
    BYTE* pbTStream = pbTarget;

    COPY_DATA(pbTStream, &m_bIsUpdated,       sizeof(m_bIsUpdated        ));
    COPY_DATA(pbTStream, &m_bEnabled,         sizeof(m_bEnabled          ));
    COPY_DATA(pbTStream, &m_eLogTimerMode,    sizeof(m_eLogTimerMode     ));
    COPY_DATA(pbTStream, &m_eNumFormat,       sizeof(m_eNumFormat        ));
    COPY_DATA(pbTStream, &m_eFileMode,        sizeof(m_eFileMode         ));
    COPY_DATA(pbTStream, &m_bResetAbsTimeStamp, sizeof(m_bResetAbsTimeStamp));
    COPY_DATA(pbTStream, &m_ChannelSelected,  sizeof(m_ChannelSelected   ));
    COPY_DATA(pbTStream, m_sLogFileName,      sizeof(m_sLogFileName      ));
    COPY_DATA(pbTStream, &m_sLogTrigger,      sizeof(m_sLogTrigger       ));

    return pbTStream;
}

BOOL tagLogInfo::pbGetConfigData(xmlNodePtr pxmlNodePtr) const
{
    const char* omstrIsLogEnabled = "";

    if(m_bEnabled == TRUE)
    {
        omstrIsLogEnabled = "TRUE";
    }
    else
    {
        omstrIsLogEnabled = "FALSE";
    }

    const char* omstrTimerMode = "";

    if(m_eLogTimerMode == TIME_MODE_SYSTEM)
    {
        omstrTimerMode = "SYSTEM";
    }
    else if(m_eLogTimerMode == TIME_MODE_RELATIVE)
    {
        omstrTimerMode = "RELATIVE";
    }
    else if(m_eLogTimerMode == TIME_MODE_ABSOLUTE)
    {
        omstrTimerMode = "ABSOLUTE";
    }

    const char* omstrNumMode = "";

    if(m_eNumFormat == HEXADECIMAL)
    {
        omstrNumMode = "HEX";
    }
    else if(m_eNumFormat == DEC)
    {
        omstrNumMode = "DEC";
    }

    const char* omstrFileMode = "FALSE";

    if(m_eFileMode == APPEND_MODE)
    {
        omstrFileMode = "TRUE";
    }

    const char* omstrAbsMode = "FALSE";

    if(m_bResetAbsTimeStamp == TRUE)
    {
        omstrAbsMode = "TRUE";
    }

    CString omstrChannel = "";

    if (m_ChannelSelected == CHANNEL_All_UNSPECIFIED || m_ChannelSelected > CHANNEL_ALLOWED )
    {
        //due to const declaration
        omstrChannel.Format("%d", 0);
    }
    else
    {
        omstrChannel.Format("%d", m_ChannelSelected);
    }

    const char* omChannel = omstrChannel;


    CString omstrStartId = "";
    CString omstrStpId = "";
    if ( m_sLogTrigger.m_unTriggerType == START )
    {
        omstrStartId.Format("%d", m_sLogTrigger.m_unStartID);
        omstrStpId.Format("%d", -1);
    }
    else if ( m_sLogTrigger.m_unTriggerType == STOP )
    {
        omstrStartId.Format("%d", -1);
        omstrStpId.Format("%d", m_sLogTrigger.m_unStopID);
    }
    else if ( m_sLogTrigger.m_unTriggerType == BOTH )
    {
        omstrStartId.Format("%d", m_sLogTrigger.m_unStartID);
        omstrStpId.Format("%d", m_sLogTrigger.m_unStopID);
    }
    else
    {
        omstrStartId.Format("%d", -1);
        omstrStpId.Format("%d", -1);
    }

    const char* omcStartId = omstrStartId;

    const char* omcStpId = omstrStpId;

    // Is log enabled
    xmlNodePtr pLogEnbldPtr = xmlNewChild(pxmlNodePtr, NULL, BAD_CAST DEF_IsEnabled, BAD_CAST omstrIsLogEnabled);
    xmlAddChild(pxmlNodePtr, pLogEnbldPtr);

    xmlNodePtr pTimerMdePtr = xmlNewChild(pxmlNodePtr, NULL, BAD_CAST DEF_TIME_MODE, BAD_CAST omstrTimerMode);
    xmlAddChild(pxmlNodePtr, pTimerMdePtr);

    xmlNodePtr pNumMdPtr = xmlNewChild(pxmlNodePtr, NULL, BAD_CAST DEF_NUMERIC_MODE, BAD_CAST omstrNumMode);
    xmlAddChild(pxmlNodePtr, pNumMdPtr);

    xmlNodePtr pAppEnbldPtr = xmlNewChild(pxmlNodePtr, NULL, BAD_CAST DEF_IS_APPEND_ENABLED, BAD_CAST omstrFileMode);
    xmlAddChild(pxmlNodePtr, pAppEnbldPtr);

    xmlNodePtr pAbsMdPtr = xmlNewChild(pxmlNodePtr, NULL, BAD_CAST DEF_RESET_ABS_TIME, BAD_CAST omstrAbsMode);
    xmlAddChild(pxmlNodePtr, pAbsMdPtr);

    xmlNodePtr pChnlPtr = xmlNewChild(pxmlNodePtr, NULL, BAD_CAST DEF_CHANNEL, BAD_CAST omChannel);
    xmlAddChild(pxmlNodePtr, pChnlPtr);

    string omPath;
    char configPath[MAX_PATH];
    string omStrConfigFolder;
    AfxGetMainWnd()->SendMessage(MSG_GET_CONFIGPATH, (WPARAM)configPath, 0);
    CUtilFunctions::nGetBaseFolder(configPath, omStrConfigFolder );
    CUtilFunctions::MakeRelativePath(omStrConfigFolder.c_str(), (char*)m_sLogFileName, omPath);
    xmlNodePtr pLogFilePtr = xmlNewChild(pxmlNodePtr, NULL, BAD_CAST DEF_FILE_PATH, BAD_CAST omPath.c_str());
    xmlAddChild(pxmlNodePtr, pLogFilePtr);

    xmlNodePtr pStrtIdPtr = xmlNewChild(pxmlNodePtr, NULL, BAD_CAST DEF_TRGR_STRT_ID, BAD_CAST omcStartId);
    xmlAddChild(pxmlNodePtr, pStrtIdPtr);

    xmlNodePtr pStpIdPtr = xmlNewChild(pxmlNodePtr, NULL, BAD_CAST DEF_TRGR_STP_ID, BAD_CAST omcStpId);
    xmlAddChild(pxmlNodePtr, pStpIdPtr);

    /*xmlNewChild(pxmlNodePtr, NULL, BAD_CAST DEF_CAN_LOG_BLOCK, BAD_CAST "");
    xmlNewChild(pxmlNodePtr, NULL, BAD_CAST DEF_CAN_LOG_BLOCK, BAD_CAST "");
    xmlNewChild(pxmlNodePtr, NULL, BAD_CAST DEF_CAN_LOG_BLOCK, BAD_CAST "");
    xmlNewChild(pxmlNodePtr, NULL, BAD_CAST DEF_CAN_LOG_BLOCK, BAD_CAST "");
    xmlNewChild(pxmlNodePtr, NULL, BAD_CAST DEF_CAN_LOG_BLOCK, BAD_CAST "");*/

    /*BYTE* pbTStream = pbTarget;

    COPY_DATA(pbTStream, &m_bIsUpdated,       sizeof(m_bIsUpdated        ));
    COPY_DATA(pbTStream, &m_bEnabled,         sizeof(m_bEnabled          ));
    COPY_DATA(pbTStream, &m_eLogTimerMode,    sizeof(m_eLogTimerMode     ));
    COPY_DATA(pbTStream, &m_eNumFormat,       sizeof(m_eNumFormat        ));
    COPY_DATA(pbTStream, &m_eFileMode,        sizeof(m_eFileMode         ));
    COPY_DATA(pbTStream, &m_bResetAbsTimeStamp, sizeof(m_bResetAbsTimeStamp));
    COPY_DATA(pbTStream, &m_ChannelSelected,  sizeof(m_ChannelSelected   ));
    COPY_DATA(pbTStream, m_sLogFileName,      sizeof(m_sLogFileName      ));
    COPY_DATA(pbTStream, &m_sLogTrigger,      sizeof(m_sLogTrigger       ));*/

    return TRUE;
}
/******************************************************************************
  Function Name    :  pbSetConfigData
  Input(s)         :  pbSource - The source buffer to retrieve logging data.
                      It assumes that pbSource is currently pointing to locati-
                      on of a logging block data.
  Output           :  BYTE * - Location of the next byte after the block.
  Functionality    :  Retrieves logging block information and copies them into
                      the current object. Advances the reading pointer to the
                      next byte occurring after the block.
  Member of        :  tagLogInfo
  Friend of        :  -
  Author(s)        :  Ratnadip Choudhury
  Date Created     :  1.12.2009
  Modification date:
  Modification By  :
******************************************************************************/
BYTE* tagLogInfo::pbSetConfigData(BYTE* pbSource, BYTE bytLogVersion)
{
    BYTE* pbSStream = pbSource;

    COPY_DATA_2(&m_bIsUpdated,       pbSStream, sizeof(m_bIsUpdated      ));
    COPY_DATA_2(&m_bEnabled,         pbSStream, sizeof(m_bEnabled        ));
    COPY_DATA_2(&m_eLogTimerMode,    pbSStream, sizeof(m_eLogTimerMode   ));
    COPY_DATA_2(&m_eNumFormat,       pbSStream, sizeof(m_eNumFormat      ));
    COPY_DATA_2(&m_eFileMode,        pbSStream, sizeof(m_eFileMode       ));

    /* If version is 1.6.2 and above */
    if ( bytLogVersion > 0x1 )
    {
        COPY_DATA_2(&m_bResetAbsTimeStamp, pbSStream, sizeof(m_bResetAbsTimeStamp));
    }

    COPY_DATA_2(&m_ChannelSelected,  pbSStream, sizeof(m_ChannelSelected ));
    COPY_DATA_2(m_sLogFileName,      pbSStream, sizeof(m_sLogFileName    ));
    COPY_DATA_2(&m_sLogTrigger,      pbSStream, sizeof(m_sLogTrigger     ));

    return pbSStream;
}
//MVN

INT tagLogInfo::nSetConfigData(xmlNodePtr pNodePtr)
{
    //pNodePtr = pNodePtr->xmlChildrenNode;
    vClear();   //To give Initial Values
    INT nRetValue = S_OK;
    m_sLogTrigger.m_unStartID = -1;
    m_sLogTrigger.m_unStopID = -1;

    while (pNodePtr != NULL)
    {
        if ( pNodePtr->xmlChildrenNode != NULL )
        {
            if ((!xmlStrcmp((const xmlChar*)pNodePtr->name, (const xmlChar*)"IsEnabled")))
            {
                xmlChar* key = xmlNodeListGetString(pNodePtr->doc, pNodePtr->xmlChildrenNode, 1);
                if(NULL != key)
                {
                    m_bEnabled = xmlUtils::bGetBooleanValue((char*)key);
                    xmlFree(key);
                }
            }
            else if ((!xmlStrcmp((const xmlChar*)pNodePtr->name, (const xmlChar*)"Time_Mode")))
            {
                xmlChar* key = xmlNodeListGetString(pNodePtr->doc, pNodePtr->xmlChildrenNode, 1);
                if(NULL != key)
                {
                    m_eLogTimerMode =  xmlUtils::eGetTimerMode((char*)key);
                    xmlFree(key);
                }
            }
            else if ((!xmlStrcmp((const xmlChar*)pNodePtr->name, (const xmlChar*)"Numeric_Mode")))
            {
                xmlChar* key = xmlNodeListGetString(pNodePtr->doc, pNodePtr->xmlChildrenNode, 1);
                if(NULL != key)
                {
                    m_eNumFormat = xmlUtils::eGetNumericMode((char*)key);
                    xmlFree(key);
                }
            }

            else if ((!xmlStrcmp((const xmlChar*)pNodePtr->name, (const xmlChar*)"IsAppendLog_Enabled")))
            {
                xmlChar* key = xmlNodeListGetString(pNodePtr->doc, pNodePtr->xmlChildrenNode, 1);
                if(NULL != key)
                {
                    BOOL bValue = xmlUtils::bGetBooleanValue((char*)key);
                    m_eFileMode = OVERWRITE_MODE;
                    if(bValue == TRUE)
                    {
                        m_eFileMode = APPEND_MODE;
                    }
                    xmlFree(key);
                }
            }


            else if ((!xmlStrcmp((const xmlChar*)pNodePtr->name, (const xmlChar*)"Reset_Absolute_Time")))
            {
                xmlChar* key = xmlNodeListGetString(pNodePtr->doc, pNodePtr->xmlChildrenNode, 1);
                if(NULL != key)
                {
                    m_bResetAbsTimeStamp = xmlUtils::bGetBooleanValue((char*)key);
                    xmlFree(key);
                }
            }



            else if ((!xmlStrcmp((const xmlChar*)pNodePtr->name, (const xmlChar*)"Channel")))
            {
                xmlChar* key = xmlNodeListGetString(pNodePtr->doc, pNodePtr->xmlChildrenNode, 1);
                if(NULL != key)
                {
                    m_ChannelSelected = atoi((char*)key);
                    if(m_ChannelSelected < 0 || m_ChannelSelected > CHANNEL_ALLOWED )
                    {
                        m_ChannelSelected = CAN_CHANNEL_ALL;
                    }
                    xmlFree(key);
                }
            }

            else if ((!xmlStrcmp((const xmlChar*)pNodePtr->name, (const xmlChar*)"File_Path")))
            {
                xmlChar* key = xmlNodeListGetString(pNodePtr->doc, pNodePtr->xmlChildrenNode, 1);
                if(NULL != key)
                {
                    if(PathIsRelative((char*)key) == TRUE)
                    {
                        string omStrConfigFolder;
                        string omPath;
                        char configPath[MAX_PATH];
                        AfxGetMainWnd()->SendMessage(MSG_GET_CONFIGPATH, (WPARAM)configPath, 0);
                        CUtilFunctions::nGetBaseFolder(configPath, omStrConfigFolder );
                        char chAbsPath[MAX_PATH];
                        PathCombine(chAbsPath, omStrConfigFolder.c_str(), (char*)key);
                        strcpy_s(m_sLogFileName, MAX_PATH, chAbsPath);
                    }
                    else
                    {
                        strcpy_s(m_sLogFileName, _MAX_PATH, (char*)key);
                    }
                    xmlFree(key);
                    nRetValue = S_OK;
                }
                else
                {
                    nRetValue = S_FALSE;
                }
            }

            else if ((!xmlStrcmp((const xmlChar*)pNodePtr->name, (const xmlChar*)"Trigger_Start_ID")))
            {
                xmlChar* key = xmlNodeListGetString(pNodePtr->doc, pNodePtr->xmlChildrenNode, 1);
                if(NULL != key)
                {
                    m_sLogTrigger.m_unStartID = atoi((char*)key);
                    xmlFree(key);
                }
            }
            else if ((!xmlStrcmp((const xmlChar*)pNodePtr->name, (const xmlChar*)"Trigger_Stop_ID")))
            {
                xmlChar* key = xmlNodeListGetString(pNodePtr->doc, pNodePtr->xmlChildrenNode, 1);
                if(NULL != key)
                {
                    m_sLogTrigger.m_unStopID = atoi((char*)key);
                    xmlFree(key);
                }
            }
        }//if Conditions
        pNodePtr = pNodePtr->next;
    }//While
    if (strlen(m_sLogFileName) == 0)
    {
        nRetValue = S_FALSE;
    }
    //0 - None, 1 - Start, 2 - Stop, 3 - Both
    m_sLogTrigger.m_unTriggerType = NONE;
    int nStartID = m_sLogTrigger.m_unStartID;
    int nStopID = m_sLogTrigger.m_unStopID;

    if( nStartID >= 0 && nStopID < 0 )
    {
        m_sLogTrigger.m_unTriggerType = START;
    }
    else if( nStartID < 0 && nStopID >= 0 )
    {
        m_sLogTrigger.m_unTriggerType = STOP;
    }
    else if( nStartID < 0 && nStopID < 0 )
    {
        m_sLogTrigger.m_unTriggerType = NONE;
    }
    else if( nStartID >= 0 && nStopID >= 0 )
    {
        m_sLogTrigger.m_unTriggerType = BOTH;
    }

    return nRetValue;
}
//~MVN
/* tagLogInfo --- ENDS */
