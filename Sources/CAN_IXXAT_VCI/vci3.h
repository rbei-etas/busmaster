//////////////////////////////////////////////////////////////////////////
// IXXAT Automation GmbH
//////////////////////////////////////////////////////////////////////////
/**
  VCI 3 

  @file "vci3.h"
*/
//////////////////////////////////////////////////////////////////////////
// general VCI functions
//////////////////////////////////////////////////////////////////////////
// vciInitialize 
// vciFormatError
// vciDisplayError
// vciGetVersion 
// vciLuidToChar 
// vciCharToLuid
// vciGuidToChar
// vciCharToGuid 
//////////////////////////////////////////////////////////////////////////
// (C) 2002-2011 IXXAT Automation GmbH, all rights reserved
//////////////////////////////////////////////////////////////////////////

#ifndef VCI3_H
#define VCI3_H

//////////////////////////////////////////////////////////////////////////
//  include files
//////////////////////////////////////////////////////////////////////////
#include <pshpack4.h>

/*##########################################################################*/
/*##                                                                      ##*/
/*##     common declarations                                              ##*/
/*##                                                                      ##*/
/*##########################################################################*/
/*****************************************************************************
 * calling conventions
 ****************************************************************************/
#define VCIAPI __stdcall

/*****************************************************************************
 * current VCI version number
 ****************************************************************************/
#include <vciver.h> 

/*****************************************************************************
 * error handling
 ****************************************************************************/
#include <vcierr.h> 

/*****************************************************************************
 * definition of the vci id
 ****************************************************************************/
#include <vciIdType.h>


#define VCI_MAX_ERRSTRLEN    256  // maximum length of an error string

/*##########################################################################*/
/*##                                                                      ##*/
/*##     exported API functions                                           ##*/
/*##                                                                      ##*/
/*##########################################################################*/


//////////////////////////////////////////////////////////////////////////
// general functions 
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
/**
  This function initialize the VCI for the calling thread.

  @return
    VCI_OK on success, otherwise an Error code
*/
//////////////////////////////////////////////////////////////////////////
EXTERN_C HRESULT VCIAPI vciInitialize ( );

//////////////////////////////////////////////////////////////////////////
/**
  This function is called to format the specified error code into a human
  readable text string.

  @param hrError
    error code to format
  @param pszText
    Points to the location where this function stores a human readable string 
    with error information. This buffer must be at least 'dwSize'bytes in size.
  @param dwSize
    size of the memory behind pszText
*/
//////////////////////////////////////////////////////////////////////////
EXTERN_C void    VCIAPI vciFormatError( IN  HRESULT hrError,
                                        OUT PCHAR   pszText,
                                        IN  UINT32  dwSize );

//////////////////////////////////////////////////////////////////////////
/**
  This function shows a message box according to the specified error code.

  @param hwndParent
    Handle to the owner window of the message box to be showed. If this 
    parameter is NULL, the message box has no owner window. 
  @param pszCaption
    Pointer to a null-terminated string that contains the title for the 
    message box. If this parameter is NULL, a default title is used.
  @param hrError
    Error code or -1 to retrieve the error code by a call to GetLastError. 
    If this parameter is set to 0, the function does not display a message box.
*/
//////////////////////////////////////////////////////////////////////////
EXTERN_C void    VCIAPI vciDisplayError( IN HWND    hwndParent,
                                         IN PCHAR   pszCaption,
                                         IN HRESULT hrError );

//////////////////////////////////////////////////////////////////////////
/**
  This function obtains the version of the currently running VCI.

  @param pdwMajorVersion
    Pointer to an variable where the function stores the major VCI version 
    number.
  @param pdwMinorVersion
    Pointer to an variable where the function stores the minor VCI version 
    number.

  @return
    VCI_OK on success, otherwise an Error code.
*/
//////////////////////////////////////////////////////////////////////////
EXTERN_C HRESULT VCIAPI vciGetVersion ( OUT PUINT32 pdwMajorVersion,
                                        OUT PUINT32 pdwMinorVersion );

//////////////////////////////////////////////////////////////////////////
/**
  This function converts a locally unique VCI identifier (VCIID) into a string 
  of printable characters.

  @param rVciid
    Reference to the the binary-format VCIID to convert.
  @param pszLuid
    Pointer to a buffer for the converted VCIID. This buffer must be at least 
    large enough to store 17 characters.
  @param cbSize
    Size of the buffer in bytes.

  @return
    VCI_OK on success, otherwise an Error code.
*/
//////////////////////////////////////////////////////////////////////////
EXTERN_C HRESULT VCIAPI vciLuidToChar( IN  REFVCIID rVciid,
                                       OUT PCHAR    pszLuid,
                                       IN  LONG     cbSize );

//////////////////////////////////////////////////////////////////////////
/**
  This function converts a string generated by the VciLuidToChar function
  back into the original locally unique VCI identifier (VCIID).

  @param pszLuid
    Pointer to the string to be converted to a VCIID.
  @param pVciid
    Pointer to a buffer where the function stores the converted VCIID.

  @return
    VCI_OK on success, otherwise an Error code.
*/
//////////////////////////////////////////////////////////////////////////
EXTERN_C HRESULT VCIAPI vciCharToLuid( IN  PCHAR  pszLuid,
                                       OUT PVCIID pVciid );

//////////////////////////////////////////////////////////////////////////
/**
This function converts a globally unique identifier (GUID)
into a string of printable characters.

  @param rGuid
    Pointer to the GUID to convert.
  @param pszGuid
    Pointer to a buffer for the converted GUID. This buffer must be at least 
    large enough to store 39 characters.
  @param cbSize
    Size of the buffer in bytes.

  @return
    VCI_OK on success, otherwise an Error code.
*/
//////////////////////////////////////////////////////////////////////////
EXTERN_C HRESULT VCIAPI vciGuidToChar( IN  REFGUID rGuid,
                                       OUT PCHAR   pszGuid,
                                       IN  LONG    cbSize );

//////////////////////////////////////////////////////////////////////////
/**
  This function converts a string generated by the VciGuidToChar function
  back into the original globally unique identifier (GUID).

  @param pszGuid
    Pointer to the string to be converted to a GUID.
  @param pGuid
    Pointer to a buffer where the function stores the converted GUID.

  @return
    VCI_OK on success, otherwise an Error code.
*/
//////////////////////////////////////////////////////////////////////////
EXTERN_C HRESULT VCIAPI vciCharToGuid( IN  PCHAR pszGuid,
                                       OUT GUID *pGuid );

#include <poppack.h>
#endif //_VCI3_H_
