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
#include "FibexConfigurationDlg.h"
#include "FibexClass_extern.h"
#include "Utility\WaitIndicator.h"

IMPLEMENT_DYNAMIC(CFibexConfigDlg, CDialog)
CFibexConfigDlg::CFibexConfigDlg(CMsgSignal*& pMsgSignal ,FLEXRAY_CHANNEL_CONFIG ouFlexrayChannelConfig[], INT& nChannelConfigured, CWnd* pParent /*=NULL*/)
    : CDialog(CFibexConfigDlg::IDD, pParent)
{
    m_nChannelConfigured = nChannelConfigured;
    m_pMsgSignal = pMsgSignal;
    for ( int i = 0 ; i < nChannelConfigured; i++ )
    {
        m_ouFlexrayChannelConfig[i] = ouFlexrayChannelConfig[i];
    }
    m_nCurrentChannel = -1;
}

CFibexConfigDlg::~CFibexConfigDlg()
{
}

void CFibexConfigDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_EDIT_FIBEX_PATH, m_strFibexFilePath);
    DDX_Control(pDX, IDC_EDIT_FIBEX_PATH, m_omFibexPath);
    DDX_Control(pDX, IDC_COMBO_CHANNEL, m_ComboChannelSelect);
    DDX_Control(pDX, IDC_LIST_ECU, m_omEcuList);
    DDX_Control(pDX, IDC_COMBO_CLUSTER, m_omComboCluster);
}


BEGIN_MESSAGE_MAP(CFibexConfigDlg, CDialog)
    ON_WM_SYSCOMMAND()
    ON_WM_PAINT()
    ON_WM_QUERYDRAGICON()
    //}}AFX_MSG_MAP
    ON_BN_CLICKED(IDC_BUTTON_FIBEXPATH, OnBnClickedButtonFibexpath)
    ON_CBN_SELCHANGE(IDC_COMBO_CLUSTER, OnCbnSelchangeComboCluster)
    ON_CBN_SELCHANGE(IDC_COMBO_CHANNEL, OnCbnSelchangeComboChannel)
    ON_BN_CLICKED(IDOK, onBtnOk)
END_MESSAGE_MAP()

BOOL CFibexConfigDlg::OnInitDialog()
{


    CDialog::OnInitDialog();




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
    /*list<FLEXRAY_CHANNEL_CONFIG>::iterator itrFlexrayChannelConfig = m_ouFlexrayChannelConfig.begin();
    if ( itrFlexrayChannelConfig != m_ouFlexrayChannelConfig.end() )
    {
        list<Cluster> ouClusterList;
        if ( itrFlexrayChannelConfig->m_strFibexPath.c_str() != "")
        {
            m_pMsgSignal->hLoadFibexDBFile(itrFlexrayChannelConfig->m_strFibexPath.c_str(), ouClusterList);
            m_ouCurrentChannelCluster = ouClusterList;
        }

    }*/

    return TRUE;
}

void CFibexConfigDlg::OnBnClickedButtonFibexpath()
{
    CFileDialog omFibexDlg(TRUE, ".xml", 0, OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, "FIBEX Files (*.xml)|*.xml||", this);
    omFibexDlg.m_ofn.lpstrTitle = "Select A FIBEX File";

    if ( omFibexDlg.DoModal() == IDOK )
    {
        CString strPath = omFibexDlg.GetPathName();
        list<Cluster> ouClusterList;
        CWaitIndicator ouWaitIndicator;
        ouWaitIndicator.DisplayWindow("Parsing Fibex File. Please Wait...", this);

        int nResult = m_pMsgSignal->hLoadFibexDBFile(strPath, ouClusterList);

        ouWaitIndicator.CloseWindow();
        m_nCurrentChannel = m_ComboChannelSelect.GetCurSel();

        if ( nResult == FCLASS_SUCCESS && ouClusterList.size() > 0 )
        {
            m_omComboCluster.ResetContent();
            //list<FLEXRAY_CHANNEL_CONFIG>::iterator itrChannelConfig = m_ouFlexrayChannelConfig.begin();
            //advance(itrChannelConfig, m_nCurrentChannel );
            //if ( itrChannelConfig != m_ouFlexrayChannelConfig.end() )
            {
                m_ouCurrentChannelCluster = ouClusterList;
                list<Cluster>::iterator itrCluster = ouClusterList.begin();
                //itrChannelConfig->m_ouClusterInfo = *itrCluster;

                for ( ; itrCluster != ouClusterList.end(); itrCluster++)
                {
                    m_omComboCluster.AddString(itrCluster->m_strName.c_str());
                }
                m_omComboCluster.SetCurSel(0);
                nUpdateEcuList(0, 0);
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
                MessageBox("Invalid FIBEX File", "Error", MB_OK | MB_ICONERROR );
            }
        }
        UpdateData(FALSE);
    }

}

void CFibexConfigDlg::OnCbnSelchangeComboCluster()
{
    // TODO: Add your control notification handler code here

    //  nUpdateEcuList(0, 0);
}

void CFibexConfigDlg::OnCbnSelchangeComboChannel()
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
                m_ouFlexrayChannelConfig[m_nCurrentChannel].m_strFibexPath = m_strFibexFilePath;

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
        m_strFibexFilePath = m_ouFlexrayChannelConfig[nSelcetedIndex].m_strFibexPath.c_str();

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
    }
    UpdateData(FALSE);
}
bool CFibexConfigDlg::bIsEcuSlected(list<string>& ouEcuList, string strEcuName)
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


INT CFibexConfigDlg::nUpdateEcuList( INT nChannelIndex, INT nClusterIndex )
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

INT CFibexConfigDlg::nUpdateEcuList( Cluster& ouCluster )
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
void CFibexConfigDlg::onBtnOk()
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
                m_ouFlexrayChannelConfig[m_nCurrentChannel].m_strFibexPath = m_strFibexFilePath;

            }
        }
    }
    OnOK();
}