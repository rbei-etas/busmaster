#ifndef __CAN_SETUP_H__
#define __CAN_SETUP_H__

#include "mhs_types.h"

#ifdef __cplusplus
  extern "C" {
#endif

int ShowCanSetupDlg(HINSTANCE hInstance, HWND hwndm, struct TMhsCanCfg *cfg);

#ifdef __cplusplus
  }
#endif

#endif