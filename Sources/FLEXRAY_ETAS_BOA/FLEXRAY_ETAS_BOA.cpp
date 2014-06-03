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
 * \file      CAN_Vector_XL.cpp
 * \brief     Source file for Vector XL DIL functions
 * \author    Tobias Lorenz, Arunkumar Karri
 * \copyright Copyright (c) 2011, ETAS GmbH. All rights reserved.
 *
 * Source file for Vector XL DIL functions
 */
// CAN_Vector_XL.cpp : Defines the initialization routines for the DLL.
//

/* C++ includes */

#include "FLEXRAY_ETAS_BOA_stdafx.h"
#include "FLEXRAY_ETAS_BOA_Defs.h"
#include "include/Error.h"

/* C++ includes */
#include <sstream>
#include <string>
#include <vector>

/* Project includes */
#include "FLEXRAY_ETAS_BOA.h"
#include "DataTypes/Base_WrapperErrorLogger.h"
#include "DataTypes/DIL_Datatypes.h"
#include "DataTypes/FLEXRAY_Datatypes.h"
#include "Utility/Utility_Thread.h"
#include "DataTypes/MsgBufAll_DataTypes.h"
#include "DIL_Interface/BaseDIL_FLEXRAY_Controller.h"
#include "Include/DIL_CommonDefs.h"
#include "DIL_Interface\ClientList.h"
#include "HardwareListing.h"
#include "DIL_Interface\CommanDIL_Flexray.h"

#define USAGE_EXPORT
#include "FLEXRAY_ETAS_BOA_Extern.h"

DWORD WINAPI DataTransmitThread(LPVOID pVoid);
void vInitializeQueueConfig(UINT nChannel);
BEGIN_MESSAGE_MAP(CFLEXRAY_ETAS_BOA, CWinApp)
END_MESSAGE_MAP()


/**
 * CCAN_Vector_XL construction
 */
CFLEXRAY_ETAS_BOA::CFLEXRAY_ETAS_BOA()
{
    // Place all significant initialization in InitInstance
}


// The one and only CCAN_Vector_XL object
CFLEXRAY_ETAS_BOA theApp;
const BYTE FILTER_ADD = 0x01;
const BYTE FILTER_REMOVE = 0x02;
const BYTE QUEUE_ADD = 0x01;
const BYTE QUEUE_DESTROY = 0x02;

/**
 * CCAN_Vector_XL initialization
 */

static HINSTANCE ghLangInst=nullptr;

BOOL CFLEXRAY_ETAS_BOA::InitInstance()
{
    CWinApp::InitInstance();

    return TRUE;
}

/**********************Global functions***********************************/

static void vInitialiseAllData(void)
{
    // Initialise both the time parameters
    INITIALISE_DATA(sg_CurrSysTime);
    sg_TimeStamp = 0x0;

    // INITIALISE_DATA(sg_sCurrStatus);
    //INITIALISE_DATA(sg_sBoardCfg);
    sg_acErrStr = "";
}

/**
 * Gets the CSI API function pointer from the cslproxy.dll
 */
BOOL bGetBOAInstallationPath(std::string& pcPath)
{
    USES_CONVERSION;

    BOOL bResult = FALSE;
    LONG lError = 0;
    HKEY sKey;
    // Get the installation path for BOA 1.4
    lError = RegOpenKeyEx( HKEY_LOCAL_MACHINE, BOA_REGISTRY_LOCATION, 0, KEY_READ, &sKey);
    // If the registry key open successfully, get the value in "path"
    // sub key
    if(lError==ERROR_SUCCESS)
    {
        ULONG ulType = REG_SZ;
        BYTE acGCCPath[1024];
        DWORD dwSize = sizeof(acGCCPath);
        lError = RegQueryValueEx(sKey, "path", 0, &ulType, acGCCPath, &dwSize);
        pcPath = A2T((char*)acGCCPath);
        RegCloseKey(sKey);
        bResult = TRUE;
    }
    return bResult;
}

/**
 * Gets the CSI API function pointer from the cslproxy.dll
 */
HRESULT GetCSI_API_Pointers(HMODULE hLibCSI)
{
    HRESULT hResult = S_OK;
    if (hLibCSI != nullptr)
    {
        if ((sBOA_PTRS.m_sCSI.createProtocolTree        = (PROC1)GetProcAddress(hLibCSI, "CSI_CreateProtocolTree")) == nullptr)
        {
            hResult = S_FALSE;
        }
        else if ((sBOA_PTRS.m_sCSI.destroyProtocolTree  = (PROC2)GetProcAddress(hLibCSI, "CSI_DestroyProtocolTree")) == nullptr)
        {
            hResult = S_FALSE;
        }
        else if ((sBOA_PTRS.m_sCSI.getUriForUuid        = (PROC3)GetProcAddress(hLibCSI, "CSI_GetUriForUuid")) == nullptr)
        {
            hResult = S_FALSE;
        }
    }
    return hResult;
}

/**
 * Gets the OCI API function pointer from the ocdproxy.dll
 */
HRESULT GetOCI_API_Pointers(HMODULE hLibOCI)
{
    HRESULT hResult = S_OK;
    if (hLibOCI != nullptr)
    {
#if BOA_VERSION >= BOA_VERSION_2_0
        if ((sBOA_PTRS.createFlexRayController  = (PF_OCI_CreateFlexRayControllerVersion)
                GetProcAddress(hLibOCI, "OCI_CreateFlexRayControllerVersion")) == nullptr)
        {
            hResult = S_FALSE;
        }
#else
        if ((sBOA_PTRS.m_sOCI.createFlexRayController = (PF_OCI_CreateFlexRayController)
                GetProcAddress(hLibOCI, "OCI_CreateFlexRayController")) == nullptr)
        {
            hResult = S_FALSE;
        }
#endif
        if ((sBOA_PTRS.m_sOCI.destroyFlexRayController = (PF_OCI_DestroyFlexRayController )
                GetProcAddress(hLibOCI, "OCI_DestroyFlexRayController")) == nullptr)
        {
            hResult = S_FALSE;
        }
        if ((sBOA_PTRS.m_sOCI.openFlexRayControllerEx = (PF_OCI_OpenFlexRayControllerEx)
                GetProcAddress(hLibOCI, "OCI_OpenFlexRayControllerEx")) == nullptr)
        {
            hResult = S_FALSE;
        }

        if ((sBOA_PTRS.m_sOCI.closeFlexRayController = (PF_OCI_CloseFlexRayController)
                GetProcAddress(hLibOCI, "OCI_CloseFlexRayController")) == nullptr)
        {
            hResult = S_FALSE;
        }
        if ((sBOA_PTRS.m_sOCI.getFlexRayConfiguration = (PF_OCI_GetFlexRayConfiguration)
                GetProcAddress(hLibOCI, "OCI_GetFlexRayConfiguration")) == nullptr)
        {
            hResult = S_FALSE;
        }

        if ((sBOA_PTRS.m_sOCI.getFlexRayControllerProperties = (PF_OCI_GetFlexRayControllerProperties)
                GetProcAddress(hLibOCI, "OCI_GetFlexRayControllerProperties")) == nullptr)
        {
            hResult = S_FALSE;
        }
        if ((sBOA_PTRS.m_sOCI.setFlexRayControllerProperties = (PF_OCI_SetFlexRayControllerProperties)
                GetProcAddress(hLibOCI, "OCI_SetFlexRayControllerProperties")) == nullptr)
        {
            hResult = S_FALSE;
        }

        if ((sBOA_PTRS.m_sOCI.flexRayIOVTable.getFlexRayControllerCapabilities = (PF_OCI_GetFlexRayControllerCapabilities)
                GetProcAddress(hLibOCI, "OCI_GetFlexRayControllerCapabilities")) == nullptr)
        {
            hResult = S_FALSE;
        }
        else if ((sBOA_PTRS.m_sOCI.flexRayIOVTable.getFlexRayControllerStatus = (PF_OCI_GetFlexRayControllerStatus)
                  GetProcAddress(hLibOCI, "OCI_GetCANControllerStatus")) == nullptr)
        {
            hResult = S_FALSE;
        }

        else if ((sBOA_PTRS.m_sOCI.flexRayIOVTable.createFlexRayTxQueue = (PF_OCI_CreateFlexRayTxQueue)
                  GetProcAddress(hLibOCI, "OCI_CreateFlexRayTxQueue")) == nullptr)
        {
            hResult = S_FALSE;
        }
        else if ((sBOA_PTRS.m_sOCI.flexRayIOVTable.writeFlexRayData = (PF_OCI_WriteFlexRayData)
                  GetProcAddress(hLibOCI, "OCI_WriteFlexRayData")) == nullptr)
        {
            hResult = S_FALSE;
        }
        else if ((sBOA_PTRS.m_sOCI.flexRayIOVTable.destroyFlexRayTxQueue = (PF_OCI_DestroyFlexRayTxQueue)
                  GetProcAddress(hLibOCI, "OCI_DestroyFlexRayTxQueue")) == nullptr)
        {
            hResult = S_FALSE;
        }

        else if ((sBOA_PTRS.m_sOCI.flexRayIOVTable.createFlexRayRxQueue = (PF_OCI_CreateFlexRayRxQueue)
                  GetProcAddress(hLibOCI, "OCI_CreateFlexRayRxQueue")) == nullptr)
        {
            hResult = S_FALSE;
        }
        else if ((sBOA_PTRS.m_sOCI.flexRayIOVTable.readFlexRayData = (PF_OCI_ReadFlexRayData)
                  GetProcAddress(hLibOCI, "OCI_ReadFlexRayData")) == nullptr)
        {
            hResult = S_FALSE;
        }
        else if ((sBOA_PTRS.m_sOCI.flexRayIOVTable.destroyFlexRayRxQueue= (PF_OCI_DestroyFlexRayRxQueue)
                  GetProcAddress(hLibOCI, "OCI_DestroyFlexRayRxQueue")) == nullptr)
        {
            hResult = S_FALSE;
        }

        else if ((sBOA_PTRS.m_sOCI.flexRayIOVTable.addFlexRayFrameFilter = (PF_OCI_AddFlexRayFrameFilter)
                  GetProcAddress(hLibOCI, "OCI_AddFlexRayFrameFilter")) == nullptr)
        {
            hResult = S_FALSE;
        }
        else if ((sBOA_PTRS.m_sOCI.flexRayIOVTable.removeFlexRayFrameFilter = (PF_OCI_RemoveFlexRayFrameFilter)
                  GetProcAddress(hLibOCI, "OCI_RemoveFlexRayFrameFilter")) == nullptr)
        {
            hResult = S_FALSE;
        }
        else if ((sBOA_PTRS.m_sOCI.flexRayIOVTable.addFlexRayEventFilter = (PF_OCI_AddFlexRayEventFilter)
                  GetProcAddress(hLibOCI, "OCI_AddFlexRayEventFilter")) == nullptr)
        {
            hResult = S_FALSE;
        }
        else if ((sBOA_PTRS.m_sOCI.flexRayIOVTable.removeFlexRayEventFilter = (PF_OCI_RemoveFlexRayEventFilter)
                  GetProcAddress(hLibOCI, "OCI_RemoveCANBusEventFilter")) == nullptr)
        {
            hResult = S_FALSE;
        }
        //else if ((sBOA_PTRS.m_sOCI.flexRayIOVTable.addFlexRaye = (PF_OCI_AddCANErrorFrameFilter)
        //          GetProcAddress(hLibOCI, "OCI_AddCANErrorFrameFilter")) == nullptr)
        //{
        //    hResult = S_FALSE;
        //}
        //else if ((sBOA_PTRS.m_sOCI.flexRayIOVTable.removeCANErrorFrameFilter = (PF_OCI_RemoveCANErrorFrameFilter)
        //          GetProcAddress(hLibOCI, "OCI_RemoveCANErrorFrameFilter")) == nullptr)
        //{
        //    hResult = S_FALSE;
        //}
        else if ((sBOA_PTRS.m_sOCI.timeVTable.getTimerStatus = (PF_OCI_GetTimerStatus)
                  GetProcAddress(hLibOCI, "OCI_GetTimerStatus")) == nullptr)
        {
            hResult = S_FALSE;
        }
        else if ((sBOA_PTRS.m_sOCI.timeVTable.getTimerStatus = (PF_OCI_GetTimerStatus)
                  GetProcAddress(hLibOCI, "OCI_GetTimerStatus")) == nullptr)
        {
            hResult = S_FALSE;
        }
        else if ((sBOA_PTRS.m_sOCI.timeVTable.getTimerCapabilities = (PF_OCI_GetTimerCapabilities)
                  GetProcAddress(hLibOCI, "OCI_GetTimerCapabilities")) == nullptr)
        {
            hResult = S_FALSE;
        }
        else if ((sBOA_PTRS.m_sOCI.timeVTable.getTimerValue = (PF_OCI_GetTimerValue)
                  GetProcAddress(hLibOCI, "OCI_GetTimerValue")) == nullptr)
        {
            hResult = S_FALSE;
        }
        else if ((sBOA_PTRS.m_sOCI.timeVTable.addTimerEventFilter = (PF_OCI_AddTimerEventFilter)
                  GetProcAddress(hLibOCI, "OCI_AddTimerEventFilter")) == nullptr)
        {
            hResult = S_FALSE;
        }
        else if ((sBOA_PTRS.m_sOCI.timeVTable.removeTimerEventFilter = (PF_OCI_RemoveTimerEventFilter)
                  GetProcAddress(hLibOCI, "OCI_RemoveTimerEventFilter")) == nullptr)
        {
            hResult = S_FALSE;
        }
        else if ((sBOA_PTRS.m_sOCI.errorVTable.getError = (PF_OCI_GetError)
                  GetProcAddress(hLibOCI, "OCI_GetError")) == nullptr)
        {
            hResult = S_FALSE;
        }
        else if ((sBOA_PTRS.m_sOCI.errorVTable.addInternalErrorEventFilter = (PF_OCI_AddInternalErrorEventFilter)
                  GetProcAddress(hLibOCI, "OCI_AddInternalErrorEventFilter")) == nullptr)
        {
            hResult = S_FALSE;
        }
        else if ((sBOA_PTRS.m_sOCI.errorVTable.removeInternalErrorEventFilter = (PF_OCI_RemoveInternalErrorEventFilter)
                  GetProcAddress(hLibOCI, "OCI_RemoveInternalErrorEventFilter")) == nullptr)
        {
            hResult = S_FALSE;
        }
    }
    return hResult;
}

/**
 *  Recursively scans a tree of CSI nodes created by CSI_CreateProtocolTree() and copies the URI-Paths
 *  for all devces that support FLX.
 *
 *  @param[in]  sfsTree     Root of the tree to scan
 *  @param[in]  uriPrefix   The prefix which must be added to the URI name of the root of the tree in order to create
 *                          a complete URI.
 *  @param[out] uriNames    The array which will be filled with the URI locations of all the hardware
 *                          devices that support the OCI FLX interface.
 *  @param[in]  size        The size of the <b>uriNames</b> array.
 *  @param[out] position    The current entry in the <b>uriNames</b> array.
 */
void findFlxNodes( CSI_Tree* sfsTree, OCI_URIName uriPrefix, OCI_URIName uriNames[], uint32 size, uint32* position )
{
    /* Uncomment the next line to get a view of the items in the tree */
    /* printf( "uriPrefix is %s; node is %s\n", uriPrefix, sfsTree->item.uriNames ); */

    /* Basic error checking */
    if ( !sfsTree || !uriNames || !uriPrefix || !position )
    {
        return;
        //exitWithMessage( "ERROR: parameter is nullptr", OCI_ERR_UNEXPECTED_NULL );
    }

    /* Does the current tree node have the URI name which begins with "FLX:"?
     * (Each node which represents a FLX port always has a URI name of the form "FLX:n". */
    if( 0 == strncmp( sfsTree->item.uriName, "FLX:", 4 ) )
    {
        if (*position < size)
        {
            strcpy( uriNames[ *position ], uriPrefix );
            strcat( uriNames[ *position ], "/" );
            strcat( uriNames[ *position ], sfsTree->item.uriName );
            (*position)++;
        }
        else
        {
            return;
            // exitWithMessage( "ERROR: Not enough space to list all connected devices.\n", OCI_ERR_OUT_OF_MEMORY );
        }
    }

    /* If the current tree node has a child, recurse into it */
    if (sfsTree->child)
    {
        OCI_URIName newUriPrefix;

        strcpy( newUriPrefix, uriPrefix );
        strcat( newUriPrefix, "/" );
        strcat( newUriPrefix, sfsTree->item.uriName );
        findFlxNodes( sfsTree->child, newUriPrefix, uriNames, size, position );
    }
    /* If the current tree node has a sibling, recurse into it */
    if (sfsTree->sibling)
    {
        findFlxNodes( sfsTree->sibling, uriPrefix, uriNames, size, position );
    }
}

/**
 * Search for all connected Hardware, that supports the OCI
 * FLEXRAY interface and deliver the URI location of the hardware.
 */
BOA_ResultCode OCI_FindFlexRayController(OCI_URIName uriNames[], INT nSize, uint32* nFound)
{
    //OCI_ErrorCode ec;

    ///* Container for search results */
    //CSI_Tree* sfsTree = nullptr;
    ///* Specify that we want to search for nodes which implement v1.1.0.0 of OCI_FLX. */
    //static const BOA_UuidVersion ociFlexRayUuid = { UUID_OCIFLX, {1,1,0,0} };

    ///* Specify that we want to search for any kind of node, not just hardware nodes */
    //const CSI_NodeRange nodeRange = {CSI_NODE_MIN, CSI_NODE_MAX};

    ///* Search for all connected hardware and latch the result for further processing */
    //ec = (*(sBOA_PTRS.m_sCSI.createProtocolTree))("", nodeRange, &sfsTree);
    //if (ec == OCI_SUCCESS)
    //{
    //    /* Find the URIs for all nodes which implement v1.1.0.0 of OCI_CAN. */
    //    ec = (*(sBOA_PTRS.m_sCSI.getUriForUuid))(sfsTree, &ociFlexRayUuid, uriName, nSize, nFound);
    //    if (ec == OCI_SUCCESS)
    //    {
    //        ec = (*(sBOA_PTRS.m_sCSI.destroyProtocolTree))(sfsTree);
    //    }
    //}

    //return ec;

    OCI_ErrorCode   ec;

    /* Container for search results */
    CSI_Tree* sfsTree = nullptr;

    /* Specify that we want to search for physical hardware nodes */
    const CSI_NodeRange nodeRange = {CSI_NODE_MIN_PHYSICAL_NODE , CSI_NODE_MAX_PHYSICAL_NODE };

    OCI_URIName uriPrefix = "ETAS:/";   /* The prefix of the URI of the root of the device tree */

    /* parameter validation to avoid possible crash. */
    if ( !nFound )
    {
        return OCI_ERR_UNEXPECTED_NULL;
        //exitWithMessage( "ERROR: parameter \'found\' is nullptr", OCI_ERR_UNEXPECTED_NULL );
    }

    /* Search for all connected hardware and latch the result for further processing */
    ec = (*(sBOA_PTRS.m_sCSI.createProtocolTree))("", nodeRange, &sfsTree);

    if ( OCI_FAILED( ec ) )
    {
        return ec;
        //exitWithMessage( "CSI_CreateProtocolTree failed! Error Code: 0x%x\n", ec );
    }

    /* Search the tree and fill array with the results */
    *nFound = 0;
    findFlxNodes( sfsTree, uriPrefix, uriNames, nSize, nFound );

    /* Clean up the protocol tree. */
    ec = (*(sBOA_PTRS.m_sCSI.destroyProtocolTree))(sfsTree);

    if ( OCI_FAILED( ec ) )
    {
        return ec;
        //exitWithMessage( "CSI_DestroyProtocolTree failed! Error Code: 0x%x\n", ec );
    }

    return ec;
}

/**
 * \return Operation Result. 0 incase of no errors. Failure Error codes otherwise.
 *
 * This function will popup hardware selection dialog and gets the user selection of channels.
 *
 */
int ListHardwareInterfaces(HWND hParent, DWORD /*dwDriver*/, INTERFACE_HW* psInterfaces, int* pnSelList, int& nCount)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());

    CWnd objMainWnd;
    objMainWnd.Attach(hParent);
    CHardwareListing HwList(psInterfaces, nCount, pnSelList, &objMainWnd);
    INT nRet = HwList.DoModal();
    objMainWnd.Detach();

    if ( nRet == IDOK)
    {
        nCount = HwList.nGetSelectedList(pnSelList);
        return 0;
    }
    else
    {
        return -1;
    }
}

/**
 * CallBack function used by the qsort Function
**/

INT nCallBackStrCompareFn( const void* str1, const void* str2)
{
    return( strcmp((char*)str1,(char*)str2) );
}


/**
 * \return S_OK for success, S_FALSE for failure
 *
 * Adds or removes the queue to/from the channel.
 */
HRESULT ManageQueue(BYTE byCode, UINT nChannel)
{
    HRESULT hResult = S_FALSE;
    BOA_ResultCode Err = OCI_ERR_FLAG_ERROR;
    if (byCode == QUEUE_ADD)
    {

        //Create CAN Rx queue
        vInitializeQueueConfig(nChannel);
        Err = (*(sBOA_PTRS.m_sOCI.flexRayIOVTable.createFlexRayRxQueue))
              (sg_asChannel[nChannel].m_OCI_HwHandle,
               &(sg_asChannel[nChannel].m_OCI_RxQueueCfg),
               &(sg_asChannel[nChannel].m_OCI_RxQueueHandle));
        if (Err >= OCI_FAILURE)
        {
            sg_hLastError = Err;

            hResult = S_FALSE;
            sg_pIlog->vLogAMessage(A2T(__FILE__), __LINE__, _("could not create Rx Queue"));
        }
        else
        {
            hResult = S_OK;
        }
        return hResult;
        //Create CAN Tx queue
        if (hResult == S_OK)
        {
            sg_asChannel[nChannel].m_OCI_TxQueueCfg.count = sg_asChannel[nChannel].m_unMsgConfig;
            for ( int i = 0 ; i < sg_asChannel[nChannel].m_unMsgConfig; i++ )
            {
                sg_asChannel[nChannel].m_OCI_TxQueueCfg.bufferIndexes[i] = i;
            }


            OCI_ErrorCode Err = (*(sBOA_PTRS.m_sOCI.flexRayIOVTable.createFlexRayTxQueue))
                                (sg_asChannel[nChannel].m_OCI_HwHandle,
                                 &(sg_asChannel[nChannel].m_OCI_TxQueueCfg),
                                 &(sg_asChannel[nChannel].m_OCI_TxQueueHandle));
            /*if ( Err != OCI_SUCCESS )
            {
                int i = 0 ;
            }*/

            if (Err >= BOA_RESULT_FLAG_ERROR )
            {
                sg_hLastError = Err;

                hResult = S_FALSE;
                sg_pIlog->vLogAMessage(A2T(__FILE__), __LINE__, _("could not create TX Queue"));
            }
        }
    }
    else if (byCode == QUEUE_DESTROY)
    {
        //Destroy CAN Rx queue
        Err = (*(sBOA_PTRS.m_sOCI.flexRayIOVTable.destroyFlexRayRxQueue))(sg_asChannel[nChannel].m_OCI_RxQueueHandle);
        if (Err == OCI_SUCCESS)
        {
            hResult = S_OK;
        }
        else
        {
            sg_hLastError = Err;

            hResult = S_FALSE;
            sg_pIlog->vLogAMessage(A2T(__FILE__), __LINE__, _("could not create Rx Queue"));
        }
        //Create CAN Tx queue
        if (hResult == S_OK)
        {
            Err = (*(sBOA_PTRS.m_sOCI.flexRayIOVTable.destroyFlexRayTxQueue))(sg_asChannel[nChannel].m_OCI_TxQueueHandle);
            if ( OCI_FAILED( Err ) )
            {
                sg_hLastError = Err;

                //hResult = S_FALSE;
                sg_pIlog->vLogAMessage(A2T(__FILE__), __LINE__, _("could not create TX Queue"));
            }
        }
    }
    return hResult;
}

/**
 * \return S_OK for success, S_FALSE for failure
 *
 * Adds or removes the HW filter to/from the channel.
 */
HRESULT ManageFilters(BYTE byCode, UINT nChannel)
{
    HRESULT hResult = S_FALSE;
    BOA_ResultCode ErrCode = OCI_FAILURE;
    if (byCode == FILTER_ADD)
    {
        // Add Frame filter
        ErrCode = (*(sBOA_PTRS.m_sOCI.flexRayIOVTable.addFlexRayFrameFilter))
                  (sg_asChannel[nChannel].m_OCI_RxQueueHandle,
                   &(sg_asChannel[nChannel].m_OCI_FrameFilter), 1);
        if (ErrCode == OCI_SUCCESS)
        {
            hResult = S_OK;
        }
        else
        {
            sg_hLastError = ErrCode;

            hResult = S_FALSE;
            sg_pIlog->vLogAMessage(A2T(__FILE__), __LINE__, _("could not add frame filter"));
        }

        // Add Event filter
        if (hResult == S_OK)
        {
            ErrCode = (*(sBOA_PTRS.m_sOCI.flexRayIOVTable.addFlexRayEventFilter))
                      (sg_asChannel[nChannel].m_OCI_RxQueueHandle,
                       &(sg_asChannel[nChannel].m_OCI_EventFilter), 1);

            if (ErrCode != OCI_SUCCESS)
            {
                sg_hLastError = ErrCode;

                hResult = S_FALSE;
                sg_pIlog->vLogAMessage(A2T(__FILE__), __LINE__, _("could not add Event filter"));
            }
        }

        // Add internal error filter
        if (hResult == S_OK)
        {
            ErrCode = (*(sBOA_PTRS.m_sOCI.errorVTable.addInternalErrorEventFilter))
                      (sg_asChannel[nChannel].m_OCI_RxQueueHandle,
                       &(sg_asChannel[nChannel].m_OCI_InternalErrorFilter), 1);
            if (ErrCode != OCI_SUCCESS)
            {
                sg_hLastError = ErrCode;

                hResult = S_FALSE;
                sg_pIlog->vLogAMessage(A2T(__FILE__), __LINE__, _("could not add Error filter"));
            }
        }

    }
    else if (byCode == FILTER_REMOVE)
    {
        // Remove Frame filter
        ErrCode = (*(sBOA_PTRS.m_sOCI.flexRayIOVTable.removeFlexRayFrameFilter))
                  (sg_asChannel[nChannel].m_OCI_RxQueueHandle,
                   &(sg_asChannel[nChannel].m_OCI_FrameFilter), 1);
        if (ErrCode == OCI_SUCCESS)
        {
            hResult = S_OK;
        }
        else
        {
            sg_hLastError = ErrCode;

            hResult = S_FALSE;
            sg_pIlog->vLogAMessage(A2T(__FILE__), __LINE__, _("could not remove frame filter"));
        }

        // Remove Event filter
        if (hResult == S_OK)
        {
            ErrCode = (*(sBOA_PTRS.m_sOCI.flexRayIOVTable.removeFlexRayEventFilter))
                      (sg_asChannel[nChannel].m_OCI_RxQueueHandle,
                       &(sg_asChannel[nChannel].m_OCI_EventFilter), 1);
            if ( OCI_FAILED( ErrCode ) )
            {
                sg_hLastError = ErrCode;

                //hResult = S_FALSE;
                sg_pIlog->vLogAMessage(A2T(__FILE__), __LINE__, _("could not remove Event filter"));
            }
        }

        if (hResult == S_OK)
        {
            ErrCode = (*(sBOA_PTRS.m_sOCI.errorVTable.removeInternalErrorEventFilter))
                      (sg_asChannel[nChannel].m_OCI_RxQueueHandle,
                       &(sg_asChannel[nChannel].m_OCI_InternalErrorFilter), 1);
            if ( OCI_FAILED( ErrCode ) )
            {
                sg_hLastError = ErrCode;

                //hResult = S_FALSE;
                sg_pIlog->vLogAMessage(A2T(__FILE__), __LINE__, _("could not add Error filter"));
            }
        }
    }
    return hResult;
}


/**
 * \return TRUE for success, FALSE for failure
 *
 * Copies the controller config values into channel's
 * controller config structure.
 */
OCI_FlexRayBusConfiguration     g_ouFlxBusConfiguration;

struct FRAME_STRUCT_SORT
{
    std::string m_strFrameId;
    std::string m_strFrameName;
    int m_nSlotId;
    int m_nBaseCycle;
    int m_nReptition;
    int m_nLength;
    //bool m_bConsiderPdu;
    ECHANNEL    m_ouChannel;
    ESLOT_TYPE m_eSlotType;
    ESYNC m_eSync;

    bool operator == (FRAME_STRUCT& ob1)
    {
        bool bResult = false;

        if((m_strFrameId == ob1.m_strFrameId) && (m_nSlotId == ob1.m_nSlotId) && (m_nBaseCycle == ob1.m_nBaseCycle))
        {
            bResult = true;
        }

        return bResult;
    }
    bool operator < (FRAME_STRUCT_SORT& ob1)
    {
        return (m_nSlotId < ob1.m_nSlotId);
    }

    bool operator > (FRAME_STRUCT_SORT& ob1)
    {
        return (m_nSlotId > ob1.m_nSlotId);
    }
};

static BOOL bLoadDataFromContr(CHANNEL_CONFIG&  asDeviceConfig)
{
    BOOL bReturn = FALSE;
    // If successful
    INT nChannel = 0;
    ABS_FLEXRAY_CLUSTER ouCluster;
    BOOL bPayloadPreambleIndicator;
    //1. Cluster Configuration
    asDeviceConfig.m_ouClusterInfo.GetClusterInfo(ouCluster);
    sg_asChannel[nChannel].m_OCI_FlexRayConfig.bus.gColdStartAttempts                   = (uint32)ouCluster.m_shCOLD_START_ATTEMPTS;
    sg_asChannel[nChannel].m_OCI_FlexRayConfig.bus.gdActionPointOffset                  = (uint32)ouCluster.m_shACTION_POINT_OFFSET;
    sg_asChannel[nChannel].m_OCI_FlexRayConfig.bus.gdCASRxLowMax                        = (uint32)ouCluster.m_shCAS_RX_LOW_MAX;
    sg_asChannel[nChannel].m_OCI_FlexRayConfig.bus.gdDynamicSlotIdlePhase               = (uint32)ouCluster.m_shDYNAMIC_SLOT_IDLE_PHASE;
    sg_asChannel[nChannel].m_OCI_FlexRayConfig.bus.gdMinislot                           = (uint32)ouCluster.m_shMINISLOT;
    sg_asChannel[nChannel].m_OCI_FlexRayConfig.bus.gdMinislotActionPointOffset          = (uint32)ouCluster.m_shMINISLOT_ACTION_POINT_OFFSET;
    sg_asChannel[nChannel].m_OCI_FlexRayConfig.bus.gdStaticSlot                         = (uint32)ouCluster.m_shSTATIC_SLOT;
    sg_asChannel[nChannel].m_OCI_FlexRayConfig.bus.gdSymbolWindow                       = (uint32)ouCluster.m_shSYMBOL_WINDOW;
    sg_asChannel[nChannel].m_OCI_FlexRayConfig.bus.gdTSSTransmitter                     = (uint32)ouCluster.m_shTSS_TRANSMITTER;
    sg_asChannel[nChannel].m_OCI_FlexRayConfig.bus.gdWakeupSymbolRxIdle                 = (uint32)ouCluster.m_ouWAKEUP.m_shWAKE_UP_SYMBOL_RX_IDLE;
    sg_asChannel[nChannel].m_OCI_FlexRayConfig.bus.gdWakeupSymbolRxLow                  = (uint32)ouCluster.m_ouWAKEUP.m_shWAKE_UP_SYMBOL_RX_LOW;
    sg_asChannel[nChannel].m_OCI_FlexRayConfig.bus.gdWakeupSymbolRxWindow               = (uint32)ouCluster.m_ouWAKEUP.m_shWAKE_UP_SYMBOL_RX_WINDOW;
    sg_asChannel[nChannel].m_OCI_FlexRayConfig.bus.gdWakeupSymbolTxIdle                 = (uint32)ouCluster.m_ouWAKEUP.m_shWAKE_UP_SYMBOL_TX_IDLE;
    sg_asChannel[nChannel].m_OCI_FlexRayConfig.bus.gdWakeupSymbolTxLow                  = (uint32)ouCluster.m_ouWAKEUP.m_shWAKE_UP_SYMBOL_TX_LOW;
    sg_asChannel[nChannel].m_OCI_FlexRayConfig.bus.gListenNoise                         = (uint32)ouCluster.m_shLISTEN_NOISE;
    sg_asChannel[nChannel].m_OCI_FlexRayConfig.bus.gMacroPerCycle                       = (uint32)ouCluster.m_shMACRO_PER_CYCLE;
    sg_asChannel[nChannel].m_OCI_FlexRayConfig.bus.gMaxWithoutClockCorrectionFatal      = (uint32)ouCluster.m_shMAX_WITHOUT_CLOCK_CORRECTION_FATAL;
    sg_asChannel[nChannel].m_OCI_FlexRayConfig.bus.gMaxWithoutClockCorrectionPassive    = (uint32)ouCluster.m_shMAX_WITHOUT_CLOCK_CORRECTION_PASSIVE;
    sg_asChannel[nChannel].m_OCI_FlexRayConfig.bus.gNetworkManagementVectorLength       = (uint32)ouCluster.m_shNETWORK_MANAGEMENT_VECTOR_LENGTH;
    sg_asChannel[nChannel].m_OCI_FlexRayConfig.bus.gNumberOfMinislots                   = (uint32)ouCluster.m_shNUMBER_OF_MINISLOTS;
    sg_asChannel[nChannel].m_OCI_FlexRayConfig.bus.gNumberOfStaticSlots                 = (uint32)ouCluster.m_shNUMBER_OF_STATIC_SLOTS;
    sg_asChannel[nChannel].m_OCI_FlexRayConfig.bus.gOffsetCorrectionStart               = (uint32)ouCluster.m_shOFFSET_CORRECTION_START;
    sg_asChannel[nChannel].m_OCI_FlexRayConfig.bus.gPayloadLengthStatic                 = (uint32)ouCluster.m_shPAYLOAD_LENGTH_STATIC;
    sg_asChannel[nChannel].m_OCI_FlexRayConfig.bus.gSyncNodeMax                         = (uint32)ouCluster.m_shSYNC_NODE_MAX;

    sg_asChannel[nChannel].m_unCycleTime = ouCluster.m_shCYCLE;

    bPayloadPreambleIndicator = ( ouCluster.m_shNETWORK_MANAGEMENT_VECTOR_LENGTH > 0 ) ? true : false;

    //2.Take First ECU Controller Parameters for Controller Settings
    std::list<ECU_Struct> ouEcuList;
    asDeviceConfig.m_ouClusterInfo.GetECUList(ouEcuList);
    std::list<ECU_Struct>::iterator itrEcu = ouEcuList.begin();
    ABS_FLEXRAY_SPEC_CNTLR ouControllerData;
    if ( itrEcu!= ouEcuList.end() )
    {
        //asDeviceConfig.m_ouClusterInfo.GetECU(*itrEcu, ouEcu);
        itrEcu->GetControllerParams(ouControllerData);
        sg_asChannel[nChannel].m_OCI_FlexRayConfig.bus.pAllowHaltDueToClock     = (uint32)ouControllerData.m_bAllowHaltDewToClock;
        sg_asChannel[nChannel].m_OCI_FlexRayConfig.bus.pAllowPassiveToActive    = (uint32)ouControllerData.m_shAllowPassiveToActive;
        sg_asChannel[nChannel].m_OCI_FlexRayConfig.bus.pChannels                = OCI_FLEXRAY_CHANNEL_A | OCI_FLEXRAY_CHANNEL_B; ;//(uint32)ouControllerData.m_nChannels;
        sg_asChannel[nChannel].m_OCI_FlexRayConfig.bus.pClusterDriftDamping     = (uint32)ouControllerData.m_shClusterDriftDamping;
        sg_asChannel[nChannel].m_OCI_FlexRayConfig.bus.pdAcceptedStartupRange   = (uint32)ouControllerData.m_shAcceptedStartUpRange;
        sg_asChannel[nChannel].m_OCI_FlexRayConfig.bus.pDecodingCorrection      = (uint32)ouControllerData.m_nDecodingCorr;
        sg_asChannel[nChannel].m_OCI_FlexRayConfig.bus.pDelayCompensationA      = (uint32)ouControllerData.m_shDelayCompensationA;
        sg_asChannel[nChannel].m_OCI_FlexRayConfig.bus.pDelayCompensationB      = (uint32)ouControllerData.m_shDelayCompensationB;
        sg_asChannel[nChannel].m_OCI_FlexRayConfig.bus.pdListenTimeout          = (uint32)ouControllerData.m_nListenTimeOut;
        sg_asChannel[nChannel].m_OCI_FlexRayConfig.bus.pdMaxDrift               = (uint32)ouControllerData.m_shMaxDrift;
        sg_asChannel[nChannel].m_OCI_FlexRayConfig.bus.pExternOffsetCorrection  = (uint32)ouControllerData.m_shExternOffsetCorr;
        sg_asChannel[nChannel].m_OCI_FlexRayConfig.bus.pExternRateCorrection    = (uint32)ouControllerData.m_shExternRateCorr;

        //For Debugging
        sg_asChannel[nChannel].m_OCI_FlexRayConfig.bus.pKeySlotId               = 0;//(uint32)asDeviceConfig.m_nKetSlot;
        sg_asChannel[nChannel].m_OCI_FlexRayConfig.bus.pSecondKeySlotId         = (uint32)asDeviceConfig.m_ouFlexRayParams.m_nSecondKeySlot;
        sg_asChannel[nChannel].m_OCI_FlexRayConfig.advanced.exclusiveAccess     = false;                                    // The client requests exclusive access to the controller.


        sg_asChannel[nChannel].m_OCI_FlexRayConfig.advanced.busMode = OCI_FLEXRAY_BUSMODE_NORMAL;
        if ( sg_asChannel[nChannel].m_ControllerCapabilities.supportedBusMode & OCI_FLEXRAY_BUSMODE_SELFRECEPTION )
        {
            sg_asChannel[nChannel].m_OCI_FlexRayConfig.advanced.busMode = OCI_FLEXRAY_BUSMODE_SELFRECEPTION;
        }

        //if (asDeviceConfig.m_nSecondKeySlot < 0 )
        //{
        //  sg_asChannel[nChannel].m_OCI_FlexRayConfig.advanced.busMode         = OCI_FLEXRAY_BUSMODE_NORMAL | OCI_FLEXRAY_BUSMODE_TWOKEYSLOTS;
        //}
        //

        sg_asChannel[nChannel].m_OCI_FlexRayConfig.bus.pKeySlotUsedForStartup   = 0;//(uint32)ouControllerData.m_sKeySlotUsage.m_nStartUpSync;
        sg_asChannel[nChannel].m_OCI_FlexRayConfig.bus.pKeySlotUsedForSync      = 0;//(uint32)ouControllerData.m_sKeySlotUsage.m_nSync;
        sg_asChannel[nChannel].m_OCI_FlexRayConfig.bus.pLatestTx                = (uint32)ouControllerData.m_shLatestTx;
        sg_asChannel[nChannel].m_OCI_FlexRayConfig.bus.pMacroInitialOffsetA     = (uint32)ouControllerData.m_shMacroInitialOffsetA;
        sg_asChannel[nChannel].m_OCI_FlexRayConfig.bus.pMacroInitialOffsetB     = (uint32)ouControllerData.m_shMacroInitialOffsetB;
        sg_asChannel[nChannel].m_OCI_FlexRayConfig.bus.pMicroInitialOffsetA     = (uint32)ouControllerData.m_shMicroInitialOffsetA;
        sg_asChannel[nChannel].m_OCI_FlexRayConfig.bus.pMicroInitialOffsetB     = (uint32)ouControllerData.m_shMicroInitialOffsetB;
        sg_asChannel[nChannel].m_OCI_FlexRayConfig.bus.pMicroPerCycle           = (uint32)ouControllerData.m_nMicroPreCycle;
        sg_asChannel[nChannel].m_OCI_FlexRayConfig.bus.pOffsetCorrectionOut     = (uint32)ouControllerData.m_shOffsetCorrOut;
        sg_asChannel[nChannel].m_OCI_FlexRayConfig.bus.pRateCorrectionOut       = (uint32)ouControllerData.m_shRateCorrOut;
        sg_asChannel[nChannel].m_OCI_FlexRayConfig.bus.pSingleSlotEnabled       = (uint32)ouControllerData.m_bSingleSlotEnable;
        sg_asChannel[nChannel].m_OCI_FlexRayConfig.bus.pWakeupChannel           = OCI_FLEXRAY_CHANNEL_A;//(uint32)ouControllerData.m_nWakeUpChannel;
        sg_asChannel[nChannel].m_OCI_FlexRayConfig.bus.pWakeupPattern           = (uint32)ouControllerData.m_shWakeUpPattern;

        sg_asChannel[nChannel].m_OCI_FlexRayConfig.flow.wakeupEnabled           = false;
        sg_asChannel[nChannel].m_OCI_FlexRayConfig.flow.waitForWUPA             = false;
        sg_asChannel[nChannel].m_OCI_FlexRayConfig.flow.waitForWUPB             = false;
        sg_asChannel[nChannel].m_OCI_FlexRayConfig.flow.coldstartInhibit        = true;
        sg_asChannel[nChannel].m_OCI_FlexRayConfig.flow.repeatColdstart         = true;
        sg_asChannel[nChannel].m_OCI_FlexRayConfig.flow.resumeAfterHalt         = true;

        sg_asChannel[nChannel].m_OCI_FlexRayConfig.rxFifo.payloadLength         = 127;                                              // Payload length reserved for each FIFO entry.
        sg_asChannel[nChannel].m_OCI_FlexRayConfig.rxFifo.rejectFrameIDValue    = 0;                                                // Enqueue all frames in the FIFO.
        sg_asChannel[nChannel].m_OCI_FlexRayConfig.rxFifo.rejectFrameIDMask     = 0;                                                // Enqueue all frames in the FIFO.

        sg_asChannel[nChannel].m_OCI_FlexRayConfig.rxFifo.channel               = OCI_FLEXRAY_CHANNEL_A | OCI_FLEXRAY_CHANNEL_B;    // Reception channel (A, B or both).
    }

    //3. Create Tx Buffers
    //itrEcu = asDeviceConfig.m_strSlectedEculist.begin();
    INT nFrameCount = 0;

    std::list<FRAME_STRUCT> ouFrameList;



    asDeviceConfig.GetSelectedECUTxFrames(ouFrameList);
    sg_asChannel[nChannel].m_SlotBufferMap.clear();

    std::list<FRAME_STRUCT_SORT> ouSortFrameList;
    ouSortFrameList.clear();
    std::list<FRAME_STRUCT>::iterator itrTemp;
    for (itrTemp = ouFrameList.begin(); itrTemp != ouFrameList.end(); itrTemp++)
    {
        FRAME_STRUCT_SORT ouSortFrame;
        ouSortFrame.m_nLength      = itrTemp->m_nLength;
        ouSortFrame.m_nSlotId      = itrTemp->m_nSlotId;
        ouSortFrame.m_nReptition   = itrTemp->m_nReptition;
        ouSortFrame.m_nBaseCycle   = itrTemp->m_nBaseCycle;
        ouSortFrame.m_eSlotType    = itrTemp->m_eSlotType;
        ouSortFrame.m_ouChannel    = itrTemp->m_ouChannel;
        ouSortFrameList.push_back(ouSortFrame);
    }
    ouSortFrameList.sort();

    //ouFrameList.sort();
    for (std::list<FRAME_STRUCT_SORT>::iterator itrFrameList = ouSortFrameList.begin(); (itrFrameList != ouSortFrameList.end() && (nFrameCount < 128)); itrFrameList++ )
    {
        sg_asChannel[nChannel].m_OCI_FlexRayConfig.txBuffer[nFrameCount].payloadLength             = itrFrameList->m_nLength / 2;
        sg_asChannel[nChannel].m_OCI_FlexRayConfig.txBuffer[nFrameCount].payloadPreambleIndicator  = bPayloadPreambleIndicator;
        sg_asChannel[nChannel].m_OCI_FlexRayConfig.txBuffer[nFrameCount].core.slot                 = itrFrameList->m_nSlotId;
        sg_asChannel[nChannel].m_OCI_FlexRayConfig.txBuffer[nFrameCount].core.cyclePeriod          = itrFrameList->m_nReptition;
        sg_asChannel[nChannel].m_OCI_FlexRayConfig.txBuffer[nFrameCount].core.cycleOffset          = itrFrameList->m_nBaseCycle;

        //Dynamic Slots use only Channel A BOA is not supporting AB or B channel
        if ( itrFrameList->m_eSlotType == DYNAMIC )
        {
            sg_asChannel[nChannel].m_OCI_FlexRayConfig.txBuffer[nFrameCount].core.channel = OCI_FLEXRAY_CHANNEL_A;
        }
        else
        {
            if  ( itrFrameList->m_ouChannel == CHANNEL_A )
            {
                sg_asChannel[nChannel].m_OCI_FlexRayConfig.txBuffer[nFrameCount].core.channel = OCI_FLEXRAY_CHANNEL_A;
            }
            else if  ( itrFrameList->m_ouChannel == CHANNEL_B )
            {
                sg_asChannel[nChannel].m_OCI_FlexRayConfig.txBuffer[nFrameCount].core.channel = OCI_FLEXRAY_CHANNEL_B;
            }
            else if ( itrFrameList->m_ouChannel == CHANNEL_AB )
            {
                sg_asChannel[nChannel].m_OCI_FlexRayConfig.txBuffer[nFrameCount].core.channel = OCI_FLEXRAY_CHANNEL_A | OCI_FLEXRAY_CHANNEL_B;
            }
        }
        int nSlotBase = MAKELONG(itrFrameList->m_nSlotId, MAKEWORD(itrFrameList->m_nBaseCycle, itrFrameList->m_ouChannel));
        sg_asChannel[nChannel].m_SlotBufferMap.insert(std::map<SLOT_BASECYCLE, INT>::value_type(nSlotBase, nFrameCount));

        nFrameCount++;
    }

    sg_asChannel[nChannel].m_OCI_FlexRayConfig.validBufferConfigurationCount = nFrameCount;
    sg_asChannel[nChannel].m_unMsgConfig = nFrameCount;

    sg_asChannel[nChannel].m_ouFlexrayData.m_nValidMessages = 0;
    sg_asChannel[0].m_ouFlexrayData.m_mapSlotBaseCycle.clear();
    return true;

    //Power Train
    /*sg_asChannel[nChannel].m_OCI_FlexRayConfig.bus.gColdStartAttempts                   = 8;
    sg_asChannel[nChannel].m_OCI_FlexRayConfig.bus.gdActionPointOffset                    = 2;
    sg_asChannel[nChannel].m_OCI_FlexRayConfig.bus.gdCASRxLowMax                      = 87;
    sg_asChannel[nChannel].m_OCI_FlexRayConfig.bus.gdDynamicSlotIdlePhase             = 1;
    sg_asChannel[nChannel].m_OCI_FlexRayConfig.bus.gdMinislot                         = 5;
    sg_asChannel[nChannel].m_OCI_FlexRayConfig.bus.gdMinislotActionPointOffset            = 2;
    sg_asChannel[nChannel].m_OCI_FlexRayConfig.bus.gdStaticSlot                           = 24;
    sg_asChannel[nChannel].m_OCI_FlexRayConfig.bus.gdSymbolWindow                     = 0;
    sg_asChannel[nChannel].m_OCI_FlexRayConfig.bus.gdTSSTransmitter                       = 9;
    sg_asChannel[nChannel].m_OCI_FlexRayConfig.bus.gdWakeupSymbolRxIdle                   = 59;
    sg_asChannel[nChannel].m_OCI_FlexRayConfig.bus.gdWakeupSymbolRxLow                    = 50;
    sg_asChannel[nChannel].m_OCI_FlexRayConfig.bus.gdWakeupSymbolRxWindow             = 301;
    sg_asChannel[nChannel].m_OCI_FlexRayConfig.bus.gdWakeupSymbolTxIdle                   = 180;
    sg_asChannel[nChannel].m_OCI_FlexRayConfig.bus.gdWakeupSymbolTxLow                    = 60;
    sg_asChannel[nChannel].m_OCI_FlexRayConfig.bus.gListenNoise                           = 2;
    sg_asChannel[nChannel].m_OCI_FlexRayConfig.bus.gMacroPerCycle                     = 3636;
    sg_asChannel[nChannel].m_OCI_FlexRayConfig.bus.gMaxWithoutClockCorrectionFatal        = 2;
    sg_asChannel[nChannel].m_OCI_FlexRayConfig.bus.gMaxWithoutClockCorrectionPassive  = 2;
    sg_asChannel[nChannel].m_OCI_FlexRayConfig.bus.gNetworkManagementVectorLength     = 0;
    sg_asChannel[nChannel].m_OCI_FlexRayConfig.bus.gNumberOfMinislots                 = 289;
    sg_asChannel[nChannel].m_OCI_FlexRayConfig.bus.gNumberOfStaticSlots                   = 91;
    sg_asChannel[nChannel].m_OCI_FlexRayConfig.bus.gOffsetCorrectionStart             = 3632;
    sg_asChannel[nChannel].m_OCI_FlexRayConfig.bus.gPayloadLengthStatic                   = 8;
    sg_asChannel[nChannel].m_OCI_FlexRayConfig.bus.gSyncNodeMax                           = 15;

    //controller configuration
    sg_asChannel[nChannel].m_OCI_FlexRayConfig.bus.pAllowHaltDueToClock                   = 1;
    sg_asChannel[nChannel].m_OCI_FlexRayConfig.bus.pAllowPassiveToActive              = 1;
    sg_asChannel[nChannel].m_OCI_FlexRayConfig.bus.pChannels                          = OCI_FLEXRAY_CHANNEL_A | OCI_FLEXRAY_CHANNEL_B;
    sg_asChannel[nChannel].m_OCI_FlexRayConfig.bus.pClusterDriftDamping                   = 2;
    sg_asChannel[nChannel].m_OCI_FlexRayConfig.bus.pdAcceptedStartupRange             = 220;
    sg_asChannel[nChannel].m_OCI_FlexRayConfig.bus.pDecodingCorrection                    = 48;
    sg_asChannel[nChannel].m_OCI_FlexRayConfig.bus.pDelayCompensationA                    = 1;
    sg_asChannel[nChannel].m_OCI_FlexRayConfig.bus.pDelayCompensationB                    = 1;
    sg_asChannel[nChannel].m_OCI_FlexRayConfig.bus.pdListenTimeout                        = 401202;
    sg_asChannel[nChannel].m_OCI_FlexRayConfig.bus.pdMaxDrift                         = 601;
    sg_asChannel[nChannel].m_OCI_FlexRayConfig.bus.pExternOffsetCorrection                = 0;
    sg_asChannel[nChannel].m_OCI_FlexRayConfig.bus.pExternRateCorrection              = 0;
    sg_asChannel[nChannel].m_OCI_FlexRayConfig.bus.pKeySlotId                         = 3;

    sg_asChannel[nChannel].m_OCI_FlexRayConfig.bus.pKeySlotUsedForStartup             = 1;
    sg_asChannel[nChannel].m_OCI_FlexRayConfig.bus.pKeySlotUsedForSync                    = 1;
    sg_asChannel[nChannel].m_OCI_FlexRayConfig.bus.pLatestTx                          = 249;
    sg_asChannel[nChannel].m_OCI_FlexRayConfig.bus.pMacroInitialOffsetA                   = 3;
    sg_asChannel[nChannel].m_OCI_FlexRayConfig.bus.pMacroInitialOffsetB                   = 3;
    sg_asChannel[nChannel].m_OCI_FlexRayConfig.bus.pMicroInitialOffsetA                   = 6;
    sg_asChannel[nChannel].m_OCI_FlexRayConfig.bus.pMicroInitialOffsetB                   = 6;
    sg_asChannel[nChannel].m_OCI_FlexRayConfig.bus.pMicroPerCycle                     = 200000;
    sg_asChannel[nChannel].m_OCI_FlexRayConfig.bus.pOffsetCorrectionOut                   = 127;
    sg_asChannel[nChannel].m_OCI_FlexRayConfig.bus.pRateCorrectionOut                 = 601;
    sg_asChannel[nChannel].m_OCI_FlexRayConfig.bus.pSingleSlotEnabled                 = 0;
    sg_asChannel[nChannel].m_OCI_FlexRayConfig.bus.pWakeupChannel                     = OCI_FLEXRAY_CHANNEL_A;
    sg_asChannel[nChannel].m_OCI_FlexRayConfig.bus.pWakeupPattern                     = 33;
    */
}


/**
 * Create time mod mapping
 */
void vCreateTimeModeMapping(HANDLE hEvent)
{
    WaitForSingleObject(hEvent, INFINITE);
    GetLocalTime(&sg_CurrSysTime);
    //Query Tick Count
    QueryPerformanceCounter(&sg_QueryTickCount);
}

/**
 * Copies from STCAN_MSG structure into OCI_CANTxMessage message structure
 */
void vCopy_2_OCI_FLexRay_Data(OCI_FlexRayTxMessage& DestMsg, const s_FLXTXMSG& SrcMsg)
{
    DestMsg.size    = SrcMsg.m_sFlxMsg.m_nDLC;
    DestMsg.flags = 0;
    memcpy(DestMsg.data, SrcMsg.m_sFlxMsg.m_ucData, sizeof(WORD) * 127);
}

/**
 * Pushes an entry into the list at the last position
 */
/*void vMarkEntryIntoMap(const SACK_MAP& RefObj)
{
    //EnterCriticalSection(&sg_CritSectForAckBuf); // Lock the buffer
    sg_asAckMapBuf.push_back(RefObj);
    //LeaveCriticalSection(&sg_CritSectForAckBuf); // Unlock the buffer
}
*/
/**
 * \return TRUE if client exists else FALSE
 *
 * Checks for the existance of the client with the name pcClientName.
 */
static BOOL bClientExist(std::string pcClientName, INT& Index)
{
    for (UINT i = 0; i < sg_asClientToBufMap.size(); i++)
    {
        if (pcClientName == sg_asClientToBufMap[i].m_acClientName)
        {
            Index = i;
            return TRUE;
        }
    }
    return FALSE;
}


/**
 * \return TRUE if client exists else FALSE
 *
 * Searches for the client with the id dwClientId.
 */
static BOOL bClientIdExist(const DWORD& dwClientId)
{
    BOOL bReturn = FALSE;

    for (UINT i = 0; i < sg_asClientToBufMap.size(); i++)
    {
        if (sg_asClientToBufMap[i].m_dwClientID == dwClientId)
        {
            bReturn = TRUE;
            i = sg_unClientCnt; // break the loop
        }
    }
    return bReturn;
}

/**
 * Returns the available slot
 */
static DWORD dwGetAvailableClientSlot()
{
    DWORD nClientId = 2;
    for (INT i = 0; i < MAX_CLIENT_ALLOWED; i++)
    {
        if (bClientIdExist(nClientId))
        {
            nClientId += 1;
        }
        else
        {
            i = MAX_CLIENT_ALLOWED; //break the loop
        }
    }
    return nClientId;
}

/**
 * \return Returns true if found else false.
 *
 * unClientIndex will have index to client array which has clientId dwClientID.
 */
static BOOL bGetClientObj(DWORD dwClientID, UINT& unClientIndex)
{
    BOOL bResult = FALSE;

    for (UINT i = 0; i < sg_asClientToBufMap.size(); i++)
    {
        if (sg_asClientToBufMap[i].m_dwClientID == dwClientID)
        {
            unClientIndex = i;
            i = sg_unClientCnt; //break the loop
            bResult = TRUE;
            break;
        }
    }
    return bResult;
}

/**
 * \return TRUE if client removed else FALSE
 *
 * Removes the client with client id dwClientId.
 */
static BOOL bRemoveClient(DWORD dwClientId)
{
    BOOL bResult = FALSE;
    if (sg_unClientCnt > 0)
    {
        UINT unClientIndex = 0;
        if (bGetClientObj(dwClientId, unClientIndex))
        {
            sg_asClientToBufMap[unClientIndex].m_dwClientID = 0;
            sg_asClientToBufMap[unClientIndex].m_acClientName = "";

            for (INT i = 0; i < MAX_BUFF_ALLOWED; i++)
            {
                sg_asClientToBufMap[unClientIndex].m_pClientBuf[i] = nullptr;
            }
            sg_asClientToBufMap[unClientIndex].m_unBufCount = 0;
            if ((unClientIndex + 1) < sg_unClientCnt)
            {
                sg_asClientToBufMap[unClientIndex] = sg_asClientToBufMap[sg_unClientCnt - 1];
            }
            sg_unClientCnt--;
            bResult = TRUE;
        }
    }
    return bResult;
}

/**
 * \param[in] nChannel Channel information
 *
 * Initialize default values for controller configuration
 */
void vInitializeControllerConfig(UINT nChannel)
{

    // set configuration tree
    sg_asChannel[nChannel].m_OCI_FlexRayConfig.bus.gColdStartAttempts                   = 8;
    sg_asChannel[nChannel].m_OCI_FlexRayConfig.bus.gdActionPointOffset                  = 2;
    sg_asChannel[nChannel].m_OCI_FlexRayConfig.bus.gdCASRxLowMax                        = 91;
    sg_asChannel[nChannel].m_OCI_FlexRayConfig.bus.gdDynamicSlotIdlePhase               = 1;
    sg_asChannel[nChannel].m_OCI_FlexRayConfig.bus.gdMinislot                           = 5;
    sg_asChannel[nChannel].m_OCI_FlexRayConfig.bus.gdMinislotActionPointOffset          = 2;
    sg_asChannel[nChannel].m_OCI_FlexRayConfig.bus.gdStaticSlot                         = 24;
    sg_asChannel[nChannel].m_OCI_FlexRayConfig.bus.gdSymbolWindow                       = 0;
    sg_asChannel[nChannel].m_OCI_FlexRayConfig.bus.gdTSSTransmitter                     = 9;
    sg_asChannel[nChannel].m_OCI_FlexRayConfig.bus.gdWakeupSymbolRxIdle                 = 59;
    sg_asChannel[nChannel].m_OCI_FlexRayConfig.bus.gdWakeupSymbolRxLow                  = 50;
    sg_asChannel[nChannel].m_OCI_FlexRayConfig.bus.gdWakeupSymbolRxWindow               = 301;
    sg_asChannel[nChannel].m_OCI_FlexRayConfig.bus.gdWakeupSymbolTxIdle                 = 180;
    sg_asChannel[nChannel].m_OCI_FlexRayConfig.bus.gdWakeupSymbolTxLow                  = 60;
    sg_asChannel[nChannel].m_OCI_FlexRayConfig.bus.gListenNoise                         = 2;
    sg_asChannel[nChannel].m_OCI_FlexRayConfig.bus.gMacroPerCycle                       = 3636;
    sg_asChannel[nChannel].m_OCI_FlexRayConfig.bus.gMaxWithoutClockCorrectionFatal      = 2;
    sg_asChannel[nChannel].m_OCI_FlexRayConfig.bus.gMaxWithoutClockCorrectionPassive    = 2;
    sg_asChannel[nChannel].m_OCI_FlexRayConfig.bus.gNumberOfMinislots                   = 289;
    sg_asChannel[nChannel].m_OCI_FlexRayConfig.bus.gNumberOfStaticSlots                 = 91;
    sg_asChannel[nChannel].m_OCI_FlexRayConfig.bus.gOffsetCorrectionStart               = 3632;
    sg_asChannel[nChannel].m_OCI_FlexRayConfig.bus.gPayloadLengthStatic                 = 8;
    sg_asChannel[nChannel].m_OCI_FlexRayConfig.bus.gSyncNodeMax                         = 15;
    sg_asChannel[nChannel].m_OCI_FlexRayConfig.bus.pAllowHaltDueToClock                 = 1;
    sg_asChannel[nChannel].m_OCI_FlexRayConfig.bus.pAllowPassiveToActive                = 0;
    sg_asChannel[nChannel].m_OCI_FlexRayConfig.bus.pChannels                            = OCI_FLEXRAY_CHANNEL_A;
    sg_asChannel[nChannel].m_OCI_FlexRayConfig.bus.pdAcceptedStartupRange               = 212;
    sg_asChannel[nChannel].m_OCI_FlexRayConfig.bus.pClusterDriftDamping                 = 2;
    sg_asChannel[nChannel].m_OCI_FlexRayConfig.bus.pDecodingCorrection                  = 48;
    sg_asChannel[nChannel].m_OCI_FlexRayConfig.bus.pDelayCompensationA                  = 1;
    sg_asChannel[nChannel].m_OCI_FlexRayConfig.bus.pDelayCompensationB                  = 1;
    sg_asChannel[nChannel].m_OCI_FlexRayConfig.bus.pdListenTimeout                      = 400242;
    sg_asChannel[nChannel].m_OCI_FlexRayConfig.bus.pdMaxDrift                           = 121;
    sg_asChannel[nChannel].m_OCI_FlexRayConfig.bus.pExternOffsetCorrection              = 0;
    sg_asChannel[nChannel].m_OCI_FlexRayConfig.bus.pExternRateCorrection                = 0;
    sg_asChannel[nChannel].m_OCI_FlexRayConfig.bus.pKeySlotId                           = 0;
    sg_asChannel[nChannel].m_OCI_FlexRayConfig.bus.pKeySlotUsedForStartup               = 1;
    sg_asChannel[nChannel].m_OCI_FlexRayConfig.bus.pKeySlotUsedForSync                  = 1;
    sg_asChannel[nChannel].m_OCI_FlexRayConfig.bus.pLatestTx                            = 249;
    sg_asChannel[nChannel].m_OCI_FlexRayConfig.bus.pMacroInitialOffsetA                 = 3;
    sg_asChannel[nChannel].m_OCI_FlexRayConfig.bus.pMacroInitialOffsetB                 = 3;
    sg_asChannel[nChannel].m_OCI_FlexRayConfig.bus.pMicroInitialOffsetA                 = 6;
    sg_asChannel[nChannel].m_OCI_FlexRayConfig.bus.pMicroInitialOffsetB                 = 6;
    sg_asChannel[nChannel].m_OCI_FlexRayConfig.bus.pMicroPerCycle                       = 200000;
    sg_asChannel[nChannel].m_OCI_FlexRayConfig.bus.pOffsetCorrectionOut                 = 126;
    sg_asChannel[nChannel].m_OCI_FlexRayConfig.bus.pRateCorrectionOut                   = 121;
    sg_asChannel[nChannel].m_OCI_FlexRayConfig.bus.pSingleSlotEnabled                   = 0;//<-- all slot mode
    sg_asChannel[nChannel].m_OCI_FlexRayConfig.bus.pWakeupChannel                       = 0;
    sg_asChannel[nChannel].m_OCI_FlexRayConfig.bus.pWakeupPattern                       = 33;
    sg_asChannel[nChannel].m_OCI_FlexRayConfig.bus.gNetworkManagementVectorLength       = 127;
    sg_asChannel[nChannel].m_OCI_FlexRayConfig.bus.gPayloadLengthStatic                 = 15;

    sg_asChannel[nChannel].m_OCI_FlexRayConfig.flow.wakeupEnabled                       = false;
    sg_asChannel[nChannel].m_OCI_FlexRayConfig.flow.waitForWUPA                         = false;
    sg_asChannel[nChannel].m_OCI_FlexRayConfig.flow.waitForWUPB                         = false;
    sg_asChannel[nChannel].m_OCI_FlexRayConfig.flow.coldstartInhibit                    = false;
    sg_asChannel[nChannel].m_OCI_FlexRayConfig.flow.repeatColdstart                     = true;
    sg_asChannel[nChannel].m_OCI_FlexRayConfig.flow.resumeAfterHalt                     = true;

    sg_asChannel[nChannel].m_OCI_FlexRayConfig.rxFifo.payloadLength                     = 127;// was 127
    sg_asChannel[nChannel].m_OCI_FlexRayConfig.rxFifo.rejectFrameIDValue                = 0;
    sg_asChannel[nChannel].m_OCI_FlexRayConfig.rxFifo.rejectFrameIDMask                 = 0;
    sg_asChannel[nChannel].m_OCI_FlexRayConfig.rxFifo.channel                           = OCI_FLEXRAY_CHANNEL_A;
    sg_asChannel[nChannel].m_OCI_FlexRayConfig.rxFifo.cyclePeriod                       = 1;
    sg_asChannel[nChannel].m_OCI_FlexRayConfig.rxFifo.cycleOffset                       = 0;
    sg_asChannel[nChannel].m_OCI_FlexRayConfig.rxFifo.rejectStaticSegment               = false;
    sg_asChannel[nChannel].m_OCI_FlexRayConfig.rxFifo.rejectNullFrames                  = false;

    sg_asChannel[nChannel].m_OCI_FlexRayConfig.advanced.exclusiveAccess                 = false;
    sg_asChannel[nChannel].m_OCI_FlexRayConfig.advanced.busMode                         = OCI_FLEXRAY_BUSMODE_NORMAL;

    sg_asChannel[nChannel].m_OCI_FlexRayConfig.validBufferConfigurationCount            = 2;


    //key slot, I might need it for some reason
    sg_asChannel[nChannel].m_OCI_FlexRayConfig.txBuffer[0].core.slot                    = 40;
    sg_asChannel[nChannel].m_OCI_FlexRayConfig.txBuffer[0].core.channel                 = OCI_FLEXRAY_CHANNEL_A;
    sg_asChannel[nChannel].m_OCI_FlexRayConfig.txBuffer[0].core.cyclePeriod             = 1;
    sg_asChannel[nChannel].m_OCI_FlexRayConfig.txBuffer[0].core.cycleOffset             = 0;
    sg_asChannel[nChannel].m_OCI_FlexRayConfig.txBuffer[0].payloadLength                = 15;
    sg_asChannel[nChannel].m_OCI_FlexRayConfig.txBuffer[0].payloadPreambleIndicator     = false;

    //slot 1, channel A and B
    sg_asChannel[nChannel].m_OCI_FlexRayConfig.txBuffer[1].core.slot                    = 1;
    sg_asChannel[nChannel].m_OCI_FlexRayConfig.txBuffer[1].core.channel                 = OCI_FLEXRAY_CHANNEL_A;
    sg_asChannel[nChannel].m_OCI_FlexRayConfig.txBuffer[1].core.cyclePeriod             = 1;
    sg_asChannel[nChannel].m_OCI_FlexRayConfig.txBuffer[1].core.cycleOffset             = 0;
    sg_asChannel[nChannel].m_OCI_FlexRayConfig.txBuffer[1].payloadLength                = 15;
    sg_asChannel[nChannel].m_OCI_FlexRayConfig.txBuffer[1].payloadPreambleIndicator     = false;

    //Set controller property to SUSPENDED
    sg_asChannel[nChannel].m_OCI_CntrlProp.mode = OCI_CONTROLLER_MODE_SUSPENDED;

}


/**
 * \return Returns channel number
 *
 * Gets the channel with handle OCI_ControllerHandle
 */
UINT nGetChannel(const OCI_ControllerHandle hHandle)
{
    UINT i;
    for (i = 0; i < sg_nNoOfChannels; i++)
    {
        if (sg_asChannel[i].m_OCI_HwHandle == hHandle)
        {
            break;
        }
    }
    return (i+1);
}

static void vCopyOCI_FlexRay_RX_2_DATA(s_FLXMSG& FlexData,
                                       OCI_FlexRayMessage& BOA_FlexData)
{
    FlexData.m_unTimestamp = (LONGLONG)(BOA_FlexData.data.rxMessage.timeStamp * sg_asChannel[0].m_fResolution);


    FlexData.stcDataMsg.m_nSlotID = BOA_FlexData.data.rxMessage.frameID;
    FlexData.stcDataMsg.m_ucCycleNumber = BOA_FlexData.data.rxMessage.cycleCount;
    FlexData.m_eMessageType = FLXMSGTYPE_DATA;

    FlexData.stcDataMsg.m_nDLC = BOA_FlexData.data.rxMessage.size;
    memcpy(FlexData.stcDataMsg.m_ucData, BOA_FlexData.data.rxMessage.data, sizeof(BOA_FlexData.data.rxMessage.data));

    FlexData.stcDataMsg.m_lHeaderInfoFlags = 0;




    //SelfReceiption
    if ( BOA_FlexData.data.rxMessage.flags & OCI_FLEXRAY_MSG_FLAG_SELFRECEPTION )
    {
        //Null Frame
        FlexData.stcDataMsg.m_lHeaderInfoFlags = FlexData.stcDataMsg.m_lHeaderInfoFlags | RBIN_FLXHDRINFO_SELFRECEPTION;
        if ( (BOA_FlexData.data.rxMessage.frameIndicators & OCI_FLEXRAY_NULL_FRAME_INDICATOR) )
        {
            FlexData.stcDataMsg.m_lHeaderInfoFlags = FlexData.stcDataMsg.m_lHeaderInfoFlags | RBIN_FLXHDRINFO_NULLFRAME;
            memset(FlexData.stcDataMsg.m_ucData, 0, sizeof(FlexData.stcDataMsg.m_ucData));
        }
    }
    //Null Frame
    else if ( !(BOA_FlexData.data.rxMessage.frameIndicators & OCI_FLEXRAY_NULL_FRAME_INDICATOR) )
    {
        FlexData.stcDataMsg.m_lHeaderInfoFlags = FlexData.stcDataMsg.m_lHeaderInfoFlags | RBIN_FLXHDRINFO_NULLFRAME;
        memset(FlexData.stcDataMsg.m_ucData, 0, sizeof(FlexData.stcDataMsg.m_ucData));
    }
    //Sync Frame
    if ( BOA_FlexData.data.rxMessage.frameIndicators & OCI_FLEXRAY_SYNC_FRAME_INDICATOR )
    {
        FlexData.stcDataMsg.m_lHeaderInfoFlags = FlexData.stcDataMsg.m_lHeaderInfoFlags | RBIN_FLXHDRINFO_SYNCFRAME;
    }

    if ( BOA_FlexData.data.rxMessage.frameIndicators & OCI_FLEXRAY_STARTUP_FRAME_INDICATOR )
    {
        FlexData.stcDataMsg.m_lHeaderInfoFlags = FlexData.stcDataMsg.m_lHeaderInfoFlags | RBIN_FLXHDRINFO_STARTUPFRAME;
    }

    //Channel
    if ( BOA_FlexData.data.rxMessage.ccIndicators == OCI_FLEXRAY_CHANNEL_A )
    {
        FlexData.stcDataMsg.m_eChannel = CHANNEL_A;
    }
    else if ( BOA_FlexData.data.rxMessage.ccIndicators == OCI_FLEXRAY_CHANNEL_B )
    {
        FlexData.stcDataMsg.m_eChannel = CHANNEL_B;
    }
    else if ( BOA_FlexData.data.rxMessage.ccIndicators == ( OCI_FLEXRAY_CHANNEL_A | OCI_FLEXRAY_CHANNEL_B ) )
    {
        FlexData.stcDataMsg.m_eChannel = CHANNEL_AB;
    }

    FlexData.stcDataMsg.m_bIsRxMsg = true;
    if ( (BOA_FlexData.data.rxMessage.flags & OCI_FLEXRAY_MSG_FLAG_SELFRECEPTION) == OCI_FLEXRAY_MSG_FLAG_SELFRECEPTION )
    {
        FlexData.stcDataMsg.m_bIsRxMsg = false;
    }


    //FlexData.stcDataMsg.m_lHeaderInfoFlags = ()? TX_FLAG : RX_FLAG;

}

/**
 * \param[in] nChannel Channel information
 *
 * Initialize default values for filter confgiuration
 */
void vInitializeFilterConfig(UINT nChannel)
{
    /* configure frame filter*/
    sg_asChannel[nChannel].m_OCI_FrameFilter.frameIDMask   = 0;    // The ID of a received frame will be ANDed with this value (will always result in 0 in this example).
    sg_asChannel[nChannel].m_OCI_FrameFilter.frameIDValue  = 0;    // The ID of a received frame ANDed with frameIDMask will be compared against this value.
    sg_asChannel[nChannel].m_OCI_FrameFilter.cyclePeriod   = 1;    // The periodicity on which to receive the frames.
    sg_asChannel[nChannel].m_OCI_FrameFilter.cycleOffset   = 0;    // The offset to start the periodicity.
    sg_asChannel[nChannel].m_OCI_FrameFilter.tag           = 0;    // We need no tag in this application. We can set any value. It will be ignored by the application.
    sg_asChannel[nChannel].m_OCI_FrameFilter.ccIndicators  = OCI_FLEXRAY_CHANNEL_A | OCI_FLEXRAY_CHANNEL_B;    // Receive the messages that are received on any of the two channels.

    /* configure frame filter*/
    sg_asChannel[nChannel].m_OCI_EventFilter.destination = OCI_EVENT_DESTINATION_CALLBACK;
    sg_asChannel[nChannel].m_OCI_EventFilter.eventCode = (OCI_FlexRayEvent)(OCI_FLEXRAY_EVENT_START_OF_STATIC_SEGMENT |OCI_FLEXRAY_EVENT_START_OF_DYNAMIC_SEGMENT |OCI_FLEXRAY_EVENT_POCS_CHANGE) ;
    sg_asChannel[nChannel].m_OCI_EventFilter.tag = 0;

    /* configure Error filter */

    /* configure internal error filter */
    sg_asChannel[nChannel].m_OCI_InternalErrorFilter.eventCode = OCI_INTERNAL_GENERAL_ERROR;
    sg_asChannel[nChannel].m_OCI_InternalErrorFilter.tag = 0;
}
/**************************************************************************/


/* CDIL_FLEXRAY_ETAS_BOA class definition */
class CDIL_FLEXRAY_ETAS_BOA : public CCommanDIL_Flexray
{
public:
    /* STARTS IMPLEMENTATION OF THE INTERFACE FUNCTIONS... */
    HRESULT FLEXRAY_PerformInitOperations(void) ;
    HRESULT FLEXRAY_PerformClosureOperations(void) ;
    HRESULT FLEXRAY_GetTimeModeMapping(SYSTEMTIME& CurrSysTime, UINT64& TimeStamp) ;
    HRESULT FLEXRAY_ListHwInterfaces(FLEXRAY_INTERFACE_HW& sSelHwInterface, FLEXRAY_CONFIG_FILES& sAvailableConfigFiles) ;
    HRESULT FLEXRAY_SelectHwInterface(const FLEXRAY_INTERFACE_HW_LIST& sSelHwInterface) ;
    HRESULT FLEXRAY_DeselectHwInterface(void) ;
    HRESULT FLEXRAY_DisplayConfigDlg(PCHAR& InitData, int& Length) ;
    HRESULT FLEXRAY_SetConfigData(CHANNEL_CONFIG& ouAbsSFibexContainer) ;
    HRESULT FLEXRAY_SetAppParams(HWND hWndOwner, Base_WrapperErrorLogger* pILog) ;
    HRESULT FlexRAY_GetControllerCapabilities(s_FLXControllerCapabilities& ouFlexRayCapabilities);
    //HRESULT FLEXRAY_ManageMsgBuf(BYTE byAction, DWORD ClientID, CBaseFLEXBufFSE* pBufObj) ;
    //HRESULT FLEXRAY_RegisterClient(BOOL bRegister, DWORD& ClientID, char* pacClientName);
    HRESULT FLEXRAY_StartHardware(void) ;
    HRESULT FLEXRAY_StopHardware(void) ;
    HRESULT FLEXRAY_ResetHardware(void) ;
    HRESULT FLEXRAY_GetCurrStatus(s_FLXSTATUSMSG& StatusData) ;
    HRESULT FLEXRAY_GetFlxTxMsgBuffer(CFlxTxMsgBuffer*& pouFlxTxMsgBuffer) ;
    HRESULT FLEXRAY_SendMsg(DWORD dwClientID, s_FLXTXMSG* pouFlxTxMsg);
    HRESULT FLEXRAY_DeleteMsg(DWORD dwClientID, s_FLXTXMSG* pouFlxTxMsg);
    HRESULT FLEXRAY_GetBoardInfo(s_BOARDINFO& BoardInfo) ;
    HRESULT FLEXRAY_GetFlexRayInfo(s_FLXINFO& FlexRayInfo) ;
    HRESULT FLEXRAY_GetVersionInfo(VERSIONINFO& sVerInfo) ;
    HRESULT FLEXRAY_GetLastErrorString(std::string acErrorStr, HRESULT& nError) ;
    HRESULT FLEXRAY_FilterFrames(FILTER_TYPE FilterType, FLEX_CHANNEL FlexChannel, UINT* punFrames, UINT nLength) ;
    HRESULT FLEXRAY_ConfigMsgBuf(s_MSGBUF sMsgBuf, UINT* punAddress) ;
    HRESULT FLEXRAY_LoadDriverLibrary(void) ;
    HRESULT FLEXRAY_UnloadDriverLibrary(void) ;

    int nCreateTxQueue();
private:
    //static CClientList         m_ClientList;               ///< List with all of registered clients


};
//CClientList   CDIL_FLEXRAY_ETAS_BOA::m_ClientList;
CDIL_FLEXRAY_ETAS_BOA* g_pouDIL_FLEXRAY_BOA = nullptr;
/**
 * This function writes the message to the corresponding clients buffer
 */
/*void CDIL_FLEXRAY_ETAS_BOA::vWriteIntoClientsBuffer(s_FLXMSG& sFlexRayData)
{
    //Write into the client's buffer and Increment message Count
    for (int iClientIndex=0; iClientIndex < m_ClientList.Size(); iClientIndex++)
    {
        CClientBuffer* pClient = m_ClientList.GetClientByIndex(iClientIndex);
        if (pClient)
        {
            for (int iBufIndex = 0; iBufIndex < pClient->NumOfSEBuffers(); iBufIndex++)
            {
                CBaseFLEXBufFSE* pFLEXBufSE = pClient->GetSEBufferByIndex(iBufIndex);
                if (pFLEXBufSE)
                {
                    pFLEXBufSE->WriteIntoBuffer(&sFlexRayData);
                }
            }
        }
    }
}*/
/**
 * Processes Rx msg and writes into regiastered clients buffer.
 */
void vProcessRxMsg(void* userData, struct OCI_FlexRayMessage* msg)
{
    /* First calculate timestamp for first message*/
    // EnterCriticalSection(&sg_DIL_CriticalSection);

    s_FLXMSG FlexData;
    OCI_ControllerHandle hHandle;

    hHandle = (OCI_ControllerHandle)userData;
    FlexData.stcDataMsg.m_nCluster =  (UCHAR)nGetChannel(hHandle);

    if ( msg->type == OCI_FLEXRAY_RX_MESSAGE )
    {
        //sCanData.m_uDataInfo.m_sCANMsg.m_bCANFDMsg = false;
        FlexData.m_eMessageType = FLXMSGTYPE_DATA;
        //_cprintf("%x %d %d %d\n", msg->data.rxMessage.frameID, msg->data.rxMessage.data[0], msg->data.rxMessage.data[1], msg->data.rxMessage.data[2]);
        vCopyOCI_FlexRay_RX_2_DATA(FlexData, *msg);
    }

    if ( msg->type == OCI_FLEXRAY_EVENT )
    {
        FlexData.m_eMessageType = FLXMSGTYPE_STATUS;
        if ( msg->data.flexRayEventMessage.eventCode == OCI_FLEXRAY_EVENT_POCS_CHANGE )
        {
            //There is One to One Map For BUSMASTER POC Status to BOA POC Status.
            FlexData.stcStatusMsg.m_oeFlexRayStatus = (eFlexRayPOCStatus)msg->data.flexRayEventMessage.protocolOperationControlStatus;
            FlexData.m_unTimestamp = (LONGLONG)(msg->data.flexRayEventMessage.timeStamp * sg_asChannel[0].m_fResolution);
        }
    }


    if (sg_byCurrState == CREATE_MAP_TIMESTAMP)
    {
        //sg_TimeStamp = sCanData.m_lTickCount.QuadPart;
        SetEvent(sg_hEvent);
        vCreateTimeModeMapping(sg_hEvent);
        sg_byCurrState = CALC_TIMESTAMP_READY;


        LARGE_INTEGER g_QueryTickCount;
        QueryPerformanceCounter(&g_QueryTickCount);
        UINT64 unConnectionTime;
        unConnectionTime = ((g_QueryTickCount.QuadPart * 10000) / sg_lnFrequency.QuadPart) - sg_TimeStamp;
        //Time difference should be +ve value
        if( FlexData.m_unTimestamp >= unConnectionTime)
        {
            sg_TimeStamp  = (LONGLONG)(FlexData.m_unTimestamp - unConnectionTime);
        }
        else
        {
            sg_TimeStamp  = (LONGLONG)(unConnectionTime - FlexData.m_unTimestamp);
        }
    }

    //Write the msg into registered client's buffer
    if ( nullptr != g_pouDIL_FLEXRAY_BOA )
    {
        g_pouDIL_FLEXRAY_BOA->vWriteIntoClientsBuffer(FlexData);
    }
    // LeaveCriticalSection(&sg_DIL_CriticalSection);
}
/**
 * Callback function called by the BOA framework
 * when there is presence of msg in the bus.
 */
static void (OCI_CALLBACK ProcessFlexRayData)(void* userData, OCI_FlexRayMessage* msg)
{
    switch (msg->type)
    {
        case OCI_FLEXRAY_RX_MESSAGE:
            EnterCriticalSection(&sg_DIL_CriticalSection);
            vProcessRxMsg(userData, msg);
            LeaveCriticalSection(&sg_DIL_CriticalSection);
            break;
            /*case OCI_FLEXRAY_TX_MESSAGE:
                vProcessTxMsg(userData, msg);
                break;*/

    }
}
static void EventCallback ( void* userData, struct OCI_FlexRayMessage* msg )
{
    switch ( msg->type )
    {

        case OCI_FLEXRAY_EVENT:

            EnterCriticalSection(&sg_DIL_CriticalSection);
            vProcessRxMsg(userData, msg);
            LeaveCriticalSection(&sg_DIL_CriticalSection);


            break;

        case OCI_FLEXRAY_TIMER_EVENT:
            // The OCI_FLEXRAY_TIMER_EVENT message type represents events that occurred on the Timer belonging to the same the Controller
            // which owns the Rx Queue that has received the message.  In this example, no Timer events should be received by the Event
            // Callback, but this code demonstrates how to handle such events.
            //
            // As with the OCI_FLEXRAY_EVENT type, these events can be recieved by the Frame Callback, as well as the Event Callback,
            // function depending on the value of the 'destination' field in the filter which matches the event.
            printf( "Message type: OCI_FLEXRAY_TIMER_EVENT.\n" );
            printf( "Message Time Stamp: %llu\n", msg->data.timerEventMessage.timeStamp );
            printf( "Event Code: 0x%x\n", msg->data.timerEventMessage.eventCode );
            printf( "Message Tag: 0x%x\n", msg->data.timerEventMessage.tag );

            // DO SOMETHING USEFUL WITH THE TIMER EVENT HERE.

            break;

        case OCI_FLEXRAY_QUEUE_EVENT:
            // The OCI_FLEXRAY_QUEUE_EVENT message type represents events that occurred on any Queue belonging to the same the Controller
            // which owns the Rx Queue that has received the message.  The queue on which the event ocurred is not referenced in the
            // message, but this can be overcome by coupling the tag with the Queue handle when creating the Queue Event Filters.
            // In this example, no Queue events should be received by the Event Callback, but this code demonstrates how to handle
            // such events.
            //
            // As with the OCI_FLEXRAY_EVENT type, these events can be recieved by the Frame Callback, as well as the Event Callback,
            // function depending on the value of the 'destination' field in the filter which matches the event.
            printf( "Message type: OCI_FLEXRAY_QUEUE_EVENT.\n" );
            printf( "Message Time Stamp: %llu\n", msg->data.queueEventMessage.timeStamp );
            printf( "Event Code: 0x%x\n", msg->data.queueEventMessage.eventCode );
            printf( "Message Tag: 0x%x\n", msg->data.queueEventMessage.tag );

            // DO SOMETHING USEFUL WITH THE QUEUE EVENT HERE.

            break;
    }
}
/**
 * \param[in] nChannel Channel information
 *
 * Initialize default values for queue confgiuration
 */
void vInitializeQueueConfig(UINT nChannel)
{
    /* configure Rx Queue*/
    sg_asChannel[nChannel].m_OCI_RxQueueCfg.onFrame.function = ProcessFlexRayData;
    sg_asChannel[nChannel].m_OCI_RxQueueCfg.onFrame.userData = nullptr;
    sg_asChannel[nChannel].m_OCI_RxQueueCfg.onEvent.function = EventCallback;
    sg_asChannel[nChannel].m_OCI_RxQueueCfg.onEvent.userData = nullptr;

    sg_asChannel[nChannel].m_OCI_RxQueueCfg.selfReceptionMode = OCI_SELF_RECEPTION_OFF;
    if ( sg_asChannel[nChannel].m_ControllerCapabilities.supportedBusMode & OCI_FLEXRAY_BUSMODE_SELFRECEPTION )
    {
        sg_asChannel[nChannel].m_OCI_RxQueueCfg.selfReceptionMode = OCI_SELF_RECEPTION_ON;
    }

    /* configure Tx Queue*/
    //sg_asChannel[nChannel].m_OCI_TxQueueCfg.reserved = 0;
}
/**
* \brief         Returns the CDIL_FLEXRAY_ETAS_BOA object
* \param[out]    ppvInterface, is void pointer to take back the reference to CDIL_CAN_VectorXL object
* \return        S_OK for success, S_FALSE for failure
* \authors       Arunkumar Karri
* \date          07.10.2011 Created
*/


/**
 * \return S_OK for success, S_FALSE for failure
 *
 * Registers a client to the DIL. ClientID will have client id
 * which will be used for further client related calls
 */
/*
HRESULT CDIL_FLEXRAY_ETAS_BOA::FLEXRAY_RegisterClient(BOOL bRegister, DWORD& ClientID, char* pacClientName)
{
    HRESULT hResult = S_FALSE;
    if (bRegister)
    {
        if (sg_unClientCnt < MAX_CLIENT_ALLOWED)
        {
            INT Index = 0;
            if (!bClientExist(pacClientName, Index))
            {
                //Currently store the client information
                if (strcmp(pacClientName, FLEXRAY_MONITER_NODE) == 0)
                {
                    //First slot is reserved to monitor node
                    ClientID = 1;
                    sg_asClientToBufMap[0].m_acClientName = pacClientName;
                    sg_asClientToBufMap[0].m_dwClientID = ClientID;
                    sg_asClientToBufMap[0].m_unBufCount = 0;
                }
                else
                {
                    if (!bClientExist(FLEXRAY_MONITER_NODE, Index))
                    {
                        Index = sg_unClientCnt + 1;
                    }
                    else
                    {
                        Index = sg_unClientCnt;
                    }
                    ClientID = dwGetAvailableClientSlot();
                    sg_asClientToBufMap[Index].m_acClientName = pacClientName;

                    sg_asClientToBufMap[Index].m_dwClientID = ClientID;
                    sg_asClientToBufMap[Index].m_unBufCount = 0;
                }
                sg_unClientCnt++;
                hResult = S_OK;
            }
            else
            {
                ClientID = sg_asClientToBufMap[Index].m_dwClientID;
                hResult = ERR_CLIENT_EXISTS;
            }
        }
        else
        {
            hResult = ERR_NO_MORE_CLIENT_ALLOWED;
        }
    }
    else
    {
        if (bRemoveClient(ClientID))
        {
            hResult = S_OK;
        }
        else
        {
            hResult = ERR_NO_CLIENT_EXIST;
        }
    }

    return hResult;
}
*/

USAGEMODE HRESULT GetIDIL_FLEXRAY_Controller(void** ppvInterface)
{
    HRESULT hResult = S_OK;
    if ( nullptr == g_pouDIL_FLEXRAY_BOA )
    {
        if ((g_pouDIL_FLEXRAY_BOA = new CDIL_FLEXRAY_ETAS_BOA) == nullptr)
        {
            hResult = S_FALSE;
        }
    }
    *ppvInterface = (void*) g_pouDIL_FLEXRAY_BOA;  /* Doesn't matter even if g_pouDIL_FLEXRAY_BOA is null */

    return hResult;
}

/* Implementation for CDIL_FLEXRAY_ETAS_BOA class */
HRESULT CDIL_FLEXRAY_ETAS_BOA::FlexRAY_GetControllerCapabilities(s_FLXControllerCapabilities& ouFlexRayCapabilities)
{
    ouFlexRayCapabilities.m_nMaximumTxBuffers = 128;
    ouFlexRayCapabilities.m_bSRSupport = false;         //TODO::Has to be Updated
    return S_OK;
}

HRESULT CDIL_FLEXRAY_ETAS_BOA::FLEXRAY_SetAppParams(HWND hWndOwner, Base_WrapperErrorLogger* pILog)
{
    HRESULT hResult = S_FALSE;

    bool bResult = (pILog != nullptr);

    if (bResult)
    {
        sg_hOwnerWnd = hWndOwner;
        sg_pIlog = pILog;
        vInitialiseAllData();

        hResult = S_OK;
    }
    else
    {
        sg_acErrStr = "Null argument value(s) in SetAppParams";
        sg_hLastError = hResult;
    }

    return hResult;
}

/*HRESULT CDIL_FLEXRAY_ETAS_BOA::FLEXRAY_RegisterClient(BOOL bRegister, DWORD& ClientID, char* pacClientName)
{
    HRESULT hResult;
    if (bRegister)
    {
        hResult = m_ClientList.RegisterClient(ClientID, pacClientName);
    }
    else
    {
        hResult = m_ClientList.RemoveClient(ClientID);
    }
    return hResult;
}
*/
/*HRESULT CDIL_FLEXRAY_ETAS_BOA::FLEXRAY_ManageMsgBuf(BYTE byAction, DWORD ClientID, CBaseFLEXBufFSE* pBufObj)
{
    HRESULT hResult = S_FALSE;
    if (pBufObj)
    {
        CClientBuffer* pClientBuf = m_ClientList.GetClientByID(ClientID);
        if (pClientBuf)
        {
            if (byAction == MSGBUF_ADD)
            {
                hResult = pClientBuf->AddMsgBuf(pBufObj);
            }
            else if (byAction == MSGBUF_CLEAR)
            {
                if (pBufObj != nullptr)
                {
                    hResult = pClientBuf->RemoveMsgBuf(pBufObj);
                }
                else
                {
                    // remove all message buffers
                    hResult = pClientBuf->RemoveAllMsgBuf();
                }
            }
        }
        else
        {
            hResult = ERR_NO_CLIENT_EXIST;
        }
    }
    else
    {
        if (byAction == MSGBUF_CLEAR)
        {
            hResult = m_ClientList.RemoveAllMsgBufOnAllClients();
        }
    }
    return hResult;
}
*/
HRESULT CDIL_FLEXRAY_ETAS_BOA::FLEXRAY_LoadDriverLibrary(void)
{
    HRESULT hResult = S_FALSE;
    std::string acPath;
    /* Get BOA installation path from the registery */
    bGetBOAInstallationPath(acPath);
    /* Load cslproxy.dll library */
    std::string acLIB_CSL = "";
    acLIB_CSL.append(acPath);
    acLIB_CSL.append("\\");
    acLIB_CSL.append(LIB_CSL_NAME);
    /* LoadLibraryEx instead of LoadLibrary seems to be necessary under Windows 7 when the library is not in DLL search path (system32) */
    sg_hLibCSI = LoadLibraryEx(acLIB_CSL.c_str(), nullptr, LOAD_WITH_ALTERED_SEARCH_PATH);

    if (sg_hLibCSI != nullptr)
    {
        hResult = GetCSI_API_Pointers(sg_hLibCSI);

        /* Load the OCI library to use CAN controller */
        if (hResult == S_OK)
        {
            std::string acLIB_OCI;
            acLIB_OCI.append(acPath);
            acLIB_OCI.append("\\");
            acLIB_OCI.append(LIB_OCI_NAME);
            sg_hLibOCI = LoadLibraryEx(acLIB_OCI.c_str(), nullptr, LOAD_WITH_ALTERED_SEARCH_PATH);

            if (sg_hLibOCI != nullptr)
            {
                hResult = GetOCI_API_Pointers(sg_hLibOCI);
                if (hResult != S_OK)
                {
                    sg_pIlog->vLogAMessage(A2T(__FILE__), __LINE__, _("could not get OCI function pointers"));
                }
                else
                {
                    sg_bIsDriverRunning = TRUE;
                }
            }
            else
            {
                hResult = S_FALSE;
                std::string acErr;
                acErr.append(acLIB_OCI);
                acErr.append(" failed to load");
                sg_pIlog->vLogAMessage(A2T(__FILE__), __LINE__, acErr);
                sg_pIlog->vLogAMessage(A2T(__FILE__), __LINE__, _("Please have a look at: https://github.com/rbei-etas/busmaster/wiki/Hardware-support"));
            }
        }
        else
        {
            sg_pIlog->vLogAMessage(A2T(__FILE__), __LINE__, _("could not get CSI function pointers"));
        }
    }
    else
    {
        std::string acErr;
        acErr.append(acLIB_CSL);
        acErr.append(_(" failed to load"));
        sg_pIlog->vLogAMessage(A2T(__FILE__), __LINE__, acErr);
        sg_pIlog->vLogAMessage(A2T(__FILE__), __LINE__, _("Please have a look at: https://github.com/rbei-etas/busmaster/wiki/Hardware-support"));
    }
    return hResult;
}

HRESULT CDIL_FLEXRAY_ETAS_BOA::FLEXRAY_UnloadDriverLibrary(void)
{
    /* Unload OCI library */
    if (sg_hLibOCI != nullptr)
    {
        FreeLibrary(sg_hLibOCI);
    }
    /* Unload CSI library */
    if (sg_hLibCSI != nullptr)
    {
        FreeLibrary(sg_hLibCSI);
    }

    /* Invalidate all API pointers */
    memset(&sBOA_PTRS, 0, sizeof (SBOA_POINTER_TABLE));
    return S_OK;
}

HRESULT CDIL_FLEXRAY_ETAS_BOA::FLEXRAY_PerformInitOperations(void)
{

    /* Create critical section for ensuring thread
       safeness of read message function */
    InitializeCriticalSection(&sg_DIL_CriticalSection);
    /* Create an event for timestamp calculations*/
    sg_hEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);
    /*Initial Channel info with default values.
      1. Controller configuration
      2. Queue configuration
      3. Filter configuration*/

    for (UINT i = 0; i < defNO_OF_CHANNELS; i++)
    {
        /* Controller configuration default values*/
        vInitializeControllerConfig(i);
        /* Queue configuration default values*/
        vInitializeQueueConfig(i);
        /* Filter configuration default values*/
        vInitializeFilterConfig(i);
    }
    //Register monitor client
    DWORD dwClient = 0;
    FLEXRAY_RegisterClient(TRUE, dwClient, FLEXRAY_MONITER_NODE);

    //Initialize the selected channel items array to -1
    for ( UINT i = 0; i< CHANNEL_ALLOWED; i++ )
    {
        sg_anSelectedItems[i] = -1;
    }

    return S_OK;
}

HRESULT CDIL_FLEXRAY_ETAS_BOA::FLEXRAY_PerformClosureOperations(void)
{
    FLEXRAY_StopHardware();
    FLEXRAY_DeselectHwInterface();
    for ( int i = 0 ; i < sg_nNoOfChannels; i++ )
    {
        sg_asChannel[i].m_ouDataTransmitThread.m_unActionCode = EXIT_THREAD;
        sg_asChannel[i].m_ouDataTransmitThread.bTerminateThread();
        Sleep(500);
        FLEXRAY_UnloadDriverLibrary();
    }

    return S_OK;
}

HRESULT CDIL_FLEXRAY_ETAS_BOA::FLEXRAY_GetTimeModeMapping(SYSTEMTIME& CurrSysTime, UINT64& TimeStamp)
{

    memcpy(&CurrSysTime, &sg_CurrSysTime, sizeof(SYSTEMTIME));
    TimeStamp = sg_TimeStamp;

    return S_OK;
}

HRESULT CDIL_FLEXRAY_ETAS_BOA::FLEXRAY_ListHwInterfaces(FLEXRAY_INTERFACE_HW & sSelHwInterface, FLEXRAY_CONFIG_FILES & /* sAvailableConfigFiles */)
{
    VALIDATE_VALUE_RETURN_VAL(sg_bCurrState, STATE_DRIVER_SELECTED, ERR_IMPROPER_STATE);

    /*if(sAvailableConfigFiles.m_nFilesCount == 0)
    {
        AfxMessageBox("Load a FIBEX file and then select the hardware");
    }
    else*/
    {
        int nCount = CHANNEL_ALLOWED;
        USES_CONVERSION;
        HRESULT hResult = S_OK;
        OCI_URIName acURI[defNO_OF_CHANNELS];
        uint32 nFound = 0;

        UINT unDefaultChannelCnt = nCount;

        if (OCI_FindFlexRayController(acURI, defNO_OF_CHANNELS, &nFound) == OCI_SUCCESS)
        {
            nCount = nFound;
            qsort((void*)acURI, nCount, sizeof(OCI_URIName), nCallBackStrCompareFn);

            if (nCount > 0)//Success only if there exists alteast one hw
            {
                INTERFACE_HW psHWInterface[defNO_OF_CHANNELS];
                //set the current number of channels
                nCount = min(nCount, defNO_OF_CHANNELS);

                for (UINT i = 0; i < nCount; i++)
                {
                    psHWInterface[i].m_dwIdInterface = 0;
                    psHWInterface[i].m_dwVendor = 0;
                    psHWInterface[i].m_acDeviceName = "";
                    psHWInterface[i].m_acNameInterface = acURI[i];
                    psHWInterface[i].m_acDescription = acURI[i];

#if BOA_VERSION >= BOA_VERSION_2_0
                    if ((sBOA_PTRS.createFlexRayController  = (PF_OCI_CreateFlexRayControllerVersion)
                            GetProcAddress(sg_hLibOCI, "OCI_CreateFlexRayControllerVersion")) == nullptr)
                    {
                        hResult = S_FALSE;
                    }
#else
                    if ((sBOA_PTRS.m_sOCI.createFlexRayController = (PF_OCI_CreateFlexRayController)
                            GetProcAddress(sg_hLibOCI, "OCI_CreateFlexRayController")) == nullptr)
                    {
                        hResult = S_FALSE;
                    }
#endif
                    if (hResult == S_OK)
                    {
                        OCI_ControllerHandle ouHandle;
                        BOA_ResultCode err = OCI_FAILURE;
                        BOA_Version version = {1,3,0,0};
                        BOA_Version version1 = {1,1,0,0};
#if BOA_VERSION >= BOA_VERSION_2_0
                        err =  (*(sBOA_PTRS.createFlexRayController))( acURI[i], &version, &ouHandle);
                        if(BOA_FAILED(err)  )
                        {
                            err = (*(sBOA_PTRS.createFlexRayController))( acURI[i], &version1, &ouHandle);
                        }
#else
                        err =  (*(sBOA_PTRS.m_sOCI.createFlexRayController))( acURI[i], &ouHandle);
#endif
                        if (err == OCI_SUCCESS)
                        {
                            OCI_FlexRayControllerCapabilities  ouCapabilities;
                            hResult = (*(sBOA_PTRS.m_sOCI.flexRayIOVTable.getFlexRayControllerCapabilities))(ouHandle, &ouCapabilities);

                            if ( ouCapabilities.supportedBusMode & OCI_FLEXRAY_BUSMODE_SELFRECEPTION )
                            {
                                psHWInterface[i].m_acAdditionalInfo = "";
                            }
                            else
                            {
                                psHWInterface[i].m_acAdditionalInfo = "Selected Channel will NOT support Self Recieption.So BUSMASTER will not display the Tx messages";
                            }


                            int  i = 0 ;
                            hResult = (*(sBOA_PTRS.m_sOCI.destroyFlexRayController))(ouHandle);

                        }

                    }
                }

                if (nCount > 1)// List hw interface if there are more than one hw
                {
                    /* If the default channel count parameter is set, prevent displaying the hardware selection dialog */
                    if ( unDefaultChannelCnt && nCount >= unDefaultChannelCnt )
                    {
                        for (UINT i = 0; i < unDefaultChannelCnt; i++)
                        {
                            sg_anSelectedItems[i] = i;
                        }
                        nCount  = unDefaultChannelCnt;
                    }
                    else if ( ListHardwareInterfaces(nullptr, FLEXRAY_DRIVER_ETAS_BOA, psHWInterface, sg_anSelectedItems, nCount) != 0 )
                    {
                        /* return if user cancels hardware selection */
                        return HW_INTERFACE_NO_SEL;
                    }
                }
                else if ( nCount == 1)
                {
                    sg_anSelectedItems[0] = 0;
                }

                sg_nNoOfChannels = min(nCount, defNO_OF_CHANNELS);
                //for (UINT nList = 0; nList < sg_nNoOfChannels; nList++)
                //{
                //  sSelHwInterface[nList].m_acNameInterface = psHWInterface[sg_anSelectedItems[nList]].m_acNameInterface;
                //  sSelHwInterface[nList].m_acDescription = psHWInterface[sg_anSelectedItems[nList]].m_acDescription;
                //  sSelHwInterface[nList].m_dwIdInterface = 100 + nList; // Give a dummy number
                //}

                strcpy(sSelHwInterface.m_acNameInterface, psHWInterface[sg_anSelectedItems[0]].m_acNameInterface.c_str());
                strcpy(sSelHwInterface.m_acDescription , psHWInterface[sg_anSelectedItems[0]].m_acNameInterface.c_str());
                sSelHwInterface.m_dwIdInterface = 100 ; // Give a dummy number

                sg_bCurrState = STATE_HW_INTERFACE_LISTED;
                hResult = S_OK;
            }
            else
            {
                hResult = NO_HW_INTERFACE;
            }
        }
        return hResult;
    }
}

HRESULT CDIL_FLEXRAY_ETAS_BOA::FLEXRAY_SelectHwInterface(const FLEXRAY_INTERFACE_HW_LIST& sSelHwInterface)
{
    VALIDATE_VALUE_RETURN_VAL(sg_bCurrState, STATE_HW_INTERFACE_LISTED, ERR_IMPROPER_STATE);

    USES_CONVERSION;
    HRESULT hResult = S_OK;
    //First select only supported number of HW interfaces
    for (UINT i = 0; i < sg_nNoOfChannels; i++)
    {
        strcpy(sg_asChannel[i].m_acURI, sSelHwInterface[i].m_acNameInterface);
    }

    // Create the controller instance.
    for (UINT i = 0; i < sg_nNoOfChannels; i++)
    {
        BOA_ResultCode err = OCI_FAILURE;

        BOA_Version version = {1,3,0,0};
        BOA_Version version1 = {1,1,0,0};
#if BOA_VERSION >= BOA_VERSION_2_0
        err =  (*(sBOA_PTRS.createFlexRayController))(sg_asChannel[i].m_acURI,
                &version, &sg_asChannel[i].m_OCI_HwHandle);
        if(BOA_FAILED(err)  )
        {
            err = (*(sBOA_PTRS.createFlexRayController))(sg_asChannel[i].m_acURI,
                    &version1, &sg_asChannel[i].m_OCI_HwHandle);
        }
#else
        err =  (*(sBOA_PTRS.m_sOCI.createFlexRayController))(sg_asChannel[i].m_acURI,
                &sg_asChannel[i].m_OCI_HwHandle);
#endif
        if (err == OCI_SUCCESS)
        {
            hResult = (*(sBOA_PTRS.m_sOCI.timeVTable.getTimerCapabilities))(sg_asChannel[i].m_OCI_HwHandle,
                      &(sg_asChannel[i].m_OCI_TimerCapabilities));
            if ( hResult == OCI_SUCCESS )
            {
                sg_asChannel[i].m_fResolution = (float)10000 /(float)(sg_asChannel[i].m_OCI_TimerCapabilities.tickFrequency);
            }

            hResult = (*(sBOA_PTRS.m_sOCI.flexRayIOVTable.getFlexRayControllerCapabilities))(sg_asChannel[i].m_OCI_HwHandle, &sg_asChannel[i].m_ControllerCapabilities);

            hResult = S_OK;
        }
        else
        {
            hResult = ERR_LOAD_HW_INTERFACE;
            sg_pIlog->vLogAMessage(A2T(__FILE__), __LINE__, _("could not create controller"));
        }
    }
    //Check for the success
    if (hResult == S_OK)
    {
        sg_bCurrState = STATE_HW_INTERFACE_SELECTED;
    }
    return hResult;
}

HRESULT CDIL_FLEXRAY_ETAS_BOA::FLEXRAY_DeselectHwInterface(void)
{
    VALIDATE_VALUE_RETURN_VAL(sg_bCurrState, STATE_HW_INTERFACE_SELECTED, ERR_IMPROPER_STATE);

    HRESULT hResult = S_OK;
    for (UINT i = 0; i < sg_nNoOfChannels; i++)
    {
        if ((hResult = ManageFilters(FILTER_REMOVE, i)) == S_OK)
        {
            if ((hResult = ManageQueue(QUEUE_DESTROY, i)) == S_OK)
            {
                if ((*(sBOA_PTRS.m_sOCI.closeFlexRayController))(sg_asChannel[i].m_OCI_HwHandle) == OCI_SUCCESS)
                {
                    if ((*(sBOA_PTRS.m_sOCI.destroyFlexRayController))(sg_asChannel[i].m_OCI_HwHandle) == OCI_SUCCESS)
                    {
                        sg_asChannel[i].m_ouDataTransmitThread.bTerminateThread();
                        hResult |= S_OK;
                    }
                    else
                    {
                        hResult = S_FALSE;
                        sg_pIlog->vLogAMessage(A2T(__FILE__), __LINE__, _("could not destroy controller"));
                    }
                }
                else
                {
                    hResult = S_FALSE;
                    sg_pIlog->vLogAMessage(A2T(__FILE__), __LINE__, _("could not close controller"));
                }
            }
            else
            {
                hResult = S_FALSE;
                sg_pIlog->vLogAMessage(A2T(__FILE__), __LINE__, _("could not destroy the queue"));
            }
        }
        else
        {
            hResult = S_FALSE;
            sg_pIlog->vLogAMessage(A2T(__FILE__), __LINE__, _("could not remove the filter"));
        }
    }

    if (hResult == S_OK)
    {
        sg_bCurrState = STATE_HW_INTERFACE_LISTED;
    }
    return hResult;
}

HRESULT CDIL_FLEXRAY_ETAS_BOA::FLEXRAY_DisplayConfigDlg(PCHAR & /* InitData */, int & /* Length */)
{
    return S_OK;
}

HRESULT CDIL_FLEXRAY_ETAS_BOA::FLEXRAY_SetConfigData(CHANNEL_CONFIG& ouAbsSFibexContainer)
{
    HRESULT hResult = WARNING_NOTCONFIRMED;
    m_mapSlotClient.clear();
    //bLoadDataFromContr(asDeviceConfig);           //just to test, delete it later
    VALIDATE_VALUE_RETURN_VAL(sg_bCurrState, STATE_HW_INTERFACE_SELECTED, ERR_IMPROPER_STATE);

    BOA_ResultCode ErrCode = OCI_FAILURE;
    OCI_ErrorCode   OciErrorCode =  OCI_FAILURE;
    //PSCONTROLLER_DETAILS pControllerDetails = (PSCONTROLLER_DETAILS)pInitData;
    bLoadDataFromContr(ouAbsSFibexContainer);



    for (UINT i = 0; i < sg_nNoOfChannels; i++)
    {
        //First remove all the filters
        ManageFilters(FILTER_REMOVE, 0);
        //First remove all the Rx Tx queues
        //ManageQueue(QUEUE_DESTROY, 0);
        // if controller is open, close the controller. Do not bother about return value
        ErrCode = (*sBOA_PTRS.m_sOCI.closeFlexRayController)(sg_asChannel[i].m_OCI_HwHandle);

        //Now load the controller config and open the controller
        OciErrorCode = (*sBOA_PTRS.m_sOCI.openFlexRayControllerEx)(sg_asChannel[i].m_OCI_HwHandle,
                       &(sg_asChannel[i].m_OCI_FlexRayConfig),
                       &(sg_asChannel[i].m_OCI_CntrlProp));

        /* Fill the hardware description details */
        /*((PSCONTROLLER_DETAILS)pInitData)[i].m_omHardwareDesc =
            sg_asChannel[i].m_acURI;*/

        if (OciErrorCode == OCI_SUCCESS)
        {
            // Rx Tx queue
            if (ManageQueue(QUEUE_ADD, i) == S_OK)
            {
                if (ManageFilters(FILTER_ADD, i) == S_OK)
                {
                    hResult = S_OK;
                }
            }
        }
        else
        {
            hResult = S_FALSE;
            sg_hLastError = OciErrorCode;
            sg_pIlog->vLogAMessage(A2T(__FILE__), __LINE__, _("could not configure the controller"));
        }
    }
    return hResult;
}

HRESULT CDIL_FLEXRAY_ETAS_BOA::FLEXRAY_StartHardware(void)
{

    VALIDATE_VALUE_RETURN_VAL(sg_bCurrState, STATE_HW_INTERFACE_SELECTED, ERR_IMPROPER_STATE);

    HRESULT hResult = S_OK;
    OCI_ErrorCode OciErrorCode = OCI_FAILURE;
    for (UINT i = 0; i < sg_nNoOfChannels; i++)
    {
        OciErrorCode = (*(sBOA_PTRS.m_sOCI.getFlexRayControllerProperties))(sg_asChannel[i].m_OCI_HwHandle,
                       &(sg_asChannel[i].m_OCI_CntrlProp));
        if ( OciErrorCode == OCI_SUCCESS)
        {
            if ((sg_asChannel[i].m_OCI_CntrlProp.mode == OCI_CONTROLLER_MODE_SUSPENDED)
                    || (sg_asChannel[i].m_OCI_CntrlProp.mode == OCI_CONTROLLER_MODE_RUNNING))
            {
                sg_asChannel[i].m_OCI_CntrlProp.mode = OCI_CONTROLLER_MODE_RUNNING;
                OciErrorCode = (*(sBOA_PTRS.m_sOCI.setFlexRayControllerProperties))(sg_asChannel[i].m_OCI_HwHandle,
                               &(sg_asChannel[i].m_OCI_CntrlProp));
                if ( OciErrorCode == OCI_SUCCESS)
                {
                    //Start Write Thread
                    sg_asChannel[i].m_ouDataTransmitThread.m_unActionCode = INACTION;
                    if ( sg_asChannel[i].m_ouDataTransmitThread.m_hActionEvent == nullptr )
                    {
                        sg_asChannel[i].m_ouDataTransmitThread.m_hActionEvent = CreateEvent(nullptr, FALSE, TRUE, nullptr);
                    }
                    sg_asChannel[i].m_ouDataTransmitThread.m_pBuffer = &sg_asChannel[i];

                    sg_asChannel[i].m_ouDataTransmitThread.bStartThread(DataTransmitThread);
                    SetEvent(sg_asChannel[i].m_ouDataTransmitThread.m_hActionEvent);

                    hResult |= S_OK;
                }
                else
                {
                    sg_hLastError = OciErrorCode;
                    hResult = S_FALSE;
                    sg_pIlog->vLogAMessage(A2T(__FILE__), __LINE__, _("could not start the controller in running mode"));
                }
            }
        }
        else
        {
            sg_hLastError = OciErrorCode;
            hResult = S_FALSE;
            sg_pIlog->vLogAMessage(A2T(__FILE__), __LINE__, _("could not get controller mode"));
        }
    }
    //Check for the success
    if (hResult == S_OK)
    {
        sg_bCurrState = STATE_CONNECTED;
        SetEvent(sg_hEvent);
        vCreateTimeModeMapping(sg_hEvent);

        InitializeCriticalSection(&sg_CritSectForAckBuf);


        QueryPerformanceCounter(&sg_QueryTickCount);
        // Get frequency of the performance counter
        QueryPerformanceFrequency(&sg_lnFrequency);
        // Convert it to time stamp with the granularity of hundreds of microsecond
        if ((sg_QueryTickCount.QuadPart * 10000) > sg_lnFrequency.QuadPart)
        {
            sg_TimeStamp = (sg_QueryTickCount.QuadPart * 10000) / sg_lnFrequency.QuadPart;
        }
        else
        {
            sg_TimeStamp = (sg_QueryTickCount.QuadPart / sg_lnFrequency.QuadPart) * 10000;
        }
    }

    return hResult;
}

HRESULT CDIL_FLEXRAY_ETAS_BOA::FLEXRAY_StopHardware(void)
{
    VALIDATE_VALUE_RETURN_VAL(sg_bCurrState, STATE_CONNECTED, ERR_IMPROPER_STATE);

    HRESULT hResult = S_OK;
    OCI_ErrorCode ociErrorCode = OCI_FAILURE;
    for (UINT i = 0; i < sg_nNoOfChannels; i++)
    {
        ociErrorCode = (*(sBOA_PTRS.m_sOCI.getFlexRayControllerProperties))(sg_asChannel[i].m_OCI_HwHandle,
                       &(sg_asChannel[i].m_OCI_CntrlProp));
        if (ociErrorCode == OCI_SUCCESS)
        {
            if (sg_asChannel[i].m_OCI_CntrlProp.mode == OCI_CONTROLLER_MODE_RUNNING)
            {
                sg_asChannel[i].m_OCI_CntrlProp.mode = OCI_CONTROLLER_MODE_SUSPENDED;
                ociErrorCode == (*(sBOA_PTRS.m_sOCI.setFlexRayControllerProperties))(sg_asChannel[i].m_OCI_HwHandle,
                        &(sg_asChannel[i].m_OCI_CntrlProp));
                if (ociErrorCode == OCI_SUCCESS)
                {
                    hResult |= S_OK;

                    //Clear the Data
                    EnterCriticalSection(&sg_asChannel[i].m_ouCriticalSection);
                    std::map<SLOT_BASECYCLE, BOA_FlexRayData>::iterator itrBoaData = sg_asChannel[i].m_ouBoaFlexRayData.begin();
                    for ( ; itrBoaData != sg_asChannel[i].m_ouBoaFlexRayData.end(); itrBoaData++ )
                    {
                        //No use of Return value;
                        (*(sBOA_PTRS.m_sOCI.flexRayIOVTable.destroyFlexRayTxQueue))(itrBoaData->second.m_OCI_NewTxQueueHandle);
                    }
                    sg_asChannel[i].m_ouBoaFlexRayData.clear();
                    LeaveCriticalSection(&sg_asChannel[i].m_ouCriticalSection);

                    //Stop Write Thread
                    sg_asChannel[i].m_ouDataTransmitThread.m_unActionCode = SUSPEND;
                    if ( sg_asChannel[i].m_ouDataTransmitThread.m_hActionEvent == nullptr )
                    {
                        SetEvent(sg_asChannel[i].m_ouDataTransmitThread.m_hActionEvent);
                    }
                    sg_asChannel[i].m_ouDataTransmitThread.m_pBuffer = &sg_asChannel[i];

                }
                else
                {
                    sg_hLastError = ociErrorCode;
                    hResult = S_FALSE;
                    sg_pIlog->vLogAMessage(A2T(__FILE__), __LINE__, _("could not stop the controller in suspended mode"));
                }
            }
            else if (sg_asChannel[i].m_OCI_CntrlProp.mode == OCI_CONTROLLER_MODE_SUSPENDED)
            {
                hResult |= S_OK;
            }
        }
        else
        {
            sg_hLastError = ociErrorCode;
            hResult = S_FALSE;
            sg_pIlog->vLogAMessage(A2T(__FILE__), __LINE__, _("could not get controller mode"));
        }
    }
    //Check for the success
    if (hResult == S_OK)
    {
        sg_byCurrState = CREATE_MAP_TIMESTAMP;
        sg_bCurrState = STATE_HW_INTERFACE_SELECTED;
        DeleteCriticalSection(&sg_CritSectForAckBuf);
    }

    return hResult;
}

HRESULT CDIL_FLEXRAY_ETAS_BOA::FLEXRAY_ResetHardware(void)
{

    return S_OK;
}

HRESULT CDIL_FLEXRAY_ETAS_BOA::FLEXRAY_GetCurrStatus(s_FLXSTATUSMSG& StatusData)
{
    StatusData.wControllerStatus = NORMAL_ACTIVE;

    return S_OK;
}

HRESULT CDIL_FLEXRAY_ETAS_BOA::FLEXRAY_GetFlxTxMsgBuffer(CFlxTxMsgBuffer*& pouFlxTxMsgBuffer)
{
    pouFlxTxMsgBuffer = &sg_ouFlxTxList;
    return S_OK;
}

HRESULT CDIL_FLEXRAY_ETAS_BOA::FLEXRAY_DeleteMsg(DWORD /* dwClientID */, s_FLXTXMSG * pouFlxTxMsg)
{
    //VALIDATE_VALUE_RETURN_VAL(sg_bCurrState, STATE_CONNECTED, ERR_IMPROPER_STATE);
    HRESULT hResult = S_FALSE;

    EnterCriticalSection(&sg_asChannel[0].m_ouCriticalSection);
    SLOT_BASECYCLE nSlotBase = MAKELONG( pouFlxTxMsg->m_sFlxMsg.m_nSlotID, MAKEWORD(pouFlxTxMsg->m_sFlxMsg.m_nBaseCycle, pouFlxTxMsg->m_sFlxMsg.m_eChannel) );
    std::map<SLOT_BASECYCLE, BOA_FlexRayData>::iterator itrMap = sg_asChannel[0].m_ouBoaFlexRayData.find(nSlotBase);


    //Already Created in Buffers.

    if ( itrMap != sg_asChannel[0].m_ouBoaFlexRayData.end() )
    {
        (*(sBOA_PTRS.m_sOCI.flexRayIOVTable.destroyFlexRayTxQueue))(itrMap->second.m_OCI_NewTxQueueHandle);
        sg_asChannel[0].m_ouBoaFlexRayData.erase(itrMap);
    }

    LeaveCriticalSection(&sg_asChannel[0].m_ouCriticalSection);

    return S_OK;
}

HRESULT CDIL_FLEXRAY_ETAS_BOA::FLEXRAY_SendMsg(DWORD /* dwClientID */, s_FLXTXMSG * pouFlxTxMsg)
{
    //VALIDATE_VALUE_RETURN_VAL(sg_bCurrState, STATE_CONNECTED, ERR_IMPROPER_STATE);
    HRESULT hResult = S_FALSE;

    EnterCriticalSection(&sg_asChannel[0].m_ouCriticalSection);
    SLOT_BASECYCLE nSlotBase = MAKELONG( pouFlxTxMsg->m_sFlxMsg.m_nSlotID, MAKEWORD(pouFlxTxMsg->m_sFlxMsg.m_nBaseCycle, pouFlxTxMsg->m_sFlxMsg.m_eChannel) );
    std::map<SLOT_BASECYCLE, BOA_FlexRayData>::iterator itrMap = sg_asChannel[0].m_ouBoaFlexRayData.find(nSlotBase);


    //Already Created in Buffers.

    if ( itrMap != sg_asChannel[0].m_ouBoaFlexRayData.end() )
    {
        itrMap->second.m_OCI_FlexRayMessageArray.type = OCI_FLEXRAY_TX_MESSAGE;
        itrMap->second.m_OCI_FlexRayMessageArray.data.txMessage.size =  pouFlxTxMsg->m_sFlxMsg.m_nDLC;
        memcpy(itrMap->second.m_OCI_FlexRayMessageArray.data.txMessage.data, pouFlxTxMsg->m_sFlxMsg.m_ucData, pouFlxTxMsg->m_sFlxMsg.m_nDLC);
    }
    else    //New Message ReCreate the Tx Queue
    {
        std::map<INT, INT> ::iterator itrSlotMap = sg_asChannel[0].m_SlotBufferMap.find(nSlotBase);
        if ( itrSlotMap != sg_asChannel[0].m_SlotBufferMap.end() )
        {
            BOA_FlexRayData ouBoaFlexRayData;
            memcpy(ouBoaFlexRayData.m_OCI_FlexRayMessageArray.data.txMessage.data, pouFlxTxMsg->m_sFlxMsg.m_ucData, pouFlxTxMsg->m_sFlxMsg.m_nDLC);
            ouBoaFlexRayData.m_OCI_FlexRayMessageArray.type = OCI_FLEXRAY_TX_MESSAGE;
            ouBoaFlexRayData.m_OCI_FlexRayMessageArray.data.txMessage.size =  pouFlxTxMsg->m_sFlxMsg.m_nDLC;

            ouBoaFlexRayData.m_OCI_NewTxQueueCfg.count = 1;
            ouBoaFlexRayData.m_OCI_NewTxQueueCfg.bufferIndexes[0] = itrSlotMap->second;

            OCI_ErrorCode Err = (*(sBOA_PTRS.m_sOCI.flexRayIOVTable.createFlexRayTxQueue))
                                (sg_asChannel[0].m_OCI_HwHandle,
                                 &(ouBoaFlexRayData.m_OCI_NewTxQueueCfg),
                                 &(ouBoaFlexRayData.m_OCI_NewTxQueueHandle));
            if (Err != OCI_SUCCESS)
            {
                sg_hLastError = Err;
                hResult = S_FALSE;
                sg_pIlog->vLogAMessage(A2T(__FILE__), __LINE__, _("could not create TX Queue"));
            }
            else
            {
                sg_asChannel[0].m_ouBoaFlexRayData[nSlotBase] = ouBoaFlexRayData;
            }

        }
    }
    LeaveCriticalSection(&sg_asChannel[0].m_ouCriticalSection);

    return S_OK;
}
int CDIL_FLEXRAY_ETAS_BOA::nCreateTxQueue()
{
    //Create Tx Queue
    std::map<SLOT_BASECYCLE, int>::iterator itrMap =  sg_asChannel[0].m_ouFlexrayData.m_mapSlotBaseCycle.begin();
    sg_asChannel[0].m_OCI_TxQueueCfg.count = 0;

    while ( itrMap !=  sg_asChannel[0].m_ouFlexrayData.m_mapSlotBaseCycle.end() )
    {
        std::map<INT, INT> ::iterator itrSlotMap = sg_asChannel[0].m_SlotBufferMap.find(itrMap->first);
        if ( itrSlotMap != sg_asChannel[0].m_SlotBufferMap.end())
        {
            sg_asChannel[0].m_OCI_TxQueueCfg.bufferIndexes[sg_asChannel[0].m_OCI_TxQueueCfg.count] = itrSlotMap->second;
            //sg_asChannel[0].m_OCI_TxQueueCfg.bufferIndexes[/*sg_asChannel[0].m_OCI_TxQueueCfg.count*/itrMap->second] = itrSlotMap->second;
            sg_asChannel[0].m_OCI_TxQueueCfg.count++;
        }
        itrMap++;
    }


    HRESULT hResult = S_FALSE;
    BOA_ResultCode Err = OCI_ERR_FLAG_ERROR;

    //First Destroy
    Err = (*(sBOA_PTRS.m_sOCI.flexRayIOVTable.destroyFlexRayTxQueue))(sg_asChannel[0].m_OCI_TxQueueHandle);
    if (Err != OCI_SUCCESS)
    {
        sg_hLastError = Err;

        //hResult = S_FALSE;
        sg_pIlog->vLogAMessage(A2T(__FILE__), __LINE__, _("could not create TX Queue"));
    }

    //Create
    Err = (*(sBOA_PTRS.m_sOCI.flexRayIOVTable.createFlexRayTxQueue))
          (sg_asChannel[0].m_OCI_HwHandle,
           &(sg_asChannel[0].m_OCI_TxQueueCfg),
           &(sg_asChannel[0].m_OCI_TxQueueHandle));
    if (Err != OCI_SUCCESS)
    {
        sg_hLastError = Err;

        hResult = S_FALSE;
        sg_pIlog->vLogAMessage(A2T(__FILE__), __LINE__, _("could not create TX Queue"));
    }

    return Err;
}

HRESULT CDIL_FLEXRAY_ETAS_BOA::FLEXRAY_GetBoardInfo(s_BOARDINFO & /* BoardInfo */)
{
    return S_OK;
}

HRESULT CDIL_FLEXRAY_ETAS_BOA::FLEXRAY_GetFlexRayInfo(s_FLXINFO & /* FlexRayInfo */)
{
    return S_OK;
}

HRESULT CDIL_FLEXRAY_ETAS_BOA::FLEXRAY_GetVersionInfo(VERSIONINFO & /* sVerInfo */)
{
    return S_OK;
}

HRESULT CDIL_FLEXRAY_ETAS_BOA::FLEXRAY_GetLastErrorString(std::string acErrorStr, HRESULT & nError)
{
    acErrorStr = sg_acErrStr;
    nError = sg_hLastError;
    return S_OK;
}

HRESULT CDIL_FLEXRAY_ETAS_BOA::FLEXRAY_FilterFrames(FILTER_TYPE /* FilterType */, FLEX_CHANNEL /* FlexChannel */, UINT * /* punFrames */, UINT /* nLength */)
{
    return S_OK;
}

HRESULT CDIL_FLEXRAY_ETAS_BOA::FLEXRAY_ConfigMsgBuf(s_MSGBUF /* sMsgBuf */, UINT * /* punAddress */)
{
    return S_OK;
}

DWORD WINAPI DataTransmitThread(LPVOID pVoid)
{
    CPARAM_THREADPROC* pThreadParam = (CPARAM_THREADPROC*) pVoid;
    if (pThreadParam == nullptr)
    {
        return ((DWORD)-1);
    }
    SCHANNEL* sChannel = (SCHANNEL*)(pThreadParam->m_pBuffer);
    int m_unCycleTime = sChannel->m_unCycleTime/2000;
    bool bLoopON = true;

    while (bLoopON)
    {
        WaitForSingleObject(pThreadParam->m_hActionEvent, m_unCycleTime);
        switch (pThreadParam->m_unActionCode)
        {
            case INACTION :
            {
                m_unCycleTime = sChannel->m_unCycleTime/2000;
                pThreadParam->m_unActionCode  = INVOKE_FUNCTION;
            }
            break;
            case EXIT_THREAD:
            {
                bLoopON = false;
            }
            break;
            case SUSPEND:
            {
                m_unCycleTime = INFINITE;
            }
            break;
            case CREATE_TIME_MAP:
            {
                //Nothing at this moment
            }
            break;
            case INVOKE_FUNCTION:
            {
                BOA_ResultCode ErrCode;
                uint32 nRemaining = 0;
                EnterCriticalSection(&sg_asChannel[0].m_ouCriticalSection);
                if ( nullptr != g_pouDIL_FLEXRAY_BOA )
                {
                    //g_pouDIL_FLEXRAY_BOA->nCreateTxQueue();
                }


                std::map<SLOT_BASECYCLE, BOA_FlexRayData>::iterator itrMap = sg_asChannel[0].m_ouBoaFlexRayData.begin();
                for (; itrMap != sg_asChannel[0].m_ouBoaFlexRayData.end(); itrMap++ )
                {
                    ErrCode = (*(sBOA_PTRS.m_sOCI.flexRayIOVTable.writeFlexRayData))
                              (itrMap->second.m_OCI_NewTxQueueHandle, OCI_NO_TIME, &itrMap->second.m_OCI_FlexRayMessageArray, 1, &nRemaining);
                }



                //Sleep(5);
                LeaveCriticalSection(&sg_asChannel[0].m_ouCriticalSection);
            }
            break;
            default:
            {

            }
            break;
        }
    }
    SetEvent(pThreadParam->hGetExitNotifyEvent());
    return 0;
}

