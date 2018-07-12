// FormatConverterWnd.cpp : implementation file
//


#include "FormatConverterWnd.h"
#include "DefConverterPage.h"
#include <HtmlHelp.h>
// CFormatConverterWnd

IMPLEMENT_DYNAMIC(CFormatConverterWnd, CPropertySheet)

CFormatConverterWnd::CFormatConverterWnd(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
    :CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
{
    m_pomDefConverterPage = NULL;
}

CFormatConverterWnd::CFormatConverterWnd(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
    :CPropertySheet(pszCaption, pParentWnd, iSelectPage)
{
    m_pomDefConverterPage = NULL;
}

CFormatConverterWnd::~CFormatConverterWnd()
{
    if( m_pouPluginManager != NULL)
    {
        m_pouPluginManager->UnLoadAllPlugins();
        delete m_pouPluginManager;
    }
    if(m_pomDefConverterPage != NULL)
    {
        delete m_pomDefConverterPage;
    }
}

void CFormatConverterWnd::vGettextBusmaster(void)
{
    setlocale(LC_ALL,"");
    bindtextdomain("BUSMASTER", getenv("LOCALDIR") );
    textdomain("BUSMASTER");
}

BEGIN_MESSAGE_MAP(CFormatConverterWnd, CPropertySheet)
    ON_WM_CREATE()
    ON_COMMAND ( ID_HELP, &CFormatConverterWnd::OnHelp )
END_MESSAGE_MAP()


// CFormatConverterWnd message handlers

BOOL CFormatConverterWnd::OnInitDialog()
{
    BOOL bResult = CPropertySheet::OnInitDialog();

    SetIcon( AfxGetApp()->LoadIcon(IDR_MAINFRAME), TRUE);

    // TODO:  Add your specialized code here
    ModifyStyle(0, WS_MINIMIZEBOX);
    CButton* omBtn;
    WINDOWPLACEMENT omWndPlace;

    vGettextBusmaster();

    omBtn = reinterpret_cast<CButton*>(GetDlgItem(ID_APPLY_NOW));
    omBtn->ShowWindow(SW_HIDE);
    omBtn->GetWindowPlacement(&omWndPlace);

    omBtn = reinterpret_cast<CButton*>(GetDlgItem(IDOK));
    omBtn->ShowWindow(SW_HIDE);



    omBtn = reinterpret_cast<CButton*>(GetDlgItem(IDCANCEL));
    omBtn->SetWindowText(_("Close"));
    omBtn->SetWindowPlacement(&omWndPlace);
    return bResult;
}

int CFormatConverterWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    if (CPropertySheet::OnCreate(lpCreateStruct) == -1)
    {
        return -1;
    }

    // TODO:  Add your specialized creation code here

    return 0;
}

void CFormatConverterWnd::OnHelp ()
{
    // Get Application Help File Path
    CString omStrPath = AfxGetApp ()->m_pszHelpFilePath;

    // Replace .hlp with .chm
    int nIndex = omStrPath.ReverseFind ('.');
    // Extract string before the extension
    omStrPath = omStrPath.Mid ( 0, nIndex );
    // Add New Extension
    omStrPath = omStrPath + ".chm" + "::/topics/format_converters.html";

    omStrPath.Replace ( "FormatConverter.chm", "BUSMASTER.chm" );
    
    // Make it as content display always
    ::HtmlHelp ( nullptr, omStrPath, HH_DISPLAY_TOPIC, 0 );
}

BOOL CFormatConverterWnd::Create(CWnd* pParentWnd , DWORD dwStyle , DWORD dwExStyle)
{
    // TODO: Add your specialized code here and/or call the base class

    CPropertySheet::Create(pParentWnd, dwStyle, dwExStyle);
    //SetActivePage(nCount);
    SetActivePage(1);
    return TRUE;
}


HRESULT CFormatConverterWnd::LoadConverters()
{
    m_pouPluginManager = new CPluginManagerDLL();
    m_pouPluginManager->LoadConvertersFromFolder();

    INT nCount = 0;

    //m_pomDefConverterPage->m_psp.dwFlags &= ~PSP_HIDEHEADER;

    for(INT i = 0; i < m_pouPluginManager->m_ConverterList.GetCount(); i++)
    {
        POSITION pos = m_pouPluginManager->m_ConverterList.FindIndex(i);
        ConverterInfo& ouConverterInfo = m_pouPluginManager->m_ConverterList.GetAt(pos);
        if(ouConverterInfo.m_pouConverter->bHaveOwnWindow() == TRUE)
        {
            //CPropertyPage *pPage = NULL;
            ouConverterInfo.m_pouConverter->GetPropertyPage(ouConverterInfo.m_pomPage);
            if(ouConverterInfo.m_pomPage != NULL)
            {
                ouConverterInfo.m_pomPage->m_psp.dwFlags &= ~PSP_HASHELP;
                AddPage(ouConverterInfo.m_pomPage);
                nCount++;
            }
        }
    }
    m_pomDefConverterPage = new CDefConverterPage(nCount);
    m_pomDefConverterPage->SetPluginManager(m_pouPluginManager);
    m_pomDefConverterPage->m_psp.dwFlags &= ~PSP_HASHELP;
    AddPage(m_pomDefConverterPage);

    return S_OK;

}

BOOL CFormatConverterWnd::PreCreateWindow(CREATESTRUCT& cs)
{
    // TODO: Add your specialized code here and/or call the base class
    cs.dwExStyle = cs.dwExStyle | WS_EX_TOOLWINDOW;
    return CPropertySheet::PreCreateWindow(cs);
}
