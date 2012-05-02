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
 * \file      ChangeRegisters_CAN_ICS_neoVI.cpp
 * \brief     This file contain definition of all function of
 * \author    Pradeep Kadoor, Tobias Lorenz
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * This file contain definition of all function of
 */
// For all standard header file include
#include "CAN_ICS_neoVI_stdafx.h"
#include "ContrConfigPeakUsbDefs.h"
#include "CAN_ICS_neoVI_Resource.h"
#include "ChangeRegisters_CAN_ICS_neoVI.h"

/**
 * \brief Constructor
 *
 * Constructor is called when user create an object of
 * this class. Initialisation of all data members
 */
CChangeRegisters_CAN_ICS_neoVI::CChangeRegisters_CAN_ICS_neoVI(CWnd* pParent /*=NULL*/, PSCONTROLLER_DETAILS psControllerDetails, UINT nHardwareCount)
    : CDialog(CChangeRegisters_CAN_ICS_neoVI::IDD, pParent)
    //, m_nClock(32)
    , m_omStrPropDelay("")
    , m_omStrSJW("")
{
    //{{AFX_DATA_INIT(CChangeRegisters_CAN_ICS_neoVI)
    //m_omStrEditBTR0 = "";
    //m_omStrEditBTR1 = "";
    m_omStrEditCNF1 = "";
    m_omStrEditCNF2 = "";
    m_omStrEditCNF3 = "";
    m_omStrComboSampling = "";
    m_omStrEditBaudRate = "";
    m_omStrComboClock = _T("32");
    m_omStrEditWarningLimit = "";
    //}}AFX_DATA_INIT
    m_unCombClock      = 32;
    m_bDialogCancel    = FALSE;
    memset(&m_sAccFilterInfo, 0, sizeof(m_sAccFilterInfo));
    m_ucWarningLimit    = defWARNING_LIMIT_MIN;
    m_ucControllerMode  = defCONTROLLER_MODE;
    m_usBTR0BTR1 = defDEFAUT_BAUDRATE;
    // Update controller data
    m_pControllerDetails = psControllerDetails;
    m_nPropDelay = 0;
    m_nSJWCurr = 0;
    m_bOption = NO_DEF;
    m_nNoHardware = nHardwareCount;
    m_pControllerDetails = psControllerDetails;
    m_nDataConfirmStatus = WARNING_NOTCONFIRMED;
}

/**
 * \brief Do Data Exchange
 *
 * Called by the framework to exchange and validate dialog data
 */
void CChangeRegisters_CAN_ICS_neoVI::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CChangeRegisters_CAN_ICS_neoVI)
    DDX_Control(pDX, IDC_LIST_CHANNELS, m_omChannelList);
    DDX_Control(pDX, IDC_EDIT_WARNING_LIMIT, m_omEditWarningLimit);
    DDX_Control(pDX, IDC_COMB_SAMPLING, m_omCombSampling);
    DDX_Control(pDX, IDC_LSTC_BTR_LIST, m_omListCtrlBitTime);
    DDX_Control(pDX, IDC_EDIT_BAUD_RATE, m_omEditBaudRate);
    DDX_CBString(pDX, IDC_COMB_SAMPLING, m_omStrComboSampling);
    DDV_MaxChars(pDX, m_omStrComboSampling, 1);
    DDX_Text(pDX, IDC_EDIT_BAUD_RATE, m_omStrEditBaudRate);
    DDX_CBString(pDX, IDC_COMB_CLOCK, m_omStrComboClock);
    DDX_Text(pDX, IDC_EDIT_WARNING_LIMIT, m_omStrEditWarningLimit);
    DDX_Control(pDX, IDC_COMB_PROPDELAY, m_omCtrlPropDelay);
    DDX_Text(pDX, IDC_COMB_SJW, m_omStrSJW);
    DDX_Text(pDX,IDC_COMB_PROPDELAY, m_omStrPropDelay);
    DDX_Control(pDX, IDC_COMB_SJW, m_omCtrlSJW);
    DDX_Control(pDX, IDC_COMB_CLOCK, m_omCtrlClock);
    //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CChangeRegisters_CAN_ICS_neoVI, CDialog)
    //{{AFX_MSG_MAP(CChangeRegisters_CAN_ICS_neoVI)
    ON_EN_KILLFOCUS(IDC_EDIT_BAUD_RATE, OnKillfocusEditBaudRate)
    ON_CBN_SELCHANGE(IDC_COMB_SAMPLING, OnSelchangeCombSampling)
    ON_EN_SETFOCUS(IDC_EDIT_BAUD_RATE, OnSetfocusEditBaudRate)
    ON_BN_CLICKED(IDC_ButtonOK, OnClickedOK)
    ON_CBN_SETFOCUS(IDC_COMB_SAMPLING, OnSetfocusCombSampling)
    ON_WM_HELPINFO()
    ON_NOTIFY(LVN_ITEMCHANGED, IDC_LSTC_BTR_LIST, OnItemchangedLstcBtrList)
    ON_NOTIFY(NM_CLICK, IDC_LIST_CHANNELS, OnClickListChannels)
    ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_CHANNELS, OnItemchangedListChannels)
    ON_NOTIFY(NM_DBLCLK, IDC_LIST_CHANNELS, OnDblclkListChannels)
    ON_NOTIFY(HDN_ITEMCLICK, 0, OnHdnItemclickLstcBtrList)
    ON_CBN_SELCHANGE(IDC_COMB_SJW, OnCbnSelchangeCombSjw)
    ON_CBN_SELCHANGE(IDC_COMB_PROPDELAY, OnCbnSelchangeCombPropdelay)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**
 * \brief On Init Dialog
 *
 * This function is called by the framework in response
 * to the WM_INITDIALOG message. This message is sent to
 * the dialog box during DoModal calls,which occur
 * immediately before the dialog box is displayed.
 * All controls of dialog are initialised in this func.
 */
BOOL CChangeRegisters_CAN_ICS_neoVI::OnInitDialog()
{
    CDialog::OnInitDialog();
    TCHAR acColumnName[defNUMBER_OF_COLUMNS_CAN_ICS_neoVI][50] =
    {
        defSTR_CNF1_COL_HEADING,
        defSTR_CNF2_COL_HEADING,
        defSTR_CNF3_COL_HEADING,
        defSTR_SAMPLE_POINT_COL_HEADING,
        defSTR_NBT_COL_HEADING,
        defSTR_BRP_COL_HEADING
    };
    CString omStrClock          = defCLOCK;
    CString omStrBaudRate       = "";
    CString omStrAcceptanceMask = "";
    CString omStrAcceptanceCode = "";
    CString omStrBrp            = "";
    CString omStrBtr0           = "";
    CString omStrBtr1           = "";
    INT nColumnSize             = 0;
    INT nTotalColunmSize        = 0;
    INT nTotalStrLengthPixel    = 0;
    RECT rListCtrlRect          ;
    // Init Channel List box
    // Create Image List for Channel List Control
    m_omChannelImageList.Create( defCHANNEL_ICON_SIZE,
                                 defCHANNEL_ICON_SIZE,
                                 ILC_COLOR24,
                                 defCHANNEL_LIST_INIT_SIZE,
                                 defCHANNEL_LIST_GROW_SIZE);
    // Load Channel Icon
    CWinApp* pWinApp = (CWinApp*)this;
    m_omChannelImageList.Add(pWinApp->LoadIcon(IDR_PGM_EDTYPE));
    // Assign the image list to the control
    m_omChannelList.SetImageList(&m_omChannelImageList, LVSIL_NORMAL);
    // Insert empty column
    m_omChannelList.InsertColumn( 0, "");
    // Insert all channel information
    // Insert only for available channel information
    int nAvailableHardware = m_nNoHardware;//g_podHardwareInterface->nGetNoOfHardware();

    for (int nChannel = 0 ;
            nChannel < nAvailableHardware;
            nChannel++)
    {
        CString omStrChannel("");
        // Create Channel String
        omStrChannel.Format( defSTR_CHANNEL_NAME_FORMAT,
                             defSTR_CHANNEL_NAME,
                             nChannel + 1);
        // Insert channel item
        m_omChannelList.InsertItem( nChannel, omStrChannel);
    }

    // Set the selected item index to zero
    m_nLastSelection = 0;
    m_omEditBaudRate.vSetBase( BASE_DECIMAL);
    m_omEditBaudRate.vSetSigned(FALSE);
    m_omEditBaudRate.vAcceptFloatingNum( TRUE);
    m_omEditWarningLimit.vSetBase( BASE_DECIMAL);
    m_omEditWarningLimit.vSetSigned(FALSE);
    m_omEditWarningLimit.vAcceptFloatingNum( FALSE);
    //Calculate the total size of all column header
    m_omListCtrlBitTime.GetWindowRect( &rListCtrlRect);
    nTotalColunmSize     = rListCtrlRect.right - rListCtrlRect.left;
    nTotalStrLengthPixel = 0;

    for (INT j=0; j<defNUMBER_OF_COLUMNS; j++)
    {
        nTotalStrLengthPixel +=
            m_omListCtrlBitTime.GetStringWidth(acColumnName[j]);
    }

    //Insert each column name after calculating the size for the same.
    for (INT i=0; i<defNUMBER_OF_COLUMNS; i++)
    {
        nColumnSize  = m_omListCtrlBitTime.GetStringWidth(acColumnName[i]) ;
        nColumnSize +=
            (nTotalColunmSize-nTotalStrLengthPixel)/defNUMBER_OF_COLUMNS;
        m_omListCtrlBitTime.InsertColumn(i,acColumnName[i],
                                         LVCFMT_CENTER, nColumnSize);
    }

    //Set extended style to show selection for all subitems
    m_omListCtrlBitTime.SetExtendedStyle(LVS_EX_FULLROWSELECT);
    m_omCtrlPropDelay.InsertString(0,_T("ALL"));
    m_omCtrlSJW.InsertString(0,_T("ALL"));
    m_omCtrlClock.InsertString(0,defCLOCK);
    // Add an entry in each of the two combo boxes FindStringExact
    int nIndex = m_omCtrlPropDelay.FindStringExact(-1,
                 m_pControllerDetails->m_omStrPropagationDelay);
    m_omStrPropDelay = m_pControllerDetails->m_omStrPropagationDelay;
    m_omStrSJW = m_pControllerDetails->m_omStrSjw;

    //UpdateData();
    if (CB_ERR != nIndex)
    {
        m_omCtrlPropDelay.SetCurSel (nIndex);
    }
    else
    {
        m_omCtrlPropDelay.SetCurSel (0);
    }

    nIndex = m_omCtrlSJW.FindStringExact (-1, m_pControllerDetails->m_omStrSjw);

    if (CB_ERR != nIndex)
    {
        m_omCtrlSJW.SetCurSel (nIndex);
    }
    else
    {
        m_omCtrlSJW.SetCurSel (0);
    }

    // List values having prop delay
    int nPropDelay = nGetValueFromComboBox(m_omCtrlPropDelay);

    if (nPropDelay != m_nPropDelay)
    {
        m_nPropDelay = nPropDelay;
        bDecideCalculatingOption();
        vChangeListBoxValues(-1);
    }

    // List Values having SJW
    int nSJWCurr = nGetValueFromComboBox(m_omCtrlSJW);

    if (nSJWCurr != m_nSJWCurr)
    {
        m_nSJWCurr = nSJWCurr;
        bDecideCalculatingOption();
        vChangeListBoxValues(-1);
    }

    m_omEditWarningLimit.SetReadOnly(TRUE);
    CWnd* pWndFilter = GetDlgItem(IDC_CBTN_ACCEPTANCE);

    if (pWndFilter != NULL)
    {
        pWndFilter->EnableWindow(FALSE);
    }

    //Initialise the index for number of items in list box before passing it is
    //function to calculate the same.
    // Set the Focus to the First Item
    m_omChannelList.SetItemState( 0,
                                  LVIS_SELECTED | LVIS_FOCUSED,
                                  LVIS_SELECTED | LVIS_FOCUSED);
    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

/**
 * \brief On Cancel
 *
 * message handlers on CANCEL request
 */
void CChangeRegisters_CAN_ICS_neoVI::OnCancel()
{
    // Flag to be checked while validating the edit control input on kill focus
    m_bDialogCancel = TRUE;
    m_nDataConfirmStatus = INFO_RETAINED_CONFDATA;
    CDialog::OnCancel();
}

/**
 * \brief On Ok
 *
 * Message handlers on Enter Button ( Default OK button)
 *  Every press of enter key, focus is to next control
 */
void CChangeRegisters_CAN_ICS_neoVI::OnOK()
{
    // Dummy virtual function to avoid closing the dialog when ENTER key is
    //  pressed. Instead next conrol gets focus in tab order
    NextDlgCtrl();
}

/**
 * \brief Get Formatted Register Value
 *
 * Formats the input register value as 0xYY
 */
CString CChangeRegisters_CAN_ICS_neoVI::omGetFormattedRegVal(UCHAR ucRegVal)
{
    CString omStr = "";
    omStr.Format(TEXT("0x%X"), ucRegVal);

    // Insert one zero to format the sigle digit value to 0x05 etc.
    if (omStr.GetLength() == 3)
    {
        omStr.Insert(2, '0');
    }

    return omStr;
}

/**
 * \brief Display List Box
 *
 * Insert columns of each row in the list control
 * Calls function vSelSetFocusItemList to set focus
 */
void CChangeRegisters_CAN_ICS_neoVI:: vDisplayListBox(INT nEntries, INT nItemFocus)
{
    CString cStrText    = "";
    // Clear list box first
    m_omListCtrlBitTime.DeleteAllItems();
    // Get the total number of Columns in the List View Header
    INT nColumnCount = m_omListCtrlBitTime.GetHeaderCtrl()->GetItemCount();

    // Insert items and subitems after Formating the strings in the list view
    // control.
    for (INT i = 0; i < nEntries; i++)
    {
        // First column contains value of CNF1 register
        cStrText = omGetFormattedRegVal(m_asColListCtrl[i].uCNFReg1.ucCNF1);
        // Add the first column entry of the current row
        m_omListCtrlBitTime.InsertItem(LVIF_TEXT | LVIF_STATE, i, cStrText,
                                       (i == 0) ? LVIS_SELECTED : 0, LVIS_SELECTED, 0, 0);
        // Set item data for sorting on column value basis
        m_omListCtrlBitTime.SetItemData(i, (LPARAM) &(m_asColListCtrl[i]));

        // Format and Initialize the text of the subitems.
        for (INT j = 1; j < nColumnCount; j++)
        {
            // Format the text of the subitems.
            if (1 == j) // Second column: CNF2 register value
            {
                cStrText = omGetFormattedRegVal(m_asColListCtrl[i].uCNFReg2.ucCNF2);
            }
            else if (2 == j) // Third column: CNF3 register value
            {
                cStrText = omGetFormattedRegVal(m_asColListCtrl[i].uCNFReg3.ucCNF3);
            }
            else if (3 == j) // Fourth column: sampling value
            {
                cStrText.Format(TEXT("%d%%"), m_asColListCtrl[i].sBRPNBTSampNSJW.usSampling);
            }
            else if (4 == j) // Fifth column: value of NBT
            {
                cStrText.Format(TEXT("%d"), m_asColListCtrl[i].sBRPNBTSampNSJW.usNBT);
            }
            else if (5 == j) // Sixth column: BRP value
            {
                cStrText.Format(TEXT("%d"), m_asColListCtrl[i].sBRPNBTSampNSJW.usBRP);
            }

            // Initialize the text of the subitems.
            if (m_omListCtrlBitTime.SetItemText(i, j, cStrText) == 0)
            {
                AfxMessageBox(defERRORMSG_INSERT);
            }
        }
    }

    //  Set the focus on item
    vSelSetFocusItemList(nEntries, nItemFocus);
}

/**
 * \brief On Kill Focus Edit Baud Rate
 *
 * Validate the buadrate on kill focus of this edit control.
 */
void CChangeRegisters_CAN_ICS_neoVI::OnKillfocusEditBaudRate()
{
    CString omStrBaudRate   = "";
    CString omStrValid      = "";
    INT     nLength         = 0;
    m_omEditBaudRate.GetWindowText(omStrBaudRate);
    nLength             = omStrBaudRate.GetLength();
    CButton* pomButtonCancel = (CButton*) GetDlgItem(IDCANCEL);
    // To get the state of CANCEL button. A non zero value if the button is
    // clicked.
    UINT unButtonState       = pomButtonCancel->GetState();

    // Validate only if next command is not ESC Button
    if (m_bDialogCancel != TRUE )
    {
        // Don't validate if CANCEL button is clicked.
        if (unButtonState ==0)
        {
            // Validate for empty string and if zero value is entered.
            DOUBLE dBaudRate = (FLOAT)_tstof(omStrBaudRate);

            if (nLength == 0 || dBaudRate <= 0 || dBaudRate > 1000.0)
            {
                m_omEditBaudRate.SetWindowText(m_omStrEditBaudRate);
                AfxMessageBox(defVALIDATION_MSG_BAUD_RATE);
                m_omEditBaudRate.SetFocus();
                m_omEditBaudRate.SetSel(0, -1,FALSE);
            }
            else
            {
                m_dEditBaudRate     = (FLOAT)_tstof(m_omStrEditBaudRate);

                // Call if string is valid to validate the baud rate value and
                // suggest  a next valid baud rate
                //Validate only if previous value in edit control is not the
                //  same as the one changed by user
                if (m_dEditBaudRate != dBaudRate && dBaudRate>0
                        && m_dEditBaudRate > 0 )
                {
                    vValidateBaudRate();
                    // Update List items only it is from edit box
                    vChangeListBoxValues(-1);
                    CButton* pomButtonoK = (CButton*) GetDlgItem(IDC_ButtonOK);
                    CButton* pomFocusWnd     = (CButton*)GetFocus();

                    if (pomButtonoK ==pomFocusWnd)
                    {
                        // Close the dialog if the user
                        // has pressed OK button
                        OnClickedOK();
                    }
                }
            }
        }
    }
    else
    {
        m_omEditBaudRate.SetWindowText(m_omStrEditBaudRate);
    }
}

/**
 * \brief On Selection Change Combo Sampling
 *
 * Change the content of list control on change in
 * selection of number of sampling combo box.
 */
void CChangeRegisters_CAN_ICS_neoVI::OnSelchangeCombSampling()
{
    INT nGetValue               = 0;
    CString omStrComboEditItem  =_T("");
    nGetValue =  m_omCombSampling.GetCurSel();

    if (nGetValue != CB_ERR)
    {
        m_omCombSampling.GetLBText(nGetValue, omStrComboEditItem);
    }

    if (m_omStrComboSampling != omStrComboEditItem)
    {
        vChangeListBoxValues(-1);
        m_omStrComboSampling = omStrComboEditItem;
    }
}

/**
 * \brief     Change List Box Values
 * \param[in] nFlag Flag to indicate from where this function is called
 *
 * Change the content of list control on change in
 * selection of number of sampling or clock, or BTR0
 * or BTR1 or baudrate combo box.
 */
void CChangeRegisters_CAN_ICS_neoVI::vChangeListBoxValues(INT nflag)
{
    UINT unIndex = 0;

    // Call function to calculate the list of BTR0, BTR1, SJW,NBT and Sampling.
    if (nListBoxValues(m_asColListCtrl, m_dEditBaudRate, (WORD)m_unCombClock,
                       &unIndex, nGetValueFromComboBox(m_omCombSampling)) != -1)
    {
        // Remove all the items in the list box.
        m_omListCtrlBitTime.DeleteAllItems();
        // Display all the new items in the list box
        vDisplayListBox(unIndex, nflag);
    }
    else
    {
        vValidateBaudRate();
    }
}

/**
 * \brief On Set Focus Edit Baud Rate
 *
 * Called when focus is set on baudrate edit box control 
 * Update all data members associated with Dialog control.
 */
void CChangeRegisters_CAN_ICS_neoVI::OnSetfocusEditBaudRate()
{
    // To update the data members before editing it and use it in kill focus
    UpdateData(TRUE);
}

/**
 * \brief     Select Set Focus Item List
 * \param[in] nItemCount Total number of item in list control
 * \param[in] nItem Item number for selection from list control
 *
 * Called when focus is set on baudrate edit box control
 * Update all data members associated with Dialog control.
 */
void CChangeRegisters_CAN_ICS_neoVI::vSelSetFocusItemList(INT nItemCount, INT nItem)
{
    LVITEM sItem;

    // If there is no defualt or last selected item, selection will be at
    // item number middle of the total item numbers.
    if (nItem ==-1 || nItem>=nItemCount)
    {
        nItem = 0;
    }

    sItem.mask      = LVIF_STATE;
    sItem.iItem     = nItem;
    sItem.state     = LVIS_FOCUSED|LVIS_SELECTED;
    sItem.stateMask = LVIS_SELECTED |LVIS_FOCUSED;
    sItem.iSubItem  = 0;
    m_omListCtrlBitTime.SetItem(&sItem);
    m_omListCtrlBitTime.EnsureVisible(nItem, FALSE);
}

/**
 * \brief Validate Baud Rate
 *
 * This function will validate the user input value of
 * baud rate. A valid baud rate will be calculated.
 */
void CChangeRegisters_CAN_ICS_neoVI::vValidateBaudRate()
{
    CString omStrBaudRate       = "";
    CString omStrPrvBaudRate    = "";
    CString omStrClockFreq      = "";
    DOUBLE  dBaudRate           = 0;
    //UINT    unClockFreq         = 0;
    //UINT    unClockPrevValue    = 0 ;
    UINT    unProductNbtNBrp    = 0;
    DOUBLE  dProductNbtNBrp     = 0;
    CString omStrMessage        = "";
    m_omEditBaudRate.GetWindowText(omStrBaudRate);
    dBaudRate           = (FLOAT)_tstof(omStrBaudRate);
    m_dEditBaudRate     = (FLOAT)_tstof(m_omStrEditBaudRate);
    //m_omCombClock.GetWindowText(omStrClockFreq);
    //unClockFreq          = _tstoi(omStrClockFreq);
    dProductNbtNBrp     = (DOUBLE)(m_unCombClock/dBaudRate)/2.0 *
                          (defFACT_FREQUENCY / defFACT_BAUD_RATE);
    unProductNbtNBrp    = (UINT)(dProductNbtNBrp + 0.5);

    if ((fabs((dProductNbtNBrp - unProductNbtNBrp)) > defVALID_DECIMAL_VALUE)
            ||(unProductNbtNBrp > (defMAX_NBT * defMAX_BRP))
            || (unProductNbtNBrp < defMIN_NBT))
    {
        unProductNbtNBrp =defmcROUND5(dProductNbtNBrp);
        INT nFlag = defRESET;

        while (nFlag == defRESET)
        {
            INT i = 1;
            UINT unNbt = unProductNbtNBrp / i;
            FLOAT fNbt = (FLOAT)unProductNbtNBrp / i;

            while ((unNbt >= 1) && (i <= defMAX_BRP) && (nFlag == defRESET))
            {
                if ((unNbt == fNbt) && (unNbt >= defMIN_NBT)
                        && (unNbt <=defMAX_NBT))
                {
                    nFlag =defSET;
                }
                else
                {
                    i++;
                    unNbt    = unProductNbtNBrp / i;
                    fNbt     = (FLOAT)unProductNbtNBrp / i;
                }
            } //end while( unNbt >=1 && i<=MAX_BRP)

            if ((nFlag == defRESET) && (unProductNbtNBrp < (defMIN_NBT *defMIN_BRP)))
            {
                unProductNbtNBrp = defMIN_NBT * defMIN_BRP;
            }
            else if ((unProductNbtNBrp > ( defMAX_NBT * defMAX_BRP))
                     && (nFlag == defRESET))
            {
                unProductNbtNBrp = defMAX_NBT*defMAX_BRP;
            }
            else if (nFlag == defRESET)
            {
                unProductNbtNBrp++;
            }
        }//end while(nFlag==RESET)

        dBaudRate = (DOUBLE)((m_unCombClock/2.0)*
                             ( defFACT_FREQUENCY / defFACT_BAUD_RATE))/unProductNbtNBrp;
        FLOAT  fTempBaudRate;
        fTempBaudRate = (FLOAT)((INT)(dBaudRate * 100000));
        fTempBaudRate = fTempBaudRate/100000;
        omStrBaudRate.Format(_T("%.4f"),fTempBaudRate);
        omStrMessage.Format(defBAUD_RATE_MESSAGE,omStrBaudRate);
        omStrPrvBaudRate = m_omStrEditBaudRate;
        //unClockPrevValue = m_unCombClock;
        // set the baudrate
        m_omEditBaudRate.SetWindowText(omStrBaudRate);
    }// End if

    // Change the list of BTR0, BTR1, SJW, NBT and sampling if user selected YES
    m_dEditBaudRate     = dBaudRate;
    m_omStrEditBaudRate = omStrBaudRate;
    //m_unCombClock       = unClockFreq;
}

/**
 * \brief On Clicked OK
 *
 * Message handlers on OK Button.To Remove control
 * to close when Enter Button is pressed
 */
void CChangeRegisters_CAN_ICS_neoVI::OnClickedOK()
{
    // Update modified data
    UpdateData( TRUE);
    // Validate Baud rate and find the nearest match
    vValidateBaudRate();

    // Update data members associated with the controller
    if (bUpdateControllerDataMembers() == FALSE)
    {
        return;
    }

    // Save the changes in to the local data structure
    vUpdateControllerDetails();
    // Close the dialog
    m_nDataConfirmStatus = INFO_INIT_DATA_CONFIRMED;
    CDialog::OnOK();
}

/**
 * \brief On Set Focus Combo Sampling
 *
 * Called when focus is set on Number of sampling
 * combo box control. Updates all data members
 * associated with Dialog control.
 */
void CChangeRegisters_CAN_ICS_neoVI::OnSetfocusCombSampling()
{
    UpdateData(TRUE);
}

/**
 * \brief  Calculate Baud Rate From BTRs
 * \return baudrate in kbps
 *
 * This function is called to calcualte the baudrate for
 * the provided CNF1, CNF2 and CNF3 values.
 */
DOUBLE CChangeRegisters_CAN_ICS_neoVI::dCalculateBaudRateFromBTRs(CString omStrCNF1, CString omStrCNF2,
        CString omStrCNF3)
{
    uCNF1 uCNF1val;
    uCNF2 uCNF2val;
    uCNF3 uCNF3val;
    DOUBLE dBaudRate = 0;
    BYTE   bTSEG2 = 0;
    TCHAR* pcStopStr = NULL;
    uCNF1val.ucCNF1 =
        static_cast <UCHAR >(_tcstol((LPCTSTR)omStrCNF1.GetBuffer(MAX_PATH),&pcStopStr,defHEXADECIMAL));
    uCNF2val.ucCNF2 =
        static_cast <UCHAR >(_tcstol((LPCTSTR)omStrCNF2.GetBuffer(MAX_PATH),&pcStopStr,defHEXADECIMAL));
    uCNF3val.ucCNF3 =
        static_cast <UCHAR >(_tcstol((LPCTSTR)omStrCNF3.GetBuffer(MAX_PATH),&pcStopStr,defHEXADECIMAL));
    BYTE bBRP = static_cast <BYTE> (uCNF1val.sCNF1Bit.ucBRPbit + 1);
    BYTE bTSEG1 = static_cast <BYTE> (uCNF2val.sCNF2Bit.ucTSEG1bit + 1);
    BYTE bPROPDELAY = static_cast <BYTE> (uCNF2val.sCNF2Bit.ucPropDelaybit + 1);
    BYTE bFLAG = static_cast <BYTE> (uCNF2val.sCNF2Bit.ucFLAGbit + 1);

    if (bFLAG)
    {
        bTSEG2 = static_cast <BYTE> (uCNF3val.sCNF3Bit.ucTSEG2bit + 1);
    }
    else
    {
        bTSEG2 = bTSEG1 ;
    }

    BYTE bNBT = static_cast <BYTE> (bTSEG1 + bTSEG2 + bPROPDELAY + 1);
    dBaudRate = (DOUBLE)((defICSneoVIFrequency) / (2.0 * bBRP * bNBT));
    return dBaudRate / 1000;
}

/**
 * \brief Fill Controller Config
 *
 * This function is called to read registry  or ini file
 * and return the baudrate ( BTR0 and BTR1). If there is
 * no entry return the default value
 */
BOOL CChangeRegisters_CAN_ICS_neoVI::bFillControllerConfig()
{
    BOOL bReturn = FALSE;

    // If successful then set the result to pass
    if (m_pControllerDetails != NULL)
    {
        bReturn = TRUE;
    }

    // Return the result
    return bReturn;
}

/**
 * \brief Destructor
 *
 * This is destructor of the class. It is called when
 * object of this class is being destroyed. All memory
 * allocation is deleted here.
 */
CChangeRegisters_CAN_ICS_neoVI::~CChangeRegisters_CAN_ICS_neoVI()
{
}

/**
 * \brief On Item Changed List Control
 *
 * This function will be called by framework when item in
 * list control is changed. Based on the index of
 * selected and focused item, the other field will be
 * updated by calling vUpdateBTRsBRPEditWindow(..)
 */
void CChangeRegisters_CAN_ICS_neoVI::OnItemchangedLstcBtrList(NMHDR* pNMHDR, LRESULT* pResult)
{
    NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
    TRACE("OnItemchangedLstcBtrList --- %d\n", pNMListView->iItem);
    *pResult = 0;
}

/**
 * \brief      On Click List Channels
 * \param[in]  pNMHDR Pointer to Notification Block
 * \param[out] pResult Pointer to the result
 *
 * This function will be called when the user clicks the channel
 * list item. This will set the focus to the last selected item
 * if user clicks outside.
 */
void CChangeRegisters_CAN_ICS_neoVI::OnClickListChannels(NMHDR* /*pNMHDR*/, LRESULT* pResult)
{
    // Get the selection mask
    UINT unItemStateMask = LVNI_SELECTED|LVNI_FOCUSED;
    // Get the selected item index
    int nSel = m_omChannelList.GetNextItem( -1, LVNI_SELECTED);

    // If nothing is selected then set the selection to the last saved index
    if (nSel == -1)
    {
        m_omChannelList.SetItemState( m_nLastSelection,
                                      unItemStateMask,
                                      unItemStateMask);
    }

    *pResult = 0;
}

/**
 * \brief      On Item Changed List Channels
 * \param[in]  pNMHDR Pointer to the list item struct
 * \param[out] pResult Pointer to the result value
 *
 * This function will update baudrate information of selected
 * channel
 */
void CChangeRegisters_CAN_ICS_neoVI::OnItemchangedListChannels(NMHDR* pNMHDR, LRESULT* pResult)
{
    // Get the List item data from the notification
    NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
    // Create selection mask
    UINT unItemStateMask = LVIS_SELECTED | LVIS_FOCUSED;

    // If new state is selected then show selected channel details
    if (pNMListView->uNewState == unItemStateMask)
    {
        // Set the selection
        m_nLastSelection = pNMListView->iItem;
        // Update the UI Controls with the
        vFillControllerConfigDetails();
    }
    // If it is lose of focus then save the user changes
    else if (pNMListView->uChanged  == LVIF_STATE &&
             pNMListView->uOldState == LVIS_SELECTED)
    {
        // Update modified data
        UpdateData( TRUE);

        if (bUpdateControllerDataMembers() == FALSE)
        {
            return;
        }

        // Validate Baud rate and find the nearest match
        vValidateBaudRate();
        // Save the changes in to the local data structure
        vUpdateControllerDetails();
    }

    *pResult = 0;
}

/**
 * \brief      On Doubleclick List Channels
 * \param[in]  pNMHDR Pointer to Notification Block
 * \param[out] pResult Pointer to the result
 *
 * This function will be called wher the user double clicks the
 * channel list item. This will set the focus to the last
 * selected item if user clicks outside
 */
void CChangeRegisters_CAN_ICS_neoVI::OnDblclkListChannels(NMHDR* /*pNMHDR*/, LRESULT* pResult)
{
    // Create selection mask
    UINT unItemStateMask = LVNI_SELECTED | LVNI_FOCUSED;
    // Get current selection
    int nSel = m_omChannelList.GetNextItem( -1, LVNI_SELECTED);

    // If nothing got selected restore last selection
    if (nSel == -1)
    {
        m_omChannelList.SetItemState( m_nLastSelection,
                                      unItemStateMask,
                                      unItemStateMask);
    }

    *pResult = 0;
}

/**
 * \brief Fill Controller Config Details
 *
 * This function will fill details of selected channel in to the
 * member variables used. This will also update the BTR0 and
 * BTR1 registers value and list box of possible values for the
 * selected baudrate.
 */
void CChangeRegisters_CAN_ICS_neoVI::vFillControllerConfigDetails()
{
    int nIndex = m_nLastSelection;
    /* Add hardware info to the description field */
    CWnd* pWnd = GetDlgItem(IDC_EDIT_CHANNEL_DESC);

    if (pWnd != NULL)
    {
        pWnd->SetWindowText(m_pControllerDetails[nIndex].m_omHardwareDesc);
    }

    m_omStrEditBaudRate     = m_pControllerDetails[ nIndex ].m_omStrBaudrate;
    m_omStrEditCNF1         = m_pControllerDetails[ nIndex ].m_omStrCNF1;
    m_omStrEditCNF2         = m_pControllerDetails[ nIndex ].m_omStrCNF2;
    m_omStrEditCNF3         = m_pControllerDetails[ nIndex ].m_omStrCNF3;
    //m_omStrComboClock       = m_pControllerDetails[ nIndex ].m_omStrClock;
    m_omStrComboSampling    = m_pControllerDetails[ nIndex ].m_omStrSampling;
    m_omStrEditWarningLimit = m_pControllerDetails[ nIndex ].m_omStrWarningLimit;
    m_omStrPropDelay = m_pControllerDetails[ nIndex ].m_omStrPropagationDelay;
    m_omStrSJW = m_pControllerDetails[ nIndex ].m_omStrSjw;
    int nSample             = _tstoi(m_omStrComboSampling);
    //omStrInitComboBox(ITEM_SAMPLING,1,m_omCombSampling));
    //Assign edit box string value to CString member variable of Edit control
    // for Baudrate Convert String into float or INT to be used to make a list
    // of all possible  of BTRi, SJW, Sampling Percentage, and NBT values
    //m_unCombClock       = (UINT)_tstoi(m_omStrComboClock);
    // TO BE FIXED LATER
    m_dEditBaudRate =
        dCalculateBaudRateFromBTRs( m_omStrEditCNF1, m_omStrEditCNF2, m_omStrEditCNF3);
    UpdateData(FALSE);
    unsigned int unIndex = 0;
    int nReturn = nListBoxValues(m_asColListCtrl, m_dEditBaudRate,
                                 (WORD)m_unCombClock, &unIndex,nSample);

    // if Function returns Success display the item and set the focus to last
    // saved item or at item which is at the mid of the list. Update edit boxes
    // for BRP, BTRi by calling function vUpdateBtriBrpEditWindow(..,..)
    if (nReturn == defSUCCESS)
    {
        // read the item to set the focus from the configuration
        if (m_pControllerDetails != NULL)
        {
            nReturn = m_pControllerDetails[m_nLastSelection].m_nItemUnderFocus;
        }

        vDisplayListBox(unIndex, nReturn);
    }
}

/**
 * \brief Update Controller Details
 *
 * This function will save the user enter values for baud rate
 * into the controller configuration structure
 */
void CChangeRegisters_CAN_ICS_neoVI::vUpdateControllerDetails()
{
    TCHAR*    pcStopStr              = NULL;
    CString omStrComboSampling      = "";
    CString omStrEditBtr0           = "";
    CString omStrEditBtr1           = "";
    CString omStrEditAcceptanceCode = "";
    CString omStrEditAcceptanceMask = "";
    // Update the data members before writing into ini file or registry.
    UpdateData(TRUE);
    // Get the warning limit.
    UINT unWarningLimit = 0;
    unWarningLimit = static_cast <UINT>(_tcstol((LPCTSTR)
                                        m_omStrEditWarningLimit,
                                        &pcStopStr,defBASE_DEC));
    UINT unWarningLimtMin = static_cast <UINT> (defWARNING_LIMIT_MIN);
    UINT unWarningLimtMax = static_cast <UINT> (defWARNING_LIMIT_MAX);

    if (  ( unWarningLimit >= unWarningLimtMin)
            && ( unWarningLimit <= unWarningLimtMax))
    {
        INT nItemUnderFocus = m_omListCtrlBitTime.GetNextItem(-1, LVNI_SELECTED);
        m_ucWarningLimit = static_cast <UCHAR> (unWarningLimit);
        UCHAR  ucBtr0                   = 0;
        UCHAR  ucBtr1                   = 0;
        // Pack the BTR0 and BTR1 values in two bytes before calling DIL fuction
        // to initialise.
        m_usBTR0BTR1 = static_cast <USHORT>(((ucBtr0 << 8)| ucBtr1) & 0xffff);
        m_pControllerDetails[ m_nLastSelection ].m_nItemUnderFocus   =
            nItemUnderFocus;
        strcpy_s(m_pControllerDetails[ m_nLastSelection ].m_omStrBaudrate, m_omStrEditBaudRate.GetBuffer(MAX_PATH));
        //m_pControllerDetails[ m_nLastSelection ].m_omStrClock        =
        //                                                    m_omStrComboClock;
        strcpy_s(m_pControllerDetails[m_nLastSelection].m_omStrCNF1, m_omStrEditCNF1.GetBuffer(MAX_PATH));
        strcpy_s(m_pControllerDetails[m_nLastSelection].m_omStrCNF2, m_omStrEditCNF2.GetBuffer(MAX_PATH));
        strcpy_s(m_pControllerDetails[m_nLastSelection].m_omStrCNF3, m_omStrEditCNF3.GetBuffer(MAX_PATH));
        sprintf_s(m_pControllerDetails[m_nLastSelection].m_omStrClock, _T("%d"), m_unCombClock);
        strcpy_s(m_pControllerDetails[ m_nLastSelection ].m_omStrSampling, m_omStrComboSampling.GetBuffer(MAX_PATH));
        strcpy_s(m_pControllerDetails[ m_nLastSelection ].m_omStrWarningLimit, m_omStrEditWarningLimit.GetBuffer(MAX_PATH));
        strcpy_s(m_pControllerDetails[ m_nLastSelection ].m_omStrPropagationDelay, m_omStrPropDelay.GetBuffer(MAX_PATH));
        strcpy_s(m_pControllerDetails[ m_nLastSelection ].m_omStrSjw, m_omStrSJW.GetBuffer(MAX_PATH));
    }
    else
    {
        // Invalid Warning Limit Error Message
        CString omStrMsg = "";
        omStrMsg.Format( defWARNINGLIMIT_MSG, m_omStrEditWarningLimit,
                         defWARNING_LIMIT_MIN,
                         defWARNING_LIMIT_MAX);
        m_omEditWarningLimit.SetFocus();
        m_omEditWarningLimit.SetSel(0, -1,FALSE);
    }
}

/**
 * \brief Set Baud Rate From Com
 *
 * This function will be called from COM function to set baud rate
 */
BOOL CChangeRegisters_CAN_ICS_neoVI::bSetBaudRateFromCom(int nChannel, BYTE bBTR0, BYTE bBTR1)
{
    BOOL bReturn = FALSE;
    CString omStrBtr0;
    CString omStrBtr1;
    CString omStrBaudRate;
    USHORT m_usBTR0BTR1;
    UINT unIndex                    = 0;
    UINT unClock                    = 0;
    UINT unSample                   = 1;
    // TO BE FIXED LATER
    double dBaudRate = dCalculateBaudRateFromBTRs(omStrBtr0,omStrBtr1, omStrBtr1);
    omStrBaudRate.Format("%f",dBaudRate);
    m_usBTR0BTR1 = static_cast <USHORT>(((bBTR0 << 8)| bBTR1) & 0xffff);
    //Save the changes for the channels
    unClock       = (UINT)_tstoi(m_pControllerDetails[ nChannel-1 ].m_omStrClock);

    if ((bBTR1 & 0x80) != 0)
    {
        unSample          =  3 ;
    }

    UINT nReturn  = nListBoxValues(m_asColListCtrl, dBaudRate, (WORD)unClock,
                                   &unIndex, unSample);

    if (nReturn != -1)
    {
        for (UINT i = 0; i<unIndex; i++)
        {
            //if (( bBTR0 == (m_asColListCtrl[i].uBTRReg0.ucBTR0))&&
            //    ( bBTR1 == (m_asColListCtrl[i].uBTRReg1.ucBTR1)))
            {
                m_pControllerDetails[nChannel-1 ].m_nItemUnderFocus = i;
            }
        }
    }

    m_pControllerDetails[ nChannel-1 ].m_nBTR0BTR1 = m_usBTR0BTR1;
    strcpy_s(m_pControllerDetails[ nChannel-1 ].m_omStrBaudrate, omStrBaudRate.GetBuffer(MAX_PATH));
    strcpy_s(m_pControllerDetails[nChannel - 1].m_omStrCNF1, m_omStrEditCNF1.GetBuffer(MAX_PATH));
    strcpy_s(m_pControllerDetails[nChannel - 1].m_omStrCNF2, m_omStrEditCNF2.GetBuffer(MAX_PATH));
    strcpy_s(m_pControllerDetails[nChannel - 1].m_omStrCNF3, m_omStrEditCNF3.GetBuffer(MAX_PATH));
    sprintf_s(m_pControllerDetails[ nChannel-1  ].m_omStrSampling, _T("%d"), unSample);
    m_nLastSelection = nChannel-1;
    // Update Configuration file
    //kadoor theApp.bSetData( CONTROLLER_DETAILS, m_pControllerDetails);
    //// Update Hardware Interface Layer
    //if (g_podHardwareInterface->bLoadDataFromConfig() == TRUE)
    //{
    //    int nApply = g_podHardwareInterface->nSetApplyConfiguration();
    //    if (nApply ==defERR_OK)
    //    {
    //        bReturn =TRUE;
    //    }
    //}
    return bReturn;
}

/**
 * \brief Get Baud Rate From Com
 *
 * This function will be called from COM function to set baud rate
 */
BOOL CChangeRegisters_CAN_ICS_neoVI::bGetBaudRateFromCom(int nChannel, BYTE& bBTR0, BYTE& bBTR1)
{
    BOOL bReturn =FALSE;

    if (m_pControllerDetails != NULL)
    {
        int nTempBTR0BTR1 = m_pControllerDetails[ nChannel-1 ].m_nBTR0BTR1;
        bBTR1 = (BYTE)(nTempBTR0BTR1 & 0XFF);
        bBTR0 = (BYTE)((nTempBTR0BTR1>>defBITS_IN_BYTE ) & 0XFF);
        bReturn=TRUE;
    }

    return bReturn;
}

/**
 * \brief      Set Filter From Com
 * \param[in]  nExtended for extended msg or not
 * \param[in]  dBeginMsgId filter's msg id start
 * \param[in]  dEndMsgId filter's msg id stop
 * \param[out] Operation Result. 0 in case of no errors. Failure Error codes otherwise.
 *
 * This function will set the filter information if called using
 * com interface.
 */
BOOL CChangeRegisters_CAN_ICS_neoVI::bSetFilterFromCom(BOOL bExtended, DWORD dBeginMsgId,
        DWORD dEndMsgId)
{
    BOOL bReturn = FALSE;
    // for getting separate byte
    DWORD dTemp=0XFF;

    for (UINT unIndex = 0;
            unIndex < defNO_OF_CHANNELS;
            unIndex++)
    {
        // To set no. shifts
        int nShift = sizeof( UCHAR) * defBITS_IN_BYTE;
        //to convert all acceptance and mask byets into string
        CString omStrTempByte;
        // Create Code
        omStrTempByte.Format(_T("%02X"),(dTemp & ( dBeginMsgId)));
        strcpy_s(m_pControllerDetails[ unIndex ].m_omStrAccCodeByte4[bExtended],
                 omStrTempByte.GetBuffer(MAX_PATH));
        omStrTempByte.Format(_T("%02X"),(dTemp & ( dBeginMsgId >> nShift)));
        strcpy_s(m_pControllerDetails[ unIndex ].m_omStrAccCodeByte3[bExtended],
                 omStrTempByte.GetBuffer(MAX_PATH));
        omStrTempByte.Format(_T("%02X"),(dTemp & ( dBeginMsgId >> nShift * 2)));
        strcpy_s(m_pControllerDetails[ unIndex ].m_omStrAccCodeByte2[bExtended],
                 omStrTempByte.GetBuffer(MAX_PATH));
        omStrTempByte.Format(_T("%02X"),(dTemp & ( dBeginMsgId >> nShift * 3)));
        strcpy_s(m_pControllerDetails[ unIndex ].m_omStrAccCodeByte1[bExtended],
                 omStrTempByte.GetBuffer(MAX_PATH));
        // Create Mask
        omStrTempByte.Format(_T("%02X"),(dTemp & ( dEndMsgId)));
        strcpy_s(m_pControllerDetails[ unIndex ].m_omStrAccMaskByte4[bExtended],
                 omStrTempByte.GetBuffer(MAX_PATH));
        omStrTempByte.Format(_T("%02X"),(dTemp & ( dEndMsgId >> nShift)));
        strcpy_s(m_pControllerDetails[ unIndex ].m_omStrAccMaskByte3[bExtended],
                 omStrTempByte.GetBuffer(MAX_PATH));
        omStrTempByte.Format(_T("%02X"),(dTemp & ( dEndMsgId >> nShift * 2)));
        strcpy_s(m_pControllerDetails[ unIndex ].m_omStrAccMaskByte2[bExtended],
                 omStrTempByte.GetBuffer(MAX_PATH));
        omStrTempByte.Format(_T("%02X"),(dTemp & ( dEndMsgId >> nShift * 3)));
        strcpy_s(m_pControllerDetails[ unIndex ].m_omStrAccMaskByte1[bExtended],
                 omStrTempByte.GetBuffer(MAX_PATH));
        m_pControllerDetails[ unIndex ].m_bAccFilterMode = bExtended;
    }

    //kadoor // Update Configuration file
    //theApp.bSetData( CONTROLLER_DETAILS, m_pControllerDetails);
    //// Update Hardware Interface Layer
    //if (g_podHardwareInterface->bLoadDataFromConfig() == TRUE)
    //{
    //    int nApply = g_podHardwareInterface->nSetApplyConfiguration();
    //    if (nApply ==defERR_OK)
    //    {
    //        bReturn =TRUE;
    //    }
    //}
    return bReturn;
}

/**
 * \brief      Get Filter From Com
 * \param[out] nExtended for extended msg or not
 * \param[out] dBeginMsgId acceptance code
 * \param[out] dEndMsgId mask code
 * \return     Operation Result. 0 incase of no errors. Failure Error codes otherwise.
 *
 * This function will set the filter information if called using
 * com interface.
 */
BOOL CChangeRegisters_CAN_ICS_neoVI::bGetFilterFromCom(BOOL& bExtended, double& dBeginMsgId,
        double& dEndMsgId)
{
    BOOL bReturn = FALSE;

    if (m_pControllerDetails != NULL)
    {
        TCHAR* pcStopStr ;
        //Change to separate integer value for each byte
        int nAccCodeByte1 = _tcstol((LPCTSTR)m_pControllerDetails[ 0 ].m_omStrAccCodeByte1,
                                    &pcStopStr,defHEXADECIMAL);
        int nAccCodeByte2 = _tcstol((LPCTSTR)m_pControllerDetails[ 0 ].m_omStrAccCodeByte2,
                                    &pcStopStr,defHEXADECIMAL);
        int nAccCodeByte3 = _tcstol((LPCTSTR)m_pControllerDetails[ 0 ].m_omStrAccCodeByte3,
                                    &pcStopStr,defHEXADECIMAL);
        int nAccCodeByte4 = _tcstol((LPCTSTR)m_pControllerDetails[ 0 ].m_omStrAccCodeByte4,
                                    &pcStopStr,defHEXADECIMAL);
        int nMaskCodeByte1 = _tcstol((LPCTSTR)m_pControllerDetails[ 0 ].m_omStrAccMaskByte1,
                                     &pcStopStr,defHEXADECIMAL);
        int nMaskCodeByte2 = _tcstol((LPCTSTR)m_pControllerDetails[ 0 ].m_omStrAccMaskByte2,
                                     &pcStopStr,defHEXADECIMAL);
        int nMaskCodeByte3 = _tcstol((LPCTSTR)m_pControllerDetails[ 0 ].m_omStrAccMaskByte3,
                                     &pcStopStr,defHEXADECIMAL);
        int nMaskCodeByte4 = _tcstol((LPCTSTR)m_pControllerDetails[ 0 ].m_omStrAccMaskByte4,
                                     &pcStopStr,defHEXADECIMAL);
        //now make them as dword in decimal
        dBeginMsgId = (ULONG)(nAccCodeByte1*0X1000000+nAccCodeByte2*0X10000+
                              nAccCodeByte3*0X100+nAccCodeByte4);
        dEndMsgId = (ULONG)(nMaskCodeByte1*0X1000000+nMaskCodeByte2*0X10000+
                            nMaskCodeByte3*0X100+nMaskCodeByte4);
        bExtended=  m_pControllerDetails[ 0 ].m_bAccFilterMode;
        bReturn=TRUE;
    }

    return bReturn;
}

/**
 * \brief Callback Sort Function
 * \param[in] lParam1 One of the two rows in the list box
 * \param[in] lParam2 The other row in the list box
 * \param[in] lColumn Column information
 *
 * Callback function when the user clicks on the list control
 * header.
 */
int CALLBACK CallbackSortFunction(LPARAM lParam1, LPARAM lParam2, LPARAM lColumn)
{
    int Result = 0;
    sCOLUMNS& sEntry1 = *((sCOLUMNS*) lParam1);
    sCOLUMNS& sEntry2 = *((sCOLUMNS*) lParam2);

    switch (lColumn)
    {
        case 0:
        {
            Result = sEntry1.uCNFReg1.ucCNF1 - sEntry2.uCNFReg1.ucCNF1;
        }
        break;

        case 1:
        {
            Result = sEntry1.uCNFReg2.ucCNF2 - sEntry2.uCNFReg2.ucCNF2;
        }
        break;

        case 2:
        {
            Result = sEntry1.uCNFReg3.ucCNF3 - sEntry2.uCNFReg3.ucCNF3;
        }
        break;

        default:
            break;
    }

    return Result;
}

/**
 * \brief      On Item Click List Control
 * \param[in]  pNMHDR Contains header information
 * \param[out] pResult Result (out parameter)
 *
 * Handler for the event of the user's click on the list control
 * header.
 */
void CChangeRegisters_CAN_ICS_neoVI::OnHdnItemclickLstcBtrList(NMHDR* pNMHDR, LRESULT* pResult)
{
    NMLISTVIEW* pLV = (NMLISTVIEW*) pNMHDR;
    m_omListCtrlBitTime.SortItems(CallbackSortFunction, pLV->iItem);
    *pResult = 0;
}

/**
 * \brief On Combo Button Selection Change SJW
 *
 * Handler when the user selects a specific value in the SJW list
 */
void CChangeRegisters_CAN_ICS_neoVI::OnCbnSelchangeCombSjw()
{
    int nSJWCurr = nGetValueFromComboBox(m_omCtrlSJW);

    if (nSJWCurr != m_nSJWCurr)
    {
        m_nSJWCurr = nSJWCurr;
        bDecideCalculatingOption();
        vChangeListBoxValues(-1);
    }
}

/**
 * \brief On Combo Button Selection Change Propagation Delay
 *
 * Handler when the user selects a specific value in the PD list
 */
void CChangeRegisters_CAN_ICS_neoVI::OnCbnSelchangeCombPropdelay()
{
    int nPropDelay = nGetValueFromComboBox(m_omCtrlPropDelay);

    if (nPropDelay != m_nPropDelay)
    {
        m_nPropDelay = nPropDelay;
        bDecideCalculatingOption();
        vChangeListBoxValues(-1);
    }
}

/**
 * \brief Decide Calculating Option
 *
 * Based on the present values SJW and PD, this function decides
 * the necessary parameter calculating operation.
 */
void CChangeRegisters_CAN_ICS_neoVI::bDecideCalculatingOption(void)
{
    if ((m_nSJWCurr == 0) && (m_nPropDelay == 0))
    {
        // All possible values of SJW and PD
        m_bOption = NO_DEF;
    }
    else if ((m_nSJWCurr == 0) && (m_nPropDelay != 0))
    {
        // All possible values of SJW and a specific value of PD
        m_bOption = SJW_TS1_TS2;
    }
    else if ((m_nSJWCurr != 0) && (m_nPropDelay == 0))
    {
        // All possible values of PD and a specific value of SJW
        m_bOption = PD_TS1_TS2;
    }
    else // if ((m_nSJWCurr != 0) && (m_nPropDelay != 0))
    {
        // Only specific values of SJW and PD
        m_bOption = TS1_TS2;
    }
}

/**
 * \brief Get Register Values
 *
 * This function retrieves the values of the registers CNF1,
 * CNF2 and CNF3 associated with the current selected entry
 * of the register entry list control.
 */
BOOL CChangeRegisters_CAN_ICS_neoVI::GetRegisterValues(BYTE& bCNF1, BYTE& bCNF2, BYTE& bCNF3)
{
    BOOL bResult = FALSE;
    int nIndexSel = 0;

    // First of all - get the current selection
    if ((nIndexSel = GetSelectedEntryIndex()) != -1)
    {
        sCOLUMNS* psCurrEntry =
            (sCOLUMNS*) (m_omListCtrlBitTime.GetItemData(nIndexSel));

        if (psCurrEntry != NULL)
        {
            bResult = TRUE;
            bCNF1 = psCurrEntry->uCNFReg1.ucCNF1;
            bCNF2 = psCurrEntry->uCNFReg2.ucCNF2;
            bCNF3 = psCurrEntry->uCNFReg3.ucCNF3;
        }
    }

    return bResult;
}

/**
 * \brief  Get Selected Entry Index
 * \return For a selected entry, its 0 based index; else -1
 *
 * This function retrieves the 0 based index of the selected
 * entry in the controller register entry list control.
 */
int CChangeRegisters_CAN_ICS_neoVI::GetSelectedEntryIndex(void)
{
    int nResult = -1;
    // First of all - get the current selection
    POSITION Pos = m_omListCtrlBitTime.GetFirstSelectedItemPosition();

    if (Pos != NULL)
    {
        nResult = m_omListCtrlBitTime.GetNextSelectedItem(Pos);
    }

    return nResult;
}

/**
 * \brief  Update Controller Data Members
 * \return TRUE if successful, else FALSE
 *
 * This function updates the controller data members with the
 * present selected combination value in the list control
 */
BOOL CChangeRegisters_CAN_ICS_neoVI::bUpdateControllerDataMembers(void)
{
    BYTE bCNF1, bCNF2, bCNF3;
    BOOL Result = GetRegisterValues(bCNF1, bCNF2, bCNF3);

    if (Result)
    {
        m_omStrEditCNF1.Format("%x", bCNF1);
        m_omStrEditCNF2.Format("%x", bCNF2);
        m_omStrEditCNF3.Format("%x", bCNF3);
    }

    return Result;
}

/**
 * \brief  Get Value From Combo Box
 * \return TRUE if successful, else FALSE
 *
 * This function returns value of the selected entry in a combo
 * box. Although helper in broader sense, this assumes the
 * entries to be 1 based integers and returns 0 when the entry
 * contains the string 'ALL'.
 */
int CChangeRegisters_CAN_ICS_neoVI::nGetValueFromComboBox(CComboBox& omComboBox)
{
    int nResult = 0;
    int nCurrSel =  omComboBox.GetCurSel();

    if (nCurrSel != CB_ERR)
    {
        CString omCurText = "";
        omComboBox.GetLBText(nCurrSel, omCurText);

        if (omCurText != _T("ALL"))
        {
            nResult = _tstoi(omCurText);
        }
    }

    return nResult;
}

INT CChangeRegisters_CAN_ICS_neoVI::nGetInitStatus()
{
    return m_nDataConfirmStatus;
}
