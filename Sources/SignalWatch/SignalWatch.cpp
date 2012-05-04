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
 * \file      SignalWatch.cpp
 * \author    Ratnadip Choudhury
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 */
// SignalWatch.cpp : Defines the initialization routines for the DLL.
//

#include "SignalWatch_stdafx.h"
#include "SignalWatch_CAN.h"
#include "SignalWatch_J1939.h"
#include "SignalWatch.h"


#define USAGE_EXPORT
#include "SignalWatch_extern.h"

static CSignalWatch_CAN* sg_pouSW_CAN = NULL;
static CSignalWatch_J1939* sg_pouSW_J1939 = NULL;

//
//  Note!
//
//      If this DLL is dynamically linked against the MFC
//      DLLs, any functions exported from this DLL which
//      call into MFC must have the AFX_MANAGE_STATE macro
//      added at the very beginning of the function.
//
//      For example:
//
//      extern "C" BOOL PASCAL EXPORT ExportedFunction()
//      {
//          AFX_MANAGE_STATE(AfxGetStaticModuleState());
//          // normal function body here
//      }
//
//      It is very important that this macro appear in each
//      function, prior to any calls into MFC.  This means that
//      it must appear as the first statement within the
//      function, even before any object variable declarations
//      as their constructors may generate calls into the MFC
//      DLL.
//
//      Please see MFC Technical Notes 33 and 58 for additional
//      details.
//

// CSignalWatchApp

BEGIN_MESSAGE_MAP(CSignalWatchApp, CWinApp)
END_MESSAGE_MAP()


// CSignalWatchApp construction

CSignalWatchApp::CSignalWatchApp()
{
    // TODO: add construction code here,
    // Place all significant initialization in InitInstance
}


// The one and only CSignalWatchApp object

CSignalWatchApp theApp;


// CSignalWatchApp initialization

BOOL CSignalWatchApp::InitInstance()
{
    CWinApp::InitInstance();
    return TRUE;
}

INT CSignalWatchApp::ExitInstance()
{
    if (NULL != sg_pouSW_CAN)
    {
        sg_pouSW_CAN->ExitInstance();
        delete sg_pouSW_CAN;
        sg_pouSW_CAN = NULL;
    }

    if (NULL != sg_pouSW_J1939)
    {
        sg_pouSW_J1939->ExitInstance();
        delete sg_pouSW_J1939;
        sg_pouSW_J1939 = NULL;
    }

    CWinApp::ExitInstance();
    return TRUE;
}


USAGEMODE HRESULT SW_GetInterface(ETYPE_BUS eBus,
                                  void** ppvInterface)
{
    HRESULT hResult = S_OK;

    switch (eBus)
    {
        case CAN:
        {
            if (NULL == sg_pouSW_CAN)
            {
                if ((sg_pouSW_CAN = new CSignalWatch_CAN) == NULL)
                {
                    ASSERT(FALSE);
                    hResult = S_FALSE;
                }
                else
                {
                    sg_pouSW_CAN->InitInstance();
                }
            }

            // Else the object has been existing already
            *ppvInterface = (void*) sg_pouSW_CAN; /* Doesn't matter even
                                if sg_pouFP_CAN is null */
        }
        break;

        case J1939:
        {
            if (NULL == sg_pouSW_J1939)
            {
                if ((sg_pouSW_J1939 = new CSignalWatch_J1939) == NULL)
                {
                    ASSERT(FALSE);
                    hResult = S_FALSE;
                }
                else
                {
                    sg_pouSW_J1939->InitInstance();
                }
            }

            // Else the object has been existing already
            *ppvInterface = (void*) sg_pouSW_J1939; /* Doesn't matter even
                                if sg_pouFP_CAN is null */
        }
        break;

        case MCNET:
        default:
            hResult = S_FALSE;
            break;
    }

    return hResult;
}