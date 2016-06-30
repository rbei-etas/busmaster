#ifndef LDFEDITOR_H
#define LDFEDITOR_H

#include <QtWidgets/QMainWindow>
#include <QSplitter.h>
#include <QMenubar>
#include <LDFElementView.h>
#include <LDFNetworkView.h>
#include <LDFPropertyView.h>
#include <ui_LDFEditor.h>
#include <qevent.h>

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
    int onActionFileOpen(void);
    int onActionFileSave(void);
    int onActionFileSaveAs(void);
    int onActionAbout(void);
    int onActionHelp(void);
    int onActionLIN1_3(void);
    int onActionLIN2_0(void);
    int onActionLIN2_1(void);
    int onActionHex(void);
    int onActionOpenRecentFile();
    int onActionPreviewFile();
    void OnitemElementViewSelected();
    void OnitemNetworkViewSelected();
    void OnDocumentModified();
    int onFileExit();
};

#endif // LDFEDITOR_H
