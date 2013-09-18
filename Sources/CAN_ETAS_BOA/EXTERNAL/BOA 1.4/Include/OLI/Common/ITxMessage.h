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
* @brief  ITxMessage definition
* @remark The header structure of the OLI may change
*         in future releases. Don't include this
*         header directly. Use @ref OLI.h instead.
*/

#if !defined(__OLI_ITXMESSAGE_INCLUDED__)
#define __OLI_ITXMESSAGE_INCLUDED__

// include base interface

#include "IMessage.h"

// open ETAS::OLI namespace

#include "BeginNamespace.h"

#ifdef _DOXYGEN
namespace ETAS {namespace OLI {
#endif

/** 
* @ingroup GROUP_OLI_COMMON_MESSAGES
* @brief Base interface for all transmit messages.
*
* In contrast to @ref IRxMessage "receive messages", these messages are 
* being created on behalf of the client application. Therefore, the 
* lifetime of the instances must be controlled by the client application: 
* To delete transmit messages created by some OLI factory called by the
* client, that client must call the @ref Destroy method once the message
* object is no longer needed.
*
* @remark All public methods except @ref Destroy are thread-safe.
* @coding Client applications may implement this interface.
* @since  BOA 1.3
* @see    ITxQueue, AutoDestroyPtr
*/
OLI_INTERFACE ITxMessage : public IMessage
{
protected:

    /** @brief Destructor.

        This destructor has been hidden since objects implementing 
        this class may need explicit destruction through @ref Destroy.

        @exception <none> This function must not throw exceptions.

        @since  BOA 1.3 
     */
    virtual ~ITxMessage() OLI_NOTHROW {};

public:

    /** @brief Destroys this instance.
        
        This method must be called when the respective instance has
        been created through an OLI factory method, i.e. whenever
        this interface has not been implemented by the client application.

        @exception <none> This function must not throw exceptions.

        @remark This method may not be thread-safe in the sense that
                it is not reentrant and will not synchronize with any
                calls to other methods in this interface. For most 
                applications, this is not an issue as their logic needs
                to coordinate when an object becomes invalidated.
        @remark Avoid calling @a Destroy manually. 
                Use the @ref AutoDestroyPtr wrapper template instead. 
        @since  BOA 1.3 
        @see    AutoDestroyPtr
     */
    virtual void Destroy() const OLI_NOTHROW = 0;
};

// close ETAS::OLI namespace

#ifdef _DOXYGEN
}}
#endif

#include "EndNamespace.h"

#endif // !defined(__OLI_ITXMESSAGE_INCLUDED__)
