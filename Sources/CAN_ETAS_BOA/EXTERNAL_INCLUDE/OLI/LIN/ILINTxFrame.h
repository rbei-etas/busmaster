///////////////////////////////////////////////////////////
//  ILINTxFrame.h
//  Defines the ILINTxFrame interface
///////////////////////////////////////////////////////////

/// @todo update doxygen comments

#if !defined(__OLI_ILINTXFRAME_INCLUDED__)
#define __OLI_ILINTXFRAME_INCLUDED__

// include used interface and constants

#include "LINBase.h"
#include "../Common/ITxFrame.h"
#include "../Common/ErrorInterfaces.h"
#include "../Common/ErrorProxies.h"

#include "../Common/BeginNamespace.h"

class ILINRxFrame;

/// \addtogroup GROUP_OLI_LIN_MESSAGES

/// @{

class ILINTxFrame;

/**
 * This function instantiates an object supporting ILINTxFrame. See \ref BinaryCompatibility for an explanation of why it is needed.
 *
 * NOTE that clients are encouraged to access this function via the wrapper ILINTxFrame::Create().
 *
 * \param[in]  id
 * \param[in]  data
 * \param[in]  dlc
 * \param[in]  crcType
 * \param[in]  flags
 * \param[out] ppLinTxFrame     A pointer to an object supporting ILINTxFrame. The object is owned by the OLL, not by the caller.
 *                              The OLL assumes that the caller will eventually pass the object pointer back to the OLL for
 *                              transmission. If this is not done, the object's memory will leak.
 *
 * \return A pointer to an interface based on IError, describing the error which occurred during this function. NULL if no error
 * occurred. See \ref ErrorReporting for more information on how errors are reported.
 */
OLL_API IError* OLI_CALL ILINTxFrame_Create1( uint8 id, const byte* data, uint8 dlc, LINCRCType crcType, uint16 flags, ILINTxFrame** ppLinTxFrame );

/**
 * This function instantiates an object supporting ILINTxFrame. See \ref BinaryCompatibility for an explanation of why it is needed.
 *
 * NOTE that clients are encouraged to access this function via the wrapper ILINTxFrame::Create().
 *
 * \param[in]  received         A RX LIN frame which will be used to populate the TX frame instance.
 * \param[in]  flags
 * \param[out] ppLinTxFrame     A pointer to an object supporting ILINTxFrame. The object is owned by the OLL, not by the caller.
 *                              The OLL assumes that the caller will eventually pass the object pointer back to the OLL for
 *                              transmission. If this is not done, the object's memory will leak.
 *
 * \return A pointer to an interface based on IError, describing the error which occurred during this function. NULL if no error
 * occurred. See \ref ErrorReporting for more information on how errors are reported.
 */
OLL_API IError* OLI_CALL ILINTxFrame_Create2( const ILINRxFrame* received, uint16 flags, ILINTxFrame** ppLinTxFrame );

/** The only frame type available on LIN.
* Interfacewise, it adds flags for extended ids and remote frames.
*/

OLI_INTERFACE ILINTxFrame : public ITxFrame
{
protected:

    /// Instances of this interface are owned by the OLL.

    virtual ~ILINTxFrame() OLI_NOTHROW {};

public:

    /// returned by @ref GetType

    enum {TYPE = LIN_TYPE_FRAME_BASE + 1};

    /// \name LIN-specific interface
	/// @{

    virtual uint16 OLI_CALL GetFlags() const OLI_NOTHROW = 0;
    virtual LINCRCType OLI_CALL GetCRCType() const OLI_NOTHROW = 0;

	/// @}

    /**
     * This is a helper method which wraps ILINTxFrame_Create1(): see \ref BinaryCompatibility and \ref ErrorReporting for an
     * explanation of why it is needed.
     */
    static ILINTxFrame* OLI_CALL Create 
        ( uint8         id
        , const byte*   data
        , uint8         dlc
        , LINCRCType    crcType
        , uint16        flags )
    {
        ILINTxFrame* pLinTxFrame = NULL;
        CheckAndThrow( ILINTxFrame_Create1( id, data, dlc, crcType, flags, &pLinTxFrame ) );
        return pLinTxFrame;
    }

    /**
     * This is a helper method which wraps ILINTxFrame_Create2(): see \ref BinaryCompatibility and \ref ErrorReporting for an
     * explanation of why it is needed.
     */
    static ILINTxFrame* OLI_CALL Create 
        ( const ILINRxFrame*    received
        , uint16                flags )
    {
        ILINTxFrame* pLinTxFrame = NULL;
        CheckAndThrow( ILINTxFrame_Create2( received, flags, &pLinTxFrame ) );
        return pLinTxFrame;
    }
};

/// @}

#include "../Common/EndNamespace.h"

#endif // !defined(__OLI_ILINTXFRAME_INCLUDED__)
