/*
 * BLF Library
 * (c) 2014, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Author:      Pavel Boiko
 * Release:     1.0
 * Annotation:  Output stream
 * Revision History :
 *
 *     Date             Author               Detail description
 * ------------- --------------------- ------------------------------
 * Jan. 20, 2011 Pavel Boiko           First version
 * Jan. 21, 2011 Pavel Boiko           Multiline strings providing in hierarchy mode
 * May  15, 2014 Pavel Boiko           Adapted for BLF Library
 * May  22, 2014 Andrey Oleynikov      Compatible with visual studio 2008 toolset.
 */

#include "Out.h"

Out::Out(const std::string& fileName)
{
    Open(fileName);
}

Out::Out()
{
    // Empty
}

bool Out::Open(const std::string& fileName_)
{
    Close();

    std::ofstream::open(fileName_.c_str(), std::ios::out);
    if (good())
    {
        fileName = fileName_;

        currDepth = 0;
        nextSymbolsAreNewLine = true;
        nextLineIsLevelUp = false;
        nextLineIsLevelDown = false;
        prevLineIsLastOnLevel = false;
        nextLineIsLastOnLevel = false;
        nextLineIsMarked = false;
        currTabLine.erase();

        return true;
    }
    else
    {
        return false;
    }
}

void Out::Close()
{
    if (!fileName.empty())
    {
        std::ofstream::close();
        fileName.erase();
    }
}

void Out::PrintTabIfNeed()
{
    // Note: Unfortunately this method seems not so clean. To understand it,
    //       please, first see description of Out class (how to generate output with lines hierarchy).

    // Print hierarchy prefix like "    |-" (only before new lines)
    if (nextSymbolsAreNewLine)
    {
        nextSymbolsAreNewLine = false;

        if (0 == currDepth)
        {
            currTabLine.erase();
        }
        else
        {
            if (prevLineIsLastOnLevel && nextLineIsLevelUp)
            {
                currTabLine = currTabLine.substr(0, currTabLine.size() - 2) + "  ";    // Replace last 2 symbols to spaces
            }

            if (nextLineIsLevelUp)
            {
                if (!currTabLine.empty())
                {
                    currTabLine[currTabLine.size() - 1] = ' ';    // Replace last symbol to space
                }
                currTabLine += "  |-";
            }
            else if (nextLineIsLevelDown)
            {
                currTabLine.erase(currTabLine.size() - 5, currTabLine.size() - 1); // Remove last 5 symbols
                currTabLine += '-';
            }
        }
        *this << currTabLine;
    }
}

Out& Out::endl(Out& str)
{
    static_cast<std::ofstream&>(str) << std::endl;

    nextSymbolsAreNewLine = true;
    nextLineIsLevelUp = false;
    nextLineIsLevelDown = false;
    prevLineIsLastOnLevel = nextLineIsLastOnLevel;
    nextLineIsLastOnLevel = false;
    nextLineIsMarked = false;
    lineBeforeCurrString.erase();
    lineInclCurrString.erase();

    return str;
}

Out& Out::levelup(Out& str)
{
    ++currDepth;

    nextLineIsLevelUp = true;
    nextLineIsLevelDown = false;

    return str;
}

Out& Out::leveldown(Out& str)
{
    if (currDepth > 0)
    {
        --currDepth;

        // If nothing was printed between two 'leveldown' (if nextLineIsLevelDown)
        // and if something was printed between 'levelup' and 'leveldown' (if !nextLineIsLevelUp)
        // - decrease currTabLine here.
        // If nothing was printed between 'levelup' and 'leveldown' - increase it to compensate future
        // decreasing in 'PrintTabIfNeed'
        if (nextLineIsLevelDown && (!nextLineIsLevelUp) && (currTabLine.size() >= 4))
        {
            currTabLine.erase(currTabLine.size() - 4, 4);    // Remove last 4 symbols
        }
        else if (nextLineIsLevelUp)
        {
            currTabLine += "    ";
        }

        nextLineIsLevelUp = false;
        nextLineIsLevelDown = true;

    }
    return str;
}

Out& Out::lastonlevel(Out& str)
{
    nextLineIsLastOnLevel = true;
    return str;
}

Out& Out::markedline(Out& str)
{
    nextLineIsMarked = true;
    return str;
}

void Out::Print(const char& ch)
{
    static_cast<std::ofstream&>(*this) << ch;
    lineInclCurrString += ch;
}

void Out::NewStringStart()
{
    lineBeforeCurrString = lineInclCurrString;
}

//////////////////////////////////// Global methods

Out& operator<< (Out& c, Out& (*_f)(Out&) )
{
    return (*_f)(c);
}

Out& operator<< (Out& c, const std::string& str)
{
    c << str.c_str();
    return c;
}

Out& operator<< (Out& c, const char* str)
{
    c.PrintTabIfNeed();

    c.NewStringStart();

    std::string prefix;
    while (char wch = *str++)
    {
        if ('\r' != wch)
        {
            c.Print(wch);
        }
        if ('\n' == wch)
        {
            if (prefix.empty())
            {
                prefix = c.GetTabLine();
                prefix[prefix.size() - 1] = ' '; // Replace last '-' by ' '
                prefix.resize(c.GetCountOfLineSymbolsPrintedBeforeCurrString(), ' ');
            }
            c << prefix;
        }
    }

    return c;
}

Out& operator<< (Out& c, char* _str)
{
    c << (const char*)_str;
    return c;
}

namespace std
{
Out& endl       (Out& str)
{
    return str.endl       (str);
}
}

namespace out
{
Out& levelup    (Out& str)
{
    return str.levelup    (str);
}
Out& leveldown  (Out& str)
{
    return str.leveldown  (str);
}
Out& lastonlevel(Out& str)
{
    return str.lastonlevel(str);
}
Out& markedline (Out& str)
{
    return str.markedline (str);
}
}
