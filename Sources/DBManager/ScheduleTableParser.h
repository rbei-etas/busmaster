#pragma once

#include "LDFParserHelper.h"
class CScheduleTableParser;
typedef ERRORCODE (CScheduleTableParser::*pFillTableItem)( ldfScheduleTableCommand& , CSheduleTableItem& );

class CScheduleTableParser
{
public:
    CScheduleTableParser(LDFCluster* pouCluster, std::map<std::string, UID_ELEMENT>& m_mapFrameUID);
    ~CScheduleTableParser(void);

    ERRORCODE GetScheduleTable(IScheduleTable* pScheduleTable, ldfScheduleTable& ouScheduleTable);

    pFillTableItem m_pFillTableItem[eDiagType::eLIN_SID_ALL];
private:
    LDFCluster* m_pouLDFCluster;
    std::map<std::string, UID_ELEMENT>& m_mapFrameUID;

    ERRORCODE FillScheduleTableItem(ldfScheduleTableCommand& ouCommand, CSheduleTableItem& ouTableItem);
    ERRORCODE FillNormalTableItem(ldfScheduleTableCommand& ouCommand, CSheduleTableItem& ouTableItem);
    ERRORCODE FillMasterTableItem(ldfScheduleTableCommand& ouCommand, CSheduleTableItem& ouTableItem);
    ERRORCODE FillAssignFrameIdTableItem(ldfScheduleTableCommand& ouCommand, CSheduleTableItem& ouTableItem);       //For AssignFrameId, unAssignFrameId
    ERRORCODE FillAssignNADIdTableItem(ldfScheduleTableCommand& ouCommand, CSheduleTableItem& ouTableItem);
    ERRORCODE FillConditionalChnageNADIdTableItem(ldfScheduleTableCommand& ouCommand, CSheduleTableItem& ouTableItem);
    ERRORCODE FillDataDumpTableItem(ldfScheduleTableCommand& ouCommand, CSheduleTableItem& ouTableItem);
    ERRORCODE FillSaveConfigTableItem(ldfScheduleTableCommand& ouCommand, CSheduleTableItem& ouTableItem);
    ERRORCODE FillFreeFormatTableItem(ldfScheduleTableCommand& ouCommand, CSheduleTableItem& ouTableItem);
    ERRORCODE FillAssignFrameRangeTableItem(ldfScheduleTableCommand& ouCommand, CSheduleTableItem& ouTableItem);
    ERRORCODE FillReadByIdentifierTableItem(ldfScheduleTableCommand& ouCommand, CSheduleTableItem& ouTableItem);
    unsigned char ucCalculatePID(unsigned char ucId);

    ERRORCODE CreateDiagnosticMessages();
    ERRORCODE AddMasterReq();
    ERRORCODE AddSlaveResp();
};

