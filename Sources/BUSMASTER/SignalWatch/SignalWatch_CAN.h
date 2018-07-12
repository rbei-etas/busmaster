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
 * \file      SignalWatch_CAN.h
 * \author    Ratnadip Choudhury
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 */

#pragma once

#include "SignalWatch_Resource.h"
//#include "Datatypes/MsgBufAll_Datatypes.h"
/* MSG INTERPRETATION */
#include "Utility/MsgInterpretation.h"
#include "Utility/Utility_Thread.h"
/* DIL CAN INTERFACE */
//#include "DIL_Interface_extern.h"
#include "MsgBufFSE.h"

#include "BaseSignalWatchImp.h"
class CSignalWatch_CAN : public CBaseSignalWatchImp
{
private:
    CMsgBufFSE<STCANDATA> m_ouCanBufFSE;

public:
    virtual HRESULT DoInitialization();
    virtual HRESULT StartSigWatchReadThread();
    CSignalWatch_CAN();
    ~CSignalWatch_CAN();
private:
    void vDisplayInSigWatchWnd(STCANDATA& sCanData);

private:
    int ReadCANDataBuffer( CSignalWatch_CAN* pSWCan );
    static DWORD WINAPI SigWatchDataReadThreadProc_C( LPVOID pVoid );

    INT nParseXMLColumn(xmlNodePtr pNode);

};