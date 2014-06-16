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
#include "TxWindowFlexRay_resource.h"
#include "Utility/XMLUtils.h"
#include "HashDefines.h"
//#include "..\Datatypes\FLEXRAY_Datatypes.h"
#include "txmsglist.h"
//#include "rxmsglist.h"
//#include "ReportCtrl.h"
//#include "FRAME_BaseUIDoc.h"
#include "..\Datatypes\FLEXRAY_Datatypes.h"

#include "HeaderCtrlEx.h"
//#include "EDITABLEGRID/EditFlexGrid.h"

#include "LinFlexListCtrl.h"

#include "TxLINDataStore.h"
#include "Utility/SignalMatrix.h"
#include "Utility/MsgInterpretation.h"  // For message interpretation

//******************Anish code start**********
#define defMESSAGE_BLOCK_NAME       _T("Name")
#define defMESSAGE_BLOCK_TRIGGER    _T("Type")
#define defMESSAGE_BLOCK_TRIG_VAL   _T("Value")
#define defMSG_CYCLIC               _T("CYCLIC")
#define defMSG_MONOSHOT             _T("MONOSHOT")
#define defMESSAGE_TRIG_MS          _T("ms")
#define defDEFAULT_TIMER_VAL        _T("100")
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
#define defDEFAULT_KEY_VAL          'A'

typedef struct tagSBYTE
{
    unsigned char Bit0 : 1;
    unsigned char Bit1 : 1;
    unsigned char Bit2 : 1;
    unsigned char Bit3 : 1;
    unsigned char Bit4 : 1;
    unsigned char Bit5 : 1;
    unsigned char Bit6 : 1;
    unsigned char Bit7 : 1;
} STRUCT_BYTE;

typedef union tagUBYTE
{
    BYTE        byByte;
    STRUCT_BYTE sByte;
} UNION_BYTE;

typedef enum
{
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

class CTransmitMsgLIN : public CFormView
{
public:
    CComboBox   m_omComboChannelID;
    CSignalMatrix    m_odSignalMatrix;
    CLinFlexListCtrl   m_omLctrSigList;
    CComboBox   m_omComboMsgIDorName;
    CButton m_omButtonAddMsg;
    CRadixEdit  m_odDLC;
    bool m_bFromEvent;
    /*CRadixEdit  m_odDB8;
    CRadixEdit  m_odDB7;
    CRadixEdit  m_odDB6;
    CRadixEdit  m_odDB5;
    CRadixEdit  m_odDB4;
    CRadixEdit  m_odDB3;
    CRadixEdit  m_odDB2;
    CRadixEdit  m_odDB1;*/

    CRadixEdit  m_odDB[8];
    BOOL    m_bIsRTR;
    CString m_omStrMsgIDorName;
    INT m_nChannelID;
private:

    CImageList m_omSigImageList;
    CImageList m_omHeaderImageList;

    BOOL m_bUpdating;
    bool m_bDataModified;
    UINT m_nDelay;
    unsigned short m_wTimerRes;
    BOOL    m_bStartEnabled;

    DECLARE_DYNCREATE(CTransmitMsgLIN)

    //protected:
    CTransmitMsgLIN();           // protected constructor used by dynamic creation
    virtual ~CTransmitMsgLIN();

    void vAdjustWidthMessageComboBox();


protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    DECLARE_MESSAGE_MAP()
public:
    BOOL            m_bModified;
    CButton         m_btnAutoUpdate;
    CButton         m_btnCyclic;
    CButton         m_btnMonoshot;
    CListCtrl m_omLctrMsgBlockName;
    int m_nSelectedMsgIndex;

    virtual void OnInitialUpdate();

    void vInitSignalListCtrl();
    void vEnableAddButton(BOOL bEnable);
    void vUpdateSelMsgDetails(eMsgUpdate eUpdateType, INT nItem, INT nDlc = -1, INT nChannel = -1, bool bEnable = false);

    int nGetCheckedItemCount();// find the List Item Checked Count


    enum { IDD = IDD_LIN_TRANSMIT_MSG };
#ifdef _DEBUG
    virtual void AssertValid() const;
    virtual void Dump(CDumpContext& dc) const;
#endif
    CLinFlexListCtrl m_lstMsg;
    afx_msg void OnBtnClickedAddnew();
    afx_msg void OnUpdateEditDLC();
    char chGetKeyVal(int nItem);
    void OnNMTransmitMessagesClick(NMHDR* pNMHDR, LRESULT* pResult);
    void OnNMTransmitMessagesRightClick(NMHDR* pNMHDR, LRESULT* pResult);
    void OnItemchangedLstcMsgDetails( NMHDR* pNMHDR, LRESULT* pResult );
    LIN_FRAME_DATA vGetMsgDetails(INT Index);

    INT nUpdateMessageDetails(LIN_FRAME_DATA& ouLinData);



    //afx_msg void OnNMClickTransmitMsg(NMHDR* pNMHDR, LRESULT* pResult);
    static unsigned int __stdcall ThreadFunc(void* pv);
    //CMultiMediaTimer m_MTTransmit;


    void vLoadTxMsgConfig();
    void OnBtnClose();
    void vUpdateFromRawValue(int nItem, int nSubItem);
    void vUpdateFromPhysicalValue(int nItem, int nSubItem);
    void vInitialiseDataBytes();
    void vUpdateSignalMatrix(void);
    void vUpdateDataBytes();
    void vUpdateDataBytes(LIN_FRAME_DATA& ouData);
    void psGetSelectedSignalStruct(std::list<SIGNAL_STRUCT> lstSigStrct, CString omStrSigName, SIGNAL_STRUCT& ouSigStrct);
    void vUpdateStateDataBytes();

    LIN_FRAME_DATA m_ouFrame;

    UINT m_bData [ defMAX_BYTE* defBITS_IN_BYTE ];
    UNION_BYTE m_unData[defMAX_BYTE];

    //CFRAME_BaseUIDoc* GetDocument() const;
    CHeaderCtrlEx m_wndHeader;

    CWnd* pomGetParentWindow() const;

    int nUpdateSelectedMessage();
    int nUpdateSelectedMessageInList(LIN_FRAME_DATA& ouLinData);



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
    void vClearAllViews();

    LRESULT vUserCommand(WPARAM wParam, LPARAM lParam);
    void OnBusConnect(ESTATUS_BUS eBusStatus);
    void vChangeDelButtonStatus(bool bStatus= false);

private:
    //Variables
    //CFlexListCtrl m_oFlexListCtrl;
    long    m_lCount;
    long    m_lCellPos;
    int     m_nSelDataColIndex;

    void DeleteAllMsgListColumns();
    void InitFlexList();
    void vRefreshTxMsgWindow();
protected:
    //{{AFX_MSG(CEditFlexGrid)

    //  afx_msg void OnBnClickedBtClose();

    //DECLARE_EVENTSINK_MAP()
    //}}AFX_MSG

public:

    void OnBnClickedBtDelAll();
    // Addition starts: RC
private:
    FRAME_STRUCT m_ouCurrentMsg;

    void vGetStringFromValue(int nValue, std::string& strValue, bool bHexSymbol = true);
    BYTE m_bDisplayColumns; // Columns to be displayed

public:
    static HWND m_hTransmitWnd;

    afx_msg void OnClose();
    afx_msg void OnSelchangeCombMsgIdName();
    afx_msg void onComboChnlSel();
    afx_msg void OnEditchangeCombMsgIdName();
    afx_msg void OnDestroy();
    afx_msg void OnAddMessageClick();
    afx_msg void onLinSlaveTypeSelected();
    afx_msg void onLinHeaderTypeSelected();
    afx_msg void OnClick(NMHDR* pNMHDR, LRESULT* pResult);

    afx_msg void OnUpdateEditDataBytes();
    afx_msg void OnKillfocusDataBytesEdit();
    afx_msg void OnItemchangedLstcSigDetails( NMHDR* pNMHDR, LRESULT* pResult );


    void vSetSignalValue(SIGNAL_STRUCT ouSigStrct, UCHAR aucData[8],
                         UINT64 u64SignVal);

    UINT64 un64GetBitMask(SIGNAL_STRUCT CurrSig);
    int m_nComboChannel;
    CComboBox m_wndComboChannel;

private:
    eLIN_MSG_TYPE m_CurrentMsgType;
    UINT unGetMsgIDFromName(CString omMsgName);
    BOOL bValidateData(LIN_FRAME_DATA& ouFrameData);
    INT nGetMessageID();
    BOOL bCheckIfValueIsMoreThan255(USHORT usNoOfEditCtrlsToCheck, UCHAR ucData[8] );
    BOOL bSetStatusText(const char* pStrText);

    void vSetDefaultValues();
    void vGetSelSignalFromFrame(UINT unMsgId, std::list<SIGNAL_STRUCT>& omSignalList);

    eLIN_MSG_TYPE eGetMessageType();
    int nGetChannel();
    static void vCheckBoxClickHandler( CListCtrl*, int nItem,int nSubItem, void* uParam);

    int nAddMessageToList(LIN_FRAME_DATA& ouLinData, int nRow = -1);
    void vUpdateMsgInList(LIN_FRAME_DATA ouLinData);
    void vUpdateViews(BOOL bIsUpdate);
    int nGetMessageName(std::string& strMsgName);
    int nGetDbFrame(FRAME_STRUCT& ouFrame);
    BOOL bUpdateSignalList(LIN_FRAME_DATA& sMsg);
    int nShowListOfCurrentSignls();
    void vUpdateMessageList();
    void vUpdateSignalMatrix(int nSelectedIndex, FRAME_STRUCT ouFrmData);
    int GetSelectedSignalStruct( SIGNAL_STRUCT& ouSignal, int nIndex );
    void vSetControlProperties();


};

//#ifndef _DEBUG  // debug version in FRAME_BaseUIView.cpp
//inline CFRAME_BaseUIDoc* CTransmitMsg::GetDocument() const
//   { return reinterpret_cast<CFRAME_BaseUIDoc*>(m_pDocument); }
//#endif