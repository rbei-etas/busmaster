#include "CodingDlg.h"
#include "LDFUtility.h"
#include "qmessagebox.h"
#include "LineEditWidget.h"
CodingDlg::CodingDlg(ICluster* pouCluster, ICoding** pouCoding, bool bNew, QWidget* parent)
    : QDialog(parent)
{
    ui.setupUi(this);
    m_pLdfCluster = pouCluster;
    m_pCoding = pouCoding;
    m_bEditMode = !bNew;
    vPrepareValidations();
    ui.buttonDeletePhysical->setDisabled(true);
    ui.buttonDeleteLogical->setDisabled(true);


    setWindowTitle("Create Coding");

    if ( true == m_bEditMode )
    {
        vPrepareUiForEdit();
    }
    connect(ui.buttonAddLogical, SIGNAL(clicked()), this, SLOT(AddLogicalRow()));
    connect(ui.buttonAddPhysical, SIGNAL(clicked()), this, SLOT(AddPhysicalRow()));
    connect(ui.buttonDeleteLogical, SIGNAL(clicked()), this, SLOT(DeleteLogicalRow()));
    connect(ui.buttonDeletePhysical, SIGNAL(clicked()), this, SLOT(DeletePhysicalRow()));
    connect(ui.tableLogicalValues, SIGNAL(ui.tableLogicalValues->itemSelectionChanged()), this, SLOT(ItemSelectionChangeLogical()));

    connect(ui.buttonOption, SIGNAL(accepted()), this, SLOT(onSelectionOk()));
    connect(ui.buttonOption, SIGNAL(rejected()), this, SLOT(onSelecetionCancel()));

    QIcon ouWindowIcon;
    ouWindowIcon.addFile(":/LDFEditorIcons/Resources/icons/16x16/Coding.png", QSize(16, 16));
    ouWindowIcon.addFile(":/LDFEditorIcons/Resources/icons/22x22/Coding.png", QSize(16, 16));
    ouWindowIcon.addFile(":/LDFEditorIcons/Resources/icons/32x32/Coding.png", QSize(16, 16));

    setWindowIcon(ouWindowIcon);
}


void CodingDlg::ItemSelectionChangeLogical()
{
    if ( ui.tableLogicalValues->currentItem () != nullptr )
    {
        ui.buttonDeleteLogical->setDisabled(true);
    }
}

CodingDlg::~CodingDlg()
{
}
void CodingDlg::AddPhysicalRow()
{
    int nRowCount = ui.tablePhysicalValues->rowCount();
    ui.tablePhysicalValues->setRowCount(nRowCount+1);
    QList<QVariant> ouColumns;
    QVariant ouVariant;

    LineEditWidget* pFactorEdit = new LineEditWidget(nRowCount, ui.tablePhysicalValues);
    pFactorEdit->setValidator(new QDoubleValidator());
    pFactorEdit->setFrame(false);

    LineEditWidget* pOffset = new LineEditWidget(nRowCount, ui.tablePhysicalValues);
    pOffset->setValidator(new QDoubleValidator());
    pOffset->setFrame(false);

    LineEditWidget* pMin = new LineEditWidget(nRowCount, ui.tablePhysicalValues);

    if(true == LDFDatabaseManager::GetDatabaseManager()->bIsDisplayHexOn())
    {
        pMin->setValidator(new QRegExpValidator(QRegExp(defHex_RegExp)));
    }
    else
    {
        pMin->setValidator(new QIntValidator());
    }
    //
    pMin->setFrame(false);

    LineEditWidget* pMax = new LineEditWidget(nRowCount, ui.tablePhysicalValues);

    if(true == LDFDatabaseManager::GetDatabaseManager()->bIsDisplayHexOn())
    {
        pMax->setValidator(new QRegExpValidator(QRegExp(defHex_RegExp)));
    }
    else
    {
        pMax->setValidator(new QIntValidator());
    }
    pMax->setFrame(false);

    LineEditWidget* pUnit = new LineEditWidget(nRowCount, ui.tablePhysicalValues);
    pUnit->setValidator(new QRegExpValidator(QRegExp(defUnit_RegExp)));
    pUnit->setFrame(false);

    LineEditWidget* pTextInfo = new LineEditWidget(nRowCount, ui.tablePhysicalValues);
    pTextInfo->setFrame(false);


    ouColumns.push_back(QVariant());
    ouColumns.push_back(QVariant());
    ouColumns.push_back(QVariant());
    ouColumns.push_back(QVariant());
    ouColumns.push_back(QVariant());

    ui.tablePhysicalValues->InsertRow(nRowCount, ouColumns);

    ui.tablePhysicalValues->setCellWidget(nRowCount, 0, pOffset);
    ui.tablePhysicalValues->setCellWidget(nRowCount, 1, pFactorEdit);
    ui.tablePhysicalValues->setCellWidget(nRowCount, 2, pMin);
    ui.tablePhysicalValues->setCellWidget(nRowCount, 3, pMax);
    ui.tablePhysicalValues->setCellWidget(nRowCount, 4, pUnit);
    ui.tablePhysicalValues->setCurrentItem(ui.tableLogicalValues->item(nRowCount, 0));
    ui.tablePhysicalValues->scrollToBottom();
    ui.buttonDeletePhysical->setDisabled(false);
}

void CodingDlg::DeleteLogicalRow()
{
    int currentIndex = ui.tableLogicalValues->currentRow();
    if ( currentIndex >= 0 )
    {
        ui.tableLogicalValues->removeRow(currentIndex);
    }
    if ( ui.tableLogicalValues->rowCount() <= 0 )
    {
        ui.buttonDeleteLogical->setDisabled(true);
    }
}

void CodingDlg::DeletePhysicalRow()
{
    int currentIndex = ui.tablePhysicalValues->currentRow();
    if ( currentIndex >= 0 )
    {
        ui.tablePhysicalValues->removeRow(currentIndex);
    }
    if ( ui.tablePhysicalValues->rowCount() <= 0 )
    {
        ui.buttonDeletePhysical->setDisabled(true);
    }
}

void CodingDlg::AddLogicalRow()
{
    int nRowCount = ui.tableLogicalValues->rowCount();
    ui.tableLogicalValues->setRowCount(nRowCount+1);

    QList<QVariant> ouColumns;
    QVariant ouVariant;
    LineEditWidget* pRawEdit = new LineEditWidget(nRowCount, ui.tableLogicalValues);

    if ( LDFDatabaseManager::GetDatabaseManager()->bIsDisplayHexOn() == true )
    {
        pRawEdit->setValidator(new QRegExpValidator(QRegExp(defHex_RegExp)));
    }
    else
    {
        pRawEdit->setValidator(new QIntValidator());
    }

    LineEditWidget* pDescEdit = new LineEditWidget(nRowCount, ui.tableLogicalValues);



    pRawEdit->setFrame(false);
    pDescEdit->setFrame(false);

    ouColumns.push_back(QVariant());
    ouColumns.push_back(QVariant());
    ui.tableLogicalValues->InsertRow(nRowCount, ouColumns);

    ui.tableLogicalValues->setCellWidget(nRowCount, 0, pRawEdit);
    ui.tableLogicalValues->setCellWidget(nRowCount, 1, pDescEdit);
    ui.tableLogicalValues->scrollToBottom();
    ui.tableLogicalValues->setCurrentItem(ui.tableLogicalValues->item(nRowCount, 0));
    ui.buttonDeleteLogical->setDisabled(false);
}

void CodingDlg::vPrepareUiForEdit()
{
    auto pCoding = *m_pCoding;
    if ( nullptr != pCoding )
    {
        std::string strName;
        CompuMethodProps ouCompuMethodsProps;

        //Name
        pCoding->GetName(strName);
        ui.editName->setText(QString::fromStdString(strName));
        QString strTitle = "Edit Coding - ";
        strTitle+= + strName.c_str();
        setWindowTitle(strTitle);

        //Props;
        pCoding->GetProperties(ouCompuMethodsProps);
        int nSize = ouCompuMethodsProps.m_ouLinCompuMethods.m_ouLogicalValueList.size();
        ui.tableLogicalValues->setRowCount(nSize);
        QList<QVariant> ouColumns;
        QVariant ouVariant;
        int nRow = 0;

        ui.tableLogicalValues->setEditTriggers(QAbstractItemView::AllEditTriggers);
for ( auto itr : ouCompuMethodsProps.m_ouLinCompuMethods.m_ouLogicalValueList )
        {
            LineEditWidget* pRawEdit = new LineEditWidget(nRow, ui.tableLogicalValues);
            if ( LDFDatabaseManager::GetDatabaseManager()->bIsDisplayHexOn() == true )
            {
                pRawEdit->setValidator(new QRegExpValidator(QRegExp(defHex_RegExp)));
            }
            else
            {
                pRawEdit->setValidator(new QIntValidator());
            }

            pRawEdit->setFrame(false);

            LineEditWidget* pDescEdit = new LineEditWidget(nRow, ui.tableLogicalValues);
            pDescEdit->setFrame(false);

            pRawEdit->setText(GetString(itr.first));
            pDescEdit->setText(QString::fromStdString(itr.second));
            ouColumns.push_back(QVariant());
            ouColumns.push_back(QVariant());
            ui.tableLogicalValues->InsertRow(nRow, ouColumns);

            ui.tableLogicalValues->setCellWidget(nRow, 0, pRawEdit);
            ui.tableLogicalValues->setCellWidget(nRow++, 1, pDescEdit);
            ui.buttonDeleteLogical->setDisabled(false);
            ouColumns.clear();
        }

        nSize = ouCompuMethodsProps.m_ouLinCompuMethods.m_ouPhysicalValueList.size();
        ui.tablePhysicalValues->setRowCount(nSize);
        ui.tablePhysicalValues->setEditTriggers(QAbstractItemView::AllEditTriggers);
        nRow = 0;
for ( auto itr : ouCompuMethodsProps.m_ouLinCompuMethods.m_ouPhysicalValueList )
        {
            ouColumns.clear();
            LineEditWidget* pFactorEdit = new LineEditWidget(nRow, ui.tablePhysicalValues);
            pFactorEdit->setValidator(new QDoubleValidator());
            pFactorEdit->setFrame(false);
            pFactorEdit->setText(GetString(itr.m_dFactor));

            LineEditWidget* pOffset = new LineEditWidget(nRow, ui.tablePhysicalValues);
            pOffset->setValidator(new QDoubleValidator());
            pOffset->setFrame(false);
            pOffset->setText(GetString(itr.m_dOffset));


            LineEditWidget* pMin = new LineEditWidget(nRow, ui.tablePhysicalValues);

            if(true == LDFDatabaseManager::GetDatabaseManager()->bIsDisplayHexOn())
            {
                pMin->setValidator(new QRegExpValidator(QRegExp(defHex_RegExp)));
            }
            else
            {
                pMin->setValidator(new QIntValidator());
            }
            pMin->setFrame(false);
            pMin->setText(GetString(itr.m_unMin));

            LineEditWidget* pMax = new LineEditWidget(nRow, ui.tablePhysicalValues);

            if(true == LDFDatabaseManager::GetDatabaseManager()->bIsDisplayHexOn())
            {
                pMax->setValidator(new QRegExpValidator(QRegExp(defHex_RegExp)));
            }
            else
            {
                pMax->setValidator(new QIntValidator());
            }
            pMax->setFrame(false);
            pMax->setText(GetString(itr.m_unMax));

            LineEditWidget* pUnit = new LineEditWidget(nRow, ui.tablePhysicalValues);
            pUnit->setValidator(new QRegExpValidator(QRegExp(defUnit_RegExp)));
            pUnit->setFrame(false);
            pUnit->setText(itr.m_strTextInfo.c_str());

            /*LineEditWidget *pTextInfo = new LineEditWidget(nRow, ui.tablePhysicalValues);
            pTextInfo->setFrame(false);
            pTextInfo->setText(itr.m_strUnit.c_str());*/


            ouColumns.push_back(QVariant(itr.m_dOffset));
            ouColumns.push_back(QVariant(itr.m_dFactor));
            ouColumns.push_back(QVariant(itr.m_unMin));
            ouColumns.push_back(QVariant(itr.m_unMax));
            ouColumns.push_back(QVariant(itr.m_strTextInfo.c_str()));

            ui.tablePhysicalValues->InsertRow(nRow, ouColumns);

            ui.tablePhysicalValues->setCellWidget(nRow, 0, pOffset);
            ui.tablePhysicalValues->setCellWidget(nRow, 1, pFactorEdit);
            ui.tablePhysicalValues->setCellWidget(nRow, 2, pMin);
            ui.tablePhysicalValues->setCellWidget(nRow, 3, pMax);
            ui.tablePhysicalValues->setCellWidget(nRow++, 4, pUnit);
            ui.buttonDeletePhysical->setDisabled(false);
        }
    }
}

void CodingDlg::vPrepareValidations()
{
    ui.editName->setValidator(new QRegExpValidator(QRegExp(defIdentifier_RegExp)));

    std::map<UID_ELEMENT, IElement*> ouCodingMap;
    m_pLdfCluster->GetElementList(eCodingElement, ouCodingMap);

    std::string strName;
    std::string strTempName = "";

for ( auto itr : ouCodingMap )
    {
        itr.second->GetName(strName);
        m_strCodingMap[strName] = itr.second->GetUniqueId();
    }


    if ( true == m_bEditMode && nullptr != m_pCoding )
    {
        (*m_pCoding)->GetName(strTempName);
        auto itr = m_strCodingMap.find(strTempName);
        if ( m_strCodingMap.end() != itr )
        {
            m_strCodingMap.erase(itr);
        }
    }
}

void CodingDlg::onSelectionOk()
{
    std::string strName = ui.editName->text().toStdString();
    if ( strName == "" )
    {
        QMessageBox::critical(this, "Error", "Name Should Not Be Empty", QMessageBox::Ok);
        return;
    }
    if ( m_strCodingMap.find(strName) != m_strCodingMap.end())
    {
        QMessageBox::critical(this, "Error", "Duplicate Coding Name", QMessageBox::Ok);
    }

    //
    ICoding* pCoding;
    if ( false == m_bEditMode )
    {
        m_pLdfCluster->CreateElement(eCodingElement, (IElement**)&pCoding);
        *m_pCoding = pCoding;
    }
    else
    {
        pCoding = *m_pCoding;
    }

    if ( pCoding != nullptr )
    {
        CompuMethodProps ouCompuMethods;
        pCoding->GetProperties(ouCompuMethods);

        if ( FillPhysicalValues(ouCompuMethods) == -1 )
        {
            return;
        }




        ouCompuMethods.m_eType = eLINProtocol;
        FillLogicalValues(ouCompuMethods);

        pCoding->SetProperties(ouCompuMethods);
        pCoding->SetName(strName);
        LDFDatabaseManager::GetDatabaseManager()->setDocumentModified(true);
        accept();
    }
}
int CodingDlg::FillPhysicalValues(CompuMethodProps& ouCompuMethods)
{
    ouCompuMethods.m_ouLinCompuMethods.m_ouPhysicalValueList.clear();
    PhysicalValue ouPhysicalValue;
    int nRowCount = ui.tablePhysicalValues->rowCount();
    for ( int i = 0 ; i < nRowCount; i++ )
    {
        LineEditWidget* pOffsetWidget = (LineEditWidget*)ui.tablePhysicalValues->cellWidget( i, 0 );
        LineEditWidget* pFactorWidget = (LineEditWidget*)ui.tablePhysicalValues->cellWidget( i, 1 );
        LineEditWidget* pMinWidget = (LineEditWidget*)ui.tablePhysicalValues->cellWidget( i, 2 );
        LineEditWidget* pMaxWidget = (LineEditWidget*)ui.tablePhysicalValues->cellWidget( i, 3 );
        LineEditWidget* pUnitWidget = (LineEditWidget*)ui.tablePhysicalValues->cellWidget( i, 4 );

        ouPhysicalValue.m_dOffset = pOffsetWidget->text().toDouble();
        ouPhysicalValue.m_dFactor = pFactorWidget->text().toDouble();
        ouPhysicalValue.m_unMin = GetUnsignedInt(pMinWidget->text());
        ouPhysicalValue.m_unMax = GetUnsignedInt(pMaxWidget->text());
        if ( ouPhysicalValue.m_unMax < ouPhysicalValue.m_unMin )
        {
            QString strErr;
            strErr.sprintf("Maximum value should be greater than minimum value (in row %d)", nRowCount+1);
            QMessageBox::critical(this, "Error", strErr, QMessageBox::Ok);
            return -1;
        }

        ouPhysicalValue.m_strTextInfo = pUnitWidget->text().toStdString();
        ouCompuMethods.m_ouLinCompuMethods.m_ouPhysicalValueList.push_back(ouPhysicalValue);
    }
    return 0;
}

void CodingDlg::FillLogicalValues(CompuMethodProps& ouCompuMethods)
{
    ouCompuMethods.m_ouLinCompuMethods.m_ouLogicalValueList.clear();
    int nRowCount = ui.tableLogicalValues->rowCount();
    unsigned int unRawVal = 0;
    std::string strDesc;
    for ( int i = 0 ; i < nRowCount; i++ )
    {
        LineEditWidget* pWidget = (LineEditWidget*)ui.tableLogicalValues->cellWidget( i, 0 );
        if ( nullptr != pWidget )
        {
            if ( pWidget->text().length() == 0 )
            {
                continue;
            }
            unRawVal = GetUnsignedInt(pWidget->text());
            pWidget = (LineEditWidget*)ui.tableLogicalValues->cellWidget( i, 1 );
            if ( nullptr != pWidget )
            {
                strDesc = pWidget->text().toStdString();
                ouCompuMethods.m_ouLinCompuMethods.m_ouLogicalValueList[unRawVal] = strDesc;
            }
        }
    }
}

void CodingDlg::onSelecetionCancel()
{
    reject();
}