#ifndef _EXPORTAPIS_H_
#define _EXPORTAPIS_H_
/**
    @file

    central header file defining helper functions that are implemented in the lib-boa and structures
    that are used by the helper functions.
*/

/**
    @addtogroup GROUP_LIB_BOA lib-boa

    static library that offers several support functions for using the BOA. All functions in this library
    are not related to serialization or de-serialization of data. The serialization and deserialization
    functions can be found in the lib-serDesBoa.

    The functions in this library are grouped around the following areas:
    - A handleMap that allows mapping of handles and assigning a context to a handle (@see handleMap.c)
    - Some basic functions for handling and parsing uuids (@see uriParse.c)
    - Support functions and structures to export CSI-ESS and CSI-ECS interfaces to the framework (@see exportAPIs.c)
    - Handling of UUIDs and generation of UUIDs for generation of some affinity Data required by the export interface (@see boaUuid.c)
*/

#include "Common/boaservice.h"
#include "CSI/csisfs.h"
#include "OCI/oci.h"

#ifdef __cplusplus
extern "C" {
#endif

    /** declartion of the signature for functions to generate a UUID */
    typedef BOA_UuidBin* (*generateUuid)(BOA_UuidBin* uuid);

    /** Library function that generates an UUID for the process and system the caller is running in.
        This call can stored in the struct CSI_Affinity to generate a server Affinity and a required Affinity. */
    BOA_UuidBin* GetCurrentSystemAndProcessUuid(BOA_UuidBin* uuid);

    /** Library function that generates an UUID for the system the caller is running in.
        This call can stored in the struct CSI_Affinity to generate a server Affinity and a required Affinity.
        The framework uses the MAC address of the first NIC for the UUID. Plug & Play events may cause a
        change during runtime. This may result in problems during the binding process. */
    BOA_UuidBin* GetCurrentSystemUuid(BOA_UuidBin* uuid);

    /** Library function that generates an UUID for the system the caller is running in.
        This call can stored in the struct CSI_Affinity to generate a server Affinity and a required Affinity
        and whenever a NIL uuid is required. */
    BOA_UuidBin* GetNilUuid(BOA_UuidBin* uuid);

    /** Library function that generates an UUID for a specific network. The OS-Wrapper-Lib allows a detection of
        all networks (local IP-Addresses and netMasks) of a system (for Windows and QNX) see getIpV4Info() of the
        lib-os-wrapper. */
    BOA_UuidBin* GetLanUuid(BOA_UuidBin* uuid, uint32 ipAddress, uint32 netMask);



    /** declaration of the signature for functions that set the accessParameter */
    typedef void (*CSI_SetAccessParam)(CSI_SubItem* item);


    /** Request the netMask for a specific local ipAddress from the OS. */
    uint32 GetNetMaskForIpV4(uint32 ipAddress);

    /** structure used to store references to the BOA_UuidBin generator functions to allow a static configuration
        of a DLL that offers an interface with information that only will be available at runtime. */
    typedef struct CSI_Affinity
    {
        /* routine that sets the access Parameter - the framework requires the DLL-name to be able to use the
            CSI_GetVtable() call of the dll-csiBind. The information may be extended by some driver specific
            information. They must be separated by an "/" from the string.
            New: The framwork supports the macro "$/". When the accessParameter starts with $/ the $ will be
            replaced with the DLL name by the framework. That means the DLL can store the "$/" statically in
            the structure without the need of any dynamic modification.*/
        //CSI_SetAccessParam  setAccessParam;

        /** identifies the affinity of the API offered by the implementation. Normal DLLs will use
            GetCurrentSystemAndProcessUuid() for this entry to identify that the functional API can be
            accessed inside of the same process as the CSI-Service that calls this function during the
            CSI_ESS execution. */
        generateUuid serverAffinity;
        /** identifies the required affinity of a required API. When the functional API needs no required API
            it should use GetNilUuid(). When the required API will be accessed via the DLL loading mechanism
            it must use GetCurrentSystemAndProcessUuid(). */
        generateUuid requiredAffinity;
    } CSI_Affinity;

    /** structure for identifying the offered API of an implementation that is returned when the
        CSI-SFS builds the tree view for the application.*/
    typedef struct CSI_ESS_ExportTable
    {
        const BOA_UuidVersion* id;          /**< uuid and version of the exported API */
        const void*               vtable;         /**< reference to the VTable with all function calls of the API */
        uint32                    vtableSize;     /**< size of the VTable in Bytes */
    } CSI_ESS_ExportTable;

    /** structure for identifying the offered API of an implementation that is returned when the
        CSI-SFS builds the tree view for the application. This Ex version of the struct allows the offered APIs to be
        identified by uuid, version *and* accessParam; see FindExportVTableEx(). */
    typedef struct CSI_ESS_ExportTableEx
    {
        const BOA_ServiceIdParam* serviceIdParam; /**< uuid, version and access param of the exported API */
        const void*               vtable;         /**< reference to the VTable with all function calls of the API */
        uint32                    vtableSize;     /**< size of the VTable in Bytes */
    } CSI_ESS_ExportTableEx;

    typedef struct CSI_ESS_Interface
    {
        uint32 tableSize;
        const CSI_ESS_ExportTable* table;
    } CSI_ESS_Interface;

    typedef struct CSI_ESS_InterfaceEx
    {
        uint32 tableSize;
        const CSI_ESS_ExportTableEx* table;
    } CSI_ESS_InterfaceEx;

    typedef struct CSI_ECS_Interface
    {
        uint32             numberOfItems;
        const CSI_SubItem* item;
        const CSI_Affinity* affinity;
    } CSI_ECS_Interface;

    int BOA_UuidsEqual       (const BOA_UuidBin*     lhs, const BOA_UuidBin*     rhs);
    int BOA_VersionsEqual    (const BOA_Version*     lhs, const BOA_Version*     rhs);
    int BOA_UuidVersionsEqual(const BOA_UuidVersion* lhs, const BOA_UuidVersion* rhs);

    BOA_UuidVersion* BOA_UuidVersionCpy( BOA_UuidVersion* dest, const BOA_UuidVersion* src );
    BOA_UuidBin*     BOA_UuidCpy       ( BOA_UuidBin*     dest, const BOA_UuidBin*     src );
    BOA_Version*     BOA_VersionCpy    ( BOA_Version*     dest, const BOA_Version*     src );

    int BOA_UuidVersionIsRequested(const BOA_UuidVersion* offered, const BOA_UuidVersion* required);
    int BOA_UuidVersionIsSufficient(const BOA_UuidVersion* offered, const BOA_UuidVersion* required);

#define BOA_UUID_PRINTF_FORMAT "{%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x}"
#define BOA_UUID_PRINTF_ARGS( pUuid ) \
    (pUuid)->data[0],  \
    (pUuid)->data[1],  \
    (pUuid)->data[2],  \
    (pUuid)->data[3],  \
    (pUuid)->data[4],  \
    (pUuid)->data[5],  \
    (pUuid)->data[6],  \
    (pUuid)->data[7],  \
    (pUuid)->data[8],  \
    (pUuid)->data[9],  \
    (pUuid)->data[10], \
    (pUuid)->data[11], \
    (pUuid)->data[12], \
    (pUuid)->data[13], \
    (pUuid)->data[14], \
    (pUuid)->data[15]

#define BOA_UUID_VERSION_PRINTF_FORMAT BOA_UUID_PRINTF_FORMAT " v%u.%u.%u.%u"
#define BOA_UUID_VERSION_PRINTF_ARGS( pUuidVersion )    \
    BOA_UUID_PRINTF_ARGS( &(pUuidVersion)->uuid ),      \
    (pUuidVersion)->version.majorVersion,               \
    (pUuidVersion)->version.minorVersion,               \
    (pUuidVersion)->version.bugfix,                     \
    (pUuidVersion)->version.build

    OCI_ErrorCode FindExportVTable (BOA_UuidVersion* interfaceID, void (**vtable)(), uint32 size,
                                    const CSI_ESS_Interface* exports );

    OCI_ErrorCode FindExportVTableEx (BOA_ServiceIdParam* interfaceID, void (**vtable)(), uint32 size,
                                      const CSI_ESS_InterfaceEx* exports );

    OCI_ErrorCode CSI_FindComponent(BOA_UuidBin* parentNodeId, CSI_SubItem result[], uint32 size,
                                    uint32* count, const CSI_ECS_Interface* table);

    /*
     * This set of macros is used to define an array of CSI_SubItem instances. The macros are used like this:
     *  BEGIN_SUBITEM_ARRAY(...)
     *  SUBITEM_ARRAY_ENTRY(...)
     *  SUBITEM_ARRAY_ENTRY(...)
     *  ...
     *  END_SUBITEM_ARRAY()
     */
#define BEGIN_SUBITEM_ARRAY( arrayName ) const CSI_SubItem arrayName[] = {
#define SUBITEM_ARRAY_ENTRY(                                                                                            \
        offeredApi, offeredMajorVer, offeredMinorVer, offeredBinding, offeredBindingMajorVer, offeredBindingMinorVer,       \
        nodeType,                                                                                                           \
        uriName,                                                                                                            \
        cost, optionFlags,                                                                                                  \
        requiredApi, requiredMajorVer, requiredMinorVer, requiredBinding, requiredBindingMajorVer, requiredBindingMinorVer )\
{                                                                                                                    \
    {   /* The offered API */                                                                                       \
        {{{offeredApi},{offeredMajorVer,offeredMinorVer,0,0}},{{offeredBinding},{offeredBindingMajorVer,offeredBindingMinorVer,0,0}}},\
        /* The access param: we use the URI name, preceded with "$/" (the '$' will be replaced with the DLL name at runtime). */      \
        sizeof("$/" uriName), "$/" uriName,                                                                         \
    },                                                                                                              \
    nodeType,                                                                                                       \
    uriName,                                                                                                        \
    /* Reserved fields, and structure version. */                                                                   \
    "",{1,1,0,0},"",                                                                                                \
    {UUID_NULL},                                                                                                    \
    {UUID_NULL},                                                                                                    \
    /** Quality of service / cost declaration */                                                                    \
    {cost,optionFlags,0,0,0,0,0,0},                                                                                 \
    /* Number of valid elements in the requiredAPI array. */                                                        \
    1,                                                                                                              \
    /* Array of server APIs required on the lower layer to offer the service. */                                    \
    {                                                                                                               \
    {{{requiredApi},{requiredMajorVer,requiredMinorVer,0,0}},{{requiredBinding},{requiredBindingMajorVer,requiredBindingMinorVer,0,0}}},\
    {{{UUID_NULL},{0,0,0,0}},{{UUID_NULL},{0,0,0,0}}},                                                          \
    {{{UUID_NULL},{0,0,0,0}},{{UUID_NULL},{0,0,0,0}}},                                                          \
    {{{UUID_NULL},{0,0,0,0}},{{UUID_NULL},{0,0,0,0}}}                                                           \
    }                                                                                                               \
},
#define END_SUBITEM_ARRAY() };

    /*
     * The following flags can be ORed together and used as a value for the optionFlags argument to SUBITEM_ARRAY_ENTRY()
     */

    /* This flag indicates that a node is a "preferred node".
     * A "preferred" node has these properties:
     *  - The cost of a preferred node is not added to the cost of the route, i.e. adding a preferred node to a route
     *    does not increase the cost of that route.
     *  - If two routes have equal cost, the route with the most preferred nodes will be chosen.
     *  - If two routes have equal cost and an equal number of preferred nodes, the total cost of the preferred nodes is
     *    calculated (recall that this cost is usually omitted from the cost of the route), and the route with the lowest
     *    such cost is chosen. */
#define CSI_PREFERRED_NODE          0X00000001

    /* This flag indicates that, when this node is added to the bottom of a protocol stack, it must *exactly* match the
     * API version required by the bottom of the protocol stack. (Usually, when a node is added to a protocol stack, the
     * node is permitted to offer an equal-or-newer version of the required API). */
#define CSI_MATCH_VERSION_EXACTLY   0x00000002

#ifdef __cplusplus
}
#endif


#endif // _EXPORTAPIS_H_
