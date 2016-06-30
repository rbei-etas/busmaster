#pragma once
#include <QDialog>
#include "ui_EventFrameEditDlg.h"
#include "ICluster.h"
#include "LDFUtility.h"

class EventFrameEditDlg :public QDialog
{
    Q_OBJECT
public:
    EventFrameEditDlg(ICluster* pCluster, IFrame** pouFrame, bool bNew, QWidget* parent = 0);
    ~EventFrameEditDlg(void);

private:
    IFrame** m_pouFrame;
    ICluster* m_pLdfCluster;
    bool m_bEditMode;
    std::map<unsigned int, unsigned int> m_FrameIdMap;
    std::map<std::string, std::string> m_strFrameNames;
    std::map<std::string, UID_ELEMENT> m_strTableMap;
private:
    void PopulateFrameIdCombo();
    void EnableElementsByVersion();
    void vUpdateFrameList(std::list<UID_ELEMENT> lstFrames);

    void vAddFrameToTable(IFrame* pFrame, std::map<UID_ELEMENT, UID_ELEMENT> ouSelectedElements);

private:
    Ui::EventFrameEditDlg ui;
    void vPrepareValidations();
    void vPrepareUiForEditMode();
    void vPrepareUiForNewMode();
    void vFillElementList();
    void vFillScheduleTables();
    int nValidateValues();
private slots:
    void OnButtonClickOk();
    //void onMapFrame(bool);
};
