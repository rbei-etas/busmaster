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
 * \file      GraphElement.h
 * \brief     Interface file for CGraphElement class
 * \author    Raja N
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Interface file for CGraphElement class
 */

#pragma once

typedef enum eELEMENT_TYPE
{
    eSTAT_PARAM,
    eRAW_VALUE,
    ePHY_VALUE
};


class CGraphElement
{
public:
    // Variables
    // Message ID Variable
    int     m_nMsgID;
    // Message ID Name
    CString     m_strMsgName;
    // Frame Format - Standard or Extended
    short   m_nFrameFormat;
    // Element Name String
    CString m_omStrElementName;

    // Type of the element value
    // 0 - network statistics parameter
    // 1 - Raw Value of the Signal
    // 2 - Physical Value of the signal
    int     m_nValueType;
    // Line type of the element
    int     m_nLineType;
    // Line Color of the element
    int     m_nLineColor;
    // Sample point symbol type
    int     m_nPointType;
    // Sample point symbol color
    int     m_nPointColor;
    // Visible or Not
    BOOL    m_bVisible;
    // Enabled or not
    BOOL    m_bEnabled;
    //display type for signal
    eDISPLAY_TYPE m_eDisplayType;

    // Functions
    // To serialize this class
    int nSerialize(CArchive& omArch);
    BYTE* pbySetConfigData(BYTE* pbyTrgtData, BYTE byVersion);
    BYTE* pbyGetConfigData(BYTE* pbyTrgtData, BYTE byVersion);
    // Default Constructor & destructor
    CGraphElement();
    virtual ~CGraphElement();

    static UINT unGetConfigSize(BYTE byVersion);

};
