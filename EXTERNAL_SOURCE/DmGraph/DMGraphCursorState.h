/*******************************************************************************
  Project       :  Auto-SAT_Tools
  FileName      :  CCGCtrlStateMachine.h
  Description   :  
  $Log:   X:/Archive/Sources/CGCtrl/CGCtrlStateMachine.h_v  $
 * 
 *    Rev 1.1   15 Apr 2011 19:29:16   rac2kor
 * Inserted RBEI Copyright information text into the file header.

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
