/*
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "stdafx.h"
#include "ChannelConfigurationDlg.h"
#include "FibexClass_extern.h"
#include "Utility\WaitIndicator.h"


struct LinProcolBaudRate
{
    string m_strProtocol;
    int m_nBaudRate;
};

static LinProcolBaudRate sg_LINPROTOCOL_BAUD[] =
{
    {"LIN 2.2", 19200},
    {"LIN 2.1", 19200},
    {"LIN 2.0", 19200},
    {"LIN 1.3", 9600},
    {"LIN 1.2", 9600},
    {"LIN 1.1", 9600},
};


IMPLEMENT_DYNAMIC(CChannelConfigurationDlg, CDialog)
CChannelConfigurationDlg::CChannelConfigurationDlg(CMsgSignal*& pMsgSignal ,CHANNEL_CONFIG ouFlexrayChannelConfig[], INT& nChannelConfigured, ETYPE_BUS eBusType, CWnd* pParent /*=NULL*/)
    : CDialog(CChannelConfigurationDlg::IDD, pParent)
{
    m_eBusType = eBusType;
    m_nChannelConfigured = nChannelConfigured;
    m_pMsgSignal = pMsgSignal;
    for ( int i = 0 ; i < nChannelConfigured; i++ )
    {
        m_ouFlexrayChannelConfig[i] = ouFlexrayChannelConfig[i];
    }
    m_nCurrentChannel = -1;
}

CChannelConfigurationDlg::~CChannelConfigurationDlg()
{
}

void CChannelConfigurationDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_EDIT_FIBEX_PATH, m_strFibexFilePath);
    DDX_Control(pDX, IDC_EDIT_FIBEX_PATH, m_omFibexPath);
    DDX_Control(pDX, IDC_COMBO_CHANNEL, m_ComboChannelSelect);
    DDX_Control(pDX, IDC_LIST_ECU, m_omEcuList);
    DDX_Control(pDX, IDC_COMBO_CLUSTER, m_omComboCluster);
    DDX_Control(pDX, IDC_COMBO_LIN_PROTOCOL, m_omComboLinProtocol);
    DDX_Text(pDX, IDC_EDIT_LIN_BAUDRATE, m_nLinBaudRate);
    DDV_MinMaxInt(pDX, m_nLinBaudRate, 200, 30000);
}


BEGIN_MESSAGE_MAP(CChannelConfigurationDlg, CDialog)
    ON_WM_SYSCOMMAND()
    ON_WM_PAINT()
    ON_WM_QUERYDRAGICON()
    //}}AFX_MSG_MAP
    ON_BN_CLICKED(IDC_BUTTON_FIBEXPATH, OnBnClickedButtonFibexpath)
    ON_CBN_SELCHANGE(IDC_COMBO_CLUSTER, OnCbnSelchangeComboCluster)
    ON_CBN_SELCHANGE(IDC_COMBO_CHANNEL, OnCbnSelchangeComboChannel)
    ON_CBN_SELCHANGE(IDC_COMBO_LIN_PROTOCOL, OnCbnSelchangeComboProtocol)
    ON_BN_CLICKED(IDOK, onBtnOk)
    ON_BN_CLICKED(IDC_CHECK_OVERWRITE_SETTINGS, OnOverwriteCheckBoxClick)
END_MESSAGE_MAP()

void CChannelConfigurationDlg::OnOverwriteCheckBoxClick()
{
    CButton* pTempBtn = (CButton*)GetDlgItem(IDC_CHECK_OVERWRITE_SETTINGS);
    bool bCheck = false;
    if ( NULL != pTempBtn )
    {
        bCheck = pTempBtn->GetCheck();
    }

    CWnd* pTempChild = GetDlgItem(IDC_COMBO_LIN_PROTOCOL);
    if ( NULL != pTempChild )
    {
        pTempChild->EnableWindow(bCheck);
    }

    pTempChild = GetDlgItem(IDC_EDIT_LIN_BAUDRATE);
    if ( NULL != pTempChild )
    {
        pTempChild->EnableWindow(bCheck);
    }
}

BOOL CChannelConfigurationDlg::OnInitDialog()
{


    CDialog::OnInitDialog();


    //Validate The Bus Type - FLEXRAY, LIN
    if ( m_eBusType != FLEXRAY && m_eBusType != LIN )
    {
        return S_FALSE;
    }


    //Controls Initialisation
    m_omEcuList.SetExtendedStyle(LVS_EX_CHECKBOXES | LVS_REPORT );
    m_omEcuList.InsertColumn(0, "Select ECU");
    // Set the width to occupy the whole list
    CRect omRect;
    m_omEcuList.GetWindowRect(&omRect);
    int nWidth = static_cast<int>(omRect.Width() * 0.95);

    // Set Col Width
    m_omEcuList.SetColumnWidth(0, nWidth);


    char chTemp[MAX_PATH];
    //INT nChannelCount = m_ouFlexrayChannelConfig.size();
    m_ComboChannelSelect.Clear();
    for ( int i = 0 ; i < m_nChannelConfigured ; i++ )
    {
        sprintf_s(chTemp, "Channel %d", i+1);
        m_ComboChannelSelect.AddString( chTemp);
    }


    //Update for 1st Channel
    m_nCurrentChannel = -1;
    m_ComboChannelSelect.SetCurSel(0);
    OnCbnSelchangeComboChannel();

    nUpdateLinSettings();

    nDisplayProtocolSettings(0);
    nEnableControls(m_eBusType);



    /*list<CHANNEL_CONFIG>::iterator itrFlexrayChannelConfig = m_ouFlexrayChannelConfig.begin();
    if ( itrFlexrayChannelConfig != m_ouFlexrayChannelConfig.end() )
    {
        list<Cluster> ouClusterList;
        if ( itrFlexrayChannelConfig->m_strDataBasePath.c_str() != "")
        {
            m_pMsgSignal->hLoadFibexDBFile(itrFlexrayChannelConfig->m_strDataBasePath.c_str(), ouClusterList);
            m_ouCurrentChannelCluster = ouClusterList;
        }

    }*/


    return TRUE;
}



int CChannelConfigurationDlg::nUpdateLinSettings()
{
    char chString[MAX_PATH];
    CComboBox* pomCombo = (CComboBox*)GetDlgItem(IDC_COMBO_LIN_PROTOCOL);

    if ( pomCombo != NULL )
    {
        for ( int i = 0 ; i < ( sizeof(sg_LINPROTOCOL_BAUD)/ sizeof(sg_LINPROTOCOL_BAUD[0])); i++ )
        {
            pomCombo->InsertString(i, sg_LINPROTOCOL_BAUD[i].m_strProtocol.c_str());
            sprintf_s(chString, "%d", sg_LINPROTOCOL_BAUD[i].m_nBaudRate );
        }
        pomCombo->SetCurSel(0);
    }

    m_nLinBaudRate = 1900;
    UpdateData(FALSE);
    OnOverwriteCheckBoxClick();

    return 0;
}


int CChannelConfigurationDlg::nEnableControls( ETYPE_BUS eBusType)
{
    CWnd* pWnd = NULL;
    if ( eBusType == FLEXRAY )
    {
        pWnd = GetDlgItem(IDC_STATIC_DATABASE_INFO);
        if ( pWnd != NULL )
        {
            pWnd->SetWindowText("Import Flexray Database (FIBEX)");
        }

        pWnd = GetDlgItem(IDC_STATIC_CLUSTER_INFO);
        if ( pWnd != NULL )
        {
            pWnd->SetWindowText("Select Flexray Cluster:");
        }

        pWnd = GetDlgItem(IDC_STATIC_EXTRA_CONFIG);
        if ( pWnd != NULL )
        {
            pWnd->SetWindowText("Key Slot Configuration");
        }

        pWnd = GetDlgItem(IDC_STATIC_DBNAME);
        if ( pWnd != NULL )
        {
            pWnd->SetWindowText("Fibex:");
        }

        pWnd = GetDlgItem(IDC_COMBO_LIN_PROTOCOL);
        if ( pWnd != NULL )
        {
            pWnd->ShowWindow(FALSE);
        }

        pWnd = GetDlgItem(IDC_STATIC_LIN_PROTOCOL);
        if ( pWnd != NULL )
        {
            pWnd->ShowWindow(FALSE);
        }

        pWnd = GetDlgItem(IDC_STATIC_LIN_BAUDRATE);
        if ( pWnd != NULL )
        {
            pWnd->ShowWindow(FALSE);
        }

        pWnd = GetDlgItem(IDC_EDIT_LIN_BAUDRATE);
        if ( pWnd != NULL )
        {
            pWnd->ShowWindow(FALSE);
        }

        pWnd = GetDlgItem(IDC_CHECK_OVERWRITE_SETTINGS);
        if ( pWnd != NULL )
        {
            pWnd->ShowWindow(FALSE);
        }
    }
    else if ( eBusType == LIN )
    {
        pWnd = GetDlgItem(IDC_STATIC_DATABASE_INFO);
        if ( pWnd != NULL )
        {
            pWnd->SetWindowText("Import LIN Database (LDF)");
        }

        pWnd = GetDlgItem(IDC_STATIC_CLUSTER_INFO);
        if ( pWnd != NULL )
        {
            pWnd->SetWindowText("Select LIN Cluster:");
        }

        pWnd = GetDlgItem(IDC_STATIC_EXTRA_CONFIG);
        if ( pWnd != NULL )
        {
            pWnd->SetWindowText("LIN Network Settings");
        }

        pWnd = GetDlgItem(IDC_STATIC_DBNAME);
        if ( pWnd != NULL )
        {
            pWnd->SetWindowText("LDF:");
        }

        pWnd = GetDlgItem(IDC_COMBO_CLUSTER);
        if ( pWnd != NULL )
        {
            pWnd->EnableWindow(FALSE);
        }
    }

    return S_OK;
}

void CChannelConfigurationDlg::OnBnClickedButtonFibexpath()
{
    CFileDialog* pomFibexDlg = NULL;
    string strWaitText;
    if ( m_eBusType == FLEXRAY )
    {
        pomFibexDlg = new CFileDialog(TRUE, ".xml", 0, OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, "FIBEX Files (*.xml)|*.xml||", this);
        pomFibexDlg->m_ofn.lpstrTitle = "Select A FIBEX File";
        strWaitText = "Parsing Fibex File. Please Wait...";
    }
    else if ( m_eBusType == LIN )
    {
        pomFibexDlg = new CFileDialog(TRUE, ".xml", 0, OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, "LDF Files (*.ldf)|*.ldf||", this);
        pomFibexDlg->m_ofn.lpstrTitle = "Select A LDF File";
        strWaitText = "Parsing LDF File. Please Wait...";
    }

    if ( ( pomFibexDlg != NULL ) && ( pomFibexDlg->DoModal() == IDOK ) )
    {
        CString strPath = pomFibexDlg->GetPathName();
        list<Cluster> ouClusterList;
        list<LinChannelParam> ouLinChannelParams;
        CWaitIndicator ouWaitIndicator;
        ouWaitIndicator.DisplayWindow(strWaitText.c_str(), this);

        int nResult = FCLASS_FAILURE;

        if ( m_eBusType == FLEXRAY )
        {
            nResult = m_pMsgSignal->hLoadFibexDBFile(strPath, ouClusterList);
        }
        else if ( m_eBusType == LIN )
        {
            nResult = m_pMsgSignal->hLoadLdfFile((LPCSTR)strPath, ouClusterList, ouLinChannelParams);

        }

        ouWaitIndicator.CloseWindow();
        m_nCurrentChannel = m_ComboChannelSelect.GetCurSel();

        if ( nResult == FCLASS_SUCCESS && ouClusterList.size() > 0 )
        {
            m_omComboCluster.ResetContent();
            //list<CHANNEL_CONFIG>::iterator itrChannelConfig = m_ouFlexrayChannelConfig.begin();
            //advance(itrChannelConfig, m_nCurrentChannel );
            //if ( itrChannelConfig != m_ouFlexrayChannelConfig.end() )
            {
                m_ouCurrentChannelCluster = ouClusterList;
                m_ouLinChannelParams = ouLinChannelParams;
                list<Cluster>::iterator itrCluster = ouClusterList.begin();
                //itrChannelConfig->m_ouClusterInfo = *itrCluster;

                for ( ; itrCluster != ouClusterList.end(); itrCluster++)
                {
                    m_omComboCluster.AddString(itrCluster->m_strName.c_str());
                }
                m_omComboCluster.SetCurSel(0);

                if ( m_omComboCluster.GetCount() <=1 )
                {
                    m_omComboCluster.EnableWindow(FALSE);
                }


                nUpdateEcuList(0, 0);
                nUpdateLinParams(0,0);
                m_strFibexFilePath = strPath;
            }
        }
        else
        {
            if ( nResult ==  VERSION_NOT_SUPPORTED )
            {
                MessageBox("Unsupported FIBEX Version", "Error", MB_OK | MB_ICONERROR );
            }
            else
            {
                MessageBox("Invalid Input File", "Error", MB_OK | MB_ICONERROR );
            }
        }
        UpdateData(FALSE);
    }

}

void CChannelConfigurationDlg::OnCbnSelchangeComboCluster()
{
    // TODO: Add your control notification handler code here

    //  nUpdateEcuList(0, 0);
}
void CChannelConfigurationDlg::OnCbnSelchangeComboProtocol()
{
    int nSel = m_omComboLinProtocol.GetCurSel();

    if ( nSel >= 0 )
    {
        m_nLinBaudRate = sg_LINPROTOCOL_BAUD[nSel].m_nBaudRate;
        UpdateData(FALSE);
    }

    return;
}

void CChannelConfigurationDlg::OnCbnSelchangeComboChannel()
{
    //Save Current Values to the old Channel
    if ( -1 != m_nCurrentChannel )
    {

        list<Cluster>::iterator itFlexConfig = m_ouCurrentChannelCluster.begin();

        advance(itFlexConfig, m_omComboCluster.GetCurSel());

        if ( itFlexConfig != m_ouCurrentChannelCluster.end() )
        {
            if (  m_nCurrentChannel < m_nChannelConfigured )
            {
                //1. Save Cluster Info
                m_ouFlexrayChannelConfig[m_nCurrentChannel].m_ouClusterInfo = *itFlexConfig;

                //2. Save ECU Info
                m_ouFlexrayChannelConfig[m_nCurrentChannel].m_strSlectedEculist.clear();


                list<ECU_Struct> ouEcuList;
                m_ouFlexrayChannelConfig[m_nCurrentChannel].m_ouClusterInfo.GetECUList(ouEcuList);
                list<ECU_Struct>::iterator itrEcu = ouEcuList.begin();


                int nEcuCount = m_omEcuList.GetItemCount();
                for ( int i = 0; i < nEcuCount; i++ )
                {
                    itrEcu = ouEcuList.begin();
                    if ( m_omEcuList.GetCheck(i) == TRUE )
                    {
                        advance(itrEcu, i);
                        if ( itrEcu != ouEcuList.end())
                        {
                            m_ouFlexrayChannelConfig[m_nCurrentChannel].m_strSlectedEculist.push_back( itrEcu->m_strEcuId);
                        }
                    }
                }

                //3. Fibex Path
                m_ouFlexrayChannelConfig[m_nCurrentChannel].m_strDataBasePath = m_strFibexFilePath;

            }
        }
    }



    //Update the selected channel Information

    INT nSelcetedIndex = m_ComboChannelSelect.GetCurSel();
    m_nCurrentChannel = nSelcetedIndex;
    m_ouCurrentChannelCluster.clear();
    m_omEcuList.DeleteAllItems();
    if ( nSelcetedIndex < m_nChannelConfigured )// m_ouFlexrayChannelConfig.size())
    {
        m_strFibexFilePath = m_ouFlexrayChannelConfig[nSelcetedIndex].m_strDataBasePath.c_str();

        m_ouCurrentChannelCluster.push_back(m_ouFlexrayChannelConfig[nSelcetedIndex].m_ouClusterInfo);

        m_omComboCluster.Clear();
        /*CWaitIndicator ouWaitIndicator;
        ouWaitIndicator.DisplayWindow("Parsing Fibex File. Please Wait...", AfxGetMainWnd());

        m_pMsgSignal->hLoadFibexDBFile(m_strFibexFilePath, m_ouCurrentChannelCluster);

        ouWaitIndicator.CloseWindow();*/


        m_omComboCluster.AddString( m_ouFlexrayChannelConfig[nSelcetedIndex].m_ouClusterInfo.m_strName.c_str());
        m_omComboCluster.SetCurSel(0);
        list<ECU_Struct> ouEcuList;
        m_ouFlexrayChannelConfig[nSelcetedIndex].m_ouClusterInfo.GetECUList(ouEcuList);

        INT nIndex = 0;
        for ( list<ECU_Struct>::iterator itrEcu = ouEcuList.begin(); itrEcu!= ouEcuList.end(); itrEcu++ )
        {
            m_omEcuList.InsertItem(nIndex, itrEcu->m_strECUName.c_str() );

            if ( bIsEcuSlected(m_ouFlexrayChannelConfig[nSelcetedIndex].m_strSlectedEculist,  itrEcu->m_strEcuId) )
            {
                m_omEcuList.SetCheck(nIndex);
            }
            nIndex++;
        }
        nDisplayProtocolSettings(nSelcetedIndex);
    }
    UpdateData(FALSE);
}

int CChannelConfigurationDlg::nDisplayProtocolSettings(int nChannelIndex)
{
    if ( m_eBusType == LIN )
    {
        BOOL bOverrite = m_ouFlexrayChannelConfig[nChannelIndex].m_ouLinParams.m_bOverWriteSettings;

        ((CButton*)GetDlgItem(IDC_CHECK_OVERWRITE_SETTINGS))->SetCheck(bOverrite);

        CComboBox* pomCombo = (CComboBox*)GetDlgItem(IDC_COMBO_LIN_PROTOCOL);
        pomCombo->EnableWindow(bOverrite);

        GetDlgItem(IDC_EDIT_LIN_BAUDRATE)->EnableWindow(bOverrite);
        char chBaudText[MAX_PATH];

        m_nLinBaudRate = 19200;
        pomCombo->SetCurSel(0);
        for ( int i = 0 ; i < ( sizeof(sg_LINPROTOCOL_BAUD)/ sizeof(sg_LINPROTOCOL_BAUD[0])); i++ )
        {
            if ( m_ouFlexrayChannelConfig[nChannelIndex].m_ouLinParams.m_strProtocolVersion == sg_LINPROTOCOL_BAUD[i].m_strProtocol )
            {
                pomCombo->SetCurSel(i);
                if ( bOverrite == false )
                {
                    m_nLinBaudRate = sg_LINPROTOCOL_BAUD[i].m_nBaudRate;
                }
                else
                {
                    m_nLinBaudRate =  m_ouFlexrayChannelConfig[nChannelIndex].m_ouLinParams.m_nBaudRate;
                }

                break;
            }
        }
        UpdateData(FALSE);
    }
    return 0;
}


bool CChannelConfigurationDlg::bIsEcuSlected(list<string>& ouEcuList, string strEcuName)
{
    bool bFound = false;
    list<string>::iterator itrEcu = ouEcuList.begin();
    for ( ; itrEcu!= ouEcuList.end(); itrEcu++)
    {
        if ( strEcuName == *itrEcu )
        {
            bFound = true;
        }
    }
    return bFound;
}

INT CChannelConfigurationDlg::nUpdateLinParams( INT nChannelIndex, INT nClusterIndex)
{
    INT nResult = S_OK;
    //todo::
    list<LinChannelParam>::iterator itrCluster =  m_ouLinChannelParams.begin();
    advance(itrCluster, nClusterIndex);
    if ( itrCluster != m_ouLinChannelParams.end() )
    {
        char chText[MAX_PATH];
        sprintf(chText, "%d", itrCluster->m_nBaudRate);
        GetDlgItem(IDC_EDIT_LIN_BAUDRATE)->SetWindowText(chText);

        int nIndex = m_omComboLinProtocol.FindString(0, itrCluster->m_strProtocolVersion.c_str());
        if ( nIndex != 0 )
        {
            m_omComboLinProtocol.SetCurSel(nIndex);
        }
    }
    return 0;
}

INT CChannelConfigurationDlg::nUpdateEcuList( INT nChannelIndex, INT nClusterIndex )
{
    INT nResult = S_OK;
    //todo::
    list<Cluster>::iterator itrCluster =  m_ouCurrentChannelCluster.begin();
    advance(itrCluster, nClusterIndex);
    if ( itrCluster != m_ouCurrentChannelCluster.end() )
    {
        map<ECU_ID, ECU_Struct>::iterator itrECU = itrCluster->m_ouEcuList.begin();
        m_omEcuList.DeleteAllItems();
        int i = 0;
        for ( ; itrECU != itrCluster->m_ouEcuList.end(); itrECU++ )
        {
            m_omEcuList.InsertItem(i, itrECU->second.m_strECUName.c_str());
            //m_omEcuList.SetItemText( i , 0,itrECU->second.m_strECUName.c_str());
            //m_omEcuList.SetItemText( i , 1,itrECU->second.m_strECUName.c_str());
            i++;
        }
    }


    return nResult;
}

INT CChannelConfigurationDlg::nUpdateEcuList( Cluster& ouCluster )
{
    INT nResult = S_OK;

    list<ECU_Struct> ouEcuList;
    ouCluster.GetECUList(ouEcuList);
    INT nIndex = 0;
    for ( list<ECU_Struct>::iterator itrEcu; itrEcu!= ouEcuList.end(); itrEcu++ )
    {
        m_omEcuList.InsertItem(nIndex, itrEcu->m_strECUName.c_str() );
        m_omEcuList.SetCheck(nIndex);
        nIndex++;
    }

    return nResult;
}
int CChannelConfigurationDlg::nSaveProtocolSettings(int nIndex)
{
    UpdateData(TRUE);
    if ( m_eBusType == LIN )
    {
        int nSel = ((CComboBox*)GetDlgItem(IDC_COMBO_LIN_PROTOCOL))->GetCurSel();
        if ( nSel >= 0)
        {
            m_ouFlexrayChannelConfig[m_nCurrentChannel].m_ouLinParams.m_strProtocolVersion = sg_LINPROTOCOL_BAUD[nSel].m_strProtocol;
            m_ouFlexrayChannelConfig[m_nCurrentChannel].m_ouLinParams.m_nBaudRate = m_nLinBaudRate;
            m_ouFlexrayChannelConfig[m_nCurrentChannel].m_ouLinParams.m_bOverWriteSettings = ((CButton*)GetDlgItem(IDC_CHECK_OVERWRITE_SETTINGS))->GetCheck();
        }


        m_ouFlexrayChannelConfig[m_nCurrentChannel].m_strDataBasePath = m_strFibexFilePath;


    }
    else
    {

    }
    return 0;
}


void CChannelConfigurationDlg::onBtnOk()
{
    //Save Current Values to the old Channel
    if ( -1 != m_nCurrentChannel )
    {

        list<Cluster>::iterator itFlexConfig = m_ouCurrentChannelCluster.begin();

        advance(itFlexConfig, m_omComboCluster.GetCurSel());

        if ( itFlexConfig != m_ouCurrentChannelCluster.end() )
        {
            if (  m_nCurrentChannel < m_nChannelConfigured )
            {
                //1. Save Cluster Info
                m_ouFlexrayChannelConfig[m_nCurrentChannel].m_ouClusterInfo = *itFlexConfig;

                //2. Save ECU Info
                m_ouFlexrayChannelConfig[m_nCurrentChannel].m_strSlectedEculist.clear();


                list<ECU_Struct> ouEcuList;
                m_ouFlexrayChannelConfig[m_nCurrentChannel].m_ouClusterInfo.GetECUList(ouEcuList);
                list<ECU_Struct>::iterator itrEcu = ouEcuList.begin();


                int nEcuCount = m_omEcuList.GetItemCount();
                for ( int i = 0; i < nEcuCount; i++ )
                {
                    itrEcu = ouEcuList.begin();
                    if ( m_omEcuList.GetCheck(i) == TRUE )
                    {
                        advance(itrEcu, i);
                        if ( itrEcu != ouEcuList.end())
                        {
                            m_ouFlexrayChannelConfig[m_nCurrentChannel].m_strSlectedEculist.push_back( itrEcu->m_strEcuId);
                        }
                    }
                }

                //3. Fibex Path
                m_ouFlexrayChannelConfig[m_nCurrentChannel].m_strDataBasePath = m_strFibexFilePath;

                //Protocol;
                nSaveProtocolSettings(m_nCurrentChannel);
            }
        }
    }
    OnOK();
}
