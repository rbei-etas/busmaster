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
 * \file      FrameProcessor.cpp
 * \brief     Source file for the exported function used to retrieve the
 * \authors   Ratnadip Choudhury, 1.12.2010
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Source file for the exported function used to retrieve the
 */

// FrameProcessor.cpp : 

#include "FrameProcessor_stdafx.h"
#include "Datatypes/MsgBufAll_DataTypes.h"

#define USAGE_EXPORT
#include "FrameProcessor_extern.h"

#include "FrameProcessor_CAN.h"
#include "FrameProcessor_J1939.h"
//#include "FrameProcessor_McNet.h"
#include "FrameProcessor.h"


static CFrameProcessor_CAN* sg_pouFP_CAN = NULL;
static CFrameProcessor_J1939* sg_pouFP_J1939 = NULL;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//
//	Note!
//
//		If this DLL is dynamically linked against the MFC
//		DLLs, any functions exported from this DLL which
//		call into MFC must have the AFX_MANAGE_STATE macro
//		added at the very beginning of the function.
//
//		For example:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// normal function body here
//		}
//
//		It is very important that this macro appear in each
//		function, prior to any calls into MFC.  This means that
//		it must appear as the first statement within the 
//		function, even before any object variable declarations
//		as their constructors may generate calls into the MFC
//		DLL.
//
//		Please see MFC Technical Notes 33 and 58 for additional
//		details.
//

// CFrameProcessorApp

BEGIN_MESSAGE_MAP(CFrameProcessorApp, CWinApp)
END_MESSAGE_MAP()


// CFrameProcessorApp construction

CFrameProcessorApp::CFrameProcessorApp()
{
	// TODO: add construction code here,
}

CFrameProcessorApp::~CFrameProcessorApp()
{
	// TODO: add construction code here,
}

// The one and only CFrameProcessorApp object

CFrameProcessorApp theApp;


// CFrameProcessorApp initialization

BOOL CFrameProcessorApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}

int CFrameProcessorApp::ExitInstance()
{
    // TODO: Add your specialized code here and/or call the base class
    if (NULL != sg_pouFP_CAN)
    {
        sg_pouFP_CAN->ExitInstance();
        delete sg_pouFP_CAN;
        sg_pouFP_CAN = NULL;
    }

    if (NULL != sg_pouFP_J1939)
    {
        sg_pouFP_J1939->ExitInstance();
        delete sg_pouFP_J1939;
        sg_pouFP_J1939 = NULL;
    }
    return CWinApp::ExitInstance();
}


USAGEMODE HRESULT FP_GetInterface(eID_COMPONENT eInterfaceID, void** ppvInterface)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());

    HRESULT hResult = S_OK;

    switch (eInterfaceID)
    {
        case FRAMEPROC_CAN:
        {
            if (NULL == sg_pouFP_CAN)
            {
                if ((sg_pouFP_CAN = new CFrameProcessor_CAN) == NULL)
                {
                    ASSERT(FALSE);
					hResult = S_FALSE;
                }
                else
                {
                    sg_pouFP_CAN->InitInstance();					
                }
            }
            // Else the object has been existing already
            *ppvInterface = (void *) sg_pouFP_CAN; /* Doesn't matter even 
                                               if sg_pouFP_CAN is null */
        }
        break;
        case FRAMEPROC_J1939:
        {
            if (NULL == sg_pouFP_J1939)
            {
                if ((sg_pouFP_J1939 = new CFrameProcessor_J1939) == NULL)
                {
                    ASSERT(FALSE);
                    hResult = S_FALSE;
                }
                else
                {
                    sg_pouFP_J1939->InitInstance();
                }
            }
            // Else the object has been existing already
            *ppvInterface = (void *) sg_pouFP_J1939; /* Doesn't matter even 
                                               if sg_pouFP_J1939 is null */
        }
		case FRAMEPROC_MCNET:
        default: 
		{
				hResult = S_FALSE;
		}
		break;
	}

    return hResult;
}
