// CAN_SAINT.h : main header file for the CAN_SAINT DLL
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols
#include "SBEConnection.h"
class Message
{
public:
	CString data;
	CString comment;
	int delay;
	int enabled;

	CString receiveMessage;
	CString transmitMessage;
	

	Message(CString,CString,int,int);
	Message(CString,CString);
	Message();
};


class GroupTransmit
{
public:
	CString url;
	std::vector<Message> messages;
	CString receive;
	int size;
	int cur;
	void loadURL();

	CString getData(int index);
	CString getDelay(int index);

	

	GroupTransmit(CString,CString);
	GroupTransmit();
};


// CCAN_SAINTApp
// See CAN_SAINT.cpp for the implementation of this class
//
void CAN_ConfigureSaint(int whichCAN, int can1enabled, int can1speed, int can1type, int can2enabled, int can2speed, int can2type);
bool CAN_SendMessage(CString string);
bool checkValidity(CString str);
bool checkWildcardValidity(CString str);

bool CAN_SendFaultCodeRequest();
bool CAN_SendFaultCodeClear();

class CCAN_SAINTApp : public CWinApp
{
public:
	BOOL CCAN_SAINTApp::PreCreateWindow(CREATESTRUCT& cs);
	CCAN_SAINTApp();


// Overrides
protected:
	DECLARE_MESSAGE_MAP()
public:
	void unloadDriver();
	virtual BOOL InitInstance();

};
