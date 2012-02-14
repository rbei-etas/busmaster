/*******************************************************************************
  Project       :  Auto-SAT_Tools
  FileName      :  GraphSink.h
  Description   :  Implementation file for CGraphBottomView class     
  Author(s)     :  Arunkumar Karri
  Date Created  :  10/12/2004
  Modified By   : 
  Copyright (c) 2011, Robert Bosch Engineering and Business Solutions.  All rights reserved
*******************************************************************************/
#pragma once

#include "GraphChildFrame.h"

// CGraphSink command target

class CGraphSink : public CCmdTarget
{
	DECLARE_DYNAMIC(CGraphSink)

public:
	CGraphSink();
	virtual ~CGraphSink();

	virtual void OnFinalRelease();
	void OnCursorPosition(double x, double y, short shCursorID);	

	CGraphChildFrame* m_pParentWnd;

protected:
	DECLARE_MESSAGE_MAP()
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()	
};


