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
 * \file      GraphList.cpp
 * \brief     Implementation file for CGraphElement class
 * \author    Raja N
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Implementation file for CGraphElement class
 */
// For standard Headers
#include "stdafx.h"
// For App declaration
#include "BUSMASTER.h"
// For Graph Parameter Class
#include "GraphParameters.h"
// For Graph Element class
#include "GraphElement.h"
// For CGraphList class declaration
#include "GraphList.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

/*******************************************************************************
  Function Name  : CGraphList
  Description    : Standard default constructor
  Member of      : CGraphList
  Functionality  : This will initialise local variables
  Author(s)      : Raja N
  Date Created   : 01.12.2004
  Modifications  :
*******************************************************************************/
CGraphList::CGraphList()
{
}

/*******************************************************************************
  Function Name  : CGraphList
  Description    : Standard Destructor
  Member of      : CGraphList
  Functionality  : -
  Author(s)      : Raja N
  Date Created   : 01.12.2004
*******************************************************************************/
CGraphList::~CGraphList()
{
}

UINT CGraphList::unGetConfigSize(BYTE byVersion)
{
    UINT unSize = 0;
    /* GRAPH PARAMETERS*/
    unSize = m_odGraphParameters.unGetConfigSize(byVersion);
    unSize += sizeof(int);//To store the count of Graph elements
    /* GRAPH ELEMENTS*/
    unSize += CGraphElement::unGetConfigSize(byVersion) * m_omElementList.GetSize();
    return unSize;
}
BYTE* CGraphList::pbyGetConfigData(BYTE* pbyTrgtData, BYTE byVersion)
{
    BYTE* pbyTemp = pbyTrgtData;
    pbyTemp = m_odGraphParameters.pbyGetConfigData(pbyTemp, byVersion);
    // Save the list element count
    // Get list size
    int nElementCount = m_omElementList.GetSize();
    // Save the size
    COPY_DATA(pbyTemp, &nElementCount, sizeof (int));

    // Save list of elements
    if( nElementCount > 0 )
    {
        CGraphElement odTemp;

        for( int nIndex = 0; nIndex < nElementCount;
                nIndex++ )
        {
            // Get the object from the list
            odTemp = m_omElementList.GetAt( nIndex );
            // Save the element
            pbyTemp = odTemp.pbyGetConfigData( pbyTemp, byVersion);
        }
    }

    return pbyTemp;
}

BYTE* CGraphList::pbySetConfigData(BYTE* pbyTrgtData, BYTE byVersion)
{
    BYTE* pbyTemp = pbyTrgtData;
    pbyTemp = m_odGraphParameters.pbySetConfigData(pbyTemp, byVersion);
    // Get element count
    int nElementCount = 0;
    COPY_DATA_2(&nElementCount, pbyTemp, sizeof (int));

    // Load list if element count is more then 0
    if ( nElementCount > 0 )
    {
        // Temp object
        CGraphElement odTemp;
        int nExistingElements = m_omElementList.GetSize();

        // Reuse existing items in the list

        // Add / remove extra items
        if( nElementCount > nExistingElements )
        {
            // Add Extra Elements
            for( int nIndex = nExistingElements;
                    nIndex < nElementCount; nIndex++)
            {
                // Add Dummy Element
                m_omElementList.Add( odTemp );
            }
        }
        // Delete extra elements
        else if (nElementCount <  nExistingElements )
        {
            // Get the extra elements count
            int nDifference = nExistingElements - nElementCount;

            // Remove extra elements. Removing it from the list.
            // Remove the head element instead of any other index.
            // Because list index might become invalid if the
            // size of the list after deletion comes below the
            // index value
            for( int nIndex = 0; nIndex < nDifference; nIndex++ )
            {
                // Remove the first element
                m_omElementList.RemoveAt( 0 );
            }
        }

        // Now populate list
        for( int nIndex = 0; nIndex < nElementCount;
                nIndex++ )
        {
            // Read element from the archive
            pbyTemp =
                m_omElementList[ nIndex ].pbySetConfigData( pbyTemp, byVersion );
        }
    }
    else
    {
        // Clear list
        m_omElementList.RemoveAll();
    }

    return pbyTemp;
}
/*******************************************************************************
  Function Name  : nSerialize
  Input(s)       : omArch - Object to CArchive
  Output         :
  Functionality  : This class will serialise graph list using CArchive object
  Member of      : CGraphList
  Author(s)      : Raja N
  Date Created   : 01/12/2004
  Modifications  :
 *******************************************************************************/
int CGraphList::nSerialize(CArchive& omArch)
{
    int nReturn = 0;
    int nElementCount = 0;

    try
    {
        // Load/Store Graph Control Parameters
        nReturn = m_odGraphParameters.nSerialize( omArch );

        if( nReturn == 0 )
        {
            // Saving
            if( omArch.IsStoring() )
            {
                // Save the list element count
                // Get list size
                nElementCount = m_omElementList.GetSize();
                // Save the size
                omArch << nElementCount;

                // Save list of elements
                if( nElementCount > 0 )
                {
                    CGraphElement odTemp;

                    for( int nIndex = 0; nReturn == 0 && nIndex < nElementCount;
                            nIndex++ )
                    {
                        // Get the object from the list
                        odTemp = m_omElementList.GetAt( nIndex );
                        // Save the element
                        nReturn = odTemp.nSerialize( omArch );
                    }
                }
            }
            // Loading
            else
            {
                // Get element count
                omArch >> nElementCount;

                // Load list if element count is more then 0
                if ( nElementCount > 0 )
                {
                    // Temp object
                    CGraphElement odTemp;
                    int nExistingElements = m_omElementList.GetSize();

                    // Reuse existing items in the list

                    // Add / remove extra items
                    if( nElementCount > nExistingElements )
                    {
                        // Add Extra Elements
                        for( int nIndex = nExistingElements;
                                nIndex < nElementCount; nIndex++)
                        {
                            // Add Dummy Element
                            m_omElementList.Add( odTemp );
                        }
                    }
                    // Delete extra elements
                    else if (nElementCount <  nExistingElements )
                    {
                        // Get the extra elements count
                        int nDifference = nExistingElements - nElementCount;

                        // Remove extra elements. Removing it from the list.
                        // Remove the head element instead of any other index.
                        // Because list index might become invalid if the
                        // size of the list after deletion comes below the
                        // index value
                        for( int nIndex = 0; nIndex < nDifference; nIndex++ )
                        {
                            // Remove the first element
                            m_omElementList.RemoveAt( 0 );
                        }
                    }

                    // Now populate list
                    for( int nIndex = 0; nReturn == 0 && nIndex < nElementCount;
                            nIndex++ )
                    {
                        // Read element from the archive
                        nReturn =
                            m_omElementList[ nIndex ].nSerialize( omArch );
                    }
                }
                else
                {
                    // Clear list
                    m_omElementList.RemoveAll();
                }
            }
        }
    }
    catch(CArchiveException* poArchExcep)
    {
        nReturn = poArchExcep->m_cause;
        poArchExcep->Delete();
    }

    // Return the result
    return nReturn;
}
