#include "stdafx.h"
#include "frChannelSettingsHandler.h"


void frChannelSettingsHandler::LoadChannelConfiguration()
{
    if (nullptr == mInvokeFrChannelSettingsUi)
    {
        mFlexRayChannelConfgiDll = LoadLibrary("FlexRayChannelConfiguration.dll");
        if (mFlexRayChannelConfgiDll != nullptr)
        {
            mInvokeFrChannelSettingsUi = (INVOKEFRCHANNELSETTINGSUI)GetProcAddress(mFlexRayChannelConfgiDll, "InvokeFrChannelSettingsUi");
            mGetFrChannelConfiguration = (GETCONFIGURATION)GetProcAddress(mFlexRayChannelConfgiDll, "getConfiguration");
            mSetFrChannelConfiguration = (SETCONFIGURATION)GetProcAddress(mFlexRayChannelConfgiDll, "setConfiguration");
        }
    }
}
void frChannelSettingsHandler::UnLoadChannelConfiguration()
{
    if (nullptr != mInvokeFrChannelSettingsUi)
    {
        FreeLibrary(mFlexRayChannelConfgiDll);
        mInvokeFrChannelSettingsUi = nullptr;
        mGetFrChannelConfiguration = nullptr;
        mSetFrChannelConfiguration = nullptr;
    }
}
frChannelSettingsHandler::~frChannelSettingsHandler()
{
    UnLoadChannelConfiguration();
}
