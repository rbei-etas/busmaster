#ifndef MASTEREDITDLG_H
#define MASTEREDITDLG_H

#include <QDialog>
#include "ui_MasterEditDlg.h"
#include <ICluster.h>

class MasterEditDlg : public QDialog
{
    Q_OBJECT
private:
    IEcu* m_pouMasterEcu;
public:
    MasterEditDlg(IEcu* pouMasterEcu, QWidget* parent = 0);
    ~MasterEditDlg();
private:
    void SetUpUi();
private:
    Ui::Dialog ui;
public slots:
    void OnOk();
    void OnCancel();

};

#endif // MASTEREDITDLG_H
