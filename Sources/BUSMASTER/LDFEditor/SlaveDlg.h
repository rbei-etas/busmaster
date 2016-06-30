#ifndef SLAVEDLG_H
#define SLAVEDLG_H

#include <QDialog>
#include "ui_SlaveDlg.h"
#include "ICluster.h"
class SlaveDlg : public QDialog
{
    Q_OBJECT
private:
    ICluster* m_pBaseCluster;
    IEcu** m_pouEcu;
    bool m_bEcuInfoRead;

    std::map<unsigned int, unsigned int> m_MapunConfigId;
    std::map<std::string, std::string> m_MapNames;
    bool m_bModeEdit;
public:
    SlaveDlg(ICluster* ouCluster, IEcu** pouEcu, bool bNew = true, QWidget* parent = 0);
    ~SlaveDlg();

public slots:
    void onProtocolSelectionChanged(int);
    void onSelectionOk();
    void onSelecetionCancel();
    void onEditFaultSignals();
private:
    void vPrepareForNewSlave();
    void vPrepareForEditEcu();
    void vPrepareUiForVer13();
    void vPrepareUiForVer20();
    void vPrepareUiForVer21();

    void vPrepareValidations();
    void vPrepareDefaultValues();
    void vInitialiseProtocolComboBox(double dEcuVer = 0.0);
    void vPopulateResposeErrorCombo(bool bEdited = false);
    int nValidateValue();
    int nFillFaultStateSignals();

    Ui::SlaveDlg ui;
    std::list<UID_ELEMENT> m_lstFaultSignals;
};

#endif // SLAVEDLG_H
