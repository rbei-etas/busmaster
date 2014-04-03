#ifndef ETAS_DEV_H
#define ETAS_DEV_H

/**
* @file
* @brief      This file declares the DEV API.
* @copyright  Copyright (c) 2011 ETAS GmbH. All rights reserved.
*
* $Revision: 6779 $
*/

#include "devDefs.h"
#include "../Common/boaTypes.h"
#include "../Common/boaService.h"
#include "../Common/boaDefs.h"

#ifdef __cplusplus
extern "C" {
#endif

    /**
      @defgroup GROUP_BOA_DEV Device Interface (DEV)
      @brief    The DEV API provides access to information obtained from a connected hardware device.

      @details
      The DEV API can be used in three use cases:

      -# An instance of an ETAS relabelled hardware device usually contains some device-specific licence information in
         its firmware which determines whether the device instance is permitted to operate with an ETAS software tool.
         This licence information is sometimes referred to as an "ETAS bit" in the device's firmware.
         An ETAS software tool can use the API DEV_GetLicenceValue() to obtain this information and decide whether to
         allow the user to access the device instance.
      -# When a non-ETAS (i.e. 3rd-party) software tool is using BOA to access ETAS hardware, the 3rd-party tool may
         require ETAS hardware to act as a dongle for the tool. In order to support this, an ETAS hardware device must be
         able to encapsulate information about the licencing of various features of the 3rd-party tool. This can be achieved
         by allowing an ETAS hardware device to encapsulate an arbitrary blob of data, which is stored on the device by the
         3rd-party and later queried by the 3rd-party's software tool.
      -# When a software application is using BOA to access one or more hardware devices,
         it can use the DEV_GetDeviceInformation function to retrieve various detailed information about each device.

      <H2>Use case 1</H2>
      An instance of an ETAS relabelled hardware device usually contains some device-specific licence information in
      its firmware which determines whether the device instance is permitted to operate with an ETAS software tool.
      This licence information is sometimes referred to as an "ETAS bit" in the device's firmware.
      An ETAS software tool can use the API DEV_GetLicenceValue() to obtain this information and decide whether to
      allow the user to access the device instance.

      An ETAS software tool uses DEV_GetLicenceValue() to obtain licence information from an ETAS relabelled device as
      follows:
        -# The ETAS software tool uses @ref CSI_GetOcdAuthInfo() to authenticate the device's OCD DLL. This allows the
           ETAS software tool to trust any information which it subsequently obtained from the device's OCD DLL.
        -# The ETAS software tool calls DEV_GetLicenceValue(). This function accepts an identifier, identifying the required
           licence value, and returns a string containing the licence value. @ref DEV_LicenceValueId describes the licence
           values which are defined.
        -# The implementation of DEV_GetLicenceValue() within the device's OCD examines the device's firmware and calculates
           the licence value requested by the ETAS software tool, if necessary converting the raw information stored in
           device's firmware into a string which can be understood by the ETAS software tool.
        -# The ETAS software tool enables or disables the hardware device according to the licence value received.

      Note that although this mechanism is intended to allow an ETAS-relabelled device to expose licence information,
      ETAS's own devices must participate in this mechanism too, i.e. they must also implement DEV_GetLicenceValue().

      <H2>Use case 2</H2>
      When a non-ETAS (i.e. 3rd-party) software tool is using BOA to access ETAS hardware, the 3rd-party tool may require
      ETAS hardware to act as a dongle for the tool. In order to support this, an ETAS hardware device must be able to
      encapsulate information about the licencing of various features of the 3rd-party tool. This can be achieved by allowing
      an ETAS hardware device to encapsulate an arbitrary blob of data, which is stored on the device by the 3rd-party and
      later queried by the 3rd-party's software tool.

      The following is the sequence which is used by a 3rd-party to store licence information in an ETAS device which supports
      the DEV API, thereby turning the ETAS device into a dongle. It is expected that the 3rd-party will create a tool to
      perform this:
        -# The 3rd-party prepares a blob of licence data, which is private to the 3rd-party.
        -# The 3rd-party obtains a string containing device-specific identity information for the dongle device. On one device,
           this may be a MAC address; on another device this may be a serial number, etc. The value of this string for any
           given device can be obtained by calling the function DEV_GetDeviceId(). The value outputted by this function will
           be constant for any given device instance.
        -# To initialise an ETAS device as a dongle, the 3rd-party calls the function DEV_SetLicenceBlob(). This accepts:
            - the 3rd-party's licence blob;
            - a signature, calculated using a key which is private to the 3rd-party, over the licence blob and the
              device-specific identity string obtained via DEV_GetDeviceId(). The 3rd-party may omit this signature if he does
              not care about preventing an attacker from copying the licence blob from one device instance to another.

      This is the sequence which is followed at runtime by the 3rd-party's software tool to use the licence information
      contained in the ETAS dongle device:
        -# The 3rd-party software tool uses @ref CSI_GetOcdAuthInfo() to authenticate the ETAS device's OCD DLL. This allows the
           3rd-party software tool to trust any information which it subsequently obtains from the ETAS device.
        -# The 3rd-party software tool calls the BOA function DEV_GetLicenceBlob(). This function returns the information which
           was stored by DEV_SetLicenceBlob(), i.e.:
            - the licence blob;
            - a signature, calculated using a key which is private to the 3rd-party, over the licence blob and the
              device-specific identity string obtained via DEV_GetDeviceId().
        -# The 3rd-party software calls the BOA function DEV_GetDeviceId() to obtain the device's identity string.
        -# The 3rd-party software uses the 3rd-party's public key to check that the signature returned by DEV_GetLicenceBlob()
           is correct. This confirms that:
            - the licence blob has not been modified;
            - the licence blob has not been copied to a different device.
        -# Finally, the 3rd-party measurement tool enables or disables its various features according to the contents of the
           licence blob.

      <H2>Use case 3</H2>
      ETAS INCA wishes to implement software licensing of devices i.e. enables or disables its various features according to the
      contents of the device information retrieved (e.g. manufacturer, connection bus etc.).

    @{
    */

    /**
     * Internal identifier for the DEV interface.
     */
#define UUID_DEV {0xA4,0x79,0xF2,0x0A,0x41,0x60,0x45,0xe7,0xBF,0xA9,0x08,0x09,0x0A,0xF2,0x59,0xDA}


    /** The type used to refer to DEV API session handles. */
    typedef BOA_Handle      DEV_SessionHandle;
    /** The type returned by DEV API functions. */
    typedef BOA_ResultCode  DEV_ErrorCode;

    /**
     * This enum represents how the device is connected to the PC (ETH/USB etc.)
     */
    typedef enum DEV_BusInterface
    {
        /** local PCI card e.g. PCI-Express */
        PCI = 0,

        /** PCMCIA */
        PCMCIA,

        /** USB */
        USB,

        /** Ethernet */
        ETH,
    } DEV_BusInterface;

    /**
     * This enum represents the seller of a device (ETAS/3rdParty etc.)
     */
    typedef enum DEV_Vendor
    {
        ETAS = 0,

        THIRD_PARTY
    } DEV_Vendor;

    /**
     * This enum represents the maker of a device (Siemens/Kvaser etc.)
     */
    typedef enum DEV_Manufacturer
    {
        BEBRO = 0,

        SIEMENS,

        WONGS,

        KVASER,

        VECTOR
    } DEV_Manufacturer;


    /**
    * Structure containing specific information for a device.
    *
    * Note that a future version of the DEV API may define extra fields on the end of this structure. Furthermore,
    * as described in @ref BackwardsCompatibilityPage, such fields should not be accessed unless, at runtime, it can be
    * determined that both client and implementation of DEV support the required version of the DEV API. Therefore,
    * when re-compiling code in order to use a new version of this structure, developers should ensure that the code
    * does not unintentionally access such fields, e.g. by using memset() or memcpy().
    */
    typedef struct DEV_DeviceInformation
    {
        /** An integer value that represents a type of device of a specific manufacturer (ES595/ES593D etc.) */
        uint32 deviceID;

        /** enum that represents the maker of a device (Siemens/KVaser etc.). */
        DEV_Manufacturer manufacturerID;

        /** enum that represents the seller of a device (ETAS/3rdParty etc.). */
        DEV_Vendor vendorID;

        /** Identifying string value that is unique and represents a specific device instance */
        char serialNumber[BOA_MAX_PARAM];

        /** enum that represents how the device is connected to the PC (ETH/USB etc.) */
        DEV_BusInterface busInterface;
    } DEV_DeviceInformation;


    /**
      Create a new API session and bind it to a hardware controller identified by a URI.

      @deprecated This function is deprecated, please use @ref DEV_CreateSessionVersion instead.

      This function creates an API session, using a URI to identify the location of the API. The type of the API is
      inferred automatically, or may be hinted by the URI.

      @param[in]  uriLocation   A URI identifying the hardware controller which will be used by the API session.

      @param[out] phSession     The handle to the created API session. This handle must be used in any subsequent call
                                to access the session.

      @return
      <TABLE border="0" rules="rows" cellspacing="3" cellpadding="3">
       <tr> <td> @ref BOA_SUCCESS </td>
            <td> The operation was successful. The session handle is valid and can be used in further calls
                 to the API. </td>
       </tr>
       <tr> <td> @ref BOA_ERR_INVALID_PARAMETER </td>
            <td> @p uriLocation does not begin with the sequence "ETAS://", or @p phSession is NULL. </td>
       </tr>
       <tr> <td> @ref BOA_ERR_HW_NOT_PRESENT </td>
            <td> The operation failed because the URI specified a hardware controller which could not be found.
                 Probably the hardware is not connected or @p uriLocation is incorrect. </td>
       </tr>
       <tr> <td> @ref BOA_ERR_NO_RESOURCES </td>
            <td> The driver or CSI component ran into a resource problem and can not instantiate a new handle. Destroy
                 some unneeded handles to release resources and try again. </td>
       </tr>
       <tr> <td> @ref BOA_ERR_DRIVER_DISCONNECTED or @ref BOA_ERR_CANNOT_FIND_CHILDREN </td>
            <td> Although @p uriLocation identifies a valid hardware controller, the controller could not be reached.
                 The hardware may have been removed, or one of the drivers required to reach the hardware may be
                 experiencing a problem. </td>
       </tr>
       <tr> <td> @ref BOA_ERR_OUT_OF_MEMORY </td>
            <td> The driver ran out of memory. This error should only occur on heavy loaded systems with multiple
                 clients and large send and receive queues. The error may indicate a resource leak in the driver
                 implementation. When multiple application access the same resource (interface hardware or physical
                 controller) the termination of a different application may return the resources required for this
                 instance. </td>
       </tr>
       <tr> <td> @ref BOA_ERR_PROTOCOL_VERSION_NOT_SUPPORTED or @ref BOA_ERR_NO_INTERFACE </td>
            <td> The driver used to access the hardware controller specified by @p uriLocation does not support the
                 version of the DEV API which is required by the BOA framework. </td>
       </tr>
       <tr> <td> @ref BOA_ERR_NONUNIQUE_BIND_TARGET </td>
            <td> The @p uriLocation does not identify a unique hardware controller. </td>
       </tr>
       <tr> <td> @ref BOA_ERR_INVALID_ACCESS_SYNTAX </td>
            <td> The driver used to access the hardware controller specified by @p uriLocation exports an access parameter
                 which is of a form that cannot be parsed by the BOA framework. </td>
       </tr>
       <tr> <td> @ref BOA_ERR_INVALID_ACCESS_PARAM </td>
            <td> The BOA framework failed to access a driver which is required to establish communication with the
                 hardware controller specified by @p uriLocation. This error code indicates a transient failure; a
                 permanent failure would be indicated (e.g.) via BOA_ERR_HW_NOT_PRESENT. </td>
       </tr>
      </TABLE>
      */

#ifndef DEV_DYNAMICDLL
    BOA_DECLSPEC_DEPRECATED_FUNC( DEV_DECLSPEC DEV_ErrorCode
                                  DEV_CALL DEV_CreateSession(
                                      const char*         uriLocation,
                                      DEV_SessionHandle*  phSession
                                  ));
#endif

    typedef DEV_ErrorCode
    (DEV_CALL* PF_DEV_CreateSession)(
        const char*         uriLocation,
        DEV_SessionHandle*  phSession );


    /**
      Create a new API session and bind it to a hardware controller identified by a URI.

      This function creates an API session, using a URI to identify the location of the API. The type of the API is
      inferred automatically, or may be hinted by the URI.

      @param[in]  uriLocation   A URI identifying the hardware controller which will be used by the API session.

      @param[in]  apiVersion    The API version required.

      @param[out] phSession     The handle to the created API session. This handle must be used in any subsequent call
                                to access the session.

      @return
      <TABLE border="0" rules="rows" cellspacing="3" cellpadding="3">
       <tr> <td> @ref BOA_SUCCESS </td>
            <td> The operation was successful. The session handle is valid and can be used in further calls
                 to the API. </td>
       </tr>
       <tr> <td> @ref BOA_ERR_INVALID_PARAMETER </td>
            <td> @p uriLocation does not begin with the sequence "ETAS://", or @p phSession is NULL. </td>
       </tr>
       <tr> <td> @ref BOA_ERR_HW_NOT_PRESENT </td>
            <td> The operation failed because the URI specified a hardware controller which could not be found.
                 Probably the hardware is not connected or @p uriLocation is incorrect. </td>
       </tr>
       <tr> <td> @ref BOA_ERR_NO_RESOURCES </td>
            <td> The driver or CSI component ran into a resource problem and can not instantiate a new handle. Destroy
                 some unneeded handles to release resources and try again. </td>
       </tr>
       <tr> <td> @ref BOA_ERR_DRIVER_DISCONNECTED or @ref BOA_ERR_CANNOT_FIND_CHILDREN </td>
            <td> Although @p uriLocation identifies a valid hardware controller, the controller could not be reached.
                 The hardware may have been removed, or one of the drivers required to reach the hardware may be
                 experiencing a problem. </td>
       </tr>
       <tr> <td> @ref BOA_ERR_OUT_OF_MEMORY </td>
            <td> The driver ran out of memory. This error should only occur on heavy loaded systems with multiple
                 clients and large send and receive queues. The error may indicate a resource leak in the driver
                 implementation. When multiple application access the same resource (interface hardware or physical
                 controller) the termination of a different application may return the resources required for this
                 instance. </td>
       </tr>
       <tr> <td> @ref BOA_ERR_PROTOCOL_VERSION_NOT_SUPPORTED or @ref BOA_ERR_NO_INTERFACE </td>
            <td> The driver used to access the hardware controller specified by @p uriLocation does not support the
                 version of the DEV API which is required by the BOA framework. </td>
       </tr>
       <tr> <td> @ref BOA_ERR_NONUNIQUE_BIND_TARGET </td>
            <td> The @p uriLocation does not identify a unique hardware controller. </td>
       </tr>
       <tr> <td> @ref BOA_ERR_INVALID_ACCESS_SYNTAX </td>
            <td> The driver used to access the hardware controller specified by @p uriLocation exports an access parameter
                 which is of a form that cannot be parsed by the BOA framework. </td>
       </tr>
       <tr> <td> @ref BOA_ERR_INVALID_ACCESS_PARAM </td>
            <td> The BOA framework failed to access a driver which is required to establish communication with the
                 hardware controller specified by @p uriLocation. This error code indicates a transient failure; a
                 permanent failure would be indicated (e.g.) via BOA_ERR_HW_NOT_PRESENT. </td>
       </tr>
      </TABLE>
    */
#ifndef DEV_DYNAMICDLL
    DEV_DECLSPEC DEV_ErrorCode
    DEV_CALL DEV_CreateSessionVersion(
        const char*         uriLocation,
        const BOA_Version*  apiVersion,
        DEV_SessionHandle*  phSession );
#endif

    struct CSI_Tree;

    /**
      Create a new API session and bind it to a hardware controller identified by a URI, but without
      searching for connected hardware.

      This function is identical to @ref DEV_CreateSessionVersion(), but, whereas @ref DEV_CreateSessionVersion()
      searches for connected hardware every time it is invoked, @ref DEV_CreateSessionNoSearch() does not
      perform such a search itself and instead relies on a previously-obtained search result supplied via the
      @p pConnectedHw argument. This search result can be created separately via @ref CSI_CreateProtocolTree().

      This function allows a client to perform a single time-consuming search for hardware, then rapidly create
      several DEV API sessions by re-using the same search results.

      @param[in]  uriLocation   A URI identifying the hardware controller which will be used by the API session.

      @param[in]  version       The API version required.

      @param[in]  pConnectedHw  A tree, created via @ref CSI_CreateProtocolTree(), representing the connected hardware.
                                When this tree was created, the URI argument provided to @ref CSI_CreateProtocolTree()
                                must have been a substring of the URI argument @p uriLocation, and the CSI_NodeRange
                                argument must have specified a range from CSI_NODE_MIN to CSI_NODE_MAX.

      @param[out] phSession     The handle to the created API session. This handle must be used in any subsequent call
                                to access the session.

      @return
      See @ref DEV_CreateSessionVersion().
    */
#ifndef DEV_DYNAMICDLL
    DEV_DECLSPEC DEV_ErrorCode
    DEV_CALL DEV_CreateSessionNoSearch(
        const char*         uriLocation,
        const BOA_Version*  apiVersion,
        struct CSI_Tree*    pConnectedHw,
        DEV_SessionHandle*  phSession );
#endif

    /**
      Create a new API session and bind it to a hardware identified by a protocol stack.

      This function creates an API session, using a protocol stack to identify the location and type of the API.

      @param[in]  pProtocolStack    A protocol stack created by CSI_CreateInterfaceStack().

      @param[out] phSession         The handle to the created API session. This handle must be used in any subsequent
                                    call to access the session.

      @return
      <TABLE border="0" rules="rows" cellspacing="3" cellpadding="3">
       <tr> <td> @ref BOA_SUCCESS </td>
            <td> The operation was successful. The session handle is valid and can be used in further calls
                 to the API. </td>
       </tr>
       <tr> <td> @ref BOA_ERR_INVALID_PARAMETER </td>
            <td> @p pProtocolStack or @p phSession is NULL. </td>
       </tr>
       <tr> <td> @ref BOA_ERR_NO_RESOURCES </td>
            <td> The driver or CSI component ran into a resource problem and can not instantiate a new handle. Destroy
                 some unneeded handles to release resources and try again. </td>
       </tr>
       <tr> <td> @ref BOA_ERR_DRIVER_DISCONNECTED </td>
            <td> The removable hardware identified by @p pProtocolStack could not be reached. The hardware may have been
                 removed, or one of the drivers required to reach the hardware may be experiencing a problem. </td>
       </tr>
       <tr> <td> @ref BOA_ERR_OUT_OF_MEMORY </td>
            <td> The driver ran out of memory. This error should only occur on heavy loaded systems with multiple
                 clients and large send and receive queues. The error may indicate a resource leak in the driver
                 implementation. When multiple applications access the same resource (interface hardware or physical
                 controller) the termination of a different application may return the resources required for this
                 instance. </td>
       </tr>
       <tr> <td> @ref BOA_ERR_PROTOCOL_VERSION_NOT_SUPPORTED </td>
            <td> The API and/or version specified by @p pProtocolStack is not supported. This error will occur if
                 the driver used to access the hardware controller specified by @p pProtocolStack does not support the
                 DEV API, or if the driver does not support the version of the DEV API requested by @p pProtocolStack.
                 This can indicate a problem with the driver's support for the DEV API, or it may indicate that
                 @p pProtocolStack does not refer to the DEV API at all. </td>
       </tr>
       <tr> <td> @ref BOA_ERR_INVALID_ACCESS_SYNTAX </td>
            <td> The driver used to access the hardware controller specified by @p pProtocolStack exports an access
                 parameter which is of a form that cannot be parsed by the BOA framework. </td>
       </tr>
       <tr> <td> @ref BOA_ERR_INVALID_ACCESS_PARAM </td>
            <td> The BOA framework failed to access a driver which is required to establish communication with the
                 hardware controller specified by @p pProtocolStack. This error code indicates a transient failure; a
                 permanent failure would be indicated (e.g.) via BOA_ERR_DRIVER_DISCONNECTED. </td>
       </tr>
      </TABLE>
    */

#ifndef DEV_DYNAMICDLL
    DEV_DECLSPEC DEV_ErrorCode
    DEV_CALL DEV_CreateSessionBind(
        const BOA_ProtocolStack*    pProtocolStack,
        DEV_SessionHandle*          phSession );
#endif

    typedef DEV_ErrorCode
    (DEV_CALL* PF_DEV_CreateSessionBind)(
        const BOA_ProtocolStack*    pProtocolStack,
        DEV_SessionHandle*          phSession );

    /**
      Destroy an API session created by DEV_CreateSession() or DEV_CreateSessionBind().

      @param[in] hSession   Handle of the API session that will be destroyed.

      @return
      <TABLE border="0" rules="rows" cellspacing="3" cellpadding="3">
       <tr> <td> @ref BOA_SUCCESS </td>
            <td> The operation was successful. The @a session handle must not be used any more. </td>
       </tr>
       <tr> <td> @ref BOA_ERR_INVALID_HANDLE </td>
            <td> The @p hSession handle is not valid. Perhaps it has been destroyed already. </td>
       </tr>
      </TABLE>
    */

#ifndef DEV_DYNAMICDLL
    DEV_DECLSPEC DEV_ErrorCode
    DEV_CALL DEV_DestroySession(
        DEV_SessionHandle hSession );
#endif

    typedef DEV_ErrorCode
    (DEV_CALL* PF_DEV_DestroySession)(
        DEV_SessionHandle hSession );

    /**
     * This enum lists the IDs of the licence value which can be obtained via @ref DEV_GetLicenceValue().
     */
    typedef enum DEV_LicenceValueId
    {
        /** This licence value indicates whether or not the current session is permitted to be used by ETAS INCA.
         * The licence value is expected to be a string containing either "1" (i.e. yes) or "0" (i.e. no). */
        DEV_OPERATES_WITH_INCA = 1
    } DEV_LicenceValueId;

    /** The maximum length, including a NULL terminator, of a licence value obtained via @ref DEV_GetLicenceValue(). */
#define DEV_MAX_LICENCE_VALUE 128

    /** The type of a licence value obtained via @ref DEV_GetLicenceValue(). */
    typedef char DEV_LicenceValue[DEV_MAX_LICENCE_VALUE];

    /**
      This function obtains a specified licence value from a specified DEV API session.

      The DEV API defines various "licence value IDs" in the enum @ref DEV_LicenceValueId. Each entry in this enum identifies
      a licence value, which can be requested from an implementation of the DEV API. The licence value contains licence
      information for that particular implementation of the DEV API.

      Note that, obviously, an implementation of the DEV API could lie in its response to @ref DEV_GetLicenceValue(). Therefore,
      before relying on the information obtained from @ref DEV_GetLicenceValue(), a BOA client should use @ref CSI_GetOcdAuthInfo()
      to confirm that the OCD DLL which implements the DEV API is legitimate.

      The use case which is supported by @ref DEV_GetLicenceValue() is discussed more fully in @ref GROUP_BOA_DEV.

      @param[in] hSession           Handle of the API session whose licence value is to be obtained.
      @param[in] licenceValueId     An identifier specifying the licence value which is to be obtained.
      @param[in] licenceValueBuf    A buffer which this function will populate with the requested licence value.

      @return
      <TABLE border="0" rules="rows" cellspacing="3" cellpadding="3">
       <tr> <td> @ref BOA_SUCCESS </td>
            <td> The operation was successful. @p licenceValueBuf contains the requested licence value. </td>
       </tr>
       <tr> <td> @ref BOA_ERR_INVALID_HANDLE </td>
            <td> The @p hSession handle is not valid. Perhaps the session has been destroyed already. </td>
       </tr>
       <tr> <td> @ref BOA_ERR_INVALID_PARAMETER </td>
            <td> @p licenceValueBuf is NULL, or @p licenceValueId has an unexpected value. </td>
       </tr>
       <tr> <td> @ref BOA_ERR_DRIVER_NO_RESPONSE </td>
            <td> The implementation of the DEV API which is associated with @p hSession could not be reached. Perhaps
                 the associated hardware device has been disconnected. </td>
       </tr>
      </TABLE>
    */

#ifndef DEV_DYNAMICDLL
    DEV_DECLSPEC DEV_ErrorCode
    DEV_CALL DEV_GetLicenceValue(
        DEV_SessionHandle   hSession,
        DEV_LicenceValueId  licenceValueId,
        DEV_LicenceValue    licenceValueBuf );
#endif

    typedef DEV_ErrorCode
    (DEV_CALL* PF_DEV_GetLicenceValue)(
        DEV_SessionHandle   hSession,
        DEV_LicenceValueId  licenceValueId,
        DEV_LicenceValue    licenceValueBuf );

    /** The maximum length, including a NULL terminator, of a device ID obtained via @ref DEV_GetDeviceId(). */
#define DEV_MAX_DEVICE_ID 128

    /** The type of a device ID obtained via @ref DEV_GetDeviceId(). */
    typedef char DEV_DeviceId[DEV_MAX_DEVICE_ID];

    /**
      This function obtains a unqiue identifier for the device which is associated with a given DEV API session. The identifier
      is unique to this device instance, and constant over time. A good example is a MAC address or a serial number, but the
      actual form and meaning of the identifier is specific to the device.

      The caller of this function is expected to treat the device ID as opaque, and is strongly encouraged @em not to attempt
      to interpret the value of the device ID.

      The use case which is supported by @ref DEV_GetDeviceId() is discussed more fully in @ref GROUP_BOA_DEV.

      @param[in] hSession       Handle of an API session associated with the device whose ID is to be obtained.
      @param[in] deviceIdBuf    A buffer which this function will populate with the requested ID.

      @return
      <TABLE border="0" rules="rows" cellspacing="3" cellpadding="3">
       <tr> <td> @ref BOA_SUCCESS </td>
            <td> The operation was successful. @p deviceIdBuf contains the requested device ID. </td>
       </tr>
       <tr> <td> @ref BOA_ERR_INVALID_HANDLE </td>
            <td> The @p hSession handle is not valid. Perhaps the session has been destroyed already. </td>
       </tr>
       <tr> <td> @ref BOA_ERR_INVALID_PARAMETER </td>
            <td> @p deviceIdBuf is NULL. </td>
       </tr>
       <tr> <td> @ref BOA_ERR_DRIVER_NO_RESPONSE </td>
            <td> The implementation of the DEV API which is associated with @p hSession could not be reached. Perhaps
                 the associated hardware device has been disconnected. </td>
       </tr>
      </TABLE>
    */

#ifndef DEV_DYNAMICDLL
    DEV_DECLSPEC DEV_ErrorCode
    DEV_CALL DEV_GetDeviceId(
        DEV_SessionHandle   hSession,
        DEV_DeviceId        deviceIdBuf );
#endif

    typedef DEV_ErrorCode
    (DEV_CALL* PF_DEV_GetDeviceId)(
        DEV_SessionHandle   hSession,
        DEV_DeviceId        deviceIdBuf );

    /** The maximum length, including a NULL terminator, of the name of a licence blob, as set by @ref DEV_SetLicenceBlob(). */
#define DEV_MAX_BLOBNAME_LEN 64

    /** This type is used to name a licence blob, when the blob is being manipulated by DEV_SetLicenceBlob() and DEV_GetLicenceBlob(). */
    typedef char DEV_LicenceBlobName[DEV_MAX_BLOBNAME_LEN];

    /**
      This function is used to store a blob of licence information in the firmware of the device which is associated with a
      given DEV API session.

      The blob of licence information is opaque to the device; it is only meaningful to the caller. The caller can retrieve
      the blob via @ref DEV_GetLicenceBlob().

      If desired, the caller can secure the blob against tampering by supplying a signature calculated over the blob and the
      device's unique ID, as obtained from DEV_GetDeviceId(). Supplying such a signature will ensure that attempts to tamper
      with the blob, or copy it to a different device, will be detected.

      The use case which is supported by @ref DEV_SetLicenceBlob() is discussed more fully in @ref GROUP_BOA_DEV.

      @param[in]  hSession          Handle of an API session associated with the device whose licence blob is to be modified.
      @param[in]  blobName          The name of the new licence blob. This name should be chosen so as to distinguish the blob
                                    from any other blobs stored on the device, possibly by other parties. The name should be a
                                    NULL-terminated string. An existing blobName will not be overwritten. The application must
                                    erase a licence-blob and re-create it to avoid an unintended implicit erasure of an existing
                                    blob.
      @param[in]  pBlob             The new licence blob. This may be NULL, typically in one of two use cases:
                                    - The caller may set @p pBlob to NULL simply to erase the current licence blob.
                                    - The caller may set @p pBlob to NULL to allow him to discover the maximum permitted length
                                      of the licence blob before attempting to set a new licence blob.
                                    Note that @p pBlob is NULL iff @p pBlobSig is NULL.
      @param[in]  pBlobSig          The signature of the new licence blob. This may be NULL, typically in one of two use cases:
                                    - The caller may set @p pBlobSig to NULL simply to erase the current licence blob signature.
                                    - The caller may set @p pBlobSig to NULL to allow him to discover the maximum permitted length
                                      of the licence blob signature before attempting to set a new licence blob signature.
                                    Note that @p pBlobSig is NULL iff @p pBlob is NULL.
      @param[in]  lenBlob           The number of bytes at @p pBlob.
      @param[in]  lenBlobSig        The number of bytes at @p pBlobSig.
      @param[out] pMaxLenBlob       The maximum permitted length of the licence blob, measured in bytes. This may be NULL if the
                                    caller does not need this information.
      @param[out] pMaxLenBlobSig    The maximum permitted length of the licence blob signature, measured in bytes. This may be
                                    NULL if the caller does not need this information.

      @return
      <TABLE border="0" rules="rows" cellspacing="3" cellpadding="3">
       <tr> <td> @ref BOA_SUCCESS </td>
            <td> The operation was successful. </td>
       </tr>
       <tr> <td> @ref BOA_ERR_INVALID_HANDLE </td>
            <td> The @p hSession handle is not valid. Perhaps the session has been destroyed already. </td>
       </tr>
       <tr> <td> @ref BOA_ERR_INVALID_PARAMETER </td>
            <td> @p pBlob is NULL, even though @p lenBlob is non-zero; or @p pBlobSig is NULL, even though @p lenBlobSig is
                 non-zero; or blobName is NULL or empty; or exactly one of @p pBlob and @p pBlobSig is NULL. </td>
       </tr>
       <tr> <td> @ref BOA_ERR_BUFFER_OVERFLOW </td>
            <td> @p lenBlob is larger than the maximum permitted length (indicated by @p pMaxLenBlob); or @p lenBlobSig is
                 larger than the maximum permitted length (indicated by @p pMaxLenBlobSig) </td>
       </tr>
       <tr> <td> @ref BOA_ERR_DRIVER_NO_RESPONSE </td>
            <td> The implementation of the DEV API which is associated with @p hSession could not be reached. Perhaps
                 the associated hardware device has been disconnected. </td>
       </tr>
       <tr> <td> @ref BOA_ERR_NOT_IMPLEMENTED </td>
            <td> The implementation of the DEV API which is associated with @p hSession does not implement this function.
                 Probably the associated hardware device is not capable of storing a licence blob in its firmware. </td>
       </tr>
      </TABLE>
    */

#ifndef DEV_DYNAMICDLL
    DEV_DECLSPEC DEV_ErrorCode
    DEV_CALL DEV_SetLicenceBlob(
        DEV_SessionHandle   hSession,
        DEV_LicenceBlobName blobName,
        uint8*              pBlob,
        uint8*              pBlobSig,
        uint32              lenBlob,
        uint32              lenBlobSig,
        uint32*             pMaxLenBlob,
        uint32*             pMaxLenBlobSig );
#endif

    typedef DEV_ErrorCode
    (DEV_CALL* PF_DEV_SetLicenceBlob)(
        DEV_SessionHandle   hSession,
        DEV_LicenceBlobName blobName,
        uint8*              pBlob,
        uint8*              pBlobSig,
        uint32              lenBlob,
        uint32              lenBlobSig,
        uint32*             pMaxLenBlob,
        uint32*             pMaxLenBlobSig );

    /**
      This function is used to retrieve a blob of licence information in the firmware of the device  which is associated with a
      given DEV API session. The blob will have been previously stored in the device's firmware via DEV_SetLicenceBlob().

      The use case which is supported by @ref DEV_SetLicenceBlob() is discussed more fully in @ref GROUP_BOA_DEV.

      @param[in]  hSession      Handle of an API session associated with the device whose licence blob is to be obtained.
      @param[in]  blobName      The name of the requested licence blob, as set by @ref DEV_SetLicenceBlob().
      @param[in]  pBlobBuf      A buffer which the function will fill with the current licence blob, if any. The buffer may be NULL,
                                allowing the caller to call this function once (with a NULL @p pBlobBuf) to discover the size of the
                                current licence blob, and a second time (with a non-NULL @p pBlobBuf) to actually retrieve the
                                current licence blob.
      @param[in]  pBlobSigBuf   A buffer which the function will fill with the current licence blob signature, if any. The buffer
                                may be NULL, allowing the caller to call this function once (with a NULL @p pBlobSigBuf) to discover
                                the size of the current licence blob signature, and a second time (with a non-NULL @p pBlobSigBuf)
                                to actually retrieve the current licence blob signature.
      @param[in]  lenBlobBuf    The number of bytes at @p pBlobBuf.
      @param[in]  lenBlobSigBuf The number of bytes at @p pBlobSigBuf.
      @param[out] pLenBlob      The length of the current licence blob, measured in bytes.
      @param[out] pLenBlobSig   The length of the current licence blob signature, measured in bytes.

      @return
      <TABLE border="0" rules="rows" cellspacing="3" cellpadding="3">
       <tr> <td> @ref BOA_SUCCESS </td>
            <td> The operation was successful. </td>
       </tr>
       <tr> <td> @ref BOA_ERR_INVALID_HANDLE </td>
            <td> The @p hSession handle is not valid. Perhaps the session has been destroyed already. </td>
       </tr>
       <tr> <td> @ref BOA_ERR_INVALID_PARAMETER </td>
            <td> @p pBlobBuf is NULL, even though @p lenBlobBuf is non-zero; or @p pBlobSigBuf is NULL, even though @p lenBlobSigBuf
                 is non-zero; or @p pLenBlob is NULL; or @p pLenBlobSig is NULL; or @p blobName is NULL or empty; or @p blobName does
                 not match any licence blob which has been stored on the device by @ref DEV_SetLicenceBlob(). </td>
       </tr>
       <tr> <td> @ref BOA_ERR_BUFFER_OVERFLOW </td>
            <td> @p lenBlobBuf is shorter than the length indicated by @p pLenBlob; or @p lenBlobSigBuf is shorter than the length
                 indicated by @p pLenBlobSig </td>
       </tr>
       <tr> <td> @ref BOA_ERR_DRIVER_NO_RESPONSE </td>
            <td> The implementation of the DEV API which is associated with @p hSession could not be reached. Perhaps
                 the associated hardware device has been disconnected. </td>
       </tr>
       <tr> <td> @ref BOA_ERR_NOT_IMPLEMENTED </td>
            <td> The implementation of the DEV API which is associated with @p hSession does not implement this function.
                 Probably the associated hardware device is not capable of storing a licence blob in its firmware. </td>
       </tr>
      </TABLE>
    */

#ifndef DEV_DYNAMICDLL
    DEV_DECLSPEC DEV_ErrorCode
    DEV_CALL DEV_GetLicenceBlob(
        DEV_SessionHandle   hSession,
        DEV_LicenceBlobName blobName,
        uint8*              pBlobBuf,
        uint8*              pBlobSigBuf,
        uint32              lenBlobBuf,
        uint32              lenBlobSigBuf,
        uint32*             pLenBlob,
        uint32*             pLenBlobSig );
#endif

    typedef DEV_ErrorCode
    (DEV_CALL* PF_DEV_GetLicenceBlob)(
        DEV_SessionHandle   hSession,
        DEV_LicenceBlobName blobName,
        uint8*              pBlobBuf,
        uint8*              pBlobSigBuf,
        uint32              lenBlobBuf,
        uint32              lenBlobSigBuf,
        uint32*             pLenBlob,
        uint32*             pLenBlobSig );


    /**
      This function is used to retrieve information from the firmware of the device. Added for v1.1 of DEV API.

      @param[in]  hSession              Handle of an API session associated with the device whose information is to be obtained.
      @param[out] deviceInformation     A structure which the function will fill with specific information for the device.

      @return
      <TABLE border="0" rules="rows" cellspacing="3" cellpadding="3">
       <tr> <td> @ref BOA_SUCCESS </td>
            <td> The operation was successful. </td>
       </tr>
       <tr> <td> @ref BOA_ERR_INVALID_HANDLE </td>
            <td> The @p hSession handle is not valid. Perhaps the session has been destroyed already. </td>
       </tr>
       <tr> <td> @ref BOA_ERR_INVALID_PARAMETER </td>
            <td> @p deviceInformation is NULL </td>
       </tr>
       <tr> <td> @ref BOA_ERR_DRIVER_NO_RESPONSE </td>
            <td> The implementation of the DEV API which is associated with @p hSession could not be reached. Perhaps
                 the associated hardware device has been disconnected. </td>
       </tr>
       <tr> <td> @ref BOA_ERR_NOT_IMPLEMENTED </td>
            <td> The implementation of the DEV API which is associated with @p hSession does not implement this function.
                 Probably the associated hardware device is not capable of storing a licence blob in its firmware. </td>
       </tr>
      </TABLE>
    */

#ifndef DEV_DYNAMICDLL
    DEV_DECLSPEC DEV_ErrorCode DEV_CALL DEV_GetDeviceInformation(
        DEV_SessionHandle   hSession,
        DEV_DeviceInformation* deviceInformation );
#endif

    typedef DEV_ErrorCode
    (DEV_CALL* PF_DEV_GetDeviceInformation)(
        DEV_SessionHandle   hSession,
        DEV_DeviceInformation* deviceInformation );


    /**
    * Internal v-table for the DEV interface.
    */
    typedef struct DEV_VTable
    {
        PF_DEV_CreateSession        createSession;
        PF_DEV_CreateSessionBind    createSessionBind;
        PF_DEV_DestroySession       destroySession;

        PF_DEV_GetLicenceValue      getLicenceValue;
        PF_DEV_GetDeviceId          getDeviceId;
        PF_DEV_SetLicenceBlob       setLicenceBlob;
        PF_DEV_GetLicenceBlob       getLicenceBlob;
        PF_DEV_GetDeviceInformation getDeviceInformation;   // Added for v1.1 of DEV_API
    } DEV_VTable;

    /** @} */

#ifdef __cplusplus
}
#endif

#endif
