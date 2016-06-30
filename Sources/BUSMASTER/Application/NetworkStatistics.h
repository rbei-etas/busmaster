#pragma once



// CNetworkStatistics
#include "HashDefines.h"
#include "MessageList.h"
#include "DeviceListInfo.h"
#include "ProtocolsDefinitions.h"

#include <libxml/xmlmemory.h>
#include <libxml/parser.h>
#include <libxml/xpath.h>

#define CAN_STAT_PAGE           0
#define LIN_STAT_PAGE           1

// information of CBusStatisticsDlg serialization data
struct sBUSSTATTISTICSDATA
{
    UINT m_nColumnCount;
    bool m_arrbColumnVisible[CHANNEL_CAN_MAX+1];
    int  m_arrnOrder[CHANNEL_CAN_MAX+1];
    int  m_arrnColWidth[CHANNEL_CAN_MAX+1];
    WINDOWPLACEMENT    m_sBusStatsDlgCoOrd;      //  CBusStatisticsDlg coords
    WINDOWPLACEMENT    m_sDefaultBusStatsDlgCoOrd;
    /* constructor */
    sBUSSTATTISTICSDATA();
    void vLoadDefaults();

    bool m_bIsDirty;
};

class CCommonStatistics : public CPropertyPage
{
    DECLARE_DYNAMIC(CCommonStatistics)

public:
    ETYPE_BUS m_eBusType;
    CCommonStatistics(UINT, ETYPE_BUS);


    ~CCommonStatistics()
    {
    }
    xmlNodePtr m_pxmlNodePtr;
    CMessageList m_omStatList;
    int m_nChannelCount;

    sBUSSTATTISTICSDATA sm_sBusSerializationData;

    HRESULT vGetConfigData(xmlNodePtr& pxmlNodePtr);
    HRESULT vGetConfigDetails(xmlNodePtr& pNodePtr);
    HRESULT hSetConfigData(xmlNodePtr pDocPtr);
    HRESULT hSetConfigData();
    HRESULT hSetConfigDetails(xmlNodePtr pNodePtr);
    void vLoadDataFromStore(xmlNodePtr pxmlNodePtr);
    void vLoadDefaultValues();
    void vUpdateChannelCountInfo(int nChannelCount);

    HRESULT SetConfigData(BYTE* pvDataStream);
    HRESULT SetConfigData(xmlNodePtr pDocPtr);
    UINT nGetBusStatsDlgConfigSize();
    void vLoadDataFromStore();

protected:
    DECLARE_MESSAGE_MAP()
};

class CNetworkStatistics : public CPropertySheet
{
    DECLARE_DYNAMIC(CNetworkStatistics)

public:

    BOOL m_bIsWindowInitialised;
    CCommonStatistics* m_omStatistics[BUS_TOTAL];

    WINDOWPLACEMENT    m_sBusStatsDlgCoOrd;
    WINDOWPLACEMENT    m_sDefaultBusStatsDlgCoOrd;


    CNetworkStatistics(UINT unChnl[BUS_TOTAL]);
    CNetworkStatistics(UINT nIDCaption, CWnd* pParentWnd = nullptr, UINT iSelectPage = 0);
    CNetworkStatistics(LPCTSTR pszCaption, CWnd* pParentWnd = nullptr, UINT iSelectPage = 0);
    virtual ~CNetworkStatistics();
    void vSendMessage(ETYPE_BUS bus, BOOL bUpdateSpecific = FALSE);
    void vGetConfigData(xmlNodePtr& pxmlNodePtr);
    HRESULT hSetConfigData(xmlNodePtr pxmlDocPtr, BOOL bShow = FALSE);
    void vGetWndPosConfig(xmlNodePtr& pxmlBusStats);
    void vSetNtwStatWndPos(xmlNodePtr pDocPtr, BOOL bShow);
    void vSetDefaultWindowPos();
    xmlNodePtr m_pCopyBusStsticsNode_LIN;
    void vUpdateChannelInfo(ETYPE_BUS eBus,UINT unHardware);
    WINDOWPLACEMENT wpWindowPosition();
    virtual BOOL OnInitDialog();
public:
    afx_msg void OnClose();
protected:
    afx_msg void OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI);
    afx_msg void OnSize(UINT nType, int cx, int cy);
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);

    DECLARE_MESSAGE_MAP()
private:
    void vAddPages(UINT unChnl[BUS_TOTAL]);
    void vGetUpdatedRect(int cx, int cy, int& xPoint, int& yPoint);
    void vCalculateWndRect(CRect& r1, int xPoint, int yPoint);

private:

    CRect  m_rCrt;
    int    m_nMincX;
    int    m_nMincY;
};

