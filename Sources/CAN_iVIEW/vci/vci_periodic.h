/*****************************************************************************
 *
 * \file	vci_periodic.h
 * \brief	VCI periodic value definitions for the VCI protocol drivers
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

#ifndef _VCI_PERIODIC_H
#define _VCI_PERIODIC_H

#ifndef _VCI_H
#error "Do not include this file directly, include top level vci.h"
#endif

/* General VCI periodic message transmission implementation */
#define PERIODIC_SZ	32

/**
 * \brief TODO: docs 
 */		
union periodic_msg 
{
	uint8_t	_u8[PERIODIC_SZ];
	uint16_t _u16[(PERIODIC_SZ/sizeof(uint16_t))];
	uint32_t _u32[(PERIODIC_SZ/sizeof(uint32_t))];
} ;

typedef union periodic_msg		periodic_msg_s;

/**
 * \brief Userspace/kernel access of periodic data using ioctl VCI_FIOADDPERIODIC 
 */
struct vci_periodic_record 
{
	uint32_t	id;
	uint32_t	period;
	uint32_t	flags;
	uint32_t	data_length;
	periodic_msg_s	data;
} _PACKED_;

typedef struct vci_periodic_record	vci_periodic_record_s;

#endif


