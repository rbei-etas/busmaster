/*********************************************************************
  Project       :  Auto-SAT_Tools
  FileName      :  SigGrphWnd_Extern.h
  Description   :  Exported function prototypes
  $Log:   X:/Archive/Sources/SigGrphWnd/SigGrphWnd_Extern.h_v  $
 * 
 *    Rev 1.2   09 Jun 2011 18:05:32   CANMNTTM
 *  
 * 
 *    Rev 1.1   15 Apr 2011 19:43:38   rac2kor
 * Inserted RBEI Copyright information text into the file header.

  Author(s)     :  Arunkumar Karri
  Date Created  :  
  Modified By   :  
  Copyright (c) 2011, Robert Bosch Engineering and Business Solutions.  All rights reserved.
 *********************************************************************/

#if !defined SIGGRPHWND_EXTERN_H__INCLUDED_
#define SIGGRPHWND_EXTERN_H__INCLUDED_

#if defined USAGEMODE
#undef USAGEMODE
#endif

#if defined USAGE_EXPORT
#define USAGEMODE   __declspec(dllexport)
#else
#define USAGEMODE   __declspec(dllimport)
#endif

class CGraphList;
class CMsgBufVSE;

////Graph window splitter details
struct tagGraphWndSplitterData
{
    int m_nRootSplitterData[2][2];
    int m_nRightViewSplitterData[2][2];
};

typedef tagGraphWndSplitterData sGraphWndSplitterData;
typedef sGraphWndSplitterData SGRAPHWNDSPLITTERDATA;

#ifdef __cplusplus
extern "C" {  // only need to export C interface if used by C++ source code
#endif

/*USAGEMODE HRESULT SG_CreateGraphWindow([in] CMDIFrameWnd* pParentWnd, [in] short eBusType);
USAGEMODE HRESULT SG_ShowGraphWindow([in] short eBusType, [in] BOOL bShow = TRUE);
USAGEMODE BOOL    SG_IsWindowVisible([in] short eBusType);
USAGEMODE HRESULT SG_SetRefreshRate( [in] UINT unRefreshRate);
USAGEMODE HRESULT SG_SetSignalListDetails( [in] short eBusType, [in] CGraphList * pSignalList);
USAGEMODE HRESULT SG_vPostMessageToSGWnd([in] short eBusType, UINT msg, WPARAM wParam, LPARAM lParam);
USAGEMODE CMsgBufVSE* SG_GetGraphBuffer();
USAGEMODE HRESULT SG_GetWindowSplitterPos([in] short eBusType,
										WINDOWPLACEMENT& sWndPlacement, 
										SGRAPHWNDSPLITTERDATA& sGraphSplitter);
USAGEMODE HRESULT SG_SetWindowSplitterPos([in] short eBusType,
										WINDOWPLACEMENT& sWndPlacement, 
										SGRAPHWNDSPLITTERDATA& sGraphSplitter);*/
USAGEMODE HRESULT SG_CreateGraphWindow( CMDIFrameWnd* pParentWnd, short eBusType);
USAGEMODE HRESULT SG_ShowGraphWindow( short eBusType, BOOL bShow = TRUE);
USAGEMODE BOOL    SG_IsWindowVisible( short eBusType);
USAGEMODE HRESULT SG_SetRefreshRate( UINT unRefreshRate);
USAGEMODE HRESULT SG_SetSignalListDetails( short eBusType, CGraphList * pSignalList);
USAGEMODE HRESULT SG_vPostMessageToSGWnd( short eBusType, UINT msg, WPARAM wParam, LPARAM lParam);
USAGEMODE CMsgBufVSE* SG_GetGraphBuffer();
USAGEMODE HRESULT SG_GetWindowSplitterPos( short eBusType,
										WINDOWPLACEMENT& sWndPlacement, 
										SGRAPHWNDSPLITTERDATA& sGraphSplitter);
USAGEMODE HRESULT SG_SetWindowSplitterPos( short eBusType,
										WINDOWPLACEMENT& sWndPlacement, 
										SGRAPHWNDSPLITTERDATA& sGraphSplitter);
 
#ifdef __cplusplus
}
#endif

#endif //SIGGRPHWND_EXTERN_H__INCLUDED_