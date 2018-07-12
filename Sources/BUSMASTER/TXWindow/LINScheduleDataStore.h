#include "stdafx.h"
#include "Utility/XMLUtils.h"
#include "ICluster.h"
#include "IBMNetWorkGetService.h"


class CBaseDIL_LIN;

struct SCheduleTableData
{
    std::list<CSheduleTable> m_ouScheduleTables;
    std::list<int> m_ouTableHandles;
    std::string m_strCurrentTable;
};

struct FrameState
{
    std::string omName;
    bool   bIsEnabled;
};

class CLINScheduleDataStore
{
    IBMNetWorkGetService* m_ouClusterConfig;
    bool bAddTableListToList(std::string strSelectedTable, std::list<CSheduleTable>& ouDestTable, std::list<CSheduleTable>& ouSourceTable );
    void vAddCommandsToTable(CSheduleTable& ouDestTable, CSheduleTable& ouSourceTable );
    void vAddTableToList(std::list<CSheduleTable>& ouDestTable, CSheduleTable ouSourceTable );
    void vAddCommandsToList();


    CBaseDIL_LIN* m_pouDIL_LIN_Interface;
    ESTATUS_BUS m_eBusStatus;

public:
    DWORD m_dwClientID;
    CLINScheduleDataStore();
    SCheduleTableData m_ouTableData[CHANNEL_ALLOWED];
    WINDOWPLACEMENT m_sSchedWndPlacement;
    BOOL m_bValidWndSize;
    ~CLINScheduleDataStore()
    {
    }

    HRESULT hSetConfigData(xmlDocPtr pDoc);
    void vSetScheduleConfig(xmlNodePtr pNode);
    HRESULT hGetConfigData(xmlNodePtr pxmlNodePtr);
    HRESULT hSetClusterConfig(IBMNetWorkGetService* objLINClusterConfig);
    HRESULT hSaveSchdSelectionDetails();
    IBMNetWorkGetService* pGetClusterConfig();



    static CLINScheduleDataStore m_sTxLINDataStoreObj;
    static CLINScheduleDataStore& pGetLINSchedDataStore();
    void vSetWindowPlacement(WINDOWPLACEMENT wndPlacement);
    void vGetWindowPlacement(WINDOWPLACEMENT& wndPlacement);
    void vSetBusStatus(ESTATUS_BUS eBusStatus);

    int nEnableScheduleTable(int nChannel, int nIndex, bool bEnable = true);
    int nEnableScheduleCommand(int nChannel, int nTableIndex, int nItem, bool bEnable = true);


    int nTransmitScheduleCommand(int nChannel, int nTableIndex, int nItem, bool bEnable = true );
    int nTransmitScheduleTable(int nChannel, int nTableIndex, bool bEnable = true );

    int nEnableCommands(CSheduleTable& ouTable, bool bEnable = true);

    bool bIsFrameSupported(eCommandType eCmdType);

    int nGetTableHandle(int nChannel, int nTableIndex);

protected:


};