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
#include "stdafx.h"

#define USAGE_EXPORT
#include "Include\BusmasterDriverInterface.h"

#include "DIL_CAN.h"
#include "DIL_LIN.h"
#include "DILI_J1939.h"
#include "Include\BaseDIL_FLEXRAY.h"

static CBaseDIL_CAN* sg_pouDIL_CAN = nullptr;
static CBaseDIL_LIN* sg_pouDIL_LIN = nullptr;
static CBaseDILI_J1939* sg_pouDILI_J1939 = nullptr;
static CBaseDIL_FLEXRAY* sg_pouDIL_FLEXRAY = nullptr;

typedef HRESULT(*GETFLEXRAYDIL)(CBaseDIL_FLEXRAY** dilFlexRay);


CBaseDIL_FLEXRAY* getFlexRayDil()
{
    HMODULE dllHandle = LoadLibrary("DIL_FLEXRAY.dll");
    if (nullptr != dllHandle)
    {
        auto pGetFlexrayDil = (GETFLEXRAYDIL)GetProcAddress(dllHandle, "GetFlexRayDIL");
        if(nullptr != pGetFlexrayDil)
        {
            CBaseDIL_FLEXRAY* dilFlexRay = nullptr;
            pGetFlexrayDil(&dilFlexRay);
            return dilFlexRay;
        }
    }
    return nullptr;
}
USAGEMODE HRESULT GetDilInterface( ETYPE_BUS eBusType, IBusService** ppvInterface )
{
    HRESULT hResult = S_OK;
    switch ( eBusType )
    {
        case CAN:
        {
            if ( nullptr == sg_pouDIL_CAN )
            {
                if ( ( sg_pouDIL_CAN = new CDIL_CAN ) == nullptr )
                {
                    hResult = S_FALSE;
                }
                else
                {
                    //sg_pouDIL_CAN->InitInstance();
                }
            }
            // Else the object has been existing already
            *ppvInterface = sg_pouDIL_CAN; /* Doesn't matter even if sg_pouFP_CAN is null */
        }
        break;
        case LIN:
        {
            if ( nullptr == sg_pouDIL_LIN )
            {
                if ( ( sg_pouDIL_LIN = new CDIL_LIN ) == nullptr )
                {
                    hResult = S_FALSE;
                }
                else
                {
                    //sg_pouDIL_LIN->InitInstance();
                }
            }
            // Else the object has been existing already
            *ppvInterface = sg_pouDIL_LIN; /* Doesn't matter even if sg_pouFP_CAN is null */
        }
        break;
        case MCNET:
            break;
        case J1939:
        {
            if (nullptr == sg_pouDILI_J1939)
            {
                if ((sg_pouDILI_J1939 = new CDILI_J1939) == nullptr)
                {
                    hResult = S_FALSE;
                }
                else
                {
                    //sg_pouDILI_J1939->InitInstance();
                }
            }
            // Else the object has been existing already
            *ppvInterface = sg_pouDILI_J1939; /* Doesn't matter even if sg_pouFP_CAN is null */
        }
        break;
        case FLEXRAY:
        {
            if ( nullptr == sg_pouDIL_FLEXRAY )
            {
                sg_pouDIL_FLEXRAY = getFlexRayDil();
            }
            // Else the object has been existing already
            *ppvInterface = sg_pouDIL_FLEXRAY; /* Doesn't matter even if sg_pouFP_CAN is null */
        }
        break;
        default:
        {
            hResult = S_FALSE;
        }
        break;
    }

    return hResult;
}