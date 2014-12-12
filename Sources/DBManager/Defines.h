#pragma once

#define defERR_FILE_OPEN -1
#define defLIN_MASTER_FRAME_ID          0x3C
#define defLIN_SLAVE_FRAME_ID           0x3D

#define defLIN_NAD_INDEX                0
#define defLIN_PCI_INDEX                1
#define defLIN_SID_INDEX                2
#define defLIN_RSID_INDEX               3


#define defLIN_SID_ASSIGN_FRAME_ID      0xB1
#define defLIN_SID_UNASSIGN_FRAME_ID    0xB1
#define defLIN_SID_READ_BY_IDENTIFIER   0xB2
#define defLIN_SID_ASSIGN_NAD_ID        0xB0
#define defLIN_SID_COND_CHANGE_NAD      0xB3
#define defLIN_SID_DATA_DUMP            0xB4
#define defLIN_SID_SAVE_CONFIG          0xB6
#define defLIN_SID_ASSIGN_FRAME_RANGE   0xB7
union SignalInitVal
{
    unsigned __int64 unInitVal;
    char chInitVal[8];
};

