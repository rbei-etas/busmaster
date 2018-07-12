// ProgressBar.cpp : implementation file
//

#include "stdafx.h"
#include "BUSMASTER.h"
#include "ProgressBarDlg.h"
#include "MsgFrmtWnd.h"
#define WM_EXIT_DIALOG WM_USER+5
//#include "afxdialogex.h"




// CProgressBarDlg dialog

IMPLEMENT_DYNAMIC(CProgressBarDlg, CDialog)

CProgressBarDlg::CProgressBarDlg(const sLoadFile& structLoadFile,CWnd* pParent /*=NULL*/)
    : CDialog(CProgressBarDlg::IDD, pParent)
{
    m_pThread=nullptr;
    m_structLoadFile = structLoadFile;
}

CProgressBarDlg::~CProgressBarDlg()
{
}

void CProgressBarDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_PROGRESS_BAR, m_omProgressCtrl);
    DDX_Control(pDX,IDC_PROGRESS_TEXT, m_omProgressText);
    DDX_Control(pDX,IDC_STATIC_ERROR,m_omErrorText);
}


BEGIN_MESSAGE_MAP(CProgressBarDlg, CDialog)
    ON_WM_TIMER()
    ON_WM_CLOSE()
    ON_MESSAGE(WM_EXIT_DIALOG, OnCloseProgressBar)
END_MESSAGE_MAP()


// CProgressBarDlg message handlers
HRESULT CProgressBarDlg::SetProgressBarRange(int nMin,int nMax)
{
    HRESULT hResult = S_FALSE;
    if(nMin>=0&&nMax>nMin)
    {
        hResult = S_OK;
        m_omProgressCtrl.SetRange(nMin,nMax);
    }
    return hResult;
}
HRESULT CProgressBarDlg::SetProgressBarPos(int nPos)
{
    HRESULT hResult = S_FALSE;
    if(nPos>=0)
    {
        hResult = S_OK;
        m_omProgressCtrl.SetPos(nPos);
    }
    return hResult;
}
HRESULT CProgressBarDlg::SetProgressText(std::string strProgressText)
{
    HRESULT hResult = S_FALSE;
    if(strProgressText!="")
    {
        hResult=S_OK;
        m_omProgressText.SetWindowText(strProgressText.c_str());
    }
    return hResult;
}

BOOL CProgressBarDlg::OnInitDialog()
{
    CDialog::OnInitDialog();
    m_omProgressCtrl.SetPos(0);
    m_omProgressText.SetWindowText("0 %");
    m_pThread = AfxBeginThread(CProgressBarDlg::LoadLogFileThreadProc,this);

    SetProgressBarRange(0,100);
    m_unProgressBarTimerId = SetTimer(600, 100, NULL);
    return TRUE;

}
void CProgressBarDlg::OnCancel()
{
    KillTimer(m_unProgressBarTimerId);
    if(m_structLoadFile.m_pouMsgContainerIntrf!=nullptr)
    {
        m_structLoadFile.m_pouMsgContainerIntrf->CancelFileLoad();
    }
    CDialog::OnCancel();
}
void CProgressBarDlg::OnOK()
{
    CDialog::OnOK();
}
UINT CProgressBarDlg::LoadLogFileThreadProc(LPVOID lParams)
{
    HRESULT hResult = S_FALSE;
    CProgressBarDlg* pProgressBarDlg = (CProgressBarDlg*)lParams;
    if(pProgressBarDlg == nullptr)
    {
        return hResult;
    }
    std::string strFilePath = pProgressBarDlg->m_structLoadFile.m_strFileName;

    if(PathFileExists(strFilePath.c_str()) == TRUE && pProgressBarDlg->m_structLoadFile.m_pouMsgContainerIntrf!=nullptr)
    {
        HRESULT hImportResult = pProgressBarDlg->m_structLoadFile.m_pouMsgContainerIntrf->LoadLogFile(strFilePath) ;
        if(hImportResult == S_OK)
        {
            if(pProgressBarDlg->m_structLoadFile.m_oupMsgFrmtWnd!=nullptr)
            {
                if(pProgressBarDlg->m_structLoadFile.m_oupMsgFrmtWnd->bStartReadThread() == TRUE)
                {
                    unsigned long nPageCount=0;
                    pProgressBarDlg->m_structLoadFile.m_pouMsgContainerIntrf->GetTotalPages(nPageCount);
                    hResult = pProgressBarDlg->m_structLoadFile.m_pouMsgContainerIntrf->LoadPage(0);
                }
            }
            pProgressBarDlg->PostMessage(WM_EXIT_DIALOG,S_OK,0);
        }
        else
        {
            pProgressBarDlg->PostMessage(WM_EXIT_DIALOG,S_FALSE,hImportResult);
        }
    }
    return hResult;
}

void CProgressBarDlg::OnTimer(UINT_PTR nIDEvent)
{
    if(nIDEvent == 600)
    {
        if(m_structLoadFile.m_pouMsgContainerIntrf!=nullptr)
        {
            int nPerct = m_structLoadFile.m_pouMsgContainerIntrf->GetPercentageLoaded();
            std::string strPerct = std::to_string(nPerct)+" %";
            SetProgressBarPos(nPerct);
            SetProgressText(strPerct);
        }
    }
    CDialog::OnTimer(nIDEvent);
}

LRESULT CProgressBarDlg::OnCloseProgressBar(WPARAM wParam, LPARAM lParam)
{
    HRESULT hResult = (HRESULT)wParam;
    if(m_pThread!=nullptr)
    {
        //m_pThread->PostThreadMessage(WM_QUIT, 0, 0);
        WaitForSingleObject(m_pThread->m_hThread, 500);
    }
    //Wait for thread

    KillTimer(m_unProgressBarTimerId);
    if(hResult == S_OK)
    {
        OnOK();
    }
    else if(hResult == S_FALSE)
    {
        HRESULT hImportResult = (HRESULT)lParam;
        CString omStrError = "Error: ";
        if(hImportResult < eImportLogTotal)
        {
            omStrError += m_omImportLogErr[hImportResult].c_str();
        }
        else
        {
            omStrError += m_omImportLogErr[eImportLogInvalid].c_str();
        }
        m_omErrorText.SetWindowText(omStrError);
        //PostMessage(IDCANCEL);
    }
    return S_OK;
}
