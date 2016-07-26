#include "FaultSignalsDlg.h"
#include "LDFUtility.h"
#include "LDFDatabaseManager.h"
FaultSignalsDlg::FaultSignalsDlg(IEcu* pouEcu,double fSlaveProtVer, std::list<UID_ELEMENT>& lstSignals, QWidget* parent)
    : QDialog(parent)
{
    ui.setupUi(this);
    m_pEcu = pouEcu;
    m_lstSignals = lstSignals;
    m_fSlaveProtVersion = fSlaveProtVer;

    vPrepareUi();
    connect(ui.buttonBox, SIGNAL(accepted()), this,  SLOT(OnselectedOk()));
    connect(ui.buttonBox, SIGNAL(rejected()), this,  SLOT(reject()));

    QIcon ouWindowIcon;
    ouWindowIcon.addFile(":/LDFEditorIcons/Resources/icons/16x16/Signals.png", QSize(16, 16));
    ouWindowIcon.addFile(":/LDFEditorIcons/Resources/icons/22x22/Signals.png", QSize(16, 16));
    ouWindowIcon.addFile(":/LDFEditorIcons/Resources/icons/32x32/Signals.png", QSize(16, 16));

    setWindowTitle("Select Fault State signals:");

    setWindowIcon(ouWindowIcon);

}

FaultSignalsDlg::~FaultSignalsDlg()
{
}

void FaultSignalsDlg::vPrepareUi()
{
    VALIDATE_POINTER(m_pEcu);

    EcuProperties ouEcuProps;
    m_pEcu->GetProperties(ouEcuProps);
    if ( ouEcuProps.m_eEcuType == eLIN_Slave )
    {
        if ( false == bAreEqual(m_fSlaveProtVersion , 2.1) )
        {
            return;
        }
    }
    std::map<ISignal*, ISignal*> ouSignsalList;
    m_pEcu->GetSignalList(eTx, ouSignsalList);
    QList<QVariant> ouColumnList;
    std::string strName;
    int nRow = 0;
    ui.tableSignalList->setRowCount(ouSignsalList.size());
    SignalProps ouSignalProps;
for ( auto itr : ouSignsalList )
    {
        itr.second->GetProperties(ouSignalProps);
        if ( ouSignalProps.m_ouLINSignalProps.m_ouSignalType == eSignalNormal )
        {
            itr.second->GetName(strName);
            ouColumnList.push_back(QVariant(strName.c_str()));
            ui.tableSignalList->InsertRow(nRow, ouColumnList);
            auto itr2 =  std::find(m_lstSignals.begin(), m_lstSignals.end(),
                                   itr.second->GetUniqueId());
            if ( m_lstSignals.end() != itr2)
            {
                ui.tableSignalList->item(nRow, 0)->setCheckState(Qt::Checked);
            }
            else
            {
                ui.tableSignalList->item(nRow, 0)->setCheckState(Qt::Unchecked);
            }
            ui.tableSignalList->item(nRow, 0)->setData(Qt::UserRole, QVariant::fromValue(itr.second->GetUniqueId()));
            nRow++;
        }
        ouColumnList.clear();
    }
}

void FaultSignalsDlg::OnselectedOk()
{
    m_lstSignals.clear();

    EcuProperties ouEcuProps;
    m_pEcu->GetProperties(ouEcuProps);
    //ouEcuProps.m_ouSlavePros.m_nFaultStateSignals.clear();
    int nRows = ui.tableSignalList->rowCount();
    for ( int i = 0; i < nRows; i++ )
    {
        if ( Qt::Checked == ui.tableSignalList->item(i , 0)->checkState())
        {
            /*ouEcuProps.m_ouSlavePros.m_nFaultStateSignals.push_back(
                ui.tableSignalList->item(i , 0)->data(Qt::UserRole).value<UID_ELEMENT>());*/

            m_lstSignals.push_back(ui.tableSignalList->item(i , 0)->data(Qt::UserRole).value<UID_ELEMENT>());
        }
    }
    m_pEcu->SetProperties(ouEcuProps);
    LDFDatabaseManager::GetDatabaseManager()->setDocumentModified(true);
    accept();
    return;
}

void FaultSignalsDlg::vGetSignalList(std::list<UID_ELEMENT>& lstSignals)
{
    lstSignals = m_lstSignals;
}
