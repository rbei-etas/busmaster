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

#include "include/BaseDefs.h"
#include "include/Utils_macro.h"


#define defFILTER_TYPE_SINGLE_ID            0
#define defFILTER_TYPE_ID_RANGE             1

// Basic information needed for filtering when the bus is FlexRay.
typedef struct tagSFRAMEINFO_BASIC_FLEXRAY
{
    DWORD        m_dwFrameID;
    TYPE_CHANNEL m_eChannel;
    EDIRECTION   m_eDrctn;
} SFRAMEINFO_BASIC_FLEXRAY;

// Basic information needed for filtering when the bus is CAN.
typedef struct tagSFRAMEINFO_BASIC_CAN
{
    DWORD        m_dwFrameID;
    TYPE_CHANNEL m_eChannel;
    EDIRECTION   m_eDrctn;
    BYTE         m_byIDType;
    BYTE         m_byMsgType;
} SFRAMEINFO_BASIC_CAN;

// Basic information needed for filtering when the bus is MCNet.
typedef struct tagSFRAMEINFO_BASIC_MCNET
{
    DWORD m_dwFrameID;
} SFRAMEINFO_BASIC_MCNET;

// Basic information needed for filtering when the bus is J1939.
typedef struct tagSFRAMEINFO_BASIC_J1939
{
    DWORD m_dwPGN;
} SFRAMEINFO_BASIC_J1939;

const int LENGTH_FILTERNAME = 128;

// The descriobes a filter name with type.
typedef struct tagFilterName
{
    char m_acFilterName[LENGTH_FILTERNAME];                     // Filter name
    BOOL  m_bFilterType;                    // Filter Type 0 - Stop, 1 for Pass

    tagFilterName();    // Standard constructor

    // To copy the source object. '=' operator overloaded.
    tagFilterName& operator=(const tagFilterName& RefObj);

    // To clear the current object.
    void vClear(void);

    // Called to get the filter object's data into a stream buffer.
    BYTE* pbGetConfigData(BYTE* pbTarget) const;

    // Called to retrieve a filter object's data from a byte stream and
    // initialise the current filter object with the retrieved data.
    BYTE* pbSetConfigData(BYTE* pbTarget);

} SFILTERNAME, *PSFILTERNAME;

// Base class for an individual filter data structure.
typedef struct tagSFILTER
{
    UCHAR m_ucFilterType;   // 0 - Message ID and 1 - ID Range
    DWORD m_dwMsgIDFrom;    // From Message ID in case of range.
    // Msg ID in case of Single ID
    DWORD m_dwMsgIDTo;      // To Message ID incase of range.
    EDIRECTION m_eDrctn;    // Values are: DIR_RX, DIR_TX and DIR_ALL

    tagSFILTER();           // Standard constructor

    // To assign values from another object. The '=' operator overloaded.
    tagSFILTER& operator=(const tagSFILTER& RefObj);

    // To clear the current object.
    void vClear(void);

    // Query function to know if this filtering block is configured for the
    // message entry passed.
    virtual BOOL bDoesFrameOccur(const void* psCurrFrame) const = 0;

    // To get size of the filter block object in bytes.
    virtual UINT unGetSize(void) const;

    // Called to get the filter object's data into a stream buffer.
    virtual BYTE* pbGetConfigData(BYTE* pbTarget) const;

    // Called to retrieve a filter object's data from a byte stream and
    // initialise the current filter object with the retrieved data.
    virtual BYTE* pbSetConfigData(BYTE* pbTarget);

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
    void vClear(void);

    // Query function to know if this filtering block is configured for the
    // message entry passed.
    BOOL bDoesFrameOccur(const void* psCurrFrame) const;

    // To get size of the filter block object in bytes.
    UINT unGetSize(void) const;

    // Called to get the filter object's data into a stream buffer.
    BYTE* pbGetConfigData(BYTE* pbTarget) const;

    // Called to retrieve a filter object's data from a byte stream and
    // initialise the current filter object with the retrieved data.
    BYTE* pbSetConfigData(BYTE* pbTarget);
};
typedef SFILTER_CAN* PSFILTER_CAN;

// The below structure describes a filter block for FlexRay frames.
struct SFILTER_FLEXRAY : public SFILTER
{
    TYPE_CHANNEL m_eChannel;// Current channel

    SFILTER_FLEXRAY();  // Standard constructor
    ~SFILTER_FLEXRAY(); // Destructor

    // To assign values from another object. The '=' operator overloaded.
    SFILTER_FLEXRAY& operator=(const SFILTER_FLEXRAY& RefObj);

    // To clear the current object.
    void vClear(void);

    // Query function to know if this filtering block is configured for the
    // message entry passed.
    BOOL bDoesFrameOccur(const void* psCurrFrame) const;

    // To get size of the filter block object in bytes.
    UINT unGetSize(void) const;

    // Called to get the filter object's data into a stream buffer.
    BYTE* pbGetConfigData(BYTE* pbTarget) const;

    // Called to retrieve a filter object's data from a byte stream and
    // initialise the current filter object with the retrieved data.
    BYTE* pbSetConfigData(BYTE* pbTarget);
};
typedef SFILTER_FLEXRAY* PSFILTER_FLEXRAY;

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

    // Called to get the filter object's data into a stream buffer.
    BYTE* pbGetConfigData(BYTE* pbTarget) const;

    // Called to retrieve a filter object's data from a byte stream and
    // initialise the current filter object with the retrieved data.
    BYTE* pbSetConfigData(BYTE* pbTarget);
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

    // To get size of the filter block object in bytes.
    UINT unGetSize(void) const;

    // Called to get the filter object's data into a stream buffer.
    BYTE* pbGetConfigData(BYTE* pbTarget) const;

    // Called to retrieve a filter object's data from a byte stream and
    // initialise the current filter object with the retrieved data.
    BYTE* pbSetConfigData(BYTE* pbTarget);
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
    ~tagFilterSet();            // Destructor

    // To clear the current filter set.
    void vClear(void);

    // To clone a source filter set.
    bool bClone(const tagFilterSet& RefObj);

    // To get size of the filter block set in bytes.
    UINT unGetSize(void) const;

    // Called to get the filter block set's data into a stream buffer.
    BYTE* pbGetConfigData(BYTE* pbTarget) const;

    // Called to retrieve a filter block set's data from a byte stream and
    // initialise the current filter object with the retrieved data.
    BYTE* pbSetConfigData(BYTE* pbTarget, bool& Result);

    static tagFilterSet* psGetFilterSetPointer(tagFilterSet* psSet, UINT Count, char* acFilterName);

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
    bool bClone(const SFILTERAPPLIED<SFRAMEINFO_BASIC_BUS>& Source);

    // To clear the filter object
    void vClear(void);

    // Query function that tells if the filter object will block the frame.
    BOOL bToBeBlocked(const SFRAMEINFO_BASIC_BUS& sCurrFrame) const;

    // To get the size (in bytes) of the object.
    UINT unGetSize(void) const;

    // Called to get the filter object's data into a stream buffer.
    BYTE* pbGetConfigData(BYTE* pbTarget) const;

    // Called to retrieve a filter object's data from a byte stream and
    // initialise the current filter object with the retrieved data.
    BYTE* pbSetConfigData(BYTE* pbTarget, bool& Result);

};

/******************************************************************************
  Function Name    :  SFILTERAPPLIED
  Input(s)         :  -
  Output           :  -
  Functionality    :  Standard constructor
  Member of        :  SFILTERAPPLIED
  Friend of        :  -
  Author(s)        :  Ratnadip Choudhury
  Date Created     :  1.12.2009
  Modification date:
  Modification By  :
******************************************************************************/
template <typename SFRAMEINFO_BASIC_BUS>
SFILTERAPPLIED<SFRAMEINFO_BASIC_BUS>::SFILTERAPPLIED()
{
    m_ushTotal = 0;
    m_bEnabled = FALSE;
    m_psFilters = NULL;
}

/******************************************************************************
  Function Name    :  ~SFILTERAPPLIED
  Input(s)         :  -
  Output           :  -
  Functionality    :  Destructor
  Member of        :  SFILTERAPPLIED
  Friend of        :  -
  Author(s)        :  Ratnadip Choudhury
  Date Created     :  1.12.2009
  Modification date:
  Modification By  :
******************************************************************************/
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

        if (NULL != m_psFilters)
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

    if (NULL != m_psFilters)
    {
        delete[] m_psFilters;
        m_psFilters = NULL;
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
            }
        }
    }

    return bToBlock;
}

/******************************************************************************
  Function Name    :  unGetSize
  Input(s)         :  void
  Output           :  UINT
  Functionality    :  Returns size of the object in bytes
  Member of        :  SFILTERAPPLIED
  Friend of        :  -
  Author(s)        :  Ratnadip Choudhury
  Date Created     :  1.12.2009
  Modification date:
  Modification By  :
******************************************************************************/
template <typename SFRAMEINFO_BASIC_BUS>
UINT SFILTERAPPLIED<SFRAMEINFO_BASIC_BUS>::unGetSize(void) const
{
    //version
    UINT Result = sizeof (BYTE) + sizeof(m_bEnabled) + sizeof(m_ushTotal);

    for (USHORT i = 0; i < m_ushTotal; i++)
    {
        Result += (m_psFilters + i)->unGetSize();
    }

    return Result;
}

/******************************************************************************
  Function Name    :  pbGetConfigData
  Input(s)         :  pbSource - Target stream to copy data into.
  Output           :  Address of the next available byte to write data.
  Functionality    :  This copies current object data into the target byte stream
  Member of        :  SFILTERAPPLIED
  Friend of        :  -
  Author(s)        :  Ratnadip Choudhury
  Date Created     :  1.12.2009
  Modification date:
  Modification By  :
******************************************************************************/
template <typename SFRAMEINFO_BASIC_BUS>
BYTE* SFILTERAPPLIED<SFRAMEINFO_BASIC_BUS>::pbGetConfigData(BYTE* pbTarget) const
{
    BYTE* pbTStream = pbTarget;
    BYTE byVersion = 0x1;
    COPY_DATA(pbTStream, &byVersion, sizeof(BYTE));
    COPY_DATA(pbTStream, &m_bEnabled, sizeof(m_bEnabled));
    COPY_DATA(pbTStream, &m_ushTotal, sizeof(m_ushTotal));

    for (USHORT i = 0; i < m_ushTotal; i++)
    {
        pbTStream = m_psFilters[i].pbGetConfigData(pbTStream);
    }

    return pbTStream;
}

/******************************************************************************
  Function Name    :  pbSetConfigData
  Input(s)         :  pbSource - Source stream to copy data from.
                      Result - [out] parameter. true if operation is successful.
  Output           :  Address of the byte next to the already read block.
  Functionality    :  This copies an applied filter block from a byte stream
                      and initialises the current object.
  Member of        :  SFILTERAPPLIED
  Friend of        :  -
  Author(s)        :  Ratnadip Choudhury
  Date Created     :  1.12.2009
  Modification date:
  Modification By  :
******************************************************************************/
template <typename SFRAMEINFO_BASIC_BUS>
BYTE* SFILTERAPPLIED<SFRAMEINFO_BASIC_BUS>::pbSetConfigData(BYTE* pbSource, bool& Result)
{
    vClear();
    Result = true;
    BYTE* pbSStream = pbSource;
    BYTE byVersion = 0;
    COPY_DATA_2(&byVersion, pbSStream, sizeof(BYTE));
    COPY_DATA_2(&m_bEnabled, pbSStream, sizeof(m_bEnabled));
    COPY_DATA_2(&m_ushTotal, pbSStream, sizeof(m_ushTotal));

    if (m_ushTotal > 0)
    {
        m_psFilters = new SFILTERSET[m_ushTotal];

        if (NULL != m_psFilters)
        {
            for (USHORT i = 0; (i < m_ushTotal) && Result; i++)
            {
                pbSStream = m_psFilters[i].pbSetConfigData(pbSStream, Result);
            }
        }
        else
        {
            Result = false;
        }
    }

    if (false == Result)
    {
        vClear();
    }

    return pbSStream;
}

// For CAN
typedef SFILTERAPPLIED<tagSFRAMEINFO_BASIC_CAN> SFILTERAPPLIED_CAN;

// For FlexRay
typedef SFILTERAPPLIED<tagSFRAMEINFO_BASIC_FLEXRAY> SFILTERAPPLIED_FLEXRAY;

// For MCNet
typedef SFILTERAPPLIED<tagSFRAMEINFO_BASIC_MCNET> SFILTERAPPLIED_MCNET;

// For J1939
typedef SFILTERAPPLIED<tagSFRAMEINFO_BASIC_J1939> SFILTERAPPLIED_J1939;
