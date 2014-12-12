#include "UnconditionalFrameEditDlg.h"
#include "LDFDatabaseManager.h"
#include "IClusterProps.h"
#include "LDFUtility.h"
#include "Defines.h"
#include "MapSignalsDlg.h"
#include "LDFTagDefines.h"
#include <sstream>      // std::ostringstream
#include <fstream>

UnconditionalFrameEditDlg::UnconditionalFrameEditDlg(IFrame** pouFrame, eMode eUIType, QWidget* parent)
    : QDialog(parent)
{
    m_pouFrame = pouFrame;
    m_eUIMode = eUIType;




    m_pouLDFCluster = LDFDatabaseManager::GetDatabaseManager()->GetLDFCluster();
    ui.setupUi(this);
    layout()->setSizeConstraint( QLayout::SetFixedSize );
    SetUpUi();



    connect(ui.btnMapSignal, SIGNAL(clicked(bool)), this, SLOT(onMapSignal(bool)));
    connect(ui.buttonBox, SIGNAL(accepted()), this, SLOT(onSelectionOk()));
    connect(ui.buttonBox, SIGNAL(rejected()), this, SLOT(onSelectionCancel()));
    connect(ui.comboPublisher, SIGNAL(currentIndexChanged(QString)), this, SLOT(onChangePublisher(QString)));
    connect(ui.comboFrameLength, SIGNAL(currentIndexChanged(QString)), this, SLOT(onChangeFrameLength(QString)));

    QIcon ouWindowIcon;
    ouWindowIcon.addFile(":/LDFEditorIcons/Resources/icons/16x16/Frame.png", QSize(16, 16));
    ouWindowIcon.addFile(":/LDFEditorIcons/Resources/icons/22x22/Frame.png", QSize(16, 16));
    ouWindowIcon.addFile(":/LDFEditorIcons/Resources/icons/32x32/Frame.png", QSize(16, 16));

    setWindowIcon(ouWindowIcon);

}

UnconditionalFrameEditDlg::~UnconditionalFrameEditDlg()
{
}

void UnconditionalFrameEditDlg::SetUpUi()
{
    IFrame* pFrame = *m_pouFrame;
    m_bIsDynFrame = false;
    std::string strName, strECUName;
    QString strValue;
    FrameProps ouFrameProps;
    std::map<std::string, std::string> maSubscribers;
    std::list<IEcu*> lstTxECUs, lstRxECUs;
    ui.editFrameName->setValidator(new QRegExpValidator(QRegExp(defIdentifier_RegExp)));
    PopulateFrameIdCombo();
    PopulateFrameLengthCombo();
    vPopulatePublishers();
    vEnableDynamicFrame();

    if(m_eUIMode == eNew)
    {
        ui.comboFrameLength->setCurrentText("4");
        ui.comboFrameId->setCurrentText(defNONE);
        ui.comboPublisher->setCurrentText(defNONE);
        ui.tableSignals->setRowCount(0);
        setWindowTitle("Create Unconditional Frame");
    }
    else if(m_eUIMode == eEdit)
    {
        if (pFrame == nullptr)
        {
            return;
        }

        pFrame->GetName(strName);
        pFrame->GetProperties(ouFrameProps);

        QString strTitle = "Edit Unconditional Frame - ";
        strTitle+= + strName.c_str();
        setWindowTitle(strTitle);

        // Set FrameId
        strValue = GetString(ouFrameProps.m_ouLINUnConditionFrameProps.m_unId);
        ui.comboFrameId->setCurrentText(strValue);

        // Set Frame Length
        strValue = GetString(ouFrameProps.m_ouLINUnConditionFrameProps.m_nLength, 10);          //Always Dec
        ui.comboFrameLength->setCurrentText(strValue);
        m_omFrameLength = strValue.toStdString();

        pFrame->GetEcus(eTx, lstTxECUs);
        pFrame->GetEcus(eRx, lstRxECUs);

        // Set Frame Name
        ui.editFrameName->setText(QString::fromStdString(strName));

        // Adding ECUs to Publisherlst and Subscriberlst
        vAddSubscribers(lstRxECUs);

        // Set Publisher
        if(lstTxECUs.size() > 0)
        {
            std::list<IEcu*>::iterator itrTxECU = lstTxECUs.begin();
            ((IEcu*)*itrTxECU)->GetName(strECUName);
            ui.comboPublisher->setCurrentText(QString::fromStdString(strECUName));
            m_omStrPublisher = strECUName;
        }

        PopulateFrameSignals();

        // Dynamic Frame
        std::list<unsigned int> ouDyanamicFramelist;
        void* pDynFrameList;
        m_pouLDFCluster->GetProperties(eLdfDyanmicFrameList, &ouDyanamicFramelist);

        auto itr = std::find(ouDyanamicFramelist.begin(), ouDyanamicFramelist.end(), ouFrameProps.m_ouLINUnConditionFrameProps.m_unId);
        if(ouDyanamicFramelist.end() != itr)
        {
            ui.chkDynFrame->setChecked(true);
            m_bIsDynFrame = true;
        }

        //Store Rx Ecus;

        pFrame->GetEcus(eRx, m_ouRxEcus);


    }
    m_unFrameId = GetUnsignedInt(ui.comboFrameId->currentText());
    vExistingFrameDetails(GetUnsignedInt(ui.comboFrameId->currentText()));
}

void UnconditionalFrameEditDlg::vAddSubscribers(std::list<IEcu*> lstRxECUs)
{
    ui.listSubscribers->clear();
    std::map<std::string, std::string> maSubscribers;
    std::string strECUName;
    // Adding ECUs to Publisherlst and Subscriberlst
    maSubscribers.clear();
for(auto itrECU : lstRxECUs)
    {
        IEcu* pEcu =  (IEcu*)(itrECU);
        strECUName = "";
        // Adding Publishers
        pEcu->GetName(strECUName);
        if(maSubscribers.find(strECUName) == maSubscribers.end())
        {
            // Adding Subscribers
            QListWidgetItem* item = new QListWidgetItem(QString::fromStdString(strECUName), ui.listSubscribers);
            item->setFlags(item->flags() | Qt::ItemIsUserCheckable);
            ui.listSubscribers->addItem(item);
            maSubscribers[strECUName] = strECUName;
        }
    }
}

void UnconditionalFrameEditDlg::vExistingFrameDetails(unsigned int unFrameId)
{
    m_mapFrameNames.clear();
    m_mapFrameIds.clear();
    std::map<UID_ELEMENT, IElement*> mapFrames;
    std::string omstrFrameName;
    m_pouLDFCluster->GetElementList(eFrameElement, mapFrames);
    FrameProps ouFrameProps;

for(auto itrFrame : mapFrames)
    {
        ((IFrame*)itrFrame.second)->GetProperties(ouFrameProps);

        if(ouFrameProps.m_eFrameType == eLIN_Unconditional)
        {
            itrFrame.second->GetName(omstrFrameName);
            m_mapFrameNames[omstrFrameName] = omstrFrameName;

            if(unFrameId != ouFrameProps.m_ouLINUnConditionFrameProps.m_unId)
            {
                m_mapFrameIds[ouFrameProps.m_ouLINUnConditionFrameProps.m_unId] = ouFrameProps.m_ouLINUnConditionFrameProps.m_unId;
            }
        }
    }
}

void UnconditionalFrameEditDlg::PopulateFrameIdCombo()
{
    ui.comboFrameId->clear();
    QString strValue = defNONE;
    ui.comboFrameId->addItem(strValue);
    for(unsigned int unIndex = 0; unIndex <= MAX_UNCOND_FRAME_ID; unIndex++)
    {
        strValue = GetString(unIndex);
        ui.comboFrameId->addItem(strValue);
    }
}

void UnconditionalFrameEditDlg::PopulateFrameLengthCombo()
{
    ui.comboFrameLength->clear();
    QString strValue;
    for(unsigned int unIndex = 1; unIndex <= MAX_LIN_FRAME_LENGTH; unIndex++)
    {
        strValue = QString::number(unIndex);
        ui.comboFrameLength->addItem(strValue);
    }
}

void UnconditionalFrameEditDlg::PopulateFrameSignals()
{
    std::map<ISignal*, SignalInstanse> mapSignals;

    (*m_pouFrame)->GetSignalList(mapSignals);

for(auto itrMap : mapSignals)
    {
        vAddSignalToTree(itrMap.first, itrMap.second);
    }
    UID_ELEMENT uidECU = ui.comboPublisher->currentData().value<UID_ELEMENT>();
}

void UnconditionalFrameEditDlg::vPopulatePublishers()
{
    std::string strECUName;
    std::map<UID_ELEMENT, IElement*> mapECUs;
    m_pouLDFCluster->GetElementList(eEcuElement, mapECUs);
    ui.comboPublisher->addItem(defNONE);
    QVariant qVar;
    // Populating Publishers
for(auto itrECU : mapECUs)
    {
        IEcu* pEcu =  (IEcu*)(itrECU.second);
        strECUName = "";
        // Adding Publishers
        pEcu->GetName(strECUName);
        qVar.setValue(pEcu->GetUniqueId());
        ui.comboPublisher->addItem(QString::fromStdString(strECUName), qVar);
    }
}

void UnconditionalFrameEditDlg::vEnableDynamicFrame()
{
    LIN_Settings ouLINSettings;
    m_pouLDFCluster->GetProperties(eLINClusterProperties, &ouLINSettings);

    if(ouLINSettings.m_dProtocolVers == defVersion_2_0)
    {
        ui.chkDynFrame->setEnabled(true);
    }
    else
    {
        ui.chkDynFrame->setEnabled(false);
    }
}

void UnconditionalFrameEditDlg::vAddSignalToTree(ISignal* pSignal, SignalInstanse ouSignalInstance)
{
    SignalProps ouSignalProps;
    std::string strSubscribers = "", strName = "";
    std::list<IEcu*> lstECUs;
    ICoding* pCoding = nullptr;
    QList<QVariant> ouColumns;
    pSignal->GetName(strName);
    pSignal->GetProperties(ouSignalProps);
    pSignal->GetEcus(eRx, lstECUs);
    int nRowCount = ui.tableSignals->rowCount();
    ui.tableSignals->setRowCount(ui.tableSignals->rowCount()+1);

    ouColumns.push_back(QVariant(strName.c_str()));
    ouColumns.push_back(QVariant(GetString(ouSignalInstance.m_nStartBit, 10)));

    pSignal->GetEncoding(&pCoding);
    strName = "";
    if(nullptr != pCoding)
    {
        pCoding->GetName(strName);
    }
    strSubscribers = "";
    nGetSubscribers(lstECUs, strSubscribers);
    ouColumns.push_back(QVariant(strSubscribers.c_str()));
    ouColumns.push_back(GetString(ouSignalProps.m_ouLINSignalProps.m_nLength, 10));         //Alwas dec
    ouColumns.push_back(QVariant(ouSignalProps.m_ouLINSignalProps.m_nIntialValue));
    ouColumns.push_back(QVariant(strName.c_str()));
    QVariant ouVariant;
    ouVariant.setValue(pSignal->GetUniqueId());
    ui.tableSignals->InsertRow(nRowCount, ouColumns);
    ui.tableSignals->item(nRowCount, 0)->setData(Qt::UserRole, ouVariant);
}

void UnconditionalFrameEditDlg::onMapSignal(bool)
{
    UID_ELEMENT uidECU = ui.comboPublisher->itemData(ui.comboPublisher->currentIndex()).value<UID_ELEMENT>();
    std::map<UID_ELEMENT, SignalInstanse> mapSignalInstance;

    SignalInstanse ouSignalInstanse;
    for( unsigned int unIndex = 0; unIndex < ui.tableSignals->rowCount(); unIndex++)
    {

        ouSignalInstanse.m_nStartBit = GetUnsignedInt(ui.tableSignals->item(unIndex, 1)->text(), 10);
        mapSignalInstance[ui.tableSignals->item(unIndex, 0)->data(Qt::UserRole).value<UID_ELEMENT>()] = ouSignalInstanse;
    }

    if(uidECU != INVALID_UID_ELEMENT)
    {
        unsigned int nId = GetUnsignedInt(ui.comboFrameLength->currentText());
        MapSignalsDlg ouMapSignalsDlg(uidECU, nId, mapSignalInstance);
        if(ouMapSignalsDlg.exec() == QDialog::Accepted)
        {
            ouMapSignalsDlg.vGetSelectedSignals(mapSignalInstance);
            vUpdateFrameSignalList(mapSignalInstance);
        }
    }
}

void UnconditionalFrameEditDlg::vUpdateFrameSignalList(std::map<UID_ELEMENT, SignalInstanse>& mapSelectedSignals)
{
    UID_ELEMENT uidSignal = INVALID_UID_ELEMENT;
    IElement* pSignal = nullptr;
    std::map<UID_ELEMENT, int> mapTempSignals;
    ui.tableSignals->setRowCount(0);
    std::list<IEcu*> lstRxECUs;
    std::string strSubscribers = "";
for(auto itrSignal : mapSelectedSignals)
    {
        uidSignal = itrSignal.first;
        m_pouLDFCluster->GetElement(eSignalElement, uidSignal, &pSignal);

        if(nullptr != pSignal)
        {
            vAddSignalToTree((ISignal*)pSignal, itrSignal.second);
            mapTempSignals[((ISignal*)pSignal)->GetUniqueId()] = itrSignal.second.m_nStartBit;
            ((ISignal*)pSignal)->GetEcus(eRx, lstRxECUs);
        }
    }

    vAddSubscribers(lstRxECUs);
}

void UnconditionalFrameEditDlg::onSelectionCancel()
{
    reject();
}

void UnconditionalFrameEditDlg::vUpdateNewFrameDetails()
{
    FrameProps ouFrameProps;
    UID_ELEMENT uidFrame;
    IFrame* pFrame = nullptr;
    m_pouLDFCluster->CreateElement(eFrameElement, (IElement**)&pFrame);


    //2. Set Frame Props
    unsigned int nLength = GetUnsignedInt(ui.comboFrameLength->currentText(), 10);      //Always Dec
    ouFrameProps.m_ouLINUnConditionFrameProps.m_nLength = nLength;
    ouFrameProps.m_ouLINUnConditionFrameProps.m_unId = GetUnsignedInt(ui.comboFrameId->currentText());
    ouFrameProps.m_eFrameType = eLIN_Unconditional;

    pFrame->SetProperties(ouFrameProps);

    //3. Map Tx ECU and Map Tx Frame to Ecu
    IEcu* pouEcu = nullptr;
    UID_ELEMENT unEcuID = INVALID_UID_ELEMENT;
    m_pouLDFCluster->GetEcu(ui.comboPublisher->currentText().toStdString(), &pouEcu);
    if ( pouEcu != nullptr )
    {
        pouEcu->GetUniqueId(unEcuID);
        pFrame->MapNode(eTx, unEcuID);
        uidFrame = pFrame->GetUniqueId();
        pouEcu->MapFrame(eTx, uidFrame);
    }

    //4. Map Signals and Map RX frames TO ecu
    SignalInstanse ouSignalInstance;
    ISignal* pSignal = nullptr;
    std::list<IEcu*> ecuList;
    UID_ELEMENT uidSignal;
    for( unsigned int unIndex = 0; unIndex < ui.tableSignals->rowCount(); unIndex++)
    {
        //QTreeWidgetItem *treeItem = ui.treeSignals->topLevelItem(unIndex);
        uidSignal = ui.tableSignals->item(unIndex, 0)->data(Qt::UserRole).value<UID_ELEMENT>();
        m_pouLDFCluster->GetElement(eSignalElement, uidSignal,  (IElement**)&pSignal);

        ecuList.clear();
        if(nullptr != pSignal)
        {
            pSignal->GetEcus(eTx, ecuList);
        }

        //Ldf contains one Ecu so take First Ecu only;
        auto itrTxEcu =  ecuList.begin();
        if ( itrTxEcu != ecuList.end() )
        {
            UID_ELEMENT unTempEcuId = INVALID_UID_ELEMENT;
            (*itrTxEcu)->GetUniqueId(unTempEcuId);
            if ( unTempEcuId == unEcuID )
            {
                unsigned int unStartBit = GetUnsignedInt(ui.tableSignals->item(unIndex, 1)->text(), 10);
                ouSignalInstance.m_nStartBit =unStartBit;
                //ouSignalInstance.m_nStartBit = treeItem->text(1).toInt();
                pFrame->MapSignal(uidSignal, ouSignalInstance);
                ecuList.clear();
                if(nullptr != pSignal)
                {
                    pSignal->GetEcus(eRx, ecuList);
                }
for ( auto itrEcu : ecuList )
                {
                    itrEcu->MapFrame(eRx, uidFrame);
                }
            }
        }
    }
    //1. Set Frame Name
    pFrame->SetName(ui.editFrameName->text().toStdString());

    *m_pouFrame = pFrame;
}

void UnconditionalFrameEditDlg::vUpdateEditFrameDetails()
{
    FrameProps ouFrameProps;
    UID_ELEMENT uidFrame;
    IFrame* pFrame = *m_pouFrame;
    uidFrame = pFrame->GetUniqueId();
    pFrame->GetProperties(ouFrameProps);

    //2. Set Frame Props

    ouFrameProps.m_ouLINUnConditionFrameProps.m_nLength = GetUnsignedInt(ui.comboFrameLength->currentText(), 10);
    ouFrameProps.m_ouLINUnConditionFrameProps.m_unId = GetUnsignedInt(ui.comboFrameId->currentText());
    ouFrameProps.m_eFrameType = eLIN_Unconditional;

    pFrame->SetProperties(ouFrameProps);

    std::list<IEcu*> lstECUs;
    pFrame->GetEcus(eTx, lstECUs);

    UID_ELEMENT uidECU;
for(auto itrECU : lstECUs)
    {
        uidECU = itrECU->GetUniqueId();
        itrECU->UnMapFrame(eTx, uidFrame);
        pFrame->UnMapNode(eTx, uidECU);
    }
    lstECUs.clear();

for(auto itrECU : m_ouRxEcus)
    {
        uidECU = itrECU->GetUniqueId();
        itrECU->UnMapFrame(eRx, uidFrame);
        pFrame->UnMapNode(eRx, uidECU);
    }

    //3. Map Tx ECU and Map Tx Frame to Ecu
    IEcu* pouEcu = nullptr;
    UID_ELEMENT unEcuID = INVALID_UID_ELEMENT;
    std::string strPublisher = ui.comboPublisher->currentText().toStdString();
    m_pouLDFCluster->GetEcu(strPublisher, &pouEcu);
    if ( pouEcu != nullptr )
    {
        pouEcu->GetUniqueId(unEcuID);
        pFrame->MapNode(eTx, unEcuID);
        pouEcu->MapFrame(eTx, uidFrame);
    }

    //4. Map Signals and Map RX frames TO ecu




    SignalInstanse ouSignalInstance;
    ISignal* pSignal = nullptr;
    std::list<IEcu*> ecuList;
    UID_ELEMENT uidSignal;
    for( unsigned int unIndex = 0; unIndex < ui.tableSignals->rowCount(); unIndex++)
    {
        //QTreeWidgetItem *treeItem = ui.treeSignals->topLevelItem(unIndex);
        uidSignal = ui.tableSignals->item(unIndex, 0)->data(Qt::UserRole).value<UID_ELEMENT>();
        m_pouLDFCluster->GetElement(eSignalElement, uidSignal,  (IElement**)&pSignal);

        ecuList.clear();

        if(nullptr != pSignal)
        {
            pSignal->GetEcus(eTx, ecuList);
        }

        //Ldf contains one Ecu so take First Ecu only;
        auto itrTxEcu =  ecuList.begin();

        if ( itrTxEcu != ecuList.end() )
        {
            UID_ELEMENT unTempEcuId = INVALID_UID_ELEMENT;
            (*itrTxEcu)->GetUniqueId(unTempEcuId);
            if ( unTempEcuId == unEcuID )
            {
                ouSignalInstance.m_nStartBit = GetUnsignedInt(ui.tableSignals->item(unIndex, 1)->text(), 10);
                pFrame->MapSignal(uidSignal, ouSignalInstance);
                ecuList.clear();
                if(nullptr != pSignal)
                {
                    pSignal->GetEcus(eRx, ecuList);
                }
for ( auto itrEcu : ecuList )
                {
                    itrEcu->MapFrame(eRx, uidFrame);
                }
            }
        }
    }

    //1. Set Frame Name
    pFrame->SetName(ui.editFrameName->text().toStdString());

}

void UnconditionalFrameEditDlg::onSelectionOk()
{
    if(EC_SUCCESS == nValidateValues())
    {
        FrameProps ouFrameProps;
        UID_ELEMENT uidFrame;
        if(m_eUIMode == eNew)
        {
            vUpdateNewFrameDetails();
        }
        else if(m_eUIMode == eEdit)
        {
            vUpdateEditFrameDetails();
        }

        vUpdateDynamicFrameDetails();
        LDFDatabaseManager::GetDatabaseManager()->setDocumentModified(true);
        accept();
    }
}

void UnconditionalFrameEditDlg::vUpdateDynamicFrameDetails()
{
    if(ui.chkDynFrame->isEnabled() == true)
    {
        std::list<unsigned int> ouDyanamicFramelist;
        unsigned int unFrameId;
        (*m_pouFrame)->GetFrameId(unFrameId);
        m_pouLDFCluster->GetProperties(eLdfDyanmicFrameList, &ouDyanamicFramelist);
        if(m_eUIMode == eEdit)
        {
            auto itr = std::find(ouDyanamicFramelist.begin(), ouDyanamicFramelist.end(), m_unFrameId);
            if(ouDyanamicFramelist.end() != itr)
            {
                // If dynamic frame unchecked remove from the list
                if(ui.chkDynFrame->isChecked() == false && m_bIsDynFrame == true)
                {
                    ouDyanamicFramelist.erase(itr);
                    m_pouLDFCluster->SetProperties(eLdfDyanmicFrameList, &ouDyanamicFramelist);
                }
            }
        }

        if(ui.chkDynFrame->isChecked() == true && m_bIsDynFrame == false)
        {
            ouDyanamicFramelist.push_back(unFrameId);
            m_pouLDFCluster->SetProperties(eLdfDyanmicFrameList, &ouDyanamicFramelist);
        }
    }
}

ERRORCODE UnconditionalFrameEditDlg::nValidateValues()
{
    std::string omStrName = ui.editFrameName->text().toStdString();
    QMessageBox msgBox;
    std::string omstrCurrFrame = "";
    FrameProps ouFrameProps;
    unsigned int unExistingFrameID;
    if(nullptr != (*m_pouFrame))
    {
        (*m_pouFrame)->GetName(omstrCurrFrame);
        (*m_pouFrame)->GetProperties(ouFrameProps);
        unExistingFrameID =  ouFrameProps.m_ouLINUnConditionFrameProps.m_unId;
        vExistingFrameDetails(unExistingFrameID);
    }

    // Validate Frame Name
    if ( omStrName == "" )
    {
        msgBox.setText("Invalid Frame Name (Empty Name).");
        msgBox.exec();
        return EC_FAILURE;
    }
    if( ui.comboFrameId->currentText() == defNONE)
    {
        msgBox.setText("Frame Id cannot be empty.");
        msgBox.exec();
        return EC_FAILURE;
    }
    if( m_mapFrameNames.find(omStrName) != m_mapFrameNames.end() && omStrName != omstrCurrFrame)
    {
        msgBox.setText("Frame Name Already Exists.");
        msgBox.exec();
        return EC_FAILURE;
    }

    // Validate Frame Id
    if(ui.chkDynFrame->isChecked() == false)
    {
        QString strFrameId = ui.comboFrameId->currentText();
        unsigned int nFrameId = GetUnsignedInt(strFrameId);

        int nCount = bIsDynamicFrame(nFrameId);
        if((m_mapFrameIds.find(nFrameId) != m_mapFrameIds.end()) || nCount > 1)
        {
            msgBox.setText("Frame Id Already Exists.");
            msgBox.exec();
            return EC_FAILURE;
        }
    }

    if(ui.comboPublisher->currentText().toStdString() == defNONE)
    {
        msgBox.setText("Invalid Publisher (Empty).");
        msgBox.exec();
        return EC_FAILURE;
    }
    return 0;
}

int UnconditionalFrameEditDlg::bIsDynamicFrame(unsigned int unFrameId)
{
    std::list<unsigned int> ouDyanamicFramelist;
    m_pouLDFCluster->GetProperties(eLdfDyanmicFrameList, &ouDyanamicFramelist);
    int nCount = 0;
for(auto itr : ouDyanamicFramelist)
    {
        if(itr == unFrameId)
        {
            nCount++;
        }
    }

    return nCount;
}

void UnconditionalFrameEditDlg::onChangePublisher(QString omstrCurrentText)
{
    if(omstrCurrentText.toStdString() == m_omStrPublisher)
    {
        return;
    }

    QMessageBox msgBox;
    int nResult = QMessageBox::NoButton;
    if(ui.tableSignals->rowCount() > 0)
    {
        msgBox.setText("Signal(s) mapped will be cleared on changing Publisher.");
        msgBox.setInformativeText("Do you want to Proceed?");
        msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
        nResult = msgBox.exec();
    }

    if(nResult == QMessageBox::Ok)
    {
        ui.tableSignals->setRowCount(0);
        ui.listSubscribers->clear();
    }
    else if(nResult == QMessageBox::Cancel)
    {
        ui.comboPublisher->setCurrentText(QString::fromStdString(m_omStrPublisher));
    }

    m_omStrPublisher = ui.comboPublisher->currentText().toStdString();
}

void UnconditionalFrameEditDlg::onChangeFrameLength(QString omCurrFrameLen)
{
    if(m_omFrameLength == omCurrFrameLen.toStdString())
    {
        return;
    }

    int nCurrLen = GetUnsignedInt(omCurrFrameLen);
    int nSignalLength = 0, nStartBit = 0, nEndBit = 0;
    //QTreeWidgetItem *pItem = nullptr;
    int nCount = ui.tableSignals->rowCount();
    std::list<std::string> lstSignals;
    std::list<int> lstSignalIndex;
    for(int nIndex = 0; nIndex < nCount; nIndex++)
    {
        nStartBit = GetUnsignedInt(ui.tableSignals->item(nIndex, 1)->text(), 10);               //Always dec
        nSignalLength = GetUnsignedInt(ui.tableSignals->item(nIndex, 3)->text());
        nEndBit = nStartBit + nSignalLength;

        if(nEndBit > (nCurrLen*8))
        {
            lstSignals.push_back(ui.tableSignals->item(nIndex, 0)->text().toStdString());
            lstSignalIndex.push_back(nIndex);
        }
    }

    std::string strSignals = "";
    std::list<std::string>::iterator itr = lstSignals.begin();
    while(itr != lstSignals.end())
    {
        if(itr == lstSignals.begin())
        {
            strSignals = *itr;
        }
        else
        {
            strSignals += defCOMMA + *itr;
        }

        itr++;
    }

    if(strSignals.empty() == false)
    {
        QMessageBox msgBox;
        std::ostringstream omStringStrm;

        omStringStrm << "Signal(s) [" << strSignals.c_str() << "] exceeding frame length will be unmapped.";
        msgBox.setText(QString::fromStdString(omStringStrm.str()));
        msgBox.setInformativeText("Do you want to Proceed?");
        msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
        int nResult = msgBox.exec();

        if(nResult == QMessageBox::Ok)
        {
            int nSigCount = ui.tableSignals->rowCount();
            int nSigIndex = 0;
            while(nSigIndex < nSigCount)
            {
                if(lstSignals.end() != std::find(lstSignals.begin(), lstSignals.end(), ui.tableSignals->item(nSigIndex, 0)->text().toStdString()))
                {
                    ui.tableSignals->removeRow(nSigIndex);
                    nSigIndex = 0;
                    nSigCount = ui.tableSignals->rowCount();
                }
                else
                {
                    nSigIndex++;
                }
            }
        }
        else
        {
            ui.comboFrameLength->setCurrentText(QString::fromStdString(m_omFrameLength));
        }
    }
    m_omFrameLength = ui.comboFrameLength->currentText().toStdString();

    if(ui.tableSignals->rowCount() <= 0)
    {
        ui.listSubscribers->clear();
    }
}