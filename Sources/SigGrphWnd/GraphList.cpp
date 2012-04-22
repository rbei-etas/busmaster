/*********************************************************************
  Project       :  Auto-SAT_Tools
  FileName      :  GraphList.cpp
  Description   :  Implementation file for CGraphElement class
  $Log:   X:/Archive/Sources/SigGrphWnd/GraphList.cpv  $
   
  Author(s)     :  Raja N
  Date Created  :  01.12.2004
  Modified By   :  
  Copyright (c) 2011, Robert Bosch Engineering and Business Solutions.  All rights reserved
 *********************************************************************/
// For standard Headers
#include "SigGrphWnd_stdafx.h"
// For CGraphList class declaration
#include "GraphList.h"

#include "SigGrphWnd_Defines.h"

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

UINT CGraphList::unGetConfigSize(void)
{
    UINT unSize = 0;
    /* GRAPH PARAMETERS*/
    // Display perf conf.
    // Buffer size
    unSize += sizeof(int);
    // Display refresh rate
    unSize += sizeof(int);
    // View Style Configuration
    // Frame Color
    unSize += sizeof(int);
    // Frame Style
    unSize += sizeof(int);
    // Plot Area Color
    unSize += sizeof(int);
    // Grid Color
    unSize += sizeof(int);
    // Axis Color
    unSize += sizeof(int);
    // X Grid Lines
    unSize += sizeof(int);
    // Y Grid Lines
    unSize += sizeof(int);
    // User Selected Active Axis
    unSize += sizeof(int);
    // User Selected Action
    unSize += sizeof(int);
    // Grid Setting
    unSize += sizeof(BOOL);

    unSize += sizeof(int);//To store the count of Graph elements
    /* GRAPH ELEMENTS*/
	for (INT i = 0; i < m_omElementList.GetSize(); i++)
    {

        unSize += sizeof(int);//     m_nMsgID;
        // Frame Format - Standard or Extended
        unSize += sizeof(short);//   m_nFrameFormat;
        // Element Name String
        unSize += (sizeof(TCHAR) * MAX_PATH);//m_omStrElementName
        // Type of the element value
        // 0 - network statistics parameter
        // 1 - Raw Value of the Signal
        // 2 - Physical Value of the signal
        unSize += sizeof(int);//   m_nValueType;
        // Line type of the element
        unSize += sizeof(int);//   m_nLineType;
        // Line Color of the element
        unSize += sizeof(int);//     m_nLineColor;
        // Sample point symbol type
        unSize += sizeof(int);//     m_nPointType;
        // Sample point symbol color
        unSize += sizeof(int);//    m_nPointColor;
        // Visible or Not
        unSize += sizeof(BOOL);//    m_bVisible;
        // Enabled or not
        unSize += sizeof(BOOL);//    m_bEnabled;
    }


    return unSize;
}
BYTE* CGraphList::pbyGetConfigData(BYTE* pbyTrgtData)
{
    BYTE* pbyTemp = pbyTrgtData;
    pbyTemp = m_odGraphParameters.pbyGetConfigData(pbyTemp);

    // Save the list element count
    // Get list size
    int nElementCount = (int)m_omElementList.GetSize();
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
            pbyTemp = odTemp.pbyGetConfigData( pbyTemp );
        }
    }

    
    return pbyTemp;
}

BYTE* CGraphList::pbySetConfigData(BYTE* pbyTrgtData)
{
    BYTE* pbyTemp = pbyTrgtData;
    pbyTemp = m_odGraphParameters.pbySetConfigData(pbyTemp);

    // Get element count
    int nElementCount = 0;
    COPY_DATA_2(&nElementCount, pbyTemp, sizeof (int));
    
    // Load list if element count is more then 0
    if ( nElementCount > 0 )
    {
        // Temp object
        CGraphElement odTemp;
        int nExistingElements = (int)m_omElementList.GetSize();

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
                m_omElementList[ nIndex ].pbySetConfigData( pbyTemp );
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
                nElementCount = (int)m_omElementList.GetSize();
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
                    int nExistingElements = (int)m_omElementList.GetSize();

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
