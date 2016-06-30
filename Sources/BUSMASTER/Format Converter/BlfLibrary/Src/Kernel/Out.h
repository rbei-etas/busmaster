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
 */

// This class can be used not only for simple output generation, but also for generation
// output with lines hierarchy, like this one:
//-----------------------------
//   BLF Library log:
//     |-BLF file loading - start
//     |   |-Create aux object to access BLF file
//     |   |-Read BLF file header
//     |   |-Check BLF file signature
//     |   |-Reading BLF file objects - start
//     |   |   |-Reading and uncompressing all "log container" objects - start
//   ...
//     |-BLF file loading - finish
//-----------------------------
// Here you can see that every line has position in common hierarchy. This is representation
// of some tree: "BLF file loading" includes "Create aux object to access BLF file" and other,
// "Reading BLF file objects" includes "Reading and uncompressing...".
// To be more useful, this tree dump will be generated without redundant vertical lines - see example:
//-----------------------------
// Level 1
//   |-Level 2, Item 1
//   |   |-Level 3, Item 1
//   |-Level 2, Item 2
//       |-Level 3, Item 1
//       |-Level 3, Item 2
//-----------------------------
// - you can see that two last lines don't have vertical line to "Level 1".
// This format seems good looking, but implementation is not so clean - see method 'PrintTabIfNeed'.
// I hope after this description reading you can understand all "magic" operations with strings
// inside that method without problems.
// To generate output like above, you can write a program like this one:
//    thisStream
//        << "Level 1" << std::endl << out::levelup
//        <<     "Level 2, Item 1" << std::endl << out::levelup
//        <<         "Level 3, Item 1" << out::lastonlevel << std::endl
//        <<     out::leveldown
//        <<     "Level 2, Item 2" << out::lastonlevel << std::endl << out::levelup
//        <<         "Level 3, Item 1" << std::endl
//        <<         "Level 3, Item 2" << out::lastonlevel << std::endl
//        <<     out::leveldown
//        << out::leveldown;
// Most important line is "Level 2, Item 2" output - you should use out::lastonlevel BEFORE std::endl,
// because last line on the current hierarchy level is line BEFORE this end line marker. And you
// should use out::levelup AFTER std::endl, because hierarchy level should be increased for line AFTER
// this end line marker.
// It seems other details are clear and not require special explanations.

#ifndef OUT_H
#define OUT_H

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

//! Output stream class for simple output and output with lines hierarchy.
class Out : public std::ofstream
{
public:
    //! Constructor of the object. It opens desired file. Use 'good' method to check opening result.
    Out(const std::string& fileName);
    //! Simplified constructor of the object. Usually 'Open' method then should be used.
    Out();

    //! Attaches an output file to this object.
    bool Open(const std::string& fileName);
    //! Detaches the output file from this object.
    void Close();

    //! Returns name of file attached to this stream object.
    std::string GetFileName()
    {
        return fileName;
    }

    // Note: Following methods should not be used directly -
    //       they are implemented just to redirect calls like
    //       "thisStream << out::levelup" and other from global
    //       functions to object's methods.

    Out& endl       (Out& str);
    Out& levelup    (Out& str);
    Out& leveldown  (Out& str);
    Out& lastonlevel(Out& str);
    Out& markedline (Out& str);

    //! Print one symbol to the stream.
    void Print(const char& ch);
    //! Print prefix that represents lines hierarchy (if need).
    void PrintTabIfNeed();

    // Note: Following methods are used only to provide feature
    //       "multiline strings printing in hierarhy mode". For example,
    //       to get result like:
    //         |-This is a very long comment for some
    //         |          operation that should be
    //         |          described in detail.

    //! Call it every time as new user object is started to print to stream.
    void NewStringStart();
    //! Returns count of symbols (on the current line) that are printed before the current string printing.
    int GetCountOfLineSymbolsPrintedBeforeCurrString()
    {
        return lineBeforeCurrString.size();
    }
    //! Returns tabulator string, that should be printed to show hierarchy.
    std::string GetTabLine()
    {
        return currTabLine;
    }

private:
    friend Out& operator<< (Out& c, Out& (*_f)(Out&));
    friend Out& operator<< (Out& c, const std::string& str);
    friend Out& operator<< (Out& c, const char* str);
    friend Out& operator<< (Out& c, char* str);

private:
    //! Name of file that is associated with the stream.
    std::string fileName;

    //! Current hierarchy level.
    int currDepth;

    //! True if nearest symbols that will be printed to stream will be printed on next line.
    bool nextSymbolsAreNewLine;
    //! True if next line should have depth ++1.
    bool nextLineIsLevelUp;
    //! True if next line should have depth --1.
    bool nextLineIsLevelDown;
    //! True if previous line is last item on its level of hierarchy.
    bool prevLineIsLastOnLevel;
    //! True if next line is last item on its level of hierarchy.
    bool nextLineIsLastOnLevel;
    //! True if next line should be marked as important.
    bool nextLineIsMarked;

    //! Prefix string that was printed for the current line to represent hierarchy.
    std::string currTabLine;

    //! It contains all symbols printed on the current line before the current string object printing was started.
    std::string lineBeforeCurrString;
    //! It contains all symbols printed on the current line include the current string object printined symbols.
    std::string lineInclCurrString;
};

template <class T>
Out& operator<< (Out& str, T t)
{
    static_cast<std::ofstream&>(str) << t;
    return str;
}

namespace std
{
Out& endl       (Out& str);
}

namespace out
{
Out& levelup    (Out& str);
Out& leveldown  (Out& str);
Out& lastonlevel(Out& str);
Out& markedline (Out& str);
}

#endif //#ifndef OUT_H
