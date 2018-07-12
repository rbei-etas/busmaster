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
 * \file      Filter_Datatypes.h
 * \brief     Definition file for filter data types.
 * \author    Ratnadip Choudhury
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Definition file for filter data types.
 */

#pragma once

#include "Utility/XMLUtils.h"
#include "include/BaseDefs.h"
#include "include/Utils_macro.h"
#include "include/XMLDefines.h"
#include <map>
#include <string>
#include "CANDriverDefines.h"
#include "LINDriverDefines.h"


#include "IBMNetWorkGetService.h"
#define defFILTER_TYPE_SINGLE_ID            0
#define defFILTER_TYPE_ID_RANGE             1
#define defFILTER_TYPE_EVENT                2



// Basic information needed for filtering when the bus is CAN.
typedef struct tagSFRAMEINFO_BASIC_CAN
{
    DWORD        m_dwFrameID;
    TYPE_CHANNEL m_eChannel;
    EDIRECTION   m_eDrctn;
    BYTE         m_byIDType;
    BYTE         m_byMsgType;
    ERROR_STATE m_eEventType;
} SFRAMEINFO_BASIC_CAN;

// Basic information needed for filtering when the bus is CAN.
typedef struct tagSFRAMEINFO_BASIC_LIN
{
    DWORD        m_dwFrameID;
    TYPE_CHANNEL m_eChannel;
    EDIRECTION   m_eDrctn;
    // BYTE         m_byIDType;
    BYTE         m_byMsgType;
    eLinBusEventType         m_eEventType;
} SFRAMEINFO_BASIC_LIN;

// Basic information needed for filtering when the bus is MCNet.
typedef struct tagSFRAMEINFO_BASIC_MCNET
{
    DWORD m_dwFrameID;
} SFRAMEINFO_BASIC_MCNET;

// Basic information needed for filtering when the bus is J1939.
typedef struct tagSFRAMEINFO_BASIC_J1939
{
    DWORD m_dwPGN;
    TYPE_CHANNEL m_eChannel; //KSS
    EDIRECTION   m_eDrctn;
} SFRAMEINFO_BASIC_J1939;

const int LENGTH_FILTERNAME = 128;

// The descriobes a filter name with type.
typedef struct tagFilterName
{
    /** Filter name */
    std::string m_acFilterName;

    /**  Filter Type 0 - Stop, 1 for Pass */
    int m_bFilterType;

    tagFilterName();

    /** To copy the source object. '=' operator overloaded. */
    tagFilterName& operator=(const tagFilterName& RefObj);

    /** To clear the current object. */
    void vClear(void);

    void pbGetConfigData(xmlNodePtr pxmlNodePtr) const;
    void pbSetConfigData(xmlNodePtr pNodePtr, xmlDocPtr pDocPtr);
    INT nSetXMLConfigData(xmlNodePtr pFilter);
    BOOL nFilterType(std::string strFilteType);
} SFILTERNAME, *PSFILTERNAME;

// Base class for an individual filter data structure.
typedef struct tagSFILTER
{
    UCHAR m_ucFilterType;   // 0 - Message ID and 1 - ID Range
    DWORD m_dwMsgIDFrom;    // From Message ID in case of range.
    // Msg ID in case of Single ID
    DWORD m_dwMsgIDTo;      // To Message ID incase of range.
    EDIRECTION m_eDrctn;    // Values are: DIR_RX, DIR_TX and DIR_ALL
    std::string  m_omEventName;

    tagSFILTER();           // Standard constructor
    virtual ~tagSFILTER() {};
    // To assign values from another object. The '=' operator overloaded.
    tagSFILTER& operator=(const tagSFILTER& RefObj);

    // To clear the current object.


    virtual void vClear(void);
    // Query function to know if this filtering block is configured for the
    // message entry passed.
    virtual BOOL bDoesFrameOccur(const void* psCurrFrame) const = 0;
    virtual void pbGetConfigData(xmlNodePtr pxmlNodePtr) const;
    virtual INT nSetXMLConfigData(xmlNodePtr pNodePtr);
    eDirection eGetMsgDirection(std::string strDirection);
} SFILTER, *PSFILTER;

struct SFILTER_CAN : public SFILTER
{
    BYTE         m_byIDType; //Extended or Standard
    BYTE         m_byMsgType;//RTR or NON RTR
    TYPE_CHANNEL m_eChannel;

    SFILTER_CAN();  // Standard constructor
    ~SFILTER_CAN(); // Destructor

    // To assign values from another object. The '=' operator overloaded.
    SFILTER_CAN& operator=(const SFILTER_CAN& RefObj);

    // To clear the current object.


    // Query function to know if this filtering block is configured for the
    // message entry passed.
    void vClear(void);
    BOOL bDoesFrameOccur(const void* psCurrFrame) const;
    void pbGetConfigData(xmlNodePtr pNodePtr) const;
    INT nSetXMLConfigData(xmlNodePtr pNodePtr);

    void vGetEventName(ERROR_STATE eEventType, std::string& strEventName) const;
    INT nGetIDType(std::string strIDType);
    INT nGetMsgType(std::string strMsgType);
};
typedef SFILTER_CAN* PSFILTER_CAN;

struct SFILTER_LIN : public SFILTER
{
    TYPE_CHANNEL m_eChannel;

    SFILTER_LIN();  // Standard constructor
    ~SFILTER_LIN(); // Destructor

    // To assign values from another object. The '=' operator overloaded.
    SFILTER_LIN& operator=(const SFILTER_LIN& RefObj);

    // To clear the current object.


    // Query function to know if this filtering block is configured for the
    // message entry passed.
    void vClear(void);
    BOOL bDoesFrameOccur(const void* psCurrFrame) const;
    void pbGetConfigData(xmlNodePtr pNodePtr) const;
    INT nSetXMLConfigData(xmlNodePtr pNodePtr);
    void vGetEventName(eLinBusEventType eEventType, std::string& strEventName) const;
};

typedef SFILTER_LIN* PSFILTER_LIN;





// The below structure describes a filter block for MCNet frames.
struct SFILTER_MCNET : public SFILTER
{
    SFILTER_MCNET();    // Standard constructor
    ~SFILTER_MCNET();   // Destructor

    // To assign values from another object. The '=' operator overloaded.
    SFILTER_MCNET& operator=(const SFILTER_MCNET& RefObj);

    // To clear the current object.
    void vClear(void);

    // Query function to know if this filtering block is configured for the
    // message entry passed.
    BOOL bDoesFrameOccur(const void* psCurrFrame) const;

    // To get size of the filter block object in bytes.
    UINT unGetSize(void) const;

    void pbGetConfigData(xmlNodePtr pxmlNodePtr) const;

};
typedef SFILTER_MCNET* PSFILTER_MCNET;

// The below structure describes a filter block for J1939 frames.
struct SFILTER_J1939 : public SFILTER
{
    SFILTER_J1939();    // Standard constructor
    ~SFILTER_J1939();   // Destructor

    // To assign values from another object. The '=' operator overloaded.
    SFILTER_J1939& operator=(const SFILTER_J1939& RefObj);

    // To clear the current object.
    void vClear(void);

    // Query function to know if this filtering block is configured for the
    // message entry passed.
    BOOL bDoesFrameOccur(const void* psCurrFrame) const;


    void pbGetConfigData(xmlNodePtr pNodePtr) const;

};
typedef SFILTER_J1939* PSFILTER_J1939;

// This below structure defines a filtering block.
typedef struct tagFilterSet
{
    SFILTERNAME m_sFilterName;  // Filter name and type
    BOOL        m_bEnabled;     // Enable status of the filter set
    ETYPE_BUS   m_eCurrBus;     // The type of bus
    USHORT      m_ushFilters;   // Number of filters
    PSFILTER    m_psFilterInfo; // Filter informations

    tagFilterSet();             // Standard constructor
    virtual ~tagFilterSet();            // Destructor


    // To clear the current filter set.
    virtual void vClear(void);
    // To clone a source filter set.
    virtual bool bClone(const tagFilterSet& RefObj);
    virtual void pbGetConfigData(xmlNodePtr pNodePtr) const;



    virtual int nSetXMLConfigData(ETYPE_BUS eBus, xmlNodePtr pFilter);

    static tagFilterSet* psGetFilterSetPointer(tagFilterSet* psSet, UINT Count, const char* acFilterName);

} SFILTERSET, *PSFILTERSET;

// This structure defines a set of filters along with the sufficient entities
// to apply this for filtering process. So the necessary member functions.
template <typename SFRAMEINFO_BASIC_BUS>
struct SFILTERAPPLIED
{
    BOOL                m_bEnabled;         // Enable flag of current filter
    USHORT              m_ushTotal;         // Total number of filter blocks.
    PSFILTERSET         m_psFilters;        // The filter set dynamic array.

    SFILTERAPPLIED();       // Standard constructor
    ~SFILTERAPPLIED();      // Destructor

    // To clone a source filter object.
    virtual bool bClone(const SFILTERAPPLIED<SFRAMEINFO_BASIC_BUS>& Source);

    // To clear the filter object
    virtual void vClear(void);

    // Query function that tells if the filter object will block the frame.
    virtual BOOL bToBeBlocked(const SFRAMEINFO_BASIC_BUS& sCurrFrame) const;



    virtual void pbGetConfigData(xmlNodePtr& pNodePtr) const;
    void pbGetConfigFilterData(xmlNodePtr pNodePtr) const;


    void pbSetConfigData(SFILTERAPPLIED& pFilterAppliedCAN, xmlNodeSetPtr pNodeSet,xmlDocPtr pdocptr, bool& Result, ETYPE_BUS eBus = CAN);

    //int nGetFiltersFromName(PSFILTERSET& sFilterDest, CStringArray& omStrNames);
    int nGetFiltersFromName(SFILTERAPPLIED& sFilterDest, std::map<std::string,int>& omStrFilters);

    virtual int nSetXMLConfigData(const xmlDocPtr& pDocPtr, ETYPE_BUS eBus);
    int nGetFiltersFromName(SFILTERAPPLIED& sFilterDest, CStringArray& omStrNames);
    int GetFilterNameIndex(std::string strName);
};



template <typename SFRAMEINFO_BASIC_BUS>
SFILTERAPPLIED<SFRAMEINFO_BASIC_BUS>::SFILTERAPPLIED()
{
    m_ushTotal = 0;
    m_bEnabled = FALSE;
    m_psFilters = nullptr;
}

template <typename SFRAMEINFO_BASIC_BUS>
SFILTERAPPLIED<SFRAMEINFO_BASIC_BUS>::~SFILTERAPPLIED()
{
    vClear();
}

/******************************************************************************
  Function Name    :  bClone
  Input(s)         :  Source - The source object.
  Output           :  true if cloning is successful, else false.
  Functionality    :  Called to clone a filter object to copy its contents
                      into the current object.
  Member of        :  SFILTERAPPLIED
  Friend of        :  -
  Author(s)        :  Ratnadip Choudhury
  Date Created     :  1.12.2009
  Modification date:
  Modification By  :
******************************************************************************/
template <typename SFRAMEINFO_BASIC_BUS>
bool SFILTERAPPLIED<SFRAMEINFO_BASIC_BUS>::bClone(
    const SFILTERAPPLIED<SFRAMEINFO_BASIC_BUS>& Source)
{
    // Overloading of '=' operator isn't done because in case of unavailability
    // of heap (although highly unlikely), this copying operation will fail.

    vClear();

    bool bResult = false;

    if (Source.m_ushTotal > 0)
    {
        m_psFilters = new SFILTERSET[Source.m_ushTotal];
        if (nullptr != m_psFilters)
        {
            bResult = true;
            for (int i = 0; (i < Source.m_ushTotal) && bResult; i++)
            {
                bResult = m_psFilters[i].bClone(Source.m_psFilters[i]);
            }
            if (!bResult) // If the cloning operation above is unsuccessful,
            {
                // clear all the buffer and the current object.
                vClear();
            }
        }
    }
    else
    {
        bResult = true;
    }

    if (bResult)
    {
        m_bEnabled = Source.m_bEnabled;
        m_ushTotal = Source.m_ushTotal;
    }

    return bResult;
}

/******************************************************************************
  Function Name    :  vClear
  Input(s)         :  void
  Output           :  void
  Functionality    :  Called to clear the current object.
  Member of        :  SFILTERAPPLIED
  Friend of        :  -
  Author(s)        :  Ratnadip Choudhury
  Date Created     :  1.12.2009
  Modification date:
  Modification By  :
******************************************************************************/
template <typename SFRAMEINFO_BASIC_BUS>
void SFILTERAPPLIED<SFRAMEINFO_BASIC_BUS>::vClear(void)
{
    m_ushTotal = 0;
    m_bEnabled = FALSE;

    if (nullptr != m_psFilters)
    {
        delete[] m_psFilters;
        m_psFilters = nullptr;
    }
}

/******************************************************************************
  Function Name    :  bToBeBlocked
  Input(s)         :  sCurrFrame - The current frame entry
  Output           :  TRUE if this is to be blocked.
  Functionality    :  This function tells if the filter object will block the
                      frame entry passed as argument.
  Member of        :  SFILTERAPPLIED
  Friend of        :  -
  Author(s)        :  Ratnadip Choudhury
  Date Created     :  1.12.2009
  Modification date:
  Modification By  :
******************************************************************************/
template <typename SFRAMEINFO_BASIC_BUS>
BOOL SFILTERAPPLIED<SFRAMEINFO_BASIC_BUS>::bToBeBlocked(const SFRAMEINFO_BASIC_BUS& sCurrFrame) const
{
    // If filtering is not enabled, no filtering out takes place.
    if (m_bEnabled == FALSE)
    {
        return FALSE;
    }

    /* Truth table for Filter Type, occurrence of the frame in the filtering
    block and decision on final filtering action */
    /* Pass    Frame found   To block
         T          T           F
         F          T           T
         T          F           T
         F          F           F   */

    BOOL bToBlock = FALSE;

    BOOL bToContinue = TRUE;
    for (USHORT i = 0; (i < m_ushTotal) && bToContinue; i++)
    {
        PSFILTERSET psCurrFilterBlk = m_psFilters + i;

        if (FALSE == psCurrFilterBlk->m_bEnabled)
        {
            continue;
        }

        bToBlock = psCurrFilterBlk->m_sFilterName.m_bFilterType;
        USHORT ushFilters = psCurrFilterBlk->m_ushFilters;

        for (USHORT j = 0; (j < ushFilters) && bToContinue; j++)
        {
            switch (psCurrFilterBlk->m_eCurrBus)
            {
                case CAN:
                {
                    PSFILTER psCurrFilter = (((SFILTER_CAN*) psCurrFilterBlk->m_psFilterInfo) + j);
                    if (psCurrFilter->bDoesFrameOccur((void*) &sCurrFrame))
                    {
                        bToContinue = FALSE;
                        bToBlock = !bToBlock;
                    }
                }
                break;
                case LIN:
                {
                    PSFILTER psCurrFilter = (((SFILTER_LIN*) psCurrFilterBlk->m_psFilterInfo) + j);
                    if (psCurrFilter->bDoesFrameOccur((void*) &sCurrFrame))
                    {
                        bToContinue = FALSE;
                        bToBlock = !bToBlock;
                    }
                }
                break;
            }
        }
    }
    return bToBlock;
}


template <typename SFRAMEINFO_BASIC_BUS>
void SFILTERAPPLIED<SFRAMEINFO_BASIC_BUS>::pbGetConfigFilterData(xmlNodePtr pNodePtr) const
{
    BYTE byVersion = 0x1;

    for (USHORT i = 0; i < m_ushTotal; i++)
    {
        CString strFilterName = m_psFilters[i].m_sFilterName.m_acFilterName.c_str();

        xmlNodePtr pFilterPtr = xmlNewChild(pNodePtr, nullptr, BAD_CAST DEF_FILTER, BAD_CAST strFilterName.GetBufferSetLength(strFilterName.GetLength()));
        xmlAddChild(pNodePtr, pFilterPtr);
    }

    //return pbTStream;
}
template <typename SFRAMEINFO_BASIC_BUS>
void SFILTERAPPLIED<SFRAMEINFO_BASIC_BUS>::pbGetConfigData(xmlNodePtr& pNodePtr) const
{
    BYTE byVersion = 0x1;

    xmlNodePtr pNodeCanFilterPtr = nullptr;
    xmlNodePtr pNodeMCNEtFilterPtr = nullptr;
    xmlNodePtr pNodeJ1939FilterPtr = nullptr;

    xmlNodePtr pNodeLINFilterPtr = nullptr;
    xmlNodePtr pNodeMOSTFilterPtr = nullptr;

    for (USHORT i = 0; i < m_ushTotal; i++)
    {
        //// Adding Filter to the xml
        //xmlNodePtr pNodeFilterPtr = xmlNewNode(nullptr, BAD_CAST  DEF_FILTER);
        //xmlAddChild(pNodePtr, pNodeFilterPtr);

        if(m_psFilters[i].m_eCurrBus == MCNET)
        {
            if(pNodeMCNEtFilterPtr == nullptr)
            {
                pNodeMCNEtFilterPtr = xmlNewNode(nullptr, BAD_CAST  DEF_MCNETFILTERS);
                xmlAddChild(pNodePtr, pNodeMCNEtFilterPtr);
            }

            m_psFilters[i].pbGetConfigData(pNodeMCNEtFilterPtr);
        }

        else if(m_psFilters[i].m_eCurrBus == J1939)
        {
            if(pNodeJ1939FilterPtr == nullptr)
            {
                pNodeJ1939FilterPtr = xmlNewNode(nullptr, BAD_CAST  DEF_J1939FILTERS);
                xmlAddChild(pNodePtr, pNodeJ1939FilterPtr);
            }

            m_psFilters[i].pbGetConfigData(pNodeJ1939FilterPtr);
        }

        else if(m_psFilters[i].m_eCurrBus == LIN)
        {
            if(pNodeLINFilterPtr == nullptr)
            {
                pNodeLINFilterPtr = xmlNewNode(nullptr, BAD_CAST  DEF_LINFILTERS);
                xmlAddChild(pNodePtr, pNodeLINFilterPtr);
            }

            m_psFilters[i].pbGetConfigData(pNodeLINFilterPtr);
        }

        else if(m_psFilters[i].m_eCurrBus == MOST)
        {
            if(pNodeMOSTFilterPtr == nullptr)
            {
                pNodeMOSTFilterPtr = xmlNewNode(nullptr, BAD_CAST  DEF_MOSTFILTERS);
                xmlAddChild(pNodePtr, pNodeMOSTFilterPtr);
            }

            m_psFilters[i].pbGetConfigData(pNodeMOSTFilterPtr);
        }

        else if(m_psFilters[i].m_eCurrBus == CAN)
        {
            if(pNodeCanFilterPtr == nullptr)
            {
                pNodeCanFilterPtr = xmlNewNode(nullptr, BAD_CAST  DEF_CANFILTERS);
                xmlAddChild(pNodePtr, pNodeCanFilterPtr);
            }

            m_psFilters[i].pbGetConfigData(pNodeCanFilterPtr);
        }
    }

    //return pbTStream;
}

template <typename SFRAMEINFO_BASIC_BUS>
void SFILTERAPPLIED<SFRAMEINFO_BASIC_BUS>::pbSetConfigData( SFILTERAPPLIED& pFilterAppliedCAN, xmlNodeSetPtr pNodeSetPtr, xmlDocPtr pdocptr, bool& Result, ETYPE_BUS eBusType = CAN )
{
    vClear();

    Result = true;

    m_ushTotal = static_cast<USHORT>(pNodeSetPtr->nodeNr);
    CStringArray omStrFilters;

    std::map<std::string, int> mapFilters;
    for ( INT nIndex = 0; nIndex < m_ushTotal; nIndex++ )
    {
        xmlNodePtr pNodePtr = pNodeSetPtr->nodeTab[nIndex];

        if ( ( !xmlStrcmp( pNodePtr->name, ( const xmlChar* )"Filter" ) ) )
        {
            int nEnabled = 1;
            xmlAttrPtr pAttr = pNodePtr->properties;
            while ( pAttr )
            {
                // walk through all the attributes and find the required one
                if ( pAttr->type == XML_ATTRIBUTE_NODE )
                {
                    std::string strAttrName( (char*)pAttr->name );
                    if ( ( strAttrName == "IsEnabled" ) )
                    {
                        nEnabled = atoi( (char*)pAttr->children->content );
                        break; // found
                    }
                }
                pAttr = pAttr->next;
            }

            xmlChar* key = xmlNodeListGetString( pNodePtr->doc, pNodePtr->xmlChildrenNode, 1 );
            if ( nullptr != key )
            {
                mapFilters[(char*)key] = nEnabled;
                xmlFree( key );
            }
        }
    }

    if ( mapFilters.size() > 0 && pdocptr != nullptr )
    {
        SFILTERAPPLIED<SFRAMEINFO_BASIC_BUS> sFilterApplied;
        if ( sFilterApplied.nSetXMLConfigData( pdocptr, eBusType ) == S_OK )
        {
            sFilterApplied.nGetFiltersFromName( pFilterAppliedCAN, mapFilters );
        }
    }

}

template <typename SFRAMEINFO_BASIC_BUS>
int SFILTERAPPLIED<SFRAMEINFO_BASIC_BUS>::nGetFiltersFromName(SFILTERAPPLIED& sFilterDest, std::map<std::string,int>& omStrFilters)
{
    int nRealFilters = 0;
    sFilterDest.m_bEnabled = m_bEnabled;
    sFilterDest.m_ushTotal = static_cast<USHORT>(omStrFilters.size());
    sFilterDest.m_psFilters = new SFILTERSET[sFilterDest.m_ushTotal];

    std::map<std::string,int>::iterator itr = omStrFilters.begin();
    int i =0;
    while ( itr != omStrFilters.end() )
    {
        int nIndex = GetFilterNameIndex(itr->first.c_str());
        if(nIndex >= 0)
        {
            sFilterDest.m_psFilters[i].bClone(m_psFilters[nIndex]);
            sFilterDest.m_psFilters[i].m_bEnabled = itr->second;
            i++;
        }
        itr++;
    }
    return 0;
}
template <typename SFRAMEINFO_BASIC_BUS>
int SFILTERAPPLIED<SFRAMEINFO_BASIC_BUS>::GetFilterNameIndex(std::string strName)
{
    int nIndex = -1;
    for(int i =0 ; i < m_ushTotal; i++)
    {
        if(strName == m_psFilters[i].m_sFilterName.m_acFilterName)
        {
            nIndex = i;
            break;
        }
    }
    return nIndex;
}

template <typename SFRAMEINFO_BASIC_BUS>
int SFILTERAPPLIED<SFRAMEINFO_BASIC_BUS>::nSetXMLConfigData(const xmlDocPtr& pDocPtr, ETYPE_BUS eBus)
{
    int nRetval = S_OK;

    xmlNodeSetPtr pNodeSet;
    xmlXPathObjectPtr pPathObject;

    if(eBus == CAN)
    {
        vClear();
        xmlChar* pchPath = (xmlChar*)"//BUSMASTER_CONFIGURATION/Module_Configuration/CAN_Filters/Filter";
        pPathObject = xmlUtils:: pGetNodes(pDocPtr, pchPath);
        if( nullptr != pPathObject )
        {
            pNodeSet = pPathObject->nodesetval;
            if(nullptr != pNodeSet)
            {
                m_ushTotal = static_cast<USHORT>(pNodeSet->nodeNr);
                if (m_ushTotal > 0)
                {
                    m_psFilters = new SFILTERSET[m_ushTotal];
                    if (nullptr != m_psFilters)
                    {
                        for (USHORT i = 0; i < m_ushTotal; i++)
                        {
                            m_psFilters[i].nSetXMLConfigData(CAN, pNodeSet->nodeTab[i]);
                        }
                    }
                    else
                    {
                        nRetval = S_FALSE;
                    }
                }
            }
            else
            {
                nRetval = S_FALSE;
            }
            xmlXPathFreeObject (pPathObject);
        }
        else
        {
            nRetval = S_FALSE;
        }

        if (S_FALSE == nRetval)
        {
            vClear();
        }
    }
    else if(eBus == LIN)
    {
        vClear();
        xmlChar* pchPath = (xmlChar*)"//BUSMASTER_CONFIGURATION/Module_Configuration/LIN_Filters/Filter";
        pPathObject = xmlUtils:: pGetNodes(pDocPtr, pchPath);
        if( nullptr != pPathObject )
        {
            pNodeSet = pPathObject->nodesetval;
            if(nullptr != pNodeSet)
            {
                m_ushTotal = static_cast<USHORT>(pNodeSet->nodeNr);
                if (m_ushTotal > 0)
                {
                    m_psFilters = new SFILTERSET[m_ushTotal];
                    if (nullptr != m_psFilters)
                    {
                        for (USHORT i = 0; i < m_ushTotal; i++)
                        {
                            m_psFilters[i].nSetXMLConfigData(LIN, pNodeSet->nodeTab[i]);
                        }
                    }
                    else
                    {
                        nRetval = S_FALSE;
                    }
                }
            }
            else
            {
                nRetval = S_FALSE;
            }
            xmlXPathFreeObject (pPathObject);
        }
        else
        {
            nRetval = S_FALSE;
        }

        if (S_FALSE == nRetval)
        {
            vClear();
        }
    }


    return nRetval;
}

// For CAN
typedef SFILTERAPPLIED<tagSFRAMEINFO_BASIC_CAN> SFILTERAPPLIED_CAN;

// For LIN
typedef SFILTERAPPLIED<tagSFRAMEINFO_BASIC_LIN> SFILTERAPPLIED_LIN;

// For MCNet
typedef SFILTERAPPLIED<tagSFRAMEINFO_BASIC_MCNET> SFILTERAPPLIED_MCNET;

// For J1939
typedef SFILTERAPPLIED<tagSFRAMEINFO_BASIC_J1939> SFILTERAPPLIED_J1939;


class BusmasterFilter
{
public:
    int getFilterCount(ETYPE_BUS);
    void getFilterList( ETYPE_BUS);
    void setFilterList( ETYPE_BUS );
};