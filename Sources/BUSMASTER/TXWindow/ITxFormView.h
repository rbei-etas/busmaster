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
* \file      ITxFormView.h
* \author    Robin G.K.
* \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
*/


#pragma once
#include "stdafx.h"
#include "resource.h"
//#include "DataTypes/Cluster.h"
#include "Utility/SignalMatrix.h"
#include  "../Utility/FlexListCtrl.h"
#include "TxDataStore.h"
#include "ITransmitter.h"
#include "Utility\MultiLanguageSupport.h"

#define def_ADD_MESSAGE                                 "[Add Message]"


typedef enum eTXWNDDETAILS
{
    TX_MSG_BLOCK_COUNT,    // Message Block count
    TX_WND_SPLITTER_DATA,
    TX_WINDOW_PLACEMENT,
};

class ITxFormView : public CFormView
{
private:
    enum { IDD = IDD_LIN_TRANSMIT_MSG };

    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    DECLARE_MESSAGE_MAP()

    CImageList m_omHeaderImageList;

    /*bool m_bValidTxWndSize;*/
    bool m_bHexMode;
    BOOL m_bInitDone;
    ESTATUS_BUS m_eBusStatus;
    WINDOWPLACEMENT m_sTxWndPlacement;

    //Window Settings
    CWnd* GetResizableWnd();
    void GetTotalClientRect(LPRECT lpRect);

protected:

    struct MSG_LST_ROW
    {
        std::string Text;
        SNUMERICINFO NumInfo;
        SLISTINFO ListInfo;
        sUserProgInfo UserProgInfo;
        LVITEM lvitem;
        MSG_LST_ROW()
        {
            lvitem = { 0 };
        }
    };
    enum eSigColumns
    {
        eSigColName=0,
        eSigColRawVal=1,
        eSigColPhyVal=2,
        eSigColUnit=3
    };

    CTxDataStore* m_pouTxDataStore;
    IBMNetWorkGetService* m_pouIBMNetwork;
    ITransmitter* m_pouTransmitter;


    CSignalMatrix     m_odSignalMatrix;
    CFlexListCtrl  m_omLctrSigList;
    CFlexListCtrl     m_omLctrDataBytesList;
    CFlexListCtrl  m_lstMsg;

    ITxFormView();
    virtual ~ITxFormView();
    ITxFormView(const ITxFormView&);

    //Init Window Pos
    void SetDefaultWindowPos();

    //Init Views
    void vClearAllDataViews();
    void vClearAllViews();
    void vInitMsgListCtrl(std::list<std::pair<std::string, int>>& lstMsgDetails);
    void vInitDataByteListCtrl(bool bHexMode);
    void vInitSignalListCtrl();
    void vInitSigMatrix();

    //Update Views
    void vUpdateSignalList(bool bHexMode, std::list<SIG_DETAILS>& lstSigDetails);
    void vUpdateSignalMatrix(unsigned char* pchData, int nDlc);
    void vUpdateDataByteListCtrl(bool bHexMode, int nDlc, unsigned char* pchData);
    void UpdateDataAndSignalView(unsigned char* pchData, int nDlc, std::list<SIG_DETAILS>& lstSigDetails);


    //Helper Fns
    void vGetStringFromValue(int nValue, std::string& strValue);

    void vUpdateButtonStatus();

    void vAdjustWidthMessageComboBox(CComboBox* pComboBox);
    LRESULT vOnDataBytesUpdated(WPARAM wParam, LPARAM lParam);
    LRESULT vUserCommand(WPARAM wParam, LPARAM lParam);
    void vSetAllDataBytesNonEditable();
    void vOnSignalItemSelected(int nRow);
    void vOnSignalItemChanged(int nRow, int nColumn);
    unsigned __int64& FormatValue(eSignalDataType eDataType, int  nSiglength, unsigned __int64& un64RawValue);
    template<typename TIntType>
    TIntType& GetRawValueInRange(TIntType& RawValue,  TIntType MinValue,  TIntType MaxValue);
    void SetMsgLstRow(std::list<MSG_LST_ROW>& ouMsgLstProp, int nIndex);
    void SetMsgLstCol(std::map<int, SLISTINFO>& mapColAndProp);
    void GetDataBytesFromView(unsigned char*& chData);
    int nGetValidTimerValue(int nTimerVal);
    int OnDataBytesChanged(int nRow, unsigned char* pchData);
    void vGetDataBytesFromSignal(unsigned long long u64SignVal,std::pair<ISignal*, SignalInstanse>& Signal, int nDLC,unsigned char* pucData);
    void vSetSignalValue(int byte, int startBitIndexInByte, int length, bool bIntel, unsigned long long u64SignVal, unsigned char* aucData, int dataLenght);
    int HighlightSignalMatrix(int nDlc, const SignalInstanse& ouSignalInstance, ISignal*& pouSigStrct);
    UINT64 un64GetBitMask(int byte, int startBitIndexInByte, int length, bool bIntel, unsigned char* aucData, unsigned long long u64SignVal);
    void DisableAllColsInAddNewMsgRow();
    static void vSignalNameHandler(CListCtrl* pList, int nItem, int nSubItem, void* UParam);
    std::string GetDisplayValue(unsigned __int64 nValue, bool bIsHex, eSignalDataType eDatatype);
    //Handlers
    afx_msg void OnItemchangedLstcMsgDetails(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnItemchangedLstcSigDetails(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnBnClickedBtDelFrame();
    afx_msg void OnBnClickedBtDelAll();
    afx_msg void OnSendMessageClick();


public:

    static HWND m_hTransmitWnd;

    BOOL Create(CWnd* pParent);
    virtual void OnInitialUpdate();
    HRESULT UpdateTxView();
    int EnsureLastItemVisible();

    int SetBusStatus(ESTATUS_BUS eBusStatus);
    int SetHexMode(bool bIsHex);
    int SetConfigData(xmlDocPtr pDoc);
    int GetConfigData(xmlNodePtr pxmlNodePtr);

    bool GetTxData(eTXWNDDETAILS  eParam, LPVOID* lpData);

    int SetIBMNetwork(IBMNetWorkGetService* pouIBMNetwork);
    bool SetTxData(eTXWNDDETAILS  eParam, LPVOID lpVoid);

    int OnKeyPressed(char chKey);

    //Interface
    virtual int LoadAllMsgItems() = 0;
    virtual int OnMsgItemSelected(int nRow) = 0;
    virtual int OnMsgItemChanged(int nRow, int nColumn) = 0;
    virtual int OnBusStatusChanged(ESTATUS_BUS eBusStatus) = 0;
    virtual int GetConfigTag(std::string& strTag) = 0;
    virtual int AddNewMsgItemsToDataStore(int nCount)=0;
    virtual int GetMsgListDetails(std::list<std::pair<std::string, int>>& lstMsgDetails) = 0;
    virtual int GetEndianess(std::pair<ISignal*, SignalInstanse>& Signal, eEndianess& endian) = 0;
    virtual int SetOldFormatConfigData(xmlDocPtr pDoc) = 0;
};


















































