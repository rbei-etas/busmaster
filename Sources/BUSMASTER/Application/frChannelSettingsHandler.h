#pragma once

#include "IBMNetWorkService.h"
#include <libxml\parser.h>
class frChannelSettingsHandler
{
    typedef int(*INVOKEFRCHANNELSETTINGSUI)(IBMNetWorkService* pBmNetWork, CWnd* parent);
    typedef int(*GETCONFIGURATION)(xmlNodePtr& pNodePtr, IBMNetWorkService* bmNetWorkService);
    typedef int(*SETCONFIGURATION)(const xmlDocPtr xmlDocPtr, IBMNetWorkService* bmNetworkService);
    HMODULE mFlexRayChannelConfgiDll = nullptr;
public:
    INVOKEFRCHANNELSETTINGSUI mInvokeFrChannelSettingsUi = nullptr;
    GETCONFIGURATION mGetFrChannelConfiguration = nullptr;
    SETCONFIGURATION mSetFrChannelConfiguration = nullptr;
    void LoadChannelConfiguration();
    void UnLoadChannelConfiguration();
    ~frChannelSettingsHandler();
};

