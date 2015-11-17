#include "EventFrameEditDlg.h"
#include "LDFDatabaseManager.h"
#include "IClusterProps.h"
#include "LDFUtility.h"
#include "Defines.h"
#include "LDFTagDefines.h"
#include "qabstractitemview.h"
#include "qmessagebox.h"
#include "qdialogbuttonbox.h"
EventFrameEditDlg::EventFrameEditDlg(ICluster* pCluster, IFrame** pouFrame, bool bNew, QWidget* parent)
{
    m_pouFrame = pouFrame;
    m_pLdfCluster = pCluster;
    m_bEditMode = !bNew;

    ui.setupUi(this);




    vPrepareValidations();
    vPrepareUiForNewMode();
    if ( true == m_bEditMode )
    {
        vPrepareUiForEditMode();
    }

    EnableElementsByVersion();
    ui.tableFrames->setEditTriggers( QAbstractItemView::NoEditTriggers);

    connect(ui.buttonOption, SIGNAL(accepted()), this, SLOT(OnButtonClickOk()));
    connect(ui.buttonOption, SIGNAL(rejected()), this, SLOT(reject()));

    QIcon ouWindowIcon;
    ouWindowIcon.addFile(":/LDFEditorIcons/Resources/icons/16x16/Frame.png", QSize(16, 16));
    ouWindowIcon.addFile(":/LDFEditorIcons/Resources/icons/22x22/Frame.png", QSize(16, 16));
    ouWindowIcon.addFile(":/LDFEditorIcons/Resources/icons/32x32/Frame.png", QSize(16, 16));

    setWindowIcon(ouWindowIcon);
}


EventFrameEditDlg::~EventFrameEditDlg(void)
{
}
void EventFrameEditDlg::vPrepareUiForEditMode()
{
    IFrame* pouFrame = *m_pouFrame;
    std::string strName;

    if ( nullptr != pouFrame )
    {
        FrameProps ouFrameProps;
        pouFrame->GetProperties(ouFrameProps);
        IElement* pTable;
        m_pLdfCluster->GetElement(eScheduleTableElement, ouFrameProps.m_ouLINEventTrigFrameProps.m_nCollisionResolveTable, &pTable);
        if ( pTable != nullptr )
        {
            pTable->GetName(strName);
            ui.comboCollisionTable->setCurrentText(strName.c_str());
        }

        QString strTemp;
        unsigned int nUid;
        pouFrame->GetFrameId(nUid);
        pouFrame->GetName(strName);
        ui.editName->setText(strName.c_str());

        QString strTitle = "Edit EventFrame - ";
        strTitle+= + strName.c_str();
        setWindowTitle(strTitle);


        ui.comboFrameId->setCurrentText(GetString(nUid));
    }

}


void EventFrameEditDlg::vPrepareUiForNewMode()
{
    setWindowTitle("Create EventFrame");
}

void EventFrameEditDlg::vPrepareValidations()
{
    ui.editName->setValidator(new QRegExpValidator(QRegExp(defIdentifier_RegExp)));
    vFillElementList();
}
void EventFrameEditDlg::vFillElementList()
{
    //Id
    m_FrameIdMap.clear();
    m_strFrameNames.clear();
    std::map<UID_ELEMENT, IElement*> pElementMap;
    m_pLdfCluster->GetElementList(eFrameElement, pElementMap);
    std::string strName;
    unsigned int unFrameid;
    unsigned int unCurrentFrameid = -1;
    FrameProps ouFrameProps;
    if ( true == m_bEditMode && (*m_pouFrame != nullptr ) )
    {
        (*m_pouFrame)->GetProperties(ouFrameProps);
    }

for ( auto itr : pElementMap )
    {
        ((IFrame*)itr.second)->GetFrameId(unFrameid);
        m_FrameIdMap[unFrameid] = unFrameid;
        vAddFrameToTable((IFrame*)itr.second, ouFrameProps.m_ouLINEventTrigFrameProps.m_pouUnconditionalFrame);
        (IFrame*)itr.second->GetName(strName);
        m_strFrameNames[strName] = strName;
    }
    if ( true == m_bEditMode && (*m_pouFrame != nullptr ) )
    {
        (*m_pouFrame)->GetFrameId(unCurrentFrameid);
        auto itr = m_FrameIdMap.find(unCurrentFrameid);
        m_FrameIdMap.erase(itr);
        (*m_pouFrame)->GetName(strName);
        auto itr2 = m_strFrameNames.find(strName);
        m_strFrameNames.erase(itr2);
    }

    QString strId = "";
    for ( int i = 0 ; i < 60; i++ )
    {
        if ( m_FrameIdMap.find(i) == m_FrameIdMap.end() )
        {
            strId = GetString(i);
            ui.comboFrameId->addItem(strId);
        }
    }
    if ( true == m_bEditMode )
    {
        ui.comboFrameId->setCurrentText(GetString(unCurrentFrameid));
    }
    vFillScheduleTables();

    EnableElementsByVersion();
}

void EventFrameEditDlg::vFillScheduleTables()
{
    ui.comboCollisionTable->addItem("", QVariant(INVALID_UID_ELEMENT));
    std::map<UID_ELEMENT, IElement*> pElementMap;
    m_pLdfCluster->GetElementList(eScheduleTableElement, pElementMap);
    std::string strName;
for ( auto itr : pElementMap )
    {
        itr.second->GetName(strName);
        ui.comboCollisionTable->addItem(strName.c_str(), QVariant(itr.second->GetUniqueId()));
    }
}

void EventFrameEditDlg::vAddFrameToTable(IFrame* pFrame, std::map<UID_ELEMENT, UID_ELEMENT> ouSelectedElements)
{
    if ( nullptr == pFrame )
    {
        return;
    }

    eFrameType ouFrameType;
    EcuProperties ouEcuProps;
    std::list<IEcu*> ouEcuList;
    pFrame->GetFrameType(ouFrameType);
    pFrame->GetEcus(eTx, ouEcuList);
    auto itrTxEcu  = ouEcuList.begin();

    if ( itrTxEcu == ouEcuList.end() )
    {
        return;
    }

    ((IEcu*)*itrTxEcu)->GetProperties(ouEcuProps);

    if ( eLIN_Unconditional == ouFrameType && ouEcuProps.m_eEcuType == eLIN_Slave )
    {
        std::string strName;
        std::string strTxEcu;
        std::string strRxEcu;
        unsigned int unId;
        FrameProps ouFrameprops;

        pFrame->GetName(strName);
        pFrame->GetFrameId(unId);
        pFrame->GetProperties(ouFrameprops);
        ((IEcu*)*itrTxEcu)->GetName(strTxEcu);

        QList<QVariant> ouCloumns;
        ouCloumns.push_back(QVariant(QString::fromStdString(strName)));
        ouCloumns.push_back(QVariant(ouFrameprops.m_ouLINUnConditionFrameProps.m_unId));
        ouCloumns.push_back(QVariant(GetString(ouFrameprops.m_ouLINUnConditionFrameProps.m_nLength, 10)));
        ouCloumns.push_back(QVariant(QString::fromStdString(strTxEcu)));
        ouEcuList.clear();
        pFrame->GetEcus(eRx, ouEcuList);
        nGetSubscribers(ouEcuList, strRxEcu);
        ouCloumns.push_back(QVariant(QString::fromStdString(strRxEcu)));
        ui.tableFrames->setRowCount(ui.tableFrames->rowCount()+1);
        ui.tableFrames->InsertRow(ui.tableFrames->rowCount()-1, ouCloumns);
        if ( ouSelectedElements.find(pFrame->GetUniqueId()) == ouSelectedElements.end() )
        {
            ui.tableFrames->item(ui.tableFrames->rowCount()-1, 0)->setCheckState(Qt::Unchecked);
        }
        else
        {
            ui.tableFrames->item(ui.tableFrames->rowCount()-1, 0)->setCheckState(Qt::Checked);
        }
        ui.tableFrames->item(ui.tableFrames->rowCount()-1, 0)->setData(Qt::UserRole, QVariant(pFrame->GetUniqueId()));
    }
}

void EventFrameEditDlg::PopulateFrameIdCombo()
{
    ui.comboFrameId->clear();
    QString strValue;
    for(unsigned int unIndex = 0; unIndex <= MAX_UNCOND_FRAME_ID; unIndex++)
    {
        strValue = QString::number(unIndex, 16);
        ui.comboFrameId->addItem(strValue.toUpper());
    }
}

void EventFrameEditDlg::OnButtonClickOk()
{
    if ( 0 == nValidateValues() )
    {
        IFrame* pFrame;
        FrameProps ouFrameProps;
        if ( true == m_bEditMode )
        {
            pFrame = *m_pouFrame;
        }
        else
        {
            m_pLdfCluster->CreateElement(eFrameElement, (IElement**)&pFrame);
            *m_pouFrame = pFrame;
        }
        if ( nullptr != pFrame )
        {
            pFrame->GetProperties(ouFrameProps);

            ouFrameProps.m_ouLINEventTrigFrameProps.m_pouUnconditionalFrame.clear();

            //Frame Type
            ouFrameProps.m_eFrameType = eLIN_EventTriggered;

            //Frame id
            ouFrameProps.m_ouLINEventTrigFrameProps.m_unId = GetUnsignedInt(ui.comboFrameId->currentText());

            //Conditional Frames
            int nRow = ui.tableFrames->rowCount();
            IFrame* pouTempFrame;
            for ( int i = 0 ; i < nRow; i++ )
            {
                QTableWidgetItem* pItem = ui.tableFrames->item(i, 0);
                UID_ELEMENT uid = pItem->data(Qt::UserRole).value<UID_ELEMENT>();
                if ( Qt::Checked == pItem->checkState() )
                {
                    m_pLdfCluster->GetElement(eFrameElement, uid, (IElement**)&pouTempFrame);
                    if ( nullptr != pouTempFrame )
                    {
                        ouFrameProps.m_ouLINEventTrigFrameProps.m_pouUnconditionalFrame[uid] = uid;
                    }
                }
            }

            //Collision Table;
            UID_ELEMENT uidTable = ui.comboCollisionTable->currentData().value<UID_ELEMENT>();
            if ( INVALID_DATA != uidTable )
            {
                ouFrameProps.m_ouLINEventTrigFrameProps.m_nCollisionResolveTable = uidTable;
            }
            pFrame->SetProperties(ouFrameProps);

            //Frame Name
            pFrame->SetName(ui.editName->text().toStdString());
        }
        LDFDatabaseManager::GetDatabaseManager()->setDocumentModified(true);
        accept();
    }
    else
    {
    }
}
int EventFrameEditDlg::nValidateValues()
{
    if ( 0 == ui.editName->text().length() )
    {
        QMessageBox::critical(this, "Error", "Name Should Not Be Empty",  QMessageBox::Ok);
        ui.editName->setFocus();
        return -1;
    }

    if ( m_strFrameNames.find(ui.editName->text().toStdString()) != m_strFrameNames.end() )
    {
        QMessageBox::critical(this, "Error", "Name Already Exists",  QMessageBox::Ok);
        ui.editName->setFocus();
        return -1;
    }

    LIN_Settings ouLINSettings;
    LDFDatabaseManager::GetDatabaseManager()->GetLDFCluster()->GetProperties(eLINClusterProperties, &ouLINSettings);

    if((ouLINSettings.m_dProtocolVers != defVersion_1_3 && ouLINSettings.m_dProtocolVers != defVersion_2_0) )
    {
        if ( ui.comboCollisionTable->currentText().length() == 0 )
        {
            QMessageBox::StandardButton choice;
            choice = QMessageBox::warning(this, "Warning", "Schedule Table is not Associated.<br>Do you Want to Continue?</br>",  QMessageBox::Yes|QMessageBox::No);
            if ( choice == QMessageBox::No )
            {
                return -1;
            }
        }
        else
        {
            //Conditional Frames
            std::map<UID_ELEMENT, UID_ELEMENT> ouSelectedItems;
            int nRow = ui.tableFrames->rowCount();
            IFrame* pouTempFrame;
            for ( int i = 0 ; i < nRow; i++ )
            {
                QTableWidgetItem* pItem = ui.tableFrames->item(i, 0);
                UID_ELEMENT uid = pItem->data(Qt::UserRole).value<UID_ELEMENT>();
                if ( Qt::Checked == pItem->checkState() )
                {
                    m_pLdfCluster->GetElement(eFrameElement, uid, (IElement**)&pouTempFrame);
                    if ( nullptr != pouTempFrame )
                    {
                        ouSelectedItems[uid] = uid;
                    }
                }
            }
            IScheduleTable* pTable;
            ScheduleTableProps ouTableProps;

            UID_ELEMENT uid = ui.comboCollisionTable->currentData().value<UID_ELEMENT>();
            m_pLdfCluster->GetElement(eScheduleTableElement, uid, (IElement**)&pTable);
            if ( nullptr != pTable )    //Else case may not required
            {
                pTable->GetProperties(ouTableProps);
for ( auto itr : ouTableProps.m_ouCSheduleTableItem )
                {
                    auto itr2 = ouSelectedItems.find(itr.m_nFrameId);
                    if ( itr2 != ouSelectedItems.end() )
                    {
                        ouSelectedItems.erase(itr2);
                    }
                }
            }
            if ( ouSelectedItems.size() > 0 )
            {
                QMessageBox::StandardButton choice = QMessageBox::warning(this, "Warning", "Selected Schedule Table does not contain all the Frames Mapped.\
					<br>Do you Want to Continue?</br>",  QMessageBox::Yes|QMessageBox::No);

                if(choice == QMessageBox::No)
                {
                    return -1;
                }
            }
            return 0;
        }
    }

    return 0;
}

void EventFrameEditDlg::EnableElementsByVersion()
{
    LIN_Settings ouLINSettings;
    LDFDatabaseManager::GetDatabaseManager()->GetLDFCluster()->GetProperties(eLINClusterProperties, &ouLINSettings);

    if(ouLINSettings.m_dProtocolVers == defVersion_1_3 || ouLINSettings.m_dProtocolVers == defVersion_2_0)
    {
        ui.comboCollisionTable->setEnabled(false);
    }
}