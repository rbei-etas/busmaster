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
* @brief  IConfigurationObserver definition
* @remark The header structure of the OLI may change
*         in future releases. Don't include this
*         header directly. Use @ref OLI.h instead.
*/

/// @todo update doxygen comments

#if !defined(__OLI_ICONFIGURATIONOBSERVER_INCLUDED__)
#define __OLI_ICONFIGURATIONOBSERVER_INCLUDED__

#include "OLIDefs.h"

// open ETAS::OLI namespace

#include "BeginNamespace.h"

#ifdef _DOXYGEN
namespace ETAS {namespace OLI {
#endif

// interface forward declarations

class IConfigurationContainer;

/// @ingroup GROUP_OLI_COMMON_CONFIGURATION

/** Receives a notification whenever a parameter changes
* within the container that this instance registered on.
* A given observer may be registered on multiple 
* @ref IConfigurationContainer instances.
*
* To receive notifications for a single parameter only,
* @ref GetParameter must return the name of that parameter.
* Otherwise, it must return NULL.
*/

OLI_INTERFACE IConfigurationObserver
{
public:

    /// topmost interface requires a virtual destructor

    virtual ~IConfigurationObserver() OLI_NOTHROW {};

    /**
		The @a parameter with the given name has changed within the 
		@a container. You may modify other any parameter within the
		@a container, but you must not throw an exception.

		This method will be called within the synchronization context
		of the link. It is recommended that you do not call methods
		on other links that require synchronization to avoid circular
		lock dependencies (and hence deadlocks). If your handler needs
		to do so, it should use a deferred processing routine that executes
		outside of the synchronization context of the link owning @a
		container.
	*/
    virtual void OLI_CALL OnChange (IConfigurationContainer* container, const char *parameter) OLI_NOTHROW = 0;

    /// Returns the name of the only parameter to observe or NULL if the observer
    /// listens to all parameter changes.
    ///
    /// The return value must not change over the lifetime of the instance.

    virtual const char *OLI_CALL GetParameter() const  OLI_NOTHROW = 0;

};

#ifdef _DOXYGEN
}}
#endif

#include "EndNamespace.h"

#endif // !defined(__OLI_ICONFIGURATIONOBSERVER_INCLUDED__)
