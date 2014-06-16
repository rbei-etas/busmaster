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

class CPanelAndNode : public CDialog
{
    DECLARE_DYNAMIC(CPanelAndNode)

public:
    CPanelAndNode(CWnd* pParent/* = NULL*/, CMainPanel* pMP);   // standard constructor
    virtual ~CPanelAndNode();

    CMainPanel* MPanel;
    BOOL flagEnvio;
    unsigned int currentTime;
    unsigned int initialTimeInScl;
    unsigned int initialTimeKombi;
    unsigned int initialTimeOpVeh;


    void CPanelAndNode::InitTimers( unsigned int curTime );
    void CPanelAndNode::TimersCheck( unsigned int curTime );


    CRadixEdit editIndicatorsPeriod;
    int valueIndicatorsPeriod;
    CRadixEdit editInHornMotbk2010;
    int valueInHornMotbk2010;
    CRadixEdit editInIdcLhMotbk2010;
    int valueInIdcLhMotbk2010;
    CRadixEdit editInIdcRhMotbk2010;
    int valueInIdcRhMotbk2010;
    CRadixEdit editInIdcRstgMotbk2010;
    int valueInIdcRstgMotbk2010;
    CRadixEdit editInMabMotbk2010;
    int valueInMabMotbk2010;
    CRadixEdit editInPubuLhMotbk2010;
    int valueInPubuLhMotbk2010;
    CRadixEdit editInPubuRhMotbk2010;
    int valueInPubuRhMotbk2010;
    CRadixEdit editInRoswLhDwnMotbk2010;
    int valueInRoswLhDwnMotbk2010;
    CRadixEdit editInRoswLhUpMotbk2010;
    int valueInRoswLhUpMotbk2010;
    CRadixEdit editInRoswRhDwnMotbk2010;
    int valueInRoswRhDwnMotbk2010;
    CRadixEdit editInRoswRhUpMotbk2010;
    int valueInRoswRhUpMotbk2010;
    CRadixEdit editCtrChovRossMotbk2010;
    int valueCtrChovRossMotbk2010;
    CRadixEdit editCtrHghtMotbk2010;
    int valueCtrHghtMotbk2010;
    CRadixEdit editSuDrlAutomlyMotbk2010;
    int valueSuDrlAutomlyMotbk2010;
    CRadixEdit editCtrShtrFsMotbk2010;
    int valueCtrShtrFsMotbk2010;
    CRadixEdit editBrigMotbk2010;
    int valueBrigMotbk2010;
    CRadixEdit editMileTotMotbk2010;
    int valueMileTotMotbk2010;
    CRadixEdit editTSecCouRelMotbk2010;
    int valueTSecCouRelMotbk2010;

    CButton buttonWindshieldUp;
    BOOL flagWindshieldUp;
    CButton buttonWindshield;
    BOOL flagWindshieldDown;
    CButton buttonIndicators;
    BOOL flagIndicators;
    CButton buttonInHornMotbk2010;
    BOOL flagInHornMotbk2010;
    CButton buttonInIdcLhMotbk2010;
    BOOL flagInIdcLhMotbk2010;
    CButton buttonInIdcRhMotbk2010;
    BOOL flagInIdcRhMotbk2010;
    CButton buttonInIdcRstgMotbk2010;
    BOOL flagInIdcRstgMotbk2010;
    CButton buttonInMabMotbk2010;
    BOOL flagInMabMotbk2010;
    CButton buttonInPubuLhMotbk2010;
    BOOL flagInPubuLhMotbk2010;
    CButton buttonInPubuRhMotbk2010;
    BOOL flagInPubuRhMotbk2010;
    CButton buttonInRoswLhDwnMotbk2010;
    BOOL flagInRoswLhDwnMotbk2010;
    CButton buttonInRoswLhUpMotbk2010;
    BOOL flagInRoswLhUpMotbk2010;

    CButton buttonInRoswRhDwnMotbk2010;
    BOOL flagInRoswRhDwnMotbk2010;
    CButton buttonInRoswRhUpMotbk2010;
    BOOL flagInRoswRhUpMotbk2010;

    /*  CProgressCtrl progressWindshieldProgress;
        int valueWindshieldProgress;
        */

    //   afx_msg void OnTimer(UINT_PTR nIDEvent);


    /**
    * This function is called by the framework when the user selects an option in the
    * interface combo box.
    */

    afx_msg  void CPanelAndNode::OnCloseWnd();

    void CPanelAndNode::EvaluateMessageReceived(STCAN_MSG  Mensaje);
    void CPanelAndNode::PrepareData();
    void CPanelAndNode::UpdateAlive();
    // Dialog Data
    enum { IDD = IDD_Panel_Kombi };




    //  public:
    //UINT CPanelAndNode::StartTimer();
    //void CPanelAndNode::StopTimer();
    void CPanelAndNode::OnTimerInSwcl();
    void CPanelAndNode::OnTimerKmbi();
    void CPanelAndNode::OnTimerOpVeh();
    //  void CPanelAndNode::PrepareData();






protected:
    virtual BOOL OnInitDialog();
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    void vInitializePanelFields();
    DECLARE_MESSAGE_MAP()

private:
    //HANDLE timer;


    void CPanelAndNode::SetPanelValues();

    void CPanelAndNode::OnButtonWindshieldUp();
    void CPanelAndNode::OnButtonWindshieldDown();
    void CPanelAndNode::OnButtonIndicators();
    void CPanelAndNode::OnButtonInHornMotbk2010();
    void CPanelAndNode::OnButtonInIdcLhMotbk2010();
    void CPanelAndNode::OnButtonInIdcRhMotbk2010();
    void CPanelAndNode::OnButtonInIdcRstgMotbk2010();
    void CPanelAndNode::OnButtonInMabMotbk2010();
    void CPanelAndNode::OnButtonInPubuLhMotbk2010();
    void CPanelAndNode::OnButtonInPubuRhMotbk2010();
    void CPanelAndNode::OnButtonInRoswLhDwnMotbk2010();
    void CPanelAndNode::OnButtonInRoswLhUpMotbk2010();
    void CPanelAndNode::OnButtonInRoswRhDwnMotbk2010();
    void CPanelAndNode::OnButtonInRoswRhUpMotbk2010();
public:
    afx_msg void OnEnChangeCtrHghtMotbk2010();
};
