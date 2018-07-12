#pragma once
#include <list>
#include <string>
#include "IBMNetWorkGetService.h"
class LinChannelParam
{
public:
    int m_nBaudRate;
    bool m_bIsMasterMode;
    std::string m_strProtocolVersion;
    bool m_bOverWriteSettings;
    std::list<CSheduleTable> ouLinParams;
    void vInitialiseConfig()
    {
        m_nBaudRate = 19200;
        m_strProtocolVersion = "LIN 2.0";
        m_bOverWriteSettings = true;
        m_bIsMasterMode = false;
        ouLinParams.clear();
    }
    LinChannelParam()
    {
        vInitialiseConfig();
    }
    LinChannelParam( const LinChannelParam& ouRefObj )
    {
        m_nBaudRate = ouRefObj.m_nBaudRate;
        m_strProtocolVersion = ouRefObj.m_strProtocolVersion;
        m_bOverWriteSettings = ouRefObj.m_bOverWriteSettings;
        m_bIsMasterMode = ouRefObj.m_bIsMasterMode;
        ouLinParams.clear();
        ouLinParams.insert( ouLinParams.begin(), ouRefObj.ouLinParams.begin(), ouRefObj.ouLinParams.end() );
    }
    LinChannelParam operator=( const LinChannelParam& ouRefObj )
    {
        m_nBaudRate = ouRefObj.m_nBaudRate;
        m_strProtocolVersion = ouRefObj.m_strProtocolVersion;
        m_bOverWriteSettings = ouRefObj.m_bOverWriteSettings;
        m_bIsMasterMode = ouRefObj.m_bIsMasterMode;
        ouLinParams.clear();
        ouLinParams.insert( ouLinParams.begin(), ouRefObj.ouLinParams.begin(), ouRefObj.ouLinParams.end() );
        return *this;
    }


};