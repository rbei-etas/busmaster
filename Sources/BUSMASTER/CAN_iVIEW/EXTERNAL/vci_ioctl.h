/*****************************************************************************
 *
 * \file    vci_ioctl.h
 * \brief   VCI IOCtl value definitions for the VCI protocol drivers
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

#ifndef _VCI_IOCTL_H
#define _VCI_IOCTL_H
/*
 * RPC autogen files do not need to included the structures as they will
 * clash, they only need the IOCTL number definitions
 */
#ifdef _VCI_H
#include "vci_filter.h"
#include "vci_periodic.h"
#include "vci_bus.h"
#include "vci_can.h"
#include "vci_uart.h"
#include "vci_j1850.h"
#include "vci_adc.h"
#include "vci_ui.h"
#endif

/*
 * IOCTL definitions depend on macros defined sys/ioctl.h
 */
#ifdef __CYGWIN__
#define __USE_LINUX_IOCTL_DEFS
#endif
#if !defined( _WIN32 ) && !defined( __KERNEL__ )
#include <sys/ioctl.h>
#endif

/*
 * The following #define is required to avoid C++ compilation errors that
 * appear to relate to the fact that for compiler version 3.4 and later,
 * #defines that incorporate IOW ** CANNOT ** be used in switch statements.
 * This means that most VCI_ IOCTL definitions (e.g. VCI_FIOCONNECT) fail.
 */

#ifdef __cplusplus
#undef VCI_IOC_TYPECHECK
#define VCI_IOC_TYPECHECK(t) (sizeof(t))
#endif

/*
 * Size and Dir bit defintions differ between i386 and PPC.
 * We need cross platform conformity so we are using the
 * PPC comformant defintions for all VCI IOCTL commands
 */
#define VCI_IOC_SIZEBITS 13 /**< \internal \brief Size & Direction bits */
#define VCI_IOC_DIRBITS 3   /**< \internal \brief TSize & Direction bits */
#define VCI_IOC_NONE    1U  /**< \internal \brief Size & Direction bitss */
#define VCI_IOC_READ    2U  /**< \internal \brief Size & Direction bits */
#define VCI_IOC_WRITE   4U  /**< \internal \brief Size & Direction bits */

/*
 * The following is for compatibility across the various Linux
 * platforms.  The i386 ioctl numbering scheme doesn't really enforce
 * a type field.  De facto, however, the top 8 bits of the lower 16
 * bits are indeed used as a type field, so we might just as well make
 * this explicit here.  Please be sure to use the decoding macros
 * below from now on.
 */
#define VCI_IOC_NRBITS      8
#define VCI_IOC_TYPEBITS    8

#define VCI_IOC_NRMASK      ((1 << VCI_IOC_NRBITS)-1)
#define VCI_IOC_TYPEMASK    ((1 << VCI_IOC_TYPEBITS)-1)
#define VCI_IOC_SIZEMASK    ((1 << VCI_IOC_SIZEBITS)-1)
#define VCI_IOC_DIRMASK     ((1 << VCI_IOC_DIRBITS)-1)

#define VCI_IOC_NRSHIFT 0
#define VCI_IOC_TYPESHIFT   (VCI_IOC_NRSHIFT+VCI_IOC_NRBITS)
#define VCI_IOC_SIZESHIFT   (VCI_IOC_TYPESHIFT+VCI_IOC_TYPEBITS)
#define VCI_IOC_DIRSHIFT    (VCI_IOC_SIZESHIFT+VCI_IOC_SIZEBITS)

#define VCI_IOC(dir,type,nr,size) \
    (((dir)  << VCI_IOC_DIRSHIFT) | \
     ((type) << VCI_IOC_TYPESHIFT) | \
     ((nr)   << VCI_IOC_NRSHIFT) | \
     ((size) << VCI_IOC_SIZESHIFT))

/* provoke compile error for invalid uses of size argument */
extern unsigned int __invalid_size_argument_for_IOC;

/* used to create numbers */
#define VCI_IO(type,nr)         VCI_IOC(VCI_IOC_NONE,(type),(nr),0)
#define VCI_IOR(type,nr,size)       VCI_IOC(VCI_IOC_READ,(type),(nr),sizeof(size))
#define VCI_IOW(type,nr,size)       VCI_IOC(VCI_IOC_WRITE,(type),(nr),sizeof(size))
#define VCI_IOWR(type,nr,size)      VCI_IOC(VCI_IOC_READ|VCI_IOC_WRITE,(type),(nr),sizeof(size))
#define VCI_IOR_BAD(type,nr,size)   VCI_IOC(VCI_IOC_READ,(type),(nr),sizeof(size))
#define VCI_IOW_BAD(type,nr,size)   VCI_IOC(VCI_IOC_WRITE,(type),(nr),sizeof(size))
#define VCI_IOWR_BAD(type,nr,size)  VCI_IOC(VCI_IOC_READ|VCI_IOC_WRITE,(type),(nr),sizeof(size))

/* used to decode ioctl numbers.. */
#define VCI_IOC_DIR(nr)         (((nr) >> VCI_IOC_DIRSHIFT) & VCI_IOC_DIRMASK)
#define VCI_IOC_TYPE(nr)        (((nr) >> VCI_IOC_TYPESHIFT) & VCI_IOC_TYPEMASK)
#define VCI_IOC_NR(nr)          (((nr) >> VCI_IOC_NRSHIFT) & VCI_IOC_NRMASK)
#define VCI_IOC_SIZE(nr)        (((nr) >> VCI_IOC_SIZESHIFT) & VCI_IOC_SIZEMASK)

/* IOCTLs */
/* VCI  */

/**
 * \brief Max size of names
 *
 * Generally used throughout the T-MVCI interface as the size for human readable names
 */
#define VCI_NAME_SZ (32)
#define VCI_FIO_TYPE    'j'         /**< \internal \brief Magic # */

#define VCI_FIOFIRMWARE_VER         VCI_IOR(VCI_FIO_TYPE,   0,  char[VCI_NAME_SZ])
#define VCI_FIOCONNECT              VCI_IOW(VCI_FIO_TYPE,   1,  vci_bus_s)      /**< \brief TODO: docs */
#define VCI_FIODISCONNECT           VCI_IO(VCI_FIO_TYPE,    2)              /**< \brief TODO: docs */
#define VCI_FIOADDFILTER            VCI_IOWR(VCI_FIO_TYPE,  3,  vci_filter_s)       /**< \brief TODO: docs */
#define VCI_FIODELFILTER            VCI_IOW(VCI_FIO_TYPE,   4,  uint32_t)           /**< \brief TODO: docs */
#define VCI_FIOADDPERIODIC          VCI_IOWR(VCI_FIO_TYPE,  5,  vci_periodic_record_s)  /**< \brief TODO: docs */
#define VCI_FIODELPERIODIC          VCI_IOW(VCI_FIO_TYPE,   6,  uint32_t)           /**< \brief TODO: docs */

#define VCI_FIORESETPERIODIC            VCI_IOWR(VCI_FIO_TYPE,  7,  uint32_t)           /**< \brief TODO: docs */
#define VCI_FIODISABLEPERIODIC          VCI_IOWR(VCI_FIO_TYPE,  8,  uint32_t)           /**< \brief TODO: docs */

/**
 * \brief Configure a baudrate
 *
 * Configures a baudrate on the channel that makes the call
 */
#define VCI_FIOBAUD             VCI_IOW(VCI_FIO_TYPE,   9,  uint32_t)

/**
 * \brief Fetches the baudrate
 *
 * Fetches the baudrate that's used on the channel that queries it
 */
#define VCI_FIOGET_BAUD             VCI_IOR(VCI_FIO_TYPE,   9,  uint32_t)
#define VCI_FIOTXNOTIFY             VCI_IOW(VCI_FIO_TYPE,   10, uint32_t)   /**< \brief TODO: docs */
#define VCI_FIOGET_TXNOTIFY         VCI_IOR(VCI_FIO_TYPE,   10, uint32_t)   /**< \brief TODO: docs */
#define VCI_FIORXNOTIFY             VCI_IOW(VCI_FIO_TYPE,   11, uint32_t)   /**< \brief TODO: docs */
#define VCI_FIOGET_RXNOTIFY         VCI_IOR(VCI_FIO_TYPE,   11, uint32_t)   /**< \brief TODO: docs */
#define VCI_FIOLISTEN               VCI_IOWR(VCI_FIO_TYPE,  12, uint32_t)   /**< \brief TODO: docs */
#define VCI_FIOLOOPBACK             VCI_IOWR(VCI_FIO_TYPE,  13, uint32_t) /**< \internal \brief Do not call from userspace, for kernel use only */

/**
 * \brief Fetches the human readable channel name
 *
 * Can be used to make log messages more readable
 * \internal Use to reference the /proc filesystem
 */
#define VCI_FIOCHANNEL_NAME         VCI_IOR(VCI_FIO_TYPE,   14, char[VCI_NAME_SZ])
#define VCI_FIOTERM             VCI_IOW(VCI_FIO_TYPE,   15, uint32_t)   /**< \brief TODO: docs */
#define VCI_FIOGET_TERM             VCI_IOR(VCI_FIO_TYPE,   15, uint32_t)   /**< \brief TODO: docs */

/* CAN  */
#define VCI_FIOSP               VCI_IOW(VCI_FIO_TYPE,   16, uint32_t)   /**< \brief TODO: docs */
#define VCI_FIOGET_SP               VCI_IOR(VCI_FIO_TYPE,   16, uint32_t)   /**< \brief TODO: docs */
#define VCI_FIOSJW              VCI_IOW(VCI_FIO_TYPE,   17, uint32_t)   /**< \brief TODO: docs */
#define VCI_FIOGET_SJW              VCI_IOR(VCI_FIO_TYPE,   17, uint32_t)   /**< \brief TODO: docs */

/**
 * \brief Set CAN frame padding Specified byte to be used for unused bytes, in all ISO15765 frame transmissions
 *
 * TDS J2534 Extension PADDING_BYTE, Range 0-0xFF, Default Value 0x00
 * \internal Use to Set Frame PADDING_BYTE value in ISO15765 Driver
 */
#define VCI_FIOPADBYTE              VCI_IOW(VCI_FIO_TYPE,   18, uint32_t)

/**
 * \brief Get CAN frame padding Specified byte
 *
 * TDS J2534 Extension PADDING_BYTE, Range 0-0xFF, Default Value 0x00
 * \internal Use to Get Frame PADDING_BYTE value from ISO15765 Driver
 */
#define VCI_FIOGET_PADBYTE          VCI_IOR(VCI_FIO_TYPE,   18, uint32_t)

/************************************************************************************************/
/* ISO15765 specific IOCTLs Section                                 */
/************************************************************************************************/

/**
 * \brief Set ISO15765 specific value for Block Size the interface should report to the vehicle for receiving seqmented transfers
 *
 * J2534 SET_CONFIG value ISO15765_BS, Range 0-0xFF, Units are ISO15765-2, Default Value 0
 * \internal Use to Set RX Block Size in ISO15765 Driver
 */
#define VCI_FIOBSRX             VCI_IOW(VCI_FIO_TYPE,   19, uint32_t)

/**
 * \brief Get ISO15765 specific value for Block Size the interface should report to the vehicle for receiving seqmented transfers
 *
 * J2534 GET_CONFIG value ISO15765_BS, Range 0-0xFF, Units are ISO15765-2, Default Value 0
 * \internal Use to Get RX Block Size from ISO15765 Driver
 */
#define VCI_FIOGET_BSRX             VCI_IOR(VCI_FIO_TYPE,   19, uint32_t)

/**
 * \brief Set ISO15765 specific value for STMIN RX override (sets the separation time the interface should report to the vehicle for Rx seqmented messages)
 *
 * J2534 SET_CONFIG value ISO15765_STMIN, Range 0-0xFF, Units are ISO15765-2, Default Value 0x00
 * \internal Use to Set RX STMIN value in ISO15765 Driver
 */
#define VCI_FIOSTMINRX              VCI_IOW(VCI_FIO_TYPE,   20, uint32_t)

/**
 * \brief Get ISO15765 specific value for STMIN RX override (sets the separation time the interface should report to the vehicle for Rx seqmented messages)
 *
 * J2534 GET_CONFIG value ISO15765_STMIN, Range 0-0xFF, Units are ISO15765-2, where where 0-7F = 0-127 mS, F1-F9 = 100uS - 900uS, Default Value 0x00
 * \internal Use to Set RX STMIN value from ISO15765 Driver
 */
#define VCI_FIOGET_STMINRX          VCI_IOR(VCI_FIO_TYPE,   20, uint32_t)

/**
 * \brief Set ISO15765 specific value for Block Size override the interface should use to transmit seqmented messages to the vehicle
 *
 * J2534 SET_CONFIG value BS_TX, Range 0-0xFF, 0xFFFF (use value reported by vehicle), Units are ISO15765-2, where where 0-7F = 0-127 mS, F1-F9 = 100uS - 900uS, Default Value 0xFFFF
 * \internal Use to Set TX Block Size in ISO15765 Driver
 */
#define VCI_FIOBSTX             VCI_IOW(VCI_FIO_TYPE,   21, uint32_t)

/**
 * \brief Get ISO15765 specific value for Block Size the interface should use to transmit seqmented messages to the vehicle
 *
 * J2534 GET_CONFIG value BS_TX, Range 0-0xFF, 0xFFFF (use value reported by vehicle), Units are ISO15765-2, Default Value 0xFFFF
 * \internal Use to Get TX Block Size from ISO15765 Driver
 */
#define VCI_FIOGET_BSTX             VCI_IOR(VCI_FIO_TYPE,   21, uint32_t)

/**
 * \brief Set ISO15765 specific value for STMIN TX override (separation time of interface transmited seqmented messages) / Set J1939  specific value for Timeout Tx Tr
 *
 * J2534 SET_CONFIG value STMIN_TX, Range 0-0xFF, 0xFFFF (use value reported by vehicle), Units are ISO15765-2, where where 0-7F = 0-127 mS, F1-F9 = 100uS - 900uS, Default Value 0xFFFF
 * \internal Use to Set TX STMIN value in ISO15765 Driver
 *
 * J1939:  SET macro for setting Timeout Tx Tr.
 *  Used to configure the time between sending the packets in multipacket , Broadcast and Destination specific messages.
 *  Tx Tr default value is 200 ms

 */
#define VCI_FIOSTMINTX              VCI_IOW(VCI_FIO_TYPE,   22, uint32_t)

/**
 * \brief Get ISO15765 specific value for STMIN TX override (separation time of interface transmited seqmented messages) / Get  J1939  specific value for Timeout Tx Tr
 *
 * J2534 GET_CONFIG value STMIN_TX, Range 0-0xFF, 0xFFFF (use value reported by vehicle), Units are ISO15765-2, where where 0-7F = 0-127 mS, F1-F9 = 100uS - 900uS, Default Value 0xFFFF
 * \internal Use to Get TX STMIN value from ISO15765 Driver
 *
 * J1939:  Het  macro for setting Timeout Tx Tr.
 *  Used to Get the time between sending the packets in multipacket , Broadcast and Destination specific messages.

 */
#define VCI_FIOGET_STMINTX          VCI_IOR(VCI_FIO_TYPE,   22, uint32_t)

/**
 * \brief Set ISO15765 specific value for N_WFTmax (Max number of WAIT flow control frames allowed during a multi-seqment transfer)
 *
 * J2534 SET_CONFIG value ISO15765_WFT_MAX, Range 0-0xFF, Default Value 0x00
 * \internal Use to Set WFT MAX value in ISO15765 Driver
 */
#define VCI_FIOWFTMAX               VCI_IOW(VCI_FIO_TYPE,   23, uint32_t)

/**
 * \brief Get ISO15765 specific value for N_WFTmax (Max number of WAIT flow control frames allowed during a multi-seqment transfer)
 *
 * J2534 GET_CONFIG value ISO15765_WFT_MAX, Range 0-0xFF, Default Value 0x00
 * \internal Use to Get WFT MAX value from ISO15765 Driver
 */
#define VCI_FIOGET_WFTMAX           VCI_IOR(VCI_FIO_TYPE,   23, uint32_t)

/**
 * \brief Set ISO15765 N_Cr (Consecutive Frame) Timeout
 *
 * TDS J2534 Extension ISO15765_N_CR_TIMEOUT, Range 0-0xFFFF (Scale 1 ms per bit), Default Value 1000mS
 * \internal Use to Set ISO15765 Driver N_Cr Timeout value, Scaled from 1mS per Bit to uS (used by the driver)
 */
#define VCI_FION_CR_TIMEOUT         VCI_IOW(VCI_FIO_TYPE,   24, uint32_t)

/**
 * \brief Get ISO15765 N_Cr (Consecutive Frame) Timeout
 *
 * TDS J2534 Extension ISO15765_N_CR_TIMEOUT, Range 0-0xFFFF (Scale 1 ms per bit), Default Value 1000mS
 * \internal Use to Get ISO15765 Driver N_Cr Timeout value, Scaled from uS (used by the driver) back to 1mS per Bit
 */
#define VCI_FIONGET_CR_TIMEOUT          VCI_IOR(VCI_FIO_TYPE,   24, uint32_t)

/**
 * \brief Set ISO15765 N_Br delay for tester transmission flow control frame
 *
 * TDS J2534 Extension ISO15765_N_BR_DELAY, Range 0-0xFFFF (Scale 1 ms per bit), Default Value 0mS
 * \internal Use to Set ISO15765 Driver N_Br delay value, Scaled from 1mS per Bit to uS (used by the driver)
 */
#define VCI_FION_BR_DELAY           VCI_IOW(VCI_FIO_TYPE,   25, uint32_t)

/**
 * \brief Get ISO15765 N_Br delay for tester transmission flow control frame
 *
 * TDS J2534 Extension ISO15765_N_BR_DELAY, Range 0-0xFFFF (Scale 1 ms per bit), Default Value 0mS
 * \internal Use to Get ISO15765 Driver N_Br delay value, Scaled from uS (used by the driver) back to 1mS per Bit
 */
#define VCI_FIONGET_BR_DELAY            VCI_IOR(VCI_FIO_TYPE,   25, uint32_t)

/**
 * \brief Set ISO15765 N_Bs timeout for reception of flow control frame
 *
 * TDS J2534 Extension ISO15765_N_BS_TIMEOUT, Range 0-0xFFFF (Scale 1 ms per bit), Default Value 1000mS
 * \internal Use to Set ISO15765 Driver N_Bs Timeout value, Scaled from 1mS per Bit to uS (used by the driver)
 */
#define VCI_FION_BS_TIMEOUT         VCI_IOW(VCI_FIO_TYPE,   26, uint32_t)

/**
 * \brief Get ISO15765 N_Bs timeout for reception of flow control frame
 *
 * TDS J2534 Extension ISO15765_N_BS_TIMEOUT, Range 0-0xFFFF (Scale 1 ms per bit), Default Value 1000mS
 * \internal Use to Get ISO15765 Driver N_Bs Timeout value, Scaled from uS (used by the driver) back to 1mS per Bit
 */
#define VCI_FIONGET_BS_TIMEOUT          VCI_IOR(VCI_FIO_TYPE,   26, uint32_t)

/**
 * \brief Fetches the human readable controller name
 *
 * Can be used to make log messages more readable
 * \internal Use to reference the /proc filesystem
 */
#define VCI_FIOCNTRLR_NAME          VCI_IOR(VCI_FIO_TYPE,   27,     char[VCI_NAME_SZ])

#define VCI_FIOUARTSET_FASTINIT         VCI_IOW(VCI_FIO_TYPE,   28, vci_uart_fastinit_s)    /**< \brief Configures a fast initialisation */
#define VCI_FIOUARTGET_FASTINIT         VCI_IOR(VCI_FIO_TYPE,   28, vci_uart_fastinit_s)    /**< \brief Fetches the fast initialisation configuration */
/**
 * \brief
 *
 * Use this ioctl to set the tx line state.
 */
#define VCI_FIOSET_TX_LINE_STATE            VCI_IOW(VCI_FIO_TYPE,   29,     uint32_t)
/**
 * \brief
 *
 * Use this ioctl to get the rx line state.
 */
#define VCI_FIOGET_RX_LINE_STATE            VCI_IOR(VCI_FIO_TYPE,   30,     uint32_t)

#define VCI_FIOUARTSET_XBAUD_INIT       VCI_IOW(VCI_FIO_TYPE,   31, vci_uart_xbaudinit_s)   /**< \brief Configures an x baud initialisation */
#define VCI_FIOUARTGET_XBAUD_INIT       VCI_IOR(VCI_FIO_TYPE,   31, vci_uart_xbaudinit_s)   /**< \brief Fetches x baud initialisation configuration  */
#define VCI_FIOUARTSET_RX_INTERBYTE     VCI_IOW(VCI_FIO_TYPE,   32, vci_minmax_s)       /**< \brief Sets the interbyte timing for incomming data */
#define VCI_FIOUARTGET_RX_INTERBYTE     VCI_IOR(VCI_FIO_TYPE,   32, vci_minmax_s)       /**< \brief Gets the interbyte timing for incomming data */
#define VCI_FIOUARTSET_TX_INTERBYTE     VCI_IOW(VCI_FIO_TYPE,   33, vci_minmax_s)       /**< \brief Sets the interbyte timing for outgoing data */
#define VCI_FIOUARTGET_TX_INTERBYTE     VCI_IOR(VCI_FIO_TYPE,   33, vci_minmax_s)       /**< \brief Gets the interbyte timing for outgoing data */
#define VCI_FIOUARTSET_RX_INTERPACKET       VCI_IOW(VCI_FIO_TYPE,   34, vci_minmax_s)       /**< \brief Sets the interpacket timing for incomming data */
#define VCI_FIOUARTGET_RX_INTERPACKET       VCI_IOR(VCI_FIO_TYPE,   34, vci_minmax_s)       /**< \brief Gets the interpacket timing for incomming data */
#define VCI_FIOUARTSET_TX_INTERPACKET       VCI_IOW(VCI_FIO_TYPE,   35, vci_minmax_s)       /**< \brief Sets the interpacket timing for outgoing data */
#define VCI_FIOUARTGET_TX_INTERPACKET       VCI_IOR(VCI_FIO_TYPE,   35, vci_minmax_s)       /**< \brief Gets the interpacket timing for outgoing data */

#define VCI_FIOCLEAR_TXBUFF         VCI_IO(VCI_FIO_TYPE,    36)             /**< \brief Clears the TX buffer */
#define VCI_FIOCLEAR_RXBUFF         VCI_IO(VCI_FIO_TYPE,    37)             /**< \brief Clears the RX buffer */

#define VCI_FIOUARTSET_RX_INTERBYTE_MIN     VCI_IOW(VCI_FIO_TYPE,   38, uint32_t)       /**< \brief Sets the minimum interbyte timing for incomming data */
#define VCI_FIOUARTGET_RX_INTERBYTE_MIN     VCI_IOR(VCI_FIO_TYPE,   38, uint32_t)       /**< \brief Gets the minimum interbyte timing for incomming data */
#define VCI_FIOUARTSET_RX_INTERBYTE_MAX     VCI_IOW(VCI_FIO_TYPE,   39, uint32_t)       /**< \brief Sets the maximum interbyte timing for incomming data */
#define VCI_FIOUARTGET_RX_INTERBYTE_MAX     VCI_IOR(VCI_FIO_TYPE,   39, uint32_t)       /**< \brief Gets the maximum interbyte timing for incomming data */
#define VCI_FIOUARTSET_TX_INTERBYTE_MIN     VCI_IOW(VCI_FIO_TYPE,   40, uint32_t)       /**< \brief Sets the minimum interbyte timing for outgoing data */
#define VCI_FIOUARTGET_TX_INTERBYTE_MIN     VCI_IOR(VCI_FIO_TYPE,   40, uint32_t)       /**< \brief Gets the minimum interbyte timing for outgoing data */
#define VCI_FIOUARTSET_TX_INTERBYTE_MAX     VCI_IOW(VCI_FIO_TYPE,   41, uint32_t)       /**< \brief Sets the maximum interbyte timing for outgoing data */
#define VCI_FIOUARTGET_TX_INTERBYTE_MAX     VCI_IOR(VCI_FIO_TYPE,   41, uint32_t)       /**< \brief Gets the maximum interbyte timing for outgoing data */
#define VCI_FIOUARTSET_RX_INTERPACKET_MIN   VCI_IOW(VCI_FIO_TYPE,   42, uint32_t)       /**< \brief Sets the minimum interpacket timing for incomming data */
#define VCI_FIOUARTGET_RX_INTERPACKET_MIN   VCI_IOR(VCI_FIO_TYPE,   42, uint32_t)       /**< \brief Gets the minimum interpacket timing for incomming data */
#define VCI_FIOUARTSET_RX_INTERPACKET_MAX   VCI_IOW(VCI_FIO_TYPE,   43, uint32_t)       /**< \brief Sets the maximum interpacket timing for incomming data */
#define VCI_FIOUARTGET_RX_INTERPACKET_MAX   VCI_IOR(VCI_FIO_TYPE,   43, uint32_t)       /**< \brief Gets the maximum interpacket timing for incomming data */
#define VCI_FIOUARTSET_TX_INTERPACKET_MIN   VCI_IOW(VCI_FIO_TYPE,   44, uint32_t)       /**< \brief Sets the minimum interpacket timing for outgoing data */
#define VCI_FIOUARTGET_TX_INTERPACKET_MIN   VCI_IOR(VCI_FIO_TYPE,   44, uint32_t)       /**< \brief Gets the minimum interpacket timing for outgoing data */
#define VCI_FIOUARTSET_TX_INTERPACKET_MAX   VCI_IOW(VCI_FIO_TYPE,   45, uint32_t)       /**< \brief Sets the maximum interpacket timing for outgoing data */
#define VCI_FIOUARTGET_TX_INTERPACKET_MAX   VCI_IOR(VCI_FIO_TYPE,   45, uint32_t)       /**< \brief Gets the maximum interpacket timing for outgoing data */

/**
 * \brief Sets the periodic message configuration
 *
 * TDS J2534 Extension PERIODIC_TIMER_RESET and PERIODIC_INDICATIONS, Use VCI_PERIODIC_NONE, VCI_PERIODIC_TX_NOTIFY, VCI_PERIODIC_TX_RESET and VCI_PERIODIC_MASK flags.
 * \internal Use to set Disable periodic tx notifications and Reset periodic timer on call to file write in ISO15765/CAN/UART Driver.
 */
#define VCI_FIOPERIODICCONFIG           VCI_IOW(VCI_FIO_TYPE,   46, uint32_t)

/**
 * \brief Gets the periodic message configuration
 *
 * TDS J2534 Extension PERIODIC_TIMER_RESET and PERIODIC_INDICATIONS, Use VCI_PERIODIC_NONE, VCI_PERIODIC_TX_NOTIFY, VCI_PERIODIC_TX_RESET and VCI_PERIODIC_MASK flags.
 * \internal Use to get the Disable periodic tx notifications and Reset periodic timer on call to file write from the ISO15765/CAN/UART Driver.
 */
#define VCI_FIOGET_PERIODICCONFIG       VCI_IOR(VCI_FIO_TYPE,   46, uint32_t)

/**
 * \brief Set UART frame format
 *
 * Use this ioctl to set the frame format used by the supported UART protocols
 */
#define VCI_FIOUARTSET_FRAME            VCI_IOW(VCI_FIO_TYPE,   47, vci_uart_frame_s)

/**
 * \brief Get UART frame format
 *
 * Use this ioctl to get the frame format used by the supported UART protocols
 */
#define VCI_FIOUARTGET_FRAME            VCI_IOR(VCI_FIO_TYPE,   47, vci_uart_frame_s)

/**
 * \brief
 *
 * Use this ioctl to transmit a BREAK symbol on to the bus.
 */
#define VCI_FIOTX_BREAK                 VCI_IOW(VCI_FIO_TYPE, 48, uint32_t)

/**
 * \brief
 *
 * Use this ioctl to set the arbitration timeout for J1850VPW/Class2.
 * MCH: TODO: change to VCI_FIOSET_TX_TIMEOUT for use by all drivers
 */
#define VCI_FIOSET_ARB_TIMEOUT          VCI_IOW(VCI_FIO_TYPE, 49, uint32_t)

/**
 * \brief Echo
 *
 * Use this ioctl to test the framework without the drivers getting involved
 */
#define VCI_FIOECHO         VCI_IOWR(VCI_FIO_TYPE, 50, uint32_t)

/**
 * \brief VCI ADC Configuration
 *
 * Use this ioctl to configure the VCI ADC driver and retrieve samples.
 * The samples will be returned asyncronously in a vci data record
 * See the description of the vci_adc_config structure for more information
 */
#define VCI_FIOADCCONFIG        VCI_IOWR(VCI_FIO_TYPE, 51, vci_adc_config_s)

/**
 * \brief
 *
 * Use this ioctl to Read Battery Current.
 * Mode :
 * 0 = reads bat current in mA
 * 1 = reads bat accumulator value in mAh
 *
 * See enum vci_battery_mode_current_types
 * Returns Battery Current reported by SC
 */
#define VCI_FIOBATTCURRENT              VCI_IOWR(VCI_FIO_TYPE, 52, uint32_t)

/**
 * \brief
 *
 * Use this ioctl to set beep config.
 * duration, frequency and volume
 */
#define VCI_FIOBEEPCONFIG           VCI_IOW(VCI_FIO_TYPE, 53, vci_beep_config_s)

/**
 * \brief
 *
 * Use this ioctl to set LED state
 * led, state
 *
 */
#define VCI_FIOLEDCONFIG            VCI_IOW(VCI_FIO_TYPE, 54, vci_led_config_s)


/**
 * \brief
 *
 * Use this ioctl to set VPROG
 * channel, volt
 *
 */
#define VCI_FIOVPROG                VCI_IOW(VCI_FIO_TYPE, 55, vci_vprog_config_s)

/**
 * \internal
 * \brief
 *
 * Use this ioctl to see what baudrate is achievable.
 */
#define VCI_FIOHASBAUD          VCI_IOW(VCI_FIO_TYPE, 56, uint32_t)

/**
 * \brief
 *
 * Use these ioctls to get/set the physical layer mode.
 */
#define VCI_FIOSET_PHYS_MODE                VCI_IOW(VCI_FIO_TYPE,   57,     phys_mode_e)
#define VCI_FIOGET_PHYS_MODE                VCI_IOR(VCI_FIO_TYPE,   57,     phys_mode_e)
/**
 * \brief Set ISO15765 N_As Timeout
 *
 * TDS J2534 Extension ISO15765_N_AS_TIMEOUT, Range 0-0xFFFF (Scale 1 ms per bit), Default Value 1000mS
 * \internal Use to Set ISO15765 Driver N_As Timeout value, Scaled from 1mS per Bit to uS (used by the driver)
 */
#define VCI_FION_AS_TIMEOUT         VCI_IOW(VCI_FIO_TYPE,   58, uint32_t)

/**
 * \brief Get ISO15765 N_As Timeout
 *
 * TDS J2534 Extension ISO15765_N_AS_TIMEOUT, Range 0-0xFFFF (Scale 1 ms per bit), Default Value 1000mS
 * \internal Use to Get ISO15765 Driver N_As Timeout value, Scaled from uS (used by the driver) back to 1mS per Bit

*/
#define VCI_FIONGET_AS_TIMEOUT          VCI_IOR(VCI_FIO_TYPE,   58, uint32_t)
/**
 * \brief Set CAN frame padding to be used for unused bytes, in all ISO15765 frame transmissions
 *
 * TDS J2534 Extension PADDING_BYTE, Range 0-1, Default Value 0x00
 * \internal Use to Set Frame PADDING_BYTE value in ISO15765 Driver
 */
#define VCI_FIOPADCONFIG            VCI_IOW(VCI_FIO_TYPE,   59, uint32_t)

/**
 * \brief Get CAN frame padding config
 *
 * TDS J2534 Extension PADDING_BYTE, Range 0-1, Default Value 0x00
 * \internal Use to Get Frame PADDING_BYTE value from ISO15765 Driver
 */
#define VCI_FIOGET_PADCONFIG            VCI_IOR(VCI_FIO_TYPE,   59, uint32_t)

/**
 * \internal
 * \brief Used interally by VCI protocol drivers
 *
 * Allows the protocol driver to tell the VCI controller the wakeup sequence is
 * over and normal comm's can continue
 */
#define VCI_FIOUARTSET_WAKEUP           VCI_IOW(VCI_FIO_TYPE,   60, uint32_t)

/**
 * \internal
 * \brief Used interally by VCI protocol drivers
 *
 * Allows to transmit raw data on a VCI controller
 */
#define VCI_FIO_TXRAW           VCI_IOW(VCI_FIO_TYPE,   61,     vci_raw_s)

/**
 * \brief Sets the protocol configuration
 *
 * Set protocol relevant flags to make the drivers more/less protocol aware
 */
#define VCI_FIOSET_PROTOCONFIG      VCI_IOW(VCI_FIO_TYPE,   62,     uint32_t)

/**
 * \brief Gets the protocol configuration
 *
 * Get protocol configuration flags
 */
#define VCI_FIOGET_PROTOCONFIG      VCI_IOR(VCI_FIO_TYPE,   62,     uint32_t)

/**
 * \brief Sets the System time
 *
 * Sets the system time.  The only valid value is 0.
 */
#define VCI_FIOTIMESTAMP        VCI_IOW(VCI_FIO_TYPE,   63,     uint32_t)

/**
 * \brief Gets the System time
 *
 * Gets the system time.  Returned in microseconds.
 */
#define VCI_FIOGET_TIMESTAMP        VCI_IOR(VCI_FIO_TYPE,   63,     uint32_t)

/**
 * \brief Sets,Clears or reads the Message check flag
 *
 */
#define VCI_FIOSET_MSG_CHECK        VCI_IOW(VCI_FIO_TYPE,   64,     uint32_t)
#define VCI_FIOGET_MSG_CHECK        VCI_IOR(VCI_FIO_TYPE,   64,     uint32_t)

/************************************************************************************************/
/* J1939     specific IOCTLs Section                                                            */
/************************************************************************************************/


/**
 * \brief Set J1939  specific value for Timeout T1
 *
 *  SET macro for setting Timeout T1 . That is,a gap of more than T1 sec after receipt of the last packet when more were expected
 *  T1 default value is 750 ms
 */
#define VCI_J1939_SET_T1_TIMEOUT  VCI_IOW(VCI_FIO_TYPE, 65, uint32_t )

/**
 * \brief GET  J1939  specific value for Timeout T1
 *
 *  GET  macro for setting Timeout T1 . That is, a gap of more than T1 sec after receipt of the last packet when more were expected
 *  T1 default value is 750 ms
 */
#define VCI_J1939_GET_T1_TIMEOUT  VCI_IOR(VCI_FIO_TYPE, 65, uint32_t )


/**
 * \brief Set J1939  specific value for Timeout T2
 *
 *  SET macro for setting Timeout T2 . Ta gap of more than T2 after a CTS was transmitted (originator failure);
 *  T2 default value is 1250 ms
 */

#define VCI_J1939_SET_T2_TIMEOUT  VCI_IOW(VCI_FIO_TYPE, 66, uint32_t )

/**
 * \brief Get J1939  specific value for Timeout T2
 *
 *  GET macro for setting Timeout T2 . Ta gap of more than T2 after a CTS was transmitted (originator failure);
 *  T2 default value is 1250 ms
 */

#define VCI_J1939_GET_T2_TIMEOUT  VCI_IOR(VCI_FIO_TYPE, 66, uint32_t )

/**
 * \brief Set J1939  specific value for Timeout T3
 *
 *  SET macro for setting Timeout T3 . a lack of CTS or ACK for more than T3 after the last packet was transmitted (responder failure);
 *  T3 default value is 1250 ms
 */

#define VCI_J1939_SET_T3_TIMEOUT  VCI_IOW(VCI_FIO_TYPE, 67, uint32_t )

/**
 * \brief Get J1939  specific value for Timeout T3
 *
 *  GET macro for setting Timeout T3 . a lack of CTS or ACK for more than T3 after the last packet was transmitted (responder failure);
 *  T3 default value is 1250 ms
 */
#define VCI_J1939_GET_T3_TIMEOUT  VCI_IOR(VCI_FIO_TYPE, 67, uint32_t )

/**
 * \brief Set J1939  specific value for Timeout T4
 *
 *  SET macro for setting Timeout T4 .a lack of CTS for more than T4 after a CTS (0) message to "hold the connection open";
 *  T4 default value is 1050 ms
 */
#define VCI_J1939_SET_T4_TIMEOUT  VCI_IOW(VCI_FIO_TYPE, 68, uint32_t )

/**
 * \brief Get J1939  specific value for Timeout T4
 *
 *  GET macro for setting Timeout T4 .a lack of CTS for more than T4 after a CTS (0) message to "hold the connection open";
 *  T4 default value is 1050 ms
 */

#define VCI_J1939_GET_T4_TIMEOUT  VCI_IOR(VCI_FIO_TYPE, 68, uint32_t )

/**
 * \brief Set J1939  specific value for Timeout Tr
 *
 *  SET macro for setting Timeout Tr .All devices, when required to provide a response, must do so within Tr
 * This sets the  max response delay time after processing the valid destination specific message request.
 *  Tr default value is 200 ms
 */
#define VCI_J1939_SET_Tr_TIMEOUT  VCI_IOW(VCI_FIO_TYPE, 69, uint32_t )

/**
 * \brief Get J1939  specific value for Timeout Tr
 *
 *  Get macro for setting Timeout Tr .All devices, when required to provide a response, must do so within Tr
 *  Tr default value is 200 ms
 */
#define VCI_J1939_GET_Tr_TIMEOUT  VCI_IOR(VCI_FIO_TYPE, 69, uint32_t )


/**
 * \brief Set J1939  specific value for Timeout Th
 *
 *  SET macro for setting Timeout Th . In the case where the flow must be stopped for some number of seconds,
 *            the responder must repeat the transmission of the CTS message once per Th to assure the originator the connection is not broken.
 *  Th default value is 500 ms
 */
#define VCI_J1939_SET_Th_TIMEOUT  VCI_IOW(VCI_FIO_TYPE, 70, uint32_t )

/**
 * \brief Get J1939  specific value for Timeout Th
 *
 *  GET macro for setting Timeout Th . In the case where the flow must be stopped for some number of seconds,
 *            the responder must repeat the transmission of the CTS message once per Th to assure the originator the connection is not broken.
 *  Th default value is 500 ms
 */
#define VCI_J1939_GET_Th_TIMEOUT  VCI_IOR(VCI_FIO_TYPE, 70, uint32_t )

/**
 * \brief Set J1939  specific value for Block size to be sent in CTS
 *
 *  SET macro for setting PKT size  . if FF sent max size
 *
 */
#define VCI_J1939_SET_CTS_PKT_SZ  VCI_IOW(VCI_FIO_TYPE, 71, uint32_t )

/**
 * \brief Get J1939  specific value for Block size to be sent in CTS
 *
 *  Get macro for setting PKT size  . if FF sent max size
 */
#define VCI_J1939_GET_CTS_PKT_SZ  VCI_IOR(VCI_FIO_TYPE, 71, uint32_t )

/* Configures baudrate tolerance
 */
#define VCI_FIOBAUD_TOLERANCE               VCI_IOW(VCI_FIO_TYPE,   72, uint32_t)

/**
 * \brief Fetches the baudrate
 *
 * Fetches the baudrate that's used on the channel that queries it
 */
#define VCI_FIOGET_BAUD_TOLERANCE           VCI_IOR(VCI_FIO_TYPE,   72, uint32_t)

/**
 * \brief set Twkup paramter for UART Fast initilization
 *
 * Set Total wakeup pulse duration in us
 */
#define VCI_FIOUARTSET_FASTINIT_TWUP            VCI_IOW(VCI_FIO_TYPE,   73, uint32_t)

/**
 * \brief Get Twkup paramter for UART Fast initilization
 *
 * Get Total wakeup pulse duration in us
 */
#define VCI_FIOUARTGET_FASTINIT_TWUP            VCI_IOR(VCI_FIO_TYPE,   73, uint32_t)

/**
 * \brief set TIniL paramter for UART Fast initilization
 *
 * Set Bus low time during wakeup. duration in us
 */
#define VCI_FIOUARTSET_FASTINIT_TINIL           VCI_IOW(VCI_FIO_TYPE,   74, uint32_t)

/**
 * \brief set TIniL paramter for UART Fast initilization
 *
 * Get Bus low time during wakeup. duration in us
 */
#define VCI_FIOUARTGET_FASTINIT_TINIL           VCI_IOR(VCI_FIO_TYPE,   74, uint32_t)

/* Configures  controller notification flags
 */
#define VCI_FIOSET_CONTROLLER_NOTIFICATION          VCI_IOW(VCI_FIO_TYPE,   75, uint32_t)

/**
 * \brief Fetches the controller notification flags
 * *
 * returns the configured controller notification value
 */
#define VCI_FIOGET_CONTROLLER_NOTIFICATION          VCI_IOR(VCI_FIO_TYPE,   75, uint32_t)
/**
 * < \internal \brief All VCI_* ioctl values fall below this value
 */
/**
 * \brief Set node address for J1850PWM
 * *
 * Set node address for J1850PWM
 */
#define VCI_FIOSET_NODE_ADDRESS                 VCI_IOW(VCI_FIO_TYPE,   76, uint32_t)
/**
 * \brief Get node address for J1850PWM
 * *
 * Get node address for J1850PWM
 */
#define VCI_FIOGET_NODE_ADDRESS                 VCI_IOR(VCI_FIO_TYPE,   77, uint32_t)

/**
 * \brief Add an address byte to the J1850PWM functional lookup table used by the gateway.
 * *
 * Add address to J1850PWM functional lookup table
 */
#define VCI_FIOADD_TO_FUNCT_MSG_LOOKUP_TABLE  VCI_IOW(VCI_FIO_TYPE, 78, uint32_t)

/**
 * \brief Delete an address byte from the J1850PWM functional lookup table used by the gateway.
 * *
 * Delete address from J1850PWM functional lookup table (0xFFFFFFFF = Delete All Addresses)
 */
#define VCI_FIODELETE_FROM_FUNCT_MSG_LOOKUP_TABLE  VCI_IOW(VCI_FIO_TYPE,    79, uint32_t)

/**
 * \brief Set number of retries to switch to random backoff retry strategy after
 * *
 * Sets number of retries after which retry switches from fixed to random backoff delay
 */
#define VCI_FIORANDOM_BACKOFF_AFTER  VCI_IOWR(VCI_FIO_TYPE, 80, uint32_t)

/**
 * \brief Set minimum valid message length (in bytes)
 * *
 * Sets the minimum number of bytes for a valid message (shorter messages will be discarded)
 */
#define VCI_FIOMIN_MSG_LEN  VCI_IOWR(VCI_FIO_TYPE,  81, uint32_t)

/**
 * \brief Set maximum valid message length (in bytes)
 * *
 * Sets the maximum number of bytes for a valid message (longer messages will be discarded)
 */
#define VCI_FIOMAX_MSG_LEN  VCI_IOWR(VCI_FIO_TYPE,  82, uint32_t)

/**
 * \brief Set maximum allowed inter-byte interval
 * *
 * Sets the maximum allowable delay between bytes within the same message (expressed in bit times)
 */
#define VCI_FIOMAX_INTER_BYTE_BITS  VCI_IOWR(VCI_FIO_TYPE,  83, uint32_t)

/**
 * \brief Set maximum number of retries
 * *
 * Sets the maximum number of retries to perform
 */
#define VCI_FIOMAX_RETRIES  VCI_IOWR(VCI_FIO_TYPE,  84, uint32_t)

/**
 * \brief CAN: Set the number of samples per bit
 * *
 * CAN: Set the number of samples per bit
 */
#define VCI_FIOSET_SAMPLES_PER_BIT  VCI_IOW(VCI_FIO_TYPE, 85, uint32_t) /**< \brief CAN: Set number of samples per bit */

/**
 * \brief CAN: Get the number of samples per bit
 * *
 * CAN: Get the number of samples per bit
 */
#define VCI_FIOGET_SAMPLES_PER_BIT  VCI_IOR(VCI_FIO_TYPE, 86, uint32_t) /**< \brief CAN: Get number of samples per bit */


/**
 * \brief CAN: Set listening mode
 * *
 * CAN: Set listening (monitoring) mode on or off
 */
#define VCI_FIOSET_LISTEN_ONLY_MODE VCI_IOW(VCI_FIO_TYPE, 87, uint32_t) /**< \brief CAN: Set listening mode */


/**
 * \brief CAN: Get listening mode
 * *
 * CAN: Get whether the controller is currently set to listening
 *      (monitoring) mode
 */
#define VCI_FIOGET_LISTEN_ONLY_MODE VCI_IOR(VCI_FIO_TYPE, 88, uint32_t) /**< \brief CAN: Set listening mode */


/**
 * \brief CAN: Set padding mode
 * *
 * CAN: Set the frame padding mode to off (0) or on (1)
 */
#define VCI_FIOSET_PADDING_MODE VCI_IOW(VCI_FIO_TYPE, 89, uint32_t) /**< \brief CAN: Set padding mode */


/**
 * \brief ISO15765: Set Time for transmission of the CAN frame (any N_PDU) on the receiver side (in us)
 */
#define VCI_FION_AR_TIMEOUT VCI_IOWR(VCI_FIO_TYPE, 90, uint32_t)    /**< \brief ISO15765: Set frame transmission time */


/**
 * \brief ISO9141 and ISO14230: Used in Simulation of 5Baid.
 */

#define VCI_FIOUART_XBAUD_SIM_INIT         VCI_IOW(VCI_FIO_TYPE,   91, vci_uart_simulation_xbaudinit_s)    /**< \brief connfiguration for xbaud simulation */

/**
 * \brief ISO9141 and ISO14230: Used in Simulation of 5Baid.
 */

#define VCI_FIOUARTGET_XBAUD_SIM_INIT      VCI_IOR(VCI_FIO_TYPE,   91, vci_uart_simulation_xbaudinit_s)    /**< \brief Fetches connfiguration for xbaud simulation */

/**
 * \brief ISO9141 :  Five Baud Mode selection
 */

#define VCI_FIOUARTSET_FIVEBAUD_MODE       VCI_IOW(VCI_FIO_TYPE,   92, uint32_t)


/**
 * \brief ISO9141 :  Five Baud Mode selection
 */

#define VCI_FIOUARTGET_FIVEBAUD_MODE       VCI_IOR(VCI_FIO_TYPE,   92, uint32_t)

/**
 * \brief ISO9141 :  Five Baud Mode selection
 */

#define VCI_FIOUARTSET_FIVEBAUD_MODE       VCI_IOW(VCI_FIO_TYPE,   92, uint32_t)


/**
 * \brief ISO9141 :  Five Baud Mode selection
 */

#define VCI_FIOUARTGET_FIVEBAUD_MODE       VCI_IOR(VCI_FIO_TYPE,   92, uint32_t)

#define VCI_FIOCFG_KEEP_PCI     VCI_IOW(VCI_FIO_TYPE, 93, uint32_t) /**< \brief ISO15765: Should the PCI byte be stripped? */


/**
 * \brief Set J1939  specific value for Block size to be sent in RTS
 *
 *  SET macro for setting PKT size  .
 * Setting value 0xFF refers no packet size limitation.
 *
 */
#define VCI_J1939_SET_RTS_TxMAX_PKT_SZ  VCI_IOW(VCI_FIO_TYPE,   94, uint32_t )

/**
 * \brief Get J1939  specific value for Block size to be sent in RTS
 *
 *  Get macro for setting PKT size  .
 */
#define VCI_J1939_GET_RTS_TxMAX_PKT_SZ  VCI_IOR(VCI_FIO_TYPE,   94, uint32_t )

/**
 * \brief Get byte duration for current configuration
 */
#define VCI_FIOGET_BYTEDUR  VCI_IOR(VCI_FIO_TYPE,   95, uint32_t )

/** \brief Get the initial wakeup delay for current configuration */
#define VCI_FIOUART_XBAUD_WAKEUP    VCI_IOR(VCI_FIO_TYPE,   96, uint32_t )
/** \brief Set XBaud simulation */
#define VCI_FIOUART_XBAUD_SIM       VCI_IOW(VCI_FIO_TYPE,   97, uint32_t )
/** \brief Set expected "end transmission" value */
#define VCI_FIOSET_MSG_LEN_FN       VCI_IOW(VCI_FIO_TYPE,   98, uint32_t )
/** \brief Set expected acknowledgement function */
#define VCI_FIOUARTEX_SET_EXPACK_FN VCI_IOW(VCI_FIO_TYPE,   99, uint32_t )
/** \brief Set message termination type:
 *  0   Finish message on transmission/reception of last byte
 *  1   Await ack from receiver before completing
 *  2   brief Keep going until we timeout waiting for a byte (default)
 */
#define VCI_FIOSET_MSG_TERM     VCI_IOW(VCI_FIO_TYPE,   100,    uint32_t )
/** \brief Set expected "end transmission" value when transmitting */
#define VCI_FIOSET_ETX_BYTE_TX      VCI_IOW(VCI_FIO_TYPE,   101,    uint32_t )
/** \brief Set expected "end transmission" value to expect when receiving */
#define VCI_FIOSET_ETX_BYTE_RX      VCI_IOW(VCI_FIO_TYPE,   102,    uint32_t )
/** \brief Set number of bytes required to calculate the message length */
#define VCI_FIOSET_NUM_BYTES_FOR_LEN_CALC   VCI_IOW(VCI_FIO_TYPE,   103,    uint32_t )
/** \brief Set list of messages that require auto-acking by driver */
#define VCI_FIOSET_MSG_ACK_LIST VCI_IOW(VCI_FIO_TYPE,   104,    vci_uart_ack_list)
#define VCI_FIOGET_MSG_ACK_LIST VCI_IOR(VCI_FIO_TYPE,   105,    vci_uart_ack_list)
/**
 * \brief Configure UI notification events
 * Configure the transmission of asynchronous messages on the current system
 * channel when the specified event occurs.
 */
#define VCI_FIO_UI_EVENT_CONFIG VCI_IOW(VCI_FIO_TYPE, 106, vci_ui_event_config_s)

/** \brief Read the current ignition sense state (MVCI pin 24) */
#define VCI_FIOREAD_IGN_SENSE       VCI_IOR(VCI_FIO_TYPE, 107, uint32_t)

#define VCI_FIO_TYPE_MAX        (108)   /**< \internal \brief All VCI_* ioctl values fall below this value */

/*****************************************************************************/
/* VCI_FIOTXNOTIFY ioctl transmit notification configuration                 */
/*****************************************************************************/

/* Use VCI_TX_<mode> to set tx notification mode (mutually exclusive)               */
/* Tx echoes generate a data frame with VCI_TX_ECHO set in the message header flags field   */
/* Tx acknowledges generate a protocol specific protocol event                  */

#define VCI_TX_NONE         (0x00000000)        /**< \brief No tx notification      */
#define VCI_TX_ACK          (0x10000000)        /**< \brief Transmit acknowledge only   */
#define VCI_TX_ECHO         (0x20000000)        /**< \brief Echo of transmitted message     */
#define VCI_TX_ECHO_FILTERING           (0x40000000)        /**< \brief Used together with teh VCI_TX_ECHO flag to say filtering should be applied
to the received echo, this flag should NOT be used from an application
directly, instead, the VCI_TX_ECHO_FILTERED flag should be used */
#define VCI_TX_INT_FRAME        (0x80000000)        /**< \brief Internally generated frame (not normally echoed)    */
#define VCI_TX_ECHO_FILTERED        (VCI_TX_ECHO_FILTERING|VCI_TX_ECHO) /**< \brief Echo of transmitted message should be filtered,
this flag should be used from applications to enable
filtering of transmit echoes */
#define VCI_TX_MASK         (VCI_TX_ACK|VCI_TX_ECHO) /**< \brief TODO: doc */

/*****************************************************************************/
/* VCI_FIORXNOTIFY ioctl receive notification configuration                  */
/*****************************************************************************/

#define VCI_RX_NONE         (0x00000000)        /**< \brief No rx notification (except rx data frames) */
#define VCI_RX_SOF          (0x40000000)        /**< \brief Start Of Frame      */
#define VCI_RX_TIMEOUT          (0x80000000)        /**< \brief Send event when no more responses are expected */
#define VCI_RX_ACK          (0x08000000)
#define VCI_RX_MASK         (VCI_RX_SOF|VCI_RX_TIMEOUT) /**< \brief TODO: doc */

/*****************************************************************************/
/* VCI_FIOCONNECT ioctl connect configuration                                */
/*****************************************************************************/

/* N.B. Must remain unique from VCI_FIOTXNOTIFY flags if copied into priv */
#define VCI_DEFAULT_BLOCK_ALL       (0x08000000)        /**< \brief No rx until filter configured */

/*****************************************************************************/
/* VCI_FIOPERIODICCONFIG ioctl periodic message configuration                */
/*****************************************************************************/

/**
 * \brief Default periodic message configuration
 *
 */
#define VCI_PERIODIC_NONE       (0x00000000)

/**
 * \brief Disable periodic tx notifications
 *
 *  Default is to adopt the policy set by VCI_FIOTXNOTIFY
 */
#define VCI_PERIODIC_TX_NOTIFY      (0x01000000)

/**
 * \brief Reset periodic timer on call to file write
 *
 */
#define VCI_PERIODIC_TX_RESET       (0x02000000)

/**
 * \brief Disable periodic rx notifications
 *
 *  Default is to adopt the policy set by VCI_FIORXNOTIFY
 */
#define VCI_PERIODIC_RX_NOTIFY      (0x04000000)

/**
 * \brief Periodic message configuration mask
 *
 */
#define VCI_PERIODIC_MASK       (VCI_PERIODIC_TX_NOTIFY|VCI_PERIODIC_RX_NOTIFY|VCI_PERIODIC_TX_RESET)

/*****************************************************************************/
/* VCI_FIOSET_PROTOCONFIG ioctl protocol configuration                       */
/*****************************************************************************/

#define VCI_PROTO_LENCHECK      (0x00010000)        /**< \brief Have the driver check the message length */
#define VCI_PROTO_MASK          (0x00FF0000)        /**< \brief TODO: doc */

/*
 * Define a simple config data record that takes a single uint32
 * data field. This can be used for 90% of configurations records
 */
struct vci_uint32_config_record
{
    vci_config_record_s config_record ;
    uint32_t    value ;     /* VCI config request value */
} _PACKED_  ;

typedef struct vci_uint32_config_record vci_uint32_config_record_s;

#define VCI_UINT32_CONFIG_RECORD_SZ sizeof( vci_uint32_config_record_s )
#define VCI_UINT32_CONFIG_DATA_SZ   sizeof( uint32_t )

#define vci_uint32_config_record_constructor( \
        _vci_rec, _op_code, _user_data, _value ) \
{ \
    vci_uint32_config_record_s* _vci_uint32_rec=\
            (vci_uint32_config_record_s*)_vci_rec; \
    vci_config_request_record_constructor( (vci_config_record_s*)_vci_rec,\
                                           _op_code, _user_data, VCI_UINT32_CONFIG_DATA_SZ ); \
    _vci_uint32_rec->value = htonl( _value ); \
}
#endif

