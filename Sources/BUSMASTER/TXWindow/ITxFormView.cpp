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
* \file      ITxFormView.cpp
* \author    Robin G.K.
* \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
*/


#pragma once
#include "ITxFormView.h"
#include "TxMsgChildFrame.h"
#include "HashDefines.h"
#include "Utility\Utility.h"
#include "Utility\SignalDetailsDlg.h"
#include <bitset>

#define defSTR_PHYSICAL_COLUMN                          "Physical Value"
#define defSTR_RAW_COLUMN                               "Raw Value"
#define defSTR_SIGNAL_UNIT                              "Unit"
#define def_MAXIMUM_TIMER_VAL                           60000
#define def_MINIMUM_TIMER_VAL                           1
#define def_DATA_BYTE_HEX                               "Data Byte View (HEX)"
#define def_DATA_BYTE_DEC                               "Data Byte View (DEC)"

#define defSTR_CONFIRM_DELETEALL                        "Are you sure you want to delete all messages?"

HWND ITxFormView::m_hTransmitWnd = nullptr;

extern BOOL bCalcBitMaskForSig(BYTE* pbyMaskByte, UINT unArrayLen,
                               UINT nByteNum, UINT nBitNum, UINT nLength,
                               eEndianess bDataFormat);



BEGIN_MESSAGE_MAP(ITxFormView, CFormView)
    ON_NOTIFY(LVN_ITEMCHANGED, IDC_LSTC_TRANSMIT_MSG, OnItemchangedLstcMsgDetails)
    ON_NOTIFY(LVN_ITEMCHANGED, IDC_LSTC_SIG_DETAILS, OnItemchangedLstcSigDetails)
    ON_BN_CLICKED(IDC_DEL_FRAME, OnBnClickedBtDelFrame)
    ON_BN_CLICKED(IDC_BT_DEL_ALL, OnBnClickedBtDelAll)
    ON_BN_CLICKED(IDC_CBTN_SEND_MSG_LIN, OnSendMessageClick)
    ON_MESSAGE(WM_DATA_UPDATED, vOnDataBytesUpdated)
    ON_MESSAGE(WM_USER + 44, vUserCommand)
END_MESSAGE_MAP()

ITxFormView::ITxFormView()
    : CFormView(ITxFormView::IDD)
{
    m_bInitDone = FALSE;
    /*m_bValidTxWndSize = false;*/
    m_pouTxDataStore = new CTxDataStore();
    m_pouIBMNetwork = nullptr;
    m_pouTransmitter = nullptr;
    m_eBusStatus = BUS_DISCONNECTED;
    SetDefaultWindowPos();
}

ITxFormView::~ITxFormView()
{
    delete m_pouTxDataStore;
    m_pouTxDataStore = nullptr;
}
void ITxFormView::SetDefaultWindowPos()
{
    m_sTxWndPlacement.flags = 1;
    m_sTxWndPlacement.length = 44;
    m_sTxWndPlacement.ptMaxPosition.x = 0;
    m_sTxWndPlacement.ptMaxPosition.x = 0;
    m_sTxWndPlacement.ptMinPosition.x = 0;
    m_sTxWndPlacement.ptMinPosition.y = 0;
    m_sTxWndPlacement.rcNormalPosition.top = 1;
    m_sTxWndPlacement.rcNormalPosition.bottom = 661;
    m_sTxWndPlacement.rcNormalPosition.left = 4;
    m_sTxWndPlacement.rcNormalPosition.right = 864;
    m_sTxWndPlacement.showCmd = 1;
}
void ITxFormView::DoDataExchange(CDataExchange* pDX)
{
    CFormView::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_STAT_MATRIX_RECT, m_odSignalMatrix);
    DDX_Control(pDX, IDC_LSTC_SIG_DETAILS, m_omLctrSigList);
    DDX_Control(pDX, IDC_LSTC_DATA_BYTES, m_omLctrDataBytesList);
}

void ITxFormView::OnItemchangedLstcMsgDetails(NMHDR* pNMHDR, LRESULT* pResult)
{
    NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
    if (pNMListView->uChanged != LVIF_TEXT)
    {
        int nPrevMsgItem = 0;
        // Selected & Focused
        if (pNMListView->uNewState & (LVIS_FOCUSED | LVIS_SELECTED))
        {
            // For Row selection
            if (pNMListView->iItem != -1)
            {
                CString omStrMsgNameId = m_lstMsg.GetItemText(pNMListView->iItem, 0);
                if (omStrMsgNameId != def_ADD_MESSAGE)
                {
                    OnMsgItemSelected(pNMListView->iItem);
                }
                else
                {
                    vClearAllDataViews();
                }

                m_lstMsg.SetItemState(pNMListView->iItem, LVIS_FOCUSED | LVIS_SELECTED, LVIS_FOCUSED|LVIS_SELECTED);
                m_lstMsg.SetSelectionMark(pNMListView->iItem);
                vUpdateButtonStatus();
            }
        }
        else if (pNMListView->uChanged == LVIF_STATE && pNMListView->uNewState == 0)
        {
            int nItem = m_lstMsg.GetSelectionMark();
            m_lstMsg.SetItemState(nItem, 0, LVIS_FOCUSED | LVIS_SELECTED);
            m_lstMsg.SetSelectionMark(-1);
            vClearAllDataViews();
            vUpdateButtonStatus();
        }

    }
    //If Text is Changed...
    if (pNMListView->uChanged == LVIF_TEXT)
    {
        LV_DISPINFO ls;
        if (m_lstMsg.sGetModificationStructure(ls) == TRUE)
        {
            CString omStrMsgNameId = m_lstMsg.GetItemText(pNMListView->iItem, 0);
            if (omStrMsgNameId != def_ADD_MESSAGE)
            {
                OnMsgItemChanged(pNMListView->iItem, ls.item.iSubItem);
            }
        }

    }
    *pResult = 0;
}
void ITxFormView::OnItemchangedLstcSigDetails(NMHDR* pNMHDR, LRESULT* pResult)
{
    NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
    // Update Matrix while selection got changed or updating
    // through program
    if (pNMListView->uChanged != LVIF_TEXT)
    {
        // Selected & Focused
        if (pNMListView->uNewState == (LVIS_FOCUSED | LVIS_SELECTED))
        {
            vOnSignalItemSelected(pNMListView->iItem);
        }
    }
    // Notification for text change
    if (pNMListView->uChanged == LVIF_TEXT)
    {
        LV_DISPINFO ls;
        // Get the change information from the list control
        if (m_omLctrSigList.sGetModificationStructure(ls) == TRUE)
        {
            vOnSignalItemChanged(ls.item.iItem, ls.item.iSubItem);
        }
    }
    //vCallApplyChanges();

    *pResult = 0;
}

void ITxFormView::OnBnClickedBtDelFrame()
{
    int nSelItem = m_lstMsg.GetNextItem(-1, LVNI_SELECTED);
    if (nullptr == m_pouTxDataStore)
    {
        return;
    }
    if (-1 != nSelItem)
    {
        m_pouTxDataStore->DeleteMsgItemAt(nSelItem);
        m_lstMsg.DeleteItem(nSelItem);
        int nCount = m_lstMsg.GetItemCount();
        if (nCount > 0)
        {
            if (nCount == 1)
            {
                nSelItem = nCount-1;
            }
            else
            {
                nSelItem = nSelItem % (nCount - 1);
            }
            m_lstMsg.SetItemState(nSelItem, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
            m_lstMsg.SetSelectionMark(nSelItem);
        }
        DisableAllColsInAddNewMsgRow();

    }
    vUpdateButtonStatus();
}

void ITxFormView::OnBnClickedBtDelAll()
{
    if (IDYES == AfxMessageBox(_T(defSTR_CONFIRM_DELETEALL), MB_YESNO | MB_DEFBUTTON2))
    {
        if (nullptr == m_pouTxDataStore)
        {
            return;
        }
        m_pouTxDataStore->DeleteAllMsgItems();
        LoadAllMsgItems();
    }
}
void ITxFormView::OnSendMessageClick()
{
    int nItem = m_lstMsg.GetSelectionMark();
    if (nItem == -1)
    {
        return;
    }
    if (nullptr == m_pouTransmitter)
    {
        return;
    }
    m_pouTransmitter->TransmitAt(nItem);
}
CWnd* ITxFormView::GetResizableWnd()
{
    return this;
}
BOOL ITxFormView::Create(CWnd* pParent)
{
    CRect rect;
    pParent->GetClientRect(rect);
    return CFormView::Create(NULL, NULL, WS_CHILD | WS_VISIBLE, rect, pParent, 0, NULL);
}

void ITxFormView::OnInitialUpdate()
{
    CFormView::OnInitialUpdate();

    ResizeParentToFit(TRUE);
    m_bInitDone = TRUE;

    std::list<std::pair<std::string, int>> lstMsgDetails;
    GetMsgListDetails(lstMsgDetails);

    vInitMsgListCtrl(lstMsgDetails);
    vInitSignalListCtrl();
    vInitDataByteListCtrl(m_bHexMode);
    vInitSigMatrix();
    UpdateTxView();
}
void ITxFormView::GetTotalClientRect(LPRECT lpRect)
{
    GetClientRect(lpRect);

    // get scrollable size
    CSize size = GetTotalSize();

    // before initialization, "size" is dialog template size
    if (!m_bInitDone)
    {
        lpRect->right = lpRect->left + size.cx;
        lpRect->bottom = lpRect->top + size.cy;
        return;
    }

    // otherwise, give correct size if scrollbars active

    if (m_nMapMode < 0) // scrollbars disabled
    {
        return;
    }

    // enlarge reported client area when needed
    CRect rect(lpRect);
    if (rect.Width() < size.cx)
    {
        lpRect->right = lpRect->left + size.cx;
    }
    if (rect.Height() < size.cy)
    {
        lpRect->bottom = lpRect->top + size.cy;
    }
}

void ITxFormView::vClearAllDataViews()
{
    // Init Signal List control
    m_omLctrSigList.DeleteAllItems();
    //Clear Data Bytes
    m_omLctrDataBytesList.DeleteAllItems();
    // Clear the matrix
    m_odSignalMatrix.vResetValues();
    m_odSignalMatrix.vSetMessageLength(0);
}
void ITxFormView::vClearAllViews()
{
    m_lstMsg.DeleteAllItems();
    vClearAllDataViews();
}
void ITxFormView::vInitMsgListCtrl(std::list<std::pair<std::string, int>>& lstMsgDetails)
{
    INT nTotalColunmSize = 0;
    INT nTotalStrLengthPixel = 0;
    INT nColumnSize = 0;

    m_lstMsg.SubclassDlgItem(IDC_LSTC_TRANSMIT_MSG, this);
    m_lstMsg.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_SUBITEMIMAGES | LVS_EX_GRIDLINES);

    CWnd* pWnd = m_lstMsg.GetWindow(GW_CHILD);
    ASSERT(pWnd != nullptr);
    CRect rctList;
    m_lstMsg.GetWindowRect(&rctList);
    int nColWidth = rctList.Width() - 2;
    int total_cx = 0;

    LVCOLUMN lvcolumn;
    memset(&lvcolumn, 0, sizeof(lvcolumn));
    lvcolumn.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
    // Associate Header control Image List
    if (m_omHeaderImageList.m_hImageList == nullptr)
    {
        m_omHeaderImageList.Create(IDR_BMP_CHECKBOX, 16, 1, RGB(0, 0, 255));
    }
    // Set the Image List
    // Only if it is sucessfully created
    if (m_omHeaderImageList.m_hImageList != nullptr)
    {
        m_lstMsg.SetImageList(&m_omHeaderImageList, LVSIL_SMALL);
        CHeaderCtrl* pHeader = m_lstMsg.GetHeaderCtrl();
        if (pHeader != nullptr)
        {
            pHeader->SetImageList(&m_omHeaderImageList);
            HDITEM hditem;
            hditem.mask = HDI_IMAGE | HDI_FORMAT;
            if (pHeader->GetItem(0, &hditem) == TRUE)
            {
                hditem.fmt |= HDF_IMAGE;
                hditem.iImage = 0;
                pHeader->SetItem(0, &hditem);
                //pHeader->SetItem(6, &hditem );
            }
        }
    }
    m_hTransmitWnd = m_hWnd;

    int nCol = 0;
for (auto itr : lstMsgDetails)
    {
        // Column formating
        if ((nCol == FIRST_COL)) //4 col - Cycle No 6-DataLen , 7 - DataBytes
        {
            lvcolumn.fmt = LVCFMT_LEFT;
        }

        else
        {
            lvcolumn.fmt = LVCFMT_CENTER;
        }

        lvcolumn.pszText = const_cast<char*>(itr.first.c_str());
        lvcolumn.cchTextMax = (int)strlen(itr.first.c_str());
        lvcolumn.iSubItem = nCol;
        lvcolumn.cx = (nColWidth * itr.second) / 64;
        total_cx += lvcolumn.cx;

        m_lstMsg.InsertColumn(nCol, &lvcolumn);
        nCol++;
    }
}
void ITxFormView::vInitDataByteListCtrl(bool bHexMode)
{
    CHAR caColumnName[][defSIGNAL_LIST_STRING_MAX_LENGTH]
        = { "Index",
            "00",
            "01",
            "02",
            "03",
            "04",
            "05",
            "06",
            "07"
          };
    INT nColumnFormat[] = { LVCFMT_RIGHT,
                            LVCFMT_CENTER,
                            LVCFMT_CENTER,
                            LVCFMT_CENTER,
                            LVCFMT_CENTER,
                            LVCFMT_CENTER,
                            LVCFMT_CENTER,
                            LVCFMT_CENTER,
                            LVCFMT_CENTER
                          };
    RECT rListCtrlRect;
    INT nTotalColunmSize = 0;
    INT nTotalStrLengthPixel = 0;
    INT nColumnSize = 0;

    m_omLctrDataBytesList.GetWindowRect(&rListCtrlRect);
    nTotalColunmSize = rListCtrlRect.right - rListCtrlRect.left;
    nTotalStrLengthPixel = 0;

    UINT i; //i declared outside loop
    for (i = 0; i<defSIGNAL_FRAME_COLUMN; i++)
    {
        nTotalStrLengthPixel +=
            m_omLctrDataBytesList.GetStringWidth(caColumnName[i]);
    }
    //Insert each column name after calculating the size for the same.
    for (i = 0; i<defSIGNAL_FRAME_COLUMN; i++)
    {
        nColumnSize = m_omLctrDataBytesList.GetStringWidth(caColumnName[i]);
        nColumnSize +=
            (nTotalColunmSize - nTotalStrLengthPixel) / defSIGNAL_FRAME_COLUMN;
        m_omLctrDataBytesList.InsertColumn(i, caColumnName[i],
                                           nColumnFormat[i], nColumnSize - 1);
    }

    // Set the extended style
    m_omLctrDataBytesList.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);
    SNUMERICINFO    sNumInfo;
    SLISTINFO       sListInfo;

    sListInfo.m_eType = eBuddy;

    sNumInfo.m_uMaxVal.m_n64Value = 256;
    sNumInfo.m_uMinVal.m_n64Value = 0;
    sNumInfo.m_byFlag = SIGNED_VAL;


    if (bHexMode == false)
    {
        sNumInfo.m_byBase = BASE_DECIMAL;
        sNumInfo.m_nTextLimit = 3;
    }
    else
    {
        sNumInfo.m_byBase = BASE_HEXADECIMAL;
        sNumInfo.m_nTextLimit = 2;
    }

    m_omLctrDataBytesList.vSetColumnInfo(0, 1, sListInfo);
    m_omLctrDataBytesList.vSetNumericInfo(0, 1, sNumInfo);
    vSetAllDataBytesNonEditable();
}
void ITxFormView::vInitSignalListCtrl()
{
    char caColumnName[4][defSIGNAL_LIST_STRING_MAX_LENGTH];
    strncpy(caColumnName[0], defSTR_COL_SIGNAL_NAME, defSIGNAL_LIST_STRING_MAX_LENGTH);
    strncpy(caColumnName[1], defSTR_RAW_COLUMN, defSIGNAL_LIST_STRING_MAX_LENGTH);
    strncpy(caColumnName[2], defSTR_PHYSICAL_COLUMN, defSIGNAL_LIST_STRING_MAX_LENGTH);
    strncpy(caColumnName[3], defSTR_SIGNAL_UNIT, defSIGNAL_LIST_STRING_MAX_LENGTH);

    INT nColumnFormat[] = { LVCFMT_LEFT,
                            LVCFMT_CENTER,
                            LVCFMT_CENTER,
                            LVCFMT_LEFT
                          };
    RECT rListCtrlRect;
    INT nTotalColunmSize = 0;
    INT nTotalStrLengthPixel = 0;
    INT nColumnSize = 0;


    //Calculate the total size of all column header
    m_omLctrSigList.GetWindowRect(&rListCtrlRect);
    nTotalColunmSize = rListCtrlRect.right - rListCtrlRect.left;
    nTotalStrLengthPixel = 0;

    UINT i; //i declared outside loop
    for (i = 0; i<4; i++)
    {
        nTotalStrLengthPixel +=
            m_omLctrSigList.GetStringWidth(caColumnName[i]);
    }
    //Insert each column name after calculating the size for the same.
    for (i = 0; i<4; i++)
    {
        nColumnSize = m_omLctrSigList.GetStringWidth(caColumnName[i]);
        nColumnSize +=
            (nTotalColunmSize - nTotalStrLengthPixel) / defSIGNAL_FRAME_COLUMN;
        m_omLctrSigList.InsertColumn(i, caColumnName[i],
                                     nColumnFormat[i], nColumnSize - 1);
    }

    // Set the extended style
    m_omLctrSigList.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);
    SLISTINFO       sListInfo;

    // Init Signal Name column
    // Column 0 : User Function call
    sListInfo.m_eType = eUser;
    m_omLctrSigList.vSetColumnInfo(0, 0, sListInfo);

    SNUMERICINFO sNumInfo;

    sNumInfo.m_uMinVal.m_un64Value = 0;
    sNumInfo.m_uMaxVal.m_un64Value = 100;
    sNumInfo.m_uDelta.m_un64Value = 1;

    sNumInfo.m_byFlag = BUDDY_CTRL;
    // Physical value should be in dec always
    sNumInfo.m_byBase = defBASE_DEC;

    // Init Raw Value column
    // Column 1 : Numeric Edit with Spin Control
    sListInfo.m_eType = eBuddy;
    m_omLctrSigList.vSetColumnInfo(0, 1, sListInfo);
    m_omLctrSigList.vSetNumericInfo(0, 1, sNumInfo);

    // Init Phy Value column
    // Column 2 : Numeric Edit with Spin Control
    sListInfo.m_eType = eBuddy;
    m_omLctrSigList.vSetColumnInfo(0, 2, sListInfo);
    m_omLctrSigList.vSetNumericInfo(0, 2, sNumInfo);
}
void ITxFormView::vInitSigMatrix()
{
    m_odSignalMatrix.vResetValues();
    m_odSignalMatrix.vSetMessageLength(0);
}

void ITxFormView::vUpdateSignalList(bool /*bHexMode*/, std::list<SIG_DETAILS>& lstSigDetails)
{
    std::string strInit = "";
    int nCount = 0;
    SLISTINFO sListInfo;
    SNUMERICINFO sPhyNumInfo,sRawNumInfo;
    SUSERPROGINFO sProgInfo;
    unsigned __int64 unMin = 0, unMax = 0;
    unsigned int unLength = 0;
    double dMin = 0, dMax = 0, dInit = 0;
    int nTopIndex = m_omLctrSigList.GetTopIndex(); //To reset the scroll pos to prev value

    m_omLctrSigList.DeleteAllItems();
    eSignalDataType eDataType = eUnsigned;

for (auto itrSig: lstSigDetails)
    {
        //Signal name;
        strInit = itrSig.SignalInterprete.m_omSigName;
        sListInfo.m_eType = eUser;
        m_omLctrSigList.InsertItem(nCount, strInit.c_str());
        sProgInfo.m_pfHandler = vSignalNameHandler;
        sProgInfo.m_pUserParam = this;
        m_omLctrSigList.vSetColumnInfo(nCount, eSigColName, sListInfo);
        m_omLctrSigList.vSetUserProgInfo(nCount, eSigColName, sProgInfo);
        //Unit
        strInit = itrSig.SignalInterprete.m_omUnit;
        sListInfo.m_eType = eNoControl;
        m_omLctrSigList.vSetColumnInfo(nCount, eSigColUnit, sListInfo);
        m_omLctrSigList.SetItemText(nCount, eSigColUnit, strInit.c_str());

        //Raw Value
        strInit = itrSig.SignalInterprete.m_omRawValue;
        itrSig.Signal.first->GetMinMaxValue(unMin, unMax);

        itrSig.Signal.first->GetDataType(eDataType);
        if (eUnsigned == eDataType)
        {
            sRawNumInfo.m_byFlag = BUDDY_CTRL;
            sRawNumInfo.m_uMinVal.m_un64Value = unMin;
            sRawNumInfo.m_uMaxVal.m_un64Value = unMax;
            if (sRawNumInfo.m_uMinVal.m_un64Value > sRawNumInfo.m_uMaxVal.m_un64Value)
            {
                std::swap(sRawNumInfo.m_uMinVal.m_un64Value, sRawNumInfo.m_uMaxVal.m_un64Value);
            }
            sRawNumInfo.m_uDelta.m_un64Value = 1;
        }
        else if (eSigned == eDataType)
        {
            sRawNumInfo.m_byFlag = BUDDY_CTRL | SIGNED_VAL;
            sRawNumInfo.m_uMinVal.m_n64Value = static_cast<__int64>(unMin);
            sRawNumInfo.m_uMaxVal.m_n64Value = static_cast<__int64>(unMax);
            if (sRawNumInfo.m_uMinVal.m_n64Value > sRawNumInfo.m_uMaxVal.m_n64Value)
            {
                std::swap(sRawNumInfo.m_uMinVal.m_n64Value, sRawNumInfo.m_uMaxVal.m_n64Value);
            }
            sRawNumInfo.m_uDelta.m_n64Value = 1;
        }


        itrSig.Signal.first->GetLength(unLength);
        sRawNumInfo.m_nSigLength = unLength;
        if (true == m_bHexMode)
        {
            sRawNumInfo.m_byBase = defBASE_HEX;
        }
        else
        {
            sRawNumInfo.m_byBase = defBASE_DEC;
        }

        sListInfo.m_eType = eNumber;
        m_omLctrSigList.vSetColumnInfo(nCount, eSigColRawVal, sListInfo);
        m_omLctrSigList.vSetNumericInfo(nCount, eSigColRawVal, sRawNumInfo);
        m_omLctrSigList.SetItemText(nCount, eSigColRawVal, strInit.c_str());

        //Physical Value
        strInit = itrSig.SignalInterprete.m_omEnggValue;
        itrSig.Signal.first->GetEnggValueFromRaw(unMin, dMin);
        itrSig.Signal.first->GetEnggValueFromRaw(unMax, dMax);
        if (dMin > dMax)
        {
            std::swap(dMax, dMin);
        }
        if (eUnsigned == eDataType)
        {
            sPhyNumInfo.m_byFlag = BUDDY_CTRL | FLOAT_VAL;
            sPhyNumInfo.m_uMinVal.m_dValue = dMin;
            sPhyNumInfo.m_uMaxVal.m_dValue = dMax;
            sPhyNumInfo.m_uDelta.m_dValue = itrSig.dPhyFactor;
        }
        else if (eSigned == eDataType)
        {
            sPhyNumInfo.m_byFlag = BUDDY_CTRL | FLOAT_VAL | SIGNED_VAL;
            sPhyNumInfo.m_uMinVal.m_dValue = dMin;
            sPhyNumInfo.m_uMaxVal.m_dValue = dMax;
            sPhyNumInfo.m_uDelta.m_dValue = itrSig.dPhyFactor;
        }

        sPhyNumInfo.m_nSigLength = unLength;
        sPhyNumInfo.m_byBase = defBASE_DEC;

        sListInfo.m_eType = eNumber;
        m_omLctrSigList.vSetColumnInfo(nCount, eSigColPhyVal, sListInfo);
        m_omLctrSigList.vSetNumericInfo(nCount, eSigColPhyVal, sPhyNumInfo);
        m_omLctrSigList.SetItemText(nCount, eSigColPhyVal, strInit.c_str());

        nCount++;
    }
    m_omLctrSigList.EnsureVisible(m_omLctrSigList.GetItemCount() - 1,TRUE); // Scroll down to the bottom
    m_omLctrSigList.EnsureVisible(nTopIndex,TRUE);
}
void ITxFormView::vUpdateSignalMatrix(unsigned char* pchData, int nDlc)
{
    m_odSignalMatrix.vSetMessageLength(nDlc);
    m_odSignalMatrix.vSetByteValue(pchData);
}
void ITxFormView::vUpdateDataByteListCtrl(bool bHexMode, int nDlc, unsigned char* pchData)
{
    m_omLctrDataBytesList.DeleteAllItems();
    vSetAllDataBytesNonEditable();
    for (DWORD j = 0; j < nDlc; j++)
    {
        unsigned char ucCurrData = pchData[j];


        //setting data to the signal list
        int nRow = (j / 8);         //row number is current (WORD number / 8) , since there r 8 colums
        int nCol = (j % 8) + 1;     //since there r 8 columns

        CString     omstrData;
        SLISTINFO       sListInfo;
        SNUMERICINFO    sNumInfo;
        sListInfo.m_eType = eKeyBuddy;
        sNumInfo.m_uMaxVal.m_n64Value = 255;
        sNumInfo.m_uMinVal.m_n64Value = 0;
        sNumInfo.m_byFlag = SIGNED_VAL;
        if (bHexMode == true)
        {
            sNumInfo.m_nTextLimit = 2;
            sNumInfo.m_byBase = BASE_HEXADECIMAL;
        }
        else
        {
            sNumInfo.m_nTextLimit = 3;
            sNumInfo.m_byBase = BASE_DECIMAL;
        }
        m_omLctrDataBytesList.vSetColumnInfo(nRow, nCol, sListInfo);
        m_omLctrDataBytesList.vSetNumericInfo(nRow, nCol, sNumInfo);

        CString strString;
        strString.Format("%03d", j);
        if (nCol == 1)       //Insert the row only once after writing 8 columns, else a row will b inserted 8 times
        {
            m_omLctrDataBytesList.InsertItem(j, strString);
        }


        if (true == bHexMode)
        {
            strString.Format("%02X", ucCurrData);
        }
        else
        {
            strString.Format("%03d", ucCurrData);
        }
        m_omLctrDataBytesList.SetItemText(nRow, nCol, strString);
    }
}


void ITxFormView::UpdateDataAndSignalView(unsigned char* pchData, int nDlc, std::list<SIG_DETAILS>& lstSigDetails)
{
    vUpdateDataByteListCtrl(m_bHexMode, nDlc, pchData);
    vUpdateSignalList(m_bHexMode,lstSigDetails);
    vUpdateSignalMatrix(pchData, nDlc);
}

void ITxFormView::vGetStringFromValue(int nValue, std::string& strValue)
{
    char chValue[1024];
    if (m_bHexMode == false)
    {
        sprintf_s(chValue, 1024, "%d", nValue);
    }
    else
    {
        sprintf_s(chValue, 1024, "0x%X", nValue);
    }
    strValue = chValue;
}


void ITxFormView::vUpdateButtonStatus()
{
    BOOL bSendMsgEnable = FALSE;
    BOOL bDeleteFrameEnable = FALSE;
    BOOL bDeleteAllFrameEnable = FALSE;
    if (m_lstMsg.GetItemCount() > 1)
    {
        if (m_eBusStatus == BUS_CONNECTED)
        {
            bSendMsgEnable = TRUE;
            bDeleteFrameEnable = FALSE;
            bDeleteAllFrameEnable = FALSE;
        }
        else if (m_eBusStatus == BUS_DISCONNECTED)
        {
            bSendMsgEnable = FALSE;
            bDeleteFrameEnable = TRUE;
            bDeleteAllFrameEnable = TRUE;
        }
        int nSel = m_lstMsg.GetSelectionMark();
        if (nSel == -1 || (nSel != -1 && def_ADD_MESSAGE == m_lstMsg.GetItemText(nSel, 0)))
        {
            bDeleteFrameEnable = FALSE;
            bSendMsgEnable = FALSE;

        }
    }
    GetDlgItem(IDC_CBTN_SEND_MSG_LIN)->EnableWindow(bSendMsgEnable);
    GetDlgItem(IDC_DEL_FRAME)->EnableWindow(bDeleteFrameEnable);
    GetDlgItem(IDC_BT_DEL_ALL)->EnableWindow(bDeleteAllFrameEnable);
}

LRESULT ITxFormView::vOnDataBytesUpdated(WPARAM /*wParam*/, LPARAM /*lParam*/)
{
    int nRow = m_lstMsg.GetSelectionMark();
    unsigned char chData[MAX_PATH] = { 0 };
    unsigned char* pchData = chData;
    GetDataBytesFromView(pchData);
    if (-1 != nRow)
    {
        OnDataBytesChanged(nRow, pchData);
    }
    return S_OK;
}
LRESULT ITxFormView::vUserCommand(WPARAM wParam, LPARAM lParam)
{
    int nUserSel = 0;
    nUserSel = static_cast <int>(wParam);
    switch (nUserSel)
    {
            //Change to Numeric Mode.
        case 0:
            m_bHexMode = (lParam!=0);
            UpdateTxView();
            break;
            //Change BUS Status.
        case 2:
            //SetBusStatus must be called before Updating Tx Window.
            m_eBusStatus = static_cast<ESTATUS_BUS>(lParam);
            OnBusStatusChanged(m_eBusStatus);
            break;
    }
    return S_OK;
}
void ITxFormView::vSetAllDataBytesNonEditable()
{
    SNUMERICINFO    sNumInfo;
    SLISTINFO       sListInfo;

    sNumInfo.m_uMaxVal.m_n64Value = 256;
    sNumInfo.m_uMinVal.m_n64Value = 0;
    sNumInfo.m_byFlag = SIGNED_VAL;

    // Column 2 : Numeric Edit with Spin Control
    sListInfo.m_eType = eNoControl;
    m_omLctrDataBytesList.vSetColumnInfo(0, 2, sListInfo);
    m_omLctrDataBytesList.vSetNumericInfo(0, 2, sNumInfo);

    sListInfo.m_eType = eNoControl;
    m_omLctrDataBytesList.vSetColumnInfo(0, 3, sListInfo);
    m_omLctrDataBytesList.vSetNumericInfo(0, 3, sNumInfo);

    sListInfo.m_eType = eNoControl;
    m_omLctrDataBytesList.vSetColumnInfo(0, 4, sListInfo);
    m_omLctrDataBytesList.vSetNumericInfo(0, 4, sNumInfo);

    sListInfo.m_eType = eNoControl;
    m_omLctrDataBytesList.vSetColumnInfo(0, 5, sListInfo);
    m_omLctrDataBytesList.vSetNumericInfo(0, 5, sNumInfo);

    sListInfo.m_eType = eNoControl;
    m_omLctrDataBytesList.vSetColumnInfo(0, 6, sListInfo);
    m_omLctrDataBytesList.vSetNumericInfo(0, 6, sNumInfo);

    sListInfo.m_eType = eNoControl;
    m_omLctrDataBytesList.vSetColumnInfo(0, 7, sListInfo);
    m_omLctrDataBytesList.vSetNumericInfo(0, 7, sNumInfo);

    sListInfo.m_eType = eNoControl;
    m_omLctrDataBytesList.vSetColumnInfo(0, 8, sListInfo);
    m_omLctrDataBytesList.vSetNumericInfo(0, 8, sNumInfo);
}
void ITxFormView::vOnSignalItemSelected(int nRow)
{
    if (nullptr == m_pouTxDataStore)
    {
        return;
    }
    ITxMsgItem* pouMsgItem = nullptr;
    int nIndex = m_lstMsg.GetSelectionMark();
    if (-1 == nIndex)
    {
        return;
    }
    m_pouTxDataStore->GetMsgItem(nIndex, pouMsgItem);
    if (nullptr == pouMsgItem)
    {
        return;
    }
    CString omStr = m_omLctrSigList.GetItemText(nRow, eSigColName);
    SIG_DETAILS ouSignalDetails;
    pouMsgItem->GetSignal(m_pouIBMNetwork, m_bHexMode,std::string(omStr), ouSignalDetails);
    HighlightSignalMatrix(pouMsgItem->MsgDetails.nDLC, ouSignalDetails.Signal.second, ouSignalDetails.Signal.first);
}
void ITxFormView::vOnSignalItemChanged(int nRow, int nColumn)
{
    if (nullptr == m_pouTxDataStore)
    {
        return;
    }

    CString omStrSigName = m_omLctrSigList.GetItemText(nRow, eSigColName);
    ITxMsgItem* pouMsgItem = nullptr;
    int nIndex = m_lstMsg.GetSelectionMark();
    if (-1 == nIndex)
    {
        return;
    }

    m_pouTxDataStore->GetMsgItem(nIndex, pouMsgItem);

    if (nullptr == pouMsgItem)
    {
        return;
    }
    SIG_DETAILS ouSignalDetails;
    pouMsgItem->GetSignal(m_pouIBMNetwork, m_bHexMode,std::string(omStrSigName), ouSignalDetails);

    // If it is a valid signal
    if (ouSignalDetails.Signal.first != nullptr)
    {
        // Get the Signal Raw Value
        CString omStr = m_omLctrSigList.GetItemText(nRow, nColumn);
        unsigned __int64 un64Min, un64Max;
        unsigned __int64 unI64RawValue = 0;
        __int64 nI64TempRawValue;
        double dwPhyVal = 0;
        int nBase = m_bHexMode ? defBASE_HEX : defBASE_DEC;


        switch (nColumn)
        {
            case eSigColPhyVal:
                dwPhyVal = atof(omStr);
                ouSignalDetails.Signal.first->GetRawValueFromEng(dwPhyVal, unI64RawValue);
                break;
            case eSigColRawVal:
                bConvertStringToInt64(omStr, nI64TempRawValue, nBase);
                unI64RawValue = nI64TempRawValue;
                break;
        }

        ouSignalDetails.Signal.first->GetMinMaxValue(un64Min, un64Max);

        eSignalDataType eDataType = eUnsigned;
        ouSignalDetails.Signal.first->GetDataType(eDataType);
        if (eUnsigned == eDataType)
        {
            GetRawValueInRange<unsigned __int64>(unI64RawValue, un64Min, un64Max);
        }
        else if (eSigned == eDataType)
        {
            unsigned int unSiglength = 0;
            ouSignalDetails.Signal.first->GetLength(unSiglength);
            __int64 nI64RawValue = static_cast<__int64>(FormatValue(eSigned,unSiglength,unI64RawValue));
            __int64 n64Min = static_cast<__int64>(un64Min);
            __int64 n64Max = static_cast<__int64>(un64Max);
            GetRawValueInRange<__int64>(nI64RawValue, n64Min, n64Max);
            unI64RawValue = static_cast<unsigned __int64>(nI64RawValue);
        }

        vGetDataBytesFromSignal(unI64RawValue, ouSignalDetails.Signal, pouMsgItem->MsgDetails.nDLC, pouMsgItem->MsgDetails.pchData);

        m_pouTxDataStore->SetMsgItem(nIndex, pouMsgItem);

        std::list<SIG_DETAILS> lstSignalDetails;
        pouMsgItem->GetSignalList(m_pouIBMNetwork, m_bHexMode,lstSignalDetails);
        UpdateDataAndSignalView(pouMsgItem->MsgDetails.pchData, pouMsgItem->MsgDetails.nDLC, lstSignalDetails);
    }
}
unsigned __int64& ITxFormView::FormatValue(eSignalDataType eDataType, int  nSiglength, unsigned __int64& un64RawValue)
{
    if (eSigned == eDataType)
    {
        std::string strBinValue = std::bitset<64>(un64RawValue).to_string();
        int nStringLength = strBinValue.length();
        if (nSiglength > 0 && nSiglength <= nStringLength)
        {
            char chMSBBit = strBinValue.at(nStringLength - (nSiglength));
            if (chMSBBit == '1')
            {
                unsigned __int64 mask = 0xFFFFFFFFFFFFFFFF;
                mask = mask << nSiglength;
                un64RawValue = mask | un64RawValue;
            }
        }
    }
    return un64RawValue;
}
template<typename TIntType>
TIntType& ITxFormView::GetRawValueInRange(TIntType& RawValue,  TIntType MinValue,  TIntType MaxValue)
{
    if (MaxValue < MinValue)
    {
        std::swap(MinValue, MaxValue);
    }
    if (RawValue > MaxValue)
    {
        RawValue = MaxValue;
    }
    else if (RawValue < MinValue)
    {
        RawValue = MinValue;
    }
    return RawValue;
}
void ITxFormView::SetMsgLstRow(std::list<MSG_LST_ROW>& ouMsgLstProp, int nIndex)
{
    int nSize = m_lstMsg.GetItemCount();

    if (nIndex < nSize)
    {
        m_lstMsg.DeleteItem(nIndex);
    }
    int nColumn = 0;
for (auto itr : ouMsgLstProp)
    {
        if (0 == nColumn) //Column indicating the name of the message
        {
            m_lstMsg.InsertItem(nIndex, itr.Text.c_str(), -1);
        }
        else
        {
            m_lstMsg.SetItemText(nIndex, nColumn, itr.Text.c_str());
            m_lstMsg.vSetNumericInfo(nIndex, nColumn, itr.NumInfo);
            m_lstMsg.vSetUserProgInfo(nIndex, nColumn, itr.UserProgInfo);
            m_lstMsg.SetItem(&itr.lvitem);
        }

        m_lstMsg.vSetColumnInfo(nIndex, nColumn, itr.ListInfo);
        nColumn++;
    }

}
void ITxFormView::SetMsgLstCol(std::map<int, SLISTINFO>& mapColAndProp)
{
    int nSize = m_lstMsg.GetItemCount();
    for (int nRow = 0; nRow < nSize; nRow++)
    {
for (auto itr : mapColAndProp)
        {
            m_lstMsg.vSetColumnInfo(nRow, itr.first, itr.second);
        }
    }
}

void ITxFormView::GetDataBytesFromView(unsigned char*& chData)
{
    int nRows = m_omLctrDataBytesList.GetItemCount();
    int nCols = m_omLctrDataBytesList.GetHeaderCtrl()->GetItemCount();
    char chText[MAX_PATH];
    int n = 0;
    for (int i = 0; i < nRows; i++)
    {
        for (int j = 1; j < nCols; j++)
        {
            m_omLctrDataBytesList.GetItemText(i, j, chText, MAX_PATH);
            if (m_bHexMode == true)
            {
                chData[n] = strtoul(chText, nullptr, 16);
            }
            else
            {
                chData[n] = strtoul(chText, nullptr, 10);
            }
            n++;
        }
    }
}
int ITxFormView::nGetValidTimerValue(int nTimerVal)
{
    if (nTimerVal < def_MINIMUM_TIMER_VAL)
    {
        nTimerVal = def_MINIMUM_TIMER_VAL;
    }
    else if (nTimerVal > def_MAXIMUM_TIMER_VAL)
    {
        nTimerVal = def_MAXIMUM_TIMER_VAL;
    }
    return nTimerVal;
}
int ITxFormView::OnDataBytesChanged(int nRow, unsigned char* pchData)
{
    int hResult = S_FALSE;
    if (nullptr == m_pouTxDataStore)
    {
        return hResult;
    }
    ITxMsgItem* pouMsgItem = nullptr;
    if (S_FALSE == m_pouTxDataStore->GetMsgItem(nRow, pouMsgItem))
    {
        return hResult;
    }
    if (nullptr == pouMsgItem)
    {
        return hResult;
    }

    memcpy(pouMsgItem->MsgDetails.pchData, pchData, pouMsgItem->MsgDetails.nDLC);

    m_pouTxDataStore->SetMsgItem(nRow, pouMsgItem);

    if (nullptr == pouMsgItem)
    {
        return hResult;
    }

    hResult = S_OK;

    std::list<SIG_DETAILS> lstSigDetails;
    pouMsgItem->GetSignalList(m_pouIBMNetwork,m_bHexMode ,lstSigDetails);
    UpdateDataAndSignalView(pouMsgItem->MsgDetails.pchData, pouMsgItem->MsgDetails.nDLC, lstSigDetails);



    return hResult;
}
int ITxFormView::OnKeyPressed(char chKey)
{
    int hResult = S_FALSE;
    if (nullptr == m_pouTransmitter)
    {
        return hResult;
    }
    hResult = m_pouTransmitter->OnKeyPressed(chKey);
    return hResult;
}
bool ITxFormView::SetTxData(eTXWNDDETAILS  eParam, LPVOID lpVoid)
{
    bool bRetVal = true;
    switch (eParam)
    {
        case TX_WINDOW_PLACEMENT:
        {
            WINDOWPLACEMENT* psData =
                static_cast<WINDOWPLACEMENT*>(lpVoid);
            m_sTxWndPlacement = *psData;
        }
        break;
        default:
        {
            ASSERT(false);
        }
        break;
    }
    return bRetVal;
}
void ITxFormView::vGetDataBytesFromSignal(unsigned long long u64SignVal, std::pair<ISignal*, SignalInstanse>& Signal, int nDLC, unsigned char* pucData)
{
    unsigned char ucData[MAX_PATH] = {0};
    memcpy(ucData, pucData, nDLC);
    int byteOffsetBy8 = Signal.second.m_nStartBit / 64;
    int signalStartBitInByte = Signal.second.m_nStartBit % 8;
    int byteStart = Signal.second.m_nStartBit / 8 - (byteOffsetBy8 * 8) + 1;
    unsigned int nLength = 0;
    Signal.first->GetLength(nLength);
    bool bIntel = false;
    eEndianess endian = eMotorola;
    GetEndianess(Signal, endian);
    if (eIntel == endian)
    {
        bIntel = true;
    }
    vSetSignalValue(byteStart, signalStartBitInByte, nLength, bIntel, u64SignVal, &ucData[0], nDLC);
    memcpy(pucData, ucData, nDLC);
}
void ITxFormView::vSetSignalValue(int byte, int startBitIndexInByte, int length, bool bIntel, unsigned long long u64SignVal, unsigned char* aucData, int dataLenght)
{
    /* Signal valuedata type happens to be of the same size of the entire CAN
    data byte array. Hence there is an opportunity to take advantage of this
    idiosyncratic characteristics. We will shifts the bit array in u64SignVal
    by the required number of bit positions to exactly map it as a data byte
    array and then interchange positions of bytes as per the endianness and
    finally use it as the etching mask on the target. */
    UINT64* pu64Target = (UINT64*)aucData; // We should be able to work on
    BYTE* pbData = (BYTE*)&u64SignVal; // these variables as an arrayof
    // bytes andvice versa.
    // First findout offset between the last significant bits of the signal
    // and theframe. Finding out the lsb will directly answer to thisquery.
    UINT64 unMaxVal = pow((double)2, (double)length);
    unMaxVal -= 1;
    u64SignVal = u64SignVal&unMaxVal;
    if (bIntel == true)// If Intel format
    {
        int Offset = (byte - 1) * 8 + startBitIndexInByte;
        u64SignVal <<= Offset;// Exactly map the data bits on the databytes.
    }
    else// If Motorola format
    {
        int Offset = byte * 8 - startBitIndexInByte;
        u64SignVal <<= (64 - Offset);
        BYTE byTmp = 0x0;
        byTmp = pbData[7];
        pbData[7] = pbData[0];
        pbData[0] = byTmp;
        byTmp = pbData[6];
        pbData[6] = pbData[1];
        pbData[1] = byTmp;
        byTmp = pbData[5];
        pbData[5] = pbData[2];
        pbData[2] = byTmp;
        byTmp = pbData[4];
        pbData[4] = pbData[3];
        pbData[3] = byTmp;
    }
    UINT64 unTmp = un64GetBitMask(byte, startBitIndexInByte, length, bIntel, aucData, u64SignVal);
    *pu64Target &= ~unTmp;// All bits related to the current signal willbe
    // be made0.
    *pu64Target |= u64SignVal;
}

int ITxFormView::HighlightSignalMatrix(int nDlc, const SignalInstanse& ouSignalInstance, ISignal*& pouSigStrct)
{
    int hResult = S_FALSE;
    if (nullptr == pouSigStrct)
    {
        return hResult;
    }
    hResult = S_OK;
    BYTE abySigMask[8] = { 0 };
    unsigned int unLength;
    int nByte = ouSignalInstance.m_nStartBit / 8 + 1;
    int nSignal = ouSignalInstance.m_nStartBit % 8;
    pouSigStrct->GetLength(unLength);

    eEndianess endian;
    GetEndianess(std::make_pair(pouSigStrct, ouSignalInstance), endian);
    bCalcBitMaskForSig(abySigMask,
                       8,
                       nByte,
                       nSignal,
                       unLength,
                       endian);

    m_odSignalMatrix.vSetMessageLength(nDlc);
    m_odSignalMatrix.vSetHighlight(abySigMask, nDlc);
    return hResult;
}
UINT64 ITxFormView::un64GetBitMask(int byte, int startBitIndexInByte, int length, bool bIntel, unsigned char* /*aucData*/, unsigned long long /*u64SignVal*/)
{
    UINT64 Result = 0x1;

    // First make the required number of bits (m_unSignalLength) up.
    Result <<= length;
    --Result; // These bits are now up.

    // Then shift them to the appropriate place.
    short Shift = (true == bIntel) ?
                  ((short)byte - 1) * 8 + startBitIndexInByte
                  : 64 - ((short)byte * 8 - startBitIndexInByte);
    Result <<= Shift;

    if (false == bIntel)
    {
        BYTE* pbStr = (BYTE*)&Result;

        BYTE bTmp = 0x0;
        bTmp = pbStr[0];
        pbStr[0] = pbStr[7];
        pbStr[7] = bTmp;
        bTmp = pbStr[1];
        pbStr[1] = pbStr[6];
        pbStr[6] = bTmp;
        bTmp = pbStr[2];
        pbStr[2] = pbStr[5];
        pbStr[5] = bTmp;
        bTmp = pbStr[3];
        pbStr[3] = pbStr[4];
        pbStr[4] = bTmp;
    }
    return Result;
}
void ITxFormView::DisableAllColsInAddNewMsgRow()
{
    //This function is used to disable the cols in Add New Msg row which is the
    //last row in the msg list on deleting an item.
    //This is required because on deletion of an item, the [Add Message] row goes
    //one row up and acquires the column properties of the row above it.
    //So, explicitly all the columns in the [Add Message] row is disabled.
    SLISTINFO sInfo;
    sInfo.m_eType = eNoControl;
    int nColCount = m_lstMsg.GetHeaderCtrl()->GetItemCount();
    int nLastRow = m_lstMsg.GetItemCount()-1;
    for (auto nCol = 1; nCol < nColCount; nCol++)
    {
        m_lstMsg.vSetColumnInfo(nLastRow, nCol, sInfo);
    }
}
void ITxFormView::vSignalNameHandler(CListCtrl* /*pList*/, int nItem, int /*nSubItem*/, void* UParam)
{
    ITxFormView* pITxFormView = (ITxFormView*)UParam;
    if (nullptr == pITxFormView)
    {
        return;
    }
    SIGNAL_DETAILS ouSignalDetails;
    SIG_DETAILS ouSigDetails;
    ITxMsgItem* pouMsgItem = nullptr;
    int nSelMsgItem = pITxFormView->m_lstMsg.GetSelectionMark();
    pITxFormView->m_pouTxDataStore->GetMsgItem(nSelMsgItem, pouMsgItem);
    CString omStr = pITxFormView->m_omLctrSigList.GetItemText(nItem, eSigColName);
    if (nullptr == pouMsgItem)
    {
        return;
    }
    pouMsgItem->GetSignal(pITxFormView->m_pouIBMNetwork, pITxFormView->m_bHexMode, std::string(omStr), ouSigDetails);

    //Name
    ouSignalDetails.m_strSignalName = ouSigDetails.SignalInterprete.m_omSigName;

    //Type
    eSignalDataType eDataType;
    ouSigDetails.Signal.first->GetDataType(eDataType);
    switch (eDataType)
    {
        case eSigned:
            ouSignalDetails.m_strType = "int";
            break;
        case eUnsigned:
            ouSignalDetails.m_strType = "unsigned int";
            break;
    }
    //Byte Index
    ouSignalDetails.m_strByteIndex = std::to_string(ouSigDetails.Signal.second.m_nStartBit / 8);

    //Start Bit
    ouSignalDetails.m_strStartBit = std::to_string(ouSigDetails.Signal.second.m_nStartBit % 8);

    //Length
    unsigned int nSignalLength = 0;
    ouSigDetails.Signal.first->GetLength(nSignalLength);
    ouSignalDetails.m_strLength = std::to_string(nSignalLength);

    //Min Value and Max Value
    unsigned __int64 unMin = 0, unMax = 0;
    ouSigDetails.Signal.first->GetMinMaxValue(unMin, unMax);

    ouSignalDetails.m_strMinValue = pITxFormView->GetDisplayValue(unMin, pITxFormView->m_bHexMode, eDataType);

    ouSignalDetails.m_strMaxValue = pITxFormView->GetDisplayValue(unMax, pITxFormView->m_bHexMode, eDataType);

    //Unit
    ouSignalDetails.m_strUnit = ouSigDetails.SignalInterprete.m_omUnit;

    //Endian
    eEndianess endian;
    pITxFormView->GetEndianess(ouSigDetails.Signal, endian);
    if (eIntel == endian)
    {
        ouSignalDetails.m_strByteOrder = "Intel";
    }
    else if (eMotorola == endian)
    {
        ouSignalDetails.m_strByteOrder = "Motorola";
    }
    CSignalDetailsDlg ouSignalDetailsDlg(ouSignalDetails,nullptr);
    ouSignalDetailsDlg.DoModal();
}

std::string ITxFormView::GetDisplayValue(unsigned __int64 nValue, bool bIsHex, eSignalDataType eDatatype)
{
    CString omStrValue = "";
    std::string strValue = "";
    if (true == bIsHex)
    {
        omStrValue.Format("%X", nValue);
        strValue = std::string(omStrValue);
    }
    else
    {
        if (eSigned == eDatatype)
        {
            strValue = std::to_string(static_cast<__int64>(nValue));
        }
        else if (eUnsigned == eDatatype)
        {
            strValue = std::to_string(nValue);
        }
    }
    return strValue;
}
bool ITxFormView::GetTxData(eTXWNDDETAILS  eParam, LPVOID* lpData)
{
    bool bReturn = false;
    switch (eParam)
    {
        case TX_WINDOW_PLACEMENT:
        {
            bReturn = true;
            WINDOWPLACEMENT* psData =
                static_cast<WINDOWPLACEMENT*>(*lpData);
            *psData = m_sTxWndPlacement;
        }
        break;
        default:
        {
            ASSERT(false);
        }
        break;
    }
    return bReturn;
}
HRESULT ITxFormView::UpdateTxView()
{
    int hResult = LoadAllMsgItems();
    if (S_OK == hResult)
    {
        EnsureLastItemVisible();
        //Based on the connection status the form has to be updated.
        hResult = OnBusStatusChanged(m_eBusStatus);
    }
    CStatic* pomStatic = (CStatic*)GetDlgItem(IDC_STATIC_DATA_BYTE_VIEW);
    if (nullptr != pomStatic)
    {
        if (true == m_bHexMode)
        {
            pomStatic->SetWindowText(def_DATA_BYTE_HEX);
        }
        else
        {
            pomStatic->SetWindowText(def_DATA_BYTE_DEC);
        }
    }

    return hResult;
}
int ITxFormView::EnsureLastItemVisible()
{
    // Ensure that the last item is visible.
    // i.e Scroll Bar is moved to the last item in the Msg List.
    int nCount = m_lstMsg.GetItemCount();
    if (nCount > 0)
    {
        m_lstMsg.EnsureVisible(nCount - 1, FALSE);
    }
    return S_OK;

}
int ITxFormView::SetBusStatus(ESTATUS_BUS eBusStatus)
{
    //The Frame Window might not be alive at the time of Busstatus change,
    //Therefore we need this function to set the status as and when the Busstatus is changed.
    m_eBusStatus = eBusStatus;
    return S_OK;
}
int ITxFormView::SetHexMode(bool bIsHex)
{
    m_bHexMode = bIsHex;
    return S_OK;
}
int ITxFormView::SetConfigData(xmlDocPtr pDoc)
{
    int hResult = S_FALSE;
    if (nullptr != m_pouTxDataStore)
    {
        m_pouTxDataStore->DeleteAllMsgItems();
    }
    if (pDoc == nullptr)
    {
        SetDefaultWindowPos();
        return hResult;
    }
    std::string strConfigTag = "";
    GetConfigTag(strConfigTag);
    strConfigTag += "/" + std::string(DEF_WND_POS);
    xmlChar* pXpath = (xmlChar*)(strConfigTag.c_str());

    xmlXPathObjectPtr pObjectPath = xmlUtils::pGetNodes(pDoc, pXpath);
    if (pObjectPath != nullptr)
    {
        xmlNodeSetPtr pNodeSet = pObjectPath->nodesetval;
        if (nullptr != pNodeSet)
        {
            xmlNodePtr pNode = pNodeSet->nodeTab[0];       //Take First One only
            WINDOWPLACEMENT WndPlacement;
            xmlUtils::ParseWindowsPlacement(pNode, WndPlacement);
            SetTxData(TX_WINDOW_PLACEMENT, &WndPlacement);
        }
    }

    strConfigTag = "";
    GetConfigTag(strConfigTag);
    strConfigTag += "/" + std::string(DEF_MSG_LIST);
    xmlChar* pXpathChannel = (xmlChar*)strConfigTag.c_str();
    pObjectPath = xmlUtils::pGetNodes(pDoc, pXpathChannel);
    if (pObjectPath != nullptr)
    {
        xmlNodeSetPtr pNodeSet = pObjectPath->nodesetval;

        if (nullptr != pNodeSet)
        {
            xmlChar* pchPathMsg = (xmlChar*)DEF_MESSAGE;
            xmlXPathObjectPtr pObjectPath = xmlUtils::pGetChildNodes(pNodeSet->nodeTab[0], pchPathMsg);
            if (nullptr != pObjectPath)
            {
                hResult = S_OK;
                xmlNodeSetPtr pNodeMsg = pObjectPath->nodesetval;
                if (nullptr != pNodeMsg)
                {
                    if (S_OK == AddNewMsgItemsToDataStore(pNodeMsg->nodeNr) && nullptr != m_pouTxDataStore)
                    {
                        m_pouTxDataStore->SetMsgItemsConfigData(pNodeSet->nodeTab[0]);
                    }
                }
            }
        }
    }
    //Support for older .cfx for CAN where Message Blocks were used and LIN used Channel_Msg_List Tag.
    else
    {
        hResult = SetOldFormatConfigData(pDoc);
    }
    return hResult;
}
int ITxFormView::GetConfigData(xmlNodePtr pxmlNodePtr)
{
    int hResult = S_FALSE;
    //windows position
    xmlNodePtr pNodeWndPos = xmlNewNode(nullptr, BAD_CAST DEF_WND_POS);
    if (nullptr != pNodeWndPos)
    {
        hResult = S_OK;
        xmlUtils::CreateXMLNodeFrmWindowsPlacement(pNodeWndPos, m_sTxWndPlacement);
        xmlAddChild(pxmlNodePtr, pNodeWndPos);
    }

    //Channel Messages
    xmlNodePtr pxmlNodeMsgItems = xmlNewNode(nullptr, BAD_CAST DEF_MSG_LIST);
    if (nullptr != m_pouTxDataStore && nullptr != pxmlNodeMsgItems);
    {
        hResult = S_OK;
        m_pouTxDataStore->GetMsgItemsConfigData(pxmlNodeMsgItems);
        xmlAddChild(pxmlNodePtr, pxmlNodeMsgItems);
    }

    return hResult;
}

int ITxFormView::SetIBMNetwork(IBMNetWorkGetService* pouIBMNetwork)
{
    m_pouIBMNetwork = pouIBMNetwork;
    return S_OK;
}

