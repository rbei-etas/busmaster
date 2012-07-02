/*
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/**
 * \file      Error.h
 * \brief     This contains various error code definitions.
 * \authors   Ratnadip Choudhury, Anish Kumar, Pradeep Kadoor
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * This contains various error code definitions.
 */

#pragma once

const HRESULT CALL_SUCCESS          = 0;

// Bits 31-30 denote severity, 29th bit is microsoft / customer specific, 28th is reserved

const HRESULT MASK_INFO    = 0x60000000;
const HRESULT MASK_WARN    = 0xA0000000;
const HRESULT MASK_ERR     = 0xE0000000;

/* We keep the 6th and 7th nibbles to denote the module */

const HRESULT ERRLGR       = 0x00100000; // Error Logger
const HRESULT LVLDTR       = 0x00200000; // License Validator
const HRESULT UTLLIB       = 0x00300000; // Utility Library
const HRESULT DATLIB       = 0x00400000; // Datatype library
const HRESULT SIMENG       = 0x00500000; // Simulation Engine
const HRESULT PRJCNF       = 0x00600000; // Project configuration library
const HRESULT DILLIB       = 0x00700000; // Driver Interface Library
const HRESULT FRMGUI       = 0x00800000; // Main GUI
const HRESULT CNFDIL       = 0x00900000; // Configuration Dialog at DIL level
const HRESULT FIBXED       = 0x00A00000; // Fibex Editor
const HRESULT COMMON       = 0x00B00000; // Common messages
const HRESULT TRALIB       = 0x00C00000; // MCNet Translator lib
const HRESULT PUILIB       = 0x00D00000; // PUI Library
//venkatanarayana
const HRESULT TSELIB       = 0x00E00000; // TestSetupEditor Library
const HRESULT TSEGUI       = 0x00F00000; // TestSetupEditor GUI
const HRESULT TSEXEC       = 0x01000000; // TestSuite Executor



/* STARTS DIL RELATED ERROR MESSAGES */
const HRESULT NO_LICENSE            = MASK_ERR | DILLIB | 0x01; // License unavailable
const HRESULT DAL_INVALID           = MASK_ERR | DILLIB | 0x02; // Invalid DAL id
const HRESULT DATREP_INVALID        = MASK_ERR | DILLIB | 0x03; // Invalid data repository address
const HRESULT ERRLGR_INVALID        = MASK_ERR | DILLIB | 0x04; // Invalid error logger interface
const HRESULT ERR_LOAD_DRIVER       = MASK_ERR | DILLIB | 0x05; // Error in loading driver library
const HRESULT DAL_ALREADY_SELECTED  = MASK_INFO| DILLIB | 0x06; // DAL already selected
const HRESULT NO_HW_INTERFACE       = MASK_ERR | DILLIB | 0x07; // Hardware interface doesn't exist
const HRESULT ERR_LOAD_HW_INTERFACE = MASK_ERR | DILLIB | 0x08; // Error in hardware interface loading
const HRESULT HW_INTERFACE_NO_SEL   = MASK_WARN| DILLIB | 0x09; // User didn't choose any hardware interface
const HRESULT CONTROLLER_NO_CONFIG  = MASK_WARN| DILLIB | 0x0A; // User didn't configure the controller.
const HRESULT ERROR_OTHER           = MASK_ERR | DILLIB | 0x0B; // Other type of error.
const HRESULT WARN_DUMMY_API        = MASK_WARN| DILLIB | 0x0C; // Called an API of the dummy interface.
const HRESULT E_USER_RESPONSE_WRONG = MASK_ERR | DILLIB | 0x0D; // User's response is wrong
const HRESULT HW_INTERFACE_ALREADY_SELECTED
    = MASK_INFO| DILLIB | 0x0E; // Hardware interface already selected
const HRESULT HW_INTERFACE_ALREADY_CLOSED
    = MASK_INFO| DILLIB | 0x0F; // Hardware interfaces already closed
const HRESULT ERR_CLOSE_HW_INTERFACE= MASK_ERR | DILLIB | 0x10; // Err in closing the hardware interface
const HRESULT INFO_CHANGED_INITDATA = MASK_INFO| DILLIB | 0x11; // Initialization data changed
const HRESULT WARN_INITDAT_NCONFIRM = MASK_WARN| DILLIB | 0x12; // Controller initialisation data not confirmed
const HRESULT INFO_INITDAT_RETAINED = MASK_INFO| DILLIB | 0x13; // Controller initialisation data retained
const HRESULT INFO_INITDAT_CONFIRM_CONFIG
    = MASK_INFO| DILLIB | 0x14; // Controller initialisation data confirmed and controller successfully configured
const HRESULT ERR_INITDAT_CONFIRM_CONFIG
    = MASK_ERR | DILLIB | 0x15; // Controller initialisation data confirmed but failure in controller configuration
const HRESULT ERR_CLIENT_EXISTS     = MASK_ERR | DILLIB | 0x16; // Client already exists
const HRESULT ERR_NO_MORE_CLIENT_ALLOWED    = MASK_ERR | DILLIB | 0x17; // Maximum number of clients already registered. No more allowed.
const HRESULT ERR_NO_CLIENT_EXIST           = MASK_ERR | DILLIB | 0x18; // No client exists
const HRESULT ERR_IMPROPER_STATE            = MASK_ERR | DILLIB | 0x19; // Function called at improper state.
const HRESULT ERR_INVALID_CHANNEL           = MASK_ERR | DILLIB | 0x20; // Invalid channel number specified
const HRESULT ERR_BUFFER_EXISTS             = MASK_ERR | DILLIB | 0x21; // Buffer already exists for the current client

/* ENDS DIL RELATED ERROR MESSAGES */

/* STARTS DATATYPE RELATED ERROR MESSAGES */
const HRESULT EMPTY_APP_BUFFER             =  MASK_ERR | DATLIB | 0x01;
const HRESULT WARN_BUFFER_SIZE_MIN_ASSUMED =  MASK_WARN| DATLIB | 0x02;
const HRESULT WARN_BUFFER_SIZE_MAX_ASSUMED =  MASK_WARN| DATLIB | 0x03;
const HRESULT WARN_BUFFER_OVERRUN          =  MASK_WARN| DATLIB | 0x04;
const HRESULT ERR_READ_MEMORY_SHORT        =  MASK_ERR | DATLIB | 0x05;
const HRESULT ERR_WRITE_MSG_TOO_LARGE      =  MASK_ERR | DATLIB | 0x06;
const HRESULT ERR_WRITE_INVALID_SIZE       =  MASK_ERR | DATLIB | 0x07;
const HRESULT ERR_CREATE_MEMORY_FAIL       =  MASK_ERR | DATLIB | 0x08;
const HRESULT ERR_FULL_APP_BUFFER          =  MASK_ERR | DATLIB | 0x09;
const HRESULT ERR_INVALID_INDEX            =  MASK_ERR | DATLIB | 0x0A;
const HRESULT ERR_INVALID_SLOT             =  MASK_ERR | DATLIB | 0x0B;
const HRESULT ERR_NOT_SUPPORTED            =  MASK_ERR | DATLIB | 0x0C;


/* ENDS DATATYPE RELATED ERROR MESSAGES *
/* STARTS COMMON MESSAGES */
const HRESULT INVALID_PARENTWND     = MASK_WARN| COMMON | 0x01; // Parent window handle null
const HRESULT DLL_ALREADY_LOADED    = MASK_WARN| COMMON | 0x02; // Dll already loaded
const HRESULT ERR_MISMATCH_SIZE_ARG = MASK_ERR | COMMON | 0x03; // Size mismatch in parameter byte stream.
const HRESULT ERR_NO_DYNAMIC_MEM    = MASK_ERR | COMMON | 0x04; // Sufficient memory not available in heap.
const HRESULT ERR_EXCEED_PERMITTED_VALUE
    = MASK_ERR | COMMON | 0x05; // Value in question exceeds limit
const HRESULT ERR_READ_PERMSN       = MASK_ERR | COMMON | 0x06; // No read permission
const HRESULT ERR_WRITE_PERMSN      = MASK_ERR | COMMON | 0x07; // No write permission
const HRESULT ERR_PATH_TMPFILE      = MASK_ERR | COMMON | 0x08; // Problem in retrieving path for temporary files: system problem
const HRESULT ERR_OPEN_FILE         = MASK_ERR | COMMON | 0x09; // Error in opening of file.

/* ENDS COMMON MESSAGES */


/* STARTS MCNet TRANSLATOR MESSAGES */
const HRESULT ALREADY_REGISTERED = MASK_WARN| TRALIB | 0x01; //Client already regd
const HRESULT ALLOWED_NOMORE     = MASK_ERR | TRALIB | 0x02; //Client number reached max
const HRESULT ERROR_NOCLIENT     = MASK_ERR | TRALIB | 0x03; //No client available for LCN
const HRESULT DATAREP_INVALID    = MASK_ERR | TRALIB | 0x04; //MsgBuff is NULL
const HRESULT NOT_PERMITTED      = MASK_ERR | TRALIB | 0x05; //Watch dog not permitted for the node
const HRESULT INVALID_PARAM      = MASK_ERR | TRALIB | 0x06; //Parameters not valid

/* ENDS MCNet TRANSLATOR MESSAGES */


/* STARTS PUI LIBRARY MESSAGES */
const HRESULT ERR_KEYLEN_INAPPR  = MASK_ERR | PUILIB | 0x01; // Key length inappropriate
const HRESULT ERR_NOT_INSTALLED  = MASK_ERR | PUILIB | 0x02; // Application not installed
const HRESULT ERR_INST_LOWERVER  = MASK_ERR | PUILIB | 0x03; // Lower version installed
const HRESULT ERR_VAL_ACT_LOCK   = MASK_ERR | PUILIB | 0x04; // Error in validation between activation and licence keys
const HRESULT ERR_VAL_TIME       = MASK_ERR | PUILIB | 0x05; // Problem in system time
const HRESULT ERR_MISMATCH_CREATIONTIME = MASK_ERR | PUILIB | 0x06; //Mismatch in ADS and File Creation time
/* ENDS   PUI LIBRARY MESSAGES */

//venkatanarayana
/* STARTS TESTSETUPEDITOR LIB MESSAGES */
const HRESULT ERR_NOT_IMPLEMENTED  = MASK_ERR | TSELIB | 0x01; // The defintion of the function was not implemented(usually base class)
const HRESULT ERR_INVALID_DATABASE = MASK_ERR | TSELIB | 0x02; // The file format of the specified database is not compatiable or wrong
const HRESULT ERR_VALID_SUCCESS    = MASK_ERR | TSELIB | 0x03;
const HRESULT ERR_VALID_WARNING    = MASK_ERR | TSELIB | 0x04;
const HRESULT ERR_VALID_ERROR      = MASK_ERR | TSELIB | 0x05;
/* END TESTSETUPEDITOR LIB MESSAGES */

/* STARTS TESTSETUPEDITOR GUI MESSAGES */
const HRESULT ERR_INVALID_ENTITY   = MASK_ERR | TSEGUI | 0x03;
/* END TESTSETUPEDITOR GUI MESSAGES */

/*STARTS TESTSUITE EXECUTOR MESSAGES */
const HRESULT ERR_PATH_INCORRECT   = MASK_ERR | TSEXEC | 0X01; // Incorrect File path
const HRESULT ERR_WRONG_ID         = MASK_ERR | TSEXEC | 0X02;
const HRESULT ERR_WRONG_ID_REF     = MASK_ERR | TSEXEC | 0X03;
const HRESULT ERR_WRONG_INDEX      = MASK_ERR | TSEXEC | 0x04;  // Wrong Index
/*ENDS TESTSUITE EXECUTOR MESSAGES */
