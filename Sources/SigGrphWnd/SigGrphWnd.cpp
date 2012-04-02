/*********************************************************************
  Project       :  Auto-SAT_Tools
  FileName      :  SigGrphWnd.cpp
  Description   :  
  $Log:   X:/Archive/Sources/SigGrphWnd/SigGrphWnd.cpv  $
   
      Rev 1.10   22 Jul 2011 11:46:06   CANMNTTM
   Updation of SIgnal plotter thread: Timings of signal data plotting.
   
      Rev 1.9   09 Jun 2011 18:05:32   CANMNTTM
   All errors in VS2005 are removed
   
      Rev 1.8   06 Jun 2011 11:51:38   CANMNTTM
    
   
      Rev 1.7   06 May 2011 14:29:52   CANMNTTM
   Updated with resource switch.
   
      Rev 1.6   06 May 2011 14:24:04   CANMNTTM
   Updated with resource switching code in necessary functions.
   
      Rev 1.5   21 Apr 2011 14:28:08   CANMNTTM
   Updated SG_GetWindowSplitterPos(..) function to get window config data from datastore.
   
      Rev 1.4   15 Apr 2011 19:43:34   rac2kor
   Inserted RBEI Copyright information text into the file header.

  Author(s)     :  
  Date Created  :  
  Modified By   :  
  Copyright (c) 2011, Robert Bosch Engineering and Business Solutions.  All rights reserved
*********************************************************************/

// SigGrphWnd.cpp : Defines the initialization routines for the DLL.
//

#include "SigGrphWnd_stdafx.h"
#include <afxdllx.h>

#define USAGE_EXPORT
#include "SigGrphWnd_Extern.h"
#include "SigGrphWnd_Resource.h"
#include "SigGrphWnd_Defines.h"

#include "GraphChildFrame.h"
#include "GraphLeftView.h"
#include "GraphBottomView.h"
#include "DataTypes/MsgBufVSE.h"
#include <DataTypes/SigGrphWnd_Datatypes.h>
#include "Utility/Utility_Thread.h"
// For Timer Manager interface
#include "TimeManager.h"
#include "Include/BaseDefs.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

static AFX_EXTENSION_MODULE SigGrphWndDLL = { NULL, NULL };
WINDOWPLACEMENT m_sGraphWndPlacement[AVAILABLE_PROTOCOLS];
SGRAPHSPLITTERDATA m_sGraphSplitterPos[AVAILABLE_PROTOCOLS];

extern "C" int APIENTRY
DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	// Remove this if you use lpReserved
	UNREFERENCED_PARAMETER(lpReserved);

	if (dwReason == DLL_PROCESS_ATTACH)
	{
		TRACE0("SigGrphWnd.DLL Initializing!\n");
		
		// Extension DLL one-time initialization
		if (!AfxInitExtensionModule(SigGrphWndDLL, hInstance))
			return 0;

		// Insert this DLL into the resource chain
		// NOTE: If this Extension DLL is being implicitly linked to by
		//  an MFC Regular DLL (such as an ActiveX Control)
		//  instead of an MFC application, then you will want to
		//  remove this line from DllMain and put it in a separate
		//  function exported from this Extension DLL.  The Regular DLL
		//  that uses this Extension DLL should then explicitly call that
		//  function to initialize this Extension DLL.  Otherwise,
		//  the CDynLinkLibrary object will not be attached to the
		//  Regular DLL's resource chain, and serious problems will
		//  result.

		new CDynLinkLibrary(SigGrphWndDLL);

		for(int nBUSID = 0; nBUSID<AVAILABLE_PROTOCOLS;nBUSID++)
		{
			m_sGraphWndPlacement[nBUSID].length = 0;
			m_sGraphWndPlacement[nBUSID].rcNormalPosition.top = -1;
			m_sGraphSplitterPos[nBUSID].m_nRootSplitterData[0][0] = -1;
		}
	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{
		TRACE0("SigGrphWnd.DLL Terminating!\n");

		// Terminate the library before destructors are called
		AfxTermExtensionModule(SigGrphWndDLL);
	}
	return 1;   // ok
}

CGraphChildFrame* m_pomGraphWindows[MAX_PROTOCOLS] = {NULL};
CMsgBufVSE m_ouMsgInterpretBuffer; 
CPARAM_THREADPROC m_ouGraphReadThread;

BOOL bStartGraphReadThread();
BOOL bStopGraphReadThread();
DWORD WINAPI SignalDataPlotterThread(LPVOID pVoid);

CString arrStrBusNames[] = { "CAN","MCNET","J1939","FLEXRAY"};

//Export Function Definitions.


USAGEMODE CMsgBufVSE* SG_GetGraphBuffer()
{	
	
	return &m_ouMsgInterpretBuffer;
}

USAGEMODE HRESULT SG_CreateGraphWindow( CMDIFrameWnd* pParentWnd,  short eBusType)
{
	if(m_pomGraphWindows[eBusType] == NULL)
	{
		//Place this code at the beginning of the export function.
		//Save previous resource handle and switch to current one.
		HINSTANCE hInst = AfxGetResourceHandle();
		AfxSetResourceHandle(SigGrphWndDLL.hResource);

		// Register Window Class				
		LPCTSTR strMDIClass = AfxRegisterWndClass(
									CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS,
									::LoadCursor(NULL, IDC_CROSS), 0,
									AfxGetApp()->LoadIcon(MAKEINTRESOURCE(IDR_ICON_GRAPH)));		
		// Create window instance
		m_pomGraphWindows[eBusType] = new CGraphChildFrame;
		if( m_pomGraphWindows[eBusType] != NULL )
		{
			if( pParentWnd != NULL )
			{				
			    // Get size from configuration module
				if( m_sGraphWndPlacement[eBusType].rcNormalPosition.top == -1 )
				{
					CRect rcParent;
					// Get Main Frame Size
					::GetWindowRect(pParentWnd->m_hWnd, &rcParent);					
					m_sGraphWndPlacement[eBusType].rcNormalPosition.top = 0;
					m_sGraphWndPlacement[eBusType].rcNormalPosition.left = 0;
					m_sGraphWndPlacement[eBusType].rcNormalPosition.right = 
									rcParent.right-10;
					m_sGraphWndPlacement[eBusType].rcNormalPosition.bottom = 
									rcParent.bottom- (rcParent.bottom/4);
				}
				// Create Child Window
				if( m_pomGraphWindows[eBusType]->Create( strMDIClass, defSTR_GRAPH_WINDOW_NAME,
											WS_CHILD | WS_OVERLAPPEDWINDOW/*|SW_SHOWNOACTIVATE*/,
											m_sGraphWndPlacement[eBusType].rcNormalPosition,
											pParentWnd ) == TRUE )
				{
	                
					m_pomGraphWindows[eBusType]->ShowWindow(SW_SHOW);
					m_pomGraphWindows[eBusType]->SetFocus();		

					CString strWindowText;
					strWindowText = _T("Graph Display - ") + arrStrBusNames[eBusType];					
					m_pomGraphWindows[eBusType]->SetWindowText(strWindowText);

					m_pomGraphWindows[eBusType]->SetWindowPlacement(&m_sGraphWndPlacement[eBusType]);					
					m_pomGraphWindows[eBusType]->vSetSplitterConfig(m_sGraphSplitterPos[eBusType]);	
				}
			}
		}
		else
		{
			AfxMessageBox( MSG_MEMORY_CONSTRAINT );
		}
		//Place this at the end of the export function.
		//switch back to previous resource handle.
		AfxSetResourceHandle(hInst); 
	}
	return S_OK;
}

USAGEMODE HRESULT SG_ShowGraphWindow( short eBusType,  BOOL bShow)
{	
	//Place this code at the beginning of the export function.
	//Save previous resource handle and switch to current one.
	HINSTANCE hInst = AfxGetResourceHandle();
	AfxSetResourceHandle(SigGrphWndDLL.hResource);

	if(bShow)
	{
		m_pomGraphWindows[eBusType]->ShowWindow(SW_SHOW);
		m_pomGraphWindows[eBusType]->BringWindowToTop();

		// If it is show then update NC area
        // Activate the window
        m_pomGraphWindows[eBusType]->MDIActivate();
        // Send NC Paint message in case of Window position change
        m_pomGraphWindows[eBusType]->SendMessage(WM_NCPAINT, 1, 0);
	}
	else
	{
		m_pomGraphWindows[eBusType]->ShowWindow(SW_HIDE);
	}
	//Place this at the end of the export function.
	//switch back to previous resource handle.
	AfxSetResourceHandle(hInst); 
	return S_OK;
}

USAGEMODE BOOL SG_IsWindowVisible( short eBusType)
{
	if(m_pomGraphWindows[eBusType]!=NULL)
	{
		if(m_pomGraphWindows[eBusType]->IsWindowVisible())
			return TRUE;		
	}	
	return FALSE;
}

USAGEMODE HRESULT SG_SetSignalListDetails(  short eBusType,  CGraphList * pSignalList)
{
	if(m_pomGraphWindows[eBusType] == NULL)
		return S_FALSE;

	m_pomGraphWindows[eBusType]->vSetSignalListDetails(pSignalList);
	// Update List Control
	// Get Left View Pointer
	//CGraphChildFrame* ptr = m_pomGraphWindows[eBusType];
    
	//Update Left view
	CGraphLeftView * pLeftView = ( CGraphLeftView *)
        m_pomGraphWindows[eBusType]->pomGetLeftViewPointer();	
	pLeftView->vPopulateElementList();
	pLeftView->vPopulateGraphControl();

	//Update bottom view
	CGraphBottomView* pBottomView = (CGraphBottomView*)
		m_pomGraphWindows[eBusType]->pomGetRightBottomViewPointer();		
	pBottomView->vInitGraphControl();
	pBottomView->vInsertSignalData();

	return S_OK;
}

USAGEMODE HRESULT SG_vPostMessageToSGWnd( short eBusType, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if(m_pomGraphWindows[eBusType])
	{
		m_pomGraphWindows[eBusType]->PostMessage(msg, wParam, lParam);

		eUSERSELCTION eUserSel = eDATABASEIMPORTCMD;
		eUserSel               = static_cast <eUSERSELCTION>(wParam);
		BOOL bConnect = FALSE;

		switch(eUserSel)
		{
			case eCONNECTCMD:
				bConnect = (BOOL)lParam;
				if(bConnect)
				{
					if(m_ouGraphReadThread.m_unActionCode != CREATE_TIME_MAP)
						bStartGraphReadThread();
				}
				else
				{
					bStopGraphReadThread();
				}
			break;
		}
	}

	return S_OK;
}

/* Function to get current config data */
USAGEMODE HRESULT SG_GetWindowSplitterPos( short eBusType,
										WINDOWPLACEMENT& sWndPlacement, 
										SGRAPHWNDSPLITTERDATA& sGraphSplitter)
{
    if(m_pomGraphWindows[eBusType])
    {
		CRect rcClient;
		//POINT ptClientUL;              // client upper left corner 
		//POINT ptClientLR;              // client lower right corner 

		m_pomGraphWindows[eBusType]->GetWindowRect(&rcClient);
		m_pomGraphWindows[eBusType]->GetWindowPlacement(&sWndPlacement);	
        m_pomGraphWindows[eBusType]->vGetSplitterConfig((SGRAPHSPLITTERDATA&)sGraphSplitter);
		return S_OK;
    }
    else
    {
		sWndPlacement = m_sGraphWndPlacement[eBusType];
		sGraphSplitter = (SGRAPHWNDSPLITTERDATA&)m_sGraphSplitterPos[eBusType];
		return S_FALSE;
    }	
}

/* Function to Set current config data */
USAGEMODE HRESULT SG_SetWindowSplitterPos( short eBusType,
										WINDOWPLACEMENT& sWndPlacement, 
										SGRAPHWNDSPLITTERDATA& sGraphSplitter)
{

    if(m_pomGraphWindows[eBusType])
    {
        m_pomGraphWindows[eBusType]->SetWindowPlacement(&sWndPlacement);
        m_pomGraphWindows[eBusType]->vSetSplitterConfig((SGRAPHSPLITTERDATA&)sGraphSplitter);		
    }
	else
	{
		m_sGraphWndPlacement[eBusType] = sWndPlacement;
		m_sGraphSplitterPos[eBusType]  = (SGRAPHSPLITTERDATA&)sGraphSplitter;
	}		
	return S_OK;
}

USAGEMODE HRESULT SG_SetRefreshRate(  UINT /*unRefreshRate*/)
{
	return S_OK;
}

void vClearGraphElementPoints(CComPtr<IDMGraphCollection>& spGraphCollection)
{
	long lCount = 0;
	spGraphCollection->get_Count(&lCount);
	for( int nIndex = 0; nIndex < lCount; nIndex++ )
	{
		CComPtr<IDispatch> spDispatch;
		CComPtr<IDMGraphElement> spGraphElement;
		spGraphCollection->get_Item(nIndex, &spDispatch);
		spDispatch.QueryInterface(&spGraphElement);

		CComVariant varrPt;
		varrPt.parray = SafeArrayCreateVector(VT_R8, 0, 0);
		varrPt.vt = VT_ARRAY|VT_R8;

		spGraphElement->Plot(varrPt, varrPt);		
	}
}

/* Read thread function for graph display*/
DWORD WINAPI SignalDataPlotterThread(LPVOID pVoid)
{
    CPARAM_THREADPROC* pThreadParam = (CPARAM_THREADPROC *) pVoid;
    if (pThreadParam == NULL)
    {
        return (DWORD)-1;
    }
	
	pThreadParam->m_unActionCode = CREATE_TIME_MAP;
	int nRefreshTime = 0;	
	int nBufferSize = 0;
	for(int nBUSId = 0; nBUSId<AVAILABLE_PROTOCOLS; nBUSId++)
	{
		if(m_pomGraphWindows[nBUSId] != NULL)
		{
			nRefreshTime =
				m_pomGraphWindows[nBUSId]->m_pGraphList->m_odGraphParameters.m_nRefreshRate;
			nBufferSize = m_pomGraphWindows[nBUSId]->m_pGraphList->m_odGraphParameters.m_nBufferSize;
		}
	}

	//Clear the buffer with previous contents if any.
	m_ouMsgInterpretBuffer.vClearMessageBuffer();

	LONGLONG g_nInitTimeStamp[MAX_PROTOCOLS] = {0};
	double g_dElapsdTime[MAX_PROTOCOLS] = {0};
    bool bLoopON = true;

	//Allow the connection related activities take place prior to proceeding with loop
	Sleep(1000);

	UINT64 unPointsCnt = 0;

	while (bLoopON)
    {
		//Commented by Arunkumar Karri on 07/02/2012 to make the plotting realtime.
        //Sleep(nRefreshTime);
		//Introduced a 50 Millisecond delay
		Sleep(50);
		if(pThreadParam->m_unActionCode == IDLE)
		{
			bLoopON = false;
		}		
		while(m_ouMsgInterpretBuffer.GetMsgCount() > 0)
		{
			INT nType = 0;
			INT nSize = MAX_DATA_LEN_J1939;
			SINTERPRETDATA_LIST sInterpretList;	
			sInterpretList.m_unValue.m_dPhysical = 0;
			sInterpretList.m_unValue.m_nRawValue = 0;
			HRESULT hResult = m_ouMsgInterpretBuffer.ReadFromBuffer(nType,
									(BYTE*)&sInterpretList, nSize);			

			//Get access to the Graph control
			IDMGraphCtrl* pDMGraphCtrl = m_pomGraphWindows[nType]->m_pDMGraphCtrl;
			if (  pDMGraphCtrl ==NULL )
			{				
				return 0;
			}

			//Get the Graph collectrion
			CComPtr<IDMGraphCollection> spGraphCollection;			
			pDMGraphCtrl->get_Elements(&spGraphCollection);				


			if(hResult == S_OK && m_pomGraphWindows[nType]!=NULL)
			{				
				if(g_nInitTimeStamp[nType] == 0)
				{
					g_nInitTimeStamp[nType] = sInterpretList.m_nTimeStamp;					

					g_dElapsdTime[nType] = CTimeManager::nCalculateCurrTimeStamp() -
												CTimeManager::nGetAbsoluteTime();
					g_dElapsdTime[nType] /= defTIMER_RESOLUTION_FACTOR;				
				}

				double dAbsTime = (double)sInterpretList.m_nTimeStamp - g_nInitTimeStamp[nType];
				dAbsTime /= defTIMER_RESOLUTION_FACTOR;
				dAbsTime+=g_dElapsdTime[nType];
				
				CGraphElement odTemp;
				CGraphList * podList = NULL;
				podList = m_pomGraphWindows[nType]->m_pGraphList;
				INT_PTR nItemCount  = podList->m_omElementList.GetSize();
				for( int nIndex = 0; nIndex < nItemCount; nIndex++ )
				{
					CComPtr<IDispatch> spDispatch;
					CComPtr<IDMGraphElement> spGraphElement;
					hResult = spGraphCollection->get_Item(nIndex, &spDispatch);
					if ( spDispatch == NULL )
						return 0;
					hResult = spDispatch.QueryInterface(&spGraphElement);					

					odTemp = podList->m_omElementList[ nIndex ];

					//if the read data is of type signal
					if( hResult == S_OK &&
						odTemp.m_bEnabled == TRUE &&
						odTemp.m_nValueType != eSTAT_PARAM &&
						sInterpretList.unMsgID == (unsigned int)odTemp.m_nMsgID)
					{
						CString strSigName(sInterpretList.m_acSigName);
						if(odTemp.m_omStrElementName == strSigName)
						{
							if(sInterpretList.m_shType == odTemp.m_nValueType)
							{
								if(odTemp.m_nValueType == eRAW_VALUE)
								{
									spGraphElement->PlotXY(dAbsTime, 
														  (double)sInterpretList.m_unValue.m_nRawValue);
								}
								else
								{
									spGraphElement->PlotXY(dAbsTime, 
														  sInterpretList.m_unValue.m_dPhysical);
								}
								unPointsCnt++;		
							}
						}
					}
					//If the read data from buffer is of parameter type
					else if( odTemp.m_bEnabled == TRUE &&
							odTemp.m_nValueType == eSTAT_PARAM &&
							sInterpretList.unMsgID == (unsigned int)nIndex)
					{
						double dTime = CTimeManager::nCalculateCurrTimeStamp() -
									   CTimeManager::nGetAbsoluteTime();
						dTime /= defTIMER_RESOLUTION_FACTOR;
						if(sInterpretList.m_unValue.m_nRawValue == -1)
						{
							spGraphElement->PlotXY(dTime, 
												  sInterpretList.m_unValue.m_dPhysical);
						}
						else
						{
							spGraphElement->PlotXY(dTime, 
												  (double)sInterpretList.m_unValue.m_nRawValue);
						}
						unPointsCnt++;
					}
				}
				//If the plotted points exceeds the maximum buffer limit
				if ( unPointsCnt > nBufferSize )
				{
					vClearGraphElementPoints(spGraphCollection);
					//After clearing the Graph, sleep for 100 msec
					Sleep(100);
					unPointsCnt = 0;
				}
			}
		}
		double dAbsTime = CTimeManager::nCalculateCurrTimeStamp() -
				CTimeManager::nGetAbsoluteTime();
		dAbsTime /= defTIMER_RESOLUTION_FACTOR;

		for(register int nBusID = 0; nBusID<AVAILABLE_PROTOCOLS; nBusID++)
		{
			if(m_pomGraphWindows[nBusID] == NULL)
				continue;

			if ( m_pomGraphWindows[nBusID]->m_pDMGraphCtrl == NULL )
			{
				break;
			}

			IDMGraphCtrl* pDMGraphCtrl = m_pomGraphWindows[nBusID]->m_pDMGraphCtrl;			

			//Update X-axis Range
			double dXMin = 0.0, dXMax = 0.0,dYMin = 0.0, dYMax = 0.0;
			// Get Present X,Y Axis Values
			pDMGraphCtrl->GetRange(&dXMin, &dXMax,&dYMin, &dYMax);			
			// Check the Max Range is lesser then the present time value			
			// If the time axis is old then set the latest value
			if( dXMax < dAbsTime )
			{
				double dRange = dXMax - dXMin;
				// Update Graph Control
				pDMGraphCtrl->SetRange(dAbsTime - dRange, dAbsTime, dYMin, dYMax);
			}
		}
    }    
    return 0;
}
/* Function to start Msg read thread*/
BOOL bStartGraphReadThread()
{
    BOOL bReturn = FALSE;
    //First stop the thread if running
    bStopGraphReadThread();
	m_ouGraphReadThread.m_hActionEvent = NULL;
    m_ouGraphReadThread.m_unActionCode = IDLE;
    m_ouGraphReadThread.m_pBuffer = NULL;
    m_ouGraphReadThread.m_hActionEvent = m_ouMsgInterpretBuffer.hGetNotifyingEvent();
    bReturn = m_ouGraphReadThread.bStartThread(SignalDataPlotterThread);
    return bReturn;
}

/* Function to stop msg read thread*/
BOOL bStopGraphReadThread()
{
    BOOL bReturn = FALSE;
    bReturn = m_ouGraphReadThread.bTerminateThread();
    m_ouGraphReadThread.m_hActionEvent = NULL;
    m_ouGraphReadThread.m_unActionCode = IDLE;
    return bReturn;
}
