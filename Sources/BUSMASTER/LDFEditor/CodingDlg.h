#ifndef CODINGDLG_H
#define CODINGDLG_H

#include <QDialog>
#include "ui_CodingDlg.h"
#include "ICluster.h"

class CodingDlg : public QDialog
{
    Q_OBJECT
private:
    ICluster* m_pLdfCluster;
    bool m_bEditMode;
    ICoding** m_pCoding;
    std::map<std::string, UID_ELEMENT> m_strCodingMap;
public:
    CodingDlg(ICluster* pouCluster, ICoding** pouCoding, bool bNew, QWidget* parent = nullptr);
    ~CodingDlg();
private:
    void vPrepareValidations();
    void vPrepareUiForEdit();
private slots:
    void AddLogicalRow();
    void AddPhysicalRow();
    void DeleteLogicalRow();
    void DeletePhysicalRow();
    void onSelectionOk();
    void onSelecetionCancel();
    void ItemSelectionChangeLogical();
    void FillLogicalValues(CompuMethodProps& ouCompuProps);
    int FillPhysicalValues(CompuMethodProps& ouCompuProps);
private:
    Ui::CodingDlg ui;
};

#endif // CODINGDLG_H
