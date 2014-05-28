/***************************************************************************
                          mhsbmcfg.c  -  description
                             -------------------
    begin             : 24.12.2011
    copyright         : Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
    author            : Saravanan
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software, you can redistribute it and/or modify  *
 *   it under the terms of the GNU Lesser General Public License           *
 *   version 2.1 as published by the Free Software Foundation.             *
 *                                                                         *
 ***************************************************************************/

/**
    Library to talk to Tiny-CAN devices. You find the latest versions at
       http://www.tiny-can.com/
**/
#include "global.h"
#include <windows.h>
#include <string.h>
#include <stdio.h>
#include <locale.h>
#include <stddef.h>
#include <tchar.h>
#include "mhs_types.h"
#include "util.h"
#include "can_setup.h"
#include "mhsbmcfg.h"


HINSTANCE AppHInstance = nullptr;

/***************************************************************/
/*  DLL Main Funktion                                          */
/***************************************************************/
int WINAPI DllMain(HINSTANCE hInstance, DWORD fdwReason, PVOID pvReserved)
{
switch (fdwReason)
  {
  case DLL_PROCESS_ATTACH : {
                            AppHInstance = hInstance;
                            //setlocale(LC_NUMERIC, "german");
                            break;
                            }
  case DLL_THREAD_ATTACH  : break;
  case DLL_THREAD_DETACH  : break;
  case DLL_PROCESS_DETACH : {
                            break;
                            }
  }
return(TRUE);
}


int WINAPI ShowCanSetup(HWND hwnd, struct TMhsCanCfg *setup)
{
return(ShowCanSetupDlg(AppHInstance, hwnd, setup));
}
