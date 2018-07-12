#pragma once
#include <QDialog>
#include "ui_MapSignalsDlg.h"
#include "ICluster.h"
#include "qmessagebox.h"
#include "qevent.h"

#define defSIGNAL_STARTBIT_INVALID ": Invalid Signal Start bit "
#define defERR_SIGNAL_STARTBIT     " Signal Start bit ["
#define defERR_SIGNAL_OFFSET              " Signal offset ["

#define defERR_FRAME_LENGTH        "] exceeds Frame length ["
#define defERROR                   "Error: "
#define defBYTES                   "] byte(s)"
#define defSIGNAL                  "Signal "
#define defSIGNAL_OVERLAPPING      " is overallapping with other Signal(s)"
#define defERR_EMPTY_STARTBIT      " Signal Start bit Empty"

class MapSignalsDlg :public QDialog
{
    Q_OBJECT
public:
    MapSignalsDlg(UID_ELEMENT uidECU,int nFrameLen, std::map<UID_ELEMENT, SignalInstanse> mapSignals, QWidget* parent = 0);
    ~MapSignalsDlg(void);

private:
    IEcu* m_pouEcu;
    IFrame* m_pouFrame;
    std::map<UID_ELEMENT, SignalInstanse> m_mapSignalUIDSigInst;
    bool m_bIsError;
    int m_nFrameLen;

private:
    void SetUpUi();
    ERRORCODE vCollectSelectedSignals(std::string& ouErrorString);
    ERRORCODE vValidateSignalStartBit(UID_ELEMENT uidSignal,int, QString strSB, std::string& ouErrorString);
    ERRORCODE nValidateSignalOverlapping(int , std::string& );

private:
    Ui::MapSignalsDlg ui;
private slots:
    void onSelectionOk();
    void onSelectionCancel();

public:
    void vGetSelectedSignals(std::map<UID_ELEMENT, SignalInstanse>& mapSignals);
};

