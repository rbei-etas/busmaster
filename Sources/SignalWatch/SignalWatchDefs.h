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
 * \file      SignalWatchDefs.h
 * \author    Ratnadip Choudhury
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 */

#pragma once

#define WM_REMOVE_SIGNAL            WM_USER + 41
#define defSTR_PHYSICAL_COLUMN      _T("Physical Value")
#define defSTR_RAW_COLUMN           _T("Raw Value")

#define defSW_LIST_COLUMN_COUNT     4
#define defSTR_SW_MSG_NAME          _T("Message")
#define defSTR_SW_MSG_COL           0
#define defSTR_SW_SIG_NAME          _T("Signal")
#define defSTR_SW_SIG_COL           1
#define defSTR_SW_RAW_VALUE         defSTR_RAW_COLUMN
#define defSTR_SW_RAW_VAL_COL       3
#define defSTR_SW_PHY_VALUE         defSTR_PHYSICAL_COLUMN
#define defSTR_SW_PHY_VAL_COL       2
#define defCOLOR_WHITE              RGB(255,255,255)
#define defSTR_SIGNAL_WATCH_FMT     _T(" %-2s %-16s-> %-14s %-10s[%s]")



#define STR_EMPTY _T("")
#define defSTR_MSG_SIG_SEPERATER _T(" -> ")
#define defSTR_FORMAT_SW_LIST    _T("%s%s%s")
#define defSIZE_OF_IMAGE         25
#define defSTR_SW_DELETE_ERROR   _T("Error while deleting temporary list!!!")
#define defSTR_SW_DELETE_SIG_ERROR  _T("Error while deleting Signal %s from Message %s !!!")
#define defSTR_SW_DELETE_SIG_MSGID_ERROR _T("Error while deleting Signal %s from Message ID: %x !!!")
#define defSTR_SW_PARSE_ERROR    _T("Parse Error in %s ")
#define defSTR_SW_SIGNAL_DETAIL_ERROR   _T("Signal Not Found in the Database!!\nPlease remove the Signal")
#define defSTR_SW_SIGNAL_GENERIC_ERROR  _T("Error showing Signal Details!!")
#define defSTR_MSG_ID_IN_HEX          _T("[0x%x]")
#define defMSGID_EXTENDED       'x'
#define defMSGID_NAME_DELIMITER  "["
#define defMSGID_NAME_START_CHAR '['
#define defMSG_NAME_END_CHAR     ']'

#define defSTR_FORMAT_DISPLAY_DEC    _T("%-60s   [%16I64d]")
#define defSTR_FORMAT_DISPLAY_HEX    _T("%-60s   [%16I64X]")
#define defSTR_FORMAT_OL_INTRP   _T("         %-20s %-20s")
#define defSTR_FORMAT_PHY_VALUE  _T("%.3f")
#define defSTR_FORMAT_PHY_VALUE_WITH_UNIT  _T("%16s %s")

#define defSIGNAL_ICON_SIZE               16
