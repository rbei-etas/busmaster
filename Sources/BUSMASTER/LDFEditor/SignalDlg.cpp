
#include "SignalDlg.h"
#include "qvalidator.h"
#include "LDFUtility.h"
#include "qmessagebox.h"
SignalDlg::SignalDlg(ICluster* pouCluster, ISignal** pSignal, bool bNew, QWidget* parent)
    : QDialog(parent)
{
    m_pLdfCluster = pouCluster;
    m_bEditMode = !bNew;
    m_pSignal = pSignal;
    m_unExistingCoidngUid = INVALID_UID_ELEMENT;
    ui.setupUi(this);

    layout()->setSizeConstraint( QLayout::SetFixedSize );

    vPrepareValidations();
    vPrepareUiForNewMode();
    if ( true == m_bEditMode )
    {
        vPrepareUiForEditMode();
    }
    ui.labelLength->setHidden(true);

    ui.labelPublisher->setHidden(true);
    connect(ui.buttonOption, SIGNAL(accepted()), this, SLOT(onSelectionOk()));
    connect(ui.buttonOption, SIGNAL(rejected()), this, SLOT(onSelecetionCancel()));

    QIcon ouWindowIcon;
    ouWindowIcon.addFile(":/LDFEditorIcons/Resources/icons/16x16/Signals.png", QSize(16, 16));
    ouWindowIcon.addFile(":/LDFEditorIcons/Resources/icons/22x22/Signals.png", QSize(16, 16));
    ouWindowIcon.addFile(":/LDFEditorIcons/Resources/icons/32x32/Signals.png", QSize(16, 16));

    setWindowIcon(ouWindowIcon);

}

SignalDlg::~SignalDlg()
{

}

void SignalDlg::vFillElementList()
{
    eClusterElementType ouClusterElement[defTotalElement] = {eEcuElement, eSignalElement, eCodingElement};
    std::string strName;
    std::map<UID_ELEMENT, IElement*> ouElementMap;
    for ( int i = 0 ; i < defTotalElement; i++ )
    {
        eClusterElementType eElementType = ouClusterElement[i];
        m_pLdfCluster->GetElementList(eElementType, ouElementMap);

for ( auto itr : ouElementMap )
        {
            itr.second->GetName(strName);
            (m_ouElementMap[i])[strName] = itr.second->GetUniqueId();
        }
        ouElementMap.clear();
    }
    if ( true == m_bEditMode )
    {
        (*m_pSignal)->GetName(strName);
        auto itrSignal = m_ouElementMap[defSignalIndex].find(strName);
        if ( itrSignal !=  m_ouElementMap[defSignalIndex].end() )
        {
            m_ouElementMap[defSignalIndex].erase(itrSignal);
        }
    }

}


void SignalDlg::vPrepareValidations()
{
    ui.editInitialValue->setValidator(new QRegExpValidator(QRegExp(defHex_RegExp)));
    ui.labelInitailValueHex->setVisible(true);
    if ( LDFDatabaseManager::GetDatabaseManager()->bIsDisplayHexOn() == false )
    {
        ui.labelInitailValueHex->setVisible(false);
        ui.editInitialValue->setValidator(new QIntValidator());

    }
    QIntValidator* pSignalLengthValidator = new QIntValidator();
    pSignalLengthValidator->setRange(0, 63);
    ui.editLength->setValidator(pSignalLengthValidator);
    ui.editName->setValidator(new QRegExpValidator(QRegExp(defIdentifier_RegExp)));
    vFillElementList();
}
void SignalDlg::vPrepareUiForEditMode()
{

    std::string strName;
    SignalProps ouSignalProps;
    ouSignalProps.eType = eInvalidProtocol;
    std::list<IEcu*> ouEcuTxList;
    std::list<IEcu*> ouEcuRxList;
    ICoding* pCoding = nullptr;

    auto pSignal = *m_pSignal;
    pSignal->GetName(strName);

    QString strTitle = "Edit Signal - ";
    strTitle+= + strName.c_str();
    setWindowTitle(strTitle);


    pSignal->GetProperties(ouSignalProps);
    pSignal->GetEcus(eTx, ouEcuTxList);
    pSignal->GetEcus(eRx, ouEcuRxList);
    pSignal->GetEncoding(&pCoding);
    m_nActualLength = ouSignalProps.m_ouLINSignalProps.m_nLength;
    ui.editName->setText(QString::fromStdString(strName));
    ui.editLength->setText(tr("%1").arg(ouSignalProps.m_ouLINSignalProps.m_nLength));
    ui.editInitialValue->setText(GetString(ouSignalProps.m_ouLINSignalProps.m_nIntialValue, false));

    if ( ouSignalProps.m_ouLINSignalProps.m_ouValueType == eScalar )
    {
        ui.comboSignalType->setCurrentIndex(0);
    }
    else
    {
        ui.comboSignalType->setCurrentIndex(1);
    }
    m_unExistingCoidngUid = INVALID_UID_ELEMENT;
    if ( nullptr != pCoding )
    {
        pCoding->GetName(strName);
        int i = ui.comboCoding->findText(QString::fromStdString(strName));
        if ( i >= 0 )
        {
            ui.comboCoding->setCurrentIndex(i);
        }
        m_unExistingCoidngUid = pCoding->GetUniqueId();
    }

    auto itr = ouEcuTxList.begin();
    if ( ouEcuTxList.end() != itr )
    {
        IEcu* pouIecu = (IEcu*)*itr;
        pouIecu->GetName(strName);
        int i = ui.comboPublisher->findText(QString::fromStdString(strName));
        if ( i >= 0 )
        {
            ui.comboPublisher->setCurrentIndex(i);
            m_strCurrentPublisher = QString::fromStdString(strName);
        }
    }

    std::map<std::string, std::string> strEcuNameMap;
for ( auto itr2 : ouEcuRxList )
    {
        itr2->GetName(strName);
        strEcuNameMap[strName] = strName;
    }

    QListWidgetItem* pListItem;
    ui.listSubscribers->clear();
for ( auto itr : m_ouElementMap[defEcuIndex] )
    {
        pListItem = new QListWidgetItem(QString::fromStdString(itr.first));
        if ( strEcuNameMap.find(itr.first) != strEcuNameMap.end())
        {
            pListItem->setCheckState(Qt::Checked);
        }
        else
        {
            pListItem->setCheckState(Qt::Unchecked);
        }
        pListItem->setData(Qt::UserRole, QVariant(itr.second));
        ui.listSubscribers->addItem(pListItem);
    }
}

void SignalDlg::vPrepareUiForNewMode()
{
    m_nActualLength = -1;
    m_strCurrentPublisher = "";


    setWindowTitle("Create Signal");

    //Ecu Fill
    QListWidgetItem* pListItem;
    ui.comboPublisher->addItem(QString(""));
for ( auto itr : m_ouElementMap[defEcuIndex] )
    {
        ui.comboPublisher->addItem(QString::fromStdString(itr.first), QVariant(itr.second));

        pListItem = new QListWidgetItem(QString::fromStdString(itr.first));
        pListItem->setCheckState(Qt::Unchecked);
        pListItem->setData(Qt::UserRole, QVariant(itr.second));
        ui.listSubscribers->addItem(pListItem);
    }
    ui.comboPublisher-> setCurrentIndex(0);

    ui.comboCoding->addItem(QString(""), QVariant(INVALID_UID_ELEMENT));
for ( auto itr: m_ouElementMap[defCodingIndex] )
    {
        ui.comboCoding->addItem(QString::fromStdString(itr.first), QVariant(itr.second));
    }
    ui.comboCoding-> setCurrentIndex(0);
}



void SignalDlg::onSelectionOk()
{
    QMessageBox msgBox;
    QString strErrors = "";
    QString strTemp;
    int nErrors = 0;
    if ( ui.editName->text() == "" )
    {
        strErrors += strTemp.sprintf("<br>%d. Signal Name Should Not Be Empty<\br>", ++nErrors);
        ui.editName->setFocus();

    }

    auto itr = m_ouElementMap[defSignalIndex].find(ui.editName->text().toStdString());
    if ( itr != m_ouElementMap[defSignalIndex].end())
    {
        strErrors += strTemp.sprintf("<br>%d. Duplicate Signal Name<\br>", ++nErrors);
        ui.editName->setFocus();

    }

    unsigned int nLength = GetUnsignedInt(ui.editLength->text(), 10);
    if ( nLength == 0 )
    {
        strErrors += strTemp.sprintf("<br>%d. Signal Length Should be greater than 0<\br>", ++nErrors);

        ui.editLength->setFocus();

    }
    if ( nLength >= 64 )
    {
        strErrors += strTemp.sprintf("<br>%d. Signal Length Should be Less than 64<\br>", ++nErrors);

        ui.editLength->setFocus();
    }

    if ( nLength > 0 )
    {
        unsigned int nInitilVal = GetUnsignedInt(ui.editInitialValue->text());
        unsigned __int64 nMaxVal = pow(2, nLength );
        if ( nInitilVal >= nMaxVal )
        {
            strErrors += strTemp.sprintf("<br>%d. Initial Value Exceeds the Signal Value Range<\br>", ++nErrors);
            ui.editInitialValue->setFocus();
        }
    }


    if ( ui.comboPublisher->currentText() == "" )
    {
        strErrors += strTemp.sprintf("<br>%d. Invalid publisher.<\br>", ++nErrors);
        ui.comboPublisher->setFocus();

    }
    if ( strErrors.length() > 0 )
    {
        QMessageBox::critical(this, "Following Errors Occured:", strErrors, QMessageBox::Ok);
        return;
    }

    if (m_bEditMode == true && nLength > m_nActualLength )
    {
        QMessageBox::StandardButton reply = QMessageBox::warning(this, "Warning","Increasing signal length may overlap in some associted messages.<br>Do you want to continue</br>",
                                            QMessageBox::Yes|QMessageBox::No);
        if ( reply == QMessageBox::No )
        {
            return;
        }
    }


    if ( m_strCurrentPublisher != "" && ui.comboPublisher->currentText() !=  m_strCurrentPublisher )
    {
        QString strTemp;
        strTemp.sprintf("Changing the Publisher will unmap the signal from the <br>Frames Published by the Ecu <b>%s</b></br>.<br>Do You want to Continue?</br>",
                        m_strCurrentPublisher.toStdString().c_str());

        QMessageBox::StandardButton reply = QMessageBox:: information(this, "Change in Publisher", strTemp, QMessageBox::Yes|QMessageBox::No);
        if (reply == QMessageBox::No)
        {
            return;
        }
    }
    vFillSignalDetails();

    LDFDatabaseManager::GetDatabaseManager()->setDocumentModified(true);
    accept();

}
void SignalDlg::onSelecetionCancel()
{
    reject();
}


void SignalDlg::vFillSignalDetails()
{
    ISignal* pSignal;
    if ( false == m_bEditMode )
    {
        //Create New Signal
        m_pLdfCluster->CreateElement(eSignalElement, (IElement**)&pSignal);
        *m_pSignal = pSignal;
    }
    else
    {
        pSignal = *m_pSignal;
    }

    SignalProps ouSignalProps;
    ouSignalProps.eType = eLINProtocol;

    //Length
    unsigned int nLength = GetUnsignedInt(ui.editLength->text(), 10);
    ouSignalProps.m_ouLINSignalProps.m_nLength = nLength;

    //Initial Value
    unsigned int nInitialValue =GetUnsignedInt(ui.editInitialValue->text());
    ouSignalProps.m_ouLINSignalProps.m_nIntialValue = nInitialValue;

    //Signal Type
    ouSignalProps.m_ouLINSignalProps.m_ouSignalType = eSignalNormal;

    //Endianess always Intel in LDF
    ouSignalProps.m_ouLINSignalProps.m_ouEndianess = eIntel;

    //Data type always Unsigned in LDF
    ouSignalProps.m_ouLINSignalProps.m_ouDataType = eUnsigned;

    //Byte array is valid if user selects and signal length is more than 8 bits
    ouSignalProps.m_ouLINSignalProps.m_ouValueType = eScalar;
    if ( nLength > 8 && ui.comboSignalType->currentIndex() != 0)
    {
        ouSignalProps.m_ouLINSignalProps.m_ouValueType = eByteArray;
    }

    pSignal->SetName(ui.editName->text().toStdString());

    pSignal->SetProperties(ouSignalProps);


    //Coding
    if ( ui.comboCoding->currentIndex() >= 0 )
    {
        ICoding* pTempCoding;

        UID_ELEMENT uidCoding = ui.comboCoding->currentData(Qt::UserRole).value<UID_ELEMENT>();
        pSignal->SetEncoding(uidCoding);

        if(m_unExistingCoidngUid != uidCoding)
        {
            //Unmap to existing coding
            m_pLdfCluster->GetElement(eCodingElement, m_unExistingCoidngUid, (IElement**)&pTempCoding);
            if ( nullptr != pTempCoding )
            {
                pTempCoding->UnMapSignal(pSignal->GetUniqueId());
            }

            //Map to new coding
            m_pLdfCluster->GetElement(eCodingElement, uidCoding, (IElement**)&pTempCoding);
            if ( nullptr != pTempCoding )
            {
                pTempCoding->MapSignal(pSignal->GetUniqueId());
            }
        }

    }

    //Map Publisher
    IEcu* pEcu = nullptr;
    UID_ELEMENT unTempId = INVALID_UID_ELEMENT;
    UID_ELEMENT uidPublisher = ui.comboPublisher->currentData().value<UID_ELEMENT>();
    pSignal->MapNode(eTx, uidPublisher);
    m_pLdfCluster->GetElement(eEcuElement, uidPublisher, (IElement**)&pEcu);
    unTempId = pSignal->GetUniqueId();
    if ( nullptr != pEcu )
    {
        pEcu->MapSignal(eTx, unTempId);
    }

    int count = ui.listSubscribers->count();
    for(int index = 0; index < count; index++)
    {
        QListWidgetItem* itr = ui.listSubscribers->item(index);


        if ( Qt::Checked == itr->checkState() )
        {
            unTempId = itr->data(Qt::UserRole).value<UID_ELEMENT>();
            pSignal->MapNode(eRx, unTempId);

            m_pLdfCluster->GetElement(eEcuElement, unTempId, (IElement**)&pEcu);
            unTempId = pSignal->GetUniqueId();
            if ( nullptr != pEcu )
            {
                pEcu->MapSignal(eRx, unTempId);
            }
        }
        else
        {
            unTempId = itr->data(Qt::UserRole).value<UID_ELEMENT>();
            pSignal->UnMapNode(eRx, unTempId);

            m_pLdfCluster->GetElement(eEcuElement, unTempId, (IElement**)&pEcu);
            unTempId = pSignal->GetUniqueId();
            if ( nullptr != pEcu )
            {
                pEcu->UnMapSignal(eRx, unTempId);
            }
        }
    }
}
