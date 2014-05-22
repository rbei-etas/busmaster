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
 * \file      CanUsbDefs.h
 * \brief     Includes PEAK CAN USB related macro definition
 * \author    Pradeep Kadoor
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Includes PEAK CAN USB related macro definition
 */
#include "BaseDefs.h"

#define STR_EMPTY               _T("")

#define defBITS_IN_BYTE         8

#define defWARNING_LIMIT_INT    96

#define defBASE_DEC                 10
#define defBASE_HEX                 16

#define defUSB_MODE_ACTIVE                     1
#define defUSB_MODE_PASSIVE                    2
#define defUSB_MODE_SIMULATE                   3

#define defMODE_ACTIVE                         1
#define defMODE_PASSIVE                        2
#define defMODE_SIMULATE                       3

#define defCONTROLLER_ACTIVE                   1
#define defCONTROLLER_PASSIVE                  2
#define defCONTROLLER_BUSOFF                   3

#define defINIT_CODE_VALUE                  0x00
#define defINIT_MASK_VALUE                  0xFF
//
#define defERR_OK                           0
#define defBAUD_RATE_VAL                    500
#define defBAUD_RATE                        0xC03A // 500 KBPS
#define defClockVal                         16
#define defSamplingVal                      1

#define defSTR_WARNING_LIMIT_SET_FAILED     _T("Setting warning limit failed")
