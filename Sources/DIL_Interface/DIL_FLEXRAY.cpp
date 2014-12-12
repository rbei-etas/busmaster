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
 * \file      DIL_Interface/DIL_FLEXRAY.cpp
 * \brief     Interface file for FLEXRAY BUS
 * \author    Arunkumar Karri
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Interface file for FLEXRAY BUS
 */
#include "DIL_Interface_stdafx.h"
#include "Dil_FLEXRAY.h"

typedef HRESULT (__stdcall* GETIDIL_FLEXRAY_CONTROLLER)(void** ppvInterface);
static GETIDIL_FLEXRAY_CONTROLLER pfGetIDILFLEXRAY_Controller;

#define VALIDATE_FLEX_POINTER(Ptr) if (Ptr == nullptr) {return S_FALSE;}

class ENTRY_DIL
{
public:
    DWORD           m_dwDIL;
    std::string          m_acDIL;
};

static ENTRY_DIL sg_ListDIL[] =
{
    /* simulation should be the first entry... */
    {FLEXRAY_DAL_NONE,       "&Deactivate"        },
    /* ...all other drivers should be in alphabetical order */
    {FLEXRAY_DRIVER_ETAS_BOA,   "ETAS &BOA"          },
    {FLEXRAY_DRIVER_GIGATRONIK_FLEXI,   "&GIGATRONIK flex-i"          },

    //{FLEXRAY_DRIVER_TZMCYCLONE, "&TZMCylone"         },
};

CDIL_FLEXRAY::CDIL_FLEXRAY()
{
    m_hDriverDll                    = nullptr;
    m_hOldDriverDll                 = nullptr;
    m_dwDriverID                    = FLEXRAY_DAL_NONE;
    pfGetIDILFLEXRAY_Controller     = nullptr;
    m_pBaseDILFLEXRAY_Controller    = nullptr;
    m_pOldBaseDILFLEXRAY_Controller = nullptr;
}

CDIL_FLEXRAY::~CDIL_FLEXRAY()
{
    //Free the currently selected DIL library
    if ( m_hDriverDll )
    {
        FreeLibrary(m_hDriverDll);
        m_hDriverDll = nullptr;
    }
    //Free the previosuly selected DIL library
    if ( m_hOldDriverDll )
    {
        FreeLibrary(m_hOldDriverDll);
        m_hOldDriverDll = nullptr;
    }
}

int CDIL_FLEXRAY::ExitInstance()
{
    return 0;
}
/* ROUTER CODE STARTS */

/**
 * The router is realised as a set of function pointers
 * Initialisation with dummy functions
 */
BOOL CDIL_FLEXRAY::InitInstance()
{
    return TRUE;
}

/**
 * Helper Function for Dummy Interface
 */
void CDIL_FLEXRAY::vSelectInterface_Dummy(void)
{

}

/* ROUTER CODE FINISHES */

DWORD CDIL_FLEXRAY::DILF_GetDILList(bool /* bAvailable */, FLEXRAY_DILLIST* List)
{
    DWORD Result = 0;

    if (List != nullptr)
    {
        for (int i = 0; i < sizeof(sg_ListDIL)/sizeof(ENTRY_DIL); i++)
        {
            (*List)[i].m_acName     = sg_ListDIL[i].m_acDIL;
            (*List)[i].m_dwDriverID = sg_ListDIL[i].m_dwDIL;
            Result++;
        }
    }

    return Result;
}

/**
 * Check and load support for latest available ETAS BOA version.
 *
 * \return Library handle
 */
HMODULE CDIL_FLEXRAY::vLoadEtasBoaLibrary(Base_WrapperErrorLogger* pILog)
{
    USES_CONVERSION;

    LONG lError = 0;
    HKEY sKey;

    // Check for BOA 2.0
    lError = RegOpenKeyEx(HKEY_LOCAL_MACHINE, "SOFTWARE\\ETAS\\BOA\\2", 0, KEY_READ, &sKey);
    if(lError==ERROR_SUCCESS)
    {
        RegCloseKey(sKey);
        pILog->vLogAMessage(A2T(__FILE__), __LINE__, _("Using ETAS BOA v2..."));
        return LoadLibrary("FlexRay_ETAS_BOA_2.dll");
    }

    // Check for BOA 1.5
    lError = RegOpenKeyEx(HKEY_LOCAL_MACHINE, "SOFTWARE\\ETAS\\BOA\\1.5", 0, KEY_READ, &sKey);
    if(lError==ERROR_SUCCESS)
    {
        RegCloseKey(sKey);
        pILog->vLogAMessage(A2T(__FILE__), __LINE__, _("Using ETAS BOA v1.5..."));
        return LoadLibrary("FlexRay_ETAS_BOA_1_5.dll");
    }

    // Check for BOA 1.4
    lError = RegOpenKeyEx(HKEY_LOCAL_MACHINE, "SOFTWARE\\ETAS\\BOA\\1.4", 0, KEY_READ, &sKey);
    if(lError==ERROR_SUCCESS)
    {
        RegCloseKey(sKey);
        pILog->vLogAMessage(A2T(__FILE__), __LINE__, _("Using ETAS BOA v1.4..."));
        return LoadLibrary("FlexRay_ETAS_BOA_1_4.dll");
    }

    pILog->vLogAMessage(A2T(__FILE__), __LINE__, _("ETAS BOA not found in registry."));
    return nullptr;
}

HRESULT CDIL_FLEXRAY::DILF_SelectDriver(DWORD dwDriverID, HWND hWndParent, Base_WrapperErrorLogger* pILog)
{
    USES_CONVERSION;

    HRESULT hResult = S_FALSE;

    if (DILF_GetSelectedDriver() == dwDriverID )
    {
        hResult = DAL_ALREADY_SELECTED;
    }
    else if (pILog == nullptr)
    {
        hResult = ERRLGR_INVALID;
    }
    else
    {
        if (hWndParent == nullptr)
        {
            /* Log a warning message informing about the invalidity of the
            owner window handle. */
        }

        /* Free the previosly selected DIL controller library */
        if ( m_hDriverDll )
        {
            ::FreeLibrary(m_hDriverDll);
            m_hDriverDll = nullptr;
            m_pBaseDILFLEXRAY_Controller = nullptr;
            //m_hOldDriverDll = m_hDriverDll;
            //m_pOldBaseDILFLEXRAY_Controller = m_pBaseDILFLEXRAY_Controller;
            //m_dwOldDriverID = m_dwDriverID;
            //m_hDriverDll = nullptr;
        }

        switch(dwDriverID)
        {
            case FLEXRAY_DRIVER_ETAS_BOA:
                m_hDriverDll = vLoadEtasBoaLibrary(pILog);
                break;
            case FLEXRAY_DRIVER_GIGATRONIK_FLEXI:
                m_hDriverDll = LoadLibrary("FlexRay_GIGATRONIK_flex-i.dll");
                break;
            case DRIVER_CAN_STUB:
                // m_hDriverDll = LoadLibrary("FLEXRAY_STUB.dll");
                hResult = DAL_INVALID;
                m_dwDriverID = DAL_NONE;
                return hResult;
            default:
                hResult = DAL_INVALID;
                m_dwDriverID = DAL_NONE;
                return hResult;
        }

        if (m_hDriverDll == nullptr)
        {
            hResult = ERR_LOAD_DRIVER;
            pILog->vLogAMessage(A2T(__FILE__), __LINE__, _("Load library failed..."));
        }
        else
        {
            pILog->vLogAMessage(A2T(__FILE__), __LINE__, _("Load library successful..."));
            pfGetIDILFLEXRAY_Controller = (GETIDIL_FLEXRAY_CONTROLLER)GetProcAddress(m_hDriverDll, "GetIDIL_FLEXRAY_Controller");
            pfGetIDILFLEXRAY_Controller((void**)&m_pBaseDILFLEXRAY_Controller);
            if ( m_pBaseDILFLEXRAY_Controller )
            {
                // set the application parameters.
                m_pBaseDILFLEXRAY_Controller->FLEXRAY_SetAppParams(hWndParent, pILog);
                hResult = m_pBaseDILFLEXRAY_Controller->FLEXRAY_LoadDriverLibrary();
                switch (hResult)
                {
                    case S_OK:
                    case DLL_ALREADY_LOADED:
                        m_pBaseDILFLEXRAY_Controller->FLEXRAY_PerformInitOperations();
                        m_dwDriverID = dwDriverID;
                        hResult = S_OK;
                        break;

                    default:
                        hResult = ERR_LOAD_DRIVER;
                        pILog->vLogAMessage(A2T(__FILE__), __LINE__, _("Load library failed..."));
                        /* Get rid of current DIL library */
                        if ( m_pBaseDILFLEXRAY_Controller )
                        {
                            m_pBaseDILFLEXRAY_Controller->FLEXRAY_PerformClosureOperations();
                            m_pBaseDILFLEXRAY_Controller->FLEXRAY_UnloadDriverLibrary();
                        }
                        FreeLibrary(m_hDriverDll);
                        m_pBaseDILFLEXRAY_Controller = nullptr;
                        if ( m_hOldDriverDll )
                        {
                            m_hDriverDll = m_hOldDriverDll;
                            m_pBaseDILFLEXRAY_Controller = m_pOldBaseDILFLEXRAY_Controller;
                            m_dwDriverID =  m_dwOldDriverID;
                        }
                }
            }
        }
    }

    return hResult;
}

/**
 * \brief     Manage target message buffer list
 *
 * This function manages the target message buffer list.
 */
HRESULT CDIL_FLEXRAY::DILF_ManageMsgBuf(BYTE bAction, DWORD ClientID, CBaseFLEXBufFSE* pBufObj)
{
    if ( m_pBaseDILFLEXRAY_Controller )
    {
        return m_pBaseDILFLEXRAY_Controller->FLEXRAY_ManageMsgBuf(bAction, ClientID, pBufObj);
    }
    else
    {
        return S_FALSE;
    }
}

/**
 * \brief     Register a client
 *
 * This function registers the client. Only registered client's buffer will be
 * updated on receive of a msg in the bus.
 */
HRESULT CDIL_FLEXRAY::DILF_RegisterClient(BOOL bRegister, DWORD& ClientID, char* pacClientName)
{
    VALIDATE_FLEX_POINTER(m_pBaseDILFLEXRAY_Controller);
    return m_pBaseDILFLEXRAY_Controller->FLEXRAY_RegisterClient(bRegister, ClientID, pacClientName);
}

DWORD CDIL_FLEXRAY::DILF_GetSelectedDriver(void)
{
    return m_dwDriverID;
}

HRESULT CDIL_FLEXRAY::DILF_PerformInitOperations(void)
{
    VALIDATE_FLEX_POINTER(m_pBaseDILFLEXRAY_Controller);
    return m_pBaseDILFLEXRAY_Controller->FLEXRAY_PerformInitOperations();
}

HRESULT CDIL_FLEXRAY::DILF_PerformClosureOperations(void)
{

    if ( m_pBaseDILFLEXRAY_Controller )
    {
        return m_pBaseDILFLEXRAY_Controller->FLEXRAY_PerformClosureOperations();
    }
    return S_FALSE;
}

HRESULT CDIL_FLEXRAY::DILF_GetTimeModeMapping(SYSTEMTIME& CurrSysTime, UINT64& TimeStamp)
{
    VALIDATE_FLEX_POINTER(m_pBaseDILFLEXRAY_Controller);
    return m_pBaseDILFLEXRAY_Controller->FLEXRAY_GetTimeModeMapping( CurrSysTime, TimeStamp );
}
HRESULT CDIL_FLEXRAY::DILF_ListHwInterfaces(FLEXRAY_INTERFACE_HW& sSelHwInterface, FLEXRAY_CONFIG_FILES& sAvailableConfigFiles)
{
    VALIDATE_FLEX_POINTER(m_pBaseDILFLEXRAY_Controller);
    return m_pBaseDILFLEXRAY_Controller->FLEXRAY_ListHwInterfaces( sSelHwInterface, sAvailableConfigFiles );
}
HRESULT CDIL_FLEXRAY::DILF_SelectHwInterfaces(const FLEXRAY_INTERFACE_HW_LIST& sSelHwInterface)
{
    VALIDATE_FLEX_POINTER(m_pBaseDILFLEXRAY_Controller);
    return m_pBaseDILFLEXRAY_Controller->FLEXRAY_SelectHwInterface( sSelHwInterface );
}

HRESULT CDIL_FLEXRAY::DILF_DeselectHwInterfaces(void)
{
    VALIDATE_FLEX_POINTER(m_pBaseDILFLEXRAY_Controller);
    return m_pBaseDILFLEXRAY_Controller->FLEXRAY_DeselectHwInterface();
}

HRESULT CDIL_FLEXRAY::DILF_DisplayConfigDlg(PCHAR& InitData, int& Length)
{
    VALIDATE_FLEX_POINTER(m_pBaseDILFLEXRAY_Controller);
    return m_pBaseDILFLEXRAY_Controller->FLEXRAY_DisplayConfigDlg( InitData, Length );
}

HRESULT CDIL_FLEXRAY::DILF_SetConfigData(CHANNEL_CONFIG& ouAbsSFibexContainer )
{
    VALIDATE_FLEX_POINTER(m_pBaseDILFLEXRAY_Controller);
    return m_pBaseDILFLEXRAY_Controller->FLEXRAY_SetConfigData( ouAbsSFibexContainer );
}
/*HRESULT CDIL_FLEXRAY::DILF_SetConfigData(PCHAR pInitData,  ABS_CLUSTER_INFO asClusterinfo, int Length, ABS_DEVICE_CONFIG  asDeviceConfig)
{

    return m_pBaseDILFLEXRAY_Controller->FLEXRAY_SetConfigData( pInitData, asClusterinfo, Length,asDeviceConfig );
}*/

HRESULT CDIL_FLEXRAY::DILF_StartHardware(void)
{
    if ( m_pBaseDILFLEXRAY_Controller )
    {
        return m_pBaseDILFLEXRAY_Controller->FLEXRAY_StartHardware();
    }
    else
    {
        return S_FALSE;
    }
}

HRESULT CDIL_FLEXRAY::DILF_StopHardware(void)
{
    VALIDATE_FLEX_POINTER(m_pBaseDILFLEXRAY_Controller);
    return m_pBaseDILFLEXRAY_Controller->FLEXRAY_StopHardware();
}


HRESULT CDIL_FLEXRAY::DILF_ResetHardware(void)
{
    VALIDATE_FLEX_POINTER(m_pBaseDILFLEXRAY_Controller);
    return m_pBaseDILFLEXRAY_Controller->FLEXRAY_ResetHardware();
}

HRESULT CDIL_FLEXRAY::DILF_GetCurrStatus(s_FLXSTATUSMSG& StatusData)
{
    VALIDATE_FLEX_POINTER(m_pBaseDILFLEXRAY_Controller);
    return m_pBaseDILFLEXRAY_Controller->FLEXRAY_GetCurrStatus( StatusData );
}

HRESULT CDIL_FLEXRAY::DILF_GetFlxTxMsgBuffer(CFlxTxMsgBuffer*& pouFlxTxMsgBuffer)
{

    if ( m_pBaseDILFLEXRAY_Controller )
    {
        return m_pBaseDILFLEXRAY_Controller->FLEXRAY_GetFlxTxMsgBuffer( pouFlxTxMsgBuffer );
    }
    else
    {
        return S_FALSE;
    }
}

HRESULT CDIL_FLEXRAY::DILF_SendFlxMsg(DWORD dwClientID, s_FLXTXMSG* pouFlxTxMsg)
{
    VALIDATE_FLEX_POINTER(m_pBaseDILFLEXRAY_Controller);
    return m_pBaseDILFLEXRAY_Controller->FLEXRAY_SendFlxMsg(dwClientID, pouFlxTxMsg );
}

HRESULT CDIL_FLEXRAY::DILF_DeleteFlxMsg(DWORD dwClientID, s_FLXTXMSG* pouFlxTxMsg)
{
    VALIDATE_FLEX_POINTER(m_pBaseDILFLEXRAY_Controller);
    return m_pBaseDILFLEXRAY_Controller->FLEXRAY_DeleteFlxMsg(dwClientID, pouFlxTxMsg );
}

HRESULT CDIL_FLEXRAY::DILF_GetBoardInfo(s_BOARDINFO& BoardInfo)
{
    VALIDATE_FLEX_POINTER(m_pBaseDILFLEXRAY_Controller);
    return m_pBaseDILFLEXRAY_Controller->FLEXRAY_GetBoardInfo( BoardInfo );
}

HRESULT CDIL_FLEXRAY::DILF_GetFlexRayInfo(s_FLXINFO& FlexRayInfo)
{
    VALIDATE_FLEX_POINTER(m_pBaseDILFLEXRAY_Controller);
    return m_pBaseDILFLEXRAY_Controller->FLEXRAY_GetFlexRayInfo( FlexRayInfo );
}

HRESULT CDIL_FLEXRAY::DILF_GetVersionInfo(VERSIONINFO& sVerInfo)
{
    VALIDATE_FLEX_POINTER(m_pBaseDILFLEXRAY_Controller);
    return m_pBaseDILFLEXRAY_Controller->FLEXRAY_GetVersionInfo( sVerInfo );
}

HRESULT CDIL_FLEXRAY::DILF_GetLastErrorString(std::string acErrorStr, HRESULT& nError)
{
    VALIDATE_FLEX_POINTER(m_pBaseDILFLEXRAY_Controller);
    return m_pBaseDILFLEXRAY_Controller->FLEXRAY_GetLastErrorString( acErrorStr, nError );
}

HRESULT CDIL_FLEXRAY::DILF_FilterFrames(FILTER_TYPE FilterType, FLEX_CHANNEL FlexChannel, UINT* punFrames, UINT nLength)
{
    VALIDATE_FLEX_POINTER(m_pBaseDILFLEXRAY_Controller);
    return m_pBaseDILFLEXRAY_Controller->FLEXRAY_FilterFrames( FilterType, FlexChannel, punFrames,  nLength );
}

HRESULT CDIL_FLEXRAY::DILF_ConfigMsgBuf(s_MSGBUF sMsgBuf, UINT* punAddress)
{
    VALIDATE_FLEX_POINTER(m_pBaseDILFLEXRAY_Controller);
    return m_pBaseDILFLEXRAY_Controller->FLEXRAY_ConfigMsgBuf( sMsgBuf, punAddress);
}

HRESULT CDIL_FLEXRAY::DILF_GetControllerCapabilities(s_FLXControllerCapabilities& /* ouFlexRayCapabilities */)
{
    VALIDATE_FLEX_POINTER(m_pBaseDILFLEXRAY_Controller);
    return S_OK;
}
