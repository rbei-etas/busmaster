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
 * \file      LIN_ETAS_BOA.cpp
 * \brief     Source file for LIN ETAS BOA DIL functions
 * \author    Prathiba, Venkat
 * \copyright Copyright (c) 2011, ETAS GmbH. All rights reserved.
 *
 * Source file for LIN ETAS BOA DIL functions
 */
// CLIN_ETAS_BOA.cpp : Defines the initialization routines for the DLL.
//

#include "LIN_ETAS_BOA_stdafx.h"
#include "LIN_ETAS_BOA_Defs.h"
#include "include/Error.h"

/* C++ includes */
#include <sstream>
#include <string>
#include <vector>

/* Project includes */
#include "LIN_ETAS_BOA.h"
#include "DataTypes/Base_WrapperErrorLogger.h"
#include "DataTypes/DIL_Datatypes.h"
//#include "DataTypes/LIN_Datatypes.h"
#include "Utility/Utility_Thread.h"
#include "DataTypes/MsgBufAll_DataTypes.h"
#include "DIL_Interface/BaseDIL_LIN_Controller.h"
#include "Include/DIL_CommonDefs.h"
#include "DIL_Interface\ClientList.h"
#include "HardwareListing.h"
#include "DIL_Interface\CommanDIL_LIN.h"

#define USAGE_EXPORT
#include "LIN_ETAS_BOA_Extern.h"

void vInitializeQueueConfig(UINT nChannel);

BEGIN_MESSAGE_MAP(CLIN_ETAS_BOA, CWinApp)
END_MESSAGE_MAP()


/**
 * CLIN_ETAS_BOA construction
 */
CLIN_ETAS_BOA::CLIN_ETAS_BOA()
{
    // Place all significant initialization in InitInstance
}


// The one and only CLIN_ETAS_BOA object
CLIN_ETAS_BOA theApp;
const BYTE FILTER_ADD = 0x01;
const BYTE FILTER_REMOVE = 0x02;
const BYTE QUEUE_ADD = 0x01;
const BYTE QUEUE_DESTROY = 0x02;

/**
 * CLIN_ETAS_BOA initialization
 */

static HINSTANCE ghLangInst=nullptr;

BOOL CLIN_ETAS_BOA::InitInstance()
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
BOOL bGetBOAInstallationPath(std::string & pcPath)
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
        if ((sBOA_PTRS.m_sOCI.createLINController = (PF_OCI_CreateLINController)
                GetProcAddress(hLibOCI, "OCI_CreateLINController")) == nullptr)
        {
            hResult = S_FALSE;
        }
        if ((sBOA_PTRS.m_sOCI.destroyLINController = (PF_OCI_DestroyLINController )
                GetProcAddress(hLibOCI, "OCI_DestroyLINController")) == nullptr)
        {
            hResult = S_FALSE;
        }
        if ((sBOA_PTRS.m_sOCI.openLINController = (PF_OCI_OpenLINController)
                GetProcAddress(hLibOCI, "OCI_OpenLINController")) == nullptr)
        {
            hResult = S_FALSE;
        }

        if ((sBOA_PTRS.m_sOCI.closeLINController = (PF_OCI_CloseLINController)
                GetProcAddress(hLibOCI, "OCI_CloseLINController")) == nullptr)
        {
            hResult = S_FALSE;
        }
        if ((sBOA_PTRS.m_sOCI.getLINConfiguration = (PF_OCI_GetLINConfiguration)
                GetProcAddress(hLibOCI, "OCI_GetLINConfiguration")) == nullptr)
        {
            hResult = S_FALSE;
        }

        if ((sBOA_PTRS.m_sOCI.getLINControllerProperties = (PF_OCI_GetLINControllerProperties)
                GetProcAddress(hLibOCI, "OCI_GetLINControllerProperties")) == nullptr)
        {
            hResult = S_FALSE;
        }
        if ((sBOA_PTRS.m_sOCI.setLINControllerProperties = (PF_OCI_SetLINControllerProperties)
                GetProcAddress(hLibOCI, "OCI_SetLINControllerProperties")) == nullptr)
        {
            hResult = S_FALSE;
        }

        if ((sBOA_PTRS.m_sOCI.linioVTable.getLINControllerCapabilities = (PF_OCI_GetLINControllerCapabilities)
                GetProcAddress(hLibOCI, "OCI_GetLINControllerCapabilities")) == nullptr)
        {
            hResult = S_FALSE;
        }
        else if ((sBOA_PTRS.m_sOCI.linioVTable.getLINControllerStatus = (PF_OCI_GetLINControllerStatus)
                  GetProcAddress(hLibOCI, "OCI_GetCANControllerStatus")) == nullptr)
        {
            hResult = S_FALSE;
        }

        else if ((sBOA_PTRS.m_sOCI.linioVTable.createLINTxQueue = (PF_OCI_CreateLINTxQueue)
                  GetProcAddress(hLibOCI, "OCI_CreateLINTxQueue")) == nullptr)
        {
            hResult = S_FALSE;
        }
        else if ((sBOA_PTRS.m_sOCI.linioVTable.writeLINData = (PF_OCI_WriteLINData)
                  GetProcAddress(hLibOCI, "OCI_WriteLINData")) == nullptr)
        {
            hResult = S_FALSE;
        }
        else if ((sBOA_PTRS.m_sOCI.linioVTable.destroyLINTxQueue = (PF_OCI_DestroyLINTxQueue)
                  GetProcAddress(hLibOCI, "OCI_DestroyLINTxQueue")) == nullptr)
        {
            hResult = S_FALSE;
        }

        else if ((sBOA_PTRS.m_sOCI.linioVTable.createLINRxQueue = (PF_OCI_CreateLINRxQueue)
                  GetProcAddress(hLibOCI, "OCI_CreateLINRxQueue")) == nullptr)
        {
            hResult = S_FALSE;
        }
        else if ((sBOA_PTRS.m_sOCI.linioVTable.readLINData = (PF_OCI_ReadLINData)
                  GetProcAddress(hLibOCI, "OCI_ReadLINData")) == nullptr)
        {
            hResult = S_FALSE;
        }
        else if ((sBOA_PTRS.m_sOCI.linioVTable.destroyLINRxQueue= (PF_OCI_DestroyLINRxQueue)
                  GetProcAddress(hLibOCI, "OCI_DestroyLINRxQueue")) == nullptr)
        {
            hResult = S_FALSE;
        }

        else if ((sBOA_PTRS.m_sOCI.linioVTable.addLINFrameFilter = (PF_OCI_AddLINFrameFilter)
                  GetProcAddress(hLibOCI, "OCI_AddLINFrameFilter")) == nullptr)
        {
            hResult = S_FALSE;
        }
        else if ((sBOA_PTRS.m_sOCI.linioVTable.removeLINFrameFilter = (PF_OCI_RemoveLINFrameFilter)
                  GetProcAddress(hLibOCI, "OCI_RemoveLINFrameFilter")) == nullptr)
        {
            hResult = S_FALSE;
        }
        else if ((sBOA_PTRS.m_sOCI.linioVTable.addLINEventFilter = (PF_OCI_AddLINEventFilter)
                  GetProcAddress(hLibOCI, "OCI_AddLINEventFilter")) == nullptr)
        {
            hResult = S_FALSE;
        }
        else if ((sBOA_PTRS.m_sOCI.linioVTable.removeLINEventFilter = (PF_OCI_RemoveLINEventFilter)
                  GetProcAddress(hLibOCI, "OCI_RemoveLINEventFilter")) == nullptr)
        {
            hResult = S_FALSE;
        }
        /**/ //else if ((sBOA_PTRS.m_sOCI.LINIOVTable.addLINe = (PF_OCI_AddCANErrorFrameFilter)
        //          GetProcAddress(hLibOCI, "OCI_AddCANErrorFrameFilter")) == nullptr)
        //{
        //    hResult = S_FALSE;
        //}
        //else if ((sBOA_PTRS.m_sOCI.LINIOVTable.removeCANErrorFrameFilter = (PF_OCI_RemoveCANErrorFrameFilter)
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
 *  for all devces that support LIN.
 *
 *  @param[in]  sfsTree     Root of the tree to scan
 *  @param[in]  uriPrefix   The prefix which must be added to the URI name of the root of the tree in order to create
 *                          a complete URI.
 *  @param[out] uriNames    The array which will be filled with the URI locations of all the hardware
 *                          devices that support the OCI LIN interface.
 *  @param[in]  size        The size of the <b>uriNames</b> array.
 *  @param[out] position    The current entry in the <b>uriNames</b> array.
 */
void findLinNodes( CSI_Tree* sfsTree, OCI_URIName uriPrefix, OCI_URIName uriNames[], uint32 size, uint32* position )
{
    /* Uncomment the next line to get a view of the items in the tree */
    /* printf( "uriPrefix is %s; node is %s\n", uriPrefix, sfsTree->item.uriNames ); */

    /* Basic error checking */
    if ( !sfsTree || !uriNames || !uriPrefix || !position )
    {
        return;
        //exitWithMessage( "ERROR: parameter is nullptr", OCI_ERR_UNEXPECTED_NULL );
    }

    /* Does the current tree node have the URI name which begins with "LIN:"?
     * (Each node which represents a LIN port always has a URI name of the form "LIN:n". */
    if( 0 == strncmp( sfsTree->item.uriName, "LIN:", 4 ) )
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
            //exitWithMessage( "ERROR: Not enough space to list all connected devices.\n", OCI_ERR_OUT_OF_MEMORY );
        }
    }

    /* If the current tree node has a child, recurse into it */
    if (sfsTree->child)
    {
        OCI_URIName newUriPrefix;

        strcpy( newUriPrefix, uriPrefix );
        strcat( newUriPrefix, "/" );
        strcat( newUriPrefix, sfsTree->item.uriName );
        findLinNodes( sfsTree->child, newUriPrefix, uriNames, size, position );
    }
    /* If the current tree node has a sibling, recurse into it */
    if (sfsTree->sibling)
    {
        findLinNodes( sfsTree->sibling, uriPrefix, uriNames, size, position );
    }
}

/**
 * Search for all connected Hardware, that supports the OCI
 * LIN interface and deliver the URI location of the hardware.
 */
BOA_ResultCode OCI_FindLINController(OCI_URIName uriName[], INT nSize, uint32* nFound)
{
    //OCI_ErrorCode ec;

    ///* Container for search results */
    //CSI_Tree* sfsTree = nullptr;
    ///* Specify that we want to search for nodes which implement v1.1.0.0 of OCI_LIN. */
    //static const BOA_UuidVersion ociLINUuid = { UUID_OCILIN, {1, 0, 0, 0} };

    ///* Specify that we want to search for any kind of node, not just hardware nodes */
    //const CSI_NodeRange nodeRange = {CSI_NODE_MIN, CSI_NODE_MAX};

    ///* Search for all connected hardware and latch the result for further processing */
    //ec = (*(sBOA_PTRS.m_sCSI.createProtocolTree))("", nodeRange, &sfsTree);
    //if (ec == OCI_SUCCESS)
    //{
    //    /* Find the URIs for all nodes which implement v1.1.0.0 of OCI_CAN. */
    //    ec = (*(sBOA_PTRS.m_sCSI.getUriForUuid))(sfsTree, &ociLINUuid, uriName, nSize, nFound);
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
        //exitWithMessage( "ERROR: parameter \'found\' is nullptr", OCI_ERR_UNEXPECTED_NULL );
        return OCI_ERR_UNEXPECTED_NULL;
    }

    /* Search for all connected hardware and latch the result for further processing */
    ec = sBOA_PTRS.m_sCSI.createProtocolTree( "", nodeRange, &sfsTree );

    if ( OCI_FAILED( ec ) )
    {
        //exitWithMessage( "CSI_CreateProtocolTree failed! Error Code: 0x%x\n", ec );
        return ec;
    }

    /* Search the tree and fill array with the results */
    *nFound = 0;
    findLinNodes( sfsTree, uriPrefix, uriName, nSize, nFound );

    /* Clean up the protocol tree. */
    ec = sBOA_PTRS.m_sCSI.destroyProtocolTree( sfsTree );

    if ( OCI_FAILED( ec ) )
    {
        //exitWithMessage( "CSI_DestroyProtocolTree failed! Error Code: 0x%x\n", ec );
        return ec;
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
        Err = (*(sBOA_PTRS.m_sOCI.linioVTable.createLINRxQueue))
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

    }
    else if (byCode == QUEUE_DESTROY)
    {
        //Destroy CAN Rx queue
        Err = (*(sBOA_PTRS.m_sOCI.linioVTable.destroyLINRxQueue))(sg_asChannel[nChannel].m_OCI_RxQueueHandle);
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
            Err = (*(sBOA_PTRS.m_sOCI.linioVTable.destroyLINTxQueue))(sg_asChannel[nChannel].m_OCI_TxQueueHandle);
            if (Err != OCI_SUCCESS)
            {
                sg_hLastError = Err;

                hResult = S_FALSE;
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
        ErrCode = (*(sBOA_PTRS.m_sOCI.linioVTable.addLINFrameFilter))
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
            for(INT nEventFilter = OCI_LIN_EVENT_WAKEUP; nEventFilter <= OCI_LIN_ERR_DATA_LOST; nEventFilter++)
            {
                sg_asChannel[nChannel].m_OCI_EventFilter.eventCode = nEventFilter;
                ErrCode = (*(sBOA_PTRS.m_sOCI.linioVTable.addLINEventFilter))
                          (sg_asChannel[nChannel].m_OCI_RxQueueHandle,
                           &(sg_asChannel[nChannel].m_OCI_EventFilter), 1);

                if (ErrCode != OCI_SUCCESS)
                {
                    sg_hLastError = ErrCode;

                    hResult = S_FALSE;
                    sg_pIlog->vLogAMessage(A2T(__FILE__), __LINE__, _("could not add Event filter"));
                }
            }
        }

        //// Add Error filter
        //if (hResult == S_OK)
        //{
        //    ErrCode = (*(sBOA_PTRS.m_sOCI.LINIOVTable.addFlex))
        //              (sg_asChannel[nChannel].m_OCI_RxQueueHandle,
        //               &(sg_asChannel[nChannel].m_OCI_ErrorFilter), 1);
        //    if (ErrCode != OCI_SUCCESS)
        //    {
        //        hResult = S_FALSE;
        //        sg_pIlog->vLogAMessage(A2T(__FILE__), __LINE__, _("could not add Error filter"));
        //    }
        //}
        // Add internal error filter
        if (hResult == S_OK)
        {
            /* ErrCode = (*(sBOA_PTRS.m_sOCI.errorVTable.addInternalErrorEventFilter))
                       (sg_asChannel[nChannel].m_OCI_RxQueueHandle,
                        &(sg_asChannel[nChannel].m_OCI_InternalErrorFilter), 1);
             if (ErrCode != OCI_SUCCESS)
             {
                 sg_hLastError = ErrCode;

                 hResult = S_FALSE;
                 sg_pIlog->vLogAMessage(A2T(__FILE__), __LINE__, _("could not add Error filter"));
             }*/
        }

    }
    else if (byCode == FILTER_REMOVE)
    {
        // Remove Frame filter
        ErrCode = (*(sBOA_PTRS.m_sOCI.linioVTable.removeLINFrameFilter))
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
            for(INT nEventFilter = OCI_LIN_EVENT_WAKEUP; nEventFilter <= OCI_LIN_ERR_DATA_LOST; nEventFilter++)
            {
                sg_asChannel[nChannel].m_OCI_EventFilter.eventCode = nEventFilter;
                ErrCode = (*(sBOA_PTRS.m_sOCI.linioVTable.removeLINEventFilter))
                          (sg_asChannel[nChannel].m_OCI_RxQueueHandle,
                           &(sg_asChannel[nChannel].m_OCI_EventFilter), 1);
                if (ErrCode != OCI_SUCCESS)
                {
                    sg_hLastError = ErrCode;

                    hResult = S_FALSE;
                    sg_pIlog->vLogAMessage(A2T(__FILE__), __LINE__, _("could not remove Event filter"));
                }
            }
        }

        //// Remove Error filter
        //if (hResult == S_OK)
        //{
        //    ErrCode = (*(sBOA_PTRS.m_sOCI.canioVTable.removeCANErrorFrameFilter))
        //              (sg_asChannel[nChannel].m_OCI_RxQueueHandle,
        //               &(sg_asChannel[nChannel].m_OCI_ErrorFilter), 1);
        //    if (ErrCode != OCI_SUCCESS)
        //    {
        //        hResult = S_FALSE;
        //        sg_pIlog->vLogAMessage(A2T(__FILE__), __LINE__, _("could not remove Error filter"));
        //    }
        //}
        if (hResult == S_OK)
        {
            /* ErrCode = (*(sBOA_PTRS.m_sOCI.errorVTable.removeInternalErrorEventFilter))
                       (sg_asChannel[nChannel].m_OCI_RxQueueHandle,
                        &(sg_asChannel[nChannel].m_OCI_InternalErrorFilter), 1);
             if (ErrCode != OCI_SUCCESS)
             {
                 sg_hLastError = ErrCode;

                 hResult = S_FALSE;
                 sg_pIlog->vLogAMessage(A2T(__FILE__), __LINE__, _("could not add Error filter"));
             }*/
        }
    }
    return hResult;
}


UINT nGetProtocolVersion(std::string& strProtocol)
{
    UINT unVer = OCI_LIN_VERSION_2_1;
    if ( strProtocol == "LIN 1.1" || strProtocol == "LIN 1.2" || strProtocol == "LIN 1.1" || strProtocol == "LIN 1.3" )
    {
        unVer = OCI_LIN_VERSION_1_3;
    }
    else if ( strProtocol == "LIN 2.0" )
    {
        unVer = OCI_LIN_VERSION_2_0;
    }
    /*else  ( strProtocol == "LIN 2.1"  )
    {
        unVer = XL_LIN_VERSION_2_1;
    }*/
    return unVer;
}

static int nGetCalcChecksum(int nChannel, int nId )
{
    int nChecksum = OCI_LIN_CHECKSUM_ENHANCED;

    if ((nChannel >= 0) && (nChannel < sg_nNoOfChannels))
    {
        if ( sg_asChannel[nChannel ].m_OCI_FlexRayConfig.version <= OCI_LIN_VERSION_1_3 || ( nId >= 60 && nId <= 63 ) )
        {
            nChecksum = OCI_LIN_CHECKSUM_CLASSIC;
        }
    }

    return nChecksum;
}

static BOOL bLoadDataFromContr(ClusterConfig&  asDeviceConfig)
{
    BOOL bReturn = FALSE;

    // If successful
    for ( int nChannel = 0 ; nChannel < sg_nNoOfChannels; nChannel++ )
    {
        //1. Cluster Configuration
        memset ( &sg_asChannel[nChannel].m_OCI_FlexRayConfig, 0, sizeof( sg_asChannel[nChannel].m_OCI_FlexRayConfig ) );                    // Initialize the Controller Configuration to 0

        sg_asChannel[nChannel].m_OCI_FlexRayConfig.mode               = OCI_LIN_SLAVE;                    // Open this controller as a LIN Slave.
        sg_asChannel[nChannel].m_OCI_FlexRayConfig.baudrate           = asDeviceConfig.m_ouFlexChannelConfig[nChannel].m_ouLinParams.m_nBaudRate;                            // The chosen baudrate on the LIN BUS.  Note that this must match the
        // baudrate of the other controllers connected to the same BUS.
        sg_asChannel[nChannel].m_OCI_FlexRayConfig.version            = nGetProtocolVersion(asDeviceConfig.m_ouFlexChannelConfig[nChannel].m_ouLinParams.m_strProtocolVersion);
        sg_asChannel[nChannel].m_strLinVersion            =     asDeviceConfig.m_ouFlexChannelConfig[nChannel].m_ouLinParams.m_strProtocolVersion;
        sg_asChannel[nChannel].m_OCI_FlexRayConfig.masterResistor     = OCI_LIN_MASTER_RESISTOR_OFF;      // LIN master resistor is not connected to LIN bus.
        sg_asChannel[nChannel].m_OCI_FlexRayConfig.linSupply          = OCI_LIN_UBAT_SUPPLY_INTERNAL;     // LIN Port Power is supplied internally.
        sg_asChannel[nChannel].m_OCI_FlexRayConfig.selfReceptionMode  = OCI_SELF_RECEPTION_ON;            // The Controller will receive whatever is found on the LIN Bus, including
        // the frames it transmits itself.  Note that for LIN, this cannot be set
        // otherwise.

        // Set ECU Controller properties
        sg_asChannel[nChannel].m_OCI_CntrlProp.mode = OCI_CONTROLLER_MODE_SUSPENDED;

        std::list<FRAME_STRUCT> lstMsgNames;
        asDeviceConfig.m_ouFlexChannelConfig[nChannel].m_ouClusterInfo.GetFrames(lstMsgNames);

        std::list<FRAME_STRUCT>::iterator itrFrame = lstMsgNames.begin();

        //3. Create Tx Buffers
        for (INT nIndex = 0; nIndex < ( sizeof( sg_asChannel[nChannel].m_OCI_FlexRayConfig.messages ) / sizeof( sg_asChannel[nChannel].m_OCI_FlexRayConfig.messages[0] ) ); ++nIndex )
        {
            sg_asChannel[nChannel].m_OCI_FlexRayConfig.messages[nIndex].mode       = OCI_LIN_FRAME_DEFINED;      // The controller will not use this Frame ID for transmission of messages.

            //       // hard-coding values to avoid having uninitialized memory cause some unexpected issue.  These should all be ignored
            //       // since the frame mode is set to undefined.
            sg_asChannel[nChannel].m_OCI_FlexRayConfig.messages[nIndex].frameType  = OCI_LIN_UNCONDITIONAL_FRAME;
            sg_asChannel[nChannel].m_OCI_FlexRayConfig.messages[nIndex].crcType    = nGetCalcChecksum(nChannel, nIndex+1);
            sg_asChannel[nChannel].m_OCI_FlexRayConfig.messages[nIndex].dlc        = 8;
        }

        while(itrFrame != lstMsgNames.end())
        {
            if(itrFrame->m_nSlotId >= 0 && itrFrame->m_nSlotId <= 63)
            {
                sg_asChannel[nChannel].m_OCI_FlexRayConfig.messages[itrFrame->m_nSlotId].mode       = OCI_LIN_FRAME_DEFINED;      // The controller will not use this Frame ID for transmission of messages.

                //       // hard-coding values to avoid having uninitialized memory cause some unexpected issue.  These should all be ignored
                //       // since the frame mode is set to undefined.
                sg_asChannel[nChannel].m_OCI_FlexRayConfig.messages[itrFrame->m_nSlotId].frameType  = OCI_LIN_UNCONDITIONAL_FRAME;
                sg_asChannel[nChannel].m_OCI_FlexRayConfig.messages[itrFrame->m_nSlotId].crcType    = nGetCalcChecksum(nChannel, itrFrame->m_nSlotId);
                sg_asChannel[nChannel].m_OCI_FlexRayConfig.messages[itrFrame->m_nSlotId].dlc        = itrFrame->m_nLength;
            }

            itrFrame++;
        }
    }

    return true;
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
void vCopy_2_OCI_LIN_Data(OCI_LINTxMessage& DestMsg, const STLIN_MSG& SrcMsg)
{
    // DestMsg.size    = SrcMsg.m_ucDataLen;
    DestMsg.flags = 0;
    // TO DO : LIN
    memcpy(DestMsg.data, SrcMsg.m_ucData, SrcMsg.m_ucDataLen);
}

/**
 * \return TRUE if client exists else FALSE
 *
 * Checks for the existance of the client with the name pcClientName.
 */
static BOOL bClientExist(std::string pcClientName, INT & Index)
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
    memset ( &sg_asChannel[nChannel].m_OCI_FlexRayConfig, 0, sizeof( sg_asChannel[nChannel].m_OCI_FlexRayConfig ) );                    // Initialize the Controller Configuration to 0
    sg_asChannel[nChannel].m_OCI_FlexRayConfig.mode               = OCI_LIN_SLAVE;                    // Open this controller as a LIN Slave.
    sg_asChannel[nChannel].m_OCI_FlexRayConfig.baudrate           = 9600;                            // The chosen baudrate on the LIN BUS.  Note that this must match the
    // baudrate of the other controllers connected to the same BUS.
    sg_asChannel[nChannel].m_OCI_FlexRayConfig.version            = OCI_LIN_VERSION_1_3;              // Use LIN v2.1
    sg_asChannel[nChannel].m_OCI_FlexRayConfig.masterResistor     = OCI_LIN_MASTER_RESISTOR_OFF;      // LIN master resistor is not connected to LIN bus.
    sg_asChannel[nChannel].m_OCI_FlexRayConfig.linSupply          = OCI_LIN_UBAT_SUPPLY_INTERNAL;     // LIN Port Power is supplied internally.
    sg_asChannel[nChannel].m_OCI_FlexRayConfig.selfReceptionMode  = OCI_SELF_RECEPTION_ON;            // The Controller will receive whatever is found on the LIN Bus, including
    // the frames it transmits itself.  Note that for LIN, this cannot be set
    // otherwise.
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
    return (1); //Currently 1 Channel Has to e fixed for FlexRay also if required
}
eLinBusEventType vGetTranmissionEventState(UINT unstate)
{
    switch(unstate)
    {
            /*case OCI_LIN_TRANS_OK:
                return EVENT_LIN_TX_OK;*/
        case OCI_LIN_TRANS_NONE:
            return EVENT_LIN_NONE;
        case OCI_LIN_TRANS_ERR_BIT:
            return EVENT_LIN_ERRBIT;
        case OCI_LIN_TRANS_ERR_CRC:
            return EVENT_LIN_ERRCRC;
        case OCI_LIN_TRANS_ERR_HEADER:
            return EVENT_LIN_ERRHEADER;
        case OCI_LIN_TRANS_ERR_TOUT:
            return EVENT_LIN_ERRTOUT;
        case OCI_LIN_TRANS_COLLISION:
            return EVENT_LIN_COLLISION;
        case OCI_LIN_TRANS_NO_RESPONSE:
            return EVENT_LIN_ERRNOANS;
        case OCI_LIN_TRANS_NO_MSTR_SCHED:
            return EVENT_LIN_NO_MSTR_SCHED;
        case OCI_LIN_TRANS_NO_TX_DATA:
            return EVENT_LIN_NO_TX_DATA;
        default:
            break;
    }
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
    sg_asChannel[nChannel].m_OCI_FrameFilter.tag           = 0;    // We need no tag in this application. We can set any value. It will be ignored by the application.

    /* configure frame filter*/

    sg_asChannel[nChannel].m_OCI_EventFilter.destination = OCI_EVENT_DESTINATION_CALLBACK;
    sg_asChannel[nChannel].m_OCI_EventFilter.eventCode = (OCI_LINEvent)(OCI_LIN_EVENT_WAKEUP|OCI_LIN_EVENT_SLEEP_MODE|OCI_LIN_EVENT_TRANSMISSION_STATUS|OCI_LIN_ERR_SYNC|OCI_LIN_EVENT_CRC_DETECT|OCI_LIN_EVENT_DLC_DETECT|OCI_LIN_ERR_DATA_LOST) ;
    //sg_asChannel[nChannel].m_OCI_EventFilter.eventCode = (OCI_LINEvent)(OCI_LIN_EVENT_TRANSMISSION_STATUS) ;
    sg_asChannel[nChannel].m_OCI_EventFilter.tag = 0;

    /* configure Error filter */

    /* configure internal error filter */
    sg_asChannel[nChannel].m_OCI_InternalErrorFilter.eventCode = OCI_INTERNAL_GENERAL_ERROR;
    sg_asChannel[nChannel].m_OCI_InternalErrorFilter.tag = 0;
}
/**************************************************************************/


/* CDIL_FLEXRAY_ETAS_BOA class definition */
class CDIL_LIN_ETAS_BOA : public CCommanDIL_LIN
{

private:
    HINSTANCE m_dllHandle,m_dllHandle_tx;
    ECONTR_STATUS m_Ctrl_Status;

public:
    /* STARTS IMPLEMENTATION OF THE INTERFACE FUNCTIONS... */
    HRESULT LIN_PerformInitOperations(void);
    HRESULT LIN_PerformClosureOperations(void);
    HRESULT LIN_GetTimeModeMapping(SYSTEMTIME& CurrSysTime, UINT64& TimeStamp, LARGE_INTEGER* QueryTickCount = nullptr);
    HRESULT LIN_ListHwInterfaces(INTERFACE_HW_LIST& sSelHwInterface, INT& nCount);
    HRESULT LIN_SelectHwInterface(const INTERFACE_HW_LIST& sSelHwInterface, INT nCount);
    HRESULT LIN_DeselectHwInterface(void);
    HRESULT LIN_DisplayConfigDlg(PSCONTROLLER_DETAILS_LIN /* InitData */, int & /* Length */)
    {
        return S_OK;
    }
    HRESULT LIN_SetConfigData(ClusterConfig& ouConfig);
    HRESULT LIN_StartHardware(void);
    HRESULT LIN_PreStartHardware(void)
    {
        return S_OK;
    }

    HRESULT LIN_StopHardware(void);
    HRESULT hRecreateLINController(INT nIndex);
    HRESULT LIN_ResetHardware(void)
    {
        return S_OK;
    }

    HRESULT LIN_GetCurrStatus(s_STATUSMSG & /* StatusData */)
    {
        return S_OK;
    }

    HRESULT LIN_GetTxMsgBuffer(BYTE*& pouFlxTxMsgBuffer);
    HRESULT LIN_Send(STLIN_MSG& sCanTxMsg);
    HRESULT LIN_InitializeBuffers();

    HRESULT LIN_SetSlaveRespData(const STLIN_MSG /* stRespMsg */)
    {
        return S_OK;
    };

    HRESULT LIN_ResetSlaveRespData(void)
    {
        return S_OK;
    };

    HRESULT LIN_DisableSlave(STLIN_MSG & /* sMessage */)
    {
        return S_OK;
    }

    HRESULT LIN_GetBusConfigInfo(BYTE* BusInfo);
    HRESULT LIN_GetLastErrorString(std::string & acErrorStr);
    HRESULT LIN_FilterFrames(FILTER_TYPE FilterType, TYPE_CHANNEL Channel, UINT* punMsgIds, UINT nLength);
    HRESULT LIN_GetControllerParams(LONG& lParam, UINT nChannel, ECONTR_PARAM eContrParam);
    HRESULT LIN_GetConfiguration(sCONTROLLERDETAILSLIN[], INT& nSize);
    HRESULT LIN_SetControllerParams(int nValue, ECONTR_PARAM eContrparam);
    HRESULT LIN_GetErrorCount(SERROR_CNT& sErrorCnt, UINT nChannel, ECONTR_PARAM eContrParam);
    HRESULT LIN_SetAppParams(HWND hWndOwner, Base_WrapperErrorLogger* pILog);
    HRESULT LIN_GetCntrlStatus(const HANDLE& hEvent, UINT& unCntrlStatus);
    HRESULT LIN_LoadDriverLibrary(void);
    HRESULT LIN_UnloadDriverLibrary(void);

    void vCopyOCI_LIN_RX_2_DATA(STLINDATA& stLinMsg,
                                OCI_LINMessage& BOA_FlexData);

    void ProcessLINMsg(void* userData, struct OCI_LINMessage* msg);
    HRESULT nWriteMessage(STLIN_MSG& ouData);
    void vCopyOCI_LIN_EVENT_2_DATA(STLINDATA& stLinMsg,
                                   OCI_LINMessage& BOA_FlexData, BOOL& bIsError);
};

CDIL_LIN_ETAS_BOA* g_pouDIL_LIN_BOA = nullptr;


void CDIL_LIN_ETAS_BOA::vCopyOCI_LIN_EVENT_2_DATA(STLINDATA& stLinMsg,
        OCI_LINMessage& BOA_FlexData, BOOL& bIsError)
{
    HRESULT hResult = S_OK;
    UINT unBaudRate;

    stLinMsg.m_uDataInfo.m_sErrInfo.m_ulTimeStamp = (LONGLONG)(BOA_FlexData.data.rxMessage.timeStamp * sg_asChannel[0].m_fResolution);

    switch ( BOA_FlexData.data.linEventMessage.eventCode)
    {
        case OCI_LIN_EVENT_WAKEUP:

            stLinMsg.m_eLinMsgType = LIN_EVENT;
            stLinMsg.m_uDataInfo.m_sErrInfo.m_ucId = -1;
            stLinMsg.m_eLinMsgType = LIN_EVENT;
            stLinMsg.m_uDataInfo.m_sErrInfo.m_eEventType = EVENT_LIN_WAKEUP;
            bIsError = TRUE;
            break;
        case OCI_LIN_EVENT_SLEEP_MODE:
            stLinMsg.m_uDataInfo.m_sErrInfo.m_ucId = -1;
            stLinMsg.m_eLinMsgType = LIN_EVENT;
            stLinMsg.m_uDataInfo.m_sErrInfo.m_eEventType = EVENT_LIN_SLEEP;

            switch ( BOA_FlexData.data.linEventMessage.data.sleepModeEvent.mode)
            {
                case OCI_LIN_SLEEPMODE_GOTOSLEEP:
                    stLinMsg.m_uDataInfo.m_sErrInfo.m_eSleepEvent = LIN_SLEEPMODE_SET;
                    bIsError = TRUE;
                    break;
                case OCI_LIN_SLEEPMODE_WAKEUP:
                    stLinMsg.m_uDataInfo.m_sErrInfo.m_eSleepEvent = LIN_SLEEPMODE_WAKEUP;
                    bIsError = TRUE;
                    break;
                case OCI_LIN_SLEEPMODE_STAYALIVE:
                default:
                    stLinMsg.m_uDataInfo.m_sErrInfo.m_eSleepEvent = LIN_SLEEPMODE_STAYALIVE;
                    bIsError = TRUE;
                    break;
            }
            hResult = S_OK;
            break;
        case OCI_LIN_ERR_SYNC:
            stLinMsg.m_uDataInfo.m_sErrInfo.m_ucId = -1;
            stLinMsg.m_eLinMsgType = LIN_EVENT;
            unBaudRate = BOA_FlexData.data.linEventMessage.data.errSyncEvent.baudrate;

            bIsError = TRUE;
            stLinMsg.m_uDataInfo.m_sErrInfo.m_eEventType = EVENT_LIN_ERRSYNC;
            break;
        case OCI_LIN_EVENT_CRC_DETECT:
            stLinMsg.m_eLinMsgType = LIN_EVENT;
            stLinMsg.m_uDataInfo.m_sErrInfo.m_eEventType = EVENT_LIN_ERRCRC;
            stLinMsg.m_uDataInfo.m_sErrInfo.m_ucId = BOA_FlexData.data.linEventMessage.data.crcDetectEvent.frameID;
            bIsError = TRUE;
            SLIN_CRC sCRC;
            memcpy(sCRC.ucData, stLinMsg.m_uDataInfo.m_sLINMsg.m_ucData, stLinMsg. m_uDataInfo.m_sLINMsg.m_ucDataLen);
            sCRC.unDlc = stLinMsg. m_uDataInfo.m_sLINMsg.m_ucDataLen;
            sCRC.unID = stLinMsg. m_uDataInfo.m_sLINMsg.m_ucMsgID;
            sCRC.unCrcType = LIN_CHECKSUM_ENHANCED;

            stLinMsg.m_uDataInfo.m_sLINMsg.m_ucChksumTyp = LIN_CHECKSUM_ENHANCED;
            if ( BOA_FlexData.data.linEventMessage.data.crcDetectEvent.crcType == OCI_LIN_CHECKSUM_CLASSIC )
            {
                sCRC.unCrcType = LIN_CHECKSUM_CLASSIC;
                stLinMsg.m_uDataInfo.m_sLINMsg.m_ucChksumTyp = LIN_CHECKSUM_CLASSIC;
            }

            if ( nullptr != g_pouDIL_LIN_BOA )
            {
                stLinMsg.m_uDataInfo.m_sErrInfo.m_ucCrc = g_pouDIL_LIN_BOA->ucChecksumCalculation(sCRC);
            }
            //stLinMsg.m_uDataInfo.m_sErrInfo.m_ucCrc =  BOA_FlexData.data.linEventMessage.data.crcDetectEvent.crcType;
            break;
        case OCI_LIN_EVENT_DLC_DETECT:
            stLinMsg.m_uDataInfo.m_sErrInfo.m_ucId = -1;
            stLinMsg.m_eLinMsgType = LIN_EVENT;

            stLinMsg.m_uDataInfo.m_sErrInfo.m_eEventType = EVENT_LIN_DLC;
            stLinMsg.m_uDataInfo.m_sErrInfo.m_ucDlc = BOA_FlexData.data.linEventMessage.data.dlcDetectEvent.dlc;
            stLinMsg.m_uDataInfo.m_sErrInfo.m_ucId = BOA_FlexData.data.linEventMessage.data.dlcDetectEvent.frameID;
            bIsError = TRUE;
            break;
        case OCI_LIN_ERR_DATA_LOST:
            stLinMsg.m_eLinMsgType = LIN_EVENT;
            stLinMsg.m_uDataInfo.m_sErrInfo.m_eEventType = EVENT_LIN_ERRMSG;
            stLinMsg.m_uDataInfo.m_sErrInfo.m_ucTxErrCount = BOA_FlexData.data.linEventMessage.data.errDataLostEvent.lostMessageCount;
            bIsError = TRUE;
            break;
        case OCI_LIN_EVENT_TRANSMISSION_STATUS:
            stLinMsg.m_eLinMsgType = LIN_EVENT;
            // calculate crc value
            //stLinMsg.m_uDataInfo.m_sErrInfo.m_ucCrc = BOA_FlexData.data.linEventMessage.data.transmissionStatusEvent.crcType;
            stLinMsg.m_uDataInfo.m_sErrInfo.m_ucId = BOA_FlexData.data.linEventMessage.data.transmissionStatusEvent.frameID;
            stLinMsg.m_uDataInfo.m_sErrInfo.m_ucDlc = BOA_FlexData.data.linEventMessage.data.transmissionStatusEvent.dlc;
            stLinMsg.m_uDataInfo.m_sErrInfo.m_ucDir = BOA_FlexData.data.linEventMessage.data.transmissionStatusEvent.direction;

            stLinMsg.m_uDataInfo.m_sErrInfo.m_eEventType = vGetTranmissionEventState(BOA_FlexData.data.linEventMessage.data.transmissionStatusEvent.state);
            if(BOA_FlexData.data.linEventMessage.data.transmissionStatusEvent.state != OCI_LIN_TRANS_OK)
            {
                bIsError = TRUE;
                //stLinMsg.m_uDataInfo.m_sErrInfo.m_eEventType = vGetTranmissionEventState(BOA_FlexData.data.linEventMessage.data.transmissionStatusEvent.state);
            }

            break;
        default:
            break;
    }
}

void CDIL_LIN_ETAS_BOA::vCopyOCI_LIN_RX_2_DATA(STLINDATA& stLinMsg,
        OCI_LINMessage& BOA_FlexData)
{
    stLinMsg.m_uDataInfo.m_sLINMsg.m_ulTimeStamp =(LONGLONG)(BOA_FlexData.data.rxMessage.timeStamp * sg_asChannel[0].m_fResolution);
    stLinMsg.m_uDataInfo.m_sErrInfo.m_ulTimeStamp = (LONGLONG)(BOA_FlexData.data.rxMessage.timeStamp * sg_asChannel[0].m_fResolution);

    stLinMsg.m_ucDataType   = (UCHAR)RX_FLAG;

    switch ( BOA_FlexData.type)
    {
        case OCI_LIN_RX_MESSAGE:

            stLinMsg.m_eLinMsgType = LIN_MSG;
            stLinMsg.m_uDataInfo.m_sLINMsg.m_ucMsgID = BOA_FlexData.data.rxMessage.frameID;
            stLinMsg. m_uDataInfo.m_sLINMsg.m_ucDataLen = BOA_FlexData.data.rxMessage.dlc;
            memcpy(stLinMsg.m_uDataInfo.m_sLINMsg.m_ucData, BOA_FlexData.data.rxMessage.data, BOA_FlexData.data.rxMessage.dlc);

            if (BOA_FlexData.data.rxMessage.flags & OCI_LIN_MESSAGE_TX)
            {
                stLinMsg.m_ucDataType   = (UCHAR)TX_FLAG;
            }

            SLIN_CRC sCRC;
            memcpy(sCRC.ucData, stLinMsg.m_uDataInfo.m_sLINMsg.m_ucData, stLinMsg. m_uDataInfo.m_sLINMsg.m_ucDataLen);
            sCRC.unDlc = stLinMsg. m_uDataInfo.m_sLINMsg.m_ucDataLen;
            sCRC.unID = stLinMsg. m_uDataInfo.m_sLINMsg.m_ucMsgID;
            sCRC.unCrcType = LIN_CHECKSUM_ENHANCED;

            stLinMsg.m_uDataInfo.m_sLINMsg.m_ucChksumTyp = LIN_CHECKSUM_ENHANCED;
            if ( BOA_FlexData.data.rxMessage.crcType == OCI_LIN_CHECKSUM_CLASSIC )
            {
                sCRC.unCrcType = LIN_CHECKSUM_CLASSIC;
                stLinMsg.m_uDataInfo.m_sLINMsg.m_ucChksumTyp = LIN_CHECKSUM_CLASSIC;
            }

            if ( nullptr != g_pouDIL_LIN_BOA )
            {
                stLinMsg.m_uDataInfo.m_sLINMsg.m_ucChksum = g_pouDIL_LIN_BOA->ucChecksumCalculation(sCRC);
            }
            break;
    }
}

/**
 * Processes Rx msg and writes into regiastered clients buffer.
 */
void CDIL_LIN_ETAS_BOA:: ProcessLINMsg(void* userData, struct OCI_LINMessage* msg)
{
    /* First calculate timestamp for first message*/
    // EnterCriticalSection(&sg_DIL_CriticalSection);

    STLINDATA FlexData;
    OCI_ControllerHandle hHandle;
    BOOL bIsNotError = FALSE, bWrite = TRUE;;

    hHandle = (OCI_ControllerHandle)userData;

    FlexData.m_uDataInfo.m_sLINMsg.m_ucChannel = (UCHAR)nGetChannel(hHandle);
    FlexData.m_uDataInfo.m_sErrInfo.m_ucChannel = (UCHAR)nGetChannel(hHandle);
    if ( msg->type == OCI_LIN_RX_MESSAGE )
    {
        //sCanData.m_uDataInfo.m_sCANMsg.m_bCANFDMsg = false;

        //_cprintf("%x %d %d %d\n", msg->data.rxMessage.frameID, msg->data.rxMessage.data[0], msg->data.rxMessage.data[1], msg->data.rxMessage.data[2]);
        vCopyOCI_LIN_RX_2_DATA(FlexData, *msg);
    }

    if ( msg->type == OCI_LIN_EVENT )
    {
        vCopyOCI_LIN_EVENT_2_DATA(FlexData, *msg, bIsNotError);

        if(bIsNotError == FALSE)
        {
            bWrite = FALSE;
        }
    }

    FlexData.m_lTickCount.QuadPart = ((LONGLONG)(*msg).data.rxMessage.timeStamp / 100000) /*/ 8*/;

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
        if(FlexData.m_lTickCount.QuadPart >= unConnectionTime)
        {
            sg_TimeStamp  = (LONGLONG)(FlexData.m_lTickCount.QuadPart - unConnectionTime);
        }
        else
        {
            sg_TimeStamp  = (LONGLONG)(unConnectionTime - FlexData.m_lTickCount.QuadPart);
        }
    }

    //Write the msg into registered client's buffer
    // bWrite will be true if it is succesful tranmissions
    // Hence avoided intimating
    if ( nullptr != g_pouDIL_LIN_BOA && bWrite == TRUE)
    {
        g_pouDIL_LIN_BOA->vWriteIntoClientsBuffer(FlexData);
    }
}

/**
 * Callback function called by the BOA framework
 * when there is presence of msg in the bus.
 */
static void (OCI_CALLBACK ProcessLINData)(void* userData, OCI_LINMessage* msg)
{
    switch (msg->type)
    {
        case OCI_LIN_RX_MESSAGE:
            EnterCriticalSection(&sg_DIL_CriticalSection);
            if ( g_pouDIL_LIN_BOA != nullptr )
            {
                g_pouDIL_LIN_BOA->ProcessLINMsg(userData, msg);
            }
            LeaveCriticalSection(&sg_DIL_CriticalSection);
            break;
            /*case OCI_FLEXRAY_TX_MESSAGE:
                vProcessTxMsg(userData, msg);
                break;*/

        default:
            break;

    }
}
static void EventCallback( void* userData,OCI_LINMessage* msg )
{
    switch ( msg->type )
    {
        case OCI_LIN_EVENT:

            EnterCriticalSection(&sg_DIL_CriticalSection);
            if ( g_pouDIL_LIN_BOA != nullptr )
            {
                g_pouDIL_LIN_BOA->ProcessLINMsg(userData, msg);
            }
            LeaveCriticalSection(&sg_DIL_CriticalSection);
            break;

        case OCI_LIN_TIMER_EVENT:
            // The OCI_FLEXRAY_TIMER_EVENT message type represents events that occurred on the Timer belonging to the same the Controller
            // which owns the Rx Queue that has received the message.  In this example, no Timer events should be received by the Event
            // Callback, but this code demonstrates how to handle such events.
            //
            // As with the OCI_FLEXRAY_EVENT type, these events can be recieved by the Frame Callback, as well as the Event Callback,
            // function depending on the value of the 'destination' field in the filter which matches the event.
            printf( "Message type: OCI_LIN_TIMER_EVENT.\n" );
            printf( "Message Time Stamp: %llu\n", msg->data.timerEventMessage.timeStamp );
            printf( "Event Code: 0x%x\n", msg->data.timerEventMessage.eventCode );
            printf( "Message Tag: 0x%x\n", msg->data.timerEventMessage.tag );

            // DO SOMETHING USEFUL WITH THE TIMER EVENT HERE.

            break;

        case OCI_LIN_QUEUE_EVENT:
            // The OCI_FLEXRAY_QUEUE_EVENT message type represents events that occurred on any Queue belonging to the same the Controller
            // which owns the Rx Queue that has received the message.  The queue on which the event ocurred is not referenced in the
            // message, but this can be overcome by coupling the tag with the Queue handle when creating the Queue Event Filters.
            // In this example, no Queue events should be received by the Event Callback, but this code demonstrates how to handle
            // such events.
            //
            // As with the OCI_FLEXRAY_EVENT type, these events can be recieved by the Frame Callback, as well as the Event Callback,
            // function depending on the value of the 'destination' field in the filter which matches the event.
            printf( "Message type: OCI_LIN_QUEUE_EVENT.\n" );
            printf( "Message Time Stamp: %llu\n", msg->data.queueEventMessage.timeStamp );
            printf( "Event Code: 0x%x\n", msg->data.queueEventMessage.eventCode );
            printf( "Message Tag: 0x%x\n", msg->data.queueEventMessage.tag );

            // DO SOMETHING USEFUL WITH THE QUEUE EVENT HERE.

            break;

        default:
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
    sg_asChannel[nChannel].m_OCI_RxQueueCfg.onFrame.function = ProcessLINData;
    sg_asChannel[nChannel].m_OCI_RxQueueCfg.onFrame.userData = nullptr;
    sg_asChannel[nChannel].m_OCI_RxQueueCfg.onEvent.function = EventCallback;
    sg_asChannel[nChannel].m_OCI_RxQueueCfg.onEvent.userData = nullptr;

    sg_asChannel[nChannel].m_OCI_RxQueueCfg.selfReceptionMode = OCI_SELF_RECEPTION_ON;

    // TO DO : LIN
    /* if ( sg_asChannel[nChannel].m_ControllerCapabilities.supportedBusMode & OCI_FLEXRAY_BUSMODE_SELFRECEPTION )
     {
         sg_asChannel[nChannel].m_OCI_RxQueueCfg.selfReceptionMode = OCI_SELF_RECEPTION_ON;
     }*/

    /* configure Tx Queue*/
    //sg_asChannel[nChannel].m_OCI_TxQueueCfg.reserved = 0;
}
/**
* \brief         Returns the CDIL_FLEXRAY_ETAS_BOA object
* \param[out]    ppvInterface, is void pointer to take back the reference to CDIL_LIN_ETAS_BOA object
* \return        S_OK for success, S_FALSE for failure
* \authors       Arunkumar Karri
* \date          07.10.2011 Created
*/
USAGEMODE HRESULT GetIDIL_LIN_Controller(void** ppvInterface)
{
    HRESULT hResult = S_OK;
    if ( nullptr == g_pouDIL_LIN_BOA )
    {
        if ((g_pouDIL_LIN_BOA = new CDIL_LIN_ETAS_BOA) == nullptr)
        {
            hResult = S_FALSE;
        }
    }
    *ppvInterface = (void*) g_pouDIL_LIN_BOA;  /* Doesn't matter even if g_pouDIL_FLEXRAY_BOA is null */

    return hResult;
}

HRESULT CDIL_LIN_ETAS_BOA::LIN_SetAppParams(HWND hWndOwner, Base_WrapperErrorLogger* pILog)
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

HRESULT CDIL_LIN_ETAS_BOA::LIN_LoadDriverLibrary(void)
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

HRESULT CDIL_LIN_ETAS_BOA::LIN_UnloadDriverLibrary(void)
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

HRESULT CDIL_LIN_ETAS_BOA::LIN_PerformInitOperations(void)
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
    LIN_RegisterClient(TRUE, dwClient, FLEXRAY_MONITER_NODE);

    //Initialize the selected channel items array to -1
    for ( UINT i = 0; i< CHANNEL_ALLOWED; i++ )
    {
        sg_anSelectedItems[i] = -1;
    }

    return S_OK;
}

HRESULT CDIL_LIN_ETAS_BOA::LIN_PerformClosureOperations(void)
{
    sg_bCurrState = STATE_DRIVER_SELECTED;
    return S_OK;
}

HRESULT CDIL_LIN_ETAS_BOA::LIN_GetTimeModeMapping(SYSTEMTIME & CurrSysTime, UINT64 & TimeStamp, LARGE_INTEGER * /* QueryTickCount */)
{
    memcpy(&CurrSysTime, &sg_CurrSysTime, sizeof(SYSTEMTIME));
    TimeStamp = sg_TimeStamp;

    return S_OK;
}

HRESULT CDIL_LIN_ETAS_BOA::LIN_ListHwInterfaces(INTERFACE_HW_LIST & sSelHwInterface, INT & /* sAvailableConfigFiles */)
{
    VALIDATE_VALUE_RETURN_VAL(sg_bCurrState, STATE_DRIVER_SELECTED, ERR_IMPROPER_STATE);

    {
        int nCount = CHANNEL_ALLOWED;
        USES_CONVERSION;
        HRESULT hResult = S_FALSE;
        OCI_URIName acURI[defNO_OF_CHANNELS];
        uint32 nFound = 0;

        UINT unDefaultChannelCnt = nCount;

        if (OCI_FindLINController(acURI, defNO_OF_CHANNELS, &nFound) == OCI_SUCCESS)
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

                    OCI_LIN_VTable          sOCI;

                    if ((sOCI.createLINController = (PF_OCI_CreateLINController)
                                                    GetProcAddress(sg_hLibOCI, "OCI_CreateLINController")) != nullptr)
                    {
                        OCI_ControllerHandle ouHandle;

                        BOA_ResultCode err =  (*(sOCI.createLINController))( acURI[i], &ouHandle);
                        if (err == OCI_SUCCESS)
                        {
                            OCI_LINControllerCapabilities  ouCapabilities;
                            hResult = (*(sBOA_PTRS.m_sOCI.linioVTable.getLINControllerCapabilities))(ouHandle, &ouCapabilities);

                            /*  if ( ouCapabilities.supportedBusMode & OCI_FLEXRAY_BUSMODE_SELFRECEPTION )
                              {
                                  psHWInterface[i].m_acAdditionalInfo = "";
                              }
                              else
                              {
                                  psHWInterface[i].m_acAdditionalInfo = "Selected Channel will NOT support Self Recieption.So BUSMASTER will not display the Tx messages";
                              }*/


                            int  i = 0 ;
                            hResult = (*(sBOA_PTRS.m_sOCI.destroyLINController))(ouHandle);

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
                    else if ( ListHardwareInterfaces(nullptr, DRIVER_LIN_ETAS_BOA, psHWInterface, sg_anSelectedItems, nCount) != 0 )
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
                for (UINT nList = 0; nList < sg_nNoOfChannels; nList++)
                {
                    sSelHwInterface[nList].m_acNameInterface = psHWInterface[sg_anSelectedItems[nList]].m_acNameInterface;
                    sSelHwInterface[nList].m_acDescription = psHWInterface[sg_anSelectedItems[nList]].m_acDescription;
                    sSelHwInterface[nList].m_dwIdInterface = 100 + nList; // Give a dummy number
                }

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

HRESULT CDIL_LIN_ETAS_BOA::LIN_SelectHwInterface(const INTERFACE_HW_LIST & sSelHwInterface, INT /* nCount */)
{
    VALIDATE_VALUE_RETURN_VAL(sg_bCurrState, STATE_HW_INTERFACE_LISTED, ERR_IMPROPER_STATE);

    USES_CONVERSION;
    HRESULT hResult = S_OK;
    //First select only supported number of HW interfaces
    for (UINT i = 0; i < sg_nNoOfChannels; i++)
    {
        strcpy(sg_asChannel[i].m_acURI, sSelHwInterface[i].m_acNameInterface.c_str());
    }

    // Create the controller instance.
    for (UINT i = 0; i < sg_nNoOfChannels; i++)
    {
        BOA_ResultCode err = OCI_FAILURE;

        OCI_LIN_VTable          sOCI;

        if ((sOCI.createLINController = (PF_OCI_CreateLINController)
                                        GetProcAddress(sg_hLibOCI, "OCI_CreateLINController")) == nullptr)
        {
            hResult = S_FALSE;
        }

        /*err =  (*(sBOA_PTRS.m_sOCI.createFlexRayController))(sg_asChannel[i].m_acURI,
                              &m_OCIHandle);*/
        err =  (*(sOCI.createLINController))(sg_asChannel[i].m_acURI,
                                             &sg_asChannel[i].m_OCI_HwHandle);
        if (err == OCI_SUCCESS)
        {
            //sg_asChannel[i].m_OCI_HwHandle = m_OCIHandle;
            hResult = (*(sBOA_PTRS.m_sOCI.timeVTable.getTimerCapabilities))(sg_asChannel[i].m_OCI_HwHandle,
                      &(sg_asChannel[i].m_OCI_TimerCapabilities));
            if ( hResult == OCI_SUCCESS )
            {
                sg_asChannel[i].m_fResolution = (float)10000 /(float)(sg_asChannel[i].m_OCI_TimerCapabilities.tickFrequency);
            }

            hResult = (*(sBOA_PTRS.m_sOCI.linioVTable.getLINControllerCapabilities))(sg_asChannel[i].m_OCI_HwHandle, &sg_asChannel[i].m_ControllerCapabilities);

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

HRESULT CDIL_LIN_ETAS_BOA::LIN_DeselectHwInterface(void)
{
    VALIDATE_VALUE_RETURN_VAL(sg_bCurrState, STATE_HW_INTERFACE_SELECTED, ERR_IMPROPER_STATE);

    HRESULT hResult = S_OK;
    for (UINT i = 0; i < sg_nNoOfChannels; i++)
    {
        if ((hResult = ManageFilters(FILTER_REMOVE, i)) == S_OK)
        {
            if ((hResult = ManageQueue(QUEUE_DESTROY, i)) == S_OK)
            {
                if ((*(sBOA_PTRS.m_sOCI.closeLINController))(sg_asChannel[i].m_OCI_HwHandle) == OCI_SUCCESS)
                {
                    if ((*(sBOA_PTRS.m_sOCI.destroyLINController))(sg_asChannel[i].m_OCI_HwHandle) == OCI_SUCCESS)
                    {
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

HRESULT CDIL_LIN_ETAS_BOA::LIN_SetConfigData(ClusterConfig& ouAbsSFibexContainer)
{
    HRESULT hResult = WARNING_NOTCONFIRMED;

    VALIDATE_VALUE_RETURN_VAL(sg_bCurrState, STATE_HW_INTERFACE_SELECTED, ERR_IMPROPER_STATE);

    BOA_ResultCode ErrCode = OCI_FAILURE;

    bLoadDataFromContr(ouAbsSFibexContainer);

    return S_OK;
}

HRESULT CDIL_LIN_ETAS_BOA::LIN_StartHardware(void)
{
    VALIDATE_VALUE_RETURN_VAL(sg_bCurrState, STATE_HW_INTERFACE_SELECTED, ERR_IMPROPER_STATE);

    HRESULT hResult = S_OK;
    OCI_ErrorCode OciErrorCode = OCI_FAILURE;
    for (UINT i = 0; i < sg_nNoOfChannels; i++)
    {
        OciErrorCode = (*(sBOA_PTRS.m_sOCI.getLINControllerProperties))(sg_asChannel[i].m_OCI_HwHandle,
                       &(sg_asChannel[i].m_OCI_CntrlProp));
        if ( OciErrorCode == OCI_SUCCESS)
        {
            if ((sg_asChannel[i].m_OCI_CntrlProp.mode == OCI_CONTROLLER_MODE_SUSPENDED)
                    || (sg_asChannel[i].m_OCI_CntrlProp.mode == OCI_CONTROLLER_MODE_RUNNING))
            {
                LIN_InitializeBuffers();

                sg_asChannel[i].m_OCI_CntrlProp.mode = OCI_CONTROLLER_MODE_RUNNING;
                OciErrorCode = (*(sBOA_PTRS.m_sOCI.setLINControllerProperties))(sg_asChannel[i].m_OCI_HwHandle,
                               &(sg_asChannel[i].m_OCI_CntrlProp));
                if ( OciErrorCode == OCI_SUCCESS)
                {
                    m_bLINConnected = TRUE;

                    std::map<UINT, BOA_LINData>::iterator itr =  sg_asChannel[i].m_ouBoaLINData.begin();
                    for(; itr != sg_asChannel[i].m_ouBoaLINData.end(); itr++ )
                    {
                        nWriteMessage( itr->second.m_ouLinData );
                    }

                    hResult |= S_OK;
                }
                else
                {
                    m_bLINConnected = FALSE;
                    sg_asChannel[i].m_OCI_CntrlProp.mode = OCI_CONTROLLER_MODE_RUNNING;
                    sg_hLastError = OciErrorCode;
                    hResult = S_FALSE;
                    sg_pIlog->vLogAMessage(A2T(__FILE__), __LINE__, _("could not start the controller in running mode"));
                }
            }
        }
        else
        {
            m_bLINConnected = FALSE;
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

HRESULT CDIL_LIN_ETAS_BOA::LIN_InitializeBuffers(void)
{
    HRESULT hResult = S_OK;
    BOA_ResultCode ErrCode;
    OCI_ErrorCode   OciErrorCode =  OCI_FAILURE;
    for (UINT i = 0; i < sg_nNoOfChannels; i++)

    {
        //First remove all the filters
        ManageFilters(FILTER_REMOVE, 0);
        ////First remove all the Rx Tx queues
        ManageQueue(QUEUE_DESTROY, 0);
        // if controller is open, close the controller. Do not bother about return value
        ErrCode = (*sBOA_PTRS.m_sOCI.closeLINController)(sg_asChannel[i].m_OCI_HwHandle);

        //Now load the controller config and open the controller
        std::map<UINT, BOA_LINData>::iterator itr = sg_asChannel[i].m_ouBoaLINData.begin();
        for ( ; itr != sg_asChannel[i].m_ouBoaLINData.end(); itr++ )
        {
            sg_asChannel[i].m_OCI_FlexRayConfig.messages[itr->second.m_ouLinData.m_ucMsgID].frameType      = OCI_LIN_UNCONDITIONAL_FRAME;  // Declare that this will be an unconditional frame id for this controller.
            sg_asChannel[i].m_OCI_FlexRayConfig.messages[itr->second.m_ouLinData.m_ucMsgID].crcType        = nGetCalcChecksum(i, itr->second.m_ouLinData.m_ucMsgID);//OCI_LIN_CHECKSUM_ENHANCED;    // Use enhanced checksum for transmissions on this frame id.
            sg_asChannel[i].m_OCI_FlexRayConfig.messages[itr->second.m_ouLinData.m_ucMsgID].dlc            = itr->second.m_ouLinData.m_ucDataLen;                            // The frames will have 8 bytes of payload.
            sg_asChannel[i].m_OCI_FlexRayConfig.messages[itr->second.m_ouLinData.m_ucMsgID].mode           = OCI_LIN_FRAME_DEFINED;        // Declare that the details of frame ID 3 are defined.
        }


        OciErrorCode = (*sBOA_PTRS.m_sOCI.openLINController)(sg_asChannel[i].m_OCI_HwHandle,
                       &(sg_asChannel[i].m_OCI_FlexRayConfig),
                       &(sg_asChannel[i].m_OCI_CntrlProp));

        if(OciErrorCode != OCI_SUCCESS)
        {
            OciErrorCode = hRecreateLINController(i);
        }

        OCI_LINTxQueueConfiguration m_OCI_NewTxQueueCfg;

        itr = sg_asChannel[i].m_ouBoaLINData.begin();
        for ( ; itr != sg_asChannel[i].m_ouBoaLINData.end(); itr++ )
        {
            m_OCI_NewTxQueueCfg.type = OCI_LIN_FRAME_QUEUE;
            m_OCI_NewTxQueueCfg.frameID                     = itr->second.m_ouLinData.m_ucMsgID;                  // Unconditional Frame ID for messages to be transmitted
            m_OCI_NewTxQueueCfg.assignedEventFrameID0to31   = 0x0;                  // No Event-triggered frame ID
            m_OCI_NewTxQueueCfg.assignedEventFrameID32to59  = 0x0;


            OciErrorCode = (*(sBOA_PTRS.m_sOCI.linioVTable.createLINTxQueue))
                           (sg_asChannel[i].m_OCI_HwHandle, &(m_OCI_NewTxQueueCfg), &(itr->second.m_OCI_TxQueueHandle));
        }
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

HRESULT CDIL_LIN_ETAS_BOA::hRecreateLINController(INT nIndex)
{
    HRESULT hResult = S_FALSE;
    OCI_ErrorCode OciErrorCode = OCI_SUCCESS;

    sg_asChannel[nIndex].m_OCI_CntrlProp.mode = OCI_CONTROLLER_MODE_SUSPENDED;
    OciErrorCode == (*(sBOA_PTRS.m_sOCI.setLINControllerProperties))(sg_asChannel[nIndex].m_OCI_HwHandle,
            &(sg_asChannel[nIndex].m_OCI_CntrlProp));

    // First destroy LIN Controller
    (*(sBOA_PTRS.m_sOCI.destroyLINController))(sg_asChannel[nIndex].m_OCI_HwHandle);

    OciErrorCode =  (*(sBOA_PTRS.m_sOCI.createLINController))(sg_asChannel[nIndex].m_acURI,
                    &sg_asChannel[nIndex].m_OCI_HwHandle);
    if (OciErrorCode == OCI_SUCCESS)
    {
        //sg_asChannel[i].m_OCI_HwHandle = m_OCIHandle;
        hResult = (*(sBOA_PTRS.m_sOCI.timeVTable.getTimerCapabilities))(sg_asChannel[nIndex].m_OCI_HwHandle,
                  &(sg_asChannel[nIndex].m_OCI_TimerCapabilities));
        if ( hResult == OCI_SUCCESS )
        {
            sg_asChannel[nIndex].m_fResolution = (float)10000 /(float)(sg_asChannel[nIndex].m_OCI_TimerCapabilities.tickFrequency);
        }

        hResult = (*(sBOA_PTRS.m_sOCI.linioVTable.getLINControllerCapabilities))(sg_asChannel[nIndex].m_OCI_HwHandle, &sg_asChannel[nIndex].m_ControllerCapabilities);

        OciErrorCode = (*sBOA_PTRS.m_sOCI.openLINController)(sg_asChannel[nIndex].m_OCI_HwHandle,
                       &(sg_asChannel[nIndex].m_OCI_FlexRayConfig),
                       &(sg_asChannel[nIndex].m_OCI_CntrlProp));
        hResult = S_OK;
    }
    else
    {
        hResult = ERR_LOAD_HW_INTERFACE;
        sg_pIlog->vLogAMessage(A2T(__FILE__), __LINE__, _("could not create controller"));
    }

    return OciErrorCode;
}

HRESULT CDIL_LIN_ETAS_BOA::LIN_StopHardware(void)
{
    VALIDATE_VALUE_RETURN_VAL(sg_bCurrState, STATE_CONNECTED, ERR_IMPROPER_STATE);

    HRESULT hResult = S_OK;
    m_bLINConnected = FALSE;
    OCI_ErrorCode ociErrorCode = OCI_FAILURE;
    for (UINT i = 0; i < sg_nNoOfChannels; i++)
    {
        ociErrorCode = (*(sBOA_PTRS.m_sOCI.getLINControllerProperties))(sg_asChannel[i].m_OCI_HwHandle,
                       &(sg_asChannel[i].m_OCI_CntrlProp));
        if (ociErrorCode == OCI_SUCCESS)
        {
            if (sg_asChannel[i].m_OCI_CntrlProp.mode == OCI_CONTROLLER_MODE_RUNNING)
            {
                sg_asChannel[i].m_OCI_CntrlProp.mode = OCI_CONTROLLER_MODE_SUSPENDED;
                ociErrorCode == (*(sBOA_PTRS.m_sOCI.setLINControllerProperties))(sg_asChannel[i].m_OCI_HwHandle,
                        &(sg_asChannel[i].m_OCI_CntrlProp));
                if (ociErrorCode == OCI_SUCCESS)
                {
                    hResult |= S_OK;

                    // TO DO: LIN
                    //Clear the Data
                    EnterCriticalSection(&sg_asChannel[i].m_ouCriticalSection);
                    std::map<UINT, BOA_LINData>::iterator itrBoaData = sg_asChannel[i].m_ouBoaLINData.begin();
                    for ( ; itrBoaData != sg_asChannel[i].m_ouBoaLINData.end(); itrBoaData++ )
                    {
                        //No use of Return value;
                        (*(sBOA_PTRS.m_sOCI.linioVTable.destroyLINTxQueue))(itrBoaData->second.m_OCI_TxQueueHandle);
                    }
                    sg_asChannel[i].m_ouBoaLINData.clear();
                    LeaveCriticalSection(&sg_asChannel[i].m_ouCriticalSection);

                    ociErrorCode = (*sBOA_PTRS.m_sOCI.closeLINController)(sg_asChannel[i].m_OCI_HwHandle);


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
        //DeleteCriticalSection(&sg_CritSectForAckBuf);
    }

    return hResult;
}

/**
* \brief         This will send a LIN message to the driver. In case of USB
*                this will write the message in to the driver buffer and will
*                return. In case if parallel port mode this will write the
*                message and will wait for the ACK event from the driver. If
*                the event fired this will return 0. Otherwise this will return
*                wait time out error. In parallel port it is a blocking call
*                and in case of failure condition this will take 2 seconds.
* \param[in]     sMessage Message to Transmit
* \return        Operation Result. 0 incase of no errors. Failure Error codes otherwise.
* \authors       Arunkumar Karri
* \date          07.26.2013 Created
*/
HRESULT CDIL_LIN_ETAS_BOA::nWriteMessage(STLIN_MSG& ouData)
{
    BOA_ResultCode ErrCode;
    uint32 nRemaining = 0;
    EnterCriticalSection(&sg_asChannel[ouData.m_ucChannel-1].m_ouCriticalSection);

    std::map<UINT, BOA_LINData>::iterator itr = sg_asChannel[ouData.m_ucChannel-1].m_ouBoaLINData.find(ouData.m_ucMsgID);

    if ( itr != sg_asChannel[ouData.m_ucChannel-1].m_ouBoaLINData.end() )
    {
        OCI_LINMessage msg;
        msg.type                    = OCI_LIN_TX_MESSAGE;
        msg.reserved                = 0;
        msg.data.txMessage.flags    = 0;

        memcpy(msg.data.txMessage.data, ouData.m_ucData, ouData.m_ucDataLen);

        ErrCode = (*(sBOA_PTRS.m_sOCI.linioVTable.writeLINData))( itr->second.m_OCI_TxQueueHandle, OCI_NO_TIME, &msg, 1, &nRemaining);
    }

    LeaveCriticalSection(&sg_asChannel[ouData.m_ucChannel-1].m_ouCriticalSection);
    return S_OK;
}

HRESULT CDIL_LIN_ETAS_BOA::LIN_Send(STLIN_MSG& pouFlxTxMsg)
{
    HRESULT hResult = S_OK;

    EnterCriticalSection(&sg_asChannel[pouFlxTxMsg.m_ucChannel-1].m_ouCriticalSection);

    if (m_bLINConnected == FALSE )          //Controller Not active
    {
        BOA_LINData ouData;
        ouData.m_OCI_TxQueueHandle = 0;
        ouData.m_ouLinData =  pouFlxTxMsg;
        sg_asChannel[pouFlxTxMsg.m_ucChannel-1].m_ouBoaLINData[pouFlxTxMsg.m_ucMsgID]  = ouData;
    }
    else            //Active
    {
        nWriteMessage(pouFlxTxMsg);
    }

    LeaveCriticalSection(&sg_asChannel[0].m_ouCriticalSection);

    return hResult;
}

HRESULT CDIL_LIN_ETAS_BOA::LIN_GetLastErrorString(std::string & acErrorStr)
{
    acErrorStr = sg_acErrStr;
    return S_OK;
}

HRESULT CDIL_LIN_ETAS_BOA::LIN_SetControllerParams(int nValue, ECONTR_PARAM eContrparam)
{
    switch(eContrparam)
    {
        case HW_MODE:
        {
            switch(nValue)
            {
                case defMODE_ACTIVE:
                {
                    for (UINT i = 0; i < sg_nNoOfChannels;)
                    {
                        OCI_LINControllerProperties val;
                        val.mode = OCI_CONTROLLER_MODE_RUNNING;
                        (*(sBOA_PTRS.m_sOCI.setLINControllerProperties))(sg_asChannel[i].m_OCI_HwHandle,
                                &val);
                        i++;

                    }
                }
                break;
                case defMODE_PASSIVE:
                {
                    for (UINT i = 0; i < sg_nNoOfChannels;)
                    {
                        OCI_LINControllerProperties val;
                        val.mode  = OCI_CONTROLLER_MODE_SUSPENDED;
                        (*(sBOA_PTRS.m_sOCI.setLINControllerProperties))(sg_asChannel[i].m_OCI_HwHandle,
                                &val);
                        i++;

                    }
                }
                break;
            }
        }
    }
    return S_OK;
}

HRESULT CDIL_LIN_ETAS_BOA::LIN_GetConfiguration(sCONTROLLERDETAILSLIN psControllerConfig[], INT& nSize)
{
    for ( int i = 0; i < sg_nNoOfChannels; i++ )
    {
        psControllerConfig[i].m_strHwUri =  (char*)(sg_asChannel[i].m_acURI);
        psControllerConfig[i].m_nBaudRate =  (sg_asChannel[i].m_OCI_FlexRayConfig.baudrate);
        psControllerConfig[i].m_strProtocolVersion = (sg_asChannel[i].m_strLinVersion);
    }
    nSize = sg_nNoOfChannels;
    return S_OK;
}


/**
 * Gets the controller param eContrParam of the channel.
 * Value stored in lParam.
 */
HRESULT CDIL_LIN_ETAS_BOA::LIN_GetControllerParams(LONG& lParam, UINT nChannel, ECONTR_PARAM eContrParam)
{
    HRESULT hResult = S_OK;
    if ((sg_bCurrState == STATE_HW_INTERFACE_SELECTED) || (sg_bCurrState == STATE_CONNECTED))
    {
        switch (eContrParam)
        {
            case NUMBER_HW:
            {
                lParam = sg_nNoOfChannels;
            }
            break;
            case NUMBER_CONNECTED_HW:
            {
                lParam = sg_nNoOfChannels;
            }
            break;
            case DRIVER_STATUS:
            {
                lParam = sg_bIsDriverRunning;
            }
            break;
            case HW_MODE:
            {
                if (nChannel < sg_nNoOfChannels)
                {
                    lParam = defMODE_ACTIVE;
                }
                else
                {
                    //unknown
                    lParam = defCONTROLLER_BUSOFF + 1;
                }

            }
            break;
            case CON_TEST:
            {
                lParam = TRUE;
            }
            break;
            default:
            {
                hResult = S_FALSE;
            }
            break;

        }
    }
    else
    {
        hResult = ERR_IMPROPER_STATE;
    }
    return hResult;
}

/**
 * \return S_OK for success, S_FALSE for failure
 *
 * Returns the controller status. hEvent will be registered
 * and will be set whenever there is change in the controller
 * status.
 */
HRESULT CDIL_LIN_ETAS_BOA::LIN_GetCntrlStatus(const HANDLE& /*hEvent*/, UINT& unCntrlStatus)
{
    unCntrlStatus = defCONTROLLER_ACTIVE; //Temporary solution. TODO
    return S_OK;
}

/**
 * Gets the error counter for corresponding channel.
 */
HRESULT CDIL_LIN_ETAS_BOA::LIN_GetErrorCount(SERROR_CNT& sErrorCnt, UINT nChannel, ECONTR_PARAM eContrParam)
{
    HRESULT hResult = S_OK;
    if ((sg_bCurrState == STATE_CONNECTED) || (sg_bCurrState == STATE_HW_INTERFACE_SELECTED))
    {
        if (nChannel <= sg_nNoOfChannels)
        {
            if (eContrParam == ERR_CNT)
            {
                sErrorCnt.m_ucTxErrCount = sg_asChannel[nChannel].m_ucTxErrorCounter;
                sErrorCnt.m_ucRxErrCount = sg_asChannel[nChannel].m_ucRxErrorCounter;
            }
            else if (eContrParam == PEAK_ERR_CNT)
            {
                sErrorCnt.m_ucTxErrCount = sg_asChannel[nChannel].m_ucPeakTxErrorCounter;
                sErrorCnt.m_ucRxErrCount = sg_asChannel[nChannel].m_ucPeakRxErrorCounter;
            }
        }
        else
        {
            hResult = ERR_INVALID_CHANNEL;
        }
    }
    else
    {
        hResult = ERR_IMPROPER_STATE;
    }
    return hResult;
}