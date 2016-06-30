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
* \file      FrameProcessor_LIN.h
* \brief     Definition file for CFrameProcessor_LIN class.
* \author    Shashank Vernekar
* \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
*
* Definition file for CFrameProcessor_LIN class.
*/

#pragma once

#include "Include/BaseDefs.h"
#include "BaseFrameProcessor_LIN.h"
#include "FrameProcessor_Common.h"
#include "Format/FormatMsgLIN.h"
//#include "DIL_Interface_extern.h"
#include "BaseDIL_LIN.h"
#include "MsgBufFSE.h"

class CFrameProcessor_LIN : public CBaseFrameProcessor_LIN, CFrameProcessor_Common
{
private:
    SLINPROC_PARAMS     m_sLINProcParams;
    CMsgBufFSE<STLINDATA> m_ouFSEBufLIN;
    CFormatMsgLIN       m_ouFormatMsgLIN;
    CBaseDIL_LIN*       m_pouDilLINInterface;
    void vEmptyLogObjArray(CLogObjArray& omLogObjArray);
    // To create a new logging object
    CBaseLogObject* CreateNewLogObj(const CString& omStrVersion);
    // To delete a logging object
    void DeleteLogObj(CBaseLogObject*& pouLogObj);

    // To create the time mode mapping
    void CreateTimeModeMapping(SYSTEMTIME& CurrSysTime, UINT64& unAbsTime);


public:

    CFrameProcessor_LIN();      // Constructor
    ~CFrameProcessor_LIN();     // Destructor

    BOOL InitInstance(void);
    int ExitInstance(void);
    void vRetrieveDataFromBuffer(void);

    /* STARTS IMPLEMENTATION OF THE INTERFACE FUNCTIONS... */
    // To initialise this module
    HRESULT FPL_DoInitialisation(SLINPROC_PARAMS* psInitParams);

    HRESULT FPL_ApplyFilters( SFILTERAPPLIED_LIN& );

    // To modify the filtering scheme of a logging block
    HRESULT FPL_ApplyFilteringScheme(USHORT ushLogBlkID,
                                     const SFILTERAPPLIED_LIN& sFilterObj);

    // Getter for the filtering scheme of a logging block
    HRESULT FPL_GetFilteringScheme(USHORT ushLogBlk,
                                   SFILTERAPPLIED_LIN& sFilterObj);

    HRESULT FPL_SetClientLINBufON(bool bEnable);

    CBaseLINBufFSE* FPL_GetLINBuffer(void);

    void FPL_vCloseLogFile();

    bool FPL_IsDataLogged(void);

    bool FPL_IsThreadBlocked(void);

    void FPL_DisableDataLogFlag(void);


    // To update the channel baud rate info to logger
    HRESULT FPL_SetChannelBaudRateDetails(SCONTROLLER_DETAILS_LIN* controllerDetails,
                                          int nNumChannels);
    void vSetMeasurementFileName();
};
