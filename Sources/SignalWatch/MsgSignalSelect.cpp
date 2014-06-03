#include "SignalWatch_stdafx.h"
#include "MsgSignalSelect.h"
#include "SignalWatch_resource.h"
#include "Application\HashDefines.h"

#define TREE_VIEW_CHECK_STATE_CHANGE (WM_USER + 100)
#define IDC_FILESYS_TREE 99999

// CTree dialog

IMPLEMENT_DYNAMIC(CMsgSignalSelect, CDialog)

CMsgSignalSelect::CMsgSignalSelect(ClusterConfig* cluster, CWnd* pParent /*=nullptr*/, std::map<int, std::list<std::string>>* pMapMsgIDtoSignallst)
    : CDialog(CMsgSignalSelect::IDD,pParent)
{
    m_ouCluster=*cluster;

    pTreeStruct = new CTreeCtrl;
    m_mapMsgIDtoSignallst = pMapMsgIDtoSignallst;
}

CMsgSignalSelect::~CMsgSignalSelect()
{
}

void CMsgSignalSelect::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_TREE_LIN, m_omMsgSignalTree);
}


BEGIN_MESSAGE_MAP(CMsgSignalSelect, CDialog)
    //ON_NOTIFY(TVN_ITEMCHANGED, IDC_TREE_LIN, OnClickTree)

    ON_WM_SIZE()
    ON_MESSAGE(IDD_DLG_SIGNAL_WATCH_LIN,OnInitDialog)
    //ON_WM_GETMINMAXINFO()
    ON_BN_CLICKED(IDOK, OnBnClickedOk)
END_MESSAGE_MAP()




void CMsgSignalSelect::OnClickTree(NMHDR* pNMHDR, LRESULT* pResult)
{
    NMTVITEMCHANGE* pNMTVItemChange = reinterpret_cast<NMTVITEMCHANGE*>(pNMHDR);

    *pResult = 0;
    // TODO: Add your control notification handler code here
    *pResult = 0;
}

void CMsgSignalSelect::AutoMove(int iID, double dXMovePct, double dYMovePct, double dXSizePct, double dYSizePct)
{
    ASSERT((dXMovePct + dXSizePct) <= 100.0);   // can't use more than 100% of the resize for the child
    ASSERT((dYMovePct + dYSizePct) <= 100.0);   // can't use more than 100% of the resize for the child
    SMovingChild s;
    GetDlgItem(iID, &s.m_hWnd);
    ASSERT(s.m_hWnd != nullptr);
    s.m_dXMoveFrac = dXMovePct / 100.0;
    s.m_dYMoveFrac = dYMovePct / 100.0;
    s.m_dXSizeFrac = dXSizePct / 100.0;
    s.m_dYSizeFrac = dYSizePct / 100.0;
    ::GetWindowRect(s.m_hWnd, &s.m_rcInitial);
    ScreenToClient(s.m_rcInitial);
    m_MovingChildren.push_back(s);
}

void CMsgSignalSelect::OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI)
{
    CDialog::OnGetMinMaxInfo(lpMMI);

    if (lpMMI->ptMinTrackSize.x < m_szMinimum.cx)
    {
        lpMMI->ptMinTrackSize.x = m_szMinimum.cx;
    }
    if (lpMMI->ptMinTrackSize.y < m_szMinimum.cy)
    {
        lpMMI->ptMinTrackSize.y = m_szMinimum.cy;
    }
}

/**
 * \return TRUE if success, FALSE otherwise
 *
 * To create image lists used in the dialog
 */
BOOL CMsgSignalSelect::bCreateImageList()
{
    // Flag to indicate result
    BOOL bReturn = FALSE;
    // Create Image List
    if( m_omImageList.Create( IDR_BMP_MSG_SIG_WATCH, defICON_SIZE,
                              defICON_GROW, defCOLOR_WHITE ) == TRUE )
    {
        // If successful set the result
        bReturn = TRUE;
    }
    // Return the result
    return bReturn;
}

BOOL CMsgSignalSelect::bCreateMsgImageList()
{



    // Flag to indicate result
    BOOL bReturn = FALSE;
    // Create Image List


    if( m_omImageList1.Create( defICON_SIZE, defICON_SIZE, ILC_COLOR32|ILC_MASK, 5, 5) == TRUE )
    {
        // If successful set the result
        bReturn = TRUE;
    }
    // Return the result
    return bReturn;
}

BOOL CMsgSignalSelect::OnInitDialog()
{
    CDialog::OnInitDialog();
    //m_omImageList1 = new CImageList();
    bCreateImageList();
    // use the initial dialog size as the default minimum
    if ((m_szMinimum.cx == 0) && (m_szMinimum.cy == 0))
    {
        CRect rcWindow;
        GetWindowRect(rcWindow);
        m_szMinimum = rcWindow.Size();
    }
    AutoMove(IDC_TREE_LIN, 0, 0, 100.0, 100.0);
    AutoMove(IDOK, 100.0, 100.0, 0.0, 0.0);
    AutoMove(IDCANCEL, 100.0, 100.0, 0.0, 0.0);
    // keep the initial size of the client area as a baseline for moving/sizing controls
    CRect rcClient;
    GetClientRect(rcClient);
    m_szInitial = rcClient.Size();

    pTreeStruct = (CTreeCtrl*)GetDlgItem(IDC_TREE_LIN);

    /* Remove the style */
    pTreeStruct->ModifyStyle( TVS_CHECKBOXES, 0 );
    /* Now explicitly set it */
    pTreeStruct->ModifyStyle( 0, TVS_CHECKBOXES );
    if( nullptr == pTreeStruct )
    {

        return FALSE;
    }

    HTREEITEM hNumofChannel,hmsg,sig;
    for(int i=0; i < m_ouCluster.m_nChannelsConfigured; i++)
    {
        hNumofChannel=pTreeStruct->InsertItem("Channel 1");
        pTreeStruct->SetItemImage(hNumofChannel, 4, 4);
        std::list<FRAME_STRUCT> lstNumofFrames;
        m_ouCluster.m_ouFlexChannelConfig[0].m_ouClusterInfo.GetFrames(lstNumofFrames);
        pTreeStruct->SetImageList(&m_omImageList, TVSIL_NORMAL);

        std::list<FRAME_STRUCT>::iterator itr= lstNumofFrames.begin();
        while(itr != lstNumofFrames.end())
        {
            hmsg=pTreeStruct->InsertItem(itr->m_strFrameName.c_str(),hNumofChannel);
            pTreeStruct->SetItemImage(hmsg, 3, 3);

            int selkey = -1;
            if ( m_mapMsgIDtoSignallst[i].find(itr->m_nSlotId) != m_mapMsgIDtoSignallst[i].end() )
            {
                selkey=m_mapMsgIDtoSignallst[i].find(itr->m_nSlotId)->first;
            }

            if(selkey == itr->m_nSlotId)
            {
                pTreeStruct->SetCheck(hNumofChannel);
                pTreeStruct->SetCheck(hmsg);
            }

            pTreeStruct->SetItemData(hmsg,itr->m_nSlotId);
            std::list<SIGNAL_STRUCT> ouSignalList;
            itr->GetSignalList(ouSignalList);
            std::list<SIGNAL_STRUCT>::iterator itr_sig=ouSignalList.begin();
            while(itr_sig != ouSignalList.end())
            {
                sig=pTreeStruct->InsertItem(itr_sig->m_strSignalName.c_str(),hmsg);

                pTreeStruct->SetItemImage(sig, 0, 0);
                if(selkey == itr->m_nSlotId)
                {
                    if ( m_mapMsgIDtoSignallst[i].find(itr->m_nSlotId) != m_mapMsgIDtoSignallst[i].end() )
                    {
                        std::list<std::string> lstSignals=m_mapMsgIDtoSignallst[i].find(itr->m_nSlotId)->second;
                        std::list<std::string>::iterator itrselSignals= lstSignals.begin();

                        while(itrselSignals != lstSignals.end())
                        {
                            std::string signame=pTreeStruct->GetItemText(sig);
                            if(strcmpi(pTreeStruct->GetItemText(sig),(*itrselSignals).c_str())== 0)
                            {
                                pTreeStruct->SetCheck(sig);
                            }
                            itrselSignals++;
                        }
                    }
                }

                itr_sig++;
            }

            itr++;
        }

    }
    pTreeStruct->Expand(hNumofChannel,TVE_EXPAND);
    //vTest(pTreeStruct);
    return TRUE;

    // CTree message handlers
}

void CMsgSignalSelect::vStoreintoMap()
{
    m_mapMsgIDtoSignallst->clear();
    HTREEITEM root = pTreeStruct->GetRootItem();

    if(root == nullptr)
    {
        return;
    }

    //MessageBox( "BEFORE IF", "IF1", MB_OK);
    int i=0;
    if(pTreeStruct->ItemHasChildren(root) && pTreeStruct->GetCheck(root) )
    {

        HTREEITEM msgs= pTreeStruct->GetChildItem(root);

        while(pTreeStruct->ItemHasChildren(msgs))
        {
            //MessageBox( "while", "wh1", MB_OK);
            if(pTreeStruct->GetCheck(msgs))
            {

                std::list<std::string> selSignalNames;

                HTREEITEM signalName= pTreeStruct->GetChildItem(msgs);
                int id= pTreeStruct->GetItemData(msgs);
                std::string sigName=pTreeStruct->GetItemText(signalName);

                if(pTreeStruct->GetCheck(signalName))
                {
                    selSignalNames.push_front(sigName);
                }
                //m_mapMsgIDtoSignallst[0].insert(map<int,list<string>>::value_type(id,sigName));
                HTREEITEM htiSibling = pTreeStruct->GetNextSiblingItem(signalName);
                while (htiSibling )
                {
                    //MessageBox( "while_child", "wh2", MB_OK);
                    if(pTreeStruct->GetCheck(htiSibling))
                    {
                        int id= pTreeStruct->GetItemData(msgs);
                        std::string sigName=pTreeStruct->GetItemText(htiSibling);
                        //m_mapMsgIDtoSignallst[0].insert(map<int,list<string>>::value_type(id,sigName));

                        selSignalNames.push_back(sigName);
                        htiSibling = pTreeStruct->GetNextSiblingItem(htiSibling);
                    }
                    else
                    {
                        htiSibling = pTreeStruct->GetNextSiblingItem(htiSibling);
                    }
                }

                m_mapMsgIDtoSignallst[i].insert(std::map<int, std::list<std::string>>::value_type(id,selSignalNames));

            }

            msgs= pTreeStruct->GetNextSiblingItem(msgs);

            if(msgs == nullptr)
            {
                break;

            }
        }

        i++;
    }
    else
    {
        m_mapMsgIDtoSignallst[i].clear();
    }

}


void CMsgSignalSelect::OnSize(UINT nType, int cx, int cy)
{
    CDialog::OnSize(nType, cx, cy);

    int iXDelta = cx - m_szInitial.cx;
    int iYDelta = cy - m_szInitial.cy;
    HDWP hDefer = nullptr;
    for (MovingChildren::iterator p = m_MovingChildren.begin();  p != m_MovingChildren.end();  ++p)
    {
        if (p->m_hWnd != nullptr)
        {
            CRect rcNew(p->m_rcInitial);
            rcNew.OffsetRect(int(iXDelta * p->m_dXMoveFrac), int(iYDelta * p->m_dYMoveFrac));
            rcNew.right += int(iXDelta * p->m_dXSizeFrac);
            rcNew.bottom += int(iYDelta * p->m_dYSizeFrac);
            if (hDefer == nullptr)
            {
                hDefer = BeginDeferWindowPos(m_MovingChildren.size());
            }
            UINT uFlags = SWP_NOACTIVATE | SWP_NOOWNERZORDER | SWP_NOZORDER;
            if ((p->m_dXSizeFrac != 0.0) || (p->m_dYSizeFrac != 0.0))
            {
                uFlags |= SWP_NOCOPYBITS;
            }
            DeferWindowPos(hDefer, p->m_hWnd, nullptr, rcNew.left, rcNew.top, rcNew.Width(), rcNew.Height(), uFlags);
        }
    }
    if (hDefer != nullptr)
    {
        EndDeferWindowPos(hDefer);
    }

    //if (m_hGripper != nullptr)
    //   ::ShowWindow(m_hGripper, (nType == SIZE_MAXIMIZED) ? SW_HIDE : SW_SHOW);
}


void CMsgSignalSelect::OnBnClickedOk()
{
    vStoreintoMap();
    OnOK();

}

//map<int,list<string>>* CMsgSignalSelect::vGetMapValue()
//{
//  return m_mapMsgIDtoSignallst;
//
//}


