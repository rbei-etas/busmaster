/*
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/**
 * \file      mhsbmcfg.c
 * \brief     Library to talk to Tiny-CAN devices
 * \author    Klaus Demlehner
 * \copyright Copyright (c) 2011, MHS-Elektronik GmbH & Co. KG
 *
 * Library to talk to Tiny-CAN devices.
 * You find the latest versions at http://www.tiny-can.com/
 */

#include "global.h"
#include <windows.h>
#include <string.h>
#include <stdio.h>
#include <locale.h>
#include <stddef.h>
#include "mhs_types.h"
#include "util.h"
#include "can_setup.h"
#include "mhsbmcfg.h"


HINSTANCE AppHInstance = NULL;

/***************************************************************/
/*  DLL Main Function                                          */
/***************************************************************/
int WINAPI DllMain(HINSTANCE hInstance, DWORD fdwReason, PVOID pvReserved)
{
    switch (fdwReason)
    {
        case DLL_PROCESS_ATTACH:
            AppHInstance = hInstance;
            //setlocale(LC_NUMERIC, "german");
            break;

        case DLL_THREAD_ATTACH:
            break;

        case DLL_THREAD_DETACH:
            break;

        case DLL_PROCESS_DETACH:
            break;
    }

    return(TRUE);
}


int WINAPI ShowCanSetup(HWND hwnd, struct TMhsCanCfg* setup)
{
    return(ShowCanSetupDlg(AppHInstance, hwnd, setup));
}
