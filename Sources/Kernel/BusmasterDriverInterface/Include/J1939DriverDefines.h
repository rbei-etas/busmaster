/******************************************************************************
  Project       :  Auto-SAT_Tools
  FileName      :  J1939_Datatypes.h
  Description   :
  $Log:   X:/Archive/Sources/DataTypes/J1939_DataTypes.h_v  $
 *
 *    Rev 1.11   Jun 07 2011 16:16:54   rac2kor
 * Removed compiler warnings
 *
 *    Rev 1.10   15 Apr 2011 18:55:06   rac2kor
 * Inserted RBEI Copyright information text into the file header.
 *
 *    Rev 1.9   04 Mar 2011 17:30:38   CANMNTTM
 * Updated to solve the issues in J1939 MsgWnd:
 * 1. Sorting issues in J1939 Msg Wnd.
 * 2. Problem with J1939 messages with DLC > 150.
 *
 *    Rev 1.8   02 Mar 2011 11:47:08   CANMNTTM
 * Support to multiple bus
 *
 *    Rev 1.7   13 Jan 2011 14:49:12   CANMNTTM
 * MAX_LEN for respective messages moved to
 * Include/Basedefs.h
 *
 *    Rev 1.6   23 Dec 2010 16:46:06   CANMNTTM
 * MAX_MSG_LEN_J1939 macro is added.
 *
 *    Rev 1.5   22 Dec 2010 19:18:00   CANMNTTM
 * Call back function prototype has been added.
 *
 *    Rev 1.4   14 Dec 2010 17:52:16   CANMNTTM
 * Added a member function vSetPGN for STRUCT_29_BIT_ID.
 *
 *
 *    Rev 1.3   13 Dec 2010 22:00:22   CANMNTTM
 * Required number of characters for storing "Direction" in log file is reduced to 3 from 4.
 *
 *    Rev 1.2   13 Dec 2010 21:21:48   rac2kor
 * 1. Added a data member for message type in SFORMATTEDATA_J1939
 * 2. Added a helper function to get the associated text for a type.
 *
 *    Rev 1.1   13 Dec 2010 16:43:36   CANMNTTM
 * Structure J1939 client params introduced.
 *
 *    Rev 1.0   06 Dec 2010 18:34:34   rac2kor
 *

  Author(s)     :  Pradeep Kadoor
  Date Created  :  23/11/2010
  Modified By   :
  Copyright (c) 2011, Robert Bosch Engineering and Business Solutions.  All rights reserved.
******************************************************************************/

#pragma once

#include <windows.h>
#include "CANDriverDefines.h"
#include <locale.h>

#define J1939_MONITOR_NODE      "J1939_MONITOR"
const UINT64 J1939_ECU_NAME = 0x8000000000000001;

/* TYPES OF TIMEOUT */
#define DEFAULT_PRIORITY        0x7
#define ADDRESS_NULL            254
#define ADDRESS_ALL             255


#define MAX_DATA_LEN_J1939      1785
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
#define LEN_STR_DLC_J1939       5
#define LENGTH_STR_ID_CAN       16

#define COPY_DATA_INC_DEST(TgtStream, SrcStream, TotBytes) { memcpy(TgtStream, SrcStream, TotBytes); TgtStream += TotBytes; }
#define COPY_DATA_INC_SOURCE(TgtStream, SrcStream, TotBytes) { memcpy(TgtStream, SrcStream, TotBytes); SrcStream += TotBytes; }



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
    char   m_acName[MAX_PATH];
    DWORD   m_dwClientId;
    UINT64  m_unEcuName;
    BYTE    m_byAddress;
} SJ1939CLIENTPARAM;

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
    UINT32 unGetPGN( void )
    {
        UINT32 unPGN = m_uPGN.m_unPGN & 0x3FFFF;
        if ( m_uPGN.m_sPGN.m_byPDU_Format < 240 )
        {
            unPGN = m_uPGN.m_unPGN & 0x3FF00;
        }
        return unPGN;
    }

    void vSetPGN( UINT32 unPGN )
    {
        UNION_PGN uPGN;
        uPGN.m_unPGN = unPGN & 0x3FFFF; //Mask unecessary bits.
        m_uPGN.m_unPGN = m_uPGN.m_unPGN & 0xFFFC0000;   //Mask PGN bits

        BYTE byPduFormat = (BYTE)( unPGN >> 8 );
        if ( byPduFormat < 240 )
        {
            m_uPGN.m_unPGN |= ( unPGN & 0x3FF00 );
        }
        else
        {
            m_uPGN.m_unPGN |= unPGN;
        }
    }
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

    tagSTJ1939_MSG()
    {
        memset( &m_sMsgProperties, 0, sizeof( STJ1939_MSG_PROPERTIES ) );
        m_unDLC = 0;
        m_pbyData = nullptr;
    }

    /* Destructor */
    ~tagSTJ1939_MSG()
    {
        memset( &m_sMsgProperties, 0, sizeof( STJ1939_MSG_PROPERTIES ) );
        m_unDLC = 0;
        if ( nullptr != m_pbyData )
        {
            delete[] m_pbyData;
            m_pbyData = nullptr;
        }
    }
    UINT unGetSize( void ) const
    {
        return ( sizeof( STJ1939_MSG_PROPERTIES ) + sizeof( UINT ) + ( sizeof( BYTE ) * m_unDLC ) );
    }
    /* Renders the BYTE stream of the total message. */
    void vGetDataStream( BYTE* pbyData ) const
    {
        COPY_DATA_INC_DEST( pbyData, &m_sMsgProperties, sizeof( STJ1939_MSG_PROPERTIES ) );
        COPY_DATA_INC_DEST( pbyData, &m_unDLC, sizeof( UINT ) );
        COPY_DATA_INC_DEST( pbyData, m_pbyData, ( sizeof( BYTE ) * m_unDLC ) );
    }

    void vSetDataStream( BYTE* pbyData )
    {
        COPY_DATA_INC_SOURCE( &m_sMsgProperties, pbyData, sizeof( STJ1939_MSG_PROPERTIES ) );
        UINT unTempDLC = 0;
        COPY_DATA_INC_SOURCE( &unTempDLC, pbyData, sizeof( UINT ) );
        if (/*unTempDLC > m_unDLC*/ m_pbyData == nullptr )
        {
            //DELETE_ARRAY(m_pbyData);
            m_pbyData = new BYTE[MAX_DATA_LEN_J1939];
        }
        m_unDLC = unTempDLC;
        COPY_DATA_INC_SOURCE( m_pbyData, pbyData, ( sizeof( BYTE ) * m_unDLC ) );
    }
    void vInitialize( int nSize )
    {
        if ( m_unDLC < (UINT)nSize )
        {
            if ( nullptr != m_pbyData )
            {
                delete[] m_pbyData;
            }
            m_pbyData = new BYTE[nSize];
        }
        m_unDLC = nSize;
    }

    tagSTJ1939_MSG& operator=( const tagSTJ1939_MSG& RefObj )
    {
        m_sMsgProperties = RefObj.m_sMsgProperties;
        if ( m_unDLC < RefObj.m_unDLC )
        {
            delete[] m_pbyData;
            m_pbyData = new BYTE[RefObj.m_unDLC];
        }
        memcpy( m_pbyData, RefObj.m_pbyData, RefObj.m_unDLC );
        m_unDLC = RefObj.m_unDLC;
        return *this;
    }

    void vClear( void )
    {
        m_sMsgProperties.m_eType = MSG_TYPE_NONE;
        m_unDLC = 0;
        if ( nullptr != m_pbyData )
        {
            delete[] m_pbyData;
            m_pbyData = nullptr;
        }
    }

} STJ1939_MSG, *PSTJ1939_MSG;


const int MAX_MSG_LEN_J1939 = sizeof( STJ1939_MSG_PROPERTIES ) + sizeof( UINT ) + MAX_DATA_LEN_J1939;