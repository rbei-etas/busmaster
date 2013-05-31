/**
 * \file include/vci/vci_uart.h
 * \brief Definitions for using the VCI UART protocol drivers
 * 
 * All UART protocol specific definitions can be found in this file.  This file does not need to be explicitly
 * included, instead include vci.h
 *
 * \internal TODO: licence/copyright?
 * \author Dirk Witvrouwen
 */
 
/**
 * \example vci_iso14230.c
 * Example usage of the T-MVCI UART ISO14230 protocol interface
 */

/**
 * \example vci_iso9141.c
 * Example usage of the T-MVCI UART ISO9141 protocol interface
 */

#ifndef _VCI_UART_H
#define _VCI_UART_H

#include <vci/vci.h>
#ifdef _WIN32
#include <limits.h>
#endif

/*******************************************************************************
 * General
 ******************************************************************************/

/******************************************************************************/
/**
 * \brief Default baudrate setting if not overriden by a protocol default 
 */
/******************************************************************************/
#define VCI_DEFUART_BAUD                          10400  
/******************************************************************************/
/**
 * \brief Default max RX interbyte timeout if not overriden by a protocol
 * default 
 */
/******************************************************************************/
#define	VCI_DEFUART_RX_MAX_INTERB	              20000
/******************************************************************************/
/**
 * \brief Default min RX interbyte timeout if not overriden by a protocol
 * default 
 */
/******************************************************************************/
#define	VCI_DEFUART_RX_MIN_INTERB	                  0
/******************************************************************************/
/**
 * \brief Default max TX interbyte timeout if not overriden by a protocol
 * default 
 */
/******************************************************************************/
#define	VCI_DEFUART_TX_MAX_INTERB			UINT_MAX
/******************************************************************************/
/**
 * \brief Default min TX interbyte timeout if not overriden by a protocol
 * default 
 */
/******************************************************************************/
#define	VCI_DEFUART_TX_MIN_INTERB	               5000
/******************************************************************************/
/**
 * \brief Default max RX interpacket timeout if not overriden by a protocol 
 * default 
 */
/******************************************************************************/
#define	VCI_DEFUART_RX_MAX_INTERP                 50000  
/******************************************************************************/
/**
 * \brief Default min RX interpacket timeout if not overriden by a protocol 
 * default 
 */
/******************************************************************************/
#define	VCI_DEFUART_RX_MIN_INTERP                     0
/******************************************************************************/
/**
 * \brief Default max TX interpacket timeout if not overriden by a protocol 
 * default 
 */
/******************************************************************************/
#define	VCI_DEFUART_TX_MAX_INTERP               5000000
/******************************************************************************/
/**
 * \brief Default min TX interpacket timeout if not overriden by a protocol 
 * default 
 */
/******************************************************************************/
#define	VCI_DEFUART_TX_MIN_INTERP                 55000
/******************************************************************************/
/**
 * \brief Default min bus idle timeout if not overriden by a protocol default 
 */
/******************************************************************************/
#define VCI_DEFUART_IDLE_MIN                          0
/******************************************************************************/
/**
 * \brief Default max bus idle timeout if not overriden by a protocol default 
 */
/******************************************************************************/
#define VCI_DEFUART_IDLE_MAX         ((uint32_t)-1)
 
/**
 * \brief Groups lower and upper limits
 * 
 * This structure is used to group upper and lower limits that apply to the same
 * value.
 */
typedef struct {
	uint32_t min;	/**< \brief lower limit */
	uint32_t max;	/**< \brief upper limit */
} vci_minmax;

typedef vci_minmax vci_minmax_s; /**< \brief typedef of struct vci_minmax */

/**
 * \brief Defintion of a UART frame format.
 * 
 * One start bit is implicit
 */
typedef struct vci_uart_frame {
	uint32_t msbfirst;	/**< \brief 0=lsb send first; non-zero=msb send first */
	uint32_t databits;	/**< \brief The amount of data bits in the frame */
	uint32_t parity;	/**< \brief 0=none, 1=odd and 2=even */
	uint32_t stopbits;	/**< \brief The amount of stop bits in a frame */
} vci_uart_frame_s ;

/*******************************************************************************
 * Initialisation structures
 ******************************************************************************/

/**
 * \brief This structure defines an ISO14230(KWP2000) fast initialisation.
 * 
 * Used to configure ISO14230 initialisation.  The actual initialisation shall commence when a record is written
 * containing the VCI_UART_FASTINIT flag. Typically the data part of the written record contains a 
 * 'start communications' service request.
 */
typedef struct {
	/**
	 * \brief Idle time in us to be respected before starting the wakeup pulse 
	 * 
	 * It is advised to set a resonable Tidle.max value so that the actual initialisation sequence can timeout.
	 * Set Tidle = { 0, x } when no idle time needs to be respected.  
	 * TiniH =  TWup - TiniL (bus high time during wakeup pulse)
	 */
	vci_minmax	Tidle;
	uint32_t	TWup;  /**< \brief Total wakeup pulse duration in us */
	uint32_t	TiniL; /**< \brief Bus low time during wakeup pulse in us*/
} vci_uart_fastinit;

typedef vci_uart_fastinit vci_uart_fastinit_s; /**< \brief typedef of struct vci_uart_fastinit */

/**
 * \brief This structure defines an x baud initialisation, like ISO9141's 5-baud initialisation.
 * 
 * Used to configure an x-baud initialisation.  The actual initialisation commences when a record is written 
 * containing the VCI_UART_XBAUDINIT flag.  The data part of the written record contains the wakeup address.
 * 
 */
typedef struct {
	/**
	 * \brief Idle time in us to respect before starting initialisation 
	 * 
	 * It is advised to set a resonable VCI_W0.max value so that the actual
	 * initialisation sequence can timeout.
	 * Set VCI_W0 = { 0, x } when no idle time needs to be respected.
	 */
	vci_minmax        VCI_W0;    
	uint32_t            VCI_W1;    /**< \brief Maximum time (us) to wait for the sync byte */
	uint32_t            VCI_W2;    /**< \brief Maximum time (us) to wait for KB1 */
	uint32_t            VCI_W3;    /**< \brief Maximum time (us) to waut for KB2 */
	uint32_t            VCI_W4a;   /**< \brief Minimum time (us) to wait before sending KB2 complement */
	uint32_t            VCI_W4b;   /**< \brief Maximum time (us) to wait for address complement (set to zero if not sent by ECU) */
	uint32_t            VCI_xbaud;   /**< \brief The baudrate at which to send the address byte (often 5 baud) */
} vci_uart_xbaudinit;

typedef vci_uart_xbaudinit vci_uart_xbaudinit_s; /**< \brief typedef of struct vci_uart_xbaudinit */


typedef struct {
	vci_uart_xbaudinit xbaud_sim_init_datas;
	uint32_t xbaud_sim_KB1;
 	uint32_t xbaud_sim_KB2;
	uint32_t xbaud_sim_Add;
}vci_uart_simulation_xbaudinit;
 
typedef vci_uart_simulation_xbaudinit vci_uart_simulation_xbaudinit_s;


#define VCI_UART_MAX_ACK_LIST_SZ	((unsigned char)15)
#define VCI_UART_ACK_ALL		((unsigned char)0xFF)
typedef struct {
	unsigned char AckListSz;	/**< \brief Size of the list, i.e. what is actually populated */
	char AckList[VCI_UART_MAX_ACK_LIST_SZ];	/**< \brief Size of the list, i.e. what is actually populated */
} vci_uart_ack_list;

/********************************************************************************************************
 * Events
 ********************************************************************************************************/

/**
 * \brief Generic VCI UART events 
 * 
 * These events can be generated by the UART's transmitting/receiving the data, 
 * regardless of the protocol in use.
 */
enum VCI_UART_EVENTS
{
	VCI_UART_FRAME_ERR = VCI_START_xEVENT_INDEX,	/**< \brief Framing error occured */				
	VCI_UART_PARITY_ERR,				/**< \brief Parity error occured */
	VCI_UART_RX_OVERRUN,				/**< \brief The RX buffer overrun */
	VCI_UART_RX_BREAK,				/**< \brief A BREAK was received */
	VCI_UART_OVER_I,				/**< \brief Overcurrent condition encountered */
	VCI_UART_CD_COLLISION,				/**< \brief Collision detected */
	/**
	 * \internal \brief Maximum generic UART event number 
	 * 
	 * The VCI interface will get broken if you ever need to increase 
	 * VCI_UART_EVENT_MAX.  All userspace app's using the UART enumeration 
	 * types will need recompilation.
	 */
	VCI_UART_EVENT_MAX = VCI_START_xEVENT_INDEX + 32
} ;

/**
 * \brief ISO9141 protocol events 
 */
enum VCI_ISO9141_EVENTS
{
	/**< \internal \brief Not reported as an event */
	VCI_ISO9141_SUCCESS		= VCI_EVENT_SUCCESS,

	/**< \brief Returns initialisation data: address, sync, KB1, KB2, ~KB2, [~address] */	
	VCI_ISO9141_XBAUD_INIT		= VCI_UART_EVENT_MAX,

	/**< \brief x-baud initialisation failure, could not evaluate the sync pattern */	
	VCI_ISO9141_SYNC_BYTE_RX_TIMEOUT,

	/**< \brief x-baud initialisation failure, KB1 not received in time */	
	VCI_ISO9141_KEY_BYTE1_RX_TIMEOUT,

	/**< \brief x-baud initialisation failure, KB2 not received in time */
	VCI_ISO9141_KEY_BYTE2_RX_TIMEOUT,

	/**< \brief x-baud initialisation failure, Inverted address byte not received in time */
	VCI_ISO9141_INV_BYTE_RX_TIMEOUT,

	/**< \brief x-baud initialisation failure, The x-baud request has invalid data */
	VCI_ISO9141_XBAUD_INIT_INVAL,

	/**< \brief Message length error */
	VCI_ISO9141_MSG_LEN_ERR,

	/**< \brief x-baud initialisation failure, sync btye incorrect */
	VCI_ISO9141_SYNC_BYTE_RX_INVALID,

	/**< \brief x-baud initialisation failure, KB1 incorrect */
	VCI_ISO9141_KEY_BYTE1_RX_INVALID,

	/**< \brief x-baud initialisation failure, KB2 incorrect */	
	VCI_ISO9141_KEY_BYTE2_RX_INVALID,

	/**< \internal \brief Maximum ISO9141 protocol event number */
	VCI_ISO9141_EVENT_MAX
} ;

/**
 * \brief ISO14230 protocol events 
 * 
 * Handle the ISO14230 x-baud events like you would for the ISO9141 x-baud events.
 */
enum VCI_ISO14230_EVENTS
{
	/**< \internal \brief Not reported as an event */
	VCI_ISO14230_SUCCESS			= VCI_EVENT_SUCCESS,

	/**< \brief Returns initialisation data: address, sync, KB1, KB2, ~KB2, [~address] */	
	VCI_ISO14230_XBAUD_INIT			= VCI_ISO9141_XBAUD_INIT,

	/**< \brief x-baud initialisation failure, could not evaluate the sync pattern */	
	VCI_ISO14230_SYNC_BYTE_RX_TIMEOUT	= VCI_ISO9141_SYNC_BYTE_RX_TIMEOUT,

	/**< \brief x-baud initialisation failure, KB1 not received in time */	
	VCI_ISO14230_KEY_BYTE1_RX_TIMEOUT	= VCI_ISO9141_KEY_BYTE1_RX_TIMEOUT,

	/**< \brief x-baud initialisation failure, KB2 not received in time */
	VCI_ISO14230_KEY_BYTE2_RX_TIMEOUT	= VCI_ISO9141_KEY_BYTE2_RX_TIMEOUT,

	/**< \brief x-baud initialisation failure, Inverted address byte not received in time */
	VCI_ISO14230_INV_BYTE_RX_TIMEOUT	= VCI_ISO9141_INV_BYTE_RX_TIMEOUT,

	/**< \brief x-baud initialisation failure, The x-baud request has invalid data */
	VCI_ISO14230_XBAUD_INIT_INVAL		= VCI_ISO9141_XBAUD_INIT_INVAL,

	/**< \brief Message length error */
	VCI_ISO14230_MSG_LEN_ERR		= VCI_ISO9141_MSG_LEN_ERR,

	/**< \brief x-baud initialisation failure, sync btye incorrect */
	VCI_ISO14230_SYNC_BYTE_RX_INVALID	= VCI_ISO9141_SYNC_BYTE_RX_INVALID,

	/**< \brief x-baud initialisation failure, KB1 incorrect */
	VCI_ISO14230_KEY_BYTE1_RX_INVALID	= VCI_ISO9141_KEY_BYTE1_RX_INVALID,

	/**< \brief x-baud initialisation failure, KB2 incorrect */	
	VCI_ISO14230_KEY_BYTE2_RX_INVALID	= VCI_ISO9141_KEY_BYTE2_RX_INVALID,

	/**< \internal \brief Maximum ISO9141 protocol event number */
	VCI_ISO14230_EVENT_MAX
} ;

/**
 * \brief UART_EX protocol events 
 * 
 * Handle the UART_EX x-baud events like you would for the ISO9141 x-baud events.
 */
enum VCI_UART_EX_EVENTS
{
	/** \internal \brief Not reported as an event */
	VCI_UART_EX_SUCCESS = VCI_EVENT_SUCCESS,
	/** \brief Returns initialisation data: address, sync, KB1, KB2, 
	 * ~KB2, [~address] */
	VCI_UART_EX_XBAUD_INIT = VCI_ISO9141_XBAUD_INIT,
	/** \brief x-baud initialisation failure, could not evaluate the 
	 * sync pattern */
	VCI_UART_EX_SYNC_BYTE_RX_TIMEOUT = VCI_ISO9141_SYNC_BYTE_RX_TIMEOUT,
	/** \brief x-baud initialisation failure, KB1 not received in time */
	VCI_UART_EX_KEY_BYTE1_RX_TIMEOUT = VCI_ISO9141_KEY_BYTE1_RX_TIMEOUT,
	/** \brief x-baud initialisation failure, KB2 not received in time */
	VCI_UART_EX_KEY_BYTE2_RX_TIMEOUT = VCI_ISO9141_KEY_BYTE2_RX_TIMEOUT,
	/** \brief x-baud initialisation failure, Inverted address byte not 
	 * received in time */
	VCI_UART_EX_INV_BYTE_RX_TIMEOUT = VCI_ISO9141_INV_BYTE_RX_TIMEOUT,
	/** \brief x-baud initialisation failure, The x-baud request has 
	 * invalid data */
	VCI_UART_EX_XBAUD_INIT_INVAL = VCI_ISO9141_XBAUD_INIT_INVAL,
	/** \brief Message length error */
	VCI_UART_EX_MSG_LEN_ERR = VCI_ISO9141_MSG_LEN_ERR,
	/** \brief x-baud initialisation failure, sync btye incorrect */
	VCI_UART_EX_SYNC_BYTE_RX_INVALID = VCI_ISO9141_SYNC_BYTE_RX_INVALID,
	/** \brief x-baud initialisation failure, KB1 incorrect */
	VCI_UART_EX_KEY_BYTE1_RX_INVALID = VCI_ISO9141_KEY_BYTE1_RX_INVALID,
	/** \brief x-baud initialisation failure, KB2 incorrect */
	VCI_UART_EX_KEY_BYTE2_RX_INVALID = VCI_ISO9141_KEY_BYTE2_RX_INVALID,
	/** \brief TX timeout failure (generic) */
	VCI_UART_EX_TX_TIMEOUT,
	/** \brief TX loopback timeout failure */
	VCI_UART_EX_TX_LOOPBACK_TIMEOUT,
	/** \brief TX ACK timeout failure */
	VCI_UART_EX_TX_ACK_TIMEOUT,
	/** \brief TX loopback did not match transmitted data */
	VCI_UART_EX_BAD_TX_ECHO,
	/** \brief TX ACK did not match that expected */
	VCI_UART_EX_BAD_TX_ACK,
	/** \brief RX ACK timeout failure */
	VCI_UART_EX_RX_ACK_TIMEOUT,
	/** \brief RX incorrect ETX */
	VCI_UART_EX_BAD_ETX_RX,
	/** \brief TX incorrect ETX */
	VCI_UART_EX_BAD_ETX_TX,
	/** \internal \brief Maximum ISO9141 protocol event number */
	VCI_UART_EX_EVENT_MAX
} ;

/** \brief Values for bitfield in VCI_FIOSET_CONTROLLER_NOTIFICATION IOCTL */
#define VCI_UART_CD_TX_COLLISION	(1<< 0)		/**< \brief Enable collision detection events */
#define VCI_UART_CD_UART_BRK		(1<< 1)		/**< \brief Enable UART break detection events */
#define VCI_UART_CD_UART_FRAMING_ERR	(1<< 2)		/**< \brief Enable UART framing error events */
#define VCI_UART_CD_UART_PARITY_ERR	(1<< 3)		/**< \brief Enable UART parity error events */
#define VCI_UART_CD_UART_OVERRUN_ERR	(1<< 4)		/**< \brief Enable UART RX overrun error events */

/** \brief Values for bitfield in VCI_FIOSET_CONTROLLER_NOTIFICATION IOCTL */
#define VCI_UART_EX_BRK		(1<< 1)		/**< \brief Enable UART break detection events */
#define VCI_UART_EX_FRAMING_ERR	(1<< 2)		/**< \brief Enable UART framing error events */
#define VCI_UART_EX_PARITY_ERR	(1<< 3)		/**< \brief Enable UART parity error events */
#define VCI_UART_EX_OVERRUN_ERR	(1<< 4)		/**< \brief Enable UART RX overrun error events */

/** \brief Different types of 5 baud wakeup */
typedef enum VCI_UART_XBAUD_MODES {
	VCI_UART_XBAUD_MODE_0,	/**< \brief Initialization as defined in ISO 9141-2 and ISO 14230-4 */
	VCI_UART_XBAUD_MODE_1,	/**< \brief ISO 9141 initialization followed by interface sending inverted Key Byte 2 */
	VCI_UART_XBAUD_MODE_2,	/**< \brief ISO 9141 initialization followed by ECU sending inverted address */
	VCI_UART_XBAUD_MODE_3,	/**< \brief Initialization as defined in ISO 9141 no inverted key byte 2 nor inverted address*/
	VCI_UART_XBAUD_MODE_MAX
} e_VCI_UART_XBAUD_MODES;

/** \brief Different combinations of comms lines for 5 baud wakeup */
typedef enum VCI_UART_XBAUD_LINES {
	VCI_UART_XBAUD_LINE_BOTH = 0,	/* Initialization on both K & L Lines */
	VCI_UART_XBAUD_LINE_PRIMARY,	/* Initialization on K-Line Only */
	VCI_UART_XBAUD_LINE_SECONDARY	/* Initialization on L-Line Only */
} e_VCI_UART_XBAUD_LINES;



/******************************************************************************************************************
 * WRITE FLAGS
 ******************************************************************************************************************/

#define UART_WAIT_P3_MIN_ONLY		(0x00000100)		/**< \internal \brief TODO: implement */

/**
 * \brief Initiates the fast intitialisation 
 * 
 * Use this flag in the vci_record_hdr_s::flags field to initiate a fastinit wakeup sequence. The start 
 * communication service request should be stored in vci_record_s::data of the request initiating the wakeup. The 
 * fast init wakeup pulse shall be generated prior to transmitting the start communication request.
 */
#define VCI_UART_FASTINIT               (0x00000200)
  
/**
 * \brief Initiates the x-baud intitialisation sequence using the address byte contained in the request 
 * 
 * Use this flag in the vci_record_hdr_s::flags field to initiate an x-baud wakeup sequence. The address should be 
 * stored as the first byte in vci_record_s::data of the request initiating the wakeup. The x-baud wakeup sequence 
 * shall be generated when this request is processed.  No baudrate detection is performed if only the address byte
 * is contained in the vci_record, instead the configured baudrate will be used. To detect the baudrate for this 
 * particular initialisation, add an extra non-zero byte in vci_record_s::data.
 */
#define VCI_UART_XBAUDINIT              (0x00000400)
#define VCI_UART_INITMASK               (VCI_UART_FASTINIT|VCI_UART_XBAUDINIT) /**< \brief Mask for all supported UART ECU initialisation sequences */


/*********************************************************************
 * PROTOCOL SPECIFIC DATA RECORD STRUCTURES
 *********************************************************************/

/**
 * \brief Data record cast for generic vci_data_record payload with collision based UART protocol
 * 
 * This structure is used to define the content of a data record that
 * incorporates collision detection protocol data (VCI_RECORD_SUB_TYPE_COLLISION_DETECT).
 */
struct vci_uart_data_collision_det {
	uint32_t	access_time;	/* Delay before transmitting (in bit times) */
#ifndef _WIN32
	uint8_t	data[];		/* VCI data payload.  Variable length record */
#endif
} _PACKED_;

/** \brief typedef of struct vci_uart_data_access_time */
typedef struct vci_uart_data_collision_det vci_uart_data_collision_det_s; 
/** \brief Return the payload length of a vci_uart_data_collision_det record */
#define VCI_UART_ACCES_TIME_DATA_PAYLOAD_LEN(d)	\
	(VCI_DATA_PAYLOAD_LEN(d) - sizeof(vci_uart_data_collision_det_s))

/*****************************************************************************/
/* FUNCTION : vci_data_record_collision_det_constructor */
/*!
 * \brief PURPOSE : Completes the fields of a vci collision detection data record
 *
 * DESCRIPTION:	Fills in the fields of a vci collision detection data record based
 *		on the supplied parameters.
 *
 \param	_vci_rec	Pointer to a vci_event_record structure
 \param	_flags		Flags
 \param	_user_data	Value for user data in the VCI record header
 \param _payload	Data record payload length in bytes
 \param _payload_ptr	Pointer to the data to copy to the record, may be NULL
 \param _access_time	Delay before transmitting on bus (in bit times)
 */
#define vci_data_record_collision_det_constructor(			\
		_vci_rec, _flags, _user_data, _payload, _payload_ptr,	\
		_access_time)						\
{									\
	vci_data_record_s* _vci_data_rec = (vci_data_record_s*)_vci_rec;\
	vci_uart_data_collision_det_s* _vci_data_rec_acc_time = 	\
		(vci_uart_data_collision_det_s *)_vci_data_rec->data;	\
	vci_record_hdr_s* _hdr_record;					\
	_hdr_record = (vci_record_hdr_s*)_vci_data_rec;			\
	_hdr_record->sub_type = VCI_RECORD_SUB_TYPE_COLLISION_DETECT;	\
	vci_record_constructor( _vci_rec, VCI_DATA, _user_data,		\
			VCI_DATA_DATA_SZ + _payload +			\
			sizeof(vci_uart_data_collision_det_s));		\
	_vci_data_rec->flags = htonl( _flags );				\
	_vci_data_rec->timestamp = htonl( do_gettimestamp );		\
	_vci_data_rec_acc_time->access_time = htonl(_access_time);	\
	if( _payload_ptr )						\
		memcpy( _vci_data_rec_acc_time->data, _payload_ptr,	\
		_payload  );						\
}

#endif
