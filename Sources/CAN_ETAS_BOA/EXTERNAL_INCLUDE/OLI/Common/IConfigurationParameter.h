///////////////////////////////////////////////////////////
//  IConfigurationParameter.h
//  Implementation of the Interface IConfigurationParameter
//  Created on:      19-Mrz-2008 01:23:28
///////////////////////////////////////////////////////////

/// @todo update doxygen comments

#if !defined(__OLI_ICONFIGURATIONPARAMETER_INCLUDED__)
#define __OLI_ICONFIGURATIONPARAMETER_INCLUDED__

#include "IDictionaryEntry.h"
#include "ErrorInterfaces.h"
#include "ErrorProxies.h"

#include "BeginNamespace.h"

/// @ingroup GROUP_OLI_COMMON_CONFIGURATION

/**
    Extends \ref IDictionaryEntry by methods to write parameter values.
    It also provides information on whether the parameter is optional etc.
*/

OLI_INTERFACE IConfigurationParameter : public IDictionaryEntry
{
protected:

    /// Instances of this interface are owned by the OLL and cannot
    /// be destroyed by the client application.

    virtual ~IConfigurationParameter() OLI_NOTHROW {};

    /// set value. Triggers observers (see @ref IConfigurationObserver).
    /// Must not be called if @ref IsWritable returns @a false.
    /// Throws an exception if the value does not match the type or
    /// if there was a failure within some observer.
    /// The value of parameters whose type is \ref RECORD, \ref ARRAY
    /// and \ref UNION cannot be set using this method.
    /// If the parameter is of a type other than \ref STRING, at
    /// least \ref GetSize() bytes must be readable from \a newVal on.
    /// If the parameter is of type STRING, it is sufficient for
    /// \a newVal to be null-terminated.

    /**
     * This method sets the values of the parameter. It triggers observers (see \ref IConfigurationObserver).
     * It must not be called if \ref IsWritable() returns \a false. The value of a parameter whose type is \ref RECORD,
     * \ref ARRAY or \ref UNION cannot be set using this method. If the parameter is of a type other than \ref STRING, at
     * least \ref GetSize() bytes must be readable from \a newVal on. If the parameter is of type STRING, it is sufficient
     * for \a newVal to be null-terminated.
     *
     * \param[in] newVal    The new value.
     *
     * \return A pointer to an interface based on IError, describing the error which occurred during this function. NULL if
     * no error occurred. See \ref ErrorReporting for more information on how errors are reported.
     */
    virtual IError* OLI_CALL SetValueRaw( const char* newVal ) = 0;

public:

    /**
     * This is a helper method which wraps a corresponding protected method: \ref ErrorReporting for an
     * explanation of why it is needed.
     */
    void OLI_CALL SetValue (const char *newVal)
    {
        CheckAndThrow( SetValueRaw( newVal ) );
    }

    /// Returns the parameter that determines the variable array size.
    /// Will be NULL, if this not an array or if the array has a fixed size.

    virtual IConfigurationParameter* OLI_CALL GetSizeParameter() const OLI_NOTHROW = 0; 

    /// Returns the parameter that determines the index of a tagged union.
    /// Will be NULL, if this not a tagged union.

    virtual IConfigurationParameter* OLI_CALL GetStructureSelector() const OLI_NOTHROW = 0; 

    /// Some parameters may not be readable, e.g. those corresponding
    /// to write-only hardware registers.

    virtual bool OLI_CALL IsReadable() const OLI_NOTHROW = 0;

    /// some parameters may be read-only. The result of this fuction
    /// may depend on other parameter values.

    virtual bool OLI_CALL IsWritable() const OLI_NOTHROW = 0;

    /// if this returns @a false , the value must be set before activating
    /// the new configuration (see @ref ILink::SetActive).

    virtual bool OLI_CALL IsOptional() const OLI_NOTHROW = 0;

    /// if this returns @a true, the value has not been set by an application.

    virtual bool OLI_CALL IsDefault() const OLI_NOTHROW = 0;

    /// if this returns @a false, the value must be set, if \ref IsDefault
    /// still returns @a true.

    virtual bool OLI_CALL DefaultIsValid() const OLI_NOTHROW = 0;
};

#include "EndNamespace.h"

#endif // !defined(__OLI_ICONFIGURATIONPARAMETER_INCLUDED__)
