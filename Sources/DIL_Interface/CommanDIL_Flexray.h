#pragma once
#include "BaseDIL_FLEXRAY_Controller.h"
#include "ClientList.h"

typedef struct tagAckMap
{
    UINT m_unMsgID;
    UINT m_unClientID;
    UINT m_unChannel;

    BOOL operator == (const tagAckMap& RefObj)
    {
        return ((m_unMsgID == RefObj.m_unMsgID) && (m_unChannel == RefObj.m_unChannel));
    }
} SACK_MAP;

typedef list<SACK_MAP> CACK_MAP_LIST;
static CACK_MAP_LIST sg_asAckMapBuf;


class CCommanDIL_Flexray : public CBaseDIL_FLEXRAY_Controller
{
public:
    CCommanDIL_Flexray(void);
    ~CCommanDIL_Flexray(void);

public:
    virtual HRESULT FLEXRAY_PerformInitOperations(void) = 0;
    virtual HRESULT FLEXRAY_PerformClosureOperations(void) = 0;
    virtual HRESULT FLEXRAY_GetTimeModeMapping(SYSTEMTIME& CurrSysTime, UINT64& TimeStamp) = 0;
    virtual HRESULT FLEXRAY_ListHwInterfaces(FLEXRAY_INTERFACE_HW& sSelHwInterface, FLEXRAY_CONFIG_FILES& sAvailableConfigFiles) = 0;
    virtual HRESULT FLEXRAY_SelectHwInterface(const FLEXRAY_INTERFACE_HW_LIST& sSelHwInterface) = 0;
    virtual HRESULT FLEXRAY_DeselectHwInterface(void) = 0;
    virtual HRESULT FLEXRAY_DisplayConfigDlg(PCHAR& InitData, int& Length) = 0;
    virtual HRESULT FLEXRAY_SetConfigData(CHANNEL_CONFIG& ouAbsSFibexContainer) = 0;
    virtual HRESULT FLEXRAY_SetAppParams(HWND hWndOwner, Base_WrapperErrorLogger* pILog) = 0;
    virtual HRESULT FLEXRAY_StartHardware(void) = 0;
    virtual HRESULT FLEXRAY_StopHardware(void) = 0;
    virtual HRESULT FLEXRAY_ResetHardware(void) = 0;
    virtual HRESULT FLEXRAY_GetCurrStatus(s_FLXSTATUSMSG& StatusData) = 0;
    virtual HRESULT FLEXRAY_GetFlxTxMsgBuffer(CFlxTxMsgBuffer*& pouFlxTxMsgBuffer) = 0;
    virtual HRESULT FLEXRAY_SendMsg(DWORD dwClientID, s_FLXTXMSG* pouFlxTxMsg) = 0;
    virtual HRESULT FLEXRAY_DeleteMsg(DWORD dwClientID, s_FLXTXMSG* pouFlxTxMsg) = 0;
    virtual HRESULT FLEXRAY_GetBoardInfo(s_BOARDINFO& BoardInfo) = 0;
    virtual HRESULT FLEXRAY_GetFlexRayInfo(s_FLXINFO& FlexRayInfo) = 0;
    virtual HRESULT FLEXRAY_GetVersionInfo(VERSIONINFO& sVerInfo) = 0;
    virtual HRESULT FLEXRAY_GetLastErrorString(string acErrorStr, HRESULT& nError) = 0;
    virtual HRESULT FLEXRAY_FilterFrames(FILTER_TYPE FilterType, FLEX_CHANNEL FlexChannel, UINT* punFrames, UINT nLength) = 0;
    virtual HRESULT FLEXRAY_ConfigMsgBuf(s_MSGBUF sMsgBuf, UINT* punAddress) = 0;
    virtual HRESULT FLEXRAY_LoadDriverLibrary(void) = 0;
    virtual HRESULT FLEXRAY_UnloadDriverLibrary(void) = 0;
    virtual HRESULT FlexRAY_GetControllerCapabilities(s_FLXControllerCapabilities& ouFlexRayCapabilities) = 0;



    virtual HRESULT FLEXRAY_RegisterClient(BOOL bRegister, DWORD& ClientID, char* pacClientName);
    virtual HRESULT FLEXRAY_ManageMsgBuf(BYTE bAction, DWORD ClientID, CBaseFLEXBufFSE* pBufObj);
    virtual HRESULT FLEXRAY_SendFlxMsg(DWORD dwClientID, s_FLXTXMSG* pouFlxTxMsg);
    virtual HRESULT FLEXRAY_DeleteFlxMsg(DWORD dwClientID, s_FLXTXMSG* pouFlxTxMsg);


public:
    void vWriteIntoClientsBuffer(s_FLXMSG& sFlexRayData);
    void vMarkEntryIntoMap(const SACK_MAP& RefObj);
    bool bRemoveMapEntry(const SACK_MAP& RefObj, UINT& ClientID);


public:
    CRITICAL_SECTION sg_CritSectForAckBuf;

private:
    CClientList     m_ClientList;               ///< List with all of registered clients
    CACK_MAP_LIST   sg_asAckMapBuf;
protected:
    map< SLOT, list<int> > m_mapSlotClient;
};

