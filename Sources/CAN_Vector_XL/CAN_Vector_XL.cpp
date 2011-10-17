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
 * \author    Tobias Lorenz
 * \copyright Copyright (c) 2011, ETAS GmbH. All rights reserved.
 *
 * Source file for Vector XL DIL functions
 */
// CAN_Vector_XL.cpp : Defines the initialization routines for the DLL.
//

#include "CAN_Vector_XL_stdafx.h"
#include "DataTypes/Base_WrapperErrorLogger.h"
#include "DataTypes/MsgBufAll_DataTypes.h"
#include "DataTypes/DIL_Datatypes.h"
#include "Include/BaseDefs.h"
#include "Include/DIL_CommonDefs.h"
#include "Include/CanUsbDefs.h"
#include "Include/Can_Error_Defs.h"
#include "ConfigDialogsDIL/API_Dialog.h"

#define DYNAMIC_XLDRIVER_DLL
#include "EXTERNAL_INCLUDE/vxlapi.h"

#define USAGE_EXPORT
#include "CAN_Vector_XL_Extern.h"

/* function pointers */
XLGETAPPLCONFIG                xlGetApplConfig = NULL;
XLSETAPPLCONFIG                xlSetApplConfig = NULL;
XLGETDRIVERCONFIG              xlGetDriverConfig = NULL;
XLGETCHANNELINDEX              xlGetChannelIndex = NULL;
XLGETCHANNELMASK               xlGetChannelMask = NULL;
XLOPENPORT                     xlOpenPort = NULL;
XLSETTIMERRATE                 xlSetTimerRate = NULL;
XLRESETCLOCK                   xlResetClock = NULL;
XLSETNOTIFICATION              xlSetNotification = NULL;
XLFLUSHRECEIVEQUEUE            xlFlushReceiveQueue = NULL;
XLGETRECEIVEQUEUELEVEL         xlGetReceiveQueueLevel = NULL;
XLACTIVATECHANNEL              xlActivateChannel = NULL;
XLRECEIVE                      xlReceive = NULL;                        
XLGETEVENTSTRING               xlGetEventString = NULL;
XLGETERRORSTRING               xlGetErrorString = NULL;
XLGETSYNCTIME                  xlGetSyncTime = NULL;
XLGENERATESYNCPULSE            xlGenerateSyncPulse = NULL;
XLPOPUPHWCONFIG                xlPopupHwConfig = NULL;
XLDEACTIVATECHANNEL            xlDeactivateChannel = NULL;
XLCLOSEPORT                    xlClosePort = NULL;
XLSETTIMERBASEDNOTIFY          xlSetTimerBasedNotify = NULL;  
XLSETTIMERRATEANDCHANNEL       xlSetTimerRateAndChannel = NULL;
XLGETLICENSEINFO               xlGetLicenseInfo = NULL;

/* CAN specific functions */
XLCANSETCHANNELOUTPUT          xlCanSetChannelOutput = NULL;    
XLCANSETCHANNELMODE            xlCanSetChannelMode = NULL; 
XLCANSETRECEIVEMODE            xlCanSetReceiveMode = NULL; 
XLCANSETCHANNELTRANSCEIVER     xlCanSetChannelTransceiver = NULL;
XLCANSETCHANNELPARAMS          xlCanSetChannelParams = NULL;           
XLCANSETCHANNELPARAMSC200      xlCanSetChannelParamsC200 = NULL;        
XLCANSETCHANNELBITRATE         xlCanSetChannelBitrate = NULL;   
XLCANSETCHANNELACCEPTANCE      xlCanSetChannelAcceptance = NULL;       
XLCANADDACCEPTANCERANGE        xlCanAddAcceptanceRange = NULL;    
XLCANREMOVEACCEPTANCERANGE     xlCanRemoveAcceptanceRange = NULL; 
XLCANRESETACCEPTANCE           xlCanResetAcceptance = NULL;   
XLCANREQUESTCHIPSTATE          xlCanRequestChipState = NULL; 
XLCANFLUSHTRANSMITQUEUE        xlCanFlushTransmitQueue = NULL;           
XLCANTRANSMIT                  xlCanTransmit = NULL;

/* Local variables */
static XLCLOSEDRIVER           xlDllCloseDriver = NULL;
static XLOPENDRIVER            xlDllOpenDriver = NULL;

static HINSTANCE               hxlDll;

static HWND sg_hOwnerWnd = NULL;
static Base_WrapperErrorLogger* sg_pIlog   = NULL;

/**
 * \return S_OK for success, S_FALSE for failure
 *
 * Sets the application params.
 */
USAGEMODE HRESULT CAN_Vector_XL_SetAppParams(HWND hWndOwner, Base_WrapperErrorLogger *pILog)
{
    sg_hOwnerWnd = hWndOwner;
    sg_pIlog = pILog;

	return S_OK;
}


/**
 * \return S_OK for success, S_FALSE for failure
 *
 * Unloads the driver library.
 */
USAGEMODE HRESULT CAN_Vector_XL_UnloadDriverLibrary(void)
{
    if (hxlDll != NULL)
    {
        FreeLibrary(hxlDll);
        hxlDll = NULL;
    }

    return S_OK;
}

/**
 * \return S_OK for success, S_FALSE for failure
 *
 * Registers the buffer pBufObj to the client ClientID
 */
USAGEMODE HRESULT CAN_Vector_XL_ManageMsgBuf(BYTE /*bAction*/, DWORD /*ClientID*/, CBaseCANBufFSE* /*pBufObj*/)
{
    return S_OK;
}

/**
 * \return S_OK for success, S_FALSE for failure
 *
 * Registers a client to the DIL. ClientID will have client id
 * which will be used for further client related calls  
 */
USAGEMODE HRESULT CAN_Vector_XL_RegisterClient(BOOL /*bRegister*/, DWORD& /*ClientID*/, TCHAR* /*pacClientName*/)
{
    return S_OK;
}

/**
 * \return S_OK for success, S_FALSE for failure
 *
 * Returns the controller status. hEvent will be registered
 * and will be set whenever there is change in the controller
 * status.
 */
USAGEMODE HRESULT CAN_Vector_XL_GetCntrlStatus(const HANDLE& /*hEvent*/, UINT& /*unCntrlStatus*/)
{
    return S_OK;
}

/**
 * \return S_OK for success, S_FALSE for failure
 *
 * Loads BOA related libraries. Updates BOA API pointers
 */
USAGEMODE HRESULT CAN_Vector_XL_LoadDriverLibrary(void)
{
    HRESULT hResult = S_OK;

	if (hxlDll != NULL)
    {
        sg_pIlog->vLogAMessage(A2T(__FILE__), __LINE__, _T("vxlapi.dll already loaded"));
        hResult = DLL_ALREADY_LOADED;
    }

    if (hResult == S_OK)
    {
        hxlDll = LoadLibrary("vxlapi.dll");
        if (hxlDll == NULL)
        {
            sg_pIlog->vLogAMessage(A2T(__FILE__), __LINE__, _T("vxlapi.dll loading failed"));
            sg_pIlog->vLogAMessage(A2T(__FILE__), __LINE__, _T("Please have a look at: https://github.com/rbei-etas/busmaster/wiki/Hardware-support"));
            hResult = ERR_LOAD_DRIVER;
        }
        else
        {
			xlDllOpenDriver           = (XLOPENDRIVER)                GetProcAddress(hxlDll,"xlOpenDriver");
			xlDllCloseDriver          = (XLCLOSEDRIVER)               GetProcAddress(hxlDll,"xlCloseDriver");
	                                                                                                                                  
			/* bus independed functions */
			xlGetApplConfig            = (XLGETAPPLCONFIG)            GetProcAddress(hxlDll,"xlGetApplConfig");
			xlSetApplConfig            = (XLSETAPPLCONFIG)            GetProcAddress(hxlDll,"xlSetApplConfig");
			xlGetDriverConfig          = (XLGETDRIVERCONFIG)          GetProcAddress(hxlDll,"xlGetDriverConfig");
			xlGetChannelIndex          = (XLGETCHANNELINDEX)          GetProcAddress(hxlDll,"xlGetChannelIndex");
			xlGetChannelMask           = (XLGETCHANNELMASK)           GetProcAddress(hxlDll,"xlGetChannelMask");
			xlOpenPort                 = (XLOPENPORT)                 GetProcAddress(hxlDll,"xlOpenPort");
			xlSetTimerRate             = (XLSETTIMERRATE)             GetProcAddress(hxlDll,"xlSetTimerRate");
			xlResetClock               = (XLRESETCLOCK)               GetProcAddress(hxlDll,"xlResetClock");
			xlSetNotification          = (XLSETNOTIFICATION)          GetProcAddress(hxlDll,"xlSetNotification");
			xlFlushReceiveQueue        = (XLFLUSHRECEIVEQUEUE)        GetProcAddress(hxlDll,"xlFlushReceiveQueue");
			xlGetReceiveQueueLevel     = (XLGETRECEIVEQUEUELEVEL)     GetProcAddress(hxlDll,"xlGetReceiveQueueLevel");
			xlActivateChannel          = (XLACTIVATECHANNEL)          GetProcAddress(hxlDll,"xlActivateChannel");
			xlReceive                  = (XLRECEIVE)                  GetProcAddress(hxlDll,"xlReceive");
			xlGetEventString           = (XLGETEVENTSTRING)           GetProcAddress(hxlDll,"xlGetEventString");
			xlGetErrorString           = (XLGETERRORSTRING)           GetProcAddress(hxlDll,"xlGetErrorString");
			xlGenerateSyncPulse        = (XLGENERATESYNCPULSE)        GetProcAddress(hxlDll,"xlGenerateSyncPulse");
			xlGetSyncTime              = (XLGETSYNCTIME)              GetProcAddress(hxlDll,"xlGetSyncTime");
			xlPopupHwConfig            = (XLPOPUPHWCONFIG)            GetProcAddress(hxlDll,"xlPopupHwConfig");
			xlDeactivateChannel        = (XLDEACTIVATECHANNEL)        GetProcAddress(hxlDll,"xlDeactivateChannel");
			xlClosePort                = (XLCLOSEPORT)                GetProcAddress(hxlDll,"xlClosePort");
			xlSetTimerBasedNotify      = (XLSETTIMERBASEDNOTIFY)      GetProcAddress(hxlDll,"xlSetTimerBasedNotify");
			xlSetTimerRateAndChannel   = (XLSETTIMERRATEANDCHANNEL)   GetProcAddress(hxlDll, "xlSetTimerRateAndChannel");
			xlGetLicenseInfo           = (XLGETLICENSEINFO)           GetProcAddress(hxlDll, "xlGetLicenseInfo");
	   
			/* CAN specific functions */
			xlCanSetChannelOutput      = (XLCANSETCHANNELOUTPUT)      GetProcAddress(hxlDll,"xlCanSetChannelOutput");
			xlCanSetChannelMode        = (XLCANSETCHANNELMODE)        GetProcAddress(hxlDll,"xlCanSetChannelMode");
			xlCanSetReceiveMode        = (XLCANSETRECEIVEMODE)        GetProcAddress(hxlDll,"xlCanSetReceiveMode");
			xlCanSetChannelTransceiver = (XLCANSETCHANNELTRANSCEIVER) GetProcAddress(hxlDll,"xlCanSetChannelTransceiver");
			xlCanSetChannelParams      = (XLCANSETCHANNELPARAMS)      GetProcAddress(hxlDll,"xlCanSetChannelParams");
			xlCanSetChannelParamsC200  = (XLCANSETCHANNELPARAMSC200)  GetProcAddress(hxlDll,"xlCanSetChannelParamsC200");
			xlCanSetChannelBitrate     = (XLCANSETCHANNELBITRATE)     GetProcAddress(hxlDll,"xlCanSetChannelBitrate");
			xlCanSetChannelAcceptance  = (XLCANSETCHANNELACCEPTANCE)  GetProcAddress(hxlDll,"xlCanSetChannelAcceptance");
			xlCanAddAcceptanceRange    = (XLCANADDACCEPTANCERANGE)    GetProcAddress(hxlDll,"xlCanAddAcceptanceRange");
			xlCanRemoveAcceptanceRange = (XLCANREMOVEACCEPTANCERANGE) GetProcAddress(hxlDll,"xlCanRemoveAcceptanceRange");
			xlCanResetAcceptance	   = (XLCANRESETACCEPTANCE)       GetProcAddress(hxlDll,"xlCanResetAcceptance");
			xlCanRequestChipState      = (XLCANREQUESTCHIPSTATE)      GetProcAddress(hxlDll,"xlCanRequestChipState");
			xlCanFlushTransmitQueue	   = (XLCANFLUSHTRANSMITQUEUE)    GetProcAddress(hxlDll,"xlCanFlushTransmitQueue");
			xlCanTransmit              = (XLCANTRANSMIT)              GetProcAddress(hxlDll,"xlCanTransmit");
    
			/* check for error */
			if (!xlDllOpenDriver || !xlDllCloseDriver ||
				!xlGetApplConfig || !xlSetApplConfig ||
				!xlGetDriverConfig || !xlGetChannelIndex ||
				!xlGetChannelMask || !xlOpenPort ||
				!xlSetTimerRate || !xlResetClock ||
				!xlSetNotification || !xlFlushReceiveQueue ||
				!xlGetReceiveQueueLevel || !xlActivateChannel ||
				!xlReceive || !xlGetEventString ||
				!xlGetErrorString || !xlGenerateSyncPulse ||
				!xlGetSyncTime || !xlPopupHwConfig ||
				!xlDeactivateChannel || !xlClosePort ||
				!xlSetTimerBasedNotify || !xlSetTimerRateAndChannel ||
				!xlGetLicenseInfo || !xlCanSetChannelOutput ||
				!xlCanSetChannelMode || !xlCanSetReceiveMode ||
				!xlCanSetChannelTransceiver || !xlCanSetChannelParams ||
				!xlCanSetChannelParamsC200 || !xlCanSetChannelBitrate ||
				!xlCanSetChannelAcceptance || !xlCanAddAcceptanceRange ||
				!xlCanRemoveAcceptanceRange || !xlCanResetAcceptance ||
				!xlCanRequestChipState || !xlCanFlushTransmitQueue ||
				!xlCanTransmit)
			{
                FreeLibrary(hxlDll);
                sg_pIlog->vLogAMessage(A2T(__FILE__),
                                       __LINE__, _T("Getting Process address of the APIs failed"));
                hResult = ERR_LOAD_DRIVER;
            }
        }
    }

    return hResult;
}

/**
 * \return S_OK for success, S_FALSE for failure
 *
 * Performs intial operations.
 * Initializes filter, queue, controller config
 * with default values.
 */
USAGEMODE HRESULT CAN_Vector_XL_PerformInitOperations(void)
{
	HRESULT hResult = S_FALSE;

	if (xlDllOpenDriver() == XL_SUCCESS)
		hResult = S_OK;

	return hResult;
}

/**
 * \return S_OK for success, S_FALSE for failure
 *
 * Copies the controller config values into channel's
 * controller config structure.
 */
static BOOL bLoadDataFromContr(PSCONTROLER_DETAILS /*pControllerDetails*/)
{
    return S_OK;
}

/**
 * \return S_OK for success, S_FALSE for failure
 *
 * Performs closure operations.
 */
USAGEMODE HRESULT CAN_Vector_XL_PerformClosureOperations(void)
{
    HRESULT hResult = S_OK;

	xlDllCloseDriver();

	return hResult;
}

/**
 * \return S_OK for success, S_FALSE for failure
 *
 * Gets the time mode mapping of the hardware. CurrSysTime
 * will be updated with the system time ref.
 * TimeStamp will be updated with the corresponding timestamp.
 */
USAGEMODE HRESULT CAN_Vector_XL_GetTimeModeMapping(SYSTEMTIME& /*CurrSysTime*/, UINT64& /*TimeStamp*/, LARGE_INTEGER* /*QueryTickCount*/)
{
    return S_OK;
}

/**
 * \return S_OK for success, S_FALSE for failure
 *
 * Lists the hardware interface available. sSelHwInterface
 * will contain the user selected hw interface.
 */
USAGEMODE HRESULT CAN_Vector_XL_ListHwInterfaces(INTERFACE_HW_LIST& asSelHwInterface, INT& nCount)
{
    USES_CONVERSION;
    HRESULT hResult = S_FALSE;

    asSelHwInterface[0].m_dwIdInterface = 0;
	asSelHwInterface[0].m_dwVendor = DRIVER_CAN_VECTOR_XL;
    _stprintf(asSelHwInterface[0].m_acNameInterface, _T("Vector XL"));
    _stprintf(asSelHwInterface[0].m_acDescription, _T("PCMCIA to 2xCAN"));
    _stprintf(asSelHwInterface[0].m_acDeviceName, _T("CANcardXL"));
	nCount = 1;
	hResult = S_OK;

    return hResult;
}

/**
 * \brief Selects the hardware interface selected by the user.
 * \return S_OK for success, S_FALSE for failure
 *
 * Function to List Hardware interfaces connect to the system and requests to the
 * user to select.
 */
USAGEMODE HRESULT CAN_Vector_XL_SelectHwInterface(const INTERFACE_HW_LIST& /*asSelHwInterface*/, INT /*nCount*/)
{   
    return S_OK;
}

/**
 * \return S_OK for success, S_FALSE for failure
 *
 * Deselects the selected hardware interface.
 */
USAGEMODE HRESULT CAN_Vector_XL_DeselectHwInterface(void)
{
    return S_OK;
}

/**
 * \return S_OK for success, S_FALSE for failure
 *
 * Displays the controller configuration dialog.
 * Fields are initialized with values supplied by InitData.
 * InitData will be updated with the user selected values.
 */
USAGEMODE HRESULT CAN_Vector_XL_DisplayConfigDlg(PCHAR& /*InitData*/, INT& /*Length*/)
{
    return S_OK;
}

/**
 * S_OK for success, S_FALSE for failure
 *
 * Sets the controller configuration data supplied by InitData.
 */
USAGEMODE HRESULT CAN_Vector_XL_SetConfigData(PCHAR /*pInitData*/, INT /*Length*/)
{
    return S_OK;
}

/**
 * \return S_OK for success, S_FALSE for failure
 *
 * Starts the controller.
 */
USAGEMODE HRESULT CAN_Vector_XL_StartHardware(void)
{
    return S_OK;
}

/**
 * \return S_OK for success, S_FALSE for failure
 *
 * Stops the controller.
 */
USAGEMODE HRESULT CAN_Vector_XL_StopHardware(void)
{
    return S_OK;
}

/**
 * \return S_OK for success, S_FALSE for failure
 *
 * Resets the controller.
 */
USAGEMODE HRESULT CAN_Vector_XL_ResetHardware(void)
{
    return S_OK;
}

/**
 * \return S_OK for success, S_FALSE for failure
 *
 * Gets the Tx queue configured.
 */
USAGEMODE HRESULT CAN_Vector_XL_GetTxMsgBuffer(BYTE*& /*pouFlxTxMsgBuffer*/)
{
    return S_OK;
}

/**
 * \return S_OK for success, S_FALSE for failure
 *
 * Sends STCAN_MSG structure from the client dwClientID.   
 */
USAGEMODE HRESULT CAN_Vector_XL_SendMsg(DWORD /*dwClientID*/, const STCAN_MSG& /*sCanTxMsg*/)
{
    return S_OK;
}

/**
 * \return S_OK for success, S_FALSE for failure
 *
 * Gets board info.
 */
USAGEMODE HRESULT CAN_Vector_XL_GetBoardInfo(s_BOARDINFO& /*BoardInfo*/)
{
    return S_OK;
}

/**
 * \return S_OK for success, S_FALSE for failure
 *
 * Gets bus config info.
 */
USAGEMODE HRESULT CAN_Vector_XL_GetBusConfigInfo(BYTE* /*BusInfo*/)
{
    return S_OK;
}

/**
 * \return S_OK for success, S_FALSE for failure
 *
 * Gets driver version info.
 */
USAGEMODE HRESULT CAN_Vector_XL_GetVersionInfo(VERSIONINFO& /*sVerInfo*/)
{
    return S_OK;
}

/**
 * \return S_OK for success, S_FALSE for failure
 *
 * Gets last occured error and puts inside acErrorStr.
 */
USAGEMODE HRESULT CAN_Vector_XL_GetLastErrorString(CHAR* /*acErrorStr*/, INT /*nLength*/)
{
    return S_OK;
}

/**
 * \return S_OK for success, S_FALSE for failure
 *
 * Applies FilterType(PASS/STOP) filter for corresponding
 * channel. Frame ids are supplied by punMsgIds.
 */
USAGEMODE HRESULT CAN_Vector_XL_FilterFrames(FILTER_TYPE /*FilterType*/, TYPE_CHANNEL /*Channel*/, UINT* /*punMsgIds*/, UINT /*nLength*/)
{
    return S_OK;
}

/**
 * \return S_OK for success, S_FALSE for failure
 *
 * Gets the controller param eContrParam of the channel.
 * Value stored in lParam.
 */
USAGEMODE HRESULT CAN_Vector_XL_GetControllerParams(LONG& /*lParam*/, UINT /*nChannel*/, ECONTR_PARAM /*eContrParam*/)
{
    return S_OK;
}

/**
 * \return S_OK for success, S_FALSE for failure
 *
 * Gets the error counter for corresponding channel.
 */
USAGEMODE HRESULT CAN_Vector_XL_GetErrorCount(SERROR_CNT& /*sErrorCnt*/, UINT /*nChannel*/, ECONTR_PARAM /*eContrParam*/)
{
    return S_OK;
}
