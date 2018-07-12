#include "FrameProcessor_stdafx.h"
#include "ConfigMsgLogJ1939Dlg.h"
#include "ProtocolsDefinitions.h"
#include "Utility\MultiLanguageSupport.h"
#include "Filter\Filter_extern.h"
#include "BaseDIL_CAN.h"
#include "DataTypes\Filter_Datatypes.h"
#define BBUSMASTER_J1939_LOGFILENAME     "BUSMASTERLogFile_J1939"

CConfigMsgLogJ1939Dlg::CConfigMsgLogJ1939Dlg(CBaseFrameProcessor_J1939* pouBaseLogger, BOOL& bLogOnConnect, CWnd* pParent, const SFILTERAPPLIED_J1939* psFilter)
    :CConfigMsgLogDlg(pouBaseLogger, bLogOnConnect, pParent)

{
    m_psFilterConfigured = psFilter;
    mLinFrameprocessor = pouBaseLogger;
    m_strCurrWndText = _("Configure Logging for J1939");
    m_omControlParam = _("Start on Reception of PGN 0x");
    m_omControlParam2 = _("Stop on Reception of PGN 0x");
}


CConfigMsgLogJ1939Dlg::~CConfigMsgLogJ1939Dlg()
{
}


std::string CConfigMsgLogJ1939Dlg::GetDefaultLogFileName()
{
    return BBUSMASTER_J1939_LOGFILENAME;
}
HRESULT CConfigMsgLogJ1939Dlg::HandleFilterSettings()
{
    return S_OK;
}

void CConfigMsgLogJ1939Dlg::InitialiseWindow()
{
    //Load channel combo box
    CBaseDIL_CAN* pouDilInterface;
    DIL_GetInterface(CAN, (void**)&pouDilInterface);
    m_omComboChannel.ResetContent();
    if (nullptr != pouDilInterface)
    {
        LPARAM lParam = 0;
        if (S_OK == pouDilInterface->DILC_GetControllerParams(lParam, 0, NUMBER_HW))
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