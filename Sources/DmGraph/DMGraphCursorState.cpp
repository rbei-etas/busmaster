/*******************************************************************************
  Project       :  Auto-SAT_Tools
  FileName      :  CGCtrlStateMachine.cpp
  Description   :  
  $Log:   X:/Archive/Sources/CGCtrl/CGCtrlStateMachine.cpv  $

  Author(s)     :  
  Date Created  :  
  Modified By   :  
  Copyright (c) 2011, Robert Bosch Engineering and Business Solutions.  All rights reserved.
*******************************************************************************/

#include "stdafx.h"
#include ".\DMGraphCursorState.h"


/******************************************************************************
    Function Name    :  eGetCurrState
    Input(s)         :  -
    Output           :  -
    Functionality    :  Constructor
    Member of        :  CDMGraphCursorState    
    Author(s)        :  ArunKumar K
    Date Created     :  20.12.2010
******************************************************************************/
CDMGraphCursorState::CDMGraphCursorState(void)
{
	m_eCursorState = NO_GRAPH_DRAWN;
}


/******************************************************************************
    Function Name    :  ~CCGCtrlStateMachine
    Input(s)         :  -
    Output           :  -
    Functionality    :  Destructor
    Member of        :  CDMGraphCursorState    
    Author(s)        :  ArunKumar K
    Date Created     :  20.12.2010
******************************************************************************/
CDMGraphCursorState::~CDMGraphCursorState(void)
{
}

/******************************************************************************
    Function Name    :  eGetCurrState
    Input(s)         :  -
    Output           :  EDMGRAPHCURSORSTATE
    Functionality    :  Gets the current state of the cursor display.
    Member of        :  CDMGraphCursorState    
    Author(s)        :  ArunKumar K
    Date Created     :  20.12.2010
******************************************************************************/
EDMGRAPHCURSORSTATE CDMGraphCursorState::eGetCurrState(void)
{
	return m_eCursorState;
}

/******************************************************************************
    Function Name    :  eApplyEvents
    Input(s)         :  EDMGRAPHEVENT eEvent
    Output           :  EDMGRAPHCURSORSTATE
    Functionality    :  Modifies the cursor state depending on input event type.
    Member of        :  CDMGraphCursorState    
    Author(s)        :  ArunKumar K
    Date Created     :  20.12.2010
******************************************************************************/
EDMGRAPHCURSORSTATE CDMGraphCursorState::eApplyEvents(EDMGRAPHEVENT eEvent)
{
	if(eEvent == ADDED_GRAPH_ELEM)
	{
		if(m_eCursorState == NO_GRAPH_DRAWN)
			m_eCursorState = WAITING_FOR_OFFSET_OPRN;		
	}
	else if(eEvent == GRAPH_ELEM_ALL_CLEAR)
	{
		m_eCursorState = NO_GRAPH_DRAWN;
	}
	else if(eEvent == EVENT_LDBL_CLK)
	{
		if(m_eCursorState == WAITING_FOR_OFFSET_OPRN)
			m_eCursorState = FIRST_OFFSET_DATA;
		else if(m_eCursorState == FIRST_OFFSET_DATA)
			m_eCursorState = LAST_OFFSET_DATA;
	}
	else if(eEvent == EVENT_RDBL_CLK)
	{
		if(m_eCursorState != NO_GRAPH_DRAWN)
			m_eCursorState = WAITING_FOR_OFFSET_OPRN;
	}

	return m_eCursorState;
}

/******************************************************************************
    Function Name    :  vReset
    Input(s)         :  -
    Output           :  -
    Functionality    :  Resets the cursor state.
    Member of        :  CDMGraphCursorState    
    Author(s)        :  ArunKumar K
    Date Created     :  20.12.2010
******************************************************************************/
void CDMGraphCursorState::vReset(void)
{
}