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

#include "DIL_Interface/DIL_Interface_extern.h"
#include "DIL_Interface/BaseDIL_CAN.h"

class CFrameProcessor_CAN : public CBaseFrameProcessor_CAN, CFrameProcessor_Common
{
private:
	SCANPROC_PARAMS		m_sCANProcParams;
	CCANBufFSE			m_ouFSEBufCAN;
    CFormatMsgCAN       m_ouFormatMsgCAN;
    CBaseDIL_CAN*       m_pouDilCanInterface;
    void vEmptyLogObjArray(CLogObjArray& omLogObjArray);
	// To create a new logging object
	CBaseLogObject* CreateNewLogObj(void);
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

    /* STARTS IMPLEMENTATION OF THE INTERFACE FUNCTIONS... */
    // To initialise this module
    HRESULT FPC_DoInitialisation(SCANPROC_PARAMS* psInitParams);

    // To modify the filtering scheme of a logging block
    HRESULT FPC_ApplyFilteringScheme(USHORT ushLogBlkID,
                                     const SFILTERAPPLIED_CAN& sFilterObj);

    // Getter for the filtering scheme of a logging block
    HRESULT FPC_GetFilteringScheme(USHORT ushLogBlk, 
                                   SFILTERAPPLIED_CAN& sFilterObj);

    // To enable/disable updation of the client flexray frame buffer.
    HRESULT FPC_SetClientCANBufON(BOOL bEnable);

    // To get the flexray buffer of this module
    CBaseCANBufFSE* FPC_GetCANBuffer(void);


    /* USE COMMON BASE CLASS ALIAS FUNCTIONS */

    /* Call to enable/disable logging for a particular block. Having ushBlk equal
    to FOR_ALL, signifies the operation to be performed for all the blocks */
    HRESULT FPC_EnableLoggingBlock(USHORT ushBlk, BOOL bEnable);

    // To enable/disable logging
    HRESULT FPC_EnableLogging(BOOL bEnable);

    /* Call to enable/disable logging for a particular block. Having ushBlk equal
    to FOR_ALL, signifies the operation to be performed for all the blocks */
    HRESULT FPC_EnableFilter(USHORT ushBlk, BOOL bEnable);

    // Query function - client flexray buffer updation status (OFF/ON)
    BOOL FPC_IsClientCANBufON(void);

    // Query function - current logging status (OFF/ON). 
    BOOL FPC_IsLoggingON(void);

    // Query function - current filtering status
    BOOL FPC_IsFilterON(void);

    // To log a string
    HRESULT FPC_LogString(CString& omStr);

    // To add a logging block; must be in editing mode
    HRESULT FPC_AddLoggingBlock(const SLOGINFO& sLogObject);

    // To remove a logging block by its index in the list; editing mode prerequisite
    HRESULT FPC_RemoveLoggingBlock(USHORT ushBlk);

    // Getter for total number of logging blocks
    USHORT FPC_GetLoggingBlockCount(void);

    // To clear the logging block list
    HRESULT FPC_ClearLoggingBlockList(void);

    // Getter for a logging block by specifying its index in the list
    HRESULT FPC_GetLoggingBlock(USHORT ushBlk, SLOGINFO& sLogObject);

    // Setter for a logging block by specifying its index in the list
    HRESULT FPC_SetLoggingBlock(USHORT ushBlk, const SLOGINFO& sLogObject);

    // To reset or revoke the modifications made
    HRESULT FPC_Reset(void);

    // To confirm the modifications made
    HRESULT FPC_Confirm(void);

    // To start logging block editing session
    HRESULT FPC_StartEditingSession(void);

    // To stop logging block editing session
    HRESULT FPC_StopEditingSession(BOOL bConfirm);

    // Getter for the logging configuration data
    HRESULT FPC_GetConfigData(BYTE** ppvConfigData, UINT& unLength);

    // Setter for the logging configuration data
    HRESULT FPC_SetConfigData(BYTE* pvDataStream);

	// To update the associated database list to logger
	HRESULT FPC_SetDatabaseFiles(const CStringArray& omList);

	// To update the channel baud rate info to logger
	HRESULT FPC_SetChannelBaudRateDetails(SCONTROLER_DETAILS* controllerDetails,
															int nNumChannels);

	/* ENDS IMPLEMENTATION OF THE INTERFACE FUNCTIONS... */
};
