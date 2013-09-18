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
* @brief  IQueue definition
* @remark The header structure of the OLI may change
*         in future releases. Don't include this
*         header directly. Use @ref OLI.h instead.
*/

#if !defined(__OLI_IQUEUE_INCLUDED__)
#define __OLI_IQUEUE_INCLUDED__

#include "IRefCountable.h"
#include "ErrorInterfaces.h"
#include "ErrorProxies.h"

// open ETAS::OLI namespace

#include "BeginNamespace.h"

#ifdef _DOXYGEN
namespace ETAS {namespace OLI {
#endif

// interface forward declarations

class IQueueObserver;

/**
* @ingroup GROUP_OLI_COMMON_QUEUES
* @brief Base interface for all queues.
*
* The interface provides an abstract notion of @ref IMessage
* "message" queue with a fill level. Access to the messages 
* will be offered in derived interfaces and will be different 
* for @ref IRxQueue "rx" and @ref ITxQueue "tx queues".
*
* Also, queues may be (de-)activated. Only when activated,
* will they send or receive data, respectively. Data coming
* from the controller will be lost for de-acticated receive 
* queues. To actually exchange messages with the OCI,
* the queue and respective link has to be active. (see @ref 
* ILink::IsActive) If not specified otherwise for a particular 
* link type, queues are active per default.
*
* @remark All public methods are thread-safe.
* @coding Client applications are not meant to implement
*         this interface.
* @since  BOA 1.3 
* @see    IRxQueue, ITxQueue, ILink::IsActive
*/

OLI_INTERFACE IQueue : public IRefCountable
{
protected:

    /** @brief Destructor.

        This destructor has been hidden to force the client
        application to use proper reference counting.

        @exception <none> This function must not throw exceptions.

        @since  BOA 1.3 
     */
    virtual ~IQueue() OLI_NOTHROW {};

    /** @brief  Activates or deactivates the queue.

                To actually send or receive messages, both the queue 
                and its associated link object must be active. 
     
                It is permitted to call @ref SetActiveRaw( @ref IsActive() ), 
                though no action will be taken by the OLL.
     
        @param[in] newVal    
                The new queue state. @c true means messages will be 
                exchanged with the OCI, if the respective @ref ILink 
                is active as well.
        @return A pointer to an interface based on @ref IError, 
                describing the error which occurred during this 
                function. @c NULL if no error occurred. See 
                @ref ErrorReporting "error reporting" for more 
                information on how errors are reported.
        @exception <none> This function must not throw exceptions.
     
        @remark The actually send or receive messages, both, queue
                and @ref ILink "link" must be active.
        @since  BOA 1.3 
        @see    SetActive, IsActive, ILink::IsActive, ILink::SetActive, 
                @ref ErrorReporting "error reporting" 
     */
    virtual IError* OLI_CALL SetActiveRaw( bool newVal ) OLI_NOTHROW = 0;

public:

    /** @brief  Get the number of messages in this queue.
        
        Returns the number of @ref IMessage "messages" currently
        queued in this instance. 

        @return The current number of entries in this queue.
        @exception <none> This function must not throw exceptions.

        @remark When called on a receive queue (@ref IRxQueue)
                outside a @ref IQueueObserver::OnTrigger,
                other threads like the OCI handler may add or remove
                messages. In that case, two consecutive calls to this 
                function may return different results.
        @since  BOA 1.3 
     */
	virtual uint32 OLI_CALL GetFillLevel() const OLI_NOTHROW = 0;

    /** @brief  Remove old entries from the queue.
        
        All messages older or just as old than the given time stamp
        will be removed from the queue.

        @param[in] checkpoint For @ref IRxQueue "receive queues", this is
                              method compares the @ref IRxMessage::GetRawTimeStamp
                              "raw time stamps".
                              Because there is no reliable time stamping
                              facility for transmit queues, the @a checkpoint
                              is simply interpreted as the maximum remaining 
                              @ref GetFillLevel "fill level" of that queue.
        @exception <none> This function must not throw exceptions.

        @remark For transmit queues (see @ref ITxQueue), there is no guarantee
                that the affected messages get actually discarded. Some
                or all of them may have been forwarded to the OCI layer
                by the time this method returns.
        @since  BOA 1.3 
        @see    GetFillLevel
     */
	virtual void OLI_CALL Clear (RawTime checkpoint) OLI_NOTHROW = 0;

    /** @brief  Test whether the queue is active.
        
        Returns whether this queue would receive / transmit 
        messages from / to the respective link.

        @return The current state of this queue. @c true means
                messages will be exchanged with the OCI, if the
                respective @ref ILink is active as well.
        @exception <none> This function must not throw exceptions.

        @remark The actually send or receive messages, both, queue
                and @ref ILink "link" must be active.
        @since  BOA 1.3 
        @see    SetActive, ILink::IsActive, ILink::SetActive 
     */
	virtual bool OLI_CALL IsActive() const OLI_NOTHROW = 0;

    /** @brief  Activates or deactivates the queue.

                To actually send or receive messages, both the queue 
                and its associated link object must be active. 
     
                It is permitted to call @ref SetActiveRaw( @ref IsActive() ), 
                though no action will be taken by the OLL.
     
        @param[in] newVal    
                The new queue state. @c true means messages will be 
                exchanged with the OCI, if the respective @ref ILink 
                is active as well.
        @exception CError This function may throw CError-derived exceptions.
     
        @remark The actually send or receive messages, both, queue
                and @ref ILink "link" must be active.
        @remark This is a helper method which wraps a corresponding 
                protected method (@ref SetActiveRaw): see
                @ref ErrorReporting "error reporting" for an explanation 
                of why it is needed.
        @since  BOA 1.3 
        @see    IsActive, ILink::IsActive, ILink::SetActive, 
                @ref ErrorReporting "error reporting" 
     */
    void OLI_CALL SetActive( bool newVal )
    {
        CheckAndThrow( SetActiveRaw( newVal ) );
    }
};

// close ETAS::OLI namespace

#ifdef _DOXYGEN
}}
#endif

#include "EndNamespace.h"

#endif // !defined(__OLI_IQUEUE_INCLUDED__)
