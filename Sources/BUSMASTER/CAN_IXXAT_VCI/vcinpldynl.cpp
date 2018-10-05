/*****************************************************************************
 IXXAT Automation GmbH
******************************************************************************

 File    : VCINPLDYNL.C
 Summary : Native VCI Programming Library.- for dynamic loading of vcinpl.dll

 Date    : 2007-07-03
 Author  : Michael Ummenhofer

 Compiler: MSVC

******************************************************************************
 all rights reserved
*****************************************************************************/
#include "CAN_IXXAT_VCI_stdafx.h"
#include <windows.h>
#include "vcinpldynl.h"

/*****************************************************************************
* constants and macros
*****************************************************************************/

//*** DLL access Macros
#define DECLARE_FUNCTION_POINTER(FUNC)  PF_NPL_##FUNC DYNFUNC(FUNC)=nullptr;
#define LOAD_FUNCTION_POINTER(DLL,FUNC) DYNFUNC(FUNC) = (PF_NPL_##FUNC)GetProcAddress(DLL, #FUNC);\
    lProcAddr &= DYNFUNC(FUNC)!= nullptr?1:0;
#define UNLOAD_FUNCTION_POINTER(FUNC)   DYNFUNC(FUNC) = nullptr;

//*** Expected Major Version Numbers
#define VCI_V3_NPL_MAJOR_VERION 3
#define VCI_V4_NPL_MAJOR_VERION 4

//*** VCI V3 NPL DLL Name
const TCHAR k_tszVciV3NplDllName[] = TEXT("vcinpl.dll");


/*****************************************************************************
* global variables
*****************************************************************************/

/*****************************************************************************
 * general VCI functions
 ****************************************************************************/
DECLARE_FUNCTION_POINTER(vciInitialize);
DECLARE_FUNCTION_POINTER(vciFormatError);
DECLARE_FUNCTION_POINTER(vciDisplayError);
DECLARE_FUNCTION_POINTER(vciGetVersion);
DECLARE_FUNCTION_POINTER(vciLuidToChar);
DECLARE_FUNCTION_POINTER(vciCharToLuid);
DECLARE_FUNCTION_POINTER(vciGuidToChar);
DECLARE_FUNCTION_POINTER(vciCharToGuid);

/*****************************************************************************
 * event specific functions
 ****************************************************************************/
DECLARE_FUNCTION_POINTER(vciEventCreate);
DECLARE_FUNCTION_POINTER(vciEventDelete);
DECLARE_FUNCTION_POINTER(vciEventSignal);
DECLARE_FUNCTION_POINTER(vciEventReset);
DECLARE_FUNCTION_POINTER(vciEventWaitFor);

/*****************************************************************************
 * device manager specific functions
 ****************************************************************************/
DECLARE_FUNCTION_POINTER(vciEnumDeviceOpen);
DECLARE_FUNCTION_POINTER(vciEnumDeviceClose );
DECLARE_FUNCTION_POINTER(vciEnumDeviceNext);
DECLARE_FUNCTION_POINTER(vciEnumDeviceReset);
DECLARE_FUNCTION_POINTER(vciEnumDeviceWaitEvent);
DECLARE_FUNCTION_POINTER(vciFindDeviceByHwid);
DECLARE_FUNCTION_POINTER(vciFindDeviceByClass);
DECLARE_FUNCTION_POINTER(vciSelectDeviceDlg);

/*****************************************************************************
 * device specific functions
 ****************************************************************************/
DECLARE_FUNCTION_POINTER(vciDeviceOpen);
DECLARE_FUNCTION_POINTER(vciDeviceOpenDlg);
DECLARE_FUNCTION_POINTER(vciDeviceClose);
DECLARE_FUNCTION_POINTER(vciDeviceGetInfo);
DECLARE_FUNCTION_POINTER(vciDeviceGetCaps);

/*****************************************************************************
 * CAN controller specific functions
 ****************************************************************************/
DECLARE_FUNCTION_POINTER(canControlOpen);
DECLARE_FUNCTION_POINTER(canControlClose);
DECLARE_FUNCTION_POINTER(canControlGetCaps);
DECLARE_FUNCTION_POINTER(canControlGetStatus);
DECLARE_FUNCTION_POINTER(canControlDetectBitrate);
DECLARE_FUNCTION_POINTER(canControlInitialize);
DECLARE_FUNCTION_POINTER(canControlReset);
DECLARE_FUNCTION_POINTER(canControlStart);
DECLARE_FUNCTION_POINTER(canControlSetAccFilter);
DECLARE_FUNCTION_POINTER(canControlAddFilterIds);
DECLARE_FUNCTION_POINTER(canControlRemFilterIds);

/*****************************************************************************
 * CAN message channel specific functions
 ****************************************************************************/
DECLARE_FUNCTION_POINTER(canChannelOpen);
DECLARE_FUNCTION_POINTER(canChannelClose);
DECLARE_FUNCTION_POINTER(canChannelGetCaps);
DECLARE_FUNCTION_POINTER(canChannelGetStatus);
DECLARE_FUNCTION_POINTER(canChannelInitialize);
DECLARE_FUNCTION_POINTER(canChannelActivate);
DECLARE_FUNCTION_POINTER(canChannelPeekMessage);
DECLARE_FUNCTION_POINTER(canChannelPostMessage);
DECLARE_FUNCTION_POINTER(canChannelWaitRxEvent);
DECLARE_FUNCTION_POINTER(canChannelWaitTxEvent);
DECLARE_FUNCTION_POINTER(canChannelReadMessage);
DECLARE_FUNCTION_POINTER(canChannelReadMultipleMessages);
DECLARE_FUNCTION_POINTER(canChannelSendMessage);

/*****************************************************************************
 * cyclic CAN message scheduler specific functions
 ****************************************************************************/
DECLARE_FUNCTION_POINTER(canSchedulerOpen);
DECLARE_FUNCTION_POINTER(canSchedulerClose);
DECLARE_FUNCTION_POINTER(canSchedulerGetCaps);
DECLARE_FUNCTION_POINTER(canSchedulerGetStatus);
DECLARE_FUNCTION_POINTER(canSchedulerActivate);
DECLARE_FUNCTION_POINTER(canSchedulerReset);
DECLARE_FUNCTION_POINTER(canSchedulerAddMessage);
DECLARE_FUNCTION_POINTER(canSchedulerRemMessage);
DECLARE_FUNCTION_POINTER(canSchedulerStartMessage);
DECLARE_FUNCTION_POINTER(canSchedulerStopMessage);

/*****************************************************************************
 * LIN controller specific functions
 ****************************************************************************/
DECLARE_FUNCTION_POINTER(linControlOpen);
DECLARE_FUNCTION_POINTER(linControlClose);
DECLARE_FUNCTION_POINTER(linControlGetCaps);
DECLARE_FUNCTION_POINTER(linControlGetStatus);
DECLARE_FUNCTION_POINTER(linControlInitialize);
DECLARE_FUNCTION_POINTER(linControlReset);
DECLARE_FUNCTION_POINTER(linControlStart);
DECLARE_FUNCTION_POINTER(linControlWriteMessage);

/*****************************************************************************
 * LIN message monitor specific functions
 ****************************************************************************/
DECLARE_FUNCTION_POINTER(linMonitorOpen);
DECLARE_FUNCTION_POINTER(linMonitorClose);
DECLARE_FUNCTION_POINTER(linMonitorGetCaps);
DECLARE_FUNCTION_POINTER(linMonitorGetStatus);
DECLARE_FUNCTION_POINTER(linMonitorInitialize);
DECLARE_FUNCTION_POINTER(linMonitorActivate);
DECLARE_FUNCTION_POINTER(linMonitorPeekMessage);
DECLARE_FUNCTION_POINTER(linMonitorWaitRxEvent);
DECLARE_FUNCTION_POINTER(linMonitorReadMessage);

/*****************************************************************************
* module global variables
*****************************************************************************/
static HINSTANCE g_hInstVciNplLib     = nullptr;
static DWORD     g_dwInstanceCounter  = 0;


/*****************************************************************************
 Function:
  LoadVciNplLib

 Description:
  loads the VciNpl.DLL dynamically

 Arguments:
  none

 Results:
  S_OK on success
  otherwise an error value
*****************************************************************************/
HRESULT LoadVciNplLib(void)
{
    HRESULT hResult         = VCI_OK;
    long    lProcAddr       = 0x1;
    UINT32  dwMajorVersion  = 0;
    UINT32  dwMinorVersion  = 0;

    //*** If DLL is not already loaded
    if(g_dwInstanceCounter == 0)
    {
        g_hInstVciNplLib = LoadLibrary(k_tszVciV3NplDllName);
        if(g_hInstVciNplLib)
        {
            //*** Load Function Pointers
            LOAD_FUNCTION_POINTER(g_hInstVciNplLib, vciInitialize);
            LOAD_FUNCTION_POINTER(g_hInstVciNplLib, vciGetVersion);

            if(!lProcAddr)  // at least one function was not found
            {
                hResult = GetLastError();
            }

            //*** Initialize DLL
            if(hResult == S_OK)
            {
                hResult = DYNCALL(vciInitialize)();
            }

            //*** Check Version Major Number
            if(hResult == S_OK)
                hResult = DYNCALL(vciGetVersion)(&dwMajorVersion,
                                                 &dwMinorVersion);
            if(hResult == S_OK)
            {
				// the set the result to bad environment as default
				// this can be made in one line but this here is more understandable
				hResult = ERROR_BAD_ENVIRONMENT;

                if (dwMajorVersion == VCI_V3_NPL_MAJOR_VERION) 
                {
					// if it's VCI version 3 then ok
					hResult = S_OK;
                }
				else if (dwMajorVersion == VCI_V4_NPL_MAJOR_VERION)
				{
					// if it's VCI version 4 then ok
					hResult = S_OK;
				}
            }

            //*** Unload Function Pointers
            UNLOAD_FUNCTION_POINTER(vciInitialize);
            UNLOAD_FUNCTION_POINTER(vciGetVersion);
        }
        else
        {
            hResult = GetLastError();
        }
    }//endif

    //*** Increment reference counter
    if(hResult == S_OK)
    {
        g_dwInstanceCounter++;
    }

    //*** Unload DLL on error, if loaded
    if((hResult != S_OK) && (g_hInstVciNplLib != nullptr))
    {
        FreeLibrary( g_hInstVciNplLib );
        g_hInstVciNplLib = nullptr;
    }

    return hResult;
}


/*****************************************************************************
 Function:
  FreeVciNplLib

 Description:
  free dynamically loaded VciNpl.DLL

 Arguments:
  none

  Results:
  S_OK on success
  otherwise an error value
*****************************************************************************/
HRESULT FreeVciNplLib(void)
{
    HRESULT hResult = S_OK;

    //*** Only one reference left?
    if(g_dwInstanceCounter == 1)
    {
        UnmapGeneralVciFunctions();
        UnmapEventFunctions();
        UnmapDeviceManagerFunctions();
        UnmapDeviceFunctions();
        UnmapCANControllerFunctions();
        UnmapCANMessageChannelFunctions();
        UnmapCANMessageSchedulerFunctions();
        UnmapLINControllerFunctions();
        UnmapLINMessageMonitorFunctions();

        //*** Free Library
        if(g_hInstVciNplLib != nullptr)
        {
            FreeLibrary(g_hInstVciNplLib);
            g_hInstVciNplLib = nullptr;
        }
    }

    //*** Dercement reference counter
    if(g_dwInstanceCounter > 0)
    {
        g_dwInstanceCounter--;
    }

    return hResult;
}


/*****************************************************************************
 Function:
  MapGeneralVciFunctions

 Description:
  Loads the function pointer addresses of the "general VCI functions"

 Arguments:
  none

  Results:
  S_OK on success
  otherwise an error value
*****************************************************************************/
HRESULT MapGeneralVciFunctions(void)
{
    HRESULT hResult   = CO_E_RELOAD_DLL;
    long    lProcAddr = 0x1;

    //*** Check if DLL is loaded
    if(g_hInstVciNplLib != nullptr)
    {
        LOAD_FUNCTION_POINTER(g_hInstVciNplLib, vciInitialize);
        LOAD_FUNCTION_POINTER(g_hInstVciNplLib, vciFormatError);
        LOAD_FUNCTION_POINTER(g_hInstVciNplLib, vciDisplayError);
        LOAD_FUNCTION_POINTER(g_hInstVciNplLib, vciGetVersion);
        LOAD_FUNCTION_POINTER(g_hInstVciNplLib, vciLuidToChar);
        LOAD_FUNCTION_POINTER(g_hInstVciNplLib, vciCharToLuid);
        LOAD_FUNCTION_POINTER(g_hInstVciNplLib, vciGuidToChar);
        LOAD_FUNCTION_POINTER(g_hInstVciNplLib, vciCharToGuid);

        if(!lProcAddr)  // at least one function was not found
        {
            hResult = GetLastError();
        }
        else
        {
            hResult = S_OK;
        }
    }

    return hResult;
}


/*****************************************************************************
 Function:
  UnmapGeneralVciFunctions

 Description:
  Unloads the function pointer addresses of the "general VCI functions"

 Arguments:
  none

  Results:
  S_OK on success
  otherwise an error value
*****************************************************************************/
HRESULT UnmapGeneralVciFunctions(void)
{
    HRESULT hResult = ERROR_REDIRECTOR_HAS_OPEN_HANDLES;

    //*** Unload function pointer if referenced once only
    if(g_dwInstanceCounter == 1)
    {
        UNLOAD_FUNCTION_POINTER(vciInitialize);
        UNLOAD_FUNCTION_POINTER(vciFormatError);
        UNLOAD_FUNCTION_POINTER(vciDisplayError);
        UNLOAD_FUNCTION_POINTER(vciGetVersion);
        UNLOAD_FUNCTION_POINTER(vciLuidToChar);
        UNLOAD_FUNCTION_POINTER(vciCharToLuid);
        UNLOAD_FUNCTION_POINTER(vciGuidToChar);
        UNLOAD_FUNCTION_POINTER(vciCharToGuid);

        hResult = S_OK;
    }

    return hResult;
}


/*****************************************************************************
 Function:
  MapEventFunctions

 Description:
  Loads the function pointer addresses of the "event specific functions"

 Arguments:
  none

  Results:
  S_OK on success
  otherwise an error value
*****************************************************************************/
HRESULT MapEventFunctions(void)
{
    HRESULT hResult   = CO_E_RELOAD_DLL;
    long    lProcAddr = 0x1;

    //*** Check if DLL is loaded
    if(g_hInstVciNplLib != nullptr)
    {
        LOAD_FUNCTION_POINTER(g_hInstVciNplLib, vciEventCreate);
        LOAD_FUNCTION_POINTER(g_hInstVciNplLib, vciEventDelete);
        LOAD_FUNCTION_POINTER(g_hInstVciNplLib, vciEventSignal);
        LOAD_FUNCTION_POINTER(g_hInstVciNplLib, vciEventReset);
        LOAD_FUNCTION_POINTER(g_hInstVciNplLib, vciEventWaitFor);

        if(!lProcAddr)  // at least one function was not found
        {
            hResult = GetLastError();
        }
        else
        {
            hResult = S_OK;
        }
    }

    return hResult;
}


/*****************************************************************************
 Function:
  UnmapEventFunctions

 Description:
  Unloads the function pointer addresses of the "event specific functions"

 Arguments:
  none

  Results:
  S_OK on success
  otherwise an error value
*****************************************************************************/
HRESULT UnmapEventFunctions(void)
{
    HRESULT hResult = ERROR_REDIRECTOR_HAS_OPEN_HANDLES;

    //*** Unload function pointer if referenced once only
    if(g_dwInstanceCounter == 1)
    {
        UNLOAD_FUNCTION_POINTER(vciEventCreate);
        UNLOAD_FUNCTION_POINTER(vciEventDelete);
        UNLOAD_FUNCTION_POINTER(vciEventSignal);
        UNLOAD_FUNCTION_POINTER(vciEventReset);
        UNLOAD_FUNCTION_POINTER(vciEventWaitFor);

        hResult = S_OK;
    }

    return hResult;
}


/*****************************************************************************
 Function:
  MapDeviceManagerFunctions

 Description:
  Loads the function pointer addresses of the "device manager specific
  functions"

 Arguments:
  none

  Results:
  S_OK on success
  otherwise an error value
*****************************************************************************/
HRESULT MapDeviceManagerFunctions(void)
{
    HRESULT hResult   = CO_E_RELOAD_DLL;
    long    lProcAddr = 0x1;

    //*** Check if DLL is loaded
    if(g_hInstVciNplLib != nullptr)
    {
        LOAD_FUNCTION_POINTER(g_hInstVciNplLib, vciEnumDeviceOpen);
        LOAD_FUNCTION_POINTER(g_hInstVciNplLib, vciEnumDeviceClose );
        LOAD_FUNCTION_POINTER(g_hInstVciNplLib, vciEnumDeviceNext);
        LOAD_FUNCTION_POINTER(g_hInstVciNplLib, vciEnumDeviceReset);
        LOAD_FUNCTION_POINTER(g_hInstVciNplLib, vciEnumDeviceWaitEvent);
        LOAD_FUNCTION_POINTER(g_hInstVciNplLib, vciFindDeviceByHwid);
        LOAD_FUNCTION_POINTER(g_hInstVciNplLib, vciFindDeviceByClass);
        LOAD_FUNCTION_POINTER(g_hInstVciNplLib, vciSelectDeviceDlg);

        if(!lProcAddr)  // at least one function was not found
        {
            hResult = GetLastError();
        }
        else
        {
            hResult = S_OK;
        }
    }

    return hResult;
}


/*****************************************************************************
 Function:
  UnmapDeviceManagerFunctions

 Description:
  Unloads the function pointer addresses of the "device manager specific
  functions"

 Arguments:
  none

  Results:
  S_OK on success
  otherwise an error value
*****************************************************************************/
HRESULT UnmapDeviceManagerFunctions(void)
{
    HRESULT hResult = ERROR_REDIRECTOR_HAS_OPEN_HANDLES;

    //*** Unload function pointer if referenced once only
    if(g_dwInstanceCounter == 1)
    {
        UNLOAD_FUNCTION_POINTER(vciEnumDeviceOpen);
        UNLOAD_FUNCTION_POINTER(vciEnumDeviceClose );
        UNLOAD_FUNCTION_POINTER(vciEnumDeviceNext);
        UNLOAD_FUNCTION_POINTER(vciEnumDeviceReset);
        UNLOAD_FUNCTION_POINTER(vciEnumDeviceWaitEvent);
        UNLOAD_FUNCTION_POINTER(vciFindDeviceByHwid);
        UNLOAD_FUNCTION_POINTER(vciFindDeviceByClass);
        UNLOAD_FUNCTION_POINTER(vciSelectDeviceDlg);

        hResult = S_OK;
    }

    return hResult;
}


/*****************************************************************************
 Function:
  MapDeviceFunctions

 Description:
  Loads the function pointer addresses of the "device specific functions"

 Arguments:
  none

  Results:
  S_OK on success
  otherwise an error value
*****************************************************************************/
HRESULT MapDeviceFunctions(void)
{
    HRESULT hResult   = CO_E_RELOAD_DLL;
    long    lProcAddr = 0x1;

    //*** Check if DLL is loaded
    if(g_hInstVciNplLib != nullptr)
    {
        LOAD_FUNCTION_POINTER(g_hInstVciNplLib, vciDeviceOpen);
        LOAD_FUNCTION_POINTER(g_hInstVciNplLib, vciDeviceOpenDlg);
        LOAD_FUNCTION_POINTER(g_hInstVciNplLib, vciDeviceClose);
        LOAD_FUNCTION_POINTER(g_hInstVciNplLib, vciDeviceGetInfo);
        LOAD_FUNCTION_POINTER(g_hInstVciNplLib, vciDeviceGetCaps);

        if(!lProcAddr)  // at least one function was not found
        {
            hResult = GetLastError();
        }
        else
        {
            hResult = S_OK;
        }
    }

    return hResult;
}


/*****************************************************************************
 Function:
  UnmapDeviceFunctions

 Description:
  Unloads the function pointer addresses of the "device specific functions"

 Arguments:
  none

  Results:
  S_OK on success
  otherwise an error value
*****************************************************************************/
HRESULT UnmapDeviceFunctions(void)
{
    HRESULT hResult = ERROR_REDIRECTOR_HAS_OPEN_HANDLES;

    //*** Unload function pointer if referenced once only
    if(g_dwInstanceCounter == 1)
    {
        UNLOAD_FUNCTION_POINTER(vciDeviceOpen);
        UNLOAD_FUNCTION_POINTER(vciDeviceOpenDlg);
        UNLOAD_FUNCTION_POINTER(vciDeviceClose);
        UNLOAD_FUNCTION_POINTER(vciDeviceGetInfo);
        UNLOAD_FUNCTION_POINTER(vciDeviceGetCaps);

        hResult = S_OK;
    }

    return hResult;
}


/*****************************************************************************
 Function:
  MapCANControllerFunctions

 Description:
  Loads the function pointer addresses of the "CAN controller specific
  functions"

 Arguments:
  none

  Results:
  S_OK on success
  otherwise an error value
*****************************************************************************/
HRESULT MapCANControllerFunctions(void)
{
    HRESULT hResult   = CO_E_RELOAD_DLL;
    long    lProcAddr = 0x1;

    //*** Check if DLL is loaded
    if(g_hInstVciNplLib != nullptr)
    {
        LOAD_FUNCTION_POINTER(g_hInstVciNplLib, canControlOpen);
        LOAD_FUNCTION_POINTER(g_hInstVciNplLib, canControlClose);
        LOAD_FUNCTION_POINTER(g_hInstVciNplLib, canControlGetCaps);
        LOAD_FUNCTION_POINTER(g_hInstVciNplLib, canControlGetStatus);
        LOAD_FUNCTION_POINTER(g_hInstVciNplLib, canControlDetectBitrate);
        LOAD_FUNCTION_POINTER(g_hInstVciNplLib, canControlInitialize);
        LOAD_FUNCTION_POINTER(g_hInstVciNplLib, canControlReset);
        LOAD_FUNCTION_POINTER(g_hInstVciNplLib, canControlStart);
        LOAD_FUNCTION_POINTER(g_hInstVciNplLib, canControlSetAccFilter);
        LOAD_FUNCTION_POINTER(g_hInstVciNplLib, canControlAddFilterIds);
        LOAD_FUNCTION_POINTER(g_hInstVciNplLib, canControlRemFilterIds);

        if(!lProcAddr)  // at least one function was not found
        {
            hResult = GetLastError();
        }
        else
        {
            hResult = S_OK;
        }
    }

    return hResult;
}


/*****************************************************************************
 Function:
  UnmapCANControllerFunctions

 Description:
  Unloads the function pointer addresses of the "CAN controller specific
  functions"

 Arguments:
  none

  Results:
  S_OK on success
  otherwise an error value
*****************************************************************************/
HRESULT UnmapCANControllerFunctions(void)
{
    HRESULT hResult = ERROR_REDIRECTOR_HAS_OPEN_HANDLES;

    //*** Unload function pointer if referenced once only
    if(g_dwInstanceCounter == 1)
    {
        UNLOAD_FUNCTION_POINTER(canControlOpen);
        UNLOAD_FUNCTION_POINTER(canControlClose);
        UNLOAD_FUNCTION_POINTER(canControlGetCaps);
        UNLOAD_FUNCTION_POINTER(canControlGetStatus);
        UNLOAD_FUNCTION_POINTER(canControlDetectBitrate);
        UNLOAD_FUNCTION_POINTER(canControlInitialize);
        UNLOAD_FUNCTION_POINTER(canControlReset);
        UNLOAD_FUNCTION_POINTER(canControlStart);
        UNLOAD_FUNCTION_POINTER(canControlSetAccFilter);
        UNLOAD_FUNCTION_POINTER(canControlAddFilterIds);
        UNLOAD_FUNCTION_POINTER(canControlRemFilterIds);

        hResult = S_OK;
    }

    return hResult;
}


/*****************************************************************************
 Function:
  MapCANMessageChannelFunctions

 Description:
  Loads the function pointer addresses of the "CAN message channel specific
  functions"

 Arguments:
  none

  Results:
  S_OK on success
  otherwise an error value
*****************************************************************************/
HRESULT MapCANMessageChannelFunctions(void)
{
    HRESULT hResult   = CO_E_RELOAD_DLL;
    long    lProcAddr = 0x1;

    //*** Check if DLL is loaded
    if(g_hInstVciNplLib != nullptr)
    {
        LOAD_FUNCTION_POINTER(g_hInstVciNplLib, canChannelOpen);
        LOAD_FUNCTION_POINTER(g_hInstVciNplLib, canChannelClose);
        LOAD_FUNCTION_POINTER(g_hInstVciNplLib, canChannelGetCaps);
        LOAD_FUNCTION_POINTER(g_hInstVciNplLib, canChannelGetStatus);
        LOAD_FUNCTION_POINTER(g_hInstVciNplLib, canChannelInitialize);
        LOAD_FUNCTION_POINTER(g_hInstVciNplLib, canChannelActivate);
        LOAD_FUNCTION_POINTER(g_hInstVciNplLib, canChannelPeekMessage);
        LOAD_FUNCTION_POINTER(g_hInstVciNplLib, canChannelPostMessage);
        LOAD_FUNCTION_POINTER(g_hInstVciNplLib, canChannelWaitRxEvent);
        LOAD_FUNCTION_POINTER(g_hInstVciNplLib, canChannelWaitTxEvent);
        LOAD_FUNCTION_POINTER(g_hInstVciNplLib, canChannelReadMessage);
        LOAD_FUNCTION_POINTER(g_hInstVciNplLib, canChannelReadMultipleMessages);
        LOAD_FUNCTION_POINTER(g_hInstVciNplLib, canChannelSendMessage);

        if(!lProcAddr)  // at least one function was not found
        {
            hResult = GetLastError();
        }
        else
        {
            hResult = S_OK;
        }
    }

    return hResult;
}


/*****************************************************************************
 Function:
  UnmapCANMessageChannelFunctions

 Description:
  Unloads the function pointer addresses of the "CAN message channel specific
  functions"

 Arguments:
  none

  Results:
  S_OK on success
  otherwise an error value
*****************************************************************************/
HRESULT UnmapCANMessageChannelFunctions(void)
{
    HRESULT hResult = ERROR_REDIRECTOR_HAS_OPEN_HANDLES;

    //*** Unload function pointer if referenced once only
    if(g_dwInstanceCounter == 1)
    {
        UNLOAD_FUNCTION_POINTER(canChannelOpen);
        UNLOAD_FUNCTION_POINTER(canChannelClose);
        UNLOAD_FUNCTION_POINTER(canChannelGetCaps);
        UNLOAD_FUNCTION_POINTER(canChannelGetStatus);
        UNLOAD_FUNCTION_POINTER(canChannelInitialize);
        UNLOAD_FUNCTION_POINTER(canChannelActivate);
        UNLOAD_FUNCTION_POINTER(canChannelPeekMessage);
        UNLOAD_FUNCTION_POINTER(canChannelPostMessage);
        UNLOAD_FUNCTION_POINTER(canChannelWaitRxEvent);
        UNLOAD_FUNCTION_POINTER(canChannelWaitTxEvent);
        UNLOAD_FUNCTION_POINTER(canChannelReadMessage);
        UNLOAD_FUNCTION_POINTER(canChannelReadMultipleMessages);
        UNLOAD_FUNCTION_POINTER(canChannelSendMessage);

        hResult = S_OK;
    }

    return hResult;
}


/*****************************************************************************
 Function:
  MapCANMsgSchedulerFunctions

 Description:
  Loads the function pointer addresses of the "cyclic CAN message scheduler
  specific functions"

 Arguments:
  none

  Results:
  S_OK on success
  otherwise an error value
*****************************************************************************/
HRESULT MapCANMsgSchedulerFunctions(void)
{
    HRESULT hResult   = CO_E_RELOAD_DLL;
    long    lProcAddr = 0x1;

    //*** Check if DLL is loaded
    if(g_hInstVciNplLib != nullptr)
    {
        LOAD_FUNCTION_POINTER(g_hInstVciNplLib, canSchedulerOpen);
        LOAD_FUNCTION_POINTER(g_hInstVciNplLib, canSchedulerClose);
        LOAD_FUNCTION_POINTER(g_hInstVciNplLib, canSchedulerGetCaps);
        LOAD_FUNCTION_POINTER(g_hInstVciNplLib, canSchedulerGetStatus);
        LOAD_FUNCTION_POINTER(g_hInstVciNplLib, canSchedulerActivate);
        LOAD_FUNCTION_POINTER(g_hInstVciNplLib, canSchedulerReset);
        LOAD_FUNCTION_POINTER(g_hInstVciNplLib, canSchedulerAddMessage);
        LOAD_FUNCTION_POINTER(g_hInstVciNplLib, canSchedulerRemMessage);
        LOAD_FUNCTION_POINTER(g_hInstVciNplLib, canSchedulerStartMessage);
        LOAD_FUNCTION_POINTER(g_hInstVciNplLib, canSchedulerStopMessage);

        if(!lProcAddr)  // at least one function was not found
        {
            hResult = GetLastError();
        }
        else
        {
            hResult = S_OK;
        }
    }

    return hResult;
}


/*****************************************************************************
 Function:
  UnmapCANMessageSchedulerFunctions

 Description:
  Unloads the function pointer addresses of the "cyclic CAN message scheduler
  specific functions"

 Arguments:
  none

  Results:
  S_OK on success
  otherwise an error value
*****************************************************************************/
HRESULT UnmapCANMessageSchedulerFunctions(void)
{
    HRESULT hResult = ERROR_REDIRECTOR_HAS_OPEN_HANDLES;

    //*** Unload function pointer if referenced once only
    if(g_dwInstanceCounter == 1)
    {
        UNLOAD_FUNCTION_POINTER(canSchedulerOpen);
        UNLOAD_FUNCTION_POINTER(canSchedulerClose);
        UNLOAD_FUNCTION_POINTER(canSchedulerGetCaps);
        UNLOAD_FUNCTION_POINTER(canSchedulerGetStatus);
        UNLOAD_FUNCTION_POINTER(canSchedulerActivate);
        UNLOAD_FUNCTION_POINTER(canSchedulerReset);
        UNLOAD_FUNCTION_POINTER(canSchedulerAddMessage);
        UNLOAD_FUNCTION_POINTER(canSchedulerRemMessage);
        UNLOAD_FUNCTION_POINTER(canSchedulerStartMessage);
        UNLOAD_FUNCTION_POINTER(canSchedulerStopMessage);

        hResult = S_OK;
    }

    return hResult;
}


/*****************************************************************************
 Function:
  MapLINControllerFunctions

 Description:
  Loads the function pointer addresses of the "LIN controller specific
  functions"

 Arguments:
  none

  Results:
  S_OK on success
  otherwise an error value
*****************************************************************************/
HRESULT MapLINControllerFunctions(void)
{
    HRESULT hResult   = CO_E_RELOAD_DLL;
    long    lProcAddr = 0x1;

    //*** Check if DLL is loaded
    if(g_hInstVciNplLib != nullptr)
    {
        LOAD_FUNCTION_POINTER(g_hInstVciNplLib, linControlOpen);
        LOAD_FUNCTION_POINTER(g_hInstVciNplLib, linControlClose);
        LOAD_FUNCTION_POINTER(g_hInstVciNplLib, linControlGetCaps);
        LOAD_FUNCTION_POINTER(g_hInstVciNplLib, linControlGetStatus);
        LOAD_FUNCTION_POINTER(g_hInstVciNplLib, linControlInitialize);
        LOAD_FUNCTION_POINTER(g_hInstVciNplLib, linControlReset);
        LOAD_FUNCTION_POINTER(g_hInstVciNplLib, linControlStart);
        LOAD_FUNCTION_POINTER(g_hInstVciNplLib, linControlWriteMessage);

        if(!lProcAddr)  // at least one function was not found
        {
            hResult = GetLastError();
        }
        else
        {
            hResult = S_OK;
        }
    }

    return hResult;
}


/*****************************************************************************
 Function:
  UnmapLINControllerFunctions

 Description:
  Unloads the function pointer addresses of the "LIN controller specific
  functions"

 Arguments:
  none

  Results:
  S_OK on success
  otherwise an error value
*****************************************************************************/
HRESULT UnmapLINControllerFunctions(void)
{
    HRESULT hResult = ERROR_REDIRECTOR_HAS_OPEN_HANDLES;

    //*** Unload function pointer if referenced once only
    if(g_dwInstanceCounter == 1)
    {
        UNLOAD_FUNCTION_POINTER(linControlOpen);
        UNLOAD_FUNCTION_POINTER(linControlClose);
        UNLOAD_FUNCTION_POINTER(linControlGetCaps);
        UNLOAD_FUNCTION_POINTER(linControlGetStatus);
        UNLOAD_FUNCTION_POINTER(linControlInitialize);
        UNLOAD_FUNCTION_POINTER(linControlReset);
        UNLOAD_FUNCTION_POINTER(linControlStart);
        UNLOAD_FUNCTION_POINTER(linControlWriteMessage);

        hResult = S_OK;
    }

    return hResult;
}


/*****************************************************************************
 Function:
  MapLINMessageMonitorFunctions

 Description:
  Loads the function pointer addresses of the "LIN message monitor specific
  functions"

 Arguments:
  none

  Results:
  S_OK on success
  otherwise an error value
*****************************************************************************/
HRESULT MapLINMessageMonitorFunctions(void)
{
    HRESULT hResult   = CO_E_RELOAD_DLL;
    long    lProcAddr = 0x1;

    //*** Check if DLL is loaded
    if(g_hInstVciNplLib != nullptr)
    {
        LOAD_FUNCTION_POINTER(g_hInstVciNplLib, linMonitorOpen);
        LOAD_FUNCTION_POINTER(g_hInstVciNplLib, linMonitorClose);
        LOAD_FUNCTION_POINTER(g_hInstVciNplLib, linMonitorGetCaps);
        LOAD_FUNCTION_POINTER(g_hInstVciNplLib, linMonitorGetStatus);
        LOAD_FUNCTION_POINTER(g_hInstVciNplLib, linMonitorInitialize);
        LOAD_FUNCTION_POINTER(g_hInstVciNplLib, linMonitorActivate);
        LOAD_FUNCTION_POINTER(g_hInstVciNplLib, linMonitorPeekMessage);
        LOAD_FUNCTION_POINTER(g_hInstVciNplLib, linMonitorWaitRxEvent);
        LOAD_FUNCTION_POINTER(g_hInstVciNplLib, linMonitorReadMessage);

        if(!lProcAddr)  // at least one function was not found
        {
            hResult = GetLastError();
        }
        else
        {
            hResult = S_OK;
        }
    }

    return hResult;
}


/*****************************************************************************
 Function:
  UnmapLINMessageMonitorFunctions

 Description:
  Unloads the function pointer addresses of the "LIN message monitor specific
  functions"

 Arguments:
  none

  Results:
  S_OK on success
  otherwise an error value
*****************************************************************************/
HRESULT UnmapLINMessageMonitorFunctions(void)
{
    HRESULT hResult = ERROR_REDIRECTOR_HAS_OPEN_HANDLES;

    //*** Unload function pointer if referenced once only
    if(g_dwInstanceCounter == 1)
    {
        UNLOAD_FUNCTION_POINTER(linMonitorOpen);
        UNLOAD_FUNCTION_POINTER(linMonitorClose);
        UNLOAD_FUNCTION_POINTER(linMonitorGetCaps);
        UNLOAD_FUNCTION_POINTER(linMonitorGetStatus);
        UNLOAD_FUNCTION_POINTER(linMonitorInitialize);
        UNLOAD_FUNCTION_POINTER(linMonitorActivate);
        UNLOAD_FUNCTION_POINTER(linMonitorPeekMessage);
        UNLOAD_FUNCTION_POINTER(linMonitorWaitRxEvent);
        UNLOAD_FUNCTION_POINTER(linMonitorReadMessage);
    }

    return hResult;
}

