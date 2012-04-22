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
 * \file      GraphList.h
 * \brief     Interface file for CGraphList class
 * \author    Raja N
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Interface file for CGraphList class
 */

#pragma once

class CGraphList
{
public:
    int nSerialize(CArchive& omArch);
    BYTE* pbyGetConfigData(BYTE* pbyTrgtData, BYTE byVersion);
    BYTE* pbySetConfigData(BYTE* pbyTrgtData, BYTE byVersion);
    UINT unGetConfigSize(BYTE byVersion);
    // Default Constructor & destructor
    CGraphList();
    virtual ~CGraphList();

    // Variables
    // Graph control parameters
    CGraphParameters m_odGraphParameters;
    // Graph element array
    CArray<CGraphElement, CGraphElement&> m_omElementList;
};
