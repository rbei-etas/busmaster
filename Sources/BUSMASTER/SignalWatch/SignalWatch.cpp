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
#include "SignalWatch_LIN.h"
#include "SignalWatch.h"
#include "../Application/MultiLanguage.h"
#include "BaseSignalWatch.h"

#define USAGE_EXPORT
#include "SignalWatch_extern.h"

static CBaseSignalWatch* sg_pouSW_CAN = nullptr;
static CBaseSignalWatch* sg_pouSW_J1939 = nullptr;
static CBaseSignalWatch* sg_pouSW_LIN = nullptr;

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
static HINSTANCE ghLangInst=nullptr;
BOOL CSignalWatchApp::InitInstance()
{
    CWinApp::InitInstance();
    if ( CMultiLanguage::m_nLocales <= 0 )    // Not detected yet
    {
        CMultiLanguage::DetectLangID(); // Detect language as user locale
        CMultiLanguage::DetectUILanguage();    // Detect language in MUI OS
    }
    TCHAR szModuleFileName[MAX_PATH];        // Get Module File Name and path
    int ret = ::GetModuleFileName(theApp.m_hInstance, szModuleFileName, MAX_PATH);
    if ( ret == 0 || ret == MAX_PATH )
    {
        ASSERT(false);
    }
    ghLangInst = CMultiLanguage::LoadLangResourceDLL( szModuleFileName );
    if (ghLangInst)
    {
        AfxSetResourceHandle( ghLangInst );
    }

    return TRUE;
}

INT CSignalWatchApp::ExitInstance()
{
    if (nullptr != sg_pouSW_CAN)
    {
        delete sg_pouSW_CAN;
        sg_pouSW_CAN = nullptr;
    }
    if (nullptr != sg_pouSW_J1939)
    {
        delete sg_pouSW_J1939;
        sg_pouSW_J1939 = nullptr;
    }
    if ( nullptr != sg_pouSW_LIN )
    {
        delete sg_pouSW_LIN;
        sg_pouSW_LIN = nullptr;
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
            if (nullptr == sg_pouSW_CAN)
            {
                if ((sg_pouSW_CAN = new CSignalWatch_CAN) == nullptr)
                {
                    ASSERT(false);
                    hResult = S_FALSE;
                }
            }
            // Else the object has been existing already
            *ppvInterface = (void*) sg_pouSW_CAN;
        }
        break;
        case J1939:
        {
            if ( nullptr == sg_pouSW_J1939 )
            {
                if ( ( sg_pouSW_J1939 = new CSignalWatch_J1939 ) == nullptr )
                {
                    ASSERT( false );
                    hResult = S_FALSE;
                }
            }
            // Else the object has been existing already
            *ppvInterface = (void*) sg_pouSW_J1939;
        }
        break;
        case LIN:
        {
            if ( nullptr == sg_pouSW_LIN )
            {
                if ( ( sg_pouSW_LIN = new CSignalWatch_LIN ) == nullptr )
                {
                    ASSERT( false );
                    hResult = S_FALSE;
                }
            }
            // Else the object has been existing already
            *ppvInterface = (void*) sg_pouSW_LIN;
        }
        break;

        case MCNET:
        default:
            hResult = S_FALSE;
            break;
    }

    return hResult;
}