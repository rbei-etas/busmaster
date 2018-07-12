#pragma once
// MessageWindowSettingsDialog
#include "DataTypes\Filter_Datatypes.h"

#include "PPageMessage.h"
#include "MsgFilterConfigPage.h"
#include "MsgBufferConfigPage.h"





struct MessageColorSettings
{
    bool mValidSettings;
    class CMessageAttrib* mMsgAttributes;
    UINT* mMessageIDs;
    int mMsgCount;
};
struct MessageWindowSettings
{
    bool mBusmasterIsOnline;
    MessageColorSettings mMessageAttribute;
    msgBufferSettings mBufferSettings;
    filterDetails mFilterDetails;
};

class MessageWindowSettingsDialog : public CPropertySheet
{
    DECLARE_DYNAMIC(MessageWindowSettingsDialog)

    MessageWindowSettings* mSettings;
    CPPageMessage* odDBMsg;
    CPPageMessage* odNDBMsg;
    CMsgFilterConfigPage* omFilter;
    CMsgBufferConfigPage* mMsgBuffConf;


public:
    MessageWindowSettingsDialog(LPCTSTR pszCaption, MessageWindowSettings*);
    virtual ~MessageWindowSettingsDialog();

protected:
    DECLARE_MESSAGE_MAP()
public:
    virtual BOOL OnInitDialog();
};


