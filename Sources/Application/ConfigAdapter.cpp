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
 * \file      ConfigAdapter.cpp
 * \author    Pradeep Kadoor
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 */
#include "stdafx.h"
#include "Hashdefines.h"
#include "Struct.h"
#include "SectionNames.h"
#include "Datatypes/DIL_Datatypes.h"
#include "ConfigDetails.h"
#include "ConfigAdapter.h"


CConfigAdapter::CConfigAdapter()
{

}


CConfigAdapter::~CConfigAdapter()
{

}
INT CConfigAdapter::nLoadConfigFile(CString omFileName)
{
    return m_ouConfigDetails.nLoadConfiguration(omFileName);
}
static void vPopulateLogInfo(SLOGINFO& Dest, const SLOGFILEDETS& Src)
{
    Dest.m_bEnabled = Src.m_bEnabled;
    Dest.m_bIsUpdated = FALSE;
    switch (Src.m_nFileMode)
    {
        case 0:
        {
            Dest.m_eFileMode = APPEND_MODE;
        }
        break;

        case 1:
        {
            Dest.m_eFileMode = OVERWRITE_MODE;
        }
        break;

        default:
        {
            ASSERT(FALSE);
        }
        break;
    }
    
    switch (Src.m_nTimeMode)
    {
        case 0:
        {
            Dest.m_eLogTimerMode = TIME_MODE_RELATIVE;
        }
        break;

        case 1:
        {
            Dest.m_eLogTimerMode = TIME_MODE_SYSTEM;
        }
        break;
        case 2:
        {
            Dest.m_eLogTimerMode = TIME_MODE_ABSOLUTE;
        }
        break;
        default:
        {
            ASSERT(FALSE);
        }
        break;
    }
    
    switch (Src.m_nNumMode)
    {
        case 0:
        {
            Dest.m_eNumFormat = HEXADECIMAL;
        }
        break;

        case 1:
        {
            Dest.m_eNumFormat = DEC;
        }
        break;

        default:
        {
            ASSERT(FALSE);
        }
        break;
    }
    strcpy_s(Dest.m_sLogFileName, Src.m_omStrFileName);
    Dest.m_sLogTrigger = Src.m_sLogTrigger;
}

static BYTE* pbGetReplayFileConfig(SREPLAYFILE& sReplayFile, BYTE* pbTarget)
{
    BYTE* pbTemp = pbTarget;
    COPY_DATA(pbTemp, &(sReplayFile.m_bEnabled), sizeof(sReplayFile.m_bEnabled));
    COPY_DATA(pbTemp, &(sReplayFile.m_bInteractive), sizeof(sReplayFile.m_bInteractive));
    COPY_DATA(pbTemp, &(sReplayFile.m_nReplayMode), sizeof(sReplayFile.m_nReplayMode));
    COPY_DATA(pbTemp, &(sReplayFile.m_nTimeMode), sizeof(sReplayFile.m_nTimeMode));
    COPY_DATA(pbTemp, &(sReplayFile.m_unCycleTimeDelay), sizeof(sReplayFile.m_unCycleTimeDelay));
    COPY_DATA(pbTemp, &(sReplayFile.m_unMsgTimeDelay), sizeof(sReplayFile.m_unMsgTimeDelay));
    TCHAR acName[MAX_PATH] = {_T('\0')};
    strcpy_s(acName, sReplayFile.m_omStrFileName.GetBuffer(MAX_PATH));
    COPY_DATA(pbTemp, acName, sizeof(TCHAR) * MAX_PATH);

    return pbTemp;
}

static UINT unGetReplayFileSize(SREPLAYFILE& sReplayFile)
{
    UINT nSize = 0;
    nSize += sizeof (sReplayFile.m_bEnabled);
    nSize += sizeof (sReplayFile.m_bInteractive);
    nSize += sizeof (sReplayFile.m_nReplayMode);
    nSize += sizeof (sReplayFile.m_nTimeMode);
    nSize += sizeof (sReplayFile.m_unCycleTimeDelay);
    nSize += sizeof (sReplayFile.m_unMsgTimeDelay);
    nSize += (sizeof(TCHAR) * MAX_PATH);
    return nSize;
}
static UINT unGetFilterSize(CModuleFilterArray* pouModuleFilterArray, SFILTERAPPLIED_CAN* psFilterConfigured)
{
    UINT unSize = sizeof(BYTE);//Configuration version
    unSize += sizeof (BOOL); //Enable or not
    unSize += sizeof (USHORT);// Total filter set
    if ((pouModuleFilterArray != NULL) && (psFilterConfigured != NULL))
    {
		for (INT_PTR i = 0; i < pouModuleFilterArray->GetSize(); i++)
        {
            SMODULEFILTER& sModuleFilter = pouModuleFilterArray->GetAt(i);
            SFILTERSET* psFilterSet = 
                SFILTERSET::psGetFilterSetPointer(psFilterConfigured->m_psFilters,
                                                    psFilterConfigured->m_ushTotal,
                                                    sModuleFilter.m_omFilterName);
            if (psFilterSet != NULL)
            {
                unSize += psFilterSet->unGetSize();
            }  
        }
    }
    return unSize;
}
BYTE* pbyGetFilterConfig(BYTE* pbyTrgBuffer,
                         CModuleFilterArray* pouModuleFilterArray, 
                         const SFILTERAPPLIED_CAN* SrcFilter)
{
    BYTE* pbyTemp = pbyTrgBuffer;
    USHORT ushFilterCount = 0;
    if (pouModuleFilterArray != NULL && (SrcFilter != NULL))
    {
		ushFilterCount = (USHORT)pouModuleFilterArray->GetSize();
    }

    SFILTERAPPLIED_CAN sFilterApplied;
    sFilterApplied.vClear();
    sFilterApplied.m_bEnabled = TRUE;
    sFilterApplied.m_ushTotal = ushFilterCount;
    sFilterApplied.m_psFilters = new SFILTERSET[sFilterApplied.m_ushTotal];
    
    for (INT i = 0; i < ushFilterCount; i++)
    {
        //update filter applied
        SMODULEFILTER sModuleFilter = pouModuleFilterArray->GetAt(i);
        SFILTERSET* psFilterSet = 
            SFILTERSET::psGetFilterSetPointer(SrcFilter->m_psFilters,
                                                SrcFilter->m_ushTotal,
                                                sModuleFilter.m_omFilterName);
        if (psFilterSet != NULL)
        {
            sFilterApplied.m_psFilters[i].bClone(*psFilterSet);
        }                                                                
    }
    pbyTemp = sFilterApplied.pbGetConfigData(pbyTemp);

    sFilterApplied.vClear();

    return pbyTemp;
}
BOOL CConfigAdapter::bGetConfigData(BYTE*& lpData, int &nStreamLength, eSECTION_ID eSectionId)
{
    BOOL bReturn = TRUE;
    switch (eSectionId)
    {
        case MAINFRAME_SECTION_ID:
        {
            UINT unSize = 0;
            BYTE* pbyConfigData = NULL;
            //FIRST CALC SIZE REQUIRED
            unSize += sizeof(BYTE); //Configuration version
            unSize += (sizeof(TCHAR) * MAX_PATH);
            unSize += sizeof(STOOLBARINFO);

            //ALLOCATE THE MEMORY
            pbyConfigData = new BYTE[unSize];
            BYTE* pbyTemp = pbyConfigData;
                
            //UPDATE THE DATA
            BYTE byVersion = 0x1;
            COPY_DATA(pbyTemp, &byVersion, sizeof(BYTE));

            CString* pomMRU_C_FILE_NAME = NULL;
            m_ouConfigDetails.bGetData(MRU_C_FILE_NAME, (void **)(&pomMRU_C_FILE_NAME));
            TCHAR acName[MAX_PATH] = {_T('0')};
            ASSERT(pomMRU_C_FILE_NAME != NULL);
            strcpy_s(acName, pomMRU_C_FILE_NAME->GetBuffer(MAX_PATH));
            delete pomMRU_C_FILE_NAME;            
            COPY_DATA(pbyTemp, acName, (sizeof(TCHAR) * MAX_PATH));

            PSTOOLBARINFO psToolBarInfo = NULL;
            m_ouConfigDetails.bGetData(TOOLBAR_DETAILS, (void **)(&psToolBarInfo));
            ASSERT(psToolBarInfo != NULL);
            COPY_DATA(pbyTemp, psToolBarInfo, sizeof(STOOLBARINFO));
            delete psToolBarInfo;

            lpData = pbyConfigData;
            nStreamLength = unSize;
        }
        break;
        case LOG_SECTION_ID:
        {
            UINT unSize = 0;
            BYTE* pbyConfigData = NULL;
            SLOGCONFIGDETS* psLogConfigDets = NULL;

            m_ouConfigDetails.bGetData(LOG_CONFIG_DETS, (void **)(&psLogConfigDets));

            //FIRST CALCULATE SIZE REQUIRED
            unSize += sizeof(BYTE);//Configuration version
            if (psLogConfigDets != NULL)
            {
                
                //Filter info size
                SFILTERAPPLIED_CAN* psFilterConfigured = NULL;
                m_ouConfigDetails.bGetData(FILTER_CONFIG_DETS, (void **)(&psFilterConfigured));

                unSize += sizeof (USHORT); // Log file count
                for (UINT i = 0; i < psLogConfigDets->m_unCount; i++)
                {
                    //log info size
                    SLOGINFO sLogInfo;
                    unSize += sLogInfo.unGetSize();
                    
                    CModuleFilterArray* pModuleFilterArray = 
                                  (&(psLogConfigDets->m_asLogFileDets[i].m_omFilter));
                    unSize += unGetFilterSize(pModuleFilterArray, psFilterConfigured);                    
                }
                //CALCULATING SIZE ENDS

                //NOW UPDATE THE VALUES
                pbyConfigData = new BYTE[unSize];
                BYTE* pbyTemp = pbyConfigData;

                BYTE byVersion = 0x1;
                COPY_DATA(pbyTemp, &byVersion, sizeof(BYTE));

                memcpy(pbyTemp, &(psLogConfigDets->m_unCount), sizeof (USHORT));
                pbyTemp += sizeof (USHORT);

                for (UINT i = 0; i < psLogConfigDets->m_unCount; i++)
                {
                    //log info 
                    SLOGINFO sLogInfo;
                    vPopulateLogInfo(sLogInfo, psLogConfigDets->m_asLogFileDets[i]);
                    pbyTemp = sLogInfo.pbGetConfigData(pbyTemp);
                    //Filter info size
                    CModuleFilterArray* pomFilterArray = &(psLogConfigDets->m_asLogFileDets[i].m_omFilter);
                    pbyTemp = pbyGetFilterConfig(pbyTemp, pomFilterArray, psFilterConfigured);
                }
                //Now update the parameter out pointer
                lpData = pbyConfigData;
                nStreamLength = unSize;
            }           
        }
        break;
        case SIMSYS_SECTION_ID:
        {
            
            PSSIMSYSARRAY psSimSysArray;
            UINT unSize = 0;

            //FIRST CALCULATE THE SIZE REQUIRED MEANWHILE RETRIEVE THE DATA
            unSize += sizeof(BYTE);//Configuration version

            WINDOWPLACEMENT WndPlacement;
            //retrieve the window placement
            m_ouConfigDetails.bGetDefaultValue(SIMSYS_WND_PLACEMENT,WndPlacement);
            unSize += sizeof(WINDOWPLACEMENT);

            m_ouConfigDetails.bGetData(SIMSYS_LIST, (void **)(&psSimSysArray));
            PSSIMSYSARRAY psTemp = psSimSysArray;            
            UINT nCount = 0;

            unSize += sizeof(UINT); // To store count

            while (psTemp != NULL)
            {
                nCount++;
                unSize += (sizeof (TCHAR) * MAX_PATH);//File Path
                psTemp = psTemp->psNextSimsys;
            }
            
            unSize += sizeof (WINDOWPLACEMENT);// Window placement for out window
            //CALCULATE THE SIZE MEANWHILE RETRIEVE THE DATA ENDS

            // ALLOCATE THE MEMORY
            BYTE* pbyConfigData = new BYTE[unSize];
            BYTE* pbyTemp = pbyConfigData;

            //UPDATE THE DATA
            
            BYTE byVersion = 0x1;
            COPY_DATA(pbyTemp, &byVersion, sizeof(BYTE));
            //update window placement    
            COPY_DATA(pbyTemp, &WndPlacement, sizeof(WINDOWPLACEMENT));
            //Update Simsys file path
            psTemp = psSimSysArray;
            COPY_DATA(pbyTemp, &nCount, sizeof(UINT));
            while (psTemp != NULL)
            {
                TCHAR acFilePath[MAX_PATH] = {_T('\0')};
                strcpy_s(acFilePath, psTemp->m_omStrSimSysPath.GetBuffer(MAX_PATH));
                COPY_DATA(pbyTemp, acFilePath, (sizeof(TCHAR) * MAX_PATH));
                psTemp = psTemp->psNextSimsys;
            }

            //update out window placement
            WINDOWPLACEMENT* pWndPlacement = &WndPlacement;
            m_ouConfigDetails.bGetData(OUT_WND_PLACEMENT, (void **)(&pWndPlacement));
            COPY_DATA(pbyTemp, &WndPlacement, sizeof(WINDOWPLACEMENT));
            //UPDATE THE DATA ENDS
            
            //FINALLY UPDATE THE OUT PARAM
            lpData = pbyConfigData;
            nStreamLength = unSize;

        }
        break;
        case REPLAY_SECTION_ID:
        {
            UINT unSize = 0;
            BYTE* pbyConfigData = NULL;
            SREPLAYDETS* psRepConfigDets = NULL;

            unSize += sizeof(BYTE);//Configuration version
            m_ouConfigDetails.bGetData(REPLAY_CONFIG_DETS, (void **)(&psRepConfigDets));
            if (psRepConfigDets != NULL)
            {
                //FIRST CALCULATE SIZE REQUIRED
                SFILTERAPPLIED_CAN* psFilterConfigured = NULL;
                m_ouConfigDetails.bGetData(FILTER_CONFIG_DETS, (void **)(&psFilterConfigured));
                        
                unSize += sizeof (INT); // 
                for (UINT i = 0; i < psRepConfigDets->m_unCount; i++)
                {
                    unSize += unGetReplayFileSize(psRepConfigDets->m_asReplayFile[i]);
                    unSize += unGetFilterSize(&(psRepConfigDets->m_asReplayFile[i].m_omFilter),
                                                psFilterConfigured);
                }
                
                unSize += sizeof(WINDOWPLACEMENT);

                //CALCULATING SIZE ENDS

                //NOW UPDATE THE VALUES
                pbyConfigData = new BYTE[unSize];
                BYTE* pbyTemp = pbyConfigData;

                
                BYTE byVersion = 0x1;
                COPY_DATA(pbyTemp, &byVersion, sizeof(BYTE));

                memcpy(pbyTemp, &(psRepConfigDets->m_unCount), sizeof (INT));
                pbyTemp += sizeof (INT);

                for (UINT i = 0; i < psRepConfigDets->m_unCount; i++)
                {                    
                    pbyTemp = pbGetReplayFileConfig(psRepConfigDets->m_asReplayFile[i], pbyTemp);
                    //Filter info size
                    pbyTemp = pbyGetFilterConfig(pbyTemp, &(psRepConfigDets->m_asReplayFile[i].m_omFilter),
                                                    psFilterConfigured);
                }
                //Update window placement
                WINDOWPLACEMENT WndPlacement;
                WINDOWPLACEMENT* pWndPlacement = &WndPlacement;
                m_ouConfigDetails.bGetData(REPLAY_WND_PLACEMENT, (void **)(&pWndPlacement));
                memcpy(pbyTemp, &WndPlacement, sizeof(WINDOWPLACEMENT));
                pbyTemp += sizeof(WINDOWPLACEMENT);
                //Now update the parameter out pointer
                lpData = pbyConfigData;
                nStreamLength = unSize;
            }           
        }
        break;
        case MSGWND_SECTION_ID:
        {
            UINT unSize = 0;
            BYTE* pbyConfigData = NULL;


            //FIRST CALC SIZE
            unSize += sizeof(BYTE);//Configuration version

            PSMESSAGE_ATTRIB psMsgAttrib = NULL;
            m_ouConfigDetails.bGetData(MSG_ATTRIBUTES, (void **)(&psMsgAttrib));

            unSize += sizeof (UINT);// To store count of MsgAttribs

            if (psMsgAttrib != NULL)
            {                
                UINT nCount = psMsgAttrib->m_usMsgCount;
                         //Count             To store Msg Name         MsgId        Msg Color
                unSize += (nCount * ((sizeof (TCHAR) * MAX_PATH) + sizeof(UINT) + sizeof (COLORREF)));
            }

            INT anMsgBuffSize[defDISPLAY_CONFIG_PARAM] = {0};
            m_ouConfigDetails.bGetData(MSG_BUFFER_SIZE, (void **)(&anMsgBuffSize));
            unSize += (sizeof (INT) * defDISPLAY_CONFIG_PARAM);

            
            CModuleFilterArray ouModuleFilterArray;
            CModuleFilterArray* pouModuleFilterArray = &ouModuleFilterArray;
            m_ouConfigDetails.bGetData(MSG_DISPLAY_FILTER_DETAILS, (void **)(&pouModuleFilterArray));
            SFILTERAPPLIED_CAN* psFilterConfigured = NULL;
            m_ouConfigDetails.bGetData(FILTER_CONFIG_DETS, (void **)(&psFilterConfigured));
            //Filter size
            unSize += unGetFilterSize(&ouModuleFilterArray, psFilterConfigured);
            //MsgFormat window config data
            unSize += sizeof(BYTE);//Msg frmt windows version information
            unSize += sizeof(UINT);//buffer size
            //CALC SIZE ENDS
            
            //ALLOCATE THE MEMORY
            pbyConfigData = new BYTE[unSize];
            BYTE* pbyTemp = pbyConfigData;

            //UPDATE THE DATA
            
            BYTE byVersion = 0x1;
            COPY_DATA(pbyTemp, &byVersion, sizeof(BYTE));
            //Attributes
            if (psMsgAttrib != NULL)
            {
                COPY_DATA(pbyTemp, &(psMsgAttrib->m_usMsgCount), sizeof(UINT));
                for (INT i = 0; i < psMsgAttrib->m_usMsgCount; i++)
                {
                    TCHAR acName[MAX_PATH] = {_T('\0')};
                    strcpy_s(acName, psMsgAttrib->m_psMsgAttribDetails[i].omStrMsgname);
                    COPY_DATA(pbyTemp, acName, (sizeof(TCHAR) * MAX_PATH));
                    COPY_DATA(pbyTemp, &(psMsgAttrib->m_psMsgAttribDetails[i].unMsgID), sizeof(UINT));
                    COPY_DATA(pbyTemp, &(psMsgAttrib->m_psMsgAttribDetails[i].sColor), sizeof(COLORREF));                
                }                
            }
            m_ouConfigDetails.vRelease(MSG_ATTRIBUTES, (LPVOID*)(&psMsgAttrib));
            //Display buffer size
            COPY_DATA(pbyTemp, anMsgBuffSize, (sizeof (INT) * defDISPLAY_CONFIG_PARAM));

            //Filters
            pbyTemp = pbyGetFilterConfig(pbyTemp, &ouModuleFilterArray, psFilterConfigured);
            //Msg format window default values
            BYTE byVer = 0x0;//version information
            COPY_DATA(pbyTemp, &byVer, sizeof(BYTE));
            UINT unMsgWndCfgSize = 0;
            COPY_DATA(pbyTemp, &unMsgWndCfgSize, sizeof(UINT));

            //Update the out parameters
            lpData = pbyConfigData;
            nStreamLength = unSize;
        }       
        break;
        case SIGWATCH_SECTION_ID:
        {
            UINT unSize = 0;
            BYTE* pbyConfigData = NULL;
            PSSIGNALWATCHLIST psSigWatchList = NULL;
            m_ouConfigDetails.bGetData(SIGNAL_WATCH_LIST, (void **)(&psSigWatchList));
            //Find the Size
            unSize += sizeof(BYTE);//Configuration version
            UINT unMsgcount = 0;
            unSize += sizeof (UINT); // To store Msg Count
            PSSIGNALWATCHLIST psTemp = psSigWatchList;
            while (psTemp != NULL)
            {
                unMsgcount++;
                unSize += sizeof (UINT); // To store Msg ID
                unSize += (sizeof (TCHAR) * MAX_PATH);// To store Msg Name
                unSize += sizeof (UINT); //To store selected signal count
				INT nSelCount = psTemp->omCSASignals.GetSize();
                for (INT i = 0; i < nSelCount; i++)
                {
                    unSize += sizeof (UINT); // To store Signal ID
                    unSize += (sizeof (TCHAR) * MAX_PATH);// To store Signal Name
                }
                psTemp = psTemp->psNextMessageNode;
            }
            unSize += sizeof (WINDOWPLACEMENT); // window placement size
            //ALLOCATE THE MEMORY
            pbyConfigData = new  BYTE[unSize];
            BYTE* pbyTemp = pbyConfigData;

            //UPDATE THE DATA
            
            BYTE byVersion = 0x1;
            COPY_DATA(pbyTemp, &byVersion, sizeof(BYTE));

            //Find the Size
            COPY_DATA(pbyTemp, &unMsgcount, sizeof (UINT));

            psTemp = psSigWatchList;
            while (psTemp != NULL)
            {
                COPY_DATA(pbyTemp, &(psTemp->unMsgID), sizeof (UINT));
                TCHAR acName[MAX_PATH] = {_T('\0')};
                COPY_DATA(pbyTemp, acName, (sizeof (TCHAR) * MAX_PATH));

				UINT unSelCount = psTemp->omCSASignals.GetSize();

                COPY_DATA(pbyTemp, &unSelCount, sizeof (UINT));
                for (UINT i = 0; i < unSelCount; i++)
                {
                    CString omSigName = psTemp->omCSASignals.GetAt(i);
                    UINT unSigId = 0;
                    COPY_DATA(pbyTemp, &(unSigId), sizeof (UINT));
                    strcpy_s(acName, omSigName.GetBuffer(MAX_PATH));
                    COPY_DATA(pbyTemp, acName, (sizeof (TCHAR) * MAX_PATH));
                }
                psTemp = psTemp->psNextMessageNode;
            }

            //Signal Window placement
            WINDOWPLACEMENT wndPlacement;
            WINDOWPLACEMENT* pWndPlacement = &wndPlacement;
            m_ouConfigDetails.bGetData(SIGWATCH_WND_PLACEMENT, (void **)(&pWndPlacement));
            COPY_DATA(pbyTemp, &wndPlacement, sizeof (WINDOWPLACEMENT));

            //Update the out parameters
            lpData = pbyConfigData;
            nStreamLength = unSize;
        }
        break;
        case DIL_SECTION_ID:
        {
            PSCONTROLLER_DETAILS psContrlDets;
            BYTE* pbyConfigData = NULL;
            UINT unSize = 0;
            unSize += sizeof(BYTE);//Configuration version
            unSize += sizeof(DWORD);
            unSize += sizeof(BYTE);
            unSize += (sizeof(SCONTROLLER_DETAILS) * defNO_OF_CHANNELS);
            if (m_ouConfigDetails.bGetData(CONTROLLER_DETAILS, (void**)(&psContrlDets)))
            {
                pbyConfigData = new BYTE[unSize];
                BYTE* pbyTemp = pbyConfigData;

                
                BYTE byVersion = 0x1;
                COPY_DATA(pbyTemp, &byVersion, sizeof(BYTE));

                DWORD dwDriverId = DRIVER_CAN_STUB;
                COPY_DATA(pbyTemp, &dwDriverId, sizeof(DWORD));

                BYTE byContlrMode = defMODE_SIMULATE;
                COPY_DATA(pbyTemp, &byContlrMode, sizeof(BYTE));

                COPY_DATA(pbyTemp, psContrlDets, unSize);
                m_ouConfigDetails.vRelease(CONTROLLER_DETAILS, (LPVOID*)(&psContrlDets));
            }
            lpData = pbyConfigData;
            nStreamLength = unSize;
        }
        break;
        case GRAPH_SECTION_ID:
        {
			BYTE byVersion = 0x1;
            BYTE* pbyConfigData = NULL;
            UINT unSize = 0;
            CGraphList *podGraphList;
            m_ouConfigDetails.bGetData(SIGNAL_GRAPH_LIST, (void**)(&podGraphList));
            //FIRST CALCULATE THE SIZE
            unSize += sizeof(BYTE); //Configuration version
            if (podGraphList != NULL)
            {               
                unSize += podGraphList->unGetConfigSize(byVersion);
            }
            unSize += sizeof (WINDOWPLACEMENT);

            unSize += sizeof (SGRAPHSPLITTERDATA); //Splitter data

            //ALLOCATE THE MEMORY
            pbyConfigData = new BYTE[unSize];
            BYTE* pbyTemp = pbyConfigData;

            //UPDATE THE DATA            
            COPY_DATA(pbyTemp, &byVersion, sizeof(BYTE));

            if (podGraphList != NULL)
            {
                pbyTemp = podGraphList->pbyGetConfigData(pbyTemp, byVersion);
            }

            //UPDATE GRAPH WINDOW PLACEMENT
            WINDOWPLACEMENT wndPalcement;
            WINDOWPLACEMENT* pWndPalcement = &wndPalcement;
            m_ouConfigDetails.bGetData(GRAPH_WND_PLACEMENT, (void**)(&pWndPalcement));
            COPY_DATA(pbyTemp, &wndPalcement, sizeof (WINDOWPLACEMENT));

            SGRAPHSPLITTERDATA sGraphSplitterData;
            PSGRAPHSPLITTERDATA psGraphSplitterData = &sGraphSplitterData;
            m_ouConfigDetails.bGetData(GRAPH_WND_SPLITTER_DATA, (void**)(&psGraphSplitterData));
            COPY_DATA(pbyTemp, &sGraphSplitterData, sizeof (SGRAPHSPLITTERDATA));

            //UPDATE OUT PARAMS
            lpData = pbyConfigData;
            nStreamLength = unSize;
        }
        break;
        case TXWND_SECTION_ID:
        {
            BYTE* pbyCfgData = NULL;
            UINT unSize = 0;
            
            //FIRST CALCULATE THE SIZE
            unSize += sizeof(BYTE); //Configuration version

            UINT unBlockCount = 0;
            UINT* punBlockCount = &unBlockCount;
            m_ouConfigDetails.bGetData(MSG_BLOCK_COUNT, (void**)(&punBlockCount));
            unSize += sizeof (UINT); //block count

            PSMSGBLOCKLIST psMsgBlockList = NULL;
            if (m_ouConfigDetails.bGetData(SEND_MULTI_MSGS, (void**)(&psMsgBlockList)))
            {
                PSMSGBLOCKLIST psTemp = psMsgBlockList;
                while (psTemp != NULL)
                {                    
                    unSize += (sizeof(TCHAR) * MAX_PATH); // To store the block name
                    unSize += sizeof(UCHAR); // To store the trigger
                    unSize += sizeof(BOOL); // To store active or not
                    unSize += sizeof(UCHAR); // To store the key value
                    unSize += sizeof (UINT); // to store the timeinterval
                    unSize += sizeof (BOOL); // Type
                    unSize += sizeof (BOOL); // Send All msgs

                    unSize += sizeof(UINT);// To store no of msgs in each block

                    unSize += (psTemp->m_unMsgCount) * sizeof (STXCANMSGDETAILS);
                    psTemp = psTemp->m_psNextMsgBlocksList;
                }
            }
            unSize += sizeof(WINDOWPLACEMENT);
            unSize += sizeof(STXMSGSPLITTERDATA);

            //ALLOCATE THE MEMORY
            pbyCfgData = new BYTE[unSize];
            BYTE* pbyTemp = pbyCfgData;

            BYTE byVersion = 0x1;
            COPY_DATA(pbyTemp, &byVersion, sizeof(BYTE));

            COPY_DATA(pbyTemp, &unBlockCount, sizeof(UINT));

            PSMSGBLOCKLIST psTempBlock = psMsgBlockList;
            while (psTempBlock != NULL)
            {
            
                CString m_omStrBlockName;
                TCHAR acName[MAX_PATH] = {_T('\0')};
                strcpy_s(acName, psTempBlock->m_omStrBlockName.GetBuffer(MAX_PATH));
                COPY_DATA(pbyTemp, acName, (sizeof(TCHAR) * MAX_PATH));
                COPY_DATA(pbyTemp, &(psTempBlock->m_ucTrigger), sizeof(UCHAR));
                COPY_DATA(pbyTemp, &(psTempBlock->m_bActive), sizeof(BOOL));
                COPY_DATA(pbyTemp, &(psTempBlock->m_ucKeyValue), sizeof(UCHAR));
                COPY_DATA(pbyTemp, &(psTempBlock->m_unTimeInterval), sizeof(UINT));
                COPY_DATA(pbyTemp, &(psTempBlock->m_bType), sizeof(BOOL));
                COPY_DATA(pbyTemp, &(psTempBlock->m_bTxAllFrame), sizeof(BOOL));

                COPY_DATA(pbyTemp, &(psTempBlock->m_unMsgCount), sizeof(UINT));

                PSTXCANMSGLIST psTempCanList = psTempBlock->m_psTxCANMsgList;
                while (psTempCanList != NULL)
                {
                    COPY_DATA(pbyTemp, &(psTempCanList->m_sTxMsgDetails), sizeof (STXCANMSGDETAILS));
                    psTempCanList = psTempCanList->m_psNextMsgDetails;
                }
                psTempBlock = psTempBlock->m_psNextMsgBlocksList;
            }
            m_ouConfigDetails.vRelease(SEND_MULTI_MSGS, (LPVOID*)&psMsgBlockList);

            //Get the Tx window placement
            WINDOWPLACEMENT WndPlacement;
            WINDOWPLACEMENT* pWndPlacement = &WndPlacement;
            m_ouConfigDetails.bGetData(TX_WND_PLACEMENT, (void**)(&pWndPlacement));
            COPY_DATA(pbyTemp, &WndPlacement, sizeof(WINDOWPLACEMENT));

            //Get the Tx splitter position
            STXMSGSPLITTERDATA sTxSpliiterData;
            STXMSGSPLITTERDATA* psTxSpliiterData = &sTxSpliiterData;
            m_ouConfigDetails.bGetData(TX_MSG_WND_SPLITTER_DATA, (void**)(&psTxSpliiterData));
            COPY_DATA(pbyTemp, &sTxSpliiterData, sizeof(STXMSGSPLITTERDATA));
            //Update the OUT PARAMETERS
            lpData = pbyCfgData;
            nStreamLength = unSize;
        }
        break;
        case DATABASE_SECTION_ID:
        {
            BYTE* pbyCfgData = NULL;
            UINT unSize = 0;
            CStringArray* pomStrDBArray = NULL;
            m_ouConfigDetails.bGetData(DATABASE_FILE_NAME, (void**)(&pomStrDBArray));

            //FIRST CALC SIZE
            unSize += sizeof(BYTE); //Configuration version

            unSize += sizeof (UINT); // To store the count
            if (pomStrDBArray != NULL)
            {
                //To store the stringd
				unSize += (pomStrDBArray->GetSize()) * (sizeof(TCHAR) * MAX_PATH);
            }

            //ALLOCATE THE MEMORY
            pbyCfgData = new BYTE[unSize];
            BYTE* pbyTemp = pbyCfgData;

            //UPDATE THE DATA 
            BYTE byVersion = 0x1;
            COPY_DATA(pbyTemp, &byVersion, sizeof(BYTE));

            UINT unCount = 0;
            if (pomStrDBArray != NULL)
            {
				unCount = pomStrDBArray->GetSize();
            }
            COPY_DATA(pbyTemp, &unCount, sizeof(UINT));

            for (UINT i =0; i < unCount; i++)
            {
                CString omName = pomStrDBArray->GetAt(i);
                TCHAR acName[MAX_PATH] = {_T('\0')};
                strcpy_s(acName, omName.GetBuffer(MAX_PATH));
                COPY_DATA(pbyTemp, acName, (sizeof(TCHAR) * MAX_PATH));
            }
            m_ouConfigDetails.vRelease(DATABASE_FILE_NAME,(LPVOID*)&pomStrDBArray);
            
            //UPDATE THE OUT PARAMETER
            lpData = pbyCfgData;
            nStreamLength = unSize;
            
        }
        break;
        case FILTER_SECTION_ID:
        {
            BYTE* pbyCfgData = NULL;
            UINT unSize = 0;
            SFILTERAPPLIED_CAN* psFilterConfigured = NULL;
            m_ouConfigDetails.bGetData(FILTER_CONFIG_DETS, (void **)(&psFilterConfigured));

            if (psFilterConfigured != NULL)
            {   
                unSize += psFilterConfigured->unGetSize();
            }
            pbyCfgData = new BYTE[unSize];                              
            BYTE* pbyTemp = pbyCfgData;
        
            pbyTemp = psFilterConfigured->pbGetConfigData(pbyTemp);
            BYTE byVersion = 0x1;
            memcpy(pbyCfgData, &byVersion, sizeof(BYTE));
            lpData = pbyCfgData;
            nStreamLength = unSize;
        }
        break;
        default:
        {
            ASSERT(FALSE);
        }
        break;
        

    }
    return bReturn;
}