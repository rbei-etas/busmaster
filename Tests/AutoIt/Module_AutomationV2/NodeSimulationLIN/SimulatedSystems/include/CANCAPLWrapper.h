#pragma once
/***CAN Protocol CAPL Wrapper - Begin***/
GCC_EXTERN UCHAR* byte_CAPL(STCAN_MSG* sMsg,int dlc, INT nByte);  //deprecated
GCC_EXTERN USHORT* word_CAPL(STCAN_MSG* sMsg,int dlc, INT nByte); //deprecated
GCC_EXTERN ULONG* long_CAPL(STCAN_MSG* sMsg,int dlc, INT nByte);  //deprecated
GCC_EXTERN INT output_CAPL(void* ouMsg);
/***CAN Protocol CAPL Wrapper - End***/
