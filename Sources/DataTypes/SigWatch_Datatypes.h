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
 * \file      SigWatch_Datatypes.h
 * \brief     Definition file for signal watch data types.
 * \author    Pradeep Kadoor
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Definition file for signal watch data types.
 */

#pragma once

// Structure for Signal Watch Implementation
struct tagSignalWatchList
{
    unsigned int unMsgID;                   // Message ID
    CStringArray omCSASignals;              // Signals List
    tagSignalWatchList* psNextMessageNode; // Pointer to the next node
    // Init Necessery Values
    tagSignalWatchList()
    {
        unMsgID = 0;
        psNextMessageNode = NULL;
    }
};

typedef tagSignalWatchList sSignalWatchList;
typedef sSignalWatchList* PSSIGNALWATCHLIST;
