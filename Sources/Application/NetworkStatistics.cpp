// NetworkStatistics.cpp : implementation file
//

#include "stdafx.h"
#include "NetworkStatistics.h"
#include "BusStatisticsDlg.h"
#include "LINBusStatisticsDlg.h"
#include "Application\InterfaceGetter.h"

// CNetworkStatistics
//OnSize() logic is taken from MSDN -- http://support.microsoft.com/kb/300606

/* constructor */
sBUSSTATTISTICSDATA::sBUSSTATTISTICSDATA()
{
    m_bIsDirty = false;
    //One for the first default parameters column
    m_nColumnCount = CHANNEL_CAN_MAX + 1;
}

void sBUSSTATTISTICSDATA::vLoadDefaults()
{
    //For first column 'Parameters'
    m_arrbColumnVisible[0] = true;
    m_arrnOrder[0] = 0;
    m_arrnColWidth[0] = defPARAMETER_COL_WIDTH;


    for(short i = 1; i < CHANNEL_CAN_MAX+1; i++)
    {
        m_arrbColumnVisible[i] = true;
        m_arrnOrder[i] = i;
        m_arrnColWidth[i] = defVALUE_COL_WIDTH;
    }
    m_sBusStatsDlgCoOrd = m_sDefaultBusStatsDlgCoOrd;
    m_sBusStatsDlgCoOrd.showCmd = SW_HIDE;
}

IMPLEMENT_DYNAMIC(CCommonStatistics, CPropertyPage)

BEGIN_MESSAGE_MAP(CCommonStatistics, CPropertyPage)

END_MESSAGE_MAP()

CCommonStatistics::CCommonStatistics(UINT unIDtemplate, ETYPE_BUS eBus)
    : CPropertyPage(unIDtemplate),
      m_eBusType(eBus)
{
    m_pxmlNodePtr = NULL;
}

/*******************************************************************************
  Function Name  : vUpdateChannelCountInfo
  Input(s)       : nChannelCount
  Output         : -
  Functionality  : Updates the channel count value when a hardware interface
                   is updated.
  Member of      : CBusStatisticsDlg
  Author(s)      : Arunkumar K
  Date Created   : 07-04-2011
  Modifications  :
*******************************************************************************/
void CCommonStatistics::vUpdateChannelCountInfo(int nChannelCount)
{
    if(m_nChannelCount != nChannelCount)
    {
        if(m_nChannelCount > nChannelCount)
        {
            INT nChannelIndex = nChannelCount+1;
            for(int i = nChannelCount+1; i <= m_nChannelCount; i++)
            {
                m_omStatList.DeleteColumn(nChannelIndex);
            }
        }
        else
        {
            for(int i = m_nChannelCount+1; i <= nChannelCount; i++)
            {
                CString cs;
                cs.Format( defSTR_CHANNEL_NAME_FORMAT, _(defSTR_CHANNEL_NAME), i);
                m_omStatList.InsertColumn( i , cs, LVCFMT_CENTER );
                m_omStatList.SetColumnWidth( i , defVALUE_COL_WIDTH );
            }
        }

        m_nChannelCount = nChannelCount;
    }
}

IMPLEMENT_DYNAMIC(CNetworkStatistics, CPropertySheet)

BEGIN_MESSAGE_MAP(CNetworkStatistics, CPropertySheet)
    ON_WM_CLOSE()
    ON_WM_SIZE()
    ON_WM_CREATE()
END_MESSAGE_MAP()

CNetworkStatistics::CNetworkStatistics(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
    :CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
{

    m_pCopyBusStsticsNode_LIN = NULL;

    for(UINT unIndex = 0; unIndex < BUS_TOTAL; unIndex++)
    {
        m_omStatistics[unIndex] = NULL;
    }

    m_bIsWindowInitialised = FALSE;
}

CNetworkStatistics::CNetworkStatistics(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
    :CPropertySheet(pszCaption, pParentWnd, iSelectPage)
{

    m_pCopyBusStsticsNode_LIN = NULL;

    for(UINT unIndex = 0; unIndex < BUS_TOTAL; unIndex++)
    {
        m_omStatistics[unIndex] = NULL;
    }

    m_bIsWindowInitialised = FALSE;
}


CNetworkStatistics::CNetworkStatistics(UINT unChnl[BUS_TOTAL])
{
    m_bIsWindowInitialised = FALSE;

    for(UINT unIndex = 0; unIndex < BUS_TOTAL; unIndex++)
    {
        m_omStatistics[unIndex] = NULL;
    }

    vAddPages(unChnl);
}

CNetworkStatistics::~CNetworkStatistics()
{
}

void CNetworkStatistics::vAddPages(UINT unChnl[BUS_TOTAL])
{
    for(UINT unIndex = 0; unIndex < BUS_TOTAL; unIndex++)
    {
        switch(unIndex)
        {
            case CAN:
                m_omStatistics[CAN] = new CBusStatisticsDlg(GetICANBusStat(), NULL, unChnl[CAN]);
                AddPage(m_omStatistics[CAN]);
                break;
            case LIN:
                m_omStatistics[LIN] = new CLINBusStatisticsDlg(GetILINBusStat(), NULL, unChnl[LIN]);
                AddPage(m_omStatistics[LIN]);
                break;
            default:
                break;
        }
    }

    /*m_cBSD = new CBusStatisticsDlg(GetICANBusStat(), NULL, unChlCAN);
    m_lBSD = new CLINBusStatisticsDlg(GetILINBusStat(), NULL, unChlLIN);
    AddPage(m_cBSD);
    AddPage(m_lBSD);
    */
}

int CNetworkStatistics::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    if (CPropertySheet::OnCreate(lpCreateStruct) == -1)
    {
        return -1;
    }

    // keep client area
    CRect rect;
    GetClientRect(&rect);
    // set resizable style
    ModifyStyle(DS_MODALFRAME, WS_POPUP | WS_THICKFRAME);
    // adjust size to reflect new style
    ::AdjustWindowRectEx(&rect, GetStyle(),
                         ::IsMenu(GetMenu()->GetSafeHmenu()), GetExStyle());
    SetWindowPos(NULL, 0, 0, rect.Width(), rect.Height(), SWP_FRAMECHANGED|
                 SWP_NOMOVE|SWP_NOZORDER|SWP_NOACTIVATE|SWP_NOREPOSITION);

    return 0;
}

BOOL CNetworkStatistics::OnInitDialog()
{
    CPropertySheet::OnInitDialog();

    HMODULE hModAdvancedUILib = ::LoadLibrary("AdvancedUIPlugIn.dll");

    if ( hModAdvancedUILib )
    {
        SetIcon( ::LoadIcon(hModAdvancedUILib, MAKEINTRESOURCE( IDI_ICON_NETWORKSTAT )), TRUE);
        ::FreeLibrary(hModAdvancedUILib);
        hModAdvancedUILib = NULL;
    }
    else
    {
        SetIcon( AfxGetApp()->LoadIcon(IDI_ICON_NETWORKSTAT), TRUE);
    }

    m_pCopyBusStsticsNode_LIN = NULL;

    CRect r;
    GetWindowRect(&r);
    r.bottom += GetSystemMetrics(SM_CYMENU);
    MoveWindow(r);

    m_nMincX = r.Width();
    m_nMincY = r.Height();
    // After this point we allow resize code to kick in

    GetClientRect(&m_rCrt);

    //SetWindowPlacement(&m_sBusStatsDlgCoOrd);

    m_bIsWindowInitialised = TRUE;

    return TRUE;
}


void CNetworkStatistics::OnSize(UINT nType, int cx, int cy)
{
    CRect r1;
    CPropertySheet::OnSize(nType, cx, cy);


    if (m_bIsWindowInitialised == FALSE)
    {
        return;
    }

    CTabCtrl* pTab = GetTabControl();
    ASSERT(NULL != pTab && IsWindow(pTab->m_hWnd));

    int dx = cx - m_rCrt.Width();
    int dy = cy - m_rCrt.Height();
    GetClientRect(&m_rCrt);

    HDWP hDWP = ::BeginDeferWindowPos(5);

    pTab->GetClientRect(&r1);
    r1.right += dx;
    r1.bottom += dy;
    ::DeferWindowPos(hDWP, pTab->m_hWnd, NULL,
                     0, 0, r1.Width(), r1.Height(),
                     SWP_NOACTIVATE|SWP_NOMOVE|SWP_NOZORDER);

    // Move all buttons with the lower right sides
    for (CWnd* pChild = GetWindow(GW_CHILD);
            pChild != NULL;
            pChild = pChild->GetWindow(GW_HWNDNEXT))
    {
        if (pChild->SendMessage(WM_GETDLGCODE) & DLGC_BUTTON)
        {
            pChild->GetWindowRect(&r1);
            ScreenToClient(&r1);
            r1.top += dy;
            r1.bottom += dy;
            r1.left+= dx;
            r1.right += dx;
            ::DeferWindowPos(hDWP, pChild->m_hWnd, NULL,
                             r1.left, r1.top, 0, 0,
                             SWP_NOACTIVATE|SWP_NOSIZE|SWP_NOZORDER);
        }
        // Resize everything else...
        else
        {
            pChild->GetClientRect(&r1);
            r1.right += dx;
            r1.bottom += dy;
            ::DeferWindowPos(hDWP, pChild->m_hWnd, NULL, 0, 0, r1.Width(), r1.Height(),SWP_NOACTIVATE|SWP_NOMOVE|SWP_NOZORDER);
        }

    }
    //m_cBSD->MoveWindow(0, 0, cx, cy);

    ::EndDeferWindowPos(hDWP);
}

void CNetworkStatistics::OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI)
{
    CPropertySheet::OnGetMinMaxInfo(lpMMI);
    lpMMI->ptMinTrackSize.x = m_nMincX;
    lpMMI->ptMinTrackSize.y = m_nMincY;
}


void CNetworkStatistics::AutoMove(int iID, double dXMovePct, double dYMovePct, double dXSizePct, double dYSizePct)
{
    ASSERT((dXMovePct + dXSizePct) <= 100.0);   // can't use more than 100% of the resize for the child
    ASSERT((dYMovePct + dYSizePct) <= 100.0);   // can't use more than 100% of the resize for the child
    SMovingChild s;
    GetDlgItem(iID, &s.m_hWnd);
    ASSERT(s.m_hWnd != NULL);
    s.m_dXMoveFrac = dXMovePct / 100.0;
    s.m_dYMoveFrac = dYMovePct / 100.0;
    s.m_dXSizeFrac = dXSizePct / 100.0;
    s.m_dYSizeFrac = dYSizePct / 100.0;
    ::GetWindowRect(s.m_hWnd, &s.m_rcInitial);
    ScreenToClient(s.m_rcInitial);
    m_MovingChildren.push_back(s);
}

void CNetworkStatistics::vSendMessage(ETYPE_BUS eBus, BOOL bUpdateSpecific)
{
    switch(eBus)
    {
        case CAN:
            m_omStatistics[CAN]->SendMessage(WM_DISPLAY_MESSAGE, 1, bUpdateSpecific);
            break;
        case LIN:
            m_omStatistics[LIN]->SendMessage(WM_DISPLAY_MESSAGE, 1, bUpdateSpecific);
            break;
    }
}

HRESULT CCommonStatistics::vGetConfigData(xmlNodePtr& pxmlNodePtr)
{
    vGetConfigDetails(pxmlNodePtr);

    return S_OK;
}

HRESULT CCommonStatistics::vGetConfigDetails(xmlNodePtr& pxmlNodePtr)
{
    const char* omcVarChar ;
    if(m_omStatList.m_hWnd == NULL)
    {
        if(m_pxmlNodePtr != NULL)
        {
            xmlNodePtr pBUSStatNode = NULL;
            pBUSStatNode = xmlCopyNode(m_pxmlNodePtr, 1);
            xmlAddChild(pxmlNodePtr, pBUSStatNode);
        }
        return true;
    }

    switch (m_eBusType)
    {
        case CAN:
            pxmlNodePtr = xmlNewNode(NULL, BAD_CAST DEF_BUS_STATS_CAN);

            break;
        case LIN:
            pxmlNodePtr = xmlNewNode(NULL, BAD_CAST DEF_BUS_STATS_LIN);
            break;
        default:
            break;
    }
    CHeaderCtrl* pHeaderCtrl = m_omStatList.GetHeaderCtrl();
    int  nColumnCount = pHeaderCtrl->GetItemCount();
    LPINT pnOrder = (LPINT) malloc(nColumnCount*sizeof(int));
    m_omStatList.GetColumnOrderArray(pnOrder, nColumnCount);

    LPINT pnOrderTemp = (LPINT) malloc(nColumnCount*sizeof(int));

    for(INT nIndex = 0; nIndex < nColumnCount; nIndex++)
    {
        if(pnOrder != NULL)
        {
            INT nColumn = pnOrder[nIndex];
            pnOrderTemp[nColumn] = nIndex;
        }
    }

    LVCOLUMN    oCol;
    TCHAR pcName[MAX_PATH]; // sufficient for now
    oCol.mask = LVCF_TEXT;
    oCol.pszText = (LPTSTR)pcName;
    oCol.cchTextMax = MAX_PATH;

    for (int iItr = 0 ; iItr < nColumnCount; iItr++)
    {

        xmlNodePtr pNodeColumn = xmlNewNode(NULL, BAD_CAST DEF_COLUMN);
        xmlAddChild(pxmlNodePtr, pNodeColumn);

        m_omStatList.GetColumn(iItr, &oCol);

        //  <ID>Channel 1</ID>
        CString csColumnName;
        csColumnName.Format("%s", oCol.pszText);
        omcVarChar = csColumnName;
        xmlNodePtr pColName = xmlNewChild(pNodeColumn, NULL, BAD_CAST DEF_ID,BAD_CAST omcVarChar);
        xmlAddChild(pNodeColumn, pColName);

        //<Order>1</Order>
        CString csOrder;
        csOrder.Format("%d", pnOrderTemp[iItr] + 1);
        omcVarChar = csOrder;
        xmlNodePtr pOrder = xmlNewChild(pNodeColumn, NULL, BAD_CAST DEF_MWND_ORDER,BAD_CAST omcVarChar);
        xmlAddChild(pNodeColumn, pOrder);


        //<IsVisible>bool</IsVisible>
        CString csWidth;
        csWidth.Format("%d", m_omStatList.GetColumnWidth(iItr));
        omcVarChar = csWidth;
        xmlNodePtr pWidth = xmlNewChild(pNodeColumn, NULL, BAD_CAST DEF_MWND_COL_WIDTH,BAD_CAST omcVarChar);
        xmlAddChild(pNodeColumn, pWidth);

        CString csVisible;

        bool bIsVisible = false;
        bIsVisible = m_omStatList.IsColumnShown(iItr);
        if(bIsVisible == true)
        {
            csVisible = "1";
        }
        else
        {
            csVisible = "0";
        }
        //csVisible.Format("%d",sm_sBusSerializationData.m_arrbColumnVisible[iItr]);
        omcVarChar = csVisible;
        xmlNodePtr pVisisble = xmlNewChild(pNodeColumn, NULL, BAD_CAST DEF_IS_VISIBLE,BAD_CAST omcVarChar);
        xmlAddChild(pNodeColumn, pVisisble);
    }

    free(pnOrderTemp);
    free(pnOrder);
    return true;
}

HRESULT CCommonStatistics::hSetConfigData(xmlNodePtr pDocPtr)
{
    xmlXPathObjectPtr pTempNode = NULL;
    switch (m_eBusType)
    {
        case CAN:
        {
            if(pDocPtr != NULL)
            {
                pTempNode = xmlUtils::pGetChildNodes(pDocPtr, (xmlChar*)("CAN_Statistics"));
                //pTempNode = xmlUtils::pGetNodes(pDocPtr, (xmlChar*)("CAN_Statistics"));
            }
        }
        break;
        case LIN:
        {
            if(pDocPtr != NULL)
            {
                pTempNode = xmlUtils::pGetChildNodes(pDocPtr, (xmlChar*)("LIN_Statistics"));
                //pTempNode = xmlUtils::pGetNodes(pDocPtr, (xmlChar*)("LIN_Statistics"));
            }
        }
        break;
        default:
            break;
    }

    if(pTempNode != NULL && pTempNode->nodesetval != NULL && pTempNode->nodesetval->nodeTab[0] != NULL)
    {
        m_pxmlNodePtr = xmlCopyNode(pTempNode->nodesetval->nodeTab[0], 1);
    }
    else
    {
        m_pxmlNodePtr = NULL;
    }

    // Sets the dialog configuration for CAN and LIN
    hSetConfigData();

    return S_OK;
}

void CCommonStatistics::vLoadDataFromStore(xmlNodePtr pNodePtr)
{
    if(sm_sBusSerializationData.m_bIsDirty)
    {
        hSetConfigData();
        sm_sBusSerializationData.m_bIsDirty = false;
    }
}

HRESULT CCommonStatistics::hSetConfigData()
{
    HRESULT nRetVal = S_FALSE;

    sm_sBusSerializationData.vLoadDefaults();

    if(m_omStatList.m_hWnd != NULL && m_pxmlNodePtr != NULL)
    {
        xmlXPathObjectPtr pObjectPath = NULL;
        nRetVal = S_OK;
        //xmlChar* pXmlPath = (xmlChar*) BUS_STATICS_CONFIG_PATH;

        pObjectPath = xmlUtils::pGetChildNodes(m_pxmlNodePtr, (xmlChar*) "COLUMN");

        if(pObjectPath != NULL)
        {
            xmlNodeSetPtr pNodeSet = pObjectPath->nodesetval;
            sm_sBusSerializationData.m_nColumnCount = pObjectPath->nodesetval->nodeNr;

            if(pNodeSet != NULL && (sm_sBusSerializationData.m_nColumnCount == m_nChannelCount + 1))
            {
                ColumnInfoMap ColumnMap;
                nRetVal = xmlUtils::parseColumnInfoNode(pNodeSet, ColumnMap);

                int* pnOrder = (int*) malloc((ColumnMap.size()+1)*sizeof(int));
                if(S_OK == nRetVal)
                {
                    for (int i = 0 ; i < ColumnMap.size(); i++)
                    {
                        xmlNodePtr pNodePtr = pNodeSet->nodeTab[i]->xmlChildrenNode;

                        while(pNodePtr != NULL)
                        {
                            CString strName = pNodePtr->name;
                            if( pNodePtr->xmlChildrenNode != NULL)
                            {
                                if(strName == "ID")
                                {
                                    xmlChar* ptext = xmlNodeListGetString(pNodePtr->doc, pNodePtr->xmlChildrenNode, 1);

                                    strName = ptext;

                                    ColumnInfoMap::iterator itr = ColumnMap.find(strName.GetBuffer(strName.GetLength()));

                                    if(itr != ColumnMap.end())
                                    {
                                        INT nOrder = itr->second.nOrder;
                                        pnOrder[i] = itr->second.nOrder;
                                        m_omStatList.MakeColumnVisible(i, itr->second.isVisble);
                                        m_omStatList.SetColumnWidth(i, itr->second.nWidth);
                                    }
                                    xmlFree(ptext);
                                    break;
                                }
                            }
                            pNodePtr = pNodePtr->next;
                        }
                    }

                    LPINT pnOrderTemp = (LPINT) malloc(ColumnMap.size()*sizeof(int));
                    for(INT nIndex = 0; nIndex < ColumnMap.size(); nIndex++)
                    {
                        INT nColumn = pnOrder[nIndex];
                        pnOrderTemp[nColumn] = nIndex;
                    }

                    m_omStatList.SetColumnOrderArray(ColumnMap.size(), pnOrderTemp);

                }

                if(pnOrder != NULL)
                {
                    free(pnOrder);
                    pnOrder = NULL;
                }

                m_pxmlNodePtr = NULL;
            }
            else
            {
                nRetVal = S_FALSE;
            }
        }
        else
        {
            nRetVal = S_FALSE;
        }
    }

    if(nRetVal == S_FALSE)
    {
        vLoadDefaultValues();
    }

    return S_OK;
}

///*******************************************************************************
//  Function Name  : vLoadDefaultValues
//  Input(s)       : -
//  Output         : -
//  Functionality  : Loads the Bus stats dialog Configuration defaults.
//  Member of      : CBusStatisticsDlg
//  Author(s)      : Arunkumar K
//  Date Created   : 07-04-2011
//  Modifications  :
//*******************************************************************************/
void CCommonStatistics::vLoadDefaultValues()
{
    sm_sBusSerializationData.vLoadDefaults();

    if(m_omStatList.m_hWnd != NULL)
    {
        for (int i = 0 ; i < m_nChannelCount+1; i++)
        {
            m_omStatList.MakeColumnVisible(i, sm_sBusSerializationData.m_arrbColumnVisible[i]);
            m_omStatList.SetColumnWidth(i, sm_sBusSerializationData.m_arrnColWidth[i]);
        }
        m_omStatList.SetColumnOrderArray(m_nChannelCount+1, (LPINT)sm_sBusSerializationData.m_arrnOrder);
    }
}

void CNetworkStatistics::vGetConfigData(xmlNodePtr& pxmlNodePtr)
{
    pxmlNodePtr = xmlNewNode(NULL, BAD_CAST DEF_BUS_STATS);

    xmlNodePtr pxmlBusStats = NULL;
    BOOL bIsStatCreated = FALSE;
    for(UINT unIndex = 0; unIndex < BUS_TOTAL; unIndex++)
    {
        if(m_omStatistics[unIndex] != NULL)
        {
            m_omStatistics[unIndex]->vGetConfigData(pxmlBusStats);
            xmlAddChild(pxmlNodePtr, pxmlBusStats);
        }
    }

    //xmlAddChild(pxmlNodePtr, pxmlBusStats);
    xmlNodePtr pxmlWndPos = NULL;
    vGetWndPosConfig(pxmlWndPos);
    xmlAddChild(pxmlNodePtr, pxmlWndPos);
}

void CNetworkStatistics::vGetWndPosConfig(xmlNodePtr& pxmlBusStats)
{
    pxmlBusStats = xmlNewNode(NULL, BAD_CAST DEF_WND_POS);
    wpWindowPosition();
    xmlUtils::CreateXMLNodeFrmWindowsPlacement(pxmlBusStats, wpWindowPosition());


}

void CNetworkStatistics::vSetNtwStatWndPos(xmlNodePtr pDocPtr, BOOL bShow)
{
    if ( NULL != pDocPtr )
    {
        xmlXPathObjectPtr pTempNode = NULL;
        pTempNode = xmlUtils::pGetChildNodes(pDocPtr, (xmlChar*)("Window_Position"));

        if(pTempNode != NULL && pTempNode->nodesetval != NULL && pTempNode->nodesetval->nodeTab[0] != NULL)
        {
            pDocPtr = xmlCopyNode(pTempNode->nodesetval->nodeTab[0], 1);
        }
    }
    if(pDocPtr == NULL)
    {
        vSetDefaultWindowPos();
        m_sBusStatsDlgCoOrd.showCmd = SW_SHOWNORMAL;
        if(bShow == FALSE)
        {
            m_sBusStatsDlgCoOrd.showCmd = SW_HIDE;
        }
        SetWindowPlacement(&m_sBusStatsDlgCoOrd);
    }
    else
    {
        xmlUtils::ParseWindowsPlacement(pDocPtr,m_sBusStatsDlgCoOrd);

        if(m_bIsWindowInitialised == TRUE)
        {
            m_sBusStatsDlgCoOrd.showCmd = SW_SHOWNORMAL;
            if(bShow == FALSE)
            {
                m_sBusStatsDlgCoOrd.showCmd = SW_HIDE;
            }
            SetWindowPlacement(&m_sBusStatsDlgCoOrd);
        }
    }
}

void CNetworkStatistics::vSetDefaultWindowPos()
{
    m_sBusStatsDlgCoOrd.showCmd = SW_SHOWNORMAL;
    m_sBusStatsDlgCoOrd.flags = WPF_RESTORETOMAXIMIZED;
    m_sBusStatsDlgCoOrd.rcNormalPosition.top = 149;
    m_sBusStatsDlgCoOrd.rcNormalPosition.left = 305;
    m_sBusStatsDlgCoOrd.rcNormalPosition.bottom = 813;
    m_sBusStatsDlgCoOrd.rcNormalPosition.right = 727;

    //SetWindowPlacement(&m_sBusStatsDlgCoOrd);
}


HRESULT CNetworkStatistics::hSetConfigData(xmlNodePtr pDocPtr, BOOL bShow)
{
    vSetNtwStatWndPos(pDocPtr, bShow);
    for(UINT unIndex = 0; unIndex < BUS_TOTAL; unIndex++)
    {
        if(m_omStatistics[unIndex] != NULL)
        {
            m_omStatistics[unIndex]->hSetConfigData(pDocPtr);
        }
    }


    return S_OK;
}

void CNetworkStatistics::OnClose()
{
    ShowWindow(SW_HIDE);
}

void CNetworkStatistics::vUpdateChannelInfo(ETYPE_BUS eBus,UINT unHardware)
{
    switch(eBus)
    {
        case CAN:
            m_omStatistics[CAN]->vUpdateChannelCountInfo(unHardware);
            break;
        case LIN:
            m_omStatistics[LIN]->vUpdateChannelCountInfo(unHardware);
            break;
    }
}

WINDOWPLACEMENT CNetworkStatistics::wpWindowPosition()
{
    if(m_bIsWindowInitialised == TRUE)
    {
        m_sBusStatsDlgCoOrd.length=sizeof(WINDOWPLACEMENT);
        GetWindowPlacement(&m_sBusStatsDlgCoOrd);

        return m_sBusStatsDlgCoOrd;
    }
    else
    {
        vSetDefaultWindowPos();
        return m_sBusStatsDlgCoOrd;
    }
}