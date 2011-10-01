///////////////////////////////////////////////////////////
//  IDictionary.h
//  Specification of the Interface IDictionary.
///////////////////////////////////////////////////////////

/// @todo update doxygen comments

#if !defined(__OLI_IDICTIONARY_INCLUDED__)
#define __OLI_IDICTIONARY_INCLUDED__

#include "OLIDefs.h"
#include "ErrorInterfaces.h"
#include "ErrorProxies.h"
#include "BeginNamespace.h"

// interface forward declarations

class IDictionaryEntry;

/// @ingroup GROUP_OLI_COMMON_CONFIGURATION

/** 
* Represents read-only a key/value container based on some OCI 
* data dictionary. Specialized interfaces will extend it with
* support for modification and other features.
*
* Objects providing this interface are expected to be unique
* members of a link object providing @ref ILink. Thus, it has
* an (indirect) connection to the actual controller and cannot
* be instantiated by OLI users.
*
* The class implementing @ref IDictionary is also expected to 
* keep the structure definitions (OCI data dictionary) in some 
* internal buffer. For some sub-classes, that buffer must be
* filled explicitly (see there).
*/

OLI_INTERFACE IDictionary
{
protected:

    /// Topmost interface requires a virtual destructor.

    /// Instances implementing this interface are owned
    /// by the OLL and must not be destructed by the client.

    virtual ~IDictionary() OLI_NOTHROW {};

    /**
     * This method accesses the individual dictionary entries (key/value pairs). 
     *
     * \param[in]  index        The valid range is 0 .. GetCount - 1.
     * \param[out] ppDictEntry  The specifies dictionary entry; the ownership of this object remains with the dictionary.
     *
     * \return A pointer to an interface based on IError, describing the error which occurred during this function, or NULL
     * if no error occurred. A pointer to IOutOfRangeError will be returned if \a index is out of range. See
     * \ref ErrorReporting for more information on how errors are reported.
     */
    virtual IError* OLI_CALL GetEntry( uint32 index, const IDictionaryEntry** ppDictEntry ) const OLI_NOTHROW = 0;

    /**
     * This method provides case-sensitive parameter lookup.
     *
     * \param[in]  name
     * \param[out] ppDictEntry  The specifies dictionary entry; the ownership of this object remains with the dictionary.
     *                          The value is NULL if the specified entry does not exist.
     *
     * \return A pointer to an interface based on IError, describing the error which occurred during this function, or NULL
     * if no error occurred. See \ref ErrorReporting for more information on how errors are reported.
     */
    virtual IError* OLI_CALL FindEntry( const char* name, const IDictionaryEntry** ppDictEntry ) const OLI_NOTHROW = 0;

    /**
     * This method writes offline configuration to a file.
     *
     * \param[in] fileName      The name of the new configuration file. An existing file will be overwritten without further
     *                          notice.
     * \param[in] mimeType      The MIME type to use for the specified file. Currently only "application/json" is supported.
     *
     * \return A pointer to an interface based on IError, describing the error which occurred during this function, or NULL
     * if no error occurred. A pointer to INotSupportedException will be returned if the specified MIME type cannot be handled.
     * See \ref ErrorReporting for more information on how errors are reported.
     */
	virtual IError* OLI_CALL WriteToFileRaw( const char* fileName, const char* mimeType ) OLI_NOTHROW = 0;

public:

    /// number of entries. Some dictionaries require to be filled
    /// explicitly. Before that, they will report 0 entries.
    /// \see IConfigurationContainer .

    virtual uint32 OLI_CALL GetCount() const OLI_NOTHROW = 0;

    /**
     * This is a helper method which wraps a corresponding protected method: see \ref ErrorReporting for an explanation of why it is needed.
     */
    const IDictionaryEntry* OLI_CALL GetEntry (uint32 index) const
    {
        const IDictionaryEntry* pDictEntry = NULL;
        CheckAndThrow( GetEntry( index, &pDictEntry ) );
        return pDictEntry;
    }

    /**
     * This is a helper method which wraps a corresponding protected method: see \ref ErrorReporting for an explanation of why it is needed.
     */
    const IDictionaryEntry* OLI_CALL FindEntry (const char* name) const
    {
        const IDictionaryEntry* pDictEntry = NULL;
        CheckAndThrow( FindEntry( name, &pDictEntry ) );
        return pDictEntry;
    }

    /**
     * This is a helper method which wraps a corresponding protected method: see \ref ErrorReporting for an explanation of why it is needed.
     */
    void OLI_CALL WriteToFile( const char* fileName, const char* mimeType)
    {
        CheckAndThrow( WriteToFileRaw( fileName, mimeType ) );
    }
};

#include "EndNamespace.h"

#endif // !defined(__OLI_IDICTIONARY_INCLUDED__)
