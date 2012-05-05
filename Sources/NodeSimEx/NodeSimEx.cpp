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
 * \file      NodeSimEx.cpp
 * \author    Ratnadip Choudhury
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 */
// NodeSimEx.cpp : Defines the initialization routines for the DLL.
//
#include "NodeSimEx_stdafx.h"
#include "Include/Utils_macro.h"
#include "Include/BaseDefs.h"
#include "NodeSim.h"
#define USAGE_EXPORT
#include "NodeSimEx_extern.h"

static CNodeSim* sg_pouNS_CAN = NULL;
static CNodeSim* sg_pouNS_J1939 = NULL;
static CDynLinkLibrary* sg_pomDynLinkLib = NULL;

static AFX_EXTENSION_MODULE NodeSimExDLL = { NULL, NULL };

extern "C" int APIENTRY
DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
    // Remove this if you use lpReserved
    UNREFERENCED_PARAMETER(lpReserved);

    if (dwReason == DLL_PROCESS_ATTACH)
    {
        TRACE0("NodeSimEx.DLL Initializing!\n");

        // Extension DLL one-time initialization
        if (!AfxInitExtensionModule(NodeSimExDLL, hInstance))
        {
            return 0;
        }

        // Insert this DLL into the resource chain
        // NOTE: If this Extension DLL is being implicitly linked to by
        //  an MFC Regular DLL (such as an ActiveX Control)
        //  instead of an MFC application, then you will want to
        //  remove this line from DllMain and put it in a separate
        //  function exported from this Extension DLL.  The Regular DLL
        //  that uses this Extension DLL should then explicitly call that
        //  function to initialize this Extension DLL.  Otherwise,
        //  the CDynLinkLibrary object will not be attached to the
        //  Regular DLL's resource chain, and serious problems will
        //  result.
        sg_pomDynLinkLib = new CDynLinkLibrary(NodeSimExDLL);
    }
    else if (dwReason == DLL_PROCESS_DETACH)
    {
        if (sg_pouNS_CAN != NULL)
        {
            sg_pouNS_CAN->ExitInstance();
            delete sg_pouNS_CAN;
            sg_pouNS_CAN = NULL;
        }

        if (sg_pouNS_J1939 != NULL)
        {
            sg_pouNS_J1939->ExitInstance();
            delete sg_pouNS_J1939;
            sg_pouNS_J1939 = NULL;
        }

        if (NULL != sg_pomDynLinkLib)
        {
            delete sg_pomDynLinkLib;
            sg_pomDynLinkLib = NULL;
        }

        // Terminate the library before destructors are called
        AfxTermExtensionModule(NodeSimExDLL);
    }

    //CGlobalObj::m_pEditorDocTemplate = NULL;
    return 1;   // ok
}
USAGEMODE HRESULT NS_GetInterface(ETYPE_BUS eBus,
                                  void** ppvInterface)
{
    HRESULT hResult = S_OK;
    //Place this code at the beginning of the export function.
    //Save previous resource handle and switch to current one.
    HINSTANCE hInst = AfxGetResourceHandle();
    AfxSetResourceHandle(NodeSimExDLL.hResource);

    switch (eBus)
    {
        case CAN:
        {
            if (NULL == sg_pouNS_CAN)
            {
                if ((sg_pouNS_CAN = new CNodeSim(CAN)) == NULL)
                {
                    ASSERT(FALSE);
                    hResult = S_FALSE;
                }
                else
                {
                    sg_pouNS_CAN->InitInstance();
                }
            }

            // Else the object has been existing already
            *ppvInterface = (void*) sg_pouNS_CAN;
            /* Doesn't matter even if sg_pouFP_CAN is null */
        }
        break;

        case J1939:
        {
            if (NULL == sg_pouNS_J1939)
            {
                if ((sg_pouNS_J1939 = new CNodeSim(J1939)) == NULL)
                {
                    ASSERT(FALSE);
                    hResult = S_FALSE;
                }
                else
                {
                    sg_pouNS_J1939->InitInstance();
                }
            }

            // Else the object has been existing already
            *ppvInterface = (void*) sg_pouNS_J1939;
            /* Doesn't matter even if sg_pouFP_J1939 is null */
        }
        break;

        default:
        {
            hResult = S_FALSE;
            *ppvInterface = NULL;
        }
        break;
    }

    //Place this at the end of the export function.
    //switch back to previous resource handle.
    AfxSetResourceHandle(hInst);
    return hResult;
}