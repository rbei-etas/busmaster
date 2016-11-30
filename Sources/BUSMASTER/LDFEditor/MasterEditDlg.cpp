#include "MasterEditDlg.h"
#include "LDFDatabaseManager.h"
#include "LINDefines.h"
#include "LDFUtility.h"
#include "qmessagebox.h"
MasterEditDlg::MasterEditDlg(IEcu* pouMasterEcu, QWidget* parent)
    : QDialog(parent)
{
    m_pouMasterEcu = pouMasterEcu;
    ui.setupUi(this);



    SetUpUi();
    connect(ui.buttonOk, SIGNAL(clicked()), this, SLOT(OnOk()));
    connect(ui.buttonCanel, SIGNAL(clicked()), this, SLOT(OnCancel()));

    ui.editName->setValidator(new QRegExpValidator(QRegExp(defIdentifier_RegExp)));
    ui.editJitter->setValidator(new QDoubleValidator);
    ui.editTimeBase->setValidator(new QDoubleValidator);

    QIcon ouWindowIcon;
    ouWindowIcon.addFile(":/LDFEditorIcons/Resources/icons/16x16/Ecu.png", QSize(16, 16));
    ouWindowIcon.addFile(":/LDFEditorIcons/Resources/icons/22x22/Ecu.png", QSize(16, 16));
    ouWindowIcon.addFile(":/LDFEditorIcons/Resources/icons/32x32/Ecu.png", QSize(16, 16));

    setWindowIcon(ouWindowIcon);

}

MasterEditDlg::~MasterEditDlg()
{
}

void MasterEditDlg::OnOk()
{
    if ( m_pouMasterEcu == nullptr )
    {
        return ;
    }

    if ( ui.editName->text().length() <= 0 )
    {
        QMessageBox::critical(this, "Error", "Name Should Not be empty", QMessageBox::Ok);
        return;
    }

    if ( ui.editName->text() == "Master" )
    {
        QMessageBox::critical(this, "Error", "Name Master is keyword and not allowed as ECU Name", QMessageBox::Ok);
        return;
    }

    std::string strName;
    m_pouMasterEcu->SetName(ui.editName->text().toStdString());

    LinEcuProps ouEcuProps;
    ouEcuProps.m_eEcuType = eEcuNone;
    m_pouMasterEcu->GetProperties(ouEcuProps);

    ouEcuProps.mMasterProps.m_fJitter = ui.editJitter->text().toDouble();
    ouEcuProps.mMasterProps.m_fTimeBase = ui.editTimeBase->text().toDouble();
    m_pouMasterEcu->SetProperties(ouEcuProps);
    LDFDatabaseManager::GetDatabaseManager()->setDocumentModified(true);
    accept();
}

void MasterEditDlg::OnCancel()
{
    reject();
}

void MasterEditDlg::SetUpUi()
{
    if ( m_pouMasterEcu == nullptr )
    {
        return ;
    }

    std::string strName;
    m_pouMasterEcu->GetName(strName);
    ui.editName->setText(QString::fromStdString(strName));

    QString strTitle = "Edit Master - ";
    strTitle+= + strName.c_str();
    setWindowTitle(strTitle);

    LinEcuProps ouEcuProps;
    ouEcuProps.m_eEcuType = eEcuNone;
    m_pouMasterEcu->GetProperties(ouEcuProps);

    QString strTemp;
    GetString(ouEcuProps.mMasterProps.m_fJitter, strTemp);
    ui.editJitter->setText(strTemp);

    GetString(ouEcuProps.mMasterProps.m_fTimeBase, strTemp);
    ui.editTimeBase->setText(strTemp);
}
