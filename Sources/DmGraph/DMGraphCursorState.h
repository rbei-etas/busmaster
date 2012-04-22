/*******************************************************************************
  Project       :  Auto-SAT_Tools
  FileName      :  CCGCtrlStateMachine.h
  Description   :  
  $Log:   X:/Archive/Sources/CGCtrl/CGCtrlStateMachine.h_v  $

  Author(s)     :  Raja N
  Date Created  :  12/12/2004
  Modified By   :  
  Copyright (c) 2011, Robert Bosch Engineering and Business Solutions.  All rights reserved.
*******************************************************************************/

#pragma once

#include "DMGraph_Defines.h"

class CDMGraphCursorState
{
public:
	CDMGraphCursorState(void);
	~CDMGraphCursorState(void);
public: 
	EDMGRAPHCURSORSTATE eGetCurrState(void);
	EDMGRAPHCURSORSTATE eApplyEvents(EDMGRAPHEVENT eEvent);
	void vReset(void);
private:
	EDMGRAPHCURSORSTATE	m_eCursorState;
};
