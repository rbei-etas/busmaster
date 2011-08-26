/// @todo update doxygen comments

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
