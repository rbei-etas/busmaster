/*******************************************************************************
  Project       :  Auto-SAT_Tools
  FileName      :  Wrapper_J1939.h
  Description   :

  $Log:   Y:/EMC2/Projects/CAN_Monitor_Maintenance/Archives/Source/Misc/GCC/Wrapper_J1939.h_v  $
 *
 *    Rev 1.0   02 Mar 2011 17:37:12   rac2kor
 *

  Author(s)     :  Amarnath Shastry
  Date Created  :  20/03/2002
  Modified By   :  Ravikumar Patil  on 04.03.03
*******************************************************************************/
#ifndef _WRAPPER_J1939_H_
#define _WRAPPER_J1939_H_

#include "Common.h"

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
enum EJ1939_MSG_TYPE
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

enum EDIRECTION
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


typedef UINT    (*APPSENDPROC_J) (STJ1939_MSG*, HMODULE);
typedef UINT    (*APPREQUESTPGN_J) (UINT, BYTE, UINT, HMODULE);
typedef UINT    (*APPACKMSG_J) (UINT, BYTE, BYTE, UINT, HMODULE);
typedef UINT    (*APPCLAIMADDRESS_J) (BYTE, UINT, HMODULE);
typedef UINT    (*APPRQSTADDRESS_J) (UINT, HMODULE);
typedef UINT    (*APPCMDADDRESS_J) (UINT64, BYTE, UINT, HMODULE);
typedef UINT    (*APPSETTIMEOUT_J) (BYTE, UINT, HMODULE);
typedef UINT    (*APPONLINEOFFLINE_J) (BOOL, HMODULE);
typedef UINT    (*LOGENABLE) (BOOL);
typedef UINT    (*WRITETOLOGFILE) (char*);
typedef UINT    (*WRITETOTRACE) (char*);
typedef UINT    (*GOONLINEOFFLINE) (BOOL, HMODULE) ;
typedef UINT    (*STARTSTOPTIMER) (char*,UINT,BOOL,HMODULE) ;
typedef UINT    (*SETTIMERVAL) (char*,UINT,HMODULE);
typedef BOOL    (*ENABLEDISABLEALLHANDLERS)(BOOL, HMODULE);
typedef BOOL    (*ENABLEDISABLEMSGHANDLERS)(BOOL, HMODULE);
typedef BOOL    (*ENABLEDISABLEKEYHANDLERS)(BOOL, HMODULE);
typedef BOOL    (*ENABLEDISABLEERRORHANDLERS)(BOOL, HMODULE);
typedef BOOL    (*MSGTXONOFF)(BOOL, HMODULE);
typedef HMODULE (*GETNODEHANDLER)(char*);

// Please insert any new wrapper prototype here
// Prototypes Start

GCC_EXTERN UINT GCC_EXPORT EnableLogging() ;
GCC_EXTERN UINT GCC_EXPORT DisableLogging() ;
GCC_EXTERN UINT GCC_EXPORT WriteToLogFile(char*) ;
GCC_EXTERN UINT GCC_EXPORT Trace(char*,...) ;
GCC_EXTERN UINT GCC_EXPORT GoOnline() ;
GCC_EXTERN UINT GCC_EXPORT GoOffline() ;
GCC_EXTERN UINT GCC_EXPORT StartTimer(char*,UINT) ;
GCC_EXTERN UINT GCC_EXPORT StopTimer(char*) ;
GCC_EXTERN BOOL GCC_EXPORT SetTimerVal(char*,UINT);
GCC_EXTERN BOOL GCC_EXPORT EnableAllHandlers(BOOL);
GCC_EXTERN BOOL GCC_EXPORT EnableMsgHandlers(BOOL);
GCC_EXTERN BOOL GCC_EXPORT EnableErrorHandlers(BOOL);
GCC_EXTERN BOOL GCC_EXPORT EnableKeyHandlers(BOOL);
GCC_EXTERN BOOL GCC_EXPORT bGetProgramVersion(int* pnMajorVersion, int* pnMinorVersion,HMODULE hModuleHandle );
GCC_EXTERN BOOL GCC_EXPORT EnableDisableMsgTx(BOOL);
GCC_EXTERN HMODULE GCC_EXPORT GetDllHandle(char*);


GCC_EXTERN void GCC_EXPORT vSetSendMsgProcAddress(APPSENDPROC_J pAppFunc);
GCC_EXTERN void GCC_EXPORT vSetEnableLoggingProcAddress(LOGENABLE pEnableLoggingFunc);
GCC_EXTERN void GCC_EXPORT vSetDisableLoggingProcAddress(LOGENABLE pDisableLoggingFunc);
GCC_EXTERN void GCC_EXPORT vSetWriteToLogFileProcAddress(WRITETOLOGFILE pLogFileFunc);
GCC_EXTERN void GCC_EXPORT vSetTraceProcAddress(WRITETOTRACE pTraceFunc);
GCC_EXTERN void GCC_EXPORT vSetGoOfflineProcAddress(GOONLINEOFFLINE pGoOffline);
GCC_EXTERN void GCC_EXPORT vSetGoOnlineProcAddress(GOONLINEOFFLINE pGoOnline);
GCC_EXTERN void GCC_EXPORT vSetStopTimerProcAddress(STARTSTOPTIMER pStopTimer);
GCC_EXTERN void GCC_EXPORT vSetStartTimerProcAddress(STARTSTOPTIMER pStartTimer);
GCC_EXTERN void GCC_EXPORT vSetSetTimerValProcAddress(SETTIMERVAL pSetTimerVal);
GCC_EXTERN void GCC_EXPORT vSetEnableMsgHandlersProcAddress(ENABLEDISABLEMSGHANDLERS pMsgHandlers);
GCC_EXTERN void GCC_EXPORT vSetEnableKeyHandlersProcAddress(ENABLEDISABLEKEYHANDLERS pKeyHandlers);
GCC_EXTERN void GCC_EXPORT vSetEnableDisableMsgTxProcAddress(MSGTXONOFF pMsgTxFunc);
GCC_EXTERN void GCC_EXPORT vSetGetDllHandleProcAddress(GETNODEHANDLER pGetNodeHandler);
//J1939 specific functions
GCC_EXTERN void GCC_EXPORT vSetEnableAllHandlersProcAddress(ENABLEDISABLEALLHANDLERS pAllHandlers);
GCC_EXTERN void GCC_EXPORT vSetClaimAddressProcAddress(APPCLAIMADDRESS_J pClaimAddress);
GCC_EXTERN void GCC_EXPORT vSetCommandAddressProcAddress(APPCMDADDRESS_J pCommandAddress);
GCC_EXTERN void GCC_EXPORT vSetEnableErrorHandlersProcAddress(ENABLEDISABLEERRORHANDLERS pErrorHandlers);
GCC_EXTERN void GCC_EXPORT vSetRequestPGNProcAddress(APPREQUESTPGN_J pRequestPGN);
GCC_EXTERN void GCC_EXPORT vSetRequestAddressProcAddress(APPRQSTADDRESS_J pRequestAddress);
GCC_EXTERN void GCC_EXPORT vSetSendAckMsgProcAddress(APPACKMSG_J pSendAckMsg);
GCC_EXTERN void GCC_EXPORT vSetSetTimeoutProcAddress(APPSETTIMEOUT_J pSetTimeout);
GCC_EXTERN UINT GCC_EXPORT SendMsg(STJ1939_MSG*);
GCC_EXTERN UINT GCC_EXPORT RequestPGN(UINT, BYTE, UINT);
GCC_EXTERN UINT GCC_EXPORT SendAckMsg(UINT, BYTE, BYTE, UINT);
GCC_EXTERN UINT GCC_EXPORT ClaimAddress(BYTE, UINT);
GCC_EXTERN UINT GCC_EXPORT RequestAddress(UINT);
GCC_EXTERN UINT GCC_EXPORT CommandAddress(UINT64, BYTE, UINT);
GCC_EXTERN UINT GCC_EXPORT SetTimout(BYTE, UINT);

// Prototypes End
typedef struct _InternalStructJ1939
{
    _InternalStructJ1939()
    {
        Trace("temporary Structure");
    }
} InternalStructJ1939;
_InternalStructJ1939 obj1939;

#endif // _WRAPPER_J1939_H_