
/******************************************************************************
  Project       :  Auto-SAT_Tools
  FileName      :  BaseFrameProcessor_LIN.h
  Description   :  Definition file for CBaseFrameProcessor_LIN class which
                   describes the interface of LIN logging module.
  $Log:   X:/Archive/Sources/FrameProcessor/BaseFrameProcessor_LIN.h_v  $
 *
 *    Rev 1.3   02 Dec 2011 20:16:48   rac2kor
 * Removed hard coding of version information
 * string in the log file by accepting it as a parameter
 * from the application / client.
 *
 *    Rev 1.2   15 Apr 2011 19:20:30   rac2kor
 * Inserted RBEI Copyright information text into the file header.
 *
 *    Rev 1.1   11 Dec 2010 15:43:30   rac2kor
 * Wrong constructor name was used.
 *
 *    Rev 1.0   06 Dec 2010 18:53:02   rac2kor
 *

  Author(s)     :  Ratnadip Choudhury
  Date Created  :  1.12.2010
  Modified By   :
  Copyright (c) 2011, Robert Bosch Engineering and Business Solutions.  All rights reserved.
******************************************************************************/

#pragma once

#include "DataTypes/MsgBufAll_DataTypes.h"
#include "DataTypes/MsgBufVSE.h"
#include "DataTypes/Log_DataTypes.h"
#include "DataTypes/Filter_DataTypes.h"
#include "DataTypes/Base_WrapperErrorLogger.h"
//#include "DataTypes/LIN_DataTypes.h"
#include "Logger_CommonDataTypes.h"

typedef struct tagLINPROC_PARAMS
{
    char m_acVersion[MAX_PATH];        // Version info of the application suite
    CBaseLINBufFSE* m_pouLINBuffer;     // Client frame buffer to update
    Base_WrapperErrorLogger* m_pILog;   // Error logger module
    //CNetworkStats* m_pouNetworkStat;    // Network statistics object
    DWORD dwClientID;
    tagLINPROC_PARAMS()
    {
        m_pouLINBuffer = NULL;
        m_pILog = NULL;
        dwClientID = 0;
        for (int i = 0; i < MAX_PATH; i++)
        {
            m_acVersion[i] = L'\0';
        }
    }
} SLINPROC_PARAMS;
class CParamLoggerLIN : public CParamLoggerBUS
{
public:
    CParamLoggerLIN()
    {
    };
};

class CBaseFrameProcessor_LIN
{
public:

    CBaseFrameProcessor_LIN()
    {
    };

    ~CBaseFrameProcessor_LIN()
    {
    };

    // To initialise this module
    virtual HRESULT FPL_DoInitialisation(SLINPROC_PARAMS* psInitParams) = 0;

    /* Call to enable/disable logging for a particular block. Having ushBlk equal
    to FOR_ALL, signifies the operation to be performed for all the blocks */
    virtual HRESULT FPL_EnableLoggingBlock(USHORT ushBlk, BOOL bEnable) = 0;

    // To enable/disable logging
    virtual HRESULT FPL_EnableLogging(BOOL bEnable) = 0;

    /* Call to enable/disable logging for a particular block. Having ushBlk equal
    to FOR_ALL, signifies the operation to be performed for all the blocks */
    virtual HRESULT FPL_EnableFilter(USHORT ushBlk, BOOL bEnable) = 0;

    // Query function - current logging status (OFF/ON).
    virtual BOOL FPL_IsLoggingON(void) = 0;

    // PTV [1.6.4]
    virtual BOOL FPL_IsLINDataLogged(void) = 0;

    virtual void FPL_DisableLINDataLogFlag(void) = 0;

    virtual BOOL FPL_IsLINThreadBlocked(void) = 0;

    virtual void FPL_vCloseLogFile(void) = 0;

    // PTV [1.6.4]
    // Query function - current filtering status
    virtual BOOL FPL_IsFilterON(void) = 0;

    // To log a string
    virtual HRESULT FPL_LogString(CString& omStr) = 0;

    // To add a logging block; must be in editing mode
    virtual HRESULT FPL_AddLoggingBlock(const SLOGINFO& sLogObject) = 0;

    // To remove a logging block by its index in the list; editing mode prerequisite
    virtual HRESULT FPL_RemoveLoggingBlock(USHORT ushBlk) = 0;

    // Getter for total number of logging blocks
    virtual USHORT FPL_GetLoggingBlockCount(void) = 0;

    // To clear the logging block list
    virtual HRESULT FPL_ClearLoggingBlockList(void) = 0;

    // Getter for a logging block by specifying its index in the list
    virtual HRESULT FPL_GetLoggingBlock(USHORT ushBlk, SLOGINFO& sLogObject) = 0;

    // Setter for a logging block by specifying its index in the list
    virtual HRESULT FPL_SetLoggingBlock(USHORT ushBlk,
                                        const SLOGINFO& sLogObject) = 0;

    // To modify the filtering scheme of a logging block
    virtual HRESULT FPL_ApplyFilteringScheme(USHORT ushLogBlkID,
            const SFILTERAPPLIED_LIN& sFilterObj) = 0;

    // Getter for the filtering scheme of a logging block
    virtual HRESULT FPL_GetFilteringScheme(USHORT ushLogBlk,
                                           SFILTERAPPLIED_LIN& sFilterObj) = 0;

    // Getter for the logging configuration data
    virtual HRESULT FPL_GetConfigData(BYTE** ppvConfigData, UINT& unLength) = 0;

    virtual HRESULT FPL_GetConfigData(xmlNodePtr pNodePtr) = 0;
    // Setter for the logging configuration data
    virtual HRESULT FPL_SetConfigData(BYTE* pvDataStream, const CString& omStrVersion) = 0;
    virtual HRESULT FPL_SetConfigData(xmlDocPtr pDoc) = 0;

    // To reset or revoke the modifications made
    virtual HRESULT FPL_Reset(void) = 0;

    // To confirm the modifications made
    virtual HRESULT FPL_Confirm(void) = 0;

    // To start logging block editing session
    virtual HRESULT FPL_StartEditingSession(void) = 0;

    // To stop logging block editing session
    virtual HRESULT FPL_StopEditingSession(BOOL bConfirm) = 0;

    // To update the associated database list to logger
    virtual HRESULT FPL_SetDatabaseFiles(const CStringArray& omList) = 0;

    // To update the channel baud rate info to logger
    virtual HRESULT FPL_SetChannelBaudRateDetails(SCONTROLLER_DETAILS* controllerDetails,
            int nNumChannels) = 0;

};
