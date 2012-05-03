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
 * \file      GraphElement.cpp
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
// For CGraphElement class declaration
#include "GraphElement.h"
#include "InterfaceGetter.h"

/*******************************************************************************
 Function Name  : CGraphElement
 Description    : Standard default constructor
 Member of      : CGraphElement
 Functionality  : This will initialise local variables
 Author(s)      : Raja N
 Date Created   : 01.12.2004
 Modifications  :
*******************************************************************************/
CGraphElement::CGraphElement()
{
    // Message ID
    m_nMsgID = -1;
    // Message Name
    m_strMsgName = STR_EMPTY;
    // Format - Standard or Extended
    m_nFrameFormat = -1;
    // Element Name
    m_omStrElementName = STR_EMPTY;
    // Value Type
    m_nValueType = -1;
    // Graph Type
    m_nLineType = 0; // For Solid
    // Default Color
    m_nLineColor = RED_COLOR;
    // Sample point type
    m_nPointType = 0; // None
    // Sample point color
    m_nPointColor = 0; // Black
    // Visible
    m_bVisible = TRUE;
    // Enabled
    m_bEnabled = TRUE;
    //Display Type Normal
    m_eDisplayType = eDISPLAY_NORMAL;
}

/*******************************************************************************
 Function Name  : CGraphElement
 Description    : Standard Destructor
 Member of      : CGraphElement
 Functionality  : -
 Author(s)      : Raja N
 Date Created   : 01.12.2004
*******************************************************************************/
CGraphElement::~CGraphElement()
{
}

BYTE* CGraphElement::pbyGetConfigData(BYTE* pbyTrgData, BYTE byVersion)
{
    BYTE* pbyTemp = pbyTrgData;

    if (pbyTemp != NULL)
    {
        // Save properties one after another
        // Message ID
        COPY_DATA(pbyTemp, &m_nMsgID, sizeof(int));
        // Frame Format - Standard
        COPY_DATA(pbyTemp, &m_nFrameFormat, sizeof(short));
        // Element Name String
        char acName[MAX_PATH] = {_T('\0')};
        strcpy_s(acName, m_omStrElementName.GetBuffer(MAX_PATH));
        COPY_DATA(pbyTemp, acName, (sizeof(char) * MAX_PATH));
        // Type of the element val
        COPY_DATA(pbyTemp, &m_nValueType, sizeof(int));
        // Line type of the elemen
        COPY_DATA(pbyTemp, &m_nLineType, sizeof(int));
        // Line Color of the eleme
        COPY_DATA(pbyTemp, &m_nLineColor, sizeof(int));
        // Sample point symbol typ
        COPY_DATA(pbyTemp, &m_nPointType, sizeof(int));
        // Sample point symbol col
        COPY_DATA(pbyTemp, &m_nPointColor, sizeof(int));
        // Visible or Not
        COPY_DATA(pbyTemp, &m_bVisible, sizeof(BOOL));
        // Enabled or not
        COPY_DATA(pbyTemp, &m_bEnabled, sizeof(BOOL));

        if(byVersion == 2)
        {
            // Message Name
            char MsgName[MAX_PATH] = {_T('\0')};
            strcpy_s(MsgName, m_strMsgName.GetBuffer(MAX_PATH));
            COPY_DATA(pbyTemp, MsgName, (sizeof(char) * MAX_PATH));
            //Line Display type
            COPY_DATA(pbyTemp, &m_eDisplayType, sizeof(eDISPLAY_TYPE));
        }
    }

    return pbyTemp;
}

BYTE* CGraphElement::pbySetConfigData(BYTE* pbyTrgData, BYTE byVersion)
{
    BYTE* pbyTemp = pbyTrgData;

    if (pbyTemp != NULL)
    {
        // Save properties one after another
        // Message ID
        COPY_DATA_2(&m_nMsgID, pbyTemp, sizeof(int));
        // Frame Format - Standard
        COPY_DATA_2(&m_nFrameFormat, pbyTemp, sizeof(short));
        // Element Name String
        char acName[MAX_PATH] = {_T('\0')};
        COPY_DATA_2(acName, pbyTemp, (sizeof(char) * MAX_PATH));
        m_omStrElementName.Format("%s", acName);
        // Type of the element val
        COPY_DATA_2(&m_nValueType, pbyTemp, sizeof(int));
        // Line type of the elemen
        COPY_DATA_2(&m_nLineType, pbyTemp, sizeof(int));
        // Line Color of the eleme
        COPY_DATA_2(&m_nLineColor, pbyTemp, sizeof(int));
        // Sample point symbol typ
        COPY_DATA_2(&m_nPointType, pbyTemp, sizeof(int));
        // Sample point symbol col
        COPY_DATA_2(&m_nPointColor, pbyTemp, sizeof(int));
        // Visible or Not
        COPY_DATA_2(&m_bVisible, pbyTemp, sizeof(BOOL));
        // Enabled or not
        COPY_DATA_2(&m_bEnabled, pbyTemp, sizeof(BOOL));

        if(byVersion == 2)
        {
            //Message Name
            char MsgName[MAX_PATH] = {_T('\0')};
            COPY_DATA_2(MsgName, pbyTemp, (sizeof(char) * MAX_PATH));
            m_strMsgName.Format("%s", MsgName);
            //Line Display type
            COPY_DATA_2(&m_eDisplayType, pbyTemp, sizeof(eDISPLAY_TYPE));
        }
        else if(byVersion == 1)
        {
            m_strMsgName = GetIMsgDB()->omStrGetMessageNameFromMsgCode(m_nMsgID);
            m_eDisplayType = eDISPLAY_NORMAL;
        }
    }

    return pbyTemp;
}
/*******************************************************************************
 Function Name  : nSerialize
 Input(s)       : omArchive - CArchive class. Serialisation stream
 Output         : -
 Functionality  : This function will serialise this class using CArchive stream
 Member of      : CGraphElement
 Author(s)      : Raja N
 Date Created   : 01.12.2004
 Modifications  : ArunKumar K
                  28.10.2010
                  Addition of m_eDisplayType variable for serialization.
*******************************************************************************/
int CGraphElement::nSerialize(CArchive& omArch)
{
    int nReturn = 0;

    // If it is storing
    try
    {
        if( omArch.IsStoring() )
        {
            // Save properties one after another
            // Message ID
            omArch << m_nMsgID  ;
            // Frame Format - Standard
            omArch << m_nFrameFormat;
            // Element Name String
            omArch << m_omStrElementName;
            // Type of the element val
            omArch << m_nValueType;
            // Line type of the elemen
            omArch << m_nLineType;
            // Line Color of the eleme
            omArch << m_nLineColor;
            // Sample point symbol typ
            omArch << m_nPointType;
            // Sample point symbol col
            omArch << m_nPointColor;
            // Visible or Not
            omArch << m_bVisible;
            // Enabled or not
            omArch << m_bEnabled;
        }
        // This is for loading
        else
        {
            // Load properties one after another
            omArch >> m_nMsgID;
            // Frame Format - Standard
            omArch >> m_nFrameFormat;
            // Element Name String
            omArch >> m_omStrElementName;
            // Type of the element val
            omArch >> m_nValueType;
            // Line type of the elemen
            omArch >> m_nLineType;
            // Line Color of the eleme
            omArch >> m_nLineColor;
            // Sample point symbol typ
            omArch >> m_nPointType;
            // Sample point symbol col
            omArch >> m_nPointColor;
            // Visible or Not
            omArch >> m_bVisible;
            // Enabled or not
            omArch >> m_bEnabled;
        }
    }
    catch(CArchiveException* poArchExcep)
    {
        // Get the Exception code and delete the dynamic object
        nReturn = poArchExcep->m_cause;
        poArchExcep->Delete();
    }

    // Return the result
    return nReturn;
}

/*******************************************************************************
 Function Name  : unGetConfigSize
 Input(s)       : byVersion
 Output         : -
 Functionality  : This function will get the size of the class data members.
 Member of      : CGraphElement
 Author(s)      : ArunKumar K
 Date Created   : 16.12.2010
 Modifications  :
*******************************************************************************/
UINT CGraphElement::unGetConfigSize(BYTE byVersion)
{
    UINT unSize = 0;
    unSize += sizeof(int);//     m_nMsgID;
    // Frame Format - Standard or Extended
    unSize += sizeof(short);//   m_nFrameFormat;
    // Element Name String
    unSize += (sizeof(char) * MAX_PATH);//m_omStrElementName
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

    if(byVersion == 2)
    {
        // Line Display
        unSize += sizeof(int);//    m_eDisplayType;
        //Message Name String
        unSize += (sizeof(char) * MAX_PATH);//m_strMsgName
    }

    return unSize;
}

