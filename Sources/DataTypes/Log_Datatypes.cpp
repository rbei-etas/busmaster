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
 * \file      Log_Datatypes.cpp
 * \brief     Source file for log data types.
 * \author    Ratnadip Choudhury
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Source file for log data types.
 */


#include "DataTypes_StdAfx.h"
#include "include/Utils_macro.h"
#include "Log_Datatypes.h"


/* tagLogInfo --- STARTS */

/******************************************************************************
  Function Name    :  tagLogInfo
  Input(s)         :  -
  Output           :  -
  Functionality    :  Standard constructor
  Member of        :  tagLogInfo
  Friend of        :  -                                   
  Author(s)        :  Ratnadip Choudhury
  Date Created     :  1.12.2009
  Modification date:  
  Modification By  :  
******************************************************************************/
tagLogInfo::tagLogInfo()
{
    vClear();
}

/******************************************************************************
  Function Name    :  vClear
  Input(s)         :  void
  Output           :  void
  Functionality    :  Clears information inthe current logging block
  Member of        :  tagLogInfo
  Friend of        :  -                                   
  Author(s)        :  Ratnadip Choudhury
  Date Created     :  1.12.2009
  Modification date:  
  Modification By  :  
******************************************************************************/
void tagLogInfo::vClear(void)
{
    m_ushID              = 0x0;
    m_bIsUpdated         = TRUE;
    m_bEnabled           = TRUE;
    m_eLogTimerMode      = TIME_MODE_SYSTEM;
    m_eNumFormat         = HEXADECIMAL;
    m_eFileMode          = APPEND_MODE;
    m_bResetAbsTimeStamp = FALSE;
    m_ChannelSelected    = CHANNEL_All_UNSPECIFIED;
    _tcscpy(m_sLogFileName, _T(""));

    // Init Trigger Condition
    m_sLogTrigger.m_unTriggerType = NONE;   // No trigger
    m_sLogTrigger.m_unStartID = 0;	        // No Start-ID
    m_sLogTrigger.m_unStopID = 0;           // No Stop-ID
}

/******************************************************************************
  Function Name    :  unGetSize
  Input(s)         :  void
  Output           :  UINT - size of the current logging block
  Functionality    :  Returns in bytes size of the current logging block.
  Member of        :  tagLogInfo
  Friend of        :  -                                   
  Author(s)        :  Ratnadip Choudhury
  Date Created     :  1.12.2009
  Modification date:  
  Modification By  :  
******************************************************************************/
UINT tagLogInfo::unGetSize(void) const
{
    UINT Result = 
        sizeof(m_bIsUpdated) + sizeof(m_bEnabled) + sizeof(m_eLogTimerMode) + 
        sizeof(m_eNumFormat) + sizeof(m_eFileMode) + sizeof(m_bResetAbsTimeStamp) + sizeof(m_ChannelSelected)
        + sizeof(m_sLogFileName) + sizeof(m_sLogTrigger);

    return Result;
}

/******************************************************************************
  Function Name    :  pbGetConfigData
  Input(s)         :  pbTarget - The target buffer to save logging data.
                      It assumes that pbTarget is currently pointing to a loca-
                      tion having sufficient space.
  Output           :  BYTE * - Location of the next available buffer.
  Functionality    :  Saves logging block information of the current object into
                      the target buffer. Advances the writing pointer to the
                      next byte occurring after the written block.
  Member of        :  tagLogInfo
  Friend of        :  -                                   
  Author(s)        :  Ratnadip Choudhury
  Date Created     :  1.12.2009
  Modification date:  
  Modification By  :  
******************************************************************************/
BYTE* tagLogInfo::pbGetConfigData(BYTE* pbTarget) const
{
    BYTE* pbTStream = pbTarget;

    COPY_DATA(pbTStream, &m_bIsUpdated,       sizeof(m_bIsUpdated        ));
    COPY_DATA(pbTStream, &m_bEnabled,         sizeof(m_bEnabled          ));
    COPY_DATA(pbTStream, &m_eLogTimerMode,    sizeof(m_eLogTimerMode     ));
    COPY_DATA(pbTStream, &m_eNumFormat,       sizeof(m_eNumFormat        ));
    COPY_DATA(pbTStream, &m_eFileMode,        sizeof(m_eFileMode         ));
    COPY_DATA(pbTStream, &m_bResetAbsTimeStamp, sizeof(m_bResetAbsTimeStamp));
    COPY_DATA(pbTStream, &m_ChannelSelected,  sizeof(m_ChannelSelected   ));
    COPY_DATA(pbTStream, m_sLogFileName,      sizeof(m_sLogFileName      ));
    COPY_DATA(pbTStream, &m_sLogTrigger,      sizeof(m_sLogTrigger       ));

    return pbTStream;
}

/******************************************************************************
  Function Name    :  pbSetConfigData
  Input(s)         :  pbSource - The source buffer to retrieve logging data.
                      It assumes that pbSource is currently pointing to locati-
                      on of a logging block data.
  Output           :  BYTE * - Location of the next byte after the block.
  Functionality    :  Retrieves logging block information and copies them into
                      the current object. Advances the reading pointer to the
                      next byte occurring after the block.
  Member of        :  tagLogInfo
  Friend of        :  -                                   
  Author(s)        :  Ratnadip Choudhury
  Date Created     :  1.12.2009
  Modification date:  
  Modification By  :  
******************************************************************************/
BYTE* tagLogInfo::pbSetConfigData(BYTE* pbSource, BYTE bytLogVersion)
{
    BYTE* pbSStream = pbSource;

    COPY_DATA_2(&m_bIsUpdated,       pbSStream, sizeof(m_bIsUpdated      ));
    COPY_DATA_2(&m_bEnabled,         pbSStream, sizeof(m_bEnabled        ));
    COPY_DATA_2(&m_eLogTimerMode,    pbSStream, sizeof(m_eLogTimerMode   ));
    COPY_DATA_2(&m_eNumFormat,       pbSStream, sizeof(m_eNumFormat      ));
    COPY_DATA_2(&m_eFileMode,        pbSStream, sizeof(m_eFileMode       ));

	/* If version is 1.6.2 and above */
	if ( bytLogVersion > 0x1 )
		COPY_DATA_2(&m_bResetAbsTimeStamp, pbSStream, sizeof(m_bResetAbsTimeStamp));
    
	COPY_DATA_2(&m_ChannelSelected,  pbSStream, sizeof(m_ChannelSelected ));
    COPY_DATA_2(m_sLogFileName,      pbSStream, sizeof(m_sLogFileName    ));
    COPY_DATA_2(&m_sLogTrigger,      pbSStream, sizeof(m_sLogTrigger     ));

    return pbSStream;
}
/* tagLogInfo --- ENDS */
