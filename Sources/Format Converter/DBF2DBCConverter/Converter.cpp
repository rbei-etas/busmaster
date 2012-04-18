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
 * \brief     Implementation of the CConverter class.
 * \authors   Mahesh.B.S, Padmaja.A, Tobias Lorenz
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Implementation of the CConverter class.
 */

/* C++ includes */
#include <fstream>
#include <string>

/* Project includes */
#include "Converter.h"

fstream CConverter::fileLog;
bool CConverter::bLOG_ENTERED = false;
unsigned int CConverter::ucMsg_DLC = 8;

/**
 * \brief Constructor
 *
 * Constructor of CConverter
 */
CConverter::CConverter()
{
    m_uiResultCode = CON_RC_NOERROR;
}

/**
 * \brief Destructor
 *
 * Destructor of CConverter
 */
CConverter::~CConverter()
{
}

/**
 * \brief     Converter function
 * \param[in] sCanMonFile Input file name
 * \param[in] sCanoeFile Output file name
 * \return    Status code
 *
 * This is the basic function which is to be called
 * to convert any given CANMon file to a CANoe file.
 */
unsigned int CConverter::Convert(string sCanMonFile,string sCanoeFile)
{
    fstream fileInput,fileOutput;
    char acLine[defCON_MAX_LINE_LEN]; // I don't expect one line to be more than this
    fileInput.open(sCanMonFile.c_str(), fstream::in);

    if(!fileInput.is_open())
    {
        return SetResultCode(CON_RC_FILEOPEN_ERROR_INFILE);
    }

    // first line of input file starts with header followed by a version no, else file format error
    if(fileInput.getline(acLine,defCON_MAX_LINE_LEN) == NULL)
    {
        // eof file reached without reading anything
        fileInput.close();
        return SetResultCode(CON_RC_FORMAT_ERROR_INFILE);
    }
    else // if something was read
    {
        fileInput.getline(acLine,defCON_MAX_LINE_LEN);
        fileInput.getline(acLine,defCON_MAX_LINE_LEN);
        // verify the format
        /*if(strncmp(acLine,"[DATABASE_VERSION] ",19) != 0)
        {
        fileInput.Close();
        return SetResultCode(CON_RC_FORMAT_ERROR_INFILE);
        }*/
    }

    //Create log file
    CConverter::bLOG_ENTERED = false;
    string sLogFile = sCanoeFile.substr(0, sCanoeFile.length()-4);
    sLogFile += ".log";
    m_omLogFilePath = sLogFile;
    fileLog.open(sLogFile.c_str(), fstream::out);

    if(!fileLog.is_open())
    {
        return SetResultCode(CON_RC_FILEOPEN_ERROR_LOGFILE);
    }

    // Generate the list of messages
    GenerateMessageList(fileInput);
    DecryptData(m_notProcessed);
    // All information gathered, validate and update if necessary
    // Make appropriate changes in the contents of the list
    ValidateMessageList();
    // the format is OK then open the output file
    fileOutput.open(sCanoeFile.c_str(), fstream::out);

    if(!fileOutput.is_open())
    {
        // if output file cannot be opened the close the input file
        // and return the error code
        fileInput.close();
        return SetResultCode(CON_RC_FILEOPEN_ERROR_OUTFILE);
    }

    bool bRes = WriteToOutputFile(fileOutput);
    fileInput.close();
    fileOutput.close();

    //If any error log it in the log file.
    if(!bRes)
    {
        CreateLogFile(fileLog);
        fileLog.close();
        CConverter::bLOG_ENTERED = true;
        return SetResultCode(CON_RC_COMPLETED_WITH_ERROR);
    }

    fileLog.close();

    //If log file is empty delete it
    if(!CConverter::bLOG_ENTERED)
    {
        DeleteFile(sLogFile.c_str());
    }
    else
    {
        return SetResultCode(CON_RC_COMPLETED_WITH_ERROR);
    }

    return m_uiResultCode;
}

/**
 * \brief     Set Result Code
 * \param[in] uiCode Result code
 * \return    Result code
 *
 * Set the result code for the convertor class.This function
 * is called only on error
 */
unsigned int CConverter::SetResultCode(unsigned int uiCode)
{
    return (m_uiResultCode = uiCode);
}

/**
 * \brief      Get Result String
 * \param[out] str Result String depending on m_uiResultCode
 *
 * Returns the result string.
 */
void CConverter::GetResultString(string& str)
{
    switch(m_uiResultCode)
    {
        case 0:
            str = "Conversion completed.";
            break;

        case 1:
            str = "Conversion completed with warnings. See log file.";
            break;

        case 2:
            str = "Conversion aborted. Error opening input file.";
            break;

        case 3:
            str = "Conversion aborted. Error creating output file.";
            break;

        case 4:
            str = "Conversion aborted. Error with input file format.";
            break;

        case 5:
            str = "Conversion aborted. Error creating log file.";
            break;

        default:
            str = "Unknown";
            break;
    }
}

/**
 * \brief     Generate Message List
 * \param[in] fileInput Input file
 *
 * This function will parse the input file and line by line
 * and generates a list of message,signal,value table,comments,etc
 */
void CConverter::GenerateMessageList(fstream& fileInput)
{
    char acLine[defCON_MAX_LINE_LEN]; // I don't expect one line to be more than this
    bool valTab = false;
    CMessage& msg();
    CSignal& sig();
    POSITION posMsg,posSig;
    // parsing the input file

    while(fileInput.getline(acLine,defCON_MAX_LINE_LEN))
    {
        char* pcToken, *pcLine;
        // avoid leading <spaces> before tokenising, so passing the
        // starting point will be correct in each case, when calling
        // msg.Format, sig.Format etc.
        pcLine = acLine;

        while(*pcLine && *pcLine == ' ')
        {
            *pcLine++;
        }

        pcToken = strtok(pcLine," ");

        if(pcToken)
        {
            //compare token to known types to interpret the line further

            // new line - skip
            if(strcmp(pcToken,"\n") == 0)
            {
                continue;
            }
            // message
            else if(strcmp(pcToken,"[START_MSG]") == 0)
            {
                CMessage msg;
                msg.Format(pcLine + strlen(pcToken)+1);
                posMsg = m_listMessages.Find(msg);

                if(posMsg == NULL)
                {
                    posMsg = m_listMessages.AddTail(msg);
                }
            }
            else if(strcmp(pcToken,"[START_SIG_LIST]\n") == 0)
            {
                CMessage msg;
                msg.m_sName = "VECTOR__INDEPENDENT_SIG_MSG";
                msg.m_sTxNode = "Vector__XXX";
                msg.m_ucLength = 0;
                msg.m_uiMsgID = 3221225472;
                CConverter::ucMsg_DLC = 8;
                posMsg = m_listMessages.AddHead(msg);
                fileInput.getline(acLine,defCON_MAX_LINE_LEN);
                pcToken = strtok(pcLine," ");

                while(strcmp(acLine,"[END_SIG_LIST]\n") != 0)
                {
                    CSignal sig;
                    pcToken = strtok(pcLine," ");

                    if(strcmp(pcToken,"[START_SIGNALS]") == 0)
                    {
                        sig.Format(pcLine + strlen(pcToken)+1);
                        sig.m_ucStartBit = 0;
                        CMessage& inMsg = m_listMessages.GetHead();
                        posSig = inMsg.m_listSignals.AddHead(sig);
                    }
                    else if(strcmp(pcToken,"[VALUE_DESCRIPTION]") == 0)
                    {
                        CValueDescriptor val;
                        val.Format(pcLine + strlen(pcToken)+1);
                        CMessage& inMsg = m_listMessages.GetHead();
                        CSignal& inSig = inMsg.m_listSignals.GetAt(posSig);
                        inSig.m_listValueDescriptor.AddHead(val);
                    }

                    fileInput.getline(acLine,defCON_MAX_LINE_LEN);
                }
            }
            else if(strcmp(pcToken,"[START_SIGNALS]") == 0)
            {
                CSignal sig;
                sig.Format(pcLine + strlen(pcToken)+1);
                CMessage& msg = m_listMessages.GetAt(posMsg);
                posSig = msg.m_listSignals.AddHead(sig);
            }
            else if(strcmp(pcToken,"[VALUE_DESCRIPTION]") == 0 && valTab == false)
            {
                CValueDescriptor val;
                val.Format(pcLine + strlen(pcToken)+1);
                CMessage& msg = m_listMessages.GetAt(posMsg);
                CSignal& sig = msg.m_listSignals.GetAt(posSig);
                sig.m_listValueDescriptor.AddHead(val);
            }
            else if(strcmp(pcToken,"[START_NOT_SUPPORTED]\n") == 0)
            {
            }
            //nodes
            else if(strcmp(pcToken,"[NODE]") == 0)
            {
                create_Node_List(pcLine + strlen(pcToken)+1);
            }
            else if(strcmp(pcToken,"[START_VALUE_TABLE]\n")==0)
            {
                while(fileInput.getline(acLine,defCON_MAX_LINE_LEN) && strcmp(acLine, "[END_VALUE_TABLE]\n")!=0)
                {
                    pcLine = acLine;
                    pcToken=strtok(pcLine," ");

                    if(strcmp(pcToken,"[START_TABLE]")==0)
                    {
                        CValueTable vTab;
                        vTab.Format_ValueTable(pcLine + strlen(pcToken)+1,fileInput);
                        m_vTab.AddTail(vTab);
                    }
                }
            }
            else if(strcmp(pcToken,"[START_PARAM]\n")==0)
            {
                while(fileInput.getline(acLine,defCON_MAX_LINE_LEN) && strcmp(acLine, "[END_PARAM]\n")!=0)
                {
                    pcLine = acLine;
                    CParameters rParam;

                    if(strcmp(pcLine,"[START_PARAM_NET]\n")==0)
                    {
                        while(fileInput.getline(acLine, defCON_MAX_LINE_LEN) && strcmp(acLine, "[END_PARAM_NET]\n")!=0)
                        {
                            pcLine = acLine;
                            rParam.Format_ParamDef(pcLine,0);
                            m_listParameterArray[0].AddTail(rParam);
                        }
                    }
                    else if(strcmp(pcToken,"[START_PARAM_NODE]\n")==0)
                    {
                        while(fileInput.getline(acLine, defCON_MAX_LINE_LEN) && strcmp(acLine, "[END_PARAM_NODE]\n")!=0)
                        {
                            pcLine = acLine;
                            rParam.Format_ParamDef(pcLine,1);
                            m_listParameterArray[1].AddTail(rParam);
                        }
                    }
                    else if(strcmp(pcToken,"[START_PARAM_MSG]\n")==0)
                    {
                        while(fileInput.getline(acLine, defCON_MAX_LINE_LEN) && strcmp(acLine, "[END_PARAM_MSG]\n")!=0)
                        {
                            pcLine = acLine;
                            rParam.Format_ParamDef(pcLine,2);
                            m_listParameterArray[2].AddTail(rParam);
                        }
                    }
                    else if(strcmp(pcToken,"[START_PARAM_SIG]\n")==0)
                    {
                        while(fileInput.getline(acLine, defCON_MAX_LINE_LEN) && strcmp(acLine, "[END_PARAM_SIG]\n")!=0)
                        {
                            pcLine = acLine;
                            rParam.Format_ParamDef(pcLine,3);
                            m_listParameterArray[3].AddTail(rParam);
                        }
                    }
                    else if(strcmp(pcToken,"[START_PARAM_NODE_RX_SIG]\n")==0)
                    {
                        while(fileInput.getline(acLine, defCON_MAX_LINE_LEN) && strcmp(acLine, "[END_PARAM_NODE_RX_SIG]\n")!=0)
                        {
                            pcLine = acLine;
                            rParam.Format_ParamDef(pcLine,4);
                            m_listParameterArray[4].AddTail(rParam);
                        }
                    }
                    else if(strcmp(pcToken,"[START_PARAM_NODE_TX_MSG]\n")==0)
                    {
                        while(fileInput.getline(acLine, defCON_MAX_LINE_LEN) && strcmp(acLine, "[END_PARAM_NODE_TX_MSG]\n")!=0)
                        {
                            pcLine = acLine;
                            rParam.Format_ParamDef(pcLine,5);
                            m_listParameterArray[5].AddTail(rParam);
                        }
                    }
                }
            }
            else if(strcmp(pcToken,"[START_PARAM_VAL]\n")==0)
            {
                while(fileInput.getline(acLine, defCON_MAX_LINE_LEN) && strcmp(acLine, "[END_PARAM_VAL]\n")!=0)
                {
                    pcLine = acLine;
                    CParameters tParam;

                    if(strcmp(pcLine,"[START_PARAM_NET_VAL]\n")==0)
                    {
                        tParam.Format_NetParam_Value(fileInput,m_listParameterArray[0]);
                    }
                    else if(strcmp(pcLine,"[START_PARAM_NODE_VAL]\n")==0)
                    {
                        tParam.Format_NodeParam_Value(fileInput,m_listParameterArray[1]);
                    }
                    else if(strcmp(pcLine,"[START_PARAM_MSG_VAL]\n")==0)
                    {
                        tParam.Format_MesgParam_Value(fileInput,m_listParameterArray[2]);
                    }
                    else if(strcmp(pcLine,"[START_PARAM_SIG_VAL]\n")==0)
                    {
                        tParam.Format_SigParam_Value(fileInput,m_listParameterArray[3]);
                    }
                }
            }
            else if(strcmp(pcToken,"[START_DESC]\n")==0)
            {
                while(fileInput.getline(acLine, defCON_MAX_LINE_LEN) && strcmp(acLine, "[END_DESC]\n")!=0)
                {
                    pcLine = acLine;
                    CComment tCmt;

                    if(strcmp(pcLine,"[START_DESC_NET]\n")==0)
                    {
                        tCmt.Format_netComment(fileInput, m_listComments[0]);
                    }
                    else if(strcmp(pcLine,"[START_DESC_NODE]\n")==0)
                    {
                        tCmt.Format_nodeComment(fileInput, m_listComments[1]);
                    }
                    else if(strcmp(pcLine,"[START_DESC_MSG]\n")==0)
                    {
                        tCmt.Format_msgComment(fileInput, m_listComments[2]);
                    }
                    else if(strcmp(pcLine,"[START_DESC_SIG]\n")==0)
                    {
                        tCmt.Format_sigComment(fileInput, m_listComments[3]);
                    }
                }
            }
            else if(strcmp(pcToken,"[START_NOT_PROCESSED]\n") == 0)
            {
                fileInput.getline(acLine,defCON_MAX_LINE_LEN);

                while(strcmp(pcToken,"[END_NOT_PROCESSED]\n") != 0)
                {
                    string str = acLine;
                    m_notProcessed.AddTail(str);
                    fileInput.getline(acLine, defCON_MAX_LINE_LEN);
                }

                continue;
            }
            //if any other tag continue
            else
            {
                continue;
            }
        }
    }

    return ;
}

/**
 * \brief Validate Message List
 *
 * Validates the message list and set the error in each signal
 * if present.
 */
void CConverter::ValidateMessageList(void)
{
    POSITION pos = m_listMessages.GetHeadPosition();

    while(pos != NULL)
    {
        CMessage& msg = m_listMessages.GetNext(pos);
        POSITION possig = msg.m_listSignals.GetHeadPosition();

        while(possig != NULL)
        {
            CSignal& sig = msg.m_listSignals.GetNext(possig);
            sig.Validate();
        }
    }
}

/**
 * \brief     Write to Output File
 * \param[in] fileOutput Output file
 * \return    Status code
 *
 * Writes all the data to the output file in CANoe format
 */
bool CConverter::WriteToOutputFile(fstream& fileOutput)
{
    bool bResult = true;
    // write to the output file
    // write header
    fileOutput << "VERSION \"\"" << endl;
    fileOutput << endl;
    fileOutput << endl;
    fileOutput << "BS_:" << endl;
    fileOutput << endl;
    fileOutput << "BU_:";
    //write all nodes
    POSITION pos = m_listNode.GetHeadPosition();

    while(pos != NULL)
    {
        string& node = m_listNode.GetNext(pos);
        fileOutput << " " << node.c_str();
    }

    fileOutput << endl;
    fileOutput << endl;
    //Value Table
    CValueTable temp_vtab;
    temp_vtab.writeValueTabToFile (fileOutput,m_vTab);
    fileOutput << endl;
    //write messages
    pos = m_listMessages.GetHeadPosition();

    while(pos != NULL)
    {
        CMessage& msg = m_listMessages.GetNext(pos);
        bResult = bResult & msg.writeMessageToFile(fileOutput);
    }

    //write environment variables if any
    pos = m_notProcessed.GetHeadPosition();

    while(pos != NULL)
    {
        string str = m_notProcessed.GetNext(pos);

        if(strcmp(str.substr(0, 3).c_str(), "EV_") == 0)
        {
            fileOutput << str.c_str() << endl;
        }
    }

    //Comments ----- Net
    string s_cmt;
    list<CComment>::iterator cmt;

    for(cmt=m_listComments[0].begin(); cmt!=m_listComments[0].end(); ++cmt)
    {
        fileOutput << "CM_ " << cmt->m_elementName.c_str();
        fileOutput << " " << cmt->m_comment.c_str();
    }

    //Comments ----- Node
    for (cmt=m_listComments[1].begin(); cmt!=m_listComments[1].end(); ++cmt)
    {
        fileOutput << "CM_ BU_ " << cmt->m_elementName.c_str();
        fileOutput << " " << cmt->m_comment.c_str();
    }

    //Comments ----- Mesg
    for (cmt=m_listComments[2].begin(); cmt!=m_listComments[2].end(); ++cmt)
    {
        fileOutput << "CM_ BO_ " << dec << cmt->m_msgID;
        fileOutput << " " << cmt->m_comment.c_str();
    }

    //Comments ----- Signal
    for (cmt=m_listComments[3].begin(); cmt!=m_listComments[3].end(); ++cmt)
        while(pos!=NULL)
        {
            fileOutput << "CM_ SG_ " << dec << cmt->m_msgID;
            fileOutput << " " << cmt->m_elementName.c_str();
            fileOutput << " " << cmt->m_comment.c_str();
        }

    //Param definition
    WriteParamToFile(fileOutput, m_listParameterArray[0]);
    WriteParamToFile(fileOutput, m_listParameterArray[1]);
    WriteParamToFile(fileOutput, m_listParameterArray[2]);
    WriteParamToFile(fileOutput, m_listParameterArray[3]);
    WriteParamToFile(fileOutput, m_listParameterArray[4]);
    WriteParamToFile(fileOutput, m_listParameterArray[5]);
    //Param Default values
    Write_DefVal_ToFile(fileOutput, m_listParameterArray[0]);
    Write_DefVal_ToFile(fileOutput, m_listParameterArray[1]);
    Write_DefVal_ToFile(fileOutput, m_listParameterArray[2]);
    Write_DefVal_ToFile(fileOutput, m_listParameterArray[3]);
    //Param Other values
    pos=m_listParameterArray[0].GetHeadPosition();

    while(pos!=NULL)
    {
        CParameters& rParam=m_listParameterArray[0].GetNext(pos);
        POSITION vPos=rParam.m_listParamValues[0].GetHeadPosition();

        while(vPos!=NULL)
        {
            CParameterValues& vParam=rParam.m_listParamValues[0].GetNext(vPos);
            vParam.WriteNetValuesToFile(fileOutput, rParam.m_ParamType, rParam.m_ParamName);
        }
    }

    pos=m_listParameterArray[1].GetHeadPosition();

    while(pos!=NULL)
    {
        CParameters& rParam=m_listParameterArray[1].GetNext(pos);
        POSITION vPos=rParam.m_listParamValues[1].GetHeadPosition();

        while(vPos!=NULL)
        {
            CParameterValues& vParam=rParam.m_listParamValues[1].GetNext(vPos);
            vParam.WriteNodeValuesToFile(fileOutput,rParam.m_ParamType,rParam.m_ParamName);
        }
    }

    pos=m_listParameterArray[2].GetHeadPosition();

    while(pos!=NULL)
    {
        CParameters& rParam=m_listParameterArray[2].GetNext(pos);
        POSITION vPos=rParam.m_listParamValues[2].GetHeadPosition();

        while(vPos!=NULL)
        {
            CParameterValues& vParam=rParam.m_listParamValues[2].GetNext(vPos);
            vParam.WriteMesgValuesToFile(fileOutput,rParam.m_ParamType,rParam.m_ParamName);
        }
    }

    pos=m_listParameterArray[3].GetHeadPosition();

    while(pos!=NULL)
    {
        CParameters& rParam=m_listParameterArray[3].GetNext(pos);
        POSITION vPos=rParam.m_listParamValues[3].GetHeadPosition();
        POSITION posMsg,posSig;

        while(vPos!=NULL)
        {
            CParameterValues& vParam=rParam.m_listParamValues[3].GetNext(vPos);
            posMsg = m_listMessages.GetHeadPosition();

            while(posMsg != NULL)
            {
                CMessage& msg = m_listMessages.GetNext(posMsg);

                if(msg.m_uiMsgID == vParam.m_MsgId)
                {
                    posSig = msg.m_listSignals.GetHeadPosition();

                    while(posSig != NULL)
                    {
                        CSignal& sig = msg.m_listSignals.GetNext(posSig);

                        if((sig.m_sName == vParam.m_SignalName) && (sig.m_uiError == CSignal::SIG_EC_NO_ERR))
                        {
                            vParam.WriteSigValuesToFile(fileOutput,rParam.m_ParamType,rParam.m_ParamName);
                            break;
                        }
                    }
                }
            }
        }
    }

    fileOutput << endl;
    fileOutput << endl;
    //BA_
    pos = m_notProcessed.GetHeadPosition();

    while(pos != NULL)
    {
        string& str = m_notProcessed.GetNext(pos);

        if(strcmp(str.substr(0, 3).c_str(), "BA_") == 0)
        {
            fileOutput << str.c_str() << endl;
        }
    }

    //VAL_
    pos = m_listMessages.GetHeadPosition();

    while(pos != NULL)
    {
        CMessage& msg = m_listMessages.GetNext(pos);
        POSITION possig = msg.m_listSignals.GetHeadPosition();

        while(possig != NULL)
        {
            CSignal& sig = msg.m_listSignals.GetNext(possig);

            if(sig.m_listValueDescriptor.IsEmpty() == 0 && sig.m_uiError == CSignal::SIG_EC_NO_ERR)
            {
                fileOutput << " VAL_ " << dec << msg.m_uiMsgID;
                fileOutput << " " << sig.m_sName.c_str();
                fileOutput << " ";
                CValueDescriptor temp;
                temp.writeValuDescToFile(fileOutput,sig.m_ucType,sig.m_listValueDescriptor);
            }
        }
    }

    //write SIG_VALTYPE_
    pos = m_listMessages.GetHeadPosition();

    while(pos != NULL)
    {
        CMessage& msg = m_listMessages.GetNext(pos);
        POSITION possig = msg.m_listSignals.GetHeadPosition();

        while(possig != NULL)
        {
            CSignal& sig = msg.m_listSignals.GetNext(possig);

            if(sig.m_ucType == 'F')
            {
                fileOutput << "SIG_VALTYPE_ " << dec << msg.m_uiMsgID;
                fileOutput << " " << sig.m_sName.c_str();
                fileOutput << " : 1;" << endl;
            }

            if(sig.m_ucType == 'D')
            {
                fileOutput << "SIG_VALTYPE_ " << dec << msg.m_uiMsgID;
                fileOutput << " " << sig.m_sName.c_str();
                fileOutput << " : 2;" << endl;
            }
        }
    }

    fileOutput << endl;
    return bResult;
}

/**
 * \brief     Create Log File
 * \param[in] fileLog Log file
 *
 * Logs the eror info in log file.
 */
void CConverter::CreateLogFile(fstream& fileLog)
{
    // write to the output file
    char acMsgLine[200];
    fileLog << "Conversion Error Log" << endl;
    fileLog << endl;
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
                string str;

                // for the first wrong signal, log the message details also
                if(acMsgLine[0] == '\0')
                {
                    fileLog << endl;
                    fileLog << "MSG_ID: " << dec << msg.m_uiMsgID;
                    fileLog << " \tMSG_NAME: " << msg.m_sName.c_str() << endl;
                }

                fileLog << "\tSignal Discarded SIG_NAME: " << sig.m_sName.c_str();
                sig.GetErrorString(str);
                fileLog << ", Reason: " << str << " " << endl;
            }
        }
    }
}

/**
 * \brief     Create Node List
 * \param[in] pcLine Input line
 *
 * Creates a list of nodes in the network.
 */
void CConverter::create_Node_List(char* pcLine)
{
    char* pcToken;
    // get the MSG ID
    pcToken = strtok(pcLine,",");

    while(pcToken)
    {
        string str = pcToken;
        m_listNode.AddTail(str);
        pcToken = strtok(NULL,",");
    }
}

/**
 * \brief     Decrypt Data
 * \param[in] m_notProcessed List of not processed strings
 *
 * Decrypts the not processed lines which are read from between
 * the tag START_NOT_PROCESSED and END_NOT_PROCESSED
 */
void CConverter::DecryptData(CList<string,string& > &m_notProcessed)
{
    //char c_str[defCON_MAX_LINE_LEN];
    string c_str;
    string str;
    POSITION prev_pos,pos = m_notProcessed.GetHeadPosition();

    while(pos != NULL)
    {
        prev_pos = pos;
        //read the string at the position
        str = m_notProcessed.GetNext(pos);
        //make a local copy
        c_str = str;

        for(int i=0; i < (int)c_str.length(); i++)
        {
            if ((c_str[i] >= 'a' && c_str[i] <= 'm') || (c_str[i] >= 'A' && c_str[i] <= 'M'))
            {
                c_str[i] = c_str[i] + 13;
            }
            else if ((c_str[i] >= 'n' && c_str[i] <= 'z') || (c_str[i] >= 'N' && c_str[i] <= 'Z'))
            {
                c_str[i] = c_str[i] - 13;
            }
        }

        str = c_str.c_str();
        //put it back at the same position
        m_notProcessed.SetAt(prev_pos,str);
    }
}
