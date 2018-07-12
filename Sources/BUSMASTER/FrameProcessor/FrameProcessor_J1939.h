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
 * \file      FrameProcessor_J1939.h
 * \brief     Definition file for CFrameProcessor_J1939 class
 * \author    Ratnadip Choudhury
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Definition file for CFrameProcessor_J1939 class.
 */

#pragma once

#include "BaseFrameProcessor_J1939.h"
#include "FrameProcessor_Common.h"
#include "Format/FormatMsgJ1939.h"
#include "BaseDIL_CAN.h"
#include "BaseDIL_J1939.h"

#pragma warning( disable : 4250 )

class CFrameProcessor_J1939 : public CBaseFrameProcessor_J1939, CFrameProcessor_Common
{
private:
    CParamLoggerJ1939   m_sJ1939ProcParams;
    CBaseDILI_J1939*    m_pouDIL_J1939;
    CMsgBufVSE          m_ouVSEBufJ1939;
    STJ1939_MSG         m_sJ1939Data;
    BYTE*               m_pbyJ1939Data;
    SFORMATTEDATA_J1939 m_sCurrFormatDat;
    CFormatMsgJ1939     m_ouFormatMsgJ1939;

    USHORT ushCalculateStrLen(bool bForHex, USHORT ushLength);
    void vEmptyLogObjArray(CLogObjArray& omLogObjArray);
    // To create a new logging object
    CBaseLogObject* CreateNewLogObj(const CString& omStrVersion);
    // To delete a logging object
    void DeleteLogObj(CBaseLogObject*& pouLogObj);
    // To create the time mode mapping
    void CreateTimeModeMapping(SYSTEMTIME& CurrSysTime, UINT64& unAbsTime);


public:

    CFrameProcessor_J1939();      // Constructor
    ~CFrameProcessor_J1939();     // Destructor

    BOOL InitInstance(void);
    int ExitInstance(void);
    void vRetrieveDataFromBuffer(void);

    /* STARTS IMPLEMENTATION OF THE INTERFACE FUNCTIONS... */
    // To initialise this module
    HRESULT FPJ1_DoInitialisation(CParamLoggerJ1939* psInitParams);

    // To modify the filtering scheme of a logging block
    HRESULT FPJ1_ApplyFilteringScheme(USHORT ushLogBlkID,
                                      const SFILTERAPPLIED_J1939& sFilterObj);

    // Getter for the filtering scheme of a logging block
    HRESULT FPJ1_GetFilteringScheme(USHORT ushLogBlk,
                                    SFILTERAPPLIED_J1939& sFilterObj);


    /* USE COMMON BASE CLASS ALIAS FUNCTIONS */


    // To update the channel baud rate info to logger
    HRESULT FPJ1_SetChannelBaudRateDetails(SCONTROLLER_DETAILS* controllerDetails,
                                           int nNumChannels);

    void vSetMeasurementFileName();
    /* ENDS IMPLEMENTATION OF THE INTERFACE FUNCTIONS... */
};
