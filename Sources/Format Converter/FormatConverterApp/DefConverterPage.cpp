// DefConverterPage.cpp : implementation file
//

#include "stdafx.h"
#include "DefConverterPage.h"
#include <strsafe.h>

// CDefConverterPage dialog

IMPLEMENT_DYNAMIC(CDefConverterPage, CPropertyPage)

CDefConverterPage::CDefConverterPage()
	: CPropertyPage(CDefConverterPage::IDD)
    , m_omStrInputFilePath(_T(""))
    , m_omStrOutputFilePath(_T(""))
    , m_omstrConversionComment(_T(""))
    , m_omstrEditHelp(_T(""))
{

}

CDefConverterPage::~CDefConverterPage()
{
}

void CDefConverterPage::DoDataExchange(CDataExchange* pDX)
{
    CPropertyPage::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_COMBO_CONVERSIONS, m_omComboConverterNames);
    DDX_Control(pDX, IDC_EDIT_INPUTFILEPATH, m_omEditInputPath);
    DDX_Control(pDX, IDC_EDIT_OUTPUTFILEPATH, m_omEditOutputPath);
    DDX_Text(pDX, IDC_EDIT_INPUTFILEPATH, m_omStrInputFilePath);
    DDX_Text(pDX, IDC_EDIT_OUTPUTFILEPATH, m_omStrOutputFilePath);
    DDX_Text(pDX, IDC_EDIT_COMMENT, m_omstrConversionComment);
    DDX_Text(pDX, IDC_EDIT_HELP, m_omstrEditHelp);
	DDV_MaxChars(pDX, m_omstrEditHelp, 1024);
}


BEGIN_MESSAGE_MAP(CDefConverterPage, CPropertyPage)
    ON_BN_CLICKED(IDC_BTN_INPUT, CDefConverterPage::OnBnClickedBtnInput)
    ON_BN_CLICKED(IDC_BTN_OUTPUT, CDefConverterPage::OnBnClickedBtnOutput)
    ON_BN_CLICKED(IDC_BTN_CONVERT, CDefConverterPage::OnBnClickedBtnConvert)
    ON_CBN_SELCHANGE(IDC_COMBO_CONVERSIONS, CDefConverterPage::OnCbnSelchangeComboConversions)
END_MESSAGE_MAP()


// CDefConverterPage message handlers
HRESULT CDefConverterPage::SetPluginManager(CPluginManagerBase *pouPluginManager)
{
    m_pouPluginManager = pouPluginManager;
    return S_FALSE;
}
HRESULT CDefConverterPage::LoadConverters()
{
    HRESULT hResult = S_FALSE;
    if(m_pouPluginManager != NULL)
    {
        INT nCount = m_pouPluginManager->m_ConverterList.GetCount();
        TCHAR strConverterName[1024] = {'\0'};
        for(INT i = 0; i < nCount; i++)
        {
            POSITION pos = m_pouPluginManager->m_ConverterList.FindIndex(i);
            ConverterInfo& ouConverterInfo = m_pouPluginManager->m_ConverterList.GetAt(pos);
            if(ouConverterInfo.m_pouConverter->bHaveOwnWindow() == FALSE)
            {
                ouConverterInfo.m_pouConverter->GetConverterName(strConverterName);
                INT nItem = m_omComboConverterNames.AddString(strConverterName);
                if(nItem >= 0)
                {
                    m_omComboConverterNames.SetItemData(nItem, i);
                }
            }
        }
        hResult = S_OK;
    }
    return hResult;
}
BOOL CDefConverterPage::OnInitDialog()
{
    CPropertyPage::OnInitDialog();
    // TODO:  Add extra initialization here
    LoadConverters();
    m_omComboConverterNames.SetCurSel(0);
    OnCbnSelchangeComboConversions();
    return TRUE;  // return TRUE unless you set the focus to a control
}

void CDefConverterPage::OnBnClickedBtnInput()
{
    // TODO: Add your control notification handler code here
    TCHAR strFileExt[1024];
    TCHAR strFileFilters[1024];
    INT nSelected = GetConverterPos();
    if( nSelected < m_pouPluginManager->m_ConverterList.GetCount())
    {
        POSITION pos = m_pouPluginManager->m_ConverterList.FindIndex(nSelected);
        ConverterInfo &ouConverterInfo = m_pouPluginManager->m_ConverterList.GetAt(pos);
        if( ouConverterInfo.m_pouConverter != NULL)
        {
            ouConverterInfo.m_pouConverter->GetInputFileFilters(strFileExt, strFileFilters);
        }
    }
    CFileDialog fileDlg( TRUE,     // Open File dialog
                         strFileExt,     // Default Extension,
                         NULL,     // Initial file name.
						 OFN_OVERWRITEPROMPT|OFN_FILEMUSTEXIST|
						 OFN_PATHMUSTEXIST,
                         strFileFilters,
                         this );
    INT_PTR nRetVal = fileDlg.DoModal();
    if(IDOK == nRetVal)
    {
        m_omEditInputPath.SetWindowText(fileDlg.GetPathName());
    }
}

void CDefConverterPage::OnBnClickedBtnOutput()
{
    TCHAR strFileExt[1024];
    TCHAR strFileFilters[1024];
    INT nSelected = GetConverterPos();
    if( nSelected < m_pouPluginManager->m_ConverterList.GetCount())
    {
        POSITION pos = m_pouPluginManager->m_ConverterList.FindIndex(nSelected);
        ConverterInfo &ouConverterInfo = m_pouPluginManager->m_ConverterList.GetAt(pos);
        if( ouConverterInfo.m_pouConverter != NULL)
        {
            ouConverterInfo.m_pouConverter->GetOutputFileFilters(strFileExt, strFileFilters);
        }
    }
    CFileDialog fileDlg( TRUE,     // Open File dialog
                         strFileExt,     // Default Extension,
                         NULL,     // Initial file name.
                         OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
                         strFileFilters,
                         this );
    INT_PTR nRetVal = fileDlg.DoModal();
    
    if(IDOK == nRetVal)
    {
        m_omEditOutputPath.SetWindowText(fileDlg.GetPathName());
    }
}

void CDefConverterPage::OnBnClickedBtnConvert()
{
    // TODO: Add your control notification handler code here
    INT nSelected = GetConverterPos();
  
    if( (nSelected >= 0 ) && (nSelected < m_pouPluginManager->m_ConverterList.GetCount()))
    {
        POSITION pos = m_pouPluginManager->m_ConverterList.FindIndex(nSelected);
        ConverterInfo &ouConverterInfo = m_pouPluginManager->m_ConverterList.GetAt(pos);
        if( ouConverterInfo.m_pouConverter != NULL)
        {
            UpdateData();
            TCHAR strInputFilePath[1026];
            TCHAR strOutputFilePath[1026];
            StringCbCopy(strInputFilePath, sizeof(strInputFilePath), m_omStrInputFilePath);
            StringCbCopy(strOutputFilePath, sizeof(strOutputFilePath), m_omStrOutputFilePath);
            HRESULT hResult = ouConverterInfo.m_pouConverter->ConvertFile(strInputFilePath, strOutputFilePath);
            ouConverterInfo.m_pouConverter->GetLastConversionStatus(hResult, m_omstrConversionComment);
            UpdateData(FALSE);
        }
    }
}

void CDefConverterPage::OnCbnSelchangeComboConversions()
{
    // TODO: Add your control notification handler code here
    m_omStrInputFilePath = _T("");
    m_omStrOutputFilePath = _T("");
    m_omstrConversionComment = _T("");
    CString omstrConversionName;
    INT nSelectedItemIndex = GetConverterPos();
    if( nSelectedItemIndex >= 0 )
    {
        TCHAR chHelpText[1024];
        TCHAR chConversionName[1024];
        POSITION pos = m_pouPluginManager->m_ConverterList.FindIndex(nSelectedItemIndex);
        ConverterInfo &ouConverterInfo = m_pouPluginManager->m_ConverterList.GetAt(pos);
        ouConverterInfo.m_pouConverter->GetHelpText(chHelpText);
        ouConverterInfo.m_pouConverter->GetConverterName(chConversionName);
        
        m_omstrEditHelp = chHelpText;
        omstrConversionName = chConversionName;
        //TODO::Require some more knowledge on how to change 
        //propertypage title.
        TC_ITEM tcItem;
        tcItem.mask = TCIF_TEXT;
        tcItem.pszText = (LPTSTR)((LPCTSTR)omstrConversionName);
        ((CPropertySheet*)GetParent())->GetTabControl()->SetItem(0, &tcItem );
        SetWindowText(omstrConversionName);
    }
    UpdateData(FALSE);
}


INT CDefConverterPage::GetConverterPos()
{
    INT nComboBoxIndex = m_omComboConverterNames.GetCurSel();
    INT nListIndex = m_omComboConverterNames.GetItemData(nComboBoxIndex);
    if( (nListIndex >= 0 ) && (nListIndex < m_pouPluginManager->m_ConverterList.GetCount()))
    {
        return nListIndex;
    }
    else
    {
        return -1;
    }
}