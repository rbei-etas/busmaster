///////////////////////////////////////////////////////////
//  IFlexRayTxFrame.h
//  Definition of the IFlexRayTxFrame interface
///////////////////////////////////////////////////////////

/// @todo update doxygen comments

#if !defined(__OLI_IFLEXRAYTXFRAME_INCLUDED__)
#define __OLI_IFLEXRAYTXFRAME_INCLUDED__

// include used interface and constants

#include "FlexRayBase.h"
#include "../Common/ITxFrame.h"
#include "../Common/ErrorInterfaces.h"
#include "../Common/ErrorProxies.h"

#include "../Common/BeginNamespace.h"

class IFlexRayRxFrame;

/// \addtogroup GROUP_OLI_FLEXRAY_MESSAGES

/// @{

class IFlexRayTxFrame;

/**
 * This function instantiates an object supporting IFlexRayTxFrame. See \ref BinaryCompatibility for an explanation of why
 * it is needed.
 *
 * NOTE that clients are encouraged to access this function via the wrapper IFlexRayTxFrame::Create().
 *
 * \param[in]  flags
 * \param[in]  payloadLength
 * \param[in]  data
 * \param[out] ppFlexRayTxFrame     A pointer to an object supporting IFlexRayTxFrame. The object is owned by the OLL, not by
 *                                  the caller. The OLL assumes that the caller will eventually pass the object pointer back
 *                                  to the OLL for transmission. If this is not done, the object's memory will leak.
 *
 * \return A pointer to an interface based on IError, describing the error which occurred during this function. NULL if no error
 * occurred. See \ref ErrorReporting for more information on how errors are reported.
 */
OLL_API IError* OLI_CALL IFlexRayTxFrame_Create1( uint16 flags, uint8 payloadLength, const byte* data, IFlexRayTxFrame** ppFlexRayTxFrame );

/**
 * This function instantiates an object supporting IFlexRayTxFrame. See \ref BinaryCompatibility for an explanation of why
 * it is needed.
 *
 * NOTE that clients are encouraged to access this function via the wrapper IFlexRayTxFrame::Create().
 *
 * \param[in]  received             A RX FlexRay frame which will be used to populate the TX frame instance.
 * \param[in]  flags
 * \param[out] ppFlexRayTxFrame     A pointer to an object supporting IFlexRayTxFrame. The object is owned by the OLL, not by
 *                                  the caller. The OLL assumes that the caller will eventually pass the object pointer back
 *                                  to the OLL for transmission. If this is not done, the object's memory will leak.
 *
 * \return A pointer to an interface based on IError, describing the error which occurred during this function. NULL if no error
 * occurred. See \ref ErrorReporting for more information on how errors are reported.
 */
OLL_API IError* OLI_CALL IFlexRayTxFrame_Create2( const IFlexRayRxFrame* received, uint16 flags, IFlexRayTxFrame** ppFlexRayTxFrame );

/** The only frame type available on FlexRay.
*/

OLI_INTERFACE IFlexRayTxFrame : public ITxFrame
{
protected:

    /// Instances of this interface are owned by the OLL.

    virtual ~IFlexRayTxFrame() OLI_NOTHROW {};

public:

    /// returned by @ref GetType

    enum {TYPE = FLEXRAY_TYPE_FRAME};

    /// \name FlexRay-specific r/o interface
	/// @{

    virtual uint16 OLI_CALL GetFlags() const OLI_NOTHROW = 0;

	/// @}

    /**
     * This is a helper method which wraps IFlexRayTxFrame_Create1(): see \ref BinaryCompatibility and \ref ErrorReporting for an
     * explanation of why it is needed.
     */
    static IFlexRayTxFrame* OLI_CALL Create(
        uint16      flags,
        uint8       payloadLength,
        const byte* data )
    {
        IFlexRayTxFrame* pFlexRayTxFrame = NULL;
        CheckAndThrow( IFlexRayTxFrame_Create1( flags, payloadLength, data, &pFlexRayTxFrame ) );
        return pFlexRayTxFrame;
    }

    /**
     * This is a helper method which wraps IFlexRayTxFrame_Create2(): see \ref BinaryCompatibility and \ref ErrorReporting for an
     * explanation of why it is needed.
     */
    static IFlexRayTxFrame* OLI_CALL Create(
        const IFlexRayRxFrame*  received,
        uint16                  flags )
    {
        IFlexRayTxFrame* pFlexRayTxFrame = NULL;
        CheckAndThrow( IFlexRayTxFrame_Create2( received, flags, &pFlexRayTxFrame ) );
        return pFlexRayTxFrame;
    }
};

/// @}

#include "../Common/EndNamespace.h"

#endif // !defined(__OLI_IFLEXRAYTXFRAME_INCLUDED__)
