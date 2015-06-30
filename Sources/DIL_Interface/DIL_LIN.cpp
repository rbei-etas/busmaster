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
 * \file      DIL_Interface/DIL_LIN.cpp
 * \brief     Interface file for LIN BUS
 * \author    Pradeep Kadoor, Arunkumar Karri
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Interface file for LIN BUS
 */
#include "DIL_Interface_stdafx.h"
#include "include/Error.h"
#include "include/basedefs.h"
#include "DataTypes/Base_WrapperErrorLogger.h"
#include "DataTypes/MsgBufAll_DataTypes.h"
#include "DataTypes/DIL_Datatypes.h"
#include "DILL_Dummy.h"
#include "Dil_LIN.h"

typedef HRESULT (__stdcall* GETIDIL_LIN_CONTROLLER)(void** ppvInterface);
static GETIDIL_LIN_CONTROLLER pfGetIDILLIN_Controller;
static CDIL_LIN_DUMMY* sg_pouDIL_LIN_DUMMY = new CDIL_LIN_DUMMY;

#define VALIDATE_LIN_POINTER(Ptr) if (Ptr == nullptr) {return S_FALSE;}

class ENTRY_DIL
{
public:
    DWORD           m_dwDIL;
    std::string          m_acDIL;
};

static ENTRY_DIL sg_ListDIL[] =
{
    // Added Short cut keys
    /* simulation should be the first entry... */
    {DAL_LIN_NONE, "&Deactivate"},
    {DRIVER_LIN_ETAS_BOA, "ETAS &BOA"},
    {DRIVER_LIN_ISOLAR_EVE_VLIN,"ETAS &ISOLAR-EVE"},
    {DRIVER_LIN_PEAK_USB,"&PEAK USB"},
    {DRIVER_LIN_VECTOR_XL, "&Vector XL"},
	{DRIVER_LIN_KVASER, "&Kvaser LIN"},
};

CDIL_LIN::CDIL_LIN()
{
    m_hDll = nullptr;
    m_hOldDll = nullptr;
    m_dwDriverID = DAL_NONE;
    pfGetIDILLIN_Controller = nullptr;
    m_pBaseDILLIN_Controller = nullptr;
    m_pOldBaseDILLIN_Controller = nullptr;
    vSelectInterface_Dummy();
}

CDIL_LIN::~CDIL_LIN()
{
    //Free the currently selected DIL library
    if ( m_hDll )
    {
        FreeLibrary(m_hDll);
        m_hDll = nullptr;
    }
    //Free the previosuly selected DIL library
    if ( m_hOldDll )
    {
        FreeLibrary(m_hOldDll);
        m_hOldDll = nullptr;
    }
}

int CDIL_LIN::ExitInstance()
{
    return 0;
}
/* ROUTER CODE STARTS */

/**
 * The router is realised as a set of function pointers
 * Initialisation with dummy functions
 */
BOOL CDIL_LIN::InitInstance()
{
    vSelectInterface_Dummy();

    return TRUE;
}

/**
 * Helper Function for Dummy Interface
 */
void CDIL_LIN::vSelectInterface_Dummy(void)
{
    //Delete will be done in DILL_SelectDriver
    m_pBaseDILLIN_Controller = new CDIL_LIN_DUMMY();
}

/* ROUTER CODE FINISHES */

/**
 * \brief     Get DIL list
 * \req       RSI_14_001 - DILL_GetDILList
 * \req       RS_23_01 - Getter for the DIL List
 *
 * Based on the parameter this function renders number of the driver interface
 * layers supported or available. If 'bAvailable' is true, this returns number of
 * the DILs implemented; else the list of the DILs supported by the existing
 * license will be returned. If List is nullptr, only number is returned.
 */
DWORD CDIL_LIN::DILL_GetDILList(bool /*bAvailable*/, DILLIST* List)
{
    DWORD Result = 0;

    if (List != nullptr)
    {
        for (int i = 0; i < sizeof(sg_ListDIL)/sizeof(ENTRY_DIL); i++)
        {
            (*List)[i].m_acName = sg_ListDIL[i].m_acDIL;
            //_tcscpy((*List)[i].m_acName, sg_ListDIL[i].m_acDIL);
            (*List)[i].m_dwDriverID = sg_ListDIL[i].m_dwDIL;
            Result++;
        }
    }

    return Result;
}

/**
 * \brief     Select driver from DIL list
 * \req       RSI_14_002 - DILL_SelectDriver
 * \req       RS_23_02 - Selecting a driver from the DIL list
 *
 * This function selects a driver abstraction layer (DAL). If support for the
 * intended one isn't allowed with the present license, this returns NO_LICENSE.
 */
HRESULT CDIL_LIN::DILL_SelectDriver(DWORD dwDriverID, HWND hWndOwner,
                                    Base_WrapperErrorLogger* pILog)
{
    USES_CONVERSION;

    HRESULT hResult = S_FALSE;

    if (DILL_GetSelectedDriver() == dwDriverID)
    {
        hResult = DAL_ALREADY_SELECTED;
    }
    else if (pILog == nullptr)
    {
        hResult = ERRLGR_INVALID;
    }
    else
    {
        if (hWndOwner == nullptr)
        {
            /* Log a warning message informing about the invalidity of the
            owner window handle. */
        }

        //Save the currently selected DIL library handle
        if ( m_hDll )
        {
            DILL_PerformClosureOperations();
            FreeLibrary(m_hDll);
            delete m_pBaseDILLIN_Controller;
            m_pBaseDILLIN_Controller = nullptr;
            m_hDll = nullptr;
            m_hOldDll = m_hDll;
            m_pOldBaseDILLIN_Controller = m_pBaseDILLIN_Controller;
            m_dwOldDriverID = m_dwDriverID;
            m_hDll = nullptr;
        }

        switch(dwDriverID)
        {

            case DRIVER_LIN_ISOLAR_EVE_VLIN:
                m_hDll = LoadLibrary("LIN_ISOLAR_EVE_VLIN.dll");
                break;

                /* case DRIVER_LIN_DEACTIVATE_VLIN:
                 if (m_hDll == nullptr)
                 {
                     hResult = DAL_INVALID;
                     FreeLibrary(m_hDll);
                     m_dwDriverID = DAL_NONE;
                 }
                 else
                 {
                     hResult = ERR_LOAD_DRIVER;
                     pILog->vLogAMessage(A2T(__FILE__), __LINE__, _("LIN not Active..."));
                 }
                     return hResult;*/
            case DRIVER_LIN_VECTOR_XL:
                m_hDll = LoadLibrary("LIN_Vector_XL.dll");
                break;
            case DRIVER_LIN_ETAS_BOA:
                m_hDll = LoadLibrary("LIN_ETAS_BOA.dll");
                break;
            case DRIVER_LIN_PEAK_USB:
                m_hDll = LoadLibrary("LIN_PEAK_USB.dll");
                break;
			case DRIVER_LIN_KVASER:
                m_hDll = LoadLibrary("LIN_Kvaser.dll");
                break;
            default:
                DILL_PerformClosureOperations();
                vSelectInterface_Dummy();
                m_dwDriverID = DAL_NONE;
                return hResult;
        }

        if (m_hDll == nullptr)
        {
            hResult = ERR_LOAD_DRIVER;
            pILog->vLogAMessage(A2T(__FILE__), __LINE__, _("Load library failed..."));
        }
        else
        {
            // First select the dummy interface
            //DILL_SelectDriver((DWORD)DAL_NONE, hWndOwner, pILog);

            pILog->vLogAMessage(A2T(__FILE__), __LINE__, _("Load library successful..."));
            pfGetIDILLIN_Controller = (GETIDIL_LIN_CONTROLLER)GetProcAddress(m_hDll, "GetIDIL_LIN_Controller");
            pfGetIDILLIN_Controller((void**)&m_pBaseDILLIN_Controller);
            if ( m_pBaseDILLIN_Controller )
            {
                // set the application parameters.
                m_pBaseDILLIN_Controller->LIN_SetAppParams(hWndOwner, pILog);
                hResult = m_pBaseDILLIN_Controller->LIN_LoadDriverLibrary();
                switch (hResult)
                {
                    case S_OK:
                    case DLL_ALREADY_LOADED:
                        m_dwDriverID = dwDriverID;
                        hResult = S_OK;
                        break;

                    default:
                        hResult = ERR_LOAD_DRIVER;
                        pILog->vLogAMessage(A2T(__FILE__), __LINE__, _("Load library failed..."));
                        /* New Code */
                        /* Get rid of current DIL library */
                        if ( m_pBaseDILLIN_Controller )
                        {
                            m_pBaseDILLIN_Controller->LIN_PerformClosureOperations();
                            m_pBaseDILLIN_Controller->LIN_UnloadDriverLibrary();
                        }
                        FreeLibrary(m_hDll);
                        delete m_pBaseDILLIN_Controller;
                        m_pBaseDILLIN_Controller = nullptr;
                        vSelectInterface_Dummy();
                        m_dwDriverID = DAL_NONE;
                        if ( m_hOldDll )
                        {
                            m_hDll = m_hOldDll;
                            m_pBaseDILLIN_Controller = m_pOldBaseDILLIN_Controller;
                            m_dwDriverID =  m_dwOldDriverID;
                        }
                        /* New Code */
                        break;
                }
            }
        }
    }

    return hResult;
}

/**
 * \brief     Manage target message buffer list
 * \req       RSI_14_004 - DILL_ManageMsgBuf
 *
 * This function manages the target message buffer list.
 */
HRESULT CDIL_LIN::DILL_ManageMsgBuf(BYTE bAction, DWORD ClientID, CBaseLINBufFSE* pBufObj)
{
    VALIDATE_LIN_POINTER(m_pBaseDILLIN_Controller);
    return m_pBaseDILLIN_Controller->LIN_ManageMsgBuf(bAction, ClientID, pBufObj);
}

/**
 * \brief     Register a client
 * \req       RSI_14_003 - DILL_RegisterClient
 * \req       RS_23_04 - Registration of a client to simulate a node
 *
 * This function registers the client. Only registered client's buffer will be
 * updated on receive of a msg in the bus.
 */
HRESULT CDIL_LIN::DILL_RegisterClient(BOOL bRegister, DWORD& ClientID, char* pacClientName)
{
    VALIDATE_LIN_POINTER(m_pBaseDILLIN_Controller);
    return m_pBaseDILLIN_Controller->LIN_RegisterClient(bRegister, ClientID, pacClientName);
}

/**
 * \brief     Get selected driver
 * \req       RSI_14_005 - DILL_GetSelectedDriver
 * \req       RS_23_03 - Getter for the presently selected driver
 *
 * This returns ID of the driver interface layer presently selected.
 */
DWORD CDIL_LIN::DILL_GetSelectedDriver(void)
{
    return m_dwDriverID;
}

/**
 * \brief     Initialize operation
 * \req       RSI_14_006 - DILL_PerformInitOperations
 * \req       RS_23_08 - Carry out initialization operations
 *
 * Call for all initialisation operations
 */
HRESULT CDIL_LIN::DILL_PerformInitOperations(void)
{
    VALIDATE_LIN_POINTER(m_pBaseDILLIN_Controller);
    return m_pBaseDILLIN_Controller->LIN_PerformInitOperations();
}

/**
 * \brief     Closure operation
 * \req       RSI_14_007 - DILL_PerformClosureOperations
 * \req       RS_23_09 - Carry out closure operations
 *
 * Call for all uninitialisation operations
 */
HRESULT CDIL_LIN::DILL_PerformClosureOperations(void)
{
    HRESULT hResult =  S_FALSE;
    if ( m_pBaseDILLIN_Controller )
    {
        m_pBaseDILLIN_Controller->LIN_PerformClosureOperations();
        m_pBaseDILLIN_Controller->LIN_UnloadDriverLibrary();
    }

    vSelectInterface_Dummy();
    m_dwDriverID = DAL_NONE;
    return hResult;
}

/**
 * \brief     Get time mode mapping
 * \req       RSI_14_008 - DILL_GetTimeModeMapping
 * \req       RS_23_10 - Getter for the time mode mapping (usually the 64-bit time stamp by the driver)
 *
 * Call this function to get a system time and the time stamp associated with it
 */
HRESULT CDIL_LIN::DILL_GetTimeModeMapping(SYSTEMTIME& CurrSysTime, UINT64& TimeStamp, LARGE_INTEGER& QueryTickCount)
{
    VALIDATE_LIN_POINTER(m_pBaseDILLIN_Controller);
    return m_pBaseDILLIN_Controller->LIN_GetTimeModeMapping(CurrSysTime, TimeStamp, QueryTickCount);
}

/**
 * \brief     List hardware interfaces
 * \req       RSI_14_009 - DILL_ListHwInterfaces
 * \req       RS_23_11 - Listing of the controllers for the current driver
 *
 * Call this function to list the hardware interfaces available and receive user's choice.
 */
HRESULT CDIL_LIN::DILL_ListHwInterfaces(INTERFACE_HW_LIST& sSelHwInterface, INT& nCount)
{
    HRESULT hr = m_pBaseDILLIN_Controller->LIN_ListHwInterfaces(sSelHwInterface, nCount);

    if ( hr != S_OK && m_hOldDll )
    {
        /* If it is a not same Dll, Ex: in case of ES581, IntrepidCS */
        if ( m_hOldDll != m_hDll )
        {
            /* Get rid of current DIL library */
            if ( m_pBaseDILLIN_Controller )
            {
                m_pBaseDILLIN_Controller->LIN_PerformClosureOperations();
                m_pBaseDILLIN_Controller->LIN_UnloadDriverLibrary();
            }
            FreeLibrary(m_hDll);
        }

        /* Retain old DIL selection */
        m_hDll = m_hOldDll;
        m_pBaseDILLIN_Controller = m_pOldBaseDILLIN_Controller;
        m_dwDriverID = m_dwOldDriverID;
    }
    else if ( m_hOldDll )
    {
        /* If it is a not same Dll, Ex: in case of ES581, IntrepidCS */
        if ( m_hOldDll != m_hDll )
        {
            if ( m_pOldBaseDILLIN_Controller )
            {
                m_pOldBaseDILLIN_Controller->LIN_PerformClosureOperations();
                m_pOldBaseDILLIN_Controller->LIN_UnloadDriverLibrary();
            }
            /* Get rid of old DIL library */
            FreeLibrary(m_hOldDll);
        }
    }
    m_hOldDll = nullptr;

    return hr;
}

/**
 * \brief     Select hardware interfaces
 * \req       RSI_14_010 - DILL_SelectHwInterfaces
 * \req       RS_23_12 - Selection of a controller from the hardware interface list
 *
 * Call this function to select hardware interfaces.
 */
HRESULT CDIL_LIN::DILL_SelectHwInterfaces(const INTERFACE_HW_LIST& sSelHwInterface, INT nCount)
{
    VALIDATE_LIN_POINTER(m_pBaseDILLIN_Controller);
    return m_pBaseDILLIN_Controller->LIN_SelectHwInterface(sSelHwInterface, nCount);
}

/**
 * \brief     Deselect hardware interfaces
 * \req       RSI_14_011 - DILL_DeselectHwInterfaces
 * \req       RS_23_13 - Deselection of the presently selected controller
 *
 * Call this function to deselect the selected hardware interface
 */
HRESULT CDIL_LIN::DILL_DeselectHwInterfaces(void)
{
    VALIDATE_LIN_POINTER(m_pBaseDILLIN_Controller);
    return m_pBaseDILLIN_Controller->LIN_DeselectHwInterface();
}

/**
 * \brief     Display configuration dialog box
 * \req       RSI_14_012 - DILL_DisplayConfigDlg
 * \req       RS_23_14 - Display the configuration dialog box of the present controller
 *
 * Function to display the configuration dialog box for the selected DIL. If
 * the dialog box needs to be displayed been initialised, pass the relevant data
 * as InitData. If it is null, the dialog box is uninitialised. This also contains
 * the user's choice as OUT parameter
 */
HRESULT CDIL_LIN::DILL_DisplayConfigDlg(PSCONTROLLER_DETAILS_LIN InitData, int& Length)
{
    VALIDATE_LIN_POINTER(m_pBaseDILLIN_Controller);
    return m_pBaseDILLIN_Controller->LIN_DisplayConfigDlg(InitData, Length);
}

/**
 * \brief     Set configuration data
 * \req       RSI_14_013 - DILL_SetConfigData
 * \req       RS_23_15 - Setting of the configuration data for the present controller
 *
 * To set the configuration data for the currently selected DIL. Caller must
 * release the memory.
 */
HRESULT CDIL_LIN::DILL_SetConfigData( ClusterConfig& ouConfig)
{
    VALIDATE_LIN_POINTER(m_pBaseDILLIN_Controller);
    return m_pBaseDILLIN_Controller->LIN_SetConfigData(ouConfig);
}

/**
 * \brief     Start the controller
 * \req       RSI_14_014 - DILL_StartHardware
 * \req       RS_23_16 - Start the presently selected controller (or connect)
 *
 * Start the controller
 */
HRESULT CDIL_LIN::DILL_StartHardware(void)
{
    if ( m_pBaseDILLIN_Controller )
    {
        return m_pBaseDILLIN_Controller->LIN_StartHardware();
    }
    else
    {
        return S_FALSE;
    }
}

HRESULT CDIL_LIN::DILL_PreStartHardware(void)
{
    if ( m_pBaseDILLIN_Controller )
    {
        return m_pBaseDILLIN_Controller->LIN_PreStartHardware();
    }
    else
    {
        return S_FALSE;
    }
}


/**
 * \brief     Stop the controller
 * \req       RSI_14_015 - DILL_StopHardware
 * \req       RS_23_17 - Stop the presently selected controller (or disconnect)
 *
 * Stop the controller
 */
HRESULT CDIL_LIN::DILL_StopHardware(void)
{
    VALIDATE_LIN_POINTER(m_pBaseDILLIN_Controller);
    return m_pBaseDILLIN_Controller->LIN_StopHardware();
}

/**
 * \brief     Reset hardware
 * \req       RSI_14_016 - DILL_ResetHardware
 * \req       RS_23_18 - Reset the presently selected controller
 *
 * Reset Hardware
 */
HRESULT CDIL_LIN::DILL_ResetHardware(void)
{
    VALIDATE_LIN_POINTER(m_pBaseDILLIN_Controller);
    return m_pBaseDILLIN_Controller->LIN_ResetHardware();
}


/**
 * \brief     Send messages
 * \req       RSI_14_017 - DILL_SendMsg
 * \req       RS_23_19 - Transmit a frame
 *
 * Send messages
 */
HRESULT CDIL_LIN::DILL_SendMsg(DWORD dwClientID, STLIN_MSG& sLinTxMsg)
{
    VALIDATE_LIN_POINTER(m_pBaseDILLIN_Controller);
    return m_pBaseDILLIN_Controller->LIN_SendMsg(dwClientID, sLinTxMsg);
}
/**
 * \brief     Reset Response data of Slave Messages while disconnecting
 * \req       RSI_14_017 - DILL_SendMsg
 * \req       RS_23_19 - Transmit a frame
 *
 *
 */
HRESULT CDIL_LIN::DILL_ResetSlaveRespData(void)
{
    VALIDATE_LIN_POINTER(m_pBaseDILLIN_Controller);
    return m_pBaseDILLIN_Controller->LIN_ResetSlaveRespData();
}

HRESULT CDIL_LIN::DILL_DisableSlaveRespData(DWORD dwClientID, STLIN_MSG& sMessage)
{
    VALIDATE_LIN_POINTER(m_pBaseDILLIN_Controller);
    return m_pBaseDILLIN_Controller->LIN_DisableSlaveRespData(dwClientID, sMessage);

}

/**
 * \brief     Set Response data of Slave Messages while connecting
 * \req       RSI_14_017 - DILL_SendMsg
 * \req       RS_23_19 - Transmit a frame
 *
 * Send messages
 */
HRESULT CDIL_LIN::DILL_SetSlaveRespData(const STLIN_MSG stRespMsg)
{
    VALIDATE_LIN_POINTER(m_pBaseDILLIN_Controller);
    return m_pBaseDILLIN_Controller->LIN_SetSlaveRespData(stRespMsg);
}
/**
 * \brief     Get last error as string
 * \req       RSI_14_019 - DILL_GetLastErrorString
 * \req       RS_23_21 - In case of any error, a function returns the associated string of the last error
 *
 * Call to get descriptive string of the last error occurred
 */
HRESULT CDIL_LIN::DILL_GetLastErrorString(std::string& acErrorStr)
{
    VALIDATE_LIN_POINTER(m_pBaseDILLIN_Controller);
    return m_pBaseDILLIN_Controller->LIN_GetLastErrorString(acErrorStr);
}

/**
 * \brief     Get controller status
 * \req       RSI_14_021 - DILL_GetControllreStatus
 * \req       RS_23_24 - Getter for controller status by callback mechanism
 *
 * Call to get controller status. Caller has to give the handle of a
 * event which will set whenever the controller changes the state.
 */
HRESULT CDIL_LIN::DILL_GetCntrlStatus(const HANDLE& hEvent, UINT& unCntrlStatus)
{
    VALIDATE_LIN_POINTER(m_pBaseDILLIN_Controller);
    return m_pBaseDILLIN_Controller->LIN_GetCntrlStatus(hEvent, unCntrlStatus);
}

/**
 * \brief     Get controller parameters
 * \req       RSI_14_022 - DILL_GetControllerParams
 *
 * Call to get Controller parameters. Value will be returned stored in lParam.
 */
HRESULT CDIL_LIN::DILL_GetControllerParams(LONG& lParam, UINT nChannel, ECONTR_PARAM eContrParam)
{
    if ( m_pBaseDILLIN_Controller )
    {
        return m_pBaseDILLIN_Controller->LIN_GetControllerParams(lParam, nChannel, eContrParam);
    }
    else
    {
        return S_FALSE;
    }
}

HRESULT CDIL_LIN::DILL_GetConfiguration(sCONTROLLERDETAILSLIN psControllerDeConfig[], INT& nSize)
{
    if ( m_pBaseDILLIN_Controller )
    {
        return m_pBaseDILLIN_Controller->LIN_GetConfiguration(psControllerDeConfig, nSize);
    }
    else
    {
        return S_FALSE;
    }
}
/**
 * \brief     Get controller parameters
 *
 *
 * Call to Set Controller parameters. Value will be returned stored in lParam.
 */
HRESULT CDIL_LIN::DILL_SetControllerParams(int nValue, ECONTR_PARAM eContrparam)
{
    if ( m_pBaseDILLIN_Controller )
    {
        return m_pBaseDILLIN_Controller->LIN_SetControllerParams(nValue, eContrparam);
    }
    else
    {
        return S_FALSE;
    }
}

/**
 * \brief     Get error counts
 * \req       RSI_14_023 - DILL_GetErrorCount
 *
 * Call to get error counts
 */
HRESULT  CDIL_LIN::DILL_GetErrorCount(SERROR_CNT& sErrorCnt, UINT nChannel, ECONTR_PARAM eContrParam)
{
    VALIDATE_LIN_POINTER(m_pBaseDILLIN_Controller);
    return m_pBaseDILLIN_Controller->LIN_GetErrorCount(sErrorCnt, nChannel, eContrParam);
}


//Schedule table
HRESULT CDIL_LIN::DILL_RegisterLinScheduleTable(DWORD& dwClientId, int& nChannel, CSheduleTable ouTable, int& nHandle)
{
    VALIDATE_LIN_POINTER(m_pBaseDILLIN_Controller);
    return m_pBaseDILLIN_Controller->LIN_RegisterLinScheduleTable(dwClientId, nChannel, ouTable, nHandle);
}

HRESULT CDIL_LIN::DIIL_DeRegisterLinScheduleTabel(DWORD& dwClientId, int& nChannel, int& nTableHandle)
{
    VALIDATE_LIN_POINTER(m_pBaseDILLIN_Controller);
    return m_pBaseDILLIN_Controller->LIN_DeRegisterLinScheduleTabel( dwClientId, nChannel, nTableHandle);
}

HRESULT CDIL_LIN::DILL_StartLinScheduleTable(DWORD& dwClientId, int& nChannel, int& nTableHandle)
{
    VALIDATE_LIN_POINTER(m_pBaseDILLIN_Controller);
    return m_pBaseDILLIN_Controller->LIN_StartLinScheduleTable(dwClientId, nChannel, nTableHandle);
}
HRESULT CDIL_LIN::DILL_UpdateLinScheduleTable( DWORD& dwClientId, int& nChannel, int& nHandle, CSheduleTable& ouTable )
{
    VALIDATE_LIN_POINTER(m_pBaseDILLIN_Controller);
    return m_pBaseDILLIN_Controller->LIN_UpdateLinScheduleTable( dwClientId, nChannel, nHandle, ouTable );
}


HRESULT CDIL_LIN::DIIL_EnableLinScheuleCommand( DWORD& dwClientId, int& nChannel, int nTableHandle, int nIndex, bool bEnable)
{
    VALIDATE_LIN_POINTER(m_pBaseDILLIN_Controller);
    return m_pBaseDILLIN_Controller->LIN_EnableLinScheuleCommand( dwClientId, nChannel, nTableHandle, nIndex, bEnable);
}


//Individual Header commands.
HRESULT CDIL_LIN::DIIL_RegisterLinHeader(DWORD& dwClientId, int& nChannel, int nId, int nCycleTimer)
{
    VALIDATE_LIN_POINTER(m_pBaseDILLIN_Controller);
    return m_pBaseDILLIN_Controller->LIN_RegisterLinHeader( dwClientId, nChannel, nId, nCycleTimer);
}

HRESULT CDIL_LIN::DIIL_DeRegisterLinHeader(DWORD& dwClientId, int& nChannel, int nId)
{
    VALIDATE_LIN_POINTER(m_pBaseDILLIN_Controller);
    return m_pBaseDILLIN_Controller->LIN_DeRegisterLinHeader( dwClientId, nChannel, nId);
}
