/**
 * @file
 **********************************************************************************************
 * Compiler independent alignment of data structures.
 * Most compilers use a pragma-directive like "#pragma pack" to control alignment and packing,
 * but there is no common standard. A compiler independent method to align structures at 1 byte
 * boundaries is to include this header file. After inclusion, 1 byte alignment is used. Switch
 * back to the natural compiler alignment by including the header file "poppack.h".
 *
 **********************************************************************************************
 * 2005-06-03                                                   Copyright ETAS GmbH, Stuttgart.
 **********************************************************************************************/

/*====(global) interfaces exported by this file===============================================*/

#if defined(__GNUC__)
#	pragma pack(push,1)
#elif defined(_MSC_VER)
#   pragma warning(disable:4103)
#	pragma pack(push,1)
#else 
#	error Unknown compiler. Do not know how to do 1 byte alignment.
#endif

