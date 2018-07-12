#pragma once
#include "ConfigMsgLogDlg.h"

#include "ConfigMsgLogDlg.h"
#include "FrameProcessor/BaseFrameProcessor_LIN.h"
#include "Datatypes/Filter_Datatypes.h"
class CConfigMsgLogLINDlg : public CConfigMsgLogDlg
{
    CBaseFrameProcessor_LIN* mLinFrameprocessor;
    const SFILTERAPPLIED_LIN* m_psFilterConfigured;
public:
    CConfigMsgLogLINDlg(CBaseFrameProcessor_LIN* pouBaseLogger, BOOL& bLogOnConnect, CWnd* pParent = nullptr, const SFILTERAPPLIED_LIN* psFilter = nullptr);
    ~CConfigMsgLogLINDlg();

    virtual std::string GetDefaultLogFileName();
    virtual HRESULT HandleFilterSettings();
    virtual void InitialiseWindow();
};

