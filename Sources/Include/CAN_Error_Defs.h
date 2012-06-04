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
 * \file      CAN_Error_Defs.h
 * \author    Pemmaiah BD
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 */

#pragma once

//Error Val Interpretation
#define ERROR_BUS                       0x01
#define ERROR_DEVICE_BUFF_OVERFLOW      0x02
#define ERROR_DRIVER_BUFF_OVERFLOW      0x03
#define ERROR_WARNING_LIMIT_REACHED 	0x04
#define ERROR_APPLICATION_BUFF_OVERFLOW 0x05
#define ERROR_TX_TIMEOUT                0x06
#define ERROR_INTERRUPT                 0x07
#define ERROR_DLLMSG_BUFF_OVERFLOW      0x08

#define ERROR_UNKNOWN                   0xFF


#define BIT_ERROR_TX                       0x00
#define FORM_ERROR_TX                      0x40
#define STUFF_ERROR_TX                     0x80
#define OTHER_ERROR_TX                     0xC0
#define BIT_ERROR_RX                       0x20
#define FORM_ERROR_RX                      0x60
#define STUFF_ERROR_RX                     0xA0
#define OTHER_ERROR_RX                     0xE0


#define DIL_OK                          0x0000
#define DIL_FAIL                        0xFFFF

//These codes are returned by DIL_nMsgWrite func
#define DIL_ERR_WRITE_SYS               0xF001
#define DIL_ERR_DEVICE_BUSY				0xF002
//#define DIL_ERR_READ_BYTE_MISMATCH      0xF002


//These error codes are returned by DIL_nMsgRead func
#define DIL_ERR_READ_SYS                0xFFF0
#define DIL_ERR_READ_BYTE_MISMATCH      0xFFF1
