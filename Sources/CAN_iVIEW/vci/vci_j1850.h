/*****************************************************************************
 *
 * \file	vci_j1850.h
 * \brief	VCI J1850 value definitions for the VCI protocol drivers
 * 		IOCTL interface for the T-MVCI
 *
 * Copyright (C) 2013 Bosch Automotive Service Solutions
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 *****************************************************************************/

#ifndef _VCI_J1850_H
#define _VCI_J1850_H

/* J1850 RX VCI protocol specific flags */
#define VCI_J1850VPW_ARB_LOST		0x00000100	/* Transmission was successful, but arb was lost */
#define VCI_J1850PWM_ARB_LOST		0x00000100	/* Transmission was successful, but arb was lost */

/* J1850 controller events */
enum VCI_J1850_EVENTS
{
	VCI_J1850_SUCCESS		= 0,				/* Not reported as an event */
	VCI_J1850_RX_OVERRUN		= VCI_START_xEVENT_INDEX,
	VCI_J1850_CRC_ERR,						/* CRC error on Bus */
	VCI_J1850_BYTE_ERR,		/* Currently unused */		/* Incomplete byte received */
	VCI_J1850_BIT_ERR,		/* Currently unused */		/* Bit timing error */
	VCI_J1850_RX_BREAK,						/* RX Break detected on bus */
	VCI_J1850_TX_NOT_INV,		/* Currently unused */		/* Transmitter not involved */
	VCI_J1850_TX_UNDERRUN,		/* Currently unused */		/* Transmitter underrun */
	VCI_J1850_TX_ARB_LOST,						/* Transmitter lost arbitration */
	VCI_J1850_SC_GND,		/* Currently unused */		/* Short to ground */
	VCI_J1850_MODE_1X,		/* Currently unused */		/* 1* speed mode event */
	VCI_J1850_MODE_4X,		/* Currently unused */		/* 4* speed mode event */
	VCI_J1850_TX_BREAK,						/* User sent TX break */
	VCI_J1850_RX_ERR,						/* Rx Timing error detected on bus */
	VCI_J1850_IFR,							/* In-frame response received */
	VCI_J1850_IFR_CRC,		/* Currently unused */
	VCI_J1850_TX_TOUT_ERR,		
	/* */
	VCI_J1850_EVENT_MAX
} ;
#endif
