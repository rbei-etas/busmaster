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
* @brief  IFlexRayLink definition
* @remark The header structure of the OLI may change
*         in future releases. Don't include this
*         header directly. Use @ref OLI.h instead.
*/

/// @todo update doxygen comments

#if !defined(__OLI_IFLEXRAYLINK_INCLUDED__)
#define __OLI_IFLEXRAYLINK_INCLUDED__

// include used interface

#include "../Common/ILink.h"
#include "../Common/AutoPtr.h"
#include "../Common/ErrorInterfaces.h"
#include "../Common/ErrorProxies.h"

// open ETAS::OLI namespace

#include "../Common/BeginNamespace.h"

#ifdef _DOXYGEN
namespace ETAS {namespace OLI {
#endif

/// \addtogroup GROUP_OLI_FLEXRAY_LINKS

/// @{

class IFlexRayLink;

/**
 * This function instantiates an object supporting IFlexRayLink. See \ref BinaryCompatibility for an explanation of why it is needed.
 *
 * NOTE that clients are encouraged to access this function via the wrapper IFlexRayLink::Create().
 *
 * \param[in]  URI              The URI identifying the instance which is to be created.
 * \param[in]  version          The OLI version which the client expects to use.
 * \param[out] ppFlexRayLink    A pointer to an object supporting IFlexRayLink, which has been AddRef-ed already. The object must
 *                              be reference-counted by the caller, using the object's methods AddRef() and Release(). This is
 *                              easily done by wrapping the object pointer in an instance of the AutoPtr class, which will be done
 *                              automatically if the caller accesses IFlexRayLink_Create() via the wrapper IFlexRayLink::Create().
 *
 * \return A pointer to an interface based on IError, describing the error which occurred during this function. NULL if no error
 * occurred. See \ref ErrorReporting for more information on how errors are reported.
 */
OLL_API IError* OLI_CALL IFlexRayLink_Create( const char *URI, uint32 version, IFlexRayLink** ppFlexRayLink );

/** FlexRay-specific extension of ILink.
*/

OLI_INTERFACE IFlexRayLink : public ILink
{
protected:

    /// Instances of this interface are owned by the OLL.

    virtual ~IFlexRayLink() OLI_NOTHROW {};

    /**
     * This method creates a TX queue. There can be multiple tx queues on a single CFlexRayLink, however buffer IDs must not overlap.
     *
     * The caller should access this method via the public wrapper IFlexRayLink::CreateTxQueue().
     *
     * \param[in]  bufferIndexes    Array of buffer configurations indexes to use for this queue
     * \param[in]  count            Number of entries in \a buffers
     * \param[out] ppTxQueue        A pointer to a new TX queue which is associated with this FlexRay link. The object will
     *                              have a reference count of 1 and must be reference-counted by the caller, using the object's
     *                              methods AddRef() and Release(). This is easily done by wrapping the object pointer in an
     *                              instance of the AutoPtr class, which will be done automatically if the caller accesses this
     *                              method via the wrapper IFlexRayLink::CreateTxQueue().
     *
     * \return A pointer to an interface based on IError, describing the error which occurred during this function. NULL
     * if no error occurred. See \ref ErrorReporting for more information on how errors are reported. A pointer to IOCIError
     * will be returned if the queue could not be allocated (for example because some buffers are already in use).
     */
    virtual IError* OLI_CALL CreateTxQueue( const uint32 bufferIndexes[], uint32 count, ITxQueue** ppTxQueue ) OLI_NOTHROW = 0;

    /**
     * This method gets the index of a given valid transmit buffer configuration of the original controller configuration.
     *
     * \param[in]  channel          Transmission channel.
     * \param[in]  slot             Transmission slot. Valid values are 1 .. 2047.
     * \param[in]  cyclePeriod      Transmission period. Valid values are 1, 2, 4, 8, 16, 32, 64.
     * \param[in]  cycleOffset      Starting offset for period. Valid values are 0 .. \a cyclePeriod - 1.
     * \param[out] pTxBufferIndex   The transmit buffer configuration index.
     *
     * \return A pointer to an interface based on IError, describing the error which occurred during this function. NULL
     * if no error occurred. A pointer to IInavlidArgumentError will be returned if the transmit buffer configuration is
     * invalid or could not be found. See \ref ErrorReporting for more information on how errors are reported. 
     */
    virtual IError* OLI_CALL GetTxBufferIndex( FlexRayChannel channel, uint32 slot, uint32 cyclePeriod, uint32 cycleOffset, uint32* pTxBufferIndex ) const OLI_NOTHROW = 0;

public:

    /// \name FlexRay-specific interface
	/// @{

    /**
     * This is a helper method which wraps a corresponding protected method: see \ref BinaryCompatibility and \ref ErrorReporting
     * for an explanation of why it is needed.
     */
    AutoPtr<ITxQueue> OLI_CALL CreateTxQueue( const uint32 bufferIndexes[], uint32 count )
    {
        ITxQueue* pTxQueue = NULL;
        CheckAndThrow( CreateTxQueue( bufferIndexes, count, &pTxQueue ) );

        // The wrapped method has already AddRef-ed the pointer, so we tell AutoPtr to take ownership of the pointer without a
        // further AddRef.
        return AutoPtr<ITxQueue>( pTxQueue, false );
    }

    /**
     * This is a helper method which wraps a corresponding protected method: see \ref BinaryCompatibility and \ref ErrorReporting
     * for an explanation of why it is needed.
     */
    uint32 OLI_CALL GetTxBufferIndex( FlexRayChannel channel, uint32 slot, uint32 cyclePeriod, uint32 cycleOffset ) const
    {
        uint32 txBufferIndex = 0;
        CheckAndThrow( GetTxBufferIndex( channel, slot, cyclePeriod, cycleOffset, &txBufferIndex ) );
        return txBufferIndex;
    }

	/// @}

    /**
     * This is a helper method which wraps IFlexRayLink_Create(): see \ref BinaryCompatibility and \ref ErrorReporting for an
     * explanation of why it is needed.
     */
    static AutoPtr<IFlexRayLink> OLI_CALL 
    Create( const char *URI, uint32 version = VERSION )
    {
        IFlexRayLink* pFlexRayLink = NULL;
        CheckAndThrow( IFlexRayLink_Create( URI, version, &pFlexRayLink ) );

        // The wrapped method has already AddRef-ed the pointer, so we tell AutoPtr to take ownership of the pointer without a
        // further AddRef.
        return AutoPtr<IFlexRayLink>( pFlexRayLink, false );
    }
};

// close ETAS::OLI namespace

#ifdef _DOXYGEN
}}
#endif

#include "../Common/EndNamespace.h"

#endif // !defined(__OLI_IFLEXRAYLINK_INCLUDED__)
