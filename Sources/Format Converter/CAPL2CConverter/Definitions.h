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
 * \file      Definitions.h
 * \brief     This file has the definition of all the constants used
 * \author    Mahesh B S
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * This file has the definition of all the constants used
 */

#pragma once

//Converter.cpp
#define defCON_MAX_MSGID_LEN 20 /* maximum length of Message ID */
#define defCON_MAX_MSGN_LEN 32

//Signal.Cpp
#define defCON_MAX_LINE_LEN 1026  /* Maximum length of one line */
#define defCON_MAX_TOKN_LEN 1026  /* Maximum length that any field can be */

//Valuedescriptor.cpp
#define	defVDES_MAX_DESC_LEN 150  /* hopefully nobody puts value descriptor strings */
#define defVDES_MAX_OUT_STR  150  /* I don't expect a name more than 150 bytes */

#define ERR_INPUT_FILE_NOTFOUND          -1
#define ERR_OUTPUT_FILE_NOTFOUND         -2
