#include "LDFCLusterPropsDlg.h"
#include "LDFUtility.h"
LDFCLusterPropsDlg::LDFCLusterPropsDlg(ICluster* pCluster, QWidget* parent)
    : QDialog(parent)
{
    ui.setupUi(this);
    m_pCluster = pCluster;
    vPrepareUi();
    connect(ui.buttonOption, SIGNAL(accepted()), this, SLOT(OnSelectOk()));
    connect(ui.buttonOption, SIGNAL(rejected()), this, SLOT(reject()));



}

LDFCLusterPropsDlg::~LDFCLusterPropsDlg()
{

}
void LDFCLusterPropsDlg::OnSelectOk()
{
    LIN_Settings ouLINSettings;
    m_pCluster->GetProperties(eLINClusterProperties, &ouLINSettings);
    ouLINSettings.m_dBaudRate = ui.comboBaudRate->currentText().toDouble();
    ouLINSettings.m_strChannelName = ui.editChannelName->text().toStdString();
    m_pCluster->SetProperties(eLINClusterProperties, &ouLINSettings);
    accept();
}

void LDFCLusterPropsDlg::vPrepareUi()
{
    ui.editProtocolVer->setDisabled(true);
    ui.editLangaugeVer->setDisabled(true);



    if ( nullptr != m_pCluster )
    {
        LIN_Settings ouLINSettings;
        m_pCluster->GetProperties(eLINClusterProperties, &ouLINSettings);

        if ( bAreEqual (ouLINSettings.m_dProtocolVers , 2.1) == false)
        {
            ui.editChannelName->hide();
            ui.labelChannelName->hide();
        }


        ui.editLangaugeVer->setText(GetString(ouLINSettings.m_dLanguageVers));
        ui.editProtocolVer->setText(GetString(ouLINSettings.m_dProtocolVers));
        ui.editChannelName->setText(ouLINSettings.m_strChannelName.c_str());

        ui.comboBaudRate->setValidator(new QDoubleValidator());

        ui.comboBaudRate->setCurrentText(GetString(ouLINSettings.m_dBaudRate));
    }

}
