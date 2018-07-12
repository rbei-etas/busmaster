#include "FrameProcessor_stdafx.h"
#include "ConfigMsgLogLINDlg.h"



#include "stdafx.h"
#include "ConfigMsgLogCANDlg.h"
#include "ProtocolsDefinitions.h"
#include "Utility\MultiLanguageSupport.h"
#include "Filter/Filter_extern.h"
#include "BaseDIL_LIN.h"
#include "DataTypes\Filter_Datatypes.h"
#define BUSMASTER_LIN_LOGFILENAME     "BUSMASTERLogFile_LIN"

static void vPopulateFilterApplied(const SFILTERAPPLIED_LIN* psFilterConfigured, SFILTERAPPLIED_LIN& sFilterApplied, CMainEntryList& SrcList)
{
    SMAINENTRY& sMainEntry = SrcList.GetHead();
    int nCount = sMainEntry.m_odSelEntryList.GetCount();
    sFilterApplied.vClear();
    sFilterApplied.m_psFilters = new SFILTERSET[nCount];

    POSITION pos = sMainEntry.m_odSelEntryList.GetHeadPosition();
    while (pos)
    {
        SSUBENTRY& sSubEntry = sMainEntry.m_odSelEntryList.GetNext(pos);
        const PSFILTERSET psTemp = SFILTERSET::psGetFilterSetPointer(psFilterConfigured->m_psFilters,
                                   psFilterConfigured->m_ushTotal, sSubEntry.m_omSubEntryName.GetBuffer(MAX_PATH));
        if (psTemp != nullptr)
        {
            sFilterApplied.m_psFilters[sFilterApplied.m_ushTotal].bClone(*psTemp);
            sFilterApplied.m_ushTotal++;
        }
    }
}


static void vPopulateMainSubList(CMainEntryList& DestList, const SFILTERAPPLIED_LIN* psFilterConfigured,
                                 const SFILTERAPPLIED_LIN* psFilterApplied)
{
    ASSERT(psFilterConfigured != nullptr);
    DestList.RemoveAll();

    SMAINENTRY sMainEntry;
    sMainEntry.m_omMainEntryName = "LIN";
    if (psFilterApplied == nullptr)
    {
        SMAINENTRY sMainEntry;
        sMainEntry.m_omMainEntryName = "FILTER_SELECTION_LIN";
        for (INT i = 0; i < psFilterConfigured->m_ushTotal; i++)
        {
            SSUBENTRY sSubEntry;
            sSubEntry.m_omSubEntryName.Format("%s",
                                              psFilterConfigured->m_psFilters[i].m_sFilterName.m_acFilterName.c_str());
            sMainEntry.m_odUnSelEntryList.AddTail(sSubEntry);
        }
    }
    else
    {

        for (INT i = 0; i < psFilterConfigured->m_ushTotal; i++)
        {
            SSUBENTRY sSubEntry;
            sSubEntry.m_omSubEntryName.Format("%s",
                                              psFilterConfigured->m_psFilters[i].m_sFilterName.m_acFilterName.c_str());
            if (SFILTERSET::psGetFilterSetPointer(psFilterApplied->m_psFilters,
                                                  psFilterApplied->m_ushTotal,
                                                  sSubEntry.m_omSubEntryName.GetBuffer(MAX_PATH)) != nullptr)
            {
                sMainEntry.m_odSelEntryList.AddTail(sSubEntry);
            }
            else
            {
                sMainEntry.m_odUnSelEntryList.AddTail(sSubEntry);
            }
        }
    }
    DestList.AddTail(sMainEntry);
}

CConfigMsgLogLINDlg::CConfigMsgLogLINDlg(CBaseFrameProcessor_LIN* pouBaseLogger, BOOL& bLogOnConnect, CWnd* pParent, const SFILTERAPPLIED_LIN* psFilter)
    :CConfigMsgLogDlg(pouBaseLogger, bLogOnConnect, pParent)

{
    m_psFilterConfigured = psFilter;
    mLinFrameprocessor = pouBaseLogger;
    m_strCurrWndText = _("Configure Logging for LIN");
    m_omControlParam = _("Start on Reception of ID 0x");
    m_omControlParam2 = _("Stop on Reception of ID 0x");
}


CConfigMsgLogLINDlg::~CConfigMsgLogLINDlg()
{
}


std::string CConfigMsgLogLINDlg::GetDefaultLogFileName()
{
    return BUSMASTER_LIN_LOGFILENAME;
}
HRESULT CConfigMsgLogLINDlg::HandleFilterSettings()
{
    CMainEntryList DestList;
    SFILTERAPPLIED_LIN sFilterApplied;
    mLinFrameprocessor->FPL_GetFilteringScheme((USHORT)m_nLogIndexSel, sFilterApplied);
    vPopulateMainSubList(DestList, m_psFilterConfigured, &sFilterApplied);
    //Show dialog
    if (Filter_ShowSelDlg(this, &DestList) == IDOK)
    {
        vPopulateFilterApplied(m_psFilterConfigured, sFilterApplied, DestList);
        mLinFrameprocessor->FPL_ApplyFilteringScheme((USHORT)m_nLogIndexSel, sFilterApplied);
    }
    return S_OK;
}

void CConfigMsgLogLINDlg::InitialiseWindow()
{
    CBaseDIL_LIN* pouDilLINInterface;
    DIL_GetInterface(LIN, (void**)&pouDilLINInterface);
    //Load channel combo box
    m_omComboChannel.ResetContent();
    if (nullptr != pouDilLINInterface)
    {
        LPARAM lParam = 0;
        if (S_OK == pouDilLINInterface->DILL_GetControllerParams(lParam, 0, NUMBER_HW))
        {
            m_unChannelCount = (UINT)lParam;
        }
    }

    m_omComboChannel.InsertString(0, _("ALL"));
    for (UINT i = 1; i <= m_unChannelCount; i++)
    {
        CString omChannel;
        omChannel.Format("%d", i);
        m_omComboChannel.InsertString(i, omChannel.GetBuffer(MAX_PATH));
    }
    m_omComboChannel.SetCurSel(0);
}