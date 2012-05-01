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
 * \file      Flags.h
 * \brief     Contains CFlags class definition
 * \author    Ratnadip Choudhury
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Contains CFlags class definition
 */

#pragma once

#include "NodeSimEx_Struct.h"

class CFlags
{
public:
    int nGetFlagStatus(eSIMSYSFLAG WhichFlag);
    VOID vSetFlagStatus(eSIMSYSFLAG WhichFlag, int nValue);
    CFlags();
    virtual ~CFlags();

private:
    BOOL m_bDllLoaded;
    BOOL m_bTimerButtonOn;
    BOOL m_bFunctionEditorOn;
    BOOL m_bMsgHandlerOn;
    BOOL m_bKeyHandlerOn;
    BOOL m_bErrorHandlerOn;
    BOOL m_bDLLHandlerOn;
    BOOL m_bALLHandler;
private:
    CCriticalSection m_omCriticalSec;
};
