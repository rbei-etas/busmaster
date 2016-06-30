// MessageWindowSettingsDialog.cpp : implementation file
//

#include "stdafx.h"
#include "BUSMASTER.h"
#include "MessageWindowSettingsDialog.h"

// MessageWindowSettingsDialog

IMPLEMENT_DYNAMIC(MessageWindowSettingsDialog, CPropertySheet)

#include "MessageAttrib.h"

MessageWindowSettingsDialog::MessageWindowSettingsDialog(LPCTSTR pszCaption, MessageWindowSettings* settings)
    :CPropertySheet(pszCaption)
{
    mSettings = settings;
    // TODO:  Add your specialized code here
    odDBMsg = new CPPageMessage(TRUE, mSettings->mMessageAttribute.mMessageIDs, mSettings->mMessageAttribute.mMsgCount, mSettings->mMessageAttribute.mMsgAttributes);
    odNDBMsg = new CPPageMessage(FALSE, mSettings->mMessageAttribute.mMessageIDs, mSettings->mMessageAttribute.mMsgCount, mSettings->mMessageAttribute.mMsgAttributes);
    omFilter = new CMsgFilterConfigPage(pszCaption, &mSettings->mFilterDetails, nullptr);


    //mMsgBuffConf->vSetBufferSize(m_anMsgBuffSize[CAN]);
    if (true == mSettings->mFilterDetails.mISValidSettings)
    {
        AddPage(omFilter);
    }
    if (true == mSettings->mMessageAttribute.mValidSettings)
    {
        AddPage(odDBMsg);
        AddPage(odNDBMsg);
    }
    if (true == mSettings->mBufferSettings.mISValidSettings)
    {
        mMsgBuffConf = new CMsgBufferConfigPage(&settings->mBufferSettings);
        AddPage(mMsgBuffConf);
    }
}

MessageWindowSettingsDialog::~MessageWindowSettingsDialog()
{
}


BEGIN_MESSAGE_MAP(MessageWindowSettingsDialog, CPropertySheet)
END_MESSAGE_MAP()


// MessageWindowSettingsDialog message handlers


BOOL MessageWindowSettingsDialog::OnInitDialog()
{
    BOOL bResult = CPropertySheet::OnInitDialog();

    CString title;
    GetWindowText(title);
    title = "Configure Message Display - " + title;
    SetWindowText(title);

    return bResult;
}
