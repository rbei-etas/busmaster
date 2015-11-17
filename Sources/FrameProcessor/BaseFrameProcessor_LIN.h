
/******************************************************************************

FileName      :  BaseFrameProcessor_LIN.h
Description   :  Definition file for CBaseFrameProcessor_LIN class which
describes the interface of LIN logging module.

Author(s)     : Shashank Vernekar
Date Created  : 29.01.2014
Modified By   :
Copyright (c) 2011, Robert Bosch Engineering and Business Solutions.  All rights reserved.
******************************************************************************/

#pragma once

#include "DataTypes/MsgBufAll_DataTypes.h"
#include "DataTypes/MsgBufVSE.h"
#include "DataTypes/Log_DataTypes.h"
#include "DataTypes/Filter_DataTypes.h"
#include "DataTypes/Base_WrapperErrorLogger.h"
#include "Logger_CommonDataTypes.h"

typedef struct tagLINPROC_PARAMS
{
    char m_acVersion[MAX_PATH];        // Version info of the application suite
    CBaseLINBufFSE* m_pouLINBuffer;     // Client frame buffer to update
    Base_WrapperErrorLogger* m_pILog;   // Error logger module

    DWORD dwClientID;
    tagLINPROC_PARAMS()
    {
        m_pouLINBuffer = nullptr;
        m_pILog = nullptr;
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
    virtual HRESULT FPL_EnableLoggingBlock(USHORT ushBlk, bool bEnable) = 0;

    // To enable/disable logging
    virtual HRESULT FPL_EnableLogging(bool bEnable) = 0;

    /* Call to enable/disable logging for a particular block. Having ushBlk equal
    to FOR_ALL, signifies the operation to be performed for all the blocks */
    virtual HRESULT FPL_EnableFilter(USHORT ushBlk, bool bEnable) = 0;

    // Query function - current logging status (OFF/ON).
    virtual bool FPL_IsLoggingON(void) = 0;

    virtual bool FPL_IsLINDataLogged(void) = 0;

    virtual void FPL_DisableLINDataLogFlag(void) = 0;

    virtual bool FPL_IsLINThreadBlocked(void) = 0;

    virtual void FPL_vCloseLogFile(void) = 0;

    // Query function - current filtering status
    virtual bool FPL_IsFilterON(void) = 0;

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
    virtual HRESULT FPL_StopEditingSession(bool bConfirm) = 0;

    // To update the associated database list to logger
    virtual HRESULT FPL_SetDatabaseFiles(const CStringArray& omList) = 0;

    // To update the channel baud rate info to logger
    virtual HRESULT FPL_SetChannelBaudRateDetails(SCONTROLLER_DETAILS_LIN* controllerDetails, int nNumChannels,ETYPE_BUS eBus) = 0;

    virtual void vSetMeasurementFileName() = 0;
};
