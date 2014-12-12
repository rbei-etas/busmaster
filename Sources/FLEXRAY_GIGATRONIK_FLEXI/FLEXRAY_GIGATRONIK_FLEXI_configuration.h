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

/**********************************************************************************************************************
*
*  FILENAME:           FLEXRAY_GIGATRONIK_FLEXI_configuration.h
*
*  COPYRIGHT:          Gigatronik Ingolstadt GmbH
*  AUTHOR, DPT:        M. Morgott, KE
*
*  DATE OF CREATION:   09.12.2014
*
*  DESCRIPTION:        Defines relevant flags and configurations.
*
*********************************************************************************************************************/

/** --- CHANGE LOG ---
 *  V1.0.0.0        First reviewed version
 *
 */

#pragma once

#include "FLEXRAY_GIGATRONIK_FLEXI_GtFr_NativeInterface.h"


/**
* Defines the maximum number of message buffer of the E-Ray Controller.
*/
#define GTFR_ERAY_MAX_MSGBUF_COUNT 128u

/**
* Defines the highest number of frame ID.
* For FlexRay in general, the range is from 1 to 1024.
*/
#define GTFR_MAX_ALLOWED_FRAME_ID 1024u

/**
* Defines the threshold, responsible for calling the event interpreter function 'callbackFct()'
*/
#define EVENTQUEUE_THRESHOLD 3

/* typedefinitions of flex-i API relevant types */
#define MSG_BUF_IDX_TYPE uint32
#define MSG_REPETITION_TYPE uint32

/* Definition of available controller index */
#define GTFR_COMMUNICATION_CONTROLLER_0          0
#define GTFR_COMMUNICATION_CONTROLLER_1          1
#define GTFR_CTRL_COUNT                             2

/* Definition of flex-i API relevant flags */
#define GTFR_ENABLED                             1u
#define GTFR_DISABLED                            0u

/* Definitions of FlexRay message header indicators */
#define GTFR_HEADERFLAG_STARTUP                    0x01
#define GTFR_HEADERFLAG_SYNC                    0x02
#define GTFR_HEADERFLAG_NULL                    0x04
#define GTFR_HEADERFLAG_PAYLOAD_PREAMBLE        0x08

/* Defines the number of possible messages for one slot ID */
#define GTFR_NUMBER_SLOTS                        64




/* fifo configurations */
/* CC0 fifo configurations is taken from the CLI demo */
#define GTFR_FIFO_CC0_CRITICAL_LEVEL    4u
#define GTFR_FIFO_CC0_DEPTH                8u
/* CC1 do not use FIFO, therefore it is only important to set the depth to 0 to deactivate fifo */
#define GTFR_FIFO_CC1_CRITICAL_LEVEL    4u
#define GTFR_FIFO_CC1_DEPTH                0u
