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
 * \file      DIL_CAN.cpp
 * \brief     Interface file for CAN BUS
 * \author    Pradeep Kadoor, Arunkumar Karri
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Interface file for CAN BUS
 */

/* Project includes */
#include "DIL_Interface_stdafx.h"
#include "include/Error.h"
#include "include/basedefs.h"
#include "DataTypes/Base_WrapperErrorLogger.h"
#include "DataTypes/MsgBufAll_DataTypes.h"
#include "DataTypes/DIL_Datatypes.h"
#include "DILC_Dummy.h"
#include "CAN_MHS/CAN_MHS_Extern.h"
#include "CAN_PEAK_USB/CAN_PEAK_USB_Extern.h"
#include "CAN_ICS_neoVI/CAN_ICS_neoVI_Extern.h"
#include "CAN_STUB/CAN_STUB_Extern.h"
#include "CAN_ETAS_BOA/CAN_ETAS_BOA_Extern.h"
#include "CAN_Vector_XL/CAN_Vector_XL_Extern.h"
#include "CAN_Kvaser_CAN/CAN_Kvaser_CAN_Extern.h"
#include "Dil_CAN.h"

typedef HRESULT (*GETIDIL_CAN_CONTROLLER)(void** ppvInterface);
static GETIDIL_CAN_CONTROLLER pfGetIDILCAN_Controller;
static CDIL_CAN_DUMMY* sg_pouDIL_CAN_DUMMY = new CDIL_CAN_DUMMY;

typedef struct
{
    DWORD           m_dwDIL;
    TCHAR           m_acDIL[MAX_DILNAME];
} ENTRY_DIL;

static ENTRY_DIL sg_ListDIL[] =
{
    /* simulation should be the first entry... */
    {DRIVER_CAN_STUB,       _T("Simulation")       },
    /* ...all other drivers should be in alphabetical order */
    {DRIVER_CAN_ETAS_BOA,   _T("ETAS BOA")         },
    {DRIVER_CAN_ETAS_ES581, _T("ETAS ES581")       },
    {DRIVER_CAN_ICS_NEOVI,  _T("IntrepidCS neoVI") },
    {DRIVER_CAN_KVASER_CAN, _T("Kvaser CAN")       },
    {DRIVER_CAN_MHS,        _T("MHS Tiny-CAN")     },
    {DRIVER_CAN_PEAK_USB,   _T("PEAK USB")         },
    {DRIVER_CAN_VECTOR_XL,  _T("Vector XL")        },
};

/**
 * \brief Constructor
 *
 * Constructor of CDIL_CAN
 */
CDIL_CAN::CDIL_CAN()
{
    m_hDll = NULL;
    m_dwDriverID = DAL_NONE;
    pfGetIDILCAN_Controller = NULL;
    m_pBaseDILCAN_Controller = NULL;
    vSelectInterface_Dummy();
}

/**
 * \brief Destructor
 *
 * Destructor of CDIL_CAN
 */
CDIL_CAN::~CDIL_CAN()
{
    //Free the currently selected DIL library
    if ( m_hDll )
    {
        FreeLibrary(m_hDll);
        m_hDll = NULL;
    }
}

/**
 * \brief Init instance
 *
 * The router is realised as a set of function pointers
 * Initialisation with dummy functions
 */
BOOL CDIL_CAN::InitInstance()
{
    vSelectInterface_Dummy();
    return TRUE;
}

/**
 * \brief Exit instance
 *
 * Deinitialization
 */
int CDIL_CAN::ExitInstance()
{
    return 0;
}

/**
 * \brief Helper Function for Dummy Interface
 *
 * Helper Function for Dummy Interface
 */
void CDIL_CAN::vSelectInterface_Dummy(void)
{
    m_pBaseDILCAN_Controller = (CBaseDIL_CAN_Controller*) sg_pouDIL_CAN_DUMMY;
}

/**
 * \brief      Get DIL list
 * \req        RSI_14_001 - DILC_GetDILList
 * \req        RS_23_01 - Getter for the DIL List
 * \param[out] Returns the DIL list
 * \return     Status code
 *
 * Based on the parameter this function renders number of the driver interface
 * layers supported or available. If 'bAvailable' is true, this returns number of
 * the DILs implemented; else the list of the DILs supported by the existing
 * license will be returned. If List is NULL, only number is returned.
 */
DWORD CDIL_CAN::DILC_GetDILList(bool /*bAvailable*/, DILLIST* List)
{
    DWORD Result = 0;

    if (List != NULL)
    {
        for (int i = 0; i < sizeof(sg_ListDIL)/sizeof(ENTRY_DIL); i++)
        {
            strcpy_s((*List)[i].m_acName, sg_ListDIL[i].m_acDIL);
            (*List)[i].m_dwDriverID = sg_ListDIL[i].m_dwDIL;
            Result++;
        }
    }

    return Result;
}

/**
 * \brief     Select driver from DIL list
 * \req       RSI_14_002 - DILC_SelectDriver
 * \req       RS_23_02 - Selecting a driver from the DIL list
 *
 * This function selects a driver abstraction layer (DAL). If support for the
 * intended one isn't allowed with the present license, this returns NO_LICENSE.
 */
HRESULT CDIL_CAN::DILC_SelectDriver(DWORD dwDriverID, HWND hWndOwner,
                                    Base_WrapperErrorLogger* pILog)
{
    USES_CONVERSION;
    HRESULT hResult = S_FALSE;

    if (DILC_GetSelectedDriver() == dwDriverID)
    {
        hResult = DAL_ALREADY_SELECTED;
    }
    else if (pILog == NULL)
    {
        hResult = ERRLGR_INVALID;
    }
    else
    {
        if (hWndOwner == NULL)
        {
            /* Log a warning message informing about the invalidity of the
            owner window handle. */
        }

        //Free the currently selected DIL library
        if ( m_hDll )
        {
            FreeLibrary(m_hDll);
            m_hDll = NULL;
        }

        switch(dwDriverID)
        {
            case DRIVER_CAN_PEAK_USB:
                m_hDll = LoadLibrary(_T("CAN_PEAK_USB.dll"));
                break;

            case DRIVER_CAN_ICS_NEOVI:
            case DRIVER_CAN_ETAS_ES581:
                m_hDll = LoadLibrary(_T("CAN_ICS_neoVI.dll"));
                break;

            case DRIVER_CAN_ETAS_BOA:
                m_hDll = LoadLibrary(_T("CAN_ETAS_BOA.dll"));
                break;

            case DRIVER_CAN_VECTOR_XL:
                m_hDll = LoadLibrary(_T("CAN_Vector_XL.dll"));
                break;

            case DRIVER_CAN_KVASER_CAN:
                m_hDll = LoadLibrary(_T("CAN_Kvaser_CAN.dll"));
                break;

            case DRIVER_CAN_STUB:
                m_hDll = LoadLibrary(_T("CAN_STUB.dll"));
                break;

            case DRIVER_CAN_MHS:
                m_hDll = LoadLibrary(_T("CAN_MHS.dll"));
                break;

            case DAL_NONE:
                DILC_PerformClosureOperations();
                vSelectInterface_Dummy();
                m_dwDriverID = DAL_NONE;
                return hResult;

            default:
                hResult = DAL_INVALID;
                m_dwDriverID = DAL_NONE;
                return hResult;
        }

        if (m_hDll == NULL)
        {
            hResult = ERR_LOAD_DRIVER;
            pILog->vLogAMessage(A2T(__FILE__), __LINE__, _T("Load library failed..."));
        }
        else
        {
            // First select the dummy interface
            DILC_SelectDriver((DWORD)DAL_NONE, hWndOwner, pILog);
            pILog->vLogAMessage(A2T(__FILE__), __LINE__, _T("Load library successful..."));
            pfGetIDILCAN_Controller = (GETIDIL_CAN_CONTROLLER)GetProcAddress(m_hDll, "GetIDIL_CAN_Controller");
            pfGetIDILCAN_Controller((void**)&m_pBaseDILCAN_Controller);

            if ( m_pBaseDILCAN_Controller )
            {
                // set the application parameters.
                m_pBaseDILCAN_Controller->CAN_SetAppParams(hWndOwner, pILog);
                hResult = m_pBaseDILCAN_Controller->CAN_LoadDriverLibrary();

                switch (hResult)
                {
                    case S_OK:
                    case DLL_ALREADY_LOADED:
                        m_pBaseDILCAN_Controller->CAN_PerformInitOperations();
                        m_dwDriverID = dwDriverID;
                        hResult = S_OK;
                        break;

                    default:
                        hResult = ERR_LOAD_DRIVER;
                        pILog->vLogAMessage(A2T(__FILE__), __LINE__, _T("Load library failed..."));
                        break;
                }
            }
        }
    }

    return hResult;
}

/**
 * \brief     Manage target message buffer list
 * \req       RSI_14_004 - DILC_ManageMsgBuf
 *
 * This function manages the target message buffer list.
 */
HRESULT CDIL_CAN::DILC_ManageMsgBuf(BYTE bAction, DWORD ClientID, CBaseCANBufFSE* pBufObj)
{
    return m_pBaseDILCAN_Controller->CAN_ManageMsgBuf(bAction, ClientID, pBufObj);
}

/**
 * \brief     Register a client
 * \req       RSI_14_003 - DILC_RegisterClient
 * \req       RS_23_04 - Registration of a client to simulate a node
 *
 * This function registers the client. Only registered client buffers will be
 * updated on receive of a msg in the bus.
 */
HRESULT CDIL_CAN::DILC_RegisterClient(BOOL bRegister, DWORD& ClientID, TCHAR* pacClientName)
{
    return m_pBaseDILCAN_Controller->CAN_RegisterClient(bRegister, ClientID, pacClientName);
}

/**
 * \brief     Get selected driver
 * \req       RSI_14_005 - DILC_GetSelectedDriver
 * \req       RS_23_03 - Getter for the presently selected driver
 *
 * This returns ID of the driver interface layer presently selected.
 */
DWORD CDIL_CAN::DILC_GetSelectedDriver(void)
{
    return m_dwDriverID;
}

/**
 * \brief     Initialize operation
 * \req       RSI_14_006 - DILC_PerformInitOperations
 * \req       RS_23_08 - Carry out initialization operations
 *
 * Call for all initialisation operations
 */
HRESULT CDIL_CAN::DILC_PerformInitOperations(void)
{
    return m_pBaseDILCAN_Controller->CAN_PerformInitOperations();
}

/**
 * \brief     Closure operation
 * \req       RSI_14_007 - DILC_PerformClosureOperations
 * \req       RS_23_09 - Carry out closure operations
 *
 * Call for all uninitialisation operations
 */
HRESULT CDIL_CAN::DILC_PerformClosureOperations(void)
{
    HRESULT hResult = S_FALSE;

    if ( m_pBaseDILCAN_Controller )
    {
        m_pBaseDILCAN_Controller->CAN_PerformClosureOperations();
        m_pBaseDILCAN_Controller->CAN_UnloadDriverLibrary();
    }

    vSelectInterface_Dummy();
    m_dwDriverID = DAL_NONE;
    return hResult;
}

/**
 * \brief     Get time mode mapping
 * \req       RSI_14_008 - DILC_GetTimeModeMapping
 * \req       RS_23_10 - Getter for the time mode mapping (usually the 64-bit time stamp by the driver)
 *
 * Call this function to get a system time and the time stamp associated with it
 */
HRESULT CDIL_CAN::DILC_GetTimeModeMapping(SYSTEMTIME& CurrSysTime, UINT64& TimeStamp, LARGE_INTEGER* QueryTickCount)
{
    return m_pBaseDILCAN_Controller->CAN_GetTimeModeMapping(CurrSysTime, TimeStamp, QueryTickCount);
}

/**
 * \brief     List hardware interfaces
 * \req       RSI_14_009 - DILC_ListHwInterfaces
 * \req       RS_23_11 - Listing of the controllers for the current driver
 *
 * Call this function to list the hardware interfaces available and receive user's choice.
 */
HRESULT CDIL_CAN::DILC_ListHwInterfaces(INTERFACE_HW_LIST& sSelHwInterface, INT& nCount)
{
    return m_pBaseDILCAN_Controller->CAN_ListHwInterfaces(sSelHwInterface, nCount);
}

/**
 * \brief     Select hardware interfaces
 * \req       RSI_14_010 - DILC_SelectHwInterfaces
 * \req       RS_23_12 - Selection of a controller from the hardware interface list
 *
 * Call this function to select hardware interfaces.
 */
HRESULT CDIL_CAN::DILC_SelectHwInterfaces(const INTERFACE_HW_LIST& sSelHwInterface, INT nCount)
{
    return m_pBaseDILCAN_Controller->CAN_SelectHwInterface(sSelHwInterface, nCount);
}

/**
 * \brief     Deselect hardware interfaces
 * \req       RSI_14_011 - DILC_DeselectHwInterfaces
 * \req       RS_23_13 - Deselection of the presently selected controller
 *
 * Call this function to deselect the selected hardware interface
 */
HRESULT CDIL_CAN::DILC_DeselectHwInterfaces(void)
{
    return m_pBaseDILCAN_Controller->CAN_DeselectHwInterface();
}

/**
 * \brief     Display configuration dialog box
 * \req       RSI_14_012 - DILC_DisplayConfigDlg
 * \req       RS_23_14 - Display the configuration dialog box of the present controller
 *
 * Function to display the configuration dialog box for the selected DIL. If
 * the dialog box needs to be displayed been initialised, pass the relevant data
 * as InitData. If it is null, the dialog box is uninitialised. This also contains
 * the user's choice as OUT parameter
 */
HRESULT CDIL_CAN::DILC_DisplayConfigDlg(PCHAR& InitData, int& Length)
{
    return m_pBaseDILCAN_Controller->CAN_DisplayConfigDlg(InitData, Length);
}

/**
 * \brief     Set configuration data
 * \req       RSI_14_013 - DILC_SetConfigData
 * \req       RS_23_15 - Setting of the configuration data for the present controller
 *
 * To set the configuration data for the currently selected DIL. Caller must
 * release the memory.
 */
HRESULT CDIL_CAN::DILC_SetConfigData(PCHAR pInitData, int Length)
{
    return m_pBaseDILCAN_Controller->CAN_SetConfigData(pInitData, Length);
}

/**
 * \brief     Start the controller
 * \req       RSI_14_014 - DILC_StartHardware
 * \req       RS_23_16 - Start the presently selected controller (or connect)
 *
 * Start the controller
 */
HRESULT CDIL_CAN::DILC_StartHardware(void)
{
    return m_pBaseDILCAN_Controller->CAN_StartHardware();
}

/**
 * \brief     Stop the controller
 * \req       RSI_14_015 - DILC_StopHardware
 * \req       RS_23_17 - Stop the presently selected controller (or disconnect)
 *
 * Stop the controller
 */
HRESULT CDIL_CAN::DILC_StopHardware(void)
{
    return m_pBaseDILCAN_Controller->CAN_StopHardware();
}

/**
 * \brief     Reset hardware
 * \req       RSI_14_016 - DILC_ResetHardware
 * \req       RS_23_18 - Reset the presently selected controller
 *
 * Reset Hardware
 */
HRESULT CDIL_CAN::DILC_ResetHardware(void)
{
    return m_pBaseDILCAN_Controller->CAN_ResetHardware();
}

/**
 * \brief     Send messages
 * \req       RSI_14_017 - DILC_SendMsg
 * \req       RS_23_19 - Transmit a frame
 *
 * Send messages
 */
HRESULT CDIL_CAN::DILC_SendMsg(DWORD dwClientID, const STCAN_MSG& sCanTxMsg)
{
    return m_pBaseDILCAN_Controller->CAN_SendMsg(dwClientID, sCanTxMsg);
}

/**
 * \brief     Get last error as string
 * \req       RSI_14_019 - DILC_GetLastErrorString
 * \req       RS_23_21 - In case of any error, a function returns the associated string of the last error
 *
 * Call to get descriptive string of the last error occurred
 */
HRESULT CDIL_CAN::DILC_GetLastErrorString(string& acErrorStr)
{
    return m_pBaseDILCAN_Controller->CAN_GetLastErrorString(acErrorStr);
}

/**
 * \brief     Get controller status
 * \req       RSI_14_021 - DILC_GetControllreStatus
 * \req       RS_23_24 - Getter for controller status by callback mechanism
 *
 * Call to get controller status. Caller has to give the handle of a
 * event which will set whenever the controller changes the state.
 */
HRESULT CDIL_CAN::DILC_GetCntrlStatus(const HANDLE& hEvent, UINT& unCntrlStatus)
{
    return m_pBaseDILCAN_Controller->CAN_GetCntrlStatus(hEvent, unCntrlStatus);
}

/**
 * \brief     Get controller parameters
 * \req       RSI_14_022 - DILC_GetControllerParams
 *
 * Call to get Controller parameters. Value will be returned stored in lParam.
 */
HRESULT CDIL_CAN::DILC_GetControllerParams(LONG& lParam, UINT nChannel, ECONTR_PARAM eContrParam)
{
    if ( m_pBaseDILCAN_Controller )
    {
        return m_pBaseDILCAN_Controller->CAN_GetControllerParams(lParam, nChannel, eContrParam);
    }
    else
    {
        return S_FALSE;
    }
}

/**
 * \brief     Get error counts
 * \req       RSI_14_023 - DILC_GetErrorCount
 *
 * Call to get error counts
 */
HRESULT  CDIL_CAN::DILC_GetErrorCount(SERROR_CNT& sErrorCnt, UINT nChannel, ECONTR_PARAM eContrParam)
{
    return m_pBaseDILCAN_Controller->CAN_GetErrorCount(sErrorCnt, nChannel, eContrParam);
}
