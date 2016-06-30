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
* \file      ITransmitter.h
* \author    Robin G.K.
* \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
*/

#pragma once
#include"TxDataStore.h"

class ITransmitter
{
public:
    virtual int SetTxDataStore(CTxDataStore* pouTxDataStore) = 0;
    virtual int SetClientId(unsigned long ulClientId) = 0;
    virtual int OnBusStatusChanged(ESTATUS_BUS eBusStatus) = 0;
    virtual int TransmitAt(int nIndex) = 0;
    virtual int TransmitAll() = 0;
    virtual int OnKeyPressed(char chKey) = 0;
};