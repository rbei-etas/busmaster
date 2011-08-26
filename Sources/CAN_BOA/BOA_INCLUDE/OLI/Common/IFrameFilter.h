///////////////////////////////////////////////////////////
//  IFrameFilter.h
//  Implementation of the Interface IFrameFilter
//  Created on:      19-Mrz-2008 01:23:28
///////////////////////////////////////////////////////////

/// @todo update doxygen comments

#if !defined(__OLI_IFRAMEFILTER_INCLUDED__)
#define __OLI_IFRAMEFILTER_INCLUDED__

// include base interface

#include "IFilter.h"
#include "../Common/ErrorInterfaces.h"
#include "../Common/ErrorProxies.h"

#include "BeginNamespace.h"

/// @ingroup GROUP_OLI_COMMON_FILTERS

/** Type for a single payload filter condition. The message payload is 
* interpreted as a little endian byte stream to make 
* *(BYTE *)(data+offset) == 0xff & *(DWORD *)(data+offset).
* @todo 2009-05-06, kuh: this description is at least misleading 
*  - be more exhaustive on the behaviour, e.g. in case you pass a mask > 0xFF
*  - I interpret this as 1 PayloadCondition acts on exactly 1 byte of payload. 
*  - Please change this to uint32, as a signed mask is somehow strange and a negativ offset also is.
*/

struct PayloadCondition
{
	uint32 mask;
	uint32 offset;
	uint32 value;
};

/** Just as @ref IRxFrame extends @ref IRxMessage by payload,
* @ref IFrameFilter extends @ref IFilter by conditions on
* payload filters. All conditions must be met to pass the filter.
*
* Initializing the filter condition is up to the implementation classes
* and may be part of the respective constructors.
*/

OLI_INTERFACE IFrameFilter : public IFilter
{
protected:

    /// Lifetime of filter instances is controlled by reference counting.

    virtual ~IFrameFilter() OLI_NOTHROW {};

    /**
     * This method provides access to the individual playload conditions.
     *
     * \param[in]  index        Valid range is from 0 .. GetCount() - 1
     * \param[out] ppCondition  The specified condition.
     *
     * \return A pointer to an interface based on IError, describing the error which occurred during this function. NULL if no error
     * occurred. See \ref ErrorReporting for more information on how errors are reported.
     */
    virtual IError* OLI_CALL GetCondition( uint32 index, const PayloadCondition** ppCondition ) const OLI_NOTHROW = 0;

public:

    /// number of payload conditions

    virtual uint32 OLI_CALL GetCount() const OLI_NOTHROW = 0;

    /**
     * This is a helper method which wraps a corresponding protected method: see \ref ErrorReporting for an explanation of why it is needed.
     */
    const PayloadCondition& OLI_CALL GetCondition (uint32 index) const
    {
        const PayloadCondition* pCondition = NULL;
        CheckAndThrow( GetCondition( index, &pCondition ) );
        return *pCondition;
    }
};

#include "EndNamespace.h"

#endif // !defined(__OLI_IFRAMEFILTER_INCLUDED__)
