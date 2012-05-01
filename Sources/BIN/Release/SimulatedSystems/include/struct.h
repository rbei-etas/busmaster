/********************************************************************************
  Project       :  CAN-Monitor
  FileName      :  struct.h
  Description   :  This file contain the definition of structure used in message
                   handler
  $Log:   Y:/EMC2/Projects/CAN_Monitor_Maintenance/Archives/Source/Misc/GCC/struct.h_v  $
 *
 *    Rev 1.4   21 Jul 2005 19:06:48   amb2kor
 * changes due to Multi Channel concept.
 *
 *    Rev 1.3   Jun 04 2004 12:29:08   amb2kor
 * Changes in databytes to access data in char, word,long
 *
 *
 *    Rev 1.2   29 Apr 2003 16:39:02   amb2kor
 * changed the name m_mucData to m_ucData
 *
 *    Rev 1.1   24 Apr 2003 09:50:02   amb2kor
 * Changes to Pack the structure
 *
 *    Rev 1.0   04 Apr 2003 11:25:36   amb2kor
 * Initial version

  Author(s)     :  Amitesh Bharti
  Date Created  :  25.03.2003
  Modified By   :
********************************************************************************/
#ifndef _STRUCT_H_
#define _STRUCT_H_
#pragma pack(push,1)

typedef struct sTCAN_MSG
{
    unsigned int m_unMsgID;     // 11/29 Bit-
    unsigned char m_ucEXTENDED; // true, for (29 Bit) Frame
    unsigned char m_ucRTR;      // true, for remote request
    unsigned char m_ucDLC;  // Data len (0..8)
    unsigned char m_ucChannel;  //Message Length
    union
    {
        unsigned char      m_aucData[8];
        unsigned short int m_auwData[4];
        unsigned long  int m_aulData[2];
    } m_sWhichBit;
} STCAN_MSG;

//This structure holds the error and the channel number
typedef struct sCAN_ERR
{
    unsigned char m_ucTxError ;
    unsigned char m_ucRxError ;
    unsigned char m_ucChannel ;
} SCAN_ERR;



#pragma pack(pop,1)
#endif