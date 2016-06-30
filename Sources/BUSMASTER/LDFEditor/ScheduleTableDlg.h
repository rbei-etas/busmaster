#pragma once
#include <QDialog>
#include "ui_ScheduleTableDlg.h"
#include "ICluster.h"
#include "Defines.h"
#include "qcombobox.h"
#include "ScheduleComboWidget.h"

enum eScheduleCommand_Type
{
    eUnConditional = 0,
    eEventTriggered = 1,
    eSporadic = 2,
    eAssignNAD = 3,
    eConditionalChangeNAD = 4,
    eAssignFrameIdRange = 5,
    eAssignFrameId = 6,
    eUnAssignFrameId = 7,
    eReadByIdentifier = 8,
    eSaveConfiguration = 9,
    eDataDump = 10,
    eFreeFormat = 11,
    eMasterReq = 12,
    eSlaveResp = 13,
    eCommandTotal,
    eCommandInvalid
};




Q_DECLARE_METATYPE(CLINSheduleTableItem);

class ScheduleTableDlg :public QDialog
{
    Q_OBJECT
public:
    ScheduleTableDlg(IScheduleTable**, eMode ouMode, QWidget* parent = 0);
    ~ScheduleTableDlg(void);

private:
    Ui::ScheduleTableDlg ui;
    eMode m_ouMode;
    ICluster* m_pouLDFCluster;
    IScheduleTable** m_pouSchedTable;
    int m_nPrevSelRow;
    UID_ELEMENT m_DiagnosticuID[2];
    std::map<std::string, std::string> m_TableNameMap;
private:
    void vPrepareUI();
    void vPrepareUIForNewMode();
    void vPrepareUIForEditMode();
    void vPrepareValidations();
    ERRORCODE vFillFramesCombo(int nRow, ScheduleComboWidget** pComboBox);
    QString nGetFrameLength(FrameProps ouFrameProps);
    eScheduleCommand_Type vGetFrameType(eFrameType ouFrameType, std::string omstrFrameName);
    void vPopulateSelFrameDetails();
    void vPopulateSelFrameDetails(int nIndex);
    void vPopulatePropertyView(CLINSheduleTableItem& ouSchedTableItem);
    void vPopulateAssignNADProperties(CLINSheduleTableItem&);
    void vPopulateCondChangeNAD(CLINSheduleTableItem&);
    void vPopulateFreeFormat(CLINSheduleTableItem&);
    void vPopulateSaveConfig(CLINSheduleTableItem&);
    void vPopulateDataDump(CLINSheduleTableItem&);
    ERRORCODE vPopulateAssignFrameId(CLINSheduleTableItem& ouSchedTableItem);
    void vPopulateAssignFrameIdRange(CLINSheduleTableItem& ouSchedTableItem);
    void vPopulateReadByIdentifier(CLINSheduleTableItem& ouSchedTableItem);
    ERRORCODE vIfFramesExists();
    void vUpdateScheduleTableItems(std::list<CLINSheduleTableItem>& lstScheduleTableItem);

    int nValidateValues();
private slots:
    void OnButtonClickOk();
    void onComboSelectionChange(int nRow, int nIndex);
    void onFrameSelectionChange();
    void onButtonClickAdd(bool);
    void onButtonClickDelete(bool);
    void onComboAssignNADNodeChange(int nIndex);
    void onComboAssignIdRangeNodeChange(int nIndex);
    void onCondNADEditChange(const QString&);
    void onFreeFormatEditChange(const QString&);
    void onReadByIdEditChange(const QString&);
    void onComboSaveConfigChange(int nIndex);
    void onAssignDataDumpEditChange(const QString&);
    void onAssignFrameIdEditChange(const QString&);
    void onAssignFrameIdRangeEditChange(const QString&);
    void onComboAssignFrameIdNodeChange(int nIndex);
    void onComboAssignFrameIdFrameChange(int nIndex);
    void onFrameComboClicked(int nRow, int nCol);
    void onComboDataDumpChange(int nIndex);
    void onScheduleItemDragged(int, int , int);
    void onDelayEditChange(const QString&);
};

