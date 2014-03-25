#ifndef ETAS_BOA_H
#define ETAS_BOA_H

/**
* @file       boa.h
* @brief      Doxygen-Documentation of Basic OpenAPI.
* @copyright  Copyright (c) 2008 ETAS GmbH. All rights reserved.
*
* $Revision: 7109 $
*
*
* @mainpage Basic OpenAPI Overview
* <HR>
* @htmlinclude "page_boa_overview.html"
*
* @page PAGE_BOA_ERROR_MANAGEMENT BOA - Error Management
* @htmlinclude "page_boa_error_management.html"
*
* @page PAGE_BOA_TIMER_MANAGEMENT BOA - Timer Management
* @htmlinclude "page_boa_time_management.html"
*
* @page PAGE_BOA_EXTENSIONS_CAN BOA - CAN Specifics
* @htmlinclude "page_boa_extensions_CAN.html"
*
* @page PAGE_BOA_EXTENSIONS_LIN BOA - LIN Specifics
* @htmlinclude "page_boa_extensions_LIN.html"
*
* @page PAGE_BOA_EXTENSIONS_FLEXRAY BOA - FlexRay Specifics
* @htmlinclude "page_boa_extensions_FlexRay.html"
*
* @page PAGE_BOA_MULTITHREADING BOA - Multi-Threading
* @htmlinclude "page_boa_multithreading.html"
*
* @addtogroup GROUP_BOA_COMMON Common (BOA)
* @{
*
* @defgroup GROUP_BOA_GLOBAL_TYPES Global Types
* @ingroup GROUP_BOA_COMMON
*
* @defgroup GROUP_BOA_ERROR_MANAGEMENT Error Management
* @ingroup GROUP_BOA_COMMON
*
* @}
*
*
* @addtogroup GROUP_BOA_OCI Open Controller Interface (OCI)
* @brief    The Open Controller Interface (OCI) offers an access to
*           the Automotive Bus Systems (CAN, LIN, FlexRay) via ETAS
*           systems.\ It is a low level, bus specific interface, that
*           allows an access to the bus systems on a frame based
*           level.
* @{
*
* @defgroup GROUP_OCI_COMMON OCI Common
* @ingroup GROUP_BOA_OCI
*
* @defgroup GROUP_OCI_CAN_CONTROLLER OCI CAN
* @ingroup GROUP_BOA_OCI
*
* @defgroup GROUP_OCI_LIN_CONTROLLER OCI LIN
* @ingroup GROUP_BOA_OCI
*
* @defgroup GROUP_OCI_FLEXRAY_CONTROLLER OCI FlexRay
* @ingroup GROUP_BOA_OCI
*
* @}
*
*
*
*
*
* @addtogroup GROUP_BOA_CSI Connection Service Interface (CSI)
* @brief    The Connection Service Interface (CSI) implements a framework for the different
            Basic OpenAPI components.

*
* @{
*
* @defgroup GROUP_CSI_COMMON Enumerate Sub Service (ESS)
* @ingroup GROUP_BOA_CSI
*
* @defgroup GROUP_CSI_COMPONENT Enumerate Component Service (ECS)
* @ingroup GROUP_BOA_CSI
*
* @defgroup GROUP_CSI_SFS Search For Service (SFS)
* @ingroup GROUP_BOA_CSI
*
* @defgroup GROUP_CSI_CTS Connect To Service (CTS)
* @ingroup GROUP_BOA_CSI
*
* @}
*
*/

/**
@page BackwardsCompatibilityPage Backwards compatibility and API versioning
This page discusses BOA's approach to backwards compatibility of OCI APIs.

@section ApiVersioning API versioning

As the OCI APIs evolve, new functionality may be added to an API, or existing functionality may be modified and expanded.
To allow changes to an API to be identified, each OCI API has an associated version number. The version of the original
API is defined as 1.0.0.0, and the version number is incremented as new BOA releases modify the definition of the API.

The documentation for each OCI API notes the changes which have occurred in each version. So, for example, the
documentation for the structure field @ref OCI_LINControllerCapabilities::slaveBaudrateType notes that this field was
introduced in version 1.1 of the OCI_LIN API.

(The version number of an API contains 4 digits, but currently only the first two are used meaningfully).

@section BackwardsCompatibility Backwards compatibility

When a OCI API is modified and a new version is created, it is necessary that the following use cases are correctly
supported:
- An OCD which implements the new API definition may be used by a client which was built to use the old API definition.
  Such a client must still get the behaviour which he expects from the OCD.
- A client which was built to use the new API definition may be used with an OCD which implements the old API definition.
  The client must still be able to operate, though obviously without using any features which were introduced in the new
  API definition.

The BOA framework supports both these use cases via a common mechanism. When a OCI client creates a new session of a
OCI API for a particular URI, the OCI client and the referenced OCD can each discover which versions of that OCI API
are supported by the other.

First, consider how a OCI client can discover the latest API version which is supported by the OCD associated with his
chosen URI (using the example of OCI_CAN):
-# The OCI client begins by choosing the latest version of OIC_CAN of which he is aware, e.g. v1.3. He uses
   @ref OCI_CreateCANControllerVersion() or @ref OCI_CreateCANControllerNoSearch() to try to create a OCI_CAN session
   which uses this API version for his chosen URI.
-# If @ref OCI_CreateCANControllerVersion() or @ref OCI_CreateCANControllerNoSearch() fails with the error
   @ref BOA_ERR_PROTOCOL_VERSION_NOT_SUPPORTED the OCI client concludes that the OCD associated with his chosen URI does
   not support the latest version of OCI_CAN. The OCI client then repeatedly calls @ref OCI_CreateCANControllerVersion()
   or @ref OCI_CreateCANControllerNoSearch() for older and older versions of OCI_CAN, until he finds a version
   of OCI_CAN which succeeds. This is the latest version of OCI_CAN which is supported by the OCD associated with the
   client's chosen URI.

Now consider the OCD's point of view: the OCD must discover the API version which is being requested by the OCI client
so that the OCD can adapt his behaviour to that expected by the client. An OCD can discover the API version requested
by a OCI client via the parameter <tt>protocolStack</tt> which is passed to the OCD's implementation of
@ref OCI_CreateCANControllerBind() (again using OCI_CAN as an example). The field <tt>protocolStack->server.id.api.version</tt>
contains the required information.
Once the OCD has discovered the API version requested by the client, the OCD can behave accordingly, either offering
new behaviour if the client requested a newer API version, or offering old behaviour in order to maintain backwards
compatibility with an older client.

There now follow some examples, using the new functionality introduced in v1.1 of OCI_LIN. In this version of OCI_LIN,
the field @ref OCI_LINControllerCapabilities::slaveBaudrateType was added, and zero became a permitted value for the
field @ref OCI_LINConfiguration::baudrate.

@section Example1 Example: client built with OCI_LIN v1.1, using an OCD built with OCI_LIN v1.1

The client begins by using @ref OCI_CreateLINControllerVersion() to create a session of OCI_LIN v1.1. This is
successful, and so the client knows that the OCD will provide a valid value in the field
@ref OCI_LINControllerCapabilities::slaveBaudrateType when he calls @ref OCI_GetLINControllerCapabilities(), and that
he may be able to use zero as a value for @ref OCI_LINConfiguration::baudrate.

The OCD knows that the client requested OCI_LIN v1.1, so the OCD knows that it can safely populate the field
@ref OCI_LINControllerCapabilities::slaveBaudrateType when the client calls @ref OCI_GetLINControllerCapabilities(),
and that in some circumstances the client may use zero as a value for @ref OCI_LINConfiguration::baudrate.

@section Example2 Example: client built with OCI_LIN v1.0, using an OCD built with OCI_LIN v1.1

The client begins by using @ref OCI_CreateLINControllerVersion() to create a session of OCI_LIN v1.0, because the
client knows no newer API version. This is successful.

The client is unaware of the new field @ref OCI_LINControllerCapabilities::slaveBaudrateType and will not attempt to
access it when he calls @ref OCI_GetLINControllerCapabilities(). Furthermore, the client will never use zero as a value
for @ref OCI_LINConfiguration::baudrate.

The OCD knows that the client requested OCI_LIN v1.0, so the OCD knows that it should not populate the field
@ref OCI_LINControllerCapabilities::slaveBaudrateType when the client calls @ref OCI_GetLINControllerCapabilities()
(indeed, this field may lie beyond the end of the structure instance provided by the client), and that the client will
never use zero as a value for @ref OCI_LINConfiguration::baudrate.

@section Example3 Example: client built with OCI_LIN v1.1, using an OCD built with OCI_LIN v1.0

The client begins by trying to use @ref OCI_CreateLINControllerVersion() to create a session of OCI_LIN v1.1. However,
this is unsuccessful, so he tries again with OCI_LIN v1.0. This works.

The client knows that he must not expect the OCD to output a valid value in the field
@ref OCI_LINControllerCapabilities::slaveBaudrateType when he calls @ref OCI_GetLINControllerCapabilities(), since the
OCD knows nothing of this field. Furthermore, the client knows that he must never use the value zero for the field
@ref OCI_LINConfiguration::baudrate, since the OCD will treat this as an error.

The OCD is unaware of the new field @ref OCI_LINControllerCapabilities::slaveBaudrateType and will not attempt to populate
it when the client calls @ref OCI_GetLINControllerCapabilities(). Furthermore, the OCD will never expect the value zero
to be used for @ref OCI_LINConfiguration::baudrate.

@section Conclusion

By enabling the OCI client and the OCD to reach a common understanding of each other's abilities at runtime, the above
mechanism enables future versions of BOA to make complex syntactic and semantic changes to the OCI APIs while still
allowing clients and OCDs to maintain backwards-compatibility with older versions.

However, in order to keep the implementation of clients and OCDs simple, future versions of BOA will restrict the kinds
of changes which may be made to OCI APIs:
- Extra functions may be added to the end of an OCI API's vtable.
- Extra fields may be added to the end of an existing OCI structure.
- Existing fields of an OCI structure may have their semantics modified (e.g. the range of permitted values may be extended
  or reduced).

In all cases:
- a client should not use the new functionality unless he has successfully requested an API version which supports it;
- an OCD should not implement the new functionality unless the client requested an API version which supports it.

*/

#endif

