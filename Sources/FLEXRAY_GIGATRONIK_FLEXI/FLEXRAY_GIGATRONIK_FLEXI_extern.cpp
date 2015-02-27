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

/**********************************************************************************************************************
 *
 *  FILENAME:           FLEXRAY_GIGATRONIK_FLEXI_extern.cpp
 *
 *  COPYRIGHT:          Gigatronik Ingolstadt GmbH
 *  AUTHOR, DPT:        M. Morgott, KE
 *
 *  DATE OF CREATION:   28.11.2014
 *
 *  DESCRIPTION:        Creates DIL-object und exports API functions for GIGABOX flex-i FLEXRAY Hardware interface.
 *
 *********************************************************************************************************************/

/** --- CHANGE LOG ---
 *  V1.0.0.0        First reviewed version
 *
 */

/* Project includes */
#include "FLEXRAY_GIGATRONIK_FLEXI.h"                   // definition of class CDIL_FLEXRAY_GIGATRONIK_FLEXI
#define USAGE_EXPORT
#include "FLEXRAY_GIGATRONIK_FLEXI_extern.h"



/********************** object of the only one flex-i instance to pass the object to the application **********************/
CDIL_FLEXRAY_GIGATRONIK_FLEXI* g_pouDIL_FLEXRAY_GIGATRONIK_FLEXI = nullptr;


/* This function will be called by the DIL control if this driver got selected. It returns an instance of the flexi DIL class. */
USAGEMODE HRESULT GetIDIL_FLEXRAY_Controller(void** ppvInterface)
{

    HRESULT hResult = S_OK;


    if ( nullptr == g_pouDIL_FLEXRAY_GIGATRONIK_FLEXI )
    {
        if ((g_pouDIL_FLEXRAY_GIGATRONIK_FLEXI = new CDIL_FLEXRAY_GIGATRONIK_FLEXI()) == nullptr)
        {
            hResult = S_FALSE;
        }
    }
    *ppvInterface = (void*) g_pouDIL_FLEXRAY_GIGATRONIK_FLEXI;  /* Doesn't matter even if g_pouDIL_FLEXRAY_BOA is null */


    return hResult;
}
