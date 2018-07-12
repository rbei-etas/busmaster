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

#pragma once
#include "resource.h"
#include "IBMNetWorkService.h"
#include "BusMasterNetWork.h"
void vUpdateScheduleTableList( ChannelSettings& ouChannelSettings, unsigned int unChannel, IBMNetWorkService* pBMNetwork );
eCommandType vGetScheduleCommandType(eDiagType oueDiagType);


class CChannelConfigurationDlg : public CDialog
{
    DECLARE_DYNAMIC(CChannelConfigurationDlg)

public:
    CChannelConfigurationDlg( IBMNetWorkService* pBmNetWork, ETYPE_BUS eBusType, CWnd* pParent =nullptr);
    virtual ~CChannelConfigurationDlg();

    // Dialog Data
    enum { IDD = IDD_DLG_FIBEX_IMPORT };

    IBMNetWorkService* m_pBmNetWork;

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    virtual BOOL OnInitDialog();
    DECLARE_MESSAGE_MAP()

private:
    INT m_nChannelConfigured;
    ETYPE_BUS m_eBusType;
    std::list<ICluster*> m_ouCurrentChannelCluster;
    //std::list<LinChannelParam> m_ouLinChannelParams;
    int m_nLinBaudRate;


    //UI Related
    CString m_strFibexFilePath;
    CComboBox m_ComboChannelSelect;
    CComboBox m_ComboColdStartMode1;
    CComboBox m_ComboColdStartMode2;
    CComboBox m_ComboKeySlotUsage1;
    CComboBox m_ComboKeySlotUsage2;
    CComboBox m_ComboKeySlot1;
    CComboBox m_ComboKeySlot2;
    CComboBox m_ComboChannelMask1;
    CComboBox m_ComboChannelMask2;
    CListCtrl m_omEcuList;
    INT m_nCurrentChannel;
    CComboBox m_omComboCluster;
    CComboBox m_omComboLinProtocol;
    CButton m_LeadingColdStart;
    CButton m_LeadingColdStart1;
    CEdit m_omFibexPath;

	std::string m_omStrPreviousDb;
public:
    afx_msg void OnBnClickedButtonFibexpath();
    afx_msg void OnBnClickedButtonLDFEditor();
    afx_msg void OnCbnSelchangeComboCluster();
    afx_msg void OnCbnSelchangeComboChannel();
    afx_msg void OnCbnSelchangeComboProtocol();
    afx_msg void OnCbnSelchangeComboKeySlotMode();
    afx_msg void OnCbnSelchangeComboKeySlotMode1();
    afx_msg void onBtnOk();
    afx_msg void OnOverwriteCheckBoxClick();

    INT GetCurrentChannel();

private:
    INT nUpdateLinParams( INT nChannelIndex, INT nClusterIndex);
    bool bIsEcuSlected(std::list<IEcu*>& ouEcuList, IEcu* EcuName);
    int nEnableControls( ETYPE_BUS eBusType );
    int nUpdateLinSettings();
    int nDisplayProtocolSettings(int nChannelIndex);
    int nSaveProtocolSettings(int nIndex);
    int nUpdateEcuList(INT /* nChannelIndex */, INT nClusterIndex);
};
