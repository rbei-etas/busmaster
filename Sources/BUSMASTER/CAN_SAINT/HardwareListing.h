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
 * \file      HardwareListing.h
 * \brief     Interface file for CHardwareListing class
 * \author    RaTnadip Choudhury
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Interface file for CHardwareListing class
 */

#pragma once

// For resource symbols6
#include "resource.h"
// For DIL datatypes
#include <map>
#include "afxwin.h"
#include <string>
#include <hash_map>
#include "CResizableDialog.h"
#include "DeviceListInfo.h"
#include "afxcmn.h"
#include "CAN_SAINT.h"
class FaultCode
{
public:

	std::string faultCode;
	std::string DTC;
	std::string type;

	FaultCode(std::string ,std::string,std::string);
	FaultCode();
};

class EmulationMessage
{
public:

	CString receiveMessage;
	CString transmitMessage;

	EmulationMessage(CString ,CString);
	EmulationMessage();
};

CString GetExcelDriver();
/////////////////////////////////////////////////////////////////////////////
// CHardwareListing dialog
class CHardwareListing : public CResizableDialog
{

public:
    // Array of channels


    int m_anSelectedChannels[ CHANNEL_ALLOWED ];
    // Array to hold driver handle
    //SHWNETLIST m_sHwNetList[ MAX_HCANNET ];
    // To update selected hardware detais
    void vUpdateHwDetails( int nIndex );
	static void faultsFailed();
	void finishedFault(CString mem);
	void loadConfig();
	bool passesFilter(CString str);
	void handleEmulations(CString);
	void FilterMessageBox(int ID,CEdit *edit);
	static stdext::hash_map<std::string, std::vector<FaultCode>> getFault();
	CString prevMessageFile;
	CString prevExcelFile;
	CString prevPeriodicFile;
	CString prevFilterFile;
	CString prevEmulationFile;
	CString prevGroupFile;
	CString prevConfigFile;
	void readSettingsFile();
	void repaintTabs();
	void writeSettingsFile();
	int getEPageTransmit();

	std::vector<CString> CAN1_Speed;
	std::vector<CString> CAN2_Speed;
	std::vector<CString> CAN1_Speed_Messages;
	std::vector<CString> CAN2_Speed_Messages;
	CString CAN_Prepare_Message;
	CString CAN_Init_Message;
	CString CAN1_Set;
	std::vector<CString> CAN1_Type;
	std::vector<CString> CAN1_Type_Messages;
	std::vector<CString> CAN1_Sleep_Messages;
	CString CAN2_Set;
	std::vector<CString> CAN2_Type;
	std::vector<CString> CAN2_Type_Messages;
	std::vector<CString> CAN2_Sleep_Messages;
	std::vector<CString> Fault_Code_Requests;
	CString Fault_Code_Data_ID;
	CString Fault_Code_Data_End;
	int Fault_Code_Data_Byte_Start;
	int Fault_Code_Data_Byte_End;
	CString Clear_Fault_Message;
	int ID_Start;
	int ID_End;
	int DTC_Start;
	int DTC_End;
	int Type_Start;
	int Type_End;
	int Status_Start;
	int Status_End;
	CString Active_History_Mask;
	CString Fault_Code_Excel_Page_Name;

    // To set List of hardware handles
    //void vSetHardwareList(INTERFACE_HW*, int );
    // Constructor
    CHardwareListing(INTERFACE_HW*, int , int*,CCAN_SAINTApp *, CWnd* pParent = nullptr);
    // standard constructor
    CHardwareListing();
    //Get selection list
    //Set selection list

    // Dialog Data
    //{{AFX_DATA(CHardwareListing)
    enum { IDD = IDD_DLG_HW_LISTING };
    //}}AFX_DATA


    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CHardwareListing)
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

    // Implementation
protected:

    // Generated message map functions
    //{{AFX_MSG(CHardwareListing)
    virtual BOOL OnInitDialog();
	void SetRectangle();
    virtual void OnOK();
    virtual void OnCancel();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
private:
    // Pointer to Hardware List
public:

public:
    // Hardware CONTAINER
 
	afx_msg void OnBnLoadPerFile();
	void OnChangeToGroup();
	afx_msg void OnBnLoadEmuFile();
	afx_msg void OnBnLoadFltFile();
	afx_msg void OnBnLoadGrpFile();
	afx_msg void OnBnNewFile();
	afx_msg void OnBnSavePerFile();
	afx_msg void OnLbnSelchangeList1();
	afx_msg void OnBnClickedButton1();
	afx_msg bool OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
	virtual BOOL DestroyWindow();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnBnClickedStatic4();
	CTabCtrl tabCtrl;
	afx_msg void OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnTcnSelchangingTab1(NMHDR *pNMHDR, LRESULT *pResult);
	CStatic labelFile;
};