///////////////////////////////////////////////////////////
//  IRxQueue.h
//  Implementation of the Interface IRxQueue
//  Created on:      19-Mrz-2008 01:23:29
//
//  $Revision: 4804 $
///////////////////////////////////////////////////////////

/// @todo update doxygen comments

#if !defined(__OLI_IRXQUEUE_INCLUDED__)
#define __OLI_IRXQUEUE_INCLUDED__

// include base interface

#include "IQueue.h"
#include "ErrorInterfaces.h"
#include "ErrorProxies.h"

#include "BeginNamespace.h"

// prevent collision with <winuser.h> definition

// The Windows SDK defines GetMessage as a preprocessor macro, so we must undefine it before we can use it as a function name.
#ifdef GetMessage 
#undef GetMessage
#endif

/// interface forward declarations

class IRxMessage;
class IFilter;

/// @ingroup GROUP_OLI_COMMON_QUEUES

/**
* \brief Receive queue
*
* It extends the base interface by a method 
* to extract the oldest message from the queue.
*
* The messages that will be received must match at least one
* filter condition. So, to receive any data, at least one
* filter must be registered.
*
* If multiple rx queues have been created for a link, they *all*
* first receive the data. Only after the last queue got filled,
* will the respective observers be triggered. Otherwise event
* queue listeners could get out of sync with queues receiving
* the same events in-band.
*/

OLI_INTERFACE IRxQueue : public IQueue
{
protected:

    /// Instances implementing this interface are owned
    /// by the OLL and must not be destructed by the client.

    virtual ~IRxQueue() OLI_NOTHROW {};

    /**
     * This method gets a pointer to the oldest message in the queue. The queue may be inactive.
     *
     * \param[out] ppRxMsg  The oldest message in the queue. The ownership of this object remains with the queue.
     *
     * \return A pointer to an interface based on IError, describing the error which occurred during this function, or NULL
     * if no error occurred. A pointer to IOutOfRangeError will be returned if the queue is empty.
     * See \ref ErrorReporting for more information on how errors are reported.
     */
    virtual IError* OLI_CALL GetOldest( const IRxMessage** ppRxMsg ) const OLI_NOTHROW = 0;

    /**
     * This method gets a pointer to a message in the queue. The queue may be inactive.
     *
     * \param[in]  index    The 0-based index of the message to get. For \a index == 0, this method is equivalent to GetOldest().
     * \param[out] ppRxMsg  The specified message in the queue. The ownership of this object remains with the queue.
     *
     * \return A pointer to an interface based on IError, describing the error which occurred during this function, or NULL
     * if no error occurred. A pointer to IOutOfRangeError will be returned if \a index exceeds the queue's depth.
     * See \ref ErrorReporting for more information on how errors are reported.
     */
    virtual IError* OLI_CALL GetMessage( uint32 index, const IRxMessage** ppRxMsg ) const OLI_NOTHROW = 0;

    /**
     * This method registers an observer. It may be called at any time, even while observer callbacks are in progress.
     *
     * \param[in] pObserver     The observer. This object remains in the ownership of the caller.
     *
     * \return A pointer to an interface based on IError, describing the error which occurred during this function, or NULL
     * if no error occurred. See \ref ErrorReporting for more information on how errors are reported.
     */
    virtual IError* OLI_CALL AddObserverRaw( IQueueObserver* pObserver ) OLI_NOTHROW = 0;

    /**
     * This method removes a registered observer. It may be called at any time, even while observer callbacks are in progress.
     *
     * \param[in] pObserver     The observer to be removed.
     *
     * \return A pointer to an interface based on IError, describing the error which occurred during this function, or NULL
     * if no error occurred. See \ref ErrorReporting for more information on how errors are reported.
     */
    virtual IError* OLI_CALL RemoveObserverRaw( IQueueObserver* pObserver ) OLI_NOTHROW = 0;

    /**
     * This method adds a filter. It may be called for active as well as inactive queues. Since the queue will create a copy of
     * the provided filter, the original \a pFilter instance remains in the ownership of the caller. Unless a user-defined
     * implementation uses a shared filter state, the queue-private copy will not be affected by changes to the original filter
     * object.
     *
     * \param[in]  pFilter          The filter to be added. Ownership remains with the caller, since this method creates a copy
     *                              of the provided filter.
     * \param[out] pFilterToken     This token must be provided if RemoveFilter() is called.
     *
     * \return A pointer to an interface based on IError, describing the error which occurred during this function, or NULL
     * if no error occurred. See \ref ErrorReporting for more information on how errors are reported.
     */
	virtual IError* OLI_CALL AddFilter( const IFilter* pFilter, uint32* pFilterToken ) OLI_NOTHROW = 0;

    /**
     * This method removes a filter which was previously added with AddFilter().
     *
     * \param[in]  filterToken      The token supplied by a previous call to AddFilter().
     * \param[out] pbResult         An indication of whether the filter was found and removed.
     *
     * \return A pointer to an interface based on IError, describing the error which occurred during this function, or NULL
     * if no error occurred. See \ref ErrorReporting for more information on how errors are reported.
     */
    virtual IError* OLI_CALL RemoveFilter( uint32 filterToken, bool* pbResult ) OLI_NOTHROW = 0;

public:

    /// Get a reference to the oldest messages in the queue. 
    /// May be called on inactive queues. The ownership of the 
    /// returned message object remains with the queue.
    /// Throws an exception for empty queues.

    /**
     * This is a helper method which wraps a corresponding protected method: \ref ErrorReporting for an explanation of why it is needed.
     */
    const IRxMessage* OLI_CALL GetOldest() const
    {
        const IRxMessage* pRxMsg = NULL;
        CheckAndThrow( GetOldest( &pRxMsg ) );
        return pRxMsg;
    }

    /**
     * This is a helper method which wraps a corresponding protected method: \ref ErrorReporting for an explanation of why it is needed.
     */
    const IRxMessage* OLI_CALL GetMessage (uint32 index) const
    {
        const IRxMessage* pRxMsg = NULL;
        CheckAndThrow( GetMessage( index, &pRxMsg ) );
        return pRxMsg;
    }

    /// Remove the @a count oldest messages from the queue. 
    /// May be called on inactive queues. 
    /// If the queues contains less than @a count messages,
    /// \ref Pop will remove all of them and return.
    /// \param count number of messages to remove

    virtual void OLI_CALL Pop (uint32 count = 1) OLI_NOTHROW = 0;

    /**
     * This is a helper method which wraps a corresponding protected method: \ref ErrorReporting for an explanation of why it is needed.
     */
	uint32 OLI_CALL AddFilter( const IFilter* pFilter )
    {
        uint32 filterToken = 0;
        CheckAndThrow( AddFilter( pFilter, &filterToken ) );
        return filterToken;
    }

    /**
     * This is a helper method which wraps a corresponding protected method: \ref ErrorReporting for an explanation of why it is needed.
     */
    bool OLI_CALL RemoveFilter (uint32 token)
    {
        bool bResult = false;
        CheckAndThrow( RemoveFilter( token, &bResult ) );
        return bResult;
    }

    /// 

	virtual bool OLI_CALL GetAutoPop() const OLI_NOTHROW = 0;

    /// fill level to reach to get this observer triggered.
    /// Must not change over the lifetime of this instance.

	virtual uint32 OLI_CALL GetWatermark() const OLI_NOTHROW = 0;

    /**
     * This is a helper method which wraps a corresponding protected method: \ref ErrorReporting for an explanation of why it is needed.
     */
    void OLI_CALL AddObserver (IQueueObserver* observer)
    {
        CheckAndThrow( AddObserverRaw( observer ) );
    }

    /**
     * This is a helper method which wraps a corresponding protected method: \ref ErrorReporting for an explanation of why it is needed.
     */
    void OLI_CALL RemoveObserver (IQueueObserver* observer)
    {
        CheckAndThrow( RemoveObserverRaw( observer ) );
    }
};

#include "EndNamespace.h"

#endif // !defined(__OLI_IRXQUEUE_INCLUDED__)
