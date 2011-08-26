///////////////////////////////////////////////////////////
//  ICANErrorFrame.h
//  Defines the ICANErrorFrame interface
///////////////////////////////////////////////////////////

#if !defined(__OLI_ICANERRORFRAME__INCLUDED__)
#define __OLI_ICANERRORFRAME__INCLUDED__

/// @todo update doxygen comments

// include used interface and constants

#include "CANBase.h"
#include "../Common/IEvent.h"
#include "../Common/BeginNamespace.h"

/// \addtogroup GROUP_OLI_CAN_MESSAGES
/// @{

/** 
*/

OLI_INTERFACE ICANErrorFrame : public IEvent
{
protected:

    /// Instances of this interface are owned by the OLL.

    virtual ~ICANErrorFrame() OLI_NOTHROW {};

public:

    /// returned by @ref GetType

    enum {TYPE = CAN_TYPE_EVENT_BASE + 2};
};

/// @}

#include "../Common/EndNamespace.h"

#endif // !defined(__OLI_ICANERRORFRAME__INCLUDED__)
