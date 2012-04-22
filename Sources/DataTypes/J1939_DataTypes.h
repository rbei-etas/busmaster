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
 * \file      J1939_Datatypes.h
 * \brief     Definition file for filter data types.
 * \author    Pradeep Kadoor
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Definition file for filter data types.
 */

#pragma once

/* TYPES OF TIMEOUT */
const BYTE DEFAULT_PRIORITY     = 0x7;
const BYTE ADDRESS_NULL         = 254;
const BYTE ADDRESS_ALL          = 255;

/* Call back function pointer and type */
enum ETYPE_CLBCK_FN
{
    CLBCK_FN_LDATA_CONF, //Peer to peer long data transmission
    CLBCK_FN_LDATA_IND, //Peer to peer long data reception
    CLBCK_FN_BC_LDATA_CONF, //Broacast long data transmission
    CLBCK_FN_BC_LDATA_IND, //Broacast long data reception
    CLBCK_FN_NM_ACL
};

/* Call back function type */
typedef void (*PCLBCK_FN_LDATA_CONF)(DWORD dwClient, UINT32 unPGN, BYTE bySrc, 
                                BYTE byDest, BOOL bSuccess);
typedef void (*PCLBCK_FN_LDATA_IND)(DWORD dwClient, UINT32 unPGN, BYTE bySrc, 
                               BYTE byDest, BOOL bSuccess);
typedef void (*PCLBCK_FN_BC_LDATA_CONF)(DWORD dwClient, UINT32 unPGN, BYTE bySrc, 
                                   BYTE byDest, BOOL bSuccess);
typedef void (*PCLBCK_FN_BC_LDATA_IND)(DWORD dwClient, UINT32 unPGN, BYTE bySrc, 
                                  BYTE byDest, BOOL bSuccess);
typedef void (*PCLBCK_FN_NM_ACL)(DWORD dwClient, BYTE byAddress);

enum ETYPE_TIMEOUT
{
    TYPE_TO_BROADCAST = 0,
    TYPE_TO_RESPONSE,
    TYPE_TO_HOLDING,
    TYPE_TO_T1,
    TYPE_TO_T2,
    TYPE_TO_T3,
    TYPE_TO_T4
};

/* TYPES OF ACKNOWLEDGEMENT */
enum ETYPE_ACK
{
    ACK_POS = 0,
    ACK_NEG
};

/* Node parmaeters in j1939 network*/
typedef struct tagSJ1939CLIENTPARAM
{
    TCHAR   m_acName[MAX_PATH];
    DWORD   m_dwClientId;
    UINT64  m_unEcuName;
    BYTE    m_byAddress;
}SJ1939CLIENTPARAM;

/* ECU NAME STRUCTURE */
typedef struct tagSECU_NAME
{
    UINT32  m_unID           : 21;
    UINT32  m_unSAE_MF_CODE  : 11;
    BYTE    m_byECU_INST     : 3;
    BYTE    m_byFUNC_INST    : 5;
    BYTE    m_bySAE_FUNCTION : 8;
    BYTE    m_bySAE_RESERVED : 1;
    BYTE    m_bySAE_VHL_SYS  : 7;
    BYTE    m_byVHL_SYS_INST : 4;
    BYTE    m_bySAE_IND_GRP  : 3;
    BYTE    m_byARB_ADRS_CPL : 1;

} STRUCT_ECU_NAME;

typedef union tagECU_NAME
{
    UINT64          m_un64ECU_NAME;
    STRUCT_ECU_NAME m_sECU_NAME;

} UNION_ECU_NAME;

/* J1939 PGN structure */
typedef struct tagSTRUCT_PGN
{   
    BYTE m_byPDU_Specific   : 8; /* PDU Specific (PS), Destination address if 0 <= PF <= 239.
                                                       Group Extension (GE)if 240 <= PF <= 255. */
    BYTE m_byPDU_Format     : 8; // PDU Format (PF), Indicates Peer_2_peer or Broadcast.
    BYTE m_byDataPage       : 1; // DataPage (DP), Set to 0 currently.
    BYTE m_byReserved       : 1; // Reserved, Set to 0.
    BYTE m_byPriority       : 3;

} STRUCT_PGN;

typedef union tagUPGN
{
    UINT32      m_unPGN : 21;
    STRUCT_PGN  m_sPGN;

} UNION_PGN;

/* J1939 Extended 29 bit ID */
typedef struct tagSTRUCT_29_BIT_ID
{
    BYTE        m_bySrcAddress     : 8; // Sender node address.
    UNION_PGN   m_uPGN;                 // Parameter group number(PGN). SAE lists various PGNS and its SPNS.    
    //BYTE        m_byPriority       : 3; // Priority (0 - 7). Lower the value higher the priority
public:
    UINT32 unGetPGN(void);
    void vSetPGN(UINT32 unPGN);
} STRUCT_29_BIT_ID;

typedef union tag29BitID
{
    UINT32              m_unExtID : 29;
    STRUCT_29_BIT_ID    m_s29BitId;

} UNION_29_BIT_ID;

/* J1939 message type */
typedef enum EJ1939_MSG_TYPE
{
    MSG_TYPE_NONE = 0x0,
    MSG_TYPE_COMMAND,
    MSG_TYPE_REQUEST,
    MSG_TYPE_DATA,
    MSG_TYPE_BROADCAST,
    MSG_TYPE_ACKNOWLEDGEMENT,
    MSG_TYPE_GROUP_FUNCTIONS,

    MSG_TYPE_NM_ACL,
    MSG_TYPE_NM_RQST_ACL,
    MSG_TYPE_NM_CMD_ADDRESS,
    MSG_TYPE_NM_TPCM_BAM, 
    MSG_TYPE_NM_TPCM_RTS,
    MSG_TYPE_NM_TPCM_CTS,
    MSG_TYPE_NM_TPCM_EOM_ACK,
    MSG_TYPE_NM_TPCM_CON_ABORT,
    MSG_TYPE_NM_TPDT
};

/* J1939 message properties */
typedef struct tagSTJ1939_MSG_PROPERTIES
{
    UINT64          m_un64TimeStamp; // Timestamp.
    BYTE            m_byChannel;     // Channel number.
    EJ1939_MSG_TYPE m_eType;         
    EDIRECTION      m_eDirection;    // DIR_TX, DIR_RX
    UNION_29_BIT_ID m_uExtendedID;   // 29 bit extended ID

} STJ1939_MSG_PROPERTIES;

/* J1939 message structure */
typedef struct tagSTJ1939_MSG
{
//public:
/* Public member variables */

	static int  m_nSortField;
    static int  m_nMFactor;     // Multiplication factor
    STJ1939_MSG_PROPERTIES  m_sMsgProperties;   /* J1939 message properties. (29 bit Extended ID, Timestamp, 
                                                   Channel, Type, Direction) */
    UINT                    m_unDLC;            // Data length. 0 <= m_unDLC <= 1785 bytes.
    BYTE*                   m_pbyData;          // J1939 message data.

/* Public member functions */

    /* constructor */
    tagSTJ1939_MSG();
    /* Destructor */
    ~tagSTJ1939_MSG();
    /* Returns the total size of the message. */
    UINT unGetSize(void) const;
    /* Renders the BYTE stream of the total message.
       Data has to be copied before the scope of the object ends. */
    void vGetDataStream(BYTE* pbyData) const;
    void vSetDataStream(BYTE* m_pbyData);
    void vClear();
	void vInitialize(int nSize);
    static void vSetSortField(int nField);
	static void vSetSortAscending(bool bAscending);
	static int DoCompareIndiv(const void* pEntry1, const void* pEntry2);
	static __int64 GetSlotID(tagSTJ1939_MSG& pDatCAN);	
	tagSTJ1939_MSG& operator=(const tagSTJ1939_MSG& RefObj);

} STJ1939_MSG, *PSTJ1939_MSG;

#define LEN_STR_TYPE_J1939      32
#define LEN_STR_TIMESTAMP_J1939 16
#define LEN_STR_CHANNEL_J1939   4
#define LEN_STR_PGN_J1939       16
#define LEN_STR_NAME_J1939      64
#define LEN_STR_SENDNODE_J1939  32
#define LEN_STR_SRC_J1939       4
#define LEN_STR_DEST_J1939      4
#define LEN_STR_PRIO_J1939      4
#define LEN_STR_DIR_J1939       3
#define LEN_STR_DLC_J1939       2


typedef struct tagFormattedData_J1939
{
    DWORD  m_dwPGN;                                 // PGN of the present message
    DWORD  m_dwMsgID;                               // The message ID

    TCHAR  m_acTimeSys[LEN_STR_TIMESTAMP_J1939];    // System time stamp
    TCHAR  m_acTimeRel[LEN_STR_TIMESTAMP_J1939];    // Relative time stamp
    TCHAR  m_acTimeAbs[LEN_STR_TIMESTAMP_J1939];    // Absolute time stamp 
    TCHAR  m_acTimeAbsReset[LEN_STR_TIMESTAMP_J1939];    // Absolute time stamp Reset

    TCHAR  m_acMsgType[LEN_STR_TYPE_J1939];         // Type indicator
    TCHAR  m_acChannel[LEN_STR_CHANNEL_J1939];      // Channel/controller indicator

    TCHAR  m_acPGNHex[LEN_STR_PGN_J1939];           // PGN in hex
    TCHAR  m_acPGNDec[LEN_STR_PGN_J1939];           // PGN in dec
    TCHAR  m_acMsgName[LEN_STR_NAME_J1939];         // Message name
    TCHAR  m_acSenderName[LEN_STR_SENDNODE_J1939];  // Send node name

	TCHAR	m_acMsgIDHex[LENGTH_STR_ID_CAN];          // Identifier in hex
    TCHAR	m_acMsgIDDec[LENGTH_STR_ID_CAN];          // Identifier in dec
    TCHAR  m_acSrcHex[LEN_STR_SRC_J1939];           // Source node address in hex.
    TCHAR  m_acSrcDec[LEN_STR_SRC_J1939];           // Source node address in dec.
    TCHAR  m_acDestHex[LEN_STR_DEST_J1939];         // Destination node address in hex
    TCHAR  m_acDestDec[LEN_STR_DEST_J1939];         // Destination node address in dec
                                                    // Other values are - "all", "--"

    TCHAR  m_acPriority[LEN_STR_PRIO_J1939];        // Priority (always in decimal)
    TCHAR  m_acMsgDir[LEN_STR_DIR_J1939];           // "Tx" or "Rx"
    TCHAR  m_acDataLen[LEN_STR_DLC_J1939];          // DLC always in dec
    TCHAR* m_pcDataHex;                             // Data in hexadecimal
    TCHAR* m_pcDataDec;                             // Data in decimal

    tagFormattedData_J1939();                       // Constructor
    ~tagFormattedData_J1939();                      // Destructor;

} SFORMATTEDATA_J1939, *PSFORMATTEDATA_J1939;

// Helper functions.

// Given the type code, this function will render the associated string.
void GetMessageTypeStr(EJ1939_MSG_TYPE eType, TCHAR acResult[LEN_STR_TYPE_J1939]);

const int MAX_MSG_LEN_J1939 = sizeof (STJ1939_MSG_PROPERTIES) + sizeof (UINT) + MAX_DATA_LEN_J1939;
