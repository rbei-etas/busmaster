#include "ScheduleTableDlg.h"
#include "LDFDatabaseManager.h"
#include "IClusterProps.h"
#include "LDFUtility.h"
#include "qcombobox.h"
#include <iostream>
#include <sstream>      // std::ostringstream
#include "qmessagebox.h"
#include "LDFTagDefines.h"
#include "LineEditWidget.h"
#include "qheaderview.h"

#define defSCHEDULE_DELAY           10.000000
std::string m_omstrScheduleCmdString[eCommandTotal] = {"Unconditionl Frame",
        "EventTriggered",
        "Sporadic",
        "AssignNAD",
        "ConditionalChangeNAD",
        "AssignFrameIdRange",
        "AssignFrameId",
        "UnAssignFrameId",
        "ReadByIdentifier",
        "SaveConfiguration",
        "DataDump",
        "FreeFormat",
        "MasterReq",
        "SlaveResp"
                                                      };

ScheduleTableDlg::ScheduleTableDlg(IScheduleTable** pouScheduleTable,eMode ouMode, QWidget* parent)
{
    m_ouMode = ouMode;
    m_pouSchedTable = pouScheduleTable;
    m_pouLDFCluster = LDFDatabaseManager::GetDatabaseManager()->GetLDFCluster();
    ui.setupUi(this);
    m_nPrevSelRow = -1;


    setWindowTitle("Create Schedule Table");

    connect(ui.buttonOk, SIGNAL(accepted()), this, SLOT(OnButtonClickOk()));
    connect(ui.buttonOk, SIGNAL(rejected()), this, SLOT(reject()));
    connect(ui.tableFrames, SIGNAL(itemSelectionChanged()), this, SLOT(onFrameSelectionChange()));
    connect(ui.btnAdd, SIGNAL(clicked(bool)), this, SLOT(onButtonClickAdd(bool)));
    connect(ui.btnDelete, SIGNAL(clicked(bool)), this, SLOT(onButtonClickDelete(bool)));
    connect(ui.tableFrames, SIGNAL(onItemDragged(int,int, int)), this, SLOT(onScheduleItemDragged(int,int,int)));

    ui.tableFrames->verticalHeader()->setDefaultSectionSize(24);
    vPrepareValidations();
    vPrepareUI();
    QIcon ouWindowIcon;
    ouWindowIcon.addFile(":/LDFEditorIcons/Resources/icons/16x16/ScheduleTable.png", QSize(16, 16));
    ouWindowIcon.addFile(":/LDFEditorIcons/Resources/icons/22x22/ScheduleTable.png", QSize(16, 16));
    ouWindowIcon.addFile(":/LDFEditorIcons/Resources/icons/32x32/ScheduleTable.png", QSize(16, 16));

    setWindowIcon(ouWindowIcon);


}


ScheduleTableDlg::~ScheduleTableDlg(void)
{
}


void ScheduleTableDlg::onScheduleItemDragged(int nLogical, int nOld, int nNew)
{
    IScheduleTable* pScheduleTable = *m_pouSchedTable;
    if(nullptr == pScheduleTable)
    {
        return;
    }

    ScheduleTableProps ouScheduleTableProps;
    pScheduleTable->GetProperties(ouScheduleTableProps);

    //ui.tableFrames->setRowCount(ouScheduleTableProps.m_ouCSheduleTableItem.size());

    std::list<CSheduleTableItem> tempScheduleTableItem;
    int nRows = ui.tableFrames->rowCount();
    CSheduleTableItem ouTableItem;
    for(int unIndex = 0; unIndex < nRows; unIndex++)
    {
        int nVisualRow = ui.tableFrames->verticalHeader()->logicalIndex(unIndex);
        QTableWidgetItem* pItem = ui.tableFrames->item(nVisualRow,0);
        ouTableItem = pItem->data(Qt::UserRole).value<CSheduleTableItem>();

        tempScheduleTableItem.push_back(ouTableItem);
    }

    ui.tableFrames->setRowCount(0);
    vUpdateScheduleTableItems(tempScheduleTableItem);

    /*EcuProperties ouEcuProps;
    m_pCurrentEcu->GetProperties(ouEcuProps);
    if ( ouEcuProps.m_eEcuType != eLIN_Slave )
    {
        return;
    }

    LDFDatabaseManager::GetDatabaseManager()->setDocumentModified(true);

    ouEcuProps.m_ouSlavePros.m_nConfiurableFrameIdList.clear();

    int nRows = rowCount();

    for ( int  i = 0 ; i < nRows; i++ )
    {
        int nVisualRow = verticalHeader()->logicalIndex(i);
        QTableWidgetItem* pItem = item(nVisualRow,0);
        ConfigFrameDetails ouConfigFrame = pItem->data(Qt::UserRole).value<ConfigFrameDetails>();

        ouEcuProps.m_ouSlavePros.m_nConfiurableFrameIdList.push_back(ouConfigFrame);
    }
    m_pCurrentEcu->SetProperties(ouEcuProps);
    DisplayEcuAdditionalProps(m_pCurrentEcu);

    QTableWidgetItem* pItem = item(nNew,0);
    if ( nullptr != pItem )
    {
        setCurrentItem(pItem);
    }*/
}

void ScheduleTableDlg::vPrepareValidations()
{
    ui.editScheduleName->setValidator(new QRegExpValidator(QRegExp(defIdentifier_RegExp)));

    std::map<UID_ELEMENT, IElement*> mapElement;
    std::string strName;
    m_pouLDFCluster->GetElementList(eScheduleTableElement, mapElement);
    m_TableNameMap.clear();
for ( auto itr : mapElement )
    {
        itr.second->GetName(strName);
        m_TableNameMap[strName] = strName;
    }
}

void ScheduleTableDlg::vPrepareUI()
{
    ui.editScheduleName->setValidator(new QRegExpValidator(QRegExp(defIdentifier_RegExp)));

    ui.btnAdd->setEnabled(false);
    ui.btnDelete->setEnabled(false);

    if(EC_SUCCESS == vIfFramesExists())
    {
        ui.btnAdd->setEnabled(true);
        ui.btnDelete->setEnabled(true);
    }

    if(eNew == m_ouMode)
    {
        vPrepareUIForNewMode();
    }
    else if(eEdit == m_ouMode)
    {
        vPrepareUIForEditMode();
    }

    ui.tableFrames->verticalHeader()->setSectionsMovable(true);
}

void ScheduleTableDlg::vPrepareUIForNewMode()
{

}

void ScheduleTableDlg::vPrepareUIForEditMode()
{
    IScheduleTable* pScheduleTable = *m_pouSchedTable;
    if(nullptr == pScheduleTable)
    {
        return;
    }

    std::string omStrSchedName;
    pScheduleTable->GetName(omStrSchedName);

    QString strTitle = "Edit Schedule Table - ";
    strTitle+= + omStrSchedName.c_str();
    setWindowTitle(strTitle);


    auto itr = m_TableNameMap.find(omStrSchedName);
    if ( m_TableNameMap.end() != itr )
    {
        m_TableNameMap.erase(itr);
    }

    ui.editScheduleName->setText(QString::fromStdString(omStrSchedName));

    std::string strFrameName;
    FrameProps ouFrameProps;
    ScheduleTableProps ouScheduleTableProps;
    pScheduleTable->GetProperties(ouScheduleTableProps);
    QVariant qVar;
    ui.tableFrames->setRowCount(ouScheduleTableProps.m_ouCSheduleTableItem.size());
    std::string strFrameType;
    IFrame* pouMsg = nullptr;
    eFrameType oueFrameType;
    int nRow = 0;

    bool bFrameExists = false;
    QList<QVariant> ouColumns;

    vUpdateScheduleTableItems(ouScheduleTableProps.m_ouCSheduleTableItem);
    //for ( auto itr : ouScheduleTableProps.m_ouCSheduleTableItem )
    //{
    //  m_pouLDFCluster->GetElement(eFrameElement, itr.m_nFrameId, (IElement**)&pouMsg);

    //  int nFrameLength = 8;
    //  if(nullptr == pouMsg)
    //  {
    //      continue;
    //  }
    //
    //  pouMsg->GetName(strFrameName);
    //  pouMsg->GetProperties(ouFrameProps);
    //  pouMsg->GetFrameType(oueFrameType);


    //  if ( itr.m_eDiagType == eLIN_NORMAL_FRAME_ID )
    //  {
    //      //nFrameLength = nGetFrameLength(ouFrameProps).toInt();
    //      GetString(oueFrameType, strFrameType);
    //  }
    //  else if ( itr.m_eDiagType == eLIN_MASTER_FRAME_ID )
    //  {
    //      strFrameType = "Master Request";
    //      pouMsg->GetName(strFrameName);
    //  }
    //  else if ( itr.m_eDiagType == eLIN_SLAVE_FRAME_ID )
    //  {
    //      strFrameType = "Slave Response";
    //      pouMsg->GetName(strFrameName);
    //  }

    //  else if ( itr.m_eDiagType >= eLIN_SID_ASSIGN_FRAME_ID && itr.m_eDiagType <= eLIN_SID_READ_BY_IDENTIFIER )
    //  {
    //      strFrameType = "Master Request";
    //      GetString(itr.m_eDiagType, strFrameName);
    //  }
    //
    //
    //  ScheduleComboWidget *pComboBox = nullptr;
    //  vFillFramesCombo(nRow, &pComboBox);

    //  ouColumns.clear();

    //  LineEditWidget *pLineEdit = new LineEditWidget(nRow, ui.tableFrames);
    //  pLineEdit->setValidator(new QDoubleValidator());
    //  pLineEdit->setFrame(false);
    //
    //
    //  ouColumns.push_back(QVariant(QString::fromStdString(strFrameName.c_str())));
    //  ouColumns.push_back(QVariant(strFrameType.c_str()));
    //
    //  ouColumns.push_back(QVariant(nGetFrameLength(ouFrameProps)));
    //  ouColumns.push_back(QVariant());
    //
    //  pLineEdit->setText(GetString(itr.m_dDelay));
    //  ui.tableFrames->InsertRow(nRow, ouColumns);
    //  pComboBox->setCurrentText((QString::fromStdString(strFrameName.c_str())));
    //  ui.tableFrames->setCellWidget(nRow, 0, pComboBox);
    //  qVar.setValue(itr);
    //  ui.tableFrames->item(nRow, 0)->setData(Qt::UserRole, qVar);
    //  ui.tableFrames->setCellWidget(nRow++, 3, pLineEdit);
    //
    //  connect(pComboBox, SIGNAL(onScheduleComboChange(int, int)), this, SLOT(onComboSelectionChange(int, int)));
    //  ouColumns.clear();
    //}

}

void ScheduleTableDlg::vUpdateScheduleTableItems(std::list<CSheduleTableItem>& lstScheduleTableItem)
{
    IFrame* pouMsg = nullptr;
    eFrameType oueFrameType;
    std::string strFrameName, strFrameType;
    FrameProps ouFrameProps;
    int nRow = 0;
    QList<QVariant> ouColumns;
    QVariant qVar;
    ui.tableFrames->setRowCount(lstScheduleTableItem.size());
for ( auto itr : lstScheduleTableItem)
    {
        m_pouLDFCluster->GetElement(eFrameElement, itr.m_nFrameId, (IElement**)&pouMsg);

        int nFrameLength = 8;
        if(nullptr == pouMsg)
        {
            continue;
        }

        pouMsg->GetName(strFrameName);
        pouMsg->GetProperties(ouFrameProps);
        pouMsg->GetFrameType(oueFrameType);


        if ( itr.m_eDiagType == eLIN_NORMAL_FRAME_ID )
        {
            //nFrameLength = nGetFrameLength(ouFrameProps).toInt();
            GetString(oueFrameType, strFrameType);
        }
        else if ( itr.m_eDiagType == eLIN_MASTER_FRAME_ID )
        {
            strFrameType = "Master Request";
            pouMsg->GetName(strFrameName);
        }
        else if ( itr.m_eDiagType == eLIN_SLAVE_FRAME_ID )
        {
            strFrameType = "Slave Response";
            pouMsg->GetName(strFrameName);
        }

        else if ( itr.m_eDiagType >= eLIN_SID_ASSIGN_FRAME_ID && itr.m_eDiagType <= eLIN_SID_READ_BY_IDENTIFIER )
        {
            strFrameType = "Master Request";
            GetString(itr.m_eDiagType, strFrameName);
        }


        ScheduleComboWidget* pComboBox = nullptr;
        vFillFramesCombo(nRow, &pComboBox);

        ouColumns.clear();

        LineEditWidget* pLineEdit = new LineEditWidget(nRow, ui.tableFrames);
        pLineEdit->setValidator(new QDoubleValidator());
        pLineEdit->setFrame(false);


        ouColumns.push_back(QVariant(QString::fromStdString(strFrameName.c_str())));
        ouColumns.push_back(QVariant(strFrameType.c_str()));

        ouColumns.push_back(QVariant(nGetFrameLength(ouFrameProps)));
        ouColumns.push_back(QVariant());

        pLineEdit->setText(GetString(itr.m_dDelay));
        ui.tableFrames->InsertRow(nRow, ouColumns);
        pComboBox->setCurrentText((QString::fromStdString(strFrameName.c_str())));
        ui.tableFrames->setCellWidget(nRow, 0, pComboBox);
        qVar.setValue(itr);
        ui.tableFrames->item(nRow, 0)->setData(Qt::UserRole, qVar);
        ui.tableFrames->setCellWidget(nRow++, 3, pLineEdit);

        connect(pLineEdit, SIGNAL(textChanged ( const QString&)), this, SLOT(onDelayEditChange(const QString&)));

        connect(pComboBox, SIGNAL(onScheduleComboChange(int, int)), this, SLOT(onComboSelectionChange(int, int)));
        ouColumns.clear();
    }
}

void ScheduleTableDlg::onDelayEditChange(const QString& str)
{
    int nRow = ui.tableFrames->currentRow();
    if(nRow >= 0)
    {
        QVariant qVar;
        CSheduleTableItem ouSchedTableItem = ui.tableFrames->item( nRow, 0)->data(Qt::UserRole).value<CSheduleTableItem>();
        ouSchedTableItem.m_dDelay = ((LineEditWidget*)ui.tableFrames->cellWidget(nRow, 3))->text().toDouble();

        qVar.setValue(ouSchedTableItem);
        ui.tableFrames->item( nRow, 0)->setData(Qt::UserRole, qVar);
    }
}

QString ScheduleTableDlg::nGetFrameLength(FrameProps ouFrameProps)
{
    switch(ouFrameProps.m_eFrameType)
    {
        case eLIN_Unconditional:
            return GetString(ouFrameProps.m_ouLINUnConditionFrameProps.m_nLength, 10);
            break;
        case eLIN_Sporadic:
            return "";
            break;
        case eLIN_EventTriggered:
            return "";
            break;
        case eLIN_Diagnostic:
            return GetString(ouFrameProps.m_ouLINDiagnosticFrameProps.m_nLength, 10);
            break;
        default:
            return "8";
            break;
    }
}

ERRORCODE ScheduleTableDlg::vFillFramesCombo(int nRow, ScheduleComboWidget** pComboBox)
{
    ERRORCODE ecCode = EC_FAILURE;
    LIN_Settings ouLinSettings;
    m_pouLDFCluster->GetProperties(eLINClusterProperties, &ouLinSettings);

    FrameProps ouFrameProps;
    std::map<UID_ELEMENT, IElement*> mapFrames;
    m_pouLDFCluster->GetElementList(eFrameElement, mapFrames);
    ScheduleComboWidget* pTempComboBox = new ScheduleComboWidget(nRow, this);
    IFrame* pFrame = nullptr;
    std::string strFrameName;
    m_DiagnosticuID[0] =  INVALID_UID_ELEMENT;
    m_DiagnosticuID[1] =  INVALID_UID_ELEMENT;
    UID_ELEMENT uidFrame = INVALID_UID_ELEMENT;
for(auto itrFrame : mapFrames)
    {
        pFrame = (IFrame*)itrFrame.second;
        pFrame->GetName(strFrameName);
        pFrame->GetProperties(ouFrameProps);
        uidFrame = pFrame->GetUniqueId();
        if ( ouFrameProps.m_eFrameType == eLIN_Diagnostic )
        {
            if ( ouFrameProps.m_ouLINDiagnosticFrameProps.m_eDiagType == eLIN_MASTER_FRAME_ID )
            {
                m_DiagnosticuID[0] = uidFrame;
            }
            else
            {
                m_DiagnosticuID[1] = uidFrame;
            }
        }
        ecCode = EC_SUCCESS;
        pTempComboBox->addItem(QString::fromStdString(strFrameName), QVariant(uidFrame));
    }

    if(m_DiagnosticuID[0] != INVALID_UID_ELEMENT)
    {
        uidFrame = m_DiagnosticuID[0];
        if ( ouLinSettings.m_dProtocolVers == defVersion_2_0)
        {
            ecCode = EC_SUCCESS;
            GetString(eLIN_SID_ASSIGN_NAD_ID, strFrameName);
            pTempComboBox->addItem(QString::fromStdString(strFrameName), QVariant(uidFrame));
            GetString(eLIN_SID_ASSIGN_FRAME_ID, strFrameName);
            pTempComboBox->addItem(QString::fromStdString(strFrameName), QVariant(uidFrame));
            GetString(eLIN_SID_FREEFORMAT, strFrameName);
            pTempComboBox->addItem(QString::fromStdString(strFrameName), QVariant(uidFrame));
            GetString(eLIN_SID_READ_BY_IDENTIFIER, strFrameName);
            pTempComboBox->addItem(QString::fromStdString(strFrameName), QVariant(uidFrame));
        }
        else if ( ouLinSettings.m_dProtocolVers == defVersion_2_1)
        {
            ecCode = EC_SUCCESS;
            GetString(eLIN_SID_ASSIGN_NAD_ID, strFrameName);
            pTempComboBox->addItem(QString::fromStdString(strFrameName), QVariant(uidFrame));
            GetString(eLIN_SID_COND_CHANGE_NAD, strFrameName);
            pTempComboBox->addItem(QString::fromStdString(strFrameName), QVariant(uidFrame));
            GetString(eLIN_SID_ASSIGN_FRAME_RANGE, strFrameName);
            pTempComboBox->addItem(QString::fromStdString(strFrameName), QVariant(uidFrame));
            GetString(eLIN_SID_ASSIGN_FRAME_ID, strFrameName);
            pTempComboBox->addItem(QString::fromStdString(strFrameName), QVariant(uidFrame));
            GetString(eLIN_SID_SAVE_CONFIG, strFrameName);
            pTempComboBox->addItem(QString::fromStdString(strFrameName), QVariant(uidFrame));
            GetString(eLIN_SID_DATA_DUMP, strFrameName);
            pTempComboBox->addItem(QString::fromStdString(strFrameName), QVariant(uidFrame));
            GetString(eLIN_SID_FREEFORMAT, strFrameName);
            pTempComboBox->addItem(QString::fromStdString(strFrameName), QVariant(uidFrame));
            GetString(eLIN_SID_READ_BY_IDENTIFIER, strFrameName);
            pTempComboBox->addItem(QString::fromStdString(strFrameName), QVariant(uidFrame));

        }
    }
    *pComboBox = pTempComboBox;

    return ecCode;
}

ERRORCODE ScheduleTableDlg::vIfFramesExists()
{
    ERRORCODE ecCode = EC_FAILURE;
    LIN_Settings ouLinSettings;
    m_pouLDFCluster->GetProperties(eLINClusterProperties, &ouLinSettings);

    FrameProps ouFrameProps;
    std::map<UID_ELEMENT, IElement*> mapFrames;
    m_pouLDFCluster->GetElementList(eFrameElement, mapFrames);
    IFrame* pFrame = nullptr;
    std::string strFrameName;
    m_DiagnosticuID[0] =  INVALID_UID_ELEMENT;
    m_DiagnosticuID[1] =  INVALID_UID_ELEMENT;
    UID_ELEMENT uidFrame = INVALID_UID_ELEMENT;
for(auto itrFrame : mapFrames)
    {
        pFrame = (IFrame*)itrFrame.second;
        pFrame->GetName(strFrameName);
        pFrame->GetProperties(ouFrameProps);
        uidFrame = pFrame->GetUniqueId();
        if ( ouFrameProps.m_eFrameType == eLIN_Diagnostic )
        {
            if ( ouFrameProps.m_ouLINDiagnosticFrameProps.m_eDiagType == eLIN_MASTER_FRAME_ID )
            {
                m_DiagnosticuID[0] = uidFrame;
            }
            else
            {
                m_DiagnosticuID[1] = uidFrame;
            }
        }
        ecCode = EC_SUCCESS;
    }

    if(m_DiagnosticuID[0] != INVALID_UID_ELEMENT)
    {
        uidFrame = m_DiagnosticuID[0];
        if ( ouLinSettings.m_dProtocolVers == defVersion_2_0)
        {
            ecCode = EC_SUCCESS;
        }
        else if ( ouLinSettings.m_dProtocolVers == defVersion_2_1)
        {
            ecCode = EC_SUCCESS;
        }
    }

    return ecCode;
}

void ScheduleTableDlg::onComboSelectionChange(int nRow, int nIndex)
{
    ui.tableFrames->selectRow(nRow);
    vPopulateSelFrameDetails(nIndex);
    m_nPrevSelRow = nRow;
}

eScheduleCommand_Type ScheduleTableDlg::vGetFrameType(eFrameType ouFrameType, std::string omstrFrameName)
{
    switch (ouFrameType)
    {
        case eLIN_Unconditional:
            return eUnConditional;
            break;
        case eLIN_Sporadic:
            return eSporadic;
            break;
        case eLIN_EventTriggered:
            return eEventTriggered;
            break;
        case eLIN_Diagnostic:
            if(omstrFrameName == defLINMasterFrameName)
            {
                return eMasterReq;
            }
            else if(omstrFrameName == defLINSlaveFrameName)
            {
                return eSlaveResp;
            }
            break;
        default:

            for(int eType = eAssignNAD; eType < eCommandTotal; eType++)
            {
                if(omstrFrameName == m_omstrScheduleCmdString[eType])
                {
                    return (eScheduleCommand_Type)eType;
                }
            }

            break;
    }
}


int ScheduleTableDlg::nValidateValues()
{
    std::string strName = ui.editScheduleName->text().toStdString();
    if ( strName == "" )
    {
        QMessageBox::critical(this, "Error", "Name Should Not Be Empty", QMessageBox::Ok);
        return 1;
    }
    if ( m_TableNameMap.find(strName) != m_TableNameMap.end())
    {
        QMessageBox::critical(this, "Error", "Name Already Exists", QMessageBox::Ok);
        return 1;
    }
    return 0;
}

void ScheduleTableDlg::OnButtonClickOk()
{
    if ( 0 == nValidateValues() )
    {
        ScheduleTableProps ouTableProps;
        IScheduleTable* pouTable = nullptr;


        int nRowCount = ui.tableFrames->rowCount();

        for ( int i = 0 ; i  < nRowCount; i++ )
        {
            CSheduleTableItem ouTableItem = ui.tableFrames->item(i, 0)->data(Qt::UserRole).value<CSheduleTableItem>();
            LineEditWidget* pDelayEdit = (LineEditWidget*)ui.tableFrames->cellWidget(i, 3);
            if ( nullptr != pDelayEdit )
            {
                ouTableItem.m_dDelay = pDelayEdit->text().toDouble();
            }
            else
            {
                ouTableItem.m_dDelay = defSCHEDULE_DELAY;
            }
            ouTableProps.m_ouCSheduleTableItem.push_back(ouTableItem);
        }



        if ( m_ouMode == eEdit )
        {
            pouTable = *m_pouSchedTable;
        }
        else
        {
            m_pouLDFCluster->CreateElement(eScheduleTableElement, (IElement**)&pouTable);
            *m_pouSchedTable = pouTable;
        }

        if ( nullptr != pouTable )
        {
            ouTableProps.m_strTableName = ui.editScheduleName->text().toStdString();
            pouTable->SetProperties(ouTableProps);
            pouTable->SetName(ouTableProps.m_strTableName);
        }
        LDFDatabaseManager::GetDatabaseManager()->setDocumentModified(true);
        accept();
    }
}

void ScheduleTableDlg::onFrameSelectionChange()
{
    vPopulateSelFrameDetails();
}

void ScheduleTableDlg::vPopulateSelFrameDetails()
{
    int nSelectedRow = ui.tableFrames->currentRow();

    if(nSelectedRow >= 0)
    {
        ScheduleComboWidget* pComboWidget = (ScheduleComboWidget*)ui.tableFrames->cellWidget(nSelectedRow, 0);
        vPopulateSelFrameDetails(pComboWidget->currentIndex());
    }
}

void ScheduleTableDlg::vPopulateSelFrameDetails(int nIndex)
{
    int nSelectedRow = ui.tableFrames->currentRow();

    if(nSelectedRow >= 0)
    {
        ScheduleComboWidget* pComboWidget = (ScheduleComboWidget*)ui.tableFrames->cellWidget(nSelectedRow, 0);
        UID_ELEMENT uidElement = pComboWidget->itemData(nIndex, Qt::UserRole).value<UID_ELEMENT>();

        CSheduleTableItem omSchedItem = ui.tableFrames->item(nSelectedRow, 0)->data(Qt::UserRole).value<CSheduleTableItem>();
        //memset(omSchedItem.m_chDataBytes, 0, sizeof(omSchedItem.m_chDataBytes));

        IFrame* pFrame;
        eFrameType oueFrameType;
        std::string strFrameType;
        m_pouLDFCluster->GetElement(eFrameElement, uidElement, (IElement**)&pFrame);

        if ( nullptr != pFrame )
        {
            omSchedItem.m_nFrameId = uidElement;
            FrameProps ouFrameProps;
            pFrame->GetProperties(ouFrameProps);

            if ( uidElement != INVALID_UID_ELEMENT )
            {
                if ( uidElement == m_DiagnosticuID[0] )
                {
                    strFrameType = "Master Request";
                    oueFrameType = eLIN_Diagnostic;
                }
                else if ( uidElement == m_DiagnosticuID[1] )
                {
                    strFrameType = "Slave Response";
                    oueFrameType = eLIN_Diagnostic;
                }
                else
                {

                    GetString(ouFrameProps.m_eFrameType, strFrameType);
                }
            }

            ui.tableFrames->item(nSelectedRow, 1)->setText(strFrameType.c_str());
            if(ouFrameProps.m_eFrameType == eLIN_Unconditional || ouFrameProps.m_eFrameType == eLIN_EventTriggered || ouFrameProps.m_eFrameType == eLIN_Sporadic)
            {
                omSchedItem.m_eDiagType = eLIN_NORMAL_FRAME_ID;
                ui.tableFrames->item(nSelectedRow, 2)->setText(nGetFrameLength(ouFrameProps));
            }
            else
            {
                ui.tableFrames->item(nSelectedRow, 2)->setText("");

                eDiagType ouDiagType;
                QString omStrType = ((ScheduleComboWidget*)ui.tableFrames->cellWidget(nSelectedRow, 0))->currentText();
                GetType(omStrType.toStdString(), ouDiagType);
                omSchedItem.m_eDiagType = ouDiagType;

                strFrameType = "Master Request";
                ui.tableFrames->item(nSelectedRow, 1)->setText(strFrameType.c_str());
                ui.tableFrames->item(nSelectedRow, 2)->setText("8");
                omSchedItem.m_nFrameId = m_DiagnosticuID[0];

                if(ouDiagType == eLIN_SLAVE_FRAME_ID )
                {
                    omSchedItem.m_nFrameId = m_DiagnosticuID[1];
                    strFrameType = "Slave Response";
                    ui.tableFrames->item(nSelectedRow, 1)->setText(strFrameType.c_str());
                    ui.tableFrames->item(nSelectedRow, 2)->setText("8");
                }
            }
        }

        QVariant qVar;

        vPopulatePropertyView(omSchedItem);
        qVar.setValue(omSchedItem);
        ui.tableFrames->item(nSelectedRow, 0)->setData(Qt::UserRole, qVar);
    }
}

void ScheduleTableDlg::onButtonClickAdd(bool)
{
    int nRow = ui.tableFrames->rowCount();
    ui.tableFrames->setRowCount(ui.tableFrames->rowCount()+1);
    QList<QVariant> ouColumns;
    ScheduleComboWidget* pComboBox = nullptr;
    vFillFramesCombo(nRow, &pComboBox);
    ouColumns.clear();
    UID_ELEMENT uid =  pComboBox->currentData().value<UID_ELEMENT>();
    CSheduleTableItem ouTableItem;
    ouTableItem.m_nFrameId = uid;
    if ( uid != INVALID_DATA)
    {
        if ( uid == m_DiagnosticuID [ 0 ] )
        {
            ouTableItem.m_eDiagType = eLIN_MASTER_FRAME_ID;
        }
        else if ( uid == m_DiagnosticuID [ 1 ] )
        {
            ouTableItem.m_eDiagType = eLIN_SLAVE_FRAME_ID;
        }
        else
        {
            ouTableItem.m_eDiagType = eLIN_NORMAL_FRAME_ID;
        }

        LineEditWidget* pLineEdit = new LineEditWidget(nRow, ui.tableFrames);
        pLineEdit->setValidator(new QDoubleValidator());
        pLineEdit->setText("50.000000");
        pLineEdit->setFrame(false);

        ouColumns.push_back(QVariant());
        ouColumns.push_back(QVariant());
        ouColumns.push_back(QVariant());
        ouColumns.push_back(QVariant());

        ui.tableFrames->InsertRow(nRow, ouColumns);

        ui.tableFrames->setCellWidget(nRow, 0, pComboBox);
        ui.tableFrames->setCellWidget(nRow, 3, pLineEdit);
        connect(pLineEdit, SIGNAL(textChanged ( const QString&)), this, SLOT(onDelayEditChange(const QString&)));
        connect(pComboBox, SIGNAL(onScheduleComboChange(int, int)), this, SLOT(onComboSelectionChange(int, int)));
        ouColumns.clear();

        ui.tableFrames->selectRow(nRow);
        QVariant ouVar;
        ouVar.setValue(ouTableItem);
        ui.tableFrames->item(nRow, 0)->setData(Qt::UserRole, ouVar);

    }
}

void ScheduleTableDlg::onButtonClickDelete(bool)
{
    int nRowCnt = ui.tableFrames->rowCount();
    ui.tableFrames->removeRow(ui.tableFrames->currentRow());
    nRowCnt = ui.tableFrames->rowCount();

}

void ScheduleTableDlg::vPopulatePropertyView(CSheduleTableItem& ouSchedTableItem)
{
    //ui.tableProperties->clear();
    ui.tableProperties->setRowCount(0);
    IElement* pElement = nullptr;
    eFrameType ouFrameType;
    switch(ouSchedTableItem.m_eDiagType)
    {
        case eLIN_MASTER_FRAME_ID:
            break;
        case eLIN_SID_ASSIGN_NAD_ID:
            vPopulateAssignNADProperties(ouSchedTableItem);
            break;
        case eLIN_SID_COND_CHANGE_NAD:
            vPopulateCondChangeNAD(ouSchedTableItem);
            break;
        case eLIN_SID_FREEFORMAT:
            vPopulateFreeFormat(ouSchedTableItem);
            break;
        case eLIN_SID_DATA_DUMP:
            vPopulateDataDump(ouSchedTableItem);
            break;
        case eLIN_SID_SAVE_CONFIG:
            vPopulateSaveConfig(ouSchedTableItem);
            break;
        case eLIN_SID_READ_BY_IDENTIFIER:
            vPopulateReadByIdentifier(ouSchedTableItem);
            ouSchedTableItem.m_eDiagType = eLIN_SID_READ_BY_IDENTIFIER;
            break;
        case eLIN_SID_UNASSIGN_FRAME_ID:
            if(EC_SUCCESS == vPopulateAssignFrameId(ouSchedTableItem))
            {
                ouSchedTableItem.m_eDiagType = eLIN_SID_UNASSIGN_FRAME_ID;
            }
            break;
        case eLIN_SID_ASSIGN_FRAME_ID:
            if(EC_SUCCESS == vPopulateAssignFrameId(ouSchedTableItem))
            {
                vPopulateAssignFrameId(ouSchedTableItem);
                ouSchedTableItem.m_eDiagType = eLIN_SID_ASSIGN_FRAME_ID;
            }
            break;
        case eLIN_SID_ASSIGN_FRAME_RANGE:
            vPopulateAssignFrameIdRange(ouSchedTableItem);
            break;
        case eLIN_NORMAL_FRAME_ID:
            m_pouLDFCluster->GetElement(eFrameElement, ouSchedTableItem.m_nFrameId, &pElement);

            if(nullptr != pElement)
            {
                ((IFrame*)pElement)->GetFrameType(ouFrameType);

                switch(ouFrameType)
                {
                    case eLIN_Unconditional:
                        break;
                    case eLIN_EventTriggered:
                        break;
                    case eLIN_Sporadic:
                        break;
                    case eLIN_Diagnostic:
                        break;
                }
            }
            break;
    };
}

void ScheduleTableDlg::vPopulateAssignNADProperties(CSheduleTableItem& ouSchedTableItem)
{
    std::map<UID_ELEMENT, IElement*> mapEcus;
    eEcuType ouECUType;
    std::string strName;
    //ui.tableProperties->clear();
    ui.tableProperties->setRowCount(0);
    ui.tableProperties->setRowCount(6);

    int nSelectedRow = ui.tableFrames->currentRow();

    UID_ELEMENT unECUId = ouSchedTableItem.m_nNode;
    std::string strECUName;
    IElement* pElement = nullptr;

    // Node
    QComboBox* pComboBox = new QComboBox(nullptr);
    pComboBox->setFrame(false);

    m_pouLDFCluster->GetElementList(eEcuElement, mapEcus);

    QVariant qVar;
    int nIndex = 0;
for(auto itrECU : mapEcus)
    {
        ((IEcu*)itrECU.second)->GetEcuType(ouECUType);

        if(ouECUType == eLIN_Slave)
        {
            itrECU.second->GetName(strName);
            pComboBox->addItem(QString::fromStdString(strName));
            qVar.setValue(((IEcu*)itrECU.second)->GetUniqueId());
            pComboBox->setItemData(nIndex, qVar, Qt::UserRole);
            nIndex++;
        }
    }

    if(unECUId == INVALID_UID_ELEMENT)
    {
        unECUId = pComboBox->itemData(0, Qt::UserRole).value<UID_ELEMENT>();
    }

    if(EC_SUCCESS == m_pouLDFCluster->GetElement(eEcuElement, unECUId, &pElement))
    {
        pElement->GetName(strECUName);
        pComboBox->setCurrentText(QString::fromStdString(strECUName));
    }

    connect(pComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(onComboAssignNADNodeChange(int)));
    IEcu* pEcu = (IEcu*)pElement;

    EcuProperties ouEcuProps;
    pEcu->GetProperties(ouEcuProps);
    QList<QVariant> ouColumns;
    std::string strColName = "Node Name";
    ouColumns.push_back(QVariant(strColName.c_str()));
    ouColumns.push_back(QVariant());

    ui.tableProperties->InsertRow(0, ouColumns);
    ui.tableProperties->setCellWidget(0, 1, pComboBox);

    ouColumns.clear();
    ouColumns.push_back(QVariant("Initial NAD"));
    ouColumns.push_back(QVariant(ouEcuProps.m_ouSlavePros.m_nInitialNAD));
    ui.tableProperties->InsertRow(1, ouColumns);

    ouColumns.clear();
    ouColumns.push_back(QVariant("Supplier ID"));
    ouColumns.push_back(QVariant(ouEcuProps.m_ouSlavePros.m_nSupplierId));
    ui.tableProperties->InsertRow(2, ouColumns);

    ouColumns.clear();
    ouColumns.push_back(QVariant("Function ID"));
    ouColumns.push_back(QVariant(ouEcuProps.m_ouSlavePros.m_nFunctionId));
    ui.tableProperties->InsertRow(3, ouColumns);

    ouColumns.clear();
    ouColumns.push_back(QVariant("Configure NAD"));
    ouColumns.push_back(QVariant(ouEcuProps.m_ouSlavePros.m_nConfiguredNAD));
    ui.tableProperties->InsertRow(4, ouColumns);

    int nLSBSupplierId, nMSBSupplierId, nLSBFunctionId, nMSBFunctionId;
    nLSBSupplierId = ouEcuProps.m_ouSlavePros.m_nSupplierId & 0xFF;
    nMSBSupplierId = (ouEcuProps.m_ouSlavePros.m_nSupplierId & 0xFF00)>> 8;
    nLSBFunctionId = ouEcuProps.m_ouSlavePros.m_nFunctionId & 0xFF;
    nMSBFunctionId =(ouEcuProps.m_ouSlavePros.m_nFunctionId & 0xFF00)>> 8;

    int nPCI = 0x06, nSID = 0xB0;
    std::ostringstream omStrStream;
    omStrStream << "[" << GetString(ouEcuProps.m_ouSlavePros.m_nInitialNAD).toStdString().c_str() << ","
                <<  GetString(nPCI).toStdString().c_str() << "," << GetString(nSID).toStdString().c_str() << ","
                <<  GetString(nLSBSupplierId).toStdString().c_str() << "," << GetString(nMSBSupplierId).toStdString().c_str()
                << "," << GetString(nLSBFunctionId).toStdString().c_str() << "," << GetString(nMSBFunctionId).toStdString().c_str()
                << "," << GetString(ouEcuProps.m_ouSlavePros.m_nConfiguredNAD).toStdString().c_str()  << "]";
    ouColumns.clear();
    ouColumns.push_back(QVariant("PDU"));
    ouColumns.push_back(QVariant(omStrStream.str().c_str()));
    ui.tableProperties->InsertRow(5, ouColumns);

    ouSchedTableItem.m_nNode = unECUId;
}

void ScheduleTableDlg::onCondNADEditChange(const QString& strText)
{
    if(ui.tableFrames->currentRow() >= 0)
    {
        QVariant qVar;
        CSheduleTableItem ouSchedTableItem = ui.tableFrames->item( ui.tableFrames->currentRow(), 0)->data(Qt::UserRole).value<CSheduleTableItem>();
        ouSchedTableItem.m_chDataBytes[0] =  GetUnsignedInt(((LineEditWidget*)ui.tableProperties->cellWidget(4,1))->text()); // nad
        ouSchedTableItem.m_chDataBytes[3] =  GetUnsignedInt(((LineEditWidget*)ui.tableProperties->cellWidget(0,1))->text()); // id
        ouSchedTableItem.m_chDataBytes[4] =  GetUnsignedInt(((LineEditWidget*)ui.tableProperties->cellWidget(1, 1))->text()); // byte
        ouSchedTableItem.m_chDataBytes[5] =  GetUnsignedInt(((LineEditWidget*)ui.tableProperties->cellWidget(2, 1))->text()); // mask
        ouSchedTableItem.m_chDataBytes[6] =  GetUnsignedInt(((LineEditWidget*)ui.tableProperties->cellWidget(3, 1))->text()); // invert
        ouSchedTableItem.m_chDataBytes[7] =  GetUnsignedInt(((LineEditWidget*)ui.tableProperties->cellWidget(5, 1))->text()); // new nad

        ouSchedTableItem.m_eDiagType = eLIN_SID_COND_CHANGE_NAD;
        qVar.setValue(ouSchedTableItem);
        ui.tableFrames->item( ui.tableFrames->currentRow(), 0)->setData(Qt::UserRole, qVar);

        int nPCI = 0x06, nSID = 0xB3;
        std::ostringstream omStrStream;
        omStrStream << "[" << GetString(ouSchedTableItem.m_chDataBytes[0]).toStdString() << "," <<  GetString(nPCI).toStdString().c_str()
                    << "," << GetString(nSID).toStdString().c_str() << "," << GetString(ouSchedTableItem.m_chDataBytes[3]).toStdString()
                    << "," << GetString(ouSchedTableItem.m_chDataBytes[4]).toStdString()
                    << "," << GetString(ouSchedTableItem.m_chDataBytes[5]).toStdString() << ","
                    << GetString(ouSchedTableItem.m_chDataBytes[6]).toStdString() << "," << GetString(ouSchedTableItem.m_chDataBytes[7]).toStdString() << "]";

        ui.tableProperties->item(6,1)->setText( QString::fromStdString(omStrStream.str().c_str()));
    }
}

void ScheduleTableDlg::vPopulateReadByIdentifier(CSheduleTableItem& ouSchedTableItem)
{
    std::map<UID_ELEMENT, IElement*> mapEcus;
    eEcuType ouECUType;
    std::string strName;
    //ui.tableProperties->clear();
    ui.tableProperties->setRowCount(0);
    ui.tableProperties->setRowCount(7);
    QList<QVariant> ouColumns;

    ouSchedTableItem.m_eDiagType = eLIN_SID_READ_BY_IDENTIFIER;
    int nSelectedRow = ui.tableFrames->currentRow();

    m_pouLDFCluster->GetElementList(eEcuElement, mapEcus);

    LineEditWidget* pLineEdit[6];

    for(int nIndex = 0; nIndex < 6; nIndex++)
    {
        pLineEdit[nIndex] = new LineEditWidget(nIndex, ui.tableProperties);
        if(true == LDFDatabaseManager::GetDatabaseManager()->bIsDisplayHexOn())
        {
            pLineEdit[nIndex]->setValidator(new QRegExpValidator(QRegExp(defHex_RegExp)));
        }
        else
        {
            pLineEdit[nIndex]->setValidator(new QIntValidator);
        }
        pLineEdit[nIndex]->setFrame(false);
    }

    ouColumns.clear();
    ouColumns.push_back(QVariant("Configured NAD"));
    pLineEdit[0]->setText(GetString( ouSchedTableItem.m_chDataBytes[0], false));
    ui.tableProperties->InsertRow(0, ouColumns);
    ui.tableProperties->setCellWidget(0, 1, pLineEdit[0]);

    ouColumns.clear();
    ouColumns.push_back(QVariant("Data1"));
    pLineEdit[1]->setText(GetString( ouSchedTableItem.m_chDataBytes[3], false));
    ui.tableProperties->InsertRow(1, ouColumns);
    ui.tableProperties->setCellWidget(1, 1, pLineEdit[1]);

    ouColumns.clear();
    ouColumns.push_back(QVariant("Data2"));
    //ouColumns.push_back(QVariant(ouSchedTableItem.m_chDataBytes[2]));
    pLineEdit[2]->setText(GetString( ouSchedTableItem.m_chDataBytes[4], false));
    ui.tableProperties->InsertRow(2, ouColumns);
    ui.tableProperties->setCellWidget(2, 1, pLineEdit[2]);

    ouColumns.clear();
    ouColumns.push_back(QVariant("Data3"));
    //ouColumns.push_back(QVariant(ouSchedTableItem.m_chDataBytes[3]));
    pLineEdit[3]->setText(GetString( ouSchedTableItem.m_chDataBytes[5], false));
    ui.tableProperties->InsertRow(3, ouColumns);
    ui.tableProperties->setCellWidget(3, 1, pLineEdit[3]);

    ouColumns.clear();
    ouColumns.push_back(QVariant("Data4"));
    //ouColumns.push_back(QVariant(ouSchedTableItem.m_chDataBytes[4]));
    pLineEdit[4]->setText(GetString( ouSchedTableItem.m_chDataBytes[6], false));
    ui.tableProperties->InsertRow(4, ouColumns);
    ui.tableProperties->setCellWidget(4, 1, pLineEdit[4]);

    ouColumns.clear();
    ouColumns.push_back(QVariant("Data5"));
    pLineEdit[5]->setText(GetString( ouSchedTableItem.m_chDataBytes[7], false));
    ui.tableProperties->InsertRow(5, ouColumns);
    ui.tableProperties->setCellWidget(5, 1, pLineEdit[5]);

    unsigned char nPCI = 0x06, nSID = 0xB2;

    ouSchedTableItem.m_chDataBytes[1] = nPCI;
    ouSchedTableItem.m_chDataBytes[2] = nSID;

    std::ostringstream omStrStream;
    omStrStream << "[" << GetString(ouSchedTableItem.m_chDataBytes[0]).toStdString() << ","
                << GetString(ouSchedTableItem.m_chDataBytes[1]).toStdString() << ","
                << GetString(ouSchedTableItem.m_chDataBytes[2]).toStdString()
                << "," << GetString(ouSchedTableItem.m_chDataBytes[3]).toStdString() << "," << GetString(ouSchedTableItem.m_chDataBytes[4]).toStdString() << ","
                << GetString(ouSchedTableItem.m_chDataBytes[5]).toStdString() <<
                ","
                << GetString(ouSchedTableItem.m_chDataBytes[6]).toStdString()
                << ","
                << GetString(ouSchedTableItem.m_chDataBytes[7]).toStdString() << "]";

    std::string str = omStrStream.str().c_str();
    ouColumns.clear();
    ouColumns.push_back(QVariant("PDU"));
    ouColumns.push_back(QVariant(omStrStream.str().c_str()));
    ui.tableProperties->InsertRow(6, ouColumns);

    for(int nIndex = 0; nIndex < 6; nIndex++)
    {
        connect(pLineEdit[nIndex], SIGNAL(textChanged ( const QString&)), this, SLOT(onReadByIdEditChange(const QString&)));
    }
}

void ScheduleTableDlg::vPopulateFreeFormat(CSheduleTableItem& ouSchedTableItem)
{
    std::map<UID_ELEMENT, IElement*> mapEcus;
    eEcuType ouECUType;
    std::string strName;
    //ui.tableProperties->clear();
    ui.tableProperties->setRowCount(0);
    ui.tableProperties->setRowCount(9);
    QList<QVariant> ouColumns;

    int nSelectedRow = ui.tableFrames->currentRow();

    m_pouLDFCluster->GetElementList(eEcuElement, mapEcus);

    LineEditWidget* pLineEdit[8];

    for(int nIndex = 0; nIndex < 8; nIndex++)
    {
        pLineEdit[nIndex] = new LineEditWidget(nIndex, ui.tableProperties);
        if(true == LDFDatabaseManager::GetDatabaseManager()->bIsDisplayHexOn())
        {
            pLineEdit[nIndex]->setValidator(new QRegExpValidator(QRegExp(defHex_RegExp)));
        }
        else
        {
            pLineEdit[nIndex]->setValidator(new QIntValidator);
        }
        pLineEdit[nIndex]->setFrame(false);
    }

    ouSchedTableItem.m_eDiagType = eLIN_SID_FREEFORMAT;

    ouColumns.clear();
    ouColumns.push_back(QVariant("Data0"));
    pLineEdit[0]->setText(GetString( ouSchedTableItem.m_chDataBytes[0], false));
    ui.tableProperties->InsertRow(0, ouColumns);
    ui.tableProperties->setCellWidget(0, 1, pLineEdit[0]);

    ouColumns.clear();
    ouColumns.push_back(QVariant("Data1"));
    pLineEdit[1]->setText(GetString( ouSchedTableItem.m_chDataBytes[1], false));
    ui.tableProperties->InsertRow(1, ouColumns);
    ui.tableProperties->setCellWidget(1, 1, pLineEdit[1]);

    ouColumns.clear();
    ouColumns.push_back(QVariant("Data2"));
    //ouColumns.push_back(QVariant(ouSchedTableItem.m_chDataBytes[2]));
    pLineEdit[2]->setText(GetString( ouSchedTableItem.m_chDataBytes[2], false));
    ui.tableProperties->InsertRow(2, ouColumns);
    ui.tableProperties->setCellWidget(2, 1, pLineEdit[2]);

    ouColumns.clear();
    ouColumns.push_back(QVariant("Data3"));
    //ouColumns.push_back(QVariant(ouSchedTableItem.m_chDataBytes[3]));
    pLineEdit[3]->setText(GetString( ouSchedTableItem.m_chDataBytes[3], false));
    ui.tableProperties->InsertRow(3, ouColumns);
    ui.tableProperties->setCellWidget(3, 1, pLineEdit[3]);

    ouColumns.clear();
    ouColumns.push_back(QVariant("Data4"));
    //ouColumns.push_back(QVariant(ouSchedTableItem.m_chDataBytes[4]));
    pLineEdit[4]->setText(GetString( ouSchedTableItem.m_chDataBytes[4], false));
    ui.tableProperties->InsertRow(4, ouColumns);
    ui.tableProperties->setCellWidget(4, 1, pLineEdit[4]);

    ouColumns.clear();
    ouColumns.push_back(QVariant("Data5"));
    pLineEdit[5]->setText(GetString( ouSchedTableItem.m_chDataBytes[5], false));
    ui.tableProperties->InsertRow(5, ouColumns);
    ui.tableProperties->setCellWidget(5, 1, pLineEdit[5]);

    ouColumns.clear();
    ouColumns.push_back(QVariant("Data6"));
    //ouColumns.push_back(QVariant(ouSchedTableItem.m_chDataBytes[5]));
    pLineEdit[6]->setText(GetString( ouSchedTableItem.m_chDataBytes[6], false));
    ui.tableProperties->InsertRow(6, ouColumns);
    ui.tableProperties->setCellWidget(6, 1, pLineEdit[6]);

    ouColumns.clear();
    ouColumns.push_back(QVariant("Data7"));
    //ouColumns.push_back(QVariant(ouSchedTableItem.m_chDataBytes[5]));
    pLineEdit[7]->setText(GetString( ouSchedTableItem.m_chDataBytes[7], false));
    ui.tableProperties->InsertRow(7, ouColumns);
    ui.tableProperties->setCellWidget(7, 1, pLineEdit[7]);

    std::ostringstream omStrStream;
    omStrStream << "[" << GetString(ouSchedTableItem.m_chDataBytes[0]).toStdString() << "," <<  GetString(ouSchedTableItem.m_chDataBytes[1]).toStdString()
                << "," << GetString(ouSchedTableItem.m_chDataBytes[2]).toStdString() << "," << GetString(ouSchedTableItem.m_chDataBytes[3]).toStdString() << ","
                << GetString(ouSchedTableItem.m_chDataBytes[4]).toStdString()
                << "," << GetString(ouSchedTableItem.m_chDataBytes[5]).toStdString() << "," << GetString(ouSchedTableItem.m_chDataBytes[6]).toStdString() << ","
                << GetString(ouSchedTableItem.m_chDataBytes[7]).toStdString() << "]";

    std::string str = omStrStream.str().c_str();
    ouColumns.clear();
    ouColumns.push_back(QVariant("PDU"));
    ouColumns.push_back(QVariant(omStrStream.str().c_str()));
    ui.tableProperties->InsertRow(8, ouColumns);

    for(int nIndex = 0; nIndex < 8; nIndex++)
    {
        connect(pLineEdit[nIndex], SIGNAL(textChanged ( const QString&)), this, SLOT(onFreeFormatEditChange(const QString&)));
    }
}

void ScheduleTableDlg::onReadByIdEditChange(const QString&)
{
    if(ui.tableFrames->currentRow() >= 0)
    {
        QVariant qVar;
        CSheduleTableItem ouSchedTableItem = ui.tableFrames->item( ui.tableFrames->currentRow(), 0)->data(Qt::UserRole).value<CSheduleTableItem>();

        unsigned char nPCI = 0x6, nSID = 0xB2;
        ouSchedTableItem.m_chDataBytes[0] = GetUnsignedInt(((LineEditWidget*)ui.tableProperties->cellWidget(0,1))->text());
        ouSchedTableItem.m_chDataBytes[1] = nPCI;
        ouSchedTableItem.m_chDataBytes[2] = 0xB2;
        ouSchedTableItem.m_chDataBytes[3] = GetUnsignedInt(((LineEditWidget*)ui.tableProperties->cellWidget(1, 1))->text());
        ouSchedTableItem.m_chDataBytes[4] = GetUnsignedInt(((LineEditWidget*)ui.tableProperties->cellWidget(2, 1))->text());
        ouSchedTableItem.m_chDataBytes[5] = GetUnsignedInt(((LineEditWidget*)ui.tableProperties->cellWidget(3, 1))->text());
        ouSchedTableItem.m_chDataBytes[6] = GetUnsignedInt(((LineEditWidget*)ui.tableProperties->cellWidget(4, 1))->text());
        ouSchedTableItem.m_chDataBytes[7] = GetUnsignedInt(((LineEditWidget*)ui.tableProperties->cellWidget(5, 1))->text());

        ouSchedTableItem.m_eDiagType = eLIN_SID_READ_BY_IDENTIFIER;
        qVar.setValue(ouSchedTableItem);
        ui.tableFrames->item( ui.tableFrames->currentRow(), 0)->setData(Qt::UserRole, qVar);

        std::ostringstream omStrStream;
        omStrStream << "[" << GetString(ouSchedTableItem.m_chDataBytes[0]).toStdString() << ","
                    <<  GetString(ouSchedTableItem.m_chDataBytes[1]).toStdString()
                    << "," << GetString(ouSchedTableItem.m_chDataBytes[2]).toStdString() << "," << GetString(ouSchedTableItem.m_chDataBytes[3]).toStdString() << ","
                    << GetString(ouSchedTableItem.m_chDataBytes[4]).toStdString()
                    << "," << GetString(ouSchedTableItem.m_chDataBytes[5]).toStdString() << "," << GetString(ouSchedTableItem.m_chDataBytes[6]).toStdString() << ","
                    << GetString(ouSchedTableItem.m_chDataBytes[7]).toStdString() << "]";

        ui.tableProperties->item(6,1)->setText( QString::fromStdString(omStrStream.str().c_str()));
    }
}

void ScheduleTableDlg::onFreeFormatEditChange(const QString&)
{
    if(ui.tableFrames->currentRow() >= 0)
    {
        QVariant qVar;
        CSheduleTableItem ouSchedTableItem = ui.tableFrames->item( ui.tableFrames->currentRow(), 0)->data(Qt::UserRole).value<CSheduleTableItem>();

        ouSchedTableItem.m_chDataBytes[0] = GetUnsignedInt(((LineEditWidget*)ui.tableProperties->cellWidget(0,1))->text());
        ouSchedTableItem.m_chDataBytes[1] = GetUnsignedInt(((LineEditWidget*)ui.tableProperties->cellWidget(1,1))->text());
        ouSchedTableItem.m_chDataBytes[2] = GetUnsignedInt(((LineEditWidget*)ui.tableProperties->cellWidget(2, 1))->text());
        ouSchedTableItem.m_chDataBytes[3] = GetUnsignedInt(((LineEditWidget*)ui.tableProperties->cellWidget(3, 1))->text());
        ouSchedTableItem.m_chDataBytes[4] = GetUnsignedInt(((LineEditWidget*)ui.tableProperties->cellWidget(4, 1))->text());
        ouSchedTableItem.m_chDataBytes[5] = GetUnsignedInt(((LineEditWidget*)ui.tableProperties->cellWidget(5, 1))->text());
        ouSchedTableItem.m_chDataBytes[6] = GetUnsignedInt(((LineEditWidget*)ui.tableProperties->cellWidget(6, 1))->text());
        ouSchedTableItem.m_chDataBytes[7] = GetUnsignedInt(((LineEditWidget*)ui.tableProperties->cellWidget(7, 1))->text());

        ouSchedTableItem.m_eDiagType = eLIN_SID_FREEFORMAT;
        qVar.setValue(ouSchedTableItem);
        ui.tableFrames->item( ui.tableFrames->currentRow(), 0)->setData(Qt::UserRole, qVar);

        std::ostringstream omStrStream;
        omStrStream << "[" << GetString(ouSchedTableItem.m_chDataBytes[0]).toStdString() << ","
                    <<  GetString(ouSchedTableItem.m_chDataBytes[1]).toStdString()
                    << "," << GetString(ouSchedTableItem.m_chDataBytes[2]).toStdString() << "," << GetString(ouSchedTableItem.m_chDataBytes[3]).toStdString() << ","
                    << GetString(ouSchedTableItem.m_chDataBytes[4]).toStdString()
                    << "," << GetString(ouSchedTableItem.m_chDataBytes[5]).toStdString() << "," << GetString(ouSchedTableItem.m_chDataBytes[6]).toStdString() << ","
                    << GetString(ouSchedTableItem.m_chDataBytes[7]).toStdString() << "]";

        ui.tableProperties->item(8,1)->setText( QString::fromStdString(omStrStream.str().c_str()));
    }
}

void ScheduleTableDlg::vPopulateCondChangeNAD(CSheduleTableItem& ouSchedTableItem)
{
    std::map<UID_ELEMENT, IElement*> mapEcus;
    eEcuType ouECUType;
    std::string strName;
    //ui.tableProperties->clear();
    ui.tableProperties->setRowCount(0);
    ui.tableProperties->setRowCount(7);
    QList<QVariant> ouColumns;

    int nSelectedRow = ui.tableFrames->currentRow();

    m_pouLDFCluster->GetElementList(eEcuElement, mapEcus);

    LineEditWidget* pLineEdit[6];

    for(int nIndex = 0; nIndex < 6; nIndex++)
    {
        pLineEdit[nIndex] = new LineEditWidget(nIndex, ui.tableProperties);
        if(true == LDFDatabaseManager::GetDatabaseManager()->bIsDisplayHexOn())
        {
            pLineEdit[nIndex]->setValidator(new QRegExpValidator(QRegExp(defHex_RegExp)));
        }
        else
        {
            pLineEdit[nIndex]->setValidator(new QIntValidator);
        }
        pLineEdit[nIndex]->setFrame(false);
    }

    ouSchedTableItem.m_eDiagType = eLIN_SID_COND_CHANGE_NAD;


    ouColumns.clear();
    ouColumns.push_back(QVariant("Identifier (D1)"));
    pLineEdit[0]->setText(GetString( ouSchedTableItem.m_chDataBytes[3],false));
    ui.tableProperties->InsertRow(0, ouColumns);
    ui.tableProperties->setCellWidget(0, 1, pLineEdit[0]);

    ouColumns.clear();
    ouColumns.push_back(QVariant("Byte (D2)"));
    //ouColumns.push_back(QVariant(ouSchedTableItem.m_chDataBytes[2]));
    pLineEdit[1]->setText(GetString( ouSchedTableItem.m_chDataBytes[4],false));
    ui.tableProperties->InsertRow(1, ouColumns);
    ui.tableProperties->setCellWidget(1, 1, pLineEdit[1]);

    ouColumns.clear();
    ouColumns.push_back(QVariant("Mask (D3)"));
    //ouColumns.push_back(QVariant(ouSchedTableItem.m_chDataBytes[3]));
    pLineEdit[2]->setText(GetString( ouSchedTableItem.m_chDataBytes[5],false));
    ui.tableProperties->InsertRow(2, ouColumns);
    ui.tableProperties->setCellWidget(2, 1, pLineEdit[2]);

    ouColumns.clear();
    ouColumns.push_back(QVariant("Invert (D4)"));
    //ouColumns.push_back(QVariant(ouSchedTableItem.m_chDataBytes[4]));
    pLineEdit[3]->setText(GetString( ouSchedTableItem.m_chDataBytes[6],false));
    ui.tableProperties->InsertRow(3, ouColumns);
    ui.tableProperties->setCellWidget(3, 1, pLineEdit[3]);

    ouColumns.clear();
    ouColumns.push_back(QVariant("NAD"));
    pLineEdit[4]->setText(GetString( ouSchedTableItem.m_chDataBytes[0],false));
    ui.tableProperties->InsertRow(4, ouColumns);
    ui.tableProperties->setCellWidget(4, 1, pLineEdit[4]);

    ouColumns.clear();
    ouColumns.push_back(QVariant("New NAD (D5)"));
    //ouColumns.push_back(QVariant(ouSchedTableItem.m_chDataBytes[5]));
    pLineEdit[5]->setText(GetString( ouSchedTableItem.m_chDataBytes[7],false));
    ui.tableProperties->InsertRow(5, ouColumns);
    ui.tableProperties->setCellWidget(5, 1, pLineEdit[5]);

    int nPCI = 0x06, nSID = 0xB3;
    ouSchedTableItem.m_chDataBytes[1] = nPCI;
    ouSchedTableItem.m_chDataBytes[2] = nSID;
    std::ostringstream omStrStream;
    omStrStream << "[" << GetString(ouSchedTableItem.m_chDataBytes[0]).toStdString() << "," <<  GetString(nPCI).toStdString().c_str()
                << "," << GetString(nSID).toStdString().c_str()  << "," << GetString(ouSchedTableItem.m_chDataBytes[3]).toStdString()
                << "," << GetString(ouSchedTableItem.m_chDataBytes[4]).toStdString()
                << "," << GetString(ouSchedTableItem.m_chDataBytes[5]).toStdString() << ","
                << GetString(ouSchedTableItem.m_chDataBytes[6]).toStdString() << ","
                << GetString(ouSchedTableItem.m_chDataBytes[7]).toStdString() << "]";

    ouColumns.clear();
    ouColumns.push_back(QVariant("PDU"));
    ouColumns.push_back(QVariant(omStrStream.str().c_str()));
    ui.tableProperties->InsertRow(6, ouColumns);

    for(int nIndex = 0; nIndex < 6; nIndex++)
    {
        connect(pLineEdit[nIndex], SIGNAL(textChanged ( const QString&)), this, SLOT(onCondNADEditChange(const QString&)));
    }


}

void ScheduleTableDlg::onComboAssignNADNodeChange(int nIndex)
{
    UID_ELEMENT uidECU = ((ScheduleComboWidget*)ui.tableProperties->cellWidget(0, 1))->currentData(Qt::UserRole).value<UID_ELEMENT>();

    IElement* pElement = nullptr;
    if(EC_SUCCESS == m_pouLDFCluster->GetElement(eEcuElement ,uidECU, &pElement))
    {

        EcuProperties ouEcuProps;
        ((IEcu*)pElement)->GetProperties(ouEcuProps);
        ui.tableProperties->item(1, 1)->setText(GetString(ouEcuProps.m_ouSlavePros.m_nInitialNAD));
        ui.tableProperties->item(2, 1)->setText(GetString(ouEcuProps.m_ouSlavePros.m_nSupplierId));

        ui.tableProperties->item(3, 1)->setText(GetString(ouEcuProps.m_ouSlavePros.m_nFunctionId));

        ui.tableProperties->item(4, 1)->setText(GetString(ouEcuProps.m_ouSlavePros.m_nConfiguredNAD));

        int nLSBSupplierId, nMSBSupplierId, nLSBFunctionId, nMSBFunctionId;
        nLSBSupplierId = ouEcuProps.m_ouSlavePros.m_nSupplierId & 0xFF;
        nMSBSupplierId = (ouEcuProps.m_ouSlavePros.m_nSupplierId & 0xFF00) >> 8;
        nLSBFunctionId = ouEcuProps.m_ouSlavePros.m_nFunctionId & 0xFF;
        nMSBFunctionId = (ouEcuProps.m_ouSlavePros.m_nFunctionId & 0xFF00) >> 8;

        int nPCI = 0x06, nSID = 0xB0;
        std::ostringstream omStrStream;
        omStrStream << "[" << GetString(ouEcuProps.m_ouSlavePros.m_nInitialNAD).toStdString().c_str() << "," <<  GetString(nPCI).toStdString().c_str()
                    << "," << GetString(nSID).toStdString().c_str() << "," << GetString(ouEcuProps.m_ouSlavePros.m_nSupplierId).toStdString().c_str()
                    << "," << GetString(nMSBSupplierId).toStdString().c_str()
                    << "," << GetString(ouEcuProps.m_ouSlavePros.m_nFunctionId).toStdString().c_str() << ","
                    << GetString(nMSBFunctionId).toStdString().c_str() << "," << GetString(ouEcuProps.m_ouSlavePros.m_nConfiguredNAD).toStdString().c_str() << "]";

        ui.tableProperties->item(5, 1)->setText(omStrStream.str().c_str());

        CSheduleTableItem ouSchedItem = ui.tableFrames->item(ui.tableFrames->currentRow(), 0)->data(Qt::UserRole).value<CSheduleTableItem>();
        ouSchedItem.m_nNode = uidECU;
        ouSchedItem.m_chDataBytes[0] = ouEcuProps.m_ouSlavePros.m_nInitialNAD;
        ouSchedItem.m_chDataBytes[1] = ouEcuProps.m_ouSlavePros.m_nConfiguredNAD;
        ouSchedItem.m_chDataBytes[2] = ouEcuProps.m_ouSlavePros.m_nSupplierId;
        ouSchedItem.m_chDataBytes[3] = ouEcuProps.m_ouSlavePros.m_nFunctionId;
        ouSchedItem.m_dDelay = ui.tableFrames->item(ui.tableFrames->currentRow(), 3)->text().toDouble();
        QVariant qVar;
        qVar.setValue(ouSchedItem);
        ui.tableFrames->item(ui.tableFrames->currentRow(), 0)->setData(Qt::UserRole, qVar);
    }
}


void ScheduleTableDlg::onComboAssignIdRangeNodeChange(int nIndex)
{
    UID_ELEMENT uidECU = ((ScheduleComboWidget*)ui.tableProperties->cellWidget(0, 1))->currentData(Qt::UserRole).value<UID_ELEMENT>();

    IElement* pElement = nullptr;
    if(EC_SUCCESS == m_pouLDFCluster->GetElement(eEcuElement ,uidECU, &pElement))
    {
        CSheduleTableItem ouSchedItem = ui.tableFrames->item(ui.tableFrames->currentRow(), 0)->data(Qt::UserRole).value<CSheduleTableItem>();
        int nStartIndex = ouSchedItem.m_chDataBytes[3];

        EcuProperties ouEcuProps;
        ((IEcu*)pElement)->GetProperties(ouEcuProps);

        unsigned char nPID[4];

        memset(&nPID, 0xFF, sizeof(nPID));
        int nIndex = 0;
        IElement* pFrame = nullptr;
        unsigned int unFrameId;

        auto itr = ouEcuProps.m_ouSlavePros.m_nConfiurableFrameIdList.begin();
        std::advance(itr, nStartIndex);
        for(; itr != ouEcuProps.m_ouSlavePros.m_nConfiurableFrameIdList.end(); itr++)
        {
            if(nIndex >= 4)
            {
                break;
            }
            if(EC_SUCCESS == m_pouLDFCluster->GetElement(eFrameElement, itr->m_uidFrame, &pFrame))
            {
                unFrameId = 0;
                ((IFrame*)pFrame)->GetFrameId(unFrameId);
                nPID[nIndex] = ucCalculatePID(unFrameId);
            }
            nIndex++;
        }

        ((LineEditWidget*)ui.tableProperties->cellWidget(1, 1))->setText((GetString(nPID[0],false).toStdString().c_str()));
        ((LineEditWidget*)ui.tableProperties->cellWidget(2, 1))->setText(GetString(nPID[1],false).toStdString().c_str());

        ((LineEditWidget*)ui.tableProperties->cellWidget(3, 1))->setText(GetString(nPID[2],false).toStdString().c_str());

        ((LineEditWidget*)ui.tableProperties->cellWidget(4, 1))->setText(GetString(nPID[3],false).toStdString().c_str());
        ((LineEditWidget*)ui.tableProperties->cellWidget(5, 1))->setText(GetString(nStartIndex,false).toStdString().c_str());

        int nPCI = 0x06, nSID = 0xB7;
        std::ostringstream omStrStream;
        omStrStream << "[" << GetString(ouEcuProps.m_ouSlavePros.m_nConfiguredNAD).toStdString().c_str() << ","
                    <<  GetString(nPCI).toStdString().c_str() << "," << GetString(nSID).toStdString().c_str() << ","
                    << GetString(nStartIndex).toStdString().c_str() << "," << GetString(nPID[0]).toStdString().c_str()
                    << "," << GetString(nPID[1]).toStdString().c_str() << "," << GetString(nPID[2]).toStdString().c_str() << ","
                    << GetString(nPID[3]).toStdString().c_str() << "]";

        ui.tableProperties->item(6, 1)->setText(omStrStream.str().c_str());


        ouSchedItem.m_nNode = uidECU;
        //ouSchedItem.m_chDataBytes[3] = nStartIndex;
        ouSchedItem.m_chDataBytes[4] = nPID[0];
        ouSchedItem.m_chDataBytes[5] = nPID[1];
        ouSchedItem.m_chDataBytes[6] = nPID[2];
        ouSchedItem.m_chDataBytes[7] = nPID[3];
        ouSchedItem.m_dDelay = ui.tableFrames->item(ui.tableFrames->currentRow(), 3)->text().toDouble();
        QVariant qVar;
        qVar.setValue(ouSchedItem);
        ui.tableFrames->item(ui.tableFrames->currentRow(), 0)->setData(Qt::UserRole, qVar);
    }
}

void ScheduleTableDlg::onComboDataDumpChange(int nIndex)
{
    UID_ELEMENT uidECU = ((ScheduleComboWidget*)ui.tableProperties->cellWidget(0, 1))->currentData(Qt::UserRole).value<UID_ELEMENT>();

    IElement* pElement = nullptr;
    if(EC_SUCCESS == m_pouLDFCluster->GetElement(eEcuElement ,uidECU, &pElement))
    {
        CSheduleTableItem ouSchedItem = ui.tableFrames->item(ui.tableFrames->currentRow(), 0)->data(Qt::UserRole).value<CSheduleTableItem>();
        EcuProperties ouEcuProps;
        ((IEcu*)pElement)->GetProperties(ouEcuProps);
        ui.tableProperties->item(1, 1)->setText(GetString(ouEcuProps.m_ouSlavePros.m_nConfiguredNAD));
        int nVal = 0x06, nVal1= 0xB4, nVal2 = 255;

        std::ostringstream omStrStream;

        omStrStream << "[" << GetString(ouEcuProps.m_ouSlavePros.m_nConfiguredNAD).toStdString().c_str() << "," <<  GetString(nVal).toStdString().c_str()
                    << "," << GetString(nVal1).toStdString().c_str() << ","
                    <<  GetString(ouSchedItem.m_chDataBytes[3]).toStdString().c_str() << "," << GetString(ouSchedItem.m_chDataBytes[4]).toStdString().c_str()
                    << "," << GetString(ouSchedItem.m_chDataBytes[5]).toStdString().c_str()
                    << "," << GetString(ouSchedItem.m_chDataBytes[6]).toStdString().c_str() << ","
                    << GetString(ouSchedItem.m_chDataBytes[7]).toStdString().c_str() << "]";

        ui.tableProperties->item(7, 1)->setText(omStrStream.str().c_str());


        ouSchedItem.m_nNode = uidECU;
        //memset(ouSchedItem.m_chDataBytes, 0, sizeof(ouSchedItem.m_chDataBytes));
        ouSchedItem.m_dDelay = ui.tableFrames->item(ui.tableFrames->currentRow(), 3)->text().toDouble();
        QVariant qVar;
        qVar.setValue(ouSchedItem);
        ui.tableFrames->item(ui.tableFrames->currentRow(), 0)->setData(Qt::UserRole, qVar);
    }
}

void ScheduleTableDlg::onComboSaveConfigChange(int nIndex)
{
    UID_ELEMENT uidECU = ((ScheduleComboWidget*)ui.tableProperties->cellWidget(0, 1))->currentData(Qt::UserRole).value<UID_ELEMENT>();

    IElement* pElement = nullptr;
    if(EC_SUCCESS == m_pouLDFCluster->GetElement(eEcuElement ,uidECU, &pElement))
    {
        CSheduleTableItem ouSchedItem = ui.tableFrames->item(ui.tableFrames->currentRow(), 0)->data(Qt::UserRole).value<CSheduleTableItem>();
        EcuProperties ouEcuProps;
        ((IEcu*)pElement)->GetProperties(ouEcuProps);
        ui.tableProperties->item(1, 1)->setText(GetString(ouEcuProps.m_ouSlavePros.m_nConfiguredNAD));
        int nVal = 0x01, nVal1= 0xB6, nVal2 = 255;
        if(ouSchedItem.m_eDiagType == eLIN_SID_SAVE_CONFIG)
        {
            nVal1 = 0xB6;
        }
        else if(ouSchedItem.m_eDiagType == eLIN_SID_DATA_DUMP)
        {
            nVal1 = 0xB4;
        }

        std::ostringstream omStrStream;

        omStrStream << "[" << GetString(ouEcuProps.m_ouSlavePros.m_nConfiguredNAD).toStdString().c_str() << "," <<  GetString(nVal).toStdString().c_str() << "," << GetString(nVal1).toStdString().c_str() << ","
                    <<  GetString(nVal2).toStdString().c_str() << "," << GetString(nVal2).toStdString().c_str() << "," << GetString(nVal2).toStdString().c_str()
                    << "," << GetString(nVal2).toStdString().c_str() << "," << GetString(nVal2).toStdString().c_str() << "]";

        ui.tableProperties->item(2, 1)->setText(omStrStream.str().c_str());


        ouSchedItem.m_nNode = uidECU;
        memset(ouSchedItem.m_chDataBytes, 0, sizeof(ouSchedItem.m_chDataBytes));
        ouSchedItem.m_dDelay = ui.tableFrames->item(ui.tableFrames->currentRow(), 3)->text().toDouble();
        QVariant qVar;
        qVar.setValue(ouSchedItem);
        ui.tableFrames->item(ui.tableFrames->currentRow(), 0)->setData(Qt::UserRole, qVar);
    }
}

void ScheduleTableDlg::vPopulateDataDump(CSheduleTableItem& ouSchedTableItem)
{
    std::map<UID_ELEMENT, IElement*> mapEcus;
    eEcuType ouECUType;
    std::string strName;
    //ui.tableProperties->clear();
    ui.tableProperties->setRowCount(0);
    ui.tableProperties->setRowCount(8);

    int nSelectedRow = ui.tableFrames->currentRow();

    UID_ELEMENT unECUId = ouSchedTableItem.m_nNode;
    std::string strECUName;
    IElement* pElement = nullptr;

    // Node
    QComboBox* pComboBox = new QComboBox(nullptr);
    pComboBox->setFrame(false);

    m_pouLDFCluster->GetElementList(eEcuElement, mapEcus);

    QVariant qVar;
    int nIndex = 0;
for(auto itrECU : mapEcus)
    {
        ((IEcu*)itrECU.second)->GetEcuType(ouECUType);

        if(ouECUType == eLIN_Slave)
        {
            itrECU.second->GetName(strName);
            pComboBox->addItem(QString::fromStdString(strName));
            qVar.setValue(((IEcu*)itrECU.second)->GetUniqueId());
            pComboBox->setItemData(nIndex, qVar, Qt::UserRole);
            nIndex++;
        }
    }

    if(unECUId == INVALID_UID_ELEMENT)
    {
        unECUId = pComboBox->itemData(0, Qt::UserRole).value<UID_ELEMENT>();
    }

    if(EC_SUCCESS == m_pouLDFCluster->GetElement(eEcuElement, unECUId, &pElement))
    {
        pElement->GetName(strECUName);
        pComboBox->setCurrentText(QString::fromStdString(strECUName));
    }

    connect(pComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(onComboDataDumpChange(int)));
    IEcu* pEcu = (IEcu*)pElement;

    int nRow = 0;
    EcuProperties ouEcuProps;
    pEcu->GetProperties(ouEcuProps);
    QList<QVariant> ouColumns;
    std::string strColName = "Node Name";
    ouColumns.push_back(QVariant(strColName.c_str()));
    ouColumns.push_back(QVariant());

    ui.tableProperties->InsertRow(nRow, ouColumns);
    ui.tableProperties->setCellWidget(nRow, 1, pComboBox);
    nRow++;

    ouColumns.clear();
    ouColumns.push_back(QVariant("Configured NAD"));
    ouColumns.push_back(QVariant(ouEcuProps.m_ouSlavePros.m_nConfiguredNAD));
    ui.tableProperties->InsertRow(nRow++, ouColumns);

    std::ostringstream omStrStream;
    int nVal = 0x01, nVal1= 0xB6, nVal2 = 255;

    if(ouSchedTableItem.m_eDiagType == eLIN_SID_DATA_DUMP)
    {
        nVal = 0x06, nVal1=0xB4;
    }

    LineEditWidget* pLineEdit[5];
    for(int nIndex = 0; nIndex < 5; nIndex++)
    {
        pLineEdit[nIndex] = new LineEditWidget(nIndex, ui.tableProperties);
        if(true == LDFDatabaseManager::GetDatabaseManager()->bIsDisplayHexOn())
        {
            pLineEdit[nIndex]->setValidator(new QRegExpValidator(QRegExp(defHex_RegExp)));
        }
        else
        {
            pLineEdit[nIndex]->setValidator(new QIntValidator);
        }
        pLineEdit[nIndex]->setFrame(false);
    }

    ouColumns.clear();
    ouColumns.push_back(QVariant("Data1"));
    ouColumns.push_back(QVariant());
    ui.tableProperties->InsertRow(nRow, ouColumns);
    ui.tableProperties->setCellWidget(nRow++, 1, pLineEdit[0]);
    pLineEdit[0]->setText(GetString( ouSchedTableItem.m_chDataBytes[3], false));

    ouColumns.clear();
    ouColumns.push_back(QVariant("Data2"));
    ouColumns.push_back(QVariant());
    ui.tableProperties->InsertRow(nRow, ouColumns);
    ui.tableProperties->setCellWidget(nRow++, 1, pLineEdit[1]);
    pLineEdit[1]->setText(GetString( ouSchedTableItem.m_chDataBytes[4], false));

    ouColumns.clear();
    ouColumns.push_back(QVariant("Data3"));
    ouColumns.push_back(QVariant());
    ui.tableProperties->InsertRow(nRow, ouColumns);
    ui.tableProperties->setCellWidget(nRow++, 1, pLineEdit[2]);
    pLineEdit[2]->setText(GetString( ouSchedTableItem.m_chDataBytes[5], false));

    ouColumns.clear();
    ouColumns.push_back(QVariant("Data4"));
    ouColumns.push_back(QVariant());
    ui.tableProperties->InsertRow(nRow, ouColumns);
    ui.tableProperties->setCellWidget(nRow++, 1, pLineEdit[3]);
    pLineEdit[3]->setText(GetString( ouSchedTableItem.m_chDataBytes[6], false));


    ouColumns.clear();
    ouColumns.push_back(QVariant("Data5"));
    ouColumns.push_back(QVariant());
    ui.tableProperties->InsertRow(nRow, ouColumns);
    ui.tableProperties->setCellWidget(nRow++, 1, pLineEdit[4]);
    pLineEdit[4]->setText(GetString( ouSchedTableItem.m_chDataBytes[7], false));

    omStrStream << "[" <<  GetString(ouEcuProps.m_ouSlavePros.m_nConfiguredNAD).toStdString().c_str()
                << "," <<  GetString(nVal).toStdString().c_str() << "," << GetString(nVal1).toStdString().c_str() << ","
                <<  GetString(ouSchedTableItem.m_chDataBytes[3]).toStdString() << "," << GetString(ouSchedTableItem.m_chDataBytes[4]).toStdString()
                << "," << GetString(ouSchedTableItem.m_chDataBytes[5]).toStdString()
                << "," << GetString(ouSchedTableItem.m_chDataBytes[6]).toStdString() << "," << GetString(ouSchedTableItem.m_chDataBytes[7]).toStdString() << "]";
    ouColumns.clear();
    ouColumns.push_back(QVariant("PDU"));
    ouColumns.push_back(QVariant(omStrStream.str().c_str()));
    ui.tableProperties->InsertRow(nRow++, ouColumns);

    for(int nIndex = 0; nIndex < 5; nIndex++)
    {
        connect(pLineEdit[nIndex], SIGNAL(textChanged ( const QString&)), this, SLOT(onAssignDataDumpEditChange(const QString&)));
    }

    ouSchedTableItem.m_nNode = unECUId;
}

void ScheduleTableDlg::vPopulateSaveConfig(CSheduleTableItem& ouSchedTableItem)
{
    std::map<UID_ELEMENT, IElement*> mapEcus;
    eEcuType ouECUType;
    std::string strName;
    //ui.tableProperties->clear();
    ui.tableProperties->setRowCount(0);
    ui.tableProperties->setRowCount(3);

    int nSelectedRow = ui.tableFrames->currentRow();

    UID_ELEMENT unECUId = ouSchedTableItem.m_nNode;
    std::string strECUName;
    IElement* pElement = nullptr;

    // Node
    QComboBox* pComboBox = new QComboBox(nullptr);
    pComboBox->setFrame(false);

    m_pouLDFCluster->GetElementList(eEcuElement, mapEcus);

    QVariant qVar;
    int nIndex = 0;
for(auto itrECU : mapEcus)
    {
        ((IEcu*)itrECU.second)->GetEcuType(ouECUType);

        if(ouECUType == eLIN_Slave)
        {
            itrECU.second->GetName(strName);
            pComboBox->addItem(QString::fromStdString(strName));
            qVar.setValue(((IEcu*)itrECU.second)->GetUniqueId());
            pComboBox->setItemData(nIndex, qVar, Qt::UserRole);
            nIndex++;
        }
    }

    if(unECUId == INVALID_UID_ELEMENT)
    {
        unECUId = pComboBox->itemData(0, Qt::UserRole).value<UID_ELEMENT>();
    }

    if(EC_SUCCESS == m_pouLDFCluster->GetElement(eEcuElement, unECUId, &pElement))
    {
        pElement->GetName(strECUName);
        pComboBox->setCurrentText(QString::fromStdString(strECUName));
    }

    connect(pComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(onComboSaveConfigChange(int)));
    IEcu* pEcu = (IEcu*)pElement;

    EcuProperties ouEcuProps;
    pEcu->GetProperties(ouEcuProps);
    QList<QVariant> ouColumns;
    std::string strColName = "Node Name";
    ouColumns.push_back(QVariant(strColName.c_str()));
    ouColumns.push_back(QVariant());

    ui.tableProperties->InsertRow(0, ouColumns);
    ui.tableProperties->setCellWidget(0, 1, pComboBox);

    ouColumns.clear();
    ouColumns.push_back(QVariant("Configured NAD"));
    ouColumns.push_back(QVariant(ouEcuProps.m_ouSlavePros.m_nConfiguredNAD));
    ui.tableProperties->InsertRow(1, ouColumns);

    std::ostringstream omStrStream;
    int nVal = 0x01, nVal1= 0xB6, nVal2 = 255;

    if(ouSchedTableItem.m_eDiagType == eLIN_SID_DATA_DUMP)
    {
        nVal = 0x06, nVal1=0xB4;
    }

    omStrStream << "[" <<  GetString(ouEcuProps.m_ouSlavePros.m_nConfiguredNAD).toStdString().c_str()
                << "," <<  GetString(nVal).toStdString().c_str() << "," << GetString(nVal1).toStdString().c_str() << ","
                <<  GetString(nVal2).toStdString().c_str() << "," << GetString(nVal2).toStdString().c_str() << "," << GetString(nVal2).toStdString().c_str()
                << "," << GetString(nVal2).toStdString().c_str() << "," << GetString(nVal2).toStdString().c_str() << "]";
    ouColumns.clear();
    ouColumns.push_back(QVariant("PDU"));
    ouColumns.push_back(QVariant(omStrStream.str().c_str()));
    ui.tableProperties->InsertRow(2, ouColumns);

    ouSchedTableItem.m_nNode = unECUId;
}

void ScheduleTableDlg::vPopulateAssignFrameIdRange(CSheduleTableItem& ouSchedTableItem)
{
    std::map<UID_ELEMENT, IElement*> mapEcus;
    eEcuType ouECUType;
    std::string strName;
    //ui.tableProperties->clear();
    ui.tableProperties->setRowCount(0);
    ui.tableProperties->setRowCount(7);

    int nSelectedRow = ui.tableFrames->currentRow();

    UID_ELEMENT unECUId = ouSchedTableItem.m_nNode;
    std::string strECUName;
    IElement* pElement = nullptr;

    // Node
    QComboBox* pComboBox = new QComboBox(nullptr);
    pComboBox->setFrame(false);

    m_pouLDFCluster->GetElementList(eEcuElement, mapEcus);

    QVariant qVar;
    int nIndex = 0;
for(auto itrECU : mapEcus)
    {
        ((IEcu*)itrECU.second)->GetEcuType(ouECUType);

        if(ouECUType == eLIN_Slave)
        {
            itrECU.second->GetName(strName);
            pComboBox->addItem(QString::fromStdString(strName));
            qVar.setValue(((IEcu*)itrECU.second)->GetUniqueId());
            pComboBox->setItemData(nIndex, qVar, Qt::UserRole);
            nIndex++;
        }
    }

    bool bIsNewCmd = true;
    int nPID[4], nStartIndex = 0;
    memset(nPID, -1, sizeof(nPID));
    if(unECUId == INVALID_UID_ELEMENT)
    {
        unECUId = pComboBox->itemData(0, Qt::UserRole).value<UID_ELEMENT>();
        bIsNewCmd = false;
    }

    if(EC_SUCCESS == m_pouLDFCluster->GetElement(eEcuElement, unECUId, &pElement))
    {
        pElement->GetName(strECUName);
        pComboBox->setCurrentText(QString::fromStdString(strECUName));
        nPID[0] = ouSchedTableItem.m_chDataBytes[4];
        nPID[1] = ouSchedTableItem.m_chDataBytes[5];
        nPID[2] = ouSchedTableItem.m_chDataBytes[6];
        nPID[3] = ouSchedTableItem.m_chDataBytes[7];
        nStartIndex = ouSchedTableItem.m_chDataBytes[3];
    }

    EcuProperties ouECUProps;
    ((IEcu*)pElement)->GetProperties(ouECUProps);
    if(bIsNewCmd == false)
    {
        int nIndex = 0;
        IElement* pFrame = nullptr;
        unsigned int unFrameId = 0;
for(auto itr : ouECUProps.m_ouSlavePros.m_nConfiurableFrameIdList)
        {
            if(nIndex >= 4)
            {
                break;
            }
            if(EC_SUCCESS == m_pouLDFCluster->GetElement(eFrameElement, itr.m_uidFrame, &pFrame))
            {
                unFrameId = 0;
                ((IFrame*)pFrame)->GetFrameId(unFrameId);
                nPID[nIndex] = ucCalculatePID(unFrameId);
            }

            nIndex++;
        }
    }

    LineEditWidget* pLineEdit[5];
    for(int nIndex = 0; nIndex < 5; nIndex++)
    {
        pLineEdit[nIndex] = new LineEditWidget(nIndex, ui.tableProperties);

        if(true == LDFDatabaseManager::GetDatabaseManager()->bIsDisplayHexOn())
        {
            pLineEdit[nIndex]->setValidator(new QRegExpValidator(QRegExp(defHex_RegExp)));
        }
        else
        {
            pLineEdit[nIndex]->setValidator(new QIntValidator);
        }
        pLineEdit[nIndex]->setFrame(false);
    }

    IEcu* pEcu = (IEcu*)pElement;

    EcuProperties ouEcuProps;
    pEcu->GetProperties(ouEcuProps);
    QList<QVariant> ouColumns;
    std::string strColName = "Node Name";
    ouColumns.push_back(QVariant(strColName.c_str()));
    ouColumns.push_back(QVariant());

    ui.tableProperties->InsertRow(0, ouColumns);
    ui.tableProperties->setCellWidget(0, 1, pComboBox);

    ouColumns.clear();
    ouColumns.push_back(QVariant("PID1"));
    ouColumns.push_back(QVariant( GetString(nPID[0],false).toStdString().c_str()));
    ui.tableProperties->InsertRow(1, ouColumns);
    pLineEdit[0]->setText(GetString(nPID[0],false).toStdString().c_str());
    ui.tableProperties->setCellWidget(1, 1, pLineEdit[0]);

    ouColumns.clear();
    ouColumns.push_back(QVariant("PID2"));
    ouColumns.push_back(QVariant( GetString(nPID[1]).toStdString().c_str()));
    ui.tableProperties->InsertRow(2, ouColumns);
    pLineEdit[1]->setText(GetString(nPID[1],false).toStdString().c_str());
    ui.tableProperties->setCellWidget(2, 1, pLineEdit[1]);

    ouColumns.clear();
    ouColumns.push_back(QVariant("PID3"));
    ouColumns.push_back(QVariant( GetString(nPID[2]).toStdString().c_str()));
    ui.tableProperties->InsertRow(3, ouColumns);
    pLineEdit[2]->setText(GetString(nPID[2],false).toStdString().c_str());
    ui.tableProperties->setCellWidget(3, 1, pLineEdit[2]);

    ouColumns.clear();
    ouColumns.push_back(QVariant("PID4"));
    ouColumns.push_back(QVariant( GetString(nPID[3]).toStdString().c_str()));
    ui.tableProperties->InsertRow(4, ouColumns);
    pLineEdit[3]->setText(GetString(nPID[3],false).toStdString().c_str());
    ui.tableProperties->setCellWidget(4, 1, pLineEdit[3]);

    ouColumns.clear();
    ouColumns.push_back(QVariant("Start Index"));
    ouColumns.push_back(QVariant(nStartIndex));
    ui.tableProperties->InsertRow(5, ouColumns);
    pLineEdit[4]->setText(GetString(nStartIndex,false).toStdString().c_str());
    ui.tableProperties->setCellWidget(5, 1, pLineEdit[4]);
    int nPCI = 0x06, nSID = 0xB7;
    std::ostringstream omStrStream;
    omStrStream << "[" << GetString(ouEcuProps.m_ouSlavePros.m_nConfiguredNAD).toStdString().c_str() << ","
                <<  GetString(nPCI).toStdString().c_str() << "," << GetString(nSID).toStdString().c_str()
                << "," << GetString(nStartIndex).toStdString().c_str() << "," << GetString(nPID[0]).toStdString().c_str()
                << "," << GetString(nPID[1]).toStdString().c_str() << ","
                << GetString(nPID[2]).toStdString().c_str() << ","
                << GetString(nPID[3]).toStdString().c_str() << "]";

    ouColumns.clear();
    ouColumns.push_back(QVariant("PDU"));
    ouColumns.push_back(QVariant(omStrStream.str().c_str()));
    ui.tableProperties->InsertRow(6, ouColumns);

    ouSchedTableItem.m_nNode = unECUId;

    for(int nIndex = 0; nIndex < 5; nIndex++)
    {
        connect(pLineEdit[nIndex], SIGNAL(textChanged ( const QString&)), this, SLOT(onAssignFrameIdRangeEditChange(const QString&)));
    }
    connect(pComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(onComboAssignIdRangeNodeChange(int)));
}

void ScheduleTableDlg::onAssignFrameIdRangeEditChange(const QString&)
{
    if(ui.tableFrames->currentRow() >= 0)
    {
        UID_ELEMENT uidECU = ((ScheduleComboWidget*)ui.tableProperties->cellWidget(0, 1))->currentData(Qt::UserRole).value<UID_ELEMENT>();

        IElement* pElement = nullptr;
        if(EC_SUCCESS == m_pouLDFCluster->GetElement(eEcuElement ,uidECU, &pElement))
        {
            EcuProperties ouEcuProps;
            ((IEcu*)pElement)->GetProperties(ouEcuProps);
            CSheduleTableItem ouSchedItem = ui.tableFrames->item(ui.tableFrames->currentRow(), 0)->data(Qt::UserRole).value<CSheduleTableItem>();

            ouSchedItem.m_chDataBytes[3] = GetUnsignedInt(((LineEditWidget*)ui.tableProperties->cellWidget(5, 1))->text());
            ouSchedItem.m_chDataBytes[4] =  GetUnsignedInt(((LineEditWidget*)ui.tableProperties->cellWidget(1, 1))->text());
            ouSchedItem.m_chDataBytes[5] =  GetUnsignedInt(((LineEditWidget*)ui.tableProperties->cellWidget(2, 1))->text());
            ouSchedItem.m_chDataBytes[6] =  GetUnsignedInt(((LineEditWidget*)ui.tableProperties->cellWidget(3, 1))->text());
            ouSchedItem.m_chDataBytes[7] =  GetUnsignedInt(((LineEditWidget*)ui.tableProperties->cellWidget(4, 1))->text());

            int nPCI = 0x06, nSID = 0xB7;
            std::ostringstream omStrStream;
            omStrStream << "[" << GetString(ouEcuProps.m_ouSlavePros.m_nConfiguredNAD).toStdString().c_str() << ","
                        <<  GetString(nPCI).toStdString().c_str() << "," << GetString(nSID).toStdString().c_str() << ","
                        << GetString(ouSchedItem.m_chDataBytes[3]).toStdString().c_str() << ","
                        << GetString(ouSchedItem.m_chDataBytes[4]).toStdString().c_str()
                        << "," << GetString(ouSchedItem.m_chDataBytes[5]).toStdString().c_str()
                        << "," << GetString(ouSchedItem.m_chDataBytes[6]).toStdString().c_str() << ","
                        << GetString(ouSchedItem.m_chDataBytes[7]).toStdString().c_str() << "]";

            ui.tableProperties->item(6, 1)->setText(omStrStream.str().c_str());
            QVariant qVar;
            qVar.setValue(ouSchedItem);
            ui.tableFrames->item(ui.tableFrames->currentRow(), 0)->setData(Qt::UserRole, qVar);
        }
    }
}

void ScheduleTableDlg::onAssignDataDumpEditChange(const QString&)
{
    if(ui.tableFrames->currentRow() >= 0)
    {
        UID_ELEMENT uidECU = ((ScheduleComboWidget*)ui.tableProperties->cellWidget(0, 1))->currentData(Qt::UserRole).value<UID_ELEMENT>();

        IElement* pElement = nullptr;
        if(EC_SUCCESS == m_pouLDFCluster->GetElement(eEcuElement ,uidECU, &pElement))
        {
            EcuProperties ouEcuProps;
            ((IEcu*)pElement)->GetProperties(ouEcuProps);
            CSheduleTableItem ouSchedItem = ui.tableFrames->item(ui.tableFrames->currentRow(), 0)->data(Qt::UserRole).value<CSheduleTableItem>();

            int nPCI = 0x06, nSID = 0xB4;
            ouSchedItem.m_chDataBytes[0] = ouEcuProps.m_ouSlavePros.m_nConfiguredNAD;
            ouSchedItem.m_chDataBytes[1] = nPCI;
            ouSchedItem.m_chDataBytes[2] = nSID;
            ouSchedItem.m_chDataBytes[3] = GetUnsignedInt(((LineEditWidget*)ui.tableProperties->cellWidget(2, 1))->text());
            ouSchedItem.m_chDataBytes[4] =  GetUnsignedInt(((LineEditWidget*)ui.tableProperties->cellWidget(3, 1))->text());
            ouSchedItem.m_chDataBytes[5] =  GetUnsignedInt(((LineEditWidget*)ui.tableProperties->cellWidget(4, 1))->text());
            ouSchedItem.m_chDataBytes[6] =  GetUnsignedInt(((LineEditWidget*)ui.tableProperties->cellWidget(5, 1))->text());
            ouSchedItem.m_chDataBytes[7] =  GetUnsignedInt(((LineEditWidget*)ui.tableProperties->cellWidget(6, 1))->text());

            std::ostringstream omStrStream;
            omStrStream << "["
                        << GetString(ouSchedItem.m_chDataBytes[0]).toStdString().c_str() << ","
                        << GetString(ouSchedItem.m_chDataBytes[1]).toStdString().c_str()
                        << "," << GetString(ouSchedItem.m_chDataBytes[2]).toStdString().c_str()
                        << "," << GetString(ouSchedItem.m_chDataBytes[3]).toStdString().c_str() << ","
                        << GetString(ouSchedItem.m_chDataBytes[4]).toStdString().c_str() << "," << GetString(ouSchedItem.m_chDataBytes[5]).toStdString().c_str() <<
                        "," << GetString(ouSchedItem.m_chDataBytes[6]).toStdString().c_str() << "," << GetString(ouSchedItem.m_chDataBytes[7]).toStdString().c_str() <<
                        "]";

            ui.tableProperties->item(7, 1)->setText(omStrStream.str().c_str());
            QVariant qVar;
            qVar.setValue(ouSchedItem);
            ui.tableFrames->item(ui.tableFrames->currentRow(), 0)->setData(Qt::UserRole, qVar);
        }
    }
}

void ScheduleTableDlg::onAssignFrameIdEditChange(const QString&)
{
    if(ui.tableFrames->currentRow() >= 0)
    {
        UID_ELEMENT uidECU = ((ScheduleComboWidget*)ui.tableProperties->cellWidget(0, 1))->currentData(Qt::UserRole).value<UID_ELEMENT>();

        IElement* pElement = nullptr;
        if(EC_SUCCESS == m_pouLDFCluster->GetElement(eEcuElement ,uidECU, &pElement))
        {
            EcuProperties ouEcuProps;
            ((IEcu*)pElement)->GetProperties(ouEcuProps);
            CSheduleTableItem ouSchedItem = ui.tableFrames->item(ui.tableFrames->currentRow(), 0)->data(Qt::UserRole).value<CSheduleTableItem>();

            ouSchedItem.m_chDataBytes[0] = GetUnsignedInt(((LineEditWidget*)ui.tableProperties->cellWidget(5, 1))->text());
            ouSchedItem.m_chDataBytes[1] =  GetUnsignedInt(((LineEditWidget*)ui.tableProperties->cellWidget(1, 1))->text());
            ouSchedItem.m_chDataBytes[2] =  GetUnsignedInt(((LineEditWidget*)ui.tableProperties->cellWidget(2, 1))->text());
            ouSchedItem.m_chDataBytes[3] =  GetUnsignedInt(((LineEditWidget*)ui.tableProperties->cellWidget(3, 1))->text());
            ouSchedItem.m_chDataBytes[4] =  GetUnsignedInt(((LineEditWidget*)ui.tableProperties->cellWidget(4, 1))->text());

            int nPCI = 0x06, nSID = 0xB1;
            std::ostringstream omStrStream;
            omStrStream << "[" << GetString(ouEcuProps.m_ouSlavePros.m_nConfiguredNAD).toStdString().c_str() << ","
                        <<  GetString(nPCI).toStdString().c_str() << "," << GetString(nSID).toStdString().c_str() << ","
                        << GetString(ouSchedItem.m_chDataBytes[0]).toStdString().c_str() << ","
                        << GetString(ouSchedItem.m_chDataBytes[1]).toStdString().c_str()
                        << "," << GetString(ouSchedItem.m_chDataBytes[2]).toStdString().c_str()
                        << "," << GetString(ouSchedItem.m_chDataBytes[3]).toStdString().c_str() << ","
                        << GetString(ouSchedItem.m_chDataBytes[4]).toStdString().c_str() << "]";

            ui.tableProperties->item(6, 1)->setText(omStrStream.str().c_str());
            QVariant qVar;
            qVar.setValue(ouSchedItem);
            ui.tableFrames->item(ui.tableFrames->currentRow(), 0)->setData(Qt::UserRole, qVar);
        }
    }
}

void ScheduleTableDlg::onComboAssignFrameIdFrameChange(int nIndex)
{
    UID_ELEMENT uidFrame = ((QComboBox*)ui.tableProperties->cellWidget(0, 1))->currentData(Qt::UserRole).value<UID_ELEMENT>();
    UID_ELEMENT uidECU = ((QComboBox*)ui.tableProperties->cellWidget(1, 1))->currentData(Qt::UserRole).value<UID_ELEMENT>();

    if(INVALID_UID_ELEMENT != uidFrame)
    {
        IElement* pFrame = nullptr, *pECU = nullptr;
        int nPid = 0xFF;
        if(EC_SUCCESS ==  m_pouLDFCluster->GetElement(eFrameElement, uidFrame, &pFrame))
        {
            unsigned int unFrameId;
            ((IFrame*)pFrame)->GetFrameId(unFrameId);

            nPid = ucCalculatePID(unFrameId);
            (ui.tableProperties->item(3, 1))->setText(GetString(nPid));
        }

        if(EC_SUCCESS == m_pouLDFCluster->GetElement(eEcuElement, uidECU, &pECU))
        {
            EcuProperties ouEcuProps;
            ((IEcu*)pECU)->GetProperties(ouEcuProps);

            ui.tableProperties->item(2, 1)->setText(GetString(ouEcuProps.m_ouSlavePros.m_nSupplierId));
            ui.tableProperties->item(3, 1)->setText(GetString(nPid));

            int nLSBSupplierId, nMSBSupplierId, nLSBFunctionId, nMSBFunctionId;
            nLSBSupplierId = ouEcuProps.m_ouSlavePros.m_nSupplierId & 0xFF;
            nMSBSupplierId = (ouEcuProps.m_ouSlavePros.m_nSupplierId & 0xFF00) >> 8;
            nLSBFunctionId = 0xFF;
            nMSBFunctionId = 0xFF;
            int nSID = 0xB1, nPCI = 6;
            std::ostringstream omStrStream;
            omStrStream << "[" << GetString(ouEcuProps.m_ouSlavePros.m_nConfiguredNAD).toStdString().c_str() << ","
                        <<  GetString(nPCI).toStdString().c_str() << ","
                        << GetString(nSID).toStdString().c_str() << "," << GetString(nLSBSupplierId).toStdString().c_str() << ","
                        << GetString(nMSBSupplierId).toStdString().c_str()
                        << "," << GetString(nLSBFunctionId).toStdString().c_str() << "," << GetString(nMSBFunctionId).toStdString().c_str()
                        << "," << GetString(nPid).toStdString().c_str() << "]";

            ui.tableProperties->item(4, 1)->setText(omStrStream.str().c_str());

            CSheduleTableItem ouSchedItem = ui.tableFrames->item(ui.tableFrames->currentRow(), 0)->data(Qt::UserRole).value<CSheduleTableItem>();
            ouSchedItem.m_nNode = uidECU;

            ouSchedItem.m_chDataBytes[7] = uidFrame;

            ouSchedItem.m_dDelay = ui.tableFrames->item(ui.tableFrames->currentRow(), 3)->text().toDouble();
            QVariant qVar;
            qVar.setValue(ouSchedItem);
            ui.tableFrames->item(ui.tableFrames->currentRow(), 0)->setData(Qt::UserRole, qVar);
        }
    }
}

void ScheduleTableDlg::onComboAssignFrameIdNodeChange(int nIndex)
{
    IElement* pElement = nullptr, *pFrame = nullptr;
    int nFramePID = 0xFF;
    UID_ELEMENT uidECU = ((ScheduleComboWidget*)ui.tableProperties->cellWidget(1, 1))->currentData(Qt::UserRole).value<UID_ELEMENT>();
    UID_ELEMENT uidFrame = ((ScheduleComboWidget*)ui.tableProperties->cellWidget(0, 1))->currentData(Qt::UserRole).value<UID_ELEMENT>();
    unsigned int unFrameId;
    if(EC_SUCCESS ==  m_pouLDFCluster->GetElement(eFrameElement, uidFrame, &pFrame))
    {
        ((IFrame*)pFrame)->GetFrameId(unFrameId);

        nFramePID = ucCalculatePID(unFrameId);
        ui.tableProperties->item(3, 1)->setText(GetString(nFramePID));
    }

    if(EC_SUCCESS == m_pouLDFCluster->GetElement(eEcuElement ,uidECU, &pElement))
    {

        EcuProperties ouEcuProps;
        ((IEcu*)pElement)->GetProperties(ouEcuProps);

        (ui.tableProperties->item(2, 1))->setText(GetString(ouEcuProps.m_ouSlavePros.m_nSupplierId));
        (ui.tableProperties->item(3, 1))->setText(GetString(nFramePID));

        int nLSBSupplierId, nMSBSupplierId, nLSBFunctionId, nMSBFunctionId;
        nLSBSupplierId = ouEcuProps.m_ouSlavePros.m_nSupplierId & 0xFF;
        nMSBSupplierId = (ouEcuProps.m_ouSlavePros.m_nSupplierId & 0xFF00) >> 8;
        nLSBFunctionId = 0xFF;
        nMSBFunctionId = 0xFF;

        int nSID = 0xB1, nPCI = 6;
        std::ostringstream omStrStream;
        omStrStream << "[" << GetString(ouEcuProps.m_ouSlavePros.m_nConfiguredNAD).toStdString().c_str() << ","
                    <<  GetString(nPCI).toStdString().c_str() << ","
                    << GetString(nSID).toStdString().c_str() << "," << GetString(nLSBSupplierId).toStdString().c_str()
                    << "," << GetString(nMSBSupplierId).toStdString().c_str()
                    << "," << GetString(nLSBFunctionId).toStdString().c_str() << "," << GetString(nMSBFunctionId).toStdString().c_str()
                    << "," << GetString(nFramePID).toStdString().c_str() << "]";

        ui.tableProperties->item(4, 1)->setText(omStrStream.str().c_str());

        CSheduleTableItem ouSchedItem = ui.tableFrames->item(ui.tableFrames->currentRow(), 0)->data(Qt::UserRole).value<CSheduleTableItem>();
        ouSchedItem.m_nNode = uidECU;

        ouSchedItem.m_chDataBytes[7] = uidFrame;

        ouSchedItem.m_dDelay = ui.tableFrames->item(ui.tableFrames->currentRow(), 3)->text().toDouble();
        QVariant qVar;
        qVar.setValue(ouSchedItem);
        ui.tableFrames->item(ui.tableFrames->currentRow(), 0)->setData(Qt::UserRole, qVar);
    }
}

ERRORCODE ScheduleTableDlg::vPopulateAssignFrameId(CSheduleTableItem& ouSchedTableItem)
{
    std::map<UID_ELEMENT, IElement*> mapEcus;
    std::map<UID_ELEMENT, IElement*> mapFrames;
    eEcuType ouECUType;
    std::string strName;

    int nSelectedRow = ui.tableFrames->currentRow();

    UID_ELEMENT unECUId = ouSchedTableItem.m_nNode;
    std::string strECUName;
    IElement* pElement = nullptr;

    // Node
    QComboBox* pComboBox = new QComboBox(nullptr);
    QComboBox* pFrameComboBox = new QComboBox(nullptr);
    pComboBox->setFrame(false);
    pFrameComboBox->setFrame(false);

    m_pouLDFCluster->GetElementList(eEcuElement, mapEcus);
    m_pouLDFCluster->GetElementList(eFrameElement, mapFrames);

    QVariant qVar;
    eFrameType ouFrameType;
    std::string omFrameName;
    int nFrameIndex = 0;
    unsigned int unFrameId;
    UID_ELEMENT uidFrame = INVALID_UID_ELEMENT;
for(auto itrFrame : mapFrames)
    {
        ((IFrame*)itrFrame.second)->GetFrameType(ouFrameType);

        if(ouFrameType == eLIN_Diagnostic || ouFrameType == eLIN_Sporadic)
        {
            continue;
        }
        ((IFrame*)itrFrame.second)->GetName(omFrameName);

        pFrameComboBox->addItem(QString::fromStdString(omFrameName));
        qVar.setValue(((IFrame*)itrFrame.second)->GetUniqueId());
        pFrameComboBox->setItemData(nFrameIndex, qVar, Qt::UserRole);
        nFrameIndex++;
    }

    if(pFrameComboBox->count() <= 0)
    {
        QMessageBox::critical(this, "Error", "Cannot create Assign Frame Id as there are no frames available", QMessageBox::Ok);
        ((QComboBox*)ui.tableFrames->cellWidget(nSelectedRow, 0))->setCurrentText("MasterReq");
        ouSchedTableItem.m_eDiagType = eLIN_MASTER_FRAME_ID;
        return EC_FAILURE;
    }

    //ui.tableProperties->clear();
    ui.tableProperties->setRowCount(0);
    ui.tableProperties->setRowCount(5);

    uidFrame = pFrameComboBox->itemData(0, Qt::UserRole).value<UID_ELEMENT>();
    if(ouSchedTableItem.m_chDataBytes[7] != 0xFF && ouSchedTableItem.m_chDataBytes[7] != INVALID_UID_ELEMENT)
    {
        uidFrame = ouSchedTableItem.m_chDataBytes[7];
    }
    else
    {
        ouSchedTableItem.m_chDataBytes[7] = uidFrame;
    }

    IElement* pFrame = nullptr;
    if(EC_SUCCESS == m_pouLDFCluster->GetElement(eFrameElement, uidFrame, &pFrame))
    {
        ((IFrame*)pFrame)->GetFrameId(unFrameId);
        std::string strFrameName;

        ((IFrame*)pFrame)->GetName(strFrameName);

        pFrameComboBox->setCurrentText(QString::fromStdString(strFrameName));
    }

    int nIndex = 0;
for(auto itrECU : mapEcus)
    {
        ((IEcu*)itrECU.second)->GetEcuType(ouECUType);

        if(ouECUType == eLIN_Slave)
        {
            itrECU.second->GetName(strName);
            pComboBox->addItem(QString::fromStdString(strName));
            qVar.setValue(((IEcu*)itrECU.second)->GetUniqueId());
            pComboBox->setItemData(nIndex, qVar, Qt::UserRole);
            nIndex++;
        }
    }

    if(unECUId == INVALID_UID_ELEMENT)
    {
        unECUId = pComboBox->itemData(0, Qt::UserRole).value<UID_ELEMENT>();
    }

    int nFramePID = ucCalculatePID(unFrameId);

    if(EC_SUCCESS == m_pouLDFCluster->GetElement(eEcuElement, unECUId, &pElement))
    {
        pElement->GetName(strECUName);
        pComboBox->setCurrentText(QString::fromStdString(strECUName));
    }

    connect(pComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(onComboAssignFrameIdNodeChange(int)));
    connect(pFrameComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(onComboAssignFrameIdFrameChange(int)));
    IEcu* pEcu = (IEcu*)pElement;

    EcuProperties ouEcuProps;
    pEcu->GetProperties(ouEcuProps);
    QList<QVariant> ouColumns;

    ouColumns.clear();
    std::string strColName = "Frame";
    ouColumns.push_back(QVariant(strColName.c_str()));
    ouColumns.push_back(QVariant());

    ui.tableProperties->InsertRow(0, ouColumns);
    ui.tableProperties->setCellWidget(0, 1, pFrameComboBox);

    ouColumns.clear();
    strColName = "Node Name";
    ouColumns.push_back(QVariant(strColName.c_str()));
    ouColumns.push_back(QVariant());

    ui.tableProperties->InsertRow(1, ouColumns);
    ui.tableProperties->setCellWidget(1, 1, pComboBox);

    ouColumns.clear();
    ouColumns.push_back(QVariant("Supplier ID"));
    ouColumns.push_back(QVariant(ouEcuProps.m_ouSlavePros.m_nSupplierId));
    ui.tableProperties->InsertRow(2, ouColumns);

    ouColumns.clear();
    ouColumns.push_back(QVariant("PID"));
    ouColumns.push_back(QVariant(nFramePID));
    ui.tableProperties->InsertRow(3, ouColumns);

    int nLSBSupplierId, nMSBSupplierId, nLSBFunctionId, nMSBFunctionId;
    nLSBSupplierId = ouEcuProps.m_ouSlavePros.m_nSupplierId & 0xFF;
    nMSBSupplierId = (ouEcuProps.m_ouSlavePros.m_nSupplierId & 0xFF00) >> 8;
    nLSBFunctionId = 0xFF;
    nMSBFunctionId = 0xFF;

    int nSID = 0xB1, nPCI = 6;
    std::ostringstream omStrStream;
    omStrStream << "[" << GetString(ouEcuProps.m_ouSlavePros.m_nConfiguredNAD).toStdString().c_str() << "," <<  GetString(nPCI).toStdString().c_str() << ","
                << GetString(nSID).toStdString().c_str() << "," << GetString(nLSBSupplierId).toStdString().c_str() << ","
                << GetString(nMSBSupplierId).toStdString().c_str()
                << "," << GetString(nLSBFunctionId).toStdString().c_str() << ","
                << GetString(nMSBFunctionId).toStdString().c_str() << "," << GetString(nFramePID).toStdString().c_str() << "]";
    ouColumns.clear();
    ouColumns.push_back(QVariant("PDU"));
    ouColumns.push_back(QVariant(omStrStream.str().c_str()));
    ui.tableProperties->InsertRow(4, ouColumns);

    ouSchedTableItem.m_nNode = unECUId;


    qVar.setValue(ouSchedTableItem);
    ui.tableFrames->item(ui.tableFrames->currentRow(), 0)->setData(Qt::UserRole, qVar);

    return EC_SUCCESS;
}

void ScheduleTableDlg::onFrameComboClicked(int nRow, int nCol)
{
    ui.tableFrames->selectRow(nRow);
}
