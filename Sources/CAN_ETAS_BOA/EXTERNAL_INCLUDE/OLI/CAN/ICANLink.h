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
* @brief  ICANLink definition
* @remark The header structure of the OLI may change
*         in future releases. Don't include this
*         header directly. Use @ref OLI.h instead.
*/

#if !defined(__OLI_ICANLINK_INCLUDED__)
#define __OLI_ICANLINK_INCLUDED__

/// @todo update doxygen comments

// include base interface

#include "../Common/ILink.h"
#include "../Common/AutoPtr.h"
#include "../Common/ErrorInterfaces.h"
#include "../Common/ErrorProxies.h"

// open ETAS::OLI namespace

#include "../Common/BeginNamespace.h"

#ifdef _DOXYGEN
namespace ETAS {namespace OLI {
#endif

/// \addtogroup GROUP_OLI_CAN_LINKS
/// @{

class ICANLink;

/**
 * This function instantiates an object supporting ICANLink. See \ref BinaryCompatibility for an explanation of why it is needed.
 *
 * NOTE that clients are encouraged to access this function via the wrapper ICANLink::Create().
 *
 * \param[in]  URI       The URI identifying the instance which is to be created.
 * \param[out] ppCanLink The new instance of CCANLink; the function addrefs this instance before handing it out. The object must
 *                       be reference-counted by the caller, using the object's methods AddRef() and Release(). This is easily
 *                       done by wrapping the object pointer in an instance of the AutoPtr class, which will be done automatically
 *                       if the caller accesses ICANLink_Create() via the wrapper ICANLink::Create().
 * \param[in]  version   The OLI version which the client expects to use.
 *
 * \return A pointer to an interface based on IError, describing the error which occurred during this function. NULL if no error
 * occurred. See \ref ErrorReporting for more information on how errors are reported.
 */
OLL_API IError* OLI_CALL ICANLink_Create( const char *URI, ICANLink** ppCanLink, uint32 version );

/** CAN-specific extension to ILink.
*/

OLI_INTERFACE ICANLink : public ILink
{
protected:

    /// Instances of this interface are owned by the OLL.

    virtual ~ICANLink() OLI_NOTHROW {};

    /**
     * This method creates a TX queue. Note that each CAN link can have at most one TX queue; therefore repeated calls
     * to this method will merely return a pointer to the original queue.
     *
     * The caller should access this method via the public wrapper ICANLink::CreateTxQueue().
     *
     * \param[out] ppTxQueue    A pointer to the one-and-only TX queue which is associated with this CAN link. The method AddRefs
     *                          this pointer before handing it out. The object must be reference-counted by the caller, using the
     *                          object's methods AddRef() and Release(). This is easily done by wrapping the object pointer in an
     *                          instance of the AutoPtr class, which will be done automatically if the caller accesses this method
     *                          via the wrapper ICANLink::CreateTxQueue().
     *
     * \return A pointer to an interface based on IError, describing the error which occurred during this function. NULL
     * if no error occurred. See \ref ErrorReporting for more information on how errors are reported.
     */
    virtual IError* CreateTxQueue( ITxQueue** ppTxQueue ) OLI_NOTHROW = 0;

public:

    /// \name CAN-specific interface
    /// @{

    /**
     * This is a helper method which wraps a corresponding protected method: see \ref BinaryCompatibility and \ref ErrorReporting
     * for an explanation of why it is needed.
     */
    AutoPtr<ITxQueue> OLI_CALL CreateTxQueue()
    {
        ITxQueue* pTxQueue = NULL;
        CheckAndThrow( CreateTxQueue( &pTxQueue ) );

        // The wrapped method has already AddRef-ed the pointer, so we tell AutoPtr to take ownership of the pointer without a
        // further AddRef.
        return AutoPtr<ITxQueue>( pTxQueue, false );
    };

    /// @}

    /**
     * This is a helper method which wraps ICANLink_Create(): see \ref BinaryCompatibility and \ref ErrorReporting for an
     * explanation of why it is needed.
     */
    static AutoPtr<ICANLink> OLI_CALL 
    Create( const char *URI, uint32 version = VERSION )
    {
        ICANLink* pCanLink = NULL;
        CheckAndThrow( ICANLink_Create( URI, &pCanLink, version ) );

        // The wrapped method has already AddRef-ed the pointer, so we tell AutoPtr to take ownership of the pointer without a
        // further AddRef.
        return AutoPtr<ICANLink>( pCanLink, false );
    }
};

// close ETAS::OLI namespace

#ifdef _DOXYGEN
}}
#endif

#include "../Common/EndNamespace.h"

#endif // !defined(__OLI_ICANLINK_INCLUDED__)
