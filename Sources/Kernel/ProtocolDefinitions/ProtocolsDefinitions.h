#pragma once

enum ETYPE_BUS
{
    CAN = 0,
    MCNET,
    J1939,
    FLEXRAY,
    LIN,
    MOST,
    BUS_TOTAL,
    BUS_INVALID
};


enum eBUSEVENT
{
    ON_ACTIVATED = 0,       //Feature usage
    ON_CONNECT,
    ON_PRE_CONNECT,
    ON_DISCONNECT,
    ON_DEACTIVATED
};
