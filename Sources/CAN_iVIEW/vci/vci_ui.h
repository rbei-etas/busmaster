/*****************************************************************************
 *
 * \file	vci_ui.h
 * \brief	VCI User Interface value definitions for the VCI protocol
 * 		drivers IOCTL interface for the T-MVCI
 *
 * Copyright (C) 2013 Bosch Automotive Service Solutions
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 *****************************************************************************/

#ifndef _VCI_UI_H
#define _VCI_UI_H

#ifndef _VCI_H
#error "Do not include this file directly, include top level vci.h"
#endif


/**
 * \brief VCI Battery Mode Current types
 *
 */
enum vci_battery_mode_current_types
{
	VCI_BAT_CURRENT_MA,
	VCI_BAT_CURRENT_MAH,
	VCI_BATTERY_CURRENT_TYPE_MAX
};

enum VCI_UI_EVENTS
{
	VCI_UI_EVENT_TRIGGER = VCI_START_xEVENT_INDEX,
	VCI_UI_EVENT_KEYPRESS
};

/**
 * \brief VCI Beeb Configuration ioctl structure
 *
 */
struct vci_beep_config
{
	uint32_t duration ;
	uint32_t frequency ;
	uint32_t volume ;
} _PACKED_ ;

typedef struct vci_beep_config	vci_beep_config_s;

/**
 * \brief beep configuation record structure
 */
struct vci_beep_config_record
{
	vci_config_record_s	config_record;
	vci_beep_config_s	data;
};

typedef struct vci_beep_config_record	vci_beep_config_record_s;

#define VCI_BEEP_CONFIG_RECORD_SZ	sizeof( vci_beep_config_record_s )
#define VCI_BEEP_CONFIG_DATA_SZ		sizeof( vci_beep_config_s )

#define vci_beep_config_request_record_constructor( \
		_vci_rec, _user_data, \
		_duration, _frequency, _volume  ) \
{ \
	vci_beep_config_s* _vci_beep=\
		&((vci_beep_config_record_s*)_vci_rec)->data;\
	vci_config_request_record_constructor( _vci_rec, VCI_FIOBEEPCONFIG, \
			_user_data, VCI_BEEP_CONFIG_DATA_SZ ); \
	_vci_beep->duration = htonl( _duration ); \
	_vci_beep->frequency = htonl( _frequency ); \
	_vci_beep->volume = htonl( _volume ); \
}

#define VCI_LED_PERIODS	(BITS_PER_BYTE * sizeof(u32))

/**
 * \brief VCI LED state types
 *
 */
enum vci_led_types
{
	VCI_LED_POWER,
	VCI_LED_HOST_LINK,
	VCI_LED_BATTERY,
	VCI_LED_VEHICLE_LINK,
	VCI_LED_INFO,
	VCI_LED_ERROR,
	VCI_LED_MAX
};

#define VCI_LED_TRIGGER	VCI_LED_INFO

/**
 * \brief VCI LED state types
 *
 */
enum vci_led_state_types
{
	VCI_LED_STATE_OFF,
	VCI_LED_STATE_ON,
	VCI_LED_STATE_FLASH,
	VCI_LED_STATE_BLINK,
	VCI_LED_STATE_ANTI_BLINK,
	VCI_LED_STATE_DOUBLE_FLASH,
	VCI_LED_STATE_LONG_ON,
	VCI_LED_STATE_LONG_OFF,
	VCI_LED_STATE_ONE_SECOND_FLASH,
	VCI_LED_STATE_USER_DEFINE,
	VCI_LED_STATE_TYPE_MAX
};

/**
 * \brief VCI led Configuration ioctl structure
 *
 */
struct vci_led_config
{
	uint32_t led ;
	uint32_t mode ;	// See enum vci_led_state_types
	uint32_t arg ;	// Option argument
} _PACKED_ ;

typedef struct vci_led_config	vci_led_config_s;

/**
 * \brief led configuation record structure
 */
struct vci_led_config_record
{
	vci_config_record_s	config_record;
	vci_led_config_s	data;
};

typedef struct vci_led_config_record	vci_led_config_record_s;

#define VCI_LED_CONFIG_RECORD_SZ	sizeof( vci_led_config_record_s )
#define VCI_LED_CONFIG_DATA_SZ		sizeof( vci_led_config_s )

#define vci_led_config_constructor( \
		_vci_rec, _led, _mode, _arg ) \
{ \
	vci_led_config_s* _vci_led=(vci_led_config_s*)_vci_rec;\
	_vci_led->led = htonl( _led ); \
	_vci_led->mode = htonl( _mode ); \
	_vci_led->arg = htonl( _arg ); \
}

#define vci_led_config_request_record_constructor( \
		_vci_rec, _user_data, \
		_led, _mode, _arg ) \
{ \
	vci_led_config_s* _led_cfg=&((vci_led_config_record_s*)_vci_rec)->data;\
	vci_config_request_record_constructor( _vci_rec, VCI_FIOLEDCONFIG, \
			_user_data, VCI_LED_CONFIG_DATA_SZ ); \
	vci_led_config_constructor( _led_cfg, _led, _mode, _arg ); \
}

/**
 * \brief VCI Event Commands
 *
 *	VCI_EVENT_DISABLE
 *		Generic event disable
 *	VCI_EVENT_ENABLE
 *		Generic event enable
 *	VCI_EVENT_ARM_TRIGGER
 *		Specific command to arm trigger cable for button press.
 *	VCI_EVENT_ARM_
 *
 */
enum event_command_e
{
	/*
	 * Generic
	 */
	VCI_UI_EVENT_DISABLE,
	VCI_UI_EVENT_ENABLE,
};

/**
 * \brief VCI UI Event Configuration ioctl structure
 *
 *	type
 *		The event type, according to the VCI_UI_EVENTS enum (above),
 *		coded as a 32-bit value
 *	command
 *		The event command, according to the event_command_e enum,
 *		coded as a 32-bit value
 *	arg
 *		The command specific argument.
 *		VCI_UI_EVENT_ENABLE:
 *			The user-defined unique event handle that will be
 *			returned in the asynchronous VCI_EVENT messages that
 *			are generated.
 *		VCI_UI_EVENT_TRIGGER_LED:
 *			0 Led Off. 1 Led On.
 *
 */
struct vci_ui_event_config
{
	uint32_t	type;	/* event type - VCI_EVENTS enum (vci.h) */
	uint32_t	command;/* event_command_e */
	uint32_t	arg;	/* command specific argument */
} _PACKED_ ;

typedef struct vci_ui_event_config	vci_ui_event_config_s;

/**
 * \brief Event configuration record structure
 */
struct vci_ui_event_config_record
{
	vci_config_record_s	config_record;
	vci_ui_event_config_s	data;
};

typedef struct vci_ui_event_config_record	vci_ui_event_config_record_s;

#define VCI_UI_EVENT_CONFIG_RECORD_SZ		sizeof( vci_ui_event_config_record_s )
#define VCI_UI_EVENT_CONFIG_DATA_SZ		sizeof( vci_ui_event_config_s )

#define vci_ui_event_config_constructor( \
		_vci_rec, _type, _command, _arg ) \
{ \
	vci_ui_event_config_s* _vci_ui_event_config= \
		(vci_ui_event_config_s*)_vci_rec;\
	_vci_ui_event_config->type = htonl( _type ); \
	_vci_ui_event_config->command = htonl( _command ); \
	_vci_ui_event_config->arg = htonl( _arg ); \
}

#define vci_ui_event_config_request_record_constructor( \
		_vci_rec, _user_data, \
		_type, _command, _arg ) \
{ \
	vci_ui_event_config_s* _ui_event_cfg= \
		&((vci_ui_event_config_record_s*)_vci_rec)->data;\
	vci_config_request_record_constructor( _vci_rec, VCI_FIO_UI_EVENT_CONFIG, \
		_user_data, VCI_UI_EVENT_CONFIG_DATA_SZ ); \
	vci_ui_event_config_constructor( _ui_event_cfg, _type,\
		_command, _arg );\
}

/**
 * /brief: vci_ui_event_record
 *
 * vci ui event record definition and helper macros
 */
struct vci_ui_event_record
{
	vci_event_record_s	event ;
	uint32_t		handle; /* echoed from vci_event_config */
#ifndef _WIN32
  u_char data[];	/* VCI event payload.  Variable length record for future
			  use, where additional data is required. */
#endif
} _PACKED_  ;

typedef struct vci_ui_event_record	vci_ui_event_record_s;

#define VCI_UI_EVENT_RECORD_SZ	sizeof( vci_ui_event_record_s )
#define VCI_UI_EVENT_DATA_SZ	(sizeof( vci_ui_event_record_s ) -\
				 sizeof( vci_ui_record_s ))

#define vci_ui_event_record_constructor( \
		_vci_rec, _user_data, _type, _handle, _payload, _payload_ptr) \
{ \
	vci_ui_event_record_s* _vci_ui_event_record =\
		(vci_ui_event_record_s*)_vci_rec; \
	vci_event_record_constructor( _vci_rec, _type, \
		_user_data, VCI_ERROR_EVENT_DATA_SZ + _payload); \
	_vci_ui_event_record->handle = htonl( _handle ); \
	if( _payload_ptr )\
		memcpy(_vci_ui_event_record->data, _payload_ptr, _payload );\
}

#endif
