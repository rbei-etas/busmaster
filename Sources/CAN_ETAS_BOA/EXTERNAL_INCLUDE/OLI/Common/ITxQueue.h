///////////////////////////////////////////////////////////
//  ITxQueue.h
//  Implementation of the Interface ITxQueue
//  Created on:      19-Mrz-2008 01:23:29
///////////////////////////////////////////////////////////

/// @todo update doxygen comments

#if !defined(__OLI_ITXQUEUE_INCLUDED__)
#define __OLI_ITXQUEUE_INCLUDED__

// include base interface

#include "IQueue.h"
#include "../Common/ErrorInterfaces.h"
#include "../Common/ErrorProxies.h"

#include "BeginNamespace.h"

/// interace forward declarations

class ITxMessage;

/// @ingroup GROUP_OLI_COMMON_QUEUES

/** Send queue. It extends the base interface by a method 
* to append a message to the queue.
*/

OLI_INTERFACE ITxQueue : public IQueue
{
protected:

    /// Instances implementing this interface are owned
    /// by the OLL and must not be destructed by the client.

    virtual ~ITxQueue() OLI_NOTHROW {};

    /**
     * This method queues a message for transmission. It may block for an active queue if its capacity is reached.
     *
     * \param[in] message   The message to be transmitted; ownership remains with the caller.
     *
     * \return A pointer to an interface based on IError, describing the error which occurred during this function. NULL
     * if no error occurred. See \ref ErrorReporting for more information on how errors are reported.
     */
    virtual IError* OLI_CALL AddRaw( const ITxMessage* message ) OLI_NOTHROW = 0;

    /**
     * This method queues an array of messages for transmission. It may block for an active queue if its capacity is reached.
     *
     * \param[in] messages  The messages to be transmitted; ownership of the messages remains with the caller.
     * \param[in] count     The number of messages to be transmitted.
     *
     * \return A pointer to an interface based on IError, describing the error which occurred during this function. NULL
     * if no error occurred. See \ref ErrorReporting for more information on how errors are reported.
     */
    virtual IError* OLI_CALL AddRaw( const ITxMessage* messages[], uint32 count ) OLI_NOTHROW = 0;

public:

    /**
     * This is a helper method which wraps a corresponding protected method: see \ref ErrorReporting for an explanation of why it is needed.
     */
    void OLI_CALL Add( const ITxMessage* message )
    {
        CheckAndThrow( AddRaw( message ) );
    }

    /**
     * This is a helper method which wraps a corresponding protected method: see \ref ErrorReporting for an explanation of why it is needed.
     */
    void OLI_CALL Add( const ITxMessage* messages[], uint32 count )
    {
        CheckAndThrow( AddRaw( messages, count ) );
    }
};

#include "EndNamespace.h"

#endif // !defined(__OLI_ITXQUEUE_INCLUDED__)
