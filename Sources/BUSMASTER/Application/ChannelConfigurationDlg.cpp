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

#include "Utility\WaitIndicator.h"
#include "LinChannelParam.h"
struct LinProcolBaudRate
{
    std::string m_strProtocol;
    int m_nBaudRate;
};

static LinProcolBaudRate sg_LINPROTOCOL_BAUD[] =
{
    { "LIN 2.2", 19200 },
    { "LIN 2.1", 19200 },
    { "LIN 2.0", 19200 },
    { "LIN 1.3", 9600 },
    { "LIN 1.2", 9600 },
    { "LIN 1.1", 9600 },
};


IMPLEMENT_DYNAMIC(CChannelConfigurationDlg, CDialog)
CChannelConfigurationDlg::CChannelConfigurationDlg(IBMNetWorkService* pBmService, ETYPE_BUS eBusType, CWnd* pParent /*=nullptr*/)
    : CDialog(CChannelConfigurationDlg::IDD, pParent)
{
    m_eBusType = eBusType;
    m_pBmNetWork = pBmService;
    m_nCurrentChannel = -1;
	m_omStrPreviousDb = "";

    pBmService->GetChannelCount(eBusType, m_nChannelConfigured);
    //Atleast one channel should configued
    if (m_nChannelConfigured == 0)
    {
        m_nChannelConfigured = 1;
    }

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
    ON_BN_CLICKED(IDC_BUTTON_FIBEXPATH, OnBnClickedButtonFibexpath)
    ON_BN_CLICKED(IDC_BUTTON_LDF_EDIT, OnBnClickedButtonLDFEditor)
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
    if (nullptr != pTempBtn)
    {
        bCheck = (pTempBtn->GetCheck() != 0);
    }

    CWnd* pTempChild = GetDlgItem(IDC_COMBO_LIN_PROTOCOL);
    if (nullptr != pTempChild)
    {
        pTempChild->EnableWindow(bCheck);
    }

    pTempChild = GetDlgItem(IDC_EDIT_LIN_BAUDRATE);
    if (nullptr != pTempChild)
    {
        pTempChild->EnableWindow(bCheck);
    }
}

BOOL CChannelConfigurationDlg::OnInitDialog()
{
    CDialog::OnInitDialog();

    //Validate The Bus Type - LIN
    if (m_eBusType != LIN)
    {
        return S_FALSE;
    }

    //Controls Initialisation

    m_omEcuList.SetExtendedStyle(LVS_REPORT);

    m_omEcuList.InsertColumn(0, "ECU List");
    // Set the width to occupy the whole list
    CRect omRect;
    m_omEcuList.GetWindowRect(&omRect);
    int nWidth = static_cast<int>(omRect.Width() * 0.95);

    // Set Col Width
    m_omEcuList.SetColumnWidth(0, nWidth);

    char chTemp[MAX_PATH];

    m_ComboChannelSelect.Clear();
    for (int i = 0; i < m_nChannelConfigured; i++)
    {
        sprintf_s(chTemp, "Channel %d", i + 1);
        m_ComboChannelSelect.AddString(chTemp);
    }

    //Update for 1st Channel
    m_nCurrentChannel = -1;
    m_ComboChannelSelect.SetCurSel(0);
    OnCbnSelchangeComboChannel();

    nUpdateLinSettings();
	
    nDisplayProtocolSettings(0);
    nEnableControls(m_eBusType);
    if (m_eBusType == LIN)
    {
        GetDlgItem(IDC_BUTTON_LDF_EDIT)->ShowWindow(TRUE);
        GetDlgItem(IDC_BUTTON_LDF_EDIT)->EnableWindow(TRUE);

		m_omStrPreviousDb = m_strFibexFilePath;
    }
    return TRUE;
}

int CChannelConfigurationDlg::nUpdateLinSettings()
{
    CComboBox* pomCombo = (CComboBox*)GetDlgItem(IDC_COMBO_LIN_PROTOCOL);

    if (pomCombo != nullptr)
    {
        for (int i = 0; i < (sizeof(sg_LINPROTOCOL_BAUD) / sizeof(sg_LINPROTOCOL_BAUD[0])); i++)
        {
            pomCombo->InsertString(i, sg_LINPROTOCOL_BAUD[i].m_strProtocol.c_str());
        }
        pomCombo->SetCurSel(0);
    }

    m_nLinBaudRate = 1900;
    UpdateData(FALSE);
    OnOverwriteCheckBoxClick();

    return 0;
}


int CChannelConfigurationDlg::nEnableControls(ETYPE_BUS eBusType)
{
    CWnd* pWnd = nullptr;
    if (eBusType == LIN)
    {
        pWnd = GetDlgItem(IDC_STATIC_DATABASE_INFO);
        if (pWnd != nullptr)
        {
            pWnd->SetWindowText("Import LIN Database (LDF)");
        }

        pWnd = GetDlgItem(IDC_STATIC_CLUSTER_INFO);
        if (pWnd != nullptr)
        {
            pWnd->SetWindowText("Select LIN Cluster:");
        }

        pWnd = GetDlgItem(IDC_STATIC_EXTRA_CONFIG);
        if (pWnd != nullptr)
        {
            pWnd->SetWindowText("LIN Network Settings");
        }

        pWnd = GetDlgItem(IDC_STATIC_DBNAME);
        if (pWnd != nullptr)
        {
            pWnd->SetWindowText("LDF:");
        }

        pWnd = GetDlgItem(IDC_COMBO_CLUSTER);
        if (pWnd != nullptr)
        {
            pWnd->EnableWindow(FALSE);
        }
    }

    return S_OK;
}

/*
 * \req      RS_FLX_11
 * \design   DS_FLX_11
 * \codetag  CT_FLX_11
 */
void CChannelConfigurationDlg::OnBnClickedButtonFibexpath()
{
    CFileDialog* pomFibexDlg = nullptr;
    std::string strWaitText;
    if (m_eBusType == LIN)
    {
        pomFibexDlg = new CFileDialog(TRUE, ".xml", 0, OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, "LDF Files (*.ldf)|*.ldf||", this);
        pomFibexDlg->m_ofn.lpstrTitle = "Select A LDF File";
        strWaitText = "Parsing LDF File. Please Wait...";
    }

    if ((pomFibexDlg != nullptr) && (pomFibexDlg->DoModal() == IDOK))
    {
        CString strPath = pomFibexDlg->GetPathName();
        std::list<ICluster*> ouClusterList;
        std::list<LinChannelParam> ouLinChannelParams;
        CWaitIndicator ouWaitIndicator;
        ouWaitIndicator.DisplayWindow(strWaitText.c_str(), this);

        int nCount = 0;
        ERRORCODE ecCode = m_pBmNetWork->GetChannelCount(m_eBusType, nCount);

		/*ERRORCODE ecDBLoad = m_pBmNetWork->LoadDb(LIN, 0, (LPCSTR)strPath);

		if (ecDBLoad == EC_SUCCESS)
		{
			m_pBmNetWork->GetDBServiceList(LIN, 0, ouClusterList);
		}*/

        ecCode = m_pBmNetWork->ParseDbFile((LPCSTR)strPath, m_eBusType, ouClusterList);

		////Load First Cluster Only
		//if (ouClusterList.size() > 0)
		//{
		//	m_pBmNetWork->AddDBService(LIN, 0, *(ouClusterList.begin()));
		//}

        ouWaitIndicator.CloseWindow();
        m_nCurrentChannel = m_ComboChannelSelect.GetCurSel();

        if ((ecCode != EC_FAILURE) && (!ouClusterList.empty()))
        {
            m_omComboCluster.ResetContent();
            //advance(itrChannelConfig, m_nCurrentChannel );

            {
                m_ouCurrentChannelCluster = ouClusterList;
                //m_ouLinChannelParams = ouLinChannelParams;
                std::string strName;
for (auto itrCluster : ouClusterList)
                {
                    itrCluster->GetName(strName);
                    std::string strFile;
                    itrCluster->GetDBFilePath(strFile);
                    m_omComboCluster.AddString(strName.c_str());
                }
                m_omComboCluster.SetCurSel(0);

                if (m_omComboCluster.GetCount() <= 1)
                {
                    m_omComboCluster.EnableWindow(FALSE);
                }
                nUpdateEcuList(0, 0);
                nUpdateLinParams(0, 0);
                m_strFibexFilePath = strPath;
            }
        }
        else
        {
            MessageBox("Invalid Input File or Unsupported Version", "Error", MB_OK | MB_ICONERROR);
        }
        UpdateData(FALSE);
    }

}
void CChannelConfigurationDlg::OnBnClickedButtonLDFEditor()
{
    try
    {
        // Get the working directory
        char acPath[MAX_PATH] = "";
        GetModuleFileName(nullptr, acPath, MAX_PATH);
        PathRemoveFileSpec(acPath);
        CString strPath = acPath;
        strPath += "\\LDFEditor.exe";

        if (PathFileExists(strPath) == TRUE)
        {
            // Launch the converter utility
            PROCESS_INFORMATION sProcessInfo;
            STARTUPINFO sStartInfo;

            memset(&sProcessInfo, 0, sizeof(PROCESS_INFORMATION));
            memset(&sStartInfo, 0, sizeof(STARTUPINFO));
            std::string strCmdline = "\"" + std::string(strPath) + "\"" + " " + "\"" + std::string(m_strFibexFilePath) + "\"";
            // \" is added to allow spaces in the file path.
            int nSuccess = CreateProcess(strPath.GetBuffer(MAX_PATH), (LPSTR)strCmdline.c_str(),
                                         nullptr, nullptr, true, CREATE_NO_WINDOW, nullptr, nullptr,
                                         &sStartInfo, &sProcessInfo);
            if (!nSuccess)
            {
                AfxMessageBox("Unable to launch LDF Editor.", MB_ICONSTOP | MB_OK);
            }
        }
    }
    catch (...)
    {
    }
}
void CChannelConfigurationDlg::OnCbnSelchangeComboCluster()
{
}

void CChannelConfigurationDlg::OnCbnSelchangeComboProtocol()
{
    int nSel = m_omComboLinProtocol.GetCurSel();

    if (nSel >= 0)
    {
        m_nLinBaudRate = sg_LINPROTOCOL_BAUD[nSel].m_nBaudRate;
        UpdateData(FALSE);
    }
}


void CChannelConfigurationDlg::OnCbnSelchangeComboKeySlotMode()
{
    int nSel = m_ComboColdStartMode1.GetCurSel();

    if (nSel == 1) // Key Slot Off
    {
        m_ComboKeySlot1.EnableWindow(FALSE);
        m_ComboKeySlotUsage1.EnableWindow(FALSE);
        m_ComboChannelMask1.EnableWindow(FALSE);
        m_LeadingColdStart.EnableWindow(FALSE);
    }
    else
    {
        m_ComboKeySlot1.EnableWindow(TRUE);
        m_ComboKeySlotUsage1.EnableWindow(TRUE);
        m_ComboChannelMask1.EnableWindow(TRUE);
        m_LeadingColdStart.EnableWindow(TRUE);
    }
}

void CChannelConfigurationDlg::OnCbnSelchangeComboKeySlotMode1()
{
    // second key slot
    int nSel = m_ComboColdStartMode2.GetCurSel();

    if (nSel == 1) // Key Slot Off
    {
        m_ComboKeySlot2.EnableWindow(FALSE);
        m_ComboKeySlotUsage2.EnableWindow(FALSE);
        m_ComboChannelMask2.EnableWindow(FALSE);
        m_LeadingColdStart1.EnableWindow(FALSE);
    }
    else
    {
        m_ComboKeySlot2.EnableWindow(TRUE);
        m_ComboKeySlotUsage2.EnableWindow(TRUE);
        m_ComboChannelMask2.EnableWindow(TRUE);
        m_LeadingColdStart1.EnableWindow(TRUE);
    }
}

void CChannelConfigurationDlg::OnCbnSelchangeComboChannel()
{
    //Update the selected channel Information

    INT nSelcetedIndex = m_ComboChannelSelect.GetCurSel();
    if (m_nCurrentChannel == nSelcetedIndex)
    {
        return;
    }
    m_nCurrentChannel = nSelcetedIndex;
    m_ouCurrentChannelCluster.clear();
    m_omEcuList.DeleteAllItems();
    if (nSelcetedIndex < m_nChannelConfigured)
    {
        ICluster* pCluster;
        std::string strFile;
        m_pBmNetWork->GetDBService(m_eBusType, nSelcetedIndex, 0, &pCluster);
        if (nullptr != pCluster)
        {
            std::string strName;
            pCluster->GetDBFilePath(strFile);
            pCluster->GetName(strName);
            m_omComboCluster.Clear();
            m_omComboCluster.AddString(strName.c_str());

            m_strFibexFilePath = strFile.c_str();
            m_ouCurrentChannelCluster.push_back(pCluster);


            m_omComboCluster.SetCurSel(0);
            std::list<IEcu*> ouEcuList;
            std::list<IEcu*> ouSelectedEcus;

            pCluster->GetEcuList(ouEcuList);

            m_pBmNetWork->GetSimulatedEcuList(m_eBusType, nSelcetedIndex, ouSelectedEcus);
            INT nIndex = 0;
for (auto itrEcu : ouEcuList)
            {
                itrEcu->GetName(strName);
                m_omEcuList.InsertItem(nIndex, strName.c_str());
                m_omEcuList.SetItemData(nIndex, itrEcu->GetUniqueId());
                if (bIsEcuSlected(ouSelectedEcus, itrEcu))
                {
                    m_omEcuList.SetCheck(nIndex);
                }
            }
            nIndex++;
        }
        nDisplayProtocolSettings(nSelcetedIndex);
    }
    UpdateData(FALSE);
}

int CChannelConfigurationDlg::nDisplayProtocolSettings(int nChannelIndex)
{
    if (m_eBusType == LIN)
    {
        ChannelSettings ouChannelSettings;

        m_pBmNetWork->GetChannelSettings(LIN, nChannelIndex, &ouChannelSettings);
        LINChannelSettings ouLinParams = ouChannelSettings.m_ouLINSettings;
        BOOL bOverrite = ouLinParams.m_bOverWriteSettings;

        ((CButton*)GetDlgItem(IDC_CHECK_OVERWRITE_SETTINGS))->SetCheck(bOverrite);

        CComboBox* pomCombo = (CComboBox*)GetDlgItem(IDC_COMBO_LIN_PROTOCOL);
        pomCombo->EnableWindow(bOverrite);

        GetDlgItem(IDC_EDIT_LIN_BAUDRATE)->EnableWindow(bOverrite);

        m_nLinBaudRate = 19200;
        pomCombo->SetCurSel(0);
        for (int i = 0; i < (sizeof(sg_LINPROTOCOL_BAUD) / sizeof(sg_LINPROTOCOL_BAUD[0])); i++)
        {
            if (ouLinParams.m_strProtocolVersion == sg_LINPROTOCOL_BAUD[i].m_strProtocol)
            {
                pomCombo->SetCurSel(i);
                if (bOverrite == false)
                {
                    m_nLinBaudRate = sg_LINPROTOCOL_BAUD[i].m_nBaudRate;
                }
                else
                {
                    m_nLinBaudRate = ouLinParams.m_nBaudRate;
                }

                break;
            }
        }
        ((CButton*)GetDlgItem(IDC_ENABLE_MASTER_MODE))->SetCheck(ouLinParams.m_bIsMasterMode);
        UpdateData(FALSE);
    }
    return 0;
}


bool CChannelConfigurationDlg::bIsEcuSlected(std::list<IEcu*>& ouEcuList, IEcu* EcuName)
{
    bool bFound = false;
for (auto itrEcu : ouEcuList)
    {
        if (itrEcu->GetUniqueId() == EcuName->GetUniqueId())
        {
            bFound = true;
        }
    }
    return bFound;
}

INT CChannelConfigurationDlg::nUpdateLinParams(INT /* nChannelIndex */, INT /*nClusterIndex*/)
{
	return 0;
}

INT CChannelConfigurationDlg::nUpdateEcuList(INT /* nChannelIndex */, INT nClusterIndex)
{
    std::list<ICluster*>::iterator itrCluster = m_ouCurrentChannelCluster.begin();
    advance(itrCluster, nClusterIndex);
    std::string strName;
    if (itrCluster != m_ouCurrentChannelCluster.end())
    {
        m_omEcuList.DeleteAllItems();
        int i = 0;
        std::map<UID_ELEMENT, IElement*> ouEcuList;
        (*itrCluster)->GetElementList(eEcuElement, ouEcuList);

        std::map<int, int> mapSlots;

for (auto itrECU : ouEcuList)
        {
            (itrECU).second->GetName(strName);

            m_omEcuList.InsertItem(i, strName.c_str());
            m_omEcuList.SetItemData(i, itrECU.first);
        }
    }

    return 0;
}
int CChannelConfigurationDlg::nSaveProtocolSettings(int /* nIndex */)
{
    UpdateData(TRUE);
    if (m_eBusType == LIN)
    {
        ChannelSettings ouChannelSettings;
        int nSel = ((CComboBox*)GetDlgItem(IDC_COMBO_LIN_PROTOCOL))->GetCurSel();
        if (nSel >= 0)
        {

            ouChannelSettings.m_ouLINSettings.m_strProtocolVersion = sg_LINPROTOCOL_BAUD[nSel].m_strProtocol;
            ouChannelSettings.m_ouLINSettings.m_nBaudRate = m_nLinBaudRate;
            ouChannelSettings.m_ouLINSettings.m_bOverWriteSettings = (((CButton*)GetDlgItem(IDC_CHECK_OVERWRITE_SETTINGS))->GetCheck() != 0);
            ouChannelSettings.m_ouLINSettings.m_bIsMasterMode = (((CButton*)GetDlgItem(IDC_ENABLE_MASTER_MODE))->GetCheck() !=0);

            vUpdateScheduleTableList(ouChannelSettings, m_nCurrentChannel, (IBMNetWorkService*)m_pBmNetWork);
        }

        m_pBmNetWork->SetChannelSettings(LIN, m_nCurrentChannel, &ouChannelSettings);
    }

    return 0;
}

void vUpdateScheduleTableList(ChannelSettings& ouChannelSettings, unsigned int unChannel, IBMNetWorkService* pBMNetwork)
{
    ICluster* pCluster;
    pBMNetwork->GetDBService(LIN, unChannel, 0, &pCluster);

    if (nullptr != pCluster)
    {
        std::map<UID_ELEMENT, IElement*> mapScheduleTables;
        std::list<IScheduleTable*> lstScheduleTables;

        pCluster->GetElementList(eScheduleTableElement, mapScheduleTables);

for (auto itrScheduleTable : mapScheduleTables)
        {
            CSheduleTable ouScheduleTable;
            itrScheduleTable.second->GetName(ouScheduleTable.m_strTableName);

            ScheduleTableProps ouScheduleTableProps;
            ((IScheduleTable*)itrScheduleTable.second)->GetProperties(ouScheduleTableProps);

for (auto itrSchedCmd : ouScheduleTableProps.m_ouCLINSheduleTableItem)
            {
                CScheduleCommands ouScheduleCmd;
                ouScheduleCmd.m_eCommandType = vGetScheduleCommandType(itrSchedCmd.m_eDiagType);

                IFrame* pFrame = nullptr;
                IEcu* pECU = nullptr;
                pCluster->GetElement(eFrameElement, itrSchedCmd.m_nFrameId, (IElement**)&pFrame);
                pCluster->GetElement(eEcuElement, itrSchedCmd.m_nNode, (IElement**)&pECU);

                if (nullptr != pFrame)
                {
                    pFrame->GetName(ouScheduleCmd.m_strCommandName);
                    unsigned int unFrameId;
                    pFrame->GetFrameId(unFrameId);

                    ouScheduleCmd.m_nId = unFrameId;
                }

                if (nullptr != pECU)
                {
                    pECU->GetName(ouScheduleCmd.m_strNodeName);
                }

                ouScheduleCmd.m_fDelay = itrSchedCmd.m_dDelay;
                memcpy(ouScheduleCmd.m_listIDs, itrSchedCmd.m_chDataBytes, MAX_LIN_FRAME_LENGTH);

                ouScheduleTable.m_listCommands.push_back(ouScheduleCmd);
            }
            ouChannelSettings.m_ouLINSettings.ouLinParams.push_back(ouScheduleTable);
            //lstScheduleTables.push_back((IScheduleTable*)itrScheduleTable.second);
        }
    }
}

eCommandType vGetScheduleCommandType(eDiagType oueDiagType)
{
    switch (oueDiagType)
    {
        case eLIN_NORMAL_FRAME_ID:
            return COMMAND_UNCONDITIONAL;
            break;
        case eLIN_MASTER_FRAME_ID:
            return COMMAND_MASTERREQ;
            break;
        case eLIN_SLAVE_FRAME_ID:
            return COMMAND_SLAVE;
            break;
        case eLIN_SID_ASSIGN_FRAME_ID:
            return COMMAND_ASSIGN_FRAME_ID;
            break;
        case eLIN_SID_UNASSIGN_FRAME_ID:
            return COMMAND_UNASSIGN_FRAME_ID;
            break;
        case eLIN_SID_ASSIGN_NAD_ID:
            return COMMAND_ASSIGN_NAD;
            break;
        case eLIN_SID_COND_CHANGE_NAD:
            return COMMAND_CONDITIONAL_CHANGE_NAD;
            break;
        case eLIN_SID_DATA_DUMP:
            return COMMAND_DATA_DUMP;
            break;
        case eLIN_SID_SAVE_CONFIG:
            return COMMAND_SAVE_CONFIGURATION;
            break;
        case eLIN_SID_ASSIGN_FRAME_RANGE:
            return COMMAND_ASSIGN_FRAME_ID_RANGE;
            break;
        case eLIN_SID_FREEFORMAT:
            return COMMAND_FREE_FORMAT;
            break;
        case eLIN_SID_READ_BY_IDENTIFIER:
            return COMMAND_READ_BY_IDENTIFIER;
            break;
        case eLIN_SID_ALL:
            return COMMAND_INVALID;
            break;
        case eLIN_SID_NONE:
            return COMMAND_INVALID;
            break;
        default:
			return COMMAND_INVALID;
            break;
    }
}

void CChannelConfigurationDlg::onBtnOk()
{
    //Save Current Values to the old Channel
    if (-1 != m_nCurrentChannel)
    {		

        std::list<ICluster*>::iterator itFlexConfig = m_ouCurrentChannelCluster.begin();
        if (itFlexConfig != m_ouCurrentChannelCluster.end())
        {
            advance(itFlexConfig, m_omComboCluster.GetCurSel());
        }
        if (itFlexConfig != m_ouCurrentChannelCluster.end())
        {
			std::string strCurrentDb = "";
			(*itFlexConfig)->GetDBFilePath(strCurrentDb);
			if (m_omStrPreviousDb != "" && strCurrentDb != m_omStrPreviousDb)
			{
				m_pBmNetWork->DeleteDBService(LIN, m_nCurrentChannel, m_omStrPreviousDb);
			}

            if (m_nCurrentChannel < m_nChannelConfigured)
            {
				if (strCurrentDb != m_omStrPreviousDb)
				{
					//Load First Cluster Only
					m_pBmNetWork->AddDBService(LIN, 0, (*itFlexConfig));
				}

                m_pBmNetWork->SetChannelCount(m_eBusType, 1);
                m_pBmNetWork->SetDBService(m_eBusType, m_nCurrentChannel, 0, (*itFlexConfig));


                ChannelSettings ouSettings;
                std::list<IEcu*> ouEcuList;
                int nEcuCount = m_omEcuList.GetItemCount();
                IElement* ouEcu;
                for (int i = 0; i < nEcuCount; i++)
                {
                    //For LIN ECU selection is not considered.
                    if (m_omEcuList.GetCheck(i) == TRUE || LIN == m_eBusType)
                    {
                        UID_ELEMENT uidElement = static_cast<UID_ELEMENT>(m_omEcuList.GetItemData(i));
                        (*itFlexConfig)->GetElement(eEcuElement, uidElement, &ouEcu);
                        if (nullptr != ouEcu)
                        {
                            ouEcuList.push_back((IEcu*)ouEcu);
                        }
                    }
                }

                m_pBmNetWork->SetSimulatedEcuList(m_eBusType, m_nCurrentChannel, ouEcuList);

				
            }
        }

        //Protocol;
        nSaveProtocolSettings(m_nCurrentChannel);
    }
    OnOK();
}


INT CChannelConfigurationDlg::GetCurrentChannel()
{
    return m_nCurrentChannel;
}