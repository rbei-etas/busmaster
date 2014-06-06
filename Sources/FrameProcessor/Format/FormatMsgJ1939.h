/******************************************************************************
  Description   :  Definition of CFormatMsgJ1939 class.
  Author(s)     :  Ratnadip Choudhury
  Date Created  :  1.12.2010
  Copyright (c) 2011, Robert Bosch Engineering and Business Solutions.  All rights reserved.
******************************************************************************/

#pragma once

#include "Datatypes/J1939_Datatypes.h"
#include "FormatMsgCommon.h"

class CFormatMsgJ1939: public CFormatMsgCommon
{
public:
    CFormatMsgJ1939(CRefTimeKeeper & ouRefTimeKeeper);
    ~CFormatMsgJ1939();

    void vInit();
    void vFormatJ1939DataMsg(STJ1939_MSG* pMsgMcANet,
                             SFORMATTEDATA_J1939* CurrDataJ1939,
                             BYTE bExprnFlag_Log);

private:
    void vFormatTime(BYTE bExprnFlag, PSTJ1939_MSG psJ1939BData,
                     PSFORMATTEDATA_J1939 psJ1939FData);
    void vFormatDataAndId(BYTE bExprnFlag, PSTJ1939_MSG psJ1939BData,
                          PSFORMATTEDATA_J1939 psJ1939FData);
};
