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

/**
 * \file      ConfigMsgLogDlg.h
 * \brief     This is the class definition file for log configuration
 * \author    Pradeep Kadoor
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * This is the class definition file for log configuration
 */

#if !defined CONFIGMSGLOGDLG_H__INCLUDED_
#define CONFIGMSGLOGDLG_H__INCLUDED_


typedef enum ECONTROLTYPE
{
    EDITCTRL = 0, COMBOBOX, RADIOBUTTON, BUTTON, CHECKBOX, STATICTEXT,
};

class CActionFlag
{
public:

    static const BYTE ACTIONVAL_RESET  = 0x00;
    static const BYTE ENABLE_CTRL      = 0x01;
    static const BYTE CLEAR_CTRL       = 0x02;

    CActionFlag();
    ~CActionFlag();

    // Query functions
    static BOOL bCtrlToBeEnabled(BYTE bFlag)
    {
        return ((bFlag & ENABLE_CTRL) == ENABLE_CTRL);
    };

    static BOOL bCtrlToBeCleared(BYTE bFlag)
    {
        return ((bFlag & CLEAR_CTRL) == CLEAR_CTRL);
    };

    // Value setting functions
    static BYTE bResetActionFlag(void)
    {
        return ACTIONVAL_RESET;
    };

    static BYTE bSetEnableCtrlFlag(BYTE bFlag)
    {
        return (bFlag | ENABLE_CTRL);
    };

    static BYTE bSetClearCtrlFlag(BYTE bFlag)
    {
        return (bFlag | CLEAR_CTRL);
    };
};
// CConfigMsgLogDlg dialog
class CConfigMsgLogDlg : public CDialog
{
	DECLARE_DYNAMIC(CConfigMsgLogDlg)

public:
    CConfigMsgLogDlg(ETYPE_BUS eCurrBus, void* pouBaseLogger, BOOL& bLogOnConnect,
                     CWnd* pParent = NULL, const void* psFilter = NULL);    
	virtual ~CConfigMsgLogDlg();
	enum { IDD = IDD_DLG_CONFIG_MSG_LOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	DECLARE_MESSAGE_MAP()

public:
	// Init Log File Control
    virtual BOOL OnInitDialog();
	void vSetLogFileONOFF(BOOL bLogON);
	

private:
	// Dialog control variables
	CComboBox m_omComboChannel;
	CComboBox m_omComboTimeMode;
	CListCtrl m_omListLogFiles;
    CRadixEdit m_odStartMsgID;
    CRadixEdit m_odStopMsgID;
	CButton m_ChkbEnableLogOnConnect;
	int m_nLogIndexSel;
    // To indicate programmed UI update
    BOOL m_bUpdatingUI;
    CBaseFrameProcessor_CAN* m_pouFProcCAN;
    CBaseFrameProcessor_J1939* m_pouLoggerJ1939;
    const SFILTERAPPLIED_CAN* m_psFilterConfigured;
    const SFILTERAPPLIED_J1939* m_psJ1939Filter;
    BOOL m_bEditingON;
	BOOL m_bLogON;
	UINT m_unDispUpdateTimerId;
	CString m_strCurrWndText;
    ETYPE_BUS m_eCurrBus;
    BOOL& m_bLogOnConnect;
    UINT m_unChannelCount;
private:
	void vEnableDisableControls(BOOL);
	SLOGTRIGGER GetLogTriggerFromGUI(void);
    void vCreateFileList(void);
    void vEnableDisableControl(int nControlID, ECONTROLTYPE eCtrlType, BOOL Enable);
    void vUpdateControl(int nControlID, ECONTROLTYPE eCtrlType, BYTE bAction);
    void SetGUIFromTimeMode(ETIMERMODE eTimeMode);
    void SetGUIFromChannel(TYPE_CHANNEL CurrChannel);
    CString GetUniqueLogFilePath(void);
    void vUpdate_GUI_From_Datastore(USHORT usIndex);
    void vUpdate_Datastore_From_GUI(USHORT usIndex, int CtrlID);
    BOOL FoundInLogList(CString omFullPath, CString omFileName);
    void AddNewItem_GUI(SLOGINFO sLogStructNew, int Index);

    // Abstract helper functions
    USHORT GetLoggingBlockCount(void);
    HRESULT GetLoggingBlock(USHORT ushBlk, SLOGINFO& sLogObject);
    HRESULT SetLoggingBlock(USHORT ushBlk, const SLOGINFO& sLogObject);
    HRESULT AddLoggingBlock(const SLOGINFO& sLogObject);
    HRESULT RemoveLoggingBlock(USHORT ushBlk);
    HRESULT ApplyFilteringScheme(USHORT ushLogBlkID, const void* psFilterObj);
    HRESULT GetFilteringScheme(USHORT ushLogBlk, void* psFilterObj);

protected:
    afx_msg void OnBnClickedCbtnAddlog(void);
    afx_msg void OnBnClickedCbtnLogFilePath(void);
	afx_msg void OnBnClickedCbtnRemovelog(void);
	afx_msg void OnLvnItemchangedLogBlockLst(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnNumRButtonClick(UINT);
    afx_msg void OnFileRButtonClick(UINT);
    afx_msg void OnChkbTriggerClick(UINT);
    afx_msg void OnSelChangeComb(UINT);
    afx_msg void OnStartStopMsgIDEnChange(UINT);
	afx_msg void OnBnClickedLogFilter(void);
    virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnTimer(UINT nIDEvent);	
protected:
    CString m_omControlParam;
    CString m_omControlParam2;
public:
    afx_msg void OnBnClickedOk();
};

#endif // CONFIGMSGLOGDLG_H__INCLUDED_

