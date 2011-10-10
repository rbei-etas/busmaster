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
* @brief  IDictionaryEntry definition
* @remark The header structure of the OLI may change
*         in future releases. Don't include this
*         header directly. Use @ref OLI.h instead.
*/

#if !defined(__OLI_IDICTIONARYENTRY_INCLUDED__)
#define __OLI_IDICTIONARYENTRY_INCLUDED__

#include "OLIDefs.h"
#include "OLITypes.h"

// open ETAS::OLI namespace

#include "BeginNamespace.h"

#ifdef _DOXYGEN
namespace ETAS {namespace OLI {
#endif

/** @ingroup GROUP_OLI_COMMON_CONFIGURATION
* @brief  Access to a single @ref IDictionary entry. 
*
*         The @a name of the entry will be constructed from hierarchical 
*         information to form a flat namespace, e.g. 'buffer[123].offset'.
*
* @remark All public methods are thread-safe.
* @remark Objects implementing this interface are gurateed to live as
*         long as the @ref IDictionary instance that returned them. 
*
* @since  BOA 1.3
* @see    IDictionary
*/

OLI_INTERFACE IDictionaryEntry
{
protected:

    /** @brief Destructor.

        This destructor has been hidden because the client cannot
        directly control the lifetime of this object.

        @exception <none> This function must not throw exceptions.

        @since  BOA 1.3 
     */
    virtual ~IDictionaryEntry() OLI_NOTHROW {};

public:

    /** Available entry types. Returned by @ref GetType.
     */
    enum Type
    {
        /** an unsigned integer */
        UNSIGNED = 1,

        /** an signed integer */
        SIGNED = 2,

        /** a float value */
        FLOAT = 3,

        /** a 0-terminated UTF8 string */
        STRING = 4,

        /** a binary string */
        BINARY = 5,

        /** a structure consisting of several sub-entries */
        RECORD = 6,

        /** a fixed or variable sized array of elements of the same type */
        ARRAY = 7,

        /** a union of several sub-entries */
        UNION = 8
    };

    /** @brief  This method returns the fully qualified name of the entry.

                The name of sub-entries in structures or unions consists
                of the name of the parent entry followed by a dot and the
                sub-entry identifier. Array indices will be given inside
                squared brackets.

        @return The fully qualified name of the entry.
       
        @exception <none> This function must not throw exceptions.

        @since  BOA 1.3 
     */
    virtual const char* OLI_CALL GetName() const OLI_NOTHROW = 0;

    /** @brief  Get the entry type.

                The name of sub-entries in structures or unions consists
                of the name of the parent entry followed by a dot and the
                sub-entry identifier. Array indices will be given inside
                squared brackets.

        @return The entry type.
       
        @exception <none> This function must not throw exceptions.

        @remark Please note that the exact type will only be known in
                combination with @ref GetSize "size" of the entry.
        @since  BOA 1.3 
        @see    GetSize
     */
    virtual Type OLI_CALL GetType() const OLI_NOTHROW = 0;

    /** @brief  Get the size of entry.

                The actual meaning of the size depends on the @ref 
                GetType "type".

        @return The size of entry. For unsigned, signed and float entries,
                this is thier length in bytes. Strings and binary sequences
                specify the maximum length of the respective data buffer.
                Strings of unbound length identify as -1. Records and
                unions specify the number of members, arrays the number
                of elements.
       
        @exception <none> This function must not throw exceptions.

        @since  BOA 1.3 
        @see    GetType
        @todo   check def of unbound string
                check union selector
                check array length selector
     */
    virtual uint32 OLI_CALL GetSize() const OLI_NOTHROW = 0;

    /** @brief  Get the current value.

                The interpretation of the string depends on the @ref 
                GetType "type".

        @return The current value of the entry as either a 0-terminated
                UTF8 string or a binary string of the @ref GetLength bytes. 
                The latter is only valid for @a BINARY entries.
       
        @exception <none> This function must not throw exceptions.

        @since  BOA 1.3 
        @see    GetType, GetLength
     */
    virtual const char* OLI_CALL GetValue() const OLI_NOTHROW = 0;
};

// close ETAS::OLI namespace

#ifdef _DOXYGEN
}}
#endif

#include "../Common/EndNamespace.h"

#endif // !defined(__OLI_IDICTIONARYENTRY_INCLUDED__)
