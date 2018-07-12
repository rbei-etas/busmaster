/*****************************************************************************
 *
 * \file    vci_filter.h
 * \brief   VCI Filter definitions for the VCI protocol drivers
 *      IOCTL interface for the T-MVCI
 *
 * Copyright (C) 2013 Bosch Automotive Service Solutions
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 *****************************************************************************/

#ifndef _VCI_FILTER_H
#define _VCI_FILTER_H

#ifndef _VCI_H
#error "Do not include this file directly, include top level vci.h"
#endif

/* User visible stuff. Glibc provides its own but libc5 folk will use these */

#define VCI_FILTER_SZ       12  /**< \brief Number of bytes in filter; includes all headers e.g. 32-bit CAN addr; must be 32-bit aligned */


/**
 * TODO: docs
 */
union filter_msg
{
    uint8_t _u8[VCI_FILTER_SZ];     /**< \brief TODO: docs */
    uint16_t _u16[(VCI_FILTER_SZ/sizeof(uint16_t))];    /**< \brief probably won't be used */
    uint32_t _u32[(VCI_FILTER_SZ/sizeof(uint32_t))];    /**< \brief TODO: docs */
} ;

typedef union filter_msg    filter_msg_u;

#define VCI_PASS_FILTER     0x1
#define VCI_BLOCK_FILTER    0x2
#define VCI_FLOW_CONTROL_FILTER 0x3

#define VCI_FILTER_DELETE_ALL   (-1)

/**
 * \brief TODO: docs
 */
struct vci_filter
{
    uint32_t    index;      /**< \brief TODO: docs */
    uint32_t    type;       /**< \brief TODO: docs */
    uint32_t    flags;      /**< \brief TODO: docs */
    filter_msg_u    mask;       /**< \brief TODO: docs */
    filter_msg_u    data;       /**< \brief pattern */
    filter_msg_u    flow;       /**< \brief TODO: docs */
    uint8_t     params[VCI_FILTER_SZ* sizeof(uint32_t)];
} _PACKED_ ;

typedef struct vci_filter   vci_filter_s;

/**
 * \brief Filter vci config record structure
 *  vci_config_record_s vci config record header;
 *  vci_filter_s        data;
 */
struct vci_filter_config_record
{
    vci_config_record_s config_record;
    vci_filter_s        data;
} _PACKED_ ;

typedef struct vci_filter_config_record vci_filter_config_record_s;

#define VCI_FILTER_CONFIG_RECORD_SZ sizeof( vci_filter_config_record_s )
#define VCI_FILTER_CONFIG_DATA_SZ   sizeof( vci_filter_s )

#define vci_filter_config_record_constructor( \
        _vci_rec, _user_data, \
        _type, _flags, \
        _mask, _masklen, _data, _datalen, _flow, _flowlen ) \
{ \
    vci_filter_s* _vci_filter =\
                               &((vci_filter_config_record_s*)_vci_rec)->data; \
    vci_config_request_record_constructor( _vci_rec, VCI_FIOADDFILTER, \
                                           _user_data, VCI_FILTER_CONFIG_DATA_SZ ); \
    _vci_filter->type = htonl( _type ); \
    _vci_filter->flags = htonl( _flags ); \
    memcpy(&_vci_filter->mask,_mask,min(_masklen,sizeof(filter_msg_u)));\
    memcpy(&_vci_filter->data,_data,min(_datalen,sizeof(filter_msg_u)));\
    memcpy(&_vci_filter->flow,_flow,min(_flowlen,sizeof(filter_msg_u)));\
}

#endif      /* _VCI_FILTER_H */
