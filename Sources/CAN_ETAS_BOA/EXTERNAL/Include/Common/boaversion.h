#ifndef ETAS_BOAVERSION_H
#define ETAS_BOAVERSION_H

/**
* @file       boaversion.h
* @brief      Header file for declaration of the version of BOA
* @copyright  Copyright (c) 2007-2008 ETAS GmbH. All rights reserved.
*/

/* A BOA version is defined by four integers, e.g. "1.3.0.12". These preprocessor definitions define the version resource which
 * is applied to all BOA DLLs. */
#define BOA_VERSION_DIGIT1  1
#define BOA_VERSION_DIGIT2  4
#define BOA_VERSION_DIGIT3  2
#define BOA_VERSION_DIGIT4  4

/* This preprocessor definition expands to the full BOA version, with digits separated by commas. */
#define BOA_VERSION_COMMAS  BOA_VERSION_DIGIT1, BOA_VERSION_DIGIT2, BOA_VERSION_DIGIT3, BOA_VERSION_DIGIT4

#define BOA_STRINGISE_INTERNAL( arg ) #arg
#define BOA_STRINGISE( arg ) BOA_STRINGISE_INTERNAL( arg )

/* This preprocessor definition expands to the full BOA version, with digits separated by commas, enclosed in double quotes. */
#define BOA_VERSION_COMMAS_STRING BOA_STRINGISE( BOA_VERSION_COMMAS )

#endif
