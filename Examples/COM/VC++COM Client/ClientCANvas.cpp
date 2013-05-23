// ClientCANvas.cpp : implementation file
//

#include "stdafx.h"
#include "Server1.h"
#include "ClientCANvas.h"
#include "GetTextDialog.h"
#include "GetNumDialog.h"
#include "CANFrameParam.h"
#include "BusStatDlg.h"
#include ".\clientcanvas.h"
#include "Utility_Thread.h"

static IApplication* pICANvas = NULL;

/* Global declarations */
CPARAM_THREADPROC sg_ReceiveThreadObj;
static void vProcessCanMsg(HANDLE hClientHandle, CClientCANvas *pDlg);


typedef struct tagGUIComp
{
    UINT    m_unID;
    CString m_omHelpText;
} SGUICOMP;

static SGUICOMP asGuiComp[] =
{
    { 0x0,  _T("") },
    { IDC_BTN_CANVAS_INIT,  _T("Starts CANvas as an automation server") },
    { IDC_BTN_CANVAS_CLOSE, _T("Closes the CANvas automation server") },
    { IDC_BTN_LOAD_CONFIG,  _T("Loads a configuration file") },
    { IDC_BTN_SAVE_CONFIG,  _T("Saves the loaded configuration file") },
    { IDC_BTN_CONNECT,      _T("Connects CANvas to the bus") },
    { IDC_BTN_START_LOG,    _T("Starts logging") },
    { IDC_BTN_STOP_LOG,     _T("Stops logging") },
    { IDC_BTN_WRITE_LOG,    _T("Write a text into the log file") },
    { IDC_BTN_LOAD_DLL,     _T("Loads the user-defined dll") },
    { IDC_BTN_UNLOAD_DLL,   _T("Unloads the loaded user-defined dll") },
    { IDC_BTN_START_TX,     _T("Starts transmission") },
    { IDC_BTN_STOP_TX,      _T("Stops transmission") },
    { IDC_BTN_SEND,         _T("Sends a message") },
    { IDC_BTN_DB_IMPORT,    _T("Imports a database") },
    { IDC_BTN_INFO_MSG,     _T("Shows message information") },
    { IDC_BTN_TXBLK_COUNT,  _T("Count transmission blocks") },
    { IDC_BTN_TXBLK_CLEAR,  _T("Clear transmission blocks") },
    { IDC_BTN_NET_STAT,     _T("Displays network statistics") },
    { IDC_BTN_LOGBLK_COUNT, _T("Counts logging blocks") },
    { IDC_BTN_LOGBLK_REMOVE,_T("Deletes a logging block") },
    { IDC_BTN_LOGBLK_CLEAR, _T("Clears logging blocks") },
    { IDC_BTN_HANDLERS_MOD, _T("Enable / disable handlers") },
    { IDC_BTN_SEND_KEYVAL,  _T("Send a key value") },
    { IDC_BTN_SAVE_CONFIG_AS, _T("Saves the present configuration as...") }
};
const int GUICOMPS = sizeof(asGuiComp) / sizeof(SGUICOMP);

SGUICOMP* psGetGUICompEntry(UINT unCompID)
{
    SGUICOMP* Result = &asGuiComp[0];
    for (int i = 0; i < GUICOMPS; i++)
    {
        if (asGuiComp[i].m_unID == unCompID)
        {
            Result = &asGuiComp[i];
            i = GUICOMPS;
        }
    }
    return Result;
}

BSTR ConvertStringToBSTR(const char* Source)
{
    BSTR Result = NULL;

    if (NULL != Source)
    {
        int Chars = lstrlen(Source);

        Result = ::SysAllocStringLen(NULL, Chars);
        if (NULL != Result)
        {
            if (::MultiByteToWideChar(CP_ACP, 0, Source, -1, Result, Chars) == 0)
            {
                if (ERROR_INSUFFIENT_BUFFER != ::GetLastError())
                {
                    ::SysFreeString(Result);
                    Result = NULL;
                }
            }
        }
    }

    return Result;
}

void CClientCANvas::UpdateGUI(void)
{
    GetDlgItem(IDC_BTN_CANVAS_INIT  )->EnableWindow(NULL == pICANvas);
	GetDlgItem(IDC_EDIT_UNIQUE_ID   )->EnableWindow(NULL == pICANvas);
    GetDlgItem(IDC_BTN_CANVAS_CLOSE )->EnableWindow(NULL != pICANvas);
    GetDlgItem(IDC_BTN_LOAD_CONFIG  )->EnableWindow(NULL != pICANvas);
    GetDlgItem(IDC_BTN_SAVE_CONFIG  )->EnableWindow(NULL != pICANvas);
    GetDlgItem(IDC_BTN_CONNECT      )->EnableWindow(NULL != pICANvas);
    GetDlgItem(IDC_BTN_START_LOG    )->EnableWindow(NULL != pICANvas);
    GetDlgItem(IDC_BTN_STOP_LOG     )->EnableWindow(NULL != pICANvas);
    GetDlgItem(IDC_BTN_WRITE_LOG    )->EnableWindow(NULL != pICANvas);
    GetDlgItem(IDC_BTN_LOAD_DLL     )->EnableWindow(NULL != pICANvas);
    GetDlgItem(IDC_BTN_UNLOAD_DLL   )->EnableWindow(NULL != pICANvas);
    GetDlgItem(IDC_BTN_START_TX     )->EnableWindow(NULL != pICANvas);
    GetDlgItem(IDC_BTN_STOP_TX      )->EnableWindow(NULL != pICANvas);
    GetDlgItem(IDC_BTN_SEND         )->EnableWindow(NULL != pICANvas);
    GetDlgItem(IDC_BTN_NET_STAT     )->EnableWindow(NULL != pICANvas);
    GetDlgItem(IDC_BTN_SAVE_CONFIG_AS)->EnableWindow(NULL != pICANvas);
    GetDlgItem(IDC_BTN_DB_IMPORT    )->EnableWindow(NULL != pICANvas);
    GetDlgItem(IDC_BTN_INFO_MSG     )->EnableWindow(NULL != pICANvas);
    GetDlgItem(IDC_BTN_TXBLK_COUNT  )->EnableWindow(NULL != pICANvas);
    GetDlgItem(IDC_BTN_TXBLK_CLEAR  )->EnableWindow(NULL != pICANvas);
    GetDlgItem(IDC_BTN_LOGBLK_COUNT )->EnableWindow(NULL != pICANvas);
    GetDlgItem(IDC_BTN_LOGBLK_REMOVE)->EnableWindow(NULL != pICANvas);
    GetDlgItem(IDC_BTN_LOGBLK_CLEAR )->EnableWindow(NULL != pICANvas);
    GetDlgItem(IDC_BTN_HANDLERS_MOD )->EnableWindow(NULL != pICANvas);
    GetDlgItem(IDC_BTN_SEND_KEYVAL  )->EnableWindow(NULL != pICANvas);

    CString omStr = m_bConnected ? _T("Disconnect") : _T("Connect");
    GetDlgItem(IDC_BTN_CONNECT      )->SetWindowText(omStr);

	UpdateData(FALSE);
}

// CClientCANvas dialog

IMPLEMENT_DYNAMIC(CClientCANvas, CDialog)
CClientCANvas::CClientCANvas(CWnd* pParent /*=NULL*/)
	: CDialog(CClientCANvas::IDD, pParent), m_omHelpText(_T(""))
{
    m_bConnected = FALSE;
    if (SUCCEEDED( CoInitializeEx(NULL, COINIT_MULTITHREADED )))
    {
        // Do nothing
    }
    else
    {
        ASSERT(FALSE);
    }
	
	m_pConnPtContainer = NULL;
	m_pCntPoint = NULL;
	m_pSinkUnk = NULL;
	m_pSink = NULL;
	m_dwAdvise = 0;

	/* Rx Thread creation */
	sg_ReceiveThreadObj.m_hActionEvent = NULL;
	sg_ReceiveThreadObj.m_pBuffer = this;
	sg_ReceiveThreadObj.m_unActionCode = INACTION;
	m_shUniqueID = 25;

	m_hReadPIPE = m_hEvent = NULL;
}

CClientCANvas::~CClientCANvas()
{
    CoUninitialize();

	/* Unadvise and release */
	if ( m_pCntPoint )
	{
		m_pCntPoint->Unadvise(m_dwAdvise); //disconnect from server
		m_pCntPoint->Release();
		if ( m_pSink )
		{
			delete m_pSink;
		}
	}
}

void CClientCANvas::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_STATIC_HELPTEXT, m_omHelpText);
	DDX_Control(pDX, IDC_LIST_RX_DATA, m_lstReceivedData);
	DDX_Control(pDX, IDC_EDIT_RX_COUNT, m_edMsgReadCount);	
	DDX_Text(pDX, IDC_EDIT_UNIQUE_ID, m_shUniqueID);
	DDV_MinMaxShort(pDX, m_shUniqueID, 0, 255);
}


BEGIN_MESSAGE_MAP(CClientCANvas, CDialog)
    ON_WM_DESTROY()
END_MESSAGE_MAP()



BOOL CClientCANvas::OnInitDialog()
{
    CDialog::OnInitDialog();

    // TODO:  Add extra initialization here
    UpdateGUI();

    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

/* Receive Thread */
DWORD WINAPI ReceiveThread(LPVOID pVoid)
{
    bool bLoopON = true;	
	CPARAM_THREADPROC* pThreadParam = (CPARAM_THREADPROC*) pVoid;
	CClientCANvas* pDlg = (CClientCANvas*)pThreadParam->m_pBuffer;	
	UINT64 unRecvId = 0;

    while (bLoopON)
    {
		WaitForSingleObject(pDlg->m_hEvent, INFINITE);		
        switch (pThreadParam->m_unActionCode)
        {
            case INVOKE_FUNCTION:
            {
				// Retrieve message from the pipe
				vProcessCanMsg(pDlg->m_hReadPIPE, pDlg);
            }
            break;
            case EXIT_THREAD:
            {
                bLoopON = false;
            }
            break;
            break;
            default:
            case INACTION:
            {
                // nothing right at this moment
            }
            break;
        }
    }
    SetEvent(pThreadParam->hGetExitNotifyEvent());	
    pThreadParam->m_hActionEvent = NULL;

	return 0;
}

static void vProcessCanMsg(HANDLE hClientHandle, CClientCANvas *pDlg)
{
	/*CANRxMessage sReceivedMsg;*/
	CAN_MSGS sReceivedMsg;
	static DWORD dwBytes = 0;
	static DWORD dwBytesToRead = 0;
	UCHAR ucTempBuffer[500000] = {0};
	static UINT64 unNoOfRecvdId = 0;
	DWORD dwCount = 0;
	CString strRxCnt;
	CString strRecvd;
	BYTE bytChannel;

	while(PeekNamedPipe(hClientHandle, NULL, NULL, NULL, &dwBytesToRead, NULL))				
	{
		if(0 == dwBytesToRead)
		{
			break;
		}
		else if(dwBytesToRead>0)
		{
			ReadFile(hClientHandle, (void*)ucTempBuffer, dwBytesToRead, &dwBytes, NULL);
			if((dwBytes == sizeof(CAN_MSGS))||((dwBytes % sizeof(CAN_MSGS)) == 0))
			{
				while(dwBytes)
				{
					memcpy(&sReceivedMsg, ucTempBuffer, sizeof(CAN_MSGS));
					//bytChannel = sReceivedMsg.m_ucChannel;
					//sReceivedMsg.m_ucData[8] = 0;

					//strRecvd.Format("Msg ID= %d, DLC= %d, Data= %s, Ch = %d, RTR= %d, Ext = %d",
					//	sReceivedMsg.m_unMsgID, sReceivedMsg.m_ucDataLen, sReceivedMsg.m_ucData, bytChannel, 
					//	sReceivedMsg.m_bRTR, sReceivedMsg.m_bEXTENDED );
					strRecvd.Format("%s", sReceivedMsg.m_ucData );

					pDlg->m_lstReceivedData.InsertString(unNoOfRecvdId++, strRecvd);
					strRxCnt.Format("%ld",unNoOfRecvdId);
					pDlg->m_edMsgReadCount.SetWindowText(strRxCnt);

					dwBytes -= sizeof(CAN_MSGS);
				}
			}
		}
		else
		{
			/* Do nothing */
		}
	}
}

// CClientCANvas message handlers

BOOL CClientCANvas::OnCommand(WPARAM wParam, LPARAM lParam)
{
	UpdateData();
    m_omHelpText = psGetGUICompEntry((UINT) wParam)->m_omHelpText;
    UpdateData(FALSE);
	USES_CONVERSION;

    switch (wParam)
    {
        case IDC_BTN_CANVAS_INIT:
        {
            HRESULT hResult = CoCreateInstance(CLSID_Application, NULL, 
                   CLSCTX_LOCAL_SERVER, IID_IApplication, (void **) &pICANvas);
            if ((SUCCEEDED(hResult)) && (NULL != pICANvas))
            {
				/* Register client for Rx data */

				pICANvas->RegisterClientForRx(m_shUniqueID, &m_acEventName, &m_acPIPEName);

				LPCTSTR pcstrEvent = W2T(m_acEventName);
				LPCTSTR pcstrPIPE  = W2T(m_acPIPEName);

				if ( m_hEvent )
				{
					::CloseHandle( m_hEvent );
				}

				if ( m_hReadPIPE )
				{
					::CloseHandle( m_hReadPIPE );
				}
				/* Open Event */
				m_hEvent = OpenEvent(EVENT_ALL_ACCESS, FALSE, pcstrEvent);				
				/* Get the pipe handle for reading */
				m_hReadPIPE =CreateFile(
										pcstrPIPE,			// pipe name
										GENERIC_READ,					// read access
										0,								// no sharing
										NULL,							// no security attributes
										OPEN_EXISTING,					// opens existing pipe
										0,								// default attributes
										NULL);							// no template file

				sg_ReceiveThreadObj.bTerminateThread();

				sg_ReceiveThreadObj.m_pBuffer = this;
				sg_ReceiveThreadObj.m_unActionCode = INVOKE_FUNCTION;
				sg_ReceiveThreadObj.m_hActionEvent   = m_hEvent;
				sg_ReceiveThreadObj.bStartThread(ReceiveThread);

                UpdateGUI();
            }
            else
            {
                ASSERT(FALSE);
            }
        }
        break;
        case IDC_BTN_CANVAS_CLOSE:
        {
            if (NULL != pICANvas)
            {

				pICANvas->UnRegisterClient(m_shUniqueID);
                pICANvas->Release();
                pICANvas = NULL;
                UpdateGUI();
            }
        }
        break;
        case IDC_BTN_LOAD_CONFIG:
        {
            CFileDialog omFileDlg(TRUE, "cfx", "", 
                                  OFN_HIDEREADONLY | OFN_FILEMUSTEXIST,
                                  "CANvas configuration(*.cfx)|*.cfx||", NULL);
            // Set Title
            omFileDlg.m_ofn.lpstrTitle  = _T("Load Configuration File...");
            if (omFileDlg.DoModal() == IDOK)
            {
                CString omPath = omFileDlg.GetPathName();
                BSTR bstrPath = ConvertStringToBSTR(omPath.GetBuffer(MAX_PATH));
                pICANvas->LoadConfiguration(bstrPath);
            }
        }
        break;
        case IDC_BTN_SAVE_CONFIG:
        {
            pICANvas->SaveConfiguration();
        }
        break;
        case IDC_BTN_SAVE_CONFIG_AS:
        {
            CFileDialog omFileDlg(FALSE, "cfx", "", 
                                  OFN_HIDEREADONLY | OFN_PATHMUSTEXIST,
                                  "CANvas configuration(*.cfx)|*.cfx||", NULL);
            // Set Title
            omFileDlg.m_ofn.lpstrTitle  = _T("Save Configuration As...");
            if (omFileDlg.DoModal() == IDOK)
            {
                CString omPath = omFileDlg.GetPathName();
                BSTR bstrPath = ConvertStringToBSTR(omPath.GetBuffer(MAX_PATH));
                pICANvas->SaveConfigurationAs(bstrPath);
            }
        }
        break;
        case IDC_BTN_CONNECT:
        {
            if (pICANvas->Connect(!m_bConnected) == S_OK)
            {
                m_bConnected = !m_bConnected;
                UpdateGUI();
            }
        }
        break;
        case IDC_BTN_START_LOG:
        {
            pICANvas->StartLogging();
        }
        break;
        case IDC_BTN_STOP_LOG:
        {
            pICANvas->StopLogging();
        }
        break;
        case IDC_BTN_WRITE_LOG:
        {
            CGetTextDialog odTxtDlg;
            if (odTxtDlg.DoModal() == IDOK)
            {
                BSTR bstrTxt = ConvertStringToBSTR(odTxtDlg.m_omText.GetBuffer(MAX_PATH));
                pICANvas->WriteToLogFile(0, bstrTxt);
            }
        }
        break;
        case IDC_BTN_LOAD_DLL:
        {
            pICANvas->LoadAllDll();
        }
        break;
        case IDC_BTN_UNLOAD_DLL:
        {
            pICANvas->UnLoadAllDll();
        }
        break;
        case IDC_BTN_START_TX:
        {
            pICANvas->StartTxMsgBlock();
        }
        break;
        case IDC_BTN_STOP_TX:
        {
            pICANvas->StopTxMsgBlock();
        }
        break;
        case IDC_BTN_TXBLK_COUNT:
        {
            USHORT Result;
            if (pICANvas->GetTxBlockCount(&Result) == S_OK)
            {
                CString omStr;
                omStr.Format(_T("Tx Blocks: %d"), Result);
                AfxMessageBox(omStr, MB_OK);
            }
        }
        break;
        case IDC_BTN_TXBLK_CLEAR:
        {
            pICANvas->ClearTxBlockList();
        }
        break;
        case IDC_BTN_SEND:
        {
            static CCANFrameParam odDlg;
            if (odDlg.DoModal() == IDOK)
            {
				/*Send Msg */
                pICANvas->SendCANMSg(&(odDlg.m_sCFrame));
            }
        }
        break;
        case IDC_BTN_NET_STAT:
        {
            sBUSSTATISTICS_USR sBusStat;
            pICANvas->GetNetworkStatistics(1, &sBusStat);
            CBusStatDlg odBusStat(&sBusStat, this);
            odBusStat.DoModal();
        }
        break;
        case IDC_BTN_DB_IMPORT:
        {
            CFileDialog omFileDlg(TRUE, "cfx", "", 
                                  OFN_HIDEREADONLY | OFN_FILEMUSTEXIST,
                                  "CANvas database(*.dbf)|*.dbf||", NULL);
            // Set Title
            omFileDlg.m_ofn.lpstrTitle  = _T("Import Database File...");
            if (omFileDlg.DoModal() == IDOK)
            {
                CString omPath = omFileDlg.GetPathName();
                BSTR bstrPath = ConvertStringToBSTR(omPath.GetBuffer(MAX_PATH));
                pICANvas->ImportDatabase(bstrPath);
            }
        }
        break;
        case IDC_BTN_INFO_MSG:
        {
            CGetTextDialog odTxtDlg;
            if (odTxtDlg.DoModal() == IDOK)
            {
                BSTR bstrTxt = ConvertStringToBSTR(odTxtDlg.m_omText.GetBuffer(MAX_PATH));
                sMESSAGESTRUCT sMsgStruct;
                if (pICANvas->GetMsgInfo(bstrTxt, &sMsgStruct) == S_OK)
                {
                    CString omStr;
                    omStr.Format(_T("Code: %x\r\nSignals: %d\r\nLength: %d"),
                        sMsgStruct.m_dMessageCode, sMsgStruct.m_dNumberOfSignals, 
                        sMsgStruct.m_dMessageLength);
                    AfxMessageBox(omStr, MB_OK);
                }
            }
        }
        break;
        case IDC_BTN_LOGBLK_COUNT:
        {
            USHORT Result;
            if (pICANvas->GetLoggingBlockCount(&Result) == S_OK)
            {
                CString omStr;
                omStr.Format(_T("Logging Blocks: %d"), Result);
                AfxMessageBox(omStr, MB_OK);
            }
        }
        break;
        case IDC_BTN_LOGBLK_REMOVE:
        {
            GetNumDialog odNumDlg;
            if (odNumDlg.DoModal() == IDOK)
            {
                USHORT Index = (USHORT) odNumDlg.m_dwNumber;
                pICANvas->RemoveLoggingBlock(Index);
            }
        }
        break;
        case IDC_BTN_LOGBLK_CLEAR:
        {
            pICANvas->ClearLoggingBlockList();
        }
        break;
        case IDC_BTN_HANDLERS_MOD:
        {
            static BOOL bEnabled = FALSE;
            pICANvas->EnableDisableHandlers(!bEnabled, all);
            bEnabled = !bEnabled;
        }
        break;
        case IDC_BTN_SEND_KEYVAL:
        {
            pICANvas->SendKeyValue((UCHAR) 'A');
        }
        break;
    }

    return CDialog::OnCommand(wParam, lParam);
}


void CClientCANvas::OnDestroy()
{
    CDialog::OnDestroy();

    if (NULL != pICANvas)
    {
        pICANvas->Release();
        pICANvas = NULL;
    }
}
