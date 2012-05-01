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
 * \file      Filter_defines.h
 * \author    Ratnadip Choudhury
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 */

#pragma once

// Filter
#define defFILTER_TYPE_SINGLE_ID                0
#define defFILTER_TYPE_ID_RANGE                 1

#define defFILTER_IGNORE_CHANNEL        0
#define defFILTER_IGNORE_DIRECTION      0
#define defFILTER_IGNORE_ID_TYPE        2
#define defFILTER_IGNORE_MSG_TYPE       2

#define defSTR_FILTER_NAME          _T("Name")
#define defSTR_FILTER_TYPE          _T("Type")

#define defSTR_FILTER_DETAILS_ID_FROM           _T("From")
#define defSTR_FILTER_DETAILS_ID_TO             _T("To")
#define defSTR_FILTER_DETAILS_ID_TYPE           _T("ID Type")
#define defSTR_FILTER_DETAILS_MSG_TYPE          _T("Msg Type")
#define defSTR_FILTER_DETAILS_MSG_DIRECTION     _T("Direction")
#define defSTR_FILTER_DETAILS_MSG_CHANNEL       _T("Channel")

#define defSTR_SELECTION_ALL                    _T("All")

#define defSTR_MSG_ID_TYPE_STD                  _T("Std")
#define defSTR_MSG_ID_TYPE_EXTD                 _T("Exd")

#define defSTR_MSG_TYPE_RTR                     _T("RTR")
#define defSTR_MSG_TYPE_NON_RTR                 _T("Non RTR")

#define defSTR_MSG_DIR_TX                       _T("Tx")
#define defSTR_MSG_DIR_RX                       _T("Rx")

#define defSTR_FILTER_TYPE_PASS                 _T("Pass")
#define defSTR_FILTER_TYPE_STOP                 _T("Stop")

#define defSTR_MSG_ID_FORMAT_DB                 _T("%x [%s]")
#define defSTR_MSG_ID_FORMAT_NDB                _T("%x")

#define defFILTER_IMAGE_INDEX_DB                0
#define defFILTER_IMAGE_INDEX_NDB               1
#define defFILTER_IMAGE_INDEX_FILTER            2
#define defFILTER_IMAGE_INDEX_ID_RANGE          3

#define defNUMBER_OF_FILTERS_SUPPORTED          100

#define defSTR_FILTER_NAME_COLUMN       _T("Filter Name")

#define defSTR_DEFAULT_FILTER_NAME              _T("Filter")
#define defSTR_DEFAULT_FILTER_FORMAT            _T("%s_%d")

#define defCOLOR_WHITE                          RGB(255,255,255)

#define defFORMAT_MSGID_DECIMAL _T("%d")
#define defMSGID_NAME_START_CHAR '['
#define defSIGNAL_LIST_STRING_MAX_LENGTH     50
//For adding MSgID to message name
#define defSTR_MSG_ID_IN_HEX          _T("[0x%x]")
#define defMSGID_EXTENDED       'x'
#define defMSGID_NAME_DELIMITER  "["
#define defMSGID_NAME_START_CHAR '['
#define defMSG_NAME_END_CHAR     ']'

#define defSTR_MSG_ID_STANDARD              _T("Standard")
#define defSTR_MSG_ID_EXTENDED              _T("Extended")

#define MAX_LMT_FOR_STD_MSG_ID  2048
#define defMAX_LMT_EXTD_MSG_ID 0x20000000


struct tagFilterDisplayInfo
{
    CString m_omStrMsgIDFrom;
    CString m_omStrMsgIDTo;
    CString m_omStrMsgIDType;
    CString m_omStrMsgType;
    CString m_omStrMsgDirection;
    CString m_omStrMsgChannel;
    int     m_nImageIndex;
};

typedef tagFilterDisplayInfo sFilterDisplayInfo;
typedef sFilterDisplayInfo sFilterDisplayInfo;
typedef sFilterDisplayInfo SFILTERDISPLAYINFO;
typedef SFILTERDISPLAYINFO* PSFILTERDISPLAYINFO;


// details of the message filters
struct sMESSAGEFILTERDETAILS
{
    UINT*    m_punUndefinedMsgID;       // ID's of undefined messages. The
    // first element will have the count of
    // message ID's.
    CStringArray    m_omMsgNameArray;   // the database message names in the
    // filter list
    INT     m_nRangeFrom;               // range start information
    INT     m_nRangeTo;                 // range end information
    UINT    m_unCount;                  // count information of message names
    BYTE    m_byFilterType;             // type of filter
    BOOL    m_bChekTxMsg;               // filter transmitted messages
};
typedef sMESSAGEFILTERDETAILS   SMESSAGE_FILTER_DETAILS;
typedef sMESSAGEFILTERDETAILS*   PSMESSAGE_FILTER_DETAILS;
