/*****************************************************************************/
/* MODULE : vci */
/*!
* \brief PURPOSE : VCI periodic message definitions for the T-MVCI char driver
* 
*
* usr/local/include/vci_periodic.h
* 
* 	
*
* DESCRIPTION : TODO: detailed description incl author and licence/copyright?
*
*/
/*****************************************************************************/

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
	uint8_t	_u8[PERIODIC_SZ];			/**< \brief TODO: docs */
	uint16_t _u16[(PERIODIC_SZ/sizeof(uint16_t))];	/**< \brief probably won't be used */
	uint32_t _u32[(PERIODIC_SZ/sizeof(uint32_t))];	/**< \brief TODO: docs */
} ;

typedef union periodic_msg		periodic_msg_s;

/**
 * \brief Userspace/kernel access of periodic data using ioctl VCI_FIOADDPERIODIC 
 */
struct vci_periodic_record 
{
	uint32_t	id;		/**< \brief TODO: docs */
	uint32_t	period ;	/**< \brief TODO: docs */
	uint32_t	flags ; 	/**< \brief TODO: docs */
	uint32_t	data_length ; 	/**< \brief TODO: docs */
	periodic_msg_s	data ;		/**< \brief TODO: docs */
} _PACKED_  ;

typedef struct vci_periodic_record	vci_periodic_record_s;

#endif


