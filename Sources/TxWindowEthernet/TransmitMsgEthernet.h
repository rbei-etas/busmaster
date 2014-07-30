/**********************************************************************************
    Project         :   Frame
    FileName        :   TransmitMsg.h
    Description     :   Definition of CTransmitMsgLIN class
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
#include "TxWindowEthernet_resource.h"
#include "Utility/XMLUtils.h"
//#include "HashDefines.h"
//#include "..\Datatypes\FLEXRAY_Datatypes.h"
//#include "txmsglist.h"
//#include "rxmsglist.h"
//#include "ReportCtrl.h"
//#include "FRAME_BaseUIDoc.h"
#include "..\Datatypes\DIL_Datatypes.h"

//#include "HeaderCtrlEx.h"
//#include "EDITABLEGRID/EditFlexGrid.h"

//#include "LinFlexListCtrl.h"

#include "TxEthernetDataStore.h"
#include "Utility/SignalMatrix.h"
#include "Utility/MsgInterpretation.h"  // For message interpretation
#include "HeaderCtrlEx.h"
#include "Utility/FFListCtrl.h"
#include "LinFlexListCtrl.h"
#include "EditExtended.h"
#include "Base_TxEAppLayerProtocol.h"


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
#define defSTR_INVALID_MESSAGE_ID   "Invalid Message ID or Name"
#define defSTR_INVALID_DLC          "Invalid DLC value."
#define defSTR_INVALID_DATA_BYTES   "Invalid data for byte : %d"
#define defFORMAT_DATA_HEX          "%02X"
#define defFORMAT_DATA_DECIMAL      "%03d"
#define def_HEX_FORMAT              "%X"
#define def_DECIMAL_FORMAT          "%d"
#define defSTR_SIGNAL_NAME          "Name"
#define defSTR_PHYSICAL_COLUMN      "Physical Value"
#define defSTR_RAW_COLUMN           "Raw Value"
#define defSTR_UNITS                "Units"
#define defSTR_SIGNAL_UNIT          "Unit"

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

#define defSTR_SIGNAL_UNIT          "Unit"
#define def_SIG_NAME_COLUMN         0
#define def_RAW_VALUE_COLUMN        1
#define def_PHY_VALUE_COLUMN        2
#define def_SIG_UNIT_COLUMN         3
#define def_MAX_DATALEN_ETH			1500
//const CSize NOANCHOR(-1,-1),
//      TOP_LEFT(0,0), TOP_CENTER(50,0), TOP_RIGHT(100,0),
//      MIDDLE_LEFT(0,50), MIDDLE_CENTER(50,50), MIDDLE_RIGHT(100,50),
//      BOTTOM_LEFT(0,100), BOTTOM_CENTER(50,100), BOTTOM_RIGHT(100,100);

#define MAX_EDITGRID_COL 7

// CTransmitMsg form view\

typedef std::basic_string<TCHAR> TString;

//typedef struct tagTXWNDPARAMS
//{
//    //LOGPARAMS           m_sLogParams;
//    WINDOWPLACEMENT     m_sWndPlacement;
//    // To save Interpretation window co ordinates in Configuration
//} TXWNDPARAMS, *PTXWNDPARAMS;

//enum DATA_VIEW
//{
//    NONE,
//    BYTE_VIEW,
//    SIGNAL_VIEW,
//};
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

#define defBASE_DEC                         10
#define defBASE_HEX                         16
#define defCREATE_FAILED                    -1
#define defFORMAT_INT64_DECIMAL             "%I64d"
#define defFORMAT_UINT64_DEC                "%I64u"
#define defFORMAT_INT64_HEX                 "%I64X"
#define defSTR_FORMAT_PHY_VALUE             "%.3f"
#define defFLC_CREATE_FAILED                "Failed to create %s Control"
#define defCOMBO_ITEM                       "ComboItem"
#define defCOMBO_LIST                       "ComboLsit"
#define defEDIT_ITEM                        "EditItem"
#define defNUM_ITEM                         "NumItem"
#define defLIST_ITEM_TYPE_TABLE_SIZE        131
#define defLIST_NUM_DETAILS_TABLE_SIZE      67
#define defVIRTUAL_COL_COUNT                50
#define STR_EMPTY                           ""
#define defMAX_BITS                         64
#define defSIGN_MASK                0x8000000000000000

typedef union tagUBYTE
{
    BYTE        byByte;
    //STRUCT_BYTE sByte;
} UNION_BYTE;

typedef enum
{
	ePacketName,
	eSrcMAC,
	eDestMAC,
    eEnable,
    eKeyval,
    eKeyCheck,
    eRepetitionVal,
    eRepetitionCheck,
    eDataBytes,
    eDlc,
    eChannel,
    eAll
} eMsgUpdate;

class CTransmitMsgEthernet : public CFormView
{
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    DECLARE_MESSAGE_MAP()

private:
    //Variables
	BOOL							m_bUpdating;
    bool							m_bDataModified;
    UINT							m_nDelay;
    unsigned short					m_wTimerRes;
    BOOL							m_bStartEnabled;
	CImageList						m_omHeaderImageList;
    long							m_lCount;
    long							m_lCellPos;
    int								m_nSelDataColIndex;
	FRAME_STRUCT					m_ouCurrentMsg;
	CBase_TxEAppLayerProtocol*		m_pBaseTxAppProtocol;

    void DeleteAllMsgListColumns();
    void vRefreshTxMsgWindow();

    void vGetStringFromValue(int nValue, string& strValue, bool bHexSymbol = true);
    BYTE m_bDisplayColumns; // Columns to be displayed
    static void vCheckBoxClickHandler( CListCtrl*, int nItem,int nSubItem, void* uParam);
    int nAddMessageToList(ETHERNET_FRAME_DATA& ouLinData, int nRow = -1);
    void vUpdateMessageList();
    int GetSelectedSignalStruct( SIGNAL_STRUCT& ouSignal, int nIndex );

    DECLARE_DYNCREATE(CTransmitMsgEthernet)

    //protected:
    CTransmitMsgEthernet();           // protected constructor used by dynamic creation
    virtual ~CTransmitMsgEthernet();

public:

    bool m_bFromEvent;
    CLinFlexListCtrl m_omHeaderList;
	CEditExtended m_omEditFrameData;
	CEditExtended m_omEditRawFrame;
    CString m_omStrMsgIDorName;
    INT m_nChannelID;

	BOOL            m_bModified;
	bool			m_bIsDataSetByCode;
	bool			m_bIsRawSetByCode;
    CButton         m_btnAutoUpdate;
    CButton         m_btnCyclic;
    CButton         m_btnMonoshot;
    int m_nSelectedMsgIndex;

	virtual void OnInitialUpdate();
    void vEnableAddButton(BOOL bEnable);
    void vUpdateSelMsgDetails(eMsgUpdate eUpdateType, INT nItem, INT nDlc = -1, INT nChannel = -1, BOOL bEnable = FALSE);
    int nGetCheckedItemCount();// find the List Item Checked Count
    enum { IDD = IDD_DLG_ETHERNET_TX };

#ifdef _DEBUG
    virtual void AssertValid() const;
    virtual void Dump(CDumpContext& dc) const;
#endif
    afx_msg void OnBtnClickedAddnew();
    char chGetKeyVal(int nItem);
    void OnNMTransmitMessagesClick(NMHDR* pNMHDR, LRESULT* pResult);
    void OnNMTransmitMessagesRightClick(NMHDR* pNMHDR, LRESULT* pResult);
    void OnItemchangedLstcMsgDetails( NMHDR* pNMHDR, LRESULT* pResult );
    ETHERNET_FRAME_DATA vGetMsgDetails(INT Index);
    INT nUpdateMessageDetails(ETHERNET_FRAME_DATA& ouLinData);

    static unsigned int __stdcall ThreadFunc(void* pv);
    void vLoadTxMsgConfig();
    void OnBtnClose();
    void vInitialiseDataBytes();
    void vUpdateDataBytes(ETHERNET_FRAME_DATA& ouData);
    void psGetSelectedSignalStruct(list<SIGNAL_STRUCT> lstSigStrct, CString omStrSigName, SIGNAL_STRUCT& ouSigStrct);
	void vInitialiseDataList(int nDlc, int nItem);
    //UINT m_bData [ def_MAX_DATALEN_ETH* defBITS_IN_BYTE ];
    UNION_BYTE m_unData[def_MAX_DATALEN_ETH];
    CWnd* pomGetParentWindow() const;

    int nUpdateSelectedMessage();

    virtual CWnd* GetResizableWnd()
    {
        // make the layout know its parent window
        return this;
    };

    // override for scrollable or expanding parent windows
    virtual void GetTotalClientRect(LPRECT lpRect);

    HRESULT UpdateTxView(int& nChannelsConfigured);

    BOOL m_bInitDone;           // if all internal vars initialized
    BOOL m_bMsgLstInit;

    afx_msg void OnBnClickedBtUpdate();
    afx_msg void OnBnClickedBtDelFrame();
    void vDeleteFrameFrmList(INT nIndex);
    void vDeleteAllFrames();

    LRESULT vUserCommand(WPARAM wParam, LPARAM lParam);
    void OnBusConnect(ESTATUS_BUS eBusStatus);
    void vChangeDelButtonStatus(bool bStatus= false);

    static HWND m_hTransmitWnd;
    afx_msg void OnClose();
    afx_msg void OnDestroy();
    afx_msg void OnClick(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBnClickedBtnEthConfiguration();
	afx_msg void OnEnChangeEditData();
	afx_msg void OnEnChangeEditRaw();
	afx_msg void OnBnClickedBtnProtocolSetting();
};

//#ifndef _DEBUG  // debug version in FRAME_BaseUIView.cpp
//inline CFRAME_BaseUIDoc* CTransmitMsg::GetDocument() const
//   { return reinterpret_cast<CFRAME_BaseUIDoc*>(m_pDocument); }
//#endif