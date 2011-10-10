#ifndef ETAS_BOA_SERVICE_H
#define ETAS_BOA_SERVICE_H

/**
* @file       boaservice.h
* @brief      Public declaration of service data types;
*             part of the Basic OpenAPI (BOA).
* @copyright  Copyright (c) 2008 ETAS GmbH. All rights reserved.
*
* $Revision: 4675 $
*/

#include "stdtypes.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
@page VersionHandling Version Handling

@section VersionInformation Version Information

Any version consists of four independent parts:

@li   majorVersion. 
- The major Version identifies the feature set of an Interface. This version is incremented
when some incompatible changes in the @ref BOA_ServiceId are made. When a server offers a higher
major version than the client can access, an error is generated. A server may offer several
different major versions at the same time.

@li   minorVersion.
- The minor Version identifies compatible extensions of an Interface. When a server offers additional
features, but can still serve an older client, this number is increased. 

@li   bugfix.
- The bugfix identifies corrections made in the implementation of the interface. The bugfix is
implementation specific and therefore it may have different values for different implementations of
the same feature set. The bugfix must not be used for any version check, but can only be used for
informational and debugging purposes on a specific hardware.

@li   build.
- The build revision allows the verification of a specific version during the development phase.

@section VersionNegotiation Version negotiation

During the binding of a protocol a negotiation of the versions of a service are done. The client requests a
version and the server delivers a version. In general the negotiation is done in three steps:

-# The client selects the major and minor version it wants to use. This version is transmitted as requested 
version to the server.
-# The server receives the request and offers a proposed version. The major version of the proposed version
must be identical to the the major version of the requested version. When the server does not offer
the requested major version it generates an error. The minor version may be not equal to the requested 
version. A higher minor version than the requested version identifies an server with additional feature.
The client can ignore this information. A lower version than the requested version identifies an older
interface. The client may not be able to use any service that is offered in the requested version. 
-# The client verifies the offered version. When the offered minor version is lower than the requested minor
version the client may adapt itself to the limited feature set of the older interface and continue operation 
or may stop the binding process and exit with an error.
*/

#define BOA_MAX_PARAM 128

#define UUID_DLL_BIND {0x25,0xB9,0xD4,0x52,0x3A,0xCA,0x4c,0x89,0xA0,0x50,0x7E,0x9F,0x79,0xAD,0x34,0x21}

/* {5DB45414-7D9D-4aa0-AF11-2A741EF978F6} */
#define UUID_TCP_BIND {0x5d,0xb4,0x54,0x14,0x7d,0x9d,0x4a,0xa0,0xaf,0x11,0x2A,0x74,0x1E,0xF9,0x78,0xF6}

// {8B5B3678-2448-417f-8625-55B3C1C3B4D1} - serialized data stream
#define UUID_SERIALIZED_DATA_BIND { 0x8b, 0x5b, 0x36, 0x78, 0x24, 0x48, 0x41, 0x7f, 0x86, 0x25, 0x55, 0xb3, 0xc1, 0xc3, 0xb4, 0xd1 }

#define UUID_NULL {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}

#if defined (_WIN32)
#define ALIGN( type, arg ) __declspec(align(arg)) type
#elif defined(__GNUC__)
#define ALIGN( type, arg ) type __attribute__ ((aligned (arg)))
#endif

/** Binary representation of a UUID. We align it on a dword boundary for efficient copying and comparison. */
ALIGN( typedef struct BOA_UuidBin
{
   /** binary data representation of the UUID. */
   uint8  data[16];
} BOA_UuidBin, 4 );

/** Representation of a service API or access method version. We align it on a dword boundary for efficient copying and comparison. */
ALIGN( typedef struct BOA_Version
{
   /** major version of the interface identified by this structure. See Also: @ref VersionHandling */
   uint8 majorVersion;
   /** minor version of the interface identified by this structure. @sa VersionHandling */
   uint8 minorVersion;
   /** bugfix of the interface identified by this structure. See Also: @ref VersionHandling */
   uint8 bugfix;
    /** build of the interface identified by this structure. See Also: @ref VersionHandling */
   uint8 build;
} BOA_Version, 4 );

/** representation of a UUID and Version information. */
typedef struct BOA_UuidVersion
{
   /** The UUID */
   BOA_UuidBin uuid;
   /** The associated version */
   BOA_Version version;
} BOA_UuidVersion;

/** Service Identification based on the functional api and access protocol. */
typedef struct BOA_ServiceId
{
   /**
       Functional API of the service. This field defines the functions,
       their signatures, pre- and postconditions.
   */
   BOA_UuidVersion api;
   /** Access protocol supported by the API. */
   BOA_UuidVersion access;
} BOA_ServiceId;

/** Service Identification with required parameter. */
typedef struct BOA_ServiceIdParam
{
   /** Service Identification based on the functional API and access protocol. */
   BOA_ServiceId id;
   /** number of the valid elements in accessParam[]. */
   uint32 count;
   /**
    Access protocol specific parameter for a correct setup of a connection to 
    this service (IP-Address and port, DLL name, etc.).
   */
   char accessParam[BOA_MAX_PARAM];
} BOA_ServiceIdParam;

/** tree representation of API access parameters */
typedef struct BOA_ProtocolStack
{
   /** information about the item */
   BOA_ServiceIdParam server;
   /**
      Linked list of all items in this hierarchy layer;
      Every entry represents one of the required services on the parent level.
   */
   struct BOA_ProtocolStack *sibling;
   /**
      References the first child of the node. Additional children
      may be available as child->sibling etc.
   */
   struct BOA_ProtocolStack *child;
} BOA_ProtocolStack;

bool BOA_IsServiceIdSufficient(const BOA_ServiceId *required, const BOA_ServiceId *offered);

#if 0
/** 
* @} 
*/
#endif

#ifdef __cplusplus
}
#endif

#endif
