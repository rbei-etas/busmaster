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
* @brief  IConfigurationParameter definition
* @remark The header structure of the OLI may change
*         in future releases. Don't include this
*         header directly. Use @ref OLI.h instead.
*/

#if !defined(__OLI_ICONFIGURATIONPARAMETER_INCLUDED__)
#define __OLI_ICONFIGURATIONPARAMETER_INCLUDED__

#include "IDictionaryEntry.h"
#include "ErrorInterfaces.h"
#include "ErrorProxies.h"

// open ETAS::OLI namespace

#include "BeginNamespace.h"

#ifdef _DOXYGEN
namespace ETAS {namespace OLI {
#endif

/** @ingroup GROUP_OLI_COMMON_CONFIGURATION
* @brief  Access to a single @ref IConfigurationContainer entry. 
*
*         This interface extends @ref IDictionaryEntry by @ref SetValue
*         to modify this parameter. The other methods of this interface
*         describe whether the parameter value can or should be modified.
*
* @remark All public methods are thread-safe.
* @remark Objects implementing this interface are gurateed to live as
*         long as the @ref IConfigurationContainer instance that returned
*         them. 
*
* @since  BOA 1.3
* @see    IDictionaryEntry, IConfigurationContainer, IDictionary
*/
OLI_INTERFACE IConfigurationParameter : public IDictionaryEntry
{
protected:

    /** @brief Destructor.

        This destructor has been hidden because the client cannot
        directly control the lifetime of this object.

        @exception <none> This function must not throw exceptions.

        @since  BOA 1.3 
     */
    virtual ~IConfigurationParameter() OLI_NOTHROW {};

    /** @brief  Set the value of the parameter. 
     
                It triggers observers (see @ref IConfigurationObserver).
                It must not be called if @ref IsWritable() returns @c false.
                The value of a parameter whose @ref IDictionaryEntry::GetType
                "type" is @ref IDictionaryEntry::RECORD "RECORD", 
                @ref IDictionaryEntry::ARRAY "ARRAY" or
                @ref IDictionaryEntry::UNION "UNION" cannot be set using this
                method.
                
        @param  [in]  newVal        
                The new value as null-terminated string.
                If the parameter is of a type other than @ref 
                IDictionaryEntry::STRING "STRING", @a newVal will be
                parsed into the respective type.

        @return A pointer to an interface based on @ref IError, describing 
                the error which occurred during this function. @c NULL if 
                no error occurred. See @ref ErrorReporting "error reporting"
                for more information on how errors are reported.
       
        @exception <none> This function must not throw exceptions.
       
        @since  BOA 1.3 
        @see    @ref SetValue,
                @ref BinaryCompatibility "binary compatibility", 
                @ref ErrorReporting "error reporting",
                @ref IConfigurationObserver,
                @ref IDictionaryEntry::GetType
     */
    virtual IError* OLI_CALL SetValueRaw( const char* newVal ) = 0;

public:

    /** @brief  Set the value of the parameter. 
     
                It triggers observers (see @ref IConfigurationObserver).
                It must not be called if @ref IsWritable() returns @c false.
                The value of a parameter whose @ref IDictionaryEntry::GetType
                "type" is @ref IDictionaryEntry::RECORD "RECORD", 
                @ref IDictionaryEntry::ARRAY "ARRAY" or
                @ref IDictionaryEntry::UNION "UNION" cannot be set using this
                method.
                
        @param  [in]  newVal        
                The new value as null-terminated string.
                If the parameter is of a type other than @ref 
                IDictionaryEntry::STRING "STRING", @a newVal will be
                parsed into the respective type.

        @exception CError This function may throw an exception
                derived from @ref CError.
       
        @remark This is a helper method which wraps the internal @ref 
                SetValueRaw function. 
                See @ref BinaryCompatibility "binary compatibility" 
                and @ref ErrorReporting "error reporting"
                for an explanation of why it is needed.
        @since  BOA 1.3 
        @see    SetValueRaw
     */
    void OLI_CALL SetValue (const char *newVal)
    {
        CheckAndThrow( SetValueRaw( newVal ) );
    }

    /** @brief  Get the size parameter for this array.

                If this parameter is a variable size array, this method
                will return the parameter that contains the actual size
                of that array, i.e. the number of valid array entries.

        @return Parameter that contains the actual array size.
                @c NULL, if this is not an array of variable size.
       
        @exception <none> This function must not throw exceptions.

        @since  BOA 1.3 
        @see    @ref IDictionaryEntry::GetType,
                @ref IDictionaryEntry::Type,
                @ref IDictionaryEntry::ARRAY
     */
    virtual IConfigurationParameter* OLI_CALL 
        GetSizeParameter() const OLI_NOTHROW = 0; 

    /** @brief  Get the tag parameter for this union.

                If this parameter is a tagged union, this method will
                return the parameter that contains the tag (selector)
                that determines which sub-structure of this union is valid.

        @return Parameter that contains the tag for this tagged union.
                @c NULL, if this is not a tagged union.
       
        @exception <none> This function must not throw exceptions.

        @since  BOA 1.3 
        @see    @ref IDictionaryEntry::GetType "GetValue",
                @ref IDictionaryEntry::Type "Type",
                @ref IDictionaryEntry::ARRAY "ARRAY"
     */
    virtual IConfigurationParameter* OLI_CALL 
        GetStructureSelector() const OLI_NOTHROW = 0; 

    /** @brief  Test whether this parameter can be read.

                Some parameters may not be readable, e.g. those 
                corresponding to write-only hardware registers.
                For these parameters, we will only see the values
                that have been written by the client application
                to the respective container.

        @return If true, @ref GetValue may be called before
                @ref SetValue for this parameter.
       
        @exception <none> This function must not throw exceptions.

        @since  BOA 1.3 
        @see    @ref IDictionaryEntry::GetValue "GetValue",
                @ref SetValue
     */
    virtual bool OLI_CALL IsReadable() const OLI_NOTHROW = 0;

    /** @brief  Test whether this parameter can be modified.

        @return If @c true, @ref SetValue may be called for this parameter.
                Otherwise, @ref SetValue will throw a @ref 
                CInvalidOperationException exception
       
        @exception <none> This function must not throw exceptions.

        @remark The result of this fuction may depend on other parameter
                values.
        @since  BOA 1.3 
        @see    @ref SetValue, @ref CInvalidOperationException
     */
    virtual bool OLI_CALL IsWritable() const OLI_NOTHROW = 0;

    /** @brief  Test whether this parameter is optional.

        @return @c true, if the user does not need to specify this
                parameter at all.
                @c false, if the parameter must be given. In that case,
                it may either be the default value (see @ref IsDefault 
                and @ref DefaultIsValid) or must be set explicitly via
                @ref SetValue before activating the new configuration 
                can be activated (see @ref ILink::SetActive).
       
        @exception <none> This function must not throw exceptions.

        @remark Once a valid configuration has been read from the hardware
                (@ref IConfigurationContainer::Read) or from a file
                (@ref IConfigurationContainer::ReadFromFile), no parameter
                needs to be modified. Use @ref IsDefault to test whether
                this parameter needs to be set, yet.
        @since  BOA 1.3 
        @see    @ref SetValue, 
                @ref IsDefault,
                @ref IsDefaultValid,
                @ref ILink::SetActive,
                @ref IConfigurationContainer::Read,
                @ref IConfigurationContainer::ReadFromFile
     */
    virtual bool OLI_CALL IsOptional() const OLI_NOTHROW = 0;

    /** @brief  Test whether this parameter is at default.

        @return @c true, the parameter value has not been set, yet.
                Otherwise, it may still have the default value but
                that value has been set explicitly.
       
        @exception <none> This function must not throw exceptions.

        @remark Once a valid configuration has been read from the hardware
                (@ref IConfigurationContainer::Read) or from a file
                (@ref IConfigurationContainer::ReadFromFile), this
                function will always return @a false.
        @since  BOA 1.3 
        @see    @ref SetValue, 
                @ref IsOptional,
                @ref IConfigurationContainer::Read,
                @ref IConfigurationContainer::ReadFromFile
     */
    virtual bool OLI_CALL IsDefault() const OLI_NOTHROW = 0;

    /** @brief  Test whether the default is valid for this parameter.

        @return @c true, the default parameter value (see @ref IsDefault)
                does not need to be overwritten using @ref SetValue.
                Otherwise, the default parameter value is usually
                not a valid configuration.
       
        @exception <none> This function must not throw exceptions.

        @since  BOA 1.3 
        @see    @ref SetValue, 
                @ref IsOptional
     */
    virtual bool OLI_CALL DefaultIsValid() const OLI_NOTHROW = 0;
};

// close ETAS::OLI namespace

#ifdef _DOXYGEN
}}
#endif

#include "../Common/EndNamespace.h"

#endif // !defined(__OLI_ICONFIGURATIONPARAMETER_INCLUDED__)
