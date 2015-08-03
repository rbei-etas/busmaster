/**
 * \section LICENSE
 *                   Copyright 2011 by KVASER AB, SWEDEN
 *
 *                        WWW: http://www.kvaser.com
 *
 * This software is furnished under a license and may be used and copied
 * only in accordance with the terms of such license.
 *
 * \section DESCRIPTION
 *
 * THIS IS A PRELIMINARY VERSION AND SUBJECT TO CHANGE.
 *
 * Proposed new remote device API.
 *
 * \file kvrlib.h
 * \version PRELIMINARY
 * \author Kvaser AB
 */
 
#ifndef KVRLIB_H_
#define KVRLIB_H_

#include <windows.h>
#include <kvaser_stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Return type of kvrlib functions.
 */
typedef enum {
  kvrOK                    =  0,  /**< OK! */ 
  kvrERR_NOT_INITIALIZED   = -1,  /**< kvrlib has not been initialized. */
  kvrERR_GENERIC           = -2,  /**< Generic error. */
  kvrERR_CHECKSUM          = -3,  /**< Checksum problem. */
  kvrERR_PARAMETER         = -4,  /**< Error in supplied in parameters. */
  kvrERR_PASSWORD          = -5,  /**< Supplied password was wrong. */
  kvrERR_BLANK             = -6,  /**< List was not set or no more results. */
  kvrERR_NO_DEVICE         = -7,  /**< Remote device is unreachable. */
  kvrERR_NO_ANSWER         = -8,  /**< No answer arrived within given timeout. */
  kvrERR_NOT_IMPLEMENTED   = -9,  /**< Function is not yet implemented. */
  kvrERR_PERMISSION_DENIED = -10, /**< Permission denied. */
  kvrERR_OUT_OF_SPACE      = -11, /**< Out of space, eg. to many open handles, to small buffer. */
  kvrERR_NO_SERVICE        = -12, /**< The helper service is not running. */
  kvrERR_DUPLICATED_DEVICE = -13, /**< There are duplicates in the device list. */
  /* Remember to update the list of errorstrings (error_text_array) in the library as well. */  
} kvrStatus;

/**
 * \defgroup Configuration  Local configuration
 */
/**@{*/

/** 
 * A configuration handle. Created by calling kvrConfigOpen() or kvrConfigOpenEx().
 */
typedef int32_t kvrConfigHandle;

/**
 * \name kvrConfigMode_xxx
 * Configuration mode.
 * \anchor kvrConfigMode_xxx
 * @{
 */
#define kvrConfigMode_R         0  /**< Read only. */
#define kvrConfigMode_RW        1  /**< Read/write. */
#define kvrConfigMode_ERASE     2  /**< Erase and write. */
/** @} */

/**
 * Open a configuration handle to the device.
 * It should later be closed with kvrConfigClose().
 *
 * This configuration handle is used both for changing the device configuration,
 * reading status information, e.g. kvrNetworkGetConnectionStatus(), and
 * issuing some other commands such as e.g. kvrNetworkConnectionTest() and
 * kvrWlanStartScan().
 *
 * To change the configuration, you need to open the configuration with
 * ::kvrConfigMode_RW before calling kvrConfigSet(). 
 *
 * To read the configuration, you can open the configuration with either
 * ::kvrConfigMode_RW or ::kvrConfigMode_R before calling kvrConfigGet().
 *
 * Setting a password is done through the configuration (with kvrConfigSet()).
 * Resetting a password can be done by erasing the whole configuration with
 * kvrConfigClear(), while first opening the configuration with
 * ::kvrConfigMode_ERASE and supplying an empty password.
 *
 * Before calling any other function, you must open the configuration with
 * ::kvrConfigMode_R.
 *
 * \param[in]  can_channel_no  CAN channel number.
 * \param[in]  mode            Can be set to one of \ref kvrConfigMode_xxx
 * \param[in]  password        The password as a C string.
 *                             Use an empty string, i.e. "", if no password 
 *                             is required.
 * \param[out] handle          A configuration handle
 *
 * \return ::kvrOK on success or any other ::kvrStatus on failure.
 *
 * \note kvrConfigOpen() will operate on the active profile.
 * See \ref page_configuration.
 * \note When using ::kvrConfigMode_RW or ::kvrConfigMode_ERASE, a pause of
 *       ~2 seconds is needed after kvrConfigClose(), to allow CANlib time to
 *       discard the device.
 * \sa kvrConfigOpenEx()
 */
kvrStatus WINAPI kvrConfigOpen(int32_t can_channel_no, 
                               int32_t mode, 
                               const char *password, 
                               kvrConfigHandle *handle);


/**
 * Set active profile. See \ref page_configuration. 
 * 
 *
 * \param[in] can_channel_no  CAN channel number
 * \param[in] profile_number  
 * \return ::kvrOK on success or any other ::kvrStatus on failure.
 *
 * \note A pause of ~2 seconds is needed after kvrConfigActiveProfileSet(),
 *       to allow CANlib time to discard the device.
 */                               
kvrStatus WINAPI kvrConfigActiveProfileSet (int32_t can_channel_no, int32_t profile_number);

/**
 * Get active profile. See \ref page_configuration. 
 * 
 *
 * \param[in] can_channel_no  CAN channel number
 * \param[out] profile_number  
 * \return ::kvrOK on success or any other ::kvrStatus on failure.
 */                               
kvrStatus WINAPI kvrConfigActiveProfileGet (int32_t can_channel_no, int32_t *profile_number);

/**
 * Get the maximum number of profile(s) the device can store. 
 * See \ref page_configuration. 
 * 
 *
 * \param[in] can_channel_no  CAN channel number
 * \param[out] no_profiles  
 * \return ::kvrOK on success or any other ::kvrStatus on failure.
 */                               
kvrStatus WINAPI kvrConfigNoProfilesGet(int32_t can_channel_no, int32_t *no_profiles);


/**
 * Open a configuration handle to the device.
 * It should later be closed with kvrConfigClose().
 *
 * This configuration handle is used both for changing the device configuration,
 * reading status information, e.g. kvrNetworkGetConnectionStatus(), and
 * issuing some other commands such as e.g. kvrNetworkConnectionTest() and
 * kvrWlanStartScan().
 *
 * To change the configuration, you need to open the configuration with
 * ::kvrConfigMode_RW before calling kvrConfigSet(). 
 *
 * To read the configuration, you can open the configuration with either
 * ::kvrConfigMode_RW or ::kvrConfigMode_R before calling kvrConfigGet().
 *
 * Setting a password is done through the configuration (with kvrConfigSet()).
 * Resetting a password can be done by erasing the whole configuration with
 * kvrConfigClear(), while first opening the configuration with
 * ::kvrConfigMode_ERASE and supplying an empty password.
 *
 * Before calling any other function, you must open the configuration with
 * ::kvrConfigMode_R.
 * 
 * The profile number is used to open a specific profile. 
 * See \ref page_configuration. 
 *
 * \param[in]  can_channel_no  CAN channel number
 * \param[in]  mode            Can be set to one of \ref kvrConfigMode_xxx
 * \param[in]  password        The password as a C string.
 *                             Use an empty string, i.e. "", if no password 
 *                             is required.
 * \param[out] handle          A configuration handle
 * \param[in]  profile_no      Profile number
 *
 * \return ::kvrOK on success or any other ::kvrStatus on failure.
 *
 * \note When using ::kvrConfigMode_RW or ::kvrConfigMode_ERASE, a pause of
 *       ~2 seconds is needed after kvrConfigClose(), to allow CANlib time to
 *       discard the device.
 * \sa kvrConfigOpen()
 */
kvrStatus WINAPI kvrConfigOpenEx(int32_t can_channel_no, 
                               int32_t mode, 
                               const char *password, 
                               kvrConfigHandle *handle,
                               uint32_t profile_no);

/**
 * Closes the device's configuration area without programming it.
 * Programming is done with kvrConfigSet().
 *
 * \param[in] handle  A configuration handle
 *
 * \note When using ::kvrConfigMode_RW or ::kvrConfigMode_ERASE, a pause of
 *       ~2 seconds is needed after kvrConfigClose(), to allow CANlib time to
 *       discard the device.
 */
void WINAPI kvrConfigClose(kvrConfigHandle handle);

/**
 * Verify that the xml buffer complies with both the DTD and internal restrictions. If
 * the XML input creates any errors and err_buffer is to small to hold the resulting
 * error message, ::kvrERR_PARAMETER will be returned.
 *
 * \param[in]  xml_buffer       A pointer to a C string containing an XML configuration.
 * \param[out] err_buffer       A pointer to a buffer that will hold any error messages.
 * \param[in]  err_buffer_size  The buffer size. Maximum size needed is 2048 bytes.
 *
 * \return ::kvrOK on success or any other ::kvrStatus on failure.
 *
 */
kvrStatus WINAPI kvrConfigVerifyXml(const char *xml_buffer,
                                    char *err_buffer,
                                    uint32_t err_buffer_size);

/**
 * Set the device configuration. The area is erased before it is
 * programmed. The handle must be opened ::kvrConfigMode_RW and closed with
 * kvrConfigClose() afterward. If the XML input creates any errors,
 * ::kvrERR_PARAMETER will be returned.
 *
 * \param[in] handle      A configuration handle.
 * \param[in] xml_buffer  A pointer to a C string containing a valid XML config.
 *
 * \return ::kvrOK on success or any other ::kvrStatus on failure.
 *
 */
kvrStatus WINAPI kvrConfigSet(kvrConfigHandle handle, const char *xml_buffer);

/**
 * Reads the device configuration. On successful return, the buffer
 * will contain a valid C string with the configuration in XML format.
 * The handle must be opened ::kvrConfigMode_R or ::kvrConfigMode_RW and closed
 * with kvrConfigClose().
 *
 * \param[in]  handle           A configuration handle.
 * \param[out] xml_buffer       A pointer to the data buffer.
 * \param[in]  xml_buffer_size  The buffer size.
 *
 * \return ::kvrOK on success or any other ::kvrStatus on failure.
 *
 */
kvrStatus WINAPI kvrConfigGet(kvrConfigHandle handle, 
                              char *xml_buffer, 
                              uint32_t xml_buffer_size);

/**
 * Reads a simplified version of A device configuration PROFILE.
 * On successful return, the buffer will contain a valid C string of this
 * in XML format.
 * Since the function takes a CAN channel rather than a kvrConfigHandle, it is
 * not necessary to know the configuration password.
 * Note that the partial XML data returned is not enough to reconfigure a device.
 *
 * \param[in]  can_channel_no   CAN channel number.
 * \param[in]  profile_no       Profile number
 * \param[out] xml_buffer       A pointer to the data buffer.
 * \param[in]  xml_buffer_size  The buffer size.
 *
 * \return ::kvrOK on success,
 *         ::kvrERR_BLANK when the profile is empty,
 *         or any other ::kvrStatus on failure.
 *
 */
kvrStatus WINAPI kvrConfigInfoGet(int32_t can_channel_no,
                                  int32_t profile_no,
                                  char *xml_buffer,   
                                  uint32_t xml_buffer_size);

/**
 * Clear the device configuration area. This will also clear any
 * previously set device password. The handle must be opened ::kvrConfigMode_ERASE
 * and closed with kvrConfigClose().
 *
 * \param[in] handle  A configuration handle.
 *
 * \return ::kvrOK on success or any other ::kvrStatus on failure.
 */
kvrStatus WINAPI kvrConfigClear(kvrConfigHandle handle);

/**@}*/

/**
 * \defgroup Network Network information
 */
/**@{*/

/**
 * Device address.
 */
typedef struct {
  uint32_t type;        /**< \ref kvrAddressType_xxx. */
  uint8_t address[20];  /**< IP or MAC address. */
} kvrAddress;


/**
 * Capability of a WLAN AP. These are values from the standard 802.11 beacon.
 *
 * To convert the security information into a more human readable form,
 * use kvrWlanGetSecurityText().
 */
typedef struct {
  uint32_t version;
  uint32_t capability;        /**< Advertised capabilities. 
                               capability[5] Privacy flag 1: WEP
                                                          0: Open */
  uint32_t group_cipher;      /**< 0xff: No WPA/RSN.
                                   0x02: TKIP
                                   0x04: CCMP */
  uint32_t list_cipher_auth;  /**< 8 nybbles (In each nybble: 
                                   pos 0: cipher where;
                                      0x2: TKIP
                                      0x4: CCMP
                                   pos 1: authentication where;
                                      0xa: PSK
                                      0x9: RADIUS                                      
                                   Unused nybbles are set to 0xf. */
} kvrCipherInfoElement;

/**
 * Type of device address. 
 * \note Ports are currently not used.
 * \anchor kvrAddressType_xxx
 * \name kvrAddressType_xxx
 * @{
 */
#define kvrAddressType_UNKNOWN     0  /**< Unknown (e.g., no reply from device). */
#define kvrAddressType_IPV4        1  /**< IP v.4 address. */
#define kvrAddressType_IPV6        2  /**< IP v.6 address. */
#define kvrAddressType_IPV4_PORT   3  /**< IP v.4 address with tcp-port. */
#define kvrAddressType_MAC         4  /**< Ethernet MAC address. */  
/** @} */

/**
 * Receive Signal Strength Indicator (RSSI).
 */
typedef int32_t kvrRssiHistory;

/**
 * Round-trip delay time (RTT).
 */
typedef uint32_t kvrRttHistory;

 /**
 * States for network connection.
 * \anchor kvrNetworkState_xxx
 * \name kvrNetworkState_xxx
 * @{
 */
#define kvrNetworkState_UNKNOWN                0  /**< Bad state, should never be reported. */
#define kvrNetworkState_INVALID                1  /**< Network hardware has been disabled. */
#define kvrNetworkState_STARTUP                2  /**< Configuring network hardware. */
#define kvrNetworkState_INITIALIZING           3  /**< Started, waiting for initialization. */
#define kvrNetworkState_NOT_CONNECTED          4  /**< No connection (may auto-connect). */
#define kvrNetworkState_CONNECTION_DELAY       5  /**< Delay during connection (ad-hoc). */
#define kvrNetworkState_CONNECTING             6  /**< Waiting for connections (ad-hoc).  */
#define kvrNetworkState_CONNECTED              7  /**< Network is reached. */
#define kvrNetworkState_AUTHENTICATING         8  /**< EAPOL handshake ongoing. */
#define kvrNetworkState_AUTHENTICATION_FAILED  9  /**< Authentication have failed. */
#define kvrNetworkState_ONLINE                 10 /**< Authentication completed. */
#define kvrNetworkState_FAILED_MIC             11 /**< MIC verification (EAPOL-key) failed. */
/** @} */

/**
 * Basic Service Set.
 * \anchor kvrBss_xxx
 * \name kvrBss_xxx
 * @{
 */
#define kvrBss_INFRASTRUCTURE     0 /**< Network with AP. */
#define kvrBss_INDEPENDENT        1 /**< Ad-hoc network. */
#define kvrBss_ANY                2 /**< Any. */
/** @} */

/**
 * Regulatory domain.
 * \anchor kvrRegulatoryDomain_xxx
 * \name kvrRegulatoryDomain_xxx
 * @{
 */

#define kvrRegulatoryDomain_JAPAN_TELEC         0  /**< TELEC */
#define kvrRegulatoryDomain_EUROPE_ETSI         1  /**< ETSI */
#define kvrRegulatoryDomain_NORTH_AMERICA_FCC   2  /**< FCC */
#define kvrRegulatoryDomain_WORLD               3  /**< WORLD */
#define kvrRegulatoryDomain_CHINA_MII           4  /**< MII */
/** @} */

/**
 * Activate or deactivate connection test. When actived, the device will
 * connect and start pinging itself to measure RTT. Use kvrNetworkGetRssiRtt()
 * (after a while) to get the latest values.
 *
 * \param[in] handle  A configuration handle.
 * \param[in] active  Activate or deactivate connection test.
 *
 * \return ::kvrOK on success or any other ::kvrStatus on failure.
 * \sa kvrNetworkGetRssiRtt()
 */
 kvrStatus WINAPI kvrNetworkConnectionTest(kvrConfigHandle handle, 
                                           int32_t active);

/**
 * Get a history of Receive Signal Strength Indicator (RSSI) and
 * round-trip delay time (RTT) from the connection test.
 *
 * \param[in]  handle       A configuration handle.
 * \param[out] rssi         Receive Signal Strength Indicator.
 * \param[in]  rssi_size    Number of entries in \a rssi.
 * \param[out] rssi_count   Number of RSSI elements returned.
 * \param[out] rtt          Round-trip delay time.
 * \param[in]  rtt_size     Number of entries in \a rtt.
 * \param[out] rtt_count    Number of RTT elements returned.
 *
 * \return ::kvrOK on success or any other ::kvrStatus on failure.
 * \sa kvrNetworkConnectionTest()
 */
kvrStatus WINAPI kvrNetworkGetRssiRtt(kvrConfigHandle handle,
                                      kvrRssiHistory *rssi,
                                      uint32_t rssi_size,
                                      uint32_t *rssi_count,
                                      kvrRttHistory *rtt,
                                      uint32_t rtt_size,
                                      uint32_t *rtt_count);

/**
 * Initiate a scan for existing WLAN networks. The result is acquired
 * with consecutive calls to kvrWlanGetScanResults(). A new scan can not
 * be initiated until all data has been retrieved from the previous one.
 *
 * \note The device should not be connected to a network when scanning. This
 *       includes the implicit connection done by kvrNetworkConnectionTest().
 * 
 * \param[in] handle    A configuration handle.
 * \param[in] active    If set, performs an active scan.
 * \param[in] bss_type  Basic service set (BSS) selection. \ref kvrBss_xxx.
 * \param[in] domain    Regulatory domain. See \ref kvrRegulatoryDomain_xxx.
 *
 * \return ::kvrOK on success,
 *         ::kvrERR_NO_ANSWER when previous scan is ongoing
 *         or any other ::kvrStatus on failure.
 */

kvrStatus WINAPI kvrWlanStartScan(kvrConfigHandle handle,
                                  int32_t active,
                                  int32_t bss_type,
                                  int32_t domain);


/**
 * Get results from WLAN scan. Call kvrWlanGetScanResults() until it returns
 * ::kvrERR_BLANK to mark that no more information is available from this scan.
 *
 * To convert the security information into a more human readable form,
 * use kvrWlanGetSecurityText().
 *
 * \param[in]  handle       A configuration handle.
 * \param[out] rssi         Receive Signal Strength Indicator (RSSI).
 * \param[out] channel      WLAN Channel.
 * \param[out] mac          Media Access Control address. 
 * \param[out] bss_type     Basic Service Set (BSS) (see \ref kvrBss_xxx).
 * \param[out] ssid         Service Set Identifier(SSID) as a C string.
 *                          Maximum length is 32 bytes.
 * \param[out] capability   The advertised capabilities of the BSS.
 * \param[out] type_wpa     Only type 1 (802.1X) is supported for connection.
 * \param[out] wpa_info     Advertised information for WPA 
 *                          (see ::kvrCipherInfoElement).
 * \param[out] rsn_info     Advertised information for RSN 
 *                          (see ::kvrCipherInfoElement).
 *
 * \return ::kvrOK on success,
 *         ::kvrERR_NO_ANSWER when waiting for further scan results
 *         ::kvrERR_BLANK when no further scan results are available
 *         or any other ::kvrStatus on failure.
 *
 */
kvrStatus WINAPI kvrWlanGetScanResults(kvrConfigHandle handle,
                                       int32_t *rssi,
                                       int32_t *channel,
                                       kvrAddress *mac,
                                       int32_t *bss_type,
                                       char *ssid,
                                       uint32_t *capability,
                                       uint32_t *type_wpa,
                                       kvrCipherInfoElement *wpa_info,
                                       kvrCipherInfoElement *rsn_info);

/**
 * Read the device Hostname.
 *
 * \param[in] handle               A configuration handle
 * \param[out] buffer              The device Hostname as a C string.
 * \param[in]  buffer_size         The device Hostname buffer size.
 *
 * \return ::kvrOK on success or any other ::kvrStatus on failure.
 */                                                                        
kvrStatus WINAPI kvrNetworkGetHostName(kvrConfigHandle handle, 
                                       char* buffer, uint32_t buffer_size);
                                              
/**
 * Read the generated Hostname.
 *
 * \param[in] ean_hi               The device EAN_high number.
 * \param[in] ean_lo               The device EAN_low number.
 * \param[in] ser_no               The device serial number.
 * \param[out] buffer              The device Hostname as a C string.
 * \param[in]  buffer_size         The device Hostname buffer size.
 *
 * \return ::kvrOK on success or any other ::kvrStatus on failure.
 */                                                                        
kvrStatus WINAPI kvrHostName(uint32_t ean_hi, uint32_t ean_lo, uint32_t ser_no, 
                             char* buffer, uint32_t buffer_size);
                                       
                                       
/**
 * Get connection status information. 
 *
 * \param[in]  handle      A configuration handle.
 * \param[out] state       Network connection state, see \ref kvrNetworkState_xxx.
 * \param[out] tx_rate     Transmit rate in kbit/s.
 * \param[out] rx_rate     Receive rate in kbit/s.
 * \param[out] channel     Channel.
 * \param[out] rssi        Receive Signal Strength Indicator (RSSI).
 * \param[out] tx_power    Transmit power level in dB.
 *
 * \return ::kvrOK on success or any other ::kvrStatus on failure.
 */
kvrStatus WINAPI kvrNetworkGetConnectionStatus(kvrConfigHandle handle,
                                               int32_t *state,
                                               int32_t *tx_rate,
                                               int32_t *rx_rate,
                                               int32_t *channel,
                                               int32_t *rssi,
                                               int32_t *tx_power);

/**
 * Get information about the network address settings.
 * For a WLAN connected device, address1, netmask and gateway are IP addresses
 * and address2 is the MAC address.
 *
 * \param[in]  handle    A configuration handle.
 * \param[out] address1  The first address associated with the device.
 * \param[out] address2  The second address associated with the device.
 * \param[out] netmask   The netmask for the device.
 * \param[out] gateway   The gateway for the device.
 * \param[out] dhcp      The device ueses Dynamic Host Configuration 
 *                       Protocol (DHCP).
 *
 * \return ::kvrOK on success or any other ::kvrStatus on failure.
 */
kvrStatus WINAPI kvrNetworkGetAddressInfo(kvrConfigHandle handle,
                                          kvrAddress *address1,
                                          kvrAddress *address2,  
                                          kvrAddress *netmask,
                                          kvrAddress *gateway,
                                          int32_t *dhcp);

/**@}*/

/**
 * \defgroup Discovery Device discovery
 */
/**@{*/

/**
 * Remote device usage status.
 * \anchor kvrDeviceUsage_xxx
 * \name kvrDeviceUsage_xxx
 * @{
 */

#define kvrDeviceUsage_UNKNOWN   0 /**< Unknown (e.g., no reply from device). */
#define kvrDeviceUsage_FREE      1 /**< Not in use. */
#define kvrDeviceUsage_REMOTE    2 /**< Connected to a PC (as a remote device). */
#define kvrDeviceUsage_USB       3 /**< Connected via USB cable. */
#define kvrDeviceUsage_CONFIG    4 /**< Device is being configured via USB. */
/** @} */


/**
 * Remote device accessability status.
 * \anchor kvrAccessibility_xxx
 * \name kvrAccessibility_xxx
 */
#define kvrAccessibility_UNKNOWN      0 /**< Unknown (e.g., no reply from device). */
#define kvrAccessibility_PUBLIC       1 /**< Public (visible for all, no password required to connect). */
#define kvrAccessibility_PROTECTED    2 /**< Protected (visible for all, password needed to connect). */
#define kvrAccessibility_PRIVATE      3 /**< Private (invisible, password needed to connect). */
/** @} */

/**
 * State of connection to device.
 * \anchor kvrRemoteState_xxx
 * \name kvrRemoteState_xxx
 * @{
 */

#define kvrRemoteState_VOID                  0  /**< Marked as not in list. */
#define kvrRemoteState_AVAILABLE             1  /**< Tries to ping known device. */
#define kvrRemoteState_DISCOVERED            2  /**< Currently not used. */
#define kvrRemoteState_STARTING              3  /**< Initializes for new device. */
#define kvrRemoteState_STARTED               4  /**< Currently not used. */
#define kvrRemoteState_CONNECTION_DOWN       5  /**< Will try and restore connection. */
#define kvrRemoteState_CONNECTION_UP         6  /**< Device connected, heartbeat up. */
#define kvrRemoteState_REDISCOVER            7  /**< Trying to talk to device. */
#define kvrRemoteState_UNWILLING             8  /**< Device turned down connection req. */
#define kvrRemoteState_REDISCOVER_PENDING    9  /**< Will do rediscover in a moment. */
#define kvrRemoteState_CLOSING               10 /**< Will stop communication. */
#define kvrRemoteState_REMOVE_ME             11 /**< Device removed, it will be stopped. */
#define kvrRemoteState_STANDBY               12 /**< Known device, but unused. */
#define kvrRemoteState_CONFIG_CHANGED        13 /**< Same as UNWILLING. */
#define kvrRemoteState_STOPPING              14 /**< Tries to stop device. */
#define kvrRemoteState_INSTALLING            15 /**< Driver installation is in progress. */
/** @} */

/**
 * Device avalability flags.
 * \anchor kvrAvailability_xxx
 * \name kvrAvailability_xxx
 * @{
 */
#define kvrAvailability_NONE             0 /**< Manually added. */
#define kvrAvailability_FOUND_BY_SCAN    1 /**< Device was found by scan. */
#define kvrAvailability_STORED           2 /**< Device was stored. */
/** @} */


/**
 * \brief Holds information about a discovered device.
 *
 * The information in here is returned when a device is discovered.
 * For a WLAN connected device, device_address and client_address are IP
 * addresses, and base_station_id is the ethernet MAC address of the AP.
 *
 * Depending on the "availability" flag, not all fields may be used. If
 * "availability" includes the flag ::kvrAvailability_STORED the following
 * fields are set: ean_hi, ean_lo, ser_no, device_address, request_connection,
 * name and accessibility_pwd.
 * 
 * If the field "availability" includes the flag
 * ::kvrAvailability_FOUND_BY_SCAN, the following fields are set: fw_major_ver,
 * fw_minor_ver, fw_build_ver, usage, accessibility, host_name and
 * client_address.
 */
typedef struct {
  uint32_t      struct_size;     /**< Size of struct, used for compatibility. */
  uint32_t      ean_hi;          /**< High part of EAN. */
  uint32_t      ean_lo;          /**< Low part of EAN. */
  uint32_t      ser_no;          /**< Serial number. */
  int32_t       fw_major_ver;    /**< Major firmware version. */
  int32_t       fw_minor_ver;    /**< Minor firmware version. */
  int32_t       fw_build_ver;    /**< Firmware build version. */
  char          name[256];       /**< User-defined name. */
  char          host_name[256];  /**< DNS hostname or empty. */
  int32_t       usage;           /**< \ref kvrDeviceUsage_xxx. */
  int32_t       accessibility;   /**< \ref kvrAccessibility_xxx. */
  char          accessibility_pwd[256];  /**< Accessibility password or empty. */  
  kvrAddress    device_address;  /**< Address of remote device. */
  kvrAddress    client_address;  /**< Address of connected client, if any. */
  kvrAddress    base_station_id; /**< Unique ID of base station, if any. */
  int32_t       request_connection; /**< Activate or deactivate a request for connection to a specified device. 
                                      * Activation means that the device will be connected to when it appears in the 
                                      * future. */
  int32_t       availability;    /**< The device availability. One or more \ref kvrAvailability_xxx flags.*/
  char          encryption_key[32];  /**< Encryption key. */  
  char          reserved1[256];
  char          reserved2[256];
} kvrDeviceInfo;

/** 
 * Handle used for discovery.
 */
typedef int32_t kvrDiscoveryHandle;

 
/**
 * Flags for setting what addresses that should be returned by
 * kvrDiscoveryGetDefaultAddresses().
 * \anchor kvrAddressTypeFlag_xxx
 * \name kvrAddressTypeFlag_xxx
 * @{
 */
#define kvrAddressTypeFlag_ALL         0xff /**< All defined below */
#define kvrAddressTypeFlag_BROADCAST   0x01 /**< Broadcast addresses */
#define kvrAddressTypeFlag_STORED      0x02 /**< Previously stored addresses */
/** @} */

/**
 * Read out the list of default broadcast addresses. If \a address_type_flags
 * is set to ::kvrAddressTypeFlag_ALL the returned list will contain all found
 * addresses (both broadcast addresses and earlier stored addresses).
 *
 * \param[out]    address_list        An array of addresses.
 * \param[in]     address_list_size   Number of entries in \a address_list.
 * \param[out]    address_list_count  Number of addresses returned.
 * \param[in]     address_type_flags  Which \ref kvrAddressTypeFlag_xxx types of addresses to return
 *
 * \return ::kvrOK on success or any other ::kvrStatus on failure.
 */
kvrStatus WINAPI kvrDiscoveryGetDefaultAddresses(kvrAddress address_list[],
                                                 uint32_t address_list_size,
                                                 uint32_t *address_list_count,
                                                 uint32_t address_type_flags);

/**
 * Create a handle for device discovery. Used by for instance kvrDiscoveryStart(). 
 * Close with kvrDiscoveryClose(). 
 *
 * \param[out]     handle             Discovery handle.
 *
 * \return ::kvrOK on success or any other ::kvrStatus on failure.
 */                                              
kvrStatus WINAPI kvrDiscoveryOpen(kvrDiscoveryHandle *handle);

/**
 * Closes the discovery handle opened with kvrDiscoveryOpen().
 *
 * \param[in]     handle             Discovery handle.
 *
 * \return ::kvrOK on success or any other ::kvrStatus on failure.
 */                                              
kvrStatus WINAPI kvrDiscoveryClose(kvrDiscoveryHandle handle);
                                     
/**
 * Set a list of addresses to use for discovery (overwrites any 
 * existing addresses).
 * Setting \a address_list_size with size = 0 will cause kvrDiscoveryStart() 
 * to only return stored devices (no network traffic). 
 *
 * \param[in]     handle             Discovery handle.
 * \param[in]     address_list       An array of addresses.
 * \param[in]     address_list_size  Number of entries in \a address_list.
 * \return ::kvrOK on success or any other ::kvrStatus on failure.
 */
kvrStatus WINAPI kvrDiscoverySetAddresses(kvrDiscoveryHandle handle, 
                                          const kvrAddress address_list[], 
                                          uint32_t address_list_size);
                        
                        
/**
 * Start discovering devices on the addresses previously specified with
 * kvrDiscoverySetAddresses().  A delay of \a delay_ms ms is inferred between
 * each device address request. After the last device address is probed, one
 * more delay of \a timeout_ms is added before returning.
 *
 * This means that the function will return in (about)
 * &lt;address_list_size&gt; * \a delay_ms + \a timeout_ms ms
 *
 * The results can be retrieved using kvrDiscoveryGetResults(). A new call to 
 * kvrDiscoveryStart() will discard any results not yet retrieved by 
 * kvrDiscoveryGetResults().
 *
 * To decide if an address is a broadcast address, the ip address and subnet
 * mask for all availible network cards are considered.
 *
 * Beside returning the devices discovered by scan, it will also return any
 * devices previously stored with kvrDiscoveryStoreDevices().
 *
 * \note A remote device with accessibility set to "private" will not reply to
 * a broadcast scan.
 *
 * \param[in]  handle      Discovery handle. 
 * \param[in]  delay_ms    Delay (in ms) in between sending discovery 
 *                         messages to addresses in the address list.
 * \param[in]  timeout_ms  Stop waiting for device discovery after timeout_ms milliseconds.
 *
 * \return ::kvrOK on success or any other ::kvrStatus on failure.
 */
kvrStatus WINAPI kvrDiscoveryStart(kvrDiscoveryHandle handle, uint32_t delay_ms,
                                   uint32_t timeout_ms);
 
/**
 * Call this after calling kvrDiscoveryStart(). The first call will return the first result, 
 * second call will return the second etc. 
 * Will return found devices until ::kvrERR_BLANK is returned. 
 *
 * \param[in]   handle       Discovery handle. 
 * \param[out]  device_info  Device info.
 *
 * \return ::kvrOK on success or any other ::kvrStatus on failure.
 */ 
kvrStatus WINAPI kvrDiscoveryGetResults(kvrDiscoveryHandle handle,
                                        kvrDeviceInfo *device_info);

/**
 * Store a list of devices that can be discovered later. 
 *
 * \param[in] device_info_list        A list of devices to remember. 
 * \param[in] device_info_list_size   The number of elements in \a device_info_list.
 *
 * \return ::kvrOK on success or any other ::kvrStatus on failure.
 */                                 
kvrStatus WINAPI kvrDiscoveryStoreDevices(const kvrDeviceInfo device_info_list[],
                                          uint32_t device_info_list_size);
                                      
                                      
/**
 * Turn automatic clearing of the stored devices on/off. 
 *
 * \param[in] onoff        Turn auto-clear on/off. TRUE: Stored devices will 
 * be cleared automatically when the application exits, even if the application 
 * terminates abnormally. FALSE: Stored devices will be stored until removed. 
 *
 * \return ::kvrOK on success or any other ::kvrStatus on failure.
 */                                 
kvrStatus WINAPI kvrDiscoveryClearDevicesAtExit(BOOL onoff);

/**
 * Sets the accessibility password to use when connecting to a device.
 *
 * \param[in] device_info             The device to set the password for.
 * \param[in] password                The password as a C string.
 *
 * \return ::kvrOK on success or any other ::kvrStatus on failure.
 */                                 
kvrStatus WINAPI kvrDiscoverySetPassword(kvrDeviceInfo *device_info,
                                         const char *password);
                                       
/**
 * Sets the encryption key to use when encrypting communication.
 *
 * \param[in] device_info             The device to set the password for.
 * \param[in] key                     The key as a C string.
 *
 * \return ::kvrOK on success or any other ::kvrStatus on failure.
 */                                                                        
kvrStatus WINAPI kvrDiscoverySetEncryptionKey(kvrDeviceInfo *device_info,
                                              const char *key);
                                             
                             
/**
 * Returns local connection status of the selected device as ASCII text.
 *
 * \param[in]  device_info      The device to request the status information from.
 * \param[out] buffer           The service status as a C string.
 * \param[in]  buffer_size      The service status buffer size.
 *
 * \return ::kvrOK on success or any other ::kvrStatus on failure.
 */
kvrStatus WINAPI kvrDeviceGetServiceStatusText(const kvrDeviceInfo *device_info,
                                               char *buffer,
                                               uint32_t buffer_size);


/**
 * Current service state
 * \anchor kvrServiceState_xxx
 * \name kvrServiceState_xxx
 * @{
 */
#define kvrServiceState_VOID               0  /**< Void */
#define kvrServiceState_AVAILABLE          1  /**< Device available */
#define kvrServiceState_DISCOVERED         2  /**< Device discovered */
  /**
   * Device is starting, other devices may inhibit this device from being started 
   * at the moment (e.g. by installing).
   */
#define kvrServiceState_STARTING           3  
#define kvrServiceState_STARTED            4  /**< Device is started */
#define kvrServiceState_CONNECTION_DOWN    5  /**< Connection is currently down */
#define kvrServiceState_CONNECTION_UP      6  /**< Connection is corrently up */
#define kvrServiceState_REDISCOVER         7  /**< We've lost the device - rediscover it */
#define kvrServiceState_UNWILLING          8  /**< Unwilling, see sub state for reason */
#define kvrServiceState_REDISCOVER_PENDING 9  /**< Rediscover is pending */
#define kvrServiceState_CLOSING            10 /**< Closing */
#define kvrServiceState_REMOVE_ME          11 /**< Remove me */
#define kvrServiceState_STANDBY            12 /**< Standbe */
#define kvrServiceState_CONFIG_CHANGED     13 /**< Configuration has changed */
#define kvrServiceState_STOPPING           14 /**< Stopping */
#define kvrServiceState_INSTALLING         15 /**< Device is currently being installed */
/** @} */

/**
 * Current start information
 * \anchor kvrStartInfo_xxx
 * \name kvrStartInfo_xxx
 * @{
 */
#define kvrStartInfo_NONE               0 /**< No information available */
#define kvrStartInfo_START_OK           1 /**< Started OK */
#define kvrStartInfo_ERR_IN_USE         2 /**< Already connected to someone else */
#define kvrStartInfo_ERR_PWD            3 /**< Wrong connection pwd */
#define kvrStartInfo_ERR_NOTME          4 /**< This start is not for me */
#define kvrStartInfo_ERR_CONFIGURING    5 /**< I'm being configured so won't start */
#define kvrStartInfo_ERR_PARAM          6 /**< Invalid parameters in QRV (non matching versions) */
#define kvrStartInfo_ERR_ENCRYPTION_PWD 7 /**< Wrong encryption password.  */   
/** @} */


/**
 * Returns local connection status of the selected device.
 *
 * \param[in]  device_info      The device to request the status information from.
 * \param[out] state            The service state as a \ref kvrServiceState_xxx
 * \param[out] start_info       The start information as a \ref kvrStartInfo_xxx
 *
 * \return ::kvrOK on success or any other ::kvrStatus on failure.
 */
kvrStatus WINAPI kvrDeviceGetServiceStatus(const kvrDeviceInfo *device_info,
                                           int32_t *state, int32_t *start_info);

/**@}*/




/**
 * \defgroup Helper Helper functions
 */
/**@{*/

/**
 * Convert a ::kvrStatus errorcode to a text.
 *
 * \param[in]  error         The error code to convert.
 * \param[out]  buffer       Buffer to receive error text.
 * \param[in]  buffer_size   Buffer size in bytes.
 */
kvrStatus WINAPI kvrGetErrorText(kvrStatus error, char *buffer,
                                 uint32_t buffer_size);

/**
 * Convert a ::kvrAddress to a text string. 
 * The output format depends on the \ref kvrAddressType_xxx. 
 * \a buffer_size must be the maximum lenth for each type. 
 * i.e. ::kvrAddressType_IPV4 must have lenght at least 16. 
 *
 * \param[out] buffer         The converted string as a C string.
 * \param[in]  buffer_size    Size of buffer.
 * \param[in]  address        The address to convert.
 */
kvrStatus WINAPI kvrStringFromAddress(char *buffer, uint32_t buffer_size,
                                      const kvrAddress *address);

/**
 * Convert a C string into a ::kvrAddress.
  *
 * Examples: 
 * - <b>MAC address</b><br>
 * address_string: "90:E6:BA:3C:32:12"<br>
 * type: ::kvrAddressType_MAC
 *
 * - <b>IP v.4</b><br>
 * address_string: "192.168.1.142"<br>
 * type: ::kvrAddressType_IPV4
 *
 * - <b>IP v.4 with port</b><br>
 * address_string: "192.168.1.142:8080"<br>
 * type: ::kvrAddressType_IPV4_PORT
 * 
 *
 * \param[in]  address_type    \ref kvrAddressType_xxx to convert into.
 * \param[out] address         Returned address.
 * \param[in]  address_string  C string to convert into a kvrAddress.
 */
kvrStatus WINAPI kvrAddressFromString(int32_t address_type, kvrAddress *address,
                                      const char *address_string);
 
/**
 * Returns a C string in human readable format from the security
 * information gathered with kvrWlanGetScanResults()
 *
 * Example output: 
 * "WPA2: G-TKIP (PSK) P1-AES P2-TKIP WPA: G-TKIP (PSK) P1-AES P2-TKIP"
 *
 * The above example shows a network using Pre-Shared Key with both
 * WPA2 and WPA enabled, for both modes, a group key using TKIP is
 * needed, and in both modes you can choose between AES (CCMP) and
 * TKIP as the cipher for the pairwise key.
 *
 * The length of the generated string could theoretically be up to
 * about 180 characters. If the length of the supplied security_string
 * is too short, the result will be truncated and the function will
 * return kvrERR_PARAMETER.
 *
 * \param[out] security_string       A C string.
 * \param[in]  security_string_size  Max size of security_string.
 * \param[in]  capability            The advertised capabilities of the BSS.
 * \param[in]  type_wpa              Authentication suite type.
 * \param[in]  wpa_info              Advertised information for WPA.
 * \param[in]  rsn_info              Advertised information for RSN.
 *
 * \return ::kvrOK on success or any other ::kvrStatus on failure.
 */
kvrStatus WINAPI kvrWlanGetSecurityText(char *security_string,
                                        uint32_t security_string_size,
                                        uint32_t capability,
                                        uint32_t type_wpa,
                                        const kvrCipherInfoElement *wpa_info,
                                        const kvrCipherInfoElement *rsn_info);

/**
 * Generates four 64-bit and one 128-bit WEP keys.
 *
 * All generated keys are returned as ASCII hexadecimal C strings.
 *
 * \param[in]  pass_phrase  The pass phrase as a C string.
 * \param[out] key64_1      Generated 64-bit WEP key 1 (10 + 1 bytes).
 * \param[out] key64_2      Generated 64-bit WEP key 2 (10 + 1 bytes).
 * \param[out] key64_3      Generated 64-bit WEP key 3 (10 + 1 bytes).
 * \param[out] key64_4      Generated 64-bit WEP key 4 (10 + 1 bytes).
 * \param[out] key128       Generated 128-bit WEP key  (26 + 1 bytes).
 *
 * \return ::kvrOK on success or any other ::kvrStatus on failure.
 */
kvrStatus WINAPI kvrNetworkGenerateWepKeys(const char *pass_phrase,
                                           char *key64_1, 
                                           char *key64_2,
                                           char *key64_3,
                                           char *key64_4,
                                           char *key128);

/**
 * Generates a WPA key.
 *
 * \param[in]  pass_phrase  The pass phrase as a C string.
 * \param[in]  ssid         SSID as a C string. Maximum length is 32 bytes.
 * \param[out] key          The WPA key, 256 bits as a an ASCII hexadecimal 
 *                          C string (64 + 1 bytes).
 *
 * \return ::kvrOK on success or any other ::kvrStatus on failure.
 */
kvrStatus WINAPI kvrNetworkGenerateWpaKeys(const char *pass_phrase,
                                           const char *ssid,
                                           char *key);

/**@}*/

/**
 * Initializes library stuff.
 * Call this function before calling any other kvr function.
 */
void WINAPI kvrInitializeLibrary(void);


/**
 * Unloads library stuff.
 * Call this function after calling all other kvr functions.
 */
void WINAPI kvrUnloadLibrary(void);


/**
 * Queries the status of the helper service. The helper service is installed as a part
 * of the driver package and is normally set to automatic start.
 *
 * \note This API call requires read access to the service.
 *
 * \param[out]     status             Win32 status code on failure.
 *
 * \return ::kvrOK on success (meaning that the service is running) or any other ::kvrStatus on failure.
 */                                              
kvrStatus WINAPI kvrServiceQuery(int *status);

/**
 * Starts the helper service. The helper service is installed as a part
 * of the driver package and is normally set to automatic start.
 *
 * \note This API call requires control access to the service.
 *
 * \param[out]     status             Win32 status code on failure.
 *
 * \return ::kvrOK on success (meaning that the service is started or
 * already is running) or any other ::kvrStatus on failure.
 */                                              
kvrStatus WINAPI kvrServiceStart(int *status);

/**
 * Stops the helper service. The helper service is installed as a part
 * of the driver package and is normally set to automatic start.
 *
 * \note This API call requires control access to the service.
 *
 * \param[out]     status             Win32 status code on failure.
 *
 * \return ::kvrOK on success (meaning that the service is stopped or
 * already is stopped) or any other ::kvrStatus on failure.
 */                                              
kvrStatus WINAPI kvrServiceStop(int *status);


/**
 * \mainpage Remote Device API
 *
 * <h1>THIS IS A PRELIMINARY VERSION AND SUBJECT TO CHANGE</h1>
 *
 * This is an API for remote devices.
 *
 * - \subpage page_configuration
 * - \subpage page_discovery
 * - \subpage page_network
 * - \subpage page_support
 *
 * \page page_support Support
 * For support, contact support@kvaser.com
 *
 * \page page_discovery Device discovery
 * The following is an example of calls that can be used for device discovery. 
 *
 * Initialize/Unload library
 * - kvrInitializeLibrary()  
 * - kvrUnloadLibrary()
 *
 * Get the default addresses used for discovering devices
 * - kvrDiscoveryGetDefaultAddresses()
 *
 * Discover all devices 
 * to the device list
 * - kvrDiscoveryStart()
 * - kvrDiscoveryGetResults()
 *
 * Save devices you want to remember or use. 
 * - kvrDiscoveryStoreDevices()
 *
 * Get device status
 * - kvrDeviceGetServiceStatusText()
 * 
 * \page page_configuration Local Configuration
 * When the remote device is connected to the host it can be configured. 
 * A device can hold a number of different profiles. The number of profiles 
 * that the device supports can be found by using kvrConfigNoProfilesGet(). 
 * Each profile contains a complete configuration. To configure a specific 
 * profile open it with kvrConfigOpenEx(). To activate a specific profile 
 * use kvrConfigActiveProfileSet(). The active profile is the one the 
 * device will use. 
 * 
 * 
 * The following is an example of calls that are used to configure a device. 
 *
 * Initialize/Unload library
 * - kvrInitializeLibrary() 
 * - kvrUnloadLibrary() 
 *
 * Get the number of supported profiles. 
 * - kvrConfigNoProfilesGet()
 *
 * Get and set the active profile. 
 * - kvrConfigActiveProfileGet()
 * - kvrConfigActiveProfileSet()
 *
 * Open the device's configuration area. If protected, you need to enter a
 * password as well.
 * - kvrConfigOpen()
 * - kvrConfigOpenEx()
 *
 * If password is unknown, it is possible to first clear entire area including 
 * password
 * - kvrConfigClear()
 *
 * Write new configuration and read the configuration from the device
 * - kvrConfigSet()
 * - kvrConfigGet()
 *
 * Close the device's configuration area for writing
 * - kvrConfigClose() 
 *
 * \page page_network Network information
 * The following is an example of calls that are used for network maintenance. 
 *
 * Initialize library
 * - kvrInitializeLibrary()  
 *
 * Start scan for existing WLAN networks and get the result
 * - kvrWlanStartScan()
 * - kvrWlanGetScanResults()
 *
 * Get device's IP settings
 * - kvrNetworkGetAddressInfo()
 *
 * Get status information; WLAN connection state, RSSI, RTT, TX power etc.
 * - kvrNetworkGetConnectionStatus()
 *
 * Start sending 'ping' and get latest RTT (and RSSI) values.
 * - kvrNetworkConnectionTest()
 * - kvrNetworkGetRssiRtt() 
 */

 
/**
 * \example kvrConnect.c
 * \example kvrConfig.c
 * \example kvrNetworkConnectionTest.c
 */

#ifdef __cplusplus
}
#endif
 
#endif
