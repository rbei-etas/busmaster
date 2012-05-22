/////////////////////////////////////////////////////////////////////////////
// (c) Copyright The Code Project Open License (CPOL)
//  http://www.codeproject.com/info/cpol10.aspx
//
//  Marius Samoila, Nikolai Teofilov, 2011
//
// FILE NAME
//		About.h: Declaration of the CAboutDlg class
//
// CLASS NAME
//		CAboutDlg
//
// DESCRIPTION
// 
// MODIFICATIONS
//		01-Dec-2011 MSamoila created
//

#ifndef __ABOUT__H_
#define __ABOUT__H_

#include "resource.h"
#include "DmGraphVer.h"


class CAboutDlg : public CDialogImpl<CAboutDlg>
{
public:
	enum { IDD = IDD_ABOUT };

protected:
BEGIN_MSG_MAP(CAboutDlg)
	MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
	COMMAND_ID_HANDLER(IDOK, OnCancel)
	COMMAND_ID_HANDLER(IDCANCEL, OnCancel)
END_MSG_MAP()

	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		TCHAR szVersion[128];
		wsprintf(szVersion, _T("%d.%d.%d%d"), DM_GRAPH_VER);
		SetDlgItemText(IDC_VERSION, szVersion);

		HRSRC hrscr;
		HGLOBAL hMem;
		LPSTR szTxt;
		void* ptr;
		
		hrscr = FindResource(_Module.GetResourceInstance(), MAKEINTRESOURCE( IDR_ABOUT ), _T("TXT"));
		hMem = LoadResource(_Module.GetResourceInstance(), hrscr);
		DWORD dwSize = SizeofResource(_Module.GetResourceInstance(), hrscr);
		ptr = LockResource(hMem);
		if(ptr)
		{
			szTxt = (LPSTR)malloc(dwSize+1);
			if(szTxt)
			{
				memcpy(szTxt, ptr, dwSize);
				szTxt[dwSize] = '\0';
				::SetDlgItemTextA(m_hWnd, IDC_DETAILS, szTxt);
				free(szTxt);
			}

			UnlockResource(hMem);
		}
		FreeResource(hMem);
		
		return 1;  // Let the system set the focus
	}
	LRESULT OnCancel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
	{
		EndDialog(wID);
		return 0;
	}

};

#endif // __ABOUT__H_