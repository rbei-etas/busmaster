//FILE: icsneo40DLLAPI.H

#include <windows.h>
#include "icsnVC40.h"

bool LoadDLLAPI(HINSTANCE& hAPIDLL);
void UnloadDLLAPI(HINSTANCE& hAPIDLL);


//Basic Functions
typedef int  (__stdcall* FINDNEODEVICES)(unsigned long DeviceTypes, NeoDevice* pNeoDevice, int* pNumDevices);
typedef int  (__stdcall* OPENNEODEVICE)(NeoDevice* pNeoDevice, int* hObject, unsigned char* bNetworkIDs, int bConfigRead, int bOptions);
typedef int  (__stdcall* OPENNEODEVICEBYCHANNELS)(NeoDevice* pNeoDevice, int* hObject, unsigned char* uChannels,
        int iSize, int bConfigRead, int iOptions);
typedef int  (__stdcall* CLOSEPORT)(int hObject, int* pNumberOfErrors);
typedef void (__stdcall* FREEOBJECT)(int hObject);
typedef int  (__stdcall* OPENPORTEX)(int lPortNumber, int lPortType, int lDriverType, int lIPAddressMSB, int lIPAddressLSBOrBaudRate,
                                     int bConfigRead, unsigned char* bNetworkID, int* hObject);
typedef int (_stdcall*  OPENPORT)(int lPortNumber, int lPortType, int lDriverType,unsigned char* bNetworkID,unsigned char* bSCPIDs,
                                  int* hObject);
typedef int (_stdcall*  FINDALLUSBDEVICES)(int lDriverType,
        int iGetSerialNumbers,
        int* iDevices,
        int* iSerialNumbers,
        int* iOpenedStates,
        int* iNumDevices);
typedef int (_stdcall* FINDALLCOMDEVICES) (int lDriverType,
        int lGetSerialNumbers,
        int lStopAtFirst,
        int lUSBCommOnly,
        int* p_lDeviceTypes,
        int* p_lComPorts,
        int* p_lSerialNumbers,
        int* lNumDevices);

//Message Functions
typedef int  (__stdcall* GETMESSAGES)(int hObject, icsSpyMessage* pMsg, int* pNumberOfMessages, int* pNumberOfErrors);
typedef int  (__stdcall* TXMESSAGES)(int hObject, icsSpyMessage* pMsg, int lNetworkID, int lNumMessages);
typedef int  (__stdcall* WAITFORRXMSGS)(int hObject, unsigned int iTimeOut);
typedef int  (__stdcall* ENABLERXQUEUE)(int hObject, int iEnable);
typedef int  (__stdcall* GETTSFORMSG)(int hObject, icsSpyMessage* pMsg, double* pTimeStamp);
typedef void (__stdcall* GETISO15765STATUS)(int hObject, int lNetwork, int lClearTxStatus,
        int lClearRxStatus, int* lTxStatus, int* lRxStatus);
typedef void (__stdcall* SETISO15765RXPARMS)(int hObject, int lNetwork, int lEnable,
        spyFilterLong* pFF_CFMsgFilter, icsSpyMessage* pTxMsg,
        int lCFTimeOutMs, int lFlowCBlockSize,
        int lUsesExtendedAddressing, int lUseHardwareIfPresent);
typedef void (__stdcall* GETPERFTIMER)(int hObject);
typedef int  (__stdcall* GETHWFIRMWAREINFO)( int hObject, stAPIFirmwareInfo* pInfo );
typedef void (__stdcall* GETCANCONTROLLERCLKFREQ)(int hObject,int lNetworkID);

//Device Functions
typedef int (__stdcall* GETCONFIG)(int hObject, unsigned char* pData, int* lNumBytes);
typedef int (__stdcall* SENDCONFIG)(int hObject, unsigned char* pData, int lNumBytes);
typedef int (__stdcall* GETFIRESETTINGS)(int hObject, SFireSettings* pSettings, int iNumBytes);
typedef int (__stdcall* SETFIRESETTINGS)(int hObject, SFireSettings* pSettings, int iNumBytes, int bSaveToEEPROM);
typedef int (__stdcall* GETVCAN3SETTINGS)(int hObject, SVCAN3Settings* pSettings, int iNumBytes);
typedef int (__stdcall* SETVCAN3SETTINGS)(int hObject, SVCAN3Settings* pSettings, int iNumBytes, int bSaveToEEPROM);
typedef int (__stdcall* GETYELLOWSETTINGS)(int hObject, SYellowSettings* pSettings, int iNumBytes);
typedef int (__stdcall* SETYELLOWSETTINGS)(int hObject, SYellowSettings* pSettings, int iNumBytes, int bSaveToEEPROM);
typedef int (__stdcall* SETBITRATE)(int hObject, int BitRate, int NetworkID);
typedef int (__stdcall* GETDEVICEPARMS)(int hObject, char* pParameter, char* pValues, short ValuesLength);
typedef int (__stdcall* SETDEVICEPARMS)(int hObject, char* pParmValue, int* pErrorIndex, int bSaveToEEPROM);
typedef int (__stdcall* ENABLENETWORKCOM)(int hObject,int lEnable);
typedef int (__stdcall* ENABLENETWORKCOMEX)((int hObject, int iEnable, int iNetId);
        typedef int (__stdcall* GETPERFORMANCEPARAMETERS)(int hObject, int* iBufferCount, int* iBufferMax,
                int* iOverFlowCount, int*   iReserved1,
                int*   iReserved2,int*   iReserved3,
                int*   iReserved4,int*   iReserved5);
        typedef int (__stdcall* GETOPENPORTS)(unsigned char* pBuf, unsigned long ulSize);
        typedef int (__stdcall* FORCEFWUPDATE)( int hObject );
        typedef int (__stdcall* GETDLLFIRMWARE)( int hObject, stAPIFirmwareInfo* pInfo );
        typedef int (__stdcall* OPENDEODEVICEBYSERIALNUMBER)(int SerialNumber,
                int* hObject,
                int iOpenFastDeviceType,
                int bConfigRead,
                int bSyncToPC);
        typedef int (__stdcall* SETUSBLATENCY)(NeoDevice* pNeoDevice, unsigned long LowLatency);
        typedef int (__stdcall* SETCANPARAMETERS)(int hObject, unsigned int baudRatePrescaler,
                unsigned int propagationTqs,
                unsigned int phase1Tqs,
                unsigned int phase2Tqs,
                unsigned int syncJumpWidthTqs,
                unsigned int optionBits,
                int lNetworkID);
        typedef int (__stdcall* SETCANPARAMETERSPHILIPSJA1000)(int hObject, unsigned int btr0,
                unsigned int btr1,
                unsigned int optionBits,
                int lNetworkID);
        typedef int (__stdcall* INITTXRXAPP)(int hObject,
                unsigned long timeout); //Deprecated
        typedef int (__stdcall* PERFORMTXRXPAIRS)(  int hObject,
                icsSpyMessage* tx_msgs,
                icsSpyMessage* rx_msgs,
                icsSpyMessage* rx_mask_msgs,
                unsigned long msg_cnt);
        typedef int (__stdcall* PERFORMTXRXPAIRSEX)(    int hObject,
                icsSpyMessage* tx_msgs,
                icsSpyMessage* rx_msgs,
                icsSpyMessage* rx_mask_msgs,
                unsigned long msg_cnt,
                unsigned long Timeout,
                unsigned long Delay,
                unsigned long options); //7760
        typedef int (__stdcall* CLEARAPP)(int hObject); //Deprecated
        typedef int (__stdcall* ENABLEBITSMASH)(int hObject,
                int netId,
                unsigned int arbId,
                unsigned int numWaitBits,
                unsigned int numSmashBits,
                unsigned int numFrames,
                unsigned int timeout_ms,
                unsigned int optionBits,
                unsigned int reservedZero );
        typedef int (__stdcall* SENDHWTIMEREQUEST)(int hObject);
        typedef int (__stdcall* RECEIVEHWTIMERESP)(int hObject,double* TimeHardware,unsigned long TimeOut);
        typedef int (__stdcall* LOADDEFAULTSETTINGS)(int hObject);


        //Error Functions
        typedef int (__stdcall* GETLASTAPIERROR)(int hObject, unsigned long* pErrorNumber);
        typedef int (__stdcall* GETERRMSGS)(int hObject, int* pErrorMsgs, int* pNumberOfErrors);
        typedef int (__stdcall* GETERRORINFO)(int lErrorNumber, TCHAR* szErrorDescriptionShort,
                TCHAR* szErrorDescriptionLong, int* lMaxLengthShort,
                int* lMaxLengthLong,int* lErrorSeverity,int* lRestartNeeded);

        //General Utility Functions
        typedef int (__stdcall* VALIDATEHOBJECT)(int hObject);
        typedef int (__stdcall* GETDLLVERSION)(void);
        typedef int (__stdcall* GETSERIALNUMBER)(int hObject, unsigned int* iSerialNumber);
        typedef int (__stdcall* STARTSOCKSERVER)(int hObject, int iPort);
        typedef int (__stdcall* STOPSOCKSERVER)(int hObject);

        //CoreMini Script functions
        typedef int  (__stdcall* SCRIPTSTART)(int hObject, int iLocation);
        typedef int  (__stdcall* SCRIPTSTOP)(int hObject);
        typedef int  (__stdcall* SCRIPTLOAD)(int hObject, const unsigned char* bin, unsigned long len_bytes, int iLocation);
        typedef int  (__stdcall* SCRIPTCLEAR)(int hObject, int iLocation);
        typedef int  (__stdcall* SCRIPTSTARTFBLOCK)(int hObject,unsigned int fb_index);
        typedef int  (__stdcall* SCRIPTGETFBLOCKSTATUS)(int hObject, unsigned int fb_index, int* piRunStatus);
        typedef int  (__stdcall* SCRIPTSTOPFBLOCK)(int hObject,unsigned int fb_index);
        typedef int  (__stdcall* SCRIPTGETSCRIPTSTATUS)(int hObject, int* piStatus);
        typedef int  (__stdcall* SCRIPTREADAPPSIGNAL)(int hObject, unsigned int iIndex, double* dValue);
        typedef int  (__stdcall* SCRIPTWRITEAPPSIGNAL)(int hObject, unsigned int iIndex, double dValue);
        typedef int  (__stdcall* SCRIPTREADRXMESSAGE)(int hObject, unsigned int iIndex,
                icsSpyMessage* pRxMessageMask, icsSpyMessage* pRxMessageValue);
        typedef int  (__stdcall* SCRIPTREADTXMESSAGE)(int hObject, unsigned int iIndex, icsSpyMessage* pTxMessage);
        typedef int  (__stdcall* SCRIPTWRITERXMESSAGE)(int hObject, unsigned int iIndex,
                const icsSpyMessage* pRxMessageMask, const icsSpyMessage* pRxMessageValue);
        typedef int  (__stdcall* SCRIPTWRITETXMESSAGE)(int hObject, unsigned int iIndex, const icsSpyMessage* pTxMessage);
        typedef int  (__stdcall* SCRIPTREADISO15765TXMESSAGE)(int hObject, unsigned int iIndex, stCM_ISO157652_TxMessage* pTxMessage);
        typedef int  (__stdcall* SCRIPTWRITEISO15765TXMESSAGE)(int hObject, unsigned int iIndex, const stCM_ISO157652_TxMessage* pTxMessage);


        //Deprecated (but still suppored in the DLL)
        typedef int  (__stdcall* OPENPORTEX)(int lPortSerialNumber, int lPortType, int lDriverType,
                int lIPAddressMSB, int lIPAddressLSBOrBaudRate,int bConfigRead,
                unsigned char* bNetworkID, int* hObject);
        typedef int  (__stdcall* OPENPORT)(int lPortNumber, int lPortType, int lDriverType,
                unsigned char* bNetworkID, unsigned char* bSCPIDs,  int* hObject);
        typedef int (__stdcall* ENABLENETWORKCOM)(int hObject, int Enable);
        typedef int (__stdcall* FINDCOMDEVICES)(int lDriverType,  int lGetSerialNumbers, int lStopAtFirst, int lUSBCommOnly,
                int* p_lDeviceTypes, int* p_lComPorts, int* p_lSerialNumbers, int* lNumDevices);

        //HW Support Functions
        typedef int (__stdcall* ISO15765HWSUPPORT)(int hObject, int iNetIDCAN, int iFilter, int iFilterMask,
                int iFilterXtdCANID, int iFilterXtdAddress,
                icsSpyMessage* p_stFlowControl, int iFlowControlXtdAddress,int iEnableFCTransmit);

        typedef int (__stdcall* ISO15765HWSUPPORTEX)(int hObject, int iNetIDCAN, int iFilter, int iFilterMask,
                int iFilterXtdCANID, int iFilterXtdAddress,
                icsSpyMessage* p_stFlowControl, int iFlowControlXtdAddress,int iEnableFCTransmit,
                unsigned char CCPLength);

        typedef int (__stdcall* J2534CMD)(int hObject, unsigned char* CmdBuf, short Len, void* pVoid);
        //typedef int (__stdcall *SENDRAWCMDS)(int hObject, icsneoVICommand * pCmdMsgs, int lNumOfCmds);
        typedef int (__stdcall* ENABLEBUSVOLTMON)(int hObject,unsigned int enable, unsigned int reserved);
        typedef int (__stdcall* ISO15765_DISABLENETWORKS)(int hObject);
        typedef int (__stdcall* ENABLECMUSBFILTER)(int hObject,unsigned int enable, unsigned int reserved);
        typedef int (__stdcall* ISO15765_TXMESSAGE)(int hObject, unsigned long ulNetworkID, stCM_ISO157652_TxMessage* pMsg, unsigned long ulBlockingTimeout);
        typedef int (__stdcall* GETBUSVOLTAGE)(int hObject,unsigned long* pVBusVoltage, unsigned int reserved);
        typedef int (__stdcall* SCRIPTREADISO15765_2_RX_MSG)(int hObject, unsigned int iIndex, stCM_ISO157652_RxMessage* pRxMessage);
        typedef int (__stdcall* ISO15765_2_ENABLENETWORKS)(int hObject, unsigned long ulNetworks);
        typedef int (__stdcall* SCRIPTWRITE_ISO15765_2_RX_MSG)(int hObject, unsigned int iIndex, const stCM_ISO157652_RxMessage* pRxMessage);

        typedef int (__stdcall* FINDREMOTEDEVICES)(char* pIPAddress, NeoDevice* pNeoDevice, int* pNumDevices);
        typedef int (__stdcall* OPENREMOTEDEVICE)(char* pIPAddress, NeoDevice* pNeoDevice,  int* hObject, unsigned char* bNetworkIDs, int iOptions);

        typedef int (__stdcall* LOCKCHANNELS)(int hObject, unsigned char* uChannels, int iSize);
        typedef int (__stdcall* UNLOCKCHANNELS)(int hObject, unsigned char* uChannels, int iSize);
        typedef int (__stdcall* GETCHANNELLOCKSTATUS)(int hObject, unsigned char* uChannels, int iSize);

        //Basic Functions
        extern FINDNEODEVICES icsneoFindNeoDevices;
        extern OPENNEODEVICE icsneoOpenNeoDevice;
        extern OPENNEODEVICEBYCHANNELS icsneoOpenNeoDeviceByChannels;
        extern CLOSEPORT icsneoClosePort;
        extern FREEOBJECT icsneoFreeObject;
        extern OPENPORTEX icsneoOpenPortEx;
        extern OPENPORT   icsneoOpenPort;
        extern FINDALLUSBDEVICES icsneoFindAllUSBDevices;
        extern FINDALLCOMDEVICES icsneoFindAllCOMDevices;

        //Message Functions
        extern GETMESSAGES icsneoGetMessages;
        extern TXMESSAGES icsneoTxMessages;
        extern WAITFORRXMSGS icsneoWaitForRxMessagesWithTimeOut;
        extern GETTSFORMSG icsneoGetTimeStampForMsg;
        extern ENABLERXQUEUE icsneoEnableNetworkRXQueue;
        extern GETISO15765STATUS icsneoGetISO15765Status;
        extern SETISO15765RXPARMS icsneoSetISO15765RxParameters;
        extern GETOPENPORTS icsneoGetOpenPorts;
        extern GETPERFTIMER icsneoGetPerformanceTimer;
        extern GETDLLFIRMWARE icsneoGetDLLFirmwareInfo;
        extern GETHWFIRMWAREINFO icsneoGetHWFirmwareInfo;
        extern GETCANCONTROLLERCLKFREQ icsneoGetCANControllerClockFrequency;

        //Device Functions
        extern GETCONFIG icsneoGetConfiguration;
        extern SENDCONFIG icsneoSendConfiguration;
        extern GETFIRESETTINGS icsneoGetFireSettings;
        extern SETFIRESETTINGS icsneoSetFireSettings;
        extern GETVCAN3SETTINGS icsneoGetVCAN3Settings;
        extern SETVCAN3SETTINGS icsneoSetVCAN3Settings;
        extern GETYELLOWSETTINGS icsneoGetYellowSettings;
        extern SETYELLOWSETTINGS icsneoSetYellowSettings;
        extern SETBITRATE icsneoSetBitRate;
        extern GETDEVICEPARMS icsneoGetDeviceParameters;
        extern SETDEVICEPARMS icsneoSetDeviceParameters;
        extern ENABLENETWORKCOM icsneoEnableNetworkCom;
        extern ENABLENETWORKCOMEX icsneoEnableNetworkComEx;
        extern GETPERFORMANCEPARAMETERS icsneoGetPerformanceParameters;
        extern FORCEFWUPDATE icsneoForceFirmwareUpdate;
        extern OPENDEODEVICEBYSERIALNUMBER icsneoOpenNeoDeviceBySerialNumber;
        extern SETUSBLATENCY icsneoSetUSBLatency;
        extern SETCANPARAMETERS icsneoSetCANParameters;
        extern SETCANPARAMETERSPHILIPSJA1000 icsneoSetCANParametersPhilipsSJA1000;
        extern INITTXRXAPP icsneoInitTxRxApp;
        extern PERFORMTXRXPAIRS icsneoPerformTxRxPairs;
        extern CLEARAPP icsneoClearApp;
        extern ENABLEBITSMASH icsneoEnableBitSmash;
        extern SENDHWTIMEREQUEST icsneoSendHWTimeRequest;
        extern RECEIVEHWTIMERESP icsneoReceiveHWTimeResponse;
        extern LOADDEFAULTSETTINGS icsneoLoadDefaultSettings;

        //Error Functions
        extern GETLASTAPIERROR icsneoGetLastAPIError;
        extern GETERRMSGS icsneoGetErrorMessages;
        extern GETERRORINFO icsneoGetErrorInfo;

        //General Utility Functions
        extern VALIDATEHOBJECT icsneoValidateHObject;
        extern GETDLLVERSION icsneoGetDLLVersion;
        extern GETSERIALNUMBER icsneoGetSerialNumber;
        extern STARTSOCKSERVER icsneoStartSockServer;
        extern STOPSOCKSERVER icsneoStopSockServer;

        //CoreMini Functions
        extern SCRIPTSTART icsneoScriptStart;
        extern SCRIPTSTOP icsneoScriptStop;
        extern SCRIPTLOAD icsneoScriptLoad;
        extern SCRIPTCLEAR icsneoScriptClear;
        extern SCRIPTSTARTFBLOCK icsneoScriptStartFBlock;
        extern SCRIPTGETFBLOCKSTATUS icsneoScriptGetFBlockStatus;
        extern SCRIPTSTOPFBLOCK icsneoScriptStopFBlock;
        extern SCRIPTGETSCRIPTSTATUS icsneoScriptGetScriptStatus;
        extern SCRIPTREADAPPSIGNAL icsneoScriptReadAppSignal;
        extern SCRIPTWRITEAPPSIGNAL icsneoScriptWriteAppSignal;
        extern SCRIPTREADRXMESSAGE icsneoScriptReadRxMessage;
        extern SCRIPTREADTXMESSAGE icsneoScriptReadTxMessage;
        extern SCRIPTWRITERXMESSAGE icsneoScriptWriteRxMessage;
        extern SCRIPTWRITETXMESSAGE icsneoScriptWriteTxMessage;
        extern SCRIPTREADISO15765TXMESSAGE icsneoScriptReadISO15765_2_TxMessage;
        extern SCRIPTWRITEISO15765TXMESSAGE icsneoScriptWriteISO15765_2_TxMessage;

        //HW Support Functions
        extern ISO15765HWSUPPORT icsneoSetupISO15765HwSupport;
        extern ISO15765HWSUPPORTEX icsneoSetupISO15765HwSupportEx;

        extern J2534CMD icsneoJ2534Cmd;
        extern ENABLEBUSVOLTMON icsneoEnableBusVoltageMonitor;
        extern ISO15765_DISABLENETWORKS icsneoISO15765_DisableNetworks;
        extern ENABLECMUSBFILTER icsneoEnableCoreMiniUsbFilter;
        extern ISO15765_TXMESSAGE icsneoISO15765_TransmitMessage;
        extern GETBUSVOLTAGE icsneoGetBusVoltage;
        extern SCRIPTREADISO15765_2_RX_MSG icsneoScriptReadISO15765_2_RxMessage;
        extern ISO15765_2_ENABLENETWORKS icsneoISO15765_EnableNetworks;
        extern SCRIPTWRITE_ISO15765_2_RX_MSG icsneoScriptWriteISO15765_2_RxMessage;

        extern OPENREMOTEDEVICE icsneoOpenRemoteNeoDevice;
        extern FINDREMOTEDEVICES icsneoFindRemoteNeoDevices;
        extern LOCKCHANNELS icsneoLockChannels;
        extern UNLOCKCHANNELS icsneoUnlockChannels;
        extern GETCHANNELLOCKSTATUS icsneoGetChannelLockStatus;















