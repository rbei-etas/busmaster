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
 * \file      BaseFrameProcessor_CAN.h
 * \brief     Definition file for CBaseFrameProcessor_CAN class which des-
 * \author    Ratnadip Choudhury
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Definition file for CBaseFrameProcessor_CAN class which des-
 */

#pragma once

#include "DataTypes/MsgBufAll_DataTypes.h"
#include "DataTypes/Log_DataTypes.h"
#include "DataTypes/Filter_DataTypes.h"
#include "DataTypes/Base_WrapperErrorLogger.h"
//#include "DataTypes/NetworkStats.h"

typedef struct tagCANPROC_PARAMS
{
    char m_acVersion[MAX_PATH];        // Version info of the application suite
    CBaseCANBufFSE* m_pouCANBuffer;     // Client frame buffer to update
    Base_WrapperErrorLogger* m_pILog;   // Error logger module
    //CNetworkStats* m_pouNetworkStat;    // Network statistics object
    DWORD dwClientID;
    tagCANPROC_PARAMS()
    {
        m_pouCANBuffer = NULL;
        m_pILog = NULL;
        dwClientID = 0;

        for (int i = 0; i < MAX_PATH; i++)
        {
            m_acVersion[i] = L'\0';
        }
    }
} SCANPROC_PARAMS;

class CBaseFrameProcessor_CAN
{
public:

    CBaseFrameProcessor_CAN()
    {
    };

    ~CBaseFrameProcessor_CAN()
    {
    };

    // To initialise this module
    virtual HRESULT FPC_DoInitialisation(SCANPROC_PARAMS* psInitParams) = 0;

    /* Call to enable/disable logging for a particular block. Having ushBlk equal
    to FOR_ALL, signifies the operation to be performed for all the blocks */
    virtual HRESULT FPC_EnableLoggingBlock(USHORT ushBlk, BOOL bEnable) = 0;

    // To enable/disable logging
    virtual HRESULT FPC_EnableLogging(BOOL bEnable) = 0;

    /* Call to enable/disable logging for a particular block. Having ushBlk equal
    to FOR_ALL, signifies the operation to be performed for all the blocks */
    virtual HRESULT FPC_EnableFilter(USHORT ushBlk, BOOL bEnable) = 0;

    // To enable/disable updation of the client flexray frame buffer.
    virtual HRESULT FPC_SetClientCANBufON(BOOL bEnable) = 0;

    // Query function - client flexray buffer updation status (OFF/ON)
    virtual BOOL FPC_IsClientCANBufON(void) = 0;

    // Query function - current logging status (OFF/ON).
    virtual BOOL FPC_IsLoggingON(void) = 0;

    // Query function - current filtering status
    virtual BOOL FPC_IsFilterON(void) = 0;

    // To log a string
    virtual HRESULT FPC_LogString(CString& omStr) = 0;

    // To get the flexray buffer of this module
    virtual CBaseCANBufFSE* FPC_GetCANBuffer(void) = 0;

    // To add a logging block; must be in editing mode
    virtual HRESULT FPC_AddLoggingBlock(const SLOGINFO& sLogObject) = 0;

    // To remove a logging block by its index in the list; editing mode prerequisite
    virtual HRESULT FPC_RemoveLoggingBlock(USHORT ushBlk) = 0;

    // Getter for total number of logging blocks
    virtual USHORT FPC_GetLoggingBlockCount(void) = 0;

    // To clear the logging block list
    virtual HRESULT FPC_ClearLoggingBlockList(void) = 0;

    // Getter for a logging block by specifying its index in the list
    virtual HRESULT FPC_GetLoggingBlock(USHORT ushBlk, SLOGINFO& sLogObject) = 0;

    // Setter for a logging block by specifying its index in the list
    virtual HRESULT FPC_SetLoggingBlock(USHORT ushBlk,
                                        const SLOGINFO& sLogObject) = 0;

    // To modify the filtering scheme of a logging block
    virtual HRESULT FPC_ApplyFilteringScheme(USHORT ushLogBlkID,
            const SFILTERAPPLIED_CAN& sFilterObj) = 0;

    // Getter for the filtering scheme of a logging block
    virtual HRESULT FPC_GetFilteringScheme(USHORT ushLogBlk,
                                           SFILTERAPPLIED_CAN& sFilterObj) = 0;

    // Getter for the logging configuration data
    virtual HRESULT FPC_GetConfigData(BYTE** ppvConfigData, UINT& unLength) = 0;

    // Setter for the logging configuration data
    virtual HRESULT FPC_SetConfigData(BYTE* pvDataStream, const CString& omStrVersion) = 0;

    // To reset or revoke the modifications made
    virtual HRESULT FPC_Reset(void) = 0;

    // To confirm the modifications made
    virtual HRESULT FPC_Confirm(void) = 0;

    // To start logging block editing session
    virtual HRESULT FPC_StartEditingSession(void) = 0;

    // To stop logging block editing session
    virtual HRESULT FPC_StopEditingSession(BOOL bConfirm) = 0;

    // To update the associated database list to logger
    virtual HRESULT FPC_SetDatabaseFiles(const CStringArray& omList) = 0;

    // To update the channel baud rate info to logger
    virtual HRESULT FPC_SetChannelBaudRateDetails(SCONTROLLER_DETAILS* controllerDetails,
            int nNumChannels) = 0;
};
