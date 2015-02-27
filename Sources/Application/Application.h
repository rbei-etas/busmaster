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
 * \file      Application.h
 * \brief     CCANMonitorApp class implementation file
 * \author    Anish kumar
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * CCANMonitorApp class implementation file
 */

#pragma once

// For DIL Interface Class
#include "Include/BaseDefs.h"
// License validator interface, needed to query for other interfaces
#include "PSDI_Extern.h"
#include "CommonClass/MsgContainerBase.h"
//#include "DataTypes/MsgBufAll_Datatypes.h"
//#include "DataTypes/DIL_Datatypes.h"
//#include "DIL_Interface/BaseDIL_CAN.h"

// For CAN logger interface
#include "include/ModuleID.h"
#include "FrameProcessor/FrameProcessor_extern.h"
#include "FrameProcessor/BaseFrameProcessor_CAN.h"
#include "Struct.h"
#include "Utility/Utility.h"
#include "BUSMASTER_Interface.h"
#include "BUSMASTER.h"
#include "MainFrm.h"
#include "DIL_interface/BaseDIL_CAN.h"

#include "InterfaceGetter.h"

extern CCANMonitorApp theApp;

extern SBUSSTATISTICS g_sBusStatistics[ defNO_OF_CHANNELS ];
CCANBufFSE g_ouCanBufForCOM;
CPARAM_THREADPROC g_ouCOMReadThread;
static HANDLE g_hndPIPE[8]      = {NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL};
static HANDLE g_hndEvent[8]     = {NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL};
static SHORT  g_shUniqueID[8]   = {-1,-1,-1,-1,-1,-1,-1,-1};
static BYTE   g_bytClientCount  = 0;
static bool   g_bInitCOMThread = false;

/* PIPE related declarations */
#define BASE_PIPENAME   "\\\\.\\Pipe\\"
#define PIPE_TIMEOUT    500
#define PIPE_BUFFER_SIZE 131072
#define PASSTHRUMSG_SIZE sizeof(CAN_MSGS)

class CApplication : public CCmdTarget
{
public:
    CApplication(void);
    ~CApplication(void);

    virtual void OnFinalRelease()
    {
        CCmdTarget::OnFinalRelease();
    }

    void vInitializeCOMReadBuffer();
    BOOL bStartCOMReadThread();
    BOOL bStopCOMReadThread();
    void ReadCOMDataBuffer();

    DECLARE_DYNCREATE(CApplication)
    DECLARE_MESSAGE_MAP()
    DECLARE_OLECREATE(CApplication)
    DECLARE_DISPATCH_MAP()
    DECLARE_INTERFACE_MAP()

protected:
    // Connection point for ISMCLink interface
    BEGIN_CONNECTION_PART(CApplication, AppEvents)
    CONNECTION_IID(IID__IAppEvents)
    END_CONNECTION_PART(AppEvents)

    DECLARE_CONNECTION_MAP()

public:
    enum
    {
        dispidConnect                      =               1L,
        dispidGetMsgInfo                   =               2L,
        dispidGetNetworkStatistics         =               3L,
        dispidGetErrorCounter              =               4L,
        dispidImportDatabase               =               5L,
        dispidLoadConfiguration            =               6L,
        dispidSaveConfiguration            =               7L,
        dispidSaveConfigurationAs          =               8L,
        dispidRegisterClientForRx          =               9L,
        dispidUnRegisterClient             =               10L
                //dispidGetApplication             =               11L
    };


    /******************************************************************************
        Function Name    : Connect

        Input(s)         :  BOOL
        Output           :  -
        Functionality    :  Connect/disconnect the tool
        Member of        :  CApplication
        Author(s)        :  Anish kumar
        Date Created     :  20.06.2006
        Modifications    :
    ******************************************************************************/
    HRESULT Connect(BOOL bConnect)
    {
        HRESULT hResult = E_POINTER;
        CFlags* pFlags = GetIFlags();
        CMainFrame* pMainFrm = GetIMainFrame();

        if ((NULL != pFlags) && (NULL != pMainFrm))
        {
            if ((BOOL) pFlags->nGetFlagStatus(CONNECTED) != bConnect)
            {
                if ( bConnect )
                {
                    vInitializeCOMReadBuffer();
                }
                pMainFrm->COM_ConnectTool();
                hResult = S_OK;
            }
            else
            {
                hResult = E_FAIL;
            }
        }

        return hResult;
    }

    /******************************************************************************
        Function Name    :  GetErrorCounter

        Input(s)         :
        Output           :  -
        Functionality    :  Provide's the error count
        Member of        :  CApplication
        Author(s)        :  Anish kumar
        Date Created     :  20.06.2006
        Modifications    :
    ******************************************************************************/
    HRESULT GetErrorCounter(UCHAR* Tx, UCHAR* Rx, INT nChannel)
    {
        HRESULT hResult = E_POINTER;
        CBaseDIL_CAN* pDIL_CAN = GetICANDIL();

        if (NULL != pDIL_CAN)
        {
            SERROR_CNT sErrorCount;
            hResult = pDIL_CAN->DILC_GetErrorCount(sErrorCount, nChannel, ERR_CNT);

            if (S_OK == hResult)
            {
                *Tx = sErrorCount.m_ucTxErrCount;
                *Rx = sErrorCount.m_ucRxErrCount;
            }
            else
            {
                hResult = E_FAIL;
            }
        }

        return hResult;
    }

    /******************************************************************************
        Function Name    :  vSendCANMsgToClients

        Input(s)         :  message details
        Output           :  -
        Functionality    :  sends the message to registered clients
        Member of        :  CApplication
        Author(s)        :  Arunkumar Karri
        Date Created     :  08.01.2013
        Modifications    :
    ******************************************************************************/
    void vSendCANMsgToClients(CAN_MSGS sMsg)
    {
        const CPtrArray* pConnections = m_xAppEvents.GetConnections ();
        ASSERT (pConnections != NULL);
        int nConnections = pConnections->GetSize ();
        if (nConnections)
        {
            for (int i=0; i<nConnections; i++)
            {
                _IAppEvents* pInterface = (_IAppEvents*) (pConnections->GetAt (i));
                ASSERT (pInterface != NULL);

                // Outgoing!
                HRESULT hr = pInterface->OnMessageReceived (sMsg);
                hr = S_OK;
            }
        }
    }

    /******************************************************************************
        Function Name    :  GetMsgInfo

        Input(s)         :  message name
        Output           :  message structure
        Functionality    :  Provides the Message structure
        Member of        :  CApplication
        Author(s)        :  Anish kumar
        Date Created     :  20.06.2006
        Modifications    :
    ******************************************************************************/
    HRESULT GetMsgInfo(BSTR MsgName, sMESSAGESTRUCT* psMsgStruct)
    {
        HRESULT hResult = E_POINTER;
        CMsgSignal* pMsgSignal = GetIMsgDB();

        if (NULL != pMsgSignal)
        {
            char acStr[256] = {'\0'};
            BSTR_2_PCHAR(MsgName, acStr, 256);
            CString omMsgName(acStr);
            sMESSAGE* psMessage = pMsgSignal->psGetMessagePointer(omMsgName);

            if (NULL != psMessage)
            {
                psMsgStruct->m_dMessageCode = psMessage->m_unMessageCode;
                psMsgStruct->m_dNumberOfSignals = psMessage->m_unNumberOfSignals;
                psMsgStruct->m_dMessageLength = psMessage->m_unMessageLength;
                psMsgStruct->m_bMessageFrameFormat = psMessage->m_bMessageFrameFormat;
                psMsgStruct->m_nMsgDataFormat = psMessage->m_nMsgDataFormat;
                hResult = S_OK;
            }
        }

        return hResult;
    }

    /******************************************************************************
        Function Name    :  GetNetworkStatistics

        Input(s)         :  Channel no., statistic structure
        Output           :  Statistics of the channel
        Functionality    :  Provides all the statistics
        Member of        :  CApplication
        Author(s)        :  Anish kumar
        Date Created     :  20.06.2006
        Modifications    :
    ******************************************************************************/
    HRESULT GetNetworkStatistics(int nChannel, sBUSSTATISTICS_USR* psStat)
    {
        HRESULT hResult = E_POINTER;
        CBaseBusStatisticCAN* pBusStatCAN = GetICANBusStat();

        if ((NULL != pBusStatCAN) && (NULL != psStat))
        {
            if (nChannel >= 1)
            {
                nChannel--; // To access data from 0th index
            }
            SBUSSTATISTICS sBusStat = {'\0'};
            hResult = pBusStatCAN->BSC_GetBusStatistics((UINT) nChannel, sBusStat);
            psStat->m_dAvarageBusLoad = sBusStat.m_dAvarageBusLoad;
            psStat->m_dBaudRate = sBusStat.m_dBaudRate;
            psStat->m_dBusLoad = sBusStat.m_dBusLoad;
            psStat->m_unTotalMsgCount = sBusStat.m_unTotalMsgCount;
            psStat->m_unTotalTxMsgCount = sBusStat.m_unTotalTxMsgCount;
            psStat->m_unTotalRxMsgCount = sBusStat.m_unTotalRxMsgCount;
            psStat->m_dErrorRate = sBusStat.m_dErrorRate;
            psStat->m_dErrorRxRate = sBusStat.m_dErrorRxRate;
            psStat->m_dErrorTxRate = sBusStat.m_dErrorTxRate;
            psStat->m_dPeakBusLoad = sBusStat.m_dPeakBusLoad;
            psStat->m_dRxEXTMsgRate = sBusStat.m_dRxEXTMsgRate;
            psStat->m_dRxSTDMsgRate = sBusStat.m_dRxSTDMsgRate;
            psStat->m_dTotalBusLoad = sBusStat.m_dTotalBusLoad;
            psStat->m_dTotalRxMsgRate = sBusStat.m_dTotalRxMsgRate;
            psStat->m_dTotalTxMsgRate = sBusStat.m_dTotalTxMsgRate;
            psStat->m_dTxEXTMsgRate = sBusStat.m_dTxEXTMsgRate;
            psStat->m_dTxSTDMsgRate = sBusStat.m_dTxSTDMsgRate;
            psStat->m_nSamples = sBusStat.m_nSamples;
            psStat->m_ucRxErrorCounter = sBusStat.m_ucRxErrorCounter;
            psStat->m_ucRxPeakErrorCount = sBusStat.m_ucRxPeakErrorCount;
            psStat->m_ucStatus = sBusStat.m_ucStatus;
            psStat->m_ucTxErrorCounter = sBusStat.m_ucTxErrorCounter;
            psStat->m_ucTxPeakErrorCount = sBusStat.m_ucTxPeakErrorCount;
            psStat->m_unDLCCount = sBusStat.m_unDLCCount;
            psStat->m_unErrorRxCount = sBusStat.m_unErrorRxCount;
            psStat->m_unErrorTotalCount = sBusStat.m_unErrorTotalCount;
            psStat->m_unErrorTxCount = sBusStat.m_unErrorTxCount;
            psStat->m_unMsgPerSecond = sBusStat.m_unMsgPerSecond;
            psStat->m_unRxEXTDMsgCount = sBusStat.m_unRxEXTDMsgCount;
            psStat->m_unRxEXTD_RTRMsgCount = sBusStat.m_unRxEXTD_RTRMsgCount;
            psStat->m_unTxSTDMsgCount = sBusStat.m_unTxSTDMsgCount;
            psStat->m_unTxSTD_RTRMsgCount = sBusStat.m_unTxSTD_RTRMsgCount;
            psStat->m_unTxEXTDMsgCount = sBusStat.m_unTxEXTDMsgCount;
            psStat->m_unRxSTDMsgCount = sBusStat.m_unRxSTDMsgCount;
            psStat->m_unRxSTD_RTRMsgCount = sBusStat.m_unRxSTD_RTRMsgCount;
            /*memcpy(psStat, &sBusStat, sizeof(SBUSSTATISTICS));*/
        }

        return hResult;
    }

    // CONFIGURATION RELATED FUNCTIONS: START

    /******************************************************************************
        Function Name    :  LoadConfiguration

        Input(s)         :
        Output           :  -
        Functionality    :  Calls the mainframe class function to load the
                            configuration
        Member of        :  CApplication
        Author(s)        :  Anish kumar
        Date Created     :  20.06.2006
        Modifications    :
    ******************************************************************************/
    HRESULT LoadConfiguration(BSTR FileName)
    {
        HRESULT hResult = E_POINTER;
        CFlags* pFlags = GetIFlags();
        CMainFrame* pMainFrm = GetIMainFrame();

        if ((NULL != pFlags) && (NULL != pMainFrm))
        {
            if ((BOOL) pFlags->nGetFlagStatus(CONNECTED) == FALSE) // Loading of
            {
                // configuration is done iff the present node is disconnected.
                char acStr[_MAX_PATH] = {'\0'};
                BSTR_2_PCHAR(FileName, acStr, _MAX_PATH);
                CString omFilePath(acStr);

                if (pMainFrm->nLoadConfigFile(omFilePath) == 1)
                {
                    hResult = S_OK;
                }
                else
                {
                    hResult = E_FAIL;
                }
            }
            else
            {
                hResult = E_FAIL;
            }
        }

        return hResult;
    }

    /******************************************************************************
        Function Name    :  ImportDatabase

        Input(s)         :  File path
        Output           :  -
        Functionality    :  import the database file
        Member of        :  CApplication
        Author(s)        :  Anish kumar
        Date Created     :  20.06.2006
        Modifications    :
    ******************************************************************************/
    HRESULT ImportDatabase(BSTR DBFilePath)
    {
        HRESULT hResult = E_POINTER;
        CMainFrame* pMainFrm = GetIMainFrame();

        if (NULL != pMainFrm)
        {
            char acStr[_MAX_PATH] = {'\0'};
            BSTR_2_PCHAR(DBFilePath, acStr, _MAX_PATH);
            CString omFilePath(acStr);
            hResult = pMainFrm->dLoadDataBaseFile(omFilePath, TRUE);
        }

        return hResult;
    }

    /******************************************************************************
        Function Name    :  SaveConfiguration

        Input(s)         :  -
        Output           :  -
        Functionality    :  Save the Configurations in file
        Member of        :  CApplication
        Author(s)        :  Anish kumar
        Date Created     :  25.09.2006
        Modifications    :
    ******************************************************************************/
    HRESULT SaveConfiguration()
    {
        HRESULT hResult = E_POINTER;
        CMainFrame* pMainFrm = GetIMainFrame();

        if (NULL != pMainFrm)
        {
            if (pMainFrm->COM_SaveConfigFile())
            {
                hResult = S_OK;
            }
            else
            {
                hResult = E_FAIL;
            }
        }

        return hResult;
    }

    /******************************************************************************
        Function Name    :  SaveConfigurationAs

        Input(s)         :  Config File path
        Output           :
        Functionality    :  Save the configuration to new file
        Member of        :  CApplication
        Author(s)        :  Anish kumar
        Date Created     :  01.06.2010
        Modifications    :
    ******************************************************************************/
    HRESULT SaveConfigurationAs(BSTR ConfigPath)
    {
        char acStr[_MAX_PATH] = {'\0'};
        BSTR_2_PCHAR(ConfigPath, acStr, _MAX_PATH);
        CString omConfigPath(acStr);
        HRESULT hResult = E_FAIL;

        if (theApp.COM_nSaveConfiguration(omConfigPath) == defCONFIG_FILE_SUCCESS)
        {
            hResult = S_OK;
        }

        return hResult;
    }
    // CONFIGURATION RELATED FUNCTIONS: END

    /******************************************************************************
        Function Name    :  RegisterClientForRx

        Input(s)         :  USHORT usUniqueID, BSTR* pEventName, BSTR* pPIPEName
        Output           :
        Functionality    :  Registers a client for receiving Rx data through PIPE and event
        Member of        :  CApplication
        Author(s)        :  ArunKumar Karri
        Date Created     :  01.24.2013
        Modifications    :
    ******************************************************************************/
    HRESULT RegisterClientForRx(USHORT usUniqueID, BSTR* pEventName, BSTR* pPIPEName)
    {
        HRESULT hResult = S_OK;

        /* if not more than 8 clients connected for Rx */
        if ( g_bytClientCount < 8 )
        {
            for ( int i = 0; i < 8 ; i++ )
            {
                if ( g_shUniqueID[i] == -1 )
                {
                    hResult = hCreatePipeAndEventForClient(usUniqueID, g_hndPIPE[i], g_hndEvent[i], pEventName, pPIPEName);
                    g_shUniqueID[i] = usUniqueID;

                    g_bytClientCount++;
                    break;
                }
            }
        }
        else
        {
            hResult = S_FALSE;
        }

        return hResult;
    }

    /******************************************************************************
        Function Name    :  UnRegisterClient

        Input(s)         :  USHORT usUniqueID
        Output           :
        Functionality    :  Unregisters a client for receiving Rx data
        Member of        :  CApplication
        Author(s)        :  ArunKumar Karri
        Date Created     :  01.25.2013
        Modifications    :
    ******************************************************************************/
    HRESULT UnRegisterClient(USHORT usUniqueID)
    {
        HRESULT hResult = S_OK;

        for ( int i = 0; i < 8 ; i++ )
        {
            /* if the client ID exists */
            if ( usUniqueID == g_shUniqueID[i] )
            {
                /* Close all communication objects */

                DisconnectNamedPipe(g_hndPIPE[i]);
                CloseHandle(g_hndPIPE[i]);
                g_hndPIPE[i] = NULL;

                CloseHandle(g_hndEvent[i]);
                g_hndEvent[i] = NULL;

                g_shUniqueID[i] = -1;
                g_bytClientCount--;

                break;
            }
            /* if not found */
            else if ( i == 7 )
            {
                hResult = S_FALSE;
            }
        }


        return hResult;
    }

    /* Function to create PIPE handle for a client */
    HRESULT  hCreatePipeAndEventForClient(USHORT usUniqueID, HANDLE& hndPipe, HANDLE& hndEvent, BSTR* pEventName, BSTR* pPIPEName)
    {
        char EventName[32] = {'\0'};
        sprintf_s(EventName, "%X", usUniqueID);
        // followed by the pipe name
        char PipeName[64] = BASE_PIPENAME;
        strcat_s(PipeName, EventName);

        // Pipe name; convert from ASCII string to BSTR
        BSTR bstrPipe = A2BSTR(PipeName);
        *pPIPEName = bstrPipe;
        // Mutex name; convert from ASCII string to BSTR
        BSTR bstrEvent = A2BSTR(EventName);
        *pEventName = bstrEvent;

        bool bProceed = ((bstrPipe != NULL) && (bstrEvent != NULL));

        static HANDLE hEvent = NULL;

        if ( bProceed )
        {
            /* Create named pipe for client for writing*/
            hndPipe = CreateNamedPipe(
                          PipeName,                 // pipe name
                          PIPE_ACCESS_DUPLEX | FILE_FLAG_OVERLAPPED,     // read/write access
                          PIPE_TYPE_BYTE |          // message type pipe
                          PIPE_READMODE_BYTE |      // message-read mode
                          PIPE_NOWAIT,              // blocking mode
                          PIPE_UNLIMITED_INSTANCES, // max. instances
                          PIPE_BUFFER_SIZE,         // output buffer size
                          PASSTHRUMSG_SIZE,         // input buffer size
                          PIPE_TIMEOUT,             // client time-out
                          NULL);                    // no security attribute

            /* Set timeouts */
            COMMTIMEOUTS CommTimeouts;
            //BOOL   bStatus;

            CommTimeouts.ReadIntervalTimeout         = 0;
            CommTimeouts.ReadTotalTimeoutMultiplier  = 0;
            CommTimeouts.ReadTotalTimeoutConstant    = 1;
            CommTimeouts.WriteTotalTimeoutMultiplier = 0;
            CommTimeouts.WriteTotalTimeoutConstant   = 1000;

            //bStatus = SetCommTimeouts(hndPipe,&CommTimeouts);

            /* Create event */
            BOOL bRet = ConnectNamedPipe(hndPipe, NULL);
            /*if ( !bRet )
                return false;*/

            // Generate the communication event
            bProceed = bProceed && ((hndEvent= CreateEvent(NULL, FALSE, FALSE, EventName)) != NULL);

            if ( !bProceed )
            {
                return S_FALSE;
            }
        }
        else
        {
            return S_OK;
        }

        return S_OK;
    }

    /******************************************************************************
        Function Name    :  GetApplication

        Input(s)         :  -
        Output           :
        Functionality    :  Retrieves the application pointer
        Member of        :  CApplication
        Author(s)        :  Saravanan
        Date Created     :  30.07.2014
        Modifications    :
    ******************************************************************************/
    /*HRESULT GetApplication(IBusMaster** pApplication)
    {
        return E_NOTIMPL;
    }*/

    ////////////////////////////////////////////////////////////////////////////////

    BEGIN_INTERFACE_PART(LocalClass, IBusMasterApp)
    STDMETHOD(GetTypeInfoCount)(UINT FAR* pctinfo);
    STDMETHOD(GetTypeInfo)(
        UINT itinfo,
        LCID lcid,
        ITypeInfo FAR* FAR* pptinfo);
    STDMETHOD(GetIDsOfNames)(
        REFIID riid,
        OLECHAR FAR* FAR* rgszNames,
        UINT cNames,
        LCID lcid,
        DISPID FAR* rgdispid);
    STDMETHOD(Invoke)(
        DISPID dispidMember,
        REFIID riid,
        LCID lcid,
        WORD wFlags,
        DISPPARAMS FAR* pdispparams,
        VARIANT FAR* pvarResult,
        EXCEPINFO FAR* pexcepinfo,
        UINT FAR* puArgErr);
    // General methods
    STDMETHOD(GetMsgInfo)(BSTR MsgName,sMESSAGESTRUCT*  sMsgStruct);
    STDMETHOD(GetNetworkStatistics)(int nChannel, sBUSSTATISTICS_USR* sStat);
    STDMETHOD(GetErrorCounter)(UCHAR* Tx, UCHAR* Rx, INT nChannel);
    STDMETHOD(ImportDatabase)(BSTR DBFilePath);
    STDMETHOD(LoadConfiguration)(BSTR FileName);
    STDMETHOD(RegisterClientForRx)( USHORT usUniqueID, BSTR* pEventName, BSTR* pPIPEName );
    STDMETHOD(UnRegisterClient)(USHORT usUniqueID);
    STDMETHOD(Connect)(BOOL bConnect);
    STDMETHOD(SaveConfiguration)();
    STDMETHOD(SaveConfigurationAs) (BSTR ConfigPath);
    //STDMETHOD(GetApplication) (IBusMaster** pApplication);
    END_INTERFACE_PART(LocalClass)
};

class CCommClass : public CCmdTarget
{
public:
    CCommClass(void);
    ~CCommClass(void);

    virtual void OnFinalRelease()
    {
        CCmdTarget::OnFinalRelease();
    }

    DECLARE_DYNCREATE(CCommClass)
    DECLARE_MESSAGE_MAP()
    DECLARE_OLECREATE(CCommClass)
    DECLARE_DISPATCH_MAP()
    DECLARE_INTERFACE_MAP()

protected:
    // Connection point for ISMCLink interface
    BEGIN_CONNECTION_PART(CCommClass, AppEvents)
    CONNECTION_IID(IID__IAppEvents)
    END_CONNECTION_PART(AppEvents)

    DECLARE_CONNECTION_MAP()

public:
    enum
    {
        dispidSendCANMSg = 1L,
        dispidGetApplication = 2L
    };

    /******************************************************************************
        Function Name    :  SendCANMSg

        Input(s)         :  message details
        Output           :  -
        Functionality    :  sends the message
        Member of        :  CApplication
        Author(s)        :  Anish kumar
        Date Created     :  20.06.2006
        Modifications    :
    ******************************************************************************/
    HRESULT SendCANMSg(CAN_MSGS* psMsg)
    {
        HRESULT hResult = E_POINTER;
        CBaseDIL_CAN* pDIL_CAN = GetICANDIL();
        CMainFrame* pMainFrm = GetIMainFrame();

        if ((NULL != pDIL_CAN) && (NULL != psMsg))
        {
            STCAN_MSG sTempMsg = {'\0'};
            sTempMsg.m_ucChannel  = psMsg->m_ucChannel;
            sTempMsg.m_ucDataLen  = psMsg->m_ucDataLen;
            sTempMsg.m_ucEXTENDED = psMsg->m_bEXTENDED;
            sTempMsg.m_ucRTR      = psMsg->m_bRTR;
            sTempMsg.m_unMsgID    = psMsg->m_unMsgID;

            // Copy the data bytes
            for (int i = 0; i < sTempMsg.m_ucDataLen; i++)
            {
                sTempMsg.m_ucData[i] = psMsg->m_ucData[i];
            }
            /* Client ID should be set to CAN_MONITOR_NODE ID */
            hResult = pDIL_CAN->DILC_SendMsg(pMainFrm->dwGetMonitorNodeClientID()
                                             , sTempMsg);
        }

        return hResult;
    }

    /******************************************************************************
    Function Name    :  GetApplication

    Input(s)         :  -
    Output           :
    Functionality    :  Retrieves the application pointer
    Member of        :  CApplication
    Author(s)        :  Saravanan
    Date Created     :  30.07.2014
    Modifications    :
    ******************************************************************************/
    HRESULT GetApplication(IBusMasterApp** pApplication)
    {
        HRESULT hResult = CoCreateInstance(CLSID_BusMasterApp, NULL,
                                           CLSCTX_LOCAL_SERVER, IID_IBusMasterApp, (void**) &pApplication);

        return hResult;
    }

    BEGIN_INTERFACE_PART(CommClass, ICANComm)
    STDMETHOD(GetTypeInfoCount)(UINT FAR* pctinfo);
    STDMETHOD(GetTypeInfo)(
        UINT itinfo,
        LCID lcid,
        ITypeInfo FAR* FAR* pptinfo);
    STDMETHOD(GetIDsOfNames)(
        REFIID riid,
        OLECHAR FAR* FAR* rgszNames,
        UINT cNames,
        LCID lcid,
        DISPID FAR* rgdispid);
    STDMETHOD(Invoke)(
        DISPID dispidMember,
        REFIID riid,
        LCID lcid,
        WORD wFlags,
        DISPPARAMS FAR* pdispparams,
        VARIANT FAR* pvarResult,
        EXCEPINFO FAR* pexcepinfo,
        UINT FAR* puArgErr);
    STDMETHOD(SendCANMSg)(CAN_MSGS* sMsg);
    STDMETHOD(GetApplication)(IBusMasterApp** pApplication);
    END_INTERFACE_PART(CommClass)

};

class CLogClass : public CCmdTarget
{
public:
    CLogClass(void);
    ~CLogClass(void);

    virtual void OnFinalRelease()
    {
        CCmdTarget::OnFinalRelease();
    }

    DECLARE_DYNCREATE(CLogClass)
    DECLARE_MESSAGE_MAP()
    DECLARE_OLECREATE(CLogClass)
    DECLARE_DISPATCH_MAP()
    DECLARE_INTERFACE_MAP()

protected:
    // Connection point for ISMCLink interface
    BEGIN_CONNECTION_PART(CLogClass, AppEvents)
    CONNECTION_IID(IID__IAppEvents)
    END_CONNECTION_PART(AppEvents)

    DECLARE_CONNECTION_MAP()

public:
    enum
    {
        dispidStopLogging                  =               1L,
        dispidStartLogging                 =               2L,
        dispidAddLoggingBlock              =               3L,
        dispidGetLoggingBlock              =               4L,
        dispidRemoveLoggingBlock           =               5L,
        dispidGetLoggingBlockCount         =               6L,
        dispidClearLoggingBlockList        =               7L,
        dispidWriteToLogFile               =               8L,
        dispidGetApplication               =               9L
    };

    /******************************************************************************
       Function Name    :  GetLoggingBlockCount

       Input(s)         :  -
       Output           :
       Functionality    :  Returns total number of logging blocks defined
       Member of        :  CApplication
       Author(s)        :  Anish kumar
       Date Created     :  01.06.2010
       Modifications    :
    ******************************************************************************/
    HRESULT GetLoggingBlockCount(USHORT* BlockCount)
    {
        HRESULT hResult = E_POINTER;
        CBaseFrameProcessor_CAN* pCANLogger = GetICANLogger();

        if (NULL != pCANLogger)
        {
            *BlockCount = pCANLogger->FPC_GetLoggingBlockCount();
            hResult = S_OK;
        }

        return hResult;
    }

    /******************************************************************************
        Function Name    :  RemoveLoggingBlock

        Input(s)         :  -
        Output           :
        Functionality    :  Deletes a logging block
        Member of        :  CApplication
        Author(s)        :  Anish kumar
        Date Created     :  01.06.2010
        Modifications    :
    ******************************************************************************/
    HRESULT RemoveLoggingBlock(USHORT BlockIndex)
    {
        HRESULT hResult = E_POINTER;
        CBaseFrameProcessor_CAN* pCANLogger = GetICANLogger();

        if (NULL != pCANLogger)
        {
            hResult = pCANLogger->FPC_RemoveLoggingBlock(BlockIndex);
        }

        return hResult;
    }

    /******************************************************************************
        Function Name    :  ClearLoggingBlockList

        Input(s)         :  -
        Output           :
        Functionality    :  Clears the logging block list
        Member of        :  CApplication
        Author(s)        :  Anish kumar
        Date Created     :  01.06.2010
        Modifications    :
    ******************************************************************************/
    HRESULT ClearLoggingBlockList(void)
    {
        HRESULT hResult = E_POINTER;
        CBaseFrameProcessor_CAN* pCANLogger = GetICANLogger();

        if (NULL != pCANLogger)
        {
            hResult = pCANLogger->FPC_ClearLoggingBlockList();
        }

        return hResult;
    }
    // LOG RELATED FUNCTIONS: END

    /******************************************************************************
        Function Name    :  AddLoggingBlock

        Input(s)         :  Log file name,time mode,numeric mode,file mode for logging
        Output           :
        Functionality    :  Add a log file
        Member of        :  CApplication
        Author(s)        :  Anish kumar
        Date Created     :  06.10.2006
        Modifications    :
    ******************************************************************************/
    HRESULT AddLoggingBlock(SLOGGINGBLOCK_USR* psLoggingBlock)
    {
        HRESULT hResult = E_POINTER;
        CBaseFrameProcessor_CAN* pCANLogger = GetICANLogger();
        SLOGINFO logInfo;
        logInfo.m_eFileMode = (eMode)psLoggingBlock->m_eFileMode;
        logInfo.m_eLogTimerMode = (eTimerMode)psLoggingBlock->m_eLogTimerMode;
        logInfo.m_eNumFormat = (eFormat)psLoggingBlock->m_eNumFormat;
        strncpy(logInfo.m_sLogFileName, (const char*)psLoggingBlock->m_sLogFileName, 256);
        logInfo.m_sLogTrigger.m_unStartID = psLoggingBlock->m_sLogTrigger.m_unStartID;
        logInfo.m_sLogTrigger.m_unStopID = psLoggingBlock->m_sLogTrigger.m_unStopID;
        logInfo.m_sLogTrigger.m_unTriggerType = (eLogTriggerState)psLoggingBlock->m_sLogTrigger.m_unTriggerType;

        if (NULL != pCANLogger)
        {
            hResult = pCANLogger->FPC_StartEditingSession();
            if (S_OK == hResult)
            {
                hResult = pCANLogger->FPC_AddLoggingBlock(logInfo);
            }
            pCANLogger->FPC_StopEditingSession(true);
        }

        return hResult;

    }

    /******************************************************************************
        Function Name    :  GetLoggingBlock

        Input(s)         :  -
        Output           :
        Functionality    :  Renders a logging block based on its index in the list
        Member of        :  CApplication
        Author(s)        :  Anish kumar
        Date Created     :  01.06.2010
        Modifications    :
    ******************************************************************************/
    HRESULT GetLoggingBlock(USHORT /*BlockIndex*/, void* /*psLoggingBlock*/)
    {
        return E_NOTIMPL;
    }

    /******************************************************************************
       Function Name    :  StartLogging

       Input(s)         :
       Output           :  -
       Functionality    :  Start logging if it is stopped
       Member of        :  CApplication
       Author(s)        :  Anish kumar
       Date Created     :  20.06.2006
       Modifications    :
    ******************************************************************************/
    HRESULT StartLogging()
    {
        HRESULT hResult = E_POINTER;
        CBaseFrameProcessor_CAN* pCANLogger = GetICANLogger();

        if (NULL != pCANLogger)
        {
            hResult = pCANLogger->FPC_EnableLogging(TRUE);
        }

        return hResult;
    }

    /******************************************************************************
        Function Name    :  StopLogging

        Input(s)         :
        Output           :  -
        Functionality    :  disable logging of messages
        Member of        :  CApplication
        Author(s)        :  Anish kumar
        Date Created     :  20.06.2006
        Modifications    :
    ******************************************************************************/
    HRESULT StopLogging()
    {
        HRESULT hResult = E_POINTER;
        CBaseFrameProcessor_CAN* pCANLogger = GetICANLogger();

        if (NULL != pCANLogger)
        {
            hResult = pCANLogger->FPC_EnableLogging(FALSE);
        }

        return hResult;
    }

    /******************************************************************************
        Function Name    :  WriteToLogFile

        Input(s)         :  -
        Output           :
        Functionality    :  Writes a string to the log file specified
        Member of        :  CApplication
        Author(s)        :  Anish kumar
        Date Created     :  01.06.2010
        Modifications    :
    ******************************************************************************/
    HRESULT WriteToLogFile(USHORT /*BlockIndex*/, BSTR bstrStr)
    {
        HRESULT hResult = E_POINTER;
        CBaseFrameProcessor_CAN* pCANLogger = GetICANLogger();

        if (NULL != pCANLogger)
        {
            char acStr[256] = {'\0'};
            BSTR_2_PCHAR(bstrStr, acStr, 256);
            CString omStr(acStr);
            hResult = pCANLogger->FPC_LogString(omStr);
        }

        return hResult;
    }

    /******************************************************************************
        Function Name    :  GetApplication

        Input(s)         :  -
        Output           :
        Functionality    :  Retrieves the application pointer
        Member of        :  CApplication
        Author(s)        :  Saravanan
        Date Created     :  30.07.2014
        Modifications    :
    ******************************************************************************/
    HRESULT GetApplication(IBusMasterApp** pApplication)
    {
        HRESULT hResult = CoCreateInstance(CLSID_BusMasterApp, NULL,
                                           CLSCTX_LOCAL_SERVER, IID_IBusMasterApp, (void**)&pApplication);

        return hResult;
    }

    BEGIN_INTERFACE_PART(LogClass, ICANLog)
    STDMETHOD(GetTypeInfoCount)(UINT FAR* pctinfo);
    STDMETHOD(GetTypeInfo)(
        UINT itinfo,
        LCID lcid,
        ITypeInfo FAR* FAR* pptinfo);
    STDMETHOD(GetIDsOfNames)(
        REFIID riid,
        OLECHAR FAR* FAR* rgszNames,
        UINT cNames,
        LCID lcid,
        DISPID FAR* rgdispid);
    STDMETHOD(Invoke)(
        DISPID dispidMember,
        REFIID riid,
        LCID lcid,
        WORD wFlags,
        DISPPARAMS FAR* pdispparams,
        VARIANT FAR* pvarResult,
        EXCEPINFO FAR* pexcepinfo,
        UINT FAR* puArgErr);
    STDMETHOD(StopLogging)();
    STDMETHOD(StartLogging)();
    STDMETHOD(AddLoggingBlock)(SLOGGINGBLOCK_USR* psLoggingBlock);
    STDMETHOD(WriteToLogFile)(USHORT BlockIndex, BSTR pcStr);
    STDMETHOD(RemoveLoggingBlock)(USHORT BlockIndex);
    STDMETHOD(GetLoggingBlockCount)(USHORT* BlockCount);
    STDMETHOD(ClearLoggingBlockList)(void);
    STDMETHOD(GetLoggingBlock)(USHORT BlockIndex, SLOGGINGBLOCK_USR* psLoggingBlock);
    STDMETHOD(GetApplication)(IBusMasterApp** pApplication);
    END_INTERFACE_PART(LogClass)
};
