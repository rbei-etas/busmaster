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
 * \file      TSEditorGUI_Definitions.h
 * \brief     Include file containing all the macro definitions
 * \author    Venkatanarayana Makam
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Include file containing all the macro definitions
 */

#pragma once

//List Control
#define def_COLOR_FIRSTROW      RGB(237, 239, 254)
#define def_COLOR_SECONDROW     RGB(255, 255, 255)
#define def_COLOR_ROW1_TEXT     RGB(0, 0, 0)
#define def_COLOR_ROW2_TEXT     RGB(0, 0, 0)

//Propertyview Definitions
#define def_COLUMN_CATEGORY    0
#define def_COLUMN_VALUE       1
#define defPROPERTYCOLUMN_NUM   2
#define def_WIDTH_LISTITEM      1
#define def_HEIGHT_LISTITEM     16

#define def_COLOR_TREE_BKG     RGB(237, 239, 254)

//Window Definitions
#define def_NUM_ROWS_TSEDITOR   1
#define def_NUM_COLS_TSEDITOR   2

#define def_INDEX_TREEVIEW      0
#define def_INDEX_PROPVIEW      1

#define def_ROW_INDEX           0

#define def_HEIGHT_PANE         150
#define def_WIDTH_PANE          200

#define def_HEIGHT_IMAGE        16
#define def_WIDTH_IMAGE         16

#define def_NUM_IMAGE                   5
#define def_INDEX_SEND_IMAGE            1
#define def_INDEX_VERIFY_IMAGE          2
#define def_INDEX_WAIT_IMAGE            3
#define def_INDEX_REPLAY_IMGAE          4
#define def_INDEX_SENDMSG_IMAGE         2
#define def_INDEX_VERIFYMSG_IMAGE       3


#define defLIST_SEND_ST_ROW             1
#define defLIST_SENDMSG_ST_ROW          2
 
//List Ctrl Row Numbers
//Header

#define def_TS_ROWNUM_TITLE             0
#define def_TS_ROWNUM_DESCRIPTION       1
#define def_TS_ROWNUM_VERSION           2
#define def_TS_ROWNUM_BUSTYPE           3
#define def_TS_ROWNUM_MODULE            4
#define def_TS_ROWNUM_ENGINEER1         5
#define def_TS_ROWNUM_ENGINEER2         6
#define def_TS_ROWNUM_DATABASE          7
#define def_TS_ROWNUM_REPORT            8
#define def_TS_ROWNUM_TIMEMODE          9
#define def_TS_ROWNUM_FILEFORMAT        10

#define def_TC_ROWNUM_TCID              0
#define def_TC_ROWNUM_TCNAME            1
#define def_TC_ROWNUM_TCEXP             2

#define def_SEND_ROWNUM_MSGCNT          0

#define def_VERIFY_ROWNUM_MSGCNT        0
#define def_VERIFY_ROWNUM_FAILURE       1
#define def_VERIFY_ROWNUM_MSGLIST       2
#define def_VERIFYRES_ROWNUM_MSGLIST    3

#define def_WAIT_ROWNUM_PURPOSE         0
#define def_WAIT_ROWNUM_DELAY           1
#define def_ROWNNUM_WAITFOR             2

#define def_REPLAY_ROWNUM_FILEPATH      0

#define def_SMSG_ROWNUM_SUINT           0
#define def_SMSG_ROWNUM_SDEFVALUE       1
#define def_SMAG_ROWNUM_SVALUE          1

#define def_VMSG_ROWNUM_SUINT          0
#define def_VMSG_ROWNUM_SVALUE         1

#define def_NAME_SEND           "Send"      
#define def_NAME_VERIFY         "verify"
#define def_NAME_VERIFYRESPONSE "verifyResponse"
