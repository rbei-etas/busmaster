/*
* Basic Open API - Open Link Interface
* Version 1.3
*
* Copyright (c) ETAS GmbH. All rights reserved.
*
* $Revision: 4509 $
*/

/** 
* @file
* @brief  IAPIConnector definition
* @remark The header structure of the OLI may change
*         in future releases. Don't include this
*         header directly. Use @ref OLI.h instead.
*/

#if !defined(__OLI_IAPICONNECTOR_INCLUDED__)
#define __OLI_IAPICONNECTOR_INCLUDED__

#include "..\..\Common\BOAService.h"   

#include "IRefCountable.h"
#include "ErrorInterfaces.h"
#include "ErrorProxies.h"

#include "AutoPtr.h"

// open ETAS::OLI namespace

#include "BeginNamespace.h"

#ifdef _DOXYGEN
namespace ETAS {namespace OLI {
#endif

// interface forward declaration

class ILink;
class IAPIConnector;

/**
* @ingroup GROUP_OLI_INTERFACES_UTILITIES
* @brief  This function instantiates an object supporting 
*         @ref IAPIConnector. 
*
*         See @ref BinaryCompatibility "binary compatibility" 
*         for an explanation of why it is needed.
*
*         NOTE that clients are encouraged to access this function 
*         via the wrapper @ref IAPIConnector::Create().
*
* @param[in]  link   
*         The OLI link instance for which the extension API shall
*         be instantiated.
* @param[in]  usingAPI  
*         A unique identifier for the using (calling) API.
*         Per extension API and @a link instance, there may be at
*         most one API connector instance.
* @param[out] ppAPIConnector   
*         A pointer to an object supporting @ref IAPIConnector, 
*         which has already been AddRef-ed. The object must be 
*         reference-counted by the caller, using the object's
*         methods @ref IRefCountable::AddRef "AddRef()" and 
*         @ref IRefCountable::Release "Release()". This is easily 
*         done by wrapping the object pointer in an instance of the 
*         @ref AutoPtr class, which will be done automatically if 
*         the caller accesses @ref IAPIConnector_Create() 
*         via the wrapper @ref IAPIConnector::Create().
*
* @return A pointer to an interface based on @ref IError, describing 
*         the error which occurred during this function. @c NULL if 
*         no error occurred. See @ref ErrorReporting "error reporting"
*         for more information on how errors are reported.
*
* @exception <none> This function must not throw exceptions.
*
* @since  BOA 1.3 
* @see    @ref BinaryCompatibility "binary compatibility", 
*         @ref ErrorReporting "error reporting",
*         IAPIConnector
*/
OLL_API IError* OLI_CALL IAPIConnector_Create( 
    ILink* link,
    const BOA_UuidBin& usingAPI,
    IAPIConnector** ppAPIConnector);

/** @ingroup GROUP_OLI_INTERFACES_UTILITIES
* @brief  Managed access to an existing link's OCI-level information.
*
* The core OLI supports only a pre-defined set of bus-specific APIs.
* Additional functionality can be made available through hard- and
* firmware-specific extension APIs. They augment existing links to 
* the target with an additional set of functions.
*
* Prior to creating a instance of the extension API on the OLI level, 
* an instance on the next lower level, the OCI, must be established.
* To do that, OLL-internal information is required that can be 
* retrieved through this interace. It also keeps the underlying 
* OLI and OCI core connections valid for as long as they are used
* by the extension API.
*
* An API can be available only once per link. Therefore, for any 
* @ref ILink "link" - @ref BOA_UuidBin "API ID" combination, there
* can be at most one @a IAPIConnector instance. Any attempt to create
* more instances will result in exceptions being thrown.
*
* @remark All public methods are thread-safe.
* @coding This interface is intended for usage among "peer DLLs"
*         implementing extension APIs on the same abstraction level
*         as the OLI. Client applications should not instantiate
*         this interface.
* @since  BOA 1.3
* @see    ILink
*/

OLI_INTERFACE IAPIConnector : public IRefCountable
{
protected:

    /** @brief Destructor.

        This destructor has been hidden to force the client
        application to use proper reference counting.

        @exception <none> This function must not throw exceptions.

        @since  BOA 1.3 
     */
    virtual ~IAPIConnector() OLI_NOTHROW {};

public:

    /** @brief  Returns the link's OCI controller handle.

                The handle will remain valid as long as this interface
                instance is valid. However, the communication with the
                target may be interrupted at any time and subsequent API
                calls will result in corresponding error return values.

                Therefore, the respective @a link should be @ref 
                IQueueObserver "monitored"  for @ref IInternalErrorEvent 
                messages.

        @return The link's OCI controller handle that is required
                to instantiate the extension API on the OCI level.
        @exception <none> This function must not throw exceptions.

        @since  BOA 1.3
        @see    IInternalErrorEvent, IQueueObserver
     */
	virtual OCI_ControllerHandle OLI_CALL 
        GetControllerHandle() const OLI_NOTHROW = 0;

    /** @brief  Create an @ref IAPIConnector instance.

                This connection object is required to coordinate link 
                resource usage and lifetime between an instance of an
                extension API and the core OLL implementation. It should
                only be used by DLLs implementing such extension APIs
                on the same abstraction level as the OLI.

        @param[in]  link   
                The OLI link instance for which the extension API shall
                be instantiated.
        @param[in]  usingAPI  
                A unique identifier for the using (calling) API.
                Per extension API and @a link instance, there may be at
                most one API connector instance.
        @return New @ref IAPIConnector instance.
        @exception CError This function may throw an exception
                derived from @ref CError.

        @remark This is a helper method which wraps 
                @ref IAPIConnector_Create(): 
                see @ref BinaryCompatibility "binary compatibility" 
                and @ref ErrorReporting "error reporting"
                for an explanation of why it is needed.
        @since  BOA 1.3 
     */
    static AutoPtr<IAPIConnector> OLI_CALL Create(
        ILink* link, 
        const BOA_UuidBin& usingAPI)
    {
        IAPIConnector* pAPIConnector = NULL;
        CheckAndThrow( IAPIConnector_Create( link, 
                                             usingAPI, 
                                             &pAPIConnector ) );

        // The wrapped method has already AddRef-ed the pointer, 
        // so we tell AutoPtr to take ownership of the pointer 
        // without a further AddRef.
        return AutoPtr<IAPIConnector>( pAPIConnector, false );
    }
};

// close ETAS::OLI namespace

#ifdef _DOXYGEN
}}
#endif

#include "EndNamespace.h"

#endif // !defined(__OLI_IAPICONNECTOR_INCLUDED__)
