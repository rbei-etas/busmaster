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
 * \file      ReplayFile.h
 * \brief     Interface file for CReplayFile class
 * \author    Raja N
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Interface file for CReplayFile class
 */

#pragma once

#include "Application/struct.h"

class CReplayFile
{
public:
    // Methods
    // Default constructor
    CReplayFile();
    // Copy Constructor
    CReplayFile(const CReplayFile& ouRef);
    // Assignment operator
    CReplayFile& operator = ( const CReplayFile& ouRef);
    // Don't use virtual as of not it is not going to be inherited
    // and to have optimization
    ~CReplayFile();
    // To serialise the class
    BOOL bSerialize(CArchive& omArchive);
    // Friends of this class
    friend class CReplayFileConfigDlg;
    friend class CReplayManager;
    friend class CReplayProcess;
    friend class CMsgReplayWnd;
    UINT  unGetConfigSize();
    BYTE* pbySaveConfig(BYTE* pDesBuffer);
    BYTE* pbyLoadConfig(BYTE* pSrcBuffer);
    BOOL  bisConfigChanged(BYTE* &pSrcBuffer);

public:
    // Members
    SFILTERAPPLIED_CAN m_sFilterApplied;
private:
    // Methods
    void vCopyContent(const CReplayFile& ouRef);
    UINT  unGetConfigSizeOfCommonMembers();

private:
    // Members, tmp members for config comparision so they have to be of same data type
    CString m_omStrFileName, m_omNameTmp;    // Replay File Name
    int     m_nTimeMode, m_nTimeModeTmp;        // 0 - Retain Delay, 1 - Specific Delay
    UINT    m_unMsgTimeDelay, m_unTimeDelTmp;   // Time delay between messages in case time mode = 1
    int     m_nReplayMode, m_nRepModeTmp;      // 0 - Monoshot, 1 - Cyclic
    UINT    m_unCycleTimeDelay, m_unCycleTimeTmp; // Cyclic delay in case replay mode = 1
    //CModuleFilterArray m_sFilter; // Filter List
    BOOL    m_bEnabled, m_bEnabledTmp;         // To indicate replay is enabled or not
    BOOL    m_bInteractive, m_bInteractiveTmp;     // 0 - Non Interactive, 1 - interactive
};
