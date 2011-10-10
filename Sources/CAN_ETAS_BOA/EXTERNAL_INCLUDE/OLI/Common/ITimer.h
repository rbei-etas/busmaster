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
* @brief  ITimer definition
* @remark The header structure of the OLI may change
*         in future releases. Don't include this
*         header directly. Use @ref OLI.h instead.
*/

#if !defined(__OLI_ITIMER_INCLUDED__)
#define __OLI_ITIMER_INCLUDED__

#include "OLIDefs.h"
#include "AutoPtr.h"
#include "ITimerStatus.h"
#include "ErrorInterfaces.h"
#include "ErrorProxies.h"

// open ETAS::OLI namespace

#include "BeginNamespace.h"

#ifdef _DOXYGEN
namespace ETAS {namespace OLI {
#endif

/**
* @ingroup GROUP_OLI_COMMON_TIMERS
* @brief  Load a leap second file to allow for UTC / TAI conversion.
*
*         This function loads a leap second data file which is in the 
*         NIST format. See ftp://time.nist.gov/pub/leap-seconds.list
*         for a description and example of this format, along with the 
*         most current leap second data.
*
*         NOTE that clients are encouraged to access this function via 
*         the wrapper ITimer::LoadLeapSecondDataFile().
*
* @param[in] leapSecondDataFileName    
*         A NULL-terminated UTF-8 string containing the name of the leap 
*         second data file.
* @return A pointer to an interface based on @ref IError, describing 
*         the error which occurred during this function. @c NULL if 
*         no error occurred. See @ref ErrorReporting "error reporting"
*         for more information on how errors are reported.
*
* @exception <none> This function must not throw exceptions.
*
* @since  BOA 1.3 
* @see    @ref BinaryCompatibility "binary compatibility", 
*         @ref ErrorReporting "error reporting",
*         @ref IRxMessage::GetUTCTimeStamp, @ref IRxMessage::GetTAITimeStamp
*/
OLL_API IError* OLI_CALL ITimer_LoadLeapSecondDataFile( 
    const char* leapSecondDataFileName);


/** @ingroup GROUP_OLI_COMMON_TIMERS
* @brief  Invariant properties of the timer that is associated with the 
*         @ref ILink "link's" virtual controller instance.
*
*         Call @ref GetStatus to read the current timestamp an other
*         transient values.
*
* @remark All public methods are thread-safe.
* @remark Objects implementing this interface are gurateed to live as
*         long as the @ref ILink instance that returned them. 
*
* @since  BOA 1.3
* @see    ITimerStatus, ITimerEvent, ILink
*/

OLI_INTERFACE ITimer
{
protected:

    /** @brief Destructor.

        This destructor has been hidden because the client cannot
        directly control the lifetime of this object.

        @exception <none> This function must not throw exceptions.

        @since  BOA 1.3 
     */
    virtual ~ITimer() OLI_NOTHROW {};

    /** @brief  This method gets the current timer status.
     
                The caller should access this method via the public wrapper 
                @ref ITimer::GetStatus().

        @param [out] ppStatus    
                A pointer to an object describing the current status of 
                this timer. The object will already have a reference count 
                of 1 and must be reference-counted by the caller, using the 
                object's methods @ref IRefCountable::AddRef "AddRef()" and 
                @ref IRefCountable::Release "Release()". This is easily 
                done by wrapping the object pointer in an instance of the 
                @ref AutoPtr class, which will be done automatically if 
                the caller accesses this method via the wrapper 
                @ref ITimer::GetStatus().
        @return A pointer to an interface based on @ref IError, describing 
                the error which occurred during this function. @c NULL if 
                no error occurred. See @ref ErrorReporting "error reporting"
                for more information on how errors are reported.
       
        @exception <none> This function must not throw exceptions.
       
        @since  BOA 1.3 
        @see    @ref BinaryCompatibility "binary compatibility", 
                @ref ErrorReporting "error reporting",
                ITimerStatus
     */
    virtual IError* OLI_CALL 
        GetStatusRaw( ITimerStatus** ppStatus ) const OLI_NOTHROW = 0;

public:

    /** Available return values for @ref ITimer::GetFormat
     */
    enum Format
    {
        /* Timestamps are 64 bit integers. */
        BINARY_64BIT = 1
    };

    /** Available return values for @ref ITimer::GetLocalReferenceScale
     */
    enum ReferenceScale
    {
        /* The timer is not related to any standard time scale. */
		referenceScale_unknown = 0,

        /* The timer is based on TAI. */
		referenceScale_TAI = 1,

        /* The timer is based on UTC. */
		referenceScale_UTC = 2,
    };

    /** The return values of @ref ITimer::GetSyncSlave and @ref 
     *  ITimer::GetSyncMaster are a combination of these flags.
     */
    enum SyncState
    {
        /**
         * Flag to indicate the synchronization capabilities to an external 
         * reference timer.
         */
        SUPPORTS_EXTERNAL_SYNC = 1u << 31,

        /**
         * Flag to indicate that the timer supports the prorietary 
         * synchronization format of ETAS via an Ethernet connection.
         */
        ETAS_ETH         = 1<<0,

        /**
         * Flag to indicate that the timer supports synchronization to 
         * the IEEE1588 (precision time protocol).
         */
        IEEE1588         = 1<<1,

        /**
         * Flag to indicate that the timer supports synchronization to 
         * messages in the  NMEA-0183 ZDA message format 
         */
        GPS_NMEA_ZDA     = 1<<2,

        /**
         * Flag to indicate that the timer supports synchronization to 
         * a one pulse per second (1PPS) singnal of a GPS receiver and 
         * the NMEA-0183 ZDA message format. 
         */
        GPS_PPS          = 1<<3,

        /**
         * Flag to indicate that the timer supports synchronization to 
         * IRIG standard 200-2004 code G146. 
         */
        GPS_IRIG_G146    = 1<<4
    };

    /** @brief  This method returns an identifier for the timer.
     
                This string should be unique in the network. ETAS 
                recommends to use a string representation of an UUID.
                The system may generate a UUID in the boot phase. In case of 
                an ethernet based system, which is not used to generate other 
                UUIDs, the system can simply extend a template by the 
                MAC-Address to generate a UUID Version 1 
                (e.g. used by DCE V1.1).
         
                Example: 
                - Timestamp (low,mid,high) = 0, 
                - Clock Sequence = 0,
                - UUID-Version = 1, DCE variant, 
                - MAC-Address of the system = 00-60-34-00-14-00 
            
                ==> uuid = "00000000-0000-1000-8000-006034001400"

        @return Pointer to the UTF8 encoded timer ID of the local system.
       
        @exception <none> This function must not throw exceptions.
       
        @remark This clock ID will be used for timestamps unless the timer
                gets @ref ITimerStatus::IsSyncLocked "sync-locked" to an
                external clock.
        @since  BOA 1.3 
        @see    ITimerStatus::GetGlobalID, ITimerStatus::IsSyncLocked
     */
    virtual const char* OLI_CALL GetLocalID() const OLI_NOTHROW = 0;

    /** @brief  This method returns the timestamp representation format.

                Timestamps will always be 64 bit values but their 
                interpretation may vary (e.g. 2 concatenated 32 bit values
                for seconds and factions, respectively).

        @return The timestamp representation format.
       
        @exception <none> This function must not throw exceptions.

        @remark Currently, only @ref BINARY_64BIT is supported. Future 
                implementation may support different timestamp formats.
        @since  BOA 1.3 
     */
	virtual Format OLI_CALL GetFormat() const OLI_NOTHROW = 0;

    /** @brief  Returns the number of clock ticks per second.
     
                This is the numerical timer resolution.
                The current timer value devided by this tick frequency
                yields the number of seconds since passed since the
                @ref GetLocalTimeOriginIso8601 "timer origin".

        @return The number of clock ticks per second.
       
        @exception <none> This function must not throw exceptions.
       
        @since  BOA 1.3 
        @see    GetTicksPerIncrement, GetLocalTimeOriginIso8601
     */
    virtual uint32 OLI_CALL GetTickFrequency() const OLI_NOTHROW = 0;

    /** @brief  Returns minimum number of ticks between increments.
     
                This is a measurement for the actual timer resolution. 
                For instance, a value of 55 for a timer with a @ref 
                GetTickFrequency "numerical resolution" of 1000 ticks/sec, 
                has an actual resolution of about 55ms or 18Hz.

        @return The minimum number of ticks between increments.
       
        @exception <none> This function must not throw exceptions.

        @remark Timestamps are not necessarily a multiple of this value.
                It is strictly a lower limit to the resolution.
        @since  BOA 1.3 
        @see    GetTicksPerIncrement
     */
    virtual uint32 OLI_CALL GetTicksPerIncrement() const OLI_NOTHROW = 0;

    /** @brief  Get the stratum level.
     
                Stratum is used to describe the quality of a clock used for 
                synchronization. The ANSI Synchronization Interface Standard 
                T1.101 defines profiles for clock accuracy at each stratum 
                level, as does ITU standard G.810, and Telecordia/Bellcore 
                standards GR-253 and GR-1244.

        @return The stratum level. Systems with no information about the 
                relation to UTC or TAI use stratum Level 255.
       
        @exception <none> This function must not throw exceptions.
       
        @since  BOA 1.3 
        @see    GetLocalReferenceScale
     */
	virtual int OLI_CALL GetStratumLevel() const OLI_NOTHROW = 0;

    /** @brief  Get the reference scale.
     
                The reference scale is used for time stamping and for the
                @ref GetLocalTimeOriginIso8601 "timer origin". It defines
                the standard clock to which this timer synchronizes.

        @return The reference scale.
       
        @exception <none> This function must not throw exceptions.
       
        @since  BOA 1.3 
        @see    GetLocalTimeOriginIso8601
     */
	virtual ReferenceScale OLI_CALL 
        GetLocalReferenceScale() const OLI_NOTHROW = 0;

    /** @brief  Get a textual representation of the 0 timestamp.
     
                For synchronized clocks this string defines the origin of the 
                clock as a date and time in the Gregorian calendar and 
                International Atomic Time (TAI) or date and time in the 
                Gregorian calendar and Coordinated Universal Time (UTC).

                The binary timer value 0x0000 0000 0000 0000 is equivalent to 
                this time reference. This member represents a string in the 
                ISO8601:2000 format. 

        @return A textual representation of the 0 timestamp formatted 
                according to the ISO 8601 standard. For @ref 
                GetStratumLevel "stratum level" 255, this must be an
                empty string. Otherwise, it uses a complete representation 
                in the "Extended Format" (YYYY-MM-DDThh:mm:ss+/-hh:mm).
       
        @exception <none> This function must not throw exceptions.
       
        @since  BOA 1.3 
        @see    GetLocalReferenceScale, GetStratumLevel
     */
	virtual const char *OLI_CALL 
        GetLocalTimeOriginIso8601() const OLI_NOTHROW = 0;

    /** @brief  Get the supported mechanisms for synchronization to an 
                external reference clock.

        @return A combination of flags each representing a supported 
                mechanism for synchronization to an external reference clock.
       
        @exception <none> This function must not throw exceptions.
       
        @since  BOA 1.3 
        @see    GetLocalReferenceScale, GetSyncMaster
     */
	virtual uint32 OLI_CALL GetSyncSlave() const OLI_NOTHROW = 0;

    /** @brief  Get the supported mechanisms for generating a reference to an 
                external reference clock.

        @return A combination of flags each representing a supported 
                mechanism for generating a reference to an external reference 
                clock.
       
        @exception <none> This function must not throw exceptions.
       
        @since  BOA 1.3 
        @see    GetLocalReferenceScale, GetSyncSlave
     */
	virtual uint32 OLI_CALL GetSyncMaster() const OLI_NOTHROW = 0;

    /** @brief  This method gets the current timer status.
     
        @return New @ref ITimerStatus instance.
        @exception CError This function may throw an exception
                derived from @ref CError.

        @remark This is a helper method which wraps @ref GetStatusRaw(): 
                see @ref BinaryCompatibility "binary compatibility" 
                and @ref ErrorReporting "error reporting"
                for an explanation of why it is needed.
        @since  BOA 1.3 
        @see    ITimerStatus
     */
	AutoPtr<ITimerStatus> OLI_CALL GetStatus() const
    {
        ITimerStatus* pStatus = NULL;
        CheckAndThrow( GetStatusRaw ( &pStatus ) );

        // The wrapped method has already AddRef-ed the pointer, 
        // so we tell AutoPtr to take ownership of the pointer 
        // without a further AddRef.
        return AutoPtr<ITimerStatus>( pStatus, false );
    }

    /** @brief  Load a leap second file to allow for UTC / TAI conversion.
       
                This function loads a leap second data file which is in the 
                NIST format. See ftp://time.nist.gov/pub/leap-seconds.list
                for a description and example of this format, along with the 
                most current leap second data.
       
        @param[in] leapSecondDataFileName    
                A NULL-terminated UTF-8 string containing the name of the 
                leap second data file.
       
        @exception CError This function may throw an exception
                derived from @ref CError.
       
        @since  BOA 1.3 
        @see    IRxMessage::GetUTCTimeStamp, IRxMessage::GetTAITimeStamp
     */
	static void OLI_CALL 
        LoadLeapSecondDataFile( const char* leapSecondDataFileName )
    {
        CheckAndThrow( ITimer_LoadLeapSecondDataFile( leapSecondDataFileName ) );
    }
};

// close ETAS::OLI namespace

#ifdef _DOXYGEN
}}
#endif

#include "../Common/EndNamespace.h"

#endif // !defined(__OLI_ITIMER_INCLUDED__)
