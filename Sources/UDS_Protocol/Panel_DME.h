#pragma once


#include "UDS_Resource.h"
#include "UDS_Protocol.h"
#include "Utility/RadixEdit.h"
#include "DataTypes/UDS_DataTypes.h"


#include "stdafx.h"
//#include "MainPanel.h"
//#include "Node.h"
//#include "Panel.h"
#include "UDSMainWnd.h"
//#include "UDS_Protocol.h"
//#include "DataTypes/UDS_DataTypes.h"
#include "UDSWnd_Defines.h"
#include <Windows.h>




//extern UINT_PTR m_nTimer;
// CUDSSettingsWnd dialog

class CPanelDme : public CDialog
{
    DECLARE_DYNAMIC(CPanelDme)

public:
    CPanelDme(CWnd* pParent/* = NULL*/, CMainPanel* pMP);   // standard constructor
    virtual ~CPanelDme();

    CMainPanel* MPnel;
    BOOL flagEnvio;
    unsigned int initialTimeEngDat2;
    unsigned int initialTimeEngDat1;
    unsigned int initialTimeModVeh;
    unsigned int initialTimeClmpStat;
    unsigned int initialTimeDtDrv;
    unsigned int initialTimeDispDrv;
    unsigned int currentTime;



    void CPanelDme::InitTimers( unsigned int curTime );
    void CPanelDme::TimersCheck( unsigned int curTime );




    CRadixEdit editStKl15Motbk2010;
    int valueStKl15Motbk2010;
    CRadixEdit editIjvFuMotbk2010;
    int valueIjvFuMotbk2010;
    CRadixEdit editTempEngMotbk2010;
    int valueTempEngMotbk2010;
    CRadixEdit editStEmoffMotbk2010;
    int valueStEmoffMotbk2010;
    CRadixEdit editStKl50Motbk2010;
    int valueStKl50Motbk2010;
    CRadixEdit editStSdstMotbk2010;
    int valueStSdstMotbk2010;
    CRadixEdit editRpmEngMotbk2010;
    int valueRpmEngMotbk2010;
    CRadixEdit editBnkAngMotbk2010;
    int valueBnkAngMotbk2010;
    CRadixEdit editAlivDispDrvMotbk2010;
    int valueAlivDispDrvMotbk2010;
    CRadixEdit editCrcDispDrvMotbk2010;
    int valueCrcDispDrvMotbk2010;
    CRadixEdit editDispRdiDrvgMotbk2010;
    int valueDispRdiDrvgMotbk2010;
    CRadixEdit editAlivDtDrvMotbk2010;
    int valueAlivDtDrvMotbk2010;
    CRadixEdit editCrcDtDrvMotbk2010;
    int valueCrcDtDrvMotbk2010;
    CRadixEdit editStBrgRrMotbk2010;
    int valueStBrgRrMotbk2010;
    CRadixEdit editDispModVehAcvMotbk2010;
    int valueDispModVehAcvMotbk2010;

    CRadixEdit editKlShow;
    int valueKlShow;
    CButton buttonKlChanger;
    int valueKlChanger;

    CButton buttonStEmoffMotbk2010;
    BOOL flagStEmoffMotbk2010;
    CButton buttonStKl50Motbk2010;
    BOOL flagStKl50Motbk2010;
    CButton buttonStSdstMotbk2010;
    BOOL flagStSdstMotbk2010;
    CButton checkAlivDispDrvMotbk2010;
    BOOL flagAlivDispDrvMotbk2010;
    CButton checkCrcDispDrvMotbk2010;
    BOOL flagCrcDispDrvMotbk2010;
    CButton checkAlivDtDrvMotbk2010;
    BOOL flagAlivDtDrvMotbk2010;
    CButton checkCrcDtDrvMotbk2010;
    BOOL flagCrcDtDrvMotbk2010;


    CSliderCtrl sliderRpm;
    int valueSliderRpm;



    /**
    * This function is called by the framework when the user selects an option in the
    * interface combo box.
    */

    afx_msg  void CPanelDme::OnCloseWnd();

    void CPanelDme::EvaluateMessageReceived(STCAN_MSG  Mensaje);
    void CPanelDme::PrepareData();
    //void CPanelDme::UpdateAlive();
    // Dialog Data
    enum { IDD = IDD_Panel_Dme };

protected:
    virtual BOOL OnInitDialog();
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    void vInitializePanelFields();
    DECLARE_MESSAGE_MAP()
public:
    //UINT CPanelDme::StartTimer();
    //void CPanelDme::StopTimer();
    void CPanelDme::OnTimerEngDat2();
    void CPanelDme::OnTimerEngDat1();
    void CPanelDme::OnTimerModVeh();
    void CPanelDme::OnTimerClmpStat();
    void CPanelDme::OnTimerDtDrv();
    void CPanelDme::OnTimerDispDrv();

private:
    //HANDLE timer;
    //void CPanelDme::PrepareData();

    void CPanelDme::SetPanelValues();

    void CPanelDme::OnButtonStEmoffMotbk2010();
    void CPanelDme::OnButtonStKl50Motbk2010();
    void CPanelDme::OnButtonStSdstMotbk2010();
    void CPanelDme::OnButtonKlChanger();
    void CPanelDme::OnCheckAlivDispDrvMotbk2010();
    void CPanelDme::OnCheckCrcDispDrvMotbk2010();
    void CPanelDme::OnCheckAlivDtDrvMotbk2010();
    void CPanelDme::OnCheckCrcDtDrvMotbk2010();
    //void CPanelDme::OnEnChangeRpmEng();
    //void CPanelDme::OnEnChangeRpmSlider();

    void CPanelDme::UpdateAliveDt();
    void CPanelDme::UpdateAliveDisp();
    void CPanelDme::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);

    /*public:
        afx_msg void OnNMCustomdrawSliderRpm(NMHDR *pNMHDR, LRESULT *pResult);*/
};
