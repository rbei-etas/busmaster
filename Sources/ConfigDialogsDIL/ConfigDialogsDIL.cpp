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
 * \file      ConfigDialogsDIL.cpp
 * \brief     Source file for CConfigDialogsDILApp class and the exported 
 * \authors   Ratnadip Choudhury, Pradeep Kadoor
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Source file for CConfigDialogsDILApp class and the exported 
 */


// ConfigDialogsDIL.cpp : Defines the initialization routines for the DLL.
//

#include "CongDlg_stdafx.h"

#define USAGE_EXPORT

#include "DataTypes/DIL_Datatypes.h"
#include "Include/struct_can.h"
#include "Include/CanUsbDefs.h"
#include "ConfigDialogsDIL.h"
#include "ChangeRegisters.h"
#include "ChangeRegisters_CAN_ICS_neoVI.h"
#include "ChangeRegisters_CAN_ETAS_BOA.h"
#include "ControllerPPage.h"
#include "AcceptanceFilterDlg.h"
#include "ListHwInterface.h"
#include "HardwareListing.h"

#define USAGE_EXPORT
#include "API_dialog.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//
//	Note!
//
//		If this DLL is dynamically linked against the MFC
//		DLLs, any functions exported from this DLL which
//		call into MFC must have the AFX_MANAGE_STATE macro
//		added at the very beginning of the function.
//
//		For example:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// normal function body here
//		}
//
//		It is very important that this macro appear in each
//		function, prior to any calls into MFC.  This means that
//		it must appear as the first statement within the 
//		function, even before any object variable declarations
//		as their constructors may generate calls into the MFC
//		DLL.
//
//		Please see MFC Technical Notes 33 and 58 for additional
//		details.
//

// CConfigDialogsDILApp

BEGIN_MESSAGE_MAP(CConfigDialogsDILApp, CWinApp)
END_MESSAGE_MAP()


// CConfigDialogsDILApp construction

CConfigDialogsDILApp::CConfigDialogsDILApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}


// The one and only CConfigDialogsDILApp object

CConfigDialogsDILApp theApp;


// CConfigDialogsDILApp initialization

BOOL CConfigDialogsDILApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}

int CListHWInterface::GetSelIndex()
{
    return m_nSelIndex;
}

USAGEMODE int DisplayConfigurationDlg(HWND hParent, DILCALLBACK /*ProcDIL*/, 
                            PSCONTROLER_DETAILS pControllerDetails, UINT nCount, DWORD dwDriverID)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());
    int nResult = WARNING_NOTCONFIRMED;
    
    if (dwDriverID == DRIVER_CAN_PEAK_USB)
    {
        CChangeRegisters ouChangeRegister(CWnd::FromHandle(hParent), pControllerDetails, nCount);
        ouChangeRegister.DoModal();
        nResult = ouChangeRegister.nGetInitStatus();
    }
    else if ((dwDriverID == DRIVER_CAN_ICS_NEOVI) || (dwDriverID == DRIVER_CAN_ETAS_ES581))
    {
        CChangeRegisters_CAN_ICS_neoVI ouChangeRegister(CWnd::FromHandle(hParent), pControllerDetails, nCount);
        ouChangeRegister.DoModal();
        nResult = ouChangeRegister.nGetInitStatus();
    }
    else if (dwDriverID == DRIVER_CAN_ETAS_BOA)
    {
        CChangeRegisters_CAN_ETAS_BOA ouChangeRegister(CWnd::FromHandle(hParent), pControllerDetails, nCount);
        ouChangeRegister.DoModal();
        nResult = ouChangeRegister.nGetInitStatus();
    }
    else if (dwDriverID == DRIVER_CAN_VECTOR_XL)
    {
        CChangeRegisters_CAN_ETAS_BOA ouChangeRegister(CWnd::FromHandle(hParent), pControllerDetails, nCount);
        ouChangeRegister.DoModal();
        nResult = ouChangeRegister.nGetInitStatus();    	
    }

    return nResult;
}

USAGEMODE int ListHardwareInterfaces(HWND hParent, DWORD dwDriver, INTERFACE_HW* psInterfaces, int* pnSelList, int& nCount)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());
	int Result = -1;
    if ((dwDriver == DRIVER_CAN_ETAS_BOA) ||
        (dwDriver == DRIVER_CAN_PEAK_USB) ||
        (dwDriver == DRIVER_CAN_ICS_NEOVI) ||
        (dwDriver == DRIVER_CAN_ETAS_ES581) ||
		(dwDriver == DRIVER_CAN_VECTOR_XL))
    {
        CHardwareListing HwList(psInterfaces, nCount, NULL);
        HwList.DoModal();
        nCount = HwList.nGetSelectedList(pnSelList);
        Result = 0;
    }
    else
    {
        CListHWInterface ListHWIntr(CWnd::FromHandle(hParent), nCount, psInterfaces);   
        ListHWIntr.DoModal();
	    Result = ListHWIntr.GetSelIndex();
    }
    return Result;
}
