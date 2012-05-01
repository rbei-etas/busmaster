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
 * \file      BaseFrameProcessor_J1939.h
 * \brief     Definition file for CBaseFrameProcessor_J1939 class
 * \author    Ratnadip Choudhury
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Definition file for CBaseFrameProcessor_J1939 class which
 * describes the interface of J1939 logging module.
 */

#pragma once

#include "DataTypes/MsgBufAll_DataTypes.h"
#include "DataTypes/MsgBufVSE.h"
#include "DataTypes/Log_DataTypes.h"
#include "DataTypes/Filter_DataTypes.h"
#include "DataTypes/Base_WrapperErrorLogger.h"
#include "DataTypes/J1939_DataTypes.h"
#include "Logger_CommonDataTypes.h"


class CParamLoggerJ1939 : public CParamLoggerBUS
{
public:
    CParamLoggerJ1939()
    {
    };
};

class CBaseFrameProcessor_J1939
{
public:

    CBaseFrameProcessor_J1939()
    {
    };

    ~CBaseFrameProcessor_J1939()
    {
    };

    // To initialise this module
    virtual HRESULT FPJ1_DoInitialisation(CParamLoggerJ1939* psInitParams) = 0;

    /* Call to enable/disable logging for a particular block. Having ushBlk equal
    to FOR_ALL, signifies the operation to be performed for all the blocks */
    virtual HRESULT FPJ1_EnableLoggingBlock(USHORT ushBlk, BOOL bEnable) = 0;

    // To enable/disable logging
    virtual HRESULT FPJ1_EnableLogging(BOOL bEnable) = 0;

    /* Call to enable/disable logging for a particular block. Having ushBlk equal
    to FOR_ALL, signifies the operation to be performed for all the blocks */
    virtual HRESULT FPJ1_EnableFilter(USHORT ushBlk, BOOL bEnable) = 0;

    // Query function - current logging status (OFF/ON).
    virtual BOOL FPJ1_IsLoggingON(void) = 0;

    // Query function - current filtering status
    virtual BOOL FPJ1_IsFilterON(void) = 0;

    // To log a string
    virtual HRESULT FPJ1_LogString(CString& omStr) = 0;

    // To add a logging block; must be in editing mode
    virtual HRESULT FPJ1_AddLoggingBlock(const SLOGINFO& sLogObject) = 0;

    // To remove a logging block by its index in the list; editing mode prerequisite
    virtual HRESULT FPJ1_RemoveLoggingBlock(USHORT ushBlk) = 0;

    // Getter for total number of logging blocks
    virtual USHORT FPJ1_GetLoggingBlockCount(void) = 0;

    // To clear the logging block list
    virtual HRESULT FPJ1_ClearLoggingBlockList(void) = 0;

    // Getter for a logging block by specifying its index in the list
    virtual HRESULT FPJ1_GetLoggingBlock(USHORT ushBlk, SLOGINFO& sLogObject) = 0;

    // Setter for a logging block by specifying its index in the list
    virtual HRESULT FPJ1_SetLoggingBlock(USHORT ushBlk,
                                         const SLOGINFO& sLogObject) = 0;

    // To modify the filtering scheme of a logging block
    virtual HRESULT FPJ1_ApplyFilteringScheme(USHORT ushLogBlkID,
            const SFILTERAPPLIED_J1939& sFilterObj) = 0;

    // Getter for the filtering scheme of a logging block
    virtual HRESULT FPJ1_GetFilteringScheme(USHORT ushLogBlk,
                                            SFILTERAPPLIED_J1939& sFilterObj) = 0;

    // Getter for the logging configuration data
    virtual HRESULT FPJ1_GetConfigData(BYTE** ppvConfigData, UINT& unLength) = 0;

    // Setter for the logging configuration data
    virtual HRESULT FPJ1_SetConfigData(BYTE* pvDataStream) = 0;

    // To reset or revoke the modifications made
    virtual HRESULT FPJ1_Reset(void) = 0;

    // To confirm the modifications made
    virtual HRESULT FPJ1_Confirm(void) = 0;

    // To start logging block editing session
    virtual HRESULT FPJ1_StartEditingSession(void) = 0;

    // To stop logging block editing session
    virtual HRESULT FPJ1_StopEditingSession(BOOL bConfirm) = 0;

    // To update the associated database list to logger
    virtual HRESULT FPJ1_SetDatabaseFiles(const CStringArray& omList) = 0;

    // To update the channel baud rate info to logger
    virtual HRESULT FPJ1_SetChannelBaudRateDetails(SCONTROLLER_DETAILS* controllerDetails,
            int nNumChannels) = 0;

};
