#include "SlaveDlg.h"
#include "LDFDefines.h"
#include "LDFUtility.h"
#include "qmessagebox.h"
#include "FaultSignalsDlg.h"

SlaveDlg::SlaveDlg(ICluster* pouCluster, IEcu** pouEcu, bool bNew, QWidget* parent)
    : QDialog(parent)
{
    m_pBaseCluster = pouCluster;
    ui.setupUi(this);
    m_pouEcu = pouEcu;

    vPrepareValidations();

    vPopulateResposeErrorCombo();

    if ( bNew == true )
    {
        m_bModeEdit = false;
        vPrepareForNewSlave();
        vPrepareDefaultValues();

    }
    else
    {
        m_bModeEdit = true;
        vPrepareForEditEcu();
    }

    m_bEcuInfoRead = false;
    connect(ui.comboProtocol, SIGNAL(currentIndexChanged(int)), this, SLOT(onProtocolSelectionChanged(int)));
    connect(ui.buttonFaultSignals, SIGNAL(clicked()), this, SLOT(onEditFaultSignals()));
    connect(ui.buttonOption, SIGNAL(accepted()), this, SLOT(onSelectionOk()));
    connect(ui.buttonOption, SIGNAL(rejected()), this, SLOT(onSelecetionCancel()));

    QIcon ouWindowIcon;
    ouWindowIcon.addFile(":/LDFEditorIcons/Resources/icons/16x16/Ecu.png", QSize(16, 16));
    ouWindowIcon.addFile(":/LDFEditorIcons/Resources/icons/22x22/Ecu.png", QSize(16, 16));
    ouWindowIcon.addFile(":/LDFEditorIcons/Resources/icons/32x32/Ecu.png", QSize(16, 16));

    setWindowIcon(ouWindowIcon);
}

void SlaveDlg::onSelectionOk()
{
    if ( 0 == nValidateValue() )
    {
        IEcu* pouEcu;
        LinEcuProps ouEcuProps;
        ouEcuProps.m_eEcuType = eEcuNone;
        if ( m_bModeEdit == false )
        {
            m_pBaseCluster->CreateElement(eEcuElement, (IElement**)&pouEcu);
        }
        else
        {
            pouEcu = *m_pouEcu;
        }

        pouEcu->SetName(ui.editName->text().toStdString());
        pouEcu->GetProperties(ouEcuProps);
        ouEcuProps.m_eEcuType = eSlave;

        ouEcuProps.mSlaveProps.m_dNASTimeout = ui.editNAS->text().toDouble();
        ouEcuProps.mSlaveProps.m_dNCRTimeout = ui.editNCR->text().toDouble();
        ouEcuProps.mSlaveProps.m_dP2Min = ui.editP2MIN->text().toDouble();
        ouEcuProps.mSlaveProps.m_dSTMin = ui.editSTMIN->text().toDouble();
        ouEcuProps.mSlaveProps.m_fProtocolVersion = ui.comboProtocol->currentText().toDouble();
        ouEcuProps.mSlaveProps.m_nConfiguredNAD = GetUnsignedInt(ui.editConfigNAD->text());
        ouEcuProps.mSlaveProps.m_nFunctionId = GetUnsignedInt(ui.editFunctionId->text());
        ouEcuProps.mSlaveProps.m_nSupplierId = GetUnsignedInt(ui.editSupplierID->text());
        ouEcuProps.mSlaveProps.m_nInitialNAD = GetUnsignedInt(ui.editInitialNAD->text());
        ouEcuProps.mSlaveProps.m_nVariant = GetUnsignedInt(ui.editVariant->text());

        if(ui.comboRespErr->currentText().toStdString().empty() == false)
        {
			ouEcuProps.mSlaveProps.m_nRespErrSignal = ui.comboRespErr->currentData().value<UID_ELEMENT>();
        }
        else
        {
			ouEcuProps.mSlaveProps.m_nRespErrSignal = INVALID_UID_ELEMENT;
        }

        // Fault State signals
		ouEcuProps.mSlaveProps.m_nFaultStateSignals = m_lstFaultSignals;

        pouEcu->SetProperties(ouEcuProps);

        *m_pouEcu = pouEcu;
        LDFDatabaseManager::GetDatabaseManager()->setDocumentModified(true);
        accept();
    }
}

void SlaveDlg::onSelecetionCancel()
{
    reject();
}

void SlaveDlg::onEditFaultSignals()
{
    double fSlaveVersion = ui.comboProtocol->currentText().toDouble();
    FaultSignalsDlg dlg(*m_pouEcu, fSlaveVersion, m_lstFaultSignals, this);
    if ( QDialog::Accepted == dlg.exec() )
    {
        dlg.vGetSignalList(m_lstFaultSignals);
        nFillFaultStateSignals();
    }
}

int SlaveDlg::nValidateValue()
{
    //TODO:: Validations
    if ( false == m_bEcuInfoRead )
    {
        std::string strName;
        LinEcuProps ouecuProps;
        ouecuProps.m_eEcuType = eEcuNone;
        std::map<UID_ELEMENT, IElement*> ouUidElementMap;
        m_pBaseCluster->GetElementList(eEcuElement, ouUidElementMap);

for ( auto itrEcu : ouUidElementMap )
        {
            itrEcu.second->GetName(strName);
            m_MapNames[strName] = strName;
            ((IEcu*)itrEcu.second)->GetProperties(ouecuProps);
            if ( ouecuProps.m_eEcuType == eSlave )
            {
				m_MapunConfigId[ouecuProps.mSlaveProps.m_nConfiguredNAD] = ouecuProps.mSlaveProps.m_nConfiguredNAD;
            }
        }
        m_bEcuInfoRead = true;

        if ( true == m_bModeEdit )
        {
            (*m_pouEcu)->GetProperties(ouecuProps);
            (*m_pouEcu)->GetName(strName);

            auto itr = m_MapNames.find(strName);
            if ( itr != m_MapNames.end() )
            {
                m_MapNames.erase(itr);
            }

			auto itrId = m_MapunConfigId.find(ouecuProps.mSlaveProps.m_nConfiguredNAD);
            if ( itrId != m_MapunConfigId.end() )
            {
                m_MapunConfigId.erase(itrId);
            }
        }

    }
    std::string strName = ui.editName->text().toStdString();

    unsigned int unIConfigId = GetUnsignedInt(ui.editConfigNAD->text());
    QMessageBox msgBox;
    ERRORCODE ecCode = EC_SUCCESS;

    if ( strName == "" )
    {
        msgBox.setText("Invalid ECU Name (Empty Name).");
        msgBox.exec();
        return -1;
    }
    else if( m_MapNames.find(strName) != m_MapNames.end() )
    {
        msgBox.setText("ECU Name Already Exists.");
        msgBox.exec();
        return -1;
    }

    else if ( m_MapunConfigId.find(unIConfigId) != m_MapunConfigId.end() )
    {
        msgBox.setText("Config NAD Already Exists.");
        msgBox.exec();
        return -1;
    }
    return 0;
}

void SlaveDlg::vPrepareForEditEcu()
{
    IEcu* pEcu = *m_pouEcu;

    if ( nullptr != pEcu )
    {
       LinEcuProps ouEcuProps;
        ouEcuProps.m_eEcuType = eEcuNone;
        std::string strName = "";

        pEcu->GetName(strName);
        QString strTitle = "Edit Slave - ";
        strTitle+= + strName.c_str();
        setWindowTitle(strTitle);

        pEcu->GetProperties(ouEcuProps);

        //m_ouEcuCurrentProps = ouEcuProps;

        ui.editName->setText(QString::fromStdString(strName));
		ui.editConfigNAD->setText(GetString(ouEcuProps.mSlaveProps.m_nConfiguredNAD));
        ui.editFunctionId->setText(GetString(ouEcuProps.mSlaveProps.m_nFunctionId));
        ui.editInitialNAD->setText(GetString(ouEcuProps.mSlaveProps.m_nInitialNAD));
        ui.editNAS->setText(GetString(ouEcuProps.mSlaveProps.m_dNASTimeout));
        ui.editNCR->setText(GetString(ouEcuProps.mSlaveProps.m_dNCRTimeout));
        ui.editP2MIN->setText(GetString(ouEcuProps.mSlaveProps.m_dP2Min));
        ui.editSTMIN->setText(GetString(ouEcuProps.mSlaveProps.m_dSTMin));
        ui.editSupplierID->setText(GetString(ouEcuProps.mSlaveProps.m_nSupplierId));
        ui.editVariant->setText(GetString(ouEcuProps.mSlaveProps.m_nVariant));
        vInitialiseProtocolComboBox(ouEcuProps.mSlaveProps.m_fProtocolVersion);

        m_lstFaultSignals = ouEcuProps.mSlaveProps.m_nFaultStateSignals;
        nFillFaultStateSignals();
    }
}
int SlaveDlg::nFillFaultStateSignals()
{
    if ( m_bModeEdit == false )
    {
        return 0;
    }
    LinEcuProps ouEcuProps;
    ouEcuProps.m_eEcuType = eEcuNone;
    std::string strName = "";

    (*m_pouEcu)->GetName(strName);
    (*m_pouEcu)->GetProperties(ouEcuProps);

    if ( ouEcuProps.mSlaveProps.m_fProtocolVersion == 2.1 && ouEcuProps.m_eEcuType == eSlave )
    {

        QString strTemp = "";
        IElement* pElement;
for ( auto itr : m_lstFaultSignals )
        {
            LDFDatabaseManager::GetDatabaseManager()->GetLDFCluster()->GetElement(eSignalElement, itr, &pElement);
            if ( pElement != nullptr )
            {
                pElement->GetName(strName);
                strTemp+= "," + QString::fromStdString(strName.c_str());
            }
        }
        ui.editFaultStateSignals->setText(strTemp.mid(1));
    }

    return 0;
}

void SlaveDlg::vPrepareForNewSlave()
{
    vInitialiseProtocolComboBox();
    ui.buttonFaultSignals->setDisabled(true);
    setWindowTitle("Create Slave");

}

void SlaveDlg::vPopulateResposeErrorCombo(bool bEdited)
{
    IEcu* pECU = *m_pouEcu;
    VALIDATE_POINTER(pECU);

    LinEcuProps ouEcuProps;
    ouEcuProps.m_eEcuType = eEcuNone;
    pECU->GetProperties(ouEcuProps);

    double fProtocolVer =  ouEcuProps.mSlaveProps.m_fProtocolVersion;

    if(bEdited == true)
    {
        fProtocolVer = ui.comboProtocol->currentText().toDouble();
    }
    if (ouEcuProps.m_eEcuType == eSlave && fProtocolVer == defVersion_1_3)
    {
        return;
    }

    std::map<ISignal*, ISignal*> mapSignals;
    pECU->GetSignalList(eTx, mapSignals);
    ui.comboRespErr->clear();
    LINSignalProps ouSignalProps;
    ouSignalProps.eType = eInvalidProtocol;
    std::string omstrSigName;
    ui.comboRespErr->addItem("");
    QVariant qVar;
    int nIndex = 1;
for(auto itrSignal : mapSignals)
    {
        ((ISignal*)itrSignal.second)->GetProperties(ouSignalProps);

        if(ouSignalProps.m_ouSignalType == eSignalNormal)
        {
            ((ISignal*)itrSignal.second)->GetName(omstrSigName);
            ui.comboRespErr->addItem(QString::fromStdString(omstrSigName));
            qVar.setValue(((ISignal*)itrSignal.second)->GetUniqueId());
            ui.comboRespErr->setItemData(nIndex, qVar, Qt::UserRole);
            nIndex++;
        }
    }

    IEcu* pEcu = *m_pouEcu;

    if(nullptr == pEcu)
    {
        return;
    }

    LinEcuProps ouECUProps;
    ouEcuProps.m_eEcuType = eEcuNone;
    (*m_pouEcu)->GetProperties(ouECUProps);

    if(ouECUProps.mSlaveProps.m_nRespErrSignal != INVALID_UID_ELEMENT)
    {
        IElement* pSignal = nullptr;
        m_pBaseCluster->GetElement(eSignalElement, ouECUProps.mSlaveProps.m_nRespErrSignal, &pSignal);

        if(nullptr != pSignal)
        {
            pSignal->GetName(omstrSigName);
            ui.comboRespErr->setCurrentText(omstrSigName.c_str());
        }
    }
}

void SlaveDlg::vInitialiseProtocolComboBox(double dEcuVer)
{
    ui.comboProtocol->clear();
    LIN_Settings ouLIN_Settings;
    m_pBaseCluster->GetProperties(eLINClusterProperties, &ouLIN_Settings);
    QString chText;

    chText.sprintf("%.1f", dEcuVer);

    double dProtocolVer = ouLIN_Settings.m_dProtocolVers;
    if ( dEcuVer != defVersion_1_3 && dEcuVer != defVersion_2_0 && dEcuVer != defVersion_2_1 )
    {
        dEcuVer = ouLIN_Settings.m_dProtocolVers;
    }

    if ( dProtocolVer == defVersion_1_3 )
    {
        ui.comboProtocol->insertItem(0, defLIN_VERSION_1_3);
        ui.comboProtocol->setCurrentIndex(0);
    }
    else if ( dProtocolVer == defVersion_2_0 )
    {
        ui.comboProtocol->insertItem(0, defLIN_VERSION_1_3);
        ui.comboProtocol->insertItem(1, defLIN_VERSION_2_0);
        ui.comboProtocol->setCurrentIndex(1);
    }
    else if ( dProtocolVer == defVersion_2_1 )
    {
        ui.comboProtocol->insertItem(0, defLIN_VERSION_1_3);
        ui.comboProtocol->insertItem(1, defLIN_VERSION_2_0);
        ui.comboProtocol->insertItem(2, defLIN_VERSION_2_1);
        ui.comboProtocol->setCurrentIndex(2);
    }


    if ( dEcuVer == defVersion_1_3 )
    {
        vPrepareUiForVer13();
        ui.comboProtocol->setCurrentText(chText);
    }
    else if ( dEcuVer == defVersion_2_0 )
    {
        vPrepareUiForVer20();
        ui.comboProtocol->setCurrentText(chText);
    }
    else if ( dEcuVer == defVersion_2_1 )
    {
        vPrepareUiForVer21();
        ui.comboProtocol->setCurrentText(chText);
    }
}

void SlaveDlg::vPrepareValidations()
{

    ui.editSupplierID->setValidator(new QIntValidator);
    ui.editFunctionId->setValidator(new QIntValidator);
    ui.editVariant->setValidator(new QIntValidator);
    ui.editName->setValidator(new QRegExpValidator(QRegExp(defIdentifier_RegExp)));
    ui.editP2MIN->setValidator(new QDoubleValidator);
    ui.editSTMIN->setValidator(new QDoubleValidator);
    ui.editNAS->setValidator(new QDoubleValidator);
    ui.editNCR->setValidator(new QDoubleValidator);

    if(true == LDFDatabaseManager::GetDatabaseManager()->bIsDisplayHexOn())
    {
        ui.editConfigNAD->setValidator(new QRegExpValidator(QRegExp(defHex_RegExp)));
        ui.editInitialNAD->setValidator(new QRegExpValidator(QRegExp(defHex_RegExp)));
        ui.editFunctionId->setValidator(new QRegExpValidator(QRegExp(defHex_RegExp)));
        ui.editSupplierID->setValidator(new QRegExpValidator(QRegExp(defHex_RegExp)));
        ui.editVariant->setValidator(new QRegExpValidator(QRegExp(defHex_RegExp)));
    }
    else
    {
        ui.editConfigNAD->setValidator(new QIntValidator);
        ui.editInitialNAD->setValidator(new QIntValidator);
        ui.editFunctionId->setValidator(new QIntValidator);
        ui.editSupplierID->setValidator(new QIntValidator);
        ui.editVariant->setValidator(new QIntValidator);
    }
}

void SlaveDlg::vPrepareDefaultValues()
{
    bool bHex = LDFDatabaseManager::GetDatabaseManager()->bIsDisplayHexOn();
    QString strTemp;


    ui.editP2MIN->setText("50.000000");         //def_DEFAULT_P2MIN

    ui.editSTMIN->setText("0.000000");          //def_DEFAULT_STMIN

    ui.editNCR->setText("1000.0000");           //def_DEFAULT_NCR

    ui.editNAS->setText("1000.0000");           //def_DEFAULT_NAS

    GetString(0xFFFF, strTemp);
    ui.editSupplierID->setText(strTemp);        //Supplier Id

    ui.editFunctionId->setText(strTemp);        //Function Id

    GetString(0xFFF, strTemp);
    ui.editVariant->setText(strTemp);           //Variant
}



void SlaveDlg::vPrepareUiForVer13()
{
    ui.groupDiagTime->hide();
    ui.groupProductId->hide();
    ui.editInitialNAD->setDisabled(true);
    ui.labelInitialNAD->setDisabled(true);
    ui.grpRespErr->setDisabled(true);
    ui.grpRespErr->hide();
    ui.groupFaultStateSignals->hide();
}

void SlaveDlg::vPrepareUiForVer20()
{
    ui.groupDiagTime->show();
    ui.groupProductId->show();
    ui.editInitialNAD->setEnabled(true);
    ui.labelInitialNAD->setEnabled(true);
    ui.comboProtocol->show();
    ui.comboProtocol->show();
    ui.editInitialNAD->setDisabled(true);
    ui.editNAS->setDisabled(true);
    ui.editNCR->setDisabled(true);
    ui.grpRespErr->setEnabled(true);
    ui.grpRespErr->show();
    ui.groupFaultStateSignals->hide();
}

void SlaveDlg::vPrepareUiForVer21()
{
    //May Be Last Combo selection changed there proerties.
    ui.groupDiagTime->show();
    ui.groupProductId->show();
    ui.editInitialNAD->setEnabled(true);
    ui.labelInitialNAD->setEnabled(true);
    ui.comboProtocol->show();
    ui.comboProtocol->show();
    ui.editInitialNAD->setEnabled(true);
    ui.editNAS->setEnabled(true);
    ui.editNCR->setEnabled(true);
    ui.grpRespErr->setEnabled(true);
    ui.grpRespErr->show();
    ui.groupFaultStateSignals->show();
}


void SlaveDlg::onProtocolSelectionChanged(int nIndex)
{
    QString str = ui.comboProtocol->currentText();
    if ( str == defLIN_VERSION_1_3 )
    {
        vPrepareUiForVer13();
    }
    else if ( str == defLIN_VERSION_2_0 )
    {
        vPopulateResposeErrorCombo(true);
        vPrepareUiForVer20();
    }
    else if ( str == defLIN_VERSION_2_1 )
    {
        vPopulateResposeErrorCombo(true);
        vPrepareUiForVer21();
    }
    int i = 0;
}

SlaveDlg::~SlaveDlg()
{

}
