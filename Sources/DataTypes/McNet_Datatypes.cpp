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
 * \file      McNet_Datatypes.cpp
 * \brief     Source file for McNet data types.
 * \author    Ratnadip Choudhury
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Source file for McNet data types.
 */


#include "DataTypes_StdAfx.h"
#include "include/Error.h"
#include "include/Utils_macro.h"
#include "McNet_Datatypes.h"

/* sTMCNET_MSG ---- STARTS */
sTMCNET_MSG::sTMCNET_MSG(int nDataSize)
{
    if (m_pbData != NULL)
    {
        delete[] m_pbData;
        m_pbData = NULL;
    }
    m_pbData = new BYTE[nDataSize];
}
void sTMCNET_MSG::vClear(int nDataSize)
{
    if (m_pbData != NULL)
    {
        memset(m_pbData, 0, nDataSize);
    }
}
void sTMCNET_MSG::vInitialize(int nDataSize)
{
    if (m_pbData != NULL)
    {
        delete[] m_pbData;
        m_pbData = NULL;
    }
    m_pbData = new BYTE[nDataSize];
    memset(m_pbData, 0, nDataSize);
}
/* sTMCNET_MSG ---- ENDS   */

/* CMcNetData --- STARTS */
CMcNetData::CMcNetData()
{
    m_sMcNetMsg.m_pbData = NULL;
    m_sMcNetMsg.m_ushDLC = (USHORT) 0;
    m_ushMaxDLC          = (USHORT) 0;
}

CMcNetData::~CMcNetData()
{
    vClearAll();
}

void CMcNetData::vClearAll(void)
{
    if (NULL != m_sMcNetMsg.m_pbData)
    {
        delete[] m_sMcNetMsg.m_pbData;
        m_sMcNetMsg.m_pbData = NULL;
        m_sMcNetMsg.m_ushDLC = (USHORT) 0;
        m_ushMaxDLC          = (USHORT) 0;
    }
}

HRESULT CMcNetData::hSetMaxDLC(USHORT ushSize)
{
    HRESULT Result = S_OK; 

    if (ushSize != m_ushMaxDLC)
    {
        vClearAll();    // Clear all
        if ((m_sMcNetMsg.m_pbData = new BYTE[ushSize]) != NULL)
        {
            m_ushMaxDLC = ushSize;
        }
        else
        {
            Result = ERR_NO_DYNAMIC_MEM;
        }
    }
    if (S_OK == Result)
    {
        memset(m_sMcNetMsg.m_pbData, '\0', m_ushMaxDLC);
    }

    return Result;
}

USHORT CMcNetData::ushGetMaxBufferSize(void) const
{
    USHORT Result = sizeof(m_sMcNetMsg.m_u64TimeStamp) + 
                 sizeof(m_sMcNetMsg.m_unMsgID) + sizeof(m_sMcNetMsg.m_eMsgType)
                 + sizeof(m_sMcNetMsg.m_ushDLC) + m_ushMaxDLC;
    return Result;
}

BYTE* CMcNetData::pbCopyMcNetDataInto(BYTE* pbTarget, INT& Length) const
{
    BYTE* pbTStream = pbTarget;

    COPY_DATA(pbTStream, &(m_sMcNetMsg.m_u64TimeStamp), sizeof(m_sMcNetMsg.m_u64TimeStamp)  );
    COPY_DATA(pbTStream, &(m_sMcNetMsg.m_unMsgID),      sizeof(m_sMcNetMsg.m_unMsgID)       );
    COPY_DATA(pbTStream, &(m_sMcNetMsg.m_shLOCAL_LC),   sizeof(m_sMcNetMsg.m_shLOCAL_LC)    );
    COPY_DATA(pbTStream, &(m_sMcNetMsg.m_shREMOTE_LC),  sizeof(m_sMcNetMsg.m_shREMOTE_LC)   );
    COPY_DATA(pbTStream, &(m_sMcNetMsg.m_eMsgType),     sizeof(m_sMcNetMsg.m_eMsgType)      );
    COPY_DATA(pbTStream, &(m_sMcNetMsg.m_ushDLC),       sizeof(m_sMcNetMsg.m_ushDLC)        );
    COPY_DATA(pbTStream, m_sMcNetMsg.m_pbData,          m_sMcNetMsg.m_ushDLC                );

    Length = (INT) (pbTStream - pbTarget);

    return pbTStream;
}

BYTE* CMcNetData::pbCopyMcNetDataFrom(BYTE* pbSource, INT& Length)
{
    BYTE* pbSStream = pbSource;

    COPY_DATA_2(&(m_sMcNetMsg.m_u64TimeStamp), pbSStream, sizeof(m_sMcNetMsg.m_u64TimeStamp));
    COPY_DATA_2(&(m_sMcNetMsg.m_unMsgID),      pbSStream, sizeof(m_sMcNetMsg.m_unMsgID)     );
    COPY_DATA_2(&(m_sMcNetMsg.m_shLOCAL_LC),   pbSStream, sizeof(m_sMcNetMsg.m_shLOCAL_LC)  );
    COPY_DATA_2(&(m_sMcNetMsg.m_shREMOTE_LC),  pbSStream, sizeof(m_sMcNetMsg.m_shREMOTE_LC) );
    COPY_DATA_2(&(m_sMcNetMsg.m_eMsgType),     pbSStream, sizeof(m_sMcNetMsg.m_eMsgType)    );
    COPY_DATA_2(&(m_sMcNetMsg.m_ushDLC),       pbSStream, sizeof(m_sMcNetMsg.m_ushDLC)      );
    COPY_DATA_2(m_sMcNetMsg.m_pbData,          pbSStream, m_sMcNetMsg.m_ushDLC              );

    Length = (INT) (pbSStream - pbSource);

    return pbSStream;
}

/* CMcNetData --- ENDS */

/* tagFormattedData_MCNET --- STARTS */
tagFormattedData_MCNET::tagFormattedData_MCNET()
{
    m_pcDataHex = NULL;
    m_pcDataDec = NULL;
    memset(m_acTimeAbs, '\0', sizeof(TCHAR) * LENGTH_STR_TS_MCNET);
    memset(m_acTimeRel, '\0', sizeof(TCHAR) * LENGTH_STR_TS_MCNET);
    memset(m_acTimeSys, '\0', sizeof(TCHAR) * LENGTH_STR_TS_MCNET);

    memset(m_acMsgIDHex, '\0', sizeof(TCHAR) * LENGTH_STR_ID_MCNET);
    memset(m_acMsgIDDec, '\0', sizeof(TCHAR) * LENGTH_STR_ID_MCNET);

    memset(m_acDirection, '\0', sizeof(TCHAR) * LENGTH_STR_DIR_MCNET);
    memset(m_acType, '\0', sizeof(TCHAR) * LENGTH_STR_TYPE_MCNET);

    memset(m_acName, '\0', sizeof(TCHAR) * LENGTH_STR_NAME_MCNET);
    memset(m_acDataLen, '\0', sizeof(TCHAR) * LENGTH_STR_DLC_MCNET);
}

tagFormattedData_MCNET::~tagFormattedData_MCNET()
{
    vClear();
}

void tagFormattedData_MCNET::vClear(void)
{
    if (NULL != m_pcDataHex)
    {
        delete[] m_pcDataHex;
        m_pcDataHex = NULL;
    }
    if (NULL != m_pcDataDec)
    {
        delete[] m_pcDataDec;
        m_pcDataDec = NULL;
    }
}
tagFormattedData_MCNET& tagFormattedData_MCNET::
    operator=(const tagFormattedData_MCNET& refObj)
{
    _tcscpy(m_acTimeAbs, refObj.m_acTimeAbs);
    _tcscpy(m_acTimeRel, refObj.m_acTimeRel);
    _tcscpy(m_acTimeSys, refObj.m_acTimeSys);

    _tcscpy(m_acMsgIDHex, refObj.m_acMsgIDHex);
    _tcscpy(m_acMsgIDDec, refObj.m_acMsgIDDec);

    _tcscpy(m_acDirection, refObj.m_acDirection);
    _tcscpy(m_acType, refObj.m_acType);

    _tcscpy(m_pcDataDec, refObj.m_pcDataDec);
    _tcscpy(m_pcDataHex, refObj.m_pcDataHex);

    m_n64MapId   = refObj.m_n64MapId;
    m_ColourCode = refObj.m_ColourCode;
    return *this;
}

void tagFormattedData_MCNET::vInitStrings()
{
    if (m_pcDataDec != NULL)
    {
        m_pcDataDec[0] = '\0';
    }
    if (m_pcDataHex != NULL)
    {
        m_pcDataHex[0] = '\0';
    }
    memset(m_acTimeAbs, '\0', sizeof(TCHAR) * LENGTH_STR_TS_MCNET);
    memset(m_acTimeRel, '\0', sizeof(TCHAR) * LENGTH_STR_TS_MCNET);
    memset(m_acTimeSys, '\0', sizeof(TCHAR) * LENGTH_STR_TS_MCNET);

    memset(m_acMsgIDHex, '\0', sizeof(TCHAR) * LENGTH_STR_ID_MCNET);
    memset(m_acMsgIDDec, '\0', sizeof(TCHAR) * LENGTH_STR_ID_MCNET);

    memset(m_acDirection, '\0', sizeof(TCHAR) * LENGTH_STR_DIR_MCNET);
    memset(m_acType, '\0', sizeof(TCHAR) * LENGTH_STR_TYPE_MCNET);

    memset(m_acName, '\0', sizeof(TCHAR) * LENGTH_STR_NAME_MCNET);
    memset(m_acDataLen, '\0', sizeof(TCHAR) * LENGTH_STR_DLC_MCNET);
}


/* tagFormattedData_MCNET --- ENDS */


/* CMcNetDataSpl --- STARTS */
CMcNetDataSpl::CMcNetDataSpl()
{
    m_sMcNetMsgSpl.m_pbData = NULL;
    m_sMcNetMsgSpl.m_ushDLC = (USHORT) 0;
    m_ushMaxDLC          = (USHORT) 0;
}

CMcNetDataSpl::~CMcNetDataSpl()
{
    vClearAll();
}

void CMcNetDataSpl::vClearAll(void)
{
    if (NULL != m_sMcNetMsgSpl.m_pbData)
    {
        delete[] m_sMcNetMsgSpl.m_pbData;
        m_sMcNetMsgSpl.m_pbData = NULL;
        m_sMcNetMsgSpl.m_ushDLC = (USHORT) 0;
        m_ushMaxDLC          = (USHORT) 0;
    }
}

void CMcNetDataSpl::vCopyFromCMcNetData(const CMcNetData& ouSource, UINT64 u64DelTS)
{
    m_sMcNetMsgSpl.m_u64TimeStamp = ouSource.m_sMcNetMsg.m_u64TimeStamp;
    m_sMcNetMsgSpl.m_u64DelTS     = u64DelTS;
    m_sMcNetMsgSpl.m_unMsgID      = ouSource.m_sMcNetMsg.m_unMsgID;
    m_sMcNetMsgSpl.m_shLOCAL_LC   = ouSource.m_sMcNetMsg.m_shLOCAL_LC;
    m_sMcNetMsgSpl.m_shREMOTE_LC  = ouSource.m_sMcNetMsg.m_shREMOTE_LC;
    m_sMcNetMsgSpl.m_eMsgType     = ouSource.m_sMcNetMsg.m_eMsgType;
    m_sMcNetMsgSpl.m_ushDLC       = ouSource.m_sMcNetMsg.m_ushDLC;

    memcpy(m_sMcNetMsgSpl.m_pbData, ouSource.m_sMcNetMsg.m_pbData, 
           m_sMcNetMsgSpl.m_ushDLC);
}

HRESULT CMcNetDataSpl::hSetMaxDLC(USHORT ushSize)
{
    HRESULT Result = S_OK; 

    if (ushSize != m_ushMaxDLC)
    {
        vClearAll();    // Clear all
        if ((m_sMcNetMsgSpl.m_pbData = new BYTE[ushSize]) != NULL)
        {
            m_ushMaxDLC = ushSize;
        }
        else
        {
            Result = ERR_NO_DYNAMIC_MEM;
        }
    }
    if (S_OK == Result)
    {
        memset(m_sMcNetMsgSpl.m_pbData, '\0', m_ushMaxDLC);
    }

    return Result;
}

USHORT CMcNetDataSpl::ushGetMaxBufferSize(void) const
{
    USHORT Result = sizeof(m_sMcNetMsgSpl.m_u64TimeStamp) + 
        sizeof(m_sMcNetMsgSpl.m_u64DelTS) + sizeof(m_sMcNetMsgSpl.m_unMsgID) + 
        sizeof(m_sMcNetMsgSpl.m_eMsgType) + sizeof(m_sMcNetMsgSpl.m_ushDLC) + 
        m_ushMaxDLC;
    return Result;
}

BYTE* CMcNetDataSpl::pbCopyMcNetDataInto(BYTE* pbTarget, INT& Length) const
{
    BYTE* pbTStream = pbTarget;

    COPY_DATA(pbTStream, &(m_sMcNetMsgSpl.m_u64TimeStamp), sizeof(m_sMcNetMsgSpl.m_u64TimeStamp)  );
    COPY_DATA(pbTStream, &(m_sMcNetMsgSpl.m_u64DelTS),     sizeof(m_sMcNetMsgSpl.m_u64DelTS)      );
    COPY_DATA(pbTStream, &(m_sMcNetMsgSpl.m_unMsgID),      sizeof(m_sMcNetMsgSpl.m_unMsgID)       );
    COPY_DATA(pbTStream, &(m_sMcNetMsgSpl.m_shLOCAL_LC),   sizeof(m_sMcNetMsgSpl.m_shLOCAL_LC)    );
    COPY_DATA(pbTStream, &(m_sMcNetMsgSpl.m_shREMOTE_LC),  sizeof(m_sMcNetMsgSpl.m_shREMOTE_LC)   );
    COPY_DATA(pbTStream, &(m_sMcNetMsgSpl.m_eMsgType),     sizeof(m_sMcNetMsgSpl.m_eMsgType)      );
    COPY_DATA(pbTStream, &(m_sMcNetMsgSpl.m_ushDLC),       sizeof(m_sMcNetMsgSpl.m_ushDLC)        );
    COPY_DATA(pbTStream, m_sMcNetMsgSpl.m_pbData,          m_sMcNetMsgSpl.m_ushDLC                );

    Length = (INT) (pbTStream - pbTarget);

    return pbTStream;
}

BYTE* CMcNetDataSpl::pbCopyMcNetDataFrom(BYTE* pbSource, INT& Length)
{
    BYTE* pbSStream = pbSource;

    COPY_DATA_2(&(m_sMcNetMsgSpl.m_u64TimeStamp), pbSStream, sizeof(m_sMcNetMsgSpl.m_u64TimeStamp));
    COPY_DATA_2(&(m_sMcNetMsgSpl.m_u64DelTS),     pbSStream, sizeof(m_sMcNetMsgSpl.m_u64DelTS)    );
    COPY_DATA_2(&(m_sMcNetMsgSpl.m_unMsgID),      pbSStream, sizeof(m_sMcNetMsgSpl.m_unMsgID)     );
    COPY_DATA_2(&(m_sMcNetMsgSpl.m_shLOCAL_LC),   pbSStream, sizeof(m_sMcNetMsgSpl.m_shLOCAL_LC)  );
    COPY_DATA_2(&(m_sMcNetMsgSpl.m_shREMOTE_LC),  pbSStream, sizeof(m_sMcNetMsgSpl.m_shREMOTE_LC) );
    COPY_DATA_2(&(m_sMcNetMsgSpl.m_eMsgType),     pbSStream, sizeof(m_sMcNetMsgSpl.m_eMsgType)    );
    COPY_DATA_2(&(m_sMcNetMsgSpl.m_ushDLC),       pbSStream, sizeof(m_sMcNetMsgSpl.m_ushDLC)      );
    COPY_DATA_2(m_sMcNetMsgSpl.m_pbData,          pbSStream, m_sMcNetMsgSpl.m_ushDLC              );

    Length = (INT) (pbSStream - pbSource);

    return pbSStream;
}

/* CMcNetDataSpl --- ENDS */
