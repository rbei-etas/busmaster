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
 * \file      Struct_BUS.cpp
 * \author    Ratnadip Choudhury
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 */
#include "DataTypes_stdafx.h"
#include "include/Struct_CAN.h"
#include "application/hashdefines.h"

int sTCANDATA::m_nSortField = 0;
int sTCANDATA::m_nMFactor = 1;

void sTCANDATA::vSetSortField(int nField)
{
    m_nSortField = nField;
}

void sTCANDATA::vSetSortAscending(bool bAscending)
{
    m_nMFactor = bAscending ? 1 : -1;
};

int sTCANDATA::DoCompareIndiv(const void* pEntry1, const void* pEntry2)
{
    int Result = 0;
    sTCANDATA* pDatCAN1 = (sTCANDATA*) pEntry1;
    sTCANDATA* pDatCAN2 = (sTCANDATA*) pEntry2;

    switch (m_nSortField)
    {
        case 3: // Sort by channel
        {
            Result = (int) (pDatCAN1->m_uDataInfo.m_sCANMsg.m_ucChannel - pDatCAN2->m_uDataInfo.m_sCANMsg.m_ucChannel);
            Result *= m_nMFactor;

            if (Result != 0)
            {
                break;
            }
        }

        case 5: // Sort by CAN id
        {
            Result = (int) (pDatCAN1->m_uDataInfo.m_sCANMsg.m_unMsgID - pDatCAN2->m_uDataInfo.m_sCANMsg.m_unMsgID);
            Result *= m_nMFactor;

            if (Result != 0)
            {
                break;
            }
        }

        case 1: // Sort by time stamp
        {
            Result = (int) (pDatCAN1->m_lTickCount.QuadPart - pDatCAN2->m_lTickCount.QuadPart);
            Result *= m_nMFactor;
        }
        break;

        default:
        {
            ASSERT(FALSE);
        }
        break;
    }

    return Result;
};

__int64 sTCANDATA::GetSlotID(sTCANDATA& pDatCAN)
{
    STCAN_MSG& sMsg = pDatCAN.m_uDataInfo.m_sCANMsg;
    // Form message to get message index in the CMap
    int nMsgID = MAKE_RX_TX_MESSAGE( sMsg.m_unMsgID,
                                     IS_RX_MESSAGE(pDatCAN.m_ucDataType));
    nMsgID = MAKE_DEFAULT_MESSAGE_TYPE(nMsgID);

    // For extended message
    if (sMsg.m_ucEXTENDED)
    {
        nMsgID = MAKE_EXTENDED_MESSAGE_TYPE(nMsgID);
    }

    // Apply Channel Information
    __int64 n64MapIndex = MAKE_CHANNEL_SPECIFIC_MESSAGE( nMsgID,
                          sMsg.m_ucChannel );
    return n64MapIndex;
};

