#ifndef __MHS_TYPES_H__
#define __MHS_TYPES_H__

#include "global.h"

#ifdef __cplusplus
extern "C" {
#endif


    struct TMhsCanCfg
    {
        char CanSnrStr[MHS_STR_SIZE];
        unsigned int CanSpeed;
        unsigned int CanBtrValue;
    };


#ifdef __cplusplus
}
#endif

#endif
