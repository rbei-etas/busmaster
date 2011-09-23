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
 * \author    Pradeep Kadoor
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Interface file for CAN BUS
 */
#include "DIL_Interface_stdafx.h"
#include "include/Error.h"
#include "include/basedefs.h"
#include "DataTypes/Base_WrapperErrorLogger.h"
#include "DataTypes/MsgBufAll_DataTypes.h"
#include "DataTypes/DIL_Datatypes.h"
#include "DILC_Dummy.h"
#include "CAN_PEAK_USB_Extern.h"
#include "CAN_ETAS_ES581/CAN_ETAS_ES581_Extern.h"
#include "CAN_STUB/CAN_STUB_Extern.h"
#include "CAN_BOA/CAN_BOA_Extern.h"
#include "Dil_CAN.h"


typedef struct
{
    DWORD           m_dwDIL;
    TCHAR           m_acDIL[MAX_DILNAME];
} ENTRY_DIL;

static ENTRY_DIL sg_ListDIL[DIL_TOTAL] =
{
    {DRIVER_CAN_STUB,       _T("CAN_Simulation")},
    {DRIVER_CAN_ETAS_ES581, _T("CAN_ES581")     },
    {DRIVER_CAN_BOA,        _T("CAN_BOA")       },
    {DRIVER_CAN_PEAK_USB,   _T("CAN_USB_PEAK")  },
    {DRIVER_CAN_PEAK_PP,    _T("CAN_PP_PEAK")   }
};


CDIL_CAN::CDIL_CAN()
{
    vSelectInterface_Dummy();
}

CDIL_CAN::~CDIL_CAN()
{
}

int CDIL_CAN::ExitInstance()
{
    return 0;
}
/* ROUTER CODE STARTS */


/* The router is realised as a set of function pointers */
/* Start of router implementation along with initialisation with dummy functions */
BOOL CDIL_CAN::InitInstance()
{
    vSelectInterface_Dummy();
    return TRUE;
}

/* End of router implementation along with initialisation with dummy functions */

/**
 * Helper Function for Dummy Interface
 */
void CDIL_CAN::vSelectInterface_Dummy(void)
{
    m_pfPerformInitOperations = DILC_Dummy_PerformInitOperations;
    m_pfPerformClosureOperations = DILC_Dummy_PerformClosureOperations;
    m_pfGetTimeModeMapping = DILC_Dummy_GetTimeModeMapping;
    m_pfListHwInterfaces = DILC_Dummy_ListHwInterfaces;
    m_pfSelectHwInterface = DILC_Dummy_SelectHwInterface;
    m_pfDeselectHwInterfaces = DILC_Dummy_DeselectHwInterface;
    m_pfDisplayConfigDlg = DILC_Dummy_DisplayConfigDlg;
    m_pfSetConfigData = DILC_Dummy_SetConfigData;
    m_pfStartHardware = DILC_Dummy_StartHardware;
    m_pfStopHardware = DILC_Dummy_StopHardware;
    m_pfResetHardware = DILC_Dummy_ResetHardware;
    m_pfGetTxMsgBuffer = DILC_Dummy_GetTxMsgBuffer;
    m_pfSendMsg = DILC_Dummy_SendMsg;
    m_pfGetBoardInfo = DILC_Dummy_GetBoardInfo;
    m_pfGetBusConfigInfo = DILC_Dummy_GetBusConfigInfo;
    m_pfGetVersionInfo = DILC_Dummy_GetVersionInfo;
    m_pfGetLastErrorString = DILC_Dummy_GetLastErrorString;
    m_pfFilterFrames = DILC_Dummy_FilterFrames;
    m_pfManageMsgBuf = DILC_Dummy_ManageMsgBuf;
    m_pfRegisterClient = DILC_Dummy_RegisterClient;
    m_pfGetCntrlStatus = DILC_Dummy_GetCntrlStatus;
    m_pfGetControllerParams = DILC_Dummy_GetControllerParams;
    m_pfGetErrorCount = DILC_Dummy_GetErrorCount;
}

/**
 * Helper Function for CAN_Usb Interface
 */
void CDIL_CAN::vSelectInterface_CAN_Usb(void)
{
    m_pfPerformInitOperations = CAN_Usb_PerformInitOperations;
    m_pfPerformClosureOperations = CAN_Usb_PerformClosureOperations;
    m_pfGetTimeModeMapping = CAN_Usb_GetTimeModeMapping;
    m_pfListHwInterfaces = CAN_Usb_ListHwInterfaces;
    m_pfSelectHwInterface = CAN_Usb_SelectHwInterface;
    m_pfDeselectHwInterfaces = CAN_Usb_DeselectHwInterface;
    m_pfDisplayConfigDlg = CAN_Usb_DisplayConfigDlg;
    m_pfSetConfigData = CAN_Usb_SetConfigData;
    m_pfStartHardware = CAN_Usb_StartHardware;
    m_pfStopHardware = CAN_Usb_StopHardware;
    m_pfResetHardware = CAN_Usb_ResetHardware;
    m_pfGetTxMsgBuffer = CAN_Usb_GetTxMsgBuffer;
    m_pfSendMsg = CAN_Usb_SendMsg;
    m_pfGetBoardInfo = CAN_Usb_GetBoardInfo;
    m_pfGetBusConfigInfo = CAN_Usb_GetBusConfigInfo;
    m_pfGetVersionInfo = CAN_Usb_GetVersionInfo;
    m_pfGetLastErrorString = CAN_Usb_GetLastErrorString;
    m_pfFilterFrames = CAN_Usb_FilterFrames;
    m_pfManageMsgBuf = CAN_Usb_ManageMsgBuf;
    m_pfRegisterClient = CAN_Usb_RegisterClient;
    m_pfGetCntrlStatus = CAN_Usb_GetCntrlStatus;
    m_pfGetControllerParams = CAN_Usb_GetControllerParams;
    m_pfGetErrorCount = CAN_Usb_GetErrorCount;
}

/**
 * Helper Function for CAN_STUB Interface
 */
void CDIL_CAN::vSelectInterface_CAN_STUB(void)
{
    m_pfPerformInitOperations = CAN_STUB_PerformInitOperations;
    m_pfPerformClosureOperations = CAN_STUB_PerformClosureOperations;
    m_pfGetTimeModeMapping = CAN_STUB_GetTimeModeMapping;
    m_pfListHwInterfaces = CAN_STUB_ListHwInterfaces;
    m_pfSelectHwInterface = CAN_STUB_SelectHwInterface;
    m_pfDeselectHwInterfaces = CAN_STUB_DeselectHwInterface;
    m_pfDisplayConfigDlg = CAN_STUB_DisplayConfigDlg;
    m_pfSetConfigData = CAN_STUB_SetConfigData;
    m_pfStartHardware = CAN_STUB_StartHardware;
    m_pfStopHardware = CAN_STUB_StopHardware;
    m_pfResetHardware = CAN_STUB_ResetHardware;
    m_pfGetTxMsgBuffer = CAN_STUB_GetTxMsgBuffer;
    m_pfSendMsg = CAN_STUB_SendMsg;
    m_pfGetBoardInfo = CAN_STUB_GetBoardInfo;
    m_pfGetBusConfigInfo = CAN_STUB_GetBusConfigInfo;
    m_pfGetVersionInfo = CAN_STUB_GetVersionInfo;
    m_pfGetLastErrorString = CAN_STUB_GetLastErrorString;
    m_pfFilterFrames = CAN_STUB_FilterFrames;
    m_pfManageMsgBuf = CAN_STUB_ManageMsgBuf;
    m_pfRegisterClient = CAN_STUB_RegisterClient;
    m_pfGetCntrlStatus = CAN_STUB_GetCntrlStatus;
    m_pfGetControllerParams = CAN_STUB_GetControllerParams;
    m_pfGetErrorCount = CAN_STUB_GetErrorCount;
}

/**
 * Helper Function for CAN_ETAS_ES581 Interface
 */
void CDIL_CAN::vSelectInterface_CAN_ETAS_ES581(void)
{
    m_pfPerformInitOperations = CAN_ES581_Usb_PerformInitOperations;
    m_pfPerformClosureOperations = CAN_ES581_Usb_PerformClosureOperations;
    m_pfGetTimeModeMapping = CAN_ES581_Usb_GetTimeModeMapping;
    m_pfListHwInterfaces = CAN_ES581_Usb_ListHwInterfaces;
    m_pfSelectHwInterface = CAN_ES581_Usb_SelectHwInterface;
    m_pfDeselectHwInterfaces = CAN_ES581_Usb_DeselectHwInterface;
    m_pfDisplayConfigDlg = CAN_ES581_Usb_DisplayConfigDlg;
    m_pfSetConfigData = CAN_ES581_Usb_SetConfigData;
    m_pfStartHardware = CAN_ES581_Usb_StartHardware;
    m_pfStopHardware = CAN_ES581_Usb_StopHardware;
    m_pfResetHardware = CAN_ES581_Usb_ResetHardware;
    m_pfGetTxMsgBuffer = CAN_ES581_Usb_GetTxMsgBuffer;
    m_pfSendMsg = CAN_ES581_Usb_SendMsg;
    m_pfGetBoardInfo = CAN_ES581_Usb_GetBoardInfo;
    m_pfGetBusConfigInfo = CAN_ES581_Usb_GetBusConfigInfo;
    m_pfGetVersionInfo = CAN_ES581_Usb_GetVersionInfo;
    m_pfGetLastErrorString = CAN_ES581_Usb_GetLastErrorString;
    m_pfFilterFrames = CAN_ES581_Usb_FilterFrames;
    m_pfManageMsgBuf = CAN_ES581_Usb_ManageMsgBuf;
    m_pfRegisterClient = CAN_ES581_Usb_RegisterClient;
    m_pfGetCntrlStatus = CAN_ES581_Usb_GetCntrlStatus;
    m_pfGetControllerParams = CAN_ES581_Usb_GetControllerParams;
    m_pfGetErrorCount = CAN_ES581_Usb_GetErrorCount;
}

/**
 * Helper Function for CAN_Parallel_Port Interface
 */
void CDIL_CAN::vSelectInterface_CAN_Parallel_Port(void)
{

}

/**
 * Helper Function for CAN_BOA Interface
 */
void CDIL_CAN::vSelectInterface_CAN_BOA(void)
{
    m_pfPerformInitOperations = CAN_BOA_PerformInitOperations;
    m_pfPerformClosureOperations = CAN_BOA_PerformClosureOperations;
    m_pfGetTimeModeMapping = CAN_BOA_GetTimeModeMapping;
    m_pfListHwInterfaces = CAN_BOA_ListHwInterfaces;
    m_pfSelectHwInterface = CAN_BOA_SelectHwInterface;
    m_pfDeselectHwInterfaces = CAN_BOA_DeselectHwInterface;
    m_pfDisplayConfigDlg = CAN_BOA_DisplayConfigDlg;
    m_pfSetConfigData = CAN_BOA_SetConfigData;
    m_pfStartHardware = CAN_BOA_StartHardware;
    m_pfStopHardware = CAN_BOA_StopHardware;
    m_pfResetHardware = CAN_BOA_ResetHardware;
    m_pfGetTxMsgBuffer = CAN_BOA_GetTxMsgBuffer;
    m_pfSendMsg = CAN_BOA_SendMsg;
    m_pfGetBoardInfo = CAN_BOA_GetBoardInfo;
    m_pfGetBusConfigInfo = CAN_BOA_GetBusConfigInfo;
    m_pfGetVersionInfo = CAN_BOA_GetVersionInfo;
    m_pfGetLastErrorString = CAN_BOA_GetLastErrorString;
    m_pfFilterFrames = CAN_BOA_FilterFrames;
    m_pfManageMsgBuf = CAN_BOA_ManageMsgBuf;
    m_pfRegisterClient = CAN_BOA_RegisterClient;
    m_pfGetCntrlStatus = CAN_BOA_GetCntrlStatus;
    m_pfGetControllerParams = CAN_BOA_GetControllerParams;
    m_pfGetErrorCount = CAN_BOA_GetErrorCount;
}
/* ROUTER CODE FINISHES */

/**
 * \brief     Get DIL list
 * \req       RS_23_01 - Getter for the DIL List
 *
 * Based on the parameter this function renders number of the driver interface
 * layers supported or available. If 'bAvailable' is true, this returns number of
 * the DILs implemented; else the list of the DILs supported by the existing
 * license will be returned. If List is NULL, only number is returned.
 */
DWORD CDIL_CAN::DILC_GetDILList(bool /*bAvailable*/, DILLIST* List)
{
    DWORD Result = DIL_TOTAL;

    if (List != NULL)
    {
        for (int i = 0; i < DIL_TOTAL; i++)
        {
            _tcscpy((*List)[i].m_acName, sg_ListDIL[i].m_acDIL);
            (*List)[i].m_dwDriverID = sg_ListDIL[i].m_dwDIL;
        }
    }

    return Result;
}

/**
 * \brief     Select driver from DIL list
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

        if (dwDriverID == DRIVER_CAN_PEAK_USB)
        {
            pILog->vLogAMessage(A2T(__FILE__), __LINE__, _T("Selecting DAL_NONE..."));
            // First select the dummy interface
            DILC_SelectDriver((DWORD)DAL_NONE, hWndOwner, pILog);
            // First set the application parameters.
            CAN_Usb_SetAppParams(hWndOwner, pILog);
            // Next load the driver library
            hResult = CAN_Usb_LoadDriverLibrary();
            switch (hResult)
            {
                case S_OK:
                case DLL_ALREADY_LOADED:
                {
                    pILog->vLogAMessage(A2T(__FILE__), __LINE__, _T("Load library successful..."));
                    hResult = S_OK;
                    CAN_Usb_PerformInitOperations();
                    vSelectInterface_CAN_Usb();
                }
                break;
                default:
                {
                    hResult = ERR_LOAD_DRIVER;
                    pILog->vLogAMessage(A2T(__FILE__), __LINE__, _T("Load library failed..."));
                }
                break;
            }

        }
        else if (dwDriverID == DRIVER_CAN_ETAS_ES581)
        {
            pILog->vLogAMessage(A2T(__FILE__), __LINE__, _T("Selecting DAL_NONE..."));
            // First select the dummy interface
            DILC_SelectDriver((DWORD)DAL_NONE, hWndOwner, pILog);
            // First set the application parameters.
            hResult = CAN_ES581_Usb_SetAppParams(hWndOwner, pILog);
            // Next load the driver library
            hResult = CAN_ES581_Usb_LoadDriverLibrary();
            switch (hResult)
            {
                case S_OK:
                case DLL_ALREADY_LOADED:
                {
                    pILog->vLogAMessage(A2T(__FILE__), __LINE__, _T("Load library successful..."));
                    hResult = S_OK;
                    CAN_ES581_Usb_PerformInitOperations();
                    vSelectInterface_CAN_ETAS_ES581();
                }
                break;
                default:
                {
                    hResult = ERR_LOAD_DRIVER;
                    pILog->vLogAMessage(A2T(__FILE__), __LINE__, _T("Load library failed..."));
                }
                break;
            }

        }
        else if (dwDriverID == DRIVER_CAN_BOA)
        {
            pILog->vLogAMessage(A2T(__FILE__), __LINE__, _T("Selecting DAL_NONE..."));
            // First select the dummy interface
            DILC_SelectDriver((DWORD)DAL_NONE, hWndOwner, pILog);

            // First set the application parameters.
            hResult = CAN_BOA_SetAppParams(hWndOwner, pILog);
            // Next load the driver library
            hResult = CAN_BOA_LoadDriverLibrary();
            switch (hResult)
            {
                case S_OK:
                case DLL_ALREADY_LOADED:
                {
                    pILog->vLogAMessage(A2T(__FILE__), __LINE__, _T("Load library successful..."));
                    hResult = S_OK;
                    CAN_BOA_PerformInitOperations();
                    vSelectInterface_CAN_BOA();
                }
                break;
                default:
                {
                    hResult = ERR_LOAD_DRIVER;
                    pILog->vLogAMessage(A2T(__FILE__), __LINE__, _T("Load library failed..."));
                }
                break;
            }
        }
        else if (dwDriverID == DRIVER_CAN_PEAK_PP)
        {
            // First select the dummy interface
            DILC_SelectDriver((DWORD)DAL_NONE, hWndOwner, pILog);

            // First set the application parameters.
            //CAN_Parallel_Port_SetAppParams(hWndOwner, pILog);
            // Next load the driver library
            //hResult = CAN_Parallel_Port_LoadDriverLibrary();
            switch (hResult)
            {
                case S_OK:
                case DLL_ALREADY_LOADED:
                {
                    hResult = S_OK;
                    //CAN_Parallel_Port_PerformInitOperations();
                    vSelectInterface_CAN_Parallel_Port();
                }
                break;
            }

        }
        else if (dwDriverID == DRIVER_CAN_STUB)
        {
            pILog->vLogAMessage(A2T(__FILE__), __LINE__, _T("Selecting DRIVER_CAN_STUB..."));
            // First select the dummy interface
            DILC_SelectDriver((DWORD)DAL_NONE, hWndOwner, pILog);

            // First set the application parameters.
            hResult = CAN_STUB_SetAppParams(hWndOwner, pILog);
            // Next load the driver library
            hResult = CAN_STUB_LoadDriverLibrary();
            switch (hResult)
            {
                case S_OK:
                case DLL_ALREADY_LOADED:
                {
                    pILog->vLogAMessage(A2T(__FILE__), __LINE__, _T("Load library successful..."));
                    hResult = S_OK;
                    CAN_STUB_PerformInitOperations();
                    vSelectInterface_CAN_STUB();
                }
                break;
                default:
                {
                    hResult = ERR_LOAD_DRIVER;
                    pILog->vLogAMessage(A2T(__FILE__), __LINE__, _T("Load library failed..."));
                }
                break;
            }

        }
        else if (dwDriverID == DAL_NONE)
        {
            DILC_PerformClosureOperations();
            vSelectInterface_Dummy();
        }
        else // invalid driver / DAL id
        {
            hResult = DAL_INVALID;
        }
    }

    return hResult;
}

/**
 * \brief     Manage target message buffer list
 *
 * This function manages the target message buffer list.
 */
HRESULT CDIL_CAN::DILC_ManageMsgBuf(BYTE bAction, DWORD ClientID, CBaseCANBufFSE* pBufObj)
{
    return (*m_pfManageMsgBuf)(bAction, ClientID, pBufObj);
}

/**
 * \brief     Register a client
 * \req       RS_23_04 - Registration of a client to simulate a node
 *
 * This function registers the client. Only registered client's buffer will be
 * updated on receive of a msg in the bus.
 */
HRESULT CDIL_CAN::DILC_RegisterClient(BOOL bRegister, DWORD& ClientID, TCHAR* pacClientName)
{
    return (*m_pfRegisterClient)(bRegister, ClientID, pacClientName);
}

/**
 * \brief     Get selected driver
 * \req       RS_23_03 - Getter for the presently selected driver
 *
 * This returns ID of the driver interface layer presently selected.
 */
DWORD CDIL_CAN::DILC_GetSelectedDriver(void)
{
    DWORD Result = (DWORD)DAL_NONE;

    if (m_pfDisplayConfigDlg == CAN_Usb_DisplayConfigDlg)
    {
        Result = DRIVER_CAN_PEAK_USB;
    }
    else if (m_pfDisplayConfigDlg == CAN_BOA_DisplayConfigDlg)
    {
        Result = DRIVER_CAN_BOA;
    }
    else if (m_pfDisplayConfigDlg == CAN_STUB_DisplayConfigDlg)
    {
        Result = DRIVER_CAN_STUB;
    }
    else if (m_pfDisplayConfigDlg == CAN_ES581_Usb_DisplayConfigDlg)
    {
        Result = DRIVER_CAN_ETAS_ES581;
    }
    return Result;
}

/**
 * \brief     Initialize operation
 * \req       RS_23_08 - Carry out initialization operations
 *
 * Call for all initialisation operations
 */
HRESULT CDIL_CAN::DILC_PerformInitOperations(void)
{
    return (*m_pfPerformInitOperations)();
}

/**
 * \brief     Closure operation
 * \req       RS_23_09 - Carry out closure operations
 *
 * Call for all uninitialisation operations
 */
HRESULT CDIL_CAN::DILC_PerformClosureOperations(void)
{
    HRESULT hResult =  (*m_pfPerformClosureOperations)();
    vSelectInterface_Dummy();
    return hResult;
}

/**
 * \brief     Get time mode mapping
 * \req       RS_23_10 - Getter for the time mode mapping (usually the 64-bit time stamp by the driver)
 *
 * Call this function to get a system time and the time stamp associated with it
 */
HRESULT CDIL_CAN::DILC_GetTimeModeMapping(SYSTEMTIME& CurrSysTime, UINT64& TimeStamp, LARGE_INTEGER* QueryTickCount)
{
    return (*m_pfGetTimeModeMapping)(CurrSysTime, TimeStamp, QueryTickCount);
}

/**
 * \brief     List hardware interfaces
 * \req       RS_23_11 - Listing of the controllers for the current driver
 *
 * Call this function to list the hardware interfaces available and receive user's choice.
 */
HRESULT CDIL_CAN::DILC_ListHwInterfaces(INTERFACE_HW_LIST& sSelHwInterface, INT& nCount)
{
    return (*m_pfListHwInterfaces)(sSelHwInterface, nCount);
}

/**
 * \brief     Select hardware interfaces
 * \req       RS_23_12 - Selection of a controller from the hardware interface list
 *
 * Call this function to select hardware interfaces.
 */
HRESULT CDIL_CAN::DILC_SelectHwInterfaces(const INTERFACE_HW_LIST& sSelHwInterface, INT nCount)
{
    return (*m_pfSelectHwInterface)(sSelHwInterface, nCount);
}

/**
 * \brief     Deselect hardware interfaces
 * \req       RS_23_13 - Deselection of the presently selected controller
 *
 * Call this function to deselect the selected hardware interface
 */
HRESULT CDIL_CAN::DILC_DeselectHwInterfaces(void)
{
    return (*m_pfDeselectHwInterfaces)();
}

/**
 * \brief     Display configuration dialog box
 * \req       RS_23_14 - Display the configuration dialog box of the present controller
 *
 * Function to display the configuration dialog box for the selected DIL. If
 * the dialog box needs to be displayed been initialised, pass the relevant data
 * as InitData. If it is null, the dialog box is uninitialised. This also contains
 *  the user's choice as OUT parameter */
 */
HRESULT CDIL_CAN::DILC_DisplayConfigDlg(PCHAR& InitData, int& Length)
{
    return (*m_pfDisplayConfigDlg)(InitData, Length);
}

/**
 * \brief     Set configuration data
 * \req       RS_23_15 - Setting of the configuration data for the present controller
 *
 * To set the configuration data for the currently selected DIL. Caller must
 * release the memory.
 */
HRESULT CDIL_CAN::DILC_SetConfigData(PCHAR pInitData, int Length)
{
    return (*m_pfSetConfigData)(pInitData, Length);
}

/**
 * \brief     Start the controller
 * \req       RS_23_16 - Start the presently selected controller (or connect)
 *
 * Start the controller
 */
HRESULT CDIL_CAN::DILC_StartHardware(void)
{
    return (*m_pfStartHardware)();
}

/**
 * \brief     Stop the controller
 * \req       RS_23_17 - Stop the presently selected controller (or disconnect)
 *
 * Stop the controller
 */
HRESULT CDIL_CAN::DILC_StopHardware(void)
{
    return (*m_pfStopHardware)();
}

/**
 * \brief     Reset hardware
 * \req       RS_23_18 - Reset the presently selected controller
 *
 * Reset Hardware
 */
HRESULT CDIL_CAN::DILC_ResetHardware(void)
{
    return (*m_pfResetHardware)();
}

/**
 * \brief     Get transmittable messages
 *
 * Call to receive list of the transmittable messages
 */
HRESULT CDIL_CAN::DILC_GetTxMsgBuffer(BYTE*& pouTxMsgBuffer)
{
    return (*m_pfGetTxMsgBuffer)(pouTxMsgBuffer);
}

/**
 * \brief     Send messages
 * \req       RS_23_19 - Transmit a frame
 *
 * Send messages
 */
HRESULT CDIL_CAN::DILC_SendMsg(DWORD dwClientID, const STCAN_MSG& sCanTxMsg)
{
    return (*m_pfSendMsg)(dwClientID, sCanTxMsg);
}

/**
 * \brief     Get basic board info
 *
 * Get basic information of the board
 */
HRESULT CDIL_CAN::DILC_GetBoardInfo(s_BOARDINFO& RBIN_BoardInfo)
{
    return (*m_pfGetBoardInfo)(RBIN_BoardInfo);
}

/**
 * \brief     Get bus configuration info
 *
 * Get salient informations on current bus configuration
 */
HRESULT CDIL_CAN::DILC_GetBusConfigInfo(BYTE* RBIN_BusInfo)
{
    return (*m_pfGetBusConfigInfo)(RBIN_BusInfo);
}

/**
 * \brief     Get version information
 * \req       RS_23_20 - Getter for the version information of the DIL for the present bus
 *
 * Call to receive the version informations
 */
HRESULT CDIL_CAN::DILC_GetVersionInfo(VERSIONINFO& sVerInfo)
{
    return (*m_pfGetVersionInfo)(sVerInfo);
}

/**
 * \brief     Get last error as string
 * \req       RS_23_21 - In case of any error, a function returns the associated string of the last error
 *
 * Call to get descriptive string of the last error occurred
 */
HRESULT CDIL_CAN::DILC_GetLastErrorString(char acErrorStr[], int nLength)
{
    return (*m_pfGetLastErrorString)(acErrorStr, nLength);
}

/**
 * \brief     Get controller status
 * \req       RS_23_24 - Getter for controller status by callback mechanism
 *
 * Call to get controller status. Caller has to give the handle of a
 * event which will set whenever the controller changes the state.
 */
HRESULT CDIL_CAN::DILC_GetCntrlStatus(const HANDLE& hEvent, UINT& unCntrlStatus)
{
    return (*m_pfGetCntrlStatus)(hEvent, unCntrlStatus);
}

/**
 * \brief     Get controller parameters
 *
 * Call to get Controller parameters. Value will be returned stored in lParam.
 */
HRESULT CDIL_CAN::DILC_GetControllerParams(LONG& lParam, UINT nChannel, ECONTR_PARAM eContrParam)
{
    return (*m_pfGetControllerParams)(lParam, nChannel, eContrParam);
}

/**
 * \brief     Set PASS/STOP filter
 * \req       RS_23_22 - To set pass filters at hardware level
 * \req       RS_23_23 - To set stop filters at hardware level
 *
 * Call to set PASS/STOP filter
 */
HRESULT CDIL_CAN::DILC_FilterFrames(FILTER_TYPE FilterType, TYPE_CHANNEL Channel, UINT* punMsgIDs, UINT nLength)
{
    return (*m_pfFilterFrames)(FilterType, Channel, punMsgIDs, nLength);
}

/**
 * \brief     Get error counts
 *
 * Call to get error counts
 */
HRESULT  CDIL_CAN::DILC_GetErrorCount(SERROR_CNT& sErrorCnt, UINT nChannel, ECONTR_PARAM eContrParam)
{
    return (*m_pfGetErrorCount)(sErrorCnt, nChannel, eContrParam);
}
