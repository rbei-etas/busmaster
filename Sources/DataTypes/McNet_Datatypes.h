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
 * \file      McNet_Datatypes.h
 * \brief     Definition file for McNet data types.
 * \author    Ratnadip Choudhury
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Definition file for McNet data types.
 */

#ifndef MCNET_DATATYPES_H__INCLUDED_
#define MCNET_DATATYPES_H__INCLUDED_



#include "afxtempl.h"

typedef enum
{
    REMOTE = 0,
    DIRECT,
    UNUSED
}ECONNECT_TRIGGER;

typedef enum
{
    WD_INACTIVE,
    WD_ACTIVE,
    WD_PRESENT,
    WD_MISSING
} EWD_STATUS;

typedef enum
{
    T_DISCONNECTED = 0,
    T_CONNECTED,
    T_STARTUP
} eCON_STATUS;

typedef enum
{
    T_FAULT = 0,
    T_SHUTDOWN
}eREASON;

typedef enum eTYPE_MCNET_MSG
{
    //Don't change the first item's position
    MCNET_INVALID     = 0,
    MCNET_LONG,
    MCNET_EXPEDITED,
    MCNET_BROADCAST,
    MCNET_WATCHDOG,
    MCNET_CON_SETUP,
    MCNET_CON_TEST,
    MCNET_CON_ACK,
    MCNET_DATA_ACK

} ETYPE_MCNET_MSG;

const SHORT LCN_MONITOR = 0x7FFF;

struct sTMCNET_MSG
{
    UINT64          m_u64TimeStamp; // Time stamp
    UINT            m_unMsgID;      // Message identifier
    SHORT           m_shLOCAL_LC;   // Sender identifier
    SHORT           m_shREMOTE_LC;  // Receiver identifier
    ETYPE_MCNET_MSG m_eMsgType;     // Type of the message
    USHORT          m_ushDLC;       // Data length
    BYTE*           m_pbData;       // Message data
    
    sTMCNET_MSG()
    {
        m_pbData = NULL;
    }
    ~sTMCNET_MSG()
    {
        if (m_pbData != NULL)
        {
            delete[] m_pbData;
            m_pbData = NULL;
        }
    }
    sTMCNET_MSG(int nDataSize);
    void vClear(int nDataSize);
    void vInitialize(int nDataSize);
};
typedef sTMCNET_MSG STMCNET_MSG;
typedef sTMCNET_MSG *PSTMCNET_MSG;

class CMcNetData
{
protected:
    USHORT m_ushMaxDLC;    // Maximum value of DLC


public:

    CMcNetData();
    ~CMcNetData();
    STMCNET_MSG m_sMcNetMsg;

    // Helper functions
    void vClearAll(void);
    // To set maximum capacity of the message data buffer
    HRESULT hSetMaxDLC(USHORT ushSize);
    // To get the maximum buffer size to hold McNet data
    USHORT ushGetMaxBufferSize(void) const;
    // To copy McNet data into a target buffer
    BYTE* pbCopyMcNetDataInto(BYTE* pbTarget, INT& Length) const;
    // To copy McNet data from a source buffer
    BYTE* pbCopyMcNetDataFrom(BYTE* pbSource, INT& Length);
};

// Special tailor-made realisation of McNet message structure & class: START
struct sTMCNET_MSG_SPL : public sTMCNET_MSG
{
    UINT64  m_u64DelTS; // Time stamp difference to the previous message

    sTMCNET_MSG_SPL() : sTMCNET_MSG()
    {
        m_u64DelTS = 0x0;
        m_u64TimeStamp = 0x0;
    };

    ~sTMCNET_MSG_SPL()
    {
    };

    sTMCNET_MSG_SPL(int nDataSize) : sTMCNET_MSG(nDataSize)
    {
        m_u64DelTS = 0x0;
        m_u64TimeStamp = 0x0;
    };

    void vClear(int nDataSize)
    {
        m_u64TimeStamp = 0x0;
        m_u64DelTS     = 0x0;
        sTMCNET_MSG::vClear(nDataSize);
    }
};

typedef sTMCNET_MSG_SPL STMCNET_MSG_SPL;
typedef sTMCNET_MSG_SPL *PSTMCNET_MSG_SPL;

#define LENGTH_STR_TS_MCNET      16
#define LENGTH_STR_ID_MCNET      16
#define LENGTH_STR_TYPE_MCNET    16
#define LENGTH_STR_NAME_MCNET    32
#define LENGTH_STR_DIR_MCNET     32
#define LENGTH_STR_DLC_MCNET     6

typedef struct tagFormattedData_MCNET
{
    tagFormattedData_MCNET();               // Constructor
    ~tagFormattedData_MCNET();              // Destructor
    void vClear(void);
    void vInitStrings();
    tagFormattedData_MCNET& operator=(const tagFormattedData_MCNET& refObj);

    TCHAR m_acTimeAbs[LENGTH_STR_TS_MCNET]; // Absolute time stamp
    TCHAR m_acTimeRel[LENGTH_STR_TS_MCNET]; // Relative time stamp
    TCHAR m_acTimeSys[LENGTH_STR_TS_MCNET]; // System time stamp

    TCHAR m_acMsgIDHex[LENGTH_STR_ID_MCNET];// Identifier in hex
    TCHAR m_acMsgIDDec[LENGTH_STR_ID_MCNET];// Identifier in dec

    TCHAR m_acDirection[LENGTH_STR_DIR_MCNET]; // Direction
    TCHAR m_acType[LENGTH_STR_TYPE_MCNET];  // long/ expedited/ network 
                                            // related / broadcast type
    TCHAR m_acName[LENGTH_STR_NAME_MCNET];
    TCHAR m_acDataLen[LENGTH_STR_DLC_MCNET];// DLC always in dec
    TCHAR* m_pcDataHex;                     // Data in hexadecimal
    TCHAR* m_pcDataDec;                     // Data in decimal

    __int64         m_n64MapId;             // Map id of the message
    COLORREF        m_ColourCode;           // Colour code associated
    UINT64          m_u64TimeStamp;         // For Offline time format 

} SFORMATTEDDATA_MCNET;
typedef SFORMATTEDDATA_MCNET *PSFORMATTEDDATA_MCNET;

typedef CMap<UINT, UINT, CString, CString> CMcNetMessageMap;


// Special tailor-made realisation of McNet message structure & class: START
class CMcNetDataSpl
{
protected:
    USHORT m_ushMaxDLC;    // Maximum value of DLC

public:

    CMcNetDataSpl();
    ~CMcNetDataSpl();
    STMCNET_MSG_SPL m_sMcNetMsgSpl;

    // Helper functions
    void vClearAll(void);
    // To copy data from CMcNetData.
    void vCopyFromCMcNetData(const CMcNetData& ouSource, UINT64 u64DelTS);
    // To set maximum capacity of the message data buffer
    HRESULT hSetMaxDLC(USHORT ushSize);
    // To get the maximum buffer size to hold McNet data
    USHORT ushGetMaxBufferSize(void) const;
    // To copy McNet data into a target buffer
    BYTE* pbCopyMcNetDataInto(BYTE* pbTarget, INT& Length) const;
    // To copy McNet data from a source buffer
    BYTE* pbCopyMcNetDataFrom(BYTE* pbSource, INT& Length);
};
// Special tailor-made realisation of McNet message structure & class: END


#endif // MCNET_DATATYPES_H__INCLUDED_
