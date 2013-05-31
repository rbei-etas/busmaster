/*****************************************************************************
 *
 * MODULE : vci
 *!
 * \brief PURPOSE : VCI char driver interface definitions for the T-MVCI 
 *
 * usr/local/include/vci/vci.h
 *
 * DESCRIPTION : TODO: detailed description incl author and licence/copyright?
 *
 *****************************************************************************/

#ifndef _VCI_H
#define _VCI_H

/* Packed structure directive/macro */
#ifdef _WIN32
/* Turn off MSVC 4200 warning */
#ifdef _MSC_VER
#pragma warning( disable : 4200 )
#endif
/* For Windows: remember the current packing value and then pack on 1 byte boundary */
#pragma pack( push, pack_on_entry )
#pragma pack(1)
#define _PACKED_
/* bsd */
typedef	unsigned __int8		uint8_t;
typedef	unsigned __int16	uint16_t;
typedef	unsigned __int32	uint32_t;
typedef unsigned __int64	uint64_t;
#else
/* For Linux platforms */
#define _PACKED_		__attribute__ ((packed))
#if defined( __KERNEL__ )
#include <linux/types.h>
#else
#include <stdint.h>
#endif
#endif

#ifndef min
#define min(x,y) ((x) < (y) ? (x) : (y))
#endif
#ifndef max
#define max(x,y) ((x) > (y) ? (x) : (y))
#endif

/*****************************************************************************/
/*                               S Y S C T L s                               */
/*****************************************************************************/

/* TODO: move CTL_VCI into include/linux/sysctl.h ? */
#define CTL_VCI		CTL_CPU     + 100
#define VCI_CTRLRS	1
#define CTRLR_UART	1000
#define CTRLR_CAN	2000
#define CTRLR_BDL	3000
#define VCI_PROTOS	2
#define PROTO_UART	1
#define PROTO_ISO9141	1
#define PROTO_ISO14230	2
#define PROTO_CHANNELS	100
#define PROTO_CAN	2
#define PROTO_BDLC	3
#define	VCI_HAL		3

#define VCI_SYSCTL_VCI_HAL(dir)     { 0,    "hal"       , NULL, 0, 0555, dir }
#define VCI_SYSCTL_PROTOBDLC(dir)   { 0,    "bdlc"      , NULL, 0, 0555, dir }
#define VCI_SYSCTL_PROTOCAN(dir)    { 0,    "can"       , NULL, 0, 0555, dir }
#define VCI_SYSCTL_PROTOCHANLS(dir) { 0			,      "channels", NULL, 0, 0555, dir }
#define VCI_SYSCTL_PROTOUART(dir)   { 0 		,    "uart"      , NULL, 0, 0555, dir }
#define VCI_SYSCTL_PROTOS(dir)      { 0			,  "protocols"   , NULL, 0, 0555, dir }
#define VCI_SYSCTL_UARTCTRLR(dir)   { 0 		,    NULL        , NULL, 0, 0555, dir }
#define VCI_SYSCTL_CTRLRS(dir)      { 0 		,  "controllers" , NULL, 0, 0555, dir }
#define VCI_SYSCTL_VCI(dir)         { 0 	,"vci"           , NULL, 0, 0555, dir } 

#define VCI_SYSCTL_DEV(dir)    { CTL_DEV         , "dev"  , NULL, 0, 0555, dir }

/*****************************************************************************/
/*                   F R A M E   T Y P E   E N C O D I N G                   */
/*****************************************************************************/

/* These frame types apply to all messages... */

/* Frame type */
enum vci_record_type
{
	VCI_DATA,
	VCI_EVENT,
	VCI_CONFIG_REQUEST,
	VCI_CONFIG_RESPONSE,
	VCI_RECORD_TYPE_MAX
};

typedef enum vci_record_type		vci_record_type_e;

/*****************************************************************************/
/*                 V C I   R E C O R D   S T R U C T U R E S                 */
/*****************************************************************************/

/* Message header structure */

struct vci_record_hdr 
{
	uint8_t		record_type ;	/* frame type and protocol */
	uint8_t		sub_type ;	/* frame type and protocol */
	uint16_t	data_length ;	/* length of data field */
	uint32_t	user_data ;	/* passed in by the client;
				   returned unmodified in response records */
} _PACKED_ ;

typedef struct vci_record_hdr		vci_record_hdr_s;

#define VCI_HLEN	(sizeof(vci_record_hdr_s))

enum VCI_RECORD_HDR_SUB_TYPES
{
	VCI_RECORD_SUB_TYPE_RAW = 0,		/* Just data bytes */
	VCI_RECORD_SUB_TYPE_COLLISION_DETECT
};

struct vci_record 
{
	vci_record_hdr_s	header ; 
#ifndef _WIN32
	uint8_t 	data[] ;/* VCI payload.  Variable length record */
#endif
} _PACKED_  ;

typedef struct vci_record	vci_record_s;

struct vci_config_record 
{
	vci_record_hdr_s	header ; 
	uint32_t	op_code ; 	/* VCI config request (IOCTL) code */
	uint32_t	return_code ; 	/* VCI config response code */
        uint32_t  timestamp ; 
#ifndef _WIN32
	uint8_t data[] ;/* VCI config payload.  Variable length record */
#endif
} _PACKED_  ;

typedef struct vci_config_record	vci_config_record_s;

struct vci_config_record_with_data 
{
	vci_record_hdr_s	header ; 
	uint32_t	op_code ; 	/* VCI config request (IOCTL) code */
	uint32_t	return_code ; 	/* VCI config response code */
        uint32_t  timestamp ; 
//#ifndef _WIN32
	uint8_t data[] ;/* VCI config payload.  Variable length record */
//#endif
} _PACKED_  ;

typedef struct vci_config_record_with_data	vci_config_record_with_data_s;

struct vci_data_record 
{
	vci_record_hdr_s	header ; 
	uint32_t	flags ;		/* VCI protocol specific flags */ 
	uint32_t	timestamp ;	/* 1 us/bit */
#ifndef _WIN32
	uint8_t	data[] ;	/* VCI data payload.  Variable length record */
#endif
} _PACKED_  ;

struct vci_data_record_with_data
{
	vci_record_hdr_s	header ; 
	uint32_t	flags ;		/* VCI protocol specific flags */ 
	uint32_t	timestamp ;	/* 1 us/bit */
	uint8_t	data[] ;		/* VCI data payload.  Variable length record */
} _PACKED_ ;

typedef struct vci_data_record	vci_data_record_s;
typedef struct vci_data_record_with_data vci_data_record_with_data_s;

struct vci_event_record 
{
	vci_record_hdr_s	header ; 
	uint32_t	event_code ;	/* VCI event code */ 
	uint32_t	timestamp ;	/* 1 us/bit */
#ifndef _WIN32
	uint8_t	data[] ;	/* VCI event payload.  Variable length record */
#endif
} _PACKED_  ;

typedef struct vci_event_record		vci_event_record_s;
/*
 * Define the size for each record type
 */
#define VCI_RECORD_HDR_SZ	sizeof( vci_record_hdr_s )
#define VCI_RECORD_SZ		sizeof( vci_record_s )
#define VCI_EVENT_RECORD_SZ	sizeof( vci_event_record_s )
#define VCI_DATA_RECORD_SZ	sizeof( vci_data_record_s )
#define VCI_CONFIG_RECORD_SZ	sizeof( vci_config_record_s )
/*
 * Define just the size of the data portion (payload) for each record type
 */
#define VCI_RECORD_HDR_DATA_SZ	sizeof( vci_record_hdr_s )
#define VCI_RECORD_DATA_SZ	sizeof( vci_record_s )
#define VCI_EVENT_DATA_SZ	(sizeof( vci_event_record_s ) -\
				 sizeof( vci_record_s )) 
#define VCI_DATA_DATA_SZ	(sizeof( vci_data_record_s ) -\
				 sizeof( vci_record_s ))
#define VCI_CONFIG_DATA_SZ	(sizeof( vci_config_record_s ) -\
				 sizeof( vci_record_s ))
/*
 * Return the payload length of a vci data record
 * takes a vci_data_record_s pointer parameter
 */
#define VCI_DATA_PAYLOAD_LEN(d) (ntohs( \
				((vci_data_record_s*)d)->header.data_length) \
				- VCI_DATA_DATA_SZ)
/*
 * Helper macros for manipulating vci_records
 *
 * TODO: Base MAX_RECORD_SZ on the union of the possible record types?
 */
#define VCI_MAX_FRAME_SZ	(4200)
#define VCI_MAX_RECORD_SZ	(VCI_MAX_FRAME_SZ+sizeof( vci_data_record_s ))
#define VCI_MAXDATALEN		(4*VCI_MAX_RECORD_SZ)
/*
 * This macro changes depending upon where is file is used. The timestamp
 * field is only usually completed by records generated in the Linux kernel
 */
#if defined( __KERNEL__ )
extern u_long  vci_get_rebased_timestamp( void );
#define do_gettimestamp		vci_get_rebased_timestamp() 
#define do_gettimediff( x )	get_usec_timediff( x )
#elif defined( _WIN32 )
#define do_gettimestamp		0
#define __inline__
#else
#define do_gettimestamp		0
#endif

/*****************************************************************************/
/* FUNCTION : vci_record_constructor */
/*!
 * \brief PURPOSE : Completes the fields of a vci record
 *
 * DESCRIPTION : Fills in the fields of a vci record based on the supplied
 * 		parameters.
 *
 \param	vci_rec		Pointer to a vci_record struct
 \param	type		VCI record type
 \param	user_data	value for user data in the VCI record header
 \return
*/

#define  vci_record_constructor( _vci_rec, _type, _user_data, _payload ) \
{ \
	((vci_record_s*)_vci_rec)->header.record_type = _type; \
	((vci_record_s*)_vci_rec)->header.sub_type = 0; \
	((vci_record_s*)_vci_rec)->header.data_length = htons( _payload ); \
	((vci_record_s*)_vci_rec)->header.user_data = htonl( _user_data ); \
}


/*****************************************************************************/
/* FUNCTION : vci_event_record_constructor */
/*!
 * \brief PURPOSE : Completes the fields of a vci event record
 *
 * DESCRIPTION : Fills in the fields of a vci record based on the supplied
 * 		parameters.
 *
 \param	vci_rec		Pointer to a vci_event_record struct
 \param	event_code	Event code
 \param	user_data	value for user data in the VCI record header
 \return
 */

#define vci_event_record_constructor( \
		_vci_rec, _event_code, _user_data, _payload) \
{ \
	vci_event_record_s* _vci_event_rec = (vci_event_record_s*)_vci_rec; \
	vci_record_constructor( _vci_rec, VCI_EVENT, _user_data, \
			VCI_EVENT_DATA_SZ + (_payload) ); \
	_vci_event_rec->event_code = htonl( _event_code ); \
	_vci_event_rec->timestamp = htonl( do_gettimestamp ); \
}

/*****************************************************************************/
/* FUNCTION : vci_event_flags_append_record_constructor */
/*!
 * \brief PURPOSE : Completes the fields of a vci event record
 *
 * DESCRIPTION : Fills in the fields of a vci record based on the supplied
 * 		parameters.
 *
 \param	vci_rec		Pointer to a vci_event_record struct
 \param	event_code	Event code
 \param	user_data	value for user data in the VCI record header
 \return
 */

#define vci_event_flags_append_record_constructor( \
		_vci_rec,_flags, _event_code, _user_data, _payload) \
{ \
	vci_event_record_s* _vci_event_rec = (vci_event_record_s*)_vci_rec; \
	vci_record_constructor( _vci_rec, VCI_EVENT, _user_data, \
			VCI_EVENT_DATA_SZ + (_payload) ); \
	_vci_event_rec->event_code = htonl(((_flags&0xFFFFFF00)|_event_code)) ; \
	_vci_event_rec->timestamp = htonl( do_gettimestamp ); \
}


/*****************************************************************************/
/* FUNCTION : vci_data_record_constructor */
/*!
 * \brief PURPOSE : Completes the fields of a vci data record
 *
 * DESCRIPTION:	Fills in the fields of a vci data record based on the
 *		supplied parameters.
 *
 \param	vci_rec		Pointer to a vci_event_record structure
 \param	flags		Flags
 \param	user_data	Value for user data in the VCI record header
 \param payload		Data record payload length in bytes
 \param payload_ptr	Pointer to the data to copy to the record, may be NULL
 */

#define vci_data_record_constructor( \
		_vci_rec, _flags, _user_data, _payload_len, _payload_ptr) \
{ \
	vci_data_record_with_data_s* _vci_data_rec = \
		(vci_data_record_with_data_s*)_vci_rec; \
	vci_record_constructor( _vci_rec, VCI_DATA, _user_data, \
			VCI_DATA_DATA_SZ + _payload_len ); \
	_vci_data_rec->flags = htonl( _flags ); \
	_vci_data_rec->timestamp = htonl( do_gettimestamp ); \
	if( _payload_ptr != NULL )\
		memcpy( _vci_data_rec->data, (const void*)_payload_ptr, \
				_payload_len  );\
}

/*****************************************************************************/
/* FUNCTION : vci_config_response_record_constructor */
/*!
 * \brief PURPOSE : Completes the fields of a vci config response record
 *
 * DESCRIPTION:	Fills in the fields of a vci config response record based
 * 		on the supplied parameters.
 *
 \param	vci_rec		Pointer to a vci_event_record struct
 \param	op_code		Operation code
 \param	user_data	Value for user data in the VCI record header
 \param payload		Config response parameter length
 */

#define vci_config_response_record_constructor( \
		_vci_rec, _op_code, _rtn_code, _user_data, _payload, _timestamp ) \
{ \
	vci_config_record_s* _vci_config_rec = (vci_config_record_s*)_vci_rec;\
	vci_record_constructor( _vci_rec, VCI_CONFIG_RESPONSE, _user_data, \
			VCI_CONFIG_DATA_SZ + _payload ); \
	_vci_config_rec->op_code = htonl( _op_code ); \
	_vci_config_rec->return_code = htonl( _rtn_code ); \
	 _vci_config_rec->timestamp = _timestamp ; \
}

/*****************************************************************************/
/* FUNCTION : vci_config_request_constructor */
/*!
 * \brief PURPOSE : Completes the fields of a vci config request record
 *
 * DESCRIPTION:	Fills in the fields of a vci config request record based
 * 		on the supplied parameters.
 *
 \param	vci_rec		Pointer to a vci_event_record struct
 \param	op_code		Operation code
 \param	user_data	Value for user data in the VCI record header
 \param payload		Config request parameter payload
 */

#define vci_config_request_record_constructor( \
		_vci_rec, _op_code, _user_data, _payload) \
{ \
	vci_config_record_s* _vci_config_rec = (vci_config_record_s*)_vci_rec;\
	vci_record_constructor( _vci_rec, VCI_CONFIG_REQUEST, _user_data,\
			VCI_CONFIG_DATA_SZ + _payload ); \
	_vci_config_rec->op_code = htonl( _op_code ); \
	_vci_config_rec->return_code = 0; \
}

				

/*****************************************************************************/
/*                     E V E N T    D E F I N I T I O N S                    */
/*****************************************************************************/

/* Common events use enum values 0 through 15.  Protocol and controller specific  */
/* enumerations start at 16.  All event enumerations define an equivalent success */
/* event (= 0).                                                                   */


/* VCI common events */

enum VCI_EVENTS
{
	VCI_EVENT_SUCCESS,	/* Not reported as an event */
	VCI_EVENT_TX_ACK,	/* Transmit acknowledge */
	VCI_EVENT_RX_SOF,	/* Start of frame */
	VCI_EVENT_RX_TIMEOUT,	/* Receive timeout event */
	VCI_EVENT_ERROR,	/* Linux driver error event, error is stored in error_code */
	VCI_EVENT_TX_TIMEOUT,	/* Transmit timeout event */ // TODO: move protocol specific timeouts to use this event
	VCI_EVENT_SYS_CON_ERR,	/* System Controller error event */
	VCI_EVENT_TX_ECHO_CORRUPT, /* Transmit Echo Error */
	VCI_EVENT_TX_CMD_QUEUED,
	VCI_EVENT_RSVD_09,
	VCI_EVENT_RSVD_10,
	VCI_EVENT_RSVD_11,
	VCI_EVENT_RSVD_12,
	VCI_EVENT_RSVD_13,
	VCI_EVENT_RSVD_14,
	VCI_EVENT_RSVD_15,
	/* */
	VCI_EVENT_MAX		/* May use up to VCI_START_xEVENT_INDEX - 1 */
} ;

/* The index at which the protocol and controller events start */
#define VCI_START_xEVENT_INDEX			(128)


/* RAW protocol events.  Used for abort events, timeouts and errors */
enum VCI_RAW_EVENTS
{
	VCI_RAW_SUCCESS		= 0,		/* Not reported as an event */
	/* */
	VCI_RAW_EVENT_MAX
} ;

/**
 * /brief: vci_error_event_record
 *
 * vci error event record definition and helper macros
 */
struct vci_error_event_record 
{
	vci_event_record_s	event ; 
	uint32_t			error_code ;
} _PACKED_  ;

typedef struct vci_error_event_record		vci_error_event_record_s;

#define VCI_ERROR_EVENT_RECORD_SZ	sizeof( vci_error_event_record_s ) 
#define VCI_ERROR_EVENT_DATA_SZ	(sizeof( vci_error_event_record_s ) -\
				 sizeof( vci_event_record_s ))

#define vci_error_event_record_constructor( \
		_vci_rec, _user_data, _error_code) \
{ \
	vci_error_event_record_s* _vci_error_event_rec =\
		(vci_error_event_record_s*)_vci_rec; \
	vci_event_record_constructor( _vci_rec, VCI_EVENT_ERROR, _user_data, \
		VCI_ERROR_EVENT_DATA_SZ ); \
	_vci_error_event_rec->error_code = htonl( _error_code ); \
}

/*****************************************************************************/
/*                      F L A G   D E F I N I T I O N S                      */
/*****************************************************************************/

/* Flag values - do not use ls 8-bits on write, reserved for internal use */
#define VCI_FLAGS_INTERNAL		(0x000000FF)

/* NOTE: Tx echoes generate a data frame with VCI_TX_ECHO set in the message header flags field */

/* tx/rx flags */
#define VCI_RX_BREAK			(0x80000000)		// event ?
//#define VCI_BLOCKING_TX		(0x04000000)		// Implement via fsync?


/*
 * Include the IOCTL definitions
 */
#include <vci/vci_ioctl.h>

#ifdef _WIN32
/* For Windows: reinstate value of packing value on entry to this file */
#pragma pack( pop, pack_on_entry )
#endif

/*
 * Include errno for user space programs
 */
#if !defined( __KERNEL__ )
#include <errno.h>
#endif

#if defined(_WIN32) && !defined(VCI_NO_ERROR_CODES)
/*
 * Include standard windows defintions
 */

/*****************************************************************************
 * Extends the Windows posix system wide error numbers (ERRNO.H) with Linux 
 * errors.
 * Note. Errors codes with values between 1 and 42 are already defined in 
 * windows header file ERRNO.H
 *
 * Allows windows apps to decode vci error events which have been set by a 
 * Linux system call returns.
 *
 * The vci framework will convert a Linux driver system error 
 * (negative number defined in "include/asm/errno.h") to a positive number 
 * defined below.
 * 
 * The struct vci_error_event_record member event.event_code will be set 
 * to VCI_EVENT_ERROR and the error_code member variable will contain 
 * the error code.
 ******************************************************************************/

#define	EIDRM		43	/* Identifier removed */
#define	ECHRNG		44	/* Channel number out of range */
#define	EL2NSYNC	45	/* Level 2 not synchronized */
#define	EL3HLT		46	/* Level 3 halted */
#define	EL3RST		47	/* Level 3 reset */
#define	ELNRNG		48	/* Link number out of range */
#define	EUNATCH		49	/* Protocol driver not attached */
#define	ENOCSI		50	/* No CSI structure available */
#define	EL2HLT		51	/* Level 2 halted */
#define	EBADE		52	/* Invalid exchange */
#define	EBADR		53	/* Invalid request descriptor */
#define	EXFULL		54	/* Exchange full */
#define	ENOANO		55	/* No anode */
#define	EBADRQC		56	/* Invalid request code */
#define	EBADSLT		57	/* Invalid slot */
/*
 * This clashes with windows definition so override with our own value
 */
#undef EDEADLOCK
#define	EDEADLOCK	58	/* File locking deadlock error */
#define	EBFONT		59	/* Bad font file format */
#define	ENOSTR		60	/* Device not a stream */
#define	ENODATA		61	/* No data available */
#define	ETIME		62	/* Timer expired */
#define	ENOSR		63	/* Out of streams resources */
#define	ENONET		64	/* Machine is not on the network */
#define	ENOPKG		65	/* Package not installed */
#define	EREMOTE		66	/* Object is remote */
#define	ENOLINK		67	/* Link has been severed */
#define	EADV		68	/* Advertise error */
#define	ESRMNT		69	/* Srmount error */
#define	ECOMM		70	/* Communication error on send */
#define	EPROTO		71	/* Protocol error */
#define	EMULTIHOP	72	/* Multihop attempted */
#define	EDOTDOT		73	/* RFS specific error */
#define	EBADMSG		74	/* Not a data message */
#define	EOVERFLOW	75	/* Value too large for defined data type */
#define	ENOTUNIQ	76	/* Name not unique on network */
#define	EBADFD		77	/* File descriptor in bad state */
#define	EREMCHG		78	/* Remote address changed */
#define	ELIBACC		79	/* Can not access a needed shared library */
#define	ELIBBAD		80	/* Accessing a corrupted shared library */
#define	ELIBSCN		81	/* .lib section in a.out corrupted */
#define	ELIBMAX		82	/* Attempting to link in too many shared libraries */
#define	ELIBEXEC	83	/* Cannot exec a shared library directly */
/*
 * This clashes with windows definition so override with our own value
 */
#undef  EILSEQ
#define	EILSEQ		84	/* Illegal byte sequence */
#define	ERESTART	85	/* Interrupted system call should be restarted */
#define	ESTRPIPE	86	/* Streams pipe error */
#define	EUSERS		87	/* Too many users */
#define	ENOTSOCK	88	/* Socket operation on non-socket */
#define	EDESTADDRREQ	89	/* Destination address required */
#define	EMSGSIZE	90	/* Message too long */
#define	EPROTOTYPE	91	/* Protocol wrong type for socket */
#define	ENOPROTOOPT	92	/* Protocol not available */
#define	EPROTONOSUPPORT	93	/* Protocol not supported */
#define	ESOCKTNOSUPPORT	94	/* Socket type not supported */
#define	EOPNOTSUPP	95	/* Operation not supported on transport endpoint */
#define	EPFNOSUPPORT	96	/* Protocol family not supported */
#define	EAFNOSUPPORT	97	/* Address family not supported by protocol */
#define	EADDRINUSE	98	/* Address already in use */
#define	EADDRNOTAVAIL	99	/* Cannot assign requested address */
#define	ENETDOWN	100	/* Network is down */
#define	ENETUNREACH	101	/* Network is unreachable */
#define	ENETRESET	102	/* Network dropped connection because of reset */
#define	ECONNABORTED	103	/* Software caused connection abort */
#define	ECONNRESET	104	/* Connection reset by peer */
#define	ENOBUFS		105	/* No buffer space available */
#define	EISCONN		106	/* Transport endpoint is already connected */
#define	ENOTCONN	107	/* Transport endpoint is not connected */
#define	ESHUTDOWN	108	/* Cannot send after transport endpoint shutdown */
#define	ETOOMANYREFS	109	/* Too many references: cannot splice */
#define	ETIMEDOUT	110	/* Connection timed out */
#define	ECONNREFUSED	111	/* Connection refused */
#define	EHOSTDOWN	112	/* Host is down */
#define	EHOSTUNREACH	113	/* No route to host */
#define	EALREADY	114	/* Operation already in progress */
#define	EINPROGRESS	115	/* Operation now in progress */
#define	ESTALE		116	/* Stale NFS file handle */
#define	EUCLEAN		117	/* Structure needs cleaning */
#define	ENOTNAM		118	/* Not a XENIX named type file */
#define	ENAVAIL		119	/* No XENIX semaphores available */
#define	EISNAM		120	/* Is a named type file */
#define	EREMOTEIO	121	/* Remote I/O error */
#define	EDQUOT		122	/* Quota exceeded */
#define	ENOMEDIUM	123	/* No medium found */
#define	EMEDIUMTYPE	124	/* Wrong medium type */

#define _LAST_ERRNO	515
#endif

#endif

