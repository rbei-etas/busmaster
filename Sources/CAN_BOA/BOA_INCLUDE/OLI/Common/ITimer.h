///////////////////////////////////////////////////////////
//  ITimer.h
//  Implementation of the Interface ITimer
//  Created on:      19-Mrz-2008 01:23:29
///////////////////////////////////////////////////////////

/// @todo update doxygen comments

#if !defined(__OLI_ITIMER_INCLUDED__)
#define __OLI_ITIMER_INCLUDED__

#include "OLIDefs.h"
#include "AutoPtr.h"
#include "ITimerStatus.h"
#include "ErrorInterfaces.h"
#include "ErrorProxies.h"

#include "BeginNamespace.h"

/// @ingroup GROUP_OLI_COMMON_TIMERS

/**
 * This function loads a leap second data file which is in the NIST format. See ftp://time.nist.gov/pub/leap-seconds.list
 * for a description and example of this format, along with the most current leap second data.
 *
 * NOTE that clients are encouraged to access this function via the wrapper ITimer::LoadLeapSecondDataFile().
 *
 * \param[in] leapSecondDataFileName    A NULL-terminated UTF-8 string containing the name of the leap second data file.
 *
 * \return A pointer to an interface based on IError, describing the error which occurred during this function. NULL
 * if no error occurred. See \ref ErrorReporting for more information on how errors are reported.
 */
OLL_API IError* OLI_CALL ITimer_LoadLeapSecondDataFile( const char* leapSecondDataFileName );

/**
	Contains the invariant properties of the timer that is
	associated with the link's controller. See the OCI timer
	description for details on those values.

	Call @ref GetStatus to read the current timestamp an other
	transient values.

	Instances of this class are immutable and thread-safe
	without synchronization. However, when the owning link
	is destroyed, the timer may be destroyed as well. Therefore,
	callers must ensure that the link remains alive until the
	ITimer instance is no longer needed.
*/

OLI_INTERFACE ITimer
{
protected:

    /// Topmost interface requires a virtual destructor.

    /// Instances implementing this interface are owned
    /// by the OLL and must not be destructed by the client.

    virtual ~ITimer() OLI_NOTHROW {};

    /**
     * This method gets the current timer status.
     *
     * The caller should access this method via the public wrapper ITimer::GetStatus().
     *
     * \param [out] ppStatus    A pointer to an object describing the current status of this timer. The object will already have
     *                          a reference count of 1 and must be reference-counted by the caller, using the object's methods
     *                          AddRef() and Release(). This is easily done by wrapping the object pointer in an instance of the
     *                          AutoPtr class, which will be done automatically if the caller accesses this method via the
     *                          wrapper ITimer::GetStatus().
     *
     * \return A pointer to an interface based on IError, describing the error which occurred during this function. NULL
     * if no error occurred. See \ref ErrorReporting for more information on how errors are reported.
     */
    virtual IError* OLI_CALL GetStatus( ITimerStatus** ppStatus ) const OLI_NOTHROW = 0;

public:

    /// return values for \ref ITimer::GetFormat

    enum Format
    {
        BINARY_64BIT = 1
    };

    /// return values for \ref ITimer::GetLocalReferenceScale

    enum ReferenceScale
    {
		referenceScale_unknown = 0,
		referenceScale_TAI = 1,
		referenceScale_UTC = 2,
    };

    /// return values of \ref ITimer::GetSyncSlave and \ref ITimer::GetSyncMaster
    /// are a combination of the following flags

    enum SyncState
    {
        /**
         * Flag to indicate the synchronization capabilities to an external reference timer 
         */
        SUPPORTS_EXTERNAL_SYNC = 1u << 31,

        /**
         * Flag to indicate that the timer supports the prorietary synchronization format of
         * ETAS via an Ethernet connection.
         */
        ETAS_ETH         = 1<<0,

        /**
         * Flag to indicate that the timer supports synchronization to the IEEE1588 (precision
         * time protocol).
         */
        IEEE1588         = 1<<1,

        /**
         * Flag to indicate that the timer supports synchronization to messages in the 
         * NMEA-0183 ZDA message format 
         */
        GPS_NMEA_ZDA     = 1<<2,

        /**
         * Flag to indicate that the timer supports synchronization to a one pulse per second
         * (1PPS) singnal of a GPS receiver and the NMEA-0183 ZDA message format. 
         */
        GPS_PPS          = 1<<3,

        /**
         * Flag to indicate that the timer supports synchronization to IRIG standard
         * 200-2004 code G146. 
         */
        GPS_IRIG_G146    = 1<<4
    };

    /// timer properties (see OCI for details)

    virtual const char* OLI_CALL GetLocalID() const OLI_NOTHROW = 0;

    /// return the timestamp representation format.

	virtual Format OLI_CALL GetFormat() const OLI_NOTHROW = 0;

    /// return the number of clock ticks per second

    virtual uint32 OLI_CALL GetTickFrequency() const OLI_NOTHROW = 0;

    /// return the minimum number of ticks between increments.

    virtual uint32 OLI_CALL GetTicksPerIncrement() const OLI_NOTHROW = 0;

    /// return the stratum level.

	virtual int OLI_CALL GetStratumLevel() const OLI_NOTHROW = 0;

    /// return the reference scale being used.

	virtual ReferenceScale OLI_CALL GetLocalReferenceScale() const OLI_NOTHROW = 0;

    /// return the textual representation of the "0" timestamp
    /// formatted according to the ISO 8601 standard.

	virtual const char *OLI_CALL GetLocalTimeOriginIso8601() const OLI_NOTHROW = 0;

    /// x

	virtual uint32 OLI_CALL GetSyncSlave() const OLI_NOTHROW = 0;

    /// x

	virtual uint32 OLI_CALL GetSyncMaster() const OLI_NOTHROW = 0;

    /// get the current timer status. 
    /// The caller owns the returned object.

    /**
     * This is a helper method which wraps a corresponding protected method: see \ref BinaryCompatibility and \ref ErrorReporting
     * for an explanation of why it is needed.
     */
	AutoPtr<ITimerStatus> OLI_CALL GetStatus() const
    {
        ITimerStatus* pStatus = NULL;
        CheckAndThrow( GetStatus( &pStatus ) );

        // The wrapped method has already AddRef-ed the pointer, so we tell AutoPtr to take ownership of the pointer without a
        // further AddRef.
        return AutoPtr<ITimerStatus>( pStatus, false );
    }

    /**
     * This is a helper method which wraps ITimer_LoadLeapSecondDataFile(): see \ref BinaryCompatibility and \ref ErrorReporting
     * for an explanation of why it is needed.
     */
	static void OLI_CALL LoadLeapSecondDataFile( const char* leapSecondDataFileName )
    {
        CheckAndThrow( ITimer_LoadLeapSecondDataFile( leapSecondDataFileName ) );
    }
};

#include "EndNamespace.h"

#endif // !defined(__OLI_ITIMER_INCLUDED__)
