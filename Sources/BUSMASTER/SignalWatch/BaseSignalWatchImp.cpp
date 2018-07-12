#include "SignalWatch_stdafx.h"
#include "BaseSignalWatchImp.h"

#include "SignalWatch_resource.h"
#include "MsgSignalSelect.h"
#include "SignalWatchDefs.h"
CBaseSignalWatchImp::CBaseSignalWatchImp(ETYPE_BUS busType)
{
    mBusType = busType;
    mDbCluster = nullptr;
    m_pouSigWnd = nullptr;
}

CBaseSignalWatchImp::~CBaseSignalWatchImp()
{
    DeleteCriticalSection(&m_omCritSecSW); //delete critical section

    if ( m_pouSigWnd != nullptr)
    {
        delete m_pouSigWnd;
        m_pouSigWnd = nullptr;
    }
}

HRESULT CBaseSignalWatchImp::SW_DoInitialization(IBMNetWorkGetService* dbCluster)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());

    InitializeCriticalSection(&m_omCritSecSW);
    //m_pouSigWnd = nullptr;

    mDbCluster = (IBMNetWorkGetService*)dbCluster;
    if (m_pouSigWnd == nullptr)
    {
        m_pouSigWnd = new CSigWatchDlg(this, AfxGetMainWnd(), mBusType);
        m_pouSigWnd->Create(IDD_DLG_SIGNAL_WATCH, nullptr);
        m_pouSigWnd->SetWindowText("Signal Watch");
    }
    DoInitialization();
    return StartSigWatchReadThread() ? S_OK : S_FALSE;
}
HRESULT CBaseSignalWatchImp::SW_ShowAddDelSignalsDlg(CWnd* pParent, void* /*dbCluster*/)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());
    //mDbCluster = (IBMNetWorkGetService*)dbCluster;

    //cluster[LIN].m_nChannelsConfigured;
    CMsgSignalSelect odDlg(mDbCluster, mBusType, pParent, m_mapMsgIDtoSignallst, m_omCritSecSW);

    //WndPlace=odDlg.wpPosition();

    int nRet = odDlg.DoModal();
    return (HRESULT)nRet;
}
HRESULT CBaseSignalWatchImp::SW_ShowSigWatchWnd(CWnd* /*pParent*/, HWND hMainWnd, INT nCmd)
{
    if (nullptr == m_pouSigWnd)
    {
        SW_DoInitialization(mDbCluster);
    }
    if (m_pouSigWnd != nullptr)
    {
        m_pouSigWnd->vUpdateMainWndHandle(hMainWnd);
        m_pouSigWnd->SetParent(CWnd::FromHandle(hMainWnd));
        return m_pouSigWnd->ShowWindow(nCmd);
    }
    return S_FALSE;
}
HRESULT CBaseSignalWatchImp::SW_GetConfigData(xmlNodePtr pNodePtr)
{
    std::map<UID_ELEMENT, IElement*> lstNumofFrames;
    if (nullptr != mDbCluster)
    {
        int nChannelConfig;
        unsigned int unId = 0xFF;
        mDbCluster->GetChannelCount(mBusType, nChannelConfig);
        FrameProps ouFrameProps;
        for (int i = 0; i < nChannelConfig; i++)
        {
            ICluster* pDbService;

			int nDBCount = 0;
			mDbCluster->GetDBServiceCount(mBusType, i, nDBCount);

			for (auto nDBIndex = 0; nDBIndex < nDBCount; nDBIndex++)
			{
				mDbCluster->GetDBService(mBusType, i, nDBIndex, &pDbService);
				if (pDbService == nullptr)
				{
					continue;
				}
				pDbService->GetElementList(eFrameElement, lstNumofFrames);
				auto itr = lstNumofFrames.begin();
				while (itr != lstNumofFrames.end())
				{
					int selkey = -1;
					((IFrame*)itr->second)->GetFrameId(unId);
					if (m_mapMsgIDtoSignallst->find(unId) != m_mapMsgIDtoSignallst->end())
					{
						selkey = m_mapMsgIDtoSignallst->find(unId)->first;
					}
					CString strMessageId = "";
					if (selkey == unId)
					{
						xmlNodePtr pMsgTagPtr = xmlNewNode(nullptr, BAD_CAST DEF_MESSAGE);
						xmlAddChild(pNodePtr, pMsgTagPtr);
						strMessageId.Format("%d", selkey);

						/* Generating Message names */
						xmlNodePtr pMsgPtr = xmlNewChild(pMsgTagPtr, nullptr, BAD_CAST DEF_MSGID
							, BAD_CAST strMessageId.GetBufferSetLength(strMessageId.GetLength()));
						xmlAddChild(pMsgTagPtr, pMsgPtr);
						CString strSignalName = "";
						if (selkey == unId)
						{
							std::list<std::string> lstSignals = m_mapMsgIDtoSignallst->find(unId)->second;
							std::list<std::string>::iterator itrselSignals = lstSignals.begin();

							while (itrselSignals != lstSignals.end())
							{
								strSignalName = (*itrselSignals).c_str();
								// strSignalName.Format("%s", *itrselSignals);

								xmlNodePtr pSignalPtr = xmlNewChild(pMsgTagPtr, nullptr, BAD_CAST DEF_SIGNAL
									, BAD_CAST strSignalName.GetBufferSetLength(strSignalName.GetLength()));
								xmlAddChild(pMsgTagPtr, pSignalPtr);
								itrselSignals++;
							}
						}
					}
					itr++;
				}
			}
        }
    }
    // Setting signal watch window placement and column width
    xmlNodePtr pWndPositn = xmlNewNode(nullptr, BAD_CAST DEF_WINDOW_POSITION);
    xmlAddChild(pNodePtr, pWndPositn);
    WINDOWPLACEMENT WndPlace;
    UINT nDebugSize = 0;
    //BYTE* pbyTemp = (BYTE*)pbyConfigData;
    if ((m_pouSigWnd != nullptr))
    {
        m_pouSigWnd->GetWindowPlacement(&WndPlace);
        //COPY_DATA(pbyTemp, &WndPlace, sizeof (WINDOWPLACEMENT));

        BOOL bIsWndwVisible = m_pouSigWnd->IsWindowVisible();

        CString strWindwVisibility = "";

        strWindwVisibility = xmlUtils::nSetWindowVisibility(WndPlace.showCmd);

        // Writing visibility in to xml
        xmlNodePtr pVisibility = xmlNewChild(pWndPositn, nullptr, BAD_CAST DEF_VISIBILITY, BAD_CAST strWindwVisibility.GetBuffer(strWindwVisibility.GetLength()));
        xmlAddChild(pWndPositn, pVisibility);

        UINT nFlag = WndPlace.flags;

        CString strWindPlcmnt = "";

        strWindPlcmnt = xmlUtils::nSetWindowVisibility(nFlag);

        // Setting window Placement
        xmlNodePtr pWndwVisibility = xmlNewChild(pWndPositn, nullptr, BAD_CAST DEF_WINDOW_PLACEMENT, BAD_CAST strWindPlcmnt.GetBuffer(strWindPlcmnt.GetLength()));
        xmlAddChild(pWndPositn, pWndwVisibility);

        CString strBottomPos = "", strLeftPos = "", strTopPos = "", strRightPos = "";

        strTopPos.Format("%d", WndPlace.rcNormalPosition.top);
        strLeftPos.Format("%d", WndPlace.rcNormalPosition.left);
        strRightPos.Format("%d", WndPlace.rcNormalPosition.right);
        strBottomPos.Format("%d", WndPlace.rcNormalPosition.bottom);

        // Writing co-ordinates of the window in to XML file
        xmlNodePtr pTopPos = xmlNewChild(pWndPositn, nullptr, BAD_CAST DEF_TOP, BAD_CAST strTopPos.GetBuffer(strTopPos.GetLength()));
        xmlAddChild(pWndPositn, pTopPos);

        xmlNodePtr pLeftPos = xmlNewChild(pWndPositn, nullptr, BAD_CAST DEF_Left, BAD_CAST strLeftPos.GetBuffer(strLeftPos.GetLength()));
        xmlAddChild(pWndPositn, pLeftPos);

        xmlNodePtr pRightPos = xmlNewChild(pWndPositn, nullptr, BAD_CAST DEF_Right, BAD_CAST strRightPos.GetBuffer(strRightPos.GetLength()));
        xmlAddChild(pWndPositn, pRightPos);

        xmlNodePtr pBottomPos = xmlNewChild(pWndPositn, nullptr, BAD_CAST DEF_Bottom, BAD_CAST strBottomPos.GetBuffer(strBottomPos.GetLength()));
        xmlAddChild(pWndPositn, pBottomPos);

        // Setting Column width in to the xml file
        xmlNodePtr pColWidthPtr = xmlNewNode(nullptr, BAD_CAST DEF_Columns_Width);
        xmlAddChild(pNodePtr, pColWidthPtr);

        for (UINT i = 0; i < defSW_LIST_COLUMN_COUNT; i++)
        {
            INT nWidth = m_pouSigWnd->m_omSignalList.GetColumnWidth(i);
            CString strWidth = "";
            strWidth.Format("%d", nWidth);

            if (i == 0)
            {
                // Writing message column value in to xml
                xmlNodePtr pMsgClmnPtr = xmlNewChild(pColWidthPtr, nullptr, BAD_CAST DEF_Message_Column, BAD_CAST strWidth.GetBuffer(strWidth.GetLength()));
                xmlAddChild(pColWidthPtr, pMsgClmnPtr);
            }
            if (i == 1)
            {
                // Writing Raw value column value in to xml
                xmlNodePtr pRawValClmn = xmlNewChild(pColWidthPtr, nullptr, BAD_CAST DEF_Raw_Val_Column, BAD_CAST strWidth.GetBuffer(strWidth.GetLength()));
                xmlAddChild(pColWidthPtr, pRawValClmn);
            }
            if (i == 2)
            {
                // Writing Raw value column value in to xml
                xmlNodePtr pPhyalClmn = xmlNewChild(pColWidthPtr, nullptr, BAD_CAST DEF_Physical_Val_Column, BAD_CAST strWidth.GetBuffer(strWidth.GetLength()));
                xmlAddChild(pColWidthPtr, pPhyalClmn);
            }
            if (i == 3)
            {
                // Writing Raw value column value in to xml
                xmlNodePtr pSigValClmn = xmlNewChild(pColWidthPtr, nullptr, BAD_CAST DEF_Signal_Column, BAD_CAST strWidth.GetBuffer(strWidth.GetLength()));
                xmlAddChild(pColWidthPtr, pSigValClmn);
            }
        }
    }
    return S_OK;
}
HRESULT CBaseSignalWatchImp::SW_SetConfigData(xmlNodePtr pNode)
{
    m_pouSigWnd->ShowWindow(SW_HIDE);
    INT nRetValue = S_OK;
    if ((pNode != nullptr) && (m_pouSigWnd != nullptr))
    {
		m_mapMsgIDtoSignallst->clear();
        WINDOWPLACEMENT WndPlace;
        while (pNode != nullptr)
        {
            if ((!xmlStrcmp(pNode->name, (const xmlChar*)"Message")))
            {
                int id = 0;
                std::list<std::string> signame;

                xmlNodePtr child = pNode->children;
                while (child != nullptr)
                {
                    if ((!xmlStrcmp(child->name, (const xmlChar*)"Id")))
                    {
                        xmlChar* key = xmlNodeListGetString(child->doc, child->xmlChildrenNode, 1);
                        if (nullptr != key)
                        {
                            id = atoi((char*)key);
                        }
                    }

                    if ((!xmlStrcmp(child->name, (const xmlChar*)"Signal")))
                    {
                        xmlChar* key = xmlNodeListGetString(child->doc, child->xmlChildrenNode, 1);
                        if (nullptr != key)
                        {
                            signame.push_back((char*)key);
                        }
                    }

                    child = child->next;
                }

				m_mapMsgIDtoSignallst[0].insert(std::map<long, std::list<std::string>>::value_type(id, signame));
            }

            if ((!xmlStrcmp(pNode->name, (const xmlChar*)"Window_Position")))
            {
                nRetValue = xmlUtils::ParseWindowsPlacement(pNode, WndPlace);
                if (nRetValue == S_OK)
                {
                    m_pouSigWnd->MoveWindow(&(WndPlace.rcNormalPosition), FALSE);
                }
                break;
            }

            pNode = pNode->next;
        }
    }
    if (m_pouSigWnd != S_OK)
    {
        //Signal watch window will move the List control in OnSize().
        //So the default values should be as followes.
        for (UINT i = 0; i < defSW_LIST_COLUMN_COUNT; i++)
        {
            RECT sClientRect;
            m_pouSigWnd->GetClientRect(&sClientRect);
            int ClientWidth = abs(sClientRect.left - sClientRect.right);
            m_pouSigWnd->m_omSignalList.SetColumnWidth(0, (int)(0.2 * ClientWidth));
            m_pouSigWnd->m_omSignalList.SetColumnWidth(1, (int)(0.2 * ClientWidth));
            m_pouSigWnd->m_omSignalList.SetColumnWidth(2, (int)(0.4 * ClientWidth));
            m_pouSigWnd->m_omSignalList.SetColumnWidth(3, (int)(0.2 * ClientWidth));
            m_pouSigWnd->m_omSignalList.MoveWindow(&sClientRect);
        }
    }
    if (pNode == nullptr)
    {
        WINDOWPLACEMENT WndPlace;
        m_mapMsgIDtoSignallst->clear();
        //     // memcpy(&WndPlace, pbyConfigData, sizeof (WINDOWPLACEMENT));
        //      m_pouSigWnd->MoveWindow(&(WndPlace.rcNormalPosition), FALSE);
        //Signal watch window will move the List control in OnSize().
        //So the default values should be as followes.

        WndPlace.rcNormalPosition.top = 70;
        WndPlace.rcNormalPosition.left = 10;
        WndPlace.rcNormalPosition.right = 500;
        WndPlace.rcNormalPosition.bottom = 300;
        m_pouSigWnd->MoveWindow(&(WndPlace.rcNormalPosition), TRUE);

        m_pouSigWnd->ShowWindow(SW_HIDE);
    }

    return S_OK;
}
HRESULT CBaseSignalWatchImp::SW_ClearSigWatchWnd(void)
{
    if (m_pouSigWnd != nullptr)
    {
        m_pouSigWnd->PostMessage(WM_REMOVE_SIGNAL, 0, 0);
    }
    return S_OK;
}
BOOL CBaseSignalWatchImp::SW_IsWindowVisible(void)
{
    BOOL bResult = FALSE;
    if (m_pouSigWnd != nullptr && m_pouSigWnd->GetSafeHwnd()!=nullptr)
    {
        bResult = m_pouSigWnd->IsWindowVisible();
    }
    return bResult;
}
HRESULT CBaseSignalWatchImp::SW_SetDisplayMode(BOOL bHex)
{
    m_bHex = (bHex!=0);
    return S_OK;
}
HRESULT CBaseSignalWatchImp::SW_SetClusterInfo(void* ouCluster)
{
    mDbCluster = (IBMNetWorkGetService*)ouCluster;
    return S_FALSE;
}