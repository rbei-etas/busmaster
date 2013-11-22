///**********************************************************************************
//Project       :   Frame
//FileName      :   TxMsgList.cpp
//Description   :   Implementation of CTxMsgList class, custom list control
//Directory     :
//Version       :   V1.0
//Authors       :   Ravi D
//Date Created  :   19/09/2006
//Date Modified :
//Modifications :
//**********************************************************************************/
#pragma once
#include "TxWindowFlexRay_stdafx.h"
#include "DataTypes/Base_FlexRay_Buffer.h"
#include "hashdefines.h"
#include "CHexEdit/Hexedit.h"
//#include "FRAME_BaseUI.h"
#include "TxMsgList.h"
#include "TxListComboBox.h"
//#include "DisplayMatrix/DisplayMatrixDlg.h"
//#include "Mainfrm.h"


// ID for Combobox
#define IDC_CONTROL 0x12345
BOOL g_Clicked = FALSE;

BEGIN_MESSAGE_MAP(CTxMsgList, CFlickerFreeListCtrl)
    ON_NOTIFY(HDN_BEGINTRACKA, 0, OnHdnBegintrack)
    ON_NOTIFY(HDN_BEGINTRACKW, 0, OnHdnBegintrack)
    ON_WM_LBUTTONDBLCLK()
    ON_WM_LBUTTONDOWN()
    //ON_NOTIFY_REFLECT(NM_CLICK, OnNMClick)
    ON_NOTIFY_REFLECT(NM_DBLCLK, OnNMDblclk)
    ON_NOTIFY_REFLECT(LVN_ENDLABELEDIT, OnLvnEndlabeledit)
    ON_WM_MEASUREITEM_REFLECT()
END_MESSAGE_MAP()
///**********************************************************************************
//Function Name :   CTxMsgList
//Input(s)      :   -
//Output        :   -
//Functionality :   constructor
//Member of     :   CTxMsgList
//Friend of     :   -
//Authors       :   Ravi D
//Date Created  :   19/09/2006
//Modifications :   -
//************************************************************************************/
CTxMsgList::CTxMsgList(void)
    : m_pWndEdit(NULL)
    , m_bAllowEdit(FALSE)
    , m_nSortCol(-1)
    , m_bSortAscending(TRUE)
{
    m_pWndEdit = NULL;

    m_ptEditting.x = -1;
    m_ptEditting.y = -1;
    m_dwPrevEditFmt = ES_LEFT;

#if 0
    CFRAME_BaseUIDoc* pDoc = NULL;
    CMDIChildWnd* pMDIActive = ((CMainFrame*)AfxGetMainWnd())->MDIGetActive();
    if (pMDIActive == NULL)
    {
        pDoc = (CFRAME_BaseUIDoc*)theApp.m_pTransmitViewTemplate->CreateNewDocument();
    }
    else
    {
        pDoc = (CFRAME_BaseUIDoc*)pMDIActive->GetActiveDocument();
    }

    if (pDoc == NULL)
    {
        pDoc = ((CMainFrame*)AfxGetMainWnd())->m_pDoc;
    }

    _bstr_t bstrProjCfgName = _T("");
    IProjectCfgHandlerPtr pProjCfgHandler = NULL;
    pProjCfgHandler = pDoc->m_oProjCfgHandler->GetProjCfgHandler();
    ASSERT(pProjCfgHandler != NULL);

    pProjCfgHandler->MoveFirst();
    pProjCfgHandler->get_ProjectCfgByName(&bstrProjCfgName.GetBSTR());

    IProjectCfgPtr pProjCfg = NULL;
    pProjCfgHandler->get_ProjectCfgName(bstrProjCfgName, &pProjCfg);
    ASSERT(pProjCfg != NULL);

    pProjCfg->get_ProjCfg_MsgDisp(&m_pProjCfgMsgDisp);
    ASSERT(m_pProjCfgMsgDisp != NULL);
#endif
}

///**********************************************************************************
//Function Name :   ~CTxMsgList
//Input(s)      :   -
//Output        :   -
//Functionality :   Destructor
//Member of     :   CTxMsgList
//Friend of     :   -
//Authors       :   Ravi D
//Date Created  :   19/09/2006
//Modifications :   -
//************************************************************************************/
CTxMsgList::~CTxMsgList(void)
{
    if(m_pWndEdit != NULL)
    {
        delete m_pWndEdit;
        m_pWndEdit =  NULL;
    }
    m_fntText.DeleteObject();
}

///**********************************************************************************
//Function Name :   DrawItem
//Input(s)      :   lpDrawItemStruct - information about the type of drawing required
//Output        :   -
//Functionality :   to implement drawing for an owner-draw CListCtrl object
//Member of     :   CTxMsgList
//Friend of     :   -
//Authors       :   Ravi D
//Date Created  :   19/09/2006
//Modifications :   -
//************************************************************************************/

void CTxMsgList::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
    ULONG ulColor = 0;
    CDC* pDC = CDC::FromHandle(lpDrawItemStruct->hDC);
    CRect rect(lpDrawItemStruct->rcItem);


    if (lpDrawItemStruct->itemAction & ODA_DRAWENTIRE)
    {
        CRect rctSubItem;

        CHeaderCtrl* pHeader = CFlickerFreeListCtrl::GetHeaderCtrl();
        int nColumnCount = pHeader->GetItemCount();

        char szTabText[256];
        memset(szTabText,'\0', sizeof(szTabText));

        UINT textFormat = 0;
        textFormat = DT_VCENTER | DT_SINGLELINE | DT_END_ELLIPSIS;

        //Get the item Text
        ListView_GetItemText(lpDrawItemStruct->hwndItem, lpDrawItemStruct->itemID, 0, szTabText, 256);
        // Setting Message Background Color
#if 0
        if ( stricmp(szTabText, "Static") == 0 )
        {
            m_pProjCfgMsgDisp->get_StaticFrameColor(&ulColor);
        }
        else if(stricmp(szTabText, "Dynamic") == 0)
        {
            m_pProjCfgMsgDisp->get_DynamicFrameColor(&ulColor);
        }
        else if(stricmp(szTabText, "Null") == 0)
        {
            m_pProjCfgMsgDisp->get_NullFrameColor(&ulColor);
        }
        else if(stricmp(szTabText, "Sync") == 0)
        {
            m_pProjCfgMsgDisp->get_SyncFrameColor(&ulColor);
        }
#endif

        pDC->SetTextColor(ulColor);

        LVITEM lviItemData;
        for (int nColumnIndex = 0; nColumnIndex < nColumnCount; nColumnIndex++)
        {
            ListView_GetItemText(lpDrawItemStruct->hwndItem, lpDrawItemStruct->itemID, nColumnIndex, szTabText, 256);
            ListView_GetSubItemRect(lpDrawItemStruct->hwndItem, lpDrawItemStruct->itemID, nColumnIndex, LVIR_LABEL, &rctSubItem);
            rctSubItem.DeflateRect(2, 0);

            memset(&lviItemData, 0, sizeof(lviItemData));
            lviItemData.mask =  LVIF_PARAM;
            lviItemData.iItem = lpDrawItemStruct->itemID;
            ListView_GetItem(lpDrawItemStruct->hwndItem, &lviItemData);
            DWORD dwChangeable = (DWORD)lviItemData.lParam;

            // Setting the Disable Color
            switch(nColumnIndex)
            {
                case TX_COLUMN_TYPE: // Message Type
                {
                    if (dwChangeable & RBIN_FLXTXMSGINFO_CHANGEABLE_FRAMETYPE)
                    {
                    }
                    else
                    {
                        pDC->FillSolidRect( &rctSubItem, ::GetSysColor(COLOR_BTNFACE)); // Fill in the background.
                    }


                    pDC->DrawText(szTabText, &rctSubItem, textFormat);
                    break;
                }
                case TX_COLUMN_MESSAGE_ID: // Message ID
                {
                    if (dwChangeable & RBIN_FLXTXMSGINFO_CHANGEABLE_IDENTIFIER)
                    {
                    }
                    else
                    {
                        pDC->FillSolidRect( &rctSubItem, ::GetSysColor(COLOR_BTNFACE)); // Fill in the background.
                    }
                    pDC->DrawText(szTabText, &rctSubItem, textFormat);
                    break;
                }
                case TX_COLUMN_DESCRIPTION: // Description
                {
                    pDC->DrawText(szTabText, &rctSubItem, textFormat);
                    break;
                }
                case  TX_COLUMN_CYCLE_NO: // Cycle No
                {
                    char szFrameType[32];
                    ListView_GetItemText(lpDrawItemStruct->hwndItem, lpDrawItemStruct->itemID, TX_COLUMN_TYPE, szFrameType, 32);
                    if ((dwChangeable & RBIN_FLXTXMSGINFO_CHANGEABLE_CYCLENUMBER) == FALSE)
                    {
                        pDC->FillSolidRect( &rctSubItem, ::GetSysColor(COLOR_BTNFACE)); // Fill in the background.
                    }
                    pDC->DrawText(szTabText, &rctSubItem, textFormat);

                    break;
                }
                case TX_COLUMN_CHANNEL: // Channel
                {
                    if (dwChangeable & RBIN_FLXTXMSGINFO_CHANGEABLE_CHANNELID)
                    {
                    }
                    else
                    {
                        pDC->FillSolidRect( &rctSubItem, ::GetSysColor(COLOR_BTNFACE)); // Fill in the background.
                    }
                    pDC->DrawText(szTabText, &rctSubItem, textFormat);
                    break;
                }
                case TX_COLUMN_DATA_LENGTH: // Data length
                {
                    if (dwChangeable & RBIN_FLXTXMSGINFO_CHANGEABLE_DATALENGTH)
                    {
                    }
                    else
                    {
                        pDC->FillSolidRect( &rctSubItem, ::GetSysColor(COLOR_BTNFACE)); // Fill in the background.
                    }
                    pDC->DrawText(szTabText, &rctSubItem, textFormat);
                    break;
                }
                case TX_COLUMN_DATA_BYTES: // Data Bytes
                {
                    if (dwChangeable & RBIN_FLXTXMSGINFO_CHANGEABLE_DATA)
                    {
                    }
                    else
                    {
                        pDC->FillSolidRect( &rctSubItem, ::GetSysColor(COLOR_BTNFACE)); // Fill in the background.
                    }
                    pDC->DrawText(szTabText, &rctSubItem, textFormat);
                    break;
                }

            }//switch

        }//for
    }


    if ((lpDrawItemStruct->itemState & ODS_SELECTED) &&
            (lpDrawItemStruct->itemAction & (ODA_SELECT | ODA_DRAWENTIRE)))
    {
        //m_ptSubItemPoint;
        COLORREF clrOldText;

        //Make the color of text of the selected tab to be BLUE.
        clrOldText = pDC->SetTextColor(RGB(255, 255, 255));
        pDC->FillSolidRect(&rect, RGB(0, 0, 255));

        CRect rctSubItem;
        CHeaderCtrl* pHeader = CFlickerFreeListCtrl::GetHeaderCtrl();
        int nColumnCount = pHeader->GetItemCount();

        char szTabText[256];
        memset(szTabText,'\0',sizeof(szTabText));

        UINT textFormat = 0;
        textFormat = DT_VCENTER | DT_SINGLELINE | DT_END_ELLIPSIS;

        LVITEM lviItemData;
        for (int i = 0; i < nColumnCount; i++)
        {
            ListView_GetItemText(lpDrawItemStruct->hwndItem, lpDrawItemStruct->itemID, i, szTabText, 256);
            ListView_GetSubItemRect(lpDrawItemStruct->hwndItem, lpDrawItemStruct->itemID, i, LVIR_LABEL, &rctSubItem);
            rctSubItem.DeflateRect(2, 0);
            pDC->DrawText(szTabText, &rctSubItem, textFormat);

            memset(&lviItemData, 0, sizeof(lviItemData));
            lviItemData.mask = LVIF_PARAM;
            ListView_GetItem(lpDrawItemStruct->hwndItem, &lviItemData);
            DWORD dwChangeable = (DWORD)lviItemData.lParam;

            if (i == 5)
            {
                //rctSubItem.left = rctSubItem.right - GetSystemMetrics(SM_CYVSCROLL);
                //pDC->DrawFrameControl(rctSubItem, DFC_SCROLL, DFCS_SCROLLDOWN);
                if (dwChangeable & RBIN_FLXTXMSGINFO_CHANGEABLE_CHANNELID)
                {
                    rctSubItem.left = rctSubItem.right - GetSystemMetrics(SM_CYVSCROLL);
                    pDC->DrawFrameControl(rctSubItem, DFC_SCROLL, DFCS_SCROLLDOWN);
                }
                else
                {
                    int i = 0;
                }
            }
            else if (i == 8)  //Data Matrix
            {
                textFormat = DT_CENTER | DT_SINGLELINE | DT_END_ELLIPSIS;
                if (g_Clicked == TRUE)
                {
                    pDC->DrawFrameControl(rctSubItem, DFC_BUTTON, DFCS_BUTTONPUSH | DFCS_PUSHED);
                }
                else
                {
                    pDC->DrawFrameControl(rctSubItem, DFC_BUTTON, DFCS_BUTTONPUSH);
                }
                pDC->DrawText("...", &rctSubItem, textFormat);
            }

        }

        pDC->SetTextColor(clrOldText);
    }



}

///**********************************************************************************
//Function Name :   OnHdnBegintrack
//Input(s)      :   pNMHDR - information about the type of drawing required
//                  pResult -
//Output        :   -
//Functionality :   to disable resizing of channel column
//Member of     :   CTxMsgList
//Friend of     :   -
//Authors       :   Ravi D
//Date Created  :   19/09/2006
//Modifications :   -
//************************************************************************************/
void CTxMsgList::OnHdnBegintrack(NMHDR* pNMHDR, LRESULT* pResult)
{
    LPNMHEADER phdr = reinterpret_cast<LPNMHEADER>(pNMHDR);
    // TODO: Add your control notification handler code here

    *pResult = 0;

}

///**********************************************************************************
//Function Name :   StartEdit
//Input(s)      :   nItem - item
//                  nSubItem - subitem
//Output        :   -
//Functionality :   To edit the list item
//Member of     :   CTxMsgList
//Friend of     :   -
//Authors       :   Ravi D
//Date Created  :   19/09/2006
//Modifications :   -
//************************************************************************************/
BOOL CTxMsgList::StartEdit(int nItem, int nSubItem)
{
    // Get the grid width and height
    if (!m_bAllowEdit || !_IsValidIndex(nItem) || nSubItem < 0 || nSubItem >= GetColumnCount())
    {
        return FALSE;
    }
    //Check for Editable column && Data length column should not be editable
    //if(bIsChangeable(nItem,nSubItem) == false || nSubItem == SIXTH_COL)
    if (bIsChangeable(nItem,nSubItem) == false)
    {
        return FALSE;
    }

    if (m_ptEditting.x == nItem && m_ptEditting.y == nSubItem)
    {
        return TRUE;
    }

    //For alphanumeric characters column types
    if(nSubItem == TX_COLUMN_MESSAGE_ID || nSubItem == TX_COLUMN_CYCLE_NO
            || nSubItem == TX_COLUMN_DATA_LENGTH || nSubItem == TX_COLUMN_DATA_BYTES)
    {
        if(m_pWndEdit != NULL)
        {
            delete m_pWndEdit;
            m_pWndEdit =  NULL;
        }
        m_pWndEdit = new CHexEdit;
    }
    else if( nSubItem == TX_COLUMN_DESCRIPTION) //For Hexa-Decimal column types
    {
        if(m_pWndEdit != NULL)
        {
            delete m_pWndEdit;
            m_pWndEdit =  NULL;
        }
        m_pWndEdit = new CEdit;
    }

    EndEdit(TRUE);
    m_ptEditting.x = nItem;
    m_ptEditting.y = nSubItem;

    // determine editbox font and alignment
    const DWORD FMT = _GetHeaderTextFormat(nSubItem);
    if (FMT != m_dwPrevEditFmt)
    {
        m_dwPrevEditFmt = FMT;

        // Funny thing:
        // Changing CEdit style among ES_LEFT, ES_CENTER, ES_RIGHT at runtime works
        // sometimes and fails other times. It just cannot guarantee to be succeed.
        // So I decided to destroy and recreate the CEdit every time when the text
        // format changes.
        if (m_pWndEdit->GetSafeHwnd() != NULL)
        {
            m_pWndEdit->DestroyWindow();
        }

        if (!m_pWndEdit->Create(ES_AUTOHSCROLL | ES_NOHIDESEL | WS_CHILD | WS_BORDER | FMT, CRect(0, 0, 1, 1), this, 0))
        {
            return FALSE;
        }
    }
    else
    {
        if (m_pWndEdit->GetSafeHwnd() == NULL
                && !m_pWndEdit->Create(ES_AUTOHSCROLL | ES_NOHIDESEL | WS_CHILD | WS_BORDER | FMT, CRect(0, 0, 1, 1), this, 0))
        {
            return FALSE;
        }
    }

    m_pWndEdit->SetFont(GetFont());

    if(nSubItem == TX_COLUMN_MESSAGE_ID) // Limit the Message ID For Hex Format
    {
        m_pWndEdit->SetLimitText(2);    ///  Maximum is FF(2 Bytes) in according to INI file ATBFRID Register.
    }


    CRect rcEdit;
    ListView_GetSubItemRect(GetSafeHwnd(), m_ptEditting.x, m_ptEditting.y, LVIR_LABEL, &rcEdit);
    if (m_ptEditting.y > 0 && GetImageList() != NULL && GetItemImage(m_ptEditting.x, m_ptEditting.y) >= 0)
    {
        rcEdit.DeflateRect(16, 0, 0, 0);
    }

    // Move the editbox to that grid, obtain text from the grid, display the
    // editbox, and, finally, highlights all text in the editbox and set the
    // windows focus to the editbox.
    m_pWndEdit->MoveWindow(&rcEdit);
    m_pWndEdit->SetWindowText(GetItemText(m_ptEditting.x, m_ptEditting.y));
    m_pWndEdit->ShowWindow(SW_SHOW);
    m_pWndEdit->SetSel(0, -1);
    m_pWndEdit->SetFocus();
    return TRUE;
}
///**********************************************************************************
//Function Name :   bValidateDynamicLength
//Input(s)      :   lValue - Message Length Value
//
//Output        :   true - if valid data length
//                  false - if data length is not valid
//Functionality :   Validate the Dynamic length
//Member of     :   CTxMsgList
//Friend of     :   -
//Authors       :   Ramkrishnan S.
//Date Created  :   11/02/2007
//Modifications :   -
//************************************************************************************/
bool CTxMsgList::bValidateDynamicLength(long lValue)
{
    return true;
#if 0
    bool bResult = false;
    UCI_FLXINFO sUCI_FLXINFO;

    if( ((CMainFrame*)AfxGetMainWnd())->m_pDoc->m_opFlexRay->GetHarwareConfigStatus(&sUCI_FLXINFO) != S_FALSE )
    {
        if(sUCI_FLXINFO.wMaxDynamicDataLength >=  lValue)
        {
            bResult = TRUE;
        }
    }
    return bResult;
#endif
}


///**********************************************************************************
//Function Name :   EndEdit
//Input(s)      :   bCommit -   ture  - save the data from the edit control to the list item
//                              false - the data is discarded
//Output        :   true - data is changed
//                  false - data is not changed
//Functionality :   To stop editing of the list item
//Member of     :   CTxMsgList
//Friend of     :   -
//Authors       :   Ravi D
//Date Created  :   19/09/2006
//Modifications :   -
//************************************************************************************/
BOOL CTxMsgList::EndEdit(BOOL bCommit)
{
    if (!_IsEditVisible())
    {
        return FALSE;
    }

    CString omStrData;
    m_pWndEdit->GetWindowText(omStrData);
    BOOL bChanged = bCommit && omStrData.Compare(GetItemText(m_ptEditting.x, m_ptEditting.y)) != 0;
    if (bChanged)
    {
        if(m_ptEditting.y == SIXTH_COL)//Check only for Data Bytes column
        {
            CString sDataBytes = _T("");
            CString sDataLen   = _T("");
            int nDataLen = 0;
            sDataBytes = CFlickerFreeListCtrl::GetItemText(m_ptEditting.x,SEVENTH_COL);
            sscanf(omStrData,"%d",&nDataLen); // Decimal
            if(!bValidateDynamicLength(nDataLen))
            {
                MessageBox(DYNAMIC_MSG_DATA_LENGTH,APPLICATION_NAME,MB_OK|MB_ICONWARNING);
            }
            else
            {
                vDataFormat(sDataBytes,&nDataLen,TX_DATA_LENGTH); //Str & Datalen are return pointers
                sDataLen.Format("%d",nDataLen);
                CFlickerFreeListCtrl::SetItemText(m_ptEditting.x,SIXTH_COL,sDataLen);
                CFlickerFreeListCtrl::SetItemText(m_ptEditting.x,SEVENTH_COL,sDataBytes);
            }
        }
        else if(m_ptEditting.y == SEVENTH_COL)//Check only for Data Bytes column
        {
            int nChangedDataLen = 0;
            int nOriginalDataLen = 0;
            CString sOriginalData = CFlickerFreeListCtrl::GetItemText(m_ptEditting.x,SEVENTH_COL);

            CString sDataLen = CFlickerFreeListCtrl::GetItemText(m_ptEditting.x,SIXTH_COL);
            sscanf(sDataLen,"%d",&nOriginalDataLen); // Decimal
            vDataFormat(omStrData,&nChangedDataLen,TX_DATA_BYTE); //Str & Datalen are return pointers

            //Validation for Data Length for static message
            CString omMsgType = CFlickerFreeListCtrl::GetItemText(m_ptEditting.x,FIRST_COL);
            if(omMsgType.CompareNoCase("static")== 0 && nChangedDataLen != nOriginalDataLen)
            {
                MessageBox(STATIC_MSG_DATA_LENGTH,APPLICATION_NAME,MB_OK|MB_ICONWARNING);
            }
            else if(omMsgType.CompareNoCase("dynamic")==0 && bValidateDynamicLength(nChangedDataLen) == FALSE)
            {
                MessageBox(DYNAMIC_MSG_DATA_LENGTH,APPLICATION_NAME,MB_OK|MB_ICONWARNING);
            }
            else
            {
                sDataLen.Format("%d",nChangedDataLen);
                CFlickerFreeListCtrl::SetItemText(m_ptEditting.x,SIXTH_COL,sDataLen);
                CFlickerFreeListCtrl::SetItemText(m_ptEditting.x,SEVENTH_COL,omStrData);
            }
        }
        else if(m_ptEditting.y == SECOND_COL) /// formatting the message id . message id maximum is FF.
        {
            for(int nIndex = omStrData.GetLength(); nIndex < 2; nIndex++) // Id Maximum length is 2 Bytes in according to INI file ATBFRID Register.
            {
                omStrData.Insert(0,"0");
            }
            if (omStrData != "00")//if Message Id is not equal to empty,please update the message id
            {
                CFlickerFreeListCtrl::SetItemText(m_ptEditting.x, m_ptEditting.y, omStrData);
            }
        }
        else
        {
            CFlickerFreeListCtrl::SetItemText(m_ptEditting.x, m_ptEditting.y, omStrData);
        }

        _UnsetSortedColumn();
        GetParent()->SendMessage(WM_EDIT_COMMITTED, (WPARAM)m_ptEditting.x, (LPARAM)m_ptEditting.y);

        //Sandip Added for updating the setModified flag
        //((CMainFrame *)AfxGetMainWnd())->m_pDoc->SetProjectCfgUpadte(TRUE);

    }
    m_pWndEdit->ShowWindow(SW_HIDE);
    m_ptEditting.x = -1;
    m_ptEditting.y = -1;
    return bChanged;
}

///**********************************************************************************
//Function Name :   GetEditControl
//Input(s)      :   -
//Output        :   Pointer to the edit control
//Functionality :   returns pointer to the edit control
//Member of     :   CTxMsgList
//Friend of     :   -
//Authors       :   Ravi D
//Date Created  :   19/09/2006
//Modifications :   -
//************************************************************************************/

CEdit* CTxMsgList::GetEditControl(void)
{
    return m_pWndEdit;
}

///**********************************************************************************
//Function Name :   _GetHeaderTextFormat
//Input(s)      :   nColumn - column number
//Output        :   text format of the column
//Functionality :   To Retrieve the text format of the specified column
//Member of     :   CTxMsgList
//Friend of     :   -
//Authors       :   Ravi D
//Date Created  :   19/09/2006
//Modifications :   -
//************************************************************************************/
DWORD CTxMsgList::_GetHeaderTextFormat(int nColumn) const
{
    if (!HasColumnHeader())
    {
        return ES_LEFT;
    }

    HDITEM hd;
    hd.mask = HDI_FORMAT;
    if (!GetHeaderCtrl()->GetItem(nColumn, &hd))
    {
        return ES_LEFT;
    }

    if (hd.fmt & HDF_CENTER)
    {
        return ES_CENTER;
    }
    else if (hd.fmt & HDF_RIGHT)
    {
        return ES_RIGHT;
    }
    else
    {
        return ES_LEFT;
    }
}

///**********************************************************************************
//Function Name :   _IsEditVisible
//Input(s)      :   -
//Output            :   true - item is visible
//                  flase  - item is not visible
//Functionality :   To check whether the item is visible or not
//Member of     :   CTxMsgList
//Friend of     :   -
//Authors           :   Ravi D
//Date Created  :   19/09/2006
//Modifications :   -
//************************************************************************************/
BOOL CTxMsgList::_IsEditVisible(void) const
{
    return m_bAllowEdit
           && m_ptEditting.x >= 0 && m_ptEditting.x < CFlickerFreeListCtrl::GetItemCount()
           && m_ptEditting.y >= 0 && m_ptEditting.y < GetColumnCount();
}

///**********************************************************************************
//Function Name :   _IsValidIndex
//Input(s)      :   nIndex - index to be checked
//Output        :   true - index is valid
//                  flase  - index is not valid
//Functionality :   To check whether the specified index is valid or not
//Member of     :   CTxMsgList
//Friend of     :   -
//Authors       :   Ravi D
//Date Created  :   19/09/2006
//Modifications :   -
//************************************************************************************/
BOOL CTxMsgList::_IsValidIndex(int nIndex) const
{
    return nIndex >= 0 && nIndex < CFlickerFreeListCtrl::GetItemCount();
}

///**********************************************************************************
//Function Name :   _UnsetSortedColumn
//Input(s)      :   -
//Output        :   -
//Functionality :   reset the sort column data
//Member of     :   CTxMsgList
//Friend of     :   -
//Authors       :   Ravi D
//Date Created  :   19/09/2006
//Modifications :   -
//************************************************************************************/
void CTxMsgList::_UnsetSortedColumn(void)
{
    m_nSortCol = -1;
    m_bSortAscending = TRUE;
}

// FALSE if the list control has LVS_NOCOLUMNHEADER flag
///**********************************************************************************
//Function Name :   HasColumnHeader
//Input(s)      :   -
//Output        :   true - list has a column header
//                  false -  list does not have a colmn header
//Functionality :   check whether the list has a column header or not
//Member of     :   CTxMsgList
//Friend of     :   -
//Authors       :   Ravi D
//Date Created  :   19/09/2006
//Modifications :   -
//************************************************************************************/
BOOL CTxMsgList::HasColumnHeader(void) const
{
    return (GetStyle() & LVS_NOCOLUMNHEADER) == 0;
}

///**********************************************************************************
//Function Name :   GetItemImage
//Input(s)      :   nItem - row
//                  nsubitem - column
//Output        :   index of the image in the list control
//Functionality :   to Retrieve image of a specified item in the list
//Member of     :   CTxMsgList
//Friend of     :   -
//Authors       :   Ravi D
//Date Created  :   19/09/2006
//Modifications :   -
//************************************************************************************/
int CTxMsgList::GetItemImage(int nItem, int nSubItem) const
{
    LVITEM lvi;
    lvi.iItem = nItem;
    lvi.iSubItem = nSubItem;
    lvi.mask = LVIF_IMAGE;
    return CFlickerFreeListCtrl::GetItem(&lvi) ? lvi.iImage : -1;
}

///**********************************************************************************
//Function Name :   GetColumnCount
//Input(s)      :
//Output        :   number of columns in the list control
//Functionality :   to get the number of column in the list control
//Member of     :   CTxMsgList
//Friend of     :   -
//Authors       :   Ravi D
//Date Created  :   19/09/2006
//Modifications :   -
//************************************************************************************/
int CTxMsgList::GetColumnCount(void) const
{
    return GetHeaderCtrl()->GetItemCount();
}

///**********************************************************************************
//Function Name :   GetHeaderCtrl
//Input(s)      :
//Output        :   pointer to the header control of the list
//Functionality :   to Retrieve the header control of the list
//Member of     :   CTxMsgList
//Friend of     :   -
//Authors       :   Ravi D
//Date Created  :   19/09/2006
//Modifications :   -
//************************************************************************************/
const CHeaderCtrl* CTxMsgList::GetHeaderCtrl() const
{
    // Yes, CListCtrl already provides "GetHeaderCtrl", but not const.
    // I desperately need a "const" version of "GetHeaderCtrl" because that's the
    // only way to make "GetColumnCount" const, which, in turn, is being called by
    // A LOT of const member functions. So if "GetHeaderCtrl" is not const, there will
    // be almost no const member function at all in this class. Terrible.
    return (const CHeaderCtrl*)(CWnd::FromHandle(ListView_GetHeader(GetSafeHwnd())));
}

///**********************************************************************************
//Function Name :   GetImageList
//Input(s)      :
//Output        :   pointer to the image list of the list control
//Functionality :   to Retrieve the image list of the list control
//Member of     :   CTxMsgList
//Friend of     :   -
//Authors       :   Ravi D
//Date Created  :   19/09/2006
//Modifications :   -
//************************************************************************************/
CImageList* CTxMsgList::GetImageList() const
{
    return CFlickerFreeListCtrl::GetImageList(LVSIL_SMALL);
}

///**********************************************************************************
//Function Name :   SetEditable
//Input(s)      :   bSet - ture editable, false - noneditable
//Output        :   -
//Functionality :   to make the items in the list control editable
//Member of     :   CTxMsgList
//Friend of     :   -
//Authors       :   Ravi D
//Date Created  :   19/09/2006
//Modifications :   -
//************************************************************************************/
void CTxMsgList::SetEditable(BOOL bSet)
{
    if (!bSet)
    {
        EndEdit(TRUE);
    }
    m_bAllowEdit = bSet;
}

///**********************************************************************************
//Function Name :   IsEditable
//Input(s)      :   -
//Output        :   true - the list control is editable
//                  false - the list control is not editable
//Functionality :   to check whether the list control is editable or not
//Member of     :   CTxMsgList
//Friend of     :   -
//Authors       :   Ravi D
//Date Created  :   19/09/2006
//Modifications :   -
//************************************************************************************/
BOOL CTxMsgList::IsEditable(void)const
{
    return m_bAllowEdit;
}

///**********************************************************************************
//Function Name :   OnLButtonDblClk
//Input(s)      :   nFlags -Indicates whether various virtual keys are down
//                  point - the x- and y-coordinate of the cursor
//Output        :   -
//Functionality :   to allow the user to edit the list item
//Member of     :   CTxMsgList
//Friend of     :   -
//Authors       :   Ravi D
//Date Created  :   19/09/2006
//Modifications :   -
//************************************************************************************/
void CTxMsgList::OnLButtonDblClk(UINT nFlags, CPoint point)
{

    CFlickerFreeListCtrl::OnLButtonDblClk(nFlags, point);

    LVHITTESTINFO hti;
    hti.pt = point;
    const int IDX = SubItemHitTest(&hti);
    const BOOL BEFORE = CFlickerFreeListCtrl::GetCheck(IDX) > 0;
    const BOOL WAS_EDIT = _IsEditVisible();

    StartEdit(IDX, hti.iSubItem);
}

///**********************************************************************************
//Function Name :   OnLButtonDown
//Input(s)      :   nFlags -Indicates whether various virtual keys are down
//                  point - the x- and y-coordinate of the cursor
//Output        :   -
//Functionality :   if the list item is being edited stop editing
//Member of     :   CTxMsgList
//Friend of     :   -
//Authors       :   Ravi D
//Date Created  :   19/09/2006
//Modifications :   -
//************************************************************************************/
void CTxMsgList::OnLButtonDown(UINT nFlags, CPoint point)
{
    m_ptSubItemPoint = point;
    m_bSelected = FALSE;
    EndEdit(TRUE);
    CFlickerFreeListCtrl::OnLButtonDown(nFlags, point);
}

///**********************************************************************************
//Function Name :   PreTranslateMessage
//Input(s)      :   pMsg - Pointer to the message structure
//Output        :   true - message was translated and not dispatched
//                  false -  message was not translated and should be dispatched
//Functionality :   To translate window messages before they are dispatched
//Member of     :   CTxMsgList
//Friend of     :   -
//Authors       :   Ravi D
//Date Created  :   19/09/2006
//Modifications :   -
//************************************************************************************/
BOOL CTxMsgList::PreTranslateMessage(MSG* pMsg)
{
    if (pMsg->message == WM_KEYDOWN)
    {
        if (!_IsEditVisible())
        {
            // disable user from check/uncheck the checkboxes using space key
            // things get nasty if the user is HOLDING the space bar down
            if (pMsg->wParam == VK_SPACE)
            {
                return TRUE;
            }
        }
        else
        {
            POINT pt = m_ptEditting;
            switch (pMsg->wParam)
            {
                case VK_ESCAPE: // Cancel edit
                    EndEdit(FALSE);
                    return TRUE;

                case VK_RETURN: // Commit edit
                {
                    EndEdit(TRUE);
                    HWND hwnd = GetParent()->m_hWnd;
                    ::SendMessage(hwnd,WM_TX_EDIT_COMPLETE ,NULL,NULL);
                }
                return TRUE;

                case VK_TAB: // switch edit sub items
                    if (pt.y == GetColumnCount() - 1)
                    {
                        pt.y = 0;
                    }
                    else
                    {
                        pt.y++;
                    }
                    EndEdit(TRUE);
                    StartEdit(pt.x, pt.y);
                    return TRUE;

                case VK_UP: // edit upper item
                    if (pt.x > 0)
                    {
                        pt.x--;
                        EndEdit(TRUE);
                        StartEdit(pt.x, pt.y);
                        EnsureVisible(pt.x, FALSE);
                        return TRUE;
                    }
                    break;

                case VK_DOWN: // edit lower item
                    //if (pt.x < CListCtrl::GetItemCount() - 1)
                    if (pt.x < CFlickerFreeListCtrl::GetItemCount() - 1)
                    {
                        pt.x++;
                        EndEdit(TRUE);
                        StartEdit(pt.x, pt.y);
                        EnsureVisible(pt.x, FALSE);
                        return TRUE;
                    }
                    break;

                default:
                    break;
            }
        }
    }
    return CFlickerFreeListCtrl::PreTranslateMessage(pMsg);
}

///**********************************************************************************
//Function Name :   OnNMClick
//Input(s)      :   pNMHDR -
//                  pResult -
//Output            :   -
//Functionality :   To display the combo box and Retrieve the data from fibex
//Member of     :   CTxMsgList
//Friend of     :   -
//Authors           :   Ravi D
//Date Created  :   19/09/2006
//Modifications :   -
//************************************************************************************/
void CTxMsgList::OnNMClick(NMHDR* pNMHDR, LRESULT* pResult)
{

    CString omFrameId   = "";
    CString omFrameData = "";
    CString omFibexId   = "";
    int nFibexCount = 0;
    //IFibex* pFibex  =  NULL;
    int nFrameLength = 0;
    CString omFrameLength = "";
    //IFrames *pFrame = NULL;
    int nFrameCount = 0;
    NMITEMACTIVATE* pNMListView = reinterpret_cast<NMITEMACTIVATE*>(pNMHDR);

    g_Clicked = FALSE;
    switch(pNMListView->iSubItem)
    {
        case TX_COLUMN_TYPE: //Display slot type
        {
            CStringArray arr;
            arr.SetAtGrow(0, "static");
            arr.SetAtGrow(1, "dynamic");
            ShowCombo(pNMListView->iItem, pNMListView->iSubItem, arr);
            break;
        }
        case TX_COLUMN_CHANNEL:
        {
            CStringArray arr;
            arr.SetAtGrow(0, "A");
            arr.SetAtGrow(1, "B");
            arr.SetAtGrow(2, "AB");
            ShowCombo(pNMListView->iItem, pNMListView->iSubItem, arr);
            break;
        }
        /*      case TX_COLUMN_DATA_MATRIX: //Uncomment at Data Matrix implementation
                    {
                        g_Clicked = TRUE;

                        // Get Frame Id ,Frame Length and Frame Data  from Item Level
                        omFrameId   = GetItemText( pNMListView->iItem, 2);
                        omFrameLength = GetItemText( pNMListView->iItem, 6);
                        sscanf(omFrameLength, "%d",&nFrameLength);
                        omFrameData = GetItemText( pNMListView->iItem, 7);

                        /// Get the FibexKey

                        CMainFrame *pMainFrame = (CMainFrame * ) AfxGetMainWnd();
                        if ( pMainFrame->m_pFibexHan != NULL )
                        {
                            // Get Fibex Count
                            nFibexCount = pMainFrame->m_pFibexHan->GetCount();
                            if(nFibexCount > 0 )
                            {
                                pMainFrame->m_pFibexHan->MoveFirst();
                                pFibex = pMainFrame->m_pFibexHan->GetByIterator();
                                if(pFibex != NULL )
                                {
                                    CHex_Block_EditorDlg oHexBlockEditorDLG(pFibex,omFrameId,omFrameData);
                                    INT_PTR nResult = oHexBlockEditorDLG.DoModal();
                                    if( nResult == IDOK )
                                    {
                                    } /// nResult == IDOK
                                } //pFibex != NULL
                            } // nFibexCount > 0
                        }// pMainFrame->m_pFibexHan != NULL
                    }
                    break;
        */
        default:
            break;
    } // switch

    Update(pNMListView->iItem);

    *pResult = 0;
}

///**********************************************************************************
//Function Name :   OnNMDblclk
//Input(s)      :   pNMHDR -
//                  pResult -
//Output        :   -
//Functionality :   To display the combo box and Retrieve the data from fibex
//Member of     :   CTxMsgList
//Friend of     :   -
//Authors       :   Ravi D
//Date Created  :   19/09/2006
//Modifications :   -
//************************************************************************************/
void CTxMsgList::OnNMDblclk(NMHDR* pNMHDR, LRESULT* pResult)
{
    // TODO: Add your control notification handler code here

    *pResult = 0;
}

///**********************************************************************************
//Function Name :   ShowCombo
//Input(s)      :   nItem - Row
//                  nSubItem - Column
//                  omList - combo box list data
//Output            :   pointer to the combo box
//Functionality :   To display the combo box
//Member of     :   CTxMsgList
//Friend of     :   -
//Authors           :   Ravi D
//Date Created  :   19/09/2006
//Modifications :   -
//************************************************************************************/
CComboBox* CTxMsgList::ShowCombo(int nItem, int nSubItem, const CStringArray& omList)
{
    // Get the item text from the list control
    CString strFind = GetItemText(nItem, nSubItem);

    //basic code start
    CRect omRect;
    // Make sure that the item is visible
    if( !EnsureVisible(nItem, TRUE))
    {
        return NULL;
    }

    if(bIsChangeable(nItem,nSubItem) == false)
    {
        return NULL;
    }
    // Get the size of the list item
    GetSubItemRect(nItem, nSubItem, LVIR_BOUNDS, omRect);
    // Now scroll if we need to expose the column
    CRect omClientRect;
    // Get the list rect
    GetClientRect(omClientRect);
    // Check for scrolling
    if( omRect.left < 0 || omRect.left > omClientRect.right )
    {
        CSize size;
        size.cx = omRect.left;
        size.cy = 0;
        Scroll(size);
        omRect.left -= size.cx;
    }

    omRect.right = omRect.left + GetColumnWidth(nSubItem);
    // Reduce the size of the control if the list item is not completely
    // Visible
    if(omRect.right > omClientRect.right)
    {
        omRect.right = omClientRect.right;
    }
    //basic code end

    //dropdown area
    omRect.bottom += 100;

    // Set the standard style and combobox type

    DWORD dwStyle =  WS_CHILD |WS_VISIBLE | WS_BORDER | WS_HSCROLL |
                     WS_VSCROLL | CBS_SORT | CBS_DROPDOWNLIST;

    // Create the non editable combobox
    CListComboBox* pomCBox = NULL;

    // Create the control
    pomCBox = new CListComboBox(  nItem,        // Item Index
                                  nSubItem,     // Sub Item Index
                                  omList,       // Lsit of strings
                                  strFind,      // Selected Text
                                  FALSE);       // Editing is FALSE
    if( pomCBox != NULL )
    {
        // Create the UI
        pomCBox->Create(dwStyle, omRect, this, IDC_CONTROL);

        pomCBox->SetItemHeight(-1, 12);

        // Set the text
        pomCBox->SetWindowText(strFind);
        // Select the text from the list
        pomCBox->SelectString(-1 , strFind.GetBuffer(1));
        // Show the drop-down list
        pomCBox->ShowDropDown();
    }
    else
    {
        CString omStrErr;
        omStrErr.Format(defFLC_CREATE_FAILED,defCOMBO_ITEM);
        AfxMessageBox( omStrErr );
    }
    // The return the pointer just for reference
    // Destroy will delete this memory. So this should not be deleted outside
    return pomCBox;
}

///**********************************************************************************
//Function Name :   CalcHorzExtent
//Input(s)      :   pWnd -
//                  pList -
//Output            :   return the extent of the list
//Functionality :   To calculate the extent of the list
//Member of     :   CTxMsgList
//Friend of     :   -
//Authors           :   Ravi D
//Date Created  :   19/09/2006
//Modifications :   -
//************************************************************************************/
int CTxMsgList::CalcHorzExtent(CWnd* pWnd, CStringList* pList)
{
    int nExtent=0;
    if(pWnd!=NULL)
    {
        CDC* pDC = pWnd->GetDC();
        HFONT hFont = (HFONT)pWnd->SendMessage(WM_GETFONT); //why not pWnd->GetFont();..I like the send thing alot and
        CFont* pFont = CFont::FromHandle(hFont);            //this way I get to use this function..cool :)
        if(pFont!=NULL)                                     //ya what ever makes me happy,.right :}
        {
            CFont* pOldFont = pDC->SelectObject(pFont);
            CSize newExtent;
            POSITION pos = pList->GetHeadPosition();
            while(pos != NULL)
            {
                CString str(pList->GetNext(pos));
                newExtent = pDC->GetTextExtent(str);
                newExtent.cx += 6;
                if (newExtent.cx > nExtent)
                {
                    nExtent = newExtent.cx;
                }
            }
            pDC->SelectObject(pOldFont);
        }
        pWnd->ReleaseDC(pDC);
    }
    return nExtent;
}

///**********************************************************************************
//Function Name :   OnLvnEndlabeledit
//Input(s)      :   pNMHDR -
//                  pResult -
//Output            :   -
//Functionality :   This function is called when label editing for an item is about to end
//Member of     :   CTxMsgList
//Friend of     :   -
//Authors           :   Ravi D
//Date Created  :   19/09/2006
//Modifications :   -
//************************************************************************************/
void CTxMsgList::OnLvnEndlabeledit(NMHDR* pNMHDR, LRESULT* pResult)
{
    NMLVDISPINFO* pDispInfo = reinterpret_cast<NMLVDISPINFO*>(pNMHDR);
    // TODO: Add your control notification handler code here
    LV_ITEM* pItem = &(pDispInfo)->item;

    if( pItem->iItem != -1 &&  // valid item
            pItem->pszText != NULL)       // valid text
    {
        // Update the list text
        SetItemText( pItem->iItem, pItem->iSubItem, pItem->pszText);
    }
    *pResult = 0;
}

//***********************Anupam Kumar********************

//********************************************************************************
//*  Function Name        : GetTextFont
//*  Input(s)             : LOGFONT* plgfnt
//*  Output               : bool
//*  Member of            : CHexEdit
//*  Description          : Returns the current Text font
//*                       :
//*  Author               : Ravi
//*  Creation Date        : 09/11/2006
//*  Modifications        :
//**********************************************************************************

bool CTxMsgList::GetTextFont( LOGFONT* plgfnt)
{
    this->GetFont()->GetObject( sizeof(LOGFONT), (void*)plgfnt);
    return true;
}

//********************************************************************************
//  Function Name        : SetTextFont
//  Input(s)             : const LOGFONT& lgfnt
//  Output               : bool
//  Member of            : CTxMsgList
//  Description          : Set Text font
//                       :
//  Author               : Ravi
//  Creation Date        : 09/11/2006
//  Modifications        :
//**********************************************************************************

bool CTxMsgList::SetTextFont( const LOGFONT& lgfnt )
{
    m_fntText.DeleteObject();
    m_fntText.CreateFontIndirect( &lgfnt );
    SetFont( &m_fntText, TRUE );
    return true;
}
//********************************************************************************
//  Function Name        : SetFontBold
//  Input(s)             : const LOGFONT& lgfnt
//  Output               : bool
//  Member of            : CTxMsgList
//  Description          : Sets windows font
//
//  Author               : Ravi
//  Creation Date        : 09/11/2006
//  Modifications        :
//********************************************************************************
bool CTxMsgList::SetFontBold(bool bBold)
{
    LOGFONT lgfnt;

    GetTextFont( &lgfnt );
    lgfnt.lfWeight  = bBold?FW_BOLD:FW_DONTCARE;
    SetTextFont( lgfnt );
    return true;
}

//********************************************************************************
//  Function Name        : SetFontFaceName
//  Input(s)             : const CString& sFaceName
//  Output               : bool
//  Member of            : CTxMsgList
//  Description          :
//                       :
//  Author               : Ravi
//  Creation Date        : 09/11/2006
//  Modifications        :
//**********************************************************************************
bool CTxMsgList::SetFontFaceName( const CString& sFaceName )
{
    LOGFONT lgfnt;

    GetTextFont( &lgfnt );
    strcpy( lgfnt.lfFaceName, sFaceName );
    SetTextFont( lgfnt );
    return true;
}

//********************************************************************************
//  Function Name        : MeasureItem
//  Input(s)             : LPMEASUREITEMSTRUCT
//  Output               : bool
//  Member of            : CTxMsgList
//  Description          :
//                       :
//  Author               : Ravi
//  Creation Date        : 09/11/2006
//  Modifications        :
//**********************************************************************************
void CTxMsgList::MeasureItem (LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{

    TEXTMETRIC tm;
    HDC hDC = ::GetDC(NULL);
    CFont* pFont = GetFont();
    HFONT hFontOld = (HFONT)SelectObject(hDC, pFont->GetSafeHandle());
    GetTextMetrics(hDC, &tm);
    lpMeasureItemStruct->itemHeight = tm.tmHeight + tm.tmExternalLeading + 1;
    SelectObject(hDC, hFontOld);
    ::ReleaseDC(NULL, hDC);
}

//********************************************************************************
//  Function Name        : bIsChangeable
//  Input(s)             :  nItem       - Row Index
//                          nColumnIndex- Column Index
//  Output               : bool
//  Member of            : CHexEdit
//  Description          : Verifies whether the selected item is changeable or not
//  Author               : Sandip R. Bhirud
//  Creation Date        : 09/11/2006
//  Modifications        :
//**********************************************************************************
bool CTxMsgList::bIsChangeable(int nItem, int nColumnIndex)
{
    bool bIsEditable = true;
    //UCI_FLXTXMSGINFO flxTxMsgInfo;
    //((CMainFrame *)AfxGetMainWnd())->m_pDoc->m_opFlexRay->GetTxMsgInfo(nItem, &flxTxMsgInfo);


    //DWORD dwChangeableFlag = flxTxMsgInfo.dwChangeable;
    DWORD dwChangeableFlag = GetItemData(nItem);
    switch(nColumnIndex)
    {
        case TX_COLUMN_TYPE: // Message Type

            if ((dwChangeableFlag & RBIN_FLXTXMSGINFO_CHANGEABLE_FRAMETYPE) == FALSE)
            {
                bIsEditable = false;
            }
            break;
        case TX_COLUMN_MESSAGE_ID: // Message ID
            if ((dwChangeableFlag & RBIN_FLXTXMSGINFO_CHANGEABLE_IDENTIFIER) == FALSE)
            {
                bIsEditable = false;
            }
            break;
        case  TX_COLUMN_CYCLE_NO: // Cycle No
            if ((dwChangeableFlag & RBIN_FLXTXMSGINFO_CHANGEABLE_CYCLENUMBER) == FALSE)
            {
                bIsEditable = false;
            }
            break;
        case TX_COLUMN_CHANNEL: // Channel
            if((dwChangeableFlag & RBIN_FLXTXMSGINFO_CHANGEABLE_CHANNELID) == FALSE)
            {
                bIsEditable = false;
            }
            break;
        case TX_COLUMN_DATA_LENGTH: // Data length
            if ((dwChangeableFlag & RBIN_FLXTXMSGINFO_CHANGEABLE_DATALENGTH) == FALSE)
            {
                bIsEditable = false;
            }
            break;
        case TX_COLUMN_DATA_BYTES: // Data Bytes
            if ((dwChangeableFlag & RBIN_FLXTXMSGINFO_CHANGEABLE_DATA) == FALSE)
            {
                bIsEditable = false;
            }
            break;
    }
    return bIsEditable;
}

///**********************************************************************************
//Function Name :   vDataFormat
//Input(s)      :   omDataBytes[IN,OUT] - String reference
//                  nDataLength[OUT]    - Formatted string length
//                  bFlag       - TRUE for Data bytes
//                              - FALSE for string length
//Output        :   none
//Functionality :   returns pointer to the edit control
//Member of     :   CTxMsgList
//Friend of     :   -
//Authors       :   Ravi D
//Date Created  :   19/09/2006
//Modifications :   -
//************************************************************************************/
void CTxMsgList::vDataFormat(CString& omDataBytes,int* nDataLength,bool bFlag)
{
    CString omResult = _T("");
    int nTempIndex = 0;
    int nLength = 0;;

    //Removing Space
    omDataBytes.Replace(" ","");
    int nStringLength = omDataBytes.GetLength();

    // find the length
    for(int nIndex =(nStringLength - 1); nIndex >=0; nIndex--)
    {
        omResult.Insert(0,omDataBytes.GetAt(nIndex));
        nTempIndex++;

        // insert the space
        if((nTempIndex%4)== 0 )
        {
            // compare current length with input length
            if (nTempIndex != omDataBytes.GetLength() )
            {
                omResult.Insert(0, " ");    // insert space
            }
        }
    }

    // find the length for padding zero
    nTempIndex = omResult.GetLength();
    nTempIndex = ( nTempIndex % 5 );

    /// padding zero
    for(int nIndex = nTempIndex ; nIndex < 4; nIndex++)
    {
        omResult.Insert(0,'0');
    }

    // copy the data
    omDataBytes = omResult;
    omResult.Replace(" ","");
    if(bFlag) // Data Bytes format
    {
        *nDataLength = (omResult.GetLength() / 4) ;

    }
    else // Data length format
    {
        nLength = (*nDataLength -  (omResult.GetLength() / 4)) ;

        if ( nLength != 0 )
        {
            omResult.Insert(0, " ");    // insert space
        }
        /// padding zero
        for(int nIndex = 1; nIndex <= ( nLength * 4 ); nIndex++)
        {
            omResult.Insert(0, '0'); // insert space
            // insert the space
            if((nIndex%4)== 0 )
            {
                // compare current length with input length
                if ( nIndex != (nLength * 4 ) )
                {
                    omResult.Insert(0, " ");    // insert space
                }
            } // if

        } //for
        omDataBytes = omResult;
    }
}


