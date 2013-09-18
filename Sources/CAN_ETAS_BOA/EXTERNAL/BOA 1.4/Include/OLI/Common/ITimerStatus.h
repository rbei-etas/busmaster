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
* @brief  ITimerStatus definition
* @remark The header structure of the OLI may change
*         in future releases. Don't include this
*         header directly. Use @ref OLI.h instead.
*/

#if !defined(__OLI_ITIMERSTATUS_INCLUDED__)
#define __OLI_ITIMERSTATUS_INCLUDED__

#include "IRefCountable.h"

// open ETAS::OLI namespace

#include "BeginNamespace.h"

#ifdef _DOXYGEN
namespace ETAS {namespace OLI {
#endif

// interface forward declarations

class ITimer;

/** @ingroup GROUP_OLI_COMMON_TIMERS
* @brief  Contains a snapshot of the variable properties (such as the 
*         current time stamp) of a given @ref ITimer "timer".
*
* @remark All public methods are thread-safe.
* @remark Instances of ITimerStatus are immutable but the caller must
*         still guarantee proper lifetime management through reference 
*         counting.
*
* @since  BOA 1.3
* @see    ITimer
*/

OLI_INTERFACE ITimerStatus : public IRefCountable
{
protected:

    /** @brief  Destructor.

        This destructor has been hidden to force the client
        application to use proper reference counting.

        @exception <none> This function must not throw exceptions.

        @since  BOA 1.3 
     */
    virtual ~ITimerStatus() OLI_NOTHROW {};

public:

    /** @brief  This method returns an identifier for the timer.
     
                This string should be unique in the network. ETAS 
                recommends to use a string representation of an UUID.
                If two clockIDs are equal, it can be assumed that both 
                designate the same timer / clock source, deliver the same
                absolute value and do not drift.

        @return Pointer to the UTF8 encoded timer ID currently used for
                timestamping.
       
        @exception <none> This function must not throw exceptions.

        @remark Changing the cabeling between hardware devices and clocks
                may result in a change of the reference clock.
        @since  BOA 1.3 
        @see    ITimer::GetLocalID
     */
	virtual const char* OLI_CALL GetGlobalID() const OLI_NOTHROW = 0;

    /** @brief  This method indicates whether the synchronization is locked.
     
                This information is independent of the @ref 
                ITimer::GetStratumLevel "stratum level" of the reference 
                clock. Even when different timer are synchronized to a clock 
                with a bad stratum level all sync'd timer can offer a high 
                relative accuracy against each other.

                When an external synchronization is lost, the system must
                change to a @ref ITimer::GetLocalID "local reference clock" 
                and change the @ref GetGlobalID "clock ID" and stratumLevel 
                to the local defaults.

                Nevertheless the timer must maintain a monotonous increasing 
                timestamp with the same nominal frequency.

        @return @c true, if the synchronization is locked to @ref GetGlobalID
                and the absolute offset to the reference is less then 
                @ref GetEstimatedOffsetTicks.
       
        @exception <none> This function must not throw exceptions.

        @since  BOA 1.3 
        @see    GetGlobalID, GetEstimatedOffsetTicks, ITimer::GetStratumLevel
     */
	virtual bool OLI_CALL IsSyncLocked() const OLI_NOTHROW = 0;

    /** @brief  This method indicates whether there will be a positive leap 
                second at the end of day.
     
        @return @c true, the current day will be one second longer, i.e.
                23:59:60 UTC will be inserted.
       
        @exception <none> This function must not throw exceptions.

        @since  BOA 1.3 
        @see    IsNegativeLeapSecondAtEndOfDay
     */
    virtual bool OLI_CALL 
        IsPositiveLeapSecondAtEndOfDay() const OLI_NOTHROW = 0;

    /** @brief  This method indicates whether there will be a negative leap 
                second at the end of day.
     
        @return @c true, the current day will be one second longer, i.e.
                23:59:59 UTC will be missing.
       
        @exception <none> This function must not throw exceptions.

        @since  BOA 1.3 
        @see    IsPositiveLeapSecondAtEndOfDay
     */
	virtual bool OLI_CALL 
        IsNegativeLeapSecondAtEndOfDay() const OLI_NOTHROW = 0;

    /** @brief  Returns the epoch of the time stamp.
     
                Whenever system can not guarantee a monotonous increment 
                of the time stamp this value will be incremented. In typical 
                cases this will happen when the sync was lost and 
                re-established or the source of the sync signal has changed. 

        @return Current epoch.
       
        @exception <none> This function must not throw exceptions.

        @remark Timestamps from different epochs may not be comparable.
                
        @since  BOA 1.3 
        @see    GetCurrentTime
     */
	virtual uint32 OLI_CALL GetEpoch() const OLI_NOTHROW = 0;

    /** @brief  Get the estimated maximum offset to the time referenced 
                by @ref GetGlobalID.
     
                This value is an indicator for the quality of the 
                synchronization inside of a cluster, but independent to the 
                @ref ITimer::GetStratumLevel "stratum level" of the 
                reference clock for that cluster.

        @return The estimated maximum offset to the time referenced 
                by @ref GetGlobalID.
       
        @exception <none> This function must not throw exceptions.

        @since  BOA 1.3 
        @see    GetGlobalID, IsSyncLocked
     */
    virtual int64 OLI_CALL GetEstimatedOffsetTicks() const OLI_NOTHROW = 0;

    /** @brief  Get the current difference between TAI and UTC timestamps.
     
                If @ref IsNegativeLeapSecondAtEndOfDay and @ref 
                IsNegativeLeapSecondAtEndOfDay return @c false, this offset 
                will be constant for at least 24 hours. If one of the flags 
                is set the value will be incremented or decremented at the 
                end of the day (UTC).

        @return The current difference between TAI and UTC timestamps.
                Systems without relation to TAI / UTC deliver MIN_SINT32 
                (-0x8000 0000). Systems with relation to TAI, but no 
                information about the leap seconds may also return MIN_SINT32.
       
        @exception <none> This function must not throw exceptions.

        @since  BOA 1.3 
        @see    IsNegativeLeapSecondAtEndOfDay, IsNegativeLeapSecondAtEndOfDay
     */
    virtual int64 OLI_CALL GetTAI2UTCOffset() const OLI_NOTHROW = 0;

    /** @brief  Get the current timer value.
     
        @return The timer value at the moment the request was served
                by the hardware.
       
        @exception <none> This function must not throw exceptions.

        @since  BOA 1.3 
        @see    GetEpoch
     */
    virtual int64 OLI_CALL GetCurrentTime() const OLI_NOTHROW = 0;

    /** @brief  Get the timer description interface.
     
        @return The timer description interface.
       
        @exception <none> This function must not throw exceptions.

        @remark The pointer will remain valid at least as long as the 
                respective @ref ILink instance or this timer status
                instances exists, whichever lasts longer.
        @since  BOA 1.3 
        @see    GetEpoch
     */
	virtual const ITimer* OLI_CALL GetTimer() const OLI_NOTHROW = 0;
};

// close ETAS::OLI namespace

#ifdef _DOXYGEN
}}
#endif

#include "../Common/EndNamespace.h"

#endif // !defined(__OLI_ITIMERSTATUS_INCLUDED__)
