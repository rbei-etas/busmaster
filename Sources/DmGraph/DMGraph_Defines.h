/*******************************************************************************
  Project       :  Auto-SAT_Tools
  FileName      :  DMGraph_Defines.h
  Description   :
  Author(s)     :  Arunkumar Karri
  Date Created  :  23/01/2012
  Modified By   :
  Copyright (c) 2012, Robert Bosch Engineering and Business Solutions.  All rights reserved.
*******************************************************************************/

//Cursor state management enums.
typedef enum eDMGraphCursorState
{
    NO_GRAPH_DRAWN,
    WAITING_FOR_OFFSET_OPRN,
    FIRST_OFFSET_DATA,
    LAST_OFFSET_DATA
} EDMGRAPHCURSORSTATE;

typedef enum eDMGraphEvent
{
    ADDED_GRAPH_ELEM,
    GRAPH_ELEM_ALL_CLEAR,
    EVENT_LDBL_CLK,
    EVENT_RDBL_CLK
} EDMGRAPHEVENT;