/*
 * BLF Library
 * (c) 2014, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Author:      Pavel Boiko
 * Release:     1.0
 * Annotation:  External interfaces of the library.
 *              The library goal is providing access to BLF file data.
 *              It is based on a prototype of Tobias Lorenz and some investigation results of Andrey Oleynikov.
 * Revision History :
 *
 *     Date             Author               Detail description
 * ------------- --------------------- ------------------------------
 * May  15, 2014 Pavel Boiko           First version. It supports only CAN messages, and only reading (not writing).
 * May  21, 2014 Andrey Oleynikov      Timestamps, message directions and types (extended or standard) were added.
 */

#ifndef BLFLIBRARY_H
#define BLFLIBRARY_H

// BLFLIBRARY_API macro is required for switching the dllexport/dllimport mode - it is useful to be able to use
// the same header file for library building and for library using.
#undef BLFLIBRARY_API
#    ifdef BLFLIBRARY_EXPORTS
#        define BLFLIBRARY_API __declspec(dllexport)
#    else
#        define BLFLIBRARY_API __declspec(dllimport)
#    endif

#include <string>
#include <windows.h> // Just for types like DWORD

namespace BLF
{

class ICanMessage;

//! Kinds of BLF objects.
enum BlfObjectKind
{
    bokUnknown       //! Unknown object type.
    , bokCanMessage    //! CAN message.
};

//! Interface for classes that support dump ability.
class IDumper
{
public:
    //! Prints content of the class to std::out.
    virtual bool Dump() = 0;
};

//! BLF object interface class. It is a base interface for all specific BLF objects like ICanMessage.
class IBlfObject : public IDumper
{
public:
    //! Returns kind of the object.
    virtual BlfObjectKind GetKind() = 0;
    //! Returns CAN Message object if current object represents it (or nullptr otherwise).
    virtual ICanMessage* GetICanMessage() = 0;
};

//! Message direction kinds.
BLFLIBRARY_API enum MessageDirection
{
    mdRx     //! Received message.
    , mdTx     //! Transmitted message.
};

//! BLF object "CAN Message" interface class.
class ICanMessage : public IBlfObject
{
public:
    //! Returns message channel number.
    virtual WORD GetChannelNo() = 0;
    //! Returns message dentifier.
    virtual DWORD GetId() = 0;
    //! Returns message DLC (data length).
    virtual BYTE GetDLC() = 0;
    //! Returns message data.
    virtual const BYTE* GetData() = 0;
    //! Returns message timestamp
    virtual ULONGLONG GetTimestamp() = 0;
    //! Returns message direction (Rx/Tx)
    virtual MessageDirection GetDirection() = 0;
};

//! Interface class of the library.
class IBlfLibrary : public IDumper
{
public:
    //! Enables logging mechanism of the library. Disabled by default.
    virtual bool EnableLogging(const std::string& sLogFilePath) = 0;

    //! Loads desired BLF file.
    //! \param sBlfFilePath Path to BLF file that should be loaded.
    virtual HRESULT Load(const std::string& sBlfFilePath) = 0;
    //! Releases all resources that were allocated by IBlfLibrary::Load (if they are). If the library will be unloaded, this method will be called automatically.
    virtual bool UnLoad() = 0;

    //! Returns count of available BLF objects.
    virtual size_t GetBlfObjectsCount() = 0;
    //! Returns BLF object by index.
    //! \param index Index of desired BLF object. For example, zero for the first one.
    virtual IBlfObject* GetBlfObject(size_t index) = 0;

    //! Returns start time for loaded blf file
    virtual SYSTEMTIME GetStartTime() = 0;
};

//! Returns the library interface.
BLFLIBRARY_API IBlfLibrary* GetIBlfLibrary();

} // namespace blf

#endif //#ifndef BLFLIBRARY_H
