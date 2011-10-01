///////////////////////////////////////////////////////////
//  CANTxFrame.h
//  Defines the ICANTxFrame interface
///////////////////////////////////////////////////////////

#if !defined(__OLI_ICANTXFRAME_INCLUDED__)
#define __OLI_ICANTXFRAME_INCLUDED__

/// @todo update doxygen comments

// include used interface and constants

#include "CANBase.h"
#include "../Common/ITxFrame.h"
#include "../Common/ErrorInterfaces.h"
#include "../Common/ErrorProxies.h"

#include "../Common/BeginNamespace.h"

class ICANRxFrame;

/// \addtogroup GROUP_OLI_CAN_MESSAGES
/// @{

class ICANTxFrame;

/**
 * This function instantiates an object supporting ICANTxFrame. See \ref BinaryCompatibility for an explanation of why it is needed.
 *
 * NOTE that clients are encouraged to access this function via the wrapper ICANTxFrame::Create().
 *
 * \param[in]  id               The CAN message ID of the new TX frame.
 * \param[in]  extendedID       Indicates whether \a id is an extended CAN ID (29 bits) or a standard CAN ID (11 bits).
 * \param[in]  data             The TX frame payload.
 * \param[in]  size             The number of bytes at \a data.
 * \param[out] ppCanTxFrame     A pointer to an object supporting ICANTxFrame. The object is owned by the OLL, not by the caller.
 *                              The OLL assumes that the caller will eventually pass the object pointer back to the OLL for
 *                              transmission. If this is not done, the object's memory will leak.
 *
 * \return A pointer to an interface based on IError, describing the error which occurred during this function. NULL if no error
 * occurred. See \ref ErrorReporting for more information on how errors are reported.
 */
OLL_API IError* OLI_CALL ICANTxFrame_Create1( uint32 id, bool extendedID, const byte* data, uint32 size, ICANTxFrame** ppCanTxFrame );

/**
 * This function instantiates an object supporting ICANTxFrame. See \ref BinaryCompatibility for an explanation of why it is needed.
 *
 * NOTE that clients are encouraged to access this function via the wrapper ICANTxFrame::Create().
 *
 * \param[in]  received         A RX CAN frame which will be used to populate the TX frame instance.
 * \param[out] ppCanTxFrame     A pointer to an object supporting ICANTxFrame. The object is owned by the OLL, not by the caller.
 *                              The OLL assumes that the caller will eventually pass the object pointer back to the OLL for
 *                              transmission. If this is not done, the object's memory will leak.
 *
 * \return A pointer to an interface based on IError, describing the error which occurred during this function. NULL if no error
 * occurred. See \ref ErrorReporting for more information on how errors are reported.
 */
OLL_API IError* OLI_CALL ICANTxFrame_Create2( const ICANRxFrame* received, ICANTxFrame** ppCanTxFrame );

/** The only frame type available on CAN.
* Interfacewise, it adds flags for extended ids and remote frames.
*/

OLI_INTERFACE ICANTxFrame : public ITxFrame
{
protected:

    /// Instances of this interface are owned by the OLL.

    virtual ~ICANTxFrame() OLI_NOTHROW {};

public:

    /// returned by @ref GetType

    enum {TYPE = CAN_TYPE_FRAME_BASE + 1};

    /// \name CAN-specific interface

    /// @{
    virtual bool OLI_CALL IsExtendedID() const OLI_NOTHROW = 0;
    /// @}

    /**
     * This is a helper method which wraps ICANTxFrame_Create1(): see \ref BinaryCompatibility and \ref ErrorReporting for an
     * explanation of why it is needed.
     */
    static ICANTxFrame* OLI_CALL Create 
        ( uint32 id
        , bool extendedID
        , const byte* data
        , uint32 size )
    {
        ICANTxFrame* pCanTxFrame = NULL;
        CheckAndThrow( ICANTxFrame_Create1( id, extendedID, data, size, &pCanTxFrame ) );
        return pCanTxFrame;
    }

    /**
     * This is a helper method which wraps ICANTxFrame_Create2(): see \ref BinaryCompatibility and \ref ErrorReporting for an
     * explanation of why it is needed.
     */
    static ICANTxFrame* OLI_CALL Create( const ICANRxFrame* received )
    {
        ICANTxFrame* pCanTxFrame = NULL;
        CheckAndThrow( ICANTxFrame_Create2( received, &pCanTxFrame ) );
        return pCanTxFrame;
    }
};

/// @}

#include "../Common/EndNamespace.h"

#endif // !defined(__OLI_ICANTXFRAME_INCLUDED__)
