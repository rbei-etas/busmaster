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
 * \file      DataBaseMsgList.h
 * \author    Venkatanarayana makam
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 */

#pragma once

#include "DataTypes\MsgSignal_Datatypes.h"
#include "Application\DataType.h"
#include "Application\hashdefines.h"
#include "TSDefinitions.h"

class CDataBaseMsgList
{
//Member Functions
public:
    CDataBaseMsgList(void);
    virtual ~CDataBaseMsgList(void);
    BOOL bFillDataStructureFromDatabaseFile( CString strFileName);
    BOOL bValidateDatabaseFile(CString strFileName);
    BOOL bIsValidMessageID(UINT dwMsgId);
    BOOL bFreeMessageMemory();
    INT nFillMessageList(CComboBox& omDbMsgCombo, BOOL bEmptyString = FALSE);
    INT nFillMessageList(CStringArray& omstrEntryList, BOOL bEmptyString = FALSE);
    INT nGetMessageName(UINT unMsgId, CString& omstrMsgName);
    INT nGetSignalList(CString omstrMsgName, sSIGNALS* psSignals);
    INT nGetMessageInfo(CString omstrMsgName, sMESSAGE& sMsg);
    UINT unGetMessageID(CString omstrMsgName);
    sMESSAGE* unGetMsg(UINT unMsgId);
//Member Variables
private:
    sMESSAGE *m_psMessages;
    UINT    m_unMessageCount;
private:
    
};
