#include "SignalWatch_stdafx.h"
#include "MsgSignalSelect.h"
#include "SignalWatch_resource.h"
#include "Application\HashDefines.h"

#define TREE_VIEW_CHECK_STATE_CHANGE (WM_USER + 100)
#define IDC_FILESYS_TREE 99999

// CTree dialog

IMPLEMENT_DYNAMIC(CMsgSignalSelect, CDialog)

CMsgSignalSelect::CMsgSignalSelect(IBMNetWorkGetService* cluster, ETYPE_BUS ouEbus, CWnd* pParent /*=nullptr*/, std::map<long, std::list<std::string>>* pMapMsgIDtoSignallst, CRITICAL_SECTION& omCritSecSW)
    : CDialog(CMsgSignalSelect::IDD, pParent), m_omCritSecSW(omCritSecSW)
{
    m_ouCluster = cluster;
    pTreeStruct = new CTreeCtrl;
    m_mapMsgIDtoSignallst = pMapMsgIDtoSignallst;
    m_ouEbus = ouEbus;
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
    //ON_MESSAGE(IDD_DLG_SIGNAL_WATCH_CONFIGURE,OnInitDialog)
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

void CMsgSignalSelect::vGetAutoResizeValues(double fSizeX, double fSizeY, double fToXPos, double fToYPos, _AUTORESIZE& autoResize)
{
    autoResize.fToXPos = fToXPos / 100.0;
    autoResize.fToYPos = fToYPos / 100.0;
    autoResize.fSizeX = fSizeX / 100.0;
    autoResize.fSizeY = fSizeY / 100.0;
}

void CMsgSignalSelect::vMoveWindowPos(int nResourceId, double fToXPos, double fToYPos, double fSizeX, double fSizeY)
{
    double dStandard = 100.0;
    _AUTORESIZE autoResize;
    GetDlgItem(nResourceId, &autoResize.wndHandle);
    if (autoResize.wndHandle != nullptr)
    {
        vGetAutoResizeValues(fSizeX, fSizeY, fToXPos, fToYPos, autoResize);
        ::GetWindowRect(autoResize.wndHandle, &autoResize.wndRect);
        ScreenToClient(autoResize.wndRect);
        m_VecAutoSize.push_back(autoResize);
    }
}

void CMsgSignalSelect::OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI)
{
    CDialog::OnGetMinMaxInfo(lpMMI);

    if (lpMMI->ptMinTrackSize.x < m_AllowedSize.cx)
    {
        lpMMI->ptMinTrackSize.x = m_AllowedSize.cx;
    }
    if (lpMMI->ptMinTrackSize.y < m_AllowedSize.cy)
    {
        lpMMI->ptMinTrackSize.y = m_AllowedSize.cy;
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
    if (m_omImageList.Create(IDR_BMP_MSG_SIG_WATCH, defICON_SIZE,
                             defICON_GROW, defCOLOR_WHITE) == TRUE)
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

    if (m_omImageList1.Create(defICON_SIZE, defICON_SIZE, ILC_COLOR32 | ILC_MASK, 5, 5) == TRUE)
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
    if ((m_AllowedSize.cx == 0) && (m_AllowedSize.cy == 0))
    {
        CRect rcWindow;
        GetWindowRect(rcWindow);
        m_AllowedSize = rcWindow.Size();
    }
    vMoveWindowPos(IDC_TREE_LIN, 0, 0, 100.0, 100.0);
    vMoveWindowPos(IDOK, 100.0, 100.0, 0.0, 0.0);
    vMoveWindowPos(IDCANCEL, 100.0, 100.0, 0.0, 0.0);
    // keep the initial size of the client area as a baseline for moving/sizing controls
    CRect rcClient;
    GetClientRect(rcClient);
    m_DefaultSize = rcClient.Size();

    pTreeStruct = (CTreeCtrl*)GetDlgItem(IDC_TREE_LIN);

    /* Remove the style */
    pTreeStruct->ModifyStyle(TVS_CHECKBOXES, 0);
    /* Now explicitly set it */
    pTreeStruct->ModifyStyle(0, TVS_CHECKBOXES);
    if (nullptr == pTreeStruct)
    {
        return FALSE;
    }

    HTREEITEM hNumofChannel = nullptr, hmsg, sig;
    std::string strName;
    int nChannels = 0;
    unsigned int unId;
    
    m_ouCluster->GetChannelCount(m_ouEbus, nChannels);

    char chText[255];
    for (int i = 0; i < nChannels; i++)
    {
        sprintf_s(chText, sizeof(chText), "Channel %d", i + 1);
        hNumofChannel = pTreeStruct->InsertItem(chText);
        pTreeStruct->SetItemImage(hNumofChannel, 4, 4);
        std::list<IFrame*> lstNumofFrames;
        m_ouCluster->GetFrameList(m_ouEbus, i, lstNumofFrames);
        {
            //pCluster->GetElementList(eFrameElement, lstNumofFrames);

            pTreeStruct->SetImageList(&m_omImageList, TVSIL_NORMAL);

            auto itr = lstNumofFrames.begin();
            while (itr != lstNumofFrames.end())
            {
                IFrame* pouFrame = *itr;

                pouFrame->GetFrameId(unId);
                
                pouFrame->GetName(strName);
                hmsg = pTreeStruct->InsertItem(strName.c_str(), hNumofChannel);
                pTreeStruct->SetItemImage(hmsg, 3, 3);

                long nSelkey = -1;
                long nKey = -1;

                nKey = unId;

                if (m_mapMsgIDtoSignallst[i].find(nKey) != m_mapMsgIDtoSignallst[i].end())
                {
                    nSelkey = m_mapMsgIDtoSignallst[i].find(nKey)->first;
                }

                if (nSelkey == nKey)
                {
                    pTreeStruct->SetCheck(hNumofChannel);
                    pTreeStruct->SetCheck(hmsg);
                }
                pTreeStruct->SetItemData(hmsg, nKey);

                std::map<ISignal*, SignalInstanse> ouSignalList;
                pouFrame->GetSignalList(ouSignalList);
                auto itr_sig = ouSignalList.begin();
                while (itr_sig != ouSignalList.end())
                {
                    itr_sig->first->GetName(strName);
                    sig = pTreeStruct->InsertItem(strName.c_str(), hmsg);

                    pTreeStruct->SetItemImage(sig, 0, 0);
                    if (nSelkey == nKey)
                    {
                        if (m_mapMsgIDtoSignallst[i].find(nKey) != m_mapMsgIDtoSignallst[i].end())
                        {
                            std::list<std::string> lstSignals = m_mapMsgIDtoSignallst[i].find(nKey)->second;
                            std::list<std::string>::iterator itrselSignals = lstSignals.begin();

                            while (itrselSignals != lstSignals.end())
                            {
                                std::string signame = pTreeStruct->GetItemText(sig);
                                if (_strcmpi(pTreeStruct->GetItemText(sig), (*itrselSignals).c_str()) == 0)
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
    }
    pTreeStruct->Expand(hNumofChannel, TVE_EXPAND);
    //vTest(pTreeStruct);
    return TRUE;

    // CTree message handlers
}

void CMsgSignalSelect::vStoreintoMap()
{
    EnterCriticalSection(&m_omCritSecSW);
    m_mapMsgIDtoSignallst->clear();
    HTREEITEM root = pTreeStruct->GetRootItem();

    if (root == nullptr)
    {
        LeaveCriticalSection(&m_omCritSecSW);
        return;
    }

    int i = 0;
    if (pTreeStruct->ItemHasChildren(root) && pTreeStruct->GetCheck(root))
    {
        HTREEITEM msgs = pTreeStruct->GetChildItem(root);

        while (msgs != nullptr)
        {
            CString omStrMsgs = pTreeStruct->GetItemText(msgs);
            if (pTreeStruct->GetCheck(msgs))
            {
                std::list<std::string> selSignalNames;

                HTREEITEM signalName = pTreeStruct->GetChildItem(msgs);
                long id = pTreeStruct->GetItemData(msgs);
                std::string sigName = pTreeStruct->GetItemText(signalName);

                if (signalName != nullptr && pTreeStruct->GetCheck(signalName))
                {
                    selSignalNames.push_front(sigName);
                }

                HTREEITEM htiSibling = pTreeStruct->GetNextSiblingItem(signalName);
                while (htiSibling)
                {
                    if (pTreeStruct->GetCheck(htiSibling))
                    {
                        std::string sigName = pTreeStruct->GetItemText(htiSibling);
                        selSignalNames.push_back(sigName);
                        htiSibling = pTreeStruct->GetNextSiblingItem(htiSibling);
                    }
                    else
                    {
                        htiSibling = pTreeStruct->GetNextSiblingItem(htiSibling);
                    }
                }

                m_mapMsgIDtoSignallst[i].insert(std::map<long, std::list<std::string>>::value_type(id, selSignalNames));
            }

            msgs = pTreeStruct->GetNextSiblingItem(msgs);

            if (msgs == nullptr)
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
    LeaveCriticalSection(&m_omCritSecSW);
}

UINT CMsgSignalSelect::vChangeWindowPos(HDWP& handleWP, HWND /*wndHandle*/, double fSizeX, double fSizeY, CRect /*rectWndPos*/)
{
    if (handleWP == NULL)
    {
        handleWP = BeginDeferWindowPos(m_VecAutoSize.size());
    }
    UINT uFlags = SWP_NOACTIVATE | SWP_NOOWNERZORDER | SWP_NOZORDER;
    if ((fSizeX != 0.0) || (fSizeY != 0.0))
    {
        uFlags |= SWP_NOCOPYBITS;
    }

    return uFlags;
}

HDWP CMsgSignalSelect::vUpdateWindowPosition(int nXPos, int nYPos)
{
    HDWP handleWP = NULL;

    for (VecAutoSize::iterator itr = m_VecAutoSize.begin(); itr != m_VecAutoSize.end(); ++itr)
    {
        if (itr->wndHandle != NULL)
        {
            CRect rectWndPos(itr->wndRect);
            int nCalXPos = int(nXPos * itr->fToXPos);
            int nCalYPos = int(nYPos * itr->fToYPos);

            int nCalRightPos = int(nXPos * itr->fSizeX);
            int nCalBottomPos = int(nYPos * itr->fSizeY);

            rectWndPos.OffsetRect(nCalXPos, nCalYPos);
            rectWndPos.right = rectWndPos.right + nCalRightPos;
            rectWndPos.bottom = rectWndPos.bottom + nCalBottomPos;
            UINT uFlags = vChangeWindowPos(handleWP, itr->wndHandle, itr->fSizeX, itr->fSizeY, rectWndPos);

            DeferWindowPos(handleWP, itr->wndHandle, NULL, rectWndPos.left, rectWndPos.top, rectWndPos.Width(), rectWndPos.Height(), uFlags);
        }
    }

    return handleWP;
}

void CMsgSignalSelect::OnSize(UINT nType, int cx, int cy)
{
    CDialog::OnSize(nType, cx, cy);

    int nXPos = cx - m_DefaultSize.cx;
    int nYPos = cy - m_DefaultSize.cy;
    HDWP hDefer = NULL;

    hDefer = vUpdateWindowPosition(nXPos, nYPos);

    if (hDefer != NULL)
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