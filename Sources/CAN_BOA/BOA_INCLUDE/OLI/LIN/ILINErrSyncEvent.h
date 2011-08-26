///////////////////////////////////////////////////////////
//  ILINErrSyncEvent.h
//  Defines the ILINErrSyncEvent interface
///////////////////////////////////////////////////////////

/// @todo update doxygen comments

#if !defined(__OLI_ILINERRSYNCEVENT__INCLUDED__)
#define __OLI_ILINERRSYNCEVENT__INCLUDED__

// include used interface and constants

#include "LINBase.h"
#include "../Common/IEvent.h"

#include "../Common/BeginNamespace.h"

/// \addtogroup GROUP_OLI_LIN_MESSAGES

/// @{

/** Represents the OCI_LINErrSyncEventMessage event.
*/

OLI_INTERFACE ILINErrSyncEvent : public IEvent
{
protected:

    /// Instances of this interface are owned by the OLL.

    virtual ~ILINErrSyncEvent() OLI_NOTHROW {};

public:

    /// returned by @ref GetType

    enum {TYPE = LIN_TYPE_EVENT_BASE + 5};

    /// \name LIN-specific interface
	/// @{

    virtual uint32 OLI_CALL GetBaudrate() const OLI_NOTHROW = 0;

	/// @}
};

/// @}

#include "../Common/EndNamespace.h"

#endif // !defined(__OLI_ILINERRSYNCEVENT__INCLUDED__)
