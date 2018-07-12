#include "SignalGroupDlg.h"
#include "LDFUtility.h"
#include "qmath.h"
#include "qmessagebox.h"

SignalGroupDlg::SignalGroupDlg(ICluster* pouCluster, ISignalGroup** pSignalGroup, bool bNew, QWidget* parent)
    : QDialog(parent)
{
    ui.setupUi(this);

    m_pLdfCluster = pouCluster;
    m_bEditMode = !bNew;
    m_pSignalGroup = pSignalGroup;


    vPrepareValidations();

    if ( true == m_bEditMode )
    {
        vPrepareUiForEditMode();
    }
    else
    {
        vPrepareUiForNewMode();;
    }
    connect(ui.buttonOption, SIGNAL(accepted()), this, SLOT(onSelectionOk()));
    connect(ui.buttonOption, SIGNAL(rejected()), this, SLOT(reject()));

    QIcon ouWindowIcon;
    ouWindowIcon.addFile(":/LDFEditorIcons/Resources/icons/16x16/Signals.png", QSize(16, 16));
    ouWindowIcon.addFile(":/LDFEditorIcons/Resources/icons/22x22/Signals.png", QSize(16, 16));
    ouWindowIcon.addFile(":/LDFEditorIcons/Resources/icons/32x32/Signals.png", QSize(16, 16));

    setWindowIcon(ouWindowIcon);


}

SignalGroupDlg::~SignalGroupDlg()
{

}

void SignalGroupDlg::onSelectionOk()
{
    if ( 0 == nValidate() )
    {
        ISignalGroup* pElement;
        if ( m_bEditMode == false )
        {
            m_pLdfCluster->CreateElement(eSignalGroupElement, (IElement**)&pElement);
            *m_pSignalGroup = pElement;
        }
        else
        {
            pElement = *m_pSignalGroup;
        }

        if ( nullptr != pElement )
        {
            std::string strName = ui.editName->text().toStdString();


            LinSignalGroupProps ouSignalGroupProps;
            ouSignalGroupProps.eType = eLINProtocol;
            ouSignalGroupProps.m_nGroupSize = GetUnsignedInt(ui.editSize->text(), 10);
            pElement->SetProperties(ouSignalGroupProps);

            int nRow = ui.tableSignals->rowCount();
            QLineEdit* pEdit ;
            SignalInstanse ouInstance;
            ouInstance.m_nStartBit = INVALID_DATA;
            ouInstance.m_nUpdateBitPos = INVALID_DATA;
            UID_ELEMENT unId = INVALID_UID_ELEMENT;
            for ( int i = 0 ; i < nRow; i++ )
            {
                if ( ui.tableSignals->item(i, 0)->checkState() == Qt::Checked )
                {
                    unId = ui.tableSignals->item(i, 0)->data(Qt::UserRole).value<UID_ELEMENT>();
                    QLineEdit* pEdit = (QLineEdit*)ui.tableSignals->cellWidget(i, 1);
                    ouInstance.m_nStartBit = pEdit->text().toInt();
                    pElement->MapSignal(unId, ouInstance);
                }
                else
                {
                    unId = ui.tableSignals->item(i, 0)->data(Qt::UserRole).value<UID_ELEMENT>();
                    QLineEdit* pEdit = (QLineEdit*)ui.tableSignals->cellWidget(i, 1);
                    ouInstance.m_nStartBit = pEdit->text().toInt();
                    pElement->UnMapSignal(unId);

                }
            }

            pElement->SetName(strName);

        }
        accept();
    }

}

int SignalGroupDlg::nValidate()
{
    std::string strName = ui.editName->text().toStdString();
    if ( strName == "" )
    {
        QMessageBox::critical(this, "Error", "Name Should Not Be Empty", QMessageBox::Ok);
        return 1;
    }
    if ( m_ouExistingNameMap.find(strName) != m_ouExistingNameMap.end())
    {
        QMessageBox::critical(this, "Error", "Duplicate SignalGroup Name", QMessageBox::Ok);
        return 1;
    }

    unsigned int unSize = GetUnsignedInt(ui.editSize->text(), 10);

    if ( unSize <= 0 )
    {
        QMessageBox::critical(this, "Error", "Size Should be greater than 0", QMessageBox::Ok);
        return 1;
    }


    int nRow = ui.tableSignals->rowCount();
    QLineEdit* pEdit ;
    for ( int i = 0 ; i < nRow; i++ )
    {
        if ( ui.tableSignals->item(i, 0)->checkState() == Qt::Checked )
        {
            QLineEdit* pEdit = (QLineEdit*)ui.tableSignals->cellWidget(i, 1);
            if ( pEdit->text().toInt() > unSize-1 )
            {
                QMessageBox::critical(this, "Error", "Signal Offset should not be more than group size", QMessageBox::Ok);
                pEdit->setFocus();
                return 1;
            }
        }
    }
    return 0;

}

void SignalGroupDlg::vPrepareValidations()
{
    ui.editName->setValidator(new QRegExpValidator(QRegExp(defIdentifier_RegExp)));

    QIntValidator* pSignalLengthValidator = new QIntValidator();
    pSignalLengthValidator->setRange(0, 63);
    ui.editSize->setValidator(pSignalLengthValidator);

    vFillExistingNameList();
}
void SignalGroupDlg::vFillExistingNameList()
{
    eClusterElementType ouClusterElement[2] = {eSignalElement, eSignalGroupElement};
    std::string strName;
    std::map<UID_ELEMENT, IElement*> ouElementMap;
    for ( int i = 0 ; i < 2; i++ )
    {
        eClusterElementType eElementType = ouClusterElement[i];
        m_pLdfCluster->GetElementList(eElementType, ouElementMap);

for ( auto itr : ouElementMap )
        {
            itr.second->GetName(strName);
            m_ouExistingNameMap[strName] = itr.second->GetUniqueId();
        }
        ouElementMap.clear();
    }
    if ( true == m_bEditMode )
    {
        (*m_pSignalGroup)->GetName(strName);
        auto itrSignal = m_ouExistingNameMap.find(strName);
        if ( itrSignal !=  m_ouExistingNameMap.end() )
        {
            m_ouExistingNameMap.erase(itrSignal);
        }
    }
}


void SignalGroupDlg::vPrepareUiForEditMode()
{
    if ( *m_pSignalGroup == nullptr )
    {
        return;
    }

    auto* pSignalGroup = *m_pSignalGroup;
    LinSignalGroupProps ouSignalGroupProps;
    std::string strName;
    pSignalGroup->GetName(strName);
    ui.editName->setText(strName.c_str());

    pSignalGroup->GetProperties(ouSignalGroupProps);
    int nSize =  qMin( (int)ouSignalGroupProps.m_nGroupSize , 64);


    ui.editSize->setText(GetString(nSize, 10));

    std::map<ISignal*, SignalInstanse> ouSignalList;

    pSignalGroup->GetSignalList(ouSignalList);

    std::map<UID_ELEMENT, int> ouSelectedSignals;

    int nRow = 0;
    QList<QVariant> ouColumns;
for ( auto itr : ouSignalList )
    {
        ouSelectedSignals[itr.first->GetUniqueId()] = itr.second.m_nStartBit;
    }

    std::map<UID_ELEMENT, IElement*> ouElementMap;
    eClusterElementType eElementType = eSignalElement;
    m_pLdfCluster->GetElementList(eElementType, ouElementMap);

    ui.tableSignals->setRowCount(ouElementMap.size());

for ( auto itr : ouElementMap )
    {
        ouColumns.clear();
        itr.second->GetName(strName);
        ouColumns.push_back(strName.c_str());
        ouColumns.push_back("");
        ui.tableSignals->InsertRow(nRow, ouColumns);

        QLineEdit* poffsetWidget = new QLineEdit();
        poffsetWidget->setFrame(false);

        QIntValidator* pSignalLengthValidator = new QIntValidator();
        pSignalLengthValidator->setRange(0, 63);
        poffsetWidget->setValidator(pSignalLengthValidator);

        ui.tableSignals->setCellWidget(nRow, 1, poffsetWidget);

        ui.tableSignals->item(nRow, 0)->setData(Qt::UserRole, itr.first);

        auto itrSelectedSig = ouSelectedSignals.find(itr.first);
        if ( itrSelectedSig != ouSelectedSignals.end() )
        {
            ui.tableSignals->item(nRow, 0)->setCheckState(Qt::Checked);
            poffsetWidget->setText(GetString(itrSelectedSig->second,false));
        }
        else
        {
            ui.tableSignals->item(nRow, 0)->setCheckState(Qt::Unchecked);
        }

        nRow++;
    }
}

void SignalGroupDlg::vPrepareUiForNewMode()
{
    setWindowTitle("Create Signal Group");

    //Fill Signals
    std::string strName;
    std::map<UID_ELEMENT, IElement*> ouElementMap;
    eClusterElementType eElementType = eSignalElement;
    m_pLdfCluster->GetElementList(eElementType, ouElementMap);

    ui.tableSignals->setRowCount(ouElementMap.size());
    QList<QVariant> ouColumns;
    int nRow = 0;
for ( auto itr : ouElementMap )
    {
        ouColumns.clear();
        itr.second->GetName(strName);
        ouColumns.push_back(strName.c_str());
        ouColumns.push_back("");
        ui.tableSignals->InsertRow(nRow, ouColumns);

        QLineEdit* poffsetWidget = new QLineEdit();
        poffsetWidget->setFrame(false);

        QIntValidator* pSignalLengthValidator = new QIntValidator();
        pSignalLengthValidator->setRange(0, 63);
        poffsetWidget->setValidator(pSignalLengthValidator);

        ui.tableSignals->setCellWidget(nRow, 1, poffsetWidget);
        ui.tableSignals->item(nRow, 0)->setCheckState(Qt::Unchecked);
        ui.tableSignals->item(nRow, 0)->setData(Qt::UserRole, itr.first);
        nRow++;
    }
}
