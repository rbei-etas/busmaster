#ifndef LDFEDITOR_H
#define LDFEDITOR_H

#include <QtWidgets/QMainWindow>
#include <QSplitter>
#include <QMenuBar>
#include <QEvent>
#include <QCloseEvent>

#include <ui_LDFEditor.h>

#include "LDFElementView.h"
#include "LDFNetworkView.h"
#include "LDFPropertyView.h"

class LDFEditor : public QMainWindow
{
    Q_OBJECT
private:
    Ui::LDFEditor ui;
    QString m_strCurrentLDFFilePath;

    //ICluster* m_pouLdfCluster;
    QAction* recentFileActs[5];
    LDFCommonTableWidget* m_pWarningTable;
private:
    int ConnectMenuActions(void);
    int UpdatedRecentFileList();
    int OpenLDFFile(QString strFile);
    int OnFileLoad(QString strFile);
    int CloseCurrentLDFFile();
    int CheckForSaving();
    void setTitle();
    void SetupWarningPane();
    void AddWarningsOfNewFile(double dVers);
    int ValidateForSaving();

    int nValidateForCluster(std::list<ParsingResults>& ouErrors, std::list<ParsingResults>& ouWarnings);

public:
    LDFEditor(std::string omstrLdfPath, QWidget* parent = 0);
    ~LDFEditor();
public:
    void closeEvent (QCloseEvent* event);

public slots:
    int on_actionOpen_triggered(void);
    int on_actionSave_triggered(void);
    int on_actionSave_As_triggered(void);
    int on_action_About_triggered(void);
    int on_actionHelp_triggered(void);
    int on_actionNewLIN_1_3_triggered(void);
    int on_actionNewLIN_2_0_triggered(void);
    int on_actionNewLIN_2_1_triggered(void);
    int on_actionHex_triggered(void);
    int on_actionRecent_triggered();
    int on_actionPreview_LDF_File_triggered();
    void on_m_pouLDFElementView_itemSelectionChanged();
    void on_m_pouLDFNetworkView_itemSelectionChanged();
    int on_actionExit_triggered();

private slots:
    void OnDocumentModified();
};

#endif // LDFEDITOR_H
