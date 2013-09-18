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
* @brief  IConfigurationContainer definition
* @remark The header structure of the OLI may change
*         in future releases. Don't include this
*         header directly. Use @ref OLI.h instead.
*/

/// @todo update doxygen comments

#if !defined(__OLI_ICONFIGURATIONCONTAINER_INCLUDED__)
#define __OLI_ICONFIGURATIONCONTAINER_INCLUDED__

#include "OLITypes.h"
#include "IDictionary.h"
#include "ErrorInterfaces.h"
#include "ErrorProxies.h"

// open ETAS::OLI namespace

#include "BeginNamespace.h"

#ifdef _DOXYGEN
namespace ETAS {namespace OLI {
#endif

// interface forward declarations

class IConfigurationObserver;
class IConfigurationParameter;

/// @ingroup GROUP_OLI_COMMON_CONFIGURATION

/** Represents the controller configuration as a flat list of 
* @ref IConfigurationParameter instances.
*
* Objects providing this interface are expected to be unique
* members of a link object providing @ref ILink. Thus, it has
* an (indirect) connection to the actual controller and cannot
* be instantiated by OLI users.
*
* The class implementing @ref IConfigurationContainer is also
* expected to store the parameter definitions (OCI data dictionary)
* values in some internal structure. Initially, this structure
* is empty and must be filled by calling @ref Read. Once the
* user completed the configuration, it will be applied automatically
* when the respective link is activated (see \ref ILink::SetActive).
*
* Virtual parameters (like file names) are allowed. Use observers
* (see @ref IConfigurationObserver) to ensure consistency and to
* map virtual parameters onto 'real' ones. Observers are called 
* nestedly (if they modify other parameters) and in the same 
* sequence they have been added.
*
* If modifying a parameter value fails with a (possibly
* indirect) exception, the chang(es) must be undone automatically
* and no further notifications must be sent. This includes
* nested changes that have completed successfully.
*/

OLI_INTERFACE IConfigurationContainer : public IDictionary
{
protected:

    /// Instances of this interface are owned by the OLL and cannot
    /// be destroyed by the client application.

    virtual ~IConfigurationContainer() OLI_NOTHROW {};

    /// access to the individual parameters. 
    /// Valid @a index range is 0 .. GetCount-1.
    /// The ownership over the @ref IConfigurationParameter instances 
    /// remains with the container object.
    /// Throws an exception when @a index is out of range.

    /**
     * This method allows access to the individual parameters.
     *
     * \param[in]  index    The valid range is 0 .. GetCount - 1
     * \param[out] ppParam  The specified parameter; this object is owned by the configuration container.
     *
     * \return A pointer to an interface based on IError, describing the error which occurred during this function, or NULL
     * if no error occurred. A pointer to IOutOfRangeError will be returned if \a index is out of range. See
     * \ref ErrorReporting for more information on how errors are reported.
     */
    virtual IError* OLI_CALL GetParameter( uint32 index, IConfigurationParameter** ppParam ) OLI_NOTHROW = 0;

    /// Case-sensitive parameter lookup.
    /// Returns NULL, if there is no such parameter.
    /// The ownership over the @ref IConfigurationParameter instances 
    /// remains with the container object.

    /**
     * This method provides case-sensitive parameter lookup.
     *
     * \param[in]  name
     * \param[out] ppParam  The specified parameter; this object is owned by the configuration container. It will be NULL if
     *                      no parameter exists with the specified name.
     *
     * \return A pointer to an interface based on IError, describing the error which occurred during this function, or NULL
     * if no error occurred. See \ref ErrorReporting for more information on how errors are reported.
     */
    virtual IError* OLI_CALL FindParameter( const char* name, IConfigurationParameter** ppParam ) OLI_NOTHROW = 0;

    /**
     * This method registers an observer. It may be called at any time, even while bserver callbacks are in progres.
     *
     * \param[in] observer  The observer object. This remains owned by the caller.
     *
     * \return A pointer to an interface based on IError, describing the error which occurred during this function, or NULL
     * if no error occurred. See \ref ErrorReporting for more information on how errors are reported.
     */
    virtual IError* OLI_CALL AddObserverRaw( IConfigurationObserver* pObserver ) OLI_NOTHROW = 0;

    /**
     * This method removes a registered observer. It may be called at any time, even while bserver callbacks are in progres.
     *
     * \param[in] observer  The observer object.
     *
     * \return A pointer to an interface based on IError, describing the error which occurred during this function, or NULL
     * if no error occurred. See \ref ErrorReporting for more information on how errors are reported.
     */
    virtual IError* OLI_CALL RemoveObserverRaw( IConfigurationObserver* pObserver ) OLI_NOTHROW = 0;

    /**
     * This method reads data dictionary and current configuration set from the OCI layer. It may be called more than once.
     * All existing  settings (including virtual parameters) will be lost.
     *
     * \return A pointer to an interface based on IError, describing the error which occurred during this function, or NULL
     * if no error occurred. See \ref ErrorReporting for more information on how errors are reported.
     */
    virtual IError* OLI_CALL ReadRaw() OLI_NOTHROW = 0;

    /**
     * This method reads offline configuration from a file.
     *
     * \param[in] fileName  The name of the configuration file
     * \param[in] mimeType  The MIME type of the specified file. Currently this must be "application/json".
     *
     * \return A pointer to an interface based on IError, describing the error which occurred during this function, or NULL
     * if no error occurred. A pointer to INotSupportedError will be returned if the specified MIME type cannot be handled.
     * See \ref ErrorReporting for more information on how errors are reported.
     */
    virtual IError* OLI_CALL ReadFromFileRaw( const char* fileName, const char* mimeType ) OLI_NOTHROW = 0;

public:

    /**
     * This is a helper method which wraps a corresponding protected method: see  \ref ErrorReporting for an
     * explanation of why it is needed.
     */
    IConfigurationParameter* OLI_CALL GetParameter( uint32 index )
    {
        IConfigurationParameter* pParam = NULL;
        CheckAndThrow( GetParameter( index, &pParam ) );
        return pParam;
    }

    /**
     * This is a helper method which wraps a corresponding protected method: see \ref ErrorReporting for an
     * explanation of why it is needed.
     */
    IConfigurationParameter* OLI_CALL FindParameter( const char* name )
    {
        IConfigurationParameter* pParam = NULL;
        CheckAndThrow( FindParameter( name, &pParam ) );
        return pParam;
    }

    /**
     * This is a helper method which wraps a corresponding protected method: see \ref ErrorReporting for an
     * explanation of why it is needed.
     */
    void OLI_CALL AddObserver (IConfigurationObserver* observer)
    {
        CheckAndThrow( AddObserverRaw( observer ) );
    }

    /**
     * This is a helper method which wraps a corresponding protected method: see \ref ErrorReporting for an
     * explanation of why it is needed.
     */
    void OLI_CALL RemoveObserver (IConfigurationObserver* observer)
    {
        CheckAndThrow( RemoveObserverRaw( observer ) );
    }

    /**
     * This is a helper method which wraps a corresponding protected method: see \ref ErrorReporting for an
     * explanation of why it is needed.
     */
    void OLI_CALL Read()
    {
        CheckAndThrow( ReadRaw() );
    }

    /**
     * This is a helper method which wraps a corresponding protected method: see \ref ErrorReporting for an
     * explanation of why it is needed.
     */
    void OLI_CALL ReadFromFile( const char* fileName, const char* mimeType )
    {
        CheckAndThrow( ReadFromFileRaw( fileName, mimeType ) );
    }
};

#ifdef _DOXYGEN
}}
#endif

#include "../Common/EndNamespace.h"

#endif // !defined(__OLI_ICONFIGURATIONCONTAINER_INCLUDED__)
