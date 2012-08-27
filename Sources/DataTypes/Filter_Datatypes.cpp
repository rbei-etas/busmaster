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

const int SIZE_CHAR = sizeof(char);

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
    _tcsset(m_acFilterName, L'\0');
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

    COPY_DATA(pbTStream, m_acFilterName, LENGTH_FILTERNAME * SIZE_CHAR);
    COPY_DATA(pbTStream, &m_bFilterType, sizeof(m_bFilterType));

    return pbTStream;
}

void tagFilterName::pbGetConfigData(xmlNodePtr pFilterTag) const
{
    /*BYTE* pbTStream = pbTarget;

    COPY_DATA(pbTStream, m_acFilterName, LENGTH_FILTERNAME * SIZE_CHAR);
    COPY_DATA(pbTStream, &m_bFilterType, sizeof(m_bFilterType));

    return pbTStream;*/

    /*xmlNodePtr pFilterTag = xmlNewNode(NULL, BAD_CAST DEF_FILTER);
    xmlAddChild(pxmlNodePtr, pFilterTag);*/

    // Adding Filter Name to the xml
    xmlNodePtr pFilterNameNodePtr = xmlNewChild(pFilterTag, NULL, BAD_CAST DEF_NAME, BAD_CAST m_acFilterName);
    xmlAddChild(pFilterTag, pFilterNameNodePtr);

    const char* omstrFilterType = "";

    // Getting the Filter Type
    if(m_bFilterType == 1)
    {
        omstrFilterType = "PASS";
    }
    else
    {
        omstrFilterType = "STOP";
    }


    // Adding Filter Type to the xml
    xmlNodePtr pFilterTypeNodePtr = xmlNewChild(pFilterTag, NULL, BAD_CAST DEF_TYPE, BAD_CAST omstrFilterType);
    xmlAddChild(pFilterTag, pFilterTypeNodePtr);
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

    COPY_DATA_2(m_acFilterName, pbTStream, LENGTH_FILTERNAME * SIZE_CHAR);
    COPY_DATA_2(&m_bFilterType, pbTStream, sizeof(m_bFilterType));

    return pbTStream;
}

// PTV XML
void tagFilterName::pbSetConfigData(xmlNodePtr pNodePtr, xmlDocPtr xmlConfigFiledoc)
{
    pNodePtr = pNodePtr->xmlChildrenNode;

    while(pNodePtr != NULL)
    {
        if((!xmlStrcmp(pNodePtr->name, (const xmlChar*)"Filter")))
        {
            char* ptext = (char*)xmlNodeListGetString(xmlConfigFiledoc, pNodePtr->xmlChildrenNode, 1);
            if(NULL != ptext)
            {
                if(ptext != NULL)
                {
                    strcpy_s(m_acFilterName, 128, ptext);
                }

                xmlFree(ptext);
            }
        }
        pNodePtr = pNodePtr->next;
    }

    // PTV TODO

    /*BYTE* pbTStream = pbTarget;

    COPY_DATA_2(m_acFilterName, pbTStream, LENGTH_FILTERNAME * SIZE_CHAR);
    COPY_DATA_2(&m_bFilterType, pbTStream, sizeof(m_bFilterType));

    return pbTStream;*/
}
// PTV XML
INT tagFilterName::nSetXMLConfigData(xmlNodePtr pFilter)
{
    INT nRetVal = S_OK;
    xmlNodePtr pTempFilter = pFilter->xmlChildrenNode;
    m_acFilterName[0] = '\0';
    m_bFilterType = FALSE;
    while (pTempFilter != NULL)
    {
        if ((!xmlStrcmp(pTempFilter->name, (const xmlChar*)"Name")))
        {
            char* pcTemp = (char*)xmlNodeListGetString(pTempFilter->doc, pTempFilter->xmlChildrenNode, 1);
            if(pcTemp != NULL)
            {
                strcpy_s(m_acFilterName, 128, pcTemp);
            }
        }
        if ((!xmlStrcmp(pTempFilter->name, (const xmlChar*)"Type")))
        {
            m_bFilterType = nFilterType((char*)xmlNodeListGetString(pTempFilter->doc, pTempFilter->xmlChildrenNode, 1));
        }
        pTempFilter = pTempFilter->next;
    }
    if( strlen(m_acFilterName ) == 0 )
    {
        UINT unRand = time(NULL);
        sprintf_s(m_acFilterName, 128, "Filter_%u", (UINT)time(NULL));
        nRetVal = S_FALSE;
    }
    return nRetVal;
}

BOOL tagFilterName::nFilterType(string strFilteType)
{
    m_bFilterType = FALSE;
    if(strFilteType == "PASS")
    {
        m_bFilterType = TRUE;
    }
    return m_bFilterType;
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

// PTV XML
void tagSFILTER::pbGetConfigData(xmlNodePtr pxmlNodePtr) const
{

    CString omStartId = "";
    const char* omcStartId;

    omStartId.Format("%d", m_dwMsgIDFrom);

    omcStartId = omStartId;

    const char* omcStpId;

    CString omStpId = "";
    omStpId.Format("%d", m_dwMsgIDTo);

    omcStpId = omStpId;

    CString omstrDir = "ALL";

    if(m_eDrctn == DIR_RX)
    {
        omstrDir = "Rx";
    }
    else if(m_eDrctn == DIR_TX)
    {
        omstrDir = "Tx";
    }


    xmlNodePtr pStrtIdPtr = xmlNewChild(pxmlNodePtr, NULL, BAD_CAST DEF_IDFROM, BAD_CAST omcStartId);
    xmlAddChild(pxmlNodePtr, pStrtIdPtr);

    xmlNodePtr pStpIdPtr = xmlNewChild(pxmlNodePtr, NULL, BAD_CAST DEF_IDTO, BAD_CAST omcStpId);
    xmlAddChild(pxmlNodePtr, pStpIdPtr);

    xmlNodePtr pDirPtr = xmlNewChild(pxmlNodePtr, NULL, BAD_CAST DEF_DIRECTION, BAD_CAST omstrDir.GetBufferSetLength(omstrDir.GetLength()));
    xmlAddChild(pxmlNodePtr, pDirPtr);

    /*BYTE* pbTStream = pbTarget;

    COPY_DATA(pbTStream, &m_ucFilterType, sizeof(m_ucFilterType));
    COPY_DATA(pbTStream, &m_dwMsgIDFrom, sizeof(m_dwMsgIDFrom));
    COPY_DATA(pbTStream, &m_dwMsgIDTo, sizeof(m_dwMsgIDTo));
    COPY_DATA(pbTStream, &m_eDrctn, sizeof(m_eDrctn));

    return pbTStream;*/
}
// PTV XML
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
eDirection tagSFILTER::eGetMsgDirection(string strDirection)
{
    eDirection eDir = DIR_ALL;
    if(strDirection == "Tx")
    {
        eDir = DIR_TX;
    }
    if(strDirection == "Rx")
    {
        eDir = DIR_RX;
    }
    return eDir;
}

INT tagSFILTER::nSetXMLConfigData(xmlNodePtr pNodePtr)
{
    INT nRetValue = S_OK;
    m_dwMsgIDFrom = 0;
    m_dwMsgIDTo = 0;
    m_eDrctn = DIR_ALL;
    while (pNodePtr != NULL)
    {
        if ((!xmlStrcmp(pNodePtr->name, (const xmlChar*)"IdFrom")))
        {
            if( NULL != pNodePtr->xmlChildrenNode )
            {
                m_dwMsgIDFrom = atoi((char*)xmlNodeListGetString(pNodePtr->doc, pNodePtr->xmlChildrenNode, 1)); //single nodee);
            }
        }
        if ((!xmlStrcmp(pNodePtr->name, (const xmlChar*)"IdTo")))
        {
            if( NULL != pNodePtr->xmlChildrenNode )
            {
                m_dwMsgIDTo = atoi((char*)xmlNodeListGetString(pNodePtr->doc, pNodePtr->xmlChildrenNode, 1));
            }
        }
        if ((!xmlStrcmp(pNodePtr->name, (const xmlChar*)"Direction")))
        {
            if( NULL != pNodePtr->xmlChildrenNode )
            {
                m_eDrctn = eGetMsgDirection((char*)xmlNodeListGetString(pNodePtr->doc, pNodePtr->xmlChildrenNode, 1));
            }
        }
        pNodePtr = pNodePtr->next;
    }

    m_ucFilterType = defFILTER_TYPE_ID_RANGE;
    if(m_dwMsgIDFrom == m_dwMsgIDTo)
    {
        m_ucFilterType = defFILTER_TYPE_SINGLE_ID;
    }
    return nRetValue;
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

    SFRAMEINFO_BASIC_CAN& sCurrFrame = *((SFRAMEINFO_BASIC_CAN*) psCurrFrame);

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

// PTV XML
void SFILTER_CAN::pbGetConfigData(xmlNodePtr pNodePtr) const
{
    xmlNodePtr pFltrMsgPtr = xmlNewNode(NULL, BAD_CAST DEF_FILTER_MESSAGE);
    xmlAddChild(pNodePtr, pFltrMsgPtr);

    this->SFILTER::pbGetConfigData(pFltrMsgPtr);

    CString omstrMsgType = "";
    CString omstrIdType = "";

    if(m_byIDType == TYPE_ID_CAN_EXTENDED)
    {
        omstrIdType = "EXT";
    }
    else if(m_byIDType == TYPE_ID_CAN_ALL)
    {
        omstrIdType = "ALL";
    }
    else if(m_byIDType == TYPE_ID_CAN_NONE)
    {
        omstrIdType = "NONE";
    }
    else
    {
        omstrIdType = "STD";
    }

    if(m_byMsgType == TYPE_MSG_CAN_RTR)
    {
        omstrMsgType = "RTR";
    }
    else if(m_byMsgType == TYPE_MSG_CAN_ALL)
    {
        omstrMsgType = "ALL";
    }
    else if(m_byMsgType == TYPE_MSG_CAN_NONE)
    {
        omstrMsgType = "NONE";
    }
    else
    {
        omstrMsgType = "NONRTR";
    }
    xmlNodePtr pIdTypePtr = xmlNewChild(pFltrMsgPtr, NULL, BAD_CAST DEF_IDTYPE, BAD_CAST omstrIdType.GetBuffer(omstrIdType.GetLength()));
    xmlAddChild(pFltrMsgPtr, pIdTypePtr);

    xmlNodePtr pMsgPtr = xmlNewChild(pFltrMsgPtr, NULL, BAD_CAST DEF_MSGTYPE, BAD_CAST omstrMsgType.GetBuffer(omstrMsgType.GetLength()));
    xmlAddChild(pFltrMsgPtr, pMsgPtr);

    CString omStrChannel = "";

    omStrChannel.Format("%d", m_eChannel);

    xmlNodePtr pChnlPtr = xmlNewChild(pFltrMsgPtr, NULL, BAD_CAST DEF_CHANNEL, BAD_CAST omStrChannel.GetBuffer(omStrChannel.GetLength()));
    xmlAddChild(pFltrMsgPtr, pChnlPtr);

    /* COPY_DATA(pbTStream, &m_byIDType, sizeof(m_byIDType));
     COPY_DATA(pbTStream, &m_byMsgType, sizeof(m_byMsgType));
     COPY_DATA(pbTStream, &m_eChannel, sizeof(m_eChannel));*/

    //return pbTStream;
}
void SFILTER_CAN::pbSetConfigData(xmlNodePtr xmlNodePtr)
{
    vClear();

    /*BYTE* pbTStream = pbTarget;

    pbTStream = this->SFILTER::pbSetConfigData(pbTStream);
    COPY_DATA_2(&m_byIDType, pbTStream, sizeof(m_byIDType));
    COPY_DATA_2(&m_byMsgType, pbTStream, sizeof(m_byMsgType));
    COPY_DATA_2(&m_eChannel, pbTStream, sizeof(m_eChannel));*/

    /*  return pbTStream;*/
}
// PTV XML
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
INT SFILTER_CAN::nGetIDType(string strIDType)
{
    int nId = TYPE_ID_CAN_ALL;
    if(strIDType == "STD")
    {
        nId = TYPE_ID_CAN_STANDARD;
    }
    if(strIDType == "EXT")
    {
        nId = TYPE_ID_CAN_EXTENDED;
    }
    return nId;
}
INT SFILTER_CAN::nGetMsgType(string strMsgType)
{
    int nId = TYPE_MSG_CAN_ALL;
    if(strMsgType == "RTR")
    {
        nId = TYPE_MSG_CAN_RTR;
    }
    if(strMsgType == "NONRTR")
    {
        nId = TYPE_MSG_CAN_NON_RTR;
    }
    return nId;
}

INT SFILTER_CAN::nSetXMLConfigData(xmlNodePtr pNodePtr)
{
    int nRetValue = S_OK;
    vClear();

    nRetValue = this->SFILTER::nSetXMLConfigData(pNodePtr);
    //<IDType>NONRTR</IDType>
    //<MsgType>EXT</MsgType>
    //<Channel>2</Channel>
    string strTemp;
    m_byMsgType = TYPE_MSG_CAN_ALL;
    m_byIDType = TYPE_ID_CAN_ALL;
    m_eChannel = 0;
    while (pNodePtr != NULL)
    {
        if ((!xmlStrcmp(pNodePtr->name, (const xmlChar*)"IDType")))
        {
            if ( NULL != pNodePtr->xmlChildrenNode )
            {
                strTemp = (char*)xmlNodeListGetString(pNodePtr->doc, pNodePtr->xmlChildrenNode, 1); //single node
                m_byIDType  =  nGetIDType(strTemp);
            }
        }
        else if ((!xmlStrcmp(pNodePtr->name, (const xmlChar*)"MsgType")))
        {
            if ( NULL != pNodePtr->xmlChildrenNode )
            {
                strTemp = (char*)xmlNodeListGetString(pNodePtr->doc, pNodePtr->xmlChildrenNode, 1);
                m_byMsgType  = nGetMsgType(strTemp);
            }
        }
        else if ((!xmlStrcmp(pNodePtr->name, (const xmlChar*)"Channel")))
        {
            if ( NULL != pNodePtr->xmlChildrenNode )
            {
                m_eChannel = atoi((char*)xmlNodeListGetString(pNodePtr->doc, pNodePtr->xmlChildrenNode, 1));
                if( m_eChannel < 0 || m_eChannel > CHANNEL_ALLOWED )
                {
                    m_eChannel = 0;
                }
            }
        }
        pNodePtr = pNodePtr->next;
    }
    return nRetValue;
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

    SFRAMEINFO_BASIC_FLEXRAY& sCurrFrame = *((SFRAMEINFO_BASIC_FLEXRAY*) psCurrFrame);
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

// PTV XML
void SFILTER_FLEXRAY::pbGetConfigData(xmlNodePtr pNodePtr) const
{
    this->SFILTER::pbGetConfigData(pNodePtr);
    /* COPY_DATA(pbTStream, &m_eChannel, sizeof(m_eChannel));*/

    //return pbTStream;
}
// PTV XML
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

    SFRAMEINFO_BASIC_MCNET& sCurrFrame = *((SFRAMEINFO_BASIC_MCNET*) psCurrFrame);
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

void SFILTER_MCNET::pbGetConfigData(xmlNodePtr pxmlNodePtr) const
{
    //BYTE* pbTStream = pbTarget;
    this->SFILTER::pbGetConfigData(pxmlNodePtr);
    //return pbTStream;
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

    SFRAMEINFO_BASIC_J1939& sCurrFrame = *((SFRAMEINFO_BASIC_J1939*) psCurrFrame);
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

// PTV XML
void SFILTER_J1939::pbGetConfigData(xmlNodePtr pNodePtr) const
{
    //BYTE* pbTStream = pbTarget;
    this->SFILTER::pbGetConfigData(pNodePtr);
    //return pbTStream;
}
// PTV XML
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
            default:
                ASSERT(FALSE);
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
                        SFILTER_CAN* pDestObj = ((SFILTER_CAN*)m_psFilterInfo) + i;
                        SFILTER_CAN* pSrcObj = ((SFILTER_CAN*)RefObj.m_psFilterInfo) + i;
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
                        SFILTER_FLEXRAY* pDestObj = ((SFILTER_FLEXRAY*) m_psFilterInfo) + i;
                        SFILTER_FLEXRAY* pSrcObj = ((SFILTER_FLEXRAY*) RefObj.m_psFilterInfo) + i;
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
                        SFILTER_MCNET* pDestObj = ((SFILTER_MCNET*)m_psFilterInfo) + i;
                        SFILTER_MCNET* pSrcObj = ((SFILTER_MCNET*)RefObj.m_psFilterInfo) + i;
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
                        SFILTER_J1939* pDestObj = ((SFILTER_J1939*)m_psFilterInfo) + i;
                        SFILTER_J1939* pSrcObj = ((SFILTER_J1939*)RefObj.m_psFilterInfo) + i;
                        *pDestObj = *pSrcObj;
                    }
                }
                else
                {
                    m_ushFilters = 0;
                }
            }
            break;
            default:
                ASSERT(FALSE);
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

void tagFilterSet::pbGetConfigData(xmlNodePtr pNodePtr) const
{
    //BYTE* pbTStream = pbTarget;

    xmlNodePtr pNodeCanFilterPtr;
    xmlNodePtr pNodeMCNEtFilterPtr;
    xmlNodePtr pNodeJ1939FilterPtr;
    xmlNodePtr pNodeFLEXRAYFilterPtr;
    xmlNodePtr pNodeLINFilterPtr;
    xmlNodePtr pNodeMOSTFilterPtr;

    xmlNodePtr pFilterTag;


    if(m_eCurrBus == MCNET)
    {
        // Adding Filter to the xml
        pNodeMCNEtFilterPtr = xmlNewNode(NULL, BAD_CAST  DEF_MCNETFILTERS);
        xmlAddChild(pNodePtr, pNodeMCNEtFilterPtr);

        pFilterTag = xmlNewNode(NULL, BAD_CAST DEF_FILTER);
        xmlAddChild(pNodeMCNEtFilterPtr, pFilterTag);
    }
    else if(m_eCurrBus == J1939)
    {
        // Adding Filter to the xml
        pNodeJ1939FilterPtr = xmlNewNode(NULL, BAD_CAST  DEF_J1939FILTERS);
        xmlAddChild(pNodePtr, pNodeJ1939FilterPtr);

        pFilterTag = xmlNewNode(NULL, BAD_CAST DEF_FILTER);
        xmlAddChild(pNodeJ1939FilterPtr, pFilterTag);
    }
    else if(m_eCurrBus == FLEXRAY)
    {
        // Adding Filter to the xml
        pNodeFLEXRAYFilterPtr = xmlNewNode(NULL, BAD_CAST  DEF_FLEXRAYFILTERS);
        xmlAddChild(pNodePtr, pNodeFLEXRAYFilterPtr);

        pFilterTag = xmlNewNode(NULL, BAD_CAST DEF_FILTER);
        xmlAddChild(pNodeFLEXRAYFilterPtr, pFilterTag);
    }
    else if(m_eCurrBus == LIN)
    {
        // Adding Filter to the xml
        pNodeLINFilterPtr = xmlNewNode(NULL, BAD_CAST  DEF_LINFILTERS);
        xmlAddChild(pNodePtr, pNodeLINFilterPtr);

        pFilterTag = xmlNewNode(NULL, BAD_CAST DEF_FILTER);
        xmlAddChild(pNodeLINFilterPtr, pFilterTag);
    }
    else if(m_eCurrBus == MOST)
    {
        // Adding Filter to the xml
        pNodeMOSTFilterPtr = xmlNewNode(NULL, BAD_CAST  DEF_MOSTFILTERS);
        xmlAddChild(pNodePtr, pNodeMOSTFilterPtr);

        pFilterTag = xmlNewNode(NULL, BAD_CAST DEF_FILTER);
        xmlAddChild(pNodeMOSTFilterPtr, pFilterTag);
    }
    else
    {
        // Adding Filter to the xml
        pNodeCanFilterPtr = xmlNewNode(NULL, BAD_CAST  DEF_CANFILTERS);
        xmlAddChild(pNodePtr, pNodeCanFilterPtr);

        pFilterTag = xmlNewNode(NULL, BAD_CAST DEF_FILTER);
        xmlAddChild(pNodeCanFilterPtr, pFilterTag);
    }
    m_sFilterName.pbGetConfigData(pFilterTag);

    //COPY_DATA(pbTStream, &m_bEnabled, sizeof(m_bEnabled));
    //COPY_DATA(pbTStream, &m_eCurrBus, sizeof(m_eCurrBus));
    //COPY_DATA(pbTStream, &m_ushFilters, sizeof(m_ushFilters));

    for (USHORT i = 0; i < m_ushFilters; i++)
    {
        switch (m_eCurrBus)
        {
            case CAN:
            {
                //pbTStream = (((SFILTER_CAN*) m_psFilterInfo) + i)->pbGetConfigData(pbTStream);

                (((SFILTER_CAN*) m_psFilterInfo) + i)->pbGetConfigData(pFilterTag);
            }
            break;
            case FLEXRAY:
            {
                //pbTStream = (((SFILTER_FLEXRAY*) m_psFilterInfo) + i)->pbGetConfigData(pbTStream);
                (((SFILTER_FLEXRAY*) m_psFilterInfo) + i)->pbGetConfigData(pFilterTag);
            }
            break;
            case MCNET:
            {
                //pbTStream = (((SFILTER_MCNET*) m_psFilterInfo) + i)->pbGetConfigData(pbTStream);
                (((SFILTER_MCNET*) m_psFilterInfo) + i)->pbGetConfigData(pFilterTag);
            }
            break;
            case J1939:
            {
                //pbTStream = (((SFILTER_J1939*) m_psFilterInfo) + i)->pbGetConfigData(pbTStream);
                (((SFILTER_J1939*) m_psFilterInfo) + i)->pbGetConfigData(pFilterTag);
            }
            break;
            default:
            {
                ASSERT(FALSE);
            }
            break;
        }
    }

    //return pbTStream;
}

// PTV XML
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
void tagFilterSet::pbSetConfigData(xmlNodePtr pNodePtr, xmlDocPtr pxmlDocPtr,bool& Result)
{
    vClear(); // First clear everything

    //BYTE* pbTStream = pbTarget;
    Result = true;

    m_eCurrBus = CAN;


    m_sFilterName.pbSetConfigData(pNodePtr, pxmlDocPtr);


    //COPY_DATA_2(&m_bEnabled, pbTStream, sizeof(m_bEnabled));
    //COPY_DATA_2(&m_eCurrBus, pbTStream, sizeof(m_eCurrBus));
    //COPY_DATA_2(&m_ushFilters, pbTStream, sizeof(m_ushFilters));

    if (m_ushFilters > 0)
    {
        switch (m_eCurrBus)
        {
            case CAN:
                m_psFilterInfo = new SFILTER_CAN[m_ushFilters];
                break;
            case FLEXRAY:
                m_psFilterInfo = new SFILTER_FLEXRAY[m_ushFilters];
                break;
            case MCNET:
                m_psFilterInfo = new SFILTER_MCNET[m_ushFilters];
                break;
            case J1939:
                m_psFilterInfo = new SFILTER_J1939[m_ushFilters];
                break;
            default:
                ASSERT(FALSE);
        }

        if (NULL != m_psFilterInfo)
        {
            switch (m_eCurrBus)
            {
                case CAN:
                {

                    for (USHORT i = 0; i < m_ushFilters; i++)
                    {
                        //(((SFILTER_CAN*) m_psFilterInfo) + i)->pbSetConfigData(pbTStream);
                    }
                }
                break;
                case FLEXRAY:
                {

                    for (USHORT i = 0; i < m_ushFilters; i++)
                    {
                        //(((SFILTER_FLEXRAY*) m_psFilterInfo) + i)->pbSetConfigData(pbTStream);
                    }
                }
                break;
                case MCNET:
                {
                    for (USHORT i = 0; i < m_ushFilters; i++)
                    {
                        //(((SFILTER_MCNET*) m_psFilterInfo) + i)->pbSetConfigData(pbTStream);
                    }
                }
                break;
                case J1939:
                {
                    for (USHORT i = 0; i < m_ushFilters; i++)
                    {
                        //(((SFILTER_J1939*) m_psFilterInfo) + i)->pbSetConfigData(pbTStream);
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
                case CAN:
                    unFilterSize = sizeof(SFILTER_CAN);
                    break;
                case FLEXRAY:
                    unFilterSize = sizeof(SFILTER_FLEXRAY);
                    break;
                case MCNET:
                    unFilterSize = sizeof(SFILTER_MCNET);
                    break;
                case J1939:
                    unFilterSize = sizeof(SFILTER_J1939);
                    break;
                default:
                    ASSERT(FALSE);
            }
            //pbTStream += m_ushFilters * unFilterSize;
            m_ushFilters = 0;
            Result = false;
        }
    }
}
// PTV XML
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
            case CAN:
                m_psFilterInfo = new SFILTER_CAN[m_ushFilters];
                break;
            case FLEXRAY:
                m_psFilterInfo = new SFILTER_FLEXRAY[m_ushFilters];
                break;
            case MCNET:
                m_psFilterInfo = new SFILTER_MCNET[m_ushFilters];
                break;
            case J1939:
                m_psFilterInfo = new SFILTER_J1939[m_ushFilters];
                break;
            default:
                ASSERT(FALSE);
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
                case CAN:
                    unFilterSize = sizeof(SFILTER_CAN);
                    break;
                case FLEXRAY:
                    unFilterSize = sizeof(SFILTER_FLEXRAY);
                    break;
                case MCNET:
                    unFilterSize = sizeof(SFILTER_MCNET);
                    break;
                case J1939:
                    unFilterSize = sizeof(SFILTER_J1939);
                    break;
                default:
                    ASSERT(FALSE);
            }
            pbTStream += m_ushFilters * unFilterSize;
            m_ushFilters = 0;
            Result = false;
        }
    }

    return pbTStream;
}

int tagFilterSet::nSetXMLConfigData( ETYPE_BUS eBus, xmlNodePtr pFilter)
{
    vClear(); // First clear everything

    INT nResult = S_OK;

    m_sFilterName.nSetXMLConfigData(pFilter);

    //xmlXPathObjectPtr pObj = xmlUtils::pGetChildNodes(pFilter,  ( xmlChar *)"FilterMessage");

    m_eCurrBus = eBus;
    xmlNodePtr pTempFilter = pFilter->xmlChildrenNode;
    m_ushFilters = 0;
    while (pTempFilter != NULL)
    {
        if ((!xmlStrcmp(pTempFilter->name, (const xmlChar*)"FilterMessage")))
        {
            m_ushFilters++;
        }
        pTempFilter = pTempFilter->next;
    }


    if (m_ushFilters > 0)
    {
        switch (m_eCurrBus)
        {
            case CAN:
                m_psFilterInfo = new SFILTER_CAN[m_ushFilters];
        }
    }

    if (NULL != m_psFilterInfo)
    {
        switch (m_eCurrBus)
        {
            case CAN:
            {
                pTempFilter = pFilter->xmlChildrenNode;
                int i =0;
                while (pTempFilter != NULL)
                {
                    if ((!xmlStrcmp(pTempFilter->name, (const xmlChar*)"FilterMessage")))
                    {
                        xmlNodePtr pTempNode = pTempFilter->children;
                        (((SFILTER_CAN*) m_psFilterInfo) + i)->nSetXMLConfigData(pTempNode);
                        i++;
                    }
                    pTempFilter = pTempFilter->next;
                }
            }
        }
    }
    return nResult;
}
tagFilterSet* tagFilterSet::psGetFilterSetPointer(tagFilterSet* psSet, UINT Count, char* acFilterName)
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
