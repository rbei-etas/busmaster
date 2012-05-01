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
 * \file      Macros.h
 * \brief     Definition file for macros
 * \author    Anish Kumar
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Definition file for macros
 */

Author(s)     :  Anish Kumar

#pragma once

#define Def_TPCI_SEQ_NO_MASK              0x0F
#define Def_TPCI_BLK_SIZE_MASK            0x0F
#define Def_TPCI_MSG_COM_MASK             0x40
#define Def_TPCI_DATA_TYPE_MASK           0x40
#define Def_TPCI_MSG_EOM_MASK             0x10
#define Def_TPCI_ACK_REQ_MASK             0x20
#define Def_TPCI_RX_STATUS_MASK           0x20
#define Def_TPCI_DATA_MSG_MASK            0x80
#define Def_TPCI_BROADCAST_VAL            0x30
#define Def_TPCI_DAT_ACK_MASK             0x90
#define Def_TPCI_WATCH_DOG_MASK           0xF0
#define Def_TPCI_WATCH_DOG_VAL            0x80
#define Def_TPCI_WD_DEFAULT_TIME          0x0A  //eq. to 2pow10 = 1024 ms
#define Def_TPCI_RXR_MASK                 0x20


#define defCS_STD_FIRST_BYTE_VAL               0xA0
#define defCS_ENHANCE_FIRST_BYTE_VAL           0xE0
#define defCA_STD_FIRST_BYTE_VAL               0xA1
#define defCA_ENHANCE_FIRST_BYTE_VAL           0xE1
#define defALL_MSG_CT_FIRST_BYTE_VAL           0xA3


#define GET_SEQ_NUMBER(byTPCI)((byTPCI) & Def_TPCI_SEQ_NO_MASK)
#define GET_BLOCKSIZE(byTPCI)((byTPCI) & Def_TPCI_BLK_SIZE_MASK)
#define IS_COM_ENHANCE_MODE(byTPCI)((byTPCI) & Def_TPCI_MSG_COM_MASK)
#define IS_MSG_END_TPDU(byTPCI)((byTPCI) & Def_TPCI_MSG_EOM_MASK)
#define IS_ACK_REQUESTED(byTPCI)(!((byTPCI) & Def_TPCI_ACK_REQ_MASK))
#define IS_RX_NODE_READY(byTPCI)((byTPCI) & Def_TPCI_RX_STATUS_MASK)
#define IS_FRAME_DATA_MSG(byTPCI)(((!((byTPCI) & Def_TPCI_DATA_MSG_MASK)) && \
                                   (byTPCI != Def_TPCI_BROADCAST_VAL)))
#define IS_FRAME_EXP_MSG(byTPCI)((byTPCI) & Def_TPCI_DATA_TYPE_MASK)
#define IS_BROADCAST_MSG(byTPCI)(((byTPCI) == Def_TPCI_BROADCAST_VAL ? true : false))
#define IS_DATA_ACK_MSG(byTPCI)((((byTPCI) & Def_TPCI_DAT_ACK_MASK) == \
                                 Def_TPCI_DAT_ACK_MASK) ? true : false)
#define IS_WATCH_DOG_MSG(byTPCI)((((byTPCI) & Def_TPCI_WATCH_DOG_MASK) == \
                                  Def_TPCI_WATCH_DOG_VAL ? true : false))

#define CLEAR_SEQ_BITS_TPCI(byTPCI)((byTPCI) &= (~Def_TPCI_SEQ_NO_MASK))

#define MAKE_EXP_TPCI(byTPCI)(byTPCI = ((Def_TPCI_DATA_TYPE_MASK | Def_TPCI_MSG_EOM_MASK)\
                                        & (~Def_TPCI_ACK_REQ_MASK )))//Seq no. will be 0
#define MAKE_BROADCAST_TPCI(byTPCI)((byTPCI) = Def_TPCI_BROADCAST_VAL)
#define MAKE_WD_TPCI(byTPCI)((byTPCI) = (Def_TPCI_WD_DEFAULT_TIME | Def_TPCI_WATCH_DOG_VAL))
#define MAKE_LONG_MSG_TPCI(byTPCI)((byTPCI) &= (~Def_TPCI_DATA_TYPE_MASK) )
#define MAKE_ACK_REQ_TPCI(byTPCI)((byTPCI) &= (~Def_TPCI_ACK_REQ_MASK) )
#define MAKE_ACK_REQ_LOW(byTPCI)(byTPCI |= Def_TPCI_ACK_REQ_MASK)
#define MAKE_ACK_TPCI(byTPCI)((byTPCI) |= Def_TPCI_DAT_ACK_MASK )
#define MAKE_EOM_BIT_UP_TPCI(byTPCI)((byTPCI) |= Def_TPCI_MSG_EOM_MASK )
#define MAKE_SERVICE_LONG_TPCI(byTPCI)((byTPCI) &= (~Def_TPCI_DATA_TYPE_MASK) )
#define MAKE_SERVICE_EXP_TPCI(byTPCI)((byTPCI) |= Def_TPCI_DATA_TYPE_MASK )
#define MAKE_RXR(byTPCI)((byTPCI) |= Def_TPCI_RXR_MASK )
#define MAKE_RXNR(byTPCI)((byTPCI) &= (~Def_TPCI_RXR_MASK))
