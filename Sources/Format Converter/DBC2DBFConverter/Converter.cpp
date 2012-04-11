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
 * \file      Converter.cpp
 * \brief     Implementation of converter class
 * \author    Amitesh Bharti, Tobias Lorenz
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Implementation of the converter class.
 */

#include "Converter.h"
#include "Signal.h"
#include "Tag.h"

using namespace std;

bool CConverter::valid_msg = true;
unsigned char CConverter::ucMsg_DLC = 8;


/**
 * \brief Constructor
 */
CConverter::CConverter()
{
    m_uiResultCode = CON_RC_NOERROR;
}


/**
 * \brief      Destructor
 */
CConverter::~CConverter()
{
    m_listMessages.clear();
}


/**
 * This is the basic function which is to be called
 * to convert any given CANoe file to a CANMon file.
 */
unsigned int CConverter::Convert(string sCanoeFile, string sCanMonFile)
{
    fstream fileInput, fileOutput;
    char acLine[defCON_MAX_LINE_LEN]; // I don't expect one line to be more than this
    fileInput.open(sCanoeFile.c_str(), fstream::in);
    if(!fileInput.is_open())
    {
        return SetResultCode(CON_RC_FILEOPEN_ERROR_INFILE);
    }

    // first line of input file starts with keyword "VERSION", else file format error
    if(fileInput.getline(acLine, defCON_MAX_LINE_LEN) == NULL)
    {
        // eof file reached without reading anything
        fileInput.close();
        return SetResultCode(CON_RC_FORMAT_ERROR_INFILE);
    }

    // Generate the list of messages

    GenerateMessageList(fileInput);

    // All information gathered, validate and update if necessary
    // Make appropriate changes in the contents of the list

    ValidateMessageList();

    // the format is OK then open the output file
    fileOutput.open(sCanMonFile.c_str(), fstream::out);
    if(!fileOutput.is_open())
    {
        // if output file cannot be opened the close the input file
        // and return the error code
        fileInput.close();
        return SetResultCode(CON_RC_FILEOPEN_ERROR_OUTFILE);
    }

    EncryptData(m_notProcessed);
    bool bRes = WriteToOutputFile(fileOutput);

    fileInput.close();
    fileOutput.close();

    if(!bRes)
    {
        string sLogFile = sCanoeFile.substr(0, sCanoeFile.length()-4);
        sLogFile += ".log";
        m_omLogFilePath = sLogFile;
        fstream fileLog;
        fileLog.open(sLogFile.c_str(), fstream::out);
        if(!fileLog.is_open())
        {
            // if log file cannot be opened return the error code
            return SetResultCode(CON_RC_FILEOPEN_ERROR_LOGFILE);
        }
        else
        {
            CreateLogFile(fileLog);
            fileLog.close();
            return SetResultCode(CON_RC_COMPLETED_WITH_ERROR);
        }
    }

    return m_uiResultCode;
}


const char* CConverter::m_pacResultStrings[] =
{
    "Conversion completed.",
    "Conversion completed with errors. See log file.",
    "Conversion aborted. Error opening input file.",
    "Conversion aborted. Error creating output file.",
    "Conversion aborted. Error with input file format.",
    "Conversion aborted. Error creating log file."
};


/**
 * \brief Returns the error string.
 */
const char* CConverter::GetResultString()
{
    return m_pacResultStrings[m_uiResultCode];
}


/**
 * \brief Set result code.
 */
unsigned int CConverter::SetResultCode(unsigned int uiCode)
{
    return (m_uiResultCode = uiCode);
}


/**
 * \brief Validates the message list
 *
 * Validates the message list and set the error in each signal if present
 */
void CConverter::ValidateMessageList()
{
    list<CMessage>::iterator rMsg;
    unsigned int uiResult;
    for(rMsg=m_listMessages.begin(); rMsg!=m_listMessages.end(); ++rMsg)
    {
        unsigned char ucDataFormat = 0;

        // set the id and frame format
        // canoe puts MSbit = 1 for extended ID
        if(rMsg->m_uiMsgID < 0x80000000UL)
        {
            rMsg->m_cFrameFormat = CMessage::MSG_FF_STANDARD;
        }
        else
        {
            rMsg->m_cFrameFormat = CMessage::MSG_FF_EXTENDED;
            rMsg->m_uiMsgID &= 0x7FFFFFFF;
        }

        rMsg->m_ucNumOfSignals = 0; // reset number of signals to 0
        // this should be updated to number of
        // valid signals as we parse the Signal list

        //pems - Start
        //Scan the message list and make the message format same as the
        //one that has maximum number of signals.
        int iCntMotorolaSignals = 0;
        int iCntIntelSignals = 0;
        list<CSignal>::iterator rSig;
        for(rSig=rMsg->m_listSignals.begin(); rSig!=rMsg->m_listSignals.end(); ++rSig)
        {
            if(rSig->m_ucDataFormat == CSignal::SIG_DF_INTEL)
                iCntIntelSignals++;
            else if(rSig->m_ucDataFormat == CSignal::SIG_DF_MOTOROLA)
                iCntMotorolaSignals++;
        }
        // Update the message data format
        if(iCntIntelSignals >= iCntMotorolaSignals)
        {
            ucDataFormat = CSignal::SIG_DF_INTEL;
            rMsg->m_cDataFormat = ucDataFormat; // set message data format to this i.e format of first valid signal
        }
        else
        {
            ucDataFormat = CSignal::SIG_DF_MOTOROLA;
            rMsg->m_cDataFormat = ucDataFormat;
        }
        //pems - end

        for(rSig=rMsg->m_listSignals.begin(); rSig!=rMsg->m_listSignals.end(); ++rSig)
        {
            uiResult = rSig->Validate(ucDataFormat);

            // if the signal is valid
            if((uiResult == CSignal::SIG_EC_NO_ERR) || (uiResult == CSignal::SIG_EC_OVERFLOW))
            {
                /*Pems Start
                if(ucDataFormat == 0)
                {
                ucDataFormat = rSig.m_ucDataFormat; // now this is updated to the first valid signal's format
                rMsg.m_cDataFormat = ucDataFormat; // set message data format to this i.e format of first valid signal
                }
                Pems End*/
                rMsg->m_ucNumOfSignals++; // increment the signal count for this message
            }
            else
            {
                int flag = 0;
                list<CMessage>::iterator usMsg;
                for(usMsg=m_unsupList.begin(); usMsg!=m_unsupList.end(); ++usMsg)
                {
                    // find matching message from list
                    if((usMsg->m_uiMsgID == rMsg->m_uiMsgID) && (usMsg->m_cFrameFormat == rMsg->m_cFrameFormat))
                    {
                        usMsg->m_listSignals.push_back(*rSig);
                        flag = 1;
                    }

                }
                if(flag == 0)
                {
                    m_unsupList.push_back(*rMsg);
                    CMessage&msg = m_unsupList.back();
                    msg.m_listSignals.clear();
                    msg.m_listSignals.push_back(*rSig);
                }
            }
        }
    }
    //vaildate signals not associated with any messages.
    list<CSignal>::iterator rSig;
    for(rSig=m_listSignal.begin(); rSig!=m_listSignal.end(); ++rSig)
    {
        uiResult = rSig->Validate(rSig->m_ucDataFormat);
        // if the signal is valid
        if((uiResult == CSignal::SIG_EC_NO_ERR) || (uiResult == CSignal::SIG_EC_OVERFLOW))
        {
            /*Pems Start
            if(ucDataFormat == 0)
            {
            ucDataFormat = rSig.m_ucDataFormat; // now this is updated to the first valid signal's format
            rMsg.m_cDataFormat = ucDataFormat; // set message data format to this i.e format of first valid signal
            }
            Pems End*/
        }
        else
        {
            int flag = 0;
            list<CMessage>::iterator usMsg;
            for(usMsg=m_unsupList.begin(); usMsg!=m_unsupList.end(); ++usMsg)
            {
                if((usMsg->m_uiMsgID == 1073741824) && (usMsg->m_cFrameFormat == 'X'))
                {
                    usMsg->m_listSignals.push_back(*rSig);
                    flag = 1;
                }

            }
            if(flag == 0)
            {
                CMessage def_msg;
                def_msg.m_acName = "VECTOR__INDEPENDENT_SIG_MSG";
                def_msg.m_uiMsgID = 1073741824 ;
                def_msg.m_cFrameFormat = 'X';
                m_unsupList.push_back(def_msg);
                def_msg.m_listSignals.push_back(*rSig);
            }
        }
    }
    list<CParameters>::iterator rParam;
    for(rParam=m_listParameters.begin(); rParam!=m_listParameters.end(); ++rParam)
    {
        if(rParam->m_ObjectId == "BU_")
            m_listParameterArray[1].push_back(*rParam);
        else if(rParam->m_ObjectId == "BO_")
            m_listParameterArray[2].push_back(*rParam);
        else if(rParam->m_ObjectId == "SG_")
            m_listParameterArray[3].push_back(*rParam);
        else if(rParam->m_ObjectId == "")
            m_listParameterArray[0].push_back(*rParam);
        else if(rParam->m_ObjectId == "BU_SG_REL_")
            m_listParameterArray[4].push_back(*rParam);
        else if(rParam->m_ObjectId == "BU_BO_REL_")
            m_listParameterArray[5].push_back(*rParam);
    }
}


/**
 * \brief Generates the message list
 *
 * This function will parse the input file and line by line
 * and generates  a list of message,signal,value table,comments,etc
 */
void CConverter::GenerateMessageList(fstream& fileInput)
{
    char acLine[defCON_MAX_LINE_LEN],local_copy[defCON_MAX_LINE_LEN]; // I don't expect one line to be more than this
    char *pcTok;
    int flag=0;
    // parsing the input file
    while(fileInput.getline(acLine, defCON_MAX_LINE_LEN))
    {
        char *pcToken, *pcLine;

        for (;;) {
            pcLine = acLine;
            pcLine += strlen(pcLine); // go to last position
            pcLine --;
            if (*pcLine == '\r')
                fileInput.getline(pcLine, defCON_MAX_LINE_LEN);
            else
                break;
        }


        // avoid leading <spaces> before tokenising, so passing the
        // starting point will be correct in each case, when calling
        // msg.Format, sig.Format etc.
        strcpy_s(local_copy, sizeof(local_copy), acLine);
        pcLine = acLine;

        while(*pcLine && *pcLine == ' ')
        {
            *pcLine++;
        }

        pcToken = strtok_s(pcLine, " :", &pcTok);

        if(pcToken)
        {
            //compare token to known types to interpret the line further

            // new line - skip
            if(strcmp(pcToken, "\n") == 0)
            {
                continue;
            }

            // message
            else if(strcmp(pcToken, "BO_") == 0)
            {
                CMessage msg;
                msg.Format(pcLine + strlen(pcToken)+1);

                // add the new message to the list
                if((msg.m_acName != "VECTOR__INDEPENDENT_SIG_MSG") && !(msg.m_uiMsgID == 3221225472))
                {
                    CConverter::valid_msg = true;
                    m_listMessages.push_back(msg);
                }
                else
                    CConverter::valid_msg = false;

            }

            // signal
            else if(strcmp(pcToken, "SG_") == 0)
            {
                CSignal sig;
                sig.Format(pcLine + strlen(pcToken) + 1);
                //if signal associated with a msg add it to that perticular list
                //elses add it to msg independent list
                if(CConverter::valid_msg == true)
                {
                    //insert signals in sorted order
                    int flag = 0;
                    CMessage& msg = m_listMessages.back();
                    if (msg.m_listSignals.empty()) {
                        msg.m_listSignals.push_front(sig);
                        flag = 1;
                        continue;
                    }
                    int count = 0;
                    list<CSignal>::iterator sig1 = msg.m_listSignals.end();
                    while(sig1!=msg.m_listSignals.begin())
                    {
                        --sig1;
                        ++count;
                        if(((sig1->m_ucWhichByte * 8) + sig1->m_ucStartBit) > ((sig.m_ucWhichByte * 8) + sig.m_ucStartBit))
                        {
                            ++sig1;
                            msg.m_listSignals.insert(sig1, sig);
                            flag = 1;
                            break;
                        }
                    }
                    if (flag == 0)
                    {
                        msg.m_listSignals.push_front(sig);
                    }
                    // this signal should belong to the last message
                    msg.m_ucNumOfSignals++; // increment the signal count
                }
                else
                {
                    sig.m_ucWhichByte = 1;
                    sig.m_ucStartBit = 0;
                    m_listSignal.push_back(sig);
                }
            }

            // value descriptor
            else if(strcmp(pcToken, "VAL_") == 0)
            {
                // <msgid><sp><signalName><sp><value1><sp><"desc1"><sp><value2><sp><"desc2"> ...;
                // get MsgId, find the message from the messagelist.
                // find the signal from the message, then add the value descritors
                // to the respective signals

                pcLine = pcLine + strlen(pcToken) + 1; // to get next token
                pcToken = strtok_s(pcLine, " ", &pcTok); // msgid

                unsigned int id = (unsigned int) strtoul(pcToken, NULL, 10);

                if(id != 3221225472)
                {
                    list<CMessage>::iterator rMsg;
                    for(rMsg=m_listMessages.begin(); rMsg!=m_listMessages.end(); ++rMsg)
                    {
                        // find matching message from list
                        if(rMsg->m_uiMsgID == id)
                        {
                            pcLine = pcLine + strlen(pcToken) + 1; // to get next token
                            pcToken = strtok_s(pcLine, " ", &pcTok); // Signal name
                            list<CSignal>::iterator rSig;
                            // find matching signal
                            for(rSig=rMsg->m_listSignals.begin(); rSig!=rMsg->m_listSignals.end(); ++rSig)
                            {
                                if(rSig->m_acName == pcToken)
                                {
                                    rSig->AddValueDescriptors(pcLine + strlen(pcToken) + 1, fileInput);
                                    break; // if we got the signal we wanted
                                }
                            }
                            break; // we got the message we wanted
                        }
                    }
                }

                else
                {
                    pcLine = pcLine + strlen(pcToken) + 1; // to get next token
                    pcToken = strtok_s(pcLine, " ", &pcTok); // Signal name

                    list<CSignal>::iterator rSig;
                    // find matching signal
                    for(rSig=m_listSignal.begin(); rSig!=m_listSignal.end(); ++rSig)
                    {
                        if(rSig->m_acName == pcToken)
                        {
                            rSig->AddValueDescriptors(pcLine + strlen(pcToken) + 1, fileInput);
                            break; // if we got the signal we wanted
                        }
                    }

                }

            }
            // signal value qualifier
            else if(strcmp(pcToken, "SIG_VALTYPE_") == 0)
            {
                // <msgID> <signal name> : 1 -- float
                // <msgID> <signal name> : 2 -- double
                // get MsgId, find the message from the messagelist.
                // find the signal from the message, then update the
                // signal type appropriately of the respective signal

                pcToken = strtok_s(NULL, " :;", &pcTok); // msgid
                unsigned int id = (unsigned int)atoi(pcToken);
                if(id != 3221225472)
                {
                    list<CMessage>::iterator rMsg;
                    for(rMsg=m_listMessages.begin(); rMsg!=m_listMessages.end(); ++rMsg)
                    {
                        // find matching message from list
                        if(rMsg->m_uiMsgID == id)
                        {
                            pcToken = strtok_s(NULL, " :;", &pcTok); // Signal name

                            list<CSignal>::iterator rSig;
                            // find matching signal
                            for(rSig=rMsg->m_listSignals.begin(); rSig!=rMsg->m_listSignals.end(); ++rSig)
                            {
                                if(rSig->m_acName == pcToken)
                                {
                                    if(pcToken = strtok_s(NULL, " :;", &pcTok)) // qualifier (1 or 2)
                                    {
                                        // update signal type based on qualifier
                                        switch(*pcToken)
                                        {
                                            case '1':
                                                rSig->m_ucType = CSignal::SIG_TYPE_FLOAT;
                                                break;
                                            case '2':
                                                rSig->m_ucType = CSignal::SIG_TYPE_DOUBLE;
                                                break;
                                            default:
                                                break;
                                        }
                                    }
                                    break; // we got the signal we wanted
                                }
                            }

                            break; // we got the message we wanted
                        }
                    }
                }
                else
                {
                    pcToken = strtok_s(NULL, " :;", &pcTok); // Signal name
                    // find matching signal
                    list<CSignal>::iterator rSig;
                    for(rSig=m_listSignal.begin(); rSig!=m_listSignal.end(); ++rSig)
                    {
                        if(rSig->m_acName == pcToken)
                        {
                            if(pcToken = strtok_s(NULL, " :;", &pcTok)) // qualifier (1 or 2)
                            {
                                // update signal type based on qualifier
                                switch(*pcToken)
                                {
                                    case '1':
                                        rSig->m_ucType = CSignal::SIG_TYPE_FLOAT;
                                        break;
                                    case '2':
                                        rSig->m_ucType = CSignal::SIG_TYPE_DOUBLE;
                                        break;
                                    default:
                                        break;
                                }
                            }
                            break; // we got the signal we wanted
                        }
                    }
                }
            }
            //value table
            else if(strcmp(pcToken, "VAL_TABLE_") == 0)
            {
                CValueTable tab;
                pcToken = strtok_s(pcLine, " ", &pcTok);
                tab.Format(pcLine + strlen(pcToken) + 1, fileInput);
                m_vTab.push_back(tab);
            }
            //comments
            else if(strcmp(pcToken, "CM_") == 0)
            {
                string comment = pcTok;	// for network node - venkat
				pcLine = pcLine + strlen(pcToken) + 1;
                pcToken = strtok_s(pcLine, " ", &pcTok);
                CComment cm;
				
                //comments related to node
                if(strcmp(pcToken, "BU_") == 0)
                {
                    pcToken = strtok_s(NULL, " ", &pcTok);
                    cm.m_elementName= pcToken;
                    pcToken = strtok_s(NULL, "", &pcTok);
                    comment = pcToken;
                    while(strstr(pcToken, "\";") == NULL)
                    {
                        fileInput.getline(acLine, defCON_MAX_LINE_LEN);
                        pcToken = acLine;
                        comment = comment + pcToken;
                    }
                    cm.m_comment= comment;
                    m_cmNode.push_back(cm);
                }
                //comments related to messages
                else if(strcmp(pcToken,"BO_") == 0)
                {
                    pcToken = strtok_s(NULL, " ", &pcTok);
                    cm.m_msgID = atoi(pcToken);

                    // set the id and frame format
                    // canoe puts MSbit = 1 for extended ID
                    if(cm.m_msgID < 0x80000000UL)
                    {
                        cm.m_msgType= 'S';
                    }
                    else
                    {
                        cm.m_msgType= 'X';
                        cm.m_msgID  &= 0x7FFFFFFF;
                    }
                    pcToken = strtok_s(NULL, "", &pcTok);
                    comment = pcToken;
                    while(strstr(pcToken, "\";") == NULL)
                    {
                        fileInput.getline(acLine, defCON_MAX_LINE_LEN);
                        pcToken = acLine;
                        comment = comment + pcToken;
                    }
                    cm.m_comment= comment;
                    m_cmMsg.push_back(cm);
                }
                //comments related to signals
                else if(strcmp(pcToken, "SG_") == 0)
                {
                    pcToken = strtok_s(NULL, " ", &pcTok);
                    cm.m_msgID = strtoul(pcToken, NULL, 10);
                    if(cm.m_msgID < 0x80000000UL)
                    {
                        cm.m_msgType = 'S';
                    }
                    else
                    {
                        cm.m_msgType = 'X';
                        cm.m_msgID &= 0x7FFFFFFF;
                    }
                    pcToken = strtok_s(NULL, " ", &pcTok);
                    cm.m_elementName = pcToken;
                    pcToken = strtok_s(NULL, "", &pcTok);
                    comment = pcToken;
                    while(strstr(pcToken, "\";") == NULL)
                    {
                        fileInput.getline(acLine, defCON_MAX_LINE_LEN);
                        pcToken = acLine;
                        comment = comment + pcToken;
                    }
                    cm.m_comment= comment;
                    m_cmSig.push_back(cm);
                }
                //comments related to network
                else
                {
                    //comment = pcToken;
					if(comment.find(";") < 0)
					{
						while(strstr(pcToken, "\";") == NULL)
						{
							fileInput.getline(acLine, defCON_MAX_LINE_LEN);
							pcToken = acLine;
							comment = comment + pcToken;
						}
					}
                    cm.m_comment= comment;
                    m_cmNet.push_back(cm);
                }

            }
            // node
            else if(strcmp(pcToken, "BU_") == 0)
            {
                create_Node_List(pcLine + strlen(pcToken)+1);
            }

            else if ((strcmp(pcToken, "BA_DEF_")==0) || (strcmp(pcToken, "BA_DEF_REL_")==0))
            {
                CParameters pObj;
                pObj.Format(pcLine + strlen(pcToken) + 1); // to get next token
                m_listParameters.push_back(pObj);
            }

            //Param Initial Values
            else if(strcmp(pcToken, "BA_DEF_DEF_")==0 )
            {
                char acTemp[defCON_TEMP_LEN],*pcTemp;
                pcTemp = acTemp;
                pcToken = strtok_s(NULL, "\"", &pcTok);
                pcToken = strtok_s(NULL, "\"", &pcTok);
                flag=0;
                while(*pcToken && *pcToken != '"')
                {
                    *pcTemp++ = *pcToken++;
                }
                *pcTemp = '\0';
                list<CParameters>::iterator rParam;
                for(rParam=m_listParameters.begin(); rParam!=m_listParameters.end(); ++rParam)
                {
                    // find matching Parameter from list
                    if(rParam->m_ParamName == acTemp)
                    {
                        pcTemp=acTemp;
                        pcToken = strtok_s(NULL, ";", &pcTok); // default val
                        rParam->ReadDefaultVal(pcToken);
                        flag=1;
                        break;
                    }
                }
                if(flag==0)
                {
                    string errString = "BA_DEF_DEF_ \"";
                    errString += acTemp;
                    errString += "\" ";
                    errString += pcToken;
                    errString += " : Match not Found in Param List\n";
                    defList.push_back(errString);
                }

            }

            //RX,Tx Parameter Definition
            else if(strcmp(pcToken,"BA_DEF_DEF_REL_")==0)
            {
                char acTemp[defCON_TEMP_LEN],*pcTemp;
                pcTemp = acTemp;
                flag = 0;
                pcToken = strtok_s(NULL, "\"", &pcTok);
                while(*pcToken && (*pcToken != '"'))
                {
                    *pcTemp++ = *pcToken++; // copy SIG_NAME only, i.e. till first 'space'
                }
                *pcTemp = '\0';
                list<CParameters>::iterator rParam;
                for(rParam=m_listParameters.begin(); rParam!=m_listParameters.end(); ++rParam)
                {
                    // find matching Parameter from list
                    if(rParam->m_ParamName == acTemp)
                    {
                        pcTemp = acTemp;
                        pcToken = strtok_s(NULL, ";", &pcTok); // default val
                        rParam->ReadDefaultVal(pcToken);
                        flag=1;
                        break;
                    }
                }
                if(flag==0)
                {
                    string errString = "BA_DEF_DEF_REL \"";
                    errString += acTemp;
                    errString += "\" ";
                    errString += pcToken;
                    errString += " : Match not Found in Param List\n";
                    defList.push_back(errString);

                }
            }

            // Parameter Other values //
            else if(strcmp(pcToken, "BA_")==0)
            {
                char acTemp[defCON_TEMP_LEN],*pcTemp;
                pcTemp = acTemp;
                while(*pcLine && (*pcLine == ' '))
                {
                    *pcLine++;
                }

                //get Param name
                pcLine = pcLine + strlen(pcToken) + 1;
                pcToken = strtok_s(pcLine, "\"", &pcTok);

                while(*pcToken && (*pcToken != '"'))
                {
                    *pcTemp++ = *pcToken++;
                }
                *pcTemp = '\0';

                list<CParameters>::iterator rParam;
                for(rParam=m_listParameters.begin(); rParam!=m_listParameters.end(); ++rParam)
                {
                    // find matching Parameter from list
                    if(rParam->m_ParamName == acTemp)
                    {
                        rParam->FormatParamValue(pcLine + strlen(acTemp) + 3); // to get next token
                        pcTemp=acTemp;
                        break;
                    }
                }
            }
            //maintain a list of lines not processed
            else
            {
                string str = local_copy;
                m_notProcessed.push_back(str);
                continue;
            }
        }
    }
}


/**
 * \brief Writes all the data to the output file in CANoe format
 *
 * Writes all the data to the output file in CANoe format.
 */
bool CConverter::WriteToOutputFile(fstream& fileOutput)
{
    bool bResult = true;
    // write to the output file
    // write header
    fileOutput << T_HEADER << endl << endl;
    fileOutput << T_DB_VER " " T_VER_NO << endl << endl;

    // number of messages
    fileOutput << T_NUM_OF_MSG " ";
    fileOutput << dec << m_listMessages.size();
    fileOutput << endl << endl;

    //Write Messagess to the Output file
    CMessage msg;
    bResult &= msg.writeMessageToFile(fileOutput, m_listMessages, false);

    // write all messages, signals not associated with any Messages
    if(!m_listSignal.empty())
    {
        fileOutput << T_ST_SIG_LIST << endl;
        CSignal sig;
        bResult &= sig.WriteSignaltofile(fileOutput, m_listSignal, 0, 0, false);
        fileOutput << T_END_SIG_LIST << endl;
        fileOutput << endl;
    }

    //write value table
    fileOutput << T_ST_VAL_TAB << endl;
    CValueTable temp_vtab;
    temp_vtab.writeValueTabToFile (fileOutput,m_vTab);
    fileOutput << T_END_VAL_TAB << endl;
    fileOutput << endl;

    //write list of nodes
    fileOutput << T_NODE " ";
    bool comma = false;
    list<string>::iterator node;
    for(node=m_listNode.begin(); node!=m_listNode.end(); ++node)
    {
        if(comma)
            fileOutput << ",";
        fileOutput << node->c_str();
        comma = true;
    }
    fileOutput << endl;
    fileOutput << endl;

    //Write Comments
    fileOutput << T_ST_COMMENT << endl;
    //network comments
    fileOutput << T_ST_CM_NET << endl;
    list<CComment>::iterator cmt;
    for(cmt=m_cmNet.begin(); cmt!=m_cmNet.end(); ++cmt)
    {
        fileOutput << cmt->m_elementName.c_str();
        fileOutput << " ";
        fileOutput << cmt->m_comment.c_str();
        fileOutput << endl;
    }
    fileOutput << T_END_CM_NET << endl;
    fileOutput << endl;

    //node comments
    fileOutput << T_ST_CM_NODE << endl;
    for(cmt=m_cmNode.begin(); cmt!=m_cmNode.end(); ++cmt)
    {
        fileOutput << cmt->m_elementName.c_str();
        fileOutput << " ";
        fileOutput << cmt->m_comment.c_str();
        fileOutput << endl;
    }
    fileOutput << T_END_CM_NODE << endl;
    fileOutput << endl;

    //message comments
    fileOutput << T_ST_CM_MSG << endl;
    for(cmt=m_cmMsg.begin(); cmt!=m_cmMsg.end(); ++cmt)
    {
        fileOutput << cmt->m_msgID;
        fileOutput << " ";
        fileOutput << cmt->m_msgType;
        fileOutput << " ";
        fileOutput << cmt->m_comment.c_str();
        fileOutput << endl;
    }
    fileOutput << T_END_CM_MSG << endl;
    fileOutput << endl;

    //signal comments
    fileOutput << T_ST_CM_SIG << endl;
    for(cmt=m_cmSig.begin(); cmt!=m_cmSig.end(); ++cmt)
    {
        fileOutput << cmt->m_msgID;
        fileOutput << " ";
        fileOutput << cmt->m_msgType;
        fileOutput << " ";
        fileOutput << cmt->m_elementName.c_str();
        fileOutput << " ";
        fileOutput << cmt->m_comment.c_str();
        fileOutput << endl;
    }
    fileOutput << T_END_CM_SIG << endl;
    fileOutput << T_END_COMMENT << endl;
    fileOutput << endl;

    //Write Parameters to the output file.
    fileOutput << START_PARAM_TAG << endl;

    fileOutput << START_NETPARAM_TAG << endl;
    bResult=bResult & WriteParametersToFile(fileOutput, m_listParameterArray[0]);
    fileOutput << END_NETPARAM_TAG << endl;
    fileOutput << endl;

    fileOutput << START_NODEPARAM_TAG << endl;
    bResult=bResult & WriteParametersToFile(fileOutput, m_listParameterArray[1]);
    fileOutput << END_NODEPARAM_TAG << endl;
    fileOutput << endl;

    fileOutput << START_MSGPARAM_TAG << endl;
    bResult=bResult & WriteParametersToFile(fileOutput, m_listParameterArray[2]);
    fileOutput << END_MSGPARAM_TAG << endl;
    fileOutput << endl;

    fileOutput << START_SIGPARAM_TAG << endl;
    bResult=bResult & WriteParametersToFile(fileOutput, m_listParameterArray[3]);
    fileOutput << END_SIGPARAM_TAG << endl;
    fileOutput << endl;

    fileOutput << START_RXPARAM_TAG << endl;
    bResult=bResult & WriteParametersToFile(fileOutput, m_listParameterArray[4]);
    fileOutput << END_RXPARAM_TAG << endl;
    fileOutput << endl;

    fileOutput << START_TXPARAM_TAG << endl;
    bResult=bResult & WriteParametersToFile(fileOutput, m_listParameterArray[5]);
    fileOutput << END_TXPARAM_TAG << endl;
    fileOutput << END_PARAM_TAG << endl;
    fileOutput << endl;

    //Parameter Values
    fileOutput << START_PARAMVAL_TAG << endl;
    fileOutput << START_NETVAL_TAG << endl;
    list<CParameters>::iterator rParam;
    for(rParam=m_listParameterArray[0].begin(); rParam!=m_listParameterArray[0].end(); ++rParam)
    {
        list<CParameterValues>::iterator vParam;
        for(vParam=rParam->m_listParamValues[0].begin(); vParam!=rParam->m_listParamValues[0].end(); ++vParam)
        {
            vParam->WriteNetValuesToFile(fileOutput, rParam->m_ParamType, rParam->m_ParamName);
        }

    }
    fileOutput << END_NETVAL_TAG << endl;
    fileOutput << endl;

    fileOutput << START_NODEVAL_TAG << endl;
    for(rParam=m_listParameterArray[1].begin(); rParam!=m_listParameterArray[1].end(); ++rParam)
    {
        list<CParameterValues>::iterator vParam;
        for(vParam=rParam->m_listParamValues[1].begin(); vParam!=rParam->m_listParamValues[1].end(); ++vParam)
        {
            vParam->WriteNodeValuesToFile(fileOutput, rParam->m_ParamType, rParam->m_ParamName);
        }

    }
    fileOutput << END_NODEVAL_TAG << endl;
    fileOutput << endl;

    fileOutput << START_MSGVAL_TAG << endl;
    for(rParam=m_listParameterArray[2].begin(); rParam!=m_listParameterArray[2].end(); ++rParam)
    {
        list<CParameterValues>::iterator vParam;
        for(vParam=rParam->m_listParamValues[2].begin(); vParam!=rParam->m_listParamValues[2].end(); ++vParam)
        {
            vParam->WriteMesgValuesToFile(fileOutput, rParam->m_ParamType, rParam->m_ParamName);
        }

    }
    fileOutput << END_MSGVAL_TAG << endl;
    fileOutput << endl;

    fileOutput << START_SIGVAL_TAG << endl;
    for(rParam=m_listParameterArray[3].begin(); rParam!=m_listParameterArray[3].end(); ++rParam)
    {
        list<CParameterValues>::iterator vParam;
        for(vParam=rParam->m_listParamValues[3].begin(); vParam!=rParam->m_listParamValues[3].end(); ++vParam)
        {
            vParam->WriteSigValuesToFile(fileOutput, rParam->m_ParamType, rParam->m_ParamName);
        }
    }
    fileOutput << END_SIGVAL_TAG << endl;
    fileOutput << endl;
    fileOutput << END_PARAMVAL_TAG << endl;
    fileOutput << endl;
    fileOutput << endl;

    //list of not supported
    fileOutput << T_ST_NOT_SUP << endl;
    msg.writeMessageToFile(fileOutput, m_unsupList, true);
    fileOutput << T_END_NOT_SUP << endl;
    fileOutput << endl;

    //lines that were not processed
    fileOutput << T_ST_NOT_PRO << endl;
    list<string>::iterator np;
    for(np=m_notProcessed.begin(); np!=m_notProcessed.end(); ++np)
    {
        fileOutput << np->c_str();
        fileOutput << endl;
    }
    fileOutput << endl;
    fileOutput << T_END_NOT_PRO << endl;

    return bResult;
}


/**
 * \brief Logs the eror info in log file
 *
 * Logs the eror info in log file.
 */
void CConverter::CreateLogFile(fstream& fileLog)
{
    char first_msg = 1;

    // write to the output file
    fileLog << "Conversion Error Log" << endl;
    fileLog << endl;

    list<CMessage>::iterator msg;
    for(msg=m_listMessages.begin(); msg!=m_listMessages.end(); ++msg)
    {
        first_msg = 1;
        list<CSignal>::iterator sig;
        for(sig=msg->m_listSignals.begin(); sig!=msg->m_listSignals.end(); ++sig)
        {
            // write signal only if it is not valid
            if(sig->m_uiError != CSignal::SIG_EC_NO_ERR)
            {
                // for the first wrong signal, log the message details also
                if(first_msg == 1)
                {
                    fileLog << endl;
                    fileLog << "MSG_ID: ";
                    fileLog << dec << msg->m_uiMsgID;
                    fileLog << " \tMSG_TYPE: ";
                    fileLog << msg->m_cFrameFormat;
                    fileLog << " \tMSG_NAME: ";
                    fileLog << msg->m_acName.c_str();
                    fileLog << endl;
                    first_msg = 0;
                }
                fileLog << "\tSIG_NAME: ";
                fileLog << sig->m_acName.c_str();
                fileLog << ", ";
                fileLog << sig->GetErrorString();
                fileLog << ", ACTION: ";
                fileLog << sig->GetErrorAction();
                fileLog << endl;
            }
        }
    }

    //log errors in the signal list.
    first_msg = 1;
    list<CSignal>::iterator sig;
    for(sig=m_listSignal.begin(); sig!=m_listSignal.end(); ++sig)
    {
        // write signal only if it is not valid
        if(sig->m_uiError != CSignal::SIG_EC_OVERFLOW)
        {
            // for the first wrong signal, log the message details also
            if(first_msg == 1)
            {
                fileLog << endl;
                fileLog << "MSG_ID: 1073741824";
                fileLog << " \tMSG_TYPE: X";
                fileLog << " \tMSG_NAME: VECTOR__INDEPENDENT_SIG_MSG";
                fileLog << endl;
                first_msg = 0;
            }
            fileLog << "\tSIG_NAME: ";
            fileLog << sig->m_acName.c_str();
            fileLog << ", ";
            fileLog << sig->GetErrorString();
            fileLog << ", ACTION: ";
            fileLog << sig->GetErrorAction();
            fileLog << endl;
        }
    }
    list<string>::iterator str;
    for(str=defList.begin(); str!=defList.end(); ++str)
    {
        fileLog << str->c_str();
    }
    list<CParameters>::iterator rParam;
    for(rParam=m_listParameters.begin(); rParam!=m_listParameters.end(); ++rParam)
    {
        if(rParam->m_defError)
        {
            fileLog << "OBJECT ID : ";
            fileLog << rParam->m_ObjectId.c_str();
            fileLog << "\tPARAM_NAME :\"";
            fileLog << rParam->m_ParamName.c_str();
            fileLog << "\"";
            fileLog << endl;
            fileLog << "\tDescription:Default Value tag(BA_DEF_DEF_) doesn;t exist ";
            fileLog << "\t Action Taken : Reset to default value";
            fileLog << endl;
        }
        if(rParam->m_RangeError)
        {
            fileLog << "OBJECT ID : ";
            fileLog << rParam->m_ObjectId.c_str();
            fileLog << "\tPARAM_NAME :\"";
            fileLog << rParam->m_ParamName.c_str();
            fileLog << "\"";
            fileLog << endl;
            fileLog << "\tDescription: Invalid Data Ranges";
            fileLog << "\t Action Taken : Reset to default value";
            fileLog << endl;
        }
    }
}


/**
 * \brief     creates a list of nodes inthe network
 * \param[in] pcLine String having all the node names
 *
 * Creates a list of nodes in the network.
 */
void CConverter::create_Node_List(char *pcLine)
{
    char* pcToken, *pcTok;
    // get the MSG ID
    pcToken = strtok_s(pcLine, " ,", &pcTok);
    while(pcToken)
    {
        string str = pcToken;
        m_listNode.push_back(str);
        pcToken = strtok_s(NULL, " ,", &pcTok);
    }
}


/**
 * \brief     encrypts all the strings present in the list
 * \param[in] m_notProcessed List of strings
 *
 * Encrypts all the strings present in the list
 */
void CConverter::EncryptData(list<string> &m_notProcessed)
{
    list<string>::iterator str;
    for(str=m_notProcessed.begin(); str!=m_notProcessed.end(); ++str)
    {
        //read the string at the position
        string::iterator ch;
        for(ch=str->begin() ; ch<str->end(); ++ch)
        {
            if ((*ch >= 'a' && *ch <= 'm') || (*ch >= 'A' && *ch <= 'M'))
            {
                *ch = *ch + 13;
            }
            else if ((*ch >= 'n' && *ch <= 'z') || (*ch >= 'N' && *ch <= 'Z'))
                *ch = *ch - 13;
        }
    }
}
