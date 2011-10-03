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
 * \file      DIL_Interface/DIL_Interface.cpp
 * \brief     Source file for the exported function used to retrieve the
 * \author    Pradeep Kadoor
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Source file for the exported function used to retrieve the
 */


// DIL_Interface.cpp : 
#include "DIL_Interface_stdafx.h"
#include "Include/BaseDefs.h"

#define USAGE_EXPORT
#include "DIL_Interface_extern.h"

#include "DIL_CAN.h"
#include "DIL_Interface.h"


static CDIL_CAN* sg_pouDIL_CAN = NULL;

// CDILApp

BEGIN_MESSAGE_MAP(CDILApp, CWinApp)
END_MESSAGE_MAP()


/**
 * CDILApp construction
 */
CDILApp::CDILApp()
{
    // TODO: add construction code here,
}

CDILApp::~CDILApp()
{
    // TODO: add construction code here,
}

/**
 * The one and only CDILApp object
 */
CDILApp theApp;

/**
 * CDILApp initialization
 */
BOOL CDILApp::InitInstance()
{
    return TRUE;
}

int CDILApp::ExitInstance()
{
    INT nResult = -1;
    // TODO: Add your specialized code here and/or call the base class
    if (NULL != sg_pouDIL_CAN)
    {
        nResult = sg_pouDIL_CAN->ExitInstance();
        delete sg_pouDIL_CAN;
        sg_pouDIL_CAN = NULL;
    }
    return nResult;
}

USAGEMODE HRESULT DIL_GetInterface(ETYPE_BUS eBusType, void** ppvInterface)
{
    HRESULT hResult = S_OK;

    switch (eBusType)
    {
        case CAN:
        {
            if (NULL == sg_pouDIL_CAN)
            {
                if ((sg_pouDIL_CAN = new CDIL_CAN) == NULL)
                {
                    ASSERT(FALSE);
                    hResult = S_FALSE;
                }
                else
                {
                    sg_pouDIL_CAN->InitInstance();
                }
            }
            // Else the object has been existing already
            *ppvInterface = (void *) sg_pouDIL_CAN; /* Doesn't matter even 
                                                if sg_pouFP_CAN is null */
        }
        break;
        case MCNET:
        case J1939: 
        default: 
        {
            hResult = S_FALSE;
        }
        break;
    }

    return hResult;
}