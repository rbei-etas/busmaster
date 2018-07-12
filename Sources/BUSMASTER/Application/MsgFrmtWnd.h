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
 * \file      MsgFrmtWnd.h
 * \authors   Ratnadip Choudhury, Arunkumar Karri
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 */
#pragma once
#include "RxMsgList.h"
#include <set>
#include "include/BaseDefs.h"
//#include "PSDIHandler.h"
#include "TreeItemParam.h"
#include "Utility/MsgInterpretation.h"
#include "Utility/FFListCtrl.h"
#include "MessageInterpretation.h"
#include "ProgressBarDlg.h"
#include "PSDI_Extern.h"
#include "MsgWndHdrInfo.h"
#include "CommonClass/MsgContainerBase.h"
// Interface file for the message attribute class
#include "MessageAttrib.h"
#include "ImportLogToolbar.h"
#include "Utility\RadixEdit.h"
// CMsgFrmtWnd
const int nOverWriteMaxMsg         =  1000;
const int nMaxProtocol             =  20;

typedef struct sMsgDispMapEntry
{
    int              m_mListIndex;     // Message / Error Entry Index
    int              m_nBufferIndex;   // Store its last index in Append Buffer
    int              m_nSignalCnt;     // Store the no. of signals if expanded
    __int64          m_nTimeStamp;     // Time stamp of previous message
    __int64          m_nTimeOffset;    // time offset to its previous instance
    EINTERPRET_MODE  m_eInterpretMode; // Expanded / Collapsed /non interpret
    CTreeItemParam*  m_opTreeWndParam;

    sMsgDispMapEntry();
    sMsgDispMapEntry(const sMsgDispMapEntry& sRhsObj);
    sMsgDispMapEntry& operator=(const sMsgDispMapEntry& sRhsObj);

} SMSGDISPMAPENTRY, *PSMSGDISPMAPENTRY;



class CMsgFrmtWnd : public CMDIChildWnd, public IRxMsgCallBack
{
    //DECLARE_DYNAMIC(CMsgFrmtWnd)
protected:
    static CMenu NEAR menu;
    CMsgWndHdrInfo m_MsgHdrInfo;
public:


    CMsgFrmtWnd(ETYPE_BUS eBusType, CMsgContainerBase* msgContainer, HWND parentWnd);

    virtual ~CMsgFrmtWnd();
    virtual void vSetDefaultHeaders();


    BOOL Create(LPCTSTR szTitle, LONG style, const RECT& rect, CMDIFrameWnd* parent);

    void vRearrangeCols();
    void vSetColTitles(CString omArrColTitle[], int nColCount);
    void vCreateCols();
    int nGetListCtrlTextExtent(CString omColTitle);

    void onRxMsg(void* pMsg);
    BOOL bStartReadThread();
    BOOL bStopReadThread();
    void vUpdatePtrInLstCtrl();
    void vInit(void* pParam);
    //To set DIL client ID
    void vSetClientID(DWORD dwClientID);

    //Get Config Data.
    bool GetConfigData(xmlNodePtr pxmlNodePtr);
    //Set Config Data.
    HRESULT SetConfigData(BYTE* pvDataStream);
    HRESULT SetConfigData(xmlDocPtr pNodeSet);

    HRESULT SetConfigData(xmlNodePtr pNode);
    UINT nGetMsgFrmtWndConfigSize();

    void vSetDefaultWindowPosition(CRect omrect);

private:

    CMessageAttrib& m_ouMsgAttr;
    CMsgContainerBase* m_pouMsgContainerIntrf = nullptr;

    IBMNetWorkGetService* m_pBmNetWork;

    BOOL m_bInterPretMsg;
    //CMsgSignal** m_ppMsgDB;
    CRITICAL_SECTION       m_omCritSecForMapArr;
    CRITICAL_SECTION       m_omCritSecClearAll;
    CRITICAL_SECTION m_CritSec1;
    CRITICAL_SECTION m_ouCriticalSection;
    BOOL m_bMsgIntrprtnDlgShown;
    bool m_bUnloadOnClear;


    //end bhaskar
    UINT m_unProgressBarTimerId;
    //Message Window Title
    CString m_omStrWindowTitle;
    LRESULT OnUpdateFont(WPARAM wParam, LPARAM lParam);


    //bool bIsInterpretable(__int64 nMsgCode);
    void vExpandContractAllEntries (BOOL bInterpretationON, BOOL bChangeIntptnState = TRUE);
    void vCreateAllMsgTree();
    HRESULT vGetSignalInfoArray(__int64 nMapIndex, SSignalInfoArray& SigInfoArray);
    void vExpandMsgEntry( SMSGDISPMAPENTRY& sEntry, SSignalInfoArray& SigInfoArray,int nMsgIndex);
    void vFormatSignalInfo (const SSignalInfoArray& omSigInfoArray, CStringArray& omSigStrArray);
    int nGetMsgListIndexFromMapIndex(__int64 nMapIndex);
    void vContractMsgEntry(SMSGDISPMAPENTRY& sEntry, int nMsgIndex);
    void vGetTreeRect(int nItem, int nCount, CRect& omTreeRect);
    void vExpandContractMsg(int nIndex,eTreeItemStates eTreeItemState = TREESTATE_TOGGLE);
    void vArrangeAllTreeWnd();
    void vRemoveAllMsgTree();


    // To format sg_omColmStr to display error message on message window
    void vSetDefaultConfigValues();
    char* m_pomDataPtrArr[MAX_MSG_WND_COL_CNT];
    int m_nIndex;
    void vShowUpdateMsgIntrpDlg(__int64 nMapIndex);
    void vUpdateMsgTreeWnd(__int64 nMapIndex);
    void vUpdateAllTreeWnd();
    void vSetDefaultPlacement();
    void CmdRouteMenu(CWnd* pWnd,CMenu* pPopupMenu);
    void UnloadFile(bool bFileUnload);
    void ShowImportLogToolbar(BOOL bShow);
    void OnImportLogSetTotalPages(int pageNumber);
    UINT m_unCurrInterpretedMsgID;
    __int64 m_unCurrInterpretedMapIndex;
    CMessageInterpretation* m_podMsgIntprtnDlg;
    //CProgressBarDlg m_odProgressCtrlDlg;
    INT m_anMsgBuffSize[defDISPLAY_CONFIG_PARAM];
    void vUpdateMsgBufferDetails(INT* pMsgBuffSize);
    void vDisplayToolTip(MSG* pMsg);
    CToolTipCtrl m_objToolTip;
    SMSGWNDHDRCOL m_sHdrColStruct;
    int m_nPrevToolCol;
    int m_nPrevToolRow;
    CRect m_omDefaultRect;



    //Connection Status
    bool m_bConnected;
    bool m_bIsLogFileImported;
    //Client Id from the DIL
    DWORD m_dwClientID;
    void createToolBar();

protected:
    DECLARE_MESSAGE_MAP()

    void vFitListCtrlToWindow();

    BOOL CMsgFrmtWnd::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
    CImportLogToolbar m_wndToolbarImportLog;
    CSpinButtonCtrl m_omSpinPageNo;
    CRadixEdit m_omEditPageNo;
    CSliderCtrl m_omSliderMsgs;

    UINT m_unDispUpdateTimerId;

    //List ctrl Msg Identifier(ID + TYPE + ...) to index in list control MAP
    CMap<__int64, __int64, SMSGDISPMAPENTRY, SMSGDISPMAPENTRY> m_omMsgDispMap;
    //Array of SMSGDISPMAPENTRY on overwrite display
    std::vector<__int64> m_omMgsIndexVec;
    BOOL m_bUpdate;

    HWND m_hMainWnd;
    //BOOL m_bLogON;
    BOOL m_bSignalWatchON;
    SERROR_INFO m_sErrorInfo;
    CRxMsgList m_lstMsg;
    ETYPE_BUS m_eBusType;
    UINT m_nColCount;
    CStringArray m_omArrColTitle;

    BYTE m_bExprnFlag_Disp; // expression flag for message display
    //kadoor CSigWatchDlg *m_podSignalWatchDlg;
    int m_nField;
    bool m_bAscending;
    __int64* m_pExpandedMapIndexes;

    WINDOWPLACEMENT m_sMsgIntrpWndPlacement;
    bool mBusConnectedState;
public:
    afx_msg void OnSize(UINT nType, int cx, int cy);
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
    afx_msg LRESULT vNotificationFromOtherWin(WPARAM wParam, LPARAM lParam);
    afx_msg LRESULT vUpdateFormattedMsgStruct(WPARAM wParam, LPARAM lParam);
    afx_msg LRESULT vSortMsgWndColumn(WPARAM wParam, LPARAM lParam);
    afx_msg LRESULT vUpdateMsgClr(WPARAM wParam, LPARAM lParam);
    afx_msg void OnTimer(UINT nIDEvent);
    afx_msg LRESULT ModifyMsgWndProperty(WPARAM wParam, LPARAM lParam);
    afx_msg LRESULT OnDisplaySettingUpdated(WPARAM wParam, LPARAM lParam);


    afx_msg LRESULT ProvideMsgWndProperty(WPARAM wParam, LPARAM lParam);
    afx_msg LRESULT SetFilterDetails(WPARAM wParam, LPARAM lParam);
    afx_msg LRESULT GetFilterDetails(WPARAM wParam, LPARAM lParam);
    afx_msg LRESULT EnableFilterApplied(WPARAM wParam, LPARAM lParam);
    afx_msg LRESULT vLCtrlDoubleClick(WPARAM wParam, LPARAM lParam);
    afx_msg void OnParentNotify(UINT message, LPARAM lParam);
    afx_msg void OnEditClearAll();
    afx_msg void OnSendSelectedMessageEntry();
    afx_msg void OnExpandSelectedMessageEntry();


    afx_msg void OnImportLogFile();
    //kadoorafx_msg LRESULT vOpenCloseSignalWatchWnd(WPARAM wParam, LPARAM lParam);
    afx_msg LRESULT vInvalidateListDisplay(WPARAM wParam, LPARAM lParam);
    afx_msg LRESULT vOnGetInterpretState(WPARAM wParam, LPARAM lParam);
    afx_msg LRESULT vOnClearSortColumns(WPARAM wParam, LPARAM lParam);
    afx_msg LRESULT vOnUpdateConnectionStatus(WPARAM wParam, LPARAM lParam);
    afx_msg LRESULT vOnExpandCollapseMsg(WPARAM wParam, LPARAM lParam);
    afx_msg LRESULT vOnSetFocusMsgList(WPARAM wParam, LPARAM lParam);
    afx_msg LRESULT vOnGetNextPrevMsgIndex(WPARAM wParam, LPARAM lParam);
    afx_msg LRESULT OnListCtrlMsgDblClick(WPARAM wParam, LPARAM lParam);
    afx_msg LRESULT OnUpdateMsgIntrpWndPlcmnt(WPARAM wParam, LPARAM lParam);
    afx_msg LRESULT OnUpdateMsgTreeItemsPositions(WPARAM wParam, LPARAM lParam);
    afx_msg LRESULT OnClearAll(WPARAM wParam, LPARAM lParam);
    afx_msg LRESULT OnShowHideMessageWindow(WPARAM wParam, LPARAM lParam);
    afx_msg LRESULT OnToggleInterpretStatusAllEntries(WPARAM wParam, LPARAM lParam);
    afx_msg void OnImportLogPageNoChange();
    afx_msg void OnUpdateImportLogOverwriteScroll();
    afx_msg LRESULT OnImportLogOverwriteScroll(WPARAM wParam, LPARAM lParam);
    afx_msg void OnClose();
    afx_msg void OnMsgwndResetColumns();
    afx_msg void OnMDIActivate(BOOL bActivate, CWnd* pActivateWnd, CWnd* pDeactivateWnd);
    virtual BOOL PreTranslateMessage(MSG* pMsg);
    virtual std::string GetProtocolStringName();

};


