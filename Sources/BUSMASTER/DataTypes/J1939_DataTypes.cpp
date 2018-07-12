/******************************************************************************
  Project       :  Auto-SAT_Tools
  FileName      :  DataTypes_J1939.cpp
  Description   :
  $Log:   X:/Archive/Sources/DataTypes/J1939_DataTypes.cpv  $

      Rev 1.5   15 Apr 2011 18:55:06   rac2kor
   Inserted RBEI Copyright information text into the file header.

      Rev 1.4   04 Mar 2011 17:30:38   CANMNTTM
   Updated to solve the issues in J1939 MsgWnd:
   1. Sorting issues in J1939 Msg Wnd.
   2. Problem with J1939 messages with DLC > 150.

      Rev 1.3   18 Feb 2011 12:06:46   CANMNTTM
   Updated to suite multiple message windows for buses.

      Rev 1.2   14 Dec 2010 17:52:16   CANMNTTM
   Added a member function vSetPGN for STRUCT_29_BIT_ID.


      Rev 1.1   13 Dec 2010 21:21:46   rac2kor
   1. Added a data member for message type in SFORMATTEDATA_J1939
   2. Added a helper function to get the associated text for a type.

      Rev 1.0   06 Dec 2010 18:34:32   rac2kor


  Author(s)     :  Pradeep Kadoor
  Date Created  :  23/11/2010
  Modified By   :
  Copyright (c) 2011, Robert Bosch Engineering and Business Solutions.  All rights reserved.
******************************************************************************/
#include "Datatypes_stdafx.h"
#include "Include/BaseDefs.h"
#include "Include/Utils_Macro.h"
#include "J1939_DataTypes.h"
#include "application/hashdefines.h"
#include "application/Common.h"



/* Starts tagFormattedData_J1939 related codes */
tagFormattedData_J1939::tagFormattedData_J1939()
{
    _tcsnset(m_acTimeSys,   L'\0', LEN_STR_TIMESTAMP_J1939);
    _tcsnset(m_acTimeRel,   L'\0', LEN_STR_TIMESTAMP_J1939);
    _tcsnset(m_acTimeAbs,   L'\0', LEN_STR_TIMESTAMP_J1939);

    _tcsnset(m_acMsgType,   L'\0', LEN_STR_TYPE_J1939     );
    _tcsnset(m_acChannel,   L'\0', LEN_STR_CHANNEL_J1939  );

    _tcsnset(m_acPGNHex,    L'\0', LEN_STR_PGN_J1939      );
    _tcsnset(m_acPGNDec,    L'\0', LEN_STR_PGN_J1939      );
    _tcsnset(m_acMsgName,   L'\0', LEN_STR_NAME_J1939     );
    _tcsnset(m_acSenderName,L'\0', LEN_STR_SENDNODE_J1939 );

    _tcsnset(m_acSrcHex,    L'\0', LEN_STR_SRC_J1939      );
    _tcsnset(m_acSrcDec,    L'\0', LEN_STR_SRC_J1939      );
    _tcsnset(m_acDestHex,   L'\0', LEN_STR_DEST_J1939     );
    _tcsnset(m_acDestDec,   L'\0', LEN_STR_DEST_J1939     );


    _tcsnset(m_acPriority,  L'\0', LEN_STR_PRIO_J1939     );
    _tcsnset(m_acMsgDir,    L'\0', LEN_STR_DIR_J1939      );
    _tcsnset(m_acDataLen,   L'\0', LEN_STR_DLC_J1939      );

    m_acMsgDir[LEN_STR_DIR_J1939 - 2] = L'x'; // It will be either Tx or Rx

    m_pcDataHex = nullptr;
    m_pcDataDec = nullptr;
}

tagFormattedData_J1939::~tagFormattedData_J1939()
{
    DELETE_ARRAY(m_pcDataHex);
    DELETE_ARRAY(m_pcDataDec);
}
/* Ends tagFormattedData_J1939 related codes */


// Helper functions start
void GetMessageTypeStr(EJ1939_MSG_TYPE eType, char acResult[LEN_STR_TYPE_J1939])
{
    switch (eType)
    {
        case MSG_TYPE_NONE:
            _tcscpy(acResult, "NONE");
            break;
        case MSG_TYPE_COMMAND:
            _tcscpy(acResult, "CMD");
            break;
        case MSG_TYPE_REQUEST:
            _tcscpy(acResult, "RQST");
            break;
        case MSG_TYPE_DATA:
            _tcscpy(acResult, "DATA");
            break;
        case MSG_TYPE_BROADCAST:
            _tcscpy(acResult, "BROADCAST");
            break;
        case MSG_TYPE_ACKNOWLEDGEMENT:
            _tcscpy(acResult, "ACK");
            break;
        case MSG_TYPE_GROUP_FUNCTIONS:
            _tcscpy(acResult, "GRP_FUNC");
            break;

        case MSG_TYPE_NM_ACL:
            _tcscpy(acResult, "ACL");
            break;
        case MSG_TYPE_NM_RQST_ACL:
            _tcscpy(acResult, "RQST_ACL");
            break;
        case MSG_TYPE_NM_CMD_ADDRESS:
            _tcscpy(acResult, "CA");
            break;
        case MSG_TYPE_NM_TPCM_BAM:
            _tcscpy(acResult, "BAM");
            break;
        case MSG_TYPE_NM_TPCM_RTS:
            _tcscpy(acResult, "RTS");
            break;
        case MSG_TYPE_NM_TPCM_CTS:
            _tcscpy(acResult, "CTS");
            break;
        case MSG_TYPE_NM_TPCM_EOM_ACK:
            _tcscpy(acResult, "EOM");
            break;
        case MSG_TYPE_NM_TPCM_CON_ABORT:
            _tcscpy(acResult, "CON_ABORT");
            break;
        case MSG_TYPE_NM_TPDT:
            _tcscpy(acResult, "TPDT");
            break;
        default:
            ASSERT(false);
    }
}

// Helper functions end