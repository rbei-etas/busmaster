/*
**                   Copyright 2005 by KVASER AB, SWEDEN      
**                        WWW: http://www.kvaser.com
**
**
** Description:
**   This file sets the struct alignment to 1 byte and saves the old
**   alignment.  Many compilers provide a file similar to this one.
** ---------------------------------------------------------------------------
*/
#if ! (defined(lint) || defined(_lint) || defined(RC_INVOKED))
#  if defined(_MSC_VER)
#    if ( _MSC_VER > 800 ) || defined(_PUSHPOP_SUPPORTED)
#      pragma warning(disable:4103)
#      if !(defined( MIDL_PASS )) || defined( __midl )
#        pragma pack(push)
#      endif
#      pragma pack(1)
#    else
#      pragma warning(disable:4103)
#      pragma pack(1)
#    endif
#  elif defined(__C166__)
#     pragma pack(1)
#  elif defined(__GCC__)
#     pragma pack(push, 1)
#  elif defined(__LCC__)
#     pragma pack(push, 1)
#  elif defined(__MWERKS__)
#     pragma pack(push, 1)
#  elif defined(__BORLANDC__)
#    if (__BORLANDC__ >= 0x460)
#       pragma nopackwarning
#       pragma pack(push, 1)
#    else
#       pragma option -a1
#    endif
#  else
#    error Unsupported compiler. See the compiler docs for more info on its alignment pragmas.
#  endif
#endif /* ! (defined(lint) || defined(_lint) || defined(RC_INVOKED)) */
