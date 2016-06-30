/*
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/**
 * @file      CGCtrlStateMachine.cpp
 * @author    Raja N
 * @copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 */

/* Project includes */
#include "stdafx.h"
#include ".\DMGraphCursorState.h"

CDMGraphCursorState::CDMGraphCursorState(void)
{
    m_eCursorState = NO_GRAPH_DRAWN;
}

EDMGRAPHCURSORSTATE CDMGraphCursorState::eGetCurrState(void)
{
    return m_eCursorState;
}

EDMGRAPHCURSORSTATE CDMGraphCursorState::eApplyEvents(EDMGRAPHEVENT eEvent)
{
    if(eEvent == ADDED_GRAPH_ELEM)
    {
        if(m_eCursorState == NO_GRAPH_DRAWN)
        {
            m_eCursorState = WAITING_FOR_OFFSET_OPRN;
        }
    }
    else if(eEvent == GRAPH_ELEM_ALL_CLEAR)
    {
        m_eCursorState = NO_GRAPH_DRAWN;
    }
    else if(eEvent == EVENT_LDBL_CLK)
    {
        if(m_eCursorState == WAITING_FOR_OFFSET_OPRN)
        {
            m_eCursorState = FIRST_OFFSET_DATA;
        }
        else if(m_eCursorState == FIRST_OFFSET_DATA)
        {
            m_eCursorState = LAST_OFFSET_DATA;
        }
    }
    else if(eEvent == EVENT_RDBL_CLK)
    {
        if(m_eCursorState != NO_GRAPH_DRAWN)
        {
            m_eCursorState = WAITING_FOR_OFFSET_OPRN;
        }
    }

    return m_eCursorState;
}
