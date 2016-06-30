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

//#include "DataTypes/MsgBufAll_DataTypes.h"
#include "BaseMsgBufAll.h"
#include "BaseMsgBufAll.h"
#include "DataTypes/Log_DataTypes.h"
#include "DataTypes/Filter_DataTypes.h"
#include "DataTypes/Base_WrapperErrorLogger.h"
#include "CANDriverDefines.h"
#include "IFrameProcessor_Common.h"
typedef struct tagCANPROC_PARAMS
{
    char m_acVersion[MAX_PATH];        // Version info of the application suite
    CBaseMsgBufFSE<STCANDATA>* m_pouCANBuffer;     // Client frame buffer to update
    Base_WrapperErrorLogger* m_pILog;   // Error logger module
    //CNetworkStats* m_pouNetworkStat;    // Network statistics object
    DWORD dwClientID;
    tagCANPROC_PARAMS()
    {
        m_pouCANBuffer = nullptr;
        m_pILog = nullptr;
        dwClientID = 0;
        for (int i = 0; i < MAX_PATH; i++)
        {
            m_acVersion[i] = L'\0';
        }
    }
} SCANPROC_PARAMS;

class CBaseFrameProcessor_CAN : virtual public IFrameProcessor_Common
{
public:

    CBaseFrameProcessor_CAN()
    {
    };

    virtual ~CBaseFrameProcessor_CAN()
    {
    };

    // To initialise this module
    virtual HRESULT FPC_DoInitialisation(SCANPROC_PARAMS* psInitParams) = 0;



    virtual void FPC_vCloseLogFile() = 0;


    virtual HRESULT FPC_ApplyFilters( const SFILTERAPPLIED_CAN& sFilterObj ) = 0;

    // To modify the filtering scheme of a logging block
    virtual HRESULT FPC_ApplyFilteringScheme(USHORT ushLogBlkID,
            const SFILTERAPPLIED_CAN& sFilterObj) = 0;



    // Getter for the filtering scheme of a logging block
    virtual HRESULT FPC_GetFilteringScheme(USHORT ushLogBlk,
                                           SFILTERAPPLIED_CAN& sFilterObj) = 0;

    // To update the channel baud rate info to logger
    virtual HRESULT FPC_SetChannelBaudRateDetails(SCONTROLLER_DETAILS* controllerDetails,int nNumChannels) = 0;
    virtual void vSetMeasurementFileName() = 0;

};
