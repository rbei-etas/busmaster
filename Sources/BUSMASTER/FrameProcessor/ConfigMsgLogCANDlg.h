#pragma once
#include "ConfigMsgLogDlg.h"
#include "FrameProcessor/BaseFrameProcessor_CAN.h"
#include "Datatypes/Filter_Datatypes.h"
class CConfigMsgLogCANDlg :
    public CConfigMsgLogDlg
{
protected:
    DECLARE_MESSAGE_MAP()
private:
    CBaseFrameProcessor_CAN* mLinFrameprocessor;
    const SFILTERAPPLIED_CAN* m_psFilterConfigured;
public:
    CConfigMsgLogCANDlg(CBaseFrameProcessor_CAN* pouBaseLogger, BOOL& bLogOnConnect, CWnd* pParent = nullptr, const SFILTERAPPLIED_CAN* psFilter = nullptr);
    ~CConfigMsgLogCANDlg();

    virtual std::string GetDefaultLogFileName();
    virtual HRESULT HandleFilterSettings();
    virtual void InitialiseWindow();
};

