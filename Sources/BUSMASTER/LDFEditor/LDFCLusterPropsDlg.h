#ifndef LDFCLUSTERPROPSDLG_H
#define LDFCLUSTERPROPSDLG_H

#include <QDialog>
#include "ui_LDFCLusterPropsDlg.h"
#include "ICluster.h"
class LDFCLusterPropsDlg : public QDialog
{
    Q_OBJECT
private:
    ICluster* m_pCluster;
public:
    LDFCLusterPropsDlg(ICluster* pCluster, QWidget* parent = 0);
    ~LDFCLusterPropsDlg();
private:
    void vPrepareUi();
private slots:
    void OnSelectOk();
private:
    Ui::LDFCLusterPropsDlg ui;
};

#endif // LDFCLUSTERPROPSDLG_H
