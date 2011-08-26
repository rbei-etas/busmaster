// open the namespace

/// @todo update doxygen comments

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
