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
* @brief  Open namespace and initialize structure aligmnent
* @remark The header structure of the OLI may change
*         in future releases. Don't include this
*         header directly. Use @ref OLI.h instead.
*/

/** @namespace ETAS 
 * Common namespace for all C++ interfaces published by ETAS GmbH.
 */

/** @namespace ETAS::OLI
 * Common namespace for all OLI definitions.
 */

namespace ETAS { namespace OLI {

// all structures within this namespace 
// shall use the same explicit alignment
    
#if defined(__GNUC__)
#	pragma pack(push,1)
#elif defined(_MSC_VER)
#   pragma warning(disable:4103)
#	pragma pack(push,4)
#else 
#	error Unknown compiler. Do not know how to do 1 byte alignment.
#endif
