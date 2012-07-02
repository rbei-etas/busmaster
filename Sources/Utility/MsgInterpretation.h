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
 * \file      MsgInterpretation.h
 * \brief     This file contain the definition of CMsgInterpretation class
 * \authors   Amitesh Bharti, Amarnath Shastry
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * This file contain the definition of CMsgInterpretation class
 */

#pragma once

#include "Include/Basedefs.h"
#include "Datatypes/SigWatch_datatypes.h" // Signal list selected
#include "Datatypes/MsgSignal_Datatypes.h"


class CMsgInterpretationJ1939
{
private:
    SMSGENTRY*  m_psMsgRoot;
    EFORMAT     m_eNumFormat;
public:
    CMsgInterpretationJ1939();
    ~CMsgInterpretationJ1939();
    // Get signal value
    __int64 n64GetSignalValue(CByteArray*, UINT, UINT, UINT, BYTE, EFORMAT_DATA);

    /* To set the message list root. J1939 database needs to be set before calling the above
    function */
    void vSetJ1939Database(const SMSGENTRY* psCurrMsgEntry);
    int nGetJ1939SignalCount(CString strMsgName);

    /* Interpretation function for J1939 msgs.
    Given the PGN it parses the data and calculates signal values */
    BOOL bInterPretJ1939_MSGS(EFORMAT eNumformat, UINT32 unPGN, UINT unDLC, BYTE* pbyData,
                              CString& omMsgName,
                              SSignalInfoArray& odSigInfoArray);

    void vCopy(CMsgInterpretationJ1939* pDest) const;
    void vClear();
};

class CMsgInterpretation
{
private:
    EFORMAT     m_eNumFormat;
    SMSGENTRY*  m_psMsgRoot;

public:
    // Get signal value
    __int64 n64GetSignalValue(CByteArray*, UINT, UINT, UINT, BYTE, EFORMAT_DATA);

    // To set the message list root
    void vSetMessageList(SMSGENTRY* psCurrMsgEntry);
    int nGetSignalCount(CString strMsgName);

    // To set the current numeric mode for formatting
    void vSetCurrNumMode(EFORMAT eNumFormat);


    // This will interpret all the signals
    BOOL vInterpretMsgs(UINT unMsgCode,const UCHAR* ucData,CString& omStrMsgName,
                        CStringArray& omStrSigNames,
                        CStringArray& omStrRawValues,
                        CStringArray& omStrPhyValues, BOOL bHexON);
    // This is for Signal Watch List
    BOOL bInterpretMsgSigList(UINT unMsgCode,
                              const UCHAR* ucData,CString& omStrMsgName,
                              CStringArray& omStrSigNames,
                              CStringArray& omStrRawValues,
                              CStringArray& omStrPhyValues,//with Unit
                              BOOL bHexON);
    // This will interpret message and fill data value. This will not format
    // any value
    BOOL vInterpretMsgs(UINT unMsgCode,
                        const UCHAR* ucData, CSignalInfoArray& omSignalInfo );

    //VENKAT
    BOOL bInterpretMsgs(UINT unMsgCode,
                        const UCHAR* ucData,
                        CSignalInfoArray& omSignalInfo );

    // This will interpret all the signals
    BOOL bInterpretMsgs(UINT unMsgCode, const UCHAR* ucData,
                        CString& omStrMsgName, CStringArray& omStrSigNames,
                        CStringArray& omStrRawValues, CStringArray& omStrPhyValues);

    // The same function with a different prototype
    BOOL bInterpretMsgs(EFORMAT eNumFormat, SMSGENTRY* psMsgRoot,
                        UINT unMsgCode, const UCHAR* ucData,
                        CString& omStrMsgName, SSignalInfoArray& SigInfoArray);
    // The same function with a different prototype for interpreting
    //individual message, provided the msgdetail pointer
    BOOL bInterpretMsgs(EFORMAT eNumFormat, const sMESSAGE* pMsg,
                        const UCHAR* ucData, SSignalInfoArray& SigInfoArray);
    //The same function with a different prototype for interpreting
    //individual message, provided the msgdetail pointer
    BOOL bInterpretMsgs(EFORMAT /*eNumFormat*/,
                        const sMESSAGE* pMsg,
                        const UCHAR* ucData,
                        CSignalInfoArray& omSigInfoArray);

    CMsgInterpretation();
    virtual ~CMsgInterpretation();
public:
    void vCopy(CMsgInterpretation* pDest) const;
    void vClear();
};
