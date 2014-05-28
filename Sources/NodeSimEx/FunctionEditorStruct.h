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
 * \file      FunctionEditorStruct.h
 * \author    Ratnadip Choudhury
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 */

#pragma once

#include "SimSysNodeInfo.h"

struct tagFuncProtoType
{
    CString m_omFuncName;
    CString m_omFuncPrototype;
};
typedef tagFuncProtoType sFuncProtoType;
typedef CList<sFuncProtoType, sFuncProtoType&> CFuncPrototypeList;

struct tagBusSpecificInfo
{
    ETYPE_BUS m_eBus;
    CString m_omBusName;
    CString m_omWrapperObjName;
    CString m_omHeaderFileName;
    CFuncPrototypeList m_odAPIFuncList;
    PVOID m_pvErrorEventList;
    PVOID m_pvDefinedMsgsList;
    tagBusSpecificInfo()
    {
        m_eBus = CAN;
        m_pvErrorEventList = nullptr;
        m_pvDefinedMsgsList = nullptr;
    }
    ~tagBusSpecificInfo()
    {
        if (m_pvErrorEventList != nullptr)
        {
            delete m_pvErrorEventList;
            m_pvErrorEventList = nullptr;
        }
        if (m_pvDefinedMsgsList != nullptr)
        {
            delete m_pvDefinedMsgsList;
            m_pvDefinedMsgsList = nullptr;
        }
    }
};
typedef tagBusSpecificInfo SBUS_SPECIFIC_INFO;
typedef SBUS_SPECIFIC_INFO* PSBUS_SPECIFIC_INFO;
