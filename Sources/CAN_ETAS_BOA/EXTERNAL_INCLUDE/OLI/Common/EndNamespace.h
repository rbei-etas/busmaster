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
* @brief  Close namespace and restore previous structure aligmnent
* @remark The header structure of the OLI may change
*         in future releases. Don't include this
*         header directly. Use @ref OLI.h instead.
*/

// reset the structure alignment

#if defined(__GNUC__)
#	pragma pack(pop)
#elif defined (_MSC_VER)
#  pragma pack(pop)
#else
#	error Unknown compiler. Cannot explicitly switch to natural alignment.
#endif

// close the namespace
}}
