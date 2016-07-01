/*
* Copyright Robert Bosch Engineering and Business Solutions Private Limited. All Rights Reserved.
*/

#pragma once

enum eFlexRayChannel
{
    eFlexRayChannelNone,
    eFlexRayChannelA,
    eFlexRayChannelB,
    eFlexRayChannelAB,
    eFlexRayChannelTotal
};
enum eFlexRaySlotType
{
    eStatic,
    eDynamic,
};
enum eFlexRayFrameType
{
    eFelxRayFrameNone,
    eNormal,
    eSync,
    eStartUp,
    eSyncStartup,
};

enum eFlexRayKeySlotType
{
    eSyncType,
    eSyncStartupType,
    eNone
};