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
 * \file      ContrConfigPeakUsbDefs.h
 * \brief     Macro definitions for controller configuration.
 * \author    Pradeep Kadoor
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Macro definitions for controller configuration.
 */



#define defBITS_IN_BYTE         8

#define defCLOCK            _T("16")
#ifdef EVAL_COPY_EXT 
#define defCONTROLLER_MODE  3 // Simulation mode
#else 
#define defCONTROLLER_MODE  1 // Active mode
# endif

#define defBASE_DEC                 10

#define defSTR_CHANNEL_NAME                _T("Channel")
#define defSTR_CHANNEL_NAME_FORMAT         _T("%s %d")

#define defSTR_BTR0_COL_HEADING             _T("BTR0")
#define defSTR_BTR1_COL_HEADING             _T("BTR1")

#define defSTR_SAMPLE_POINT_COL_HEADING     _T("Sampling Point")
#define defSTR_NBT_COL_HEADING              _T("NBT")
#define defSTR_SJW_COL_HEADING              _T("SJW")
#define defSTR_CNF1_COL_HEADING             _T("CNF1")
#define defSTR_CNF2_COL_HEADING             _T("CNF2")
#define defSTR_CNF3_COL_HEADING             _T("CNF3")
#define defSTR_BRP_COL_HEADING              _T("BRP")

#define defCHANNEL_ICON_SIZE                24
#define defCHANNEL_LIST_INIT_SIZE           5
#define defCHANNEL_LIST_GROW_SIZE           1

#define defSTR_MSG_ID_STANDARD              _T("Standard")
#define defSTR_MSG_ID_EXTENDED              _T("Extended")