#include "ldfadditionalview.h"
#include "qheaderview.h"
#include "LDFDatabaseManager.h"
#include "LDFUtility.h"
#include <algorithm>
#include "LDFDefines.h"
#include "LineEditWidget.h"
LDFAdditionalView::LDFAdditionalView(QWidget* parent)
    : LDFCommonTableWidget(parent)
{
    m_pouLDFDatabaseManager = LDFDatabaseManager::GetDatabaseManager();
    m_pouLdfCluster = m_pouLDFDatabaseManager->GetLDFCluster();

    QHeaderView* pHeader = horizontalHeader();

    VALIDATE_POINTER(pHeader);

    connect(verticalHeader(), SIGNAL(sectionMoved(int,int, int)), this, SLOT(tableItemMoved(int,int,int)));
    m_pHelpLabel = nullptr;
}

LDFAdditionalView::~LDFAdditionalView()
{
}

void LDFAdditionalView::tableItemMoved(int nLogical, int nOld, int nNew)
{
    VALIDATE_POINTER(m_pCurrentEcu);

    LinEcuProps ouEcuProps;
    ouEcuProps.m_eEcuType = eEcuNone;
    m_pCurrentEcu->GetProperties(ouEcuProps);
    if ( ouEcuProps.m_eEcuType != eSlave )
    {
        return;
    }

    LDFDatabaseManager::GetDatabaseManager()->setDocumentModified(true);

    //ouEcuProps.m_nConfiurableFrameIdList.clear();

    int nRows = rowCount();

    for ( int  i = 0 ; i < nRows; i++ )
    {
        int nVisualRow = verticalHeader()->logicalIndex(i);
        QTableWidgetItem* pItem = item(nVisualRow,0);

        if(nullptr != pItem)
        {
            ConfigFrameDetails ouConfigFrame = pItem->data(Qt::UserRole).value<ConfigFrameDetails>();

            ouEcuProps.mSlaveProps.m_nConfiurableFrameIdList.push_back(ouConfigFrame);
        }
    }

    m_pCurrentEcu->SetProperties(ouEcuProps);
    DisplayEcuAdditionalProps(m_pCurrentEcu);

    QTableWidgetItem* pItem = item(nNew,0);
    if ( nullptr != pItem )
    {
        setCurrentItem(pItem);
    }
}

void LDFAdditionalView::onTreeItemClicked(QTreeWidgetItem* pouTreeItem, int nId)
{
    if ( m_pHelpLabel != nullptr )
    {
        m_pHelpLabel->setText(" ");
    }
    TreeItemID ouTreeItemID = pouTreeItem->data(0,  Qt::UserRole).value<TreeItemID>();
    if ( ouTreeItemID.m_eTreeElementType == eLdfItem && ouTreeItemID.m_uidElement != 0 )
    {
        ICluster* pouCluster = m_pouLDFDatabaseManager->GetLDFCluster();
        IElement* pouElement;
        VALIDATE_POINTER(pouCluster);
        pouCluster->GetElement(pouCluster->GetElementType(ouTreeItemID.m_uidElement), ouTreeItemID.m_uidElement, &pouElement);
        VALIDATE_POINTER(pouElement);
        vDisplayElement(pouElement);
    }

    else if ( ouTreeItemID.m_eTreeElementType == eTreeHeader )
    {
        switch(ouTreeItemID.m_uidElement)
        {
            default:
                clear();
                setRowCount(0);
                setColumnCount(0);
                break;
        }
    }
}
void LDFAdditionalView::SetInfoLable(QLabel* pHelpLabel)
{
    m_pHelpLabel = pHelpLabel;
}

void LDFAdditionalView::vDisplayElement ( IElement* pouLdfElement)
{
    if ( nullptr == pouLdfElement )
    {
        return ;
    }

    eClusterElementType eElementType = pouLdfElement->GetElementType();
    verticalHeader()->setSectionsMovable(false);
    m_pCurrentEcu = nullptr;
    switch (eElementType)
    {
        case eFrameElement:
            DisplayFrameAdditionalProps((IFrame*)pouLdfElement);
            break;
        case eSignalElement:
            DisplaySignalAdditionalProps((ISignal*)pouLdfElement);
            break;
        case eEcuElement:
            DisplayEcuAdditionalProps((IEcu*)pouLdfElement);
            break;
        case eCodingElement:
            DisplayCodingAdditionalProps((ICoding*)pouLdfElement);
            break;
        case eScheduleTableElement:
            clear();
            setColumnCount(0);
            setRowCount(0);
            break;
        case eSignalGroupElement:
            DisplaySignalGroupAdditionalProps((ISignalGroup*)pouLdfElement);
        default:
            break;
    }
}
int LDFAdditionalView::DisplayScedulingTableAdditionalProps(IScheduleTable* pouTable, int nItemIndex )
{
    VALIDATE_AND_RETURN(pouTable);
    if ( nullptr == pouTable )
    {
        return -1;
    }

    ScheduleTableProps ouTableProps;
    pouTable->GetProperties(ouTableProps);

    auto itrTableItem = ouTableProps.m_ouCLINSheduleTableItem.begin();
    std::advance(itrTableItem, nItemIndex);

    if ( ouTableProps.m_ouCLINSheduleTableItem.end() != itrTableItem )
    {
        DisplayScheduleTableFrameProps(*itrTableItem);
    }

    return 0;
}

int LDFAdditionalView::DisplayScheduleTableFrameProps(CLINSheduleTableItem& ouTableItem)
{
    switch (ouTableItem.m_eDiagType)
    {
        case eLIN_MASTER_FRAME_ID:
        case eLIN_NORMAL_FRAME_ID:
        case eLIN_SLAVE_FRAME_ID:
        {
            IElement* pouElement;
            m_pouLdfCluster->GetElement(eFrameElement, ouTableItem.m_nFrameId, &pouElement);
            if(nullptr != pouElement)
            {
                DisplayFrameAdditionalProps((IFrame*)pouElement);
            }
        }
        break;

        case eLIN_SID_ASSIGN_FRAME_ID:
        case eLIN_SID_UNASSIGN_FRAME_ID:
        case eLIN_SID_ASSIGN_NAD_ID:
        case eLIN_SID_ASSIGN_FRAME_RANGE:

        case eLIN_SID_COND_CHANGE_NAD:
        case eLIN_SID_DATA_DUMP:
        case eLIN_SID_SAVE_CONFIG:
        case eLIN_SID_FREEFORMAT:
            //DisplayDiagProps(ouTableItem);
            clear();
            setColumnCount(0);
            setRowCount(0);
        default:
            break;
    }
    return 0;
}

int LDFAdditionalView::DisplaySignalGroupAdditionalProps(ISignalGroup* pouLdfElement)
{
    QList<QVariant> ouValueList;
    std::string strTemp;
    SignalGroupProps ouSignalGroupProps;
    std::map<ISignal*, SignalInstanse> ouSignalList;
    pouLdfElement->GetSignalList(ouSignalList);

    QList<QString> strColumnList;
    strColumnList << "Signal Name" << "Offset";
    CreateColumns(strColumnList);
    setRowCount(ouSignalList.size());
    int nRow = 0;
for ( auto itr : ouSignalList )
    {
        ouValueList.clear();

        itr.first->GetName(strTemp);
        ouValueList.push_back(strTemp.c_str());
        ouValueList.push_back(GetString(itr.second.m_nStartBit, 10));
        InsertRow(nRow, ouValueList);
        nRow++;
    }

    return 0;
}

int LDFAdditionalView::DisplayDiagProps(CLINSheduleTableItem& ouTableItem)
{
    QList<QString> strColumnList;
    strColumnList <<"Propert Name" << "Value";
    CreateColumns(strColumnList);
    setRowCount(1);
    QList<QVariant> ouRowValue;
    ouRowValue.push_back("Data Bytes");
    QString strVal;
    strVal.sprintf("{0x%X, 0x%X, 0x%X, 0x%X, 0x%X, 0x%X, 0x%X, 0x%X}", ouTableItem.m_chDataBytes[0],
                   ouTableItem.m_chDataBytes[1],
                   ouTableItem.m_chDataBytes[2],
                   ouTableItem.m_chDataBytes[3],
                   ouTableItem.m_chDataBytes[4],
                   ouTableItem.m_chDataBytes[5],
                   ouTableItem.m_chDataBytes[6],
                   ouTableItem.m_chDataBytes[7]
                  );
    ouRowValue.push_back(strVal);

    InsertRow(0, ouRowValue);

    return 0;
}

int LDFAdditionalView::DisplayAssignFrameIdProps(CLINSheduleTableItem& ouTableItem)
{
    return 0;
}

int LDFAdditionalView::DisplaySignalAdditionalProps(ISignal* pouLdfElement)
{
    VALIDATE_AND_RETURN(pouLdfElement);

    ICoding* ouCoding;
    pouLdfElement->GetEncoding(&ouCoding);

    QList<QString> strColumns;
    strColumns <<"Encoding Type" << "Min"<<"Max" << "Factor" << "Offset" <<"Unit" <<"Value"  <<"Description" ;
    CreateColumns(strColumns);

    if ( nullptr != m_pHelpLabel )
    {
        std::string strName;
        pouLdfElement->GetName(strName);
        m_pHelpLabel->setText(tr("Codings of signal <b>%1</b>").arg(strName.c_str()));
    }

    if ( nullptr != ouCoding )
    {
        LINCompuMethods ouCompuMethods;
        ouCompuMethods.m_eType = eInvalidProtocol;
        ouCoding->GetProperties(ouCompuMethods);

        QList<QVariant> ouColumnList;
        int nRow = 0;
        setRowCount(ouCompuMethods.m_ouPhysicalValueList.size()+ouCompuMethods.m_ouLogicalValueList.size());

for ( auto itr : ouCompuMethods.m_ouPhysicalValueList )
        {
            ouColumnList.clear();
            ouColumnList.push_back(QVariant("Physical"));
            ouColumnList.push_back(QVariant(itr.m_unMin));
            ouColumnList.push_back(QVariant(itr.m_unMax));
            ouColumnList.push_back(QVariant(itr.m_dFactor));
            ouColumnList.push_back(QVariant(itr.m_dOffset));
            ouColumnList.push_back(QVariant(itr.m_strTextInfo.c_str()));
            ouColumnList.push_back(QVariant("-"));
            ouColumnList.push_back(QVariant("-"));
            InsertRow(nRow++, ouColumnList);
        }

for ( auto itr : ouCompuMethods.m_ouLogicalValueList)
        {
            ouColumnList.clear();
            ouColumnList.push_back(QVariant("Logical"));
            ouColumnList.push_back(QVariant("-"));
            ouColumnList.push_back(QVariant("-"));
            ouColumnList.push_back(QVariant("-"));
            ouColumnList.push_back(QVariant("-"));
            ouColumnList.push_back(QVariant("-"));
            ouColumnList.push_back(QVariant(itr.first));
            ouColumnList.push_back(QVariant(QString::fromStdString(itr.second)));
            InsertRow(nRow++, ouColumnList);
        }
    }
    return 0;
}
int LDFAdditionalView::DisplayCodingAdditionalProps(ICoding* pouLdfElement)
{
    if ( nullptr != m_pHelpLabel )
    {
        m_pHelpLabel->setText(" ");

    }
    clear();
    setColumnCount(0);
    setRowCount(0);
    return 0;
}

int LDFAdditionalView::DisplayEcuAdditionalProps( IEcu* pouEcu)
{
    VALIDATE_AND_RETURN(pouEcu);
    m_pCurrentEcu = pouEcu;
    /*std::list<IFrame*> ouTxFrameList;
    std::list<IFrame*> ouRxFrameList;
    std::map<IFrame*, std::string> ouFrameDir;          //1- tx, 2- rx, 3-both

    pouEcu->GetFrameList(eTx, ouTxFrameList);
    for ( auto itr : ouTxFrameList )
    {
    ouFrameDir[itr]  = "Tx";
    }

    int nTxCount = ouTxFrameList.size();
    ouTxFrameList.clear();
    pouEcu->GetFrameList(eRx, ouTxFrameList);
    for ( auto itr : ouTxFrameList )
    {
    if ( ouFrameDir.find(itr) != ouFrameDir.end() )
    {
    if ( ouFrameDir[itr] == "Tx" ) ouFrameDir[itr] = "Tx & Rx";
    }
    else
    {
    ouFrameDir[itr] = "Rx";
    }
    }
    */
    LinEcuProps ouEcuProperties;
    ouEcuProperties.m_eEcuType = eEcuNone;
    pouEcu->GetProperties(ouEcuProperties);
    if ( ouEcuProperties.m_eEcuType == eSlave )
    {
        DisplaySlaveAdditionalProps(pouEcu);
    }
    else if ( ouEcuProperties.m_eEcuType == eMaster )
    {
        DisplayMasterAdditionalProps(pouEcu);
    }
    else
    {
        setRowCount(0);
        setColumnCount(0);
    }
    return 0;
}

int LDFAdditionalView::DisplayMasterAdditionalProps(IEcu* pouEcu)
{
    VALIDATE_AND_RETURN(pouEcu);
    if ( m_pHelpLabel != nullptr )
    {
        std::string strName;
        pouEcu->GetName(strName);
        m_pHelpLabel->setText(tr("Frames Associated with the Master Ecu <b>%1</b>").arg(strName.c_str()));
    }

    std::list<IFrame*> ouFrameList;
    std::list<ConfigFrameDetails> ouUidList;

    pouEcu->GetFrameList(eTx, ouFrameList);
    ConfigFrameDetails ouConfigFrame;
for ( auto itr : ouFrameList )
    {
        ouConfigFrame.m_uidFrame = itr->GetUniqueId();
        ouUidList.push_back(ouConfigFrame);
    }

    pouEcu->GetFrameList(eRx, ouFrameList);
for ( auto itr : ouFrameList )
    {
        ouConfigFrame.m_uidFrame = itr->GetUniqueId();
        ouUidList.push_back(ouConfigFrame);
    }
    ouUidList.sort();
    ouUidList.unique();
    return DisplayConfigFrames(pouEcu, ouUidList);
}

int LDFAdditionalView::DisplayConfigFrames(IEcu* pouEcu, std::list<ConfigFrameDetails>& ouFrameUidList)
{
    int nRow = 0;
    QList<QVariant> ouValueList;
    std::string strTemp;
    unsigned int unId = -1;
    LinFrameProps ouFrameProps;
    IFrame*  pouFrame = nullptr;

    LinEcuProps ouEcuProperties;
    ouEcuProperties.m_eEcuType = eEcuNone;
    pouEcu->GetProperties(ouEcuProperties);
    QList<QString> strColumnList;



    strColumnList << "Frame Name" << "Frame Id" << "Length (Byte(s))";

    if ( ( ouEcuProperties.m_eEcuType == eSlave ) && ( bAreEqual ( ouEcuProperties.mSlaveProps.m_fProtocolVersion, defVersion_2_0) == true ) )
    {
        strColumnList << "Message Id";
    }

    CreateColumns(strColumnList);
    setRowCount(ouFrameUidList.size());

for ( auto itr : ouFrameUidList )
    {
        ouValueList.clear();
        m_pouLdfCluster->GetElement(eFrameElement, itr.m_uidFrame, (IElement**)&pouFrame);
        if ( nullptr != pouFrame )
        {
            pouFrame->GetName(strTemp);
            pouFrame->GetFrameId(unId);
            pouFrame->GetProperties(ouFrameProps);
            ouValueList.push_back(QVariant(QString::fromStdString(strTemp)));
            if ( ouFrameProps.m_eLinFrameType == eLinDiagnosticFrame )
            {
                ouValueList.push_back(QVariant(ouFrameProps.m_nMsgId));
                ouValueList.push_back(QVariant(GetString(ouFrameProps.m_unMsgSize, 10)));
                InsertRow(nRow, ouValueList);
                QVariant qVar;
                qVar.setValue(itr);
                item(nRow, 0)->setData(Qt::UserRole, qVar);
                nRow++;
            }
			else if (ouFrameProps.m_eLinFrameType == eLinUnconditionalFrame)
            {
                ouValueList.push_back(QVariant(ouFrameProps.m_nMsgId));
                ouValueList.push_back(QVariant(GetString(ouFrameProps.m_unMsgSize, 10)));
                InsertRow(nRow, ouValueList);

                if ( bAreEqual ( ouEcuProperties.mSlaveProps.m_fProtocolVersion, defVersion_2_0) == true )
                {
                    LineEditWidget* pLineWidget = new LineEditWidget(nRow, this);
                    this->setCellWidget(nRow, 3, pLineWidget);
                    if ( LDFDatabaseManager::GetDatabaseManager()->bIsDisplayHexOn() == false )
                    {
                        QIntValidator* pValidator = new QIntValidator();
                        pLineWidget->setValidator(pValidator);
                        //pValidator->setRange(0, 0xFFFE);
                    }
                    else
                    {
                        pLineWidget->setValidator(new QRegExpValidator(QRegExp(defHex_RegExp)));
                    }


                    pLineWidget->setText(GetString(itr.m_unConfigMsgId));
                    connect(pLineWidget, SIGNAL(textChanged ( const QString&)), this, SLOT(onConfigFrameEditChange(const QString&)));

                }
                QVariant qVar;
                qVar.setValue(itr);
                item(nRow, 0)->setData(Qt::UserRole, qVar);
                nRow++;
            }
        }
    }

    return 0;
}
void LDFAdditionalView::onConfigFrameEditChange(const QString& strText )
{
    if(currentRow() >= 0 && m_pCurrentEcu != nullptr)
    {
        int nRow = currentRow();
        LinEcuProps ouEcuProps;
        ouEcuProps.m_eEcuType = eEcuNone;
        m_pCurrentEcu->GetProperties(ouEcuProps);
        auto itrFrame = ouEcuProps.mSlaveProps.m_nConfiurableFrameIdList.begin();
        std::advance(itrFrame, nRow);
        if ( itrFrame != ouEcuProps.mSlaveProps.m_nConfiurableFrameIdList.end())
        {
            itrFrame->m_unConfigMsgId =  GetUnsignedInt(strText);
            if ( itrFrame->m_unConfigMsgId > 0xFFFE )
            {
                itrFrame->m_unConfigMsgId = 0;
            }
            ConfigFrameDetails ouDetails = *itrFrame;
            QVariant qVar;
            qVar.setValue(ouDetails);
            auto rowItem = item(currentRow(), 0);
            if ( nullptr != rowItem )
            {
                rowItem->setData(Qt::UserRole, qVar);
            }
            LDFDatabaseManager::GetDatabaseManager()->GetLDFCluster()->EnableEventNotofications(false);
            m_pCurrentEcu->SetProperties(ouEcuProps);   //No clients need to know about this changes.
            LDFDatabaseManager::GetDatabaseManager()->GetLDFCluster()->EnableEventNotofications(true);
            LDFDatabaseManager::GetDatabaseManager()->setDocumentModified(true);
        }

    }

}

int LDFAdditionalView::DisplaySlaveAdditionalProps(IEcu* pouEcu)
{
    VALIDATE_AND_RETURN(pouEcu);

    /*QTabWidget *o = new QTabWidget();
    o->addTab(this, "Test");
    o->show();
    o->*/
    LinEcuProps ouEcuProperties;
    ouEcuProperties.m_eEcuType = eEcuNone;
    pouEcu->GetProperties(ouEcuProperties);

    if ( m_pHelpLabel != nullptr )
    {
        std::string strName;
        pouEcu->GetName(strName);
        if ( ouEcuProperties.mSlaveProps.m_fProtocolVersion == 1.3 )
        {
            m_pHelpLabel->setText(tr("Frames Associated with the Ecu <b>%1</b>").arg(strName.c_str()));
            verticalHeader()->setSectionsMovable(false);
        }
        else
        {
            m_pHelpLabel->setText(tr("Configurable Frame List of Ecu <b>%1</b><br>Drag & Drop to change the order</br>").arg(strName.c_str()));
            verticalHeader()->setSectionsMovable(true);
        }
    }

    std::list<ConfigFrameDetails> ouUidList;

for ( auto itr : ouEcuProperties.mSlaveProps.m_nConfiurableFrameIdList )
    {
        ouUidList.push_back(itr);
    }
    DisplayConfigFrames(pouEcu, ouUidList);

    return 0;
}

int LDFAdditionalView::DisplayFrameAdditionalProps( IFrame* pouFrame)
{
    VALIDATE_AND_RETURN(pouFrame);

    LinFrameProps ouFrameProps;
	ouFrameProps.m_eLinFrameType = eLinInvalidFrame;
    pouFrame->GetProperties(ouFrameProps);

	switch (ouFrameProps.m_eLinFrameType)
    {
	case eLinUnconditionalFrame:
            DisplayUnConditionalFrameProps(pouFrame);
            break;
        case eLinSporadicFrame:
            DisplaySporadicProps(pouFrame);
            break;
        case eLinEventTriggeredFrame:
            DisplayEventTrigProps(pouFrame);
            break;
        case eLinDiagnosticFrame:
            DisplayDiagProps(pouFrame);
            break;
    };

    return 0;
}

int LDFAdditionalView::DisplayUnConditionalFrameProps(IFrame* pouFrame)
{
    VALIDATE_AND_RETURN(pouFrame);

    std::map<ISignal*, SignalInstanse> mapSignals;

    pouFrame->GetSignalList(mapSignals);

    int nRowCount = mapSignals.size() ;

    QList<QString> strColumnList;
    strColumnList <<"Signal" << "Startbit" << "Publisher" << "Subscribers" << "Length (Bit(s))" << "Init Value" << "Coding";

    CreateColumns(strColumnList);
    setRowCount(nRowCount);

    LINSignalProps ouSignalProps;
    ouSignalProps.eType = eInvalidProtocol;
    int nRow = 0;
    QList<QVariant> ouValueList;
    std::list<IEcu*> lstTxEcus, lstRxEcus;
    ICoding* pCoding = nullptr;

    std::string strSigName, strPublishers, strSubscribers, strEncoding, strUnit, strCoding;
    unsigned int unId;
    CompuMethodProps ouCompuProps;
    ouCompuProps.m_eType = eInvalidProtocol;
    if ( nullptr != m_pHelpLabel )
    {
        std::string strName;
        pouFrame->GetName(strName);
        m_pHelpLabel->setText(tr("Signals mapped for Unconditional Frame <b>%1</b>").arg(strName.c_str()));
    }
for ( auto itr : mapSignals )
    {
        ouValueList.clear();
        itr.first->GetName(strSigName);
        itr.first->GetProperties(ouSignalProps);

        strPublishers = "";
        lstTxEcus.clear();
        itr.first->GetEcus(eTx, lstTxEcus);
        nGetSubscribers(lstTxEcus, strPublishers);

        strSubscribers = "";
        lstRxEcus.clear();
        itr.first->GetEcus(eRx, lstRxEcus);
        nGetSubscribers(lstRxEcus, strSubscribers);

        itr.first->GetEncoding(&pCoding);

        ouValueList.push_back(QVariant(QString::fromStdString(strSigName)));
        ouValueList.push_back(QVariant(GetString(itr.second.m_nStartBit, 10)));                         //In dec always
        ouValueList.push_back(QVariant(QString::fromStdString(strPublishers)));
        ouValueList.push_back(QVariant(QString::fromStdString(strSubscribers)));
        ouValueList.push_back(QVariant(GetString(ouSignalProps.m_unSignalSize, 10)));     //In dec always
        ouValueList.push_back(QVariant(ouSignalProps.m_nIntialValue));

        strEncoding = "", strUnit = "", strCoding = "";
        if(nullptr != pCoding)
        {
            pCoding->GetName(strCoding);
        }

        ouValueList.push_back(QVariant(QString::fromStdString(strCoding)));

        InsertRow(nRow++, ouValueList);
    }
    return 0;
}

int LDFAdditionalView::DisplayEventTrigProps( IFrame* pouFrame)
{
    VALIDATE_AND_RETURN(pouFrame);

    LinFrameProps ouFrameProps;
    ouFrameProps.m_eLinFrameType = eLinInvalidFrame;
    pouFrame->GetProperties(ouFrameProps);

    std::list<IFrame*> lstUnCondFrames;
for ( auto itr : ouFrameProps.m_ouLINEventTrigFrameProps.m_pouUnconditionalFrame)
    {
        IFrame* pouFrame = nullptr;
        m_pouLdfCluster->GetElement(eFrameElement, UID_ELEMENT(itr.first), (IElement**)&pouFrame);
        if ( nullptr != pouFrame )
        {
            lstUnCondFrames.push_back(pouFrame);
        }
    }

    QList<QString> strColumnList;
    strColumnList <<"Frame" << "ID" << "Length" << "Publisher" << "Subscribers";

    CreateColumns(strColumnList);
    setRowCount(lstUnCondFrames.size());

    int nRow = 0;
    QList<QVariant> ouValueList;
    std::list<IEcu*> lstTxEcus, lstRxEcus;

    std::string strFrameName, strPublishers, strSubscribers, strEncoding, strUnit, strCoding;
    unsigned int unId;
    CompuMethodProps ouCompuProps;
    ouCompuProps.m_eType = eInvalidProtocol;
    if ( nullptr != m_pHelpLabel )
    {
        std::string strName;
        pouFrame->GetName(strName);
        m_pHelpLabel->setText(tr("Unconditional Frames associated to Eventtriggered Frame <b>%1</b>").arg(strName.c_str()));
    }

for ( auto itr : lstUnCondFrames )
    {
        ouValueList.clear();
        itr->GetName(strFrameName);

        strPublishers = "";
        lstTxEcus.clear();
        itr->GetEcus(eTx, lstTxEcus);
        nGetSubscribers(lstTxEcus, strPublishers);

        strSubscribers = "";
        lstRxEcus.clear();
        itr->GetEcus(eRx, lstRxEcus);
        nGetSubscribers(lstRxEcus, strSubscribers);

        itr->GetProperties(ouFrameProps);

        ouValueList.push_back(QVariant(QString::fromStdString(strFrameName)));
        ouValueList.push_back(QVariant(ouFrameProps.m_nMsgId));
        ouValueList.push_back(QVariant(GetString(ouFrameProps.m_unMsgSize, 10)));
        ouValueList.push_back(QVariant(QString::fromStdString(strPublishers)));
        ouValueList.push_back(QVariant(QString::fromStdString(strSubscribers)));

        InsertRow(nRow++, ouValueList);
    }
    return 0;
}

int LDFAdditionalView::DisplaySporadicProps( IFrame* pouFrame)
{
    VALIDATE_AND_RETURN(pouFrame);

    LinFrameProps ouFrameProps;
    ouFrameProps.m_eLinFrameType = eLinInvalidFrame;
    pouFrame->GetProperties(ouFrameProps);

    std::list<IFrame*> lstUnCondFrames;

for ( auto itr : ouFrameProps.m_ouLINSporadicFrameProps.m_pouUnconditionalFrame)
    {
        IFrame* pouFrame = nullptr;
        m_pouLdfCluster->GetElement(eFrameElement, UID_ELEMENT(itr.first), (IElement**)&pouFrame);
        if ( nullptr != pouFrame )
        {
            lstUnCondFrames.push_back(pouFrame);
        }
    }


    QList<QString> strColumnList;
    strColumnList <<"Frame" <<  "Length" << "Publisher" << "Subscribers";

    CreateColumns(strColumnList);
    setRowCount(lstUnCondFrames.size());

    int nRow = 0;
    QList<QVariant> ouValueList;
    std::list<IEcu*> lstTxEcus, lstRxEcus;

    std::string strFrameName, strPublishers, strSubscribers, strEncoding, strUnit, strCoding;
    unsigned int unId;
    CompuMethodProps ouCompuProps;
    ouCompuProps.m_eType = eInvalidProtocol;
    if ( nullptr != m_pHelpLabel )
    {
        std::string strName;
        pouFrame->GetName(strName);
        m_pHelpLabel->setText(tr("Frames associated to Sporadic Frame <b>%1</b>").arg(strName.c_str()));
    }

for ( auto itr : lstUnCondFrames )
    {
        ouValueList.clear();
        itr->GetName(strFrameName);

        strPublishers = "";
        lstTxEcus.clear();
        itr->GetEcus(eTx, lstTxEcus);
        nGetSubscribers(lstTxEcus, strPublishers);

        strSubscribers = "";
        lstRxEcus.clear();
        itr->GetEcus(eRx, lstRxEcus);
        nGetSubscribers(lstRxEcus, strSubscribers);

        itr->GetProperties(ouFrameProps);

        ouValueList.push_back(QVariant(QString::fromStdString(strFrameName)));
        ouValueList.push_back(QVariant(GetString(ouFrameProps.m_unMsgSize, 10)));
        ouValueList.push_back(QVariant(QString::fromStdString(strPublishers)));
        ouValueList.push_back(QVariant(QString::fromStdString(strSubscribers)));

        InsertRow(nRow++, ouValueList);
    }
    return 0;
}

int LDFAdditionalView::DisplayDiagProps( IFrame* pouFrame)
{
    VALIDATE_AND_RETURN(pouFrame);
    std::map<ISignal*, SignalInstanse> mapSignals;

    pouFrame->GetSignalList(mapSignals);

    int nRowCount = mapSignals.size() ;

    QList<QString> strColumnList;
    strColumnList <<"Signal" << "Startbit" << "Publisher" << "Subscribers" << "Length (Bit(s))" << "Init Value" << "Coding";

    CreateColumns(strColumnList);
    setRowCount(nRowCount);

    LINSignalProps ouSignalProps;
    ouSignalProps.eType = eInvalidProtocol;
    int nRow = 0;
    QList<QVariant> ouValueList;
    std::list<IEcu*> lstTxEcus, lstRxEcus;
    ICoding* pCoding = nullptr;

    std::string strSigName, strPublishers, strSubscribers, strEncoding, strUnit, strCoding;
    unsigned int unId;
    CompuMethodProps ouCompuProps;
    ouCompuProps.m_eType = eInvalidProtocol;
    if ( nullptr != m_pHelpLabel )
    {
        std::string strName;
        pouFrame->GetName(strName);
        m_pHelpLabel->setText(tr("Diagnostic signals associated to Dignostic Frame <b>%1</b>").arg(strName.c_str()));
    }

for ( auto itr : mapSignals )
    {
        ouValueList.clear();
        itr.first->GetName(strSigName);
        itr.first->GetProperties(ouSignalProps);

        strPublishers = "";
        lstTxEcus.clear();
        itr.first->GetEcus(eTx, lstTxEcus);
        nGetSubscribers(lstTxEcus, strPublishers);

        strSubscribers = "";
        lstRxEcus.clear();
        itr.first->GetEcus(eRx, lstRxEcus);
        nGetSubscribers(lstRxEcus, strSubscribers);

        itr.first->GetEncoding(&pCoding);

        ouValueList.push_back(QVariant(QString::fromStdString(strSigName)));
        ouValueList.push_back(QVariant(GetString(itr.second.m_nStartBit, 10)));
        ouValueList.push_back(QVariant(QString::fromStdString(strPublishers)));
        ouValueList.push_back(QVariant(QString::fromStdString(strSubscribers)));
        ouValueList.push_back(QVariant(GetString(ouSignalProps.m_unSignalSize, 10)));
        ouValueList.push_back(QVariant(ouSignalProps.m_nIntialValue));

        strEncoding = "", strUnit = "", strCoding = "";
        if(nullptr != pCoding)
        {
            pCoding->GetName(strCoding);
        }

        ouValueList.push_back(QVariant(QString::fromStdString(strCoding)));

        InsertRow(nRow++, ouValueList);
    }
    return 0;
}

ERRORCODE LDFAdditionalView::nGetEncodingnUnit(LINCompuMethods ouCompuProps, std::string& strEncoding, std::string& strUnit)
{
    strEncoding = "";
    strUnit = "";

    if(ouCompuProps.m_ouPhysicalValueList.size() > 0)
    {
        strEncoding = "physical";
        auto itr = ouCompuProps.m_ouPhysicalValueList.begin();
        PhysicalValue ouPhyVal = *itr;
        strUnit = ouPhyVal.m_strTextInfo;
    }
    else if(ouCompuProps.m_ouLogicalValueList.size() > 0)
    {
        strEncoding = "Logical";
    }
    else if(ouCompuProps.m_bASCII == true)
    {
        strEncoding = "Ascii";
    }
    else if(ouCompuProps.m_bBCD == true)
    {
        strEncoding = "BCD";
    }

    return EC_SUCCESS;
}