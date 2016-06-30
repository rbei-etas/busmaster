#ifndef NodeConfigurationDlg_H
#define NodeConfigurationDlg_H

#include <QDialog>
#include "ui_NodeConfiguration.h"
#include "Defines.h"
#include "ICluster.h"
#include <QMenu>

enum eNodeConfigMenu
{
    eAddNodeConfig,
    eDeleteNodeConfig,
    eNodeCompTotal
};

class NodeConfigurationDlg : public QDialog
{
    Q_OBJECT
private:
    ICluster* m_pLdfCluster;

private slots:
    void onSelectionOk();
    void onButtonClickAdd(bool);
    void onButtonClickDelete(bool);
    void onEditSelectionChange(int);
    void onSelecetionCancel();

signals:
    void OnEditFocused();
public:
    NodeConfigurationDlg(INodeConfiguration** pNodeConfig,eMode ouMode, QWidget* parent);
    ~NodeConfigurationDlg();

private:
    Ui::NodeConfiguration ui;
    QMenu m_omMenuConfig[eNodeCompTotal];
    eMode m_ouMode;
    INodeConfiguration** m_pouNodeConfig;
    std::map<std::string, std::string> m_mapNodeNames;
    std::map<std::string, std::string> m_mapCompositeNodes;
    std::map<std::string, std::string> m_logicalNodes;

private:
    void vPrepareUI();
    void vPrepareUIForNew();
    void vPrepareUIForEdit();
    void vPopulateEcusCombo(QComboBox** pComboBox);
    ERRORCODE nValidateNodes(std::string&);
    void vCollectNodeNames();
};

#endif // NodeConfigurationDlg_H
