/******************************************************************************
  Project       :  Auto-SAT_Tools
  FileName      :  DataTypes_J1939.cpp
  Description   :
  $Log:   X:/Archive/Sources/DataTypes/J1939_DataTypes.cpv  $

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

int STJ1939_MSG::m_nSortField = 0;
int STJ1939_MSG::m_nMFactor = 1;

/* strcut STRUCT_29_BIT_ID*/
UINT32 tagSTRUCT_29_BIT_ID::unGetPGN(void)
{
    UINT32 unPGN = m_uPGN.m_unPGN & 0x3FFFF;

    if (m_uPGN.m_sPGN.m_byPDU_Format < 240)
    {
        unPGN = m_uPGN.m_unPGN & 0x3FF00;
    }

    return unPGN;
}

void tagSTRUCT_29_BIT_ID::vSetPGN(UINT32 unPGN)
{
    UNION_PGN uPGN;
    uPGN.m_unPGN = unPGN & 0x3FFFF; //Mask unecessary bits.
    m_uPGN.m_unPGN = m_uPGN.m_unPGN & 0xFFFC00FF;   //Mask PGN bits
    BYTE byPduFormat = (BYTE)(unPGN >> 8);

    if  (byPduFormat < 240)
    {
        m_uPGN.m_unPGN |= (unPGN & 0x3FF00);
    }
    else
    {
        m_uPGN.m_unPGN |= unPGN;
    }
}

/* Constructor*/
tagSTJ1939_MSG::tagSTJ1939_MSG()
{
    memset(&m_sMsgProperties, 0, sizeof(STJ1939_MSG_PROPERTIES));
    m_unDLC = 0;
    m_pbyData = NULL;
}

/* Destructor */
tagSTJ1939_MSG::~tagSTJ1939_MSG()
{
    memset(&m_sMsgProperties, 0, sizeof(STJ1939_MSG_PROPERTIES));
    m_unDLC = 0;
    DELETE_ARRAY(m_pbyData);
}

/* returns the total size of the message.*/
UINT tagSTJ1939_MSG::unGetSize(void) const
{
    return (sizeof(STJ1939_MSG_PROPERTIES) + sizeof (UINT) + (sizeof (BYTE) * m_unDLC) );
}
/* Renders the BYTE stream of the total message. */
void tagSTJ1939_MSG::vGetDataStream(BYTE* pbyData) const
{
    COPY_DATA(pbyData, &m_sMsgProperties, sizeof(STJ1939_MSG_PROPERTIES));
    COPY_DATA(pbyData, &m_unDLC, sizeof (UINT));
    COPY_DATA(pbyData, m_pbyData, (sizeof (BYTE) * m_unDLC));
}

void tagSTJ1939_MSG::vSetDataStream(BYTE* pbyData)
{
    COPY_DATA_2(&m_sMsgProperties, pbyData,  sizeof(STJ1939_MSG_PROPERTIES));
    UINT unTempDLC = 0;
    COPY_DATA_2(&unTempDLC, pbyData, sizeof (UINT));

    if (unTempDLC > m_unDLC)
    {
        DELETE_ARRAY(m_pbyData);
        m_pbyData = new BYTE[unTempDLC];
    }

    m_unDLC = unTempDLC;
    COPY_DATA_2(m_pbyData, pbyData, (sizeof (BYTE) * m_unDLC));
}
void tagSTJ1939_MSG::vInitialize(int nSize)
{
    if (m_unDLC < (UINT)nSize)
    {
        DELETE_ARRAY(m_pbyData);
        m_pbyData = new BYTE[nSize];
    }

    m_unDLC = nSize;
}

tagSTJ1939_MSG& tagSTJ1939_MSG::operator=(const tagSTJ1939_MSG& RefObj)
{
    m_sMsgProperties = RefObj.m_sMsgProperties;

    if (m_unDLC < RefObj.m_unDLC)
    {
        delete[] m_pbyData;
        m_pbyData = new BYTE[RefObj.m_unDLC];
    }

    memcpy(m_pbyData, RefObj.m_pbyData, RefObj.m_unDLC);
    m_unDLC = RefObj.m_unDLC;
    return *this;
}

void tagSTJ1939_MSG::vSetSortField(int nField)
{
    m_nSortField = nField;
}

void tagSTJ1939_MSG::vSetSortAscending(bool bAscending)
{
    m_nMFactor = bAscending ? 1 : -1;
};

int tagSTJ1939_MSG::DoCompareIndiv(const void* pEntry1, const void* pEntry2)
{
    UINT Result = 0;
    STJ1939_MSG* pJ1939Msg1 = (STJ1939_MSG*) pEntry1;
    STJ1939_MSG* pJ1939Msg2 = (STJ1939_MSG*) pEntry2;

    switch (m_nSortField)
    {
        case 8: // Sort by DEST
        {
            Result = (UINT) (pJ1939Msg1->m_sMsgProperties.m_uExtendedID.m_s29BitId.m_uPGN.m_sPGN.m_byPDU_Specific -
                             pJ1939Msg2->m_sMsgProperties.m_uExtendedID.m_s29BitId.m_uPGN.m_sPGN.m_byPDU_Specific);
            Result *= m_nMFactor;

            if (Result != 0)
            {
                break;
            }
        }

        case 7: // Sort by SRC
        {
            Result = (UINT) (pJ1939Msg1->m_sMsgProperties.m_uExtendedID.m_s29BitId.m_bySrcAddress -
                             pJ1939Msg2->m_sMsgProperties.m_uExtendedID.m_s29BitId.m_bySrcAddress);
            Result *= m_nMFactor;

            if (Result != 0)
            {
                break;
            }
        }

        case 4: // Sort by PGN
        {
            Result = (UINT) (pJ1939Msg1->m_sMsgProperties.m_uExtendedID.m_s29BitId.unGetPGN() -
                             pJ1939Msg2->m_sMsgProperties.m_uExtendedID.m_s29BitId.unGetPGN());
            Result *= m_nMFactor;

            if (Result != 0)
            {
                break;
            }
        }

        case 3: // Sort by CANID
        {
            Result = (UINT) (pJ1939Msg1->m_sMsgProperties.m_uExtendedID.m_unExtID -
                             pJ1939Msg2->m_sMsgProperties.m_uExtendedID.m_unExtID);
            Result *= m_nMFactor;

            if (Result != 0)
            {
                break;
            }
        }

        case 2: // Sort by channel
        {
            Result = (int) (pJ1939Msg1->m_sMsgProperties.m_byChannel - pJ1939Msg2->m_sMsgProperties.m_byChannel);
            Result *= m_nMFactor;

            if (Result != 0)
            {
                break;
            }
        }

        case 1: // Sort by time stamp
        {
            Result = (int) (pJ1939Msg1->m_sMsgProperties.m_un64TimeStamp -
                            pJ1939Msg2->m_sMsgProperties.m_un64TimeStamp);
            Result *= m_nMFactor;
        }
        break;

        default:
        {
            ASSERT(FALSE);
        }
        break;
    }

    return Result;
}

__int64 tagSTJ1939_MSG::GetSlotID(STJ1939_MSG& pouJ1939Msg)
{
    // Form message to get message index in the CMap
    UINT nMsgID = MAKE_RX_TX_MESSAGE(
                      pouJ1939Msg.m_sMsgProperties.m_uExtendedID.m_s29BitId.unGetPGN(),
                      IS_RX_MESSAGE(pouJ1939Msg.m_sMsgProperties.m_eDirection));
    // For extended message
    nMsgID = MAKE_EXTENDED_MESSAGE_TYPE(nMsgID);
    // Apply Channel Information
    __int64 n64MapIndex = MAKE_CHANNEL_SPECIFIC_MESSAGE( nMsgID,
                          pouJ1939Msg.m_sMsgProperties.m_byChannel);
    n64MapIndex = MAKE_SOURCE_SPECIFIC_MESSAGE( n64MapIndex,
                  pouJ1939Msg.m_sMsgProperties.m_uExtendedID.m_s29BitId.m_bySrcAddress);
    n64MapIndex = MAKE_DEST_SPECIFIC_MESSAGE( n64MapIndex,
                  pouJ1939Msg.m_sMsgProperties.m_uExtendedID.m_s29BitId.m_uPGN.m_sPGN.m_byPDU_Specific);
    n64MapIndex = MAKE_TYPE_SPECIFIC_MESSAGE( n64MapIndex,
                  pouJ1939Msg.m_sMsgProperties.m_eType);
    return n64MapIndex;
};


void tagSTJ1939_MSG::vClear(void)
{
    m_sMsgProperties.m_eType = MSG_TYPE_NONE;
    m_unDLC = 0;
    DELETE_ARRAY(m_pbyData);
}

/* Starts tagFormattedData_J1939 related codes */
tagFormattedData_J1939::tagFormattedData_J1939()
{
    memset(m_acTimeSys,    0, sizeof(m_acTimeSys));
    memset(m_acTimeRel,    0, sizeof(m_acTimeRel));
    memset(m_acTimeAbs,    0, sizeof(m_acTimeAbs));
    memset(m_acMsgType,    0, sizeof(m_acMsgType));
    memset(m_acChannel,    0, sizeof(m_acChannel));
    memset(m_acPGNHex,     0, sizeof(m_acPGNHex));
    memset(m_acPGNDec,     0, sizeof(m_acPGNDec));
    memset(m_acMsgName,    0, sizeof(m_acMsgName));
    memset(m_acSenderName, 0, sizeof(m_acSenderName));
    memset(m_acSrcHex,     0, sizeof(m_acSrcHex));
    memset(m_acSrcDec,     0, sizeof(m_acSrcDec));
    memset(m_acDestHex,    0, sizeof(m_acDestHex));
    memset(m_acDestDec,    0, sizeof(m_acDestDec));
    memset(m_acPriority,   0, sizeof(m_acPriority));
    memset(m_acMsgDir,     0, sizeof(m_acMsgDir));
    memset(m_acDataLen,    0, sizeof(m_acDataLen));
    m_acMsgDir[LEN_STR_DIR_J1939 - 2] = L'x'; // It will be either Tx or Rx
    m_pcDataHex = NULL;
    m_pcDataDec = NULL;
}

tagFormattedData_J1939::~tagFormattedData_J1939()
{
    DELETE_ARRAY(m_pcDataHex);
    DELETE_ARRAY(m_pcDataDec);
}
/* Ends tagFormattedData_J1939 related codes */


// Helper functions start
void GetMessageTypeStr(EJ1939_MSG_TYPE eType, TCHAR acResult[LEN_STR_TYPE_J1939])
{
    switch (eType)
    {
        case MSG_TYPE_NONE:
            strcpy_s(acResult, LEN_STR_TYPE_J1939, _T("NONE"));
            break;

        case MSG_TYPE_COMMAND:
            strcpy_s(acResult, LEN_STR_TYPE_J1939, _T("CMD"));
            break;

        case MSG_TYPE_REQUEST:
            strcpy_s(acResult, LEN_STR_TYPE_J1939, _T("RQST"));
            break;

        case MSG_TYPE_DATA:
            strcpy_s(acResult, LEN_STR_TYPE_J1939, _T("DATA"));
            break;

        case MSG_TYPE_BROADCAST:
            strcpy_s(acResult, LEN_STR_TYPE_J1939, _T("BROADCAST"));
            break;

        case MSG_TYPE_ACKNOWLEDGEMENT:
            strcpy_s(acResult, LEN_STR_TYPE_J1939, _T("ACK"));
            break;

        case MSG_TYPE_GROUP_FUNCTIONS:
            strcpy_s(acResult, LEN_STR_TYPE_J1939, _T("GRP_FUNC"));
            break;

        case MSG_TYPE_NM_ACL:
            strcpy_s(acResult, LEN_STR_TYPE_J1939, _T("ACL"));
            break;

        case MSG_TYPE_NM_RQST_ACL:
            strcpy_s(acResult, LEN_STR_TYPE_J1939, _T("RQST_ACL"));
            break;

        case MSG_TYPE_NM_CMD_ADDRESS:
            strcpy_s(acResult, LEN_STR_TYPE_J1939, _T("CA"));
            break;

        case MSG_TYPE_NM_TPCM_BAM:
            strcpy_s(acResult, LEN_STR_TYPE_J1939, _T("BAM"));
            break;

        case MSG_TYPE_NM_TPCM_RTS:
            strcpy_s(acResult, LEN_STR_TYPE_J1939, _T("RTS"));
            break;

        case MSG_TYPE_NM_TPCM_CTS:
            strcpy_s(acResult, LEN_STR_TYPE_J1939, _T("CTS"));
            break;

        case MSG_TYPE_NM_TPCM_EOM_ACK:
            strcpy_s(acResult, LEN_STR_TYPE_J1939, _T("EOM"));
            break;

        case MSG_TYPE_NM_TPCM_CON_ABORT:
            strcpy_s(acResult, LEN_STR_TYPE_J1939, _T("CON_ABORT"));
            break;

        case MSG_TYPE_NM_TPDT:
            strcpy_s(acResult, LEN_STR_TYPE_J1939, _T("TPDT"));
            break;

        default:
            ASSERT(FALSE);
    }
}

// Helper functions end