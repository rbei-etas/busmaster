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
    m_sLogAdvStngs.m_bIsLogOnMesurement = FALSE;
    m_sLogAdvStngs.m_bIsLogOnSize = FALSE;
    m_sLogAdvStngs.m_bIsLogOnTime = FALSE;
    m_sLogAdvStngs.m_omSizeInMB = "50";
    m_sLogAdvStngs.m_omLogTimeHrs = "0";
    m_sLogAdvStngs.m_omLogTimeMins = "30";
    m_sLogAdvStngs.m_omMaxNoOfLogFiles = "10";
    m_sLogAdvStngs.m_omLogComment = "";
    m_sLogAdvStngs.m_nConnectionCount = -1;
}

bool tagLogInfo::pbGetConfigData(xmlNodePtr pxmlNodePtr) const
{
    const char* omstrIsLogEnabled = "";
    omstrIsLogEnabled = m_bEnabled ? "TRUE" : "FALSE";

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

    const char* omstrAbsMode = "";
    omstrAbsMode = m_bResetAbsTimeStamp ? "TRUE" : "FALSE";

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
    xmlNodePtr pLogEnbldPtr = xmlNewChild(pxmlNodePtr, nullptr, BAD_CAST DEF_IsEnabled, BAD_CAST omstrIsLogEnabled);
    xmlAddChild(pxmlNodePtr, pLogEnbldPtr);

    xmlNodePtr pTimerMdePtr = xmlNewChild(pxmlNodePtr, nullptr, BAD_CAST DEF_TIME_MODE, BAD_CAST omstrTimerMode);
    xmlAddChild(pxmlNodePtr, pTimerMdePtr);

    xmlNodePtr pNumMdPtr = xmlNewChild(pxmlNodePtr, nullptr, BAD_CAST DEF_NUMERIC_MODE, BAD_CAST omstrNumMode);
    xmlAddChild(pxmlNodePtr, pNumMdPtr);

    xmlNodePtr pAppEnbldPtr = xmlNewChild(pxmlNodePtr, nullptr, BAD_CAST DEF_IS_APPEND_ENABLED, BAD_CAST omstrFileMode);
    xmlAddChild(pxmlNodePtr, pAppEnbldPtr);

    xmlNodePtr pAbsMdPtr = xmlNewChild(pxmlNodePtr, nullptr, BAD_CAST DEF_RESET_ABS_TIME, BAD_CAST omstrAbsMode);
    xmlAddChild(pxmlNodePtr, pAbsMdPtr);

    xmlNodePtr pChnlPtr = xmlNewChild(pxmlNodePtr, nullptr, BAD_CAST DEF_CHANNEL, BAD_CAST omChannel);
    xmlAddChild(pxmlNodePtr, pChnlPtr);

    std::string omPath;
    char configPath[MAX_PATH]= {0};
    std::string omStrConfigFolder;
    AfxGetMainWnd()->SendMessage(MSG_GET_CONFIGPATH, (WPARAM)configPath, 0);
    CUtilFunctions::nGetBaseFolder(configPath, omStrConfigFolder );
    CUtilFunctions::MakeRelativePath(omStrConfigFolder.c_str(), (char*)m_sLogFileName, omPath);
    xmlNodePtr pLogFilePtr = xmlNewChild(pxmlNodePtr, nullptr, BAD_CAST DEF_FILE_PATH_XML, BAD_CAST omPath.c_str());
    xmlAddChild(pxmlNodePtr, pLogFilePtr);

    xmlNodePtr pStrtIdPtr = xmlNewChild(pxmlNodePtr, nullptr, BAD_CAST DEF_TRGR_STRT_ID, BAD_CAST omcStartId);
    xmlAddChild(pxmlNodePtr, pStrtIdPtr);

    xmlNodePtr pStpIdPtr = xmlNewChild(pxmlNodePtr, nullptr, BAD_CAST DEF_TRGR_STP_ID, BAD_CAST omcStpId);
    xmlAddChild(pxmlNodePtr, pStpIdPtr);

    const char* omstrIsLogOnTransmission ="FALSE";
    const char* omstrIsLogOnSize ="FALSE";
    const char* omstrIsLogOnTime ="FALSE";
    const char* omstrLogTime ="FALSE";


    if(m_sLogAdvStngs.m_bIsLogOnMesurement)
    {
        omstrIsLogOnTransmission = "TRUE";
    }

    if(m_sLogAdvStngs.m_bIsLogOnSize)
    {
        omstrIsLogOnSize = "TRUE";
    }

    if(m_sLogAdvStngs.m_bIsLogOnTime)
    {
        omstrIsLogOnTime = "TRUE";
    }
    CString LogOnSize = m_sLogAdvStngs.m_omSizeInMB;

    CString LogTime = m_sLogAdvStngs.m_omLogTimeHrs + "." + m_sLogAdvStngs.m_omLogTimeMins;
    CString LogFilesAllowed = m_sLogAdvStngs.m_omMaxNoOfLogFiles;
    CString LogComments = m_sLogAdvStngs.m_omLogComment;
    xmlNodePtr pLogPtr = xmlNewChild(pxmlNodePtr, NULL, BAD_CAST DEF_IS_LOGON_TRANS, BAD_CAST omstrIsLogOnTransmission);
    xmlAddChild(pxmlNodePtr, pLogPtr);

    pLogPtr = xmlNewChild(pxmlNodePtr, NULL, BAD_CAST DEF_IS_LOGON_SIZE, BAD_CAST omstrIsLogOnSize);
    xmlAddChild(pxmlNodePtr, pLogPtr);

    pLogPtr = xmlNewChild(pxmlNodePtr, NULL, BAD_CAST DEF_LOGON_SIZE, BAD_CAST LogOnSize.GetBuffer(LogOnSize.GetLength()));
    xmlAddChild(pxmlNodePtr, pLogPtr);

    pLogPtr = xmlNewChild(pxmlNodePtr, NULL, BAD_CAST DEF_IS_LOGON_TIME, BAD_CAST omstrIsLogOnTime);
    xmlAddChild(pxmlNodePtr, pLogPtr);

    pLogPtr = xmlNewChild(pxmlNodePtr, NULL, BAD_CAST DEF_LOGON_TIME, BAD_CAST LogTime.GetBuffer(LogTime.GetLength()));
    xmlAddChild(pxmlNodePtr, pLogPtr);

    pLogPtr = xmlNewChild(pxmlNodePtr, NULL, BAD_CAST DEF_FILES_ALLOWED, BAD_CAST LogFilesAllowed.GetBuffer(LogFilesAllowed.GetLength()));
    xmlAddChild(pxmlNodePtr, pLogPtr);

    pLogPtr = xmlNewChild(pxmlNodePtr, NULL, BAD_CAST DEF_COMMENTS, BAD_CAST LogComments.GetBuffer(LogComments.GetLength()));
    xmlAddChild(pxmlNodePtr, pLogPtr);

    return true;
}

INT tagLogInfo::nSetConfigData(xmlNodePtr pNodePtr)
{
    //pNodePtr = pNodePtr->xmlChildrenNode;
    vClear();   //To give Initial Values
    INT nRetValue = S_OK;
    m_sLogTrigger.m_unStartID = -1;
    m_sLogTrigger.m_unStopID = -1;

    while (pNodePtr != nullptr)
    {
        if ( pNodePtr->xmlChildrenNode != nullptr )
        {
            if ((!xmlStrcmp((const xmlChar*)pNodePtr->name, (const xmlChar*)"IsEnabled")))
            {
                xmlChar* key = xmlNodeListGetString(pNodePtr->doc, pNodePtr->xmlChildrenNode, 1);
                if(nullptr != key)
                {
                    m_bEnabled = xmlUtils::bGetBooleanValue((char*)key);
                    xmlFree(key);
                }
            }
            else if ((!xmlStrcmp((const xmlChar*)pNodePtr->name, (const xmlChar*)"Time_Mode")))
            {
                xmlChar* key = xmlNodeListGetString(pNodePtr->doc, pNodePtr->xmlChildrenNode, 1);
                if(nullptr != key)
                {
                    m_eLogTimerMode =  xmlUtils::eGetTimerMode((char*)key);
                    xmlFree(key);
                }
            }
            else if ((!xmlStrcmp((const xmlChar*)pNodePtr->name, (const xmlChar*)"Numeric_Mode")))
            {
                xmlChar* key = xmlNodeListGetString(pNodePtr->doc, pNodePtr->xmlChildrenNode, 1);
                if(nullptr != key)
                {
                    m_eNumFormat = xmlUtils::eGetNumericMode((char*)key);
                    xmlFree(key);
                }
            }

            else if ((!xmlStrcmp((const xmlChar*)pNodePtr->name, (const xmlChar*)"IsAppendLog_Enabled")))
            {
                xmlChar* key = xmlNodeListGetString(pNodePtr->doc, pNodePtr->xmlChildrenNode, 1);
                if(nullptr != key)
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
                if(nullptr != key)
                {
                    m_bResetAbsTimeStamp = xmlUtils::bGetBooleanValue((char*)key);
                    xmlFree(key);
                }
            }



            else if ((!xmlStrcmp((const xmlChar*)pNodePtr->name, (const xmlChar*)"Channel")))
            {
                xmlChar* key = xmlNodeListGetString(pNodePtr->doc, pNodePtr->xmlChildrenNode, 1);
                if(nullptr != key)
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
                if(nullptr != key)
                {
                    if(PathIsRelative((char*)key) == TRUE)
                    {
                        std::string omStrConfigFolder;
                        std::string omPath;
                        char configPath[MAX_PATH]= {0};
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
                if(nullptr != key)
                {
                    m_sLogTrigger.m_unStartID = atoi((char*)key);
                    xmlFree(key);
                }
            }
            else if ((!xmlStrcmp((const xmlChar*)pNodePtr->name, (const xmlChar*)"Trigger_Stop_ID")))
            {
                xmlChar* key = xmlNodeListGetString(pNodePtr->doc, pNodePtr->xmlChildrenNode, 1);
                if(nullptr != key)
                {
                    m_sLogTrigger.m_unStopID = atoi((char*)key);
                    xmlFree(key);
                }
            }
            //Get XML Node value for LogOnTransmission bool and update the GUI.
            else if ((!xmlStrcmp((const xmlChar*)pNodePtr->name, (const xmlChar*)"IsLogOnTransmission")))
            {
                xmlChar* key = xmlNodeListGetString(pNodePtr->doc, pNodePtr->xmlChildrenNode, 1);
                if(NULL != key)
                {
                    m_sLogAdvStngs.m_bIsLogOnMesurement = xmlUtils::bGetBooleanValue((char*)key);
                    xmlFree(key);
                }
            }
            //Get XML Node value for LogOnSize bool and update the GUI.
            else if ((!xmlStrcmp((const xmlChar*)pNodePtr->name, (const xmlChar*)"IsLogOnSize")))
            {
                xmlChar* key = xmlNodeListGetString(pNodePtr->doc, pNodePtr->xmlChildrenNode, 1);
                if(NULL != key)
                {
                    m_sLogAdvStngs.m_bIsLogOnSize = xmlUtils::bGetBooleanValue((char*)key);
                    xmlFree(key);
                }
            }

            //Get XML Node value for LogOnSize MB value and update the GUI.
            else if ((!xmlStrcmp((const xmlChar*)pNodePtr->name, (const xmlChar*)"LogOnSize")))
            {
                xmlChar* key = xmlNodeListGetString(pNodePtr->doc, pNodePtr->xmlChildrenNode,1);
                if(NULL != key)
                {
                    CString stemp((char*)key);
                    m_sLogAdvStngs.m_omSizeInMB =  stemp;
                    xmlFree(key);
                }
            }

            //Get XML Node value for LogOnTime bool and update the GUI.
            else if ((!xmlStrcmp((const xmlChar*)pNodePtr->name, (const xmlChar*)"IsLogOnTime")))
            {
                xmlChar* key = xmlNodeListGetString(pNodePtr->doc, pNodePtr->xmlChildrenNode, 1);
                if(NULL != key)
                {
                    m_sLogAdvStngs.m_bIsLogOnTime = xmlUtils::bGetBooleanValue((char*)key);
                    xmlFree(key);
                }
            }

            else if ((!xmlStrcmp((const xmlChar*)pNodePtr->name, (const xmlChar*)"LogOnTime")))
            {
                xmlChar* key = xmlNodeListGetString(pNodePtr->doc, pNodePtr->xmlChildrenNode, 1);
                if(NULL != key)
                {
                    CString stemp((char*)key);
                    char* Hrs= strtok(stemp.GetBuffer(stemp.GetLength()) ,".");
                    char* Min= strtok(NULL ,".");

                    CString sHrs(Hrs);
                    CString sMin(Min);
                    m_sLogAdvStngs.m_omLogTimeHrs = sHrs;
                    m_sLogAdvStngs.m_omLogTimeMins = sMin;

                    //m_sLogAdvStngs.m_omSizeInMB =  stemp.;
                    //strcpy_s(m_sLogAdvStngs.m_omSizeInMB ,temp.GetLength(),temp);
                    xmlFree(key);
                }
            }

            //Get XML Node value for LogOnSize MB value and update the GUI.
            else if ((!xmlStrcmp((const xmlChar*)pNodePtr->name, (const xmlChar*)"FilesAllowed")))
            {
                xmlChar* key = xmlNodeListGetString(pNodePtr->doc, pNodePtr->xmlChildrenNode,1);
                if(NULL != key)
                {
                    CString stemp((char*)key);
                    m_sLogAdvStngs.m_omMaxNoOfLogFiles =  stemp;
                    xmlFree(key);
                }
            }

            //Get XML Node value for LogOnSize MB value and update the GUI.
            else if ((!xmlStrcmp((const xmlChar*)pNodePtr->name, (const xmlChar*)"Comments")))
            {
                xmlChar* key = xmlNodeListGetString(pNodePtr->doc, pNodePtr->xmlChildrenNode,1);
                if(NULL != key)
                {
                    CString stemp((char*)key);
                    m_sLogAdvStngs.m_omLogComment =  stemp;
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
