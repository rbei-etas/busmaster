/********************************************************************************
  Project       :  Auto-SAT_Tools                                      
  FileName      :  Struct_J1939.h                                         
  Description   :  This file contain the definition of structure used in message 
                   handler
  $Log: $

  Author(s)     :  Amitesh Bharti                                         
  Date Created  :  25.03.2003                                             
  Modified By   :  
********************************************************************************/
#ifndef _STRUCT_J1939_H_
#define _STRUCT_J1939_H_
#pragma pack(push, 8)
//#pragma pack(show)



typedef enum ETYPE_TIMEOUT
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
typedef enum ETYPE_ACK
{
    ACK_POS = 0,
    ACK_NEG
};

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

typedef enum EDIRECTION
{
    DIR_RX = 'R',
    DIR_TX = 'T',
    DIR_ALL
};

/* J1939 message properties */
typedef struct tagSTJ1939_MSG_PROPERTIES
{
    UINT64                  m_un64TimeStamp; // Timestamp.
    BYTE                    m_byChannel;     // Channel number.
    enum EJ1939_MSG_TYPE    m_eType;         
    enum EDIRECTION         m_eDirection;    // DIR_TX, DIR_RX
    UNION_29_BIT_ID         m_uExtendedID;   // 29 bit extended ID

} STJ1939_MSG_PROPERTIES;

typedef struct tagSTJ1939_MSG
{
    STJ1939_MSG_PROPERTIES  m_sMsgProperties;   /* J1939 message properties. (29 bit Extended ID, Timestamp, 
                                                   Channel, Type, Direction) */
    UINT                    m_unDLC;            // Data length. 0 <= m_unDLC <= 1785 bytes.
    BYTE*                   m_pbyData;          // J1939 message data.
} STJ1939_MSG;


#pragma pack(pop, 8)
#endif //_STRUCT_J1939_H_