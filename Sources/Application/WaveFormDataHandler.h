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
 * \file      WaveFormDataHandler.h
 * \brief     Interface file for CWaveformDataHandler class
 * \author    Arunkumar Karri
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Interface file for CWaveformDataHandler class
 */

#pragma once

#include "DataTypes/MsgSignal_DataTypes.h"
#include "DataTypes/MainSubEntry.h" 

class CWaveFormDataHandler
{
public:
	CWaveFormDataHandler(void);
	~CWaveFormDataHandler(void);

	//Get complete list of messages.
	void vGetCompleteMsgList(CStringArray& arrMsgList) const;

	//Get default wave pattern values. 
	void vGetDefaultSignalWaveValues(sWaveformInfo& stDefaultWaveInfo) const;
	//Set default wave pattern values. 
	void vSetDefaultSignalWaveValues(const sWaveformInfo& stDefaultWaveInfo);

	// Get default signal values of a message.
	float fGetMsgIDDefaultValue(UINT nMsgID);

	//Set default signal values of a message.
	void vSetMsgIDDefaultValue(UINT nMsgID, float fDefaultValue);

	//Given a Message ID, get complete list of available signals associated with it.
	void vGetAvailableSignalsInMsgID(UINT& nMsgID, CStringArray& arrAvailableSignals, 
									 bool bExcludeDefinedSignals = true);
	//Given a Message ID, get complete list of defined signals associated with it.
	void vGetDefinedSignalsInMsgID(UINT unMsgID, CStringArray& omDefinedSignals);

	//Move a signal from available list into defined list.
	BOOL bAddSignalToDefinedWavesList(UINT unMsgID, CString strSignalName, 
		                              sWaveformInfo& stSigWaveInfo);
	//Move a signal from defined list into available list.
	BOOL bRemoveSignalFromDefinedWavesList(UINT unMsgID, CString omSignalName);

	//Get wave pattern details of a signal in defined list.
	BOOL bGetSignalWavePatternDetails(UINT nMsgID, CString strSignalName, 
		                              sWaveformInfo& stSigWaveInfo);
	//Set wave pattern details of a signal in defined list.
	void vSetSignalWavePatternDetails(UINT unMsgID, CString omSignalName, 
		                              const sWaveformInfo& stSigWaveInfo);

	// Confirm the changes. 
	void vCommit(void);
	// Rollback.
	void vRollback(void);

	// Get complete list of defined signal names.
	void vGetAllDefinedSignalsNames(CStringArray& omSignalNames);
	// Copy the defined waveform signal list data structure
    void GetSigGenerationInfoList(CSigGenerationInfoList& Target);
	//Set Complete list of messages.
	void vSetCompleteMsgList(CMainEntryList* podMsgIDList);

	//Get Sampling Time Period.
	short shGetSamplingTimePeriod();
	//Set Sampling Time Period.
	void vSetSamplingTimePeriod(short shSamplingTimePeriod);

	//Get Config Data.
	HRESULT GetConfigData(BYTE** ppvDataStream, UINT& unLength);
	//Set Config Data.
	HRESULT SetConfigData(BYTE* pvDataStream);

	//Gets the Count of defined signals.
	UINT nGetNumberOfDefinedSignals(void);

	//Clear signal list data
	void vClearSignalInfoList(void);

private:
	CSigGenerationInfoList m_lstSignalWaveformInfo;
	CSigGenerationInfoList m_lstTempSignalWaveformInfo;
	sWaveformInfo m_sDefaultWaveInfo;
	CStringArray m_omMsgIDList;
	CMainEntryList* m_podMsgEntyList;	
	short m_shSamplingTP;

    // Given the waveform code, this returns the waveform name string
    CString omGetWaveformName(eWAVEFORMTYPE eWaveform);
    // Getter for message entry, given the message ID and the source list
    SSigGeneration* psGetMsgEntryFromList(UINT unMsgID,
                                          CSigGenerationInfoList& omCurrList);
    // Getter for signal info entry, given the message ID, signal name and source list
    sWaveformInfo* psGetSignalEntryFromList(UINT MsgID, CString omSignalName, 
                                           CSigGenerationInfoList& omCurrList);
    // Getter for signal info entry, given the signal name and message entry
    sWaveformInfo* psGetSignalEntryFromMsgEntry(CString omSignalName, 
                                                SSigGeneration& ouMsgEntry);
    // Getter for signal wave entry, given the signal wave list
    sSigWaveMap* psGetSignalEntry(CString omSignalName, CSigWaveMapList& omSigWaveList);
    // Set (add / modify) a signal wave entry, given the signal wave list
    BOOL bSetSignalWaveEntry(sSigWaveMap ouSignalEntry, CSigWaveMapList& omSigWaveList);

    // If the message and signal exist in the defined list
	BOOL bIsSignalInMsgIDDefined(UINT unMsgID, CString strSignal);
    // To calculate the granularity for a waveform
    float fCalculateGranularity(sWaveformInfo& sCurrWaveInfo, short shSamplingTP);

	//Return the Size of CSigGenerationInfoList list in BYTES.
	UINT nGetWaveInfoListSize();
	BYTE* pbyGetListConfigData(BYTE* pByteTrgt, const UINT nSize);

	bool bIsSignalInMsgFoundInDB(UINT& nMsgID, CString& strSignalName);
};
