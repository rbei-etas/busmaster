
/******************************************************************************
  Project       :  Auto-SAT_Tools
  FileName      :  BaseFrameProcessor_J1939.h
  Description   :  Definition file for CBaseFrameProcessor_J1939 class which 
                   describes the interface of J1939 logging module.
  $Log:   X:/Archive/Sources/FrameProcessor/BaseFrameProcessor_J1939.h_v  $
 * 
 *    Rev 1.3   02 Dec 2011 20:16:48   rac2kor
 * Removed hard coding of version information 
 * string in the log file by accepting it as a parameter
 * from the application / client.
 * 
 *    Rev 1.2   15 Apr 2011 19:20:30   rac2kor
 * Inserted RBEI Copyright information text into the file header.
 * 
 *    Rev 1.1   11 Dec 2010 15:43:30   rac2kor
 * Wrong constructor name was used.
 * 
 *    Rev 1.0   06 Dec 2010 18:53:02   rac2kor
 *  

  Author(s)     :  Ratnadip Choudhury
  Date Created  :  1.12.2010
  Modified By   :  
  Copyright (c) 2011, Robert Bosch Engineering and Business Solutions.  All rights reserved.
******************************************************************************/

#if !defined BASEFRAMEPROCESSOR_J1939_H__INCLUDED_
#define BASEFRAMEPROCESSOR_J1939_H__INCLUDED_

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
};

#endif // BASEFRAMEPROCESSOR_J1939_H__INCLUDED_
