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
 * \file      BaseDefs.h
 * \brief     This contains various basic definitions.
 * \author    Ratnadip Choudhury
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * This contains various basic definitions.
 */

#if !defined BASEDEFS_H__INCLUDED_
#define BASEDEFS_H__INCLUDED_

#include "Struct_CAN.h"


/*Maximum possible length of a CAN*/
#define MAX_DATA_LEN_CAN 8
/*Maximum possible length of a J1939*/
#define MAX_DATA_LEN_J1939 1785
/*Maximum possible length of a MCNET*/
#define MAX_DATA_LEN_MCNET 0x7FFF

typedef enum eTYPE_BUS
{
    CAN     = 0,
    MCNET,
    J1939,
    FLEXRAY,
    LIN,
    MOST,
    BUS_TOTAL,
    BUS_INVALID
} ETYPE_BUS;

typedef enum eDirection
{
    DIR_RX = 'R',
    DIR_TX = 'T',
    DIR_ALL
} EDIRECTION;

typedef enum eFORMAT_DATA
{
    DATA_FORMAT_MOTOROLA = 0,
    DATA_FORMAT_INTEL
} EFORMAT_DATA;

typedef enum eMSG_INTERPRETATION_PROPERTY
{
    NON_INTERPRETABLE = 0,
    INTERPRETABLE,
    INTERPRETING,
    MODE_NONE
} EINTERPRET_MODE;

typedef enum eMSG_WND_PROPERTY
{
    NUMERIC = 0x1,
    TIME_MODE = 0x2,
    DISPLAY_MODE = 0x4
};

typedef enum  eTimerMode{
    TIME_MODE_RELATIVE = 0,
    TIME_MODE_SYSTEM,
    TIME_MODE_ABSOLUTE
} ETIMERMODE;

typedef enum eFormat {
    HEXADECIMAL = 0,
    DEC
} EFORMAT;

enum eMode {
    APPEND_MODE = 0,
    OVERWRITE_MODE,
    OVERWRITE_INTERPRET_MODE
};

enum eScroll {
    SCROLL = 0,
    FREEZE
};

enum eTreeItemStates { TREESTATE_TOGGLE, 
					   TREESTATE_EXPAND, 
					   TREESTATE_COLLAPSE };


typedef UINT TYPE_CHANNEL;

const TYPE_CHANNEL CHANNEL_All_UNSPECIFIED = (UINT)-1; // Unspecified channel
const TYPE_CHANNEL CAN_CHANNEL_ALL = 0;     // All channel notation
const TYPE_CHANNEL CHANNEL_CAN_MIN = 1;     // Minimum value of a CAN channel
const TYPE_CHANNEL CHANNEL_CAN_MAX = 32;    // Maximum value of a CAN channel
const TYPE_CHANNEL CHANNEL_ALLOWED = 16;
#define defNO_OF_CHANNELS CHANNEL_CAN_MAX

// Starts FlexRay related bus information
const TYPE_CHANNEL FLEXRAY_CHANNEL_AB = 65;

const BYTE TYPE_ID_CAN_NONE        = 0x0;
const BYTE TYPE_ID_CAN_STANDARD    = 0x1;
const BYTE TYPE_ID_CAN_EXTENDED    = 0x2;
const BYTE TYPE_ID_CAN_ALL         = 0x3;

const BYTE TYPE_MSG_CAN_NONE        = 0x0;
const BYTE TYPE_MSG_CAN_RTR         = 0x1;
const BYTE TYPE_MSG_CAN_NON_RTR     = 0x2;
const BYTE TYPE_MSG_CAN_ALL         = 0x3;


#define LENGTH_STR_TIMESTAMP_CAN        16
#define LENGTH_STR_DATA_CAN             32
#define LENGTH_STR_ID_CAN               16
#define LENGTH_STR_DESCRIPTION_CAN      256
#define LENGTH_STR_DLC_CAN              2
#define LENGTH_STR_CHANNEL_CAN          4
#define LENGTH_STR_DIRECTION_CAN        4
#define LENGTH_STR_TYPE_CAN             4


typedef struct tagFormattedData_CAN
{
    UINT64          m_u64TimeStamp;                 // Time stamp
    DWORD           m_dwMsgID;                      // Message identifier
    EDIRECTION      m_eDirection;                   // Direction (Rx / Tx)
    TYPE_CHANNEL    m_eChannel;                     // Channel
    BYTE            m_byDataLength;                 // Data length count
    BYTE            m_abData[8];                    // Message data
    BYTE            m_byIDType;                     // Type of the ID (STD or EXTENDED)
    BYTE            m_byMsgType;                    // Type of the message (RTR or NRTR)

    TCHAR	m_acMsgDir[LENGTH_STR_DIRECTION_CAN];     // "Tx" or "Rx"
    TCHAR	m_acChannel[LENGTH_STR_CHANNEL_CAN];      // "1" or "2"
    TCHAR	m_acType[LENGTH_STR_TYPE_CAN];            // s / x / r
    TCHAR	m_acDataLen[LENGTH_STR_DLC_CAN];          // DLC always in dec
    TCHAR	m_acMsgDesc[LENGTH_STR_DESCRIPTION_CAN];  // Message description

    TCHAR	m_acMsgIDHex[LENGTH_STR_ID_CAN];          // Identifier in hex
    TCHAR	m_acMsgIDDec[LENGTH_STR_ID_CAN];          // Identifier in dec

    TCHAR	m_acDataHex[LENGTH_STR_DATA_CAN];         // Data in hex
    TCHAR	m_acDataDec[LENGTH_STR_DATA_CAN];         // Data in dec

    TCHAR	m_acTimeAbsReset[LENGTH_STR_TIMESTAMP_CAN];    // Absolute time stamp Reset
    TCHAR	m_acTimeAbs[LENGTH_STR_TIMESTAMP_CAN];    // Absolute time stamp 
    TCHAR	m_acTimeRel[LENGTH_STR_TIMESTAMP_CAN];    // Relative time stamp
    TCHAR	m_acTimeSys[LENGTH_STR_TIMESTAMP_CAN];    // System time stamp

    __int64         m_n64MapId;                     // Map id of the message
    COLORREF        m_ColourCode;                   // Colour code associated

} SFORMATTEDDATA_CAN;

typedef struct sWMUpdatePtrPara
{
    int              m_nListIndex;
    EINTERPRET_MODE  m_eInPretMode;
    BOOL             m_bResult;
}SWMUPDATEPTRPARA, *PSWMUPDATEPTRPARA;

#define MAX_MSG_WND_COL_CNT              12

typedef struct sMsgWndHdrCol
{
    BYTE m_byTimePos;
    BYTE m_byCodeNamePos;
    BYTE m_byIDPos;
    BYTE m_byRxTxPos;
    BYTE m_byMsgTypePos;
    BYTE m_byDLCPos;
    BYTE m_byDataPos;
    BYTE m_byChannel;
	BYTE m_byPGNPos;
	BYTE m_bySrcPos;
	BYTE m_byDestPos;
	BYTE m_byPriorityPos;

    sMsgWndHdrCol()
    {
        m_byTimePos     = (BYTE) -1;
        m_byCodeNamePos = (BYTE) -1;
        m_byIDPos       = (BYTE) -1;
        m_byRxTxPos     = (BYTE) -1;
        m_byMsgTypePos  = (BYTE) -1;
        m_byDLCPos      = (BYTE) -1;
        m_byDataPos     = (BYTE) -1;
        m_byChannel     = (BYTE) -1;
		m_byPGNPos      = (BYTE) -1;
		m_bySrcPos      = (BYTE) -1;
		m_byDestPos     = (BYTE) -1;
		m_byPriorityPos = (BYTE) -1;		
    }
}SMSGWNDHDRCOL;

#endif // BASEDEFS_H__INCLUDED_
