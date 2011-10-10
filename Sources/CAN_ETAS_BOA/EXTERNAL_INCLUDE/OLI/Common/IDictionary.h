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
* @brief  IDictionary definition
* @remark The header structure of the OLI may change
*         in future releases. Don't include this
*         header directly. Use @ref OLI.h instead.
*/

#if !defined(__OLI_IDICTIONARY_INCLUDED__)
#define __OLI_IDICTIONARY_INCLUDED__

#include "OLIDefs.h"
#include "ErrorInterfaces.h"
#include "ErrorProxies.h"

// open ETAS::OLI namespace

#include "BeginNamespace.h"

#ifdef _DOXYGEN
namespace ETAS {namespace OLI {
#endif

// interface forward declarations

class IDictionaryEntry;

/** @ingroup GROUP_OLI_COMMON_CONFIGURATION
* @brief  Base interface that represents a read-only container of
*         key/value entries.
*
*         The information is based on some OCI level data dictionary. 
*         Specialized interfaces will extend it with support for 
*         modification and other features.
*
*         Objects providing this interface are expected to be unique
*         members of a link object providing @ref ILink. Thus, it has
*         an (indirect) connection to the virtual controller instance.
*
* @remark All public methods are thread-safe.
* @remark Objects implementing this interface are gurateed to live as
*         long as the @ref ILink instance that returned them. 
*
* @since  BOA 1.3
* @see    IDictionaryEntry
*/

OLI_INTERFACE IDictionary
{
protected:

    /** @brief Destructor.

        This destructor has been hidden because the client cannot
        directly control the lifetime of this object.

        @exception <none> This function must not throw exceptions.

        @since  BOA 1.3 
     */
    virtual ~IDictionary() OLI_NOTHROW {};

    /** @brief  This method accesses the individual dictionary entries 
                (key/value pairs). 
        @anchor GetStatusRaw
     
        @param [in]  index        
                Index of the entry to get. 
                The valid range is 0 .. @ref GetCount - 1.
        @param [out] ppDictEntry  
                The specifies dictionary entry; the ownership of this 
                object remains with the dictionary.
        @return A pointer to an interface based on @ref IError, describing 
                the error which occurred during this function. @c NULL if 
                no error occurred. See @ref ErrorReporting "error reporting"
                for more information on how errors are reported.
       
        @exception <none> This function must not throw exceptions.
       
        @since  BOA 1.3 
        @see    @ref BinaryCompatibility "binary compatibility", 
                @ref ErrorReporting "error reporting",
                IDictionaryEntry
     */
    virtual IError* OLI_CALL GetEntry( 
        uint32 index, 
        const IDictionaryEntry** ppDictEntry ) const OLI_NOTHROW = 0;

    /** @brief  This method provides case-sensitive parameter lookup. 
        @anchor FindEntryRaw

        @param [in]  name        
                Fully qualified name of the entry to find.
        @param [out] ppDictEntry  
                The specifies dictionary entry; the ownership of this 
                object remains with the dictionary. The value is @a NULL 
                if the specified entry does not exist.
        @return A pointer to an interface based on @ref IError, describing 
                the error which occurred during this function. @c NULL if 
                no error occurred. See @ref ErrorReporting "error reporting"
                for more information on how errors are reported.
       
        @exception <none> This function must not throw exceptions.
       
        @since  BOA 1.3 
        @see    @ref BinaryCompatibility "binary compatibility", 
                @ref ErrorReporting "error reporting",
                IDictionaryEntry
     */
    virtual IError* OLI_CALL FindEntry( 
        const char* name, 
        const IDictionaryEntry** ppDictEntry ) const OLI_NOTHROW = 0;

    /** @brief  Write the current dictionary content to a file. 
     
        @param [in]  fileName        
                The name of the new configuration file. An existing file 
                will be overwritten without further notice.
        @param [in]  mimeType  
                The MIME type to use for the specified file. 
                Currently only "application/json" is supported.
        @return A pointer to an interface based on @ref IError, describing 
                the error which occurred during this function. @c NULL if 
                no error occurred. A pointer to @ref INotSupportedException 
                will be returned if the specified MIME type cannot be 
                handled. See @ref ErrorReporting "error reporting" for more 
                information on how errors are reported.
       
        @exception <none> This function must not throw exceptions.
       
        @since  BOA 1.3 
        @see    @ref BinaryCompatibility "binary compatibility", 
                @ref ErrorReporting "error reporting",
                IDictionaryEntry
     */
	virtual IError* OLI_CALL WriteToFileRaw( 
        const char* fileName, 
        const char* mimeType ) OLI_NOTHROW = 0;

public:

    /** @brief  Get the number of entries in this dictionary.

                This tells the client application which @a index values
                are valid in @ref GetEntry.

                Some dictionaries require to be filled explicitly and will
                report 0 entries until that happened.

        @return The number of entries in this dictionary.
       
        @exception <none> This function must not throw exceptions.
       
        @since  BOA 1.3 
        @see    GetEntry
     */
    virtual uint32 OLI_CALL GetCount() const OLI_NOTHROW = 0;

    /** @brief  This method accesses the individual dictionary entries 
                (key/value pairs). 
     
        @param [in]  index        
                Index of the entry to get. 
                The valid range is 0 .. @ref GetCount - 1.
        @return The @ref IDictionaryEntry instance; the ownership of this 
                object remains with the dictionary. The instance will
                remain valid for as long as this @ref IDictionary is valid.

        @exception CError This function may throw an exception
                derived from @ref CError.
       
        @remark This is a helper method which wraps the internal @ref 
                GetStatusRaw "GetStatus" function. 
                See @ref BinaryCompatibility "binary compatibility" 
                and @ref ErrorReporting "error reporting"
                for an explanation of why it is needed.
        @since  BOA 1.3 
        @see    IDictionaryEntry
     */
    const IDictionaryEntry* OLI_CALL GetEntry (uint32 index) const
    {
        const IDictionaryEntry* pDictEntry = NULL;
        CheckAndThrow( GetEntry( index, &pDictEntry ) );
        return pDictEntry;
    }

    /** @brief  This method provides case-sensitive parameter lookup. 
     
        @param [in]  name        
                Fully qualified name of the entry to find.
        @return The @ref IDictionaryEntry instance; the ownership of this 
                object remains with the dictionary. @a NULL is returned
                if the specified entry does not exist. The instance will
                remain valid for as long as this @ref IDictionary is valid.

        @exception CError This function may throw an exception
                derived from @ref CError.
       
        @remark This is a helper method which wraps the internal @ref 
                FindEntryRaw "FindEntry" function. 
                See @ref BinaryCompatibility "binary compatibility" 
                and @ref ErrorReporting "error reporting"
                for an explanation of why it is needed.
        @since  BOA 1.3 
        @see    IDictionaryEntry
     */
    const IDictionaryEntry* OLI_CALL FindEntry (const char* name) const
    {
        const IDictionaryEntry* pDictEntry = NULL;
        CheckAndThrow( FindEntry( name, &pDictEntry ) );
        return pDictEntry;
    }

    /** @brief  Write the current dictionary content to a file. 
     
        @param [in]  fileName        
                The name of the new configuration file. An existing file 
                will be overwritten without further notice.
        @param [in]  mimeType  
                The MIME type to use for the specified file. 
                Currently only "application/json" is supported.
       
        @exception CNotSupportedException
                The specified MIME type cannot be handled.
        @exception CError 
                This function may throw other exceptions derived from 
                @ref CError.
       
        @remark This is a helper method which wraps the internal @ref 
                WriteToFileRaw function. 
                See @ref BinaryCompatibility "binary compatibility" 
                and @ref ErrorReporting "error reporting"
                for an explanation of why it is needed.
        @since  BOA 1.3 
     */
    void OLI_CALL WriteToFile( const char* fileName, const char* mimeType)
    {
        CheckAndThrow( WriteToFileRaw( fileName, mimeType ) );
    }
};

// close ETAS::OLI namespace

#ifdef _DOXYGEN
}}
#endif

#include "../Common/EndNamespace.h"

#endif // !defined(__OLI_IDICTIONARY_INCLUDED__)
