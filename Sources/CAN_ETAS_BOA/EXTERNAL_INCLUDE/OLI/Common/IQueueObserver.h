///////////////////////////////////////////////////////////
//  IQueueObserver.h
//  Implementation of the Interface IQueueObserver
//  Created on:      19-Mrz-2008 01:23:28
///////////////////////////////////////////////////////////

/// @todo update doxygen comments

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

/// @ingroup GROUP_OLI_COMMON_QUEUES

/** Receives a notification whenever the @ref IRxQueue::GetWatermark of the
* queue is reached or passed. 
* A given observer may be registered on multiple @ref IQueue instances.
*/

OLI_INTERFACE IQueueObserver // Pure abstract class without vtable
{
public:

    /// topmost interface requires a virtual destructor

    virtual ~IQueueObserver() OLI_NOTHROW {};

    /**
		Callback function. May modify the @a queue, e.g. by removing
        messages from it.
		This method is called in the synchronization context of the
		link owning \a queue. The observer should not call methods
		that block for a long time (or indefinitely) and should also
		not call other links to avoid circular lock dependencies
		(and hence deadlocks). If either or both are necessary,
		the corresponding work should be offloaded to a deferred handler.

		The contents of the queue (and other queues on the same link)
		may be modified after OnTrigger returns. Therefore, any volatile
		information required by the deferred handler must be extracted
		synchronously.

		A canonical pseudo-code example:

		\code
		MyHandler::WorkPackage MyHandler::ExtractInformation(IQueue *queue)
		{
		// extraction of work package information goes here
		// (in link's synchronization context)
		}

		void MyHandler::DeferredProcessing(WorkPackage workPackage)
		{
		// complex processing goes here (typically outside of all
		// synchronization contexts)
		}

		void MyHandler::OnTrigger(IQueue *queue)
		{
			// The hypothetical MemberFunction wrapper takes an object reference,
			// a reference to a non-static member function of the corresponding class
			// and additional arguments and returns a functor.
			// The Schedule function accepts a functor and schedules it for execution
			// on a system thread, then returns immediately. Note that the WorkPackage
			// object is copied - passing a reference to a local variable to another
			// thread is dangerous and in this context cleary wrong.
			GetSystemQueue().Schedule(MemberFunction(*this, DeferredProcesing, ExtractInformation(queue));
		}
		\endcode

        TODO: add OLI_NOTHROW spec
	*/
    virtual void OLI_CALL OnTrigger (IRxQueue* queue) OLI_NOTHROW = 0;
};

// close ETAS::OLI namespace

#ifdef _DOXYGEN
}}
#endif

#include "EndNamespace.h"

#endif // !defined(__OLI_IQUEUEOBSERVER_INCLUDED__)
