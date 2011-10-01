///////////////////////////////////////////////////////////
//  ITimerStatus.h
//  Implementation of the Interface ITimerStatus
//  Created on:      19-Mrz-2008 01:23:29
///////////////////////////////////////////////////////////

/// @todo update doxygen comments

#if !defined(__OLI_ITIMERSTATUS_INCLUDED__)
#define __OLI_ITIMERSTATUS_INCLUDED__

#include "IRefCountable.h"

#include "BeginNamespace.h"


// interface forward declarations

class ITimer;

/// @ingroup GROUP_OLI_COMMON_TIMERS

/**
	Contains a snapshot of the variable properties (such
	as the current time stamp) of a given timer.

	Instances of ITimerStatus are immutable and thread-safe
	without synchronization. However, the caller must
	still provide proper lifetime management if instances
	are used from several threads.
*/

OLI_INTERFACE ITimerStatus : public IRefCountable
{
protected:

    /// Instances implementing this interface are owned
    /// by the OLL and must not be destructed by the client.

    virtual ~ITimerStatus() OLI_NOTHROW {};

public:

    /// timer properties (see OCI for details)

	virtual const char* OLI_CALL GetGlobalID() const OLI_NOTHROW = 0;
	virtual bool OLI_CALL IsSyncLocked() const OLI_NOTHROW = 0;
	virtual bool OLI_CALL IsPositiveLeapSecondAtEndOfDay() const OLI_NOTHROW = 0;
	virtual bool OLI_CALL IsNegativeLeapSecondAtEndOfDay() const OLI_NOTHROW = 0;
	virtual uint32 OLI_CALL GetEpoch() const OLI_NOTHROW = 0;
	virtual int64 OLI_CALL GetEstimatedOffsetTicks() const OLI_NOTHROW = 0;
	virtual int64 OLI_CALL GetTAI2UTCOffset() const OLI_NOTHROW = 0;
	virtual int64 OLI_CALL GetCurrentTime() const OLI_NOTHROW = 0;

    /// get a reference to the timer this status belongs to

	virtual const ITimer* OLI_CALL GetTimer() const OLI_NOTHROW = 0;

    /// call this method to delete this timer status instance.

};

#include "EndNamespace.h"

#endif // !defined(__OLI_ITIMERSTATUS_INCLUDED__)
