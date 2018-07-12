/**
 * \file      CANCAPLWrapper
 * \brief
 * \author    Robin G.K.
 * \date      Created 20/07/2015
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 */
#include "CANIncludes.h"

unsigned char* byte_CAPL(STCAN_MSG* sMsg,int dlc, int nByte)
{
    if(nByte<dlc)
    {
        return ((&(sMsg->data[nByte])));
    }
    else
    {
        return NULL;
    }
}

unsigned short* word_CAPL(STCAN_MSG* sMsg,int dlc, int nByte)
{
    if(nByte<(dlc-2))
    {
        return  ((unsigned short*)(&(sMsg->data[nByte])));
    }
    else
    {
        return NULL;
    }
}

unsigned long* long_CAPL(STCAN_MSG* sMsg,int dlc, int nByte)
{
    if(nByte<(dlc-4))
    {
        return  ((unsigned long*)(&(sMsg->data[nByte])));
    }
    else
    {
        return NULL;
    }
}
INT output_CAPL(void* ouMsg)
{
    return SendMsg(*((STCAN_MSG*)ouMsg));
}

void initialize(STCAN_MSG* pCANMsg, unsigned int id, bool isExtended, unsigned char dlc, unsigned char cluster, int byte1,
                int byte2, int byte3, int byte4, int byte5, int byte6, int byte7, int byte8)
{
    pCANMsg->id = id;
    pCANMsg->isExtended = isExtended;
    pCANMsg->dlc = dlc;
    pCANMsg->cluster = cluster;
    pCANMsg->data[0] = byte1;
    pCANMsg->data[1] = byte2;
    pCANMsg->data[2] = byte3;
    pCANMsg->data[3] = byte4;
    pCANMsg->data[4] = byte5;
    pCANMsg->data[5] = byte6;
    pCANMsg->data[6] = byte7;
    pCANMsg->data[7] = byte8;
}


