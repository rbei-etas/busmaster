#ifndef __MHS_TYPES_H__
#define __MHS_TYPES_H__

#ifdef __cplusplus
  extern "C" {
#endif

#define MHS_STR_SIZE 255


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
