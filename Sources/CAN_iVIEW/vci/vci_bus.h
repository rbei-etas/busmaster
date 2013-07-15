/*****************************************************************************
 *
 * \file	vci_bus.h
 * \brief	VCI Bus connection definitions for the VCI protocol drivers
 * 		IOCTL interface for the T-MVCI
 *
 * Copyright (C) 2013 Bosch Automotive Service Solutions
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 *
 *****************************************************************************/

#ifndef _VCI_BUS_H
#define _VCI_BUS_H

#ifndef _VCI_H
#error "Do not include this file directly, include top level vci.h"
#endif

enum protocols_e	/* protocols */
{
	/* None */
	VCI_PROTO_NONE,
	/* All raw */
	VCI_PROTO_RAW,
	/* CAN */
	VCI_PROTO_ISO15765,
	VCI_PROTO_J1939,
	/* UART */
	VCI_PROTO_ISO9141,
	VCI_PROTO_ISO14230,
	VCI_PROTO_SCI,
	VCI_PROTO_DIAG_H,
	VCI_PROTO_KWP81,
	VCI_PROTO_KWP82,
	VCI_PROTO_HONDA_H9x,
	/* J1850 */
	VCI_PROTO_J1850_VPW,
	VCI_PROTO_J1850_PWM,
	/* ADC - pin voltage, cable ident */
	VCI_PROTO_ADC,
	/* J1708 */
	VCI_PROTO_J1708,
	/* UART EX */
	VCI_PROTO_KWP1271,
	VCI_PROTO_JED677,
	/* max */
	VCI_PROTO_MAX
} ;

/* Protocol synonyms */

#define VCI_PROTO_KWP2000	VCI_PROTO_ISO14230
#define VCI_PROTO_HONDA_H99	VCI_PROTO_HONDA_H9x
#define VCI_PROTO_HONDA_H99B	VCI_PROTO_HONDA_H9x
#define VCI_PROTO_HONDA_95Hm	VCI_PROTO_HONDA_H9x

enum physical_layer_e	/* physical layers */
{
	/* None */
	VCI_PHYS_NONE,
	/* Loopback test */
	VCI_PHYS_TEST,
	/* CAN */
	VCI_PHYS_ISO11898_2,
	VCI_PHYS_ISO11898_3,
	VCI_PHYS_GMW_3089,
	/* UART */
	VCI_PHYS_ISO9141,
	VCI_PHYS_DC_SCI,
	VCI_PHYS_FIAT_ISO,
	VCI_PHYS_ISUZU_TTL,
	VCI_PHYS_SUZUKI_SDL,
	VCI_PHYS_GM_XDE5024,
	VCI_PHYS_GM_XDE5024C,
	VCI_PHYS_DIAG_H,
	/* J1850 */
	VCI_PHYS_J1850_VPW,
	VCI_PHYS_J1850_PWM,
	/* VPROG */
	VCI_PHYS_VPROG,
	/* ADC */
	VCI_PHYS_SYSTEM,
	/* Honda SCS */
	VCI_PHYS_SCS,
	/* Honda WEN */
	VCI_PHYS_WEN,
	/* J1708 */
	VCI_PHYS_J1708,
	/* max */
	VCI_PHYS_MAX
} ;

/* Physical layer synonyms */

#define VCI_PHYS_HSCAN		VCI_PHYS_ISO11898_2
#define VCI_PHYS_FTCAN		VCI_PHYS_ISO11898_3
#define VCI_PHYS_SWCAN		VCI_PHYS_GMW_3089
#define VCI_PHYS_ADC		VCI_PHYS_SYSTEM

enum phys_mode		/* physical layer mode */
{
	VCI_PHYS_OFF,
	VCI_PHYS_ON,
	VCI_PHYS_SLEEP,
	VCI_PHYS_WAKE_UP,
	VCI_PHYS_HIGH_SPEED,
	/* max */
	VCI_PHYS_MODE_MAX
} ;

/* Physical layer mode synonyms */

#define VCI_PHYS_NORMAL		VCI_PHYS_ON
#define VCI_PHYS_HIGH_VOLTAGE	VCI_PHYS_WAKE_UP

typedef enum phys_mode		phys_mode_e ;

/* VCI Termination Values */

typedef enum VCI_Term_e {
	VCI_TERM_NONE = 0,
	VCI_CAN_TERM_120OHM = 120
} VCI_Term_t;

#if 0
enum controller_mode		/* controller mode */
{
	VCI_DEV_OFF,
	VCI_DEV_ON,
	VCI_DEV_SLEEP,
	VCI_DEV_WAKE_UP,
	VCI_DEV_LISTEN_ONLY,
	VCI_DEV_LOOPBACK,
	/* max */
	VCI_DEV_MODE_MAX
} ;

typedef enum controller_mode	controller_mode_e ;
#endif

enum connectors_e	/* connectors */
{
	VCI_CONN_J1962,				/* SAE J1962 */
	VCI_CONN_LOOPBACK,			/* VCI DLC 26-way D-type loop-back */

	/* ...add new connectors here... */

	/* max */
	VCI_CONN_MAX
} ;

enum vci_pin_state
{
	VCI_PIN_STATE_LOW,
	VCI_PIN_STATE_HIGH,
	VCI_PIN_STATE_NORMAL,			/* 'normal' pin function e.g. UART */
	VCI_PIN_STATE_INVALID
} ;

typedef enum vci_pin_state		vci_pin_state_e ;

enum vci_can_bus_status_int_events
{
  VCI_CAN_BUS_DISABLE_ALL_STATUS_INT=0,
  VCI_CAN_BUS_TX_STATUS_INT_DISABLE,
  VCI_CAN_BUS_TX_BUSOFF_INT_ENABLE,
  VCI_CAN_BUS_TX_BUSOFF_TXERR_INT_ENABLE,
  VCI_CAN_BUS_TX_ENABLE_ALL_STATUS_INT,
  VCI_CAN_BUS_RX_STATUS_INT_DISABLE = 0x10,
  VCI_CAN_BUS_RX_BUSOFF_INT_ENABLE=0x20,
  VCI_CAN_BUS_RX_BUSOFF_TXERR_INT_ENABLE=0x30,
  VCI_CAN_BUS_RX_ENABLE_ALL_STATUS_INT=0x40

};

enum vci_dlc_pins_e	/* VCI DLC D-SUB 26 pins */
{
	VCI_DLC_PIN_NONE,
	VCI_DLC_PIN_01,
	VCI_DLC_PIN_02,
	VCI_DLC_PIN_03,
	VCI_DLC_PIN_04,
	VCI_DLC_PIN_05,
	VCI_DLC_PIN_06,
	VCI_DLC_PIN_07,
	VCI_DLC_PIN_08,
	VCI_DLC_PIN_09,
	VCI_DLC_PIN_10,
	VCI_DLC_PIN_11,
	VCI_DLC_PIN_12,
	VCI_DLC_PIN_13,
	VCI_DLC_PIN_14,
	VCI_DLC_PIN_15,
	VCI_DLC_PIN_16,
	VCI_DLC_PIN_17,
	VCI_DLC_PIN_18,
	VCI_DLC_PIN_19,
	VCI_DLC_PIN_20,
	VCI_DLC_PIN_21,
	VCI_DLC_PIN_22,
	VCI_DLC_PIN_23,
	VCI_DLC_PIN_24,
	VCI_DLC_PIN_25,
	VCI_DLC_PIN_26,
	VCI_DLC_PIN_4MM,
	/* max */
	VCI_DLC_PIN_MAX
} ;

enum j1962_pins_e	/* J1962 pins */
{
	VCI_J1962_PIN_NONE,
	VCI_J1962_PIN_01,
	VCI_J1962_PIN_02,
	VCI_J1962_PIN_03,
	VCI_J1962_PIN_04,
	VCI_J1962_PIN_05,
	VCI_J1962_PIN_06,
	VCI_J1962_CAN_L = VCI_J1962_PIN_06,
	VCI_J1962_PIN_07,
	VCI_J1962_PIN_08,
	VCI_J1962_PIN_09,
	VCI_J1962_PIN_10,
	VCI_J1962_PIN_11,
	VCI_J1962_PIN_12,
	VCI_J1962_PIN_13,
	VCI_J1962_PIN_14,
	VCI_J1962_CAN_H = VCI_J1962_PIN_14,
	VCI_J1962_PIN_15,
	VCI_J1962_PIN_16,
	/* max */
	VCI_J1962_PIN_MAX
} ;

enum vci_loopback_pins_e	/* VCI loop-back pins */
{
	VCI_LOOPBACK_PIN_NONE,
	VCI_LOOPBACK_PIN_01,
	VCI_LOOPBACK_PIN_02,
	VCI_LOOPBACK_PIN_03,
	VCI_LOOPBACK_PIN_04,
	VCI_LOOPBACK_PIN_05,
	VCI_LOOPBACK_PIN_06,
	VCI_LOOPBACK_CAN_L = VCI_LOOPBACK_PIN_06,
	VCI_LOOPBACK_PIN_07,
	VCI_LOOPBACK_PIN_08,
	VCI_LOOPBACK_PIN_09,
	VCI_LOOPBACK_PIN_10,
	VCI_LOOPBACK_PIN_11,
	VCI_LOOPBACK_PIN_12,
	VCI_LOOPBACK_PIN_13,
	VCI_LOOPBACK_PIN_14,
	VCI_LOOPBACK_CAN_H = VCI_LOOPBACK_PIN_14,
	VCI_LOOPBACK_PIN_15,
	VCI_LOOPBACK_PIN_16,
	VCI_LOOPBACK_PIN_17,
	VCI_LOOPBACK_PIN_18,
	VCI_LOOPBACK_PIN_19,
	VCI_LOOPBACK_PIN_20,
	VCI_LOOPBACK_PIN_21,
	VCI_LOOPBACK_PIN_22,
	VCI_LOOPBACK_PIN_23,
	VCI_LOOPBACK_PIN_24,
	VCI_LOOPBACK_PIN_25,
	VCI_LOOPBACK_PIN_26,
	/* max */
	VCI_LOOPBACK_PIN_MAX
} ;

/**
 * \brief Connect ioctl structure
 *
 * TODO: Documentation for this vital configuration request record
 */
struct vci_bus
{
	uint32_t	protocol ;		/**< \brief TODO: docs */
	uint32_t	physical_layer ;	/**< \brief TODO: docs */
	uint32_t	connector ;		/**< \brief TODO: docs */
	uint32_t	primary_pin ;		/**< \brief TODO: docs */
	uint32_t	secondary_pin ;		/**< \brief TODO: docs */
	uint32_t	termination ; 		/**< \brief TODO: docs */
	uint32_t	baudrate ;		/**< \brief TODO: docs */
	uint32_t	flags ;			/**< \brief TODO: docs */
} _PACKED_ ;

typedef struct vci_bus	vci_bus_s;

#define VCI_USE_CHANNEL_MASK    (0x00000003)
#define VCI_USE_CHANNEL_0	(0x00000000)
#define VCI_USE_CHANNEL_1	(0x00000001)
#define VCI_USE_CHANNEL_2	(0x00000002)
#define VCI_USE_CHANNEL_3	(0x00000003)

#define VCI_NO_PHYS_CONNECT	(0x00000004)

/**
 * \brief Connect vci config record structure
 */
struct vci_bus_config_record
{
	vci_config_record_s	config_record;	/**< \brief TODO: docs */
	vci_bus_s		data;		/**< \brief TODO: docs */
};

typedef struct vci_bus_config_record	vci_bus_config_record_s;

#define VCI_BUS_CONFIG_RECORD_SZ	sizeof( vci_bus_config_record_s )
#define VCI_BUS_CONFIG_DATA_SZ		sizeof( vci_bus_s )

#define vci_bus_config_record_constructor( \
		_vci_rec, _user_data, \
		_protocol, _physical_layer, _connector, _primary_pin, \
		_secondary_pin, _termination, _baudrate, _flags ) \
{ \
	vci_bus_s* _vci_bus = &((vci_bus_config_record_s*)_vci_rec)->data; \
	vci_config_request_record_constructor( (vci_config_record_s*)_vci_rec,\
		VCI_FIOCONNECT, _user_data, VCI_BUS_CONFIG_DATA_SZ ); \
	_vci_bus->protocol = htonl( _protocol ); \
	_vci_bus->physical_layer = htonl( _physical_layer ); \
	_vci_bus->connector = htonl( _connector ); \
	_vci_bus->primary_pin = htonl( _primary_pin ); \
	_vci_bus->secondary_pin = htonl( _secondary_pin ); \
	_vci_bus->termination = htonl( _termination ); \
	_vci_bus->baudrate = htonl( _baudrate ); \
	_vci_bus->flags = htonl( _flags ); \
}

#endif
