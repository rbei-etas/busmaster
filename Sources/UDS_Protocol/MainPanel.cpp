#include "MainPanel.h"
#include "Panel.h"
#include "Node.h"
#include "PanelAndNode.h"
#include "Panel_DME.h"
#include "PanelAbs.h"
//#include "UDS_Protocol.h"

//#include "UDS_Resource.h"
//#include "DIL_Interface/BaseDIL_CAN.h"
//#include "DIL_Interface/DIL_Interface_extern.h"

#define TRUE  1
#define FALSE  0




static DWORD g_dwClientID = 0;
static CBaseDIL_CAN* Panel_DIL_CAN_Interface;
CUDS_Protocol* UDSP;
CMainPanel* CMainPanel::m_spodInstance = NULL;
//CUDS_Protocol* UDSP;
CUDSMainWnd* UDSMW;
CPanelAndNode* PanelAndNode1;
CPanelDme* PanelDme;
CPanelAbs* PanelAbs;
CMainPanel* MaiPai;


HANDLE timer;
BOOL flagEnvio;


bool FKombi;
bool FKombiInit;
bool FAbs;
bool FAbsInit;
bool FDme;
bool FDmeInit;



mPSTXSELMSGDATA psTxCanMsgUds  = new mSTXSELMSGDATA;



IMPLEMENT_DYNAMIC(CMainPanel, CDialog)

CMainPanel::CMainPanel( CWnd* pParent)
    :CDialog(CMainPanel::IDD, pParent)
{
    //Init
    vInitializeMainPanel();
    MaiPai=this;
    StartTimer();
    FKombi=FALSE;
    FKombiInit=FALSE;
    FAbs=FALSE;
    FAbsInit=FALSE;
}

CMainPanel::~CMainPanel(void)
{
}


BEGIN_MESSAGE_MAP(CMainPanel, CDialog)
    ON_BN_CLICKED(IDC_BUTTON_TO_ABS, OnBnClickedOpenAbsPanel)
    ON_BN_CLICKED(IDC_BUTTON_TO_KOMBI, OnBnClickedOpenKombiPanel)
    ON_BN_CLICKED(IDC_BUTTON_TO_DME, OnBnClickedOpenDmePanel)
    ON_BN_CLICKED(IDC_CHECK_ABS, OnCheckAbsNode)
    ON_BN_CLICKED(IDC_CHECK_DME, OnCheckDmeNode)
    ON_BN_CLICKED(IDC_CHECK_KOMBI, OnCheckKombiNode)

END_MESSAGE_MAP()



BOOL CMainPanel::OnInitDialog()
{
    CDialog::OnInitDialog();

    if ( UDSP == NULL)
    {
        UDSP = new CUDS_Protocol();
    }

    if ( UDSMW == NULL)
    {
        UDSMW = new CUDSMainWnd(UDSP->SourceAddress, UDSP->TargetAddress, UDSP->fInterface,UDSP->MsgID,this);
    }


    //vSetDILInterfacePtr();
    //NodeAux=new CNode();
    return TRUE;
}


void CMainPanel::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_BUTTON_TO_ABS, m_omButtonToAbsPanel);
    DDX_Control(pDX, IDC_BUTTON_TO_DME, m_omButtonToDmePanel);
    DDX_Control(pDX, IDC_BUTTON_TO_KOMBI, m_omButtonToKombiPanel);
    DDX_Control(pDX, IDC_CHECK_ABS, m_omCheckAbsNode);
    DDX_Control(pDX, IDC_CHECK_DME, m_omCheckDmeNode);
    DDX_Control(pDX, IDC_CHECK_KOMBI, m_omCheckKombiNode);
}


void CMainPanel::vInitializeMainPanel()
{


    DIL_GetInterface(CAN, (void**)&Panel_DIL_CAN_Interface);
    Panel_DIL_CAN_Interface->DILC_RegisterClient(TRUE, g_dwClientID, _T("CAN_MONITOR"));
}


void CMainPanel::EvaluatePanelsMessage(STCAN_MSG  Mensaje )
{
    PanelAbs->EvaluateMessageReceived(Mensaje);
    PanelAndNode1->EvaluateMessageReceived(Mensaje);
    PanelDme->EvaluateMessageReceived(Mensaje);
}


/***********************************************************************************************************/
void CMainPanel::SendPanelsMessage(mPSTXSELMSGDATA sMessage)
{

    //CUDS_NegRespMng UDSNRM= new CUDS_NegRespMng(omManagerPtr);

    //DIL_GetInterface(CAN, (void**)&g_pouDIL_CAN_Interface);
    //g_pouDIL_CAN_Interface->DILC_RegisterClient(TRUE, g_dwClientID, _T("CAN_MONITOR"));


    int nReturn = Panel_DIL_CAN_Interface->DILC_SendMsg(g_dwClientID, sMessage->m_psTxMsg[0]);


    //UDSMW->SendSimpleDiagnosticMessagePanels(sMessage);
}

/***********************************************************************************************************/


void CMainPanel::OnBnClickedOpenAbsPanel()
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());
    INT_PTR nRet = -1;
    CWnd objParent;
    //objParent.Attach(this);
    //CMainPanel* MPAux = this;

    if ( PanelAbs == NULL)
    {
        PanelAbs = new CPanelAbs(&objParent, this);
        //omMainPanel->TotalChannel = TotalChannels;
        PanelAbs->Create(IDD_Panel);
        //NegRespManager = new CUDS_NegRespMng(omManagerPtr);
        FAbs = TRUE;
        FAbsInit = FALSE;
        m_omCheckAbsNode.SetCheck(1);
    }

    PanelAbs->ShowWindow(SW_SHOW);

    //CWinThread* pomThread = NULL ;
    //pomThread = AfxBeginThread( Node1->ContinousMessagesSend);
    //objParent.Detach();
    //return 0;
    //Node1->ContinousMessagesSend();

    //Panel1 = new CPanel();

}


/***********************************************************************************************************/


void CMainPanel::OnBnClickedOpenKombiPanel()
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());
    INT_PTR nRet = -1;
    CWnd objParent;
    //objParent.Attach(this);
    //CMainPanel* MPAux = this;

    if ( PanelAndNode1 == NULL)
    {
        PanelAndNode1 = new CPanelAndNode(&objParent, this);
        //omMainPanel->TotalChannel = TotalChannels;
        PanelAndNode1->Create(IDD_Panel_Kombi);
        //NegRespManager = new CUDS_NegRespMng(omManagerPtr);
        FKombi = TRUE;
        FKombiInit = FALSE;
        m_omCheckKombiNode.SetCheck(1);
    }

    PanelAndNode1->ShowWindow(SW_SHOW);


    //CWinThread* pomThread = NULL ;
    //pomThread = AfxBeginThread( Node1->ContinousMessagesSend);
    //objParent.Detach();
    //return 0;
    //Node1->ContinousMessagesSend();

    //Panel1 = new CPanel();



}


///////////////////////////////////////////////////////




void CMainPanel::OnBnClickedOpenDmePanel()
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());
    INT_PTR nRet = -1;
    CWnd objParent;
    //objParent.Attach(this);
    //CMainPanel* MPAux = this;

    if ( PanelDme == NULL)
    {
        PanelDme = new CPanelDme(&objParent, this);
        //omMainPanel->TotalChannel = TotalChannels;
        PanelDme->Create(IDD_Panel_Dme);
        //NegRespManager = new CUDS_NegRespMng(omManagerPtr);
        FDme = TRUE;
        FDmeInit = FALSE;
        m_omCheckDmeNode.SetCheck(1);
    }

    PanelDme->ShowWindow(SW_SHOW);

    //CWinThread* pomThread = NULL ;
    //pomThread = AfxBeginThread( Node1->ContinousMessagesSend);
    //objParent.Detach();
    //return 0;
    //Node1->ContinousMessagesSend();

    //Panel1 = new CPanel();



}
/**********************************************************************************************************
 Function Name  :
 Description    :
 Member of      :   CPanelAndNode
 Author(s)      :   Marc Serra
 Date Created   :   11.06.2013
**********************************************************************************************************/
unsigned __stdcall TimerFunction(void* a)
{
    CMainPanel* b = (CMainPanel*) a;
    b->currentTime = b->nCalculateCurrTime(FALSE) ;
    b->initialTimeRel = b->currentTime;
    b->initialTimeSpeed = b->currentTime;

    //flagEnvio = TRUE;
    //flagEnvio for now... igual es canvia
    while (b->flagEnvio)
    {
        //Actualize time
        b->currentTime = b->nCalculateCurrTime(FALSE);
        b->currentTime = b->nCalculateCurrTime(TRUE);

        //Falta Cridar a les funcions OnTimerMileRel i OnTimerSpeed
        if (FKombi == TRUE)
        {
            if(FKombiInit != TRUE)
            {
                PanelAndNode1->InitTimers(b->currentTime);
                FKombiInit = TRUE;
            }
            else
            {
                PanelAndNode1->TimersCheck(b->currentTime);
                /*

                if((b->currentTime - b->initialTimeRel) > 100 - 1){
                    PanelAndNode1->OnTimerKmbi();
                    //b->initialTimeRel = b->currentTime;
                    //b->MainP->SendPanelsMessage(b->MessageNode);
                }
                if((b->currentTime - b->initialTimeSpeed) > 50 - 1 ){
                    PanelAndNode1->OnTimerInSwcl();
                    //b->initialTimeSpeed = b->currentTime;
                    //b->MainP->SendPanelsMessage(b->MessageNode);
                }*/
            }

        }

        if (FAbs == TRUE)
        {
            if(FAbsInit != TRUE)
            {
                PanelAbs->InitTimers(b->currentTime);
                FAbsInit = TRUE;
            }
            else
            {
                PanelAbs->TimersCheck(b->currentTime);
                /*

                if((b->currentTime - b->initialTimeRel) > 100 - 1){
                    PanelAndNode1->OnTimerKmbi();
                    //b->initialTimeRel = b->currentTime;
                    //b->MainP->SendPanelsMessage(b->MessageNode);
                }
                if((b->currentTime - b->initialTimeSpeed) > 50 - 1 ){
                    PanelAndNode1->OnTimerInSwcl();
                    //b->initialTimeSpeed = b->currentTime;
                    //b->MainP->SendPanelsMessage(b->MessageNode);
                }*/
            }

        }

        if (FDme == TRUE)
        {
            if(FDmeInit != TRUE)
            {
                PanelDme->InitTimers(b->currentTime);
                FDmeInit = TRUE;
            }
            else
            {
                PanelDme->TimersCheck(b->currentTime);
                /*

                if((b->currentTime - b->initialTimeRel) > 100 - 1){
                    PanelAndNode1->OnTimerKmbi();
                    //b->initialTimeRel = b->currentTime;
                    //b->MainP->SendPanelsMessage(b->MessageNode);
                }
                if((b->currentTime - b->initialTimeSpeed) > 50 - 1 ){
                    PanelAndNode1->OnTimerInSwcl();
                    //b->initialTimeSpeed = b->currentTime;
                    //b->MainP->SendPanelsMessage(b->MessageNode);
                }*/
            }

        }

    }

    return 0;
}

/////////////////////////////////////////////////////////////////////////


UINT CMainPanel::StartTimer()
{
    //int id1=11;
    flagEnvio= TRUE;
    timer = (HANDLE) _beginthreadex(NULL, 0,  TimerFunction,  this, 0, 0);
    //StopTimer();
    return 0;
}
















////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned int CMainPanel::nCalculateCurrTime(BOOL bFromDIL)      // Calculates the differential time in sec
{
    SYSTEMTIME CurrSysTimes;
    UINT64 c_TimeStamp;

    if (bFromDIL == FALSE)
    {
        GetLocalTime(&CurrSysTimes);
    }
    else
    {
        //MainP->Panel_DIL_CAN_Interface->DILC_GetTimeModeMapping(CurrSysTimes, c_TimeStamp);
    }
    unsigned int nResult = (CurrSysTimes.wHour * 3600000 + CurrSysTimes.wMinute * 60000
                            + CurrSysTimes.wSecond) * 1000 + CurrSysTimes.wMilliseconds;
    return nResult;                  // Milliseconds
}


/***********************************************************************************************************/


void CMainPanel::OnCheckAbsNode()
{
    if(m_omCheckAbsNode.GetCheck()==0)
    {
        FAbs = false;
    }
    else
    {
        FAbs = true;
    }
}

/***********************************************************************************************************/


void CMainPanel::OnCheckDmeNode()
{
    if(m_omCheckDmeNode.GetCheck()==0)
    {
        FDme = false;
    }
    else
    {
        FDme = true;
    }
}

/***********************************************************************************************************/


void CMainPanel::OnCheckKombiNode()
{
    if(m_omCheckKombiNode.GetCheck()==0)
    {
        FKombi = false;
    }
    else
    {
        FKombi = true;
    }
}

