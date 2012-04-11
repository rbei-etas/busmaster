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
 * \file      ConfigMsgLogDlg.cpp
 * \author    Ratnadip Choudhury
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 */
// ConfigMsgLogDlg.cpp : implementation file

#include "stdafx.h"
#include "Resource.h"
#include "FrameProcessor/BaseFrameProcessor_CAN.h"
#include "FrameProcessor/BaseFrameProcessor_J1939.h"
#include "Datatypes/Filter_Datatypes.h"
#include "DataTypes/Log_DataTypes.h"
#include "Utility/RadixEdit.h"
#include "Struct.h"
#include "Filter/Filter_extern.h"
#include "ConfigMsgLogDlg.h"
#include "InterfaceGetter.h"
#include "DIL_Interface/BaseDIL_CAN.h"
#include ".\configmsglogdlg.h"

#define STR_FILTER_DIALOG_FORMAT        _T("Configure Filter for Log File: %s")
#define BUSMASTER_LOG_REMOVE			_T("Do you want to remove selected log file entry?")
#define BUSMASTER_LOG_ALREADYEXISTS 	_T("Log file: %s is already added to the log configuration list!")
#define BUSMASTER_CAN_LOGFILENAME		_T("BUSMASTERLogFile%d.log")
#define BUSMASTER_J1939_LOGFILENAME	    _T("BUSMASTERLogFile_J1939_%d.log")
#define BUSMASTER_LOG_SELECTION_TITLE	_T("Select a Log file")
#define BUSMASTER_LOG_FILTER			_T("*.log|*.log||")
#define BUSMASTER_LOG_FILE_EXTENSION	_T("log")
#define BUSMASTER_LOG_COL_NAME			_T("Log File")



enum
{
    CAN_CHNL1_INDEX = 0, CAN_CHNL2_INDEX, CAN_ALL_INDEX,
};

enum
{
    TIME_ABS_INDEX = 0, TIME_REL_INDEX, TIME_SYS_INDEX,
};

#define LOG_FILE_COL_WIDTH_PROPOTION    0.95
#define MASK_CHECK_UNCHECK              0x00003000

// CConfigMsgLogDlg dialog

IMPLEMENT_DYNAMIC(CConfigMsgLogDlg, CDialog)


CConfigMsgLogDlg::CConfigMsgLogDlg(ETYPE_BUS eCurrBus,void* pouBaseLogger, BOOL& bLogOnConnect,
                                   CWnd* pParent, const void* psFilter)
      : CDialog(CConfigMsgLogDlg::IDD, pParent), m_eCurrBus(eCurrBus)
      , m_omControlParam(_T(""))
      , m_bLogOnConnect(bLogOnConnect)
      , m_omControlParam2(_T(""))
      , m_unChannelCount(0)
{
    switch (m_eCurrBus)
    {
        case CAN:
        {
            m_pouFProcCAN = (CBaseFrameProcessor_CAN *) pouBaseLogger;
            m_psFilterConfigured = (SFILTERAPPLIED_CAN *) psFilter;
            m_strCurrWndText =_T("Configure Logging for CAN");
            m_omControlParam = _T("Start on Reception of ID 0x");
            m_omControlParam2 = _T("Stop on Reception of ID 0x");
        }
        break;
        case J1939:
        {
            m_pouLoggerJ1939 = (CBaseFrameProcessor_J1939 *) pouBaseLogger;
            m_psJ1939Filter = (SFILTERAPPLIED_J1939 *) psFilter;
            m_strCurrWndText =_T("Configure Logging for J1939");
            m_omControlParam = _T("Start on Reception of PGN 0x");
            m_omControlParam2 = _T("Stop on Reception of PGN 0x");
        }
        break;
        default: ASSERT(FALSE);
    }
    m_nLogIndexSel = -1;
    m_bUpdatingUI = FALSE;
    m_bEditingON = FALSE;
	m_bLogON = FALSE;
	m_unDispUpdateTimerId = 0;
}

CConfigMsgLogDlg::~CConfigMsgLogDlg()
{
}

void CConfigMsgLogDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);

    DDX_Control(pDX, IDC_COMB_CHANNEL, m_omComboChannel);
    DDX_Control(pDX, IDC_COMB_TIMEMODE, m_omComboTimeMode);
    DDX_Control(pDX, IDC_CHECK_RESET_TIMESTAMP, m_ChkbResetTimeStamp);
    DDX_Control(pDX, IDC_LOGBLOCK_LST, m_omListLogFiles);
    DDX_Control(pDX, IDC_CHKB_ENABLE_LOG_ONCONNECT, m_ChkbEnableLogOnConnect);
    DDX_Control(pDX, IDC_EDIT_STARTMSGID, m_odStartMsgID);
    DDX_Control(pDX, IDC_EDIT_STOPMSGID, m_odStopMsgID);
    DDX_Text(pDX, IDC_STATIC_01, m_omControlParam);
    DDX_Text(pDX, IDC_STATIC_02, m_omControlParam2);
}

BEGIN_MESSAGE_MAP(CConfigMsgLogDlg, CDialog)
	ON_BN_CLICKED(IDC_CBTN_ADDLOG, OnBnClickedCbtnAddlog)
	ON_BN_CLICKED(IDC_CBTN_LOGFILEPATH, OnBnClickedCbtnLogFilePath)
	ON_BN_CLICKED(IDC_CBTN_DELETELOG, OnBnClickedCbtnRemovelog)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LOGBLOCK_LST, OnLvnItemchangedLogBlockLst)
    ON_CONTROL_RANGE(CBN_SELCHANGE, IDC_COMB_TIMEMODE, IDC_COMB_CHANNEL, OnSelChangeComb)
    ON_CONTROL_RANGE(BN_CLICKED, IDC_RBTN_DECIMAL, IDC_RBTN_HEX, OnNumRButtonClick)
    ON_CONTROL_RANGE(BN_CLICKED, IDC_RBTN_APPEND, IDC_CHECK_RESET_TIMESTAMP, OnFileRButtonClick)
    ON_CONTROL_RANGE(BN_CLICKED, IDC_CHKB_STARTTRIGGER, IDC_CHKB_STOPTRIGGER, OnChkbTriggerClick)
    ON_CONTROL_RANGE(EN_CHANGE, IDC_EDIT_STARTMSGID, IDC_EDIT_STOPMSGID, OnStartStopMsgIDEnChange)
	ON_BN_CLICKED(IDC_LOG_FILTER, OnBnClickedLogFilter)
	ON_WM_TIMER()	
    ON_BN_CLICKED(IDOK, OnBnClickedOk)
END_MESSAGE_MAP()


// Helper functions: start
void CConfigMsgLogDlg::vCreateFileList(void)
{
    m_omListLogFiles.SetExtendedStyle(LVS_EX_CHECKBOXES | LVS_EX_GRIDLINES);
	m_omListLogFiles.InsertColumn(0, BUSMASTER_LOG_COL_NAME);
    // Set the width to occupy the whole list
	CRect omRect;
	m_omListLogFiles.GetWindowRect(&omRect);
	int nWidth = static_cast<int>(omRect.Width() * LOG_FILE_COL_WIDTH_PROPOTION);
	// Set Col Width
	m_omListLogFiles.SetColumnWidth(0, nWidth);
}

void CConfigMsgLogDlg::SetGUIFromTimeMode(ETIMERMODE eTimeMode)
{
    switch (eTimeMode)
    {
        case TIME_MODE_ABSOLUTE:
			m_omComboTimeMode.SetCurSel(TIME_ABS_INDEX);
			m_ChkbResetTimeStamp.EnableWindow(1); 
			break;
        case TIME_MODE_RELATIVE: m_omComboTimeMode.SetCurSel(TIME_REL_INDEX); break;
        case TIME_MODE_SYSTEM: 
        default: m_omComboTimeMode.SetCurSel(TIME_SYS_INDEX); break;
    }
}

void CConfigMsgLogDlg::SetGUIFromChannel(TYPE_CHANNEL CurrChannel)
{
    /* CAN channel number starts from 1 upto 32. The combo box lists equivalent
    entries in the following order: "ALL", "1", "2", ... , "32" */
    if (CurrChannel <= m_unChannelCount)
    {
        m_omComboChannel.SetCurSel(CurrChannel);
    }
    else
    {
        ASSERT(FALSE);
    }
}

SLOGTRIGGER CConfigMsgLogDlg::GetLogTriggerFromGUI(void)
{
    SLOGTRIGGER sTrigger = {NONE, 0, 0};

    if (IsDlgButtonChecked(IDC_CHKB_STARTTRIGGER))
    {
        sTrigger.m_unStartID = (UINT) m_odStartMsgID.lGetValue();
        sTrigger.m_unTriggerType = START;
    }
    if (IsDlgButtonChecked(IDC_CHKB_STOPTRIGGER))
    {
        sTrigger.m_unStopID = (UINT) m_odStopMsgID.lGetValue();
        sTrigger.m_unTriggerType = (START == sTrigger.m_unTriggerType) ? BOTH : STOP;
    }
    return sTrigger;
}

void CConfigMsgLogDlg::vEnableDisableControl(int nControlID, 
                                           ECONTROLTYPE eCtrlType, BOOL Enable)
{
    CWnd* pWnd = GetDlgItem(nControlID);
    if (pWnd != NULL)
    {
        pWnd->EnableWindow(Enable);
		if (Enable == FALSE)
        {
            switch (eCtrlType)
            {
                case EDITCTRL: 
                case COMBOBOX: pWnd->SetWindowText(NULL); break;
                case RADIOBUTTON:
                case CHECKBOX: ((CButton*) pWnd)->SetCheck(0); break;
                case BUTTON: break;
                case STATICTEXT: break;
            }
        }
    }
    else
    {
        ASSERT(FALSE);
    }
}

void CConfigMsgLogDlg::vUpdateControl(int nControlID, ECONTROLTYPE eCtrlType,
                                      BYTE bAction)
{
    CWnd* pWnd = GetDlgItem(nControlID);
    if (NULL != pWnd)
    {
        // Let's start with enable/disable flag 
        pWnd->EnableWindow(CActionFlag::bCtrlToBeEnabled(bAction));

        // Clear flag
        if (CActionFlag::bCtrlToBeCleared(bAction))
        {
            switch (eCtrlType)
            {
                case EDITCTRL:
                case COMBOBOX: pWnd->SetWindowText(NULL); break;
                case RADIOBUTTON:
                case CHECKBOX: ((CButton*) pWnd)->SetCheck(0); break;
                case BUTTON: break;
                case STATICTEXT: break;
                default: ASSERT(FALSE);
            }
        }
    }
    else
    {
        ASSERT(FALSE);
    }
}
void CConfigMsgLogDlg::vEnableDisableControls(BOOL bValue)
{
    // Log file Path
    vEnableDisableControl(IDC_EDIT_LOGFILEPATH, EDITCTRL, bValue);

    // Time mode selection combo box
    vEnableDisableControl(IDC_COMB_TIMEMODE, COMBOBOX, bValue);

    // Channel selection combo box
    vEnableDisableControl(IDC_COMB_CHANNEL, COMBOBOX, bValue);

    // Decimal radio button
    vEnableDisableControl(IDC_RBTN_DECIMAL, RADIOBUTTON, bValue);

    // Hexadecimal radio button
    vEnableDisableControl(IDC_RBTN_HEX, RADIOBUTTON, bValue);

    // Append file mode radio button
    vEnableDisableControl(IDC_RBTN_APPEND, RADIOBUTTON, bValue);

    // Overwrite file mode radio button
    vEnableDisableControl(IDC_RBTN_OVERWRITE, RADIOBUTTON, bValue);

    // Overwrite file mode radio button
    vEnableDisableControl(IDC_CHECK_RESET_TIMESTAMP, CHECKBOX, bValue);

	// Start trigger check box and edit control
    vEnableDisableControl(IDC_CHKB_STARTTRIGGER, CHECKBOX, bValue);
    vEnableDisableControl(IDC_EDIT_STARTMSGID, EDITCTRL, bValue);

    // Stop trigger check box and edit control
    vEnableDisableControl(IDC_CHKB_STOPTRIGGER, CHECKBOX, bValue);
    vEnableDisableControl(IDC_EDIT_STOPMSGID, EDITCTRL, bValue);

   // Log file path button
   vEnableDisableControl(IDC_CBTN_LOGFILEPATH, BUTTON, bValue);

   // Log filter button
   vEnableDisableControl(IDC_LOG_FILTER, BUTTON, bValue);

   // Delete logging block button
   vEnableDisableControl(IDC_CBTN_DELETELOG, BUTTON, bValue);

   // Indicator static text controls in trogger group box
   vEnableDisableControl(IDC_STATIC_01, STATICTEXT, bValue);
   vEnableDisableControl(IDC_STATIC_02, STATICTEXT, bValue);
}

BOOL CConfigMsgLogDlg::FoundInLogList(CString omFullPath, CString omFileName)
{
    BOOL Found = FALSE;

    USHORT TotalLogBlocks = GetLoggingBlockCount();

    for (USHORT i = 0; (i < TotalLogBlocks) && (Found == FALSE); i++)
    {
        SLOGINFO sLogObjTmp; // Iterate through the logging list in the
        GetLoggingBlock(i, sLogObjTmp); // datastore
        CString omCurrLogFile = sLogObjTmp.m_sLogFileName; // and retrieve
        // the log file name

        // Subject to its content type (whether bare log file name or the
        // full path), the comparison will be carried out.
        CString omCompareStr = (omCurrLogFile.Find(_T("\\")) != -1) ?
                                        omFullPath : omFileName;
        Found = (omCurrLogFile.CompareNoCase(omCompareStr) == 0);
    }

    return Found;
}

CString CConfigMsgLogDlg::GetUniqueLogFilePath(void)
{
    CString omStrFullPath = _T("");

    TCHAR acPathBuffer[MAX_PATH] = {L'\0'};      // Get current working
    GetCurrentDirectory(MAX_PATH, acPathBuffer); // directory
    BOOL bFound = TRUE; // Means - "found unique name"

    for (USHORT Count = 0; bFound == TRUE; Count++)
    {
        CString omNewLogFileName = _T("");  // New Log file name
        // Contrive a log file name on interation index
        if (CAN == m_eCurrBus)
        {
            omNewLogFileName.Format(BUSMASTER_CAN_LOGFILENAME, Count);
        }
        else if (J1939 == m_eCurrBus)
        {
            omNewLogFileName.Format(BUSMASTER_J1939_LOGFILENAME, Count);
        }
        omStrFullPath = acPathBuffer; // Full Path
        omStrFullPath = omStrFullPath + _T("\\") + omNewLogFileName;
        // We have two different strings to compare. The first one is the bare
        // log file name and the second one is the full path.

        // Iterate through the list of existing logging blocks. If the same
        // file name has a hit, then try for another name. 
        bFound = FoundInLogList(omStrFullPath, omNewLogFileName);
    }
    // At the end of this routine a unique log file should've been found.
    return omStrFullPath;
}

void CConfigMsgLogDlg::vUpdate_GUI_From_Datastore(USHORT usIndex)
{
    SLOGINFO sLogStruct;
    SLOGTRIGGER& sTrigger = sLogStruct.m_sLogTrigger;

    if (GetLoggingBlock(usIndex, sLogStruct) == S_OK)
    {
        vEnableDisableControls(!m_bLogON);
    }
    else
    {
        vEnableDisableControls(FALSE);
        return;
    }

    m_omListLogFiles.SetCheck(usIndex, sLogStruct.m_bEnabled);
    (GetDlgItem(IDC_EDIT_LOGFILEPATH))->SetWindowText(sLogStruct.m_sLogFileName);
    SetGUIFromTimeMode(sLogStruct.m_eLogTimerMode);
    SetGUIFromChannel(sLogStruct.m_ChannelSelected);

    int CheckBox = (DEC == sLogStruct.m_eNumFormat) ? IDC_RBTN_DECIMAL : IDC_RBTN_HEX;
    CheckRadioButton(IDC_RBTN_DECIMAL, IDC_RBTN_HEX, CheckBox);

    CheckBox = (APPEND_MODE == sLogStruct.m_eFileMode) ? IDC_RBTN_APPEND : IDC_RBTN_OVERWRITE;
    CheckRadioButton(IDC_RBTN_APPEND, IDC_RBTN_OVERWRITE, CheckBox);

	if(sLogStruct.m_eLogTimerMode  == TIME_MODE_ABSOLUTE)
		m_ChkbResetTimeStamp.EnableWindow(1);
	else
		m_ChkbResetTimeStamp.EnableWindow(0);

	if(sLogStruct.m_bResetAbsTimeStamp == TRUE)
		m_ChkbResetTimeStamp.SetCheck(1);
	else
		m_ChkbResetTimeStamp.SetCheck(0);

    if (sTrigger.m_unTriggerType != NONE)
    {
        if (sTrigger.m_unTriggerType == BOTH) // Start and stop trigger edit
        {                                     // control
            CheckDlgButton(IDC_CHKB_STARTTRIGGER, BST_CHECKED);
            CheckDlgButton(IDC_CHKB_STOPTRIGGER, BST_CHECKED);
            m_odStartMsgID.vSetValue((__int64) sTrigger.m_unStartID);
            m_odStopMsgID.vSetValue((__int64) sTrigger.m_unStopID);
        }
        else
        {
            if (sTrigger.m_unTriggerType == START) // Start trigger edit control
            {
                CheckDlgButton(IDC_CHKB_STARTTRIGGER, BST_CHECKED);
                m_odStartMsgID.vSetValue((__int64) sTrigger.m_unStartID);
            }
            else
            {
                vUpdateControl(IDC_CHKB_STARTTRIGGER, CHECKBOX, CActionFlag::CLEAR_CTRL);
                vUpdateControl(IDC_EDIT_STARTMSGID, EDITCTRL, CActionFlag::CLEAR_CTRL);
            }
            if (sTrigger.m_unTriggerType == STOP) // Stop trigger edit control
            {
                CheckDlgButton(IDC_CHKB_STOPTRIGGER, BST_CHECKED);
                m_odStopMsgID.vSetValue((__int64) sTrigger.m_unStopID);
            }
            else
            {
                vUpdateControl(IDC_CHKB_STOPTRIGGER, CHECKBOX, CActionFlag::CLEAR_CTRL);
                vUpdateControl(IDC_EDIT_STOPMSGID, EDITCTRL, CActionFlag::CLEAR_CTRL);
            }
        }
    }
    else
    {
        // Start trigger check box control: clear
        CheckDlgButton(IDC_CHKB_STARTTRIGGER, BST_UNCHECKED);
        // Stop trigger check box control: clear
        CheckDlgButton(IDC_CHKB_STOPTRIGGER, BST_UNCHECKED);
        // Start trigger edit control: clear and disable
        vEnableDisableControl(IDC_EDIT_STARTMSGID, EDITCTRL, FALSE);
        // Stop trigger edit control: clear and disable
        vEnableDisableControl(IDC_EDIT_STOPMSGID, EDITCTRL, FALSE);
    }
}

void CConfigMsgLogDlg::vUpdate_Datastore_From_GUI(USHORT ushIndex, int CtrlID)
{
    if (m_bEditingON == FALSE) return;

    SLOGINFO sLogStruct;
    GetLoggingBlock(ushIndex, sLogStruct);

    switch (CtrlID)
    {
        case IDC_RBTN_DECIMAL:
        {
            sLogStruct.m_eNumFormat = DEC;
        }
        break;
        case IDC_RBTN_HEX:
        {
            sLogStruct.m_eNumFormat = HEXADECIMAL;
        }
        break;
        case IDC_RBTN_APPEND:
        {
            sLogStruct.m_eFileMode = APPEND_MODE;
        }
        break;
        case IDC_RBTN_OVERWRITE:
        {
            sLogStruct.m_eFileMode = OVERWRITE_MODE;
        }
        break;
        case IDC_CHECK_RESET_TIMESTAMP:
        {
			if(m_ChkbResetTimeStamp.GetCheck()) 
	            sLogStruct.m_bResetAbsTimeStamp = TRUE;
			else
	            sLogStruct.m_bResetAbsTimeStamp = FALSE;
        }
        break;
        case IDC_COMB_TIMEMODE:
        {
            switch (m_omComboTimeMode.GetCurSel())
            {
                case TIME_ABS_INDEX:
					sLogStruct.m_eLogTimerMode = TIME_MODE_ABSOLUTE;
					m_ChkbResetTimeStamp.EnableWindow(1); 
					break;
                case TIME_REL_INDEX: sLogStruct.m_eLogTimerMode = TIME_MODE_RELATIVE;
					m_ChkbResetTimeStamp.EnableWindow(0); 
					break;
                case TIME_SYS_INDEX: 
                default: sLogStruct.m_eLogTimerMode = TIME_MODE_SYSTEM; 
					m_ChkbResetTimeStamp.EnableWindow(0); 
					break;
            }
        }
        break;
        case IDC_COMB_CHANNEL:
        {
            sLogStruct.m_ChannelSelected = m_omComboChannel.GetCurSel();
            if (sLogStruct.m_ChannelSelected == 0)  // First one denotes ALL
            {
                sLogStruct.m_ChannelSelected = CAN_CHANNEL_ALL;
            }
        }
        break;
        case IDC_EDIT_STARTMSGID: 
        {
            sLogStruct.m_sLogTrigger.m_unStartID = (UINT) m_odStartMsgID.lGetValue();
        }
        break;
        case IDC_EDIT_STOPMSGID:
        {
            sLogStruct.m_sLogTrigger.m_unStopID = (UINT) m_odStopMsgID.lGetValue();
        }
        break;
        case IDC_CHKB_STARTTRIGGER:
        case IDC_CHKB_STOPTRIGGER:
        {
            sLogStruct.m_sLogTrigger = GetLogTriggerFromGUI();
        }
        break;
        case IDC_LOGBLOCK_LST:
        {
            sLogStruct.m_bEnabled = m_omListLogFiles.GetCheck(ushIndex);
        }
        break;
        case IDC_EDIT_LOGFILEPATH:
        {
            int MaxCount = sizeof(sLogStruct.m_sLogFileName) / sizeof(TCHAR);
            (GetDlgItem(IDC_EDIT_LOGFILEPATH))->GetWindowText(
                                        sLogStruct.m_sLogFileName, MaxCount);
        }
        break;
    }

    sLogStruct.m_bIsUpdated = TRUE;
    SetLoggingBlock(ushIndex, sLogStruct);
}
// Helper functions: end

// GUI event handler functions: start
void CConfigMsgLogDlg::OnNumRButtonClick(UINT CtrlID)
{
    vUpdate_Datastore_From_GUI((USHORT) m_nLogIndexSel, CtrlID);
}

void CConfigMsgLogDlg::OnFileRButtonClick(UINT CtrlID)
{
    vUpdate_Datastore_From_GUI((USHORT) m_nLogIndexSel, CtrlID);
}

void CConfigMsgLogDlg::OnChkbTriggerClick(UINT CtrlID)
{
    vUpdate_Datastore_From_GUI((USHORT) m_nLogIndexSel, CtrlID);
    if (IDC_CHKB_STARTTRIGGER == CtrlID)
    {
        vEnableDisableControl(IDC_EDIT_STARTMSGID, EDITCTRL, IsDlgButtonChecked(CtrlID));
    }
    else if (IDC_CHKB_STOPTRIGGER == CtrlID)
    {
        vEnableDisableControl(IDC_EDIT_STOPMSGID, EDITCTRL, IsDlgButtonChecked(CtrlID));
    }
}

void CConfigMsgLogDlg::OnSelChangeComb(UINT CtrlID)
{
    vUpdate_Datastore_From_GUI((USHORT) m_nLogIndexSel, CtrlID);
}

void CConfigMsgLogDlg::OnStartStopMsgIDEnChange(UINT CtrlID)
{
    vUpdate_Datastore_From_GUI((USHORT) m_nLogIndexSel, CtrlID);
}


void CConfigMsgLogDlg::AddNewItem_GUI(SLOGINFO sLogStructNew, int Index)
{
    m_bUpdatingUI = TRUE;
    m_omListLogFiles.InsertItem(Index, sLogStructNew.m_sLogFileName);
    m_omListLogFiles.SetCheck(Index, sLogStructNew.m_bEnabled);
    m_bUpdatingUI = FALSE;
}

void CConfigMsgLogDlg::OnBnClickedCbtnAddlog(void)
{
    USES_CONVERSION;

    // Get unique log file name with full path.
    CString omFilePathNewLog = GetUniqueLogFilePath();

    // Add the new logging object in the datastore.
    SLOGINFO sLogStruct;
    //Select All CAN channels
    sLogStruct.m_ChannelSelected = CAN_CHANNEL_ALL;
    _tcscpy(sLogStruct.m_sLogFileName, omFilePathNewLog.GetBuffer(MAX_PATH));
    AddLoggingBlock(sLogStruct);

    // Added in data store updated. Now do the same in the GUI
    // Insert this item into the list and select the same. Naturally item index
    // will be 1 less than the total number of logging blocks.
    m_nLogIndexSel = GetLoggingBlockCount() - 1;
    AddNewItem_GUI(sLogStruct, m_nLogIndexSel);

    // Select this item
    m_omListLogFiles.SetItemState(m_nLogIndexSel, LVIS_SELECTED | LVIS_FOCUSED,
                                  LVIS_SELECTED | LVIS_FOCUSED);
}

void CConfigMsgLogDlg::OnBnClickedCbtnRemovelog(void)
{
    m_bUpdatingUI = TRUE;
    RemoveLoggingBlock((USHORT) m_nLogIndexSel);
    m_omListLogFiles.DeleteItem(m_nLogIndexSel);
    m_bUpdatingUI = FALSE;
    if (GetLoggingBlockCount() > 0)
    {
        m_nLogIndexSel = 0;
        m_omListLogFiles.SetItemState(m_nLogIndexSel, 
                   LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
    }
    else
    {
        m_nLogIndexSel = -1;
        vEnableDisableControls(FALSE);
    }
    ASSERT(GetLoggingBlockCount() == m_omListLogFiles.GetItemCount());
}

void CConfigMsgLogDlg::OnBnClickedCbtnLogFilePath(void)
{
    USES_CONVERSION;

	CString omStrLogFile = _T("");
    (GetDlgItem(IDC_EDIT_LOGFILEPATH))->GetWindowText(omStrLogFile);

    // Show File Selection Dialog to select Log File
    CFileDialog omFileDlg(FALSE, BUSMASTER_LOG_FILE_EXTENSION, omStrLogFile,
                     OFN_HIDEREADONLY | OFN_PATHMUSTEXIST | 
                     OFN_EXTENSIONDIFFERENT | OFN_OVERWRITEPROMPT, 
                     BUSMASTER_LOG_FILTER, NULL);
    // Set the caption
    omFileDlg.m_ofn.lpstrTitle = BUSMASTER_LOG_SELECTION_TITLE;

    // Show File open dialog
    if (omFileDlg.DoModal() == IDOK)
    {
        omStrLogFile = omFileDlg.GetPathName();
        for (USHORT i = 0; i < GetLoggingBlockCount(); i++)
        {
            SLOGINFO sLogObject;
            GetLoggingBlock(i, sLogObject);
            if (_tcsicmp(sLogObject.m_sLogFileName, omStrLogFile.GetBuffer(MAX_PATH)) == 0)
            {
		        CString omBuf;
		        omBuf.Format(BUSMASTER_LOG_ALREADYEXISTS, omStrLogFile);
		        AfxMessageBox(omBuf, MB_OK | MB_ICONINFORMATION);
                return;
            }
        }
        (GetDlgItem(IDC_EDIT_LOGFILEPATH))->SetWindowText(omStrLogFile);
        m_omListLogFiles.SetItemText(m_nLogIndexSel, 0, omStrLogFile);
        vUpdate_Datastore_From_GUI((USHORT) m_nLogIndexSel, IDC_EDIT_LOGFILEPATH);
    }
}

void CConfigMsgLogDlg::OnLvnItemchangedLogBlockLst(NMHDR *pNMHDR, LRESULT *pResult)
{
    NM_LISTVIEW* pNMListView = (NM_LISTVIEW*) pNMHDR;

    // Update Log File Components
    if ((pNMListView->uChanged != LVIF_TEXT) && (m_bUpdatingUI == FALSE))
    {
        // Selected & Focused
        if (pNMListView->uNewState == (LVIS_FOCUSED | LVIS_SELECTED))
        {
            // Update selection
            m_nLogIndexSel = pNMListView->iItem;
            // Update selected Log file details
            vUpdate_GUI_From_Datastore((USHORT) m_nLogIndexSel);

			if(m_bLogON)
			{
				m_bEditingON = FALSE;
				GetDlgItem(IDC_CHECK_RESET_TIMESTAMP)->EnableWindow(FALSE);
			}
			else
				m_bEditingON = TRUE;
        }
        else if (pNMListView->uNewState & MASK_CHECK_UNCHECK)
        {
            //ASSERT(m_nLogIndexSel == pNMListView->iItem);
            vUpdate_Datastore_From_GUI((USHORT) pNMListView->iItem, IDC_LOGBLOCK_LST);
        }
        else
        {
            m_bEditingON = FALSE;
            // Update Button Status
            vEnableDisableControls(FALSE);
			GetDlgItem(IDC_CHECK_RESET_TIMESTAMP)->EnableWindow(FALSE);
        }
    }
	*pResult = 0;
}

// GUI event handler functions: end

BOOL CConfigMsgLogDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
    m_ChkbEnableLogOnConnect.SetCheck(m_bLogOnConnect == TRUE? BST_CHECKED : BST_UNCHECKED); 
    m_odStartMsgID.vSetConfigData(IDC_EDIT_STARTMSGID);
    m_odStopMsgID.vSetConfigData(IDC_EDIT_STOPMSGID);
    m_odStartMsgID.vSetBase(BASE_HEXADECIMAL);
    m_odStopMsgID.vSetBase(BASE_HEXADECIMAL);
    m_odStartMsgID.vSetSigned(false);
    m_odStopMsgID.vSetSigned(false);

    vCreateFileList();
    //Load channel combo box
    m_omComboChannel.ResetContent();
    if (NULL != GetICANDIL())
    {
        LPARAM lParam = 0;
        if (S_OK == GetICANDIL()->DILC_GetControllerParams(lParam, NULL, NUMBER_HW))
        {
            m_unChannelCount = (UINT)lParam;
        }
    }

    m_omComboChannel.InsertString(0, _T("ALL"));
    for (UINT i = 1; i <= m_unChannelCount; i++)
    {
        CString omChannel; omChannel.Format("%d", i);
        m_omComboChannel.InsertString(i, omChannel.GetBuffer(MAX_PATH)); 
    }
    m_omComboChannel.SetCurSel(0);

    USHORT LogBlocks = GetLoggingBlockCount();
    if (LogBlocks > 0)
    {
        for (USHORT i = 0; i < LogBlocks; i++)
        {
            SLOGINFO sLogObject;
            GetLoggingBlock(i, sLogObject);
            AddNewItem_GUI(sLogObject, i);
        }
        m_nLogIndexSel = 0;
        m_omListLogFiles.SetItemState(m_nLogIndexSel, 
                   LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
    }
    else
    {
        vEnableDisableControls(FALSE);
    }

    SetWindowText(m_strCurrWndText);

	if (m_bLogON)
	{
		vEnableDisableControls(FALSE);
		if(m_omListLogFiles.GetItemCount()>0)
			vUpdate_GUI_From_Datastore(0);
		GetDlgItem(IDC_CBTN_ADDLOG)->EnableWindow(FALSE);
		GetDlgItem(IDC_CHECK_RESET_TIMESTAMP)->EnableWindow(FALSE);

		GetWindowText(m_strCurrWndText);
		m_strCurrWndText+=_T(" - Read Only as Logging is ON");
		//SetWindowText(m_strCurrWndText);
		SetWindowText(_T(""));
		m_unDispUpdateTimerId = SetTimer(600, 600, NULL);		
	}

	// Hide or show the Filters button in the dialog based on the protocol
	if (CAN == m_eCurrBus)
	{
		GetDlgItem(IDC_LOG_FILTER)->ShowWindow(1);
	}
	else if (J1939 == m_eCurrBus)
	{
		GetDlgItem(IDC_LOG_FILTER)->ShowWindow(0);
	}

	return TRUE;
}

// Unpunctual codes

void CConfigMsgLogDlg::OnTimer(UINT nIDEvent)
{		
	if (nIDEvent == m_unDispUpdateTimerId) 
	{
		static bool bSwitchDisplay = true;

		CDC* pdc= GetWindowDC();
		pdc->SetTextColor(RGB(253,153,4));
		pdc->SetBkMode(TRANSPARENT);
		if(bSwitchDisplay)
		{
			SetWindowText(_T(""));
			pdc->DrawText(m_strCurrWndText,CRect(4,4,400,50),DT_END_ELLIPSIS);	
			bSwitchDisplay = false;
		}
		else
		{
			pdc->DrawText(m_strCurrWndText,CRect(4,4,400,50),DT_END_ELLIPSIS);	
			SetWindowText(_T(""));
			//SetWindowText(m_strCurrWndText);
			bSwitchDisplay = true;
		}
	}

	CDialog::OnTimer(nIDEvent);
}


void CConfigMsgLogDlg::vSetLogFileONOFF(BOOL bLogON)
{
	m_bLogON = bLogON;
}

static void vPopulateMainSubList(CMainEntryList& DestList, const SFILTERAPPLIED_CAN* psFilterConfigured,
                                 const SFILTERAPPLIED_CAN* psFilterApplied)
{
    ASSERT(psFilterConfigured != NULL);
    DestList.RemoveAll();

    SMAINENTRY sMainEntry;
    sMainEntry.m_omMainEntryName = _T("CAN");
    if (psFilterApplied == NULL)
    {
        SMAINENTRY sMainEntry;
        sMainEntry.m_omMainEntryName = _T("FILTER_SELECTION_CAN");
        for (INT i = 0; i < psFilterConfigured->m_ushTotal; i++)
        {
            SSUBENTRY sSubEntry;
            sSubEntry.m_omSubEntryName.Format("%s",
                 psFilterConfigured->m_psFilters[i].m_sFilterName.m_acFilterName);
            sMainEntry.m_odUnSelEntryList.AddTail(sSubEntry);            
        }
    }
    else
    {
        
        for (INT i = 0; i < psFilterConfigured->m_ushTotal; i++)
        {
            SSUBENTRY sSubEntry;
            sSubEntry.m_omSubEntryName.Format("%s",
                                    psFilterConfigured->m_psFilters[i].m_sFilterName.m_acFilterName);
            if (SFILTERSET::psGetFilterSetPointer(psFilterApplied->m_psFilters, 
                                                  psFilterApplied->m_ushTotal,
                                                  sSubEntry.m_omSubEntryName.GetBuffer(MAX_PATH)) != NULL)
            {
                 sMainEntry.m_odSelEntryList.AddTail(sSubEntry);  
            }
            else
            {
                sMainEntry.m_odUnSelEntryList.AddTail(sSubEntry);  
            }
        }
    }
    DestList.AddTail(sMainEntry);
}

static void vPopulateFilterApplied(const SFILTERAPPLIED_CAN* psFilterConfigured, SFILTERAPPLIED_CAN& sFilterApplied, CMainEntryList& SrcList)
{
    SMAINENTRY& sMainEntry = SrcList.GetHead();
    int nCount  = sMainEntry.m_odSelEntryList.GetCount();
    sFilterApplied.vClear();
    sFilterApplied.m_psFilters = new SFILTERSET[nCount];
    
    POSITION pos = sMainEntry.m_odSelEntryList.GetHeadPosition();
    while (pos)
    {        		
        SSUBENTRY& sSubEntry = sMainEntry.m_odSelEntryList.GetNext(pos);
        const PSFILTERSET psTemp = SFILTERSET::psGetFilterSetPointer(psFilterConfigured->m_psFilters, 
            psFilterConfigured->m_ushTotal, sSubEntry.m_omSubEntryName.GetBuffer(MAX_PATH));
        ASSERT (psTemp != NULL);
        sFilterApplied.m_psFilters[sFilterApplied.m_ushTotal].bClone(*psTemp);
        sFilterApplied.m_ushTotal++;
    }
}

void CConfigMsgLogDlg::OnBnClickedLogFilter(void)
{
    if (CAN == m_eCurrBus)
    {
        CMainEntryList DestList;
        SFILTERAPPLIED_CAN sFilterApplied;
        m_pouFProcCAN->FPC_GetFilteringScheme((USHORT)m_nLogIndexSel, sFilterApplied);
        vPopulateMainSubList(DestList, m_psFilterConfigured, &sFilterApplied);
        //Show dialog
        if (Filter_ShowSelDlg(this, &DestList) == IDOK)
        {
            vPopulateFilterApplied(m_psFilterConfigured, sFilterApplied, DestList);
            m_pouFProcCAN->FPC_ApplyFilteringScheme((USHORT)m_nLogIndexSel, sFilterApplied);
        }
    }
}

BOOL CConfigMsgLogDlg::OnCommand(WPARAM wParam, LPARAM lParam)
{
    // TODO: Add your specialized code here and/or call the base class
    if (HIWORD(wParam) == EN_CHANGE)
    {
        WORD CtrlID = LOWORD(wParam);
        if ((CtrlID == IDC_EDIT_STARTMSGID) || (CtrlID == IDC_EDIT_STOPMSGID))
        {
            if (m_bEditingON)
            {
                vUpdate_Datastore_From_GUI((USHORT) m_nLogIndexSel, CtrlID);
            }
        }
    }

    return CDialog::OnCommand(wParam, lParam);
}

/* START ABSRACT HELPER FUNCTIONS. IN ORDER TO ADD SUPPORT TO ANY NEW BUS, 
UPDATE THIS GROUP OF FUNCTIONS WITH BUS SPECIFIC CODES. */
USHORT CConfigMsgLogDlg::GetLoggingBlockCount(void)
{
    USHORT Result = 0x0;
    switch (m_eCurrBus)
    {
        case CAN: Result = m_pouFProcCAN->FPC_GetLoggingBlockCount(); break;
        case J1939: Result = m_pouLoggerJ1939->FPJ1_GetLoggingBlockCount(); break;
        default: ASSERT(FALSE);
    }
    return Result;
}

HRESULT CConfigMsgLogDlg::GetLoggingBlock(USHORT ushBlk, SLOGINFO& sLogObject)
{
    HRESULT Result = S_FALSE;
    switch (m_eCurrBus)
    {
        case CAN: Result = m_pouFProcCAN->FPC_GetLoggingBlock(ushBlk, sLogObject); break;
        case J1939: Result = m_pouLoggerJ1939->FPJ1_GetLoggingBlock(ushBlk, sLogObject); break;
        default: ASSERT(FALSE);
    }
    return Result;
}

HRESULT CConfigMsgLogDlg::SetLoggingBlock(USHORT ushBlk, const SLOGINFO& sLogObject)
{
    HRESULT Result = S_FALSE;
    switch (m_eCurrBus)
    {
        case CAN: Result = m_pouFProcCAN->FPC_SetLoggingBlock(ushBlk, sLogObject); break;
        case J1939: Result = m_pouLoggerJ1939->FPJ1_SetLoggingBlock(ushBlk, sLogObject); break;
        default: ASSERT(FALSE);
    }
    return Result;
}

HRESULT CConfigMsgLogDlg::AddLoggingBlock(const SLOGINFO& sLogObject)
{
    HRESULT Result = S_FALSE;
    switch (m_eCurrBus)
    {
        case CAN: Result = m_pouFProcCAN->FPC_AddLoggingBlock(sLogObject); break;
        case J1939: Result = m_pouLoggerJ1939->FPJ1_AddLoggingBlock(sLogObject); break;
        default: ASSERT(FALSE);
    }
    return Result;
}

HRESULT CConfigMsgLogDlg::RemoveLoggingBlock(USHORT ushBlk)
{
    HRESULT Result = S_FALSE;
    switch (m_eCurrBus)
    {
        case CAN: Result = m_pouFProcCAN->FPC_RemoveLoggingBlock(ushBlk); break;
        case J1939: Result = m_pouLoggerJ1939->FPJ1_RemoveLoggingBlock(ushBlk); break;
        default: ASSERT(FALSE);
    }
    return Result;
}

HRESULT CConfigMsgLogDlg::ApplyFilteringScheme(USHORT ushLogBlkID, const void* psFilterObj)
{
    HRESULT Result = S_FALSE;

    switch (m_eCurrBus)
    {
        case CAN:
        {
            Result = m_pouFProcCAN->FPC_ApplyFilteringScheme(ushLogBlkID, 
                                   *((const SFILTERAPPLIED_CAN*) psFilterObj));
        }
        break;
        case J1939:
        {
            Result = m_pouLoggerJ1939->FPJ1_ApplyFilteringScheme(ushLogBlkID, 
                                 *((const SFILTERAPPLIED_J1939*) psFilterObj));
        }
        break;
        default: ASSERT(FALSE);
    }

    return Result;
}

HRESULT CConfigMsgLogDlg::GetFilteringScheme(USHORT ushLogBlk, void* psFilterObj)
{
    HRESULT Result = S_FALSE;

    switch (m_eCurrBus)
    {
        case CAN:
        {
            Result = m_pouFProcCAN->FPC_GetFilteringScheme(ushLogBlk, 
                                        *((SFILTERAPPLIED_CAN *) psFilterObj));
        }
        break;
        case J1939:
        {
            Result = m_pouLoggerJ1939->FPJ1_GetFilteringScheme(ushLogBlk, 
                                      *((SFILTERAPPLIED_J1939 *) psFilterObj));
        }
        break;
        default: ASSERT(FALSE);
    }

    return Result;
}
// END ABSRACT HELPER FUNCTIONS


void CConfigMsgLogDlg::OnBnClickedOk()
{
    m_bLogOnConnect = m_ChkbEnableLogOnConnect.GetCheck();
    // TODO: Add your control notification handler code here
    OnOK();
}
