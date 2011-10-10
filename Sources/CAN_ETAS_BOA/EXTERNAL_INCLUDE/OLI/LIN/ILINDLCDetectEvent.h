/*
* Basic Open API - Open Link Interface
* Version 1.3
*
* Copyright (c) ETAS GmbH. All rights reserved.
*
* $Revision: 4794 $
*/

/** 
* @file
* @brief  ILINDLCDetectEvent definition
* @remark The header structure of the OLI may change
*         in future releases. Don't include this
*         header directly. Use @ref OLI.h instead.
*/

#if !defined(__OLI_ILINDLCDETECTEVENT__INCLUDED__)
#define __OLI_ILINDLCDETECTEVENT__INCLUDED__

// include used interface and constants

#include "LINBase.h"
#include "../Common/IEvent.h"

// open ETAS::OLI namespace

#include "../Common/BeginNamespace.h"

#ifdef _DOXYGEN
namespace ETAS {namespace OLI {
#endif

/** 
* @ingroup GROUP_OLI_LIN_MESSAGES
* @brief  LIN frame DLC detection event.
*
* Specific structure for a message concerning frame length (DLC)
* of an unconfigured receive LIN frame. If an unconfigured frame 
* is received, the hardware detects the according DLC by itself. 
* The message occurs only once for the according LIN ID.
*
* Extends the base interface by adding a method to access the @ref
* GetDLC "DLC being detected". 
*
* This interface's implementation of @ref IMessage::GetID returns the @ref IMessage::GetID "frame ID"
* of the frame that triggered the DLC detection event.
*
* @remark All public methods are thread-safe.
* @remark The lifetime of all objects implementing this interface 
*         is defined by the @ref IRxQueue "receive queue" instance
*         that contains them.
* @since  BOA 1.3
* @see    IRxQueue, ILINLink, ILINDLCDetectEventFilter
*/

OLI_INTERFACE ILINDLCDetectEvent : public IEvent
{
protected:

    /** @brief Destructor.

        This destructor has been hidden since objects implementing 
        this class are controlled by the receiving queue.

        @exception <none> This function must not throw exceptions.

        @since  BOA 1.3 
     */
    virtual ~ILINDLCDetectEvent() OLI_NOTHROW {};

public:

    /** The unique identifier for the type of this interface
        and will be returned by @ref IMessage::GetType.
     */
    enum {TYPE = LIN_TYPE_EVENT_BASE + 6};

    /** @brief  Get the detected DLC.
        
        @return Detected DLC. 
        @exception <none> This function must not throw exceptions.

        @since  BOA 1.3 
     */
    virtual uint8 OLI_CALL GetDLC() const OLI_NOTHROW = 0;
};

// close ETAS::OLI namespace

#ifdef _DOXYGEN
}}
#endif

#include "../Common/EndNamespace.h"

#endif // !defined(__OLI_ILINDLCDETECTEVENT__INCLUDED__)
