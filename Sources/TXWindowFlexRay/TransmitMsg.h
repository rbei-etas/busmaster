/**********************************************************************************
    Project         :   Frame
    FileName        :   TransmitMsg.h
    Description     :   Definition of CTransmitMsg class
    Directory       :
    Version         :   V1.0
    Authors         :   Ravi D
    Date Created    :   05/09/2006
    Date Modified   :
    Modifications   :
**********************************************************************************/

#pragma once
#include <afxcmn.h>
#include <afxwin.h>
#include "TxWindowFlexRay_resource.h"
#include "Utility/XMLUtils.h"
#include "HashDefines.h"
#include "..\Datatypes\FLEXRAY_Datatypes.h"
#include "txmsglist.h"
//#include "rxmsglist.h"
//#include "ReportCtrl.h"
//#include "FRAME_BaseUIDoc.h"


#include "HeaderCtrlEx.h"
//#include "EDITABLEGRID/EditFlexGrid.h"

#include "FlexListCtrl.h"

#include "TxFlexRayDataStore.h"

//******************Anish code start**********
#define defMESSAGE_BLOCK_NAME       _T("Name")
#define defMESSAGE_BLOCK_TRIGGER    _T("Type")
#define defMESSAGE_BLOCK_TRIG_VAL   _T("Value")
#define defMSG_CYCLIC               _T("CYCLIC")
#define defMSG_MONOSHOT             _T("MONOSHOT")
#define defMESSAGE_TRIG_MS          _T("ms")
#define defDEFAULT_TIMER_VAL        _T("100")
#define STR_EMPTY                   _T("")
#define defDEFAULT_KEY_VAL          'A'

#define defDEL_MSG_BLOCK            _T("You are about to delete a message block.\n\n\
Are you sure you want to delete?")


#define defMAIN_ITEM                0
#define defSUBITEM_MSGBLOCK_TYPE    1
#define defSUBITEM_MSGBLOCK_TRG_VAL 2
#define defSTR_TIMER_VAL_FMT_SPECIFIER      _T("%d")
#define defDEFAULT_TIME_VAL         100

#define defMESSAGE_BLOCK_COLUMN     3
#define defSTRING_SIZE              50
#define defMESSAGE_BLOCK_COLUMN     3

const CSize NOANCHOR(-1,-1),
      TOP_LEFT(0,0), TOP_CENTER(50,0), TOP_RIGHT(100,0),
      MIDDLE_LEFT(0,50), MIDDLE_CENTER(50,50), MIDDLE_RIGHT(100,50),
      BOTTOM_LEFT(0,100), BOTTOM_CENTER(50,100), BOTTOM_RIGHT(100,100);

#define MAX_EDITGRID_COL 7

// CTransmitMsg form view\

typedef std::basic_string<TCHAR> TString;

typedef struct tagTXWNDPARAMS
{
    //LOGPARAMS           m_sLogParams;
    WINDOWPLACEMENT     m_sWndPlacement;
    // To save Interpretation window co ordinates in Configuration
} TXWNDPARAMS, *PTXWNDPARAMS;





enum DATA_VIEW
{
    NONE,
    BYTE_VIEW,
    SIGNAL_VIEW,
};
//Anupam Kumar Code End
//******************Anish code start**********
#define defMESSAGE_BLOCK_NAME       _T("Name")
#define defMESSAGE_BLOCK_TRIGGER    _T("Type")
#define defMESSAGE_BLOCK_TRIG_VAL   _T("Value")
#define defMSG_CYCLIC               _T("CYCLIC")
#define defMSG_MONOSHOT             _T("MONOSHOT")
//#define defDEFAULT_MSG_BLOCK_NAME   _T("Group")
#define defMESSAGE_TRIG_MS          _T("ms")
#define defDEFAULT_MSG_BLOCK_NAME   _T("OnTime")
#define defDEFAULT_TIMER_VAL        _T("100")
#define STR_EMPTY                   _T("")
#define defDEFAULT_KEY_VAL          'A'

class CTransmitMsg : public CFormView
{
public:

private:
    bool m_bDataModified;
    UINT m_nDelay;
    unsigned short m_wTimerRes;
    BOOL    m_bStartEnabled;

    DECLARE_DYNCREATE(CTransmitMsg)

    //protected:
    CTransmitMsg();           // protected constructor used by dynamic creation
    virtual ~CTransmitMsg();

    void vInsertMessage(FRAME_STRUCT&);

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    DECLARE_MESSAGE_MAP()
public:
    BOOL            m_bModified;
    CButton         m_btnAutoUpdate;
    CButton         m_btnCyclic;
    CButton         m_btnMonoshot;
    CListCtrl m_omLctrMsgBlockName;

    virtual void OnInitialUpdate();
    int nGetCheckedItemCount();// find the List Item Checked Count


    enum { IDD = IDD_FRM_TRANSMIT_MSG };
#ifdef _DEBUG
    virtual void AssertValid() const;
    virtual void Dump(CDumpContext& dc) const;
#endif
    CTxMsgList m_lstMsg;
    afx_msg void OnBtnClickedAddnew();
    afx_msg void OnNMTransmitMessagesClick(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnNMTransmitMessagesRightClick(NMHDR* pNMHDR, LRESULT* pResult);

    CArray<RxMSG, RxMSG> m_aMsgList;
    afx_msg void OnNMClickTransmitMsg(NMHDR* pNMHDR, LRESULT* pResult);
    static unsigned int __stdcall ThreadFunc(void* pv);
    //CMultiMediaTimer m_MTTransmit;

    void vStartMsgTimerThreads();
    void vStopMsgTimerThreads();
    void vLoadTxMsgConfig();
    void OnBtnClose();

    //CFRAME_BaseUIDoc* GetDocument() const;
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
    CHeaderCtrlEx m_wndHeader;

    CWnd* pomGetParentWindow() const;




    virtual CWnd* GetResizableWnd()
    {
        // make the layout know its parent window
        return this;
    };

    // adjust children's layout, when parent's size changes


    // override for scrollable or expanding parent windows
    virtual void GetTotalClientRect(LPRECT lpRect);


    void vDisplayMsgListDetails(sFrameMsgManager* psTempFrameMsgBlock);

    HRESULT UpdateTxView(int& nChannelsConfigured);



    BOOL m_bInitDone;           // if all internal vars initialized
    BOOL m_bMsgLstInit;

    afx_msg void OnBnClickedBtUpdate();
    afx_msg void OnBnClickedBtDelFrame();

    LRESULT vUserCommand(WPARAM wParam, LPARAM lParam);
    LRESULT vOnDataBytesUpdated(WPARAM wParam, LPARAM lParam);
    void OnBusConnect(bool bConnect);
    void vChangeDelButtonStatus(bool bStatus= false);

private:
    DATA_VIEW m_ouCurrentView;
    //Variables
    CFlexListCtrl m_oFlexListCtrl;
    CFlxTxMsgBuffer* m_pouFlxTxMsgBuffer;
    CButton m_omBit[16];
    long    m_lCount;
    long    m_lCellPos;
    int     m_nSelDataColIndex;
    std::map<unsigned long, TString> m_oGridEditdata;

    void DisplayTxBlockFromHwConfig();
    void DeleteAllMsgListColumns();
    //Methods
    //void InitFlexList(BYTE yByteCount = 127, unsigned short* pwBuffer = NULL);
    void InitFlexList(DATA_VIEW ouDataView);
    void vRefreshTxMsgWindow();
    void SwitchView(DATA_VIEW);
protected:
    //{{AFX_MSG(CEditFlexGrid)

    //  afx_msg void OnBnClickedBtClose();

    DECLARE_EVENTSINK_MAP()
    //}}AFX_MSG

public:

    CEdit m_omEditTrgTimeIntervalVal;
    //int nGetBlockCount();

    void OnBnClickedBtDelAll();
    // Addition starts: RC
private:
    FLEXRAY_FRAME_DATA m_ouCurrentMsg;
    void vStoreDataBytes();
    void vStoreSignalValues();
    void vGetStringFromValue(int nValue, std::string& strValue);
    BYTE m_bDisplayColumns; // Columns to be displayed
    //temporary blockmanager
    //CMSGBLOCKMANAGER m_omMsgBlockMan;


public:
    static HWND m_hTransmitWnd;

    afx_msg LRESULT OnClickFlexListCtrl(WPARAM WParam, LPARAM LParam);
    afx_msg void OnClose();
    afx_msg void OnRadioSignalView();
    afx_msg void OnRadioByteView();

    afx_msg void OnDestroy();

    int m_nComboChannel;
    CComboBox m_wndComboChannel;
};

//#ifndef _DEBUG  // debug version in FRAME_BaseUIView.cpp
//inline CFRAME_BaseUIDoc* CTransmitMsg::GetDocument() const
//   { return reinterpret_cast<CFRAME_BaseUIDoc*>(m_pDocument); }
//#endif