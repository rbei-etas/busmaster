
/******************************************************************************
  Project       :  Auto-SAT_Tools
  FileName      :  J1939Translator.h
  Description   :  J1939 Translator application class
  $Log:   X:/Archive/Sources/DIL_J1939/DIL_J1939.h_v  $
 * 
 *    Rev 1.1   15 Apr 2011 18:48:34   CANMNTTM
 * Added RBEI Copyright information.
 * 
 *    Rev 1.0   06 Dec 2010 18:47:20   rac2kor
 *  
  Author(s)     :  Pradeep Kadoor
  Date Created  :  26/10/2010
  Modified By   :  
  Copyright (c) 2011, Robert Bosch Engineering and Business Solutions.  All rights reserved.
******************************************************************************/


// J1939Translator.h : main header file for the MCNetTranslator DLL
//

#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif


// CMCNetTranslatorApp
// See MCNetTranslator.cpp for the implementation of this class
//

class CJ1939TranslatorApp : public CWinApp
{
public:
	CJ1939TranslatorApp();

// Overrides
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
