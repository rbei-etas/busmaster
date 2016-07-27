// CAN_SAINT.cpp : Defines the initialization routines for the DLL.
//

#include "CAN_SAINT_stdafx.h"

#include <sstream>
#include <string>
#include <vector>

#include "CAN_SAINT.h"
//#include "include/Error.h"
#include "include/basedefs.h"
#include "DataTypes/Base_WrapperErrorLogger.h"
#include "CAN_Error_Defs.h"
//#include "Utility/Utility_Thread.h"
#include "BaseDIL_CAN_Controller.h"
#include "ChangeRegisters.h"
#include "../Application/MultiLanguage.h"
#include "Utility/MultiLanguageSupport.h"
#include "DILPluginHelperDefs.h"
#include "HardwareListing.h" 
#include "FaultCodes.h"
#include "Filter.h"
#include "DeviceSelect.h"
#include <sys/timeb.h>
#include "CANConfig.h"
#include "BaseDIL_CAN.h"
#include "CANDriverDefines.h"

#include "../Application/MultiLanguage.h"
#include "Utility\MultiLanguageSupport.h"

#define USAGE_EXPORT
#include "CAN_SAINT_Extern.h"

#define STATE_OFF 0
#define STATE_INITIAL 1
#define STATE_SECOND 2
#define STATE_THIRD 3

#define defUSB_MODE_ACTIVE                     1
#define defUSB_MODE_PASSIVE                    2
#define defUSB_MODE_SIMULATE                   3


#define FAULT_CODE_WAIT 2500
#define FAULT_CODE_BUFF_WAIT 200
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//
//TODO: If this DLL is dynamically linked against the MFC DLLs,
//		any functions exported from this DLL which call into
//		MFC must have the AFX_MANAGE_STATE macro added at the
//		very beginning of the function.
//
//		For example:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// normal function body here
//		}
//
//		It is very important that this macro appear in each
//		function, prior to any calls into MFC.  This means that
//		it must appear as the first statement within the 
//		function, even before any object variable declarations
//		as their constructors may generate calls into the MFC
//		DLL.
//
//		Please see MFC Technical Notes 33 and 58 for additional
//		details.
//

// CCAN_SAINTApp

BEGIN_MESSAGE_MAP(CCAN_SAINTApp, CWinApp)
END_MESSAGE_MAP()


// CCAN_SAINTApp construction
void EnableMinimizeButton(HWND hwnd)
{
	 SetWindowLong(hwnd, GWL_STYLE, GetWindowLong(hwnd, GWL_STYLE) | WS_MINIMIZEBOX);
}

CCAN_SAINTApp::CCAN_SAINTApp()
{

}
Message::Message(){

}
Message::Message(CString rec,CString tran){
	receiveMessage = rec;
	transmitMessage = tran;
}
Message::Message(CString dat,CString comm, int del, int en){
	data = dat;
	comment = comm;
	delay = del;
	enabled = en;
}

GroupTransmit::GroupTransmit(){

}
GroupTransmit::GroupTransmit(CString _url, CString _receive){
	url = _url;
	cur = 0;
	size = 0;
	receive = _receive;
	char lineBuffer[1000];
	int i = 0;
	CFile	myFile;
	char * curChar;

	char stringToSave[256];
	strcpy(stringToSave, (LPCTSTR)url);
	if ( myFile.Open( _T(url),CFile::modeReadWrite ) )
	{

		ULONGLONG dwLength = myFile.GetLength();
		BYTE *buffer = (BYTE *) malloc(dwLength + 1); 

		myFile.Read(buffer, dwLength); // read whole file
		*(buffer + dwLength) = '\0';   // add null

		curChar = (char*)buffer;
		int commaCount = 0;
		int nIndex = 0;
		int spaceCount = 0;
		bool old = false;
		Message temp;
		while(*curChar != '\0'){
			if(*curChar == '\n'){
				lineBuffer[i] = '\0';
				i = 0;
				curChar++;
				CString line(lineBuffer);
				line.Remove('\r');
				temp.comment = line;
				messages.push_back(temp);
				spaceCount = 0;
				old = false;
				size++;
			}else if(*curChar == ' ' && old == false){
				lineBuffer[i] = '\0';
				i = 0;
				curChar++;
				spaceCount++;
				CString line(lineBuffer);

				if(spaceCount == 1){	
					line.Remove(' ');
					if(line.Compare("Enabled") == 0 || line.Compare("Disabled") == 0){
						if(line.Compare("Enabled") == 0){
							temp.enabled = true;
						}else{
							temp.enabled = false;
						}
					}else{
						old = true;
						temp.enabled = true;
						temp.delay = atoi(line);
					}
				}else if(spaceCount == 2){
					temp.delay = atoi(line);
					old = true;
				}
			}else if(*curChar == '"'){
				lineBuffer[i] = '\0';
				i = 0;
				curChar++;
				CString d(lineBuffer);
				d.Remove('\t');
				temp.data = d;
			}else{
				lineBuffer[i] = *curChar;
				i++;
				curChar++;
			}
		}
	}else{
		AfxMessageBox("File failed to open");
		return;
	}

}

bool checkValidity(CString str){
	str.Remove(' ');
	for(int i = 0; i < str.GetLength();i++){
		if(!(str.GetAt(i) >= '0' && str.GetAt(i) <= '9') && !(str.GetAt(i) >= 'A' && str.GetAt(i) <= 'F') && (str.GetAt(i) != '\0')&& !(str.GetAt(i) >= 'a' && str.GetAt(i) <= 'f')){
			return false;
		}
	}
	return true;
}
bool checkWildcardValidity(CString str){
	str.Remove(' ');
	for(int i = 0; i < str.GetLength();i++){
		if(!(str.GetAt(i) >= '0' && str.GetAt(i) <= '9') && !(str.GetAt(i) >= 'A' && str.GetAt(i) <= 'F') && (str.GetAt(i) != '\0') && (str.GetAt(i) != 'X')){
			return false;
		}
	}
	return true;
}


#define MAX_BUFF_ALLOWED 16
typedef struct tagClientBufMap
{
    DWORD dwClientID;
    BYTE hClientHandle;
    CBaseCANBufFSE* pClientBuf[MAX_BUFF_ALLOWED];
    char pacClientName[MAX_PATH];
    UINT unBufCount;
    tagClientBufMap()
    {
        dwClientID = 0;
        hClientHandle = 0;
        unBufCount = 0;
        memset(pacClientName, 0, sizeof (char) * MAX_PATH);
        for (int i = 0; i < MAX_BUFF_ALLOWED; i++)
        {
            pClientBuf[i] = nullptr;
        }

    }
} SCLIENTBUFMAP;


// The one and only CCAN_SAINTApp object

CCAN_SAINTApp theApp;

CHardwareListing *pHardware;
#define MAX_CLIENT_ALLOWED 16

static HINSTANCE ghLangInst=nullptr;
// CCAN_SAINTApp initialization

enum
{
    CREATE_MAP_TIMESTAMP = 0x1,
    CALC_TIMESTAMP_READY = 0x2,
};

static BYTE sg_byCurrState = CREATE_MAP_TIMESTAMP;


static HWND sg_hOwnerWnd = nullptr;
static SCONTROLLER_DETAILS sg_ControllerDetails[defNO_OF_CHANNELS];
int faultcodeCount = 0;

static bool sg_bIsConnected = false;
static unsigned short sg_ucControllerMode = defUSB_MODE_ACTIVE; 
static int sg_nFRAMES = 128;
const int ENTRIES_IN_GBUF       = 2000;
static STCANDATA sg_asCANMsg;
static INTERFACE_HW sg_HardwareIntr[defNO_OF_CHANNELS];

PSCONTROLLER_DETAILS detailsController;

int currentMessageState = STATE_OFF;

CSBEConnection EngineConnection; //Class to communicate with SBE2
int deviceID = -1;


static  CRITICAL_SECTION sg_CritSectForWrite;   
CString Host;
CString Port;
CString MsgBufferSize;
CString DeviceList[32]; //List of Devices seen by SBE2
static UINT sg_unClientCnt = 0;
UINT_PTR myTimer;
UINT_PTR myTimerPause;

static SCLIENTBUFMAP sg_asClientToBufMap[MAX_CLIENT_ALLOWED];

std::string sg_acErrStr = "";
CString device;
static Base_WrapperErrorLogger* sg_pIlog   = nullptr;


#define RECEIVE_EVENT_SIZE 1                // DO NOT EDIT! Currently 1 is supported only
#define RX_QUEUE_SIZE      4096             // internal driver queue size in CAN events


#define HOST_KW				"HOST="
#define HOST_DEFAULT		"localhost"
#define PORT_KW				"PORT="
#define PORT_DEFAULT		"8000"
#define MSG_BUFFER_KW		"MSG_BUFFER="
#define MSG_BUFFER_DEFAULT	"10000"






BOOL CCAN_SAINTApp::InitInstance()
{
	if ( CMultiLanguage::m_nLocales <= 0 )    // Not detected yet
    {
        CMultiLanguage::DetectLangID(); // Detect language as user locale
        CMultiLanguage::DetectUILanguage();    // Detect language in MUI OS
    }
    TCHAR szModuleFileName[MAX_PATH];        // Get Module File Name and path
    int ret = ::GetModuleFileName(theApp.m_hInstance, szModuleFileName, MAX_PATH);
    if ( ret == 0 || ret == MAX_PATH )
    {
        ASSERT(false);
    }
	ghLangInst = CMultiLanguage::LoadLangResourceDLL( szModuleFileName );
    if (ghLangInst)
    {
        AfxSetResourceHandle( ghLangInst );
    }



	CWinApp::InitInstance();

	return TRUE;
}


class CDIL_CAN_SAINT : public CBaseDIL_CAN_Controller
{
	public:
		/* STARTS IMPLEMENTATION OF THE INTERFACE FUNCTIONS... */

		HRESULT CAN_PerformInitOperations(void);
		HRESULT CAN_PerformClosureOperations(void);
		HRESULT CAN_GetTimeModeMapping(SYSTEMTIME& CurrSysTime, UINT64& TimeStamp, LARGE_INTEGER& QueryTickCount);
		HRESULT CAN_ListHwInterfaces(INTERFACE_HW_LIST& sSelHwInterface, INT& nCount);
		HRESULT CAN_SelectHwInterface(const INTERFACE_HW_LIST& sSelHwInterface, INT nCount);
		HRESULT CAN_DeselectHwInterface(void);
		HRESULT CAN_DisplayConfigDlg(PSCONTROLLER_DETAILS InitData, int& Length);
		HRESULT CAN_SetConfigData(PSCONTROLLER_DETAILS InitData, int Length);
		HRESULT CAN_StartHardware(void);
		HRESULT CAN_StopHardware(void);
		HRESULT CAN_GetCurrStatus(STATUSMSG& StatusData);
		HRESULT CAN_GetTxMsgBuffer(BYTE*& pouFlxTxMsgBuffer);
		HRESULT CAN_SendMsg(DWORD dwClientID, const STCAN_MSG& sCanTxMsg);
		HRESULT CAN_GetBusConfigInfo(BYTE* BusInfo);
		HRESULT CAN_GetLastErrorString(std::string& acErrorStr);
		HRESULT CAN_GetControllerParams(LONG& lParam, UINT nChannel, ECONTR_PARAM eContrParam);
		HRESULT CAN_SetControllerParams(int nValue, ECONTR_PARAM eContrparam);
		HRESULT CAN_GetErrorCount(SERROR_CNT& sErrorCnt, UINT nChannel, ECONTR_PARAM eContrParam);
		HRESULT CAN_SetAppParams(HWND hWndOwner);
		HRESULT CAN_ManageMsgBuf(BYTE bAction, DWORD ClientID, CBaseCANBufFSE* pBufObj);
		HRESULT CAN_RegisterClient(BOOL bRegister, DWORD& ClientID, char* pacClientName);
		HRESULT CAN_GetCntrlStatus(const HANDLE& hEvent, UINT& unCntrlStatus);
		HRESULT CAN_LoadDriverLibrary(void);
		HRESULT CAN_UnloadDriverLibrary(void);
		HRESULT CAN_SetHardwareChannel(PSCONTROLLER_DETAILS,DWORD dwDriverId,bool bIsHardwareListed, unsigned int unChannelCount);
};

CDIL_CAN_SAINT* g_pouDIL_CAN_SAINT = nullptr;

USAGEMODE HRESULT GetIDIL_CAN_Controller(void** ppvInterface)
{
    HRESULT hResult = S_OK;
    if ( nullptr == g_pouDIL_CAN_SAINT )
    {
        if ((g_pouDIL_CAN_SAINT = new CDIL_CAN_SAINT) == nullptr)
        {
            hResult = S_FALSE;
        }
    }
    *ppvInterface = (void*) g_pouDIL_CAN_SAINT;  /* Doesn't matter even if g_pouDIL_CAN_VectorXL is null */

    return hResult;
}

static UINT sg_nNoOfChannels = 0;

static char sg_omErrStr[256] = {0};

// Count variables
static UCHAR sg_ucNoOfHardware = 0;

/**
 * Starts code for the state machine
 */
enum
{
    STATE_DRIVER_SELECTED    = 0x0,
    STATE_HW_INTERFACE_LISTED,
    STATE_HW_INTERFACE_SELECTED,
    STATE_CONNECTED
};

BYTE sg_bCurrState = STATE_DRIVER_SELECTED;

static SYSTEMTIME sg_CurrSysTime;
static UINT64 sg_TimeStamp = 0;

/**
 * Query Tick Count
 */
static LARGE_INTEGER sg_QueryTickCount;
static LARGE_INTEGER sg_lnFrequency;
CString membuffer;



/**
 * Channel information
 */
struct CChannel
{
    /* To store baud rate information */
    USHORT  m_usClock;
    USHORT  m_usSampling;
    UINT    m_unBaudrate;

    /* Bit Timing */
    USHORT  m_usBaudRate; /*BTR0,BTR1*/
    BYTE    m_bCNF1;
    BYTE    m_bCNF2;
    BYTE    m_bCNF3;

    /* Acceptance Filter information for standard and extended envelopes*/
    SACC_FILTER_INFO     m_sFilter[2];

    /**
     * To store controller state
     * 0 - Active State
     * 1 - Passive State
     * 2 - Bus Off State
     * -1 - For invalid value
     */
    UCHAR m_ucControllerState;

    /**
     * Pointer to corresponding XLchannelConfig
     */
    CSBEConnection* m_ChannelInfo;

    /** Programmed warning limit of this channel */
    UCHAR m_ucWarningLimit;

    /** Tx Error counter value */
    UCHAR m_ucTxErrorCounter;

    /** Rx Error counter value */
    UCHAR m_ucRxErrorCounter;

    /** Peak Tx Error counter value */
    UCHAR m_ucPeakTxErrorCounter;

    /** Peak Rx Error counter value */
    UCHAR m_ucPeakRxErrorCounter;

    /** Tx Error Handler execution state */
    BOOL m_bTxErrorExecuted;

    /** Rx Error Handler execution state */
    BOOL m_bRxErrorExecuted;

    // Init members with default value
    CChannel()
    {
        // Baud Rate
        m_usBaudRate = defBAUD_RATE;

        // Controller state
        m_ucControllerState = defMODE_ACTIVE;

        // Pointer to corresponding XLchannelConfig
        m_ChannelInfo = nullptr;

        // Programmed warning limit of this channel
        m_ucWarningLimit = defWARNING_LIMIT_INT;

        // Tx Error counter value
        m_ucTxErrorCounter = 0;

        // Rx Error counter value
        m_ucRxErrorCounter = 0;

        // Peak Tx Error counter value
        m_ucPeakTxErrorCounter = 0;

        // Peak Rx Error counter value
        m_ucPeakRxErrorCounter = 0;

        // Tx Error Handler execution state
        m_bTxErrorExecuted = FALSE;

        // Rx Error Handler execution state
        m_bRxErrorExecuted = FALSE;
    }
};


/**
 * Array of channel objects. The size of this array is maximun number
 * of channels the application will support.
 */
static CChannel sg_aodChannels[ defNO_OF_CHANNELS ];

static INT sg_anSelectedItems[CHANNEL_ALLOWED];

BOOL Callback_DILTZM(BYTE /*Argument*/, PSCONTROLLER_DETAILS pDatStream, int /*Length*/)
{
    return (g_pouDIL_CAN_SAINT->CAN_SetConfigData(pDatStream, 0) == S_OK);
}




HRESULT CDIL_CAN_SAINT::CAN_DeselectHwInterface(void)
{

    VALIDATE_VALUE_RETURN_VAL(sg_bCurrState, STATE_HW_INTERFACE_SELECTED, ERR_IMPROPER_STATE);

    HRESULT hResult = S_OK;

    sg_bCurrState = STATE_HW_INTERFACE_LISTED;

    return hResult;
}

void CALLBACK faultcodeTimer( HWND hWnd,UINT nMsg,UINT_PTR nIDEvent,DWORD dwTime)
{
	if(faultcodeCount > 2){
		KillTimer(hWnd,myTimer);
		faultcodeCount = 0;
		CHardwareListing::faultsFailed();
		AfxMessageBox("Error: Could not receive fault codes, check configuration");
		return;
	}
	faultcodeCount++;
	KillTimer(hWnd,myTimer);
	CAN_SendFaultCodeRequest();
}
void CALLBACK faultCodeSend( HWND hWnd,UINT nMsg,UINT_PTR nIDEvent,DWORD dwTime)
{
	if(currentMessageState == STATE_INITIAL){
		CString UNLOCKmsg(pHardware->Fault_Code_Requests[1]);
		char c [100];
		memcpy(c,UNLOCKmsg,UNLOCKmsg.GetLength());
		currentMessageState = STATE_SECOND;
		EngineConnection.SendDataToDevice(deviceID,c);
		return;
	}
	if(currentMessageState == STATE_SECOND){
		CString READmsg(pHardware->Fault_Code_Requests[2]);
		char c [100];
		memcpy(c,READmsg,READmsg.GetLength());
		currentMessageState = STATE_THIRD;
		EngineConnection.SendDataToDevice(deviceID,c);
		KillTimer(hWnd,myTimerPause);
		return;
	}
}

bool CAN_SendFaultCodeRequest(){
	currentMessageState = STATE_INITIAL;
	if(sg_bCurrState != 3){
		AfxMessageBox("Connect to the device before reading fault codes.");
		return false;
	}
	myTimer = SetTimer (sg_hOwnerWnd,1, FAULT_CODE_WAIT, &faultcodeTimer); 
	myTimerPause =  SetTimer(sg_hOwnerWnd, 2, FAULT_CODE_BUFF_WAIT, &faultCodeSend); 
	CString HVWUmsg(pHardware->Fault_Code_Requests[0]);
	char c [100];
	memcpy(c,HVWUmsg,HVWUmsg.GetLength());
	EngineConnection.SendDataToDevice(deviceID,c);
	return true;
}

bool CAN_SendFaultCodeClear(){
	if(sg_bCurrState != 3){
		AfxMessageBox("Connect to the device before clearing fault codes.");
		return false;
	}
	CString HVWUmsg(pHardware->Clear_Fault_Message);
	char c [100];
	memcpy(c,HVWUmsg,HVWUmsg.GetLength());
	EngineConnection.SendDataToDevice(deviceID,c);
	return true;
}
bool CAN_SendMessage(CString string){

	string.Replace(" ","");
	if(string.Compare("connect") == 0){
		HRESULT hResult = S_OK;
		deviceID = EngineConnection.ConnectToDevice(device);
		if (hResult == S_OK){
			hResult = S_OK;
			sg_bCurrState = STATE_HW_INTERFACE_LISTED;
		}else{
			hResult = ERR_LOAD_HW_INTERFACE;
		}
		return true;
	}
	if(string.Compare("disconnect") == 0){
		HRESULT hResult = S_OK;
		EngineConnection.DisconnectFromDevice(deviceID);
		if (hResult == S_OK)
		{
			hResult = S_OK;
			sg_bCurrState = STATE_HW_INTERFACE_SELECTED;
		}
		else
		{
			hResult = ERR_LOAD_HW_INTERFACE;
		}
		return true;
	}
	if(string.Compare("test") == 0){
		if(sg_bCurrState != 3 && sg_bCurrState != STATE_HW_INTERFACE_LISTED){
			AfxMessageBox("Connect to the device before transmitting messages.");
			return false;
		}else{
			return true;
		}
	}
	if(checkValidity(string) == false){
		return false;
	}
	int i = string.GetLength();
	for(; i < 28; i++){
		string += "0";
	}
	if(sg_bCurrState != 3 && sg_bCurrState != STATE_HW_INTERFACE_LISTED ){
		AfxMessageBox("Connect to the device before transmitting messages.");
		return false;
	}else{
		char c [100];
		memcpy(c,string,string.GetLength());
		EngineConnection.SendDataToDevice(deviceID,c);
	}
	return true;
}
static int nGetNoOfConnectedHardware(void) 
{
    int nResult = 0;
    CString list = EngineConnection.RequestDeviceList();
	int num = list.GetLength();
	if(num != 0){
		nResult = 1;
	}
    return nResult;
}
static int nTestHardwareConnection(UCHAR& ucaTestResult, UINT nChannel) 
{
    int nReturn = 0;

    return nReturn;
}
void DisableMaximizeButton(HWND hwnd)
{
 SetWindowLong(hwnd, GWL_STYLE,
               GetWindowLong(hwnd, GWL_STYLE) & ~WS_MAXIMIZEBOX);
}
int ListHardwareInterfaces(HWND hParent,INTERFACE_HW* psInterfaces, int* pnSelList, int& nCount)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	CWnd* pWnd = CWnd::FromHandle(hParent);
	if(!pHardware){
		pHardware = new CHardwareListing(0, 0, 0,&theApp, pWnd);
	}
	if(nCount ==  1){//clicked on load tool
		BOOL ret = pHardware->Create(IDD_DLG_HW_LISTING,AfxGetApp()->GetMainWnd());
		pHardware->SetParent(AfxGetApp()->GetMainWnd());
		pHardware->ShowWindow(SW_SHOW);


	}else{//clicked on start driver
		CString devices;
		int pos;
		CString id;
		devices = EngineConnection.RequestDeviceList();

		DeviceSelect dlg(devices,pWnd);
		dlg.DoModal();
		device = dlg.getSelectedDevice();

		
		CCANConfig dlg2(pWnd);
		dlg2.DoModal();
	}

	// Invoking the Dialog 


	return S_OK;
}
static int nCreateMultipleHardwareNetwork(UINT unDefaultChannelCnt = 0)
{
    int nHardwareCountPrev = sg_ucNoOfHardware;
    int nHwCount = unDefaultChannelCnt;
    int nChannels = 0;
    ListHardwareInterfaces(sg_hOwnerWnd, sg_HardwareIntr, sg_anSelectedItems, nHwCount);

    return S_OK;
}



static int nInitHwNetwork(UINT unDefaultChannelCnt) 
{
    int nChannelCount = 0;
    int nResult = 0;
    nChannelCount = nGetNoOfConnectedHardware();
    if(nChannelCount == -1)
    {
        nResult = -1;
        return nResult;
    }
    sg_ucNoOfHardware = (UCHAR)nChannelCount;
    char acNo_Of_Hw[256] = {0};
    sprintf(acNo_Of_Hw, _("Number of hardwares Available: %d"), nChannelCount);
    if( nChannelCount == 0 ){
        nChannelCount = -1;
    }else{
        nResult = nCreateMultipleHardwareNetwork(unDefaultChannelCnt);
    }
    return nResult;
}


static BOOL bLoadDataFromContr(PSCONTROLLER_DETAILS pControllerDetails)
{
    BOOL bReturn = FALSE;
    if (pControllerDetails != nullptr){
        for( UINT nIndex = 0; nIndex < sg_nNoOfChannels ; nIndex++ ){
            char* pcStopStr = nullptr;
            CChannel& odChannel = sg_aodChannels[ nIndex ];
            odChannel.m_usBaudRate = static_cast <USHORT>(pControllerDetails[ nIndex ].m_nBTR0BTR1);
            odChannel.m_unBaudrate = static_cast <UINT>(
                                         _tcstol( pControllerDetails[ nIndex ].m_omStrBaudrate.c_str(),
                                                  &pcStopStr, defBASE_DEC ));
            odChannel.m_ucWarningLimit = static_cast <UCHAR>(
                                             _tcstol( pControllerDetails[ nIndex ].m_omStrWarningLimit.c_str(),
                                                     &pcStopStr, defBASE_DEC ));

            for ( int i = 0; i < defCAN_MSG_IDS ; i++ )
            {
                if ( pControllerDetails[ nIndex ].m_enmHWFilterType[i] == HW_FILTER_ACCEPT_ALL )
                {
                    odChannel.m_sFilter[i].m_ucACC_Code0 = 0;
                    odChannel.m_sFilter[i].m_ucACC_Code1 = 0;
                    odChannel.m_sFilter[i].m_ucACC_Code2 = 0;
                    odChannel.m_sFilter[i].m_ucACC_Code3 = 0;

                    odChannel.m_sFilter[i].m_ucACC_Mask0 = 0;
                    odChannel.m_sFilter[i].m_ucACC_Mask1 = 0;
                    odChannel.m_sFilter[i].m_ucACC_Mask2 = 0;
                    odChannel.m_sFilter[i].m_ucACC_Mask3 = 0;
                }
                else if( pControllerDetails[ nIndex ].m_enmHWFilterType[i] == HW_FILTER_REJECT_ALL )
                {
                    odChannel.m_sFilter[i].m_ucACC_Code0 = 0xFF;
                    odChannel.m_sFilter[i].m_ucACC_Code1 = 0xFF;
                    odChannel.m_sFilter[i].m_ucACC_Code2 = 0xFF;
                    odChannel.m_sFilter[i].m_ucACC_Code3 = 0xFF;

                    odChannel.m_sFilter[i].m_ucACC_Mask0 = 0xFF;
                    odChannel.m_sFilter[i].m_ucACC_Mask1 = 0xFF;
                    odChannel.m_sFilter[i].m_ucACC_Mask2 = 0xFF;
                    odChannel.m_sFilter[i].m_ucACC_Mask3 = 0xFF;
                    if ( i == 0 )
                    {
                        odChannel.m_sFilter[i].m_ucACC_Code1 = 0x0F;
                        odChannel.m_sFilter[i].m_ucACC_Code2 = 0;
                        odChannel.m_sFilter[i].m_ucACC_Code3 = 0;

                        odChannel.m_sFilter[i].m_ucACC_Mask1 = 0x0F;
                        odChannel.m_sFilter[i].m_ucACC_Mask2 = 0;
                        odChannel.m_sFilter[i].m_ucACC_Mask3 = 0;
                    }
                }
                else
                {
                    odChannel.m_sFilter[i].m_ucACC_Code0 = static_cast <UCHAR>(
                            _tcstol( pControllerDetails[ nIndex ].m_omStrAccCodeByte1[i].c_str(),
                                     &pcStopStr, defBASE_HEX ));

                    odChannel.m_sFilter[i].m_ucACC_Code1 = static_cast <UCHAR>(
                            _tcstol( pControllerDetails[ nIndex ].m_omStrAccCodeByte2[i].c_str(),
                                     &pcStopStr, defBASE_HEX ));

                    odChannel.m_sFilter[i].m_ucACC_Code2 = static_cast <UCHAR>(
                            _tcstol( pControllerDetails[ nIndex ].m_omStrAccCodeByte3[i].c_str(),
                                     &pcStopStr, defBASE_HEX ));

                    odChannel.m_sFilter[i].m_ucACC_Code3 = static_cast <UCHAR>(
                            _tcstol(pControllerDetails[ nIndex ].m_omStrAccCodeByte4[i].c_str(),
                                    &pcStopStr, defBASE_HEX));

                    odChannel.m_sFilter[i].m_ucACC_Mask0 = static_cast <UCHAR>(
                            _tcstol( pControllerDetails[ nIndex ].m_omStrAccMaskByte1[i].c_str(),
                                     &pcStopStr, defBASE_HEX));

                    odChannel.m_sFilter[i].m_ucACC_Mask1 = static_cast <UCHAR>(
                            _tcstol( pControllerDetails[ nIndex ].m_omStrAccMaskByte2[i].c_str(),
                                     &pcStopStr, defBASE_HEX));

                    odChannel.m_sFilter[i].m_ucACC_Mask2 = static_cast <UCHAR>(
                            _tcstol( pControllerDetails[ nIndex ].m_omStrAccMaskByte3[i].c_str(),
                                     &pcStopStr, defBASE_HEX));

                    odChannel.m_sFilter[i].m_ucACC_Mask3 = static_cast <UCHAR>(
                            _tcstol( pControllerDetails[ nIndex ].m_omStrAccMaskByte4[i].c_str(),
                                     &pcStopStr, defBASE_HEX));
                }
                odChannel.m_sFilter[i].m_ucACC_Filter_Type = (UCHAR)i ;
            }
            odChannel.m_usBaudRate = static_cast <USHORT>(
                                         pControllerDetails[ nIndex ].m_nBTR0BTR1 );
        }
        sg_ucControllerMode = pControllerDetails[ 0 ].m_ucControllerMode;

        bReturn = TRUE;
    }
    return bReturn;
}
HRESULT CDIL_CAN_SAINT::CAN_SetConfigData(PSCONTROLLER_DETAILS ConfigFile, int /* Length */) 
{
    VALIDATE_VALUE_RETURN_VAL(sg_bCurrState, STATE_HW_INTERFACE_SELECTED, ERR_IMPROPER_STATE);
    USES_CONVERSION;
	detailsController = ConfigFile;
    for (UINT nCount = 0; nCount < sg_ucNoOfHardware; nCount++)
    {
        ((PSCONTROLLER_DETAILS)ConfigFile)[nCount].m_omHardwareDesc = sg_ControllerDetails[nCount].m_omHardwareDesc;
    }
    for (UINT i = 0; i < defNO_OF_CHANNELS; i++)
    {
        sg_ControllerDetails[i] = ConfigFile[i];
    }
    bLoadDataFromContr(sg_ControllerDetails);

    return S_OK;
}

HRESULT CDIL_CAN_SAINT::CAN_DisplayConfigDlg(PSCONTROLLER_DETAILS InitData, int& Length)
{
  
    return S_OK;
}
HRESULT CDIL_CAN_SAINT::CAN_GetCntrlStatus(const HANDLE& /*hEvent*/, UINT& /*unCntrlStatus*/)
{
    return S_OK;
}
HRESULT CDIL_CAN_SAINT::CAN_GetControllerParams(LONG& lParam, UINT nChannel, ECONTR_PARAM eContrParam)
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
                if (nGetNoOfConnectedHardware() > 0)
                {
                    lParam = sg_nNoOfChannels;
                }
                else
                {
                    hResult = S_FALSE;
                }
            }
            break;
            case DRIVER_STATUS:
            {
                lParam = true;
            }
            break;
            case HW_MODE:
            {
                if (nChannel < sg_nNoOfChannels)
                {
                    lParam = sg_ucControllerMode;
                    if( sg_ucControllerMode == 0 || sg_ucControllerMode > defMODE_SIMULATE )
                    {
                        lParam = defCONTROLLER_BUSOFF;
                        hResult = S_FALSE;
                    }
                }
                else
                {
                    lParam = defCONTROLLER_BUSOFF + 1;
                }
            }
            break;
            case CON_TEST:
            {
                UCHAR ucResult;
                if (nTestHardwareConnection(ucResult, nChannel) == S_OK)
                {
                    lParam = (LONG)ucResult;
                }
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

int getMilliCount(){
	timeb tb;
	ftime(&tb);
	int nCount = tb.millitm + (tb.time & 0xfffff) * 1000;
	return nCount;
}

static BYTE bClassifyMsgType(int DeviceID, unsigned __int8 *pData, unsigned __int32 Length, ABS_TIMESTAMP * timestamp, STCANDATA& sCanData)//HERE
{
	int milli = timestamp->milliseconds + (timestamp->seconds * 1000) + (timestamp->minutes * 60 * 1000) + (timestamp->hours * 60 * 60 * 1000);
    sCanData.m_lTickCount.QuadPart = milli * 10;
	char *pDataStr;
	DWORD HexLen;
	pDataStr = (char *)malloc((Length * 2) + Length + 10);
	Hex2String(pData,pDataStr,Length - 2 ,true);
    CString str(pDataStr);
	str.Remove(' ');
	CString id = str.Mid(pHardware->ID_Start*2,(pHardware->ID_End-pHardware->ID_Start+1)*2);

	if(id.Compare(pHardware->Fault_Code_Data_ID) == 0 && currentMessageState == STATE_THIRD){
		KillTimer(sg_hOwnerWnd,myTimer);
		membuffer += str;
		membuffer += "\n";
		if(str.Mid(pHardware->Fault_Code_Data_Byte_Start*2,(pHardware->Fault_Code_Data_Byte_End-pHardware->Fault_Code_Data_Byte_Start+1)*2).Compare("00") == 0){
			currentMessageState = STATE_OFF;
			AFX_MANAGE_STATE(AfxGetStaticModuleState());
			pHardware->finishedFault(membuffer);
			membuffer = "";
		}
	}

	if(Length == 14){
		str = str.Mid(6,16);
		strcpy(pDataStr, (LPCTSTR)str);
		String2Hex(pDataStr,pData,16,&HexLen);
		 std::string stri(id);
		 unsigned int x = strtoul(stri.c_str(), NULL, 16);
		 sCanData.m_uDataInfo.m_sCANMsg.m_unMsgID = x;
		 sCanData.m_uDataInfo.m_sCANMsg.m_ucRTR = FALSE;
		 sCanData.m_ucDataType = RX_FLAG;
		 memcpy(sCanData.m_uDataInfo.m_sCANMsg.m_ucData,pData, 16);
		 sCanData.m_uDataInfo.m_sCANMsg.m_ucDataLen = 8;
		 sCanData.m_uDataInfo.m_sCANMsg.m_ucChannel = 1; 
	}else{
		 sCanData.m_uDataInfo.m_sCANMsg.m_unMsgID = 0;
		 memcpy(sCanData.m_uDataInfo.m_sCANMsg.m_ucData,pData, Length);
		 sCanData.m_uDataInfo.m_sCANMsg.m_ucRTR = FALSE;
		 sCanData.m_ucDataType = RX_FLAG;
		 sCanData.m_uDataInfo.m_sCANMsg.m_ucDataLen = Length;
		 sCanData.m_uDataInfo.m_sCANMsg.m_ucChannel = 1; 
	}

    if (CREATE_MAP_TIMESTAMP == sg_byCurrState)
    {
        LARGE_INTEGER g_QueryTickCount;
        QueryPerformanceCounter(&g_QueryTickCount);
        UINT64 unConnectionTime;
        unConnectionTime = sg_TimeStamp;
        if(sCanData.m_lTickCount.QuadPart >= unConnectionTime)
        {
            sg_TimeStamp  = (LONGLONG)(sCanData.m_lTickCount.QuadPart - unConnectionTime);
        }
        else
        {
            sg_TimeStamp  = (LONGLONG)(unConnectionTime - sCanData.m_lTickCount.QuadPart);
        }

        sg_byCurrState = CALC_TIMESTAMP_READY;
    }
    return TRUE;
}
static void vWriteIntoClientsBuffer(STCANDATA& sCanData, UINT unClientIndex)
{
    for (UINT j = 0; j < sg_asClientToBufMap[unClientIndex].unBufCount; j++)
    {
        sg_asClientToBufMap[unClientIndex].pClientBuf[j]->WriteIntoBuffer(&sCanData);
    }
}
static void CALLBACK DataCallback(int DeviceID, unsigned __int8 *pData, unsigned __int32 Length, ABS_TIMESTAMP * timestamp)
{
	if(sg_bCurrState == STATE_HW_INTERFACE_LISTED){
		return;
	}
	int nSize = sg_nFRAMES;
	char * pDataStr;
	pDataStr = (char *)malloc((Length * 2) + Length + 10);
	Hex2String(pData,pDataStr,Length - 2 ,true);
	CString sData(pDataStr);
	CString test(pData);
	if(pHardware->passesFilter(sData) == true){
		bClassifyMsgType(DeviceID,pData,Length,timestamp, sg_asCANMsg);
		vWriteIntoClientsBuffer(sg_asCANMsg, 0);
	}
	pHardware->handleEmulations(sData);
}


HRESULT CDIL_CAN_SAINT::CAN_PerformInitOperations(void)
{
    HRESULT hResult = S_FALSE;
    sg_asCANMsg.m_uDataInfo.m_sCANMsg.m_bCANFD = false;
    DWORD dwClientID = 0;
    CAN_RegisterClient(TRUE, dwClientID, CAN_MONITOR_NODE);
    for ( UINT i = 0; i< CHANNEL_ALLOWED; i++ )
    {
        sg_anSelectedItems[i] = -1;
    }
    sg_asCANMsg.m_uDataInfo.m_sCANMsg.m_bCANFD = false;
    return hResult;
}
HRESULT CDIL_CAN_SAINT::CAN_GetCurrStatus(STATUSMSG& StatusData)
{
    StatusData.wControllerStatus = NORMAL_ACTIVE;
    return S_OK;
}	

HRESULT CDIL_CAN_SAINT::CAN_GetErrorCount(SERROR_CNT& sErrorCnt, UINT nChannel, ECONTR_PARAM eContrParam)
{
    HRESULT hResult = S_OK;
    if ((sg_bCurrState == STATE_CONNECTED) || (sg_bCurrState == STATE_HW_INTERFACE_SELECTED))
    {
        if (nChannel <= sg_nNoOfChannels)
        {
            if (eContrParam == ERR_CNT)
            {
                sErrorCnt.m_ucTxErrCount = sg_aodChannels[nChannel].m_ucTxErrorCounter;
                sErrorCnt.m_ucRxErrCount = sg_aodChannels[nChannel].m_ucRxErrorCounter;
            }
            else if (eContrParam == PEAK_ERR_CNT)
            {
                sErrorCnt.m_ucTxErrCount = sg_aodChannels[nChannel].m_ucPeakTxErrorCounter;
                sErrorCnt.m_ucRxErrCount = sg_aodChannels[nChannel].m_ucPeakRxErrorCounter;
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
HRESULT CDIL_CAN_SAINT::CAN_GetLastErrorString(std::string& acErrorStr)
{
    acErrorStr = sg_acErrStr;
    return S_OK;
}

HRESULT CDIL_CAN_SAINT::CAN_SetAppParams(HWND hWndOwner)
{
    sg_hOwnerWnd = hWndOwner;
    GetLocalTime(&sg_CurrSysTime);
    sg_TimeStamp = 0x0;
    sg_QueryTickCount.QuadPart = 0;
    sg_acErrStr = "";
    CAN_ManageMsgBuf(MSGBUF_CLEAR, 0, nullptr);

    return S_OK;
}

HRESULT CDIL_CAN_SAINT::CAN_GetTimeModeMapping(SYSTEMTIME& CurrSysTime, UINT64& TimeStamp, LARGE_INTEGER& QueryTickCount)
{
    CurrSysTime = sg_CurrSysTime;
    TimeStamp = sg_TimeStamp;
    QueryTickCount = sg_QueryTickCount;

    return S_OK;
}

HRESULT CDIL_CAN_SAINT::CAN_ListHwInterfaces(INTERFACE_HW_LIST& asSelHwInterface, INT& nCount)
{
    USES_CONVERSION;
    HRESULT hResult = S_FALSE;

    hResult = nInitHwNetwork(nCount);
    if ( hResult == 0)
    {
        nCount = sg_nNoOfChannels;
        for (UINT i = 0; i < sg_nNoOfChannels; i++)
        {
            asSelHwInterface[i].m_dwIdInterface = i;

            std::ostringstream oss;
            std::string strTemp =  oss.str();
            asSelHwInterface[i].m_acDescription = strTemp;
            std::ostringstream oss1;
            sg_ControllerDetails[i].m_omHardwareDesc = oss1.str();
            asSelHwInterface[i].m_acNameInterface = oss1.str();
            sg_bCurrState = STATE_HW_INTERFACE_LISTED;
        }
        hResult = S_OK;
    }
    else
    {
        sg_pIlog->vLogAMessage(A2T(__FILE__), __LINE__, _("Error connecting to driver"));
    }
    return S_OK;
}

HRESULT CDIL_CAN_SAINT::CAN_LoadDriverLibrary(void)
{
    
	
    HRESULT hResult = S_OK;
	Port = PORT_DEFAULT;
	Host = HOST_DEFAULT;
	MsgBufferSize = MSG_BUFFER_DEFAULT;
	CString Text;
	CString WindowText;
	CString devices;
	int pos;
	CString id;
	CString ClassID;
    if (EngineConnection.Connect(Host, atoi(Port), Text))
    {
		EngineConnection.SetDataCallback(DataCallback);
	}
	else
	{
		AfxMessageBox("Failed to connect to engine.");
	}
    InitializeCriticalSection(&sg_CritSectForWrite);
    return hResult;
}
static BOOL bGetClientObj(DWORD dwClientID, UINT& unClientIndex)
{
    BOOL bResult = FALSE;
    for (UINT i = 0; i < sg_unClientCnt; i++)
    {
        if (sg_asClientToBufMap[i].dwClientID == dwClientID)
        {
            unClientIndex = i;
            i = sg_unClientCnt; 
            bResult = TRUE;
        }
    }
    return bResult;
}
static BOOL bIsBufferExists(const SCLIENTBUFMAP& sClientObj, const CBaseCANBufFSE* pBuf)
{
    BOOL bExist = FALSE;
    for (UINT i = 0; i < sClientObj.unBufCount; i++)
    {
        if (pBuf == sClientObj.pClientBuf[i])
        {
            bExist = TRUE;
            i = sClientObj.unBufCount; 
        }
    }

    return bExist;
}
static BOOL bRemoveClientBuffer(CBaseCANBufFSE* RootBufferArray[MAX_BUFF_ALLOWED], UINT& unCount, CBaseCANBufFSE* BufferToRemove)
{
    BOOL bReturn = TRUE;
    for (UINT i = 0; i < unCount; i++)
    {
        if (RootBufferArray[i] == BufferToRemove)
        {
            if (i < (unCount - 1)) /* If not the last bufffer */
            {
                RootBufferArray[i] = RootBufferArray[unCount - 1];
            }
            unCount--;
        }
    }
    return bReturn;
}

HRESULT CDIL_CAN_SAINT::CAN_ManageMsgBuf(BYTE bAction, DWORD ClientID, CBaseCANBufFSE* pBufObj)
{
    HRESULT hResult = S_FALSE;
    if (ClientID != 0)
    {
        UINT unClientIndex;
        if (bGetClientObj(ClientID, unClientIndex))
        {
            SCLIENTBUFMAP& sClientObj = sg_asClientToBufMap[unClientIndex];
            if (bAction == MSGBUF_ADD)
            {
                /* Add msg buffer */
                if (pBufObj != nullptr)
                {
                    if (sClientObj.unBufCount < MAX_BUFF_ALLOWED)
                    {
                        if (bIsBufferExists(sClientObj, pBufObj) == FALSE)
                        {
                            sClientObj.pClientBuf[sClientObj.unBufCount++] = pBufObj;
                            hResult = S_OK;
                        }
                        else
                        {
                            hResult = ERR_BUFFER_EXISTS;
                        }
                    }
                }
            }
            else if (bAction == MSGBUF_CLEAR)
            {
                /* clear msg buffer */
                if (pBufObj != nullptr)
                {
                    /* Remove only buffer mentioned */
                    bRemoveClientBuffer(sClientObj.pClientBuf, sClientObj.unBufCount, pBufObj);
                }
                else
                {
                    /* Remove all */
                    for (UINT i = 0; i < sClientObj.unBufCount; i++)
                    {
                        sClientObj.pClientBuf[i] = nullptr;
                    }
                    sClientObj.unBufCount = 0;
                }
                hResult = S_OK;
            }
        }
        else
        {
            hResult = ERR_NO_CLIENT_EXIST;
        }
    }
    else
    {
        if (bAction == MSGBUF_CLEAR)
        {
            /* clear msg buffer */
            for (UINT i = 0; i < sg_unClientCnt; i++)
            {
                CAN_ManageMsgBuf(MSGBUF_CLEAR, sg_asClientToBufMap[i].dwClientID, nullptr);
            }
            hResult = S_OK;
        }
    }

    return hResult;
}
static BOOL bRemoveClient(DWORD dwClientId)
{
    BOOL bResult = FALSE;

    if (sg_unClientCnt > 0)
    {
        UINT unClientIndex = (UINT)-1;
        if (bGetClientObj(dwClientId, unClientIndex))
        {
            /* clear the client first */
            if (sg_asClientToBufMap[unClientIndex].hClientHandle != 0)
            {
                HRESULT hResult = S_OK;
                if (hResult == S_OK)
                {
                    sg_asClientToBufMap[unClientIndex].dwClientID = 0;
                    sg_asClientToBufMap[unClientIndex].hClientHandle = 0;
                    memset (sg_asClientToBufMap[unClientIndex].pacClientName, 0, sizeof (char) * MAX_PATH);
                    for (int i = 0; i < MAX_BUFF_ALLOWED; i++)
                    {
                        sg_asClientToBufMap[unClientIndex].pClientBuf[i] = nullptr;
                    }
                    sg_asClientToBufMap[unClientIndex].unBufCount = 0;
                    bResult = TRUE;
                }
            }
            else
            {
                sg_asClientToBufMap[unClientIndex].dwClientID = 0;
                memset (sg_asClientToBufMap[unClientIndex].pacClientName, 0, sizeof (char) * MAX_PATH);
                for (int i = 0; i < MAX_BUFF_ALLOWED; i++)
                {
                    sg_asClientToBufMap[unClientIndex].pClientBuf[i] = nullptr;
                }
                sg_asClientToBufMap[unClientIndex].unBufCount = 0;
                bResult = TRUE;

            }
            if (bResult == TRUE)
            {
                if ((unClientIndex + 1) < sg_unClientCnt)
                {
                    sg_asClientToBufMap[unClientIndex] = sg_asClientToBufMap[sg_unClientCnt - 1];
                }
                sg_unClientCnt--;
            }
        }
    }
	EngineConnection.DisconnectFromDevice(deviceID);
    return bResult;
}

HRESULT CDIL_CAN_SAINT::CAN_PerformClosureOperations(void)
{
    HRESULT hResult = S_OK;

    hResult = CAN_StopHardware();
    if (hResult == S_OK)
    {
        UINT ClientIndex = 0;
        while (sg_unClientCnt > 0)
        {
            bRemoveClient(sg_asClientToBufMap[ClientIndex].dwClientID);

        }

        if (hResult == S_OK)
        {
            sg_bCurrState = STATE_DRIVER_SELECTED;

        }
		EngineConnection.Disconnect();
    }
    return hResult;
}
static BOOL bClientIdExist(const DWORD& dwClientId)
{
    BOOL bReturn = FALSE;
    for (UINT i = 0; i < sg_unClientCnt; i++)
    {
        if (sg_asClientToBufMap[i].dwClientID == dwClientId)
        {
            bReturn = TRUE;
            i = sg_unClientCnt; 
        }
    }
    return bReturn;
}
static DWORD dwGetAvailableClientSlot()
{
    DWORD nClientId = 2;
    for (int i = 0; i < MAX_CLIENT_ALLOWED; i++)
    {
        if (bClientIdExist(nClientId))
        {
            nClientId += 1;
        }
        else
        {
            i = MAX_CLIENT_ALLOWED; 
        }
    }

    return nClientId;
}


static BOOL bClientExist(std::string pcClientName, INT& Index)
{
    for (UINT i = 0; i < sg_unClientCnt; i++)
    {
        if (!_tcscmp(pcClientName.c_str(), sg_asClientToBufMap[i].pacClientName))
        {
            Index = i;
            return TRUE;
        }
    }
    return FALSE;
}



HRESULT CDIL_CAN_SAINT::CAN_RegisterClient(BOOL bRegister, DWORD& ClientID, char* pacClientName) 
{
    USES_CONVERSION;
    HRESULT hResult = S_OK;
	if (bRegister)
    {
        if (sg_unClientCnt < MAX_CLIENT_ALLOWED)
        {
            INT Index = 0;
            if (!bClientExist(pacClientName, Index))
            {
                /* Currently store the client information */
                if (_tcscmp(pacClientName, CAN_MONITOR_NODE) == 0)
                {
                    /* First slot is reserved to monitor node */
                    ClientID = 1;
                    _tcscpy(sg_asClientToBufMap[0].pacClientName, pacClientName);
                    sg_asClientToBufMap[0].dwClientID = ClientID;
                    sg_asClientToBufMap[0].unBufCount = 0;
                }
                else
                {
                    Index = sg_unClientCnt;
                    ClientID = dwGetAvailableClientSlot();
                    _tcscpy(sg_asClientToBufMap[Index].pacClientName, pacClientName);

                    sg_asClientToBufMap[Index].dwClientID = ClientID;
                    sg_asClientToBufMap[Index].unBufCount = 0;
                }
                sg_unClientCnt++;
                hResult = S_OK;
            }
            else
            {
                ClientID = sg_asClientToBufMap[Index].dwClientID;
                hResult = ERR_CLIENT_EXISTS;
            }
        }
        else
        {
            hResult = ERR_NO_MORE_CLIENT_ALLOWED;
        }
    }
    else
    {
        if (bRemoveClient(ClientID))
        {
            hResult = S_OK;
        }
        else
        {
            hResult = ERR_NO_CLIENT_EXIST;
        }
    }
	

    return hResult;
}
HRESULT CDIL_CAN_SAINT::CAN_SelectHwInterface(const INTERFACE_HW_LIST& asSelHwInterface, INT /*nCount*/)
{
    USES_CONVERSION;
    sg_bCurrState = STATE_HW_INTERFACE_SELECTED;
    for (UINT nCount = 0; nCount < sg_ucNoOfHardware; nCount++)
    {
        sg_ControllerDetails[nCount].m_omHardwareDesc = asSelHwInterface[nCount].m_acNameInterface;
    }

    return S_OK;
}
HRESULT CDIL_CAN_SAINT::CAN_SendMsg(DWORD dwClientID, const STCAN_MSG& sMessage)
{

   CString message = pHardware->Clear_Fault_Message.Mid(0, 2);
	CString id;
	id.Format("%x", sMessage.m_unMsgID);
	for (int i = id.GetLength(); i < 4; i++){
		id = "0" + id;
	}
	message += id;
	for (int i = 0; i < sMessage.m_ucDataLen; i++){
		CString d;
		d.Format("%x", sMessage.m_ucData[i]);
		for (int j = d.GetLength(); j < 2; j++){
			d = "0" + d;
		}
		message += d;
	}
	CAN_SendMessage(message);
    return S_OK;
}
HRESULT CDIL_CAN_SAINT::CAN_SetControllerParams(INT nValue, ECONTR_PARAM eContrparam)
{
    return S_OK;
}

HRESULT CDIL_CAN_SAINT::CAN_SetHardwareChannel(PSCONTROLLER_DETAILS ouControllerDetails,DWORD dwDriverId,bool bIsHardwareListed, unsigned int unChannelCount)//TODO dont know what this is doing
{
    PSCONTROLLER_DETAILS psContrlDets = (PSCONTROLLER_DETAILS)ouControllerDetails;

    sg_SelectedChannels.m_nChannelCount = 0;
    for ( UINT i = 0; i< CHANNEL_ALLOWED; i++ )
    {
        sg_anSelectedItems[i] = -1;
    }

    for (int nChannel = 0; nChannel < unChannelCount ; nChannel++)
    {
        if((psContrlDets[nChannel].m_omHardwareDesc != "") && (psContrlDets[nChannel].m_omHardwareDesc != "Simulation"))
        {
            sg_SelectedChannels.m_omHardwareChannel[nChannel] = psContrlDets[nChannel].m_omHardwareDesc;
            sg_ControllerDetails[nChannel].m_omHardwareDesc = psContrlDets[nChannel].m_omHardwareDesc;
            sg_SelectedChannels.m_nChannelCount += 1;
        }
    }

    sg_ucNoOfHardware = (UCHAR)sg_SelectedChannels.m_nChannelCount;
    sg_nNoOfChannels = (UINT)sg_SelectedChannels.m_nChannelCount;
    return S_OK;
}
static void vCreateTimeModeMapping(HANDLE hDataEvent)
{
    WaitForSingleObject(hDataEvent, INFINITE);
    GetLocalTime(&sg_CurrSysTime);
    /*Query Tick Count*/
    QueryPerformanceCounter(&sg_QueryTickCount);
}

HRESULT CDIL_CAN_SAINT::CAN_StartHardware(void)
{
    VALIDATE_VALUE_RETURN_VAL(sg_bCurrState, STATE_HW_INTERFACE_SELECTED, ERR_IMPROPER_STATE);

    USES_CONVERSION;
    HRESULT hResult = S_OK;
	deviceID = EngineConnection.ConnectToDevice(device);
	if (hResult == S_OK){
        hResult = S_OK;
        sg_bCurrState = STATE_CONNECTED;
    }else{
        hResult = ERR_LOAD_HW_INTERFACE;
    }



	return hResult;
}
HRESULT CDIL_CAN_SAINT::CAN_StopHardware(void)
{
    VALIDATE_VALUE_RETURN_VAL(sg_bCurrState, STATE_CONNECTED, ERR_IMPROPER_STATE);

    HRESULT hResult = S_OK;
	EngineConnection.DisconnectFromDevice(deviceID);
    if (hResult == S_OK)
    {
        hResult = S_OK;
        sg_bCurrState = STATE_HW_INTERFACE_SELECTED;
    }
    else
    {
        hResult = ERR_LOAD_HW_INTERFACE;
    }

    return hResult;
}
void CCAN_SAINTApp::unloadDriver(){
	pHardware->DestroyWindow();
}
HRESULT CDIL_CAN_SAINT::CAN_UnloadDriverLibrary(void)
{
	if(pHardware){
		pHardware->DestroyWindow();
	}
    return S_OK;
}
