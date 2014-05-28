// NetworkStatsDlg.cpp : implementation file
/**********************************************************************************
Project         :   Frame
FileName        :   NetworkStatsDlg.cpp
Description     :   Implementation of CFlexRayNetworkStatsDlg class, to display the
                    network statistics
Directory       :
Version         :   V1.0
Authors         :   Ravi D
Date Created    :   17/10/2006
Date Modified   :
Modifications   :
**********************************************************************************/

#include "stdafx.h"
//#include "FRAME_BaseUI.h"
#include "FlexRayNetworkStatsDlg.h"
//#include "DisplayMatrix/BlockEditorMsgFormat.h"
#include "common.h"

// Bus Statistics Dialog
#define defHEADING_COLOR            RGB(150,0,0)
#define defITEM_COLOR               RGB(0,0,150)
#define defPARAMETER_COL_WIDTH      200
#define defVALUE_COL_WIDTH          90
// CFlexRayNetworkStatsDlg dialog

IMPLEMENT_DYNAMIC(CFlexRayNetworkStatsDlg, CDialog)
CFlexRayNetworkStatsDlg::CFlexRayNetworkStatsDlg(CFlexRayNetworkStats* pouNetworkStat,
        CWnd* pParent) : CDialog(CFlexRayNetworkStatsDlg::IDD, pParent)
{
    ASSERT(nullptr != pouNetworkStat);
    m_pouNetworkStat = pouNetworkStat;
    m_hParent = pParent->m_hWnd;
}

/**********************************************************************************
Function Name   :   ~CFlexRayNetworkStatsDlg()
Input(s)        :   -
Output          :   -
Functionality   :   Destructor
Member of       :   CFlexRayNetworkStatsDlg
Friend of       :   -
Authors         :   Ravi D
Date Created    :   17/10/2006
Modifications   :   -
************************************************************************************/
CFlexRayNetworkStatsDlg::~CFlexRayNetworkStatsDlg()
{
}

/**********************************************************************************
Function Name   :   DoDataExchange
Input(s)        :   pDX - Pointer to the exchange object
Output          :   -
Functionality   :   To exchange and validate the dialog data
Member of       :   CFlexRayNetworkStatsDlg
Friend of       :   -
Authors         :   Ravi D
Date Created    :   17/10/2006
Modifications   :   -
************************************************************************************/
void CFlexRayNetworkStatsDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_LSTC_FLEXRAY_NETWORK_STATISTICS, m_lstNetWorkStat);
}

BEGIN_MESSAGE_MAP(CFlexRayNetworkStatsDlg, CDialog)
    ON_MESSAGE(WM_HW_FLEXRAY_NETWORK_STATISTICS, OnUpdateNetworkStat)
    ON_WM_CREATE()
    ON_BN_CLICKED(IDOK, OnBnClickedOk)
    ON_WM_DESTROY()
    ON_WM_SIZE()
END_MESSAGE_MAP()

// CFlexRayNetworkStatsDlg message handlers

/**********************************************************************************
Function Name   :   OnInitDialog
Input(s)        :   -
Output          :   > 0 - set the focus to the first control in the dialog.
                    0 - if the focus has been explicitly set to one of the control
Functionality   :   Initialize the dialog controls
Member of       :   CFlexRayNetworkStatsDlg
Friend of       :   -
Authors         :   Ravi D
Date Created    :   17/10/2006
Modifications   :   -
************************************************************************************/
BOOL CFlexRayNetworkStatsDlg::OnInitDialog()
{
    CDialog::OnInitDialog();

    // set the icon
    SetIcon(AfxGetApp()->LoadIcon(IDI_ICON_FLEXRAY_CHANNEL), TRUE);

    CString strWndText;
    strWndText.LoadString(IDR_FLEX_NETWORKSTATISTICS_WINDOW_TEXT);
    SetWindowText(strWndText);

    //m_lstNetWorkStat.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);
    m_lstNetWorkStat.SetExtendedStyle( LVS_EX_FULLROWSELECT | LVS_EX_HEADERDRAGDROP );

    RECT Rect;
    m_lstNetWorkStat.GetWindowRect(&Rect);

    int nHeadingColor, nItemColor;
    nHeadingColor = defHEADING_COLOR;
    nItemColor = defITEM_COLOR;

    //LVCOLUMN lvcol;
    //lvcol.mask = LVCF_TEXT | LVCF_WIDTH;
    //lvcol.cx = (Rect.right - Rect.left) / 3 - 1;

    // Insert Column Header for the first column

    m_lstNetWorkStat.InsertColumn( 0, _("Parameters") );
    m_lstNetWorkStat.SetColumnWidth( 0, defPARAMETER_COL_WIDTH );

    // Insert Column Header for the second column
    m_lstNetWorkStat.InsertColumn(SECOND_COL, "Channel A");
    m_lstNetWorkStat.SetColumnWidth( 1, defVALUE_COL_WIDTH );

    // Insert Column Header for the third column
    m_lstNetWorkStat.InsertColumn(THIRD_COL, "Channel B");
    m_lstNetWorkStat.SetColumnWidth( 2, defVALUE_COL_WIDTH );

    // Initialize the Listview item structure.
    m_lstNetWorkStat.InsertItem(0,"Static Frames");
    m_lstNetWorkStat.SetItemData(0, nItemColor);

    m_lstNetWorkStat.InsertItem(1,"Dynamic Frames");
    m_lstNetWorkStat.SetItemData(1, nItemColor);

    m_lstNetWorkStat.InsertItem(2,"Sync Frames [fr/s]");
    m_lstNetWorkStat.SetItemData(2, nItemColor);

    m_lstNetWorkStat.InsertItem(3,"Sync Frames [total]");
    m_lstNetWorkStat.SetItemData(3, nItemColor);

    m_lstNetWorkStat.InsertItem(4,"Null Frames [fr/s]");
    m_lstNetWorkStat.SetItemData(4, nItemColor);

    m_lstNetWorkStat.InsertItem(5,"Null Frames [total]");
    m_lstNetWorkStat.SetItemData(5, nItemColor);

    m_lstNetWorkStat.InsertItem(6,"Errors [fr/s]");
    m_lstNetWorkStat.SetItemData(6, nItemColor);

    m_lstNetWorkStat.InsertItem(7,"Errors [total]");
    m_lstNetWorkStat.SetItemData(7, nItemColor);

    m_lstNetWorkStat.InsertItem(8,"Errors [total]");
    m_lstNetWorkStat.SetItemData(8, nItemColor);

    m_lstNetWorkStat.InsertItem(9,"Peak Load");
    m_lstNetWorkStat.SetItemData(9, nItemColor);

    m_lstNetWorkStat.InsertItem(10,"Average Load");
    m_lstNetWorkStat.SetItemData(10, nItemColor);

    m_lstNetWorkStat.InsertItem(11,"Frames [total]");
    m_lstNetWorkStat.SetItemData(11, nItemColor);

    m_lstNetWorkStat.InsertItem(12,"		Transmitted") ;
    m_lstNetWorkStat.SetItemData(12, nHeadingColor);

    // Initialize for transmission.
    m_lstNetWorkStat.InsertItem(13,"Static Frames") ;
    m_lstNetWorkStat.SetItemData(13, nItemColor);

    m_lstNetWorkStat.InsertItem(14,"Dynamic Frames") ;
    m_lstNetWorkStat.SetItemData(14, nItemColor);

    m_lstNetWorkStat.InsertItem(15,"Sync Frames [fr/s]") ;
    m_lstNetWorkStat.SetItemData(15, nItemColor);

    m_lstNetWorkStat.InsertItem(16,"Sync Frames [total]") ;
    m_lstNetWorkStat.SetItemData(16, nItemColor);

    m_lstNetWorkStat.InsertItem(17,"Null Frames [fr/s]") ;
    m_lstNetWorkStat.SetItemData(17, nItemColor);

    m_lstNetWorkStat.InsertItem(18,"Null Frames [total]") ;
    m_lstNetWorkStat.SetItemData(18, nItemColor);

    m_lstNetWorkStat.InsertItem(19,"Errors [fr/s]") ;
    m_lstNetWorkStat.SetItemData(19, nItemColor);

    m_lstNetWorkStat.InsertItem(20,"Errors [total]") ;
    m_lstNetWorkStat.SetItemData(20, nItemColor);

    // Initialize for reception.
    m_lstNetWorkStat.InsertItem(21,"		Received");
    m_lstNetWorkStat.SetItemData(21, nHeadingColor);

    m_lstNetWorkStat.InsertItem(22,"Static Frames");
    m_lstNetWorkStat.SetItemData(22, nItemColor);

    m_lstNetWorkStat.InsertItem(23,"Dynamic Frames");
    m_lstNetWorkStat.SetItemData(23, nItemColor);

    m_lstNetWorkStat.InsertItem(24,"Sync Frames [fr/s]");
    m_lstNetWorkStat.SetItemData(24, nItemColor);

    m_lstNetWorkStat.InsertItem(25,"Sync Frames [total]");
    m_lstNetWorkStat.SetItemData(25, nItemColor);

    m_lstNetWorkStat.InsertItem(26,"Null Frames [fr/s]");
    m_lstNetWorkStat.SetItemData(26, nItemColor);

    m_lstNetWorkStat.InsertItem(27,"Null Frames [total]");
    m_lstNetWorkStat.SetItemData(27, nItemColor);

    m_lstNetWorkStat.InsertItem(28,"Errors [fr/s]");
    m_lstNetWorkStat.SetItemData(28, nItemColor);

    m_lstNetWorkStat.InsertItem(29,"Errors [total]");
    m_lstNetWorkStat.SetItemData(29, nItemColor);

    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

/**********************************************************************************
Function Name   :   OnUpdateNetworkStat
Input(s)        :   WPARAM - Windows Message Parameter
                    LPARAM - Windows Message Parameter
Output          :   for success returns 1
                    for failure returns 0
Functionality   :   Updates the network statistics dialog box for the network values
Member of       :   CFlexRayNetworkStatsDlg
Friend of       :
Authors         :   Sandip R. Bhirud
Date Created    :   18-12-2006
Modifications   :
************************************************************************************/
LRESULT CFlexRayNetworkStatsDlg::OnUpdateNetworkStat(WPARAM, LPARAM)
{
    TCHAR szDataValue[52];
    DWORD lTotal = 0;

    SSUBFLEXRAYBUSSTATISTICS*       psTxBusStats = m_pouNetworkStat->nGetChannelStats();
    if(psTxBusStats == nullptr)
    {
        return FALSE;
    }


    vDisplayStats(psTxBusStats,0);
    //Total Channel A
    lTotal = psTxBusStats->m_StaticFramesChA + psTxBusStats->m_DynamicFramesChA + psTxBusStats->m_SyncFrameChA + psTxBusStats->m_NullFrameChA ;
    sprintf(szDataValue,_T("%ld"),lTotal);
    m_lstNetWorkStat.SetItemText(11,SECOND_COL,szDataValue);

    //Total Channel B
    lTotal = psTxBusStats->m_StaticFramesChB + psTxBusStats->m_DynamicFramesChB + psTxBusStats->m_SyncFrameChB + psTxBusStats->m_NullFrameChB ;
    sprintf(szDataValue,_T("%ld"),lTotal);
    m_lstNetWorkStat.SetItemText(11,THIRD_COL,szDataValue);

    vFillTxRxContent();

    return (LRESULT)1;
}

void CFlexRayNetworkStatsDlg::vFillTxRxContent()
{
    SSUBFLEXRAYBUSSTATISTICS*       psTxBusStats;
    SSUBFLEXRAYBUSSTATISTICS*       psRxBusStats;

    psTxBusStats = m_pouNetworkStat->nGetTxStats();
    if(psTxBusStats != nullptr)
    {
        vDisplayStats(psTxBusStats,13);
    }
    psRxBusStats = m_pouNetworkStat->nGetRxStats();
    if(psRxBusStats != nullptr)
    {
        vDisplayStats(psTxBusStats,22);
    }
}

void CFlexRayNetworkStatsDlg::vDisplayStats(SSUBFLEXRAYBUSSTATISTICS* psBusStats, int nStartIndex)
{
    //Update for Channel A
    //Static Frame count
    TCHAR szDataValue[52];
    sprintf(szDataValue,_T("%ld"),psBusStats->m_StartUpFrameChA);
    m_lstNetWorkStat.SetItemText(nStartIndex + 0,SECOND_COL,szDataValue);

    //Dynamic Frame count
    sprintf(szDataValue,_T("%ld"),psBusStats->m_DynamicFramesChA);
    m_lstNetWorkStat.SetItemText(nStartIndex + 1,SECOND_COL,szDataValue);

    //Sync Frame total
    sprintf(szDataValue,_T("%ld"),psBusStats->m_SyncFramePerSecChA);
    m_lstNetWorkStat.SetItemText(nStartIndex + 2,SECOND_COL,szDataValue);

    //Sync Frame total
    sprintf(szDataValue,_T("%ld"),psBusStats->m_SyncFrameChA);
    m_lstNetWorkStat.SetItemText(nStartIndex + 3,SECOND_COL,szDataValue);

    //nullptr Frame count per Sec
    sprintf(szDataValue,_T("%ld"),psBusStats->m_NullFramePerSecChA);
    m_lstNetWorkStat.SetItemText(nStartIndex + 4,SECOND_COL,szDataValue);

    //nullptr Frame count
    sprintf(szDataValue,_T("%ld"),psBusStats->m_NullFrameChA);
    m_lstNetWorkStat.SetItemText(nStartIndex + 5,SECOND_COL,szDataValue);


    //Static Frame count
    sprintf(szDataValue,_T("%ld"),psBusStats->m_StaticFramesChB);
    m_lstNetWorkStat.SetItemText(nStartIndex + 0,THIRD_COL,szDataValue);

    //Dynamic Frame count
    sprintf(szDataValue,_T("%ld"),psBusStats->m_DynamicFramesChB);
    m_lstNetWorkStat.SetItemText(nStartIndex + 1,THIRD_COL,szDataValue);

    //Sync Frame total
    sprintf(szDataValue,_T("%ld"),psBusStats->m_SyncFramePerSecChB);
    m_lstNetWorkStat.SetItemText(nStartIndex + 2,THIRD_COL,szDataValue);

    //Sync Frame total
    sprintf(szDataValue,_T("%ld"),psBusStats->m_SyncFrameChB);
    m_lstNetWorkStat.SetItemText(nStartIndex + 3,THIRD_COL,szDataValue);

    //nullptr Frame count per Sec
    sprintf(szDataValue,_T("%ld"),psBusStats->m_NullFramePerSecChB);
    m_lstNetWorkStat.SetItemText(nStartIndex + 4,THIRD_COL,szDataValue);

    //nullptr Frame count
    sprintf(szDataValue,_T("%ld"),psBusStats->m_NullFrameChB);
    m_lstNetWorkStat.SetItemText(nStartIndex + 5,THIRD_COL,szDataValue);

}

/*******************************************************************************
  Function Name  : OnSize
  Input(s)       : UINT nType, int cx, int cy
  Output         : -
  Functionality  : This function Makes the List control to the same size as
                   that of dialog whenever dialog window is resized.
  Member of      : CBusStatisticsDlg
  Author(s)      : Ashwin R Uchil
  Date Created   : 23.8.2013
  Modifications  :
*******************************************************************************/
void CFlexRayNetworkStatsDlg::OnSize(UINT nType, int cx, int cy)
{
    CDialog::OnSize(nType, cx, cy);

    if(m_lstNetWorkStat.m_hWnd)
    {
        CRect omClientRect;
        GetClientRect(&omClientRect);
        m_lstNetWorkStat.MoveWindow(&omClientRect);
    }
}

/**********************************************************************************
Function Name   :   OnBnClickedOk
Input(s)        :   None
Output          :
Functionality   :
Member of       :   CFlexRayNetworkStatsDlg
Friend of       :
Authors         :   Sandip R. Bhirud
Date Created    :   18-12-2006
Modifications   :
************************************************************************************/
void CFlexRayNetworkStatsDlg::OnBnClickedOk()
{
    DestroyWindow();
}

/**********************************************************************************
//Function Name :   PreTranslateMessage
//Input(s)      :   None
//Output        :
//Functionality :   Blocks Escape and Enter keys
//Member of     :   CFlexRayNetworkStatsDlg
//Friend of     :
//Authors       :   Sandip R. Bhirud
//Date Created  :   05-02-2007
//Modifications :
************************************************************************************/
BOOL CFlexRayNetworkStatsDlg::PreTranslateMessage(MSG* pMsg)
{
    if (pMsg->message == WM_KEYDOWN)
    {
        if (pMsg->wParam == VK_ESCAPE)
        {
            pMsg->wParam = 0;
        }
    }

    return CDialog::PreTranslateMessage(pMsg);
}

void CFlexRayNetworkStatsDlg::OnDestroy()
{
    CDialog::OnDestroy();

    //// TODO: Add your message handler code here
    //BOOL Result = ::PostMessage(m_hParent, WM_NOTIFICATION_FROM_OTHER,
    //    (WPARAM) eWINID_NETWORK_STATISTICS, (LPARAM) WM_CLOSE);
}

void CFlexRayNetworkStatsDlg::GetConfigData(BYTE ** /* ppvConfigData */, UINT & /* unLength */)
{
}

void CFlexRayNetworkStatsDlg::SetConfigData(BYTE * /* pvDataStream */)
{
}
