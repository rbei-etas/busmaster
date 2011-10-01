///////////////////////////////////////////////////////////
//  ICANRxFrame.h
//  Defines the ICANRxFrame interface
///////////////////////////////////////////////////////////

#if !defined(__OLI_ICANRXFRAME_INCLUDED__)
#define __OLI_ICANRXFRAME_INCLUDED__

/// @todo update doxygen comments

// include used interface and constants

#include "CANBase.h"
#include "../Common/IRxFrame.h"

#include "../Common/BeginNamespace.h"

/// \addtogroup GROUP_OLI_CAN_MESSAGES
/// @{

/** The only frame type available on CAN.
* Interfacewise, it adds flags for extended ids and remote frames.
*/

OLI_INTERFACE ICANRxFrame : public IRxFrame
{
protected:

    /// Instances of this interface are owned by the OLL.

    virtual ~ICANRxFrame() OLI_NOTHROW {};

public:

    /// returned by @ref GetType

    enum {TYPE = CAN_TYPE_FRAME_BASE + 1};

    /// \name CAN-specific interface

    /// @{

    virtual bool OLI_CALL IsExtendedID() const OLI_NOTHROW = 0;
    virtual bool OLI_CALL IsRemoteFrame() const OLI_NOTHROW = 0;

    /// @todo 2008-09-26, kuh: what is this property good for? thought remoteFrame tells if the frame came from another device - selfReception tells if it is from here!? --> one is useless or I misunderstood one or both of them. please clarify with comments here and in the constructor.
    virtual bool OLI_CALL IsSelfReception() const OLI_NOTHROW = 0;
    /// @}

};

/// @}

#include "../Common/EndNamespace.h"

#endif // !defined(__OLI_ICANRXFRAME_INCLUDED__)
