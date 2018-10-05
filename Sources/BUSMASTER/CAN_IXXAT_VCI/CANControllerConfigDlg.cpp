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
 * @file
 *  CANControllerConfigDlg.cpp
 *
 * @brief
 *  Implements the configuration dialog to select the baud rate of the CAN controller.
 *  It's a MFC dialog.
 */

#include "CAN_IXXAT_VCI_stdafx.h"
#include "CAN_IXXAT_VCI.h"
#include "vcinpl.h"
#include "CANControllerConfigDlg.h"
#include "Utility/MultiLanguageSupport.h"


IMPLEMENT_DYNAMIC(CCANControllerConfigDlg, CDialog)


BEGIN_MESSAGE_MAP(CCANControllerConfigDlg, CDialog)
    ON_BN_CLICKED(IDC_BUTTON_CANCFG_OK, OnBnClickedButtonCancfgOk)
    ON_CBN_SELENDOK(IDC_COMBO_CIA_BAUD_SELECTION, OnCbnSelendokComboCiaBaudSelection)
    ON_BN_CLICKED(IDC_BUTTON_CANCEL, OnBnClickedButtonCancel)
    ON_EN_CHANGE(IDC_EDIT_BITREG0, OnEnChangeEditBitreg0)
    ON_EN_CHANGE(IDC_EDIT_BITREG1, OnEnChangeEditBitreg1)
END_MESSAGE_MAP()

/**
 * @fn CCANControllerConfigDlg::CCANControllerConfigDlg
 *
 * @brief
 *  Constructor of the dialog.
 *
 *
 * @param iBTRRegisters
 *  Value of the bit timing registers which should used.
 * @param [in,out]  pParent
 *  If non-null, the parent window.
 */
CCANControllerConfigDlg::CCANControllerConfigDlg(std::string omBaudRate, int iBTRRegisters, CWnd* pParent /*=nullptr*/)
    : CDialog(CCANControllerConfigDlg::IDD, pParent)
{
    m_dwBTR0 = (iBTRRegisters & 0xFF00) >> 8;
    m_dwBTR1 = (iBTRRegisters & 0x00FF);
    m_bSuppressUpdateCalculation = FALSE;
    m_dwBaudRate = strtoul(omBaudRate.c_str(),NULL,0);
}

/**
 * @fn  CCANControllerConfigDlg::~CCANControllerConfigDlg()
 *
 * @brief
 *  Destructor.
 *
 */
CCANControllerConfigDlg::~CCANControllerConfigDlg()
{
}

/**
 * @brief Exchanges data to/from the controls in this dialog.
 *
 * @param [in,out]  pDX
 *  If non-null, an object that manages the data exchange operation.
 *
 */
void CCANControllerConfigDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_COMBO_CIA_BAUD_SELECTION, m_comboBoxCiABaudSelection);
    DDX_Control(pDX, IDC_EDIT_BITREG0, m_editBTR0);
    DDX_Control(pDX, IDC_EDIT_BITREG1, m_editBTR1);
}


/**
 * @brief
 *  Called when this dialog is created.
 *
 * @return
 *  true if it succeeds, false if it fails.
 *
 */
BOOL CCANControllerConfigDlg::OnInitDialog()
{
    // first initialize the elements of this dialog
    CDialog::OnInitDialog();


    FillBaudStruct();
    for (int i=0; i < IXXAT_NUM_OF_CIA_ENTRIES; i++)
    {
        m_comboBoxCiABaudSelection.AddString(m_asBaudList[i].strName.c_str());
    }

    SelectComboBaudRate();//Setting the cursor value in BaudRate Selection Box
    int iCurIndex = GetListIndexFromBTRRegisters();
    m_comboBoxCiABaudSelection.SetCurSel(iCurIndex);
    UpdateBTRFields(iCurIndex);
    m_bDialogCancel = FALSE;

    return TRUE;
}


/**
 * @brief
 *  Called when the OK button was clicked.
 *  Closes the dialog with the result IDOK.
*/
void CCANControllerConfigDlg::OnBnClickedButtonCancfgOk()
{
    CDialog::OnOK();
}


/**
 * @brief
 *  Called when the Cancel button was clicked.
 *  Closes the dialog with the result IDCANCEL.
 *
 */
void CCANControllerConfigDlg::OnBnClickedButtonCancel()
{
    m_bDialogCancel = TRUE;
    CDialog::OnCancel();
}


/**
 * @brief
 *  Fill baud structure with IXXAT values and the
 *  CiA baud rates. Called once at dialog creation time.
 *
 */
void CCANControllerConfigDlg::FillBaudStruct()
{
    m_asBaudList[0].strName = "10";
    m_asBaudList[0].dwBaud = 10;
    m_asBaudList[0].dwBTR0 = CAN_BT0_10KB;
    m_asBaudList[0].dwBTR1 = CAN_BT1_10KB;

    m_asBaudList[1].strName = "20";
    m_asBaudList[1].dwBaud = 20;
    m_asBaudList[1].dwBTR0 = CAN_BT0_20KB;
    m_asBaudList[1].dwBTR1 = CAN_BT1_20KB;

    m_asBaudList[2].strName = "50";
    m_asBaudList[2].dwBaud = 50;
    m_asBaudList[2].dwBTR0 = CAN_BT0_50KB;
    m_asBaudList[2].dwBTR1 = CAN_BT1_50KB;

    m_asBaudList[3].strName = "100";
    m_asBaudList[3].dwBaud = 100;
    m_asBaudList[3].dwBTR0 = CAN_BT0_100KB;
    m_asBaudList[3].dwBTR1 = CAN_BT1_100KB;

    m_asBaudList[4].strName = "125";
    m_asBaudList[4].dwBaud = 125;
    m_asBaudList[4].dwBTR0 = CAN_BT0_125KB;
    m_asBaudList[4].dwBTR1 = CAN_BT1_125KB;

    m_asBaudList[5].strName = "250";
    m_asBaudList[5].dwBaud = 250;
    m_asBaudList[5].dwBTR0 = CAN_BT0_250KB;
    m_asBaudList[5].dwBTR1 = CAN_BT1_250KB;

    m_asBaudList[6].strName = "500";
    m_asBaudList[6].dwBaud = 500;
    m_asBaudList[6].dwBTR0 = CAN_BT0_500KB;
    m_asBaudList[6].dwBTR1 = CAN_BT1_500KB;

    m_asBaudList[7].strName = "800";
    m_asBaudList[7].dwBaud = 800;
    m_asBaudList[7].dwBTR0 = CAN_BT0_800KB;
    m_asBaudList[7].dwBTR1 = CAN_BT1_800KB;

    m_asBaudList[8].strName = "1000";
    m_asBaudList[8].dwBaud = 1000;
    m_asBaudList[8].dwBTR0 = CAN_BT0_1000KB;
    m_asBaudList[8].dwBTR1 = CAN_BT1_1000KB;

    // the class members m_dwBTR0 and m_dwBTR1
    // was set in the constructor
    m_asBaudList[9].strName = _("User defined");
    m_asBaudList[9].dwBaud = 0;
    m_asBaudList[9].dwBTR0 = m_dwBTR0;
    m_asBaudList[9].dwBTR1 = m_dwBTR1;

    m_strSelectedBaudName = _("User defined");
    m_mapBaudRateToCiaBaudIndex.clear();
    for(int index = 0; index<= 9; index++)
    {
        m_mapBaudRateToCiaBaudIndex[m_asBaudList[index].dwBaud] =  index;
    }
}

/**
 * @brief
 *  Called when another entry in the combo box
 *  was selected.
 *
 */
void CCANControllerConfigDlg::OnCbnSelendokComboCiaBaudSelection()
{
    int iIndex = m_comboBoxCiABaudSelection.GetCurSel();
    if (iIndex >= 0)
    {
        UpdateBTRFields(iIndex);
    }
}

/**
 * @brief
 *  Updates the bit timing edit fields after changing the selection in the combo box.
 *
 * @param iIndex
 *  Zero-based index of the index selected in the combo box.
 *
 */
void CCANControllerConfigDlg::UpdateBTRFields(int iIndex)
{
    if (iIndex < IXXAT_NUM_OF_CIA_ENTRIES)
    {
        m_bSuppressUpdateCalculation = TRUE;

        CString strTemp;
        // the data should used from the internal list
        // because the user has selected it by the combo box
        strTemp.Format("%.2X", m_asBaudList[iIndex].dwBTR0);
        m_editBTR0.SetWindowText(strTemp);

        strTemp.Format("%.2X", m_asBaudList[iIndex].dwBTR1);
        m_editBTR1.SetWindowText(strTemp);

        m_strSelectedBaudName = m_asBaudList[iIndex].strName;

        m_dwBTR0 = m_asBaudList[iIndex].dwBTR0;
        m_dwBTR1 = m_asBaudList[iIndex].dwBTR1;
        m_dwBaudRate = m_asBaudList[iIndex].dwBaud;

        m_bSuppressUpdateCalculation = FALSE;
    }
}


/**
 * @brief
 *  Return the index in the combo box list for the
 *  current bit timing values stored in the class
 *  variables m_dwBTR0 and m_dwBTR1. If these are
 *  no valid values then the last index is used which
 *  is user defined.
 *
 * @return
 *  The index to select in the combox box.
 *
 */
int CCANControllerConfigDlg::GetListIndexFromBTRRegisters()
{
    int iResult = (IXXAT_NUM_OF_CIA_ENTRIES-1); // used defined baud rate

    if ( (m_dwBTR0 == CAN_BT0_10KB) && (m_dwBTR1 == CAN_BT1_10KB) )
    {
        iResult = 0;
    }
    else if ( (m_dwBTR0 == CAN_BT0_20KB) && (m_dwBTR1 == CAN_BT1_20KB) )
    {
        iResult = 1;
    }
    else if ( (m_dwBTR0 == CAN_BT0_50KB) && (m_dwBTR1 == CAN_BT1_50KB) )
    {
        iResult = 2;
    }
    else if ( (m_dwBTR0 == CAN_BT0_100KB) && (m_dwBTR1 == CAN_BT1_100KB) )
    {
        iResult = 3;
    }
    else if ( (m_dwBTR0 == CAN_BT0_125KB) && (m_dwBTR1 == CAN_BT1_125KB) )
    {
        iResult = 4;
    }
    else if ( (m_dwBTR0 == CAN_BT0_250KB) && (m_dwBTR1 == CAN_BT1_250KB) )
    {
        iResult = 5;
    }
    else if ( (m_dwBTR0 == CAN_BT0_500KB) && (m_dwBTR1 == CAN_BT1_500KB) )
    {
        iResult = 6;
    }
    else if ( (m_dwBTR0 == CAN_BT0_800KB) && (m_dwBTR1 == CAN_BT1_800KB) )
    {
        iResult = 7;
    }
    else if ( (m_dwBTR0 == CAN_BT0_1000KB) && (m_dwBTR1 == CAN_BT1_1000KB) )
    {
        iResult = 8;
    }

    return iResult;
}


/**
 * @brief
 *  Gets the bit timing value. The int values
 *  is 0xXXXX0011, left BTR Register 0 and then
 *  Register 1.
 *  E.g. 0x031C = 0x03 and 0x1C  (125 kBit/s).
 *
 * @return
 *  The bit timing value.
 *
 */
int CCANControllerConfigDlg::GetBitTimingValue()
{
    int iResult = (m_dwBTR0 << 8);
    iResult |= m_dwBTR1;
    return iResult;
}

/**
 * @brief
 *  Return the baud rate to show the user.

 * @return
 *  The bit timing name.
 *
 */
std::string CCANControllerConfigDlg::GetBitTimingName()
{
    return m_strSelectedBaudName;
}

/**
 * @brief
 *  Called when the user changes the value in the edit
 *  field for the bit timing register 0. The combo box
 *  selection will be set the "user defined" or if it
 *  is a CiA baud rate to that value.
 *
 */
void CCANControllerConfigDlg::OnEnChangeEditBitreg0()
{
    if (!m_bSuppressUpdateCalculation)
    {
        OutputDebugString("OnEnChangeEditBitreg0");
        UpdateComboBoxIndexFromEditFields();
    }
}

/**
 * @brief
 *  Called when the user changes the value in the edit
 *  field for the bit timing register 1. The combo box
 *  selection will be set the "user defined" or if it
 *  is a CiA baud rate to that value.
 *
 */
void CCANControllerConfigDlg::OnEnChangeEditBitreg1()
{
    if (!m_bSuppressUpdateCalculation)
    {
        OutputDebugString("OnEnChangeEditBitreg1");
        UpdateComboBoxIndexFromEditFields();
    }
}


/**
 * @brief
 *  Called when the user had changed the values
 *  in the edit fields for the bit timing registers.
 *  The baud rate combo box selection will we set
 *  to "user defined" or the CiA baud rate.
 *
 */
void CCANControllerConfigDlg::UpdateComboBoxIndexFromEditFields()
{
    TCHAR szText[20];
    TCHAR* szDummy;
    CString omStrBaudRate;
    m_editBTR0.GetWindowText(szText, 20);
    m_dwBTR0 = (DWORD) _tcstoul(szText, &szDummy, 16);


    m_editBTR1.GetWindowText(szText, 20);
    m_dwBTR1 = (DWORD) _tcstoul(szText, &szDummy, 16);

    int iIndexInList = GetListIndexFromBTRRegisters();
    m_comboBoxCiABaudSelection.SetCurSel(iIndexInList);
    m_comboBoxCiABaudSelection.GetWindowText(omStrBaudRate);
    m_dwBaudRate = strtoul(omStrBaudRate,NULL,0);
}

/**
 * @brief
 *  Called when selected a baud rate in the combo box.
 *
 */
void CCANControllerConfigDlg::SelectComboBaudRate()
{
    int nBaudrate = m_dwBaudRate/1000; // BaudRate in KBPS
    std::map<int, int>::iterator itr = m_mapBaudRateToCiaBaudIndex.find(nBaudrate);
    if(itr != m_mapBaudRateToCiaBaudIndex.end() && itr->second >= 0)
    {
        int nIndex = itr->second;
        m_comboBoxCiABaudSelection.SetCurSel(nIndex);
        m_strSelectedBaudName = m_asBaudList[nIndex].strName;
        m_dwBTR0 = m_asBaudList[nIndex].dwBTR0;
        m_dwBTR1 = m_asBaudList[nIndex].dwBTR1;
    }
    else
    {
        m_comboBoxCiABaudSelection.SetCurSel(9);
    }
}

/**
 * @brief
 *  Called when the user open by button clicked
 *  the dialog with the advanced baud rate settings.
 *
 */
int CCANControllerConfigDlg::InvokeAdavancedSettings(PSCONTROLLER_DETAILS pControllerDetails, UINT nCount, UINT nSelectedHw)
{
    if (pControllerDetails!=nullptr)
    {
        m_dwBTR0 = (pControllerDetails[nSelectedHw].m_nBTR0BTR1 & 0xFF00) >> 8;
        m_dwBTR1 = (pControllerDetails[nSelectedHw].m_nBTR0BTR1 & 0x00FF);
        m_bSuppressUpdateCalculation = FALSE;
        m_dwBaudRate = atol(pControllerDetails[nSelectedHw].m_omStrBaudrate.c_str());
        m_asDummyControllerDetails[nSelectedHw] = pControllerDetails[nSelectedHw];
        int nRet = DoModal();
        if(nRet != IDOK) // On Cancel
        {
            pControllerDetails[nSelectedHw] = m_asDummyControllerDetails[nSelectedHw];
        }
        else
        {
            m_bDialogCancel = FALSE;
            CString omTempBaudRate;
            omTempBaudRate.Format("%ld",(m_dwBaudRate*1000));
            pControllerDetails[nSelectedHw].m_omStrBaudrate = omTempBaudRate;
        }
    }
    return 1;
}

/**
 * @brief
 *  This function will validate the user input value of
 *  baud rate. A valid baud rate will be calculated.
 * @remark comment from the file ChangeRegisters.cpp
*/
DOUBLE CCANControllerConfigDlg::vValidateBaudRate(DOUBLE dBaudRate,int,UINT )
{
    int nBaudRate = (int)dBaudRate;
    int nBaudrate = nBaudRate/1000; // BaudRate in KBPS
    if(m_mapBaudRateToCiaBaudIndex.size() == 0)
    {
        FillBaudStruct();
    }
    std::map<int, int>::iterator itr = m_mapBaudRateToCiaBaudIndex.find(nBaudrate);
    if(itr != m_mapBaudRateToCiaBaudIndex.end() && itr->second >= 0)
    {
        return nBaudRate;
    }
    else
    {
        return 500000;
    }
}
