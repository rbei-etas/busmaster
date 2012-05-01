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
 * \file      SimENG.h
 * \brief     Declaration of the CSimENG
 * \author    Ratnadip Choudhury
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Declaration of the CSimENG
 */

#pragma once

#include "resource_BusSim.h"       // main symbols
#include "BusEmulation.h"

// CSimENG

class ATL_NO_VTABLE CSimENG :
    public CComObjectRootEx<CComSingleThreadModel>,
    public CComCoClass<CSimENG, &CLSID_SimENG>,
    public IDispatchImpl<ISimENG, &IID_ISimENG, &LIBID_BusEmulationLib, /*wMajor =*/ 1, /*wMinor =*/ 0>
{
public:
    CSimENG();

    DECLARE_REGISTRY_RESOURCEID(IDR_SIMENG)

    DECLARE_CLASSFACTORY_SINGLETON(CSimENG) // To create one and only one object of
    // this class

    BEGIN_COM_MAP(CSimENG)
    COM_INTERFACE_ENTRY(ISimENG)
    COM_INTERFACE_ENTRY(IDispatch)
    END_COM_MAP()


    DECLARE_PROTECT_FINAL_CONSTRUCT()

    HRESULT FinalConstruct();
    void FinalRelease();
    //void UpdateState(void);

public:

    /* Call to transmit a message frame across the virtual bus. The data byte
    stream is the message frame without the additional two fields (i.e.,
    Tx/Rx flag (=1 byte) and time stamp (= sizeof(UINT64)) */
    STDMETHOD(SendMessage)(USHORT ClientID, USHORT CurrDataLength, BYTE pbCurrDataByte[128]);

    /* To register as a client node in a bus. Simulation engine doesn't contain
    any information on specific bus (i.e., CAN, FlexRay, MCNet, J1939 etc). So,
    its the responsibility of the user to provide diferent values for different
    bus nodes. MaxDataLen should be the length of a frame not including the
    Rx/Tx flag and time stamp fields. Rest all are [out] parameters. */
    STDMETHOD(RegisterClient)(USHORT Bus, USHORT MaxDataLen, USHORT* ClientID, BSTR* PipeName, BSTR* EventName);

    STDMETHOD(UnregisterClient)(USHORT ClientID);
    STDMETHOD(ConnectNode)(USHORT ClientID);
    STDMETHOD(DisconnectNode)(USHORT ClientID);
    STDMETHOD(GetTimeModeMapping)(SYSTEMTIME* CurrSysTime, ULONGLONG* TimeStamp, LARGE_INTEGER* lQueryTickCount);
    STDMETHOD(GetCurrentStatus)(USHORT ClientID, VARIANT* pNodeStatus);
};

OBJECT_ENTRY_AUTO(__uuidof(SimENG), CSimENG)
