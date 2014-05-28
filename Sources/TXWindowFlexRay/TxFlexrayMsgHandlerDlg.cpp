// FlexrayMsgHandlerDlg.cpp : implementation file
//

#include "TxWindowFlexRay_stdafx.h"
#include "DataTypes/Base_FlexRay_Buffer.h"
//#include "Base_WrapperErrorLogger.h"
//#include "DIL/Dil_extern.h"
#include "TxFlexrayMsgHandlerDlg.h"
#include "TxFlexRayDataStore.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


#define FIRST_COL       0       // First column of the List control.
#define SECOND_COL      1       // Second column of the List control.
#define THIRD_COL       2       // Third column of the List control.
#define FOURTH_COL      3
#define FIFTH_COL       4



CFlexrayMsgHandlerDlg::CFlexrayMsgHandlerDlg(INT nChannel , CWnd* pParent /*=nullptr*/)
    : CDialog(CFlexrayMsgHandlerDlg::IDD, pParent)
{
    m_nCurrentChannel = nChannel;
}

void CFlexrayMsgHandlerDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_LSTC_MSG_LIST, m_lstcMsgList);
}

BEGIN_MESSAGE_MAP(CFlexrayMsgHandlerDlg, CDialog)
    ON_WM_PAINT()
    ON_WM_QUERYDRAGICON()
    //}}AFX_MSG_MAP
    ON_BN_CLICKED(IDC_CBTN_SEL_MSG, OnBnClickedCbtnSelectmsg)
    ON_BN_CLICKED(IDC_CBTN_CANCEL_MSG_SEL, OnBnClickedCbtnCancelmsgselection)
    ON_NOTIFY(LVN_COLUMNCLICK, IDC_LSTC_MSG_LIST, OnColumnclickLstcMsgDetails)
END_MESSAGE_MAP()

void CFlexrayMsgHandlerDlg::vRefreshMsgList(void)
{
    m_lstcMsgList.DeleteAllItems();

    std::list<FRAME_STRUCT>::iterator itrFramelist = CTxFlexRayDataStore::ouGetTxFlexRayDataStoreObj().m_ouFrameList[m_nCurrentChannel].begin();
    int nRow = 0;
    CString strValue="";

    for ( ; itrFramelist != CTxFlexRayDataStore::ouGetTxFlexRayDataStoreObj().m_ouFrameList[m_nCurrentChannel].end(); itrFramelist++)
    {
        strValue.Format("%s", itrFramelist->m_strFrameName.c_str() );
        m_lstcMsgList.InsertItem(nRow, strValue);

        //m_lstcMsgList.SetItemText(nRow, 1, strValue);

        strValue.Format("%d", itrFramelist->m_nSlotId );
        m_lstcMsgList.SetItemText(nRow, 1, strValue);

        strValue.Format("%d", itrFramelist->m_nBaseCycle );
        m_lstcMsgList.SetItemText(nRow, 2, strValue);

        strValue.Format("%d", itrFramelist->m_nReptition );
        m_lstcMsgList.SetItemText(nRow, 3, strValue);

        if ( itrFramelist->m_ouChannel == CHANNEL_A )
        {
            strValue.Format("%s", "A" );
        }
        else if ( itrFramelist->m_ouChannel == CHANNEL_B )
        {
            strValue.Format("%s", "B" );
        }
        else if ( itrFramelist->m_ouChannel == CHANNEL_AB )
        {
            strValue.Format("%s", "AB" );
        }

        m_lstcMsgList.SetItemText(nRow, 4, strValue);

        nRow++;
    }
}

BOOL CFlexrayMsgHandlerDlg::OnInitDialog()
{
    CDialog::OnInitDialog();

    ModifyStyle(LVS_TYPEMASK, LVS_REPORT);
    m_lstcMsgList.ModifyStyle(LVS_SINGLESEL, 0);
    m_lstcMsgList.SetExtendedStyle(LVS_EX_CHECKBOXES | LVS_EX_FULLROWSELECT
                                   | LVS_EX_GRIDLINES | LVS_EX_INFOTIP);

    m_lstcMsgList.InsertColumn(0, _T("Message"), LVCFMT_LEFT);
    m_lstcMsgList.InsertColumn(1, _T("Frame Id"), LVCFMT_LEFT);
    m_lstcMsgList.InsertColumn(2, _T("Base Cycle"), LVCFMT_LEFT);
    m_lstcMsgList.InsertColumn(3, _T("Repetition"), LVCFMT_LEFT);
    m_lstcMsgList.InsertColumn(4, _T("Channel"), LVCFMT_LEFT);

    RECT Rect;
    m_lstcMsgList.GetWindowRect(&Rect);
    int ColumnWidth = (Rect.right - Rect.left) / 5;
    for (int i = 0; i < 5; i++)
    {
        m_lstcMsgList.SetColumnWidth(i, ColumnWidth - 1);
    }



    // Associate Header control Image List
    if( m_omHeaderImageList.m_hImageList == nullptr )
    {
        m_omHeaderImageList.Create( IDR_BMP_CHECKBOX,
                                    16,
                                    1,
                                    RGB(0,0,255) );
    }
    // Set the Image List
    // Only if it is sucessfully created
    if( m_omHeaderImageList.m_hImageList != nullptr )
    {
        CHeaderCtrl* pHeader = m_lstcMsgList.GetHeaderCtrl();
        if( pHeader != nullptr )
        {
            pHeader->SetImageList( &m_omHeaderImageList );
            HDITEM hditem;
            hditem.mask = HDI_IMAGE | HDI_FORMAT;
            if( pHeader->GetItem(0, &hditem ) == TRUE )
            {
                hditem.fmt |=  HDF_IMAGE;
                hditem.iImage = 0;
                pHeader->SetItem(0, &hditem );
            }
        }
    }
    vRefreshMsgList();
    return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CFlexrayMsgHandlerDlg::OnPaint()
{
    if (IsIconic())
    {
        CPaintDC dc(this); // device context for painting

        SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

        // Center icon in client rectangle
        int cxIcon = GetSystemMetrics(SM_CXICON);
        int cyIcon = GetSystemMetrics(SM_CYICON);
        CRect rect;
        GetClientRect(&rect);
        int x = (rect.Width() - cxIcon + 1) / 2;
        int y = (rect.Height() - cyIcon + 1) / 2;

        // Draw the icon
        dc.DrawIcon(x, y, m_hIcon);
    }
    else
    {
        CDialog::OnPaint();
    }
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CFlexrayMsgHandlerDlg::OnQueryDragIcon()
{
    return static_cast<HCURSOR>(m_hIcon);
}

void CFlexrayMsgHandlerDlg::OnBnClickedCbtnSelectmsg()
{
    for (int i = 0; i < m_lstcMsgList.GetItemCount(); i++)
    {
        if (m_lstcMsgList.GetCheck(i))
        {
            m_pSelectedItem[m_pSelectedItem.size()] = i;
        }
    }
    OnOK();
}

void CFlexrayMsgHandlerDlg::OnBnClickedCbtnCancelmsgselection()
{
    OnCancel();
}
void CFlexrayMsgHandlerDlg::OnColumnclickLstcMsgDetails(NMHDR* pNMHDR, LRESULT* pResult)
{
    NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
    // If it is first column
    if( pNMListView->iSubItem == 0 )
    {
        // Get the status from Image Index
        BOOL bToBeChecked = FALSE;
        // Change header Control Image Index
        CHeaderCtrl* pHeader = m_lstcMsgList.GetHeaderCtrl();
        if( pHeader != nullptr )
        {
            // Get Current Image Index
            HDITEM hditem;
            hditem.mask = HDI_IMAGE | HDI_FORMAT;
            if( pHeader->GetItem(0, &hditem ) == TRUE )
            {
                // Image Index To be Set
                int nNewImageIndex = 0;
                // Toggle Image Index
                if( hditem.iImage == 0 )
                {
                    bToBeChecked = TRUE;
                    nNewImageIndex = 1;
                }

                // Update Image Index
                hditem.fmt |=  HDF_IMAGE;
                hditem.iImage = nNewImageIndex;
                pHeader->SetItem(0, &hditem );
                // Update Message Check Value
                vSetMessageCheckValue( bToBeChecked );

                /*CTxFunctionsView* pView = (CTxFunctionsView*)
                                          pomGetFunctionsViewPointer();

                if( pView != nullptr )
                {
                    if(pView->m_CheckBoxAutoUpdate.GetCheck() == BST_CHECKED)
                    {
                        pView->vAccessButtonApply();
                        this->SetFocus();
                    }
                }*/
            }
        }
    }
    *pResult = 0;
}

void CFlexrayMsgHandlerDlg::vSetMessageCheckValue( BOOL bToBeChecked )
{
    for (int i = 0; i < m_lstcMsgList.GetItemCount(); i++)
    {
        m_lstcMsgList.SetCheck(i, bToBeChecked);
    }
}
