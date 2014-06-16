#pragma once
#include "afxwin.h"
#include "UDS_Resource.h"
#include "UDSWnd_Defines.h"
//#include "Panel.h"
//#include "Node.h"
#include "UDSMainWnd.h"
#include "DIL_Interface/BaseDIL_CAN.h"
#include "DIL_Interface/DIL_Interface_extern.h"


//#include "MainPanel.h"
//#include "Panel.h"
//#include "PanelandNode.h"
#include "UDS_Protocol.h"



//#include "UDS_Resource.h"
//#include "DIL_Interface/BaseDIL_CAN.h"
//#include "DIL_Interface/DIL_Interface_extern.h"


extern bool FStart;
extern bool FKombi;
extern bool FKombiInit;
extern bool FAbs;
extern bool FAbsInit;


class CMainPanel :  public CDialog
{
    DECLARE_DYNAMIC(CMainPanel) ;

public:
    CMainPanel(CWnd* pParent =NULL/*, int TotalChannels=0*/);
    ~CMainPanel(void);


    //MainPanel  Variables
    //CPanelAndNode* PanelAndNode1;
    CUDS_Protocol* UDSP;
    CUDSMainWnd* UDSMW;
    CButton m_omButtonToAbsPanel;
    CButton m_omButtonToDmePanel;
    CButton m_omButtonToKombiPanel;
    CButton m_omCheckAbsNode;
    CButton m_omCheckDmeNode;
    CButton m_omCheckKombiNode;
    BOOL flagEnvio;

    //  bool FKombi;
    //  bool FKombiInit;

    unsigned int currentTime;
    unsigned int initialTimeSpeed;
    unsigned int initialTimeRel;
    //CPanel* Panel1;
    //CNode* Node1;

    static CMainPanel* m_spodInstance;
    mPSTXSELMSGDATA psTxCanMsgMainPanel;
    //static CBaseDIL_CAN* Panel_DIL_CAN_Interface;


    //MainPanel  Methods
    void CMainPanel::EvaluatePanelsMessage(STCAN_MSG  Mensaje );
    void CMainPanel::SendPanelsMessage(mPSTXSELMSGDATA sMessage);
    //USAGEMODE HRESULT EvaluateMessageMainPanel( STCAN_MSG  Mensaje );
    //CPanelAndNode* PanelAndNode1;
    enum { IDD = IDD_MainPanel};


    UINT CMainPanel::StartTimer();
    void CMainPanel::StopTimer();

    unsigned int CMainPanel::nCalculateCurrTime(BOOL bFromDIL);
    void vInitializeMainPanel();
    DECLARE_MESSAGE_MAP()

protected:

    void CMainPanel::OnCheckAbsNode();
    void CMainPanel::OnCheckDmeNode();
    void CMainPanel::OnCheckKombiNode();

    //  UINT CMainPanel::StartTimer();
    BOOL CMainPanel::OnInitDialog();
    static UINT OnSendSelectedMsg(LPVOID pParam);
    void CMainPanel::OnBnClickedOpenAbsPanel();
    void CMainPanel::OnBnClickedOpenKombiPanel();
    void CMainPanel::OnBnClickedOpenDmePanel();
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

public:
    afx_msg void OnBnClickedButtonToDme();
};
