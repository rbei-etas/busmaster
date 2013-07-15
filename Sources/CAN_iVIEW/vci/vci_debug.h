/******************************************************************************
 *
 * \file	vci_debug.h
 * \brief	VCI Debug definitions for the VCI protocol drivers
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

#ifndef VCI_DEBUG_H
#define VCI_DEBUG_H

/* define to 1 to enable debug support */
#ifndef ENABLE_VCI_DEBUG 
#define ENABLE_VCI_DEBUG		1
#endif

/* error debug - probably don't want to turn this off! */
#ifndef ENABLE_VCI_ERROR_DEBUG 
#define ENABLE_VCI_ERROR_DEBUG	1
#endif

/* debug enabler bitmasks */

#define DBG_TX			(0x00000001)		/* Transmit messages (non-ISR) e.g. write method */
#define DBG_RX			(0x00000002)		/* Receive messages (non-ISR) */
#define DBG_RX_INT		(0x00000004)		/* Receive messages (ISR) */
#define DBG_TX_INT		(0x00000008)		/* Transmit complete (ISR) */
#define DBG_TX_ECHO_INT		(0x00000010)		/* Transmit echo (ISR) - where enabled */
#define DBG_STATUS_INT		(0x00000020)		/* Status interrupts (ISR) - includes error interrupts */
#define DBG_TX_QUEUE		(0x00000040)		/* Transmit queues */
#define DBG_RX_QUEUE		(0x00000080)		/* Receive queues */
#define DBG_CONFIG		(0x00000100)		/* Configuration information */
#define DBG_FILTER		(0x00000200)		/* Filter information */
#define DBG_TX_SM		(0x00000400)		/* Tx state machine */
#define DBG_RX_SM		(0x00000800)		/* Rx state machine */
#define DBG_TX_PROTO		(0x00001000)		/* Tx protocol */
#define DBG_RX_PROTO		(0x00002000)		/* Rx protocol */
#define DBG_RX_NOTIFIER		(0x00004000)		/* Rx notification */
#define DBG_VCI_QUEUES 		(0x00008000)		/**< \brief Use to check for mem leaks caused by vci_queue_* functions */
#define DBG_PROFILE 		(0x40000000)		/* Profile function timing.  Typically ISRs and bottom halves */
#define DBG_POLL		(0x80000000)		/* Poll/select */


#if ENABLE_VCI_DEBUG
/* maskable debug printk macro - note: prints function name for you */
#  define DPRINTK(mask, debug, fmt, args...) if (mask & debug) printk( KERN_DEBUG "%s: " fmt, __FUNCTION__ , ## args)
#else
#  define DPRINTK(mask, debug, fmt, args...)
#endif

/* maskable printk macro */
#if ENABLE_VCI_DEBUG
#  define MPRINTK(mask, debug, fmt, args...) if (mask & debug) printk( KERN_DEBUG fmt, ## args)
#else
#  define MPRINTK(mask, debug, fmt, args...)
#endif

/* basic printk macro */
#if ENABLE_VCI_DEBUG
#  define PRINTK(fmt, args...) printk(fmt, ## args)
#else
#  define PRINTK(fmt, args...)
#endif

/* error debug print macro */
#if ENABLE_VCI_ERROR_DEBUG
#  define EPRINTK(fmt, args...) printk( KERN_ERR __FILE__  ": " fmt, ## args)
#else
#  define EPRINTK(fmt, args...)
#endif


/* define to 1 to disable lightweight runtime debugging checks */
#undef VCI_DRIVER_NDEBUG

#ifdef VCI_DRIVER_NDEBUG
#  define assert(expr) do {} while (0)
#else
#  define assert(expr) \
        if(!(expr)) {					\
        printk( "Assertion failed! %s,%s,%s,line=%d\n",	\
        #expr,__FILE__,__FUNCTION__,__LINE__);		\
        }
#endif

/* macro to display a message - requires a 'u32 debug' bitmask in the module */
/*
 * e.g. DISPLAY_MSG( DBG_TX, priv->debug, "can", priv->chan, canh_dst, skb->len, skb->h.raw , true, "tx" ) ;
 */

#define DISPLAY_MSG( mask, debug, dev, chan, id, len, buf, ext, info ) \
{ \
	if ( mask & debug ) \
	{ \
		int i ; \
		u8 c = (ext) ? 'x' : ' ' ; \
		PRINTK( "%s[%d] %s 0x%x%c: ", dev, chan, info, id, c ) ; \
		for ( i = 0 ; i < len ; i++ ) \
		{ \
			PRINTK( "%02x ", (u8) buf[i] ) ; \
		} \
		PRINTK( "\n" ) ; \
	} \
} ;


#endif	/* VCI_DEBUG_H */
