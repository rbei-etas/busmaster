
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

#pragma once

//#include "DataTypes/MsgBufAll_DataTypes.h"
#include "MsgBufVSE.h"
#include "DataTypes/Log_DataTypes.h"
#include "DataTypes/Filter_DataTypes.h"
#include "DataTypes/Base_WrapperErrorLogger.h"
#include "DataTypes/J1939_DataTypes.h"
#include "Logger_CommonDataTypes.h"
#include "IFrameProcessor_Common.h"

class CParamLoggerJ1939 : public CParamLoggerBUS
{
public:
    CParamLoggerJ1939()
    {
    };
};

class CBaseFrameProcessor_J1939 : virtual public IFrameProcessor_Common
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


    // To modify the filtering scheme of a logging block
    virtual HRESULT FPJ1_ApplyFilteringScheme(USHORT ushLogBlkID,
            const SFILTERAPPLIED_J1939& sFilterObj) = 0;

    // Getter for the filtering scheme of a logging block
    virtual HRESULT FPJ1_GetFilteringScheme(USHORT ushLogBlk,
                                            SFILTERAPPLIED_J1939& sFilterObj) = 0;

    // To update the channel baud rate info to logger
    virtual HRESULT FPJ1_SetChannelBaudRateDetails(SCONTROLLER_DETAILS* controllerDetails,
            int nNumChannels) = 0;

    virtual void vSetMeasurementFileName() = 0;
};
