/*
**                   Copyright 2005 by KVASER AB, SWEDEN      
**                        WWW: http://www.kvaser.com
**
**
** Description:
**   This file resets the struct alignment to the value
**   prevalent before pshpackN.h was included.  Many compilers provide a
**   file similar to this one.
** ---------------------------------------------------------------------------
*/
#if ! (defined(lint) || defined(_lint) || defined(RC_INVOKED))
#  if defined(_MSC_VER)
#    if ( _MSC_VER > 800 ) || defined(_PUSHPOP_SUPPORTED)
#      pragma warning(disable:4103)
#      if !(defined( MIDL_PASS )) || defined( __midl )
#        pragma pack(pop)
#      else
#        pragma pack()
#      endif
#    else
#      pragma warning(disable:4103)
#      pragma pack()
#    endif
#  elif defined(__C166__)
#    pragma pack()
#  elif defined(__GCC__)
#    pragma pack(pop)
#  elif defined(__LCC__)
#    pragma pack(pop)
#  elif defined(__MWERKS__)
#    pragma pack(pop)
#  elif defined(__BORLANDC__)
#    if (__BORLANDC__ >= 0x460)
#       pragma nopackwarning
#       pragma pack(pop)
#    else
#       pragma option -a.
#    endif
#  else
#    error Unsupported compiler.
#  endif
#endif /* ! (defined(lint) || defined(_lint) || defined(RC_INVOKED)) */
