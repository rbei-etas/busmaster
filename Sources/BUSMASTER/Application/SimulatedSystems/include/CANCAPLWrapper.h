#pragma once
#include "CAPLWrapper.h"
/***CAN Protocol CAPL Wrapper - Begin***/
GCC_EXTERN UCHAR* byte_CAPL(STCAN_MSG* sMsg,int dlc, INT nByte);  //deprecated
GCC_EXTERN USHORT* word_CAPL(STCAN_MSG* sMsg,int dlc, INT nByte); //deprecated
GCC_EXTERN ULONG* long_CAPL(STCAN_MSG* sMsg,int dlc, INT nByte);  //deprecated
GCC_EXTERN INT output_CAPL(void* ouMsg);
GCC_EXTERN void initialize(STCAN_MSG* pCANMsg, unsigned int id, bool isExtended, unsigned char dlc = 8,
                           unsigned char cluster = 1, int byte1 = 0,int byte2 = 0, int byte3 = 0, int byte4 = 0, int byte5 = 0, int byte6 = 0, int byte7 = 0, int byte8 = 0);
/***CAN Protocol CAPL Wrapper - End***/
