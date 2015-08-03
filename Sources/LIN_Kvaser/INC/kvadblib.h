/**
 * \section LICENSE
 *                   Copyright 2002-2011 by KVASER AB, SWEDEN
 *
 *                        WWW: http://www.kvaser.com
 *
 * This software is furnished under a license and may be used and copied
 * only in accordance with the terms of such license.
 *
 * \section DESCRIPTION
 *
 *   Definitions for the CAN Database API.
 *
 * \file kvaDBlib.h
 * \author Kvaser AB
 *
 */

#ifndef KVADBLIB_H
#define KVADBLIB_H

/**
 * \defgroup CANDB  CAN Database
 * @{
 */


#ifdef __cplusplus
extern "C" {
#endif


/**
 *
 * \name kvaDbErr_xxx
 * \anchor kvaDbErr_xxx
 *
 * The following codes are returned by most CANDB API calls.
 *
 * \note Error codes are always negative.
 * @{
 */
typedef enum {
    kvaDbOK                      =  0,  ///< OK - no error
    kvaDbErr_Fail                = -1,  ///< General failure
    kvaDbErr_NoDatabase          = -2,  ///< No database was found
    kvaDbErr_Param               = -3,  ///< One or more of the parameters in call is erronous
    kvaDbErr_NoMsg               = -4,  ///< No message was found
    kvaDbErr_NoSignal            = -5,  ///< No signal was found
    kvaDbErr_Internal            = -6,  ///< An internal error occured in the library
    kvaDbErr_DbFileOpen          = -7,  ///< Could not open the database file
    kvaDbErr_DatabaseInternal    = -8,  ///< An internal error occured in the database handler
    kvaDbErr_NoNode              = -9,  ///< Could not find the database node
    kvaDbErr_NoAttrib            = -10, ///< No attribute found
    kvaDbErr_OnlyOneAllowed      = -11, ///< Only one CANdb structure is allowed
} KvaDbStatus;
/** @} */

/**
 * \name Kvaser Database Message Flags
 * \anchor KVADB_MESSAGE_xxx
 *
 * The following flags are never used.
 *
 * \todo Why are they never used?
 * @{
 */
#define KVADB_MESSAGE_EXT      0x80000000  ///< Message is an extended CAN message
#define KVADB_MESSAGE_J1939    0x00000001  ///< Message uses J1939 protocol
#define KVADB_MESSAGE_WAKEUP   0x00000002  ///< Message is a wake-up frame, Id/msg/dlc are undefined
/** @} */

/**
 * \name CANDBSignalEncoding API Signal Encoding
 * 
 * The following values are used by kvaDbGetSignalEncoding() and
 * kvaDbSetSignalEncoding()
 * @{
 */
typedef enum {
  kvaDb_Intel     = 0, ///< Intel
  kvaDb_Motorola  = 1, ///< kvaDb_Motorola
} KvaDbSignalEncoding;
/** @} */

/**
 * \name CANDBSignalRepresentation API Signal Representation
 * 
 * The type ::KvaDbSignalType is used by e.g. kvaDbGetSignalRepresentationType().
 * @{
 */
typedef enum  {
   kvaDb_Invalid,   ///< Invalid representation
   kvaDb_Signed,    ///< Signed integer
   kvaDb_Unsigned,  ///< Unsigned integer
   kvaDb_Float,     ///< Float
   kvaDb_Double     ///< Double
} KvaDbSignalType;
/** @} */

// Database flags

/**
 * \name CANDBDatabaseFlags API Database Flags
 * \anchor KVADB_DATABASE_xxx
 * 
 * The following value is used by kvaDbGetFlags().
 * @{
 */
#define KVADB_DATABASE_J1939   0x0001  ///< The database uses the J1939 protocol

typedef void* KvaDbHnd;         ///< Database handle
typedef void* KvaDbMessageHnd;  ///< Message handle
typedef void* KvaDbSignalHnd;   ///< Signal handle
typedef void* KvaDbNodeHnd;     ///< Node handle 
/** @} */

/**
 * This function creates a new database handle.
 * 
 * \param[out] dh  A handle to a new database
 * 
 * \return ::kvaDbOK (zero) if success
 * \return \ref kvaDbErr_xxx (negative) if failure
 * 
 * \sa kvaDbClose()
 */
KvaDbStatus WINAPI kvaDbOpen(KvaDbHnd *dh);

/**
 * This loads a database file into a handle created with kvaDbOpen().
 * 
 * \note This function is deprecated and will be replaced by kvaDbCreate()
 *
 * \param[in] dh        A handle database handle
 * \param[in] filename  A database file
 * 
 * \return ::kvaDbOK (zero) if success
 * \return \ref kvaDbErr_xxx (negative) if failure
 * 
 * \sa kvaDbCreate()
 */
KvaDbStatus WINAPI kvaDbAddFile(KvaDbHnd dh, const char *filename);

/**
 * This function creates a new database in a handle created with
 * kvaDbOpen(). It is used when there is no file to read data from.
 *
 * \note This function is deprecated and will be replaced by kvaDbCreate().
 * 
 * \param[in] dh        A database handle
 * \param[in] filename  The the dummy filename to use for this database
 * 
 * \return ::kvaDbOK (zero) if success
 * \return \ref kvaDbErr_xxx (negative) if failure
 * 
 * \sa kvaDbCreate()
 */
KvaDbStatus WINAPI kvaDbSetDummyFileName(KvaDbHnd dh, const char *filename);

/**
 * This function creates a new database in a handle created with
 * kvaDbOpen(). There are three ways to call this function:
 *   
 * \li To load data from an existing database file, set \a localName to \c NULL
 * and set \a filename to the database file.
 *   
 * \li To add an empty database, set \a localName to any name and set \a
 * filename to \c NULL.
 *   
 * \li To load data from an existing database file and give it a new name, set
 * \a localName to the new name and set \a filename to the database file.
 * 
 * \param[in] dh         A database handle
 * \param[in] localName  The name of the database or \c NULL
 * \param[in] filename   The database file or \c NULL
 * 
 * \return ::kvaDbOK (zero) if success
 * \return \ref kvaDbErr_xxx (negative) if failure
 * 
 * \sa kvaDbOpen()
 */
KvaDbStatus WINAPI kvaDbCreate(KvaDbHnd dh, const char *localName, const char *filename);

/**
 * This function closes an open database handle.
 * 
 * \param[in] dh  A handle to a database
 * 
 * \return ::kvaDbOK (zero) if success
 * \return \ref kvaDbErr_xxx (negative) if failure
 * 
 * \sa kvaDbOpen()
 */
KvaDbStatus WINAPI kvaDbClose(KvaDbHnd dh);

/**
 * Write a database to file.
 * 
 * \param[in] dh        A handle to a database
 * \param[in] filename  The name of the database file
 * 
 * \return ::kvaDbOK (zero) if success
 * \return \ref kvaDbErr_xxx (negative) if failure
 * 
 * \sa kvaDbReadFile()
 */
KvaDbStatus WINAPI kvaDbWriteFile(KvaDbHnd dh, char* filename);

/**
 * Load a database from file into a handle created with kvaDbOpen().
 * 
 * \param[out]  dh        A handle to a database
 * \param[in]   filename  The name of the database file.
 * 
 * \return ::kvaDbOK (zero) if success
 * \return \ref kvaDbErr_xxx (negative) if failure
 * 
 * \sa kvaDbCreate()
 */
KvaDbStatus WINAPI kvaDbReadFile(KvaDbHnd dh, char* filename);

/**
 * Get the database flags.
 * 
 * \param[in]  dh     A handle to a database
 * \param[out] flags  The database flags, \ref KVADB_DATABASE_xxx
 * 
 * \return ::kvaDbOK (zero) if success
 * \return \ref kvaDbErr_xxx (negative) if failure
 * 
 * \sa kvaDbReadFile()
 *
 * \todo check kvaDbReadFile(), we should perhaps link to kvaDbCreate()?
 */
KvaDbStatus WINAPI kvaDbGetFlags(KvaDbHnd dh, unsigned int *flags);

/**
 * Get the first message in a database.
 * 
 * \param[in]  dh  A handle to a database
 * \param[out] mh  A handle to the first message
 * 
 * \return ::kvaDbOK (zero) if success
 * \return \ref kvaDbErr_xxx (negative) if failure
 * 
 * \sa kvaDbGetNextMsg()
 */
KvaDbStatus WINAPI kvaDbGetFirstMsg(KvaDbHnd dh, KvaDbMessageHnd *mh);

/**
 * Get a handle to the next message in a database. Should be called after
 * kvaDbGetFirstMsg().
 * 
 * \param[in]  dh  A database handle
 * \param[out] mh  A message handle
 * 
 * \return ::kvaDbOK (zero) if success
 * \return \ref kvaDbErr_xxx (negative) if failure
 * 
 * \sa kvaDbGetFirstMsg()
 */
KvaDbStatus WINAPI kvaDbGetNextMsg(KvaDbHnd dh, KvaDbMessageHnd *mh);

/**
 * Get a handle to a message with a specific identifier.
 * 
 * \param[in]  dh  A database handle
 * \param[in]  id  The selected message identifier
 * \param[out] mh  A message handle
 * 
 * \return ::kvaDbOK (zero) if success
 * \return \ref kvaDbErr_xxx (negative) if failure
 * 
 * \sa kvaDbGetMsgByName()
 */
KvaDbStatus WINAPI kvaDbGetMsgById(KvaDbHnd dh, 
                                   unsigned int id, 
                                   KvaDbMessageHnd *mh);

/**
 * Get a handle to a message with a specific name.
 * 
 * \param[in]  dh        A databse handle
 * \param[in]  msg_name  The message name to search for
 * \param[out] mh        A message handle
 * 
 * \return ::kvaDbOK (zero) if success
 * \return \ref kvaDbErr_xxx (negative) if failure
 * 
 * \sa kvaDbGetMsgById()
 */
KvaDbStatus WINAPI kvaDbGetMsgByName(KvaDbHnd dh, 
                                     const char *msg_name, 
                                     KvaDbMessageHnd *mh);

/**
 * Get the message name.
 * 
 * \param[in]  mh      A message handle
 * \param[out] buf     The buffer that will hold the message name
 * \param[in]  buflen  The length of the buffer.
 * 
 * \return ::kvaDbOK (zero) if success
 * \return \ref kvaDbErr_xxx (negative) if failure
 * 
 * \sa kvaDbSetMsgName(), kvaDbGetMsgQualifiedName()
 */
KvaDbStatus WINAPI kvaDbGetMsgName(KvaDbMessageHnd mh, char *buf, size_t buflen);

/**
 * Get the qualified message name, which is the database name and the message
 * name separated by a dot.
 * 
 * \param[in]  mh      A message handle
 * \param[out] buf     The buffer that will hold the qualified message name
 * \param[in]  buflen  The length of the buffer
 * 
 * \return ::kvaDbOK (zero) if success
 * \return \ref kvaDbErr_xxx (negative) if failure
 * 
 * \sa kvaDbGetMsgName()
 */
KvaDbStatus WINAPI kvaDbGetMsgQualifiedName(KvaDbMessageHnd mh, char *buf, size_t buflen);

/**
 * Get the message comment.
 * 
 * \param[in]  mh      A message handle
 * \param[out] buf     The buffer that will hold the message comment
 * \param[in]  buflen  The length of the buffer
 * 
 * \return ::kvaDbOK (zero) if success
 * \return \ref kvaDbErr_xxx (negative) if failure
 * 
 * \sa kvaDbSetMsgComment()
 */
KvaDbStatus WINAPI kvaDbGetMsgComment(KvaDbMessageHnd mh, char *buf, size_t buflen);

/**
 * Get message identifier and flags.
 * 
 * \param[in]  mh     A message handle.
 * \param[out] id     The identifier.
 * \param[out] flags  The flags, \ref KVADB_MESSAGE_xxx
 * 
 * \return ::kvaDbOK (zero) if success
 * \return \ref kvaDbErr_xxx (negative) if failure
 * 
 * \todo "flags" contains extended? or use 0x80000000 convention?
 *       could also contain J1939 flag, wakeup etc. qqq
 *
 * \sa kvaDbSetMsgId()
 */
KvaDbStatus WINAPI kvaDbGetMsgId(KvaDbMessageHnd mh, 
                                 unsigned int *id, 
                                 unsigned int *flags);

/**
 * Get the data length code for a message.
 * 
 * \param[in]  mh   A message handle
 * \param[out] dlc  The data length code
 * 
 * \return ::kvaDbOK (zero) if success
 * \return \ref kvaDbErr_xxx (negative) if failure
 * 
 * \sa kvaDbSetMsgDlc()
 */
KvaDbStatus WINAPI kvaDbGetMsgDlc(KvaDbMessageHnd mh, int *dlc);

/**
 * Add a new message to a database.
 * 
 * \param[in]  dh  A database handle
 * \param[out] mh  A handle to the new message
 * 
 * \return ::kvaDbOK (zero) if success
 * \return \ref kvaDbErr_xxx (negative) if failure
 * 
 * \sa kvaDbDeleteMsg()
 */
KvaDbStatus WINAPI kvaDbAddMsg(KvaDbHnd dh, KvaDbMessageHnd *mh);

/**
 * Delete a message from a database.
 * 
 * \param[in] dh  A database handle
 * \param[in] mh  A handle to the message that will be deleted
 * 
 * \return ::kvaDbOK (zero) if success
 * \return \ref kvaDbErr_xxx (negative) if failure
 * 
 * \sa kvaDbAddMsg()
 */
KvaDbStatus WINAPI kvaDbDeleteMsg(KvaDbHnd dh, KvaDbMessageHnd mh);

/**
 * Set the database flags.
 *
 * \note This feature is currently not enabled.
 * 
 * \param[in] dh     A database handle
 * \param[in] flags  The flags to set, \ref KVADB_MESSAGE_xxx
 * 
 * \return ::kvaDbOK (zero) if success
 * \return \ref kvaDbErr_xxx (negative) if failure
 * 
 * \sa kvaDbGetFlags()
 */
KvaDbStatus WINAPI kvaDbSetFlags(KvaDbHnd dh, unsigned int flags);

/**
 * Set the message name.
 * 
 * \param[in] mh   A message handle
 * \param[in] buf  The buffer that contains the message name.
 * 
 * \return ::kvaDbOK (zero) if success
 * \return \ref kvaDbErr_xxx (negative) if failure
 * 
 * \sa kvaDbGetMsgName()
 */
KvaDbStatus WINAPI kvaDbSetMsgName(KvaDbMessageHnd mh, char *buf);

/**
 * Set the message comment.
 * 
 * \param[in] mh   A message handle
 * \param[in] buf The buffer that contains the message comment
 * 
 * \return ::kvaDbOK (zero) if success
 * \return \ref kvaDbErr_xxx (negative) if failure
 * 
 * \sa kvaDbGetMsgComment()
 */
KvaDbStatus WINAPI kvaDbSetMsgComment(KvaDbMessageHnd mh, char *buf);

/**
 * Set the message identifier and flags.
 * 
 * \param[in] mh     A message handle
 * \param[in] id     The message identifier
 * \param[in] flags  The message flags, \ref KVADB_MESSAGE_xxx
 * 
 * \return ::kvaDbOK (zero) if success
 * \return \ref kvaDbErr_xxx (negative) if failure
 * 
 * \todo "flags" contains extended? or use 0x80000000 convention?
 *       could also contain J1939 flag, wakeup etc. qqq
 *
 * \sa kvaDbGetMsgId()
 */
KvaDbStatus WINAPI kvaDbSetMsgId(KvaDbMessageHnd mh, unsigned int id, unsigned int flags);

/**
 * Set the data length code for a message.
 * 
 * \param[in] mh   A message handle
 * \param[in] dlc  The data length code
 * 
 * \return ::kvaDbOK (zero) if success
 * \return \ref kvaDbErr_xxx (negative) if failure
 * 
 * \sa kvaDbGetMsgDlc()
 */
KvaDbStatus WINAPI kvaDbSetMsgDlc(KvaDbMessageHnd mh, int dlc);

// qqq pragma för att sätta sizeof(enum) till 4? annars skippa enum...

// J1939 fungerar hur? Helt enkelt genom att id == PGN?  Hur hantera
// SA, DA, etc? qqq

/**
 * Get the first signal in a message.
 * 
 * \param[in]  mh  A message handle
 * \param[out] sh  A handle to the first signal
 * 
 * \return ::kvaDbOK (zero) if success
 * \return \ref kvaDbErr_xxx (negative) if failure
 * 
 * \sa kvaDbGetNextSignal()
 */
KvaDbStatus WINAPI kvaDbGetFirstSignal(KvaDbMessageHnd mh, KvaDbSignalHnd *sh);

/**
 * Get the next signal in a message. Should be called after
 * kvaDbGetFirstSignal().
 * 
 * \param[in]  mh  A message handle
 * \param[out] sh  A handle to the next signal
 * 
 * \return ::kvaDbOK (zero) if success
 * \return \ref kvaDbErr_xxx (negative) if failure
 * 
 * \sa kvaDbGetFirstSignal
 */
KvaDbStatus WINAPI kvaDbGetNextSignal(KvaDbMessageHnd mh, KvaDbSignalHnd *sh);

/**
 * Get a signal handle by seaching for the signals's name.
 * 
 * \param[in]  mh   A message handle
 * \param[in]  buf  The buffer that contains the name of the signal to search for
 * \param[out] sh   A handle to the signal
 * 
 * \return ::kvaDbOK (zero) if success
 * \return \ref kvaDbErr_xxx (negative) if failure
 * 
 * \sa kvaDbGetFirstMsg()
 */
KvaDbStatus WINAPI kvaDbGetSignalByName(KvaDbMessageHnd mh, char *buf, KvaDbSignalHnd *sh);

/**
 * Convert data to a signal's physical (floating-point) value.
 * 
 * \param[in]  sh    A signal handle
 * \param[out] f     The converted value
 * \param[in]  data  The data buffer to be converted
 * \param[in]  len   The length of the data buffer
 * 
 * \return ::kvaDbOK (zero) if success
 * \return \ref kvaDbErr_xxx (negative) if failure
 * 
 * \sa kvaDbStoreSignalValuePhys()
 */
KvaDbStatus WINAPI kvaDbGetSignalValueFloat(KvaDbSignalHnd sh, double *f, void *data, size_t len);

/**
 * Convert data to a signal's raw (integer) value.
 * 
 * \param[in]  sh    A signal handle
 * \param[out] f     The converted value
 * \param[in]  data  The data buffer to be converted
 * \param[in]  len   The length of the data buffer
 * 
 * \return ::kvaDbOK (zero) if success
 * \return \ref kvaDbErr_xxx (negative) if failure
 * 
 * \sa kvaDbStoreSignalValueRaw()
 */
KvaDbStatus WINAPI kvaDbGetSignalValueInteger(KvaDbSignalHnd sh, int *f, void *data, size_t len);

/**
 * Get the minimum and maximum values from a signal.
 * 
 * \param[in]  sh      A signal handle
 * \param[out] minval  The minimum value
 * \param[out] maxval  The maximum value
 * 
 * \return ::kvaDbOK (zero) if success
 * \return \ref kvaDbErr_xxx (negative) if failure
 * 
 * \sa kvaDbSetSignalValueLimits()
 */
KvaDbStatus WINAPI kvaDbGetSignalValueLimits(KvaDbSignalHnd sh, double *minval, double *maxval);

/**
 * Get the scale factor and offset from a signal.
 * 
 * \param[in]  sh      A signal handle
 * \param[out] factor  The signal scale factor
 * \param[out] offset  The signal offset
 * 
 * \return ::kvaDbOK (zero) if success
 * \return \ref kvaDbErr_xxx (negative) if failure
 * 
 * \sa kvaDbSetSignalValueScaling()
 */
KvaDbStatus WINAPI kvaDbGetSignalValueScaling(KvaDbSignalHnd sh, double *factor, double *offset);

/**
 * Get the size and position of the signal value.
 * 
 * \param[in]  sh        A signal handle
 * \param[out] startbit  The bit where the signal value starts
 * \param[out] length    The length of the signal value in bits
 * 
 * \return ::kvaDbOK (zero) if success
 * \return \ref kvaDbErr_xxx (negative) if failure
 * 
 * \sa kvaDbSetSignalValueSize()
 */
KvaDbStatus WINAPI kvaDbGetSignalValueSize(KvaDbSignalHnd sh, int *startbit, int *length);

/**
 * Get the signal name.
 * 
 * \param[in]  sh      A signal handle
 * \param[out] buf     The signal name
 * \param[in]  buflen  The length of the buffer that will hold the signal name
 * 
 * \return ::kvaDbOK (zero) if success
 * \return \ref kvaDbErr_xxx (negative) if failure
 * 
 * \sa kvaDbSetSignalName()
 */
KvaDbStatus WINAPI kvaDbGetSignalName(KvaDbSignalHnd sh, char *buf, size_t buflen);

/**
 * Get the qualified signal name. which is the database, message ans signal
 * names separated by dots.
 * 
 * \param[in]  sh      A signal handle
 * \param[out] buf     The qualified signal name
 * \param[in]  buflen  The length of the buffer that will hold the qualified
 *                     signal name
 * 
 * \return ::kvaDbOK (zero) if success
 * \return \ref kvaDbErr_xxx (negative) if failure
 * 
 * \sa kvaDbGetSignalName()
 */
KvaDbStatus WINAPI kvaDbGetSignalQualifiedName(KvaDbSignalHnd sh, char *buf, size_t buflen);

/**
 * Get the signal comment.
 * 
 * \param[in]  sh      A signal handle
 * \param[out] buf     The signal comment
 * \param[in]  buflen  The length of the buffer that will hold the signal comment
 * 
 * \return ::kvaDbOK (zero) if success
 * \return \ref kvaDbErr_xxx (negative) if failure
 * 
 * \sa kvaDbSetSignalComment()
 */
KvaDbStatus WINAPI kvaDbGetSignalComment(KvaDbSignalHnd sh, char *buf, size_t buflen);

/**
 * Get the signal unit.
 * 
 * \param[in]  sh      A signal handle
 * \param[out] buf     The signal unit
 * \param[in]  buflen  The length of the buffer that will hold the signal unit
 * 
 * \return ::kvaDbOK (zero) if success
 * \return \ref kvaDbErr_xxx (negative) if failure
 * 
 * \sa kvaDbSetSignalUnit()
 */
KvaDbStatus WINAPI kvaDbGetSignalUnit(KvaDbSignalHnd sh, char *buf, size_t buflen);

/**
 * Get the signal encoding.
 * 
 * \param[in]  sh  A signal handle
 * \param[out] e   The signal encoding, Intel or Motorola, ::KvaDbSignalEncoding
 * 
 * \return ::kvaDbOK (zero) if success
 * \return \ref kvaDbErr_xxx (negative) if failure
 * 
 * \sa kvaDbSetSignalEncoding()
 */
KvaDbStatus WINAPI kvaDbGetSignalEncoding(KvaDbSignalHnd sh, KvaDbSignalEncoding *e);

/**
 * Get the signal representation type.
 * 
 * \param[in]  sh  A signal handle
 * \param[out] t   The signal representation type, ::KvaDbSignalType
 * 
 * \return ::kvaDbOK (zero) if success
 * \return \ref kvaDbErr_xxx (negative) if failure
 * 
 * \sa kvaDbSetSignalRepresentationType()
 */
KvaDbStatus WINAPI kvaDbGetSignalRepresentationType(KvaDbSignalHnd sh, KvaDbSignalType *t);

/**
 * Get the signal representation type.
 * 
 * \note This function is deprecated and will be replaced by
 * kvaDbGetSignalRepresentationType()
 * 
 * \param[in]  sh
 * \param[out] t   The signal representation type, ::KvaDbSignalType
 * 
 * \return ::kvaDbOK (zero) if success
 * \return \ref kvaDbErr_xxx (negative) if failure
 * 
 * \sa kvaDbSetSignalRepresentationType()
 */
KvaDbStatus WINAPI kvaDbGetSignalPresentationType(KvaDbSignalHnd sh, KvaDbSignalType *t);

/**
 * Add a new signal to a message.
 * 
 * \param[in]  mh  A message handle
 * \param[out] sh  A handle to the new signal
 * 
 * \return ::kvaDbOK (zero) if success
 * \return \ref kvaDbErr_xxx (negative) if failure
 * 
 * \sa kvaDbDeleteSignal()
 */
KvaDbStatus WINAPI kvaDbAddSignal(KvaDbMessageHnd mh, KvaDbSignalHnd *sh);

/**
 * Delete a signal from a message.
 * 
 * \param[in] mh  A message handle
 * \param[in] sh  A handle to the signal to delete
 * 
 * \return ::kvaDbOK (zero) if success
 * \return \ref kvaDbErr_xxx (negative) if failure
 * 
 * \sa kvaDbAddSignal()
 */
KvaDbStatus WINAPI kvaDbDeleteSignal(KvaDbMessageHnd mh, KvaDbSignalHnd sh);

/**
 * Set the value limits for a signal.
 * 
 * \param[in] sh      A signal handle
 * \param[in] minval  The minimum signal value
 * \param[in] maxval  The maximum signal value
 * 
 * \return ::kvaDbOK (zero) if success
 * \return \ref kvaDbErr_xxx (negative) if failure
 * 
 * \sa kvaDbGetSignalValueLimits()
 */
KvaDbStatus WINAPI kvaDbSetSignalValueLimits(KvaDbSignalHnd sh, 
                                             double minval, 
                                             double maxval);

/**
 * Set the scale factor and offset for a signal.
 * 
 * \param[in] sh      A signal handle
 * \param[in] factor  The scale factor for the signal value
 * \param[in] offset  The offset for the signal value
 * 
 * \return ::kvaDbOK (zero) if success
 * \return \ref kvaDbErr_xxx (negative) if failure
 * 
 * \sa kvaDbGetSignalValueScaling()
 */
KvaDbStatus WINAPI kvaDbSetSignalValueScaling(KvaDbSignalHnd sh, 
                                              double factor, 
                                              double offset);

/**
 * Set the value position and size for a signal.
 * 
 * \param[in] sh        A signal handle
 * \param[in] startbit  The starting bit of the signal's value
 * \param[in] length    The length of the value in bits
 * 
 * \return ::kvaDbOK (zero) if success
 * \return \ref kvaDbErr_xxx (negative) if failure
 * 
 * \sa kvaDbGetSignalValueSize()
 */
KvaDbStatus WINAPI kvaDbSetSignalValueSize(KvaDbSignalHnd sh, 
                                           int startbit, 
                                           int length);

/**
 * Set the signal name.
 * 
 * \param[in] sh   A signal handle
 * \param[in] buf  The buffer that contains the signal name
 * 
 * \return ::kvaDbOK (zero) if success
 * \return \ref kvaDbErr_xxx (negative) if failure
 * 
 * \sa kvaDbGetSignalName()
 */
KvaDbStatus WINAPI kvaDbSetSignalName(KvaDbSignalHnd sh, char *buf);

/**
 * Set the signal comment.
 * 
 * \param[in] sh   A signal handle
 * \param[in] buf  The buffer that contains the signal comment
 * 
 * \return ::kvaDbOK (zero) if success
 * \return \ref kvaDbErr_xxx (negative) if failure
 * 
 * \sa kvaDbGetSignalComment()
 */
KvaDbStatus WINAPI kvaDbSetSignalComment(KvaDbSignalHnd sh, char *buf);

/**
 * Set the signal unit.
 * 
 * \param[in] sh   A signal handle
 * \param[in] buf  The buffer that contains the unit for the signal
 * 
 * \return ::kvaDbOK (zero) if success
 * \return \ref kvaDbErr_xxx (negative) if failure
 * 
 * \sa kvaDbGetSignalUnit()
 */
KvaDbStatus WINAPI kvaDbSetSignalUnit(KvaDbSignalHnd sh, char *buf);

/**
 * Set the signal encoding.
 * 
 * \param[in] sh  A signal handle
 * \param[in] e   The signal encoding, Intel or Motorola, ::KvaDbSignalEncoding
 * 
 * \return ::kvaDbOK (zero) if success
 * \return \ref kvaDbErr_xxx (negative) if failure
 * 
 * \sa kvaDbGetSignalEncoding()
 */
KvaDbStatus WINAPI kvaDbSetSignalEncoding(KvaDbSignalHnd sh, KvaDbSignalEncoding e);

/**
 * Set the signal representation type.
 * 
 * \param[in] sh  A signal handle
 * \param[in] t   The signal representation type, ::KvaDbSignalType
 * 
 * \return ::kvaDbOK (zero) if success
 * \return \ref kvaDbErr_xxx (negative) if failure
 * 
 * \sa kvaDbGetSignalRepresentationType()
 */
KvaDbStatus WINAPI kvaDbSetSignalRepresentationType(KvaDbSignalHnd sh, KvaDbSignalType t);

/**
 * Get the first node.
 * 
 * \param[in]  dh  A database handle
 * \param[out] nh  A node handle
 * 
 * \return ::kvaDbOK (zero) if success
 * \return \ref kvaDbErr_xxx (negative) if failure
 * 
 * \sa kvaDbGetNextNode()
 */
KvaDbStatus WINAPI kvaDbGetFirstNode(KvaDbHnd dh, KvaDbNodeHnd *nh);

/**
 * Get the next node. Should be called after kvaDbGetFirstNode().
 * 
 * \param[in]  dh  A database handle
 * \param[out] nh  A handle to the next node
 * 
 * \return ::kvaDbOK (zero) if success
 * \return \ref kvaDbErr_xxx (negative) if failure
 * 
 * \sa kvaDbGetNodeByName()
 */
KvaDbStatus WINAPI kvaDbGetNextNode(KvaDbHnd dh, KvaDbNodeHnd *nh);

/**
 * Get a node by seaching for the node's name.
 * 
 * \param[in]  dh         A database handle
 * \param[in]  node_name  The buffer that contains the node name to search for
 * \param[out] nh         A node handle
 * 
 * \return ::kvaDbOK (zero) if success
 * \return \ref kvaDbErr_xxx (negative) if failure
 * 
 * \sa kvaDbGetNodeAttributeIntByName()
 */
KvaDbStatus WINAPI kvaDbGetNodeByName(KvaDbHnd dh, const char *node_name, KvaDbNodeHnd *nh);

/**
 * Get the value of a node attribute as an integer.
 * 
 * \param[in]  nh        A node handle
 * \param[in]  attrName  The buffer that contains the attribute name
 * \param[out] val       The value of the attribute
 * 
 * \return ::kvaDbOK (zero) if success
 * \return \ref kvaDbErr_xxx (negative) if failure
 * 
 * \sa kvaDbGetFirstNode()
 */
KvaDbStatus WINAPI kvaDbGetNodeAttributeIntByName(KvaDbNodeHnd nh, const char *attrName, int *val);

/**
 * Add a new node to a database.
 * 
 * \param[in]  dh  A database handle
 * \param[out] nh  A handle to the new node
 * 
 * \return ::kvaDbOK (zero) if success
 * \return \ref kvaDbErr_xxx (negative) if failure
 */
KvaDbStatus WINAPI kvaDbAddNode(KvaDbHnd dh, KvaDbNodeHnd *nh);

/**
 * Delete a node from a database.
 * 
 * \param[in] dh  A database handle
 * \param[in] nh  A handle to the node that will be deleted
 * 
 * \return ::kvaDbOK (zero) if success
 * \return \ref kvaDbErr_xxx (negative) if failure
 */
KvaDbStatus WINAPI kvaDbDeleteNode(KvaDbHnd dh, KvaDbNodeHnd nh);

/**
 * Set the node name.
 * 
 * \param[in] nh   A node handle
 * \param[in] buf  The buffer that contains the node name.
 * 
 * \return ::kvaDbOK (zero) if success
 * \return \ref kvaDbErr_xxx (negative) if failure
 */
KvaDbStatus WINAPI kvaDbSetNodeName(KvaDbNodeHnd nh, char *buf);

/**
 * Set the node comment.
 * 
 * \param[in] nh   A node handle
 * \param[in] buf  The buffer that contains the node comment
 * 
 * \return ::kvaDbOK (zero) if success
 * \return \ref kvaDbErr_xxx (negative) if failure
 */
KvaDbStatus WINAPI kvaDbSetNodeComment(KvaDbNodeHnd nh, char *buf);

/**
 * Add a receiving node to a signal.
 * 
 * \param[in] sh   A signal handle
 * \param[in] nh   A node handle
 * 
 * \return ::kvaDbOK (zero) if success
 * \return \ref kvaDbErr_xxx (negative) if failure
 */
KvaDbStatus WINAPI kvaDbAddReceiveNodeToSignal(KvaDbSignalHnd sh, KvaDbNodeHnd nh);

/**
 * Remove a receiving node from a signal.
 * 
 * \param[in] sh   A signal handle
 * \param[in] nh   A node handle
 * 
 * \return ::kvaDbOK (zero) if success
 * \return \ref kvaDbErr_xxx (negative) if failure
 */
KvaDbStatus WINAPI kvaDbRemoveReceiveNodeFromSignal(KvaDbSignalHnd sh, KvaDbNodeHnd nh);

/**
 * Convert a physical signal value to data bytes.
 * 
 * \param[in]  sh        A signal handle
 * \param[out] can_data  The buffer that will hold the converted value
 * \param[in]  dlc       The length of the data buffer
 * \param[in]  value     The physical signal value to convert
 * 
 * \return ::kvaDbOK (zero) if success
 * \return \ref kvaDbErr_xxx (negative) if failure
 * 
 * \sa kvaDbGetSignalValueFloat()
 */
KvaDbStatus WINAPI kvaDbStoreSignalValuePhys(KvaDbSignalHnd sh, unsigned char *can_data, int dlc, double value);

/**
 * Convert a raw signal value to data bytes.
 * 
 * \param[in]  sh        A signal handle
 * \param[out] can_data  The buffer that will hold the converted value
 * \param[in]  dlc       The length of the data buffer
 * \param[in]  value     The raw signal value to convert
 * 
 * \return ::kvaDbOK (zero) if success
 * \return \ref kvaDbErr_xxx (negative) if failure
 * 
 * \sa kvaDbGetSignalValueInteger()
 */
KvaDbStatus WINAPI kvaDbStoreSignalValueRaw(KvaDbSignalHnd sh, unsigned char *can_data, int dlc, int value);

#ifdef __cplusplus
}
#endif

/** @} */

#endif //KVADBLIB_H
