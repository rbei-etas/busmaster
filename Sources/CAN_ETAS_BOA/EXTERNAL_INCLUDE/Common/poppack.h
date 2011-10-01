/**
 * @file
 **********************************************************************************************
 * Compiler independent alignment of data structures.
 * Inclusion of this header files switches back to compiler natural alignment and packing.
 *
 **********************************************************************************************
 * 2005-06-03                                                   Copyright ETAS GmbH, Stuttgart.
 **********************************************************************************************/

/*====(global) interfaces exported by this file===============================================*/

#if defined(__GNUC__)
#	pragma pack(pop)
#elif defined (_MSC_VER)
#  pragma pack(pop)
#else
#	error Unknown compiler. Cannot explicitly switch to natural alignment.
#endif

