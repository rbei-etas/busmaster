#include "NodeConfigurationDlg.h"
#include "LDFUtility.h"
#include "LDFDatabaseManager.h"
#include <QTreeWidgetItem>
#include <QComboBox>
#include "LDFTagDefines.h"
#include "LineEditWidget.h"
#include <sstream>      // std::ostringstream
#include <fstream>
#include <QMessageBox>

NodeConfigurationDlg::NodeConfigurationDlg(INodeConfiguration** pNodeConfig, eMode ouMode, QWidget* parent)
    : QDialog(parent)
{
    ui.setupUi(this);
    m_ouMode = ouMode;
    m_pLdfCluster = LDFDatabaseManager::GetDatabaseManager()->GetLDFCluster();
    m_pouNodeConfig = pNodeConfig;
    vCollectNodeNames();
    vPrepareUI();

    connect(ui.buttonBox, SIGNAL(accepted()), this, SLOT(onSelectionOk()));
    connect(ui.buttonBox, SIGNAL(rejected()), this, SLOT(onSelecetionCancel()));
    connect(ui.btnAdd, SIGNAL(clicked(bool)), this, SLOT(onButtonClickAdd(bool)));
    connect(ui.btnDelete, SIGNAL(clicked(bool)), this, SLOT(onButtonClickDelete(bool)));

    QIcon ouWindowIcon;
    ouWindowIcon.addFile(":/LDFEditorIcons/Resources/icons/16x16/Ecu.png", QSize(16, 16));
    ouWindowIcon.addFile(":/LDFEditorIcons/Resources/icons/22x22/Ecu.png", QSize(16, 16));
    ouWindowIcon.addFile(":/LDFEditorIcons/Resources/icons/32x32/Ecu.png", QSize(16, 16));

    setWindowIcon(ouWindowIcon);

}

NodeConfigurationDlg::~NodeConfigurationDlg()
{

}

void NodeConfigurationDlg::vCollectNodeNames()
{
    auto pNodeConfig = *m_pouNodeConfig;
    std::string strNodeName, strNodeConfigName;
    std::list<CompositeNode> lstCompNodes;
    if(nullptr != pNodeConfig)
    {
        pNodeConfig->GetName(strNodeConfigName);
    }
    std::map<UID_ELEMENT, IElement*> mapNodes;
    m_pLdfCluster->GetElementList(eLinNodeComposition, mapNodes);
    m_logicalNodes.clear();
for(auto itrNode : mapNodes)
    {
        itrNode.second->GetName(strNodeName);
        if(strNodeName != strNodeConfigName)
        {
            m_mapNodeNames[strNodeName] = strNodeName;

            ((INodeConfiguration*)itrNode.second)->GetCompositeNodes(lstCompNodes);

for(auto itrCompNode : lstCompNodes)
            {
for(auto itrLogicNode : itrCompNode.m_strLogicalNodes)
                {
                    m_logicalNodes[itrLogicNode] = itrLogicNode;
                }
            }
        }
    }
}

void NodeConfigurationDlg::vPrepareUI()
{
    ui.lineEdit->setValidator(new QRegExpValidator(QRegExp(defIdentifier_RegExp)));
    if(eNew == m_ouMode)
    {
        vPrepareUIForNew();
    }
    else if(eEdit == m_ouMode)
    {
        vPrepareUIForEdit();
    }
}

void NodeConfigurationDlg::onSelectionOk()
{
    std::string ouErrString = "";

    if(EC_FAILURE == nValidateNodes(ouErrString))
    {
        QMessageBox omMsgBox;
        omMsgBox.setText(ouErrString.c_str());
        omMsgBox.exec();
    }
    else
    {
        if(eNew == m_ouMode)
        {
            INodeConfiguration* pNodeConfig = nullptr;
            m_pLdfCluster->CreateElement(eLinNodeComposition, (IElement**)&pNodeConfig);

            *m_pouNodeConfig = pNodeConfig;
        }
        std::list<std::string> lstLogicalNodes;
        std::list<CompositeNode> ouCompositeNodeList;
        QStringList qStrList;
        for(int nIndex = 0; nIndex < ui.tableWidget->rowCount(); nIndex++)
        {
            UID_ELEMENT uidECU = ((QComboBox*)ui.tableWidget->cellWidget(nIndex, 0))->currentData(Qt::UserRole).value<UID_ELEMENT>();
            CompositeNode ouCompNode;
            ouCompNode.m_uidNode = uidECU;
            lstLogicalNodes.clear();
            qStrList.clear();

            LineEditWidget* pLineEdit = (LineEditWidget*)ui.tableWidget->cellWidget(nIndex, 1);
            if(nullptr != pLineEdit)
            {
                qStrList = pLineEdit->text().split(',');
                for(auto itrStr = qStrList.begin(); itrStr!=qStrList.end(); itrStr++)
                {
                    lstLogicalNodes.push_back(itrStr->toStdString());
                }
            }

            ouCompNode.m_strLogicalNodes = lstLogicalNodes;

            ouCompositeNodeList.push_back(ouCompNode);
        }

        (*m_pouNodeConfig)->SetCompositeNodes(ouCompositeNodeList);
        (*m_pouNodeConfig)->SetName(ui.lineEdit->text().toStdString());

        LDFDatabaseManager::GetDatabaseManager()->setDocumentModified(true);
        accept();
    }
}

ERRORCODE NodeConfigurationDlg::nValidateNodes(std::string& ouErrString)
{
    std::string strNodeConfigName = ui.lineEdit->text().toStdString();
    if(strNodeConfigName.empty() == true)
    {
        std::ostringstream strStream;
        strStream << "Error: Node Configuration name cannot be empty";
        ouErrString = strStream.str();
        return EC_FAILURE;
    }

    if(m_mapNodeNames.find(strNodeConfigName) != m_mapNodeNames.end())
    {
        std::ostringstream strStream;
        strStream << "Error: Node Configuration name <b>" << strNodeConfigName << "</b> is not unique";
        ouErrString = strStream.str();
        return EC_FAILURE;
    }

    QStringList qStrList;
    std::map<std::string, std::string> mapLogicalNodes;
    for(int nIndex = 0; nIndex < ui.tableWidget->rowCount(); nIndex++)
    {
        QComboBox* pComboBox = (QComboBox*)ui.tableWidget->cellWidget(nIndex, 0);

        std::string strCompNode = "";
        if(nullptr != pComboBox)
        {
            strCompNode = pComboBox->currentText().toStdString();
        }

        LineEditWidget* pLineEdit = (LineEditWidget*)ui.tableWidget->cellWidget(nIndex, 1);
        if(nullptr != pLineEdit)
        {
            qStrList = pLineEdit->text().split(',');
            for(auto itrStr = qStrList.begin(); itrStr != qStrList.end(); itrStr++)
                //for(auto itrStr : qStrList)
            {
                std::string strLogicalNode = itrStr->toStdString();

                if(strLogicalNode.empty() == true)
                {
                    std::ostringstream strStream;
                    strStream << "Error: Logical Node cannot be empty " << "in Composite Node <b>"
                              << strCompNode << "</b> Row Number " << nIndex+1;
                    ouErrString = strStream.str();
                    return EC_FAILURE;
                }

                if(m_mapCompositeNodes.find(itrStr->toStdString()) != m_mapCompositeNodes.end())
                {
                    std::ostringstream strStream;
                    strStream << "Error: Logical Node <b>" << itrStr->toStdString() << "</b> not unique in Composite Node <b>"
                              << strCompNode << "</b> Row Number " << nIndex+1;
                    ouErrString = strStream.str();
                    return EC_FAILURE;
                }

                if(mapLogicalNodes.find(itrStr->toStdString()) != mapLogicalNodes.end())
                {
                    std::ostringstream strStream;
                    strStream << "Error: Logical Node <b>" << itrStr->toStdString() << "</b> not unique in Composite Node <b>"
                              << strCompNode << "</b> Row Number " << nIndex+1;
                    ouErrString = strStream.str();
                    return EC_FAILURE;
                }

                if(m_logicalNodes.find(itrStr->toStdString()) != m_logicalNodes.end())
                {
                    std::ostringstream strStream;
                    strStream << "Error: Logical Node <b>" << itrStr->toStdString() << "</b> not unique in Composite Node <b>"
                              << strCompNode << "</b> Row Number " << nIndex+1;
                    ouErrString = strStream.str();
                    return EC_FAILURE;
                }

                mapLogicalNodes[strLogicalNode] = strLogicalNode;
            }
        }
    }
    return EC_SUCCESS;
}

void NodeConfigurationDlg::vPrepareUIForNew()
{
    setWindowTitle("Create Node Configuration");
}

void NodeConfigurationDlg::vPrepareUIForEdit()
{
    auto pNodeConfig = *m_pouNodeConfig;
    if(nullptr == pNodeConfig)
    {
        return;
    }

    std::string omstrNodeConfig;
    pNodeConfig->GetName(omstrNodeConfig);

    ui.lineEdit->setText(QString::fromStdString(omstrNodeConfig));

    QString strTitle = "Edit Node Configuration - ";
    strTitle+= + omstrNodeConfig.c_str();
    setWindowTitle(strTitle);

    std::list<CompositeNode> lstCompNodes;
    IElement* pECU = nullptr;
    std::string omstrNodeName = "";
    QList<QVariant> ouColumns;
    int nRow = 0;

    pNodeConfig->GetCompositeNodes(lstCompNodes);

    ui.tableWidget->setRowCount(lstCompNodes.size());
for(auto itrNodeComp : lstCompNodes)
    {
        ouColumns.clear();
        omstrNodeName = "";
        if(EC_SUCCESS == m_pLdfCluster->GetElement(eEcuElement, itrNodeComp.m_uidNode, &pECU))
        {
            pECU->GetName(omstrNodeName);
        }
        QComboBox* pComboEcus = new QComboBox(nullptr);
        vPopulateEcusCombo(&pComboEcus);
        pComboEcus->setCurrentText(QString::fromStdString(omstrNodeName));

        std::string strLogicNodes = "";
        int nCount =0;
for(auto itrLogicNode : itrNodeComp.m_strLogicalNodes)
        {
            if(0 == nCount)
            {
                strLogicNodes = itrLogicNode ;
            }
            else
            {
                strLogicNodes += "," + itrLogicNode ;
            }
            nCount++;
        }

        QString omString = QString::fromStdString(strLogicNodes.c_str());
        ouColumns.push_back(QVariant());
        ouColumns.push_back(QVariant());
        ui.tableWidget->InsertRow(nRow, ouColumns);
        ui.tableWidget->setCellWidget(nRow, 0, pComboEcus);

        LineEditWidget* pLineEdit = new LineEditWidget(nRow, ui.tableWidget);
        pLineEdit->setText((strLogicNodes.c_str()));
        pLineEdit->setFrame(false);
        pLineEdit->setValidator(new QRegExpValidator(QRegExp(defIdentifier_RegExp_MULTIPLE)));
        ui.tableWidget->setCellWidget(nRow, 1, pLineEdit);

        nRow++;
    }

}

void NodeConfigurationDlg::onEditSelectionChange(int nIndex)
{
    ui.tableWidget->selectRow(nIndex);
}

void NodeConfigurationDlg::vPopulateEcusCombo(QComboBox** pComboBox)
{
    m_mapCompositeNodes.clear();
    std::string omStrName = "";
    std::map<UID_ELEMENT, IElement*> mapEcus;
    m_pLdfCluster->GetElementList(eEcuElement, mapEcus);
    int nIndex = 0;
    QVariant qVar;
for(auto itrEcu : mapEcus)
    {
        itrEcu.second->GetName(omStrName);
        m_mapCompositeNodes[omStrName] = omStrName;
        (*pComboBox)->addItem(QString::fromStdString(omStrName));
        qVar.setValue(itrEcu.second->GetUniqueId());
        (*pComboBox)->setItemData(nIndex, qVar, Qt::UserRole);
        nIndex++;
    }
}

void NodeConfigurationDlg::onButtonClickAdd(bool)
{
    int nRow = ui.tableWidget->rowCount();
    ui.tableWidget->setRowCount(nRow+1);

    QComboBox* pComboEcus = new QComboBox(nullptr);
    vPopulateEcusCombo(&pComboEcus);
    QList<QVariant> ouColumns;
    ouColumns.push_back(QVariant());
    ouColumns.push_back(QVariant());
    ui.tableWidget->InsertRow(nRow, ouColumns);
    ui.tableWidget->setCellWidget(nRow, 0, pComboEcus);

    LineEditWidget* pLineEdit = new LineEditWidget(nRow, ui.tableWidget);
    pLineEdit->setText("");
    pLineEdit->setFrame(false);
    pLineEdit->setValidator(new QRegExpValidator(QRegExp(defIdentifier_RegExp_MULTIPLE)));
    ui.tableWidget->setCellWidget(nRow, 1, pLineEdit);
}

void NodeConfigurationDlg::onButtonClickDelete(bool)
{
    ui.tableWidget->removeRow( ui.tableWidget->currentRow() );
}

void NodeConfigurationDlg::onSelecetionCancel()
{
    reject();
}
