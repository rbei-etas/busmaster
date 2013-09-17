/*****************************************************************************
 *
 * \file    vci_adc.h
 * \brief   VCI char driver interface definitions for the T-MVCI
 *
 * Copyright (C) 2013 Bosch Automotive Service Solutions
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 *****************************************************************************/

#ifndef _VCI_ADC_H
#define _VCI_ADC_H

#ifndef _VCI_H
#error "Do not include this file directly, include top level vci.h"
#endif

/**
 * \brief VCI ADC filter modes
 *
 *  VCI_ADC_FILTER_NONE
 *      Apply no filtering
 *  VCI_ADC_FILTER_AVERAGE
 *      Average the ADCs over n samples. n is given in the filter
 *      parameter. This will also have the effect of reducing the
 *      effective sample rate. e.g. 1000 samples at a rate of 1KHz
 *      with an average of 10 will take 10 times longer as 10000
 *      samples will have to read from the ADC to return 1000
 *      to the user.
 *
 */
enum adc_filter_mode_e
{
    VCI_ADC_FILTER_NONE,
    VCI_ADC_FILTER_AVERAGE,
    /* max */
    VCI_ADC_FILTER_MAX
} ;

/**
 * \brief VCI ADC units
 *
 *  VCI_ADC_RAW
 *      Apply no post processing
 *  VCI_ADC_MILLIVOLTS
 *      Process the samples and return the values as millivolts
 *
 */
enum adc_units_e
{
    VCI_ADC_RAW,
    VCI_ADC_MILLIVOLTS,         /* Note. On NGFTT ADC_NGFTT_CABLE_IDENT_CHAN is reported in Ohms */
    VCI_ADC_RAW_WITH_HEADER,        /* Store ADC channel number (4 bytes) in first sample position */
    VCI_ADC_MILLIVOLTS_WITH_HEADER,     /* Store ADC channel number (4 bytes) in first sample position */

    /* max */
    VCI_ADC_UNITS_MAX
} ;

/**
 * \brief VCI ADC Configuration ioctl structure
 *
 *  total_samples
 *      This is the number of samples that will be returned to the user
 *      in the vci adc data records resulting from this request
 *
 *      On NGFTT a value of zero generates a continuous sample stream. In continuous
 *      mode the trigger count (number of samples per continuous stream)
 *      should be set in the block_size parameter below.
 *  block_size
 *      This is the maximum number of samples that will be returned
 *      in each vci adc data record. The last data record from a
 *      request will contain the remainder.
 *      Maximum block_size is 512 samples.
 *
 *      On NGFTT a value of zero is used to stop continuous sample stream mode,
 *      only if total_samples value is also zero (continuous mode).
 *  bits_per_sample
 *      If configurable, the number of bits to use for each sample.
 *      This dependant upon the actual ADC hardware used.
 *      The configuration response will contain the actual bits used.
 *      This may or may not be the same as the request.
 *      Zero is used to indicate the driver should use its default.
 *  sample_rate
 *      If configurable, the number of samples per second.
 *      This dependant upon the actual ADC hardware used.
 *      The configuration response will contain the actual sample rate
 *      used. This may or may not be the same as the request.
 *      Zero is used to indicate the driver should use its default.
 *  units
 *      Sample data post processing. Indicated how the  sample data
 *      should be processed before being returned to the user.
 *      See description above
 *  filter_type
 *      Sample data post processing. Perform one of the data processing
 *      operations described above
 *  filter_parameter
 *      Parameter for the filter type operation, described above.
 *  adc_channel
 *      On T200 (8051 SC) the adc channel number. (0..25 shown on schematics as 8051_ADCx).
 *      All channels reporting in mV, except for channels 4 (NTC Battery Temperture in degree C x 10 )
 *      and 26 (internal 8051 temperature in degree C x 100)
 *
 *      On NGFTT the adc channel number. (0..15 shown on schematics as VMEAS_x,
 *      i.e. adc_channel 0 = J1962 Pin1 is VMEAS_1).
 *      All channels reporting in mV or raw adc values (depending on 'units' parameter), except for
 *      ADC_NGFTT_CABLE_IDENT_CHAN (Cable ID) ADC channel 4 which will returned in ohms if 'units'
 *      parmameter set to VCI_ADC_MILLIVOLTS.
 *
 */
struct vci_adc_config
{
    uint32_t        total_samples ;
    uint32_t        block_size;
    uint32_t        bits_per_sample ;
    uint32_t        sample_rate ;
    uint32_t        units ;
    uint32_t        filter_type ;
    uint32_t        filter_parameter ;
    uint32_t        adc_channel ;
} _PACKED_ ;

typedef struct vci_adc_config   vci_adc_config_s;

/**
 * \brief ADC configuation record structure
 */
struct vci_adc_config_record
{
    vci_config_record_s config_record;
    vci_adc_config_s    data;
};

typedef struct vci_adc_config_record    vci_adc_config_record_s;

/**
 * \brief ADC data record structure
 */
struct vci_adc_data_record
{
    vci_data_record_s   data_record;
    uint32_t            samples[];
};

typedef struct vci_adc_data_record  vci_adc_data_record_s;

#define VCI_ADC_CONFIG_RECORD_SZ    sizeof( vci_adc_config_record_s )
#define VCI_ADC_CONFIG_DATA_SZ      sizeof( vci_adc_config_s )

#define vci_adc_config_request_record_constructor( \
        _vci_rec, _user_data, \
        _total_samples, _block_size, _bits_per_sample, _sample_rate, \
        _units, _filter_type, _filter_parameter, _adc_channel ) \
{ \
    vci_adc_config_s* _vci_adc=&((vci_adc_config_record_s*)_vci_rec)->data;\
    vci_config_request_record_constructor( (vci_config_record_s*)_vci_rec,\
                                           VCI_FIOADCCONFIG, _user_data, VCI_ADC_CONFIG_DATA_SZ ); \
    _vci_adc->total_samples = htonl( _total_samples ); \
    _vci_adc->block_size = htonl( _block_size ); \
    _vci_adc->bits_per_sample = htonl( _bits_per_sample ); \
    _vci_adc->sample_rate = htonl( _sample_rate ); \
    _vci_adc->units = htonl( _units ); \
    _vci_adc->filter_type = htonl( _filter_type ); \
    _vci_adc->filter_parameter = htonl( _filter_parameter ); \
    _vci_adc->adc_channel = htonl( _adc_channel ); \
}

/* T200 J1962 pin to ADC channel */

#define ADC_VPROG_4MM_CHAN      (6)
#define ADC_J1962_PIN_01_CHAN       (7)
#define ADC_J1962_PIN_02_CHAN       (8)
#define ADC_J1962_PIN_03_CHAN       (9)


#define ADC_J1962_PIN_06_CHAN       (11)
#define ADC_J1962_PIN_07_CHAN       (12)
#define ADC_J1962_PIN_08_CHAN       (13)
#define ADC_J1962_PIN_09_CHAN       (14)
#define ADC_J1962_PIN_10_CHAN       (15)
#define ADC_J1962_PIN_11_CHAN       (16)
#define ADC_J1962_PIN_12_CHAN       (17)
#define ADC_J1962_PIN_13_CHAN       (18)
#define ADC_J1962_PIN_14_CHAN       (19)
#define ADC_J1962_PIN_15_CHAN       (20)
#define ADC_J1962_PIN_16_CHAN       (3)

/* NGFTT J1962 pin to ADC channel */

#define ADC_NGFTT_J1962_PIN_01_CHAN (0)
#define ADC_NGFTT_J1962_PIN_02_CHAN (1)
#define ADC_NGFTT_J1962_PIN_03_CHAN (2)
#define ADC_NGFTT_J1962_PIN_04_CHAN (3)

#define ADC_NGFTT_CABLE_IDENT_CHAN  (4)

#define ADC_NGFTT_J1962_PIN_06_CHAN (5)
#define ADC_NGFTT_J1962_PIN_07_CHAN (6)
#define ADC_NGFTT_J1962_PIN_08_CHAN (7)
#define ADC_NGFTT_J1962_PIN_09_CHAN (8)
#define ADC_NGFTT_J1962_PIN_10_CHAN (9)
#define ADC_NGFTT_J1962_PIN_11_CHAN (10)
#define ADC_NGFTT_J1962_PIN_12_CHAN (11)
#define ADC_NGFTT_J1962_PIN_13_CHAN (12)
#define ADC_NGFTT_J1962_PIN_14_CHAN (13)
#define ADC_NGFTT_J1962_PIN_15_CHAN (14)
#define ADC_NGFTT_J1962_PIN_16_CHAN (15)

/**
 * \brief VProg Definitions.
 *
 */

/* voltage ranges */
#define VPROG_VOLTAGE_OFF               0
#define VPROG_MIN_mV                    5000
#define VPROG_MAX_mV                    20000

/**
 * \brief VCI vprog Configuration ioctl structure
 *
 */
struct vci_vprog_config
{
    uint32_t    channel ;
    uint32_t    volt ;
} _PACKED_ ;

typedef struct vci_vprog_config vci_vprog_config_s;

/**
 * \brief vprog configuation record structure
 */
struct vci_vprog_config_record
{
    vci_config_record_s config_record;
    vci_vprog_config_s  data;
};

typedef struct vci_vprog_config_record  vci_vprog_config_record_s;

#define VCI_VPROG_CONFIG_RECORD_SZ  sizeof( vci_vprog_config_record_s )
#define VCI_VPROG_CONFIG_DATA_SZ    sizeof( vci_vprog_config_s )

#define vci_vprog_config_request_record_constructor( \
        _vci_rec, _user_data, \
        _channel, _volt ) \
{ \
    vci_vprog_config_s* _vci_vprog=\
                                   &((vci_vprog_config_record_s*)_vci_rec)->data;\
    vci_config_request_record_constructor( _vci_rec, VCI_FIOVPROG, \
                                           _user_data, VCI_VPROG_CONFIG_DATA_SZ ); \
    _vci_vprog->channel = htonl( _channel ); \
    _vci_vprog->volt = htonl( _volt ); \
}

#endif
