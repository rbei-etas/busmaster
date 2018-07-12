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

#include "FrameProcessor_stdafx.h"
#include "resource.h"
#include "DataTypes/Log_DataTypes.h"
#include "ConfigMsgLogDlg.h"
#include "Utility\MultiLanguageSupport.h"
#include "Utility\UtilFunctions.h"
#include "Utility\Utility.h"

#define STR_FILTER_DIALOG_FORMAT        "Configure Filter for Log File: %s"
#define BUSMASTER_LOG_REMOVE            "Do you want to remove selected log file entry?"
#define BUSMASTER_LOG_ALREADYEXISTS     "Log file: %s is already added to the log configuration list!"
#define BUSMASTER_J1939_LOGFILENAME     "BUSMASTERLogFile_J1939.log"
#define BUSMASTER_LIN_LOGFILENAME     "BUSMASTERLogFile_LIN.log"
#define BUSMASTER_LOG_SELECTION_TITLE   "Select a Log file"
#define BUSMASTER_LOG_FILTER            "*.log|*.log||"
#define BUSMASTER_LOG_FILE_EXTENSION    "log"
#define BUSMASTER_LOG_COL_NAME          "Log File"


//Advance Log File Constraints
#define defMIN_LOG_FILE_SIZE 1 //1Mb
#define defMAX_LOG_FILE_SIZE 4096  //4k(Mb)
#define defDEFAULT_LOG_FILE_SIZE 50 //50MB

#define defMAX_LOG_FILE_COUNT 1000
#define defMIN_LOG_FILE_COUNT 1
#define defDEFAULT_LOG_FILE_COUNT 10

#define defMAX_LOG_TIME_HRS 168
#define defMIN_LOG_TIME_HRS 0
#define defDEFAULT_LOG_TIME_HRS 0

#define defMAX_LOG_TIME_MIN 59
#define defMIN_LOG_TIME_MIN 0
#define defDEFAULT_LOG_TIME_MIN 30

//Advanced Log file Errors
#define defSTR_LOG_TIME_ERROR "Minimum logging time required is 1 min. Resetting logging time to default 30 min."

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


CConfigMsgLogDlg::CConfigMsgLogDlg( IFrameProcessor_Common* baseLogger, BOOL& bLogOnConnect, CWnd* pParent)
    : CDialog(CConfigMsgLogDlg::IDD, pParent)
    , m_omControlParam("")
    , m_bLogOnConnect(bLogOnConnect)
    , m_omControlParam2("")
    , m_unChannelCount(0)
{
    m_nMeasurement = 0;
    m_nSize = 0;
    m_nTime = 0;
    mFrameProcessor = baseLogger;
    m_nLogIndexSel = -1;
    m_bUpdatingUI = FALSE;
    m_bEditingON = FALSE;
    m_bLogON = FALSE;
    m_unDispUpdateTimerId = 0;
    m_nFileSize = defDEFAULT_LOG_FILE_SIZE;
    m_nFileCount = defDEFAULT_LOG_FILE_COUNT;
    m_nTimeHrs = defDEFAULT_LOG_TIME_HRS;
    m_nTimeMin = defDEFAULT_LOG_TIME_MIN;

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

    DDX_Control(pDX, IDC_CHKB_ON_TRANSMISSION, m_ChkLogOnMeasurement);
    DDX_Control(pDX, IDC_CHKB_LOG_FILE_SIZE, m_ChkLogOnSize);
    DDX_Control(pDX, IDC_CHKB_LOG_ON_TIME, m_ChkLogOnTime);
    DDX_Text(pDX, IDC_EDIT_FILE_SIZE, m_nFileSize);
    DDV_MinMaxInt(pDX, m_nFileSize, defMIN_LOG_FILE_SIZE, defMAX_LOG_FILE_SIZE);
    DDX_Text(pDX, IDC_EDIT_HOURS, m_nTimeHrs);
    DDV_MinMaxInt(pDX, m_nTimeHrs, defMIN_LOG_TIME_HRS, defMAX_LOG_TIME_HRS);
    DDX_Text(pDX, IDC_EDIT_MINUTES, m_nTimeMin);
    DDV_MinMaxInt(pDX, m_nTimeMin, defMIN_LOG_TIME_MIN, defMAX_LOG_TIME_MIN);
    DDX_Text(pDX, IDC_EDIT_NO_OF_FILES, m_nFileCount);
    DDV_MinMaxInt(pDX, m_nFileCount, defMIN_LOG_FILE_COUNT, defMAX_LOG_FILE_COUNT);
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

    ON_BN_CLICKED(IDC_CHKB_ON_TRANSMISSION, OnBnClickedLogOnMeasurement)
    ON_BN_CLICKED(IDC_CHKB_LOG_FILE_SIZE, OnBnClickedLogOnSize)
    ON_BN_CLICKED(IDC_CHKB_LOG_ON_TIME, OnBnClickedLogOnTime)
    ON_EN_CHANGE(IDC_EDIT_FILE_SIZE, OnEnChngLogFileSize)
    ON_EN_CHANGE(IDC_EDIT_HOURS, OnEnChngLogFileTimeHrs)
    ON_EN_CHANGE(IDC_EDIT_MINUTES, OnEnChngLogFileTimeMins)
    ON_EN_CHANGE(IDC_EDIT_NO_OF_FILES, OnEnChngLogFilesAllowed)
    ON_EN_CHANGE(IDC_EDIT_LOG_COMMENT, OnEnChngLogComment) //arun

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

        case TIME_MODE_RELATIVE:
            m_omComboTimeMode.SetCurSel(TIME_REL_INDEX);
            break;

        case TIME_MODE_SYSTEM:
        default:
            m_omComboTimeMode.SetCurSel(TIME_SYS_INDEX);
            break;
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
        ASSERT(false);
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
    if (pWnd != nullptr)
    {
        pWnd->EnableWindow(Enable);
        if (Enable == FALSE)
        {
            switch (eCtrlType)
            {
                case EDITCTRL:
                case COMBOBOX:
                    pWnd->SetWindowText(nullptr);
                    break;

                case RADIOBUTTON:
                case CHECKBOX:
                    ((CButton*) pWnd)->SetCheck(0);
                    break;

                case BUTTON:
                    break;

                case STATICTEXT:
                    break;
            }
        }
    }
    else
    {
        ASSERT(false);
    }
}

void CConfigMsgLogDlg::vUpdateControl(int nControlID, ECONTROLTYPE eCtrlType,
                                      BYTE bAction)
{
    CWnd* pWnd = GetDlgItem(nControlID);
    if (nullptr != pWnd)
    {
        // Let's start with enable/disable flag
        pWnd->EnableWindow(CActionFlag::bCtrlToBeEnabled(bAction));

        // Clear flag
        if (CActionFlag::bCtrlToBeCleared(bAction))
        {
            switch (eCtrlType)
            {
                case EDITCTRL:
                case COMBOBOX:
                    pWnd->SetWindowText(nullptr);
                    break;

                case RADIOBUTTON:
                case CHECKBOX:
                    ((CButton*) pWnd)->SetCheck(0);
                    break;

                case BUTTON:
                    break;

                case STATICTEXT:
                    break;

                default:
                    ASSERT(false);
            }
        }
    }
    else
    {
        ASSERT(false);
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
    vEnableDisableControl(IDC_CHKB_ON_TRANSMISSION, CHECKBOX, bValue);
    vEnableDisableControl(IDC_CHKB_LOG_FILE_SIZE, CHECKBOX, bValue);
    GetDlgItem(IDC_EDIT_FILE_SIZE)->EnableWindow(bValue);
    GetDlgItem(IDC_EDIT_NO_OF_FILES)->EnableWindow(bValue);
    vEnableDisableControl(IDC_CHKB_LOG_ON_TIME, CHECKBOX, bValue);
    GetDlgItem(IDC_EDIT_HOURS)->EnableWindow(bValue);
    GetDlgItem(IDC_EDIT_MINUTES)->EnableWindow(bValue);
    vEnableDisableControl(IDC_EDIT_LOG_COMMENT, EDITCTRL, bValue);
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
        CString omCompareStr = (omCurrLogFile.Find("\\") != -1) ?
                               omFullPath : omFileName;
        Found = (omCurrLogFile.CompareNoCase(omCompareStr) == 0);
    }

    return Found;
}

CString CConfigMsgLogDlg::GetUniqueLogFilePath(void)
{
    CString omStrFullPath = "";
    std::string acPathBuffer = {L'\0'};      // Get current working
    char acFilePath[BM_MAX_PATH] = {L'\0'};      // Get current working
    GetCurrentVerBusMasterUserDataPath(acPathBuffer);

    BOOL bFound = TRUE; // Means - "found unique name"

    for (USHORT Count = 0; bFound == TRUE; Count++)
    {
        CString omNewLogFileName = "";  // New Log file name
        auto baseName = GetDefaultLogFileName();
        omNewLogFileName.Format("%s_%d.log", baseName.c_str(), Count);
        PathCombine(acFilePath, acPathBuffer.c_str(), omNewLogFileName.GetBuffer(MAX_PATH));
        omStrFullPath = acFilePath;


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
        EnablingAdvSettings(!m_bLogON);
    }
    else
    {
        vEnableDisableControls(FALSE);
        EnablingAdvSettings(FALSE);
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
    {
        m_ChkbResetTimeStamp.EnableWindow(1);
    }
    else
    {
        m_ChkbResetTimeStamp.EnableWindow(0);
    }

    if(sLogStruct.m_bResetAbsTimeStamp == TRUE)
    {
        m_ChkbResetTimeStamp.SetCheck(1);
    }
    else
    {
        m_ChkbResetTimeStamp.SetCheck(0);
    }

    if (sTrigger.m_unTriggerType != NONE)
    {
        if (sTrigger.m_unTriggerType == BOTH) // Start and stop trigger edit
        {
            // control
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
                vUpdateControl(IDC_CHKB_STARTTRIGGER, CHECKBOX, CActionFlag::ENABLE_CTRL);
                vUpdateControl(IDC_EDIT_STARTMSGID, EDITCTRL, CActionFlag::CLEAR_CTRL);
            }
            if (sTrigger.m_unTriggerType == STOP) // Stop trigger edit control
            {
                CheckDlgButton(IDC_CHKB_STOPTRIGGER, BST_CHECKED);
                m_odStopMsgID.vSetValue((__int64) sTrigger.m_unStopID);
            }
            else
            {
                vUpdateControl(IDC_CHKB_STOPTRIGGER, CHECKBOX, CActionFlag::ENABLE_CTRL);
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
    // Retaining Advanced Settings
    if(sLogStruct.m_sLogAdvStngs.m_bIsLogOnMesurement == TRUE)
    {
        CheckDlgButton(IDC_CHKB_ON_TRANSMISSION, BST_CHECKED);
    }
    else
    {
        CheckDlgButton(IDC_CHKB_ON_TRANSMISSION, BST_UNCHECKED);
    }

    if(sLogStruct.m_sLogAdvStngs.m_bIsLogOnSize == TRUE)
    {
        CheckDlgButton(IDC_CHKB_LOG_FILE_SIZE, BST_CHECKED);
    }
    else
    {
        CheckDlgButton(IDC_CHKB_LOG_FILE_SIZE, BST_UNCHECKED);
    }

    if(sLogStruct.m_sLogAdvStngs.m_bIsLogOnTime == TRUE)
    {
        CheckDlgButton(IDC_CHKB_LOG_ON_TIME, BST_CHECKED);
    }
    else
    {
        CheckDlgButton(IDC_CHKB_LOG_ON_TIME, BST_UNCHECKED);
    }
    EnablingMaxNumEdit();
    GetDlgItem(IDC_EDIT_FILE_SIZE)->SetWindowText(sLogStruct.m_sLogAdvStngs.m_omSizeInMB);
    GetDlgItem(IDC_EDIT_HOURS)->SetWindowText(sLogStruct.m_sLogAdvStngs.m_omLogTimeHrs);
    GetDlgItem(IDC_EDIT_MINUTES)->SetWindowText(sLogStruct.m_sLogAdvStngs.m_omLogTimeMins);
    GetDlgItem(IDC_EDIT_NO_OF_FILES)->SetWindowText(sLogStruct.m_sLogAdvStngs.m_omMaxNoOfLogFiles);
    GetDlgItem(IDC_EDIT_LOG_COMMENT)->SetWindowText(sLogStruct.m_sLogAdvStngs.m_omLogComment);//arun
}

void CConfigMsgLogDlg::vUpdate_Datastore_From_GUI(USHORT ushIndex, int CtrlID)
{
    if (m_bEditingON == FALSE)
    {
        return;
    }

    SLOGINFO sLogStruct;
    GetLoggingBlock(ushIndex, sLogStruct);
    sLogStruct.m_sLogAdvStngs.m_nConnectionCount = -1;
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
            {
                sLogStruct.m_bResetAbsTimeStamp = TRUE;
            }
            else
            {
                sLogStruct.m_bResetAbsTimeStamp = FALSE;
            }
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

                case TIME_REL_INDEX:
                    sLogStruct.m_eLogTimerMode = TIME_MODE_RELATIVE;
                    m_ChkbResetTimeStamp.EnableWindow(0);
                    break;

                case TIME_SYS_INDEX:
                default:
                    sLogStruct.m_eLogTimerMode = TIME_MODE_SYSTEM;
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
            sLogStruct.m_bEnabled = (m_omListLogFiles.GetCheck(ushIndex) == TRUE);
        }
        break;
        case IDC_EDIT_LOGFILEPATH:
        {
            int MaxCount = sizeof(sLogStruct.m_sLogFileName) / sizeof(char);
            (GetDlgItem(IDC_EDIT_LOGFILEPATH))->GetWindowText(
                sLogStruct.m_sLogFileName, MaxCount);
        }
        break;
        case IDC_CHKB_ON_TRANSMISSION:
        {
            if(m_ChkLogOnMeasurement.GetCheck() == BST_CHECKED)
            {
                sLogStruct.m_sLogAdvStngs.m_bIsLogOnMesurement = TRUE;
            }
            else if(m_ChkLogOnMeasurement.GetCheck() == BST_UNCHECKED)
            {
                sLogStruct.m_sLogAdvStngs.m_bIsLogOnMesurement = FALSE;
            }
        }
        break;
        case IDC_CHKB_LOG_FILE_SIZE:
        {
            if(m_ChkLogOnSize.GetCheck() == BST_CHECKED)
            {
                sLogStruct.m_sLogAdvStngs.m_bIsLogOnSize = TRUE;
            }
            else if(m_ChkLogOnSize.GetCheck() == BST_UNCHECKED)
            {
                sLogStruct.m_sLogAdvStngs.m_bIsLogOnSize = FALSE;
            }
        }
        break;
        case IDC_CHKB_LOG_ON_TIME:
        {
            if(m_ChkLogOnTime.GetCheck() == BST_CHECKED)
            {
                sLogStruct.m_sLogAdvStngs.m_bIsLogOnTime = TRUE;
            }
            else if(m_ChkLogOnTime.GetCheck() == BST_UNCHECKED)
            {
                sLogStruct.m_sLogAdvStngs.m_bIsLogOnTime = FALSE;
            }
        }
        break;
        case IDC_EDIT_FILE_SIZE:
        {
            CString strSize = "";

            GetDlgItem(IDC_EDIT_FILE_SIZE)->GetWindowText(strSize);

            sLogStruct.m_sLogAdvStngs.m_omSizeInMB = strSize;
        }
        break;
        case IDC_EDIT_HOURS:
        {
            CString strHrs = "";

            GetDlgItem(IDC_EDIT_HOURS)->GetWindowText(strHrs);

            sLogStruct.m_sLogAdvStngs.m_omLogTimeHrs = strHrs;

        }
        break;
        case IDC_EDIT_MINUTES:
        {
            CString strMins = "";

            GetDlgItem(IDC_EDIT_MINUTES)->GetWindowText(strMins);

            sLogStruct.m_sLogAdvStngs.m_omLogTimeMins = strMins;

        }
        break;
        case IDC_EDIT_NO_OF_FILES:
        {
            CString strNoOfFiles = "";

            GetDlgItem(IDC_EDIT_NO_OF_FILES)->GetWindowText(strNoOfFiles);

            sLogStruct.m_sLogAdvStngs.m_omMaxNoOfLogFiles = strNoOfFiles;
        }
        break;
        case IDC_EDIT_LOG_COMMENT:
        {
            CString strLogComment = "";
            GetDlgItem(IDC_EDIT_LOG_COMMENT)->GetWindowText(strLogComment);
            sLogStruct.m_sLogAdvStngs.m_omLogComment = strLogComment;
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
    strcpy_s(sLogStruct.m_sLogFileName, _MAX_PATH, omFilePathNewLog.GetBuffer(_MAX_PATH));
    AddLoggingBlock(sLogStruct);

    // Added in data store updated. Now do the same in the GUI
    // Insert this item into the list and select the same. Naturally item index
    // will be 1 less than the total number of logging blocks.
    m_nLogIndexSel = GetLoggingBlockCount() - 1;
    AddNewItem_GUI(sLogStruct, m_nLogIndexSel);

    // Select this item
    m_omListLogFiles.SetItemState(m_nLogIndexSel, LVIS_SELECTED | LVIS_FOCUSED,
                                  LVIS_SELECTED | LVIS_FOCUSED);

    //EnablingAdvSettings(FALSE);
}

void CConfigMsgLogDlg::OnBnClickedCbtnRemovelog(void)
{
    m_bUpdatingUI = TRUE;
    RemoveLoggingBlock((USHORT) m_nLogIndexSel);
    m_omListLogFiles.DeleteItem(m_nLogIndexSel);
    m_bUpdatingUI = FALSE;
    if (GetLoggingBlockCount() > 0)
    {
        m_nLogIndexSel--;
        if(m_nLogIndexSel < 0)
        {
            m_nLogIndexSel = 0;
        }
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

    CString omStrLogFile = "";
    (GetDlgItem(IDC_EDIT_LOGFILEPATH))->GetWindowText(omStrLogFile);

    // Show File Selection Dialog to select Log File
    CFileDialog omFileDlg(FALSE, BUSMASTER_LOG_FILE_EXTENSION, omStrLogFile,
                          OFN_HIDEREADONLY | OFN_PATHMUSTEXIST |
                          OFN_EXTENSIONDIFFERENT | OFN_OVERWRITEPROMPT,
                          BUSMASTER_LOG_FILTER, nullptr);
    // Set the caption
    omFileDlg.m_ofn.lpstrTitle = _(BUSMASTER_LOG_SELECTION_TITLE);

    const int nSize = _MAX_PATH  * _MAX_PATH;  //Total 260 Files
    char* szTempBuf = new char[nSize];
    memset(szTempBuf, 0, sizeof(char) * nSize);
    omFileDlg.m_ofn.lpstrFile = szTempBuf;
    omFileDlg.m_ofn.nMaxFile = nSize - 1;
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
                omBuf.Format(_(BUSMASTER_LOG_ALREADYEXISTS), omStrLogFile);
                MessageBox(omBuf, "BUSMASTER", MB_OK | MB_ICONINFORMATION);
                return;
            }
        }
        (GetDlgItem(IDC_EDIT_LOGFILEPATH))->SetWindowText(omStrLogFile);
        m_omListLogFiles.SetItemText(m_nLogIndexSel, 0, omStrLogFile);
        vUpdate_Datastore_From_GUI((USHORT) m_nLogIndexSel, IDC_EDIT_LOGFILEPATH);
        vResetAdvLogParameters();
        vAddSuffixToLogFileName(SUFFIX_DEFAULT);
    }

    delete[]szTempBuf;

}

void CConfigMsgLogDlg::OnLvnItemchangedLogBlockLst(NMHDR* pNMHDR, LRESULT* pResult)
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
            {
                m_bEditingON = TRUE;
            }
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
            EnablingMaxNumEdit();
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
    InitialiseWindow();

    USHORT LogBlocks = GetLoggingBlockCount();
    if (LogBlocks > 0)
    {
        for (USHORT i = 0; i < LogBlocks; i++)
        {
            SLOGINFO sLogObject;
            HANDLE hFind;
            WIN32_FIND_DATA FindData;
            CStdioFile omStdiofile;

            GetLoggingBlock(i, sLogObject);

            // check for valid log file
            CString strTempLog = sLogObject.m_sLogFileName;

            hFind = FindFirstFile(strTempLog, &FindData);

            if(hFind == INVALID_HANDLE_VALUE )//file not found
            {
                DWORD dError = GetLastError();
                if(dError == ERROR_FILE_NOT_FOUND)//file not found
                {
                    BOOL bFileOpen = omStdiofile.Open(strTempLog,
                                                      CFile::modeCreate | CFile::modeWrite | CFile::typeBinary);

                    if(bFileOpen == TRUE)///file created, add in the list
                    {
                        omStdiofile.Close();
                        AddNewItem_GUI(sLogObject, i);
                    }
                    else //not a valid file
                    {
                        RemoveLoggingBlock(i--); //remove the old log data
                        LogBlocks = GetLoggingBlockCount(); //refresh the log count
                    }
                }
                else //not a valid filepath / file folder is removed
                {
                    RemoveLoggingBlock(i--); //remove the old log data
                    LogBlocks = GetLoggingBlockCount(); //refresh the log count
                }
            }
            else ///file found add in the list
            {
                AddNewItem_GUI(sLogObject, i);
            }

        }
        m_nLogIndexSel = 0;
        if(m_omListLogFiles.GetItemCount() > 0 )//check for log files exists
        {
            m_omListLogFiles.SetItemState(m_nLogIndexSel,
                                          LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
        }
        else
        {
            vEnableDisableControls(FALSE);//disable controls if no log files exists
        }
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
        {
            vUpdate_GUI_From_Datastore(0);
        }

        GetDlgItem(IDC_CBTN_ADDLOG)->EnableWindow(FALSE);
        GetDlgItem(IDC_CHECK_RESET_TIMESTAMP)->EnableWindow(FALSE);
        GetDlgItem(IDC_EDIT_NO_OF_FILES)->EnableWindow(FALSE);
        GetWindowText(m_strCurrWndText);
        m_strCurrWndText+=_(" - Read Only as Logging is ON");
        SetWindowText(m_strCurrWndText);

        // If Logging is on
        GetDlgItem(IDOK)->EnableWindow(FALSE);

        m_unDispUpdateTimerId = SetTimer(600, 600, nullptr);
    }
    else
    {
        // If Logging is on
        GetDlgItem(IDOK)->EnableWindow(TRUE);
    }

    // Hide or show the Filters button in the dialog based on the protocol

    GetDlgItem(IDC_LOG_FILTER)->ShowWindow(SW_SHOWNORMAL);

    /*if (J1939 == m_eCurrBus)
    {
        GetDlgItem(IDC_LOG_FILTER)->ShowWindow(0);
    }*/

    return TRUE;
}


void CConfigMsgLogDlg::EnablingAdvSettings(BOOL bEnable)
{
    SLOGINFO sLogStruct;
    GetLoggingBlock(m_nLogIndexSel, sLogStruct);

    if(sLogStruct.m_sLogAdvStngs.m_bIsLogOnSize == TRUE)
    {
        GetDlgItem(IDC_EDIT_FILE_SIZE)->EnableWindow(bEnable);
    }
    else
    {
        GetDlgItem(IDC_EDIT_FILE_SIZE)->EnableWindow(!bEnable);
    }

    if(sLogStruct.m_sLogAdvStngs.m_bIsLogOnTime  == BST_CHECKED)
    {
        GetDlgItem(IDC_EDIT_HOURS)->EnableWindow(bEnable);
        GetDlgItem(IDC_EDIT_MINUTES)->EnableWindow(bEnable);
    }
    else
    {
        GetDlgItem(IDC_EDIT_HOURS)->EnableWindow(!bEnable);
        GetDlgItem(IDC_EDIT_MINUTES)->EnableWindow(!bEnable);
    }
    EnablingMaxNumEdit();
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
            SetWindowText("");
            pdc->DrawText(m_strCurrWndText,CRect(4,4,400,50),DT_END_ELLIPSIS);
            bSwitchDisplay = false;
        }
        else
        {
            pdc->DrawText(m_strCurrWndText,CRect(4,4,400,50),DT_END_ELLIPSIS);
            SetWindowText(m_strCurrWndText);
            bSwitchDisplay = true;
        }
        ReleaseDC(pdc);
    }
    CDialog::OnTimer(nIDEvent);
}


void CConfigMsgLogDlg::vSetLogFileONOFF(BOOL bLogON)
{
    m_bLogON = bLogON;
}



void CConfigMsgLogDlg::OnBnClickedLogFilter(void)
{
    HandleFilterSettings();
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
USHORT CConfigMsgLogDlg::GetLoggingBlockCount( void )
{
    return mFrameProcessor->GetLoggingBlockCount();
}

HRESULT CConfigMsgLogDlg::GetLoggingBlock(USHORT ushBlk, SLOGINFO& sLogObject)
{
    return mFrameProcessor->GetLoggingBlock( ushBlk, sLogObject );
}

HRESULT CConfigMsgLogDlg::SetLoggingBlock(USHORT ushBlk, const SLOGINFO& sLogObject)
{
    return mFrameProcessor->SetLoggingBlock( ushBlk, sLogObject );
}

HRESULT CConfigMsgLogDlg::AddLoggingBlock(const SLOGINFO& sLogObject)
{
    return mFrameProcessor->AddLoggingBlock( sLogObject );
}

HRESULT CConfigMsgLogDlg::RemoveLoggingBlock(USHORT ushBlk)
{
    return mFrameProcessor->RemoveLoggingBlock( ushBlk );
}

// END ABSRACT HELPER FUNCTIONS


void CConfigMsgLogDlg::OnBnClickedOk()
{
    CString omStrLogTimeHrs;
    CString omStrLogTimeMin;
    GetDlgItem(IDC_EDIT_MINUTES)->GetWindowText(omStrLogTimeMin);
    GetDlgItem(IDC_EDIT_HOURS)->GetWindowText(omStrLogTimeHrs);
    if(atoi(omStrLogTimeHrs) == 0 && atoi(omStrLogTimeMin) == 0 && m_ChkLogOnTime.GetCheck() == BST_CHECKED)
    {
        int nResult = MessageBox(defSTR_LOG_TIME_ERROR, "BUSMASTER", MB_ICONERROR|MB_OKCANCEL);
        if(nResult == IDOK)
        {
            GetDlgItem(IDC_EDIT_MINUTES)->SetWindowText(std::to_string(defDEFAULT_LOG_TIME_MIN).c_str());
            vUpdate_Datastore_From_GUI((USHORT) m_nLogIndexSel, (IDC_EDIT_MINUTES));
            return;
        }
        else
        {
            return;
        }
    }
    m_bLogOnConnect = m_ChkbEnableLogOnConnect.GetCheck();
    // TODO: Add your control notification handler code here
    OnOK();
}


void CConfigMsgLogDlg::OnBnClickedLogOnMeasurement()
{
    vUpdate_Datastore_From_GUI((USHORT) m_nLogIndexSel, (IDC_CHKB_ON_TRANSMISSION));
    EnablingAdvSettings(TRUE);
    if(m_ChkLogOnMeasurement.GetCheck() == BST_CHECKED)
    {
        vAddSuffixToLogFileName(SUFFIX_MEASUREMENT);
    }
    else
    {
        vRemoveSuffixFromLogFileName(SUFFIX_MEASUREMENT);
    }

}

void CConfigMsgLogDlg::OnBnClickedLogOnSize()
{
    vUpdate_Datastore_From_GUI((USHORT) m_nLogIndexSel, (IDC_CHKB_LOG_FILE_SIZE));

    EnablingAdvSettings(TRUE);
    if(m_ChkLogOnSize.GetCheck() == BST_CHECKED)
    {
        vAddSuffixToLogFileName(SUFFIX_SIZE);
    }
    else
    {
        vRemoveSuffixFromLogFileName(SUFFIX_SIZE);
    }

}

void CConfigMsgLogDlg::OnBnClickedLogOnTime()
{
    vUpdate_Datastore_From_GUI((USHORT) m_nLogIndexSel, (IDC_CHKB_LOG_ON_TIME));

    EnablingAdvSettings(TRUE);

    if(m_ChkLogOnTime.GetCheck() == BST_CHECKED)
    {
        vAddSuffixToLogFileName(SUFFIX_TIME);
    }
    else
    {
        vRemoveSuffixFromLogFileName(SUFFIX_TIME);
    }

}

void CConfigMsgLogDlg::OnEnChngLogFileSize()
{

    CString omStrFileSize;
    GetDlgItem(IDC_EDIT_FILE_SIZE)->GetWindowText(omStrFileSize);
    if(!UpdateData(TRUE))
    {
        GetDlgItem(IDC_EDIT_FILE_SIZE)->SetWindowText(std::to_string(defDEFAULT_LOG_FILE_SIZE).c_str());
        vUpdate_Datastore_From_GUI((USHORT) m_nLogIndexSel, (IDC_EDIT_FILE_SIZE));
    }
    else
    {
        vUpdate_Datastore_From_GUI((USHORT) m_nLogIndexSel, (IDC_EDIT_FILE_SIZE));
    }

}

void CConfigMsgLogDlg::OnEnChngLogFileTimeHrs()
{
    CString omStrLogTimeHrs;
    CString omStrLogTimeMin;
    GetDlgItem(IDC_EDIT_MINUTES)->GetWindowText(omStrLogTimeMin);
    GetDlgItem(IDC_EDIT_HOURS)->GetWindowText(omStrLogTimeHrs);

    if(!UpdateData(TRUE))
    {
        GetDlgItem(IDC_EDIT_HOURS)->SetWindowText(std::to_string(defDEFAULT_LOG_TIME_HRS).c_str());
        vUpdate_Datastore_From_GUI((USHORT) m_nLogIndexSel, (IDC_EDIT_HOURS));
    }
    else
    {
        vUpdate_Datastore_From_GUI((USHORT) m_nLogIndexSel, (IDC_EDIT_HOURS));
    }


}

void CConfigMsgLogDlg::OnEnChngLogFileTimeMins()
{
    CString omStrLogTimeMin;
    CString omStrLogTimeHrs;
    GetDlgItem(IDC_EDIT_MINUTES)->GetWindowText(omStrLogTimeMin);
    GetDlgItem(IDC_EDIT_HOURS)->GetWindowText(omStrLogTimeHrs);
    if(!UpdateData(TRUE))
    {
        GetDlgItem(IDC_EDIT_MINUTES)->SetWindowText(std::to_string(defDEFAULT_LOG_TIME_MIN).c_str());
        vUpdate_Datastore_From_GUI((USHORT) m_nLogIndexSel, (IDC_EDIT_MINUTES));
    }
    else
    {
        vUpdate_Datastore_From_GUI((USHORT) m_nLogIndexSel, (IDC_EDIT_MINUTES));
    }
}

void CConfigMsgLogDlg::OnEnChngLogFilesAllowed()
{
    CString omStrFileCount;
    GetDlgItem(IDC_EDIT_NO_OF_FILES)->GetWindowText(omStrFileCount);
    if(!UpdateData(TRUE))
    {
        GetDlgItem(IDC_EDIT_NO_OF_FILES)->SetWindowText(std::to_string(defDEFAULT_LOG_FILE_COUNT).c_str());
        vUpdate_Datastore_From_GUI((USHORT) m_nLogIndexSel, (IDC_EDIT_NO_OF_FILES));
    }
    else
    {
        vUpdate_Datastore_From_GUI((USHORT) m_nLogIndexSel, (IDC_EDIT_NO_OF_FILES));
    }
}

//arun
void CConfigMsgLogDlg::OnEnChngLogComment()
{
    vUpdate_Datastore_From_GUI((USHORT) m_nLogIndexSel, (IDC_EDIT_LOG_COMMENT));
}

void CConfigMsgLogDlg::EnablingMaxNumEdit()
{
    if(m_ChkLogOnSize.GetCheck() == BST_CHECKED || m_ChkLogOnTime.GetCheck() == BST_CHECKED
            || m_ChkLogOnMeasurement.GetCheck() == BST_CHECKED)
    {
        GetDlgItem(IDC_EDIT_NO_OF_FILES)->EnableWindow(TRUE);

    }
    else
    {
        GetDlgItem(IDC_EDIT_NO_OF_FILES)->EnableWindow(FALSE);
    }
}

void CConfigMsgLogDlg::vAddSuffixToLogFileName(eFILENAMESUFFIX eFileNameSuffix)
{
    std::string strSuffix ="";
    CString omStrFileName;
    (GetDlgItem(IDC_EDIT_LOGFILEPATH))->GetWindowText(omStrFileName);
    if(m_ChkLogOnMeasurement.GetCheck() == BST_CHECKED)
    {
        if((omStrFileName.Find(S_MEASUREMENT) != std::string::npos) && (eFileNameSuffix != SUFFIX_MEASUREMENT) && (eFileNameSuffix != SUFFIX_DEFAULT))
        {
            vRemoveSuffixFromLogFileName(SUFFIX_MEASUREMENT);
        }
        strSuffix.append(S_MEASUREMENT);
        strSuffix.append(std::to_string(m_nMeasurement));
    }
    if(m_ChkLogOnSize.GetCheck() == BST_CHECKED)
    {
        if((omStrFileName.Find(S_SIZE) != std::string::npos) && (eFileNameSuffix != SUFFIX_SIZE) && (eFileNameSuffix != SUFFIX_DEFAULT))
        {
            vRemoveSuffixFromLogFileName(SUFFIX_SIZE);
        }
        strSuffix.append(S_SIZE);
        strSuffix.append(std::to_string(m_nSize));
    }
    if(m_ChkLogOnTime.GetCheck() == BST_CHECKED)
    {
        if((omStrFileName.Find(S_TIME) != std::string::npos) && (eFileNameSuffix != SUFFIX_TIME) && (eFileNameSuffix != SUFFIX_DEFAULT))
        {
            vRemoveSuffixFromLogFileName(SUFFIX_TIME);
        }
        strSuffix.append(S_TIME);
        strSuffix.append(std::to_string(m_nTime));
    }

    (GetDlgItem(IDC_EDIT_LOGFILEPATH))->GetWindowText(omStrFileName);
    std::string strFileName = std::string(omStrFileName);
    strFileName.replace(strFileName.find(".log"),sizeof(".log")-1,"");
    strFileName.append(strSuffix);
    strFileName.append(".log");
    (GetDlgItem(IDC_EDIT_LOGFILEPATH))->SetWindowText(strFileName.c_str());
    vUpdate_Datastore_From_GUI((USHORT) m_nLogIndexSel, IDC_EDIT_LOGFILEPATH);
    m_omListLogFiles.SetItemText(m_nLogIndexSel,0,strFileName.c_str());

}

void CConfigMsgLogDlg::vRemoveSuffixFromLogFileName(eFILENAMESUFFIX eFileNameSuffix)
{
    CString omStrFileName;
    (GetDlgItem(IDC_EDIT_LOGFILEPATH))->GetWindowText(omStrFileName);
    std::string strFileName = std::string(omStrFileName);
    std::string strSuffixSubString = "";
    int n_pos = 0;

    switch(eFileNameSuffix)
    {
        case SUFFIX_MEASUREMENT:
        {

            if(!CUtilFunctions::bFindLastSuffix(strFileName,S_MEASUREMENT,n_pos))
            {

                m_nMeasurement = 0;
                return;
            }
            strSuffixSubString = strFileName.substr(n_pos,sizeof(strFileName));
            sscanf_s(strSuffixSubString.c_str(),M_SUFFIX_FORMAT,&m_nMeasurement);
            std::string strCurr_M = S_MEASUREMENT+std::to_string(m_nMeasurement);
            strSuffixSubString.replace(strSuffixSubString.find(strCurr_M),strlen(strCurr_M.c_str()),"");

        }
        break;
        case SUFFIX_SIZE:
        {

            if(!CUtilFunctions::bFindLastSuffix(strFileName,S_SIZE,n_pos))
            {
                m_nSize = 0;
                return;
            }

            strSuffixSubString = strFileName.substr(n_pos,sizeof(strFileName));
            sscanf_s(strSuffixSubString.c_str(),S_SUFFIX_FORMAT,&m_nSize);
            std::string strCurr_S = S_SIZE+std::to_string(m_nSize);
            strSuffixSubString.replace(strSuffixSubString.find(strCurr_S),strlen(strCurr_S.c_str()),"");
        }
        break;
        case SUFFIX_TIME:
        {

            if(!CUtilFunctions::bFindLastSuffix(strFileName,S_TIME,n_pos))
            {
                m_nTime = 0;
                return;
            }
            strSuffixSubString = strFileName.substr(n_pos,sizeof(strFileName));
            sscanf_s(strSuffixSubString.c_str(),T_SUFFIX_FORMAT,&m_nTime);
            std::string strCurr_T = S_TIME+std::to_string(m_nTime);
            strSuffixSubString.replace(strSuffixSubString.find(strCurr_T),strlen(strCurr_T.c_str()),"");
        }
        break;
    }

    std::string strBaseSubString = strFileName.substr(0,n_pos);
    strBaseSubString.append(strSuffixSubString);
    (GetDlgItem(IDC_EDIT_LOGFILEPATH))->SetWindowText(strBaseSubString.c_str());
    vUpdate_Datastore_From_GUI((USHORT) m_nLogIndexSel, IDC_EDIT_LOGFILEPATH);
    m_omListLogFiles.SetItemText(m_nLogIndexSel,0,strBaseSubString.c_str());
}
void CConfigMsgLogDlg::vResetAdvLogParameters()
{
    m_nMeasurement = 0;
    m_nSize = 0;
    m_nTime = 0;
}


