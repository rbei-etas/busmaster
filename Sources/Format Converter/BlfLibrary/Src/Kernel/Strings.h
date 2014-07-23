/*
 * BLF Library
 * (c) 2014, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Author:      Pavel Boiko (based on original code of unknown author, ~2000)
 * Release:     1.0
 * Annotation:  Enhancements of string (STL).
 * Revision History :
 *
 *     Date             Author               Detail description
 * ------------- --------------------- ------------------------------
 * Aug. 25, 2010 Pavel Boiko           First experimental version
 * May  15, 2014 Pavel Boiko           Adapted for BLF Library
 */

#ifndef STRINGS_H
#define STRINGS_H

#include <string>
#include <sstream>
#include <iostream>

using namespace std;

inline string& operator+= (string& s, int i)
{
    std::ostringstream stream;
    stream << s << i;
    return s = stream.str();
}

inline string operator+ (const string& s, int i)
{
    std::ostringstream stream;
    stream << s << i;
    return stream.str();
}

inline string operator+ (int i, const string& s)
{
    std::ostringstream stream;
    stream << i << s;
    return stream.str();
}

inline string operator+ (const string& s, long i)
{
    std::ostringstream stream;
    stream << s << i;
    return stream.str();
}

inline string operator+ (const string& s, size_t i)
{
    std::ostringstream stream;
    stream << s << i;
    return stream.str();
}

#endif //#ifndef STRINGS_H
