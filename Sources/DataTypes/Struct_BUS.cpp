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
#include "DataTypes/Base_FlexRay_Buffer.h"
#include "include/Struct_LIN.h"
#include "include/Struct_ETHERNET.h"
#include "application/hashdefines.h"
#include "application/Common.h"

int sTCANDATA::m_nSortField = 0;
int sTCANDATA::m_nMFactor = 1;

int sTLINDATA::m_nSortField = 0;
int sTLINDATA::m_nMFactor = 1;

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
        case 6: // Sort by message name
        {
            CString str1, str2;
            AfxGetMainWnd()->SendMessage(WM_GET_MSG_NAME_FROM_CODE, (WPARAM)pDatCAN1->m_uDataInfo.m_sCANMsg.m_unMsgID, (LPARAM)&str1);
            AfxGetMainWnd()->SendMessage(WM_GET_MSG_NAME_FROM_CODE, (WPARAM)pDatCAN2->m_uDataInfo.m_sCANMsg.m_unMsgID, (LPARAM)&str2);

            Result = (int) (str1.CompareNoCase(str2));
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
        case 3: // Sort by channel
        {
            Result = (int) (pDatCAN1->m_uDataInfo.m_sCANMsg.m_ucChannel - pDatCAN2->m_uDataInfo.m_sCANMsg.m_ucChannel);
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
            ASSERT(false);
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

/*----------------------------- FlexRay Implementation -----------------------------*/

void s_FLXMSG::vSetSortField(int /* nField */)
{
}

void s_FLXMSG::vSetSortAscending(bool /* bAscending */)
{
};

int s_FLXMSG::DoCompareIndiv(const void* /* pEntry1 */, const void* /* pEntry2 */)
{
    return 0;
};

__int64 s_FLXMSG::GetSlotID(struct_UCI_FLXMSG& /* pDatFLEX */)
{
    return 0;
};

void sTLINDATA::vSetSortField(int nField)
{
    m_nSortField = nField;
}

void sTLINDATA::vSetSortAscending(bool bAscending)
{
    m_nMFactor = bAscending ? 1 : -1;
};

int sTLINDATA::DoCompareIndiv(const void* pEntry1, const void* pEntry2)
{
    int Result = 0;

    sTLINDATA* pDatLIN1 = (sTLINDATA*) pEntry1;
    sTLINDATA* pDatLIN2 = (sTLINDATA*) pEntry2;

    switch (m_nSortField)
    {
        case 6: // Sort by message name
        {
            CString str1, str2;
            AfxGetMainWnd()->SendMessage(WM_GET_MSG_NAME_FROM_CODE, (WPARAM)pDatLIN1->m_uDataInfo.m_sLINMsg.m_ucMsgID, (LPARAM)&str1);
            AfxGetMainWnd()->SendMessage(WM_GET_MSG_NAME_FROM_CODE, (WPARAM)pDatLIN2->m_uDataInfo.m_sLINMsg.m_ucMsgID, (LPARAM)&str2);

            Result = (int) (str1.CompareNoCase(str2));
            Result *= m_nMFactor;

            if (Result != 0)
            {
                break;
            }
        }
        case 5: // Sort by LIN id
        {
            Result = (int) (pDatLIN1->m_uDataInfo.m_sLINMsg.m_ucMsgID - pDatLIN2->m_uDataInfo.m_sLINMsg.m_ucMsgID);
            Result *= m_nMFactor;
            if (Result != 0)
            {
                break;
            }
        }
        case 3: // Sort by channel
        {
            Result = (int) (pDatLIN1->m_uDataInfo.m_sLINMsg.m_ucChannel - pDatLIN2->m_uDataInfo.m_sLINMsg.m_ucChannel);
            Result *= m_nMFactor;
            if (Result != 0)
            {
                break;
            }
        }
        case 1: // Sort by time stamp
        {
            Result = (int) (pDatLIN1->m_lTickCount.QuadPart - pDatLIN2->m_lTickCount.QuadPart);
            Result *= m_nMFactor;
        }
        break;
        default:
        {
            ASSERT(false);
        }
        break;
    }
    return Result;
};

__int64 sTLINDATA::GetSlotID(sTLINDATA& pDatLIN)
{
    STLIN_MSG& sMsg = pDatLIN.m_uDataInfo.m_sLINMsg;
    // Form message to get message index in the CMap
    int nMsgID = MAKE_RX_TX_MESSAGE( sMsg.m_ucMsgID,
                                     IS_RX_MESSAGE(pDatLIN.m_ucDataType));

    nMsgID = MAKE_DEFAULT_MESSAGE_TYPE(nMsgID);
    // For extended message

    // Apply Channel Information
    __int64 n64MapIndex = MAKE_CHANNEL_SPECIFIC_MESSAGE( nMsgID,
                          sMsg.m_ucChannel );
    return n64MapIndex;
};

/*----------------------------- Ethernet Implementation -----------------------------*/
void sTETHERNETDATA::vSetSortField(int nField)
{
    //m_nSortField = nField;
}

void sTETHERNETDATA::vSetSortAscending(bool bAscending)
{
    //m_nMFactor = bAscending ? 1 : -1;
};

int sTETHERNETDATA::DoCompareIndiv(const void* pEntry1, const void* pEntry2)
{
    int Result = 0;
    //}
    return Result;
};

__int64 sTETHERNETDATA::GetSlotID(sTETHERNETDATA& pDatFLEX)
{
    return 0;
};

/*-----------------------------------------------------------------------------*/