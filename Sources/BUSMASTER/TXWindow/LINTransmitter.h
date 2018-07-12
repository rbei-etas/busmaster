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
* \file      LINTransmitter.h
* \author    Robin G.K.
* \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
*/

#pragma once
#include "stdafx.h"
#include "..\Utility\Utility_Thread.h"
#include"ITransmitter.h"

class CLINTransmitter:public ITransmitter
{
private:
    static CLINTransmitter* m_pouLINTransmitter;
    static int m_nRefCounter;
    unsigned long m_ulClientId;
    CTxDataStore* m_pouTxDataStore;
    ESTATUS_BUS m_eBusStatus;

    CPARAM_THREADPROC m_ouTransmitThread;
    static DWORD WINAPI TransmitThread(LPVOID pVoid);


    CLINTransmitter();
    ~CLINTransmitter();

    CLINTransmitter(const CLINTransmitter& ouLINTransmitter);
    CLINTransmitter& operator=(const CLINTransmitter&);

    //Helpers
    int nGetMsgAt(int nIndex, STLIN_MSG& ouLinMsg);
    int nPreMessageTransmission();
public:
    static ITransmitter* GetInstance();
    static void ReleaseInstance();



    int SetTxDataStore(CTxDataStore* pouTxDataStore);
    int SetClientId(unsigned long ulClientId);
    int OnBusStatusChanged(ESTATUS_BUS eBusStatus);
    int TransmitAt(int nIndex);
    int TransmitAll();
    int OnKeyPressed(char chKey);
};