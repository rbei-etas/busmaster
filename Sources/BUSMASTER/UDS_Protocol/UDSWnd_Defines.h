/**
 * \file      UDSWnd_Defines.h
 * \brief     Definition file for UDS_Protocol project
 * \author    Sanchez Marin Maria Alejandra
 * Designer:  Bentea Radu Mihai
 */

#pragma once

//#include "include/struct_can.h"
#include "CANDriverDefines.h"
const int NO_OF_CHAR_IN_BYTE = 2;


/** Message Structure */
struct msTXMSGDATA
{
    UINT  m_unCount;              // Total array element in the point m_psTxMsg
    PSTCAN_MSG m_psTxMsg;         // pointer to array of structure for
    // Transmitting  the data.
};
typedef msTXMSGDATA mSTXSELMSGDATA;
typedef msTXMSGDATA* mPSTXSELMSGDATA;

#define defDIV_FACT_FOR_SECOND              10000.0
#define defFORMAT_MSGID_DECIMAL "%d"
#define TIME_UNABLE_SEND_BUTTON             10
#define MASK_SA_ID_29Bits                   0x3FF800

#define MASK_TA_ID_29Bits                   0x7FF

/** It's used to change the SA from the Main Wnd */
#define NEG_MASK_SA_ID_29Bits               0x1FC007FF

/** It's used to change the TA from the Main Wnd */
#define NEG_MASK_TA_ID_29Bits               0x1FFFF800

#define MAX_VALUE_29BIT_ID                  0x1FFFFFFF
#define MASK_TA_ID_29Bits_J1939             0xFF00
//#define NUM_BYTES_SHOWN_RESP                16
