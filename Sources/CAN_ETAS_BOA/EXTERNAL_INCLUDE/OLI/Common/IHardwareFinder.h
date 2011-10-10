/*
* Basic Open API - Open Link Interface
* Version 1.3
*
* Copyright (c) ETAS GmbH. All rights reserved.
*
* $Revision: 4412 $
*/

/** 
* @file
* @brief  IHardwareFinder definition
* @remark The header structure of the OLI may change
*         in future releases. Don't include this
*         header directly. Use @ref OLI.h instead.
*/

/// @todo update doxygen comments

#if !defined(__OLI_IHARDWAREFINDER_INCLUDED__)
#define __OLI_IHARDWAREFINDER_INCLUDED__

#include "IRefCountable.h"
#include "ErrorInterfaces.h"
#include "ErrorProxies.h"

#include "AutoPtr.h"

// open ETAS::OLI namespace

#include "BeginNamespace.h"

#ifdef _DOXYGEN
namespace ETAS {namespace OLI {
#endif

// interface forward declaration

class IHardwareFinder;

/**
 * This function instantiates an object supporting IHardwareFinder. See \ref BinaryCompatibility for an explanation of why it is needed.
 *
 * NOTE that clients are encouraged to access this function via the wrapper IHardwareFinder::Create().
 *
 * \param[out] ppHwFinder   A pointer to an object supporting IHardwareFinder. The object will already have a reference count of 1 and
 *                          must be reference-counted by the caller, using the object's methods AddRef() and Release(). This is easily
 *                          done by wrapping the object pointer in an instance of the AutoPtr class, which will be done automatically
 *                          if the caller accesses IHardwareFinder_Create() via the wrapper IHardwareFinder::Create().
 *
 * \return A pointer to an interface based on IError, describing the error which occurred during this function. NULL if no error
 * occurred. See \ref ErrorReporting for more information on how errors are reported.
 */

OLL_API IError* OLI_CALL IHardwareFinder_Create( IHardwareFinder** ppHwFinder );

/// @ingroup GROUP_OLI_INTERFACES_UTILITIES

/**
*/

OLI_INTERFACE IHardwareFinder : public IRefCountable
{
protected:

    /// Topmost interface requires a virtual destructor.

    virtual ~IHardwareFinder() OLI_NOTHROW {};

    /// access to the individual URIs

    /**
     * This method provides access to the individual URIs.
     *
     * \param[in]  index
     * \param[out] ppUri
     *
     * \return A pointer to an interface based on IError, describing the error which occurred during this function. NULL
     * if no error occurred. See \ref ErrorReporting for more information on how errors are reported.
     */
	virtual IError* OLI_CALL GetURI( uint32 index, const char** ppUri ) const OLI_NOTHROW = 0;

public:

    /// different controller types to search for

    enum ControllerType
    {
        CAN = 1,
        LIN = 2,
        FlexRay = 4
    };

    /// crawl the hardware tree and collect all URIs that match
    /// the requested controller \ref type.

    virtual void OLI_CALL SearchFor (enum ControllerType type) OLI_NOTHROW = 0;

    /// number of URIs found

	virtual uint32 OLI_CALL GetCount() const OLI_NOTHROW = 0;

    /**
     * This is a helper method which wraps a corresponding protected method: see \ref ErrorReporting for an explanation of why it is needed.
     */
	const char* OLI_CALL GetURI (uint32 index) const
    {
        const char* pUri = NULL;
        CheckAndThrow( GetURI( index, &pUri ) );
        return pUri;
    }

    /**
     * This is a helper method which wraps IHardwareFinder_Create(): see \ref BinaryCompatibility and \ref ErrorReporting for an
     * explanation of why it is needed.
     */
    static AutoPtr<IHardwareFinder> OLI_CALL Create()
    {
        IHardwareFinder* pHwFinder = NULL;
        CheckAndThrow( IHardwareFinder_Create( &pHwFinder ) );

        // The wrapped method has already AddRef-ed the pointer, so we tell AutoPtr to take ownership of the pointer without a
        // further AddRef.
        return AutoPtr<IHardwareFinder>( pHwFinder, false );
    }
};

// close ETAS::OLI namespace

#ifdef _DOXYGEN
}}
#endif

#include "EndNamespace.h"

#endif // !defined(__OLI_IHARDWAREFINDER_INCLUDED__)
