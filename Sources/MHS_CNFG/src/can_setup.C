/***************************************************************************
                          can_setup.c  -  description
                             -------------------
    begin             : 24.12.2011
    copyright         : Copyright (c) 2011, Robert Bosch Engineering and 
						Business Solutions. All rights reserved.
    author            : Klaus Demlehner, klaus@mhs-elektronik.de
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software, you can redistribute it and/or modify  *
 *   it under the terms of the GNU Lesser General Public License           *
 *   version 2.1 as published by the Free Software Foundation.             *
 *                                                                         *
 ***************************************************************************/

/**
    Library to talk to Tiny-CAN devices. You find the latest versions at
       http://www.tiny-can.com/
**/
#include <windows.h>
#include <basetyps.h>
#include <commctrl.h>
#include <windowsx.h>
#include "global.h"
#include "mhs_types.h"
#include "resource.h"
#include "util.h"
#include "gui_util.h"
#include "mhsbmcfg.h"
#include "can_setup.h"



static struct TMhsCanCfg *CanCfg = nullptr;

#define CanSpeedTabSize 9
static const char *CanSpeedTabStr[] = {"10 kBit/s", "20 kBit/s", "50 kBit/s", "100 kBit/s",
                          "125 kBit/s", "250 kBit/s", "500 kBit/s", "800 kBit/s",
                          "1 MBit/s"};
static const uint32_t CanSpeedTab[] = {10, 20, 50, 100, 125, 250, 500, 800, 1000};
static BOOL g_bBTRRadioBtnChecked = FALSE;

static void EnableDisableControls(HWND hdlg, int nDlgItem)
{	
	HWND hWndBitRate = GetDlgItem(hdlg, IDC_CAN_SPEED);
	HWND hWndBTR = GetDlgItem(hdlg, IDC_BTR_EDIT);

	if (nDlgItem == IDC_RADIOBTN_BITRATE)
	{
		EnableWindow(hWndBitRate, TRUE);	// Enable the bit rate combo box
		EnableWindow(hWndBTR, FALSE);		// Disable the BTR edit box
		g_bBTRRadioBtnChecked = FALSE;
	}
	else
	{
		EnableWindow(hWndBitRate, FALSE);	// Disable the bit rate combo box
		EnableWindow(hWndBTR, TRUE);		// Enable the BTR edit box
		g_bBTRRadioBtnChecked = TRUE;
	}
}

static void InitCanSetupDlg(HWND hdlg, struct TMhsCanCfg *cfg)
{
	int nResIDRadioItem = 0;
	SetDlgItemText(hdlg, IDC_SNR_EDIT, cfg->CanSnrStr);
	SetDlgItemHex(hdlg, IDC_BTR_EDIT, HEX_WORD, cfg->CanBtrValue);

	FillComboBox(hdlg, IDC_CAN_SPEED, CanSpeedTabStr, (const DWORD *) CanSpeedTab,
				 CanSpeedTabSize, cfg->CanSpeed);

	// Set default value as bitrate	
	if ( cfg->m_bBitRateSelected )
	{				
		nResIDRadioItem = IDC_RADIOBTN_BITRATE;
	}
	else
	{		
		nResIDRadioItem = IDC_RADIOBTN_BTR;
	}

	CheckRadioButton(hdlg, IDC_RADIOBTN_BITRATE, IDC_RADIOBTN_BTR, nResIDRadioItem);
	EnableDisableControls(hdlg, nResIDRadioItem);
}

DOUBLE dCalculateBaudRateFromBTRs(UCHAR ucBTR0, UCHAR ucBTR1)
{
    UBTR1 uBTR1val;
    UBTR0 uBTR0val;
    DOUBLE dBaudRate    = 0;
    BYTE   byBRP        = 0;
    BYTE   byNBT        = 0;
    BYTE   byTSEG1      = 0;
    BYTE   byTSEG2      = 0;
    char* pcStopStr     = nullptr;
	DOUBLE dblClock     = 16;

    uBTR0val.ucBTR0 = ucBTR0;
    uBTR1val.ucBTR1 = ucBTR1;

    // BRP = BRPbit+1
    byBRP      = (uBTR0val.sBTR0Bit.ucBRPbit + 1);

    //  TSEG1 = TSEG1bit +1
    byTSEG1    = (uBTR1val.sBTR1Bit.ucTSEG1bit + 1 );

    //TSEG2 = TSEG2bit+1;
    byTSEG2    = (uBTR1val.sBTR1Bit.ucTSEG2bit + 1 );

    //NBT = TESG1 + TSEG2 +1
    byNBT      = ( byTSEG1 + byTSEG2 + 1) ;

    dBaudRate  = (DOUBLE)( dblClock / ( 2.0 * byBRP * byNBT ));
    dBaudRate  = dBaudRate * (defFACT_FREQUENCY / defFACT_BAUD_RATE);

    /* covert to bps */
    dBaudRate*=1000;

    return dBaudRate;
}

static BOOL SaveCanSetup(HWND hdlg, struct TMhsCanCfg *cfg)
{
	short unBTRVal;
	UBTR0 sBtr0Reg;
	UBTR1 sBtr1Reg;	
    UINT    unNbt0          = 0;
    FLOAT   fNbt            = 0;
    UINT    unBrp0          = 0;
    UINT    unNbt1          = 0;
    UINT    unBrp1          = 0;
    FLOAT   fBrp            = 0;
	WORD    unProductNbtNBrp= 0;
	DOUBLE  dBaudRate       = 0;
	DOUBLE  dblClock		= 16;

	GetDlgItemTextCpy(cfg->CanSnrStr, hdlg, IDC_SNR_EDIT, MHS_STR_SIZE);

	if ( g_bBTRRadioBtnChecked )
	{
		cfg->CanBtrValue = GetDlgItemHex(hdlg, IDC_BTR_EDIT);
		unBTRVal = cfg->CanBtrValue;
		sBtr1Reg.ucBTR1 = (0x00FF & unBTRVal);		
		sBtr0Reg.ucBTR0 = (unBTRVal >> 8);

		// Get the baudrate for BTR0 and BTR1 values.
		dBaudRate = dCalculateBaudRateFromBTRs(sBtr0Reg.ucBTR0 ,sBtr1Reg.ucBTR1);

		// Calculate the NBT and BRP product. and NBT value using BTR0 value
		unProductNbtNBrp    = (UINT)((dblClock/(dBaudRate/1000))/2.0*
									 (defFACT_FREQUENCY / defFACT_BAUD_RATE));

		unBrp0              = (sBtr0Reg.sBTR0Bit.ucBRPbit+1);
		unNbt0              = unProductNbtNBrp/unBrp0;
		fNbt                = (FLOAT)unProductNbtNBrp/unBrp0;

		unNbt1              = (sBtr1Reg.sBTR1Bit.ucTSEG1bit+1)+
							  (sBtr1Reg.sBTR1Bit.ucTSEG2bit+1)+1;
		unBrp1              = unProductNbtNBrp/unNbt1;
		fBrp                = (FLOAT)unProductNbtNBrp/unNbt1;

        //Check if the BTR0 and BTR1 value entered is valid. if Not valid
        // Restore the previos value else calculate the new list.
        if( unNbt0>defMAX_NBT || unNbt0<defMIN_NBT || fNbt != unNbt0  ||		    
			unBrp1>defMAX_BRP || unBrp1<defMIN_BRP || unBrp1 != fBrp  || 
			unNbt1>defMAX_NBT || unNbt1<defMIN_NBT )
		{
			MessageBox(hdlg, "Invalid BTRs Configuration!", "Warning", MB_OK|MB_ICONSTOP);
			return FALSE;			
        }
		
	}	
	else
	{
		cfg->CanBtrValue = 0;
		cfg->CanSpeed = GetComboBox(hdlg, IDC_CAN_SPEED);

		/* Validate baud rate selection */
		if ( cfg->CanSpeed > 1000 )
		{
			MessageBox(hdlg, "Please select baud rate!", "Warning", MB_OK|MB_ICONSTOP);
			return FALSE;
		}
	}	

	return TRUE;
}
static BOOL CALLBACK CanSetupDlgProc(HWND hdlg, UINT uMessage, WPARAM wparam, LPARAM lparam)
{
	switch(uMessage)
	  {
	  case WM_INITDIALOG : {
						   InitCanSetupDlg(hdlg, CanCfg);
						   break;
						   }
	  case WM_COMMAND    : {
						   switch (LOWORD(wparam))
							 {
							 case IDOK     : {
											 if( SaveCanSetup(hdlg, CanCfg) )
											 {
												 EndDialog(hdlg, TRUE);
												 return(TRUE);
											 }
											 else
											 {
												 return(FALSE);
											 }
											 }
							 case IDCANCEL : {
											 EndDialog(hdlg, TRUE);
											 return(TRUE);
											 }
							 case IDC_RADIOBTN_BITRATE :
							 case IDC_RADIOBTN_BTR :
							 case BN_CLICKED: {
											  EnableDisableControls(hdlg, wparam);											  
											  }
							 }
						   break;
						   }
	  }
	return(FALSE);
}


int ShowCanSetupDlg(HINSTANCE hInstance, HWND hwnd, struct TMhsCanCfg *cfg)
{
CanCfg = cfg;
if (((DWORD)DialogBox(hInstance, MAKEINTRESOURCE(IDD_CAN_SETUP), hwnd, CanSetupDlgProc)) == TRUE)
  return(1);
else
  return(0);
}