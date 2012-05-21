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
 * \file      DIL_Interface/DIL_Interface_Include.h
 * \brief     Includes for CDIL_McNet class.
 * \author    Pradeep Kadoor
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Includes for CDIL_McNet class.
 */

#include "DataTypes/McNet_Datatypes.h"
//Typedefs for all the callback functions the application should register to lib
typedef void (*PDILM_CB_N_CONNTESTCONF) (DWORD LCN1, DWORD LCN2, DWORD Result);
typedef void (*PDILM_CB_N_CONNCONF) ( DWORD LOCAL_LC, DWORD REMOTE_LC,
                                      EWD_STATUS eWdStatus, eCON_STATUS eConStatus);
typedef void (*PDILM_CB_N_CONNIND) ( DWORD LOCAL_LC, DWORD REMOTE_LC,
                                     EWD_STATUS eWdStatus, eCON_STATUS eConStatus);
typedef void (*PDILM_CB_A_DATAIND) ( PSTMCNET_MSG pouMsg);
typedef void (*PDILM_CB_A_DATACONF)(DWORD dwLC, DWORD dwRC, char cTransferResult);
typedef void (*PDILM_CB_N_DISCONIND)(DWORD LOCAL_LC, DWORD REMOTE_LC, eREASON Reason);