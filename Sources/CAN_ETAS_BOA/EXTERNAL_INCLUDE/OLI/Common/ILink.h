/*
* Basic Open API - Open Link Interface
* Version 1.3
*
* Copyright (c) ETAS GmbH. All rights reserved.
*
* $Revision: 4575 $
*/

/** 
* @file
* @brief  ILink definition
* @remark The header structure of the OLI may change
*         in future releases. Don't include this
*         header directly. Use @ref OLI.h instead.
*/

/// @todo update doxygen comments

#if !defined(__OLI_ILINK_INCLUDED__)
#define __OLI_ILINK_INCLUDED__

#include "IRefCountable.h"
#include "AutoPtr.h"
#include "IRxQueue.h"
#include "ErrorInterfaces.h"
#include "ErrorProxies.h"

// open ETAS::OLI namespace

#include "BeginNamespace.h"

#ifdef _DOXYGEN
namespace ETAS {namespace OLI {
#endif

// interface forward declarations

class IConfigurationContainer;
class IDictionary;
class ITimer;
class ITxQueue;

/// @ingroup GROUP_OLI_COMMON_LINK

/** A counted reference to some OCI-level controller handle.
* It provides its own configuration, timer and queue objects.
*
* The link exposes zero or more read and write queues. Since their
* creation may require bus-specific parameters, CreateRxQueue
* and CreateTxQueue cannot be declared in this base interface.
* Also, the number of queues that may be created depends on the
* bus type.
*
* Every link contains a mutex variable that must be used to
* synchronize access to the link itself and all dependent objects.
*/

OLI_INTERFACE ILink : public IRefCountable
{
protected:

    /// topmost interface requires a virtual destructor

    virtual ~ILink() OLI_NOTHROW {};

    /**
     * This method creates a RX queue and registers it for accepting incoming messages. This method can be used to create as
     * many RX queues as necessary provided sufficient resources are available.
     *
     * The caller should access this method via the public wrapper ILink::CreateRxQueue().
     *
     * \param [in]  watermark
     * \param [in]  autopop
     * \param [in]  enablePrebuf    Enables the queue to handle data which was acquired before the queue was created,
     *                              and which has been pre-buffered elsewhere.
     * \param [out] ppRxQueue       A pointer to a new RX queue which is associated with this link, and which has already been
     *                              AddRef-ed. The object must be reference-counted by the caller, using the object's methods
     *                              AddRef() and Release(). This is easily done by wrapping the object pointer in an instance of
     *                              the AutoPtr class, which will be done automatically if the caller accesses this method via
     *                              the wrapper ILink::CreateRxQueue().
     *
     * \return A pointer to an interface based on IError, describing the error which occurred during this function. NULL
     * if no error occurred. See \ref ErrorReporting for more information on how errors are reported.
     */
    virtual IError* OLI_CALL CreateRxQueue( uint32 watermark, bool autoPop, bool enablePrebuf, IRxQueue** ppRxQueue ) OLI_NOTHROW = 0;

    /**
     * This method provides access to the driver capabilities.
     *
     * \param[out] ppDict  The driver capabilities; this object is owned by the link instance.
     *
     * \return A pointer to an interface based on IError, describing the error which occurred during this function, or NULL
     * if no error occurred. See \ref ErrorReporting for more information on how errors are reported.
     */
    virtual IError* OLI_CALL GetCapabilities( const IDictionary** ppDict ) const OLI_NOTHROW = 0;

    /**
     * This method provides access to a read queue object.
     *
     * \param[in]  index
     * \param[out] ppRxQueue    The read queue; this object is owned by the link instance.
     *
     * \return A pointer to an interface based on IError, describing the error which occurred during this function, or NULL
     * if no error occurred. A pointer to IOutOfRangeError will be returned if \a index is out of bounds.
     * See \ref ErrorReporting for more information on how errors are reported.
     */
    virtual IError* OLI_CALL GetRxQueue( uint32 index, IRxQueue** ppRxQueue ) const OLI_NOTHROW = 0;

    /**
     * This method provides access to a write queue object.
     *
     * \param[in]  index
     * \param[out] ppTxQueue    The write queue; this object is owned by the link instance.
     *
     * \return A pointer to an interface based on IError, describing the error which occurred during this function, or NULL
     * if no error occurred. A pointer to IOutOfRangeError will be returned if \a index is out of bounds.
     * See \ref ErrorReporting for more information on how errors are reported.
     */
    virtual IError* OLI_CALL GetTxQueue( uint32 index, ITxQueue** ppTxQueue ) const OLI_NOTHROW = 0;

    /**
     * This method activates or deactivates the link. Calling SetActive( IsActive() ) is allowed, but takes no action.
     *
     * \param[in] newVal    Indicates the new state of the link: true => activated, false => deactivated
     *
     * \return A pointer to an interface based on IError, describing the error which occurred during this function, or NULL
     * if no error occurred. See \ref ErrorReporting for more information on how errors are reported.
     */
    virtual IError* OLI_CALL SetActiveRaw( bool newVal ) OLI_NOTHROW = 0;

public:

    /// access to the controller configuration object. Please note 
    /// that the container will initially be empty.
    /// The returned instance remains in the ownership of this link instance.

    virtual IConfigurationContainer* OLI_CALL GetConfiguration() const OLI_NOTHROW = 0;

    /**
     * This is a helper method which wraps a corresponding protected method: \ref ErrorReporting for an explanation of why it is needed.
     */
    const IDictionary* OLI_CALL GetCapabilities() const
    {
        const IDictionary* pDict = NULL;
        CheckAndThrow( GetCapabilities( &pDict ) );
        return pDict;
    }

    /// access to the timer associated with the underlying controller.
    /// The returned instance remains in the ownership of this link instance.

    virtual const ITimer* OLI_CALL GetTimer() const OLI_NOTHROW = 0;

    /// is this link receiving and sending messages?
    /// A queue will be active, if this and the queue's setting are 'active'.

    virtual bool OLI_CALL IsActive() const OLI_NOTHROW = 0;

    /**
     * This is a helper method which wraps a corresponding protected method: \ref ErrorReporting for an explanation of why it is needed.
     */
    void OLI_CALL SetActive (bool newVal)
    {
        CheckAndThrow( SetActiveRaw( newVal ) );
    }

    /**
     * This is a helper method which wraps a corresponding protected method: see \ref BinaryCompatibility and \ref ErrorReporting
     * for an explanation of why it is needed.
     */
    AutoPtr<IRxQueue> OLI_CALL CreateRxQueue( uint32 watermark = 1, bool autoPop = true, bool enablePrebuf = false )
    {
        IRxQueue* pRxQueue = NULL;
        CheckAndThrow( CreateRxQueue( watermark, autoPop, enablePrebuf, &pRxQueue ) );

        // The wrapped method has already AddRef-ed the pointer, so we tell AutoPtr to take ownership of the pointer without a
        // further AddRef.
        return AutoPtr<IRxQueue>( pRxQueue, false );
    }

    /// \returns the number of read queue objects.

    virtual uint32 OLI_CALL GetRxQueueCount() const OLI_NOTHROW = 0;

    /**
     * This is a helper method which wraps a corresponding protected method: \ref ErrorReporting for an explanation of why it is needed.
     */
    IRxQueue* OLI_CALL GetRxQueue (uint32 index) const
    {
        IRxQueue* pRxQueue = NULL;
        CheckAndThrow( GetRxQueue( index, &pRxQueue ) );
        return pRxQueue;
    }

    /// \returns the number of write queue objects.

    virtual uint32 OLI_CALL GetTxQueueCount() const OLI_NOTHROW = 0;

    /**
     * This is a helper method which wraps a corresponding protected method: \ref ErrorReporting for an explanation of why it is needed.
     */
    ITxQueue* OLI_CALL GetTxQueue (uint32 index) const
    {
        ITxQueue* pTxQueue = NULL;
        CheckAndThrow( GetTxQueue( index, &pTxQueue ) );
        return pTxQueue;
    }
};

// close ETAS::OLI namespace

#ifdef _DOXYGEN
}}
#endif

#include "EndNamespace.h"

#endif // !defined(__OLI_ILINK_INCLUDED__)
