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
 * \brief     Implementation file for the Comment class.
 * \author    Amitesh Bharti
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Implementation file for the Comment class.
 */

#include "StdAfx.h"
#include "App.h"
#include "Converter.h"
#include "Signal.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

/**
 * \brief Constructor
 */
CConverter::CConverter()
{
    m_uiResultCode = CON_RC_NOERROR;
}

bool CConverter::valid_msg = TRUE;
unsigned char CConverter::ucMsg_DLC = 8;

/**
 * \brief      Destructor
 */
CConverter::~CConverter()
{
    if(m_listMessages.IsEmpty())
    {
        m_listMessages.RemoveAll();
    }
}

const char CConverter::m_accHeader[] =
    "//******************************BUSMASTER Messages and signals Database ******************************//\n\n";
/**
 * This is the basic function which is to be called
 * to convert any given CANoe file to a CANMon file.
 */
unsigned int CConverter::Convert(CString sCanoeFile,CString sCanMonFile)
{
    CStdioFile fileInput,fileOutput;
    char acLine[defCON_MAX_LINE_LEN]; // I don't expect one line to be more than this
    if(!fileInput.Open(sCanoeFile,CFile::modeRead))
    {
        return SetResultCode(CON_RC_FILEOPEN_ERROR_INFILE);
    }

    // first line of input file starts with keyword "VERSION", else file format error
    if(fileInput.ReadString(acLine,defCON_MAX_LINE_LEN) == NULL)
    {
        // eof file reached without reading anything
        fileInput.Close();
        return SetResultCode(CON_RC_FORMAT_ERROR_INFILE);
    }

    // Generate the list of messages

    GenerateMessageList(fileInput);

    // All information gathered, validate and update if necessary
    // Make appropriate changes in the contents of the list

    ValidateMessageList();

    // the format is OK then open the output file
    if(!fileOutput.Open(sCanMonFile,CFile::modeWrite | CFile::modeCreate))
    {
        // if output file cannot be opened the close the input file
        // and return the error code
        fileInput.Close();
        return SetResultCode(CON_RC_FILEOPEN_ERROR_OUTFILE);
    }

    EncryptData(m_notProcessed);
    bool bRes = WriteToOutputFile(fileOutput);

    fileInput.Close();
    fileOutput.Close();

    if(!bRes)
    {
        CString sLogFile = sCanoeFile.Left(sCanoeFile.GetLength()-4);
        sLogFile += ".log";
        m_omLogFilePath = sLogFile;
        CStdioFile fileLog;
        if(!fileLog.Open(sLogFile,CFile::modeWrite | CFile::modeCreate))
        {
            // if log file cannot be opened return the error code
            return SetResultCode(CON_RC_FILEOPEN_ERROR_LOGFILE);
        }
        else
        {
            CreateLogFile(fileLog);
            fileLog.Close();
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
    POSITION pos = m_listMessages.GetHeadPosition();
    unsigned int uiResult;
    while(pos != NULL)
    {
        CMessage& rMsg = m_listMessages.GetNext(pos);
        unsigned char ucDataFormat = 0;


        // set the id and frame format
        // canoe puts MSbit = 1 for extended ID
        if(rMsg.m_uiMsgID < 0x80000000UL)
        {
            rMsg.m_cFrameFormat = CMessage::MSG_FF_STANDARD;
        }
        else
        {
            rMsg.m_cFrameFormat = CMessage::MSG_FF_EXTENDED;
            rMsg.m_uiMsgID &= 0x7FFFFFFF;
        }

        rMsg.m_ucNumOfSignals = 0; // reset number of signals to 0
        // this should be updated to number of
        // valid signals as we parse the Signal list

        //pems - Start
        //Scan the message list and make the message format same as the
        //one that has maximum number of signals.
        POSITION posSigx = rMsg.m_listSignals.GetHeadPosition();
        int iCntMotorolaSignals = 0;
        int iCntIntelSignals = 0;
        while(posSigx != NULL)
        {
            CSignal& rSig = rMsg.m_listSignals.GetNext(posSigx);

            if(rSig.m_ucDataFormat == CSignal::SIG_DF_INTEL)
                iCntIntelSignals++;
            else if(rSig.m_ucDataFormat == CSignal::SIG_DF_MOTOROLA)
                iCntMotorolaSignals++;
        }
        // Update the message data format
        if(iCntIntelSignals >= iCntMotorolaSignals)
        {
            ucDataFormat = CSignal::SIG_DF_INTEL;
            rMsg.m_cDataFormat = ucDataFormat; // set message data format to this i.e format of first valid signal
        }
        else
        {
            ucDataFormat = CSignal::SIG_DF_MOTOROLA;
            rMsg.m_cDataFormat = ucDataFormat;
        }
        //pems - end

        POSITION posSig = rMsg.m_listSignals.GetHeadPosition();
        while(posSig != NULL)
        {
            CSignal& rSig = rMsg.m_listSignals.GetNext(posSig);
            uiResult = rSig.Validate(ucDataFormat);

            // if the signal is valid
            if(uiResult == CSignal::SIG_EC_NO_ERR || uiResult == CSignal::SIG_EC_OVERFLOW)
            {
                /*Pems Start
                if(ucDataFormat == 0)
                {
                ucDataFormat = rSig.m_ucDataFormat; // now this is updated to the first valid signal's format
                rMsg.m_cDataFormat = ucDataFormat; // set message data format to this i.e format of first valid signal
                }
                Pems End*/
                rMsg.m_ucNumOfSignals++; // increment the signal count for this message
            }
            else
            {
                int flag = 0;
                POSITION us_posMsg = m_unsupList.GetHeadPosition();
                while(us_posMsg != NULL)
                {
                    CMessage& usMsg = m_unsupList.GetNext(us_posMsg);
                    // find matching message from list
                    if(usMsg.m_uiMsgID == rMsg.m_uiMsgID && usMsg.m_cFrameFormat == rMsg.m_cFrameFormat )
                    {
                        usMsg.m_listSignals.AddTail(rSig);
                        flag = 1;
                    }

                }
                if(flag == 0)
                {
                    POSITION pos = m_unsupList.AddTail(rMsg);
                    CMessage &msg = m_unsupList.GetAt(pos);
                    msg.m_listSignals.RemoveAll();
                    msg.m_listSignals.AddTail(rSig);
                }
            }
        }
    }
    //vaildate signals not associated with any messages.
    POSITION posSig = m_listSignal.GetHeadPosition();
    while(posSig != NULL)
    {
        CSignal& rSig = m_listSignal.GetNext(posSig);
        uiResult = rSig.Validate(rSig.m_ucDataFormat);
        // if the signal is valid
        if(uiResult == CSignal::SIG_EC_NO_ERR || uiResult == CSignal::SIG_EC_OVERFLOW)
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
            POSITION us_posMsg = m_unsupList.GetHeadPosition();
            while(us_posMsg != NULL)
            {
                CMessage& usMsg = m_unsupList.GetNext(us_posMsg);
                if(usMsg.m_uiMsgID == 1073741824 && usMsg.m_cFrameFormat == 'X')
                {
                    usMsg.m_listSignals.AddTail(rSig);
                    flag = 1;
                }

            }
            if(flag == 0)
            {
                CMessage def_msg;
                strcpy(def_msg.m_acName,"VECTOR__INDEPENDENT_SIG_MSG");
                def_msg.m_uiMsgID = 1073741824 ;
                def_msg.m_cFrameFormat = 'X';
                POSITION pos = m_unsupList.AddTail(def_msg);
                CMessage &msg = m_unsupList.GetAt(pos);
                msg.m_listSignals.AddTail(rSig);
            }
        }
    }
    pos=m_listParameters.GetHeadPosition();
    while(pos!=NULL)
    {
        CParameters& rParam=m_listParameters.GetNext(pos);
        if(strcmp(rParam.m_ObjectId,"BU_")==0)
            m_listParameterArray[1].AddTail(rParam);
        else if(strcmp(rParam.m_ObjectId,"BO_")==0)
            m_listParameterArray[2].AddTail(rParam);
        else if(strcmp(rParam.m_ObjectId,"SG_")==0)
            m_listParameterArray[3].AddTail(rParam);
        else if(strcmp(rParam.m_ObjectId,"")==0)
            m_listParameterArray[0].AddTail(rParam);
        else if(strcmp(rParam.m_ObjectId,"BU_SG_REL_")==0)
            m_listParameterArray[4].AddTail(rParam);
        else if(strcmp(rParam.m_ObjectId,"BU_BO_REL_")==0)
            m_listParameterArray[5].AddTail(rParam);
    }
}

/**
 * \brief Generates the message list
 *
 * This function will parse the input file and line by line
 * and generates  a list of message,signal,value table,comments,etc
 */
void CConverter::GenerateMessageList(CStdioFile& fileInput)
{
    char acLine[defCON_MAX_LINE_LEN],local_copy[defCON_MAX_LINE_LEN]; // I don't expect one line to be more than this
    int flag=0;
    // parsing the input file
    while(fileInput.ReadString(acLine,defCON_MAX_LINE_LEN))
    {

        char *pcToken, *pcLine;

        // avoid leading <spaces> before tokenising, so passing the
        // starting point will be correct in each case, when calling
        // msg.Format, sig.Format etc.
        strcpy(local_copy,acLine);
        pcLine = acLine;

        while(*pcLine && *pcLine == ' ')
        {
            *pcLine++;
        }

        pcToken = strtok(pcLine," :");

        if(pcToken)
        {
            //compare token to known types to interpret the line further

            // new line - skip
            if(strcmp(pcToken,"\n") == 0)
            {
                continue;
            }

            // message
            else if(strcmp(pcToken,"BO_") == 0)
            {
                CMessage msg;
                msg.Format(pcLine + strlen(pcToken)+1);

                // add the new message to the list
                if(strcmp(msg.m_acName,"VECTOR__INDEPENDENT_SIG_MSG") && !(msg.m_uiMsgID == 3221225472))
                {
                    CConverter::valid_msg = TRUE;
                    m_listMessages.AddTail(msg);
                }
                else
                    CConverter::valid_msg = FALSE;

            }

            // signal
            else if(strcmp(pcToken,"SG_") == 0)
            {
                CSignal sig;
                sig.Format(pcLine + strlen(pcToken)+1);
                //if signal associated with a msg add it to that perticular list
                //elses add it to msg independent list
                if(CConverter::valid_msg == TRUE)
                {
                    //insert signals in sorted order
                    POSITION pos = m_listMessages.GetTailPosition();
                    CMessage& msg = m_listMessages.GetAt(pos);
                    POSITION prevpos,possig = msg.m_listSignals.GetTailPosition();
                    int flag = 0;
                    if (possig == NULL)
                    {
                        msg.m_listSignals.AddHead(sig);
                        flag = 1;
                        continue;
                    }
                    int count =0;
                    while(possig != NULL)
                    {
                        count++;
                        prevpos = possig;
                        CSignal &sig1 = msg.m_listSignals.GetPrev(possig);
                        if(((sig1.m_ucWhichByte * 8) + sig1.m_ucStartBit) > ((sig.m_ucWhichByte * 8) + sig.m_ucStartBit))
                        {
                            msg.m_listSignals.InsertAfter(prevpos,sig);
                            flag = 1;
                            break;
                        }
                    }
                    if (flag == 0)
                    {
                        msg.m_listSignals.AddHead(sig);
                    }
                    // this signal should belong to the last message
                    msg.m_ucNumOfSignals++; // increment the signal count

                }
                else
                {
                    sig.m_ucWhichByte = 1;
                    sig.m_ucStartBit = 0;
                    m_listSignal.AddTail(sig);
                }
            }

            // value descriptor
            else if(strcmp(pcToken,"VAL_") == 0)
            {
                // <msgid><sp><signalName><sp><value1><sp><"desc1"><sp><value2><sp><"desc2"> ...;
                // get MsgId, find the message from the messagelist.
                // find the signal from the message, then add the value descritors
                // to the respective signals

                pcLine = pcLine + strlen(pcToken) + 1; // to get next token
                pcToken = strtok(pcLine," "); // msgid

                unsigned int id = (unsigned int)strtoul(pcToken, NULL, 10);

                if(id != 3221225472)
                {
                    POSITION posMsg = m_listMessages.GetHeadPosition();

                    while(posMsg != NULL)
                    {
                        CMessage& rMsg = m_listMessages.GetNext(posMsg);
                        // find matching message from list
                        if(rMsg.m_uiMsgID == id)
                        {
                            pcLine = pcLine + strlen(pcToken) + 1; // to get next token
                            pcToken = strtok(pcLine," "); // Signal name
                            POSITION posSig = rMsg.m_listSignals.GetHeadPosition();
                            // find matching signal
                            while(posSig != NULL)
                            {
                                CSignal& rSig =  rMsg.m_listSignals.GetNext(posSig);
                                if(strcmp(rSig.m_acName,pcToken) == 0)
                                {
                                    rSig.AddValueDescriptors(pcLine + strlen(pcToken) + 1,fileInput);
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
                    pcToken = strtok(pcLine," "); // Signal name

                    POSITION posSig = m_listSignal.GetHeadPosition();

                    // find matching signal
                    while(posSig != NULL)
                    {
                        CSignal& rSig =  m_listSignal.GetNext(posSig);
                        if(strcmp(rSig.m_acName,pcToken) == 0)
                        {
                            rSig.AddValueDescriptors(pcLine + strlen(pcToken) + 1,fileInput);
                            break; // if we got the signal we wanted
                        }
                    }

                }

            }
            // signal value qualifier
            else if(strcmp(pcToken,"SIG_VALTYPE_") == 0)
            {
                // <msgID> <signal name> : 1 -- float
                // <msgID> <signal name> : 2 -- double
                // get MsgId, find the message from the messagelist.
                // find the signal from the message, then update the
                // signal type appropriately of the respective signal

                pcToken = strtok(NULL," :;"); // msgid
                unsigned int id = (unsigned int)atoi(pcToken);
                POSITION posMsg = m_listMessages.GetHeadPosition();
                if(id != 3221225472)
                {

                    while(posMsg != NULL)
                    {
                        CMessage& rMsg = m_listMessages.GetNext(posMsg);
                        // find matching message from list
                        if(rMsg.m_uiMsgID == id)
                        {
                            pcToken = strtok(NULL," :;"); // Signal name
                            POSITION posSig = rMsg.m_listSignals.GetHeadPosition();

                            // find matching signal
                            while(posSig != NULL)
                            {
                                CSignal& rSig =  rMsg.m_listSignals.GetNext(posSig);
                                if(strcmp(rSig.m_acName,pcToken) == 0)
                                {
                                    if(pcToken = strtok(NULL," :;")) // qualifier (1 or 2)
                                    {
                                        // update signal type based on qualifier
                                        switch(*pcToken)
                                        {
                                            case '1':
                                                rSig.m_ucType = CSignal::SIG_TYPE_FLOAT;
                                                break;
                                            case '2':
                                                rSig.m_ucType = CSignal::SIG_TYPE_DOUBLE;
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
                    pcToken = strtok(NULL," :;"); // Signal name
                    POSITION posSig = m_listSignal.GetHeadPosition();
                    // find matching signal
                    while(posSig != NULL)
                    {
                        CSignal& rSig =  m_listSignal.GetNext(posSig);
                        if(strcmp(rSig.m_acName,pcToken) == 0)
                        {
                            if(pcToken = strtok(NULL," :;")) // qualifier (1 or 2)
                            {
                                // update signal type based on qualifier
                                switch(*pcToken)
                                {
                                    case '1':
                                        rSig.m_ucType = CSignal::SIG_TYPE_FLOAT;
                                        break;
                                    case '2':
                                        rSig.m_ucType = CSignal::SIG_TYPE_DOUBLE;
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
            else if(strcmp(pcToken,"VAL_TABLE_") == 0)
            {
                CValueTable tab;
                pcToken = strtok(pcLine," ");
                tab.Format(pcLine + strlen(pcToken)+1,fileInput);
                m_vTab.AddTail(tab);
            }
            //comments
            else if(strcmp(pcToken,"CM_") == 0)
            {
                pcLine = pcLine + strlen(pcToken) + 1;
                pcToken = strtok(pcLine," ");
                CComment cm;
                CString comment;
                //comments related to node
                if(strcmp(pcToken,"BU_") == 0)
                {
                    pcToken = strtok(NULL," ");
                    cm.m_elementName= pcToken;
                    pcToken = strtok(NULL,"");
                    comment = pcToken;
                    while(strstr(pcToken,"\";") == NULL)
                    {
                        fileInput.ReadString(acLine,defCON_MAX_LINE_LEN);
                        pcToken = acLine;
                        comment = comment + pcToken;
                    }
                    cm.m_comment= comment;
                    m_cmNode.AddTail(cm);
                }
                //comments related to messages
                else if(strcmp(pcToken,"BO_") == 0)
                {
                    pcToken = strtok(NULL," ");
                    cm.m_msgID= atoi(pcToken);

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
                    pcToken = strtok(NULL,"");
                    comment = pcToken;
                    while(strstr(pcToken,"\";") == NULL)
                    {
                        fileInput.ReadString(acLine,defCON_MAX_LINE_LEN);
                        pcToken = acLine;
                        comment = comment + pcToken;
                    }
                    cm.m_comment= comment;
                    m_cmMsg.AddTail(cm);
                }
                //comments related to signals
                else if(strcmp(pcToken,"SG_") == 0)
                {
                    pcToken = strtok(NULL," ");
                    cm.m_msgID= atoi(pcToken);
                    if(cm.m_msgID < 0x80000000UL)
                    {
                        cm.m_msgType = 'S';
                    }
                    else
                    {
                        cm.m_msgType = 'X';
                        cm.m_msgID &= 0x7FFFFFFF;
                    }
                    pcToken = strtok(NULL," ");
                    cm.m_elementName = pcToken;
                    pcToken = strtok(NULL,"");
                    comment = pcToken;
                    while(strstr(pcToken,"\";") == NULL)
                    {
                        fileInput.ReadString(acLine,defCON_MAX_LINE_LEN);
                        pcToken = acLine;
                        comment = comment + pcToken;
                    }
                    cm.m_comment= comment;
                    m_cmSig.AddTail(cm);
                }
                //comments related to network
                else
                {
                    comment = pcToken;
                    while(strstr(pcToken,"\";") == NULL)
                    {
                        fileInput.ReadString(acLine,defCON_MAX_LINE_LEN);
                        pcToken = acLine;
                        comment = comment + pcToken;
                    }
                    cm.m_comment= comment;
                    m_cmNet.AddTail(cm);
                }

            }
            // node
            else if(strcmp(pcToken,"BU_") == 0)
            {
                create_Node_List(pcLine + strlen(pcToken)+1);
            }

            else if (strcmp(pcToken,"BA_DEF_")==0 || strcmp(pcToken,"BA_DEF_REL_")==0)
            {
                CParameters pObj;
                pObj.Format(pcLine + strlen(pcToken) + 1); // to get next token
                m_listParameters.AddTail(pObj);
            }

            //Param Initial Values
            else if(strcmp(pcToken,"BA_DEF_DEF_")==0 )
            {
                char acTemp[defCON_TEMP_LEN],*pcTemp;
                pcTemp = acTemp;
                pcToken=strtok(NULL,"\"");
                pcToken=strtok(NULL,"\"");
                flag=0;
                while(*pcToken && *pcToken != '"')
                {
                    *pcTemp++ = *pcToken++;
                }
                *pcTemp = '\0';
                POSITION posMsg = m_listParameters.GetHeadPosition();
                while(posMsg != NULL)
                {
                    CParameters& rParam = m_listParameters.GetNext(posMsg);
                    // find matching Parameter from list
                    if(strcmp(rParam.m_ParamName,acTemp)==0 )
                    {
                        pcTemp=acTemp;
                        pcToken = strtok(NULL,";"); // default val
                        rParam.ReadDefaultVal(pcToken);
                        flag=1;
                        break;
                    }
                }
                if(posMsg==NULL && flag==0)
                {
                    CString errString="BA_DEF_DEF_ \"";
                    errString=errString+acTemp+"\" ";
                    errString=errString+pcToken;
                    errString+=" : Match not Found in Param List\n";
                    defList.AddTail(errString);
                }

            }

            //RX,Tx Parameter Definition
            else if(strcmp(pcToken,"BA_DEF_DEF_REL_")==0)
            {
                char acTemp[defCON_TEMP_LEN],*pcTemp;
                pcTemp = acTemp;
                flag=0;
                pcToken=strtok(NULL,"\"");
                while(*pcToken && *pcToken != '"')
                {
                    *pcTemp++ = *pcToken++; // copy SIG_NAME only, i.e. till first 'space'
                }
                *pcTemp = '\0';
                POSITION posMsg = m_listParameters.GetHeadPosition();
                while(posMsg != NULL)
                {
                    CParameters& rParam = m_listParameters.GetNext(posMsg);
                    // find matching Parameter from list
                    if(strcmp(rParam.m_ParamName,acTemp)==0 )
                    {
                        pcTemp=acTemp;
                        pcToken = strtok(NULL,";"); // default val
                        rParam.ReadDefaultVal(pcToken);
                        flag=1;
                        break;
                    }
                }
                if(posMsg==NULL && flag==0)
                {
                    CString errString="BA_DEF_DEF_REL \"";
                    errString=errString+acTemp+"\" ";
                    errString=errString+pcToken;
                    errString+=" : Match not Found in Param List\n";
                    defList.AddTail(errString);

                }
            }

            // Parameter Other values //
            else if(strcmp(pcToken,"BA_")==0)
            {
                char acTemp[defCON_TEMP_LEN],*pcTemp;
                pcTemp = acTemp;
                while(*pcLine && *pcLine==' ')
                {
                    *pcLine++;
                }

                //get Param name
                pcLine=pcLine + strlen(pcToken) + 1;
                pcToken=strtok(pcLine,"\"");

                while(*pcToken && *pcToken != '"')
                {
                    *pcTemp++ = *pcToken++;
                }
                *pcTemp = '\0';

                POSITION posMsg = m_listParameters.GetHeadPosition();
                while(posMsg != NULL)
                {
                    CParameters& rParam = m_listParameters.GetNext(posMsg);
                    // find matching Parameter from list
                    if(strcmp(rParam.m_ParamName,acTemp)==0 )
                    {
                        rParam.FormatParamValue((pcLine+ strlen(acTemp)+3 ) ); // to get next token
                        pcTemp=acTemp;
                        break;
                    }
                }
            }
            //maintain a list of lines not processed
            else
            {
                CString str = local_copy;
                m_notProcessed.AddTail(str);
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
bool CConverter::WriteToOutputFile(CStdioFile& fileOutput)
{
    bool bResult = true;
    char acLine[defCON_MAX_LINE_LEN]; // I don't expect one line to be more than this
    // write to the output file
    // write header
    fileOutput.WriteString(CConverter::m_accHeader);
    fileOutput.WriteString(T_DB_VER" "T_VER_NO);
    fileOutput.WriteString("\n");
    // number of messages
    sprintf(acLine,"\n"T_NUM_OF_MSG" %d\n\n",m_listMessages.GetCount());
    fileOutput.WriteString(acLine);

    //Write Messagess to the Output file
    CMessage msg;
    bResult &= msg.writeMessageToFile(fileOutput,m_listMessages,false);

    // write all messages, signals not associated with any Messages
    if(m_listSignal.IsEmpty() == 0)
    {
        fileOutput.WriteString(T_ST_SIG_LIST"\n");
        CSignal sig;
        bResult &= sig.WriteSignaltofile (fileOutput,m_listSignal,0,0,false);
        fileOutput.WriteString(T_END_SIG_LIST"\n\n");
    }

    //write value table
    fileOutput.WriteString(T_ST_VAL_TAB"\n");
    CValueTable temp_vtab;
    temp_vtab.writeValueTabToFile (fileOutput,m_vTab);
    fileOutput.WriteString(T_END_VAL_TAB"\n");

    //write list of nodes
    fileOutput.WriteString("\n"T_NODE" ");
    POSITION pos = m_listNode.GetHeadPosition();
    bool comma = false;
    while(pos != NULL)
    {
        CString node = m_listNode.GetNext(pos);
        if(comma)
            node = ","+node;
        fileOutput.WriteString(node);
        comma = true;
    }

    //Write Comments
    fileOutput.WriteString("\n"T_ST_COMMENT"\n"T_ST_CM_NET"\n");
    //network comments
    pos = m_cmNet.GetHeadPosition();
    while(pos != NULL)
    {
        CComment &cmt = m_cmNet.GetNext(pos);
        cmt.m_elementName += " " + cmt.m_comment;
        fileOutput.WriteString(cmt.m_elementName);
    }

    fileOutput.WriteString(T_END_CM_NET"\n\n"T_ST_CM_NODE"\n");
    //node comments
    pos = m_cmNode.GetHeadPosition();
    while(pos != NULL)
    {
        CComment &cmt = m_cmNode.GetNext(pos);
        cmt.m_elementName += " " + cmt.m_comment;
        fileOutput.WriteString(cmt.m_elementName);
    }

    fileOutput.WriteString(T_END_CM_NODE"\n\n"T_ST_CM_MSG"\n");
    //message comments
    pos = m_cmMsg.GetHeadPosition();
    while(pos != NULL)
    {
        CComment &cmt = m_cmMsg.GetNext(pos);
        char c_msgID[20];
        sprintf(c_msgID,"%u",cmt.m_msgID);
        CString s_cmt;
        s_cmt = c_msgID;
        s_cmt = s_cmt + " ";
        s_cmt = s_cmt + cmt.m_msgType ;
        s_cmt = s_cmt + " " + cmt.m_comment;
        fileOutput.WriteString(s_cmt);
    }

    fileOutput.WriteString(T_END_CM_MSG"\n\n"T_ST_CM_SIG"\n");
    //signal comments
    pos = m_cmSig.GetHeadPosition();
    while(pos != NULL)
    {
        CComment &cmt = m_cmSig.GetNext(pos);
        char c_msgID[20];
        sprintf(c_msgID,"%u",cmt.m_msgID);
        CString s_cmt;
        s_cmt = c_msgID;
        s_cmt += " ";
        s_cmt += cmt.m_msgType;
        s_cmt += " " + cmt.m_elementName + " "+ cmt.m_comment;
        fileOutput.WriteString(s_cmt);
    }
    fileOutput.WriteString(T_END_CM_SIG"\n"T_END_COMMENT"\n");

    //Write Parameters to the output file.
    fileOutput.WriteString("\n"START_PARAM_TAG"\n");
    fileOutput.WriteString(START_NETPARAM_TAG"\n");

    bResult=bResult & WriteParametersToFile(fileOutput,m_listParameterArray[0]);
    fileOutput.WriteString(END_NETPARAM_TAG"\n\n");
    fileOutput.WriteString(START_NODEPARAM_TAG"\n");

    bResult=bResult & WriteParametersToFile(fileOutput,m_listParameterArray[1]);
    fileOutput.WriteString(END_NODEPARAM_TAG"\n\n");
    fileOutput.WriteString(START_MSGPARAM_TAG"\n");

    bResult=bResult & WriteParametersToFile(fileOutput,m_listParameterArray[2]);

    fileOutput.WriteString(END_MSGPARAM_TAG"\n\n");
    fileOutput.WriteString(START_SIGPARAM_TAG"\n");

    bResult=bResult & WriteParametersToFile(fileOutput,m_listParameterArray[3]);

    fileOutput.WriteString(END_SIGPARAM_TAG"\n\n");
    fileOutput.WriteString (START_RXPARAM_TAG"\n");
    bResult=bResult & WriteParametersToFile(fileOutput,m_listParameterArray[4]);
    fileOutput.WriteString (END_RXPARAM_TAG"\n\n");
    fileOutput.WriteString (START_TXPARAM_TAG"\n");
    bResult=bResult & WriteParametersToFile(fileOutput,m_listParameterArray[5]);
    fileOutput.WriteString (END_TXPARAM_TAG"\n");
    fileOutput.WriteString(END_PARAM_TAG"\n\n");

    //Parameter Values
    fileOutput.WriteString(START_PARAMVAL_TAG"\n");
    fileOutput.WriteString(START_NETVAL_TAG"\n");
    pos=m_listParameterArray[0].GetHeadPosition();
    while(pos!=NULL)
    {
        CParameters& rParam=m_listParameterArray[0].GetNext(pos);
        POSITION m_vPos=rParam.m_listParamValues[0].GetHeadPosition();
        while(m_vPos!=NULL)
        {
            CParameterValues& vParam=rParam.m_listParamValues[0].GetNext(m_vPos);
            vParam.WriteNetValuesToFile(fileOutput,rParam.m_ParamType,rParam.m_ParamName);
        }

    }
    fileOutput.WriteString(END_NETVAL_TAG"\n\n");
    fileOutput.WriteString(START_NODEVAL_TAG"\n");
    pos=m_listParameterArray[1].GetHeadPosition();
    while(pos!=NULL)
    {
        CParameters& rParam=m_listParameterArray[1].GetNext(pos);
        POSITION m_vPos=rParam.m_listParamValues[1].GetHeadPosition();
        while(m_vPos!=NULL)
        {
            CParameterValues& vParam=rParam.m_listParamValues[1].GetNext(m_vPos);
            vParam.WriteNodeValuesToFile(fileOutput,rParam.m_ParamType,rParam.m_ParamName);
        }
    }

    fileOutput.WriteString(END_NODEVAL_TAG"\n\n");
    fileOutput.WriteString(START_MSGVAL_TAG"\n");
    pos=m_listParameterArray[2].GetHeadPosition();
    while(pos!=NULL)
    {
        CParameters& rParam=m_listParameterArray[2].GetNext(pos);
        POSITION m_vPos=rParam.m_listParamValues[2].GetHeadPosition();
        while(m_vPos!=NULL)
        {
            CParameterValues& vParam=rParam.m_listParamValues[2].GetNext(m_vPos);
            vParam.WriteMesgValuesToFile(fileOutput,rParam.m_ParamType,rParam.m_ParamName);
        }
    }
    fileOutput.WriteString(END_MSGVAL_TAG"\n\n");
    fileOutput.WriteString(START_SIGVAL_TAG"\n");
    pos=m_listParameterArray[3].GetHeadPosition();
    while(pos!=NULL)
    {
        CParameters& rParam=m_listParameterArray[3].GetNext(pos);
        POSITION m_vPos=rParam.m_listParamValues[3].GetHeadPosition();
        while(m_vPos!=NULL)
        {
            CParameterValues& vParam=rParam.m_listParamValues[3].GetNext(m_vPos);
            vParam.WriteSigValuesToFile(fileOutput,rParam.m_ParamType,rParam.m_ParamName);
        }

    }
    fileOutput.WriteString(END_SIGVAL_TAG"\n\n");
    fileOutput.WriteString(END_PARAMVAL_TAG"\n\n");

    //list of not supported
    fileOutput.WriteString("\n"T_ST_NOT_SUP"\n");
    msg.writeMessageToFile(fileOutput,m_unsupList,true);
    fileOutput.WriteString(T_END_NOT_SUP"\n");

    //lines that were not processed
    fileOutput.WriteString("\n"T_ST_NOT_PRO"\n");
    pos = m_notProcessed.GetHeadPosition();
    while(pos != NULL)
        fileOutput.WriteString(m_notProcessed.GetNext(pos));
    fileOutput.WriteString("\n"T_END_NOT_PRO"\n");

    return bResult;
}

/**
 * \brief Logs the eror info in log file
 *
 * Logs the eror info in log file.
 */
void CConverter::CreateLogFile(CStdioFile& fileLog)
{
    char acLine[defCON_MAX_LINE_LEN]; // I don't expect one line to be more than this
    // write to the output file
    char acMsgLine[200];

    sprintf(acLine,"Conversion Error Log \n\n");
    fileLog.WriteString(acLine);

    POSITION pos = m_listMessages.GetHeadPosition();
    while(pos != NULL)
    {
        acMsgLine[0] = '\0';
        CMessage& msg = m_listMessages.GetNext(pos);
        POSITION posSig = msg.m_listSignals.GetHeadPosition();
        while(posSig != NULL)
        {
            CSignal& sig = msg.m_listSignals.GetNext(posSig);
            // write signal only if it is not valid
            if(sig.m_uiError != CSignal::SIG_EC_NO_ERR)
            {
                // for the first wrong signal, log the message details also
                if(acMsgLine[0] == '\0')
                {
                    sprintf(acMsgLine,"\nMSG_ID: %u \tMSG_TYPE: %c \tMSG_NAME: %s\n", msg.m_uiMsgID,msg.m_cFrameFormat,msg.m_acName);
                    fileLog.WriteString(acMsgLine);
                }
                sprintf(acLine,"\tSIG_NAME: %s, %s, ACTION: %s\n",sig.m_acName,sig.GetErrorString(),sig.GetErrorAction());
                fileLog.WriteString(acLine);
            }
        }
    }

    //log errors in the signal list.
    acMsgLine[0] = '\0';
    POSITION posSig = m_listSignal.GetHeadPosition();
    while(posSig != NULL)
    {
        CSignal& sig = m_listSignal.GetNext(posSig);
        // write signal only if it is not valid
        if(sig.m_uiError != CSignal::SIG_EC_OVERFLOW)
        {
            // for the first wrong signal, log the message details also
            if(acMsgLine[0] == '\0')
            {
                sprintf(acMsgLine,"\nMSG_ID: 1073741824 \tMSG_TYPE: X \tMSG_NAME: VECTOR__INDEPENDENT_SIG_MSG\n");
                fileLog.WriteString(acMsgLine);
            }
            sprintf(acLine,"\tSIG_NAME: %s, %s, ACTION: %s\n",sig.m_acName,sig.GetErrorString(),sig.GetErrorAction());
            fileLog.WriteString(acLine);
        }
    }
    pos=defList.GetHeadPosition();
    while(pos!=NULL)
    {
        CString str=defList.GetNext(pos);
        fileLog.WriteString(str);
    }
    pos=m_listParameters.GetHeadPosition();
    while(pos!=NULL)
    {
        CParameters& rParam=m_listParameters.GetNext(pos);
        if(rParam.m_defError)
        {
            sprintf(acLine,"OBJECT ID : %s\tPARAM_NAME :\"%s\"\n\tDescription:Default Value tag(BA_DEF_DEF_) doesn;t exist \t Action Taken : Reset to default value\n",rParam.m_ObjectId ,rParam.m_ParamName);
            fileLog.WriteString(acLine);
        }
        if(rParam.m_RangeError)
        {
            sprintf(acLine,"OBJECT ID : %s\tPARAM_NAME :\"%s\"\n\tDescription: Invalid Data Ranges\t Action Taken : Reset to default value\n",rParam.m_ObjectId ,rParam.m_ParamName);
            fileLog.WriteString(acLine);
        }
    }
}

/**
 * \brief     creates a list of nodes inthe network
 * \param[in] pcLine String having all the node names
 *
 * Creates a list of nodes inthe network.
 */
void CConverter::create_Node_List(char *pcLine)
{
    char* pcToken;
    // get the MSG ID
    pcToken = strtok(pcLine," ,");
    while(pcToken)
    {
        CString str = pcToken;
        m_listNode.AddTail(str);
        pcToken = strtok(NULL," ,");
    }
}

/**
 * \brief     encrypts all the strings present in the list
 * \param[in] m_notProcessed List of strings
 *
 * Encrypts all the strings present in the list
 */
void CConverter::EncryptData(CList<CString,CString& > &m_notProcessed)
{
    char c_str[defCON_MAX_LINE_LEN];
    CString str;
    POSITION prev_pos,pos = m_notProcessed.GetHeadPosition();
    while(pos != NULL)
    {
        prev_pos = pos;
        //read the string at the position
        str = m_notProcessed.GetNext(pos);
        //make a local copy
        strcpy(c_str,str);
        for(int i=0; i<(int)strlen(c_str); i++)
        {
            if ((c_str[i] >= 'a' && c_str[i] <= 'm') || (c_str[i] >= 'A' && c_str[i] <= 'M'))
            {
                c_str[i] = c_str[i] + 13;
            }
            else if ((c_str[i] >= 'n' && c_str[i] <= 'z') || (c_str[i] >= 'N' && c_str[i] <= 'Z'))
                c_str[i] = c_str[i] - 13;
        }
        str = c_str;
        //put it back at the same position
        m_notProcessed.SetAt(prev_pos,str);
    }
}
