#pragma once
#include "ConfigMsgLogDlg.h"
#include "FrameProcessor/BaseFrameProcessor_J1939.h"
#include "Datatypes/Filter_Datatypes.h"
class CConfigMsgLogJ1939Dlg : public CConfigMsgLogDlg
{
    CBaseFrameProcessor_J1939* mLinFrameprocessor;
    const SFILTERAPPLIED_J1939* m_psFilterConfigured;
public:
    CConfigMsgLogJ1939Dlg(CBaseFrameProcessor_J1939* pouBaseLogger, BOOL& bLogOnConnect, CWnd* pParent = nullptr, const SFILTERAPPLIED_J1939* psFilter = nullptr);
    ~CConfigMsgLogJ1939Dlg();

    virtual std::string GetDefaultLogFileName();
    virtual HRESULT HandleFilterSettings();
    virtual void InitialiseWindow();
};
