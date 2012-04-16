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
 * \brief     Implementation file for CConverter class
 * \author    Amit Ranjan
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Implementation file for CConverter class
 */


#include "StdAfx.h"
//#include "CANDBConverter.h"
#include "Signal.h"
#include "Converter.h"
#include "ConstString.h"



#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define defCON_MAX_LINE_LEN 1024

/**
 * Construction
 */
CConverter::CConverter()
{
    m_uiResultCode = CON_RC_NOERROR;
}

/**
 * Destruction
 */
CConverter::~CConverter()
{

    m_listMessages.RemoveAll();

}

const char CConverter::m_accHeader[] =
    "//******************************BUSMASTER Messages and signals Database ******************************//\n";

unsigned int CConverter::Convert(CString sCanoeFile)
{
    fstream fileInput,fileOutput;
    char acLine[defCON_MAX_LINE_LEN]; // I don't expect one line to be more than this

	fileInput.open(sCanoeFile, fstream::in);
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
    else // if something was read
    {
        // verify the format
        if(strncmp(acLine, "VERSION ",8) != 0)
        {
            fileInput.close();
            return SetResultCode(CON_RC_FORMAT_ERROR_INFILE);
        }
    }

    // Generate the list of messages

    GenerateMessageList(fileInput);

    // All information gathered, validate and update if necessary
    // Make appropriate changes in the contents of the list

    ValidateMessageList();

    // the format is OK then open the output file



//	bool bRes = WriteToOutputFile(fileOutput);
    fileInput.close();
//	fileOutput.close();
    BOOL bRes = FALSE;
    if(!bRes)
    {
        CString sLogFile = sCanoeFile.Left(sCanoeFile.GetLength()-4);
        sLogFile += ".log";
        fstream fileLog;
		fileLog.open(sLogFile, fstream::out);
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

    return 1;
}

void CConverter::GetResultString(string &str)
{
	switch(m_uiResultCode) {
		case 0:
			str = "Conversion completed.";
			break;
		case 1:
			str = "Conversion completed with errors. See log file.";
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

unsigned int CConverter::SetResultCode(unsigned int uiCode)
{
    return (m_uiResultCode = uiCode);
}

// parses the full message list and makes any changes required for
// BUSMASTER format
// Message Data format = DF of first signal, discard all signals of other type

bool CConverter::WriteToOutputFile(CString sCanMonFile)
{
    bool bResult = true;
    char acLine[defCON_MAX_LINE_LEN]; // I don't expect one line to be more than this
    fstream fileOutput;
    // write to the output file
    // write header
//	FILE *message = fopen("d:\\message.txt","w");
	fileOutput.open(sCanMonFile, fstream::out);
	if(!fileOutput.is_open())
    {
        // if output file cannot be opened the close the input file
        // and return the error code
        return SetResultCode(CON_RC_FILEOPEN_ERROR_OUTFILE);
    }

    fileOutput << CConverter::m_accHeader;
    fileOutput << endl;
    //Version no.
    fileOutput << defSTR_DB_VER " " defSTR_VER_NO;
    fileOutput << endl;
    // number of messages
    sprintf(acLine, "\n[NUMBER_OF_MESSAGES] %d\n\n",m_listMessages.GetCount());
    fileOutput << acLine;

    // write all messages, signals and value descriptors
    POSITION pos = m_listMessages.GetHeadPosition();
    while(pos != NULL)
    {
        CMessage& msg = m_listMessages.GetNext(pos);
        // MSG,MSGID,MSG_LENGTH,NO_OF_SIGNALS,DATA_FORMAT,FRAME_FORMAT

        sprintf(acLine,"[START_MSG] %s,%u,%u,%u,%c,%c\n",msg.m_acName,msg.m_uiMsgID,msg.m_ucLength,msg.m_ucNumOfSignals,msg.m_cDataFormat,msg.m_cFrameFormat);
        fileOutput << acLine;

//		fprintf(message,"%s,%u,%u\n",msg.m_acName,msg.m_uiMsgID,msg.m_ucLength);
        POSITION posSig = msg.m_listSignals.GetHeadPosition();
        while(posSig != NULL)
        {
            // SIG_NAME,SIG_LENGTH,WHICH_BYTE_IN_MSG,START_BIT,SIG_TYPE,MAX_VAL,MIN_VAL,SIG_DATA_FORMAT,SIG_OFFSET,SIG_FACTOR,SIG_UNIT
            CSignal& sig = msg.m_listSignals.GetNext(posSig);
            // write signal only if it is valid
            if(sig.m_uiError == CSignal::SIG_EC_NO_ERR)
            {
                // For signal having motoroal format, the message length could be less
                // then eight byte. so in that case the whichByte needs to be shifted
                // accordingly.
                if(msg.m_cDataFormat == '0')
                {
                    sig.m_ucWhichByte = sig.m_ucWhichByte - (8 - msg.m_ucLength);
                }
                switch(sig.m_ucType)
                {
                    case CSignal::SIG_TYPE_BOOL:
                    case CSignal::SIG_TYPE_UINT:
                        sprintf(acLine,"[START_SIGNALS] %s,%u,%u,%u,%c,%u,%u,%c,%f,%f,%s,%s,%s\n",
                                sig.m_acName,sig.m_ucLength,sig.m_ucWhichByte,sig.m_ucStartBit,sig.m_ucType,
                                sig.m_MaxValue.uiValue,sig.m_MinValue.uiValue,sig.m_ucDataFormat,
                                sig.m_fOffset,sig.m_fScaleFactor,sig.m_acUnit,sig.m_acMultiplex,sig.m_rxNode);

                        break;

                    case CSignal::SIG_TYPE_INT:
                        sprintf(acLine,"[START_SIGNALS] %s,%u,%u,%u,%c,%d,%d,%c,%f,%f,%s,%s,%s\n",
                                sig.m_acName,sig.m_ucLength,sig.m_ucWhichByte,sig.m_ucStartBit,sig.m_ucType,
                                sig.m_MaxValue.iValue,sig.m_MinValue.iValue,sig.m_ucDataFormat,
                                sig.m_fOffset,sig.m_fScaleFactor,sig.m_acUnit,sig.m_acMultiplex,sig.m_rxNode);

                        break;

                        // Enable these when FLOAT and DOUBLE are supported
                        /*
                        case CSignal::SIG_TYPE_FLOAT:
                        	sprintf(acLine,"[START_SIGNALS] %s,%u,%u,%u,%c,%f,%f,%c,%f,%f,%s\n",
                        	sig.m_acName,sig.m_ucLength,sig.m_ucWhichByte,sig.m_ucStartBit,sig.m_ucType,
                        	sig.m_MaxValue.fValue,sig.m_MinValue.fValue,sig.m_ucDataFormat,
                        	sig.m_fOffset,sig.m_fScaleFactor,sig.m_acUnit);

                        	break;

                        case CSignal::SIG_TYPE_DOUBLE:
                        	sprintf(acLine,"[START_SIGNALS] %s,%u,%u,%u,%c,%f,%f,%c,%f,%f,%s\n",
                        	sig.m_acName,sig.m_ucLength,sig.m_ucWhichByte,sig.m_ucStartBit,sig.m_ucType,
                        	sig.m_MaxValue.dValue,sig.m_MinValue.dValue,sig.m_ucDataFormat,
                        	sig.m_fOffset,sig.m_fScaleFactor,sig.m_acUnit);

                        	break;
                        */

                    case CSignal::SIG_TYPE_INT64:
                        sprintf(acLine,"[START_SIGNALS] %s,%u,%u,%u,%c,%I64d,%I64d,%c,%f,%f,%s,%s,%s\n",
                                sig.m_acName,sig.m_ucLength,sig.m_ucWhichByte,sig.m_ucStartBit,'I'/*sig.m_ucType*/,
                                sig.m_MaxValue.dValue,sig.m_MinValue.dValue,sig.m_ucDataFormat,
                                sig.m_fOffset,sig.m_fScaleFactor,sig.m_acUnit,sig.m_acMultiplex,sig.m_rxNode);
                        break;

                    case CSignal::SIG_TYPE_UINT64:
                        sprintf(acLine,"[START_SIGNALS] %s,%u,%u,%u,%c,%I64u,%I64u,%c,%f,%f,%s,%s,%s\n",
                                sig.m_acName,sig.m_ucLength,sig.m_ucWhichByte,sig.m_ucStartBit,'U'/*sig.m_ucType*/,
                                sig.m_MaxValue.dValue,sig.m_MinValue.dValue,sig.m_ucDataFormat,
                                sig.m_fOffset,sig.m_fScaleFactor,sig.m_acUnit,sig.m_acMultiplex,sig.m_rxNode);
                        break;

                    default:
                        break;
                }

                fileOutput << acLine;

                // now write value descriptors for this signal
                POSITION posValDesc = sig.m_listValueDescriptor.GetHeadPosition();
                while(posValDesc != NULL)
                {
                    CValueDescriptor& rValDesc = sig.m_listValueDescriptor.GetNext(posValDesc);
                    switch(sig.m_ucType)
                    {
                        case CSignal::SIG_TYPE_BOOL:
                        case CSignal::SIG_TYPE_UINT:
                            sprintf(acLine,"[VALUE_DESCRIPTION] %s,%u\n",rValDesc.m_acDescriptor,rValDesc.m_value.uiValue);
                            break;

                        case CSignal::SIG_TYPE_INT:
                            sprintf(acLine,"[VALUE_DESCRIPTION] %s,%d\n",rValDesc.m_acDescriptor,rValDesc.m_value.iValue);
                            break;

                            // when FLOAT and DOUBLE are supported enable this
                            /*
                            case CSignal::SIG_TYPE_FLOAT:
                            	sprintf(acLine,"[VALUE_DESCRIPTION] %s,%f\n",rValDesc.m_acDescriptor,rValDesc.m_value.fValue);
                            	break;

                            case CSignal::SIG_TYPE_DOUBLE:
                            	sprintf(acLine,"[VALUE_DESCRIPTION] %s,%f\n",rValDesc.m_acDescriptor,rValDesc.m_value.dValue);
                            	break;
                            */

                        case CSignal::SIG_TYPE_INT64:
                            sprintf(acLine,"[VALUE_DESCRIPTION] %s,%I64d\n",rValDesc.m_acDescriptor,rValDesc.m_value.i64Value);
                            break;

                        case CSignal::SIG_TYPE_UINT64:
                            sprintf(acLine,"[VALUE_DESCRIPTION] %s,%I64u\n",rValDesc.m_acDescriptor,rValDesc.m_value.ui64Value);
                            break;

                        default:
                            break;
                    }

                    fileOutput << acLine;
                }
            }
            else
            {
                bResult = false;
            }
        }

//		fclose(message);
        fileOutput << "[END_MSG]" << endl << endl;
    }

    fileOutput.close();

    return bResult;
}



void CConverter::ValidateMessageList()
{
    POSITION pos = m_listMessages.GetHeadPosition();
    while(pos != NULL)
    {
        CMessage& rMsg = m_listMessages.GetNext(pos);
        unsigned char ucDataFormat = 0;
        unsigned int uiResult;

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
            if(uiResult == CSignal::SIG_EC_NO_ERR)
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
        }

        posSig = rMsg.m_listSignals.GetHeadPosition();
        __int64 n64Matrix = 0;

        while(posSig != NULL)
        {
            CSignal& rSig = rMsg.m_listSignals.GetNext(posSig);
            __int64 n64Mask = ( 1 << rSig.m_ucLength ) - 1;
            n64Mask <<=
                ( rSig.m_ucWhichByte - 1 ) * 8 + rSig.m_ucStartBit;
            if( n64Matrix & n64Mask )
            {
                rSig.m_uiError = CSignal::SIG_EC_OVERLAP;
                rMsg.m_ucNumOfSignals--;
            }
            else
            {
                n64Matrix |= n64Mask;
            }
        }
    }
}

// generates the message list with signals and value descriptors
void CConverter::GenerateMessageList(fstream& fileInput)
{
    char acLine[defCON_MAX_LINE_LEN]; // I don't expect one line to be more than this
    // Flag to skip signal parsing of independent messages
    BOOL bSkipSignalParsing = FALSE;
    // parsing the input file
    while(fileInput.getline(acLine, defCON_MAX_LINE_LEN))
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
                if( msg.m_uiMsgID != 0xc0000000 ) // Vector independent message
                {
                    bSkipSignalParsing = FALSE;
                    // add the new message to the list
                    m_listMessages.AddTail(msg);
                }
                else
                {
                    bSkipSignalParsing = TRUE;
                }
            }

            // signal
            else if(strcmp(pcToken,"SG_") == 0)
            {
                if( bSkipSignalParsing == FALSE )
                {
                    CSignal sig;
                    sig.Format(pcLine + strlen(pcToken)+1);

                    POSITION pos = m_listMessages.GetTailPosition();
                    CMessage& msg = m_listMessages.GetAt(pos);

                    msg.m_listSignals.AddTail(sig);
                    // this signal should belong to the last message
                    msg.m_ucNumOfSignals++; // increment the signal count
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

                unsigned int id = (unsigned int)atoi(pcToken);
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
                            //	char *pcToken1;
                            CSignal& rSig =  rMsg.m_listSignals.GetNext(posSig);
                            if(strcmp(rSig.m_acName.c_str(),pcToken) == 0)
                            {
                                rSig.AddValueDescriptors(pcLine + strlen(pcToken) + 1);
                                break; // if we got the signal we wanted
                            }
                        }
                        break; // we got the message we wanted
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
                            if(strcmp(rSig.m_acName.c_str(),pcToken) == 0)
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

            // anything else skip
            else
            {
                continue;
            }
        }
    }
}


// Create a log file of errors encountered
// file name  = Inputfile with extension changed to ".log"
void CConverter::CreateLogFile(fstream& fileLog)
{
    char acLine[defCON_MAX_LINE_LEN]; // I don't expect one line to be more than this
    // write to the output file
    char acMsgLine[200];

    sprintf(acLine,"Conversion Error Log \n\n");
    fileLog << acLine;

    // write all signals & messages which encountered errors
    // MSG_ID ,MAG_NAME,SIG_NAME:
    POSITION pos = m_listMessages.GetHeadPosition();
    while(pos != NULL)
    {
        acMsgLine[0] = '\0';
        CMessage& msg = m_listMessages.GetNext(pos);
        POSITION posSig = msg.m_listSignals.GetHeadPosition();
        while(posSig != NULL)
        {
            CSignal& sig = msg.m_listSignals.GetNext(posSig);
            // write signal only if it is valid
            if(sig.m_uiError != CSignal::SIG_EC_NO_ERR)
            {
				string str;
                // for the first wrong signal, log the message details also
                if(acMsgLine[0] == '\0')
                {
                    sprintf(acMsgLine,"\nMSG_ID: %u\tMSG_NAME: %s\n", msg.m_uiMsgID,msg.m_acName);
                    fileLog << acMsgLine;
                }
				sig.GetErrorString(str);
                sprintf(acLine,"\tDiscarded SIG_NAME: %s, Reason: %s\n",sig.m_acName,str.c_str());
                fileLog << acLine;
            }
        }
    }
}

CMessage CConverter::ouFindMessage(CString omStrMsgName)
{
    CMessage omTemp;
    POSITION pos = m_listMessages.GetHeadPosition();

    // iterate through the list
    for (int index = 0; index < m_listMessages.GetCount(); index++)
    {
        // store the data
        omTemp = m_listMessages.GetNext(pos);
        // compare with message name
        if(omStrMsgName == omTemp.m_acName.c_str())
        {
            // send the result
            return omTemp;
        }
    }
    // if not found then
    omTemp.m_uiMsgID = 0xffffffff;
    return omTemp;

}


CMessage CConverter::ouFindMessage(UINT nMsgID)
{
    POSITION pos = m_listMessages.GetHeadPosition();
    CMessage omTemp;

    // iterate through the list
    for (int index = 0; index < m_listMessages.GetCount(); index++)
    {
        // store the data
        omTemp = m_listMessages.GetNext(pos);
        // compare with message name
        if( nMsgID == omTemp.m_uiMsgID)
        {
            // send the result
            return omTemp;
        }

    }
    // if not found
    omTemp.m_uiMsgID = 0xffffffff;
    return omTemp;
}
