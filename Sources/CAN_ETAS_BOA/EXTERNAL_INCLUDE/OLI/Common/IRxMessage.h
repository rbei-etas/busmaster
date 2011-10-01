/*
* Basic Open API - Open Link Interface
* Version 1.3
*
* Copyright (c) ETAS GmbH. All rights reserved.
*/

/** 
* @file
* @brief  IRxMessage definition
* @remark The header structure of the OLI may change
*         in future releases. Don't include this
*         header directly. Use @ref OLI.h instead.
*/

#if !defined(__OLI_IRXMESSAGE_INCLUDED__)
#define __OLI_IRXMESSAGE_INCLUDED__

#include "OLITypes.h"
#include "IMessage.h"
#include "ErrorInterfaces.h"
#include "ErrorProxies.h"

// open ETAS::OLI namespace

#include "BeginNamespace.h"

#ifdef _DOXYGEN
namespace ETAS {namespace OLI {
#endif

/** 
* @ingroup GROUP_OLI_COMMON_MESSAGES
* @brief Base interface for all received messages.
*
* Extends the base interface by adding a receive time stamp.
* Up to three different time scales are available. The @ref
* GetRawTimeStamp "raw" format is the original value that
* had been attached to the message at the source. Its 
* interpretation depends on the respective link's @ref 
* ILink::GetTimer "timer".
*
* If available, the OLI will interpret that information for
* your convenience and convert the raw time stamp into @ref
* GetTAITimeStamp "TAI" and @ref GetUTCTimeStamp "UTC" time 
* stamps. The latter is only possible if the necessary information
* has been provided through @ref ITimer::LoadLeapSecondDataFile.
*
* @remark All public methods are thread-safe.
* @remark The lifetime of all objects implementing this interface 
*         is defined by the @ref IRxQueue "receive queue" instance
*         that contains them.
* @since  BOA 1.3
* @see    IRxQueue, ITimer
*/

OLI_INTERFACE IRxMessage : public IMessage
{
protected:

    /** @brief Destructor.

        This destructor has been hidden since objects implementing 
        this class are controlled by the receiving queue.

        @exception <none> This function must not throw exceptions.

        @since  BOA 1.3 
     */
    virtual ~IRxMessage() OLI_NOTHROW {};

    // the only class that is allowed to delete a generic receive message

    friend class CMessageShared;

    /**
     * This method gets the message reception time stamp on UTC scale. This is a simple transformation of the @ref GetTAITimeStamp
     * "TAI time stamp" to UTC. If the respective @ref ILink::GetTimer "timer" does not provide enough information, this method
     * will return a timestamp of 0.
     *
     * The conversion is possible, iff the respective @ref  ILink "link" instance's @ref ITimer "timer" uses a local
     * @ref ITimer::GetLocalReferenceScale "reference scale" other than @ref ITimer::referenceScale_unknown. However, a leap
     * second file must also have been  imported through @ref ITimer::LoadLeapSecondDataFile.
     *
     * \since  BOA 1.3 
     * \see    GetRawTimeStamp, GetTAITimeStamp, ITimer::LoadLeapSecondDataFile
     *
     * \param[out] pUtcTime     Frame reception or event creation time stamp on the  UTC time scale. 0, if unavailable.
     *
     * \return A pointer to an interface based on IError, describing the error which occurred during this function. NULL
     * if no error occurred. IInvalidArgumentError will be returned if the time stamp lies before the first leap second.
     * IInvalidStateError will be returned if No leap second file has been @ref ITimer::LoadLeapSecondDataFile "imported".
     * See \ref ErrorReporting for more information on how errors are reported.
     */
	virtual IError* OLI_CALL GetUTCTimeStamp( UTCTime* pUtcTime ) const OLI_NOTHROW = 0;

public:

    /** @brief  Raw message reception time stamp.
        
        For frames, this is the time stamp added by the firmware
        upon the frame's reception. The same applies to bus- or
        controller-related events.
        
        Some events, however, may be generated higher up in the 
        driver stack or at a point in time when no timer information 
        is available anymore (e.g. after unplugging cable between 
        PC and bus device).

        @return Frame reception or event creation time stamp
                on the uninterpreted, raw time scale.
                -1, if unavailable.
        @exception <none> This function must not throw exceptions.

        @remark The interpretation of the time stamps depends on
                the respective @ref ILink "link" instance's @ref
                ITimer "timer".
        @since  BOA 1.3 
        @see    ITimer, ILink::GetTimer
     */
	virtual RawTime OLI_CALL GetRawTimeStamp() const OLI_NOTHROW = 0;

    /**
     * This is a helper method which wraps a corresponding protected method: see \ref ErrorReporting for an explanation of why it is needed.
     */
	UTCTime OLI_CALL GetUTCTimeStamp() const
    {
        UTCTime utcTime;
        CheckAndThrow( GetUTCTimeStamp( &utcTime ) );
        return utcTime;
    }

    /** @brief  Message reception time stamp on TAI scale.
        
        This is a simple transformation of the @ref GetRawTimeStamp
        "raw" time stamp to TAI. If the respective @ref ILink::GetTimer
        "timer" does not provide enough information, this method
        will return 0.

        @return Frame reception or event creation time stamp on the 
                TAI time scale. 0, if unavailable.
        @exception <none> This function must not throw exceptions.

        @remark The conversion is possible, iff the respective @ref 
                ILink "link" instance's @ref ITimer "timer" uses
                a local @ref ITimer::GetLocalReferenceScale 
                "reference scale" other than @ref ITimer::referenceScale_unknown.
        @since  BOA 1.3 
        @see    GetRawTimeStamp, ITimer, ILink::GetTimer, ITimer::GetLocalReferenceScale
     */
	virtual TAITime OLI_CALL GetTAITimeStamp() const OLI_NOTHROW = 0;
};

// close ETAS::OLI namespace

#ifdef _DOXYGEN
}}
#endif

#include "EndNamespace.h"

#endif // !defined(__OLI_IRXMESSAGE_INCLUDED__)
