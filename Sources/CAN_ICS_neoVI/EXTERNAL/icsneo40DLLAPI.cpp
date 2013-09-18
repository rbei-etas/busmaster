//FILE: icsneo40DLLAPI.cpp
#include "icsneo40DLLAPI.h"
#include <stdio.h>

//Basic Functions
FINDNEODEVICES icsneoFindNeoDevices;
OPENNEODEVICE icsneoOpenNeoDevice;
OPENNEODEVICEBYCHANNELS icsneoOpenNeoDeviceByChannels;

CLOSEPORT icsneoClosePort;
FREEOBJECT icsneoFreeObject;
OPENPORTEX icsneoOpenPortEx;
OPENPORT   icsneoOpenPort; //5594
FINDALLUSBDEVICES icsneoFindAllUSBDevices; //5594
FINDALLCOMDEVICES icsneoFindAllCOMDevices; //5594

//Message Functions
GETMESSAGES icsneoGetMessages;
TXMESSAGES icsneoTxMessages;
WAITFORRXMSGS icsneoWaitForRxMessagesWithTimeOut;
GETTSFORMSG icsneoGetTimeStampForMsg;
ENABLERXQUEUE icsneoEnableNetworkRXQueue;
GETISO15765STATUS icsneoGetISO15765Status;
SETISO15765RXPARMS icsneoSetISO15765RxParameters;
GETOPENPORTS icsneoGetOpenPorts; //5594
GETPERFTIMER icsneoGetPerformanceTimer; //5594
GETDLLFIRMWARE icsneoGetDLLFirmwareInfo; //5594
GETHWFIRMWAREINFO icsneoGetHWFirmwareInfo;  //5594
GETCANCONTROLLERCLKFREQ icsneoGetCANControllerClockFrequency; //5594

//Device Functions
GETCONFIG icsneoGetConfiguration;
SENDCONFIG icsneoSendConfiguration;
GETFIRESETTINGS icsneoGetFireSettings;
SETFIRESETTINGS icsneoSetFireSettings;
GETVCAN3SETTINGS icsneoGetVCAN3Settings;
SETVCAN3SETTINGS icsneoSetVCAN3Settings;
GETYELLOWSETTINGS icsneoGetYellowSettings;
SETYELLOWSETTINGS icsneoSetYellowSettings;
SETBITRATE icsneoSetBitRate;
GETDEVICEPARMS icsneoGetDeviceParameters;
SETDEVICEPARMS icsneoSetDeviceParameters;
ENABLENETWORKCOM icsneoEnableNetworkCom; //5594
ENABLENETWORKCOMEX icsneoEnableNetworkComEx; 
GETPERFORMANCEPARAMETERS icsneoGetPerformanceParameters; //5594
FORCEFWUPDATE icsneoForceFirmwareUpdate; //5594
OPENDEODEVICEBYSERIALNUMBER icsneoOpenNeoDeviceBySerialNumber; //5594
SETUSBLATENCY icsneoSetUSBLatency; //5594
SETCANPARAMETERS icsneoSetCANParameters; //5594
SETCANPARAMETERSPHILIPSJA1000 icsneoSetCANParametersPhilipsSJA1000; //5594
INITTXRXAPP icsneoInitTxRxApp; //5594 ////Deprecated
PERFORMTXRXPAIRS icsneoPerformTxRxPairs; //5594
CLEARAPP icsneoClearApp;  //5594 ////Deprecated
ENABLEBITSMASH icsneoEnableBitSmash;//5594
SENDHWTIMEREQUEST icsneoSendHWTimeRequest;//5594
RECEIVEHWTIMERESP icsneoReceiveHWTimeResponse; //5594
LOADDEFAULTSETTINGS icsneoLoadDefaultSettings;
PERFORMTXRXPAIRSEX icsneoPerformTxRxPairsEx; //7760

//Error Functions
GETLASTAPIERROR icsneoGetLastAPIError;
GETERRMSGS icsneoGetErrorMessages;
GETERRORINFO icsneoGetErrorInfo;

//General Utility Functions
VALIDATEHOBJECT icsneoValidateHObject;
GETDLLVERSION icsneoGetDLLVersion;
GETSERIALNUMBER icsneoGetSerialNumber;
STARTSOCKSERVER icsneoStartSockServer;
STOPSOCKSERVER icsneoStopSockServer;

//CoreMini Functions
SCRIPTLOAD icsneoScriptLoad;
SCRIPTSTART icsneoScriptStart;
SCRIPTSTOP icsneoScriptStop;
SCRIPTCLEAR icsneoScriptClear;
SCRIPTSTARTFBLOCK icsneoScriptStartFBlock;
SCRIPTSTOPFBLOCK icsneoScriptStopFBlock;
SCRIPTGETFBLOCKSTATUS icsneoScriptGetFBlockStatus;
SCRIPTGETSCRIPTSTATUS icsneoScriptGetScriptStatus;
SCRIPTREADAPPSIGNAL icsneoScriptReadAppSignal;
SCRIPTWRITEAPPSIGNAL icsneoScriptWriteAppSignal;
SCRIPTREADRXMESSAGE icsneoScriptReadRxMessage;
SCRIPTREADTXMESSAGE icsneoScriptReadTxMessage;
SCRIPTWRITERXMESSAGE icsneoScriptWriteRxMessage;
SCRIPTWRITETXMESSAGE icsneoScriptWriteTxMessage;
SCRIPTREADISO15765TXMESSAGE icsneoScriptReadISO15765_2_TxMessage;
SCRIPTWRITEISO15765TXMESSAGE icsneoScriptWriteISO15765_2_TxMessage;

//HWSupport functions
ISO15765HWSUPPORT icsneoSetupISO15765HwSupport; //5594
ISO15765HWSUPPORTEX icsneoSetupISO15765HwSupportEx;//5594

J2534CMD icsneoJ2534Cmd;
//SENDRAWCMDS icsneoSendRawCmds;
ENABLEBUSVOLTMON icsneoEnableBusVoltageMonitor;
ISO15765_DISABLENETWORKS icsneoISO15765_DisableNetworks;
ENABLECMUSBFILTER icsneoEnableCoreMiniUsbFilter;
ISO15765_TXMESSAGE icsneoISO15765_TransmitMessage;
GETBUSVOLTAGE icsneoGetBusVoltage;
SCRIPTREADISO15765_2_RX_MSG icsneoScriptReadISO15765_2_RxMessage;
ISO15765_2_ENABLENETWORKS icsneoISO15765_EnableNetworks;
SCRIPTWRITE_ISO15765_2_RX_MSG icsneoScriptWriteISO15765_2_RxMessage;

OPENREMOTEDEVICE icsneoOpenRemoteNeoDevice;
FINDREMOTEDEVICES icsneoFindRemoteNeoDevices;

LOCKCHANNELS icsneoLockChannels;
UNLOCKCHANNELS icsneoUnlockChannels;
GETCHANNELLOCKSTATUS icsneoGetChannelLockStatus;

//The following are valid strings for setting parameters on devices 
//using the icsneoGetDeviceParameters() and icsneoSetDeviceParameters() functions
char *FireParameters[] =
{
	"can1", "can2", "can3", "can4", "swcan", "lsftcan", "lin1", "lin2",
	"lin3", "lin4", "cgi_baud", "cgi_tx_ifs_bit_times",
	"cgi_rx_ifs_bit_times", "cgi_chksum_enable", "network_enables", 
	"network_enabled_on_boot", "pwm_man_timeout", "pwr_man_enable", 
	"misc_io_initial_ddr", "misc_io_initial_latch", "misc_io_analog_enable", 
	"misc_io_report_period", "misc_io_on_report_events", "ain_sample_period", 
	"ain_threshold", "iso15765_separation_time_offset", "iso9141_kwp_settings", 
	"perf_en", "iso_parity", "iso_msg_termination", "network_enables_2"
};


char *VCAN3Parameters[] =
{
	"can1", "can2", "network_enables", "network_enabled_on_boot", "iso15765_separation_time_offset",
	"perf_en", "misc_io_initial_ddr", "misc_io_initial_latch", "misc_io_report_period", 
	"misc_io_on_report_events"
};

char *CANParameters[] = 
{
     "Mode", "SetBaudrate", "Baudrate", "NetworkType", "TqSeg1",
	 "TqSeg2", "TqProp", "TqSync", "BRP", "auto_baud"
};

char *SWCANParameters[] = 
{
	 "Mode", "SetBaudrate", "Baudrate", "NetworkType", "TqSeg1", "TqSeg2", 
	 "TqProp", "TqSync", "BRP", "high_speed_auto_switch", "auto_baud"
};

char *LINParameters[] = 
{
	 "Baudrate", "spbrg", "brgh", "MasterResistor", "Mode"
};

char *ISOKWPParms[] =
{
	 "Baudrate", "spbrg", "brgh", "init_steps", "init_step_count", 
	 "p2_500us", "p3_500us", "p4_500us", "chksum_enabled"
};


bool LoadDLLAPI(HINSTANCE &hAPIDLL)
{
    char SysDirName[501];
    char DLLNameWithPath[600];
    char OutMsg[700];
    STARTUPINFO          si;
    PROCESS_INFORMATION  pi;
  
    memset(&si, 0, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory( &pi, sizeof(pi) );

    GetSystemDirectory(SysDirName, 500);
    sprintf(DLLNameWithPath, "%s\\icsneo40.dll", SysDirName);

 	if((hAPIDLL = LoadLibrary(DLLNameWithPath)) == NULL)
 	{	 
		return false;
    }
 

	icsneoFindNeoDevices =    (FINDNEODEVICES) GetProcAddress(hAPIDLL,              "icsneoFindNeoDevices");
	icsneoOpenNeoDevice =     (OPENNEODEVICE) GetProcAddress(hAPIDLL,               "icsneoOpenNeoDevice");
 	icsneoOpenNeoDeviceByChannels = (OPENNEODEVICEBYCHANNELS) GetProcAddress(hAPIDLL, "icsneoOpenNeoDeviceByChannels");
 
	icsneoClosePort =         (CLOSEPORT) GetProcAddress(hAPIDLL,                   "icsneoClosePort");	
	icsneoFreeObject =        (FREEOBJECT) GetProcAddress(hAPIDLL,                  "icsneoFreeObject");
    icsneoOpenPortEx =        (OPENPORTEX) GetProcAddress(hAPIDLL,                  "icsneoOpenPortEx");
	
    icsneoTxMessages =        (TXMESSAGES) GetProcAddress(hAPIDLL,                  "icsneoTxMessages");
	icsneoGetMessages =       (GETMESSAGES) GetProcAddress(hAPIDLL,                 "icsneoGetMessages");
	icsneoWaitForRxMessagesWithTimeOut = (WAITFORRXMSGS) GetProcAddress(hAPIDLL,    "icsneoWaitForRxMessagesWithTimeOut");
    icsneoGetTimeStampForMsg = (GETTSFORMSG) GetProcAddress(hAPIDLL,                "icsneoGetTimeStampForMsg");
    icsneoEnableNetworkRXQueue = (ENABLERXQUEUE) GetProcAddress(hAPIDLL,            "icsneoEnableNetworkRXQueue");
    icsneoGetISO15765Status =  (GETISO15765STATUS) GetProcAddress(hAPIDLL,          "icsneoGetISO15765Status");
    icsneoSetISO15765RxParameters = (SETISO15765RXPARMS) GetProcAddress(hAPIDLL,    "icsneoSetISO15765RxParameters");

	icsneoGetConfiguration =  (GETCONFIG) GetProcAddress(hAPIDLL,                   "icsneoGetConfiguration");
  	icsneoSendConfiguration = (SENDCONFIG) GetProcAddress(hAPIDLL,                  "icsneoSendConfiguration");
    icsneoGetFireSettings =   (GETFIRESETTINGS) GetProcAddress(hAPIDLL,             "icsneoGetFireSettings");
	icsneoSetFireSettings =   (SETFIRESETTINGS) GetProcAddress(hAPIDLL,             "icsneoSetFireSettings");
	icsneoGetVCAN3Settings =  (GETVCAN3SETTINGS) GetProcAddress(hAPIDLL,            "icsneoGetVCAN3Settings");
    icsneoSetVCAN3Settings =  (SETVCAN3SETTINGS) GetProcAddress(hAPIDLL,            "icsneoSetVCAN3Settings");
    icsneoGetYellowSettings = (GETYELLOWSETTINGS) GetProcAddress(hAPIDLL,           "icsneoGetYellowSettings");
    icsneoSetYellowSettings = (SETYELLOWSETTINGS) GetProcAddress(hAPIDLL,           "icsneoSetYellowSettings");

    icsneoSetBitRate =        (SETBITRATE)       GetProcAddress(hAPIDLL,            "icsneoSetBitRate");
    icsneoSetBitRateEx =      (SETBITRATEEX)     GetProcAddress(hAPIDLL,            "icsneoSetBitRateEx");
	icsneoGetDeviceParameters = (GETDEVICEPARMS) GetProcAddress(hAPIDLL,            "icsneoGetDeviceParameters");
	icsneoSetDeviceParameters = (SETDEVICEPARMS) GetProcAddress(hAPIDLL,            "icsneoSetDeviceParameters");

	icsneoGetLastAPIError =   (GETLASTAPIERROR) GetProcAddress(hAPIDLL,             "icsneoGetLastAPIError");
    icsneoGetErrorMessages = (GETERRMSGS) GetProcAddress(hAPIDLL,                   "icsneoGetErrorMessages");
    icsneoGetErrorInfo =     (GETERRORINFO) GetProcAddress(hAPIDLL,                 "icsneoGetErrorInfo");

	icsneoScriptLoad =        (SCRIPTLOAD) GetProcAddress(hAPIDLL,                  "icsneoScriptLoad");
	icsneoScriptStart =       (SCRIPTSTART) GetProcAddress(hAPIDLL,                 "icsneoScriptStart");
	icsneoScriptStop =        (SCRIPTSTOP) GetProcAddress(hAPIDLL,                  "icsneoScriptStop");
	icsneoScriptClear =       (SCRIPTCLEAR) GetProcAddress(hAPIDLL,                 "icsneoScriptClear");
	icsneoScriptStartFBlock = (SCRIPTSTARTFBLOCK) GetProcAddress(hAPIDLL,           "icsneoScriptStartFBlock");
	icsneoScriptStopFBlock =  (SCRIPTSTOPFBLOCK) GetProcAddress(hAPIDLL,            "icsneoScriptStopFBlock");
	icsneoScriptGetFBlockStatus = (SCRIPTGETFBLOCKSTATUS) GetProcAddress(hAPIDLL,   "icsneoScriptGetFBlockStatus");
	icsneoScriptGetScriptStatus = (SCRIPTGETSCRIPTSTATUS) GetProcAddress(hAPIDLL, "icsneoScriptGetScriptStatus");
	icsneoScriptReadAppSignal = (SCRIPTREADAPPSIGNAL) GetProcAddress(hAPIDLL,       "icsneoScriptReadAppSignal");
    icsneoScriptWriteAppSignal = (SCRIPTWRITEAPPSIGNAL) GetProcAddress(hAPIDLL,     "icsneoScriptWriteAppSignal");

	icsneoScriptReadRxMessage = (SCRIPTREADRXMESSAGE) GetProcAddress(hAPIDLL,       "icsneoScriptReadRxMessage");
	icsneoScriptReadTxMessage = (SCRIPTREADTXMESSAGE) GetProcAddress(hAPIDLL,       "icsneoScriptReadTxMessage");
    icsneoScriptWriteRxMessage = (SCRIPTWRITERXMESSAGE)GetProcAddress(hAPIDLL,      "icsneoScriptWriteRxMessage");
	icsneoScriptWriteTxMessage = (SCRIPTWRITETXMESSAGE) GetProcAddress(hAPIDLL,     "icsneoScriptWriteTxMessage");
 	icsneoScriptReadISO15765_2_TxMessage = (SCRIPTREADISO15765TXMESSAGE) GetProcAddress(hAPIDLL, "icsneoScriptReadISO15765_2_TxMessage");
	icsneoScriptWriteISO15765_2_TxMessage = (SCRIPTWRITEISO15765TXMESSAGE) GetProcAddress(hAPIDLL, "icsneoScriptWriteISO15765_2_TxMessage");
	icsneoOpenPort 						  = (OPENPORT) GetProcAddress(hAPIDLL,"icsneoOpenPort");
	icsneoEnableNetworkCom 				  = (ENABLENETWORKCOM) GetProcAddress(hAPIDLL,"icsneoEnableNetworkCom");
	icsneoEnableNetworkComEx			  = (ENABLENETWORKCOMEX) GetProcAddress(hAPIDLL,"icsneoEnableNetworkComEx");
	icsneoFindAllUSBDevices 			  = (FINDALLUSBDEVICES) GetProcAddress(hAPIDLL,"icsneoFindAllUSBDevices");
	icsneoFindAllCOMDevices 			  = (FINDALLCOMDEVICES) GetProcAddress(hAPIDLL,"icsneoFindAllCOMDevices"); 
	icsneoSetupISO15765HwSupport		  = (ISO15765HWSUPPORT) GetProcAddress(hAPIDLL,"icsneoSetupISO15765HwSupport"); 
	icsneoSetupISO15765HwSupportEx 		  = (ISO15765HWSUPPORTEX) GetProcAddress(hAPIDLL,"icsneoSetupISO15765HwSupportEx"); 
	icsneoGetPerformanceParameters 		  = (GETPERFORMANCEPARAMETERS) GetProcAddress(hAPIDLL,"icsneoGetPerformanceParameters"); 
	icsneoGetOpenPorts 		             = (GETOPENPORTS) GetProcAddress(hAPIDLL,"icsneoGetOpenPorts"); 
	icsneoGetPerformanceTimer 		     = (GETPERFTIMER) GetProcAddress(hAPIDLL,"icsneoGetPerformanceTimer");
	icsneoForceFirmwareUpdate  		     = (FORCEFWUPDATE) GetProcAddress(hAPIDLL,"icsneoForceFirmwareUpdate");
	icsneoGetHWFirmwareInfo  		     = (GETHWFIRMWAREINFO) GetProcAddress(hAPIDLL,"icsneoGetHWFirmwareInfo");
	icsneoOpenNeoDeviceBySerialNumber    = (OPENDEODEVICEBYSERIALNUMBER) GetProcAddress(hAPIDLL,"icsneoOpenNeoDeviceBySerialNumber");
	icsneoSetUSBLatency  		         = (SETUSBLATENCY) GetProcAddress(hAPIDLL,"icsneoSetUSBLatency");
	icsneoGetCANControllerClockFrequency = (GETCANCONTROLLERCLKFREQ) GetProcAddress(hAPIDLL,"icsneoGetCANControllerClockFrequency");
	icsneoSetCANParameters  		     = (SETCANPARAMETERS) GetProcAddress(hAPIDLL,"icsneoSetCANParameters");
	icsneoSetCANParametersPhilipsSJA1000 = (SETCANPARAMETERSPHILIPSJA1000) GetProcAddress(hAPIDLL,"icsneoSetCANParametersPhilipsSJA1000");
	icsneoInitTxRxApp  		          = (INITTXRXAPP) GetProcAddress(hAPIDLL,"icsneoInitTxRxApp");
	icsneoPerformTxRxPairs  		  = (PERFORMTXRXPAIRS) GetProcAddress(hAPIDLL,"icsneoPerformTxRxPairs");
	icsneoClearApp  		          = (CLEARAPP) GetProcAddress(hAPIDLL,"icsneoClearApp");
	icsneoEnableBitSmash  		      = (ENABLEBITSMASH) GetProcAddress(hAPIDLL,"icsneoEnableBitSmash");
	icsneoSendHWTimeRequest  		  = (SENDHWTIMEREQUEST) GetProcAddress(hAPIDLL,"icsneoSendHWTimeRequest");
	icsneoReceiveHWTimeResponse       = (RECEIVEHWTIMERESP) GetProcAddress(hAPIDLL,"icsneoReceiveHWTimeResponse");
	icsneoGetDLLFirmwareInfo  		  = (GETDLLFIRMWARE)  GetProcAddress(hAPIDLL,"icsneoGetDLLFirmwareInfo");
	icsneoJ2534Cmd  		          = (J2534CMD) GetProcAddress(hAPIDLL, "icsneoJ2534Cmd");
	//icsneoSendRawCmds  		          = (SENDRAWCMDS) GetProcAddress(hAPIDLL, "icsneoSendRawCmds");
	icsneoEnableBusVoltageMonitor     = (ENABLEBUSVOLTMON) GetProcAddress(hAPIDLL, "icsneoEnableBusVoltageMonitor");
	icsneoISO15765_DisableNetworks    = (ISO15765_DISABLENETWORKS) GetProcAddress(hAPIDLL, "icsneoISO15765_DisableNetworks");
	icsneoEnableCoreMiniUsbFilter  	  = (ENABLECMUSBFILTER) GetProcAddress(hAPIDLL, "icsneoEnableCoreMiniUsbFilter");	
	icsneoISO15765_TransmitMessage    = (ISO15765_TXMESSAGE) GetProcAddress(hAPIDLL, "icsneoISO15765_TransmitMessage");
	icsneoGetBusVoltage 		      = (GETBUSVOLTAGE) GetProcAddress(hAPIDLL, "icsneoGetBusVoltage");
    icsneoScriptReadISO15765_2_RxMessage = (SCRIPTREADISO15765_2_RX_MSG) GetProcAddress(hAPIDLL, "icsneoScriptReadISO15765_2_RxMessage");
	icsneoISO15765_EnableNetworks = (ISO15765_2_ENABLENETWORKS) GetProcAddress(hAPIDLL, "icsneoISO15765_EnableNetworks");	
	//icsneoScriptWriteISO15765_2_RxMessage = (SCRIPTWRITE_ISO15765_2_RX_MSG) GetProcAddress(hAPIDLL, "icsneoScriptWriteISO15765_2_RxMessage");
	icsneoOpenRemoteNeoDevice = (OPENREMOTEDEVICE) GetProcAddress(hAPIDLL, "icsneoOpenRemoteNeoDevice");	
    icsneoFindRemoteNeoDevices = (FINDREMOTEDEVICES) GetProcAddress(hAPIDLL, "icsneoFindRemoteNeoDevices");
	icsneoLockChannels  		       = (LOCKCHANNELS) GetProcAddress(hAPIDLL, "icsneoLockChannels");
	icsneoUnlockChannels  		       = (UNLOCKCHANNELS) GetProcAddress(hAPIDLL, "icsneoUnlockChannels");
	icsneoGetChannelLockStatus = (GETCHANNELLOCKSTATUS) GetProcAddress(hAPIDLL, "icsneoGetChannelLockStatus");


	//  		          = () GetProcAddress(hAPIDLL, "");

    //int xyz = 0;


    if(!icsneoFindNeoDevices || !icsneoOpenNeoDevice || !icsneoOpenNeoDeviceByChannels || !icsneoClosePort || !icsneoFreeObject ||
	   !icsneoOpenPortEx || !icsneoTxMessages || !icsneoGetMessages || !icsneoWaitForRxMessagesWithTimeOut ||
       !icsneoGetTimeStampForMsg || !icsneoEnableNetworkRXQueue || !icsneoGetISO15765Status ||
       !icsneoSetISO15765RxParameters || !icsneoGetConfiguration || !icsneoSendConfiguration ||
       !icsneoGetFireSettings || !icsneoSetFireSettings || !icsneoGetVCAN3Settings ||
       !icsneoGetYellowSettings || !icsneoSetYellowSettings ||
       !icsneoSetVCAN3Settings || !icsneoSetBitRate || !icsneoGetDeviceParameters ||
       !icsneoSetDeviceParameters || !icsneoGetLastAPIError || !icsneoGetErrorMessages ||
       !icsneoGetErrorInfo || !icsneoScriptLoad || !icsneoScriptStart || !icsneoScriptStop ||
       !icsneoScriptClear || !icsneoScriptStartFBlock || !icsneoScriptStopFBlock ||
       !icsneoScriptGetFBlockStatus || !icsneoScriptGetScriptStatus || !icsneoScriptReadAppSignal ||
       !icsneoScriptWriteAppSignal || !icsneoScriptReadRxMessage || !icsneoScriptReadTxMessage ||
       !icsneoScriptWriteRxMessage || !icsneoScriptWriteTxMessage || !icsneoScriptReadISO15765_2_TxMessage ||
       !icsneoScriptWriteISO15765_2_TxMessage || !icsneoOpenPort || !icsneoEnableNetworkCom || !icsneoFindAllUSBDevices || !icsneoFindAllCOMDevices || !icsneoSetupISO15765HwSupport || 
	   !icsneoSetupISO15765HwSupportEx || !icsneoGetPerformanceParameters || !icsneoGetOpenPorts || !icsneoGetPerformanceTimer || !icsneoGetHWFirmwareInfo || 
	   !icsneoOpenNeoDeviceBySerialNumber || /*!icsneoSetUSBLatency || */ !icsneoGetCANControllerClockFrequency || !icsneoSetCANParameters || 
	   !icsneoSetCANParametersPhilipsSJA1000 || !icsneoInitTxRxApp || !icsneoPerformTxRxPairs || !icsneoClearApp || !icsneoEnableBitSmash || /*!icsneoSendHWTimeRequest ||*/
	   /*!icsneoReceiveHWTimeResponse || */ !icsneoGetDLLFirmwareInfo || !icsneoJ2534Cmd || !icsneoEnableBusVoltageMonitor ||
	   !icsneoISO15765_DisableNetworks || !icsneoEnableCoreMiniUsbFilter || !icsneoISO15765_TransmitMessage || !icsneoGetBusVoltage ||
	   !icsneoScriptReadISO15765_2_RxMessage || !icsneoISO15765_EnableNetworks || //!icsneoScriptWriteISO15765_2_RxMessage ||
	   !icsneoOpenRemoteNeoDevice || !icsneoFindRemoteNeoDevices || !icsneoGetChannelLockStatus || 	!icsneoLockChannels || !icsneoUnlockChannels || !icsneoEnableNetworkComEx)
	{
		FreeLibrary(hAPIDLL);
		
       // g_Logger.LogMessage("LoadDLLAPI()", CLogging::LOG_DETAIL, "An exported function was missing from the icsneo API"); 

		return false;
	}
	
	//FROG TRM - Need to log the version number of the icsneo API DLL by calling icsneoGetDLLVersionEx,
    //           but that is risky right now because the call will crash on some European (German) computers
	
    return true;
}	


void UnloadDLLAPI(HINSTANCE &hAPIDLL)
{
	if(hAPIDLL)
		FreeLibrary(hAPIDLL);
}