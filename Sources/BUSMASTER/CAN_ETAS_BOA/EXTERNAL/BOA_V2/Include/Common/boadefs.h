#ifndef ETAS_BOA_DEFS_H
#define ETAS_BOA_DEFS_H

/**
* @file       boadefs.h
* @brief      Global defines for BOA.
* @copyright  Copyright (c) ETAS GmbH. All rights reserved.
*
* $Revision: 7119 $
*/

/**
 * The macro BOA_DECLSPEC_DEPRECATED_FUNC() is used to declare a function as "deprecated", thereby causing the compiler to
 * issue a warning whenever the function is used. These warnings can be suppressed globally by defining the preprocessor symbol
 * BOA_NO_DEPRECATED_WARNINGS, or individually by using the macro BOA_USE_DEPRECATED_FN().
 */
#if defined (_MSC_VER) && !defined(BOA_NO_DEPRECATED_WARNINGS)
#define BOA_DECLSPEC_DEPRECATED_FUNC(FUNC_DEF) __declspec(deprecated("This function is deprecated. Please see BOA Reference Guide for further details."))  FUNC_DEF
#elif defined(__GNUC__) && defined (__QNX__) && !defined(BOA_NO_DEPRECATED_WARNINGS)
#define BOA_DECLSPEC_DEPRECATED_FUNC(FUNC_DEF) FUNC_DEF __attribute__ ((deprecated))
#else
#define BOA_DECLSPEC_DEPRECATED_FUNC(FUNC_DEF) FUNC_DEF
#endif

/*
 * The macro BOA_USE_DEPRECATED_FN() allows the suppression of an individual instance of a "deprecated" warning caused by
 * using a function which was declared with BOA_DECLSPEC_DEPRECATED_FUNC.
 * For example:
 *      result = DeprecatedBoaFn();                             // Causes a compiler warning about use of a deprecated function.
 *      BOA_USE_DEPRECATED_FN( result = DeprecatedBoaFn() );    // Suppresses the compiler warning.
 */
#if defined (_MSC_VER)
#define BOA_USE_DEPRECATED_FN( fn )     \
    __pragma( warning(suppress: 4996) )     \
    fn
#elif defined(__GNUC__)
/* Unfortunately, the required functionality for suppressing warnings (i.e. #pragma GCC diagnostic) is not supported
 * in the version of gcc used by QNX 6.5.0 */
#define BOA_USE_DEPRECATED_FN( fn ) fn
#else
#define BOA_USE_DEPRECATED_FN( fn ) fn
#endif

/** disable a specific warning on a microsoft visual C project */
#if defined(_MSC_VER) && !defined(BOA_DECLSPEC_WARNING_DISABLE) && !defined(MIDL_RUNNING)
#define BOA_DECLSPEC_WARNING_DISABLE(WARNING_IDENTIFIER)    \
    __pragma (warning(push))   \
    __pragma (warning(disable:WARNING_IDENTIFIER))
#else
#define BOA_DECLSPEC_WARNING_DISABLE(WARNING_IDENTIFIER)
#endif

/** resume the warning level to the previous value on a microsoft visual C project */
#if defined(_MSC_VER) && !defined(BOA_DECLSPEC_WARNING_RESUME) && !defined(MIDL_RUNNING)
#define BOA_DECLSPEC_WARNING_RESUME    \
    __pragma (warning(pop)   )
#else
#define BOA_DECLSPEC_WARNING_RESUME
#endif

#endif /* ETAS_BOA_DEFS_H */