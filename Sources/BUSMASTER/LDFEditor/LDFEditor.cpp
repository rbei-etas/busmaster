

#include <QToolButton>
#include <QMessageBox>
#include <QTextEdit>
#include <QDesktopServices>
#include <QPushButton>
#include <QList>
#include <QFileDialog>


#include "IClusterProps.h"
#include "ICluster.h"

#include "LDFDatabaseManager.h"
#include "LDFEditor.h"

#include <math.h>
#include <fstream>
#include <sstream>
#include <algorithm>

#include "AboutLDFEditor.h"
#include "LDFUtility.h"



//#include "../DBManager/LINDBManager/LDFCluster.h"


using namespace std;
LDFEditor::LDFEditor(std::string omstrLdfPath, QWidget* parent)
    : QMainWindow(parent)
{
    QList<int> sizeList;
    sizeList << 25 << 150 << 25;
    m_pWarningTable = nullptr;

    ui.setupUi(this);
    ui.splitter_main->setSizes(sizeList);
    ConnectMenuActions();
    UpdatedRecentFileList();
    SetupWarningPane();
    m_strCurrentLDFFilePath = "";

    connect(LDFDatabaseManager::GetDatabaseManager(), SIGNAL(DocumentModified()), this, SLOT(OnDocumentModified()));

    ui.m_pouLDFAdditionalView->SetInfoLable(ui.labelAdditionalViewHelp);
    ui.m_pouLDFPropertyView->SetAdditionalPropsView(ui.m_pouLDFAdditionalView);
    ui.m_pouLDFElementView->SetLDFNetworkView(ui.m_pouLDFNetworkView);
    ui.dockPaneWarning->hide();

    if(false == omstrLdfPath.empty())
    {
        OpenLDFFile(omstrLdfPath.c_str());
        setWindowFilePath(omstrLdfPath.c_str());
    }
}

LDFEditor::~LDFEditor()
{
    LDFDatabaseManager::GetDatabaseManager()->DeleteDatabaseManager();
}

void LDFEditor::SetupWarningPane()
{
    if ( nullptr == m_pWarningTable )
    {
        m_pWarningTable = new LDFCommonTableWidget();
        QList<QString> ouColumns;
        ouColumns<<"Severity" <<"Description"<<"Action Taken";
        m_pWarningTable->CreateColumns(ouColumns);
        ui.dockPaneWarning->setWidget(m_pWarningTable);
    }

}


void LDFEditor::closeEvent (QCloseEvent* event)
{
    if ( CheckForSaving() == 0 )
    {
        event->ignore();
    }
    else
    {
        event->accept();
    }
}

void LDFEditor::OnDocumentModified()
{
    bool bModified = LDFDatabaseManager::GetDatabaseManager()->bIsDocumentModified();
    ui.actionSave->setDisabled(!bModified);
    if ( LDFDatabaseManager::GetDatabaseManager()->GetLDFFilePath() != "" )
    {
        ui.actionSave_As->setDisabled(false);
    }
    setTitle();
}

void LDFEditor::on_m_pouLDFNetworkView_itemSelectionChanged()
{
    if(ui.m_pouLDFNetworkView->selectedItems().size() > 0)
    {
        ui.labelAdditionalViewHelp->setText(" ");
        QTreeWidgetItem* selecedItem = ui.m_pouLDFNetworkView->selectedItems().at(0);

        if(nullptr != selecedItem)
        {
            TreeItemID ouTreeItemId = selecedItem->data(0, Qt::UserRole).value<TreeItemID>();
            ui.m_pouLDFPropertyView->clear();
            ui.m_pouLDFPropertyView->setColumnCount(0);
            ui.m_pouLDFPropertyView->setRowCount(0);

            ui.m_pouLDFAdditionalView->clear();
            ui.m_pouLDFAdditionalView->setColumnCount(0);
            ui.m_pouLDFAdditionalView->setRowCount(0);

            ui.m_pouLDFElementView->SelectElement(ouTreeItemId);
        }
    }
}


void LDFEditor::on_m_pouLDFElementView_itemSelectionChanged()
{
    if(ui.m_pouLDFElementView->selectedItems().size() > 0)
    {
        QTreeWidgetItem* selecedItem = ui.m_pouLDFElementView->selectedItems().at(0);

        if(nullptr != selecedItem)
        {
            ui.m_pouLDFPropertyView->onTreeItemClicked(selecedItem, 0);
            ui.m_pouLDFAdditionalView->onTreeItemClicked(selecedItem, 0);
        }

    }
}

int LDFEditor::ConnectMenuActions(void)
{
    QIcon newIcon;
    newIcon.addFile(":/LDFEditorIcons/Resources/icons/16x16/new.png", QSize(16, 16));
    newIcon.addFile(":/LDFEditorIcons/Resources/icons/22x22/new.png", QSize(22, 22));
    newIcon.addFile(":/LDFEditorIcons/Resources/icons/32x32/new.png", QSize(32, 32));
    ui.actionNew->setIcon(newIcon);

    QIcon openIcon;
    openIcon.addFile(":/LDFEditorIcons/Resources/icons/16x16/open.png", QSize(16, 16));
    openIcon.addFile(":/LDFEditorIcons/Resources/icons/22x22/open.png", QSize(22, 22));
    openIcon.addFile(":/LDFEditorIcons/Resources/icons/32x32/open.png", QSize(32, 32));
    ui.actionOpen->setIcon(openIcon);


    QIcon saveIcon;
    saveIcon.addFile(":/LDFEditorIcons/Resources/icons/16x16/save.png", QSize(16, 16));
    saveIcon.addFile(":/LDFEditorIcons/Resources/icons/22x22/save.png", QSize(22, 22));
    saveIcon.addFile(":/LDFEditorIcons/Resources/icons/32x32/save.png", QSize(32, 32));
    ui.actionSave->setIcon(saveIcon);
    ui.actionSave->setDisabled(true);

    QIcon saveIconAs;
    saveIconAs.addFile(":/LDFEditorIcons/Resources/icons/16x16/saveas.png", QSize(16, 16));
    saveIconAs.addFile(":/LDFEditorIcons/Resources/icons/22x22/saveas.png", QSize(22, 22));
    saveIconAs.addFile(":/LDFEditorIcons/Resources/icons/32x32/saveas.png", QSize(32, 32));
    ui.actionSave_As->setIcon(saveIconAs);
    ui.actionSave_As->setDisabled(true);

    QIcon hexIcon;
    hexIcon.addFile(":/LDFEditorIcons/Resources/icons/16x16/hex.png", QSize(16, 16));
    hexIcon.addFile(":/LDFEditorIcons/Resources/icons/22x22/hex.png", QSize(22, 22));
    hexIcon.addFile(":/LDFEditorIcons/Resources/icons/32x32/hex.png", QSize(32, 32));
    ui.actionHex->setIcon(hexIcon);
    ui.actionHex->setCheckable(true);
    ui.actionHex->setChecked(false);
    if ( LDFDatabaseManager::GetDatabaseManager()->bIsDisplayHexOn() == true )
    {
        ui.actionHex->setChecked(true);
    }

    QIcon recentIcon;
    recentIcon.addFile(":/LDFEditorIcons/Resources/icons/16x16/recent.png", QSize(16, 16));
    recentIcon.addFile(":/LDFEditorIcons/Resources/icons/22x22/recent.png", QSize(22, 22));
    recentIcon.addFile(":/LDFEditorIcons/Resources/icons/32x32/recent.png", QSize(32, 32));
    ui.actionRecent->setIcon(recentIcon);


    ui.mainToolBar->setMovable(true);
    ui.mainToolBar->setIconSize(QSize(32, 32));


    for (int i = 0; i < 5; ++i)
    {
        recentFileActs[i] = new QAction(this);
        recentFileActs[i]->setVisible(false);
        ui.menu_Recent_Files->addAction(recentFileActs[i]);
        connect(recentFileActs[i], SIGNAL(triggered()),
                this, SLOT(on_actionRecent_triggered()));
    }

    ui.mainToolBar->addAction(ui.menu_New->menuAction());
    ui.menu_New->setIcon(newIcon);

    ui.mainToolBar->addAction(ui.actionOpen);
    ui.mainToolBar->addSeparator();
    ui.mainToolBar->addAction(ui.actionSave);
    ui.mainToolBar->addAction(ui.actionSave_As);
    ui.mainToolBar->addSeparator();
    ui.mainToolBar->addAction(ui.actionHex);

    ui.mainToolBar->addSeparator();
    ui.mainToolBar->addAction(ui.menu_Recent_Files->menuAction());
    ui.menu_Recent_Files->setIcon(recentIcon);

    //Warning Window
    QString strWarningWindow = "&Warnings Window";
    QAction* ouWarningAction = ui.dockPaneWarning->toggleViewAction();
    ouWarningAction->setText(strWarningWindow);
    ui.menuView->addAction(ouWarningAction);
    ui.actionPreview_LDF_File->setDisabled(true);

    return 0;
}

int LDFEditor::on_actionExit_triggered()
{
    if ( CheckForSaving() != 0 )
    {
        QApplication::quit();
    }

    return 0;
}

int LDFEditor::on_actionRecent_triggered()
{
    if ( CheckForSaving() == 0 )
    {
        return 0;
    }
    QAction* pAction = qobject_cast<QAction*>(sender());
    if (pAction)
    {
        if(-1 ==  OpenLDFFile(pAction->data().toString()))
        {
            UpdatedRecentFileList();
        }
    }
    return 1;
}

int LDFEditor::UpdatedRecentFileList()
{
    QStringList strRecentFileList;
    int nMaxRecentFiles = LDFDatabaseManager::GetDatabaseManager()->GetMaxRecentFileCount();
    LDFDatabaseManager::GetDatabaseManager()->GetRecentFileList(strRecentFileList);

    int numRecentFiles = min((int)strRecentFileList.size(), (int)nMaxRecentFiles);

    int i = 0;
    for (auto itr = strRecentFileList.begin(); itr != strRecentFileList.end(); itr++ )
    {
        QString text = tr("&%1 %2").arg(i + 1).arg(*itr);
        QString strFile = *itr;
        std::string str = strFile.toStdString();
        str = text.toStdString();
        QFile file(strFile);
        if(false == file.exists())
        {
            continue;
        }
        recentFileActs[i]->setText(text);
        recentFileActs[i]->setData(*itr);
        recentFileActs[i]->setVisible(true);
        i++;
        if ( i >= nMaxRecentFiles )
        {
            break;
        }
    }
    nMaxRecentFiles = i;
    for (int j = numRecentFiles; j < nMaxRecentFiles; ++j)
    {
        recentFileActs[j]->setVisible(false);
    }

    return 0;
}


int LDFEditor::on_action_About_triggered()
{
    AboutLDFEditor ouAbouDlg(this);
    ouAbouDlg.exec();
    return 0;
}
int LDFEditor::on_actionHelp_triggered(void)
{
    QString strTemp  =  QApplication::applicationDirPath();
    strTemp = strTemp + "/ldfeditor.chm";
    return QDesktopServices::openUrl(QUrl::fromLocalFile(strTemp));
}

int LDFEditor::on_actionOpen_triggered(void)
{
    if ( CheckForSaving() == 0 )
    {
        return 0;
    }

    QString strLDFFilePath = QFileDialog::getOpenFileName(this, tr("Open LDF File..."), "", tr("LDF Files (*.ldf)"));
    OpenLDFFile(strLDFFilePath);
    setWindowFilePath(strLDFFilePath);
    return 0;
}
int LDFEditor::CloseCurrentLDFFile()
{
    ICluster* LDFCluster = LDFDatabaseManager::GetDatabaseManager()->GetLDFCluster();
    if (LDFCluster == nullptr) {
        QMessageBox::critical(this,
                              tr("Unable to load function"),
                              tr("Unable to load CreateLDFCluster function from the DBManager library!"));
        return -1;
    }

    LDFDatabaseManager::GetDatabaseManager()->GetLDFCluster()->Clear();
    LDFDatabaseManager::GetDatabaseManager()->SetLDFFilePath(std::string(""));

    ui.m_pouLDFPropertyView->setRowCount(0);


    ui.m_pouLDFAdditionalView->setColumnCount(0);
    ui.m_pouLDFAdditionalView->setRowCount(0);

    ui.labelAdditionalViewHelp->setText(" ");

    ui.actionSave->setDisabled(true);
    ui.actionSave_As->setDisabled(true);
    ui.actionPreview_LDF_File->setDisabled(true);
    LDFDatabaseManager::GetDatabaseManager()->setDocumentModified(false);

    return 0;
}

int LDFEditor::OpenLDFFile(QString strLDFFilePath)
{
    if ( "" != strLDFFilePath )
    {
        if (CloseCurrentLDFFile())
            return -1;

        list<ParsingResults> ouErrors;
        list<ParsingResults> ouWarnings;
        QFile file(strLDFFilePath);

        if(file.exists() == false)
        {
            QMessageBox::critical(this, "Error", "File does not exists", QMessageBox::Ok);
            return -1;
        }
        ERRORCODE ecResult = LDFDatabaseManager::GetDatabaseManager()->GetLDFCluster()->LoadFromFile(strLDFFilePath.toStdString(), ouErrors, ouWarnings);

        m_pWarningTable->setRowCount(0);
        if ( ecResult == EC_SUCCESS )
        {
            ICluster* pLdfCluster = LDFDatabaseManager::GetDatabaseManager()->GetLDFCluster();
            if ( nullptr != pLdfCluster )
            {
                LIN_Settings ouLINSettings;
                pLdfCluster->GetProperties(eLINClusterProperties, &ouLINSettings);
                if ( !(bAreEqual(ouLINSettings.m_dProtocolVers, defVersion_1_3) ||
                        bAreEqual(ouLINSettings.m_dProtocolVers, defVersion_2_0) ||
                        bAreEqual(ouLINSettings.m_dProtocolVers, defVersion_2_1) ) )
                {
                    ParsingResults ouParsingResults;
                    ouParsingResults.m_ouErrorType = eError;
                    ouParsingResults.m_strErrorDesc = "Invalid Protocol Version (Supported Versions 1.3, 2.0, 2.1)" ;
                    ouParsingResults.m_unErrorCode = 0;
                    ouParsingResults.m_unLineNum = 0;
                    ouParsingResults.m_strActionTaken = "File Can not be loaded";
                    ouErrors.push_back(ouParsingResults);
                    ecResult = EC_FAILURE;
                }
            }

        }
        if ( ouErrors.size() > 0)
        {
            auto itr = ouErrors.begin();
            QString strErrorString;
            if ( ouErrors.end() != itr )
            {
                strErrorString.sprintf("Error While Parsing <br>(Error : %s)</br>", itr->m_strErrorDesc.c_str());
                QMessageBox::critical(this, "Error", strErrorString, QMessageBox::Ok);
            }
        }
        else if ( ouWarnings.size() > 0)
        {
            QList<QVariant> ouRow;
            int nRow = 0;
            m_pWarningTable->setRowCount(ouWarnings.size());
            for ( auto itr : ouWarnings )
            {
                ouRow.clear();
                ouRow.push_back("Warning");
                ouRow.push_back(itr.m_strErrorDesc.c_str());
                ouRow.push_back(itr.m_strActionTaken.c_str());
                m_pWarningTable->InsertRow(nRow, ouRow);
                nRow++;
            }
            ui.dockPaneWarning->show();
        }
        else
        {
            ui.dockPaneWarning->hide();
        }
        if ( ecResult == EC_SUCCESS || ecResult == EC_WARNING_SUCCESS )
        {
            OnFileLoad(strLDFFilePath);
        }
    }
    return 0;
}

int LDFEditor::OnFileLoad(QString strLDFFilePath)
{
    LDFDatabaseManager::GetDatabaseManager()->SetLDFFilePath(strLDFFilePath.toStdString());
    setTitle();
    m_strCurrentLDFFilePath = strLDFFilePath;
    if ( strLDFFilePath.size() > 0 )
    {
        UpdatedRecentFileList();
        ui.actionSave->setDisabled(true);
        ui.actionSave_As->setDisabled(false);
        LDFDatabaseManager::GetDatabaseManager()->setDocumentModified(false);
        ui.actionPreview_LDF_File->setDisabled(false);
    }
    else                                //Empty file created for New Menu
    {

        LDFDatabaseManager::GetDatabaseManager()->setDocumentModified(true);
        ui.actionSave->setDisabled(false);
        ui.actionSave_As->setDisabled(false);
        ui.actionPreview_LDF_File->setDisabled(false);
    }

    return 0;
}
int LDFEditor::CheckForSaving()
{
    if ( LDFDatabaseManager::GetDatabaseManager()->bIsDocumentModified() == true )
    {
        QMessageBox::StandardButton  ret = QMessageBox::warning(this, tr("BUSMASTER LDF Editor"),
                                           tr("The document has been modified.\n"
                                              "Do you want to save your changes?"),
                                           QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
        if (ret == QMessageBox::Save)
        {
            on_actionSave_triggered(); // TODO : Update it to Save
            return 1;
        }
        if (ret == QMessageBox::Discard)
        {
            return 1;
        }
        else
        {
            return 0;
        }
    }
    return 1;
}


void LDFEditor::setTitle()
{
    QString strTitle = "BUSMASTER LDF Editor -";
    strTitle+= LDFDatabaseManager::GetDatabaseManager()->GetLDFFilePath().c_str();
    if ( LDFDatabaseManager::GetDatabaseManager()->bIsDocumentModified() == true )
    {
        strTitle += "*";
    }
    setWindowTitle(strTitle);
}

int LDFEditor::on_actionSave_triggered(void)
{
    if ( ValidateForSaving() == 0 && m_strCurrentLDFFilePath.length() > 0)
    {
        LDFDatabaseManager::GetDatabaseManager()->GetLDFCluster()->SaveToFile(m_strCurrentLDFFilePath.toStdString());
        LDFDatabaseManager::GetDatabaseManager()->setDocumentModified(false);
        ui.actionSave_As->setDisabled(false);
        ui.actionSave->setDisabled(true);
        return 0;
    }

    return 0;
}


int LDFEditor::ValidateForSaving()
{
    int nRet = 0;
    list<ParsingResults> ouErrors;
    list<ParsingResults> ouWarnings;
    m_pWarningTable->setRowCount(0);
    nValidateForCluster(ouErrors, ouWarnings);
    if ( ouErrors.size() > 0 )
    {
        auto itr = ouErrors.begin();
        QString strErrorString;
        if ( ouErrors.end() != itr )
        {
            strErrorString.sprintf("Error While Saving. <br>(Error : %s)</br>", itr->m_strErrorDesc.c_str());
            QMessageBox::critical(this, "Error", strErrorString, QMessageBox::Ok);

        }
        nRet = -1;
    }
    else if ( ouWarnings.size() > 0 )
    {
        QList<QVariant> ouRow;
        int nRow = 0;
        m_pWarningTable->setRowCount(ouWarnings.size());
        for ( auto itr : ouWarnings )
        {
            ouRow.clear();
            ouRow.push_back("Warning");
            ouRow.push_back(itr.m_strErrorDesc.c_str());
            ouRow.push_back(itr.m_strActionTaken.c_str());
            m_pWarningTable->InsertRow(nRow, ouRow);
            nRow++;
        }
        ui.dockPaneWarning->show();
        QMessageBox::StandardButton ret= QMessageBox::warning(this, "Warning", "Warnings found while saving the file.Do you Want to Continue to save?", QMessageBox::Yes|QMessageBox::No);
        if ( ret == QMessageBox::Yes)
        {
            nRet = 0;
        }
        else
        {
            nRet = -1;
        }
    }
    return nRet;
}


int LDFEditor::on_actionSave_As_triggered(void)
{
    if ( ValidateForSaving() == 0 )
    {
        QString strLDFFilePath = QFileDialog::getSaveFileName(this, tr("Save LDF File"), "", tr("LDF Files (*.ldf)"));
        LDFDatabaseManager::GetDatabaseManager()->GetLDFCluster()->SaveToFile(strLDFFilePath.toStdString());
        return 0;
    }
    return 0;
}

int LDFEditor::on_actionNewLIN_1_3_triggered(void)
{
    if ( CheckForSaving() == 0 )
    {
        return 0;
    }

    if (CloseCurrentLDFFile())
        return -1;

    m_strCurrentLDFFilePath = QFileDialog::getSaveFileName(this, tr("New LDF File 1.3"), "", tr("LDF Files (*.ldf)"));

    if(m_strCurrentLDFFilePath.isEmpty() == false)
    {
        CreateDefaultItems(defVersion_1_3);
        LDFDatabaseManager::GetDatabaseManager()->GetLDFCluster()->SaveToFile(m_strCurrentLDFFilePath.toStdString());
        LDFDatabaseManager::GetDatabaseManager()->setDocumentModified(false);
        ui.actionSave_As->setDisabled(false);
        ui.actionSave->setDisabled(true);

        OnFileLoad(m_strCurrentLDFFilePath);
        AddWarningsOfNewFile(defVersion_1_3);
    }
    return 0;
}

int LDFEditor::on_actionNewLIN_2_0_triggered(void)
{
    if ( CheckForSaving() == 0 )
    {
        return 0;
    }

    if (CloseCurrentLDFFile())
        return -1;

    m_strCurrentLDFFilePath = QFileDialog::getSaveFileName(this, tr("New LDF File 2.0"), "", tr("LDF Files (*.ldf)"));

    if(m_strCurrentLDFFilePath.isEmpty() == false)
    {
        CreateDefaultItems(defVersion_2_0);
        LDFDatabaseManager::GetDatabaseManager()->GetLDFCluster()->SaveToFile(m_strCurrentLDFFilePath.toStdString());
        LDFDatabaseManager::GetDatabaseManager()->setDocumentModified(false);
        ui.actionSave_As->setDisabled(false);
        ui.actionSave->setDisabled(true);

        OnFileLoad(m_strCurrentLDFFilePath);
        AddWarningsOfNewFile(defVersion_2_0);
    }
    return 0;
}

int LDFEditor::on_actionNewLIN_2_1_triggered(void)
{
    if ( CheckForSaving() == 0 )
    {
        return 0;
    }

    if (CloseCurrentLDFFile())
        return -1;

    m_strCurrentLDFFilePath = QFileDialog::getSaveFileName(this, tr("New LDF File 2.1"), "", tr("LDF Files (*.ldf)"));

    if(m_strCurrentLDFFilePath.isEmpty() == false)
    {
        CreateDefaultItems(defVersion_2_1);
        LDFDatabaseManager::GetDatabaseManager()->GetLDFCluster()->SaveToFile(m_strCurrentLDFFilePath.toStdString());
        LDFDatabaseManager::GetDatabaseManager()->setDocumentModified(false);
        ui.actionSave_As->setDisabled(false);
        ui.actionSave->setDisabled(true);

        OnFileLoad(m_strCurrentLDFFilePath);
        AddWarningsOfNewFile(defVersion_2_1);
    }
    return 0;
}


int LDFEditor::on_actionHex_triggered(void)
{
    LDFDatabaseManager::GetDatabaseManager()->SetDisplayHex(ui.actionHex->isChecked());
    return 0;
}

void LDFEditor::AddWarningsOfNewFile(double dVers)
{
    QList<QVariant> ouRow;
    int nRow = 0;
    m_pWarningTable->setRowCount(3);


    ouRow.push_back("Warning");
    ouRow.push_back("Valid LDF File should Contain Atleast one signal");
    ouRow.push_back("");
    m_pWarningTable->InsertRow(nRow, ouRow);
    nRow++;

    ouRow.clear();
    ouRow.push_back("Warning");
    ouRow.push_back("Valid LDF File should Contain Atleast one Unconditional Frame");
    ouRow.push_back("");
    m_pWarningTable->InsertRow(nRow, ouRow);
    nRow++;

    ouRow.clear();
    ouRow.push_back("Warning");
    ouRow.push_back("Valid LDF File should Contain Atleast one Schedule Table");
    ouRow.push_back("");
    m_pWarningTable->InsertRow(nRow, ouRow);

    ui.dockPaneWarning->show();
    nRow++;
}

int LDFEditor::on_actionPreview_LDF_File_triggered()
{
    QLDFViewerProcess* pouViewerProc = new QLDFViewerProcess();
    pouViewerProc->start();

    return 0;
}


int LDFEditor::nValidateForCluster(list<ParsingResults>& ouErrors, list<ParsingResults>& ouWarnings)
{
    //1. Ecu Validations
    std::map<UID_ELEMENT, IElement*> pElement;

    LDFDatabaseManager::GetDatabaseManager()->GetLDFCluster()->GetElementList(eEcuElement, pElement);
    bool bMasterFound = false;
    bool bSlaveFound = false;

for ( auto itr : pElement )
    {
        eEcuType ouEcuType;
        IEcu* pEcu = (IEcu*)itr.second;
        if ( nullptr == pEcu )
        {
            continue;
        }

        pEcu->GetEcuType(ouEcuType);
        if ( ouEcuType == eLIN_Master )
        {
            bMasterFound = true;
        }
        else if ( ouEcuType == eLIN_Slave )
        {
            bSlaveFound = true;
        }
    }

    if ( false == bMasterFound )
    {
        ParsingResults ouPasringResult;
        ouPasringResult.m_ouErrorType = eError;
        ouPasringResult.m_strActionTaken = "";
        ouPasringResult.m_strErrorDesc = "Master ECU should be defined in LDF File";
        ouPasringResult.m_unErrorCode = 1;
        ouPasringResult.m_unLineNum = 0;
        ouErrors.push_back(ouPasringResult);
    }
    if ( false == bSlaveFound )
    {
        ParsingResults ouPasringResult;
        ouPasringResult.m_ouErrorType = eError;
        ouPasringResult.m_strActionTaken = "";
        ouPasringResult.m_strErrorDesc = "Atleast One Slave should be Defined in LDF File";
        ouPasringResult.m_unErrorCode = 1;
        ouPasringResult.m_unLineNum = 0;
        ouErrors.push_back(ouPasringResult);
    }


    //2. UnCondition Frames
    bool bUnconditionalFrameFound = false;;
    pElement.clear();
    LDFDatabaseManager::GetDatabaseManager()->GetLDFCluster()->GetElementList(eFrameElement, pElement);
    for ( auto itr : pElement )
    {
        IFrame* pFrame = (IFrame*)itr.second;
        eFrameType ouFrameType;
        if ( nullptr == pFrame )
        {
            continue;
        }

        pFrame->GetFrameType(ouFrameType);
        if ( ouFrameType == eLIN_Unconditional )
        {
            bUnconditionalFrameFound = true;
            break;
        }
    }

    if ( false == bUnconditionalFrameFound )
    {
        ParsingResults ouPasringResult;
        ouPasringResult.m_ouErrorType = eWarning;
        ouPasringResult.m_strActionTaken = "";
        ouPasringResult.m_strErrorDesc = "Valid LDF File should Contain Atleast one Unconditional Frame";
        ouPasringResult.m_unErrorCode = 1;
        ouPasringResult.m_unLineNum = 0;
        ouWarnings.push_back(ouPasringResult);
    }

    //3. Signals
    bool SignalFound = false;
    pElement.clear();
    LDFDatabaseManager::GetDatabaseManager()->GetLDFCluster()->GetElementList(eSignalElement, pElement);
    for ( auto itr : pElement )
    {
        ISignal* pSignal = (ISignal*)itr.second;
        SignalProps ouSignalProps;
        if ( nullptr == pSignal )
        {
            continue;
        }

        pSignal->GetProperties(ouSignalProps);
        if ( ouSignalProps.m_ouLINSignalProps.m_ouSignalType == eSignalNormal )
        {
            SignalFound = true;
            break;
        }
    }

    if ( false == SignalFound )
    {
        ParsingResults ouPasringResult;
        ouPasringResult.m_ouErrorType = eWarning;
        ouPasringResult.m_strActionTaken = "";
        ouPasringResult.m_strErrorDesc = "Valid LDF File should Contain Atleast one signal";
        ouPasringResult.m_unErrorCode = 1;
        ouPasringResult.m_unLineNum = 0;
        ouWarnings.push_back(ouPasringResult);
    }

    //4. Tables
    bool TableFound = false;
    pElement.clear();
    LDFDatabaseManager::GetDatabaseManager()->GetLDFCluster()->GetElementList(eScheduleTableElement, pElement);
    for ( auto itr : pElement )
    {
        ScheduleTableProps ouTableProps;
        IScheduleTable* pTable = (IScheduleTable*)itr.second;

        if ( nullptr == pTable )
        {
            continue;
        }
        pTable->GetProperties(ouTableProps);

        if ( ouTableProps.m_ouCLINSheduleTableItem.size() > 0 )
        {
            TableFound = true;
            break;
        }
    }

    if ( false == TableFound )
    {
        ParsingResults ouPasringResult;
        ouPasringResult.m_ouErrorType = eWarning;
        ouPasringResult.m_strActionTaken = "";
        ouPasringResult.m_strErrorDesc = "Valid LDF File should Contain Atleast one Schedule Table";
        ouPasringResult.m_unErrorCode = 1;
        ouPasringResult.m_unLineNum = 0;
        ouWarnings.push_back(ouPasringResult);
    }
    return 0;
}
