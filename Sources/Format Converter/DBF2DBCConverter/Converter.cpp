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
 * \authors   RBIN/EBS1 - Mahesh.B.S, Padmaja.A
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Implementation of the CConverter class.
 */
/**
* \file       Converter.cpp
* \brief      implementation of the CConverter class.
* \authors    Mahesh.B.S, Padmaja.A
* \date       15/11/04
* \copyright  Copyright &copy; 2011 Robert Bosch Engineering and Business Solutions.  All rights reserved.
*/
#include "StdAfx.h"
#include <string.h>
#include "Converter.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
CStdioFile CConverter::fileLog;
bool CConverter::bLOG_ENTERED = false;
unsigned char CConverter::ucMsg_DLC = 8;

/**
* \brief      Constructor of CConverter
* \param[in]  None
* \param[out] None
* \return     void
* \authors    Mahesh.B.S
* \date       15.11.2004
*/
CConverter::CConverter()
{
    m_uiResultCode = CON_RC_NOERROR;
}

/**
* \brief      Destructort of CConverter
* \param[in]  None
* \param[out] None
* \return     void
* \authors    Mahesh.B.S
* \date       15.11.2004
*/
CConverter::~CConverter()
{
}

/**
* \brief      This is the baisc function which is to be called
to convert any given CANMon file to a CANoe file
* \param[in]  CString sCanMonFile,CString sCanoeFile
* \param[out] None
* \return     unsigned int
* \authors    Mahesh.B.S
* \date       15.11.2004
*/
unsigned int CConverter::Convert(CString sCanMonFile,CString sCanoeFile)
{
    CStdioFile fileInput,fileOutput;
    char acLine[defCON_MAX_LINE_LEN]; // I don't expect one line to be more than this
    if(!fileInput.Open(sCanMonFile,CFile::modeRead))
    {
        return SetResultCode(CON_RC_FILEOPEN_ERROR_INFILE);
    }

    // first line of input file starts with header followed by a version no, else file format error
    if(fileInput.ReadString(acLine,defCON_MAX_LINE_LEN) == NULL)
    {
        // eof file reached without reading anything
        fileInput.Close();
        return SetResultCode(CON_RC_FORMAT_ERROR_INFILE);
    }
    else // if something was read
    {
        fileInput.ReadString(acLine,defCON_MAX_LINE_LEN);
        fileInput.ReadString(acLine,defCON_MAX_LINE_LEN);
        // verify the format
        /*if(strncmp(acLine,"[DATABASE_VERSION] ",19) != 0)
        {
        fileInput.Close();
        return SetResultCode(CON_RC_FORMAT_ERROR_INFILE);
        }*/
    }
    //Create log file
    CConverter::bLOG_ENTERED = false;
    CString sLogFile = sCanoeFile.Left(sCanoeFile.GetLength()-4);
    sLogFile += ".log";
    m_omLogFilePath = sLogFile;
    if(!fileLog.Open(sLogFile,CFile::modeWrite | CFile::modeCreate))
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
    if(!fileOutput.Open(sCanoeFile,CFile::modeWrite | CFile::modeCreate))
    {
        // if output file cannot be opened the close the input file
        // and return the error code
        fileInput.Close();
        return SetResultCode(CON_RC_FILEOPEN_ERROR_OUTFILE);
    }

    bool bRes = WriteToOutputFile(fileOutput);

    fileInput.Close();
    fileOutput.Close();
    //If any error log it in the log file.
    if(!bRes)
    {
        CreateLogFile(fileLog);
        fileLog.Close();
        return SetResultCode(CON_RC_COMPLETED_WITH_ERROR);
        CConverter::bLOG_ENTERED = true;
    }
    fileLog.Close();
    //If log file is empty delete it
    if(!CConverter::bLOG_ENTERED)
    {
        DeleteFile(sLogFile);
    }
    else
        return SetResultCode(CON_RC_COMPLETED_WITH_ERROR);
    return m_uiResultCode;
}

/**
* \brief      Set the result code for the convertor class.This function
is called only on error
* \param[in]  unsigned int uiCode
* \param[out] None
* \return     unsigned int
* \authors    Mahesh.B.S
* \date       15.11.2004
*/
unsigned int CConverter::SetResultCode(unsigned int uiCode)
{
    return (m_uiResultCode = uiCode);
}

const char* CConverter::m_pacResultStrings[] =
{
    "Conversion completed.\n",
    "Conversion completed with errors. See log file.\n",
    "Conversion aborted. Error opening input file.",
    "Conversion aborted. Error creating output file.",
    "Conversion aborted. Error with input file format.",
    "Conversion aborted. Error creating log file.\n"
};

/**
* \brief      Returns the error string.
* \param[in]  None
* \param[out] None
* \return     const char*
* \authors    Mahesh.B.S
* \date       15.11.2004
*/
const char* CConverter::GetResultString()
{
    return m_pacResultStrings[m_uiResultCode];
}

/**
* \brief      This function will parse the input file and line by line
and generates  a list of message,signal,value table,comments,etc
* \param[in]  CStdioFile& fileInput
* \param[out] None
* \return     void
* \authors    Padmaja.A.,Mahesh.B.S.
* \date       15.11.2004
*/
void CConverter::GenerateMessageList(CStdioFile& fileInput)
{
    char acLine[defCON_MAX_LINE_LEN]; // I don't expect one line to be more than this
    bool valTab = false;
    CMessage &msg();
    CSignal &sig();
    POSITION posMsg,posSig;
    // parsing the input file

    while(fileInput.ReadString(acLine,defCON_MAX_LINE_LEN))
    {

        char *pcToken, *pcLine;

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
                    posMsg = m_listMessages.AddTail(msg);
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
                fileInput.ReadString(acLine,defCON_MAX_LINE_LEN);
                pcToken = strtok(pcLine," ");
                while(strcmp(acLine,"[END_SIG_LIST]\n") != 0)
                {
                    CSignal sig;
                    pcToken = strtok(pcLine," ");
                    if(strcmp(pcToken,"[START_SIGNALS]") == 0)
                    {
                        sig.Format(pcLine + strlen(pcToken)+1);
                        sig.m_ucStartBit = 0;
                        CMessage &inMsg = m_listMessages.GetHead();
                        posSig = inMsg.m_listSignals.AddHead(sig);
                    }
                    else if(strcmp(pcToken,"[VALUE_DESCRIPTION]") == 0)
                    {
                        CValueDescriptor val;
                        val.Format(pcLine + strlen(pcToken)+1);
                        CMessage &inMsg = m_listMessages.GetHead();
                        CSignal& inSig = inMsg.m_listSignals.GetAt(posSig);
                        inSig.m_listValueDescriptor.AddHead(val);
                    }
                    fileInput.ReadString(acLine,defCON_MAX_LINE_LEN);
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
                while(strcmp((fileInput.ReadString(acLine,defCON_MAX_LINE_LEN)),"[END_VALUE_TABLE]\n")!=0)
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
                while(strcmp((fileInput.ReadString(acLine,defCON_MAX_LINE_LEN)),"[END_PARAM]\n")!=0)
                {
                    pcLine = acLine;
                    CParameters rParam;
                    if(strcmp(pcLine,"[START_PARAM_NET]\n")==0)
                    {
                        while(strcmp((fileInput.ReadString(acLine,defCON_MAX_LINE_LEN)),"[END_PARAM_NET]\n")!=0)
                        {
                            pcLine = acLine;
                            rParam.Format_ParamDef(pcLine,0);
                            m_listParameterArray[0].AddTail(rParam);
                        }
                    }
                    else if(strcmp(pcToken,"[START_PARAM_NODE]\n")==0)
                    {
                        while(strcmp((fileInput.ReadString(acLine,defCON_MAX_LINE_LEN)),"[END_PARAM_NODE]\n")!=0)
                        {
                            pcLine = acLine;
                            rParam.Format_ParamDef(pcLine,1);
                            m_listParameterArray[1].AddTail(rParam);
                        }
                    }
                    else if(strcmp(pcToken,"[START_PARAM_MSG]\n")==0)
                    {
                        while(strcmp((fileInput.ReadString(acLine,defCON_MAX_LINE_LEN)),"[END_PARAM_MSG]\n")!=0)
                        {
                            pcLine = acLine;
                            rParam.Format_ParamDef(pcLine,2);
                            m_listParameterArray[2].AddTail(rParam);
                        }
                    }
                    else if(strcmp(pcToken,"[START_PARAM_SIG]\n")==0)
                    {
                        while(strcmp((fileInput.ReadString(acLine,defCON_MAX_LINE_LEN)),"[END_PARAM_SIG]\n")!=0)
                        {
                            pcLine = acLine;
                            rParam.Format_ParamDef(pcLine,3);
                            m_listParameterArray[3].AddTail(rParam);
                        }
                    }
                    else if(strcmp(pcToken,"[START_PARAM_NODE_RX_SIG]\n")==0)
                    {
                        while(strcmp((fileInput.ReadString(acLine,defCON_MAX_LINE_LEN)),"[END_PARAM_NODE_RX_SIG]\n")!=0)
                        {
                            pcLine = acLine;
                            rParam.Format_ParamDef(pcLine,4);
                            m_listParameterArray[4].AddTail(rParam);
                        }
                    }
                    else if(strcmp(pcToken,"[START_PARAM_NODE_TX_MSG]\n")==0)
                    {
                        while(strcmp((fileInput.ReadString(acLine,defCON_MAX_LINE_LEN)),"[END_PARAM_NODE_TX_MSG]\n")!=0)
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
                while(strcmp((fileInput.ReadString(acLine,defCON_MAX_LINE_LEN)),"[END_PARAM_VAL]\n")!=0)
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
                while(strcmp((fileInput.ReadString(acLine,defCON_MAX_LINE_LEN)),"[END_DESC]\n")!=0)
                {
                    pcLine = acLine;
                    CComment tCmt;
                    if(strcmp(pcLine,"[START_DESC_NET]\n")==0)
                    {
                        tCmt.Format_netComment(fileInput,m_listComments[0]);
                    }
                    else if(strcmp(pcLine,"[START_DESC_NODE]\n")==0)
                    {
                        tCmt.Format_nodeComment(fileInput,m_listComments[1]);
                    }
                    else if(strcmp(pcLine,"[START_DESC_MSG]\n")==0)
                    {
                        tCmt.Format_msgComment(fileInput,m_listComments[2]);
                    }
                    else if(strcmp(pcLine,"[START_DESC_SIG]\n")==0)
                    {
                        tCmt.Format_sigComment(fileInput,m_listComments[3]);
                    }
                }

            }

            else if(strcmp(pcToken,"[START_NOT_PROCESSED]\n") == 0)
            {
                fileInput.ReadString(acLine,defCON_MAX_LINE_LEN);
                while(strcmp(pcToken,"[END_NOT_PROCESSED]\n") != 0)
                {
                    CString str = acLine;
                    m_notProcessed.AddTail(str);
                    fileInput.ReadString(acLine,defCON_MAX_LINE_LEN);
                }
                continue;
            }
            //if any other tag continue
            else
                continue;
        }
    }
    return ;
}

/**
* \brief      Validates the message list and set the error in each signal
if present
* \param[in]  void
* \param[out] None
* \return     void
* \authors    Mahesh.B.S
* \date       15.11.2002
*/
void CConverter::ValidateMessageList()
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

    return ;
}

/**
* \brief      Writes all the data to the output file in CANoe format
* \param[in]  CStdioFile& fileOutput
* \param[out] None
* \return     bool
* \authors    Padmaja.A.,Mahesh.B.S.
* \date       15.11.2002
*/
bool CConverter::WriteToOutputFile(CStdioFile& fileOutput)
{
    bool bResult = true;
    char acLine[defCON_MAX_LINE_LEN]; // I don't expect one line to be more than this
    // write to the output file
    // write header

    fileOutput.WriteString("VERSION \"\"\n\nBS_:");
    fileOutput.WriteString("\n");
    fileOutput.WriteString("\nBU_:");
    //write all nodes
    POSITION pos = m_listNode.GetHeadPosition();
    while(pos != NULL)
    {
        fileOutput.WriteString(" ");
        CString &node = m_listNode.GetNext(pos);
        fileOutput.WriteString(node);
    }
    fileOutput.WriteString("\n");

    //Value Table
    CValueTable temp_vtab;
    temp_vtab.writeValueTabToFile (fileOutput,m_vTab);
    fileOutput.WriteString("\n");

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
        CString &str = m_notProcessed.GetNext(pos);
        if(str.Left(3) == "EV_")
        {
            fileOutput.WriteString(str);
            fileOutput.WriteString("\n");
        }
    }

    //Comments ----- Net
    CString s_cmt;
    pos=m_listComments[0].GetHeadPosition();
    while(pos!=NULL)
    {
        CComment &cmt = m_listComments[0].GetNext(pos);
        s_cmt="CM_ "+cmt.m_elementName+" "+cmt.m_comment;
        fileOutput.WriteString(s_cmt);
    }

    //Comments ----- Node
    pos=m_listComments[1].GetHeadPosition();
    while(pos!=NULL)
    {
        CComment &cmt = m_listComments[1].GetNext(pos);
        s_cmt = "CM_ BU_ "+cmt.m_elementName+" "+cmt.m_comment;
        fileOutput.WriteString(s_cmt);
    }
    //Comments ----- Mesg
    pos=m_listComments[2].GetHeadPosition();
    while(pos!=NULL)
    {
        CComment &cmt = m_listComments[2].GetNext(pos);
        char c_msgID[defCON_MAX_MSGID_LEN + 10];
        sprintf(c_msgID,"CM_ BO_ %u",cmt.m_msgID);
        s_cmt = c_msgID;
        s_cmt = s_cmt + " ";
        s_cmt = s_cmt  + cmt.m_comment;
        fileOutput.WriteString(s_cmt);

    }
    //Comments ----- Signal
    pos=m_listComments[3].GetHeadPosition();

    while(pos!=NULL)
    {
        CComment &cmt = m_listComments[3].GetNext(pos);
        char c_msgID[defCON_MAX_MSGID_LEN + defCON_MAX_MSGN_LEN + 10];
        sprintf(c_msgID,"CM_ SG_ %u %s",cmt.m_msgID,cmt.m_elementName);
        s_cmt = c_msgID;
        s_cmt = s_cmt + " ";
        s_cmt = s_cmt  + cmt.m_comment;
        fileOutput.WriteString(s_cmt);
    }


    //Param definition
    WriteParamToFile(fileOutput,m_listParameterArray[0]);
    WriteParamToFile(fileOutput,m_listParameterArray[1]);
    WriteParamToFile(fileOutput,m_listParameterArray[2]);
    WriteParamToFile(fileOutput,m_listParameterArray[3]);
    WriteParamToFile(fileOutput,m_listParameterArray[4]);
    WriteParamToFile(fileOutput,m_listParameterArray[5]);

    //Param Default values
    Write_DefVal_ToFile(fileOutput,m_listParameterArray[0]);
    Write_DefVal_ToFile(fileOutput,m_listParameterArray[1]);
    Write_DefVal_ToFile(fileOutput,m_listParameterArray[2]);
    Write_DefVal_ToFile(fileOutput,m_listParameterArray[3]);

    //Param Other values
    pos=m_listParameterArray[0].GetHeadPosition();
    while(pos!=NULL)
    {
        CParameters& rParam=m_listParameterArray[0].GetNext(pos);
        POSITION vPos=rParam.m_listParamValues[0].GetHeadPosition();
        while(vPos!=NULL)
        {
            CParameterValues& vParam=rParam.m_listParamValues[0].GetNext(vPos);
            vParam.WriteNetValuesToFile(fileOutput,rParam.m_ParamType,rParam.m_ParamName);
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
                CMessage &msg = m_listMessages.GetNext(posMsg);
                if(msg.m_uiMsgID == vParam.m_MsgId)
                {
                    posSig = msg.m_listSignals.GetHeadPosition();
                    while(posSig != NULL)
                    {
                        CSignal &sig = msg.m_listSignals.GetNext(posSig);
                        if(strcmp(sig.m_sName,vParam.m_SignalName) == 0 && sig.m_uiError == CSignal::SIG_EC_NO_ERR )
                        {
                            vParam.WriteSigValuesToFile(fileOutput,rParam.m_ParamType,rParam.m_ParamName);
                            break;
                        }
                    }
                }
            }
        }
    }

    pos = m_notProcessed.GetHeadPosition();
    while(pos != NULL)
    {
        CString &str = m_notProcessed.GetNext(pos);
        if(str.Left(3) == "BA_")
        {
            fileOutput.WriteString(str);
            fileOutput.WriteString("\n");
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
                sprintf(acLine," VAL_ %u %s ",msg.m_uiMsgID,sig.m_sName);
                fileOutput.WriteString(acLine);
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
                sprintf(acLine," SIG_VALTYPE_ %u %s : 1;\n",msg.m_uiMsgID,sig.m_sName);
                fileOutput.WriteString(acLine);
            }
            if(sig.m_ucType == 'D')
            {
                sprintf(acLine," SIG_VALTYPE_ %u %s : 2;\n",msg.m_uiMsgID,sig.m_sName);
                fileOutput.WriteString(acLine);
            }
        }
    }
    return bResult;
}

/**
* \brief      Logs the eror info in log file.
* \param[in]  CStdioFile& fileOutput
* \param[out] None
* \return     void
* \authors    Mahesh.B.S
* \date       15.11.2004
*/
void CConverter::CreateLogFile(CStdioFile &fileLog)
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
                    sprintf(acMsgLine,"\nMSG_ID: %u \tMSG_NAME: %s\n", msg.m_uiMsgID,msg.m_sName);
                    fileLog.WriteString(acMsgLine);
                }
                sprintf(acLine,"\tSignal Discarded SIG_NAME: %s, REASON: %s \n",sig.m_sName,sig.GetErrorString());
                fileLog.WriteString(acLine);
            }
        }
    }
    return ;
}

/**
* \brief      creates a list of nodes in the network
* \param[in]  char *pcLine
* \param[out] None
* \return     void
* \authors    Mahesh.B.S
* \date       15.11.2004
*/
void CConverter::create_Node_List(char *pcLine)
{
    char* pcToken;
    // get the MSG ID
    pcToken = strtok(pcLine,",");
    while(pcToken)
    {
        CString str = pcToken;
        if(str.GetLength() > defCON_MAX_MSGN_LEN)
            Truncate_str("Node name",str,true);

        m_listNode.AddTail(str);
        pcToken = strtok(NULL,",");
    }
}

/**
* \brief      Decrypts the not processed lines which are read from between
the tag [START_NOT_PROCESSED] and [END_NOT_PROCESSED]
* \param[in]  CList<CString,CString& > &m_notProcessed
* \param[out] None
* \return     void
* \authors    Mahesh.B.S
* \date       15.11.2004
*/
void CConverter::DecryptData(CList<CString,CString& > &m_notProcessed)
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
        strncpy(c_str, str, sizeof(c_str));
        for(int i=0; i < (int)strlen(c_str); i++)
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


