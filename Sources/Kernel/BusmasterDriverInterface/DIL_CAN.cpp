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
 * \file      DIL_Interface/DIL_CAN.cpp
 * \brief     Interface file for CAN BUS
 * \author    Pradeep Kadoor, Arunkumar Karri
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Interface file for CAN BUS
 */
#include "stdafx.h"
#include "include/Error.h"
//#include "include/basedefs.h"
//#include "..\DataTypes/MsgBufAll_DataTypes.h"
/*#include "DILC_Dummy.h"
#include "..\..\CAN_MHS/CAN_MHS_Extern.h"
#include "..\..\CAN_PEAK_USB/CAN_PEAK_USB_Extern.h"
#include "..\..\CAN_ICS_neoVI/CAN_ICS_neoVI_Extern.h"
#include "..\..\CAN_STUB/CAN_STUB_Extern.h"
#include "..\..\CAN_ETAS_BOA/CAN_ETAS_BOA_Extern.h"
#include "..\..\CAN_Vector_XL/CAN_Vector_XL_Extern.h"
#include "..\..\CAN_Kvaser_CAN/CAN_Kvaser_CAN_Extern.h"
#include "..\..\CAN_iVIEW/CAN_i-VIEW_Extern.h"
#include "..\..\CAN_NSI/CAN_NSI_Extern.h"*/
#include "DIL_CAN.h"
#include "../../BUSMASTER/Utility/MultiLanguageSupport.h"

typedef HRESULT (*GETIDIL_CAN_CONTROLLER)(void** ppvInterface);
static GETIDIL_CAN_CONTROLLER pfGetIDILCAN_Controller;
//static CDIL_CAN_DUMMY* sg_pouDIL_CAN_DUMMY = new CDIL_CAN_DUMMY;

class ENTRY_DIL
{
public:
    DWORD           m_dwDIL;
    std::string          m_acDIL;

};

enum
{
    DRIVER_CAN_STUB = 0,
    DRIVER_CAN_PEAK_USB,
    DRIVER_CAN_ICS_NEOVI,
    DRIVER_CAN_ETAS_BOA,
    DRIVER_CAN_ETAS_ES581,
    DRIVER_CAN_ETAS_ES5814,
	DRIVER_CAN_ETAS_ES5821,
    DRIVER_CAN_VECTOR_XL,
    DRIVER_CAN_KVASER_CAN,
    DRIVER_CAN_MHS,
    DRIVER_CAN_NSI,
    DRIVER_CAN_IXXAT,
    DRIVER_CAN_VSCOM,
    DRIVER_CAN_IVIEW,
    DRIVER_CAN_ISOLAR,
    DIL_TOTAL,          // Its value must be <= MAX_DILS
    DAL_NONE = ~0x0
};
static ENTRY_DIL sg_ListDIL[] =
{
    // Added Short cut keys
    /* simulation should be the first entry... */
    {DRIVER_CAN_STUB,       "&Simulation"       },
    /* ...all other drivers should be in alphabetical order */
    {DRIVER_CAN_ETAS_BOA,   "ETAS &BOA"         },
    {DRIVER_CAN_ETAS_ES581, "ETAS &ES581.3"     },
    {DRIVER_CAN_ETAS_ES5814, "ETAS ES581.&4"    },
	{DRIVER_CAN_ETAS_ES5821, "ETAS ES582.&1"	},
    {DRIVER_CAN_ISOLAR,     "ETAS ISO&LAR-EVE"  },
    {DRIVER_CAN_IVIEW,      "&i-VIEW"           },
    {DRIVER_CAN_ICS_NEOVI,  "IntrepidCS ne&oVI" },
    {DRIVER_CAN_IXXAT,      "I&XXAT VCI"        },
    {DRIVER_CAN_KVASER_CAN, "&Kvaser CAN"       },
    {DRIVER_CAN_MHS,        "&MHS Tiny-CAN"     },
    {DRIVER_CAN_NSI,        "&NSI CAN-API"      },
    {DRIVER_CAN_PEAK_USB,   "&PEAK USB"         },
    {DRIVER_CAN_VECTOR_XL,  "&Vector XL"        },
    {DRIVER_CAN_VSCOM,      "VScom &CAN-API"    },
};

CDIL_CAN::CDIL_CAN()
{
    m_hDll = nullptr;
    m_hOldDll = nullptr;
    m_dwDriverID = DAL_NONE;
    pfGetIDILCAN_Controller = nullptr;
    m_pBaseDILCAN_Controller = nullptr;
    m_pOldBaseDILCAN_Controller = nullptr;
    vSelectInterface_Dummy();
}

CDIL_CAN::~CDIL_CAN()
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

int CDIL_CAN::ExitInstance()
{
    return 0;
}
/* ROUTER CODE STARTS */

/**
 * The router is realised as a set of function pointers
 * Initialisation with dummy functions
 */
BOOL CDIL_CAN::InitInstance()
{
    //vSelectInterface_Dummy();

    return TRUE;
}

void CDIL_CAN::vInitialize(SCONTROLLER_DETAILS& controller, BOOL bUpdateHWDesc)
{
	// The default baudrate is taken as 500 kbps
	controller.m_nItemUnderFocus = 64;
	controller.m_nBTR0BTR1 = 49210;
	controller.m_omStrCNF1 = "7";
	controller.m_omStrCNF2 = "B8";
	controller.m_omStrCNF3 = "5";
	controller.m_omStrBTR0 = "C0";
	controller.m_omStrBTR1 = "3A";
	controller.m_omStrBaudrate = "500000";
	controller.m_omStrClock = "16";
	controller.m_omStrSamplePercentage = "75";
	controller.m_omStrSampling = "1";
	controller.m_omStrWarningLimit = "96";
	controller.m_omStrPropagationDelay = "ALL";
	controller.m_omStrSjw = "4";
	controller.m_omStrAccCodeByte1[0] = "0";
	controller.m_omStrAccCodeByte2[0] = "0";
	controller.m_omStrAccCodeByte3[0] = "0";
	controller.m_omStrAccCodeByte4[0] = "0";
	controller.m_omStrAccCodeByte1[1] = "0";
	controller.m_omStrAccCodeByte2[1] = "0";
	controller.m_omStrAccCodeByte3[1] = "0";
	controller.m_omStrAccCodeByte4[1] = "0";
	controller.m_omStrAccMaskByte1[0] = "FF";
	controller.m_omStrAccMaskByte2[0] = "FF";
	controller.m_omStrAccMaskByte3[0] = "FF";
	controller.m_omStrAccMaskByte4[0] = "FF";
	controller.m_omStrAccMaskByte1[1] = "FF";
	controller.m_omStrAccMaskByte2[1] = "FF";
	controller.m_omStrAccMaskByte3[1] = "FF";
	controller.m_omStrAccMaskByte4[1] = "FF";

	if (bUpdateHWDesc)
	{
		controller.m_omHardwareDesc = "";
	}
	controller.m_bAccFilterMode = FALSE;
	controller.m_ucControllerMode = 0x1;
	controller.m_enmHWFilterType[0] = HW_FILTER_ACCEPT_ALL;
	controller.m_enmHWFilterType[1] = HW_FILTER_ACCEPT_ALL;
	controller.m_bSelfReception = TRUE;
	controller.m_bLowSpeed = FALSE;
	controller.m_unBTL_Cycles = 10;

	/* CAN FD related parameters */
	controller.m_bcanFDEnabled = FALSE;
	controller.m_bSupportCANFD = FALSE;
	controller.m_unDataBitRate = 2000000;
	controller.m_unDataSamplePoint = 70;
	controller.m_unDataBTL_Cycles = 10;
	controller.m_unDataSJW = 03;
	controller.m_bTxDelayCompensationControl = 0;   // OCI_CANFD_TX_DELAY_COMPENSATION_OFF
	controller.m_unTxSecondarySamplePointOffset = 0;
}
/**
 * Helper Function for Dummy Interface
 */
void CDIL_CAN::vSelectInterface_Dummy(void)
{
    //m_pBaseDILCAN_Controller = (CBaseDIL_CAN_Controller*) sg_pouDIL_CAN_DUMMY;
}

/* ROUTER CODE FINISHES */

/**
 * \brief     Get DIL list
 * \req       RSI_14_001 - DILC_GetDILList
 * \req       RS_23_01 - Getter for the DIL List
 *
 * Based on the parameter this function renders number of the driver interface
 * layers supported or available. If 'bAvailable' is true, this returns number of
 * the DILs implemented; else the list of the DILs supported by the existing
 * license will be returned. If List is nullptr, only number is returned.
 */
DWORD CDIL_CAN::DILC_GetDILList(bool /*bAvailable*/, DILLIST* List)
{
    DWORD Result = 0;

    if (List != nullptr)
    {
        for (int i = 0; i < sizeof(sg_ListDIL)/sizeof(ENTRY_DIL); i++)
        {
            (*List)[i].m_acName = sg_ListDIL[i].m_acDIL;
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
HMODULE CDIL_CAN::vLoadEtasBoaLibrary()
{
    //USES_CONVERSION;

    LONG lError = 0;
    HKEY sKey;

    // Check for BOA 2
    lError = RegOpenKeyEx(HKEY_LOCAL_MACHINE, "SOFTWARE\\ETAS\\BOA\\2", 0, KEY_READ, &sKey);
    if(lError==ERROR_SUCCESS)
    {
        RegCloseKey(sKey);
        return LoadLibrary("CAN_ETAS_BOA_2.dll");
    }

    // Check for BOA 1.5
    lError = RegOpenKeyEx(HKEY_LOCAL_MACHINE, "SOFTWARE\\ETAS\\BOA\\1.5", 0, KEY_READ, &sKey);
    if(lError==ERROR_SUCCESS)
    {
        RegCloseKey(sKey);
        return LoadLibrary("CAN_ETAS_BOA_1_5.dll");
    }

    // Check for BOA 1.4
    lError = RegOpenKeyEx(HKEY_LOCAL_MACHINE, "SOFTWARE\\ETAS\\BOA\\1.4", 0, KEY_READ, &sKey);
    if(lError==ERROR_SUCCESS)
    {
        RegCloseKey(sKey);
        return LoadLibrary("CAN_ETAS_BOA_1_4.dll");
    }

    return nullptr;
}

/**
 * \brief     Select driver from DIL list
 * \req       RSI_14_002 - DILC_SelectDriver
 * \req       RS_23_02 - Selecting a driver from the DIL list
 *
 * This function selects a driver abstraction layer (DAL). If support for the
 * intended one isn't allowed with the present license, this returns NO_LICENSE.
 */
HRESULT CDIL_CAN::DILC_SelectDriver(DWORD dwDriverID, HWND hWndOwner)
{
    //USES_CONVERSION;

    HRESULT hResult = S_FALSE;

    if (DILC_GetSelectedDriver() == dwDriverID)
    {
        hResult = DAL_ALREADY_SELECTED;
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
            //FreeLibrary(m_hDll);
            m_hOldDll = m_hDll;
            m_pOldBaseDILCAN_Controller = m_pBaseDILCAN_Controller;
            m_dwOldDriverID = m_dwDriverID;
            m_hDll = nullptr;
        }

        switch(dwDriverID)
        {
            case DRIVER_CAN_PEAK_USB:
                m_hDll = LoadLibrary("CAN_PEAK_USB.dll");
                break;

            case DRIVER_CAN_ICS_NEOVI:
            case DRIVER_CAN_ETAS_ES581:
                m_hDll = LoadLibrary("CAN_ICS_neoVI.dll");
                break;
            case DRIVER_CAN_ETAS_ES5814:
			case DRIVER_CAN_ETAS_ES5821:
            case DRIVER_CAN_ETAS_BOA:
                m_hDll = vLoadEtasBoaLibrary();
                break;

            case DRIVER_CAN_IVIEW:
                m_hDll = LoadLibrary("CAN_i-VIEW.dll");
                break;

            case DRIVER_CAN_VECTOR_XL:
                m_hDll = LoadLibrary("CAN_Vector_XL.dll");
                break;

            case DRIVER_CAN_IXXAT:
                m_hDll = LoadLibrary("CAN_IXXAT_VCI.dll");
                break;

            case DRIVER_CAN_KVASER_CAN:
                m_hDll = LoadLibrary("CAN_Kvaser_CAN.dll");
                break;

            case DRIVER_CAN_STUB:
                m_hDll = LoadLibrary("CAN_STUB.dll");
                break;

            case DRIVER_CAN_MHS:
                m_hDll = LoadLibrary("CAN_MHS.dll");
                break;

            case DRIVER_CAN_NSI:
                m_hDll = LoadLibrary("CAN_NSI.dll");
                break;

            case DRIVER_CAN_VSCOM:
                m_hDll = LoadLibrary("CAN_VSCOM.dll");
                break;
            case DRIVER_CAN_ISOLAR:
                m_hDll = LoadLibrary("CAN_ISOLAR_EVE_VCAN.dll");
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

        if (m_hDll == nullptr)
        {
            hResult = ERR_LOAD_DRIVER;
        }
        else
        {
            pfGetIDILCAN_Controller = (GETIDIL_CAN_CONTROLLER)GetProcAddress(m_hDll, "GetIDIL_CAN_Controller");
            pfGetIDILCAN_Controller((void**)&m_pBaseDILCAN_Controller);
            if ( m_pBaseDILCAN_Controller )
            {
                // set the application parameters.
                m_pBaseDILCAN_Controller->CAN_SetAppParams(hWndOwner);
                hResult = m_pBaseDILCAN_Controller->CAN_LoadDriverLibrary();
                switch (hResult)
                {
                    case S_OK:
                    case DLL_ALREADY_LOADED:
                        m_dwDriverID = dwDriverID;
                        hResult = S_OK;
                        break;

                    default:
                        hResult = ERR_LOAD_DRIVER;
                        /* New Code */
                        /* Get rid of current DIL library */
                        if ( m_pBaseDILCAN_Controller )
                        {
                            m_pBaseDILCAN_Controller->CAN_PerformClosureOperations();
                            m_pBaseDILCAN_Controller->CAN_UnloadDriverLibrary();
                        }
                        FreeLibrary(m_hDll);
                        if ( m_hOldDll )
                        {
                            m_hDll = m_hOldDll;
                            m_pBaseDILCAN_Controller = m_pOldBaseDILCAN_Controller;
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
 * This function registers the client. Only registered client's buffer will be
 * updated on receive of a msg in the bus.
 */
HRESULT CDIL_CAN::DILC_RegisterClient(BOOL bRegister, DWORD& ClientID, char* pacClientName)
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
    HRESULT hResult =  S_FALSE;
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
HRESULT CDIL_CAN::DILC_GetTimeModeMapping(SYSTEMTIME& CurrSysTime, UINT64& TimeStamp, LARGE_INTEGER& QueryTickCount)
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
HRESULT CDIL_CAN::DILC_ListHwInterfaces(INTERFACE_HW_LIST& sSelHwInterface, INT& nCount, PSCONTROLLER_DETAILS InitData, bool bLoadedFromXml /* =1 only when loaded from nLoadXMLConfiguration() */)
{
    if(nullptr == m_pBaseDILCAN_Controller)
    {
        return S_FALSE;
    }

	if (m_dwDriverID != m_dwOldDriverID && bLoadedFromXml != 1) // Flag is used to prevent initialization when loaded from XMl Config
	{
		for(int i = 0; i < defNO_OF_CHANNELS; i++)
		{
			m_asOldControllerDetails[i] = InitData[i];
			vInitialize(InitData[i],FALSE);
		}
	}
    HRESULT hr = m_pBaseDILCAN_Controller->CAN_ListHwInterfaces(sSelHwInterface, nCount,InitData);

    if ( hr != S_OK && m_hOldDll )
    {
        /* If it is a not same Dll, Ex: in case of ES581, IntrepidCS */
        if ( m_hOldDll != m_hDll )
        {
            /* Get rid of current DIL library */
            if ( nullptr != m_pBaseDILCAN_Controller  )
            {
                m_pBaseDILCAN_Controller->CAN_PerformClosureOperations();
                m_pBaseDILCAN_Controller->CAN_UnloadDriverLibrary();
            }
            FreeLibrary(m_hDll);
        }

        /* Retain old DIL selection */
        m_hDll = m_hOldDll;
        m_pBaseDILCAN_Controller = m_pOldBaseDILCAN_Controller;
        m_dwDriverID = m_dwOldDriverID;
		for(int i = 0; i < defNO_OF_CHANNELS; i++)
		{
			InitData[i] = m_asOldControllerDetails[i];
		}
    }
    else if ( m_hOldDll )
    {
        /* If it is a not same Dll, Ex: in case of ES581, IntrepidCS */
        if ( m_hOldDll != m_hDll )
        {
            if ( nullptr != m_pOldBaseDILCAN_Controller )
            {
                m_pOldBaseDILCAN_Controller->CAN_PerformClosureOperations();
                m_pOldBaseDILCAN_Controller->CAN_UnloadDriverLibrary();
            }
            /* Get rid of old DIL library */
            FreeLibrary(m_hOldDll);
        }
		m_dwOldDriverID = m_dwDriverID;
    }
    m_hOldDll = nullptr;

    return hr;
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
 * \brief     Set configuration data
 * \req       RSI_14_013 - DILC_SetConfigData
 * \req       RS_23_15 - Setting of the configuration data for the present controller
 *
 * To set the configuration data for the currently selected DIL. Caller must
 * release the memory.
 */
HRESULT CDIL_CAN::DILC_SetConfigData(PSCONTROLLER_DETAILS pInitData, int Length)
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
HRESULT CDIL_CAN::DILC_GetLastErrorString(std::string& acErrorStr)
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
 * \brief     Get controller parameters
 *
 *
 * Call to Set Controller parameters. Value will be returned stored in lParam.
 */
HRESULT CDIL_CAN::DILC_SetControllerParams(int nValue, ECONTR_PARAM eContrparam)
{
    if ( m_pBaseDILCAN_Controller )
    {
        return m_pBaseDILCAN_Controller->CAN_SetControllerParams(nValue, eContrparam);
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
HRESULT CDIL_CAN::DILC_SetHardwareChannel(PSCONTROLLER_DETAILS m_asControllerDetails, DWORD dwDriverId,bool bHardwareListed, unsigned int unChannelCount)
{
    return m_pBaseDILCAN_Controller->CAN_SetHardwareChannel(m_asControllerDetails, dwDriverId, bHardwareListed, unChannelCount);
}
