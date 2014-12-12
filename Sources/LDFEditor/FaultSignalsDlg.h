#ifndef FAULTSIGNALSDLG_H
#define FAULTSIGNALSDLG_H

#include <QDialog>
#include "ui_FaultSignalsDlg.h"
#include "LDFCluster.h"
class FaultSignalsDlg : public QDialog
{
    Q_OBJECT
private:
    IEcu* m_pEcu;
public:
    FaultSignalsDlg(IEcu* pouEcu, double fSlaveProtVer, std::list<UID_ELEMENT>& lstSignals, QWidget* parent = 0);
    ~FaultSignalsDlg();

    void vGetSignalList(std::list<UID_ELEMENT>& lstSignals);
private:
    void vPrepareUi();

private slots:
    void OnselectedOk();
private:
    Ui::FaultSignalsDlg ui;
    std::list<UID_ELEMENT> m_lstSignals;
    double m_fSlaveProtVersion;
};

#endif // FAULTSIGNALSDLG_H
