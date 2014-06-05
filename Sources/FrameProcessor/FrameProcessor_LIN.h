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
#include "DIL_Interface/DIL_Interface_extern.h"
#include "DIL_Interface/BaseDIL_LIN.h"


class CFrameProcessor_LIN : public CBaseFrameProcessor_LIN, CFrameProcessor_Common
{
private:
    SLINPROC_PARAMS     m_sLINProcParams;
    CLINBufFSE          m_ouFSEBufLIN;
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

    // To modify the filtering scheme of a logging block
    HRESULT FPL_ApplyFilteringScheme(USHORT ushLogBlkID,
                                     const SFILTERAPPLIED_LIN& sFilterObj);

    // Getter for the filtering scheme of a logging block
    HRESULT FPL_GetFilteringScheme(USHORT ushLogBlk,
                                   SFILTERAPPLIED_LIN& sFilterObj);

    // To enable/disable updation of the client flexray frame buffer.
    HRESULT FPL_SetClientLINBufON(bool bEnable);

    // To get the flexray buffer of this module
    CBaseLINBufFSE* FPL_GetLINBuffer(void);

    void FPL_vCloseLogFile();

    bool FPL_IsDataLogged(void);

    bool FPL_IsThreadBlocked(void);

    void FPL_DisableDataLogFlag(void);

    /* USE COMMON BASE CLASS ALIAS FUNCTIONS */

    /* Call to enable/disable logging for a particular block. Having ushBlk equal
    to FOR_ALL, signifies the operation to be performed for all the blocks */
    HRESULT FPL_EnableLoggingBlock(USHORT ushBlk, bool bEnable);

    // To enable/disable logging
    HRESULT FPL_EnableLogging(bool bEnable);

    // Query function - client flexray buffer updation status (OFF/ON)
    bool FPL_IsClientLINBufON(void);

    HRESULT FPL_EnableFilter(USHORT ushBlk, bool bEnable);
    // Query function - current logging status (OFF/ON).
    bool FPL_IsLoggingON(void);

    bool FPL_IsLINDataLogged(void);

    bool FPL_IsLINThreadBlocked(void);

    void FPL_DisableLINDataLogFlag(void);

    // Query function - current filtering status
    bool FPL_IsFilterON(void);

    // To log a string
    HRESULT FPL_LogString(CString& omStr);

    // To add a logging block; must be in editing mode
    HRESULT FPL_AddLoggingBlock(const SLOGINFO& sLogObject);

    // To remove a logging block by its index in the list; editing mode prerequisite
    HRESULT FPL_RemoveLoggingBlock(USHORT ushBlk);

    // Getter for total number of logging blocks
    USHORT FPL_GetLoggingBlockCount(void);

    // To clear the logging block list
    HRESULT FPL_ClearLoggingBlockList(void);

    // Getter for a logging block by specifying its index in the list
    HRESULT FPL_GetLoggingBlock(USHORT ushBlk, SLOGINFO& sLogObject);

    // Setter for a logging block by specifying its index in the list
    HRESULT FPL_SetLoggingBlock(USHORT ushBlk, const SLOGINFO& sLogObject);

    // To reset or revoke the modifications made
    HRESULT FPL_Reset(void);

    // To confirm the modifications made
    HRESULT FPL_Confirm(void);

    // To start logging block editing session
    HRESULT FPL_StartEditingSession(void);

    // To stop logging block editing session
    HRESULT FPL_StopEditingSession(bool bConfirm);

    // Getter for the logging configuration data
    HRESULT FPL_GetConfigData(BYTE** ppvConfigData, UINT& unLength);

    // For writing in to XML
    HRESULT FPL_GetConfigData(xmlNodePtr pxmlNodePtr);
    // Setter for the logging configuration data
    HRESULT FPL_SetConfigData(BYTE* pvDataStream, const CString& omStrVersion);
    HRESULT FPL_SetConfigData(xmlDocPtr pDoc);

    // To update the associated database list to logger
    HRESULT FPL_SetDatabaseFiles(const CStringArray& omList);

    // To update the channel baud rate info to logger
    HRESULT FPL_SetChannelBaudRateDetails(SCONTROLLER_DETAILS_LIN* controllerDetails,
                                          int nNumChannels,ETYPE_BUS eBus);
};
