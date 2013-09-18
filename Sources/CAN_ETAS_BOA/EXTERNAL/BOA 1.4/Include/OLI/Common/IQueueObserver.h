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
* @brief  IQueueObserver definition
* @remark The header structure of the OLI may change
*         in future releases. Don't include this
*         header directly. Use @ref OLI.h instead.
*/

#if !defined(__OLI_IQUEUEOBSERVER_INCLUDED__)
#define __OLI_IQUEUEOBSERVER_INCLUDED__

#include "OLIDefs.h"

// open ETAS::OLI namespace

#include "BeginNamespace.h"

#ifdef _DOXYGEN
namespace ETAS {namespace OLI {
#endif

// interface forward declarations

class IRxQueue;

/** @ingroup GROUP_OLI_COMMON_QUEUES
* @brief  Receives a notification whenever the @ref IRxQueue::GetWatermark
*         of a receive queue is reached or passed. 
*
*         It is recommended that to each @ref IQueue::IsActive "active"
*         @ref IRxQueue "receive queue", at least one object implementing
*         this observer interface is added. A given observer instance may
*         be registered on multiple @ref IRxQueue instances.
*
* @remark Objects implementing this interface must live as long as the
*         queues that they are registered at. 
*
* @since  BOA 1.3
* @see    @ref IRxQueue, @ref IRxQueue::GetWatermark
*/

OLI_INTERFACE IQueueObserver
{
public:

    /** @brief  Destructor.
        
                The topmost interface requires a virtual destructor.

        @exception <none> This function must not throw exceptions.

        @since  BOA 1.3 
     */
    virtual ~IQueueObserver() OLI_NOTHROW {};

    /** @brief  Called when enough messages have been queued. 
     
		        This callback function will be triggered whenever the receive
                @a queue's @ref IRxQueue::GetWatermark "watermark" has been 
                reached. It may modify the @a queue, e.g. by removing
                messages from it using @ref IRxQueue::Pop.

                The implementation is supposed to consume the messages in the
                receive queue. That is, if @ref IRxQueue::GetAutoPop 
                "auto-Pop" has not been set on this queue, the last observer
                must call @ref IRxQueue::Pop "queue->Pop()" explicitly.
                Because the notification order is undefined, this explicit
                removal scheme is only suitable if there is a single observer
                for the respective queue.

        @param  [in]  queue        
                Receive queue that just reached its watermark and this
                observer is registered to.

        @exception <none> This function must not throw exceptions.
       
        @coding The implementation must be thread-safe and re-entrant.

		@coding This method is called in the synchronization context of the
		        link owning @a queue. The observer should not call methods
		        that block for a long time (or indefinitely) and should also
		        not call other links to avoid circular lock dependencies
		        (and hence deadlocks). If either or both are necessary,
		        the corresponding work should be offloaded to a deferred
                handler.

		        The contents of the queue (and other queues on the same link)
		        may be modified after OnTrigger returns. Therefore, any 
                volatile information required by the deferred handler must 
                be extracted synchronously.

		        A canonical pseudo-code example:

		        @code
		        MyHandler::WorkPackage MyHandler::ExtractInfo(IRxQueue *queue)
		        {
		            // extraction of work package information goes here
		            // (in link's synchronization context)
		        }

		        void MyHandler::DeferredProcessing(WorkPackage workPackage)
		        {
		            // complex processing goes here (typically outside of
		            // all synchronization contexts)
		        }

		        void MyHandler::OnTrigger(IRxQueue *queue)
		        {
			        // The hypothetical MemberFunction wrapper takes an
                    // object reference, a reference to a non-static member
                    // function of the corresponding class and additional
                    // arguments and returns a functor.
                    //
			        // The Schedule function accepts a functor and schedules
                    // it for execution on a system thread, then returns
                    // immediately. Note that the WorkPackage object is 
                    // copied - passing a reference to a local variable to
                    // another thread is dangerous and in this context cleary
                    // wrong.

			        GetSystemQueue().Schedule
                        (MemberFunction (*this, 
                                         DeferredProcesing,
                                         ExtractInfo (queue));
		        }
		        @endcode

        @since  BOA 1.3 
        @see    @ref IRxQueue,
                @ref IRxQueue::GetWatermark "GetWatermark", 
                @ref IRxQueue::GetAutoPop "GetAutoPop", 
                @ref IRxQueue::Pop "Pop", 
                @ref IRxQueue::GetOldest "GetOldest", 
                @ref IRxQueue::GetMessage "GetMessage" 
	*/
    virtual void OLI_CALL OnTrigger (IRxQueue* queue) OLI_NOTHROW = 0;
};

// close ETAS::OLI namespace

#ifdef _DOXYGEN
}}
#endif

#include "EndNamespace.h"

#endif // !defined(__OLI_IQUEUEOBSERVER_INCLUDED__)
