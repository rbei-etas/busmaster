#ifndef ETAS_BOA_SERVICE_H
#define ETAS_BOA_SERVICE_H

/**
* @file       boaservice.h
* @brief      Public declaration of service data types;
*             part of the Basic OpenAPI (BOA).
* @copyright  Copyright (c) 2008 ETAS GmbH. All rights reserved.
*
* $Revision: 5976 $
*/

#include "stdtypes.h"

#ifdef __cplusplus
extern "C" {
#endif


#define BOA_MAX_PARAM 128

#define UUID_DLL_BIND {0x25,0xB9,0xD4,0x52,0x3A,0xCA,0x4c,0x89,0xA0,0x50,0x7E,0x9F,0x79,0xAD,0x34,0x21}

/* {5DB45414-7D9D-4aa0-AF11-2A741EF978F6} */
#define UUID_TCP_BIND {0x5d,0xb4,0x54,0x14,0x7d,0x9d,0x4a,0xa0,0xaf,0x11,0x2A,0x74,0x1E,0xF9,0x78,0xF6}

// {8B5B3678-2448-417f-8625-55B3C1C3B4D1} - serialized data stream
#define UUID_SERIALIZED_DATA_BIND { 0x8b, 0x5b, 0x36, 0x78, 0x24, 0x48, 0x41, 0x7f, 0x86, 0x25, 0x55, 0xb3, 0xc1, 0xc3, 0xb4, 0xd1 }

#define UUID_NULL {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}

#if defined(MIDL_RUNNING)
#define ALIGN( type, arg ) type
#elif defined (_WIN32)
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
