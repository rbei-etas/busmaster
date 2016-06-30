
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

//#include "DataTypes/MsgBufAll_DataTypes.h"
#include "MsgBufVSE.h"
#include "LINDriverDefines.h"
#include "DataTypes/Log_DataTypes.h"
#include "DataTypes/Filter_DataTypes.h"
#include "DataTypes/Base_WrapperErrorLogger.h"
#include "Logger_CommonDataTypes.h"
#include "IFrameProcessor_Common.h"
typedef struct tagLINPROC_PARAMS
{
    char m_acVersion[MAX_PATH];        // Version info of the application suite
    CBaseMsgBufFSE<STLINDATA>* m_pouLINBuffer;     // Client frame buffer to update
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

class CBaseFrameProcessor_LIN : virtual public IFrameProcessor_Common
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

    virtual HRESULT FPL_ApplyFilters( SFILTERAPPLIED_LIN& ) = 0;

    // To modify the filtering scheme of a logging block
    virtual HRESULT FPL_ApplyFilteringScheme(USHORT ushLogBlkID,
            const SFILTERAPPLIED_LIN& sFilterObj) = 0;

    // Getter for the filtering scheme of a logging block
    virtual HRESULT FPL_GetFilteringScheme(USHORT ushLogBlk,
                                           SFILTERAPPLIED_LIN& sFilterObj) = 0;

    // To update the channel baud rate info to logger
    virtual HRESULT FPL_SetChannelBaudRateDetails(SCONTROLLER_DETAILS_LIN* controllerDetails, int nNumChannels) = 0;

    virtual void vSetMeasurementFileName() = 0;
};
