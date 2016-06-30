#ifndef UNCONDITIONALFRAMEEDITDLG_H
#define UNCONDITIONALFRAMEEDITDLG_H

#include "qobject.h"
#include <QDialog>
#include "ui_UnconditionalFrameEditDlg.h"
#include <ICluster.h>
#include "Defines.h"

enum eFrameSignalColumns
{
    eColSignalName,
    eColStartBit,
    eColSubscriber,
    eColSignalLength,
    eColSignalInitValue,
    eColSignalCoding
};

class UnconditionalFrameEditDlg : public QDialog
{
    Q_OBJECT
private:
    IFrame** m_pouFrame;
    std::map<std::string, std::string> m_mapFrameNames;
    std::map<unsigned int, unsigned int> m_mapFrameIds;
    std::map<UID_ELEMENT, std::map<UID_ELEMENT, int>> m_mapECUToSignals;
    std::list<IEcu*> m_ouRxEcus;

    std::string m_omStrPublisher;
    std::string m_omFrameLength;
    unsigned int m_unFrameId;
    ICluster* m_pouLDFCluster;
    eMode m_eUIMode;
    bool m_bIsDynFrame;

public:
    UnconditionalFrameEditDlg(IFrame** pouFrame, eMode , QWidget* parent = 0);
    ~UnconditionalFrameEditDlg();
private slots:
    void onMapSignal(bool);
    void onSelectionOk();
    void onSelectionCancel();
    void onChangePublisher(QString);
    void onChangeFrameLength(QString);

private:
    void SetUpUi();
private:
    Ui::UnConditionalFrameEditDlg ui;

private:
    void PopulateFrameIdCombo();
    void PopulateFrameLengthCombo();
    void PopulateFrameSignals();
    void vPopulatePublishers();
    void vEnableDynamicFrame();
    void vAddSignalToTree(ISignal* pSignal, SignalInstanse ouSignalInstance);
    void vUpdateFrameSignalList(std::map<UID_ELEMENT, SignalInstanse>& mapSelectedSignals);
    void vExistingFrameDetails(unsigned int);
    ERRORCODE nValidateValues();
    void vAddSubscribers(std::list<IEcu*> lstRxECUs);
    void vUpdateNewFrameDetails();
    void vUpdateEditFrameDetails();
    void vUpdateDynamicFrameDetails();
    int bIsDynamicFrame(unsigned int unFrameId);
};

#endif // UNCONDITIONALFRAMEEDITDLG_H
