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

class CPanelAbs : public CDialog
{
    DECLARE_DYNAMIC(CPanelAbs)

public:
    CPanelAbs(CWnd* pParent/* = NULL*/, CMainPanel* pMP);   // standard constructor
    virtual ~CPanelAbs();

    CMainPanel* MPanel;
    BOOL flagEnvio;
    unsigned int currentTime;
    unsigned int initialTimeRel;
    unsigned int initialTimeSpeed;


    void CPanelAbs::InitTimers( unsigned int curTime );
    void CPanelAbs::TimersCheck( unsigned int curTime );

    /** Associated to the Diagnostic Standard ComboBox */
    CComboBox  m_omStdDiag;
    /** Associated to CAN ID (Resq from ECU) */
    CRadixEdit m_omReqCanID;
    /** Associated to CAN ID (Resq from ECU) */
    CString StringReqCanID;


    CMainPanel* MP;

    CRadixEdit editMileCouFtMotbk2010;
    int valueMileCouFtMotbk2010;
    CRadixEdit editMileCouRrMotbk2010;
    int valueMileCouRrMotbk2010;
    CRadixEdit editCrcVMotbk2010;
    int valueCrcVMotbk2010;
    CRadixEdit editAlivVMotbk2010;
    int valueAlivVMotbk2010;
    CRadixEdit editModVehAbsAcvMotbk2010;
    int valueModVehAbsAcvMotbk2010;
    CRadixEdit editModVehAbsChovMotbk2010;
    int valueModVehAbsChovMotbk2010;
    CRadixEdit editStIlkTltMotbk2010;
    int valueStIlkTltMotbk2010;
    CRadixEdit editStModTltMotbk2010;
    int valueStModTltMotbk2010;
    CRadixEdit editStSenVFtMotbk2010;
    int valueStSenVFtMotbk2010;
    CRadixEdit editStSenVRrMotbk2010;
    int valueStSenVRrMotbk2010;
    CRadixEdit editStStassMotbk2010;
    int valueStStassMotbk2010;
    CRadixEdit editVWhlFtMotbk2010;
    int valueVWhlFtMotbk2010;
    CRadixEdit editVWhlRrMotbk2010;
    int valueVWhlRrMotbk2010;
    CRadixEdit editStAbsMotbk2010;
    int valueStAbsMotbk2010;
    CRadixEdit editStBrkFtMotbk2010;
    int valueStBrkFtMotbk2010;
    CRadixEdit editStBrkRrMotbk2010;
    int valueStBrkRrMotbk2010;
    CButton checkAlivVMotbk2010;
    BOOL flagAlivVMotbk2010;
    CButton checkCrcVMotbk2010;
    BOOL flagCrcVMotbk2010;
    CButton checkStBrkFtMotbk2010;
    BOOL flagStBrkFtMotbk2010;
    CButton checkStBrkRrMotbk2010;
    BOOL flagStBrkRrMotbk2010;


    /*  CProgressCtrl progressWindshieldProgress;
        int valueWindshieldProgress;
        */


    void CPanelAbs::OnButtonPressedOK();

    void CPanelAbs::OnButtonPressedCancel();


    /**
    * This function is called by the framework when the user selects an option in the
    * interface combo box.
    */

    afx_msg  void CPanelAbs::OnCloseWnd();

    void CPanelAbs::EvaluateMessageReceived(STCAN_MSG  Mensaje);
    void CPanelAbs::PrepareData();
    void CPanelAbs::UpdateAlive();
    // Dialog Data
    enum { IDD = IDD_Panel };




    //  public:
    //UINT CPanelAbs::StartTimer();
    //void CPanelAbs::StopTimer();
    void CPanelAbs::OnTimerRel();
    void CPanelAbs::OnTimerSpeed();
    //  void CPanelAbs::PrepareData();


    void CPanelAbs::OnCheckAlivVMotbk2010();
    void CPanelAbs::OnCheckCrcVMotbk2010();
    void CPanelAbs::OnCheckStBrkFtMotbk2010();
    void CPanelAbs::OnCheckStBrkRrMotbk2010();



protected:
    virtual BOOL OnInitDialog();
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    void vInitializePanelFields();
    DECLARE_MESSAGE_MAP()

private:
    //HANDLE timer;

    void CPanelAbs::SetPanelValues();

};
