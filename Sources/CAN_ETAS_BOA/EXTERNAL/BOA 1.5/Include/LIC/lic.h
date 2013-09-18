#ifndef ETAS_LIC_H
#define ETAS_LIC_H

/**
* @file       
* @brief      This file declares the LIC API.
* @copyright  Copyright (c) 2011 ETAS GmbH. All rights reserved.
*
* $Revision: 5815 $
*/

#include "licDefs.h"
#include "../Common/boaTypes.h"
#include "../Common/boaService.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
  @defgroup GROUP_BOA_LIC Licence Interface (LIC)
  @brief    The LIC API provides licence information obtained from a connected hardware controller.

  @details
  The LIC API can be used in two use cases:

  -# An instance of an ETAS relabelled hardware device usually contains some device-specific licence information in
     its firmware which determines whether the device instance is permitted to operate with an ETAS software tool.
     This licence information is sometimes referred to as an "ETAS bit" in the device's firmware.
     An ETAS software tool can use the API LIC_GetLicenceValue() to obtain this information and decide whether to
     allow the user to access the device instance.

  -# When a non-ETAS (i.e. 3rd-party) software tool is using BOA to access ETAS hardware, the 3rd-party tool may
     require ETAS hardware to act as a dongle for the tool. In order to support this, an ETAS hardware device must be
     able to encapsulate information about the licencing of various features of the 3rd-party tool. This can be achieved
     by allowing an ETAS hardware device to encapsulate an arbitrary blob of data, which is stored on the device by the
     3rd-party and later queried by the 3rd-party's software tool.

  <H2>Use case 1</H2>
  An instance of an ETAS relabelled hardware device usually contains some device-specific licence information in
  its firmware which determines whether the device instance is permitted to operate with an ETAS software tool.
  This licence information is sometimes referred to as an "ETAS bit" in the device's firmware.
  An ETAS software tool can use the API LIC_GetLicenceValue() to obtain this information and decide whether to
  allow the user to access the device instance.

  An ETAS software tool uses LIC_GetLicenceValue() to obtain licence information from an ETAS relabelled device as
  follows:
    -# The ETAS software tool uses @ref CSI_GetOcdAuthInfo() to authenticate the device's OCD DLL. This allows the
       ETAS software tool to trust any information which it subsequently obtained from the device's OCD DLL.
    -# The ETAS software tool calls LIC_GetLicenceValue(). This function accepts an identifier, identifying the required
       licence value, and returns a string containing the licence value. @ref LIC_LicenceValueId describes the licence
       values which are defined.
    -# The implementation of LIC_GetLicenceValue() within the device's OCD examines the device's firmware and calculates
       the licence value requested by the ETAS software tool, if necessary converting the raw information stored in
       device's firmware into a string which can be understood by the ETAS software tool.
    -# The ETAS software tool enables or disables the hardware device according to the licence value received.

  Note that although this mechanism is intended to allow an ETAS-relabelled device to expose licence information,
  ETAS's own devices must participate in this mechanism too, i.e. they must also implement LIC_GetLicenceValue().

  <H2>Use case 2</H2>
  When a non-ETAS (i.e. 3rd-party) software tool is using BOA to access ETAS hardware, the 3rd-party tool may require
  ETAS hardware to act as a dongle for the tool. In order to support this, an ETAS hardware device must be able to
  encapsulate information about the licencing of various features of the 3rd-party tool. This can be achieved by allowing
  an ETAS hardware device to encapsulate an arbitrary blob of data, which is stored on the device by the 3rd-party and
  later queried by the 3rd-party's software tool.

  The following is the sequence which is used by a 3rd-party to store licence information in an ETAS device which supports
  the LIC API, thereby turning the ETAS device into a dongle. It is expected that the 3rd-party will create a tool to
  perform this:
    -# The 3rd-party prepares a blob of licence data, which is private to the 3rd-party.
    -# The 3rd-party obtains a string containing device-specific identity information for the dongle device. On one device,
       this may be a MAC address; on another device this may be a serial number, etc. The value of this string for any
       given device can be obtained by calling the function LIC_GetDeviceId(). The value outputted by this function will
       be constant for any given device instance.
    -# To initialise an ETAS device as a dongle, the 3rd-party calls the function LIC_SetLicenceBlob(). This accepts:
        - the 3rd-party's licence blob;
        - a signature, calculated using a key which is private to the 3rd-party, over the licence blob and the
          device-specific identity string obtained via LIC_GetDeviceId(). The 3rd-party may omit this signature if he does
          not care about preventing an attacker from copying the licence blob from one device instance to another.

  This is the sequence which is followed at runtime by the 3rd-party's software tool to use the licence information
  contained in the ETAS dongle device:
    -# The 3rd-party software tool uses @ref CSI_GetOcdAuthInfo() to authenticate the ETAS device's OCD DLL. This allows the
       3rd-party software tool to trust any information which it subsequently obtains from the ETAS device.
    -# The 3rd-party software tool calls the BOA function LIC_GetLicenceBlob(). This function returns the information which
       was stored by LIC_SetLicenceBlob(), i.e.:
        - the licence blob;
        - a signature, calculated using a key which is private to the 3rd-party, over the licence blob and the
          device-specific identity string obtained via LIC_GetDeviceId().
    -# The 3rd-party software calls the BOA function LIC_GetDeviceId() to obtain the device's identity string.
    -# The 3rd-party software uses the 3rd-party's public key to check that the signature returned by LIC_GetLicenceBlob()
       is correct. This confirms that:
        - the licence blob has not been modified;
        - the licence blob has not been copied to a different device.
    -# Finally, the 3rd-party measurement tool enables or disables its various features according to the contents of the
       licence blob.

@{
*/

/**
 * Internal identifier for the LIC interface.
 */
#define UUID_LIC {0xA4,0x79,0xF2,0x0A,0x41,0x60,0x45,0xe7,0xBF,0xA9,0x08,0x09,0x0A,0xF2,0x59,0xDA}

/** The type used to refer to LIC API session handles. */
typedef BOA_Handle      LIC_SessionHandle;
/** The type returned by LIC API functions. */
typedef BOA_ResultCode  LIC_ErrorCode;

/**
  Create a new API session and bind it to a hardware controller identified by a URI.

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
             version of the LIC API which is required by the BOA framework. </td>
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

#ifndef LIC_DYNAMICDLL
LIC_DECLSPEC LIC_ErrorCode
LIC_CALL LIC_CreateSession(
    const char*         uriLocation,
    LIC_SessionHandle*  phSession );
#endif

typedef LIC_ErrorCode
(LIC_CALL *PF_LIC_CreateSession)(
    const char*         uriLocation,
    LIC_SessionHandle*  phSession );

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
             LIC API, or if the driver does not support the version of the LIC API requested by @p pProtocolStack.
             This can indicate a problem with the driver's support for the LIC API, or it may indicate that
             @p pProtocolStack does not refer to the LIC API at all. </td>
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

#ifndef LIC_DYNAMICDLL
LIC_DECLSPEC LIC_ErrorCode
LIC_CALL LIC_CreateSessionBind(
    const BOA_ProtocolStack*    pProtocolStack,
    LIC_SessionHandle*          phSession );
#endif

typedef LIC_ErrorCode
(LIC_CALL *PF_LIC_CreateSessionBind)(
    const BOA_ProtocolStack*    pProtocolStack,
    LIC_SessionHandle*          phSession );

/**
  Destroy an API session created by LIC_CreateSession() or LIC_CreateSessionBind().

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

#ifndef LIC_DYNAMICDLL
LIC_DECLSPEC LIC_ErrorCode
LIC_CALL LIC_DestroySession(
    LIC_SessionHandle hSession );
#endif

typedef LIC_ErrorCode
(LIC_CALL *PF_LIC_DestroySession)(
    LIC_SessionHandle hSession );

/**
 * This enum lists the IDs of the licence value which can be obtained via @ref LIC_GetLicenceValue().
 */
typedef enum LIC_LicenceValueId
{
    /** This licence value indicates whether or not the current session is permitted to be used by ETAS INCA.
     * The licence value is expected to be a string containing either "1" (i.e. yes) or "0" (i.e. no). */
    LIC_OPERATES_WITH_INCA = 1
} LIC_LicenceValueId;

/** The maximum length, including a NULL terminator, of a licence value obtained via @ref LIC_GetLicenceValue(). */
#define LIC_MAX_LICENCE_VALUE 128

/** The type of a licence value obtained via @ref LIC_GetLicenceValue(). */
typedef char LIC_LicenceValue[LIC_MAX_LICENCE_VALUE];

/**
  This function obtains a specified licence value from a specified LIC API session.

  The LIC API defines various "licence value IDs" in the enum @ref LIC_LicenceValueId. Each entry in this enum identifies
  a licence value, which can be requested from an implementation of the LIC API. The licence value contains licence
  information for that particular implementation of the LIC API.

  Note that, obviously, an implementation of the LIC API could lie in its response to @ref LIC_GetLicenceValue(). Therefore,
  before relying on the information obtained from @ref LIC_GetLicenceValue(), a BOA client should use @ref CSI_GetOcdAuthInfo()
  to confirm that the OCD DLL which implements the LIC API is legitimate.

  The use case which is supported by @ref LIC_GetLicenceValue() is discussed more fully in @ref GROUP_BOA_LIC.

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
        <td> The implementation of the LIC API which is associated with @p hSession could not be reached. Perhaps
             the associated hardware device has been disconnected. </td>
   </tr>
  </TABLE>
*/

#ifndef LIC_DYNAMICDLL
LIC_DECLSPEC LIC_ErrorCode
LIC_CALL LIC_GetLicenceValue(
    LIC_SessionHandle   hSession,
    LIC_LicenceValueId  licenceValueId,
    LIC_LicenceValue    licenceValueBuf );
#endif

typedef LIC_ErrorCode
(LIC_CALL *PF_LIC_GetLicenceValue)(
    LIC_SessionHandle   hSession,
    LIC_LicenceValueId  licenceValueId,
    LIC_LicenceValue    licenceValueBuf );

/** The maximum length, including a NULL terminator, of a device ID obtained via @ref LIC_GetDeviceId(). */
#define LIC_MAX_DEVICE_ID 128

/** The type of a device ID obtained via @ref LIC_GetDeviceId(). */
typedef char LIC_DeviceId[LIC_MAX_DEVICE_ID];

/**
  This function obtains a unqiue identifier for the device which is associated with a given LIC API session. The identifier
  is unique to this device instance, and constant over time. A good example is a MAC address or a serial number, but the
  actual form and meaning of the identifier is specific to the device.

  The caller of this function is expected to treat the device ID as opaque, and is strongly encouraged @em not to attempt
  to interpret the value of the device ID.

  The use case which is supported by @ref LIC_GetDeviceId() is discussed more fully in @ref GROUP_BOA_LIC.

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
        <td> The implementation of the LIC API which is associated with @p hSession could not be reached. Perhaps
             the associated hardware device has been disconnected. </td>
   </tr>
  </TABLE>
*/

#ifndef LIC_DYNAMICDLL
LIC_DECLSPEC LIC_ErrorCode
LIC_CALL LIC_GetDeviceId(
    LIC_SessionHandle   hSession,
    LIC_DeviceId        deviceIdBuf );
#endif

typedef LIC_ErrorCode
(LIC_CALL *PF_LIC_GetDeviceId)(
    LIC_SessionHandle   hSession,
    LIC_DeviceId        deviceIdBuf );

/** The maximum length, including a NULL terminator, of the name of a licence blob, as set by @ref LIC_SetLicenceBlob(). */
#define LIC_MAX_BLOBNAME_LEN 64

/** This type is used to name a licence blob, when the blob is being manipulated by LIC_SetLicenceBlob() and LIC_GetLicenceBlob(). */
typedef char LIC_LicenceBlobName[LIC_MAX_BLOBNAME_LEN];

/**
  This function is used to store a blob of licence information in the firmware of the device which is associated with a
  given LIC API session.

  The blob of licence information is opaque to the device; it is only meaningful to the caller. The caller can retrieve
  the blob via @ref LIC_GetLicenceBlob().

  If desired, the caller can secure the blob against tampering by supplying a signature calculated over the blob and the
  device's unique ID, as obtained from LIC_GetDeviceId(). Supplying such a signature will ensure that attempts to tamper
  with the blob, or copy it to a different device, will be detected.

  The use case which is supported by @ref LIC_SetLicenceBlob() is discussed more fully in @ref GROUP_BOA_LIC.

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
        <td> The implementation of the LIC API which is associated with @p hSession could not be reached. Perhaps
             the associated hardware device has been disconnected. </td>
   </tr>
   <tr> <td> @ref BOA_ERR_NOT_IMPLEMENTED </td>
        <td> The implementation of the LIC API which is associated with @p hSession does not implement this function.
             Probably the associated hardware device is not capable of storing a licence blob in its firmware. </td>
   </tr>
  </TABLE>
*/

#ifndef LIC_DYNAMICDLL
LIC_DECLSPEC LIC_ErrorCode
LIC_CALL LIC_SetLicenceBlob(
    LIC_SessionHandle   hSession,
    LIC_LicenceBlobName blobName,
    uint8*              pBlob,
    uint8*              pBlobSig,
    uint32              lenBlob,
    uint32              lenBlobSig,
    uint32*             pMaxLenBlob,
    uint32*             pMaxLenBlobSig );
#endif

typedef LIC_ErrorCode
(LIC_CALL *PF_LIC_SetLicenceBlob)(
    LIC_SessionHandle   hSession,
    LIC_LicenceBlobName blobName,
    uint8*              pBlob,
    uint8*              pBlobSig,
    uint32              lenBlob,
    uint32              lenBlobSig,
    uint32*             pMaxLenBlob,
    uint32*             pMaxLenBlobSig );

/**
  This function is used to retrieve a blob of licence information in the firmware of the device  which is associated with a
  given LIC API session. The blob will have been previously stored in the device's firmware via LIC_SetLicenceBlob().

  The use case which is supported by @ref LIC_SetLicenceBlob() is discussed more fully in @ref GROUP_BOA_LIC.

  @param[in]  hSession      Handle of an API session associated with the device whose licence blob is to be obtained.
  @param[in]  blobName      The name of the requested licence blob, as set by @ref LIC_SetLicenceBlob().
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
             not match any licence blob which has been stored on the device by @ref LIC_SetLicenceBlob(). </td>
   </tr>
   <tr> <td> @ref BOA_ERR_BUFFER_OVERFLOW </td>
        <td> @p lenBlobBuf is shorter than the length indicated by @p pLenBlob; or @p lenBlobSigBuf is shorter than the length
             indicated by @p pLenBlobSig </td>
   </tr>
   <tr> <td> @ref BOA_ERR_DRIVER_NO_RESPONSE </td>
        <td> The implementation of the LIC API which is associated with @p hSession could not be reached. Perhaps
             the associated hardware device has been disconnected. </td>
   </tr>
   <tr> <td> @ref BOA_ERR_NOT_IMPLEMENTED </td>
        <td> The implementation of the LIC API which is associated with @p hSession does not implement this function.
             Probably the associated hardware device is not capable of storing a licence blob in its firmware. </td>
   </tr>
  </TABLE>
*/

#ifndef LIC_DYNAMICDLL
LIC_DECLSPEC LIC_ErrorCode
LIC_CALL LIC_GetLicenceBlob(
    LIC_SessionHandle   hSession,
    LIC_LicenceBlobName blobName,
    uint8*              pBlobBuf,
    uint8*              pBlobSigBuf,
    uint32              lenBlobBuf,
    uint32              lenBlobSigBuf,
    uint32*             pLenBlob,
    uint32*             pLenBlobSig );
#endif

typedef LIC_ErrorCode
(LIC_CALL *PF_LIC_GetLicenceBlob)(
    LIC_SessionHandle   hSession,
    LIC_LicenceBlobName blobName,
    uint8*              pBlobBuf,
    uint8*              pBlobSigBuf,
    uint32              lenBlobBuf,
    uint32              lenBlobSigBuf,
    uint32*             pLenBlob,
    uint32*             pLenBlobSig );

/**
* Internal v-table for the LIC interface.
*/
typedef struct LIC_VTable
{
    PF_LIC_CreateSession     createSession;
    PF_LIC_CreateSessionBind createSessionBind;
    PF_LIC_DestroySession    destroySession;

    PF_LIC_GetLicenceValue   getLicenceValue;
    PF_LIC_GetDeviceId       getDeviceId;
    PF_LIC_SetLicenceBlob    setLicenceBlob;
    PF_LIC_GetLicenceBlob    getLicenceBlob;
} LIC_VTable;

/** @} */

#ifdef __cplusplus
}
#endif

#endif
