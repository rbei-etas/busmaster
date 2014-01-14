#pragma once
#include "BaseDIL_LIN_Controller.h"


#define MAX_CLIENT_ALLOWED 16
#define MAX_BUFF_ALLOWED    16
#define LIN_MAX_ERRSTR 256
#define RECEIVE_EVENT_SIZE   1     // DO NOT EDIT! Currently 1 is supported only
#define RX_QUEUE_SIZE      256     // internal driver queue size in LIN events
#define DEFAULT_LIN_DLC      8     //!< default DLC for master/slave
#define DEFAULT_LIN_BAUDRATE 19200     //!< default LIN baudrate

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

typedef struct tagClientBufMap
{
    DWORD dwClientID;
    BYTE hClientHandle;
    CBaseLINBufFSE* pClientBuf[MAX_BUFF_ALLOWED];
    char pacClientName[MAX_PATH];
    UINT unBufCount;
    tagClientBufMap()
    {
        dwClientID = 0;
        hClientHandle = NULL;
        unBufCount = 0;
        memset(pacClientName, 0, sizeof (char) * MAX_PATH);
        for (int i = 0; i < MAX_BUFF_ALLOWED; i++)
        {
            pClientBuf[i] = NULL;
        }

    }
} SLINCLIENTBUFMAP;

typedef list<SACK_MAP> CACK_MAP_LIST;


class CCommanDIL_LIN:public CBaseDIL_LIN_Controller
{
    UINT m_unClientCnt;
    SLINCLIENTBUFMAP m_asClientToBufMap[MAX_CLIENT_ALLOWED];
    //static CACK_MAP_LIST m_asAckMapBuf;

public:
    CCommanDIL_LIN();
    ~CCommanDIL_LIN();
    virtual HRESULT LIN_PerformInitOperations(void) = 0;
    virtual HRESULT LIN_PerformClosureOperations(void) = 0;
    virtual HRESULT LIN_GetTimeModeMapping(SYSTEMTIME& CurrSysTime, UINT64& TimeStamp, LARGE_INTEGER* QueryTickCount = NULL) = 0;
    virtual HRESULT LIN_ListHwInterfaces(INTERFACE_HW_LIST& sSelHwInterface, INT& nCount) = 0;
    virtual HRESULT LIN_SelectHwInterface(const INTERFACE_HW_LIST& sSelHwInterface, INT nCount) = 0;
    virtual HRESULT LIN_DeselectHwInterface(void) = 0;
    virtual HRESULT LIN_DisplayConfigDlg(PSCONTROLLER_DETAILS InitData, int& Length) = 0;
    virtual HRESULT LIN_SetConfigData(ClusterConfig& ouConfig) = 0;
    virtual HRESULT LIN_StartHardware(void) = 0;
    virtual HRESULT LIN_PreStartHardware(void) = 0;
    virtual HRESULT LIN_StopHardware(void) = 0;
    virtual HRESULT LIN_ResetHardware(void) = 0;
    virtual HRESULT LIN_GetCurrStatus(s_STATUSMSG& StatusData) = 0;

    virtual HRESULT LIN_SetSlaveRespData(const STLIN_MSG stRespMsg) = 0;
    virtual HRESULT LIN_ResetSlaveRespData(void) = 0;

    virtual HRESULT LIN_GetLastErrorString(string& acErrorStr) = 0;
    virtual HRESULT LIN_GetControllerParams(LONG& lParam, UINT nChannel, ECONTR_PARAM eContrParam) = 0;
    //MVN
    virtual HRESULT LIN_SetControllerParams(int nValue, ECONTR_PARAM eContrparam) = 0;
    //~MVN
    virtual HRESULT LIN_GetErrorCount(SERROR_CNT& sErrorCnt, UINT nChannel, ECONTR_PARAM eContrParam) = 0;

    virtual HRESULT LIN_Send(STLIN_MSG& sCanTxMsg) = 0;
    virtual HRESULT LIN_DisableSlave(STLIN_MSG& pouFlxTxMsg) = 0;

    // Specific function set
    virtual HRESULT LIN_SetAppParams(HWND hWndOwner, Base_WrapperErrorLogger* pILog) = 0;


    virtual HRESULT LIN_GetCntrlStatus(const HANDLE& hEvent, UINT& unCntrlStatus) = 0;
    virtual HRESULT LIN_LoadDriverLibrary(void) = 0;
    virtual HRESULT LIN_UnloadDriverLibrary(void) = 0;


    virtual HRESULT LIN_RegisterClient(BOOL bRegister, DWORD& ClientID, char* pacClientName);
    virtual HRESULT LIN_ManageMsgBuf(BYTE bAction, DWORD ClientID, CBaseLINBufFSE* pBufObj);
    virtual HRESULT LIN_SendMsg(DWORD dwClientID, STLIN_MSG& sCanTxMsg);
    virtual HRESULT LIN_DisableSlaveRespData(DWORD dwClientID, STLIN_MSG& pouFlxTxMsg);


protected:
    void vWriteIntoClientsBuffer(STLINDATA& sLinData);
public:

    void vMarkEntryIntoMap(const SACK_MAP& RefObj);
    bool vPeakMapEntry(const SACK_MAP& RefObj, UINT& ClientID, bool bRemove = false);


public:
    CRITICAL_SECTION sg_CritSectForAckBuf;
    DWORD dwGetAvailableClientSlot();
    BOOL bClientIdExist(const DWORD& dwClientId);
    BOOL bClientExist(string pcClientName, INT& Index);
    BOOL bRemoveClient(DWORD dwClientId);
    BOOL bGetClientObj(DWORD dwClientID, UINT& unClientIndex);
    BOOL bIsBufferExists(const SLINCLIENTBUFMAP& sClientObj, const CBaseLINBufFSE* pBuf);
    BOOL bRemoveClientBuffer(CBaseLINBufFSE* RootBufferArray[MAX_BUFF_ALLOWED], UINT& unCount, CBaseLINBufFSE* BufferToRemove);


private:
    //CClientList     m_ClientList;               ///< List with all of registered clients
    CACK_MAP_LIST   sg_asAckMapBuf;
protected:
    map< SLOT, list<int> > m_mapSlotClient;
    map<int, int> m_MapIdClient[16];
};



