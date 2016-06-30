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
#include "Utility\MultiLanguageSupport.h"
#include "Filter/Filter_defines.h"
#include "ICluster.h"

const int SIZE_CHAR = sizeof(char);

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
    m_acFilterName.clear();
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
    m_acFilterName.clear();
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
    m_acFilterName = RefObj.m_acFilterName;
    m_bFilterType = RefObj.m_bFilterType;

    return *this;
}

void tagFilterName::pbGetConfigData(xmlNodePtr pFilterTag) const
{
    // Adding Filter Name to the xml
    xmlNodePtr pFilterNameNodePtr = xmlNewChild(pFilterTag, nullptr, BAD_CAST _(DEF_NAME), BAD_CAST m_acFilterName.c_str());
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
    xmlNodePtr pFilterTypeNodePtr = xmlNewChild(pFilterTag, nullptr, BAD_CAST _(DEF_TYPE), BAD_CAST omstrFilterType);
    xmlAddChild(pFilterTag, pFilterTypeNodePtr);
}


void tagFilterName::pbSetConfigData(xmlNodePtr pNodePtr, xmlDocPtr xmlConfigFiledoc)
{
    pNodePtr = pNodePtr->xmlChildrenNode;

    while(pNodePtr != nullptr)
    {
        if((!xmlStrcmp(pNodePtr->name, (const xmlChar*)"Filter")))
        {
            char* ptext = (char*)xmlNodeListGetString(xmlConfigFiledoc, pNodePtr->xmlChildrenNode, 1);
            if(nullptr != ptext)
            {
                if(ptext != nullptr)
                {
                    m_acFilterName = ptext;
                }

                xmlFree(ptext);
            }
        }
        pNodePtr = pNodePtr->next;
    }
}

INT tagFilterName::nSetXMLConfigData(xmlNodePtr pFilter)
{
    INT nRetVal = S_OK;
    xmlNodePtr pTempFilter = pFilter->xmlChildrenNode;
    m_acFilterName.clear();
    m_bFilterType = FALSE;
    while (pTempFilter != nullptr)
    {
        if ((!xmlStrcmp(pTempFilter->name, (const xmlChar*)_("Name"))))
        {
            char* pcTemp = (char*)xmlNodeListGetString(pTempFilter->doc, pTempFilter->xmlChildrenNode, 1);
            if(pcTemp != nullptr)
            {
                m_acFilterName = pcTemp;
            }
        }
        if ((!xmlStrcmp(pTempFilter->name, (const xmlChar*)_("Type"))))
        {
            m_bFilterType = nFilterType((char*)xmlNodeListGetString(pTempFilter->doc, pTempFilter->xmlChildrenNode, 1));
        }
        pTempFilter = pTempFilter->next;
    }
    if(m_acFilterName.empty())
    {
        UINT unRand = time(nullptr);
        char temp[128];
        sprintf_s(temp, 128, "Filter_%u", (UINT)time(nullptr));
        m_acFilterName = temp;
        nRetVal = S_FALSE;
    }
    return nRetVal;
}

BOOL tagFilterName::nFilterType(std::string strFilteType)
{
    m_bFilterType = FALSE;
    if(strFilteType == _("PASS"))
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
    m_omEventName = "";
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
    m_omEventName   = RefObj.m_omEventName;

    return *this;
}

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

    CString omstrDir = _("ALL");

    if(m_eDrctn == DIR_RX)
    {
        omstrDir = _("Rx");
    }
    else if(m_eDrctn == DIR_TX)
    {
        omstrDir = _("Tx");
    }

    if(m_ucFilterType == defFILTER_TYPE_ALL_FRAMES)
    {
        omcStartId = defSTR_ALL_FRAMES;
    }

    xmlNodePtr pStrtIdPtr = xmlNewChild(pxmlNodePtr, nullptr, BAD_CAST DEF_IDFROM, BAD_CAST omcStartId);
    xmlAddChild(pxmlNodePtr, pStrtIdPtr);

    xmlNodePtr pStpIdPtr = xmlNewChild(pxmlNodePtr, nullptr, BAD_CAST DEF_IDTO, BAD_CAST omcStpId);
    xmlAddChild(pxmlNodePtr, pStpIdPtr);

    if(m_ucFilterType == defFILTER_TYPE_EVENT)
    {
        xmlNodePtr pStpIdPtr = xmlNewChild(pxmlNodePtr, nullptr, BAD_CAST DEF_EVENT, BAD_CAST m_omEventName.c_str());
        xmlAddChild(pxmlNodePtr, pStpIdPtr);
    }
    xmlNodePtr pDirPtr = xmlNewChild(pxmlNodePtr, nullptr, BAD_CAST DEF_DIRECTION, BAD_CAST omstrDir.GetBufferSetLength(omstrDir.GetLength()));
    xmlAddChild(pxmlNodePtr, pDirPtr);

}

eDirection tagSFILTER::eGetMsgDirection(std::string strDirection)
{
    eDirection eDir = DIR_ALL;
    if(strDirection == _("Tx"))
    {
        eDir = DIR_TX;
    }
    if(strDirection == _("Rx"))
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
    BOOL bEvent = FALSE;
    std::string omMsgIDFrom = "";
    while (pNodePtr != nullptr)
    {
        if ((!xmlStrcmp(pNodePtr->name, (const xmlChar*)"IdFrom")))
        {
            if( nullptr != pNodePtr->xmlChildrenNode )
            {
                omMsgIDFrom = (char*)xmlNodeListGetString(pNodePtr->doc, pNodePtr->xmlChildrenNode, 1);
                m_dwMsgIDFrom = atoi(omMsgIDFrom.c_str()); //single nodee);
            }
        }
        if ((!xmlStrcmp(pNodePtr->name, (const xmlChar*)"IdTo")))
        {
            if( nullptr != pNodePtr->xmlChildrenNode )
            {
                m_dwMsgIDTo = atoi((char*)xmlNodeListGetString(pNodePtr->doc, pNodePtr->xmlChildrenNode, 1));
            }
        }
        if ((!xmlStrcmp(pNodePtr->name, (const xmlChar*)"Direction")))
        {
            if( nullptr != pNodePtr->xmlChildrenNode )
            {
                m_eDrctn = eGetMsgDirection((char*)xmlNodeListGetString(pNodePtr->doc, pNodePtr->xmlChildrenNode, 1));
            }
        }
        if((!xmlStrcmp(pNodePtr->name, (const xmlChar*)"Event")))
        {
            if( nullptr != pNodePtr->xmlChildrenNode )
            {
                bEvent = TRUE;
                m_omEventName = (char*)xmlNodeListGetString(pNodePtr->doc, pNodePtr->xmlChildrenNode, 1);
            }
        }
        pNodePtr = pNodePtr->next;
    }

    m_ucFilterType = defFILTER_TYPE_ID_RANGE;
    if(m_dwMsgIDFrom == m_dwMsgIDTo)
    {
        m_ucFilterType = defFILTER_TYPE_SINGLE_ID;
    }
    if(bEvent == TRUE)
    {
        m_ucFilterType = defFILTER_TYPE_EVENT;
    }
    if(omMsgIDFrom == defSTR_ALL_FRAMES)
    {
        m_ucFilterType = defFILTER_TYPE_ALL_FRAMES;
    }
    return nRetValue;
}

/* Ends SFILTER / tagSFILTER */
/* Starts SFILTER_LIN */

/******************************************************************************
  Function Name    :  SFILTER_LIN
  Input(s)         :  -
  Output           :  -
  Functionality    :  Standard constructor
  Member of        :  SFILTER_LIN
  Friend of        :  -
  Author(s)        :  Prathiba P
  Date Created     :  3.6.2014
  Modification date:
  Modification By  :
******************************************************************************/
SFILTER_LIN::SFILTER_LIN()
{
    vClear();
}


/******************************************************************************
  Function Name    :  ~SFILTER_LIN
  Input(s)         :  -
  Output           :  -
  Functionality    :  Destructor
  Member of        :  SFILTER_LIN
  Friend of        :  -
  Author(s)        :  Prathiba P
  Date Created     :  3.6.2014
  Modification date:
  Modification By  :
******************************************************************************/
SFILTER_LIN::~SFILTER_LIN()
{
    vClear();
}

/******************************************************************************
  Function Name    :  operator=
  Input(s)         :  const SFILTER_LIN& RefObj - The source object
  Output           :  SFILTER_LIN& - The current object reference.
  Functionality    :  Copies a source object by '=' operator overloading.
  Member of        :  SFILTER_LIN
  Friend of        :  -
  Author(s)        :  Prathiba P
  Date Created     :  3.6.2014
  Modification date:
  Modification By  :
******************************************************************************/
SFILTER_LIN& SFILTER_LIN::operator=(const SFILTER_LIN& RefObj)
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
  Member of        :  SFILTER_LIN
  Friend of        :  -
  Author(s)        :  Prathiba P
  Date Created     :  3.6.2014
  Modification date:
  Modification By  :
******************************************************************************/
void SFILTER_LIN::vClear(void)
{
    m_eChannel = CAN_CHANNEL_ALL;
    this->SFILTER::vClear();
}


/******************************************************************************
  Function Name    :  bDoesFrameOccur
  Input(s)         :  psCurrFrame - The frame in question.
  Output           :  TRUE if the frame occurs in the filtering block.
  Functionality    :  Query function to know if this filtering block is
                      configured for the message entry passed.
  Member of        :  SFILTER_LIN
  Friend of        :  -
  Author(s)        :  Prathiba P
  Date Created     :  3.6.2014
  Modification date:
  Modification By  :
******************************************************************************/
BOOL SFILTER_LIN::bDoesFrameOccur(const void* psCurrFrame) const
{
    BOOL bResult = FALSE;

    SFRAMEINFO_BASIC_LIN& sCurrFrame = *((SFRAMEINFO_BASIC_LIN*) psCurrFrame);

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
            if ( (CAN_CHANNEL_ALL == m_eChannel) ||
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
    else if(m_ucFilterType == defFILTER_TYPE_EVENT)
    {
        std::string strEventName = "";

        vGetEventName(sCurrFrame.m_eEventType, strEventName);
        // Check for Event type
        if (m_omEventName == strEventName)
        {
            // Check for message Channel
            if ( (CAN_CHANNEL_ALL == m_eChannel) ||
                    (m_eChannel == sCurrFrame.m_eChannel) )
            {
                // Check for message direction
                bResult = TRUE;
            }
        }
    }
    else
    {
        ASSERT(false);
    }

    return bResult;
}

void SFILTER_LIN::vGetEventName(eLinBusEventType eEventType, std::string& strEventName) const
{
    switch ( eEventType )
    {
        case EVENT_LIN_ERRSYNC:
            strEventName = defFILTER_SYNC_EVNT_LIN;
            break;
        case EVENT_LIN_ERRNOANS:
            strEventName =defFILTER_SLAVE_NO_RESP_EVNT_LIN;
            break;
        case EVENT_LIN_ERRCRC:
            strEventName = defFILTER_CHECKSUM_EVNT_LIN;
            break;
        case EVENT_LIN_ERRTOUT:
            strEventName = defFILTER_RX_FRAME_EVNT_LIN;
            break;
        default:
            strEventName ="Error - Unknown";
            break;
    }
}

void SFILTER_CAN::vGetEventName(ERROR_STATE eEventType, std::string& strEventName) const
{
    switch ( eEventType )
    {
        case ERROR_ACTIVE:
            strEventName = defFILTER_ERR_FRAME;
            break;
        case ERROR_WARNING_LIMIT:
            strEventName =defFILTER_ERR_FRAME;
            break;
        case ERROR_PASSIVE:
            strEventName = defFILTER_ERR_FRAME;
            break;
        case ERROR_BUS_OFF:
            strEventName = defFILTER_ERR_FRAME;
            break;
        case ERROR_FRAME:
            strEventName = defFILTER_ERR_FRAME;
            break;
        default:
            break;
    }
}

void SFILTER_LIN::pbGetConfigData(xmlNodePtr pNodePtr) const
{
    xmlNodePtr pFltrMsgPtr = xmlNewNode(nullptr, BAD_CAST DEF_FILTER_MESSAGE_LIN);
    xmlAddChild(pNodePtr, pFltrMsgPtr);

    this->SFILTER::pbGetConfigData(pFltrMsgPtr);

    CString omStrChannel = "";

    omStrChannel.Format("%d", m_eChannel);

    xmlNodePtr pChnlPtr = xmlNewChild(pFltrMsgPtr, nullptr, BAD_CAST DEF_CHANNEL, BAD_CAST omStrChannel.GetBuffer(omStrChannel.GetLength()));
    xmlAddChild(pFltrMsgPtr, pChnlPtr);
}


INT SFILTER_LIN::nSetXMLConfigData(xmlNodePtr pNodePtr)
{
    int nRetValue = S_OK;
    vClear();

    nRetValue = this->SFILTER::nSetXMLConfigData(pNodePtr);
    //<IDType>NONRTR</IDType>
    //<MsgType>EXT</MsgType>
    //<Channel>2</Channel>
    m_eChannel = 0;
    while (pNodePtr != nullptr)
    {
        if ((!xmlStrcmp(pNodePtr->name, (const xmlChar*)"Channel")))
        {
            if ( nullptr != pNodePtr->xmlChildrenNode )
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
/* Ends SFILTER_LIN */


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
    else if(m_ucFilterType == defFILTER_TYPE_EVENT)
    {
        std::string strEventName = "";

        vGetEventName(sCurrFrame.m_eEventType, strEventName);
        // Check for Event type
        if (m_omEventName == strEventName)
        {
            // Check for message Channel
            if ( (CAN_CHANNEL_ALL == m_eChannel) ||
                    (m_eChannel == sCurrFrame.m_eChannel) )
            {
                // Check for message direction
                bResult = TRUE;
            }
        }
    }
    else
    {
        ASSERT(false);
    }

    return bResult;
}



void SFILTER_CAN::pbGetConfigData(xmlNodePtr pNodePtr) const
{
    xmlNodePtr pFltrMsgPtr = xmlNewNode(nullptr, BAD_CAST DEF_FILTER_MESSAGE);
    xmlAddChild(pNodePtr, pFltrMsgPtr);

    this->SFILTER::pbGetConfigData(pFltrMsgPtr);

    CString omstrMsgType = "";
    CString omstrIdType = "";

    if(m_byIDType == TYPE_ID_CAN_EXTENDED)
    {
        omstrIdType = _("EXT");
    }
    else if(m_byIDType == TYPE_ID_CAN_ALL)
    {
        omstrIdType = _("ALL");
    }
    else if(m_byIDType == TYPE_ID_CAN_NONE)
    {
        omstrIdType = _("NONE");
    }
    else
    {
        omstrIdType = _("STD");
    }

    if(m_byMsgType == TYPE_MSG_CAN_RTR)
    {
        omstrMsgType = _("RTR");
    }
    else if(m_byMsgType == TYPE_MSG_CAN_ALL)
    {
        omstrMsgType = _("ALL");
    }
    else if(m_byMsgType == TYPE_MSG_CAN_NONE)
    {
        omstrMsgType = _("NONE");
    }
    else
    {
        omstrMsgType = _("NONRTR");
    }
    xmlNodePtr pIdTypePtr = xmlNewChild(pFltrMsgPtr, nullptr, BAD_CAST DEF_IDTYPE, BAD_CAST omstrIdType.GetBuffer(omstrIdType.GetLength()));
    xmlAddChild(pFltrMsgPtr, pIdTypePtr);

    xmlNodePtr pMsgPtr = xmlNewChild(pFltrMsgPtr, nullptr, BAD_CAST DEF_MSGTYPE, BAD_CAST omstrMsgType.GetBuffer(omstrMsgType.GetLength()));
    xmlAddChild(pFltrMsgPtr, pMsgPtr);

    CString omStrChannel = "";

    omStrChannel.Format("%d", m_eChannel);

    xmlNodePtr pChnlPtr = xmlNewChild(pFltrMsgPtr, nullptr, BAD_CAST DEF_CHANNEL, BAD_CAST omStrChannel.GetBuffer(omStrChannel.GetLength()));
    xmlAddChild(pFltrMsgPtr, pChnlPtr);
}



INT SFILTER_CAN::nGetIDType(std::string strIDType)
{
    int nId = TYPE_ID_CAN_ALL;
    if(strIDType == _("STD"))
    {
        nId = TYPE_ID_CAN_STANDARD;
    }
    if(strIDType == _("EXT"))
    {
        nId = TYPE_ID_CAN_EXTENDED;
    }
    return nId;
}

INT SFILTER_CAN::nGetMsgType(std::string strMsgType)
{
    int nId = TYPE_MSG_CAN_ALL;
    if(strMsgType == _("RTR"))
    {
        nId = TYPE_MSG_CAN_RTR;
    }
    if(strMsgType == _("NONRTR"))
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
    std::string strTemp;
    m_byMsgType = TYPE_MSG_CAN_ALL;
    m_byIDType = TYPE_ID_CAN_ALL;
    m_eChannel = 0;
    while (pNodePtr != nullptr)
    {
        if ((!xmlStrcmp(pNodePtr->name, (const xmlChar*)"IDType")))
        {
            if ( nullptr != pNodePtr->xmlChildrenNode )
            {
                strTemp = (char*)xmlNodeListGetString(pNodePtr->doc, pNodePtr->xmlChildrenNode, 1); //single node
                m_byIDType  =  nGetIDType(strTemp);
            }
        }
        else if ((!xmlStrcmp(pNodePtr->name, (const xmlChar*)"MsgType")))
        {
            if ( nullptr != pNodePtr->xmlChildrenNode )
            {
                strTemp = (char*)xmlNodeListGetString(pNodePtr->doc, pNodePtr->xmlChildrenNode, 1);
                m_byMsgType  = nGetMsgType(strTemp);
            }
        }
        else if ((!xmlStrcmp(pNodePtr->name, (const xmlChar*)"Channel")))
        {
            if ( nullptr != pNodePtr->xmlChildrenNode )
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
        ASSERT(false);
    }

    return bResult;
}


void SFILTER_MCNET::pbGetConfigData(xmlNodePtr pxmlNodePtr) const
{
    //BYTE* pbTStream = pbTarget;
    this->SFILTER::pbGetConfigData(pxmlNodePtr);
    //return pbTStream;
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
        ASSERT(false);
    }

    return bResult;
}


void SFILTER_J1939::pbGetConfigData(xmlNodePtr pNodePtr) const
{
    //BYTE* pbTStream = pbTarget;
    this->SFILTER::pbGetConfigData(pNodePtr);
    //return pbTStream;
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
    m_psFilterInfo = nullptr;
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
    if (nullptr != m_psFilterInfo)
    {
        delete[] m_psFilterInfo;
        m_psFilterInfo = nullptr;
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
                if (nullptr != m_psFilterInfo)
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

            case MCNET:
            {
                m_psFilterInfo = new SFILTER_MCNET[m_ushFilters];
                if (nullptr != m_psFilterInfo)
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
                if (nullptr != m_psFilterInfo)
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
            case LIN:
            {
                m_psFilterInfo = new SFILTER_LIN[m_ushFilters];
                if (nullptr != m_psFilterInfo)
                {
                    bResult = true;
                    for (USHORT i = 0; i < m_ushFilters; i++)
                    {
                        SFILTER_LIN* pDestObj = ((SFILTER_LIN*)m_psFilterInfo) + i;
                        SFILTER_LIN* pSrcObj = ((SFILTER_LIN*)RefObj.m_psFilterInfo) + i;
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
                ASSERT(false);
        }
    }
    else
    {
        m_psFilterInfo = nullptr;
        bResult = true;
    }

    return bResult;
}


void tagFilterSet::pbGetConfigData(xmlNodePtr pNodePtr) const
{
    //BYTE* pbTStream = pbTarget;

    xmlNodePtr pFilterTag;
    pFilterTag = xmlNewNode(nullptr, BAD_CAST DEF_FILTER);
    xmlAddChild(pNodePtr, pFilterTag);
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
            case LIN:
            {
                (((SFILTER_LIN*) m_psFilterInfo) + i)->pbGetConfigData(pFilterTag);
            }
            break;
            default:
            {
                ASSERT(false);
            }
            break;
        }
    }

    //return pbTStream;
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
    if(eBus == CAN)
    {
        while (pTempFilter != nullptr)
        {
            if ((!xmlStrcmp(pTempFilter->name, (const xmlChar*)"FilterMessage")))
            {
                m_ushFilters++;
            }
            pTempFilter = pTempFilter->next;
        }
    }
    else if(eBus == LIN)
    {
        while (pTempFilter != nullptr)
        {
            if ((!xmlStrcmp(pTempFilter->name, (const xmlChar*)"FilterMessage_LIN")))
            {
                m_ushFilters++;
            }
            pTempFilter = pTempFilter->next;
        }
    }


    if (m_ushFilters > 0)
    {
        switch (m_eCurrBus)
        {
            case CAN:
                m_psFilterInfo = new SFILTER_CAN[m_ushFilters];
                break;
            case LIN:
                m_psFilterInfo = new SFILTER_LIN[m_ushFilters];
                break;
        }
    }

    if (nullptr != m_psFilterInfo)
    {
        switch (m_eCurrBus)
        {
            case CAN:
            {
                pTempFilter = pFilter->xmlChildrenNode;
                int i =0;
                while (pTempFilter != nullptr)
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
            break;
            case LIN:
            {
                pTempFilter = pFilter->xmlChildrenNode;
                int i =0;
                while (pTempFilter != nullptr)
                {
                    if ((!xmlStrcmp(pTempFilter->name, (const xmlChar*)"FilterMessage_LIN")))
                    {
                        xmlNodePtr pTempNode = pTempFilter->children;
                        (((SFILTER_LIN*) m_psFilterInfo) + i)->nSetXMLConfigData(pTempNode);
                        i++;
                    }
                    pTempFilter = pTempFilter->next;
                }
            }
            break;
        }
    }
    return nResult;
}
tagFilterSet* tagFilterSet::psGetFilterSetPointer(tagFilterSet* psSet, UINT Count, const char* acFilterName)
{
    for (UINT i = 0; i < Count; i++)
    {
        tagFilterSet* psTemp = psSet + i;
        if ((psTemp != nullptr) && (psTemp->m_sFilterName.m_acFilterName == acFilterName))
        {
            return psTemp;
        }
    }
    return nullptr;
}
