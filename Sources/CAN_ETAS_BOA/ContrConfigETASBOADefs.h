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

#pragma once

#define defBITS_IN_BYTE         8

#define defCLOCK            "16"
#ifdef EVAL_COPY_EXT
#define defCONTROLLER_MODE  3 // Simulation mode
#else
#define defCONTROLLER_MODE  1 // Active mode
# endif

#define defBASE_DEC                 10

#define defSTR_CHANNEL_NAME                "Channel"
#define defSTR_CHANNEL_NAME_FORMAT         "%s %d"

#define defSTR_BTR0_COL_HEADING             "BTR0"
#define defSTR_BTR1_COL_HEADING             "BTR1"

#define defSTR_SAMPLE_POINT_COL_HEADING     "Sampling Point"
#define defSTR_NBT_COL_HEADING              "NBT"
#define defSTR_SJW_COL_HEADING              "SJW"
#define defSTR_CNF1_COL_HEADING             "CNF1"
#define defSTR_CNF2_COL_HEADING             "CNF2"
#define defSTR_CNF3_COL_HEADING             "CNF3"
#define defSTR_BRP_COL_HEADING              "BRP"

#define defCHANNEL_ICON_SIZE                24
#define defCHANNEL_LIST_INIT_SIZE           5
#define defCHANNEL_LIST_GROW_SIZE           1

#define defSTR_MSG_ID_STANDARD              "Standard"
#define defSTR_MSG_ID_EXTENDED              "Extended"

/* CAN FD parameter definitions */

/* tx compatibility flags */
#define defSTR_CANFD_TX_USE_NBR             "CANFD_TX_USE_NBR"
#define defSTR_CANFD_TX_USE_DBR             "CANFD_TX_USE_DBR"

/* rx compatibility flags */
#define defSTR_CANFD_RX_FILTER_NBR_ONLY             "CANFD_RX_FILTER_NBR_ONLY"
#define defSTR_CANFD_RX_FILTER_DBR8                 "CANFD_RX_FILTER_DBR8"
#define defSTR_CANFD_RX_USE_RXFD_MESSAGE            "CANFD_RX_USE_RXFD_MESSAGE"
#define defSTR_CANFD_RX_USE_RXFD_MESSAGE_PADDING    "CANFD_RX_USE_RXFD_MESSAGE_PADDING"

/* tx Delay compensation */
#define defSTR_CANFD_TX_DELAY_COMPENSATION_ON       "ON"
#define defSTR_CANFD_TX_DELAY_COMPENSATION_OFF      "OFF"

#define CANFD_COUNT_TX_COMPATIBILITY_MODES              2
#define CANFD_COUNT_RX_COMPATIBILITY_MODES              4


