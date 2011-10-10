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
* @brief  ILINLink definition
* @remark The header structure of the OLI may change
*         in future releases. Don't include this
*         header directly. Use @ref OLI.h instead.
*/

/// @todo update doxygen comments

#if !defined(__OLI_ILINLINK_INCLUDED__)
#define __OLI_ILINLINK_INCLUDED__

// include used interface

#include "../Common/ILink.h"
#include "../Common/AutoPtr.h"
#include "../Common/ErrorInterfaces.h"
#include "../Common/ErrorProxies.h"

// open ETAS::OLI namespace

#include "../Common/BeginNamespace.h"

#ifdef _DOXYGEN
namespace ETAS {namespace OLI {
#endif

// forward declaration

class ILINTxFrame;

/// \addtogroup GROUP_OLI_LIN_LINKS

/// @{

class ILINLink;

/**
 * This function instantiates an object supporting ILINLink. See \ref BinaryCompatibility for an explanation of why it is needed.
 *
 * NOTE that clients are encouraged to access this function via the wrapper ILINLink::Create().
 *
 * \param[in]  URI          The URI identifying the instance which is to be created.
 * \param[in]  version      The OLI version which the client expects to use.
 * \param[out] ppLinLink    A pointer to an object supporting ILINLink. The object will already have a reference count of 1 and
 *                          must be reference-counted by the caller, using the object's methods AddRef() and Release(). This is
 *                          easily done by wrapping the object pointer in an instance of the AutoPtr class, which will be done
 *                          automatically if the caller accesses ILINLink_Create() via the wrapper ILINLink::Create().
 *
 * \return A pointer to an interface based on IError, describing the error which occurred during this function. NULL if no error
 * occurred. See \ref ErrorReporting for more information on how errors are reported.
 */
OLL_API IError* OLI_CALL ILINLink_Create( const char *URI, uint32 version, ILINLink** ppLinLink );

/** LIN-specific extension of ILink.
*/

OLI_INTERFACE ILINLink : public ILink
{
protected:

    /// Instances of this interface are owned by the OLL.

    virtual ~ILINLink() OLI_NOTHROW {};

    /**
     * This method creates a LIN master TX queue. The underlying controller must be the bus master. There can only be one master
     * queue on a single CLINLink. Repeated calls to CreateMasterQueue will merely return a pointer to the original queue.
     *
     * \param[out] ppTxQueue    The new master queue. This object is owned by the link.
     *
     * \return A pointer to an interface based on IError, describing the error which occurred during this function. NULL if no error
     * occurred. A pointer to IOCIError will be returned if the controller is not the bus master, or if OCI reported another problem.
     * See \ref ErrorReporting for more information on how errors are reported.
     */
    virtual IError* OLI_CALL CreateMasterQueue( ITxQueue** ppTxQueue ) OLI_NOTHROW = 0;

    /**
     * @todo kuh, 2008-08-26: docu missing
     *
     * \param[in] frame
     *
     * \return A pointer to an interface based on IError, describing the error which occurred during this function. NULL if no error
     * occurred. See \ref ErrorReporting for more information on how errors are reported.
     */
    virtual IError* OLI_CALL SetSlaveDataRaw( const ILINTxFrame* frame ) OLI_NOTHROW = 0;

public:

    /// \name LIN-specific interface
	/// @{

	/// Creates a LIN master tx queue
    /**
		The underlying controller must be the bus master.

		There can only be one master queue on a single CLINLink. Repeated calls
		to CreateMasterQueue will merely return a pointer to the original queue.

		\throws OCIException
			If the controller is not the bus master, or if OCI reported another problem.
    */

    /**
     * This is a helper method which wraps a corresponding protected method: see \ref ErrorReporting for an explanation of why it is needed.
     */
    ITxQueue* OLI_CALL CreateMasterQueue()
    {
        ITxQueue* pTxQueue = NULL;
        CheckAndThrow( CreateMasterQueue( &pTxQueue ) );
        return pTxQueue;
    }

    /**
     * This is a helper method which wraps a corresponding protected method: see \ref ErrorReporting for an explanation of why it is needed.
     */
    void OLI_CALL SetSlaveData (const ILINTxFrame* frame)
    {
        CheckAndThrow( SetSlaveDataRaw( frame ) );
    }

    /// @}

    /**
     * This is a helper method which wraps ILINLink_Create(): see \ref BinaryCompatibility and \ref ErrorReporting for an
     * explanation of why it is needed.
     */
    static AutoPtr<ILINLink> OLI_CALL 
    Create( const char *URI, uint32 version = VERSION )
    {
        ILINLink* pLinLink = NULL;
        CheckAndThrow( ILINLink_Create( URI, version, &pLinLink ) );

        // The wrapped method has already AddRef-ed the pointer, so we tell AutoPtr to take ownership of the pointer without a
        // further AddRef.
        return AutoPtr<ILINLink>( pLinLink, false );
    }
};

// close ETAS::OLI namespace

#ifdef _DOXYGEN
}}
#endif

#include "../Common/EndNamespace.h"

#endif // !defined(__OLI_ILINLINK_INCLUDED__)
