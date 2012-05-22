
/******************************************************************************
  Project       :  Auto-SAT_Tools
  FileName      :  LogObjectJ1939.h
  Description   :  Definition file for CLogObjectJ1939 class.
  $Log:   X:/Archive/Sources/FrameProcessor/LogObjectJ1939.h_v  $
 * 
 *    Rev 1.2   02 Dec 2011 20:16:50   rac2kor
 * Removed hard coding of version information 
 * string in the log file by accepting it as a parameter
 * from the application / client.
 * 
 *    Rev 1.1   15 Apr 2011 19:20:38   rac2kor
 * Inserted RBEI Copyright information text into the file header.
 * 
 *    Rev 1.0   06 Dec 2010 18:53:02   rac2kor
 *  

  Author(s)     :  Ratnadip Choudhury
  Date Created  :  30.11.2010
  Modified By   :  
  Copyright (c) 2011, Robert Bosch Engineering and Business Solutions.  All rights reserved.
******************************************************************************/

#if !defined LOGOBJECTJ1939_H__INCLUDED_
#define LOGOBJECTJ1939_H__INCLUDED_

#include "include/BaseDefs.h"
#include "DataTypes/Log_Datatypes.h"
#include "DataTypes/Filter_Datatypes.h"
#include "BaseLogObject.h"
#include "DataTypes/J1939_DataTypes.h"


class CLogObjectJ1939 : public CBaseLogObject
{
private:
	// The filter object
	SFILTERAPPLIED_J1939 m_sFilterApplied;

    // To format the header 
    void vFormatHeader(CString& omHeader);

    // To format the footer 
    void vFormatFooter(CString& omFooter);

	BOOL bToBeLogged(SFRAMEINFO_BASIC_J1939& J1939Info_Basic);

protected:
    // To copy specific data pertaining to the conrete class. 
    void Der_CopySpecificData(const CBaseLogObject* pouLogObjRef);
    // Set configuration data - concrete class specific logics
    BYTE* Der_SetConfigData(BYTE* pvDataStream);
    // Get configuration data - concrete class specific logics
    BYTE* Der_GetConfigData(BYTE* pvDataStream) const;
    // To get the total buffer size - concrete class specific logics
	UINT Der_unGetBufSize(void) const;

public:

	CStringArray		m_omListDBFiles;
	SCONTROLER_DETAILS*	m_pasControllerDetails;
	int					m_nNumChannels;

    CLogObjectJ1939(CString omVersion);    // Overloaded constructor
    ~CLogObjectJ1939();   // Destructor

    // Log a J1939 data object
    BOOL bLogData(const SFORMATTEDATA_J1939&);

    // Enable / disable filter
    void EnableFilter(BOOL bEnable);

    // Get the filter info structure
    void GetFilterInfo(SFILTERAPPLIED_J1939& sFilterInfo) const;
    // Set the filter info structure
    void SetFilterInfo(const SFILTERAPPLIED_J1939& sFilterInfo);
	
	// Get the list of database files associated
	void Der_GetDatabaseFiles(CStringArray& omList);
	// Set the list of database files associated
	void Der_SetDatabaseFiles(const CStringArray& omList);
	// Set the baud rate details for each channel
	void Der_SetChannelBaudRateDetails(SCONTROLER_DETAILS* controllerDetails, 
										int nNumChannels);
	// To get the channel baud rate info for each channel
	void Der_GetChannelBaudRateDetails(SCONTROLER_DETAILS* controllerDetails,
											int& nNumChannels);

};

#endif // LOGOBJECTJ1939_H__INCLUDED_
