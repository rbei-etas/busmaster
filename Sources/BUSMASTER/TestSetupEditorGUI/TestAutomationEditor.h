
// TestAutomationEditor.h : main header file for the TestAutomationEditor application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "stdafx.h"
#include "resource.h"       // main symbols


// CTestAutomationEditorApp:
// See TestAutomationEditor.cpp for the implementation of this class
//

class CTestAutomationEditorApp : public CWinAppEx
{
public:
	CTestAutomationEditorApp();


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CTestAutomationEditorApp theApp;
