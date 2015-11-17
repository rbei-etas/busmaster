#include "stdafx.h"

#include "IClusterProps.h"
#include "Defines.h"
#include "ScheduleTableParser.h"
//typedef  ;
CScheduleTableParser::CScheduleTableParser(LDFCluster* pouCluster, std::map<std::string, UID_ELEMENT>& mapFrameUID):m_mapFrameUID(mapFrameUID)
{
    m_pouLDFCluster = pouCluster;


    m_pFillTableItem[eLIN_NORMAL_FRAME_ID] = &CScheduleTableParser::FillNormalTableItem;
    m_pFillTableItem[eLIN_MASTER_FRAME_ID] = &CScheduleTableParser::FillMasterTableItem;
    m_pFillTableItem[eLIN_SLAVE_FRAME_ID] = &CScheduleTableParser::FillMasterTableItem;
    m_pFillTableItem[eLIN_SID_ASSIGN_FRAME_ID] = &CScheduleTableParser::FillAssignFrameIdTableItem;
    m_pFillTableItem[eLIN_SID_UNASSIGN_FRAME_ID] = &CScheduleTableParser::FillAssignFrameIdTableItem;
    m_pFillTableItem[eLIN_SID_COND_CHANGE_NAD] = &CScheduleTableParser::FillConditionalChnageNADIdTableItem;
    m_pFillTableItem[eLIN_SID_DATA_DUMP] = &CScheduleTableParser::FillDataDumpTableItem;
    m_pFillTableItem[eLIN_SID_SAVE_CONFIG] = &CScheduleTableParser::FillSaveConfigTableItem;
    m_pFillTableItem[eLIN_SID_ASSIGN_FRAME_RANGE] = &CScheduleTableParser::FillAssignFrameRangeTableItem;
    m_pFillTableItem[eLIN_SID_FREEFORMAT] = &CScheduleTableParser::FillFreeFormatTableItem;
    m_pFillTableItem[eLIN_SID_ASSIGN_NAD_ID] = &CScheduleTableParser::FillAssignNADIdTableItem;
    m_pFillTableItem[eLIN_SID_READ_BY_IDENTIFIER] = &CScheduleTableParser::FillReadByIdentifierTableItem;
}


CScheduleTableParser::~CScheduleTableParser(void)
{
}

ERRORCODE CScheduleTableParser::CreateDiagnosticMessages()
{
    auto itrFrame = m_mapFrameUID.find("MasterReq");

    if(itrFrame == m_mapFrameUID.end())
    {
        AddMasterReq();
        AddSlaveResp();
    }

    return EC_SUCCESS;
}

ERRORCODE CScheduleTableParser::GetScheduleTable(IScheduleTable* pScheduleTable, ldfScheduleTable& ouScheduleTable)
{
    //1. Set Frame Name
    pScheduleTable->SetName(ouScheduleTable.m_strTableName);

    ScheduleTableProps ouTableProps;

    CSheduleTableItem ouTableItem;
for (auto itr : ouScheduleTable.m_ouCommandList)
    {
        FillMasterTableItem(itr, ouTableItem);
        FillScheduleTableItem(itr, ouTableItem);
        ouTableProps.m_ouCSheduleTableItem.push_back(ouTableItem);
    }

    pScheduleTable->SetProperties(ouTableProps);
    return EC_NA;
}



ERRORCODE CScheduleTableParser::FillScheduleTableItem(ldfScheduleTableCommand& ouCommand, CSheduleTableItem& ouTableItem)
{
    ERRORCODE ecRetVal = EC_FAILURE;
    ouTableItem.vInitialiseData();
    if ( ouCommand.m_eDiagType >= eLIN_NORMAL_FRAME_ID && ouCommand.m_eDiagType < eLIN_SID_ALL )
    {
        if(ouCommand.m_eDiagType >= eLIN_SID_ASSIGN_FRAME_ID && ouCommand.m_eDiagType < eLIN_SID_ALL )
        {
            CreateDiagnosticMessages();
        }

        ecRetVal = (this->*m_pFillTableItem[ouCommand.m_eDiagType])(ouCommand, ouTableItem );
        ouTableItem.m_eDiagType = ouCommand.m_eDiagType;
        ouTableItem.m_dDelay = ouCommand.m_dDelay;
    }

    return ecRetVal;
}

ERRORCODE CScheduleTableParser::FillMasterTableItem(ldfScheduleTableCommand& ouCommand, CSheduleTableItem& ouTableItem)
{
    //1. Delay
    ouTableItem.m_dDelay = ouCommand.m_dDelay;

    //2. Diag Type
    ouTableItem.m_eDiagType = ouCommand.m_eDiagType;

    //4. Frame Id
    auto itrFrame = m_mapFrameUID.find(ouCommand.m_strCommandName);
    if ( m_mapFrameUID.end() != itrFrame )
    {
        ouTableItem.m_nFrameId = itrFrame->second;
    }
    else
    {
        return EC_FAILURE;
    }

    return EC_NA;
}

ERRORCODE CScheduleTableParser::FillNormalTableItem(ldfScheduleTableCommand& ouCommand, CSheduleTableItem& ouTableItem)
{
    //1. Delay
    ouTableItem.m_dDelay = ouCommand.m_dDelay;

    //2. Diag Type
    ouTableItem.m_eDiagType = eLIN_NORMAL_FRAME_ID;

    //3. Data Type
    memset(ouTableItem.m_chDataBytes, 0, sizeof(ouTableItem.m_chDataBytes));

    //4. Frame Id
    auto itrFrame = m_mapFrameUID.find(ouCommand.m_strCommandName.c_str());
    if ( m_mapFrameUID.end() != itrFrame )
    {
        ouTableItem.m_nFrameId = itrFrame->second;
    }

    return EC_NA;
}

ERRORCODE CScheduleTableParser::AddMasterReq()
{
    IFrame* pouFrame = nullptr;

    //Slave
    FrameProps ouFrameProps;

    if(nullptr != m_pouLDFCluster)
    {
        m_pouLDFCluster->CreateElement(eFrameElement, (IElement**)&pouFrame);

        std::string omFrameName = "MasterReq";
        pouFrame->SetName(omFrameName);
        pouFrame->GetProperties(ouFrameProps);
        ouFrameProps.m_eFrameType = eLIN_Diagnostic;
        ouFrameProps.m_ouLINDiagnosticFrameProps.m_eDiagType = eLIN_MASTER_FRAME_ID;
        ouFrameProps.m_ouLINDiagnosticFrameProps.m_nLength = 8;
        ouFrameProps.m_ouLINDiagnosticFrameProps.m_unId = 0x3C;
        pouFrame->SetProperties(ouFrameProps);

        std::map<UID_ELEMENT, IElement*> ouEcuMap;
        m_pouLDFCluster->GetElementList(eEcuElement, ouEcuMap);
        eEcuType oueEcuType;
        UID_ELEMENT uidElement = pouFrame->GetUniqueId();
        UID_ELEMENT uidMaster;
        std::list<UID_ELEMENT> uidSlaveList;
for ( auto itrEcu : ouEcuMap )
        {
            eDIR oueDir;
            IEcu* pouEcu = (IEcu*)itrEcu.second;
            pouEcu->GetEcuType(oueEcuType);
            if (eLIN_Master == oueEcuType)
            {
                uidMaster = pouEcu->GetUniqueId();
                pouFrame->MapNode(eTx, uidMaster);
                oueDir = eTx;
            }
            else
            {
                UID_ELEMENT uidTemp = pouEcu->GetUniqueId();
                pouFrame->MapNode(eRx, uidTemp);
                uidSlaveList.push_back(uidTemp);
                oueDir = eRx;
            }
            pouEcu->MapFrame(oueDir, uidElement);
        }
        //Signals
        ISignal* pouSignal;
        char chName[33];
        SignalInstanse ouSignalInstanse;
        SignalProps ouSignalProps;
        ouSignalProps.m_ouLINSignalProps.m_nIntialValue = 0;
        ouSignalProps.m_ouLINSignalProps.m_nLength = 8;
        ouSignalProps.m_ouLINSignalProps.m_ouDataType = eUnsigned;
        ouSignalProps.m_ouLINSignalProps.m_ouEndianess = eIntel;
        ouSignalProps.m_ouLINSignalProps.m_ouValueType = eScalar;
        ouSignalProps.m_ouLINSignalProps.m_ouSignalType = eSignalDiag;

        UID_ELEMENT uid = INVALID_UID_ELEMENT;
        for ( int i = 0 ; i < 8; i++ )
        {
            m_pouLDFCluster->CreateElement(eSignalElement, (IElement**)&pouSignal);
            ouSignalInstanse.m_nStartBit = i * 8;
            sprintf_s(chName, "MasterReqB%d", i);
            pouSignal->SetName(std::string(chName));
            pouSignal->SetProperties(ouSignalProps);
            uid = pouSignal->GetUniqueId();
            pouFrame->MapSignal( uid, ouSignalInstanse );
            pouSignal->MapNode(eTx, uidMaster);
for ( auto itr : uidSlaveList )
            {
                pouSignal->MapNode(eRx, itr);
            }

        }
        m_mapFrameUID[omFrameName] = uidElement;
    }
    return 0;
}

int CScheduleTableParser::AddSlaveResp()
{
    IFrame* pouFrame = nullptr;

    //Slave
    FrameProps ouFrameProps;
    m_pouLDFCluster->CreateElement(eFrameElement, (IElement**)&pouFrame);

    std::string omFrameName = "SlaveResp";
    pouFrame->SetName(std::string("SlaveResp"));
    pouFrame->GetProperties(ouFrameProps);
    ouFrameProps.m_eFrameType = eLIN_Diagnostic;
    ouFrameProps.m_ouLINDiagnosticFrameProps.m_eDiagType = eLIN_SLAVE_FRAME_ID;
    ouFrameProps.m_ouLINDiagnosticFrameProps.m_nLength = 8;
    ouFrameProps.m_ouLINDiagnosticFrameProps.m_unId = 0x3D;
    pouFrame->SetProperties(ouFrameProps);

    std::map<UID_ELEMENT, IElement*> ouEcuMap;
    m_pouLDFCluster->GetElementList(eEcuElement, ouEcuMap);
    eEcuType oueEcuType;
    UID_ELEMENT uidElement = pouFrame->GetUniqueId();
    UID_ELEMENT uidMaster;
    std::list<UID_ELEMENT> uidSlaveList;
for ( auto itrEcu : ouEcuMap )
    {
        eDIR oueDir;
        IEcu* pouEcu = (IEcu*)itrEcu.second;
        pouEcu->GetEcuType(oueEcuType);
        if (eLIN_Master == oueEcuType)
        {
            uidMaster = pouEcu->GetUniqueId();
            pouFrame->MapNode(eRx, uidMaster);
            oueDir = eRx;
        }
        else
        {
            UID_ELEMENT uidTemp = pouEcu->GetUniqueId();
            pouFrame->MapNode(eTx, uidTemp);
            uidSlaveList.push_back(uidTemp);
            oueDir = eTx;
        }
        pouEcu->MapFrame(oueDir, uidElement);
    }
    //Signals
    ISignal* pouSignal;
    char chName[33];
    SignalInstanse ouSignalInstanse;
    SignalProps ouSignalProps;
    ouSignalProps.m_ouLINSignalProps.m_nIntialValue = 0;
    ouSignalProps.m_ouLINSignalProps.m_nLength = 8;
    ouSignalProps.m_ouLINSignalProps.m_ouDataType = eUnsigned;
    ouSignalProps.m_ouLINSignalProps.m_ouEndianess = eIntel;
    ouSignalProps.m_ouLINSignalProps.m_ouValueType = eScalar;
    ouSignalProps.m_ouLINSignalProps.m_ouSignalType = eSignalDiag;

    UID_ELEMENT uid = INVALID_UID_ELEMENT;
    for ( int i = 0 ; i < 8; i++ )
    {
        m_pouLDFCluster->CreateElement(eSignalElement, (IElement**)&pouSignal);
        ouSignalInstanse.m_nStartBit = i * 8;
        sprintf_s(chName, "SlaveRespB%d", i);
        pouSignal->SetName(std::string(chName));
        pouSignal->SetProperties(ouSignalProps);
        uid = pouSignal->GetUniqueId();
        pouFrame->MapSignal( uid, ouSignalInstanse );
        pouSignal->MapNode(eRx, uidMaster);
for ( auto itr : uidSlaveList )
        {
            pouSignal->MapNode(eTx, itr);
        }

    }

    m_mapFrameUID[omFrameName] = uidElement;
    return 0;
}

ERRORCODE CScheduleTableParser::FillAssignFrameIdTableItem(ldfScheduleTableCommand& ouCommand, CSheduleTableItem& ouTableItem)
{
    IEcu* pEcuElement;

    //1. FrameId
    auto itrFrame = m_mapFrameUID.find("MasterReq");

    if ( m_mapFrameUID.end() == itrFrame )
    {
        return EC_FAILURE;
    }

    ouTableItem.m_nFrameId = itrFrame->second;


    ouTableItem.m_chDataBytes[7] = 0xFF;
    itrFrame = m_mapFrameUID.find(ouCommand.m_strFrameName);
    if ( m_mapFrameUID.end() != itrFrame )
    {
        ouTableItem.m_chDataBytes[7] = itrFrame->second;             //TODO::PID
    }


    //2. Node Id
    m_pouLDFCluster->GetEcu(ouCommand.m_strNodeName, &pEcuElement);
    UID_ELEMENT uidTemp = INVALID_UID_ELEMENT;

    if(nullptr != pEcuElement)
    {
        uidTemp = pEcuElement->GetUniqueId();
        ouTableItem.m_nNode = uidTemp;
    }
    if ( INVALID_UID_ELEMENT == uidTemp )
    {
        return EC_FAILURE;
    }
    //ouTableItem.m_nFrameId = uidTemp;

    //3. Fill DataBytes
    ERRORCODE ecRetVal = EC_SUCCESS;
    if ( nullptr != pEcuElement )
    {
        EcuProperties ouEcuProperties;
        pEcuElement->GetProperties(ouEcuProperties);
        if ( ouEcuProperties.m_eEcuType == eLIN_Slave)
        {
            ouTableItem.m_eDiagType = ouCommand.m_eDiagType;
            ouTableItem.m_chDataBytes[defLIN_NAD_INDEX] = ouEcuProperties.m_ouSlavePros.m_nConfiguredNAD;
            ouTableItem.m_chDataBytes[defLIN_PCI_INDEX] = 0x6;
            ouTableItem.m_chDataBytes[defLIN_SID_INDEX] = ( ouCommand.m_eDiagType == eLIN_SID_ASSIGN_FRAME_ID ) ?
                    defLIN_SID_ASSIGN_FRAME_ID : defLIN_SID_UNASSIGN_FRAME_ID;
            ouTableItem.m_chDataBytes[3] = ouEcuProperties.m_ouSlavePros.m_nSupplierId;
            ouTableItem.m_chDataBytes[4] = 0;
            ouTableItem.m_chDataBytes[5] = 0xFF;
            ouTableItem.m_chDataBytes[6] = 0xFF;
        }
        else
        {
            ecRetVal = EC_FAILURE;
        }
    }

    return ecRetVal;
}

ERRORCODE CScheduleTableParser::FillAssignNADIdTableItem(ldfScheduleTableCommand& ouCommand, CSheduleTableItem& ouTableItem)
{
    //Syntax
    //2.0 - { <old_NAD>, <new_NAD>, <supplier_id>, <function_id> }
    //2.1 - {<node_name>}

    IEcu* pEcuElement;
    ERRORCODE ecRetVal = EC_SUCCESS;

    //1. FrameId
    auto itrFrame = m_mapFrameUID.find("MasterReq");

    if ( m_mapFrameUID.end() == itrFrame )
    {
        return EC_FAILURE;
    }

    ouTableItem.m_nFrameId = itrFrame->second;

    //TODO: Version Check
    if ( true == ouCommand.m_strNodeName.empty() )
    {
        //1. Node name

        //2. Fill Databytes
        //TODO: Validation required for proper ConfigNAD
        ouTableItem.m_eDiagType = ouCommand.m_eDiagType;
        ouTableItem.m_chDataBytes[defLIN_NAD_INDEX] = ouCommand.m_chDataBytes[0];
        ouTableItem.m_chDataBytes[defLIN_PCI_INDEX] = 0x6;
        ouTableItem.m_chDataBytes[defLIN_SID_INDEX] = defLIN_SID_ASSIGN_NAD_ID;
        ouTableItem.m_chDataBytes[3] = ouCommand.m_chDataBytes[2];
        ouTableItem.m_chDataBytes[4] = 0;
        ouTableItem.m_chDataBytes[5] = ouCommand.m_chDataBytes[3];
        ouTableItem.m_chDataBytes[6] = 0;
        ouTableItem.m_chDataBytes[7] = (unsigned char)ouCommand.m_chDataBytes[1];
    }
    else
    {
        m_pouLDFCluster->GetEcu(ouCommand.m_strNodeName, &pEcuElement);
        if ( nullptr != pEcuElement )
        {
            //1. Node
            ouTableItem.m_nNode = pEcuElement->GetUniqueId();

            //2. Fill Databytes
            EcuProperties ouEcuProperties;
            pEcuElement->GetProperties(ouEcuProperties);
            if ( ouEcuProperties.m_eEcuType == eLIN_Slave)
            {
                ouTableItem.m_eDiagType = ouCommand.m_eDiagType;
                ouTableItem.m_chDataBytes[defLIN_NAD_INDEX] = ouEcuProperties.m_ouSlavePros.m_nInitialNAD;
                ouTableItem.m_chDataBytes[defLIN_PCI_INDEX] = 0x6;
                ouTableItem.m_chDataBytes[defLIN_SID_INDEX] = defLIN_SID_ASSIGN_NAD_ID;
                ouTableItem.m_chDataBytes[3] = ouEcuProperties.m_ouSlavePros.m_nSupplierId;
                ouTableItem.m_chDataBytes[4] = 0;
                ouTableItem.m_chDataBytes[5] = ouEcuProperties.m_ouSlavePros.m_nFunctionId;
                ouTableItem.m_chDataBytes[6] = 0;
                ouTableItem.m_chDataBytes[7] = (unsigned char)ouEcuProperties.m_ouSlavePros.m_nConfiguredNAD;
            }
            else
            {
                ecRetVal = EC_FAILURE;
            }
        }
    }
    return ecRetVal;
}

ERRORCODE CScheduleTableParser::FillConditionalChnageNADIdTableItem(ldfScheduleTableCommand& ouCommand, CSheduleTableItem& ouTableItem)
{
    //1. FrameId
    auto itrFrame = m_mapFrameUID.find("MasterReq");

    if ( m_mapFrameUID.end() == itrFrame )
    {
        return EC_FAILURE;
    }

    ouTableItem.m_nFrameId = itrFrame->second;

    //{<NAD>, <id>, <byte>, <mask>, <inv>, <new_NAD>}
    ouTableItem.m_eDiagType = ouCommand.m_eDiagType;
    memset(ouTableItem.m_chDataBytes, 0, sizeof(ouTableItem.m_chDataBytes));
    ouTableItem.m_chDataBytes[defLIN_NAD_INDEX] = ouCommand.m_chDataBytes[0];
    ouTableItem.m_chDataBytes[defLIN_PCI_INDEX] = 0x6;
    ouTableItem.m_chDataBytes[defLIN_SID_INDEX] = defLIN_SID_COND_CHANGE_NAD;
    memcpy( &ouTableItem.m_chDataBytes[3], &ouCommand.m_chDataBytes[1], 5 );
    return EC_NA;
}

ERRORCODE CScheduleTableParser::FillDataDumpTableItem(ldfScheduleTableCommand& ouCommand, CSheduleTableItem& ouTableItem)
{
    //Syntax : {<node_name>, <D1>, <D2>, <D3>, <D4>, <D5>}
    //1. FrameId
    auto itrFrame = m_mapFrameUID.find("MasterReq");

    if ( m_mapFrameUID.end() == itrFrame )
    {
        return EC_FAILURE;
    }

    ouTableItem.m_nFrameId = itrFrame->second;

    IEcu* pEcuElement;
    m_pouLDFCluster->GetEcu(ouCommand.m_strNodeName, &pEcuElement);

    ERRORCODE ecRetVal = EC_SUCCESS;
    if ( nullptr != pEcuElement )
    {
        //1. Node Id
        ouTableItem.m_nNode = pEcuElement->GetUniqueId();

        //2. Fill Databytes
        EcuProperties ouEcuProperties;
        pEcuElement->GetProperties(ouEcuProperties);
        if ( ouEcuProperties.m_eEcuType == eLIN_Slave)
        {
            ouTableItem.m_eDiagType = ouCommand.m_eDiagType;
            ouTableItem.m_chDataBytes[defLIN_NAD_INDEX] = (unsigned char)ouEcuProperties.m_ouSlavePros.m_nConfiguredNAD;
            ouTableItem.m_chDataBytes[defLIN_PCI_INDEX] = 0x6;
            ouTableItem.m_chDataBytes[defLIN_SID_INDEX] = defLIN_SID_DATA_DUMP;
            memcpy(&ouTableItem.m_chDataBytes[defLIN_SID_INDEX+1], &ouCommand.m_chDataBytes[0], 5);
            //memset( &ouTableItem.m_chDataBytes[3], 0XFF, 5 );
        }
        else
        {
            ecRetVal = EC_FAILURE;
        }
    }
    return ecRetVal;
}

ERRORCODE CScheduleTableParser::FillSaveConfigTableItem(ldfScheduleTableCommand& ouCommand, CSheduleTableItem& ouTableItem)
{
    //Syntax : {<node_name>}
    //1. FrameId
    auto itrFrame = m_mapFrameUID.find("MasterReq");

    if ( m_mapFrameUID.end() == itrFrame )
    {
        return EC_FAILURE;
    }

    ouTableItem.m_nFrameId = itrFrame->second;

    IEcu* pEcuElement;
    m_pouLDFCluster->GetEcu(ouCommand.m_strNodeName, &pEcuElement);

    ERRORCODE ecRetVal = EC_SUCCESS;
    if ( nullptr != pEcuElement )
    {
        //1. Node Id
        ouTableItem.m_nNode = pEcuElement->GetUniqueId();

        //2. Fill DataBytes
        EcuProperties ouEcuProperties;
        pEcuElement->GetProperties(ouEcuProperties);
        if ( ouEcuProperties.m_eEcuType == eLIN_Slave)
        {
            ouTableItem.m_eDiagType = ouCommand.m_eDiagType;
            ouTableItem.m_chDataBytes[defLIN_NAD_INDEX] = (unsigned char)ouEcuProperties.m_ouSlavePros.m_nConfiguredNAD;
            ouTableItem.m_chDataBytes[defLIN_PCI_INDEX] = 0x6;
            ouTableItem.m_chDataBytes[defLIN_SID_INDEX] = defLIN_SID_SAVE_CONFIG;
            memset( &ouTableItem.m_chDataBytes[3], 0XFF, 5 );
        }
        else
        {
            ecRetVal = EC_FAILURE;
        }
    }
    return ecRetVal;
}

ERRORCODE CScheduleTableParser::FillFreeFormatTableItem(ldfScheduleTableCommand& ouCommand, CSheduleTableItem& ouTableItem)
{
    //Syntax: { <D1>, <D2>, <D3>, <D4>, <D5>, <D6>, <D7>, <D8> }
    //1. FrameId
    auto itrFrame = m_mapFrameUID.find("MasterReq");

    if ( m_mapFrameUID.end() == itrFrame )
    {
        return EC_FAILURE;
    }

    ouTableItem.m_nFrameId = itrFrame->second;

    memcpy( ouTableItem.m_chDataBytes, ouCommand.m_chDataBytes, sizeof(ouTableItem.m_chDataBytes) );
    return EC_SUCCESS;
}

ERRORCODE CScheduleTableParser::FillAssignFrameRangeTableItem(ldfScheduleTableCommand& ouCommand, CSheduleTableItem& ouTableItem)
{
    //Syntax: {<node_name>, <frame_index> (, <frame_PID>, <frame_PID>, <frame_PID>, <frame_PID>)}
    //1. FrameId
    auto itrFrame = m_mapFrameUID.find("MasterReq");

    if ( m_mapFrameUID.end() == itrFrame )
    {
        return EC_FAILURE;
    }

    ouTableItem.m_nFrameId = itrFrame->second;

    IEcu* pEcuElement;
    IFrame* pFrameElement;
    unsigned int unId = 0xFF;
    m_pouLDFCluster->GetEcu(ouCommand.m_strNodeName, &pEcuElement);

    ERRORCODE ecRetVal = EC_SUCCESS;
    if ( nullptr != pEcuElement )
    {
        EcuProperties ouEcuProperties;
        pEcuElement->GetProperties(ouEcuProperties);
        if ( ouEcuProperties.m_eEcuType == eLIN_Slave)
        {
            //1. Node Id
            ouTableItem.m_nNode = pEcuElement->GetUniqueId();

            //2. Fill DataBytes
            ouTableItem.m_eDiagType = ouCommand.m_eDiagType;
            ouTableItem.m_chDataBytes[defLIN_NAD_INDEX] = ouEcuProperties.m_ouSlavePros.m_nInitialNAD;
            ouTableItem.m_chDataBytes[defLIN_PCI_INDEX] = 0x6;
            ouTableItem.m_chDataBytes[defLIN_SID_INDEX] = defLIN_SID_ASSIGN_FRAME_RANGE;


            ouTableItem.m_chDataBytes[3] = ouCommand.m_chDataBytes[0];
            ouTableItem.m_chDataBytes[4] = ouCommand.m_chDataBytes[1];
            ouTableItem.m_chDataBytes[5] = ouCommand.m_chDataBytes[2];
            ouTableItem.m_chDataBytes[6] = ouCommand.m_chDataBytes[3];
            ouTableItem.m_chDataBytes[7] = ouCommand.m_chDataBytes[4];
            if ( -1 == (int)ouCommand.m_chDataBytes[1] )
            {
                auto itrConfigFrame = ouEcuProperties.m_ouSlavePros.m_nConfiurableFrameIdList.begin();
                std::advance(itrConfigFrame, ouCommand.m_chDataBytes[0]);
                int nIndex = 4;
                for ( ; itrConfigFrame != ouEcuProperties.m_ouSlavePros.m_nConfiurableFrameIdList.end() && nIndex < 8 ; itrConfigFrame++ )
                {
                    if ( EC_SUCCESS == m_pouLDFCluster->GetElement(eFrameElement, itrConfigFrame->m_uidFrame, (IElement**)&pFrameElement))
                    {
                        ouTableItem.m_nFrameId = pFrameElement->GetUniqueId();
                        pFrameElement->GetFrameId(unId);
                        ouTableItem.m_chDataBytes[nIndex] = ucCalculatePID(unId);
                        nIndex++;
                    }
                    else
                    {
                        return EC_FAILURE;
                    }
                }
            }
        }
        return EC_SUCCESS;
    }
    return EC_NA;
}

ERRORCODE CScheduleTableParser::FillReadByIdentifierTableItem(ldfScheduleTableCommand& ouCommand, CSheduleTableItem& ouTableItem)
{
    //1. Node Id
    ouTableItem.m_nNode = INVALID_UID_ELEMENT;

    auto itrFrame = m_mapFrameUID.find("MasterReq");

    if ( m_mapFrameUID.end() == itrFrame )
    {
        return EC_FAILURE;
    }

    ouTableItem.m_nFrameId = itrFrame->second;
    //2. Fill DataBytes
    ouTableItem.m_eDiagType = ouCommand.m_eDiagType;
    memcpy(ouTableItem.m_chDataBytes, ouCommand.m_chDataBytes, sizeof(ouTableItem.m_chDataBytes));
    ouTableItem.m_chDataBytes[1] = 0x6;
    ouTableItem.m_chDataBytes[2] = defLIN_SID_READ_BY_IDENTIFIER;

    return EC_SUCCESS;
}

unsigned char CScheduleTableParser::ucCalculatePID(unsigned char ucId)
{
    char chChecksum = ucId;
    char chMask[8], byte[8];
    for(int nIndex = 0; nIndex < 8; nIndex++)
    {
        chMask[nIndex] = nIndex+1;
        byte[nIndex] = ((chChecksum) >> nIndex) & 1;
    }

    char chP0, chP1;
    // chP0 = ID0+ID1+ID2+ID4
    chP0 = byte[0] ^ byte[1] ^ byte[2] ^ byte[4];


    // chP1 = ID1+ID3+ID4+ID5
    chP1 = byte[1] + byte[3] + byte[4] + byte[5];

    chP1 = ~chP1;

    //00010110 -> p00010110
    chP0 = chP0 << 6;
    chP1 = chP1 << 7;
    chChecksum = chChecksum + chP0 + chP1;
    ucId = chChecksum;
    return ucId;
}