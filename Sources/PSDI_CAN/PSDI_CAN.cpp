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
 * \file      PSDI_CAN.cpp
 * \brief     Implementation of CPSDI_CANApp class
 * \authors   Anish Kumar, Arunkumar Karri
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Implementation of CPSDI_CANApp class
 */

#include "PSDI_CAN/stdafx_CAN.h"
#include "PSDI_CAN.h"
#include "MsgContainer_CAN.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define USAGE_EXPORT
#include "Application/PSDI_Extern.h"
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

// CPSDI_CANApp

BEGIN_MESSAGE_MAP(CPSDI_CANApp, CWinApp)
END_MESSAGE_MAP()


// CPSDI_CANApp construction

CPSDI_CANApp::CPSDI_CANApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}


// The one and only CPSDI_CANApp object

CPSDI_CANApp theApp;


// CPSDI_CANApp initialization

BOOL CPSDI_CANApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}
static CMsgContainerCAN* sg_pouMsgContainer_CAN = NULL;

USAGEMODE HRESULT PSDI_GetInterface(ETYPE_BUS eBus, void** ppvInterface)
{
    HRESULT hResult = S_OK;

    switch (eBus)
    {
        case CAN:
        {
            if (NULL == sg_pouMsgContainer_CAN)
            {
                if ((sg_pouMsgContainer_CAN = new CMsgContainerCAN) == NULL)
                {
                    ASSERT(FALSE); hResult = S_FALSE;
                }
            }

			// Else the object has been existing already
			*ppvInterface = (void *) sg_pouMsgContainer_CAN;
        }
        break;
		case J1939:		
        default: 
		{
			*ppvInterface = NULL;
			hResult = S_FALSE; 
		}
		break;
    }

    return hResult;
}