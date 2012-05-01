/**
 * \file      struct.h
 * \author    Amitesh Bharti
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * This file contain the definition of structure used in message
 * handler.
 */

#pragma once

#pragma pack(push,1)

typedef struct sTCAN_MSG
{
    unsigned int m_unMsgID;     /* 11/29 Bit- */
    unsigned char m_ucEXTENDED; /* true, for (29 Bit) Frame */
    unsigned char m_ucRTR;      /* true, for remote request */
    unsigned char m_ucDLC;      /* Data len (0..8) */
    unsigned char m_ucChannel;  /* Message Length */
    union
    {
        unsigned char      m_aucData[8];
        unsigned short int m_auwData[4];
        unsigned long  int m_aulData[2];
    } m_sWhichBit;
} STCAN_MSG;

/* This structure holds the error and the channel number */
typedef struct sCAN_ERR
{
    unsigned char m_ucTxError;
    unsigned char m_ucRxError;
    unsigned char m_ucChannel;
} SCAN_ERR;

#pragma pack(pop,1)
