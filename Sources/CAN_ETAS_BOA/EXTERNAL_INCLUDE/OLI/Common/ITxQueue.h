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
* @brief  ITxQueue definition
* @remark The header structure of the OLI may change
*         in future releases. Don't include this
*         header directly. Use @ref OLI.h instead.
*/

/// @todo update doxygen comments

#if !defined(__OLI_ITXQUEUE_INCLUDED__)
#define __OLI_ITXQUEUE_INCLUDED__

// include base interface

#include "IQueue.h"
#include "../Common/ErrorInterfaces.h"
#include "../Common/ErrorProxies.h"

// open ETAS::OLI namespace

#include "BeginNamespace.h"

#ifdef _DOXYGEN
namespace ETAS {namespace OLI {
#endif

/// interace forward declarations

class ITxMessage;

/** 
* @ingroup GROUP_OLI_COMMON_QUEUES
* @brief Base interface for all transmit messages.
*
* In contrast to @ref IRxMessage "receive messages", these messages are 
* being created on behalf of the client application. Therefore, the 
* lifetime of the instances must be controlled by the client application: 
* To delete transmit messages created by some OLI factory called by the
* client, that client must call the @ref Destroy method once the message
* object is no longer needed.
*
* @remark All public methods are thread-safe.
* @coding Client applications may implement this interface.
* @since  BOA 1.3
* @see    ITxQueue, AutoDestroyPtr
*/

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

// close ETAS::OLI namespace

#ifdef _DOXYGEN
}}
#endif

#include "EndNamespace.h"

#endif // !defined(__OLI_ITXQUEUE_INCLUDED__)
