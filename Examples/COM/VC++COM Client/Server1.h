// Server1.h : main header file for the Server1 application
//
#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols


// CServer1App:
// See Server1.cpp for the implementation of this class
//

class CServer1App : public CWinApp
{
public:
	CServer1App();


// Overrides
public:
	virtual BOOL InitInstance();

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
    BOOL ExitInstance(void);
};

extern CServer1App theApp;