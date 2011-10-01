///////////////////////////////////////////////////////////
//  IDictionaryEntry.h
//  Specification of the Interface IDictionaryEntry
///////////////////////////////////////////////////////////

/// @todo update doxygen comments

#if !defined(__OLI_IDICTIONARYENTRY_INCLUDED__)
#define __OLI_IDICTIONARYENTRY_INCLUDED__

#include "OLIDefs.h"
#include "OLITypes.h"
#include "BeginNamespace.h"

/// @ingroup GROUP_OLI_COMMON_CONFIGURATION

/**
    Access a to single \ref IDictionary entry. The @a name of the
    entry may be constructed from hierarchical information
    to form a flat namespace, e.g. 'buffer[123].offset'.

    In multi-threaded applications, synchronization on
    the owning link is required before any method of a \ref IDictionaryEntry
    is called.
*/

OLI_INTERFACE IDictionaryEntry
{
protected:

    /// Topmost interface requires a virtual destructor.

    /// Instances implementing this interface are owned
    /// by the OLL and must not be destructed by the client.

    virtual ~IDictionaryEntry() OLI_NOTHROW {};

public:

    /// entry types (based on OCI data dictionary)

    enum Type
    {
        UNSIGNED = 1,
        SIGNED = 2,
        FLOAT = 3,
        STRING = 4,
        BINARY = 5,
        RECORD = 6,
        ARRAY = 7,
        UNION = 8
    };

    /// entry name (key) within a flat namespace

    virtual const char* OLI_CALL GetName() const OLI_NOTHROW = 0;

    /// parameter type

    virtual Type OLI_CALL GetType() const OLI_NOTHROW = 0;

    /// Size of the data of this entry.
    /// 
    /// \returns For records, the number of nested entries; for arrays,
    ///          the number of entries in the array; For entries
    ///          of any other type, returns the size in bytes
    virtual uint32 OLI_CALL GetSize() const OLI_NOTHROW = 0;

    /// current value

    virtual const char* OLI_CALL GetValue() const OLI_NOTHROW = 0;
};

#include "EndNamespace.h"

#endif // !defined(__OLI_IDICTIONARYENTRY_INCLUDED__)
