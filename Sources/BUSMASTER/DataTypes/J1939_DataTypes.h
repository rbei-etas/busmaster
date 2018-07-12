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

#include "Utility\MultiLanguageSupport.h"
#include <locale.h>
#include "J1939DriverDefines.h"



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


typedef struct tagFormattedData_J1939
{
    DWORD  m_dwPGN;                                 // PGN of the present message
    DWORD  m_dwMsgID;                               // The message ID

    char  m_acTimeSys[LEN_STR_TIMESTAMP_J1939];    // System time stamp
    char  m_acTimeRel[LEN_STR_TIMESTAMP_J1939];    // Relative time stamp
    char  m_acTimeAbs[LEN_STR_TIMESTAMP_J1939];    // Absolute time stamp
    char  m_acTimeAbsReset[LEN_STR_TIMESTAMP_J1939];    // Absolute time stamp Reset

    char  m_acMsgType[LEN_STR_TYPE_J1939];         // Type indicator
    char  m_acChannel[LEN_STR_CHANNEL_J1939];      // Channel/controller indicator

    char  m_acPGNHex[LEN_STR_PGN_J1939];           // PGN in hex
    char  m_acPGNDec[LEN_STR_PGN_J1939];           // PGN in dec
    char  m_acMsgName[LEN_STR_NAME_J1939];         // Message name
    char  m_acSenderName[LEN_STR_SENDNODE_J1939];  // Send node name

    char    m_acMsgIDHex[LENGTH_STR_ID_CAN];          // Identifier in hex
    char    m_acMsgIDDec[LENGTH_STR_ID_CAN];          // Identifier in dec
    char  m_acSrcHex[LEN_STR_SRC_J1939];           // Source node address in hex.
    char  m_acSrcDec[LEN_STR_SRC_J1939];           // Source node address in dec.
    char  m_acDestHex[LEN_STR_DEST_J1939];         // Destination node address in hex
    char  m_acDestDec[LEN_STR_DEST_J1939];         // Destination node address in dec
    // Other values are - "all", "--"

    char  m_acPriority[LEN_STR_PRIO_J1939];        // Priority (always in decimal)
    char  m_acMsgDir[LEN_STR_DIR_J1939];           // "Tx" or "Rx"
    char  m_acDataLen[LEN_STR_DLC_J1939];          // DLC always in dec
    char* m_pcDataHex;                             // Data in hexadecimal
    char* m_pcDataDec;                             // Data in decimal

    tagFormattedData_J1939();                       // Constructor
    ~tagFormattedData_J1939();                      // Destructor;

} SFORMATTEDATA_J1939, *PSFORMATTEDATA_J1939;

// Helper functions.

// Given the type code, this function will render the associated string.
void GetMessageTypeStr(EJ1939_MSG_TYPE eType, char acResult[LEN_STR_TYPE_J1939]);

//const int MAX_MSG_LEN_J1939 = sizeof (STJ1939_MSG_PROPERTIES) + sizeof (UINT) + MAX_DATA_LEN_J1939;
