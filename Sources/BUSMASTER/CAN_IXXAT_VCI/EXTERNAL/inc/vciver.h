/*****************************************************************************
 HMS Technology Center Ravensburg GmbH
******************************************************************************

 File    : VCIVER.H
 Summary : VCI version information.
 Date    : 2018-01-22

 Compiler: Microsoft Visual C

******************************************************************************
 all rights reserved
*****************************************************************************/

#ifndef _VCIVER_H_
#define _VCIVER_H_

/*****************************************************************************
 * Macros
 ****************************************************************************/

#define VCI_VERSION_ASTXT_(n)     #n
#define VCI_VERSION_ASTEXT(n)     VCI_VERSION_ASTXT_(n)

/*****************************************************************************
 * VCI version number
 ****************************************************************************/

//
// major VCI version
//
#define VCI_MAJOR_VERSION         4
#define VCI_MAJOR_VERSION_STR     VCI_VERSION_ASTEXT(VCI_MAJOR_VERSION)

//
// minor VCI version
//
#define VCI_MINOR_VERSION         0
#define VCI_MINOR_VERSION_STR     VCI_VERSION_ASTEXT(VCI_MINOR_VERSION)

//
// VCI revision number
//
#define VCI_REV_NUMBER            175
#define VCI_REV_NUMBER_STR        VCI_VERSION_ASTEXT(VCI_REV_NUMBER)

//
// VCI build  number
//
#define VCI_BUILD_NUMBER          0
#define VCI_BUILD_NUMBER_STR      VCI_VERSION_ASTEXT(VCI_BUILD_NUMBER)


/*****************************************************************************
 * VCI version information
 ****************************************************************************/

#define VCI_VI_PRODUCT_VERS \
  VCI_MAJOR_VERSION, VCI_MINOR_VERSION, VCI_REV_NUMBER, VCI_BUILD_NUMBER

#define VCI_VI_PRODUCT_VERS_STR \
  VCI_VERSION_ASTEXT(VCI_VI_PRODUCT_VERS)

#define VCI_VI_COPYRIGHT_STR \
  "Copyright HMS Technology Center Ravensburg GmbH, all rights reserved"

#define VCI_VI_COMPANY_NAME_STR \
  "HMS Technology Center Ravensburg GmbH"

#define VCI_VI_PRODUCT_NAME_STR \
  "VCI " VCI_MAJOR_VERSION_STR

#ifdef _DEBUG
#define VCI_VI_SPECIALBUILD_STR  "Debug"
#else
#define VCI_VI_SPECIALBUILD_STR  ""
#endif

#endif //_VCIVER_H_
