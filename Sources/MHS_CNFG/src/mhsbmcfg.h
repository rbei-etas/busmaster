#pragma once

#include "global.h"
#include "mhs_types.h"

#ifdef __cplusplus
  extern "C" {
#endif

int WINAPI ShowCanSetup(HWND hwnd, struct TMhsCanCfg *setup);

#ifdef __cplusplus
  }
#endif
