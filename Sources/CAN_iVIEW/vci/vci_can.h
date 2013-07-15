/*****************************************************************************
 *
 * \file	vci_can.h
 * \brief	VCI CAN definitions for the VCI protocol drivers
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

#ifndef _VCI_CAN_H
#define _VCI_CAN_H

#define VCI_MAX_CAN_SZ		8
#define VCI_CAN_ADDR_SZ		4

/**
 * \brief CAN data format
 *
 * Defines the CAN data format 
 */
struct can_record
{
	uint32_t		address ; /**< \brief TODO: docs */
	uint8_t 		data[] ; /**< \brief TODO: docs */
} _PACKED_  ;

typedef struct can_record	can_record_s;

/**
 * \brief CAN data record
 *
 * Defines the VCI CAN data record format 
 */
struct vci_can_data_record
{
	vci_data_record_s	data_record ; /**< \brief TODO: docs */
	uint32_t		address ;     /**< \brief TODO: docs */
	uint8_t 		data[] ;      /**< \brief TODO: docs */
} _PACKED_  ;

typedef struct vci_can_data_record	vci_can_data_record_s;

/**
 * \brief CAN_EX data format
 *
 * Defines the CAN_EX data format 
 */
struct can_ext_record
{
	uint32_t		address ; /**< \brief TODO: docs */
	uint8_t			ext_address ; /**< \brief Extended Address */
	uint8_t 		data[] ; /**< \brief TODO: docs */
} _PACKED_  ;

typedef struct can_ext_record	can_ext_record_s;

/**
 * \brief CAN data record
 *
 * Defines the VCI CAN data record format 
 */
struct vci_can_ext_data_record
{
	vci_data_record_s	data_record ; /**< \brief TODO: docs */
	uint32_t		address ;     /**< \brief TODO: docs */
	uint8_t			ext_address ; /**< \brief Extended Address */
	uint8_t 		data[] ;      /**< \brief TODO: docs */
} _PACKED_  ;

typedef struct vci_can_ext_data_record		vci_can_ext_data_record_s;

/**
 * \brief CAN event record
 *
 * Defines the VCI CAN event record format. They include the CAN Id.
 */
struct vci_can_event_record
{
	vci_event_record_s	event_record ;
	uint32_t		address ;
} _PACKED_  ;

typedef struct vci_can_event_record		vci_can_event_record_s;

/**
 * \brief ISO15765 Protocol Timing
 */
struct vci_iso15765_timing
{
	uint32_t 		n_ArAs;
	uint32_t		n_BrBs;
	uint32_t		n_CrCs;
};

typedef struct vci_iso15765_timing 		vci_iso15765_timing_s;

/**
 * \brief TX Filter Params
 */
struct vci_gmlan_tx_usdt
{
	vci_iso15765_timing_s	timing;
	uint32_t		flags;
};

typedef struct vci_gmlan_tx_usdt	vci_gmlan_tx_usdt_s;

/**
 * TX Filter flags
 */

#define TX_EXTENDED_ADDESSING 	(0x00000001)
#define TX_STMIN_OVERIDE	(0x00000002)
#define TX_PADDING_ACTIVE	(0x00000004)

/**
 * \brief RX Filter Params
 */
struct vci_gmlan_rx_usdt
{
	vci_iso15765_timing_s	timing;
	uint16_t		bs;
	uint16_t		st_min;
	uint32_t		flags;
};

typedef struct vci_gmlan_rx_usdt	vci_gmlan_rx_usdt_s;

/**
 * RX Filter flags
 */

#define RX_EXTENDED_ADDESSING 	(0x00000001)
#define RX_EAVSEDROP_MODE	(0x00000002)
#define RX_PADDING_ACTIVE	(0x00000004)

/*****************************************************************************/
/* MACRO : vci_can_data_record_constructor */
/*!
 * \brief PURPOSE : Completes the fields of a vci data record
 *
 * DESCRIPTION:	Fills in the fields of a vci can data record based on the
 *		supplied parameters.
 *
 \param	vci_rec		Pointer to a vci_event_record struct
 \param	flags		Flags
 \param	user_data	Value for user data in the VCI record header
 \param	address		CAN address
 \param payload		CAN data payload length in bytes
 \param payload_ptr	Pointer to the data to copy to the record, may be NULL
 */

#define VCI_CAN_DATA_RECORD_SZ	sizeof( vci_can_data_record_s )
#define VCI_CAN_DATA_SZ		(sizeof( vci_can_data_record_s ) -\
				 sizeof( vci_data_record_s ))
#define VCI_CAN_DATA_DATA_SZ		(sizeof( vci_can_data_record_s ) -\
				 sizeof( vci_record_s ))
/*
 * Return the payload length of a vci can data record
 * excluding the address field, takes a 
 * vci_can_data_record_s pointer parameter
 */
#define VCI_CAN_DATA_PAYLOAD_LEN(d)\
	(ntohs(((vci_can_data_record_s*)d)->data_record.header.data_length) - \
			VCI_CAN_DATA_DATA_SZ )

#define vci_can_record_constructor( \
	_can_rec, _address, _payload, _payload_ptr) \
{ \
	can_record_s* _vci_can_rec =\
		(can_record_s*)_can_rec; \
	_vci_can_rec->address = htonl( _address ); \
	if( _payload_ptr )\
		memcpy( _vci_can_rec->data, _payload_ptr, _payload  );\
}

#define vci_can_data_record_constructor( \
	_vci_rec, _flags, _user_data, _address, _payload, _payload_ptr) \
{ \
	vci_can_data_record_s* _vci_can_data_rec =\
		(vci_can_data_record_s*)_vci_rec; \
	vci_data_record_constructor( _vci_rec, _flags, _user_data, \
			VCI_CAN_DATA_SZ + _payload, NULL ); \
	_vci_can_data_rec->address = htonl( _address ); \
	if( _payload_ptr )\
		memcpy( _vci_can_data_rec->data, _payload_ptr, _payload  );\
}
/*****************************************************************************/
/* MACRO : vci_can_ext_data_record_constructor */
/*!
 * \brief PURPOSE : Completes the fields of a vci data record
 *
 * DESCRIPTION:	Fills in the fields of a vci can data record based on the
 *		supplied parameters.
 *
 \param	vci_rec		Pointer to a vci_event_record struct
 \param	flags		Flags
 \param	user_data	Value for user data in the VCI record header
 \param	address		CAN address
 \param payload		CAN data payload length in bytes
 \param payload_ptr	Pointer to the data to copy to the record, may be NULL
 */

#define VCI_CAN_EXT_DATA_RECORD_SZ	sizeof( vci_can_ext_data_record_s )
#define VCI_CAN_EXT_DATA_SZ		(sizeof( vci_can_ext_data_record_s ) -\
				 sizeof( vci_data_record_s ))
/*
 * Return the payload length of a vci can data record
 * excluding the address field, takes a 
 * vci_can_data_record_s pointer parameter
 */
#define VCI_CAN_EXT_DATA_PAYLOAD_LEN(d)\
	(ntohs(((vci_can_ext_data_record_s*)d)->data_record.header.data_length) - \
	VCI_DATA_DATA_SZ - sizeof(can_ext_record_s))

#define vci_can_data_ext_record_constructor( \
	_vci_rec, _flags, _user_data, _address, _payload, _payload_ptr) \
{ \
	vci_can_ext_data_record_s* _vci_can_data_rec =\
       		(vci_can_ext_data_record_s*)_vci_rec; \
	vci_data_record_constructor( _vci_rec, _flags, _user_data, \
			VCI_CAN_EXT_DATA_SZ + _payload, NULL ); \
	_vci_can_data_rec->address = htonl( _address ); \
	if( _payload_ptr )\
		memcpy( _vci_can_data_rec->data, _payload_ptr, _payload  );\
}
/*****************************************************************************/
/* MACRO : vci_can_ext_data_record_constructor */
/*
 * Define a simple config data record that takes a single uint32
 * data field. This can be used for 90% of configurations records
 */

#define VCI_CAN_EVENT_DATA_SZ	sizeof( uint32_t )

#define vci_can_event_record_constructor( \
		_vci_rec, _op_code, _user_data, _canid ) \
{ \
	vci_can_event_record_s* _vci_can_event_rec=\
       		(vci_can_event_record_s*)_vci_rec; \
	vci_event_record_constructor( &_vci_can_event_rec->event_record,\
		_op_code, _user_data, VCI_CAN_EVENT_DATA_SZ ); \
	_vci_can_event_rec->address = htonl( _canid ); \
}

/* CAN flags */
#define VCI_CAN_29BIT_ID		(0x00000100)
#define VCI_CAN_REMOTE_FRAME		(0x00000200)
#define VCI_CAN_ISO15765_EXT		(0x00000400)

#define VCI_CAN_J1939_EXT		(0x00000400)
#define VCI_CAN_PAD_FRAME		(0x00000800)
#define VCI_CAN_HIGH_VOLTAGE		(0x00001000)
#define VCI_CAN_PAD_ERROR		(0x00002000)		// Implement at a higher level?
#define VCI_RECOVERING_FROM_ERROR	(0x00004000)
/* GMLAN/ISO15765 Frame flags */
#define VCI_UUDT_CANID			(0x00010000)
#define VCI_USDT_CANID			(0x00020000)

#define VCI_USDT_TXIP_CANID		(0x00040000)
#define VCI_USDT_RXIP_CANID		(0x00080000)

/*
#define VCI_FRAME_TYPE_FF		(0x00040000)
#define VCI_FRAME_TYPE_FC		(0x00080000)
#define VCI_FRAME_TYPE_CF		(0x00100000)
#define VCI_FRAME_TYPE_SF		(0x00200000)
*/

#define VCI_TX_FILTER			(0x00004000)
#define VCI_RX_FILTER			(0x00008000)
#define VCI_PROCESS_FILTER 		(0x00010000)
#define VCI_TX_NO_FLOW			(0x00100000)
#define VCI_RX_NO_FLOW			(0x00200000)

/* ISO15765 protocol events.  Used for abort events, timeouts and errors */
enum VCI_ISO15765_EVENTS
{
	VCI_ISO15765_SUCCESS		= 0,				/* Not reported as an event */
	VCI_ISO15765_TIMEOUT_N_As	= VCI_START_xEVENT_INDEX,
	VCI_ISO15765_TIMEOUT_N_Ar,
	VCI_ISO15765_TIMEOUT_N_Bs,
	VCI_ISO15765_TIMEOUT_N_Br,
	VCI_ISO15765_TIMEOUT_N_Cs,
	VCI_ISO15765_TIMEOUT_N_Cr,
	VCI_ISO15765_WRONG_SN,
	VCI_ISO15765_INVALID_FS,
	VCI_ISO15765_BUFFER_OVFLW,
	VCI_ISO15765_UNEXP_PDU,
	VCI_ISO15765_WFT_OVRN,
	VCI_ISO15765_INVALID_DATA_LENGTH,
	VCI_ISO15765_INVALID_DLC,

    /* CAN controller events */

	VCI_CAN_OK,
	VCI_CAN_TX_WARN,
	VCI_CAN_RX_WARN,
	VCI_CAN_TX_ERROR,
	VCI_CAN_RX_ERROR,
	VCI_CAN_BUS_OFF,
	VCI_CAN_RX_OVERRUN,
	VCI_CAN_TX_ARB_LOST,
	VCI_CAN_NO_ACK,							/* No acknowledgers */
	VCI_CAN_WAKE_UP,						/* Wake-up from suspend */
	VCI_CAN_HIGH_V,							/* SWCAN high-voltage - rx flag in a data frame? */
	VCI_CAN_FORM_ERR,
	VCI_CAN_CRC_ERR,
	VCI_CAN_STUFF_ERR,
	VCI_CAN_BIT_ERR,

	/* */
	VCI_ISO15765_EVENT_MAX

} ;

/* J1939 protocol events.  Used for abort events, timeouts and errors */
enum VCI_J1939_EVENTS
{
	VCI_J1939_SUCCESS		= 0,				/* Not reported as an event */
	VCI_J1939_TIMEOUT_T1	= VCI_ISO15765_EVENT_MAX,
	VCI_J1939_TIMEOUT_T2,
	VCI_J1939_TIMEOUT_T3,
	VCI_J1939_TIMEOUT_T4,
	VCI_J1939_TIMEOUT_Tr,
	VCI_J1939_TIMEOUT_STMin,
	VCI_J1939_TIMEOUT_Th,
	VCI_J1939_TIMEOUT_As,
	VCI_J1939_WRONG_SN,
	VCI_J1939_INVALID_FS,
	VCI_J1939_BUFFER_OVFLW,
	VCI_J1939_UNEXP_PDU,
	VCI_J1939_WFT_OVRN,
	VCI_J1939_INVALID_DATA_LENGTH,
	VCI_J1939_INVALID_DLC,

	VCI_J1939_EVENT_MAX

} ;
#endif


