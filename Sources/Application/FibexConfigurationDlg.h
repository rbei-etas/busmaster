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
#include "MsgSignal.h"

class CFibexConfigDlg : public CDialog
{
    DECLARE_DYNAMIC(CFibexConfigDlg)

public:
    CFibexConfigDlg( CMsgSignal*& pMsgSignal ,CHANNEL_CONFIG ouFlexrayChannelConfig[], INT& m_nChannelConfigured, ETYPE_BUS eBusType, CWnd* pParent =NULL);
    virtual ~CFibexConfigDlg();

    // Dialog Data
    enum { IDD = IDD_DLG_FIBEX_IMPORT };
    CHANNEL_CONFIG m_ouFlexrayChannelConfig[CHANNEL_ALLOWED];
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    virtual BOOL OnInitDialog();
    DECLARE_MESSAGE_MAP()

private:
    INT m_nChannelConfigured;
    CMsgSignal* m_pMsgSignal;
    ETYPE_BUS m_eBusType;
    list<Cluster> m_ouCurrentChannelCluster;
    list<LinChannelParam> m_ouLinChannelParams;
    int m_nLinBaudRate;


    //UI Related
    CString m_strFibexFilePath;
    CComboBox m_ComboChannelSelect;
    CListCtrl m_omEcuList;
    INT m_nCurrentChannel;
    CComboBox m_omComboCluster;
    CComboBox m_omComboLinProtocol;
    CEdit m_omFibexPath;

public:
    afx_msg void OnBnClickedButtonFibexpath();
    afx_msg void OnCbnSelchangeComboCluster();
    afx_msg void OnCbnSelchangeComboChannel();
    afx_msg void OnCbnSelchangeComboProtocol();
    afx_msg void onBtnOk();
    afx_msg void OnOverwriteCheckBoxClick();

private:
    INT nUpdateEcuList( INT nChannelIndex, INT nClusterIndex );
    INT nUpdateLinParams( INT nChannelIndex, INT nClusterIndex);
    INT nUpdateEcuList( Cluster& ouCluster );
    bool bIsEcuSlected(list<string>& ouEcuList, string strEcuName);
    int nEnableControls( ETYPE_BUS eBusType );
    int nUpdateLinSettings();
    int nDisplayProtocolSettings(int nChannelIndex);
    int nSaveProtocolSettings(int nIndex);
};
