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
 * \file      Filter_Datatypes.cpp
 * \brief     Source file for filter data types.
 * \author    Ratnadip Choudhury
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Source file for filter data types.
 */

#include "DataTypes_StdAfx.h"
#include "Filter_Datatypes.h"

const int SIZE_TCHAR = sizeof(TCHAR);

/* Starts SFILTERNAME / tagFilterName */

/******************************************************************************
  Function Name    :  tagFilterName
  Input(s)         :  -
  Output           :  -
  Functionality    :  Standard constructor
  Member of        :  tagFilterName
  Friend of        :  -                                   
  Author(s)        :  Ratnadip Choudhury
  Date Created     :  1.12.2009
  Modification date:  
  Modification By  :  
******************************************************************************/
tagFilterName::tagFilterName()
{
    m_acFilterName[LENGTH_FILTERNAME - 1] = L'\0';
    vClear();
}

/******************************************************************************
  Function Name    :  vClear
  Input(s)         :  void
  Output           :  void
  Functionality    :  Clears information inthe current filtering block
  Member of        :  tagFilterName
  Friend of        :  -                                   
  Author(s)        :  Ratnadip Choudhury
  Date Created     :  1.12.2009
  Modification date:  
  Modification By  :  
******************************************************************************/
void tagFilterName::vClear(void)
{
    _tcsset(m_acFilterName, L'');
    m_bFilterType = FALSE;
}

/******************************************************************************
  Function Name    :  operator=
  Input(s)         :  const tagFilterName& RefObj - The source object
  Output           :  tagFilterName& - The current object reference.
  Functionality    :  Copies a source object by '=' operator overloading.
  Member of        :  tagFilterName
  Friend of        :  -                                   
  Author(s)        :  Ratnadip Choudhury
  Date Created     :  1.12.2009
  Modification date:  
  Modification By  :  
******************************************************************************/
tagFilterName& tagFilterName::operator=(const tagFilterName& RefObj)
{
    _tcscpy(m_acFilterName, RefObj.m_acFilterName);
    m_bFilterType = RefObj.m_bFilterType;

    return *this;
}

/******************************************************************************
  Function Name    :  pbGetConfigData
  Input(s)         :  pbTarget - The target buffer to save filtering data.
                      It assumes that pbTarget is currently pointing to a loca-
                      tion having sufficient space.
  Output           :  BYTE * - Location of the next available buffer.
  Functionality    :  Saves filtering block information of the current object
                      into the target buffer. Advances the writing pointer to
                      the next byte occurring after the written block.
  Member of        :  tagFilterName
  Friend of        :  -                                   
  Author(s)        :  Ratnadip Choudhury
  Date Created     :  1.12.2009
  Modification date:  
  Modification By  :  
******************************************************************************/
BYTE* tagFilterName::pbGetConfigData(BYTE* pbTarget) const
{
    BYTE* pbTStream = pbTarget;

    COPY_DATA(pbTStream, m_acFilterName, LENGTH_FILTERNAME * SIZE_TCHAR);
    COPY_DATA(pbTStream, &m_bFilterType, sizeof(m_bFilterType));

    return pbTStream;
}

/******************************************************************************
  Function Name    :  pbSetConfigData
  Input(s)         :  pbSource - The source buffer to retrieve filtering data.
                      It assumes that pbSource is currently pointing to locati-
                      on of a filtering block data.
  Output           :  BYTE * - Location of the next byte after the block.
  Functionality    :  Retrieves filtering block information and copies them
                      into the current object. Advances the reading pointer to
                      the next byte occurring after the block.
  Member of        :  tagFilterName
  Friend of        :  -                                   
  Author(s)        :  Ratnadip Choudhury
  Date Created     :  1.12.2009
  Modification date:  
  Modification By  :  
******************************************************************************/
BYTE* tagFilterName::pbSetConfigData(BYTE* pbTarget)
{
    BYTE* pbTStream = pbTarget;

    COPY_DATA_2(m_acFilterName, pbTStream, LENGTH_FILTERNAME * SIZE_TCHAR);
    COPY_DATA_2(&m_bFilterType, pbTStream, sizeof(m_bFilterType));

    return pbTStream;
}

/* Ends SFILTERNAME / tagFilterName */

/* Starts SFILTER / tagSFILTER */

/******************************************************************************
  Function Name    :  tagSFILTER
  Input(s)         :  -
  Output           :  -
  Functionality    :  Standard constructor
  Member of        :  tagSFILTER
  Friend of        :  -                                   
  Author(s)        :  Ratnadip Choudhury
  Date Created     :  1.12.2009
  Modification date:  
  Modification By  :  
******************************************************************************/
tagSFILTER::tagSFILTER()
{
    vClear();
}

/******************************************************************************
  Function Name    :  vClear
  Input(s)         :  void
  Output           :  void
  Functionality    :  Clears information inthe current filtering block
  Member of        :  tagSFILTER
  Friend of        :  -                                   
  Author(s)        :  Ratnadip Choudhury
  Date Created     :  1.12.2009
  Modification date:  
  Modification By  :  
******************************************************************************/
void tagSFILTER::vClear(void)
{
    m_ucFilterType  = 0;
    m_dwMsgIDFrom   = 0;
    m_dwMsgIDTo     = 0;
    m_eDrctn        = DIR_ALL;
}

/******************************************************************************
  Function Name    :  operator=
  Input(s)         :  const tagSFILTER& RefObj - The source object
  Output           :  tagSFILTER& - The current object reference.
  Functionality    :  Copies a source object by '=' operator overloading.
  Member of        :  tagSFILTER
  Friend of        :  -                                   
  Author(s)        :  Ratnadip Choudhury
  Date Created     :  1.12.2009
  Modification date:  
  Modification By  :  
******************************************************************************/
tagSFILTER& tagSFILTER::operator=(const tagSFILTER& RefObj)
{
    m_ucFilterType  = RefObj.m_ucFilterType;
    m_dwMsgIDFrom   = RefObj.m_dwMsgIDFrom; 
    m_dwMsgIDTo     = RefObj.m_dwMsgIDTo;
    m_eDrctn        = RefObj.m_eDrctn;

    return *this;
}

/******************************************************************************
  Function Name    :  unGetSize
  Input(s)         :  void
  Output           :  UINT - size of the current filtering block
  Functionality    :  Returns in bytes size of the current filtering block.
  Member of        :  tagSFILTER
  Friend of        :  -                                   
  Author(s)        :  Ratnadip Choudhury
  Date Created     :  1.12.2009
  Modification date:  
  Modification By  :  
******************************************************************************/
UINT tagSFILTER::unGetSize(void) const
{
    return (sizeof(m_ucFilterType) + sizeof(m_dwMsgIDFrom) + 
            sizeof(m_dwMsgIDTo) + sizeof(m_eDrctn));
}

/******************************************************************************
  Function Name    :  pbGetConfigData
  Input(s)         :  pbTarget - The target buffer to save filtering data.
                      It assumes that pbTarget is currently pointing to a loca-
                      tion having sufficient space.
  Output           :  BYTE * - Location of the next available buffer.
  Functionality    :  Saves filtering block information of the current object
                      into the target buffer. Advances the writing pointer to
                      the next byte occurring after the written block.
  Member of        :  tagSFILTER
  Friend of        :  -                                   
  Author(s)        :  Ratnadip Choudhury
  Date Created     :  1.12.2009
  Modification date:  
  Modification By  :  
******************************************************************************/
BYTE* tagSFILTER::pbGetConfigData(BYTE* pbTarget) const
{
    BYTE* pbTStream = pbTarget;

    COPY_DATA(pbTStream, &m_ucFilterType, sizeof(m_ucFilterType));
    COPY_DATA(pbTStream, &m_dwMsgIDFrom, sizeof(m_dwMsgIDFrom));
    COPY_DATA(pbTStream, &m_dwMsgIDTo, sizeof(m_dwMsgIDTo));
    COPY_DATA(pbTStream, &m_eDrctn, sizeof(m_eDrctn));

    return pbTStream;
}

/******************************************************************************
  Function Name    :  pbSetConfigData
  Input(s)         :  pbSource - The source buffer to retrieve filtering data.
                      It assumes that pbSource is currently pointing to locati-
                      on of a filtering block data.
  Output           :  BYTE * - Location of the next byte after the block.
  Functionality    :  Retrieves filtering block information and copies them
                      into the current object. Advances the reading pointer to
                      the next byte occurring after the block.
  Member of        :  tagSFILTER
  Friend of        :  -                                   
  Author(s)        :  Ratnadip Choudhury
  Date Created     :  1.12.2009
  Modification date:  
  Modification By  :  
******************************************************************************/
BYTE* tagSFILTER::pbSetConfigData(BYTE* pbTarget)
{
    vClear();

    BYTE* pbTStream = pbTarget;

    COPY_DATA_2(&m_ucFilterType, pbTStream, sizeof(m_ucFilterType));
    COPY_DATA_2(&m_dwMsgIDFrom, pbTStream, sizeof(m_dwMsgIDFrom));
    COPY_DATA_2(&m_dwMsgIDTo, pbTStream, sizeof(m_dwMsgIDTo));
    COPY_DATA_2(&m_eDrctn, pbTStream, sizeof(m_eDrctn));

    return pbTStream;
}
/* Ends SFILTER / tagSFILTER */


/* Starts SFILTER_CAN */

/******************************************************************************
  Function Name    :  SFILTER_CAN
  Input(s)         :  -
  Output           :  -
  Functionality    :  Standard constructor
  Member of        :  SFILTER_CAN
  Friend of        :  -                                   
  Author(s)        :  Ratnadip Choudhury
  Date Created     :  1.12.2009
  Modification date:  
  Modification By  :  
******************************************************************************/
SFILTER_CAN::SFILTER_CAN()
{
    vClear();
}

/******************************************************************************
  Function Name    :  ~SFILTER_CAN
  Input(s)         :  -
  Output           :  -
  Functionality    :  Destructor
  Member of        :  SFILTER_CAN
  Friend of        :  -                                   
  Author(s)        :  Ratnadip Choudhury
  Date Created     :  1.12.2009
  Modification date:  
  Modification By  :  
******************************************************************************/
SFILTER_CAN::~SFILTER_CAN()
{
    vClear();
}

/******************************************************************************
  Function Name    :  operator=
  Input(s)         :  const SFILTER_CAN& RefObj - The source object
  Output           :  SFILTER_CAN& - The current object reference.
  Functionality    :  Copies a source object by '=' operator overloading.
  Member of        :  SFILTER_CAN
  Friend of        :  -                                   
  Author(s)        :  Ratnadip Choudhury
  Date Created     :  1.12.2009
  Modification date:  
  Modification By  :  
******************************************************************************/
SFILTER_CAN& SFILTER_CAN::operator=(const SFILTER_CAN& RefObj)
{
    this->SFILTER::operator = (RefObj);

    m_byMsgType = RefObj.m_byMsgType;
    m_byIDType  = RefObj.m_byIDType;
    m_eChannel = RefObj.m_eChannel;

    return *this;
}

/******************************************************************************
  Function Name    :  vClear
  Input(s)         :  void
  Output           :  void
  Functionality    :  Clears information inthe current filtering block
  Member of        :  SFILTER_CAN
  Friend of        :  -                                   
  Author(s)        :  Ratnadip Choudhury
  Date Created     :  1.12.2009
  Modification date:  
  Modification By  :  
******************************************************************************/
void SFILTER_CAN::vClear(void)
{
    m_byIDType = TYPE_ID_CAN_NONE;
    m_byMsgType = TYPE_MSG_CAN_NONE;
    m_eChannel = CAN_CHANNEL_ALL;
    this->SFILTER::vClear();
}

/******************************************************************************
  Function Name    :  bDoesFrameOccur
  Input(s)         :  psCurrFrame - The frame in question.
  Output           :  TRUE if the frame occurs in the filtering block.
  Functionality    :  Query function to know if this filtering block is 
                      configured for the message entry passed.
  Member of        :  SFILTER_CAN
  Friend of        :  -                                   
  Author(s)        :  Ratnadip Choudhury
  Date Created     :  1.12.2009
  Modification date:  
  Modification By  :  
******************************************************************************/
BOOL SFILTER_CAN::bDoesFrameOccur(const void* psCurrFrame) const
{
    BOOL bResult = FALSE;

    SFRAMEINFO_BASIC_CAN& sCurrFrame = *((SFRAMEINFO_BASIC_CAN *) psCurrFrame);

    // For Signle Id Filtering
    if (m_ucFilterType == defFILTER_TYPE_SINGLE_ID)
    {
        // Check for Message ID
        if (m_dwMsgIDFrom == sCurrFrame.m_dwFrameID)
        {
            // Check for message Channel
            if ( (CAN_CHANNEL_ALL == m_eChannel) ||
                 (m_eChannel == sCurrFrame.m_eChannel) )
            {
                // Check for message direction
                if ( (DIR_ALL == m_eDrctn) ||
                     (DIR_ALL == sCurrFrame.m_eDrctn) ||
                     (m_eDrctn == sCurrFrame.m_eDrctn) )
				{
                    // Check for extended Id
                    if ((TYPE_ID_CAN_ALL == m_byIDType) ||
                        (m_byIDType == sCurrFrame.m_byIDType))
                    {
                        //Check for RTR
                        if ((TYPE_MSG_CAN_ALL == m_byMsgType) ||
                            (m_byMsgType == sCurrFrame.m_byMsgType))
                        {
					        bResult = TRUE;
                        }
                    }
				}
			}
        }
    }
    // For Range Filter
    else if (m_ucFilterType == defFILTER_TYPE_ID_RANGE)
    {
        // Check for message falling in the Range
        if ( (sCurrFrame.m_dwFrameID >= m_dwMsgIDFrom) &&
             (sCurrFrame.m_dwFrameID <= m_dwMsgIDTo) )
        {
            // Check for message Channel
            if ( (CAN_CHANNEL_ALL == m_eChannel) ||
                 (m_eChannel == sCurrFrame.m_eChannel) )
            {
               // Check for message Direction
                if ( (DIR_ALL == m_eDrctn) ||
                     (DIR_ALL == sCurrFrame.m_eDrctn) ||
                     (m_eDrctn == sCurrFrame.m_eDrctn) )
				{
					// Check for extended Id
                    if ((TYPE_ID_CAN_ALL == m_byIDType) ||
                        (m_byIDType == sCurrFrame.m_byIDType))
                    {
                        //Check for RTR
                        if ((TYPE_MSG_CAN_ALL == m_byMsgType) ||
                            (m_byMsgType == sCurrFrame.m_byMsgType))
                        {
					        bResult = TRUE;
                        }
                    }
				}
            }
		}
    }
    else
    {
        ASSERT(FALSE);
    }

    return bResult;
}

/******************************************************************************
  Function Name    :  unGetSize
  Input(s)         :  void
  Output           :  UINT - size of the current filtering block
  Functionality    :  Returns in bytes size of the current filtering block.
  Member of        :  SFILTER_CAN
  Friend of        :  -                                   
  Author(s)        :  Ratnadip Choudhury
  Date Created     :  1.12.2009
  Modification date:  
  Modification By  :  
******************************************************************************/
UINT SFILTER_CAN::unGetSize(void) const
{
    UINT Result = this->SFILTER::unGetSize();
    Result += sizeof(m_byIDType);
    Result += sizeof(m_byMsgType);
    Result += sizeof(m_eChannel);

    return Result;
}

/******************************************************************************
  Function Name    :  pbGetConfigData
  Input(s)         :  pbTarget - The target buffer to save filtering data.
                      It assumes that pbTarget is currently pointing to a loca-
                      tion having sufficient space.
  Output           :  BYTE * - Location of the next available buffer.
  Functionality    :  Saves filtering block information of the current object
                      into the target buffer. Advances the writing pointer to
                      the next byte occurring after the written block.
  Member of        :  SFILTER_CAN
  Friend of        :  -                                   
  Author(s)        :  Ratnadip Choudhury
  Date Created     :  1.12.2009
  Modification date:  
  Modification By  :  
******************************************************************************/
BYTE* SFILTER_CAN::pbGetConfigData(BYTE* pbTarget) const
{
    BYTE* pbTStream = pbTarget;

    pbTStream = this->SFILTER::pbGetConfigData(pbTStream);
    COPY_DATA(pbTStream, &m_byIDType, sizeof(m_byIDType));
    COPY_DATA(pbTStream, &m_byMsgType, sizeof(m_byMsgType));
    COPY_DATA(pbTStream, &m_eChannel, sizeof(m_eChannel));

    return pbTStream;
}

/******************************************************************************
  Function Name    :  pbSetConfigData
  Input(s)         :  pbSource - The source buffer to retrieve filtering data.
                      It assumes that pbSource is currently pointing to locati-
                      on of a filtering block data.
  Output           :  BYTE * - Location of the next byte after the block.
  Functionality    :  Retrieves filtering block information and copies them
                      into the current object. Advances the reading pointer to
                      the next byte occurring after the block.
  Member of        :  SFILTER_CAN
  Friend of        :  -                                   
  Author(s)        :  Ratnadip Choudhury
  Date Created     :  1.12.2009
  Modification date:  
  Modification By  :  
******************************************************************************/
BYTE* SFILTER_CAN::pbSetConfigData(BYTE* pbTarget)
{
    vClear();

    BYTE* pbTStream = pbTarget;

    pbTStream = this->SFILTER::pbSetConfigData(pbTStream);
    COPY_DATA_2(&m_byIDType, pbTStream, sizeof(m_byIDType));
    COPY_DATA_2(&m_byMsgType, pbTStream, sizeof(m_byMsgType));
    COPY_DATA_2(&m_eChannel, pbTStream, sizeof(m_eChannel));

    return pbTStream;
}

/* Ends SFILTER_CAN */

/* Starts SFILTER_FLEXRAY */

/******************************************************************************
  Function Name    :  SFILTER_FLEXRAY
  Input(s)         :  -
  Output           :  -
  Functionality    :  Standard constructor
  Member of        :  SFILTER_FLEXRAY
  Friend of        :  -                                   
  Author(s)        :  Ratnadip Choudhury
  Date Created     :  1.12.2009
  Modification date:  
  Modification By  :  
******************************************************************************/
SFILTER_FLEXRAY::SFILTER_FLEXRAY()
{
    vClear();
}

/******************************************************************************
  Function Name    :  ~SFILTER_FLEXRAY
  Input(s)         :  -
  Output           :  -
  Functionality    :  Destructor
  Member of        :  SFILTER_FLEXRAY
  Friend of        :  -                                   
  Author(s)        :  Ratnadip Choudhury
  Date Created     :  1.12.2009
  Modification date:  
  Modification By  :  
******************************************************************************/
SFILTER_FLEXRAY::~SFILTER_FLEXRAY()
{
    vClear();
}

/******************************************************************************
  Function Name    :  operator=
  Input(s)         :  const SFILTER_FLEXRAY& RefObj - The source object
  Output           :  SFILTER_FLEXRAY& - The current object reference.
  Functionality    :  Copies a source object by '=' operator overloading.
  Member of        :  SFILTER_FLEXRAY
  Friend of        :  -                                   
  Author(s)        :  Ratnadip Choudhury
  Date Created     :  1.12.2009
  Modification date:  
  Modification By  :  
******************************************************************************/
SFILTER_FLEXRAY& SFILTER_FLEXRAY::operator=(const SFILTER_FLEXRAY& RefObj)
{
    this->SFILTER::operator = (RefObj);

    m_eChannel = RefObj.m_eChannel;

    return *this;
}

/******************************************************************************
  Function Name    :  vClear
  Input(s)         :  void
  Output           :  void
  Functionality    :  Clears information inthe current filtering block
  Member of        :  SFILTER_FLEXRAY
  Friend of        :  -                                   
  Author(s)        :  Ratnadip Choudhury
  Date Created     :  1.12.2009
  Modification date:  
  Modification By  :  
******************************************************************************/
void SFILTER_FLEXRAY::vClear(void)
{
    m_eChannel = FLEXRAY_CHANNEL_AB;
    this->SFILTER::vClear();
}

/******************************************************************************
  Function Name    :  bDoesFrameOccur
  Input(s)         :  psCurrFrame - The frame in question.
  Output           :  TRUE if the frame occurs in the filtering block.
  Functionality    :  Query function to know if this filtering block is 
                      configured for the message entry passed.
  Member of        :  SFILTER_FLEXRAY
  Friend of        :  -                                   
  Author(s)        :  Ratnadip Choudhury
  Date Created     :  1.12.2009
  Modification date:  
  Modification By  :  
******************************************************************************/
BOOL SFILTER_FLEXRAY::bDoesFrameOccur(const void* psCurrFrame) const
{
    BOOL bResult = FALSE;

    SFRAMEINFO_BASIC_FLEXRAY& sCurrFrame = *((SFRAMEINFO_BASIC_FLEXRAY *) psCurrFrame);
    // For Signle Id Filtering
    if (m_ucFilterType == defFILTER_TYPE_SINGLE_ID)
    {
        // Check for Message ID
        if (m_dwMsgIDFrom == sCurrFrame.m_dwFrameID)
        {
            // Check for message Channel
            if ( (FLEXRAY_CHANNEL_AB == m_eChannel) ||
                 (FLEXRAY_CHANNEL_AB == sCurrFrame.m_eChannel) ||
                 (m_eChannel == sCurrFrame.m_eChannel) )
            {
                // Check for message direction
                if ( (DIR_ALL == m_eDrctn) ||
                     (DIR_ALL == sCurrFrame.m_eDrctn) ||
                     (m_eDrctn == sCurrFrame.m_eDrctn) )
				{
					bResult = TRUE;
				}
			}
        }
    }
    // For Range Filter
    else if (m_ucFilterType == defFILTER_TYPE_ID_RANGE)
    {
        // Check for message falling in the Range
        if ( (sCurrFrame.m_dwFrameID >= m_dwMsgIDFrom) &&
             (sCurrFrame.m_dwFrameID <= m_dwMsgIDTo) )
        {
            // Check for message Channel
            if ( (FLEXRAY_CHANNEL_AB == m_eChannel) ||
                 (FLEXRAY_CHANNEL_AB == sCurrFrame.m_eChannel) ||
                 (m_eChannel == sCurrFrame.m_eChannel) )
            {
               // Check for message Direction
                if ( (DIR_ALL == m_eDrctn) ||
                     (DIR_ALL == sCurrFrame.m_eDrctn) ||
                     (m_eDrctn == sCurrFrame.m_eDrctn) )
				{
					bResult = TRUE;
				}
            }
		}
    }
    else
    {
        ASSERT(FALSE);
    }

    return bResult;
}

/******************************************************************************
  Function Name    :  unGetSize
  Input(s)         :  void
  Output           :  UINT - size of the current filtering block
  Functionality    :  Returns in bytes size of the current filtering block.
  Member of        :  SFILTER_FLEXRAY
  Friend of        :  -                                   
  Author(s)        :  Ratnadip Choudhury
  Date Created     :  1.12.2009
  Modification date:  
  Modification By  :  
******************************************************************************/
UINT SFILTER_FLEXRAY::unGetSize(void) const
{
    UINT Result = this->SFILTER::unGetSize();
    Result += sizeof(m_eChannel);

    return Result;
}

/******************************************************************************
  Function Name    :  pbGetConfigData
  Input(s)         :  pbTarget - The target buffer to save filtering data.
                      It assumes that pbTarget is currently pointing to a loca-
                      tion having sufficient space.
  Output           :  BYTE * - Location of the next available buffer.
  Functionality    :  Saves filtering block information of the current object
                      into the target buffer. Advances the writing pointer to
                      the next byte occurring after the written block.
  Member of        :  SFILTER_FLEXRAY
  Friend of        :  -                                   
  Author(s)        :  Ratnadip Choudhury
  Date Created     :  1.12.2009
  Modification date:  
  Modification By  :  
******************************************************************************/
BYTE* SFILTER_FLEXRAY::pbGetConfigData(BYTE* pbTarget) const
{
    BYTE* pbTStream = pbTarget;

    pbTStream = this->SFILTER::pbGetConfigData(pbTStream);
    COPY_DATA(pbTStream, &m_eChannel, sizeof(m_eChannel));

    return pbTStream;
}

/******************************************************************************
  Function Name    :  pbSetConfigData
  Input(s)         :  pbSource - The source buffer to retrieve filtering data.
                      It assumes that pbSource is currently pointing to locati-
                      on of a filtering block data.
  Output           :  BYTE * - Location of the next byte after the block.
  Functionality    :  Retrieves filtering block information and copies them
                      into the current object. Advances the reading pointer to
                      the next byte occurring after the block.
  Member of        :  SFILTER_FLEXRAY
  Friend of        :  -                                   
  Author(s)        :  Ratnadip Choudhury
  Date Created     :  1.12.2009
  Modification date:  
  Modification By  :  
******************************************************************************/
BYTE* SFILTER_FLEXRAY::pbSetConfigData(BYTE* pbTarget)
{
    vClear();

    BYTE* pbTStream = pbTarget;

    pbTStream = this->SFILTER::pbSetConfigData(pbTStream);
    COPY_DATA_2(&m_eChannel, pbTStream, sizeof(m_eChannel));

    return pbTStream;
}

/* Ends SFILTER_FLEXRAY */


/* Starts SFILTER_MCNET */

/******************************************************************************
  Function Name    :  SFILTER_MCNET
  Input(s)         :  -
  Output           :  -
  Functionality    :  Standard constructor
  Member of        :  SFILTER_MCNET
  Friend of        :  -                                   
  Author(s)        :  Ratnadip Choudhury
  Date Created     :  1.12.2009
  Modification date:  
  Modification By  :  
******************************************************************************/
SFILTER_MCNET::SFILTER_MCNET()
{
    vClear();
}

/******************************************************************************
  Function Name    :  ~SFILTER_MCNET
  Input(s)         :  -
  Output           :  -
  Functionality    :  Destructor
  Member of        :  SFILTER_MCNET
  Friend of        :  -                                   
  Author(s)        :  Ratnadip Choudhury
  Date Created     :  1.12.2009
  Modification date:  
  Modification By  :  
******************************************************************************/
SFILTER_MCNET::~SFILTER_MCNET()
{
    vClear();
}

/******************************************************************************
  Function Name    :  operator=
  Input(s)         :  const SFILTER_MCNET& RefObj - The source object
  Output           :  SFILTER_MCNET& - The current object reference.
  Functionality    :  Copies a source object by '=' operator overloading.
  Member of        :  SFILTER_MCNET
  Friend of        :  -                                   
  Author(s)        :  Ratnadip Choudhury
  Date Created     :  1.12.2009
  Modification date:  
  Modification By  :  
******************************************************************************/
SFILTER_MCNET& SFILTER_MCNET::operator=(const SFILTER_MCNET& RefObj)
{
    this->SFILTER::operator = (RefObj);
    return *this;
}

/******************************************************************************
  Function Name    :  vClear
  Input(s)         :  void
  Output           :  void
  Functionality    :  Clears information inthe current filtering block
  Member of        :  SFILTER_MCNET
  Friend of        :  -                                   
  Author(s)        :  Ratnadip Choudhury
  Date Created     :  1.12.2009
  Modification date:  
  Modification By  :  
******************************************************************************/
void SFILTER_MCNET::vClear(void)
{
    this->SFILTER::vClear();
}

/******************************************************************************
  Function Name    :  bDoesFrameOccur
  Input(s)         :  psCurrFrame - The frame in question.
  Output           :  TRUE if the frame occurs in the filtering block.
  Functionality    :  Query function to know if this filtering block is 
                      configured for the message entry passed.
  Member of        :  SFILTER_MCNET
  Friend of        :  -                                   
  Author(s)        :  Ratnadip Choudhury
  Date Created     :  1.12.2009
  Modification date:  
  Modification By  :  
******************************************************************************/
BOOL SFILTER_MCNET::bDoesFrameOccur(const void* psCurrFrame) const
{
    BOOL bResult = FALSE;

    SFRAMEINFO_BASIC_MCNET& sCurrFrame = *((SFRAMEINFO_BASIC_MCNET *) psCurrFrame);
    // For Signle Id Filtering
    if (m_ucFilterType == defFILTER_TYPE_SINGLE_ID)
    {
        // Check for Message ID
        if (m_dwMsgIDFrom == sCurrFrame.m_dwFrameID)
        {
            bResult = TRUE;
        }
    }
    // For Range Filter
    else if (m_ucFilterType == defFILTER_TYPE_ID_RANGE)
    {
        // Check for message falling in the Range
        if ( (sCurrFrame.m_dwFrameID >= m_dwMsgIDFrom) &&
             (sCurrFrame.m_dwFrameID <= m_dwMsgIDTo) )
        {
            bResult = TRUE;
		}
    }
    else
    {
        ASSERT(FALSE);
    }

    return bResult;
}

/******************************************************************************
  Function Name    :  unGetSize
  Input(s)         :  void
  Output           :  UINT - size of the current filtering block
  Functionality    :  Returns in bytes size of the current filtering block.
  Member of        :  SFILTER_MCNET
  Friend of        :  -                                   
  Author(s)        :  Ratnadip Choudhury
  Date Created     :  1.12.2009
  Modification date:  
  Modification By  :  
******************************************************************************/
UINT SFILTER_MCNET::unGetSize(void) const
{
    UINT Result = this->SFILTER::unGetSize();
    return Result;
}

/******************************************************************************
  Function Name    :  pbGetConfigData
  Input(s)         :  pbTarget - The target buffer to save filtering data.
                      It assumes that pbTarget is currently pointing to a loca-
                      tion having sufficient space.
  Output           :  BYTE * - Location of the next available buffer.
  Functionality    :  Saves filtering block information of the current object
                      into the target buffer. Advances the writing pointer to
                      the next byte occurring after the written block.
  Member of        :  SFILTER_MCNET
  Friend of        :  -                                   
  Author(s)        :  Ratnadip Choudhury
  Date Created     :  1.12.2009
  Modification date:  
  Modification By  :  
******************************************************************************/
BYTE* SFILTER_MCNET::pbGetConfigData(BYTE* pbTarget) const
{
    BYTE* pbTStream = pbTarget;
    pbTStream = this->SFILTER::pbGetConfigData(pbTStream);
    return pbTStream;
}

/******************************************************************************
  Function Name    :  pbSetConfigData
  Input(s)         :  pbSource - The source buffer to retrieve filtering data.
                      It assumes that pbSource is currently pointing to locati-
                      on of a filtering block data.
  Output           :  BYTE * - Location of the next byte after the block.
  Functionality    :  Retrieves filtering block information and copies them
                      into the current object. Advances the reading pointer to
                      the next byte occurring after the block.
  Member of        :  SFILTER_MCNET
  Friend of        :  -                                   
  Author(s)        :  Ratnadip Choudhury
  Date Created     :  1.12.2009
  Modification date:  
  Modification By  :  
******************************************************************************/
BYTE* SFILTER_MCNET::pbSetConfigData(BYTE* pbTarget)
{
    vClear();
    BYTE* pbTStream = pbTarget;
    pbTStream = this->SFILTER::pbSetConfigData(pbTStream);
    return pbTStream;
}
/* Ends SFILTER_MCNET */


/* Starts SFILTER_J1939 */

/******************************************************************************
  Function Name    :  SFILTER_J1939
  Input(s)         :  -
  Output           :  -
  Functionality    :  Standard constructor
  Member of        :  SFILTER_J1939
  Friend of        :  -                                   
  Author(s)        :  Ratnadip Choudhury
  Date Created     :  30.11.2010
  Modification date:  
  Modification By  :  
******************************************************************************/
SFILTER_J1939::SFILTER_J1939()
{
    vClear();
}

/******************************************************************************
  Function Name    :  ~SFILTER_J1939
  Input(s)         :  -
  Output           :  -
  Functionality    :  Destructor
  Member of        :  SFILTER_J1939
  Friend of        :  -                                   
  Author(s)        :  Ratnadip Choudhury
  Date Created     :  30.11.2010
  Modification date:  
  Modification By  :  
******************************************************************************/
SFILTER_J1939::~SFILTER_J1939()
{
    vClear();
}

/******************************************************************************
  Function Name    :  operator=
  Input(s)         :  const SFILTER_J1939& RefObj - The source object
  Output           :  SFILTER_J1939& - The current object reference.
  Functionality    :  Copies a source object by '=' operator overloading.
  Member of        :  SFILTER_J1939
  Friend of        :  -                                   
  Author(s)        :  Ratnadip Choudhury
  Date Created     :  30.11.2010
  Modification date:  
  Modification By  :  
******************************************************************************/
SFILTER_J1939& SFILTER_J1939::operator=(const SFILTER_J1939& RefObj)
{
    this->SFILTER::operator = (RefObj);
    return *this;
}

/******************************************************************************
  Function Name    :  vClear
  Input(s)         :  void
  Output           :  void
  Functionality    :  Clears information inthe current filtering block
  Member of        :  SFILTER_J1939
  Friend of        :  -                                   
  Author(s)        :  Ratnadip Choudhury
  Date Created     :  30.11.2010
  Modification date:  
  Modification By  :  
******************************************************************************/
void SFILTER_J1939::vClear(void)
{
    this->SFILTER::vClear();
}

/******************************************************************************
  Function Name    :  bDoesFrameOccur
  Input(s)         :  psCurrFrame - The frame in question.
  Output           :  TRUE if the frame occurs in the filtering block.
  Functionality    :  Query function to know if this filtering block is 
                      configured for the message entry passed.
  Member of        :  SFILTER_J1939
  Friend of        :  -                                   
  Author(s)        :  Ratnadip Choudhury
  Date Created     :  30.11.2010
  Modification date:  
  Modification By  :  
******************************************************************************/
BOOL SFILTER_J1939::bDoesFrameOccur(const void* psCurrFrame) const
{
    BOOL bResult = FALSE;

    SFRAMEINFO_BASIC_J1939& sCurrFrame = *((SFRAMEINFO_BASIC_J1939 *) psCurrFrame);
    // For Signle Id Filtering
    if (m_ucFilterType == defFILTER_TYPE_SINGLE_ID)
    {
        // Check for Message ID
        if (m_dwMsgIDFrom == sCurrFrame.m_dwPGN)
        {
            bResult = TRUE;
        }
    }
    // For Range Filter
    else if (m_ucFilterType == defFILTER_TYPE_ID_RANGE)
    {
        // Check for message falling in the Range
        if ( (sCurrFrame.m_dwPGN >= m_dwMsgIDFrom) &&
             (sCurrFrame.m_dwPGN <= m_dwMsgIDTo) )
        {
            bResult = TRUE;
		}
    }
    else
    {
        ASSERT(FALSE);
    }

    return bResult;
}

/******************************************************************************
  Function Name    :  unGetSize
  Input(s)         :  void
  Output           :  UINT - size of the current filtering block
  Functionality    :  Returns in bytes size of the current filtering block.
  Member of        :  SFILTER_J1939
  Friend of        :  -                                   
  Author(s)        :  Ratnadip Choudhury
  Date Created     :  30.11.2010
  Modification date:  
  Modification By  :  
******************************************************************************/
UINT SFILTER_J1939::unGetSize(void) const
{
    UINT Result = this->SFILTER::unGetSize();
    return Result;
}

/******************************************************************************
  Function Name    :  pbGetConfigData
  Input(s)         :  pbTarget - The target buffer to save filtering data.
                      It assumes that pbTarget is currently pointing to a loca-
                      tion having sufficient space.
  Output           :  BYTE * - Location of the next available buffer.
  Functionality    :  Saves filtering block information of the current object
                      into the target buffer. Advances the writing pointer to
                      the next byte occurring after the written block.
  Member of        :  SFILTER_J1939
  Friend of        :  -                                   
  Author(s)        :  Ratnadip Choudhury
  Date Created     :  30.11.2010
  Modification date:  
  Modification By  :  
******************************************************************************/
BYTE* SFILTER_J1939::pbGetConfigData(BYTE* pbTarget) const
{
    BYTE* pbTStream = pbTarget;
    pbTStream = this->SFILTER::pbGetConfigData(pbTStream);
    return pbTStream;
}

/******************************************************************************
  Function Name    :  pbSetConfigData
  Input(s)         :  pbSource - The source buffer to retrieve filtering data.
                      It assumes that pbSource is currently pointing to locati-
                      on of a filtering block data.
  Output           :  BYTE * - Location of the next byte after the block.
  Functionality    :  Retrieves filtering block information and copies them
                      into the current object. Advances the reading pointer to
                      the next byte occurring after the block.
  Member of        :  SFILTER_J1939
  Friend of        :  -                                   
  Author(s)        :  Ratnadip Choudhury
  Date Created     :  30.11.2010
  Modification date:  
  Modification By  :  
******************************************************************************/
BYTE* SFILTER_J1939::pbSetConfigData(BYTE* pbTarget)
{
    vClear();
    BYTE* pbTStream = pbTarget;
    pbTStream = this->SFILTER::pbSetConfigData(pbTStream);
    return pbTStream;
}
/* Ends SFILTER_J1939 */

/* Starts SFILTERSET / tagFilterSet */
/******************************************************************************
  Function Name    :  tagFilterSet
  Input(s)         :  -
  Output           :  -
  Functionality    :  Standard constructor
  Member of        :  tagFilterSet
  Friend of        :  -                                   
  Author(s)        :  Ratnadip Choudhury
  Date Created     :  1.12.2009
  Modification date:  
  Modification By  :  
******************************************************************************/
tagFilterSet::tagFilterSet()
{
    m_sFilterName.vClear();
    m_bEnabled = TRUE;
    m_eCurrBus = BUS_INVALID;
    m_ushFilters = 0;
    m_psFilterInfo = NULL;
}

/******************************************************************************
  Function Name    :  ~tagFilterSet
  Input(s)         :  -
  Output           :  -
  Functionality    :  Destructor
  Member of        :  tagFilterSet
  Friend of        :  -                                   
  Author(s)        :  Ratnadip Choudhury
  Date Created     :  1.12.2009
  Modification date:  
  Modification By  :  
******************************************************************************/
tagFilterSet::~tagFilterSet()
{
    vClear();
}

/******************************************************************************
  Function Name    :  vClear
  Input(s)         :  void
  Output           :  void
  Functionality    :  Clears information inthe current filtering block
  Member of        :  tagFilterSet
  Friend of        :  -                                   
  Author(s)        :  Ratnadip Choudhury
  Date Created     :  1.12.2009
  ****************************************************************************/
void tagFilterSet::vClear(void)
{
    m_sFilterName.vClear();
    m_ushFilters = 0;
    if (NULL != m_psFilterInfo)
    {
        switch (m_eCurrBus)
        {
            case CAN:
            {
                PSFILTER_CAN psFilterCAN 
                                   = static_cast<PSFILTER_CAN>(m_psFilterInfo);
                delete[] psFilterCAN;
            }
            break;
            case FLEXRAY:
            {
                PSFILTER_FLEXRAY psFilterFlex 
                               = static_cast<PSFILTER_FLEXRAY>(m_psFilterInfo);
                delete[] psFilterFlex;
            }
            break;
            case MCNET:
            {
                PSFILTER_MCNET psFilterMCNet 
                                   = static_cast<PSFILTER_MCNET>(m_psFilterInfo);
                delete[] psFilterMCNet;
            }
            break;
            case J1939:
            {
                PSFILTER_J1939 psFilterJ1939 
                                   = static_cast<PSFILTER_J1939>(m_psFilterInfo);
                delete[] psFilterJ1939;
            }
            break;
            default: ASSERT(FALSE);
        }
        m_psFilterInfo = NULL;
    }
    m_eCurrBus = BUS_INVALID;
}

/******************************************************************************
  Function Name    :  bClone
  Input(s)         :  RefObj - The source object.
  Output           :  true if cloning operation is successful, else false.
  Functionality    :  Clones a filtering object to initialise the current one.
  Member of        :  tagFilterSet
  Friend of        :  -                                   
  Author(s)        :  Ratnadip Choudhury
  Date Created     :  1.12.2009
******************************************************************************/
bool tagFilterSet::bClone(const tagFilterSet& RefObj)
{
    vClear(); // First clear everything

    bool bResult = false;

    m_sFilterName = RefObj.m_sFilterName;
    m_bEnabled = RefObj.m_bEnabled;
    m_eCurrBus = RefObj.m_eCurrBus;
    m_ushFilters = RefObj.m_ushFilters;

    if (m_ushFilters > 0)
    {
        switch (m_eCurrBus)
        {
            case CAN:
            {
                m_psFilterInfo = new SFILTER_CAN[m_ushFilters];
                if (NULL != m_psFilterInfo)
                {
                    bResult = true;
                    for (USHORT i = 0; i < m_ushFilters; i++)
                    {
                        SFILTER_CAN* pDestObj = ((SFILTER_CAN *)m_psFilterInfo) + i;
                        SFILTER_CAN* pSrcObj = ((SFILTER_CAN *)RefObj.m_psFilterInfo) + i;
                        *pDestObj = *pSrcObj;
                    }
                }
                else
                {
                    m_ushFilters = 0;
                }
            }
            break;
            case FLEXRAY:
            {
                m_psFilterInfo = new SFILTER_FLEXRAY[m_ushFilters];
                if (NULL != m_psFilterInfo)
                {
                    bResult = true;
                    for (USHORT i = 0; i < m_ushFilters; i++)
                    {
                        SFILTER_FLEXRAY* pDestObj = ((SFILTER_FLEXRAY *) m_psFilterInfo) + i;
                        SFILTER_FLEXRAY* pSrcObj = ((SFILTER_FLEXRAY *) RefObj.m_psFilterInfo) + i;
                        *pDestObj = *pSrcObj;
                    }
                }
                else
                {
                    m_ushFilters = 0;
                }
            }
            break;
            case MCNET:
            {
                m_psFilterInfo = new SFILTER_MCNET[m_ushFilters];
                if (NULL != m_psFilterInfo)
                {
                    bResult = true;
                    for (USHORT i = 0; i < m_ushFilters; i++)
                    {
                        SFILTER_MCNET* pDestObj = ((SFILTER_MCNET *)m_psFilterInfo) + i;
                        SFILTER_MCNET* pSrcObj = ((SFILTER_MCNET *)RefObj.m_psFilterInfo) + i;
                        *pDestObj = *pSrcObj;
                    }
                }
                else
                {
                    m_ushFilters = 0;
                }
            }
            break;
            case J1939:
            {
                m_psFilterInfo = new SFILTER_J1939[m_ushFilters];
                if (NULL != m_psFilterInfo)
                {
                    bResult = true;
                    for (USHORT i = 0; i < m_ushFilters; i++)
                    {
                        SFILTER_J1939* pDestObj = ((SFILTER_J1939 *)m_psFilterInfo) + i;
                        SFILTER_J1939* pSrcObj = ((SFILTER_J1939 *)RefObj.m_psFilterInfo) + i;
                        *pDestObj = *pSrcObj;
                    }
                }
                else
                {
                    m_ushFilters = 0;
                }
            }
            break;
            default: ASSERT(FALSE);
        }
    }
    else
    {
        m_psFilterInfo = NULL;
        bResult = true;
    }

    return bResult;
}

/******************************************************************************
  Function Name    :  unGetSize
  Input(s)         :  void
  Output           :  UINT - size of the current filtering block
  Functionality    :  Returns in bytes size of the current filtering block.
  Member of        :  tagFilterSet
  Friend of        :  -                                   
  Author(s)        :  Ratnadip Choudhury
  Date Created     :  1.12.2009
******************************************************************************/
UINT tagFilterSet::unGetSize(void) const
{
    UINT Result = sizeof(m_sFilterName) + sizeof(m_bEnabled) + 
                  sizeof(m_eCurrBus) + sizeof(m_ushFilters);

    switch (m_eCurrBus)
    {
        case CAN:
        {
            for (UINT i = 0; i < m_ushFilters; i++)
            {

                Result += (((SFILTER_CAN*) m_psFilterInfo) + i)->unGetSize();
            }
        }
        break;
        case FLEXRAY:
        {
            for (UINT i = 0; i < m_ushFilters; i++)
            {

                Result += (((SFILTER_FLEXRAY*) m_psFilterInfo) + i)->unGetSize();
            }
        }
        break;
        case MCNET:
        {
            for (UINT i = 0; i < m_ushFilters; i++)
            {

                Result += (((SFILTER_MCNET*) m_psFilterInfo) + i)->unGetSize();
            }
        }
        break;
        case J1939:
        {
            for (UINT i = 0; i < m_ushFilters; i++)
            {

                Result += (((SFILTER_J1939*) m_psFilterInfo) + i)->unGetSize();
            }
        }
        break;
        default:
        {
            ASSERT(FALSE);            
        }
        break;
    }

    return Result;
}

/******************************************************************************
  Function Name    :  pbGetConfigData
  Input(s)         :  pbTarget - The target buffer to save filtering data.
                      It assumes that pbTarget is currently pointing to a loca-
                      tion having sufficient space.
  Output           :  BYTE * - Location of the next available buffer.
  Functionality    :  Saves filtering block information of the current object
                      into the target buffer. Advances the writing pointer to
                      the next byte occurring after the written block.
  Member of        :  tagFilterSet
  Friend of        :  -                                   
  Author(s)        :  Ratnadip Choudhury
  Date Created     :  1.12.2009
******************************************************************************/
BYTE* tagFilterSet::pbGetConfigData(BYTE* pbTarget) const
{
    BYTE* pbTStream = pbTarget;

    pbTStream = m_sFilterName.pbGetConfigData(pbTStream);
    COPY_DATA(pbTStream, &m_bEnabled, sizeof(m_bEnabled));
    COPY_DATA(pbTStream, &m_eCurrBus, sizeof(m_eCurrBus));
    COPY_DATA(pbTStream, &m_ushFilters, sizeof(m_ushFilters));

    for (USHORT i = 0; i < m_ushFilters; i++)
    {
        switch (m_eCurrBus)
        {
            case CAN:
            {
                pbTStream = (((SFILTER_CAN*) m_psFilterInfo) + i)->pbGetConfigData(pbTStream);
            }
            break;
            case FLEXRAY:
            {
                pbTStream = (((SFILTER_FLEXRAY*) m_psFilterInfo) + i)->pbGetConfigData(pbTStream);
            }
            break;
            case MCNET:
            {
                pbTStream = (((SFILTER_MCNET*) m_psFilterInfo) + i)->pbGetConfigData(pbTStream);
            }
            break;
            case J1939:
            {
                pbTStream = (((SFILTER_J1939*) m_psFilterInfo) + i)->pbGetConfigData(pbTStream);
            }
            break;
            default:
            {
                ASSERT(FALSE);
            }
            break;
        }
    }

    return pbTStream;
}

/******************************************************************************
  Function Name    :  pbSetConfigData
  Input(s)         :  pbSource - The source buffer to retrieve filtering data.
                      It assumes that pbSource is currently pointing to locati-
                      on of a filtering block data.
  Output           :  BYTE * - Location of the next byte after the block.
  Functionality    :  Retrieves filtering block information and copies them into
                      the current object. Advances the reading pointer to the
                      next byte occurring after the block.
  Member of        :  tagFilterSet
  Friend of        :  -                                   
  Author(s)        :  Ratnadip Choudhury
  Date Created     :  1.12.2009
******************************************************************************/
BYTE* tagFilterSet::pbSetConfigData(BYTE* pbTarget, bool& Result)
{
    vClear(); // First clear everything

    BYTE* pbTStream = pbTarget;
    Result = true;

    pbTStream = m_sFilterName.pbSetConfigData(pbTStream);
    COPY_DATA_2(&m_bEnabled, pbTStream, sizeof(m_bEnabled));
    COPY_DATA_2(&m_eCurrBus, pbTStream, sizeof(m_eCurrBus));
    COPY_DATA_2(&m_ushFilters, pbTStream, sizeof(m_ushFilters));

    if (m_ushFilters > 0)
    {
        switch (m_eCurrBus)
        {
            case CAN: m_psFilterInfo = new SFILTER_CAN[m_ushFilters]; break;
            case FLEXRAY: m_psFilterInfo = new SFILTER_FLEXRAY[m_ushFilters]; break;
            case MCNET: m_psFilterInfo = new SFILTER_MCNET[m_ushFilters]; break;
            case J1939: m_psFilterInfo = new SFILTER_J1939[m_ushFilters]; break;
            default: ASSERT(FALSE);
        }

        if (NULL != m_psFilterInfo)
        {
            switch (m_eCurrBus)
            {
                case CAN:
                {

                    for (USHORT i = 0; i < m_ushFilters; i++)
                    {
                        pbTStream = (((SFILTER_CAN*) m_psFilterInfo) + i)->pbSetConfigData(pbTStream);
                    }
                }
                break;
                case FLEXRAY:
                {

                    for (USHORT i = 0; i < m_ushFilters; i++)
                    {
                        pbTStream = (((SFILTER_FLEXRAY*) m_psFilterInfo) + i)->pbSetConfigData(pbTStream);
                    }
                }
                break;
                case MCNET:
                {
                    for (USHORT i = 0; i < m_ushFilters; i++)
                    {
                        pbTStream = (((SFILTER_MCNET*) m_psFilterInfo) + i)->pbSetConfigData(pbTStream);
                    }
                }
                break;
                case J1939:
                {
                    for (USHORT i = 0; i < m_ushFilters; i++)
                    {
                        pbTStream = (((SFILTER_J1939*) m_psFilterInfo) + i)->pbSetConfigData(pbTStream);
                    }
                }
                break;
                default:
                {
                    ASSERT(FALSE);
                }
                break;
            }
        }
        else
        {
            UINT unFilterSize = 0;
            switch (m_eCurrBus)
            {
                case CAN: unFilterSize = sizeof(SFILTER_CAN); break;
                case FLEXRAY: unFilterSize = sizeof(SFILTER_FLEXRAY); break;
                case MCNET: unFilterSize = sizeof(SFILTER_MCNET); break;
                case J1939: unFilterSize = sizeof(SFILTER_J1939); break;
                default: ASSERT(FALSE);
            }
            pbTStream += m_ushFilters * unFilterSize;
            m_ushFilters = 0;
            Result = false;
        }
    }

    return pbTStream;
}

tagFilterSet* tagFilterSet::psGetFilterSetPointer(tagFilterSet* psSet, UINT Count, TCHAR* acFilterName)
{    
    for (UINT i = 0; i < Count; i++)
    {
        tagFilterSet* psTemp = psSet + i;
        if ((psTemp != NULL) &&(_tcscmp(psTemp->m_sFilterName.m_acFilterName, acFilterName) == 0))
        {
            return psTemp;
        }
    }
    return NULL;
}

/* Ends SFILTERSET / tagFilterSet */
