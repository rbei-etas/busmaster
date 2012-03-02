
/******************************************************************************
  Project       :  Auto-SAT_Tools
  FileName      :  FrameProcessor_J1939.h
  Description   :  Definition file for CFrameProcessor_J1939 class.
  $Log:   X:/Archive/Sources/FrameProcessor/FrameProcessor_J1939.h_v  $
 * 
 *    Rev 1.2   02 Dec 2011 20:16:50   rac2kor
 * Removed hard coding of version information 
 * string in the log file by accepting it as a parameter
 * from the application / client.
 * 
 *    Rev 1.1   15 Apr 2011 19:20:32   rac2kor
 * Inserted RBEI Copyright information text into the file header.
 * 
 *    Rev 1.0   06 Dec 2010 18:53:02   rac2kor
 *  

  Author(s)     :  Ratnadip Choudhury
  Date Created  :  1.12.2010
  Modified By   :  
  Copyright (c) 2011, Robert Bosch Engineering and Business Solutions.  All rights reserved.
******************************************************************************/

#if !defined FRAMEPROCESSOR_J1939_H__INCLUDED_
#define FRAMEPROCESSOR_J1939_H__INCLUDED_


#include "BaseFrameProcessor_J1939.h"
#include "FrameProcessor_Common.h"
#include "Format/FormatMsgJ1939.h"
#include "DIL_Interface/BaseDIL_CAN.h"
#include "DIL_Interface/BaseDIL_J1939.h"

class CFrameProcessor_J1939 : public CBaseFrameProcessor_J1939, CFrameProcessor_Common
{
private:
    CParamLoggerJ1939	m_sJ1939ProcParams;
    CBaseDILI_J1939*    m_pouDIL_J1939;
	CMsgBufVSE			m_ouVSEBufJ1939;
    STJ1939_MSG         m_sJ1939Data;
    BYTE*               m_pbyJ1939Data;
    SFORMATTEDATA_J1939 m_sCurrFormatDat;
    CFormatMsgJ1939     m_ouFormatMsgJ1939;

    USHORT ushCalculateStrLen(bool bForHex, USHORT ushLength);
    void vEmptyLogObjArray(CLogObjArray& omLogObjArray);
	// To create a new logging object
	CBaseLogObject* CreateNewLogObj(void);
    // To delete a logging object
    void DeleteLogObj(CBaseLogObject*& pouLogObj);
    // To create the time mode mapping
	void CreateTimeModeMapping(SYSTEMTIME& CurrSysTime, UINT64& unAbsTime);


public:

    CFrameProcessor_J1939();      // Constructor
    ~CFrameProcessor_J1939();     // Destructor

	BOOL InitInstance(void);
    int ExitInstance(void);
	void vRetrieveDataFromBuffer(void);

    /* STARTS IMPLEMENTATION OF THE INTERFACE FUNCTIONS... */
    // To initialise this module
    HRESULT FPJ1_DoInitialisation(CParamLoggerJ1939* psInitParams);

    // To modify the filtering scheme of a logging block
    HRESULT FPJ1_ApplyFilteringScheme(USHORT ushLogBlkID,
                                     const SFILTERAPPLIED_J1939& sFilterObj);

    // Getter for the filtering scheme of a logging block
    HRESULT FPJ1_GetFilteringScheme(USHORT ushLogBlk, 
                                   SFILTERAPPLIED_J1939& sFilterObj);


    /* USE COMMON BASE CLASS ALIAS FUNCTIONS */

    /* Call to enable/disable logging for a particular block. Having ushBlk equal
    to FOR_ALL, signifies the operation to be performed for all the blocks */
    HRESULT FPJ1_EnableLoggingBlock(USHORT ushBlk, BOOL bEnable);

    // To enable/disable logging
    HRESULT FPJ1_EnableLogging(BOOL bEnable);

    /* Call to enable/disable logging for a particular block. Having ushBlk equal
    to FOR_ALL, signifies the operation to be performed for all the blocks */
    HRESULT FPJ1_EnableFilter(USHORT ushBlk, BOOL bEnable);

    // Query function - current logging status (OFF/ON). 
    BOOL FPJ1_IsLoggingON(void);

    // Query function - current filtering status
    BOOL FPJ1_IsFilterON(void);

    // To log a string
    HRESULT FPJ1_LogString(CString& omStr);

    // To add a logging block; must be in editing mode
    HRESULT FPJ1_AddLoggingBlock(const SLOGINFO& sLogObject);

    // To remove a logging block by its index in the list; editing mode prerequisite
    HRESULT FPJ1_RemoveLoggingBlock(USHORT ushBlk);

    // Getter for total number of logging blocks
    USHORT FPJ1_GetLoggingBlockCount(void);

    // To clear the logging block list
    HRESULT FPJ1_ClearLoggingBlockList(void);

    // Getter for a logging block by specifying its index in the list
    HRESULT FPJ1_GetLoggingBlock(USHORT ushBlk, SLOGINFO& sLogObject);

    // Setter for a logging block by specifying its index in the list
    HRESULT FPJ1_SetLoggingBlock(USHORT ushBlk, const SLOGINFO& sLogObject);

    // To reset or revoke the modifications made
    HRESULT FPJ1_Reset(void);

    // To confirm the modifications made
    HRESULT FPJ1_Confirm(void);

    // To start logging block editing session
    HRESULT FPJ1_StartEditingSession(void);

    // To stop logging block editing session
    HRESULT FPJ1_StopEditingSession(BOOL bConfirm);

    // Getter for the logging configuration data
    HRESULT FPJ1_GetConfigData(BYTE** ppvConfigData, UINT& unLength);

    // Setter for the logging configuration data
    HRESULT FPJ1_SetConfigData(BYTE* pvDataStream);

    /* ENDS IMPLEMENTATION OF THE INTERFACE FUNCTIONS... */
};

#endif // FRAMEPROCESSOR_J1939_H__INCLUDED_
