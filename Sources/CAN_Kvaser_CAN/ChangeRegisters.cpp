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
 * \file      ChangeRegisters.cpp
 * \brief     This file contain definition of all function of 
 * \authors   Amitesh Bharti, Pradeep Kadoor
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * This file contain definition of all function of 
 */
// For all standard header file include
#include "CAN_Kvaser_CAN_stdafx.h"
// Definition of App class
// Hash defines
#include "math.h"
#include "include/struct_can.h"
#include "ContrConfigPeakUsbDefs.h"
#include "Utility/RadixEdit.h"
#include "ChangeRegDefines.h"
// CChangeRegisters class defination file.
#include "ChangeRegisters.h"
// Definition of CAcceptanceFilterDlg class
#include "AcceptanceFilterDlg.h"
// For HI Layer definition

long lFromStrTCHAR_2_Long(TCHAR acStr[], TCHAR** pccEndPtr, int nBase)
{
    return _tcstol(acStr, pccEndPtr, nBase);
}

long lFromCString_2_Long(CString omStr, TCHAR** pccEndPtr, int nBase)
{
    return _tcstol(omStr.GetBuffer(MAX_PATH), pccEndPtr, nBase);
}


// For Application object
//extern CMcNetApp theApp;
// For Hardware Interface Functions
//extern CHardwareInterface* g_podHardwareInterface;
/******************************************************************************/
/*  Function Name    :  CChangeRegisters                                      */
/*  Input(s)         :                                                        */
/*  Output           :                                                        */
/*  Functionality    :  Constructor is called when user create an object of   */
/*                      this class. Initialisation of all data members        */
/*  Member of        :  CChangeRegisters                                      */
/*  Friend of        :      -                                                 */
/*  Author(s)        :  Amitesh Bharti                                        */
/*  Date Created     :  15.02.2002                                            */
/*  Modifications    :  Raja N on 13.09.2004, Added init of member variables  */
/*  Modifications    :  Raja N on 14.03.2005, Added init of member variable   */
/*                      m_pControllerDetails                                  */
/******************************************************************************/
//IMPLEMENT_DYNAMIC(CChangeRegisters, CPropertyPage)
CChangeRegisters::CChangeRegisters(CWnd* pParent /*=NULL*/, PSCONTROLER_DETAILS psControllerDetails, UINT nCount)
    : CDialog(CChangeRegisters::IDD, pParent)
    //: CPropertyPage(CChangeRegisters::IDD, IDS_PPAGE_CHANGE_REGISTER)
{    
    //{{AFX_DATA_INIT(CChangeRegisters)
    m_byEditBRP = 1;
    m_omStrEditBTR0 = _T("");
    m_omStrEditBTR1 = _T("");
    m_omStrComboSampling = _T("");
    m_omStrEditBaudRate = _T("");
    m_omStrComboClock = defCLOCK;
    m_omStrEditWarningLimit = _T("");
    //}}AFX_DATA_INIT
    m_unCombClock      = 0;
    m_bDialogCancel    = FALSE;    
    m_ucWarningLimit    = defWARNING_LIMIT_MIN;
    m_ucControllerMode  = defCONTROLLER_MODE;
    m_usBTR0BTR1 = defDEFAUT_BAUDRATE;
    // Update controller data
    m_pControllerDetails = psControllerDetails;
    bFillControllerConfig();
    m_usBTR0BTR1 = defDEFAUT_BAUDRATE;
    m_unHardwareCount = nCount;
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
/*  Member of        :  CChangeRegisters                                      */
/*  Friend of        :      -                                                 */
/*                                                                            */
/*  Author(s)        :  Amitesh Bharti                                        */
/*  Date Created     :  15.02.2002                                            */
/*  Modifications    :  Raja N on 14.03.2005                                  */
/*                      Added list variable to include channel information    */
/******************************************************************************/
void CChangeRegisters::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CChangeRegisters)
    DDX_Control(pDX, IDC_LIST_CHANNELS, m_omChannelList);
    DDX_Control(pDX, IDC_EDIT_WARNING_LIMIT, m_omEditWarningLimit);
    DDX_Control(pDX, IDC_COMB_SAMPLING, m_omCombSampling);
    DDX_Control(pDX, IDC_COMB_CLOCK, m_omCombClock);
    DDX_Control(pDX, IDC_EDIT_BRP, m_omEditBRP);
    DDX_Control(pDX, IDC_LSTC_BTR_LIST, m_omListCtrlBitTime);
    DDX_Control(pDX, IDC_EDIT_BAUD_RATE, m_omEditBaudRate);
    DDX_Control(pDX, IDC_EDIT_BTR1, m_omEditBTR1);
    DDX_Control(pDX, IDC_EDIT_BTR0, m_omEditBTR0);
    DDX_Text(pDX, IDC_EDIT_BRP, m_byEditBRP);
    DDV_MinMaxByte(pDX, m_byEditBRP, 1, 64);
    DDX_Text(pDX, IDC_EDIT_BTR0, m_omStrEditBTR0);
    DDV_MaxChars(pDX, m_omStrEditBTR0, 2);
    DDX_Text(pDX, IDC_EDIT_BTR1, m_omStrEditBTR1);
    DDV_MaxChars(pDX, m_omStrEditBTR1, 2);
    DDX_CBString(pDX, IDC_COMB_SAMPLING, m_omStrComboSampling);
    DDV_MaxChars(pDX, m_omStrComboSampling, 1);
    DDX_Text(pDX, IDC_EDIT_BAUD_RATE, m_omStrEditBaudRate);
    DDX_CBString(pDX, IDC_COMB_CLOCK, m_omStrComboClock);
    DDX_Text(pDX, IDC_EDIT_WARNING_LIMIT, m_omStrEditWarningLimit);
    //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CChangeRegisters, CDialog)
    //{{AFX_MSG_MAP(CChangeRegisters)
    ON_EN_KILLFOCUS(IDC_EDIT_BAUD_RATE, OnKillfocusEditBaudRate)
    ON_EN_KILLFOCUS(IDC_EDIT_BTR0, OnKillfocusEditBTR0)
    ON_EN_KILLFOCUS(IDC_EDIT_BTR1, OnKillfocusEditBTR1)
    ON_CBN_SELCHANGE(IDC_COMB_SAMPLING, OnSelchangeCombSampling)
    ON_EN_SETFOCUS(IDC_EDIT_BAUD_RATE, OnSetfocusEditBaudRate)
    ON_EN_SETFOCUS(IDC_EDIT_BTR0, OnSetfocusEditBTR0)
    ON_EN_SETFOCUS(IDC_EDIT_BTR1, OnSetfocusEditBTR1)
    ON_BN_CLICKED(IDC_ButtonOK, OnClickedOK)
    ON_CBN_SELCHANGE(IDC_COMB_CLOCK, OnSelchangeCombClock)
    ON_CBN_SETFOCUS(IDC_COMB_CLOCK, OnSetfocusCombClock)
    ON_CBN_SETFOCUS(IDC_COMB_SAMPLING, OnSetfocusCombSampling)
    ON_WM_HELPINFO()
    ON_BN_CLICKED(IDC_CBTN_ACCEPTANCE, OnCbtnAcceptance)
    ON_NOTIFY(LVN_ITEMCHANGED, IDC_LSTC_BTR_LIST, OnItemchangedLstcBtrList)
    ON_NOTIFY(NM_CLICK, IDC_LIST_CHANNELS, OnClickListChannels)
    ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_CHANNELS, OnItemchangedListChannels)
    ON_NOTIFY(NM_DBLCLK, IDC_LIST_CHANNELS, OnDblclkListChannels)
    ON_BN_CLICKED(IDC_CBTN_BLINK, OnCbtnBlink)
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
/*  Member of        :  CChangeRegisters                                      */
/*  Friend of        :      -                                                 */
/*                                                                            */
/*  Author(s)        :  Amitesh Bharti                                        */
/*  Date Created     :  15.02.2002                                            */
/******************************************************************************/

BOOL CChangeRegisters::OnInitDialog() 
{
    CDialog::OnInitDialog();
    TCHAR caColumnName[5][50] = { defSTR_BTR0_COL_HEADING,
                                 defSTR_BTR1_COL_HEADING,
                                 defSTR_SAMPLE_POINT_COL_HEADING,
                                 defSTR_NBT_COL_HEADING,
                                 defSTR_SJW_COL_HEADING };
    CString omStrClock          = _T("");
    CString omStrBaudRate       = _T("");
    CString omStrAcceptanceMask = _T("");
    CString omStrAcceptanceCode = _T("");
    CString omStrBrp            = _T("");
    CString omStrBtr0           = _T("");
    CString omStrBtr1           = _T("");

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
                                 defCHANNEL_LIST_GROW_SIZE );
    // Load Channel Icon
    CWinApp* pWinApp = (CWinApp*)this;
    m_omChannelImageList.Add(pWinApp->LoadIcon(IDR_PGM_EDTYPE));
    // Assign the image list to the control
    m_omChannelList.SetImageList(&m_omChannelImageList, LVSIL_NORMAL );
    // Insert empty column
    m_omChannelList.InsertColumn( 0, _T("") );
    // Insert all channel information
    // Insert only for available channel information
    int nAvailableHardware = m_unHardwareCount;//g_podHardwareInterface->nGetNoOfHardware();
    for( int nChannel = 0 ;
         nChannel < nAvailableHardware;
         nChannel++ )
    {
        CString omStrChannel(_T(""));
        // Create Channel String
        omStrChannel.Format( defSTR_CHANNEL_NAME_FORMAT,
                        defSTR_CHANNEL_NAME,
                        nChannel + 1 );
        // Insert channel item
        m_omChannelList.InsertItem( nChannel, omStrChannel );
    }

    // Set the selected item index to zero
    m_nLastSelection = 0;

    m_omEditBTR0.vSetBase( BASE_HEXADECIMAL );
    m_omEditBTR0.vSetSigned(FALSE);

    m_omEditBTR1.vSetBase( BASE_HEXADECIMAL );
    m_omEditBTR1.vSetSigned(FALSE);

    m_omEditBaudRate.vSetBase( BASE_DECIMAL );
    m_omEditBaudRate.vSetSigned(FALSE);
    m_omEditBaudRate.vAcceptFloatingNum( TRUE );
   
    m_omEditWarningLimit.vSetBase( BASE_DECIMAL );
    m_omEditWarningLimit.vSetSigned(FALSE);
    m_omEditWarningLimit.vAcceptFloatingNum( FALSE );

    m_omEditBRP.vSetBase( BASE_DECIMAL );
    
    m_omEditBTR0.LimitText(defVALID_LENGTH_BTR);

    m_omEditBTR1.LimitText(defVALID_LENGTH_BTR);

    //Calculate the total size of all column header   
    m_omListCtrlBitTime.GetWindowRect( &rListCtrlRect);
    nTotalColunmSize     = rListCtrlRect.right - rListCtrlRect.left;
    nTotalStrLengthPixel = 0;

    for(INT j=0; j<defNUMBER_OF_COLUMNS;j++)
    {
         nTotalStrLengthPixel +=
             m_omListCtrlBitTime.GetStringWidth(caColumnName[j]);
    }
    //Insert each column name after calculating the size for the same.
    for(INT i=0; i<defNUMBER_OF_COLUMNS;i++)
    {
         nColumnSize  = m_omListCtrlBitTime.GetStringWidth(caColumnName[i]) ;
         nColumnSize +=
             (nTotalColunmSize-nTotalStrLengthPixel)/defNUMBER_OF_COLUMNS;
         m_omListCtrlBitTime.InsertColumn(i,caColumnName[i],
                                        LVCFMT_CENTER, nColumnSize);
    }

    //Set extended style to show selection for all subitems   
    m_omListCtrlBitTime.SetExtendedStyle(LVS_EX_FULLROWSELECT);

    
    
    //Initialise the index for number of items in list box before passing it is
    //function to calculate the same.
    
    // Set the Focus to the First Item
    m_omChannelList.SetItemState( 0,
                                  LVIS_SELECTED | LVIS_FOCUSED,
                                  LVIS_SELECTED | LVIS_FOCUSED );

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
/*  Member of        :  CChangeRegisters                                      */    
/*  Friend of        :      -                                                 */    
/*                                                                            */    
/*  Author(s)        :  Amitesh Bharti                                        */    
/*  Date Created     :  19.02.2002                                            */    
/*  Modifications    :                                                        */    
/*                                                                            */    
/******************************************************************************/
void CChangeRegisters::OnCancel() 
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
/*  Member of        :  CChangeRegisters                                      */    
/*  Friend of        :      -                                                 */    
/*                                                                            */    
/*  Author(s)        :  Amitesh Bharti                                        */    
/*  Date Created     :  15.02.2002                                            */    
/*  Modifications    :                                                        */    
/*                                                                            */    
/******************************************************************************/
void CChangeRegisters::OnOK() 
{

 // Dummy virtual function to avoid closing the dialog when ENTER key is 
//  pressed. Instead next conrol gets focus in tab order
    NextDlgCtrl( );
}
/******************************************************************************/
/*  Function Name    : vDisplayListBox                                        */
/*                                                                            */
/*  Input(s)         :  Total number of item and item last on fucus           */
/*  Output           :                                                        */
/*  Functionality    :  Insert columns of each row in the list control        */
/*                      Calls function vSelSetFocusItemList to set focus      */
/*  Member of        :  CChangeRegisters                                      */
/*  Friend of        :      -                                                 */
/*                                                                            */
/*  Author(s)        :  Amitesh Bharti                                        */
/*  Date Created     :  19.02.2002                                            */
/******************************************************************************/
void CChangeRegisters:: vDisplayListBox(INT nIndex,INT nItemFocus )
{
//    BOOL bResult = ::IsWindowVisible(m_hWnd);

//    bResult = ::IsWindowVisible(m_omListCtrlBitTime.m_hWnd);

//    if (bResult == FALSE) return;

    CString cStrText    = _T("");
    INT nItem           = -1;
    CString omStrBTR0   = _T("");
    CString omStrBTR1   = _T("");

    // Clear list box first
    m_omListCtrlBitTime.DeleteAllItems();
    // Get the total number of Columns in the List View Header
    INT nColumnCount = m_omListCtrlBitTime.GetHeaderCtrl()->GetItemCount();

    m_omEditBTR0.GetWindowText(omStrBTR0);
    m_omEditBTR1.GetWindowText(omStrBTR1);
    // Insert items and subitems after Formating the strings in the list view 
    // control.
    for (INT i=0;i <nIndex;i++)
    {
      cStrText.Format(TEXT("0x%X"),m_asColListCtrl[i].uBTRReg0.ucBTR0);
       // Insert one zero to format the sigle digit value to 0x05 etc.
       if(cStrText.GetLength()==3)
       {
           cStrText.Insert(2,'0');
       }
       m_omListCtrlBitTime.InsertItem(LVIF_TEXT|LVIF_STATE, i, cStrText,
                        (i==0) ? LVIS_SELECTED : 0,LVIS_SELECTED, 0, 0);
       
       // Format and Initialize the text of the subitems.
       for (INT j=1;j < nColumnCount;j++)
       {
          // Format the text of the subitems.
          if(1==j )
          {
           // To set the focus point if baudrate is changed due to change in
           //  BTR value.
           if(cStrText.Find(omStrBTR0) !=-1 && 
              nItemFocus == defFromKillFocusBTR 
              && nItem ==-1)
            {
             nItem = i;
            }

           cStrText.Format(TEXT("0x%X"),m_asColListCtrl[i].uBTRReg1.ucBTR1);
          // Insert one zero to format the sigle digit value to 0x05 etc.
           if(cStrText.GetLength()==3)
           {
            cStrText.Insert(2,'0');
           }
          // To set the focus point if baudrate is changed due to change in BTR 
          // value
           if(cStrText.Find(omStrBTR1) ==-1 && nItemFocus== defFromKillFocusBTR 
               && nItem ==i)
           {
            nItem = -1;
           }
          }
          else if(2==j)
          {
           cStrText.Format(TEXT("%d%%"),
                           m_asColListCtrl[i].sBRPNBTSampNSJW.usSampling);
          }
          else if(3==j)
          {
           cStrText.Format(TEXT("%d"),m_asColListCtrl[i].sBRPNBTSampNSJW.usNBT);
          }
          else if(4==j)
          {
           cStrText.Format(TEXT("%d"),m_asColListCtrl[i].sBRPNBTSampNSJW.usSJW);
           //Set the defualt focus point on row having SJW = 2, NBT = 8
           if(m_asColListCtrl[i].sBRPNBTSampNSJW.usSJW==defMAX_SJW/2&&nItem==-1
                    && m_asColListCtrl[i].sBRPNBTSampNSJW.usNBT==defMIN_NBT )
            {
                nItem = i;
            }
          }
          // Initialize the text of the subitems.
          INT nTemp = m_omListCtrlBitTime.SetItemText(i,j, cStrText);
          if(nTemp == 0)
          {
			  //if(theApp.m_bFromAutomation == FALSE)
              AfxMessageBox(defERRORMSG_INSERT);
          }

       }
    }
    // If nItemfucus is last saved value assign it to nItem
    if(nItemFocus>=0)
    {
      nItem = nItemFocus;
    }
    //  Set the focus on item
    vSelSetFocusItemList(nIndex,nItem);
}
/******************************************************************************/
/*  Function Name    :  OnKillfocusEditBaudRate                               */
/*  Input(s)         :                                                        */
/*  Output           :                                                        */
/*  Functionality    :  Validate the buadrate on kill focus of this edit      */
/*                      control                                               */
/*  Member of        :  CChangeRegisters                                      */
/*  Friend of        :      -                                                 */
/*  Author(s)        :  Amitesh Bharti                                        */
/*  Date Created     :  19.02.2002                                            */
/******************************************************************************/
void CChangeRegisters::OnKillfocusEditBaudRate() 
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
    if(m_bDialogCancel != TRUE  )
    {
        // Don't validate if CANCEL button is clicked.
        if(unButtonState ==0 )
        {
            // Validate for empty string and if zero value is entered.
            DOUBLE dBaudRate = (FLOAT) _tstof(omStrBaudRate);
            if(nLength == 0 || dBaudRate <= 0 || dBaudRate > 1000.0 )
            {   
                m_omEditBaudRate.SetWindowText(m_omStrEditBaudRate);
				//if(theApp.m_bFromAutomation == FALSE)
                AfxMessageBox(defVALIDATION_MSG_BAUD_RATE);
                m_omEditBaudRate.SetFocus();
                m_omEditBaudRate.SetSel(0, -1,FALSE);
            }
            else
            {
                
                m_dEditBaudRate     = (FLOAT) _tstof(m_omStrEditBaudRate);

                // Call if string is valid to validate the baud rate value and 
                // suggest  a next valid baud rate
                //Validate only if previous value in edit control is not the
                //  same as the one changed by user
                if(m_dEditBaudRate != dBaudRate && dBaudRate>0 
                    && m_dEditBaudRate > 0  )
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
/******************************************************************************/
/*  Function Name    :  OnKillfocusEditBTR0                                   */
/*                                                                            */
/*  Input(s)         :                                                        */
/*  Output           :                                                        */
/*  Functionality    :  Validate the BTR0 on kill focus of this edit          */
/*                      control                                               */
/*  Member of        :  CChangeRegisters                                      */
/*  Friend of        :      -                                                 */
/*                                                                            */
/*  Author(s)        :  Amitesh Bharti                                        */
/*  Date Created     :  19.02.2002                                            */
/******************************************************************************/
void CChangeRegisters::OnKillfocusEditBTR0() 
{
    CString omStrBtr0Get    =_T("");
    CString omStrValidBtr0  =_T("");
    INT     nLength         = 0;

    m_omEditBTR0.GetWindowText(omStrBtr0Get);
    nLength             = omStrBtr0Get.GetLength();
    CButton* pomButtonCancel = (CButton*) GetDlgItem(IDCANCEL);
    CButton* pomFocusWnd     = (CButton*)GetFocus();
    // Validate only if next focus is not on Cancel Button
    if(m_bDialogCancel != TRUE )
    {
        if(pomButtonCancel !=pomFocusWnd)
        {
            // Validate for empty string
            if(nLength == 0)
            {
              m_omEditBTR0.SetWindowText(m_omStrEditBTR0);
			  //if(theApp.m_bFromAutomation == FALSE)
              AfxMessageBox(defVALIDATION_MESSAGE);
              m_omEditBTR0.SetFocus();
              m_omEditBTR0.SetSel(0, -1,FALSE);
            }
            else
            {
              if(m_omStrEditBTR0!=omStrBtr0Get)
              { 
                vCalculateBaudRateNBTR1(omStrBtr0Get);
              }

            }
        }
    }
    else
    {
     m_omEditBTR0.SetWindowText(m_omStrEditBTR0);
    }
    // Call OnCancel() function if next focus is on CANCEL button  \
    //  close the dialog if CANCEL button is clicked
    if (pomButtonCancel == pomFocusWnd)
    {
        OnCancel();
    }

}
/******************************************************************************/
/*  Function Name    :  OnKillfocusEditBTR1                                   */
/*                                                                            */
/*  Input(s)         :                                                        */
/*  Output           :                                                        */
/*  Functionality    :  Validate the BTR1 on kill focus of this edit          */
/*                      control                                               */
/*  Member of        :  CChangeRegisters                                      */
/*  Friend of        :      -                                                 */
/*                                                                            */
/*  Author(s)        :  Amitesh Bharti                                        */
/*  Date Created     :  19.02.2002                                            */
/******************************************************************************/
void CChangeRegisters::OnKillfocusEditBTR1() 
{
    CString omStrBtr1Get    =_T("");
    CString omStrValidBtr1  =_T("");
    INT     nLength         = 0;

    m_omEditBTR1.GetWindowText(omStrBtr1Get);
    
    nLength = omStrBtr1Get.GetLength();
    CButton* pomButtonCancel = (CButton*) GetDlgItem(IDCANCEL);
    CButton* pomFocusWnd     = (CButton*)GetFocus();
    // Validate only if next focus is not on Cancel Button
    if(m_bDialogCancel != TRUE)
    {
        if(pomButtonCancel !=pomFocusWnd)
        {
            // Validate for empty string
            if(nLength ==0 )
            {
                m_omEditBTR1.SetWindowText(m_omStrEditBTR1);
//				if(theApp.m_bFromAutomation == FALSE)
                AfxMessageBox(defVALIDATION_MESSAGE);
                m_omEditBTR1.SetFocus();
                m_omEditBTR1.SetSel(0, -1,FALSE);
            }
            else
            {
             if(m_omStrEditBTR1!=omStrBtr1Get)
             {
                 vCalculateBaudRateNBTR0(omStrBtr1Get);
             }
            }
        }
    }
    else
    {
      m_omEditBTR1.SetWindowText(m_omStrEditBTR1);
    }
    // Call OnCancel() function if next focus is on CANCEL button  \
    //  close the dialog if CANCEL button is clicked
    if (pomButtonCancel == pomFocusWnd)
    {
        OnCancel();
    }

}
/******************************************************************************/
/*  Function Name    :  OnSelchangeCombSampling                               */
/*                                                                            */
/*  Input(s)         :                                                        */
/*  Output           :                                                        */
/*  Functionality    :  Change the content of list control on change in       */
/*                      selection of number of sampling combo box.            */
/*  Member of        :  CChangeRegisters                                      */
/*  Friend of        :      -                                                 */
/*                                                                            */
/*  Author(s)        :  Amitesh Bharti                                        */
/*  Date Created     :  19.02.2002                                            */
/******************************************************************************/
void CChangeRegisters::OnSelchangeCombSampling() 
{ 
    INT nGetValue               = 0;
    CString omStrComboEditItem  =_T("");

    nGetValue =  m_omCombSampling.GetCurSel();
    if (nGetValue !=CB_ERR)
    {
      m_omCombSampling.GetLBText(nGetValue,omStrComboEditItem);
    }
    if(m_omStrComboSampling!=omStrComboEditItem)
    {
     vChangeListBoxValues(-1);
     m_omStrComboSampling = omStrComboEditItem;
    }

}
/******************************************************************************/
/*  Function Name    :  vChangeListBoxValues                                  */
/*                                                                            */
/*  Input(s)         :  Flag to indicate from where this function is called   */
/*  Output           :                                                        */
/*  Functionality    :  Change the content of list control on change in       */
/*                      selection of number of sampling or clock, or BTR0     */
/*                      or BTR1 or baudrate combo box.                        */
/*  Member of        :  CChangeRegisters                                      */
/*  Friend of        :      -                                                 */
/*                                                                            */
/*  Author(s)        :  Amitesh Bharti                                        */
/*  Date Created     :  19.02.2002                                            */
/******************************************************************************/
void CChangeRegisters::vChangeListBoxValues(INT nflag) 
{
    UINT unIndex                = 0;  
    CString omStrComboEditItem  =_T("");
    INT nGetValue               = 0;
    INT nSample                 = 0;
    INT nReturn                 = 0;
    UINT unClock                = 0;

    //Get the value selected by user for number of sampling/bit
    nGetValue =  m_omCombSampling.GetCurSel();
    if (nGetValue !=CB_ERR)
    {
      m_omCombSampling.GetLBText(nGetValue,omStrComboEditItem);
    }
    nSample = _tstoi(omStrComboEditItem.GetBuffer(MAX_PATH));
    unIndex = 0;
    //Get the value selected by user for clock.
    nGetValue =  m_omCombClock.GetCurSel();
    if (nGetValue !=CB_ERR)
    {
      m_omCombClock.GetLBText(nGetValue,omStrComboEditItem);
    }
	unClock       = _tstoi(omStrComboEditItem.GetBuffer(MAX_PATH));
    // Call function to calculate the list of BTR0, BTR1, SJW,NBT and Sampling.
    nReturn = nListBoxValues( m_asColListCtrl,m_dEditBaudRate,(WORD)unClock,&unIndex,
                                nSample) ;
    if(nReturn != -1)
    {
         // Remove all the items in the list box.
         m_omListCtrlBitTime.DeleteAllItems();
         // Display all the new items in the list box
         vDisplayListBox(unIndex,nflag);
         // Update the clock frequency value in data member
         m_unCombClock = unClock;
         // Get the number of columns.
         INT nColumnCount = m_omListCtrlBitTime.GetHeaderCtrl()->GetItemCount();
         // Get the row number selected.
         INT nItem        = m_omListCtrlBitTime.GetNextItem(-1, LVNI_SELECTED) ;     
         if(nColumnCount !=defERROR)
         {
             vUpdateBTRsBRPEditWindow(nColumnCount,nItem);
         }
    }
    else
    {
        vValidateBaudRate();
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
/*  Member of        :  CChangeRegisters                                      */
/*  Friend of        :      -                                                 */
/*                                                                            */
/*  Author(s)        :  Amitesh Bharti                                        */
/*  Date Created     :  15.02.2002                                            */
/******************************************************************************/
void CChangeRegisters::OnSetfocusEditBaudRate() 
{
    // To update the data members before editing it and use it in kill focus
    UpdateData(TRUE);

}
/******************************************************************************/
/*  Function Name    :  vUpdateBtriBrpEditWindow                              */
/*                                                                            */
/*  Input(s)         :                                                        */
/*  Output           :                                                        */
/*  Functionality    :  This function will update the BTR0,BTR1 and BRP value */
/*                      corresponding values in the list control.             */
/*                      control.                                              */
/*  Member of        :  CChangeRegisters                                      */
/*  Friend of        :      -                                                 */
/*                                                                            */
/*  Author(s)        :  Amitesh Bharti                                        */
/*  Date Created     :  15.02.2002                                            */
/******************************************************************************/
void CChangeRegisters:: vUpdateBTRsBRPEditWindow(INT /*nColumnCount*/, INT nItem)
{
    CString omStrItems[defNUMBER_OF_COLUMNS];
    CString omStrBaudRate   =_T("");
    CString omStrClockFreq  =_T("");
    CString omStrBrp        =_T("");

    DOUBLE  dBaudRate       = 0;
    UINT    unBrp           = 0;
    UINT    unClockFreq     = 0;
    UINT    unNbt           = 0;
    uBTR0   uBtr0;
    // Get the values of subitems having item index as nItem.  
    for(INT j=0 ; j<defNUMBER_OF_COLUMNS ; j++)
    {
     omStrItems[j] = m_omListCtrlBitTime.GetItemText(nItem,j);
    }

    omStrItems[defBRT0_COLUNM_POS-1].Replace(defHEX_STRING,defEMPTY_STRING);
    m_omEditBTR0.SetWindowText(omStrItems[defBRT0_COLUNM_POS-1]);   

    omStrItems[defBRT1_COLUNM_POS-1].Replace(defHEX_STRING,defEMPTY_STRING);
    m_omEditBTR1.SetWindowText(omStrItems[defBRT1_COLUNM_POS-1]);

    m_omEditBaudRate.GetWindowText(omStrBaudRate);
    m_omCombClock.GetWindowText(omStrClockFreq);

    dBaudRate  = (FLOAT) _tstof(omStrBaudRate);
    unClockFreq= _tstoi(omStrClockFreq.GetBuffer(MAX_PATH));

    unNbt      = _tstoi(omStrItems[defNBT_COLUNM_POS-1].GetBuffer(MAX_PATH));
    if(unNbt>0)
    {
        TCHAR* pcStopStr = NULL;
        uBtr0.ucBTR0 = (UCHAR) lFromCString_2_Long(
                 omStrItems[defBRT0_COLUNM_POS-1], &pcStopStr, defHEXADECIMAL);
        unBrp = uBtr0.sBTR0Bit.ucBRPbit + 1;
    }
    //Convert UINT to string.
    TCHAR acBrpStr[32] = {L'\0'};
    _itot(unBrp, acBrpStr, 10);
    omStrBrp = acBrpStr;
    m_omEditBRP.SetWindowText(omStrBrp);   

}
/******************************************************************************/
/*  Function Name    :  vCalculateBaudRateNBTR1                               */
/*                                                                            */
/*  Input(s)         :  BTR0 value                                            */
/*  Output           :                                                        */
/*  Functionality    :  Called to calculate baudrate or select corresponding  */
/*                      BTR1 value from list control. This function will      */
/*                      also update the correspoding BRP value.               */
/*  Member of        :  CChangeRegisters                                      */
/*  Friend of        :      -                                                 */
/*                                                                            */
/*  Author(s)        :  Amitesh Bharti                                        */
/*  Date Created     :  15.02.2002                                            */
/******************************************************************************/
void CChangeRegisters:: vCalculateBaudRateNBTR1(CString omStrBtr0)
{
    CString omStrBaudRate   =_T("");
    CString omStrClockFreq  =_T("");
    CString omStrBtr1       =_T("");
    CString omStrLocalBtr0  =_T("");
    CString omStrLocalBtr1  =_T("");
    CString omStrBrp        =_T("");
    CString omStrNbt        =_T("");

    DOUBLE  dBaudRate       = 0;
    UINT    unClockFreq     = 0;
    UINT    unNbt           = 0;
    FLOAT   fNbt            = 0;
    UINT    unBrp           = 0;
    UINT    unProductNbtNBrp= 0;
    uBTR0   sBtr0Reg    ;
    TCHAR* pcStopStr         = NULL;

    // Get the baudrate for BTR0 and BTR1 values.
    dBaudRate = dCalculateBaudRateFromBTRs(omStrBtr0,m_omStrEditBTR1);
    m_omCombClock.GetWindowText(omStrClockFreq);
    unClockFreq         = _tstoi(omStrClockFreq.GetBuffer(MAX_PATH)); 
    
    omStrLocalBtr0      = omStrBtr0;
    sBtr0Reg.ucBTR0     = (UCHAR) lFromCString_2_Long(omStrLocalBtr0, 
                                            &pcStopStr, defHEXADECIMAL);

    // Calculate the NBT and BRP product. and NBT value using BTR0 value 
    unProductNbtNBrp    = (UINT)((unClockFreq/dBaudRate)/2.0*
                            (defFACT_FREQUENCY / defFACT_BAUD_RATE));
    unBrp               = (sBtr0Reg.sBTR0Bit.ucBRPbit+1);
    unNbt               = unProductNbtNBrp/unBrp;
    fNbt                = (FLOAT)unProductNbtNBrp/unBrp;
    // Check for baudrate with baudrate in the edit field. If it is same find 
    // the corresponding BTR0 and BTR1 value in list else calculate the new list
    if(fabs((dBaudRate - m_dEditBaudRate)) >= defVALID_DECIMAL_VALUE)
    {
        //Check if the BTR0 and BTR1 value entered is valid. if Not valid 
        // Restore the previos value else calculate the new list.
        if(unNbt>defMAX_NBT || unNbt<defMIN_NBT || fNbt != unNbt )
        {
            m_omEditBTR0.SetWindowText(m_omStrEditBTR0);
            m_omEditBTR0.SetSel(0, -1,FALSE);
//			if(theApp.m_bFromAutomation == FALSE)
            AfxMessageBox(_T("Invalid Configuration"),MB_OK|MB_ICONSTOP);
        }
        else
        {
            FLOAT  fTempBaudRate;
            fTempBaudRate = (FLOAT)((INT)(dBaudRate * 100000));
            fTempBaudRate = fTempBaudRate/100000;
            omStrBaudRate.Format(_T("%.4f"),fTempBaudRate);
            m_omEditBaudRate.SetWindowText(omStrBaudRate);
            m_dEditBaudRate     = dBaudRate;
            m_omStrEditBaudRate = omStrBaudRate;
            // Calculate the new list display it is list control and set 
            // the selection on row having BTR0 and BTR1 same as
            // edit control value.
            vChangeListBoxValues(defFromKillFocusBTR);
        }
    }
    else
    {
         INT nItemCount   = m_omListCtrlBitTime.GetItemCount();

         LVFINDINFO info;
     
         info.flags = LVFI_PARTIAL|LVFI_STRING;

         omStrLocalBtr0 = _T("0x") + omStrBtr0;
         info.psz   = omStrLocalBtr0;

         INT nItem = m_omListCtrlBitTime.FindItem(&info);
         // Search for the item containing the same BTR0 and BTR1 value.
         if(nItem!= -1 )
         {
              omStrLocalBtr1 = m_omListCtrlBitTime.GetItemText(nItem,
                                                         defBRT1_COLUNM_POS-1);
              omStrLocalBtr1.Replace(defHEX_STRING,defEMPTY_STRING);
              m_omEditBTR1.GetWindowText(omStrBtr1);
              while(omStrLocalBtr1!=omStrBtr1 
                    && omStrLocalBtr1.IsEmpty() == FALSE)
              {
                  nItem++;
                  omStrLocalBtr1 = m_omListCtrlBitTime.GetItemText(nItem,
                                                        defBRT1_COLUNM_POS-1);
                  omStrLocalBtr1.Replace(defHEX_STRING,defEMPTY_STRING);
                  omStrLocalBtr0 = m_omListCtrlBitTime.GetItemText(nItem,0);
                  omStrLocalBtr0.Replace(defHEX_STRING,defEMPTY_STRING);
              }  
             if(omStrLocalBtr1.IsEmpty() == FALSE && omStrLocalBtr0 == omStrBtr0)
             {
                 omStrLocalBtr1 = m_omListCtrlBitTime.GetItemText(nItem,
                                                            defBRT1_COLUNM_POS-1);
                 omStrLocalBtr1.Replace(defHEX_STRING,defEMPTY_STRING);
                 omStrNbt      = m_omListCtrlBitTime.GetItemText(nItem,
                                                            defNBT_COLUNM_POS-1);
                 unNbt         = _tstoi(omStrNbt.GetBuffer(MAX_PATH));
                 if(unNbt >0)
                 {
                    unBrp     = unProductNbtNBrp/unNbt;
                 }
                 TCHAR acStrBrp[32] = {L'\0'};
                 _itot(unBrp, acStrBrp, 10);
                 omStrBrp = acStrBrp;

                 m_omEditBRP.SetWindowText(omStrBrp);
                 omStrLocalBtr1.Replace(defHEX_STRING,defEMPTY_STRING);
                 m_omEditBTR1.SetWindowText(omStrLocalBtr1);
                 vSelSetFocusItemList(nItemCount,nItem);
             }
             else
             {
                m_omEditBTR0.SetWindowText(m_omStrEditBTR0);
//				if(theApp.m_bFromAutomation == FALSE)
                AfxMessageBox(_T("Invalid Configuration"),MB_OK|MB_ICONSTOP);
             }
     }
     else
      {
          m_omEditBTR0.SetWindowText(m_omStrEditBTR0);
//		  if(theApp.m_bFromAutomation == FALSE)
          AfxMessageBox(_T("Invalid Configuration"),MB_OK|MB_ICONSTOP);
      }
    }
}
/******************************************************************************/
/*  Function Name    :  vCalculateBaudRateNBTR0                               */
/*                                                                            */
/*  Input(s)         :  BTR1 value                                            */
/*  Output           :                                                        */
/*  Functionality    :  Called to calculate baudrate or select corresponding  */
/*                      BTR0 value from list control. This function will      */
/*                      also update the correspoding BRP value.               */
/*  Member of        :  CChangeRegisters                                      */
/*  Friend of        :      -                                                 */
/*                                                                            */
/*  Author(s)        :  Amitesh Bharti                                        */
/*  Date Created     :  19.02.2002                                            */
/******************************************************************************/
void CChangeRegisters:: vCalculateBaudRateNBTR0(CString omStrBtr1)
{
    CString omStrBaudRate       = _T("");
    CString omStrClockFreq      = _T("");
    CString omStrBtr0           = _T("");
    CString omStrLocalBtr1      = _T("");
    CString omStrLocalBtr0      = _T("");
    CString omStrBrp            = _T("");
    CString omStrNbt            = _T("");
    CString omStrComboEditItem  = _T("");
    DOUBLE  dBaudRate           = 0;
    UINT    unClockFreq         = 0;
    UINT    unNbt               = 0;
    UINT    unBrp               = 0;
    FLOAT   fBrp                = 0;
    WORD    usProductNbtNBrp    = 0;
    INT     nSample             = 0;
    INT     nSampleChange       = 0;
    INT     nGetValue           = 0;
    TCHAR*  pcStopStr           = NULL;

    uBTR1   sBtr1Reg;
    uBTR0   sBtr0Reg;

    m_omEditBaudRate.GetWindowText(omStrBaudRate);
    dBaudRate           = (DOUBLE) _tstof(omStrBaudRate);
    
    m_omCombClock.GetWindowText(omStrClockFreq);
    unClockFreq         = _tstoi(omStrClockFreq.GetBuffer(MAX_PATH));
    
    omStrLocalBtr1      = omStrBtr1;
    // Get the baudrate for BTR0 and BTR1 values.
    dBaudRate = dCalculateBaudRateFromBTRs(m_omStrEditBTR0,omStrBtr1 );
    sBtr1Reg.ucBTR1     = (UCHAR) lFromCString_2_Long(omStrLocalBtr1, 
                                            &pcStopStr, defHEXADECIMAL);
    // Calculate the NBT and BRP product. and NBT value using BTR0 value 
    usProductNbtNBrp    = (WORD)((unClockFreq/dBaudRate)/2.0*
                                    (defFACT_FREQUENCY / defFACT_BAUD_RATE));
    unNbt               = (sBtr1Reg.sBTR1Bit.ucTSEG1bit+1)+
                                (sBtr1Reg.sBTR1Bit.ucTSEG2bit+1)+1;
    unBrp               = usProductNbtNBrp/unNbt; 
    fBrp                = (FLOAT)usProductNbtNBrp/unNbt; 
    // Check for number of sampling for new BTR1 value and get the already 
    // selected value
     nGetValue =  m_omCombSampling.GetCurSel();
     if (nGetValue !=CB_ERR)
     {
         m_omCombSampling.GetLBText(nGetValue,omStrComboEditItem);
     }
     nSample = _tstoi(omStrComboEditItem.GetBuffer(MAX_PATH));
     if(sBtr1Reg.sBTR1Bit.ucSAMbit==0)
     {
          nSampleChange = 1;
     }
     else
     {
          nSampleChange = 3;
     }

    // Check for baudrate with baudrate in the edit field. If it is same find 
    //the corresponding BTR0 and BTR1 value in list else calculate the new list
    if( fabs((dBaudRate - m_dEditBaudRate)) >=defVALID_DECIMAL_VALUE)
    {

        //Check if the BTR0 and BTR1 value entered is valid. if Not valid 
        // Restore the previos value else calculate the new list.
        if(unBrp>defMAX_BRP || unBrp<defMIN_BRP || unBrp != fBrp
                            || unNbt>defMAX_NBT || unNbt<defMIN_NBT)
        {
            m_omEditBTR1.SetWindowText(m_omStrEditBTR1);
            m_omEditBTR1.SetSel(0, -1,FALSE);
//			if(theApp.m_bFromAutomation == FALSE)
            AfxMessageBox(_T("Invalid Configuration"), MB_OK|MB_ICONSTOP);
        }
        else
        {
            FLOAT  fTempBaudRate;
            fTempBaudRate = (FLOAT)((INT)(dBaudRate * 100000));
            fTempBaudRate = fTempBaudRate/100000;
            omStrBaudRate.Format(_T("%.4f"), fTempBaudRate);
            m_omEditBaudRate.SetWindowText(omStrBaudRate);
            m_dEditBaudRate     = dBaudRate;
            m_omStrEditBaudRate = omStrBaudRate;
            // change the number of sampling field, if required before 
            // calculating the new list
            if(nSampleChange != nSample)
            {
               m_omCombSampling.SetCurSel(sBtr1Reg.sBTR1Bit.ucSAMbit);
            }
            // Calculate the new list display it is list control and set 
            // the selection on row having BTR0 and BTR1 same as
            // edit control value.
            vChangeListBoxValues(defFromKillFocusBTR);
        }
    }
    else 
    {
         // change the number of sampling field, if required before 
         // calculating the new list
         if(nSampleChange != nSample)
         {
           m_omCombSampling.SetCurSel(sBtr1Reg.sBTR1Bit.ucSAMbit);
           vChangeListBoxValues(-1);
         }
         INT nColumnCount = m_omListCtrlBitTime.GetHeaderCtrl()->GetItemCount();
         INT nItemCount   = m_omListCtrlBitTime.GetItemCount();
         sBtr0Reg.sBTR0Bit.ucBRPbit = unBrp - 1;
         sBtr0Reg.sBTR0Bit.ucSJWbit = 1;

         LVFINDINFO info;
     
         info.flags = LVFI_PARTIAL|LVFI_STRING;

         omStrLocalBtr0.Format(_T("0x%0x"), sBtr0Reg.ucBTR0);

         info.psz   = omStrLocalBtr0;

         INT nItem = m_omListCtrlBitTime.FindItem(&info);
         // Search for the item containing the same BTR0 and BTR1 value.
         if(nItem!= -1 )
         {
              omStrLocalBtr1 = m_omListCtrlBitTime.GetItemText(nItem,
                                                    defBRT1_COLUNM_POS-1);
              omStrLocalBtr1.Replace(defHEX_STRING,defEMPTY_STRING);
              while(omStrLocalBtr1!=omStrBtr1 
                     && omStrLocalBtr1.IsEmpty() == FALSE)
              {
                  nItem++;
                  omStrLocalBtr1 = m_omListCtrlBitTime.GetItemText(nItem,
                                                         defBRT1_COLUNM_POS-1);
                  omStrLocalBtr1.Replace(defHEX_STRING,defEMPTY_STRING);
                  omStrBtr0 = m_omListCtrlBitTime.GetItemText(nItem,0);
              } 
              if(omStrLocalBtr1 == omStrBtr1
                 && omStrLocalBtr1.IsEmpty() == FALSE )
              {
                 omStrNbt  = m_omListCtrlBitTime.GetItemText(nItem,
                                                          defNBT_COLUNM_POS-1);
                 unNbt     = _tstoi(omStrNbt.GetBuffer(MAX_PATH));
                 if(unNbt >0)
                 {
                  unBrp     = usProductNbtNBrp/unNbt;
                 }

                 TCHAR acStrBrp[32] = {L'\0'};
                 _itot(unBrp, acStrBrp, 10);
                 omStrBrp = acStrBrp;
                 m_omEditBRP.SetWindowText(omStrBrp);

                 omStrLocalBtr0.Replace(defHEX_STRING,defEMPTY_STRING);
                 m_omEditBTR0.SetWindowText(omStrLocalBtr0);
                 m_omEditBTR1.SetWindowText(omStrBtr1);
                 vSelSetFocusItemList(nItemCount,nItem);
              }
              else
              {
                  m_omEditBTR1.SetWindowText(m_omStrEditBTR1);
//				  if(theApp.m_bFromAutomation == FALSE)
                  AfxMessageBox(_T("Invalid Configuration"),MB_OK|MB_ICONSTOP);
              }
         }
    }
}
/******************************************************************************/
/*  Function Name    :  vSelSetFocusItemList                                  */
/*                                                                            */
/*  Input(s)         : Total number of item in list control and item number   */
/*                     for selection from list control                        */ 
/*  Output           :                                                        */
/*  Functionality    :  Called when focus is set on baudrate edit box control */
/*                      Update all data members associated with Dialog        */
/*                      control.                                              */
/*  Member of        :  CChangeRegisters                                      */
/*  Friend of        :      -                                                 */
/*                                                                            */
/*  Author(s)        :  Amitesh Bharti                                        */
/*  Date Created     :  15.02.2002                                            */
/******************************************************************************/
void CChangeRegisters::vSelSetFocusItemList(INT nItemCount,INT nItem)
{
    LVITEM sItem;
    // If there is no defualt or last selected item, selection will be at
    // item number middle of the total item numbers.
    if(nItem ==-1 || nItem>=nItemCount)
    {
        nItem = nItemCount/2;		

		/* Make the current BTR0, BTR1 values as default selection */
		CString omStrItems[defNUMBER_OF_COLUMNS];

	    CString omStrBTR0   = _T("");
		CString omStrBTR1   = _T("");

		m_omEditBTR0.GetWindowText(omStrBTR0);
		m_omEditBTR1.GetWindowText(omStrBTR1);

		// Get the values of subitems having item index as nItem.  
		for(INT i=0 ; i<nItemCount ; i++)
		{
			for(INT j=0 ; j<defNUMBER_OF_COLUMNS ; j++)
			{
			 omStrItems[j] = m_omListCtrlBitTime.GetItemText(i,j);
			}
			omStrItems[defBRT0_COLUNM_POS-1].Replace(defHEX_STRING,defEMPTY_STRING);
			omStrItems[defBRT1_COLUNM_POS-1].Replace(defHEX_STRING,defEMPTY_STRING);
			
			if ( omStrBTR0 == omStrItems[defBRT0_COLUNM_POS-1] &&
				 omStrBTR1 == omStrItems[defBRT1_COLUNM_POS-1] )
			{
				nItem = i;
			}
		}				
    }

    sItem.mask      = LVIF_STATE;
    sItem.iItem     = nItem;

    if (m_omListCtrlBitTime.GetItem(&sItem) == TRUE)
    {
        sItem.state     = LVIS_FOCUSED | LVIS_SELECTED;
        sItem.stateMask = LVIS_SELECTED | LVIS_FOCUSED;
        sItem.iSubItem  = 0;

        m_omListCtrlBitTime.SetItem(&sItem);
        m_omListCtrlBitTime.EnsureVisible(nItem, FALSE);
    }
}
/******************************************************************************/
/*  Function Name    :  OnSetfocusEditBTR0                                    */
/*                                                                            */
/*  Input(s)         :                                                        */
/*  Output           :                                                        */
/*  Functionality    :  Called when focus is set on BTR0 edit box control     */
/*                      Update all data members associated with Dialog        */
/*                      control.                                              */
/*  Member of        :  CChangeRegisters                                      */
/*  Friend of        :      -                                                 */
/*                                                                            */
/*  Author(s)        :  Amitesh Bharti                                        */
/*  Date Created     :  15.02.2002                                            */
/******************************************************************************/
void CChangeRegisters::OnSetfocusEditBTR0() 
{
    // To update the data members before editing it and use it in kill focus
     UpdateData(TRUE);
}

/******************************************************************************/
/*  Function Name    :  OnSetfocusEditBTR1                                    */
/*                                                                            */
/*  Input(s)         :                                                        */
/*  Output           :                                                        */
/*  Functionality    :  Called when focus is set on BTR1 edit box control     */
/*                      Update all data members associated with Dialog        */
/*                      control.                                              */
/*  Member of        :  CChangeRegisters                                      */
/*  Friend of        :      -                                                 */
/*                                                                            */
/*  Author(s)        :  Amitesh Bharti                                        */
/*  Date Created     :  15.02.2002                                            */
/******************************************************************************/
void CChangeRegisters::OnSetfocusEditBTR1() 
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
/*  Member of        :  CChangeRegisters                                      */    
/*  Friend of        :      -                                                 */    
/*                                                                            */    
/*  Author(s)        :  Amitesh Bharti                                        */    
/*  Date Created     :  18.02.2002                                            */
/******************************************************************************/
void CChangeRegisters::vValidateBaudRate()
{
    CString omStrBaudRate       = _T("");
    CString omStrPrvBaudRate    = _T("");
    CString omStrClockFreq      = _T("");
    DOUBLE  dBaudRate           = 0;
    UINT    unClockFreq         = 0;
    UINT    unProductNbtNBrp    = 0;
    DOUBLE  dProductNbtNBrp     = 0; 
    CString omStrMessage        = _T("");
    //INT     nUserOption         = 0;
   
   
    m_omEditBaudRate.GetWindowText(omStrBaudRate);
    dBaudRate           = (FLOAT)_tstof(omStrBaudRate);
    m_dEditBaudRate     = (FLOAT)_tstof(m_omStrEditBaudRate);

    m_omCombClock.GetWindowText(omStrClockFreq);
    unClockFreq          = _tstoi(omStrClockFreq.GetBuffer(MAX_PATH));

    dProductNbtNBrp     = (DOUBLE)(unClockFreq/dBaudRate)/2.0 * 
                                (defFACT_FREQUENCY / defFACT_BAUD_RATE);
    unProductNbtNBrp    = (UINT)(dProductNbtNBrp + 0.5);

    if( fabs((dProductNbtNBrp - unProductNbtNBrp))> defVALID_DECIMAL_VALUE 
          ||unProductNbtNBrp>(defMAX_NBT*defMAX_BRP) 
          || unProductNbtNBrp<defMIN_NBT )
    {
        unProductNbtNBrp =defmcROUND5(dProductNbtNBrp);
        INT nFlag = defRESET;

        while(nFlag == defRESET)
        {
            INT i = 1;
            UINT unNbt = unProductNbtNBrp/i;
            FLOAT fNbt  = (FLOAT)unProductNbtNBrp/i;
            
            while( unNbt >= 1 && i <= defMAX_BRP && nFlag == defRESET )
            {
                if( (unNbt == fNbt) && (unNbt >= defMIN_NBT)
                                     && (unNbt <=defMAX_NBT) )
                {
                    nFlag =defSET;
                }
                else
                {
                    i++;
                    unNbt    = unProductNbtNBrp/ i;
                    fNbt     = (FLOAT)unProductNbtNBrp/ i;
                }
            } //end while( unNbt >=1 && i<=MAX_BRP )
            
            if(nFlag == defRESET && unProductNbtNBrp < (defMIN_NBT *defMIN_BRP))
            {
                unProductNbtNBrp = defMIN_NBT *defMIN_BRP;
            }
            else if( unProductNbtNBrp > ( defMAX_NBT * defMAX_BRP ) 
                                            && nFlag == defRESET )
            {
                unProductNbtNBrp = defMAX_NBT*defMAX_BRP;
            }
            else if( nFlag == defRESET )
            {
                unProductNbtNBrp++;
            }
        }//end while(nFlag==RESET)
        dBaudRate = (DOUBLE)((unClockFreq/2.0)* 
                    ( defFACT_FREQUENCY / defFACT_BAUD_RATE ))/unProductNbtNBrp;

        FLOAT  fTempBaudRate;
        fTempBaudRate = (FLOAT)((INT)(dBaudRate * 100000));
        fTempBaudRate = fTempBaudRate/100000;
        omStrBaudRate.Format(_T("%.4f"),fTempBaudRate);   
        
        omStrMessage.Format(defBAUD_RATE_MESSAGE,omStrBaudRate);
        omStrPrvBaudRate = m_omStrEditBaudRate;
        
        // set the baudrate
        m_omEditBaudRate.SetWindowText(omStrBaudRate);
        }// End if
    // Change the list of BTR0, BTR1, SJW, NBT and sampling if user selected YES
    m_dEditBaudRate     = dBaudRate;
    m_omStrEditBaudRate = omStrBaudRate;
    m_unCombClock       = unClockFreq;
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
/*  Member of        :  CChangeRegisters                                      */    
/*  Friend of        :      -                                                 */    
/*                                                                            */    
/*  Author(s)        :  Amitesh Bharti                                        */    
/*  Date Created     :  18.02.2002                                            */
/******************************************************************************/
void CChangeRegisters::OnClickedOK() 
{
    // Update modified data
    UpdateData( TRUE );
    // Validate Baud rate and find the nearest match
    vValidateBaudRate();
    // Save the changes in to the local data structure
    vUpdateControllerDetails();
    // Update Configuration file
    //theApp.bSetData( CONTROLLER_DETAILS, m_pControllerDetails );
    // Update Hardware Interface Layer
   // if( g_podHardwareInterface->bLoadDataFromConfig() == TRUE )
   // {
   //     int nApply = g_podHardwareInterface->nSetApplyConfiguration();
   //     if( nApply != defERR_OK )
   //     {
   //         // Check for device type to select error message
   //         
   //         
   //         #ifdef PEAL_USB_BUILD
			//if(theApp.m_bFromAutomation == FALSE)
   //         AfxMessageBox( defHARDWARE_ERROR_MSG_USB );

   //         #endif
   //     }
   // }

    // Close the dialog
    m_nDataConfirmStatus = INFO_INIT_DATA_CONFIRMED;
    CDialog::OnOK();
}

/******************************************************************************/
/*  Function Name    :  OnSelchangeCombClock                                  */
/*                                                                            */
/*  Input(s)         :                                                        */
/*  Output           :                                                        */
/*  Functionality    :  This function is called by the framework in response  */
/*                      to the CNB_SELCHANGE message. This message is sent to */
/*                      the dialog box when user change the selection.        */
/*                      New list of BRT0, BTR1 etc will be calculated.        */
/*  Member of        :  CChangeRegisters                                      */
/*  Friend of        :      -                                                 */
/*                                                                            */
/*  Author(s)        :  Amitesh Bharti                                        */
/*  Date Created     :  18.02.2002                                            */
/******************************************************************************/
void CChangeRegisters::OnSelchangeCombClock() 
{
    UINT unSelClock             = 0;
    INT nGetValue               = 0;
    CString omStrComboEditItem  = _T("");

    nGetValue =  m_omCombClock.GetCurSel();
    if (nGetValue !=CB_ERR)
    {
      m_omCombClock.GetLBText(nGetValue,omStrComboEditItem);
    }
    unSelClock = _tstoi(omStrComboEditItem.GetBuffer(MAX_PATH));
    // If user has changed the clock freq. calculate the new list.
    if(m_unCombClock  != unSelClock )
    {
     vChangeListBoxValues(-1);
    }
}

/******************************************************************************/
/*  Function Name    :  OnSetfocusCombClock                                   */
/*                                                                            */
/*  Input(s)         :                                                        */
/*  Output           :                                                        */
/*  Functionality    :  Called when focus is set on Clock combo box control   */
/*                      Update all data members associated with Dialog        */
/*                      control.                                              */
/*  Member of        :  CChangeRegisters                                      */
/*  Friend of        :      -                                                 */
/*                                                                            */
/*  Author(s)        :  Amitesh Bharti                                        */
/*  Date Created     :  15.02.2002                                            */
/******************************************************************************/
void CChangeRegisters::OnSetfocusCombClock() 
{
   UpdateData(TRUE);
}

/******************************************************************************/
/*  Function Name    :  OnSetfocusCombSampling                                */
/*                                                                            */
/*  Input(s)         :                                                        */
/*  Output           :                                                        */
/*  Functionality    :  Called when focus is set on Number of sampling        */
/*                      combo box control. Updates all data members           */
/*                      associated with Dialog control.                       */
/*  Member of        :  CChangeRegisters                                      */
/*  Friend of        :      -                                                 */
/*                                                                            */
/*  Author(s)        :  Amitesh Bharti                                        */
/*  Date Created     :  15.02.2002                                            */
/******************************************************************************/
void CChangeRegisters::OnSetfocusCombSampling() 
{
    UpdateData(TRUE);
}

/******************************************************************************/
/*  Function Name    :  OnHelpInfo                                            */
/*                                                                            */
/*  Input(s)         :                                                        */
/*  Output           :                                                        */
/*  Functionality    :  This function is called by the framework in response  */
/*                      to the WM_HELPINFO message. This message is sent to   */
/*                      the dialog box during help request from user.         */
/*  Member of        :  CChangeRegisters                                      */
/*  Friend of        :      -                                                 */
/*                                                                            */
/*  Author(s)        :  Amitesh Bharti                                        */
/*  Date Created     :  15.02.2002                                            */
/******************************************************************************/
BOOL CChangeRegisters::OnHelpInfo(HELPINFO* pHelpInfo) 
{
    return CDialog::OnHelpInfo(pHelpInfo);
}

/******************************************************************************/
/*  Function Name    :  dCalculateBaudRateFromBTRs                            */
/*                                                                            */
/*  Input(s)         :  BTR0 and BTR1                                         */
/*  Output           :  baudrate                                              */
/*  Functionality    :  This function is called to calcualte the baudrate for */
/*                      given BTR0 and BTR1 value.                            */
/*  Member of        :  CChangeRegisters                                      */
/*  Friend of        :      -                                                 */
/*                                                                            */
/*  Author(s)        :  Amitesh Bharti                                        */
/*  Date Created     :  19.02.2002                                            */
/******************************************************************************/
DOUBLE CChangeRegisters::dCalculateBaudRateFromBTRs(CString omStrBTR0, 
                                                        CString omStrBTR1)
{
    uBTR1 uBTR1val      ; 
    uBTR0 uBTR0val      ;
    DOUBLE dBaudRate    = 0;
    BYTE   byBRP        = 0;
    BYTE   byNBT        = 0;
    BYTE   byTSEG1      = 0;
    BYTE   byTSEG2      = 0;
    TCHAR* pcStopStr     = NULL;
    
    uBTR0val.ucBTR0 = (UCHAR) lFromCString_2_Long(omStrBTR0, &pcStopStr,defHEXADECIMAL);
    uBTR1val.ucBTR1 = (UCHAR) lFromCString_2_Long(omStrBTR1, &pcStopStr,defHEXADECIMAL);

    // BRP = BRPbit+1
    byBRP      = static_cast <BYTE> (uBTR0val.sBTR0Bit.ucBRPbit + 1);
   
    //  TSEG1 = TSEG1bit +1
    byTSEG1    = static_cast <BYTE> (uBTR1val.sBTR1Bit.ucTSEG1bit + 1 );

    //TSEG2 = TSEG2bit+1;
    byTSEG2    =  static_cast <BYTE> (uBTR1val.sBTR1Bit.ucTSEG2bit + 1 ); 

    //NBT = TESG1 + TSEG2 +1
    byNBT      = static_cast <BYTE> ( byTSEG1 + byTSEG2 + 1) ;

    dBaudRate  = (DOUBLE)(_tstoi(defCLOCK)/ ( 2.0 * byBRP * byNBT ));
    dBaudRate  = dBaudRate * (defFACT_FREQUENCY / defFACT_BAUD_RATE);

    return dBaudRate;
}

/******************************************************************************/
/*  Function Name    : OnCbtnAcceptance                                       */
/*                                                                            */
/*  Input(s)         :                                                        */
/*  Output           :                                                        */
/*  Functionality    : This function is called by framework when user select  */
/*                     to configure Acceptance Filter. The acceptance filter  */
/*                     dialog will get initialised.                           */
/*  Member of        : CChangeRegisters                                       */
/*  Friend of        :      -                                                 */
/*  Author(s)        : Amitesh Bharti                                         */
/*  Date Created     : 26.03.2003                                             */
/******************************************************************************/
void CChangeRegisters::OnCbtnAcceptance() 
{
    // Send the controller details of selected channel
    // This dialog will modifiy this pointer as per user change
    CAcceptanceFilterDlg odAcceptance( NULL,
                    &m_pControllerDetails[ m_nLastSelection ]);
    odAcceptance.DoModal();
}

/******************************************************************************/
/*  Function Name    : bFillControllerConfig                                  */
/*                                                                            */
/*  Input(s)         : -                                                      */
/*  Output           :                                                        */
/*  Functionality    : This function is called to read registry  or ini file  */
/*                     and return the baudrate ( BTR0 and BTR1 ). If there is */
/*                     no entry return the default value                      */
/*  Member of        : CChangeRegisters                                       */
/*  Friend of        :      -                                                 */
/*                                                                            */
/*  Author(s)        : Amitesh Bharti                                         */
/*  Date Created     : 26.03.2003                                             */
/******************************************************************************/
BOOL CChangeRegisters::bFillControllerConfig()
{
    BOOL bReturn = FALSE;
    // Get the data from the configuration module
    //theApp.bGetData(CONTROLLER_DETAILS, (void**) &m_pControllerDetails);
    // If successful then set the result to pass
    if (m_pControllerDetails != NULL)
    {
        bReturn = TRUE;
    }
    // Return the result
    return bReturn;
}

/******************************************************************************/
/*  Function Name    :  ~CChangeRegisters                                     */
/*                                                                            */
/*  Input(s)         :                                                        */
/*  Output           :                                                        */
/*  Functionality    :  This is destructor of the class. It is called when    */
/*                      object of this class is being destroyed. All memory   */
/*                      allocation is deleted here.                           */
/*  Member of        :  CChangeRegisters                                      */
/*  Friend of        :      -                                                 */
/*                                                                            */
/*  Author(s)        :  Amitesh Bharti                                        */
/*  Date Created     :  07.04.2003                                            */
/******************************************************************************/
CChangeRegisters::~CChangeRegisters()
{
    // Clear memory used for getting controller information
    if( m_pControllerDetails != NULL )
    {
//        theApp.vRelease( CONTROLLER_DETAILS, (VOID**)&m_pControllerDetails);
    }
}

/******************************************************************************/
/*  Function Name    :  OnItemchangedLstcBtrList                              */
/*                                                                            */
/*  Input(s)         :  NMHDR* pNMHDR, LRESULT* pResult                       */
/*  Output           :  LRESULT* pResult                                      */
/*  Functionality    :  This function will be called by framework when item in*/
/*                      list control is changed. Based on the index of        */
/*                      selected and focused item, the other field will be    */
/*                      updated by calling vUpdateBTRsBRPEditWindow(..)       */
/*  Member of        :  CChangeRegisters                                      */
/*  Friend of        :      -                                                 */
/*                                                                            */
/*  Author(s)        :  Amitesh Bharti                                        */
/*  Date Created     :  23.10.2003                                            */
/******************************************************************************/
void CChangeRegisters::OnItemchangedLstcBtrList(NMHDR* pNMHDR, LRESULT* pResult) 
{
    NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
    INT nColumnCount = m_omListCtrlBitTime.GetHeaderCtrl()->GetItemCount();
    UINT unItemStateMask = LVNI_SELECTED|LVNI_FOCUSED;
    if(nColumnCount !=defERROR && pNMListView->uNewState == unItemStateMask)
    {
        vUpdateBTRsBRPEditWindow(nColumnCount,pNMListView->iItem);
    }
    *pResult = 0;
}

/*******************************************************************************
  Function Name  : OnClickListChannels
  Input(s)       : pNMHDR - Pointer to Notification Block
                   pResult - Pointer to the result
  Output         : -
  Functionality  : This function will be called when the user clicks the channel
                   list item. This will set the focus to the last selected item
                   if user clicks outside
  Member of      : CChangeRegisters
  Author(s)      : Raja N
  Date Created   : 14.3.2005
  Modifications  : 
*******************************************************************************/
void CChangeRegisters::OnClickListChannels(NMHDR* /*pNMHDR*/, LRESULT* pResult) 
{
    // Get the selection mask
    UINT unItemStateMask = LVNI_SELECTED|LVNI_FOCUSED;
    // Get the selected item index
    int nSel = m_omChannelList.GetNextItem( -1, LVNI_SELECTED );
    // If nothing is selected then set the selection to the last saved index
    if(nSel == -1)
    {
        m_omChannelList.SetItemState( m_nLastSelection,
                                      unItemStateMask,
                                      unItemStateMask );
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
  Member of      : CChangeRegisters
  Author(s)      : Raja N
  Date Created   : 14.03.2005
  Modifications  :
*******************************************************************************/
void CChangeRegisters::OnItemchangedListChannels(NMHDR* pNMHDR, LRESULT* pResult) 
{
    if (::IsWindow(m_hWnd) == FALSE)
    {
        *pResult = 0;
        return;
    }
    // Get the List item data from the notification
    NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
    // Create selection mask
    UINT unItemStateMask = LVIS_SELECTED | LVIS_FOCUSED;
    // If new state is selected then show selected channel details
    if(pNMListView->uNewState == unItemStateMask)
    {
        // Set the selection
        m_nLastSelection = pNMListView->iItem;
        // Update the UI Controls with the
        vFillControllerConfigDetails();

    }
    // If it is lose of focus then save the user changes
    else if( pNMListView->uChanged  == LVIF_STATE && 
             pNMListView->uOldState == LVIS_SELECTED )
    {
        // Update modified data
        UpdateData( TRUE );
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
  Member of      : CChangeRegisters
  Author(s)      : Raja N
  Date Created   : 14.3.2005
  Modifications  : 
*******************************************************************************/
void CChangeRegisters::OnDblclkListChannels(NMHDR* /*pNMHDR*/, LRESULT* pResult) 
{
    // Create selection mask
    UINT unItemStateMask = LVNI_SELECTED | LVNI_FOCUSED;
    // Get current selection
    int nSel = m_omChannelList.GetNextItem( -1, LVNI_SELECTED );
    // If nothing got selected restore last selection
    if(nSel == -1)
    {
        m_omChannelList.SetItemState( m_nLastSelection,
                                      unItemStateMask,
                                      unItemStateMask );
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
  Member of      : CChangeRegisters
  Author(s)      : Raja N, Arunkumar K
  Date Created   : 14.3.2005
  Modifications  : 17/11/2011, Updated with self reception checking
*******************************************************************************/
void CChangeRegisters::vFillControllerConfigDetails()
{
    int nIndex = m_nLastSelection;
    /* Add hardware info to the description field */
    CWnd* pWnd = GetDlgItem(IDC_EDIT_CHANNEL_DESC);
    if (pWnd != NULL)
    {
        pWnd->SetWindowText(m_pControllerDetails[nIndex].m_omHardwareDesc);
    }
    TCHAR* pcStopStr     = NULL;
    
    m_omStrEditBaudRate     = m_pControllerDetails[ nIndex ].m_omStrBaudrate;
    m_omStrEditBTR0         = m_pControllerDetails[ nIndex ].m_omStrBTR0;
    m_omStrEditBTR1         = m_pControllerDetails[ nIndex ].m_omStrBTR1;
    m_omStrComboClock       = m_pControllerDetails[ nIndex ].m_omStrClock;
    m_omStrComboSampling    = m_pControllerDetails[ nIndex ].m_omStrSampling;
    m_omStrEditWarningLimit = m_pControllerDetails[ nIndex ].m_omStrWarningLimit;

	m_usBTR0BTR1 = static_cast<USHORT>(m_pControllerDetails[ nIndex ].m_nBTR0BTR1);
    
    int nSample             = _tstoi(m_omStrComboSampling.GetBuffer(MAX_PATH));
    //omStrInitComboBox(ITEM_SAMPLING,1,m_omCombSampling));
    //Assign edit box string value to CString member variable of Edit control
    // for Baudrate Convert String into float or INT to be used to make a list
    // of all possible  of BTRi, SJW, Sampling Percentage, and NBT values
    m_unCombClock       = (UINT)_tstoi(m_omStrComboClock.GetBuffer(MAX_PATH));

    m_dEditBaudRate =
        dCalculateBaudRateFromBTRs( m_omStrEditBTR0, m_omStrEditBTR1 );

    UpdateData(FALSE);
    unsigned int unIndex = 0;
    int nReturn = 
        nListBoxValues( m_asColListCtrl, m_dEditBaudRate, (WORD)m_unCombClock,
                        &unIndex,nSample );

    // if Function returns Success display the item and set the focus to last
    // saved item or at item which is at the mid of the list. Update edit boxes
    // for BRP, BTRi by calling function vUpdateBtriBrpEditWindow(..,..)
    if(nReturn == defSUCCESS)
    {   

        // read the item to set the focus from the configuration
        if(m_pControllerDetails != NULL)
        {
            nReturn =
                m_pControllerDetails[ m_nLastSelection ].m_nItemUnderFocus;
        }

        vDisplayListBox(unIndex,nReturn);
    }
	//Set the self reception option
	CButton* pCheckSelfRec = (CButton*)GetDlgItem(IDC_CHKB_SELF_RECEPTION);
    if (pCheckSelfRec != NULL)
    {
		if ( m_pControllerDetails[ m_nLastSelection ].m_bSelfReception )
		{
			pCheckSelfRec->SetCheck(BST_CHECKED);
		}
		else
		{
			pCheckSelfRec->SetCheck(BST_UNCHECKED);
		}
    }
}

/*******************************************************************************
  Function Name  : vUpdateControllerDetails
  Input(s)       : -
  Output         : -
  Functionality  : This function will save the user enter values for baud rate
                   into the controller configuration structure
  Member of      : CChangeRegisters
  Author(s)      : Raja N
  Date Created   : 14.3.2005
  Modifications  : 
*******************************************************************************/
void CChangeRegisters::vUpdateControllerDetails()
{
    TCHAR* pcStopStr                = NULL;
    CString omStrComboSampling      = _T("");
    CString omStrEditBtr0           = _T("");
    CString omStrEditBtr1           = _T("");
    CString omStrEditAcceptanceCode = _T("");
    CString omStrEditAcceptanceMask = _T("");
    // Update the data members before writing into ini file or registry.
    UpdateData(TRUE);

    // Get the warning limit.
    UINT unWarningLimit = 0;
    unWarningLimit = (UINT) lFromCString_2_Long(m_omStrEditWarningLimit, &pcStopStr, defBASE_DEC);

    UINT unWarningLimtMin = static_cast <UINT> (defWARNING_LIMIT_MIN );
    UINT unWarningLimtMax = static_cast <UINT> (defWARNING_LIMIT_MAX );

    if(   ( unWarningLimit >= unWarningLimtMin ) 
       && ( unWarningLimit <= unWarningLimtMax ) )
    {
        INT nItemUnderFocus = m_omListCtrlBitTime.GetNextItem(-1, LVNI_SELECTED );
        m_ucWarningLimit = static_cast <UCHAR> (unWarningLimit);
        UCHAR ucBtr0 = (UCHAR) lFromCString_2_Long(m_omStrEditBTR0, &pcStopStr, defHEXADECIMAL);
        UCHAR ucBtr1 = (UCHAR) lFromCString_2_Long(m_omStrEditBTR1, &pcStopStr, defHEXADECIMAL);
        // Pack the BTR0 and BTR1 values in two bytes before calling DIL fuction 
        // to initialise.
        m_usBTR0BTR1 = static_cast <USHORT>(((ucBtr0 << 8 )| ucBtr1) & 0xffff);
        
        // Update controller information
        //int nIndex = m_nLastSelection;

        m_pControllerDetails[ m_nLastSelection ].m_nBTR0BTR1 = m_usBTR0BTR1;
        m_pControllerDetails[ m_nLastSelection ].m_nItemUnderFocus   =
                                                            nItemUnderFocus;

        _tcscpy(m_pControllerDetails[m_nLastSelection].m_omStrBaudrate, m_omStrEditBaudRate.GetBuffer(MAX_PATH));
        _tcscpy(m_pControllerDetails[m_nLastSelection].m_omStrBTR0, m_omStrEditBTR0.GetBuffer(MAX_PATH));
        _tcscpy(m_pControllerDetails[m_nLastSelection].m_omStrBTR1, m_omStrEditBTR1.GetBuffer(MAX_PATH));
        _tcscpy(m_pControllerDetails[m_nLastSelection].m_omStrClock, m_omStrComboClock.GetBuffer(MAX_PATH));
        _tcscpy(m_pControllerDetails[m_nLastSelection].m_omStrSampling, m_omStrComboSampling.GetBuffer(MAX_PATH));
        _tcscpy(m_pControllerDetails[m_nLastSelection].m_omStrWarningLimit, m_omStrEditWarningLimit.GetBuffer(MAX_PATH));

		//Get the self reception option
		CButton* pCheckSelfRec = (CButton*)GetDlgItem(IDC_CHKB_SELF_RECEPTION);
		if (pCheckSelfRec != NULL)
		{
			if ( pCheckSelfRec->GetCheck() == BST_CHECKED )			
			{
				m_pControllerDetails[ m_nLastSelection ].m_bSelfReception = TRUE;
			}
			else
			{
				m_pControllerDetails[ m_nLastSelection ].m_bSelfReception = FALSE;
			}
		}
    }
    else
    {
        // Invalid Warning Limit Error Message
        CString omStrMsg = _T("");
        omStrMsg.Format( defWARNINGLIMIT_MSG, m_omStrEditWarningLimit,
                         defWARNING_LIMIT_MIN,
                         defWARNING_LIMIT_MAX );
//		if(theApp.m_bFromAutomation == FALSE)
        AfxMessageBox(omStrMsg);
        m_omEditWarningLimit.SetFocus();
        m_omEditWarningLimit.SetSel(0, -1,FALSE);
    }
}

/*******************************************************************************
  Function Name  : OnCbtnBlink
  Input(s)       : -
  Output         : -
  Functionality  : This function will be called when the user selects blink from
                   the dialog
  Member of      : CChangeRegisters
  Author(s)      : Raja N
  Date Created   : 7.3.2005
  Modifications  : 
*******************************************************************************/
void CChangeRegisters::OnCbtnBlink() 
{
}

/*******************************************************************************
  Function Name  : bSetBaudRateFromCom
  Input(s)       : -
  Output         : -
  Functionality  : This function will be called from COM function to set baud rate
  Member of      : CChangeRegisters
  Author(s)      : Anish
  Date Created   : 21.06.06
  Modifications  : 
*******************************************************************************/
BOOL CChangeRegisters::bSetBaudRateFromCom(int nChannel,BYTE bBTR0,BYTE bBTR1)
{
	BOOL bReturn = FALSE;
	CString omStrBtr0;
	CString omStrBtr1;
	CString omStrBaudRate;
	USHORT m_usBTR0BTR1;
	//UCHAR  ucBtr0                   = 0;
    //UCHAR  ucBtr1                   = 0;
	UINT unIndex					= 0;

	UINT unClock					= 0;
	UINT unSample					= 1;

	//convert all the values in string to  save in file
    //bHexBTR0 = (bBTR0/16)*10 +(bBTR0%10);
	omStrBtr0.Format(_T("%02X"), bBTR0);
	
	omStrBtr1.Format(_T("%02X"), bBTR1);
	//calculate baudrate from BTRs
	double dBaudRate = dCalculateBaudRateFromBTRs(omStrBtr0,omStrBtr1);
	omStrBaudRate.Format(_T("%f"), dBaudRate);
	m_usBTR0BTR1 = static_cast <USHORT>(((bBTR0 << 8 )| bBTR1) & 0xffff);
	//Save the changes for the channels
	unClock       = (UINT) _tstoi(m_pControllerDetails[ nChannel-1 ].m_omStrClock);
	if( (bBTR1 & 0x80 ) != 0)
	{
		unSample		  =  3 ;
	}

	UINT nReturn  = nListBoxValues( m_asColListCtrl,dBaudRate, (WORD)unClock,&unIndex,
                                unSample) ;
	if(nReturn != -1 )
	{
		for (UINT i = 0; i<unIndex; i++)
		{
			if( ( bBTR0 == (m_asColListCtrl[i].uBTRReg0.ucBTR0) )&&
			    ( bBTR1 == (m_asColListCtrl[i].uBTRReg1.ucBTR1) ) )
			{
				m_pControllerDetails[nChannel-1 ].m_nItemUnderFocus = i;	
			}
		}
	}
	m_pControllerDetails[ nChannel-1 ].m_nBTR0BTR1 = m_usBTR0BTR1;

    _tcscpy(m_pControllerDetails[ nChannel-1 ].m_omStrBaudrate, omStrBaudRate.GetBuffer(MAX_PATH));
    _tcscpy(m_pControllerDetails[ nChannel-1 ].m_omStrBTR0, omStrBtr0.GetBuffer(MAX_PATH));
    _tcscpy(m_pControllerDetails[ nChannel-1 ].m_omStrBTR1, omStrBtr1.GetBuffer(MAX_PATH));
    _stprintf(m_pControllerDetails[ nChannel-1  ].m_omStrSampling, _T("%d"), unSample);

	m_nLastSelection = nChannel-1;

	return bReturn;
}

/*******************************************************************************
  Function Name  : vGetBaudRateFromCom
  Input(s)       : -
  Output         : -
  Functionality  : This function will be called from COM function to set baud rate
  Member of      : CChangeRegisters
  Author(s)      : Anish
  Date Created   : 21.06.06
  Modifications  : 
*******************************************************************************/
BOOL CChangeRegisters::bGetBaudRateFromCom(int nChannel,BYTE &bBTR0,BYTE &bBTR1)
{
	BOOL bReturn =FALSE;
	if(m_pControllerDetails != NULL)
	{
		 int nTempBTR0BTR1 = m_pControllerDetails[ nChannel-1 ].m_nBTR0BTR1;
		 bBTR1 = (BYTE)(nTempBTR0BTR1 & 0XFF);
		 bBTR0 = (BYTE)((nTempBTR0BTR1>>defBITS_IN_BYTE  ) & 0XFF);

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
 Member of      : CChangeRegisters
 Author(s)      : Anish kr
 Date Created   : 05.06.06

*******************************************************************************/
BOOL CChangeRegisters::bSetFilterFromCom(BOOL  bExtended, DWORD  dBeginMsgId, 
								   DWORD dEndMsgId)
{
	BOOL bReturn = FALSE;
	// for getting separate byte
	DWORD dTemp=0XFF;

	for( UINT unIndex = 0;
	unIndex < defNO_OF_CHANNELS;
	unIndex++ )
    {
        // To set no. shifts
        int nShift = sizeof( UCHAR ) * defBITS_IN_BYTE;
		
		//to convert all acceptance and mask byets into string
		CString omStrTempByte;
		// Create Code
		omStrTempByte.Format(_T("%02X"),(dTemp & ( dBeginMsgId)));
        _tcscpy(m_pControllerDetails[ unIndex ].m_omStrAccCodeByte4[bExtended], omStrTempByte.GetBuffer(MAX_PATH));

		omStrTempByte.Format(_T("%02X"),(dTemp & ( dBeginMsgId >> nShift)));
        _tcscpy(m_pControllerDetails[ unIndex ].m_omStrAccCodeByte3[bExtended], omStrTempByte.GetBuffer(MAX_PATH));

		omStrTempByte.Format(_T("%02X"),(dTemp & ( dBeginMsgId >> nShift * 2)));
        _tcscpy(m_pControllerDetails[ unIndex ].m_omStrAccCodeByte2[bExtended], omStrTempByte.GetBuffer(MAX_PATH));

		omStrTempByte.Format(_T("%02X"),(dTemp & ( dBeginMsgId >> nShift * 3)));
        _tcscpy(m_pControllerDetails[ unIndex ].m_omStrAccCodeByte1[bExtended], omStrTempByte.GetBuffer(MAX_PATH));

        // Create Mask
		omStrTempByte.Format(_T("%02X"),(dTemp & ( dEndMsgId )));
        _tcscpy(m_pControllerDetails[ unIndex ].m_omStrAccMaskByte4[bExtended], omStrTempByte.GetBuffer(MAX_PATH));

		omStrTempByte.Format(_T("%02X"),(dTemp & ( dEndMsgId >> nShift)));
        _tcscpy(m_pControllerDetails[ unIndex ].m_omStrAccMaskByte3[bExtended], omStrTempByte.GetBuffer(MAX_PATH));

		omStrTempByte.Format(_T("%02X"),(dTemp & ( dEndMsgId >> nShift * 2)));
        _tcscpy(m_pControllerDetails[ unIndex ].m_omStrAccMaskByte2[bExtended], omStrTempByte.GetBuffer(MAX_PATH));

		omStrTempByte.Format(_T("%02X"),(dTemp & ( dEndMsgId >> nShift * 3)));
        _tcscpy(m_pControllerDetails[ unIndex ].m_omStrAccMaskByte1[bExtended], omStrTempByte.GetBuffer(MAX_PATH));

		m_pControllerDetails[ unIndex ].m_bAccFilterMode = bExtended;
	}
	
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
 Member of      : CChangeRegisters
 Author(s)      : Anish kr
 Date Created   : 05.06.06

*******************************************************************************/
BOOL CChangeRegisters::bGetFilterFromCom(BOOL  &bExtended, double  &dBeginMsgId, 
								   double &dEndMsgId)
{
	BOOL bReturn = FALSE;
	if(m_pControllerDetails != NULL)
	{
		
		TCHAR* pcStopStr = NULL;
		//Change to separate integer value for each byte
        int nAccCodeByte1 = lFromStrTCHAR_2_Long(m_pControllerDetails[0].m_omStrAccCodeByte1[0], &pcStopStr, defHEXADECIMAL);
        int nAccCodeByte2 = lFromStrTCHAR_2_Long(m_pControllerDetails[0].m_omStrAccCodeByte2[0], &pcStopStr, defHEXADECIMAL);
        int nAccCodeByte3 = lFromStrTCHAR_2_Long(m_pControllerDetails[0].m_omStrAccCodeByte3[0], &pcStopStr, defHEXADECIMAL);
        int nAccCodeByte4 = lFromStrTCHAR_2_Long(m_pControllerDetails[0].m_omStrAccCodeByte4[0], &pcStopStr, defHEXADECIMAL);

        int nMaskCodeByte1 = lFromStrTCHAR_2_Long(m_pControllerDetails[0].m_omStrAccMaskByte1[0], &pcStopStr, defHEXADECIMAL);
        int nMaskCodeByte2 = lFromStrTCHAR_2_Long(m_pControllerDetails[0].m_omStrAccMaskByte2[0], &pcStopStr, defHEXADECIMAL);
        int nMaskCodeByte3 = lFromStrTCHAR_2_Long(m_pControllerDetails[0].m_omStrAccMaskByte3[0], &pcStopStr, defHEXADECIMAL);
        int nMaskCodeByte4 = lFromStrTCHAR_2_Long(m_pControllerDetails[0].m_omStrAccMaskByte4[0], &pcStopStr, defHEXADECIMAL);

		//now make them as dword in decimal
		dBeginMsgId = (ULONG)(nAccCodeByte1*0X1000000+nAccCodeByte2*0X10000+
			nAccCodeByte3*0X100+nAccCodeByte4);
		dEndMsgId = (ULONG)(nMaskCodeByte1*0X1000000+nMaskCodeByte2*0X10000+
			nMaskCodeByte3*0X100+nMaskCodeByte4);
		
		
		bExtended=	m_pControllerDetails[ 0 ].m_bAccFilterMode;
		bReturn=TRUE;
	}
	return bReturn;
}
INT CChangeRegisters::nGetInitStatus()
{
    return m_nDataConfirmStatus;
}