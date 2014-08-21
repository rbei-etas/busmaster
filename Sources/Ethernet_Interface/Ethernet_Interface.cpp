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
 * \file      CEthernet_Interface.cpp
 * \brief     Exports API functions for Ethernet interface
 * \author    Ashwin R Uchil
 * \copyright Copyright (c) 2014, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Exports API functions for Ethernet interface
 */

#pragma once

#include <sstream>
#include <string>
#include <vector>

#include "Ethernet_Interface.h"

#define USAGE_EXPORT
#define defERR_OK                           0

#include "Utility/Utility_Thread.h"
#include "Include/DIL_CommonDefs.h"

#include "DataTypes/Base_WrapperErrorLogger.h"
#include "Ethernet_Interface_Extern.h"
#include "DIL_Interface/BaseDIL_Ethernet_Controller.h"

#include "../Application/MultiLanguage.h"
#include "Connector.h"
#include "Base_EthernetAppLayerProtocol.h"
#include "EConfigDlg.h"

using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
const int nBitsIn4Bytes          = 32;
#define MAKELONGLONG(HINT, LINT) ( ((unsigned __int64)((UINT)(HINT))) | (((__int64)(LINT)) << nBitsIn4Bytes) )
typedef HRESULT (__stdcall* GET_CUSTOM_PROTOCOL_INTERFACE)(void** ppvInterface);
static GET_CUSTOM_PROTOCOL_INTERFACE pfGetCustomProtocolInterface;

//typedef std::basic_string<unsigned char> MyString;
//#define string MyString

// CEthernet_Interface

BEGIN_MESSAGE_MAP(CEthernet_Interface, CWinApp)
END_MESSAGE_MAP()

/**
 * CEthernet_Interface construction
 */
CEthernet_Interface::CEthernet_Interface()
{
    // TODO: add construction code here,
    // Place all significant initialization in InitInstance
}

// The one and only CEthernet_Interface object
CEthernet_Interface theApp;

/**
 * CEthernet_Interface initialization
 */
static HINSTANCE ghLangInst=NULL;

BOOL CEthernet_Interface::InitInstance()
{
    //// Begin of Multiple Language support
    //if ( CMultiLanguage::m_nLocales <= 0 )    // Not detected yet
    //{
    //    CMultiLanguage::DetectLangID(); // Detect language as user locale
    //    CMultiLanguage::DetectUILanguage();    // Detect language in MUI OS
    //}
    //TCHAR szModuleFileName[MAX_PATH];        // Get Module File Name and path
    //int ret = ::GetModuleFileName(theApp.m_hInstance, szModuleFileName, MAX_PATH);
    //if ( ret == 0 || ret == MAX_PATH )
    //{
    //    ASSERT(FALSE);
    //}
    //// Load resource-only language DLL. It will use the languages
    //// detected above, take first available language,
    //// or you can specify another language as second parameter to
    //// LoadLangResourceDLL. And try that first.
    //ghLangInst = CMultiLanguage::LoadLangResourceDLL( szModuleFileName );
    //if (ghLangInst)
    //{
    //    AfxSetResourceHandle( ghLangInst );
    //}
    //// End of Multiple Language support

    CWinApp::InitInstance();

    return TRUE;
}

#define ETHERNET_MAX_ERRSTR 256
/**
 * Client and Client Buffer map
 */
typedef struct tagClientBufMap
{
    DWORD dwClientID;
    BYTE hClientHandle;
    CBaseEthernetBufFSE* pClientBuf[MAX_BUFF_ALLOWED];
    char pacClientName[MAX_PATH];
    UINT unBufCount;
    tagClientBufMap()
    {
        dwClientID = 0;
        hClientHandle = NULL;
        unBufCount = 0;
        memset(pacClientName, 0, sizeof (char) * MAX_PATH);
        for (int i = 0; i < MAX_BUFF_ALLOWED; i++)
        {
            pClientBuf[i] = NULL;
        }

    }
} SCLIENTBUFMAP;

static UINT sg_unClientCnt = 0;

#define MAX_CLIENT_ALLOWED 16
const int MAX_STRING = 256;
/**
 * Query Tick Count
 */
static LARGE_INTEGER sg_QueryTickCount;

static string					sg_acErrStr = "";
static BOOL						sg_bIsConnected = FALSE;
static UINT						sg_nNoOfChannels = 0;
static char						sg_omErrStr[MAX_STRING] = {0};
static CRITICAL_SECTION			sg_CritSectForAckBuf;       // To make it thread safe
static bool						sg_bIsMaster = true;
static string					sg_acApplicationLayerProtocol;
static CConnector				sg_Connector;
static CPARAM_THREADPROC		sg_sParmRThread;
static Base_EAppLayerProtocol*  sg_pBaseAppLayProtocol;
static STETHERNETDATA			sg_sEthenetData;
static pcap_if_t *				sg_pSelectedAdapter = NULL;
static HMODULE					sg_hDll = NULL;					/* Variable to hold handle to currently selected controller DIL */
static UINT64					sg_qwRefSysTime = NULL;
extern CRITICAL_SECTION sg_CritSectReadSync;
extern HANDLE sg_hDataReceivedEvent;

// Count variables
static UCHAR sg_ucNoOfHardware = 0;
const int MAX_DEVICES = 16;

static SCLIENTBUFMAP sg_asClientToBufMap[MAX_CLIENT_ALLOWED];

static HWND sg_hOwnerWnd = NULL;
static Base_WrapperErrorLogger* sg_pIlog   = NULL;
HANDLE g_hInformStopTx;  // Indicates the stop tranmission is invoked
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

enum
{
    CREATE_MAP_TIMESTAMP = 0x1,
    CALC_TIMESTAMP_READY = 0x2,
};

typedef struct tagAckMap
{
    UINT m_MsgID;
    UINT m_ClientID;
    UINT m_Channel;

    BOOL operator == (const tagAckMap& RefObj)const
    {
        return ((m_MsgID == RefObj.m_MsgID) && (m_Channel == RefObj.m_Channel));
    }
} SACK_MAP;

typedef std::list<SACK_MAP> CACK_MAP_LIST;
static CACK_MAP_LIST sg_asAckMapBuf;

/**
 * Current state machine
 */
static BYTE sg_byCurrState = CREATE_MAP_TIMESTAMP;


/* CDIL_Ethernet_Interface class definition */
class CDIL_Ethernet_Interface : public CBaseDIL_Ethernet_Controller
{
public:

     HRESULT Ethernet_PerformInitOperations(void) ;
     HRESULT Ethernet_PerformClosureOperations(void) ;
     HRESULT Ethernet_GetTimeModeMapping(SYSTEMTIME& CurrSysTime, UINT64& TimeStamp, LARGE_INTEGER* QueryTickCount) ;
     HRESULT Ethernet_DisplayConfigDlg(PSCONTROLLER_DETAILS InitData, int& Length) ;
     HRESULT Ethernet_SetConfigData(PSCONTROLLER_DETAILS InitData, int Length) ;
     HRESULT Ethernet_SendMsg(DWORD dwClientID, const STETHERNET_MSG& sCanTxMsg) ;
     HRESULT Ethernet_GetLastErrorString(string& acErrorStr) ;
     HRESULT Ethernet_SetControllerParams(int nValue, ECONTR_PARAM eContrparam) ;
     HRESULT Ethernet_GetErrorCount(SERROR_CNT& sErrorCnt, UINT nChannel, ECONTR_PARAM eContrParam) ;
	 HRESULT Ethernet_SetAppParams(HWND hWndOwner, Base_WrapperErrorLogger* pILog);
	 HRESULT Ethernet_StartHardware(void) ;
     HRESULT Ethernet_StopHardware(void) ;
	 HRESULT Ethernet_ListHwInterfaces(INTERFACE_HW_LIST& sSelHwInterface, INT& nCount);
     HRESULT Ethernet_SelectHwInterface(const INTERFACE_HW_LIST& sSelHwInterface, INT nCount);
     HRESULT Ethernet_DeselectHwInterface(void);

    // Specific function set
     HRESULT Ethernet_ManageMsgBuf(BYTE bAction, DWORD ClientID, CBaseEthernetBufFSE* pBufObj) ;
     HRESULT Ethernet_RegisterClient(BOOL bRegister, DWORD& ClientID, char* pacClientName) ;
     HRESULT Ethernet_GetCntrlStatus(const HANDLE& hEvent, UINT& unCntrlStatus) ;
	 HRESULT Ethernet_LoadDriverLibrary(void);
     HRESULT Ethernet_UnloadDriverLibrary(void);
	 HMODULE  Ethernet_GetCustomProtocolHandle();

};

static CDIL_Ethernet_Interface* sg_pouDIL_Ethernet_Interface = NULL;



/*--------------------Global Functions---------------------------------*/
static BOOL bClientExist(string pcClientName, INT& Index)
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

static BOOL bGetClientObj(DWORD dwClientID, UINT& unClientIndex)
{
    BOOL bResult = FALSE;
    for (UINT i = 0; i < sg_unClientCnt; i++)
    {
        if (sg_asClientToBufMap[i].dwClientID == dwClientID)
        {
            unClientIndex = i;
            i = sg_unClientCnt; //break the loop
            bResult = TRUE;
        }
    }
    return bResult;
}

static BOOL bIsBufferExists(const SCLIENTBUFMAP& sClientObj, const CBaseEthernetBufFSE* pBuf)
{
    BOOL bExist = FALSE;
    for (UINT i = 0; i < sClientObj.unBufCount; i++)
    {
        if (pBuf == sClientObj.pClientBuf[i])
        {
            bExist = TRUE;
            i = sClientObj.unBufCount; //break the loop
        }
    }
    return bExist;
}


static BOOL bRemoveClientBuffer(CBaseEthernetBufFSE* RootBufferArray[MAX_BUFF_ALLOWED], UINT& unCount, CBaseEthernetBufFSE* BufferToRemove)
{
    BOOL bReturn = TRUE;
    for (UINT i = 0; i < unCount; i++)
    {
        if (RootBufferArray[i] == BufferToRemove)
        {
            if (i < (unCount - 1)) //If not the last bufffer
            {
                RootBufferArray[i] = RootBufferArray[unCount - 1];
            }
            unCount--;
        }
    }
    return bReturn;
}

static BOOL bClientIdExist(const DWORD& dwClientId)
{
    BOOL bReturn = FALSE;
    for (UINT i = 0; i < sg_unClientCnt; i++)
    {
        if (sg_asClientToBufMap[i].dwClientID == dwClientId)
        {
            bReturn = TRUE;
            i = sg_unClientCnt; // break the loop
        }
    }
    return bReturn;
}

void vMarkEntryIntoMap(const SACK_MAP& RefObj)
{
    //EnterCriticalSection(&sg_CritSectForAckBuf); // Lock the buffer
    sg_asAckMapBuf.push_back(RefObj);
    //LeaveCriticalSection(&sg_CritSectForAckBuf); // Unlock the buffer
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
            i = MAX_CLIENT_ALLOWED; //break the loop
        }
    }
    return nClientId;
}

/**
 * Function to retreive error occurred and log it
 */
static void vRetrieveAndLog(DWORD /*dwErrorCode*/, char* File, int Line)
{
    USES_CONVERSION;

    char acErrText[MAX_PATH] = {'\0'};
    // Get the error text for the corresponding error code
    //if ((*pfCAN_GetErrText)(dwErrorCode, acErrText) == CAN_USB_OK)
    {
        sg_pIlog->vLogAMessage(A2T(File), Line, A2T(acErrText));

        size_t nStrLen = strlen(acErrText);
        if (nStrLen > ETHERNET_MAX_ERRSTR)
        {
            nStrLen = ETHERNET_MAX_ERRSTR;
        }
        sg_acErrStr = acErrText;
    }
}

static BOOL bRemoveClient(DWORD dwClientId)
{
    BOOL bResult = FALSE;
    if (sg_unClientCnt > 0)
    {
        UINT unClientIndex = (UINT)-1;
        if (bGetClientObj(dwClientId, unClientIndex))
        {
            //clear the client first
            if (sg_asClientToBufMap[unClientIndex].hClientHandle != NULL)
            {
                HRESULT hResult = S_OK;//(*pfCAN_RemoveClient)(sg_asClientToBufMap[unClientIndex].hClientHandle);
                if (hResult == S_OK)
                {
                    sg_asClientToBufMap[unClientIndex].dwClientID = 0;
                    sg_asClientToBufMap[unClientIndex].hClientHandle = NULL;
                    memset (sg_asClientToBufMap[unClientIndex].pacClientName, 0, sizeof (char) * MAX_PATH);
                    for (int i = 0; i < MAX_BUFF_ALLOWED; i++)
                    {
                        sg_asClientToBufMap[unClientIndex].pClientBuf[i] = NULL;
                    }
                    sg_asClientToBufMap[unClientIndex].unBufCount = 0;
                    bResult = TRUE;
                }
                else
                {
                    vRetrieveAndLog(hResult, __FILE__, __LINE__);
                }
            }
            else
            {
                sg_asClientToBufMap[unClientIndex].dwClientID = 0;
                memset (sg_asClientToBufMap[unClientIndex].pacClientName, 0, sizeof (char) * MAX_PATH);
                for (int i = 0; i < MAX_BUFF_ALLOWED; i++)
                {
                    sg_asClientToBufMap[unClientIndex].pClientBuf[i] = NULL;
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
    return bResult;
}


/**
* \brief         Function to retreive the Tx message entry from sg_asAckMapBuf list and delete.
*                Also returns the respective client ID.
* \param[in]     RefObj, is SACK_MAP entry to find and delete
* \param[out]    ClientID, is client ID of the RefObj found
* \return        TRUE if RefObj is found, else FALSE
* \authors       Ashwin R Uchil
* \date          19.07.2014 Created
*/
BOOL bRemoveMapEntry(const SACK_MAP& RefObj, UINT& ClientID)
{
    EnterCriticalSection(&sg_CritSectForAckBuf); // Lock the buffer
    BOOL bResult = FALSE;
    CACK_MAP_LIST::iterator  iResult =
        std::find( sg_asAckMapBuf.begin(), sg_asAckMapBuf.end(), RefObj );

    //if ((*iResult).m_ClientID > 0)
    if (iResult != sg_asAckMapBuf.end())
    {
        bResult = TRUE;
        ClientID = (*iResult).m_ClientID;
        sg_asAckMapBuf.erase(iResult);
    }
    LeaveCriticalSection(&sg_CritSectForAckBuf); // Unlock the buffer
    return bResult;
}

static int nConnect(BOOL bConnect)
{
    int nReturn = -1;

    if (!sg_bIsConnected && bConnect) // Disconnected and to be connected
    {
		/*sg_Connector.CreateSocket();
		sg_Connector.ListenToSocket();*/
		sg_bIsConnected = bConnect;
		sg_Connector.StartStopReceptionLoop(false);
		//sg_Connector.ReceivePackets;
	}
	else if (sg_bIsConnected && !bConnect) // Connected & to be disconnected
    {
        sg_bIsConnected = bConnect;
        Sleep(0); // Let other threads run for once
		//nReturn = sg_Connector.ShutDownSocket();
		sg_Connector.StartStopReceptionLoop(true);
    }

	if ( sg_bIsConnected )
    {
        InitializeCriticalSection(&sg_CritSectForAckBuf);
    }
    else
    {
        DeleteCriticalSection(&sg_CritSectForAckBuf);
    }

	return S_OK;
}

/**
* \brief         Function to create time mode mapping
* \param[in]     hDataEvent, is HANDLE
* \return        void
* \authors       Ashwin R Uchil
* \date          18.06.2014 Created
*/
static void vCreateTimeModeMapping(HANDLE hDataEvent)
{
    WaitForSingleObject(hDataEvent, INFINITE);
    GetLocalTime(&sg_CurrSysTime);
    /*Query Tick Count*/
    QueryPerformanceCounter(&sg_QueryTickCount);
}

bool FillEthernetDataFromMsgDetails(STETHERNET_MSG& sETHERNETMsg, EMSGDETAILS sEthernetMsgDetails)
{
	sETHERNETMsg.m_unMsgID			= sEthernetMsgDetails.lMsgID; 
	sETHERNETMsg.m_unDataLen		= sEthernetMsgDetails.nDatalength;
	strcpy((char*)sETHERNETMsg.m_ucProtocolName, (char*)sEthernetMsgDetails.ucProtocolName);
	strcpy((char*)sETHERNETMsg.m_ucData,(char*)sEthernetMsgDetails.ucData);
	return true;
}	

static void vWriteIntoClientsBuffer(STETHERNETDATA& sEthernetData)
{
	//Write into the client's buffer and Increment message Count
	static SACK_MAP sAckMap;
	UINT ClientId = 0;
	static UINT Index = (UINT)-1;
	sAckMap.m_Channel = sEthernetData.m_uDataInfo.m_sETHERNETMsg.m_ucChannel;
	sAckMap.m_MsgID = sEthernetData.m_uDataInfo.m_sETHERNETMsg.m_unMsgID;

	//Check if it is an acknowledgement message
	if (bRemoveMapEntry(sAckMap, ClientId))
	{
		BOOL bClientExists = bGetClientObj(ClientId, Index);
		for (UINT i = 0; i < sg_unClientCnt; i++)
		{
			//Tx for sender node
			if (/*(i == CAN_MONITOR_NODE_INDEX)  ||*/ (bClientExists && (i == Index)))
			{
				sEthernetData.m_ucDataType = TX_FLAG;
				for (UINT j = 0; j < sg_asClientToBufMap[i].unBufCount; j++)
				{
					sg_asClientToBufMap[i].pClientBuf[j]->WriteIntoBuffer(&sEthernetData);
				}
			}
			else
			{
				//Send the other nodes as Rx.
				for (UINT j = 0; j < sg_asClientToBufMap[i].unBufCount; j++)
				{
					static STETHERNETDATA sTempEthData;
					sTempEthData = sEthernetData;
					sTempEthData.m_ucDataType = RX_FLAG;
					sg_asClientToBufMap[i].pClientBuf[j]->WriteIntoBuffer(&sTempEthData);
				}
			}
		}
	}
	else // provide it to everybody
	{
		for (UINT i = 0; i < sg_unClientCnt; i++)
		{
			/* Write into the respective client's buffer */
			for (UINT j = 0; j < sg_asClientToBufMap[i].unBufCount; j++)
			{
				sg_asClientToBufMap[i].pClientBuf[j]->WriteIntoBuffer(&sEthernetData);
			}
		}
	}
}

static int nWriteMessage(const STETHERNET_MSG& sMessage)
{
	if(sg_Connector.SendMsg(sMessage) == S_OK)
	{
		STETHERNETDATA   sData;
		SYSTEMTIME   sTime;
		GetLocalTime(&sTime);
		//Convert to seconds
		long lTimeInSecs = (((sTime.wHour * 60) + sTime.wMinute)* 60 + sTime.wSecond) ;
		//Convert to microseconds
		LONGLONG llTemp = lTimeInSecs * 10000 + sTime.wMilliseconds *   10;

		UINT64 un64ConnectionTime = (sg_CurrSysTime.wHour * 3600 + sg_CurrSysTime.wMinute * 60 +
				+ sg_CurrSysTime.wSecond) * 10000 + sg_CurrSysTime.wMilliseconds * 10;
		

		sData.m_lTickCount.QuadPart = llTemp - un64ConnectionTime;
		sData.m_ucDataType = TX_FLAG;
		sData.m_uDataInfo.m_sETHERNETMsg = sMessage;
		vWriteIntoClientsBuffer(sData);
	}
	return TRUE;
}

static void ProcessMsg(PPKTHDR *pHeader,const u_char *pkt_data )
{
	/*if(sg_mBaseAppLayProtocol == NULL)
	{
		sg_pIlog->vLogAMessage(A2T(__FILE__), __LINE__, _T(_("Application Layer Protocol is not yet selected")));
	}*/

	if((pHeader == NULL) || (pkt_data == NULL))
	{
		return;
	}

	int res;
	char timestr[16];
	ip_header *ih;
	//udp_header *uh;
	mac_header *mh;
	u_short sport,dport;

	EnterCriticalSection(&sg_CritSectReadSync);
	
	struct tm  * ltime;
	time_t local_tv_sec = pHeader->ts.tv_sec;
	
	ltime = localtime(&local_tv_sec);
	//Convert to seconds
	long lTimeInSecs = (((ltime->tm_hour * 60) + ltime->tm_min)* 60 + ltime->tm_sec) ;
	//Convert to microseconds
	LONGLONG llTemp = lTimeInSecs * 10000 + pHeader->ts.tv_usec/100;

	if(sg_qwRefSysTime == 0)
	{
		sg_qwRefSysTime = (sg_CurrSysTime.wHour * 3600 + sg_CurrSysTime.wMinute * 60 +
                      + sg_CurrSysTime.wSecond) * 10000 + sg_CurrSysTime.wMilliseconds * 10;
	}

	sg_sEthenetData.m_lTickCount.QuadPart = llTemp - sg_qwRefSysTime;

	//position of MAC addresses
	mh = (mac_header *)(pkt_data);

	/* retireve the position of the ip header */
	ih = (ip_header *) (pkt_data +
		14); //length of ethernet header

	ERAWMSG  sRawMsg;
	memcpy(sRawMsg.chData, (char*)pkt_data, pHeader->len);
	LeaveCriticalSection(&sg_CritSectReadSync);

	static char    cMAC[20];
	sprintf(cMAC,"%.2x%.2x%.2x%.2x%.2x%.2x\0",
		mh->source[0],mh->source[1],mh->source[2],mh->source[3],mh->source[4],mh->source[5]);
	memcpy(sg_sEthenetData.m_uDataInfo.m_sETHERNETMsg.m_ucSourceMAC, cMAC, sizeof(cMAC));
	sprintf(cMAC, "%.2x%.2x%.2x%.2x%.2x%.2x\0",
		mh->dest[0],mh->dest[1],mh->dest[2],mh->dest[3],mh->dest[4],mh->dest[5]);
	memcpy(sg_sEthenetData.m_uDataInfo.m_sETHERNETMsg.m_ucDestMac, cMAC, sizeof(cMAC));

	/* retreive ip addresses */
	static char    cIPAddr[20];
	sprintf(cIPAddr, "%d.%d.%d.%d",ih->saddr.byte1, ih->saddr.byte2, ih->saddr.byte3, ih->saddr.byte4);
	memcpy(sg_sEthenetData.m_uDataInfo.m_sETHERNETMsg.m_ucSourceIP, cIPAddr, sizeof(cIPAddr));
	sprintf(cIPAddr, "%d.%d.%d.%d",ih->daddr.byte1, ih->daddr.byte2, ih->daddr.byte3, ih->daddr.byte4);
	memcpy(sg_sEthenetData.m_uDataInfo.m_sETHERNETMsg.m_ucDestIP, cIPAddr, sizeof(cIPAddr));

	//Get only the data part
	//int i = 14 + 1;
	//string strData = "", strTempData = "";
	//strTempData = (char*)pkt_data;
	//strData= strTempData;
	
	sg_sEthenetData.m_ucDataType = RX_FLAG;
	strcpy(sRawMsg.chSrcMAC, (char*)sg_sEthenetData.m_uDataInfo.m_sETHERNETMsg.m_ucSourceMAC);
	strcpy(sRawMsg.chDestMAC, (char*)sg_sEthenetData.m_uDataInfo.m_sETHERNETMsg.m_ucDestMac);
	

	EMSGDETAILS sEthernetMsg;
	if(sg_pBaseAppLayProtocol != NULL)
	{
		sEthernetMsg =  sg_pBaseAppLayProtocol->ParseEthernetData(sRawMsg);
	}
	else
	{
		strcpy((char*)sEthernetMsg.ucMsgName,"From BUSMASTER");
		sEthernetMsg.lMsgID = 1234;
		sEthernetMsg.nDatalength = 5;
		strcpy((char*)sEthernetMsg.ucData , "abcde");
		strcpy((char*)sEthernetMsg.ucProtocolName , "From BUSMASTER");
	}

	//convert the EMSGDETAILS structure to form the STETHERNETDATA structure
	FillEthernetDataFromMsgDetails(sg_sEthenetData.m_uDataInfo.m_sETHERNETMsg, sEthernetMsg);

	vWriteIntoClientsBuffer(sg_sEthenetData);
}

/**
 * Read thread procedure
 */
DWORD WINAPI EthernetMsgReadThreadProc(LPVOID pVoid)
{
	USES_CONVERSION;

    CPARAM_THREADPROC* pThreadParam = (CPARAM_THREADPROC*) pVoid;

    // Validate certain required pointers
    VALIDATE_POINTER_RETURN_VALUE_LOG(pThreadParam, (DWORD)-1);	
    // Assign thread action to CREATE_TIME_MAP
    pThreadParam->m_unActionCode = INVOKE_FUNCTION;

	//Set event object for reading from the socket
	//SOCKET ClientSocket = sg_Connector.GetClientSocket();
	//int rc = WSAEventSelect(ClientSocket, g_hReadEventObject, FD_READ);
	int rc = 0;
	if(rc == SOCKET_ERROR)
	{
		//unable to add event for reading
		return 1;
	}

	HANDLE hDataEvent = sg_Connector.GetEvent();
	HANDLE hEvents[] = {sg_hDataReceivedEvent, g_hInformStopTx};
	bool bLoopON = true;
	sg_Connector.SetReceiveCallBackFn();
	PPKTHDR *pHeader = new PPKTHDR;
	u_char *pkt_data = new u_char[1540];

    while (bLoopON)
	{
		//g_hReadEventObject is the event triggered on reception of the Data in ethernet port
		//Event should listen to the client socket in CConnector class
		
		WaitForMultipleObjects(2, hEvents, FALSE, INFINITE);
		//WaitForSingleObject(sg_hDataReceivedEvent, INFINITE);

		if(sg_bIsConnected == false)
		{
			pThreadParam->m_unActionCode = EXIT_THREAD;
			break;
		}
		switch (pThreadParam->m_unActionCode)
		{
		case INVOKE_FUNCTION:
			{
				int nResult = 0;
				do {

					string strError;
					
					nResult = sg_Connector.ReceivePackets(&pHeader,&pkt_data);
					
					if(nResult == S_FALSE)
					{
						sg_pIlog->vLogAMessage(A2T(__FILE__), __LINE__, strError);
						break;
					}
					ProcessMsg(pHeader,pkt_data );
					ResetEvent(sg_hDataReceivedEvent);
					//delete(pHeader);
					//delete(pkt_data);
				} while (nResult > 0);
			
			}
			break;
		case EXIT_THREAD:
			{
				bLoopON = false;
			}
			break;
		case CREATE_TIME_MAP:
			{
				SetEvent(pThreadParam->m_hActionEvent);
				pThreadParam->m_unActionCode = INVOKE_FUNCTION;
			}
			break;
		default:
		case INACTION:
			{
				// nothing right at this moment
			}
			break;
		}
	}
	/*delete(pHeader);
		delete(pkt_data);*/
	

	SetEvent(pThreadParam->hGetExitNotifyEvent());
	ResetEvent(sg_hDataReceivedEvent);
	//g_hReadEventObject = NULL;
	/* for (UINT i = 0; i < sg_unClientCnt; i++)
	{
        ResetEvent(hReadEventObject);
        g_hDataEvent[i] = NULL;
    }*/
    pThreadParam->m_hActionEvent = NULL;

    return 0;
}

HRESULT LoadCustomProtocolDll(string strDllPath)
{
	if(sg_hDll != NULL)
	{
		//Clear previously loaded library
		FreeLibrary(sg_hDll);
		sg_pBaseAppLayProtocol = NULL;
	}

	sg_hDll = LoadLibrary(strDllPath.c_str());
	
	if (sg_hDll == NULL)
	{
		DWORD dError =  GetLastError();
		AfxMessageBox("Error loading custom dll. Error code %ld", dError);
		return S_FALSE;
	}
	else
	{
		sg_pIlog->vLogAMessage(A2T(__FILE__), __LINE__, _T(_("Load library successful...") ));
		pfGetCustomProtocolInterface = (GET_CUSTOM_PROTOCOL_INTERFACE)GetProcAddress(sg_hDll, "GetCustomAppLayerProtocol");
		
		if(pfGetCustomProtocolInterface == NULL)
		{
			DWORD dError =  GetLastError();
			AfxMessageBox("Unable to get the base interface from the Custom protocol dll. Frames cannot be interpreted. Error code %ld", dError);
			return S_FALSE;
		}

		pfGetCustomProtocolInterface((void**)&sg_pBaseAppLayProtocol);
		if(sg_pBaseAppLayProtocol == NULL)
		{
			AfxMessageBox("Unable to get the base interface from the Custom protocol dll. Frames cannot be interpreted.");
			return S_FALSE;
		}
	}
	return S_OK;
}
/*----------------------------------------------------------------------*/

HRESULT CDIL_Ethernet_Interface::Ethernet_LoadDriverLibrary(void)
{
	//No library is need to be loaded, hence return ok.
	return S_OK;
}

HRESULT CDIL_Ethernet_Interface::Ethernet_UnloadDriverLibrary(void)
{
	//No library has been loaded, hence return ok.
	return S_OK;
}

/**
 * \return S_OK for success, S_FALSE for failure
 *
 * Returns the interface to controller
 */

HRESULT CDIL_Ethernet_Interface::Ethernet_SetAppParams(HWND hWndOwner, Base_WrapperErrorLogger* pILog)
{
	sg_hOwnerWnd = hWndOwner;
	sg_pIlog = pILog;
	 GetLocalTime(&sg_CurrSysTime);
	//Ethernet_ManageMsgBuf(MSGBUF_CLEAR, NULL, NULL);
	return S_OK;
}

USAGEMODE HRESULT GetIDIL_Ethernet_Controller(void** ppvInterface)
{
    HRESULT hResult = S_OK;
    if ( NULL == sg_pouDIL_Ethernet_Interface )
    {
        if ((sg_pouDIL_Ethernet_Interface = new CDIL_Ethernet_Interface) == NULL)
        {
            hResult = S_FALSE;
        }
    }
    *ppvInterface = (void*) sg_pouDIL_Ethernet_Interface;  /* Doesn't matter even if sg_pouDIL_CAN_Kvaser is null */

    return hResult;
}


HRESULT CDIL_Ethernet_Interface::Ethernet_PerformInitOperations(void) 
{
	g_hInformStopTx = CreateEvent(NULL, FALSE, FALSE, NULL);
	return /*hResult*/ S_OK;
}

HRESULT CDIL_Ethernet_Interface::Ethernet_PerformClosureOperations(void) 
{
	//HRESULT hResult = sg_Connector.ShutDownSocket();
	/*if(hResult == S_FALSE)
	{
		 sg_pIlog->vLogAMessage(A2T(__FILE__), __LINE__, "Unable to Close Socket");
	}
	return hResult;*/
	return S_OK;
}

HRESULT CDIL_Ethernet_Interface::Ethernet_GetTimeModeMapping(SYSTEMTIME& CurrSysTime, UINT64& TimeStamp, LARGE_INTEGER* QueryTickCount = NULL) 
{
	memcpy(&CurrSysTime, &sg_CurrSysTime, sizeof(SYSTEMTIME));
	sg_qwRefSysTime = (CurrSysTime.wHour * 3600 + CurrSysTime.wMinute * 60 +
                      + CurrSysTime.wSecond) * 10000 + CurrSysTime.wMilliseconds * 10;
    TimeStamp = sg_TimeStamp;
    if(QueryTickCount != NULL)
    {
        *QueryTickCount = sg_QueryTickCount;
    }
    return S_OK;
}

HRESULT CDIL_Ethernet_Interface::Ethernet_DisplayConfigDlg(PSCONTROLLER_DETAILS InitData, int& Length) 
{
	//TODO: Display configuration Dlg
	return S_OK;
}

HRESULT CDIL_Ethernet_Interface:: Ethernet_SetConfigData(PSCONTROLLER_DETAILS InitData, int Length) 
{
	// TODO: Set the configuration
	return S_OK;
}


HRESULT CDIL_Ethernet_Interface::  Ethernet_StartHardware(void) 
{
	sg_bCurrState = STATE_HW_INTERFACE_SELECTED;   //TODO: remove this line, added just to test
	VALIDATE_VALUE_RETURN_VAL(sg_bCurrState, STATE_HW_INTERFACE_SELECTED, ERR_IMPROPER_STATE);

    USES_CONVERSION;
    HRESULT hResult = S_OK;
	ResetEvent(g_hInformStopTx);
    //Connect to the network
    hResult = nConnect(TRUE);
    if (hResult == defERR_OK)
    {
        hResult = S_OK;
        sg_bCurrState = STATE_CONNECTED;
       // vCreateTimeModeMapping(g_hReadEventObject);
    }
    else
    {
        //log the error for open port failure
        vRetrieveAndLog(hResult, __FILE__, __LINE__);
        hResult = ERR_LOAD_HW_INTERFACE;
    }

    //If everything is ok start the read thread
    if (hResult == S_OK)
    {
        if (sg_sParmRThread.bStartThread(EthernetMsgReadThreadProc))
        {
            hResult = S_OK;
        }
        else
        {
            sg_pIlog->vLogAMessage(A2T(__FILE__), __LINE__, _T(_("Could not start the read thread") ));
        }
    }

    return hResult;
}

HRESULT CDIL_Ethernet_Interface::  Ethernet_StopHardware(void) 
{
	VALIDATE_VALUE_RETURN_VAL(sg_bCurrState, STATE_CONNECTED, ERR_IMPROPER_STATE);

    HRESULT hResult = S_OK;

    //Terminate the read thread
    sg_sParmRThread.bTerminateThread();
	SetEvent(g_hInformStopTx);
    hResult = nConnect(FALSE);
    if (hResult == defERR_OK)
    {
        hResult = S_OK;
        sg_bCurrState = STATE_HW_INTERFACE_SELECTED;
    }
    else
    {
        //log the error for open port failure
        vRetrieveAndLog(hResult, __FILE__, __LINE__);
        hResult = ERR_LOAD_HW_INTERFACE;
    }

    return hResult;
}

HRESULT CDIL_Ethernet_Interface:: Ethernet_SendMsg(DWORD dwClientID, const STETHERNET_MSG& sMessage) 
{
	 VALIDATE_VALUE_RETURN_VAL(sg_bCurrState, STATE_CONNECTED, ERR_IMPROPER_STATE);
    static SACK_MAP sAckMap;
    HRESULT hResult = S_FALSE;
    if (bClientIdExist(dwClientID))
    {
        if (sMessage.m_ucChannel <= sg_ucNoOfHardware)
        {
            sAckMap.m_ClientID = dwClientID;
            sAckMap.m_MsgID = sMessage.m_unMsgID;
            sAckMap.m_Channel = sMessage.m_ucChannel;
            /*Mark an entry in Map. This is helpful to idendify
              which client has been sent this message in later stage*/
            vMarkEntryIntoMap(sAckMap);
            if (nWriteMessage(sMessage) == defERR_OK)
            {
                hResult = S_OK;
            }
        }
        else
        {
            hResult = ERR_INVALID_CHANNEL;
        }
    }
    else
    {
        hResult = ERR_NO_CLIENT_EXIST;
    }
    return hResult;
}

HRESULT CDIL_Ethernet_Interface::Ethernet_GetLastErrorString(string& acErrorStr) 
{
	return S_OK;
}

HRESULT CDIL_Ethernet_Interface::Ethernet_SetControllerParams(int nValue, ECONTR_PARAM eContrparam) 
{
	return S_OK;
}

HRESULT CDIL_Ethernet_Interface::Ethernet_GetErrorCount(SERROR_CNT& sErrorCnt, UINT nChannel, ECONTR_PARAM eContrParam) 
{
	return S_OK;
}

// Specific function set
HRESULT CDIL_Ethernet_Interface::Ethernet_ManageMsgBuf(BYTE bAction, DWORD ClientID, CBaseEthernetBufFSE* pBufObj) 
{
	 HRESULT hResult = S_FALSE;
    if (ClientID != NULL)
    {
        UINT unClientIndex;
        if (bGetClientObj(ClientID, unClientIndex))
        {
            SCLIENTBUFMAP& sClientObj = sg_asClientToBufMap[unClientIndex];
            if (bAction == MSGBUF_ADD)
            {
                //Add msg buffer
                if (pBufObj != NULL)
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
                //clear msg buffer
                if (pBufObj != NULL) //REmove only buffer mentioned
                {
                    bRemoveClientBuffer(sClientObj.pClientBuf, sClientObj.unBufCount, pBufObj);
                }
                else //Remove all
                {
                    for (UINT i = 0; i < sClientObj.unBufCount; i++)
                    {
                        sClientObj.pClientBuf[i] = NULL;
                    }
                    sClientObj.unBufCount = 0;
                }
                hResult = S_OK;
            }
            else
            {
                ////ASSERT(FALSE);
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
            //clear msg buffer
            for (UINT i = 0; i < sg_unClientCnt; i++)
            {
                Ethernet_ManageMsgBuf(MSGBUF_CLEAR, sg_asClientToBufMap[i].dwClientID, NULL);
            }
            hResult = S_OK;
        }
    }

    return hResult;
}

HRESULT CDIL_Ethernet_Interface::Ethernet_RegisterClient(BOOL bRegister, DWORD& ClientID, char* pacClientName) 
{
	USES_CONVERSION;
    HRESULT hResult = S_FALSE;
    if (bRegister)
    {
        if (sg_unClientCnt < MAX_CLIENT_ALLOWED)
        {
            INT Index = 0;
            if (!bClientExist(pacClientName, Index))
            {
                //Currently store the client information
                if (_tcscmp(pacClientName, ETHERNET_MONITOR_NODE) == 0)
                {
                    //First slot is reserved to monitor node
                    ClientID = 1;
                    _tcscpy(sg_asClientToBufMap[0].pacClientName, pacClientName);
                    sg_asClientToBufMap[0].dwClientID = ClientID;
                    sg_asClientToBufMap[0].unBufCount = 0;
                }
                else
                {
                    if (!bClientExist(ETHERNET_MONITOR_NODE, Index))
                    {
                        Index = sg_unClientCnt + 1;
                    }
                    else
                    {
                        Index = sg_unClientCnt;
                    }
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

HRESULT CDIL_Ethernet_Interface::Ethernet_GetCntrlStatus(const HANDLE& hEvent, UINT& unCntrlStatus) 
{
	return S_OK;
}

HRESULT  CDIL_Ethernet_Interface::Ethernet_ListHwInterfaces(INTERFACE_HW_LIST& sSelHwInterface, INT& nCount)
{
	
	pcap_if_t *pAdapter = nullptr;
	pcap_if_t* pAdapters =  sg_Connector.SearchAdapters();
	list<string> stringList;

	int  nCnt =0;
	
	
	for(pAdapter = pAdapters; pAdapter; pAdapter = pAdapter->next, nCnt++)
	{
		string strApafterName;
		if(pAdapter == NULL)
		{
			break;
		}
		if (pAdapter->description)
		{
			//contains the name in human readable form
			strApafterName =  pAdapter->description;
		}
		else
		{
			//contains hardware name in computer understandable form
			strApafterName = pAdapter->name;
		}
		stringList.push_back(strApafterName);
	}

	
	CEConfigDlg   oConfigDlg(stringList, nCnt);
	//Call the UI for selection of the available adapters
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	if(oConfigDlg.DoModal() == IDOK)
	{
		int nSel = oConfigDlg.m_nSel;
		string strFilter = oConfigDlg.m_strFilter;
		string strDllPath = oConfigDlg.m_strDllPath;
		/* Jump to the selected adapter */
		int i=0;
		for(pAdapters = pAdapters; i< nSel-1 ;pAdapters = pAdapters->next, i++);

		if(pAdapters != NULL)
		{
			sg_pSelectedAdapter = pAdapters;
			sg_Connector.ConnectToAdapter(sg_pSelectedAdapter);
			if(strFilter.length() > 0)
			{
				sg_Connector.SetFilter(strFilter);
			}	
			else
			{
				AfxMessageBox("Filter is not set, hence all the messages will be captured");
			}

			if(strDllPath.length() > 0)
			{
				LoadCustomProtocolDll(strDllPath);
			}
			else
			{
				AfxMessageBox("Please load the Custom dll for frame interpretation else default dll will loaded");
			}
		}
	}
	
	return S_OK;
}

HRESULT  CDIL_Ethernet_Interface::Ethernet_SelectHwInterface(const INTERFACE_HW_LIST& sSelHwInterface, INT nCount)
{
	return S_OK;
}

HRESULT  CDIL_Ethernet_Interface::Ethernet_DeselectHwInterface(void)
{
	return S_OK;
}

HMODULE CDIL_Ethernet_Interface::Ethernet_GetCustomProtocolHandle()
{
	return sg_hDll;
}