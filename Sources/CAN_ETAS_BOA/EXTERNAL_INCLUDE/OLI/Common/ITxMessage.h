///////////////////////////////////////////////////////////
//  ITxMessage.h
//  Definition of the ITxMessage interface
///////////////////////////////////////////////////////////

/// @todo update doxygen comments

#if !defined(__OLI_ITXMESSAGE_INCLUDED__)
#define __OLI_ITXMESSAGE_INCLUDED__

#include "IMessage.h"

#include "BeginNamespace.h"

/// @ingroup GROUP_OLI_COMMON_MESSAGES

/**
	Base interface for all frames and event interfaces.
	There is no distinction between send or receive messages
	to simplify bridging (just copy between rx and tx queues).

	Note that messages in rx queues have a limited
	lifetime that is controlled by the queue,
	observers on the queue or other objects. Do not
	store pointers or references to such messages
	unless you can guarantee that the message will not
	be removed from the queue in the meantime. The only
	way to guarantee this is to obtain a lock on the link
	owning the message before retrieving the message,
	and not calling Pop() on the queue or any method that
	may invoke Pop(). Because this blocks all activity on
	this link and may block some activity on other links,
	do not hold on to this lock for a prolonged period of
	time. Instead, when you do need to preserve a message,
	use the lock only to duplicate the message, then release
	the lock.
*/

OLI_INTERFACE ITxMessage : public IMessage
{
protected:

    /// Instances implementing this interface are owned
    /// by the OLL and must not be destructed by the client.

    virtual ~ITxMessage() OLI_NOTHROW {};

public:

    /// call this method to delete this TX message instance.

    virtual void Destroy() const OLI_NOTHROW = 0;
};

#include "EndNamespace.h"

#endif // !defined(__OLI_ITXMESSAGE_INCLUDED__)
