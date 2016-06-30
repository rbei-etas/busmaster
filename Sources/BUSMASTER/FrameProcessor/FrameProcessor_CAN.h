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
 * \file      FrameProcessor_CAN.h
 * \brief     Definition file for CFrameProcessor_CAN class.
 * \author    Ratnadip Choudhury
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Definition file for CFrameProcessor_CAN class.
 */

#pragma once

#include "Include/BaseDefs.h"
#include "BaseFrameProcessor_CAN.h"
#include "FrameProcessor_Common.h"
#include "Format/FormatMsgCAN.h"

//#include "DIL_Interface_extern.h"
#include "BaseDIL_CAN.h"
#include "MsgBufFSE.h"

class CFrameProcessor_CAN : public CBaseFrameProcessor_CAN , public CFrameProcessor_Common
{
private:
    SCANPROC_PARAMS     m_sCANProcParams;
    CMsgBufFSE<STCANDATA> m_ouFSEBufCAN;
    CFormatMsgCAN       m_ouFormatMsgCAN;
    CBaseDIL_CAN*       m_pouDilCanInterface;
    void vEmptyLogObjArray(CLogObjArray& omLogObjArray);
    // To create a new logging object
    CBaseLogObject* CreateNewLogObj(const CString& omStrVersion);
    // To delete a logging object
    void DeleteLogObj(CBaseLogObject*& pouLogObj);

    // To create the time mode mapping
    void CreateTimeModeMapping(SYSTEMTIME& CurrSysTime, UINT64& unAbsTime);


public:

    CFrameProcessor_CAN();      // Constructor
    ~CFrameProcessor_CAN();     // Destructor

    BOOL InitInstance(void);
    int ExitInstance(void);
    void vRetrieveDataFromBuffer(void);


    void vPopulateMainSubList( USHORT ushBlk, CMainEntryList& DestList );
    void vPopulateFilterApplied( USHORT ushBlk, CMainEntryList& DestList );

    /* STARTS IMPLEMENTATION OF THE INTERFACE FUNCTIONS... */
    // To initialise this module
    HRESULT FPC_DoInitialisation(SCANPROC_PARAMS* psInitParams);

    HRESULT FPC_ApplyFilters( const SFILTERAPPLIED_CAN& sFilterObj );
    // To modify the filtering scheme of a logging block
    HRESULT FPC_ApplyFilteringScheme(USHORT ushLogBlkID,
                                     const SFILTERAPPLIED_CAN& sFilterObj);

    // Getter for the filtering scheme of a logging block
    HRESULT FPC_GetFilteringScheme(USHORT ushLogBlk,
                                   SFILTERAPPLIED_CAN& sFilterObj);

    HRESULT FPC_SetClientCANBufON(bool bEnable);

    CBaseCANBufFSE* FPC_GetCANBuffer(void);

    void FPC_vCloseLogFile();


    // To update the associated database list to logger
    HRESULT FPC_SetDatabaseFiles(const CStringArray& omList);

    // To update the channel baud rate info to logger
    HRESULT FPC_SetChannelBaudRateDetails(SCONTROLLER_DETAILS* controllerDetails,
                                          int nNumChannels);
    void vSetMeasurementFileName();
};
