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
 * \file      ChangeRegisters_CAN_ETAS_BOA.cpp
 * \brief     This file contain definition of all function of 
 * \author    Amitesh Bharti
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * This file contain definition of all function of 
 */
// For all standard header file include
#include "CAN_ETAS_BOA_stdafx.h"
#include "ContrConfigPeakUsbDefs.h"
#include "CAN_ETAS_BOA_Resource.h"
#include "ChangeRegisters_CAN_ETAS_BOA.h"

/******************************************************************************/
/*  Function Name    :  CChangeRegisters_CAN_ETAS_BOA                                      */
/*  Input(s)         :                                                        */
/*  Output           :                                                        */
/*  Functionality    :  Constructor is called when user create an object of   */
/*                      this class. Initialisation of all data members        */
/*  Member of        :  CChangeRegisters_CAN_ETAS_BOA                                      */
/*  Friend of        :      -                                                 */
/*  Author(s)        :  Amitesh Bharti                                        */
/*  Date Created     :  15.02.2002                                            */
/*  Modifications    :  Raja N on 13.09.2004, Added init of member variables  */
/*  Modifications    :  Raja N on 14.03.2005, Added init of member variable   */
/*                      m_pControllerDetails                                  */
/******************************************************************************/
CChangeRegisters_CAN_ETAS_BOA::CChangeRegisters_CAN_ETAS_BOA(CWnd* pParent /*=NULL*/, PSCONTROLLER_DETAILS psControllerDetails, UINT nHardwareCount)
    : CDialog(CChangeRegisters_CAN_ETAS_BOA::IDD, pParent)
    , m_omStrSamplePoint(_T("70"))
    , m_omStrSJW(_T(""))
{
    //{{AFX_DATA_INIT(CChangeRegisters_CAN_ETAS_BOA)
    //m_omStrEditBTR0 = _T("");
    //m_omStrEditBTR1 = _T("");
    m_omStrEditCNF1 = _T("");
    m_omStrEditCNF2 = _T("");
    m_omStrEditCNF3 = _T("");
    m_omStrComboSampling = _T("");
    m_omStrEditBaudRate = _T("");
    m_omStrEditWarningLimit = _T("");
    //}}AFX_DATA_INIT
    m_unCombClock      = 32;
    m_bDialogCancel    = FALSE;
    memset(&m_sAccFilterInfo, 0, sizeof(m_sAccFilterInfo));
    m_ucWarningLimit    = defWARNING_LIMIT_MIN;
    m_ucControllerMode  = defCONTROLLER_MODE;
    m_usBTR0BTR1 = defDEFAUT_BAUDRATE;
    // Update controller data
    for (UINT i = 0; i < min(defNO_OF_CHANNELS, nHardwareCount); i++)
    {
        m_pControllerDetails[i] = psControllerDetails[i]; 
    }
    psMainContrDets = psControllerDetails;
    m_nPropDelay = 0;
    m_nSJWCurr = 0;
    m_nNoHardware = nHardwareCount;
    m_nDataConfirmStatus = WARNING_NOTCONFIRMED;
}

/******************************************************************************/
/*  Function Name    :  DoDataExchange                                        */
/*                                                                            */
/*  Input(s)         :                                                        */
/*  Output           :                                                        */
/*  Functionality    :  Called by the framework to exchange and validate      */
/*                         dialog data                                        */
/*                                                                            */
/*  Member of        :  CChangeRegisters_CAN_ETAS_BOA                                      */
/*  Friend of        :      -                                                 */
/*                                                                            */
/*  Author(s)        :  Amitesh Bharti                                        */
/*  Date Created     :  15.02.2002                                            */
/*  Modifications    :  Raja N on 14.03.2005                                  */
/*                      Added list variable to include channel information    */
/******************************************************************************/
void CChangeRegisters_CAN_ETAS_BOA::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CChangeRegisters_CAN_ETAS_BOA)
    DDX_Control(pDX, IDC_LIST_CHANNELS, m_omChannelList);
    DDX_Control(pDX, IDC_EDIT_WARNING_LIMIT, m_omEditWarningLimit);
    DDX_Control(pDX, IDC_COMB_SAMPLING, m_omCombSampling);
    DDX_Control(pDX, IDC_EDIT_BAUD_RATE, m_omEditBaudRate);
    DDX_CBString(pDX, IDC_COMB_SAMPLING, m_omStrComboSampling);
    DDV_MaxChars(pDX, m_omStrComboSampling, 1);
    DDX_Text(pDX, IDC_EDIT_BAUD_RATE, m_omStrEditBaudRate);
    DDX_Text(pDX, IDC_EDIT_WARNING_LIMIT, m_omStrEditWarningLimit);
    DDX_Control(pDX, IDC_COMB_SAMPOINT, m_omCtrlSamplePoint);
    DDX_CBString(pDX, IDC_COMB_SJW, m_omStrSJW);
    DDX_CBString(pDX,IDC_COMB_SAMPOINT, m_omStrSamplePoint);
    DDX_Control(pDX, IDC_COMB_SJW, m_omCtrlSJW);
    //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CChangeRegisters_CAN_ETAS_BOA, CDialog)
    //{{AFX_MSG_MAP(CChangeRegisters_CAN_ETAS_BOA)
    ON_EN_KILLFOCUS(IDC_EDIT_BAUD_RATE, OnKillfocusEditBaudRate)
    ON_CBN_SELCHANGE(IDC_COMB_SAMPLING, OnSelchangeCombSampling)
    ON_EN_SETFOCUS(IDC_EDIT_BAUD_RATE, OnSetfocusEditBaudRate)
    ON_BN_CLICKED(IDC_ButtonOK, OnClickedOK)
    ON_CBN_SETFOCUS(IDC_COMB_SAMPLING, OnSetfocusCombSampling)
    ON_WM_HELPINFO()
    ON_NOTIFY(NM_CLICK, IDC_LIST_CHANNELS, OnClickListChannels)
    ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_CHANNELS, OnItemchangedListChannels)
    ON_NOTIFY(NM_DBLCLK, IDC_LIST_CHANNELS, OnDblclkListChannels)
    ON_CBN_SELCHANGE(IDC_COMB_SJW, OnCbnSelchangeCombSjw)    
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/******************************************************************************/
/*  Function Name    :  OnInitDialog                                          */
/*                                                                            */
/*  Input(s)         :                                                        */
/*  Output           :                                                        */
/*  Functionality    :  This function is called by the framework in response  */
/*                      to the WM_INITDIALOG message. This message is sent to */
/*                      the dialog box during DoModal calls,which occur       */
/*                      immediately before the dialog box is displayed.       */
/*                      All controls of dialog are initialised in this func.  */
/*  Member of        :  CChangeRegisters_CAN_ETAS_BOA                                      */
/*  Friend of        :      -                                                 */
/*                                                                            */
/*  Author(s)        :  Amitesh Bharti                                        */
/*  Date Created     :  15.02.2002                                            */
/*  Modifications    :  25.02.2002, Amitesh Bharti                            */
/*                      Review comment incorporated                           */
/*                      13.11.2002, Gopi                                      */
/*                      Changed to usage of configuration file to read the    */
/*                      values.                                               */
/*                      18.12.2002, Edit box will not take negative value     */
/*                      28.03.2003, Changes for acceptance filter and warning */
/*                      limit.                                                */
/*                      23.10.2003, Moved updating fields on selection of     */
/*                      items in listctrl to OnItemchangedLstcBtrList function*/
/*  Modifications    :  31.08.2004, Raja N                                    */
/*                      Changes for USB devices specific code                 */
/*                      Added code to disable warning limit and to set default*/
/*                      value 96                                              */
/*  Modifications    :  14.03.2005, Raja N                                    */
/*                      Changes for Multi channel support to configure        */
/*                      multiple controllers                                  */
/*  Modifications    :  14.03.2005, Raja N                                    */
/*                      Implemented code review comments                      */
/*  Modifications    :  09.05.2008, Pradeep Kadoor                            */
/*                      Modification done for getting values of PropDelay and */
/*                      SJW from configuration file and display list          */ 
/*                      accordingly                                           */
/*  Modifications    :  09.05.2008, Pradeep Kadoor                            */
/*                      Modifications for setting two newly added combo box   */
/******************************************************************************/

BOOL CChangeRegisters_CAN_ETAS_BOA::OnInitDialog() 
{
    CDialog::OnInitDialog();
    CString omStrClock          = defCLOCK;
    CString omStrBaudRate       = _T("");
    CString omStrAcceptanceMask = _T("");
    CString omStrAcceptanceCode = _T("");
    CString omStrBrp            = _T("");
    CString omStrBtr0           = _T("");
    CString omStrBtr1           = _T("");

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
    m_omChannelList.InsertColumn( 0, _T(""));
    // Insert all channel information
    // Insert only for available channel information
    int nAvailableHardware = m_nNoHardware;//g_podHardwareInterface->nGetNoOfHardware();
    for (int nChannel = 0 ;
         nChannel < nAvailableHardware;
         nChannel++)
    {
        CString omStrChannel(_T(""));
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

    CComboBox* pComboBTLCYCL = (CComboBox*)GetDlgItem(IDC_COMB_BTL);
    if (pComboBTLCYCL != NULL)
    {
        pComboBTLCYCL->SetCurSel (0);
        pComboBTLCYCL->EnableWindow(FALSE);
    }
    CButton* pCheckSelfRec = (CButton*)GetDlgItem(IDC_CHECK_SELF_REC);
    if (pCheckSelfRec != NULL)
    {
        pCheckSelfRec->SetCheck(BST_CHECKED);
    }
    // Add an entry in each of the two combo boxes FindStringExact
    int nIndex = m_omCtrlSamplePoint.FindStringExact(-1, 
                           m_pControllerDetails->m_omStrSamplePercentage);
    m_omStrSamplePoint = m_pControllerDetails->m_omStrSamplePercentage;
    m_omStrSJW = m_pControllerDetails->m_omStrSjw;
    //UpdateData();
    if (CB_ERR != nIndex)
    {
        m_omCtrlSamplePoint.SetCurSel (nIndex);
    }
    else
    {
		//Set the default selection as 70% and update the controller structure
        m_omCtrlSamplePoint.SetCurSel (7);			
		strcpy_s(m_pControllerDetails->m_omStrSamplePercentage , "70");
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
    int nPropDelay = nGetValueFromComboBox(m_omCtrlSamplePoint);
    if (nPropDelay != m_nPropDelay)
    {
        m_nPropDelay = nPropDelay;
    }
    // List Values having SJW
    int nSJWCurr = nGetValueFromComboBox(m_omCtrlSJW);
    if (nSJWCurr != m_nSJWCurr)
    {
        m_nSJWCurr = nSJWCurr;
    }
    
    m_omEditWarningLimit.SetReadOnly(TRUE);
    //Initialise the index for number of items in list box before passing it is
    //function to calculate the same.
    
    // Set the Focus to the First Item
    m_omChannelList.SetItemState( 0,
                                  LVIS_SELECTED | LVIS_FOCUSED,
                                  LVIS_SELECTED | LVIS_FOCUSED);

    return TRUE;  // return TRUE unless you set the focus to a control
                  // EXCEPTION: OCX Property Pages should return FALSE
}
/******************************************************************************/
/*  Function Name    :  OnCancel                                              */    
/*                                                                            */    
/*  Input(s)         :                                                        */    
/*  Output           :                                                        */    
/*  Functionality    :  message handlers on CANCEL request                    */    
/*                                                                            */    
/*  Member of        :  CChangeRegisters_CAN_ETAS_BOA                                      */    
/*  Friend of        :      -                                                 */    
/*                                                                            */    
/*  Author(s)        :  Amitesh Bharti                                        */    
/*  Date Created     :  19.02.2002                                            */    
/*  Modifications    :                                                        */    
/*                                                                            */    
/******************************************************************************/
void CChangeRegisters_CAN_ETAS_BOA::OnCancel() 
{
    // Flag to be checked while validating the edit control input on kill focus
    m_bDialogCancel = TRUE;
    m_nDataConfirmStatus = INFO_RETAINED_CONFDATA;
    CDialog::OnCancel();
}
/******************************************************************************/
/*  Function Name    :  OnOK                                                  */    
/*                                                                            */    
/*  Input(s)         :                                                        */    
/*  Output           :                                                        */    
/*  Functionality    :  Message handlers on Enter Button ( Default OK button) */    
/*                      Every press of enter key, focus is to next control    */    
/*  Member of        :  CChangeRegisters_CAN_ETAS_BOA                                      */    
/*  Friend of        :      -                                                 */    
/*                                                                            */    
/*  Author(s)        :  Amitesh Bharti                                        */    
/*  Date Created     :  15.02.2002                                            */    
/*  Modifications    :                                                        */    
/*                                                                            */    
/******************************************************************************/
void CChangeRegisters_CAN_ETAS_BOA::OnOK() 
{

 // Dummy virtual function to avoid closing the dialog when ENTER key is 
//  pressed. Instead next conrol gets focus in tab order
    NextDlgCtrl();
}

/*****************************************************************************/
/*  Function Name    : omGetFormattedRegVal                                  */
/*                                                                           */
/*  Input(s)         :  Register value in UCHAR                              */
/*  Output           :  CString                                              */
/*  Functionality    :  Formats the input register value as 0xYY             */
/*  Member of        :  CChangeRegisters_CAN_ETAS_BOA                                     */
/*  Friend of        :      -                                                */
/*                                                                           */
/*  Author(s)        :  Ratnadip Choudhury                                   */
/*  Date Created     :  19.04.2008                                           */
/*****************************************************************************/
CString CChangeRegisters_CAN_ETAS_BOA::omGetFormattedRegVal(UCHAR ucRegVal)
{
    CString omStr = _T("");
    omStr.Format(TEXT("0x%X"), ucRegVal);
    // Insert one zero to format the sigle digit value to 0x05 etc.
    if (omStr.GetLength() == 3)
    {
        omStr.Insert(2, '0');
    }
    return omStr;
}
/******************************************************************************/
/*  Function Name    :  OnKillfocusEditBaudRate                               */
/*  Input(s)         :                                                        */
/*  Output           :                                                        */
/*  Functionality    :  Validate the buadrate on kill focus of this edit      */
/*                      control                                               */
/*  Member of        :  CChangeRegisters_CAN_ETAS_BOA                                      */
/*  Friend of        :      -                                                 */
/*  Author(s)        :  Amitesh Bharti                                        */
/*  Date Created     :  19.02.2002                                            */
/*  Modification By  :  Amitesh Bharti                                        */
/*  Modification on  :  22.03.2002, Validation for hexadecimal in CRadixEdit  */
/*  Modification By  :  Amitesh Bharti                                        */
/*  Modification on  :  27.05.2002, Validation for zero value entered         */
/*  Modification By  :  Amitesh Bharti                                        */
/*  Modification on  :  29.05.2002,Don't validate if CANCEL button is clicked */
/*  Modification By  :  Amitesh Bharti                                        */
/*  Modification on  :  12.12.2002, negative value of baudrate will not be    */
/*                      acceptect.Also the range is fixed to 1000kbps         */
/*  Modification By  :  Raja N                                                */
/*  Modification on  :  14.03.2005, Added code to update list control for the */
/*                      values updated in the baud rate edit control          */
/******************************************************************************/
void CChangeRegisters_CAN_ETAS_BOA::OnKillfocusEditBaudRate() 
{
    CString omStrBaudRate   =_T("");
    CString omStrValid      =_T("");
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
                    //vChangeListBoxValues(-1); 
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

/******************************************************************************/
/*  Function Name    :  OnSelchangeCombSampling                               */
/*                                                                            */
/*  Input(s)         :                                                        */
/*  Output           :                                                        */
/*  Functionality    :  Change the content of list control on change in       */
/*                      selection of number of sampling combo box.            */
/*  Member of        :  CChangeRegisters_CAN_ETAS_BOA                                      */
/*  Friend of        :      -                                                 */
/*                                                                            */
/*  Author(s)        :  Amitesh Bharti                                        */
/*  Date Created     :  19.02.2002                                            */
/*  Modifications    :                                                        */
/******************************************************************************/
void CChangeRegisters_CAN_ETAS_BOA::OnSelchangeCombSampling()
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
        //vChangeListBoxValues(-1);
        m_omStrComboSampling = omStrComboEditItem;
    }
}

/******************************************************************************/
/*  Function Name    :  OnSetfocusEditBaudRate                                */
/*                                                                            */
/*  Input(s)         :                                                        */
/*  Output           :                                                        */
/*  Functionality    :  Called when focus is set on baudrate edit box control */
/*                      Update all data members associated with Dialog        */
/*                      control.                                              */
/*  Member of        :  CChangeRegisters_CAN_ETAS_BOA                                      */
/*  Friend of        :      -                                                 */
/*                                                                            */
/*  Author(s)        :  Amitesh Bharti                                        */
/*  Date Created     :  15.02.2002                                            */
/*  Modifications    :                                                        */
/*                                                                            */
/******************************************************************************/
void CChangeRegisters_CAN_ETAS_BOA::OnSetfocusEditBaudRate() 
{
    // To update the data members before editing it and use it in kill focus
    UpdateData(TRUE);

}

/******************************************************************************/
/*  Function Name    :  vValidateBaudRate                                     */    
/*                                                                            */    
/*  Input(s)         :                                                        */    
/*  Output           :                                                        */    
/*  Functionality    :  This function will validate the user input value of   */    
/*                      baud rate. A valid baud rate will be calculated       */    
/*  Member of        :  CChangeRegisters_CAN_ETAS_BOA                                      */    
/*  Friend of        :      -                                                 */    
/*                                                                            */    
/*  Author(s)        :  Amitesh Bharti                                        */    
/*  Date Created     :  18.02.2002                                            */    
/*  Modifications    :  25.02.2002, Amitesh Bharti                            */    
/*                      Incorporated review comments                          */    
/*  Modification By  :  Amitesh Bharti                                        */
/*  Modification on  :  22.03.2002, If user changes clock freq. and select no */
/*                      for changing valid baudrate, change the clock freq. to*/
/*                      previous value.                                       */
/*  Modification By  :  Raja N                                                */
/*  Modification on  :  14.03.2005, Removed message box for asking the user to*/
/*                      change the baudrate to nearest possible value. Now it */
/*                      will automatically change the values                  */
/******************************************************************************/
void CChangeRegisters_CAN_ETAS_BOA::vValidateBaudRate()
{
    CString omStrBaudRate       = _T("");
    CString omStrPrvBaudRate    = _T("");
    CString omStrClockFreq      = _T("");
    DOUBLE  dBaudRate           = 0;
    //UINT    unClockFreq         = 0;
    //UINT    unClockPrevValue    = 0 ;
    UINT    unProductNbtNBrp    = 0;
    DOUBLE  dProductNbtNBrp     = 0; 
    CString omStrMessage        = _T("");
   
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
        int nFlag = defRESET;

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
/******************************************************************************/
/*  Function Name    :  OnClickedOK                                           */    
/*                                                                            */    
/*  Input(s)         :  User Selects OK Button                                */    
/*  Output           :  All user input field entry is written into            */
/*                      Registry/.ini file                                    */    
/*  Functionality    :  Message handlers on OK Button.To Remove control       */
/*                      to close when Enter Button is pressed                 */
/*                                                                            */    
/*  Member of        :  CChangeRegisters_CAN_ETAS_BOA                                      */    
/*  Friend of        :      -                                                 */    
/*                                                                            */    
/*  Author(s)        :  Amitesh Bharti                                        */    
/*  Date Created     :  18.02.2002                                            */    
/*  Modifications    :  25.02.2002, Amitesh Bharti                            */
/*                      Incorporated review comments                          */
/*                      13.11.2002, Gopi                                      */
/*                      Changed to usage of configuration file                */
/*                      14.12.2002, put the tool in same state after baudrate */
/*                      is initialise. i.e. connected or disconnected state   */
/*                      28.03.2003, Changes for acceptance filter and warning */
/*                      limit.                                                */
/*  Modifications    :  Raja N on 07.09.2004. Modified code to refer HI layer */
/*                      for Hardware related functions. Removed code to refer */
/*                      CChangeRegisters_CAN_ETAS_BOA static functions as they are moved in*/
/*                      to HI layer                                           */
/*  Modifications    :  Raja N on 09.03.2005                                  */
/*                   :  Added code to support multiple contoller information  */
/*                      in the configuration module                           */
/******************************************************************************/
void CChangeRegisters_CAN_ETAS_BOA::OnClickedOK() 
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
    // Save the changes permanently into the main data structure
    for (UINT i = 0; i < min(m_nNoHardware, defNO_OF_CHANNELS); i++)
    {
        psMainContrDets[i] = m_pControllerDetails[i];
    }
    // Close the dialog
    m_nDataConfirmStatus = INFO_INIT_DATA_CONFIRMED;
    CDialog::OnOK();
}

/******************************************************************************/
/*  Function Name    :  OnSetfocusCombSampling                                */
/*                                                                            */
/*  Input(s)         :                                                        */
/*  Output           :                                                        */
/*  Functionality    :  Called when focus is set on Number of sampling        */
/*                      combo box control. Updates all data members           */
/*                      associated with Dialog control.                       */
/*  Member of        :  CChangeRegisters_CAN_ETAS_BOA                                      */
/*  Friend of        :      -                                                 */
/*                                                                            */
/*  Author(s)        :  Amitesh Bharti                                        */
/*  Date Created     :  15.02.2002                                            */
/*  Modifications    :                                                        */
/*                                                                            */
/******************************************************************************/
void CChangeRegisters_CAN_ETAS_BOA::OnSetfocusCombSampling() 
{
    UpdateData(TRUE);
}

/******************************************************************************/
/*  Function Name    : bFillControllerConfig                                  */
/*                                                                            */
/*  Input(s)         : -                                                      */
/*  Output           :                                                        */
/*  Functionality    : This function is called to read registry  or ini file  */
/*                     and return the baudrate ( BTR0 and BTR1). If there is */
/*                     no entry return the default value                      */
/*  Member of        : CChangeRegisters_CAN_ETAS_BOA                                       */
/*  Friend of        :      -                                                 */
/*                                                                            */
/*  Author(s)        : Amitesh Bharti                                         */
/*  Date Created     : 26.03.2003                                             */
/*  Modifications    : Raja N on 13.09.2004                                   */
/*                     Modified the function as member function and changed   */
/*                     name as per coding standards                           */
/*  Modifications    : Raja N on 09.03.2005                                   */
/*                   : Added code to support multiple contoller information   */
/*                     in the configuration module                            */
/******************************************************************************/
BOOL CChangeRegisters_CAN_ETAS_BOA::bFillControllerConfig()
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

/******************************************************************************/
/*  Function Name    :  ~CChangeRegisters_CAN_ETAS_BOA                                     */
/*                                                                            */
/*  Input(s)         :                                                        */
/*  Output           :                                                        */
/*  Functionality    :  This is destructor of the class. It is called when    */
/*                      object of this class is being destroyed. All memory   */
/*                      allocation is deleted here.                           */
/*  Member of        :  CChangeRegisters_CAN_ETAS_BOA                                      */
/*  Friend of        :      -                                                 */
/*                                                                            */
/*  Author(s)        :  Amitesh Bharti                                        */
/*  Date Created     :  07.04.2003                                            */
/*  Modifications    :  Raja N on 14.03.2005, Added code to clear memory used */
/*                      to get controller information                         */
/******************************************************************************/
CChangeRegisters_CAN_ETAS_BOA::~CChangeRegisters_CAN_ETAS_BOA()
{

}


/*******************************************************************************
  Function Name  : OnClickListChannels
  Input(s)       : pNMHDR - Pointer to Notification Block
                   pResult - Pointer to the result
  Output         : -
  Functionality  : This function will be called when the user clicks the channel
                   list item. This will set the focus to the last selected item
                   if user clicks outside
  Member of      : CChangeRegisters_CAN_ETAS_BOA
  Author(s)      : Raja N
  Date Created   : 14.3.2005
  Modifications  : 
*******************************************************************************/
void CChangeRegisters_CAN_ETAS_BOA::OnClickListChannels(NMHDR* /*pNMHDR*/, LRESULT* pResult)
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

/*******************************************************************************
  Function Name  : OnItemchangedListChannels
  Input(s)       : pNMHDR - Pointer to the list item struct
                   pResult - Pointer to the result value
  Output         : -
  Functionality  : This function will update baudrate information of selected
                   channel
  Member of      : CChangeRegisters_CAN_ETAS_BOA
  Author(s)      : Raja N
  Date Created   : 14.03.2005
  Modifications  :
*******************************************************************************/
void CChangeRegisters_CAN_ETAS_BOA::OnItemchangedListChannels(NMHDR* pNMHDR, LRESULT* pResult) 
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

/*******************************************************************************
  Function Name  : OnDblclkListChannels
  Input(s)       : pNMHDR - Pointer to Notification Block
                   pResult - Pointer to the result
  Output         : -
  Functionality  : This function will be called wher the user double clicks the
                   channel list item. This will set the focus to the last
                   selected item if user clicks outside
  Member of      : CChangeRegisters_CAN_ETAS_BOA
  Author(s)      : Raja N
  Date Created   : 14.3.2005
  Modifications  : 
*******************************************************************************/
void CChangeRegisters_CAN_ETAS_BOA::OnDblclkListChannels(NMHDR* /*pNMHDR*/, LRESULT* pResult)
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

/*******************************************************************************
  Function Name  : vFillControllerConfigDetails
  Input(s)       : -
  Output         : -
  Functionality  : This function will fill details of selected channel in to the
                   member variables used. This will also update the BTR0 and
                   BTR1 registers value and list box of possible values for the
                   selected baudrate.
  Member of      : CChangeRegisters_CAN_ETAS_BOA
  Author(s)      : Raja N
  Date Created   : 14.3.2005
  Modifications  : 
*******************************************************************************/
void CChangeRegisters_CAN_ETAS_BOA::vFillControllerConfigDetails()
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
    m_omStrSamplePoint = m_pControllerDetails[ nIndex ].m_omStrSamplePercentage;
    m_omStrSJW = m_pControllerDetails[ nIndex ].m_omStrSjw;

    
    //omStrInitComboBox(ITEM_SAMPLING,1,m_omCombSampling));
    //Assign edit box string value to CString member variable of Edit control
    // for Baudrate Convert String into float or INT to be used to make a list
    // of all possible  of BTRi, SJW, Sampling Percentage, and NBT values
    //m_unCombClock       = (UINT)_tstoi(m_omStrComboClock);

    // TO BE FIXED LATER
    m_dEditBaudRate = (FLOAT)_tstof(m_omStrEditBaudRate);

    UpdateData(FALSE);
}

/*******************************************************************************
  Function Name  : vUpdateControllerDetails
  Input(s)       : -
  Output         : -
  Functionality  : This function will save the user enter values for baud rate
                   into the controller configuration structure
  Member of      : CChangeRegisters_CAN_ETAS_BOA
  Author(s)      : Raja N
  Date Created   : 14.3.2005
  Modifications  : 
*******************************************************************************/
void CChangeRegisters_CAN_ETAS_BOA::vUpdateControllerDetails()
{
    TCHAR*    pcStopStr              = NULL;
    CString omStrComboSampling      = _T("");
    CString omStrEditBtr0           = _T("");
    CString omStrEditBtr1           = _T("");
    CString omStrEditAcceptanceCode = _T("");
    CString omStrEditAcceptanceMask = _T("");
    
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
	    INT nItemUnderFocus = 0;
        m_ucWarningLimit = static_cast <UCHAR> (unWarningLimit);
		UCHAR ucBtr0 = 0;
		UCHAR ucBtr1 = 0;
        // Pack the BTR0 and BTR1 values in two bytes before calling DIL fuction 
        // to initialise.
        m_usBTR0BTR1 = static_cast <USHORT>(((ucBtr0 << 8)| ucBtr1) & 0xffff);
        
        // Update controller information
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
        strcpy_s(m_pControllerDetails[ m_nLastSelection ].m_omStrSamplePercentage, m_omStrSamplePoint.GetBuffer(MAX_PATH));
        strcpy_s(m_pControllerDetails[ m_nLastSelection ].m_omStrSjw, m_omStrSJW.GetBuffer(MAX_PATH));
    }
    else
    {
        // Invalid Warning Limit Error Message
        CString omStrMsg = _T("");
        omStrMsg.Format( defWARNINGLIMIT_MSG, m_omStrEditWarningLimit,
                         defWARNING_LIMIT_MIN,
                         defWARNING_LIMIT_MAX);
        m_omEditWarningLimit.SetFocus();
        m_omEditWarningLimit.SetSel(0, -1,FALSE);
    }

}

/*******************************************************************************
  Function Name  : vGetBaudRateFromCom
  Input(s)       : -
  Output         : -
  Functionality  : This function will be called from COM function to set baud rate
  Member of      : CChangeRegisters_CAN_ETAS_BOA
  Author(s)      : Anish
  Date Created   : 21.06.06
  Modifications  : 
*******************************************************************************/
BOOL CChangeRegisters_CAN_ETAS_BOA::bGetBaudRateFromCom(int nChannel,BYTE &bBTR0,BYTE &bBTR1)
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
/*******************************************************************************
 Function Name  : bSetFilterFromCom
 Input(s)       : long  nExtended,\\for extended msg or not 
                  DWORD  dBeginMsgId, \\filter's msg id start
                  DWORD dEndMsgId \\filter's msg id stop
 Output         : int - Operation Result. 0 incase of no errors. Failure Error
                  codes otherwise.
 Functionality  : This function will set the filter information if called using 
                  com interface.
 Member of      : CChangeRegisters_CAN_ETAS_BOA
 Author(s)      : Anish kr
 Date Created   : 05.06.06

*******************************************************************************/
BOOL CChangeRegisters_CAN_ETAS_BOA::bSetFilterFromCom(BOOL  bExtended, DWORD  dBeginMsgId, 
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

/*******************************************************************************
 Function Name  : bGetFilterFromCom
 Input(s)       : long  nExtended,\\for extended msg or not 
                  DWORD  dBeginMsgId, \\acceptance code
                  DWORD dEndMsgId \\mask code
 Output         : int - Operation Result. 0 incase of no errors. Failure Error
                  codes otherwise.
 Functionality  : This function will set the filter information if called using 
                  com interface.
 Member of      : CChangeRegisters_CAN_ETAS_BOA
 Author(s)      : Anish kr
 Date Created   : 05.06.06

*******************************************************************************/
BOOL CChangeRegisters_CAN_ETAS_BOA::bGetFilterFromCom(BOOL  &bExtended, double  &dBeginMsgId, 
                                   double &dEndMsgId)
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


/*******************************************************************************
 Function Name  : OnCbnSelchangeCombSjw
 Input(s)       : void
 Output         : void
 Functionality  : Handler when the user selects a specific value in the SJW list
 Member of      : CChangeRegisters_CAN_ETAS_BOA
 Author(s)      : Ratnadip Choudhury
 Date Created   : 19.04.2008

*******************************************************************************/
void CChangeRegisters_CAN_ETAS_BOA::OnCbnSelchangeCombSjw()
{
    int nSJWCurr = nGetValueFromComboBox(m_omCtrlSJW);
    if (nSJWCurr != m_nSJWCurr)
    {
        m_nSJWCurr = nSJWCurr;
    }
}

/*******************************************************************************
 Function Name  : OnCbnSelchangeCombPropdelay
 Input(s)       : void
 Output         : void
 Functionality  : Handler when the user selects a specific value in the PD list
 Member of      : CChangeRegisters_CAN_ETAS_BOA
 Author(s)      : Ratnadip Choudhury
 Date Created   : 19.04.2008

*******************************************************************************/
void CChangeRegisters_CAN_ETAS_BOA::OnCbnSelchangeCombPropdelay()
{
    int nPropDelay = nGetValueFromComboBox(m_omCtrlSamplePoint);
    if (nPropDelay != m_nPropDelay)
    {
        m_nPropDelay = nPropDelay;
    }
}


/*******************************************************************************
 Function Name  : bUpdateControllerDataMembers
 Input(s)       : void
 Output         : TRUE if successful, else FALSE
 Functionality  : This function updates the controller data members with the
                  present selected combination value in the list control
 Member of      : CChangeRegisters_CAN_ETAS_BOA
 Author(s)      : Ratnadip Choudhury
 Date Created   : 21.04.2008

*******************************************************************************/
BOOL CChangeRegisters_CAN_ETAS_BOA::bUpdateControllerDataMembers(void)
{

    BOOL Result = 1; // Calculate if required in future
    if (Result)
    {
		BYTE bCNF1 = 0x0, bCNF2 = 0x0, bCNF3 = 0x0;
        m_omStrEditCNF1.Format(_T("%x"), bCNF1);
        m_omStrEditCNF2.Format(_T("%x"), bCNF2);
        m_omStrEditCNF3.Format(_T("%x"), bCNF3);
    }
    return Result;
}

/*******************************************************************************
 Function Name  : nGetValueFromComboBox
 Input(s)       : void
 Output         : TRUE if successful, else FALSE
 Functionality  : This function returns value of the selected entry in a combo
                  box. Although helper in broader sense, this assumes the 
                  entries to be 1 based integers and returns 0 when the entry
                  contains the string 'ALL'.
 Member of      : CChangeRegisters_CAN_ETAS_BOA
 Author(s)      : Ratnadip Choudhury
 Date Created   : 21.04.2008

*******************************************************************************/
int CChangeRegisters_CAN_ETAS_BOA::nGetValueFromComboBox(CComboBox& omComboBox)
{
    int nResult = 0;
    int nCurrSel =  omComboBox.GetCurSel();
    if (nCurrSel != CB_ERR)
    {
        CString omCurText = _T("");
        omComboBox.GetLBText(nCurrSel, omCurText);
        if (omCurText != _T("ALL"))
        {
            nResult = _tstoi(omCurText);
        }
    }
    return nResult;
}

INT CChangeRegisters_CAN_ETAS_BOA::nGetInitStatus()
{
    return m_nDataConfirmStatus;
}