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
 * @file  CAN_IXXAT_VCI.cpp
 *
 * @brief
 *  The main header file for the DLL. The corresponding
 *  CPP file exports the DLL C-function.
 *
 */

#include "CAN_IXXAT_VCI_stdafx.h"
#include <string.h>
#include "CAN_IXXAT_VCI_Extern.h"
#include "CAN_IXXAT_VCI.h"
#include "DIL_CAN_IXXAT_VCI.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

///The one and only class object for VCI access.
//  It will be created in the C-function GetIDIL_CAN_Controller
//  and deleted in the ExitInstance function.
static CDIL_CAN_IXXAT_VCI* g_pouDIL_CAN_IXXAT_VCI = NULL;

// The one and only CCAN_IXXAT_VCIApp object
CCAN_IXXAT_VCIApp theApp;


//
//    If this DLL is dynamically linked against the MFC DLLs,
//    any functions exported from this DLL which call into
//    MFC must have the AFX_MANAGE_STATE macro added at the
//    very beginning of the function.
//
//    For example:
//
//    extern "C" BOOL PASCAL EXPORT ExportedFunction()
//    {
//      AFX_MANAGE_STATE(AfxGetStaticModuleState());
//      // normal function body here
//    }
//
//    It is very important that this macro appear in each
//    function, prior to any calls into MFC.  This means that
//    it must appear as the first statement within the
//    function, even before any object variable declarations
//    as their constructors may generate calls into the MFC
//    DLL.
//
//    Please see MFC Technical Notes 33 and 58 for additional
//    details.
//

// CCAN_IXXAT_VCIApp

BEGIN_MESSAGE_MAP(CCAN_IXXAT_VCIApp, CWinApp)
END_MESSAGE_MAP()


/**
 * @fn  CCAN_IXXAT_VCIApp::CCAN_IXXAT_VCIApp()
 *
 * @brief
 *  Default constructor.
 *
 */
CCAN_IXXAT_VCIApp::CCAN_IXXAT_VCIApp()
{
    // Place all significant initialization in InitInstance
}


/**
 * @brief Initializes the instance.
 *
 * @return
 *  true if it succeeds, false if it fails.
 *
 */
BOOL CCAN_IXXAT_VCIApp::InitInstance()
{
    CWinApp::InitInstance();

    return TRUE;
}


/**
 * @brief
 *  Called when the DLL should be closed.
 *  The g_pouDIL_CAN_IXXAT_VCI will be deleted here.
 *
 * @return
 *  Always 0.
 *
 */
int CCAN_IXXAT_VCIApp::ExitInstance( )
{
    if (NULL != g_pouDIL_CAN_IXXAT_VCI)
    {

        delete g_pouDIL_CAN_IXXAT_VCI;
        g_pouDIL_CAN_IXXAT_VCI = NULL;
    }

#ifdef _IXXAT_DEBUG
    // let it remarked: it shows false memory leaks
    // _CrtDumpMemoryLeaks();
#endif
    return 0;
}


/**
* \brief         Returns the CDIL_CAN_IXXAT_VCI object
* \param[out]    ppvInterface, is void pointer to take back the reference to CDIL_CAN_IXXAT_VCI object
* \return        S_OK for success, S_FALSE for failure
* \authors       Copied from Arunkumar Karri / updated by Peter Wucherer
* \date          07.10.2011 Created / 17.03.2012 Updated
*/
USAGEMODE HRESULT GetIDIL_CAN_Controller(void** ppvInterface)
//HRESULT __stdcall GetIDIL_CAN_Controller(void** ppvInterface)
{
    CoInitialize(NULL);

    HRESULT hResult = S_OK;
    if ( NULL == g_pouDIL_CAN_IXXAT_VCI )
    {
        if ((g_pouDIL_CAN_IXXAT_VCI = new CDIL_CAN_IXXAT_VCI) == NULL)
        {
            hResult = S_FALSE;
        }
    }
    *ppvInterface = (void*) g_pouDIL_CAN_IXXAT_VCI;  /* Doesn't matter even if g_pouDIL_CAN_IXXAT_VCI is null */

    return hResult;
}

