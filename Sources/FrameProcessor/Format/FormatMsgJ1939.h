
/******************************************************************************
  Project       :  Auto-SAT_Tools
  FileName      :  FormatMsgJ1939.h
  Description   :  Definition of CFormatMsgJ1939 class.
  $Log:   X:/Archive/Sources/FrameProcessor/FormatMsgJ1939.h_v  $
 * 
 *    Rev 1.1   15 Apr 2011 19:19:42   rac2kor
 * Inserted RBEI Copyright information text into the file header.
 * 
 *    Rev 1.0   06 Dec 2010 18:56:20   rac2kor
 *  

  Author(s)     :  Ratnadip Choudhury
  Date Created  :  1.12.2010
  Modified By   :  
  Copyright (c) 2011, Robert Bosch Engineering and Business Solutions.  All rights reserved.
******************************************************************************/

#ifndef FORMAT_MSG_J1939_H_INCLUDED
#define FORMAT_MSG_J1939_H_INCLUDED

#include "Datatypes/J1939_Datatypes.h"
#include "FormatMsgCommon.h"

class CFormatMsgJ1939: public CFormatMsgCommon
{
private:
    //CJ1939MessageMap* m_podJ1939Rules;

    void vFormatTime(BYTE bExprnFlag, PSTJ1939_MSG psJ1939BData,
                     PSFORMATTEDATA_J1939 psJ1939FData);
    void vFormatDataAndId(BYTE bExprnFlag, PSTJ1939_MSG psJ1939BData,
                     PSFORMATTEDATA_J1939 psJ1939FData);
public:
    CFormatMsgJ1939(void);
    ~CFormatMsgJ1939(void);
    
    void vInit(/*CJ1939MessageMap* podMsgMap*/);
    void vFormatJ1939DataMsg(STJ1939_MSG* pMsgMcANet, 
                             SFORMATTEDATA_J1939* CurrDataJ1939,
                             BYTE bExprnFlag_Log);
};

#endif//FORMAT_MSG_J1939_H_INCLUDED